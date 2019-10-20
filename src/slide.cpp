#include "slide.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#include "slide_math.cpp"

#include "slide_renderer.cpp"

Platform_Api platform;

internal void
update_and_render(App* app, Render_Commands* render_commands) {
    platform = app->platform_api;
    
    Render_Group render_group =
        begin_render_group(render_commands, Render_Group_Flags::DEFAULT,
                           make_v4(0.0f, 1.0f, 0.0f, 1.0f));
    
    push_rect(&render_group,
              rect_min_max(make_v2(0.0f, 0.0f), make_v2(100.0f, 100.0f)),
              make_v3(0.0f, 0.0f, 0.0f), make_v4(1.0f, 0.0f, 0.0f, 1.0f));
    
    end_render_group(&render_group);
    
}