---
name: header-doc
description: "Write/update the top-of-file doc comment for a handy public header (include/handy/*.h): SPDX block, one-line title, Description/Usage/Configuration sections, matching include/handy/log.h."
---

# header-doc

Plain-text (non-Doxygen) top-of-file doc comment for `handy` headers, per `include/handy/log.h`.

## When to use

- New module header under `include/handy/`.
- Header has public macros/functions, a usage example, or `#define` config options.
- No config/usage (e.g. `colors.h`, `common.h`) → minimal variant only, don't force the full template.

## Full template

```c
/**
 * SPDX-License-Identifier: <project license>
 * SPDX-FileCopyrightText: <year> <author name> <author email>
 *
 * <filename> v<major>.<minor>.<patch> - <one-line description, lowercase, ending in a period>.
 *
 * Description:
 *
 *    <2-4 line prose paragraph. What the header provides, its key behaviors,
 *    and anything the reader must know before using it (e.g. "no .c file
 *    needed"). Wrapped, indented 3 spaces after the `* `.>
 *
 * Usage:
 *
 *    #include <handy/<filename>.h>
 *
 *    <one example call per public macro/function, in a realistic order>
 *
 * Configuration [* = default]:
 *
 *    #define HANDY_<OPTION>=... - <lowercase description, ending in a period>.
 *
 *       <VALUE1>  - <lowercase description, ending in a period>.
 *       <VALUE2>* - <lowercase description, ending in a period>.
 *
 *    #define HANDY_<OPTION2>=... - <lowercase description, ending in a period>.
 *
 *       <VALUE1>* - <lowercase description, ending in a period>.
 *       <VALUE2>  - <lowercase description, ending in a period>.
 *
 */
```

Include guard follows immediately, no blank line skipped (see log.h). At EOF
after `#endif`, repeat a license-only block:

```c
/**
 * SPDX-License-Identifier: <project license>
 * SPDX-FileCopyrightText: <year> <author name> <author email>
 */
```

## Minimal variant

No usage/config to show (`colors.h`, `common.h`) → only this at the top,
no trailing duplicate:

```c
/**
 * SPDX-License-Identifier: <project license>
 * SPDX-FileCopyrightText: <year> <author name> <author email>
 */
```

## Formatting rules

- Title: `<filename> v<semver> - <lowercase description>.`, matches current version.
- Section headers (`Description:`/`Usage:`/`Configuration [* = default]:`) capitalized,
  blank `*` line then indented body. Omit Configuration if no `#define` options.
- Each `#define` option: `#define HANDY_OPTION=... - <lowercase description>.` +
  blank line + indented accepted-values list.
- Accepted values: one per line, indented 3 extra spaces, names padded so `-`
  separators align in a column, lowercase descriptions ending in a period.
- Default value gets a literal `*` right after its name, no space (`STDERR*`,
  `INFO*`, `ERRORS*`, `DISABLED*`, `KEEP*`) - no other marker.
- Boolean (2-value) options: verb/gerund macro names with `ENABLED`/`DISABLED`,
  not `ON`/`OFF` (e.g. `HANDY_LOG_THREAD_SAFETY`). Keep whatever naming the
  macro itself already uses.
- Blank `*` line between every section and every option paragraph, always.
- No Doxygen tags (`@file`/`@brief`/`@def`/`@code`/`@ref`) - plain prose only,
  regardless of older notes.

## Worked example

`include/handy/log.h` lines 1-53 (full); `include/handy/colors.h` /
`include/handy/common.h` lines 1-4 (minimal).
