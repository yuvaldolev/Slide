#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <tchar.h>

#define YD_STRING_IMPLEMENTATION
#include "yd/yd_string.h"

#define YD_STRING_FORMAT_IMPLEMENTATION
#include "yd/yd_string_format.h"

#define YD_MEMORY_IMPLEMENTATION
#include "yd/yd_memory.h"

// TODO(yuval): Temporary! Find a way to use slide_assert.h
// in the opengl renderer
#define ASSERT(expression) if(!(expression)) {*(volatile int *)0 = 0;}

#include "slide_types.h"
#include "slide_intrinsics.h"
#include "slide_math.h"
#include "slide_math.cpp"
#include "slide_renderer.h"
#include "win32_slide_renderer.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB              0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB              0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB                0x2093
#define WGL_CONTEXT_FLAGS_ARB                      0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB               0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB                  0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB     0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB           0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB  0x00000002

#define WGL_DRAW_TO_WINDOW_ARB                     0x2001
#define WGL_ACCELERATION_ARB                       0x2003
#define WGL_SUPPORT_OPENGL_ARB                     0x2010
#define WGL_DOUBLE_BUFFER_ARB                      0x2011
#define WGL_PIXEL_TYPE_ARB                         0x2013

#define WGL_TYPE_RGBA_ARB                          0x202B
#define WGL_FULL_ACCELERATION_ARB                  0x2027

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB           0x20A9

#define WGL_RED_BITS_ARB                           0x2015
#define WGL_GREEN_BITS_ARB                         0x2017
#define WGL_BLUE_BITS_ARB                          0x2019
#define WGL_ALPHA_BITS_ARB                         0x201B
#define WGL_DEPTH_BITS_ARB                         0x2022
#define WGL_COLOR_BITS_ARB                         0x2014
#define WGL_STENCIL_BITS_ARB                       0x2023

typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

typedef void WINAPI Type_glTexImage2DMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void WINAPI Type_glBindFramebuffer(GLenum target, GLuint framebuffer);
typedef void WINAPI Type_glGenFramebuffers(GLsizei n, GLuint *framebuffers);
typedef void WINAPI Type_glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLenum WINAPI Type_glCheckFramebufferStatus(GLenum target);
typedef void WINAPI Type_glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void WINAPI Type_glAttachShader(GLuint program, GLuint shader);
typedef void WINAPI Type_glCompileShader(GLuint shader);
typedef GLuint WINAPI Type_glCreateProgram(void);
typedef GLuint WINAPI Type_glCreateShader(GLenum type);
typedef void WINAPI Type_glLinkProgram(GLuint program);
typedef void WINAPI Type_glShaderSource(GLuint shader, GLsizei count, GLchar **string, GLint *length);
typedef void WINAPI Type_glUseProgram(GLuint program);
typedef void WINAPI Type_glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void WINAPI Type_glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void WINAPI Type_glValidateProgram(GLuint program);
typedef void WINAPI Type_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef GLint WINAPI Type_glGetUniformLocation (GLuint program, const GLchar *name);
typedef void WINAPI Type_glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI Type_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void WINAPI Type_glUniform1i(GLint location, GLint v0);

typedef void WINAPI Type_glUniform1f(GLint location, GLfloat v0);
typedef void WINAPI Type_glUniform2f(GLint location, GLfloat v0, GLfloat v1);
typedef void WINAPI Type_glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI Type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);

typedef void WINAPI Type_glEnableVertexAttribArray(GLuint index);
typedef void WINAPI Type_glDisableVertexAttribArray(GLuint index);
typedef GLint WINAPI Type_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void WINAPI Type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void WINAPI Type_glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void WINAPI Type_glVertexAttribDivisor(GLuint index, GLuint divisor);
typedef void WINAPI Type_glBindVertexArray(GLuint array);
typedef void WINAPI Type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void WINAPI Type_glBindBuffer (GLenum target, GLuint buffer);
typedef void WINAPI Type_glGenBuffers (GLsizei n, GLuint *buffers);
typedef void WINAPI Type_glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void WINAPI Type_glActiveTexture (GLenum texture);
typedef void WINAPI Type_glDeleteProgram (GLuint program);
typedef void WINAPI Type_glDeleteShader (GLuint shader);
typedef void WINAPI Type_glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers);
typedef void WINAPI Type_glDrawBuffers (GLsizei n, const GLenum *bufs);
typedef void WINAPI Type_glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void WINAPI Type_glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);

