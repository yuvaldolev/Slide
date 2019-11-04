#if !defined(YD_MEMORY)

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
        
        KEYBOARD,
        TEXT_INPUT,
        WINDOW,
        MOUSE_WHEEL,
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

namespace Key_Code {
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
        
        DELETE = 127,
        
        ARROW_UP = 128,
        ARROW_DOWN = 129,
        ARROW_LEFT = 130,
        ARROW_RIGHT = 131,
        
        ALT,
        CTRL,
        SHIFT,
        
        MOUSE_WHEEL_UP,
        MOUSE_WHEEL_DOWN,
        
        MOUSE_WHEEL_LEFT,
        MOSUE_WHEEL_MIDDLE,
        MOUSE_WHEEL_RIGHT,
        
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

struct Keyboard_Event {
    Key_Code::Type key_code;
    yd_b32 key_press; // NOTE(yuval): If not pressed, it's a key release
    yd_b32 repeat;
    Modifier_Flags modifier_flags;
};

struct Text_Input_Event {
    yd_u32 utf32;
};

struct Mouse_Event {
    yd_s32 typical_wheel_delta;
    yd_s32 wheel_delta;
};

struct Event {
    Event_Kind::Type kind;
    
    union {
        Keyboard_Event keyboard;
        Text_Input_Event text_input;
        Mouse_Event mouse;
    }
};

struct Input {
    //
    // NOTE(yuval): Per-frame Input Data
    //
    
    // TODO(yuval): Replace this with a dynamic array of some sort
    // (preferably a bucket array)
    Event frame_events[1024];
    yd_umm frame_event_count;
    
    // TODO(yuval): Compress this to a Vector3
    yd_s32 mouse_delta_x;
    yd_s32 mouse_delta_y;
    yd_s32 mouse_delta_z;
    
    //
    // NOTE(yuval): Persistant Input Data
    //
    
    // NOTE(yuval): True if pressed, false if not
    yd_b32 button_states[Key_Code::COUNT];
    
    yd_b8 shift_state;
    yd_b8 ctrl_state;
    yd_b8 alt_state;
    
    yd_b32 application_has_focus;
};

extern Input global_input;

#define YD_MEMORY
#endif // #if !defined(YD_MEMORY)

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
// NOTE(yuval): Exported variable instantiations
//

Input global_input = {};

//
// NOTE(yuval): Internal Functions
//

yd_internal inline void
add_input_event(Event event) {
    global_input.frame_events[global_input.frame_event_count++] = event;
}

yd_internal void
update_modifier_key_states(WPARAM w_param, b32 is_pressed) {
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

//
// NOTE(yuval): Exported Function Implementations
//

#if YD_WIN32
void
update_window_events() {
    MSG message;
    while (PeekMessageA(&message, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

LRESULT CALLBACK
yd_input_window_callback(HWND window, UINT message,
                         WPARAM w_param, LPARAM l_param) {
    LRESULT result = 0;
    
    switch (message) {
        case WM_ACTIVATEAPP: {
            // TODO(yuval): Update the key_state array to the current key states
        } break;
        
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN: {
            Event event = {};
            event.kind = Event_Kind::KEYBOARD;
            event.keyboard.key_code = win32_get_key_code(w_param);
            event.keyboard.key_press = true;
            event.repeat = (message & (1 << 30));
            event.keyboard.modifier_flags.shift_pressed =
                global_input.shift_state;
            event.keyboard.modifier_flags.ctrl_pressed =
                global_input.ctrl_state;
            event.keyboard.modifier_flags.alt_pressed =
                global_input.alt_state;
            
            global_input.button_states[event.keyboard.key_code] = true;
            
            add_input_event(event);
            
            update_modifier_key_states(w_param, true);
        } break;
        
        case WM_SYSKEYUP:
        case WM_KEYUP: {
            Event event = {};
            event.kind = Event_Kind::KEYBOARD;
            event.keyboard.key_code = win32_get_key_code(w_param);
            event.keyboard.key_press = false;
            event.keyboard.modifier_flags.shift_pressed =
                global_input.shift_state;
            event.keyboard.modifier_flags.ctrl_pressed =
                glogal_input.ctrl_state;
            event.keyboard.modifier_flags.alt_pressed =
                global_input.alt_state;
            
            global_input.button_states[event.keyboard.key_code] = false;
            
            add_input_event(event);
            
            update_modifier_key_states(w_param, false);
        } break;
        
        case WM_CHAR {
            yd_u32 keycode = (u32)w_param;
            
            // NOTE(yuval): Control characters generate keycodes < 32,
            // but these are redundant with KEYDOWN events and also ambiguous
            // (ctrl-m generates keyboard 13, but so does RETURN).
            if (keycode > 31) {
                Event event;
                event.kind = Event_Kind::TEXT_INPUT;
                event.text_input.utf32 = keycode;
                
                add_input_event(event);
            }
        } break;
        
        case WM_SETFOCUS: {
            global_input.application_has_focus = true;
        } break;
        
        case WM_KILLFOCUS: {
            global_input.application_has_focus = false;
        } break;
        
        case WM_PAINT: {
            VaildateRect(window, null);
            result = DefWindowProcA(window, message, w_param, l_param);
        } break;
        
        case WM_LBUTTONDOWN
    }
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
