# graphics.big MBANK — mesh GEOMETRY decode (vertex buffers + index streams)

> **★ CORRECTION (2026-07-19, EgoCore + empirical): the geometry IS LZO-compressed on
> disk — the "PLAIN / UNCOMPRESSED" verdict in §0 below is WRONG for the on-disk format.**
> Proven by porting EgoCore's `C3DMeshContent::Parse` (`EgoCore/Meshes/MeshParser.h`) and
> decoding real `graphics.big` entries: each primitive's **vertex buffer and index buffer**
> is a Fable **chunked-LZO** block (`Utils.h DecompressLZO`: a run of `[u16 clen][clen LZO
> bytes]` — `0xFFFF`→u32 escape, `clen==0`→stored chunk — filling `size-3`, then 3 trailing
> plain bytes), inflated with our LZO1X (`tools/lionhead_lz.py`). The bone-name/matrix,
> helper, dummy, packed-name and plane blocks are LZO too; only the header/stats are plain.
> **Validation** (`scratchpad/ego_mesh_validate.py`): `MESH_SEAGULL_01` (2 prims, stride 20)
> and `MESH_OBJECT_STATUE_BEAR` (2983 verts, stride 12) both inflate to the exact expected
> vertex-buffer size, all indices in range, and the computed bbox matches the entry's STORED
> bbox to **0.000**.
>
> **PROOF the on-disk blocks are genuinely LZO-compressed (not stored):** per-block frame
> ratios on the installed `graphics.big` — seagull prim0 vertex buffer `out=5080, clen=2940`
> (58%), bear prim0 `out=35796, clen=28416` (79%); only sub-20-byte blocks are stored
> (`clen=0`). LZO1X-decompressing exactly those ranges yields the bbox-matching geometry.
> So the "5 pattern-scans failed to find a plain vertex block" (§4) is explained: there is
> no plain block on disk — it's LZO. **Port target for the Blender addon `decode_geometry()`
> = EgoCore's `Parse`, not the LoadBinary walk.**
>
> **OPEN — engine inflate mechanism (contested, does NOT affect the on-disk fact):** two
> static passes disagree on *which* engine function inflates these blocks on load.
> `C3DMesh2::LoadBinary` (`0x00a8ad40`) reads *plain* (it runs on an already-inflated
> `CMemoryDataInputStream`). A later Ghidra xref pass (`docs/MESH_BANK_INFLATE.md`) traced
> the bank layer (`BuildMesh`→`CBankFileAsyncData::GetData`→`ReadEntryData`) as plain
> `Seek/Read` and found `lzo1x_decompress` @ `0x00c06b90` with only a save-side direct
> caller — concluding "raw." That conclusion is **refuted** by the compression ratios above
> (the bytes ARE LZO) and by the texture-LZO finding through the same address, so the xref
> (a newly-written `XrefTree.java`) **missed** the real caller — most likely a chunked-LZO
> *wrapper* (mirroring EgoCore `Utils.h DecompressLZO`) that frames `[u16 clen]` blocks and
> calls `lzo1x_decompress` internally, reached via indirect/vtable dispatch. Settle with a
> live x32dbg breakpoint on the bank read during a mesh load. **The on-disk LZO format is
> not in doubt; only the exact engine call site is.**

*Authored 2026-07-19 (geometry-RE agent). Method: **decompile of the Fable.exe mesh loader**
(project FableTLC, program `/Fable.exe`, ImageBase `0x400000`). This settles the compressed-vs-plain
question that `docs/BIG_MESH_FORMAT.md` §6 left open as "Big Blue Box proprietary control-byte
compression". Decompiled reference: `ghidra_out/decomp_mesh_geo*.c` / `*.log`. Decoder facts:
`tools/decode_mesh_geo.py`.*

## 0. TL;DR — VERDICT: geometry is **PLAIN / UNCOMPRESSED**

