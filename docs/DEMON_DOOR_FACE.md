# Demon door talking face — full asset/animation chain

*2026-07-20 (animation-RE agent). Status: keyframe decode + phoneme mechanism VALIDATED
from retail bytes (`tools/parse_anim_xseq.py`, byte-exact on all 3,435 anims) cross-checked
against the EgoCore reference source
(`C:\Users\Cornelio\Documents\EgoCoreInspect\EgoCore-master`). Goal: enable authoring
custom demon doors. Companion to `docs/BIG_ANIM_FORMAT.md` §9 (format spec).*

## 1. Asset inventory (retail `graphics.big`, bank `MBANK_ALLMESHES`)

Meshes (per demon-door variant; `_NW_` = northwest/snowy variant, plus GRAVEYARD wall):

| entry | type | role |
|---|---|---|
| `MESH_DEMON_DOOR_01` | 5 | door leaf (skinned; rig `Demon_Door`: `Scene Root`, `L_Gate_*`, `door`, `cobwebs`) |
| `MESH_DEMON_DOOR_FACE_01` | 5 | **the talking face** — `skel_flag=1`, 2 materials (texIds 736, 3396) |
| `MESH_DEMON_DOOR_EYE_01` | 1 | eye (materials `demoneye`, `glow`) |
| `MESH_DEMON_DOOR_WALL_01`(+`[PHYSICS]`) | 1/3 | static wall arch |
| `CMESH_DEMON_DOOR_CLOSED_01` / `_OPEN_01` | 3 | collision states |

`MESH_DEMON_DOOR_FACE_01` embeds the face skeleton (cleartext in its payload, partial due to
LZO backrefs): `Scene Root, Main_[Root_Bone], Base[_Bone], head, L_Cheek_st, Top_lip[_st],
[eye_]deck, [L/R_]brow_1, [eye_bot]tom/[eye_]top` + helpers `TARGET_CURSOR_POINT`,
`focus_eye_01`, `COLD_BREATH`. This is exactly the 16-bone **`Demon` rig** every face anim
targets (AOBJ name), decoded track-by-track below.

## 2. The `Demon` face rig (16 tracks, from any DEMON_DOOR face anim)

```
 0 Scene Root(-1)  1 Main_Root_Bone(0)  2 Base_Bone(1)  3 head(2)
 4 eye_deck(3)     5 L_brow_1(4)   6 R_brow_1(4)   7 L_c_bone_1(4)  8 R_c_bone_1(4)
 9 R_Cheek_st(3)  10 L_Cheek_st(3) 11 Chin(3)      12 Chin_tip(11)  13 Top_lip_st(3)
14 eye_bottom(3)  15 eye_top(3)                       (parentIndex in parens)
```
Mouth = `Chin`/`Chin_tip` (jaw) + `Top_lip_st` + `L/R_Cheek_st`; eyes = `eye_top`/`eye_bottom`
(lids) on `eye_deck`; brows = `L/R_brow_1` + `L/R_c_bone_1`. Same face-bone naming convention
as the biped heads (HeroUnclothed contains the identical set under `Bip01 Head`).

## 3. Animation set (all decode byte-exact via `parse_anim_xseq.py`)

| entry | rig | dur (s) | cyclic | tracks | notes |
|---|---|---:|:-:|--:|---|
| `ANIM_DEMON_DOOR_OPEN_01` / `CLOSE_01` | `Demon_Door` | 2.333 | y | 5 | door leaf swing |
| `ANIM_DEMON_DOOR_DEFAULT_01` | `Demon_Door` | 0 | n | 5 | door rest pose |
| `ANIM_DEMON_DOOR_DEFAULT` | `Demon` | 0 | n | 16 | face rest pose |
| `ANIM_DEMON_DOOR_CONVERSATION_BASE_01` | `Demon` | 5.467 | y | 16 | talk idle: head sway, brow/lid motion (30 fps, 164 frames) |
| `ANIM_DEMON_DOOR_CONVERSATION_BREAKOFF_01..04` | `Demon` | 0.9–1.5 | y | 16 | conversation punctuation gestures |
| `ANIM_DEMON_DOOR_PHONEME_{AI,EE,MM,OH,ST,WW}` | `Demon` | 0 | n | 16 | **viseme poses**: Type 9, 1 frame, `AMSK` face mask |

Key structural facts (evidence: decoded tracks):
- **Phoneme entries are single-frame POSES** (`frameCount=1`, duration 0) with an `AMSK`
  bone-mask chunk (20 bytes; identical mask on all six) restricting them to the face bones.
  In `PHONEME_OH` the animated bones are exactly the mouth/brow/lid set (§2), 13 unit
  quaternions + 15 positions, one key each.
- **TOC Type 9 = phoneme-pose family engine-wide**: every speaking creature has the same
  set — `ANIM_BIPED_GENERIC_MAN_PHONEME_{EE,MM,OH,SZ}`, `ANIM_VILLAGER_MALE_PHONEME_{AH,EE,
  MM,OH,SZ,WW}`, `ANIM_BANDIT_PHONEME_*`, `ANIM_ORACLE_HEAD_PHONEME_{AH,EE,MM,OH,ST,WW}`,
  demon door `{AI,EE,MM,OH,ST,WW}` (AI≈AH, ST≈SZ naming drift on the non-biped heads).
- The conversation base is a cyclic full-face idle; phoneme poses are blended ON TOP of it
  (mask-limited), which is why its own mouth tracks (`Chin`, `Top_lip_st`) are 1-key constant.

## 4. Lipsync data (dialogue.big) — drives the mouth

