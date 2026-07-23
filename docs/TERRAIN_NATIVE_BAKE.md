# Native terrain bake - forge owns the STB landscape bake (NO editor)

*From FableWin editor bake decomp (terrain_bake_decomp.txt, 147 fns) 2026-07-21. Feasibility: native-with-listed-ports.*

## Verdict
forge can OWN the STB baked-landscape bake editor-free for a SAME-TOPOLOGY, SAME-THEME custom region (the ForgeTest case), by re-emitting only the authored-height mesh FRAMEs raw and preserving every opaque donor structure (texture palette, tree-node HDR, local-detail region) verbatim. It CANNOT yet own an arbitrary-topology / new-texture bake, because several byte grammars are still opaque thunks (see remainingUnknowns). What is already shipped and proven in forge: the outer LZO1X frame codec (forge::lzo::compressFramed writes [uncompLen][compLen][body] in the loader's exact read order); the complete chunk segment model with byte-exact identity round-trip (stbbake::parseChunk/reserialize/emitIdentity, 46 segments byte-identical on ForgeTest); the size-absorbing re-lay + background-LOD quadtree-directory REWIRE (emitChunk, 4/4 wired); the read-side STB container with the alignment_ field; and the offline gates (stbvalidate S1/S2/S3/S7/T2). What is NOT yet in C++: (1) a StaticMapInfoBlock 0x5C reader/writer (exists only as the Python stb_infoblock_baker.py, round-trips 398/398 byte-exact — a pure transcription port, zero new RE); (2) an AlignData(pos,align) helper wired into emitChunk so re-laid frames land on the bank page boundary (donor 0x1000) — this is the single blocker that currently forces RawPassthrough-only; (3) a §3 patch Save-body WRITER (invert the already-correct parsePatchHeader) plus a CRangeCompressor flags==0 RAW inner-block writer for VB/IB (no encoder parity needed — the loader's stored-mode decode reproduces raw blocks); (4) a quad-dir GENERATOR (emitChunk only rewires an existing dir today); (5) the InfoBlock/subheader 5-dword rebase that emitChunk deliberately flags as un-wired (stbbake.cpp:344-349); and (6) forge::stb::save() to splice a chunk + patched common-header into the container. The honest verdict: registration + a donor-cloned authored-height chunk is fully buildable now with a mechanical port; a from-scratch mesh over the donor's textures is buildable once (2)+(3)+(4) land; a from-scratch NEW-texture/NEW-topology bake stays blocked on the opaque palette/tree-node/local-detail leaf grammars until bake-and-diff resolves them.

## STATUS 2026-07-21 (update) — SHIPPED + VALIDATED in forgecore
Read confirmed against `CLandscapeBackgroundPatch::Save @0x02ce3220` (full WriteUWORD/
WriteEBOOL/WriteFloat/Compress/WriteSLONG order, patch_save_decomp.txt) — the §3 body layout
below is now decomp-exact, not inferred. Implemented and unit-tested (`tests/test_formats.cpp`
testRangeCodec / testPatchBake / testPatchBodyAssembly, all green):
- `forge::rangecodec::decode` (full CRangeCompressor::Decompress port) + `encodeRaw` (flags==0 RAW).
- `forge::stbbake::serializePatchHeader` / `serializePatchVB` / `packNormal` / `rangeBlockRaw`
  (STEP 3 + the VB writer of STEP 4).
- `forge::stbbake::parsePatchBody` — segments a decompressed body into
  header/texture/VB/IB/trailer via a VERIFIED search (locates the VB block past an opaque
  texture of ANY content by requiring decode() to yield exactly vertexCount*16 bytes and the
  IB to yield indexCount*3 u16s — no texture-size formula needed). This is the READ side the
  "copy texture verbatim from the decoded donor body" minimal path depends on.
- `forge::stbbake::assemblePatchBody(pb, newHeights?)` — reassembles in exact Save order;
  height-only edit decodes the donor VB, replaces the f32 at record +4, preserves
  grid/normal/uv, RAW-reframes, and copies texture + IB + edge-strips/water verbatim.
- `forge::stbbake::patchBodyToFrameEdit` — wires an authored PatchBody into emitChunk's FrameEdit.

