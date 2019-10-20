#if !defined(WIN32_SLIDE_RENDERER_H)

#define WIN32_LOAD_RENDERER(name) Platform_Renderer* name(HDC window_dc, Platform_Renderer_Limits* limits)
typedef WIN32_LOAD_RENDERER(Win32_Load_Renderer);
#define WIN32_LOAD_RENDERER_ENTRY(...) WIN32_LOAD_RENDERER(win32_load_renderer)

internal Win32_Load_Renderer*
win32_load_renderer_dll(TCHAR* filename) {
    HMODULE renderer_dll = LoadLibrary(filename);
    Win32_Load_Renderer* result = (Win32_Load_Renderer*)
        GetProcAddress(renderer_dll, "win32_load_renderer");
    
    return result;
}

internal Platform_Renderer*
win32_init_default_renderer(HWND window,
                            Platform_Renderer_Limits* limits) {
    Win32_Load_Renderer* win32_load_opengl_renderer =
        win32_load_renderer_dll(TEXT("win32_slide_opengl.dll"));
    
    if (!win32_load_opengl_renderer) {
        MessageBox(window,
                   TEXT("Please make sure win32_slide_opengl.dll is present in the run_tree directory."),
                   TEXT("Unable to load win32_slide_opengl.dll"),
                   MB_OK | MB_ICONERROR);
        ExitProcess(0);
    }
    
    Platform_Renderer* renderer =
        win32_load_opengl_renderer(GetDC(window), limits);
    
    return renderer;
}

#define WIN32_SLIDE_RENDERER_H
#endif // #if !defined(WIN32_SLIDE_RENDERER_H)