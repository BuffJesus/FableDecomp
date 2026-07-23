# NEW-MESH COMPOSITION — authoring brand-new compiled meshes into graphics.big

*2026-07-20. The step from "edit existing meshes" (segment-capture round-trip,
`docs/BIG_MESH_GEOMETRY.md`) to "author custom assets": compose a complete
compiled-mesh payload from arbitrary geometry (new topology / vertex / face
counts) and add it as a NEW entry in a copy of `graphics.big`.
**v2 (same day): SKINNED (type-5) composition** — skeleton blocks + animated
blocks + per-vertex weights, the last unproven link in the custom-equipment
recipe (`docs/APPEARANCE_SYSTEM.md` §7).*

Code: `tools/blender_addon/io_scene_fable/mesh_rw.py` (`compose_mesh`,
`decode_to_compose_args`, `clone_skeleton`) + `tools/big_write.py`
(`rebuild(buf, edits, adds=)`).
Test/demo CLI: `tools/blender_addon/tests/compose_test.py`.

Answer key: EgoCore (MIT) `MeshCompiler.h::CompileSingleLOD/CompileForExport`
(the exact write-side serializer) + `GltfMeshImporter.h::ImportType1/ImportType5`
(glTF → C3DMeshContent, i.e. this task in C++) + `MeshParser.h`
(`SerializeEntryMetadata` = the TOC Info blob) — every field below was
cross-checked against both EgoCore and retail entries.

## 1. Payload recipe (static, v1)

A composed payload is `LOD0 block || ghost LOD`. All multi-byte fields LE.

### LOD0 block (EgoCore `CompileSingleLOD`, static subset)

| field | type | value we emit |
|---|---|---|
| MeshName | cstr | entry name, must start `MESH_` (payload classifier) |
| AnimatedFlag | u8 | 0 |
| BoundingSphereCenter / Radius | 3f + f | computed from all verts (bbox center, max distance) |
| BoundingBoxMin / Max | 3f + 3f | computed from all verts |
| HelperPointCount, DummyObjectCount, PackedNamesSize, MeshVolumeCount, MeshGeneratorCount | 5×u16 | all 0 (no optional blocks follow) |
| MaterialCount | i32 | `len(materials) + 1` (see sentinel below) |
| PrimitiveCount | i32 | `len(prims)` |
| BoneCount, BoneNameSize | 2×i32 | 0 (STATIC v1 — no bone blocks emitted) |
| ClothFlag | u8 | 0 |
| TotalStaticBlocks, TotalAnimatedBlocks | 2×u16 | `len(prims)`, 0 (one static block per primitive) |
| RootMatrix | 12f | identity 3×3 + zero translation |
| materials | see below | real materials, then the retail sentinel |
| primitives | see below | one per prim |

**Material record:** `ID:i32` (slot index), `Name:cstr`, `DecalID, DiffuseMapID,
BumpMapID, ReflectionMapID, IlluminationMapID: 5×i32` (textures.big entry IDs,
0 = none), `MapFlags:i32` (bit0 diffuse, bit1 bump, bit2 reflect, bit3 illum —
we derive from the non-zero IDs), `SelfIllumination:i32`, `IsTwoSided,
IsTransparent, BooleanAlpha, DegenerateTriangles, UseFilenames: 5×u8`
(UseFilenames=0, so no filename strings follow).

**Retail sentinel (mirrored):** every retail static mesh checked
(`MESH_OBJECT_PLATE`, `MESH_OLD_VILLAGE_SIGN`, `MESH_OBJECT_RAILING_MID`,
bear) ends its material list with a material named `DegenerateTriangles`
(ID 0, all map IDs 0, MapFlags 0, bools 0,0,0,1). `compose_mesh` appends it;
`decode_to_compose_args` strips it on decode.

