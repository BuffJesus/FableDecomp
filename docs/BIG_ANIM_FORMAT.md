# graphics.big MBANK — skeletal-animation format (3DAF / ANIM_*)

*Authored 2026-07-19 (animation-RE agent). Status: **3DAF chunk header + ANRT duration +
AOBJ target-rig + XSEQ format fingerprint + animated-bone identification VALIDATED
self-consistently across all 3,435 retail ANIM_* entries.** The **XSEQ per-bone keyframe
tracks** (quaternion rotation / translation / key times) are a **packed stream** and are
**identified but NOT byte-decoded** — a documented gap that needs the engine deserializer
(scoped Ghidra follow-up in §6), not pure bytes.*

Parser: `tools/parse_anim.py` (pure Python; depends read-only on `tools/parse_bigb.py`).
Sources: raw retail bytes of `graphics.big`; the `Fable.exe` RTTI class table (string scan);
the fabletlcmod.com wiki chunk glossary (format shape only — its `3daf` byte-spec page does
not exist; TLS cert also fails, so wiki content is used only as a low-reliability cross-check
and flagged as such). Fields whose *purpose* is inferred are marked **[hypothesis]**.

This doc is the animation companion to `docs/BIG_MESH_FORMAT.md` / `docs/BIG_MESH_GEOMETRY.md`
(Types 6/7/9 were declared "out of scope / future work" there). It does not modify them.

---

## 0. TL;DR

- MBANK Types **6 (3,272) / 7 (56) / 9 (107)** = **3,435 `ANIM_*` entries**, each a Big Blue Box
  **3DAF ("3D Animation File")** image — the *compiled/packed* runtime form emitted by
  **`C3DAnimFileXLoader`** (RTTI confirmed in `Fable.exe`).
- The 3DAF **header is plain and self-describing** (FourCC chunks). Decoded + validated:
  - `version == 0x00403e12` on **3,435/3,435**;   `3DAF` precedes `ANRT` on **3,435/3,435**.
  - **`ANRT` total duration** (f32) == the TOC `Info[0]` playback float on **3,141/3,285**
    nonzero-Info entries (95.6 %).
  - **`AOBJ` target-rig name** recovered on **3,286/3,435** (e.g. `male_villager_a_complete`,
    `HeroUnclothed`).
  - **`XSEQ` format magic `0x000d7ada`** present on **3,101/3,435**.
- The **`XSEQ` sequence chunk is PACKED**: the header's `decompSize` (payload dword 0) is
  **1.02–5.09× larger** (median 1.27×) than the on-disk payload on **all 3,435** entries — so
  the keyframe body is a compressed/packed stream, not a flat array.
- The animation is **quaternion-based** (wiki + biped-rig evidence). The **animated bone set is
  identified** by cleartext 3ds-Max **Biped** name fragments surviving inside XSEQ (`Scene Root`,
  `Bip01 Pelvis`, `Spine`, `Neck`, `Head`, `L/R Clavicle`, `UpperArm`, `Forearm`, `Hand`,
  `Thigh`, `Calf`, `Foot`, `Toe`, `Ponytail`, `Tail`, plus `Movement_dummy`/`Sub_m` helpers).
- **NOT recovered byte-level:** the per-bone quaternion/translation key values, key times, key
  counts, and per-frame index tables. They are interleaved through the Big-Blue-Box CBYTE-varint
  framing inside XSEQ and are not locatable by pattern-scan (same failure mode the mesh geometry
  agent documented). This is the scoped Ghidra follow-up (§6).

> **Correction of the prior draft.** An earlier `parse_anim.py` claimed XSEQ was **LZO1X**-
> compressed (decompress from the `AOBJ` tag) and that **fps == 30** with `frameCount = round(dur*30)`.
> **Both are refuted by the retail bytes and were removed:** LZO1X (`lionhead_lz.decompress`) fails
> on essentially every entry (`bad match distance …`), and **no f32 `30.0` occurs anywhere in the
> entire bank** (`00 00 f0 41` count = 0/3435). The prior "validated" keyframe layout parsed **0
> bones / 0 keyframes** on real entries. It was a false start; this doc supersedes it.