typedef void WINAPI Type_glDrawElementsBaseVertex (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);

typedef void WINAPI Type_glBindAttribLocation(GLuint program, GLuint index, const GLchar *name);
typedef void WINAPI Type_glBindFragDataLocation(GLuint program, GLuint color, const GLchar *name);
typedef void WINAPI Type_glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void WINAPI Type_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void WINAPI Type_glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);

#define GL_DEBUG_CALLBACK(name) void WINAPI name(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
typedef GL_DEBUG_CALLBACK(GLDEBUGPROC);
typedef void WINAPI Type_glDebugMessageCallbackARB(GLDEBUGPROC *callback, const void *userParam);

typedef const GLubyte * WINAPI Type_glGetStringi(GLenum name, GLuint index);

#define OPENGL_GLOBAL_FUNCTION(name) global Type_##name* name;

OPENGL_GLOBAL_FUNCTION(glTexImage2DMultisample);
OPENGL_GLOBAL_FUNCTION(glBindFramebuffer);
OPENGL_GLOBAL_FUNCTION(glGenFramebuffers);
OPENGL_GLOBAL_FUNCTION(glFramebufferTexture2D);
OPENGL_GLOBAL_FUNCTION(glCheckFramebufferStatus);
OPENGL_GLOBAL_FUNCTION(glBlitFramebuffer);
OPENGL_GLOBAL_FUNCTION(glAttachShader);
OPENGL_GLOBAL_FUNCTION(glCompileShader);
OPENGL_GLOBAL_FUNCTION(glCreateProgram);
OPENGL_GLOBAL_FUNCTION(glCreateShader);
OPENGL_GLOBAL_FUNCTION(glLinkProgram);
OPENGL_GLOBAL_FUNCTION(glShaderSource);
OPENGL_GLOBAL_FUNCTION(glUseProgram);
OPENGL_GLOBAL_FUNCTION(glGetProgramInfoLog);
OPENGL_GLOBAL_FUNCTION(glGetShaderInfoLog);
OPENGL_GLOBAL_FUNCTION(glValidateProgram);
OPENGL_GLOBAL_FUNCTION(glGetProgramiv);
OPENGL_GLOBAL_FUNCTION(glGetUniformLocation);
OPENGL_GLOBAL_FUNCTION(glUniform4fv);
OPENGL_GLOBAL_FUNCTION(glUniformMatrix4fv);
OPENGL_GLOBAL_FUNCTION(glUniform1i);

OPENGL_GLOBAL_FUNCTION(glUniform1f);
OPENGL_GLOBAL_FUNCTION(glUniform2f);
OPENGL_GLOBAL_FUNCTION(glUniform2fv);
OPENGL_GLOBAL_FUNCTION(glUniform3fv);
OPENGL_GLOBAL_FUNCTION(glEnableVertexAttribArray);
OPENGL_GLOBAL_FUNCTION(glDisableVertexAttribArray);
OPENGL_GLOBAL_FUNCTION(glGetAttribLocation);
OPENGL_GLOBAL_FUNCTION(glVertexAttribPointer);
OPENGL_GLOBAL_FUNCTION(glVertexAttribIPointer);
OPENGL_GLOBAL_FUNCTION(glVertexAttribDivisor);
OPENGL_GLOBAL_FUNCTION(glDebugMessageCallbackARB);
OPENGL_GLOBAL_FUNCTION(glBindVertexArray);
OPENGL_GLOBAL_FUNCTION(glGenVertexArrays);
OPENGL_GLOBAL_FUNCTION(glBindBuffer);
OPENGL_GLOBAL_FUNCTION(glGenBuffers);
OPENGL_GLOBAL_FUNCTION(glBufferData);
OPENGL_GLOBAL_FUNCTION(glActiveTexture);
OPENGL_GLOBAL_FUNCTION(glGetStringi);
OPENGL_GLOBAL_FUNCTION(glDeleteProgram);
OPENGL_GLOBAL_FUNCTION(glDeleteShader);
OPENGL_GLOBAL_FUNCTION(glDeleteFramebuffers);
OPENGL_GLOBAL_FUNCTION(glDrawBuffers);
OPENGL_GLOBAL_FUNCTION(glTexImage3D);
OPENGL_GLOBAL_FUNCTION(glTexSubImage3D);
OPENGL_GLOBAL_FUNCTION(glDrawElementsBaseVertex);

