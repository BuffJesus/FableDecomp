# `.lut` audio-bank format (`LiOnHeAdLHAudioBankCompData`) — the voice/dialogue audio banks

*Authored 2026-07-19. Status: on-disk format RE'd and VALIDATED against all 4 retail `.lut` files
(20,213 embedded clips walked byte-clean). The `<N>` ordinal join to text.big + dialogue.big is
PROVEN by matching entry counts AND matching per-clip durations. Codec is CONFIRMED (not guessed):
Xbox IMA ADPCM. This completes the dialogue chain **text.big (text) → `.lut` (audio) →
dialogue.big (lipsync)**.*

Parser: `tools/parse_lut.py` (pure Python, no Ghidra).

## Verdict up front

- **Container:** Lionhead `LiOnHeAdLHAudioBankCompData` audio bank. Header → a run of per-clip
  records (each = small header + one embedded **RIFF/WAVE** file) → a lookup-table directory at the
  file tail.
- **Audio location: EMBEDDED.** Each voice clip is a complete standard RIFF/WAVE blob stored inline
  in the `.lut`. No external blob is needed; the `.lut` is self-contained.
- **Codec: Xbox IMA ADPCM** — `WAVE_FORMAT_XBOX_ADPCM`, fmt tag `0x0069`, 1 ch, 22050 Hz, 4-bit,
  block align 36. Confirmed by two independent sources (the RIFF `fmt ` chunk bytes AND ffmpeg
  decoding a clip as `adpcm_ima_xbox`).
- **`<N>` join: PROVEN.** Each `.lut` clip carries a 1-based index; that index == the dialogue.big
  `LIPSYNC_*` entry `ID` == the ordinal in the entry name `Dialogue_<N>` / `ScriptDialogue_<N>`,
  which is the same `<N>` text.big's type-0 `SpeechBank` resolves to.
- **Extraction feasibility: YES, proven.** `tools/parse_lut.py --extract <N> out.wav` pulls a byte-
  exact, self-consistent, ffmpeg-playable RIFF/WAVE clip.

## Evidence / sources

