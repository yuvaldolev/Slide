#if !defined(YD_INPUT)

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

//
// NOTE(yuval): Type Definitions
//
namespace Event_Kind {
    enum Type {
        NONE,
        
        BUTTON,
        TEXT_INPUT,
        MOUSE_WHEEL,
        WINDOW,
        QUIT
    };
}

#if 0
namespace Key_State {
    enum Type {
        NONE,
        
        DOWN,
        START,
        END
    };
}
#endif // #if 0

namespace Button_Code {
    enum Type {
        UNKNOWN = 0,
        
        // NOTE(yuval): Non-textual keys that have place in the ASCII table
        BACKSPACE = 8,
        TAB = 9,
        LINEFEED = 10,
        ENTER = 13,
        ESCAPE = 27,
        SPACEBAR = 32,
        
        // NOTE(yuval): The letters A-Z live in here as well and may be returned
        // by keyboard events
        
        DEL = 127,
        
        ARROW_UP = 128,
        ARROW_DOWN = 129,
        ARROW_LEFT = 130,
        ARROW_RIGHT = 131,
        
        ALT,
        CTRL,
        SHIFT,
        
        MOUSE_WHEEL_UP,
        MOUSE_WHEEL_DOWN,
        
        MOUSE_BUTTON_LEFT,
        MOSUE_BUTTON_MIDDLE,
        MOUSE_BUTTON_RIGHT,
        
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        
        COUNT
    };
}

union Modifier_Flags {
    yd_b32 packed;
    
    struct {
        yd_b8 shift_pressed;
        yd_b8 ctrl_pressed;
        yd_b8 alt_pressed;
        yd_b8 pad;
    };
};

struct Button_Event {
    Button_Code::Type code;
    yd_b32 pressed; // NOTE(yuval): If not pressed, it's a key release
    yd_b32 repeated;
    Modifier_Flags modifier_flags;
};

struct Text_Input_Event {
    yd_u32 utf32;
};

struct Mouse_Wheel_Event {
    yd_s32 typical_delta;
    yd_s32 delta;
};

struct Event {
    Event_Kind::Type kind;
    
    union {
        Button_Event button;
        Text_Input_Event text_input;
        Mouse_Wheel_Event mouse_wheel;
    };
};

struct Input {
    //
    // NOTE(yuval): Per-frame Input Data
    //
    
    // TODO(yuval): Replace the event arrays with dynamic arrays of some sort
    // (preferably a bucket array)
    
    // NOTE(yuval): App Frame Events Include:
    // BUTTON, TEXT_INPUT, MOUSE_WHEEL
    Event app_frame_events[1024];
    yd_umm app_frame_event_count;
    
    // NOTE(yuval): Platform Frame Events Include:
    // WINDOW, QUIT
    Event platform_frame_events[1024];
    yd_umm platform_frame_event_count;
    
    // TODO(yuval): Compress this to a Vector3
    yd_s32 mouse_delta_x;
    yd_s32 mouse_delta_y;
    yd_s32 mouse_delta_z;
    
    //
    // NOTE(yuval): Persistant Input Data
    //
    // NOTE(yuval): True if pressed, false if not
    yd_b32 button_states[Button_Code::COUNT];
    
    yd_b8 shift_state;
    yd_b8 ctrl_state;
    yd_b8 alt_state;
    
    yd_b32 application_has_focus;
};

extern Input global_input;

#define YD_INPUT
#endif // #if !defined(YD_INPUT)

//
// NOTE(yuval): YD Input Implementation
//

#if defined(YD_INPUT_IMPLEMENTATION)

//
// NOTE(yuval): Platform-Specific Includes
//

#if YD_WIN32
# include <windows.h>
//#elif YD_MACOS || YD_LINUX
//# include <sys/mman.h>
#endif // #if YD_WIN32

//
// NOTE(yuval): Exported Variable Instantiations
//

Input global_input = {};

//
// NOTE(yuval): Internal Functions
//

yd_internal inline void
add_app_input_event(Event event) {
    global_input.app_frame_events
        [global_input.app_frame_event_count++] = event;
}

yd_internal inline void
add_platform_input_event(Event event) {
    global_input.platform_frame_events
        [global_input.platform_frame_event_count++] = event;
}

