#include "slide.h"

#define YD_CONTEXT_IMPLEMENTATION
#include "yd/yd_context.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_STRING_FORMAT_IMPLEMENTATION
#include "yd/yd_string_format.h"

#define YD_LOG_IMPLEMENTATION
#include "yd/yd_log.h"

#define YD_HOTLOADER_IMPLEMENTATION
#include "yd/yd_hotloader.h"

#include "slide_math.cpp"

#include "slide_fonts.cpp"
#include "slide_renderer.cpp"

internal Slide_Item*
make_slide_item(Slide_Item_Kind::Type kind, Vector2 pos,
                Slide_Item* next_item = 0) {
    Slide_Item* result = PUSH(Slide_Item);
    result->kind = kind;
    result->pos = pos;
    result->next = next_item;
    
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

internal void
load_slideshow(Slideshow* slideshow, const char* full_path) {
    const char* AGENT = "load_slideshow";
    
    String slide_file = platform.read_entire_file(full_path);
    
    if (!is_null_string(slide_file)) {
        DLIST_INIT(&slideshow->slide_sentinel);
        
        Slide* current_slide = 0;
        Slide_Item* current_slide_item = 0;
        
        umm line_number = 0;
        
        for (;;) {
            String line = consume_line(&slide_file);
            if (is_null_string(line)) {
                break;
            }
            
            line = eat_spaces(line);
            
            if (line.count != 0) {
                if (line[0] != '#') {
                    if (line[0] == ':') {
                        // NOTE(yuval): Command Parsing
                        advance(&line, 1);
                        line = eat_leading_spaces(line);
                        
                        String command = consume_word(&line);
                        if (match(command, "slide")) {
                            Slide* slide = PUSH(Slide);
                            DLIST_INSERT_BACK(&slideshow->slide_sentinel, &slide->link);
                            current_slide = slide;
                            current_slide_item = 0;
                        }
                        else if (match(command, "background")) {
                            String r_str = consume_word(&line);
                            F32_Conversion_Result r_conversion = to_f32(r_str);
                            if (!r_conversion.success) {
                                log(AGENT, "Error in line: %d: Unable to read 'red' color.\n",
                                    line_number);
                                continue;
                            }
                            f32 r = r_conversion.value;
                            
                            String g_str = consume_word(&line);
                            F32_Conversion_Result g_conversion = to_f32(g_str);
                            if (!g_conversion.success) {
                                log(AGENT, "Error in line: %d: Unable to read 'red' color.\n",
                                    line_number);
                                continue;
                            }
                            f32 g = g_conversion.value;
                            
                            String b_str = consume_word(&line);
                            F32_Conversion_Result b_conversion = to_f32(b_str);
                            if (!b_conversion.success) {
                                log(AGENT, "Error in line: %d: Unable to read 'red' color.\n",
                                    line_number);
                                continue;
                            }
                            f32 b = b_conversion.value;
                            
                            String a_str = consume_word(&line);
                            F32_Conversion_Result a_conversion = to_f32(a_str);
                            f32 a;
                            if (a_conversion.success) {
                                a = a_conversion.value;
                            } else {
                                a = 1.0f;
                            }
                            
                            if (current_slide) {
                                current_slide->background_color = make_v4(r, g, b, a);
                            } else {
                                log(AGENT, "Error in line: %d: Got a background color for a slide, but we haven't started any slide yet.",
                                    line_number);
                                break;
                            }
                        } else {
                            log(AGENT, "************** UNSUPPORTED COMMAND: %S", command);
                        }
                    } else {
                        // NOTE(yuval): Text Parsing
                        if (current_slide) {
                            Slide_Item* text = make_text_item(line);
                            if (current_slide_item) {
                                current_slide_item->next = text;
                            } else {
                                current_slide->first_item = text;
                                current_slide_item = text;
                            }
                        } else {
                            log(AGENT, "Error in line: %d: Got text for a slide, but we haven't started any slide yet.",
                                line_number);
                            break;
                        }
                    }
                }
            }
            
            ++line_number;
        }
        
        if (current_slide) {
            slideshow->current_slide = (Slide*)slideshow->slide_sentinel.next;
        }
    }
}

internal HOTLOADER_CALLBACK(slide_hotloader_callback) {
    if (!handled) {
        log("hotloader_callback", "Non-catalog asset change '%S'",
            change->full_name);
        
        if (match(change->extension, "slide")) {
            load_slideshow(&the_app->state->slideshow, change->full_name.data);
        }
    }
}

Application* the_app;
Platform_Api platform;

internal void
update_and_render(Application* app, Render_Commands* render_commands, Input* input) {
    the_app = app;
    platform = app->platform_api;
    
    App_State* state = app->state;
    if (!state) {
        state = app->state = BOOTSTRAP_PUSH_STRUCT(App_State, arena);
        
        Context new_context = {};
        new_context.arena = &state->arena;
        PUSH_CONTEXT(new_context);
        
        F32_Conversion_Result result = to_f32(BUNDLE_LITERAL("-0.5668"));
        
        hotloader_init(&state->hotloader);
        hotloader_register_callback(&state->hotloader, slide_hotloader_callback);
        
        load_slideshow( &state->slideshow, "data/slideshows/test.slide");
        
#if 0
        DLIST_INIT(&slideshow->slide_sentinel);
        
        // NOTE(yuval): Slide 1
        Slide* slide1 = PUSH(Slide);
        slide1->background_color = make_v4(0.5f, 0.03f, 0.06f, 1.0f);
        
        Slide_Item* slide1_text =
            make_text_item(BUNDLE_LITERAL("Hello, Friend."));
        slide1->first_item = slide1_text;
        
        DLIST_INSERT_BACK(&slideshow->slide_sentinel, &slide1->link);
        
        // NOTE(yuval): Slide 2
        Slide* slide2 = PUSH(Slide);
        slide2->background_color = make_v4(0.03f, 0.5f, 0.06f, 1.0f);
        
        Slide_Item* slide2_text =
            make_text_item(BUNDLE_LITERAL("What i'm about to tell you is top secret."));
        slide2->first_item = slide2_text;
        
        DLIST_INSERT_BACK(&slideshow->slide_sentinel, &slide2->link);
        
        // NOTE(yuval): Slide 3
        Slide* slide3 = PUSH(Slide);
        slide3->background_color = make_v4(0.03f, 0.06f, 0.5f, 1.0f);
        
        Slide_Item* slide3_text =
            make_text_item(BUNDLE_LITERAL("A conspiracy bigger than all of us."));
        slide3->first_item = slide3_text;
        
        DLIST_INSERT_BACK(&slideshow->slide_sentinel, &slide3->link);
        
        // NOTE(yuval): Slide 4
        Slide* slide4 = PUSH(Slide);
        slide4->background_color = make_v4(0.4f, 0.03f, 0.5f, 1.0f);
        
        Slide_Item* slide4_text1 =
            make_text_item(BUNDLE_LITERAL("There's a powerful group of people out"),
                           make_v2(0.5f, 0.55f));
        Slide_Item* slide4_text2 =
            make_text_item(BUNDLE_LITERAL("there that are secretly running the world."),
                           make_v2(0.5f, 0.45f));
        slide4->first_item = slide4_text1;
        slide4->first_item->next = slide4_text2;
        
        DLIST_INSERT_BACK(&slideshow->slide_sentinel, &slide4->link);
        
        slideshow->current_slide = slide1;
#endif // #if 0
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
                            if (!(slideshow->current_slide->link.next ==
                                  &slideshow->slide_sentinel)) {
                                slideshow->current_slide =
                                    (Slide*)slideshow->current_slide->link.next;
                            }
                        } break;
                        
                        case Button_Code::ARROW_LEFT: {
                            if (!(slideshow->current_slide->link.prev ==
                                  &slideshow->slide_sentinel)) {
                                slideshow->current_slide =
                                    (Slide*)slideshow->current_slide->link.prev;
                            }
                        } break;
                        
                        case Button_Code::F1: {
                            state->should_view_logs = !state->should_view_logs;
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
        
        Font* font = get_font_at_size("data/fonts", "KarminaBold.ttf", 42);
        
        for (Slide_Item* item = slide->first_item;
             item;
             item = item->next) {
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
    }
    
    if (state->should_view_logs) {
        Vector2 min_p =
            make_v2(0.0f, render_commands->render_dim.height * 0.2f);
        
        Rectangle2 background_rect = rect_min_max(
            min_p,
            make_v2(render_commands->render_dim.width,
                    render_commands->render_dim.height));
        
        push_rect(&render_group, background_rect,
                  make_v3(0.0f, 0.0f, 0.0f),
                  make_v4(5.0f / 255.0f, 35.0f / 255.0f, 41.0f / 255.0f, 1.0f));
        
        Font* font = get_font_at_size("data/fonts", "DroidSansMono.ttf", 16);
        
        f32 pad_y = 20.0f;
        Vector2 pos = (min_p + make_v2(15.0f, pad_y));
        Log* log_at = global_log_list;
        while (pos.y < render_commands->render_dim.height) {
            if (!log_at) {
                break;
            }
            
            push_text(&render_group, log_at->message, font,
                      pos, make_v2(1.0f, 0.0f),
                      make_v4(1.0f, 1.0f, 1.0f, 1.0f),
                      Draw_Mode::LEFT_JUSTIFY);
            
            // TODO(yuval): Get actual character height from the font itself
            pos.y += 10.0f + pad_y;
            log_at = log_at->next;
        }
    }
    
    end_render_group(&render_group);
    
    while (hotloader_process_change(&state->hotloader));
}