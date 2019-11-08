#if !defined(YD_STRING_FORMAT)

#include <stdarg.h>

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define yd_internal static
#define yd_global static
#define yd_local_persist static

typedef int8_t yd_s8;
typedef int16_t yd_s16;
typedef int32_t yd_s32;
typedef int64_t yd_s64;

typedef yd_s8 yd_b8;
typedef yd_s32 yd_b32;

typedef uint8_t yd_u8;
typedef uint16_t yd_u16;
typedef uint32_t yd_u32;
typedef uint64_t yd_u64;

typedef float yd_f32;
typedef double yd_f64;

typedef intptr_t yd_smm;
typedef uintptr_t yd_umm;
#define YD_TYPES
#endif // #if !defined(YD_TYPES)

//
// NOTE(yuval): Utility Macros
//

#if !defined(YD_ASSERT)
# define YD_ASSERT(expression) if (!(expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YD_ASSERT)

// NOTE(yuval): To be used only in the same function
// where the array was defined or with arrays that are defined
// within structs
#if !defined(YD_ARRAY_COUNT)
# define YD_ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))
#endif // #if !defined(YD_ARRAY_COUNT)

#if !defined(YD_READ_VAR_ARG_UNSIGNED_INTEGER)
# define YD_READ_VAR_ARG_UNSIGNED_INTEGER(length, arg_list) (length == 8) ? \
va_arg(arg_list, yd_u64) : va_arg(arg_list, yd_u32)
#endif // #if !defined(YD_READ_VAR_ARG_UNSIGNED_INTEGER)

#if !defined(YD_READ_VAR_ARG_SIGNED_INTEGER)
# define YD_READ_VAR_ARG_SIGNED_INTEGER(length, arg_list) (length == 8) ? \
va_arg(arg_list, yd_s64) : va_arg(arg_list, yd_s32)
#endif // #if !defined(YD_READ_VAR_ARG_SIGNED_INTEGER)

#if !defined(YD_READ_VAR_ARG_FLOAT)
# define YD_READ_VAR_ARG_FLOAT(length, arg_list) va_arg(arg_list, yd_f64)
#endif // #if !defined(YD_READ_VAR_ARG_FLOAT)

//
// NOTE(yuval): Type Definitions
//

struct Format_Dest {
    char* at;
    yd_umm size;
};

//
// NOTE(yuval): Constants And Flags
//

yd_global char yd_string_format__dec_chars[] = "0123456789";
yd_global char yd_string_format__lower_hex_chars[] = "0123456789abcdef";
yd_global char yd_string_format__upper_hex_chars[] = "0123456789ABCDEF";

//
// NOTE(yuval): Public API Function Declarations
//

void out_chars(Format_Dest* dest, const char* value);
void out_string(Format_Dest* dest, String value);
void out_chars_lowercase(Format_Dest* dest, const char* value);
void out_chars_uppercase(Format_Dest* dest, const char* value);

void u64_to_ascii(Format_Dest* dest, yd_u64 value, yd_u32 base, const char* digits);
void f64_to_ascii(Format_Dest* dest, yd_f64 value, yd_u32 precision);
yd_s32 s32_from_z_internal(const char** at_init);

yd_umm format_string_list(char* dest_init, yd_umm dest_size, const char* format, va_list arg_list);

//
// NOTE(yuval): Public API Inline Functions
//

//
// NOTE(yuval): Format_Dest Output Functions
//

yd_internal inline void
out_char(Format_Dest* dest, char value) {
    if (dest->size) {
        --dest->size;
        *dest->at++ = value;
    }
}

//
// NOTE(yuval): Numeric <-> String Conversion Functions
//

yd_internal inline yd_s32
s32_from_z(const char* at) {
    const char* ignored = at;
    yd_s32 result = s32_from_z_internal(&ignored);
    return result;
}

//
// NOTE(yuval): String Formatting Functions
//

// TODO(yuval): Eventually, make this return a String struct
yd_internal inline yd_umm
format_string(char* dest, yd_umm dest_size, const char *format, ...) {
    va_list arg_list;
    
    va_start(arg_list, format);
    yd_umm result = format_string_list(dest, dest_size, format, arg_list);
    va_end(arg_list);
    
    return result;
}

