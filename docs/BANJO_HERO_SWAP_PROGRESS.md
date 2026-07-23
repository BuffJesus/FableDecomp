# Banjo → hero mesh swap (TASK #18) — PROGRESS / RESULT

## v2 (2026-07-21) — SHATTER FIXED + articulated variant + equipment-blank overlay

The v1 swap below deployed but rendered as a **shattered multicolour crystalline blob**
(user screenshot). Root cause: v1 skinned the single 262-vert Banjo prim to each vert's 2
NEAREST bones by raw proximity across the 77-bone Biped — adjacent verts bound to
pelvis/thigh/spine, so triangles tore apart when the rig posed (byte-valid, geometrically
torn). Rebuilt in `work/banjo_hero_v2/` (staged, NOT deployed, NOT committed):

**v2 RIGID** (`build_banjo_v2.py` → `stage/`): every Banjo vert weighted **100 % to ONE bone,
`Bip01 Pelvis` (local slot 4**, body-centre head Z≈103.5). One palette / one block → solid
non-tearing mesh that moves as one with the body transform. Uniform (aspect-preserving)
Y-up→Z-up remap, Banjo +Z(front)→hero −Y(front), uniform scale 21.43, feet at foot-bone Z 14.96
→ head-bone Z 167.74. **CONFIRMED COHERENT IN-GAME** (shatter gone). **Verify (re-decode):**
4299 type 5, 77 bones, 805 faces, Info 108 B; ALL verts weight-sum 1.0, single slot {4}, no NaN.

**Rainbow-texture fix:** Banjo's OBJ has WEDGE (per-face-corner) UVs; the earlier one-UV-per-
position collapse put the WRONG UV on **89.2 %** of corners → rainbow stripes. **Fix: split verts
on unique (position, UV) pairs → 2415 verts**, each corner carrying its exact UV; rigid pelvis
bind preserved. Verify: **2415/2415 corners carry their exact authored vt (0 wrong)**. Texture id
1250 HERO_SKIN_FACE DXT1 512×512 decodes to Banjo (mean 131/85/50).

**Flat-shading fix (smooth normals):** the wedge-split left compose to accumulate normals PER
SPLIT VERT → split verts at each seam disagreed (241/248 shared positions differed >5°) → engine
flat-shaded. **Fix: one smooth normal per ORIGINAL position (area-weighted average of adjacent
face normals) propagated to every split vert sharing that position**, written into the stride-28
normal field. Verify: unit-length, **0 seam discontinuities**, 261 distinct directions (smooth,
matches retail hero shading). OBJ+MTL: `composed_banjo_hero.obj`/`.mtl`.

**v2b ARTICULATED** (`build_banjo_v2b.py` → `stage_articulated/`): binds only to the **12 major
deform bones** (slots 4 Pelvis, 5 Spine, 12 Neck, 57 Head, 14/16/17 L-arm, 36/38/39 R-arm,
8/10/29 L-leg, 9/37/50 R-leg) by nearest bone-SEGMENT distance (head→child-head line) with
inverse-power falloff, then **Laplacian-smoothed over the vertex adjacency graph (8 passes,
λ=0.5)** to kill isolated bone-flips (the anti-tear fix), capped ≤3 influences/vert, sum 1.
**Verify:** all verts weight-sum 1.0, max 3 infl, 16 slots (all in the major set); **anatomy
16/16 upper-outer verts on ARM bones, 50/50 lower verts on LEG bones**. OBJ:
`composed_banjo_hero_articulated.obj`.

