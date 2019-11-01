#if !defined(SLIDE_FONTS_H)

struct Font_Glyph {
    Renderer_Texture texture;
    
    Vector2 bearing;
    Vector2 advance;
};

struct Font {
    const char* name;
    
    FT_Face face;
    u32 size;
    
    Font_Glyph glyphs[128];
};

// TODO(yuval): Convert this to a hash table
struct Loaded_Font {
    Font font;
    Loaded_Font* next;
};

#define SLIDE_FONTS_H
#endif // #if !defined(SLIDE_FONTS_H)