#if YD_WIN32
yd_internal Button_Code::Type
win32_get_button_code(WPARAM w_param) {
    using namespace Button_Code;
    
    Button_Code::Type result;
    
    if ((w_param >= 48) && (w_param <= 90)) {
        result = (Button_Code::Type)w_param;
    } else if ((w_param >= VK_F1) && (w_param <= VK_F16)) {
        WPARAM delta = (w_param - VK_F1);
        result = (Button_Code::Type)(F1 + delta);
    } else {
        switch (w_param) {
            case VK_BACK: { result = BACKSPACE; } break;
            case VK_TAB: { result = TAB; } break;
            case VK_RETURN: { result = ENTER; } break;
            case VK_ESCAPE: { result =  ESCAPE; } break;
            case VK_SPACE: { result = SPACEBAR; } break;
            
            case VK_OEM_1: { result = (Button_Code::Type)';'; };
            case VK_OEM_2: { result = (Button_Code::Type)'/'; };
            case VK_OEM_3: { result = (Button_Code::Type)'`'; };
            case VK_OEM_4: { result = (Button_Code::Type)'['; };
            case VK_OEM_5: { result = (Button_Code::Type)'\\'; };
            case VK_OEM_6: { result = (Button_Code::Type)']'; };
            case VK_OEM_7: { result = (Button_Code::Type)'\''; };
            
            case VK_OEM_PLUS: { result = (Button_Code::Type)'+'; };
            case VK_OEM_MINUS: { result = (Button_Code::Type)'-'; };
            case VK_OEM_PERIOD: { result = (Button_Code::Type)'.'; };
            case VK_OEM_COMMA: { result = (Button_Code::Type)','; };
            
            case VK_DELETE: { result = DEL; } break;
            
            case VK_LEFT: { result = ARROW_LEFT; } break;
            case VK_RIGHT: { result = ARROW_RIGHT; } break;
            case VK_UP: { result = ARROW_UP; } break;
            case VK_DOWN: { result = ARROW_DOWN; } break;
            
            case VK_MENU: { result = ALT; } break;
            case VK_CONTROL: { result = CTRL; } break;
            case VK_SHIFT: { result = SHIFT; } break;
            
            default: { result = UNKNOWN; };
        }
    }
    
    return result;
}

yd_internal yd_u64
win32_get_vk(Button_Code::Type button) {
    using namespace Button_Code;
    
    yd_u64 result;
    
    if ((button >= (Button_Code::Type)48) &&
        (button <= (Button_Code::Type)90)) {
        result = (yd_u64)button;
    } else if ((button >= F1) && (button <= F16)) {
        yd_u64 delta = (yd_u64)(button - F1);
        result = (VK_F1 + delta);
    } else if (button == ((Button_Code::Type)';')) {
        result = VK_OEM_1;
    } else if (button == ((Button_Code::Type)'/')) {
        result = VK_OEM_2;
    } else if (button == ((Button_Code::Type)'`')) {
        result = VK_OEM_3;
    } else if (button == ((Button_Code::Type)'[')) {
        result = VK_OEM_4;
    } else if (button == ((Button_Code::Type)'\\')) {
        result = VK_OEM_5;
    } else if (button == ((Button_Code::Type)']')) {
        result = VK_OEM_6;
    } else if (button == ((Button_Code::Type)'\'')) {
        result = VK_OEM_7;
    } else if (button == ((Button_Code::Type)'+')) {
        result = VK_OEM_PLUS;
    } else if (button == ((Button_Code::Type)'-')) {
        result = VK_OEM_MINUS;
    } else if (button == ((Button_Code::Type)'.')) {
        result = VK_OEM_PERIOD;
    } else if (button == ((Button_Code::Type)',')) {
        result = VK_OEM_COMMA;
    } else {
        switch (button) {
            case BACKSPACE: { result = VK_BACK; } break;
            case TAB: { result = VK_TAB; } break;
            case ENTER: { result = VK_RETURN; } break;
            case ESCAPE: { result = VK_ESCAPE; } break;
            case SPACEBAR: { result = VK_SPACE; } break;
            
            case DEL: { result = VK_DELETE; } break;
            
            case ARROW_LEFT: { result = VK_LEFT; } break;
            case ARROW_RIGHT: { result = VK_RIGHT; } break;
            case ARROW_UP: { result = VK_UP; } break;
            case ARROW_DOWN: { result = VK_DOWN; } break;
            
            case ALT: { result = VK_MENU; } break;
            case CTRL: { result = VK_CONTROL; } break;
            case SHIFT: { result = VK_SHIFT; } break;
            
            default: { result = 0; } break;
        }
    }
    
    return result;
}

// TODO(yuval): Can't I create a cross-platform version of this function?
// (using the event's button code)
yd_internal void
win32_update_modifier_key_states(WPARAM w_param, yd_b8 is_pressed) {
    switch (w_param) {
        case VK_SHIFT: {
            global_input.shift_state = is_pressed;
        } break;
        
        case VK_CONTROL: {
            global_input.ctrl_state = is_pressed;
        } break;
        
        case VK_MENU: {
            global_input.alt_state = is_pressed;
        } break;
    }
}
#endif // #if YD_WIN32

//
// NOTE(yuval): Exported Function Implementations
//

