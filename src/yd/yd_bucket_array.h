#if !defined(YD_BUCKET_ARRAY)

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
struct Bucket
{
    T* data;
    yd_umm count;
    yd_umm capacity;
    
    Bucket<T>* next;
};

template <typename T>
struct Bucket_Array {
    Bucket<T>* head;
    Bucket<T>* tail;
    
    Bucket<T>* unfull_bucket_list;
    
    yd_umm items_per_bucket;
    yd_umm count;
    
    Memory_Arena* memory_arena;
    
    inline t& operator[](yd_umm index)
    {
        YD_ASSERT((index >= 0) && (index < Count));
        
        Bucket<t>* curr_bucket = head;
        while ((index >= items_per_bucket) && curr_bucket)
        {
            curr_bucket = curr_bucket->xext;
            index -= items_per_bucket;
        }
        
        YD_ASSERT(curr_bucket);
        
        return curr_bucket->data[index];
    }
};

//
// NOTE(yuval): Public API Inline Functions
//

template <typename T>
yd_internal inline
bucket_array_init(Bucket_Array<T>* array,
                  yd_umm ItemsPerBucket = 0, memory_arena* Arena = 0) {
    a
}

template <typename T>
yd_internal inline Bucket_Array<T>
make_bucket_array(yd_umm ItemsPerBucket = 0, memory_arena* Arena = 0)
{
    Bucket_Array<T> result;
    result.head = 0;
    result.tail = 0;
    result.items_per_bucket = items_per_bucket;
    result.count = 0;
    result.MemoryArena = Arena;
    
    return Result;
}

#define YD_BUCKET_ARRAY
#endif // #if !defined(YD_BUCKET_ARRAY)

//
// NOTE(yuval): YD Bucket Array Implementation
//

#if defined(YD_BUCKET_ARRAY_IMPLEMENTATION)

template <typename T>
void
BucketArrayAdd(bucket_array<t>* Array, t Value)
{
    bucket<t>* Tail = Array->Tail;
    
    if (!Tail || (Tail->Count == Tail->Capacity))
    {
        // NOTE(yuval): We need to allocate a new bucket
        if (!Array->MemoryArena)
        {
            Array->MemoryArena = GlobalMemoryArena;
        }
        
        Array->Tail = PushStruct(Array->MemoryArena, bucket<t>);
        
        if (Tail)
        {
            Tail->Next = Array->Tail;
        }
        else
        {
            Array->Head = Array->Tail;
        }
        
        Tail = Array->Tail;
        
        if (Array->ItemsPerBucket == 0)
        {
            Array->ItemsPerBucket = 8;
        }
        
        // TODO(yuval): Maybe allocate array without initializing to zero using the
        // arena push params
        Tail->Data = PushArray(Array->MemoryArena, t, Array->ItemsPerBucket);
        Tail->Capacity = Array->ItemsPerBucket;
    }
    
    Tail->Data[Tail->Count++] = Value;
    ++Array->Count;
}

#endif // #if defined(YD_BUCKET_ARRAY_IMPLEMENTATION)