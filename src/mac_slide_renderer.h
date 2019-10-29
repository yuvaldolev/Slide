#if !defined(MAC_SLIDE_RENDERER_H)

#define MAC_LOAD_RENDERER(name) Platform_Renderer* name(NSWindow* window, Platform_Renderer_Limits* limits)
typedef MAC_LOAD_RENDERER(Mac_Load_Renderer);
#define MAC_LOAD_RENDERER_ENTRY(...) MAC_LOAD_RENDERER(mac_load_renderer)

internal Mac_Load_Renderer*
mac_load_renderer_dylib(const char* filename) {
    void* renderer_dylib = dlopen(filename, RTLD_LAZY | RTLD_GLOBAL);
    Mac_Load_Renderer* result = (Mac_Load_Renderer*)
        dlsym(renderer_dylib, "mac_load_renderer");
    
    return result;
}

internal Platform_Renderer*
mac_init_default_renderer(NSWindow* window,
                          Platform_Renderer_Limits* limits) {
    Mac_Load_Renderer* mac_load_opengl_renderer =
        mac_load_renderer_dylib("mac_slide_opengl.dylib");
    
    if (!mac_load_opengl_renderer) {
        NSAlert* alert = [[[NSAlert alloc] init] autorelease];
        
        NSString* title_string =  @"Unable to load mac_slide_opengl.dylib";
        NSString* message_string = @"Please make sure mac_slide_opengl.dylib is present in the run_tree directory.";
        [alert setMessageText:title_string];
        [alert setInformativeText:message_string];
        
        [alert runModal];
        
        exit(1);
    }
    
    Platform_Renderer* renderer =
        mac_load_opengl_renderer(window, limits);
    
    return renderer;
}

#define MAC_SLIDE_RENDERER_H
#endif // #if !defined(MAC_SLIDE_RENDERER_H)