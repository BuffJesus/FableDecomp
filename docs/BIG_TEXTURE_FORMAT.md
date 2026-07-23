# GBANK on-disk texture format (frontend.big / textures.big)

*Authored 2026-07-19 (texture-RE agent). Method: pure Python bytes + community wiki, **NO Ghidra**
(another agent held the Ghidra lock). Container reader reused unmodified from `tools/parse_bigb.py`;
new decoder tool is `tools/parse_texture.py`. Extends — does not modify — the `.big` family docs
(`BIG_FAMILY_FORMATS.md` §5, `BIG_ROUNDTRIP.md` §7).*

## Status at a glance

| Sub-problem | Verdict |
|-------------|---------|
| 34-byte per-entry texture **Info descriptor** (dims / format / mips) | **CONFIRMED** — validated on all 6,324 (textures.big) + 394 (frontend.big) entries |
| **Pixel-format enum** → D3D format map | **CONFIRMED** — `0x01`=A8R8G8B8, `0x1f`=DXT1, `0x20`=DXT3; two rare enums flagged |
| **Payload framing** (per-mip block-length prefix) | **CONFIRMED** — `u16`/`u32` length prefix; Info+24 mirrors block-0 length |
| **Swizzle** (linear vs Xbox/morton) | **CONFIRMED linear** (native PC D3D9; linear S3TC + D3DFMT tail bytes) |
| **Pixel-payload compression codec** | **GAP** — proprietary Lionhead LZ stream; not any standard codec; needs the engine decompressor (Ghidra) to finish |
| **A coherent decoded PNG** | **NOT achieved** — blocked solely by the codec gap; the DXT/ARGB→PNG decoders are written and unit-test-proven, ready to run on any decompressed mip |

The honest bottom line: **everything needed to *interpret* a texture (its size, format, mip layout,
linear layout) is recovered and validated; the last mile is the custom LZ that compresses the pixel
bytes.** Per project loop-prevention, the codec was attacked with 8+ decoder families (flag-bit LZ in
both bit orders, control-byte LZ, word-level LZ, LZ4-sequence, LZSS token grid, raw-deflate, and a
preamble-strip variant) and **none reproduces a decompressed mip**; it is documented here as the gap.

---

## 1. Where texture entries live

Both files are ordinary BIGB containers (see `TEXTBIG_FORMAT.md`). Texture entries are the GBANK
sub-banks:

| File | sub-bank | entries | notes |
|------|----------|--------:|-------|
| frontend.big | `GBANK_FRONT_END_PC` | 394 | GUI backdrops / title / keyboard sprites |
| textures.big | `GBANK_GUI_PC` | 34 | in-game GUI sprites (many non-pow2 alloc) |
| textures.big | `GBANK_MAIN_PC` | 6,290 | the bulk world/character/object textures |

Every entry has `InfoSize == 34` (the texture descriptor) and one `Dep` = the source `.tga` build
path (e.g. `\Dev\BBBProjects\Fable1_5Release\...\frontend\fable_BG_01.tga`).

---

## 2. The 34-byte texture Info descriptor (CONFIRMED)

Little-endian. Offsets validated against every GBANK entry in both files.

| Offset | Type | Field | Evidence |
|-------:|------|-------|----------|
| +0 | u16 | **AllocWidth**  — allocated (padded) width  | power-of-two in **6290/6290** GBANK_MAIN entries |
| +2 | u16 | **AllocHeight** — allocated (padded) height | power-of-two in 6290/6290 |
| +4 | u16 | (0) reserved / depth-hi | always 0 in samples |
| +6 | u16 | **Width**  — real image width  | `Width ≤ AllocWidth` in **every** entry (0 exceptions) |
| +8 | u16 | **Height** — real image height | `Height ≤ AllocHeight` in every entry |
| +10 | u16 | **Depth** (=1 for 2D) | 1 in all samples |
| +12 | u8 | **Format** enum | groups CRC + payload math (see §3) |
| +13 | u8 | (0) | |
| +17 | u8 | **MipCount** | monotonic with max allocated dim (see below) |
| +24 | u32 | **FirstBlockField** | `== payloadBlock0Len + blockHeaderBytes + 3` in **394/394** frontend, **6165/6324** textures (the rest use the `0xffff` u32-length form) |
| +28..+33 | 6 B | **class / D3DFORMAT tail** | `03 04 00 00 00 00` for DXT1/DXT3; `01 20 08 08 08 08` for A8R8G8B8 |

