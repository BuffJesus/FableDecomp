# `.lug` audio-bank format (`LiOnHeAdLHFileSegmentBankInfo`) — SFX / ambience banks

*Authored 2026-07-20. Status: on-disk format RE'd and VALIDATED against **all 50 retail `.lug`
files** under `data\Sound\` (3,833 sample-table entries, 3,545 unique embedded RIFF/WAVE clips,
every byte of every file walked with zero slack). Codec CONFIRMED empirically (RIFF `fmt ` bytes +
ffmpeg decode): overwhelmingly **Xbox IMA ADPCM** (`0x0069`), plus a handful of plain 16-bit PCM.*

*WRITE side added 2026-07-20 (§8): `tools/lug_build.py` — identity repack byte-exact on all 50
retail banks, clip replace + new-clip/criteria add validated; Xbox ADPCM encoder
`tools/xbadpcm.py` (ffmpeg has no encoder for this codec).*

Parser: `tools/parse_lug.py` (pure Python, no Ghidra). Writer: `tools/lug_build.py`.
Sibling doc: `docs/LUT_AUDIOBANK_FORMAT.md` — the `.lut` **dialogue** banks. Same family
(`LiOnHeAd` + 32-byte struct names + embedded RIFFs), different container:
`.lut` = `LHAudioBankCompData` (voice), `.lug` = `LHFileSegmentBankInfo` (SFX/ambience).

## Verdict up front

- **What `.lug` holds:** per-region/ambience **sound-effect banks** (footsteps, cowbells, wind,
  creature vocalisations, GUI clicks…). NOT music (music/cutscene audio is loose `.ogg` in
  `data\Sound\`), NOT dialogue (that's the `.lut` banks in `data\lang\<lang>\`).
- **Container:** `LiOnHeAd` magic, then a stream of self-sizing blocks, each
  `char[32] structName + u32 size + payload`. Four block types in every retail file, in order:
  `LHFileSegmentBankInfo` (title), `LHAudioWaveData` (packed RIFFs), `LHAudioBankSampleTable`
  (652-byte records: dev source path, offset/size, wave format, group name, 3D params),
  `LHAudioBankCriteiaInfo` (sic — engine's own typo; event-tag → sample-id map).
- **Audio location: EMBEDDED.** `LHAudioWaveData` = complete standard RIFF/WAVE files packed
  back-to-back. Extraction = copy `riffSize` bytes at `waveDataStart + riffOffset`.
- **Codec (empirical, all 3,824 non-empty entries):** `WAVE_FORMAT_XBOX_ADPCM` (0x0069) 4-bit for
  3,773 clips; plain PCM (0x0001) 16-bit for 51 clips. 22050 or 44100 Hz, mono or stereo. No OGG,
  no XMA, nothing else.
- **`.met` sidecars:** every `.lug` has a small `.met` file — a build-time metadata mirror (dev
  source path + format + the same playback params). Not needed to read the `.lug`.
- **Extraction: PROVEN.** `tools/parse_lug.py --extract / --extract-all` produces standalone
  `.wav` files; PCM ones decode with Python stdlib `wave`, ADPCM ones decode with ffmpeg as
  `adpcm_ima_xbox` (validation stats in §6).

## Evidence / sources

1. **Raw retail bytes** of all 50 `.lug` (+ paired `.met`) files under
   `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\Sound\`. Full byte-exact walk
   of every file: block stream consumes each file exactly (no trailing slack), independent RIFF
   walk of the wave block cross-checked against the sample table for all 50 files.
2. **Internal struct names the engine itself wrote** (`LHFileSegmentBankInfo`,
   `LHAudioWaveData`, `LHAudioBankSampleTable`, `LHAudioBankCriteiaInfo`) — self-describing
   layout, same convention as the `.lut` (`LHAudioBankCompData`, `LHAudioBankLookupTable`).
3. **`docs/LUT_AUDIOBANK_FORMAT.md`** — the sibling bank format cracked yesterday; identical
   codec (fmt 0x0069, block align 36, 64 samples/block) and identical `name[32]+size` block
   convention corroborate the read.
4. **ffmpeg 8.1** — independent codec oracle: identifies extracted clips as
   `adpcm_ima_xbox … 22050/44100 Hz` and decodes them to PCM (§6).
5. **`.met` sidecar files** — independently list the dev source WAV path (e.g.
   `S:\BigBlueBox\Fable atmos\Waterfall\1644XBADPCM\WaterfallLargeMono_01.wav`) and the same
   format fields; the literal `XBADPCM` / `xbadpcm1644` directory names in Lionhead's own asset
   tree are a second confirmation of the codec.

Fields whose *purpose* is inferred, not proven, are labeled **[hypothesis]**.

EgoCore (`EgoCoreInspect\EgoCore-master`) was checked and has **no audio parsing code** — this
format was cracked byte-level only.

## 1. File inventory

50 `.lug` files under `data\Sound\`, 109 KB (`Waterfall.lug`) to 43 MB (`Ingame.lug`, 2,354
entries — the global SFX bank with the criteria/event map). Each has a matching `.met`. The same
directory also holds 68 loose `.ogg` (music, fanfares, cutscene audio — ordinary Vorbis, no
container) and the per-bank `.met` sidecars.

`text.big` `SpeechBank` strings name `ScriptDialogue.lug/.lut`; the actual dialogue audio is in
the `.lut` (see the sibling doc §1 note) — no `ScriptDialogue.lug` exists on disk.

## 2. On-disk layout

### 2.1 Block stream

```
0x00  char[8]  "LiOnHeAd"
0x08  blocks until EOF, each:
      char[32]  structName   (NUL-padded)
      u32       payloadSize
      u8[payloadSize] payload
