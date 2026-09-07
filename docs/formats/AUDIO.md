# Audio banks — `.lut` (dialogue), `.lug` (SFX/ambience), Xbox ADPCM codec, and the dialogue join

*Merged reference (2026-09-07) of `LUT_AUDIOBANK_FORMAT.md` (2026-07-19), `LUT_WRITER.md`
(2026-07-19, ADD 2026-07-20), `LUG_AUDIO_FORMAT.md` (2026-07-20, write side §8 same day) and
`DIALOGUE_PIPELINE.md` (2026-07-20). text.big grammar is in [TEXT.md](TEXT.md); LIPSYNC
(dialogue.big) grammar and composer in [ANIM.md](ANIM.md); the mouth chain in
`docs/engine/DEMON_DOOR_FACE.md`. All of this was cracked byte-level with no Ghidra (the lock was
held by other agents each run); EgoCore has **no audio parsing code**. Fields whose *purpose* is
inferred, not proven, are labeled **[hypothesis]**.*

**Status.** Both bank formats are RE'd and VALIDATED on every retail file (`.lut`: 4 files,
20,213 clips; `.lug`: 50 files, 3,833 records), with byte-exact identity repackers, clip
REPLACE and ADD, a pure-Python Xbox ADPCM encoder, and a one-command dialogue staging pipeline.
The dialogue join is SOLVED: `data\Defs\<bank>snds.bin` = sorted `{crc0("SND_"+entryName),
soundID}`; soundID == `.lut` clip `Index` == dialogue.big LIPSYNC id. **`.lut` `MaxEntries` is NOT
a count cap.** Engine runtime acceptance of written banks awaits a live test.

Tools: `tools/parse_lut.py`, `tools/lut_write.py`, `tools/parse_lug.py`, `tools/lug_build.py`,
`tools/xbadpcm.py`, `tools/dialogue_pipeline.py` (+ `tools/text_build.py`, `tools/lipsync_build.py`,
`tools/big_write.py`).

## 0. The audio file family at a glance

| File | Where | Magic / container | Holds | Codec |
|------|-------|-------------------|-------|-------|
| `Dialogue.lut`, `Dialogue2.lut`, `ScriptDialogue.lut`, `ScriptDialogue2.lut` | `data\lang\<lang>\` | `LiOnHeAdLHAudioBankCompData` | voice/dialogue clips, 1-based `Index` | Xbox IMA ADPCM 0x0069, mono 22050 Hz, block 36 (no PCM) |
| 50 × `*.lug` (+ `.met` sidecars) | `data\Sound\` | `LiOnHeAd` + `LHFileSegmentBankInfo` block stream | per-region/ambience SFX banks + `Ingame.lug` global SFX with event-tag map | Xbox IMA ADPCM (3,773 clips) + 16-bit PCM (51 clips), 22050/44100 Hz, mono/stereo |
| 68 × loose `.ogg` | `data\Sound\` | Vorbis | music, fanfares, cutscene audio | Vorbis (no container) |
| `data\Defs\<bank>snds.bin` | `data\Defs\` | `u32 count` + sorted pairs | name-CRC → soundID join table | — |
| `dialogue.big` `LIPSYNC_*` | `data\lang\<lang>\` | BIGB | 43 fps viseme weight curves + f32 duration | — (see [ANIM.md](ANIM.md)) |

Same family convention throughout: `LiOnHeAd` + 32-byte NUL-padded struct names + embedded
standard RIFF/WAVE files. `text.big` `SpeechBank` strings name `ScriptDialogue.lug/.lut`, but the
dialogue audio is in the `.lut` — no `ScriptDialogue.lug` exists on disk. `LHFileSegmentBankInfo`
also appears as a *sub-struct* inside the `.lut` tail trailer, which is why the name recurs.

Community corroboration (fabletlcmod.com; HTTPS cert fails so search snippets only — flagged):
the `.lut` are audio banks and the **Fable TLC Toolpack ships an ADPCM codec** for them; consistent
with the byte evidence, adds no byte layout.

---

# Part A — `.lut` voice/dialogue banks (`LiOnHeAdLHAudioBankCompData`)

*Read side authored 2026-07-19 (`tools/parse_lut.py`); full parse of all 4 retail files (headers
hexdumped, every clip's RIFF validated, directory decoded). Second source: this project's
dialogue.big `LIPSYNC_*` counts/names/durations (§A.5). Third: ffmpeg 8.1 identifies an extracted
clip as `adpcm_ima_xbox (i[0][0][0] / 0x0069), 22050 Hz, mono` and decodes it.*

## A.1 File inventory (all located, all sized, all parsed)

| File | Path (under Steam install) | Size (bytes) | clips walked | index range | Verdict |
|------|----------------------------|-------------:|-------------:|:-----------:|:-------:|
| `Dialogue.lut` | `data\lang\English\` | 268,367,088 | 12,134 | 1..12134 (contiguous) | CLEAN |
| `Dialogue2.lut` | `data\lang\English\` | 37,968 | 1 | 1..1 | CLEAN |
| `ScriptDialogue.lut` | `data\lang\English\` | 229,244,544 | 5,310 | 1..5310 (contiguous) | CLEAN |
| `ScriptDialogue2.lut` | `data\lang\English\` | 174,964,500 | 2,769 | 1..3060 (291 gaps) | CLEAN |

Across all 4: **20,213 embedded clips**, fmt tag exclusively `0x0069`, sample rate exclusively
22050 Hz, **zero out-of-bounds clips, zero malformed RIFFs**.

## A.2 File header (44 bytes, offset 0)

| Off | Type | Field | Value | Meaning |
|----:|------|-------|-------|---------|
| 0x00 | char[28] | `Magic` | `"LiOnHeAdLHAudioBankCompData\0"` | container magic (27 chars + NUL) |
| 0x1C | u32×3 | reserved | `0` | zero padding |
| 0x28 | u32 | `TocOffset` | file offset of the tail lookup-table directory (§A.4) — points 44 bytes BEFORE the marker |

The first **clip record begins at offset 0x2C**, immediately after `TocOffset`. (The `u32` at 0x2C
— often mis-read as a "sub-bank count" — is actually clip[0]'s `Index` field = `1`.)

## A.3 Clip record (repeats `Count` times, back-to-back from 0x2C)

Each record = a **36-byte clip header** then one complete **RIFF/WAVE** file:

| Off | Type | Field | Typical value | Meaning |
|----:|------|-------|---------------|---------|
| +0x00 | u32 | **`Index`** | 1,2,3,… | **1-based ordinal `<N>`** — the join key (§D.1) |
| +0x04 | u32 | const | `0x56220001` | packed `0x5622`=22050 sample-rate \| `0x0001` channels **[hypothesis]** |
| +0x08 | u32 | const | `0x00019C40` (105536) | constant across all clips **[hypothesis: format/quality id]** |
| +0x0C | u32 | const | `0x01010000` | constant flags **[hypothesis]** |
| +0x10 | u32 | `Flags2` | ~`0x00647F06`, varies | per-clip; low bytes differ **[hypothesis]** |
| +0x14 | f32 | `MinDistance` | 1.5 (Dialogue) / 5.0 (Script) | 3D audio min distance **[hypothesis]** |
| +0x18 | f32 | `MaxDistance` | 18.0 (Dialogue) / 25.0 (Script) | 3D audio max distance **[hypothesis]** |
| +0x1C | u32 | `Priority` | 500 (Dialogue) / 1000 (Script) | priority / range **[hypothesis]** (matches tail `MaxEntries`) |
| +0x20 | u32 | sentinel | `0xFFFFFFFF` | terminator / no-loop **[hypothesis]** |
| +0x24 | RIFF | **`Clip`** | `RIFF….WAVE` | the embedded audio file |

The next record starts immediately after the RIFF (`riff_off + 8 + RIFFsize`). Records are
**packed, no alignment padding** — the walk lands exactly on the next `Index`+RIFF or on the tail
directory. This is how the parser walks the whole file with zero slack. Each record is
`36 + (8+RIFFsize)` bytes.

**Embedded RIFF/WAVE clip** — standard Microsoft RIFF WAVE, decodable by any WAV tool that
supports Xbox ADPCM:
- `RIFF` / `<size>` / `WAVE`
- `fmt ` chunk (size 20): `wFormatTag=0x0069` (**WAVE_FORMAT_XBOX_ADPCM**), `nChannels=1`,
  `nSamplesPerSec=22050`, `nAvgBytesPerSec=12403`, `nBlockAlign=36`, `wBitsPerSample=4`,
  `cbSize=2`, extra = `0x0040` (64 samples/block).
- `data` chunk = the Xbox-ADPCM audio payload.

Clip **duration** = `data_size / nAvgBytesPerSec`. Verified to match dialogue.big's stored lipsync
duration for every sampled `<N>` (§A.5).

> Several early/placeholder clips (e.g. `Dialogue.lut` N=1) have a `data` chunk that is all zeros
> or silent lead-in; these are still valid, ffmpeg-decodable RIFFs. `Dialogue2.lut` /
> `ScriptDialogue2.lut` N=1 contain real audio.

## A.4 Tail directory (at `TocOffset`) — FULLY DECODED, NOT a hash table

The 2026-07-19 read pass flagged the tail `LHAudioBankLookupTable` "hash bucket array" as only
partially decoded and "the main writer risk". The 2026-07-19 writer run **fully decoded it on all 4
retail files**: a plain, deterministic `(index, size, offset)` array — no hashing. Layout from
`TocOffset`:

```
TocOffset (header +0x28)  ->  points 44 bytes BEFORE the marker (into the last clip's data;
                              it is NOT a separate padding region — see the identity fix below)