There is **no decompressor on the mesh load path** — not LZO, not zlib, and **not** any bespoke
control-byte codec. The earlier "Big Blue Box proprietary control-byte compression" claim
(`BIG_MESH_FORMAT.md` §6) is **refuted by the loader**: it was inferred from two failed black-box
byte parses, and the "control bytes" seen were actually (a) the `WriteCBYTE`/`ReadCBYTE` material &
submesh **index list** in `C3DMeshFilePrimitiveChunk`, (b) **quantized** (packed) vertex field bytes,
and (c) the 32-byte `CVertexCompressionParams` scale/bias block — none of which is a compression codec.

A compiled MBANK mesh entry is a **relocatable serialized memory image** of a `C3DMesh2` object graph.
Loading = deserialize the flat image field-by-field through `CMemoryDataInputStream` (every read is a
plain `Read()` / `memcpy`) and rebase the embedded pointers (`RelocateData` family). The vertex and
index arrays are copied **byte-for-byte** straight into locked D3D vertex/index buffers.

## 1. The load path (retail addresses — cite these)

| Function | Retail addr | Role |
|----------|-------------|------|
| `C3DMeshLODInfo::OnResourceLoaded` | `0x00a25470` | fires `BuildMesh` when the bank entry finishes loading |
| `C3DMeshLODInfo::BuildMesh` | `0x00a24520` | wraps the raw entry bytes in a `CMemoryDataInputStream`, calls `C3DMesh2::LoadBinary`, then uploads each primitive to D3D |
| `C3DMesh2::LoadBinary` | `0x00a8ad40` | **top-level deserializer**: counts, bone-name table, bone matrices, `C3DMeshMaterial[]`, then `C3DPrimitive2::LoadBinary` per primitive. All plain reads. |
| `C3DPrimitive2::LoadBinary` | `0x00a92900` | **per-primitive geometry**: reads `CStaticBlock`/`CAnimatedBlock` records, the `CVertexCompressionParams`, the **vertex buffer**, then the **index buffer** |
| `C3DPrimitive2::LoadVertexBuffer` | `0x00a91560` | `Read(nVerts*stride)` → vertex buffer (**raw memcpy**); copies the 8-dword `CVertexCompressionParams` into the VB object at `+0x14` |
| `C3DPrimitive2::CalculateVertexSize` | `0x00a910e0` | computes the **stride** from the FVF flags via `CVertexShaderInput{Static,Animated}::Initialise` |
| `CVertexShaderInputStatic::Initialise` | `0x00a90560` | static vertex-declaration element list |
| `CVertexShaderInputAnimated::Initialise` | `0x00a90630` | skinned vertex-declaration element list (adds bones) |
| `CVertexShaderInput::GenerateD3DDeclaration` | `0x00ab3870` | **type→byte-size table** (the stride accumulator) |
| `CIndexBuffer::Initialise` | `0x00ac1f70` | u16 index buffer, `EPrimitiveType=4` |
| `C3DMesh2::RelocateData` | `0x00a8b4c0` | pointer fixup (`ptr - oldBase + newBase`) — proves relocatable-image model |
| `CIndexBuffer::RelocateData` | `0x00ac1f50` | (no-op) |

In `BuildMesh`, the entry `Type & 0xf` selects the D3D upload variant:
`C3DPrimitive2::UploadRepeatedBuffers` (types 1/2/4) or `UploadAnimatedBuffers` (type 5, skinned).
**No branch anywhere calls a decompressor** — the only compressors in the binary
(`lzo1x_decompress` @ `0x00c06b90` for textures, `CCompressorZlib` @ `0x00a3abe0` for definition data)
are never referenced by the mesh path.

### Tagged-BBM (Type-3, `3DMF`) path is plain too — verified independently

| Function | Retail addr | Behaviour |
|----------|-------------|-----------|
| `C3DMeshFileXVertexDataChunk::ReadFromFile` | `0x00aa66b0` | reads 4 fields, `resize(stride*count)`, **plain block copy** of the vertex bytes |
| `C3DMeshFileXTriangleStripChunk::ReadFromFile` | `0x00aa6960` | reads a count, **plain copy** of `count` × u16 indices |
| `C3DMeshFilePrimitiveChunk::ReadFromFile` | `0x00abcf40` | reads a count then a plain `u32` material-index array |
| `C3DMeshFileSubMeshChunk::ReadFromFile` | `0x00ab9fc0` | name + 4 dwords (index/parent/child/sibling) then super-chunk |

