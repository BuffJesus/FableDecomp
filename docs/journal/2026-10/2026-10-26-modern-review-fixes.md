# Modern review fixes, 2026-10-26 evening

Owner journal continuation; see also `2026-10-24-modernization.md` (adversarial review section).

## WorldFrame (150): review gaps closed
- Native `work/modern_world_update_frame_20261026/` gains `capture_final2.py`, `cases_final2.py` and `audit_final2.py`.
  - Coverage added for L1-06: GameTime+8 is seeded and replaced by the 6bb990 service, so the 6b3ff0 argument is pinned by value and read order.
  - New mutation points and directed witnesses cover L1-02, L1-03 and L1-04: things+54 changed at the camera update, region status changed at aeaaf0/aeb4a0/419680/4c6060/4b4490, and paused changed at the music update.
  - The harness now has an x87 stack-empty guard (FPSW TOP==0 on return) and an exact caller-frame/argument guard. Three new original-byte controls are all rejected: environment_time_source, x87_stack_leak and bullet_store_into_caller_slot.
  - Results: 520 cases, 12,760 calls, 23 controls; the independent audit_final2 matches.
- Portable `work/modern_owned_world_update_frame_20261026/` gains a test that checks the environment-time argument, the update_player slot against the queried slot, and a 25-field state.
  - strict_x86/x64/gcc_final2 pass 520 cases in four modes.
  - mutations_final4 rejects 35/35 controls, including the four wrong implementations the review described.
- Native cut review is in `snapshot_requalification_final2/`. The native delta was CMatrix4x4 operator()/AlignedSetMultiply declarations plus the new fable_matrix4x4_simd.h (1,725 to 1,726 inputs, no layout change). Five fresh snapshot tests pass.
- `promote_final2.py` amended the applied 150 with the strengthened root WorldFrameTests.cpp only (`promotion_final2.json`, preimage kept). receipt_closure_final2 passes (9 reports, 1,841 pins). The regenerated validate_final2/preflight_final2/wait_matrix_final2 merge both promotion records through the latest_hash chain.
- Not fixed and recorded as policy: WF-1, where marker clock errors are typed and abort the frame (documented checked boundary), and L1-11, where paused byte values other than 1 are normalized to bool.

## Next
- Run the 150 final2 preflight, then the full matrix with the log outside the repository.
- Fix the game-time findings (GT-1 inf wrap, per-path validation, PC53 normal-range claim, audit -0, negative-seconds cases, untouched-field checks, x87 guard) before 151.

## Game time (151 candidate): review findings fixed
- `GameTime.hpp`:
  - GT-1: wrap_day passes +inf through, matching _ftol2's indefinite low word 0 and inf - 0 = inf.
  - GT-2/L2-04: validation is now per path. Only values retail uses in arithmetic on the taken path must be finite; fields retail does not read are never inspected; the village path checks Time only inside the band.
- `Pc53Arithmetic.hpp` (PC53-1): the doc limits PC53 equivalence to zero/normal binary64 results; narrow() passes infinities through.
- Native `work/modern_game_time_update_20261026/`:
  - capture_final3 adds an x87 stack-empty guard and the forward_x87_leak control (15 controls), plus 256 L2-02 domain cases: negative seconds, negative left, zero-left/positive seconds, signed zeros. That gives 4,368 cases.
  - The new audit_final3 tracks IEEE signed zeros exactly (L2-01) and matches.
- Portable:
  - The test checks untouched members (L2-03) and adds witnesses for the GT-1/per-path cases.
  - strict_x86/x64/gcc_final3 pass 4,368 cases in four modes.
  - mutations_final3 rejects 28/28 controls.
  - The attempts/ folder keeps the first runs: an anchor that matched twice, a heredoc syntax error, 3 survivors that needed the path witnesses, and one ill-formed control.
- prepared_final2.json and receipt_closure_final2 (8 reports, 119 pins) pass. promote.py is now pointed at the 150 validation_matrix_final2 receipt and the final3 receipts.
- After 150 final2 is accepted: run promote.py, then derive a 151 prepare_matrix from the frame lane's validate_final2.py, adding the GameTime test command build/simulation/fable_game_time_tests.exe with game_time_final3.txt.

## Script-timer tick (next boundary from the review scout)
- Native `work/modern_script_timer_tick_20261026/` (DESIGN.md): the genuine 006e75c0 tick, STLport map iterator/increment and GetFrame, with only the two vetoes supplied. 2,048 cases, 11 controls, independent audit matches. Gotcha: run one Unicorn engine per case when cases raise CPU exceptions.
- Portable `work/modern_owned_script_timer_tick_20261026/` (README.md): strict three-compiler runs pass and mutations_final2 rejects 10/10. Not promoted; it queues behind 150/151 as a three-file simulation proposal.

## Message-event expiry (scout #2)
- Native `work/modern_message_event_expiry_20261026/`: 2,048 cases, 7 controls, independent audit matches. The comparison is unsigned with wrapping deadlines; Lifespan 0 never expires; the frame is re-read for each event; the node is unlinked before its destructor runs.
- Portable `work/modern_owned_message_event_expiry_20261026/`: strict three-compiler runs pass. mutations_final2 rejects 8/8; a survivor in the first run exposed a missing unlinked-before-destroy test check, which is now added.
- Promotion queue after 150: 151 game time, then script timers, then message expiry. Each one needs its own full matrix, or they can be grouped into a single matrix.

