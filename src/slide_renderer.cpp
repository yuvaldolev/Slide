inline void
push_setup(Render_Group* group, Render_Setup* new_setup) {
    if (group->commands->max_render_target_index < new_setup->render_target_index) {
        group->commands->max_render_target_index = new_setup->render_target_index;
    }
    
    group->last_setup = *new_setup;
    group->current_quads = 0;
}

internal Push_Buffer_Result
push_buffer(Render_Group* group, u32 data_size) {
    Render_Commands* commands = group->commands;
    
    Push_Buffer_Result result = {};
    
    u8* push_buffer_end = commands->push_buffer_base +
        commands->max_push_buffer_size;
    if ((commands->push_buffer_data_at + data_size) <= push_buffer_end) {
        result.header = (Render_Group_Entry_Header*)
            commands->push_buffer_data_at;
        commands->push_buffer_data_at += data_size;
    } else {
        INVALID_CODE_PATH();
    }
    
    return result;
}

internal void*
push_render_element(Render_Group* group, u32 size,
                    Render_Group_Entry_Kind::Type kind) {
    Render_Commands* commands = group->commands;
    void* result = 0;
    
    size += sizeof(Render_Group_Entry_Header);
    
    Push_Buffer_Result push = push_buffer(group, size);
    if (push.header) {
        Render_Group_Entry_Header* header = push.header;
        header->kind = (u16)kind;
        result = (u8*)header + sizeof(*header);
    } else {
        INVALID_CODE_PATH();
    }
    
    group->current_quads = 0;
    
    return result;
}

internal Render_Entry_Textured_Quads*
get_current_quads(Render_Group* group, u32 quad_count) {
    Render_Commands* commands = group->commands;
    
    if (!group->current_quads) {
        group->current_quads = (Render_Entry_Textured_Quads*)
            push_render_element(group, sizeof(Render_Entry_Textured_Quads),
                                Render_Group_Entry_Kind::TEXTURED_QUADS);
        group->current_quads->quad_count = 0;
        group->current_quads->vertex_array_offset = commands->vertex_count;
        group->current_quads->setup = group->last_setup;
    }
    
    Render_Entry_Textured_Quads* result = group->current_quads;
    if ((commands->vertex_count + 4 * quad_count) > commands->max_vertex_count) {
        result = 0;
    }
    
    return result;
}

internal Vector4
store_color(Vector4 source) {
    Vector4 dest;
    dest.a = source.a;
    dest.r = dest.a * source.r;
    dest.g = dest.a * source.g;
    dest.b = dest.a * source.b;
    
    return dest;
}

internal void
push_quad(Render_Group* group,
          Renderer_Texture texture,
          Vector4 p0, Vector2 uv0, u32 c0,
          Vector4 p1, Vector2 uv1, u32 c1,
          Vector4 p2, Vector2 uv2, u32 c2,
          Vector4 p3, Vector2 uv3, u32 c3) {
    Render_Commands* commands = group->commands;
    
    Render_Entry_Textured_Quads* entry = group->current_quads;
    ASSERT(entry);
    
    ++entry->quad_count;
    
    commands->quad_textures[commands->vertex_count >> 2] = texture;
    Textured_Vertex* vert = &commands->vertex_array[commands->vertex_count];
    commands->vertex_count += 4;
    
    vert[0].p = p0;
    vert[0].uv = uv0;
    vert[0].color = c0;
    
    vert[1].p = p3;
    vert[1].uv = uv3;
    vert[1].color = c3;
    
    vert[2].p = p1;
    vert[2].uv = uv1;
    vert[2].color = c1;
    
    vert[3].p = p2;
    vert[3].uv = uv2;
    vert[3].color = c2;
}

internal void
push_rect(Render_Group* group,
          Vector3 p, Vector2 dim,
          Vector4 color = make_v4(1, 1, 1, 1)) {
    Vector3 basis_p = (p - make_v3(0.5f * dim, 0));
    Vector2 scaled_dim = dim;
    
    Render_Entry_Textured_Quads* entry = get_current_quads(group, 1);
    if (entry) {
        Vector4 premul_color = store_color(color);
        u32 c = rgba_pack_4x8(255.0f * premul_color);
        
        Vector3 min_p = basis_p;
        Vector3 max_p = min_p + make_v3(scaled_dim, 0.0f);
        
        Vector2 min_uv = {0, 0};
        Vector2 max_uv = {1, 1};
        
        push_quad(group, group->commands->white_texture,
                  // NOTE(yuval): Point 0
                  make_v4(min_p.x, min_p.y, min_p.z, 1.0f),
                  make_v2(min_uv.x, min_uv.y), c,
                  // NOTE(yuval): Point 1
                  make_v4(max_p.x, min_p.y, min_p.z, 1.0f),
                  make_v2(max_uv.x, min_uv.y), c,
                  // NOTE(yuval): Point 2
                  make_v4(max_p.x, max_p.y, min_p.z, 1.0f),
                  make_v2(max_uv.x, max_uv.y), c,
                  // NOTE(yuval): Point 3
                  make_v4(min_p.x, max_p.y, min_p.z, 1.0f),
                  make_v2(min_uv.x, max_uv.y), c);
    }
}

inline void
push_rect(Render_Group* group,
          Rectangle2 rectangle, Vector3 offset,
          Vector4 color = make_v4(1, 1, 1, 1)) {
    push_rect(group, offset + make_v3(get_center(rectangle), 0),
              get_dim(rectangle), color);
}

