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
 *    <one short paragraph, 2-4 sentences. What the header provides and the
 *    one or two behaviors a reader most needs before using it (e.g. "no .c
 *    file needed"). Wrapped, indented 3 spaces after the `* `. See "Keeping
 *    Description short" below - this is not the place for an exhaustive
 *    explanation.>
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

## Keeping Description short

Always one short paragraph, 2-4 sentences (see log.h and optional.h) - not
an essay explaining everything about the header. State only what it is and
the one or two behaviors a reader most needs before writing code against
it; leave the rest to be picked up from Usage's examples and the code's own
naming.

- One paragraph, no sub-lists, no multi-paragraph breakdown by topic - if a
  header seems to need that much structure to explain itself, that's a sign
  to cut content, not to add more sections.
- Cut restating the obvious (identifier names, what a line of code already
  shows) and any wording that doesn't change what the reader does
  differently.
- A genuinely non-obvious constraint (e.g. "evaluates its argument twice")
  is worth one short clause tacked onto an existing sentence, not its own
  paragraph.

## Keeping Usage succinct

Usage is a quick-glance cheat sheet, not a worked example - one example call
per public macro/function, in a realistic order, nothing more. The fuller,
realistic demonstration (helper functions, populated structs, printed
output) belongs in `examples/<module>.c` per CLAUDE.md, not the doc
comment - don't duplicate it here.

- Group related calls with a blank `*` line between clusters (e.g. the two
  constructors, then the accessor calls), not one unbroken block.
- No trailing `//` comments on Usage lines - identifier names plus the
  Description already carry the meaning; a comment restating "aborts if
  none" or "no type name needed" next to a self-explanatory call is noise.
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

`include/handy/log.h` lines 1-53 and `include/handy/optional.h` lines 1-45
(full, both single-paragraph Description, optional.h's Usage also showing
the grouped-clusters style - see "Keeping Usage succinct" above);
`include/handy/internal/colors.h` / `include/handy/internal/common.h`
lines 1-4 (minimal).
