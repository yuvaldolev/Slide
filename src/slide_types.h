#if !defined(SLIDE_TYPES_H)

//////////////////////////////
//        Compilers         //
//////////////////////////////
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif // #if !defined(COMPILER_MSVC)

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif // #if !defined(COMPILER_LLVM)

#if !COMPILER_MSVC && !COMPILER_LLVM
#if defined(_MSC_VER)
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
// TODO(yuval, eran): Support More Compilers!
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif // #if defined(_MSC_VER)
#endif // #if !COMPILER_MSVC && !COMPILER_LLVM

#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error Optimizations are not available for this compiler yet!!!
#endif // #if COMPILER_MSVC

//////////////////////////////
//          Types           //
//////////////////////////////
#include <stdint.h>
#include <stddef.h>

#define internal static
#define global static
#define local_persist static

// NOTE(yuval): To be used only in the same function
// where the array was defined or with arrays that are defined
// within structs
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#define OFFSET_OF(type, member) (umm)&(((type*)0)->member)

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef s32 b32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef intptr_t smm;
typedef uintptr_t umm;

#define U16_MAX 65535

#define BUILD_TYPES_H
#endif // #if !defined(SLIDE_TYPES_H)
