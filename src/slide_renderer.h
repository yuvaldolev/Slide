#if !defined(SLIDE_RENDERER_H)

struct Platform_Renderer;
struct Render_Commands;

namespace Draw_Mode {
    enum Type {
        LEFT_JUSTIFY,
        CENTERED,
        RIGHT_JUSTIFY
    };
}

#if 0
namespace Renderer_Texture_Target {
    enum Type {
        TEXTURE_2D
    };
}
#endif // #if 0

namespace Renderer_Texture_Format {
    enum Type {
        RGBA,
        BGRA,
        RED
    };
}

union Renderer_Texture {
    u64 packed;
    
    struct {
        u32 index;
        u16 width;
        u16 height;
    };
};

struct Renderer_Texture_Queue_Entry {
    Renderer_Texture texture;
    Renderer_Texture_Format::Type in_format;
    Renderer_Texture_Format::Type out_format;
    void* data;
};

struct Renderer_Texture_Queue {
    umm entry_count;
    umm first_entry_index;
    
    Renderer_Texture_Queue_Entry entries[256];
};

struct Platform_Renderer_Limits {
    u32 max_quad_count_per_frame;
    u32 max_texture_handle_count;
};

#define RENDERER_BEGIN_FRAME(name) Render_Commands* name(Platform_Renderer* renderer, Vector2u render_dim)
typedef RENDERER_BEGIN_FRAME(Renderer_Begin_Frame);

#define RENDERER_END_FRAME(name) void name(Platform_Renderer* renderer, Render_Commands* frame)
typedef RENDERER_END_FRAME(Renderer_End_Frame);

struct Platform_Renderer {
    Renderer_Begin_Frame* begin_frame;
    Renderer_End_Frame* end_frame;
    
    Renderer_Texture_Queue texture_queue;
};

namespace Render_Group_Entry_Kind {
    enum Type {
        TEXTURED_QUADS,
        TEXT
    };
}

struct Render_Group_Entry_Header {
    u16 kind;
};

struct Render_Setup {
    Rectangle2i clip_rect;
    u32 render_target_index;
    Matrix4x4 proj;
    
    Vector3 camera_p;
    Vector3 fog_direction;
    Vector3 fog_color;
    f32 fog_start_distance;
    f32 fog_end_distance;
};

struct Render_Entry_Textured_Quads {
    Render_Setup setup;
    
    u32 quad_count;
    u32 vertex_array_offset;
};

struct Push_Buffer_Result {
    Render_Group_Entry_Header* header;
};

struct Render_Group {
    Render_Commands* commands;
    
    Render_Entry_Textured_Quads* current_quads;
    
    Render_Setup last_setup;
};

struct Textured_Vertex {
    Vector4 p;
    Vector2 uv;
    
    // NOTE(casey): Packed RGBA in memory order (ABGR in little-endian)
    u32 color;
};

struct Render_Settings {
    // NOTE(yuval): The RenderDim is the actual size of all our render backbuffers
    Vector2u render_dim;
    
    u32 depth_peel_count_hint;
    b32 multisampling_debug;
    b32 multisampling_hint;
    b32 pixelation_hint;
    b32 lighting_disabled;
    b32 request_vsync;
};

struct Render_Commands {
    Vector2u os_window_dim;
    Rectangle2i os_draw_region;
    Vector2u render_dim;
    
    u32 max_push_buffer_size;
    u8* push_buffer_base;
    u8* push_buffer_data_at;
    
    u32 max_vertex_count;
    u32 vertex_count;
    Textured_Vertex* vertex_array;
    
    Renderer_Texture* quad_textures;
    Renderer_Texture white_texture;
    
    Vector4 clear_color;
    
    u32 next_texture_handle_index;
    u32 max_texture_handle_count;
    
    Renderer_Texture_Queue* texture_queue;
};

inline Renderer_Texture
acquire_texture(Render_Commands* commands, u32 width, u32 height);

internal void
texture_queue_add_entry(Renderer_Texture_Queue* queue,
                        Renderer_Texture texture,
                        Renderer_Texture_Format::Type in_format,
                        Renderer_Texture_Format::Type out_format,
                        void* data);
internal void
init_renderer_texture_queue(Renderer_Texture_Queue* queue) {
    queue->entry_count = 0;
    queue->first_entry_index = 0;
}

inline b32
is_valid_texture(Renderer_Texture texture) {
    b32 result = (texture.packed != 0);
    return result;
}

internal Renderer_Texture
refer_to_texture(u32 index, u32 width, u32 height) {
    Renderer_Texture result;
    
    result.index = index;
    result.width = (u16)width;
    result.height = (u16)height;
    
    ASSERT(result.index == index);
    ASSERT(result.width  == width);
    ASSERT(result.height == height);
    
    return result;
}

inline Renderer_Texture
refer_to_texture(u32 index, Vector2u dim) {
    Renderer_Texture result = refer_to_texture(index, dim.width, dim.height);
    return result;
}

#define SLIDE_RENDERER_H
#endif // #if !defined(SLIDE_RENDERER_H)