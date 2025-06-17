#include <not_implemented.h>
#include "../include/allocator_boundary_tags.h"


using byte = unsigned char;


allocator_boundary_tags::~allocator_boundary_tags()
{
    debug_with_guard("Called allocator destructor");
    (*get_mutex()).~mutex();
    ::operator delete(_trusted_memory);
}

allocator_boundary_tags::allocator_boundary_tags(
        allocator_boundary_tags &&other) noexcept
{
    debug_with_guard("move copy start");
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;
    debug_with_guard("move copy finish");
}

inline std::mutex* allocator_boundary_tags::get_mutex()
{
    trace_with_guard("started getting mutex");
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

    ptr += sizeof(class logger*);
    ptr += sizeof(allocator_dbg_helper*);
    ptr += sizeof(allocator_boundary_tags::fit_mode);

    trace_with_guard("ended getting mutex");
    return reinterpret_cast<std::mutex*>(ptr);
}

allocator_boundary_tags &allocator_boundary_tags::operator=(
        allocator_boundary_tags &&other) noexcept
{
    debug_with_guard("move assignment start");
    if (this != &other)
    {
        if (_trusted_memory)
        {
            debug_with_guard("deleting old memory");
            ::operator delete(_trusted_memory);
        }
        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }
    debug_with_guard("move assignment finish");
    return *this;
}


/** If parent_allocator* == nullptr you should use std::pmr::get_default_resource()
 */
allocator_boundary_tags::allocator_boundary_tags(
        size_t space_size,
        std::pmr::memory_resource *parent_allocator,
        logger *log,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    size_t memory_size = space_size + allocator_metadata_size;
    void * memory;

    if (parent_allocator == nullptr)
    {
        try
        {
            memory = ::operator new(memory_size);
        }
        catch (const std::bad_alloc &)
        {
            throw std::bad_alloc();
        }
    }
    else
    {
        memory = parent_allocator->allocate(memory_size);
    }
    _trusted_memory = memory;
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

    *reinterpret_cast<class logger**> (memory) = log;
    ptr += sizeof(class logger*);

    *reinterpret_cast<std::pmr::memory_resource**>(ptr) = parent_allocator;
    ptr += sizeof(std::pmr::memory_resource*);

    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr) = allocate_fit_mode;
    ptr += sizeof(allocator_with_fit_mode::fit_mode);

    new (ptr) std::mutex;
    ptr += sizeof(std::mutex);

    *reinterpret_cast<size_t*>(ptr) = space_size;
    ptr += sizeof(size_t);

    *reinterpret_cast<void**>(ptr) = nullptr;
    ptr += sizeof(void*);
}

inline void **allocator_boundary_tags::get_first_block_ptr() const noexcept
{
    byte *ptr = (reinterpret_cast<byte*>(_trusted_memory)) + allocator_metadata_size - sizeof(void *);
    return reinterpret_cast<void**>(ptr);
}
inline void* allocator_boundary_tags::get_first_block() const noexcept
{
    byte* ptr = (reinterpret_cast<byte*>(_trusted_memory)) + allocator_metadata_size;
    return reinterpret_cast<void*>(ptr);
}

