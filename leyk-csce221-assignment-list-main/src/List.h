#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() { 
            node = nullptr;
         };
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return node->data;
        }
        pointer operator->() const {
            return &node->data;
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
            basic_iterator nodeCopy = *this;
            node = node->next;
            return nodeCopy;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
             basic_iterator nodeCopy = *this;
            node = node->prev;
            return nodeCopy;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            return node != other.node;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() {
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;
        _size = 0;
    }
    List( size_type count, const T& value ) {
        head.next = &tail;
        tail.prev = &head;
        
        for(int i = 0; i < count; i++) {
            Node *tmp = new Node;
            tmp->data = value;

            tmp->next = &tail;
            tmp->prev = tail.prev;

            (tmp->prev)->next = tmp;
            tail.prev = tmp;
       }
       _size = count;
    }
    explicit List( size_type count ) {
        head.next = &tail;
        tail.prev = &head;
        for(int i = 0; i < count; i++) {
            Node *tmp = new Node(T());

            tmp->next = &tail;
            tmp->prev = tail.prev;

            (tmp->prev)->next = tmp;
            tail.prev = tmp;
        }
       _size = count;
    }
    List( const List& other ) {
        head.next = &tail;
        tail.prev = &head;
        _size = 0;

        if(other.head.next == &other.tail) {
            return;
        }

        Node* tmp = other.head.next;
        for(int i = 0; i < other.size(); i++) {
            push_back(tmp->data);
            tmp = tmp->next;
        }        
    }
    List( List&& other ) {
        if(other.head.next == &other.tail) {
            head.next = &tail;
            tail.prev = &head;
            _size = 0;
            return;
        }

        head.next = other.head.next;
        (head.next)->prev = &head;

        tail.prev = other.tail.prev;
        (tail.prev)->next = &tail;

        _size = other.size();

        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        other._size = 0;
    }
    ~List() {
        clear();
    }
    List& operator=( const List& other ) {
        if(&head == &other.head) {
            return *this;
        }

        clear();
        
        if(other.head.next == &other.tail) {
            return *this;
        }
        
        Node* tmp = other.head.next;
        for(int i = 0; i < other.size(); i++) {
            push_back(tmp->data);
            tmp = tmp->next;
        } 

        return *this;
    }
    List& operator=( List&& other ) noexcept {
        if(&head == &other.head) {
            return *this;
        }

        clear();

        if(other.head.next == &other.tail) {
            head.next = &tail;
            tail.prev = &head;
            _size = 0;
            return *this;
        }

        head.next = other.head.next;
        (head.next)->prev = &head;

        tail.prev = other.tail.prev;
        (tail.prev)->next = &tail;

        _size = other.size();

        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        other._size = 0;

        return *this;
    }

    reference front() {
        return (head.next)->data;
    }
    const_reference front() const {
        return (head.next)->data;
    }
	
    reference back() {
        return (tail.prev)->data;
    }
    const_reference back() const {
        return (tail.prev)->data;
    }
	
    iterator begin() noexcept {
        iterator first(head.next);
        return first;
    }
    const_iterator begin() const noexcept {
        const_iterator first(head.next);
        return first;
    }
    const_iterator cbegin() const noexcept {
        const_iterator first(head.next);
        return first;
    }

    iterator end() noexcept {
        iterator last(&tail);
        return last;
    }
    const_iterator end() const noexcept {
        const_iterator last(&tail);
        return last;
    }
    const_iterator cend() const noexcept {
        const_iterator last(&tail);
        return last;
    }

    bool empty() const noexcept {
        return size() == 0;        
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        if(empty()) {
            return;
        }
        
        Node* curr = head.next;
        while(head.next != &tail)   {
            curr = head.next->next;
            delete head.next;
            head.next = curr;
        }

        head.next = &tail;
        tail.prev = &head;
        _size = 0;
    }

    iterator insert( const_iterator pos, const T& value ) {
        Node *tmp = new Node;
        tmp->data = value;
        
        tmp->next = pos.node;
        tmp->prev = pos.node->prev;

        (tmp->next)->prev = tmp;
        (tmp->prev)->next = tmp;

        _size++;
        iterator newPos(tmp);
        return newPos;
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node *tmp = new Node;
        tmp->data = std::move(value);

        tmp->next = pos.node;
        tmp->prev = pos.node->prev;

        (tmp->next)->prev = tmp;
        (tmp->prev)->next = tmp;

        _size++;
        iterator newPos(tmp);
        return newPos;
    }

    iterator erase( const_iterator pos ) {
        (pos.node->next)->prev = pos.node->prev;
        (pos.node->prev)->next = pos.node->next;

        iterator newPos(pos.node->next);

        delete pos.node;
        _size--;

        return newPos;
    }

    void push_back( const T& value ) {
        Node *tmp = new Node;
        tmp->data = value;

        tmp->next = &tail;
        tmp->prev = tail.prev;

        (tmp->prev)->next = tmp;
        tail.prev = tmp;

        _size++;
    }
    void push_back( T&& value ) {
        Node *tmp = new Node;
        tmp->data = std::move(value);

        tmp->next = &tail;
        tmp->prev = tail.prev;

        (tmp->prev)->next = tmp;
        tail.prev = tmp;

        _size++;
    }

    void pop_back() {
        tail.prev = (tail.prev)->prev;
        delete (tail.prev)->next;
        (tail.prev)->next = &tail;

        _size--;
    }
	
    void push_front( const T& value ) {
        Node *tmp = new Node;
        tmp->data = value;

        tmp->next = head.next;
        tmp->prev = &head;

        (tmp->next)->prev = tmp;
        head.next = tmp;

        _size++;
    }
	void push_front( T&& value ) {
        Node *tmp = new Node;
        tmp->data = std::move(value);

        tmp->next = head.next;
        tmp->prev = &head;

        (tmp->next)->prev = tmp;
        head.next = tmp;

        _size++;
    }

    void pop_front() {
        head.next = (head.next)->next;
        delete (head.next)->prev;
        (head.next)->prev = &head;

        _size--;
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}