#if !defined(YD_STRING)

// TODO(yuval): UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING
// UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING  UNIT TESTING

// TODO(yuval): MAKE THE CODE MORE ROBUST: Check for null pointers and so on....

// TODO(yuval): to_s32 function is incorrent. Right now it does not handle negative numbers.
// fix that...

// TODO(yuval): Create / modify find / replace functions that work with char* strings with the same
// speed as with my strings

// TODO(yuval): Number To String Conversion For Different Bases

// TODO(yuval): Implement color_to_hex_string and hex_string_to_color
// after I decide on my color string format

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

#if !defined(YD_MINIMUM)
# define YD_MINIMUM(a, b) (((a) < (b)) ? (a) : (b))
#endif // #if !defined(YD_MINIMUM)

#if !defined(YD_MINIMUM3)
# define YD_MINIMUM3(a, b, c) YD_MINIMUM(YD_MINIMUM((a), (b)), (c))
#endif // #if !defined(YD_MINIMUM3)

#if !defined(YD_MAXIMUM)
# define YD_MAXIMUM(a, b) (((a) > (b)) ? (a) : (b))
#endif // #if !defined(YD_MAXIMUM)

#if !defined(YD_MAXIMUM3)
# define YD_MAXIMUM3(a, b, c) YD_MAXIMUM(YD_MAXIMUM((a), (b)), (c))
#endif // #if !defined(YD_MAXIMUM3)

//
// NOTE(yuval): Type Definitions
//

struct String {
    char* data;
    yd_umm count;
    yd_umm capacity;
    
    char& operator[](yd_umm index) {
        YD_ASSERT(index < count);
        
        char& result = data[index];
        return result;
    }
    
    const char& operator[](yd_umm index) const {
        YD_ASSERT(index < count);
        
        const char& result = data[index];
        return result;
    }
};

struct U64_Conversion_Result {
    yd_u64 value;
    yd_b32 success;
};

struct U32_Conversion_Result {
    yd_u32 value;
    yd_b32 success;
};

struct S32_Conversion_Result {
    yd_s32 value;
    yd_b32 success;
};

struct F32_Conversion_Result {
    yd_f32 value;
    yd_b32 success;
};

// TODO(yuval): Maybe create A UTF8 string struct

#if !defined(INIT_FROM_LITERAL)
# define INIT_FROM_LITERAL(str) {str, sizeof(str) - 1, sizeof(str)}
#endif // #if !defined(INIT_FROM_LITERAL)

#if !defined(EXPAND_LITERAL)
# define EXPAND_LITERAL(str) (str), (sizeof(str) - 1)
#endif // #if !defined(EXPAND_LITERAL)

#if !defined(EXPAND_STRING)
# define EXPAND_STRING(str) ((str).data), ((str).count)
#endif // #if !defined(EXPAND_STRING)

#if !defined(PRINTABLE_STRING)
# define PRINTABLE_STRING(str) ((yd_s32)((str).count)), ((str).data)
#endif // #if !defined(PRINTABLE_STRING)

//
// NOTE(yuval): Flags And Constants
//

yd_global const String NULL_STRING = {};
yd_global const yd_umm STRING_NOT_FOUND = (yd_umm)-1;

//
// NOTE(yuval): Public API Function Declarations
//

yd_b32 match(const char* a, const char* b);
yd_b32 match(String a, const char* b);
yd_b32 match(String a, String b);
yd_b32 match(const char* a, yd_umm a_count, const char* b);
yd_b32 match(const char* a, yd_umm a_count,
             const char* b, yd_umm b_count);
yd_b32 match(String a, const char* b, yd_umm count);
yd_b32 match_part(const char* a, const char* b, yd_umm* out_count);
yd_b32 match_part(String a, const char* b, yd_umm* out_count);
yd_b32 match_part(const char* a, String b, yd_umm* out_count);
yd_b32 match_part(String a, String b, yd_umm* out_count);
yd_b32 match_insensitive(const char* a, const char* b);
yd_b32 match_insensitive(String a, const char* b);
yd_b32 match_insensitive(String a, String b);
yd_b32 match_part_insensitive(const char* a, const char* b, yd_umm* out_count);
yd_b32 match_part_insensitive(String a, const char* b, yd_umm* out_count);
yd_b32 match_part_insensitive(const char* a, String b, yd_umm* out_count);
yd_b32 match_part_insensitive(String a, String b, yd_umm* out_count);
yd_b32 string_set_match(void* str_set, yd_umm item_size, yd_umm count,
                        String str, yd_umm* out_match_index);

yd_s32 compare(const char* a, const char* b);
yd_s32 compare(String a, const char* b);
yd_s32 compare(String a, String b);

yd_umm find(const char* str, char character, yd_umm start);
yd_umm find(String str, char character, yd_umm start);
yd_umm find(const char* str, String seek, yd_umm start);
yd_umm find(String str, String seek, yd_umm start);
yd_umm rfind(const char* str, yd_umm count, char character, yd_umm start);
yd_umm rfind(String str, char character, yd_umm start);
yd_umm rfind(const char* str, yd_umm count, String seek, yd_umm start);
yd_umm rfind(String str, String seek, yd_umm start);
yd_umm find_first_of(const char* str, const char* characters, yd_umm start);
yd_umm find_first_of(String str, const char* characters,  yd_umm start);
yd_umm find_insensitive(const char* str, char character, yd_umm start);
yd_umm find_insensitive(String str, char character, yd_umm start);
yd_umm find_insensitive(const char* str, String seek, yd_umm start);
yd_umm find_insensitive(String str, String seek, yd_umm start);

String eat_leading_spaces(String str);
String eat_trailing_spaces(String str);
String consume_word(String* source);
String consume_line(String* source);
String consume_double_line(String* source);

yd_umm copy_fast_unsafe(char* dest, const char* source);
yd_umm copy_fast_unsafe(char* dest, String source);
yd_b32 copy_checked(String* dest, String source);
yd_b32 copy_checked(char* dest, yd_umm dest_cap, String source);
yd_b32 copy_partial(String* dest, const char* source);
yd_b32 copy_partial(String* dest, String source);
yd_b32 copy_partial(char* dest, yd_umm dest_cap, String source);

yd_b32 append_checked(String* dest, String source);
yd_b32 append_partial(String* dest, const char* source);
yd_b32 append_partial(String* dest, String source);
yd_b32 append(String* dest, char c);
yd_b32 terminate_with_null(String* str);
yd_b32 append_padding(String* dest, char c, yd_umm target_count);

yd_b32 concat(String* dest, String source_a, String source_b);

void replace_range(String* str, yd_umm first, yd_umm one_past_last, char with);
void replace_range(String* str, yd_umm first, yd_umm one_past_last, const char* with);
void replace_range(String* str, yd_umm first, yd_umm one_past_last, String with);
void replace(String* str, char to_replace, char with);
void replace(String* str, const char* to_replace, const char* with);
void replace(String* str, const char* to_replace, String with);
void replace(String* str, String to_replace, const char* with);
void replace(String* str, String to_replace, String with);
void string_interpret_escapes(char* dest, String source);

#if defined(YD_MEMORY)
char* push_z(yd_umm count, Arena_Push_Params params = default_arena_params());
String push_string(yd_umm capacity, Arena_Push_Params params = default_arena_params());
char* push_copy_z(const char* source, Arena_Push_Params params = default_arena_params());
String push_copy_string(const char* source, Arena_Push_Params params = default_arena_params());
char* push_copy_z(String source, Arena_Push_Params params = default_arena_params());
String push_copy_string(String source, Arena_Push_Params params = default_arena_params());
#endif // #if defined(YD_MEMORY)

void to_lower(char* str);
void to_lower(String* str);
void to_lower(char* dest, const char* source);
void to_lower(String* dest, const char* source);
void to_lower(char* dest, String source);
void to_lower(String* dest, String source);
void to_upper(char* str);
void to_upper(String* str);
void to_upper(char* dest, const char* source);
void to_upper(String* dest, const char* source);
void to_upper(char* dest, String source);
void to_upper(String* dest, String source);
void to_camel(char* str);
void to_camel(String* str);
void to_camel(char* dest, const char* source);
void to_camel(String* dest, const char* source);
void to_camel(char* dest, String source);
void to_camel(String* dest, String source);
yd_umm to_string_count(yd_u64 value);
yd_b32 to_string(String* dest, yd_u64 value);
yd_b32 append(String* dest, yd_u64 value);
U64_Conversion_Result to_u64(const char* str);
U64_Conversion_Result to_u64(String str);
yd_umm to_string_count(yd_s32 value);
yd_b32 to_string(String* dest, yd_s32 value);
yd_b32 append(String* dest, yd_s32 value);
yd_umm to_string_count(yd_f32 value, yd_u32 max_precision);
yd_b32 to_string(String* dest, yd_f32 value, yd_u32 max_precision);
yd_b32 append(String* dest, yd_f32 value, yd_u32 max_precision);
F32_Conversion_Result to_f32(const char* str);
F32_Conversion_Result to_f32(String str);
yd_u32 hex_string_to_u32(String str);
yd_b32 color_to_hex_string(String* dest, yd_u32 color);
yd_b32 hex_string_to_color(yd_u32* dest, String str);

