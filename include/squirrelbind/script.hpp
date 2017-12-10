#pragma once
#ifndef SQUIRREL_BIND_SCRIPT_HEADER_H
#define SQUIRREL_BIND_SCRIPT_HEADER_H

namespace SquirrelBind {
    /**
    * @brief Squirrel script object
    */
    class SQBIND_API SqScript: public SqObject {
    public:
        /**
        * @brief Creates empty script object
        */
        SqScript(HSQUIRRELVM vm);
        /**
        * @brief Swaps two objects
        */
        void swap(SqScript& other) NOEXCEPT;
        /**
        * @brief Deleted copy constructor
        */
        SqScript(const SqScript& other) = delete;
        /**
        * @brief Move constructor
        */
        SqScript(SqScript&& other) NOEXCEPT;
        /**
        * @brief Deleted copy assignment operator
        */
        SqScript& operator = (const SqScript& other) = delete;
        /**
        * @brief Move assignment operator
        */
        SqScript& operator = (SqScript&& other) NOEXCEPT;
    };
}

#endif