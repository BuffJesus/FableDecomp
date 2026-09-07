# graphics.big MBANK compiled meshes — descriptor, geometry, skeleton, composer

*Merged reference (2026-09-07) of `BIG_MESH_FORMAT.md` (2026-07-19), `BIG_MESH_GEOMETRY.md`
(2026-07-19/20), `MESH_BANK_INFLATE.md` (2026-07-19) and `MESH_COMPOSE.md` (2026-07-20, EgoCore
section 2026-07-31). Container layout / TOC / writer live in [BIG.md](BIG.md); skeletal animations
(types 6/7/9) in [BIG_ANIM_FORMAT.md](BIG_ANIM_FORMAT.md); the LZO1X codec in
[LIONHEAD_LZ.md](LIONHEAD_LZ.md); equipment usage of composed meshes in
`docs/engine/HERO_APPEARANCE.md#equipment-and-clothing-system` §7.*

**Status:** compiled-mesh payload grammar fully decoded (header, materials, primitives, vertex
layouts, u16 indices, skeleton bind blocks, skin weights) and **round-trips**; brand-new static
(type-1) and skinned (type-5) meshes can be **composed** from arbitrary geometry and appended to a
copy of `graphics.big` (`mesh_rw.compose_mesh` + `big_write.rebuild(adds=)`); Blender import/export
proven headless. In-engine load of a composed mesh has been exercised via the custom-NPC /
equipment pipelines (see HANDOFF), not in the original compose task.

Tools: `tools/parse_mesh.py` (TOC-descriptor classifier, pure Python, read-only on
`tools/parse_bigb.py`), `tools/decode_mesh_geo.py` (stride/decode facts),
`tools/blender_addon/io_scene_fable/{fable_core,mesh_rw}.py` (`_parse_mesh_primitives`,
`_prim_geometry`, `compose_mesh`, `decode_to_compose_args`, `clone_skeleton`, `encode_skin`),
`tools/big_write.py`, tests `tools/blender_addon/tests/{compose_test,blender_compose_test,
blender_skin_test,blender_bones_test,blender_headless_test}.py`.

