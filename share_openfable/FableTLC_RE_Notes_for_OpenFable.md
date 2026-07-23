# Fable: The Lost Chapters — Reverse-Engineering Notes

Shared knowledge dump from a parallel TLC project (FableForge — a native
mod/authoring suite for the **retail** engine). Different goal from OpenFable
(you re-render the world in your own Vulkan engine; we make the shipped
`Fable.exe` load *new* content), so the overlap is in the **on-disk formats and
the retail engine's asset pipeline** — which is where these notes should be
useful to you, especially the parts your renderer deliberately skips.

All addresses are **retail `Fable.exe`** (ImageBase `0x400000`) unless prefixed
`FableWin:` (the TLC 1.5 dev editor from the 2013 Anniversary build, which ships
symbols/PDB). "Proven" = round-tripped byte-exact against real game data.

---

## 0. The one thing most relevant to your §7

Your TERRAIN.md §7 says: *"Retail uses STB-cached landscape patches; OpenFable
builds its own stride pyramid from the `.lev` grid."* We reversed those
STB-cached patches. **The retail engine does not tessellate terrain from the
`.lev` at load — there is no load-time heightmesh bake.** The visible ground is a
**pre-baked mesh** stored per-map inside `FinalAlbion_RT.stb`, and the `.lev`
(heights/themes) only feeds collision / nav / height queries at runtime.

Concretely, the retail render path is a straight pre-baked-mesh reader with a
flat fallback:

- `CEngineMap::OpenRetailStaticMap @0x00b41e50` → `ReadMapInfoBlock @0x00b3efa0`
  reads a per-map `CStaticMapInfoBlock` (0x5C bytes).
