internal void
push_filled_rect_(Renderer* renderer,
                  Vector3 p1, Vector4 c1, Vector3 p2, Vector4 c2,
                  Vector3 p3, Vector4 c3, Vector3 p4, Vector4 c4,
                  u32 flags = 0) {
    ASSERT(renderer->filled_rect_instance_data_alloc_pos + sizeof(Renderer_Filled_Rect) <=
           renderer->max_filled_rect_bytes);
    
    Render_Request_Kind::Type request_kind = Render_Request_Kind::FILLED_RECT;
    if ((renderer->active_request.kind != request_kind) ||
        (flags != renderer->active_request.flags)) {
        renderer_finish_active_request(renderer);
        
        renderer->active_request.kind = request_kind;
        renderer->active_request.data_offset =
            renderer->filled_rect_instance_data_alloc_pos;
        renderer->active_request.data_size = sizeof(Renderer_Filled_Rect);
        renderer->active_request.flags = flags;
    } else {
        renderer->active_request.data_size += sizeof(Renderer_Filled_Rect);
    }
    
    Renderer_Filled_Rect* filled_rect = (Renderer_Filled_Rect*)(renderer->filled_rect_instance_data +
                                                                renderer->filled_rect_instance_data_alloc_pos);
    filled_rect->p1 = p1;
    filled_rect->p2 = p2;
    filled_rect->p3 = p3;
    filled_rect->p4 = p4;
    filled_rect->c1 = c1;
    filled_rect->c2 = c2;
    filled_rect->c3 = c3;
    filled_rect->c4 = c4;
    
    renderer->filled_rect_instance_data_alloc_pos +=
        sizeof(Renderer_Filled_Rect);
}

// TODO(yuval): Use Rectangle3
inline void
push_filled_rect(Renderer* renderer,
                 Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4,
                 Vector4 color) {
    push_filled_rect_(renderer, p1, color, p2, color,
                      p3, color, p4, color);
}

inline void
push_filled_rect(Renderer* renderer, Rectangle2 rect,
                 Vector4 c1, Vector4 c2,
                 Vector4 c3, Vector4 c4) {
    Vector3 p1 = {rect.min.x, rect.min.y, 0};
    Vector3 p2 = {rect.min.x, rect.max.y, 0};
    Vector3 p3 = {rect.max.x, rect.min.y, 0};
    Vector3 p4 = {rect.max.x, rect.max.y, 0};
    
    push_filled_rect_(renderer, p1, c1, p2, c2, p3, c3, p4, c4,
                      Render_Flags::RENDER_2D);
}

inline void
push_filled_rect(Renderer* renderer, Rectangle2 rect, Vector4 color) {
    push_filled_rect(renderer, rect, color, color, color, color);
}