So both mesh families store geometry uncompressed; tagged-BBM adds FourCC chunk framing, the compiled
image has none.

## 2. Vertex layout — exact per-class stride (PROVEN)

`GenerateD3DDeclaration` (`0x00ab3870`) walks the declaration element list and sums a **type→size**
table (the `switch` in the decompile):

| component type code | byte size | meaning |
|--------------------:|----------:|---------|
| 0,4,5,7,9,0xc,0xd | **4** | packed dword (D3DCOLOR / UBYTE4 / packed normal / s11e4 UV pair / bone idx+weight) |
| 1,6,0xb | **8** | FLOAT2 / SHORT4 / packed pair |
| 2 | **12** | FLOAT3 — **float position** |
| 3 | **16** | FLOAT4 |
| 8 | **2** | short |
| 10 (0xa) | **6** | SHORT3 — **packed s16×3 position** |

The declaration per FVF-flag class (`flags = primitive dword & ~4`), from the two `Initialise`
routines. The stride below is `sum(type sizes)` — and it **reproduces the wiki `sVert` stride set
{12,20,28,36}**, an independent corroboration:

| path | flags | declaration (type codes) | **stride** | wiki sVert match |
|------|------:|--------------------------|-----------:|------------------|
| static, float-coord | `0x04` | `[0xd,0xc,7]` | **12** | (packed-coord class, stride 12) |
| static, float-coord | `0x06` | `[0xd,0xc,7,0xb]` | **20** | sVert 20, stride 20 |
| static, float-coord | `0x14` | `[2,0xc,7]` | **20** | sVert 20 (FLOAT3 pos), stride 20 |
| static, float-coord | `0x16` | `[2,0xc,7,0xb]` | **28** | sVert 20 + bones, stride 28 |
| animated (skinned) | `0x04` | (adds bones) | **20** | sVert 4 + bones, stride 20 |
| animated (skinned) | `0x16` | | **36** | sVert 22, stride 36 |

- **Position** is always the **first** declaration element: `type 2` = raw `FLOAT3` (12 B) for the
  float-coord classes, or `type 10` = **packed `s16×3`** (6 B) for the packed-coord classes.
- **Packed positions are dequantized in the vertex SHADER at draw time**, not at load: the loader
  copies the 8-dword `CVertexCompressionParams` (4× float **scale** + 4× **bias**) verbatim into the
  vertex-buffer object (`LoadVertexBuffer`, VB+`0x14`). Default scale = `1.0,1.0,1.0,1.0`
  (`SetupVertexBuffer` @ `0x00a8eda0`). This is why the vertex bytes are *quantized* but the buffer is
  *uncompressed*.
- Packed normal = one dword (type 4/0xc/0xd). s11e4 UVs = one dword each. These match the wiki.

## 3. Index encoding — PROVEN

Indices are **plain unsigned 16-bit** (`u16`), read with a single `memcpy` in
`C3DPrimitive2::LoadBinary` (`Read(nIndices*2)`), then handed to `CIndexBuffer::Initialise` with
`EPrimitiveType = 4`. There is no delta/strip *codec*; the D3D index buffer is a straight u16 array.
(Runtime count = `primitiveField[0x60] * repeatMultiplier`; the "Repeated" upload replicates the base
block with a per-repeat vertex-offset — relevant only for instanced/repeated primitives.)

## 4. Validation status & the remaining GAP (honest)

**What is proven (loader-level):** the codec verdict (plain), the exact stride table, the u16 index
encoding, the dequant mechanism. Cited to specific retail addresses and reproduced in
`tools/decode_mesh_geo.py`.

