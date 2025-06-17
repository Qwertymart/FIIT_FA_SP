#include "../include/allocator_buddies_system.h"
#include <new>
#include <cstring>
#include <iostream>

allocator_buddies_system::allocator_buddies_system(
        size_t space_size_power_of_two,
        std::pmr::memory_resource* parent_allocator,
        logger* log,
        fit_mode mode)
{
    if (space_size_power_of_two < min_k) {
        throw std::invalid_argument("space_size must be at least " + std::to_string(min_k));
    }

    size_t real_size = (1 << space_size_power_of_two) + allocator_metadata_size;

    if (parent_allocator == nullptr)
    {
        try
        {
            _trusted_memory = ::operator new(real_size); //глобальный new
        } catch (std::bad_alloc& ex)
        {
            error_with_guard("Bad allocation memory");
            throw;
        }
    } else
    {
        try
        {
            _trusted_memory = parent_allocator->allocate(real_size, 1);
        }
        catch (std::bad_alloc& ex)
        {
            error_with_guard("Bad allocation memory from parent allocator");
            throw;
        }
    }

    fill_allocator_fields(space_size_power_of_two, parent_allocator, log, mode);
}


allocator_buddies_system::~allocator_buddies_system() {
    trace_with_guard( "~allocator_buddies_system");

    if (_trusted_memory) {
        get_mutex().~mutex();
        ::operator delete(_trusted_memory);
    }
}

allocator_buddies_system::allocator_buddies_system(allocator_buddies_system&& other) noexcept {
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;
}

allocator_buddies_system& allocator_buddies_system::operator=(allocator_buddies_system&& other) noexcept {
    if (this != &other) {
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }
    return *this;
}

allocator_buddies_system& allocator_buddies_system::operator=(const allocator_buddies_system& other) {
    if (this != &other) {
        if (_trusted_memory) {
            ::operator delete(_trusted_memory);
        }

        if (other._trusted_memory) {
            size_t k = *reinterpret_cast<unsigned char*>(
                    static_cast<char*>(other._trusted_memory) + sizeof(logger*) + sizeof(allocator_dbg_helper*) + sizeof(fit_mode));
            size_t size = (1 << k) + allocator_metadata_size;

            _trusted_memory = ::operator new(size);
            std::memcpy(_trusted_memory, other._trusted_memory, size);
        } else {
            _trusted_memory = nullptr;
        }
    }
    return *this;
}


// заполнение меты
void allocator_buddies_system::fill_allocator_fields(
        size_t space_size,
        std::pmr::memory_resource* parent_allocator,
        logger* log,
        fit_mode mode)
{
    void* mem = _trusted_memory;

    *reinterpret_cast<class logger**>(mem) = log;
    mem = static_cast<char*>(mem) + sizeof(class logger*);

    *reinterpret_cast<std::pmr::memory_resource**>(mem) = parent_allocator;
    mem = static_cast<char*>(mem) + sizeof(std::pmr::memory_resource*);

    *reinterpret_cast<fit_mode*>(mem) = mode;
    mem = static_cast<char*>(mem) + sizeof(fit_mode);

    *reinterpret_cast<unsigned char*>(mem) = static_cast<unsigned char>(space_size);
    mem = static_cast<char*>(mem) + sizeof(unsigned char);

    new (mem) std::mutex;
    mem = static_cast<char*>(mem) + sizeof(std::mutex);

    auto* block = reinterpret_cast<block_metadata*>(mem);
    block->occupied = false;
    block->size = static_cast<unsigned char>(space_size);
}

void* allocator_buddies_system::do_allocate_sm(size_t size) {
    std::lock_guard lock(get_mutex());

    size_t required = size + occupied_block_metadata_size; // с учетом меты
    void* block = nullptr;

    switch (*reinterpret_cast<fit_mode*>(
            static_cast<char*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator_dbg_helper*))) {
        case fit_mode::first_fit:
            block = get_first_suitable(required);
            break;
        case fit_mode::the_best_fit:
            block = get_best_suitable(required);
            break;
        case fit_mode::the_worst_fit:
            block = get_worst_suitable(required);
            break;
    }

    if (!block) return nullptr;

    auto* meta = reinterpret_cast<block_metadata*>(block);
    size_t block_size = get_size_block(block);

    // делим пока возможно
    while (block_size >= 2 * required &&
           meta->size > min_k ) {

        size_t new_size = meta->size - 1;
        size_t half = 1 << new_size;


        auto* buddy = reinterpret_cast<block_metadata*>(static_cast<char*>(block) + half);
        buddy->occupied = false;
        buddy->size = new_size;


        meta->size = new_size;
        block_size = half;
    }

    meta->occupied = true;
    return static_cast<char*>(block) + occupied_block_metadata_size;
    // указатель после меты
}

