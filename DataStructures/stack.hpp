/* 
    STACK
*/

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "vector.hpp"
#include "config.hpp" // Include the configuration header

namespace adstl
{

template <typename T> class stack;
template <typename T> std::ostream& operator<<(std::ostream&, const stack<T>&);

template <typename T>
class stack final
{

    friend std::ostream& operator<< <T> (std::ostream&, const stack<T>&);

    public:

        using s_type = T;

        stack() : data() {} // def ctor
        stack(const stack&); // cpy ctor
        stack(stack&&); // move ctor

        stack& operator=(const stack&); // cpy=
        stack& operator=(stack&&); // move=

        template <typename U> void push(U&&);
        void pop();
        T& top();
        const T& top() const;
        bool empty() const;
        size_t size() const;

        

    private:
        vector<T> data;
};

template <typename T>
std::ostream& operator<<(std::ostream &os, const stack<T> &stack)
{
    os << stack.data;
    return os;
}

// cpy ctor
template <typename T>
stack<T>::stack(const stack &rhs) : data(rhs.data) {}

// move ctor
template <typename T>
stack<T>::stack(stack &&rhs) : data(std::move(rhs.data)) {}

// cpy=
template <typename T>
stack<T>& stack<T>::operator=(const stack& rhs)
{
    data = rhs.data;
    return *this;
}

// move=
template <typename T>
stack<T>& stack<T>::operator=(stack &&rhs)
{
    data = std::move(rhs.data);
    return *this;
}

template <typename T>
template <typename U>
void stack<T>::push(U &&element)
{
    data.push_back(std::forward<U>(element));
}

template <typename T>
void stack<T>::pop()
{
    if(data.size())
    {
        data.pop_back();
    }
    #ifdef ADSTL_THROWABLE
    else
    {
        throw std::out_of_range("stack::pop: stack is empty.");
    }
    #endif
}

template <typename T>
T& stack<T>::top()
{
    if(data.size())
    {
        return data[data.size() - 1];
    }
    #ifdef ADSTL_THROWABLE
    throw std::out_of_range("stack::top: stack is empty.");
    #endif
}

template <typename T>
const T& stack<T>::top() const
{
    if(data.size())
    {
        return data[data.size() - 1];
    }
    #ifdef ADSTL_THROWABLE
    throw std::out_of_range("stack::top: stack is empty.");
    #endif
}

template <typename T>
bool stack<T>::empty() const
{
    return data.size() == 0;
}

template <typename T>
size_t stack<T>::size() const
{
    return data.size();
}



}


#endif