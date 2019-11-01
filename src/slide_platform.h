#if !defined(SLIDE_PLATFORM_H)

#include "slide_types.h"

#define PLATFORM_DISPLAY_MESSAGE_BOX(name) void name(const char* title, const char* message)
typedef PLATFORM_DISPLAY_MESSAGE_BOX(Platform_Display_Message_Box);

struct Read_File_Result {
    u32 contents_size;
    void* contents;
};

#define PLATFORM_FREE_FILE_MEMORY(name) void name(void* memory)
typedef PLATFORM_FREE_FILE_MEMORY(Platform_Free_File_Memory);

#define PLATFORM_READ_ENTIRE_FILE(name) Read_File_Result name(const char* filename)
typedef PLATFORM_READ_ENTIRE_FILE(Platform_Read_Entrie_File);

#define PLATFORM_WRITE_ENTIRE_FILE(name) b32 name(const char* filename, \
void* memory, u32 memory_size)
typedef PLATFORM_WRITE_ENTIRE_FILE(Platform_Write_Entire_File);

struct Platform_Api {
    Platform_Display_Message_Box* display_message_box;
    
    Platform_Free_File_Memory* free_file_memory;
    Platform_Read_Entrie_File* read_entire_file;
    Platform_Write_Entire_File* write_entire_file;
};

extern Platform_Api platform;

struct Application {
    Platform_Api platform_api;
    
    struct App_State* state;
};

#define SLIDE_PLATFORM_H
#endif // #if !defined(SLIDE_PLATFORM_H)