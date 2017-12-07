#pragma once
#ifndef SQUIRREL_BIND_EXCEPTIONS_HEADER_H
#define SQUIRREL_BIND_EXCEPTIONS_HEADER_H

#include <string>
#include <sstream>
#include "type.hpp"

namespace SquirrelBind {
    /**
    * @brief Raw exception
    */
    class SqException: public std::exception {
    public:
        SqException(const char* msg):message(msg) {

        }
        virtual const char* what() const throw() override {
            return message;
        }
    private:
        const char* message;
    };
    /**
    * @brief Not Found exception thrown if object with a given name does not exist
    */
    class SqNotFoundException: public SqException {
    public:
        SqNotFoundException(const char* msg):SqException(msg) {
            
        }
    };
    /**
    * @brief Compile exception thrown during compilation
    */
    class SqCompileException: public SqException {
    public:
        SqCompileException(const char* msg, const char* source, int line, int column):SqException(msg) { 
            std::stringstream ss;
            ss << "Compile error at " << source << ":" << line << ":" << column << " " << msg;
            message = ss.str();
        }
        
        virtual const char* what() const throw() override {
            return message.c_str();
        }
    private:
        std::string message;
    };
    /**
    * @brief Type exception thrown if casting between squirrel and C++ objects failed
    */
    class SqTypeException: public SqException {
    public:
        SqTypeException(const char* msg):SqException(msg) {
            message = std::string(msg);
        }

        SqTypeException(const char* msg, const char* expected, const char* got):SqException(msg) {
            std::stringstream ss;
            ss << "Type error " << msg << " expected: " << expected << " got: " << got;
            message = ss.str();
        }

        virtual const char* what() const throw() override {
            return message.c_str();
        }
    private:
        std::string message;
    };
    /**
    * @brief Runtime exception thrown if something went wrong during execution
    */
    class SqRuntimeException: public SqException {
    public:
        SqRuntimeException(const char* msg):SqException(msg) {
            message = std::string(msg);
        }

        SqRuntimeException(const char* msg, const char* source, const char* func, int line):SqException(msg) {
            std::stringstream ss;
            ss << "Runtime error at (" << func << ") " << source << ":" << line << ": " << msg;
            message = ss.str();
        }

        virtual const char* what() const throw() override {
            return message.c_str();
        }
    private:
        std::string message;
    };
}

#endif