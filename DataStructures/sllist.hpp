/* 
    SINGLY LINKED LIST
*/

#ifndef SLLIST_H
#define SLLIST_H

#include <iostream>

namespace adstl
{

template <typename T> class sllist;
template <typename T> std::ostream& operator<<(std::ostream&, const sllist<T>&); 

template <typename T>
class Node final
{
    friend class sllist<T>;
    friend std::ostream& operator<< <T> (std::ostream&, const sllist<T>&);

    Node(const T &data) : data(data), next(nullptr) {} // cpy ctor
    Node(T &&data) : data(std::move(data)), next(nullptr) {} // move ctor
    ~Node() {} // dctor

    private:
        T data;
        Node *next;
};

template <typename T>
class sllist final
{

    friend std::ostream& operator<< <T> (std::ostream&, const sllist<T>&);

    private:
        class iterator;
        class const_iterator;

    public:
        typedef T l_type;
        typedef iterator iterator;
        typedef const_iterator const_iterator;

        sllist() : head(nullptr), sz(0) {} // def ctor
        sllist(const sllist&); // copy ctor
        sllist(sllist&&) noexcept; // move ctor
        ~sllist(); // dctor

        sllist& operator=(const sllist&); // cpy=
        sllist& operator=(sllist&&) noexcept; // move=

        T& operator[](size_t);
        const T& operator[](size_t) const;

        // iterator interface
        iterator begin() { return iterator(head); }
        const_iterator cbegin() const { return const_iterator(head); }

        iterator end() { return iterator(nullptr); }
        const_iterator cend() const { return const_iterator(nullptr); }

        size_t size() const { return sz; }
        template <typename U> void push_back(U&&);
        void pop_back(); // pop_back is so unefficient in singly linked list, complexity O(n)
        void clear();
        template <typename U> void insert(size_t, U&&);

    private:

        class iterator
        {
            public:
                iterator(Node<T> *it) : it(it) {}

                T& operator*() const 
                {
                    return it->data;
                }

                iterator& operator++()
                {
                    it = it->next;
                    return *this;
                }

                bool operator!=(const iterator &rhs) const
                {
                    return it != rhs.it;
                }

            private:
                Node<T> *it;
        };

        class const_iterator
        {
            public:
                const_iterator(Node<T> *it) : it(it) {}

                const T& operator*() const 
                {
                    return it->data;
                }

                const_iterator& operator++()
                {
                    it = it->next;
                    return *this;
                }

                bool operator!=(const const_iterator &rhs) const
                {
                    return it != rhs.it;
                }

            private:
                Node<T> *it;
        };

        Node<T> *head;
        size_t sz;

};

template <typename T>
std::ostream& operator<<(std::ostream &os, const sllist<T> &list)
{
    Node<T> *tmp_node = list.head;
    while(tmp_node)
    {
        os << tmp_node->data << " ";
        tmp_node = tmp_node->next;
    }
    return os;
}

// cpy ctor
template <typename T>
sllist<T>::sllist(const sllist &rhs) : head(nullptr), sz(0)
{
    if(rhs.head != nullptr)
    {
        Node<T> *tmp_node = rhs.head;
        while(tmp_node)
        {
            push_back(tmp_node->data);
            tmp_node = tmp_node->next;
        }
    }
}

// move ctor
template <typename T>
sllist<T>::sllist(sllist &&rhs) noexcept : head(rhs.head), sz(rhs.sz)
{
    rhs.head = nullptr;
    rhs.sz = 0;
}

// cpy=
template <typename T>
sllist<T>& sllist<T>::operator=(const sllist &rhs)
{
    if(this != &rhs)
    {
        Node<T> *current_node = head;
        while(current_node != nullptr)
        {
            Node<T> *next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }

        head = nullptr;
        sz = 0;

        current_node = rhs.head;
        while(current_node)
        {
            push_back(current_node->data);
            current_node = current_node->next;
        }
    }

    return *this;
}

// move=
template <typename T>
sllist<T>& sllist<T>::operator=(sllist &&rhs) noexcept
{
    if(this != &rhs)
    {
        Node<T> *current_node = head;
        while(current_node != nullptr)
        {
            Node<T> *next_node = current_node->next;
            delete current_node;
            current_node = next_node;
        }

        head = rhs.head;
        sz = rhs.sz;

        rhs.head = nullptr;
        rhs.sz = 0;
    }

    return *this;
}

// op []
template <typename T>
T& sllist<T>::operator[](size_t index)
{
    Node<T> *current_node = head;
    size_t current_index = index;
    while(current_node != nullptr)
    {
        if(current_index == 0)
        {
            return current_node->data;
        }
        current_node = current_node->next;
        --current_index;
    }
    throw std::out_of_range("Index:" + std::to_string(index) + " is out of range");
}

// op []
template <typename T>
const T& sllist<T>::operator[](size_t index) const
{
    Node<T> *current_node = head;
    size_t current_index = index;
    while(current_node != nullptr)
    {
        if(current_index == 0)
        {
            return current_node->data;
        }
        current_node = current_node->next;
        --current_index;
    }
    throw std::out_of_range("Index:" + std::to_string(index) + " is out of range");
}

template <typename T>
sllist<T>::~sllist()
{
    Node<T> *current_node = head;
    while(current_node != nullptr)
    {
        Node<T> *next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }
    head = nullptr;
}

template <typename T>
template <typename U>
void sllist<T>::push_back(U &&data)
{
    Node<T> *new_node = new Node<T>(std::forward<U>(data));

    if(head == nullptr)
    {
        head = new_node;
        ++sz;
        return;
    }

    Node<T> *tmp_node = head;
    while(tmp_node->next)
    {
        tmp_node = tmp_node->next;
    }

    tmp_node->next = new_node;

    ++sz;
}

template <typename T>
template <typename U>
void sllist<T>::insert(size_t position, U &&data)
{
    if(position > sz)
    {
        throw std::out_of_range("Inserting on positon:" + std::to_string(position) + " while list have:" + std::to_string(sz) + " elements.");
    }

    if(position == 0)
    {
        Node<T>* new_node = new Node<T>(std::forward<U>(data));
        new_node->next = head;
        head = new_node;
        ++sz;
        return;
    }

    Node<T>* current_node = head;
    for(size_t i = 1; i < position; ++i)
    {
        current_node = current_node->next;
    }

    Node<T>* new_node = new Node<T>(std::forward<U>(data));
    new_node->next = current_node->next;
    current_node->next = new_node;
    ++sz;
}

template <typename T>
void sllist<T>::pop_back()
{
    if(head == nullptr)
    {    
        return;
    }
    else if(head->next == nullptr)
    {
        delete head;
        head = nullptr;
        --sz;
        return;
    }
    else
    {
        Node<T> *current_node = head;
        while(current_node->next->next)
        {
            current_node = current_node->next;
        }

        delete current_node->next;
        current_node->next = nullptr;
        --sz;
    }
}

template <typename T>
void sllist<T>::clear()
{
    if(head == nullptr)
        return;

    Node<T> *current_node = head;
    while(current_node != nullptr)
    {   
        Node<T> *next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }
    head = nullptr;
    sz = 0;
}

}


#endif