//
// NOTE(yuval): Scalar Operations
//
inline f32
square(f32 v) {
    f32 result = v * v;
    return result;
}

inline f32
lerp(f32 a, f32 t, f32 b) {
    f32 result = (1.0f - t) * a + t * b;
    return result;
}

inline s32
s32_binormal_lerp(s32 a, f32 t_binormal, s32 b) {
    f32 t = 0.5f + (0.5f * t_binormal);
    f32 f_result = lerp((f32)a, t, (f32)b);
    s32 result = round_f32_to_s32(f_result);
    
    return result;
}

inline f32
safe_ratio_n(f32 numerator, f32 divisor, f32 n) {
    f32 result = n;
    
    if (divisor != 0.0f) {
        result = numerator / divisor;
    }
    
    return result;
}

inline f32
safe_ratio_0(f32 numerator, f32 divisor) {
    f32 result = safe_ratio_n(numerator, divisor, 0.0f);
    return result;
}

inline f32
safe_ratio_1(f32 numerator, f32 divisor) {
    f32 result = safe_ratio_n(numerator, divisor, 1.0f);
    return result;
}

//
// NOTE(yuval): Vector2 Functions
//

// NOTE(yuval): Initialization
inline Vector2
make_v2(f32 x, f32 y) {
    Vector2 result = {x, y};
    return result;
}

inline Vector2
make_v2(s32 x, s32 y) {
    Vector2 result = {(f32)x, (f32)y};
    return result;
}

inline Vector2
make_v2(u32 x, u32 y) {
    Vector2 result = {(f32)x, (f32)y};
    return result;
}

inline Vector2
v2_from(Vector2u source) {
    Vector2 result = {(f32)source.x, (f32)source.y};
    return result;
}

// NOTE(yuval): Operations
inline Vector2
operator+(Vector2 a, Vector2 b) {
    Vector2 result = {a.x + b.x, a.y + b.y};
    return result;
}

inline Vector2
operator-(Vector2 v) {
    Vector2 result = {-v.x, -v.y};
    return result;
}

inline Vector2
operator-(Vector2 a, Vector2 b) {
    Vector2 result = {a.x - b.x, a.y - b.y};
    return result;
}

inline Vector2
operator*(f32 a, Vector2 b) {
    Vector2 result = {a * b.x, a * b.y};
    return result;
}

inline Vector2
operator*(Vector2 a, f32 b) {
    Vector2 result = b * a;
    return result;
}

inline Vector2
hadamard(Vector2 a, Vector2 b) {
    Vector2 result = {a.x * b.x, a.y * b.y};
    return result;
}

//
// NOTE(yuval): Vector2u Functions
//

// NOTE(yuval): Initialization
inline Vector2u
make_v2u(u32 x, u32 y) {
    Vector2u result = {x, y};
    return result;
}

//
// NOTE(yuval): Vector3 Functions
//

// NOTE(yuval): Initialization
inline Vector3
make_v3(f32 x, f32 y, f32 z) {
    Vector3 result = {x, y, z};
    return result;
}

inline Vector3
make_v3(s32 x, s32 y, s32 z) {
    Vector3 result = {(f32)x, (f32)y, (f32)z};
    return result;
}

inline Vector3
make_v3(u32 x, u32 y, u32 z) {
    Vector3 result = {(f32)x, (f32)y, (f32)z};
    return result;
}

inline Vector3
make_v3(Vector2 xy, f32 z) {
    Vector3 result = {xy.x, xy.y, z};
    return result;
}