marker    char[32]  "LHAudioBankLookupTable\0..."   (NUL-padded name)
  +0x00 u32  LookupDataSize = 20 + (Count-1)*12      [VERIFIED exact, all 4 files]
  +0x04 u32  MaxEntries     (500 Dialogue / 1000 Script)   — copied verbatim; NOT a count cap (§D.1)
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

Header field values as read 2026-07-19:

| Type | Field | Dialogue.lut | ScriptDialogue.lut |
|------|-------|-------------:|-------------------:|
| u32 | `LookupDataSize` | 145616 | 63728 |
| u32 | `MaxEntries` | 500 | 1000 |
| u32 | **`Count`** | 12134 | 5310 |
| u32 | `flag` | 1 | 1 |
| u32 | `LookupOffset` | — | 0x14334 |
| u32 | reserved | 0 | 0 |

Every tail field is therefore a pure function of the clip layout plus **two verbatim constants**
(the 44-byte preamble — which is just the last 44 bytes of the final clip's data — and the
560-byte trailer). This makes a byte-exact **size-changing** repack deterministic.

**Decode evidence** (per file, all matched 100%):

| File | Count | array recs matched (off = rec_off-0x2c, size = span+36) | `LookupDataSize` == `20+(Count-1)*12` | 560B trailer md5 |
|------|------:|--------------------------------------------------------:|:-------------------------------------:|:----------------:|
| Dialogue.lut | 12134 | 12133 / 12133 | ✅ 145616 | dc639…32cd |
| ScriptDialogue.lut | 5310 | 5309 / 5309 | ✅ 63728 | dc639…32cd |
| ScriptDialogue2.lut | 2769 | 2768 / 2768 | ✅ 33236 | dc639…32cd |
| Dialogue2.lut | 1 | 0 / 0 | ✅ 20 | dc639…32cd |

For **extraction**, walking clip records linearly by the +0x00 `Index` field is sufficient and
fully validated — the tail lookup table is the engine's fast runtime resolver.

## A.5 `.lut` ↔ dialogue.big pairing (PROVEN) and durations

The four `.lut` files pair one-to-one with dialogue.big's four `LIPSYNC_*` sub-banks, and the clip
`Index` == the LIPSYNC entry `ID` == the `<N>` in the entry name `Dialogue_<N>` /
`ScriptDialogue_<N>`:

| `.lut` file | clips (real) | dialogue.big sub-bank | LIPSYNC entries | match |
|-------------|-------------:|-----------------------|----------------:|:-----:|
| `Dialogue.lut` | 12,134 | LIPSYNC_ENGLISH_MAIN | 12,134 | ✅ exact |
| `Dialogue2.lut` | 1 | LIPSYNC_ENGLISH_MAIN_2 | 1 | ✅ exact |
| `ScriptDialogue.lut` | 5,310 | LIPSYNC_ENGLISH_SCRIPT | 5,310 | ✅ exact |
| `ScriptDialogue2.lut` | 2,769 + **291 gaps = 3,060** | LIPSYNC_ENGLISH_SCRIPT_2 | 3,060 | ✅ (gaps = empty entries) |

`ScriptDialogue2.lut`'s 291 missing indices (first missing = **2**) correspond exactly to
dialogue.big's `Size=0` empty LIPSYNC entries (`ScriptDialogue2_2[Empty]`): a reserved index with
no clip. `2769 present + 291 empty = 3060` closes the count.

