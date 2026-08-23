/**
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: 2026 Denis Glazkov <glazzk.off@mail.ru>
 *
 * optional.h v0.1.0 - generic nullable wrapper type for C.
 *
 * Description:
 *
 *    Header-only optional/maybe type, in the spirit of Rust's Option and
 *    C++'s std::optional. C has no generics, so `handy_optional_t(T)` is a
 *    macro expanding to an anonymous struct -- spell it inline wherever a
 *    type is expected (struct field, local variable, typedef). Values are
 *    stored inline, no heap allocation, no .c file needed.
 *
 *    Raw fields sit under a member named `unsafe`, so bypassing the
 *    accessor macros is always visible at the call site.
 *    handy_optional_unwrap() aborts on a value-less optional instead of
 *    returning garbage; handy_optional_unwrap_or() is the explicit-fallback
 *    alternative.
 *
 *    Two ways to build a value:
 *
 *       handy_optional_some()/_none() are real expressions, usable
 *       anywhere (reassignment, return, arguments) -- but need a type
 *       name, since a bare handy_optional_t(T) written twice is two
 *       distinct, incompatible types.
 *
 *       handy_optional_init_some()/_none() need no type name, but
 *       only work as a declaration's initializer (`Type x = ...;`), not
 *       as a general expression.
 *
 *    handy_optional_unwrap()/_unwrap_or() evaluate opt more than once --
 *    pass a plain variable or field, not an expression with side effects.
 *
 * Usage:
 *
 *    #include <handy/optional.h>
 *
 *    handy_optional_t(int) x = handy_optional_init_some(5); // no type name needed
 *    handy_optional_t(int) y = handy_optional_init_none();
 *
 *    typedef handy_optional_t(int) my_opt_t;
 *    my_opt_t a = handy_optional_some(my_opt_t, 5);
 *    my_opt_t b = handy_optional_none(my_opt_t);
 *
 *    bool has_a = handy_optional_is_some(a);
 *    bool no_b = handy_optional_is_none(b);
 *
 *    int va = handy_optional_unwrap(a); // aborts if a is none
 *    int vb = handy_optional_unwrap_or(b, -1);
 *
 *    struct config {
 *        handy_optional_t(const char *) name; // works as a field too
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
