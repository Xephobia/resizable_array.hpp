#ifndef _XEPH_RESIZABLE_ARRAY_HPP_
#define _XEPH_RESIZABLE_ARRAY_HPP_

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <algorithm>

namespace xeph
{
    template<
            typename Type ,
            std::size_t base_array_size ,
            typename Allocator = std::allocator<Type>
            >

    class resizable_array
    {
        public :
            //constructors/destructor :

            resizable_array()
            {
                
                this -> real_array = allocator_object.allocate( base_array_size );
                this -> array_size = 0;
                this -> array_capacity = base_array_size;
            } // default constructor

            resizable_array(const resizable_array& other)
            {
                (*this).array_size = other.array_size;
                (*this).array_capacity = other.array_capacity;
                
                std::copy_n(
                            other.real_array ,
                            other.array_size ,
                            this -> real_array
                        );
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
                allocator_object.deallocate( real_array , array_capacity );
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
            
            // resizing :
            void resize(std::size_t resize_capacity)
            {
                if( resize_capacity < (this -> array_size ) )
                {
                    std::logic_error resize_error( "resize_capacity is smaller than the number of elements in the array!" );
                    throw(resize_error);
                }else
                {
                    auto new_array = Allocator::allocate( resize_capacity );

                    std::copy(
                                real_array ,
                                real_array + resize_capacity ,
                                new_array
                            );

                    Allocator::deallocate( real_array , array_capacity);

                    real_array = new_array;

                    array_capacity = resize_capacity;
                    

                }
                
            } // resize (just allocate memory)


            // pop / push back / front

            void push_back( const Type& value)
            {
                this -> real_array[ array_size - 1 ] = value;
                ++array_size;
            }// copying push_back

            void push_back( Type&& value)
            {
                real_array[ array_size ] = value;
                value = nullptr;
                ++array_size;
            }// moving push_back

            void pop_back()
            {
                real_array[ array_size - 1 ] = Type::~Type();
                --array_size;
            }// pop back

        private :
            Type *real_array;
            std::size_t array_size;
            std::size_t array_capacity;

            Allocator allocator_object;
    };
}
#endif