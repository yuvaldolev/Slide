#if !defined(SLIDE_H)

#include <ft2build.h>
#include FT_FREETYPE_H

#include "yd/yd_context.h"
#include "yd/yd_memory.h"
#include "yd/yd_string.h"
#include "yd/yd_hotloader.h"
#include "yd/yd_input.h"

#include "slide_platform.h"
#include "slide_assert.h"
#include "slide_intrinsics.h"
#include "slide_math.h"

#include "slide_renderer.h"
#include "slide_fonts.h"

#define DLIST_INIT(sentinel) { \
    (sentinel)->next = (sentinel);\
    (sentinel)->prev = (sentinel); \
}

#define DLIST_IS_EMPTY(sentinel) \
((sentinel)->next == (sentinel))

#define DLIST_INSERT(sentinel, element) { \
    (element)->next = (sentinel)->next; \
    (element)->prev = (sentinel); \
    (element)->next->prev = (element); \
    (element)->prev->next = (element); \
}

#define DLIST_INSERT_BACK(sentinel, element) { \
    (element)->next = (sentinel); \
    (element)->prev = (sentinel)->prev; \
    (element)->next->prev = (element); \
    (element)->prev->next = (element); \
}

#define DLIST_REMOVE(element) { \
    if (element->next) { \
        (element)->next->prev = (element)->prev; \
        (element)->prev->next = (element)->next; \
        (element)->next = (element)->prev = 0; \
    } \
}

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
    
    Slide_Item* next;
};

struct Slide_Link {
    Slide_Link* prev;
    Slide_Link* next;
};

struct Slide {
    Slide_Link link;
    
    Vector4 background_color;
    Slide_Item* first_item;
};

struct Slideshow {
    Slide_Link slide_sentinel;
    
    Slide* current_slide;
};

struct App_State {
    Memory_Arena arena;
    
    Slideshow slideshow;
};

#define SLIDE_H
#endif // #if !defined(SLIDE_H)