#if !defined(SLIDE_H)

#include <ft2build.h>
#include FT_FREETYPE_H

#include "yd/yd_memory.h"
#include "yd/yd_context.h"
#include "yd/yd_string.h"
#include "yd/yd_input.h"

#include "slide_platform.h"
#include "slide_assert.h"
#include "slide_intrinsics.h"
#include "slide_math.h"

#include "slide_renderer.h"
#include "slide_fonts.h"

namespace Slide_Item_Kind {
    enum Type {
        TEXT
    };
}

struct Text_Item {
    String text;
    Vector4 color;
};

struct Slide_Item {
    Slide_Item_Kind::Type kind;
    
    Vector2 pos;
    
    union {
        Text_Item text;
    };
    
    Slide_Item* next_item;
};

struct Slide {
    Vector4 background_color;
    Slide_Item* first_item;
    
    // TODO(yuval): Make a proper doubly linked list
    Slide* prev_slide;
    Slide* next_slide;
};

struct Slideshow {
    Slide* first_slide;
    Slide* current_slide;
};

struct App_State {
    Memory_Arena arena;
    
    Slideshow slideshow;
};

#define SLIDE_H
#endif // #if !defined(SLIDE_H)