**Duration cross-check (the clincher).** For sampled `ScriptDialogue_<N>`, the lipsync duration
stored in dialogue.big's per-entry `Info` float equals the `.lut` clip duration
(`data_size/avg_bps`):

| `<N>` | dialogue.big lipsync dur | `.lut` clip dur |
|------:|-------------------------:|----------------:|
| 1 | 6.664 s | 6.66 s |
| 2 | 3.184 s | 3.18 s |
| 3 | 3.242 s | 3.24 s |
| 4 | 1.271 s | 1.27 s |
| 5 | 4.165 s | 4.17 s |
| 101 | 3.823 s | 3.82 s |
| 1001 | 1.959 s | 1.96 s |

Two independently-authored files agreeing on the duration of the same `<N>` proves both (a) the
pairing is real and (b) the `.lut` clip-offset parse is correct. How a text entry reaches `<N>` is
§D.1 (snds.bin — **not** an ordinal within text.big).

## A.6 `.lut` writer — `tools/lut_write.py`

*Authored 2026-07-19 (ADD 2026-07-20). Reuses `parse_lut.walk()` and never modifies it. All
validation is a real re-read + per-clip byte-diff + ffmpeg decode, not an assertion.*

| Capability | Status | Evidence |
|------------|--------|----------|
| **Identity round-trip** (read→rebuild→write, no edit) | **PASS — byte-exact on all 4 retail `.lut`** | A.6.1 |
| **Same-size clip replace** (swap RIFF, identical length) | **PASS — validated** | A.6.2 |
| **Size-change clip replace** (different-length RIFF, full repack) | **PASS — validated** (the tail directory is fully reproducible; the feared "hash array" does not exist) | A.6.3 |
| **ADD new index** (`rebuild(b, add=(N, record))`) | **SHIPPED 2026-07-20** — appends one clip at index max+1, tail Count/LookupDataSize regenerated; staged + fixpoint-validated via `dialogue_pipeline.py` | §D.3 |
| **Voice modding feasibility** | **GO** — same-size and size-change replacement produce clean, self-consistent, ffmpeg-decodable banks | A.6.4 |

### A.6.1 Identity gate — PASS, all 4 files byte-exact

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
   separate 44-byte preamble region. Emitting it as separate bytes over-counted by 44 on every
   file. The tail proper begins *at* the marker (= end of the last clip record);
   `TocOffset = marker − 44`.
2. **Single-clip banks store `LookupOffset == TocOffset`** (there is no 2nd clip to point at; the
   field aliases into the preamble). Handled as a single-clip special case.

Passing this gate on the two 200-MB+ banks is the proof the whole model (offsets, sizes, tail
formula, constants) is right.

### A.6.2 Same-size clip replace — PASS

Swap one clip's embedded RIFF/WAVE for another of **identical byte length**. No offsets move,
`TocOffset` and the lookup array are unchanged; only the clip's payload bytes differ. The 36-byte
clip header (const fields, 3D-audio floats, `0xFFFFFFFF` sentinel) is **preserved verbatim** — only
the RIFF blob is swapped.

Test: `ScriptDialogue2.lut`, put clip **N=1471**'s RIFF (span 42024) into clip **N=3**'s slot
(also span 42024):
```
re-read: 2769 clips walked, size 174,964,500 (unchanged)
tail: Count field=2769 == walk  ✅
clip diff: changed=[3]  #identical=2768  ← ONLY the target clip changed
changed clip byte-exact to replacement RIFF: True (42024 B)
ffmpeg decode: ok=True  -> pcm_s16le 22050 Hz mono
lookup-table array self-consistent: True (0 bad recs)
SAME-SIZE VERDICT: PASS
```
Edge case — single-clip `Dialogue2.lut`, replace its only clip with its own RIFF → output
byte-exact to the original. PASS.

### A.6.3 Size-change clip replace — PASS (full repack, `--allow-resize`)

A different-length replacement is a complete deterministic repack: the target record changes
length, **every downstream clip offset ripples**, the lookup array records are regenerated with new
offsets/sizes, `LookupDataSize`/`Count` are recomputed, and `TocOffset` is back-patched. Default
mode refuses a size change.

Test: `ScriptDialogue2.lut`, put clip **N=1**'s RIFF (span 37272) into clip **N=3**'s slot (orig
span 42024, Δ = −4752 bytes):
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
strongest internal-consistency proof.

### A.6.4 Verdict & requirements

- **Same-size replace: proven** — the lowest-risk win; the rest of the 200-MB bank is untouched.
- **Size-change replace: proven** — the container fully supports arbitrary-length replacement.
- **`<N>` join preserved.** The writer never renumbers clips; `index` values (and the empty-index
  gaps in `ScriptDialogue2`) are carried through, so text.big/snds.bin `<N>` and the dialogue.big
  LIPSYNC id still resolve to the replaced clip.
- **Replacement RIFF requirement (encode side):** the writer does **not** transcode; it embeds
  whatever RIFF you hand it (after checking `RIFF…WAVE`). For the retail engine's ADPCM decoder the
  replacement must be `WAVE_FORMAT_XBOX_ADPCM` (fmt tag **0x0069**), mono, **22050 Hz**, block
  align **36** — same as every retail clip. Produce it with `tools/xbadpcm.py` (Part C). A
  wrong-format RIFF will still pack cleanly and pass the byte-diff/decode checks, but the game's
  decoder path would likely mis-play it. (No retail `.lut` clip is PCM, so for dialogue stick to
  XADPCM.) Proven chain: PCM tone → `xbadpcm.encode_wav` → `REPLACE --allow-resize` on a
  Dialogue2.lut copy → `parse_lut.py --extract` returns the injected RIFF byte-exact and ffmpeg
  decodes it.
- A byte-exact writer must preserve: `TocOffset` back-patch; packed records (no alignment); the
  clip-header const fields (+0x04 `0x56220001`, +0x08 `0x00019C40`, +0x0C `0x01010000`, +0x20
  `0xFFFFFFFF`) and 3D-audio floats/priority re-emitted verbatim (their engine role is
  **[hypothesis]**); the tail table with `Count` == clip count; `Index` continuity and the
  `ScriptDialogue2` empty slots (meaningful gaps, not removable).
- Lipsync mismatch after a size change is a cosmetic risk only (dialogue.big stores its own
  duration); it does not corrupt the bank.

### A.6.5 Reproduce

