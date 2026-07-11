---
name: test-review
description: "Audit tests/<module>/*.cpp vs include/handy/<module>.h for coverage gaps + Yegor Bugayenko 'Angry Tests' adherence, as a skeptical senior C++ reviewer. Use to review/audit/critique a module's tests, before calling a suite done, or after a new config/public macro is added."
---

# test-review

Two failure modes: **missing scenarios** (header promises, nothing tests) and
**weak tests** (tested, but wouldn't catch a regression).

## When to use

- Review/audit a module's tests (e.g. "review the log tests").
- New `HANDY_<MODULE>_<OPTION>` or public macro needs coverage checked.
- Before treating a module's suite as done.

## Process

1. **Map public surface.** Read `include/handy/<module>.h` (doc comment =
   spec, per `header-doc` skill). List: every public macro/alias (incl.
   `HANDY_PREFIX`-stripped), every config macro's full value set + default,
   every behavior claim in `Description:`/`Configuration:` — each needs a
   test proving it, not just touching that path.
2. **Map existing coverage.** Per test file in `tests/<module>/`: config
   values compiled with, macros called, single claim verified. Build a
   mental option × value × interaction matrix.
3. **Hunt scenario gaps** (matrix vs. surface, report as "no test covers X
   when Y", never "add more tests"):
   - Untested values — every value needs ≥1 test, not just the extremes.
   - Untested interactions — two options whose combo plausibly changes
     behavior but is never exercised together.
   - Boundary conditions — at/above/below threshold, empty format strings,
     no variadic args, unusually long output.
   - Failure/negative paths — OS call failure (`fprintf`/`fflush`/`open`)
     behavior undocumented/untested.
   - "Does the flag do anything" test — toggle tested in both states, proving
     behavior actually differs, not just the `ENABLED` path alone.
   - Untested defaults — no test omits every `#define` to prove what a bare
     `#include` gets.
   - Cross-cutting options (e.g. `HANDY_PREFIX`) tested only at every other
     option's default, never alongside a non-default value.
4. **Angry Tests checklist** (Bugayenko, adapted to C++/GoogleTest):
   - No branching logic in a test (`if`, `try`/`catch`, loops beyond one
     obvious pass) — logic needs its own test to trust it.
   - One behavior per test — 4 unrelated assertions under 1 name means you'd
     need 4 names to say what failed.
   - Precise assertions, not loose — `EXPECT_NE(s.find(x), npos)` only proves
     presence; match the exact string/line the docs promise. Fail for
     exactly one reason.
   - No shared mutable fixtures — no `SetUp`/`TearDown`/reused static state;
     each test builds what it needs locally.
   - Names state a fact, not an action — `KeepsConcurrentLinesIntact` good,
     `TestThreadSafety`/`CheckFlush` bad.
   - No comments narrating *what* a test does — only *why* non-obvious
     system facts matter.
   - No silent failure tolerance — `EXPECT_*` where a failed precondition
     voids the rest (should be `ASSERT_*`), or over-aggressive `ASSERT_*`
     hiding other assertions in the same test.
5. **Review like a skeptical senior engineer, not a checklist bot.**
   Mentally delete/negate each behavior — would the test go red? Question
   whether a green suite proves the doc-comment promise or just today's
   implementation. Prefer a handful of sharp tests over many loose ones.
   Stay inside project conventions (GNU-only, one binary per config combo,
   `TempFile` RAII in `tests/log/temp_file.hpp`) — don't suggest mocks,
   shared fixtures, or portability shims that conflict with `CLAUDE.md`.

## Output

Rank findings most-important first: file/scenario, what's wrong/missing,
what real bug slips through, a concrete fix (not "add more coverage"). Use
`ReportFindings` tool if part of a structured review pass; plain text for an
ad hoc audit.
