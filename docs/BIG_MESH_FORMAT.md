# graphics.big MBANK — mesh/model format (GBANK/MBANK)

*Authored 2026-07-19 (mesh agent). Status: **mesh DESCRIPTOR + material bindings + submesh
counters + vertex-format-class model VALIDATED self-consistently across thousands of retail
entries.** The packed vertex buffer and the compressed index stream are **identified but not
byte-decoded** (they use Big Blue Box's proprietary control-byte compression — documented gap,
needs the engine decompressor / Ghidra, not pure bytes).*

Parser: `tools/parse_mesh.py` (pure Python; depends read-only on `tools/parse_bigb.py`).
Sources: raw retail bytes of `graphics.big` + `textures.big`; the fabletlcmod.com wiki
`file_formats:bbm` spec (fetched via a text-proxy because the site's TLS cert fails verification —
snippets cross-checked against the bytes, flagged where only wiki-sourced); the `fable_format` Rust
crate (docs.rs) as a second community RE reference. Fields whose *purpose* is inferred are marked
**[hypothesis]**.

This doc EXTENDS `docs/BIG_FAMILY_FORMATS.md` (§5 left MBANK "payload not decoded") and
`docs/BIG_ROUNDTRIP.md` §7 (CRC = per-format-class fingerprint). It does not modify them.

---

## 0. TL;DR

- `graphics.big` = `MBANK_ALLMESHES` (8,112 entries) + `MBANK_ENGINE` (1). Entry `Type` splits the
  bank into **three payload families**, all Big-Blue-Box "BBM" (the entry Dep is a `*.bbm` build path):
  - **Types 1,2,4,5 = compiled meshes** (3,295 entries): a name-prefixed **"compiled model"** — the
    engine's runtime mesh (skeleton + LODs + submeshes + packed verts + compressed indices).
  - **Type 3 = tagged BBM** (1,383 entries, mostly `[PHYSICS]`/collision): the **`3DMF` chunk
    container** (self-describing FourCC chunks — matches the wiki spec verbatim).
  - **Types 6,7,9 = animations** (3,435 entries, `ANIM_*`): NOT meshes; out of scope here.
- The per-entry TOC **`Info` descriptor** of a compiled mesh is fully decoded and **self-checks with
  zero leftover on 3,294 / 3,295 meshes**; its material texture-IDs resolve **100 % (9,148/9,148)** to
  real `textures.big` entries with correct semantic names.
- Submesh counters decode: **6,449 / 6,449 detected `SUBM` headers satisfy `nFaceVertexIndices ==
  3 × nFaces`**, and the vertex-format code set is exactly **{4, 6, 20, 22}** — the wiki's `sVert`
  table. `sVert` is the "vertex-format class"; it correlates with the entry CRC (§7 of BIG_ROUNDTRIP).

---

## 1. Bank layout recap + type → family map

`parse_bigb` already walks the container. Within MBANK the interesting axis is `Type`:

| Type | count | InfoSize | family | payload leading bytes | note |
|-----:|------:|----------|--------|-----------------------|------|
| 1 | 2,584 | 60–176 (var) | compiled-mesh | `MESH_…\0` (36 are `BANDIT_`/`TABL`/`GAME`…) | general props/creatures |
| 2 | 55 | 64–72 | compiled-mesh | `MESH_…\0` | grass/bramble (alpha foliage) |
| 4 | 152 | 64–92 | compiled-mesh | `MESH_…\0` | fx / translucent |
| 5 | 504 | 64–124 | compiled-mesh | `MESH_…\0` | skinned/complex |
| 3 | 1,383 | 0 | tagged-bbm | `…\0\0(3DMF…` | collision/physics `[PHYSICS]` |
| 6 | 3,272 | 24 | anim | `f32 count…` | `ANIM_*` |
| 7 | 56 | 24 | anim | `f32 …` | `ANIM_*` |
| 9 | 107 | 24 | anim | `f32 …` | `ANIM_*` |

`Type` is not a clean vertex-format axis (all four mesh types carry all `sVert` codes); it is closer
to a **render-bucket / material-class** (opaque, alpha-foliage, translucent, skinned). The real
vertex-format class is the per-submesh `sVert` code (§4) and the entry CRC (§7 BIG_ROUNDTRIP).

Anim (6/7/9) `Info` (24 B) = two f32 (playback params, e.g. `3.23, 3.20`) + 4 zero dwords; the payload
starts with a keyframe count. Documented only as "not a mesh"; skeletal-anim decode is future work.

---

## 2. Compiled-mesh TOC `Info` descriptor  ✅ VALIDATED (zero leftover)

The variable-length `Info` blob (in the TOC record, `InfoSize` per entry) is a **copy of the compiled
payload's header block plus the LOD table and material bindings**. Little-endian, dword-granular:

