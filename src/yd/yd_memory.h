#if !defined(YD_MEMORY)

/* NOTE(yuval):

Custom Memory Allocation & Deallocation Functions Can Be
Specified To Be Used With This Library!!!

By Default The Library Uses A Straightforward Memory Allocation Function
That Just Allocates A New Memory Block And Sets Its Flags.

You can specify a memory allocation function by initializing
YDAllocateMemory to your allocation function.

You can specify a memory deallocation function by initializing
YDDeallocateMemory to your deallocation function.

Both your memory allocation and deallocation functions must be declared
using the ALLOCATE_MEMORY and DEALLOCATE_MEMORY macros in order to be
compatible with the library.

*/

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
// NOTE(yuval): Platform Definitions
//

#if !defined(YD_WIN32)
# if defined(_WIN32)
#  define YD_WIN32 1
# else
#  define YD_WIN32 0
# endif // # if defined(_WIN32)
#endif // #if !defined(YD_WIN32)

#if !defined(YD_MACOS)
# if defined(__APPLE__)
#  define YD_MACOS 1
# else
#  define YD_MACOS 0
# endif // # if defined(__APPLE__)
#endif // #if !defined(YD_MACOS)

#if !defined(YD_LINUX)
# if defined(__linux__)
#  define YD_LINUX 1
# else
#  define YD_LINUX 0
# endif // # if defined(__linux__)
#endif // #if !defined(YD_LINUX)

//
// NOTE(yuval): Utility Macros
//

#if !defined(YD_ASSERT)
# define YD_ASSERT(expression) if (!(expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YD_ASSERT)

#if !defined(YD_MINIMUM)
#define YD_MINIMUM(a, b) (((a) < (b)) ? (a) : (b))
#endif // #if !defined(YD_MINIMUM)

#if !defined(YD_MAXIMUM)
# define YD_MAXIMUM(a, b) (((a) > (b)) ? (a) : (b))
#endif // #if !defined(YD_MAXIMUM)

#if !defined(YD_ALIGN_POW2)
# define YD_ALIGN_POW2(value, alignment) ((value + ((alignment) - 1)) & \
~((value - value) + (alignment) - 1))
#endif // #if !defined(YD_ALIGN_POW2)

#if !defined(YD_OFFSET_OF)
# define YD_OFFSET_OF(type, member) (umm)&(((type*)0)->member)
#endif // #if !defined(YD_OFFSET_OF)

//
// NOTE(yuval): Type Definitions
//

enum Memory_Block_Flags {
    MEMORY_BLOCK_FLAG_NOT_RESTORED = 0x1,
    MEMORY_BLOCK_FLAG_OVERFLOW_CHECK = 0x2,
    MEMORY_BLOCK_FLAG_UNDERFLOW_CHECK = 0x4
};

struct Memory_Block {
    yd_u64 flags;
    
    yd_u8* base;
    yd_umm size;
    yd_umm used;
    
    yd_umm total_allocated_size;
    
    Memory_Block* prev;
};

struct Memory_Arena {
    Memory_Block* current_block;
    yd_umm minimum_block_size;
    
    yd_u64 allocation_flags;
    yd_s32 temp_count;
};

struct Temporary_Memory {
    Memory_Arena* arena;
    Memory_Block* block;
    yd_umm used;
};

enum Arena_Push_Flags {
    ARENA_FLAG_CLEAR_TO_ZERO = 0x1
};

struct Arena_Push_Params {
    yd_u32 flags;
    yd_u32 alignment;
};

struct Arena_Bootstrap_Params {
    yd_u64 allocation_flags;
    yd_umm minimum_block_size;
};

//
// NOTE(yuval): Memory Block Allocation & Deallocation Function Types
//

#define ALLOCATE_MEMORY(name) Memory_Block* name(yd_umm size, yd_u64 flags)
typedef ALLOCATE_MEMORY(Allocate_Memory);

#define DEALLOCATE_MEMORY(Name) void Name(Memory_Block* block)
typedef DEALLOCATE_MEMORY(Deallocate_Memory);

extern Allocate_Memory* yd_allocate_memory;
extern Deallocate_Memory* yd_deallocate_memory;

//
// NOTE(yuval): Public API Function Declarations
//

void* copy(void* dest_init, const void* source_init, yd_umm size);

#if !defined(COPY_ARRAY)
# define COPY_ARRAY(dest, source, count) copy((dest), (source), (count) * sizeof(*(source)))
#endif // #if !defined(COPY_ARRAY)

void zero_size(void* ptr, yd_umm size);

