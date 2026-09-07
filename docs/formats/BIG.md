# .big (BIGB) bank archive — container, round-trip gate, writer

*Merged reference (2026-09-07) of `BIG_FAMILY_FORMATS.md` (2026-07-19), `BIG_ROUNDTRIP.md`
(2026-07-19), `BIG_WRITER.md` (2026-07-19), the container-level parts of `BIG_TEXTURE_FORMAT.md`
(2026-07-19) and the `big_write.rebuild(adds=)` design from `MESH_COMPOSE.md` (2026-07-20).
Texture-specific content (34-byte Info descriptor, pixel-format enum, swizzle, retexture writer)
stays in [BIG_TEXTURE_FORMAT.md](BIG_TEXTURE_FORMAT.md) / [TEXTURE_WRITER.md](TEXTURE_WRITER.md);
mesh payloads are in [MESH.md](MESH.md); the LZO1X codec itself is in [LIONHEAD_LZ.md](LIONHEAD_LZ.md);
the text.big BIGB spec this doc extends is [TEXTBIG_FORMAT.md](TEXTBIG_FORMAT.md).*

**Status:** the BIGB reader is VALIDATED across all 8 retail `.big` files (byte-clean parse);
byte-exact read→model→write round-trip is PROVEN on 8/8 (SHA-256 equality, up to the 533 MB
`textures.big`); a controlled replace-entry-payload writer (same-size AND size-changing) is GO on
real retail files; new-TOC-entry appends (`big_write.rebuild(adds=)`) load in-engine.

Tools (pure Python, no Ghidra): `tools/parse_bigb.py` (reader, ported from the text.big spec),
`tools/bigb_roundtrip.py` (round-trip harness; reuses the reader, additive `_augment_entry_raw`
only), `tools/verify_big_crc.py` (CRC matrix), `tools/bigb_write.py` (replace-entry-payload
writer; reuses `parse_bigb.py` + `bigb_roundtrip.py` model helpers unmodified),
`tools/big_write.py` (`rebuild(buf, edits, adds=)` — new-entry writer used by the mesh composer).

Fields whose *purpose* is inferred, not proven, are labeled **[hypothesis]**.

## Evidence / sources

