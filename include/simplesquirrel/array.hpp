#pragma once
#ifndef SSQ_ARRAY_HEADER_H
#define SSQ_ARRAY_HEADER_H

#include "object.hpp"
#include "args.hpp"
#include <squirrel.h>
#include <vector>

namespace ssq {
    /**
    * @brief Squirrel intance of array object
    * @ingroup simplesquirrel
    */
    class SSQ_API Array: public Object {
    public:
        /**
        * @brief Constructs empty array
        */
        Array(HSQUIRRELVM vm, size_t len = 0);
        /**
        * @brief Destructor
        */
        virtual ~Array() = default;
        /**
        * @brief Constructs array out of std::vector
        */
        template<typename T>
        Array(HSQUIRRELVM vm, const std::vector<T>& vector):Object(vm) {
            sq_newarray(vm, 0);
            sq_getstackobj(vm, -1, &obj);
            sq_addref(vm, &obj);

            for(const auto& val : vector) {
                detail::push(vm, val);
                if(SQ_FAILED(sq_arrayappend(vm, -2))) {
                    sq_pop(vm, 2);
                    throw TypeException("Failed to push value to back of the array");
                }
            }

            sq_pop(vm, 1); // Pop array
        }
        /**
        * @brief Converts Object to Array
        * @throws TypeException if the Object is not type of an array
        */
        explicit Array(const Object& object);
        /**
        * @brief Copy constructor
        */
        Array(const Array& other);
        /**
        * @brief Move constructor
        */
        Array(Array&& other) NOEXCEPT;
        /**
        * @brief Returns the size of the array
        */
        size_t size();
        /**
        * @brief Pushes an element to the back of the array
        */
        template<typename T>
        void push(const T& value) {
            sq_pushobject(vm, obj);
            detail::push(vm, value);
            if(SQ_FAILED(sq_arrayappend(vm, -2))) {
                sq_pop(vm, 2);
                throw TypeException("Failed to push value to back of the array");
            }
            sq_pop(vm, 1);
        }
        /**
        * @brief Pops an element from the back of the array and returns it
        */
        template<typename T>
        T popAndGet() {
            sq_pushobject(vm, obj);
            auto s = sq_getsize(vm, -1);
            if(s == 0) {
                sq_pop(vm, 1);
                throw TypeException("Out of bounds");
            }

            try {
                if(SQ_FAILED(sq_arraypop(vm, -1, true))) {
                    sq_pop(vm, 1);
                    throw TypeException("Failed to pop value from back of the array");
                }
                T ret(detail::pop<T>(vm, -1));
                sq_pop(vm, 1);
                return std::move(ret);
            } catch (...) {
                sq_pop(vm, 1);
                std::rethrow_exception(std::current_exception());
            }
        }
        /**
        * @brief Pops an element from the back of the array
        */
        void pop();
        /**
        * @brief Returns an element from the specific index
        * @throws TypeException if the index is out of bounds or element cannot be returned
        */
        template<typename T>
        T get(size_t index) {
            sq_pushobject(vm, obj);
            auto s = static_cast<size_t>(sq_getsize(vm, -1));
            if(index >= s) {
                sq_pop(vm, 1);
                throw TypeException("Out of bounds");
            }
            detail::push(vm, index);
            if(SQ_FAILED(sq_get(vm, -2))) {
                sq_pop(vm, 1);
                throw TypeException("Failed to get value from the array");
            }
            try {
                T ret(detail::pop<T>(vm, -1));
                sq_pop(vm, 2);
                return std::move(ret);
            } catch (...) {
                sq_pop(vm, 2);
                std::rethrow_exception(std::current_exception());
            }
        }
        /**
         * Returns the element at the start of the array
         * @throws TypeException if the array is empty or element cannot be returned
         */
        template<typename T>
        T begin() {
            return get<T>(0);
        }
        /**
         * Returns the element at the end of the array
         * @throws TypeException if the array is empty or element cannot be returned
         */
        template<typename T>
        T back() {
            auto s = size();
            if (s == 0) throw TypeException("Out of bounds");
            return get<T>(s - 1);
        }
        /**
        * @brief Sets an element at the specific index
        * @throws TypeException if the index is out of bounds or element cannot be set
        */
        template<typename T>
        void set(size_t index, const T& value) {
            sq_pushobject(vm, obj);
            auto s = static_cast<size_t>(sq_getsize(vm, -1));
            if(index >= s) {
                sq_pop(vm, 1);
                throw TypeException("Out of bounds");
            }
            detail::push(vm, index);
            detail::push(vm, value);
            if(SQ_FAILED(sq_set(vm, -3))) {
                sq_pop(vm, 1);
                throw TypeException("Failed to set value in the array");
            }
            sq_pop(vm, 1);
        }
        /**
         * @brief Converts this array to std::vector of objects
         */
        std::vector<Object> convertRaw();
        /**
         * @brief Converts this array to std::vector of specific type T
         */
        template<typename T>
        std::vector<T> convert() {
            sq_pushobject(vm, obj);
            auto s = static_cast<size_t>(sq_getsize(vm, -1));
            std::vector<T> ret;
            ret.reserve(s);
            while(s--) {
                sq_arraypop(vm, -1, true);
                ret.push_back(detail::pop<T>(vm, -1));
            }
            sq_pop(vm, 1);
            return ret;
        }
        /**
        * @brief Copy assingment operator
        */ 
        Array& operator = (const Array& other);
        /**
        * @brief Move assingment operator
        */
        Array& operator = (Array&& other) NOEXCEPT;
    };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline Array popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_ARRAY);
            Array val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw TypeException("Could not get Array from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }
    }
#endif
}

#endif