Validated two ways: (1) synthetic round-trip (opaque RANDOM texture + trailer) — parse→identity-
assemble byte-exact, height-edit changes only the height field; (2) **REAL DONOR** round-trip
(testRealDonorPatchBody against the retargeted ForgeTest chunk, `ForgeTest_chunk_retargeted.bin`):
all 4 mesh patches parse, locate their VB past the real compressed texture, and reassemble
BYTE-IDENTICAL — proving `decode()`'s compressed path handles the donor's range-compressed VB
(stride 16) and IB (stride 2) end-to-end, and all 4 accept height edits. The earlier real-donor
gap is CLOSED: donor-body segmentation + height-preserving re-emit is proven on retail-derived data.
Still not in C++ (unchanged): STEP 1 InfoBlock writer, STEP 2 AlignData, STEP 5 quad-dir
generator, STEP 6 InfoBlock rebase, STEP 7 forge::stb::save() — the remaining pieces to splice an
authored chunk back into the STB container.

## STATUS 2026-07-21 (2nd update) — per-chunk WRITE LAYER done (STEP 1/2/5 shipped)
Three of the five remaining pieces are implemented in forgecore and unit-tested:
- **STEP 1 — `forge::stbinfo`** (`stbinfo.hpp/.cpp`): `readInfoBlock`/`writeInfoBlock` for the
  0x5C `CStaticMapInfoBlock` in the engine's exact on-disk field order (ported from the proven
  `stb_infoblock_baker.py`, 398/398 donor round-trip). Golden-vector validated (`testInfoBlock`).
- **STEP 2 — `alignUp` + `EmitOptions.frameAlign`** (`stbbake`): re-lays FRAME segments on the
  bank page boundary (0x1000), regenerating the zero PAD as the alignment gap so a size-changing
  edit keeps every frame page-aligned. `frameAlign=0` preserves byte-exact identity (legacy);
  `frameAlign=0x1000` puts all 26 donor frames on page boundaries (`testEmitAlign`). NOTE: the
  available fixture is a *retargeted* (non-page-aligned) chunk, so the aligned emit is not
  byte-identical to it — byte-reproduction of a genuinely page-aligned RETAIL chunk is not yet
  validated (needs a clean retail-chunk fixture).
- **STEP 5 — `generateQuadDir`** (`stbbake`): inverse of `parseQuadDir`; reproduces the donor's
  4 quad-dir entries BYTE-EXACT (`testQuadDirGen`).

**Remaining = the CONTAINER-SPLICE layer (2 pieces), honestly scoped:**
- **STEP 6 — InfoBlock rebase in `emitChunk`**: blocked on RE, not just code. To rebase
  `landscapeMapPtr`/`localDetailMapPtr` after frames move, we must know WHICH frame each InfoBlock
  body-pointer targets (old offset → move-table lookup). That InfoBlock-pointer↔frame mapping is
  not yet established; `emitChunk` still flags such moves via `anyUntrackedMove`. Next RE step:
  correlate the InfoBlock's landscape/local-detail pointers with the chunk's frame offsets on the
  donor, then wire the rebase through `writeInfoBlock`.
- **STEP 7 — `forge::stb::save()`**: needs a container WRITER (only `Archive::open`/`read`/`extract`
  exist). Append an entry {id,type,name,size,offset=alignUp(tail,alignment)}, bump the
  `__STATIC_MAP_COMMON_HEADER__` count + append (nameNUL)(SLONG relOffset), re-derive
  StaticMap.absoluteOffset, and recompute the appended entry's container CRC (rule to confirm).
  Gate when built: `forge stb list` shows the new map; every untouched byte identical at the same offset.

## STATUS 2026-07-21 (3rd update) — STEP 6 rebase premise resolved
The editor write sequence proves the InfoBlock fields are **logical control-stream position
snapshots**, not physical LZO-frame offsets. `LandscapeMapPtr` is captured immediately before
`CEngineLandscapeMap::GenerateStaticMapEntry`, `LocalDetailMapPtr` immediately after it, and
`HeaderEndPtr` after local-detail generation. The retail Darkwood_Filler_08 record independently
confirms this: landscape→local-detail is only `0x15` bytes even though the physical landscape frame
region is far larger. Therefore recompressing or page-relaying frames must **not** rebase these
fields. The same-topology height path preserves every decoded frame-body length, so all logical
positions remain invariant; only physical quad-directory offsets/spans need rewiring.

