#ifndef _XEPH_RESIZABLE_ARRAY_HPP_
#define _XEPH_RESIZABLE_ARRAY_HPP_

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace xeph
{
    template<
            class Type ,
            std::size_t base_array_size ,
            class Allocator = std::allocator<Type>
            >

    class resizable_array
    {
        public :
            //constructors/destructor :

            resizable_array()
            {
                this -> real_array = Allocator.allocate( base_array_size );
                this -> array_size = 0;
                this -> array_capacity = base_array_size;
            } // default constructor

            resizable_array(const resizable_array& other)
            {
                this -> real_array=other.real_array;
                this -> array_size=other.array_size;
                this -> array_capacity=other.array_capacity;
            } // copy constructor

            resizable_array(resizable_array&& other)
            {
                this -> real_array=other.real_array;
                this -> array_size=other.array_size;
                this -> array_capacity=other.array_capacity;

                //taking care of the rvalue
                other.real_array=nullptr;
                other.array_size=0;
                other.array_capacity=0;
            } // move constructor

            ~resizable_array()
            {
                Allocator.deallocate( real_array , array_capacity );
            } // destructor


            //capacity functions :

            std::size_t size()
            {
                return this -> array_size;
            } // get the number of ellements in the array

            std::size_t capacity()
            {
                return this -> array_capacity;
            } // get the capacity of the array


            // indexing functions :

            Type& at(std::size_t resizable_array_index)
            {
                if( resizable_array_index >= ( this -> array_capacity ) )
                {
                    std::out_of_range at_out_of_range_error( "out of bounds! : resizable_array_index >= (this -> array_capacity)" );
                    throw(at_out_of_range_error);
                }else
                {
                    return this -> real_array[ resizable_array_index ];
                }
                
            } // array indexing with bound checking ; throws if resizable_array_index >= (this -> array_capacity)
            
            Type& operator[] (std::size_t resizable_array_index)
            {
                return this -> real_array[ resizable_array_index ];
            } // array indexing without bound check. Cause UB if the element is nonexistant
            
            // other :
            void resize(std::size_t resize_capacity)
            {
                if( resize_capacity < (this -> array_size ) )
                {
                    std::logic_error resize_error( "resize_capacity is smaller than the number of elements in the array!" );
                    throw(resize_error);
                }else
                {
                    auto new_array = Allocator.allocate( resize_capacity );

                    std::copy(
                                real_array ,
                                real_array + resize_capacity ,
                                new_array
                            );

                    Allocator.deallocate( real_array , array_capacity);

                    real_array = new_array;

                    array_capacity = resize_capacity;
                    

                }
                
            } // resize (just allocate memory) 
        private :
            Type *real_array;
            std::size_t array_size;
            std::size_t array_capacity;
    };
}
#endif