1. **Raw retail bytes** of all 8 `.big` files under
   `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\` — full walking parse of every file.
2. **`TEXTBIG_FORMAT.md`** — the text.big BIGB spec (triangulated against EgoCore's completed-RE
   loader and the retail `Fable.exe` engine loader). Authoritative container reference.
3. **fabletlcmod.com wiki** (community source) — confirms structurally that `.big` archives are
   **banks** each containing **entries**, each entry type in its own payload format,
   edited/recompiled via *Fable Explorer*. The wiki's HTTPS cert fails verification so byte-layout
   pages could not be fetched directly; its user-level docs do not contradict the byte evidence.
4. **EgoCore** (`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`) — `BigBankCompiler`
   (record order), `Utils.h DecompressLZO` (chunk framing), `Banks/BankEditor.h`.

---

## 1. File inventory (all located, all sized)

| File | Path (under Steam install) | Size (bytes) | subBanks | Verdict |
|------|----------------------------|-------------:|:--------:|:-------:|
| `text.big` | `data\lang\English\` | 6,652,561 | 1 | CLEAN |
| `dialogue.big` | `data\lang\English\` | 17,160,084 | 4 | CLEAN |
| `fonts.big` | `data\lang\English\` | 53,822,286 | 3 | CLEAN |
| `frontend.big` | `data\graphics\pc\` | 13,794,357 | 1 | CLEAN |
| `graphics.big` | `data\graphics\` | 243,841,923 | 2 | CLEAN |
| `textures.big` | `data\graphics\pc\` | 533,633,077 | 2 | CLEAN |
| `effects.big` | `data\Misc\pc\` | 3,500,103 | 1 | CLEAN |
| `shaders.big` | `data\shaders\pc\` | 453,341 | 26 | CLEAN |

**8/8 parse clean under the unmodified BIGB reader.** Every file: `Magic=="BIGB"`, `Version==100`,
footer in range, footer directory decodes, and the sub-bank TOC chain terminates **exactly** at
`FooterOffset` (§3). No file deviates from the format.

Also present (audio companions, NOT BIGB): `Dialogue.lut` (268 MB), `Dialogue2.lut` (37 KB),
`ScriptDialogue.lut` (229 MB), `ScriptDialogue2.lut` (175 MB) — `LiOnHeAdLHAudioBankCompData`
audio banks (magic at offset 0), referenced by the LIPSYNC entries (§12). See
[LUT_AUDIOBANK_FORMAT.md](LUT_AUDIOBANK_FORMAT.md).

---

## 2. Container layout

Re-verified on every file (per `TEXTBIG_FORMAT.md`):

- **Header (16 B @ 0):** `char[4] Magic="BIGB"`, `u32 Version=100`, `u32 FooterOffset`,
  `u32 FooterSize`.
- **Footer (sub-bank directory) at `FooterOffset`:** `u32 SubBankCount`, then per sub-bank:
  NUL-terminated `Name`, `u32 Version`, `u32 EntryCount`, `u32 Offset` (TOC start), `u32 Size`
  (TOC byte size), `u32 Align` (payload alignment).
- **Per sub-bank TOC at `Offset`:** optional stats header (`u32 StatsCount<1000` then
  `StatsCount×2` u32 `(type,count)` pairs; else rewind), then `EntryCount` variable-length entry
  records:

  | TOC rec offset | Field |
  |---:|---|
  | +0x00 | `u32 Magic` (=42) |
  | +0x04 | `u32 ID` |
  | +0x08 | `i32 Type` |
  | +0x0C | `u32 Size` |
  | +0x10 | `u32 Offset` |
  | +0x14 | `u32 CRC` |
  | +0x18 | `u32 nameLen + name`, then `u32 Timestamp` (quoted "TOC +0x18" in the round-trip audit), `u32 DepCount`, `DepCount × (u32 len + str)` Deps, `u32 InfoSize`, `InfoSize` bytes `Info` |
- **Payload blobs** live between `0x10` and `FooterOffset`, each at its entry's `Offset`, `Size` long.

Field-semantics refinements confirmed by the wider family:
- **`Deps`** = the entry's original **source-asset build path(s)**, e.g.
  `\Dev\BBBProjects\Fable1_5Release\Fable\Resources\Art3\GUI\frontend\fable_BG_01.tga` (frontend),
  `...\FableEngine\shaders\pixel\diffuse_colour.psh` (shaders), a `*.bbm` path (MBANK). For LIPSYNC it
  holds the **speaker tag** (`SPEAKER_FEMALE1`). The payload is build provenance / associated symbol.
- **`Info`** = a small per-entry subheader whose meaning is bank-kind-specific: a texture descriptor
  in GBANK banks (34 bytes: dims + format — `BIG_TEXTURE_FORMAT.md` §2), a mesh descriptor in MBANK
  (24–176 bytes — `MESH.md`), a float (clip duration) in LIPSYNC (4 bytes), 4 bytes in text.big.
  **`InfoSize` is per-entry** (not a constant): always read `InfoSize` then that many bytes.
- **Name / Dep string byte-fidelity:** `parse_bigb` strips the trailing NUL when decoding
  length-prefixed strings. On disk the length prefix **includes** the trailing NUL where present
  (retail records — do not assume "no NUL"); records synthesized by
  `big_write.rebuild(adds=)` store the exact name length with NO trailing NUL, and the engine loads
  them. A writer that edits a name must recompute the prefix to match the game's convention — never
  re-encode from a stripped string (the harness re-reads the raw `u32 len + raw bytes` field via
  `_augment_entry_raw` / `_raw_lpstr_at`; `bigb_write.py` carries `name_raw` / `deps_raw`).

---

## 3. Per-file parse report and bank kinds

Validation invariant: **each sub-bank's TOC ends exactly where the next sub-bank's TOC begins, and
the last sub-bank's TOC ends exactly at `FooterOffset`** (text.big is the single-sub-bank case where
TOC-end == `FooterOffset` directly). Held for all 8 files with zero slack.

| File | sub-bank | ver | entries | stats-header (type:count) | type histogram |
|------|----------|----:|--------:|---------------------------|----------------|
| text.big | TEXT_ENGLISH_MAIN | 106 | 28,913 | (0:26807)(1:2105)(2:1) | {0:26807, 1:2105, 2:1} |
| dialogue.big | LIPSYNC_ENGLISH_MAIN | 112 | 12,134 | (1:12134) | {1:12134} |
| " | LIPSYNC_ENGLISH_MAIN_2 | 112 | 1 | (1:1) | {1:1} |
| " | LIPSYNC_ENGLISH_SCRIPT | 112 | 5,310 | (1:5310) | {1:5310} |
| " | LIPSYNC_ENGLISH_SCRIPT_2 | 112 | 3,060 | (1:3060) | {1:3060} |
| fonts.big | FONT_ENGLISH_MAIN | 105 | 8 | (0:8) | {0:8} |
| " | STREAMING_FONT_ENGLISH_PC | 109 | 9 | (0:8)(2:1) | {0:8, 2:1} |
| " | STREAMING_FONT_ENGLISH_XBOX | 109 | 9 | (1:8)(2:1) | {1:8, 2:1} |
| frontend.big | GBANK_FRONT_END_PC | 122 | 394 | (0:394) | {0:394} |
| graphics.big | MBANK_ALLMESHES | 205 | 8,112 | (1:2583)(2:55)(3:1383)(4:152)(5:504)(6:3272)(7:56)(9:107) | matches |
| " | MBANK_ENGINE | 205 | 1 | (1:1) | {1:1} |
| textures.big | GBANK_GUI_PC | 122 | 34 | (0:2)(1:32) | {0:2, 1:32} |
| " | GBANK_MAIN_PC | 122 | 6,290 | (0:5879)(1:5)(2:372)(4:4)(5:30) | matches |
| effects.big | PARTICLE_MAIN_PC | 111 | 1,165 | (0:1165) | {0:1165} |
| shaders.big | 26 sub-banks (PIXEL_SHADERS, SHADERS_*, VERTEX_FORMAT_SHADERS) | 107 | 1–112 each | matches per sub-bank | mixed {0,1} |

In every row the parsed entry count equals `EntryCount` and the stats-header sums equal `EntryCount`.
The sub-bank `Name` prefix selects bank kind exactly as the spec's `ResolveBankType` says:
`TEXT_*`→text, `LIPSYNC_*`→dialogue/speech, `FONT_*`/`STREAMING_FONT_*`→fonts,
`GBANK_*`→GUI/texture graphics, `MBANK_*`→meshes, `PARTICLE_*`→effects, `*SHADER*`→shaders.

- `parse_bigb.py`'s CLI prints `DEVIATES` for MBANK_ALLMESHES even on the UNMODIFIED retail
  graphics.big — its "TOC end == footerOffset" invariant only holds for the LAST subbank of a
  multi-subbank file; both TOCs parse 100 % of entries either way (2026-07-20).

---

## 4. Entry types (per bank kind)

**Type is per-bank-kind, not global.** The generic reader treats `Type` opaquely and still walks
the TOC perfectly — only the *payload decoder* is kind-specific.

| Bank kind (prefix) | Type values | Meaning | Info | Payload doc |
|---|---|---|---|---|
| `TEXT_*` | 0 / 1 / 2 | string / group / narrator | 4 B | [TEXTBIG_FORMAT.md](TEXTBIG_FORMAT.md) |
| `LIPSYNC_*` | 1 | lipsync clip (viseme-weight keyframes) | 4 B float duration (0 for `Size=0` empties) | §12 below — **fully cracked** |
| `FONT_*` / `STREAMING_FONT_*` | 0 / 1 / 2 | font glyph banks (PC=0, XBOX=1, 2 = one shared entry per streaming bank) | — | not decoded here; glyph payload is `lzo1x_decompress_safe` @ `0x00c08170` |
| `GBANK_*` | 0 / 1 / 2 / 4 / 5 | texture entries (sub-formats) | 34 B texture descriptor + source `.tga` Dep | [BIG_TEXTURE_FORMAT.md](BIG_TEXTURE_FORMAT.md) |
| `MBANK_*` | 1 / 2 / 4 / 5 compiled meshes; 3 tagged BBM (`3DMF`, physics); 6 / 7 / 9 animations | 1 static, 2 instanced, 3 physics, 4 particle, 5 skinned | 60–176 B mesh descriptor (var); 0 for type 3; 24 B for anims | [MESH.md](MESH.md), [BIG_ANIM_FORMAT.md](BIG_ANIM_FORMAT.md) |
| `PARTICLE_*` | 0 | particle-system entries by name, no Info/Deps | 0 | [EFFECTS_FORMAT.md](EFFECTS_FORMAT.md) |
| `*SHADER*` | 0 / 1 | compiled shader bytecode + source `.psh`/`.vsh` Dep | — | not decoded |

Where texture entries live: `frontend.big/GBANK_FRONT_END_PC` (394; GUI backdrops / title /
keyboard sprites), `textures.big/GBANK_GUI_PC` (34; in-game GUI sprites, many non-pow2 alloc),
`textures.big/GBANK_MAIN_PC` (6,290; the bulk world/character/object textures). Every GBANK entry
has `InfoSize == 34` and one `Dep` = the source `.tga` build path.

**Generalization verdict:** the same 16-byte header + footer directory + chained sub-bank TOCs
describe all 8 files; the generic TOC record parses every entry in every file (72k+ entries total).
What differs per file is only the payload decoder, selected by sub-bank `Name` prefix + `Type`.

---

## 5. On-disk region layout — TOCs and payloads are INTERLEAVED

A `.big` is NOT header + one contiguous payload block + TOC block. In `shaders.big` the per-sub-bank
TOCs and the entry payloads are **interleaved in address order** (payloads for one sub-bank can sit
before, between, and after other sub-banks' TOCs). Verified layout:

```
shaders.big: 493 byte-regions in address order, 416 zero-pad gaps (898 B total), 0 overlaps.
0x0000..0x0010 header | 0x0010.. payloads (subbank 5) | ... | 0x5a84c.. TOC blocks | 0x6e6c2 footer
```

Region kinds (address-ordered): `header` (16 B @ 0) · `toc` (one per sub-bank) · `payload` (one per
non-empty entry) · `footer` (sub-bank directory). The footer follows the last TOC contiguously
(verified: no gap before the footer in any file). Therefore the only universally-correct re-layout
is **address-order re-flow** (§9.1): keep every region in its original on-disk order; when one
payload changes size, shift the regions that follow it and re-establish alignment. No contiguous
payload region is assumed and nothing is re-ordered.

**Alignment padding:** every byte not claimed by a structural region is inter-payload gap padding and
in all 8 files it is **100 % zero** (e.g. textures.big = 6,771,202 zero bytes across 6,317 gaps
between the 2048-aligned payloads). Sub-bank `Align` is real and load-bearing: `2048` for
GBANK/MBANK/PARTICLE/FONT_MAIN (`0x800` payload boundaries), `512` for STREAMING_FONT (fonts.big mixes
512/2048), `4` for shaders, `1` for text/dialogue (packed).

---

## 6. LZO chunk framing (shared by texture mip-0, mesh buffers, save data)

Payload blobs that are compressed use one **Fable chunked-LZO** frame (EgoCore `Utils.h
DecompressLZO`; engine writer `FUN_00996610` @ `0x00996610`):

```
block := u16 clen                 ; header 2 bytes, when clen < 0xffff
       | u16 0xffff , u32 clen    ; header 6 bytes, when clen >= 0xffff
       followed by  clen  bytes of LZO1X stream      (clen == 0 -> STORED chunk, plain bytes)