```
u32   flags                 # observed 0
f32   origin[10]            # bbox / pivot / bounding-sphere block  [hypothesis on exact field split]
u32   nLOD                   # 1..8 (retail: almost always 3)
u32   lod[nLOD]              # lod[0] = BYTE OFFSET (in payload) to the LOD0 sub-block;
                             #   VERIFIED: payload[lod[0] : lod[0]+len(name)] == the entry name.
                             #   lod[1..] = DESCENDING values = LOD block sizes  [hypothesis]
u32   pad0                   # observed 0
f32   lodDist[k]             # 1 or 2 LOD switch-distance floats (k varies: 0.025, 0.040, 0.065 …)
u32   nMat                   # material count
u32   matTexId[nMat]         # each = a textures.big ENTRY ID (the material's BASE texture) ✅
```

**Self-check / how it is parsed robustly:** the variable mid-section (`pad0` + 1–2 `lodDist` floats)
is not fixed-width, so the parser recovers `nMat` from the **tail**: `nMat` is the count value whose
position leaves exactly `nMat` trailing dwords. This makes the parse *zero-leftover* by construction —
a wrong split fails. Combined with the `lod[0]`→name check it is a strong validator.

**Result (retail `graphics.big`, types 1/2/4/5):**
`3,294 / 3,295` parse zero-leftover with `lod[0]` landing on the payload's repeated entry name.
The single miss is `MESH_ENGINE_UNIT_BOX` (`InfoSize=60`, the minimum) — a degenerate engine unit box.

**Material binding — VALIDATED against textures.big (read-only cross-ref):**
`9,148 / 9,148` (100 %) `matTexId`s resolve to real `textures.big` entry IDs, with correct names:

| mesh | matTexId | resolved textures.big name |
|------|---------|----------------------------|
| `MESH_OBJECT_STATUE_BEAR` | 237, 238 | `…BROWNBEAR_BODY_32.TGA`, `…BROWNBEAR_HEAD_32.TGA` |
| `MESH_OBJECT_STATUE_BEAR_CUB` | 239, 240 | `…BEAR_CUB_BODY_32.TGA`, `…BEAR_CUB_HEAD_32.TGA` |
| `MESH_SUMMONER_BLUE_01` | 27,696,697,699,701 | reflection + `SUMMONER_ALPHA/HEAD/LEGS/TORSO_01.TGA` |
| `MESH_JOB_MASK_01` | 3749 | `…JACKOFBLADES\JACKOFBLADES_01.TGA` |

So `matTexId[i]` is the **BASE texture ID** of material *i*. The wiki lists `BASE_Texture_ID`,
`BUMPMAP_Texture_ID`, `REFLECT_Texture_ID` per material; only one ID per material appears in the
compiled `Info` array — bump/reflect IDs live in the payload material block (tagged-BBM `MTRL`, §5),
not exhaustively confirmed for the compiled variant. **[hypothesis: compiled `Info` keeps only BASE.]**

`origin[10]` exact split is unconfirmed — it is *not* a clean `min3/max3` pair (max<min for some
axes). Candidates: pivot(3) + AABB(6) + radius(1), or min3/max3/centre3/radius. The values are sane,
bounded, and identical between the TOC `Info` and the payload header (self-consistent). Treated as an
opaque 10-float bound block for now. Evidence dump:
`scratchpad/mesh_agent/seagull_origin_pts.obj` (the 10-float block of `MESH_SEAGULL_01`).

---

## 3. Compiled-model payload — container shape

A compiled-mesh payload (`MESH_…` prefixed) is laid out as:

```
char[]  name '\0'                    # == entry name (verified)
u8      skel_flag                    # 1 = has skeleton, 0 = static (verified: seagull=1, statue=0)
f32     origin[10]                   # same 10 floats as Info descriptor
…       helper points / dummies      # named ('focus_eye_01', 'movement_dummy'), count-prefixed
…       skeleton (if skel_flag)      # 3ds-Max Biped bone-name hierarchy:
                                      #   'Scene Root','Bip01',' Pelvis','Neck','Head',
                                      #   'L Clavicle','L UpperArm','L Thigh','…Calf','…Toe0','Tail'
[ per LOD block, nLOD of them ] :
    char[] name '\0'                  # the mesh name REPEATED (block delimiter — lod[0] points here)
    …      submesh table              # named submeshes ('Body','Wing') each with an id == matTexId slot
    [ per SUBM ] : 5×u32 header (§4) + compressed index stream + packed vertex buffer
    …      material block             # 'GMAT'/'GMATE' material records (payload-side)
```