1. **Raw retail bytes** of all 4 `.lut` files under
   `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\lang\English\`. Full walking
   parse of every file (headers hexdumped, every clip's RIFF validated, directory decoded).
2. **`docs/BIG_FAMILY_FORMATS.md` + `docs/TEXTBIG_FORMAT.md`** (this project's own cracked banks) —
   the second, cross-referencing source. dialogue.big's four `LIPSYNC_*` sub-bank entry counts and
   `Dialogue_<N>`/`ScriptDialogue_<N>` naming, and its per-entry lipsync **duration** float, are the
   independent check the `.lut` parse is validated against (§4).
3. **ffmpeg 8.1** — independent codec oracle; identifies an extracted clip as
   `adpcm_ima_xbox (i[0][0][0] / 0x0069), 22050 Hz, mono` and decodes it to PCM.
4. **fabletlcmod.com community** (search snippets; the site's HTTPS cert fails verification so pages
   weren't fetched directly — flagged for reliability). Corroborates at the structural level: the
   `.lut` (`Dialogue.lut`, `ScriptDialogue.lut`) are audio banks, and the **Fable TLC Toolpack ships
   an ADPCM codec** for them — consistent with the byte evidence (Xbox ADPCM). Community docs are
   user-level; they do not contradict and do not add a byte layout.

Fields whose *purpose* is inferred, not proven, are labeled **[hypothesis]**.

---

## 1. File inventory (all located, all sized, all parsed)

| File | Path (under Steam install) | Size (bytes) | clips walked | index range | Verdict |
|------|----------------------------|-------------:|-------------:|:-----------:|:-------:|
| `Dialogue.lut` | `data\lang\English\` | 268,367,088 | 12,134 | 1..12134 (contiguous) | CLEAN |
| `Dialogue2.lut` | `data\lang\English\` | 37,968 | 1 | 1..1 | CLEAN |
| `ScriptDialogue.lut` | `data\lang\English\` | 229,244,544 | 5,310 | 1..5310 (contiguous) | CLEAN |
| `ScriptDialogue2.lut` | `data\lang\English\` | 174,964,500 | 2,769 | 1..3060 (291 gaps) | CLEAN |

Across all 4: **20,213 embedded clips**, fmt tag exclusively `0x0069`, sample rate exclusively
22050 Hz, **zero out-of-bounds clips, zero malformed RIFFs**.

> **`.lug` is a DIFFERENT bank, not the `.lut` payload.** The `data\Sound\*.lug` files (ambient /
> music, e.g. `Ingame.lug`, `village.lug`) have magic `LiOnHeAd`**`LHFileSegmentBankInfo`** — a
> *segment* bank, a sibling format, NOT the audio blob for the `.lut`. The `.lut` clips are embedded
> in the `.lut` itself. (text.big's `SpeechBank` field names e.g. `ScriptDialogue.lug`/`.lut`; the
> voice audio for dialogue lives in the `.lut`. `LHFileSegmentBankInfo` also appears as a *sub-struct*
> inside the `.lut` tail directory — see §3 — which is why the name recurs.)

---

## 2. On-disk layout

### 2.1 File header (44 bytes, offset 0)

| Off | Type | Field | Value | Meaning |
|----:|------|-------|-------|---------|
| 0x00 | char[28] | `Magic` | `"LiOnHeAdLHAudioBankCompData\0"` | container magic (27 chars + NUL) |
| 0x1C | u32×3 | reserved | `0` | zero padding |
| 0x28 | u32 | `TocOffset` | file offset of the tail lookup-table directory (§3) |

The first **clip record begins at offset 0x2C**, immediately after `TocOffset`. (The `u32` at 0x2C —
often mis-read as a "sub-bank count" — is actually clip[0]'s `Index` field = `1`.)

### 2.2 Clip record (repeats `EntryCount` times, back-to-back from 0x2C)

Each record = a **36-byte clip header** then one complete **RIFF/WAVE** file:

| Off | Type | Field | Typical value | Meaning |
|----:|------|-------|---------------|---------|
| +0x00 | u32 | **`Index`** | 1,2,3,… | **1-based ordinal `<N>`** — the join key (see §4) |
| +0x04 | u32 | const | `0x56220001` | packed `0x5622`=22050 sample-rate \| `0x0001` channels **[hypothesis]** |
| +0x08 | u32 | const | `0x00019C40` (105536) | constant across all clips **[hypothesis: format/quality id]** |
| +0x0C | u32 | const | `0x01010000` | constant flags **[hypothesis]** |
| +0x10 | u32 | `Flags2` | ~`0x00647F06`, varies | per-clip; low bytes differ **[hypothesis]** |
| +0x14 | f32 | `MinDistance` | 1.5 (Dialogue) / 5.0 (Script) | 3D audio min distance **[hypothesis]** |
| +0x18 | f32 | `MaxDistance` | 18.0 (Dialogue) / 25.0 (Script) | 3D audio max distance **[hypothesis]** |
| +0x1C | u32 | `Priority` | 500 (Dialogue) / 1000 (Script) | priority / range **[hypothesis]** |
| +0x20 | u32 | sentinel | `0xFFFFFFFF` | terminator / no-loop **[hypothesis]** |
| +0x24 | RIFF | **`Clip`** | `RIFF….WAVE` | the embedded audio file (§2.3) |

The next record starts immediately after the RIFF (`riff_off + 8 + RIFFsize`). Records are **packed,
no alignment padding** — the walk lands exactly on the next `Index`+RIFF or on the tail directory.
This is how the parser walks the whole file with zero slack.

### 2.3 Embedded RIFF/WAVE clip (the actual audio)

Standard Microsoft RIFF WAVE — decodable by any WAV tool that supports Xbox ADPCM:

- `RIFF` / `<size>` / `WAVE`
- `fmt ` chunk (size 20): `wFormatTag=0x0069` (**WAVE_FORMAT_XBOX_ADPCM**), `nChannels=1`,
  `nSamplesPerSec=22050`, `nAvgBytesPerSec=12403`, `nBlockAlign=36`, `wBitsPerSample=4`,
  `cbSize=2`, extra = `0x0040` (64 samples/block).
- `data` chunk = the Xbox-ADPCM audio payload.

Clip **duration** = `data_size / nAvgBytesPerSec`. Verified to match dialogue.big's stored lipsync
duration for every sampled `<N>` (§4).

> Note: several early/placeholder clips (e.g. `Dialogue.lut` N=1) have a `data` chunk that is all
> zeros or silent lead-in; these are still valid, ffmpeg-decodable RIFFs. `Dialogue2.lut` /
> `ScriptDialogue2.lut` N=1 contain real audio.

---

## 3. Tail directory (at `TocOffset`)

`TocOffset` (header +0x28) points ~44 bytes before an `LHAudioBankLookupTable` marker; those 44
bytes are a small lookup preamble. Two named, NUL-padded (32-byte) struct blocks follow:

**`LHAudioBankLookupTable`** — the runtime index→clip lookup table. Fields after the 32-byte name:

| Type | Field | Dialogue.lut | ScriptDialogue.lut | Meaning |
|------|-------|-------------:|-------------------:|---------|
| u32 | `LookupDataSize` | — | 63728 | size of the lookup/hash data **[hypothesis]** |
| u32 | `MaxEntries` | 500 | 1000 | table capacity (matches clip-header +0x1C) **[hypothesis]** |
| u32 | **`Count`** | 12134 | 5310 | **number of clips** (matches the walk exactly) |
| u32 | `flag` | 1 | 1 | **[hypothesis]** |
| u32 | `LookupOffset` | — | 0x14334 | offset into lookup data **[hypothesis]** |
| u32 | reserved | 0 | 0 | |

**`LHFileSegmentBankInfo`** — bank provenance sub-struct; followed by the ASCII comment
`\Generated bank for Fable` and a `NoFPNameYet` name placeholder.

For **extraction**, walking clip records linearly by the +0x00 `Index` field (§2.2) is sufficient and
fully validated — the tail lookup table is the engine's fast runtime resolver and is not required to
locate/pull a clip. A byte-exact **writer** would have to reproduce `Count`, the lookup table, and
`TocOffset` (see §5).

---

## 4. The `<N>` ordinal join (PROVEN) — text.big → `.lut` → dialogue.big

The four `.lut` files pair one-to-one with dialogue.big's four `LIPSYNC_*` sub-banks, and the clip
`Index` == the LIPSYNC entry `ID` == the `<N>` in the entry name:

| `.lut` file | clips (real) | dialogue.big sub-bank | LIPSYNC entries | match |
|-------------|-------------:|-----------------------|----------------:|:-----:|
| `Dialogue.lut` | 12,134 | LIPSYNC_ENGLISH_MAIN | 12,134 | ✅ exact |
| `Dialogue2.lut` | 1 | LIPSYNC_ENGLISH_MAIN_2 | 1 | ✅ exact |
| `ScriptDialogue.lut` | 5,310 | LIPSYNC_ENGLISH_SCRIPT | 5,310 | ✅ exact |
| `ScriptDialogue2.lut` | 2,769 + **291 gaps = 3,060** | LIPSYNC_ENGLISH_SCRIPT_2 | 3,060 | ✅ (gaps = empty entries) |

`ScriptDialogue2.lut`'s 291 missing indices (first missing = **2**) correspond exactly to
dialogue.big's `Size=0` empty LIPSYNC entries (the doc's `ScriptDialogue2_2[Empty]` example): a
reserved index with no clip. `2769 present + 291 empty = 3060` closes the count.

**Duration cross-check (the clincher).** For sampled `ScriptDialogue_<N>`, the lipsync duration stored
in dialogue.big's per-entry `Info` float equals the `.lut` clip duration (`data_size/avg_bps`):

| `<N>` | dialogue.big lipsync dur | `.lut` clip dur |
|------:|-------------------------:|----------------:|
| 1 | 6.664 s | 6.66 s |
| 2 | 3.184 s | 3.18 s |
| 3 | 3.242 s | 3.24 s |
| 4 | 1.271 s | 1.27 s |
| 5 | 4.165 s | 4.17 s |
| 101 | 3.823 s | 3.82 s |
| 1001 | 1.959 s | 1.96 s |

Two independently-authored files agreeing on the duration of the same `<N>` proves both (a) the join
is real and (b) the `.lut` clip-offset parse is correct.

Full chain:
```
text.big  (TEXT_ENGLISH_MAIN, type-0 string)
   |  SpeechBank = ScriptDialogue.lug/.lut  + entry <N>
   v
