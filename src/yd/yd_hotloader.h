#if !defined(YD_HOTLOADER)

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

// NOTE(yuval): To be used only in the same function
// where the array was defined or with arrays that are defined
// within structs
#if !defined(YD_ARRAY_COUNT)
# define YD_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#endif // #if !defined(YD_ARRAY_COUNT)

//
// NOTE(yuval): Platform-Specific Includes
//

#if YD_WIN32
# include <windows.h>
//#elif YD_MACOS || YD_LINUX
//# include <sys/mman.h>
#endif // #if YD_WIN32

//
// NOTE(yuval): Type Definitions
//

#if YD_WIN32
struct Directory_Info {
    const char* name;
    HANDLE handle;
    
    FILE_NOTIFY_INFORMATION* notify_info;
    OVERLAPPED overlapped;
    yd_s32 bytes_returned;
    
    yd_b32 read_issue_failed;
    
    Directory_Info* next;
};
#endif // #if YD_WIN32

struct Asset_Change {
    String short_name;
    String full_name;
    String extension;
    
    yd_f32 last_change_time;
    
    Asset_Change* next;
};

struct Hotloader {
    yd_b32 is_initialized;
    
    // TODO(yuval): Use a Bucket_Array for Directory_Infos and Asset_Changes
    Directory_Info* first_dir;
    
    // Bucket_Array<Asset_Change> asset_changes;
    Asset_Change* first_asset_change;
};

#define YD_HOTLOADER
#endif // #if !defined(YD_HOTLOADER)

//
// NOTE(yuval): YD Input Implementation
//

#if defined(YD_HOTLOADER_IMPLEMENTATION)

//
// NOTE(yuval): Internal Macros
//

#define YD_HOTLOADER__NOTIFY_LEN 8000

//
// NOTE(yuval): Internal Functions
//

#if YD_WIN32
yd_internal void
win32_issue_one_read(Directory_Info* info) {
    DWORD bytes_returned;
    yd_b32 result = ReadDirectoryChangesW(
        info->handle, info->notify_info,
        YD_HOTLOADER__NOTIFY_LEN,
        TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE,
        &bytes_returned, &info->overlapped, 0);
    
    info->read_issue_failed = result;
}
#endif // #if YD_WIN32

//
// NOTE(yuval): Exported Function Implementations
//

#if YD_WIN32
void
hotloader_init(Hotloader* hotloader) {
    ASSERT(!hotloader->is_initialized);
    
    hotloader->first_dir = 0;
    hotloader->first_asset_change = 0;
    
    const char* dir_names[] = {
        "data"
    };
    
    for (yd_umm dir_index = 0;
         dir_index < YD_ARRAY_COUNT(dir_names);
         ++dir_index) {
        const char* dir_name = dir_names[dir_index];
        
        Directory_Info* info = PUSH(Directory_Info);
        
        // NOTE(yuval): Notification Buffer Allocation
        info->notify_info = (FILE_NOTIFY_INFORMATION*)PUSH_SIZE(YD_HOTLOADER__NOTIFY_LEN);
        
        // TODO(yuval): Check that the current working directory can be retrieved
        
        // NOTE(yuval): Overlapped Initialization
        HANDLE event = CreateEventA(0, FALSE, FALSE, 0);
        if (event != INVALID_HANDLE_VALUE) {
            info->overlapped.hEvent = event;
            info->overlapped.Offset = 0;
            
            info->read_issue_failed = false;
            
            // NOTE(yuval): Directory File Handle Creation
            HANDLE handle = CreateFileA(
                dir_name, FILE_LIST_DIRECTORY,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                0, OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                0);
            
            if (handle != INVALID_HANDLE_VALUE) {
                // NOTE(yuval): Setting the rest of the directory information
                // and appending it to the directory list
                info->name = dir_name;
                info->handle = handle;
                
                info->next = hotloader->first_dir;
                hotloader->first_dir = info;
                
                win32_issue_one_read(info);
            } else {
                // TODO(yuval): Diagnostic
            }
            
        } else {
            // TODO(yuval): Diagnostic
        }
    }
    
    hotloader->is_initialized = true;
}
#endif // #if YD_WIN32

#endif // #if defined(YD_HOTLOADER_IMPLEMENTATION)