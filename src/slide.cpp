#include "slide.h"

#define YD_CONTEXT_IMPLEMENTATION
#include "yd/yd_context.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#include "slide_math.cpp"

#include "slide_fonts.cpp"
#include "slide_renderer.cpp"

internal Slide_Item*
make_slide_item(Slide_Item_Kind::Type kind, Vector2 pos,
                Slide_Item* next_item = 0) {
    Slide_Item* result = PUSH(Slide_Item);
    result->kind = kind;
    result->pos = pos;
    result->next_item = next_item;
    
    return result;
}

internal Slide_Item*
make_text_item(String text, Vector2 pos = make_v2(0.5f, 0.5f),
               Vector4 color = make_v4(1, 1, 1, 1)) {
    Slide_Item* result = make_slide_item(Slide_Item_Kind::TEXT, pos);
    
    Text_Item* text_item = &result->text;
    // TODO(yuval): Maybe copy the text string?
    text_item->text = text;
    text_item->color = color;
    
    return result;
}

Platform_Api platform;

internal void
update_and_render(Application* app, Render_Commands* render_commands, Input* input) {
    platform = app->platform_api;
    
    App_State* state = app->state;
    if (!state) {
        state = app->state = BOOTSTRAP_PUSH_STRUCT(App_State, arena);
        
        Context new_context = {};
        new_context.arena = &state->arena;
        PUSH_CONTEXT(new_context);
        
        Slideshow* slideshow = &state->slideshow;
        
        Slide* slide1 = PUSH(Slide);
        slide1->background_color = make_v4(0.5f, 0.03f, 0.06f, 1.0f);
        
        Slide_Item* slide1_text =
            make_text_item(BUNDLE_LITERAL("Hello, Friend."));
        slide1->first_item = slide1_text;
        
        slideshow->first_slide = slide1;
        
        Slide* slide2 = PUSH(Slide);
        slide2->background_color = make_v4(0.03f, 0.5f, 0.06f, 1.0f);
        slide2->prev_slide = slide1;
        slide1->next_slide = slide2;
        
        Slide_Item* slide2_text =
            make_text_item(BUNDLE_LITERAL("What i'm about to tell you is top secret."));
        slide2->first_item = slide2_text;
        
        Slide* slide3 = PUSH(Slide);
        slide3->background_color = make_v4(0.03f, 0.06f, 0.5f, 1.0f);
        slide3->prev_slide = slide2;
        slide2->next_slide = slide3;
        
        Slide_Item* slide3_text =
            make_text_item(BUNDLE_LITERAL("A conspiracy bigger than all of us."));
        slide3->first_item = slide3_text;
        
        Slide* slide4 = PUSH(Slide);
        slide4->background_color = make_v4(0.4f, 0.03f, 0.5f, 1.0f);
        slide4->prev_slide = slide3;
        slide3->next_slide = slide4;
        
        Slide_Item* slide4_text1 =
            make_text_item(BUNDLE_LITERAL("There's a powerful group of people out"),
                           make_v2(0.5f, 0.55f));
        Slide_Item* slide4_text2 =
            make_text_item(BUNDLE_LITERAL("there that are secretly running the world."),
                           make_v2(0.5f, 0.45f));
        slide4->first_item = slide4_text1;
        slide4->first_item->next_item = slide4_text2;
        
        slideshow->current_slide = slide1;
    }
    
    Context new_context = {};
    new_context.arena = &state->arena;
    PUSH_CONTEXT(new_context);
    
    Slideshow* slideshow = &state->slideshow;
    
    for (umm event_index = 0;
         event_index < input->app_frame_event_count;
         ++event_index) {
        Event* event = &input->app_frame_events[event_index];
        
        switch (event->kind) {
            case Event_Kind::BUTTON: {
                if (event->button.pressed) {
                    switch (event->button.code) {
                        case Button_Code::ARROW_RIGHT: {
                            if (slideshow->current_slide->next_slide) {
                                slideshow->current_slide =
                                    slideshow->current_slide->next_slide;
                            }
                        } break;
                        
                        case Button_Code::ARROW_LEFT: {
                            if (slideshow->current_slide->prev_slide)
                                slideshow->current_slide =
                                slideshow->current_slide->prev_slide;
                        } break;
                    }
                }
            } break;
        }
    }
    
    Render_Group render_group =
        begin_render_group(render_commands);
    
    if (slideshow->current_slide) {
        Slide* slide = slideshow->current_slide;
        
        Rectangle2 background_rect = rect_min_max(
            make_v2(0.0f, 0.0f),
            make_v2(render_commands->render_dim.width,
                    render_commands->render_dim.height));
        
        push_rect(&render_group, background_rect,
                  make_v3(0.0f, 0.0f, 0.0f), slide->background_color);
        
        Font* font = get_font_at_size("data/fonts", "Abang.ttf", 42);
        
        for (Slide_Item* item = slide->first_item;
             item;
             item = item->next_item) {
            Vector2 item_window_pos =
                hadamard(item->pos, v2_from(render_commands->render_dim));
            
            switch (item->kind) {
                case Slide_Item_Kind::TEXT: {
                    Text_Item* text_item = &item->text;
                    push_text(&render_group, text_item->text, font,
                              item_window_pos, make_v2(3.0f, 10.0f),
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