# Custom dialogue pipeline — the cross-file join + one-command staging

*2026-07-20. Status: join model SOLVED (empirical, no Ghidra — lock was taken);
pipeline SHIPPED (`tools/dialogue_pipeline.py`); REPLACE and ADD both staged and
statically validated; engine runtime acceptance awaits a live test (no game launch
allowed this run). Companions: `docs/TEXT_BIG_WRITE.md` (text), `docs/LUT_WRITER.md` +
`docs/LUG_AUDIO_FORMAT.md` §8 (audio), `docs/ANIM_WRITER.md` (lipsync),
`docs/DEMON_DOOR_FACE.md` (how the mouth animates from the lipsync curve).*

## 1. THE JOIN (the RE question this run settled)

text.big's type-0 payload stores only the bank NAME (`SpeechBank = "ScriptDialogue.lug"`)
— **the clip index `<N>` is not in text.big, the .lut, or dialogue.big. It lives in a
fourth file family: `data\Defs\<bank>snds.bin`**, a compiled name→index table:

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

\* orphans = pairs whose CRC matches no current text entry name (renamed/cut lines);
crc and soundID are unique within every file; pair count == real clip count exactly.

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

### Evidence (all empirical, 2-source rule)

1. **CRC hit rate.** `crc0("SND_"+Name)` of text entries with `SpeechBank` set resolves
   in the matching snds.bin: 12,121/12,129 (Dialogue.lug), 5,263/6,325 (ScriptDialogue.lug),
   2,704/2,738 (ScriptDialogue2.lug). Misses are visibly audio-less lines (GUI help,
   quest-log names/descs, `TEXT_QST_LOG_STORY_PLATINUM_*`) — SpeechBank set by authors,
   audio never recorded. Zero false positives (every resolved map is injective).
2. **Duration correlation (the clincher).** Text length (chars) vs the LIPSYNC duration
   of the resolved `<N>`: r = 0.77 (MAIN), 0.84 (SCRIPT), 0.86 (SCRIPT_2). Under the
   previously-hypothesized "ordinal within group" join the same metric is r ≈ 0.00 —
   ordinal is REFUTED, snds.bin is the join.
3. **EgoCore cross-check.** EgoCore `TextBackend.h ResolveAudioID()` resolves
   `(SND_|TEXT_SND_)<Identifier> = <N>` from `dialoguesnds.h`/`scriptdialoguesnds.h`
   enum headers — the .h source form of the same tables (BankCreator emitted both; retail
   ships only the compiled .bin). `RetailHeaders\dialogue_lipsync.h` (`Dialogue_1 = 1`)
   is the LIPSYNC-side enum of the same ids.
4. **Engine-side strings.** `Fable.exe` contains the standalone prefix string `"SND_"`
   (runtime key construction) plus literal `SND_TEXT_QST_TUTORIAL_*` names; game.bin's
   `names.bin` references `gamesnds.h` / `dialoguesnds.h` / `scriptdialoguesnds(2).h`
   (the def system declares these enum tables). No Ghidra pass yet — flagged §6.
