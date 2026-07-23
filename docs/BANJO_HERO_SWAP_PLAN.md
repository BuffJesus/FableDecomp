# Banjo → hero mesh swap — execution plan (prep; run at fold time)

Goal: replace the playable hero with Banjo to test skinned-animation playback. Banjo
plays every hero animation because he's bound to the hero's own Biped skeleton.

## Assets
- **Source:** `banjo.obj` — 262 verts, 805 faces, 6 materials (`Shape.280/293/298/301/303`
  + `material_0`), **Y-up**, bbox X −6.11..5.87, Y 0..7.13 (~7 tall), Z −3.13..3.36.
  Texture `banjo_color.png` (166 KB) is the main skin; the `Shape.*` mats are tiny
  palette swatches (`*_c.png`, ~200–750 B). Extracted to scratchpad `banjo/`.
- **Target:** `MESH_HERO` = **graphics.big id 4299, type 5 (skinned)**, bound to the hero
  Biped rig. Overwrite in place (proven path; append merge still pending).

## Tooling
- `tools/blender_addon/io_scene_fable/mesh_rw.py` — byte-exact mesh r/w. Bone blocks:
  `[u16×bone_count indices][60×n bones][48×n keyframes][64×n transforms]` +
  `bone_name_size`. `encode_positions(...)`, `encode_skin(vb, ..., vert_influences)`
  where `vert_influences` = one `list[(local_bone_slot, weight)]` per vertex.
- `compose_mesh` / `clone_skeleton` (mesh compose pipeline). Texture: `texture_build.py`
  (PNG → DXT). Container write: overwrite entry via the big writer (same-size or resized).

## Steps
1. **Extract the hero rig (do first).** Pull `MESH_HERO` (4299) from
   `data/graphics/graphics.big` (243 MB) via `parse_mesh.py`/`mesh_rw.py`. Record: full
   Biped **bone list** (names, hierarchy, 64-byte inverse-bind matrices), the mesh's
   **coordinate frame + scale** (its bbox), and the material/texture slots. This is the
   skeleton Banjo must bind to so hero anims drive him — clone it with `clone_skeleton`.
2. **Fit Banjo to the hero frame.** Import `banjo.obj`; convert Y-up → the hero mesh's
   up-axis; uniform-scale so Banjo's ~7-unit height matches the hero mesh height from
   step 1; center on the pelvis origin.
3. **Auto-skin Banjo → hero bones.** Banjo has no rig, so weight each of the 262 verts to
   the hero Biped bones by proximity (nearest-bone / envelope): for each vert, pick the
   1–2 closest bones by position and assign normalized weights → `vert_influences`.
   (Rough by body region is enough for an anim test; refine per-region later. A bear on a
   humanoid rig looks goofy — that's expected and fine.)
4. **Texture.** `banjo_color.png` → DXT (`texture_build`). Either overwrite a `HERO_SKIN_*`
   sheet the composed mesh references, or add Banjo's texture and point the composed
   material at it. Keep material count aligned to what entry 4299 expects.
5. **Compose + overwrite.** `compose_mesh(name=MESH_HERO, prims=banjo, materials,
   skeleton=clone_of_hero, type=5, vert_influences)` → overwrite graphics.big **entry
   4299** (byte-safe, re-parse-validated). Stage under `work/banjo_hero/`.
6. **Deploy** graphics.big (add to the smoke deploy) → hero renders as Banjo and plays all
   hero animations = the skinned-anim test.

## Risks / notes
- **Rig mismatch:** Banjo's bear proportions won't map cleanly to a human Biped — limbs
  will deform oddly under human walk/combat cycles. Functional (proves skin + anim), not
  pretty. Manual per-region bone assignment improves it.
- **Scale/orientation** must be read from the extracted hero mesh (step 1), not guessed.
- **Overwrite, not append** — MESH_HERO 4299 overwrite is the proven path; once the append
  merge lands, the same pipeline can *append* Banjo/Mario as new meshes (id 8113+) for the
  enemy path (Mario = `mario.fbx`/`.dae`, already rigged, → a Fable creature skeleton).
- This is the direct dependency-free test of our skinned-mesh authoring end-to-end.