```
python tools/parse_lut.py "<install>\data\lang\English\Dialogue.lut"            # header + walk + codec + join stats
python tools/parse_lut.py "<install>\data\lang\English\ScriptDialogue.lut" --list
python tools/parse_lut.py "<install>\data\lang\English\Dialogue.lut" --dump 5
python tools/parse_lut.py "<install>\data\lang\English\ScriptDialogue.lut" --extract 5 clip5.wav
ffmpeg -i clip5.wav clip5_pcm.wav        # decodes Xbox ADPCM -> PCM (confirms playable)

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
Extracted files are standard RIFF/WAVE (fmt 0x0069). Any Xbox-ADPCM-aware player (ffmpeg, VLC
with the codec, the Fable TLC Toolpack) plays them. Always run against COPIES of retail files.

---

# Part B — `.lug` SFX / ambience banks (`LiOnHeAd` + `LHFileSegmentBankInfo`)

*Authored 2026-07-20. Validated against **all 50 retail `.lug` files** under `data\Sound\` (3,833
sample-table entries, 3,545 unique embedded RIFF/WAVE clips, every byte of every file walked with
zero slack). Parser `tools/parse_lug.py`; writer `tools/lug_build.py`. Evidence: raw retail bytes
(+ paired `.met`), the engine's own struct names, the `.lut` sibling (identical codec: fmt 0x0069,
block align 36, 64 samples/block; identical `name[32]+size` block convention), ffmpeg 8.1 as codec
oracle, and the `.met` sidecars whose dev paths (e.g.
`S:\BigBlueBox\Fable atmos\Waterfall\1644XBADPCM\WaterfallLargeMono_01.wav`) literally name
`XBADPCM` / `xbadpcm1644` directories.*

## B.1 Verdict & inventory

- **What `.lug` holds:** per-region/ambience **sound-effect banks** (footsteps, cowbells, wind,
  creature vocalisations, GUI clicks…). NOT music (loose `.ogg`), NOT dialogue (`.lut`).
- 50 `.lug` files, 109 KB (`Waterfall.lug`) to 43 MB (`Ingame.lug`, 2,354 entries — the global
  SFX bank with the criteria/event map). Each has a matching `.met`. The directory also holds 68
  loose `.ogg` (music, fanfares, cutscene audio — ordinary Vorbis, no container).
- **Codec (empirical, all 3,824 non-empty entries):** `WAVE_FORMAT_XBOX_ADPCM` (0x0069) 4-bit for
  3,773 clips; plain PCM (0x0001) 16-bit for 51 clips. 22050 or 44100 Hz, mono or stereo. No OGG,
  no XMA, nothing else.
- **Audio location: EMBEDDED.** Extraction = copy `riffSize` bytes at `waveDataStart + riffOffset`.
- **Extraction PROVEN.** `parse_lug.py --extract / --extract-all` produces standalone `.wav`
  files; PCM ones decode with Python stdlib `wave`, ADPCM ones with ffmpeg as `adpcm_ima_xbox`.

## B.2 On-disk layout

### B.2.1 Block stream

```
0x00  char[8]  "LiOnHeAd"
0x08  blocks until EOF, each:
      char[32]  structName   (NUL-padded)
      u32       payloadSize
      u8[payloadSize] payload
```
The walk consumes every retail file exactly (validated ×50). Block order is always:
`LHFileSegmentBankInfo`, `LHAudioWaveData`, `LHAudioBankSampleTable`, `LHAudioBankCriteiaInfo`
(sic — the engine's own typo).

### B.2.2 `LHFileSegmentBankInfo` (payload size 0x208 in all retail files)

A single NUL-padded string field: the bank title — `"Big Blue Box - Fable - Ingame SFX"` in
`Ingame.lug`; the literal placeholder `"Bank title/description"` in the ambience banks.

### B.2.3 `LHAudioWaveData` — the audio

Payload = complete **RIFF/WAVE files packed back-to-back**, no per-clip header, no padding. Each
RIFF is standard (`RIFF <size> WAVE`, `fmt ` chunk, `data` chunk):
- **Xbox ADPCM clips** (98.7%): `fmt ` size 20, `wFormatTag=0x0069`, 1–2 ch, 22050 or 44100 Hz,
  `nBlockAlign=36*ch`, `wBitsPerSample=4`, `cbSize=2`, extra=`0x0040` (64 samples/block) — byte
  identical convention to the `.lut` dialogue clips.
- **PCM clips** (1.3%, GUI sounds in `Frontend.lug`, one in `woodland.lug`, 49 more in
  `night.lug`/`Prison.lug` etc.): `fmt ` size 16, `wFormatTag=0x0001`, 16-bit.

Clip duration = `dataSize / nAvgBytesPerSec` (or table `riffSize/avgBps` to good approximation).

### B.2.4 `LHAudioBankSampleTable` — the directory

```
u16 count            number of 652-byte records
u16 unk              == non-empty record count in most ambience banks; 0 in Frontend,
                     205 in Ingame — purpose unknown [hypothesis]
record[count], each 652 bytes:
  +0    char[260] sourcePath   dev-machine WAV path (S:\BigBlueBox\...), NUL-padded
  +260  u32  id          1-based sample id (contiguous 1..count in retail)
  +264  u32  waveId      id of the wave blob; ALIASED entries (different id, same clip)
                         share waveId; ordered by wave offset [hypothesis: pre-dedup id]
  +268  u32  riffSize    total bytes of the embedded RIFF (RIFF hdr incl.)
  +272  u32  riffOffset  offset within the LHAudioWaveData payload
  +276  u32  unk0        0/1/2/4 [hypothesis]
  +280  u32  unk1        0x10000 / 0 / 40000 … [hypothesis]
  +284  u32×2 zero
  +292  u16  fmtTag      0x0069 Xbox ADPCM | 0x0001 PCM   (mirrors the RIFF fmt chunk)
  +294  u16  channels
  +296  u32  sampleRate
  +300  u32  avgBytesPerSec
  +304  u16  blockAlign
  +306  u16  bitsPerSample
  +308  u32  zero
  +312  u32×2 0xFFFFFFFF
  +320  char[256] group  category name ("Arena", "Balverine", …; empty in ambience banks)
  +576  76 bytes playback params [hypothesis, from value survey across all 3,833 records]:
        +576 u32 priority-like (1/300/1000…), +580 u32 flags (436/1056/1057…),
        +604/+608/+612 u32 volume/pitch variation percents (0..140),
        +616 f32 minDistance (3.0/5.0…), +620 f32 maxDistance (25.0/35.0…),
        +628 u32 0/2/3, +632 u32 mostly 1, +636 u32 -1/4000/800/10000
