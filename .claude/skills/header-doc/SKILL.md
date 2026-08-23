---
name: header-doc
description: "Write/update the top-of-file doc comment for a handy public header (include/handy/*.h): SPDX block, one-line title, Description/Usage/Configuration sections, matching include/handy/log.h."
---

# header-doc

Plain-text (non-Doxygen) top-of-file doc comment for `handy` headers, per `include/handy/log.h`.

## When to use

- New module header under `include/handy/`.
- Header has public macros/functions, a usage example, or `#define` config options.
- No config/usage (e.g. `internal/colors.h`, `internal/common.h`) → minimal variant only, don't force the full template.

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
 *    needed"). Wrapped, indented 3 spaces after the `* `. If the header has
 *    more than one distinct non-obvious behavior/constraint, split into
 *    several short single-topic paragraphs instead of one dense paragraph -
 *    see "Structuring a longer Description" below.>
 *
 * Usage:
 *
 *    #include <handy/<filename>.h>
 *
 *    <one example call per public macro/function, in a realistic order -
 *    see "Keeping Usage succinct" below>
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

Include guard follows immediately, no blank line skipped (see log.h). Nothing
is repeated at EOF after `#endif` - the top-of-file comment is the only copy.

## Minimal variant

No usage/config to show (`internal/colors.h`, `internal/common.h`) → only
this at the top:

```c
/**
 * SPDX-License-Identifier: <project license>
 * SPDX-FileCopyrightText: <year> <author name> <author email>
 */
```

## Structuring a longer Description

Default to one 2-4 line paragraph (see log.h). Reach for more structure only
when the header genuinely has multiple distinct things a reader must know -
e.g. more than one non-obvious behavior, or more than one way to use the
API with a real tradeoff between them. When that happens:

- Split by topic, one short paragraph each (blank `*` line between), not one
  paragraph that runs every behavior/caveat together.
- Each paragraph should be independently scannable - a reader skimming just
  that paragraph gets one complete idea, not a fragment of a longer one.
- A short indented sub-list (3 extra spaces) is fine for enumerating a
  handful of alternatives with a real tradeoff (e.g. two ways to construct a
  value) - keep each item to 2-3 lines.
- Still technically succinct: cut restating the obvious (identifier names,
  what a line of code already shows) and any wording that doesn't change
  what the reader does differently.
- See `include/handy/optional.h`'s Description for a worked multi-paragraph
  example (what it is / the unsafe-field-and-abort mechanic / two
  construction styles as a sub-list / the double-evaluation caveat).

## Keeping Usage succinct

Usage is a quick-glance cheat sheet, not a worked example - one example call
per public macro/function, in a realistic order, nothing more. The fuller,
realistic demonstration (helper functions, populated structs, printed
output) belongs in `examples/<module>.c` per CLAUDE.md, not the doc
comment - don't duplicate it here.

- Group related calls with a blank `*` line between clusters (e.g. the two
  constructors, then the accessor calls), not one unbroken block - mirrors
  how a longer Description splits by topic.
- Trailing comments: one space before `//`, short and plain. Don't pad
  whitespace to align a comment column unless several similar lines are
  genuinely grouped together - a one-off comment doesn't need alignment.
- Multi-line constructs shown inline (a struct definition, a function body)
  use normal C formatting - opening brace ends the line, body indented,
  closing brace alone - never condensed onto one line, matching the rest of
  the codebase's style.
- If Usage is growing past roughly 15-20 lines, or needs control flow or a
  helper function to make sense, that content belongs in
  `examples/<module>.c` instead - trim Usage back to the flat call list.
- See `include/handy/optional.h`'s Usage for a worked example (grouped
  constructor pairs, no printf/helper-function narrative).

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

`include/handy/log.h` lines 1-53 (full, single-paragraph Description);
`include/handy/optional.h` lines 1-63 (full, multi-paragraph Description and
grouped Usage - see "Structuring a longer Description" and "Keeping Usage
succinct" above); `include/handy/internal/colors.h` /
`include/handy/internal/common.h` lines 1-4 (minimal).
