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
meson setup .build                                          # first-time configure (nothing builds by default)
meson setup .build -Dbuild_examples=true -Dbuild_tests=true # configure with examples and tests enabled
meson compile -C .build                                     # build
meson test -C .build                                        # run full test suite (only if configured with build_tests)
meson test -C .build log_format                             # run a single test by name
```

Both examples and tests are opt-in (`-Dbuild_examples=true`, `-Dbuild_tests=true`)
- a bare `meson setup .build` builds nothing but the header-only `include/`
subdir. Tests additionally require `gtest`/`gtest_main` to be found on the
system via pkg-config - there's no bundled/wrap fallback, so configuring with
`build_tests=true` fails hard if GoogleTest isn't installed (e.g.
`apt install libgtest-dev`). Test binaries are named `<module>_<case>`,
matching the `.cpp` file. Example binaries build to `.build/examples/...`.

## Architecture

**Config mechanism (`common.h`):** each option is `#define HANDY_<MODULE>_<OPTION> <WORD>`
before including the header (`#ifndef` supplies a default). `HANDY_CONCAT(a, b)`
(indirection macro so args expand before pasting) turns the word into an
internal define: `HANDY_<MODULE>_<OPTION> <WORD>` → `HANDY_<MODULE>_<OPTION>_VALUE
== HANDY_<MODULE>_<OPTION>_<WORD>`.

**`HANDY_PREFIX` (`common.h`):** library-wide switch. `KEEP` (default) keeps
public macros under `handy_`; `STRIP` also exposes a short alias for each.
Every module's public macros must respect it.

**Tests are intentionally C++:** the library is C, but `tests/*.cpp` compile
against GoogleTest (wired per-module in `tests/meson.build`/`tests/<module>/meson.build`,
gated behind the `build_tests` option in the root `meson.build`). Each file
`#define`s its own config then includes the header fresh - one binary per
config/behavior combo, since `HANDY_*` macros resolve once per translation
unit and can't vary within one. Each binary links `gtest_main` directly, so no
test file defines its own `main()`.

**Design principles for any new module/logic:**
- Simple, readable code over faster-but-more-complex.
- Small, intuitive, guessable public API.
- GNU/glibc only - no portability guards; use GNU extensions freely, consumers
  must build against GNU.
- Boolean config macros: `ENABLED`/`DISABLED` values with a verb/gerund option
  name. Multi-value "mode" options: noun-phrase name with mode words as values.

**Header doc comments:** every public header starts with a documentation
comment. Use the `header-doc` skill to write/update these.

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