#define YD_STRING_FORMAT
#endif // #if !defined(YD_STRING_FORMAT)

//
// NOTE(yuval): Exported Function Implementations
//

#if defined(YD_STRING_FORMAT_IMPLEMENTATION)

//
// NOTE(yuval): Format_Dest Output Functions
//

void
out_chars(Format_Dest* dest, const char* value) {
    // TODO(yuval & eran): Speed this up
    while (*value) {
        out_char(dest, *value++);
    }
}

void
out_string(Format_Dest* dest, String value) {
    while (value.count--) {
        out_char(dest, *value.data++);
    }
}

void
out_chars_lowercase(Format_Dest* dest, const char* value) {
    while (*value) {
        out_char(dest, to_lower(*value++));
    }
}

void
out_chars_uppercase(Format_Dest* dest, const char* value) {
    while (*value) {
        out_char(dest, to_upper(*value++));
    }
}

//
// NOTE(yuval): Numeric <-> String Conversion Functions
//

void
u64_to_ascii(Format_Dest* dest, yd_u64 value, yd_u32 base, const char* digits) {
    YD_ASSERT(base != 0);
    
    char* start = dest->at;
    
    do {
        yd_u64 digit_index = value % base;
        char digit = digits[digit_index];
        out_char(dest, digit);
        value /= base;
    } while (value != 0);
    
    char* end = dest->at;
    
    while (start < end) {
        --end;
        
        // TODO(yuval): Metaprogramming SWAP
        char temp = *end;
        *end = *start;
        *start = temp;
        ++start;
    }
}

void
f64_to_ascii(Format_Dest* dest, yd_f64 value, yd_u32 precision) {
    if (value < 0) {
        out_char(dest, '-');
        value = -value;
    }
    
    yd_u64 integer_part = (yd_u64)value;
    u64_to_ascii(dest, integer_part, 10, yd_string_format__dec_chars);
    
    value -= integer_part;
    
    out_char(dest, '.');
    
    // TODO(yuval): Round to the precision
    for (yd_u32 precision_index = 0;
         precision_index < precision;
         ++precision_index) {
        value *= 10.0;
        
        yd_u32 integer = (yd_u32)value;
        out_char(dest, yd_string_format__dec_chars[integer]);
        
        value -= integer;
    }
}

yd_s32
s32_from_z_internal(const char** at_init) {
    yd_s32 result = 0;
    const char* at = *at_init;
    
    while ((*at >= '0') && (*at <= '9')) {
        result *= 10;
        result += *at - '0';
        ++at;
    }
    
    *at_init = at;
    return result;
}

//
// NOTE(yuval): String Formatting Functions
//