OPENGL_GLOBAL_FUNCTION(glBindAttribLocation);
OPENGL_GLOBAL_FUNCTION(glBindFragDataLocation);
OPENGL_GLOBAL_FUNCTION(glBlendFuncSeparate);
OPENGL_GLOBAL_FUNCTION(glBufferSubData);
OPENGL_GLOBAL_FUNCTION(glDrawArraysInstanced);

typedef HGLRC WINAPI Wgl_Create_Context_Attribs_Arb(HDC hDC, HGLRC hShareContext,
                                                    const int *attribList);

typedef BOOL WINAPI Wgl_Choose_Pixel_Format_Arb(HDC hdc,
                                                const int *piAttribIList,
                                                const FLOAT *pfAttribFList,
                                                UINT nMaxFormats,
                                                int *piFormats,
                                                UINT *nNumFormats);



typedef BOOL WINAPI Wgl_Swap_Interval_Ext(int interval);

global Wgl_Create_Context_Attribs_Arb* wglCreateContextAttribsARB;
global Wgl_Choose_Pixel_Format_Arb* wglChoosePixelFormatARB;
global Wgl_Swap_Interval_Ext* wglSwapIntervalEXT;

#include "slide_renderer_opengl.h"
#include "slide_renderer_opengl.cpp"

global HGLRC global_win32_opengl_context;

