# Baked STB Landscape Bank Chunk — On-Disk Byte-Layout Spec

**Target:** the extracted payload of ONE `__ENGINE_SEA_STATIC_MAP_BANK_FILE__`
STB file-entry — a per-map *baked landscape MESH chunk* consumed by the retail
render path. Producing an engine-accepted one of these is the whole job of
`forge::stbbake`.

**Donor dissected:** `Darkwood_Filler_08.lev` — 133,140 bytes (0x20814).
Its `CStaticMapInfoBlock` (from the pristine STB COMMON_HEADER) reports:
`LandscapeMapPtr=124416 (0x1e600)`, `LocalDetailMapPtr=124437 (0x1e615)`,
`EdgeHeightFileSize=4675`, `BankFileIndex=173`.

**Status:** container framing PROVEN and round-tripped byte-exact
(`chunk_parse.py` → accounting=PASS, codec=PASS, roundtrip=PASS, 18/18 LZO frames
decode byte-exact with stock lzo1x). Inner per-patch VB/IB grammar decoded far
enough to emit; a few sub-fields remain **OPAQUE-BUT-PRESERVED** and are flagged.

Evidence sources:
- Writer: `landscape_savecomp_decomp.log` — `CLandscapeBackgroundPatch::Save@0x02ce3220`,
  `SaveCompressed@0x02ce30c0`, `Load@0x02cdf860`, `LoadCompressed@0x02cdf170`,
  ctor `@0x02cdb4f0`, `BuildVertexAndIndexBuffers@0x02ce16c0`.
- Loader: `retail_meshbuild_decomp.log` (`UpdateBackgroundLOD@0x00bdc180`,
  `UpdateForeground@0x00bdc510`), `retail_openmap_decomp.log`
  (`OpenRetailStaticMap@0x00b41e50`, `OpenDevelopmentStaticMap@0x00b42530`),
  `retail_loadbake_decomp.log` (`OpenStaticMap` variants).
- InfoBlock: `stb_baker/stb_infoblock_baker.py` (byte-exact for all 398 maps).
- Codec proof: `codec_foundation/codec_proof.exe` (stock minilzo).

---

## 0. TL;DR structural model

The chunk is a **flat image of fixed-size file blocks** managed by
`CEngineStaticMapFileBlockManager`. It is NOT one monolithic stream; it is a
grid of page-aligned blocks plus tightly-packed run-on frames. Every byte is
exactly one of:

| Kind    | Meaning                                                                 |
|---------|-------------------------------------------------------------------------|
| `FRAME` | a `SaveCompressed` **LZO1X** frame: `[uncompLen u32LE][compLen u32LE][lzo1x body]` |
| `HDR`   | uncompressed structured header (block directory / bbox / foreground array) — OPAQUE-BUT-PRESERVED |
| `PAD`   | zero bytes padding to the next block-manager page boundary              |

Donor accounting: **FRAME 116,288 B (87.3%) in 18 frames · HDR 3,397 B (2.6%) ·
PAD 13,455 B (10.1%) · TOTAL 133,140 = 100%.**

The two `Ptr` fields in `CStaticMapInfoBlock` are **absolute byte offsets into
this chunk stream** (see §1). `LandscapeMapPtr=0x1e600` and
`LocalDetailMapPtr=0x1e615` are only 21 bytes apart — `LocalDetailMapPtr` points
just past the landscape sub-header into the local-detail sub-stream that shares
the same trailing block.

---

## 1. Container framing & how the InfoBlock pointers index in

### 1.1 The pointers are ABSOLUTE STREAM OFFSETS, not block indices

`CEngineMap::OpenDevelopmentStaticMap@0x00b42530` (dev path) is decisive:

```
ReadMapInfoBlock(local_5c, stream);         // reads the 0x5C CStaticMapInfoBlock
...
CDataInputStream::SetPosition(stream, local_4c);   // local_4c = LandscapeMapPtr
CEngineLandscapeMap::OpenStaticMap(this, blockMgr, stream);
...
CDataInputStream::SetPosition(stream, local_48);   // local_48 = LocalDetailMapPtr
CLocalDetailCacheMap::OpenStaticMap(this[4], blockMgr, stream);
```

