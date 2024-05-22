#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <memory>

namespace adstl
{

template <typename T> class vector;
template <typename T> std::ostream& operator<<(std::ostream&, const vector<T>&); 

template <typename T>
class vector final
{

    friend std::ostream& operator<< <T>(std::ostream&, const vector<T>&);

    public:
        typedef T v_type;
        
        static void change_realloc_size(const size_t sz)
        {
            reallocate_size = sz;
        }

        vector() : elements(nullptr), first_free(nullptr), cap(nullptr) {} // default constructor

        vector(const vector&);            // copy constructor
        vector& operator=(const vector&); // copy assignment   

        vector(vector &&) noexcept; // move constructor 
        vector& operator=(vector &&) noexcept; // move assigment    
                
        // additional constructor
        vector(const T*, const T*);

        ~vector(); 

        void push_back(const T&);  // copy the element
        void push_back(T&&); // move the element

        // add elements
        size_t size() const { return first_free - elements; }
        size_t capacity() const { return cap - elements; }

        // iterator interface
        T* begin() const { return elements; }
        T* end() const { return first_free; }

        T& operator[](std::size_t n) 
            { return elements[n]; }

        const T& operator[](std::size_t n) const 
            { return elements[n]; }

    private:
        static std::allocator<T> alloc;
        static size_t reallocate_size;

        void chk_n_alloc() 
        {
            if (size() == capacity())
                reallocate(); 
        }

        // used by the copy constructor, assignment operator, and destructor
        std::pair<T*, T*> alloc_n_copy(const T*, const T*);

        void free();             // destroy the elements and free the space
        void reallocate();       // get more space and copy the existing elements

        T *elements;   // pointer to the first element in the array
        T *first_free; // pointer to the first free element in the array
        T *cap;        // pointer to one past the end of the array
};

template <typename T>
std::allocator<T> vector<T>::alloc;

template <typename T>
size_t vector<T>::reallocate_size = 2;

template <typename T>
std::ostream& operator<<(std::ostream &os, const vector<T> &rhs)
{
    for(T *b = rhs.begin(); b != rhs.end(); ++b)
    {
        os << *b << " ";
    }
    return os;
}

template <typename T>
std::pair<T*, T*> vector<T>::alloc_n_copy(const T *begin, const T *end)
{
    T *data = alloc.allocate(end - begin);
    return std::make_pair(data, std::uninitialized_copy(begin, end, data));
}

// Constructors

template <typename It>
inline vector<It>::vector(const It *begin, const It *end)
{
    std::pair<It*, It*> new_data = alloc_n_copy(begin, end);
    elements = new_data.first;
    first_free = cap = new_data.second;
}

// cpy constructor
template <typename T>
inline vector<T>::vector(const vector<T> &rhs)
{
    std::pair<T*, T*> new_data = alloc_n_copy(rhs.begin(), rhs.end());
    elements = new_data.first;
    first_free = cap = new_data.second;
}

// move constructor
template <typename T>
inline vector<T>::vector(vector<T> &&rhs) noexcept
{
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;

    rhs.elements = rhs.first_free = rhs.cap = nullptr;
}

// = ops

// cpy=
template <typename T>
inline vector<T>& vector<T>::operator=(const vector<T> &rhs)
{
	// call alloc_n_copy to allocate exactly as many elements as in rhs
	std::pair<T*, T*> data = 
							alloc_n_copy(rhs.begin(), rhs.end());

	free();

	elements = data.first;
	first_free = cap = data.second;

	return *this;
}

// move=
template <typename T>
inline vector<T>& vector<T>::operator=(vector<T> &&rhs) noexcept
{
	elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;

    rhs.elements = rhs.first_free = rhs.cap = nullptr;

    return *this;
}

template <typename T>
inline vector<T>::~vector()
{
    free();
}

// cpy push back
template <typename T>
inline void vector<T>::push_back(const T &elem)
{
    chk_n_alloc(); // ensure that there is room for another element

    // construct a copy of s in the element to which first_free points
    alloc.construct(first_free++, elem);  
}

// move push back
template <typename T>
inline void vector<T>::push_back(T &&elem)
{
    chk_n_alloc(); // ensure that there is room for another element

    // construct a copy of s in the element to which first_free points
    alloc.construct(first_free++, std::move(elem));  
}



template <typename T>
inline void vector<T>::reallocate()
{
    // we'll allocate space for twice as many elements as the current size
    size_t new_capacity = size() ? reallocate_size * size() : 1;

	// allocate new memory
	T *new_data = alloc.allocate(new_capacity);

	// copy the data from the old memory to the new
	T *dest = new_data;  // points to the next free position in the new array
    T *elem = elements; // points to the next element in the old array

	for (size_t i = 0; i != size(); ++i)
    {
        // check if move construcotr of T obj is nothrowable
        if constexpr (std::is_nothrow_move_constructible_v<T>)
        {
            alloc.construct(dest++, std::move(*elem++));
        }
        else
        {
		    alloc.construct(dest++, *elem++);
        }
    }

	free();  // free the old space once we've moved the elements

    // update our data structure to point to the new elements
    elements = new_data;
    first_free = dest;
    cap = elements + new_capacity;
}

template <typename T>
inline void vector<T>::free()
{
    // may not pass deallocate a 0 pointer; if elements is 0, there's no work to do
	if (elements) {
    	// destroy the old elements in reverse order
		for (T *p = first_free; p != elements;)
			alloc.destroy(--p);  
        
		alloc.deallocate(elements, cap - elements);
	}
}

}

#endif