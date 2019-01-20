#pragma once
#ifndef SSQ_EXCEPTIONS_HEADER_H
#define SSQ_EXCEPTIONS_HEADER_H

#include <string>
#include <sstream>
#include "type.hpp"

namespace ssq {
    /**
    * @brief Raw exception
    * @ingroup simplesquirrel
    */
    class Exception: public std::exception {
    public:
        Exception(const char* msg):message(msg) {

        }
        virtual const char* what() const throw() override {
            return message;
        }
    private:
        const char* message;
    };
    /**
    * @brief Not Found exception thrown if object with a given name does not exist
    * @ingroup simplesquirrel
    */
    class NotFoundException: public Exception {
    public:
        NotFoundException(const char* msg):Exception(msg) {
            std::stringstream ss;
            ss << "Not found: " << msg;
            message = ss.str();
        }

        virtual const char* what() const throw() override {
            return message.c_str();
        }
    private:
        std::string message;
    };
    /**
    * @brief Compile exception thrown during compilation
    * @ingroup simplesquirrel
    */
    class CompileException: public Exception {
    public:
        CompileException(const char* msg):Exception(msg) { 
            message = std::string(msg);
        }

        CompileException(const char* msg, const char* source, int line, int column):Exception(msg) { 
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
    * @ingroup simplesquirrel
    */
    class TypeException: public Exception {
    public:
        TypeException(const char* msg):Exception(msg) {
            message = std::string(msg);
        }

        TypeException(const char* msg, const char* expected, const char* got):Exception(msg) {
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
    * @ingroup simplesquirrel
    */
    class RuntimeException: public Exception {
    public:
        RuntimeException(const char* msg):Exception(msg) {
            message = std::string(msg);
        }

        RuntimeException(const char* msg, const char* source, const char* func, int line):Exception(msg) {
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