`data\lang\English\dialogue.big` banks: `LIPSYNC_ENGLISH_MAIN` (12,134), `_MAIN_2` (1),
`_SCRIPT` (5,310), `_SCRIPT_2` (3,060). Grammar (byte-exact, `parse_bigb.decode_lipsync`,
confirmed identical to EgoCore `LipSyncParser.h`):

```
u32 visemeCount ; visemeCount x { u8 id, cstr mnemonic }      # e.g. 1=AH 2=EE 3=OH 4=SZ
u32 fps          # always 43 = int(22050/512)  — one frame per 512 samples of 22.05 kHz audio
                 # (was "blockMarker 0x2B" in parse_bigb; EgoCore proves it is the FPS field)
u32 frameCount   # duration = frameCount / 43.066
frameCount x { u8 keyCount ; keyCount x { u8 visemeId, u8 weight } }   # weight/255 = blend
TOC Info = f32 duration ; deps[1] = speaker (e.g. SPEAKER_FEMALE1)
```
Sampled retail frames look like `[(OH,255),(EE,44)] ...` — simultaneous weighted visemes,
i.e. **per-frame viseme weight curves**, not phoneme events. EgoCore's canonical set:
`FABLE_PHONEMES = { AH, EE, MM, OH, SZ }` (LipSyncProperties.h:285).

## 5. The mechanism (how a demon door talks)

1. The placed door is `OBJECT_DEMON_DOOR_01`; the face is a separate
   **`CREATURE_DEMON_DOOR_FACE_01`** def (game.bin #3840, 1060 B; NW variant #3844) — the
   face is a *creature*, so it inherits the standard creature dialogue/lipsync machinery.
   Wall/door leaf are plain OBJECT defs (#3841–3845). (Evidence: `forge defs list/show`.)
2. In conversation the engine plays `ANIM_DEMON_DOOR_CONVERSATION_BASE_01` (cyclic talk
   idle) with `BREAKOFF_0x` gestures interleaved.
3. While a dialogue line's audio plays, its LIPSYNC entry is stepped at 43.066 fps; each
   frame's `{visemeId → weight}` set blends the matching single-frame
   `ANIM_DEMON_DOOR_PHONEME_<mnemonic>` pose into the face bones through the `AMSK` mask
   (weight/255), on top of the base anim. Blending/interp is the standard runtime path
   (`C3DAnimationBlendState` / `C3DAnimationInterpolator` RTTI classes).
4. Mouth shape = weighted sum of viseme poses; eyes/brows come from the base + breakoffs.

Confidence: steps 2–4 are the only mechanism consistent with (a) phoneme anims being
mask-limited 1-frame poses, (b) lipsync being weight curves keyed by the same mnemonics,
(c) the engine-wide Type-9 pattern. The exact blend math (additive vs interpolative) and
the def field that binds `CREATURE_DEMON_DOOR_FACE_01` → the `ANIM_DEMON_DOOR_*` set (by
name prefix or anim-set id inside the 1060-byte CREATURE blob) are the remaining unknowns
(see §7).

## 6. Authoring a custom demon door — what it takes

- **Meshes**: clone `MESH_DEMON_DOOR_FACE_01` etc. (geometry pipeline per
  `BIG_MESH_GEOMETRY.md` / blender addon). Keep the 16-bone `Demon` skeleton names.
- **Anims**: fully writable now — emit 3DAF per `BIG_ANIM_FORMAT.md` §9.2 (EgoCore
  `AnimCompiler.h` is a working writer reference: `>>>>`+`3DAF`+u32 100+copyright, chunked;
  optionally LZO1X-compress with u32 decompSize prefix). Minimum custom-face set: DEFAULT,
  CONVERSATION_BASE, ≥1 BREAKOFF, and the 6 PHONEME poses (copy AMSK from retail).
- **Voice**: new dialogue.big LIPSYNC entries are trivially writable
  (`parse_bigb.decode_lipsync` grammar + EgoCore `LipSyncCompiler.h`/`GenerateEmpty`);
  generate weights from audio at 43.066 fps.
- **Defs**: clone `OBJECT_DEMON_DOOR_01` + `CREATURE_DEMON_DOOR_FACE_01` (+WALL) in
  game.bin, then place via TNG. CREATURE def field layout is the open item (§7).

## 7. Open items (genuinely need Ghidra or def-schema work — NOT anim format)

1. **CREATURE def layout** — `ghidra_out/def_schema.json` decodes neither CREATURE nor
   OBJECT; needed to see how the face creature names its anim set / phoneme prefix and to
   author #3840 clones confidently. (Def-schema extension task, likely Ghidra CDefinitionManager.)
2. **Blend math** of viseme poses (additive vs weighted-average, normalization) —
   `C3DAnimationBlendState` decompile; only matters for pixel-perfect prediction, not authoring.
3. How OBJECT_DEMON_DOOR_01 ↔ CREATURE_DEMON_DOOR_FACE_01 are bound at placement
   (TNG thing linkage vs def reference) — check a village TNG containing a demon door.

## 8. Reproduce

```
python tools/parse_anim_xseq.py <graphics.big> --list DEMON
python tools/parse_anim_xseq.py <graphics.big> --entry ANIM_DEMON_DOOR_PHONEME_OH
python tools/parse_anim_xseq.py <graphics.big> --entry ANIM_DEMON_DOOR_CONVERSATION_BASE_01 --json dd_base.json
python tools/parse_mesh.py <graphics.big> --entry MESH_DEMON_DOOR_FACE_01
D:\Code\FableForge\build\forge.exe defs list "<install>" "" DEMON   # def inventory
```
