# GBANK textures (textures.big / frontend.big) — Info descriptor, payload, codec, writer

*Merged reference (2026-09-07) of `BIG_TEXTURE_FORMAT.md` (authored 2026-07-19, texture-RE agent:
pure Python bytes + community wiki, NO Ghidra) and `TEXTURE_WRITER.md` (2026-07-20, texture-writer
agent: Route B pure Python `tools/texture_build.py`, numpy + Pillow). Container layout / TOC /
writer live in [BIG.md](BIG.md); the LZO1X codec in [LIONHEAD_LZ.md](LIONHEAD_LZ.md); compiled
meshes in [MESH.md](MESH.md). Where the 2026-07-20 work corrected the 2026-07-19 draft, the
current truth is stated in the body and the earlier claim is kept under
[Superseded](#11-superseded).*

**Status: READ + WRITE WORKING.** The 34-byte Info descriptor, pixel-format enum, payload framing
(mip 0 chunked-LZO1X, mips 1..n-1 raw), linear PC layout and the codec are all confirmed; a pure
Python encoder builds/replaces entries with three validation bars passing. Fields whose *purpose*
is inferred are marked **[hypothesis]**.

| Bar | Result |
|---|---|
| Identity oracle (decode retail → re-encode → decode) | base-mip PSNR **46.4–66.7 dB** over 10 random retail entries (DXT1+DXT3) |
| End-to-end (new image → entry → patched .big copy → independent reader) | **39.5 dB** via both `texture_build.decode_entry` and the Blender addon `fable_core.TextureBank`; **6,323/6,323** untouched entries byte-identical |
| SilverChest oracle diff (same PNG through `--texture-import`) | subheader **field-identical** except `MipSize0` (different LZO stream length); identical framing & mip layout; SC payload decodes cleanly in OUR reader |

Tools: `tools/parse_texture.py` (Info parser + framing walker + DXT1/DXT3/A8R8G8B8 linear
decoders), `tools/texture_build.py` (encoder / replace / roundtrip / decode), `tools/big_write.py
rebuild` (container side), `tools/lionhead_lz_compress.py` (`compress_fable_block`). Reference
oracles used for VALIDATION only: EgoCore (`TextureBuilder.h` / `TextureParser.h`) and SilverChest
`ModelExportBridge --texture-import` (FableMod.Gfx).

---

## 1. Where texture entries live

Both files are ordinary BIGB containers ([BIG.md](BIG.md), [TEXT.md](TEXT.md) for the shared
container grammar). Texture entries are the GBANK sub-banks:

| File | sub-bank | entries | notes |
|------|----------|--------:|-------|
| frontend.big | `GBANK_FRONT_END_PC` | 394 | GUI backdrops / title / keyboard sprites |
| textures.big | `GBANK_GUI_PC` | 34 | in-game GUI sprites (many non-pow2 alloc) |
| textures.big | `GBANK_MAIN_PC` | 6,290 | the bulk world/character/object textures |

Every entry has `InfoSize == 34` (the texture descriptor) and one `Dep` = the source `.tga` build
path (e.g. `\Dev\BBBProjects\Fable1_5Release\...\frontend\fable_BG_01.tga`).

---

## 2. The 34-byte texture Info descriptor (CONFIRMED)

Little-endian; equals EgoCore `CGraphicHeader` (28 B) + `CPixelFormatInit` (6 B), packed LE.
Offsets validated against every GBANK entry in both files (6,324 textures.big + 394 frontend.big).

| Off | Type | Field | Writer value / evidence |
|----:|------|-------|-------------------------|
| +0  | u16 | **Width** (allocated, pow2) — `AllocWidth` | alloc W; power-of-two in **6290/6290** GBANK_MAIN entries |
| +2  | u16 | **Height** (allocated, pow2) — `AllocHeight` | alloc H; power-of-two in 6290/6290 |
| +4  | u16 | Depth | 0 (2D; readers treat 0 as 1) — always 0 in samples |
| +6  | u16 | **FrameWidth** (real/authored width) | source image W; `Width ≤ AllocWidth` in **every** entry (0 exceptions) |
| +8  | u16 | **FrameHeight** (real/authored height) | source image H; `Height ≤ AllocHeight` in every entry |
| +10 | u16 | FrameCount | 1 (retail sprites: 4/16 exist — §9); 1 in all 2026-07-19 samples (read there as "Depth") |
| +12 | u32 | **PixelFormatIdx** | `0x1f` DXT1, `0x20` DXT3, `0x01` A8R8G8B8 (low byte; +13..+15 zero) — groups CRC + payload math (§3) |
| +16 | u8  | TransparencyType | DXT1: 0 (retail also uses 2 = punch-through class), DXT3: 1 (rare 4), ARGB: 0/1 |
| +17 | u8  | **MipmapLevels** (`MipCount`) | chain length (DXT: down to 4×4; ARGB: to 1×1); monotonic with max allocated dim (below) |
| +18 | u8  | Flags | 0 (rare 1 in retail; preserved on replace) |
| +19 | u8  | Padding | 0 |
| +20 | u32 | **FrameDataSize** (`allocbytes`) | **uncompressed** byte size of mip 0 = `(alloc_w/4)·(alloc_h/4)·unit` exactly for DXT (256×256 DXT1 → `0x8000` = 32768; 128×64 DXT3 → `0x2000` = 8192) |
| +24 | u32 | **MipSize0** | **on-disk** size of the mip-0 compressed region (chunk headers + chunk bytes + 3 raw tail bytes); **0 = payload all-raw**. Equals `clen0 + headerBytes + 3` — verified 394/394 frontend + 6165/6324 textures with the u16 header, the rest with the `0xffff` u32 form (`clen0 + 6 + 3`) |
| +28 | 6 B | **CPixelFormatInit** {Type, ColourDepth, R,G,B,A bits} — the class / D3DFORMAT tail | DXT1 `03 04 00 00 00 00`; DXT3 **`02 08 00 00 00 00`**; ARGB `01 20 08 08 08 08` |

**Proof the alloc/real split is real:** `AllocWidth/Height` are always powers of two and always ≥
the `Width/Height` pair, e.g. `FRONTEND_BACKDROP_01` = real **640×480** stored in an allocated
**1024×512** D3D texture (a classic non-pow2 image padded up to a pow2 surface). The
`FrameWidth/FrameHeight` pair is the image the artist authored; the `Alloc*` pair is the GPU
surface it is uploaded into.

**MipCount (+17) is the stored mip level count.** It rises with the max allocated dimension and
stops early for textures that don't ship a full chain:

```
maxAllocDim ->  4:1   8:2   16:1-3   32:1-4   64:1-5   128:3-6   256:3-7   512:5-10   1024:8-9
```

The **tail** bytes (+28) are the serialization-class descriptor that the TOC `CRC` fingerprints
(consistent with [BIG.md](BIG.md) round-trip §7: CRC is a per-format-CLASS stamp — the same CRC
value, e.g. `0x3a21ace1`, is shared by both `0x1f` and `0x20` entries that share a dims/mip class,
and `0x4e1ace9f` recurs across frontend.big **and** textures.big).

GUI sprites can have `FrameDataSize` > computed mip-0 size (frame-stride padding, e.g.
`EDITORGUI_BACKUP_ICON`) and `FrameCount` 4/16 — see §9.

**Runtime gotcha (ForgeTest64 stage20, see HANDOFF/memory):** inline background textures must store
ONE mip — the engine bounds by surface level count; a multi-mip inline background crashed.

---

## 3. Pixel-format enum → D3D format (CONFIRMED)

`Info[12]` histogram and the confirmed mapping:

| Enum | Name | Block/bpp | textures.big | frontend.big | Evidence |
|-----:|------|-----------|-------------:|-------------:|----------|
| `0x01` | **D3DFMT_A8R8G8B8** (uncompressed 32bpp) | 4 B/px | 8 | 125 | tail `08 08 08 08` = 8/8/8/8-bit channels; `alloc=real`, no mip padding; stored as BGRA bytes |
| `0x1f` | **DXT1** (S3TC, no/1-bit alpha) | 8 B / 4×4 | 3,703 | 116 | Info+20 allocbytes == `(w/4)(h/4)·8`; wiki: "DXT1 = no transparency"; tail `03 04` |
| `0x20` | **DXT3** (S3TC + explicit 4-bit alpha) | 16 B / 4×4 | 2,574 | 153 | allocbytes == `(w/4)(h/4)·16`; wiki: "DXT3 = has transparency" (Fable uses DXT1/DXT3, not DXT5); tail `02 08` |
| `0x23` | **[hypothesis]** DXT-class (16 B/block) | 16 B / 4×4 | 4 | — | rare; groups with DXT CRC classes; likely DXT5-class |
| `0x18` | **[hypothesis]** 16 B/block class | 16 B / 4×4 | 1 | — | single entry; unverified |

The wiki (fabletlcmod / Fable Explorer) independently states Fable textures are **DXT1** (no alpha)
or **DXT3** (alpha) — matching enums `0x1f`/`0x20`. Sources: fabletlcmod.com Fable Explorer wiki +
Graphics tutorials (cert fails verification, snippets only — flagged low-reliability, but they
agree with the byte evidence).

---

## 4. Payload recipe (CONFIRMED 2026-07-20 — only mip 0 is compressed)

Validated on **227/230** random retail entries (150 textures.big + 80 frontend.big, exact byte
accounting `consumed == entry size`; the 3 failures are multi-frame sprites, §9).

```
payload := for each frame (writer emits 1):
             mip0_region        ; iff MipSize0 > 0, else raw mip0
             raw mip1 … raw mip(n-1)     ; no headers, no compression

mip0_region := chunks… + 3 raw tail bytes, where the chunks LZO1X-encode
               bytes [0 .. rawlen-3) of the mip:
    chunk := u16 clen              ; 0xFFFF -> u32 clen escape follows
             clen bytes LZO1X-1    ; clen == 0 -> stored chunk (raw copy)
```

1. **Only mip 0 is LZO-compressed. Mips 1..n-1 are stored RAW**, tightly concatenated after the
   mip-0 region — they are *not* per-mip length-prefixed blocks. Source: EgoCore
   `TextureParser.h:289` `isCompressed = (m == 0 && Header.MipSize0 > 0)`; confirmed byte-exact on
   retail.
2. **`Info+24` (`MipSize0`) is the switch.** `MipSize0 == 0` ⇒ the **whole payload is raw** (no LZO
   anywhere) — this is what EgoCore/ChocolateBox mod-manager emits (`hdr->MipSize0 = 0`) and the
   loaders accept. Nonzero ⇒ it is the on-disk byte size of the mip-0 compressed region.
   Additionally `clen == 0` inside the chunk run means a *stored chunk*.
   `tools/texture_build.py replace --raw-mip0` emits the all-raw form.
3. Retail and this writer emit mip 0 as a **single** chunk
   (`lionhead_lz_compress.compress_fable_block`); the u32 escape covers clen ≥ 0xFFFF (SilverChest
   switches at 0x7FFF — both forms are read fine).
4. Pixel data is linear PC D3D9: DXT1 8 B / DXT3 16 B per 4×4 block, A8R8G8B8 stored as BGRA
   bytes.
5. Trailing bytes of the payload are zero padding up to the sub-bank `Align` (2048).
6. Container-side: the payload is placed by `tools/big_write.py rebuild` (align 2048); the 34-byte
   Info lives in the TOC record and MUST be patched when `FrameDataSize`/`MipSize0`/dims change —
   `texture_build.replace_entry` patches it in place before the rebuild (Info is the record's final
   field).

Stream facts from the 2026-07-19 survey (still true of the LZO1X chunk bytes): streams begin
overwhelmingly with `12 00 ..` / `15 ..` / `19 ..` (small first-byte values 0x12–0x21); no dominant
repeat stride (autocorrelation flat) — genuinely LZ-compressed. The codec is **LZO1X** (see
[LIONHEAD_LZ.md](LIONHEAD_LZ.md)); the 2026-07-19 "proprietary Lionhead LZ / not LZO" finding is
in §11.

---

## 5. Swizzle verdict — LINEAR (CONFIRMED, not Xbox-swizzled)

TLC is a **native PC D3D9** build (project axiom; `ImageBase 0x400000`, ships `msvcr71.dll`). The
descriptor carries a **PC D3DFORMAT class tail** (`03 04` DXT1 / `02 08` DXT3 /
`01 20 08 08 08 08` D3DFMT_A8R8G8B8) and standard **linear S3TC 4×4-block** dimensions; the
payload is a length-prefixed compressed mip-0 block + raw mips, not a morton/Xbox-tiled surface.
Xbox-swizzled textures would (a) not use PC D3DFORMAT descriptors and (b) tile the base surface
rather than LZ-compress it. The pixel layout is **standard linear** DXT1/DXT3/A8R8G8B8; **no
de-swizzle step is required** — proven by image since 2026-07-20 (decoded retail entries and
round-trips are coherent).

---

## 6. The encoder — `tools/texture_build.py`

- **DXT1/DXT3 block encoder**: numpy-vectorized stb_dxt-style — PCA principal axis per block
  (power iteration), endpoints from extreme projections, RGB565 quantization with the decoder's
  expansion, best-of-3 with two least-squares endpoint refinement passes. DXT1 always emits opaque
  4-colour mode (`c0 > c1` enforced). DXT3 alpha is rounded 4-bit (EgoCore truncates).
- **Mip chain**: 2×2 box filter (EgoCore uses stbir bilinear — equivalent for the exact ½ case).
  Retail's mip filter is the same 2×2 box average (A8R8G8B8 entries round-trip pixel-identical on
  every mip, §7).