// NOTE(yuval): The size returned from this function __DOES NOT__
// include the null terminator
yd_umm
format_string_list(char* dest_init, yd_umm dest_size, const char* format, va_list arg_list) {
    Format_Dest dest = {dest_init, dest_size};
    
    if(dest.size) {
        const char *at = format;
        while(at[0]) {
            if(*at == '%') {
                ++at;
                
                yd_b32 force_sign = false;
                yd_b32 pad_with_zeros = false;
                yd_b32 left_justify = false;
                yd_b32 positive_sign_is_blank = false;
                yd_b32 annotate_if_not_zero = false;
                
                yd_b32 parsing = true;
                
                //
                // NOTE(yuval): Handle the flags
                //
                parsing = true;
                while (parsing) {
                    switch (*at) {
                        case '+': { force_sign = true; } break;
                        case '0': { pad_with_zeros = true; } break;
                        case '-': { left_justify = true; } break;
                        case ' ': { positive_sign_is_blank = true; } break;
                        case '#': { annotate_if_not_zero = true; } break;
                        default: { parsing = false; } break;
                    }
                    
                    if (parsing) {
                        ++at;
                    }
                }
                
                //
                // NOTE(yuval): Handle the width
                //
                yd_b32 width_specified = false;
                yd_s32 width = 0;
                if (*at == '*') {
                    width = va_arg(arg_list, int);
                    width_specified = true;
                    ++at;
                } else if((*at >= '0') && (*at <= '9')) {
                    width = s32_from_z_internal(&at);
                    width_specified = true;
                }
                
                //
                // NOTE(yuval): Handle the precision
                //
                yd_b32 precision_specified = false;
                yd_u32 precision = 0;
                if (*at == '.') {
                    ++at;
                    
                    if (*at == '*') {
                        precision = va_arg(arg_list, int);
                        precision_specified = true;
                        ++at;
                    } else if((*at >= '0') && (*at <= '9')) {
                        precision = s32_from_z_internal(&at);
                        precision_specified = true;
                    } else {
                        YD_ASSERT(!"Malformed precision specifier!");
                    }
                }
                
                // TODO(yuval): Right now our routine doesn't allow non-specified
                // precisions, so we just set non-specified precisions to a specified value
                if (!precision_specified) {
                    precision = 6;
                }
                
                //
                // NOTE(yuval): Handle the length
                //
                yd_u32 integer_length = 4;
                yd_u32 float_length = 8;
                
                // TODO(yuval): Actually set different values here!
                if((at[0] == 'h') && (at[1] == 'h')) {
                    at += 2;
                } else if((at[0] == 'l') && (at[1] == 'l')) {
                    at += 2;
                } else if(*at == 'h') {
                    ++at;
                } else if(*at == 'l') {
                    integer_length = 8;
                    ++at;
                } else if(*at == 'j') {
                    ++at;
                } else if(*at == 'z') {
                    ++at;
                } else if(*at == 't') {
                    ++at;
                } else if(*at == 'L') {
                    ++at;
                }
                
                char temp_buffer[64];
                char* temp = temp_buffer;
                Format_Dest temp_dest = {temp, YD_ARRAY_COUNT(temp_buffer)};
                const char* prefix = "";
                yd_b32 is_float = false;
                
                switch(*at) {
                    case 'd':
                    case 'i': {
                        yd_s64 value = YD_READ_VAR_ARG_SIGNED_INTEGER(integer_length, arg_list);
                        
                        yd_b32 was_negative = (value < 0);
                        if (was_negative) {
                            value = -value;
                        }
                        
                        u64_to_ascii(&temp_dest, (yd_u64)value, 10, yd_string_format__dec_chars);
                        
                        // TODO(yuval): Make this a common routine once floating
                        // point is available.
                        if (was_negative) {
                            prefix = "-";
                        } else if(force_sign) {
                            // NOTE(yuval): Not a problem here, but probably shouldn't be specified?
                            YD_ASSERT(!positive_sign_is_blank);
                            prefix = "+";
                        } else if(positive_sign_is_blank) {
                            prefix = " ";
                        }
                    } break;
                    
                    case 'u': {
                        yd_u64 value = YD_READ_VAR_ARG_UNSIGNED_INTEGER(integer_length, arg_list);
                        u64_to_ascii(&temp_dest, value, 10, yd_string_format__dec_chars);
                    } break;
                    
                    case 'o': {
                        yd_u64 value = YD_READ_VAR_ARG_UNSIGNED_INTEGER(integer_length, arg_list);
                        u64_to_ascii(&temp_dest, value, 8, yd_string_format__dec_chars);
                        
                        if (annotate_if_not_zero && (value != 0)) {
                            prefix = "0";
                        }
                    } break;
                    
                    case 'x': {
                        yd_u64 value = YD_READ_VAR_ARG_UNSIGNED_INTEGER(integer_length, arg_list);
                        u64_to_ascii(&temp_dest, value, 16, yd_string_format__lower_hex_chars);
                        
                        if (annotate_if_not_zero && (value != 0)) {
                            prefix = "0x";
                        }
                    } break;
                    
                    case 'X': {
                        yd_u64 value = YD_READ_VAR_ARG_UNSIGNED_INTEGER(integer_length, arg_list);
                        u64_to_ascii(&temp_dest, value, 16, yd_string_format__upper_hex_chars);
                        
                        if (annotate_if_not_zero && (value != 0)) {
                            prefix = "0X";
                        }
                    } break;
                    
                    // TODO(yuval): Support other kinds of floating point prints
                    // (right now we only do basic decimal output)
                    case 'f':
                    case 'F':
                    case 'e':
                    case 'E':
                    case 'g':
                    case 'G':
                    case 'a':
                    case 'A': {
                        yd_f64 value = YD_READ_VAR_ARG_FLOAT(float_length, arg_list);
                        f64_to_ascii(&temp_dest, value, precision);
                        is_float = true;
                    } break;
                    
                    case 'c': {
                        int value = va_arg(arg_list, int);
                        out_char(&temp_dest, (char)value);
                    } break;
                    
                    case 's': {
                        char* string = va_arg(arg_list, char*);
                        
                        // TODO(yuval): Obey precision, width, etc.
                        
                        temp = string;
                        if (precision_specified) {
                            temp_dest.size = 0;
                            for(char* scan = string;
                                *scan && (temp_dest.size < precision);
                                ++scan) {
                                ++temp_dest.size;
                            }
                        } else {
                            temp_dest.size = string_length(string);
                        }
                        
                        temp_dest.at = string + temp_dest.size;
                    } break;
                    
                    case 'S': {
                        String string = va_arg(arg_list, String);
                        
                        // TODO(yuval): Obey precision, width, etc.
                        
                        temp = (char*)string.data;
                        
                        temp_dest.size = string.count;
                        if (precision_specified && (temp_dest.size > precision)) {
                            temp_dest.size = precision;
                        }
                        
                        temp_dest.at = temp + temp_dest.size;
                    } break;
                    
                    case 'p': {
                        void *value = va_arg(arg_list, void*);
                        u64_to_ascii(&temp_dest, *(yd_umm*)&value, 16,
                                     yd_string_format__lower_hex_chars);
                    } break;
                    
                    case 'n': {
                        int *tab_dest = va_arg(arg_list, int*);
                        *tab_dest = (int)(dest.at - dest_init);
                    } break;
                    
                    case '%': {
                        out_char(&dest, '%');
                    } break;
                    
                    default: {
                        YD_ASSERT(!"Unrecognized format specifier");
                    } break;
                }
                
                if (temp_dest.at - temp) {
                    yd_smm use_precision = precision;
                    if (is_float || !precision_specified)
                    {
                        use_precision = (temp_dest.at - temp);
                    }
                    
                    yd_smm prefix_length = string_length(prefix);
                    yd_smm use_width = width;
                    yd_smm computed_width = use_precision + prefix_length;
                    if (use_width < computed_width) {
                        use_width = computed_width;
                    }
                    
                    if (pad_with_zeros) {
                        // NOTE(yuval): Not a problem, but no way to do it?
                        YD_ASSERT(!left_justify);
                        left_justify = false;
                    }
                    
                    if(!left_justify) {
                        while (use_width > (use_precision + prefix_length)) {
                            out_char(&dest, pad_with_zeros ? '0' : ' ');
                            --use_width;
                        }
                    }
                    
                    for (const char *pre = prefix; *pre && use_width; ++pre) {
                        out_char(&dest, *pre);
                        --use_width;
                    }
                    
                    if (use_precision > use_width) {
                        use_precision = use_width;
                    }
                    
                    while (use_precision > (temp_dest.at - temp)) {
                        out_char(&dest, '0');
                        --use_precision;
                        --use_width;
                    }
                    
                    while (use_precision && (temp_dest.at != temp)) {
                        out_char(&dest, *temp++);
                        --use_precision;
                        --use_width;
                    }
                    
                    if (left_justify) {
                        while(use_width) {
                            out_char(&dest, ' ');
                            --use_width;
                        }
                    }
                }
                
                if (*at) {
                    ++at;
                }
            }
            else {
                out_char(&dest, *at++);
            }
        }
        
        if(dest.size) {
            dest.at[0] = 0;
        } else {
            dest.at[-1] = 0;
        }
    }
    
    yd_umm result = dest.at - dest_init;
    return result;
}

#endif // #if defined(YD_STRING_FORMAT_IMPLEMENTATION)
