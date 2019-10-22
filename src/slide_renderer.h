#if !defined(SLIDE_RENDERER_H)

struct Renderer;

#define RENDERER_BEGIN_FRAME(name) void name(Renderer* renderer, f32 render_width, f32 render_height)
typedef RENDERER_BEGIN_FRAME(Renderer_Begin_Frame);

#define RENDERER_END_FRAME(name) void name(Renderer* renderer)
typedef RENDERER_END_FRAME(Renderer_End_Frame);

#pragma pack(push, 1)
struct Renderer_Filled_Rect {
    Vector3 p1;
    Vector3 p2;
    Vector3 p3;
    Vector3 p4;
    
    Vector4 c1;
    Vector4 c2;
    Vector4 c3;
    Vector4 c4;
};
#pragma pack(pop)

namespace Render_Flags {
    enum Type {
        FLIP_HORIZONTAL = 0x1,
        FLIP_VERTICAL = 0x2,
        ADDITIVE_BLEND = 0x4,
        TEXT_ALIGN_CENTER_X = 0x8,
        TEXT_ALIGN_CENTER_Y = 0x10,
        TEXT_ALIGN_RIGHT = 0x20,
        RENDER_2D = 0x40,
        NO_SHADOW = 0x80,
        NO_DEPTH_WRITE = 0x100
    };
}

namespace Render_Request_Kind {
    enum Type {
        NONE,
        
        FILLED_RECT
    };
}

struct Render_Request {
    Render_Request_Kind::Type kind;
    
    u32 data_offset;
    u32 data_size;
    u32 flags;
};

struct Renderer {
    Renderer_Begin_Frame* begin_frame;
    Renderer_End_Frame* end_frame;
    
    Render_Request requests[65536];
    umm request_count;
    
    Render_Request active_request;
    
    // NOTE(yuval): Filled Rectangle Data
    u32 max_filled_rects;
    u32 max_filled_rect_bytes;
    u8* filled_rect_instance_data;
    u32 filled_rect_instance_data_alloc_pos;
};

struct Renderer_Limits {
    u32 max_filled_rects;
};

internal void
renderer_finish_active_request(Renderer* renderer) {
    if (renderer->active_request.kind != Render_Request_Kind::NONE) {
        ASSERT(renderer->request_count < sizeof(renderer->requests));
        renderer->requests[renderer->request_count++] =
            renderer->active_request;
        
        renderer->active_request.kind = Render_Request_Kind::NONE;
    }
}

#define SLIDE_RENDERER_H
#endif // #if !defined(SLIDE_RENDERER_H)