```
The walk consumes every retail file exactly (validated ×50). Block order is always:
`LHFileSegmentBankInfo`, `LHAudioWaveData`, `LHAudioBankSampleTable`, `LHAudioBankCriteiaInfo`.

### 2.2 `LHFileSegmentBankInfo` (payload size 0x208 in all retail files)

A single NUL-padded string field: the bank title — `"Big Blue Box - Fable - Ingame SFX"` in
`Ingame.lug`; the literal placeholder `"Bank title/description"` in the ambience banks.

### 2.3 `LHAudioWaveData` — the audio

Payload = complete **RIFF/WAVE files packed back-to-back**, no per-clip header, no padding.
Each RIFF is standard:

- `RIFF <size> WAVE`, `fmt ` chunk, `data` chunk.
- **Xbox ADPCM clips** (98.7%): `fmt ` size 20, `wFormatTag=0x0069`, 1–2 ch, 22050 or 44100 Hz,
  `nBlockAlign=36*ch`, `wBitsPerSample=4`, `cbSize=2`, extra=`0x0040` (64 samples/block) — byte
  identical convention to the `.lut` dialogue clips.
- **PCM clips** (1.3%, GUI sounds in `Frontend.lug`, one in `woodland.lug`, 49 more in
  `night.lug`/`Prison.lug` etc.): `fmt ` size 16, `wFormatTag=0x0001`, 16-bit.

Clip duration = `dataSize / nAvgBytesPerSec` (or table `riffSize/avgBps` to good approximation).

### 2.4 `LHAudioBankSampleTable` — the directory

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
  HookCoastVillage, Lost_Bay, Snowspire) have `riffSize=0` — reserved ids with no clip
  (same idea as the `.lut` `ScriptDialogue2` gaps).
- **Aliases:** 288 records point at a RIFF also pointed at by another record (dedup — e.g. the
  same footstep clip reused under several ids). Set of (offset,size) pairs in the table ==
  set from an independent RIFF walk of the wave block, all 50 files.

### 2.5 `LHAudioBankCriteiaInfo` — event-tag → sample map

```
u32 count
count × { u32 len; char[len] tag; u32 n; u32 sampleId[n] }
```
Tags are semicolon-joined criteria, e.g. `SI_HERO;SE_FOOTSTEP;MATERIAL_GRASS` →
10 sample ids (the footstep variations the engine picks among), or
`CS_CREATE_PARTICLE_ENFLAME_RING_RELEASE_LEVEL_1` → 1 id. `Ingame.lug` has 1,490 tags;
region ambience banks have 0–7 (`count=0` payload is just the u32). This is the engine's
sound-event resolution table — how gameplay code (surface material, creature, spell…) selects
a sample id. Total across all banks: 1,497 tags.

## 3. `.met` sidecar (build metadata, not required for extraction)

Small binary per bank: `u32×3` (1,1,1) header, then per sample: `u32 pathLen + path` (dev source
WAV), followed by the same size/format/params fields seen in the 652-byte table record (riffSize,
fmt 0x0069, rate, the 0xFFFFFFFF pair, priority, min/max distance floats…). It mirrors the sample
table minus the wave data — evidently the bank compiler's input/manifest. Not parsed further
[hypothesis: exact field order not mapped; redundant with the `.lug` itself].

## 4. Codec stats (whole-corpus, from the sample tables, cross-checked vs RIFF fmt chunks)

| codec | ch | rate | bits | clips |
|-------|---:|-----:|-----:|------:|
| XBOX_ADPCM (0x0069) | 1 | 22050 | 4 | 2,623 |
| XBOX_ADPCM | 1 | 44100 | 4 | 1,091 |
| XBOX_ADPCM | 2 | 22050 | 4 | 33 |
| XBOX_ADPCM | 2 | 44100 | 4 | 26 |
| PCM (0x0001) | 1 | 22050 | 16 | 29 |
| PCM | 1 | 44100 | 16 | 22 |

3,833 table entries = 3,824 clips + 9 empty slots; 3,545 unique RIFFs (288 aliases);
~125.5 minutes of unique audio.

## 5. Reproduce / extract

```
python tools/parse_lug.py "<install>\data\Sound\village.lug"              # summary + full validation
python tools/parse_lug.py "<install>\data\Sound\Ingame.lug" --list       # per-entry table
python tools/parse_lug.py "<install>\data\Sound\Ingame.lug" --criteria   # event-tag map
python tools/parse_lug.py "<install>\data\Sound\village.lug" --extract 1 cowbell.wav
python tools/parse_lug.py "<install>\data\Sound\Ingame.lug" --extract-all outdir\
ffmpeg -i cowbell.wav out.wav      # Xbox ADPCM -> PCM (or play PCM clips directly)
```

## 6. Validation performed (2026-07-20)

- **Container walk:** all 50 files parse with zero trailing slack; independent RIFF walk of every
  wave block matches the sample table (as sets, allowing aliases/empties) — 50/50 CLEAN.
- **PCM decode:** all 7 `Frontend.lug` clips open and read fully with Python stdlib `wave`.
- **ADPCM decode:** ffmpeg identifies extracted clips as `adpcm_ima_xbox` and decodes to PCM;
  full-bank decode sweep over `Ingame.lug`: **2,354 / 2,354 extracted clips decode with zero
  errors** (`ffmpeg -v error -f null`).

## 7. Open / unverified items

- Sample-table header `unk` u16 (205 in Ingame, 0 in Frontend) and record fields +276/+280 and
  the +576.. param block semantics — **[hypothesis]**; confirming needs the engine loader
  (`LHAudioBankSampleTable` string xref in Fable.exe). **Ghidra follow-up** (lock was held by
  another agent this run).
- `.met` exact field map (redundant for extraction). The writer (§8) does NOT regenerate the
  `.met` sidecar — build-time metadata, presumed unread by the retail engine **[hypothesis]**.
- How the engine picks among multi-id criteria lists (random/round-robin) — runtime question.
- **Engine runtime acceptance** of a written bank (replace/add) — not tested in-game this run
  (no GUI/game launches allowed). Static validation is complete (§8.3).

## 8. WRITE path (`tools/lug_build.py`) — repack / replace / add. SHIPPED 2026-07-20

Writer: `tools/lug_build.py` (imports `parse_lug` for read/validation, `xbadpcm` for codec).
Codec encoder: `tools/xbadpcm.py` (§8.4). All validation below is a real re-parse through
`parse_lug.LugBank.validate()` (zero-slack walk + independent RIFF walk vs table) plus per-clip
byte-diffs and an ffmpeg/stdlib-`wave` decode of the extracted result — not assertions.

### 8.1 Capabilities + evidence

| Capability | Status | Evidence |
|------------|--------|----------|
| **IDENTITY** (read→rebuild, no edit) | **PASS — byte-exact on ALL 50 retail `.lug`** (`IDENTITY-ALL`) | gold oracle run 2026-07-20; includes alias banks (Ingame, 288 aliases) and empty-slot banks (DarkWood…) |
| **REPLACE** existing clip (any size, ADPCM or PCM) | **PASS** | village.lug id 1 (ADPCM via `--encode`) and id 2 (plain PCM): only the target id's clip changed (78/79 byte-identical), extracted clip == input RIFF, ffmpeg/`wave` decode OK |
| **REPLACE aliased clip** | **PASS** (all alias ids follow the blob — by design) | Ingame.lug id 145: changed = {145,146} exactly (its alias pair), 1,490 criteria tags carried verbatim |
| **ADD** new clip + record + criteria tag | **PASS** | village.lug: new id 80 appended, table count 79→80, tag `SI_HERO;SE_FORGE_TEST` → [80] resolves on re-parse, clip extracts byte-exact + decodes |
| **Stable fixpoint** | **PASS** | every written output re-passes IDENTITY byte-exact |

### 8.2 How the rebuild works (surgical raw-byte model)

- Block payloads are carried **verbatim** except: the wave payload is re-emitted from the unique
  RIFF blobs (original offset order; replacements swap a blob in place, adds append), and each
  652-byte record gets `riffSize`/`riffOffset` re-patched. Downstream offsets ripple
  deterministically — that this reproduces retail byte-for-byte on all 50 banks proves the wave
  blob really is back-to-back with no hidden padding/ordering rule.
- **Replace** also refreshes the record's wave-format mirror (+292..+307) from the new RIFF's
  `fmt ` chunk, on every alias record. Aliases share the blob, so replacing one id changes all
  ids in its alias group (validated; there is no per-id storage to de-alias without growing the
  bank — if needed, ADD a new clip instead).
- **Add** clones a template record (`--like ID`, default = last non-empty record) for the
  +576.. playback-param block, then overwrites path (`--path`, default `S:\Forge\<file>`),
  `id` = max+1, `waveId` = max+1 (new blob is appended last, preserving the
  waveId-follows-offset retail order), format fields, optional `--group`. The table header
  `unk` u16 is bumped only if it equaled the non-empty count (its observed invariant),
  else preserved. `--tag "A;B;C"` appends a criteria entry `tag -> [newId]`.
- **Empty slots** (riffSize=0) are preserved verbatim (never re-patched).
- Input WAVs are canonicalized to minimal retail-style RIFFs (PCM16 fmt-16 / XADPCM fmt-20,
  `fmt `+`data` only); `--raw` embeds verbatim; `--encode` transcodes PCM16 → Xbox ADPCM.

### 8.3 Reproduce

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

### 8.4 Codec encode side: `tools/xbadpcm.py` — ffmpeg CANNOT encode this codec

**ffmpeg 8.1 decodes `adpcm_ima_xbox` but ships NO encoder for it** (`ffmpeg -h
encoder=adpcm_ima_xbox` → "no encoders … available"). `tools/xbadpcm.py` is a pure-Python
encoder/decoder. Decode semantics were pinned **byte-exact against ffmpeg** on retail clips
(mono 22050 + 44100, stereo 22050; e.g. 442,240/442,240 samples identical on village.lug id 79):

- Block = `36*ch` bytes: per channel `{i16 predictor, u8 stepIndex, u8 0}` header, then
  8 groups × ch × 4-byte words of nibbles (LOW nibble first, channels interleaved per word).
- **64 samples out per block per channel = the header predictor sample + the first 63 nibble
  expansions; the 64th nibble's sample is discarded** (hence `avgBytesPerSec =
  rate*blockAlign/64` exactly, floored: 12403 @ 22050 mono — the writer emits this retail-exact).
- Nibble expansion is the *precise* IMA variant: `diff = ((2*(n&7)+1)*step)>>3`.
- Retail blocks are **independent** (predictor = exact source sample s0 of the block; state does
  NOT continue across blocks — verified: 0/926 next-block headers equal the continued decode).
- Encoder convention: header = exact s0 + running step index; 63 nibbles encode s1..s63; the
  spare 64th nibble encodes the next block's s0 (output discarded by decoders). Input padded
  with zeros to whole blocks.
- Quality (encode → ffmpeg decode vs source): 32–36 dB SNR on tones, 22–34 dB re-encoding
  retail material — normal for 4-bit IMA ADPCM.

`xbadpcm.py encode/decode in.wav out.wav` also works standalone, and closes the encode-side gap
for the `.lut` dialogue writer (`docs/LUT_WRITER.md` §4 — its suggested
`ffmpeg -c:a adpcm_ima_xbox` does not actually exist). **Custom dialogue chain PROVEN:** PCM
tone → `xbadpcm.encode_wav` → `lut_write.py REPLACE --allow-resize` on a Dialogue2.lut copy →
`parse_lut.py --extract` returns the injected RIFF byte-exact and ffmpeg decodes it.

Alternatively, plain **PCM16 works without any encoder**: 51 retail `.lug` clips are plain PCM
(fmt 0x0001), so the engine's `.lug` path demonstrably handles PCM records — `lug_build.py`
accepts PCM16 WAVs directly. (No retail `.lut` clip is PCM, so for dialogue stick to XADPCM.)
