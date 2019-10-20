struct Opengl_Info {
    b32 modern_context;
    
    char* vendor;
    char* renderer;
    char* version;
    char* shading_language_version;
    
    b32 opengl_ext_texture_srgb;
    b32 opengl_ext_framebuffer_srgb;
    b32 opengl_arb_framebuffer_object;
};

struct Opengl_Program_Common {
    GLuint prog_handle;
    
    GLuint vert_p_id;
    GLuint vert_n_id;
    GLuint vert_uv_id;
    GLuint vert_color_id;
    GLuint vert_light_index;
    GLuint vert_texture_index;
    
    u32 sampler_count;
    GLuint samplers[16];
};

struct Z_Bias_Program {
    Opengl_Program_Common common;
    
    GLuint transform_id;
    GLuint camera_p;
    GLuint fog_direction;
    GLuint fog_color;
    GLuint fog_start_distance;
    GLuint fog_end_distance;
    GLuint clip_alpha_start_distance;
    GLuint clip_alpha_end_distance;
    GLuint alpha_threshold;
};

namespace Opengl_Color_Handle_Kind {
    enum Type {
        SURFACE_REFLECT,
        
        COUNT
    };
}

namespace Opengl_Framebuffer_Flags {
    enum Type {
        MULTISAMPLED,
        FILTERED,
        DEPTH,
        FLOAT
    };
}

struct Opengl_Framebuffer {
    GLuint framebuffer_handle;
    GLuint color_handle[Opengl_Color_Handle_Kind::COUNT];
    GLuint depth_handle;
};

struct Opengl {
    Platform_Renderer header;
    
    Render_Settings current_settings;
    
    Render_Commands render_commands;
    
    GLint max_color_attachments;
    GLint max_samplers_per_shader;
    
    GLuint shader_sim_tex_read_srgb;
    GLuint shader_sim_tex_write_srgb;
    
    GLint max_multisample_count;
    
    b32 supports_srgb_framebuffer;
    
    GLuint default_sprite_texture_format;
    GLuint default_framebuffer_texture_format;
    
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint screen_fill_vertex_buffer;
    
    GLuint reserved_blit_texture;
    
    GLuint texture_array;
    
    u32 white[4][4];
    
    b32 multisampling;
    u32 depth_peel_count;
    
    u8 push_buffer_memory[65536];
    Textured_Vertex* vertex_array;
    u16* index_array;
    Renderer_Texture* bitmap_array;
    
    u32 max_quad_texture_count;
    u32 max_texture_count;
    u32 max_vertex_count;
    u32 max_index_count;
    
    u32 max_special_texture_count;
    GLuint* special_texture_handles;
    
    //
    // NOTE(yuval): Dynamic resources that get rereated when settings change
    //
    Opengl_Framebuffer resolve_framebuffer;
    Opengl_Framebuffer depth_peel_framebuffer[16];
    Opengl_Framebuffer depth_peel_resolve_buffer[16];
    Z_Bias_Program z_bias_no_depth_peel;
    Z_Bias_Program z_bias_depth_peel;
    Opengl_Program_Common peel_composite;
    Opengl_Program_Common final_stretch;
};