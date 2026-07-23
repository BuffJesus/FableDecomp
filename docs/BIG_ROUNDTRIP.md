# .big (BIGB) byte-exact round-trip — the WRITE-PATH validation gate

*Authored 2026-07-19. Status: **GATE PASSED — 8/8 retail `.big` files round-trip byte-for-byte.***
This is the mandatory writer gate flagged in `docs/BIG_FAMILY_FORMATS.md` §6 and
`docs/TEXTBIG_FORMAT.md` §7: prove a read → in-memory model → RE-SERIALIZE cycle reproduces an
unmodified input **byte-identically** before any `.big` writer ships. It does — verified by SHA-256
equality on every file, up to and including the 533 MB `textures.big`.

Harness: `tools/bigb_roundtrip.py` (pure Python; reuses the validated reader in
`tools/parse_bigb.py` as a dependency — no forks, additive `_augment_entry_raw` only).
Format reference: `docs/TEXTBIG_FORMAT.md` (container) + `docs/BIG_FAMILY_FORMATS.md` (family).

---

## 1. Result table (all 8 files, real byte-compare)

Each PASS is an **actual `output == original` byte-equality check** plus a SHA-256 match on the full
file — not a structural re-parse. `passthru` = bytes NOT rebuilt from parsed fields but copied
verbatim (see §3); all of it is alignment padding and all of it is zero.

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

The harness prints the full `sha256(in)` and `sha256(out)` per file; they are **identical for all 8**
(the short prefixes above are `sha256(in)`==`sha256(out)`).

**8/8 byte-exact. Zero FAILs. Zero non-zero passthrough bytes anywhere.**

Reproduce:
```
python tools/bigb_roundtrip.py                    # all 8, with per-file coverage + summary
python tools/bigb_roundtrip.py <path\to.big>      # one file, verbose
python tools/bigb_roundtrip.py --smoke <path>     # single-edit controllability test
```

---

## 2. Write-path CONTRACT — what the serializer RECONSTRUCTS vs. PASSES THROUGH

The harness proves the writer can be **model-driven** (rebuilt from parsed fields), not a blind file
copy. It rebuilds every structural region from the in-memory model and lays it at the exact recorded
byte offset; only genuinely opaque padding is copied. The C++ writer must reproduce the same split.

### RECONSTRUCTED from the model (must byte-match — a mismatch is a serializer bug)
- **Header (16 B @ 0):** `Magic="BIGB"`, `Version`, `FooterOffset`, `FooterSize`. Re-emitted from
  fields. (`FooterOffset`/`FooterSize` are the header back-patch of `BIG_FAMILY_FORMATS.md` §6.1 —
  reproduced exactly.)
- **Footer / sub-bank directory @ `FooterOffset`:** `SubBankCount`, then per sub-bank
  `Name+NUL`, `Version`, `EntryCount`, `Offset`, `Size`, `Align`. Re-emitted from fields. The
  per-sub-bank `Offset`/`Size` (the TOC-chain contiguity of §6.2) are reproduced from the parsed
  values — the writer must keep TOC[i].end == TOC[i+1].start and last == `FooterOffset`.
