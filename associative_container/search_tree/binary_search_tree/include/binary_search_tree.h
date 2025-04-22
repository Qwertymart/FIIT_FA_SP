#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H

#include <list>
#include <stack>
#include <vector>
#include <memory>
#include <logger.h>
#include <logger_guardant.h>
#include <not_implemented.h>
#include <search_tree.h>
#include <stack>
#include <ranges>
#include <pp_allocator.h>
#include <concepts>

namespace __detail
{
    template<typename tkey, typename tvalue, typename compare, typename tag>
    class bst_impl;

    class BST_TAG;
}


template<typename tkey, typename tvalue, compator<tkey> compare = std::less<tkey>, typename tag = __detail::BST_TAG>
class binary_search_tree : private compare
{
public:

    using value_type = std::pair<const tkey, tvalue>;

    friend class __detail::bst_impl<tkey, tvalue, compare, tag>;

protected:
    
    struct node
    {
    
    public:
        
        value_type data;

        node* parent;
        node* left_subtree;
        node* right_subtree;

        template<class ...Args>
        explicit node(node* parent, Args&& ...args);


        virtual ~node() =default;
    };

    inline bool compare_keys(const tkey& lhs, const tkey& rhs) const;

    inline bool compare_pairs(const value_type & lhs, const value_type & rhs) const;

public:


    // region iterators definition

    class prefix_iterator;
    class prefix_const_iterator;
    class prefix_reverse_iterator;
    class prefix_const_reverse_iterator;

    class infix_iterator;
    class infix_const_iterator;
    class infix_reverse_iterator;
    class infix_const_reverse_iterator;

    class postfix_iterator;
    class postfix_const_iterator;
    class postfix_reverse_iterator;
    class postfix_const_reverse_iterator;

    /** @brief Watch about behavior of reverse iterators.
     *
     * @example Arrow is common iterator
     *  1 2 3 -> 4 5 6 7
     *  *it == 4.
     *
     *  @example But reverse:
     *  1 2 3 \<- 4 5 6 7
     *  *rit == 3
     */

    class prefix_iterator
    {
    protected:

        node* _data;

        /** If iterator == end or before_begin _data points to nullptr, _backup to last node
         *
         */
        node* _backup;
        friend class prefix_const_iterator;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;
        
        explicit prefix_iterator(node* data = nullptr, node* backup=nullptr);

        virtual ~prefix_iterator() =default;

        node* get_data() const noexcept {
            return _data;
        }

        bool operator==(
            prefix_iterator const &other) const noexcept;
        
        bool operator!=(
            prefix_iterator const &other) const noexcept;
        
        prefix_iterator &operator++() & noexcept;
        
        prefix_iterator const operator++(int not_used)  noexcept;

        prefix_iterator &operator--() & noexcept;

        prefix_iterator const operator--(int not_used) noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;
        
    };
    
    class prefix_const_iterator
    {
    protected:

        prefix_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = const value_type&;
        using pointer = value_type* const;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit prefix_const_iterator(const node* data = nullptr, const node* backup=nullptr);

        prefix_const_iterator(const prefix_iterator&) noexcept;

        virtual ~prefix_const_iterator() =default;

        bool operator==(
                prefix_iterator const &other) const noexcept;

        bool operator!=(
                prefix_iterator const &other) const noexcept;

        prefix_const_iterator &operator++() & noexcept;

        prefix_const_iterator const operator++(int not_used) noexcept;

        prefix_const_iterator &operator--() & noexcept;

        prefix_const_iterator const operator--(int not_used) noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;
        
    };
    
    class prefix_reverse_iterator
    {
    protected:

        prefix_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit prefix_reverse_iterator(node* data = nullptr, node* backup=nullptr);

        prefix_reverse_iterator(const prefix_iterator&) noexcept;
        operator prefix_iterator() const noexcept;

        prefix_iterator base() const noexcept;

        virtual ~prefix_reverse_iterator() =default;

        bool operator==(prefix_reverse_iterator const &other) const noexcept;

        bool operator!=(prefix_reverse_iterator const &other) const noexcept;

        prefix_reverse_iterator &operator++() & noexcept;

        prefix_reverse_iterator const operator++(int not_used)  noexcept;

        prefix_reverse_iterator &operator--() & noexcept;

        prefix_reverse_iterator const operator--(int not_used) noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;
        
    };
    
    class prefix_const_reverse_iterator
    {
    protected:

        prefix_const_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = const value_type&;
        using pointer = value_type* const;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit prefix_const_reverse_iterator(const node* data = nullptr, const node* backup=nullptr);

        prefix_const_reverse_iterator(const prefix_const_iterator&) noexcept;

        operator prefix_const_iterator() const noexcept;
        prefix_const_iterator base() const noexcept;

        virtual ~prefix_const_reverse_iterator() =default;

        bool operator==(prefix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(prefix_const_reverse_iterator const &other) const noexcept;

        prefix_const_reverse_iterator &operator++() & noexcept;

        prefix_const_reverse_iterator const operator++(int not_used) noexcept;

        prefix_const_reverse_iterator &operator--() & noexcept;

        prefix_const_reverse_iterator const operator--(int not_used) noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;
        
    };


    class infix_iterator
    {
        friend class binary_search_tree;
    protected:

        node* _data;

        /** If iterator == end or before_begin _data points to nullptr, _backup to last node
         *
         */
        node* _backup;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit infix_iterator(node* data = nullptr, node* backup=nullptr);

        virtual ~infix_iterator() =default;

        bool operator==(
                infix_iterator const &other) const noexcept;

        bool operator!=(
                infix_iterator const &other) const noexcept;

        infix_iterator &operator++() & noexcept;

        infix_iterator const operator++(int not_used) noexcept;

        infix_iterator &operator--() & noexcept;

        infix_iterator const operator--(int not_used) noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };

    class infix_const_iterator
    {
    protected:

        infix_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = const value_type&;
        using pointer = value_type* const;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit infix_const_iterator(const node* data = nullptr,const node* backup=nullptr);

        infix_const_iterator(const infix_iterator&) noexcept;

        virtual ~infix_const_iterator() =default;

        bool operator==(
                infix_iterator const &other) const noexcept;

        bool operator!=(
                infix_iterator const &other) const noexcept;

        infix_const_iterator &operator++() & noexcept;

        infix_const_iterator const operator++(int not_used)  noexcept;

        infix_const_iterator &operator--() & noexcept;

        infix_const_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };

    class infix_reverse_iterator
    {
    protected:

        infix_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit infix_reverse_iterator(node* data = nullptr, node* backup=nullptr);

        infix_reverse_iterator(const infix_iterator&) noexcept;
        operator infix_iterator() const noexcept;

        infix_iterator base() const noexcept;

        virtual ~infix_reverse_iterator() =default;

        bool operator==(infix_reverse_iterator const &other) const noexcept;

        bool operator!=(infix_reverse_iterator const &other) const noexcept;

        infix_reverse_iterator &operator++() & noexcept;

        infix_reverse_iterator const operator++(int not_used)  noexcept;

        infix_reverse_iterator &operator--() & noexcept;

        infix_reverse_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };

    class infix_const_reverse_iterator
    {
    protected:

        infix_const_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = const value_type&;
        using pointer = value_type* const;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit infix_const_reverse_iterator(const node* data = nullptr, const node* backup=nullptr);

        infix_const_reverse_iterator(const infix_const_iterator&) noexcept;

        operator infix_const_iterator() const noexcept;
        infix_const_iterator base() const noexcept;

        virtual ~infix_const_reverse_iterator() =default;

        bool operator==(infix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(infix_const_reverse_iterator const &other) const noexcept;

        infix_const_reverse_iterator &operator++() & noexcept;

        infix_const_reverse_iterator const operator++(int not_used)  noexcept;

        infix_const_reverse_iterator &operator--() & noexcept;

        infix_const_reverse_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };


    class postfix_iterator
    {
    protected:

        node* _data;

