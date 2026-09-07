# Fable II Bowerstone Market scene import progress

## 2026-08-25: static architecture geometry package

The authoritative Fable2RE scene handoff is consumed without changing its
coordinates or inventing material/definition mappings.  The first strict
selection includes engine-level resource paths containing `\\buildings\\` or
`\\structures\\`.

- 13 source model blocks selected.
- 10 cooker-supported blocks compiled to TLC type-1 static mesh payloads.
- 34,919 vertices and 24,224 triangles independently decoded and verified.
- 3 unsupported low-poly house blocks (36, 187, 218; 188 source instances in
  total) remain explicit `missing_geometry` records.
- F2SCENE render coordinates are converted back to game coordinates as
  `(x,y,z) -> (x,z,y)`, including normals.
- Every submesh is retained as a separate TLC primitive.
- Materials are deliberately diagnostic and untextured (`diffuse_id=0`).

The standalone package is:

`D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\scene_import\compiled_architecture`

Its `mesh_manifest.json` records source hashes, resource identities, selection
rules, counts, payload hashes, and unresolved material status.  Independent
verification passes 117 checks with zero failures.

A disposable graphics archive candidate was built at:

`D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\scene_import\graphics_with_f2_architecture.big`

It adds the 10 meshes as model ids 8114 through 8123.  A second TOC/payload
audit passes 34 checks with zero failures.  `runtime_installed` is false: the
runtime clone and live installation were not modified.

Commands:

```powershell
python tools\convert_f2scene_static_meshes.py `
  D:\Documents\Fable2RE\generated\fableforge_bwsmarket_scene\resources\meshes\bwsmarket_derived.f2scene `
  D:\Documents\Fable2RE\generated\fableforge_bwsmarket_scene\instances.json `
  D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\scene_import\compiled_architecture

python tools\verify_f2scene_static_meshes.py `
  D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\scene_import\compiled_architecture

python tools\package_f2scene_static_meshes.py `
  D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\runtime_clone\data\graphics\graphics.big `
  D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\scene_import\compiled_architecture `
  D:\Documents\FableTLC\tmp\f2_bwsmarket_native_visual\scene_import\graphics_with_f2_architecture.big
