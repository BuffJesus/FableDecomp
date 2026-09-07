# Native texture WRITE path — image → textures.big/frontend.big entry

*Authored 2026-07-20 (texture-writer agent). Route B: pure Python
(`tools/texture_build.py`, numpy + Pillow), no external exe dependencies.
Reference oracles used for VALIDATION only: EgoCore (`TextureBuilder.h` /
`TextureParser.h`) and SilverChest `ModelExportBridge --texture-import`.*

## Status: WORKING — all three validation bars pass

| Bar | Result |
|---|---|
| Identity oracle (decode retail → re-encode → decode) | base-mip PSNR **46.4–66.7 dB** over 10 random retail entries (DXT1+DXT3) |
| End-to-end (new image → entry → patched .big copy → independent reader) | **39.5 dB** via both `texture_build.decode_entry` and the Blender addon `fable_core.TextureBank`; **6,323/6,323** untouched entries byte-identical |
| SilverChest oracle diff (same PNG through `--texture-import`) | subheader **field-identical** except `MipSize0` (different LZO stream length); identical framing & mip layout; SC payload decodes cleanly in OUR reader |

## 1. Corrections to `BIG_TEXTURE_FORMAT.md` (this work supersedes §4.1/§6)

Two claims in the earlier doc are now **corrected**, validated on **227/230**
random retail entries (150 textures.big + 80 frontend.big, exact byte
accounting `consumed == entry size`):

1. **Only mip 0 is LZO-compressed. Mips 1..n-1 are stored RAW**, tightly
   concatenated after the mip-0 region — they are *not* per-mip length-prefixed
   blocks. (The "subsequent blocks" seen before were additional *chunks* of the
   mip-0 region / misread raw data.) Source: EgoCore `TextureParser.h:289`
   `isCompressed = (m == 0 && Header.MipSize0 > 0)`; confirmed byte-exact on
   retail.
2. **The §6 stored-block question is RESOLVED**: `Info+24` (`MipSize0`) is the
   switch. `MipSize0 == 0` ⇒ the **whole payload is raw** (no LZO anywhere) —
   this is what EgoCore/ChocolateBox mod-manager emits (`hdr->MipSize0 = 0`)
   and the loaders accept. Nonzero ⇒ it is the on-disk byte size of the mip-0
   compressed region. Additionally `clen == 0` inside the chunk run means a
   *stored chunk*. `tools/texture_build.py replace --raw-mip0` emits the
   all-raw form.
3. Minor: the DXT3 descriptor tail is **`02 08 00 00 00 00`**, not
   `03 04 ...` (that is DXT1-only). 100% consistent in the 230-entry sample.

## 2. The 34-byte subheader (Info) — confirmed field table

Equals EgoCore `CGraphicHeader` (28 B) + `CPixelFormatInit` (6 B), packed LE:

| Off | Type | Field | Writer value |
|----:|------|-------|--------------|
| +0  | u16 | Width (allocated, pow2) | alloc W |
| +2  | u16 | Height (allocated, pow2) | alloc H |
| +4  | u16 | Depth | 0 (2D; readers treat 0 as 1) |
| +6  | u16 | FrameWidth (real/authored) | source image W |
| +8  | u16 | FrameHeight (real/authored) | source image H |
| +10 | u16 | FrameCount | 1 |
| +12 | u32 | PixelFormatIdx | `0x1f` DXT1, `0x20` DXT3, `0x01` A8R8G8B8 |
| +16 | u8  | TransparencyType | DXT1: 0 (retail also uses 2 = punch-through class), DXT3: 1 (rare 4), ARGB: 0/1 |
| +17 | u8  | MipmapLevels | chain length (DXT: down to 4×4; ARGB: to 1×1) |
| +18 | u8  | Flags | 0 (rare 1 in retail; preserved on replace) |
| +19 | u8  | Padding | 0 |
| +20 | u32 | FrameDataSize | **uncompressed** byte size of mip 0 |
| +24 | u32 | MipSize0 | **on-disk** size of mip-0 compressed region (chunk headers + chunk bytes + 3 raw tail bytes); 0 = payload all-raw |
| +28 | 6B  | CPixelFormatInit {Type, ColourDepth, R,G,B,A bits} | DXT1 `03 04 00 00 00 00`; DXT3 `02 08 00 00 00 00`; ARGB `01 20 08 08 08 08` |

GUI sprites can have `FrameDataSize` > computed mip-0 size (frame-stride
padding, e.g. `EDITORGUI_BACKUP_ICON`) and `FrameCount` 4/16 — see gaps.

## 3. Payload recipe

```
payload := for each frame (writer emits 1):
             mip0_region        ; iff MipSize0 > 0, else raw mip0
             raw mip1 … raw mip(n-1)     ; no headers, no compression

mip0_region := chunks… + 3 raw tail bytes, where the chunks LZO1X-encode
               bytes [0 .. rawlen-3) of the mip:
    chunk := u16 clen              ; 0xFFFF -> u32 clen escape follows
             clen bytes LZO1X-1    ; clen == 0 -> stored chunk (raw copy)
```

- Retail and this writer emit mip 0 as a **single** chunk
  (`lionhead_lz_compress.compress_fable_block`); the u32 escape covers
  clen ≥ 0xFFFF (SilverChest switches at 0x7FFF — both forms are read fine).
