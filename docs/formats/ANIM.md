# graphics.big MBANK skeletal animations (3DAF / ANIM_*) + LIPSYNC — format, decode, writer

*Merged reference (2026-09-07) of `BIG_ANIM_FORMAT.md` (authored 2026-07-19, §9 added 2026-07-20,
EgoCore reconcile 2026-07-31) and `ANIM_WRITER.md` (2026-07-20, Blender bridge 2026-07-20).
Container layout / TOC / writer live in [BIG.md](BIG.md); compiled meshes (types 1/2/4/5) in
[MESH.md](MESH.md); the LZO1X codec in [LIONHEAD_LZ.md](LIONHEAD_LZ.md); the lipsync→mouth chain
in `docs/engine/DEMON_DOOR_FACE.md`; the dialogue join (`.lut` / snds.bin) in
[AUDIO.md](AUDIO.md).*

**Status: READ + WRITE both SHIPPED & VALIDATED.** The 3DAF payload is fully decoded (every byte of
every retail entry is explained by structured fields, 3,435/3,435) and re-composable byte-exact at
the decompressed-image level; a composer (`tools/anim_build.py`) and a LIPSYNC composer
(`tools/lipsync_build.py`) produce edited and brand-new entries. Ghidra is **not needed** for the
animation format. Fields whose *purpose* is inferred are marked **[hypothesis]**.

Tools: `tools/parse_anim.py` (TOC/Info side; pure Python, read-only on `tools/parse_bigb.py`),
`tools/parse_anim_xseq.py` (full 3DAF/XSEQ decoder + `--validate`), `tools/anim_build.py` (3DAF
composer), `tools/lipsync_build.py` (LIPSYNC composer), `tools/lionhead_lz_compress.py`,
`tools/big_write.py` (`rebuild(edits=, adds=)`), `tools/blender_addon/io_scene_fable/anim_ops.py`
(Blender bridge).