---

## 1. Bank position & entry classification

`parse_bigb` walks the container; the animation axis is the MBANK entry `Type`:

| Type | count | InfoSize | family | note |
|-----:|------:|---------:|--------|------|
| 6 | 3,272 | 24 | 3DAF anim | the bulk |
| 7 | 56 | 24 | 3DAF anim | |
| 9 | 107 | 24 | 3DAF anim | |

- **TOC `Info` (24 B) = 6 × f32.** `Info[0]` = the animation playback **duration** (validated ==
  the ANRT duration on 95.6 %; the two can differ by a few % — see §3). `Info[1]` is a second
  time/scale float (often ≈ duration, e.g. `1.15 / 1.10`). `Info[2..4]` are small signed floats
  — **[hypothesis: a root-motion / movement-vector triple]** (they echo the `MVEC` chunk values,
  §5). `Info[5]` observed 0. Many entries have all-zero `Info` (static/looping props like
  `ANIM_FOCAL_SITE_*`, `ANIM_*_DOOR_*`).
- **CRC classes:** `0x73ab9c0d` (3,347) + `0x827199a7` (88). Per `BIG_ROUNDTRIP.md` §7 the TOC
  CRC is a **format-class fingerprint**; the two anim CRCs are the two 3DAF serialization classes
  (the minority class correlates with the `AMSK` bone-mask variant, §5). **[hypothesis]**

---

## 2. 3DAF image layout (payload) — ✅ header VALIDATED

Little-endian. The payload begins with an 8-byte prefix, then the FourCC-chunk body:

```
u32   decompSize        # size the packed image expands to; > payload size on ALL entries
                        #   (ratio 1.02–5.09x, med 1.27x) => XSEQ body is PACKED
u32   version = 0x00403e12          # constant across all 3,435 entries
u16   ??                            # top of the first chunk's CBYTE framing (varies)
'3DAF'  <CBYTE framing>  "Copyright Big Blue Box Studios Ltd." '\0'
'ANRT'  <CBYTE framing>  f32 duration              # ANRT = anim-root chunk (C3DAnimFileRootChunk)
'AOBJ'  <CBYTE framing>  <rig-name string>         # animated object (C3DAnimFileObjectChunk)
'XSEQ'  <CBYTE framing>  <magic 0x000d7ada> ...     # sequence chunk (C3DAnimFileXSequenceChunk) — PACKED
'HLPR'  ...                                         # helpers   (C3DAnimFileHelpersChunk)
'MVEC'  ...  f32[3]                                 # movement/root-motion vector (C3DAnimFileMovementVectorChunk)
['TMEV' ...]  (624 entries)                         # timing events (C3DAnimFileTimingEventChunk)
['AMSK' ...]  (34 entries)                          # bone mask (C3DAnimFileBoneMaskChunk)
['XALO' ...]  (914 entries)                         # X-allocation-size helper (C3DAnimFileXAllocationSizeChunk)
```

**Chunk presence over all 3,435 entries:** `3DAF 3435 · ANRT 3435 · AOBJ 3286 · XSEQ 3419 ·
HLPR 3352 · MVEC 3316 · TMEV 624 · AMSK 34 · XALO 914`. (The 16 without `XSEQ` are empty/degenerate
markers such as some `ANIM_FOCAL_SITE_*`.)

The chunk-length framing after each FourCC is a **control-byte varint (CBYTE)** — the *same* scheme
the mesh chunk readers use (`WriteCBYTE`/`ReadCBYTE`, per `BIG_MESH_GEOMETRY.md`). This parser locates
fields by the stable FourCC anchors + the ANRT+9 duration slot + the XSEQ magic, which is robust for
a self-consistent *read*; a byte-exact re-serializer needs the exact CBYTE framing walk (Ghidra, §6).

