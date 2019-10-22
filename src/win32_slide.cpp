#include "slide.cpp"

#include <windows.h>

#include "win32_slide_renderer.h"

global b32 global_running;

internal PLATFORM_DISPLAY_MESSAGE_BOX(win32_display_message_box) {
    MessageBoxA(0, message, title, MB_OK);
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

internal LRESULT CALLBACK
win32_main_window_callback(HWND window, UINT message,
                           WPARAM w_param, LPARAM l_param) {
    LRESULT result = 0;
    
    switch (message) {
        case WM_CLOSE: {
            global_running = false;
        } break;
        /*
        case WM_WINDOWPOSCHANGING: {
        
        } break;
        
        case WM_WINDOWPOSCHANGED: {
        
        } break;
        
        case WM_SETCURSOR: {
        
        } break;
        
        case WM_ACTIVEAPP: {
        
        } break;
        */
        
        case WM_DESTROY: {
            global_running = false;
        } break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: {
            ASSERT(!"Keyboard input came in through a non-dispatch message!");
        } break;
        
        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC device_context = BeginPaint(window, &paint);
            EndPaint(window, &paint);
        } break;
        
        default: {
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
    }
    
    return result;
}

s32 WINAPI
WinMain(HINSTANCE instance,
        HINSTANCE prev_instance,
        LPSTR command_line,
        s32 show_code) {
    WNDCLASSA window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = win32_main_window_callback;
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
            App app;
            app.platform_api.display_message_box =
                win32_display_message_box;
            
            HDC renderer_dc = GetDC(window);
            
            Renderer_Limits limits;
            limits.max_filled_rects = 16384;
            
            Renderer* renderer =
                win32_init_default_renderer(window, &limits);
            
            global_running = true;
            while (global_running) {
                Vector2u render_dim = {
                    1280, 720,
                    
                    // 1920, 1080
                };
                
                MSG message;
                while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
                    switch (message.message) {
                        case WM_QUIT: {
                            global_running = false;
                        } break;
                        
                        default: {
                            TranslateMessage(&message);
                            DispatchMessageA(&message);
                        } break;
                    }
                }
                
                // NOTE(yuval): Update window dimensions
                app.window_dim = win32_get_window_dimensions(window);
                
                update_and_render(&app, renderer);
            }
        } else {
            // TODO(yuval): Diagnostics
        }
    }
}