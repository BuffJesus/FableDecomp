# Blender import addon — `io_scene_fable`

*Authored 2026-07-19 (Blender-addon build agent). A Blender add-on that imports
everything the FableTLC RE project can decode from a `graphics.big` mesh bank
**today**: textured materials, skeletons with REAL bind poses (2026-07-20), the
LOD/submesh tree, and real mesh geometry (2026-07-19).*

Deliverable: `tools/blender_addon/io_scene_fable/` (installable add-on package).
It **consumes the project readers READ-ONLY** and does not modify them:
`tools/parse_bigb.py`, `tools/parse_mesh.py`, `tools/parse_texture.py`,
`tools/lionhead_lz.py`.

---

## 0. TL;DR

| Feature | Status | Evidence |
|---------|--------|----------|
| Materials with **real decoded textures** (headline) | **WORKING** | seagull body (DXT1 256²) + wing (DXT3 256²) and bear body (`BROWNBEAR_BODY_32.TGA`, DXT1 512²) decode to coherent images and land in Principled BSDF base color |
| **Armature** with REAL bind poses | **WORKING (2026-07-20)** | bone names + parent hierarchy + inverse-bind matrices decoded from the compiled bone blocks (`BIG_MESH_GEOMETRY.md` §7, ported from EgoCore). Seagull → 27 bones, goblin → 67, single tree rooted at `Scene Root`, all heads inside the mesh bbox (`tests/blender_bones_test.py`) |
| **LOD / submesh / bounds** placeholder tree | **WORKING** | seagull → 3 LOD empties + 2 submesh empties (verts/faces/sVert in the names) |
| **Geometry** (verts/faces/UVs) | **WORKING** (2026-07-19) | per-primitive parse ported from EgoCore (`_parse_mesh_primitives`); on-disk buffers are Fable chunked-LZO. Seagull → 2 mesh objects (254v+165v), bear → 2983v/6596f, imported headless in Blender 5.0. Supersedes `find_subm_headers` for LOD0 (fixes the bear-yields-0-submeshes gap in §4.3) |
| **Skin weights** → vertex groups + Armature modifier | **WORKING (2026-07-20)** | per-vertex joint/weight bytes decoded via the animated-block `Groups[]` palettes (`BIG_MESH_GEOMETRY.md` §7.1, ported from EgoCore `GltfExporter.h`). Goblin: 4 skinned prims / 54 groups on the body; seagull: 2 prims. Blender weights byte-match the decode; every skinned vertex has ≥1 group summing to 1.0 (`tests/blender_skin_test.py`). Export-side inverse `mesh_rw.encode_skin` round-trips exactly |
| **Animations** ANIM (3DAF) ⇄ Blender actions | **WORKING (2026-07-20)** | import a retail `ANIM_*` entry as an action on the imported rig; export an action back via the `anim_build` donor-clone recipe. Numeric round-trip on retail clips: max quat err `1-|dot|` 1.85e-07, max pos err 2.04e-04 units (`tests/blender_anim_test.py`) |
| Blender run (headless) | **PASSED** on Blender 5.0 | `.blend` + a saved bpy-image PNG written to scratchpad |

---

## 1. What it imports today

On importing a `graphics.big` (File > Import > *Fable TLC Model (.big)*), for the
chosen compiled mesh entry the addon creates:

1. **Materials with real textures** *(the headline working feature).*
   Each mesh material's `matTexId` is a `textures.big` entry ID. The addon
   locates that entry, **LZO1X-decompresses** its base mip (`lionhead_lz.py`),
   **DXT1/DXT3/A8R8G8B8-decodes** it to RGBA (`parse_texture.py`), packs it as a
   `bpy.data.images` image, and wires it into a **Principled BSDF** (base color;
   alpha too for DXT3/ARGB). Proven on real bytes — see §4.

   *Texture-ID collision fix (important):* `textures.big` has two sub-banks
   (`GBANK_GUI_PC`, `GBANK_MAIN_PC`) whose IDs BOTH start at 1 and collide. A
   mesh's `matTexId` resolves to **GBANK_MAIN_PC** (the world/character bank).
   `fable_core.TextureBank` indexes MAIN with priority over GUI for colliding
   IDs (validated: seagull ids 5,6 → `SEAGULL_BODY`/`SEAGULL_WING`, not the
   `EDITORGUI_*` icons that share those IDs in the GUI bank).