Sources: raw retail bytes of `graphics.big` / `dialogue.big`; the `Fable.exe` RTTI class table
(string scan); **EgoCore** (MIT, `C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master\EgoCore\`
— `Animations\AnimParser.h` is the answer key, plus `AnimCompiler.h`, `GltfAnimImporter.h`,
`LipSyncParser.h`, `LipSyncCompiler.h`); the fabletlcmod.com wiki chunk glossary (format shape
only — its `3daf` byte-spec page does not exist; TLS cert also fails, so wiki content is a
low-reliability cross-check and flagged as such).

---

## 0. TL;DR (current truth)

- MBANK Types **6 (3,272) / 7 (56) / 9 (107)** = **3,435 `ANIM_*` entries**, each a Big Blue Box
  **3DAF ("3D Animation File")** image — the *compiled/packed* runtime form emitted by
  **`C3DAnimFileXLoader`** (RTTI confirmed in `Fable.exe`).
- **Payload = `u32 decompSize` + ONE raw LZO1X stream** — no per-block `[u16 clen]` frame headers
  (that framing is the texture/mesh scheme). `lionhead_lz.decompress(payload[4:], decompSize)`
  succeeds on **3,435/3,435**. If the first u32 is `0x3E3E3E3E` (`">>>>"`) the payload is already
  uncompressed (loader-accepted per EgoCore).
- Chunk framing in the *decompressed* image is plain **`[char4 fourcc][u32 payloadSize]`**; chunks
  may nest (superchunks).
- There is **one `XSEQ` chunk PER BONE TRACK** (e.g. `ANIM_HERO_THROW` = 70 XSEQ chunks = 70 bone
  tracks). Each track = quaternion key pool + optional frame→pool palette, i16 position key pool
  + optional palette, per-track `SamplesPerSecond` (20 fps is the common body rate).
- TOC `Info` (24 B) = `C3DAnimationInfo::Serialize`: `f32 duration, f32 nonLoopingDuration,
  f32[3] movementVector, f32 rotation`.
- The animation is **quaternion-based**; the rig is a 3ds-Max **Biped** (same bone-name hierarchy
  the mesh payloads embed).
- Everything the 2026-07-19 draft read past payload offset 8 ("version 0x00403e12", CBYTE varint
  framing, "XSEQ magic 0x000d7ada", "ANRT+9 duration slot", "exactly one XSEQ per entry") was a
  **compressed-stream artifact** — kept for the record under [Superseded](#13-superseded).

---

## 1. Bank position & entry classification

`parse_bigb` walks the container; the animation axis is the MBANK entry `Type`:

| Type | count | InfoSize | family | note |
|-----:|------:|---------:|--------|------|
| 6 | 3,272 | 24 | 3DAF anim | the bulk — normal clips |
| 7 | 56 | 24 | 3DAF anim | **DELTA (bind-relative) animation** (EgoCore, 2026-07-31) |
| 9 | 107 | 24 | 3DAF anim | masked/partial anims (AMSK) — incl. single-frame phoneme/viseme POSE anims (see §6) |

- **TOC `Info` (24 B) = 6 × f32 — layout CONFIRMED** against EgoCore's `C3DAnimationInfo`
  (`Animations/AnimParser.h`, `Deserialize` reads +0/+4/+8/+20):
  - `Info[0]` = **Duration** (playback duration; == the ANRT duration on 95.6 % of nonzero-Info
    entries, the two can differ by a few % — see §4).
  - `Info[1]` = **NonLoopingDuration** — the "second time float" (often ≈ duration, e.g.
    `1.15/1.10`); it's the non-looping clip length, not a scale.
  - `Info[2..4]` = **MovementVector** — root-motion translation `f32[3]`; matches the `MVEC` chunk
    values (§3). *(Was a hypothesis; EgoCore names it as a first-class field.)*
  - `Info[5]` = **Rotation** — root-motion yaw `f32`; 0 on non-turning anims, which is why it read
    as "observed 0".

  Many entries have all-zero `Info` (static/looping props like `ANIM_FOCAL_SITE_*`,
  `ANIM_*_DOOR_*`). Writer helper: `anim_info_blob(duration/nonLooping/mvec/rotation)`.
- **CRC classes:** `0x73ab9c0d` (3,347) + `0x827199a7` (88). Per [BIG.md](BIG.md) (round-trip
  §7) the TOC CRC is a **format-class fingerprint**; the two anim CRCs are the two 3DAF
  serialization classes (the minority class correlates with the `AMSK` bone-mask variant).
  **[hypothesis]**

---

## 2. Payload container — `u32 decompSize` + one raw LZO1X stream

```
u32   decompSize        # size the LZO1X stream expands to; > payload size on ALL entries
                        #   (ratio 1.02–5.09x, median 1.27x)
u8[]  LZO1X stream      # ONE raw stream (lionhead_lz.decompress(payload[4:], decompSize)),
                        #   ends with the 11 00 00 EOS marker
```

- If the leading u32 is **`0x3E3E3E3E`** (`">>>>"`) the payload is the raw (uncompressed) image
  itself — loader-accepted per EgoCore; `compose_payload(img)` may emit either form.
- Writing = `u32 len` + `lionhead_lz_compress.compress(img)` (EgoCore `CompressLZORaw` shows the
  writer; append `11 00 00` EOS, prefix u32 decompSize). Compressed bytes never match retail
  (valid ≠ identical LZO stream — same situation as the texture writer); the engine only requires
  a valid stream. Retail 87,162,281 B → ours 91,764,865 B (+5.3 %) over all 3,435 entries.
- LZO1X is the same codec as textures/meshes ([LIONHEAD_LZ.md](LIONHEAD_LZ.md)); the difference is
  purely the framing (none here vs `[u16 clen]` chunks there).

---

## 3. Chunk map & RTTI classes

Chunk presence over all 3,435 entries: `3DAF 3435 · ANRT 3435 · AOBJ 3286 · XSEQ 3419 ·
HLPR 3352 · MVEC 3316 · TMEV 624 · AMSK 34 · XALO 914`. (The 16 without `XSEQ` are
empty/degenerate markers such as some `ANIM_FOCAL_SITE_*`.)

| FourCC | RTTI class | count | role (evidence) |
|--------|-----------|------:|-----------------|
| `3DAF` | (file root) | 3435 | `u32 0x3E3E3E3E` + `'3DAF'` + u32 + copyright cstr "Copyright Big Blue Box Studios Ltd." + pad-to-4 |
| `ANRT` | `C3DAnimFileRootChunk` | 3435 | root: `u8 isCyclic ; f32 duration` (seconds) ; freeform header bytes ; nested chunks |
| `AOBJ` | `C3DAnimFileObjectChunk` | 3286 | animated-object: `cstr rigName` (target rig, §5) ; header bytes ; nested chunks |
| `XSEQ` / `SEQ0` | `C3DAnimFileXSequenceChunk` | 3419 entries (213,728 tracks) | **ONE BONE TRACK** each (§9) |
| `HLPR` | `C3DAnimFileHelpersChunk` | 3352 | helper/dummy tracks (`Movement_dummy`, `Sub_m`); nested chunks |
| `MVEC` | `C3DAnimFileMovementVectorChunk` | 3316 | root-motion vector **f32[3]** (== `Info[2..4]`, ✅ via EgoCore `C3DAnimationInfo.MovementVector`); **superchunk** — 12 B vec + nested chunks, incl. a hidden collision track (EgoCore AnimParser) |
| `TMEV` | `C3DAnimFileTimingEventChunk` | 624 | `cstr eventName ; f32 time` — timed animation events (footstep / hit windows) |
| `AMSK` | `C3DAnimFileBoneMaskChunk` | 34 | partial-body bone mask (upper/lower-body blends); payload = **`uint32[]` bitfield, one bit per bone** (EgoCore `BoneMaskBits`) |
| `XALO` | `C3DAnimFileXAllocationSizeChunk` | 914 | runtime allocation-size hint for the unpacked image (an oracle for a re-packer) |

Loader/saver classes also present in the binary: `C3DAnimFileLoader`, `C3DAnimFileXLoader`,
`C3DAnimFileSaver`; runtime: `C3DAnimation2`, `C3DAnimationInterpolator`, `C3DAnimationBuilder`,
`C3DAnimationManager`, `C3DAnimationBlendState`, `C3DAnimationStats`, `CTCAnimationComplex`.
RTTI string anchors: `.?AVC3DAnimFileX*Chunk@@`.

Canonical nesting (EgoCore `AnimCompiler.h`): `ANRT` ⊃ `XALO` + optional `HLPR`(`MVEC`/`TMEV`/
helper-`XSEQ`) + `AOBJ`(rigName) ⊃ [`AMSK`] + one `XSEQ` per bone. Our verifier accepts any nesting
the engine parser accepts. Retail images end with an 8-byte `04 00 00 00 00 00 00 00` footer that
parses as an image-level raw tail.

---

## 4. Timing

- **Duration** is stored explicitly (ANRT f32) in **seconds** — not derived from a frame count.
- **fps is per-track** (`SamplesPerSecond`, f32). **20 fps is the common body rate**;
  `frameCount/fps == ANRT duration` (e.g. HERO_THROW 23/20 = 1.15 s exact). The lone "≈20.0 after
  `Scene Root`" float seen in the compressed bytes (`… Root  77 03 a0 41` → `20.0`) was this field.
- The old **"fps = 30, frameCount = round(dur·30)"** model is **false**: no f32 `30.0`
  (`00 00 f0 41`) occurs anywhere in the bank (count 0/3435).
- `TOC Info[0]` ≈ ANRT duration on **3,141/3,285** nonzero-Info entries (95.6 %). Treat **ANRT as
  the animation's intrinsic length** and `Info[0]` as the resource's declared **playback duration**
  (usually identical). Where they differ it is a real content fact, e.g.
  `ANIM_BIPED_GENERIC_MAN_TALK_IDLE_02`: ANRT 2.047 s vs Info 2.000 s (139 such entries).
- Lipsync runs at **43 fps = int(22050/512)** (one lipsync frame per 512 audio samples @ 22.05 kHz;
  EgoCore `LipSyncParser.h` resolved the former "blockMarker 0x2B" as this FPS field).

---

## 5. Target rig, animated bone set, and track semantics

- **`AOBJ` names the target rig** the clip drives: e.g. `HeroUnclothed`,
  `male_villager_a_complete`, `Power_Generator_Wet`, `Crypt_door`. Present on 3,286/3,435.
- **The rig is a 3ds-Max Biped** — the same bone-name hierarchy the mesh payloads embed
  ([MESH.md](MESH.md) skeleton section): `Scene Root`, `Bip01`, `Bip01 Pelvis`, `Spine`, `Neck`,
  `Head`, `L/R Clavicle`, `L/R UpperArm`, `Forearm`, `Hand`, `Finger`, `L/R Thigh`, `Calf`,
  `Foot`, `Toe`, `Ponytail`, `Tail`, plus non-biped helpers `Movement_dummy` / `Sub_m`.
  (`ANIM_HERO_THROW` → 70 tracks.)
- `parentIndex` chains tracks into the skeleton hierarchy (track order = index space);
  `boneIndex` is a constant per clip (rig id/hash, not a per-bone index). Retail `bone_index` is
  the constant **`31450`** = "no global rig ID" sentinel for a track (EgoCore normalizes to it).
- **Track semantics (probed on retail, Blender bridge 2026-07-20):** an XSEQ track stores the
  bone's FULL parent-relative local transform per frame, in the SAME model space the mesh bind
  poses use (no axis conversion vs the Blender importer): pos key `p` = local translation (bind
  units); quat key `q(x,y,z,w)` = **conjugate** of the mathutils-convention local rotation
  (`q_local = Quaternion((w,-x,-y,-z))`). Proof: for every stationary bone of
  `ANIM_CREATURE_GOBLIN_ATTACK_01` the anim key equals the `MESH_CREATURE_GOBLIN` bind-local
  transform exactly (up to i16 quantization / quaternion sign).
- Runtime interpolates (quat hemisphere-nlerp/slerp, pos lerp) — see EgoCore
  `AnimTrack::EvaluateTimeSmooth` for the exact reconstruction the exporter uses;
  `C3DAnimationInterpolator` is the engine class.
- **Mario-rig gotcha (2026-07-22):** Fable ANIM position keys carry parent-relative bind
  translations, so hero-sized translations pull a smaller rig's disconnected segments out to hero
  joint positions; mesh-only rest/inverse-bind edits are insufficient — animation translation
  retargeting is required. EgoCore `GltfAnimImporter.h` is the reference retargeter (port target).

---

## 6. Type 6 / 7 / 9 semantics and the lipsync relation

- TOC **Type 9 = single-frame phoneme/viseme POSE anims** (frameCount=1, duration 0, AMSK face
  mask) blended by lipsync viseme weights; **Type 6 = normal clips**; Type 7 = same 3DAF form.
  End-to-end demon-door case study: `docs/engine/DEMON_DOOR_FACE.md`.
- **Reconcile (EgoCore, 2026-07-31):** EgoCore's primary discriminator for Type 9 is the presence
  of an **AMSK partial-body mask**, not frameCount==1 — i.e. Type 9 = "masked/partial anim"
  generally (a viseme face pose is one case). **Type 7 = DELTA (bind-relative) animation.** See
  `docs/journal/2026-07/EGOCORE_ASSESSMENT_20260731.md`.
- dialogue.big LIPSYNC grammar was already byte-exact in `parse_bigb.decode_lipsync`; EgoCore
  `LipSyncParser.h` confirms it field-for-field. LIPSYNC payloads are **uncompressed**;
  `encode(decode(x)) == x` on every non-empty entry (20,214/20,214). A LIPSYNC entry = viseme
  dictionary (→ the target creature's `ANIM_*_PHONEME_*` pose set) + weights per 43 fps frame +
  TOC `Info` = f32 duration; deps carry the speaker (e.g. `SPEAKER_FEMALE1`, on nearly everything —
  apparently vestigial).
- The `.lut` voice clip sharing the same `<N>` id/name scheme is documented in
  [AUDIO.md](AUDIO.md).

---

## 7. Feasibility verdict

- **Read/inspect metadata: GO.** Duration, target rig, animated-bone set, chunk map, CRC class —
  enumerate every clip, see what rig it drives, which bones it touches, its length.
- **Read/export keyframes: GO — done.** Full per-bone curves (times = frame/fps, quat, pos).
- **Retarget an existing clip to a compatible rig / swap a whole ANIM entry: GO.** Payload-blob
  copy + TOC descriptor, both reproduced byte-exact by the `.big` round-trip harness
  ([BIG.md](BIG.md)); compatibility depends on matching the Biped bone set the consuming object
  expects.
- **Author/edit keyframes: GO** — emit the §9 layout and LZO1X-compress (§2); `tools/anim_build.py`
  does it (§10). EgoCore also ships `AnimCompiler.h`/`GltfAnimImporter.h` as reference writers.
  The community historically edited animations via **Fable Explorer / Max-based tools** that embed
  BBB's own writer.
- **In-game visual soak untested** (see §12).

---

## 8. Read-side tools & reproduce

```
python tools/parse_anim.py "<install>\data\graphics\graphics.big"                 # enumerate + validate TOC/Info
python tools/parse_anim.py "<install>\data\graphics\graphics.big" --entry ANIM_HERO_THROW
python tools/parse_anim.py "<install>\data\graphics\graphics.big" --validate 8    # deep header check

python tools/parse_anim_xseq.py <graphics.big> --list [PATTERN]
python tools/parse_anim_xseq.py <graphics.big> --entry ANIM_HERO_THROW [--json out.json]
python tools/parse_anim_xseq.py <graphics.big> --validate      # full-bank invariant check
```
`parse_anim.py` remains for the TOC/Info side; its pre-§9 byte claims are superseded (§13).

---

## 9. XSEQ keyframe payload — FULLY DECODED (Section 9)

*This is the "§9" / "Section 9" referenced by `CLAUDE.md`, `HANDOFF.md` and the writer: formerly
`BIG_ANIM_FORMAT.md` §9 (2026-07-20). The gap was closed with EgoCore's `AnimParser.h` ported to
`tools/parse_anim_xseq.py` — no Ghidra needed.*

### 9.1 Decompressed 3DAF layout (VALIDATED byte-exact on all 3,435)

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

- **Palette tables = keyframe reduction**: frame *i* samples `pool[pal[i]]`; empty palette ⇒ pool
  is dense per-frame. Runtime interpolates (quat hemisphere-nlerp/slerp, pos lerp).
- **fps is per-track** (`SamplesPerSecond`, f32); `frameCount/fps == ANRT duration`.
- `parentIndex` chains tracks into the skeleton hierarchy (track order = index space);
  `boneIndex` is a constant per clip (rig id/hash, not a per-bone index; retail `31450`).

### 9.2 Format facts the writer nailed down (delta over the read side)

- XSEQ sections are **optional from the end**: a track may legally stop after any of the four
  `u16`-counted sections (rotPool / rotPal / posPool / posPal). `Track.has_*` flags reproduce
  truncated retail tracks; encode emits sections only up to the last present.
- Palette index width is decided by the *pool* count (`u8` if count ≤ 255 else `u16`) — same rule
  both directions.
- ANRT/AOBJ carry **freeform header bytes** between their fixed head and the first nested chunk
  (found by fourcc stop-scan: ANRT→{HLPR,AOBJ,XALO}, AOBJ→{XSEQ,SEQ0,AMSK}); preserved verbatim
  (`Node.head`). MVEC is a superchunk (12 B vec + nested chunks). Trailing non-chunk bytes at any
  level are kept as raw tails (`Node.tail`, image trailer — retail images end with an 8-byte
  `04 00 00 00 00 00 00 00` footer that parses as tail).
- f32 cells (fps, positionFactor, scalingFactor, duration, quat pool) are stored as raw 4-byte
  cells in the model so any NaN payload/denormal round-trips bit-exact; position keys are `i16×3`
  ints (exact).
- `Track.pre_fps_flag` and `post_frame_flags`: EgoCore writes `flags[0..2] = {any,rotMode,posMode}`
  (0/1/2 = none/constant/animated); retail data matches loosely; preserved verbatim on edit, donor
  values copied on authoring. Engine tolerance to "wrong" flags unprobed.
- `SEQ0` (vs `XSEQ`) never appears in retail graphics.big tracks touched so far; parser accepts
  both, writer emits whatever the node carries.

### 9.3 Read-side validation evidence (retail graphics.big, `--validate` full run)

- **3,435/3,435 entries parse; 0 failures.** 213,728 tracks total.
- **4,742,575 quaternion keys; only 5 non-unit-norm** (>1% off unit) — 99.9999 % unit.
- **0 palette indices out of bounds; 0 palette-length↔frameCount mismatches; 0 unconsumed tail
  bytes** in any XSEQ chunk (byte-exact consumption).
- `pos_far` (|coord|>100 m) = 116,905/1,032,405 keys — concentrated in Scene-Root/world tracks of
  long cutscene clips; not a decode error (all other invariants hold).

---

## 10. Writer — `tools/anim_build.py` (3DAF) + `tools/lipsync_build.py` (LIPSYNC)

*2026-07-20. Answer key: EgoCore `AnimCompiler.h` / `LipSyncCompiler.h` (ported semantics; our
writers are lossless field-level models instead of EgoCore's re-deriving compiler). All validation
ran against COPIES of the retail .bigs in `work/anim_write/` — originals untouched. Consumes
`tools/big_write.py` (`rebuild(edits=, adds=)`, unmodified), `tools/lionhead_lz_compress.py`,
`tools/parse_anim_xseq.py`, `tools/parse_bigb.py`.*

### 10.1 Round-trip proof (retail copies)

graphics.big — 3,435 ANIM entries (types 6/7/9):
```
python tools/anim_build.py work/anim_write/graphics_copy.big --roundtrip
  image recompose byte-exact : 3435/3435  (0 diffs, 0 raw-fallback nodes, 213,728 tracks)
  recompress->decompress==img: 3435/3435
  compressed bytes == retail : 0/3435    (expected: different LZO match choices)
  compressed size: retail 87,162,281 B -> ours 91,764,865 B  (+5.3 %)
```
- **The byte-exact proof lives at the decompressed-image level**: `serialize(parse_image(img)) ==
  img` for every retail entry, with zero per-node raw fallbacks — the §9.1 grammar is *closed*
  (every byte of every entry is explained by structured fields).
- The container re-encode (`u32 decompSize` + one raw LZO1X stream from
  `lionhead_lz_compress.compress`) is proven by `decompress(compose(img)) == img` on all 3,435.

dialogue.big — 20,505 LIPSYNC entries (20,214 non-empty):
```
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --roundtrip
  20214/20214 byte-exact (0 diffs, 0 decode errors, 291 empty skipped)
  2,319,908 frames, 6,301,400 weight keys, Info-duration coherent 20214/20214
```

### 10.2 Modified-content proofs (edit + brand-new entry, via `big_write.rebuild`)

```
python tools/anim_build.py    work/anim_write/graphics_copy.big --mod graphics_mod.big
python tools/anim_build.py    --verify-mod graphics_mod.big --orig graphics_copy.big
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --mod dialogue_mod.big
python tools/lipsync_build.py --verify-mod dialogue_mod.big --orig dialogue_copy.big
```
All four PASS:
- **Anim edit** `ANIM_HERO_THROW`: amplitude-scale (positionFactor ×1.25 on all 70 tracks;
  quats/fps byte-identical). Re-parses clean (0 pal_oob / tail / bad quats).
- **Anim add** `ANIM_HERO_THROW_SLOW` (new TOC entry, type 6): fps ×0.5 on every track (20→10),
  ANRT duration recomputed 1.15→2.30 s, fresh 24-B Info blob (`anim_info_blob`:
  duration/nonLooping/mvec/rotation) — Info dur == ANRT dur.
- **Lipsync edit** `Dialogue_1`: all weights ×0.5 — 166/166 weights verified.
- **Lipsync add** `LIPSYNC_FORGE_TEST_OH`: synthetic 2.0 s sinusoidal "OH" curve, 86 frames
  @43 fps, donor viseme dictionary + deps (`SPEAKER_FEMALE1`), Info = f32 2.0.
- **Non-tampering**: every untouched entry byte-identical (3434/3434 anim, 20504/20504 lipsync).

### 10.3 Custom animation authoring — end-to-end recipe

1. **Keyframes → tracks.** Per bone: quat pool (x,y,z,w) + optional u8/u16 palette (keyframe
   reduction: frame i → pool[pal[i]]), position pool via `Track.set_pos_keys(points)`
   (auto-quantizes to i16 + positionFactor), `fps`, `frame_count`. Bone names must match the target
   rig's Biped names; `parent_index` chains tracks in doc order; retail `bone_index` is the
   constant 31450 rig id (EgoCore normalizes to it). Easiest path: `parse_image()` a donor clip for
   the same rig (guaranteed-correct skeleton/track scaffold) and replace pools/palettes.
2. **Tree → image.** `new_image()` (or donor tree) + ANRT (isCyclic, duration =
   max(frameCount/fps)) ⊃ XALO + optional HLPR(MVEC/TMEV/helper-XSEQ) + AOBJ(rigName) ⊃ [AMSK] +
   one XSEQ per bone — `Anim3DAF.serialize()`.
3. **Image → payload.** `compose_payload(img)` = `u32 len` + LZO1X stream (or the raw image itself
   — the `0x3E3E3E3E` uncompressed form is loader-accepted per EgoCore).
4. **Payload → .big.** `big_write.rebuild(buf, edits={(bank,name): payload})` for replacements,
   `adds=[dict(sub,name,payload,type=6,info=anim_info_blob(dur),...)]` for new clips. Write to a
   copy; drop into the game dir to test.
5. **Speech mouth-sync.** Author a `LipSync` (viseme dictionary → the target creature's
   `ANIM_*_PHONEME_*` pose set, weights per 43 fps frame; `synth_curve()` for a placeholder),
   `encode()` + `info_blob()`, add/edit in dialogue.big with speaker deps. Pair with a `.lut` voice
   line ([AUDIO.md](AUDIO.md)) sharing the entry id/name scheme.

### 10.4 Reproduce

```
mkdir work\anim_write & copy retail bigs in (copies only!)
python tools/anim_build.py    work/anim_write/graphics_copy.big --roundtrip
python tools/anim_build.py    work/anim_write/graphics_copy.big --mod work/anim_write/graphics_mod.big
python tools/anim_build.py    --verify-mod work/anim_write/graphics_mod.big --orig work/anim_write/graphics_copy.big
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --roundtrip
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --mod work/anim_write/dialogue_mod.big
python tools/lipsync_build.py --verify-mod work/anim_write/dialogue_mod.big --orig work/anim_write/dialogue_copy.big
```

---

## 11. Blender bridge (2026-07-20) — actions ⇄ ANIM tracks

`tools/blender_addon/io_scene_fable/anim_ops.py` automates the §10.3 donor-clone recipe from
Blender armature actions (operators *File > Import/Export > Fable TLC Animation*, plus
headless-callable `build_action_from_anim` / `export_action_to_image`). Full detail in
`docs/modding/README.md#blender_addon` §1.5. Coordinate convention: §5.

- **Round-trip proof (headless Blender 5.0, `tests/blender_anim_test.py`):** retail anim → Blender
  action on the imported rig → donor-clone export → per-frame numeric compare vs retail: goblin
  ATTACK_01 (61 data tracks, 1891 frames): max quat error `1-|dot|` = **1.85e-07**, max pos error =
  **2.04e-04** model units; sparrow FLY: **1.59e-07** / **9.66e-05**. Recomposed payloads re-parse
  clean (0 pal_oob / tail / bad quats) and inject+reload via `big_write.rebuild`.
- Export rebuilds pools by keyframe reduction (dedup at f32 storage precision → pool + frame
  palette, dense when no win), re-quantizes positions with a fresh auto `positionFactor`
  (`Track.set_pos_keys`), and sign-aligns consecutive quats (retail does too — only 3
  adjacent-frame sign flips in the whole goblin clip). Donor tracks with EMPTY pools (`Scene Root`,
  `movement_dummy`), tracks for bones absent from the armature, helper tracks and MVEC/TMEV/AMSK are
  kept verbatim — donor-clone limits, documented in BLENDER_ADDON.md.

Reproduce:
```
"C:\Programs\Blender\blender.exe" --background --factory-startup --python \
    tools/blender_addon/tests/blender_anim_test.py -- \
    work/anim_write/graphics_copy.big <out_dir>
```

---

## 12. Open questions

- **In-game visual soak untested**: files verify parser-clean and structurally identical to retail,
  but nobody has watched the ×1.25-amplitude throw in the running game yet (same residual as the
  texture writer had before its live test).
- **In-place TOC Info edit**: `big_write.rebuild` copies TOC records verbatim (only size/offset
  patched), so *retiming an existing entry in place* can't update its 24-B Info duration —
  workaround is add-as-new or delete+add. **Capability request for big_write** (documented, not
  implemented): optional `info_edits={(sub,name): blob}` patching the record's info field. (The
  dialogue pipeline patches the LIPSYNC Info f32 in place for same-size blobs — [AUDIO.md](AUDIO.md).)