- **Per-sub-bank TOC:** the optional stats header (`StatsCount` + `(type,count)` pairs, emitted iff
  `StatsCount<1000` per EgoCore's rewind rule) **plus every entry record**:
  `Magic, ID, Type, Size, Offset, CRC` (6×u32), the length-prefixed `Name`, `Timestamp`, `DepCount`,
  each length-prefixed `Dep`, `InfoSize`, and the `InfoSize`-byte `Info` blob.
- **Payload blobs:** each entry's `[Offset, Offset+Size)` slice, held in the model and re-placed at
  `Offset`. (For an identity round-trip this equals the original slice; the smoke test §5 substitutes
  an edited slice here to prove controllability.)

### PASSED THROUGH verbatim (copied; the writer must PRESERVE or correctly regenerate)
- **Alignment padding only.** Every byte not claimed by a reconstructed region above is inter-payload
  gap padding, and in all 8 files it is **100 % zero**. Totals per file are in the table; e.g.
  textures.big = 6,771,202 zero bytes across 6,317 gaps sitting between the 2048-aligned payloads.
- There are **no non-zero passthrough bytes in any file** — i.e. there is no opaque/unknown region
  the model fails to explain. Structural coverage + zero padding accounts for 100 % of every file.

### Name / Dep string byte-fidelity
`parse_bigb` strips the trailing NUL when decoding length-prefixed `Name`/`Dep` strings. To reproduce
the on-disk **length prefix and any trailing NUL exactly**, the harness re-reads the raw
`u32 len + raw bytes` field from disk (`_augment_entry_raw`, `_raw_lpstr_at`) rather than
re-encoding the stripped string. A C++ writer that edits a name must recompute its length prefix and
match the game's NUL convention (length **includes** the trailing NUL where present) — do not assume
"no NUL". This was the one fidelity trap; handled.

---

## 3. Align / CRC / Timestamp verdict (the [hypothesis] risk fields)

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

**Align — reproduced exactly, and load-bearing.** graphics/textures/frontend/effects use `Align=2048`
(0x800 payload boundaries); fonts mixes 512/2048; shaders `Align=4`; text/dialogue `Align=1` (packed).
The harness does **not** re-derive padding from `Align`; it treats the gap bytes as passthrough and
copies them. Result: every pad byte (all zero, but VERIFIED zero, not assumed) is reproduced. **A real
writer that re-packs payloads MUST pad each payload up to its sub-bank `Align` and write zero fill** —
that reproduces exactly what we observe. `Align` is confirmed real and correct in the footer.

**CRC (TOC +0x14) — PASSED THROUGH verbatim; NEVER recomputed.** Contrary to the earlier "observed
0x00000000" note (which held only for text/dialogue/effects), the CRC field is **non-zero and varied**
in fonts/frontend/graphics/textures/shaders (e.g. `0xae689191`). The harness reproduces it purely by
passthrough of the parsed field value — so byte-exactness is achieved **without knowing the CRC
formula**. This is the correct and safe contract: **the writer must preserve CRC verbatim on any entry
it does not change.** For an entry whose payload IS edited, the CRC formula is **UNVERIFIED** and is a
blocking gap (§4).

**Timestamp (TOC +0x18) — PASSED THROUGH verbatim.** Also non-zero in the graphics-family files
(one value per bank build). Reproduced by passthrough; the writer must preserve it verbatim rather
than regenerate (its role/format is unconfirmed; do not stamp "now").

---

## 4. Blocking gap for a REAL (payload-editing) writer

The gate proves **faithful reproduction and controllability** (identity round-trip + targeted single
edit). It does NOT prove we can regenerate the two opaque per-entry integrity fields when a payload
actually changes:

- **`CRC` recompute formula is UNVERIFIED (BLOCKING for payload edits).** Values are non-zero in 5 of
  8 files, so if any engine/load path validates them, a writer that edits a payload must recompute the
  CRC over the new bytes. Candidate (unproven): seed-0 reflected CRC-32 `CCRC::Calc` (the canonical
  `game.bin`/`GetTextBySymbol` CRC per `TEXTBIG_FORMAT.md` §8), possibly over the payload blob.
  **Action before shipping an edit path:** take one non-zero-CRC entry (e.g. a shaders.big or
  frontend.big entry), run seed-0 `CCRC::Calc` over its `Size` payload bytes, and check it reproduces
  the stored CRC. If it does not, the CRC domain (payload? name? whole record?) is still open — do not
  emit edited payloads until resolved. Until then a writer may only edit entries whose CRC it can leave
  untouched, OR must be limited to files where CRC is 0 (text/dialogue/effects).
- **`Timestamp` semantics unconfirmed** — safe as long as the writer preserves it verbatim (which the
  gate proves is byte-exact). Only becomes a question if a tool wants to *stamp* new builds.

No other blocking gap: there is **zero non-zero passthrough** in any file, so there is no
"copied-blindly opaque region" that could hide a real modification. Every byte is either a
reconstructed structural field or verified-zero alignment padding.

---

## 5. Single-edit smoke test (controllability proof)

To prove the serializer is **model-driven, not an identity `cp`**, the harness edits ONE byte of ONE
payload and confirms ONLY that byte changes. Run on shaders.big (smallest payload picked automatically):

```
=== SMOKE TEST: shaders.big ===
  target payload: subbank[0] id=1 name='PSHADER_DIFFUSE_ONLY' size=29 at 0x5733c
  edited byte at 0x57358: 0x00 -> 0xff
  total differing byte-runs after re-serialize: 1
    diff 0x57358..0x57359 (1B)
  VERDICT: CONTROLLABLE (only the intended byte changed)
```

Exactly one differing byte-run of length 1, at the intended offset — the edit flows through the model
to precisely the targeted byte and nothing else moves. (This edit keeps `Size` constant, so no
dependent `Offset`/`Size`/`FooterOffset` fields need to move; a size-changing edit is a separate
concern that requires the re-pack + CRC path of §4.) The writer is **controllable**, not a copy.

---

## 6. Go / No-Go verdict for building a `.big` writer

- **GO — for an identity/repack writer and for edits that do not alter payload bytes** (e.g.
  re-ordering, metadata the game does not CRC, or edits to the CRC-zero files text/dialogue/effects).
  The read→model→write contract is proven byte-exact on all 8 retail files and is controllable.
- **CONDITIONAL / NO-GO — for a writer that edits payload bytes of a CRC-bearing entry** until the
  `CRC` recompute formula is verified (§4). This is the one remaining blocker and is well-scoped:
  one targeted Ghidra/oracle check of seed-0 `CCRC::Calc` over a known non-zero-CRC entry resolves it.

Everything else the family doc flagged as write-path risk (header back-patch, TOC-chain contiguity,
Align padding, stats-header regeneration, per-entry Info/Deps, name length-prefix/NUL) is
**reproduced byte-exact** and encoded as the contract in §2.

---

## 7. CRC formula — REFUTED as a payload hash; it is a per-format CLASS fingerprint

*Authored 2026-07-19 (verification agent). Tool: `tools/verify_big_crc.py` (pure Python, no Ghidra;
reuses `parse_bigb.py` read-only). Resolves the §4 blocking gap.*

### 7.1 What was tested
The strong hypothesis (§4) was: TOC `CRC` (+0x14) = engine `CCRC::Calc(seed, data, len)` — the
canonical seed-0 reflected CRC-32 (poly `0xEDB88320`, zlib table, **no final inversion**) already
confirmed for `game.bin` field-tags and `text.big` symbols (`FINDINGS.md` ★ CANONICAL CRC REFERENCE;
algorithm re-confirmed here against `ghidra_out/decomp_ccrc_calc.log` + `crc_table_full.txt`,
table[1]==`0x77073096`). The harness computed the **full matrix** over 30 real non-zero-CRC entries
sampled from **5 files** (shaders, frontend, fonts, graphics, textures):

- **60 formulas** = 12 data domains × {seed 0, seed 0xFFFFFFFF} × {no-inversion, final-inversion};
  plus a `zlib.crc32` control.
- **Domains:** `payload`, `name`, `name+NUL`, `subbank name` (+NUL), `dep` build-path (+NUL),
  `Info` descriptor, and the concatenations `name+payload`, `payload+info`, `name+info`,
  `info+payload`.

### 7.2 Result — REFUTED
**No formula matched even ONE of the 30 samples** (0/30 for every domain/seed/inversion, control
included). The seed-0 `CCRC::Calc`-over-payload candidate scored **0/30**. Reproduce:
`python tools/verify_big_crc.py`.

### 7.3 What the CRC actually is (the structural evidence)
The field is **not per-payload, per-name, or per-Info** — it is **constant across all entries that
share a resource FORMAT / serialization CLASS**:

| File | entries (non-zero CRC) | **distinct** CRC values | note |
|------|-----------------------:|------------------------:|------|
| shaders.big | 465 | **1** (`0xb8bc6765`) | one value for ALL 26 sub-banks / every shader |
| frontend.big | 394 | **3** | `0x5701ffde`, `0x4fd8a4a8`, `0x4e1ace9f` |
| textures.big | 6,324 | **29** | groups by texture format |
| fonts.big | 26 | 7 | groups by font-glyph format |
| graphics.big | 8,046 | 1,289 | meshes: many vertex/format classes |

Decisive tells that it is a **class/schema fingerprint**, not content:
- All 465 shaders share ONE CRC despite wildly different bytecode payloads, names, and sizes.
- Within a group the payload, name, size, and the dimension/size bytes of `Info` all vary; only the
  `Info` **format** descriptor (texture pixel-format / mesh vertex-format constant bytes) is shared.
- The value **recurs across files**: `0x4e1ace9f` appears in BOTH `frontend.big` and `textures.big`
  (same GUI texture format in two banks) — impossible for a per-file/per-payload hash.
- graphics.big CRCs correlate with mesh class (e.g. `0x73ab9c0d` covers only types {6,7,9} with
  `InfoSize=24`; `0xae689191` covers types {1–5} with variable `InfoSize`), i.e. the CRC keys off the
  resource's **serialization layout**, exactly like an EgoCore "resource-class version CRC" stamped at
  bank-build time so the loader can reject format-version drift.

It is almost certainly `CCRC::Calc` of the resource **class's field/type descriptor** (a build-time
schema string not present in the `.big` bytes), which is why no on-disk byte domain reproduces it.