[[nodiscard]] void* allocator_boundary_tags::do_allocate_sm(size_t size)
{

    size_t total_size = size + occupied_block_metadata_size;

    debug_with_guard("do_allocate_sm start");

    std::lock_guard<std::mutex> lock(*get_mutex());

    void** first_block = get_first_block_ptr();

    byte* ptr;
    if (*first_block == nullptr)
    {
        ptr = reinterpret_cast<byte*>(_trusted_memory) + allocator_metadata_size;
        size_t size_block = get_size();

        if (size_block < total_size)
        {
            error_with_guard("Try to allocate " +
                             std::to_string(total_size) + " from " + std::to_string(size));
            throw std::bad_alloc();
        }

        *first_block = ptr;

        debug_with_guard("Allocating first block of size " + std::to_string(size));

        return create_block_meta(reinterpret_cast<void*>(ptr),
                                 total_size - occupied_block_metadata_size,
                                 nullptr,
                                 nullptr);
    }

    allocator_with_fit_mode::fit_mode mode = get_fit_mode();
    void* memory = nullptr;

    switch (mode)
    {
        case allocator_with_fit_mode::fit_mode::first_fit:
            trace_with_guard("Started allocation with first_fit");
            memory = allocate_first_fit(*first_block, size);
            trace_with_guard("Ended allocation with first_fit");
            break;

        case allocator_with_fit_mode::fit_mode::the_best_fit:
            trace_with_guard("Started allocation with the_best_fit");
            memory = allocate_best_fit(*first_block, size);
            trace_with_guard("Ended allocation with the_best_fit");
            break;

        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            trace_with_guard("Started allocation with the_worst_fit");
            memory = allocate_worst_fit(*first_block, size);
            trace_with_guard("Ended allocation with the_worst_fit");
            break;

        default:
            error_with_guard("Unknown fit mode selected");
            throw std::runtime_error("Unknown fit mode");
    }

    if (memory != nullptr)
    {
        debug_with_guard("Successfully allocated " + std::to_string(size) +
                         " bytes with metadata size " +
                         std::to_string(occupied_block_metadata_size));
        get_blocks_info();
        information_with_guard("Available free memory after allocation");
        return memory;
    }
    else
    {
        error_with_guard("Allocation failed for size " + std::to_string(size));
        throw std::bad_alloc();
    }
}


void* allocator_boundary_tags::allocate_first_fit(void* left_elem, size_t size)
{
    void* place_to_place = nullptr;
    void* that_block = nullptr;

    void* next_block = get_next_existing_block(left_elem);
    void* block = left_elem;

    void* st_ptr = slide_block_for(_trusted_memory, allocator_metadata_size);
    if (st_ptr != left_elem)
    {
        size_t dt = reinterpret_cast<byte*>(left_elem) - reinterpret_cast<byte*>(st_ptr);
        size_t new_size = dt - occupied_block_metadata_size;
        if (dt >= size + occupied_block_metadata_size)
        {
            place_to_place = st_ptr;
            that_block = nullptr;
            *get_first_block_ptr() = place_to_place;

            return create_block_meta(place_to_place, new_size, that_block, left_elem);
        }
    }

    while (next_block != nullptr && place_to_place == nullptr)
    {
        size_t dt = get_block_distance(block, next_block);
        if (dt >= size + occupied_block_metadata_size)
        {
            place_to_place = slide_block_for(block, occupied_block_metadata_size + get_block_data_size(block));
            that_block = block;
            break;
        }
        block = next_block;
        next_block = get_next_existing_block(block);
    }

    if (place_to_place == nullptr)
    {
        size_t dt = reinterpret_cast<byte*>(block) + get_block_data_size(block) + occupied_block_metadata_size - (reinterpret_cast<byte*>(_trusted_memory) + allocator_metadata_size);
        if (get_size() - dt >= size + occupied_block_metadata_size)
        {
            place_to_place = slide_block_for(block, occupied_block_metadata_size + get_block_data_size(block));
            that_block = block;
        }
    }

    if (place_to_place == nullptr)
    {
        return nullptr;
    }

    byte* next_block_ = reinterpret_cast<byte*>(get_next_existing_block(that_block));
    if (next_block_ != nullptr)
    {
        *reinterpret_cast<void**>(next_block_ + sizeof(void*) + sizeof(size_t)) = place_to_place;
    }

    byte* prev_block = reinterpret_cast<byte*>(that_block);
    if (that_block == nullptr)
    {
        next_block = reinterpret_cast<byte*>(that_block);
    }
    else if (prev_block != nullptr)
    {
        *reinterpret_cast<void**>(prev_block + 2 * sizeof(void*) + sizeof(size_t)) = place_to_place;
    }

    return create_block_meta(place_to_place, size, that_block, reinterpret_cast<void*>(next_block));
}