internal void
win32_load_wgl_extensions() {
    wglCreateContextAttribsARB = (Wgl_Create_Context_Attribs_Arb*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (Wgl_Choose_Pixel_Format_Arb*)wglGetProcAddress("wglChoosePixelFormatARB");
    wglSwapIntervalEXT = (Wgl_Swap_Interval_Ext*)wglGetProcAddress("wglSwapIntervalEXT");
}

internal void*
win32_renderer_alloc(umm size) {
    void* result = VirtualAlloc(0, size,
                                MEM_RESERVE | MEM_COMMIT,
                                PAGE_READWRITE);
    
    return result;
}

RENDERER_BEGIN_FRAME(win32_opengl_begin_frame) {
    opengl_begin_frame((Renderer_Opengl*)renderer, render_width, render_height);
}

RENDERER_END_FRAME(win32_opengl_end_frame) {
    opengl_end_frame((Renderer_Opengl*)renderer);
    SwapBuffers(wglGetCurrentDC());
}

internal Renderer_Opengl*
win32_init_opengl(HDC window_dc, Renderer_Limits* limits) {
    Renderer_Opengl* opengl = (Renderer_Opengl*)
        win32_renderer_alloc(sizeof(Renderer_Opengl));
    
    // NOTE(yuval): Renderer Base Initialization
    Renderer* renderer = &opengl->base;
    renderer->begin_frame = win32_opengl_begin_frame;
    renderer->end_frame = win32_opengl_end_frame;
    renderer->max_filled_rects = limits->max_filled_rects;
    renderer->max_filled_rect_bytes = renderer->max_filled_rects * sizeof(Renderer_Filled_Rect);
    renderer->filled_rect_instance_data =
        (u8*)win32_renderer_alloc(renderer->max_filled_rect_bytes);
    
    // NOTE(yuval): Setting up pixel format for dummy context
    int pixel_format = 0;
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    
    pixel_format = ChoosePixelFormat(window_dc, &pfd);
    
    if (pixel_format) {
        SetPixelFormat(window_dc, pixel_format, &pfd);
        HGLRC gl_dummy_render_context = wglCreateContext(window_dc);
        
        if (wglMakeCurrent(window_dc, gl_dummy_render_context)) {
            win32_load_wgl_extensions();
            
            // NOTE(yuval): Opengl Extension Loading
#define WIN32_GET_OPENGL_FUNCTION(name) name = (Type_##name*)wglGetProcAddress(#name)
            
            WIN32_GET_OPENGL_FUNCTION(glEnableVertexAttribArray);
            WIN32_GET_OPENGL_FUNCTION(glDisableVertexAttribArray);
            WIN32_GET_OPENGL_FUNCTION(glGetAttribLocation);
            WIN32_GET_OPENGL_FUNCTION(glVertexAttribPointer);
            WIN32_GET_OPENGL_FUNCTION(glVertexAttribIPointer);
            WIN32_GET_OPENGL_FUNCTION(glVertexAttribDivisor);
            WIN32_GET_OPENGL_FUNCTION(glDebugMessageCallbackARB);
            WIN32_GET_OPENGL_FUNCTION(glBindVertexArray);
            WIN32_GET_OPENGL_FUNCTION(glGenVertexArrays);
            WIN32_GET_OPENGL_FUNCTION(glBindBuffer);
            WIN32_GET_OPENGL_FUNCTION(glGenBuffers);
            WIN32_GET_OPENGL_FUNCTION(glBufferData);
            WIN32_GET_OPENGL_FUNCTION(glActiveTexture);
            WIN32_GET_OPENGL_FUNCTION(glGetStringi);
            WIN32_GET_OPENGL_FUNCTION(glDeleteProgram);
            WIN32_GET_OPENGL_FUNCTION(glDeleteShader);
            WIN32_GET_OPENGL_FUNCTION(glDeleteFramebuffers);
            WIN32_GET_OPENGL_FUNCTION(glDrawBuffers);
            WIN32_GET_OPENGL_FUNCTION(glTexImage3D);
            WIN32_GET_OPENGL_FUNCTION(glTexSubImage3D);
            WIN32_GET_OPENGL_FUNCTION(glDrawElementsBaseVertex);
            
            WIN32_GET_OPENGL_FUNCTION(glUniform1f);
            WIN32_GET_OPENGL_FUNCTION(glUniform2f);
            WIN32_GET_OPENGL_FUNCTION(glUniform2fv);
            WIN32_GET_OPENGL_FUNCTION(glUniform3fv);
            WIN32_GET_OPENGL_FUNCTION(glBindFramebuffer);
            WIN32_GET_OPENGL_FUNCTION(glGenFramebuffers);
            WIN32_GET_OPENGL_FUNCTION(glFramebufferTexture2D);
            WIN32_GET_OPENGL_FUNCTION(glCheckFramebufferStatus);
            
            WIN32_GET_OPENGL_FUNCTION(glTexImage2DMultisample);
            WIN32_GET_OPENGL_FUNCTION(glBlitFramebuffer);
            
            WIN32_GET_OPENGL_FUNCTION(glAttachShader);
            WIN32_GET_OPENGL_FUNCTION(glCompileShader);
            WIN32_GET_OPENGL_FUNCTION(glCreateProgram);
            WIN32_GET_OPENGL_FUNCTION(glCreateShader);
            WIN32_GET_OPENGL_FUNCTION(glLinkProgram);
            WIN32_GET_OPENGL_FUNCTION(glShaderSource);
            WIN32_GET_OPENGL_FUNCTION(glUseProgram);
            WIN32_GET_OPENGL_FUNCTION(glGetProgramInfoLog);
            WIN32_GET_OPENGL_FUNCTION(glGetShaderInfoLog);
            WIN32_GET_OPENGL_FUNCTION(glValidateProgram);
            WIN32_GET_OPENGL_FUNCTION(glGetProgramiv);
            WIN32_GET_OPENGL_FUNCTION(glGetUniformLocation);
            WIN32_GET_OPENGL_FUNCTION(glUniform4fv);
            WIN32_GET_OPENGL_FUNCTION(glUniformMatrix4fv);
            WIN32_GET_OPENGL_FUNCTION(glUniform1i);
            
            WIN32_GET_OPENGL_FUNCTION(glBindAttribLocation);
            WIN32_GET_OPENGL_FUNCTION(glBindFragDataLocation);
            WIN32_GET_OPENGL_FUNCTION(glBlendFuncSeparate);
            WIN32_GET_OPENGL_FUNCTION(glBufferSubData);
            WIN32_GET_OPENGL_FUNCTION(glDrawArraysInstanced);
            
            // NOTE(yuval): Setup real pixel format
            int pf_attribs_i[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 32,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                0
            };
            
            UINT num_formats = 0;
            wglChoosePixelFormatARB(window_dc,
                                    pf_attribs_i,
                                    0,
                                    1,
                                    &pixel_format,
                                    &num_formats);
            
            if (pixel_format) {
                const int context_attribs[] = {
                    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                    0
                };
                
                global_win32_opengl_context =
                    wglCreateContextAttribsARB(window_dc,
                                               gl_dummy_render_context,
                                               context_attribs);
                
                if(global_win32_opengl_context) {
                    wglMakeCurrent(window_dc, 0);
                    wglDeleteContext(gl_dummy_render_context);
                    wglMakeCurrent(window_dc, global_win32_opengl_context);
                    wglSwapIntervalEXT(0);
                }
            }
            
            opengl_init(opengl);
        }
    }
    
    return opengl;
}

extern "C" WIN32_LOAD_RENDERER_ENTRY() {
    Renderer* result = (Renderer*)
        win32_init_opengl(window_dc, limits);
    
    return result;
}