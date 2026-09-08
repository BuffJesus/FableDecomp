# Resume here (one page) — updated 2026-09-07

## State
- Branch: `feat/script-recovery-marathon` (head `2e038aa`, clean tree, `main` == this head); `main` was fast-forwarded (519 commits) and pushed to `BuffJesus/FableDecomp` on 2026-09-07 — fast-forward it again after each checkpoint. Note: `git add` on this tree takes 1–6 minutes and HANGS if another process is writing under `rebuild/` (or a reserved-name file like `CON` exists); never run it concurrently with the header generator or retype.
- 22 old agent branches are now `archive/*` tags. Root scratch + `CON` removed. `LICENSE`, `CONTRIBUTING.md`, CI `docs-consistency` added.
- Coverage (2026-08-28 dashboard): 8,082 byte-identical (16.3%), 18,579 functional-or-matching (37.5%) of 49,568; catalog is now 18,631 landed. Regenerate `rebuild/COVERAGE.md` before quoting numbers.

## Open these five first
1. `docs/ROADMAP.md` — what is done / in flight / next (the only task list)
2. `docs/ARCHITECTURE.md` — the three source layers (lift intake / parity `rebuild/src/compiled` / modern)
3. `docs/BUILDING.md` — Ghidra, VC7.1, GhidraMCP, FSE tooling commands
4. `CONTRIBUTING.md` — landing rules, header contract, purity policy
5. `rebuild/COVERAGE.md` — the numbers

## Active lanes and their resume commands
- Typed headers: `python tools/decomp_pipeline/gen_class_headers.py --all-trusted` (add `--compile-check`), then `python tools/decomp_pipeline/retype_landed.py --class CGameScriptInterface` (dry run; `--apply` only rewrites files that stay byte-exact). Log: `rebuild/backlog/retype_log.tsv`; headers: `rebuild/include/engine/` (`INDEX.tsv`, `RECONCILE.tsv`, `_quarantine/`).
- Parity crawl: batches through 173 are reviewed in `work/crawl_batch155`; forty-three readable functions landed on 2026-09-08. Continue evidence review; do not accept propagated labels where the body contradicts the PDB layout or prototype. Details: `docs/journal/2026-09/PARITY_CRAWL_2026-09-08.md`.
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
