#include "DataStructures/vector.hpp"

struct Foo
{
    friend std::ostream& operator<<(std::ostream&, const Foo&);

    Foo(int a = 0, int b = 0) : a(a), b(b) {}
    
    Foo(const Foo &rhs) : a(rhs.a), b(rhs.b) {std::cout << "cpy=" << std::endl;}
    Foo(Foo &&rhs) noexcept : a(rhs.a), b(rhs.b) {std::cout << "move=" << std::endl;}

    Foo& operator=(const Foo &rhs)
    {
        std::cout << "cpy=" << std::endl;

        if(this != &rhs)
        {
            a = rhs.a;
            b = rhs.b;
        }

        return *this;
    }

    Foo& operator=(Foo &&rhs) noexcept
    {
        std::cout << "move=" << std::endl;

        if(this != &rhs)
        {
            a = rhs.a;
            b = rhs.b;

            rhs.a = 0;
            rhs.b = 0;
        }
        
        return *this;
    }


    int a;
    int b;
};

std::ostream& operator<<(std::ostream &os, const Foo &foo)
{
    os << foo.a << "-" << foo.b;
    return os;
}

int main(int argc, char **argv)
{

    adstl::vector<int>::change_realloc_size(3);

    adstl::vector<int> vec1;
    vec1.push_back(2);
    vec1.push_back(3);
    vec1.push_back(4);
    vec1.push_back(5);
    adstl::vector<int> vec2 = vec1;
    
    std::cout << vec2 << std::endl;
    
    vec2.push_back(123);

    adstl::vector<int> vec3;
    vec3 = vec2;

    std::cout << vec3 << std::endl;

    adstl::vector<int> vec4(std::move(vec3));

    std::cout << vec4 << std::endl;

    adstl::vector<int> vec5;
    vec5 = std::move(vec4);

    std::cout << vec5 << std::endl;

    std::cout << vec5[3] << std::endl;

    adstl::vector<Foo> fVec1;
    fVec1.push_back(Foo(2,3));
    fVec1.push_back(Foo(3,4));
    fVec1.push_back(Foo(2,3));
    fVec1.push_back(Foo(3,4));
    fVec1.push_back(Foo(2,3));
    fVec1.push_back(Foo(3,4));
    Foo f(31, 213);
    fVec1.push_back(f);
    std::cout << "#" << std::endl;
    adstl::vector<Foo> fVec2(fVec1.begin(), fVec1.end());

    std::cout << fVec2 << std::endl;

    return 0;
}