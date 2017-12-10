#pragma once
#ifndef SQUIRREL_BIND_TYPE_HEADER_H
#define SQUIRREL_BIND_TYPE_HEADER_H

#include <squirrel.h>

#if defined(DLL_CONFIG) && defined(_MSC_VER)
    #ifdef SQUIRREL_BIND_EXPORTS
		#define SQBIND_API __declspec(dllexport)
    #else
		#define SQBIND_API __declspec(dllimport)
    #endif
#else
    #define SQBIND_API
#endif

namespace SquirrelBind {
    enum class SqType {
        NULLPTR = OT_NULL,
        INTEGER = OT_INTEGER,
        FLOAT = OT_FLOAT,
        BOOL = OT_BOOL,
        STRING = OT_STRING,
        TABLE = OT_TABLE,
        ARRAY = OT_ARRAY,
        USERDATA = OT_USERDATA,
        CLOSURE = OT_CLOSURE,
        NATIVECLOSURE = OT_NATIVECLOSURE,
        GENERATOR = OT_GENERATOR,
        USERPOINTER = OT_USERPOINTER,
        THREAD = OT_THREAD,
        FUNCPROTO = OT_FUNCPROTO,
        CLASS = OT_CLASS,
        INSTANCE = OT_INSTANCE,
        WEAKREF = OT_WEAKREF,
        OUTER = OT_OUTER
    };

    SQBIND_API const char* sqTypeToStr(SqType type);
}

#endif