run of blocks fills (size - 3) output bytes, then 3 trailing PLAIN bytes
empty buffer == a lone u16 0
```

- **Codec = LZO1X**: `lzo1x_decompress` @ `0x00c06b90` (safe variant `lzo1x_decompress_safe` @
  `0x00c08170`); Python port `tools/lionhead_lz.py::decompress(src, expected_len=None)`; compressor
  `lionhead_lz_compress.compress_fable_block` (`lzo1x_999_compress` on the engine side). Full
  algorithm + proof in [LIONHEAD_LZ.md](LIONHEAD_LZ.md). Zlib (`z_inflate` @ `0x00c02470`,
  `CCompressorZlib` @ `0x00a3a840`..`0x00a3abe0`) is used for definition data
  (`CDefinitionManager::DecompressDefinitionData` @ `0x009ad5f0`) and the install-time file expander
  (`CFileInstaller::ProcessActiveFileForDecompression` @ `0x009c3c20`, ≤0x40000-byte chunks), never
  for `.big` payloads.
- **Engine writer:** `FUN_00996610` calls `_lzo1x_999_compress(src, size-3, ...)` then immediately
  `_lzo1x_decompress(...)` to verify the roundtrip of the block it just made (call site
  `0x009966e5`) — the `size-3` + trailing-3 framing above. Its callers are the SAVE/serialize side
  (`SaveGameState`@`0x4b64e0`, `TransferBinaryOut`@`0x6ba030`, `SaveBinary`@`0xa2eaa0`/`0xa75400`/
  `0xa89450`/`0xa8fd40`/`0xac3b00`). `lzo1x_decompress_safe` callers are the streaming-font bank
  (`CStreamingFontBank` ctor @`0x9a3c20` via `AcquireFont`@`0xa5f1c0`, `GlyphLookup`@`0x9a1140`,
  `FindGlyph`@`0x9a4160`).
- **Textures (GBANK):** payload = per-mip blocks; **block 0 = the base mip** and its length is
  mirrored into the Info descriptor: `Info[+24] == clen0 + headerBytes + 3` (394/394 frontend,
  6165/6324 textures; the remainder use the `0xffff` u32 form where `Info[+24] == clen0 + 6 + 3`).
  Trailing payload bytes are zero padding to the sub-bank `Align` (2048). Later refinement: only MIP 0
  is chunked-LZO, mips 1..n-1 are stored RAW, `Info+24 == 0` = all-raw payload (loader-accepted) —
  see [BIG_TEXTURE_FORMAT.md](BIG_TEXTURE_FORMAT.md) / [TEXTURE_WRITER.md](TEXTURE_WRITER.md).
- **Meshes (MBANK):** each primitive's vertex buffer and index buffer, and the bone-name / bone /
  TRS / IBM / helper / dummy / packed-name / plane blocks, are chunked-LZO; header/stats are plain.
  Measured ratios prove real compression (seagull prim0 VB `out=5080, clen=2940` = 58 %; bear prim0
  `out=35796, clen=28416` = 79 %; only sub-20-byte blocks are stored). Details + the engine
  load-path debate in [MESH.md](MESH.md#4-compression--on-disk-lzo-vs-the-engine-load-path).
- **Anims (3DAF):** a different frame — `u32 decompSize` + ONE raw LZO1X stream (no `[u16 clen]`
  chunks) — see [BIG_ANIM_FORMAT.md](BIG_ANIM_FORMAT.md).
- Pre-codec history (2026-07-19, texture agent, no Ghidra): 8+ decoder families were tried (flag-bit
  LZ both bit orders, control-byte LZ, word-level LZ, LZ4-sequence, LZSS token grid 11/12/13-bit ×
  3/4/5-bit × add 1/2/3 × 1/2 offset bytes × both token orders, raw-deflate, preamble-strip); streams
  begin with small first bytes `0x12–0x21` (`12 00 ..` / `15 ..` / `19 ..`); autocorrelation flat.
  Resolved the same day by the Ghidra decompile (LZO1X).

---

## 7. Round-trip verification gate — PASSED 8/8

Mandatory writer gate (flagged in the family doc and `TEXTBIG_FORMAT.md` §7): prove a read →
in-memory model → RE-SERIALIZE cycle reproduces an unmodified input byte-identically. Each PASS is an
actual `output == original` byte-equality check plus a SHA-256 match — not a structural re-parse.
`passthru` = bytes NOT rebuilt from parsed fields but copied verbatim; all of it is alignment padding
and all of it is zero.

| File | Status | Size (bytes) | subBanks | entries | Passthrough (all zero-pad) | SHA-256 (in==out) |
|------|:------:|-------------:|:--------:|--------:|---------------------------:|-------------------|
| text.big | **PASS** | 6,652,561 | 1 | 28,913 | 0 | (in==out) |
| dialogue.big | **PASS** | 17,160,084 | 4 | 20,505 | 0 | `70019d1f…` |
| fonts.big | **PASS** | 53,822,286 | 3 | 26 | 24,574 | `b9cd0262…` |
| frontend.big | **PASS** | 13,794,357 | 1 | 394 | 430,122 | `173a4177…` |
| graphics.big | **PASS** | 243,841,923 | 2 | 8,113 | 8,484,190 | `8b909c40…` |
| textures.big | **PASS** | 533,633,077 | 2 | 6,324 | 6,771,202 | `39ef8351…` |
| effects.big | **PASS** | 3,500,103 | 1 | 1,165 | 1,167,973 | `dcd13595…` |
| shaders.big | **PASS** | 453,341 | 26 | 465 | 898 | `ee80cd34…` |

**8/8 byte-exact. Zero FAILs. Zero non-zero passthrough bytes anywhere** — there is no
opaque/unknown region the model fails to explain; structural coverage + zero padding accounts for
100 % of every file.

### 7.1 Contract — RECONSTRUCTED vs PASSED THROUGH

RECONSTRUCTED from the model (a mismatch is a serializer bug):
- **Header (16 B @ 0):** `Magic`, `Version`, `FooterOffset`, `FooterSize` (the header back-patch).
- **Footer @ `FooterOffset`:** `SubBankCount`, then per sub-bank `Name+NUL`, `Version`,
  `EntryCount`, `Offset`, `Size`, `Align`. The writer must keep TOC[i].end == TOC[i+1].start and
  last == `FooterOffset`.
- **Per-sub-bank TOC:** the optional stats header (emitted iff `StatsCount<1000`, EgoCore's rewind
  rule) **plus every entry record**: `Magic, ID, Type, Size, Offset, CRC` (6×u32), the
  length-prefixed `Name`, `Timestamp`, `DepCount`, each length-prefixed `Dep`, `InfoSize`, `Info`.
- **Payload blobs:** each entry's `[Offset, Offset+Size)` slice, held in the model and re-placed.

PASSED THROUGH verbatim: **alignment padding only** (100 % zero — VERIFIED zero, not assumed). The
harness does not re-derive padding from `Align`; a real writer that re-packs payloads MUST pad each
payload up to its sub-bank `Align` with zero fill, which reproduces exactly what is observed.

### 7.2 Single-edit smoke test (controllability proof)

To prove the serializer is model-driven, not an identity `cp`, the harness edits ONE byte of ONE
payload and confirms ONLY that byte changes (shaders.big, smallest payload picked automatically):

```
=== SMOKE TEST: shaders.big ===
  target payload: subbank[0] id=1 name='PSHADER_DIFFUSE_ONLY' size=29 at 0x5733c
  edited byte at 0x57358: 0x00 -> 0xff
  total differing byte-runs after re-serialize: 1
    diff 0x57358..0x57359 (1B)
  VERDICT: CONTROLLABLE (only the intended byte changed)
