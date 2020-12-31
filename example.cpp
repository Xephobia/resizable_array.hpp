#include <iostream>
#include "resizable_array.hpp"

int main(){
    xeph::resizable_array<std::string , 5> xeph_array;
    
    xeph_array.push_back("one");
    xeph_array.push_back("two");
    xeph_array.push_back("three");

    xeph::resizable_array<std::string , 5> xeph_array_copy( xeph_array );

    return 0;
}