`SetPosition(stream, LandscapeMapPtr)` proves the pointer is a raw byte offset
into the bank-file stream. `EdgeHeightFilePtr`/`ChecksumBlockFilePtr` are the
same kind of absolute offset.

### 1.2 In the RETAIL path the same offset becomes a file-block handle

`UpdateBackgroundLOD@0x00bdc180`:
```
AddRequest(blockMgr, this[0x30]=LandscapeMapPtr, this[0x34]=size, false);
IsBlockLoaded(blockMgr, this[0x30]);
puVar3 = GetBlockData(blockMgr, this[0x30]);          // returns the block bytes
CMemoryDataInputStream(local_1c, puVar3, this[0x34]);
CLandscapeBackgroundTreeNode::LoadHeader(this[4], local_1c);
```
So retail hands `(offset,size)` to `CEngineStaticMapFileBlockManager`, which
returns the block content; the offset is the block key. Same number, two views
(seek-offset in dev, block-key in retail). **Emitter rule: the pointer is the
byte offset of the target sub-stream within the chunk.**

### 1.3 `CEngineLandscapeMap::OpenStaticMap@0x00bdd0e0` — the landscape sub-header

At `LandscapeMapPtr` the stream begins with a **0x14-byte (5×u32) header**
copied verbatim into the map object (`this+0x24 .. this+0x38`), fields include
the block `(offset,size)` used later by `GetBlockData`, followed by a texture
palette (`CEngineTexturePalette::Load`). At `LocalDetailMapPtr`
(`CLocalDetailCacheMap::OpenStaticMap@0x00bdf010`) the stream begins with a
**0xC-byte (3×u32) header** then `CQuadTreeElement::LoadHeader` +
`CObjectTypeCollectionPalette::Load`.

> **OPAQUE-BUT-PRESERVED:** the internal grammar of these two sub-headers and
> the two palettes is recorded verbatim by `chunk_parse.py` as `HDR` segments.
> An emitter preserves them from a donor and only rewrites the mesh `FRAME`s and
> the pointer values.

---

## 2. The LZO `SaveCompressed` frame (the unit that carries every baked mesh)

`CLandscapeBackgroundPatch::SaveCompressed@0x02ce30c0`:
1. `Save(...)` serializes the patch into a `CMemoryDataOutputStream` (§3).
2. worst-case buffer `local_60[0] = ((len*0x41)>>6) + 0x80` is allocated.
3. `lzo1x_999_compress(...)` compresses the Save stream.
4. `WriteSLONG(compressedLen)` then `WriteSLONG(local_60[0])` then the body.

**On disk the observed frame header is `[uncompLen u32LE][compLen u32LE][body]`.**
(The retail decoder `LoadCompressed@0x02cdf170` reads `ReadSLONG`=uncompLen,
`ReadSLONG`=compLen, allocates `uncompLen+3`, and `lzo1x_decompress`es the body;
it then asserts `output_size==uncompressed_size`.) The engine writes with
`lzo1x_999`; we emit with `lzo1x_1`; **same stock decoder, so RAW/any valid
lzo1x stream decodes losslessly** — no encoder parity needed.

Proven: all 18 donor frames decode with `lzo1x_decompress_safe` to EXACTLY the
declared `uncompLen`, rc `LZO_E_OK` (codec_proof.exe [B] + `frame_decode.exe`).

### 2.1 Frame placement / block grid

Two placements coexist in the donor and are both handled by `chunk_parse.py`:
- **page-aligned** frames at 0x1000, 0x3000, 0x5000, 0x7000, 0x11000, 0x1d000,
  0x1f000, 0x20000 — one per block-manager page; trailing bytes are zero `PAD`.
- **run-on** frames packed contiguously (a frame's body-end is immediately the
  next frame header) inside a larger page: 0x9800→0x10c42 and
  0x11000→0x1c967 are two such packed runs.

The four page-aligned frames at 0x1000/0x3000/0x5000/0x7000 compress ~2.1–2.3×
(VB/IB-dominated = the four background-LOD quadtree node meshes). The run-on
frames compress ~1.03–1.07× (DXT-texture-dominated = local-detail / foreground
patch meshes whose bytes are already-compressed DXT).

---

## 3. `CLandscapeBackgroundPatch::Save@0x02ce3220` — field-by-field

