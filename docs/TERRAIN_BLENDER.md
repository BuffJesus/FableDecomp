# Terrain in Blender — LEV heightmap round-trip (`io_scene_fable_level`)

*Created 2026-07-19. Task B3 (docs/CONTENT_AUTHORING_PLAN.md): "Level layout in
Blender — LEV terrain heightmap as a mesh." Non-Ghidra, self-contained. Wraps the
byte-exact `tools/lev_rw.py` (399/399 retail LEVs round-trip byte-identical).*

## What this delivers

A **new, separate** Blender add-on — `tools/blender_addon/io_scene_fable_level/`
— that imports a Fable: The Lost Chapters level's terrain as an editable Blender
heightmap **mesh**, lets you sculpt it, and exports the edited heights back to a
**byte-exact STAGED `.lev`**. It does not touch the `io_scene_fable` model-import
package.

Package split mirrors `io_scene_fable` (bpy-free core + thin bpy operator):

| File | Role |
|---|---|
| `__init__.py` | `bl_info`, register/unregister, reload support |
| `level_core.py` | **bpy-FREE** core: locates + wraps `tools/lev_rw.py`, builds a plain `Heightmap`, stages the export. Unit-testable standalone. |
| `level_io.py` | the **only** module that touches `bpy`: import/export operators, mesh builders, File>Import/Export menu + N-panel |
| `blender_manifest.toml` | Blender 4.2+ extension manifest |
| `validate_headless.py` | the headless validation gate (below) |

## Operators

### Import — "Import Fable Level (.lev)" (`import_scene.fable_level`)
- Reads a `.lev` via `lev_rw.read_file`, then builds a grid **mesh**:
  - **`(width+1) × (height+1)` vertices**, one per terrain cell, row-major
    (row `y` outer, col `x` inner) — the exact ordering `lev_rw` uses, so mesh
    vertex index `i` maps to cell `(x = i % cols, y = i // cols)`.
  - **vertex Z = the cell's world height** (`heightRaw * 2048`, matching
    FableMod/FableForge/SilverChest). X/Y are grid position × a cosmetic 1.0
    unit pitch (LEV stores no explicit cell pitch in the parsed fields; XY is
    for sculpting only and is never exported).
  - **quad faces** between adjacent cells (`(cols-1) × (rows-1)` quads).
  - a **`FableTheme` vertex-color** attribute keyed by each cell's ground-theme
    index (cosmetic, for orientation — a deterministic hashed palette).
  - **custom props** tagging the object for a safe export:
    `fable_lev_source` (abs path), `fable_lev_cols`, `fable_lev_rows`,
    `fable_lev_subver`, `fable_lev_uid`.

### Export — "Export Fable Level (.lev)" (`export_scene.fable_level`), STAGED
- Reads the edited mesh's **per-vertex Z** back into cell heights via
  `lev_rw.set_height`, then `write_file` to a **staged** path.
- **Re-reads the source `.lev`** (from `fable_lev_source`) before writing, so the
  obstacle/brush region, the entire navigation block, ground themes, and
  walkability are preserved **verbatim** — only cell heights are written through.
- **Backup**: if the output already exists, copies it to `<name>.lev.bak` first.
- **Install guard**: refuses to write to a path that looks like a game install
  (`steamapps`, `fable/data`). Write staged copies to a workspace/scratch dir.
- **Grid-lock**: asserts the mesh vertex count still equals `cols × rows`; this
  exporter maps one vertex per cell and cannot add/remove/resize.

## Headless validation (the gate) — PASS

```
blender --background --factory-startup \
  --python tools/blender_addon/io_scene_fable_level/validate_headless.py \
  -- work/level_samples/BarrowFields/BarrowFields.lev <scratch_dir>
```

Result on the real `BarrowFields.lev` (128×128 map, sub-9), exit code 0:

```
[1] imported 129x129 grid -> 16641 verts (faces 16384)
[1] max |vertZ - cellHeight| across all 16641 verts = 0
[1] PASS: heightmap verts match lev_rw cell heights
[2] control export: 0 cell(s) changed
[2] PASS: un-edited export is byte-identical to source LEV
[2] edited vertex (64,64) idx 8320: Z 28.223 -> 528.223
[2] cells whose height value changed: [(64, 64)]
[2] byte diffs edited-vs-control: [243927, 243928, 243929, 243930]
[2] PASS: edit round-trip confined to exactly one cell's height bytes
[3] saved evidence .blend
ALL CHECKS PASS
```

So: exactly `(w+1)*(h+1) = 16641` verts, every vertex Z matches `lev_rw`'s cell
height **exactly** (float32, max delta 0), an un-edited export is **byte-identical**
to the source, and a one-vertex edit changes **exactly** that one cell — confined
to its 4 height bytes (offsets 243927–243930) and nothing else. Evidence `.blend`
saved alongside the two staged `.lev`s.

## Honest gaps / out of scope

- **Navmesh is NOT regenerated on height edits.** The nav / region graph /
  obstacle-brush data are preserved opaque (inherited from `lev_rw`, see
  docs/LEV_WRITER.md). The engine's runtime navigation still reflects the
  **original** terrain; large gameplay-correct edits need the nav rebuild
  (`CMap::SaveToFile` / `CEditWorldMap`), a follow-on Ghidra job. The importer
  tags the object and the N-panel with this warning.
- **No grid resizing.** Changing terrain dimensions would move `navOffset`/
  `obsOffset` and require rewriting the opaque spans. The exporter hard-fails if
  the vertex count changes.
- **TNG "things" placement is DEFERRED.** B3 lists placing TNG things as
  Empties as an optional extra; it was deferred to keep terrain the priority and
  because no ready bpy-free Python TNG reader ships in `tools/` (TNG parsing
  lives in FableForge's C++ `forge::tng`). Adding it later is a wrapping job over
  a `forge tng` dump or a small Python TNG reader — it does not affect the
  terrain round-trip. Terrain is the priority and is complete.
- **XY scale is cosmetic** — vertices are placed on a unit grid so the heightfield
  is evenly spaced for sculpting; only Z (world height) round-trips. Object
  transforms are baked into Z on export (toggleable).

## Files

- `tools/blender_addon/io_scene_fable_level/` — the add-on (new, separate package).
- `tools/blender_addon/io_scene_fable_level/validate_headless.py` — the gate.
- Wraps `tools/lev_rw.py` (docs/LEV_WRITER.md) — the byte-exact reader/writer.
- Sample: `work/level_samples/BarrowFields/BarrowFields.lev`.