#if !defined(ZERO_STRUCT)
# define ZERO_STRUCT(instance) zero_size(&(instance), sizeof(instance))
#endif // #if !defined(ZERO_STRUCT)

#if !defined(ZERO_ARRAY)
# define ZERO_ARRAY(pointer, count) zero_size(pointer, (count) * sizeof(*(pointer)))
#endif // #if !defined(ZERO_ARRAY)

// NOTE(yuval): This function declaration is an inline function and it's declared here only
// because PushSize_'s declaration uses it
yd_internal inline Arena_Push_Params default_arena_params();

void* push_size_(Memory_Arena* arena, yd_umm size_init,
                 Arena_Push_Params params = default_arena_params());

#if !defined(PUSH_SIZE)
# define PUSH_SIZE(arena, size, ...) push_size_(arena, size, ## __VA_ARGS__)
#endif // #if !defined(PUSH_SIZE)

#if !defined(PUSH_STRUCT)
# define PUSH_STRUCT(arena, type, ...) (type*)push_size_(arena, sizeof(type), ## __VA_ARGS__)
#endif // #if !defined(PUSH_STRUCT)

#if !defined(PUSH_ARRAY)
# define PUSH_ARRAY(arena, type, count, ...) (type*)push_size_(arena, (count) * sizeof(type), \
## __VA_ARGS__)
#endif // #if !defined(PUSH_ARRAY)

#if !defined(PUSH_COPY)
# define PUSH_COPY(arena, source, size, ...) copy(push_size_(arena, size, ## __VA_ARGS__), \
(source), size)
#endif // #if !defined(PUSH_COPY)

void clear(Memory_Arena* arena);

yd_b32 memories_match(void* a_init, void* b_init, yd_umm count);

//
// NOTE(yuval): Public API Inline Functions
//

//
// NOTE(yuval): Arena & Memory Parameters
//

yd_internal inline Arena_Push_Params
default_arena_params() {
    Arena_Push_Params params;
    params.flags = ARENA_FLAG_CLEAR_TO_ZERO;
    params.alignment = 4;
    
    return params;
}

yd_internal inline Arena_Push_Params
arena_align_clear(yd_u32 alignment) {
    Arena_Push_Params params = default_arena_params();
    params.flags |= ARENA_FLAG_CLEAR_TO_ZERO;
    params.alignment = alignment;
    
    return params;
}

yd_internal inline Arena_Push_Params
arena_align_no_clear(yd_u32 alignment) {
    Arena_Push_Params params = default_arena_params();
    params.flags &= ~ARENA_FLAG_CLEAR_TO_ZERO;
    params.alignment = alignment;
    
    return params;
}

yd_internal inline Arena_Push_Params
ArenaAlign(yd_u32 alignment, yd_b32 should_clear) {
    Arena_Push_Params params;
    
    if (should_clear) {
        params = arena_align_clear(alignment);
    } else {
        params = arena_align_no_clear(alignment);
    }
    
    return params;
}

yd_internal inline Arena_Push_Params
arena_no_clear() {
    Arena_Push_Params params = default_arena_params();
    params.flags &= ~ARENA_FLAG_CLEAR_TO_ZERO;
    
    return params;
}

yd_internal inline Arena_Bootstrap_Params
default_bootstrap_params() {
    Arena_Bootstrap_Params params = {};
    return params;
};

yd_internal inline Arena_Bootstrap_Params
no_restored_arena() {
    Arena_Bootstrap_Params params = default_bootstrap_params();
    params.allocation_flags = MEMORY_BLOCK_FLAG_NOT_RESTORED;
    
    return params;
}

//
// NOTE(yuval): Arena Alignment
//

yd_internal inline yd_umm
get_alignment_offset(Memory_Arena* arena, yd_umm alignment) {
    yd_umm alignment_offset = 0;
    yd_umm tail_pointer = (yd_umm)arena->current_block->base + arena->current_block->used;
    yd_umm alignment_mask = alignment - 1;
    
    if (tail_pointer & alignment_mask) {
        alignment_offset = alignment - (tail_pointer & alignment_mask);
    }
    
    return alignment_offset;
}

yd_internal inline yd_umm
get_effective_size_for(Memory_Arena* arena, yd_umm size_init, Arena_Push_Params params) {
    yd_umm alignment_offset = get_alignment_offset(arena, params.alignment);
    yd_umm size = size_init + alignment_offset;
    YD_ASSERT(size >= size_init);
    
    return size;
}

//
// NOTE(yuval): Temporary Arena Memory
//

yd_internal inline Temporary_Memory
begin_temporary_memory(Memory_Arena* arena) {
    Temporary_Memory result;
    
    result.arena = arena;
    result.block = arena->current_block;
    result.used = arena->current_block ? arena->current_block->used : 0;
    
    ++arena->temp_count;
    
    return result;
}