### ANRT duration slot — ✅ VALIDATED
The duration `f32` sits at **`ANRT + 9`** (i.e. FourCC(4) + a 5-byte CBYTE frame). Scanning the ANRT
window for an f32 equal to `Info[0]`: **3,141 hit at +9**, 5 at +10, and 139 have no match in-window
(those are entries where ANRT's own duration legitimately differs from the TOC `Info[0]` playback
value — a real content fact, e.g. `ANIM_BIPED_GENERIC_MAN_TALK_IDLE_02`: ANRT 2.047 s vs Info 2.000 s).

---

## 3. Timing

- **Duration** is stored explicitly (ANRT f32; §2), in **seconds** — not derived from a frame count.
- The prior "fps = 30, frameCount = round(dur·30)" model is **false** (no `30.0` float exists in the
  bank). The sampling rate, if fixed, is **not 30**; a lone f32 ≈ **20.0** appears right after the
  `Scene Root` bone token in some entries (`… Root  77 03 a0 41` → `20.0`) — **[hypothesis: 20 fps
  or a per-bone key-rate field]**, unconfirmed. Recovering the true key rate / key count needs the
  XSEQ decode (§6).
- `TOC Info[0]` ≈ ANRT duration (95.6 %); treat **ANRT as the animation's intrinsic length** and
  `Info[0]` as the resource's declared **playback duration** (usually identical).

---

## 4. Target rig & animated bone set — ✅ identified (bones), keys NOT decoded

- **`AOBJ` names the target rig** the clip drives: e.g. `HeroUnclothed`, `male_villager_a_complete`,
  `Power_Generator_Wet`, `Crypt_door`. Recovered on 3,286/3,435. (The rig name is CBYTE-length-
  prefixed; a framing byte can bleed onto the last character in the raw run — cosmetic.)
- **The rig is a 3ds-Max Biped** — the same bone-name hierarchy the mesh payloads embed
  (`BIG_MESH_FORMAT.md` §3): `Scene Root`, `Bip01`, `Bip01 Pelvis`, `Spine`, `Neck`, `Head`,
  `L/R Clavicle`, `L/R UpperArm`, `Forearm`, `Hand`, `Finger`, `L/R Thigh`, `Calf`, `Foot`, `Toe`,
  `Ponytail`, `Tail`, plus non-biped helpers `Movement_dummy` / `Sub_m`. These survive as **cleartext
  fragments inside the packed XSEQ**, so the *animated bone set* is identifiable (lower-bound count
  per clip: `ANIM_HERO_THROW` → ≥9 distinct bones).
- **Exactly one `XSEQ` chunk per entry** (3,419 of 3,435 have exactly 1; the rest 0). So **all bones
  share a single packed sequence blob** — the tracks are concatenated inside one XSEQ, *not* one XSEQ
  per bone. (This is the specific structural fact the prior draft got backwards.)
- **What is NOT recovered:** per-bone key counts, the quaternion rotation keys, translation keys,
  key **times**, and the per-frame → key index tables. The wiki labels XSEQ a *"Compressed Animation
  Sequence"* and the format *"quaternion based"*; the packed layout (decompSize ≫ paysize, CBYTE
  framing, `0x000d7ada` magic) is consistent with that, but the byte offsets of the key arrays are
  computed by the deserializer, not findable by scan (§6).

---

## 5. Secondary chunks

| FourCC | RTTI class | count | role (evidence) |
|--------|-----------|------:|-----------------|
| `ANRT` | `C3DAnimFileRootChunk` | 3435 | root: total duration f32 (§2) |
| `AOBJ` | `C3DAnimFileObjectChunk` | 3286 | animated-object: target rig name (§4) |
| `XSEQ` | `C3DAnimFileXSequenceChunk` | 3419 | **packed keyframe sequence** (magic `0x000d7ada`) |
| `HLPR` | `C3DAnimFileHelpersChunk` | 3352 | helper/dummy tracks (`Movement_dummy`, `Sub_m`) |
| `MVEC` | `C3DAnimFileMovementVectorChunk` | 3316 | root-motion vector: **f32[3]** after `0c` framing (values echo `Info[2..4]`) — **[hypothesis]** |
| `TMEV` | `C3DAnimFileTimingEventChunk` | 624 | timed animation events (footstep / hit windows) — **[hypothesis]** |
| `AMSK` | `C3DAnimFileBoneMaskChunk` | 34 | partial-body bone mask (upper/lower-body blends) |
| `XALO` | `C3DAnimFileXAllocationSizeChunk` | 914 | runtime allocation-size hint for the packed image |

