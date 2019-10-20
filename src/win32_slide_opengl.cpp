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
typedef void WINAPI Type_glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
typedef void WINAPI Type_glUniform3fv(GLint location, GLsizei count, const GLfloat *value);

typedef void WINAPI Type_glEnableVertexAttribArray(GLuint index);
typedef void WINAPI Type_glDisableVertexAttribArray(GLuint index);
typedef GLint WINAPI Type_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void WINAPI Type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void WINAPI Type_glVertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
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
OPENGL_GLOBAL_FUNCTION(glUniform2fv);
OPENGL_GLOBAL_FUNCTION(glUniform3fv);
OPENGL_GLOBAL_FUNCTION(glEnableVertexAttribArray);
OPENGL_GLOBAL_FUNCTION(glDisableVertexAttribArray);
OPENGL_GLOBAL_FUNCTION(glGetAttribLocation);
OPENGL_GLOBAL_FUNCTION(glVertexAttribPointer);
OPENGL_GLOBAL_FUNCTION(glVertexAttribIPointer);
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

typedef HGLRC WINAPI Wgl_Create_Context_Attribs_Arb(HDC hDC, HGLRC hShareContext,
                                                    const int *attribList);

typedef BOOL WINAPI Wgl_Get_Pixel_Format_Attrib_Iv_Arb(HDC hdc,
                                                       int iPixelFormat,
                                                       int iLayerPlane,
                                                       UINT nAttributes,
                                                       const int *piAttributes,
                                                       int *piValues);

typedef BOOL WINAPI Wgl_Get_Pixel_Format_Attrib_Fv_Arb(HDC hdc,
                                                       int iPixelFormat,
                                                       int iLayerPlane,
                                                       UINT nAttributes,
                                                       const int *piAttributes,
                                                       FLOAT *pfValues);

typedef BOOL WINAPI Wgl_Choose_Pixel_Format_Arb(HDC hdc,
                                                const int *piAttribIList,
                                                const FLOAT *pfAttribFList,
                                                UINT nMaxFormats,
                                                int *piFormats,
                                                UINT *nNumFormats);

typedef BOOL WINAPI Wgl_Swap_Interval_Ext(int interval);
typedef const char* WINAPI Wgl_Get_Extensions_String_Ext(void);

global Wgl_Create_Context_Attribs_Arb* wglCreateContextAttribsARB;
global Wgl_Choose_Pixel_Format_Arb* wglChoosePixelFormatARB;
global Wgl_Swap_Interval_Ext* wglSwapIntervalEXT;
global Wgl_Get_Extensions_String_Ext* wglGetExtensionsStringEXT;

#include "slide_renderer_opengl.h"
#include "slide_renderer_opengl.cpp"

global int win32_opengl_attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 3,
    WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    0
};

internal void
win32_set_pixel_format(Opengl* opengl, HDC window_dc) {
    int suggested_pixel_format_index = 0;
    GLuint extended_pick = 0;
    
    if (wglChoosePixelFormatARB) {
        int int_attrib_list[] {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, // 0
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB, // 1
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE, // 2
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE, // 3
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, // 4
            WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE, // 5
            0,
        };
        
        if (!opengl->supports_srgb_framebuffer) {
            int_attrib_list[10] = 0;
        }
        
        wglChoosePixelFormatARB(window_dc, int_attrib_list, 0, 1,
                                &suggested_pixel_format_index,
                                &extended_pick);
    }
    
    if (!extended_pick) {
        PIXELFORMATDESCRIPTOR desired_pixel_format = {};
        desired_pixel_format.nSize = sizeof(desired_pixel_format);
        desired_pixel_format.nVersion = 1;
        desired_pixel_format.iPixelType = PFD_TYPE_RGBA;
        desired_pixel_format.dwFlags =
            PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        desired_pixel_format.cColorBits = 32;
        desired_pixel_format.cAlphaBits = 8;
        desired_pixel_format.cDepthBits = 24;
        desired_pixel_format.iLayerType = PFD_MAIN_PLANE;
        
        suggested_pixel_format_index =
            ChoosePixelFormat(window_dc, &desired_pixel_format);
    }
    
    PIXELFORMATDESCRIPTOR suggested_pixel_format;
    DescribePixelFormat(window_dc, suggested_pixel_format_index,
                        sizeof(suggested_pixel_format),
                        &suggested_pixel_format);
    
    SetPixelFormat(window_dc, suggested_pixel_format_index,
                   &suggested_pixel_format);
}

internal void
win32_load_wgl_extensions(Opengl* opengl) {
    WNDCLASSA window_class = {};
    
    window_class.lpfnWndProc = DefWindowProcA;
    window_class.hInstance = GetModuleHandle(0);
    window_class.lpszClassName = "SlideWGLLoader";
    
    if (RegisterClassA(&window_class)) {
        HWND window = CreateWindowExA(
            0,
            window_class.lpszClassName,
            "Slide",
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            window_class.hInstance,
            0);
        
        HDC window_dc = GetDC(window);
        win32_set_pixel_format(opengl, window_dc);
        HGLRC opengl_rc = wglCreateContext(window_dc);
        
        if (wglMakeCurrent(window_dc, opengl_rc)) {
            wglChoosePixelFormatARB =
                (Wgl_Choose_Pixel_Format_Arb*)wglGetProcAddress("wglChoosePixelFormatARB");
            wglCreateContextAttribsARB =
                (Wgl_Create_Context_Attribs_Arb*)wglGetProcAddress("wglCreateContextAttribsARB");
            wglGetExtensionsStringEXT =
                (Wgl_Get_Extensions_String_Ext*)wglGetProcAddress("wglGetExtensionsStringEXT");
            
            if (wglGetExtensionsStringEXT) {
                char* extensions = (char*)wglGetExtensionsStringEXT();
                char* at = extensions;
                
                while (*at) {
                    while (is_whitespace(*at)) {
                        ++at;
                    }
                    
                    char* end = at;
                    
                    while (*end && (!is_whitespace(*end))) {
                        ++end;
                    }
                    
                    umm count = end - at;
                    
                    if (strings_match(at, count,
                                      "WGL_EXT_framebuffer_sRGB")) {
                        opengl->supports_srgb_framebuffer = true;
                    } else if (strings_match(at, count, "WGL_ARB_framebuffer_sRGB")) {
                        opengl->supports_srgb_framebuffer = true;
                    }
                    
                    at = end;
                }
            }
            
            wglMakeCurrent(0, 0);
        }
        
        wglDeleteContext(opengl_rc);
        ReleaseDC(window, window_dc);
        DestroyWindow(window);
    }
}

