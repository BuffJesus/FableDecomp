# FableTLC / FableDecomp — AI agent guide

Reverse-engineering + behavior-first, byte-close C++ reconstruction of the **native PC** `Fable.exe`
(Fable: The Lost Chapters, Steam, VC7.1). Public repo: `BuffJesus/FableDecomp` (this tree, `main`).

**Start here, in this order:** `docs/HANDOFF.md` (current ownership + resume pointers) →
`docs/ROADMAP.md` (cross-lane priorities; current tasks stay in owner journals) → `docs/ARCHITECTURE.md` → `docs/BUILDING.md` →
`CONTRIBUTING.md`. Everything else is indexed in `docs/INDEX.md`. Solved problems go in
`docs/pipeline/GOTCHAS.md` (one line each), not here.

## The one thing to get right
**TLC is native x86-32 Windows — NOT an Xbox port.** No recompilation, no XEX, no decrypt, no packer.
Do **not** copy the Fable2RE recomp workflow (ReXGlue / codegen / guest↔host membase math). This is
Ghidra static analysis + ordinary native debugging of a clean PE32 at ImageBase `0x400000`, and
reconstruction compiled under VC7.1 and checked against retail. Passing genuine-C++ near matches are accepted under the 2026-09-26 standard; exact byte parity is tracked separately.

## Biggest assets (use them, don't re-derive)
- **PDB-derived types:** `ghidra_out/struct_layouts_egor.tsv` / `fable_types.h` (3,807 classes, real
  member names, from `debug_build/Ego_r.pdb`; FableWin.pdb + Ego_d.pdb also on disk, never committed).
  Compilable per-class headers: `rebuild/include/engine/<Class>.h` via
  `tools/decomp_pipeline/gen_class_headers.py`; retype landed code with `retype_landed.py`.
- **FSE manifest** `refs/fse_api_manifest.json` (933 reversed API functions) and FSE source at
  `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\` (hook `0xCDB355`).
- **Aeon's Lua ports** (`work/aeon_lua_ports/`, 20 packages incl. LUAGameflow) — third-party comparison material
  for retail quest scripts; resolve disagreements against native evidence; audit in `docs/scripts/AEON_LUA_PORTS.md`.
- **EgoCore** (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) — whole-format answer key
  for defs/meshes/anim/lipsync; check it BEFORE byte-RE.
- **Sibling repos:** FableForge `D:\Code\FableForge` (the level/world/content editor, formerly
  Albion Atlas; `docs/ROADMAP_1.0.md` "Resume here"), FableForge-legacy `D:\Code\FableForge-legacy`
  (the old toolchain repo, merged into FableForge piece by piece; its `docs/modding/README.md`),
  ForgeFSE-retail-shadow `D:\Code\ForgeFSE-retail-shadow` (canonical FSE fork), Fable2RE
  `D:\Documents\Fable2RE` (methodology).

## Working rules
- **Evidence, not assumption** — cross-check every claim against ≥2 sources; verify before asserting.
- **Functional first, byte-close** (user, 2026-09-26) — coverage is genuine compiler-emitted C++ that
  faithfully reproduces retail behavior, supported by candidate-bound fixtures, and is byte-close under VC7.1
  (`near_match`); exact `MATCH` is preferred, not required. A fixture pass alone does not prove all retail behavior. Stop chasing bytes once a function is
  `near_match` with a passing fixture. Still forbidden: `__asm` / `naked` / `_emit` (grade `asm_bake`:
  an oracle, never reconstruction; `verify_and_land.py` rejects it) and invented algorithms/fitted
  constants in place of the engine's own code. Details: `docs/modern/MODERNIZATION.md`.
- **Modernization fork** (x64, D3D12 + Vulkan, ffmpeg, settings/controllers) lives in a separate
  CMake tree after the fork gate; it never changes the VC7.1 parity tree. Plan:
  `docs/modern/MODERNIZATION.md`.
- **Shared headers** — model `this` with `#include "engine/<Class>.h"`, never a local
  `struct T { char pad[0x1a8]; ... }` when a header exists (the verifier warns; strict mode rejects).
- **Trust the manifest carefully** — module labels are propagated across byte-identical bodies;
  use `label_trust.py` before attributing a function to a class.
- **Reproducible DB** — names as `ghidra_out/labels_*.tsv`, applied with `LabelApply.java`.
- **Loop prevention** — never repeat a failing call >2× with the same args; after 2 failures stop and
  record the gap in `docs/journal/FINDINGS_LOG.md`.
- **Document as you go** — session notes in `docs/journal/YYYY-MM/`, status changes in
  `docs/ROADMAP.md`, resume point in `docs/HANDOFF.md` (keep it one page), gotchas in
  `docs/pipeline/GOTCHAS.md`. Never hand-edit coverage numbers; run `tools/write_decomp_dashboard.py`
  then `tools/update_readme_progress.py`.
- **Git hygiene** — `main` is what the public sees. Commit subjects `land:|docs:|tools:|fix:|hygiene:`
  with a human sentence. Never commit retail bytes, root scratch, or `work/`. A file named `CON` at the
  root hangs git on Windows.
- **Disk budget** (user, 2026-10-30: "stop filling up my hard drive so much"). D: is one 2 TB HDD shared by
  every project. `work/` grew **+279 GiB in 8 days** (Oct 22–30), almost all `modern_owned_*` bundles. The cause
  is the last loop of each bundle's `validate.py`, which `shutil.copyfile`s every hashed input into
  `validation_matrix/sources`. The input set is cumulative, so each bundle re-copies ~4 GiB of mostly unchanged files
  (two Oct 29 bundles: 4.07 of 7.92 GiB byte-identical). Rules:
  1. Never add per-run full copies of inputs. Pin inputs by the SHA-256 the validator already records (`before`).
  2. If bytes must be snapshotted, store each content once and hard-link it. In the next `prepare_matrix.py`, patch the copy loop:
     ```python
     replace(" target=out/'sources'/path.relative_to(root);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,target)",
             " target=out/'sources'/path.relative_to(root);target.parent.mkdir(parents=True,exist_ok=True);sha=before[path]\n"
             " store=root/'work/_snapshot_store'/sha[:2]/sha if sha else None\n"
             " if store and not store.exists() and digest(path)==sha:store.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,store)\n"
             " if store and store.exists():os.link(store,target)\n"
             " else:shutil.copyfile(path,target)")
     ```
     Never edit a file inside `work/_snapshot_store` or `validation_matrix*/sources`: hard-linked names share content.
  3. Before a bulk-output run, check D: free space; stop and tell the user if it is under 150 GiB.
  4. Closed bundles (untouched 72 h) may be hard-link deduped by the user's storage pass
     (`D:\Documents\Fable2RE\.analysis\runs\2026-10-29-storage-organization`, group `TlcBundleDedupe`). Content and
     hashes are unchanged; per-copy write times are kept in its manifest.

## Toolchain
Ghidra **12.1** at `D:\Subuwu\tools\ghidra-public` + GhidraMCP (port 8089). Era compiler MSVC 2003 /
VC7.1 at `D:\Tools\vc71` (QFE-4035 toolset at `D:\Tools\vc71-qfe4035` for ~11 objects). Ghidra scripts
in `tools/ghidra_scripts/`; pipeline in `tools/decomp_pipeline/`; commands in `docs/BUILDING.md`.
