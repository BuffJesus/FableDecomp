# ANIM (3DAF) + LIPSYNC write path — custom animations & speech mouth-sync

*2026-07-20. Status: **SHIPPED & VALIDATED.** Write side of `docs/BIG_ANIM_FORMAT.md` §9
(read side) and `docs/DEMON_DOOR_FACE.md` (lipsync chain). Answer key: EgoCore
`AnimCompiler.h` / `LipSyncCompiler.h` (ported semantics; our writers are lossless
field-level models instead of EgoCore's re-deriving compiler). All validation ran
against COPIES of the retail .bigs in `work/anim_write/` — originals untouched.*

Tools: **`tools/anim_build.py`** (3DAF composer) · **`tools/lipsync_build.py`**
(LIPSYNC composer) · consumes `tools/big_write.py` (`rebuild(edits=, adds=)`, unmodified),
`tools/lionhead_lz_compress.py`, `tools/parse_anim_xseq.py`, `tools/parse_bigb.py`.

---

## 1. Round-trip proof (retail copies)

### graphics.big — 3,435 ANIM entries (types 6/7/9)
```
python tools/anim_build.py work/anim_write/graphics_copy.big --roundtrip
  image recompose byte-exact : 3435/3435  (0 diffs, 0 raw-fallback nodes, 213,728 tracks)
  recompress->decompress==img: 3435/3435
  compressed bytes == retail : 0/3435    (expected: different LZO match choices)
  compressed size: retail 87,162,281 B -> ours 91,764,865 B  (+5.3 %)
```
- **The byte-exact proof lives at the decompressed-image level**: `serialize(parse_image(img)) == img`
  for every retail entry, with zero per-node raw fallbacks — the §9.2 grammar is *closed*
  (every byte of every entry is explained by structured fields).
- The container re-encode (`u32 decompSize` + one raw LZO1X stream from
  `lionhead_lz_compress.compress`) is proven by `decompress(compose(img)) == img` on all
  3,435. Compressed bytes never match retail (valid ≠ identical LZO stream; same situation
  as the texture writer) — the engine only requires a valid stream.

### dialogue.big — 20,505 LIPSYNC entries (20,214 non-empty)
```
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --roundtrip
  20214/20214 byte-exact (0 diffs, 0 decode errors, 291 empty skipped)
  2,319,908 frames, 6,301,400 weight keys, Info-duration coherent 20214/20214
```
LIPSYNC payloads are uncompressed; `encode(decode(x)) == x` on every non-empty entry.

## 2. Modified-content proofs (edit + brand-new entry, via `big_write.rebuild`)

```
python tools/anim_build.py    work/anim_write/graphics_copy.big --mod graphics_mod.big
python tools/anim_build.py    --verify-mod graphics_mod.big --orig graphics_copy.big
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --mod dialogue_mod.big
python tools/lipsync_build.py --verify-mod dialogue_mod.big --orig dialogue_copy.big
```
All four PASS:
- **Anim edit** `ANIM_HERO_THROW`: amplitude-scale (positionFactor ×1.25 on all 70 tracks;
  quats/fps byte-identical). Re-parses clean (0 pal_oob / tail / bad quats).
- **Anim add** `ANIM_HERO_THROW_SLOW` (new TOC entry, type 6): fps ×0.5 on every track
  (20→10), ANRT duration recomputed 1.15→2.30 s, fresh 24-B Info blob
  (`anim_info_blob`: duration/nonLooping/mvec/rotation) — Info dur == ANRT dur.
- **Lipsync edit** `Dialogue_1`: all weights ×0.5 — 166/166 weights verified.
- **Lipsync add** `LIPSYNC_FORGE_TEST_OH`: synthetic 2.0 s sinusoidal "OH" curve,
  86 frames @43 fps, donor viseme dictionary + deps (`SPEAKER_FEMALE1`), Info = f32 2.0.
- **Non-tampering**: every untouched entry byte-identical (3434/3434 anim, 20504/20504 lipsync).

## 3. Format facts the writer nails down (delta over BIG_ANIM_FORMAT.md §9)

- XSEQ sections are **optional from the end**: a track may legally stop after any of the
  four `u16`-counted sections (rotPool / rotPal / posPool / posPal). `Track.has_*` flags
  reproduce truncated retail tracks; encode emits sections only up to the last present.
- Palette index width is decided by the *pool* count (`u8` if count ≤ 255 else `u16`) —
  same rule both directions.
- ANRT/AOBJ carry **freeform header bytes** between their fixed head and the first nested
  chunk (found by fourcc stop-scan: ANRT→{HLPR,AOBJ,XALO}, AOBJ→{XSEQ,SEQ0,AMSK});
  preserved verbatim (`Node.head`). MVEC is a superchunk (12 B vec + nested chunks).
  Trailing non-chunk bytes at any level are kept as raw tails (`Node.tail`, image trailer —
  retail images end with an 8-byte `04 00 00 00 00 00 00 00` footer that parses as tail).
- f32 cells (fps, positionFactor, scalingFactor, duration, quat pool) are stored as raw
  4-byte cells in the model so any NaN payload/denormal round-trips bit-exact; position
  keys are `i16×3` ints (exact). TOC `Info` = `C3DAnimationInfo::Serialize`, 24 B:
  `f32 duration, f32 nonLoopingDuration, f32[3] movementVector, f32 rotation`.

## 4. Custom animation authoring — what it takes end-to-end now

1. **Keyframes → tracks.** Per bone: quat pool (x,y,z,w) + optional u8/u16 palette
  (keyframe reduction: frame i → pool[pal[i]]), position pool via
  `Track.set_pos_keys(points)` (auto-quantizes to i16 + positionFactor), `fps`,
  `frame_count`. Bone names must match the target rig's Biped names; `parent_index`
  chains tracks in doc order; retail `bone_index` is the constant 31450 rig id
  (EgoCore normalizes to it). Easiest path: `parse_image()` a donor clip for the same
  rig (guaranteed-correct skeleton/track scaffold) and replace pools/palettes.
2. **Tree → image.** `new_image()` (or donor tree) + ANRT (isCyclic, duration =
  max(frameCount/fps)) ⊃ XALO + optional HLPR(MVEC/TMEV/helper-XSEQ) + AOBJ(rigName)
  ⊃ [AMSK] + one XSEQ per bone — `Anim3DAF.serialize()`. (EgoCore `AnimCompiler.h`
  documents the canonical chunk nesting; our verifier accepts any nesting the engine
  parser accepts.)
3. **Image → payload.** `compose_payload(img)` = `u32 len` + LZO1X stream (or the raw
  image itself — the `0x3E3E3E3E` uncompressed form is loader-accepted per EgoCore).
4. **Payload → .big.** `big_write.rebuild(buf, edits={(bank,name): payload})` for
  replacements, `adds=[dict(sub,name,payload,type=6,info=anim_info_blob(dur),...)]`
  for new clips. Write to a copy; drop into the game dir to test.
5. **Speech mouth-sync.** Author a `LipSync` (viseme dictionary → the target creature's
  `ANIM_*_PHONEME_*` pose set, weights per 43 fps frame; `synth_curve()` for a
  placeholder), `encode()` + `info_blob()`, add/edit in dialogue.big with speaker deps.
  Pair with a `.lut` voice line (docs/LUT_WRITER.md) sharing the entry id/name scheme.

## 5. Open questions

- **In-game visual soak untested**: files verify parser-clean and structurally identical
  to retail, but nobody has watched the ×1.25-amplitude throw in the running game yet
  (same residual as the texture writer had before its live test).
- **In-place TOC Info edit**: `big_write.rebuild` copies TOC records verbatim (only
  size/offset patched), so *retiming an existing entry in place* can't update its 24-B
  Info duration — workaround is add-as-new (done here) or delete+add. **Capability
  request for big_write** (documented, not implemented, per text.big-agent lock):
  optional `info_edits={(sub,name): blob}` patching the record's info field.
- `Track.pre_fps_flag` and `post_frame_flags` semantics: EgoCore writes
  `flags[0..2] = {any,rotMode,posMode}` (0/1/2 = none/constant/animated), retail data
  matches loosely; we preserve them verbatim on edit and copy donor values on authoring.
  Engine tolerance to "wrong" flags unprobed.
- `SEQ0` (vs `XSEQ`) never appears in retail graphics.big tracks touched here; parser
  accepts both, writer emits whatever the node carries.
- ~~Blender bridge: exporter from Blender actions → `Track` pools~~ **SHIPPED 2026-07-20**
  — see §7 and `docs/BLENDER_ADDON.md` §1.5 (`io_scene_fable/anim_ops.py`, both
  directions, numeric round-trip proven).

## 6. Reproduce

```
mkdir work\anim_write & copy retail bigs in (copies only!)
python tools/anim_build.py    work/anim_write/graphics_copy.big --roundtrip
python tools/anim_build.py    work/anim_write/graphics_copy.big --mod work/anim_write/graphics_mod.big
python tools/anim_build.py    --verify-mod work/anim_write/graphics_mod.big --orig work/anim_write/graphics_copy.big
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --roundtrip
python tools/lipsync_build.py work/anim_write/dialogue_copy.big --mod work/anim_write/dialogue_mod.big
python tools/lipsync_build.py --verify-mod work/anim_write/dialogue_mod.big --orig work/anim_write/dialogue_copy.big
```

## 7. Blender bridge (2026-07-20) — actions ⇄ ANIM tracks, and the coordinate convention

`tools/blender_addon/io_scene_fable/anim_ops.py` automates the §4 donor-clone
recipe from Blender armature actions (operators *File > Import/Export > Fable
TLC Animation*, plus headless-callable `build_action_from_anim` /
`export_action_to_image`). Full detail in `docs/BLENDER_ADDON.md` §1.5; the
format facts pinned down while building it:

- **Track semantics (probed on retail):** an XSEQ track stores the bone's FULL
  parent-relative local transform per frame, in the SAME model space the mesh
  bind poses use (no axis conversion vs the Blender importer): pos key `p` =
  local translation (bind units); quat key `q(x,y,z,w)` = **conjugate** of the
  mathutils-convention local rotation (`q_local = Quaternion((w,-x,-y,-z))`).
  Proof: for every stationary bone of `ANIM_CREATURE_GOBLIN_ATTACK_01` the anim
  key equals the `MESH_CREATURE_GOBLIN` bind-local transform exactly (up to i16
  quantization / quaternion sign).
- **Round-trip proof (headless Blender 5.0,
  `tests/blender_anim_test.py`):** retail anim → Blender action on the imported
  rig → donor-clone export → per-frame numeric compare vs retail:
  goblin ATTACK_01 (61 data tracks, 1891 frames): max quat error `1-|dot|` =
  **1.85e-07**, max pos error = **2.04e-04** model units; sparrow FLY:
  **1.59e-07** / **9.66e-05**. Recomposed payloads re-parse clean
  (0 pal_oob / tail / bad quats) and inject+reload via `big_write.rebuild`.
- Export rebuilds pools by keyframe reduction (dedup at f32 storage precision →
  pool + frame palette, dense when no win), re-quantizes positions with a fresh
  auto `positionFactor` (`Track.set_pos_keys`), and sign-aligns consecutive
  quats (retail does too — only 3 adjacent-frame sign flips in the whole goblin
  clip). Donor tracks with EMPTY pools (`Scene Root`, `movement_dummy`), tracks
  for bones absent from the armature, helper tracks and MVEC/TMEV/AMSK are kept
  verbatim — donor-clone limits, documented in BLENDER_ADDON.md.

Reproduce:
```
"C:\Programs\Blender\blender.exe" --background --factory-startup --python \
    tools/blender_addon/tests/blender_anim_test.py -- \
    work/anim_write/graphics_copy.big <out_dir>
```