```

---

## 8. Align / CRC / Timestamp — the integrity fields

Field audit across all 8 files (offsets per `TEXTBIG_FORMAT.md` §3b):

| File | Sub-bank `Align` values | TOC `CRC` non-zero | TOC `Timestamp` non-zero | sample CRC |
|------|------------------------|:------------------:|:------------------------:|-----------|
| text.big | 1 | 0 / 28,913 | 0 | — |
| dialogue.big | 1 | 0 / 20,505 | 0 | — |
| fonts.big | 512, 2048 | 26 / 26 | 0 | `0xad673819` |
| frontend.big | 2048 | 394 / 394 | 394 | `0x5701ffde` |
| graphics.big | 2048 | 8,046 / 8,113 | 8,113 | `0xae689191` |
| textures.big | 2048 | 6,324 / 6,324 | 6,324 | `0x4e1ace9f` |
| effects.big | 2048 | 0 / 1,165 | 0 | — |
| shaders.big | 4 | 465 / 465 | 465 | `0xb8bc6765` |

### 8.1 CRC (TOC +0x14) — NOT a payload hash; a per-format CLASS fingerprint

The earlier "observed 0x00000000" note held only for text/dialogue/effects. `tools/verify_big_crc.py`
tested the strong hypothesis (TOC CRC = seed-0 reflected CRC-32 `CCRC::Calc`, poly `0xEDB88320`,
zlib table, no final inversion — the canonical `game.bin`/`text.big` CRC; algorithm re-confirmed
against `ghidra_out/decomp_ccrc_calc.log` + `crc_table_full.txt`, table[1]==`0x77073096`) over 30
real non-zero-CRC entries from 5 files (shaders, frontend, fonts, graphics, textures):
**60 formulas** = 12 data domains × {seed 0, seed 0xFFFFFFFF} × {no-inversion, final-inversion},
plus a `zlib.crc32` control. Domains: `payload`, `name`, `name+NUL`, `subbank name` (+NUL), `dep`
build-path (+NUL), `Info`, and `name+payload`, `payload+info`, `name+info`, `info+payload`.

**Result: REFUTED — no formula matched even ONE of the 30 samples** (0/30 everywhere, control
included). Reproduce: `python tools/verify_big_crc.py`.

What the CRC actually is — constant across all entries that share a resource FORMAT / serialization
CLASS:

| File | entries (non-zero CRC) | **distinct** CRC values | note |
|------|-----------------------:|------------------------:|------|
| shaders.big | 465 | **1** (`0xb8bc6765`) | one value for ALL 26 sub-banks / every shader |
| frontend.big | 394 | **3** | `0x5701ffde`, `0x4fd8a4a8`, `0x4e1ace9f` |
| textures.big | 6,324 | **29** | groups by texture format |
| fonts.big | 26 | 7 | groups by font-glyph format |
| graphics.big | 8,046 | 1,289 | meshes: many vertex/format classes |

Decisive tells:
- All 465 shaders share ONE CRC despite wildly different bytecode payloads, names, and sizes.
- Within a group payload, name, size and the dimension bytes of `Info` all vary; only the `Info`
  **format** descriptor (texture pixel-format / mesh vertex-format constant bytes) is shared. For
  textures the `03 04` (DXT) vs `01 20 08 08 08 08` (ARGB) Info tail is the class descriptor the CRC
  fingerprints; the same CRC (e.g. `0x3a21ace1`) is shared by both `0x1f` and `0x20` entries that
  share a dims/mip class.
- The value **recurs across files**: `0x4e1ace9f` appears in BOTH `frontend.big` and `textures.big`
  (same GUI texture format in two banks) — impossible for a per-file/per-payload hash.
- graphics.big CRCs correlate with mesh class (`0x73ab9c0d` covers only types {6,7,9} with
  `InfoSize=24`; `0xae689191` covers types {1–5} with variable `InfoSize`) and with the per-submesh
  `sVert` code set (`MESH.md`).

It is almost certainly `CCRC::Calc` of the resource **class's field/type descriptor** (a build-time
schema string not present in the `.big` bytes) — an EgoCore-style "resource-class version CRC"
stamped at bank-build time so the loader can reject format-version drift.

**Writer consequence:** an edited payload of an existing format legitimately keeps the SAME CRC as
its unedited siblings — **pass it through verbatim; recomputing it is wrong and unnecessary** for
retexture / mesh-swap / shader-swap. Only INTRODUCING a new resource format/class would need a fresh
stamp (build-time class-schema string + `CCRC::Calc(?, schema)`) — out of scope; a Ghidra trace of
who WRITES `CRC` at bank-build would pin the schema source. New entries appended by
`big_write.rebuild(adds=)` ship with `crc=0` (EgoCore does the same) and the engine loads them.

### 8.2 Timestamp — opaque, NOT wall-clock

High-entropy, uniformly spread across the full 32-bit range (frontend: min `24,156,209` … max
`4,258,377,908`; adjacent entries decode as unix years 1985 / 2060 / 2093 — nonsensical). Non-zero
in the graphics-family files. Treat as an opaque per-entry value: **passthrough verbatim; never
regenerate / stamp "now"**. New entries use `timestamp=0`.

---

## 9. Writer A — `tools/bigb_write.py` replace-entry-payload (in-place, whole-file byte-exact)

Swaps ONE entry's payload bytes for new bytes (texture pixel data / shader bytecode / mesh blob),
re-lays all offsets, re-pads to alignment, preserves TOC CRC/Timestamp verbatim, rewrites
header/footer. Handles **same size** (no downstream movement) and **different size** (downstream
regions ripple). Every write is validated by re-reading the output through `parse_bigb.py`.

| Demo | File | Edit | Result |
|------|------|------|--------|
| 0 identity | all 8 | none | **PASS** — byte-exact (SHA match) round-trip on every file |
| 1 same-size | shaders.big | replace 41-B shader bytecode, same length | **PASS** — exactly 1 byte-diff run of 41 B inside the payload; 464 other entries identical |
| 2 size-change (Align=4) | shaders.big | grow a 29-B shader to 93 B (+64) | **PASS** — file +256 B, 111 downstream Offsets re-flowed, only that entry changed on re-read |
| 3 size-change (Align=2048) | frontend.big | grow a GBANK texture by 3000 B | **PASS** — file +2048 B, 310 downstream Offsets shifted, 0 Align-2048 violations |
| 4 invertibility | shaders.big | grow +64 then revert to original bytes | **PASS** — reverted file is byte-exact the untouched original (writer is lossless) |
| (extra) fonts mixed-align | fonts.big | grow an Align=512 payload +700 B | **PASS** — 0 per-sub-bank Align violations; only that entry changed |

In every edited case all OTHER entries' `Name/Type/CRC/Timestamp/Deps/Info/Size` and payload bytes
are byte-identical on re-read; the edited entry reads back the EXACT new bytes with `CRC`/`Timestamp`
preserved.

### 9.1 Offset re-layout rule — MINIMAL REPACK

Walk all regions in on-disk address order with a running output `cursor`:

```
new_start = snap_up(cursor, B)      where  B = orig_boundary(orig_start, cap)
```

- `orig_boundary(x, cap)` = the largest power of two ≤ `cap` that divides the region's **original**
  start `x` — the boundary the bank builder actually placed the region on.
- `cap` = the **MAX sub-bank Align across the whole file** (4 for shaders.big, 2048 for
  frontend/effects/graphics/textures, 2048 for fonts.big which mixes 512/2048, 1 for text/dialogue).
  Header and footer use `cap = 1`.
- **NO "preserve the original gap" term** — padding is exactly the minimum zero fill to reach `B`.

Why this exact rule (all three verified on all 8 retail files): **identity** (pristine regions
already sit at their boundary → `snap_up` no-op → `identity mismatches = 0 / 8`); **size-change**
(downstream regions ripple by the minimum needed to stay aligned; shrink reclaims freed space);
**invertible** (capping at the file's max Align — not a coincidental large divisor — makes
edit-then-revert byte-exact, Demo 4). Capping at *per-sub-bank* Align breaks fonts.big identity
(some Align=512 payloads sit on a 2048 boundary); capping at a fixed 2048 breaks invertibility on
Align=4 shaders.big. **The max-file-Align cap is the one rule that satisfies both.**

### 9.2 Fields RE-PATCHED from the new layout

- **Entry `Offset` (TOC +0x10):** each payload's new start.
- **Entry `Size` (TOC +0x0C):** changed **only for the edited entry**, to `len(new_payload)`.
- **Sub-bank `Offset` / `Size` (footer):** each TOC's new start and byte length. TOC byte length is
  invariant to payload-offset *values* (fixed-width u32), so a payload size change never changes a
  TOC's length.
- **Header `FooterOffset` / `FooterSize` (+0x08 / +0x0C).**

Because a TOC may precede payloads it references, the writer lays regions in one pass (patching
payload offsets) then does a **second pass rewriting each TOC in place** (length asserted unchanged).
The header is emitted last; the footer is regenerated from the re-patched sub-bank fields.

### 9.3 Fields PRESERVED VERBATIM

`CRC` (+0x14, §8.1), `Timestamp` (§8.2), entry `Magic` (=42), `ID`, `Type`, `Name` (raw
length-prefixed field), `DepCount`, `Deps`, `InfoSize`, `Info`, and the stats header (`StatsCount` +
`(type,count)` pairs; unchanged on a payload edit because entry count is unchanged). All pad bytes
are `\x00`.

### 9.4 Size-change ripple

A payload size delta ripples through **every region after it in address order**: new starts,
recomputed alignment padding, all `Offset` fields, sub-bank/footer/header offsets. Demos 2/3
exercise real ripples (111 and 310 downstream entries). File size changes by the payload delta
rounded up to the alignment boundary of the following region(s).

### 9.5 Validation method (`validate_write()`)

Re-reads the output through `parse_bigb.parse_header/parse_footer/parse_toc` and asserts, for the
edited `(subbank_name, entry_id)`: **(a) parses clean** — `BIGB`, footer in range, every TOC walks,
chain terminates exactly at `FooterOffset`, every stats header sums to its entry count;
**(b) only the intended entry changed** — same sub-bank count and entry set; every other entry's
`Name/Type/CRC/Timestamp/Deps/Info/Size` and payload bytes identical (only `Offset` may shift);
**(c) the edit landed exactly** — exact new payload bytes, `Size == len(new_payload)`, metadata
preserved. Plus: whole-file byte-diff = one run inside the payload (same-size); every output payload
satisfies its sub-bank Align (size-change); grow-then-revert reproduces the original (invertibility).

### 9.6 NOT supported by `bigb_write.py`

- **Adding or removing entries** (→ use `big_write.rebuild(adds=)`, §10; removal still unsupported).
- **Authoring a brand-new format CLASS** (needs a synthesized CRC — §8.1).
- **Editing an entry's `Info` descriptor** — preserved verbatim; changing texture dims/format or mesh
  vertex-format means editing `Info`, untested here. A same-dims/same-format retexture needs no
  Info change and IS supported. (The later texture writer `tools/texture_build.py` does rewrite
  Info — see `TEXTURE_WRITER.md`.)
- **Editing `Name` / `Deps` bytes** — a rename changes the record length, rippling the TOC.
- **Semantic payload validity** — the writer guarantees only that the *container* is byte-correct
  and the edit isolated.

### 9.7 Reproduce / API

```
python tools/bigb_write.py --demo             # identity(8) + same-size + 2 size-changes + invertibility
python tools/bigb_write.py --demo-samesize    # shaders.big same-size shader-bytecode swap
python tools/bigb_write.py --demo-sizechange  # shaders.big (+64) and frontend.big (+3000) re-flows
python tools/bigb_write.py --identity         # byte-exact round-trip on all 8
python tools/bigb_write.py --invertible       # edit-then-revert == original
```
Edited outputs go to the session scratchpad, never over the read-only Steam install.

```python
import bigb_write as bw
model = bw.load_model(path_to_big)
bw.replace_entry_payload(model, 'PIXEL_SHADERS', entry_id, new_payload_bytes)
out = bw.serialize(model)
ok, report = bw.validate_write(path_to_big, out, ('PIXEL_SHADERS', entry_id), new_payload_bytes)
assert ok
```

---

## 10. Writer B — `tools/big_write.py` `rebuild(buf, edits=None, adds=None)` (new TOC entries)

The content-level writer used by the mesh composer (`MESH.md`) and `texture_build.py`. Each add:
`{sub, name, payload, type, info=b'', id=None, deps=[], crc=0, timestamp=0}`.

- **Record**: synthesized in the exact `parse_bigb.parse_toc` field order (mirrors EgoCore
  `BigBankCompiler`): `magic=42:u32, id, type:i32, size, offset, crc, name (u32-len-prefixed, NO
  trailing NUL — stores exact length), timestamp, depCount + deps, infoSize + info`. `size`/`offset`
  are patched at +12/+16 by the existing layout loop, same as retail records.
- **ID allocation**: default `max(id in subbank) + 1`. Retail TOCs are id-sorted with unique ids
  (verified on both graphics.big subbanks), so appending keeps the order invariant. Duplicate names
  are rejected.
- **Stats header**: the subbank TOC's leading type histogram (`u32 n + n×{type,count}`) is rebuilt
  with the added types incremented (sorted by type, matching retail).
- **Footer**: the subbank's `entry_count` is bumped.
- **CRC/timestamp**: 0. Retail CRC is NOT a CRC-32 of the name or dep path (tested); EgoCore ships
  new entries with CRC 0 and the engine loads them. `deps` (the source `.bbm`/`.tga` path in retail)
  left empty.
- API stability: `rebuild(buf, edits)` signature and no-adds behavior unchanged; `rebuild(buf)`
  round-trips all 8113 graphics.big entries' record fields + payload bytes (content round-trip;
  whole-file byte identity is `bigb_write.py`'s separate in-place contract).
- Proven in-engine: composed meshes appended as ids 8113/8114/8115 to MBANK_ALLMESHES (TOC walks
  CLEAN, `entry_count` 8115) re-open and decode; `texture_build.py replace` on UNASSIGNED_* GBANK
  slots renders in-game.

---

## 11. Write-path risk checklist (all items resolved)

The family doc listed what a byte-exact repacker must reproduce; status after the gate + writers:

1. **Header back-patch** (`FooterOffset`/`FooterSize`) — reproduced byte-exact.
2. **Sub-bank TOC chaining** (TOC[i] ends at TOC[i+1] start; last at `FooterOffset`) — reproduced.
3. **Payload alignment** to sub-bank `Align` with zero fill — reproduced (§5, §9.1).
4. **Stats header** regenerated (`StatsCount<1000`, sums to `EntryCount`) — reproduced; rebuilt on adds.
5. **`CRC`** — was **[hypothesis]** "seed-0 `CCRC::Calc`"; REFUTED, class fingerprint, passthrough (§8.1).
6. **`Timestamp`** — was **[hypothesis]** build timestamp; opaque, passthrough (§8.2).
7. **Per-entry `Info` / `Deps`** re-emitted exactly (per-entry `InfoSize`; length-prefixed strings
   with the trailing-NUL convention) — reproduced.
8. **LIPSYNC specifics** (if editing lip animation): keep `BlockMarker=0x2B`, keep the viseme table
   consistent with keyframe indices, recompute `Info` duration if timing changes, preserve the
   `Dialogue_<N>` / audio-bank ordinal alignment.

Go/No-Go history: 2026-07-19 GO for identity/repack + CRC-zero files; CONDITIONAL for
payload edits of CRC-bearing entries → same day revised to **GO for all realistic edits** once the
CRC was shown to be a class stamp; adds/removes gated → 2026-07-20 adds GO via `big_write`.

---

## 12. dialogue.big deep-dive (LIPSYNC sub-banks)

`dialogue.big` = **4 LIPSYNC sub-banks, 20,505 entries total**, all `Type 1`, paired one-to-one with
the four audio `.lut` banks in the same folder:

| sub-bank | entries | entry name pattern | audio companion |
|----------|--------:|--------------------|-----------------|
| LIPSYNC_ENGLISH_MAIN | 12,134 | `Dialogue_<N>` | `Dialogue.lut` |
| LIPSYNC_ENGLISH_MAIN_2 | 1 | `Dialogue2_<N>` | `Dialogue2.lut` |
| LIPSYNC_ENGLISH_SCRIPT | 5,310 | `ScriptDialogue_<N>` | `ScriptDialogue.lut` |
| LIPSYNC_ENGLISH_SCRIPT_2 | 3,060 | `ScriptDialogue2_<N>` | `ScriptDialogue2.lut` |

The entry `ID` is the **1-based index into the matching audio bank**: a text.big type-0 string
carries `SpeechBank = ScriptDialogue.lug/.lut`; the sound resolves to `ScriptDialogue_<N>`; this
file supplies the lip animation for the same `<N>`. Empty slots exist (e.g.
`ScriptDialogue2_2[Empty]`, `Size=0`) — a reserved index with no clip, still a valid TOC record.

### 12.1 LIPSYNC entry TOC record
- `Name` = `Dialogue_<N>` / `ScriptDialogue_<N>`.
- `DepCount = 2`: `Deps[0]` = `""` (empty), `Deps[1]` = **speaker tag** (all sampled MAIN entries =
  `SPEAKER_FEMALE1`; namespace matches text.big's Speaker/narrator scheme). **[hypothesis: Deps[0]
  reserved for a second/source path, empty in English build]**
- `InfoSize = 4`, `Info` = **float32 clip duration in seconds** (`Dialogue_1` → 1.579, payload
  keyframes span exactly that clip). `Size=0` empty entries have `InfoSize=0`.

### 12.2 LIPSYNC payload blob (fully decoded, byte-exact)

| Order | Type | Field |
|-------|------|-------|
| 1 | u32 | `VisemeCount` |
| 2 | `VisemeCount` × 4 bytes | **Viseme table**: `{ u8 index; char[2] mnemonic; u8 NUL }` |
| 3 | u32 | `BlockMarker` = **0x2B (43)** — keyframe-block sentinel (distinct from entry `Magic`=42) |
| 4 | u32 | `KeyframeCount` |
| 5 | `KeyframeCount` × keyframe | each: `u8 PairCount`, then `PairCount × { u8 VisemeIndex; u8 Weight(0..255) }` |

Visemes are mouth-shape classes named by 2-char ASCII mnemonic — observed set `AH`, `EE`, `OH`, `SZ`
(indices 1–4). A keyframe is a sparse set of `(viseme, blend-weight)` pairs. **[hypothesis: keyframes
are evenly spaced across the clip duration; no explicit per-frame timestamp is stored — count +
duration imply the frame rate.]**

Samples: `LIPSYNC_ENGLISH_MAIN / Dialogue_1` (id=1, size=428, dur=1.579s, speaker=SPEAKER_FEMALE1):
viseme table `(1,'AH') (2,'EE') (3,'OH') (4,'SZ')`; 68 keyframes; kf[0]=`(AH,100)(EE,38)(OH,22)`,
kf[1..3]=`(OH,255)`, kf[4]=`(OH,255)(EE,44)` …; decodes to exactly the `Size` boundary (0x1BC), zero
slack. `Dialogue_3` (id=3, size=262, dur=1.083s): 3 visemes `(AH,EE,OH)`, 46 keyframes, byte-exact.
`ScriptDialogue_1` (id=1, size=1879, dur=6.66s): 4 visemes, byte-exact.

**Whole-bank validation:** all **12,134** LIPSYNC_ENGLISH_MAIN entries decode with this grammar and
terminate byte-exact at their `Size` (0 failures); the other three sub-banks use the identical
grammar. A LIPSYNC entry = viseme dictionary + viseme-weight keyframe animation + duration + speaker
tag + audio-bank index. It does NOT embed the audio (`.lut`) or the display text (text.big).

### 12.3 Cross-reference (text ↔ audio ↔ lipsync)
```
text.big  (TEXT_ENGLISH_MAIN, type-0)  --SpeechBank/ID-->  ScriptDialogue.lut  (audio blob N)
                                                                   ^
