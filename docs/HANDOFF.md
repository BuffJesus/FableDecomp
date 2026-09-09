# Resume here (one page) — updated 2026-09-08

## State
- Shared working branch: `feat/novi-script-recovery`; native parity checkpoints through `39538ff` and subsequent NewOakValeIntro work are present locally. Verify `git status`, `git log`, and the active branch before every checkpoint because the native and script-recovery lanes share this worktree. `main` and remote `feat/script-recovery-marathon` were both at `8d6ce97` before this handoff. Never stage broad paths: commit only explicit files so concurrent `refs/script_recovery/new_oakvale_intro/` work and line-ending-only header changes are preserved.
- 22 old agent branches are now `archive/*` tags. Root scratch + `CON` removed. `LICENSE`, `CONTRIBUTING.md`, CI `docs-consistency` added.
- Coverage (2026-09-08 dashboard): 8,145 exact and 10,681 relocation matches among 18,870 compiled/behavior-tested candidates; 15,297 landed files are manifest-tracked, including 14,696 genuine sources. Regenerate `rebuild/COVERAGE.md` before quoting later numbers.

## Open these five first
1. `docs/ROADMAP.md` — what is done / in flight / next (the only task list)
2. `docs/ARCHITECTURE.md` — the three source layers (lift intake / parity `rebuild/src/compiled` / modern)
3. `docs/BUILDING.md` — Ghidra, VC7.1, GhidraMCP, FSE tooling commands
4. `CONTRIBUTING.md` — landing rules, header contract, purity policy
5. `rebuild/COVERAGE.md` — the numbers

## Active lanes and their resume commands
- Typed headers: `python tools/decomp_pipeline/gen_class_headers.py --all-trusted` (add `--compile-check`), then `python tools/decomp_pipeline/retype_landed.py --class CGameScriptInterface` (dry run; `--apply` only rewrites files that stay byte-exact). Log: `rebuild/backlog/retype_log.tsv`; headers: `rebuild/include/engine/` (`INDEX.tsv`, `RECONCILE.tsv`, `_quarantine/`).
- Parity crawl: batches through 377 are reviewed and ledgered in `work/crawl_batch155`; generate batch 378 next with `next_smallest.py`. Current totals are 18,870 compiled/behavior-tested, 8,145 exact, 10,681 relocation matches, and 394,217 genuine retail bytes matched. Continue evidence review; use PDB names for readability but treat retail offsets, calling conventions, behavior, and bytes as authoritative. Details: `docs/journal/2026-09/PARITY_CRAWL_2026-09-08.md`.
- Header gate: the 2026-09-08 `--all-trusted --compile-check` wrote and compiled 1,011 generated headers; 267 are layout-quarantined and `CGameDefinitionManager` remains the single additional compile quarantine. Dashboard inventory: 1,019 generated / 268 quarantined.
- De-bake: `python tools/decomp_pipeline/crawl/bake_families.py` for the live family table; `debake_family.py <template> <genuine.cpp> <prefix> --apply`.
- Script recovery: `python -m unittest discover -s tools/script_recovery -p 'test_*.py'`; `python tools/script_recovery/verify_foundation.py`; readiness needs `--vtable-slots`, `--interface-catalog`, `--fse-address-map`. Ingest Aeon's 2026-09-06 LUAGameflow batch into `work/aeon_lua_ports/`.
- ForgeFSE: canonical fork is `D:\Code\ForgeFSE-retail-shadow`, branch `feat/upstream-fse-2026-09-02` (`3f417ee`), untested in-game. `D:\Code\ForgeFSE` is stale.
- Docs hygiene: `python tools/docs_reorg.py --check-links --check-findings --check-root` and `python tools/update_readme_progress.py --check` (both run in CI).
- Build/visual QA: `rebuild/build_bootstrap.ps1 -RetailFrontendBank <frontend.big>`; recipe in `docs/pipeline/VISUAL_PARITY_STATUS.md`. Retail Fable.exe must be CLOSED (single-instance mutex fails the WinMain fixture).

## Where notes go
- This file stays one page. The long-running journal is `docs/journal/HANDOFF_ARCHIVE.md` (12k lines, append-only history).
- New session notes: `docs/journal/YYYY-MM/<TOPIC>_<date>.md` (e.g. `docs/journal/2026-09/`). Cited technical truth: `docs/journal/FINDINGS_LOG.md`.
- Solved gotchas get a one-liner in `CLAUDE.md`; roadmap status changes go in `docs/ROADMAP.md`.

## Gotchas not yet in CLAUDE.md
- A file named `CON` at the repo root hangs EVERY git command on Windows (reserved device name). Delete it with `del "\\?\D:\Documents\FableTLC\CON"` — normal `del`/Explorer cannot.
- A killed `git add` leaves a stale `.git/index.lock`; remove it before retrying (check no git process is running first).
- Sibling repos hold the moved modding docs: FableForge `D:\Code\FableForge\docs\from_fabletlc\` and ForgeFSE-retail-shadow `docs\from_fabletlc\`. `docs/modding/README.md` is the index; do not recreate those docs here.
- `native_conversion_readiness.json` silently loses all interface resolution if regenerated without `--vtable-slots` and `--interface-catalog`.
- Two ForgeFSE forks existed; only `ForgeFSE-retail-shadow` is canonical. Any binding port must land there.