2. **Armature with REAL bind poses (2026-07-20).** For skinned meshes the
   compiled payload's bone blocks are now DECODED (not skipped) by
   `fable_core._parse_mesh_primitives` / `_decode_bones` — layouts in
   `docs/BIG_MESH_GEOMETRY.md` §7, ported from EgoCore `MeshParser.h` +
   `GltfExporter.h`:
   - names: `BoneCount` NUL-terminated strings (chunked-LZO block),
   - parent hierarchy: `C3DBone.ParentIndex` from the 60 B/bone block
     (−1 = root; the raw `u16` table before it is a GLOBAL-bone-ID remap,
     NOT parents),
   - bind pose: the 64 B/bone **inverse bind matrix** block; world bind
     `W = inverse(IBM)`, bone head = `W`'s translation.
   Each edit-bone gets `eb.matrix = W` (orientation + position), length reaches
   to the nearest child head (leaves get 5 % of the skeleton span), and is
   parented per `ParentIndex`. The old Biped-token name-scan
   (`recover_bone_names`) remains as a FALLBACK if the block decode fails; the
   armature's `fable_note` custom prop says which path built it.

   **Skin binding (2026-07-20).** Skinned primitives (`AnimatedBlockCount > 0`)
   additionally get per-vertex bone weights: `fable_core._prim_skin` decodes the
   4 joint + 4 weight bytes per vertex (joint byte = `paletteIndex*3` into the
   animated block's `Groups[]` local-bone palette — `BIG_MESH_GEOMETRY.md`
   §7.1), renormalized to sum 1.0. `import_op._apply_skin` creates one **vertex
   group per used bone** (named by bone name), assigns the weights, adds an
   **Armature modifier** bound to the imported armature and parents the mesh to
   it — imported creatures deform correctly when the armature is posed. The
   export-side inverse (`mesh_rw.encode_skin`) rewrites joint/weight bytes and
   block palettes; decode→encode→decode is an exact fixpoint on retail data.

3. **Submesh / LOD / bounds tree.** An Empty hierarchy: a root empty per model,
   a `PLAIN_AXES` empty per LOD (with `switch_distance` + payload offset custom
   props), and a `CUBE` empty per submesh (with `n_verts`, `n_faces`, `sVert`,
   `material_slot` custom props, encoded in the name too). This makes the model's
   organization visible before geometry exists.

4. **Geometry — STUB.** `fable_core.decode_geometry(payload, submesh, lod_off)`
   currently returns `None` (with a TODO citing `docs/BIG_MESH_GEOMETRY.md`). See
   §3 for exactly how it slots in.

5. **Animations (2026-07-20)** — `anim_ops.py`, the visual authoring bridge for
   `docs/ANIM_WRITER.md` (write path) / `BIG_ANIM_FORMAT.md` §9 (read path).

   **Import** (*File > Import > Fable TLC Animation (.big)* with an imported
   Fable armature active, or headless
   `anim_ops.build_action_from_anim(arm_obj, anim, name)`): decodes an `ANIM_*`
   entry with `tools/parse_anim_xseq.py` and keys one Blender action —
   `rotation_quaternion` + `location` fcurves per bone track (LINEAR, anim
   frame *i* at Blender frame *i*+1, quat keys sign-aligned along time). The
   action carries `fable_fps` / `fable_frames` / `fable_cyclic` /
   `fable_src_big` / `fable_entry` custom props. Bones in the anim but not in
   the armature are skipped and reported. Works with both the legacy
   `Action.fcurves` API and slotted actions (4.4+/5.x channelbags).

   **Export** (*File > Export > Fable TLC Animation (staged .big)*, or headless
   `anim_ops.export_action_to_image(arm_obj, action, donor_img)`): the
   ANIM_WRITER §4 **donor-clone recipe**, automated. The caller names a donor
   clip whose rig matches the armature; its decompressed 3DAF image is parsed
   losslessly (`anim_build.parse_image`) and every track whose bone exists in
   the armature gets its non-empty quat/pos pools + palettes REPLACED by data
   sampled from the action's fcurves on the donor's fps/frame grid (keyframe
   reduction: dedup at f32 storage precision → pool + palette, dense when no
   win; positions re-quantized to i16 with a fresh auto `positionFactor`).
   Everything else (header bytes, MVEC/TMEV/AMSK, empty-pool stub tracks like
   `Scene Root`/`movement_dummy`, tracks for bones missing from the armature,
   helper tracks) is kept byte-verbatim. Same donor name = in-place edit;
   a new name = added TOC entry (fresh 24-B Info blob when retimed). Output is
   a STAGED .big via `big_write.rebuild` — the install is never touched.

   **Coordinate convention** (probed; see ANIM_WRITER.md §7): the importer maps
   Fable model space to Blender verbatim, and an anim track stores the bone's
   full parent-relative local transform: `p` = local translation, `q` =
   **conjugate** of the mathutils local rotation. Pose-bone math both ways:
   `rest_local = parent.matrix_local⁻¹ @ matrix_local`,
   `basis_q = conj(rest_q) @ conj(q_anim)`, `basis_p = conj(rest_q) @ (p − rest_t)`
   — export applies the exact inverse, so a stationary bone exports the retail
   constant pools back.

