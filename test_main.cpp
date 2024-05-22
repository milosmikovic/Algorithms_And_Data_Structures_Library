#include "DataStructures/vector.hpp"

int main(int argc, char **argv)
{

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

    return 0;
}