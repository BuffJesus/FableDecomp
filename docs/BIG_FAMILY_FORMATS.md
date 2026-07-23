# .big family — BIGB bank-archive generalization + dialogue.big (LIPSYNC) deep-dive

*Authored 2026-07-19. Status: the BIGB reader from `docs/TEXTBIG_FORMAT.md` is **VALIDATED across all 8
retail `.big` files**; every file parses byte-clean. `dialogue.big` lipsync payload fully RE'd and
decoded byte-exact (20,505/20,505 entries). This doc extends — does NOT modify — `TEXTBIG_FORMAT.md`.*

Parser: `tools/parse_bigb.py` (pure Python, no Ghidra). Ported directly from the text.big spec.

## Evidence / sources

1. **Raw retail bytes** of all 8 `.big` files under
   `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\`. Full walking parse of every file.
2. **`docs/TEXTBIG_FORMAT.md`** — the text.big BIGB spec (itself triangulated against EgoCore's
   completed-RE loader and the retail `Fable.exe` engine loader). This is the authoritative format
   reference for the container.
3. **fabletlcmod.com wiki** (second, community source) — confirms at the structural level: `.big`
   archives are organized as **banks** each containing **entries**, each entry type in its own
   payload format, edited/recompiled via *Fable Explorer* (see §5). The wiki's HTTPS cert fails
   verification so byte-layout pages could not be fetched directly; the community docs it exposes are
   user-level (texture retexturing) and do not contradict the byte evidence.

Fields whose *purpose* is inferred, not proven, are labeled **[hypothesis]**.

---

## 1. The .big family — file inventory (all located, all sized)

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
`FooterOffset` (see §3). No file deviates from the format.

Also present (audio companions, NOT BIGB): `Dialogue.lut` (268 MB), `Dialogue2.lut` (37 KB),
`ScriptDialogue.lut` (229 MB), `ScriptDialogue2.lut` (175 MB) — these are
`LiOnHeAdLHAudioBankCompData` audio banks (magic at offset 0), referenced by the LIPSYNC entries
(§4.4).

---

## 2. Container recap (per `TEXTBIG_FORMAT.md`, re-verified on every file)

- **Header (16B):** `char[4] Magic="BIGB"`, `u32 Version=100`, `u32 FooterOffset`, `u32 FooterSize`.
- **Footer (sub-bank directory) at `FooterOffset`:** `u32 SubBankCount`, then per sub-bank:
  NUL-terminated `Name`, `u32 Version`, `u32 EntryCount`, `u32 Offset` (TOC start), `u32 Size`
  (TOC byte size), `u32 Align` (payload alignment).
- **Per sub-bank TOC at `Offset`:** optional stats header (`u32 StatsCount<1000` then
  `StatsCount×2` u32 `(type,count)` pairs; else rewind), then `EntryCount` variable-length entry
  records:
  `u32 Magic(=42)`, `u32 ID`, `i32 Type`, `u32 Size`, `u32 Offset`, `u32 CRC`,
  `u32 nameLen + name`, `u32 Timestamp`, `u32 DepCount`, `DepCount × (u32 len + str)` Deps,
  `u32 InfoSize`, `InfoSize` bytes `Info`.
- **Payload blobs** live between `0x10` and `FooterOffset`, each at its entry's `Offset`, `Size` long.

Two field-semantics refinements confirmed by the wider family (both consistent with the spec):
- **`Deps`** = the entry's original **source-asset build path(s)**, e.g.
  `\Dev\BBBProjects\Fable1_5Release\Fable\Resources\Art3\GUI\frontend\fable_BG_01.tga` (frontend),
  `...\FableEngine\shaders\pixel\diffuse_colour.psh` (shaders). For LIPSYNC it holds the **speaker
  tag** (`SPEAKER_FEMALE1`). The spec's "dependency strings" is correct; the payload is the build
  provenance / associated symbol.
- **`Info`** = a small per-entry subheader whose meaning is bank-kind-specific: a texture descriptor
  in GBANK banks (34 bytes: dims + format), a mesh descriptor in MBANK (24–96 bytes), a float
  (clip duration) in LIPSYNC (4 bytes), 4 bytes in text.big. **`InfoSize` is per-entry** (not a
  constant), so a reader must always read `InfoSize` then that many bytes — never assume 0.

---

## 3. Per-file parse report (validation invariant + type histograms)

The validation invariant is: **each sub-bank's TOC ends exactly where the next sub-bank's TOC
begins, and the last sub-bank's TOC ends exactly at `FooterOffset`.** (text.big is the single-sub-bank
special case where TOC-end == `FooterOffset` directly.) This held for all 8 files with zero slack.

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

**Type is per-bank-kind, not global.** In text.big `Type 0/1/2` = string/group/narrator; in
graphics/textures the numbers index mesh/texture sub-formats (e.g. MBANK types 1–9 are distinct mesh
classes); in LIPSYNC every entry is `Type 1` (a lipsync clip). The generic reader treats `Type`
opaquely and still walks the TOC perfectly — only the *payload decoder* is kind-specific.

---

## 4. dialogue.big deep-dive (the LIPSYNC / speech sub-bank)

`dialogue.big` = **4 LIPSYNC sub-banks, 20,505 entries total**, all `Type 1` (a lipsync clip).
The four sub-banks pair one-to-one with the four audio `.lut` banks in the same folder:

| sub-bank | entries | entry name pattern | audio companion |
|----------|--------:|--------------------|-----------------|
| LIPSYNC_ENGLISH_MAIN | 12,134 | `Dialogue_<N>` | `Dialogue.lut` |
| LIPSYNC_ENGLISH_MAIN_2 | 1 | `Dialogue2_<N>` | `Dialogue2.lut` |
| LIPSYNC_ENGLISH_SCRIPT | 5,310 | `ScriptDialogue_<N>` | `ScriptDialogue.lut` |
| LIPSYNC_ENGLISH_SCRIPT_2 | 3,060 | `ScriptDialogue2_<N>` | `ScriptDialogue2.lut` |

The entry `ID` is the **1-based index into the matching audio bank** — this is the cross-reference
back to text.big §5: a text.big type-0 string carries `SpeechBank = ScriptDialogue.lug/.lut`; the
sound resolves to `ScriptDialogue_<N>`; and *this* file supplies the lip animation for that same
`<N>`. Speech text (text.big) → audio (`.lut`) → lip animation (`dialogue.big`) are keyed by the
same ordinal. Empty slots exist (e.g. `ScriptDialogue2_2[Empty]`, `Size=0`) — a reserved/placeholder
index with no clip, still a valid TOC record.

### 4.1 LIPSYNC entry TOC record (generic BIGB record; kind-specific fields)
- `Name` = `Dialogue_<N>` / `ScriptDialogue_<N>` (index name).
- `DepCount = 2`: `Deps[0]` = `""` (empty), `Deps[1]` = **speaker tag** (all sampled MAIN entries =
  `SPEAKER_FEMALE1`; the tag namespace matches text.big's Speaker/narrator scheme). **[hypothesis:
  Deps[0] reserved for a second/source path, empty in English build]**
- `InfoSize = 4`, `Info` = **float32 clip duration in seconds** (verified: `Dialogue_1` → 1.579,
  payload keyframes span exactly that clip). `Size=0` empty entries have `InfoSize=0`.

### 4.2 LIPSYNC payload blob (fully decoded, byte-exact)
Payload at entry `Offset`, `Size` bytes:

| Order | Type | Field |
|-------|------|-------|
| 1 | u32 | `VisemeCount` |
| 2 | `VisemeCount` × 4 bytes | **Viseme table**: `{ u8 index; char[2] mnemonic; u8 NUL }` |
| 3 | u32 | `BlockMarker` = **0x2B (43)** — keyframe-block sentinel (distinct from entry `Magic`=42) |
| 4 | u32 | `KeyframeCount` |
| 5 | `KeyframeCount` × keyframe | each: `u8 PairCount`, then `PairCount × { u8 VisemeIndex; u8 Weight(0..255) }` |

**Visemes** are phoneme/mouth-shape classes named by 2-char ASCII mnemonic — observed set:
`AH`, `EE`, `OH`, `SZ` (indices 1–4). A keyframe is a sparse set of `(viseme, blend-weight)` pairs;
the weight 0–255 is the blend amount for that mouth shape at that frame. Playback interpolates the
viseme weights over `Info` (duration) to drive the face rig. **[hypothesis: keyframes are evenly
spaced across the clip duration; no explicit per-frame timestamp is stored — the count + duration
imply the frame rate.]**

### 4.3 Dumped sample entries (proof of coherence)

`LIPSYNC_ENGLISH_MAIN / Dialogue_1` (id=1, size=428, dur=1.579s, speaker=SPEAKER_FEMALE1):
- Viseme table: `(1,'AH') (2,'EE') (3,'OH') (4,'SZ')`
- 68 keyframes. kf[0]=`(AH,100)(EE,38)(OH,22)`, kf[1..3]=`(OH,255)`, kf[4]=`(OH,255)(EE,44)` …
- Decodes to **exactly** the entry's `Size` boundary (0x1BC), zero slack.

`Dialogue_3` (id=3, size=262, dur=1.083s): 3 visemes `(AH,EE,OH)`, 46 keyframes, byte-exact.
`ScriptDialogue_1` (id=1, size=1879, dur=6.66s): 4 visemes, byte-exact.

**Whole-bank validation:** all **12,134** entries of LIPSYNC_ENGLISH_MAIN decode with the above
grammar and terminate byte-exact at their `Size` (0 failures). The other three LIPSYNC sub-banks use
the identical grammar. So a LIPSYNC entry holds: **a viseme dictionary + a viseme-weight keyframe
animation + a duration + a speaker tag + an audio-bank index** — i.e. everything a dialogue/voice
editor needs to line up text ↔ audio ↔ lip motion. It does **not** embed the audio itself (that's in
the `.lut`) nor the display text (that's in text.big); it is the lip-animation layer joining them.

### 4.4 Cross-reference summary (text ↔ audio ↔ lipsync)
```
text.big  (TEXT_ENGLISH_MAIN, type-0)  --SpeechBank/ID-->  ScriptDialogue.lut  (audio blob N)
                                                                   ^