**Primitive record:** `MaterialIndex:i32`, `RepeatingMeshReps:i32` (=0),
`SphereCenter:3f + SphereRadius:f` (per-prim bounds), `AvgTextureStretch:f`
(0.1, EgoCore default), `StaticBlockCount:u32`(=1), `AnimatedBlockCount:u32`(=0),
`VertexCount, TriangleCount, IndexCount(=3*tris), InitFlags: 4×u32`,
`StaticBlockCount_2, AnimatedBlockCount_2: 2×u32` (repeat), then per static
block `{PrimitiveCount(=tris):u32, StartIndex:u32(=0), IsStrip:u8(=0 — we emit
triangle LISTS), ChangeFlags:u8, DegenerateTriangles:u8, MaterialIndex:i32}`,
then `CVertexCompressionParams` (4f scale + 4f offset), `VertexStride:u32`,
`BufferType:u32`(=0), the **vertex buffer** (chunked-LZO framed), the **u16
index buffer** (chunked-LZO framed), `ClothPrimitiveCount:u32`(=0).

**Index winding:** the decoder (`fable_core._prim_geometry`, list path) emits
faces as `(ib[k], ib[k+2], ib[k+1])`, so `compose_mesh` stores each face
`(a,b,c)` as indices `a,c,b` — `decode(compose(x)) == x`.

**LZO framing:** each buffer is a Fable chunked-LZO block (`[u16 clen][clen
bytes]`, `clen==0` stored, final 3 bytes plain). `compress=True` (default) uses
the proven `lionhead_lz_compress.compress_fable_block`; `compress=False` emits
stored chunks — both engine-valid. An empty buffer is a lone `u16 0`.

### Vertex layouts (both retail-attested for type-1 static)

Survey of 400 random type-1 retail entries: `init 0x04/stride 12` ×443 prims,
`0x14/20` ×68, `0x06/20` ×68 (bump), `0x16/28` ×25 (bump). No float2-UV
strides (24/32/36) exist in retail type-1 — the "uncompressed" retail static
layout is float3 positions with PACKED normal/UV, so that is what we emit:

| layout | InitFlags | stride | position | normal | UV |
|---|---|---|---|---|---|
| `'float'` (default) | 0x14 | 20 | FLOAT3 (exact) | packed 11/11/10 dword | 2×i16, `(v+8)*2048` |
| `'packed'` | 0x04 | 12 | POSPACKED3 11/11/10 via comp params | packed dword | 2×i16 |

`'packed'` compression params (EgoCore ImportType1): `offset = bbox center`,
`scale = extent*0.505` per axis (min 1e-4), `scale[3]=1, offset[3]=0`.
Quantisation ≈ extent/2048 (x/y) — fine for props; use `'float'` when exact
positions matter. UV precision is 1/2048 in both layouts. Packers
(`pack_normal`, `compress_uv`, existing `pack_pos_packed3`) are exact inverses
of the decoder, so re-encoding decoded data is lossless.

**V flip:** `_prim_geometry` yields Blender-convention `1-v`; `compose_mesh`
takes the same convention and stores `1-v` back.

### Ghost LOD (retail convention)

Retail single-LOD type-1 entries all carry a trailing "ghost" LOD: a full mesh
header (same name/bounds) with MaterialCount=PrimitiveCount=0 and the identity
RootMatrix, NOT counted in the Info LODCount (verified: `MESH_OBJECT_PLATE`
size 1189 = LODSizes[0] 971 + 218-byte ghost; EgoCore `CompileForExport`
appends the same for types 2/5). `compose_mesh` appends it.

### Entry Info blob (TOC subheader)

EgoCore `SerializeEntryMetadata`, byte-validated against retail (infosize
`60 + 4*texCount`): `PhysicsIndex:i32` (0 = none — retail uses the entry id of
a type-3 physics BBM when present), `BSCenter:3f, BSRadius:f, BBMin:3f,
BBMax:3f` (same as payload header), `LODCount:u32=1`,
`LODSizes[0]:u32 = len(LOD0)` (ghost excluded), `SafeBoundingRadius:f=0`
(retail: 0), *(no LODErrors when LODCount==1)*, `TextureIDsCount:u32` +
`i32[]` (unique non-zero map IDs across materials — this is what
`parse_mesh.parse_mesh_descriptor` recovers as `matTexId` and what
`build_model` binds textures from).

