#include "../include/allocator_global_heap.h"
#include <new>
#include <utility>

allocator_global_heap::allocator_global_heap(logger *logger)
        : _logger(logger)
{
    if (_logger) _logger->trace("allocator_global_heap::allocator_global_heap(logger *) started");
    if (_logger) _logger->trace("allocator_global_heap::allocator_global_heap(logger *) finished");
}

allocator_global_heap::~allocator_global_heap()
{
    if (_logger) _logger->trace("allocator_global_heap::~allocator_global_heap() started");
    if (_logger) _logger->trace("allocator_global_heap::~allocator_global_heap() finished");
}

allocator_global_heap::allocator_global_heap(const allocator_global_heap &other)
        : _logger(other._logger)
{
    if (_logger) _logger->trace("allocator_global_heap::allocator_global_heap(const &) started");
    if (_logger) _logger->trace("allocator_global_heap::allocator_global_heap(const &) finished");
}

allocator_global_heap &allocator_global_heap::operator=(const allocator_global_heap &other)
{
    if (_logger) _logger->trace("allocator_global_heap::operator=(const &) started");
    if (this != &other) {
        _logger = other._logger;
    }
    if (_logger) _logger->trace("allocator_global_heap::operator=(const &) finished");
    return *this;
}

allocator_global_heap::allocator_global_heap(allocator_global_heap &&other) noexcept
        : _logger(other._logger)
{
    if (_logger) _logger->trace("allocator_global_heap::allocator_global_heap(&&) started");
    other._logger = nullptr;
    if (_logger) _logger->trace("allocator_global_heap::allocator_global_heap(&&) finished");
}

allocator_global_heap &allocator_global_heap::operator=(allocator_global_heap &&other) noexcept
{
    if (_logger) _logger->trace("allocator_global_heap::operator=(&&) started");
    if (this != &other) {
        _logger = other._logger;
        other._logger = nullptr;
    }
    if (_logger) _logger->trace("allocator_global_heap::operator=(&&) finished");
    return *this;
}

void *allocator_global_heap::do_allocate_sm(size_t size)
{
    if (_logger) _logger->debug("allocator_global_heap::do_allocate_sm started. Size: " + std::to_string(size));
    try {
        void *ptr = ::operator new(size);// можно без ::, но тогда может использовать не глобальную версию, а перегруз
        if (_logger) _logger->debug("allocator_global_heap::do_allocate_sm finished. Ptr: " + std::to_string(reinterpret_cast<std::uintptr_t>(ptr)));
        return ptr;
    } catch (const std::bad_alloc &) {
        if (_logger) _logger->error("allocator_global_heap::do_allocate_sm failed with std::bad_alloc");
        throw;
    }
}

void allocator_global_heap::do_deallocate_sm(void *at)
{
    if (_logger) _logger->debug("allocator_global_heap::do_deallocate_sm started. Ptr: " + std::to_string(reinterpret_cast<std::uintptr_t>(at)));
    ::operator delete(at);
    if (_logger) _logger->debug("allocator_global_heap::do_deallocate_sm finished.");
}

bool allocator_global_heap::do_is_equal(const std::pmr::memory_resource &other) const noexcept
{
    return this == &other;
}

logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

std::string allocator_global_heap::get_typename() const
{
    return "allocator_global_heap";
}
