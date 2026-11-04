# Modernization journal, 2026-11-03

Resumed by the user ("proceed on modernization/64bit until asked to stop") from the NEXT_SESSION "Session end
2026-10-29" point. 152-183 accepted locally; nothing committed.

- Native peer this session: **fabletlc-b9** (native recreation). It reports these batches as pending since the 183 cut:
  - gate_native_mesh_chunk_delete_data (7);
  - gate_native_3d_group2_vertex_blends (2, new header fable_3d_group2.h);
  - gate_native_counted_pointer_destructors (4, new header fable_soundemitter_lh.h);
  - wf-aq (3 roots into gate_native_mesh_primitive_lists, no header change).

  - Update from fabletlc-b9:
    - wf-aq landed.
    - gate_native_quadric_optimised_face 6 (new fable_quadric_optimiser.h).
    - gate_native_animation_manager_bank 2 (new fable_3d_animation_manager.h).
    - gate_native_nav_connector_delete_data 1 (new fable_nav_connector_node.h).
    - gate_native_bone_info_delete_data 1 (new fable_3d_animation_builder.h).
    - No existing shared header was edited.
    - 13 rebuild/compile-gate/*.json receipts were refreshed. All pass, but the JSONs changed: check whether any is a
      snapshot input.
    - Future work: native wants to sweep the CCountedPointer::DeleteData access mangling (CIX vs the PDB's SIX) across
      shared headers. That needs its own cut, never inside a matrix freeze.

  The next bundle takes a cut against the 183 cut. Freeze protocol unchanged ("native freeze" / "native unfrozen").
- D: free space at start: 487 GiB, above the 150 GiB floor in CLAUDE.md. The next prepare_matrix applies the CLAUDE.md
  hard-link snapshot patch to the `validation_matrix/sources` copy loop.
- 184 candidate: the 177 ScreenEffectStack sNaN correction. Lane `work/modern_owned_screen_effect_nan_20261029/` was
  restarted; the preimages are re-verified equal to production (sha256 9c2d1856... header, e4194130... tests).
- Native landed after the first 184 freeze (record these in the next cut):
  - cloth_primitive_bone_queries: new fable_particle_program.h and fable_3d_cloth_primitive.h; engine/CParticleProgram.h
    becomes a forwarder.
  - anim_sequence_setters: edits the shared fable_chunked_file.h.
  - anim_chunk_delete_data.
  - counted_pointer_destructors extended from 4 to 7.
  - nav_vector_map_sort: edits the shared fable_vector_map.h. Sort() is now unconditional and the old guarded body is
    Update(); 10 native callers were renamed to match. Check modern contact.

  Native deferred the CCountedPointer DeleteData sweep indefinitely (141+ occurrences).
- **Blocker found at the 184 preflight:** D:\Code\PS1Godot no longer exists on any drive (removed outside these lanes).
  - Every modern build cache sets FABLE_ZLIB_SOURCE_DIR=D:/Code/PS1Godot/godot-master/thirdparty/zlib.
  - The 0019 definition-block-cache receipts pin zlib.h (f7632df0...) and zconf.h (918c592f...) at that absolute path.
  - Native was unfrozen while this is fixed.
- Parallel slice: ResetToFrontEnd 0049dcc0. Lanes `work/modern_reset_front_end_tree_20261103/` and
  `work/modern_owned_reset_front_end_20261103/`.
- **184 accepted** (191 checks x5) after two freezes. Run 1 passed every check but failed `inputs_unchanged` for two
  reasons: its own log in the bundle dir, and the first relink of the zlib libs. Both runs are kept under `attempts/`.
  Details are in NEXT_SESSION "after 184".
- The sNaN staging agent finished. A second fix was needed in `blend()`: keep is `fld 1.0; fsub dword w`, whereas the
  accepted `pc53::subtract` returned the default NaN for any NaN or infinite weight.
  - Accepted vs staged on the 3,072-case hardware supplement: 1,202 mismatches vs 0.
  - Differential: 23.4M comparisons, all identical.
  - The 177 corpus re-run on hardware shows 0 Unicorn artefacts.
- **185 accepted** (192 checks x5) on its first matrix. The cut held one additive native header (fable_dinput8.h). The
  sNaN lane's strict and mutation receipts were re-run after the zlib relocation, with identical results.
- **186 accepted** (193 checks x5, OpenHeroInformationScreen 0049f3f0) on its first matrix, with no native cut.
  Agent lane, then my review, then the bundle scripts generated from 184/185.
- **187 accepted** (194 checks x5, five UpdateFrame frame services) with no cut. Wrong labels went to native
  (fabletlc-b9), which will verify them before editing the manifest or fable_engine.h.
- Wrong labels from the 181/184/186/187 lanes are in `ghidra_out/labels_modern_world_frame_20261103.tsv` (11 rows),
  **not yet applied** with LabelApply.java (Ghidra was not running). Native fixed the four UpdateFrame rows in
  rebuild/manifest/functions.tsv. The generated ghidra_out/engine_api.tsv and fable_engine.h still carry the old names
  until the labels are applied and regenerated.
- Native changes after 187, for the next cut:
  - fable_environment.h: new CEnvironmentSpellEffectInterface class and SpellEffectInterface member at +0x1960.
    MainGameReplayTests reaches it through fable_world.h.
  - engine/CBulletTimeManager.h: one added declaration.
  - New native MATCH sources for 006b32a0 and 004c5e90. They retire the stand-ins Consval..._006b32a0.cpp and
    E2_E2_004c5e90.cpp; check whether an old modern receipt pins either one.
  - rebuild/manifest/functions.tsv: 4 relabels.
- **188 accepted** (195x5, region wait = reset-to-front-end confirmation wait). Its cut covered the native
  fable_environment.h and CBulletTimeManager.h edits.
- **189 accepted** (196x5, world map + camera manager update), run under the same freeze with no cut.
- New `work/modern_bundle_kit/kit.py` with a per-bundle `bundle.py` spec replaced error-prone text substitution.
- `labels_modern_world_frame_20261103.tsv` now has 17 rows; it is still not applied in Ghidra.
- Native changes after 189, for the next cut (the cut base is 188):
  - fable_player_manager.h: EGameAction/EInputButtonEventType, CPlayer::JoystickDeviceNumber +0x228, and the two
    IsGameActionEventInQueue overloads.
  - In place: 00/44/CGamePlayerInterface_IsGameActionEventInQueue_00446b10.cpp, an _emit bake de-baked to genuine C++
    MATCH, together with its test.
  - New: 00/48/CPlayer_GetJoystickDeviceNumber_00487510.cpp. It retires global_DrawGetEnvironment_00487510.cpp.
- **190 accepted** (197x5). Its cut covered the native fable_player_manager.h change.
- 191 failed the architecture boundary check on all 5 builds (app/events must not include simulation headers). It was
  un-promoted and is being reworked (UpdateDeaths becomes a host capability).
- **191 accepted** (198x5) on its second attempt. The cut covered native's cloth, particle-program and group2 headers.
- **192 accepted** (199x5, crash marker). Its cut covered native's fable_3d_group2.h.
- **193 accepted** (200x5, UpdateSaveGameLoad), with no cut. UpdateRegionLoad (8.9 KB) is mapped and split into
  R1-R6; R1 is in progress.
- **194 accepted** (201x5, UpdateRegionLoad R1). Its cut covered native's group2 / particle_program header edits.
  kit_v4 was written for the replace-owner and simulation-owner bundles that come next.
- **195 accepted** (203x5, camera rotation angle, simulation owner with msvcr71 _CIacos hardware evidence). It was
  the first kit_v4 bundle.
- **196 accepted** (203x5, UpdateRegionLoad R2 replacing R1's owner; kit_v5).
- Native changes after 196, for the next cut (the cut base is 196):
  - fable_particle_simulation.h: adds an inline ctor.
  - New native sources: the CParticleProgram ctor 00ae3550, GetUsedBones 00ae1210, and CParticleSimulation::RelocateData
    00adfc60.
  - In place: 00/ae/CParticleProgram_RelocateData_00ae04c0.cpp (7807bdaa -> e074b993) and its test (f3903f17 ->
    08f4d396). If preflight shows that an old receipt pins either, add a source_pin_cut.
- **197 accepted** (205x5, the CGameCameraBase::Update body with hardware x87 evidence).
- **198 accepted** (205x5, UpdateRegionLoad R3 / state 5), with no cut.
- Native changes after 198, for the next cut (the cut base is 197):
  - fable_stlport_vector.h: a FABLE_VECTOR_RESIZE_OUT_OF_LINE knob. The default path is token-identical.
  - fable_3d_group2.h: Resize declaration.
  - fable_particle_program.h: ReadGroups declaration.
  - New 00/ae ReadGroups source.
  - 13 rebuild/compile-gate receipts are being refreshed.
- **199 accepted** (205x5, UpdateRegionLoad R4 / state 4). Its cut covered native's stlport_vector knob plus the
  group2 / particle_program declarations.
- Late: the 200 matrix is running. Native reports the user shutting down; the resume point was written to NEXT_SESSION
  ("2026-11-03 late, 200 running").
- **Session end (user).** 184-199 accepted today (16 bundles). The 200 matrix was stopped mid-run, and the state-1b agent
  was stopped while incomplete. Resume instructions are in NEXT_SESSION "Session end 2026-11-03".
