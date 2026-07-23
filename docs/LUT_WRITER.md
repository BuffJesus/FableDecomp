# `.lut` audio-bank WRITER — controlled voice-line replacement

*Authored 2026-07-19. Companion to `docs/LUT_AUDIOBANK_FORMAT.md` (the READ/extraction spec)
and `tools/parse_lut.py` (read-only parser). Writer lives in `tools/lut_write.py`; it reuses
`parse_lut.walk()` and never modifies it. All validation is a real re-read + per-clip byte-diff +
ffmpeg decode, not an assertion.*

## Verdict up front

| Capability | Status | Evidence |
|------------|--------|----------|
| **Identity round-trip** (read→rebuild→write, no edit) | **PASS — byte-exact on all 4 retail `.lut`** | §1 |
| **Same-size clip replace** (swap RIFF, identical length) | **PASS — validated** | §2 |
| **Size-change clip replace** (different-length RIFF, full repack) | **PASS — validated** (the tail directory turned out fully reproducible; the format doc's feared "hash array" does not exist) | §3 |
| **Voice modding feasibility** | **GO** — both same-size and size-change replacement produce clean, self-consistent, ffmpeg-decodable banks | §5 |

The one caveat that remains **[unproven]**: none of these outputs has been loaded by the retail
`Fable.exe` engine (no in-game playback test this run; Ghidra lock held by another agent). Byte-exact
identity + a fully self-consistent rebuilt directory is strong static evidence the engine will accept
them, but *engine acceptance is not yet confirmed at runtime.*

---

## 0. What closed the write gap: the tail directory is NOT a hash table

`docs/LUT_AUDIOBANK_FORMAT.md` §3/§5 flagged the tail `LHAudioBankLookupTable` "hash bucket array"
as only partially decoded and "the main writer risk." This run **fully decoded it on all 4 retail
files**, and it is a plain, deterministic `(index, size, offset)` array — no hashing. Layout from
`TocOffset`:

```
TocOffset (header +0x28)  ->  points 44 bytes BEFORE the marker (into the last clip's data;
                              it is NOT a separate padding region — see the identity fix below)
marker    char[32]  "LHAudioBankLookupTable\0..."   (NUL-padded name)
  +0x00 u32  LookupDataSize = 20 + (Count-1)*12      [VERIFIED exact, all 4 files]
  +0x04 u32  MaxEntries     (500 Dialogue / 1000 Script)   — copied verbatim
  +0x08 u32  Count          = clip count (== walk count)
  +0x0C u32  flag           = 1
  +0x10 u32  LookupOffset   = 2nd clip's rec_off-0x2c (single-clip banks: == TocOffset)
  +0x14 u32  reserved       = 0
then (Count-1) × 12-byte records, one per clip EXCEPT the first (index 1 is implicit at
     rec_off 0x2c), in ascending file-position order:
       u32 index         the clip's 1-based ordinal <N>
       u32 record_size = 36 (clip header) + riff_span
       u32 offset      = clip rec_off - 0x2c   (relative to the first clip record)
then a FIXED 560-byte TRAILER, byte-identical across ALL 4 retail files
     (md5 dc639577347ce41e08b8d9f6656532cd): "LHFileSegmentBankInfo" +
     "\Generated bank for Fable" + "NoFPNameYet" + padding, ending 04 00 00 00.
```

Every tail field is therefore a pure function of the clip layout plus **two verbatim constants**
(the 44-byte preamble — which is just the last 44 bytes of the final clip's data — and the 560-byte
trailer). This makes a byte-exact **size-changing** repack deterministic.

**Decode evidence** (per file, all matched 100%):

| File | Count | array recs matched (off = rec_off-0x2c, size = span+36) | `LookupDataSize` == `20+(Count-1)*12` | 560B trailer md5 |
|------|------:|--------------------------------------------------------:|:-------------------------------------:|:----------------:|
| Dialogue.lut | 12134 | 12133 / 12133 | ✅ 145616 | dc639…32cd |
| ScriptDialogue.lut | 5310 | 5309 / 5309 | ✅ 63728 | dc639…32cd |
| ScriptDialogue2.lut | 2769 | 2768 / 2768 | ✅ 33236 | dc639…32cd |
| Dialogue2.lut | 1 | 0 / 0 | ✅ 20 | dc639…32cd |

---

## 1. Identity gate (baseline) — PASS, all 4 files byte-exact

`python tools/lut_write.py IDENTITY <file.lut>` walks the file, rebuilds the header + every clip
record + the tail directory from the parsed layout, back-patches `TocOffset`, and compares to the
original byte-for-byte.

```
[IDENTITY] Dialogue.lut:        268,367,088 -> 268,367,088  byte-exact=True
[IDENTITY] Dialogue2.lut:            37,968 ->     37,968   byte-exact=True
[IDENTITY] ScriptDialogue.lut:  229,244,544 -> 229,244,544  byte-exact=True
[IDENTITY] ScriptDialogue2.lut: 174,964,500 -> 174,964,500  byte-exact=True
```

Two subtleties the gate forced correct:

1. **`TocOffset` points 44 bytes *before* the marker, into the final clip's data** — it is not a
   separate 44-byte preamble region. Emitting it as separate bytes over-counted by 44 on every file.
   The tail proper begins *at* the marker (= end of the last clip record); `TocOffset = marker − 44`.
2. **Single-clip banks store `LookupOffset == TocOffset`** (there is no 2nd clip to point at; the
   field aliases into the preamble). Handled as a single-clip special case.

Passing this gate on the two 200-MB+ banks is the proof the whole model (offsets, sizes, tail
formula, constants) is right.

---

## 2. Same-size clip replace — PASS, validated

The safe case: swap one clip's embedded RIFF/WAVE for another of **identical byte length**. No
offsets move, `TocOffset` and the lookup array are unchanged; only the clip's payload bytes differ.
The 36-byte clip header (const fields, 3D-audio floats, `0xFFFFFFFF` sentinel) is **preserved
verbatim** per the format doc — only the RIFF blob is swapped.

Test: `ScriptDialogue2.lut`, put clip **N=1471**'s RIFF (span 42024) into clip **N=3**'s slot
(also span 42024). Validation (real re-read through `parse_lut`, per-clip byte-diff, ffmpeg):

```
re-read: 2769 clips walked, size 174,964,500 (unchanged)
tail: Count field=2769 == walk  ✅
clip diff: changed=[3]  #identical=2768  ← ONLY the target clip changed
changed clip byte-exact to replacement RIFF: True (42024 B)
ffmpeg decode: ok=True  -> pcm_s16le 22050 Hz mono
lookup-table array self-consistent: True (0 bad recs)
SAME-SIZE VERDICT: PASS
```

Edge case — single-clip `Dialogue2.lut`, replace its only clip with its own RIFF → output byte-exact
to the original. PASS.

---

## 3. Size-change clip replace — PASS, validated (full repack)

Because the tail directory is fully reproducible (§0), a different-length replacement is a complete
deterministic repack: the target record changes length, **every downstream clip offset ripples**, the
lookup array records are regenerated with new offsets/sizes, `LookupDataSize`/`Count` are recomputed,
and `TocOffset` is back-patched. Enabled with `--allow-resize` (default mode refuses a size change).

Test: `ScriptDialogue2.lut`, put clip **N=1**'s RIFF (span 37272) into clip **N=3**'s slot
(orig span 42024, Δ = −4752 bytes):

```
old span 42024 -> new span 37272 (Δ -4752); output size 174,959,748 (= orig -4752)  ✅
re-read: 2769 clips walked, Count field=2769 == walk  ✅
clip diff: changed=[3]  #identical=2768  ← ONLY the target changed
changed clip byte-exact to replacement RIFF: True (37272 B)
downstream ripple: clip N=1 (before target) rec_off unchanged;
                   clips N=4,5,6,… all shifted by exactly -4752  ✅
lookup-table array self-consistent: True (0 bad recs)  ← offsets track the new layout
ffmpeg decode: ok=True -> pcm_s16le 22050 Hz mono
size-change output re-passes IDENTITY rebuild byte-exact (stable fixpoint)  ✅
SIZE-CHANGE VERDICT: PASS
```

The **stable-fixpoint** check (rebuilding the resized output reproduces it byte-for-byte) is the
strongest internal-consistency proof: the writer's own reader agrees the file is well-formed.

---

## 4. What a replacement RIFF must be (encode-side requirement)

The writer does **not** transcode; it embeds whatever RIFF you hand it (after checking `RIFF…WAVE`).
For the retail engine's ADPCM decoder to play it, the replacement must be
`WAVE_FORMAT_XBOX_ADPCM` (fmt tag **0x0069**), mono, **22050 Hz**, block align **36** — same as every
retail clip. Producing that blob is the encode side — **SOLVED 2026-07-20 by `tools/xbadpcm.py`**
(pure-Python Xbox ADPCM encoder; note **ffmpeg has NO `adpcm_ima_xbox` encoder** — it only
decodes, so the earlier `ffmpeg -c:a adpcm_ima_xbox` suggestion here was wrong; the Fable TLC
Toolpack codec is the other option). Codec semantics + validation: `docs/LUG_AUDIO_FORMAT.md`
§8.4. Proven chain: PCM tone → `xbadpcm.encode_wav` → `REPLACE --allow-resize` on a
Dialogue2.lut copy → `parse_lut.py --extract` returns the injected RIFF byte-exact and ffmpeg
decodes it. Container-side, this writer remains the *container* repacker;
A wrong-format RIFF will still pack cleanly and pass the byte-diff/decode checks here, but the game's
decoder path would likely mis-play it — encode correctly.

---

## 5. Voice-replacement feasibility verdict: GO

- **Same-size replace: proven and validated.** The provable, lowest-risk win — swap a voice line for
  a re-encoded clip of the same byte length and only that clip changes; the rest of the 200-MB bank
  is untouched byte-for-byte.
- **Size-change replace: proven and validated.** The container fully supports arbitrary-length
  replacement; the tail directory the format doc feared as a blocker is a plain offset array and is
  regenerated correctly (downstream ripple + self-consistent lookup table + stable fixpoint).
- **`<N>` join preserved.** The writer never renumbers clips; `index` values (and the empty-index
  gaps in `ScriptDialogue2`) are carried through, so the text.big `SpeechBank` `<N>` and dialogue.big
  `LIPSYNC` id still resolve to the replaced clip.

**Remaining risk (honest):** engine runtime acceptance is **[unproven]** — no in-game load test this
run. Recommended next step before shipping: (a) an in-game playback test of a same-size replacement
(lowest risk), then a size-change one; (b) an optional Ghidra pass on the `.lut` loader to confirm
`MaxEntries` is a hard cap (adding *new* indices beyond `MaxEntries` is untested — this writer only
*replaces* existing clips, which is safe against that cap). Lipsync mismatch after a size change is a
cosmetic risk only (dialogue.big stores its own duration); it does not corrupt the bank.

---

## 6. Reproduce

```
# identity gate (byte-exact rebuild)
python tools/lut_write.py IDENTITY "<install>\data\lang\English\ScriptDialogue2.lut"

# same-size replace (refuses if sizes differ)
python tools/lut_write.py REPLACE  "<install>\...\ScriptDialogue2.lut" 3 newclip.wav out.lut

# size-change replace (full repack)
python tools/lut_write.py REPLACE  "<install>\...\ScriptDialogue2.lut" 3 newclip.wav out.lut --allow-resize

# then re-validate through the read-only parser:
python tools/parse_lut.py out.lut
python tools/parse_lut.py out.lut --extract 3 check.wav
ffmpeg -i check.wav check_pcm.wav      # confirms decodable
```

`newclip.wav` must be a standard RIFF/WAVE; for engine playback it must be Xbox ADPCM (0x0069),
mono, 22050 Hz, block align 36 (§4).

## 7. Open / unverified items

- **Engine runtime acceptance** of a written bank — not tested (no in-game load this run).
- **Adding NEW indices** — SHIPPED 2026-07-20: `rebuild(b, add=(N, record))` appends one clip at
  index max+1 (tail Count/LookupDataSize regenerated; staged + fixpoint-validated via
  `tools/dialogue_pipeline.py`). KEY FACT: `MaxEntries` (500/1000) is **already < retail Count**
  (12134/5310) in every shipped bank, so it is NOT a count cap (likely a runtime cache/pool size).
  Engine acceptance of a grown bank remains [unproven] until a live test; filling ScriptDialogue2's
  291 empty gaps is still untested (append-only for now). See docs/DIALOGUE_PIPELINE.md.
- Clip-header fields +0x04..+0x20 remain **[hypothesis]** (per format doc) — the writer preserves the
  original 36-byte header verbatim on replace, so their purpose need not be known.