- Render-time, `CEngineLandscapeRenderer::LoadNewScene @0x00b684f0`
  → `CEngineLandscapeMap::UpdateBackgroundLOD @0x00bdc180`: if the map's
  background block id is set → `CEngineStaticMapFileBlockManager::AddRequest` +
  `GetBlockData` → `CLandscapeBackgroundTreeNode::LoadHeader` (loads the baked
  background mesh). **If it's 0 → `CLandscapeBackgroundTreeNode::BuildDefaultSection
  @0x00be6880` = a flat default = the void.**
- `CEngineLandscapeMap::UpdateForeground @0x00bdc510`:
  `CBankFileAsyncEntry::BeginReadDataAutoAlloc` reads `patchX*patchY*0x24` bytes
  of pre-baked foreground patch data → `CEngineLandscapePatch::LoadHeader`.
- The `.lev` cell grid is consumed only by `CMap` for logic:
  `DrawGetEngineThemeAt @0x008f75b0`, `DrawGetEngineBlendAt @0x008f75f0`,
  `PeekLandscapeHeight @0x00b43720`, `HasCollidedWithLandscapeAt`. None feed the
  render mesh.

So if you ever want **retail-faithful** LOD tiles (instead of your own pyramid),
the format below is what to load. `CEngineLandscapeRenderer::BuildThemes
@0x00b687d0` is a red herring — it builds **audio** themes (`CSoundThemeDef`,
symbol `"ENGINE_THEME"`), not terrain.

### The STB container itself
Pristine `FinalAlbion_RT.stb` = 424 entries: **25** `__ENGINE_SEA_STATIC_MAP_BANK_FILE__`
(region SEA/water banks) + **398** `Data\Levels\FinalAlbion\<map>.lev` **baked
mesh payloads** + **1** `__STATIC_MAP_COMMON_HEADER__` (holds the 398
`CStaticMapInfoBlock`s). Note: the STB `<map>.lev` entries are a **different
format** from the WAD source `.lev` — same name, different bytes (STB one starts
`01 00 00 00`, WAD one `19 00 00 00` = version 25). Every map, fillers included,
has a populated baked payload (e.g. `BanditCamp_Filler_05.lev` = 3.66 MB); none
are terrain-less.

---

## 1. Baked STB landscape chunk — byte layout (PROVEN, round-trips byte-exact)

Target: the payload of one baked-mesh STB entry. Donor dissected:
`Darkwood_Filler_08.lev`, 133,140 bytes. Its `CStaticMapInfoBlock` reports
`LandscapeMapPtr=0x1e600`, `LocalDetailMapPtr=0x1e615`, `EdgeHeightFileSize=4675`,
`BankFileIndex=173`.

**Structural model:** a flat image of page-aligned (`0x1000` grid) file blocks
managed by `CEngineStaticMapFileBlockManager`. Every byte is one of:

| Kind    | Meaning |
|---------|---------|
| `FRAME` | an LZO1X `SaveCompressed` frame: `[uncompLen u32LE][compLen u32LE][lzo1x body]` |
| `HDR`   | uncompressed structured header (block directory / sub-headers / palettes / foreground array) |
| `PAD`   | zero padding to the next page boundary |

Donor accounting (100%): **FRAME 116,288 B (87.3%) ×18 · HDR 3,397 B (2.6%) ·
PAD 13,455 B (10.1%) = 133,140.**

**InfoBlock pointers are ABSOLUTE BYTE OFFSETS into the chunk stream**, not block
indices. Proof: `CEngineMap::OpenDevelopmentStaticMap @0x00b42530` does
`CDataInputStream::SetPosition(stream, LandscapeMapPtr)` then
`CEngineLandscapeMap::OpenStaticMap`, and again `SetPosition(stream,
LocalDetailMapPtr)` then `CLocalDetailCacheMap::OpenStaticMap`. The retail path
reuses the same number as a file-block key via `GetBlockData`.

- At `LandscapeMapPtr`: a **0x14-byte (5×u32) landscape sub-header** copied into
  the map object, then a texture palette (`CEngineTexturePalette::Load`).
  (`CEngineLandscapeMap::OpenStaticMap @0x00bdd0e0`.)
- At `LocalDetailMapPtr`: a **0xC-byte (3×u32) header**, then
  `CQuadTreeElement::LoadHeader` + `CObjectTypeCollectionPalette::Load`.
  (`CLocalDetailCacheMap::OpenStaticMap @0x00bdf010`.)

### 1.1 The LZO frame (carries every baked mesh)
`CLandscapeBackgroundPatch::SaveCompressed @0x02ce30c0` (FableWin): `Save` into a
memory stream, alloc worst-case `((len*0x41)>>6)+0x80`, `lzo1x_999_compress`,
then `WriteSLONG(compressedLen)`, `WriteSLONG(uncompLen)`, body. On disk:
`[uncompLen u32LE][compLen u32LE][lzo1x body]`. Decoder `LoadCompressed
@0x02cdf170` reads both lengths, allocs `uncompLen+3`, `lzo1x_decompress`, asserts
output==uncompLen. **It's stock LZO1X** — the retail decompressor at
`lzo1x_decompress @0x00c06b90` is standard, workmem `0x70000` =
`LZO1X_999_MEM_COMPRESS`. We proved all 18 donor frames decode byte-exact with
vendored minilzo 2.10 `lzo1x_decompress_safe`. (Engine writes `_999`; you can
emit `_1` — same decoder.)

Placement: page-aligned frames (one per block-manager page, zero PAD to next
page) coexist with run-on frames packed contiguously inside a larger page. The
four page-aligned frames compress ~2.1–2.3× (VB/IB-dominated background-LOD
quadtree node meshes); run-on frames ~1.03–1.07× (already-DXT texture bytes).

### 1.2 `CLandscapeBackgroundPatch::Save @0x02ce3220` — inside a decompressed frame

| # | obj off | width | meaning |
|---|---------|-------|---------|
| 1 | 0x110 | u16 | patch grid width `pw` |
| 2 | 0x112 | u16 | patch grid height `ph` |
| 3 | 0x10c | u16 | coord field |
| 4 | 0x10e | u16 | coord field |
| 5 | 0x11a | u8 (EBOOL) | `IsWaterOnly` — if 1, skip mesh, jump to edge strips |
| 6 | 0x118 | u8 | `DetailMode` (1 = full-detail patch) |

If `IsWaterOnly == 0`:

| # | obj off | width | meaning |
|---|---------|-------|---------|
| 7 | 0x106 | u16 | `IndexCount` (IB has `IndexCount*3` u16) |
| 8 | 0x104 | u16 | `VertexCount` |
| 9 | 0x108 | u8 | texture extent X |
|10 | 0x10a | u8 | texture extent Y |
|11 | — | u8 (EBOOL) | `IsDXTCompressed` (true ⇒ `D3DFMT_DXT1` `0x31545844`) |

- **Texture:** `CTexture::SaveToDataStream @FableWin:0x030dbf30` = a **19-byte
  header + raw DXT** (NOT a codec). Header: width u16, height u16, noLevels u8,
  then a `CPixelFormatInit` (D3DFORMAT + flags); per mip: if compressed, the
  locked surface bytes verbatim (`GetLevelByteLength`); if not, row-by-row memcpy
  stripping lock pitch. Loader mirror `LoadFromDataStream @FableWin:0x030dc890`.
- **VB:** `for i in 0..VertexCount`, each vertex written as:
  `u16 x, u16 y, f32 z (height, world units), u32 packedNormal, u16, u16`
  (the packed record handed to the compressor is **16 bytes**; the GPU stride is
  0x18). Then `Compress_CRangeCompressor(data, VertexCount, 0x10, arr)`,
  `WriteSLONG(len)`, compressed bytes.
- **IB:** only if `obj+0x119 == 0`: `Compress_CRangeCompressor(ib, IndexCount*3,
  2, arr)`, `WriteSLONG(len)`, compressed u16 indices.
- **Edge strips:** four `CPatchTesselationEdgeStrip::Save` (N/E/S/W tessellation
  seams), then an `EBOOL WaterSubPatch`; if set, `CEngineWaterBackgroundSubPatch::Save`.
- **Foreground array:** `patchX*patchY` records of **0x24 (36) bytes** each
  (`UpdateForeground`'s `BeginReadDataAutoAlloc` size arg), each initialising a
  `CEngineLandscapePatch` (0x48-byte runtime obj).

Observed in the decoded frame @0x1000: `pw=4`, a `CRangeCompressor` preamble
`04 00 1b 01 27 04 03`, two LE lengths, a zero run, interleaved VB records
(`… 88 70 42` = f32 60.13 height), then a clean trailing u16 index list
(`00 03 00 02 00 04 …`).

### 1.3 CRangeCompressor (the inner VB/IB codec) — decoded
Despite the name it's **not** an arithmetic/range entropy coder — it's a per-block
**adaptive bit-packer with a RAW/stored fallback**. `Decompress @FableWin:0x033182e0`:
per block reads a flags byte, then:
- `flags == 0` → **stored/raw block**: read a varint size
  (`GetBlockSizeFromFlags`/`ReadVarSizedUInt`), copy verbatim.
- general path: read a symbol count; `flags & 0x11` → apply a value-remap
  dictionary (`CalcShuffleOperations`); `flags & 0x2` → read a bit-width clamped
  `[0,0x20)` and bit-unpack at that fixed width; optional bias (`TestRangeBias`).

No probability model, no carry propagation. Because the decoder is
self-describing and has the `flags==0` raw path, **you can emit VB/IB as raw
blocks and the engine decodes losslessly** — no need to reproduce the bit-packer
to get valid output. Full-parity encoder (for size) is ~1–2 days of deterministic
integer code: `WriteVarSizedUInt`, fixed-width pack, the shuffle dictionary, and
the per-block strategy chooser (`CalcBestCompressionForBlock`,
`TestRedundantBitStrip`, `CalcBitsNeeded`).

### 1.4 Still open (if you care about *authoring* new topology)
The internal grammar of the `HDR` segments — the landscape (0x14) / local-detail
(0xC) sub-headers, the two palettes, and specifically the
`CLandscapeBackgroundTreeNode::LoadHeader` **quadtree directory** that references
the four background-LOD frames by offset — is proven to be page-aligned
uncompressed structured data but not yet fully reversed. For *loading/rendering*
existing maps you can parse around it; for emitting arbitrary new topology it's
the last piece. (`SavePatchesToTemporyStream @FableWin:0x02deb570` is the editor's
emit side if you want the writer's view.)

---

## 2. `CStaticMapInfoBlock` (0x5C) — the per-map directory record
Lives in the single `__STATIC_MAP_COMMON_HEADER__` STB entry; 398 of them, one
per map. Reader `ReadMapInfoBlock @0x00b3efa0`; writer
`CEngineMap::WriteMapInfoBlock @FableWin:0x02d67ea0`. Fields include
`BankFileIndex`, `LandscapeMapPtr`, `LocalDetailMapPtr`, `EdgeHeightFilePtr` +
`EdgeHeightFileSize`, `ChecksumBlockFilePtr`, placement box, checksum. All
pointers are absolute byte offsets into that map's bank chunk (§1). We have a
byte-exact writer for all 398.

---

## 3. `.lev` (WAD source) — cross-check for your LEV_FORMAT.md
Matches what you have; noting our independent read in case it disambiguates:
- Version **25**; preface **512 theme slots × 132 bytes**; slot 0 = header,
  slot 1 = invalid standin, slots ≥2 = usable paint.
- Quad dims `W×H` at header `+0x26` / `+0x2A`.
- Vertex records **21 bytes**, tag 7: `f32 height`, three material bytes, blend
  flag / `rest[0]`.
- Trailing after the height grid: nav + water tables (we use these for
  collision/nav; largely what your §7 lists as undecoded). If useful we can share
  our nav-section notes — the retail nav is embedded in the LEV and consumed by
  `CMap::LoadFromFile @0x00821a80`.

---

## 4. Bonus: audio (`data\Sound\*.lug`) — you'll hit this for full-game
Sounds are **not** in `.big`; they're `LiOnHeAd`-magic segment banks
(`Ingame.lug` etc.), Xbox-IMA-ADPCM (`wFormatTag 0x0069`), OpenAL at runtime
(`alBufferData` sites `FUN_00c4dc40`/`FUN_00c4de00`). Block layout: named 32-byte
block tags + u32 length; `LHAudioBankSampleTable` = 652-byte records (dev path
260B, id, waveId, riffSize/riffOffset, a WAVEFORMATEX mirror @+292),
`LHAudioWaveData` = concatenated RIFF blobs, `LHAudioBankCriteiaInfo` = criteria
strings → sample-id arrays. Event routing: criteria string (e.g.
`SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS`) → CRC → `CSoundMap` → id array → bank
sample. There's a sibling `Ingame.met` (dev-path provenance sidecar; the runtime
sound loader does **not** reference it). We have byte-exact read+write for `.lug`
if you want it.

---

## 5. What we can share on request
Byte-exact readers/writers (in a C++ lib + Python tools) for: STB (`.stb`
container + `CStaticMapInfoBlock`), WAD, `.lev` (read), TNG, WLD/BWD (compiled
world), quest defs (`game.bin`/`frontend.bin` compiled-def container, zlib chunks
+ names.bin CRC), quest cards, text.big, `.lug` audio, and the FableSav save
format (seed-0 reflected CRC-32, 6 signature invariants). Plus the codec
foundation (vendored minilzo proof harness) and the full chunk-layout spec this
doc summarizes. Happy to trade for your `LEV_FORMAT.md` / `LEVEL_ADJACENCY.md`.

---

*Compiled from decompiles of retail `Fable.exe` and the FableWin 1.5 dev build
(2013 Anniversary), cross-checked against real game data with round-trip proofs.
Addresses are for those exact binaries; verify against your own build's layout.*