**What was NOT completed: a coherent decoded `.obj`.** To extract a vertex block from an arbitrary
compiled entry with a pure-Python reader you must **replicate the full deterministic `LoadBinary`
walk** end-to-end, because the compiled image has **no chunk framing** — it is a raw relocatable
`struct` dump, so the vertex/index blocks are **not locatable by byte pattern-scan**. Five distinct
scans were tried and all correctly failed (documented, per loop-prevention):
  1. plain u16 index array (== `3×nFaces`) covering all verts — none found;
  2. `nVerts×20` FLOAT3-first vertex block immediately followed by a u16 index block — none;
  3. index-first search over strides {12,16,20,24,28,32,36,48} × {FLOAT3, s16×3} — none;
  4. contiguous FLOAT-run map (≥20 dwords) — **zero** long float runs (positions are interleaved
     into the stride, and/or packed s16×3, so they are never 20 consecutive float dwords);
  5. strided-float run detector — only short degenerate 16-vertex runs.

The uniform failure of pattern-scanning is itself consistent with the finding: there is nothing to
"find" as a framed blob — the geometry sits at a byte offset that only the deterministic deserializer
computes. The `SUBM` "header" the byte-parser in `parse_mesh.py` keys on (`nVerts,nFaces,nFVI,sVert`
with `nFVI==3×nFaces`) is a **stats/LOD metadata** record, **not** the primitive's vertex/index block
(verified: on `MESH_SEAGULL_01` the bytes right after that "header" are bbox floats, not an index run).

**Scoped remaining task (serialization replication, NOT codec-reversing):** port the walk
`C3DMeshStats::LoadBinary (0x00a2f2d0)` → mesh-volumes (`C3DMeshVolume::LoadBinary`, 0x14 B each) →
generator objects (`C3DGeneratorObject::LoadBinary`, 0x40 B each) → bone-name offset table (u16×nBones)
→ bone-name strings → bone matrices (0x30 B ×nBones) + 0x30-B matrix → `C3DMeshMaterial::LoadBinary`
×nMat → per-primitive `C3DPrimitive2::LoadBinary`: `CStaticBlock::LoadBinary` (0x00a8ea70, 0x20 B each)
/ `CAnimatedBlock::LoadBinary` (0x30 B each) → `CVertexCompressionParams` (0x20 B) → **vertex buffer**
(`nVerts×stride`) → **index buffer** (`nIdx×2`). Once positioned, §2/§3 above are the complete decode
spec and the shape falls out immediately (no further RE). This is a finite mechanical port; the hard
RE unknown (is it compressed, and how) is **closed**.

## 5. Impact on the Blender importer

- **Geometry authoring is NOT blocked on a codec** — there is none. A read/export importer needs only
  the deterministic deserializer port in §4 plus the stride/index spec in §2/§3.
- **Writing geometry** is symmetric: emit the same relocatable image (the `WriteToFile`/`WriteBinary`
  side, e.g. `C3DMeshFileX*Chunk::WriteToFile`, mirrors the readers). No compressor to reproduce — this
  contradicts `BIG_MESH_FORMAT.md` §8's "NO-GO until the BBB compressor is reversed" verdict: there is
  no BBB compressor for meshes; the blocker is only the (now-specified) serialization walk.

## 6. Reproduce

```
# Ground-truth decompiles (read-only Ghidra headless; DecompByName / DecompFuncs):
#   ghidra_out/decomp_mesh_geo.c   (chunk readers: VertexData/TriangleStrip/SubMesh/Primitive)
#   ghidra_out/decomp_mesh_geo2.c  (RelocateData family + C3DMeshInfo::Sizeof)
#   ghidra_out/decomp_mesh_geo3.c/.log (BuildMesh 0xa24520/0xa25d20 + buffer lock/create)
#   ghidra_out/decomp_mesh_geo4.c  (C3DMesh2::LoadBinary + C3DPrimitive2::LoadBinary + Upload*)
#   ghidra_out/decomp_mesh_geo5.c  (CalculateVertexSize + LoadVertexBuffer + SetupVertexBuffer)
#   ghidra_out/decomp_mesh_geo6.c  (C3DMeshStats::LoadBinary)
#   ghidra_out/decomp_mesh_geo8.c  (CVertexShaderInput{Static,Animated}::Initialise + CStaticBlock::LoadBinary)
#   ghidra_out/decomp_mesh_geo9.c  (vertex-declaration element append)
#   ghidra_out/decomp_mesh_geo10.log (GenerateD3DDeclaration type->size table)
python tools/decode_mesh_geo.py    # prints the stride table + decode facts
```