#if YD_WIN32
void
update_window_events() {
    global_input.app_frame_event_count = 0;
    global_input.platform_frame_event_count = 0;
    global_input.mouse_delta_x = 0;
    global_input.mouse_delta_y = 0;
    global_input.mouse_delta_z = 0;
    
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

LRESULT CALLBACK
yd_input_window_proc(HWND window, UINT message,
                     WPARAM w_param, LPARAM l_param) {
    LRESULT result = 0;
    
    switch (message) {
        case WM_ACTIVATEAPP: {
            for (yd_umm button_index = 0;
                 button_index < Button_Code::COUNT;
                 ++button_index) {
                Button_Code::Type button_code = (Button_Code::Type)button_index;
                yd_u64 vk = win32_get_vk(button_code);
                
                if (vk != 0) {
                    SHORT state = GetKeyState((yd_s32)vk);
                    if (!(state & 0x8000)) {
                        Event event = {};
                        event.kind = Event_Kind::BUTTON;
                        event.button.code = button_code;
                        event.button.pressed = false;
                        event.button.modifier_flags.shift_pressed =
                            global_input.shift_state;
                        event.button.modifier_flags.ctrl_pressed =
                            global_input.ctrl_state;
                        event.button.modifier_flags.alt_pressed =
                            global_input.alt_state;
                        
                        add_app_input_event(event);
                        
                        global_input.button_states[button_index] = false;
                        
                        win32_update_modifier_key_states(vk, false);
                    }
                }
            }
            
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN: {
            Event event = {};
            event.kind = Event_Kind::BUTTON;
            event.button.code = win32_get_button_code(w_param);
            event.button.pressed = true;
            event.button.repeated = (message & (1 << 30));
            event.button.modifier_flags.shift_pressed =
                global_input.shift_state;
            event.button.modifier_flags.ctrl_pressed =
                global_input.ctrl_state;
            event.button.modifier_flags.alt_pressed =
                global_input.alt_state;
            
            add_app_input_event(event);
            
            global_input.button_states[event.button.code] = true;
            
            win32_update_modifier_key_states(w_param, true);
        } break;
        
        case WM_SYSKEYUP:
        case WM_KEYUP: {
            Event event = {};
            event.kind = Event_Kind::BUTTON;
            event.button.code = win32_get_button_code(w_param);
            event.button.pressed = false;
            event.button.modifier_flags.shift_pressed =
                global_input.shift_state;
            event.button.modifier_flags.ctrl_pressed =
                global_input.ctrl_state;
            event.button.modifier_flags.alt_pressed =
                global_input.alt_state;
            
            add_app_input_event(event);
            
            global_input.button_states[event.button.code] = false;
            
            win32_update_modifier_key_states(w_param, false);
        } break;
        
        case WM_CHAR: {
            yd_u32 keycode = (yd_u32)w_param;
            
            // NOTE(yuval): Control characters generate keycodes < 32,
            // but these are redundant with KEYDOWN events and also ambiguous
            // (ctrl-m generates keyboard 13, but so does RETURN).
            if (keycode > 31) {
                Event event;
                event.kind = Event_Kind::TEXT_INPUT;
                event.text_input.utf32 = keycode;
                
                add_app_input_event(event);
            }
        } break;
        
        case WM_SETFOCUS: {
            global_input.application_has_focus = true;
        } break;
        
        case WM_KILLFOCUS: {
            global_input.application_has_focus = false;
        } break;
        
        case WM_PAINT: {
            ValidateRect(window, 0);
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: {
            Event event = {};
            event.kind = Event_Kind::BUTTON;
            event.button.code = Button_Code::MOUSE_BUTTON_LEFT;
            
            yd_b32 pressed;
            if (message == WM_LBUTTONDOWN) {
                pressed = true;
                
                SetCapture(window);
            } else {
                pressed = false;
                
                ReleaseCapture();
            }
            event.button.pressed = pressed;
            
            add_app_input_event(event);
            
            global_input.button_states[event.button.code] = pressed;
        } break;
        
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            Event event;
            event.kind = Event_Kind::BUTTON;
            event.button.code = Button_Code::MOUSE_BUTTON_RIGHT;
            
            yd_b32 pressed = false;
            if (message == WM_RBUTTONDOWN) {
                pressed = true;
            }
            event.button.pressed = pressed;
            
            add_app_input_event(event);
            
            global_input.button_states[event.button.code] = pressed;
        } break;
        
        case WM_MOUSEMOVE: {
            // TODO(yuval): Implement a mouse move event
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        case WM_MOUSEWHEEL: {
            Event event = {};
            event.kind = Event_Kind::MOUSE_WHEEL;
            event.mouse_wheel.typical_delta = WHEEL_DELTA;
            event.mouse_wheel.delta = (yd_s32)(w_param >> 16);
            
            add_app_input_event(event);
            
            global_input.mouse_delta_z += event.mouse_wheel.delta;
        } break;
        
        case WM_CLOSE:
        case WM_QUIT: {
            Event event = {};
            event.kind = Event_Kind::QUIT;
            
            add_platform_input_event(event);
            
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        case WM_SIZE: {
            // TODO(yuval): Handle WM_SIZE
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        case WM_EXITSIZEMOVE: {
            // TODO(yuval): Handle WM_EXITSIZEMOVE
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        default: {
            result = DefWindowProcA(window, message, w_param, l_param);
        }
    }
    
    return result;
}
#elif YD_MACOS
void
update_window_events() {
    
}
#elif YD_LINUX
void update_window_events() {
    
}
#endif // #if YD_WIN32

#endif // #if defined(YD_INPUT_IMPLEMENTATION)
