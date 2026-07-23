# Fable: TLC Save Write Pipeline — Gate Results & Signature Verdict

**Status:** READ→EDIT→WRITE pipeline built and gated. Signature **SOLVED (CONFIRMED)**.
**Date:** 2026-07-19.
**Tool:** `tools/save_edit.py` (pure Python, stdlib `struct`+`zlib` only — no Ghidra, no deps).
**Companion spec:** `docs/SAVEGAME_FORMAT.md` (the format crack; this doc is the write-side proof).

This doc records only what is **provable on real bytes here**. The ultimate test — the retail
game loading an edited save — cannot run in this environment; the go/no-go section is explicit
about what the gates DO and DON'T prove.

---

## 0. Headline

- All four gates **PASS** on all 5 real save files on disk.
- The `+0x0C` dword is **NOT a signature** — it is the **total compressed-data length** (the file
  offset at which the trailer begins). SAVEGAME_FORMAT.md §3.1/§9 called it a
  "signature/version [hypothesis]"; that hypothesis is **corrected here**.
- The real signature is the **4-byte trailer** written immediately after the last zlib chunk. It is
  **`CCRC::Calc(seed=0, file[0 : trailer_pos])`** — a seed-**0** reflected CRC-32 over the entire
  file prefix (magic + 16-byte header + both zlib chunks + the inter-chunk framing dword).
  **CONFIRMED**: computed value equals the stored trailer on all 5 saves.
- CRC seeds: **all on-disk hashes use seed 0** — field **tags** AND the file **trailer signature**
  (same reflected table, no final inversion). [CORRECTED 2026-07-19: field tags were wrongly noted here
  as seed 0xFFFFFFFF; real save tags (WorldName/Money/Morality) are seed-0 — see SAVEGAME_FORMAT.md top
  CORRECTION + FINDINGS.md canonical reference. NOTE: `save_edit.py:get_crc` defaults to 0xFFFFFFFF —
  pass seed 0 for field tags.]

---

## 1. Corrected container layout (write-accurate)

```
0x00  "FableSave!"                10  magic (ASCII, no NUL)
0x0A  00 00                        2  pad
0x0C  u32 total_data_len          == trailer_pos (offset to trailer; = end of compressed data)
0x10  u32 chunk0_uncompressed_len == inflated HEADER-section size
0x14  u32 chunk1_uncompressed_len == inflated main/ENTITIES payload size
0x18  u32 chunk0_compressed_len   == byte length of first zlib blob
0x1C  chunk0 zlib stream (78 DA), chunk0_compressed_len bytes
      u32 chunk1_compressed_len   (inter-chunk framing dword)
      chunk1 zlib stream (78 DA)
@trailer_pos:
      u32 trailer_sig             = CCRC::Calc(0, file[0:trailer_pos])   <-- THE signature
      00 00 ... 00                zero padding to 0x4B000 (307200) bytes
```

Evidence that `+0x0C == trailer_pos` (5/5 saves):

| file | +0x0C | trailer_pos | equal |
|---|---|---|---|
| Cornelio/AutoSave     | 78321 | 78321 | yes |
| Cornelio/AutoSave.qs  | 78321 | 78321 | yes |
| Cornelio/Manual-Save1 | 78693 | 78693 | yes |
| Default/AutoSave      | 14084 | 14084 | yes |
| Test/AutoSave         | 13986 | 13986 | yes |

---

## 2. Gate results (all on real bytes, all 5 saves)

Harness: read each save, run each gate as an actual `assert`-style comparison, report per file.

### GATE A — inflated-stream identity (section walker is lossless)  ✅ PASS 5/5
Walk the inflated payload into sections (`name\0`, `u32 size`, body), re-emit every section header+body
back-to-back plus any residual bytes, and assert the reconstruction is **byte-identical** to the
original inflated payload. This proves the section walker / serializer loses nothing.

    Cornelio/AutoSave      ident=True   payload=90469 B
    Cornelio/AutoSave.qs   ident=True   payload=90469 B
    Cornelio/Manual-Save1  ident=True   payload=90799 B
    Default/AutoSave       ident=True   payload=22011 B
    Test/AutoSave          ident=True   payload=21921 B

(Only `HEADER` is a clean top-level named section; the remainder is the `ENTITIES`/`SAVED_ENTITIES`
nested entity stream. The walker recognises `HEADER` and treats the rest as an opaque residual tail —
identity still holds, and field edits reach into it via tag scan. A full positional entity schema is
out of scope for a generic set-field-by-tag editor.)

### GATE B — pipeline lossless round-trip (read → no edit → write → re-read)  ✅ PASS 5/5
Full container rebuild: re-deflate both chunks, fix all header dwords, append recomputed seed-0
trailer, pad to 0x4B000. Re-read the produced file and assert inflated payload + chunk sizes are
identical to the original, and the file is exactly 307200 bytes. The re-read **also re-verifies the
trailer signature** on our own output (so our writer produces a self-consistent, signature-valid file).

    all: payload_identical=True  chunk_sizes_identical=True  slab=307200=True

### GATE C — single-field edit  ✅ PASS 5/5
Locate a HEADER `u32` field by its CRC tag, set it to a sentinel (`0xC0FFEE01`), write, re-read, and
assert (a) the re-read field equals the sentinel and (b) the **only** bytes that changed in the whole
inflated payload are exactly that field's 4 value bytes.

    edited HEADER tag 5b0fcaa0 (u32): reread==sentinel, diff bytes == exactly the 4 value bytes, 5/5

