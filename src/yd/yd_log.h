#if !defined(YD_LOG)

// NOTE(yuval): Logs In The Log List Are Ordered From The NEWEST Log To The OLDEST!

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
// NOTE(yuval): Type Definitions
//

struct Log {
    char message[1024];
    
    Log* next;
};

//
// NOTE(yuval): Exported Variable Declarations
//

extern Log* global_log_list;

#define YD_LOG
#endif // #if !defined(YD_LOG)


//
// NOTE(yuval): YD Log Implementation
//

#if defined(YD_LOG_IMPLEMENTATION)


//
// NOTE(yuval): Exported Variable Instantiations
//

Log* global_log_list = 0;


//
// NOTE(yuval): Exported Function Implementations
//
void
log(const char* agent, const char* message, ...) {
    char message_format_buffer[1024];
    va_list arg_list;
    
    va_start(arg_list, message);
    yd_umm ignored_size = format_string_list(message_format_buffer,
                                             sizeof(message_format_buffer),
                                             message, arg_list);
    va_end(arg_list);
    
    Log* new_log = PUSH(Log);
    format_string(new_log->message, sizeof(new_log->message),
                  "[%s] %s", agent, message_format_buffer);
    new_log->next = global_log_list;
    
    global_log_list = new_log;
}

#endif // #if defined(YD_LOG_IMPLEMENTATION)
