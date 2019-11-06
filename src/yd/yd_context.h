#if !defined(YD_CONTEXT)

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

#if !defined(YD_DO_JOIN2)
# define YD_DO_JOIN2(Arg1, Arg2) Arg1 ## Arg2
#endif // #if !defined(YD_DO_JOIN2)

#if !defined(YD_JOIN2)
# define YD_JOIN2(Arg1, Arg2) YD_DO_JOIN2(Arg1, Arg2)
#endif // #if !defined(YD_JOIN2)

//
// NOTE(yuval): Type Definitions & Exported Variable Declarations
//

struct Context {
    struct Memory_Arena* arena;
};

extern Context context_;
extern Context* context;

struct Context_Push_Block {
    Context original_context;
    
    Context_Push_Block(Context new_context) {
        original_context = *context;
        *context = new_context;
    }
    
    ~Context_Push_Block() {
        *context = original_context;
    }
};

#define PUSH_CONTEXT(new_context) Context_Push_Block YD_JOIN2(context_block_, __COUNTER__)(new_context);

#define YD_CONTEXT
#endif // #if !defined(YD_CONTEXT)

//
// NOTE(yuval): YD Context Implementation
//

#if defined(YD_CONTEXT_IMPLEMENTATION)

//
// NOTE(yuval): Exported Variable Instantiations
//

Context context_;
Context* context = &context_;

#endif // #if defined(YD_CONTEXT_IMPLEMENTATION)
