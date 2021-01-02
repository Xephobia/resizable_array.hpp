#include <iostream>
#include "resizable_array.hpp"

int main(){
    xeph::resizable_array<std::string , 5> xeph_array;
    
    xeph_array.push_back("one");
    xeph_array.push_back("two");
    xeph_array.push_back("three");

    std::cout << xeph_array.size() << '\n'; // will print 3

    for(auto str : xeph_array)
    {
        std::cout << str << '\n';
    }

    xeph::resizable_array<std::string , 5> xeph_array_copy( xeph_array ); // copy xeph::resizable_array

    xeph_array_copy.pop_back();

    std::cout << xeph_array_copy.at( 3 ); // will error

    return 0;
}