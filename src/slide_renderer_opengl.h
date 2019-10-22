struct Opengl_Fbo {
    u32 flags;
    
    GLuint fbo;
    GLuint color_textures[4];
    GLuint depth_texture;
    
    union {
        struct {
            u32 w;
            u32 h;
        };
        
        struct {
            u32 width;
            u32 height;
        };
    };
};

struct Opengl_Program_Io {
    int index;
    char* name;
};

struct Opengl_Program_Common {
    GLuint prog_handle;
};

struct Opengl_World_Program {
    Opengl_Program_Common common;
    
    GLuint inverse_view_projection;
    GLuint shadow_view_projection;
    GLuint albedo_texture;
    GLuint normal_texture;
    GLuint depth_texture;
    GLuint shadow_map_texture;
};

struct Opengl_Fxaa_Program {
    Opengl_Program_Common common;
    
    GLuint color_tex;
    GLuint color_tex_resolution;
};

struct Opengl_Filled_Rect_Program {
    Opengl_Program_Common common;
    
    GLuint view_projection;
};

struct Renderer_Opengl {
    Renderer base;
    
    f32 render_width;
    f32 render_height;
    
    // NOTE(yuval): All-Purpose VAO
    GLuint all_purpose_vao;
    
    // NOTE(yuval): Opengl Filled Rectangle Data
    GLuint filled_rect_vao;
    GLuint filled_rect_instance_buffer;
    
    // NOTE(yuval): Framebuffers
    Opengl_Fbo* active_fbo;
    Opengl_Fbo main_fbo;
    Opengl_Fbo main_2d_fbo;
    Opengl_Fbo world_fbo;
    Opengl_Fbo screen_size_scratch_fbo;
    Opengl_Fbo shadow_map_fbo;
    
    // NOTE(yuval): Shaders
    Opengl_World_Program world_program;
    Opengl_Fxaa_Program fxaa_program;
    Opengl_Filled_Rect_Program filled_rect_program;
};