### GATE D — signature  ✅ CONFIRMED 5/5
Assert `stored_trailer == CCRC::Calc(0, file[0:trailer_pos])`.

    file                    stored     computed
    Cornelio/AutoSave       a3ba1c9c = a3ba1c9c
    Cornelio/AutoSave.qs    a3ba1c9c = a3ba1c9c
    Cornelio/Manual-Save1   0ddf1f62 = 0ddf1f62
    Default/AutoSave        36dfc709 = 36dfc709
    Test/AutoSave           b06b17a6 = b06b17a6

**Signature formula (CONFIRMED):**
`trailer = ccrc_calc(seed=0, data = bytes[0 .. trailer_pos))` where the input is the literal file
prefix: `"FableSave!" + pad + 16-byte header + chunk0 zlib + u32 chunk1_clen + chunk1 zlib`.
No final XOR/inversion (matches `CCRC::Calc` in SAVEGAME_FORMAT.md §6). Equivalent to
`zlib.crc32` **only** if you pre/post-invert; the game uses the raw seed-0 table form, which is what
`ccrc_calc(data, 0)` in `save_edit.py` implements and what matched.

---

## 3. zlib reproduction note (does our deflate reproduce original compressed bytes?)

**No — and it does not need to.** Python's `zlib.compress(data, 9)` emits a valid `78 DA` stream that
inflates to the identical payload, but its Huffman/match choices differ slightly from the compressor
the game shipped (`CCompressorZlib`), so the compressed bytes — and therefore whole-file bytes —
differ:

| file | whole-file identical | orig compressed | ours | delta |
|---|---|---|---|---|
| Cornelio/AutoSave     | No | 78289 | 78327 | +38 |
| Cornelio/AutoSave.qs  | No | 78289 | 78327 | +38 |
| Cornelio/Manual-Save1 | No | 78661 | 78693 | +32 |
| Default/AutoSave      | No | 14052 | 14058 |  +6 |
| Test/AutoSave         | No | 13954 | 13967 | +13 |

Delta is tiny (+6…+38 bytes on 14–78 KB). This is fine because:
- The header length dwords are **recomputed** from our actual compressed sizes.
- The `+0x0C` total_data_len and the trailer signature are **recomputed** over our actual bytes.
- The file stays far under the 300 KB slab.
The game reads lengths from the header and inflates — it never requires byte-identical compressed
output. Whole-file byte-identity is therefore **not a requirement** and is **not achievable** with
stock zlib; the pipeline is correct without it.

---

## 4. Go / No-Go for a save editor

**GO — with one caveat that only a live game-load can close.**

What is **safe now** (proven here, offline):
- Reading any `FableSave!` file: header split, chunk inflate, section walk, tag decode.
- Editing a field by CRC tag (fixed-width int/float/bool; NUL-terminated string with section-length
  fixup) and producing a **structurally valid, signature-valid, correctly-sized** save.
- The signature is solved and reproduced, so edited saves carry a **correct trailer** — no need to
  disable `UseSaveGameSignatures`, and no risk of a signature-mismatch rejection.

What still needs **live game validation** (cannot run here):
- Whether the retail loader accepts a save whose **compressed bytes differ** from its own compressor's
  output. Highly likely yes (it inflates by length), but unconfirmed against the actual binary.
- **Semantic** validity of an edited field value: the gates prove byte-level correctness, not that a
  given HEADER/ENTITIES field accepts an arbitrary value without the game clamping/rejecting it. The
  tag→name map for hero stats/gold/morality (SAVEGAME_FORMAT.md §9 item 2, owned by a separate agent
  in `docs/SAVE_HEADER_MAP.md`) is the remaining "which field means what" work — orthogonal to this
  write pipeline, which is generic over tags.
- Entity-stream (`ENTITIES`/`SAVED_ENTITIES`) positional editing: this pipeline treats that region as
  opaque and only reaches fields by tag scan. Deep entity/inventory edits need the full positional
  schema (a future task).

**Bottom line:** the container/signature layer is **done and validated**; a save editor can be built
on `tools/save_edit.py` today. The only unproven link is the game actually loading the result, which
requires running the retail build — recommended as the next confirmation step, but no reversed
signature routine is blocking it (the signature is solved).

---

## 5. Reproducing the gates

    python tools/save_edit.py <FableSave-file>     # dump container + sections + verify signature

Full gate harness (kept in scratchpad, reads copies of the real saves — real saves are treated as
read-only; all writes go to scratchpad `out/`): `run_gates.py`. Summary line it prints:

    SUMMARY: A(identity)=True  B(roundtrip)=True  C(edit)=True  D(signature)=True

---

## 6. Corrections fed back to the format spec (do NOT edit SAVEGAME_FORMAT.md here — noted for the owner)

- §3.1 / §9 item 1: `+0x0C` is **total_data_len (offset to trailer)**, not a signature/version.
- §3.2 / §8: the signature is the **4-byte** trailer at `trailer_pos` (the doc guessed "16 trailing
  bytes"); the following 12 bytes are already zero slab padding. Formula **CONFIRMED**:
  seed-0 CRC-32 over `file[0:trailer_pos]`.
- Open item §9.1 (signature input + formula) is **CLOSED** by this doc. Whether the loader *enforces*
  it at load time is the only remaining sub-question, answerable only by running the game.
