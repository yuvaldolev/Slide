#include "slide.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#include "slide_math.cpp"

#include "slide_renderer.cpp"

Platform_Api platform;

internal void
update_and_render(App* app, Renderer* renderer) {
    platform = app->platform_api;
    
    renderer->begin_frame(renderer,
                          (f32)app->window_dim.width,
                          (f32)app->window_dim.height);
    push_filled_rect(renderer,
                     rect_min_max(make_v2(50, 50), make_v2(100, 100)),
                     make_v4(1, 0, 0, 1));
    renderer->end_frame(renderer);
}