**Equipment hide — SHRINK method** (`build_combined_v2.py` → `stage_combined_v2/`): two earlier
methods failed — (1) degenerate 1-triangle replacement FROZE at save-load; (2) collapse-all-verts
-to-ONE-POINT CRASHED in-game (every face zero-area → NaN normals when lit). SHRINK method keeps
each equipment mesh's EXACT structure (vert/face/material/bone counts + Info byte-for-byte) and
**shrinks verts toward the mesh centroid ×0.001**, placed at (0,6,103) inside the body: a uniform
affine map can't make a good face degenerate, so faces stay tiny-but-nonzero → valid normals → no
crash; normals left intact. **Hidden ids (16):** apprentice outfit 4085-4091, young-hero hair
4275-4277, ranged gear 8039/8041 (bow) + 8048 (quiver) + 8042/8043 (arrows) + 8050 (stick).
Start-outfit confirmed via `docs/APPEARANCE_SYSTEM.md` + `appearance_survey.py`. **Verify:** 16/16
counts+Info == retail, 0 new degenerate faces, global min positive area 6.5e-8; MESH_HERO still
2415v/805f/77b. (Several retail equip meshes already carry zero-area faces at retail — engine
renders them fine.)

**Deploys (test Banjo-only first):** `stage_banjo_only/` = fixed-UV rigid Banjo on CLEAN retail,
exactly one entry (4299) changed, no hide — isolates the swap and is the clean textured Banjo;
differs from the known-good 262-vert build ONLY by vert count (→2415, wedge-UV split) + per-corner
UVs. `stage_combined_v2/` = Banjo + shrink-hide. `stage/` (collapse-to-point) CRASHED — negative
control only. Cures: rigid pelvis bind → shatter; wedge-UV vert-split → rainbow stripes; shrink
(not collapse) → equipment-hide crash. See `work/banjo_hero_v2/DEPLOY_NOTE.md`.

---

## v1 (original attempt — SUPERSEDED by v2 above; kept for reference)

**2026-07-21. Result: DELIVERABLE (a) — a working, format-verified swap is STAGED.**
The skinning/bone-binding format was already fully cracked before this task (see
`docs/MESH_COMPOSE.md` §1b + `docs/BIG_MESH_GEOMETRY.md` §7/§7.1); this task USED that proven
pipeline to overwrite `MESH_HERO` (4299) with Banjo geometry rigged to the hero Biped. No new RE
was required. Editor NOT run; native writers only.

Staged output: `work/banjo_hero/` (build script + src assets + DEPLOY_NOTE.md + a full copy of
graphics.big and textures.big with the swap applied). NOT deployed, NOT committed.

## Why this was not a blocker
The plan (`BANJO_HERO_SWAP_PLAN.md`) predates the type-5 skinned-compose landing. That pipeline
is now proven byte-exact:
- `mesh_rw.clone_skeleton(pay, 5)` captures the four bone blocks RAW (indices, names, 60-B
  C3DBone, 48-B TRS, 64-B IBM) — survives byte-exact, no bind-matrix re-derivation.
- `mesh_rw.compose_mesh(..., skeleton=, layout='float')` emits a full type-5 payload:
  AnimatedFlag=1, BoneCount>0 + bone blocks, per-prim `CAnimatedBlock` records
  (BonesPerVertex=3, PalettedFlag=1), 4 joint + 4 weight bytes after position (stride 28).
- `mesh_rw.decode_to_compose_args` + `_prim_skin` decode it back (recompose oracle).
- Container edit: `big_write.rebuild(buf, edits={(sub,name): payload})` overwrites a payload in
  place, patches the TOC size/offset, keeps the Info blob verbatim.

## Hero mesh facts (entry 4299 = MESH_HERO), decoded this session
- type 5 (skinned), payload 129,050 B, Info 108 B.
- **77-bone Biped skeleton** — full hero rig: Scene Root, Movement_dummy, Bip01 + Pelvis / Spine
  ×4 / Neck ×2 / Head, L+R {Thigh, Calf, Foot, Toe0}, L+R {Clavicle, UpperArm, Forearm, Hand,
  Finger0..4 chains}, facial (Chin, lips, cheeks, brows, eye deck/set), and
  WEAPON_FOCUS_01/02 + WEAPON_SCABBARD_01/02.
- 19 primitives / 3378 verts / 5258 faces, all skinned, **float layout (stride 28)**.
- 19 materials (face/eyes/torso_front/back/arms/hands/hips/thighs/calves/feet/mouth). Info blob
  advertises 12 texture ids: `1045,1233,1234,1235,1236,1237,1248,1249,1250,1251,1252,1253`
  (1250 = `HERO_SKIN_FACE` 512×512, 1252 = `HERO_SKIN_TORSO`, etc.).
