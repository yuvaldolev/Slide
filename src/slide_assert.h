#if !defined(SLIDE_ASSERT_H)

#if SLIDE_DEVELOPER
// TODO(yuval): Temporary
#include <stdio.h>

#define ASSERT_(expression, should_crash) \
if (!(expression)) { \
    char message_[256]; \
    snprintf(message_, sizeof(message_), "Assertion Failure: %s at %s:%s:%d", \
    #expression, __FILE__, __FUNCTION__, __LINE__); \
    platform.display_message_box("ASSERSION FAILURE", message_); \
    if (should_crash) { \
        *(volatile int*)0 = 0; \
    } \
}

#define ASSERT(expression) ASSERT_((expression), true)
#define SOFT_ASSERT(expression) ASSERT_((expression), false)

#else

#define ASSERT(expression)
#define SOFT_ASSERT(expression)
#endif // #if SLIDE_DEVELOPER

#define INVALID_CODE_PATH(...) ASSERT(!"Invalid Code Path")
#define INVALID_DEFAULT_CASE(...) default: {INVALID_CODE_PATH();} break

#define SLIDE_ASSERT_H
#endif // #if !defined(SLIDE_ASSERT_H)