internal void*
win32_renderer_alloc(umm size) {
    void* result = VirtualAlloc(0, size,
                                MEM_RESERVE | MEM_COMMIT,
                                PAGE_READWRITE);
    
    return result;
}

RENDERER_BEGIN_FRAME(win32_opengl_begin_frame) {
    Render_Commands* result =
        opengl_begin_frame((Opengl*)renderer,
                           window_width, window_height,
                           draw_region);
    
    return result;
}

RENDERER_END_FRAME(win32_opengl_end_frame) {
    opengl_end_frame((Opengl*)renderer, frame);
    SwapBuffers(wglGetCurrentDC());
}

internal Opengl*
win32_init_opengl(HDC window_dc, Platform_Renderer_Limits* limits) {
    Opengl* opengl = (Opengl*)win32_renderer_alloc(sizeof(Opengl));
    
    init_texture_queue(&opengl->header.texture_queue,
                       limits->texture_transfer_buffer_size,
                       win32_renderer_alloc(limits->texture_transfer_buffer_size));
    
    opengl->header.begin_frame = win32_opengl_begin_frame;
    opengl->header.end_frame = win32_opengl_end_frame;
    
    win32_load_wgl_extensions(opengl);
    win32_set_pixel_format(opengl, window_dc);
    
    u32 max_vertex_count = limits->max_quad_count_per_frame * 4;
    u32 max_index_count = limits->max_quad_count_per_frame * 6;
    opengl->max_vertex_count = max_vertex_count;
    opengl->max_index_count = max_index_count;
    
    opengl->max_texture_count = limits->max_texture_count;
    opengl->max_special_texture_count = limits->max_special_texture_count;
    
    opengl->max_quad_texture_count = limits->max_quad_count_per_frame;
    
    opengl->vertex_array = (Textured_Vertex*)
        win32_renderer_alloc(max_vertex_count * sizeof(Textured_Vertex));
    opengl->index_array = (u16*)
        win32_renderer_alloc(max_index_count * sizeof(u16));
    opengl->bitmap_array = (Renderer_Texture*)
        win32_renderer_alloc(opengl->max_quad_texture_count *
                             sizeof(Renderer_Texture));
    opengl->special_texture_handles = (GLuint*)
        win32_renderer_alloc(limits->max_special_texture_count *
                             sizeof(GLuint));
    
    b32 modern_context = true;
    HGLRC opengl_rc = 0;
    if (wglCreateContextAttribsARB) {
        opengl_rc = wglCreateContextAttribsARB(window_dc, 0,
                                               win32_opengl_attribs);
    }
    
    if (!opengl_rc) {
        modern_context = false;
        opengl_rc = wglCreateContext(window_dc);
    }
    
    if (wglMakeCurrent(window_dc, opengl_rc)) {
#define WIN32_GET_OPENGL_FUNCTION(name) name = (Type_##name*)wglGetProcAddress(#name)
        
        WIN32_GET_OPENGL_FUNCTION(glEnableVertexAttribArray);
        WIN32_GET_OPENGL_FUNCTION(glDisableVertexAttribArray);
        WIN32_GET_OPENGL_FUNCTION(glGetAttribLocation);
        WIN32_GET_OPENGL_FUNCTION(glVertexAttribPointer);
        WIN32_GET_OPENGL_FUNCTION(glVertexAttribIPointer);
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
        WIN32_GET_OPENGL_FUNCTION(glUniform2fv);
        WIN32_GET_OPENGL_FUNCTION(glUniform3fv);
        
        Opengl_Info info = opengl_get_info(modern_context);
        if (info.opengl_arb_framebuffer_object) {
            WIN32_GET_OPENGL_FUNCTION(glBindFramebuffer);
            WIN32_GET_OPENGL_FUNCTION(glGenFramebuffers);
            WIN32_GET_OPENGL_FUNCTION(glFramebufferTexture2D);
            WIN32_GET_OPENGL_FUNCTION(glCheckFramebufferStatus);
        }
        
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
        
        wglSwapIntervalEXT = (Wgl_Swap_Interval_Ext*)wglGetProcAddress("wglSwapIntervalEXT");
        if (wglSwapIntervalEXT) {
            wglSwapIntervalEXT(1);
        }
        
        opengl_init(opengl, info, opengl->supports_srgb_framebuffer);
    }
    
    return opengl;
}

extern "C" WIN32_LOAD_RENDERER_ENTRY() {
    Platform_Renderer* result = (Platform_Renderer*)
        win32_init_opengl(window_dc, limits);
    
    return result;
}