```

## Remaining integration boundary

Native `game.bin` tail-append is supported when its full contract is followed.
The earlier creature/quest failures do not prove a general engine rejection:
they involved a formerly incorrect new-name CRC and stale donor self/owner row
indices. FableForge now has `ui clone-object-model`, which appends only at the
tail, rejects duplicate names, rewrites donor-index occurrences to the clone's
new global row, and patches `Graphic.modelId`.

An isolated ten-object definition package exists under
`scene_import\defs_append_probe`. It adds `OBJECT_F2_BWS_BLOCK_0000` etc. at
global rows 14761..14770. All ten names have correct `crc0` records, all reopen,
and the full 14,771-entry semantic roundtrip passes. The selected leaf donor
(`OBJECT_BARREL_UNBREAKABLE`) has no embedded self-index occurrences.

`architecture_placement_plan.json` joins those object/model identities to 55
authoritative routed instances across 15 BWSMarket tiles. All 55 have directly
representable unit scale and decoded authoritative yaw. No runtime TNG was mutated.

## Disposable TNG/WAD/stage candidate

The placement plan has now been emitted into 15 standalone TNG files under
`scene_import\architecture_tng`. They contain 55 globally unique authored UIDs
in the `0xFFFFFE02...` namespace. Static architecture is non-targetable, has no
health, and is neither game- nor level-persistent. Independent verification
passes 472 checks with zero failures; native TNG parse/serialize also passes.

`FinalAlbion.with_architecture.wad` replaces exactly those 15 TNG entries in the
already-corrected terrain WAD. WAD diff proves 858 entries before and after,
with 15 changed, zero added, and zero removed. BWSMarket_12 is not among the
changes, so its known arrival marker remains intact.

The coordinated four-file overlay is `scene_import\architecture_stage`:

- `data/graphics/graphics.big`
- `data/CompiledDefs/game.bin`
- `data/CompiledDefs/names.bin`
- `data/Levels/FinalAlbion.wad`

Its manifest covers 427,356,483 bytes; all 12 file existence/size/hash checks
pass. The overlay is now staged in the isolated runtime clone only. The prior
terrain/teleport stage and its original backups were preserved; the rollback
manifest now covers 10 files and every `had_original` entry has a `.forgebak`.
The live retail installation remains untouched and the game was not launched.

Post-stage validation is clean: 429 maps, 858 WAD entries, 455 STB entries,
zero missing LEV/TNG/static maps, zero ownership/membership/bounds issues. A
cross-system scene audit resolves all 55 authored things, definitions and
graphics; five distinct used meshes decode, with zero missing defs, graphics,
meshes, or textures (materials intentionally reference none).

`architecture_bounds_audit.json` transforms every decoded mesh by its routed
position and authoritative yaw. Zero of 55 instances are entirely outside the
terrain and zero have extreme bounds. Aggregate deployed AABB is approximately
`(6130.18,6261.00,8.07)` to `(6433.32,6537.11,77.69)`; the slight extension
past the terrain's west edge belongs to geometry whose placement is inside the
level, not a routing failure.

## 2026-08-25 live-test correction and current stopping point

The first launch was mistakenly made with `Fable.exe` directly and therefore
bypassed ForgeFSE. It was closed normally and all subsequent runs used
`FSE_Launcher.exe`. ForgeFSE then logged a successful retail transition handoff
through `CWorld::SetAsLoadingRegion`.

Two live frames prove that the terrain remains incorrect. Large black,
triangle-shaped holes cover foreground slopes and flats even though the runtime
STB is the intended coverage candidate (SHA-256
`83C0D48AD3A05E08115D0C7A52B7295126B12E1A70CBBCB420A3AD449D58912C`).
Therefore the earlier `mapping-0 base coverage` explanation/fix was incomplete.
Offline 512-triangle and nonzero-weight checks do not predict engine-visible
coverage. Texture probes still report resolved texture handles and `missing=0`.
Do not describe the black-hole problem as solved.

The initial architecture observation on `BWSMarket_12` was invalid: the routed
plan has zero supported architecture placements on tile 12. The test target was
corrected to WLD slot 413 / `BWSMarket_13`, which has five authoritative
placements. The map transition succeeded and the arrival check reported
`map=BWSMarket_13 in_target_bounds=true`, but no building was visibly identified
in the captured frame.

A direct definition/mesh diagnostic was then added beside the hero. Its first
attempt called nonexistent Lua method `CreateObjectWithRotation`; ForgeFSE
exposes both overloads under `CreateObject`. The resulting `pcall` failure was a
test-harness error and did not test `game.bin`. The Lua was corrected to:

```lua
Quest:CreateObject("OBJECT_F2_BWS_BLOCK_0004", pos, 0.0, "")
```

with unambiguous `OBJECT_CREATED` versus `NIL/ERROR` logging. The corrected
probe was launched but the operator ended the session before loading the save,
so it has **not run**. The clone was then closed normally; no Fable process is
left running.

### Exact resume sequence

1. Launch the isolated clone with `FSE_Launcher.exe`, never `Fable.exe`.
2. Load the known-good profile and allow the automatic slot-413 transition.
3. Read `FSE/FableScriptExtender.log` for the corrected `direct CreateObject`
   result before judging the frame:
   - nil/error means appended OBJECT registration or creation remains broken;
   - a valid object with no visible mesh means investigate type-1 mesh Info,
     materials/render flags, bounds/culling, or graphics-bank runtime loading.
4. Treat TNG auto-population separately from the direct probe. Only after the
   direct object renders should the five tile-13 TNG instances be diagnosed.
5. Reopen the terrain investigation from live foreground rendering semantics.
   The next useful comparison is a single retail patch versus a generated patch
   at the actual draw-call inputs (index/vertex buffers, per-layer blend/color,
   flags, and pass ordering), not another offline triangle-count assertion.

## 2026-08-26: offline foreground base-coverage correction

No visual/runtime test was performed. The isolated clone was closed and no
runtime package or installation was changed.

The known-good ForgeTest terrain supplied the decisive serialized comparison:
every patch has one full shared-strip `mapping=0` layer with 289 vertices,
`polygonCount=1085`, and `blend=255` at every vertex. The BWSMarket themed
builder instead partitioned mapping-0 geometry among material passes. For
example, BWSMarket_13 frame 0's first mapping-0 layer had only 274 vertices and
1,011 strip polygons. Across the tile, sparse base layers also contained
zero-blend vertices. Passing an offline union-of-triangles check did not prove
that the RenderForeground compositing path produced colour for every pixel.

FableForge `buildThemedForeground` now promotes the first deterministic
mapping-0 material in each patch to an opaque full-patch base. Other material
and directional layers remain weighted sparse overlays. A split-material unit
test asserts the recovered base contract (289 vertices, shared index buffer,
1,085 polygons, all blends 255). `forge_terrain_editor_tests` and the complete
`forge_tests` suite pass.

An offline BWSMarket_13 candidate was regenerated at:

`tmp/f2_bwsmarket_native_visual/material_chunks/BWSMarket_13.opaque_base.chunk.bin`

Its foreground gates pass:

- topology exact: 58/58;
- foreground parse/serialize roundtrip exact: 16/16;
- opaque mapping-0 base coverage complete: 16/16.

Frame 0 now has the expected full base: 289 vertices, 1,085 polygons, shared
index buffer, and blend 255 throughout. The candidate is a full donor-free
terrain emission, not the rejected in-place rebake attempt (that route could
not resolve every foreground directory record after the frame sizes changed).

Next offline step: regenerate all 30 BWSMarket chunks through the same full
emitter, rebuild the coordinated STB/package with relayout, and rerun structural
and cross-system audits. Keep it unstaged until visual testing is requested.

### Full 6144-grid corrected package

That offline step is complete. The earlier single-tile candidate above used the
old 8384-grid intermediate only to prove the layer fix. The current WLD places
the 5x6 BWSMarket grid at `(6144,6144)..(6464,6528)`, so all 30 chunks were
regenerated at those authoritative coordinates with bank indices 427..456.

Candidate source set:

`tmp/f2_bwsmarket_native_visual/opaque_base_6144_candidate/`

Aggregate foreground gates:

- 30/30 tiles pass;
- 432/432 foreground frame codec round-trips are exact;
- 432/432 frames have complete opaque mapping-0 base coverage;
- 2,329/2,329 emitted layer topologies verify exactly.

The 30 replacements were relaid into a fresh STB based on the architecture
clone. Re-extracting all 30 entries produced 30/30 SHA-256 matches against the
source chunks. Corrected STB:

- bytes: `628754994`;
- SHA-256: `659AD894F9FA24D53EFAFC44A6FFDD7C3F52166A6065407C948CE8FA533884AD`.

The new read-only `forge stb diff` payload audit proves the relayout scope:
424 entries are byte-identical, exactly 31 changed (the 30 named BWSMarket
static-map payloads plus `__STATIC_MAP_COMMON_HEADER__`), and zero were added or
removed. The reproducible build driver asserts that exact changed-name set.

A separate validation root passes `forge validate` CLEAN: 429 maps, 146
regions, 858 WAD entries, 455 STB entries / 429 static maps, zero missing
LEV/TNG/static maps, zero ownership/membership/bounds issues. The architecture
cross-system audit also passes: 15/15 TNGs parsed, 55/55 things positioned and
resolved, five/five distinct used meshes decoded, and zero missing definitions,
graphics, meshes, or textures.

The coordinated, **unstaged** five-file overlay is:

`tmp/f2_bwsmarket_native_visual/opaque_base_architecture_stage/`

Its `stage_manifest.json` covers 1,056,111,477 bytes across graphics.big,
game.bin, names.bin, FinalAlbion.wad, and the corrected FinalAlbion_RT.stb. All
five size/hash checks pass and `installed` remains false. Do not stage or launch
until visual testing is explicitly requested. This overlay targets the existing
isolated relocated-6144 runtime clone and requires its matching FinalAlbion.wld
and FinalAlbion.bwd; it is deliberately not described as a standalone retail-root
installer.

The full registered FableForge matrix passes 14/14 after the change, including
terrain editor, terrain textures, direction masks, mesh previews, formats, world
editing, navigation, and BWD tests.

Rebuild driver:

`tools/build_bwsmarket_opaque_base_stage.ps1`

It refuses a non-empty output directory, generates the authoritative 5x6 grid,
runs every per-tile foreground gate, emits the common records, relayouts from an
explicit source STB, asserts the 424/31/0/0 payload-diff scope, re-extracts all
30 map entries, and verifies their SHA-256 values against the generated chunks.

## Active task list (2026-08-26)

Offline work, in priority order:

1. **DONE.** Make the corrected 30-tile opaque-base build reproducible with one scripted
   command, including coordinates, bank indices, material bindings, common
   records, relayout, extraction/hash checks, and foreground gates.
2. **DONE.** Prove the relayout changed exactly the 30 intended BWSMarket static-map
   payloads and preserved every other STB entry byte-for-byte.
3. **DONE.** Produce a machine-readable package audit covering the five overlay files,
   the required WLD/BWD baseline hashes, terrain aggregate gates, validation
   counts, and architecture resolution counts.
4. **DONE / OFFLINE STAGE READY.** The three low-poly house blocks (36, 187,
   and 218) decode and compile, and the isolated graphics/definitions/TNG/WAD/
   terrain stage has been expanded and fully audited. Runtime testing remains
   queued and prohibited until explicitly requested.
5. **DONE / OFFLINE TEXTURED STAGE READY.** All 13 MDLs now have authoritative
   material identities, all 33 diffuse payloads cook successfully, and the TLC
   texture/mesh packages use deterministic imported texture IDs. No mapping was
   inferred visually.

Queued runtime/visual work (do not run until explicitly requested):

6. Run the corrected direct `CreateObject("OBJECT_F2_BWS_BLOCK_0004", ...)`
   probe and read the log before diagnosing TNG population.
7. Visually verify the opaque-base terrain candidate on BWSMarket_13.
8. Only after the direct object renders, diagnose the five tile-13 TNG
   architecture instances and then expand the check across the 15 populated
   tiles.

### Low-poly house gap resolution (2026-08-26)

Task 4 was not a TLC cooker-format failure. Blocks 36, 187, and 218 correspond
to 75/53/60 instances of `BS_Market_LowPoly_House2/1/3` respectively. Their
level-bank bodies use an untagged rigid layout: a two-byte marker, four
big-endian fields, 48-byte full-float vertices, and a restart-delimited index
strip. In all three files the inferred vertex/index boundary lands exactly at
EOF and every non-restart index is in range.

The parser bug was dispatch: the normal scanner returned an empty buffer rather
than throwing, so the existing foliage-layout fallback was never attempted.
`Fable2AssetBrowser/source/addons/fable_mdl_format.py` now narrowly retries that
layout only when every normal buffer is empty, and accepts it only when every
recovered buffer is non-empty and bounded. The headless parser self-tests pass
3/3, including a synthetic regression for this untagged layout.

The regenerated scene candidate is
`scene_import/bwsmarket_derived_lowpoly_fixed.f2scene`. A second issue was found
and fixed in `tools/convert_f2scene_static_meshes.py`: F2SCENE `m<N>` identifies
the distinct-model first-appearance ordinal, not the engine block number. The
converter now derives an explicit block-to-model-slot join from `instances.json`.
The resulting `compiled_architecture_lowpoly_fixed` candidate compiles 13/13
blocks, 35,276 vertices and 24,452 triangles; independent verification passes
173 checks with zero failures. The recovered additions are:

- block 36 / scene slot 16: 130 vertices, 81 triangles;
- block 187 / scene slot 22: 132 vertices, 87 triangles;
- block 218 / scene slot 25: 95 vertices, 60 triangles.

`scene_import/lowpoly_mdl_probe.json` records the authoritative header/body
sources and decoded buffer offsets. `scene_import/lowpoly_layout_analysis.json`
records the independent stride/index-boundary proof. No runtime or existing
coordinated stage file was changed.

### Expanded low-poly architecture stage (2026-08-26)

The 13-model graphics candidate assigns the original ten models to 8114..8123
and the recovered house models to 8124..8126. Its archive audit passes 57 checks
with zero failures. Three definitions were appended at global rows 14771..14773;
the complete 14,774-entry semantic definitions round-trip passes.

Routing retains 136 in-bounds architecture instances across 16 BWSMarket tiles.
The emitted TNG package passes 1,122 checks with zero failures. Relative to the
prior 55-object WAD, only `BWSMarket_27.tng`, `_28.tng`, and `_29.tng` change;
the WAD remains 858 entries with zero additions/removals.

The coordinated, non-installed five-file overlay is
`tmp/f2_bwsmarket_native_visual/opaque_base_architecture_lowpoly_stage`. A fresh
validation root reports CLEAN (429 maps, 146 regions, 858 WAD entries, 455 STB
entries / 429 static maps, zero missing or ownership/coherence issues). The
cross-system audit parses 16/16 TNGs, resolves 136/136 things, and decodes all
8/8 distinct meshes actually referenced by those placements. `installed` and
`runtimeTested` remain false.

### Architecture material trace

The ten compiled blocks contain 74 submeshes. Their derived F2SCENE source slots
are now retained as `source_material_slots` in `mesh_manifest.json` instead of
being discarded when the TLC diagnostic materials are created. The independent
verifier checks slot count and uniqueness; the package passes 137 checks with
zero failures. Rebuilding with this metadata changed zero of the 20 compiled
mesh/info payloads.

The source slots are numeric derived-scene identities only (blocks 0,4,5,7..13
cover slots 0..87 with gaps belonging to unselected blocks). The original
F2SCENE emits neutral synthetic `mat_<block>_<submesh>` declarations and contains
no retail texture names or hashes, so texture recovery was performed from the
explicit authoritative MDL header/body pairs instead of guessing from those
slots.

### Authoritative textured architecture stage (2026-08-26)

`tools/trace_f2_architecture_materials.py` decodes all 13 selected models and
records 76 material records, 77 geometry-to-material assignments, and 68 unique
retail texture paths in `scene_import/architecture_material_trace.json`.
`tools/audit_f2_architecture_texture_sources.py` resolves 67 payloads directly
across the level shared/global banks and finds the remaining bump map in the
level header inventory. Most importantly, all 33 distinct diffuse textures have
authoritative payloads.

`tools/cook_f2_architecture_textures.py` cooks those albedos to DDS: 33/33 pass,
zero failures. `tools/package_f2_architecture_textures.py` appends them in one
deterministic `textures.big` rebuild as IDs 6294..6326; independent texture-bank
validation reports 33/33 entries matching the TLC texture contract, and the
manifest-to-archive verifier passes 200 checks with zero failures. The
converter's optional `--material-trace` + `--texture-manifest` path assigns those
IDs to the 13 meshes. Geometry remains exactly 35,276 vertices / 24,452
triangles; the textured package passes 199 checks with zero failures. Of the 77
submeshes, 75 carry authoritative albedos and two source geometries genuinely
have no diffuse path and retain ID 0.

The newest non-installed coordinated overlay is
`tmp/f2_bwsmarket_native_visual/opaque_base_architecture_textured_stage`. It is
a six-file package adding `data/graphics/pc/textures.big` to the prior five-file
terrain/architecture overlay. Its manifest hashes all six payloads exactly and
keeps `installed=false`. A fresh validation root is CLEAN. The texture-aware
scene audit resolves 136/136 placed objects, decodes 8/8 used meshes, resolves
23/23 distinct diffuse textures referenced by those placements, and decodes
23/23 texture previews. Runtime testing remains explicitly queued.

`tools/package_f2scene_architecture_stage.py` now supports `--textures`, `--stb`,
`--baseline-wld`, and `--baseline-bwd`, so the complete six-file overlay and its
baseline contract are generated rather than hand-assembled. A clean rebuild at
`opaque_base_architecture_textured_stage_repro` contains 1,598,021,577 payload
bytes and matches all six hashes in the primary textured-stage manifest exactly;
the WLD/BWD baseline hashes also match exactly.

Rollback remains `forge unstage <runtime_clone>`. The extended rollback manifest
tracks 10 files and all original-file backups are present. The retail install
was never modified.
