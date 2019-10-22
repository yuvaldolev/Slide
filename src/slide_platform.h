#if !defined(SLIDE_PLATFORM_H)

#include "slide_types.h"
#include "slide_math.h"

#define PLATFORM_DISPLAY_MESSAGE_BOX(name) void name(const char* title, const char* message)
typedef PLATFORM_DISPLAY_MESSAGE_BOX(Platform_Display_Message_Box);

struct Platform_Api {
    Platform_Display_Message_Box* display_message_box;
};

extern Platform_Api platform;

struct App {
    Platform_Api platform_api;
    
    Vector2u window_dim;
};

#define SLIDE_PLATFORM_H
#endif // #if !defined(SLIDE_PLATFORM_H)