- Pixel data is linear PC D3D9: DXT1 8 B / DXT3 16 B per 4×4 block,
  A8R8G8B8 stored as BGRA bytes.
- Container-side: the payload is placed by `tools/big_write.py rebuild`
  (align 2048); the 34-byte Info lives in the TOC record and MUST be patched
  when `FrameDataSize`/`MipSize0`/dims change — `texture_build.replace_entry`
  patches it in place before the rebuild (Info is the record's final field).

## 4. The encoder (`tools/texture_build.py`)

- **DXT1/DXT3 block encoder**: numpy-vectorized stb_dxt-style — PCA principal
  axis per block (power iteration), endpoints from extreme projections,
  RGB565 quantization with the decoder's expansion, best-of-3 with two
  least-squares endpoint refinement passes. DXT1 always emits opaque 4-colour
  mode (`c0 > c1` enforced). DXT3 alpha is rounded 4-bit (EgoCore truncates).
- **Mip chain**: 2×2 box filter (EgoCore uses stbir bilinear — equivalent for
  the exact ½ case).
- **Non-pow2 input** is Lanczos-resized up to the pow2 alloc surface on
  `build`; on `replace` the image is resized to the existing entry's alloc
  dims and real dims / mip count / TransparencyType / Flags are preserved.

```
python tools/texture_build.py selftest
python tools/texture_build.py build     art.png out --format dxt1 [--mips N] [--dims 512x512]
python tools/texture_build.py replace   src.big out.big ENTRY_NAME art.png [--sub GBANK_MAIN_PC]
                                        [--format dxt1|dxt3|argb8888] [--raw-mip0]
python tools/texture_build.py roundtrip textures.big [--samples N | --entry NAME]
python tools/texture_build.py decode    src.big ENTRY_NAME out.png
```

## 5. Validation detail (2026-07-20, retail Steam TLC)

**Layout probe** (scratch `probe_layout.py`): 150 random textures.big + 80
frontend.big entries walked with exact byte accounting — **227/230 OK**
(`MipSize0` == walked mip-0 region size, payload consumed exactly). The 3
failures are multi-frame sprites (`FrameCount` 4/16: `EDITORGUI_BACKUP_ICON`,
`SPRITE_DUST_01`, `SPRITE_CAMPFIRE_01`) — see gaps.

**Identity oracle** (`roundtrip --samples 10`): decode retail base mip →
re-encode full entry → decode. DXT re-encode of already-quantized data
reconverges: base-mip PSNR 46.42–66.71 dB (not bit-identical — endpoint
search from decoded pixels need not reproduce the original compressor's
choices; PSNR is the honest metric). Smaller-mip PSNR 25–36 dB against
retail's mips is expected: retail mips were filtered from the original TGA,
ours from the decoded base. **A8R8G8B8 entries round-trip pixel-IDENTICAL on
every mip level** (frontend.big: `UI_SLOT_RIGHT_NODE_OFF_FE` 64×32/4 mips,
`UI_TABLE_VK_SELECT_SPRITE_V_L` 16×16/3 mips) — which also shows retail's mip
filter is the same 2×2 box average this writer uses.

**End-to-end** (scratch `e2e_test.py`): synthetic 512×512 pattern PNG →
`replace` of `TEXTURE_OV_SMITH_TORSO_01` (DXT1, 8 mips) in a COPY of
textures.big → re-decode: 39.47 dB (our reader) / 39.44 dB (`fable_core`
Blender-addon reader, independent code path); container check: all 6,323
other texture entries byte-identical after rebuild.

**SilverChest oracle** (`--texture-import`, FableMod.Gfx): same PNG. Subheader
fields identical (alloc/real dims, frames, `PixelFormatIdx` 0x1f, transp 0,
mips 8, `FrameDataSize` 131072, tail `03 04 …`) except `MipSize0`
(theirs 118034 vs ours 118837 — different LZO output length, both valid).
Framing identical: one chunk + 3 raw tail bytes + 43,688 bytes raw mips 1–7
(exact match to the computed sum). SC's payload decodes to 36.02 dB vs the
source pattern through OUR reader (their encoder is slightly worse than ours).

## 6. Honest gaps

1. **Multi-frame sprites** (`FrameCount` > 1: SPRITE_*, some EDITORGUI GUI
   icons, ~small population) — writer emits `FrameCount=1` only; the reader
   in `texture_build.decode_entry` decodes frame 0 only. Frame-stride padding
   (`FrameDataSize` > computed, e.g. 20×20 GUI icons) unhandled.
2. **Rare formats `0x23` / `0x18`** (5 entries total, likely DXT5-class) —
   not supported; `replace` requires `--format` override for them.
3. **DXT1 1-bit alpha (punch-through)** — encoder always emits opaque
   4-colour mode; retail `TransparencyType == 2` entries would lose cutout
   alpha if replaced with our DXT1 (use DXT3 instead).
4. **In-game visual confirmation not run** (no engine launch in this session).
   The format evidence is three independent readers/writers agreeing
   (retail bytes, EgoCore, SilverChest), but a live retexture screenshot
   remains the final proof.
5. New-entry insertion (vs replacing an existing entry) awaits new-entry
   support in `big_write.py` (owned by another agent). Everything needed on
   the texture side (payload + Info) is already produced by `build`.
