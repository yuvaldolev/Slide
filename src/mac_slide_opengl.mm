#include <Cocoa/Cocoa.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define GL_GLEXT_LEGACY
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_STRING_FORMAT_IMPLEMENTATION
#include "yd/yd_string_format.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

// TODO(yuval): Temporary! Find a way to use slide_assert.h
// in the opengl renderer
#define ASSERT(expression) if(!(expression)) {*(volatile int *)0 = 0;}

#include "slide_platform.h"
#include "slide_types.h"
#include "slide_intrinsics.h"
#include "slide_math.h"
#include "slide_math.cpp"
#include "slide_renderer.h"

#include <dlfcn.h>

#include "mac_slide_renderer.h"

typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

typedef void Type_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void Type_glBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void Type_glGenFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void Type_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum Type_glCheckFramebufferStatus(GLenum target);
typedef void Type_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

typedef void Type_glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void Type_glBindVertexArray(GLuint array);
typedef void Type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void Type_glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers);

typedef void Type_glDrawElementsBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);

#define GL_DEBUG_CALLBACK(name) void name(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
typedef GL_DEBUG_CALLBACK(GLDEBUGPROC);
typedef void Type_glDebugMessageCallbackARB(GLDEBUGPROC *callback, const void *userParam);

typedef const GLubyte * Type_glGetStringi(GLenum name, GLuint index);

#define OPENGL_GLOBAL_FUNCTION(name) global Type_##name* name;

OPENGL_GLOBAL_FUNCTION(glTexImage2DMultisample);
OPENGL_GLOBAL_FUNCTION(glBindFramebuffer);
OPENGL_GLOBAL_FUNCTION(glGenFramebuffers);
OPENGL_GLOBAL_FUNCTION(glFramebufferTexture2D);
OPENGL_GLOBAL_FUNCTION(glCheckFramebufferStatus);
OPENGL_GLOBAL_FUNCTION(glBlitFramebuffer);

OPENGL_GLOBAL_FUNCTION(glVertexAttribIPointer);
OPENGL_GLOBAL_FUNCTION(glDebugMessageCallbackARB);
OPENGL_GLOBAL_FUNCTION(glBindVertexArray);
OPENGL_GLOBAL_FUNCTION(glGenVertexArrays);
OPENGL_GLOBAL_FUNCTION(glGetStringi);
OPENGL_GLOBAL_FUNCTION(glDeleteFramebuffers);

OPENGL_GLOBAL_FUNCTION(glDrawElementsBaseVertex);

#include "slide_renderer_opengl.h"
#include "slide_renderer_opengl.cpp"

global NSOpenGLContext* global_opengl_context;

@interface Opengl_View : NSOpenGLView
@end

@implementation Opengl_View
- (id)init {
    self = [super init];
    return self;
}

- (void)prepareOpenGL {
    [super prepareOpenGL];
    [[self openGLContext] makeCurrentContext];
}

- (void)reshape {
    [super reshape];
    
    NSRect bounds = [self bounds];
    [global_opengl_context makeCurrentContext];
    [global_opengl_context update];
    glViewport(0, 0, (GLsizei)bounds.size.width,
               (GLsizei)bounds.size.height);
}
@end

internal void*
mac_renderer_alloc(umm size) {
    void* result = (void*)mmap(0, size, PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANON, -1, 0);
    
    return result;
}

RENDERER_BEGIN_FRAME(mac_opengl_begin_frame) {
    [global_opengl_context makeCurrentContext];
    
    Render_Commands* result =
        opengl_begin_frame((Opengl*)renderer, render_dim);
    return result;
}

RENDERER_END_FRAME(mac_opengl_end_frame) {
    opengl_end_frame((Opengl*)renderer, frame);
    [global_opengl_context flushBuffer];
}

RENDERER_ALLOCATE_TEXTURE(mac_opengl_allocate_texture) {
    Renderer_Texture result =
        opengl_allocate_texture((Opengl*)renderer, width, height, data);
    return result;
}

