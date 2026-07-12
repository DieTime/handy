/**
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: 2026 Denis Glazkov <glazzk.off@mail.ru>
 *
 * log.h v0.1.0 - simple single-header leveled logging for C.
 *
 * Description:
 *
 *    Header-only logger with four severity levels, colored output, and
 *    configurable stream, flushing and thread-safety behavior. No .c file
 *    needed -- just define the options you need and include the header.
 *
 * Usage:
 *
 *    #include <handy/log.h>
 *
 *    handy_log_d("starting socket listening: %s", socket_path);
 *    handy_log_i("started on port %d", port);
 *    handy_log_w("handshake takes too long: %d ms", time);
 *    handy_log_e("failed: %s", strerror(errno));
 *
 * Configuration [* = default]:
 *
 *    #define HANDY_LOG_STDIO=... - stream every log line is written to.
 *
 *       STDOUT  - write log lines to standard output.
 *       STDERR* - write log lines to standard error.
 *
 *    #define HANDY_LOG_LEVEL=... - minimum severity that gets logged.
 *
 *       DEBUG - log everything, including debug messages.
 *       INFO* - log info and above.
 *       WARN  - log warnings and errors only.
 *       ERROR - log errors only.
 *       NONE  - disable all logging.
 *
 *    #define HANDY_LOG_FLUSH=... - which severity levels get flushed immediately.
 *
 *       ALL     - flush the stream after every log line.
 *       ERRORS* - flush only after error-level lines.
 *       NONE    - never flush explicitly.
 *
 *    #define HANDY_LOG_THREAD_SAFETY=... - whether the stream is locked so lines don't interleave.
 *
 *       ENABLED   - acquire a lock around every log call.
 *       DISABLED* - no locking.
 *
 *    #define HANDY_PREFIX=... - whether public macros are also exposed without the `handy_` prefix.
 *
 *       KEEP* - keep the `handy_` prefix only.
 *       STRIP - also expose macros under their short names.
 *
 */

#ifndef HANDY_LOG_H
#define HANDY_LOG_H

#include "internal/common.h"
#include "internal/colors.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#ifndef HANDY_LOG_STDIO
#define HANDY_LOG_STDIO STDERR
#endif

#define HANDY_LOG_STDIO_STDOUT stdout
#define HANDY_LOG_STDIO_STDERR stderr

#define HANDY_LOG_STDIO_VALUE HANDY_CONCAT(HANDY_LOG_STDIO_, HANDY_LOG_STDIO)

#define HANDY_LOG_LEVEL_DEBUG 0
#define HANDY_LOG_LEVEL_INFO  1
#define HANDY_LOG_LEVEL_WARN  2
#define HANDY_LOG_LEVEL_ERROR 3
#define HANDY_LOG_LEVEL_NONE  4

#ifndef HANDY_LOG_LEVEL
#define HANDY_LOG_LEVEL INFO
#endif

#define HANDY_LOG_LEVEL_VALUE HANDY_CONCAT(HANDY_LOG_LEVEL_, HANDY_LOG_LEVEL)

#define HANDY_LOG_FLUSH_ALL    0
#define HANDY_LOG_FLUSH_ERRORS 1
#define HANDY_LOG_FLUSH_NONE   2

#ifndef HANDY_LOG_FLUSH
#define HANDY_LOG_FLUSH ERRORS
#endif

#define HANDY_LOG_FLUSH_VALUE HANDY_CONCAT(HANDY_LOG_FLUSH_, HANDY_LOG_FLUSH)

#ifndef HANDY_LOG_THREAD_SAFETY
#define HANDY_LOG_THREAD_SAFETY DISABLED
#endif

#define HANDY_LOG_THREAD_SAFETY_ENABLED  0
#define HANDY_LOG_THREAD_SAFETY_DISABLED 1

#define HANDY_LOG_THREAD_SAFETY_VALUE HANDY_CONCAT(HANDY_LOG_THREAD_SAFETY_, HANDY_LOG_THREAD_SAFETY)