## 1b. SKINNED payload recipe (type-5, v2)

`compose_mesh(name, prims, materials, skeleton=clone_skeleton(donor_pay, 5))`.
Deltas vs the static recipe (all retail-verified on goblin/seagull/hero boots
+ EgoCore `ImportType5`/`CompileSingleLOD`):

* **AnimatedFlag = 1** in the header (LOD0 AND ghost).
* **BoneCount / BoneNameSize** real; the four bone blocks
  (`BoneIndices u16[]`, names, 60-B `C3DBone`s, 48-B TRS, 64-B IBMs — layouts
  in `BIG_MESH_GEOMETRY.md` §7) are emitted between `TotalAnimatedBlocks` and
  `RootMatrix`. `clone_skeleton()` captures them RAW from a donor mesh
  (retail boot / creature / the mesh's own decode), so they survive
  **byte-exact** — no bind-matrix re-derivation.
* **NO `DegenerateTriangles` sentinel material** — checked: retail skinned
  meshes don't carry one (`decode_to_compose_args` conditions on it).
* Skinned prims have **StaticBlockCount=0** and one or more `CAnimatedBlock`
  records: `{PrimitiveCount, StartIndex, IsStrip=0, ChangeFlags=0, Degenerate=0,
  VertexCount, BonesPerVertex=3, PalettedFlag=1, groupCount, Groups[]}` —
  `BonesPerVertex=3 / PalettedFlag=1` on every retail skinned prim checked.
* **Vertex layouts** grow 4 joint bytes + 4 weight bytes after the position
  (§7.1): `'float'` → InitFlags 0x14 / stride **28** (retail hero boots),
  `'packed'` → 0x04 / stride **20** (retail creatures). Joint byte =
  `paletteIndex*3` into the block's `Groups[]` (LOCAL bone slots); weight
  bytes forced to sum **exactly 255** (82,073/82,073 retail skinned verts
  checked sum to 255; max 3 non-zero influences retail-wide).
* **Block partition** (`_auto_skin_blocks`): single block covering all verts
  when the used bone set fits one palette; else EgoCore's greedy face walk
  (palette cap 16 — retail goes to 18, EgoCore's proven emit is ≤16) with
  per-block vertex regrouping/duplication. `decode_to_compose_args` instead
  passes `skin_blocks` preserving the SOURCE partition, so a recompose keeps
  the original palettes and vertex order.
* Per-vertex skin input: `prim['skin'][v] = [(bone, weight), ...]` where bone
  is a local slot int **or a bone NAME** (resolved via the skeleton's name
  list — the authoring-path format: "rig to `Bip01 L Foot` etc.").
* **Ghost LOD carries the skeleton too** (AnimatedFlag=1, MaterialCount=
  PrimitiveCount=0, full bone blocks + RootMatrix). EgoCore clones the full
  skeleton into the ghost; retail ships a REDUCED ghost skeleton (boot ghosts:
  1 bone `Scene Root`; goblin: 26 of 67) — both parse, we emit the full clone.
* Info blob unchanged (LODCount=1; boots retail-attest exactly this shape).
  Add the entry with **type=5**.

Decoded LOD counts: retail creatures ship 3–4 LOD chains + ghost; composed
meshes are LOD0 + ghost only (engine renders LOD0 at all distances).

## 2. big_write `adds=` design (new TOC entries)

`big_write.rebuild(buf, edits=None, adds=None)`; each add:
`{sub, name, payload, type, info=b'', id=None, deps=[], crc=0, timestamp=0}`.

* **Record**: synthesized in the exact `parse_bigb.parse_toc` field order
  (mirrors EgoCore `BigBankCompiler`): `magic=42:u32, id, type:i32, size,
  offset, crc, name (u32-len-prefixed, NO trailing NUL — retail stores exact
  length), timestamp, depCount + deps, infoSize + info`. `size`/`offset` are
  patched at +12/+16 by the existing layout loop, same as retail records.
* **ID allocation**: default `max(id in subbank) + 1`. Retail TOCs are
  id-sorted with unique ids (verified on both graphics.big subbanks), so
  appending keeps the order invariant. Duplicate names are rejected.
* **Stats header**: the subbank TOC's leading type histogram
  (`u32 n + n×{type,count}`) is rebuilt with the added types incremented
  (sorted by type, matching retail).
* **Footer**: the subbank's `entry_count` is bumped.
* **CRC/timestamp**: 0. Retail CRC is NOT a CRC-32 of the name or dep path
  (tested); EgoCore ships new entries with CRC 0 and the engine loads them,
  so we do the same. `deps` (the source `.bbm` path in retail) left empty.
* API stability: `rebuild(buf, edits)` signature and no-adds behavior
  unchanged (content round-trip regression in compose_test §4 passes).

## 3. Validation results (2026-07-20, all PASS)

`python tools/blender_addon/tests/compose_test.py <copy of graphics.big>`:

1. **RECOMPOSE oracle** — `MESH_OBJECT_STATUE_BEAR` (2983 verts, packed
   stride 12, 38-bone skeleton block dropped), `MESH_OBJECT_PLATE`,
   `MESH_OLD_VILLAGE_SIGN`: decode → `compose_mesh` → re-decode. Verts/faces
   **exact** (packed re-quantisation through the original comp params is
   lossless), UVs exact, material texture IDs preserved.
1b. **RECOMPOSE oracle, SKINNED** — `MESH_CREATURE_GOBLIN` (67 bones, 4 prims,
   7 animated blocks, strips, packed 0x04/20), `MESH_SEAGULL_01` (27 bones),
   `MESH_HERO_APPRENTICE_BOOT_L_01` + `_ASSASSIN_` (10-bone hero-rig subsets,
   float 0x14/28): decode LOD0 → compose(skeleton=) → re-decode. Verts/faces/
   UVs exact; `Groups[]` palettes **identical**; per-vertex skin decode
   identical (weights within 1/255, bone sets equal); the four skeleton blocks
   **byte-exact** (`clone_skeleton(recomposed) == clone_skeleton(source)`);
   decoded bones (names/parents/IBMs/global ids) identical; ghost LOD parses
   with 0 prims + full skeleton. Grammar-exact, not byte-exact: strips become
   lists, deeper LODs + helpers/dummies/packed-names/cloth are dropped.
2. **SYNTHETIC** — textured cube (24v/12f, `'float'`) and 5-blade grass
   cluster (20v/10f, `'packed'`, two-sided transparent): compose → parse →
   geometry matches authored (float layout exact; packed within 1 lsb).
2b. **SYNTHETIC SKINNED** — `MESH_FORGE_TEST_BOOT` (the custom-equipment
   shape: donor boot geometry ×1.15, re-skinned **by bone NAME** to the same
   10 hero-rig bones, auto single-block) and `MESH_FORGE_TEST_RIBBON`
   (41 bone slots of the goblin skeleton → forces the greedy **multi-block**
   partition, 3 blocks, palettes ≤16, vertex duplication): compose → parse →
   face-correspondence check proves positions + per-corner skin match the
   authored data.
3. **NEW-ENTRY oracle** — cube/grass/boot added to a COPY of graphics.big as
   ids 8113/8114/**8115 (type 5)** (MBANK_ALLMESHES, TOC walks CLEAN,
   `entry_count` 8115): re-opened with `fable_core.open_graphics_big`, found
   by name, decoded geometry + Info texture IDs match authored; `build_model`
   produces full models — the boot with `has_skeleton`, the donor's 10 bones
   by name, and `GeometryData.skin` matching the authored influences.
4. **No-edit regression** — `rebuild(buf)` round-trips all 8113 entries'
   record fields + payload bytes (big_write's documented contract; whole-file
   byte identity is `bigb_write.py`'s separate in-place contract).

Note: `parse_bigb.py`'s CLI prints `DEVIATES` for MBANK_ALLMESHES on the
modified copy — the UNMODIFIED retail graphics.big prints the same (its
"TOC end == footerOffset" invariant only holds for the LAST subbank of a
multi-subbank file); both TOCs parse 100% of entries either way.

Existing suites re-run after the change, all PASS: `tests/test_core.py`,
`blender_headless_test.py` (seagull), `blender_bones_test.py`,
`mesh_rw.roundtrip_report` on the bear (`ok: True`).

Blender end-to-end: see §5.

## 4. Honest limits (v2)

* ~~STATIC only~~ **LIFTED (v2)**: skinned type-5 composition works via
  `skeleton=clone_skeleton(donor)`. Remaining skeleton limits: the skeleton
  must be CLONED from an existing mesh (no from-scratch skeleton authoring —
  fine for equipment, which must reuse hero-rig bone names anyway; a
  from-scratch path would need NameCRC + LocalizationMatrix + IBM emission,
  all layouts already documented in `BIG_MESH_GEOMETRY.md` §7). Static meshes
  that carry a skeleton but no skinned prims (the bear) still DROP it on
  recompose.
* **LOD0 only** (+ retail ghost LOD). No LOD chain authoring (LODCount fixed
  at 1; engine will render LOD0 at all distances). Retail creatures ship 3–4
  LODs; a recomposed goblin keeps only LOD0.
* Layouts: `'float'` (0x14, stride 20 static / 28 skinned) and `'packed'`
  (0x04, stride 12 static / 20 skinned) only — no bump/tangent layouts
  (0x06/0x16), no repeated-instancing (Reps>1), no cloth (goblin prim0's
  cloth primitive is dropped on recompose), no helpers/dummies/volumes/
  generators/packed-names.
* Skin: max 3 influences/vertex (`BonesPerVertex=3`, retail-wide fact),
  palette cap 16 bones per animated block (EgoCore's proven emit; retail
  reaches 18; hard grammar limit is 85 since joint byte = paletteIndex*3).
* Triangle lists only (retail also uses strips; lists are engine-valid — the
  static-block IsStrip flag selects per block, and retail mixes both).
* Materials bind by textures.big entry ID; no new-texture authoring here
  (that's the concurrent texture agent's writer).
* **Engine smoke test not run** (no game launch in this task): the payload
  is grammar-identical to retail and every field mirrors retail/EgoCore
  conventions, but "loads in Fable.exe" has not been observed. A follow-up
  should wire a composed boot into a `CAppearanceModifierDef` Graphics[] id
  (APPEARANCE_SYSTEM.md §7) and equip it in-game.
* Blender EXPORT operator wiring (`import_op.py` "Export NEW mesh" path
  following the existing ExportFableModel pattern) is the natural follow-up,
  NOT part of this task.

## 5. Blender end-to-end proof

`tools/blender_addon/tests/blender_compose_test.py` (headless): composes the
cube+grass **+ the NEW SKINNED boot** (donor geometry scaled, re-skinned by
bone name, skeleton cloned), adds them to a copy of graphics.big (boot as
type 5), then runs the EXISTING import operator (`import_scene.fable_model`)
against the modified copy with `textures_big=` pointing at the retail
textures.big, and asserts: real textured meshes appear, and the boot comes
back with an armature carrying exactly the donor's 10 bones, an ARMATURE
modifier + parenting, vertex groups that are all real bones, and per-vertex
weights matching the authored skin (≤1.5/255) — i.e. **armature binding +
weights survive the full compose → inject → import round trip**. Saves
`.blend` + render PNG as proof.

Result: see `tools/blender_addon/tests/proof/` (`fable_compose_*.blend/png`).

## 6. Reproduce

```
cp <install>/data/graphics/graphics.big <scratch>/graphics_copy.big
python tools/blender_addon/tests/compose_test.py <scratch>/graphics_copy.big
"C:\Programs\Blender\blender.exe" --background --factory-startup --python \
    tools/blender_addon/tests/blender_compose_test.py -- <scratch>/graphics_copy.big <out_dir>
```