internal Opengl*
mac_init_opengl(NSWindow* window, Platform_Renderer_Limits* limits) {
    // NOTE(yuval): Pixel Format Choice
    NSOpenGLPixelFormatAttribute opengl_attrib_list[] = {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
        NSOpenGLPFASampleBuffers, 1,
        NSOpenGLPFASamples, 16,
        0
    };
    NSOpenGLPixelFormat* pixel_format = [[NSOpenGLPixelFormat alloc]
            initWithAttributes:opengl_attrib_list];
    
    // NOTE(yuval): Opengl Context Creation
    NSView* content_view = [window contentView];
    
    global_opengl_context = [[NSOpenGLContext alloc] initWithFormat:pixel_format
            shareContext:0];
    
    GLint swap_int = 1; // NOTE(yuval): Using vsync
    [global_opengl_context setValues:&swap_int
            forParameter:NSOpenGLCPSwapInterval];
    
    [global_opengl_context setView:content_view];
    [global_opengl_context makeCurrentContext];
    
    // NOTE(yuval): Opengl_View Creation
    Opengl_View* opengl_view = [[Opengl_View alloc] init];
    [opengl_view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    [opengl_view setPixelFormat:pixel_format];
    [opengl_view setOpenGLContext:global_opengl_context];
    [opengl_view setFrame:[content_view bounds]];
    
    [content_view addSubview:opengl_view];
    
    [pixel_format release];
    
    // NOTE(yuval): Opengl Renderer Creation
    Opengl* opengl = (Opengl*)mac_renderer_alloc(sizeof(Opengl));
    
    opengl->header.begin_frame = mac_opengl_begin_frame;
    opengl->header.end_frame = mac_opengl_end_frame;
    opengl->header.allocate_texture = mac_opengl_allocate_texture;
    
    u32 max_vertex_count = limits->max_quad_count_per_frame * 4;
    opengl->max_vertex_count = max_vertex_count;
    
    opengl->max_quad_texture_count = limits->max_quad_count_per_frame;
    
    opengl->vertex_array = (Textured_Vertex*)
        mac_renderer_alloc(max_vertex_count * sizeof(Textured_Vertex));
    opengl->bitmap_array = (Renderer_Texture*)
        mac_renderer_alloc(opengl->max_quad_texture_count *
                           sizeof(Renderer_Texture));
    
    // OpenGL Extension Loading
    void* image = dlopen(
        "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL",
        RTLD_LAZY);
    
#define MAC_GET_OPENGL_FUNCTION(name) name = (Type_##name*)dlsym(image, #name)
    MAC_GET_OPENGL_FUNCTION(glTexImage2DMultisample);
    MAC_GET_OPENGL_FUNCTION(glBindFramebuffer);
    MAC_GET_OPENGL_FUNCTION(glGenFramebuffers);
    MAC_GET_OPENGL_FUNCTION(glFramebufferTexture2D);
    MAC_GET_OPENGL_FUNCTION(glCheckFramebufferStatus);
    MAC_GET_OPENGL_FUNCTION(glBlitFramebuffer);
    
    MAC_GET_OPENGL_FUNCTION(glVertexAttribIPointer);
    MAC_GET_OPENGL_FUNCTION(glBindVertexArray);
    MAC_GET_OPENGL_FUNCTION(glGenVertexArrays);
    MAC_GET_OPENGL_FUNCTION(glGetStringi);
    MAC_GET_OPENGL_FUNCTION(glDeleteFramebuffers);
    
    MAC_GET_OPENGL_FUNCTION(glDrawElementsBaseVertex);
    
    MAC_GET_OPENGL_FUNCTION(glDebugMessageCallbackARB);
    
    // OpenGL Renderer Initialization
    Opengl_Info info = opengl_get_info(true);
    opengl_init(opengl, info);
    
    return opengl;
}

extern "C" MAC_LOAD_RENDERER_ENTRY() {
    Platform_Renderer* result = (Platform_Renderer*)
        mac_init_opengl(window, limits);
    
    return result;
}