`emitChunk` now accepts same-decoded-length edits/recompression and rejects decoded-length changes
(the latter can change the logical stream layout and remains out of scope). Real-donor gates pass on
both Darkwood_Filler_08 and ForgeTest: all 18/26 frames survive LZO1X-1 recompression, decode equal,
and remain page-aligned. This validation also fixed the frame scanner to accept legal LZO expansion
(`compLen > uncompLen`) on incompressible authored bodies. **STEP 6 is closed for the minimal
same-topology terrain path. STEP 7 container save is the remaining splice task.**

## STATUS 2026-07-21 (4th update) — STEP 7 container writer shipped
`forge::stb::appendStaticMap` and the `forge stb append/record` CLI now own the complete append
path. The writer preserves the source STB as a byte-identical prefix (apart from header fields
0x14..0x1F), appends the aligned map payload, rebuilds the common-header index, appends and rebases
the map's **full common record** (0x5C InfoBlock plus landscape/detail control subheaders through
HeaderEndPtr), then emits a cloned TOC with the relocated common entry and new map entry. Existing
payload offsets never move. Retail CRC audit: all 424 original entries store CRC=0, so the appended
entry correctly uses zero; there is no missing CRC algorithm.

An important correction was caught during retail validation: the common index targets a full map
record, not an isolated 0x5C InfoBlock. The rejected first prototype appended only 0x5C bytes; the
final API requires the complete record and rebases the five logical offsets at +0x20/+0x24/+0x58/
+0x5C/+0x71, while patching BankFileIndex at +0x04.

Retail gate is green on the pristine 597,979,518-byte FinalAlbion_RT.stb using the proven ForgeTest
record/payload: output reopens as **425 entries / 399 static maps**, id 426, payload offset aligned to
2048; all 597,979,518 original bytes are identical except the three intentional header dwords;
the 750-byte record differs from its donor only inside BankFileIndex and the five rebased pointers;
the appended chunk is byte-identical (SHA-256
`f9f96ba992c2d5b01bed072debc0401c1581114f21c9124f51ee4c5524e855ed`). Staged output:
`work/forgetest_bake/FinalAlbion_RT_step7.stb`; **not deployed**. STEP 7/G9 offline container gate is
closed. The remaining end-to-end gate is an explicit live engine load of the staged authored bake.

## The decompiled patch mesh byte layout (the missing piece)
Per-patch Save (CLandscapeBackgroundPatch::Save @0x02ce3220 / CEngineLandscapePatch::Save):

    u16 worldX, u16 worldY, u16 coord0, u16 coord1
    EBOOL isWaterOnly, u8 detailMode
    if !isWaterOnly:
      u16 indexCount, u16 vertexCount, u8 texExtX, u8 texExtY, EBOOL isDXT
      <inline texture: CTexture::SaveToDataStream = 19-byte header + raw DXT (NOT a codec)>
      VERTEX BUFFER, 16 bytes/vertex:
        u16 gridX, u16 gridY, f32 HEIGHT(=PeekLandscapeHeight), u32 packedNormal, u16 uv, u16 uv
        -> CRangeCompressor::Compress(stride=0x10); flags==0 RAW fallback = emit uncompressed
      INDEX BUFFER (if not shared): u16 * indexCount*3 -> CRangeCompressor (RAW fallback OK)
    4x CPatchTesselationEdgeStrip::Save ; EBOOL hasWater [+ water subpatch]

Outer frame grammar = [uncompLen s32LE][compLen s32LE][lzo1x body]. Engine uses lzo1x_999; forge
emits lzo1x_1 - both decode with the stock decoder, so no encoder parity is needed. Patch grid =
16x16 cells, 17x17 vertices (289), matching forge's existing PatchHeader.valid invariant. A
0x24-byte per-patch file-header table (8 dwords + flag byte) is back-patched after the bodies.

## KEY: only the outer LZO frame needs real compression
Every inner block can be emitted RAW and still render: VB/IB via CRangeCompressor flags==0 stored
fallback; texture is a header+raw-DXT copy; the outer frame forge already LZO-frames byte-exact.
So NO arithmetic-coder port is required to render authored terrain.