## Engine update pump (scout #3): CEngineManager::OnUpdate 00640320 / OnPausedUpdate 00640360
- Native `work/modern_engine_update_pump_20261026/`: the genuine bodies run with the CIEngine vtbl+cc/+24/+28 slots as boundaries. Callbacks insert after or unlink the successor mid-walk, pinning that retail reads `next` after the callbacks. 2,048 cases, 8 controls, independent audit matches (664 inserted engines visited, 421 skipped). OnUpdate forwards the caller's full flag dword.
- Portable `work/modern_owned_engine_update_pump_20261026/`: app/events EnginePump.hpp (update_engines/update_paused_engines). Strict three-compiler runs pass; mutations rejects 6/6 controls. attempts/ keeps the first GCC misleading-indentation failure.

## Matrix 150 final2 COMPLETE/PASSED (accepted)
Receipt: `work/modern_owned_world_update_frame_20261026/validation_matrix_final2/summary.json`. 150 checks per build; CTest 186/186/186/169/166 (MSVC x86/x64/clang-cl/GCC/headless); 8,617 frozen inputs, external headers and the reconstruction snapshot (1,726 inputs) unchanged. The WorldFrame owner (CWorld::UpdateFrame) is accepted locally together with the review-strengthened test. The failed initial 150 (concurrent native writes) and the preflight timeout attempt are retained.

## 151 game time promotion APPLIED; full matrix ACTIVE
`promote.py` applied the four files (simulation Pc53Arithmetic.hpp, GameTime.hpp, tests/GameTimeTests.cpp and the CMake registration) against accepted 150 final2. Two generator defects were fixed before any build, and the broken outputs are kept in attempts/:
- **Name collision:** the earlier GameTimeOffset lane already owns `game_time_promotion` and `previous_game_time_latest_hash`, so reusing them created a latest_hash cycle (RecursionError). The new lane now uses `game_clock_*`.
- **Direct pins:** three older lanes (cached_attach, texture, frontend_cache) pin their promoted files directly. A `game_clock_expected` helper maps exactly the hash 151 replaced to 151's result.
Preflight PASSED with 8,725 inputs. The full 151 matrix is ACTIVE with its log in the scratchpad. Accepted remains 150 final2.
Gotcha: give every new lane a unique variable prefix in generated validators.

## 152 frame-services bundle prepared (not promoted)
`work/modern_owned_frame_services_20261026/` bundles script timers, message expiry (simulation) and the engine pump (app/events): eight files, including two CMake registrations prepared against the post-151 roots. receipt_closure passes (22 reports, 123 pins). promote.py requires the accepted 151 receipt. prepare_matrix derives from the 151 validator with unique `fs_*` names, routes the older direct pins through `fs_expected`, and adds the three test commands (len 154).

Sequence once 151 passes: run promote.py, prepare_matrix.py and preflight.py, then the wait_matrix.py loop (log outside the repository).

## Particle emitter sweep (scout #4): CParticleEmitterManager::Update 00aeb4a0
- Native `work/modern_particle_emitter_sweep_20261026/`: the genuine body plus the landscape leaf 00aed010 run with update/secondary/handle-release/handle-free/node-free as boundaries; callbacks kill emitters and rewrite handle counts. validation has 2,048 cases. validation_final2 has 2,304 cases (it adds 256 witnesses where the update callback rewrites the +44 flags), with 9 controls. audit_final2 matches.
- Portable `work/modern_owned_particle_emitter_sweep_20261026/`: simulation ParticleEmitterSweep.hpp (ActiveEmitter list, CountedHandle, splice-unlink then release/free). strict_* pass on the final2 corpus; mutations_final2 rejects 9/9. The first controls had one survivor, flags_cached_before_update, which needed the final2 witnesses.
- Candidate for bundle 153 (with any further frame services), after 152.

## Matrix 151 COMPLETE/PASSED (accepted)
Receipt: `work/modern_owned_game_time_20261026/validation_matrix/summary.json`. 151 checks per build; CTest 187/187/187/170/167; 8,725 frozen inputs, external headers and the reconstruction snapshot unchanged. The CGameTimeManager::Update owner and the reusable Pc53Arithmetic are accepted locally.

## Environment camera blend (scout #5, private, unfinished): CWorld::UpdateEnvironmentCameraPos 0049e370
Native `work/modern_environment_camera_blend_20261026/`: capture.py runs the genuine body + GetAtmosBlend 006b74e0 + _ftol2 under PC53, with the map vtbl, GetPGameMapCellAt, the local->global theme lookup and SetWeight supplied. The exact audit.py (signed-zero arithmetic loaded by path from the game-time audit_final3) matches all 4,096 cases in a dry run. NOT yet recorded: run `capture.py` (writes validation/ with 8 controls), then `audit.py`. There is no portable owner yet.

## SESSION STOP (user request, 2026-10-26 night)
- **Accepted:** 149final4, 150 final2 (WorldFrame), 151 (game time).
- **Next:** 152 frame-services bundle (`work/modern_owned_frame_services_20261026/`: script timers, message expiry, engine pump; closure passes). Run `promote.py`, `prepare_matrix.py`, `preflight.py`, then the `wait_matrix.py` loop with the log OUTSIDE the repository.
- **Privately qualified, not bundled:** particle emitter sweep (`work/modern_owned_particle_emitter_sweep_20261026/`, final2 receipts). Bundle it into 153 with the environment blend once that has a portable owner.
- No jobs are running; nothing was committed, published or launched.
