#include <Cocoa/Cocoa.h>

#include "slide.cpp"

#include <dlfcn.h>

internal PLATFORM_DISPLAY_MESSAGE_BOX(mac_display_message_box);
#include "mac_slide_renderer.h"

#include <stdlib.h>
#include <sys/stat.h>

global b32 global_running;
global f32 global_aspect_ratio;

@interface App_Delegate : NSObject<NSApplicationDelegate, NSWindowDelegate>
@end

@implementation App_Delegate
- (void)applicationDidFinishLaunching:(id)sender {
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
    return YES;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
}

- (NSSize)windowWillResize:(NSWindow*)window toSize:(NSSize)frame_size {
    frame_size.height = ((f32)frame_size.width / global_aspect_ratio);
    return frame_size;
}

- (void)windowWillClose:(id)sender {
    global_running = false;
}
@end

internal PLATFORM_DISPLAY_MESSAGE_BOX(mac_display_message_box) {
    NSAlert* alert = [[[NSAlert alloc] init] autorelease];
    
    NSString* title_string = [NSString stringWithUTF8String:title];
    NSString* message_string = [NSString stringWithUTF8String:message];
    [alert setMessageText:title_string];
    [alert setInformativeText:message_string];
    
    [alert runModal];
}

internal PLATFORM_FREE_FILE_MEMORY(mac_free_file_memory) {
    if (memory) {
        free(memory);
    }
}

internal PLATFORM_READ_ENTIRE_FILE(mac_read_entire_file) {
    Read_File_Result result = {};
    
    s32 file_handle = open(filename, O_RDONLY);
    if (file_handle != -1) {
        struct stat file_stat;
        if (fstat(file_handle, &file_stat) == 0) {
            u32 file_size_32 = safe_truncate_to_u32(file_stat.st_size);
            
            result.contents = malloc(file_size_32);
            if (result.contents) {
                umm bytes_read = read(file_handle, result.contents, file_size_32);
                if (bytes_read == file_size_32) {
                    result.contents_size = file_size_32;
                } else {
                    mac_free_file_memory(result.contents);
                    result.contents = 0;
                }
            }
        }
        
        close(file_handle);
    }
    
    return result;
}

internal PLATFORM_WRITE_ENTIRE_FILE(mac_write_entire_file) {
    b32 result = false;
    
    s32 file_handle = open(filename, O_WRONLY | O_CREAT, 0644);
    if (file_handle != -1) {
        umm bytes_written = write(file_handle, memory, memory_size);
        result = (bytes_written == memory_size);
        
        close(file_handle);
    }
    
    return result;
}

internal Vector2u
mac_get_window_dimensions(NSWindow* window) {
    NSRect frame = [window frame];
    Vector2u result = {
        (u32)frame.size.width,
        (u32)frame.size.height
    };
    
    return result;
}

int
main(int arg_count, char** args) {
    @autoreleasepool {
        // NOTE(yuval): NSApplication & Delegate Creation
        NSApplication* ns_app = [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
        App_Delegate* app_delegate = [[App_Delegate alloc] init];
        [ns_app setDelegate:app_delegate];
        
        [NSApp finishLaunching];
        
        // NOTE(yuval): NSWindow Creation
        const s32 INITIAL_WINDOW_WIDTH = 1280;
        const s32 INITIAL_WINDOW_HEIGHT = 720;
        
        global_aspect_ratio = ((f32)INITIAL_WINDOW_WIDTH / (f32)INITIAL_WINDOW_HEIGHT);
        
        NSRect screen_rect = [[NSScreen mainScreen] frame];
        NSRect initial_frame = NSMakeRect(
            (screen_rect.size.width - INITIAL_WINDOW_WIDTH) * 0.5f,
            (screen_rect.size.height - INITIAL_WINDOW_HEIGHT) * 0.5f,
            INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
        
        NSWindow* window = [[NSWindow alloc] initWithContentRect:initial_frame
                styleMask:NSWindowStyleMaskTitled |
                NSWindowStyleMaskClosable |
                NSWindowStyleMaskMiniaturizable |
                NSWindowStyleMaskResizable
                backing:NSBackingStoreBuffered
                defer:NO];
        
        [window setBackgroundColor:NSColor.blackColor];
        [window setDelegate:app_delegate];
        [window setTitle:@"Slide"];
        [window makeKeyAndOrderFront:nil];
        
        Application app = {};
        
        app.platform_api.display_message_box = mac_display_message_box;
        app.platform_api.free_file_memory = mac_free_file_memory;
        app.platform_api.read_entire_file = mac_read_entire_file;
        app.platform_api.write_entire_file = mac_write_entire_file;
        
        Platform_Renderer_Limits limits;
        limits.max_quad_count_per_frame = (1 << 18);
        limits.max_texture_handle_count = 256;
        
        Platform_Renderer* renderer =
            mac_init_default_renderer(window, &limits);
        
        global_running = true;
        while (global_running) {
            Vector2u dimensions = mac_get_window_dimensions(window);
            
            Render_Commands* frame =
                renderer->begin_frame(renderer, dimensions);
            
            NSEvent* event;
            do {
                event = [NSApp nextEventMatchingMask:NSEventMaskAny
                        untilDate:nil
                        inMode:NSDefaultRunLoopMode
                        dequeue:YES];
                
                [NSApp sendEvent:event];
            } while (event != nil);
            
            update_and_render(&app, frame);
            
            renderer->end_frame(renderer, frame);
        }
    }
}