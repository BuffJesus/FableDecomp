# FableTLC / FableDecomp — AI agent guide

Reverse-engineering + byte-exact reconstruction of the **native PC** `Fable.exe`
(Fable: The Lost Chapters, Steam, VC7.1). Public repo: `BuffJesus/FableDecomp` (this tree, `main`).

**Start here, in this order:** `docs/HANDOFF.md` (one page: state + resume commands) →
`docs/ROADMAP.md` (the only task list) → `docs/ARCHITECTURE.md` → `docs/BUILDING.md` →
`CONTRIBUTING.md`. Everything else is indexed in `docs/INDEX.md`. Solved problems go in
`docs/pipeline/GOTCHAS.md` (one line each), not here.

## The one thing to get right
**TLC is native x86-32 Windows — NOT an Xbox port.** No recompilation, no XEX, no decrypt, no packer.
Do **not** copy the Fable2RE recomp workflow (ReXGlue / codegen / guest↔host membase math). This is
Ghidra static analysis + ordinary native debugging of a clean PE32 at ImageBase `0x400000`, and
reconstruction that must compile under VC7.1 to the retail bytes.

## Biggest assets (use them, don't re-derive)
- **PDB-derived types:** `ghidra_out/struct_layouts_egor.tsv` / `fable_types.h` (3,807 classes, real
  member names, from `debug_build/Ego_r.pdb`; FableWin.pdb + Ego_d.pdb also on disk, never committed).
  Compilable per-class headers: `rebuild/include/engine/<Class>.h` via
  `tools/decomp_pipeline/gen_class_headers.py`; retype landed code with `retype_landed.py`.
- **FSE manifest** `refs/fse_api_manifest.json` (933 reversed API functions) and FSE source at
  `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\` (hook `0xCDB355`).
- **Aeon's Lua ports** (`work/aeon_lua_ports/`, 20 packages incl. LUAGameflow) — ground-truth oracle
  for retail quest scripts; audit in `docs/scripts/AEON_LUA_PORTS.md`.
- **EgoCore** (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) — whole-format answer key
  for defs/meshes/anim/lipsync; check it BEFORE byte-RE.
- **Sibling repos:** FableForge `D:\Code\FableForge` (modding toolchain, the build target),
  ForgeFSE-retail-shadow `D:\Code\ForgeFSE-retail-shadow` (canonical FSE fork), Fable2RE
  `D:\Documents\Fable2RE` (methodology). Modding docs live there (`docs/modding/README.md`).

## Working rules
- **Evidence, not assumption** — cross-check every claim against ≥2 sources; verify before asserting.
- **Byte purity** — coverage is compiler-emitted C++ that byte-matches retail. `__asm` / `naked` /
  `_emit` is grade `asm_bake`: an oracle, never reconstruction. `verify_and_land.py` rejects it.
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

## Toolchain
Ghidra **12.1** at `D:\Subuwu\tools\ghidra-public` + GhidraMCP (port 8089). Era compiler MSVC 2003 /
VC7.1 at `D:\Tools\vc71` (QFE-4035 toolset at `D:\Tools\vc71-qfe4035` for ~11 objects). Ghidra scripts
in `tools/ghidra_scripts/`; pipeline in `tools/decomp_pipeline/`; commands in `docs/BUILDING.md`.