void* allocator_boundary_tags::allocate_best_fit(void* left_elem, size_t size)
{
    void* place_to_place = nullptr;
    void* that_block = nullptr;

    size_t actual_block_size = get_size() + 1;
    bool at_start = false;

    void* next_block = get_next_existing_block(left_elem);
    void* block = left_elem;

    while (next_block != nullptr) // search block in midle
    {
        size_t dt = get_block_distance(block, next_block);
        if (dt < actual_block_size && dt >= size + occupied_block_metadata_size)
        {
            actual_block_size = dt;
            that_block = block;
            place_to_place = slide_block_for(block, occupied_block_metadata_size + get_block_data_size(block));
        }
        block = next_block;
        next_block = get_next_existing_block(block);
    }
    size_t dt = (reinterpret_cast<byte*>(block) + get_block_data_size(block) + occupied_block_metadata_size) - (reinterpret_cast<byte*>(_trusted_memory) + allocator_metadata_size);

    if (get_size() - dt >= size + occupied_block_metadata_size && get_size() - dt < actual_block_size + occupied_block_metadata_size) // block at end
    {
        actual_block_size = get_size() - dt;
        that_block = block;
        place_to_place = slide_block_for(block, occupied_block_metadata_size + get_block_data_size(block));
    }

    if (slide_block_for(_trusted_memory, allocator_metadata_size) != left_elem) // block at start
    {
        void* st_ptr = slide_block_for(_trusted_memory, allocator_metadata_size);
        if (get_block_distance(st_ptr, left_elem) >= occupied_block_metadata_size + size &&
            get_block_distance(st_ptr, left_elem) < actual_block_size &&
            place_to_place == nullptr)
        {
            actual_block_size = get_block_distance(st_ptr, left_elem);
            that_block = left_elem;
            place_to_place = st_ptr;
            at_start = true;
        }
    }

    if (at_start)
    {
        *get_first_block_ptr() = place_to_place;
    }
    if (place_to_place == nullptr)
    {
        return nullptr;
    }
    byte* next_block_ = reinterpret_cast<byte*>(get_next_existing_block(that_block));
    if (next_block_ != nullptr)
    {
        *reinterpret_cast<void**>(next_block_ + sizeof(void*) + sizeof(size_t)) = place_to_place;
    }

    byte* prev_block = reinterpret_cast<byte*>(that_block);
    if (at_start)
    {
        next_block = reinterpret_cast<byte*>(that_block);
        that_block = nullptr;
    } else if (prev_block != nullptr)
    {
        *reinterpret_cast<void**>(prev_block + 2 * sizeof(void*) + sizeof(size_t)) = place_to_place;
    }


    return create_block_meta(place_to_place, size, that_block, reinterpret_cast<void*>(next_block));
}

void* allocator_boundary_tags::allocate_worst_fit(void* left_elem, size_t size)
{
    void* place_to_place = nullptr;
    void* that_block = nullptr;

    size_t actual_block_size = 0;
    bool at_start = false;

    void* next_block = get_next_existing_block(left_elem);
    void* block = left_elem;

    while (next_block != nullptr) // search block in midle
    {
        size_t dt = get_block_distance(block, next_block);
        if (dt > actual_block_size && dt >= size + occupied_block_metadata_size)
        {
            actual_block_size = dt;
            that_block = block;
            place_to_place = slide_block_for(block, occupied_block_metadata_size + get_block_data_size(block));
        }
        block = next_block;
        next_block = get_next_existing_block(block);
    }
    size_t dt = (reinterpret_cast<byte*>(block) + get_block_data_size(block) + occupied_block_metadata_size) - (reinterpret_cast<byte*>(_trusted_memory) + allocator_metadata_size);

    if (get_size() - dt >= size + occupied_block_metadata_size && get_size() - dt > actual_block_size + occupied_block_metadata_size) // block at end
    {
        actual_block_size = get_size() - dt;
        that_block = block;
        place_to_place = slide_block_for(block, occupied_block_metadata_size + get_block_data_size(block));
    }

    if (slide_block_for(_trusted_memory, allocator_metadata_size) != left_elem) // block at start
    {
        void* st_ptr = slide_block_for(_trusted_memory, allocator_metadata_size);
        if (get_block_distance(st_ptr, left_elem) >= occupied_block_metadata_size + size &&
            get_block_distance(st_ptr, left_elem) > actual_block_size &&
            place_to_place == nullptr)
        {
            actual_block_size = get_block_distance(st_ptr, left_elem);
            that_block = left_elem;
            place_to_place = st_ptr;
            at_start = true;
        }
    }

    if (at_start)
    {
        *get_first_block_ptr() = place_to_place;
    }

    if (place_to_place == nullptr)
    {
        return nullptr;
    }
    byte* next_block_ = reinterpret_cast<byte*>(get_next_existing_block(that_block));

    if (next_block_ != nullptr)
    {
        *reinterpret_cast<void**>(next_block_ + sizeof(void*) + sizeof(size_t)) = place_to_place;
    }

    byte* prev_block = reinterpret_cast<byte*>(that_block);

    if (at_start)
    {
        next_block = reinterpret_cast<byte*>(that_block);
        that_block = nullptr;
    }
    else if (prev_block != nullptr)
    {
        *reinterpret_cast<void**>(prev_block + 2 * sizeof(void*) + sizeof(size_t)) = place_to_place;
    }


    return create_block_meta(place_to_place, size, that_block, reinterpret_cast<void*>(next_block));

}