Sources: raw retail bytes of `graphics.big` + `textures.big`; Ghidra decompile of `Fable.exe`
(ImageBase `0x400000`; `ghidra_out/decomp_mesh_geo*.c`, `decomp_mesh_bank_inflate*.c`,
`decomp_bankfile.c`, `decomp_asyncdata.c`, `decomp_setupbuffer.c`, `decomp_lzo_named.c`); EgoCore
(MIT, `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\EgoCore\Meshes\` —
`MeshParser.h C3DMeshContent::Parse` ~line 671, `MeshCompiler.h CompileSingleLOD/CompileForExport`,
`GltfMeshImporter.h ImportType1/ImportType5` ~line 1885, `GltfExporter.h` ~lines 331-416/1357,
`MeshProperties.h UpdateAnimationBones`, `Banks/BankEditor.h FlushStagedEntries`,
`Utilities/Utils.h`); fabletlcmod.com wiki `file_formats:bbm` (fetched via text proxy, TLS cert
fails); the `fable_format` Rust crate (docs.rs). Fields whose *purpose* is inferred are
**[hypothesis]**.

---

## 1. Bank layout — entry `Type` → payload family

`graphics.big` = `MBANK_ALLMESHES` (8,112 entries) + `MBANK_ENGINE` (1). Every entry's Dep is a
`*.bbm` build path (Big Blue Box "BBM").

| Type | count | InfoSize | family | payload leading bytes | note |
|-----:|------:|----------|--------|-----------------------|------|
| 1 | 2,584 | 60–176 (var) | compiled-mesh | `MESH_…\0` (36 are `BANDIT_`/`TABL`/`GAME`…) | general props/creatures — **static** |
| 2 | 55 | 64–72 | compiled-mesh | `MESH_…\0` | grass/bramble (alpha foliage) — **static-repeated / instanced** |
| 4 | 152 | 64–92 | compiled-mesh | `MESH_…\0` | fx / translucent — **particle** |
| 5 | 504 | 64–124 | compiled-mesh | `MESH_…\0` | **skinned** / complex |
| 3 | 1,383 | 0 | tagged-bbm | `…\0\0(3DMF…` | collision/physics `[PHYSICS]` (CMESH/BBM, separate `3DMF` writer, wired via Info `PhysicsIndex`) |
| 6 | 3,272 | 24 | anim | `f32 count…` | `ANIM_*` |
| 7 | 56 | 24 | anim | `f32 …` | `ANIM_*` |
| 9 | 107 | 24 | anim | `f32 …` | `ANIM_*` |

- Types 1/2/4/5 = **compiled meshes** (3,295 entries): a name-prefixed relocatable "compiled model"
  (skeleton + LODs + primitives + vertex/index buffers). Types 6/7/9 (3,435) are animations, not
  meshes (anim `Info` 24 B = two f32 playback params e.g. `3.23, 3.20` + 4 zero dwords; payload
  starts with a keyframe count). Type-3 is the `3DMF` FourCC chunk container (§7).
- Full mesh-type map (EgoCore `Utilities/Utils.h`, 2026-07-31): **1 = static, 2 = static-repeated /
  instanced, 3 = physics, 4 = particle, 5 = skinned.** `Type` is not a vertex-format axis (all four
  mesh types carry all `sVert` codes); it is a render-bucket / material class. In `BuildMesh` the
  `Type & 0xf` selects the D3D upload variant: `C3DPrimitive2::UploadRepeatedBuffers` (types 1/2/4)
  or `UploadAnimatedBuffers` (type 5).
- Type-2 instancing = type-1 geometry + `RepeatingMeshReps` with a **third static vertex layout
  (stride 36, uncompressed pos)** and index/vertex buffers tiled ×reps (cap 65535). Types 2/4 compile
  via the same `CompileSingleLOD` grammar as type 1 (EgoCore-sourced; to-verify against retail).

---

## 2. TOC `Info` descriptor (compiled meshes) — VALIDATED

EgoCore `SerializeEntryMetadata`, byte-validated against retail (`InfoSize = 60 + 4*texCount`):

| Field | Type | Meaning |
|---|---|---|
| `PhysicsIndex` | i32 | 0 = none; retail uses the entry id of a type-3 physics BBM when present |
| `BSCenter` | 3×f32 | bounding-sphere centre (same as payload header) |
| `BSRadius` | f32 | |
| `BBMin`, `BBMax` | 3×f32 + 3×f32 | AABB |
| `LODCount` | u32 | 1..8 (retail: almost always 3; composed meshes 1) |
| `LODSizes[LODCount]` | u32[] | **LOD block BYTE SIZES** (ghost LOD excluded) |
| `SafeBoundingRadius` | f32 | retail 0 |
| `LODErrors[]` | f32[] | LOD switch-distance floats, present only when `LODCount > 1` (observed 1–2 values e.g. 0.025, 0.040, 0.065) |
| `TextureIDsCount` | u32 | |
| `TextureIDs[]` | i32[] | unique non-zero map IDs across materials = **textures.big entry IDs** (what `parse_mesh.parse_mesh_descriptor` recovers as `matTexId` and `build_model` binds textures from) |

History of the decode (2026-07-19 byte-only pass, `parse_mesh.py`): the same bytes were read as
`u32 flags(0)`, `f32 origin[10]` (an opaque 10-float bound block — the exact split "not a clean
min3/max3"; it is BSCenter(3)+BSRadius(1)+BBMin(3)+BBMax(3)), `u32 nLOD`, `u32 lod[nLOD]`,
`u32 pad0`, `f32 lodDist[k]`, `u32 nMat`, `u32 matTexId[nMat]`. The parser recovers `nMat` from the
tail (the count whose position leaves exactly `nMat` trailing dwords) so the parse is zero-leftover
by construction, and it checked `payload[lod[0] : lod[0]+len(name)] == entry name`. That check
passes because `lod[0]` = LOD0 byte size = the offset of the ghost LOD, whose header starts with the
repeated mesh name (§3.4) — consistent with the corrected reading. **Result:** `3,294 / 3,295`
types-1/2/4/5 parse zero-leftover; the single miss is `MESH_ENGINE_UNIT_BOX` (`InfoSize=60`, the
minimum, degenerate engine unit box). The bound-block evidence dump is
`scratchpad/mesh_agent/seagull_origin_pts.obj` (`MESH_SEAGULL_01`).

**Material binding — VALIDATED against textures.big (read-only cross-ref):** `9,148 / 9,148` (100 %)
`matTexId`s resolve to real `textures.big` entry IDs with correct names:

| mesh | matTexId | resolved textures.big name |
|------|---------|----------------------------|
| `MESH_OBJECT_STATUE_BEAR` | 237, 238 | `…BROWNBEAR_BODY_32.TGA`, `…BROWNBEAR_HEAD_32.TGA` |
| `MESH_OBJECT_STATUE_BEAR_CUB` | 239, 240 | `…BEAR_CUB_BODY_32.TGA`, `…BEAR_CUB_HEAD_32.TGA` |
| `MESH_SUMMONER_BLUE_01` | 27,696,697,699,701 | reflection + `SUMMONER_ALPHA/HEAD/LEGS/TORSO_01.TGA` |
| `MESH_JOB_MASK_01` | 3749 | `…JACKOFBLADES\JACKOFBLADES_01.TGA` |

The wiki lists `BASE_Texture_ID`, `BUMPMAP_Texture_ID`, `REFLECT_Texture_ID` per material; the Info
array holds the unique non-zero IDs of all map slots (diffuse/bump/reflect/illum) across materials,
while per-material bindings live in the payload material records (§3.2).

---

## 3. Compiled-mesh payload layout

A compiled payload = `LOD0 block || LOD1.. || ghost LOD`. Little-endian. It is a **relocatable
serialized memory image** of a `C3DMesh2` object graph: loading deserializes field-by-field through
`CMemoryDataInputStream` and rebases embedded pointers (`C3DMesh2::RelocateData` @ `0x00a8b4c0`,
`ptr - oldBase + newBase`; `CIndexBuffer::RelocateData` @ `0x00ac1f50` is a no-op). There is **no
chunk framing** — vertex/index blocks are located only by walking the deterministic deserializer
(pattern scans fail by design, §5.4).

### 3.1 LOD block header (EgoCore `CompileSingleLOD`; values = what `compose_mesh` emits)

| field | type | value we emit / retail |
|---|---|---|
| MeshName | cstr | entry name, must start `MESH_` (payload classifier) |
| AnimatedFlag | u8 | 0 static / **1 skinned** (verified: seagull=1, statue=0) |
| BoundingSphereCenter / Radius | 3f + f | computed from all verts (bbox center, max distance) |
| BoundingBoxMin / Max | 3f + 3f | computed from all verts |
| HelperPointCount, DummyObjectCount, PackedNamesSize, MeshVolumeCount, MeshGeneratorCount | 5×u16 | all 0 when composing (no optional blocks follow); retail carries named helper points / dummies (`focus_eye_01`, `movement_dummy`), volumes (`C3DMeshVolume::LoadBinary`, 0x14 B each), generators (`C3DGeneratorObject::LoadBinary`, 0x40 B each) |
| MaterialCount | i32 | `len(materials) + 1` on STATIC (sentinel, §3.2); `len(materials)` on skinned |
| PrimitiveCount | i32 | `len(prims)` |
| BoneCount, BoneNameSize | 2×i32 | 0 static; real for skinned |
| ClothFlag | u8 | 0 |
| TotalStaticBlocks, TotalAnimatedBlocks | 2×u16 | static: `len(prims)`, 0 (one static block per primitive); skinned: 0, Σ animated blocks |
| *(bone blocks, iff BoneCount>0)* | §8 | between `TotalAnimatedBlocks` and `RootMatrix` |
| RootMatrix | 12f | identity 3×3 + zero translation |
| materials | §3.2 | |
| primitives | §3.3 | one per prim |

The stats counts are read by `C3DMeshStats::LoadBinary` (`0x00a2f2d0`): `MaterialCount:i32,
PrimitiveCount:i32, BoneCount:i32, BoneNameSize:i32, ClothFlag:u8, TotalStaticBlocks:u16,
TotalAnimatedBlocks:u16`. The skeleton is a 3ds-Max Biped bone-name hierarchy (`Scene Root`,
`Bip01`, ` Pelvis`, `Neck`, `Head`, `L Clavicle`, `L UpperArm`, `L Thigh`, `…Calf`, `…Toe0`,
`Tail`) embedded in the payload, not in the Info descriptor.

### 3.2 Material record (`C3DMeshMaterial::LoadBinary` ×MaterialCount)

`ID:i32` (slot index), `Name:cstr`, `DecalID, DiffuseMapID, BumpMapID, ReflectionMapID,
IlluminationMapID: 5×i32` (textures.big entry IDs, 0 = none), `MapFlags:i32` (bit0 diffuse, bit1
bump, bit2 reflect, bit3 illum — derived from the non-zero IDs), `SelfIllumination:i32`,
`IsTwoSided, IsTransparent, BooleanAlpha, DegenerateTriangles, UseFilenames: 5×u8`
(UseFilenames=0 → no filename strings follow).

**`DegenerateTriangles` sentinel — STATIC meshes only:** every retail static mesh checked
(`MESH_OBJECT_PLATE`, `MESH_OLD_VILLAGE_SIGN`, `MESH_OBJECT_RAILING_MID`, bear) ends its material
list with a material named `DegenerateTriangles` (ID 0, all map IDs 0, MapFlags 0, bools
`0,0,0,1`). Retail SKINNED meshes carry none. `compose_mesh` appends it for static,
`decode_to_compose_args` strips it (conditioned on presence).

### 3.3 Primitive record (`C3DPrimitive2::LoadBinary` @ `0x00a92900` ×PrimitiveCount)

`MaterialIndex:i32`, `RepeatingMeshReps:i32` (=0; >0 = type-2 instancing), `SphereCenter:3f +
SphereRadius:f`, `AvgTextureStretch:f` (0.1, EgoCore default), `StaticBlockCount:u32`,
`AnimatedBlockCount:u32`, `VertexCount, TriangleCount, IndexCount(=3*tris), InitFlags: 4×u32`,
`StaticBlockCount_2, AnimatedBlockCount_2: 2×u32` (repeat), then:

- per **static block** (`CStaticBlock::LoadBinary` @ `0x00a8ea70`, 0x20 B each):
  `{PrimitiveCount(=tris):u32, StartIndex:u32(=0), IsStrip:u8 (0 = triangle LIST, 1 = strip;
  retail mixes both per block), ChangeFlags:u8, DegenerateTriangles:u8, MaterialIndex:i32}`;
- per **animated block** (`CAnimatedBlock::LoadBinary`, 0x30 B each, plain, before the compression
  params): `{PrimitiveCount:u32, StartIndex:u32, IsStrip:u8, ChangeFlags:u8, Degenerate:u8,
  VertexCount:u32, BonesPerVertex:u16, PalettedFlag:u8, groupCount:u8, Groups[groupCount]:u8}`
  (`BonesPerVertex=3`, `PalettedFlag=1` on every retail skinned prim checked; `Groups[]` is the
  block's bone palette of LOCAL slots, §9);
- `CVertexCompressionParams` (0x20 B: 4f scale + 4f offset) — copied verbatim into the vertex-buffer
  object at VB+`0x14` by `LoadVertexBuffer` (`0x00a91560`); default scale `1.0,1.0,1.0,1.0`
  (`SetupVertexBuffer` @ `0x00a8eda0`);
- `VertexStride:u32`, `BufferType:u32` (=0);
- **vertex buffer** (`nVerts × stride`, chunked-LZO framed on disk, §4);
- **u16 index buffer** (`nIdx × 2`, chunked-LZO framed);
- `ClothPrimitiveCount:u32` (=0; goblin prim0 carries a cloth primitive, dropped on recompose).

Runtime index count = `primitiveField[0x60] * repeatMultiplier`; the "Repeated" upload replicates
the base block with a per-repeat vertex offset (instanced/repeated primitives only).
`CIndexBuffer::Initialise` (`0x00ac1f70`): u16 indices, `EPrimitiveType=4`.

**Index winding:** the decoder (`fable_core._prim_geometry`, list path) emits faces as
`(ib[k], ib[k+2], ib[k+1])`, so `compose_mesh` stores each face `(a,b,c)` as indices `a,c,b` —
`decode(compose(x)) == x`.

### 3.4 Ghost LOD (retail convention)

Retail single-LOD type-1 entries all carry a trailing **"ghost" LOD**: a full mesh header (same
name/bounds) with `MaterialCount=PrimitiveCount=0` and the identity RootMatrix, **NOT counted in the
Info `LODCount`** and not in `LODSizes[]` (verified: `MESH_OBJECT_PLATE` size 1189 = `LODSizes[0]`
971 + 218-byte ghost). `compose_mesh` appends it. For skinned meshes the ghost carries
`AnimatedFlag=1` + full bone blocks; EgoCore clones the full skeleton into the ghost while retail
ships a REDUCED ghost skeleton (boot ghosts: 1 bone `Scene Root`; goblin: 26 of 67) — both parse.
- EgoCore (2026-07-31): ghost LOD applies to types **1/2/4/5**, not just 1/5 — `MeshCompiler::
  CompileForExport` (2/5-only) is stale; the authoritative bank-write path
  `Banks/BankEditor.h::FlushStagedEntries` appends one uncounted empty ghost LOD after the last real
  LOD for 1, 2, 4 and 5 as a defensive crash-guard (author comment: static meshes "didn't crash
  before... TO INVESTIGATE"). We retail-verified 1 and 5; **2 and 4 are to-verify.**

Decoded LOD counts: retail creatures ship 3–4 LOD chains + ghost; composed meshes are LOD0 + ghost
only (engine renders LOD0 at all distances).

---

## 4. Compression — on-disk LZO vs the engine load path

### 4.1 On-disk fact (settled): vertex/index buffers and bone blocks ARE chunked-LZO

Proven by porting EgoCore `C3DMeshContent::Parse` and decoding real `graphics.big` entries: each
primitive's vertex buffer and index buffer is a Fable **chunked-LZO** block (`[u16 clen][clen LZO
bytes]`, `0xFFFF`→u32 escape, `clen==0`→stored chunk, filling `size-3` then 3 trailing plain bytes —
[BIG.md §6](BIG.md#6-lzo-chunk-framing-shared-by-texture-mip-0-mesh-buffers-save-data)), inflated
with LZO1X (`tools/lionhead_lz.py`, `fable_core._lzo_fable`). The bone-name/matrix, helper, dummy,
packed-name and plane blocks are LZO too; only the header/stats are plain.
- Validation (`scratchpad/ego_mesh_validate.py`, `probe_lzo_framing.py`; ported into
  `fable_core._parse_mesh_primitives`): `MESH_SEAGULL_01` (2 prims, stride 20) and
  `MESH_OBJECT_STATUE_BEAR` (2983 verts, stride 12) inflate to the exact expected vertex-buffer size,
  all indices in range, computed bbox matches the STORED bbox to **0.000**.
- Proof of genuine compression: seagull prim0 VB `out=5080, clen=2940` (58 %), bear prim0
  `out=35796, clen=28416` (79 %); only sub-20-byte blocks are stored (`clen=0`).
- `compose_mesh(compress=True)` (default) uses `lionhead_lz_compress.compress_fable_block`;
  `compress=False` emits stored chunks — both engine-valid. An empty buffer is a lone `u16 0`.

This CORRECTS two earlier verdicts: (a) `BIG_MESH_FORMAT` §6's "Big Blue Box proprietary
control-byte compression" (inferred from two failed black-box parses — the "control bytes" were the
LZO frame plus quantized vertex bytes and the 32-byte `CVertexCompressionParams`), and (b)
`BIG_MESH_GEOMETRY` §0's "geometry is PLAIN / UNCOMPRESSED" (true of the *in-memory* image
`LoadBinary` reads, wrong for the on-disk bytes).

### 4.2 Engine load path — every traced node is a plain read (OPEN: where the inflate happens)

Read-only Ghidra decompile of the chain that fills the buffer `C3DMesh2::LoadBinary` reads:

| Step | Function | Retail addr | Behaviour |
|------|----------|-------------|-----------|
| 1 | `C3DMeshLODInfo::OnResourceLoaded` | `0x00a25470` | on load complete → `BuildMesh(true,false)` |
| 2 | `C3DMeshLODInfo::BuildMesh` | `0x00a24520` | `pvVar5 = CBankFileAsyncData::GetData(this+0x24)`; `uVar3 = *(this+0x24+0x10)` (size); `CMemoryDataInputStream(local_1c, pvVar5, uVar3)`; `C3DMesh2::LoadBinary(...)`; then uploads each primitive to D3D. Wraps the async buffer directly — no inflate call. |
| 3 | `C3DMesh2::LoadBinary` | `0x00a8ad40` | top-level deserializer: counts, bone-name table, bone matrices, `C3DMeshMaterial[]`, then `C3DPrimitive2::LoadBinary` per primitive; all plain `Read()`/`memcpy` |
| 4 | `C3DPrimitive2::LoadBinary` | `0x00a92900` | `CStaticBlock`/`CAnimatedBlock` records, `CVertexCompressionParams`, vertex buffer, index buffer; reads via `CMemoryDataInputStream` pointer-bump + `CDataInputStream::ReadWithSrcChunkOverflow` (a chunk stitcher, NOT a codec) |
| 5 | `C3DPrimitive2::LoadVertexBuffer` | `0x00a91560` | `Read(nVerts*stride)` raw block copy into a locked D3D VB; copies the 8-dword comp params to VB+0x14 |
| — | `C3DPrimitive2::CalculateVertexSize` | `0x00a910e0` | stride from FVF flags via `CVertexShaderInput{Static,Animated}::Initialise` |
| — | `CVertexShaderInputStatic::Initialise` / `…Animated::Initialise` | `0x00a90560` / `0x00a90630` | vertex-declaration element lists (animated adds bones) |
| — | `CVertexShaderInput::GenerateD3DDeclaration` | `0x00ab3870` | type→byte-size table (stride accumulator, §5.1) |

Buffer fill (bank layer): `CBankFileAsyncData::SetupBuffer` @ `0x009d52d0` (allocate only),
`OnFileReadCompletion` @ `0x009d54a0` (ready flag, no transform), `RelocateData` @ `0x009d5670`
(pointer rebase), `CBankFile::ReadEntryData` @ `0x009cc2a0` (raw `file->Seek(entryOffset+off)` via
`vtbl+0x14`, `file->Read(len, dst)` via `vtbl+0xc` on the object at `this+0x7c`),
`CBankFile::GetEntryDataSize` @ `0x009cc240`, `CBankFile::OpenReadOnly` @ `0x009d06f0` — the file at
`+0x7c` is a plain `CDiskFileWin32` (`CCountedPointer<CDiskFileWin32>::operator=`), bank header read
through a plain `CFileDataInputStream` (`0x4000` buffer); **no decompressing stream class at
`+0x7c`.** Snippets:

```c
// BuildMesh 0x00a24520
uVar3  = *(ulong *)(*(CBankFileAsyncData **)(this + 0x24) + 0x10);   // entry size
pvVar5 = CBankFileAsyncData::GetData(*(CBankFileAsyncData **)(this + 0x24)); // raw buffer ptr
CMemoryDataInputStream::CMemoryDataInputStream(local_1c, pvVar5, uVar3);
bVar2 = C3DMesh2::LoadBinary(*(C3DMesh2 **)this_00, local_1c, ...);
// CBankFile::ReadEntryData 0x009cc2a0
(**(code **)(**(int **)(this + 0x7c) + 0x14))(uVar1 + param_2);      // file->Seek
(**(code **)(**(int **)(this + 0x7c) + 0xc ))(param_2, param_3, 0);  // file->Read
// CBankFile::OpenReadOnly 0x009d06f0
CCountedPointer<CDiskFileWin32>::operator=((CCountedPointer<CDiskFileWin32> *)(this + 0x7c), &uStack_40);
CFileDataInputStream::CFileDataInputStream(aCStack_24, *(CAFile **)(this + 0x7c), 0x4000);
```

xref evidence (`FindRefs`/`XrefTree.java`, depth 4): `lzo1x_decompress` @ `0x00c06b90` has exactly
ONE static caller, `FUN_00996610` (call site `0x009966e5`) = the chunked-LZO **writer** (compress +
verify; callers `SaveGameState`@`0x4b64e0`, `TransferBinaryOut`@`0x6ba030`, `SaveBinary`@`0xa2eaa0`/
`0xa75400`/`0xa89450`/`0xa8fd40`/`0xac3b00`); `lzo1x_decompress_safe` @ `0x00c08170` is called only
by the streaming-font bank (`CStreamingFontBank` ctor @`0x9a3c20` via `AcquireFont`@`0xa5f1c0`,
`GlyphLookup`@`0x9a1140`, `FindGlyph`@`0x9a4160` — the font ctor uses the same generic
`GetEntryDataSize → malloc → ReadEntryData → CMemoryDataInputStream` shape and LZO is inside the
font asset). The 21 `Decompress/Inflate/Uncompress/Lzo`-named functions are all zlib
(`CCompressorZlib::{Decompress,DecompressStream,InternalDecompress,FinishDecompressStream}`
`0x00a3a840`..`0x00a3abe0`, `z_inflate` @ `0x00c02470`; `CDefinitionManager::DecompressDefinitionData`
@ `0x009ad5f0` feeding `CMemoryDataInputStream::OpenMemorySource`; `CFileInstaller::
ProcessActiveFileForDecompression` @ `0x009c3c20`) or the empty stub `CACompressor::Decompress` @
`0x0052adf0`; none is called by `BuildMesh` / `LoadBinary` / the `CBankFileAsyncData` fill path.

**Reconciliation:** the on-disk LZO framing is not in doubt (ratios above; the same `size-3` + 3
framing is exactly what `FUN_00996610` writes). The static xref pass therefore **missed the real
inflate caller** — most likely a chunked-LZO *wrapper* mirroring EgoCore `DecompressLZO` (frames
`[u16 clen]` blocks, calls `lzo1x_decompress` internally) reached via indirect/vtable dispatch (the
`CDecompressorInterface` family), or a second LZO copy; `ReadWithSrcChunkOverflow` in step 4 is the
prime suspect. The alternative hypotheses recorded at the time — (a) EgoCore decodes the Xbox layout,
(b) the retail archive is expanded once at install time by the zlib `CFileInstaller` — are refuted
for the *installed* PC `graphics.big` by the measured per-block ratios. **Settle the exact call site
with a live breakpoint on `ReadEntryData` (`0x009cc2a0`) / the bank read during a mesh load.** This
does not affect tooling.

---

## 5. Vertex layouts and index encoding — PROVEN (loader-level)

### 5.1 Declaration type → byte size (`GenerateD3DDeclaration` @ `0x00ab3870`)

| component type code | byte size | meaning |
|--------------------:|----------:|---------|
| 0,4,5,7,9,0xc,0xd | **4** | packed dword (D3DCOLOR / UBYTE4 / packed normal / s11e4 UV pair / bone idx+weight) |
| 1,6,0xb | **8** | FLOAT2 / SHORT4 / packed pair |
| 2 | **12** | FLOAT3 — float position |
| 3 | **16** | FLOAT4 |
| 8 | **2** | short |
| 10 (0xa) | **6** | SHORT3 — packed s16×3 position |

Declaration per FVF-flag class (`flags = primitive dword & ~4`) from the two `Initialise` routines;
stride = `sum(type sizes)` — reproduces the wiki `sVert` stride set {12,20,28,36}:

| path | flags | declaration (type codes) | **stride** | wiki sVert match |
|------|------:|--------------------------|-----------:|------------------|
| static, packed-coord | `0x04` | `[0xd,0xc,7]` | **12** | packed-coord class, stride 12 |
| static | `0x06` | `[0xd,0xc,7,0xb]` | **20** | sVert 20, stride 20 (bump) |
| static, float-coord | `0x14` | `[2,0xc,7]` | **20** | sVert 20 (FLOAT3 pos), stride 20 |
| static, float-coord | `0x16` | `[2,0xc,7,0xb]` | **28** | sVert 20 + bump, stride 28 |
| animated (skinned) | `0x04` | (adds bones) | **20** | sVert 4 + bones, stride 20 |
| animated (skinned) | `0x16` | | **36** | sVert 22, stride 36 |

- Position is always the **first** element: `type 2` raw `FLOAT3` (12 B) for float-coord classes;
  packed-coord classes use a 4-byte packed dword (`0xd` in the `0x04` declaration; decoder name
  POSPACKED3, 11/11/10 dequantized via comp params — the retail stride-12 layout is 4+4+4). The
  loader pass also listed `type 10` SHORT3 (6 B) as a packed-position candidate; the 4-byte form is
  the retail-attested one. `isPosComp = (InitFlags&4) && !(InitFlags&0x10)`.
- **Packed positions are dequantized in the vertex SHADER at draw time**, not at load: the loader
  copies the 8-dword `CVertexCompressionParams` (4× scale + 4× bias) verbatim (VB+`0x14`). Vertex
  bytes are *quantized* but the buffer is *uncompressed* in memory.
- Packed normal = one dword (11/11/10). UV = 2×i16, `(v+8)*2048` (s11e4-class), precision 1/2048.
- **V flip:** `_prim_geometry` yields Blender-convention `1-v`; `compose_mesh` stores `1-v` back.

### 5.2 Retail layout survey and what the composer emits

Survey of 400 random type-1 retail entries: `init 0x04/stride 12` ×443 prims, `0x14/20` ×68,
`0x06/20` ×68 (bump), `0x16/28` ×25 (bump). No float2-UV strides (24/32/36) exist in retail type-1 —
the "uncompressed" static layout is float3 positions with PACKED normal/UV:

| layout | InitFlags | stride static / skinned | position | normal | UV |
|---|---|---|---|---|---|
| `'float'` (default) | 0x14 | 20 / **28** (retail hero boots) | FLOAT3 (exact) | packed 11/11/10 dword | 2×i16 |
| `'packed'` | 0x04 | 12 / **20** (retail creatures) | POSPACKED3 via comp params | packed dword | 2×i16 |

`'packed'` compression params (EgoCore ImportType1): `offset = bbox center`, `scale = extent*0.505`
per axis (min 1e-4), `scale[3]=1, offset[3]=0`. Quantisation ≈ extent/2048 (x/y) — fine for props;
use `'float'` when exact positions matter. Packers (`pack_normal`, `compress_uv`,
`pack_pos_packed3`) are exact inverses of the decoder, so re-encoding decoded data is lossless.
Not emitted: bump/tangent layouts (0x06/0x16), repeated-instancing (Reps>1), cloth,
helpers/dummies/volumes/generators/packed-names.

### 5.3 Index encoding

Indices are **plain u16**, one `memcpy` (`Read(nIndices*2)`) in `C3DPrimitive2::LoadBinary`, handed
to `CIndexBuffer::Initialise` with `EPrimitiveType = 4`. No delta/strip codec; strips vs lists are
selected per block by `IsStrip`. The composer emits triangle LISTS only (engine-valid; retail mixes).

### 5.4 Why pattern-scanning failed (five scans, loop-prevention record)

(1) plain u16 index array (== `3×nFaces`) covering all verts — none; (2) `nVerts×20` FLOAT3-first
block followed by a u16 index block — none; (3) index-first search over strides
{12,16,20,24,28,32,36,48} × {FLOAT3, s16×3} — none; (4) contiguous FLOAT-run map (≥20 dwords) — zero
long runs (positions interleaved / packed); (5) strided-float run detector — only short degenerate
16-vertex runs. Consistent with the finding: geometry sits at an offset only the deserializer
computes, and (once LZO is known) inside compressed frames. The `SUBM`-style "header" `parse_mesh.py`
keyed on (`nVerts,nFaces,nFVI,sVert`, `nFVI==3×nFaces`) is a **stats/LOD metadata** record, not the
vertex/index block (on `MESH_SEAGULL_01` the bytes after it are bbox floats).

---

## 6. Legacy byte-level analysis — `SUBM` counters, `sVert` classes, CRC correlation

From the 2026-07-19 byte-only pass (`parse_mesh.py`); still a valid self-consistency check:

```
u32  nVerts                 # unique vertices in this submesh
u32  nFaces                 # triangles
u32  nFaceVertexIndices     # == 3 * nFaces   (100% of detected headers)
u32  sVert                  # VERTEX-FORMAT CLASS code: one of {4, 6, 20, 22}
u32  flag                   # small (0/1) — [hypothesis: destroyable-mesh-level / strip flag]
```
(The wiki's fuller `SUBM` is `nFaceVertexIndices, nFaceVertexIndices_BoneIndice, nVerts, nFaces,
nSourceVerts`.) Scanning all compiled payloads yields **6,449 candidate headers, 6,449 (100 %) with
`nFaceVertexIndices == 3 × nFaces`** and `sVert ∈ {4,6,20,22}`. Distribution: `sVert 4 → 5,837` ·
`6 → 196` · `20 → 384` · `22 → 32`. Named submeshes (`Body`, `Wing`) each carry an id == matTexId
slot; payload-side material records were labelled `GMAT`/`GMATE` in that pass.

`sVert` → stride & layout (fabletlcmod.com wiki `file_formats:bbm`):

| sVert | stride | fields |
|------:|-------:|--------|
| 4  | **12** | packed coords, packed normals, s11e4 tu tv |
| 4  | **20** | packed coords, **bones**, packed normals, s11e4 tu tv |
| 4  | 36 | float coords, float normals, float tu tv, dword meshlevel |
| 6  | **28** | packed coords, bones, packed normals, s11e4 tu, tv, dword[2] |
| 20 | **20** | float coords, packed normals, s11e4 tu tv |
| 20 | 28 | float coords, bones, packed normals, s11e4 tu tv |
| 22 | 36 | float coords, bones, packed normals, s11e4 tu, tv, dword[2] |

A given `sVert` code selects among stride variants by the presence of bones (skinned); the code
alone is ambiguous, skel_flag / block context disambiguates **[hypothesis]**. Float-coord
corroboration: the tagged-BBM `VERT` chunk's first triple decodes as `(-333.78, 509.40, -186.50)`
for a wall-collision mesh.

**CRC ↔ vertex class** (ties to [BIG.md §8.1](BIG.md#81-crc-toc-0x14--not-a-payload-hash-a-per-format-class-fingerprint);
graphics.big has 1,289 distinct CRCs):

| CRC | n | dominant sVert set | reading |
|-----|--:|--------------------|---------|
| `0xae689191` | 1,247 | `{4}` (931) | packed-coord meshes (the big class) |
| `0xceff9b23` | 298 | `{4}` (262) | packed-coord |
| `0x73fe4814` | 140 | `{20}` (46) | float-coord meshes |
| `0xe85b047b` | 94 | `{20}` (42) | float-coord |
| `0x7b646ea0` | 72 | `{20}` (31) | float-coord |

`0x73ab9c0d` covers only types {6,7,9} with `InfoSize=24`; `0xae689191` covers types {1–5}. The
CRC is the serialization-class stamp; `sVert`/InitFlags is the concrete per-primitive layout.

---

## 7. Tagged BBM (Type 3, `3DMF`) — the self-describing physics variant

Chunk presence over all 1,383 type-3 entries: `3DMF 1383 · MTLS 1383 · SUBM 1284 · PRIM 1367 ·
UNIV 1356 · VERT 600 · HLPR 794 · BONE 1`. Layout (wiki, cross-checked against bytes):

```
3DMF  "Copyright Big Blue Box Studios Ltd." then 3DRT
  MTLS   material list →  MTRL { u32 BASE_Texture_ID, BUMPMAP_Texture_ID, REFLECT_Texture_ID,
                                 Glow_Strength, Alpha_Enabled, Degenerate flag }  (+ MTLE/MMAP)
  SUBM   sub-mesh { nFaceVertexIndices, nFaceVertexIndices_BoneIndice, nVerts, nFaces, nSourceVerts }
    TRFM   transform matrix
    PRIM   primitive → TRIS triangle list
    VERT   vertex list  /  UNIV unique-vertex info
    SMTH   smoothing groups ;  VGRP vertex groups ;  BONE bones (below) ;  CLTH cloth
  HLPR   helpers → HPNT points, HDMY dummies, HCVL convex volumes
BONE  sub-chunk1 { long Index, Parent, nChild; f32 Matrix[12] }
      sub-chunk2 { f32 Matrix[12] } ; sub-chunk3 { f32 Matrix[16] }
```

Chunk framing is `TAG` + a `WriteCBYTE`/`ReadCBYTE` control-byte-encoded length/count (not a plain
u32). Engine readers (plain copies, no codec):

| Function | Retail addr | Behaviour |
|----------|-------------|-----------|
| `C3DMeshFileXVertexDataChunk::ReadFromFile` | `0x00aa66b0` | reads 4 fields, `resize(stride*count)`, plain block copy of vertex bytes |
| `C3DMeshFileXTriangleStripChunk::ReadFromFile` | `0x00aa6960` | reads a count, plain copy of `count` × u16 indices |
| `C3DMeshFilePrimitiveChunk::ReadFromFile` | `0x00abcf40` | count then a plain `u32` material-index array |
| `C3DMeshFileSubMeshChunk::ReadFromFile` | `0x00ab9fc0` | name + 4 dwords (index/parent/child/sibling) then super-chunk |

A collidable custom prop needs a paired type-3 BBM entry referenced by Info `PhysicsIndex`
(composer currently emits `PhysicsIndex=0`); EgoCore has the separate `3DMF` writer.

---

## 8. Skeleton — bone bind-pose blocks (CONFIRMED, ported from EgoCore 2026-07-20)

Position: after the stats counts, iff `BoneCount > 0`, before the 48-byte `RootMatrix`. Empirically
validated against retail by the Blender addon headless test (bone positions land inside the mesh
bbox).

| # | framing | size (bytes) | content |
|---|---------|--------------|---------|
| 1 | RAW | `2*BoneCount` | `u16 BoneIndices[]` — local slot → **GLOBAL Fable bone ID**. NOT parent indices. Helper/dummy/generator `BoneIndex` fields and anim tracks use these global IDs; remap global→local before use. |
| 2 | chunked-LZO | `BoneNameSize` | `BoneCount` NUL-terminated bone-name strings in local-slot order (`Scene Root`, `Bip01`, ...). |
| 3 | chunked-LZO | `60*BoneCount` | `C3DBone { u32 NameCRC; i32 ParentIndex; i32 OriginalNoChildren; float LocalizationMatrix[12] }` — **ParentIndex is the LOCAL-slot parent, −1 = root** (the hierarchy). NameCRC = bytewise CRC-32 (poly 0xEDB88320, init 0). LocalizationMatrix = 3×3 at [0..8] (= transpose of the IBM linear part, the bind-global rotation rows) + IBM translation at [9..11]. |
| 4 | chunked-LZO | `48*BoneCount` | per-bone TRS ("BoneKeyframesRaw"): `float quat[4]` (x,y,z,w) at [0..3], `float trans[3]` at [4..6], pad, `float scale[3]` at [8..10], pad. Bind pose ≈ identity quat + (−IBM translation). Not needed to build the skeleton. |
| 5 | chunked-LZO | `64*BoneCount` | **4×4 float INVERSE BIND MATRIX** per bone, row-major storage of a COLUMN-VECTOR-convention matrix: rows = floats [0..3],[4..7],[8..11], translation at elements 3,7,11. The 4th row ([12..15]) is NOT a valid matrix row — every consumer replaces it with (0,0,0,1). |

Bind math (EgoCore `MeshProperties.h` / `GltfExporter.h`):
```
IBM_i  = block5[i]  (4th row forced to 0,0,0,1)
W_i    = inverse(IBM_i)              # world/model-space bind matrix; W_i[.,3] = bone head
L_i    = inverse(W_parent) @ W_i     # node-local bind (glTF node matrix), parent from block3
```
`clone_skeleton(donor_payload, type)` captures blocks 1–5 RAW from a donor mesh so they survive
**byte-exact** (no bind-matrix re-derivation). Earlier note: bone matrices in the `LoadBinary` walk
are `0x30` B × nBones + a `0x30`-B matrix, preceded by a `u16×nBones` name-offset table.

---

## 9. Per-vertex skin weights (skinned/animated layout) — CONFIRMED

Source: EgoCore `GltfExporter.h::Export` vertex loop (`hasBones` branch, `JOINTS_0`/`WEIGHTS_0`),
`MeshParser.h CAnimatedBlock` (lines 67-69, parse ~717-721); empirically confirmed by
`tests/blender_skin_test.py` (every skinned vertex ≥1 group, weights sum ≈ 1.0, real bone names).

**A primitive is SKINNED iff `AnimatedBlockCount > 0`.** Skinned primitives add **8 bytes** per
vertex — 4 joint bytes + 4 weight bytes — **immediately after the position, before the normal**:

| field | offset | size | encoding |
|-------|-------:|-----:|----------|
| position | 0 | 4 (POSPACKED3) or 12 (FLOAT3) | §5 |
| **joint indices** | `iOff = isPosComp?4:12` | **4 × u8** | palette index × 3 (below) |
| **weights** | `wOff = iOff + 4` | **4 × u8** | `w[k]/255`, then renormalize |
| normal | `normOff = iOff + 8` | 4 or 12 | §5 |
| UV | `uOff = normOff + (isNormComp?4:12)` | 4 or 8 | §5 |

- **Weight decode:** `w[k] = wgt[k]/255.0`; `sum = Σw`; if `sum > 0.001` divide each by sum, else
  force `w=[1,0,0,0]`. Zero-weight influences dropped; 4 slots per vertex. **Retail: weight bytes sum
  EXACTLY 255 (82,073 / 82,073 retail skinned verts) and max 3 non-zero influences retail-wide**
  (`BonesPerVertex=3`). The composer forces the sum to 255 (EgoCore: remainder → `fw[0]`).
- **Joint decode:** each joint byte `ind[k]` is a *triangle-stride palette index*: `pID = ind[k]/3`
  indexes the per-animated-block palette `Groups[]`; `localBone = AnimatedBlocks[blk].Groups[pID]`
  is the **LOCAL bone slot** (0..BoneCount-1), NOT a global Fable ID — no `BoneIndices[]` remap for
  weights (that remap is only for helper/dummy/generator `BoneIndex` and anim tracks). Clamp
  `localBone < BoneCount` (fallback 0). Hard grammar limit 85 palette entries (joint byte =
  paletteIndex*3); EgoCore emits ≤16 per block, retail reaches 18.
- **Block ownership:** vertices are grouped by animated block in file order; block `blk` owns the
  next `AnimatedBlocks[blk].VertexCount` vertices (walk a running counter, advance when
  `proc >= VertexCount`). Each block has its own `Groups[]`.
- **Blender mapping:** one vertex group per bone (decoded name); assign `(localBone → weight)`
  pairs; `ARMATURE` modifier on the mesh pointing at the imported armature; edit-bones created in
  local-slot order so `vertex_groups[model.bones[localBone].name]` binds correctly.
- **Export (`mesh_rw.encode_skin`):** top-4 Blender vertex-group influences → bone → position in the
  primitive's `Groups[]` (append if absent); `ind[k] = paletteIndex*3`, `wgt[k] = round(weight*255)`
  with u8 renormalization, at `iOff`/`wOff`.

---

## 10. Composer recipe — `mesh_rw.compose_mesh` + `big_write.rebuild(adds=)`

```python
payload = compose_mesh(name, prims, materials,            # static type-1
                       layout='float'|'packed', compress=True)
payload = compose_mesh(name, prims, materials,
                       skeleton=clone_skeleton(donor_payload, 5))   # skinned type-5
out = big_write.rebuild(graphics_big_bytes,
                        adds=[{'sub': 'MBANK_ALLMESHES', 'name': name, 'payload': payload,
                               'type': 1 or 5, 'info': info_blob}])
```
(`adds=` record synthesis, ID allocation, stats-header/footer bump, `crc=0`/`timestamp=0` are
specified in [BIG.md §10](BIG.md#10-writer-b--toolsbig_writepy-rebuildbuf-editsnone-addsnone-new-toc-entries).)

**Static (v1):** header per §3.1 (AnimatedFlag 0, BoneCount 0, one static block per prim,
`DegenerateTriangles` sentinel appended), primitives per §3.3, ghost LOD per §3.4, Info blob per §2
with `LODCount=1`, `LODSizes[0]=len(LOD0)`, `SafeBoundingRadius=0`, no LODErrors.

**Skinned (v2) deltas** (retail-verified on goblin / seagull / hero boots + EgoCore `ImportType5`):
- `AnimatedFlag = 1` in LOD0 AND ghost; real `BoneCount`/`BoneNameSize`; the five bone blocks (§8)
  emitted between `TotalAnimatedBlocks` and `RootMatrix`, cloned RAW from a donor (retail boot /
  creature / the mesh's own decode).
- NO `DegenerateTriangles` sentinel.
- Prims have `StaticBlockCount=0` and ≥1 `CAnimatedBlock` `{PrimitiveCount, StartIndex, IsStrip=0,
  ChangeFlags=0, Degenerate=0, VertexCount, BonesPerVertex=3, PalettedFlag=1, groupCount, Groups[]}`.
- Vertex layouts grow by 8 bytes (§9): `'float'` → 0x14 / stride 28, `'packed'` → 0x04 / stride 20.
- **Block partition** (`_auto_skin_blocks`): single block when the used bone set fits one palette;
  else EgoCore's greedy face walk (palette cap 16) with per-block vertex regrouping/duplication.
  `decode_to_compose_args` passes `skin_blocks` preserving the SOURCE partition so a recompose keeps
  original palettes and vertex order.
- Per-vertex skin input: `prim['skin'][v] = [(bone, weight), ...]`, bone = local slot int **or a bone
  NAME** resolved via the skeleton's name list (authoring format: "rig to `Bip01 L Foot`").
- Ghost LOD carries the (full cloned) skeleton. Info blob unchanged (`LODCount=1`; boots
  retail-attest this shape). Add the entry with **type=5**.

### 10.1 Validation results (2026-07-20, all PASS — `compose_test.py <copy of graphics.big>`)

1. **RECOMPOSE oracle (static)** — `MESH_OBJECT_STATUE_BEAR` (2983 verts, packed stride 12, 38-bone
   skeleton block dropped), `MESH_OBJECT_PLATE`, `MESH_OLD_VILLAGE_SIGN`: decode → compose →
   re-decode; verts/faces **exact** (packed re-quantisation through the original comp params is
   lossless), UVs exact, material texture IDs preserved.
1b. **RECOMPOSE oracle (skinned)** — `MESH_CREATURE_GOBLIN` (67 bones, 4 prims, 7 animated blocks,
   strips, packed 0x04/20), `MESH_SEAGULL_01` (27 bones), `MESH_HERO_APPRENTICE_BOOT_L_01` +
   `_ASSASSIN_` (10-bone hero-rig subsets, float 0x14/28): verts/faces/UVs exact; `Groups[]`
   palettes **identical**; per-vertex skin identical (weights within 1/255, bone sets equal); the
   skeleton blocks **byte-exact** (`clone_skeleton(recomposed) == clone_skeleton(source)`); decoded
   bones (names/parents/IBMs/global ids) identical; ghost LOD parses with 0 prims + full skeleton.
   Grammar-exact, not byte-exact: strips become lists; deeper LODs, helpers/dummies/packed-names/cloth
   dropped.
2. **SYNTHETIC** — textured cube (24v/12f, `'float'`) and 5-blade grass cluster (20v/10f,
   `'packed'`, two-sided transparent): compose → parse → geometry matches (float exact; packed
   within 1 lsb).
2b. **SYNTHETIC SKINNED** — `MESH_FORGE_TEST_BOOT` (donor boot geometry ×1.15, re-skinned **by bone
   NAME** to the same 10 hero-rig bones, auto single-block) and `MESH_FORGE_TEST_RIBBON` (41 bone
   slots of the goblin skeleton → greedy **multi-block** partition, 3 blocks, palettes ≤16, vertex
   duplication): face-correspondence check proves positions + per-corner skin match.
3. **NEW-ENTRY oracle** — cube/grass/boot added to a COPY of graphics.big as ids 8113/8114/**8115
   (type 5)** (MBANK_ALLMESHES, TOC walks CLEAN, `entry_count` 8115): re-opened with
   `fable_core.open_graphics_big`, found by name, decoded geometry + Info texture IDs match;
   `build_model` yields full models — the boot with `has_skeleton`, the donor's 10 bones by name,
   `GeometryData.skin` matching the authored influences.
4. **No-edit regression** — `rebuild(buf)` round-trips all 8113 entries' record fields + payload bytes.

Existing suites re-run, all PASS: `tests/test_core.py`, `blender_headless_test.py` (seagull),
`blender_bones_test.py`, `mesh_rw.roundtrip_report` on the bear (`ok: True`).

**Blender end-to-end** (`blender_compose_test.py`, headless): composes cube + grass + the NEW SKINNED
boot, adds them to a copy of graphics.big (boot as type 5), runs the existing import operator
(`import_scene.fable_model`) with `textures_big=` pointing at retail textures.big, and asserts real
textured meshes appear and the boot returns with an armature carrying exactly the donor's 10 bones,
an ARMATURE modifier + parenting, vertex groups that are all real bones, and weights matching the
authored skin (≤1.5/255). Proof `.blend` + render PNG: `tools/blender_addon/tests/proof/
fable_compose_*.blend/png`.

### 10.2 Honest limits (v2)

- Skeleton must be CLONED from an existing mesh (no from-scratch skeleton authoring — a from-scratch
  path needs NameCRC + LocalizationMatrix + IBM emission, layouts in §8). Static meshes that carry a
  skeleton but no skinned prims (the bear) DROP it on recompose.
- **LOD0 only** (+ ghost). No LOD-chain authoring (`LODCount` fixed at 1). Retail creatures ship 3–4
  LODs; a recomposed goblin keeps only LOD0.
- Layouts `'float'`/`'packed'` only (§5.2 exclusions). Triangle lists only.
- Materials bind by textures.big entry ID; new-texture authoring is `TEXTURE_WRITER.md`.
- Engine smoke test was not part of the compose task (grammar-identical to retail; a follow-up wires
  a composed boot into a `CAppearanceModifierDef Graphics[]` id — `HERO_APPEARANCE.md#equipment-and-clothing-system` §7).
- Blender "Export NEW mesh" operator wiring (`import_op.py`, following `ExportFableModel`) is the
  natural follow-up.

---

## 11. EgoCore cross-reference (2026-07-31, `docs/journal/2026-07/EGOCORE_ASSESSMENT_20260731.md`)

EgoCore independently confirms our ghost-LOD, `DegenerateTriangles`-sentinel, `LODSizes[]=byte-sizes`
and skinning contract (weights sum 255 via remainder→`fw[0]`, joint=palette×3, max 3 influences, raw
60 B bone clone). It EXTENDS beyond our retail-verified type-1/type-5 work — treat as
EgoCore-sourced, verify against a retail diff before relying on it for a shipped mod:
- Ghost LOD for types 1/2/4/5 via `BankEditor.h::FlushStagedEntries` (§3.4); 2/4 to-verify.
- Full mesh-type map 1/2/3/4/5 (§1); type-2 stride-36 instanced layout tiled ×reps (cap 65535);
  type-3 physics via separate `3DMF` writer + Info `PhysicsIndex`.
- Skin: `BonesPerVertex=3`, palette cap 16 (retail 18, grammar 85).
- Port target for a decoder is `C3DMeshContent::Parse`, not the engine `LoadBinary` walk.
- Do NOT copy EgoCore's `CDefStringTable::GetCRC` (wrong for names.bin; unrelated to meshes).

---

## 12. Feasibility verdicts (evolution)

- 2026-07-19 (byte-only): metadata / material re-bind **GO** (edit `matTexId[]` / LOD distances, no
  CRC change); whole-mesh-entry swap **GO** (payload copy + descriptor); geometry swap **NO-GO** "until
  the BBB compressor is reversed"; community path = Fable Explorer / `.x` model injector.
- 2026-07-19 (loader decompile): no bespoke codec exists — the blocker is the deterministic
  serialization walk (`C3DMeshStats::LoadBinary 0x00a2f2d0` → volumes → generators → bone-name table
  → names → matrices → materials → primitives → `CStaticBlock`/`CAnimatedBlock` → comp params →
  VB → IB); writing is symmetric (`C3DMeshFileX*Chunk::WriteToFile` / `WriteBinary`).
- 2026-07-19/20 (EgoCore port): buffers are chunked-LZO; full decode + byte-exact segment round-trip;
  **compose GO** for static and skinned; new entries append cleanly.

---

## 13. Verified facts (with addresses)

- Load chain: `C3DMeshLODInfo::OnResourceLoaded` `0x00a25470` → `BuildMesh` `0x00a24520` →
  `C3DMesh2::LoadBinary` `0x00a8ad40` → `C3DPrimitive2::LoadBinary` `0x00a92900` →
  `LoadVertexBuffer` `0x00a91560`; `CalculateVertexSize` `0x00a910e0`;
  `CVertexShaderInputStatic/Animated::Initialise` `0x00a90560`/`0x00a90630`;
  `GenerateD3DDeclaration` `0x00ab3870`; `CIndexBuffer::Initialise` `0x00ac1f70`;
  `C3DMesh2::RelocateData` `0x00a8b4c0`; `CIndexBuffer::RelocateData` `0x00ac1f50`;
  `SetupVertexBuffer` `0x00a8eda0`; `CStaticBlock::LoadBinary` `0x00a8ea70`;
  `C3DMeshStats::LoadBinary` `0x00a2f2d0`; `BuildMesh` alt `0x00a25d20`.
- Bank layer: `CBankFileAsyncData::SetupBuffer` `0x009d52d0`, `OnFileReadCompletion` `0x009d54a0`,
  `RelocateData` `0x009d5670`; `CBankFile::ReadEntryData` `0x009cc2a0`, `GetEntryDataSize`
  `0x009cc240`, `OpenReadOnly` `0x009d06f0` (`+0x7c` = `CDiskFileWin32`).
- Tagged-BBM readers: `0x00aa66b0`, `0x00aa6960`, `0x00abcf40`, `0x00ab9fc0`.
- LZO: `lzo1x_decompress` `0x00c06b90` (single static caller `FUN_00996610` @ `0x00996610`, site
  `0x009966e5`), `lzo1x_decompress_safe` `0x00c08170` (font only); zlib `z_inflate` `0x00c02470`,
  `CCompressorZlib` `0x00a3a840`..`0x00a3abe0`, `DecompressDefinitionData` `0x009ad5f0`,
  `ProcessActiveFileForDecompression` `0x009c3c20`, `CACompressor::Decompress` `0x0052adf0` (stub).
- Descriptor zero-leftover 3,294/3,295; material xref 9,148/9,148; `SUBM` `nFVI==3*nFaces`
  6,449/6,449; `sVert ∈ {4,6,20,22}`; `LODSizes` = byte sizes; ghost LOD uncounted
  (`MESH_OBJECT_PLATE` 1189 = 971 + 218); `DegenerateTriangles` sentinel static-only; skinned weight
  bytes sum 255 (82,073/82,073), max 3 influences; `BonesPerVertex=3`, `PalettedFlag=1`; retail
  type-1 layouts 0x04/12, 0x14/20, 0x06/20, 0x16/28; skinned 0x14/28, 0x04/20; retail TOC ids
  sorted + unique.

---

## 14. Open questions

1. **Engine inflate call site** for mesh LZO blocks (static xref missed it; suspect
   `ReadWithSrcChunkOverflow` / a `CDecompressorInterface` wrapper via vtable) — settle with a live
   breakpoint on `0x009cc2a0` during a mesh load. Does not affect tooling.
2. Ghost LOD for types **2 and 4** — EgoCore-sourced, retail to-verify; type-2 stride-36 layout and
   type-4 particle grammar not retail-diffed.
3. From-scratch skeleton authoring (NameCRC / LocalizationMatrix / IBM emission) and LOD-chain
   authoring (`LODCount>1`, `LODErrors[]`).
4. `SUBM` `flag` dword meaning; `sVert` variant selection **[hypothesis]**; wiki `nSourceVerts` /
   `nFaceVertexIndices_BoneIndice` mapping onto the compiled record.
5. Bump/tangent (`0x06`/`0x16`) and cloth composition; helper points / dummies / volumes /
   generators / packed-names authoring.
6. In-engine acceptance of a `PhysicsIndex`-less composed prop vs a paired type-3 BBM.

---

## 15. Reproduce

```
python tools/parse_mesh.py "<install>\data\graphics\graphics.big"                 # classify + validate all
python tools/parse_mesh.py "<install>\data\graphics\graphics.big" --entry MESH_SEAGULL_01
python tools/parse_mesh.py "<install>\data\graphics\graphics.big" \
       --xref "<install>\data\graphics\pc\textures.big"                            # 100% material xref
python tools/decode_mesh_geo.py                                                    # stride table + decode facts

cp <install>/data/graphics/graphics.big <scratch>/graphics_copy.big
python tools/blender_addon/tests/compose_test.py <scratch>/graphics_copy.big
"C:\Programs\Blender\blender.exe" --background --factory-startup --python \
    tools/blender_addon/tests/blender_compose_test.py -- <scratch>/graphics_copy.big <out_dir>

# Ghidra ground truth (read-only headless; DecompByName / DecompFuncs / XrefTree / FindRefs):
#   ghidra_out/decomp_mesh_geo.c      chunk readers (VertexData/TriangleStrip/SubMesh/Primitive)
#   ghidra_out/decomp_mesh_geo2.c     RelocateData family + C3DMeshInfo::Sizeof
#   ghidra_out/decomp_mesh_geo3.c/.log BuildMesh 0xa24520/0xa25d20 + buffer lock/create
#   ghidra_out/decomp_mesh_geo4.c     C3DMesh2::LoadBinary + C3DPrimitive2::LoadBinary + Upload*
#   ghidra_out/decomp_mesh_geo5.c     CalculateVertexSize + LoadVertexBuffer + SetupVertexBuffer
#   ghidra_out/decomp_mesh_geo6.c     C3DMeshStats::LoadBinary
#   ghidra_out/decomp_mesh_geo8.c     CVertexShaderInput{Static,Animated}::Initialise + CStaticBlock::LoadBinary
#   ghidra_out/decomp_mesh_geo9.c     vertex-declaration element append
#   ghidra_out/decomp_mesh_geo10.log  GenerateD3DDeclaration type->size table
#   ghidra_out/decomp_mesh_bank_inflate_probe.c      FUN_00996610 (LZO compress+verify writer)
#   ghidra_out/decomp_mesh_bank_inflate_fontpath.c   CStreamingFontBank / GlyphLookup / FindGlyph
#   ghidra_out/decomp_mesh_bank_inflate_buildmesh.c  OnResourceLoaded + BuildMesh
#   ghidra_out/decomp_mesh_bank_inflate_loadvb.c     LoadVertexBuffer + C3DPrimitive2::LoadBinary
#   ghidra_out/decomp_mesh_bank_inflate_openread.c   CBankFile::OpenReadOnly
#   ghidra_out/decomp_bankfile.c / decomp_asyncdata.c / decomp_setupbuffer.c / decomp_lzo_named.c
analyzeHeadless ... -postScript XrefTree.java 4 0xc06b90 0xc08170
analyzeHeadless ... -postScript FindRefs.java c06b90        # -> 1 ref: FUN_00996610
```

## Verified facts (from FINDINGS log)

- **2026-07-20 — NEW-MESH COMPOSITION: custom static meshes as NEW graphics.big entries (2026-07-20).**
  `mesh_rw.compose_mesh` builds a complete compiled-mesh payload from arbitrary geometry;
  `big_write.rebuild(adds=)` inserts it as a NEW TOC entry (recipe `docs/formats/MESH_COMPOSE.md`;
  write-side answer key EgoCore `MeshCompiler.h` CompileSingleLOD).
  - Ghost LOD: retail single-LOD type-1 entries append an empty duplicate mesh block (header + zero
    mats/prims + identity RootMatrix) NOT counted in Info `LODCount` (PLATE: 1189 B payload = 971
    LOD0 + 218 ghost).
  - Sentinel material: material lists end with a `DegenerateTriangles` material (all map IDs 0,
    DegenerateTriangles=1); `MaterialCount` includes it.
  - Info blob = EgoCore `SerializeEntryMetadata` (`PhysicsIndex, bounds, LODCount, LODSizes[],
    SafeBoundingRadius, LODErrors[n-1], TextureIDs[]`); `lod[]` are LOD byte SIZES.
  - Retail type-1 vertex layouts (400-entry survey): init 0x04/stride 12 (packed pos+norm+UV,
    dominant), 0x14/20 (FLOAT3 pos + packed norm + i16 UV), 0x06/20 and 0x16/28 (bump). No float2-UV
    layout in type 1; composer emits 0x14/20 ('float') or 0x04/12 ('packed').
  - TOC conventions: ids unique + sorted per subbank (new id = max+1); name lpstr stored WITHOUT
    trailing NUL; stats header = type histogram (bump it); footer entry_count bumped; entry CRC is NOT
    CRC-32 of name/dep (bear and PLATE share ae689191) — new entries ship CRC=0/ts=0.
  - Bear statue gotcha: type-1 "static" entries can carry a full 38-bone skeleton block while
    primitives stay static (abc=0); compose v1 walks past and drops it.
  - Validation (all PASS, `tools/blender_addon/tests/compose_test.py` + `blender_compose_test.py`):
    recompose oracle on bear/plate/sign exact; synthetic cube+grass added to a graphics.big COPY as
    ids 8113/8114, TOC CLEAN; proof `tools/blender_addon/tests/proof/fable_compose_new_meshes.{blend,png}`.
  - Limits: static only, LOD0+ghost only, tri lists, no bump layouts/cloth/helpers; engine load not
    yet observed in-game.