---

## 2. Architecture (bpy-free core + thin bpy layer)

```
tools/blender_addon/io_scene_fable/
  __init__.py              bl_info + register/unregister (thin)
  blender_manifest.toml    Blender 4.2+/5.0 extension manifest
  fable_core.py    <-- ALL parse/convert logic, ZERO bpy. Unit-testable.
  import_op.py     <-- bpy layer: mesh import/export operators, menu, panel,
                       and the plain-data -> Blender-datablock builders.
  anim_ops.py      <-- bpy layer: ANIM (3DAF) <-> action operators + the
                       headless-callable build_action_from_anim /
                       export_action_to_image (§1.5); anim parse/encode itself
                       lives in tools/parse_anim_xseq.py + tools/anim_build.py.
tools/blender_addon/tests/
  test_core.py             standalone (bpy-free) test on REAL retail data
  blender_headless_test.py headless Blender end-to-end test
  blender_bones_test.py    headless bind-pose/skeleton validation (+ proof/)
  blender_skin_test.py     headless skin-weight validation (vertex groups vs decode)
  blender_anim_test.py     headless ANIM action round-trip (import->export vs retail)
```

`fable_core` produces plain dataclasses that are the contract with the bpy layer:

- `ImageData` — decoded RGBA (Blender-order flat float pixels) + dims + format.
- `MaterialData` — `slot`, `base_tex_id`, resolved `base_tex_name`, `image`.
- `BoneData` — `name`, `index`, `parent` (real, from `C3DBone.ParentIndex`),
  `head` (bind position), `matrix` (16-float world bind matrix; empty tuple on
  the name-scan fallback path), `global_id` (engine-wide Fable bone ID).
- `SubmeshData` / `LodData` / `MeshModel` — the LOD+submesh+bounds tree, plus a
  `GeometryData` slot filled by `decode_geometry` when it exists.

`import_op` maps each of those onto Blender: `ImageData → bpy.data.images`,
`MaterialData → ShaderNodeBsdfPrincipled`, `BoneData → armature edit_bones`,
`MeshModel → the empty hierarchy` (and `GeometryData → a real mesh` once
available).

This split is why the data path is provable **without** Blender (see §4) and the
bpy layer stays thin.

---

## 3. The geometry stub — how it slots in later

`decode_geometry()` in `fable_core.py` is the single hook. Today:

```python
def decode_geometry(payload, submesh, lod_off):
    # TODO(geometry): implement once docs/BIG_MESH_GEOMETRY.md exists.
    return None
```

The call site is already wired in two places, so **only this body** needs
filling when the mesh-geometry RE lands:

- `fable_core.build_model()` calls `decode_geometry(...)` per submesh and stores
  the result on `SubmeshData.geometry`.
- `import_op._build_placeholders()` calls `_build_geometry_mesh()`, which builds
  a **real** `bpy` mesh (verts + faces + UVs + the correct material slot) when
  `submesh.geometry` is non-`None`, and otherwise builds the placeholder empty.

When implemented, `decode_geometry` must return a `GeometryData` with:
`vertices: list[(x,y,z)]` (dequantized from the packed coords per the `sVert`
class), `faces: list[(i,j,k)]` (from the decompressed index stream), and
`uvs: per-loop [(u,v)]` (from the s11e4 tu/tv fields). Everything downstream —
material slots, LOD parenting, the operator — already handles the real-mesh case.

Per `docs/BIG_MESH_FORMAT.md` §6, the packed vertex buffer and the compressed
index stream use Big Blue Box's proprietary control-byte compression; decoding
them needs the engine's mesh decompressor (a Ghidra target). The `SUBM` counters
(`n_verts`, `n_faces`, `sVert`) are already recovered and carried through, so the
future decoder is well-scoped.

