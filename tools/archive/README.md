# tools/archive

One-off scripts that are not part of the decomp pipeline and are kept only for provenance.
Nothing under `tools/`, `rebuild/`, or CI imports them (checked 2026-09-07). Prefer the
FableForge toolchain (`D:\Code\FableForge`) for anything these once did.

- `fqt/` — FQT (C# modding tool, the donor project) export/catalog helpers. FQT was superseded by
  FableForge on 2026-07-18; its docs moved to `D:\Code\FableForge\docs\from_fabletlc\`.
- `fable2_scene_import/` — the 2026-08 experiment importing the Fable 2 Bowerstone Market scene
  (architecture textures, static meshes, TNG placement, "opaque base stage") into TLC. Record:
  `FABLE2_BWSMARKET_SCENE_IMPORT_PROGRESS.md` in the FableForge docs; verification outputs
  lived under `work/`.