dialogue.big (LIPSYNC_ENGLISH_SCRIPT)  ScriptDialogue_N  ---------/  (lip animation for N)
```
Keyed by the ordinal `<N>`. Later refinement (dialogue pipeline): the clip index `<N>` for a
SpeechBank is resolved through `data\Defs\<bank>snds.bin` = sorted `{crc0("SND_"+entryName), soundID}`
pairs; `soundID == .lut clip Index == dialogue.big LIPSYNC id` — see
[DIALOGUE_PIPELINE.md](DIALOGUE_PIPELINE.md).

---

## 13. Verified facts (with addresses)

- Header 16 B (`BIGB`, `Version=100`, `FooterOffset`, `FooterSize`); footer directory; chained
  sub-bank TOCs; entry record `Magic=42`; per-entry `InfoSize` — 8/8 files, 72k+ entries.
- TOC-chain-ends-at-footer invariant holds with zero slack in all 8 files.
- Sub-bank `Align` ∈ {1, 4, 512, 2048}; all inter-region padding is zero (verified, not assumed).
- Byte-exact round-trip 8/8 (SHA-256); single-byte edit controllable; replace-entry-payload
  same-size/size-changing/invertible PASS on shaders/frontend/fonts.
- TOC `CRC` is a per-format-class stamp (465 shaders → 1 value; `0x4e1ace9f` shared by frontend.big
  and textures.big); 0/30 match for any on-disk-byte CRC domain. Passthrough on edit; `0` on add.
- TOC `Timestamp` is opaque high-entropy, not wall-clock; passthrough.
- Chunked-LZO frame `[u16 clen | 0xFFFF u32 clen][bytes]`, `clen==0` stored, `size-3` + 3 plain
  trailing bytes; codec LZO1X `lzo1x_decompress` @ `0x00c06b90`, `lzo1x_decompress_safe` @
  `0x00c08170`; writer `FUN_00996610` @ `0x00996610` (compress + verify, call site `0x009966e5`).
- Zlib is definition-data / installer only: `z_inflate` @ `0x00c02470`, `CCompressorZlib` @
  `0x00a3a840`..`0x00a3abe0`, `CDefinitionManager::DecompressDefinitionData` @ `0x009ad5f0`,
  `CFileInstaller::ProcessActiveFileForDecompression` @ `0x009c3c20`; `CACompressor::Decompress` @
  `0x0052adf0` is an empty stub.
- Bank read path (all raw seek/read, no transform): `CBankFile::ReadEntryData` @ `0x009cc2a0`
  (`file vtbl+0x14` Seek, `vtbl+0xc` Read on the file object at `this+0x7c`),
  `CBankFile::GetEntryDataSize` @ `0x009cc240` (packed-uint array or `[entryTable + 4 + idx*0xc]`),
  `CBankFile::OpenReadOnly` @ `0x009d06f0` (file at `+0x7c` is a plain `CDiskFileWin32`, header read
  via `CFileDataInputStream` with a `0x4000` buffer), `CBankFileAsyncData::SetupBuffer` @
  `0x009d52d0` (dest sized `CBankFile::Align(dataOffset + dataSize)` from
  `CMovableResourceMemoryPool::AllocateResource` or fallback `CMemoryAllocator`),
  `CBankFileAsyncData::OnFileReadCompletion` @ `0x009d54a0`, `CBankFileAsyncData::RelocateData` @
  `0x009d5670` (`ptr - oldBase + newBase`). The engine has an abstract `CDecompressorInterface`
  (`"C:\CDecompressorInterface.cpp"` @ `0x012befc7`, `"Null decompressor"` @ `0x012befe8`).
- LIPSYNC payload grammar byte-exact on 20,505 entries; `BlockMarker=0x2B`.

---

## 14. Open questions

1. **CRC class-schema source** — which build-time string `CCRC::Calc` hashes for the class stamp
   (needed only to author a brand-new resource class). Pin by tracing who WRITES `CRC` at bank-build.
2. **Removing entries / renaming** — untested (record-length ripple through the TOC).
3. **`Timestamp` semantics** — opaque; only matters if a tool wants to stamp builds.
4. **Shader / particle / font payload decoders** — TOC clean, payloads not decoded here
   (effects: see `EFFECTS_FORMAT.md`).
5. **LIPSYNC Deps[0]** purpose and keyframe timing model remain [hypothesis].

---

## 15. Reproduce

```
python tools/parse_bigb.py "<install>\data\lang\English\dialogue.big"          # TOC + histograms
python tools/parse_bigb.py "<install>\data\lang\English\text.big" --dump 3      # sample records
python tools/bigb_roundtrip.py                    # all 8, with per-file coverage + summary
python tools/bigb_roundtrip.py <path\to.big>      # one file, verbose
python tools/bigb_roundtrip.py --smoke <path>     # single-edit controllability test
python tools/verify_big_crc.py                    # 60-formula CRC matrix (0/30)
python tools/bigb_write.py --demo                 # writer demos (§9.7)
```
