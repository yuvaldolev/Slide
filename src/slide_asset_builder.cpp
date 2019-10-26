#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#include <windows.h>
#include <stdio.h>

#define ASSERT(expression) if(!(expression)) {*(volatile int *)0 = 0;}

#include "slide_types.h"
#include "slide_intrinsics.h"

struct Read_File_Result {
    u32 contents_size;
    void* contents;
};

void
win32_free_file_memory(void* memory) {
    if (memory) {
        VirtualFree(memory, 0, MEM_RELEASE);
    }
}

Read_File_Result
win32_read_entire_file(const char* filename) {
    Read_File_Result result = {};
    
    HANDLE file_handle = CreateFileA(filename, GENERIC_READ,
                                     FILE_SHARE_READ, 0, OPEN_EXISTING,
                                     0, 0);
    
    if (file_handle != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER file_size;
        if (GetFileSizeEx(file_handle, &file_size)) {
            u32 file_size_32 = safe_truncate_to_u32(file_size.QuadPart);
            result.contents = VirtualAlloc(0, file_size_32,
                                           MEM_RESERVE | MEM_COMMIT,
                                           PAGE_READWRITE);
            
            if (result.contents) {
                DWORD bytes_read;
                if (ReadFile(file_handle, result.contents,
                             file_size_32, &bytes_read, 0) &&
                    (bytes_read == file_size_32)) {
                    result.contents_size = file_size_32;
                } else {
                    win32_free_file_memory(result.contents);
                    result.contents = 0;
                }
            }
        }
        
        CloseHandle(file_handle);
    }
    
    return result;
}

internal void
build_character_bitmap(FILE* asset_file, stbtt_fontinfo font, u32 character) {
    int width, height, x_offset, y_offset;
    u8* mono_bitmap =
        stbtt_GetCodepointBitmap(&font, 0,
                                 stbtt_ScaleForMappingEmToPixels(&font, 512.0f),
                                 character, &width, &height, &x_offset, &y_offset);
    
    // TODO(yuval): Temporary! Maybe use VirtualAlloc or HeapAlloc
    u8* memory = (u8*)malloc(width * height * 4);
    int pitch = width * 4;
    
    u8* source = mono_bitmap;
    u8* dest_row = memory + (height - 1) * pitch;
    for (s32 y = 0; y < height; ++y) {
        u32* dest = (u32*)dest_row;
        
        for (s32 x = 0; x < width; ++x) {
            u8 alpha = *source++;
            *dest++ = ((alpha << 24) |
                       (alpha << 16) |
                       (alpha << 8) |
                       (alpha));
        }
        
        dest_row -= pitch;
    }
    
    fprintf(asset_file, "%d %d ", width, height);
    fwrite(memory, width * height * 4, 1, asset_file);
}

internal void
build_font(const char* path) {
    Read_File_Result ttf_file = win32_read_entire_file(path);
    
    stbtt_fontinfo font;
    stbtt_InitFont(&font, (u8*)ttf_file.contents,
                   stbtt_GetFontOffsetForIndex((u8*)ttf_file.contents, 0));
    
    String font_name = make_string_slowly(path);
    font_name = front_of_directory(font_name);
    remove_extension(&font_name);
    printf("Parsing font: %.*s", PRINTABLE_STRING(font_name));
    
    String asset_file_directory =
        BUNDLE_LITERAL("data/generated_assets/fonts/");
    
    char asset_file_path[1024];
    String asset_file_path_str = make_string(asset_file_path, 0,
                                             sizeof(asset_file_path));
    append(&asset_file_path_str, asset_file_directory);
    append(&asset_file_path_str, font_name);
    append(&asset_file_path_str, ".sf");
    terminate_with_null(&asset_file_path_str);
    
    FILE* asset_file;
    fopen_s(&asset_file, asset_file_path, "wb");
    
    for (u32 character = 0; character < 255; ++character) {
        build_character_bitmap(asset_file, font, character);
    }
    
    fclose(asset_file);
    
    printf("...\n");
    printf("Done!\n");
}

int
main(int arg_count, char** args) {
    if (arg_count == 3) {
        if (strings_match(EXPAND_LITERAL("-font"), args[1])) {
            build_font(args[2]);
        } else {
            printf("The First Arg MUST Be One Of The Following: {-font}\n");
        }
    } else {
        printf("Usage: slide_asset_builder -font [path_to_font]\n");
    }
}