## 7. Bone bind-pose blocks — CONFIRMED layouts (2026-07-20, ported from EgoCore)

Source of truth: EgoCore (MIT), `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\EgoCore\Meshes\`
— `MeshParser.h` `C3DMeshContent::Parse` (read side, ~line 671), `MeshProperties.h`
`UpdateAnimationBones` (bind-pose math), `GltfExporter.h` (skin/IBM export, node locals ~line 1357),
`GltfMeshImporter.h` (write side ~line 1885, independently confirms each field). Empirically
validated against retail graphics.big by the Blender addon headless test (bone positions land
inside the mesh bbox — see docs/BLENDER_ADDON.md).

Position in the compiled-mesh payload: after the stats counts
`MaterialCount:i32, PrimitiveCount:i32, BoneCount:i32, BoneNameSize:i32, ClothFlag:u8,
TotalStaticBlocks:u16, TotalAnimatedBlocks:u16`, iff `BoneCount > 0`, and before the 48-byte
`RootMatrix`:

| # | framing | size (bytes)    | content |
|---|---------|-----------------|---------|
| 1 | RAW     | `2*BoneCount`   | `u16 BoneIndices[]` — local slot → **GLOBAL Fable bone ID**. NOT parent indices. Vertex joint bytes, helper/dummy/generator `BoneIndex` fields and anim tracks use these global IDs; remap global→local before use. |
| 2 | chunked-LZO | `BoneNameSize` | `BoneCount` NUL-terminated bone-name strings, in local-slot order (`Scene Root`, `Bip01`, ...). |
| 3 | chunked-LZO | `60*BoneCount` | `C3DBone { u32 NameCRC; i32 ParentIndex; i32 OriginalNoChildren; float LocalizationMatrix[12] }` — **ParentIndex is the LOCAL-slot parent, −1 = root** (this is the hierarchy). NameCRC = bytewise CRC-32 (poly 0xEDB88320, init 0). LocalizationMatrix = 3×3 at [0..8] (= transpose of the IBM linear part, i.e. the bind-global rotation rows) + IBM translation at [9..11]. |
| 4 | chunked-LZO | `48*BoneCount` | per-bone TRS record ("BoneKeyframesRaw"): `float quat[4]` (x,y,z,w) at [0..3], `float trans[3]` at [4..6], pad, `float scale[3]` at [8..10], pad. Bind pose ≈ identity quat + (−IBM translation). Not needed to build the skeleton. |
| 5 | chunked-LZO | `64*BoneCount` | **4×4 float INVERSE BIND MATRIX** per bone, row-major storage of a COLUMN-VECTOR-convention matrix: rows = floats [0..3],[4..7],[8..11], translation at elements 3,7,11. The 4th row ([12..15]) is NOT a valid matrix row — every consumer replaces it with (0,0,0,1) before use. |

Bind math (EgoCore `MeshProperties.h` / `GltfExporter.h`):
```
IBM_i  = block5[i]  (4th row forced to 0,0,0,1)
W_i    = inverse(IBM_i)              # world/model-space bind matrix; W_i[.,3] = bone head
L_i    = inverse(W_parent) @ W_i     # node-local bind (glTF node matrix), parent from block3
```
All LZO blocks use the same chunked framing as vertex/index buffers (`[u16 clen][payload]`,
0xFFFF→u32 escape, clen==0→stored, final 3 bytes plain — `fable_core._lzo_fable`).

### 7.1 Per-vertex SKIN weights (animated vertex layout) — CONFIRMED (2026-07-20, EgoCore)

Source of truth: EgoCore `GltfExporter.h::Export` vertex loop (~lines 331-416, the
`hasBones` branch) writing glTF `JOINTS_0`/`WEIGHTS_0`, and `MeshParser.h` `CAnimatedBlock`
(line 67-69) + its parse (~line 717-721). Confirmed empirically against retail graphics.big
by the Blender addon skin test (`tests/blender_skin_test.py`): every skinned vertex gets
≥1 group, per-vertex weights sum ≈ 1.0, groups are real bone names.

**A primitive is SKINNED iff `AnimatedBlockCount (abc) > 0`.** Skinned primitives add
**8 bytes** to each vertex — **4 joint bytes + 4 weight bytes** — inserted **immediately
after the position field and before the normal**. With `isPosComp = (InitFlags&4)&&!(InitFlags&0x10)`:

| field | offset | size | encoding |
|-------|-------:|-----:|----------|
| position | 0 | 4 (POSPACKED3) or 12 (FLOAT3) | as §2 |
| **joint indices** | `iOff = isPosComp?4:12` | **4 × u8** | see joint decode below |
| **weights** | `wOff = iOff + 4` | **4 × u8** | `w[k]/255`, then renormalize |
| normal | `normOff = iOff + 8` | 4 or 12 | as §2 |
| UV | `uOff = normOff + (isNormComp?4:12)` | 4 or 8 | as §2 |

**Weight decode:** `w[k] = wgt[k] / 255.0` for k in 0..3; `sum = Σw`; if `sum > 0.001`
divide each by sum (renormalize), else force `w=[1,0,0,0]`. Zero-weight influences are
dropped. 4 influences per vertex maximum.

**Joint decode (the subtle part — indices are NOT direct bone slots):** each joint byte
`ind[k]` is a *triangle-stride palette index* — divide by 3 to get the palette entry
`pID = ind[k] / 3`. That entry indexes the **per-animated-block palette** `Groups[]`
(a `u8[groupCount]` captured with each `CAnimatedBlock`): `localBone = AnimatedBlocks[blk].Groups[pID]`.
`Groups[pID]` yields the **LOCAL bone slot** directly (0..BoneCount-1) — NOT a global Fable ID.
So skin joints resolve to local slots via the block palette; no `BoneIndices[]` global→local
remap is needed for weights (that remap is only for helper/dummy/generator `BoneIndex` and
anim tracks). Clamp `localBone` to `< BoneCount` (fallback 0).

**Which animated block a vertex belongs to:** vertices are grouped by animated block in file
order; block `blk` owns the next `AnimatedBlocks[blk].VertexCount` vertices. Walk a running
counter: when `proc >= AnimatedBlocks[blk].VertexCount`, advance `blk` and reset `proc`.
Each block has its own `Groups[]` palette. `CAnimatedBlock` layout (per block, plain, in the
primitive header before the compression params — see `_parse_mesh_primitives`):
`u32 PrimitiveCount, u32 StartIndex, u8 IsStrip, u8 ChangeFlags, u8 Degenerate,
u32 VertexCount, u16 BonesPerVertex, u8 PalettedFlag, u8 groupCount, u8 Groups[groupCount]`.

**Blender mapping:** create one vertex group per bone named by its decoded bone name; assign
each vertex its (localBone → weight) pairs; add an `ARMATURE` modifier on the mesh object
pointing at the imported armature. Because JOINTS resolve to local slots and the armature's
edit-bones are created in local-slot order, `vertex_groups[boneName]` with `boneName =
model.bones[localBone].name` binds correctly.

**Export (write side):** symmetric — for each vertex take its top-4 Blender vertex-group
influences, map group name → bone → its position in that primitive's `Groups[]` palette
(append to the palette if absent), write `ind[k] = paletteIndex*3` and `wgt[k] =
round(weight*255)` with u8 renormalization, at `iOff`/`wOff`. See `mesh_rw.encode_skin`.