internal Vector2
get_texture_dim(Renderer_Texture texture, f32 height) {
    Vector2 result;
    
    f32 width_over_height = 1.0f;
    if (texture.dim.height != 0) {
        width_over_height = ((f32)texture.dim.width / (f32)texture.dim.height);
    }
    
    result.width = height * width_over_height;
    result.height = height;
    
    return result;
}

internal void
push_texture(Render_Group* group, Renderer_Texture texture,
             Vector3 min_p, Vector2 dim,
             Vector4 color = make_v4(1.0f, 1.0f, 1.0f, 1.0f)) {
    Render_Entry_Textured_Quads* entry = get_current_quads(group, 1);
    if (entry) {
        Vector4 premul_color = store_color(color);
        u32 c = rgba_pack_4x8(255.0f * premul_color);
        
        Vector3 max_p = min_p + make_v3(dim, 0.0f);
        
        Vector2 min_uv = {0, 0};
        Vector2 max_uv = {1, 1};
        
        push_quad(group, texture,
                  // NOTE(yuval): Point 0
                  make_v4(min_p.x, min_p.y, min_p.z, 1.0f),
                  make_v2(min_uv.x, min_uv.y), c,
                  // NOTE(yuval): Point 1
                  make_v4(max_p.x, min_p.y, min_p.z, 1.0f),
                  make_v2(max_uv.x, min_uv.y), c,
                  // NOTE(yuval): Point 2
                  make_v4(max_p.x, max_p.y, min_p.z, 1.0f),
                  make_v2(max_uv.x, max_uv.y), c,
                  // NOTE(yuval): Point 3
                  make_v4(min_p.x, max_p.y, min_p.z, 1.0f),
                  make_v2(min_uv.x, max_uv.y), c);
    }
}

inline void
push_texture(Render_Group* group, Renderer_Texture texture,
             Vector3 min_p, f32 height,
             Vector4 color = make_v4(1.0f, 1.0f, 1.0f, 1.0f)) {
    Vector2 scaled_dim = get_texture_dim(texture, height);
    push_texture(group, texture, min_p, scaled_dim, color);
}


inline void
push_texture(Render_Group* group, Renderer_Texture texture,
             Vector2 min_p, f32 height,
             Vector4 color = make_v4(1.0f, 1.0f, 1.0f, 1.0f)) {
    push_texture(group, texture, make_v3(min_p, 0.0f), height, color);
}

internal Vector2
get_character_dim(char character, Renderer_Font* font, f32 height) {
    Renderer_Texture char_tex = font->characters[character];
    f32 adjusted_height = height * ((f32)char_tex.dim.height /
                                    (f32)font->characters['F'].dim.height);
    
    Vector2 scaled_dim = get_texture_dim(char_tex, adjusted_height);
    return scaled_dim;
}

internal void
push_text(Render_Group* group, String text,
          Renderer_Font* font, Vector3 pos,
          f32 height, Vector2 spacing, Vector4 color) {
    f32 text_width = 0.0f;
    for (umm char_index = 0;
         char_index < text.count;
         ++char_index) {
        Vector2 char_dim = get_character_dim(text.data[char_index], font, height);
        text_width += (char_dim.width + spacing.x);
    }
    
    Vector3 curr_min_p = pos;
    curr_min_p.x -= (text_width / 2.0f);
    
    for (umm char_index = 0;
         char_index < text.count;
         ++char_index) {
        if (text.data[char_index] == '\n') {
            // TODO(yuval): Think about handling newlines
            // TODO(yuval): Add highest character height
            // curr_min_p.y -= spacing.y;
        } else {
            char character = text.data[char_index];
            Vector2 char_dim = get_character_dim(character, font, height);
            
#if 0
            // NOTE(yuval): Debug Rectangle
            push_rect(group,
                      rect_min_max(min_p.xy, min_p.xy + char_dim),
                      make_v3(0, 0, 0), make_v4(1, 1, 1, 1));
#endif // #if 0
            
            push_texture(group, font->characters[character], curr_min_p,
                         char_dim, color);
            
            curr_min_p.x += (char_dim.width + spacing.x);
        }
    }
}

inline void
push_text(Render_Group* group, String text,
          Renderer_Font* font, Vector2 min_p,
          f32 height, Vector2 spacing, Vector4 color) {
    push_text(group, text, font, make_v3(min_p, 0.0f),
              height, spacing, color);
}

inline void
push_text(Render_Group* group, const char* text,
          Renderer_Font* font, Vector3 min_p,
          f32 height, Vector2 spacing, Vector4 color) {
    push_text(group, make_string_slowly(text), font,
              min_p, height, spacing, color);
}

inline void
push_text(Render_Group* group, const char* text,
          Renderer_Font* font, Vector2 min_p,
          f32 height, Vector2 spacing, Vector4 color) {
    push_text(group, text, font, make_v3(min_p, 0.0f),
              height, spacing, color);
}

internal Render_Group
begin_render_group(Render_Commands* commands) {
    Render_Group result = {};
    
    result.commands = commands;
    
    Render_Setup initial_setup = {};
    initial_setup.fog_start_distance = 0.0f;
    initial_setup.fog_end_distance = 1.0f;
    initial_setup.clip_rect = rect_min_dim(0, 0, commands->render_dim.width,
                                           commands->render_dim.height);
    initial_setup.proj = orthographic_projection(0, (f32)commands->render_dim.width,
                                                 0, (f32)commands->render_dim.height,
                                                 -1.0f, 1.0f);
    
    push_setup(&result, &initial_setup);
    
    return result;
}

internal void
end_render_group(Render_Group* group) {
}