#include "slide.cpp"

#include <windows.h>

#include "win32_slide_renderer.h"

#define YD_INPUT_IMPLEMENTATION
#include "yd/yd_input.h"

global b32 global_running;

internal PLATFORM_DISPLAY_MESSAGE_BOX(win32_display_message_box) {
    MessageBoxA(0, message, title, MB_OK);
}

internal PLATFORM_FREE_FILE_MEMORY(win32_free_file_memory) {
    if (memory) {
        VirtualFree(memory, 0, MEM_RELEASE);
    }
}

internal PLATFORM_READ_ENTIRE_FILE(win32_read_entire_file) {
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

internal PLATFORM_WRITE_ENTIRE_FILE(win32_write_entire_file) {
    b32 result = false;
    
    HANDLE file_handle = CreateFileA(filename, GENERIC_WRITE,
                                     0, 0, CREATE_ALWAYS,
                                     0, 0);
    
    if (file_handle != INVALID_HANDLE_VALUE) {
        DWORD bytes_written;
        if (WriteFile(file_handle, memory, memory_size,
                      &bytes_written, 0)) {
            result = (bytes_written == memory_size);
        }
        
        CloseHandle(file_handle);
    }
    
    return result;
}

internal Vector2u
win32_get_window_dimensions(HWND window) {
    RECT client_rect;
    GetClientRect(window, &client_rect);
    
    Vector2u result = {
        (u32)(client_rect.right - client_rect.left),
        (u32)(client_rect.bottom - client_rect.top)
    };
    
    return result;
}

s32 WINAPI
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR command_line,
        s32 show_code) {
    WNDCLASSA window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = yd_input_window_proc;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));
    window_class.lpszClassName = "SlideWindowClass";
    
    if (RegisterClassA(&window_class)) {
        HWND window = CreateWindowExA(0,
                                      window_class.lpszClassName,
                                      "Slide",
                                      WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      CW_USEDEFAULT,
                                      0,
                                      0,
                                      instance,
                                      0);
        
        if (window) {
            Application app = {};
            
            // NOTE(yuval): Platform API Functions
            app.platform_api.display_message_box =
                win32_display_message_box;
            app.platform_api.free_file_memory = win32_free_file_memory;
            app.platform_api.read_entire_file = win32_read_entire_file;
            app.platform_api.write_entire_file = win32_write_entire_file;
            
            Platform_Renderer_Limits limits;
            limits.max_quad_count_per_frame = (1 << 18);
            limits.max_texture_handle_count = 256;
            
            Platform_Renderer* renderer =
                win32_init_default_renderer(window, &limits);
            
            global_running = true;
            while (global_running) {
                /* Vector2u render_dim = {
                    1280, 720,
                    
                    // 1920, 1080
                };*/
                
                // TODO(yuval): Get rid of all of this!
                Vector2u dimensions = win32_get_window_dimensions(window);
                
                Render_Commands* frame =
                    renderer->begin_frame(renderer, dimensions);
                
                update_window_events();
                
                // NOTE(yuval): Platform Input Event Processing
                for (umm event_index = 0;
                     event_index < global_input.platform_frame_event_count;
                     ++event_index) {
                    Event* event = &global_input.platform_frame_events[event_index];
                    
                    switch (event->kind) {
                        case Event_Kind::QUIT: {
                            global_running = false;
                        }
                    }
                }
                
                update_and_render(&app, frame, &global_input);
                
                renderer->end_frame(renderer, frame);
            }
        } else {
            // TODO(yuval): Diagnostics
        }
    }
}