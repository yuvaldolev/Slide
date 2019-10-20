#if !defined(SLIDE_MATH_H)

union Vector2 {
    struct {
        f32 x, y;
    };
    
    struct {
        f32 u, v;
    };
    
    struct {
        f32 width, height;
    };
    
    f32 e[2];
};

union Vector2u {
    struct {
        u32 x, y;
    };
    
    struct {
        u32 width, height;
    };
    
    u32 e[2];
};

union Vector3 {
    struct {
        f32 x, y, z;
    };
    
    struct {
        f32 u, v, __;
    };
    
    struct {
        f32 r, g, b;
    };
    
    struct {
        Vector2 xy;
        f32 ignored0_;
    };
    
    struct {
        f32 ignored1_;
        Vector2 yz;
    };
    
    struct {
        Vector2 uv;
        f32 ignored2_;
    };
    
    struct {
        f32 ignored3_;
        Vector2 v__;
    };
    
    f32 e[3];
};

union Vector4 {
    struct {
        union {
            Vector3 xyz;
            
            struct {
                f32 x, y, z;
            };
        };
        
        f32 w;
    };
    
    struct {
        union {
            Vector3 rgb;
            
            struct {
                f32 r, g, b;
            };
        };
        
        f32 a;
    };
    
    struct {
        Vector2 xy;
        f32 ignored0_;
        f32 ignored1_;
    };
    
    struct {
        f32 ignored2_;
        Vector2 yz;
        f32 ignored3_;
    };
    
    struct {
        f32 ignored4_;
        f32 ignored5_;
        Vector2 zw;
    };
    
    f32 e[4];
};

struct Matrix4x4 {
    // NOTE(yuval): These are stored ROW MAJOR - e[ROW][COLUMN]!!!
    f32 e[4][4];
};

struct Matrix4x4_Inv {
    Matrix4x4 forward;
    Matrix4x4 inverse;
};

struct Rectangle2i {
    s32 min_x, min_y;
    s32 max_x, max_y;
};

struct Rectangle2 {
    Vector2 min;
    Vector2 max;
};

struct Rectangle3 {
    Vector3 min;
    Vector3 max;
};

#define SLIDE_MATH_H
#endif // #if !defined(SLIDE_MATH_H)