```

- **Empty slots:** 9 records across 6 banks (DarkWood, DarkWoodLake, HangingTreePath,
  HookCoastVillage, Lost_Bay, Snowspire) have `riffSize=0` — reserved ids with no clip (same idea
  as the `.lut` `ScriptDialogue2` gaps).
- **Aliases:** 288 records point at a RIFF also pointed at by another record (dedup — e.g. the
  same footstep clip reused under several ids). Set of (offset,size) pairs in the table == set from
  an independent RIFF walk of the wave block, all 50 files.

### B.2.5 `LHAudioBankCriteiaInfo` — event-tag → sample map

```
u32 count
count × { u32 len; char[len] tag; u32 n; u32 sampleId[n] }
```
Tags are semicolon-joined criteria, e.g. `SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS` → 10 sample ids (the
footstep variations the engine picks among), or `CS_CREATE_PARTICLE_ENFLAME_RING_RELEASE_LEVEL_1` →
1 id. `Ingame.lug` has 1,490 tags; region ambience banks have 0–7 (`count=0` payload is just the
u32). This is the engine's sound-event resolution table — how gameplay code (surface material,
creature, spell…) selects a sample id. Total across all banks: 1,497 tags.

### B.2.6 `.met` sidecar (build metadata, not required)

Small binary per bank: `u32×3` (1,1,1) header, then per sample: `u32 pathLen + path` (dev source
WAV), followed by the same size/format/params fields seen in the 652-byte table record (riffSize,
fmt 0x0069, rate, the 0xFFFFFFFF pair, priority, min/max distance floats…). It mirrors the sample
table minus the wave data — evidently the bank compiler's input/manifest. Not parsed further
[hypothesis: exact field order not mapped; redundant with the `.lug` itself]. The writer does NOT
regenerate it — presumed unread by the retail engine **[hypothesis]**.

## B.3 Codec stats (whole corpus, sample tables cross-checked vs RIFF fmt chunks)

| codec | ch | rate | bits | clips |
|-------|---:|-----:|-----:|------:|
| XBOX_ADPCM (0x0069) | 1 | 22050 | 4 | 2,623 |
| XBOX_ADPCM | 1 | 44100 | 4 | 1,091 |
| XBOX_ADPCM | 2 | 22050 | 4 | 33 |
| XBOX_ADPCM | 2 | 44100 | 4 | 26 |
| PCM (0x0001) | 1 | 22050 | 16 | 29 |
| PCM | 1 | 44100 | 16 | 22 |

3,833 table entries = 3,824 clips + 9 empty slots; 3,545 unique RIFFs (288 aliases); ~125.5
minutes of unique audio.

## B.4 Read-side validation (2026-07-20) & reproduce

- **Container walk:** all 50 files parse with zero trailing slack; independent RIFF walk of every
  wave block matches the sample table (as sets, allowing aliases/empties) — 50/50 CLEAN.
- **PCM decode:** all 7 `Frontend.lug` clips open and read fully with Python stdlib `wave`.
- **ADPCM decode:** full-bank sweep over `Ingame.lug`: **2,354 / 2,354 extracted clips decode with
  zero errors** (`ffmpeg -v error -f null`).

```
python tools/parse_lug.py "<install>\data\Sound\village.lug"              # summary + full validation
python tools/parse_lug.py "<install>\data\Sound\Ingame.lug" --list       # per-entry table
python tools/parse_lug.py "<install>\data\Sound\Ingame.lug" --criteria   # event-tag map
python tools/parse_lug.py "<install>\data\Sound\village.lug" --extract 1 cowbell.wav
python tools/parse_lug.py "<install>\data\Sound\Ingame.lug" --extract-all outdir\
ffmpeg -i cowbell.wav out.wav      # Xbox ADPCM -> PCM (or play PCM clips directly)
```

## B.5 `.lug` writer — `tools/lug_build.py` (SHIPPED 2026-07-20)

Imports `parse_lug` for read/validation and `xbadpcm` for codec. All validation is a real re-parse
through `parse_lug.LugBank.validate()` (zero-slack walk + independent RIFF walk vs table) plus
per-clip byte-diffs and an ffmpeg/stdlib-`wave` decode of the extracted result.

| Capability | Status | Evidence |
|------------|--------|----------|
| **IDENTITY** (read→rebuild, no edit) | **PASS — byte-exact on ALL 50 retail `.lug`** (`IDENTITY-ALL`) | gold oracle run 2026-07-20; includes alias banks (Ingame, 288 aliases) and empty-slot banks (DarkWood…) |
| **REPLACE** existing clip (any size, ADPCM or PCM) | **PASS** | village.lug id 1 (ADPCM via `--encode`) and id 2 (plain PCM): only the target id's clip changed (78/79 byte-identical), extracted clip == input RIFF, ffmpeg/`wave` decode OK |
| **REPLACE aliased clip** | **PASS** (all alias ids follow the blob — by design) | Ingame.lug id 145: changed = {145,146} exactly (its alias pair), 1,490 criteria tags carried verbatim |
| **ADD** new clip + record + criteria tag | **PASS** | village.lug: new id 80 appended, table count 79→80, tag `SI_HERO;SE_FORGE_TEST` → [80] resolves on re-parse, clip extracts byte-exact + decodes |
| **Stable fixpoint** | **PASS** | every written output re-passes IDENTITY byte-exact |

**How the rebuild works (surgical raw-byte model):**
- Block payloads are carried **verbatim** except: the wave payload is re-emitted from the unique
  RIFF blobs (original offset order; replacements swap a blob in place, adds append), and each
  652-byte record gets `riffSize`/`riffOffset` re-patched. Downstream offsets ripple
  deterministically — reproducing retail byte-for-byte on all 50 banks proves the wave blob really
  is back-to-back with no hidden padding/ordering rule.
- **Replace** also refreshes the record's wave-format mirror (+292..+307) from the new RIFF's
  `fmt ` chunk, on every alias record. Aliases share the blob, so replacing one id changes all ids
  in its alias group (there is no per-id storage to de-alias without growing the bank — if needed,
  ADD a new clip instead).
- **Add** clones a template record (`--like ID`, default = last non-empty record) for the +576..
  playback-param block, then overwrites path (`--path`, default `S:\Forge\<file>`), `id` = max+1,
  `waveId` = max+1 (new blob appended last, preserving the waveId-follows-offset retail order),
  format fields, optional `--group`. The table header `unk` u16 is bumped only if it equaled the
  non-empty count (its observed invariant), else preserved. `--tag "A;B;C"` appends a criteria entry
  `tag -> [newId]`.
- **Empty slots** (riffSize=0) are preserved verbatim (never re-patched).
- Input WAVs are canonicalized to minimal retail-style RIFFs (PCM16 fmt-16 / XADPCM fmt-20,
  `fmt `+`data` only); `--raw` embeds verbatim; `--encode` transcodes PCM16 → Xbox ADPCM.
- Plain **PCM16 works without any encoder**: 51 retail `.lug` clips are plain PCM, so the engine's
  `.lug` path demonstrably handles PCM records — `lug_build.py` accepts PCM16 WAVs directly.

```
python tools/lug_build.py IDENTITY-ALL "<install>\data\Sound"          # gold oracle, 50/50
python tools/lug_build.py REPLACE copy.lug 1 new_pcm.wav out.lug --encode   # PCM->XADPCM swap
python tools/lug_build.py REPLACE copy.lug 2 new_pcm.wav out.lug            # plain-PCM swap
python tools/lug_build.py ADD copy.lug new.wav out.lug --encode --group Forge \
       --tag "SI_HERO;SE_MY_EVENT" [--like 5] [--path "S:\\Forge\\my.wav"]