## Minimal path (author heights, keep donor textures)
Author heights only; keep the donor's textures, palette, tree-node HDR, and local-detail region byte-for-byte. Concretely: (1) parseChunk the ForgeTest donor clone (already relocated to ForgeTest coords). (2) For each of the §3 foreground/local-detail mesh FRAMEs, build a new Save-stream body from the authored PeekLandscapeHeight grid: keep the SAME pw/ph/coord0/coord1/detailMode/isDXT/texExtX/texExtY and the SAME inline DXT texture bytes copied verbatim from the decoded donor body (do NOT re-serialize the texture), and rewrite ONLY the per-vertex height f32 (offset +4 in each 16-byte vertex record) plus the recomputed 4-byte packed normal; emit VB and IB as CRangeCompressor flags==0 RAW blocks ([u32 rawLen][flags==0 header][payload]); carry the 4 edge strips + water bool verbatim from the donor body. (3) compressFramed each new body (lzo1x_1 — decodes identically to the engine's lzo1x_999). (4) emitChunk with AlignData page-alignment so every re-laid frame starts on 0x1000, rewiring the quad dir (already implemented) AND rebasing the 5 InfoBlock/subheader dwords for the size delta. (5) Preserve the tree-node HDR, texture palette, and the entire local-detail sub-stream as verbatim HDR segments — never regenerate them. This renders the AUTHORED hill using the donor's textures/themes with ZERO opaque-grammar dependency: the only genuinely new encoder is the CRangeCompressor RAW inner block, and even the DXT is copied not encoded. If even the mesh re-emit is deferred, the strictly-shortest path is pure RawPassthrough retarget (already proven, but serves the donor's heights, not authored ones).