        /** If iterator == end or before_begin _data points to nullptr, _backup to last node
         *
         */
        node* _backup;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit postfix_iterator(node* data = nullptr, node* backup=nullptr);

        virtual ~postfix_iterator() =default;

        bool operator==(
                postfix_iterator const &other) const noexcept;

        bool operator!=(
                postfix_iterator const &other) const noexcept;

        postfix_iterator &operator++() & noexcept;

        postfix_iterator const operator++(int not_used)  noexcept;

        postfix_iterator &operator--() & noexcept;

        postfix_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        pointer operator->() const noexcept;
        size_t depth() const noexcept;

    };

    class postfix_const_iterator
    {
    protected:

        postfix_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = const value_type&;
        using pointer = value_type* const;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit postfix_const_iterator(const node* data = nullptr, const node* backup=nullptr);

        postfix_const_iterator(const postfix_iterator&) noexcept;

        virtual ~postfix_const_iterator() =default;

        bool operator==(
                postfix_iterator const &other) const noexcept;

        bool operator!=(
                postfix_iterator const &other) const noexcept;

        postfix_const_iterator &operator++() & noexcept;

        postfix_const_iterator const operator++(int not_used)  noexcept;

        postfix_const_iterator &operator--() & noexcept;

        postfix_const_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };

    class postfix_reverse_iterator
    {
    protected:

        postfix_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit postfix_reverse_iterator(node* data = nullptr, node* backup=nullptr);

        postfix_reverse_iterator(const postfix_iterator&) noexcept;
        operator postfix_iterator() const noexcept;

        postfix_iterator base() const noexcept;

        virtual ~postfix_reverse_iterator() =default;

        bool operator==(postfix_reverse_iterator const &other) const noexcept;

        bool operator!=(postfix_reverse_iterator const &other) const noexcept;

        postfix_reverse_iterator &operator++() & noexcept;

        postfix_reverse_iterator const operator++(int not_used)  noexcept;

        postfix_reverse_iterator &operator--() & noexcept;

        postfix_reverse_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };

    class postfix_const_reverse_iterator
    {
    protected:

        postfix_const_iterator _base;

    public:

        using value_type = binary_search_tree<tkey, tvalue, compare>::value_type;
        using difference_type = ptrdiff_t;
        using reference = const value_type&;
        using pointer = value_type* const;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit postfix_const_reverse_iterator(const node* data = nullptr,const node* backup=nullptr);

        postfix_const_reverse_iterator(const postfix_const_iterator&) noexcept;

        operator postfix_const_iterator() const noexcept;
        postfix_const_iterator base() const noexcept;

        virtual ~postfix_const_reverse_iterator() =default;

        bool operator==(postfix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(postfix_const_reverse_iterator const &other) const noexcept;

        postfix_const_reverse_iterator &operator++() & noexcept;

        postfix_const_reverse_iterator const operator++(int not_used)  noexcept;

        postfix_const_reverse_iterator &operator--() & noexcept;

        postfix_const_reverse_iterator const operator--(int not_used)  noexcept;

        /** Throws exception if end
         */
        reference operator*();

        /** UB if iterator points to end
         *
         */

        pointer operator->() noexcept;
        size_t depth() const noexcept;

    };



    friend class prefix_iterator;
    friend class prefix_const_iterator;
    friend class prefix_reverse_iterator;
    friend class prefix_const_reverse_iterator;

    friend class postfix_iterator;
    friend class postfix_const_iterator;
    friend class postfix_reverse_iterator;
    friend class postfix_const_reverse_iterator;

    friend class infix_iterator;
    friend class infix_const_iterator;
    friend class infix_reverse_iterator;
    friend class infix_const_reverse_iterator;

    // endregion iterators definition


protected:
    
    node *_root;
    logger* _logger;
    size_t _size;
    
    /** You should use coercion ctor or template methods of allocator
     */
    pp_allocator<value_type> _allocator;

public:
    explicit binary_search_tree(
            const compare& comp = compare(),
            pp_allocator<value_type> alloc = pp_allocator<value_type>(),
            logger *logger = nullptr);

    explicit binary_search_tree(
            pp_allocator<value_type> alloc,
            const compare& comp = compare(),
            logger *logger = nullptr);


    template<input_iterator_for_pair<tkey, tvalue> iterator>
    explicit binary_search_tree(iterator begin, iterator end, const compare& cmp = compare(), 
                                pp_allocator<value_type> alloc = pp_allocator<value_type>(), 
                                logger* logger = nullptr);

    template<std::ranges::input_range Range>
    explicit binary_search_tree(Range&& range, const compare& cmp = compare(),
                                pp_allocator<value_type> alloc = pp_allocator<value_type>(),
                                logger* logger = nullptr);


    binary_search_tree(std::initializer_list<std::pair<tkey, tvalue>> data, const compare& cmp = compare(),
                       pp_allocator<value_type> alloc = pp_allocator<value_type>(), 
                       logger* logger = nullptr);

public:
    
    binary_search_tree(const binary_search_tree &other);
    
    binary_search_tree(binary_search_tree &&other) noexcept;
    
    binary_search_tree &operator=(const binary_search_tree &other);
    
    binary_search_tree &operator=(binary_search_tree &&other) noexcept;
    
    virtual ~binary_search_tree();

public:

    tvalue& at(const tkey& key);
    const tvalue& at(const tkey& key) const;

    tvalue& operator[](const tkey& key);
    tvalue& operator[](tkey&& key);

    bool empty() const noexcept;

    size_t size() const noexcept;


    void clear() noexcept;
    void clear(node* n) noexcept;


    std::pair<infix_iterator, bool> insert(const value_type&);
    std::pair<infix_iterator, bool> insert(value_type&&);

    template<std::input_iterator InputIt>
    void insert(InputIt first, InputIt last);

    template<std::ranges::input_range R>
    void insert_range( R&& rg );

    template<class ...Args>
    std::pair<infix_iterator, bool> emplace(Args &&...args);

    infix_iterator insert_or_assign(const value_type&);
    infix_iterator insert_or_assign(value_type&&);

    template<std::input_iterator InputIt >
    void insert_or_assign(InputIt first, InputIt last);


    template<class ...Args>
    infix_iterator emplace_or_assign(Args&&...args);

    virtual void swap(binary_search_tree& other) noexcept;

    bool contains(const tkey& key) const;

    infix_iterator find(const tkey&);
    infix_const_iterator find(const tkey&) const;


    infix_iterator lower_bound(const tkey&);
    infix_const_iterator lower_bound(const tkey&) const;

    infix_iterator upper_bound(const tkey&);
    infix_const_iterator upper_bound(const tkey&) const;

    infix_iterator erase(infix_iterator pos);
    infix_iterator erase(infix_const_iterator pos);

    infix_iterator erase(infix_iterator first, infix_iterator last);
    infix_iterator erase(infix_const_iterator first, infix_const_iterator last);

    size_t erase(const tkey& key);

public:
    
    // region iterators requests definition

    infix_iterator begin() noexcept;

    infix_iterator end() noexcept;

    infix_const_iterator begin() const noexcept;

    infix_const_iterator end() const noexcept;

    infix_const_iterator cbegin() const noexcept;

    infix_const_iterator cend() const noexcept;

    infix_reverse_iterator rbegin() noexcept;

    infix_reverse_iterator rend() noexcept;

    infix_const_reverse_iterator rbegin() const noexcept;

    infix_const_reverse_iterator rend() const noexcept;

    infix_const_reverse_iterator crbegin() const noexcept;

    infix_const_reverse_iterator crend() const noexcept;

    
    prefix_iterator begin_prefix() noexcept;
    
    prefix_iterator end_prefix() noexcept;

    prefix_const_iterator begin_prefix() const noexcept;

    prefix_const_iterator end_prefix() const noexcept;
    
    prefix_const_iterator cbegin_prefix() const noexcept;
    
    prefix_const_iterator cend_prefix() const noexcept;
    
    prefix_reverse_iterator rbegin_prefix() noexcept;
    
