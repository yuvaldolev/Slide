struct Opengl_Info {
    b32 modern_context;
    
    const char* vendor;
    const char* renderer;
    const char* version;
    const char* shading_language_version;
    
    b32 opengl_ext_texture_srgb;
    b32 opengl_ext_framebuffer_srgb;
    b32 opengl_arb_framebuffer_object;
};

struct Opengl_Program_Common {
    GLuint prog_handle;
    
    GLint vert_p_id;
    GLint vert_uv_id;
    GLint vert_color_id;
};

struct Basic_Program {
    Opengl_Program_Common common;
    
    GLuint mvp_id;
    GLuint texture_sampler_id;
};

struct Opengl {
    Platform_Renderer header;
    
    Render_Commands render_commands;
    
    Renderer_Texture_Format::Type default_sprite_texture_format;
    
    GLuint vertex_buffer;
    
    u8 push_buffer_memory[65536];
    Textured_Vertex* vertex_array;
    Renderer_Texture* bitmap_array;
    GLuint* texture_handles;
    
    u32 max_quad_texture_count;
    u32 max_vertex_count;
    u32 max_texture_handle_count;
    
    Basic_Program basic_prog;
    Basic_Program text_prog;
    
    u32 white[4][4];
    Renderer_Texture white_texture;
};