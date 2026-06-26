# Contributing to TurkOS

TurkOS holds contributions to a strict standard. This is a kernel project — sloppy code here means real security and stability consequences. Read this fully before opening a Pull Request.

## Before You Start

1. Search existing issues and PRs first. Don't duplicate work.
2. For anything beyond a trivial fix, open an issue describing your proposal before writing code. Large unsolicited PRs without prior discussion are likely to be rejected regardless of code quality.
3. Read the relevant existing source files in the area you're touching. Match the existing architecture (the self-auditing bus pattern in `çekirdek/denetim/`, the watcher registration model, etc.) rather than introducing a parallel approach.

## Code Standards (Non-Negotiable)

- **No inline comments in source code.** One bilingual (Turkish/English) copyright header per file is the only comment permitted. If your code needs a comment to be understood, restructure the code or rename your identifiers until it doesn't.
- **No emojis** anywhere — code, commit messages, PR descriptions, documentation.
- **Identifiers in Öz Türkçe** (function names, variable names, struct/type names) — match the existing project convention. Public-facing docs (README, this file) are English-only; code comments/commits are English per the rules below.
- **Commit messages and PR descriptions in English**, unless explicitly discussing something Turkish-specific.
- **No third-party libraries or tools.** TurkOS is built from scratch. If you think an exception is needed, ask first.
- Clean, compact code: no unnecessary blank lines, no dead code, no commented-out blocks.
- Match existing layering: drivers / kernel / memory / boot stay separated the way the existing tree separates them.
- Every new subsystem that can detect a fault condition should report it to `denetim_otobusu` (the self-auditing bus) rather than failing silently or printing directly.

## What Gets Rejected Immediately

- Any code that doesn't compile with `./batuss-derle.sh`.
- Any code introducing a third-party dependency.
- Any PR that does not explain *why* the change is needed, not just *what* it does.
- Any PR touching security-sensitive code (`çekirdek/denetim/`, paging, ring0/ring3 boundary, syscalls) without a clear explanation of the threat model addressed and how it was tested.
- Any PR that doesn't pass automated CI (see below).
- Any PR that introduces a regression in existing watcher behavior without discussion.

## Required Testing

For any change touching kernel logic (not pure documentation):

1. Run `./batuss-derle.sh --calistir` and confirm the system boots without a crash or unexpected panic.
2. If your change affects the self-auditing bus or a watcher, describe in the PR how you triggered the condition in QEMU and what log output confirmed correct behavior (BILGI/SUPHELI/ALARM sequence, lockdown trigger, etc.).
3. Describe edge cases you considered (what happens under repeated/burst conditions, what happens if your code runs in an interrupt context, etc.).

## Automated Review

Every Pull Request is checked automatically by CI before any human review happens:

- Full build via `batuss-derle.sh` must succeed with zero errors.
- A static scan rejects PRs containing inline comments, emoji, or non-UTF-8-safe paths.
- A QEMU smoke test boots the resulting kernel and checks for a clean startup sequence.

A PR that fails any of these checks will not be reviewed until it's fixed. Passing CI does not guarantee merge — it is the minimum bar to be considered. Final review and merge decisions are made manually by the maintainer (Batuhan ALGÜL / Batuss).

## License of Contributions

By submitting a Pull Request, you agree that your contribution is licensed under the terms in [LICENSE.md](LICENSE.md) / [LICENSE.en.md](LICENSE.en.md), and that Batuss may use, modify, and distribute your contribution as part of TurkOS without further compensation or attribution beyond what Git history already provides.
