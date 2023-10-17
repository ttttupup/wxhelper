#ifndef BASE_BASE_CONFIG_H_
#define BASE_BASE_CONFIG_H_
// Lib/Dll switch
#if !defined(BASE_EXPORTS) && !defined(BASE_IMPORTS) && !defined(BASE_STATIC)
#define BASE_STATIC
#endif

#if defined(_MSC_VER)

    #ifndef COMPILER_MSVC
        #define COMPILER_MSVC 1
    #endif

    #if defined(BASE_IMPORTS)
        #define BASE_API __declspec(dllimport)
    #elif defined(BASE_EXPORTS)
        #define BASE_API __declspec(dllexport)
    #else
        #define BASE_API
    #endif

#elif defined(__GNUC__)
    #define COMPILER_GCC
    #define BASE_API
#else
    #error "Unknown or unsupported compiler"
#endif


#endif