## Implementation plan (forge::stbbake / forge::stb::save)
1. STEP 0 (no code): confirm the CRangeCompressor flags==0 RAW header byte layout by reading Decompress_CRangeCompressor @ landscape_savecomp_decomp.log:503/553 — pin the flag field width/position and whether an uncompressed-length dword precedes the payload. This is the ONE byte-detail the raw inner-block writer needs; everything else is already pinned.
2. STEP 1 — forge::stb::StaticMapInfoBlock (new header/src pair, e.g. stbinfo.hpp/.cpp): POD struct with the 17 named scalars + float[6] CameraMapBounds; readInfoBlock(span,off)->struct mirroring ReadMapInfoBlock @02d5c230; writeInfoBlock(struct)->0x5C bytes implementing the EXACT 18-step WriteMapInfoBlock @02d67ea0 permutation (VersionID, BankFileIndex, EdgeHeightFileSize, MapWidth, MapHeight, WorldX, WorldY, EdgeHeightFilePtr, LandscapeMapPtr, LocalDetailMapPtr, Quality, ShorePointArraySize, ShorePointArrayStart, LevelChecksum[ULONG], ChecksumBlockFilePtr, ChecksumBlockFileSize, CameraMapBounds[6 raw f32], HeaderEndPtr). Direct transcription of stb_infoblock_baker.py serialize_record(); gate: round-trip all 398 donor records byte-exact.
3. STEP 2 — forge::stbbake::alignUp helper + wire into emitChunk: alignUp(pos,align)=(pos+align-1)&~(align-1); pad=alignUp-pos zero bytes. In emitChunk's re-lay loop, before each FRAME segment emit alignUp(out.size(),align)-out.size() zero PAD bytes (pull align from Archive::alignment_, donor 0x1000). This removes the current RawPassthrough-only restriction and is missing-item #1 of NATIVE_TERRAIN_STATUS §3a. Gate: identity emit of the donor still byte-exact (pad math is a no-op when nothing moved).
4. STEP 3 — forge::stbbake::CRangeCompressor RAW writer (writeRangeRaw(bytes,count,stride)->[u32 rawLen][flags==0 header][payload]) using the layout pinned in STEP 0, plus a readRangeRaw for round-trip proof. Gate: writeRangeRaw then a stock-semantics readRangeRaw reproduces the input for a donor-extracted VB and IB block.
5. STEP 4 — forge::stbbake::writePatchBody (invert parsePatchHeader): emit [u16 pw][u16 ph][u16 coord0][u16 coord1][EBOOL isWaterOnly][u8 detailMode]; if !water: [u16 indexCount][u16 vertexCount][u8 texExtX][u8 texExtY][EBOOL isDXT] + inline texture blob (COPIED verbatim from the decoded donor body for the minimal path; the 19-byte SaveToDataStream header emit is a later, opaque item) + writeRangeRaw(VB, vertexCount, 0x10) + (if IB flag clear) writeRangeRaw(IB, indexCount*3, 2) + 4 edge strips (verbatim) + water bool. Provide buildVertexBuffer(heights,normals) that packs the 16-byte records [u16 gx][u16 gy][f32 h][u32 packedNormal][u16][u16]; the height and packedNormal are the only authored fields, the rest copied from the donor's decoded VB. Gate: writePatchBody of a donor patch's own fields reproduces the donor's decoded frame body byte-exact.
6. STEP 5 — forge::stbbake::generateQuadDir(frames, aabbs): synthesize the 0x24-byte QuadEntry array (flags, frameOffset, frameSpan=8+compLen, 6 f32 AABB in minX,minY,minZ,maxX,maxY,maxZ order) + zero terminator, driven by emitChunk's existing startMap/frameMoves. Force a single root file block (AssignFileBlocks forced=1 path) for the small ForgeTest region. Gate: generateQuadDir over the donor's frames reproduces the donor's directory bytes at 0x7fc (already parsed 4/4 by parseQuadDir).
7. STEP 6 — emitChunk InfoBlock rebase hook (EmitOptions.rebase): after a size-changing re-lay, recompute the 5 chunk-relative dwords (+0x20 landscape subheader, +0x24 localdetail subheader, +0x58 chunk-end, +0x5C landscape dword0, +0x71 detail dword0) from the new frame offsets using GetPosition-snapshot semantics, and rewrite both the subheader dwords in the chunk AND LandscapeMapPtr(0x10)/LocalDetailMapPtr(0x14)/HeaderEndPtr(0x58) in the owning 0x5C InfoBlock via writeInfoBlock. This replaces the anyUntrackedMove bail at stbbake.cpp:344-349 with an actual rebase. Gate: stbvalidate S4 landing + S7 wiring pass on the re-laid chunk.
8. STEP 7 — forge::stb::save() (Archive::save in stb.cpp): entry table append {id, type=0, name=fullPath, size, offset=alignUp(tail,alignment_), crc=container-rule}; __STATIC_MAP_COMMON_HEADER__ count++ + append (nameNUL)(SLONG relOffset), relOffset=absChunkOffset-common.offset, re-derive all StaticMap.absoluteOffset; recompute the appended entry's container CRC (forge already owns the CRC rule for other entries). Gate: forge stb list -> 399 static maps, appended entry at correct offset/size; every untouched payload byte-identical at the same offset.
9. STEP 8 (deferred / bake-and-diff, NOT on the minimal render path): reverse CTexture::SaveToDataStream 19-byte inline header, Save_CEngineTexturePalette/Save_CObjectTypeCollectionPalette, the tree-node SaveHeader field order, and the local-detail SaveFileBlock leaf grammar — only needed for NEW textures / NEW topology / from-scratch (not donor-preserved) palette + scenery. Author two 1-cell-different heightfields and diff chunks per BAKE_FORGETEST_TERRAIN.md to pin each.

## Validation (all OFFLINE, no in-game run)
All gates run OFFLINE — no in-game execution. (G0 identity) forge::stbbake::emitIdentity(donor)==donor byte-exact (already passing; guards that STEP 2 pad math and STEP 6 rebase are no-ops when nothing moves). (G1 InfoBlock) writeInfoBlock(readInfoBlock(x))==x for all 398 donor records, 0 byte diffs (mirrors stb_infoblock_baker.py 398/398). (G2 RangeCompressor RAW) readRangeRaw(writeRangeRaw(vb))==vb and ==ib for a donor-extracted block; confirms the flags==0 layout pinned in STEP 0. (G3 patch-body) writePatchBody over a donor patch's own decoded fields reproduces that frame's decoded body byte-exact (the §3 writer is the exact inverse of the already-proven parsePatchHeader). (G4 authored-height frame) after rewriting only height f32 + packed normal, decodeFrame of the re-emitted frame yields a body identical to the donor EXCEPT in the vertex height/normal bytes (byte-isolation via stbvalidate T2: every diff confined to frame bodies / quad entries, strayDiff==0). (G5 quad-dir) generateQuadDir over the donor's laid frames reproduces the donor's 0x7fc directory bytes; parseQuadDir then reads 4/4 back. (G6 structural acceptance) stbvalidate::validateChunk on the emitted chunk: S1 coverage/round-trip PASS, S2 all frames decode to exact uncompLen PASS, S3 every frame page-aligned-or-run-on PASS (proves STEP 2), S7 all quad entries wire to real frames with AABB min<=max PASS. (G7 diff parity) stbvalidate::diffAgainstReference vs donor: D1 frame-count parity, D2 quad-shape parity PASS. (G8 rebase landing) after a size-changing re-lay, S4 landscapeMapPtr/localDetailMapPtr land on the recomputed HDR boundaries and the 5 subheader dwords + InfoBlock 0x10/0x14/0x58 match the new offsets (proves STEP 6). (G9 container) forge stb list -> 399 static maps, appended ForgeTest entry at the aligned offset/correct size, every untouched entry byte-identical at the same offset, appended-entry CRC matches the container rule, and the common-header relOffset resolves ForgeTest to its own chunk. Deploy only when G0-G9 all PASS.