**Proof the alloc/real split is real:** `AllocWidth/Height` are always powers of two and always ≥ the
`Width/Height` pair, e.g. `FRONTEND_BACKDROP_01` = real **640×480** stored in an allocated **1024×512**
D3D texture (a classic non-pow2 image padded up to a pow2 surface). The `Width/Height` pair is the
image the artist authored; the `Alloc*` pair is the GPU surface it is uploaded into.

**MipCount (+17) is the stored mip level count.** It rises with the max allocated dimension and stops
early for textures that don't ship a full chain:

```
maxAllocDim ->  4:1   8:2   16:1-3   32:1-4   64:1-5   128:3-6   256:3-7   512:5-10   1024:8-9
```

The `03 04` (DXT) vs `01 20 08 08 08 08` (ARGB) **tail** bytes are the serialization-class descriptor
that the TOC `CRC` fingerprints (consistent with `BIG_ROUNDTRIP.md` §7: CRC is a per-format-CLASS
stamp — indeed the same CRC value, e.g. `0x3a21ace1`, is shared by both `0x1f` and `0x20` entries that
share a dims/mip class, and `0x4e1ace9f` recurs across frontend.big **and** textures.big).

---

## 3. Pixel-format enum → D3D format (CONFIRMED)

`Info[12]` histogram and the confirmed mapping:

| Enum | Name | Block/bpp | textures.big | frontend.big | Evidence |
|-----:|------|-----------|-------------:|-------------:|----------|
| `0x01` | **D3DFMT_A8R8G8B8** (uncompressed 32bpp) | 4 B/px | 8 | 125 | tail `08 08 08 08` = 8/8/8/8-bit channels; `alloc=real`, no mip padding |
| `0x1f` | **DXT1** (S3TC, no/1-bit alpha) | 8 B / 4×4 | 3,703 | 116 | Info+18 allocbytes == `(w/4)(h/4)·8`; wiki: "DXT1 = no transparency" |
| `0x20` | **DXT3** (S3TC + explicit 4-bit alpha) | 16 B / 4×4 | 2,574 | 153 | allocbytes == `(w/4)(h/4)·16`; wiki: "DXT3 = has transparency" (Fable uses DXT1/DXT3, not DXT5) |
| `0x23` | **[hypothesis]** DXT-class (16 B/block) | 16 B / 4×4 | 4 | — | rare; groups with DXT CRC classes |
| `0x18` | **[hypothesis]** 16 B/block class | 16 B / 4×4 | 1 | — | single entry; unverified |

The block-byte relation is proved by the Info+18/+20 `allocbytes` u32, which equals the **base-level
uncompressed** byte count `(alloc_w/4)·(alloc_h/4)·unit` exactly for DXT entries (e.g. 256×256 DXT1 →
`0x8000` = 32768; 128×64 DXT3 → `0x2000` = 8192). The wiki (fabletlcmod / Fable Explorer) independently
states Fable textures are **DXT1** (no alpha) or **DXT3** (alpha) — matching enums `0x1f`/`0x20`.
Sources: fabletlcmod.com Fable Explorer wiki + Graphics tutorials (cert fails verification, snippets
only — flagged low-reliability, but they agree with the byte evidence).

---

## 4. Payload framing & the compression gap

### 4.1 Framing (CONFIRMED)
Each entry's payload blob is a sequence of **per-mip compressed blocks**, each with a length prefix:

```
block := u16 clen                 ; header 2 bytes, when clen < 0xffff
       | u16 0xffff , u32 clen    ; header 6 bytes, when clen >= 0xffff
       followed by  clen  bytes of compressed stream
```

- **Block 0 = the base (largest) mip.** Its length is mirrored into the Info descriptor:
  `Info[+24] == clen0 + headerBytes + 3` — verified on 394/394 frontend + 6165/6324 textures entries
  (the remaining textures entries use the `0xffff` u32 form, where `Info[+24] == clen0 + 6 + 3`).
- Subsequent blocks are the smaller mips, and the trailing bytes of the payload are zero padding up
  to the sub-bank `Align` (2048). The base block dominates size; smaller mips compress poorly, so the
  post-base bytes are close to (but below) the uncompressed sum of the smaller mips.

### 4.2 The codec is an OPEN GAP
The compressed stream is a **proprietary Lionhead LZ**, not a standard codec:
- Streams begin overwhelmingly with `12 00 ..` / `15 ..` / `19 ..` (small first-byte values 0x12–0x21).
- It is **not** zlib/raw-deflate (no valid inflate at any offset), **not** LZ4-sequence, **not** LZO,
  and does not match any flag-bit LZ (either bit order, 0=lit or 1=lit), control-byte LZ, word-level
  LZ, or LZSS token-grid parameterization tried (offset 11/12/13-bit × len 3/4/5-bit × add 1/2/3 ×
  1/2 offset bytes × both token orders). None reproduces a decompressed mip of the known target size
  while consuming exactly `clen` input bytes.
