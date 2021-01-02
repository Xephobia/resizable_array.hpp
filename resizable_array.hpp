#ifndef _XEPH_RESIZABLE_ARRAY_HPP_
#define _XEPH_RESIZABLE_ARRAY_HPP_

#include <cstddef>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <iterator>

namespace xeph
{
    template<
            typename T ,
            std::size_t N ,
            typename Allocator = std::allocator<T>
            >

    class resizable_array
    {
        public :

            //typedefs :

            typedef T                                       value_type;
            typedef T*                                      pointer;
            typedef const T*                                const_pointer;
            typedef value_type&                             reference;
            typedef const value_type&                       const_reference;
            typedef value_type*                             iterator;
            typedef const value_type*                       const_iterator;
            typedef std::size_t                             size_type;
            typedef std::ptrdiff_t                          difference_type;
            typedef std::reverse_iterator<iterator>         reverse_iterator;
            typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;


            //constructors/destructor :

            resizable_array()
            {
                
                real_array = allocator_object.allocate( N );
                array_size = 0;
                array_capacity = N;
            } // default constructor

            resizable_array(const resizable_array& other):
                array_size( other.array_size ),
                array_capacity( other.array_capacity )
            {
                real_array = allocator_object.allocate( other.array_capacity );

                std::copy_n
                        (
                            other.real_array,
                            other.array_size,
                            real_array
                        );
            } // copy constructor

            resizable_array(resizable_array&& other):
                real_array( other.real_array ),
                array_size( array_size=other.array_size ),
                array_capacity( other.array_capacity)
            {
                //taking care of the rvalue
                other.real_array = nullptr;
                other.array_size = 0;
                other.array_capacity = 0;
            } // move constructor

            ~resizable_array()
            {
                allocator_object.deallocate( real_array , array_capacity );
            } // destructor


            // capacity :

            size_type size()
            {
                return array_size;
            } // get the number of ellements in the array

            size_type capacity()
            {
                return array_capacity;
            } // get the capacity of the array

            bool empty() const noexcept
            {
                return size() == 0;
            }


            // Element access :

            reference at(std::size_t resizable_array_index)
            {
                if( resizable_array_index >= array_size )
                {
                    std::out_of_range at_out_of_range_error( "out of bounds! : resizable_array_index >= array_capacity" );
                    throw( at_out_of_range_error );
                }else
                {
                    return real_array[ resizable_array_index ];
                }
                
            } // array indexing with bound checking ; throws if resizable_array_index >= array_capacity
            
            reference operator[] (std::size_t resizable_array_index)
            {
                return real_array[ resizable_array_index ];
            } // array indexing without bound check. Cause UB if the element is nonexistant
            
            reference front()
            {

            }

            reference back()
            {

            }

            pointer data()
            {
                if ( size() == 0)
                {
                    return nullptr;
                }else
                {
                    return real_array;
                }
                
            }


            //iterators :

            iterator begin() noexcept
            {
                return real_array;
            }

            const_iterator cbegin() const noexcept
            {
                return const_iterator( data() );
            }

            iterator end() noexcept
            {
                return data() + size();
            }
            
            const_iterator cend() const noexcept
            {
                return const_iterator( end() );
            }

            reverse_iterator rbegin() noexcept
            {
                return reverse_iterator( end() );
            }

            const_reverse_iterator crbegin() const noexcept
            {
                return const_reverse_iterator( end() );
            }

            reverse_iterator rend() noexcept
            {
                return reverse_iterator( begin() );
            }

            const_reverse_iterator crend() const noexcept
            {
                return const_reverse_iterator( begin() );
            }


            // modifiers :

            void resize(std::size_t resize_capacity)
            {
                if( resize_capacity < array_size )
                {
                    std::logic_error resize_error( "resize_capacity is smaller than the number of elements in the array!" );

                    throw( resize_error );
                }else
                {
                    pointer new_array = allocator_object.allocate( resize_capacity );

                    std::uninitialized_move_n(
                                this -> real_array ,
                                this -> array_size ,
                                new_array
                            );

                    allocator_object.deallocate( real_array , array_capacity);

                    real_array = new_array;

                    array_capacity = resize_capacity;
                }
            } // resize (just allocate memory)

            void push_back( const_reference value)
            {
                real_array[ array_size ] = std::uninitialized_copy( value );
                ++array_size;
            }// copying push_back

            void push_back( value_type&& value)
            {
                std::uninitialized_move_n
                                        (
                                        std::addressof( value ),
                                        1,
                                        std::addressof( real_array[ size() ] )
                                        );
                ++array_size;
            }// moving push_back

            void pop_back()
            {
                std::allocator_traits<Allocator>::destroy
                                                        (
                                                            allocator_object,
                                                            std::addressof( real_array[ size() ] ) 
                                                        );
                
                --array_size;
            }// pop back

        private :
            pointer real_array;
            size_type array_size;
            size_type array_capacity;

            Allocator allocator_object;
    };
}
#endif