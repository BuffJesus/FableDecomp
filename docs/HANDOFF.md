# FableDecomp working handoff

Maintenance session wrapped at the user's request on October 24. See the
[repository maintenance handoff](journal/2026-10/2026-10-24-repository-maintenance-handoff.md)
for merged PRs 39/40, validation, preserved Git state, evidence locations and
next-session checks. The lane states below must be reconciled with each owner's
latest checkpoint before restarting work.

Updated October 24, 2026. Use the owning lane's journal for its latest qualified
checkpoint; status and test totals can change while work continues.

| Lane | Current instruction | Resume point |
|---|---|---|
| Native C++ | ACTIVE 2026-11-04 (user: recreation, not Lua, until told to stop; modern lane is now fabletlc-0a and sends "native freeze" before matrices): 2026-11-04 (later): mesh_partition_element 14, index_buffer_queries 4, vertex_pair_contraction 3, vector2_normalise 1 (GFFastOneOverSqrt inline; STLport single erase now POD-aware); then optimised_face_normal (GetFaceNormal 0xAC7CB0 NEAR_MATCH 0.950; C3DVector inline Normalise/IsNonZero/^ verified byte-exact vs retail 0x486F60) and reward_hero_adjust_trajectory 1, then optimised_face_texture_tangents 1 and optimised_make_orthogonal NEAR_MATCH 0.978, optimised_texture_tangent NEAR_MATCH 0.939, optimised_self_contraction NEAR_MATCH 0.969, optimised_remap_groups 1, particle_engine_accessors 19, particle_engine_lifetime 3, emitter_database 2 (journal top entry). Earlier 2026-11-04 CParticleProgram/CParticleSimulation runtime landed: 15 MATCH + ApplyForces NEAR_MATCH; C3DVector gains its member-wise copy ctor; the 0xADF1E0-0xAE3C30 block is one retail TU (define callees first in the caller's source); next CalculateAveragePatchSize 0xAE0710 / ApplyConstraints 0xAE2510 (journal top entry). 2026-11-03: 2026-11-03 landed wf-aq + quadric_optimised_face 6, animation_manager_bank 2, nav_connector_delete_data 1, bone_info_delete_data 1, index_buffer_relocation 4, cloth_primitive_bone_queries 2, anim_sequence_setters 4, anim_chunk_delete_data 4, nav_vector_map_sort 4, counted_pointer_destructors +3, joystick_dx_enum_and_init NEAR_MATCH, cloth_counted_pointer 4, environment_update_interpolation 1, bullet_time_frame_update 1, game_action_event_in_queue 1 (de-baked), player_joystick_device_number 1, 13 manifest relabels, cloth_primitive_construct (ctor NEAR_MATCH + Reset/DeleteData), group2_construct 1, particle_program_group_lists 2, particle_remap_group_bones NEAR_MATCH, group2_set_bone_index 1, group2_basics 2, particle_program_add_group 1, particle_simulation_clear 1, particle_program_clear 1, particle_program_ctor 1, particle_program_used_bones 2, particle_simulation_relocate 1, particle_program_relocate 1, particle_program_read_groups 1, group2_resize 1, particle_simulation_read NEAR_MATCH, particle_program_read NEAR_MATCH, particle_allocate_arrays 2, particle_read_vector_array 1, particle_vector_resizes 5, 17 manifest relabels total; indexer receipts requalified (see journal top entry). Earlier: 0xA2 header batches landed - gate_native_steering_behaviour 9 MATCH, gate_native_mesh_data_bank 43 MATCH (registered, indexed at retail addresses, neighbour gates PASS); gate_native_wad_compressor 35 MATCH (0xA3: CCompressorZlib, C2DBoxI, CWADFile, CWADBankStateBlock) gate_native_lipsync_data_bank 5 MATCH and gate_native_suspendable_process 4 MATCH (0xA4) , gate_native_core_services_a5_a6 19 MATCH , gate_native_allocator_font_state 13 MATCH and gate_native_nav_quad_tree_nodes 26 MATCH, gate_native_nav_leaf_memory_usage 1 MATCH and gate_native_lipsync_entries 22 MATCH (shared STLport vector now non-POD aware) gate_native_lipsync_construct 2 MATCH and gate_native_physics_faces_segment 8 MATCH and gate_native_mesh_x_chunk_descriptions 7 MATCH, gate_native_mouse_dx_accessors 11 MATCH, gate_native_joystick_dx_overrides 11 MATCH, gate_native_keyboard_dx_is_key_pressed 1 MATCH, gate_native_mouse_dx_focus 4 MATCH, gate_native_joystick_dx_focus 2 MATCH, gate_native_keyboard_dx_focus 1 MATCH, gate_native_keyboard_dx_get_key_name 1 MATCH, gate_native_joystick_dx_exclusive_mode 1 MATCH, gate_native_mesh_cloth_chunk_description 1 MATCH, gate_native_mesh_chunk_delete_data 7, gate_native_3d_group2_vertex_blends 2, gate_native_counted_pointer_destructors 4 MATCH landed (caller-identity workflow results in work/native-recreation/wf-id/identities.json; wf-aq staged; STOP POINT 2026-10-29 at the top of the owner journal) (291 duplicate stand-ins retired: work/native-recreation/find_dupes.py) (nav duplicate stand-ins retired; work/native-recreation/retire_dupes.py); header batches can be verified while frozen via wf-a7/mirror_check.py; generic lander work/native-recreation/batch_land.py; earlier ranges 0x9D8000..0xA20000 as before | [Native continuation](journal/2026-10/2026-10-24-native-recreation-continuation.md) (top entry, 2026-11-04) |
| C++23/x64 modernization | Stopped for the night 2026-10-28 at user request; 149-171 accepted; 172 (level theme: GetLevelTheme + BlendInLevel) promoted locally and preflighted, full matrix not yet run; nothing committed | [Modern resume](modern/NEXT_SESSION.md) (2026-10-28 resume point) and [owning journal](journal/2026-10/2026-10-28-modernization.md) |
| Lua recovery | Stopped for tonight at user request; W507 accepted/W509 bundle verified; W511 scratch-only next; W499 ABI gate retained | [Lua handoff](scripts/LUA_RECOVERY_HANDOFF.md) |
| Forge | Paused at the user's request on October 24 | Separate owning checkout's `docs/HANDOFF_2026-10-24.md`, final pause section; 264-stage checkpoint. Do not operate on Forge from this checkout |

Native font/resource ownership and modern scheduling policies are bounded local
recoveries. They do not establish glyph rasterization, a complete modern
controller, playable startup, or a complete engine. Scratch work is not accepted
source until its owning lane completes promotion and validation.

## Working safely

The shared checkout is dirty and used by multiple sessions. Coordinate exact
paths through `.agents/CLEANUP_COORDINATION.md`; preserve active source, original
inputs, failed attempts, and private work/build evidence. Do not switch, reset,
stash, stage, or commit the accumulated shared tree as part of documentation work.
Publish reviewed changes from an isolated checkout.

[Roadmap](ROADMAP.md) summarizes follow-up priorities. [Architecture](ARCHITECTURE.md),
[building](BUILDING.md), [project layout](PROJECT_LAYOUT.md), and the
[verification contract](pipeline/VERIFICATION_CONTRACT.md) describe the rules
and current assurance limits. [INDEX](INDEX.md) is the reference catalog;
[journal index](journal/INDEX.md) contains dated history.

Earlier resume instructions and pause/active notices are preserved in the
[verbatim pre-cleanup handoff](journal/2026-10/2026-10-24-before-cleanup-HANDOFF-9937a912eafe.txt). They are historical records,
not commands to restart another lane.
