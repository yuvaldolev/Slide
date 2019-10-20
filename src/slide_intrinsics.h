#if !defined(SLIDE_INTRINSICS_H)

// TODO(yuval): Maybe convert this to a cpp file as it does contain logic?

#include <math.h>

inline f32
square_root(f32 value) {
    f32 result = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(value)));
    return result;
}

inline s32
round_f32_to_s32(f32 value) {
    s32 result = _mm_cvtss_si32(_mm_set_ss(value));
    return result;
}

inline u32
round_f32_to_u32(f32 value) {
    u32 result = (u32)_mm_cvtss_si32(_mm_set_ss(value));
    return result;
}

#define SLIDE_INTRINSICS_H
#endif // #if !defined(SLIDE_INTRINSICS_H)