    prefix_reverse_iterator rend_prefix() noexcept;

    prefix_const_reverse_iterator rbegin_prefix() const noexcept;

    prefix_const_reverse_iterator rend_prefix() const noexcept;
    
    prefix_const_reverse_iterator crbegin_prefix() const noexcept;
    
    prefix_const_reverse_iterator crend_prefix() const noexcept;

    
    infix_iterator begin_infix() noexcept;
    
    infix_iterator end_infix() noexcept;

    infix_const_iterator begin_infix() const noexcept;

    infix_const_iterator end_infix() const noexcept;
    
    infix_const_iterator cbegin_infix() const noexcept;
    
    infix_const_iterator cend_infix() const noexcept;
    
    infix_reverse_iterator rbegin_infix() noexcept;
    
    infix_reverse_iterator rend_infix() noexcept;

    infix_const_reverse_iterator rbegin_infix() const noexcept;

    infix_const_reverse_iterator rend_infix() const noexcept;
    
    infix_const_reverse_iterator crbegin_infix() const noexcept;
    
    infix_const_reverse_iterator crend_infix() const noexcept;

    
    postfix_iterator begin_postfix() noexcept;
    
    postfix_iterator end_postfix() noexcept;

    postfix_const_iterator begin_postfix() const noexcept;

    postfix_const_iterator end_postfix() const noexcept;
    
    postfix_const_iterator cbegin_postfix() const noexcept;
    
    postfix_const_iterator cend_postfix() const noexcept;
    
    postfix_reverse_iterator rbegin_postfix() noexcept;
    
    postfix_reverse_iterator rend_postfix() noexcept;

    postfix_const_reverse_iterator rbegin_postfix() const noexcept;

    postfix_const_reverse_iterator rend_postfix() const noexcept;
    
    postfix_const_reverse_iterator crbegin_postfix() const noexcept;
    
    postfix_const_reverse_iterator crend_postfix() const noexcept;
    
    // endregion iterators requests definition

protected:



    // region subtree rotations definition
    
    static void small_left_rotation(node *&subtree_root) noexcept;

    static void small_right_rotation(node *&subtree_root) noexcept;

    static void big_left_rotation(node *&subtree_root) noexcept;

    static void big_right_rotation(node *&subtree_root) noexcept;

    static void double_left_rotation(node *&subtree_root) noexcept;

    static void double_right_rotation(node *&subtree_root) noexcept;
    
    // endregion subtree rotations definition
    
};

namespace __detail
{
    template<typename tkey, typename tvalue, typename compare, typename tag>
    class bst_impl
    {

        friend class binary_search_tree<tkey, tvalue, compare, tag>;

        template<class ...Args>
        static binary_search_tree<tkey, tvalue, compare, tag>::node* create_node(binary_search_tree<tkey, tvalue, compare, tag>& cont, Args&& ...args);

        static void delete_node(binary_search_tree<tkey, tvalue, compare, tag>& cont, typename binary_search_tree<tkey, tvalue, compare, tag>::node** node_ptr);
        static void delete_node(binary_search_tree<tkey, tvalue, compare, tag>& cont, typename binary_search_tree<tkey, tvalue, compare, tag>::node* node_ptr);

        //Does not invalidate node*, needed for splay tree
        static void post_search(binary_search_tree<tkey, tvalue, compare, tag>::node**){}

        //Does not invalidate node*
        static void post_insert(binary_search_tree<tkey, tvalue, compare, tag>& cont, binary_search_tree<tkey, tvalue, compare, tag>::node**){}

        static void erase(binary_search_tree<tkey, tvalue, compare, tag>& cont, binary_search_tree<tkey, tvalue, compare, tag>::node**);

