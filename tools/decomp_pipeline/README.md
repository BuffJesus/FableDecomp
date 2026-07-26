# decomp_pipeline — byte-match promotion pipeline

Reusable scripts that drive the reconstruction loop: extract retail ground truth → author
candidate C++ (via ultracode workflows) → verify byte-match + behaviour → land into the build
catalog. Pairs with `tools/permuter/` (the register-allocation / flag-sweep cracker).

## The cycle

1. **Select** untried candidates from `rebuild/manifest/functions.tsv` with
   `next_batch.py <name> <count> [max_len]`. It writes a paired oracle/target manifest under
   `rebuild/oracles/pending/`. Capstone performs in-process function splitting when installed;
   `objdump` remains the portable fallback.
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

The zero-agent fast path is `auto_author_tiny.py <oracle.tsv> <output.json>`. It recognizes only
instruction streams whose source semantics are determined by the bytes (empty functions, constant
returns, fastcall self returns, simple field loads/stores, and masks). Its output still goes through
`verify_and_land.py`; a bad inference can cost one compile but cannot land.

`tools/run_local_parity_queue.ps1` connects select → deterministic author → verify/land →
canonical report refresh as a bounded, Ghidra-free background lane. It is intentionally separate
from Wave 3 structural reconstruction so the strict verified counter can advance while Ghidra is
busy. Install or update its hourly scheduled task with:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/InstallLocalParityTask.ps1
```

## VC7.1 gotchas (baked into the tooling)
- `__thiscall` on a free function is illegal (C4234) — model member accessors as `__fastcall(objptr)`
  (byte-identical for a this-only accessor).
- No C++11 (`static_assert`, `nullptr`, `auto`, `enum class`).
- A chunk of retail TUs were **size-optimized** — `#pragma optimize("s",on)` flips many DIFFERs to
  MATCH; the lander sweeps this automatically.
- Parity masks relocations (call rel32 + abs-addr operands) — declare engine callees/globals `extern`.

## Current resume point (2026-07-25)

- Batch 18 and batch 19 landed 97 authoritative manifest-backed deterministic authors through exact
  byte and behavior verification.
- The integrity follow-up removed 146 byte-matching post-`ret` tails that lacked a manifest function
  start. `next_batch.py` no longer emits those speculative rows, and `verify_and_land.py`
  independently rejects any future outside-manifest candidate.
- Older staged batches remain excluded by `next_batch.py`, so the scheduled lane advances rather
  than repeatedly selecting their unresolved rows.
- `crack_residue.py`, `verify_residue.py`, and `tools/permuter/` remain targeted tools for promising
  same-length `DIFFER` rows. They are not sprayed across every residue because prior audits proved
  several compiler register-allocation differences irreducible from VC7.1 source spelling.
