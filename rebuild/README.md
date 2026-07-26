# FableTLC source reconstruction

This is the build-oriented counterpart to the Ghidra database. It is generated from authoritative
exports and contains only source that has crossed an explicit grade.

## Regenerate

```powershell
python tools/bootstrap_rebuild_tree.py
python tools/gate_re_agent_candidates.py
python tools/audit_candidate_signatures.py
python tools/build_reconstruction_backlog.py
python tools/build_promotion_queue.py
python tools/write_decomp_dashboard.py
powershell -ExecutionPolicy Bypass -File rebuild/build_tier0.ps1
powershell -ExecutionPolicy Bypass -File rebuild/build_candidates.ps1
powershell -ExecutionPolicy Bypass -File tools/ExportCandidateOracles.ps1
python tools/compare_candidate_objects.py
powershell -ExecutionPolicy Bypass -File tools/ProbeVc71Sdk.ps1
```

`FableTLC Rebuild Refresh` is a hidden 15-minute Scheduled Task installed by
`tools/InstallRebuildRefreshTask.ps1`. It fingerprints candidate/manifests inputs, defers while the
auto-RE or naming/Ghidra pipeline owns the workspace, and performs no work when inputs are unchanged.

The bootstrap writes `manifest/functions.tsv`, `manifest/modules.tsv`, and `manifest/status.json`;
materializes the three proven Tier-0 modules under `src/tier0`; and copies their retail byte oracles.
`COVERAGE.md` is the honest project dashboard.

Generated auto-RE code remains a candidate until it compiles. The manifest records candidate paths
and checker verdicts without silently promoting them to reconstructed source.

`compile-gate/README.md` records exact immutable snapshots, strict 32-bit C++20 syntax results,
missing engine declarations, and VC7.1 language blockers. `backlog/README.md` converts those results
plus the function manifest into the next module/function work queue.
`backlog/PROMOTION_QUEUE.md` ranks uncompiled auto-RE candidates into compile-now, VC7.1-port,
declaration-fix, dependency-stub, and manual-lift lanes. Ranking is advisory; compile, focused
behavior, and retail comparison remain mandatory promotion gates.
The retail oracle export reads exact function bytes from the authoritative Ghidra `Fable.exe` image;
`retail-parity.tsv` records raw `.text` equality and the first differing byte.
Functions containing calls/globals also receive `RELOCATION_MATCH` when every non-relocation byte and
instruction offset matches retail; raw COFF relocation payloads are intentionally not called literal matches.
`ghidra_out/naming_stragglers/QUALITY_TRIAGE.md` separately records navigation-grade reconstruction
family labels and original-symbol evidence, so coverage does not depend on pretending the former are
recovered Lionhead names.

The validated engine environment combines VC7.1 with Windows SDK `10.0.22621.0` x86 headers and
import libraries. Dot-source `rebuild/vc71_sdk_env.ps1` before compiling a Win32/Direct3D module.
The probe compiles, links, and runs a `windows.h` + `d3d9.h` executable successfully; modern-header
unknown-pragmas are warnings, not ABI errors.

## Grades

- `candidate`: generated source exists.
- `checker-pass`: structural auto-RE review passed.
- `compiled`: emits a VC7.1 object, but has not yet crossed a behavioral oracle.
- `functional`: compiles under VC7.1 and passes behavioral/parity review.
- `matching`: compiles under VC7.1 to byte-identical retail `.text`.

The ultimate target is a functional rebuilt executable. Matching grade is pursued where practical.

`RUNNABLE.md` tracks the executable-integration milestone separately from
per-function coverage. `build_bootstrap.ps1` builds and runs the Stage-0
byte-identical-function proof plus the Stage-1 Win32 GUI startup shell. The
recovered WinMain body is a 141-byte relocation-normalized retail match and
reaches an instrumented GFMain boundary. This is boot-chain progress, not yet
a running game.
