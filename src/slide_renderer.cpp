inline void
push_setup(Render_Group* group, Render_Setup* new_setup) {
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
get_current_quads(Render_Group* group, u32 quad_count,
                  Renderer_Texture texture) {
    Render_Commands* commands = group->commands;
    
    u32 vertex_count = quad_count * 4;
    u32 index_count = quad_count * 6;
    
    u32 max_quads_per_batch = (U16_MAX - 1) / 4;
    
    if (group->current_quads &&
        ((group->current_quads->quad_count + quad_count) > max_quads_per_batch)) {
        group->current_quads = 0;
    }
    
    b32 this_textures = is_special_texture(texture);
    if (group->current_quads) {
        b32 current_textures = (group->current_quads->quad_textures != 0);
        
        if (current_textures != this_textures) {
            group->current_quads = 0;
        }
    }
    
    if (!group->current_quads) {
        group->current_quads = (Render_Entry_Textured_Quads*)
            push_render_element(group,
                                sizeof(Render_Entry_Textured_Quads),
                                Render_Group_Entry_Kind::TEXTURED_QUADS);
        group->current_quads->quad_count = 0;
        group->current_quads->vertex_array_offset = commands->vertex_count;
        group->current_quads->index_array_offset = commands->index_count;
        group->current_quads->setup = group->last_setup;
        group->current_quads->quad_textures = 0;
        
        if (this_textures) {
            group->current_quads->quad_textures =
                (commands->quad_textures + commands->quad_texture_count);
        }
    }
    
    Render_Entry_Textured_Quads* result = group->current_quads;
    if (((commands->vertex_count + vertex_count) >
         commands->max_vertex_count) &&
        ((commands->index_count + index_count) >
         commands->max_index_count) &&
        (this_textures &&
         ((commands->quad_texture_count + quad_count) >
          commands->max_quad_texture_count))) {
        result = 0;
        INVALID_CODE_PATH();
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
          Vector4 p3, Vector2 uv3, u32 c3,
          f32 emission = 0.0f,
          u16 light_count = 0,
          u16 light_index = 0) {
    ASSERT((emission >= 0) && (emission <= 1));
    
    Render_Commands* commands = group->commands;
    
    Render_Entry_Textured_Quads* entry = group->current_quads;
    ASSERT(entry);
    
    ++entry->quad_count;
    
    Vector2 inv_uv = {
        (f32)texture.width / (f32)TEXTURE_ARRAY_DIM,
        (f32)texture.height / (f32)TEXTURE_ARRAY_DIM
    };
    
    u32 texture_index_32 = texture_index_from(texture);
    u16 texture_index = (u16)texture_index_32;
    ASSERT((u32)texture_index == texture_index_32);
    
    if (is_special_texture(texture)) {
        inv_uv.x = 1.0f;
        inv_uv.y = 1.0f;
        commands->quad_textures[commands->quad_texture_count++] = texture;
        texture_index = 0;
    }
    
    u32 vert_index = commands->vertex_count;
    u32 index_index = commands->index_count;
    commands->vertex_count += 4;
    commands->index_count += 6;
    ASSERT(commands->vertex_count <= commands->max_vertex_count);
    ASSERT(commands->index_count <= commands->max_index_count);
    ASSERT(commands->quad_texture_count <=
           commands->max_quad_texture_count);
    
    Textured_Vertex* vert = commands->vertex_array + vert_index;
    u16* index = commands->index_array + index_index;
    
    uv0 = hadamard(inv_uv, uv0);
    uv1 = hadamard(inv_uv, uv1);
    uv2 = hadamard(inv_uv, uv2);
    uv3 = hadamard(inv_uv, uv3);
    
    Vector4 e10 = (p1 - p0);
    e10.z += e10.w;
    
    Vector4 e20 = (p2 - p0);
    e20.z += e20.w;
    
    Vector3 normal_direction = cross(e10.xyz, e20.xyz);
    Vector3 normal = noz(normal_direction);
    
    Vector3 n0, n1, n2, n3;
    n0 = n1 = n2 = n3 = normal;
    
    vert[0].p = p3;
    vert[0].n = n3;
    vert[0].uv = uv3;
    vert[0].color = c3;
    vert[0].light_index = light_index;
    vert[0].texture_index = texture_index;
    
    vert[1].p = p0;
    vert[1].n = n0;
    vert[1].uv = uv0;
    vert[1].color = c0;
    vert[1].light_index = light_index;
    vert[1].texture_index = texture_index;
    
    vert[2].p = p2;
    vert[2].n = n2;
    vert[2].uv = uv2;
    vert[2].color = c2;
    vert[2].light_index = light_index;
    vert[2].texture_index = texture_index;
    
    vert[3].p = p1;
    vert[3].n = n1;
    vert[3].uv = uv1;
    vert[3].color = c1;
    vert[3].light_index = light_index;
    vert[3].texture_index = texture_index;
    
    u32 base_index = (vert_index - entry->vertex_array_offset);
    u16 vi = (u16)base_index;
    ASSERT((u32)vi == base_index);
    
    index[0] = vi + 0;
    index[1] = vi + 1;
    index[2] = vi + 2;
    index[3] = vi + 1;
    index[4] = vi + 3;
    index[5] = vi + 2;
}

internal void
push_rect(Render_Group* group,
          Vector3 p, Vector2 dim,
          Vector4 color = make_v4(1, 1, 1, 1)) {
    Vector3 basis_p = (p - make_v3(0.5f * dim, 0));
    Vector2 scaled_dim = dim;
    
    Render_Entry_Textured_Quads* entry =
        get_current_quads(group, 1, group->white_texture);
    if (entry) {
        Vector4 premul_color = store_color(color);
        u32 c = rgba_pack_4x8(255.0f * premul_color);
        
        Vector3 min_p = basis_p;
        Vector3 max_p = min_p + make_v3(scaled_dim, 0);
        
        Vector2 min_uv = {0, 0};
        Vector2 max_uv = {1, 1};
        
        push_quad(group, group->white_texture,
                  // NOTE(yuval): Point 0
                  make_v4(min_p.x, min_p.y, min_p.z, 0),
                  make_v2(min_uv.x, min_uv.y), c,
                  // NOTE(yuval): Point 1
                  make_v4(max_p.x, min_p.y, min_p.z, 0),
                  make_v2(max_uv.x, min_uv.y), c,
                  // NOTE(yuval): Point 2
                  make_v4(max_p.x, max_p.y, min_p.z, 0),
                  make_v2(max_uv.x, max_uv.y), c,
                  // NOTE(yuval): Point 3
                  make_v4(min_p.x, max_p.y, min_p.z, 0),
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

internal Render_Group
begin_render_group(Render_Commands* commands,
                   u32 flags = Render_Group_Flags::DEFAULT,
                   Vector4 clear_color = make_v4(0.0f, 0.0f, 0.0f, 0.0f)) {
    Render_Group result = {};
    
    result.commands = commands;
    result.flags = flags;
    
    Render_Setup initial_setup = {};
    initial_setup.fog_start_distance = 0.0f;
    initial_setup.fog_end_distance = 1.0f;
    initial_setup.clip_alpha_start_distance = 0.0f;
    initial_setup.clip_alpha_end_distance = 1.0f;
    initial_setup.clip_rect = rect_min_max(make_v2(-1.0f, -1.0f),
                                           make_v2(1.0f, 1.0f));
    initial_setup.proj = identity();
    push_setup(&result, &initial_setup);
    
    return result;
}

internal void
end_render_group(Render_Group* group) {
}