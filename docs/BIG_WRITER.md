# .big (BIGB) controlled WRITE prototype — replace-entry-payload + writer contract

*Authored 2026-07-19 (write-prototype agent). Status: **GO for replace-entry-payload edits
(same-size AND size-changing), validated by re-read on real retail files.***

Extends the proven read + byte-exact round-trip work (`docs/BIG_ROUNDTRIP.md`,
`docs/BIG_FAMILY_FORMATS.md`) into a controlled WRITE prototype:
`tools/bigb_write.py` (pure Python; reuses `tools/parse_bigb.py`'s reader and
`tools/bigb_roundtrip.py`'s model helpers as unmodified dependencies).

This document is the **writer CONTRACT** the FableForge C++ writer must satisfy, plus the
two demo results and the explicit not-yet-supported list. It supersedes nothing; it is the
write-path companion to the read-path docs.

---

## 0. What was built + proven (summary)

`tools/bigb_write.py` implements **replace-entry-payload**: swap ONE entry's payload bytes
for new bytes (texture pixel data / shader bytecode / mesh blob), re-lay all offsets, re-pad
to alignment, preserve TOC CRC/Timestamp verbatim, and rewrite header/footer. It handles the
new payload being the **same size** (no downstream movement) OR a **different size**
(downstream regions ripple — the whole point of a model-driven writer).

Every write is validated by **re-reading the output through `parse_bigb.py`** and asserting
exactly the intended change. Run `python tools/bigb_write.py --demo`.

| Demo | File | Edit | Result |
|------|------|------|--------|
| 0 identity | all 8 | none | **PASS** — byte-exact (SHA match) round-trip on every file |
| 1 same-size | shaders.big | replace 41-B shader bytecode, same length | **PASS** — exactly 1 byte-diff run of 41 B inside the payload; 464 other entries identical |
| 2 size-change (Align=4) | shaders.big | grow a 29-B shader to 93 B (+64) | **PASS** — file +256 B, 111 downstream Offsets re-flowed, only that entry changed on re-read |
| 3 size-change (Align=2048) | frontend.big | grow a GBANK texture by 3000 B | **PASS** — file +2048 B, 310 downstream Offsets shifted, 0 Align-2048 violations |
| 4 invertibility | shaders.big | grow +64 then revert to original bytes | **PASS** — reverted file is byte-exact the untouched original (writer is lossless) |
| (extra) fonts mixed-align | fonts.big | grow an Align=512 payload +700 B | **PASS** — 0 per-sub-bank Align violations; only that entry changed |

In every edited case, all OTHER entries' `Name/Type/CRC/Timestamp/Deps/Info/Size` **and
payload bytes** are byte-identical on re-read; the edited entry reads back the EXACT new
bytes; and its `CRC`/`Timestamp` are preserved verbatim.

---

## 1. The layout fact that drives the writer — TOCs and payloads are INTERLEAVED

A `.big` is NOT header + one contiguous payload block + TOC block. In `shaders.big` the
per-sub-bank TOCs and the entry payloads are **interleaved in address order** (payloads for
one sub-bank can sit before, between, and after other sub-banks' TOCs). Verified layout:

```
shaders.big: 493 byte-regions in address order, 416 zero-pad gaps (898 B total), 0 overlaps.
0x0000..0x0010 header | 0x0010.. payloads (subbank 5) | ... | 0x5a84c.. TOC blocks | 0x6e6c2 footer
```

Therefore the only universally-correct re-layout is **address-order re-flow**: keep every
region in its original on-disk address order, and when one payload changes size, shift the
regions that follow it and re-establish alignment. The writer does NOT assume any contiguous
payload region and does NOT re-order.

Region kinds (address-ordered): `header` (16 B @ 0) · `toc` (one per sub-bank) ·
`payload` (one per non-empty entry) · `footer` (sub-bank directory).

---

## 2. WRITER CONTRACT — what the C++ writer MUST do

### 2.1 Offset re-layout rule (the core algorithm) — MINIMAL REPACK

Walk all regions in on-disk address order with a running output `cursor`. For each region:

```
new_start = snap_up(cursor, B)      where  B = orig_boundary(orig_start, cap)
```

- `orig_boundary(x, cap)` = the largest power of two ≤ `cap` that divides the region's
  **original** start `x`. This is the alignment boundary the bank builder actually placed the
  region on.
- `cap` = the **MAX sub-bank Align across the whole file** (e.g. 4 for shaders.big, 2048 for
  frontend/effects/graphics/textures, 2048 for fonts.big which mixes 512/2048, 1 for
  text/dialogue). Header and footer use `cap = 1` (header is at 0; the footer follows the
  last TOC contiguously — verified: no gap before the footer in any file).
- There is **NO "preserve the original gap" term.** The inter-region padding is exactly the
  minimum zero fill needed to reach `B`.

Why this exact rule (all three verified on all 8 retail files):
- **Identity:** in a pristine file every region already sits contiguously after the previous
  one at its own boundary, so `snap_up` is a no-op and every start reproduces exactly →
  byte-exact round-trip (`identity mismatches = 0 / 8`).
- **Size-change:** after an upstream payload grows, downstream regions ripple forward by the
  minimum needed to stay on their alignment boundary (every payload still satisfies its
  sub-bank Align); when a payload **shrinks**, freed space is reclaimed (no stale padding).
- **Invertible / lossless:** capping `B` at the file's max Align (not at a *coincidental*
  large divisor of an address) means an edit and its exact reverse compute the same boundary,
  so edit-then-revert reproduces the original byte-for-byte (Demo 4). Capping at the
  *per-sub-bank* Align instead breaks fonts.big identity (some Align=512 payloads sit on a
  2048 boundary); capping at a fixed 2048 breaks invertibility on Align=4 shaders.big. **The
  max-file-Align cap is the one rule that satisfies both.**

### 2.2 Fields RE-PATCHED from the new layout (position-dependent)

- **Entry `Offset` (TOC +0x10):** set to each payload's new start as it is laid.
- **Entry `Size` (TOC +0x0C):** changed **only for the one edited entry**, to `len(new_payload)`.
  Every other entry's Size is unchanged.
- **Sub-bank `Offset` and `Size` (footer directory):** each sub-bank's TOC new start and new
  byte length. (TOC byte length is invariant to payload-offset *values* — the Offset field is
  fixed-width u32 — so a payload size change does not change any TOC's length; only the edited
  entry's own `Size` field value changes, in place.)
- **Header `FooterOffset` / `FooterSize` (header +0x08 / +0x0C):** the new footer position and
  length after all payloads/TOCs re-flow.

Because a TOC may, in address order, precede payloads it references, the writer lays regions in
one pass (patching payload offsets) then does a **second pass rewriting each TOC in place** with
the now-final payload offsets. The rewrite never changes a TOC's length (asserted).

### 2.3 Fields PRESERVED VERBATIM (never recompute)

- **`CRC` (TOC +0x14) — passthrough verbatim.** Per `docs/BIG_ROUNDTRIP.md` §7 the TOC CRC is a
  **per-format-CLASS fingerprint, NOT a per-payload hash** (all 465 shaders share one CRC;
  `0x4e1ace9f` recurs across frontend.big and textures.big). An edited payload of an existing
  format legitimately keeps the SAME CRC as its unedited siblings. The writer copies the stored
  CRC unchanged. Recomputing it is **wrong** and unnecessary for retexture / mesh-swap /
  shader-swap.
- **`Timestamp` (TOC +0x18) — passthrough verbatim.** High-entropy opaque per-entry value, not
  wall-clock (§7.4). Never regenerate / stamp "now".
- **Entry `Magic` (=42), `ID`, `Type`, `Name` (length-prefixed, trailing-NUL convention),
  `DepCount`, `Deps` (length-prefixed build paths / speaker tags), `InfoSize`, `Info`** — all
  re-emitted byte-for-byte from the model. `Name`/`Deps` are re-emitted from the **raw on-disk
  length-prefixed field** (u32 len + raw bytes incl. any trailing NUL), never re-encoded from a
  stripped string — this is the one fidelity trap and it is handled (`name_raw`, `deps_raw`).
- **Stats header** (`StatsCount` + `(type,count)` pairs, `StatsCount < 1000` rewind rule): kept
  and re-emitted verbatim; it does not change on a payload edit (entry count is unchanged).

### 2.4 Align zero-fill

All inter-region padding is **zero fill** (verified 100 % zero in every file). The writer emits
`\x00` for every pad byte produced by §2.1's `snap_up`. Align is load-bearing: 2048 for
GBANK/MBANK/PARTICLE/FONT_MAIN, 512 for STREAMING_FONT, 4 for shaders, 1 for text/dialogue.

### 2.5 Stats-header / header regeneration

The header is emitted last (once `FooterOffset`/`FooterSize` are known) as
`"BIGB" + u32 Version + u32 FooterOffset + u32 FooterSize`. The footer is regenerated from the
sub-bank fields with the re-patched `Offset`/`Size` (§2.2). Both reproduce byte-exact on identity.

### 2.6 Size-change ripple (explicit)

A payload size delta ripples through **every region after it in address order**: their new
starts shift, their alignment padding is recomputed (§2.1), all their `Offset` fields and the
sub-bank/footer/header offsets are re-patched. Demos 2/3 exercise real ripples (111 and 310
downstream entries moved). The file's total size changes by the payload delta rounded up to the
alignment boundary of the following region(s).

---

## 3. Validation method (how "safe" is proven, not asserted)

`validate_write()` re-reads the output buffer through `parse_bigb.parse_header/parse_footer/
parse_toc` and asserts, for the one edited key `(subbank_name, entry_id)`:

**(a) parses clean:** output is `BIGB`, footer in range, every sub-bank TOC walks with no
error, the TOC chain terminates **exactly at `FooterOffset`**, and every stats header sums to
its entry count.

**(b) only the intended entry changed:** the output has the **same sub-bank count and the same
entry set** (no adds/removes), and for **every other entry** the `Name/Type/CRC/Timestamp/
Deps/Info/Size` **and the payload bytes** are byte-identical to the original (only `Offset`
may legitimately shift).

**(c) the edit landed exactly:** the edited entry reads back the **exact new payload bytes**,
its `Size == len(new_payload)`, and its `Name/Type/CRC/Timestamp/Deps/Info` are preserved.

Plus independent cross-checks: whole-file byte-diff = one run inside the payload (same-size);
every output payload satisfies its sub-bank Align (size-change); a grow-then-revert reproduces
the original byte-exact (invertibility). This is evidence, not assumption — a write is only
"GO" when the re-parse proves exactly the intended change and nothing else.

---

## 4. NOT yet supported (explicit gaps)

The prototype is scoped to **replace-entry-payload of an existing entry in an existing format
class**. The following are deliberately out of scope and must NOT be assumed to work:

- **Adding or removing entries.** Changes `EntryCount`, the stats-header `(type,count)` pairs,
  and the TOC byte length (so every sub-bank `Offset`/`Size` and the footer shift by TOC-record
  deltas, not just payload deltas). The re-flow engine could be extended to it, but the entry
  record layout (Magic/ID assignment, ID uniqueness/ordinal meaning — e.g. LIPSYNC IDs are
  1-based audio-bank indices) is unvalidated for insertion. **Do not add/remove entries yet.**
- **Authoring a brand-new format CLASS**, or changing an entry's format so its class fingerprint
  should differ. That requires synthesizing a fresh `CRC` from the build-time class-schema
  string, which is **not present in the `.big` bytes** (see `docs/BIG_ROUNDTRIP.md` §7.3/§7.5).
  A Ghidra trace of who WRITES `CRC` at bank-build would pin the schema source. Until then the
  writer preserves CRC verbatim and can only edit payloads that keep their existing format.
- **Editing an entry's `Info` descriptor** (texture dims/format, mesh vertex-format). The
  prototype preserves `Info` verbatim. Changing a texture's dimensions or pixel format means
  editing `Info` — the loader reads it to interpret the payload — and is untested here. A
  retexture that keeps the same dimensions/format (just new pixels) needs no `Info` change and
  IS supported.
- **Editing `Name` / `Deps` bytes.** Preserved verbatim. A rename would change the record's
  length (length-prefixed), rippling the TOC like an add/remove; untested. (The reader already
  documents the trailing-NUL length convention a writer must match — `BIG_ROUNDTRIP.md` §2.)
- **Semantic payload validity.** The writer guarantees the *container* is byte-correct and the
  edit is isolated; it does NOT validate that the new payload is a valid shader/texture/mesh for
  the engine. That is the caller's responsibility (e.g. a same-size same-format DXT blob).

---

## 5. Reproduce

```
python tools/bigb_write.py --demo             # identity(8) + same-size + 2 size-changes + invertibility
python tools/bigb_write.py --demo-samesize    # shaders.big same-size shader-bytecode swap
python tools/bigb_write.py --demo-sizechange  # shaders.big (+64) and frontend.big (+3000) re-flows
python tools/bigb_write.py --identity         # byte-exact round-trip on all 8
python tools/bigb_write.py --invertible       # edit-then-revert == original
```

Edited outputs are written to the session scratchpad, never over the read-only Steam install.

Programmatic use:
```python
import bigb_write as bw
model = bw.load_model(path_to_big)
bw.replace_entry_payload(model, 'PIXEL_SHADERS', entry_id, new_payload_bytes)
out = bw.serialize(model)
ok, report = bw.validate_write(path_to_big, out, ('PIXEL_SHADERS', entry_id), new_payload_bytes)
assert ok
```

---

## 6. Verdict

**GO — replace-entry-payload (same-size and size-changing) on any of the 8 retail `.big`
files.** The read → model → edit → serialize → re-read cycle is proven to change exactly the
intended entry and nothing else, on real files, for both Align=4 and Align=2048 (and mixed
512/2048) banks, and is lossless/invertible. CRC and Timestamp are correctly passed through
verbatim (no payload hash exists to recompute). The C++ writer in FableForge can implement
§2's contract directly. Adds/removes and new-format authoring remain gated on §4.
