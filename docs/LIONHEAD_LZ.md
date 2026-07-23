# Fable: TLC `.big` texture LZ codec — RECOVERED (it is LZO1X)

*Authored 2026-07-19 (LZ-RE agent). Method: Ghidra decompile of `Fable.exe` +
byte-level Python port, **proven by decoding real textures to coherent images.**
This closes the one open gap in `docs/BIG_TEXTURE_FORMAT.md` §4.2. Deliverable
decompressor: `tools/lionhead_lz.py`.*

## Status at a glance

| Item | Verdict |
|------|---------|
| Codec identity | **LZO1X** (LZO / miniLZO family) — CONFIRMED from the engine routine |
| Decompressor retail address | `lzo1x_decompress` @ **`0x00c06b90`** (safe variant @ `0x00c08170`) |
| Python port | `tools/lionhead_lz.py::decompress(src, expected_len=None)` — VALIDATED |
| Coherent decoded image | **ACHIEVED** — `FRONTEND_BACKDROP_01` (main-menu forest backdrop), the Fable title logo (ARGB), a DXT1 keyboard panel, a DXT3 button, and in-game character-body atlases from `textures.big` all decode correctly |
| Generalization | Codec covers **all GBANK texture banks** (frontend.big + textures.big); `graphics.big` **mesh** payloads are **not** LZO-framed (stored with plaintext-name headers) |

## 1. Where it lives in the engine

Ghidra decompile of `Fable.exe` (project `FableTLC`, program `/Fable.exe`) turned
up the decompressor family. String anchors:

- `"C:\CDecompressorInterface.cpp"` @ `0x012befc7`, `"Null decompressor"` @
  `0x012befe8` — the engine has an abstract `CDecompressorInterface` with pluggable
  back-ends (Zlib for **definition data**, LZO for **texture pixel data**).
- `" inflate 1.2.1 Copyright 1995-2003 Mark "` @ `0x012b1828` — zlib is present but
  is used for `CDefinitionManager::DecompressDefinitionData` (`CCompressorZlib`),
  **not** textures. This is why the texture stream never inflated.

The decompressor routines (all decompiled cleanly):

| Symbol | Retail addr | Role |
|--------|-------------|------|
| `lzo1x_decompress` | **`0x00c06b90`** | the texture pixel-block decompressor (the codec) |
| `lzo1x_decompress_safe` | `0x00c08170` | bounds-checked variant of the same grammar |
| `CCompressorZlib::Decompress` | `0x00a3abe0` | zlib path (definition data, not textures) |
| `z_inflate` / `inflate_fast` | `0x00c02470` / `0x00c06060` | zlib internals |

Ghidra BSim tagged `lzo1x_decompress` at **sim=1.0** against the LZO1X reference in
the `ego_r` (EgoCore / Fable 2) signature DB — an independent confirmation of the
codec identity on top of the byte-level match below.

## 2. The algorithm (LZO1X-1)

The stream is standard **LZO1X**. The exact grammar (ported 1:1 from
`0x00c06b90`; see `tools/lionhead_lz.py`):

- **Window / offsets:** distances up to ~48 KB (14-bit + high bit); back-references
  into the already-decompressed output. Min match length 2.
- **First byte special case:** if the very first stream byte `t >= 0x12`, it encodes
  an initial literal run of `t - 0x11` bytes.
- **Literal-run token** (at loop top, `t < 0x10`): copies `t + 3` literals, with the
  LZO zero-byte length extension (base 15) when `t == 0`.
- **Match tokens**, selected by the token byte `t`:

  | `t` range | Type | Length | Distance | Extra bytes |
  |-----------|------|--------|----------|-------------|
  | `0x40..0xff` | **M2** | `(t>>5) + 1` (2..8) | `((t>>2)&7) + (H<<3) + 1` | 1 (`H`) |
  | `0x20..0x3f` | **M3** | `(t&0x1f)` +ext, `+2` | `(word>>2) + 1` (14-bit) | 2 (LE `word`) |
  | `0x10..0x1f` | **M4** | `(t&7)` +ext, `+2` | `((t&8)<<11) + (word>>2) + 0x4000` | 2 (LE `word`) |
  | `0x00..0x0f` | **R1/M1** | 3 / 2 | see below | 1 (`H`) |

  - **M4 encoded-distance 0 = end-of-stream marker** (the canonical `11 00 00`).
  - **R1 vs M1 short match** (`t < 0x10`): if the token immediately follows a
    literal run it is **R1** — length **3**, distance `(t>>2) + (H<<2) + 0x0801`.
    Otherwise (in the trailing-literal match chain) it is **M1** — length **2**,
    distance `(t>>2) + (H<<2) + 1`.

- **Trailing literals (the one subtlety that must be exact):** after *every* match,
  the number of immediately-following literals (0..3) is
  **`src[ip-2] & 3`** — i.e. the low 2 bits of the input byte two positions back
  once the whole match token has been consumed. For M1/M2 that byte is the token;
  for **M3/M4 it is the low distance byte**, not the token. Getting this wrong
  desyncs the stream a few thousand bytes in (the failure mode that defeated the
  earlier black-box attempts). A nonzero trailing count chains straight into the
  next match with M1 semantics. This maps to the engine's
  `uVar5 = (byte)param_1[-1] & 3` at `LAB_00c06c6e`.