void allocator_buddies_system::do_deallocate_sm(void* at) {
    std::lock_guard lock(get_mutex());

    if (!at) return;
    auto* block = reinterpret_cast<block_metadata*>(static_cast<char*>(at) - occupied_block_metadata_size);
    block->occupied = false;

    while (true) {
        size_t offset = reinterpret_cast<char*>(block) - static_cast<char*>(_trusted_memory) - allocator_metadata_size;
        size_t size = get_size_block(block);
        size_t buddy_offset = offset ^ size;

        auto* buddy = reinterpret_cast<block_metadata*>(
                static_cast<char*>(_trusted_memory) + allocator_metadata_size + buddy_offset);

        if (buddy->occupied || buddy->size != block->size) break;

        if (buddy < block) std::swap(buddy, block);

        block->size += 1;
        block->occupied = false;
    }
}

inline std::mutex& allocator_buddies_system::get_mutex() noexcept {
    return *reinterpret_cast<std::mutex*>(
            static_cast<char*>(_trusted_memory) + allocator_metadata_size - sizeof(std::mutex));
}

bool allocator_buddies_system::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

inline void allocator_buddies_system::set_fit_mode(fit_mode mode) {
    std::lock_guard lock(get_mutex());
    void* ptr = static_cast<char*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator_dbg_helper*);
    *reinterpret_cast<fit_mode*>(ptr) = mode;
}

inline size_t allocator_buddies_system::get_size_full() const noexcept {
    void* ptr = static_cast<char*>(_trusted_memory) + sizeof(logger*) + sizeof(allocator_dbg_helper*) + sizeof(fit_mode);
    return 1 << (*reinterpret_cast<unsigned char*>(ptr));
}

void* allocator_buddies_system::get_first_suitable(size_t size) const noexcept {
    for (auto it = begin(); it != end(); ++it) {
        if (!it.occupied() && it.size() >= size) {
            return it.get_ptr_iterator();
        }
    }
    return nullptr;
}

void* allocator_buddies_system::get_best_suitable(size_t size) const noexcept {
    size_t best_size = SIZE_MAX;
    void* result = nullptr;

    for (auto it = begin(); it != end(); ++it) {
        size_t bsize = it.size();
        if (!it.occupied() && bsize >= size && bsize < best_size) {
            best_size = bsize;
            result = it.get_ptr_iterator();
        }
    }
    return result;
}

void* allocator_buddies_system::get_worst_suitable(size_t size) const noexcept {
    size_t worst_size = 0;
    void* result = nullptr;

    for (auto it = begin(); it != end(); ++it) {
        size_t bsize = it.size();
        if (!it.occupied() && bsize >= size && bsize > worst_size) {
            worst_size = bsize;
            result = it.get_ptr_iterator();
        }
    }
    return result;
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept {
    std::vector<allocator_test_utils::block_info> blocks;

    for (auto it = begin(); it != end(); ++it) {
        blocks.push_back({ .block_size = it.size(), .is_block_occupied = it.occupied() });
    }

    return blocks;
}

allocator_buddies_system::buddy_iterator allocator_buddies_system::begin() const noexcept {
    return buddy_iterator(static_cast<std::byte*>(_trusted_memory) + allocator_metadata_size);
}

allocator_buddies_system::buddy_iterator allocator_buddies_system::end() const noexcept {
    size_t full = get_size_full();
    return buddy_iterator(static_cast<std::byte*>(_trusted_memory) + allocator_metadata_size + full);
}

bool allocator_buddies_system::buddy_iterator::operator==(const buddy_iterator& other) const noexcept {
    return _block == other._block;
}

bool allocator_buddies_system::buddy_iterator::operator!=(const buddy_iterator& other) const noexcept {
    return _block != other._block;
}

allocator_buddies_system::buddy_iterator& allocator_buddies_system::buddy_iterator::operator++() noexcept {
    _block = reinterpret_cast<std::byte*>(_block) + get_size_block(_block);
    return *this;
}

allocator_buddies_system::buddy_iterator allocator_buddies_system::buddy_iterator::operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
}

size_t allocator_buddies_system::buddy_iterator::size() const noexcept {
    return get_size_block(_block);
}

bool allocator_buddies_system::buddy_iterator::occupied() const noexcept {
    return reinterpret_cast<block_metadata*>(_block)->occupied;
}

void* allocator_buddies_system::buddy_iterator::operator*() const noexcept {
    return reinterpret_cast<std::byte*>(_block) + free_block_metadata_size;
}

void* allocator_buddies_system::buddy_iterator::get_ptr_iterator() const noexcept {
    return _block;
}

allocator_buddies_system::buddy_iterator::buddy_iterator(void* start) : _block(start) {}
allocator_buddies_system::buddy_iterator::buddy_iterator() : _block(nullptr) {}

inline logger* allocator_buddies_system::get_logger() const {
    return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_buddies_system::get_typename() const {
    return "allocator_buddies_system";
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info_inner() const {
    return get_blocks_info();
}

inline size_t allocator_buddies_system::get_size_block(void* block) noexcept {
    return (1 << reinterpret_cast<block_metadata*>(block)->size);
}