- `Track.pre_fps_flag` / `post_frame_flags` engine tolerance unprobed (§9.2).
- `SEQ0` never seen in retail (§9.2).
- Type 7 (DELTA) subset semantics beyond EgoCore's label — to verify.
- Mario-rig retarget (animation translation retargeting or smooth/overlapping joint geometry) —
  resume from the Mario section at the end of `docs/HANDOFF.md`.

---

## 13. Superseded

*Claims from the 2026-07-19 draft of `BIG_ANIM_FORMAT.md` (§0–§8), refuted 2026-07-20 when the
LZO1X container was recognised. Kept verbatim for the record; the measured numbers are real
properties of the COMPRESSED bytes (LZO literal-run bytes of the constant header text), not of the
format.*

- **"3DAF header is plain and self-describing (FourCC chunks) with CBYTE varint framing"** — the
  chunk-length framing after each FourCC was read as a **control-byte varint (CBYTE)**, "the same
  scheme the mesh chunk readers use (`WriteCBYTE`/`ReadCBYTE`)". Actual framing is
  `[fourcc][u32 size]` in the decompressed image (§9.1).
- **`version == 0x00403e12` on 3,435/3,435; `3DAF` precedes `ANRT` on 3,435/3,435** — the "u32
  version" at payload offset 4 is the first LZO stream bytes; the claimed layout was:
  ```
  u32   decompSize
  u32   version = 0x00403e12          # constant across all 3,435 entries
  u16   ??                            # top of the first chunk's CBYTE framing (varies)
  '3DAF'  <CBYTE framing>  "Copyright Big Blue Box Studios Ltd." '\0'
  'ANRT'  <CBYTE framing>  f32 duration
  'AOBJ'  <CBYTE framing>  <rig-name string>
  'XSEQ'  <CBYTE framing>  <magic 0x000d7ada> ...     # PACKED
  'HLPR' ...   'MVEC' ... f32[3]   ['TMEV' ...] (624)  ['AMSK' ...] (34)  ['XALO' ...] (914)
  ```