- No dominant repeat stride in the stream (autocorrelation flat) → genuinely LZ-compressed, not a
  raw/swizzled buffer misread as compressed.

**To close this gap (next agent, Ghidra):** find the GBANK texture-load decompressor in `Fable.exe`
(the routine the loader calls after reading the `u16/u32 clen` block header) and port its inner loop.
The framing above tells you exactly where each compressed block starts and how long it is; only the
inner byte-copy grammar is missing. `tools/parse_texture.py` already reads the Info + framing and has
the DXT1/DXT3/ARGB→RGBA decoders wired and unit-tested, so decode-to-PNG is one function away once the
LZ inner loop is supplied.

---

## 5. Swizzle verdict — LINEAR (CONFIRMED, not Xbox-swizzled)

TLC is a **native PC D3D9** build (project axiom; `ImageBase 0x400000`, ships `msvcr71.dll`). The
descriptor carries a **PC D3DFORMAT class tail** (`03 04` for S3TC, `01 20 08 08 08 08` for
D3DFMT_A8R8G8B8) and standard **linear S3TC 4×4-block** dimensions; the payload is per-mip
length-prefixed compressed blocks, not a morton/Xbox-tiled surface. Xbox-swizzled textures would (a)
not use PC D3DFORMAT descriptors and (b) tile the base surface rather than per-mip LZ-compress it.
Therefore the pixel layout under the compression is **standard linear** DXT1/DXT3/A8R8G8B8 — the
DXT/ARGB decoders in `parse_texture.py` decode it directly once decompressed; **no de-swizzle step is
required.** (This remains formally unproven-by-image only because the codec gap blocks producing the
decompressed buffer; the format-descriptor and framing evidence make linear the strongly-supported
verdict, and Xbox-swizzle is refuted by the PC D3DFORMAT tail.)

---

## 6. Retexture feasibility verdict

- **Reading a texture's identity — GO.** `tools/parse_texture.py` extracts real dims, allocated dims,
  pixel format, mip count, and the source `.tga` name for every one of the ~6,700 texture entries in
  the two banks. That is enough to build a texture browser / catalog now.
- **Round-trip container edit — GO (already proven).** `docs/BIG_WRITER.md` shows replace-entry-payload
  (same-size and size-changing) is byte-exact and isolated, with CRC/Timestamp passthrough. A retexture
  that keeps the **same dims + format** needs no Info edit and rides the existing writer.
- **Authoring new pixels — BLOCKED on the codec.** To write a new texture you must (a) DXT-compress the
  new image (trivial, standard S3TC) and (b) **re-compress it with the game's LZ** into the per-mip
  block framing of §4.1. Step (b) is the open gap. Until the LZ is recovered, a retexture pipeline can
  only swap in payloads that are already in the game's compressed form (i.e. copy an existing entry's
  bytes), not encode arbitrary new art.
- **Interim workaround worth checking (next agent):** the engine loader may also accept an
  **uncompressed** block (some Lionhead loaders treat `clen == rawMipSize` as a stored/literal block).
  If confirmed in Ghidra, a writer could emit uncompressed DXT mips and skip the LZ encoder entirely —
  the cheapest path to real retexturing.

---

## 7. Deliverables & reproduce

- `tools/parse_texture.py` — Info-descriptor parser + payload framing walker + DXT1/DXT3/A8R8G8B8
  linear decoders (unit-tested: synthetic DXT1/DXT3/ARGB blocks decode to the exact expected RGBA).
  ```
  python tools/parse_texture.py <frontend.big|textures.big>            # list entries + Info
  python tools/parse_texture.py <file.big> --entry <id>                # one descriptor + framing
  python tools/parse_texture.py <file.big> --png  <id> <out.png>       # decode (needs decompressed buf)
  ```
- Info-descriptor validation over all entries: reproduced by `scratchpad/validate_info.py` (session).
- **PNG evidence NOT produced** — see §4.2. The proof-by-coherent-image step is deferred to when the
  §4.2 codec is recovered; the decode half that turns a decompressed mip into a PNG is written and
  proven correct in isolation (§7 unit tests), so it will produce the image immediately at that point.

## 8. Open gaps (for HANDOFF)
1. **Pixel-payload LZ codec** (the one real blocker) — recover from the `Fable.exe` GBANK texture
   decompressor via Ghidra; framing/block boundaries are already known (§4.1).
2. Confirm `0x23` / `0x18` rare enums (likely DXT5 / another 16-B/block class) — 5 entries total.
3. Confirm whether the loader accepts uncompressed (stored) blocks (§6 workaround).
