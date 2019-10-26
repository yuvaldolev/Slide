#include "slide.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#include "slide_math.cpp"

#include "slide_renderer.cpp"

Platform_Api platform;

internal Slide_Item*
make_slide_item(Memory_Arena* arena, Slide_Item_Kind::Type kind, Vector2 pos,
                Slide_Item* next_item = 0) {
    Slide_Item* result = PUSH_STRUCT(arena, Slide_Item);
    result->kind = kind;
    result->pos = pos;
    result->next_item = next_item;
    
    return result;
}

internal Slide_Item*
make_text_item(Memory_Arena* arena,
               String text, Vector2 pos = make_v2(0.5f, 0.5f),
               Vector4 color = make_v4(1, 1, 1, 1)) {
    Slide_Item* result = make_slide_item(arena, Slide_Item_Kind::TEXT, pos);
    
    Text_Item* text_item = &result->text;
    // TODO(yuval): Maybe copy the text string?
    text_item->text = text;
    text_item->color = color;
    
    return result;
}

internal void
update_and_render(Application* app, Render_Commands* render_commands) {
    platform = app->platform_api;
    
    Slideshow_State* state = app->state;
    if (!state) {
        state = app->state = BOOTSTRAP_PUSH_STRUCT(Slideshow_State, arena);
        
        Slideshow* slideshow = &state->slideshow;
        
        Slide* slide1 = PUSH_STRUCT(&state->arena, Slide);
        slide1->background_color = make_v4(0.5f, 0.03f, 0.06f, 1.0f);
        
        Slide_Item* slide1_text =
            make_text_item(&state->arena, BUNDLE_LITERAL("Hello World"));
        slide1->first_item = slide1_text;
        
        slideshow->first_slide = slide1;
        
        Slide* slide2 = PUSH_STRUCT(&state->arena, Slide);
        slide2->background_color = make_v4(0.03f, 0.5f, 0.06f, 1.0f);
        slide2->prev_slide = slide1;
        slide1->next_slide = slide2;
        
        Slide* slide3 = PUSH_STRUCT(&state->arena, Slide);
        slide3->background_color = make_v4(0.03f, 0.06f, 0.5f, 1.0f);
        slide3->prev_slide = slide2;
        slide2->next_slide = slide3;
        
        Slide* slide4 = PUSH_STRUCT(&state->arena, Slide);
        slide4->background_color = make_v4(0.4f, 0.03f, 0.5f, 1.0f);
        slide4->prev_slide = slide3;
        slide3->next_slide = slide4;
        
        slideshow->current_slide = slide1;
    }
    
    Render_Group render_group =
        begin_render_group(render_commands);
    
    Slideshow* slideshow = &state->slideshow;
    if (slideshow->current_slide) {
        Slide* slide = slideshow->current_slide;
        
        Rectangle2 background_rect = rect_min_max(
            make_v2(0.0f, 0.0f),
            make_v2(render_commands->render_dim.width,
                    render_commands->render_dim.height));
        
        push_rect(&render_group, background_rect,
                  make_v3(0.0f, 0.0f, 0.0f), slide->background_color);
        
        for (Slide_Item* item = slide->first_item;
             item;
             item = item->next_item) {
            Vector2 item_window_pos =
                hadamard(item->pos, v2_from(render_commands->render_dim));
            
            switch (item->kind) {
                case Slide_Item_Kind::TEXT: {
                    Text_Item* text_item = &item->text;
                    push_text(&render_group, text_item->text,
                              render_commands->default_font,
                              item_window_pos, 48.0f, make_v2(7.0f, 10.0f),
                              text_item->color);
                } break;
            }
        }
        
#if 0
        push_text(&render_group, "Hello World, Program", render_commands->default_font,
                  make_v2(200, 500), 72.0f, make_v2(10.0f, 15.0f), make_v4(0.2f, 0.3f, 0.8f, 1.0f));
#endif // #if 0
    }
    end_render_group(&render_group);
    
}