python tools/parse_lug.py out.lug            # full cross-check validation
python tools/parse_lug.py out.lug --extract 80 check.wav && ffmpeg -i check.wav -f null -
```
Always run against COPIES of retail files.

---

# Part C — Xbox IMA ADPCM codec: `tools/xbadpcm.py`

**ffmpeg 8.1 decodes `adpcm_ima_xbox` but ships NO encoder for it** (`ffmpeg -h
encoder=adpcm_ima_xbox` → "no encoders … available"). `tools/xbadpcm.py` is a pure-Python
encoder/decoder (`xbadpcm.py encode/decode in.wav out.wav`; API `xbadpcm.encode_wav`). Decode
semantics were pinned **byte-exact against ffmpeg** on retail clips (mono 22050 + 44100, stereo
22050; e.g. 442,240/442,240 samples identical on village.lug id 79):

- Block = `36*ch` bytes: per channel `{i16 predictor, u8 stepIndex, u8 0}` header, then 8 groups
  × ch × 4-byte words of nibbles (LOW nibble first, channels interleaved per word).
- **64 samples out per block per channel = the header predictor sample + the first 63 nibble
  expansions; the 64th nibble's sample is discarded** (hence `avgBytesPerSec = rate*blockAlign/64`
  exactly, floored: 12403 @ 22050 mono — the writer emits this retail-exact).
- Nibble expansion is the *precise* IMA variant: `diff = ((2*(n&7)+1)*step)>>3`.
- Retail blocks are **independent** (predictor = exact source sample s0 of the block; state does
  NOT continue across blocks — verified: 0/926 next-block headers equal the continued decode).
- Encoder convention: header = exact s0 + running step index; 63 nibbles encode s1..s63; the spare
  64th nibble encodes the next block's s0 (output discarded by decoders). Input padded with zeros
  to whole blocks.
- Quality (encode → ffmpeg decode vs source): 32–36 dB SNR on tones, 22–34 dB re-encoding retail
  material — normal for 4-bit IMA ADPCM.

The other encode option is the community Fable TLC Toolpack ADPCM codec.

---

# Part D — The dialogue join and the custom-dialogue pipeline

*2026-07-20. Join model SOLVED (empirical, 2-source rule, no Ghidra); pipeline SHIPPED
(`tools/dialogue_pipeline.py`); REPLACE and ADD both staged and statically validated; engine
runtime acceptance awaits a live test.*

## D.1 THE JOIN — `data\Defs\<bank>snds.bin`

text.big's type-0 payload stores only the bank NAME (`SpeechBank = "ScriptDialogue.lug"`) — **the
clip index `<N>` is not in text.big, the .lut, or dialogue.big. It lives in a fourth file family:
`data\Defs\<bank>snds.bin`**, a compiled name→index table:

```
u32 count
count × { u32 crc, u32 soundID }        # sorted ascending by crc (binary-searchable)
crc = CRC-32(poly 0xEDB88320, seed 0, NO final xor) of "SND_" + <text entry Name>
      (= the project's crc0(), same hash as game.bin def field tags)
```

| snds.bin | pairs | matched by a text.big name | orphans* | = .lut clips | id range |
|---|---:|---:|---:|---:|---|
| dialoguesnds.bin | 12,134 | 12,121 | 13 | 12,134 (Dialogue.lut) | 1..12134 |
| dialoguesnds2.bin | 1 | 0 | 1 | 1 (Dialogue2.lut) | 1..1 |
| scriptdialoguesnds.bin | 5,310 | 5,263 | 47 | 5,310 (ScriptDialogue.lut) | 1..5310 |
| scriptdialoguesnds2.bin | 2,769 | 2,704 | 65 | 2,769 (ScriptDialogue2.lut, 291 gaps) | 1..3060 |

\* orphans = pairs whose CRC matches no current text entry name (renamed/cut lines); crc and
soundID are unique within every file; pair count == real clip count exactly.

**Full runtime chain:**

```
quest script / gossip system
   │  text entry NAME (compiled into Fable.exe quest code / def data)
   ▼
text.big  TEXT_ENGLISH_MAIN entry ──── SpeechBank "ScriptDialogue.lug" (bank only)
   │  crc0("SND_" + Name)  ──────────► data\Defs\scriptdialoguesnds.bin ► soundID <N>
   ▼
ScriptDialogue.lut  clip header Index == <N>   (embedded Xbox-ADPCM RIFF = the voice)
dialogue.big  LIPSYNC_ENGLISH_SCRIPT entry id == <N>, name "ScriptDialogue_<N>"
              (43 fps viseme weight curve + f32 duration → phoneme poses, DEMON_DOOR_FACE.md §5)
```

**Evidence (all empirical, 2-source rule):**
1. **CRC hit rate.** `crc0("SND_"+Name)` of text entries with `SpeechBank` set resolves in the
   matching snds.bin: 12,121/12,129 (Dialogue.lug), 5,263/6,325 (ScriptDialogue.lug), 2,704/2,738
   (ScriptDialogue2.lug). Misses are visibly audio-less lines (GUI help, quest-log names/descs,
   `TEXT_QST_LOG_STORY_PLATINUM_*`) — SpeechBank set by authors, audio never recorded. Zero false
   positives (every resolved map is injective).
2. **Duration correlation (the clincher).** Text length (chars) vs the LIPSYNC duration of the
   resolved `<N>`: r = 0.77 (MAIN), 0.84 (SCRIPT), 0.86 (SCRIPT_2). Under the
   previously-hypothesized "ordinal within group" join the same metric is r ≈ 0.00 — ordinal is
   REFUTED, snds.bin is the join.
3. **EgoCore cross-check.** EgoCore `TextBackend.h ResolveAudioID()` resolves
   `(SND_|TEXT_SND_)<Identifier> = <N>` from `dialoguesnds.h`/`scriptdialoguesnds.h` enum headers
   — the .h source form of the same tables (BankCreator emitted both; retail ships only the
   compiled .bin). `RetailHeaders\dialogue_lipsync.h` (`Dialogue_1 = 1`) is the LIPSYNC-side enum
   of the same ids.
4. **Engine-side strings.** `Fable.exe` contains the standalone prefix string `"SND_"` (runtime
   key construction) plus literal `SND_TEXT_QST_TUTORIAL_*` names; game.bin's `names.bin`
   references `gamesnds.h` / `dialoguesnds.h` / `scriptdialoguesnds(2).h` (the def system declares
   these enum tables). No Ghidra pass yet — §D.6.
5. **Worked resolve:** `TEXT_QST_088_EAT_PIES_INTRO_40` ("I want beefy! Blubbery! Plump! Porcine!",
   Barrow Fields demon door) → crc `C699DB84` → scriptdialoguesnds.bin → **N=5080** →
   ScriptDialogue.lut clip 5080 (96,096 B RIFF) ↔ `ScriptDialogue_5080` lipsync dur 7.744 s. Clip
   durations and lipsync durations agree bank-wide (§A.5).

**`MaxEntries` is NOT a count cap.** The `.lut` tail `MaxEntries` (500 Dialogue / 1000 Script) —
the feared ADD blocker — is **less than the retail `Count`** (12,134 / 5,310) in every shipped
bank, so it cannot cap the entry count. (Likely a runtime cache/pool size; exact meaning still
open, §D.6.) This unblocks ADD-new-index structurally.

## D.2 ADD vs REPLACE verdict

- **REPLACE (donor line) = the honest, proven v1.** Every artifact edit re-uses a mechanism
  already byte-exact-validated (text set, .lut clip replace, LIPSYNC payload edit); the join table
  is untouched; the line already has a retail trigger. Only the generic "writer outputs not yet
  loaded by the live engine" residual applies.
- **ADD (new line) = staged and structurally clean, engine acceptance [unproven].** All four
  artifacts stage and re-validate (new text id, sorted snds.bin pair, .lut clip at index max+1
  with regenerated tail, LIPSYNC entry id max+1). Unknowns that only a live test settles: (a)
  engine tolerance of grown .lut Count, (b) whether the snds.bin loader accepts a grown table (it
  should — plain sorted array), (c) a NEW line also needs a *trigger* (quest scripts are compiled
  C++ — FSE Lua or a donor quest hook), which is outside these four files. Filling
  ScriptDialogue2's 291 empty gaps is untested (append-only for now).

## D.3 The pipeline tool — `tools/dialogue_pipeline.py`

```
# inspect the join for any voiced line
python tools/dialogue_pipeline.py resolve TEXT_QST_088_EAT_PIES_INTRO_40

# synthesize a placeholder voice clip
python tools/dialogue_pipeline.py makewav test.wav --seconds 3

# REPLACE a donor line (v1 path)
python tools/dialogue_pipeline.py stage --wav test.wav \
    --text "New line text." --speaker "DEMON DOOR" \
    --replace TEXT_QST_088_EAT_PIES_INTRO_40 --out work/dialogue_pipeline/my_line

# ADD a brand-new line
python tools/dialogue_pipeline.py stage --wav test.wav \
    --text "New line text." --speaker "DEMON DOOR" \
    --add TEXT_MY_NEW_LINE --bank ScriptDialogue2.lug --out work/dialogue_pipeline/new
```

What `stage` does (all against COPIES; retail opened read-only; prints a deploy checklist):
1. **Audio**: input WAV (PCM16, any rate/channels) → downmix + linear-resample to mono 22,050 Hz →
   `xbadpcm.encode_wav` → retail-layout Xbox-ADPCM RIFF; duration = dataSize/12403.
2. **text.big**: `text_build.set_text` (replace: text+speaker) or `add_text` (add: +SpeechBank).
   Speaker must be one of the 379 NarratorList names (enforced).
3. **snds.bin** (ADD only): insert `{crc0("SND_"+name), N}` keeping the crc sort; N = max(existing
   ids, lut indices)+1. REPLACE copies it verbatim into the staging set (mapping already retail).
4. **.lut**: `lut_write.rebuild(replace=)` or `rebuild(add=)` (appends one record — template 36-B
   clip header cloned from the bank's last clip with the +0x00 Index field patched to N — and
   regenerates the tail directory with the grown Count/LookupDataSize).
5. **dialogue.big**: `lipsync_build.synth_curve(duration)` (43 fps placeholder mouth curve, donor
   viseme dictionary) → `big_write.rebuild(edits= / adds=)`; on REPLACE the TOC `Info` f32
   duration is patched **in-place** afterwards (closes the ANIM writer's "in-place Info edit" gap
   for same-size blobs — [ANIM.md](ANIM.md)).
6. **Validation** (hard-fails staging): text decodes back; snds lookup returns N and stays sorted;
   .lut re-walks clean, clip N byte-exact to the encoded RIFF, IDENTITY fixpoint on the staged bank,
   per-clip diff vs retail (only N changed / only N added); LIPSYNC entry decodes,
   curve↔audio↔Info durations agree, all other 20k+ payloads byte-identical.

## D.4 Worked demon-door example (staged 2026-07-20)

Donor: the **Barrow Fields demon door** (quest 088 "EAT_PIES" branch, speaker `DEMON DOOR`), line
`TEXT_QST_088_EAT_PIES_INTRO_40`.

```
makewav test_voice.wav --seconds 3        # warbling two-tone "voice" with syllable envelope
stage --replace TEXT_QST_088_EAT_PIES_INTRO_40 --speaker "DEMON DOOR" \
      --text "Forge test: this fat-obsessed door now speaks a synthetic line!"
```

Result (`work/dialogue_pipeline/demon_door_replace/`): text.big + ScriptDialogue.lut +
dialogue.big staged; **ALL CHECKS PASSED** — clip 5080 swapped (37,272 B ADPCM, 3.001 s), 5,309
other clips byte-identical, `ScriptDialogue_5080` re-authored (130 frames, Info dur 3.023 s),
20,504 other LIPSYNC payloads byte-identical; extracted clip 5080 ffmpeg-decodes as
`adpcm_ima_xbox 22050 Hz mono`.

ADD example (`work/dialogue_pipeline/demon_door_add/`): `TEXT_DDOOR_FORGE_NEW_LINE_10` →
ScriptDialogue2 N=3061; all four artifacts staged, ALL CHECKS PASSED (2,769 retail clips
untouched, one added).

## D.5 What awaits live testing (do NOT skip on first deploy)

1. **REPLACE smoke test**: deploy the demon_door_replace set, become fat enough to trigger the
   Barrow Fields door intro, confirm new text + audio + moving mouth. Fastest visible line if
   pie-eating is slow: any `TEXT_AI_GOSSIP_*` Dialogue.lug line.
2. **ADD acceptance**: grown .lut / snds.bin / dialogue.big load test (deploy the add set — it
   changes nothing retail-visible, so simply reaching the main menu + hearing any retail dialogue
   proves the grown banks parse).
3. Lipsync curve quality: `synth_curve` is a placeholder pulse; real curves should come from audio
   analysis (43 fps energy → viseme weights) — tool hook exists. (EgoCore's `SpeechAnalyzer`
   WAV→lipsync is a candidate, per the 2026-07-31 EgoCore assessment.)
4. `.lug` REPLACE/ADD runtime acceptance (static validation complete, §B.5).

## D.6 Open items / Ghidra follow-ups

- The snds.bin loader: who reads `data\Defs\*.bin`, confirm `"SND_"` string xref = key
  construction, binary-search on the sorted crc array, and behaviour on missing key (expected:
  text shows, no audio).
- `MaxEntries` true semantics in the .lut runtime (cache/pool size hypothesis).
- `GetTextBySymbol` / `NGameText::CDataBank::GetTextInfo` ↔ speech resolution call path
  ([TEXT.md](TEXT.md) §7).
- How a playing line binds SPEAKER deps (LIPSYNC deps[1] is `SPEAKER_FEMALE1` on nearly everything
  — apparently vestigial; confirm ignored).
- `.lut` clip-header fields +0x04..+0x20 purposes **[hypothesis]** — the writer preserves the
  36-byte header verbatim on replace, so their purpose need not be known.
- `.lug` sample-table header `unk` u16 (205 in Ingame, 0 in Frontend), record fields +276/+280 and
  the +576.. param block — **[hypothesis]**; confirming needs the engine loader
  (`LHAudioBankSampleTable` string xref in Fable.exe).
- `.met` exact field map (redundant for extraction).
- How the engine picks among multi-id criteria lists (random/round-robin) — runtime question.

---

## Dated findings

- **2026-07-19** — `.lut` format decoded on all 4 retail files (20,213 clips, Xbox ADPCM 0x0069
  mono 22050); `.lut` Index == LIPSYNC ID pairing proven by counts + durations; `lut_write.py`
  IDENTITY byte-exact on all 4, same-size + size-change REPLACE PASS; tail directory decoded as a
  plain `(index,size,offset)` array + 560-byte trailer (md5 dc639577347ce41e08b8d9f6656532cd).
- **2026-07-20** — `.lug` format decoded on all 50 retail files (3,833 records, 288 aliases, 9
  empty slots, 1,497 criteria tags); `lug_build.py` IDENTITY-ALL 50/50, REPLACE/ADD PASS.
- **2026-07-20** — `xbadpcm.py` pure-Python Xbox ADPCM encoder/decoder (ffmpeg has no encoder);
  decode byte-exact vs ffmpeg; blocks independent; 64th nibble discarded.
- **2026-07-20** — Dialogue join solved: `<bank>snds.bin` sorted `{crc0("SND_"+Name), soundID}`;
  ordinal-within-group join refuted (r≈0 vs 0.77–0.86); `MaxEntries` < Count ⇒ not a cap;
  `lut_write.rebuild(add=)` shipped; `dialogue_pipeline.py` REPLACE (clip 5080) + ADD (N=3061)
  staged, ALL CHECKS PASSED.
- **2026-09-07** — `LUG_AUDIO_FORMAT.md`, `LUT_AUDIOBANK_FORMAT.md`, `LUT_WRITER.md`,
  `DIALOGUE_PIPELINE.md` merged into this file.

## Superseded

- **"`<N>` join: each `.lut` clip's index == the ordinal in the entry name … which is the same
  `<N>` text.big's type-0 `SpeechBank` resolves to"** (LUT_AUDIOBANK 2026-07-19) — the `.lut`
  Index == LIPSYNC ID half is true; the text.big half is not an ordinal at all: text.big carries
  only the bank name and `<N>` comes from `<bank>snds.bin` (§D.1). The "ordinal within group"
  hypothesis was refuted by duration correlation (r ≈ 0.00).
- **Tail directory "`TocOffset` points ~44 bytes before an `LHAudioBankLookupTable` marker; those
  44 bytes are a small lookup preamble"** and **"the lookup/hash array internal layout is only
  partially decoded (hash bucket array not fully reversed) — the main writer risk"** — there is no
  hash; the 44 bytes are the last 44 bytes of the final clip's data; layout fully decoded (§A.4).
  Original hypotheses `LookupDataSize` "size of the lookup/hash data", `MaxEntries` "table
  capacity (matches clip-header +0x1C)", `LookupOffset` "offset into lookup data" — resolved as in
  §A.4.
- **"`MaxEntries` may be a hard cap; adding new indices beyond `MaxEntries` is untested"**
  (LUT_WRITER §5) — `MaxEntries` (500/1000) is already < retail Count (12134/5310), so it is not a
  count cap (§D.1).
- **"Byte-exact `.lut` write round-trip not proven / not attempted"** (LUT_AUDIOBANK §5, §7) —
  PROVEN 2026-07-19 (§A.6.1).
- **"Producing the replacement blob is the encode side — `ffmpeg -c:a adpcm_ima_xbox` or the Fable
  TLC Toolpack codec"** (LUT_WRITER §4 original) — ffmpeg has NO `adpcm_ima_xbox` encoder; solved
  by `tools/xbadpcm.py` 2026-07-20 (Part C).
- **"`.lug` files are ambient / music"** (LUT_AUDIOBANK §1 note) — `.lug` holds SFX/ambience only;
  music is loose `.ogg` (§B.1).
- **"Adding NEW `.lut` indices — untested"** (LUT_WRITER §7 original) — SHIPPED 2026-07-20 via
  `rebuild(add=)`; engine acceptance of a grown bank remains [unproven] until a live test.
