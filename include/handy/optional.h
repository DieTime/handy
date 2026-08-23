/**
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: 2026 Denis Glazkov <glazzk.off@mail.ru>
 *
 * optional.h v0.1.0 - generic nullable wrapper type for C.
 *
 * Description:
 *
 *    Header-only optional type for C with convenient macros for working with it.
 *    No .c file needed -- just define the options you need and include the header.
 *
 * Usage:
 *
 *    #include <handy/optional.h>
 *
 *    handy_optional_t(int) x = handy_optional_init_some(5);
 *    handy_optional_t(int) y = handy_optional_init_none();
 *
 *    typedef handy_optional_t(int) my_opt_t;
 *
 *    my_opt_t a = handy_optional_some(my_opt_t, 5);
 *    my_opt_t b = handy_optional_none(my_opt_t);
 *
 *    my_opt_t divide(int a, int b) {
 *        return b == 0 ? handy_optional_none(my_opt_t) : handy_optional_some(my_opt_t, a / b);
 *    }
 *
 *    bool has_a = handy_optional_is_some(a);
 *    bool no_b = handy_optional_is_none(b);
 *
 *    int va = handy_optional_unwrap(a);
 *    int vb = handy_optional_unwrap_or(b, -1);
 *
 *    struct config {
 *        handy_optional_t(const char *) name;
 *    };
 *
 * Configuration [* = default]:
 *
 *    #define HANDY_OPTIONAL_PREFIX=... - whether public macros are also exposed without the `handy_optional_` prefix.
 *
 *       KEEP* - keep the `handy_optional_` prefix only.
 *       STRIP - also expose macros under their short `optional_` names.
 *
 */

#ifndef HANDY_OPTIONAL_H
#define HANDY_OPTIONAL_H

#include "internal/common.h"

#include <stdbool.h>
#include <stdlib.h>

#ifndef HANDY_OPTIONAL_PREFIX
#define HANDY_OPTIONAL_PREFIX KEEP
#endif

#define HANDY_OPTIONAL_PREFIX_KEEP  0
#define HANDY_OPTIONAL_PREFIX_STRIP 1

#define HANDY_OPTIONAL_PREFIX_VALUE HANDY_CONCAT(HANDY_OPTIONAL_PREFIX_, HANDY_OPTIONAL_PREFIX)

#define handy_optional_t(T)                                                                        \
    struct {                                                                                       \
        struct {                                                                                   \
            bool is_some;                                                                          \
            __typeof__(T) value;                                                                   \
        } unsafe;                                                                                  \
    }

#define handy_optional_some(T, val)   ((T){ .unsafe = { .is_some = true, .value = (val) } })
#define handy_optional_none(T)        ((T){ .unsafe = { .is_some = false } })
#define handy_optional_init_some(val) { .unsafe = { .is_some = true, .value = (val) } }
#define handy_optional_init_none()    { .unsafe = { .is_some = false } }
#define handy_optional_is_some(opt)   ((opt).unsafe.is_some)
#define handy_optional_is_none(opt)   (!handy_optional_is_some(opt))

#define handy_optional_unwrap(opt) (                                                               \
    handy_optional_is_some(opt) ?                                                                  \
        (opt).unsafe.value :                                                                       \
        (abort(), (opt).unsafe.value)                                                              \
)

#define handy_optional_unwrap_or(opt, default_value) (                                             \
    handy_optional_is_some(opt) ?                                                                  \
        (opt).unsafe.value :                                                                       \
        (default_value)                                                                            \
)

#if HANDY_OPTIONAL_PREFIX_VALUE == HANDY_OPTIONAL_PREFIX_STRIP
    #define optional_t         handy_optional_t
    #define optional_some      handy_optional_some
    #define optional_none      handy_optional_none
    #define optional_init_some handy_optional_init_some
    #define optional_init_none handy_optional_init_none
    #define optional_is_some   handy_optional_is_some
    #define optional_is_none   handy_optional_is_none
    #define optional_unwrap    handy_optional_unwrap
    #define optional_unwrap_or handy_optional_unwrap_or
#endif

#endif /* HANDY_OPTIONAL_H */