5. Worked resolve: `TEXT_QST_088_EAT_PIES_INTRO_40` ("I want beefy! Blubbery! Plump!
   Porcine!", Barrow Fields demon door) → crc `C699DB84` → scriptdialoguesnds.bin →
   **N=5080** → ScriptDialogue.lut clip 5080 (96,096 B RIFF) ↔ `ScriptDialogue_5080`
   lipsync dur 7.744 s. Clip durations and lipsync durations agree bank-wide
   (LUT_AUDIOBANK_FORMAT.md §4).

### Bonus finding: `MaxEntries` is NOT a count cap

The `.lut` tail `MaxEntries` (500 Dialogue / 1000 Script) — the feared ADD blocker —
is **less than the retail `Count`** (12,134 / 5,310) in every shipped bank, so it cannot
cap the entry count. (Likely a runtime cache/pool size; exact meaning still open, §6.)
This unblocks ADD-new-index structurally.

## 2. ADD vs REPLACE verdict

- **REPLACE (donor line) = the honest, proven v1.** Every artifact edit re-uses a
  mechanism already byte-exact-validated (text set, .lut clip replace, LIPSYNC payload
  edit); the join table is untouched; the line already has a retail trigger. Only the
  generic "writer outputs not yet loaded by the live engine" residual applies.
- **ADD (new line) = staged and structurally clean, engine acceptance [unproven].**
  All four artifacts stage and re-validate (new text id, sorted snds.bin pair, .lut
  clip at index max+1 with regenerated tail, LIPSYNC entry id max+1). Unknowns that
  only a live test settles: (a) engine tolerance of grown .lut Count, (b) whether the
  snds.bin loader accepts a grown table (it should — plain sorted array), (c) a NEW
  line also needs a *trigger* (quest scripts are compiled C++ — FSE Lua or a donor
  quest hook), which is outside these four files.

## 3. The pipeline tool

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

1. **Audio**: input WAV (PCM16, any rate/channels) → downmix + linear-resample to mono
   22,050 Hz → `xbadpcm.encode_wav` → retail-layout Xbox-ADPCM RIFF; duration =
   dataSize/12403.
2. **text.big**: `text_build.set_text` (replace: text+speaker) or `add_text` (add:
   +SpeechBank). Speaker must be one of the 379 NarratorList names (enforced).
3. **snds.bin** (ADD only): insert `{crc0("SND_"+name), N}` keeping the crc sort;
   N = max(existing ids, lut indices)+1. REPLACE copies it verbatim into the staging
   set (mapping already retail).
4. **.lut**: `lut_write.rebuild(replace=)` or the NEW `rebuild(add=)` (this run's
   extension: appends one record — template 36-B clip header cloned from the bank's
   last clip with the +0x00 Index field patched to N — and regenerates the tail
   directory with the grown Count/LookupDataSize).
5. **dialogue.big**: `lipsync_build.synth_curve(duration)` (43 fps placeholder mouth
   curve, donor viseme dictionary) → `big_write.rebuild(edits= / adds=)`; on REPLACE
   the TOC `Info` f32 duration is patched **in-place** afterwards (closes the
   ANIM_WRITER.md §5 "in-place Info edit" gap for same-size blobs).
6. **Validation** (hard-fails staging): text decodes back; snds lookup returns N and
   stays sorted; .lut re-walks clean, clip N byte-exact to the encoded RIFF, IDENTITY
   fixpoint on the staged bank, per-clip diff vs retail (only N changed / only N added);
   LIPSYNC entry decodes, curve↔audio↔Info durations agree, all other 20k+ payloads
   byte-identical.

## 4. Worked demon-door example (staged this run)

Donor: the **Barrow Fields demon door** (quest 088 "EAT_PIES" branch, speaker
`DEMON DOOR`), line `TEXT_QST_088_EAT_PIES_INTRO_40`.

```
makewav test_voice.wav --seconds 3        # warbling two-tone "voice" with syllable envelope
stage --replace TEXT_QST_088_EAT_PIES_INTRO_40 --speaker "DEMON DOOR" \
      --text "Forge test: this fat-obsessed door now speaks a synthetic line!"
```

Result (`work/dialogue_pipeline/demon_door_replace/`): text.big + ScriptDialogue.lut +
dialogue.big staged; **ALL CHECKS PASSED** — clip 5080 swapped (37,272 B ADPCM, 3.001 s),
5,309 other clips byte-identical, `ScriptDialogue_5080` re-authored (130 frames, Info
dur 3.023 s), 20,504 other LIPSYNC payloads byte-identical; extracted clip 5080
ffmpeg-decodes as `adpcm_ima_xbox 22050 Hz mono`.

ADD example (`work/dialogue_pipeline/demon_door_add/`): `TEXT_DDOOR_FORGE_NEW_LINE_10`
→ ScriptDialogue2 N=3061; all four artifacts staged, ALL CHECKS PASSED (2,769 retail
clips untouched, one added).

## 5. What awaits live testing (do NOT skip on first deploy)

1. **REPLACE smoke test**: deploy the demon_door_replace set, become fat enough to
   trigger the Barrow Fields door intro, confirm new text + audio + moving mouth.
   Fastest visible line if pie-eating is slow: any `TEXT_AI_GOSSIP_*` Dialogue.lug line.
2. **ADD acceptance**: grown .lut / snds.bin / dialogue.big load test (deploy the add
   set — it changes nothing retail-visible, so simply reaching the main menu + hearing
   any retail dialogue proves the grown banks parse).
3. Lipsync curve quality: `synth_curve` is a placeholder pulse; real curves should come
   from audio analysis (43 fps energy → viseme weights) — tool hook exists.

## 6. Ghidra follow-ups (lock was taken this run)

- The snds.bin loader: who reads `data\Defs\*.bin`, confirm `"SND_"` string xref =
  key construction, binary-search on the sorted crc array, and behaviour on missing key
  (expected: text shows, no audio).
- `MaxEntries` true semantics in the .lut runtime (cache/pool size hypothesis).
- `GetTextBySymbol` / `NGameText::CDataBank::GetTextInfo` ↔ speech resolution call path.
- How a playing line binds SPEAKER deps (LIPSYNC deps[1] is `SPEAKER_FEMALE1` on nearly
  everything — apparently vestigial; confirm ignored).