ScriptDialogue.lut  clip Index=<N>   ==>  embedded RIFF/WAVE (Xbox ADPCM voice audio)
   ^
dialogue.big (LIPSYNC_ENGLISH_SCRIPT)  entry ID=<N>  name ScriptDialogue_<N>  (lip animation + duration)
```

---

## 5. WRITE-PATH RISK (a byte-exact repacker must reproduce all of this)

Reading is validated; a byte-exact **write** round-trip is NOT proven (same discipline as the `.big`
docs). A writer must faithfully reproduce, or the retail engine may reject the bank:

1. **`TocOffset` back-patch** (header +0x28) after the clip region size changes.
2. **Clip records packed, no alignment** — each record is `36 + (8+RIFFsize)` bytes, next record
   immediately after. Any inserted padding breaks the walk. Preserve the packing.
3. **Clip-header const fields** (+0x04 `0x56220001`, +0x08 `0x00019C40`, +0x0C `0x01010000`,
   +0x20 `0xFFFFFFFF`) and the 3D-audio floats/priority — re-emit exactly; their engine role is
   **[hypothesis]** so preserve verbatim rather than regenerate.
4. **Tail lookup table** — `LHAudioBankLookupTable` `Count` must equal the clip count; `MaxEntries`,
   `LookupDataSize`, `LookupOffset` and the lookup/hash array must be regenerated so the engine's
   index→clip resolution still works. This is the least-understood part and the main writer risk —
   **flag before shipping a writer**; verify a byte-identical read→write of an unmodified `.lut`
   first.
5. **`Index` continuity** — keep `Dialogue_<N>` / `ScriptDialogue_<N>` alignment; the dialogue.big
   LIPSYNC id and text.big `SpeechBank` `<N>` must still resolve to the same clip. Preserve the empty
   (`Size=0`) index slots in `ScriptDialogue2` — they are meaningful gaps, not removable.
6. **RIFF fidelity** — replacement audio must be `WAVE_FORMAT_XBOX_ADPCM` (0x0069), mono, 22050 Hz,
   block align 36, or the engine's ADPCM decoder path likely mis-plays it. (Community Toolpack ADPCM
   codec = the encode side.)

---

## 6. Reproduce / extract

```
python tools/parse_lut.py "<install>\data\lang\English\Dialogue.lut"            # header + walk + codec + join stats
python tools/parse_lut.py "<install>\data\lang\English\ScriptDialogue.lut" --list
python tools/parse_lut.py "<install>\data\lang\English\Dialogue.lut" --dump 5
python tools/parse_lut.py "<install>\data\lang\English\ScriptDialogue.lut" --extract 5 clip5.wav
ffmpeg -i clip5.wav clip5_pcm.wav        # decodes Xbox ADPCM -> PCM (confirms playable)
```

Extracted files are standard RIFF/WAVE (fmt 0x0069). Any Xbox-ADPCM-aware player (ffmpeg, VLC with
the codec, the Fable TLC Toolpack) plays them. **Extraction of a playable clip is proven.**

## 7. Open / unverified items

- Clip-header fields +0x04..+0x20 purposes are **[hypothesis]** (named from position + the 3D-audio
  float pattern); not confirmed against the engine loader (Ghidra lock held by another agent this
  run). Reading/extraction does not need them.
- The tail `LHAudioBankLookupTable` lookup/hash array internal layout is only partially decoded
  (struct header fields + `Count` proven; the hash bucket array not fully reversed). Not needed for
  linear extraction; **needed for a byte-exact writer.**
- Byte-exact **write** round-trip not attempted (reading only, this run).
- fabletlcmod.com pages could not be fetched (cert failure) — community corroboration is via search
  snippets only; flagged.