yd_internal inline void
yd_memory__free_last_block(Memory_Arena* arena) {
    Memory_Block* to_free = arena->current_block;
    arena->current_block = to_free->prev;
    
    YD_ASSERT(yd_deallocate_memory);
    yd_deallocate_memory(to_free);
}

yd_internal inline void
end_temporary_memory(Temporary_Memory temp_mem) {
    Memory_Arena* arena = temp_mem.arena;
    
    while (arena->current_block != temp_mem.block) {
        yd_memory__free_last_block(arena);
    }
    
    if (arena->current_block) {
        YD_ASSERT(arena->current_block->used >= temp_mem.used);
        arena->current_block->used = temp_mem.used;
    }
    
    YD_ASSERT(arena->temp_count > 0);
    --arena->temp_count;
}

yd_internal inline void
check_arena(Memory_Arena* arena) {
    YD_ASSERT(arena->temp_count == 0);
}

//
// NOTE(yuval): Bootstrap Push
//

yd_internal inline void*
bootstrap_push_size_(yd_umm size, yd_umm offset_to_arena,
                     Arena_Bootstrap_Params bootstrap_params = default_bootstrap_params(),
                     Arena_Push_Params push_params = default_arena_params()) {
    Memory_Arena bootstrap = {};
    bootstrap.allocation_flags = bootstrap_params.allocation_flags;
    bootstrap.minimum_block_size = bootstrap_params.minimum_block_size;
    
    void* result = PUSH_SIZE(&bootstrap, size, push_params);
    *(Memory_Arena*)((yd_u8*)result + offset_to_arena) = bootstrap;
    
    return result;
}

