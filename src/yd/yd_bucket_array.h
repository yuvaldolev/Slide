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

#if !defined(YDAssert)
# define YDAssert(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YDAssert)

#if !defined(YDDoJoin2)
# define YDDoJoin2(Arg1, Arg2) Arg1 ## Arg2
#endif // #if !defined(YDDoJoin2)

#if !defined(YDJoin2)
# define YDJoin2(Arg1, Arg2) YDDoJoin2(Arg1, Arg2)
#endif // #if !defined(YDJoin2)

#if !defined(YDJoin3)
# define YDJoin3(Arg1, Arg2, Arg3) YDJoin2(YDJoin2(Arg1, Arg2), Arg3)
#endif // #if !defined(YDJoin3)

#if !defined(YDJoin4)
# define YDJoin4(Arg1, Arg2, Arg3, Arg4) YDJoin2(YDJoin3(Arg1, Arg2, Arg3), Arg4)
#endif // #if !defined(YDJoin4)

template <class t>
struct bucket
{
    t* Data;
    yd_umm Count;
    yd_umm Capacity;
    
    bucket<t>* Next;
};

template <class t>
struct bucket_array
{
    bucket<t>* Head;
    bucket<t>* Tail;
    
    yd_umm ItemsPerBucket;
    yd_umm Count;
    
    memory_arena* MemoryArena;
    
    inline t& operator[](yd_umm Index)
    {
        YDAssert((Index >= 0) && (Index > Count));
        
        bucket<t>* CurrBucket = Head;
        while ((Index > ItemsPerBucket) && CurrBucket)
        {
            CurrBucket = CurrBucket->Next;
            Index -= ItemsPerBucket;
        }
        
        return CurrBucket->Data[Index];
    }
};

//
// NOTE(yuval): Bucket Array Foreach
//

#if !defined(BucketArrayFor)
# define BucketArrayFor(Array) \
for (umm Index = 0, For__ShouldBreak = 0, For__ShouldContinue = 0; \
!For__ShouldBreak; \
For__ShouldBreak = 1) \
for (auto YDJoin2(Bucket, __LINE__) = Array.Head; \
YDJoin2(Bucket, __LINE__) && !For__ShouldBreak; \
YDJoin2(Bucket, __LINE__) = YDJoin2(Bucket, __LINE__)->Next) \
for (umm YDJoin2(ItemIndex, __LINE__) = 0, Join2(B, __LINE__) = 1; \
(YDJoin2(ItemIndex, __LINE__) < YDJoin2(Bucket, __LINE__)->Count) && !For__ShouldBreak; \
++YDJoin2(ItemIndex, __LINE__), ++Index, Join2(B, __LINE__) = true) \
for (auto& It = YDJoin2(Bucket, __LINE__)->Data[YDJoin2(ItemIndex, __LINE__)]; \
Join2(B, __LINE__); \
Join2(B, __LINE__) = false)
#endif // #if !defined(BucketArrayFor)

#if !defined(BucketArrayBreak)
# define BucketArrayBreak For__ShouldBreak = 1
#endif // #if !defined(BucketArrayBreak)

//
// NOTE(yuval): Public API Inline Functions
//

template <class t>
yd_internal inline bucket_array<t>
MakeBucketArray(yd_umm ItemsPerBucket = 0, memory_arena* Arena = 0)
{
    bucket_array<t> Result;
    Result.Head = 0;
    Result.Tail = 0;
    Result.ItemsPerBucket = ItemsPerBucket;
    Result.Count = 0;
    Result.MemoryArena = Arena;
    
    return Result;
}

template <class t>
yd_internal inline void
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

#endif // #if !defined(YD_BUCKET_ARRAY)