---

## 4. Test status (proven on REAL data)

### 4.1 Core test — bpy-free, on real retail bytes — **PASSED**
```
python tools/blender_addon/tests/test_core.py \
    "<install>/data/graphics/graphics.big"
```
Result (retail `graphics.big` + `textures.big`): indexes 6,290 texture entries,
finds 3,259 compiled meshes, and proves a real texture into a material:
`MESH_OBJECT_STATUE_BEAR` material 0 → tex id 237 (`BROWNBEAR_BODY_32.TGA`) →
LZO1X-decompress + DXT1-decode → **512×512** RGBA, 126 distinct tones, full
[0,1] range (real content, not noise). Writes a PNG of the decoded texture as
visual evidence. The bear-body atlas (fur, paw pads, eye, mouth) and the seagull
body/wing render as coherent images.

### 4.2 Blender headless test — Blender 5.0 — **PASSED**
```
blender --background --factory-startup --python \
    tools/blender_addon/tests/blender_headless_test.py -- \
    "<install>/data/graphics/graphics.big" MESH_SEAGULL_01 <out_dir>
```
(`--factory-startup` avoids unrelated pre-installed user add-ons.) It registers
the addon from source, runs the import operator, and asserts: 2 packed non-empty
bpy images, 2 Principled-BSDF materials wired to image textures, an armature with
bones, 3 LOD + 2 submesh empties, and real mesh objects (2 for the seagull).
It saves `fable_import_MESH_SEAGULL_01.blend`
and re-saves the packed bpy image to `fable_bpy_image_MESH_SEAGULL_01.png` (a
coherent seagull-body texture) as proof the texture round-tripped **into**
Blender's image datablock.

**Bind-pose test** (`tests/blender_bones_test.py`, 2026-07-20): imports
`MESH_SEAGULL_01` (27 bones) and `MESH_CREATURE_GOBLIN` (67 bones) and asserts:
armature bone count == compiled `BoneCount`; single-root tree at `Scene Root`
with a `Bip01` child chain; bone heads NOT all at origin and ALL inside the
padded mesh bbox; skeleton centroid inside the mesh volume (seagull dist 6.79
vs half-extent 102.65; goblin 14.63 vs 99.36); and the armature was built by
the REAL-bind-pose path. Proof `.blend` + renders in `tests/proof/`.

**Skin-weight test** (`tests/blender_skin_test.py`, 2026-07-20): imports
`MESH_CREATURE_GOBLIN` (4 skinned prims, 67 bones) and `MESH_SEAGULL_01`
(2 prims, 27 bones) and asserts, per mesh object: vertex groups exist and every
group name is a real armature bone; every vertex has ≥1 group; per-vertex
weights sum ≈ 1.0; an `ARMATURE` modifier is bound to the imported armature and
the mesh is parented to it; and the Blender weights EQUAL the ground-truth
`fable_core._prim_skin` decode (same bone set, |Δw| < 1e-4). Proof `.blend`s in
`tests/proof/` (`fable_skin_*.blend`).

**Anim round-trip test** (`tests/blender_anim_test.py`, 2026-07-20): for
(`MESH_CREATURE_GOBLIN`, `ANIM_CREATURE_GOBLIN_ATTACK_01`) and
(`MESH_CREATURE_SPARROW`, `ANIM_CREATURE_SPARROW_FLY`) it imports the creature,
builds an action FROM the decoded retail anim, exports the action back over the
same entry as donor, and compares per-frame quat/pos sequences numerically
against retail (sign-invariant quat metric `1-|dot|`; euclidean pos distance):

```
GOBLIN_ATTACK_01: 61/63 tracks replaced (2 empty stubs kept), 1891 frames:
                  max quat 1-|dot| = 1.85e-07, max |dp| = 2.04e-04 units
SPARROW_FLY:      27 rot / 29 pos tracks replaced (4 donor bones not in the
                  mesh rig kept verbatim), max 1.59e-07 / 9.66e-05
```

Both recomposed payloads re-parse clean with the independent decoder
(0 pal_oob / tail bytes / bad quats), and survive `big_write.rebuild`
injection + reload from the staged .big. Tolerances asserted: quat < 5e-6,
pos < 2e-3 units.

