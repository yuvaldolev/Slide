// TODO(yuval): When the program will be loaded via a dll these
// will need to be replaced with some kind of a state struct
global FT_Library global_ft_library = 0;
global Loaded_Font* global_first_loaded_font;

internal Font_Glyph*
get_font_glyph(Render_Commands* commands, Font* font, u8 character) {
    Font_Glyph* result = &font->glyphs[character];
    
    if (!is_valid_texture(result->texture)) {
        if (!FT_Load_Char(font->face, character, FT_LOAD_RENDER)) {
            FT_GlyphSlot glyph = font->face->glyph;
            
            Renderer_Texture* texture = &result->texture;
            *texture = acquire_texture(commands,
                                       glyph->bitmap.width,
                                       glyph->bitmap.rows);
            texture_queue_add_entry(commands->texture_queue, *texture,
                                    Renderer_Texture_Format::RED,
                                    Renderer_Texture_Format::RED,
                                    PUSH_COPY(glyph->bitmap.buffer,
                                              texture->width * texture->height));
            
            result->bearing = make_v2((f32)glyph->bitmap_left,
                                      (f32)glyph->bitmap_top);
            result->advance = make_v2((f32)glyph->advance.x,
                                      (f32)glyph->advance.y);
        } else {
            result = 0;
        }
    }
    
    return result;
}

internal Font*
get_font_at_size(const char* directory, const char* name, u32 size) {
    if (!global_ft_library) {
        FT_Init_FreeType(&global_ft_library);
    }
    
    Font* result = 0;
    
    // NOTE(yuval): Checking if the font is already loaded
    for (Loaded_Font* at = global_first_loaded_font;
         at;
         at = at->next) {
        Font* font = &at->font;
        
        if (strings_match(font->name, name) &&
            font->size == size) {
            result = font;
            break;
        }
    }
    
    // NOTE(yuval): If the font was not loaded yet,
    // then we need to load it...
    if (!result) {
        Loaded_Font* loaded_font = PUSH(Loaded_Font);
        
        // NOTE(yuval): Font Initialization
        Font* font = &loaded_font->font;
        font->name = name;
        font->size = size;
        
        // NOTE(yuval): Loading the font's face
        // TODO(yuval): Platform spcific define for max file path size
        char font_path[1024];
        String font_path_str = make_string(font_path, 0, sizeof(font_path));
        append(&font_path_str, directory);
        append(&font_path_str, '/'); // TODO(yuval): Check if directory ends with a slash
        append(&font_path_str, name);
        terminate_with_null(&font_path_str);
        FT_New_Face(global_ft_library, font_path, 0, &font->face);
        
        // NOTE(yuval): Setting the font size
        FT_Set_Pixel_Sizes(font->face, 0, size);
        
        // NOTE(yuval): Appending the loaded font to the loaded fonts list
        if (global_first_loaded_font) {
            Loaded_Font* font_list_at = global_first_loaded_font;
            for (; font_list_at->next; font_list_at = font_list_at->next);
            font_list_at->next = loaded_font;
        } else {
            global_first_loaded_font = loaded_font;
        }
        
        result = font;
    }
    
    return result;
}