- **`XSEQ` format magic `0x000d7ada` present on 3,101/3,435** — compressed-stream artifact.
- **ANRT duration slot at `ANRT + 9`** (FourCC(4) + a 5-byte CBYTE frame): scanning the ANRT window
  for an f32 equal to `Info[0]` hit **3,141 at +9**, 5 at +10, 139 no match in-window. The count
  facts (3,141/3,285, 139 differing) stand (§4); the "+9 CBYTE" explanation does not.
- **"Exactly one `XSEQ` chunk per entry (3,419 of 3,435 have exactly 1) — all bones share a single
  packed sequence blob"** — backwards: one XSEQ **per bone track** (§9.1). The 3,419/3,435
  entry-presence count stands.
- **"XSEQ is a bespoke packed serialization, not LZO1X"** and **"LZO1X is the *texture* codec, not
  the anim codec"** — wrong: the *whole payload* after offset 4 is one raw LZO1X stream. The
  inherited attempt failed because it (a) started at the `AOBJ` tag instead of offset 4 and (b)
  used the chunked-frame walker (`lzo1x: bad match distance`).
- **Two byte-only decode attempts that failed** (loop-prevention limit): (1) LZO1X decompress of the
  `AOBJ..end` body; (2) flat/marker scan for `u16 keyCount` + `nKeys×(4×f32 quat)` + a per-frame
  index table keyed on a `30 fps` anchor — yielded **0 unit-quaternions / 0 valid tracks**.