inline allocator_with_fit_mode::fit_mode allocator_boundary_tags::get_fit_mode()
{
    trace_with_guard("started getting fit mode");
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

    ptr += sizeof(class logger*);
    ptr += sizeof(memory_resource*);

    trace_with_guard("ended getting fit mode");
    return *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr);
}

inline void* allocator_boundary_tags::create_block_meta(void* block, size_t size, void* left, void* right)
{
    trace_with_guard("started setting meta data");
    byte* place_ptr = reinterpret_cast<byte*>(block);

    *reinterpret_cast<void**>(place_ptr) = _trusted_memory;
    place_ptr += sizeof(void*);

    *reinterpret_cast<size_t*>(place_ptr) = size;
    place_ptr += sizeof(size_t);

    *reinterpret_cast<void**>(place_ptr) = left;
    place_ptr += sizeof(void*);

    *reinterpret_cast<void**>(place_ptr) = right;
    place_ptr += sizeof(void*);
    trace_with_guard("started setting meta data");
    return place_ptr;
}


inline size_t allocator_boundary_tags::get_size() const
{
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

    ptr += sizeof(class logger*);
    ptr += sizeof(memory_resource*);
    ptr += sizeof(allocator_boundary_tags::fit_mode);
    ptr += sizeof(std::mutex);

    return *reinterpret_cast<size_t*>(ptr);
}

void allocator_boundary_tags::do_deallocate_sm(
        void *at)
{
    trace_with_guard("Started deallocate");
    std::lock_guard lock(*get_mutex());
    if (at == nullptr)
    {
        return;
    }
    void* trusted = *reinterpret_cast<void**>(reinterpret_cast<byte*>(at) - occupied_block_metadata_size);
    if (trusted != _trusted_memory || trusted == nullptr)
    {
        error_with_guard("Tryed to deallocate not allocator's property");
        throw std::logic_error("Not allocator's property");
        return;
    }


    void* block = reinterpret_cast<void*>(reinterpret_cast<byte*>(at) - occupied_block_metadata_size);

    void* next_block = get_next_existing_block(block);
    void* prev_block = get_prev_existing_block(block);

    if (next_block != nullptr && prev_block != nullptr) // that was midle block
    {
        // setting right -> left
        byte* place_ptr = reinterpret_cast<byte*>(next_block);

        place_ptr += sizeof(void*);
        place_ptr += sizeof(size_t);

        *reinterpret_cast<void**>(place_ptr) = prev_block;

        // setting left -> right
        place_ptr = reinterpret_cast<byte*>(prev_block);

        place_ptr += (2 * sizeof(void*));
        place_ptr += sizeof(size_t);

        *reinterpret_cast<void**>(place_ptr) = next_block;
    }
    else if (next_block != nullptr) // that was the first (max left block)
    {
        byte* place_ptr = reinterpret_cast<byte*>(next_block);

        place_ptr += sizeof(void*);
        place_ptr += sizeof(size_t);

        *reinterpret_cast<void**>(place_ptr) = nullptr;
        *get_first_block_ptr() = next_block;
    }
    else if (prev_block != nullptr) // that was last block
    {
        byte* place_ptr = reinterpret_cast<byte*>(prev_block);

        place_ptr += (2 * sizeof(void*));
        place_ptr += sizeof(size_t);

        *reinterpret_cast<void**>(place_ptr) = nullptr;
    }
    else if (prev_block == nullptr && next_block == nullptr) // only one block
    {
        *get_first_block_ptr() = nullptr;
    }

    trace_with_guard("Ended deallocate");
}