## Remaining unknowns
- CRangeCompressor flags==0 RAW header exact byte layout — flag field width/position and whether an uncompressed-length dword precedes the payload. Read Decompress_CRangeCompressor @ landscape_savecomp_decomp.log:503/553 before emitting; it is the single unverified byte-detail on the raw inner-block path (STEP 0). Low risk (read-only RE, block is self-describing) but MUST be pinned.
- GetPackedNormalFromTargetVertex packing — the 4-byte packed normal format written per vertex (WriteULONG). BuildTree @0180c90b shows the neighbourhood (PeekLandscapeHeight at the 4 corners -> C3DVectors, cross/edge magnitudes) but the exact pack (component bit widths / fixed-point / sign) is not in the seed dump. For the minimal donor-preserved path a flat/near-vertical normal or copying the donor's normal is acceptable (lighting-only, not load-critical), but a correct authored-hill normal needs this pinned.
- CTexture::SaveToDataStream 19-byte inline header field breakdown (thunk only in this decomp) — needed ONLY for NEW textures; the minimal path copies the donor's texture blob verbatim and never emits this header.
- Save_CEngineTexturePalette / Save_CObjectTypeCollectionPalette grammar (opaque thunks) — needed ONLY for from-scratch palettes; donor-preserved as verbatim HDR segments on the minimal path.
- Tree-node SaveHeader (background-LOD) exact field order + the leading node-AABB block + whether the 0x24 stride itself is byte-exact from-scratch — flagged as the open bake-and-diff item in BAKE_FORGETEST_TERRAIN.md. Minimal path mutates the donor's tree-node HDR (which forge round-trips byte-exact) and only regenerates the 0x24 entries, so this is not on the render-now path.
- Local-detail SaveFileBlock leaf object-cache-group grammar (absent from the dump) — reached only when scenery EXISTS; an empty/donor-preserved local-detail region sidesteps it entirely.
- STB container CRC rule for the appended entry — forge is stated to already own it for other entries; confirm the exact algorithm/seed against a donor entry before trusting a from-scratch appended-entry CRC (G9).

## C++ skeleton
See ghidra_out/_terrainbake_skeleton.txt for the forgecore-style entry-point skeleton.

## STEP 0 RESOLVED — CRangeCompressor RAW block format (was the last unknown)
Decompiled CRangeCompressor::Decompress @0x00f39ed0. The RAW/stored path (lines 82-96):
```
total = count * stride;
if (input[0] == 0x00) {          // flags byte 0 == RAW/stored
    memcpy(out, input + 1, total);  // verbatim element bytes follow the flag byte
    return;
}
// else: range/bit-unpack (the compressed path, NOT needed for authoring)
```
So a RAW CRangeCompressor block = **[u8 0x00][count*stride bytes verbatim]** (length 1+count*stride).
The caller wraps it as [s32 compLen][block], compLen = 1 + count*stride. No inner uncompressed-length
dword; count/stride come from the patch header (vertexCount, stride 0x10; indexCount*3, stride 2).
=> forge emits the VB/IB uncompressed by writing a single 0x00 byte then the raw element bytes.
Compress @0x00f3a2e0 ends by writing a 0x80 sentinel byte; the RAW path has no such trailer.