- **"fps = 30, frameCount = round(dur·30)"** (pre-2026-07-19 `parse_anim.py`) — refuted (no
  `30.0` float in the bank); fps is per-track, commonly 20 (§4).
- **"Recovering the tracks needs the engine deserializer — decompile
  `C3DAnimFileXSequenceChunk::ReadFromFile` / `LoadBinary`, cross-ref `C3DAnimFileXLoader`,
  `C3DAnimation2::LoadBinary`, `C3DAnimationInterpolator`, `C3DAnimFileXAllocationSizeChunk`;
  string anchors `.?AVC3DAnimFileX*Chunk@@`, literal `#<XSEQ`, constant `0x000d7ada`"** — the
  scoped Ghidra hand-off was never needed; EgoCore closed it.
- **Original §7 verdict "Author / edit keyframes: NO-GO until `C3DAnimFileXSequenceChunk` is
  reversed"** — superseded by §7 above (GO, shipped).
- **"Type 9 = single-frame phoneme/viseme POSE anims (frameCount=1, duration 0)"** as the
  *discriminator* — refined 2026-07-31: Type 9 = AMSK-masked partial anim generally (§6).
- **Wiki-derived "TMEV = timed animation events [hypothesis]"** — confirmed by the decoded grammar
  (`cstr eventName ; f32 time`).