The **skeleton is a bone-name string hierarchy** embedded in the payload (confirmed by reading the
classic Max Biped names). Bone matrices follow the tagged-BBM `BONE` layout (§5); in the compiled
model they are interleaved with the names. Skeleton/bone references for skinned meshes therefore live
**inside the payload**, not in the `Info` descriptor. **[hypothesis: per-vertex bone indices are the
`sVert`={4(20B),6,22} "bones" component; static meshes use `sVert`={4(12B),20(20B)} without bones.]**

---

## 4. Submesh header (`SUBM`) + vertex-format class (`sVert`)  ✅ VALIDATED

Each submesh begins with a 5-dword header (byte-granular, not 4-aligned within the block):

```
u32  nVerts                 # unique vertices in this submesh
u32  nFaces                 # triangles
u32  nFaceVertexIndices     # == 3 * nFaces   ✅ (100% of detected headers)
u32  sVert                  # VERTEX-FORMAT CLASS code: one of {4, 6, 20, 22}
u32  flag                   # small (0/1) — [hypothesis: destroyable-mesh-level / strip flag]
```

(The wiki's fuller `SUBM` is `nFaceVertexIndices, nFaceVertexIndices_BoneIndice, nVerts, nFaces,
nSourceVerts`; the compiled header exposes the load-bearing counters above.)

**Validation:** scanning all compiled-mesh payloads for this signature yields **6,449 candidate
`SUBM` headers, of which 6,449 (100 %) have `nFaceVertexIndices == 3 × nFaces`** and `sVert ∈
{4,6,20,22}` — the exact vertex-format code set the wiki documents. This is the self-consistency proof
for the submesh model. Distribution: `sVert 4 → 5,837` · `6 → 196` · `20 → 384` · `22 → 32`.

### `sVert` → stride & vertex layout (fabletlcmod.com wiki `file_formats:bbm`)

| sVert | stride | fields |
|------:|-------:|--------|
| 4  | **12** | packed coords, packed normals, s11e4 tu tv |
| 4  | **20** | packed coords, **bones**, packed normals, s11e4 tu tv |
| 4  | 36 | float coords, float normals, float tu tv, dword meshlevel |
| 6  | **28** | packed coords, bones, packed normals, s11e4 tu, tv, dword[2] |
| 20 | **20** | float coords, packed normals, s11e4 tu tv |
| 20 | 28 | float coords, bones, packed normals, s11e4 tu tv |
| 22 | 36 | float coords, bones, packed normals, s11e4 tu, tv, dword[2] |

Notes on the components (wiki + byte evidence):
- **packed coords** = quantized position (16-bit-class), **dequantized against the mesh bound block**
  (`origin[10]`); the exact scale/offset is **[hypothesis]** (wiki gives no dequant formula, and the
  bytes could not be validated without decompressing the vertex stream — see §6). Meshes with float
  positions (`sVert 20/22`) store raw `f32×3` and the tagged-BBM `VERT` chunk's first triple decodes
  cleanly as such (`(-333.78, 509.40, -186.50)` for a wall-collision mesh), corroborating float coords.
- **packed normal** = one dword (10/11-bit packed), **s11e4 tu/tv** = 16-bit float-like UVs.
- A given `sVert` code selects among its stride variants by the presence of bones (skinned) — i.e. the
  code alone is ambiguous; skel_flag / block context disambiguates. **[hypothesis]**

---

## 5. Tagged BBM (Type 3, `3DMF`) — the self-describing variant

Type-3 payloads are the classic **tagged BBM** and contain FourCC chunks exactly as the wiki lists.
Chunk presence over all 1,383 type-3 entries: `3DMF 1383 · MTLS 1383 · SUBM 1284 · PRIM 1367 ·
UNIV 1356 · VERT 600 · HLPR 794 · BONE 1`. Layout (wiki, cross-checked against bytes):

```
3DMF  "Copyright Big Blue Box Studios Ltd." then 3DRT
  MTLS   material list →  MTRL { u32 BASE_Texture_ID, BUMPMAP_Texture_ID, REFLECT_Texture_ID,
                                 Glow_Strength, Alpha_Enabled, Degenerate flag }  (+ MTLE/MMAP)
  SUBM   sub-mesh { nFaceVertexIndices, nFaceVertexIndices_BoneIndice, nVerts, nFaces, nSourceVerts }
    TRFM   transform matrix
    PRIM   primitive → TRIS triangle list  (compressed, §6)
    VERT   vertex list  /  UNIV unique-vertex info  (compressed, §6)
    SMTH   smoothing groups ;  VGRP vertex groups ;  BONE bones (below) ;  CLTH cloth
  HLPR   helpers → HPNT points, HDMY dummies, HCVL convex volumes
BONE  sub-chunk1 { long Index, Parent, nChild; f32 Matrix[12] }
      sub-chunk2 { f32 Matrix[12] } ; sub-chunk3 { f32 Matrix[16] }
```

The chunk framing is `TAG` + a control-byte-encoded length/count (NOT a plain u32) — the same BBB
compression as the compiled stream (§6). Only the FIRST `VERT` position triple reads as clean `f32×3`;
subsequent vertices are separated by control bytes (`7c 00 04` …), i.e. the vertex list is compressed
too. So tagged-BBM is *self-describing at the chunk level* but its geometry payload is still compressed.

---

## 6. DOCUMENTED GAP — the compressed vertex/index streams

Both families store geometry with **Big Blue Box's proprietary control-byte compression** (the wiki
calls it "compressed index information"). Concretely, inside a `SUBM`/`PRIM` block the index stream
begins with plausible small indices (`3,0,1,18,6,7,8,10,5,14,19,18,20,4,17,25,26,3,21,22,23,24,…`) —
i.e. a vertex-cache/strip order — then transitions into control-byte-delimited codes that are **not**
plain `u8`/`u16` indices (max byte 255 > nVerts, and no fixed stride reproduces the vertex block).

Two independent decode attempts (plain `u8`/`u16` index arrays; fixed-stride packed-`int16` and
`f32×3` vertex scans) **failed to produce an in-range / bounded parse**, which is itself evidence that
the streams are compressed rather than flat. Per the project loop-prevention rule, the byte-exact
decode of these streams is recorded here as the open gap:

- **What is needed:** the engine's mesh decompressor (the routine that consumes `PRIM`/`VERT`/the
  compiled index stream). That is a Ghidra target — pure byte analysis cannot recover the codebook.
  The `Info`/`SUBM` counters (`nVerts`, `nFaces`, `sVert`, `matTexId`) give the exact sizes the
  decompressor must produce, so the Ghidra work is well-scoped.
- **What is NOT blocked:** everything structural above (descriptor, LOD table, material bindings,
  submesh counters, vertex-format class, skeleton presence, chunk map) is decoded and validated.

---

## 7. Vertex-format class ↔ CRC (ties to BIG_ROUNDTRIP §7)

`BIG_ROUNDTRIP.md` §7 found the TOC `CRC` is a **per-format-class fingerprint**, and graphics.big has
1,289 distinct CRCs "because meshes have many vertex/format classes". This decode confirms the
mechanism: the CRC groups correlate with the `sVert` code set of the entry's submeshes —

| CRC | n | dominant sVert set | reading |
|-----|--:|--------------------|---------|
| `0xae689191` | 1,247 | `{4}` (931) | packed-coord meshes (the big class) |
| `0xceff9b23` | 298 | `{4}` (262) | packed-coord |
| `0x73fe4814` | 140 | `{20}` (46) | float-coord meshes |
| `0xe85b047b` | 94 | `{20}` (42) | float-coord |
| `0x7b646ea0` | 72 | `{20}` (31) | float-coord |

So the CRC is the serialization-class stamp and `sVert` is the concrete per-submesh vertex layout
within it — consistent with the "resource-class version CRC" interpretation.

---

## 8. Model-swap feasibility verdict

- **Metadata / material re-bind: GO.** The `Info` descriptor (LOD table + `matTexId`) is fully decoded
  and zero-leftover round-trippable, and `BIG_ROUNDTRIP.md` proves byte-exact `.big` re-serialization.
  You can **retarget a mesh's textures** (edit `matTexId[]` to other `textures.big` IDs) or adjust LOD
  distances today, with no CRC change (§7: same format class → same CRC). Payload-side material IDs
  (tagged-BBM `MTRL`, and the compiled `GMAT` block) would also need patching for full effect.
- **Vertex/geometry swap: NO-GO until the BBB compressor/decompressor is reversed.** Replacing actual
  geometry requires WRITING the compressed vertex + index streams (§6), which needs the engine codec.
  The community path historically injected geometry via **Fable Explorer / a `.x`↔`.x` model injector**
  (which embeds BBB's compressor), not by hand-writing bytes — consistent with this finding.
- **Swapping a whole mesh entry for another existing one: GO** (it's just a payload-blob copy + the
  descriptor; both are reproduced byte-exact by the round-trip harness). Cross-mesh compatibility
  depends on matching skeleton/material expectations of the consuming object.

---

## 9. Reproduce

```
python tools/parse_mesh.py "<install>\data\graphics\graphics.big"                 # classify + validate all
python tools/parse_mesh.py "<install>\data\graphics\graphics.big" --entry MESH_SEAGULL_01
python tools/parse_mesh.py "<install>\data\graphics\graphics.big" \
       --xref "<install>\data\graphics\pc\textures.big"                            # 100% material xref
```
Validated numbers (retail): descriptor `3294/3295` zero-leftover; material xref `9148/9148` (100%);
`SUBM` `nFVI==3*nFaces` `6449/6449`; `sVert ∈ {4,6,20,22}`. Evidence OBJ:
`scratchpad/mesh_agent/seagull_origin_pts.obj`.