### 7.4 Timestamp (TOC +0x18) — opaque, NOT wall-clock
The `Timestamp` u32 is high-entropy and uniformly spread across the full 32-bit range
(frontend: min `24,156,209` … max `4,258,377,908`, adjacent entries decode as unix years
1985 / 2060 / 2093 — nonsensical). It is **not** a real timestamp; treat as an opaque per-entry value.
**Passthrough verbatim** (as §3 already prescribes); never regenerate/stamp "now".

### 7.5 Writer verdict — UPDATED
The good news: because the CRC is a **format-class stamp, not a payload hash, a writer editing an
entry's payload does NOT need to recompute it.** All entries of a given resource format legitimately
carry the same CRC, so an edited payload keeps the **same** CRC as its unedited siblings of that
format — pass it through verbatim.

- **GO — payload-editing writer, CRC passthrough (revised from §6 CONDITIONAL/NO-GO).** Editing a
  payload's bytes (same format) requires **no CRC change**: preserve the stored `CRC` verbatim. The
  round-trip gate already proves verbatim CRC reproduction is byte-exact.
- **Still CONDITIONAL — only if a writer INTRODUCES A NEW resource format/class** (or changes an
  entry's format so its class fingerprint should differ). That needs the build-time class-schema
  string + `CCRC::Calc(?, schema)` to synthesize a fresh stamp — out of scope for ordinary asset
  edits (retexture, mesh-swap, shader-swap keep the format). Defer until a format-authoring tool is
  actually built; a Ghidra trace of who WRITES `CRC` at bank-build would pin the schema source.
- **NO-GO risk retired:** the §4 fear was "editing a payload silently breaks a payload-CRC check."
  That fear is unfounded — there is no payload CRC. The writer is **GO** for all realistic edits.