inline void allocator_boundary_tags::set_fit_mode(
        allocator_with_fit_mode::fit_mode mode)
{
    trace_with_guard("started setting fit mode");
    unsigned char* ptr = reinterpret_cast<unsigned char*>(_trusted_memory);

    ptr += sizeof(class logger*);
    ptr += sizeof(memory_resource*);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(ptr) = mode;
    trace_with_guard("ended setting fit mode");
}


std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info() const
{
    size_t full_free_size = 0;
    std::vector <allocator_test_utils::block_info> result;
    // block_info - размер и занят ли

    void *first_block = *get_first_block_ptr();

    if(first_block == nullptr)
    {
        full_free_size = get_size();
        result.push_back({ full_free_size, false });
        return result;
    }

    void* current = first_block;
    void* next = get_next_existing_block(current);

    // проверяем, что нет других блоков
    if (get_prev_existing_block(current) == nullptr)
    {
        size_t first_available_block_size = get_block_distance(current, slide_block_for(_trusted_memory, allocator_metadata_size));
        if (first_available_block_size > 0)
        {
            full_free_size += first_available_block_size;
            result.push_back({ first_available_block_size,
                               false });
        }
        result.push_back({ get_block_data_size(current) + occupied_block_metadata_size, true});
    } else
    {
        throw std::logic_error("it is impossible way");
    }
    while (current != nullptr)
    {
        size_t size_between_current_and_next = 0;
        bool t_ = false;
        if (next == nullptr)
        {
            void* end = slide_block_for(_trusted_memory,  allocator_metadata_size + get_size());
            void* start = slide_block_for(current, occupied_block_metadata_size + get_block_data_size(current));

            size_between_current_and_next = reinterpret_cast<byte*>(end) - reinterpret_cast<byte*>(start);
            if (size_between_current_and_next > 0)
            {
                full_free_size += size_between_current_and_next;
                result.push_back({ size_between_current_and_next, false });
            }
            return result;
        }
        else
        {
            void* next_load_block = next;
            void* end_address_current_block = slide_block_for(current, get_block_data_size(current) + occupied_block_metadata_size);

            size_between_current_and_next = reinterpret_cast<byte*>(next_load_block) - reinterpret_cast<byte*>(end_address_current_block);
            if (size_between_current_and_next > 0)
            {
                full_free_size += size_between_current_and_next;
                result.push_back({ size_between_current_and_next, false });
            }
            result.push_back({ get_block_data_size(next) + occupied_block_metadata_size, true });
        }

        current = next;
        next = get_next_existing_block(current);
    }
    return result;

}

inline size_t allocator_boundary_tags::get_block_distance(void* left, void *right) const
// конец левого - начало правого
{
    if (left == right)
    {
        return 0;
    }
    return (reinterpret_cast<byte*> (right) - reinterpret_cast<byte*>(left) -
            get_block_data_size(left) - occupied_block_metadata_size);
}

inline void* allocator_boundary_tags::slide_block_for(void* block, size_t bytes) const
{
    return reinterpret_cast<void*>(reinterpret_cast<byte*> (block) + bytes);
}

inline size_t allocator_boundary_tags::get_block_data_size(void* block) const
{
    byte* ptr = reinterpret_cast<byte*> (block);
    return *reinterpret_cast<size_t*>(ptr + sizeof(void*));
}



inline void* allocator_boundary_tags::get_prev_existing_block(void* right_elem) const
{
    if (right_elem == nullptr)
    {
        return nullptr;
    }
    byte* ptr = reinterpret_cast<byte*> (right_elem);
    return *reinterpret_cast<void**>(ptr + sizeof(void*) + sizeof(size_t));
}


