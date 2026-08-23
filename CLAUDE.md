## What this is

Collection of single-header, header-only C libraries. Each module lives at
`include/handy/<name>.h`, no `.c` file needed - consumers configure via
`#define`s before including. GNU-only: freely uses glibc/GNU extensions, no
portability fallbacks, consumers must build against the GNU standard. Target
OS is Linux only for now - no macOS/BSD/Windows support, and no guards for
them either.

## Build, test, run

Build dir is always `.build`.

```sh
meson setup .build                               # first-time configure (examples build by default, tests don't)
meson setup .build -Dbuild_tests=true            # also configure with tests enabled
meson setup --reconfigure .build -Dbuild_tests=true  # re-configure an existing build dir
meson compile -C .build                          # build
meson test -C .build                             # run full test suite (only if configured with build_tests)
meson test -C .build log_format                  # run a single test by name
```

Examples build by default (`build_examples`, default `true` - no extra
dependency beyond the C compiler consumers already need). Tests are opt-in
(`build_tests`, default `false`) since they additionally require
`gtest`/`gtest_main`. System-wide GoogleTest is preferred (e.g.
`apt install libgtest-dev`); the `subprojects/gtest.wrap` file is a fallback
that meson downloads and builds automatically when no system package is found. Only
`subprojects/*.wrap` files are committed — extracted source directories and
`packagecache/` are gitignored.

Test binaries are named `<module>_<case>`, matching the `.cpp` file. Example
binaries build to `.build/examples/...`.

## Architecture

**Config mechanism:** each option is `#define HANDY_<MODULE>_<OPTION> <WORD>`
before including the header (`#ifndef` supplies a default). `HANDY_CONCAT(a, b)`
(indirection macro so args expand before pasting) turns the word into an
internal define: `HANDY_<MODULE>_<OPTION> <WORD>` → `HANDY_<MODULE>_<OPTION>_VALUE
== HANDY_<MODULE>_<OPTION>_<WORD>`.

**Config option placement:** the `#ifndef`/default/`_VALUE` block for every
public option must live together near the top of the file, grouped with the
other options and in the same order as the doc comment - a reader shouldn't
have to scroll past implementation to see what's configurable. If a piece of
an option's behavior can only be defined later (e.g. `PREFIX`'s aliasing must
follow the macros it aliases), split it: keep the option's definition/`_VALUE`
macros in the top config block, and leave only the unavoidable later part
where it's needed.

**`HANDY_<MODULE>_PREFIX`:** per-module switch. `KEEP` (default) keeps public
macros under `handy_<module>_`; `STRIP` also exposes short aliases (e.g.
`log_d` for `handy_log_d`). Defined in each module header, not in `internal/`.

**`include/handy/internal/`:** shared support headers used by multiple
modules but not meant to be included directly by consumers (`common.h`,
`colors.h`). Module headers reference them with a relative
`#include "internal/<name>.h"`.

**Tests are intentionally C++:** the library is C, but `tests/*.cpp` compile
against GoogleTest (wired per-module in `tests/meson.build`/`tests/<module>/meson.build`,
gated behind the `build_tests` option in the root `meson.build`). Each binary
links `gtest_main` directly, so no test file defines its own `main()`.

**One file per translation unit that actually needs one - not one per
behavior:** split into a separate `.cpp` only when a test needs its own
`#define HANDY_<MODULE>_<OPTION>` before including the header (those macros
resolve once per translation unit and can't vary within one - e.g. a
`STRIP`-prefix test needs its own file; a plain behavior check under
defaults doesn't), or when the test needs an incompatible harness/technique
(e.g. `tests/log/colored_output.cpp`'s PTY capture via `forkpty()` can't
share a file with `stdio_target.cpp`'s plain `freopen()` redirection, even
though both use the same config). Tests that share both config and
technique - which is most of them - belong together as multiple `TEST()`
cases in one file (e.g. `defaults.cpp`), not split out one-per-behavior.
Splitting a test into its own file when neither its config nor its
technique requires it is unnecessary ceremony - don't do it just because
the behavior has a name.

**Design principles for any new module/logic:**
- Simple, readable code over faster-but-more-complex.
- Small, intuitive, guessable public API.
- GNU/glibc only - no portability guards; use GNU extensions freely, consumers
  must build against GNU.
- Boolean config macros: `ENABLED`/`DISABLED` values with a verb/gerund option
  name. Multi-value "mode" options: noun-phrase name with mode words as values.
- Types (typedefs, struct tags meant to be used as a type) are named C-style:
  `snake_case` with a `_t` suffix on typedef'd names (e.g. `opt_int_t`). Plain
  struct tags not typedef'd stay `snake_case` without `_t` (e.g. `struct
  config`). Applies in test/example code too, not just headers.

**`examples/`** are the canonical usage demonstration of the public API. When
any public config macro is added, renamed, or removed, update the matching
example file alongside the header and tests.

**Adding a new module** — checklist of files to create/update:
1. `include/handy/<name>.h` — the header itself.
2. `examples/<name>.c` — demonstrates every public config macro.
3. `tests/<name>/` — test directory with its own `meson.build` and `*.cpp` files.
4. `tests/meson.build` — add `subdir('<name>')`.
5. Root `meson.build` — nothing to change (tests subdir is already wired in).

**Header doc comments:** every public header starts with a documentation
comment. Use the `header-doc` skill to write/update these. The Description
section must stay technically succinct and structured for human
readability - short, single-topic paragraphs (or a short sub-list for a
handful of alternatives), not one dense paragraph carrying every behavior
and caveat at once. The Usage section is a quick-glance cheat sheet, not a
worked example - one call per public macro, grouped with blank lines by
related cluster (e.g. the two constructors, then the accessors), no helper
functions or printed output (that belongs in `examples/<module>.c`); any
inline struct/function bodies use normal multi-line C formatting, never
condensed onto one line; trailing comments get one space before `//`, no
padding to align a column unless several similar lines are genuinely
grouped.

**No auto-formatters:** no clang-format or similar tool is used in this
project. Formatting is by hand, per the author's own sense of what looks
clean - match the existing style rather than reformatting to a tool's output.

## Commits

- Conventional Commits, scoped to the module: `fix(log): ...`, `feat(log): ...`.
- No agent co-author trailer.
- Body only when genuinely needed, and then short and to the point - no
  padding, no restating the diff.

## License

ISC. Every source file starts with an SPDX header (`SPDX-License-Identifier: ISC`,
`SPDX-FileCopyrightText: <year> <author> <email>`) - bump the year when you touch a file.