yd_umm reverse_seek_slash(String str, yd_umm shift_from_last_char);
yd_b32 set_last_folder(String* dir, const char* folder_name, char slash);
yd_b32 set_last_folder(String* dir, String folder_name, char slash);
yd_b32 remove_last_folder(String* dir);
String file_extension(String filename);
yd_b32 set_extension(String* filename, const char* extension);
yd_b32 set_extension(String* filename, String extension);
yd_b32 remove_extension(String* filename);

//
// NOTE(yuval): Public API Inline Functions
//

//
// NOTE(yuval): String Making Functions
//

yd_internal inline String
make_string(void* data, yd_umm count, yd_umm capacity) {
    String result;
    result.data = (char*)data;
    result.count = count;
    result.capacity = capacity;
    
    return result;
}

yd_internal inline String
make_string(void* data, yd_umm count) {
    String result = make_string(data, count, count);
    return result;
}

yd_internal inline yd_umm
string_length(const char* str) {
    yd_umm count = 0;
    
    if (str) {
        for (; str[count]; ++count);
    }
    
    return count;
}

yd_internal inline String
make_string_slowly(const void* str) {
    String result;
    result.data = (char*)str;
    result.count = string_length((const char*)str);
    result.capacity = result.count + 1;
    
    return result;
}

#if !defined(BUNDLE_LITERAL)
# define BUNDLE_LITERAL(str) (make_string((char*)(str), sizeof(str) - 1, sizeof(str)))
#endif // #if !defined(BUNDLE_LITERAL)

#if !defined(MAKE_FIXED_WIDTH_STRING)
# define BUNDLE_LITERAL_FIXED_WIDTH(str) (make_string((char*)(str), 0, sizeof(str)))
#endif // #if !defined(MAKE_FIXED_WIDTH_STRING)

//
// NOTE(yuval): Utility String Functions
//

yd_internal inline yd_b32
is_empty(String str) {
    yd_b32 result = (str.count == 0);
    return result;
}

yd_internal inline yd_b32
is_null_string(String str) {
    yd_b32 result = ((str.data == NULL_STRING.data) &&
                     (str.count == NULL_STRING.count) &&
                     (str.capacity == NULL_STRING.capacity));
    
    return result;
}

//
// NOTE(yuval): String Slicing Functions
//

yd_internal inline String
substr(String str, yd_umm start) {
    YD_ASSERT((start >= 0) && (start <= str.count));
    
    String result;
    result.data = str.data + start;
    result.count = str.count - start;
    result.capacity = 0;
    
    return result;
}

yd_internal inline String
substr(String str, yd_umm start, yd_umm count) {
    YD_ASSERT((start >= 0) && (start <= str.count));
    
    String result;
    result.data = str.data + start;
    
    result.count = count;
    // TODO(yuval): Verify that this works
    if ((start + count) > str.count) {
        result.count = str.count - start;
    }
    
    result.capacity = 0;
    
    return result;
}

yd_internal inline String
eat_spaces(String str) {
    String result = eat_leading_spaces(str);
    result = eat_trailing_spaces(result);
    return result;
}

yd_internal inline String
tailstr(String str) {
    String result;
    result.data = str.data + str.count;
    result.count = 0;
    result.capacity = str.capacity - str.count;
    
    return result;
}

yd_internal inline void
advance(String* value, yd_umm count) {
    if (value->count >= count) {
        value->data += count;
        value->count -= count;
        value->capacity -= count;
    } else {
        value->data += value->count;
        value->count = 0;
        value->capacity = 0;
    }
}

//
// NOTE(yuval): String Comparison Functions
//

yd_internal inline yd_b32
match(const char* a, String b) {
    yd_b32 result = match(b, a);
    return result;
}

yd_internal inline yd_b32
match(const char* a, const char* b, yd_umm b_count) {
    yd_b32 result = match(b, b_count, a);
    return result;
}

yd_internal inline yd_b32
match(const char* a, String b, yd_umm count) {
    yd_b32 result = match(b, a, count);
    return result;
}