        static void swap(binary_search_tree<tkey, tvalue, compare, tag>& lhs, binary_search_tree<tkey, tvalue, compare, tag>& rhs) noexcept;
    };
}

template<typename tkey, typename tvalue, typename compare, typename tag>
void __detail::bst_impl<tkey, tvalue, compare, tag>::swap(binary_search_tree<tkey, tvalue, compare, tag> &lhs,
                                                binary_search_tree<tkey, tvalue, compare, tag> &rhs) noexcept
{
    using std::swap;
    swap(lhs._root, rhs._root);
    swap(lhs._logger, rhs._logger);
    swap(lhs._size, rhs._size);
    swap(lhs._allocator, rhs._allocator);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<input_iterator_for_pair<tkey, tvalue> iterator>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(iterator begin, iterator end, const compare &cmp,
                                                                   pp_allocator<typename binary_search_tree<tkey, tvalue, compare, tag>::value_type> alloc, logger *logger)
        : compare(cmp), _root(nullptr), _logger(logger), _size(0), _allocator(alloc)
{
    for (auto it = begin; it != end; ++it)
    {
        emplace(it->first, it->second);
    }
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::compare_pairs(const binary_search_tree::value_type &lhs,
                                                              const binary_search_tree::value_type &rhs) const
{
    return compare_keys(lhs.first, rhs.first);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::compare_keys(const tkey &lhs, const tkey &rhs) const
{
    return compare::operator()(lhs, rhs);
}

template<typename compare, typename U, typename iterator>
explicit binary_search_tree(iterator begin, iterator end, const compare& cmp = compare(),
                            pp_allocator<U> alloc = pp_allocator<U>(),
                            logger* logger = nullptr) -> binary_search_tree<const typename std::iterator_traits<iterator>::value_type::first_type, typename std::iterator_traits<iterator>::value_type::second_type, compare>;

template<typename compare, typename U, std::ranges::forward_range Range>
explicit binary_search_tree(Range&& range, const compare& cmp = compare(),
        pp_allocator<U> alloc = pp_allocator<U>(),
        logger* logger = nullptr) -> binary_search_tree<const typename std::iterator_traits<typename std::ranges::iterator_t<Range>>::value_type::first_type, typename std::iterator_traits<typename std::ranges::iterator_t<Range>>::value_type::second_type, compare> ;

template<typename tkey, typename tvalue, typename compare, typename U>
binary_search_tree(std::initializer_list<std::pair<tkey, tvalue>> data, const compare& cmp = compare(),
        pp_allocator<U> alloc = pp_allocator<U>(),
        logger* logger = nullptr) -> binary_search_tree<tkey, tvalue, compare>;


// region node implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<class ...Args>
binary_search_tree<tkey, tvalue, compare, tag>::node::node(node* parent, Args&& ...args)
        : data(std::forward<Args>(args)...),
          parent(parent), left_subtree(nullptr), right_subtree(nullptr)
{}

// endregion node implementation

// region prefix_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::prefix_iterator(node* data, node* backup)
:_data(data), _backup(backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator==(
        prefix_iterator const &other) const noexcept
{
    return _data == other._data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator!=(
        prefix_iterator const &other) const noexcept
{
    return _data != other._data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator++() & noexcept
{
    if (_data == nullptr)
    {
        _data = _backup;
        return *this;
    }
    if (_data->left_subtree != nullptr)
    {
        _backup = _data;
        _data = _data->left_subtree;
    }
    else if (_data->right_subtree != nullptr)
    {
        _backup = _data;
        _data = _data->right_subtree;
    }
    else
    {
        _backup = _data;
        node* parent = _data->parent;

        while (parent != nullptr && (_data == parent->right_subtree || parent->right_subtree == nullptr)) {
            _data = parent;
            parent = parent->parent;
        }

        if (parent == nullptr) {
            _data = nullptr;
        } else {
            _data = parent->right_subtree;
        }
    }
    return *this;

}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator++(int not_used) noexcept
{
    prefix_iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator--() & noexcept
{
    if (_data == nullptr) {
        _data = _backup;
        return *this;
    }
    
    _backup = _data;
    node* parent = _data->parent;

    if (parent == nullptr) {
        _data = nullptr;
    } else if (parent->left_subtree == _data) {
        _data = parent;
    } else if (parent->right_subtree == _data) {
        node* temp = parent->left_subtree;
        while (temp->right_subtree || temp->left_subtree) {
            if (temp->right_subtree) {
                temp = temp->right_subtree;
            } else {
                temp = temp->left_subtree;
            }
        }
        _data = temp;
    }

    return *this;
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator--(int not_used)  noexcept
{
    prefix_iterator temp = *this;
    --(*this);
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator*()
{
    if (!_data) {
        throw std::out_of_range("Dereferencing end or invalid iterator");
    }
    return _data->data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::operator->() noexcept
{
    return _data ? &(_data->data) : nullptr;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator::depth() const noexcept
{
    size_t depth = 0;
    if (_data == nullptr) {
        return depth;
    }
    node* current = _data;
    while (current->parent != nullptr) {
        current = current->parent;
        ++depth;
    }
    return depth;
}

// endregion prefix_iterator implementation

// region prefix_const_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::prefix_const_iterator(const node* data, const node* backup)
    :_base(const_cast<node*>(data), const_cast<node*>(backup))
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::prefix_const_iterator(const prefix_iterator& other) noexcept
    :_base(other)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator==(
        prefix_iterator const &other) const noexcept
{
    return _base == other;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator!=(
        prefix_iterator const &other) const noexcept
{
    return _base != other;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator++() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator--() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator::depth() const noexcept
{
    return _base.depth();
}
// endregion prefix_const_iterator implementation

// region prefix_reverse_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::prefix_reverse_iterator(node* data, node* backup)
    : _base(data, backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::prefix_reverse_iterator(const prefix_iterator& it) noexcept
        : _base(it)
{
    --_base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator() const noexcept
{
    auto copy = _base;
    ++copy;
    return copy;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::base() const noexcept
{
    return _base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator==(prefix_reverse_iterator const &other) const noexcept
{
    return _base == other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator!=(prefix_reverse_iterator const &other) const noexcept
{
    return _base != other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator++() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator--() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion prefix_reverse_iterator implementation

// region prefix_const_reverse_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(const node* data, const node* backup)
    :_base(data, backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::prefix_const_reverse_iterator(const prefix_const_iterator& it) noexcept
    :_base(it)
{
    --_base(it);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator() const noexcept
{
    auto copy = _base;
    ++copy;
    return copy;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::base() const noexcept
{
    return _base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator==(prefix_const_reverse_iterator const &other) const noexcept
{
    return _base == other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator!=(prefix_const_reverse_iterator const &other) const noexcept
{
    return _base != other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator++() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator--() & noexcept
{
     ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion prefix_const_reverse_iterator implementation

// region infix_iterator implementation
template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::infix_iterator(node* data, node* backup)
    : _data(data), _backup(backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator==(infix_iterator const &other) const noexcept
{
    return _data == other._data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator!=(infix_iterator const &other) const noexcept
{
    return _data != other._data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator++() & noexcept {
    if (_data == nullptr) {
        _data = _backup;
        return *this;
    }
    _backup = _data;
    if (_data->right_subtree != nullptr) {
        _data = _data->right_subtree;
        while (_data->left_subtree != nullptr) {
            _data = _data->left_subtree;
        }
    } else
    {
        node* parent = _data->parent;
        while (parent and _data == parent->right_subtree)
        {
            _data = parent;
            parent = parent->parent;
        }
        _data = parent;
    }
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    ++(*this);
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator--() & noexcept
{
    if (_data == nullptr) {
        _data = _backup;
        return *this;
    }
    _backup = _data;
    if (_data->left_subtree != nullptr) {
        _data = _data->left_subtree;
        while (_data->right_subtree != nullptr) {
            _data = _data->right_subtree;
        }
    } else
    {
        node* parent = _data->parent;
        while (parent and _data == parent->left_subtree)
        {
            _data = parent;
            parent = parent->parent;
        }
        _data = parent;
    }
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    --(*this);
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator*()
{
    if (!_data) {
        throw std::out_of_range("Dereferencing end or invalid iterator");
    }
    return _data->data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::operator->() noexcept
{
    return _data ? &(_data->data) : nullptr;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator::depth() const noexcept
{
    size_t depth = 0;
    if (_data == nullptr) {
        return depth;
    }
    node* current = _data;
    while (current->parent != nullptr) {
        current = current->parent;
        ++depth;
    }
    return depth;
}

// endregion infix_iterator implementation

// region infix_const_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::infix_const_iterator(const node* data, const node* backup)
        :_base(const_cast<node*>(data), const_cast<node*>(backup))
{
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::infix_const_iterator(const infix_iterator& it) noexcept
        :_base(it)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator==(infix_iterator const &other) const noexcept
{
    return _base == other;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator!=(infix_iterator const &other) const noexcept
{
    return _base != other;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator++() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator--() & noexcept
{
    --_base ;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion infix_const_iterator implementation

// region infix_reverse_iterator implementation
template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::infix_reverse_iterator(node* data, node* backup)
    :_base(data, backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::infix_reverse_iterator(const infix_iterator& it) noexcept
        : _base(it)
{
    --_base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator() const noexcept
{
    auto copy = _base;
    ++copy;
    return copy;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::base() const noexcept
{
    return _base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator==(infix_reverse_iterator const &other) const noexcept
{
    return _base == other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator!=(infix_reverse_iterator const &other) const noexcept
{
    return _base != other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator++() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator--() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator--(int not_used)  noexcept {
    auto temp = *this;
    ++_base;
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion infix_reverse_iterator implementation

// region infix_const_reverse_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::infix_const_reverse_iterator(const node* data, const node* backup)
        :_base(data, backup)
{

}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::infix_const_reverse_iterator(const infix_const_iterator& it) noexcept
        :_base(it)
{
    --_base(it);
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator() const noexcept
{
    auto copy = _base;
    ++copy;
    return copy;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::base() const noexcept
{
    return _base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator==(infix_const_reverse_iterator const &other) const noexcept
{
    return _base == other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator!=(infix_const_reverse_iterator const &other) const noexcept
{
    return _base != other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator++() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator++(int not_used)  noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator--() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion infix_const_reverse_iterator implementation

// region postfix_iterator implementation
template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::postfix_iterator(node* data, node* backup)
    :_data(data), _backup(backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator==(postfix_iterator const &other) const noexcept
{
    return _data == other._data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator!=(postfix_iterator const &other) const noexcept
{
    return _data != other._data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator++() & noexcept
{
    if (_data == nullptr) {
        _data = _backup;
        return *this;
    }

    _backup = _data;
    node* parent = _data->parent;

    // Если мы находимся в корне дерева, то следующим будет конец (nullptr)
    if (parent == nullptr) {
        _data = nullptr;
        return *this;
    }

    // Если текущий узел - правый потомок родителя,
    // или у родителя нет правого потомка,
    // то следующий узел - родитель
    if (_data == parent->right_subtree || parent->right_subtree == nullptr) {
        _data = parent;
    }
        // Если текущий узел - левый потомок родителя, и у родителя есть правый потомок,
        // то нужно найти самый глубокий лист в правом поддереве родителя
    else {
        _data = parent->right_subtree;

        // идем вниз
        while (_data->left_subtree != nullptr || _data->right_subtree != nullptr) {
            if (_data->left_subtree != nullptr) {
                _data = _data->left_subtree;
                // это условие помогает пойти в самый левый,
                // если у текущего узла есть правое поддерево, но нет левого
                // то мы должны обойти сначала то правое поддерево,
                // только после этого брать корень
            } else {
                _data = _data->right_subtree;
            }
        }
    }

    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    ++(*this);
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator--() & noexcept
{
    if (_data == nullptr) {
        _data = _backup;
        return *this;
    }

    _backup = _data;

    node* parent = _data->parent;
    if (parent == nullptr) {
        _data = nullptr;
        return *this;
    }

    if (_data == parent->right_subtree) {
        // идем в самое глубокое в левом поддереве, если оно есть
        if (parent->left_subtree) {
            node* temp = parent->left_subtree;
            while (temp->left_subtree || temp->right_subtree) {
                if (temp->right_subtree) {
                    temp = temp->right_subtree;
                    //Аналогично ++
                } else {
                    temp = temp->left_subtree;
                }
            }
            _data = temp;
        } else {
            _data = parent;
        }
    } else if (_data == parent->left_subtree) {
        _data = parent;
    }

    return *this;
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    --(*this);
    return temp;

}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator*()
{
    if (!_data)
    {
        throw std ::out_of_range("Dereferencing end or invalid iterator");
    }
    return _data->data;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator->() noexcept
{
    return _data ? &(_data->data) : nullptr;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::operator->() const noexcept
{
    return _data ? &(_data->data) : nullptr;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator::depth() const noexcept
{
    size_t depth = 0;
    if (_data == nullptr) {
        return depth;
    }
    node* current = _data;
    while (current->parent != nullptr) {
        current = current->parent;
        ++depth;
    }
    return depth;
}

// endregion postfix_iterator implementation

// region postfix_const_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::postfix_const_iterator(const node* data, const node* backup)
    :_base(const_cast<node*>(data), const_cast<node*>(backup))
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::postfix_const_iterator(const postfix_iterator& it) noexcept
        :_base(it)
{
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator==(postfix_iterator const &other) const noexcept
{
    return _base == other;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator!=(postfix_iterator const &other) const noexcept
{
    return _base != other;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator++() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator--() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return temp;
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion postfix_const_iterator implementation

// region postfix_reverse_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::postfix_reverse_iterator(node* data, node* backup)
        : _base(data, backup)
{
}
template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::postfix_reverse_iterator(const postfix_iterator& it) noexcept
        : _base(it)
{
    --_base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator() const noexcept
{
    auto copy = _base;
    ++copy;
    return copy;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::base() const noexcept
{
    return _base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator==(postfix_reverse_iterator const &other) const noexcept
{
    return _base == other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator!=(postfix_reverse_iterator const &other) const noexcept
{
    return _base != other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator++() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator--() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator*()
{
    return _base ->operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion postfix_reverse_iterator implementation

// region postfix_const_reverse_iterator implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const node* data, const node* backup)
    :_base(data, backup)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::postfix_const_reverse_iterator(const postfix_const_iterator& it) noexcept
    :_base(it)
{
    --_base(it);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator() const noexcept
{
    auto copy = _base;
    ++copy;
    return copy;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::base() const noexcept
{
    return _base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator==(postfix_const_reverse_iterator const &other) const noexcept
{
    return _base == other._base;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator!=(postfix_const_reverse_iterator const &other) const noexcept
{
    return _base != other._base();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator++() & noexcept
{
    ++_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator++(int not_used) noexcept
{
    auto temp = *this;
    --_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator &
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator--() & noexcept
{
    --_base;
    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator const
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator--(int not_used) noexcept
{
    auto temp = *this;
    ++_base;
    return *temp;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::reference
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator*()
{
    return _base.operator*();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::pointer
binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::operator->() noexcept
{
    return _base.operator->();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator::depth() const noexcept
{
    return _base.depth();
}

// endregion postfix_const_reverse_iterator implementation

// region binary_search_tree implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(
        const compare& comp,
        pp_allocator<value_type> alloc,
        logger *logger)
        : compare(comp), _allocator(alloc), _logger(logger), _root(nullptr)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(
        pp_allocator<value_type> alloc,
        const compare& comp,
        logger *logger)
        : compare(comp), _allocator(alloc), _logger(logger), _root(nullptr)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<std::ranges::input_range Range>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(
        Range&& range,
        const compare& cmp,
        pp_allocator<value_type> alloc,
        logger* logger)
        : compare(cmp), _allocator(alloc), _logger(logger), _root(nullptr)
{
    for (auto&& element : range) {
        emplace(std::forward<decltype(element)>(element));
    }
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(
        std::initializer_list<std::pair<tkey, tvalue>> data,
        const compare& cmp,
        pp_allocator<value_type> alloc,
        logger* logger)
        : compare(cmp), _allocator(alloc), _logger(logger), _root(nullptr)
{
    for (const auto& element : data) {
        emplace(element.first, element.second);
    }
}

// endregion binary_search_tree implementation

// region binary_search_tree 5_rules implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(const binary_search_tree &other)
    : compare(other.compare),
      _allocator(other._allocator),
      _logger(other._logger),
      _root(nullptr)
{
    for (auto it = other.begin(); it != other.end(); ++it)
    {
        emplace(it->first, it->second);
    }
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::binary_search_tree(binary_search_tree &&other) noexcept
        : //compare(std::move(other.compare)),
          _allocator(std::move(other._allocator)),
          _logger(std::move(other._logger)),
          _root(other._root)
{// передали владение указателем
    other._root = nullptr;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>&
binary_search_tree<tkey, tvalue, compare, tag>::operator=(const binary_search_tree &other)
{
    if (this == &other) {
        return *this; // Защита от самоприсваивания
    }

    // compare = other.compare;
    _allocator = other._allocator;
    _size = other._size;
    _logger = other.loger;
    // Очистка текущего дерева
    clear();

    // Копирование данных из другого дерева
    for (auto it = other.begin(); it != other.end(); ++it) {
        emplace(it->first, it->second);
    }

    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>&
binary_search_tree<tkey, tvalue, compare, tag>::operator=(binary_search_tree &&other) noexcept
{
    if (this == &other) {
        return *this; // Защита от самоприсваивания
    }

    // Очистка текущего дерева
    clear();

    // Перенос данных из другого дерева
    //compare = std::move(other.compare);
    _allocator = std::move(other._allocator);
    _logger = std::move(other._logger);
    _root = other._root;
    _size = other._size;

    // Обнуление другого дерева
    other._root = nullptr;
    other._size = 0;

    return *this;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
binary_search_tree<tkey, tvalue, compare, tag>::~binary_search_tree()
{
    clear();
}

// endregion binary_search_tree 5_rules implementation

// region binary_search_tree methods_access implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
tvalue& binary_search_tree<tkey, tvalue, compare, tag>::at(const tkey& key)
{
    node* current = _root;
    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first))
        {
            current = current->left_subtree;
        }
        else if (compare_keys(current->data.first, key))
        {
            current = current->right_subtree;
        }
        else
        {
            return current->data.second;
        }
    }
    throw std::out_of_range("Key not found in binary search tree");
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
const tvalue& binary_search_tree<tkey, tvalue, compare, tag>::at(const tkey& key) const
{
    const node* current = _root;
    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first))
        {
            current = current->left_subtree;
        }
        else if (compare_keys(current->data.first, key))
        {
            current = current->right_subtree;
        }
        else
        {
            return current->data.second;
        }
    }
    throw std::out_of_range("Key not found in binary search tree");
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
tvalue& binary_search_tree<tkey, tvalue, compare, tag>::operator[](const tkey& key)
{
    node* current = _root;
    node* parent = nullptr;

    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first))
        {
            parent = current;
            current = current->left_subtree;
        }
        else if (compare_keys(current->data.first, key))
        {
            parent = current;
            current = current->right_subtree;
        }
        else
        {
            return current->data.second; // Ключ найден, возвращаем значение
        }
    }

    // Ключ не найден, создаем новый узел
    auto new_node = _allocator.allocate(1);
    _allocator.construct(new_node, std::make_pair(key, tvalue()), parent);

    if (parent == nullptr)
    {
        _root = new_node; // Дерево было пустым
    }
    else if (compare_keys(key, parent->data.first))
    {
        parent->left_subtree = new_node;
    }
    else
    {
        parent->right_subtree = new_node;
    }

    ++_size;
    return new_node->data.second;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
tvalue& binary_search_tree<tkey, tvalue, compare, tag>::operator[](tkey&& key)
{
    node* current = _root;
    node* parent = nullptr;

    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first))
        {
            parent = current;
            current = current->left_subtree;
        }
        else if (compare_keys(current->data.first, key))
        {
            parent = current;
            current = current->right_subtree;
        }
        else
        {
            return current->data.second; // Ключ найден, возвращаем значение
        }
    }

    // Ключ не найден, создаем новый узел
    auto new_node = _allocator.allocate(1);
    _allocator.construct(new_node, std::make_pair(std::move(key), tvalue()), parent);

    if (parent == nullptr)
    {
        _root = new_node; // Дерево было пустым
    }
    else if (compare_keys(new_node->data.first, parent->data.first))
    {
        parent->left_subtree = new_node;
    }
    else
    {
        parent->right_subtree = new_node;
    }

    ++_size;
    return new_node->data.second;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::empty() const noexcept
{
    return _root == nullptr;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::size() const noexcept
{
    return _size;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::clear() noexcept
{
    clear(_root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::clear(node* n) noexcept
{
    if (n == nullptr) {
        return;
    }
    clear(n->left_subtree);
    clear(n->right_subtree);
    _allocator.delete_object(n);
}

// endregion binary_search_tree methods_access implementation

// region binary_search_tree methods_insert and methods_emplace implementation

template <typename tkey, typename tvalue, compator<tkey> compare, typename tag>
std::pair<typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator, bool> binary_search_tree<
        tkey, tvalue, compare, tag>::insert(const value_type &value)
{
    node **current = &_root;
    node *parent = nullptr;

    while (*current != nullptr)
    {
        parent = *current;
        if (compare_keys(value.first, (*current)->data.first))
        {
            current = &((*current)->left_subtree);
        }
        else if (compare_keys((*current)->data.first, value.first))
        {
            current = &((*current)->right_subtree);
        }
        else
        {
            return {infix_iterator(*current), false};
        }
    }
    ++_size;
    *current = __detail::bst_impl<tkey, tvalue, compare, tag>::create_node(*this, parent, value);
    __detail::bst_impl<tkey, tvalue, compare, tag>::post_insert(*this, current);
    return {infix_iterator(*current), true};
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
std::pair<typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator, bool>
binary_search_tree<tkey, tvalue, compare, tag>::insert(value_type&& value)
{
    node **current = &_root;
    node *parent = nullptr;

    while (*current != nullptr)
    {
        parent = *current;
        if (compare_keys(value.first, (*current)->data.first))
        {
            current = &((*current)->left_subtree);
        }
        else if (compare_keys((*current)->data.first, value.first))
        {
            current = &((*current)->right_subtree);
        }
        else
        {
            return {infix_iterator(*current), false};
        }
    }

    ++_size;
    *current = __detail::bst_impl<tkey, tvalue, compare, tag>::create_node(*this, parent, std::move(value));
    __detail::bst_impl<tkey, tvalue, compare, tag>::post_insert(*this, current);
    return {infix_iterator(*current), true};
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<std::input_iterator InputIt>
void binary_search_tree<tkey, tvalue, compare, tag>::insert(InputIt first, InputIt last)
{
    for (auto it = first; it != last; ++it)
    {
        insert(*it);
    }
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<std::ranges::input_range R>
void binary_search_tree<tkey, tvalue, compare, tag>::insert_range(R&& rg)
{
    for (auto&& el: rg)
    {
        insert(std::forward<decltype(el)>(el));
    }
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template <class... Args>
std::pair<typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator, bool> binary_search_tree<
        tkey, tvalue, compare, tag>::emplace(Args &&...args)
{
    value_type value(std::forward<Args>(args)...);
    return insert(std::move(value));
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::insert_or_assign(const value_type& value)
{
    node **current = &_root;
    node *parent = nullptr;

    while (*current != nullptr)
    {
        parent = *current;
        if (compare_keys(value.first, (*current)->data.first))
        {
            current = &((*current)->left_subtree);
        }
        else if (compare_keys((*current)->data.first, value.first))
        {
            current = &((*current)->right_subtree);
        }
        else
        {
            (*current)->data.second = value.second;
            return {infix_iterator(*current)};
        }
    }

    ++_size;
    *current = __detail::bst_impl<tkey, tvalue, compare, tag>::create_node(*this, parent, value);
    __detail::bst_impl<tkey, tvalue, compare, tag>::post_insert(*this, current);
    return {infix_iterator(*current)};
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::insert_or_assign(value_type&& value)
{
    node **current = &_root;
    node *parent = nullptr;

    while (*current != nullptr)
    {
        parent = *current;
        if (compare_keys(value.first, (*current)->data.first))
        {
            current = &((*current)->left_subtree);
        }
        else if (compare_keys((*current)->data.first, value.first))
        {
            current = &((*current)->right_subtree);
        }
        else
        {
            (*current)->data.second = value.second;
            return {infix_iterator(*current)};
        }
    }

    ++_size;
    *current = __detail::bst_impl<tkey, tvalue, compare, tag>::create_node(*this, parent, std::move(value));
    __detail::bst_impl<tkey, tvalue, compare, tag>::post_insert(*this, current);
    return {infix_iterator(*current)};
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<std::input_iterator InputIt>
void binary_search_tree<tkey, tvalue, compare, tag>::insert_or_assign(InputIt first, InputIt last)
{
    for (auto it = first; it != last; ++it)
    {
        insert_or_assign(*it);
    }
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
template<class... Args>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::emplace_or_assign(Args&&... args) {
    value_type value(std::forward<Args>(args)...);
    return insert_or_assign(std::move(value));
}

// endregion binary_search_tree methods_insert and methods_emplace implementation

// region binary_search_tree swap_method implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::swap(binary_search_tree& other) noexcept
{
    std::swap(_root, other._root);
    std::swap(_size, other._size);
    std::swap(_allocator, other._allocator);
    std::swap(_logger, other._logger);
}

// endregion binary_search_tree swap_method implementation

// region binary_search_tree methods_search and methods_erase implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
bool binary_search_tree<tkey, tvalue, compare, tag>::contains(const tkey& key) const
{
    return find(key) != end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::find(const tkey& key)
{
    node* current = _root;
    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first))
        {
            current = current->left_subtree;
        }
        else if (compare_keys(current->data.first, key))
        {
            current = current->right_subtree;
        }
        else
        {
            return infix_iterator(current);
        }
    }
    return end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::find(const tkey& key) const
{
    node* current = _root;

    while(current != nullptr)
    {
        if (compare_keys(current->data.first, key))
        {
            current = current->left_subtree;
        }
        else if (compare_keys(key, current->data.first))
        {
            current = current->right_subtree;
        }
        else
        {
            return infix_const_iterator(current);
        }
    }
    return end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::lower_bound(const tkey& key)
{
    node* current = _root;
    node* result = nullptr;

    while (current != nullptr)
    {
        if (!compare_keys(current->data.first, key)) // current->data.first >= key
        {
            result = current;
            current = current->left_subtree;
        }
        else
        {
            current = current->right_subtree;
        }
    }

    return result ? infix_iterator(result) : end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::lower_bound(const tkey& key) const
{
    const node* current = _root;
    const node* result = nullptr;

    while (current != nullptr)
    {
        if (!compare_keys(current->data.first, key)) // current->data.first >= key
        {
            result = current;
            current = current->left_subtree;
        }
        else
        {
            current = current->right_subtree;
        }
    }

    return result ? infix_const_iterator(result) : end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::upper_bound(const tkey& key)
{
    node* current = _root;
    node* result = nullptr;

    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first)) // current->data.first > key
        {
            result = current;
            current = current->left_subtree;
        }
        else
        {
            current = current->right_subtree;
        }
    }

    return result ? infix_iterator(result) : end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::upper_bound(const tkey& key) const
{
    const node* current = _root;
    const node* result = nullptr;

    while (current != nullptr)
    {
        if (compare_keys(key, current->data.first)) // current->data.first > key
        {
            result = current;
            current = current->left_subtree;
        }
        else
        {
            current = current->right_subtree;
        }
    }

    return result ? infix_const_iterator(result) : end();
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::erase(infix_iterator pos)
{
    if (pos == end()) {
        throw std::out_of_range("Iterator out of range");
    }

    auto current = pos._data;
    auto next = pos;
    ++next;

    __detail::bst_impl<tkey, tvalue, compare, tag>::erase(*this, &current);

    return next;
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::erase(infix_const_iterator pos)
{
    if (pos == end()) {
        throw std::out_of_range("Iterator out of range");
    }
    return __detail::bst_impl<tkey, tvalue, compare, tag>::erase(*this, &pos._base._data);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::erase(infix_iterator first, infix_iterator last)
{
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::erase(infix_const_iterator first, infix_const_iterator last)
{
    auto it = first;
    while (it != last) {
        it = erase(it);
    }
    return infix_iterator(it._base._data);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
size_t binary_search_tree<tkey, tvalue, compare, tag>::erase(const tkey& key)
{
    auto it = find(key);
    if (it != end())
    {
        erase(it);

        return 1; // Удален один элемент
    }
    return 0; // Элемент с таким ключом не найден
}

// endregion binary_search_tree methods_search and methods_erase implementation

// region infix_iterators requests implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin() noexcept
{
    if (_root == nullptr)
    {
        return infix_iterator(nullptr);
    }

    node* current = _root;
    while (current ->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }
    return infix_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end() noexcept
{
    if (_root == nullptr)
    {
        return infix_iterator(nullptr);
    }

    node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }
    node* backup = current;
    return infix_iterator(nullptr, backup);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node* current = _root;
    while (current ->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }
    return infix_const_iterator(current);
}
template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    const node* backup = current;
    current = current->right_subtree;
    return infix_const_iterator(current, backup);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cbegin() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node* current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cend() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node* current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    const node* backup = current;
    current = current->right_subtree;
    return infix_const_iterator(current, backup);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin() noexcept
{
    if (_root == nullptr)
    {
        return infix_reverse_iterator(nullptr);
    }

    node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    return infix_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend() noexcept
{
    if (_root == nullptr)
    {
        return infix_reverse_iterator(nullptr);
    }

    node *current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_reverse_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    return infix_const_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend() const noexcept
{
    if (_root == nullptr)
    {
        return infix_iterator(nullptr);
    }

    const node *current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crbegin() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }
    return infix_const_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crend() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_const_reverse_iterator(nullptr, current);
}

// endregion infix_iterators requests implementation

// region prefix_iterators requests implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin_prefix() noexcept
{
    return prefix_iterator(_root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end_prefix() noexcept
{
    node* current = _root;
    while (current && (current->left_subtree || current->right_subtree)) {
        if (current->right_subtree) {
            current = current->right_subtree;
        } else {
            current = current->left_subtree;
        }
    }
    return prefix_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin_prefix() const noexcept
{
    node* current = _root;
    return prefix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end_prefix() const noexcept
{
    node* current = _root;
    while (current && (current->left_subtree || current->right_subtree)) {
        if (current->right_subtree) {
            current = current->right_subtree;
        } else {
            current = current->left_subtree;
        }
    }
    return prefix_const_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cbegin_prefix() const noexcept
{
    node* current = _root;
    return prefix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cend_prefix() const noexcept
{
    const node* current = _root;
    while (current && (current->left_subtree || current->right_subtree)) {
        if (current->right_subtree) {
            current = current->right_subtree;
        } else {
            current = current->left_subtree;
        }
    }
    return prefix_const_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin_prefix() noexcept
{
    const node* current = _root;
    while (current && (current->left_subtree || current->right_subtree)) {
        if (current->right_subtree) {
            current = current->right_subtree;
        } else {
            current = current->left_subtree;
        }
    }
    return prefix_const_iterator(current);
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend_prefix() noexcept
{
    return prefix_reverse_iterator(nullptr, _root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin_prefix() const noexcept
{
    const node* current = _root;
    while (current && (current->left_subtree || current->right_subtree)) {
        if (current->right_subtree) {
            current = current->right_subtree;
        } else {
            current = current->left_subtree;
        }
    }
    return prefix_const_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend_prefix() const noexcept
{
    const node* current = _root;
    return prefix_const_reverse_iterator(nullptr, _root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crbegin_prefix() const noexcept
{
    const node* current = _root;
    while (current && (current->left_subtree || current->right_subtree)) {
        if (current->right_subtree) {
            current = current->right_subtree;
        } else {
            current = current->left_subtree;
        }
    }
    return prefix_const_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::prefix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crend_prefix() const noexcept
{
    const node* current = _root;
    return prefix_const_reverse_iterator(nullptr, current);
}

// endregion prefix_iterators requests implementation

// region infix_iterators methods implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin_infix() noexcept
{
    if (_root == nullptr)
    {
        return infix_iterator(nullptr);
    }

    node* current = _root;
    while (current ->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }
    return infix_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end_infix() noexcept
{
    if (_root == nullptr)
    {
        return infix_iterator(nullptr);
    }

    node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }
    node* backup = current;
    return infix_iterator(nullptr, backup);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node* current = _root;
    while (current ->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }
    return infix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    const node* backup = current;
    current = current->right_subtree;
    return infix_const_iterator(current, backup);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cbegin_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_iterator(nullptr);
    }

    const node* current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_const_iterator(current);
}
template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cend_infix() const noexcept {
    if (_root == nullptr) {
        return infix_const_iterator(nullptr);
    }

    const node *current = _root;
    while (current->right_subtree != nullptr) {
        current = current->right_subtree;
    }

    const node *backup = current;
    current = current->right_subtree;
    return infix_const_iterator(current, backup);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin_infix() noexcept
{
    if (_root == nullptr)
    {
        return infix_reverse_iterator(nullptr);
    }

    node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    return infix_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend_infix() noexcept
{
    if (_root == nullptr)
    {
        return infix_reverse_iterator(nullptr);
    }

    node *current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_reverse_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    return infix_const_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }


    return infix_const_reverse_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crbegin_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->right_subtree != nullptr)
    {
        current = current->right_subtree;
    }

    return infix_const_reverse_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::infix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crend_infix() const noexcept
{
    if (_root == nullptr)
    {
        return infix_const_reverse_iterator(nullptr);
    }

    const node *current = _root;
    while(current->left_subtree != nullptr)
    {
        current = current->left_subtree;
    }

    return infix_const_reverse_iterator(nullptr, current);
}

// endregion infix_iterators methods implementation

// region postfix_iterators requests implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin_postfix() noexcept
{
    node* current = _root;
    if (!current) {
        return postfix_iterator(nullptr);
    }

    while (current->left_subtree || current->right_subtree) {
        if (current->left_subtree) {
            current = current->left_subtree;
        } else {
            current = current->right_subtree;
        }
    }

    return postfix_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end_postfix() noexcept
{
    return postfix_iterator(nullptr, _root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::begin_postfix() const noexcept
{
    const node* current = _root;
    if (!current) {
        return postfix_iterator(nullptr);
    }

    while (current->left_subtree || current->right_subtree) {
        if (current->left_subtree) {
            current = current->left_subtree;
        } else {
            current = current->right_subtree;
        }
    }

    return postfix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::end_postfix() const noexcept
{
    const node* current = _root;
    return postfix_const_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cbegin_postfix() const noexcept
{
    const node* current = _root;
    if (!current) {
        return postfix_iterator(nullptr);
    }

    while (current->left_subtree || current->right_subtree) {
        if (current->left_subtree) {
            current = current->left_subtree;
        } else {
            current = current->right_subtree;
        }
    }

    return postfix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_iterator
binary_search_tree<tkey, tvalue, compare, tag>::cend_postfix() const noexcept
{
    const node* current = _root;
    return postfix_const_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin_postfix() noexcept
{
    return postfix_reverse_iterator(_root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend_postfix() noexcept
{
    node* current = _root;
    if (!current) {
        return postfix_reverse_iterator(nullptr);
    }

    while (current->left_subtree || current->right_subtree) {
        if (current->left_subtree) {
            current = current->left_subtree;
        } else {
            current = current->right_subtree;
        }
    }

    return postfix_reverse_iterator(nullptr, current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rbegin_postfix() const noexcept
{
    const node* current = _root;
    return postfix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::rend_postfix() const noexcept
{
    const node* current = _root;
    if (!current) {
        return postfix_iterator(nullptr);
    }

    while (current->left_subtree || current->right_subtree) {
        if (current->left_subtree) {
            current = current->left_subtree;
        } else {
            current = current->right_subtree;
        }
    }

    return postfix_const_iterator(nullptr ,current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crbegin_postfix() const noexcept
{
    const node* current = _root;
    return postfix_const_iterator(current);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
typename binary_search_tree<tkey, tvalue, compare, tag>::postfix_const_reverse_iterator
binary_search_tree<tkey, tvalue, compare, tag>::crend_postfix() const noexcept
{
    const node* current = _root;
    if (!current) {
        return postfix_iterator(nullptr);
    }

    while (current->left_subtree || current->right_subtree) {
        if (current->left_subtree) {
            current = current->left_subtree;
        } else {
            current = current->right_subtree;
        }
    }

    return postfix_const_iterator(nullptr ,current);
}

// endregion postfix_iterators requests implementation

// endregion iterators requests implementation

//region subtree rotations implementation

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::small_left_rotation(node*& subtree_root) noexcept
{
    if (subtree_root == nullptr || subtree_root->right_subtree == nullptr)
        return;

    node* old_root = subtree_root;
    node* new_root = old_root->right_subtree;

    // Переместить левое поддерево нового корня к правому поддереву старого
    old_root->right_subtree = new_root->left_subtree;
    if (new_root->left_subtree)
        new_root->left_subtree->parent = old_root;

    // Обновить родителя нового корня
    new_root->parent = old_root->parent;

    // Подключить новый корень вместо старого
    new_root->left_subtree = old_root;
    old_root->parent = new_root;

    // Обновить указатель в родителе
    subtree_root = new_root;
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::small_right_rotation(node*& subtree_root) noexcept
{
    if (subtree_root == nullptr || subtree_root->left_subtree == nullptr)
        return;

    node* old_root = subtree_root;
    node* new_root = old_root->left_subtree;

    old_root->left_subtree = new_root->right_subtree;
    if (new_root->right_subtree)
        new_root->right_subtree->parent = old_root;

    new_root->parent = old_root->parent;

    new_root->right_subtree = old_root;
    old_root->parent = new_root;

    subtree_root = new_root;
}


template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::big_left_rotation(node *&subtree_root) noexcept
{
    if (subtree_root == nullptr || subtree_root->right_subtree == nullptr || subtree_root->right_subtree->left_subtree == nullptr)
    {
        return;
    }

    small_right_rotation(subtree_root->right_subtree);
    small_left_rotation(subtree_root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::big_right_rotation(node *&subtree_root) noexcept
{
    if (subtree_root == nullptr || subtree_root->left_subtree == nullptr || subtree_root->left_subtree->right_subtree == nullptr)
    {
        return;
    }

    small_left_rotation(subtree_root->left_subtree);
    small_right_rotation(subtree_root);
}

template<typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::double_left_rotation(node *&subtree_root) noexcept
{
    if (subtree_root == nullptr || subtree_root->left_subtree == nullptr)
    {
        return;
    }
    small_left_rotation(subtree_root->left_subtree);
    small_left_rotation(subtree_root);
}

template <typename tkey, typename tvalue, compator<tkey> compare, typename tag>
void binary_search_tree<tkey, tvalue, compare, tag>::double_right_rotation(node *&subtree_root) noexcept
{
    if (subtree_root == nullptr || subtree_root->right_subtree == nullptr)
    {
        return;
    }
    small_right_rotation(subtree_root->right_subtree);
    small_right_rotation(subtree_root);
}

//endregion subtree rotations implementation

namespace __detail {
    template<typename tkey, typename tvalue, typename compare, typename tag>
    template<typename ...Args>
    typename binary_search_tree<tkey, tvalue, compare, tag>::node *
    bst_impl<tkey, tvalue, compare, tag>::create_node(binary_search_tree<tkey, tvalue, compare, tag> &cont,
                                                      Args &&...args) {
        auto *new_node = cont._allocator.template new_object<typename binary_search_tree<tkey, tvalue, compare, tag>::node>(std::forward<Args>(args)...);
        return new_node;
    }

    template <typename tkey, typename tvalue, typename compare, typename tag>
    void bst_impl<tkey, tvalue, compare, tag>::delete_node(binary_search_tree<tkey, tvalue, compare, tag> &cont,
                                                           binary_search_tree<tkey, tvalue, compare, tag>::node **node)
    {
        using node_type = typename binary_search_tree<tkey, tvalue, compare, tag>::node;
        if (node && *node)
        {
            cont._allocator.template delete_object<node_type>(*node);
            *node = nullptr;
        }
    }




    template <typename tkey, typename tvalue, typename compare, typename tag>
    void bst_impl<tkey, tvalue, compare, tag>::erase(
            binary_search_tree<tkey, tvalue, compare, tag> &cont,
            typename binary_search_tree<tkey, tvalue, compare, tag>::node **node_ptr)
    {
        using node_type = typename binary_search_tree<tkey, tvalue, compare, tag>::node;

        if (node_ptr == nullptr || *node_ptr == nullptr)
        {
            return;
        }

        node_type *node = *node_ptr;


        if (node->left_subtree == nullptr && node->right_subtree == nullptr)
        {
            if (node->parent)
            {
                if (node->parent->left_subtree == node)
                    node->parent->left_subtree = nullptr;
                else
                    node->parent->right_subtree = nullptr;
            }
            else
            {
                cont._root = nullptr;
            }
            delete_node(cont, node_ptr);
        }
            // Узел с одним поддеревом
        else if (node->left_subtree == nullptr || node->right_subtree == nullptr)
        {

            node_type *child = node->left_subtree != nullptr ? node->left_subtree : node->right_subtree;
            child->parent = node->parent;

            if (node->parent)
            {
                if (node->parent->left_subtree == node)
                {
                    node->parent->left_subtree = child;
                }
                else
                    node->parent->right_subtree = child;
            }
            else
            {
                cont._root = child;
            }
            delete_node(cont, node_ptr);
        }
            //Узел с двумя поддеревьями
        else // У узла два поддерева
        {
            // Находим predecessor (максимальный в левом поддереве)
            node_type *predecessor = node->left_subtree;
            while (predecessor->right_subtree)
            {
                predecessor = predecessor->right_subtree;
            }

            // Если predecessor не является прямым левым потомком
            if (predecessor->parent != node)
            {
                predecessor->parent->right_subtree = predecessor->left_subtree;
                if (predecessor->left_subtree)
                {
                    predecessor->left_subtree->parent = predecessor->parent;
                }

                predecessor->left_subtree = node->left_subtree;
                if (predecessor->left_subtree)
                {
                    predecessor->left_subtree->parent = predecessor;
                }
            }

            // Переносим правое поддерево
            predecessor->right_subtree = node->right_subtree;
            if (predecessor->right_subtree)
            {
                predecessor->right_subtree->parent = predecessor;
            }

            // Обновляем родителя
            predecessor->parent = node->parent;

            // Обновляем ссылку родителя
            if (node->parent)
            {
                if (node->parent->left_subtree == node)
                {
                    node->parent->left_subtree = predecessor;
                }
                else
                {
                    node->parent->right_subtree = predecessor;
                }
            }
            else
            {
                cont._root = predecessor;
            }

            delete_node(cont, node_ptr);
        }

        --cont._size;
    }


}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_BINARY_SEARCH_TREE_H