// NOTE(yuval): Operations
inline Vector3
operator+(Vector3 a, Vector3 b) {
    Vector3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

inline Vector3
operator-(Vector3 v) {
    Vector3 result = {-v.x, -v.y, -v.x};
    return result;
}

inline Vector3
operator-(Vector3 a, Vector3 b) {
    Vector3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

inline Vector3
operator*(f32 a, Vector3 b) {
    Vector3 result = {a * b.x, a * b.y, a * b.z};
    return result;
}

inline Vector3
operator*(Vector3 a, f32 b) {
    Vector3 result = b * a;
    return result;
}

inline f32
inner(Vector3 a, Vector3 b) {
    f32 result = a.x * b.x + a.y * b.y + a.z * b.z;
    return result;
}

inline Vector3
cross(Vector3 a, Vector3 b) {
    Vector3 result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    
    return result;
}

inline f32
length_sq(Vector3 v) {
    f32 result = inner(v, v);
    return result;
}

inline f32
length(Vector3 v) {
    f32 result = square_root(length_sq(v));
    return result;
}

inline Vector3
noz(Vector3 v) {
    Vector3 result = {};
    
    f32 len_sq = length_sq(v);
    if (len_sq > square(0.0001f)) {
        result = v * (1.0f / square_root(len_sq));
    }
    
    return result;
}

//
// NOTE(yuval): Vector4 Functions
//

// NOTE(yuval): Initialization
inline Vector4
make_v4(f32 x, f32 y, f32 z, f32 w) {
    Vector4 result = {x, y, z, w};
    return result;
}

inline Vector4
make_v4(s32 x, s32 y, s32 z, s32 w) {
    Vector4 result = {(f32)x, (f32)y, (f32)z, (f32)w};
    return result;
}

inline Vector4
make_v4(u32 x, u32 y, u32 z, u32 w) {
    Vector4 result = {(f32)x, (f32)y, (f32)z, (f32)w};
    return result;
}

inline Vector4
make_v4(Vector3 xyz, f32 w) {
    Vector4 result = {xyz.x, xyz.y, xyz.z, w};
    return result;
}

// NOTE(yuval): Operations
inline Vector4
operator-(Vector4 v) {
    Vector4 result = {-v.x, -v.y, -v.z, -v.w};
    return result;
}

inline Vector4
operator-(Vector4 a, Vector4 b) {
    Vector4 result = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w
    };
    
    return result;
}

inline Vector4
operator*(f32 a, Vector4 b) {
    Vector4 result = {a * b.x, a * b.y, a * b.z, a * b.w};
    return result;
}

inline Vector4
operator*(Vector4 a, f32 b) {
    Vector4 result = b * a;
    return result;
}

//
// NOTE(yuval): Rectangle2 Functions
//

// NOTE(yuval): Initialization
inline Rectangle2
rect_min_max(Vector2 min, Vector2 max) {
    Rectangle2 result;
    result.min = min;
    result.max = max;
    
    return result;
}

// NOTE(yuval): Operations
inline Vector2
get_dim(Rectangle2 rect) {
    Vector2 result = (rect.max - rect.min);
    return result;
}

inline Vector2
get_center(Rectangle2 rect) {
    Vector2 result = 0.5f * (rect.min + rect.max);
    return result;
}

//
// NOTE(yuval): Rectangle2i Functions
//

// NOTE(yuval): Initialization
inline Rectangle2i
rect_min_dim(s32 min_x, s32 min_y, s32 dim_x, s32 dim_y) {
    Rectangle2i result;
    result.min_x = min_x;
    result.min_y = min_y;
    result.max_x = min_x + dim_x;
    result.max_y = min_y + dim_y;
    
    return result;
}

// NOTE(yuval): Operations
inline s32
get_width(Rectangle2i rect) {
    s32 result = (rect.max_x - rect.max_y);
    return result;
}

inline s32
get_height(Rectangle2i rect) {
    s32 result = (rect.max_y - rect.min_y);
    return result;
}

inline Rectangle2i
offset(Rectangle2i rect, s32 x, s32 y) {
    Rectangle2i result = rect;
    result.min_x += x;
    result.max_x += x;
    result.min_y += y;
    result.max_y += y;
    
    return result;
}

//
// NOTE(yuval): Matrix4x4 Functions
//

// NOTE(yuval): Operations
inline Matrix4x4
identity() {
    Matrix4x4 result = {
        {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
    
    return result;
}

internal Matrix4x4
orthographic_projection(f32 left, f32 right, f32 bottom, f32 top, f32 near_depth, f32 far_depth)
{
    Matrix4x4 result = {};
    
    result.e[0][0] = 2.f / (right - left);
    result.e[1][1] = 2.f / (top - bottom);
    result.e[2][2] = -2.f / (far_depth - near_depth);
    result.e[3][3] = 1.f;
    result.e[3][0] = (left + right) / (left - right);
    result.e[3][1] = (bottom + top) / (bottom - top);
    result.e[3][2] = (far_depth + near_depth) / (near_depth - far_depth);
    
    return result;
}

//
// NOTE(yuval): Miscellaneous Functions
//

inline u32
rgba_pack_4x8(Vector4 unpacked) {
    u32 result = ((round_f32_to_u32(unpacked.a) << 24) |
                  (round_f32_to_u32(unpacked.b) << 16) |
                  (round_f32_to_u32(unpacked.g) << 8) |
                  (round_f32_to_u32(unpacked.r) << 0));
    
    return result;
}

inline Rectangle2i
aspect_ratio_fit(u32 render_width, u32 render_height,
                 u32 window_width, u32 window_height) {
    Rectangle2i result = {};
    
    if ((render_width > 0) && (render_height > 0) &&
        (window_width > 0) && (window_height > 0)) {
        f32 optimal_window_width =
            (f32)window_height * ((f32)render_width / (f32)render_height);
        f32 optimal_window_height =
            (f32)window_width * ((f32)render_height / (f32)render_width);
        
        if (optimal_window_width > (f32)window_width) {
            // NOTE(yuval): Width-constrained display - top and bottom black bars
            result.min_x = 0;
            result.max_x = window_width;
            
            f32 empty = (f32)window_height - optimal_window_height;
            s32 half_empty = round_f32_to_s32(0.5f * empty);
            s32 use_height = round_f32_to_s32(optimal_window_height);
            
            result.min_y = half_empty;
            result.max_y = result.min_y + use_height;
        } else {
            // NOTE(yuval): Height-constrained display - left and right black bars
            result.min_y = 0;
            result.max_y = window_height;
            
            f32 empty = (f32)window_width - optimal_window_width;
            s32 half_empty = round_f32_to_s32(0.5f * empty);
            s32 use_width = round_f32_to_s32(optimal_window_width);
            
            result.min_x = half_empty;
            result.max_x = result.min_x + use_width;
        }
    }
    
    return result;
}