- **Non-pow2 input** is Lanczos-resized up to the pow2 alloc surface on `build`; on `replace` the
  image is resized to the existing entry's alloc dims and real dims / mip count / TransparencyType /
  Flags are preserved.

```
python tools/texture_build.py selftest
python tools/texture_build.py build     art.png out --format dxt1 [--mips N] [--dims 512x512]
python tools/texture_build.py replace   src.big out.big ENTRY_NAME art.png [--sub GBANK_MAIN_PC]
                                        [--format dxt1|dxt3|argb8888] [--raw-mip0]
python tools/texture_build.py roundtrip textures.big [--samples N | --entry NAME]
python tools/texture_build.py decode    src.big ENTRY_NAME out.png
```

Read-side inspector:
```
python tools/parse_texture.py <frontend.big|textures.big>            # list entries + Info
python tools/parse_texture.py <file.big> --entry <id>                # one descriptor + framing
python tools/parse_texture.py <file.big> --png  <id> <out.png>       # decode (needs decompressed buf)
```
`parse_texture.py`'s DXT1/DXT3/A8R8G8B8→RGBA decoders are unit-tested (synthetic blocks decode to
the exact expected RGBA). Info-descriptor validation over all entries was reproduced by the
session scratch `validate_info.py`.

---

## 7. Validation detail (2026-07-20, retail Steam TLC)