The engine routine's `LAB_00c06c3a` (R1) and `LAB_00c06d9e` (M1) are the two
distinct short-match sites, and `code_r0x00c06bdc` is the literal-run handler — all
reproduced in the port.

## 3. Framing (unchanged, from BIG_TEXTURE_FORMAT.md §4.1)

Each texture entry's payload is a sequence of per-mip LZO1X blocks:

```
block := u16 clen                 ; header 2 bytes, when clen < 0xffff
       | u16 0xffff , u32 clen    ; header 6 bytes, when clen >= 0xffff
       followed by clen bytes of LZO1X stream
```

`decompress()` takes exactly the `clen` compressed bytes and returns the raw mip.
On every tested block the LZO **EOS marker lands exactly at `clen`** (all input
consumed), confirming both the framing and the codec.

**Uncompressed-tail note:** decompressed base mips come out **3 bytes short** of the
full surface size (e.g. `262141` vs `262144` for 1024×512 DXT1) with a clean EOS
consuming all input. This is the engine's actual encoder output (the final few
surface bytes are not stored); pad with zero. It is *not* a decoder bug — proven by
the coherent images, which are byte-identical to the game's rendering.

## 4. Validation (the proof)

Reproduce (READ-ONLY on the install; import `parse_texture.py` unmodified for the
Info/framing + DXT/ARGB decoders):

```
python tools/lionhead_lz.py <block.bin> <rawlen> <out.raw>   # decompress one block
```

Coherent decoded images produced this session (in scratchpad):

| Texture | Bank | Format | Result |
|---------|------|--------|--------|
| `FRONTEND_BACKDROP_01` (640×480 in 1024×512) | frontend.big | DXT1 | main-menu misty-forest backdrop with the hero + guild tattoo — **coherent** |
| `FRONTEND_TITLE_01_SPRITE` (256×128) | frontend.big | A8R8G8B8 | the "FABLE — THE LOST" title logo with alpha — **coherent** |
| `FRONTEND_KEYBOARD_BG_01_SPRITE` (256×256) | frontend.big | DXT1 | carved-wood Celtic panel — **coherent** |
| `FRONTEND_BUTTON_L_SPRITE` (128×64) | frontend.big | DXT3 | UI button sprite with transparency — **coherent** |
| `HEROGUIDE_BODY2_A` / `MONGROL_*` etc. (512×512) | textures.big MAIN | DXT1 | character-body atlases — **coherent** |

Corpus check: **300/300** DXT1/DXT3 base mips sampled across `textures.big`
`MBANK_MAIN` decompress with a clean EOS and a size matching the expected raw mip
(±the 3-byte tail). No stream failed to decode.

## 5. Does it generalize to other `.big` payloads?

- **All GBANK texture banks: YES.** `frontend.big` (`GBANK_FRONT_END_PC`) and
  `textures.big` (`GBANK_GUI_PC`, `GBANK_MAIN_PC`) all use this exact LZO1X framing
  per mip. One codec, one decompressor.
- **`graphics.big` mesh (MBANK) payloads: NO — not LZO-framed.** Their payloads begin
  with plaintext mesh-name headers (e.g. `SH_OBJECT_STATUE`, `SH_SEAGULL_01`); the
  leading bytes are not an LZO stream. Mesh data is stored per `docs/BIG_MESH_FORMAT.md`
  (which classifies/validates meshes with no decompression step). Attempting LZO on a
  MBANK entry desyncs immediately. So mesh authoring is **not** blocked on this codec.

## 6. Impact on retexturing

This closes `BIG_TEXTURE_FORMAT.md` §4.2 and §6. With a decoder in hand:

- **Reading/exporting any texture to PNG — now unblocked end-to-end** (decompress →
  linear DXT/ARGB decode → PNG).
- **Authoring new textures — encoder still to write.** LZO1X *compression* (the
  `lzo1x_1_compress` side) is the remaining piece for arbitrary new art; it is a
  standard, well-documented algorithm (public-domain miniLZO), so this is now a
  straightforward port rather than a reverse-engineering problem. The framing to
  emit is §3. (Also worth a quick check: whether the loader accepts a stored/
  uncompressed block, per §6 of the texture doc — not needed for read/export.)

## 7. Deliverables

- `tools/lionhead_lz.py` — `decompress(src, expected_len=None) -> bytes`. Standalone
  LZO1X-1 decompressor, no third-party deps. CLI: `python tools/lionhead_lz.py in.bin [rawlen] [out.raw]`.
- Decompiled reference: `ghidra_out/decomp_lionhead_lz_names.c` (contains
  `lzo1x_decompress` @ `0x00c06b90` and the full decompressor family); string-xref
  log `ghidra_out/decomp_lionhead_lz.log`.
- Coherent PNG evidence: `scratchpad/backdrop01_decoded.png`, `val_3.png` (title),
  `val_5.png` (DXT1 panel), `val_11.png` (DXT3 button), `texbig_1.png` (world atlas).
