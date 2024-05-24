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
class Node
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

    public:
        sllist() : head(nullptr) {} // def ctor
        sllist(const sllist&); // copy ctor
        sllist(sllist&&); // move ctor
        ~sllist(); // dctor

        sllist& operator=(const sllist&); // cpy=
        sllist& operator=(sllist&&); // move=

        T& operator[](size_t);
        const T& operator[](size_t) const;

        template <typename U>
        void push_back(U&&);

    private:
        Node<T> *head;

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
sllist<T>::sllist(const sllist &rhs) : head(nullptr)
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
sllist<T>::sllist(sllist &&rhs) : head(rhs.head)
{
    rhs.head = nullptr;
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
sllist<T>& sllist<T>::operator=(sllist &&rhs)
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
        rhs.head = nullptr;
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
        return;
    }

    Node<T> *tmp_node = head;
    while(tmp_node->next)
    {
        tmp_node = tmp_node->next;
    }

    tmp_node->next = new_node;
}


}


#endif