# Bank archives (`.big`)

Portable C++23 codec for the retail BIGB bank container ([BIG.md](../../../docs/formats/BIG.md)).
It holds textures, meshes, animations, text, fonts, lipsync, effects and shaders. This is the
fork-side "bank headers" item of MODERNIZATION step 2 (x64 hazard "Persisted layouts"). Every
field is an explicit little-endian `u32`, so nothing depends on pointer width or on retail struct
images.

## Model

- `BigBank`: version, then sub-banks in footer order.
- `SubBank`: name, version, alignment, the optional `(type, count)` stats header, entries.
- `BankEntry`: magic, ID, type, CRC (a per-format class fingerprint), name and dependency strings
  as raw on-disk bytes (retail length prefixes include the NUL; tool-added entries may not),
  timestamp, info sub-header and payload.
- `Payload`: a view into the file the bank was read from, or owned replacement bytes. A 533 MB
  `textures.big` is not copied to rewrite one texture.

## Writer

`encode()` lays out every region (TOCs and non-empty payloads) in its original address order,
using the minimal-repack rule of BIG.md §9.1. Each region starts on `snap_up(cursor, B)`, where
`B` is the largest power of two, at most the file's largest sub-bank alignment, that divides the
region's original start. The footer follows the last region, and padding is zero. An unmodified
bank therefore re-encodes byte for byte. A resized payload moves only the regions after it, and
each keeps its alignment.

## Verified (2026-10-17, MSVC x64 and GCC 15; MSVC x86 on the files up to 250 MB)

`fable_bank_check --edit <dir>` over the install's `data` folder: 17 bank files (the 8 retail
`.big` files plus 9 tool-written copies and backups, up to 535 MB).

- **17/17 decode and re-encode byte-identically.**
- In every sub-bank, one payload is grown by the file's largest alignment plus one. The result
  decodes, only that payload changed, and every payload that was aligned stays aligned.
  Restoring the original bytes gives back the original file in 16/16 aligned files.
- The live `textures.big` was rewritten by the texture-upscale tool with its payloads packed
  unaligned (6,321 of 6,325). It still round-trips exactly. But edit-then-revert cannot be
  byte-exact there: a re-decode can only re-derive boundaries from the shifted offsets. The check
  reports it as `exact+edit(packed)` and requires only that the reverted bank decodes.

`fable_bank_tests` covers a new bank's round trip, empty payloads, a resize ripple with
alignment, grow-then-revert, and the magic, truncation and footer-range gates.

## Not covered

- Payload formats (texture, mesh, animation, text, lipsync decoders): those are per bank kind;
  see BIG.md §4.
- Adding new entries or sub-banks. A bank built from scratch lays out in insertion order with
  every region on the largest alignment, which the engine accepts (the Python `big_write.rebuild(adds=)`
  path loads in-engine), but that path is not exercised against the engine from this codec.