(Loader/saver classes also present in the binary: `C3DAnimFileLoader`, `C3DAnimFileXLoader`,
`C3DAnimFileSaver`; runtime: `C3DAnimation2`, `C3DAnimationInterpolator`, `C3DAnimationBuilder`,
`C3DAnimationManager`, `C3DAnimationBlendState`, `C3DAnimationStats`, `CTCAnimationComplex`.)

---

## 6. DOCUMENTED GAP — the packed XSEQ keyframe stream (scoped Ghidra follow-up)

**Two byte-only decode attempts failed** (loop-prevention limit reached, so byte-guessing stops here):
1. **Inherited hypothesis — LZO1X** decompress of the `AOBJ..end` body: fails on ~every entry
   (`lzo1x: bad match distance`). LZO1X is the *texture* codec (`lionhead_lz`), not the anim codec.
2. **Flat/marker scan** for `u16 keyCount` + `nKeys×(4×f32 quat)` + a per-frame index table (the
   prior draft's model): yields **0 unit-quaternions / 0 valid tracks** on real entries; the `30 fps`
   anchor it keyed on does not exist.

The uniform failure is itself evidence the tracks are **packed** (decompSize ≫ paysize on all 3,435;
CBYTE-varint framing; `0x000d7ada` magic). Recovering them needs the engine deserializer — a Ghidra
target, **well-scoped by the RTTI names already in `Fable.exe`**:

- **Decompile `C3DAnimFileXSequenceChunk::ReadFromFile` / `LoadBinary`** (the `X` = compiled/packed
  reader, mirroring the mesh `C3DMeshFileX*Chunk` family that `BIG_MESH_GEOMETRY.md` proved does a
  deterministic serialized-image walk). This reader defines: per-bone key counts, the quaternion key
  encoding (full `4×f32` vs packed/smallest-three), translation key encoding, the key-**time**
  encoding (absolute f32 vs frame-index vs delta), and the per-frame index/interpolation table.
- **Cross-refs to pin:** `C3DAnimFileXLoader` (top-level 3DAF deserializer + chunk dispatch),
  `C3DAnimation2::LoadBinary` (runtime target of the load), `C3DAnimationInterpolator`
  (proves rotation = quaternion **slerp**, translation = lerp), `C3DAnimFileXAllocationSizeChunk`
  (the `XALO` value = the exact byte size the unpacked image must reach — an oracle for a re-packer).
- **String anchors for the search:** RTTI `.?AVC3DAnimFileX*Chunk@@`, the literal `#<XSEQ`, and the
  `0x000d7ada` constant. **No Ghidra lock is taken by this agent** — this is a scoped hand-off.

Once the XSEQ reader is decompiled, §2–§5 already give the surrounding structure, so the keyframe
shape (bone→[ (time, quat, trans) ]) falls out directly — as with the mesh case, the hard unknown
(is it a generic codec, and which — **answer: no LZO1X; it's a bespoke packed serialization**) is
closed here; what remains is a finite reader port.

---

## 7. Custom-animation feasibility verdict

- **Read/inspect metadata: GO now.** Duration, target rig, animated-bone set, chunk map, and CRC
  class are decoded and validated. You can enumerate every clip, see what rig it drives and which
  bones it touches, and read its length — enough to *catalogue* and *retarget-by-name* animations.
- **Retarget an existing clip to a different (compatible) rig / swap a whole ANIM entry: GO.** It's a
  payload-blob copy + TOC descriptor, both reproduced byte-exact by the `.big` round-trip harness
  (`BIG_ROUNDTRIP.md`); compatibility depends on matching the Biped bone set the consuming object
  expects.
- **Author / edit keyframes (new motion): NO-GO until `C3DAnimFileXSequenceChunk` is reversed (§6).**
  Writing motion requires emitting the packed XSEQ stream (quaternion keys + times + index tables in
  the exact CBYTE framing). This is **not** blocked on a generic codec (there is none — LZO1X is
  refuted); it is blocked on the (now-specified) serialization reader/writer port. The community
  historically edited animations via **Fable Explorer / Max-based tools** that embed BBB's own
  writer — consistent with this finding.

---

## 8. Reproduce

```
python tools/parse_anim.py "<install>\data\graphics\graphics.big"                 # enumerate + validate
python tools/parse_anim.py "<install>\data\graphics\graphics.big" --entry ANIM_HERO_THROW
python tools/parse_anim.py "<install>\data\graphics\graphics.big" --validate 8    # deep header check
```
Validated numbers (retail `graphics.big`, 3,435 ANIM_* entries):
`version==0x00403e12` **3435/3435** · `3DAF<ANRT` **3435/3435** · AOBJ rig **3286/3435** ·
ANRT duration == Info[0] **3141/3285** nonzero-Info · XSEQ magic `0x000d7ada` **3101/3435** ·
decompSize/paysize ratio **1.02–5.09×** (XSEQ packed). Keyframe-track byte decode: **PENDING Ghidra**
(`C3DAnimFileXSequenceChunk`, §6).
```

---

## 9. 2026-07-20 — XSEQ KEYFRAME PAYLOAD **FULLY DECODED** (supersedes §6/§8 "PENDING Ghidra")

**The gap is closed — no Ghidra needed.** The answer key was EgoCore's `AnimParser.h`
(`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\EgoCore\Animations\AnimParser.h`),
ported to **`tools/parse_anim_xseq.py`** and validated against every retail entry.

### 9.1 What §2–§6 got wrong (and why)
- The 3DAF payload after the leading `u32 decompSize` is **ONE raw LZO1X stream** —
  **no per-block `[u16 clen]` frame headers** (that framing is the texture/mesh scheme).
  The prior LZO attempt failed because it (a) started at the `AOBJ` tag instead of offset 4
  and (b) used the chunked-frame walker. `lionhead_lz.decompress(payload[4:], decompSize)`
  succeeds on **3,435/3,435** entries. If the first u32 is `0x3E3E3E3E` (`">>>>"`) the payload
  is already uncompressed.
- Everything §2 read past offset 8 — "version 0x00403e12", "CBYTE varint framing",
  "XSEQ magic 0x000d7ada", "ANRT+9 duration slot" — was **compressed-stream artifact**
  (LZO literal-run bytes of the constant header text). Chunk framing in the *decompressed*
  image is plain **`[char4 fourcc][u32 payloadSize]`**.
- "Exactly one XSEQ per entry" is backwards: there is **one XSEQ chunk PER BONE TRACK**
  (e.g. `ANIM_HERO_THROW` = 70 XSEQ chunks = 70 bone tracks).

### 9.2 Decompressed 3DAF layout (VALIDATED byte-exact on all 3,435)
```
u32 0x3E3E3E3E  '3DAF'  u32  <copyright cstr>  pad-to-4
chunk := fourcc[4] , u32 size , payload[size]        # chunks may nest (superchunks)
 ANRT: u8 isCyclic ; f32 duration ; <hdr bytes> ; nested chunks
 AOBJ: cstr rigName ; <hdr bytes> ; nested chunks
 HLPR: nested chunks (helper-object tracks)
 MVEC: f32[3] movementVector ; nested chunks (superchunk!)
 TMEV: cstr eventName ; f32 time
 AMSK: bone-mask bits     XALO: alloc-size hint
 XSEQ (or 'SEQ0'): ONE BONE TRACK:
   u32 boneIndex ; i32 parentIndex ; cstr boneName ;
   u8 preFPSFlag ; f32 samplesPerSecond ; u32 frameCount ; u8 postFrameFlags[4] ;
   f32 positionFactor ; f32 scalingFactor ;
   u16 rotCount    ; rotCount x f32[4] quat (x,y,z,w) KEY POOL
   u16 palRotCount ; palRotCount x (u8 if rotCount<=255 else u16)   frame->pool idx
   u16 posCount    ; posCount x i16[3] ; pos = int16 * positionFactor  KEY POOL
   u16 palPosCount ; palPosCount x (u8 if posCount<=255 else u16)   frame->pool idx
```
- **Palette tables = keyframe reduction**: frame *i* samples `pool[pal[i]]`; empty palette
  ⇒ pool is dense per-frame. Runtime interpolates (quat hemisphere-nlerp/slerp, pos lerp) —
  see EgoCore `AnimTrack::EvaluateTimeSmooth` for the exact reconstruction the exporter uses.
- **fps is per-track** (`SamplesPerSecond`, f32). The lone "≈20.0 after Scene Root" float in
  §3 was this field: **20 fps is the common body rate**; the old 30-fps model stays refuted.
  `frameCount/fps == ANRT duration` (e.g. HERO_THROW 23/20 = 1.15 s exact).
- `parentIndex` chains tracks into the skeleton hierarchy (track order = index space);
  `boneIndex` is a constant per clip (rig id/hash, not a per-bone index).

### 9.3 Validation evidence (retail graphics.big, `--validate` full run)
- **3,435/3,435 entries parse; 0 failures.** 213,728 tracks total.
- **4,742,575 quaternion keys; only 5 non-unit-norm** (>1% off unit) — 99.9999 % unit.
- **0 palette indices out of bounds; 0 palette-length↔frameCount mismatches;
  0 unconsumed tail bytes** in any XSEQ chunk (byte-exact consumption).
- `pos_far` (|coord|>100 m) = 116,905/1,032,405 keys — concentrated in Scene-Root/world
  tracks of long cutscene clips; not a decode error (all other invariants hold).

### 9.4 Tools
```
python tools/parse_anim_xseq.py <graphics.big> --list [PATTERN]
python tools/parse_anim_xseq.py <graphics.big> --entry ANIM_HERO_THROW [--json out.json]
python tools/parse_anim_xseq.py <graphics.big> --validate      # full-bank invariant check
```
`parse_anim.py` (§8) remains for the TOC/Info side; its §2–§6 byte claims are superseded.

### 9.5 Feasibility verdict update (supersedes §7)
- **Read/export keyframes: GO — done.** Full per-bone curves (times = frame/fps, quat, pos).
- **Author/edit keyframes: GO in principle.** Writing = emit the §9.2 layout and LZO1X-compress
  (EgoCore `CompressLZORaw` shows the writer; append `11 00 00` EOS, prefix u32 decompSize).
  EgoCore also ships `AnimCompiler.h`/`GltfAnimImporter.h` as reference writers.
- Ghidra is **no longer needed** for the animation format.

### 9.6 Related: lipsync + demon door
- dialogue.big LIPSYNC grammar was already byte-exact in `parse_bigb.decode_lipsync`; EgoCore
  `LipSyncParser.h` confirms it field-for-field and resolves the "blockMarker 0x2B" as the
  **FPS field: 43 = int(22050/512)** (one lipsync frame per 512 audio samples @22.05 kHz).
- TOC **Type 9 = single-frame phoneme/viseme POSE anims** (frameCount=1, duration 0, AMSK
  face mask) blended by lipsync viseme weights; Type 6 = normal clips; Type 7 = same 3DAF
  form (subset TBD). End-to-end demon-door case study: `docs/DEMON_DOOR_FACE.md`.
  - **Reconcile (EgoCore, 2026-07-31):** EgoCore's primary discriminator for Type 9 is the
    presence of an **AMSK partial-body mask**, not frameCount==1 — i.e. Type 9 = "masked/partial
    anim" generally (a viseme face pose is one case). **Type 7 = DELTA (bind-relative) animation.**
    Also: BoneIndex sentinel **`31450`** = "no global rig ID" for a track. See
    `docs/EGOCORE_ASSESSMENT_20260731.md`. The GLTF-anim retargeter (`GltfAnimImporter.h`) is the
    reference for the Mario-rig parent-relative-bind-translation retarget (port target).