inline void *allocator_boundary_tags::get_next_existing_block(void* left_elem) const {
    if (left_elem == nullptr) {
        return nullptr;
    }
    byte * ptr = reinterpret_cast<byte *> (left_elem);
    return *reinterpret_cast<void **>(ptr + 2 * sizeof(void *) + sizeof(size_t));
    // встали на последний указатель -> next_block
}

inline logger *allocator_boundary_tags::get_logger() const
{
    return *reinterpret_cast<logger**>(_trusted_memory);
}

inline std::string allocator_boundary_tags::get_typename() const noexcept
{
    return "allocator_boundary_tags";
}


allocator_boundary_tags::boundary_iterator allocator_boundary_tags::begin() const noexcept
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator allocator_boundary_tags::begin() const noexcept", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator allocator_boundary_tags::end() const noexcept
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator allocator_boundary_tags::end() const noexcept", "your code should be here...");
}

std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info_inner() const
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_boundary_tags::get_blocks_info_inner() const", "your code should be here...");
}


allocator_boundary_tags &allocator_boundary_tags::operator=(const allocator_boundary_tags &other)
{
    throw not_implemented("allocator_boundary_tags &allocator_boundary_tags::operator=(const allocator_boundary_tags &other)", "your code should be here...");
}

bool allocator_boundary_tags::do_is_equal(const std::pmr::memory_resource &other) const noexcept
{
    throw not_implemented("bool allocator_boundary_tags::do_is_equal(const std::pmr::memory_resource &other) const noexcept", "your code should be here...");
}

bool allocator_boundary_tags::boundary_iterator::operator==(
        const allocator_boundary_tags::boundary_iterator &other) const noexcept
{
    throw not_implemented("bool allocator_boundary_tags::boundary_iterator::operator==(const allocator_boundary_tags::boundary_iterator &) const noexcept", "your code should be here...");
}

bool allocator_boundary_tags::boundary_iterator::operator!=(
        const allocator_boundary_tags::boundary_iterator & other) const noexcept
{
    throw not_implemented("bool allocator_boundary_tags::boundary_iterator::operator!=(const allocator_boundary_tags::boundary_iterator &) const noexcept", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator &allocator_boundary_tags::boundary_iterator::operator++() & noexcept
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator &allocator_boundary_tags::boundary_iterator::operator++() & noexcept", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator &allocator_boundary_tags::boundary_iterator::operator--() & noexcept
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator &allocator_boundary_tags::boundary_iterator::operator--() & noexcept", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator allocator_boundary_tags::boundary_iterator::operator++(int n)
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator allocator_boundary_tags::boundary_iterator::operator++(int n)", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator allocator_boundary_tags::boundary_iterator::operator--(int n)
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator allocator_boundary_tags::boundary_iterator::operator--(int n)", "your code should be here...");
}

size_t allocator_boundary_tags::boundary_iterator::size() const noexcept
{
    throw not_implemented("size_t allocator_boundary_tags::boundary_iterator::size() const noexcept", "your code should be here...");
}

bool allocator_boundary_tags::boundary_iterator::occupied() const noexcept
{
    throw not_implemented("bool allocator_boundary_tags::boundary_iterator::occupied() const noexcept", "your code should be here...");
}

void* allocator_boundary_tags::boundary_iterator::operator*() const noexcept
{
    throw not_implemented("void* allocator_boundary_tags::boundary_iterator::operator*() const noexcept", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator::boundary_iterator()
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator::boundary_iterator()", "your code should be here...");
}

allocator_boundary_tags::boundary_iterator::boundary_iterator(void *trusted)
{
    throw not_implemented("allocator_boundary_tags::boundary_iterator::boundary_iterator(void *)", "your code should be here...");
}

void *allocator_boundary_tags::boundary_iterator::get_ptr() const noexcept
{
    throw not_implemented("void *allocator_boundary_tags::boundary_iterator::get_ptr() const noexcept", "your code should be here...");
}