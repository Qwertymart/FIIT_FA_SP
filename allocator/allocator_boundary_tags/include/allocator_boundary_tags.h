#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H

#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <pp_allocator.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <iterator>
#include <mutex>

class allocator_boundary_tags final :
        public smart_mem_resource,
        public allocator_test_utils,
        public allocator_with_fit_mode,
        private logger_guardant,
        private typename_holder
{

private:

    //структура меты: логгер, родительский аллокатор, фит мод, общий размер, указаеть на первый свободный

    // структура меты занятого блока: размер блока, указатель на начало, указатель вперед и назад

    /**
     * TODO: You must improve it for alignment support
     */
    static constexpr const size_t allocator_metadata_size = sizeof(logger*) + sizeof(memory_resource*) + sizeof(allocator_with_fit_mode::fit_mode) +
                                                            sizeof(size_t) + sizeof(std::mutex) + sizeof(void*);

    static constexpr const size_t occupied_block_metadata_size = sizeof(size_t) + sizeof(void*) + sizeof(void*) + sizeof(void*);

    static constexpr const size_t free_block_metadata_size = 0;

    void *_trusted_memory;

public:

    ~allocator_boundary_tags() override;

    allocator_boundary_tags(allocator_boundary_tags const &other) = delete;

    allocator_boundary_tags &operator=(allocator_boundary_tags const &other);

    allocator_boundary_tags(
            allocator_boundary_tags &&other) noexcept;

    allocator_boundary_tags &operator=(
            allocator_boundary_tags &&other) noexcept;

public:

    explicit allocator_boundary_tags(
            size_t space_size,
            std::pmr::memory_resource *parent_allocator = nullptr,
            logger *log = nullptr,
            allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:

    [[nodiscard]] void *do_allocate_sm(
            size_t bytes) override;

    void do_deallocate_sm(
            void *at) override;

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

public:

    inline void set_fit_mode(
            allocator_with_fit_mode::fit_mode mode) override;

public:

    inline size_t get_size() const;
    void* allocate_first_fit(void* left_elem, size_t size);
    void* allocate_best_fit(void* left_elem, size_t size);
    void* allocate_worst_fit(void* left_elem, size_t size);

    inline void* get_prev_existing_block(void* right_elem) const;
    inline size_t get_block_data_size(void* block) const;
    inline void* slide_block_for(void* block, size_t bytes) const;
    inline size_t get_block_distance(void* left, void *right) const;
    inline void *get_next_existing_block(void* left_elem) const ;
    inline void* create_block_meta(void* block, size_t size, void* left = nullptr, void* right = nullptr);
    inline allocator_with_fit_mode::fit_mode get_fit_mode();
    inline void* get_first_block() const noexcept;
    inline void **get_first_block_ptr() const noexcept;


    std::vector<allocator_test_utils::block_info> get_blocks_info() const override;
    inline std::mutex* get_mutex();

private:

    std::vector<allocator_test_utils::block_info> get_blocks_info_inner() const override;

/** TODO: Highly recommended for helper functions to return references */

    inline logger *get_logger() const override;

    inline std::string get_typename() const noexcept override;

    class boundary_iterator
    {
        void* _occupied_ptr;
        bool _occupied;
        void* _trusted_memory;

    public:

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = void*;
        using reference = void*&;
        using pointer = void**;
        using difference_type = ptrdiff_t;

        bool operator==(const boundary_iterator&) const noexcept;

        bool operator!=(const boundary_iterator&) const noexcept;

        boundary_iterator& operator++() & noexcept;

        boundary_iterator& operator--() & noexcept;

        boundary_iterator operator++(int n);

        boundary_iterator operator--(int n);

        size_t size() const noexcept;

        bool occupied() const noexcept;

        void* operator*() const noexcept;

        void* get_ptr() const noexcept;

        boundary_iterator();

        boundary_iterator(void* trusted);
    };

    friend class boundary_iterator;

    boundary_iterator begin() const noexcept;

    boundary_iterator end() const noexcept;
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ALLOCATOR_ALLOCATOR_BOUNDARY_TAGS_H