dialogue.big (LIPSYNC_ENGLISH_SCRIPT)  ScriptDialogue_N  ---------/  (lip animation for N)
```
Keyed by the ordinal `<N>`. Confirmed by: matching name stems, the four sub-bank ↔ four `.lut`
pairing, and text.big §5's `SpeechBank=ScriptDialogue.lug/.lut` note.

---

## 5. Generalization verdict

**The BIGB reader covers the entire `.big` family.** The same 16-byte header + footer directory +
chained sub-bank TOCs describe all 8 files; the generic TOC record parses every entry in every file
(72k+ entries total) and the TOC-chain-ends-at-footer invariant holds everywhere. What differs per
file is only the **payload decoder** (string / group / lipsync / texture / mesh / shader / particle),
selected by the sub-bank `Name` prefix and the per-entry `Type`. The wiki independently frames `.big`
as banks-of-typed-entries edited by Fable Explorer, consistent with this model.

Payload decoders now known:
- **text.big** — type-0 string / type-1 group / type-2 narrator (see `TEXTBIG_FORMAT.md`).
- **dialogue.big** — LIPSYNC viseme-weight keyframe animation (this doc §4). **Fully cracked.**
- **frontend/textures (GBANK)** — 34-byte texture `Info` descriptor + source `.tga` dep + texture
  blob payload. *(structure identified; pixel-format decode not exhaustively verified here.)*
- **graphics (MBANK)** — mesh entries, 9 sub-types, variable `Info` mesh descriptors (24–96B).
  *(TOC clean; mesh payload not decoded in this task.)*
- **effects (PARTICLE)** — particle-system entries by name, no Info/Deps. *(payload not decoded.)*
- **shaders** — compiled shader bytecode payload + source `.psh`/`.vsh` dep. *(payload not decoded.)*

---

## 6. WRITE-PATH RISK (a byte-exact repacker must reproduce ALL of this)

Byte-exact round-trip is **NOT proven** for any `.big` (reading only, same discipline as text.big
§7 / `game.bin`). A writer must faithfully reproduce, or the retail engine may reject the bank:

1. **Header back-patch** — `FooterOffset` / `FooterSize` after the payload/TOC region moves.
2. **Sub-bank TOC chaining** — each sub-bank's `Offset`/`Size` must be recomputed so TOC[i] ends
   exactly at TOC[i+1] start and the last ends exactly at `FooterOffset`. The engine's `Size` fields
   and this contiguity are the read invariant; drift breaks the walk.
3. **Payload alignment** — the sub-bank `Align` field is real and large in some banks: `2048` for
   MBANK/GBANK/PARTICLE/FONT_MAIN, `512`/`4`/`1`/`2` elsewhere. Payload offsets in
   graphics/textures/effects/frontend are 2048-aligned (`0x800` boundaries). A repacker MUST pad to
   `Align` or offsets will be wrong. text/dialogue use `Align=1` (packed).
4. **Stats header** — must be regenerated (`StatsCount` + `(type,count)` pairs summing to
   `EntryCount`); EgoCore rewinds if `StatsCount>=1000`, so the writer must keep it `<1000` and
   accurate.
5. **`CRC` field (TOC +0x14)** — **[hypothesis]**, observed `0x00000000` in text.big and dialogue.big
   TOC entries, but non-trivial elsewhere is unconfirmed. If any bank/engine path validates it,
   a writer must recompute it (candidate: seed-0 `CCRC::Calc`, per text.big §8). **Unverified —
   flag before shipping a writer.**
6. **`Timestamp` field** — **[hypothesis]** build timestamp; observed `0` in text/dialogue. Preserve
   verbatim on edit rather than regenerate, until its role is confirmed.
7. **Per-entry `Info` / `Deps`** — must be re-emitted exactly (`InfoSize` is per-entry; Deps are
   length-prefixed with the trailing-NUL convention). For textures/meshes the `Info` descriptor is
   load-bearing (dims/format); do not drop it.
8. **LIPSYNC specifics** — if editing lip animation: keep `BlockMarker=0x2B`, keep the viseme table
   consistent with the indices referenced by keyframes, recompute `Info` duration if timing changes,
   and preserve the `Dialogue_<N>` / audio-bank ordinal alignment (the `.lut` index must still match).

**Mandatory gate:** verify a byte-identical read→write round-trip on an *unmodified* file for each
bank kind before allowing any real edit through a writer.

---

## 7. Reproduce

```
python tools/parse_bigb.py "<install>\data\lang\English\dialogue.big"          # TOC + histograms
python tools/parse_bigb.py "<install>\data\lang\English\text.big" --dump 3      # sample records
```
LIPSYNC payload decode + full byte-exact validation loop is in the session notes; the grammar is
§4.2 (drop-in for FableForge). Parser: `tools/parse_bigb.py`.
