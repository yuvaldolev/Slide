#if !defined(YD_ARRAY)

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define yd_internal static
#define yd_global static
#define yd_local_persist static

typedef int8_t yd_s8;
typedef int16_t yd_s16;
typedef int32_t yd_s32;
typedef int64_t yd_s64;

typedef yd_s8 yd_b8;
typedef yd_s32 yd_b32;

typedef uint8_t yd_u8;
typedef uint16_t yd_u16;
typedef uint32_t yd_u32;
typedef uint64_t yd_u64;

typedef float yd_f32;
typedef double yd_f64;

typedef intptr_t yd_smm;
typedef uintptr_t yd_umm;
#define YD_TYPES
#endif // #if !defined(YD_TYPES)

//
// NOTE(yuval): Utility Macros
//

#if !defined(YD_ASSERT)
# define YD_ASSERT(expression) if (!(expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YD_ASSERT)

//
// NOTE(yuval): Type Definitions
//

template <typename T>
struct Array {
    T* data;
    yd_umm count;
    yd_umm capacity;
    
    inline T& operator[](yd_umm index) {
        YD_ASSERT(index < count);
        
        T& result = data[index];
        return result;
    }
    
    inline const T& operator[](yd_umm index) const {
        YD_ASSERT(index < count);
        
        const T& result = data[index];
        return result;
    }
};


#define YD_ARRAY
#endif // #if !defined(YD_ARRAY)

// NOTE(yuval): YD Input Implementation
//

#if defined(YD_ARRAY_IMPLEMENTATION)

//
// NOTE(yuval): Exported Function Implementations
//

template <typename T>
void
array_add(Array<T>* array, T value) {
    yd_umm new_count = array->count + 1;
    if (new_count > array->capacity) {
        yd_umm new_capacity = 0;
        if (array->capacity == 0) {
            // NOTE(yuval): First allocation is 8
            new_capacity = 8;
        } else {
            // NOTE(yuval): Following allocations grow array by 25%
            new_capacity = (new_count + (new_count >> 2));
        }
        
        T* new_data = PUSH_ARRAY(T, new_capacity);
        COPY_ARRAY(new_data, array_data, array->count);
        
        array->data = new_data;
        array->capacity = new_capacity;
    }
    
    array->data[array->count] = value;
    array->count = new_count;
}

template <typename T>
void
array_remove(Array<T>* array, T value) {
    
}

#endif // #if defined(YD_ARRAY_IMPLEMENTATION)