yd_internal inline yd_b32
match_part(const char* a, const char* b) {
    yd_umm ignored;
    yd_b32 result = match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_part(String a, const char* b) {
    yd_umm ignored;
    yd_b32 result = match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_part(const char* a, String b) {
    yd_umm ignored;
    yd_b32 result = match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_part(String a, String b) {
    yd_umm ignored;
    yd_b32 result = match_part(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_insensitive(const char* a, String b) {
    yd_b32 result = match_insensitive(b, a);
    return result;
}

yd_internal inline yd_b32
match_part_insensitive(const char* a, const char* b) {
    yd_umm ignored;
    yd_b32 result = match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_part_insensitive(String a, const char* b) {
    yd_umm ignored;
    yd_b32 result = match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_part_insensitive(const char* a, String b) {
    yd_umm ignored;
    yd_b32 result = match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
match_part_insensitive(String a, String b) {
    yd_umm ignored;
    yd_b32 result = match_part_insensitive(a, b, &ignored);
    return result;
}

yd_internal inline yd_b32
string_set_match(String* str_set, yd_umm count,
                 String str, yd_umm* out_match_index) {
    yd_b32 result = string_set_match(str_set, sizeof(String), count,
                                     str, out_match_index);
    return result;
}

yd_internal inline yd_s32
compare(const char* a, String b) {
    yd_s32 result = -compare(b, a);
    return result;
}

//
// NOTE(yuval): Functions For Finding Characters And SubStrings
//

yd_internal inline yd_umm
find(const char* str, char character) {
    yd_umm result = find(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find(String str, char character) {
    yd_umm result = find(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find(const char* str, String seek) {
    yd_umm result = find(str, seek, 0);
    return result;
}

yd_internal inline yd_umm
find(String str, String seek) {
    yd_umm result = find(str, seek, 0);
    return result;
}

yd_internal inline yd_umm
rfind(const char* str, yd_umm count, char character) {
    yd_umm result = rfind(str, count, character, count - 1);
    return result;
}

yd_internal inline yd_umm
rfind(String str, char character) {
    yd_umm result = rfind(str, character, str.count - 1);
    return result;
}

yd_internal inline yd_umm
rfind(const char* str, yd_umm count, String seek) {
    yd_umm result = rfind(str, count, seek, count - 1);
    return result;
}

yd_internal inline yd_umm
rfind(String str, String seek) {
    yd_umm result = find(str, seek, str.count - 1);
    return result;
}

yd_internal inline yd_umm
find_first_of(const char* str, const char* characters) {
    yd_umm result = find_first_of(str, characters, 0);
    return result;
}

yd_internal inline yd_umm
find_first_of(String str, const char* characters) {
    yd_umm result = find_first_of(str, characters, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(const char* str, char character) {
    yd_umm result = find_insensitive(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(String str, char character) {
    yd_umm result = find_insensitive(str, character, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(const char* str, String seek) {
    yd_umm result = find_insensitive(str, seek, 0);
    return result;
}

yd_internal inline yd_umm
find_insensitive(String str, String seek) {
    yd_umm result = find_insensitive(str, seek, 0);
    return result;
}

yd_internal inline yd_b32
has_substr(const char* str, String seek) {
    yd_b32 result = (find(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline yd_b32
has_substr(String str, String seek) {
    yd_b32 result = (find(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline yd_b32
has_substr_insensitive(const char* str, String seek) {
    yd_b32 result = (find_insensitive(str, seek) != STRING_NOT_FOUND);
    return result;
}

yd_internal inline yd_b32
has_substr_insensitive(String str, String seek) {
    yd_b32 result = (find_insensitive(str, seek) != STRING_NOT_FOUND);
    return result;
}

//
// NOTE(yuval): String Copy And Append Functions
//

yd_internal inline yd_umm
copy(char* dest, const char* source) {
    yd_umm result = copy_fast_unsafe(dest, source);
    return result;
}

yd_internal inline yd_umm
copy(char* dest, String source) {
    yd_umm result = copy_fast_unsafe(dest, source);
    return result;
}

yd_internal inline void
copy(String* dest, String source) {
    copy_checked(dest, source);
}

yd_internal inline void
copy(String* dest, const char* source) {
    copy_partial(dest, source);
}

yd_internal inline yd_b32
append(String* dest, const char* source) {
    yd_b32 result = append_partial(dest, source);
    return result;
}

yd_internal inline yd_b32
append(String* dest, String source) {
    yd_b32 result = append_partial(dest, source);
    return result;
}

//
// NOTE(yuval): String Concatenation Functions
//

yd_internal inline yd_b32
concat(char* dest, yd_umm dest_count,
       const char* source_a, yd_umm source_a_count,
       const char* source_b, yd_umm source_b_count) {
    String dest_string = make_string(dest, dest_count - 1);
    String source_a_string = make_string((char*)source_a, source_a_count);
    String source_b_string = make_string((char*)source_b, source_b_count);
    
    yd_b32 result = concat(&dest_string, source_a_string, source_b_string);
    dest[dest_string.count] = 0;
    
    return result;
}

yd_internal inline yd_b32
concat(char* dest, yd_umm dest_count,
       String source_a, String source_b) {
    String dest_string = make_string(dest, dest_count - 1);
    
    yd_b32 result = concat(&dest_string, source_a, source_b);
    dest[dest_string.count] = 0;
    
    return result;
}

yd_internal inline yd_b32
concat(String* dest,
       const char* source_a, yd_umm source_a_count,
       const char* source_b, yd_umm source_b_count) {
    String source_a_string = make_string((char*)source_a, source_a_count);
    String source_b_string = make_string((char*)source_b, source_b_count);
    
    yd_b32 result = concat(dest, source_a_string, source_b_string);
    return result;
}

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

yd_internal inline yd_b32
is_lower(char c) {
    yd_b32 result = ((c >= 'A') && (c <= 'z'));
    return result;
}

yd_internal inline yd_b32
is_lower(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_lower(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_lower(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_lower(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_lower_utf8(yd_u8 c) {
    yd_b32 result = (is_lower((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32 is_upper(char c);

yd_internal inline char
to_lower(char c) {
    char result = c;
    
    if (is_upper(result)) {
        result += 'a' - 'A';
    }
    
    return result;
}

yd_internal inline yd_b32
is_upper(char c) {
    yd_b32 result = ((c >= 'A') && (c <= 'Z'));
    return result;
}

yd_internal inline yd_b32
is_upper(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_upper(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_upper(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_upper(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_upper_utf8(yd_u8 c) {
    yd_b32 result = (is_upper((char)c) || (c >= 128));
    return result;
}

yd_internal inline char
to_upper(char c) {
    char result = c;
    
    if (is_lower(result)) {
        result -= 'a' - 'A';
    }
    
    return result;
}

yd_internal inline yd_b32
is_spacing(char c) {
    yd_b32 result = ((c == ' ') || (c == '\t') ||
                     (c == '\v') || (c == '\f'));
    return result;
}

yd_internal inline yd_b32
is_end_of_line(char c) {
    yd_b32 result = ((c == '\n') || (c == '\r'));
    return result;
}

yd_internal inline yd_b32
is_whitespace(char c) {
    yd_b32 result = (is_spacing(c) || is_end_of_line(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_true(char c) {
    yd_b32 result = (is_lower(c) || is_upper(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_true(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_true(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_true(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_alpha_true(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_true_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha_true((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_alpha(char c) {
    yd_b32 result = (is_alpha_true(c) || (c == '_'));
    return result;
}

yd_internal inline yd_b32
is_alpha(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at)
    {
        if (!is_alpha(*at))
        {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index)
    {
        if (!is_alpha(str.data[index]))
        {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_numeric(char c) {
    yd_b32 result = ((c >= '0') && (c <= '9'));
    return result;
}

yd_internal inline yd_b32
is_numeric(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_numeric(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_numeric(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_numeric(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_numeric_utf8(yd_u8 c) {
    yd_b32 result = (is_numeric((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_umm
to_string_count(yd_f32 value) {
    yd_umm result = to_string_count(value, 8);
    return result;
}

yd_internal inline yd_b32
to_string(String* dest, yd_f32 value) {
    yd_b32 result = to_string(dest, value, 8);
    return result;
}

yd_internal inline yd_b32
append(String* dest, yd_f32 value) {
    yd_b32 result = append(dest, value, 8);
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric(char c) {
    yd_b32 result = (is_alpha(c) || is_numeric(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_numeric(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_alpha_numeric(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha_numeric((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true(char c) {
    yd_b32 result = (is_alpha_true(c) || is_numeric(c));
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_alpha_numeric_true(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true(String str)
{
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_alpha_numeric_true(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_alpha_numeric_true_utf8(yd_u8 c) {
    yd_b32 result = (is_alpha_numeric_true((char)c) || (c >= 128));
    return result;
}

yd_internal inline U32_Conversion_Result
to_u32(const char* str) {
    U64_Conversion_Result u64_result = to_u64(str);
    U32_Conversion_Result result = {
        (yd_u32)u64_result.value,
        u64_result.success
    };
    
    return result;
}

yd_internal inline U32_Conversion_Result
to_u32(String str) {
    U64_Conversion_Result u64_result = to_u64(str);
    U32_Conversion_Result result = {
        (yd_u32)u64_result.value,
        u64_result.success
    };
    
    return result;
}

yd_internal inline S32_Conversion_Result
to_s32(const char* str) {
    U64_Conversion_Result u64_result = to_u64(str);
    S32_Conversion_Result result = {
        (yd_s32)u64_result.value,
        u64_result.success
    };
    
    return result;
}

yd_internal inline S32_Conversion_Result
to_s32(String str) {
    U64_Conversion_Result u64_result = to_u64(str);
    S32_Conversion_Result result = {
        (yd_s32)u64_result.value,
        u64_result.success
    };
    
    return result;
}

yd_internal inline yd_b32
is_hex(char c) {
    yd_b32 result = (((c >= '0') && (c <= '9')) ||
                     ((c >= 'A') && (c <= 'F')) ||
                     ((c >= 'a') && (c <= 'f')));
    return result;
}

yd_internal inline yd_b32
is_hex(const char* str) {
    yd_b32 result = true;
    
    for (const char* at = str; *at; ++at) {
        if (!is_hex(*at)) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_hex(String str) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (!is_hex(str.data[index])) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_internal inline yd_b32
is_hex_utf8(yd_u8 c) {
    yd_b32 result = (is_hex((char)c) || (c >= 128));
    return result;
}

yd_internal inline yd_s32
hex_char_to_s32(char c) {
    yd_s32 result;
    
    if (is_numeric(c)) {
        result = c - '0';
    } else if (c > 'F') {
        result = 10 + (c - 'a');
    } else {
        result = 10 + (c - 'A');
    }
    
    return result;
}

yd_internal inline char
s32_to_hex_char(yd_s32 value) {
    char result;
    
    if (value < 10) {
        result = '0' + (char)value;
    } else {
        result = 'A' + (char)(value - 10);
    }
    
    return result;
}

//
// NOTE(yuval): File / Directory Strings Management Functions
//

yd_internal inline yd_umm
reverse_seek_slash(String str) {
    yd_umm result = reverse_seek_slash(str, 0);
    return result;
}

yd_internal inline yd_b32
is_slash(char c) {
    yd_b32 result = ((c == '/') || (c == '\\'));
    return result;
}

yd_internal inline String
front_of_directory(String dir) {
    // NOTE(yuval): This works even if a slash is not found:
    // when a slash is not found, reverse_seek_slash returns
    // STRING_NOT_FOUND which is defined to be the max value
    // of yd_umm. This means that if the slash is not found,
    // we add 1 to the max unsigned value, and therefore we
    // get the index 0, which will return the whole string.
    String result = substr(dir, reverse_seek_slash(dir) + 1);
    return result;
}

yd_internal inline String
path_of_directory(String dir) {
    String result = substr(dir, 0, reverse_seek_slash(dir) + 1);
    return result;
}

yd_internal inline yd_b32
is_h(String extension) {
    yd_b32 result = (match(extension, "h") ||
                     match(extension, "hpp") ||
                     match(extension, "hin"));
    return result;
}

yd_internal inline yd_b32
is_c(String extension) {
    yd_b32 result = match(extension, "c");
    return result;
}

yd_internal inline yd_b32
is_cpp(String extension) {
    yd_b32 result = (match(extension, "cpp") ||
                     match(extension, "cc") ||
                     match(extension, "cin"));
    return result;
}

yd_internal inline yd_b32
is_objective_c(String extension) {
    yd_b32 result = (match(extension, "m") ||
                     match(extension, "mm"));
    return result;
}

yd_internal inline yd_b32
is_shader(String extension) {
    yd_b32 result = (match(extension, "ps") ||
                     match(extension, "vs") ||
                     match(extension, "cs") ||
                     match(extension, "ts") ||
                     match(extension, "gs"));
    return result;
}

yd_internal inline yd_b32
is_inl(String extension) {
    yd_b32 result = match(extension, "inl");
    return result;
}

yd_internal inline yd_b32
is_java(String extension) {
    yd_b32 result = match(extension, "java");
    return result;
}

yd_internal inline yd_b32
is_csharp(String extension) {
    yd_b32 result = match(extension, "cs");
    return result;
}

yd_internal inline yd_b32
is_python(String extension) {
    yd_b32 result = match(extension, "py");
    return result;
}

yd_internal inline yd_b32
is_swift(String extension) {
    yd_b32 result = match(extension, "swift");
    return result;
}

yd_internal inline yd_b32
is_javascript(String extension) {
    yd_b32 result = match(extension, "js");
    return result;
}

yd_internal inline yd_b32
is_bat(String extension) {
    yd_b32 result = match(extension, "bat");
    return result;
}

yd_internal inline yd_b32
is_bash(String extension) {
    yd_b32 result = match(extension, "sh");
    return result;
}

yd_internal inline yd_b32
is_txt(String extension) {
    yd_b32 result = match(extension, "txt");
    return result;
}

yd_internal inline yd_b32
is_code(String extension) {
    yd_b32 result = (is_h(extension) || is_c(extension) || is_cpp(extension) ||
                     is_objective_c(extension) || is_shader(extension) ||
                     is_inl(extension) || is_java(extension) || is_csharp(extension) ||
                     is_python(extension) || is_swift(extension) || is_javascript(extension) ||
                     is_bat(extension) || is_bash(extension));
    return result;
}

yd_internal inline yd_b32
is_doc(String extension) {
    yd_b32 result = is_txt(extension);
    return result;
}

yd_internal inline yd_b32
is_code_file(String filename) {
    String extension = file_extension(filename);
    yd_b32 result = is_code(extension);
    return result;
}

yd_internal inline yd_b32
is_doc_file(String filename) {
    String extension = file_extension(filename);
    yd_b32 result = is_doc(extension);
    return result;
}

#define YD_STRING
#endif // #if !defined(YD_STRING)

//
// NOTE(yuval): Exported Function Implementations
//

#if defined(YD_STRING_IMPLEMENTATION)

//
// NOTE(yuval): String Comparison Functions
//

yd_b32
match(const char* a, const char* b) {
    yd_b32 result = (a == b);
    
    if (a && b) {
        while (*a && *b && (*a == *b)) {
            ++a;
            ++b;
        }
        
        result = ((*a == 0) && (*b == 0));
    }
    
    return result;
}

yd_b32
match(String a, const char* b) {
    yd_b32 result = false;
    
    if (b) {
        const char* at = b;
        
        for (yd_umm index = 0; index < a.count; ++index, ++at) {
            if ((*at == 0) || (a.data[index] != *at)) {
                return false;
            }
        }
        
        result = (*at == 0);
    } else {
        result = (a.count == 0);
    }
    
    return result;
}

yd_b32
match(String a, String b) {
    yd_b32 result = (a.count == b.count);
    
    if (result) {
        for (yd_umm index = 0; index < a.count; ++index) {
            if (a.data[index] != b.data[index]) {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

yd_b32
match(const char* a, yd_umm a_count, const char* b) {
    yd_b32 result = false;
    
    if (b) {
        const char* at = b;
        
        for (yd_umm index = 0; index < a_count; ++index, ++at) {
            if ((*at == 0) || (a[index] != *at)) {
                return false;
            }
        }
        
        result = (*at == 0);
    } else {
        result = (a_count == 0);
    }
    
    return result;
}

yd_b32
match(const char* a, yd_umm a_count,
      const char* b, yd_umm b_count) {
    yd_b32 result = (a_count == b_count);
    
    if (result) {
        for (yd_umm index = 0; index < a_count; ++index) {
            if (a[index] != b[index]) {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

yd_b32
match(String a, const char* b, yd_umm count) {
    yd_b32 result = true;
    
    for (yd_umm index = 0; index < count; ++index) {
        if (a.data[index] != b[index]) {
            result = false;
            break;
        }
    }
    
    return result;
}

yd_b32
match_part(const char* a, const char* b, yd_umm* out_count) {
    yd_b32 result = (*a == *b);
    yd_umm match_count = 0;
    
    if (*a && *b) {
        while (*b && (*a == *b)) {
            ++a;
            ++b;
            ++match_count;
        }
        
        result = (*b == 0);
    }
    
    *out_count = match_count;
    return result;
}

yd_b32
match_part(String a, const char* b, yd_umm* out_count) {
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (b) {
        for (; b[index]; ++index) {
            if ((index == a.count) ||
                (a.data[index] != b[index])) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (a.count == 0);
    }
    
    *out_count = index;
    return result;
}

yd_b32
match_part(const char* a, String b, yd_umm* out_count) {
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (a)
    {
        for (; index < b.count; ++index) {
            if (a[index] != b.data[index]) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (b.count == 0);
    }
    
    *out_count = index;
    return result;
}

yd_b32
match_part(String a, String b, yd_umm* out_count) {
    yd_b32 result = (a.count >= b.count);
    yd_umm index = 0;
    
    if (result) {
        for (; index < b.count; ++index) {
            if (a.data[index] != b.data[index]) {
                result = false;
                break;
            }
        }
    }
    
    *out_count = index;
    return result;
}

yd_b32
match_insensitive(const char* a, const char* b) {
    yd_b32 result = (a == b);
    
    if (a && b) {
        while (*a && *b && (to_lower(*a) == to_lower(*b))) {
            ++a;
            ++b;
        }
        
        result = ((*a == 0) && (*b == 0));
    }
    
    return result;
}

yd_b32
match_insensitive(String a, const char* b) {
    yd_b32 result = false;
    
    if (b) {
        const char* at = b;
        
        for (yd_umm index = 0; index < a.count; ++index, ++at) {
            if ((*at == 0) || (to_lower(a.data[index]) != to_lower(*at))) {
                return false;
            }
        }
        
        result = (*at == 0);
    } else {
        result = (a.count == 0);
    }
    
    return result;
}

yd_b32
match_insensitive(String a, String b) {
    yd_b32 result = (a.count == b.count);
    
    if (result) {
        for (yd_umm index = 0; index < a.count; ++index) {
            if (to_lower(a.data[index]) != to_lower(b.data[index])) {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

yd_b32
match_part_insensitive(const char* a, const char* b, yd_umm* out_count) {
    yd_b32 result = (*a == *b);
    yd_umm match_count = 0;
    
    if (*a && *b) {
        while (*b && (to_lower(*a) == to_lower(*b))) {
            ++a;
            ++b;
            ++match_count;
        }
        
        result = (*a == 0);
    }
    
    *out_count = match_count;
    return result;
}

yd_b32
match_part_insensitive(String a, const char* b, yd_umm* out_count) {
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (b) {
        for (; b[index]; ++index) {
            if ((index == a.count) ||
                (to_lower(a.data[index]) != to_lower(b[index]))) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (a.count == 0);
    }
    
    *out_count = index;
    return result;
}
yd_b32
match_part_insensitive(const char* a, String b, yd_umm* out_count) {
    yd_b32 result = false;
    yd_umm index = 0;
    
    if (a) {
        for (; index < b.count; ++index) {
            if (to_lower(a[index]) != to_lower(b.data[index])) {
                return false;
            }
        }
        
        result = true;
    } else {
        result = (b.count == 0);
    }
    
    *out_count = index;
    return result;
}

yd_b32
match_part_insensitive(String a, String b, yd_umm* out_count) {
    yd_b32 result = (a.count >= b.count);
    yd_umm index = 0;
    
    if (result) {
        for (; index < b.count; ++index) {
            if (to_lower(a.data[index]) != to_lower(b.data[index])) {
                result = false;
                break;
            }
        }
    }
    
    *out_count = index;
    return result;
}

yd_b32
string_set_match(void* str_set, yd_umm item_size, yd_umm count,
                 String str, yd_umm* out_match_index) {
    yd_b32 result = false;
    yd_u8* at = (yd_u8*)str_set;
    
    for (yd_umm index = 0; index < count; ++index, at += item_size) {
        if (match(*((String*)at), str)) {
            *out_match_index = index;
            result = true;
            break;
        }
    }
    
    return result;
}

yd_s32
compare(const char* a, const char* b) {
    yd_umm index = 0;
    while (a[index] && b[index] &&
           (a[index] != b[index])) {
        ++index;
    }
    
    yd_s32 result = (a[index] > b[index]) - (a[index] < b[index]);
    return result;
}

yd_s32
compare(String a, const char* b) {
    yd_umm index = 0;
    while ((index < a.count) && b[index] &&
           (a.data[index] == b[index])) {
        ++index;
    }
    
    yd_s32 result = 0;
    
    if (index < a.count) {
        result = (a.data[index] > b[index]) - (a.data[index] < b[index]);
    } else {
        if (b[index]) {
            result = 0;
        } else {
            result = -1;
        }
    }
    
    return result;
}

yd_s32
compare(String a, String b) {
    yd_umm min_count = a.count;
    if (b.count < min_count) {
        min_count = b.count;
    }
    
    yd_umm index = 0;
    while ((index < min_count) && (a.data[index] == b.data[index])) {
        ++index;
    }
    
    yd_s32 result = 0;
    if (index < min_count) {
        result = (a.data[index] > b.data[index]) - (a.data[index] < b.data[index]);
    } else {
        result = (a.count > b.count) - (a.count < b.count);
    }
    
    return result;
}

//
// NOTE(yuval): Functions For Finding Characters And SubStrings
//

yd_umm
find(const char* str, char character, yd_umm start) {
    YD_ASSERT(start >= 0);
    
    for (yd_umm index = start; str[index]; ++index) {
        if (str[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find(String str, char character, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    for (yd_umm index = start; index < str.count; ++index) {
        if (str.data[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find(const char* str, String seek, yd_umm start) {
    YD_ASSERT(start >= 0);
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = start; str[index]; ++index) {
        yd_b32 hit = true;
        yd_umm str_index = index;
        
        for (yd_umm seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (!(str[str_index])) {
                return STRING_NOT_FOUND;
            }
            
            if (str[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find(String str, String seek, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    yd_umm stop_at = str.count - seek.count + 1;
    
    for (yd_umm index = start; index < stop_at; ++index) {
        yd_b32 hit = true;
        yd_umm str_index = index;
        
        for (yd_umm seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (str.data[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
rfind(const char* str, yd_umm count, char character, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < count));
    
    yd_umm index = start + 1;
    while (index--) {
        if (str[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
rfind(String str, char character, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    yd_umm index = start + 1;
    while (index--) {
        if (str.data[index] == character) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
rfind(const char* str, yd_umm count, String seek, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < count));
    
    if (count == 0) {
        return STRING_NOT_FOUND;
    }
    
    // TODO(yuval): This will overflow if seek's count is Bigger then str's count
    // An additional check is needed
    if (start + seek.count > count) {
        start = count - seek.count;
    }
    
    yd_umm index = start + 1;
    while (index--) {
        yd_b32 hit = true;
        yd_umm str_index = index;
        
        for (yd_umm seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (str[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
rfind(String str, String seek, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    // TODO(yuval): This will overflow if seek's count is Bigger then str's count
    // An additional check is needed
    if (start + seek.count > str.count) {
        start = str.count - seek.count;
    }
    
    yd_umm index = start + 1;
    while (index--) {
        yd_b32 hit = true;
        yd_umm str_index = index;
        
        for (yd_umm seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (str.data[str_index] != seek.data[seek_index]) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find_first_of(const char* str, const char* characters, yd_umm start) {
    YD_ASSERT(start >= 0);
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = start; str[index]; ++index) {
        for (const char* at = characters; *at; ++at) {
            if (str[index] == *at) {
                return index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find_first_of(String str, const char* characters, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    if (!(*characters)) {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = start; index < str.count; ++index) {
        for (const char* at = characters; *at; ++at) {
            if (str.data[index] == *at) {
                return index;
            }
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find_insensitive(const char* str, char character, yd_umm start) {
    YD_ASSERT(start >= 0);
    
    for (yd_umm index = start; str[index]; ++index) {
        if (to_lower(str[index]) == to_lower(character)) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find_insensitive(String str, char character, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    for (yd_umm index = start; index < str.count; ++index) {
        if (to_lower(str.data[index]) == to_lower(character)) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find_insensitive(const char* str, String seek, yd_umm start) {
    YD_ASSERT(start >= 0);
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    for (yd_umm index = start; str[index]; ++index) {
        yd_b32 hit = true;
        yd_umm str_index = index;
        
        for (yd_umm seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (to_lower(str[str_index]) != to_lower(seek.data[seek_index])) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

yd_umm
find_insensitive(String str, String seek, yd_umm start) {
    YD_ASSERT((start >= 0) && (start < str.count));
    
    if (seek.count == 0) {
        return STRING_NOT_FOUND;
    }
    
    yd_umm stop_at = str.count - seek.count + 1;
    
    for (yd_umm index = start; index < stop_at; ++index) {
        yd_b32 hit = true;
        yd_umm str_index = index;
        
        for (yd_umm seek_index = 0;
             seek_index < seek.count;
             ++seek_index, ++str_index) {
            if (to_lower(str.data[str_index]) != to_lower(seek.data[seek_index])) {
                hit = false;
                break;
            }
        }
        
        if (hit) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

//
// NOTE(yuval): String Slicing Functions
//

String
eat_leading_spaces(String str) {
    yd_umm skip_count = 0;
    for (; skip_count < str.count && is_whitespace(str.data[skip_count]); ++skip_count);
    
    String result = substr(str, skip_count);
    
    return result;
}

String
eat_trailing_spaces(String str) {
    yd_umm chop_index = str.count;
    for (; chop_index > 0 && is_whitespace(str.data[chop_index - 1]); --chop_index);
    
    String result = substr(str, 0, chop_index);
    return result;
}

String
consume_word(String* source) {
    String result = {};
    
    yd_umm pos0 = 0;
    for (; pos0 < source->count; ++pos0) {
        char c = source->data[pos0];
        if (!(is_whitespace(c) || (c == '(') || (c == ')'))) {
            break;
        }
    }
    
    if (pos0 < source->count) {
        yd_umm pos1 = pos0;
        
        for (; pos1 < source->count; ++pos1) {
            char c = source->data[pos1];
            if (is_whitespace(c) || c == '(' || c == ')') {
                break;
            }
        }
        
        result = substr(*source, pos0, pos1 - pos0);
        
        advance(source, pos1 + 1);
    } else {
        advance(source, source->count);
    }
    
    return result;
}

String
consume_line(String* source) {
    String result = {};
    
    if (!is_empty(*source)) {
        yd_umm pos = find(*source, '\n');
        result = substr(*source, 0, pos);
        
        advance(source, result.count + 1);
        
        if (result.count && (result[result.count - 1] == '\r')) {
            --result.count;
        }
    }
    
    return result;
}

String
consume_double_line(String* source) {
    String result = {};
    
    String nl_nl = BUNDLE_LITERAL("\n\n");
    yd_umm nl_nl_pos = find(*source, nl_nl);
    
    String rnl_rnl = BUNDLE_LITERAL("\r\n\r\n");
    yd_umm rnl_rnl_pos = find(*source, rnl_rnl);
    
    if ((nl_nl_pos == STRING_NOT_FOUND) &&
        (rnl_rnl_pos == STRING_NOT_FOUND)) {
        result = *source;
        advance(source, source->count);
    } else {
        yd_umm pos;
        yd_umm advance_count;
        if (nl_nl_pos < rnl_rnl_pos) {
            pos = nl_nl_pos;
            advance_count = nl_nl.count;
        } else {
            pos = rnl_rnl_pos;
            advance_count = rnl_rnl.count;
        }
        
        result = substr(*source, 0, pos);
        
        advance(source, result.count + advance_count);
    }
    return result;
}

//
// NOTE(yuval): String Copy And Append Functions
//

yd_umm
copy_fast_unsafe(char* dest, const char* source) {
    char* dest_at = dest;
    const char* source_at = source;
    
    while (*source_at) {
        *dest_at++ = *source_at++;
    }
    
    *dest_at = 0;
    
    yd_umm result = (dest_at - dest);
    return result;
}

yd_umm
copy_fast_unsafe(char* dest, String source)
{
    for (yd_umm index = 0; index < source.count; ++index) {
        dest[index] = source.data[index];
    }
    
    dest[source.count] = 0;
    return source.count;
}

yd_b32
copy_checked(String* dest, String source) {
    if (dest->capacity < source.count) {
        return false;
    }
    
    for (yd_umm index = 0; index < source.count; ++index) {
        dest->data[index] = source.data[index];
    }
    
    dest->count = source.count;
    return true;
}

yd_b32
copy_checked(char* dest, yd_umm dest_cap, String source) {
    if (dest_cap < source.count + 1) {
        return false;
    }
    
    for (yd_umm index = 0; index < source.count; ++index) {
        dest[index] = source.data[index];
    }
    
    dest[source.count] = 0;
    return true;
}

yd_b32
copy_partial(String* dest, const char* source) {
    yd_b32 result = true;
    yd_umm index = 0;
    
    for (; source[index]; ++index) {
        if (index >= dest->capacity) {
            result = false;
            break;
        }
        
        dest->data[index] = source[index];
    }
    
    dest->count = index;
    return result;
}

yd_b32
copy_partial(String* dest, String source) {
    yd_b32 result = true;
    yd_umm index = 0;
    
    for (; index < source.count; ++index) {
        if (index >= dest->capacity) {
            result = false;
            break;
        }
        
        dest->data[index] = source.data[index];
    }
    
    dest->count = index;
    return result;
}

yd_b32
copy_partial(char* dest, yd_umm dest_cap, String source) {
    yd_b32 result = true;
    yd_umm index = 0;
    
    for (; index < source.count; ++index) {
        if (index >= dest_cap - 1) {
            result = false;
            break;
        }
        
        dest[index] = source.data[index];
    }
    
    dest[index] = 0;
    return result;
}

yd_b32
append_checked(String* dest, String source) {
    String end = tailstr(*dest);
    yd_b32 result = copy_checked(&end, source);
    dest->count += end.count;
    return result;
}

yd_b32
append_partial(String* dest, const char* source) {
    String end = tailstr(*dest);
    yd_b32 result = copy_partial(&end, source);
    dest->count += end.count;
    return result;
}

yd_b32
append_partial(String* dest, String source) {
    String end = tailstr(*dest);
    yd_b32 result = copy_partial(&end, source);
    dest->count += end.count;
    return result;
}

yd_b32
append(String* dest, char c) {
    yd_b32 result = false;
    
    if (dest->count < dest->capacity) {
        dest->data[dest->count++] = c;
        result = true;
    }
    
    return result;
}

yd_b32
terminate_with_null(String* str) {
    yd_b32 result = false;
    
    if (str->count < str->capacity) {
        str->data[str->count] = 0;
        result = true;
    }
    
    return result;
}

yd_b32
append_padding(String* dest, char c, yd_umm target_count) {
    yd_b32 result = true;
    
    for (yd_umm count = dest->count; count < target_count; ++count) {
        if (!append(dest, c)) {
            result = false;
            break;
        }
    }
    
    return result;
}

//
// NOTE(yuval): String Concatenation
//

yd_b32
concat(String* dest, String source_a, String source_b) {
    yd_b32 can_fit_concat = (dest->capacity >= source_a.count + source_b.count);
    
    if (can_fit_concat) {
        yd_umm dest_index = 0;
        
        for (yd_umm source_index = 0;
             source_index < source_a.count;
             ++source_index, ++dest_index) {
            dest->data[dest_index] = source_a.data[source_index];
        }
        
        for (yd_umm source_index = 0;
             source_index < source_b.count;
             ++source_index, ++dest_index) {
            dest->data[dest_index] = source_b.data[source_index];
        }
        
        dest->count = source_a.count + source_b.count;
    }
    
    return can_fit_concat;
}

//
// NOTE(yuval): Other String Editing Functions
//

void
replace_range(String* str, yd_umm first, yd_umm one_past_last, char with) {
    YD_ASSERT((first >= 0) && (first < str->count));
    YD_ASSERT((one_past_last > 0) && (one_past_last <= str->count));
    YD_ASSERT(first < one_past_last);
    
    for (yd_umm index = first; index < one_past_last; ++index) {
        str->data[index] = with;
    }
}

void
replace_range(String* str, yd_umm first, yd_umm one_past_last, const char* with) {
    String with_str = make_string_slowly(with);
    replace_range(str, first, one_past_last, with_str);
}

// TODO(yuval): Maybe rename to block_copy?
yd_internal void
yd_string__block_move(void* dest_init, const void* source_init, yd_umm size)
{
    if (dest_init && source_init) {
        const yd_u8* source = (const yd_u8*)source_init;
        yd_u8* dest = (yd_u8*)dest_init;
        
        if (dest < source) {
            while (size--) {
                *dest++ = *source++;
            }
        } else if (dest > source) {
            source += size - 1;
            dest += size - 1;
            
            while (size--) {
                *dest-- = *source--;
            }
        }
    }
}

void
replace_range(String* str, yd_umm first, yd_umm one_past_last, String with) {
    YD_ASSERT((first >= 0) && (first < str->count));
    YD_ASSERT((one_past_last > 0) && (one_past_last <= str->count));
    YD_ASSERT(first < one_past_last);
    
    yd_s64 shift = with.count - (one_past_last - first);
    yd_umm new_count = str->count + shift;
    
    if (new_count <= str->capacity) {
        if (shift != 0) {
            char* tail = str->data + one_past_last;
            char* new_tail_pos = tail + shift;
            // TODO(yuval): Verify that this has no bugs!!!!!!!!!
            yd_string__block_move(new_tail_pos, tail, str->count - one_past_last);
        }
        
        yd_string__block_move(str->data + first, with.data, with.count);
        str->count += shift;
    }
}

void
replace(String* str, char to_replace, char with) {
    for (yd_umm index = 0; index < str->count; ++index) {
        if (str->data[index] == to_replace) {
            str->data[index] = with;
        }
    }
}

void
replace(String* str, const char* to_replace, const char* with) {
    replace(str, make_string_slowly(to_replace), make_string_slowly(with));
}

void
replace(String* str, const char* to_replace, String with) {
    replace(str, make_string_slowly(to_replace), with);
}

void
replace(String* str, String to_replace, const char* with) {
    replace(str, to_replace, make_string_slowly(with));
}

void
replace(String* str, String to_replace, String with) {
    yd_umm index = 0;
    
    for (;;) {
        index = find(*str, to_replace, index);
        if (index == STRING_NOT_FOUND) {
            break;
        }
        
        replace_range(str, index, index + to_replace.count, with);
        index += with.count;
    }
}

void
string_interpret_escapes(char* dest, String source) {
    yd_s32 mode = 0;
    yd_umm dest_index = 0;
    
    for (yd_umm source_index = 0; source_index < source.count; ++source_index) {
        switch (mode) {
            case 0: {
                if (source.data[source_index] == '\\') {
                    mode = 1;
                } else {
                    dest[dest_index++] = source.data[source_index];
                }
            } break;
            
            case 1: {
                char c = source.data[source_index];
                switch (c) {
                    case '\\': { dest[dest_index++] = '\\'; } break;
                    case 'n': { dest[dest_index++] = '\n'; } break;
                    case 't': { dest[dest_index++] = '\t'; } break;
                    case '"': { dest[dest_index++] = '"'; } break;
                    case '0': { dest[dest_index++] = '\0'; } break;
                    default: { dest[dest_index++] = '\\'; dest[dest_index++] = c; } break;
                }
                
                mode = 0;
            } break;
        }
    }
    
    dest[dest_index] = 0;
}

//
// NOTE(yuval): String Arena Allocation Functions
//

// TODO(yuval): This functions should all be inline!

#if defined(YD_MEMORY)
char*
push_z(yd_umm count, Arena_Push_Params params) {
    char* result = PUSH_ARRAY(char, count + 1, params);
    
    if (result) {
        result[count] = 0;
    }
    
    return result;
}

String
push_string(yd_umm capacity, Arena_Push_Params params) {
    String result = {};
    result.data = PUSH_ARRAY(char, capacity, params);
    result.count = 0;
    
    if (result.data) {
        result.capacity = capacity;
    }
    
    return result;
}

char*
push_copy_z(const char* source, Arena_Push_Params params) {
    yd_umm size = string_length(source);
    char* result = (char*)PUSH_COPY(source, size + 1, params);
    
    return result;
}

String
push_copy_string(const char* source, Arena_Push_Params params) {
    String result = {};
    
    yd_umm size = string_length(source);
    result.data = (char*)PUSH_COPY(source, size, params);
    
    if (result.data) {
        result.count = size;
        result.capacity = size;
    }
    
    return result;
}

char*
push_copy_z(String source, Arena_Push_Params params) {
    // TODO(yuval): Use PUSH_COPY
    char* result = PUSH_ARRAY(char, source.count + 1, params);
    
    if (result) {
        COPY_ARRAY(result, source.data, source.count);
        result[source.count] = 0;
    }
    
    return result;
}


String
push_copy_string(String source, Arena_Push_Params params) {
    String result = {};
    result.data = (char*)PUSH_COPY(source.data, source.count, params);
    
    if (result.data) {
        result.count = source.count;
        result.capacity = source.count;
    }
    
    return result;
}
#endif // #if defined(YD_MEMORY)

//
// NOTE(yuval): Case / Numeric Checking And Conversion Functions
//

void
to_lower(char* str) {
    for (char* at = str; *at; ++at) {
        *at = to_lower(*at);
    }
}

void
to_lower(String* str) {
    for (yd_umm index = 0; index < str->count; ++index) {
        str->data[index] = to_lower(str->data[index]);
    }
}

void
to_lower(char* dest, const char* source) {
    const char* source_at = source;
    char* dest_at = dest;
    
    while (*source_at) {
        *dest_at++ = to_lower(*source_at++);
    }
    
    *dest_at = 0;
}

void
to_lower(String* dest, const char* source) {
    yd_umm index = 0;
    
    for (; source[index]; ++index) {
        dest->data[index] = to_lower(source[index]);
    }
    
    dest->count = index;
}

void
to_lower(char* dest, String source) {
    for (yd_umm index = 0; index < source.count; ++index) {
        dest[index] = to_lower(source.data[index]);
    }
    
    dest[source.count] = 0;
}

void
to_lower(String* dest, String source) {
    if (dest->capacity >= source.count) {
        for (yd_umm index = 0; index < source.count; ++index) {
            dest->data[index] = to_lower(source.data[index]);
        }
        
        dest->count = source.count;
    }
}

void
to_upper(char* str) {
    for (char* at = str; *at; ++at) {
        *at = to_upper(*at);
    }
}

void
to_upper(String* str) {
    for (yd_umm index = 0; index < str->count; ++index) {
        str->data[index] = to_upper(str->data[index]);
    }
}

void
to_upper(char* dest, const char* source) {
    const char* source_at = source;
    char* dest_at = dest;
    
    while (*source_at) {
        *dest_at++ = to_upper(*source_at++);
    }
    
    *dest_at = 0;
}

void
to_upper(String* dest, const char* source) {
    yd_umm index = 0;
    
    for (; source[index]; ++index) {
        dest->data[index] = to_upper(source[index]);
    }
    
    dest->count = index;
}

void
to_upper(char* dest, String source) {
    for (yd_umm index = 0; index < source.count; ++index) {
        dest[index] = to_upper(source.data[index]);
    }
    
    dest[source.count] = 0;
}

void
to_upper(String* dest, String source) {
    if (dest->capacity >= source.count) {
        for (yd_umm index = 0; index < source.count; ++index) {
            dest->data[index] = to_upper(source.data[index]);
        }
        
        dest->count = source.count;
    }
}

void
to_camel(char* str) {
    yd_b32 is_first = true;
    
    for (char* at = str; *at; ++at) {
        if (is_alpha_numeric_true(*at)) {
            if (is_first) {
                *at = to_upper(*at);
                is_first = false;
            } else {
                *at = to_lower(*at);
            }
        } else {
            is_first = true;
        }
    }
}

void
to_camel(String* str) {
    yd_b32 is_first = true;
    
    for (yd_umm index = 0; index < str->count; ++index) {
        if (is_alpha_numeric_true(str->data[index])) {
            if (is_first) {
                str->data[index] = to_upper(str->data[index]);
                is_first = false;
            } else {
                str->data[index] = to_lower(str->data[index]);
            }
        } else {
            is_first = true;
        }
    }
}

void
to_camel(char* dest, const char* source) {
    const char* source_at = source;
    char* dest_at = dest;
    yd_b32 is_first = false;
    
    for (; *source_at; ++source_at, ++dest_at) {
        char c = *source_at;
        
        if (is_alpha_numeric_true(c)) {
            if (is_first) {
                c = to_upper(c);
                is_first = false;
            } else {
                c = to_lower(c);
            }
        } else {
            is_first = true;
        }
        
        *dest_at = c;
    }
    
    *dest_at = 0;
}

void
to_camel(String* dest, const char* source) {
    yd_umm index = 0;
    yd_b32 is_first = true;
    
    for (; source[index]; ++index) {
        char c = source[index];
        
        if (is_alpha_numeric_true(c)) {
            if (is_first) {
                c = to_upper(c);
                is_first = false;
            } else {
                c = to_lower(c);
            }
        } else {
            is_first = true;
        }
        
        dest->data[index] = c;
    }
    
    dest->count = index;
}

void
to_camel(char* dest, String source) {
    yd_b32 is_first = true;
    
    for (yd_umm index = 0; index < source.count; ++index) {
        char c = source.data[index];
        
        if (is_alpha_numeric_true(c)) {
            if (is_first) {
                c = to_upper(c);
                is_first = false;
            } else {
                c = to_lower(c);
            }
        } else {
            is_first = true;
        }
        
        dest[index] = c;
    }
    
    dest[source.count] = 0;
}

void
to_camel(String* dest, String source) {
    if (dest->capacity >= source.count) {
        yd_b32 is_first = true;
        
        for (yd_umm index = 0; index < source.count; ++index) {
            char c = source.data[index];
            
            if (is_alpha_numeric_true(c)) {
                if (is_first) {
                    c = to_upper(c);
                    is_first = false;
                } else {
                    c = to_lower(c);
                }
            } else {
                is_first = true;
            }
            
            dest->data[index] = c;
        }
        
        dest->count = source.count;
    }
}

yd_umm
to_string_count(yd_u64 value) {
    yd_umm count = 1;
    
    value /= 10;
    while (value != 0) {
        ++count;
        value /= 10;
    }
    
    return count;
}

yd_b32
to_string(String* dest, yd_u64 value) {
    yd_b32 result = false;
    yd_umm count = 0;
    
    do {
        if (count == dest->capacity) {
            result = false;
            break;
        }
        
        // TODO(yuval): Replace Hard-Coded Base
        yd_u64 digit = value % 10;
        char digit_char = (char)('0' + digit);
        dest->data[count++] = digit_char;
        value /= 10;
    } while (value != 0);
    
    if (result) {
        for (yd_umm start_index = 0, end_index = count - 1;
             start_index < end_index;
             ++start_index, --end_index) {
            char temp = dest->data[end_index];
            dest->data[end_index] = dest->data[start_index];
            dest->data[start_index] = temp;
        }
        
        dest->count = count;
    } else {
        dest->count = 0;
    }
    
    return result;
}

yd_b32
append(String* dest, yd_u64 value) {
    String tail = tailstr(*dest);
    yd_b32 result = to_string(&tail, value);
    
    if (result) {
        dest->count += tail.count;
    }
    
    return result;
}

U64_Conversion_Result
to_u64(const char* str) {
    U64_Conversion_Result result = {0, true};
    
    for (const char* at = str; *at; ++at) {
        if (is_numeric(*at)) {
            result.value *= 10;
            result.value += (*at - '0');
        } else {
            result.value = 0;
            result.success = false;
            break;
        }
    }
    
    return result;
}

U64_Conversion_Result
to_u64(String str) {
    U64_Conversion_Result result = {0, true};
    
    for (yd_umm index = 0; index < str.count; ++index) {
        if (is_numeric(str.data[index])) {
            result.value *= 10;
            result.value += (str.data[index] - '0');
        } else {
            result.value = 0;
            result.success = false;
            break;
        }
    }
    
    return result;
}

yd_umm
to_string_count(yd_s32 value) {
    yd_umm count = 1;
    
    if (value < 0) {
        count = 2;
    }
    
    value /= 10;
    while (value != 0) {
        ++count;
        value /= 10;
    }
    
    return value;
}

yd_b32
to_string(String* dest, yd_s32 value) {
    yd_b32 result = true;
    
    dest->count = 0;
    
    if (value < 0) {
        if (dest->capacity > 0) {
            dest->data[dest->count++] = '-';
            value = -value;
        } else {
            result = false;
        }
    }
    
    if (result) {
        result = append(dest, (yd_u64)value);
    }
    
    return result;
}

yd_b32
append(String* dest, yd_s32 value) {
    String tail = tailstr(*dest);
    yd_b32 result = to_string(&tail, value);
    
    if (result) {
        dest->count += tail.count;
    }
    
    return result;
}

yd_umm
to_string_count(yd_f32 value, yd_u32 max_precision) {
    yd_umm count = 0;
    
    if (value < 0) {
        count = 1;
        value = -value;
    }
    
    yd_u64 integer_part = (yd_u64)value;
    count += to_string_count(integer_part);
    
    value -= integer_part;
    
    for (yd_u32 precision_index = 0;
         precision_index < max_precision;
         ++precision_index) {
        if (value == 0.0f)
        {
            break;
        }
        
        value *= 10.0f;
        
        yd_u32 integer = (yd_u32)value;
        value -= integer;
        
        ++count;
    }
    
    return count;
}

yd_b32
to_string(String* dest, yd_f32 value, yd_u32 max_precision) {
    yd_s32 integer_part = (yd_s32)value;
    yd_b32 result = to_string(dest, integer_part);
    
    if (result) {
        value -= integer_part;
        
        if (value != 0.0f) {
            append(dest, '.');
            
            for (yd_u32 precision_index = 0;
                 precision_index < max_precision;
                 ++precision_index) {
                if (value == 0.0f) {
                    break;
                }
                
                value *= 10.0f;
                
                yd_u64 integer = (yd_u64)value;
                result = append(dest, integer);
                
                if (!result) {
                    break;
                }
                
                value -= integer;
            }
        }
    }
    
    return result;
}

yd_b32
append(String* dest, yd_f32 value, yd_u32 max_precision) {
    String tail = tailstr(*dest);
    yd_b32 result = to_string(&tail, value, max_precision);
    
    if (result) {
        dest->count += tail.count;
    }
    
    return result;
}

F32_Conversion_Result
to_f32(const char* str) {
    F32_Conversion_Result result = {};
    yd_b32 negative = false;
    
    if (str && (*str)) {
        result.success = true;
        
        const char* at = str;
        if (*at == '-') {
            negative = true;
            ++at;
        }
        
        for (; (*at) && (*at != '.'); ++at) {
            if (is_numeric(*at)) {
                result.value *= 10.0f;
                result.value += (yd_f32)(*at - '0');
            } else {
                result.value = 0.0f;
                result.success = false;
                break;
            }
        }
        
        if (result.success && (*at == '.')) {
            yd_f32 divisor = 10.0f;
            for (at = at + 1; (*at); ++at) {
                if (is_numeric(*at)) {
                    result.value += (((yd_f32)(*at - '0')) / divisor);
                    divisor *= 10.0f;
                } else {
                    result.value = 0.0f;
                    result.success = false;
                    break;
                }
            }
        }
        
        if (result.success && negative) {
            result.value = -result.value;
        }
    }
    
    return result;
}

F32_Conversion_Result
to_f32(String str) {
    F32_Conversion_Result result = {};
    yd_b32 negative = false;
    
    if (!is_empty(str)) {
        result.success = true;
        
        yd_umm index = 0;
        if (str[0] == '-') {
            negative = true;
            index = 1;
        }
        
        for (; (index < str.count) && (str[index] != '.'); ++index) {
            if (is_numeric(str[index])) {
                result.value *= 10.0f;
                result.value += (yd_f32)(str[index] - '0');
            } else {
                result.value = 0.0f;
                result.success = false;
                break;
            }
        }
        
        if (result.success && (index < str.count) && (str[index] == '.')) {
            yd_f32 divisor = 10.0f;
            for (index = index + 1; index < str.count; ++index) {
                if (is_numeric(str[index])) {
                    result.value += (((yd_f32)(str[index] - '0')) / divisor);
                    divisor *= 10.0f;
                } else {
                    result.value = 0.0f;
                    result.success = false;
                    break;
                }
            }
        }
        
        if (result.success && negative) {
            result.value = -result.value;
        }
    }
    
    return result;
}

yd_u32
hex_string_to_u32(String str) {
    yd_u32 result = 0;
    
    for (yd_umm index = 0; index < str.count; ++index) {
        result += hex_char_to_s32(str.data[index]);
        result *= 0x10;
    }
    
    return result;
}


yd_b32
color_to_hex_string(String* dest, yd_u32 color) {
    // TODO(yuval): Implement This
    return false;
}

yd_b32
hex_string_to_color(yd_u32* dest, String str) {
    // TODO(yuval): Implement This
    return false;
}

//
// NOTE(yuval): File / Directory Strings Management Functions
//

yd_umm
reverse_seek_slash(String str, yd_umm shift_from_last_char) {
    yd_umm index = str.count - shift_from_last_char - 1;
    while (index--) {
        if (is_slash(str.data[index])) {
            return index;
        }
    }
    
    return STRING_NOT_FOUND;
}

#if 0
yd_b32
set_last_folder(char* dir, yd_umm count, const char* folder_name, char slash) {
    yd_b32 result = false;
    yd_umm last_slash_index = reverse_seek_slash(dir, count);
    
    if (last_slash_index != STRING_NOT_FOUND)
    {
        yd_umm Newcount = last_slash_index + 1;
        Newcount = append(dir, count, folder_name));
        {
            if (append())
        }
    }
}
#endif // #if 0

yd_b32
set_last_folder(String* dir, const char* folder_name, char slash) {
    yd_b32 result = false;
    yd_umm last_slash_index = reverse_seek_slash(*dir);
    
    if (last_slash_index != STRING_NOT_FOUND) {
        yd_umm count = last_slash_index + 1;
        dir->count = count;
        
        if (append(dir, folder_name)) {
            if (append(dir, slash)) {
                result = true;
            }
        }
        
        if (!result) {
            dir->count = count;
        }
    }
    
    return result;
}

#if 0
yd_b32
set_last_folder(char* dir, yd_umm count, String folder_name, char slash) {
    
}
#endif // #if 0

yd_b32
set_last_folder(String* dir, String folder_name, char slash) {
    yd_b32 result = false;
    yd_umm last_slash_index = reverse_seek_slash(*dir);
    
    if (last_slash_index != STRING_NOT_FOUND) {
        yd_umm count = last_slash_index + 1;
        dir->count = count;
        
        if (append(dir, folder_name)) {
            if (append(dir, slash)) {
                result = true;
            }
        }
        
        if (!result) {
            dir->count = count;
        }
    }
    
    return result;
}

yd_b32
remove_last_folder(String* path) {
    yd_b32 result = false;
    yd_umm last_slash_index = reverse_seek_slash(*path, 1);
    
    if (last_slash_index != STRING_NOT_FOUND) {
        result = true;
        path->count = last_slash_index + 1;
    }
    
    return result;
}

String
file_extension(String filename) {
    String result = {};
    yd_umm dot_index = rfind(filename, '.');
    
    if (dot_index != STRING_NOT_FOUND) {
        result = make_string(filename.data + dot_index + 1,
                             filename.count - dot_index - 1);
    }
    
    return result;
}

yd_b32
set_extension(String* filename, const char* extension) {
    yd_b32 result = false;
    yd_umm last_dot_index = rfind(*filename, '.');
    
    if (last_dot_index != STRING_NOT_FOUND) {
        yd_umm count = last_dot_index;
        filename->count = count;
        
        if (append(filename, extension)) {
            result = true;
        }
        
        if (!result) {
            filename->count = count;
        }
    }
    
    return result;
}

yd_b32
set_extension(String* filename, String extension) {
    yd_b32 result = false;
    yd_umm last_dot_index = rfind(*filename, '.');
    
    if (last_dot_index != STRING_NOT_FOUND) {
        yd_umm count = last_dot_index + 1;
        filename->count = count;
        
        if (append(filename, extension)) {
            result = true;
        }
        
        if (!result) {
            filename->count = count - 1;
        }
    }
    
    return result;
}

yd_b32
remove_extension(String* filename) {
    yd_b32 result = false;
    yd_umm last_dot_index = rfind(*filename, '.');
    
    if (last_dot_index != STRING_NOT_FOUND) {
        result = true;
        filename->count = last_dot_index;
    }
    
    return result;
}

#endif // #if defined(YD_STRING_IMPLEMENTATION)
