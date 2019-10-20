#if !defined(SLIDE_RENDERER_H)

#define MAX_NORMAL_TEXTURE_COUNT 256
#define MAX_SPECIAL_TEXTURE_COUNT 16
#define TEXTURE_TRANSFER_BUFFER_SIZE (128*1024*1024)

#define TEXTURE_ARRAY_DIM 512
#define TEXTURE_SPECIAL_BIT 0x80000000

struct Platform_Renderer;
struct Render_Commands;

#define RENDERER_BEGIN_FRAME(name) Render_Commands* name(Platform_Renderer* renderer, u32 window_width, u32 window_height, Rectangle2i draw_region)
typedef RENDERER_BEGIN_FRAME(Renderer_Begin_Frame);

#define RENDERER_END_FRAME(name) void name(Platform_Renderer* renderer, Render_Commands* frame)
typedef RENDERER_END_FRAME(Renderer_End_Frame);

union Renderer_Texture {
    u64 packed;
    
    struct {
        u32 index;
        u16 width;
        u16 height;
    };
};

namespace Texture_Op_State {
    enum Type {
        EMPTY,
        PENDING_LOAD,
        READY_TO_TRANSFER
    };
}

struct Texture_Op {
    Renderer_Texture texture;
    void* data;
    u32 transfer_memory_last_used;
    volatile Texture_Op_State::Type state;
};

struct Renderer_Texture_Queue {
    u32 transfer_memory_count;
    u32 transfer_memory_first_used;
    u32 transfer_memory_last_used;
    u8* transfer_memory;
    
    u32 op_count;
    u32 first_op_index;
    Texture_Op ops[256];
};

struct Platform_Renderer_Limits {
    u32 max_quad_count_per_frame;
    u32 max_texture_count;
    u32 max_special_texture_count;
    u32 texture_transfer_buffer_size;
};

struct Platform_Renderer {
    Renderer_Texture_Queue texture_queue;
    
    Renderer_Begin_Frame* begin_frame;
    Renderer_End_Frame* end_frame;
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
    Rectangle2 clip_rect;
    u32 render_target_index;
    Matrix4x4 proj;
    Vector3 camera_p;
    
    Vector3 fog_direction;
    Vector3 fog_color;
    f32 fog_start_distance;
    f32 fog_end_distance;
    
    f32 clip_alpha_start_distance;
    f32 clip_alpha_end_distance;
};

struct Render_Entry_Textured_Quads {
    Render_Setup setup;
    
    u32 quad_count;
    u32 vertex_array_offset;
    u32 index_array_offset;
    
    // NOTE(yuval): Textures is 0 if using the default texture array / single batch render,
    // and an array of one texture per quad if not.
    Renderer_Texture* quad_textures;
};

struct Push_Buffer_Result {
    Render_Group_Entry_Header* header;
};

namespace Render_Group_Flags {
    enum Type {
        DEFAULT
    };
}

struct Render_Group {
    Render_Commands* commands;
    
    Render_Entry_Textured_Quads* current_quads;
    
    // NOTE(yuval): The app must supply this, otherwise it will not be set
    Renderer_Texture white_texture;
    
    Render_Setup last_setup;
    
    u32 flags;
};

struct Textured_Vertex {
    Vector4 p;
    Vector2 light_uv;
    Vector2 uv;
    // NOTE(casey): Packed RGBA in memory order (ABGR in little-endian)
    u32 color;
    
    Vector3 n;
    u16 light_index;
    u16 texture_index;
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
    Render_Settings settings;
    
    // NOTE(yuval): OSWindowWidth/Height should NOT be used for anything drawing-related!
    // They are just there to let you know the shape of the container window into
    // which you are drawing.  DrawRegion contains the actual width/height you would
    // use for drawing.
    Vector2u os_window_dim;
    // NOTE(yuval): This is the subsection of the window to which we will draw
    Rectangle2i os_draw_region;
    
    u32 max_push_buffer_size;
    u8* push_buffer_base;
    u8* push_buffer_data_at;
    
    u32 max_vertex_count;
    u32 vertex_count;
    Textured_Vertex* vertex_array;
    
    u32 max_index_count;
    u32 index_count;
    u16* index_array;
    
    u32 max_quad_texture_count;
    u32 quad_texture_count;
    Renderer_Texture* quad_textures;
};

internal void
init_texture_queue(Renderer_Texture_Queue* queue,
                   u32 request_transfer_buffer_size,
                   void* memory) {
    queue->transfer_memory_count = request_transfer_buffer_size;
    queue->transfer_memory_first_used = 0;
    queue->transfer_memory_last_used = 0;
    queue->transfer_memory = (u8*)memory;
    
    queue->op_count = 0;
    queue->first_op_index = 0;
    
}

internal b32
is_special_texture(Renderer_Texture texture) {
    b32 result = (texture.index & TEXTURE_SPECIAL_BIT);
    return result;
}

internal u32
texture_index_from(Renderer_Texture texture) {
    u32 result = (texture.index & ~TEXTURE_SPECIAL_BIT);
    return result;
}

inline b32
settings_match(Render_Settings* a, Render_Settings* b) {
    b32 result = memories_match(a, b, sizeof(*a));
    return result;
}

#define SLIDE_RENDERER_H
#endif // #if !defined(SLIDE_RENDERER_H)