Both DXT1 (seagull body / bear body) and DXT3 (seagull wing) decode paths are
proven through real materials. The A8R8G8B8 path uses the same
`parse_texture.decode_mip` and is covered by that reader's own unit tests
(ARGB textures are rare among mesh-referenced textures).

### 4.3 Honest limitations
- **Submesh detection** relies on the read-only `parse_mesh.find_subm_headers`
  heuristic. It reliably finds submeshes for skinned meshes (seagull → 2), but
  yields **0** for some static meshes (e.g. `MESH_OBJECT_STATUE_BEAR` — the
  reader itself reports 0 SUBM headers there). The addon faithfully reflects the
  reader; deeper submesh recovery will firm up with the geometry RE.
- **Bones exist only on skinned meshes** (`BoneCount > 0` in the compiled
  payload; static meshes import with no armature). On the decode path names,
  hierarchy and bind matrices are REAL (see §1.2); the partial name-scan is
  only the fallback. **Skin weights ARE wired up (2026-07-20)** — vertex
  groups + Armature modifier per skinned primitive (§1.2, §7.1 decode); note
  the joint bytes resolve through the animated-block `Groups[]` LOCAL palettes,
  so no `global_id` remap is needed on the weight path (that remap is for
  helper/dummy/generator `BoneIndex` fields and anim tracks).
- **Deeper LODs (LOD1+)**: only LOD0's block offset is precisely known from the
  descriptor; deeper LODs are shown as structural placeholders.
- **Geometry** is not imported at all yet (stub) — this is by design.
- **Anim export is donor-clone only**: it cannot invent tracks (bones the donor
  clip doesn't animate stay static; donor tracks whose bones are missing from
  the armature, empty stub tracks, helper tracks and the MVEC movement vector
  are copied verbatim). Retiming (`frame_count=` override) recomputes ANRT/Info
  durations, but an in-place TOC Info edit is still impossible for same-name
  edits with changed duration (`big_write` gap — export under a NEW name
  instead). Actions with non-integer-frame keys are sampled through Blender's
  fcurve interpolation. In-game visual soak untested (same residual as
  ANIM_WRITER §5).

---

## 5. Install & usage

### Install (Blender 4.2 / 5.0, extension system)
1. Zip the package folder so the zip contains `io_scene_fable/` at its root
   (with `__init__.py`, `blender_manifest.toml`, `fable_core.py`, `import_op.py`).
2. Blender → Edit > Preferences > Add-ons (or Get Extensions) → *Install from
   Disk…* → pick the zip → enable **Fable TLC model importer**.

### Install (legacy `bl_info`, Blender 3.x)
Same zip; *Install…* in the Add-ons tab. `bl_info` is present for 3.0+.

### Where it finds the readers
`fable_core` imports the four readers from the repo's `tools/` dir. If you
install the addon **inside** the repo layout (`tools/blender_addon/io_scene_fable/`),
auto-detection (two dirs up) just works. If you install a copied zip **outside**
the repo, set **Tools dir** in the import options to the repo's `tools/` folder
(the one containing `parse_bigb.py`).

### Usage
- File > Import > **Fable TLC Model (.big)**, or the **Fable TLC** N-panel in the
  3D viewport → *Import Model (.big)*.
- Pick `graphics.big`. Options:
  - **Entry (name or id)** — e.g. `MESH_SEAGULL_01`, or an entry id.
  - **Import all meshes** (+ **Max**) — bulk import with a safety cap.
  - **Decode textures** — on by default (the headline feature).
  - **textures.big (optional)** — override; else auto-located at
    `…/graphics/pc/textures.big` next to `graphics.big`.
  - **Tools dir (optional)** — only if installed outside the repo.

---

## 6. Reproduce the evidence

```
# bpy-free core proof (writes core_decoded_<id>.png):
python tools/blender_addon/tests/test_core.py \
    "C:/…/Fable The Lost Chapters/data/graphics/graphics.big"

# Blender end-to-end (writes .blend + fable_bpy_image_*.png):
"C:/…/Blender/blender.exe" --background --factory-startup --python \
    tools/blender_addon/tests/blender_headless_test.py -- \
    "C:/…/data/graphics/graphics.big" MESH_SEAGULL_01 <out_dir>

# ANIM action round-trip (retail anim -> action -> donor-clone export -> compare):
"C:/…/Blender/blender.exe" --background --factory-startup --python \
    tools/blender_addon/tests/blender_anim_test.py -- \
    "C:/…/data/graphics/graphics.big" <out_dir>
```