**Layout probe** (scratch `probe_layout.py`): 150 random textures.big + 80 frontend.big entries
walked with exact byte accounting — **227/230 OK** (`MipSize0` == walked mip-0 region size, payload
consumed exactly). The 3 failures are multi-frame sprites (`FrameCount` 4/16:
`EDITORGUI_BACKUP_ICON`, `SPRITE_DUST_01`, `SPRITE_CAMPFIRE_01`) — §9.

**Identity oracle** (`roundtrip --samples 10`): decode retail base mip → re-encode full entry →
decode. DXT re-encode of already-quantized data reconverges: base-mip PSNR 46.42–66.71 dB (not
bit-identical — endpoint search from decoded pixels need not reproduce the original compressor's
choices; PSNR is the honest metric). Smaller-mip PSNR 25–36 dB against retail's mips is expected:
retail mips were filtered from the original TGA, ours from the decoded base. **A8R8G8B8 entries
round-trip pixel-IDENTICAL on every mip level** (frontend.big: `UI_SLOT_RIGHT_NODE_OFF_FE` 64×32/4
mips, `UI_TABLE_VK_SELECT_SPRITE_V_L` 16×16/3 mips) — which also shows retail's mip filter is the
same 2×2 box average this writer uses.

**End-to-end** (scratch `e2e_test.py`): synthetic 512×512 pattern PNG → `replace` of
`TEXTURE_OV_SMITH_TORSO_01` (DXT1, 8 mips) in a COPY of textures.big → re-decode: 39.47 dB (our
reader) / 39.44 dB (`fable_core` Blender-addon reader, independent code path); container check:
all 6,323 other texture entries byte-identical after rebuild.

