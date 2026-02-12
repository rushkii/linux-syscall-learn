#ifndef TYPE_H
#define TYPE_H

#ifdef __LP64__
    #define NULL ((void *)0L)

    #define ulong unsigned long
    #define uint unsigned int
    #define ushort unsigned short

    typedef ulong size_t;
    #define __SIZE_T_DEFINED

    typedef long ssize_t;

    typedef char* str;
    typedef const char* cstr;
#else
    #define NULL ((void *)0)

    #define ulong unsigned long
    #define uint unsigned int
    #define ushort unsigned short

    typedef uint size_t;
    #define __SIZE_T_DEFINED

    typedef int ssize_t;

    typedef char* str;
    typedef const char* cstr;
#endif

#endif
