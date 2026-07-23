# decomp_pipeline — byte-match promotion pipeline

Reusable scripts that drive the reconstruction loop: extract retail ground truth → author
candidate C++ (via ultracode workflows) → verify byte-match + behaviour → land into the build
catalog. Pairs with `tools/permuter/` (the register-allocation / flag-sweep cracker).

## The cycle

1. **Select** untried candidates from `rebuild/manifest/functions.tsv` (accessors/setters/predicates
   with a complete prototype, primitive/pointer return, not already landed) → `targets.json`.
2. **Extract oracle** — coordinate with the auto-RE loop (drop `lift/state/re-agent-wave3-queue.stop`,
   run `ExportFunctionOracle.java` headless for the addresses, remove the marker). Ground-truth bytes.
   `pe_oracle.py` is a Ghidra-free fallback but only ~76% length-accurate — prefer the Ghidra export.
3. **Bundle** — `gen_bundles.py <oracle.tsv> <targets.json> <out_dir> <manifest.json> [max_len]`
   writes per-function disasm bundles + a manifest for workflow args.
4. **Author** — launch an ultracode `Workflow` fanning out one agent per candidate (prompt: match the
   disasm instruction-for-instruction under VC7.1 constraints; return `source_cpp` + `test_cpp` +
   `pass_pattern`). See prior runs' scripts under the session `workflows/scripts/`.
5. **Verify + land** — `verify_and_land.py <workflow_output.json> <oracle.tsv> [--land]`. Applies VC7.1
   fixups (`__thiscall`→`__fastcall`, strip `static_assert`), gates on exact/relocation byte-match
   **and** behaviour test, integrates the permuter pragma sweep (`optimize("s")` etc.) on DIFFERs,
   skips already-landed addresses, and (with `--land`) writes `src/compiled` + `tests` + catalog +
   oracle. Then `git` add/commit/push.

## VC7.1 gotchas (baked into the tooling)
- `__thiscall` on a free function is illegal (C4234) — model member accessors as `__fastcall(objptr)`
  (byte-identical for a this-only accessor).
- No C++11 (`static_assert`, `nullptr`, `auto`, `enum class`).
- A chunk of retail TUs were **size-optimized** — `#pragma optimize("s",on)` flips many DIFFERs to
  MATCH; the lander sweeps this automatically.
- Parity masks relocations (call rel32 + abs-addr operands) — declare engine callees/globals `extern`.

## Resume point (paused 2026-07-23 for shutdown)
- **200 fresh oracle rows already extracted** — `rebuild/oracles/pending/batch3_oracle.tsv` (+
  `batch3_targets.json`). No Ghidra run needed to resume: `gen_bundles.py` them, author in batches of
  ~50, verify_and_land.
- A **re-author fleet** for 27 semantic-miss accessors was mid-run at shutdown (lost; re-runnable —
  targets were the length-mismatched DIFFERs from the 55-accessor batch).
- Session landed **30 byte-parity promotions**; dashboard byte-identical 51→55 and climbing as the
  Rebuild Refresh ingests the ~24 accessor lands still pending a rebuild.