- **Model frame is Z-up.** From the RIG bind poses: foot bones Z≈15, head bone Z≈168
  (body height ≈153), pelvis Z≈103, hands X≈±48. Mesh bbox X ±63, Y −21..19, Z −0.8..194.5.
  (The full-mesh Z extent 195 exceeds the foot→head 153 because of raised-arm/weapon pose reach;
  Banjo was fit to the RIG span, not the bbox.)
- **Recompose oracle PASSED** on the untouched retail hero mesh: decode → compose(skeleton=) →
  re-decode gives 3378 verts / 5258 faces / 77 bones identical, Info 108 B — pipeline is sound
  before touching Banjo.

## Banjo asset
`D:\Downloads\Nintendo 64 - Banjo-Tooie - Playable Characters - Banjo (Low Poly).zip`
→ `banjo.obj` 262 verts / 805 faces / 6 materials, **Y-up**, ~7 tall (Y span 7.13),
bbox X −6.11..5.87 / Y 0..7.13 / Z −3.13..3.36. Skin `banjo_color.png`; 5 tiny palette
swatches (`*_c.png`). Matches the plan exactly.

## What was built (see work/banjo_hero/build_banjo.py + DEPLOY_NOTE.md)
1. Fit Banjo (Y-up) → hero frame (Z-up): banjo X→hero X, banjo Z→hero Y (depth),
   banjo Y→hero Z (height). Vertical scale to the foot→head bone span (feet at foot bones,
   head at head bone → Z 15..168); horizontal scale capped to hero shoulder width (X ±32).
2. Auto-skin: each vert → 2 nearest hero DEFORM bones (55 real Biped bones; focus/scabbard/eye/
   facial dummies excluded from weighting), inverse-distance normalized weights, then compose's
   `_resolve_skin` (top-3) + `_auto_skin_blocks` (palette ≤16, duplicates shared verts →
   539 emitted verts).
3. `compose_mesh(name=MESH_HERO, [banjo_prim], [{name:banjo, diffuse_id:1250}],
   skeleton=clone_skeleton(hero), layout='float')` → 33 KB type-5 payload.
4. `big_write.rebuild` edit `(MBANK_ALLMESHES, MESH_HERO)` → staged graphics.big.
5. `banjo_color.png` → DXT (fmt 31, 512×512) → `texture_build.replace_entry("HERO_SKIN_FACE")`
   → staged textures.big. (Material points at 1250, already in the Info set → Info blob
   untouched, stays valid.)

## Validation (re-decode of the staged files — NOT a game launch)
- Staged graphics.big re-opens; entry 4299 = MESH_HERO, type 5, **77 bones**, **805 faces**
  preserved, Info 108 B unchanged.
- **All 539 skinned verts: weight-sum = 1.0, zero unweighted, 25 distinct hero Biped bones**
  (Pelvis/Spine1-3/Neck/Clavicles/Thighs/Calves/...). This is the anim-correctness gate — the
  mesh is genuinely bound to the hero rig, not a static T-pose statue.
- Staged textures.big re-opens; HERO_SKIN_FACE re-decodes (size 133,529).

## Honest remaining unknown (the ONE thing static analysis can't settle)
**Engine smoke test not run** (no game launch in this task). The payload is grammar-identical to
retail type-5 skinned meshes and the compose pipeline's type-5 path is separately proven
byte-exact (goblin 67-bone / seagull 27-bone / hero-boot 10-bone recompose + synthetic skinned
meshes, docs/MESH_COMPOSE.md §3/§5), but "MESH_HERO loads and animates as Banjo in Fable.exe"
has not been observed. Deploying `work/banjo_hero/stage/` (game closed, no post-deploy Steam
verify) IS that test. Everything up to the engine's front door is verified against real bytes.

Cosmetic (not blockers): proximity auto-skin deforms a bear on a humanoid rig goofily (expected);
per-vertex UVs collapse Banjo's per-corner vt at shared verts (minor seams); overwrites the hero
face texture sheet (chosen to keep the Info blob untouched — a new-texture append path avoids it).
