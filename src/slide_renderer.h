#if !defined(SLIDE_RENDERER_H)

#define MAX_NORMAL_TEXTURE_COUNT 256
#define MAX_SPECIAL_TEXTURE_COUNT 16
#define TEXTURE_TRANSFER_BUFFER_SIZE (128*1024*1024)

#define TEXTURE_ARRAY_DIM 512
#define TEXTURE_SPECIAL_BIT 0x80000000

struct Platform_Renderer;
struct Render_Commands;
struct Renderer_Texture;

#define RENDERER_BEGIN_FRAME(name) Render_Commands* name(Platform_Renderer* renderer, Vector2u render_dim)
typedef RENDERER_BEGIN_FRAME(Renderer_Begin_Frame);

#define RENDERER_END_FRAME(name) void name(Platform_Renderer* renderer, Render_Commands* frame)
typedef RENDERER_END_FRAME(Renderer_End_Frame);

// TODO(yuval): Temporary! Use a texture queue instead
#define RENDERER_ALLOCATE_TEXTURE(name) Renderer_Texture name(Platform_Renderer* renderer, u32 width, u32 height, void* data)
typedef RENDERER_ALLOCATE_TEXTURE(Renderer_Allocate_Texture);

struct Renderer_Texture {
    void* texture_handle;
    Vector2u dim;
};

struct Texture_Op {
    Renderer_Texture texture;
    void* data;
};

struct Renderer_Font {
    Renderer_Texture characters[255];
};

struct Platform_Renderer_Limits {
    u32 max_quad_count_per_frame;
};

struct Platform_Renderer {
    Renderer_Begin_Frame* begin_frame;
    Renderer_End_Frame* end_frame;
    Renderer_Allocate_Texture* allocate_texture;
};

namespace Render_Group_Entry_Kind {
    enum Type {
        TEXTURED_QUADS
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
    
    Renderer_Font* default_font;
    
    Vector4 clear_color;
    
    u32 max_render_target_index;
};

#define SLIDE_RENDERER_H
#endif // #if !defined(SLIDE_RENDERER_H)