**SilverChest oracle** (`--texture-import`, FableMod.Gfx): same PNG. Subheader fields identical
(alloc/real dims, frames, `PixelFormatIdx` 0x1f, transp 0, mips 8, `FrameDataSize` 131072, tail
`03 04 …`) except `MipSize0` (theirs 118034 vs ours 118837 — different LZO output length, both
valid). Framing identical: one chunk + 3 raw tail bytes + 43,688 bytes raw mips 1–7 (exact match
to the computed sum). SC's payload decodes to 36.02 dB vs the source pattern through OUR reader
(their encoder is slightly worse than ours).

---

## 8. Retexture feasibility & the UNASSIGNED_* terrain slots

- **Reading a texture's identity — GO.** `tools/parse_texture.py` extracts real dims, allocated
  dims, pixel format, mip count, and the source `.tga` name for every one of the ~6,700 texture
  entries in the two banks — enough to build a texture browser / catalog.
- **Round-trip container edit — GO (proven).** [BIG.md](BIG.md) shows replace-entry-payload
  (same-size and size-changing) is byte-exact and isolated, with CRC/Timestamp passthrough. A
  retexture that keeps the **same dims + format** needs no Info edit and rides the existing writer.
- **Authoring new pixels — GO** via `texture_build.py build/replace` (§6); in-game visual
  confirmation has since been exercised by the terrain and custom-NPC pipelines (HANDOFF).