This is the content INSIDE a decompressed FRAME body. Order is the exact
`Write*` call sequence; offsets are the source `param_1+off` (object layout).
`W*` = stream write width.

| # | field (obj off) | W* | type   | meaning |
|---|-----------------|----|--------|---------|
| 1 | 0x110 | u16 | UWORD | patch grid width  `pw` (subdiv count X) |
| 2 | 0x112 | u16 | UWORD | patch grid height `ph` (subdiv count Y) |
| 3 | 0x10c | u16 | UWORD | world/local coord field |
| 4 | 0x10e | u16 | UWORD | world/local coord field |
| 5 | 0x11a | u8  | EBOOL | `IsWaterOnly` — if true, SKIP the whole mesh block (jump to §5) |
| 6 | 0x118 | u8  | UBYTE | `DetailMode` (1 ⇒ full-detail patch; else LOD-subsection) |

If `IsWaterOnly (0x11a) == 0`:

| # | field (obj off) | W* | type   | meaning |
|---|-----------------|----|--------|---------|
| 7 | 0x106 | u16 | UWORD | `IndexCount`  (triangle count; IB has `IndexCount*3` u16) |
| 8 | 0x104 | u16 | UWORD | `VertexCount` (VB has `VertexCount` verts, stride 0x18) |
| 9 | 0x108 | u8  | UBYTE | texture extent X arg (→ `C2DExtentsI`) |
|10 | 0x10a | u8  | UBYTE | texture extent Y arg |
|11 | —     | u8  | EBOOL | `IsDXTCompressed` (true ⇒ D3DFMT_DXT1 0x31545844; false ⇒ best-alpha fmt) |

**Texture:** `CTexture::SaveToDataStream` → **19-byte header + raw DXT payload**
(NOT a codec). Loader mirror: `LoadFromDataStreamToPreallocatedSurface`. The
texture is captured at the `*param_3` cursor recorded by `Save`.

**Vertex buffer (VB):** `Lock` then a loop `for i in 0..VertexCount`, each vertex:

| W* | type  | meaning (stride 0x18 = 24 bytes on GPU side) |
|----|-------|----------------------------------------------|
| u16 | UWORD | vtx.x (local grid X) |
| u16 | UWORD | vtx.y (local grid Y) |
| f32 | Float | vtx.z (height, world units) |
| u32 | ULONG | packed normal (`GetPackedNormalFromTargetVertex`) |
| u16 | UWORD | (byte at +0x16) — UV/lock hi |
| u16 | UWORD | (byte at +0x15) — UV/lock lo |

After the loop the writer asserts `savedVertexSize(0x10)*VertexCount ==
stream.GetLength()` (i.e. the *packed* per-vertex record it hands to
CRangeCompressor is **16 bytes**, distinct from the 0x18 GPU stride).

**VB compression:** `Compress_CRangeCompressor(peekData, VertexCount, 0x10, arr)`
→ `WriteSLONG(len)` + compressed VB bytes. CRangeCompressor is the self-describing
bit-packer with a **flags==0 RAW/stored fallback** — we can emit RAW and the
engine `Decompress_CRangeCompressor` restores losslessly.

**Index buffer (IB):** only if `0x119 == 0` (i.e. not a shared-IB full-detail
patch): `Lock`, `Compress_CRangeCompressor(ib, IndexCount*3, 2, arr)`
→ `WriteSLONG(len)` + compressed IB (u16 indices, elem size 2).

> Observed in decoded frame @0x1000: `pw=4`, then a `CRangeCompressor` preamble
> `04 00 1b 01 27 04 03`, two little-endian lengths `0x1059,0x1059`, a zero run,
> then the interleaved VB records (`… 88 70 42` = f32 60.13 height) and a clean
> trailing u16 index list (`00 03 00 02 00 04 …`). This confirms the VB/IB
> grammar above.
> **OPAQUE-BUT-PRESERVED:** the exact CRangeCompressor bit-stream header layout
> (the `1b 01 27 04 03` preamble) is not needed to emit (use RAW flags==0) but
> its bytes are preserved in-frame when re-emitting a donor.

---

## 4. Edge-height strips & foreground patch array