- **ANIM_WRITER §5 "Blender bridge: exporter from Blender actions → `Track` pools" open item** —
  SHIPPED 2026-07-20 (§11).

---

## 14. Dated findings

- **2026-07-19** — 3DAF chunk header, ANRT duration, AOBJ target rig, Biped bone-name set and the
  Type 6/7/9 = 3,435 `ANIM_*` classification established from retail bytes + RTTI; XSEQ body
  identified as packed (decompSize/paysize 1.02–5.09×); prior LZO-from-AOBJ and 30-fps models refuted.
- **2026-07-20** — XSEQ fully decoded via EgoCore `AnimParser.h` → `parse_anim_xseq.py`: payload =
  `u32 decompSize` + one raw LZO1X stream, `[fourcc][u32 size]` chunks, one XSEQ per bone track;
  3,435/3,435 parse, 4,742,575 quats, 0 tail bytes.
- **2026-07-20** — `anim_build.py` / `lipsync_build.py` shipped: image recompose byte-exact
  3435/3435, LIPSYNC 20214/20214; edit + add proofs (HERO_THROW ×1.25, HERO_THROW_SLOW, Dialogue_1
  ×0.5, LIPSYNC_FORGE_TEST_OH).
- **2026-07-20** — Blender bridge `anim_ops.py` shipped; track = full parent-relative local
  transform, quat stored as conjugate; goblin/sparrow numeric round-trip 1.85e-07 / 2.04e-04.
- **2026-07-20** — Lipsync FPS field = 43 = int(22050/512) (EgoCore `LipSyncParser.h`); Type 9 =
  viseme pose anims (DEMON_DOOR_FACE).
- **2026-07-22** — Mario rig: hero-sized parent-relative translations stretch a smaller rig apart;
  retargeting of translations required (HANDOFF Mario section).
- **2026-07-31** — EgoCore reconcile: Type 9 discriminator = AMSK mask; Type 7 = DELTA anim;
  BoneIndex sentinel 31450; `GltfAnimImporter.h` = retarget reference.
- **2026-09-07** — `BIG_ANIM_FORMAT.md` + `ANIM_WRITER.md` merged into this file.