- **Custom terrain textures (CLAUDE.md gotcha):** foreground triple values in the STB landscape
  chunk are **GBANK_MAIN_PC entry IDs** in `data/graphics/pc/textures.big` (NOT inline). There are
  **156 `UNASSIGNED_*` 512² slots** to repurpose. Pipeline = `texture_build.py replace <big> <out>
  <UNASSIGNED_slot> <png>` + `forge stb settex <chunk> <out> --map old:slotId` (FableForge). User
  texture library: `C:\Users\Cornelio\Documents\FableStaging\AIUpscale\`.
- **D3D9 frontend textures** (reconstruction `visual_boot_d3d9.cpp`) must be registered in BOTH
  the `FableInitialiseVisualD3D9` upload chain AND the `VisualRender2DAdapter`
  `RENDER2D_ADAPTER_ATTACH_TEXTURE` chain, else quads draw FLAT WHITE (CLAUDE.md).

---

## 9. Open gaps

1. **Multi-frame sprites** (`FrameCount` > 1: SPRITE_*, some EDITORGUI GUI icons, ~small
   population) — writer emits `FrameCount=1` only; the reader in `texture_build.decode_entry`
   decodes frame 0 only. Frame-stride padding (`FrameDataSize` > computed, e.g. 20×20 GUI icons)
   unhandled.
2. **Rare formats `0x23` / `0x18`** (5 entries total, likely DXT5-class) — not supported; `replace`
   requires `--format` override for them.
3. **DXT1 1-bit alpha (punch-through)** — encoder always emits opaque 4-colour mode; retail
   `TransparencyType == 2` entries would lose cutout alpha if replaced with our DXT1 (use DXT3
   instead).
4. **In-game visual confirmation** was not run in the 2026-07-20 session (format evidence = three
   independent readers/writers agreeing: retail bytes, EgoCore, SilverChest); later terrain work
   (stage33) has since rendered replaced textures in-game.
5. New-entry insertion (vs replacing an existing entry) awaited new-entry support in
   `big_write.py`; everything needed on the texture side (payload + Info) is produced by `build`.
   (`big_write.rebuild(adds=)` has since landed for meshes/anims — [BIG.md](BIG.md).)

---

## 10. Dated findings

- **2026-07-19** — 34-byte Info descriptor, pixel-format enum (`0x01`/`0x1f`/`0x20`, rare
  `0x23`/`0x18`), linear-PC verdict, and block framing recovered from bytes; codec left as a gap
  after 8+ decoder families failed; PNG not produced.
- **2026-07-20** — Codec = LZO1X ([LIONHEAD_LZ.md](LIONHEAD_LZ.md)); `texture_build.py` shipped;
  layout corrected (only mip 0 compressed, mips raw; `MipSize0 == 0` ⇒ all-raw; DXT3 tail
  `02 08`); 227/230 layout probe, PSNR 46.4–66.7 dB identity, 39.5 dB e2e, SilverChest field-identical.
- **2026-07-22 →** — Custom terrain retexture via `UNASSIGNED_*` slots + `forge stb settex`
  (CLAUDE.md); inline background textures must store ONE mip (ForgeTest64 stage20 root cause).
- **2026-09-07** — `BIG_TEXTURE_FORMAT.md` + `TEXTURE_WRITER.md` merged into this file.

---

## 11. Superseded

*Claims from the 2026-07-19 `BIG_TEXTURE_FORMAT.md` that the 2026-07-20 writer work corrected.
Kept verbatim for the record.*

- **Status-table verdicts "Pixel-payload compression codec — GAP — proprietary Lionhead LZ stream;
  not any standard codec; needs the engine decompressor (Ghidra)" and "A coherent decoded PNG — NOT
  achieved — blocked solely by the codec gap".** The codec is LZO1X. The 2026-07-19 attack record:
  8+ decoder families tried (flag-bit LZ in both bit orders, control-byte LZ, word-level LZ,
  LZ4-sequence, LZSS token grid — offset 11/12/13-bit × len 3/4/5-bit × add 1/2/3 × 1/2 offset
  bytes × both token orders — raw-deflate, and a preamble-strip variant); "not zlib/raw-deflate (no
  valid inflate at any offset), not LZ4-sequence, **not LZO**" — the LZO rejection was wrong
  (attempted with the wrong framing).
- **§4.1 "Each entry's payload blob is a sequence of per-mip compressed blocks, each with a length
  prefix"**:
  ```
  block := u16 clen                 ; header 2 bytes, when clen < 0xffff
         | u16 0xffff , u32 clen    ; header 6 bytes, when clen >= 0xffff
         followed by  clen  bytes of compressed stream
  ```
  with "Block 0 = the base (largest) mip … Subsequent blocks are the smaller mips … the post-base
  bytes are close to (but below) the uncompressed sum of the smaller mips." Only mip 0 is chunked;
  the "subsequent blocks" were additional chunks of the mip-0 region / misread raw data (§4).
  The block-0 grammar itself and the `Info[+24] == clen0 + headerBytes + 3` relation stand.
- **Info +28 tail "`03 04 00 00 00 00` for DXT1/DXT3"** — `03 04` is DXT1-only; DXT3 is
  `02 08 00 00 00 00` (100 % consistent in the 230-entry sample).
- **Info +4 "reserved / depth-hi" and +10 "Depth (=1 for 2D)"** — +4 is `Depth` (0) and +10 is
  `FrameCount` (1; 4/16 for sprites) per EgoCore `CGraphicHeader`.
- **Info +24 named "FirstBlockField"** — it is `MipSize0`, and `0` means all-raw payload.
- **§6 "Authoring new pixels — BLOCKED on the codec … a retexture pipeline can only swap in
  payloads that are already in the game's compressed form"** and the **"interim workaround worth
  checking: the engine loader may accept an uncompressed block (`clen == rawMipSize` as
  stored/literal)"** — resolved: `MipSize0 == 0` ⇒ all-raw payload, loader-accepted;
  `clen == 0` ⇒ stored chunk (§4). Authoring is GO.
- **§5 "linear remains formally unproven-by-image because the codec gap blocks producing the
  decompressed buffer"** — proven by image 2026-07-20.
- **§8 open gaps "(1) Pixel-payload LZ codec — recover from the `Fable.exe` GBANK texture
  decompressor via Ghidra; (3) confirm whether the loader accepts uncompressed (stored) blocks"** —
  both closed 2026-07-20 without Ghidra. Gap (2) — confirm `0x23` / `0x18` — remains (§9).