### 4.1 Per-patch edge strips (end of every `Save`)
After the mesh, `Save` writes **four** `CPatchTesselationEdgeStrip::Save` records
(N/E/S/W tessellation seams), then an `EBOOL WaterSubPatch` flag; if set, a
`CEngineWaterBackgroundSubPatch::Save` follows. Loader mirror: `Load` reads 4×
`CPatchTesselationEdgeStrip::Load` in a `for(0..4)` then the water flag.

> **OPAQUE-BUT-PRESERVED:** the internal `CPatchTesselationEdgeStrip` record
> format is not separately decoded here; it lives inside the same FRAME body and
> is preserved verbatim. `EdgeHeightFileSize=4675` in the InfoBlock sizes the
> separate edge-height file sub-stream at `EdgeHeightFilePtr`.

### 4.2 Foreground patch array
`UpdateForeground@0x00bdc510` reads the foreground block via
`BeginReadDataAutoAlloc(entry, ptr, size = patchX*patchY*0x24)` — i.e. a flat
array of **`patchX * patchY` records of 0x24 (36) bytes each**
(`this+0xc`=patchX, `this+0x10`=patchY). Each record initialises a
`CEngineLandscapePatch` (`0x48`-byte runtime object) via `Initialise` +
`LoadHeader`. The foreground array lives in its own file block reached through
the async entry, carried as run-on FRAME(s) in the packed region.

> **OPAQUE-BUT-PRESERVED:** the 0x24-byte foreground record fields are recorded
> verbatim (they reference foreground mesh/texture resources by index).

---

## 5. Water-only patches
If `IsWaterOnly (0x11a) == 1`, `Save` emits only: the 6 header fields (§3 #1–#6),
then jumps straight to the 4 edge strips + water flag (§4.1). No VB/IB/texture.

---

## 6. Round-trip proof (`chunk_parse.py`)

```
RESULT: accounting=PASS  codec=PASS  roundtrip=PASS
  FRAME 116,288 (87.3%) x18 · HDR 3,397 (2.6%) · PAD 13,455 (10.1%) · TOTAL=133,140 = 100%
  18/18 frames decode byte-exact to declared uncompLen (stock lzo1x)
  reserialize(parse(x)) == x : PASS (byte-identical)
```

The parser classifies 100% of the bytes, proves every FRAME boundary lands
exactly (`off+8+compLen` == next segment start), decodes every LZO body to its
declared length with the stock decoder, and re-emits the chunk byte-for-byte.

---

## 7. Emitter (`forge::stbbake`) recipe implied by this spec

1. Lay out a flat block image on the block-manager page grid (0x1000 pages;
   run-on frames may pack inside a page).
2. For each background-LOD quadtree node and each foreground/local-detail patch,
   build the `Save` body (§3–§5) — VB/IB via CRangeCompressor **RAW (flags==0)**,
   texture as 19-byte header + raw DXT — then wrap it
   `[uncompLen][compLen][lzo1x_1 body]` (§2).
3. Emit the landscape sub-header (0x14) + texture palette at `LandscapeMapPtr`,
   the local-detail sub-header (0xC) + quadtree + palette at `LocalDetailMapPtr`,
   the foreground `patchX*patchY*0x24` array, and the edge-height file — initially
   **copied verbatim from a donor** (the `HDR` segments) until each is fully
   reversed.
4. Write the matching `CStaticMapInfoBlock` (already byte-exact via
   `stb_infoblock_baker.py`) with absolute offsets pointing at the sub-streams.

---

## 8. Biggest remaining unknown

**The exact grammar of the `HDR` segments — specifically the landscape/local-
detail sub-headers, the two palettes (`CEngineTexturePalette` /
`CObjectTypeCollectionPalette`), and the quadtree node directory that ties the
four background-LOD FRAMEs together (`CLandscapeBackgroundTreeNode::LoadHeader`).**

We have PROVEN they are page-aligned uncompressed structured blocks and can
preserve them verbatim from a donor, and we know the four background meshes are
the four page-aligned 2.1–2.3× FRAMEs — but we have not yet reversed how the
tree-node directory *references* those FRAMEs by offset. For a same-topology
custom map (same `pw/ph/patchX/patchY` as the donor) the donor `HDR` blocks can
be reused with only the pointers and mesh bodies swapped; for an arbitrary new
topology the tree-node directory format must be reversed next
(decomp `CLandscapeBackgroundTreeNode::LoadHeader` — not yet in the logs).
