/**
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: 2026 Denis Glazkov <glazzk.off@mail.ru>
 */

#ifndef HANDY_COMMON_H
#define HANDY_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define HANDY_CONCAT_IMPL(a, b) a ## b
#define HANDY_CONCAT(a, b) HANDY_CONCAT_IMPL(a, b)

#ifndef HANDY_PREFIX
#define HANDY_PREFIX KEEP
#endif

#define HANDY_PREFIX_KEEP  0
#define HANDY_PREFIX_STRIP 1

#define HANDY_PREFIX_VALUE HANDY_CONCAT(HANDY_PREFIX_, HANDY_PREFIX)

#ifdef __cplusplus
}
#endif

#endif /* HANDY_COMMON_H */