#define HANDY_LOG_IMPL(level, tag, color_of_tag, color_of_text, perform_flush, fmt, ...) do {      \
    if (HANDY_LOG_LEVEL_VALUE <= (level)) {                                                        \
        bool enable_colors = isatty(fileno(HANDY_LOG_STDIO_VALUE));                                \
        bool thread_safety = HANDY_LOG_THREAD_SAFETY_VALUE == HANDY_LOG_THREAD_SAFETY_ENABLED;     \
                                                                                                   \
        if (thread_safety) { flockfile(HANDY_LOG_STDIO_VALUE); }                                   \
        fprintf(                                                                                   \
            HANDY_LOG_STDIO_VALUE,                                                                 \
            "%s[" tag "]%s %s" fmt "%s\n",                                                         \
            (enable_colors ? (color_of_tag) : ""),                                                 \
            (enable_colors ? HANDY_COLOR_RESET : ""),                                              \
            (enable_colors ? (color_of_text) : ""),                                                \
            ##__VA_ARGS__,                                                                         \
            (enable_colors ? HANDY_COLOR_RESET : "")                                               \
        );                                                                                         \
        if (perform_flush) { fflush(HANDY_LOG_STDIO_VALUE); }                                      \
        if (thread_safety) { funlockfile(HANDY_LOG_STDIO_VALUE); }                                 \
    }                                                                                              \
} while (0)

#define handy_log_d(fmt, ...) HANDY_LOG_IMPL(                                                      \
    HANDY_LOG_LEVEL_DEBUG,                                                                         \
    "~",                                                                                           \
    HANDY_COLOR_FAINT,                                                                             \
    HANDY_COLOR_FAINT,                                                                             \
    HANDY_LOG_FLUSH_VALUE == HANDY_LOG_FLUSH_ALL,                                                  \
    fmt, ##__VA_ARGS__                                                                             \
)

#define handy_log_i(fmt, ...) HANDY_LOG_IMPL(                                                      \
    HANDY_LOG_LEVEL_INFO,                                                                          \
    "*",                                                                                           \
    HANDY_COLOR_RESET,                                                                             \
    HANDY_COLOR_RESET,                                                                             \
    HANDY_LOG_FLUSH_VALUE == HANDY_LOG_FLUSH_ALL,                                                  \
    fmt, ##__VA_ARGS__                                                                             \
)

#define handy_log_w(fmt, ...) HANDY_LOG_IMPL(                                                      \
    HANDY_LOG_LEVEL_WARN,                                                                          \
    "?",                                                                                           \
    HANDY_COLOR_YELLOW,                                                                            \
    HANDY_COLOR_RESET,                                                                             \
    HANDY_LOG_FLUSH_VALUE == HANDY_LOG_FLUSH_ALL,                                                  \
    fmt, ##__VA_ARGS__                                                                             \
)

#define handy_log_e(fmt, ...) HANDY_LOG_IMPL(                                                      \
    HANDY_LOG_LEVEL_ERROR,                                                                         \
    "!",                                                                                           \
    HANDY_COLOR_RED,                                                                               \
    HANDY_COLOR_RESET,                                                                             \
    HANDY_LOG_FLUSH_VALUE == HANDY_LOG_FLUSH_ALL ||                                                \
    HANDY_LOG_FLUSH_VALUE == HANDY_LOG_FLUSH_ERRORS,                                               \
    fmt, ##__VA_ARGS__                                                                             \
)

#if HANDY_PREFIX_VALUE == HANDY_PREFIX_STRIP
    #define log_d handy_log_d
    #define log_i handy_log_i
    #define log_w handy_log_w
    #define log_e handy_log_e
#endif

#endif /* HANDY_LOG_H */
/**
 * SPDX-License-Identifier: ISC
 * SPDX-FileCopyrightText: 2026 Denis Glazkov <glazzk.off@mail.ru>
 */