#if !defined(BOOTSTRAP_PUSH_STRUCT)
# define BOOTSTRAP_PUSH_STRUCT(type, member, ...) \
(type*)bootstrap_push_size_(sizeof(type), \
YD_OFFSET_OF(type, member), \
## __VA_ARGS__)
#endif // #if !defined(BOOTSTRAP_PUSH_STRUCT)

#define YD_MEMORY
#endif // #if !defined(YD_MEMORY)

#if defined(YD_MEMORY_IMPLEMENTATION)

//
// NOTE(yuval): Platform-Specific Includes
//

#if YD_WIN32
# include <windows.h>
#elif YD_MACOS || YD_LINUX
# include <sys/mman.h>
#endif // #if YD_WIN32

//
// NOTE(yuval): Exported Function Implementations
//

//
// NOTE(yuval): Memory Allocation & Deallocation
//

ALLOCATE_MEMORY(yd_allocate_memory_) {
#if 0
    // NOTE(yuval): We require the memory block headers not to change the cache
    // line alignment of an allocation
    YD_ASSERT(sizeof(Memory_Block) == 64);
#endif // #if 0
    
    yd_umm page_size = 4096;
    yd_umm total_size = size + sizeof(Memory_Block);
    yd_umm base_offset = sizeof(Memory_Block);
    yd_umm protect_offset = 0;
    
    if (flags & MEMORY_BLOCK_FLAG_UNDERFLOW_CHECK) {
        total_size = size + 2 * page_size;
        base_offset = 2 * page_size;
        protect_offset = page_size;
    } else if (flags & MEMORY_BLOCK_FLAG_OVERFLOW_CHECK) {
        yd_umm size_rounded_up = YD_ALIGN_POW2(size, page_size);
        total_size = size_rounded_up + 2 * page_size;
        base_offset = page_size + size_rounded_up - size;
        protect_offset = page_size + size_rounded_up;
    }
    
    Memory_Block* block = 0;
    
#if YD_WIN32
    block = (Memory_Block*)VirtualAlloc(0, total_size,
                                        MEM_RESERVE | MEM_COMMIT,
                                        PAGE_READWRITE);
#elif YD_MACOS || YD_LINUX
    block = (Memory_Block*)mmap(0, total_size,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS,
                                -1, 0);
    YD_ASSERT(block != MAP_FAILED);
#endif // #if YD_WIN32
    
    YD_ASSERT(block);
    block->base = (yd_u8*)block + base_offset;
    YD_ASSERT(block->used == 0);
    YD_ASSERT(block->prev == 0);
    
    if (flags & (MEMORY_BLOCK_FLAG_OVERFLOW_CHECK | MEMORY_BLOCK_FLAG_UNDERFLOW_CHECK)) {
#if YD_WIN32
        DWORD old_protect = 0;
        BOOL is_protected = VirtualProtect((yd_u8*)block + protect_offset, page_size,
                                           PAGE_NOACCESS, &old_protect);
        YD_ASSERT(is_protected);
#elif YD_MACOS || YD_LINUX
        yd_s32 protect_result = mprotect((yd_u8*)block + protect_offset, page_size, PROT_NONE);
        YD_ASSERT(protect_result == 0);
#endif // #if YD_WIN32
    }
    
    block->flags = flags;
    block->size = size;
    block->total_allocated_size = total_size;
    
    return block;
}

DEALLOCATE_MEMORY(yd_deallocate_memory_) {
#if YD_WIN32
    BOOL result = VirtualFree(block, 0, MEM_RELEASE);
    YD_ASSERT(result);
#elif YD_MACOS || YD_LINUX
    yd_s32 result = munmap(block, block->total_allocated_size);
    YD_ASSERT(result == 0);
#endif // #if YD_WIN32
}

Allocate_Memory* yd_allocate_memory = yd_allocate_memory_;
Deallocate_Memory* yd_deallocate_memory = yd_deallocate_memory_;

//
// NOTE(yuval): Memory Utility Functions
//

void*
copy(void* dest_init, const void* source_init, yd_umm size) {
    void* result = 0;
    
    if (dest_init && source_init) {
        const yd_u8* source = (const yd_u8*)source_init;
        yd_u8* dest = (yd_u8*)dest_init;
        
        while (size--) {
            *dest++ = *source++;
        }
        
        result = dest_init;
    }
    
    return result;
}

void
zero_size(void* ptr, yd_umm size) {
    yd_u8* byte = (yd_u8*)ptr;
    
    while (size--) {
        *byte++ = 0;
    }
}

//
// NOTE(yuval): Arena Push
//

yd_internal inline yd_b32
yd_memory__is_pow2(yd_u32 value) {
    yd_b32 result = ((value & ~(value - 1)) == value);
    return result;
}

void*
push_size_(Memory_Arena* arena, yd_umm size_init, Arena_Push_Params params) {
    YD_ASSERT(params.alignment <= 128);
    YD_ASSERT(yd_memory__is_pow2(params.alignment));
    
    yd_umm size = 0;
    if (arena->current_block) {
        size = get_effective_size_for(arena, size_init, params);
    }
    
    if (!arena->current_block ||
        ((arena->current_block->used + size) > arena->current_block->size)) {
        size = size_init; // NOTE(yuval): The base will automatically be aligned now!
        
        if (arena->allocation_flags & (MEMORY_BLOCK_FLAG_OVERFLOW_CHECK |
                                       MEMORY_BLOCK_FLAG_UNDERFLOW_CHECK)) {
            arena->minimum_block_size = 0;
            size = YD_ALIGN_POW2(size, params.alignment);
        }
        else if (arena->minimum_block_size == 0) {
            // TODO(yuval): Tune default block size
            arena->minimum_block_size = 1024 * 1024;
        }
        
        yd_umm block_size = YD_MAXIMUM(size, arena->minimum_block_size);
        
        YD_ASSERT(yd_allocate_memory);
        Memory_Block* new_block = yd_allocate_memory(block_size, arena->allocation_flags);
        new_block->prev = arena->current_block;
        arena->current_block = new_block;
    }
    
    YD_ASSERT((arena->current_block->used + size) <= arena->current_block->size);
    
    yd_umm alignment_offset = get_alignment_offset(arena, params.alignment);
    void* result = arena->current_block->base + arena->current_block->used + alignment_offset;
    arena->current_block->used += size;
    
    // NOTE(yuval): This is just to guarantee that nobody passed in an alignment
    // on their first allocation that was _greater_ than the page alignment
    YD_ASSERT(arena->current_block->used <= arena->current_block->size);
    
    if (params.flags & ARENA_FLAG_CLEAR_TO_ZERO)
    {
        zero_size(result, size_init);
    }
    
    return result;
}

//
// NOTE(yuval): Arena Clearing
//

void
clear(Memory_Arena* arena) {
    for (;;) {
        yd_b32 is_last_block = (arena->current_block->prev == 0);
        yd_memory__free_last_block(arena);
        
        if (is_last_block) {
            break;
        }
    }
}

//
// NOTE(yuval): Memory Helper Functions
//

yd_b32
memories_match(void* a_init, void* b_init, yd_umm count) {
    yd_b32 result = true;
    
    yd_u8* a = (yd_u8*)a_init;
    yd_u8* b = (yd_u8*)b_init;
    
    while (count--) {
        if (*a++ != *b++) {
            result = false;
            break;
        }
    }
    
    return result;
}

#endif // #if defined(YD_MEMORY_IMPLEMENTATION)
