# Modernization owner journal, 2026-10-27

This continues `2026-10-26-modern-review-fixes.md`. Accepted at the start of the day: 149final4, 150 final2 (WorldFrame) and 151 (game time).

## 152 frame-services bundle: promoted, matrix queued
- `promote.py` applied the eight files against the accepted 151 receipt (`promotion.json`; the preimage is kept in `production_preimage/`). Hashing about 8.7k inputs took about 40 minutes on a loaded machine.
- `prepare_matrix.py` wrote validate/preflight/wait_matrix. Preflight PASSED with 8,827 inputs.
- The full matrix runs through a `wait_matrix.py` retry loop (exit 2 means the lease is held by the external Fable2Native session). The log is in the session scratchpad, outside the repository.

## Environment camera blend (153 candidate): CWorld::UpdateEnvironmentCameraPos 0049e370
Native lane: `work/modern_environment_camera_blend_20261026/`.
- **validation/ (capture.py):** 4,096 cases and 8 controls. audit.py matches.
- **final2 (prepare_final2.py):** adds 512 witnesses:
  - coordinates whose quarter leaves the int32 range (the fild of _ftol2's low word wraps) or reaches 2^63 (indefinite, low word 0);
  - binary32 subnormals and signed zeros;
  - local-theme callbacks that rewrite a cell byte mid-walk. These pin that retail reads each theme byte and each blend byte lazily.
  - Two new controls: `ftol_low_word_16bit` and `blend_byte_offset`.
  - The first final2 run met a held lease. Its empty folder is kept in `attempts/`.
- **Finding:** once both coordinates exceed about 2^64, the binary32 corner weights overflow to ±inf. Retail then follows x87 masked-invalid rules:
  - 0*inf and inf+(-inf) give the real indefinite (stored 0xffc00000);
  - NaN propagates;
  - a NaN total never reaches SetWeight, while a +inf total emits +inf weights.
  - The first audit_final2 crashed on this; the copy is kept as `attempts/audit_final2_without_x87_specials.py`. The fixed audit_final2 matches all 4,608 cases.
- **final3 (prepare_final3.py):** adds 256 special-operand witnesses, for 4,864 cases and 10 controls. audit_final3 matches, and 73 cases emit +inf weight tables.

Portable owner: `work/modern_owned_environment_camera_blend_20261026/`.
- `staged/include/fable/simulation/EnvironmentCameraBlend.hpp`:
  - `blend_environment_at_camera(x, y, host)` calls the host's cell_at / default_theme / global_theme / set_theme_weights;
  - it uses the accepted Pc53Arithmetic plus a local x87 special-operand multiply/add;
  - a non-finite position is rejected before any host call (documented checked boundary).
- The test replays the final3 export in four rounding modes, with constexpr witnesses for the low-word wrap and the indefinite path. There are 16 source mutation controls (`validate_mutations.py`).
- Strict x86/x64/GCC builds are queued behind the lease.

## 152 rerun after a native cut change
- The first full matrix stopped at its input pins before any build: the native lane rewrote `rebuild/include/fable_font_data_banks.h` at 06:16, after preflight. The empty run and its log are in `attempts/validation_matrix_native_font_cut_drift*`.
- The native change: CStreamingFontDataBankEntry gains base CBaseClassNonCopyable and an inline constructor. The vptr it already had is shared, so there is no size or offset change, and no modern source uses the class.
- A 152-owned cut was recorded: `prepare_source_cut.py`, `review_notes.json`, then `qualify_source_cut.py` (five snapshot tests pass, 1,726 inputs).
- `prepare_matrix_final2.py` re-points the snapshot pin to that cut. The first final2 preflight failed on the 150 receipt_closure_final2 pin of the old header (kept in `attempts/final2_first/`). final2 now adds the uniquely named `fs_cut_*` wrapper, which maps exactly the cut's before hash to its after hash. Preflight final2 PASSED with 8,841 inputs, and the full final2 matrix is running.

## Environment camera blend: privately qualified
- final4 (prepare_final4.py) adds 64 emit-threshold witnesses, found with the exact audit model. In 56 of them the binary32 total equals 1/256 exactly, and retail skips SetWeight on equality. They were added because the portable control `threshold_inclusive` survived the final3 corpus (kept in `attempts/`). Native final4 has 4,928 cases and 10 controls, and audit_final4 matches.
- MSVC gotcha: the constant evaluator mishandles `bit_cast<int32_t>` of a negative low word (recorded in GOTCHAS). truncated_low_word now converts arithmetically; the failed x86 run is in `attempts/`.
- strict x86/x64/GCC pass the 4,928 cases in four rounding modes, and validate_mutations rejects 16/16 controls. One earlier run had a stale anchor; it is kept in `attempts/`.

## 153 world-effects bundle prepared (not promoted)
`work/modern_owned_world_effects_20261027/` bundles the environment camera blend and the particle emitter sweep: five simulation files, including one CMake registration. receipt_closure passes (15 reports, 112 pins). promote.py requires the accepted 152 final2 receipt (len 154). prepare_matrix.py derives from the 152 final2 validator with the `we_` prefix and adds the two test commands (len 156). Sequence: once 152 final2 passes, run promote.py, prepare_matrix.py and preflight.py, then the wait_matrix.py loop.

## 152 final2 runs (both retained, neither accepted)
- Run 1: GCC failed at configure. Another session had turned on `FABLE_SAVE_WITH_LZO` in the shared `work/modern_build_gcc` cache (10-26 19:57), with its LZO source in a now-deleted scratchpad. The two cache entries were reset to the defaults (OFF/empty); the backup is `attempts/modern_build_gcc_CMakeCache_before_lzo_reset.txt`. x86/x64/clang/headless had passed 154/154.
- Run 2: the GCC LZO fix held. The native lane edited fable_streaming_font_cache.h and fable_font_instances.h at 07:12 (1,726 -> 1,727 inputs), and later fable_system_manager.h, fable_font_data_banks.h, fable_d3d9.h and fable_surface_types.h (07:29-07:33). That failed the clang/GCC snapshot configure. Archived as `attempts/validation_matrix_final2_native_font_cache_drift`.
- Next: once the native inputs are quiet for 30+ minutes, record a final3 cut (prepare/qualify against the 152 cut), then regenerate final3 validate/preflight/wait with the cut wrapper covering both cuts.

## 154 candidate: frame queries and walks (`work/modern_frame_queries_20261027/`)
Genuine bodies in Unicorn:
- IsBulletTimeActive 00419680;
- IsTimeForFullUpdate 004c6060, with the multiplier sum 004c5fa0, _ftol2 and GetFrame 0049d870;
- UpdateGlobalTCs 00686dc0;
- UpdateFrame_IterateConversations 006e60f0;
- CThingManager::UpdateDeaths 0051f070.

Native capture: validation has 4,608 cases and 12 controls; audit.py (an independent pointer/exact-arithmetic model) matches. final2 (prepare_final2.py) adds 256 multiplier-precision witnesses for 4,864 cases, and the audit_final2 run matches.

Recovered behaviour:
- The sum is 0.0 plus each map multiplier, unrounded PC53. The default int is used when UseDefaultMultiplier is set or the count is 0.
- idiv faults on a zero divisor (110 cases), including non-finite sums through the indefinite path.
- The TC walk reads the flag and next before each update, so an unlinked successor is still visited.
- The conversation walk takes its successor before FrameUpdate.
- UpdateDeaths re-reads size and storage each step, calls RemoveFromUpdateList even for null entries, destroys with vtbl+0(1), then clears the list.

Portable owner: `work/modern_owned_frame_queries_20261027/` (simulation).
- BulletTime.hpp (typed zero_divisor / quotient_overflow errors).
- FrameServiceWalks.hpp: an intrusive GlobalComponentNode mirroring the retail list, a std::map conversation walk, and a vector dead-thing walk.
- strict x86/x64/GCC passed on the first corpus. 14/15 controls were rejected; the survivor `sum_rounded_each_step` motivated final2 (first-corpus receipts are in attempts/). Reruns on final2 are in progress.
- Follow-up: bullet_time_detail::truncated_low_word duplicates the environment blend helper. Unify it into one shared header after 153/154 land.

## 154/155 privately qualified and folded into 153
- Frame queries (154): the final2 reruns pass strict x86/x64/GCC on 4,864 cases in four rounding modes, and mutations reject 15/15.
- Dialogue-element frame update (155, 006e37d0; native `work/modern_dialogue_element_update_20261027/`):
  - 2,048 cases and 8 controls; the independent audit.py matches on its first run.
  - Portable `work/modern_owned_dialogue_element_update_20261027/`: DialogueElementUpdate.hpp, an intrusive node that mirrors the retail list. A kept element continues from the next link read before its update; a dropped one re-reads next, decrements its 16-bit counter, unlinks, destroys and frees. The clock is re-sampled even for an empty list.
  - strict x86/x64/GCC pass, and mutations reject 9/9.
- Since 152 is not yet accepted, the 153 bundle (`work/modern_owned_world_effects_20261027/`) now carries four lanes in ten files: environment blend, emitter sweep, frame queries and dialogue elements. Closure: 29 reports, 192 pins. promote.py and prepare_matrix.py now target the 152 **final3** receipt and validator. The earlier proposals are in attempts/.

## 152 final3
- The native lane lands continuously (streaming font entry, font vertex-buffer growth, glyph cache types, D3DFMT enums; last edit 08:07), so the final3 cut was taken after 10 quiet minutes. A drift during the matrix fails it safely.
- `prepare_source_cut_final3.py` takes before copies from the first archive whose hash matches the 152 cut; one archive postdated an edit. There are 6 changed inputs plus 1 new one, and all are declaration-only (`snapshot_requalification_final3/review_notes.json`).
- `prepare_matrix_final3.py` maps older pins through both cuts in order. The qualify → preflight_final3 → wait_matrix_final3 chain is running.

## Matrix 152 final4 COMPLETE/PASSED (accepted)
- Receipt: `work/modern_owned_frame_services_20261026/validation_matrix_final4/summary.json`. 154/154 checks on each of x86, x64, clang-cl, GCC and headless. Inputs (8,856 at preflight), external headers and the reconstruction snapshot (1,727 inputs) unchanged.
- Run inside a coordinated native freeze: native session fabletlc-bb confirmed "native frozen" over cross-session messages, at the user's request, and extended the freeze through 153.
- final3 was overtaken first: the render-state header edit at 08:21 landed after the final3 qualify (preflight kept in attempts/). The final4 cut (render-state defaults extern plus a friend declaration) was taken during the freeze.
- The first final4 matrix loop exited 1 without output after four lease waits and is not understood. The rerun with per-try logging passed on its first try.
- Accepted script timers, message expiry and engine pump.

## 153 promoted; full matrix ACTIVE
- promote.py applied the ten files against the 152 final4 receipt (repointed from final3). prepare_matrix.py had a broken string from the dialogue-lane insertion, fixed before any build.
- Preflight PASSED with 9,026 inputs, and the matrix is running inside the same freeze.

## Matrix 153 COMPLETE/PASSED (accepted)
- Receipt: `work/modern_owned_world_effects_20261027/validation_matrix/summary.json`. 158/158 checks on each of x86, x64, clang-cl, GCC and headless. Inputs (9,026), external headers and the reconstruction snapshot unchanged.
- Accepted owners:
  - EnvironmentCameraBlend (CWorld::UpdateEnvironmentCameraPos, including x87 special operands);
  - ParticleEmitterSweep;
  - BulletTime;
  - FrameServiceWalks (UpdateGlobalTCs, IterateConversations, UpdateDeaths);
  - DialogueElementUpdate.
- "native unfrozen" was sent to fabletlc-bb at 11:54.
- Follow-up: unify bullet_time_detail::truncated_low_word with environment_blend_detail's copy in one shared header. This needs its own matrix.

## 154 prepared (not promoted): shared x87 helper + CMusicManager region update
- **Shared helper** (`work/modern_owned_x87_conversions_20261027/`):
  - X87Conversions.hpp `x87::ftol2_low_word` replaces the two identical copies in EnvironmentCameraBlend.hpp and BulletTime.hpp; their tests are updated. `prepare_staged.py` rewrites the accepted 153 files.
  - The test checks an independent oracle (int64 truncation) over 4,960,024 values in four modes.
  - strict x86/x64/GCC also replay both full native corpora. 6/6 controls are rejected after a double-widening witness was added: `bitcast_low_word` first survived because the int32-only static_asserts cannot see the MSVC hazard (attempts/).
- **Music region** (native `work/modern_music_region_update_20261027/`, 006bdc60):
  - Genuine body plus five tiny accessors and _ftol2; 15 recording boundaries; 2,048 cases; 9 controls; the independent audit.py matches on its first run. The literal is "SOUND_THEME".
  - The first capture hit retail's null ref+44 read: a found definition without a reference, which the game never produces. It is kept in attempts/, and the generator now ties found to ref.
  - Semantics: CMusicManager (PDB CState; retail offsets are Ego_r minus 4 after RegisteredMusic). It consumes the restart request and plays pending music. With a live hero, the cell's SOUND_THEME definition +44 becomes MapCellSet and the region category object's +78 (under a temporary reference) becomes RegionSet. It copies Paused, applies the state after the registered-music update, then releases the definition.
  - Portable `work/modern_owned_music_region_update_20261027/` MusicRegionUpdate.hpp: strict x86/x64/GCC pass; 11/11 controls are rejected. One ill-formed control (an unused reference under /WX) was fixed and is kept in attempts/.
- **Bundle** `work/modern_owned_x87_music_20261027/`: nine files; closure 13 reports, 103 pins. promote.py requires the accepted 153 receipt (len 158). prepare_matrix.py uses the `xm_` prefix (len 160). A native freeze was requested from fabletlc-bb.

## Matrix 154 COMPLETE/PASSED (accepted)
- Receipt: `work/modern_owned_x87_music_20261027/validation_matrix/summary.json`. 160/160 checks on each of x86, x64, clang-cl, GCC and headless. Inputs (9,093), external headers and the reconstruction snapshot (1,727) unchanged.
- Accepted: the shared X87Conversions.hpp (EnvironmentCameraBlend/BulletTime now use it) and MusicRegionUpdate (CMusicManager region update 006bdc60).
- Run in a coordinated freeze (fabletlc-bb, 12:16-12:53). Its 8 new native sources were not snapshot inputs; tools/modern was excluded from its writes for the run.

## Correction: 154 music capture boundary labels (behaviour unaffected)
- The real calling conventions:
  - `0044c6b0` takes no stack arguments (plain `ret`; it looks like a definition-manager getter, called with ecx = &ref left over).
  - `GetDef<CSoundThemeDef>` `006b25e0` is `this = manager`, args (&"SOUND_THEME", cell, &ref), `ret 0c`. The atmosphere scout found this: it calls 0044c6b0 with one push and then `GetDefGlobalIndexFromName` 009ad410 (ret 4).
- The accepted music capture modelled 0044c6b0 as `ret 0c` and 006b25e0 as `ret 0`. The net stack is identical and both stubs only record (the ref write lands before the body reads it either way), so the corpus, audit and portable owner are unaffected. The "theme_key" event's arguments are GetDef's arguments.
- The pinned capture.py is not edited. The portable host's `cell_sound_theme(cell, ref)` already describes the combined lookup correctly.
- Lesson (GOTCHAS): derive callee pops from each callee's own `ret`, not from caller stack bookkeeping when two adjacent calls share pushes.

## 155 prepared (not promoted): intelligent pointer + rain atmosphere
- **Rain atmosphere** (native `work/modern_rain_atmosphere_update_20261027/`, 006b2260, CGameScriptInterface_UpdateAtmosphere):
  - The genuine body, the rain getter, LowerBound and the four CBaseIntelligentPointer helpers run in the harness. There are 13 recording boundaries, with pops derived by callee_pops.py.
  - 2,048 cases and 10 controls; audit.py matches.
  - Earlier attempts are kept in attempts/:
    - a 2-byte-short SetPItem span;
    - `entry_key` and `lower_bound_signed` survivors, fixed by clustering keys around 0xff and adding -1/INT_MIN.
- **Intelligent pointer** (native `work/modern_intelligent_pointer_20261027/`): the four genuine helpers run random get/set/set-null/destroy/kill sequences. 2,048 cases, 27,441 operations, 7 controls; audit.py matches.
- **Portable** `work/modern_owned_rain_atmosphere_update_20261027/`:
  - IntelligentPointer.hpp: proxy {item,count} with the item back link, allocator concept.
  - RainAtmosphereUpdate.hpp: a missing 0xff component passes null where retail reads past the entries.
  - strict rain and pointer builds pass on x86/x64/GCC. Mutations: rain 8/8 and pointer 7/7, split by header.
  - Removing SetPItem's whole equal-item early return is an equivalent mutant (release then re-share the same live proxy). The control drops only its null guard.
- **Bundle** `work/modern_owned_rain_bundle_20261027/`: five files; closure 15 reports, 118 pins. promote.py requires the accepted 154 receipt (len 160); prepare_matrix.py uses the `rn_` prefix (len 162).

## Matrix 155 COMPLETE/PASSED (accepted)
- Receipt: `work/modern_owned_rain_bundle_20261027/validation_matrix/summary.json`. 162/162 checks on each of x86, x64, clang-cl, GCC and headless. Inputs (9,181), external headers and the reconstruction snapshot (1,727) unchanged.
- Accepted: IntelligentPointer.hpp (retail CBaseIntelligentPointer) and RainAtmosphereUpdate.hpp (006b2260).
- Native cut between 154 and 155, recorded in `work/modern_owned_rain_bundle_20261027/snapshot_requalification/`: fable_font_data_banks.h (PeekFontInfo), fable_symbol_map.h (UpdateDirtyQuick) and fable_font_vertex_buffer.h (CVertexBufferBase implicit dtor, no novtable). All declaration-only. The validator maps older pins through all four cuts.
- Coordinated freeze with fabletlc-bb (13:01-13:26).

## 156 prepared (not promoted): CQuestManager::Update
- Native `work/modern_quest_manager_update_20261027/` (004b4490):
  - Genuine body plus the hero accessor and GetPItem; 9 recording boundaries (pops via callee_pops.py).
  - Retail layout maps Ego_r names onto compacted STLport containers: +04 PGameScriptThingManager, +38 ActiveQuests, +3c PendingQuestsToDeactivate, +88 PQuestCurrentlyRunning, +90 HeroMapNumber.
  - validation: 2,048 cases, 8 controls. The audit first missed that +88 keeps its prior value when no script runs.
  - final2: the FrameUpdate stub records the published player field at call time, because the portable control `publish_after_update` survived the first corpus. 9 controls, including `publish_skipped`; audit_final2 matches.
- Portable `work/modern_owned_quest_manager_update_20261027/` QuestManagerUpdate.hpp:
  - std::list walks taking the successor after the callbacks, which is faithful here; the hero pointer uses the accepted IntelligentPointer.
  - strict x86/x64/GCC pass on final2; 10/10 controls rejected (first-corpus receipts in attempts/).
- Bundle `work/modern_owned_quest_bundle_20261027/`: three files; closure 8 reports, 85 pins; `qm_` prefix; len 163. No native drift since the 155 cut at preparation time.

## Matrix 156 COMPLETE/PASSED (accepted)
- Receipt: `work/modern_owned_quest_bundle_20261027/validation_matrix/summary.json`. 163/163 checks on each of x86, x64, clang-cl, GCC and headless. Inputs (9,228), external headers and the reconstruction snapshot (1,727) unchanged.
- Accepted: QuestManagerUpdate.hpp (CQuestManager::Update 004b4490).
- Freeze with fabletlc-bb 13:21-13:46; it reported no native changes since 155, so no new cut was needed.

## 157 accepted: CPlayerGui::Update
- **Native** `work/modern_player_gui_update_20261027/` (0043a080, the 013b8790 object, identified as CPlayerGui by its CPlayerManager* at +08 and its 8-byte CCountedPointer draw slots):
  - Genuine body plus GetFrame and GetPItem; 24 recording boundaries (pops via callee_pops.py).
  - 2,048 cases, 10 controls; audit.py matches on its first run. The literal is "PLAYER_GUI_PC".
  - The lazily fetched definition's name string is constructed in the incoming argument's slot (entry esp + 4).
- **Portable** PlayerGuiUpdate.hpp:
  - The unidentified steps and draws are named by retail address/offset.
  - The pause bytes are kept as uint8 (retail compares 1 vs 0x80 exactly). The expiry and budget lists are std::list with no list-touching callbacks.
  - strict x86/x64/GCC pass; 11/11 controls rejected.
- **Bundle** `work/modern_owned_player_gui_bundle_20261027/` (tooling via derive_from_156.py, `pg_` prefix):
  - 157-owned native cut: CConsoleCommandLine.h (private Reset), fable_console.h (ProcessBindKeyInput/Draw declarations), fable_persist_context.h (TransferIn declaration). Declaration-only; five snapshot tests pass.
  - **Matrix 157 PASSED:** `validation_matrix/summary.json`, 164/164 checks on each of x86, x64, clang-cl, GCC and headless; 9,276 inputs unchanged.
- **Announced by native:** dropping __declspec(novtable) from CTBaseSingletonNoDefaultAlloc (fable_system_manager.h), a base of CPlayerGui and CQuestManager. Record it as a cut before the next matrix.

## 158 accepted: UpdateInsideBuildings (006b1f30)
- **Native** `work/modern_inside_buildings_update_20261027/`:
  - Genuine body, GetFrame, the category accessor and _ftol2; 16 recording boundaries (pops via callee_pops.py; the region index push is shared between map vtbl+30 and 004fb490).
  - 2,048 cases, 9 controls. The audit first assumed every definition reference is written; retail only writes the ones it looks up.
  - Behaviour:
    - Every fourth frame the camera cell box (+-3) is searched, and the collector receives &checked and may rewrite it.
    - An inside change drives four sound fades and touches SOUND_THEME 1..N-1.
    - A checked frame whose theme differs, or whose blend is not ordered-equal, applies.
- **Portable** InsideBuildingsUpdate.hpp: strict x86/x64/GCC pass; 11/11 controls rejected.
- **Bundle** `work/modern_owned_inside_buildings_bundle_20261027/` (derive_from_157.py, `ib_` prefix):
  - 158-owned cut: fable_system_manager.h, where CTBaseSingletonNoDefaultAlloc drops novtable (layout unchanged).
  - The first chain run stopped in prepare_matrix (a stale cut anchor; it must target the newest pinned cut). Nothing was built, and the rerun was inside the same freeze.
  - **Matrix 158 PASSED:** 165/165 checks on each of x86, x64, clang-cl, GCC and headless; 9,320 inputs unchanged.

## 159 prepared: day/night sound themes (006b1a20)
- **Native** `work/modern_day_night_themes_update_20261027/`:
  - Genuine body; 11 recording boundaries; the harness tags each string object with its contents.
  - 2,048 cases, 9 controls; audit.py matches on its first run. The fade pair is (GetString(definition name), DAY/NIGHT).
  - Behaviour:
    - Nothing while +09 is set.
    - Outside period 1, daytime cross-fades every SOUND_THEME 1..N-1 to DAY (1.0 over 2000) and NIGHT (0), then sets period 1. Otherwise the night mirror sets period 2.
    - The period changes even with no themes.
- **Portable** DayNightThemesUpdate.hpp: strict x86/x64/GCC pass; 10/10 controls rejected. The first run had a GCC misleading-indentation error in the test and an ill-formed control (unused reference); both are kept in attempts/.
- **Bundle** `work/modern_owned_day_night_bundle_20261027/` (derive_from_158.py, `dn_` prefix): closure 8 reports, 85 pins; no native drift since the 158 cut at preparation.

## Matrix 159 COMPLETE/PASSED (accepted)
- Receipt: `work/modern_owned_day_night_bundle_20261027/validation_matrix/summary.json`. 166/166 checks on each of x86, x64, clang-cl, GCC and headless. Inputs (9,367), external headers and the reconstruction snapshot unchanged.
- Accepted: DayNightThemesUpdate.hpp (006b1a20). The rain atmosphere's two trailing updates (006b1f30, 006b1a20) are now both owned.
- Freeze with fabletlc-bb 14:43-14:54; no native header changes, so no cut. The chain now stops before the matrix loop if promote, prepare or preflight fails.

## Matrix 160 COMPLETE/PASSED (accepted): sound-theme fade 006b1900 / apply 006b1600
- **Native** `work/modern_sound_theme_fade_apply_20261027/`: both genuine bodies; 2,048 cases, 8 controls; audit.py matches.
  - fade: nothing without the sound system ([013b8394]). Otherwise the named definition is held in a cleared slot while vtbl+a0 fades (name, theme, weight, ms), then released at zero.
  - apply: MAIN_SOUND_SETUP's 0x54-byte entry for this+18 is copied to the stack and passed to vtbl+74, then vtbl+78(this+1c, 0) runs and the setup reference is released (retail does not null-check it).
- **Portable** SoundThemeFade.hpp: strict x86/x64/GCC pass; 9/9 controls rejected. The first mutation run crashed the no-sound control on an empty table (uncaught out_of_range); the no-sound default now has a table, so the control is rejected by assertion. Kept in attempts/.
- **Native cut 160** (after the 159 release): fable_generic_var.h gained CGenericVar's five virtual Init overloads. Size and offsets are unchanged, and no modern source includes the header; five snapshot tests pass.
- **Matrix** `work/modern_owned_sound_theme_fade_bundle_20261027/validation_matrix`: 167/167 on x86, x64, clang-cl, GCC and headless; inputs (9,413), external headers and the snapshot unchanged. Freeze 15:05-15:16.

## Matrix 161 COMPLETE/PASSED (accepted): CEnvironment::Update 006b3ff0
- **Native** `work/modern_environment_update_20261027/`: genuine 0x2f6-byte body with 18 boundaries; 1,024 cases, 11 controls; audit.py matches on its first run (105 creates, 77 attaches, 408 kills, 428 first updates, 62 NaN weights). Retail-compacted layout with PDB names: GameTickState +54 (6 x 500-byte CBlendedEnvironmentTheme, Time +c0c), PrevGameTickState +c74, ThemeSet +1954, PRainSplashParticle +1968.
- Behaviour:
  - The tick state is saved and Time stored.
  - Six priority stacks are blended into a local theme, then copied into BlendedTheme[i]: Colours unless ForcedColours; Scalers then 3 texture sets (Textures/Blends CArray assignment) unless ForcedNonColourValues. TimeOfDay is never copied.
  - The rain weight is BlendedTheme[0].Scalers[15] read *after* the blend (0 if !RainEnabled). Above 1e-4f a missing/dead particle is recreated at the camera (scale 1.0) and its +5c component is attached when flags&0x800. At or below, a live particle is killed. NaN does neither (jp on the x87 flags).
  - Three trailing updates run; the first update saves the tick state again and sets PassedFirstUpdate.
- **Portable** EnvironmentUpdate.hpp: strict x86/x64/GCC pass; 13/13 controls rejected.
  - Test fixes on the way: a flags typo, and a replay header width of 18 where the export writes 16.
  - The `first_update_repeats` control was ill-formed: removing the flag write let MSVC prove the defaults always fail, and C4702 unreachable code under /WX stopped it compiling. It was replaced by `first_update_not_saved` and `first_update_flag_from_rain`.
  - Float Time round-trips its bits on x86, sNaN included.
- **Native cut 161** (after the 160 release): CCodeSectionManager.h, fable_resource_list.h, fable_surface_types.h and fable_system_manager.h. All are declaration-only (CDebugInfo, member declarations, SetMaximumMemory long->unsigned long, CProfileTreeEntry class->struct).
- **Two preflight failures, no builds run:**
  - Twenty older native-receipt pin loops compared raw digests, and fable_resource_list.h (pinned by the 2026-10-19 resource lanes) is in the cut. The 161 generator now maps those loops through the cut chain.
  - Two rebuild/src files replaced in place (CResource_SetAsUnloadable_009fbf50, CResourceList_SetMaximumMemory_009fc520) are pinned only as provenance by the retail-emulating resource lanes. They are recorded in `source_pin_cut/` (before copies from the 160 archive; both behaviour-preserving: real CResourceList offsets +2c/+34 = the stand-in's, and an identical body). The cut is chained and its review pinned.
  - The chain also failed to stop on the first failure because `preflight | tail` masked its status; later chains use pipefail.
- **Matrix** `work/modern_owned_environment_bundle_20261027/validation_matrix`: 168/168 on x86, x64, clang-cl, GCC and headless; inputs (9,471), external headers and the snapshot unchanged.

## Matrix 162 COMPLETE/PASSED (accepted): CEnvironment::UpdateWind 006b3030
- **Native** `work/modern_update_wind_20261027/`, under FPCW 0x027f:
  - The genuine first loop of 00a0db60 builds the 128-word fast inverse-square-root seed table at 0x13ce558. Its later cosine loop uses fcos and is not run. The genuine UpdateWind body follows.
  - Boundaries: GFFloatRandom (the stub loads an unrounded double onto st0) and the fpatan-based rotate 00a14260.
  - 2,048 cases, 11 controls. One control was equivalent: 0x1f80 -> 0x1f00 only moves an exponent bit, which the table drops. It was replaced by 0x1fc0.
  - audit.py (Python binary64 = PC53 here) matches the table and every case on its first run (576 low clamps, 552 high, 687 override skips).
- **Retail quirk:** the seed table keeps mantissa bits only, and 1/sqrt(1.0) is exactly 1.0, one binade up. So entry 64 is 0, and squared lengths in [1, 1+1/64)*4^k get seed 0.5 and renormalise to length 0.6875, exact unit vectors included. The port keeps it, and a test default pins it.
- **Portable** WindUpdate.hpp:
  - It builds the table with the engine's own loop (IEEE sqrt + pc53::divide) and models retail's mixed roundings with Pc53Arithmetic: factor unrounded, dx*factor rounded, dy unrounded, dz rounded before the multiply. All ten constants equal their float literals.
  - Domain: finite state with squared lengths inside binary32.
  - Strict x86/x64/GCC pass; 11/11 controls rejected. Test fixes: header width 17, a wrong unit-length default, and the override default's host had no randoms (a crash, not an assertion; the recorded gotcha, repeated).
- **Matrix** `work/modern_owned_wind_bundle_20261027/validation_matrix`: 169/169 on x86, x64, clang-cl, GCC and headless; inputs 9,520 unchanged.
  - No cuts needed.
  - Promote, prepare and preflight ran before the freeze, so the freeze covered only the builds (16:41-16:49).

## Matrix 163 COMPLETE/PASSED (accepted): CEnvironment lightning update 006b3b80
- **Native** `work/modern_lightning_update_20261027/` (FPCW 0x027f): genuine 0x2de-byte body, 15 boundaries, function-static previous time (013ba8e0/guard 013ba8e8) as real memory. 2,048 cases, 12 controls.
  - Capture fixes on the way:
    - a sparse first corpus (68 starts) got a flash-start scenario every third case;
    - the 0x75->0x76 key control needed a key-0x76 element;
    - the priority-stack elements' leading pointers were uninitialised; the independent audit caught the 0xa5 call target.
    - A heredoc patch broke on `\xa5` again, and its follow-up rerun was stopped and archived.
  - audit.py matches every case (734 starts, 95 thunder plays, 279 ends, 316 inactive).
- Behaviour:
  - Runs while Scalers[17] > 0 or a flash runs.
  - Start: rain > threshold, idle, and draw(chance) < 1. The chance gains Scalers[17], the theme weight is set (4, theme, fade-in), and start = now. With sound, a live player flagged 0x200000 and no live component 0x75, a CS_LIGHTNING_THUNDER emitter is created at the player + (r20-10, r20-10, 30) (z up) and played with default params flagged 0x8080.
  - End: elapsed > duration -> unless forced, six TransitionToDefaultTheme(fade-out) calls, then start = -1.
  - Else the chance decays by now - previous, floored at 0. previous = now on every active exit.
  - When component 0x75 is absent, retail reads end->second, one element past the array. It stays a host boundary in the port.
- **Portable** LightningUpdate.hpp, with an explicit LightningClock for the function-static and Pc53Arithmetic:
  - strict x86/x64/GCC pass first time; 12/12 controls rejected;
  - `player_flag_ignored` was ill-formed (an unused constexpr under /WX) and was replaced by `|`.
- **Matrix** `work/modern_owned_lightning_bundle_20261027/validation_matrix`: 170/170 on x86, x64, clang-cl, GCC and headless; inputs 9,569. No cuts. Freeze 17:16-17:22, after preflight.
- **Open:** 006b2ca0 has 13 inline fsin/fcos. Unicorn's soft-float transcendentals are not hardware x87, so an evidence policy is needed before porting (an in-game capture, or host-boundary trig).

## Matrix 164 COMPLETE/PASSED (accepted): environment theme priority-stack transitions
- **Native** `work/modern_theme_priority_stack_20261027/` (FPCW 0x027f), genuine:
  - StartThemeTransition 008861d0, TransitionToDefaultTheme 00886400, entry ProcessTransitions 00887a30, stack ProcessTransitions 00887b00, SetThemeWeightAllChannels 006b38d0, GetDefClassIndexFromGlobalIndex 009acca0.
  - Boundaries: GFGetTime, current weight 00886160, finish 00887330/00887380, SetThemeWeight 00886d70, RebalanceWeights 00886200.
  - Retail-compacted 48-byte CEnvironmentThemePriorityStackEntry. ThemeSet elements are 12 bytes (span/12).
- Corpus history:
  - Setup fixes: overlapping regions, and a StartThemeTransition span 2 bytes short.
  - The first portable mutation run left `finish_target_sign` (>=0 vs >0) alive: no finished target-0 case. The corpus was regenerated with target 0 (52 witnesses).
  - Final: 2,048 cases, 13 controls; audit.py with x87 special operands matches every case (98 cases with NaN weights).
- Behaviour:
  - Zero-length fades are real inputs: 0/0 gives the indefinite NaN (0xffc00000 stored), x/0 gives ±inf, and the clamps drop -inf to 0 and +inf to 1 but keep NaN.
  - The stack reset writes entry 0 even for an empty stack (retail writes through begin).
- **Portable** ThemePriorityStack.hpp: local x87 special-operand add/subtract/multiply/divide/narrow over the accepted Pc53Arithmetic.
  - The replay compares decoded entry fields (bools by truthiness).
  - Strict x86/x64/GCC pass; 14/14 controls rejected. Test fixes: per-channel field offsets modulo 0x400, and the NaN count per case.
- **Matrix** `work/modern_owned_theme_priority_bundle_20261027/validation_matrix`: 171/171 on x86, x64, clang-cl, GCC and headless; inputs 9,620. No cuts. Freeze 17:50-17:58, after preflight.

## Matrix 165 COMPLETE/PASSED (accepted): environment theme-weight list operations
- **Native** `work/modern_theme_weights_20261027/` (FPCW 0x027f), genuine:
  - GetThemeWeight 00886160, SetThemeWeight 00886d70, RebalanceWeights 00886200, finish-to-target 00887330, RestoreDefaultTheme 00887380.
  - The CArray<CThemeWeight> resize 00889440 and the reallocating insert 00889010 are emulated on a scratch heap. A float driver stores st0 after GetThemeWeight.
  - Setup fixes: the driver was on an unmapped page, the get span was 2 bytes short, and `rebalance_unlocked_zero` was equivalent (a pointer reads as a tiny float and is erased like 0); it was replaced by deleting the store.
  - 2,048 cases, 13 controls. audit.py (the 164 x87 helpers) matches every case on its first run; 137 final lists hold NaN weights.
- Behaviour:
  - Clamp [0,1] keeps NaN. A new id is appended only above 2^-10, and retail copies uninitialised stack padding into the lock word.
  - Rebalance: a float running sum from BaseWeight; the locked total is unrounded. Above 0.9990234375 locked weights normalise and unlocked ones zero. Otherwise unlocked ones scale by (1-locked)/(all-locked). Base = 1, then the erase/unlock/subtract pass, then BaseWeightLocked = 0.
- **Portable** ThemeWeights.hpp: std::vector<ThemeWeight> beside the accepted ThemePriorityEntry, no host. Strict x86/x64/GCC pass first time; 15/15 controls rejected (the rebalance-flag control negates rather than deletes, to stay well formed).
- **Native cut 165:** fable_input_event.h gained three CInputEvent const getter declarations; no modern includes. The native session also added 7 00/a0 sources and replaced CWStringParser_Init_00a0c8e0 in place; no accepted receipt pins it.
- **Matrix** `work/modern_owned_theme_weights_bundle_20261027/validation_matrix`: 172/172 on x86, x64, clang-cl, GCC and headless; inputs 9,666. Freeze 18:13-18:22.

## Matrix 166 COMPLETE/PASSED (accepted): CEnvironmentThemePriorityStack::CalculateTheme 008889c0
- **Native** `work/modern_calculate_theme_20261027/` (FPCW 0x027f): genuine 0x96-byte body; GetLevelTheme 008884d0 and BlendInLevel 008886e0 are boundaries. 2,048 cases, 5 controls; audit.py matches on its first run (2,480 blends, 1,472 skipped).
- Behaviour:
  - Entry 0 always provides the level theme, even for an empty stack (retail calls through begin).
  - Each later entry blends in place (out passed as both source and destination) when 1 - BaseWeight > 2^-10. NaN never blends.
- **Portable** CalculateTheme.hpp, on the accepted ThemePriorityStack/ThemeWeights headers: strict x86/x64/GCC pass first time; 5/5 controls rejected.
- **Matrix** `work/modern_owned_calculate_theme_bundle_20261027/validation_matrix`: 173/173 on x86, x64, clang-cl, GCC and headless; inputs 9,708. No new cut: the 165 cut is the newest, and 166 is the first bundle chained after it. Freeze 18:33-18:40.

## Matrix 167 COMPLETE/PASSED (accepted): GetBlendedTheme 00888260
- **Native** `work/modern_get_blended_theme_20261027/` (FPCW 0x027f): genuine 0x153-byte body; CBlendedEnvironmentTheme::Blend 00887b70 is the boundary. 2,048 cases, 7 controls, first run. audit.py (the 164 x87 helpers) matches on its first run: 1,117 day wraps, 880 negative fractions, 201 inf/NaN fractions.
- Behaviour:
  - t = f(time*24). later = the first theme with TimeOfDay (hours) > t, wrapping to 0; earlier = (n + later - 1) % n (unsigned).
  - Keys are scaled by f(1/24) unrounded; if earlier > later, later += 1 day. fraction = f((time - earlier)/(later - earlier)), unclamped.
  - Retail quirks: negative before the first key, a single theme divides by zero, and an empty array would trap on idiv.
- **Portable** GetBlendedTheme.hpp, with the ThemePriorityStack x87 helpers: strict x86/x64/GCC pass first time; 8/8 controls rejected.
- **Matrix** `work/modern_owned_get_blended_theme_bundle_20261027/validation_matrix`: 174/174 on x86, x64, clang-cl, GCC and headless; inputs 9,753. No cut. Freeze 18:51-19:00.

## Matrix 168 COMPLETE/PASSED (accepted): CBlendedEnvironmentTheme::Blend 00887b70
- **Native** `work/modern_theme_blend_20261027/` (FPCW 0x027f): genuine 0x6e9-byte body (span fixed from 0x6e7: ret 0c at 0x888256); BlendTextureSets 00887830 is the boundary. 2,048 cases, 6 controls.
- **Rounding:** fit_rounding.py tested the four store patterns per word, and every one of the 107 words fits exactly one, consistent with the disassembly:
  - TimeOfDay, R and all 30 Scalers: unrounded;
  - G: the earlier term stored to float first;
  - B and A: both terms stored.
  - audit.py hard-codes that rule (asserting it equals the fit) and matches every case; 143 cases have inf/NaN outputs because the fraction is unclamped.
- **Portable** ThemeBlend.hpp: BlendedThemeValues (TimeOfDay, Colours[19][4], Scalers[30]) with per-channel store flags over the 164 x87 helpers; texture sets via the host. Strict x86/x64/GCC pass first time; 8/8 controls rejected.
- **Matrix** `work/modern_owned_theme_blend_bundle_20261027/validation_matrix`: 175/175 on x86, x64, clang-cl, GCC and headless; inputs 9,798. No cut; the native session added only a new 00/a1 STL source. Freeze 19:18-19:26.
- **Day total:** bundles 152-168 accepted (17), all with native freezes coordinated with fabletlc-bb.

## Matrix 169 COMPLETE/PASSED (accepted): CBlendedEnvironmentTheme::BlendTextureSets 00887830
- **Native** `work/modern_blend_texture_sets_20261027/` (FPCW 0x027f): genuine 0x1f2-byte body.
  - The CArray<long>/<float> resizes 004428f0/00442770 are emulated; CompactTextureSet 00886e80 is a boundary.
  - 2,048 cases, 5 controls. Two first-pass controls were equivalent: the unroll threshold 4->5 only routes n=4 through an identical tail, and `push eax` held the same n as `push ebp`. They were replaced by the tail weight and `push ebx`.
  - audit.py matches every case on its first run.
- Behaviour:
  - n = the earlier set's texture count. out.Textures/Blends are resized to 2n: the earlier textures, then the later.
  - Blends[i] = f((1-f)*earlier) with w unrounded; Blends[n+i] = f(f*later). Then CompactTextureSet(out, n).
  - Retail quirk: later is read for i < n without a size check (port precondition: later at least as large).
- **Portable** TextureSetBlend.hpp (EnvironmentTextureSet {vector<int32> textures; vector<float> blends}): strict x86/x64/GCC pass first time; 6/6 controls rejected.
  - `earlier_full_weight` was ill-formed (unused `weight` under /WX, again) and was replaced by applying the weight to later.
  - An out-of-bounds-writing size control was avoided in favour of an oversized blends resize.
- **Matrix** `work/modern_owned_blend_texture_sets_bundle_20261027/validation_matrix`: 176/176 on x86, x64, clang-cl, GCC and headless; inputs 9,841. No cut. Freeze 19:41-19:49.
- **Day total:** 152-169 accepted (18 bundles).

## Matrix 170 COMPLETE/PASSED (accepted): CBlendedEnvironmentTheme::CompactTextureSet 00886e80
- **Native** `work/modern_compact_texture_set_20261027/` (FPCW 0x027f): genuine 0x48e-byte body; the grow helpers and memmove are unreachable in the domain (asserted).
  - 2,048 cases, 6 controls. The first portable mutation run left `uniform_inclusive` alive: no kept sum was exactly 1/256, and equal kept weights make uniform == normalised. It was fixed by regenerating with four 3/1024 + 1/1024 witnesses.
  - audit.py matches every case (1,281 merges, 209 uniform, 554 empty).
- Behaviour:
  - Merge equal handles into the first occurrence (blend[i] = f(b[j] + b[i]), b[j] = 0).
  - Bubble-sort both arrays by blend descending (never swapping NaN).
  - Subtract blend[n] from the n kept blends; sum them unrounded.
  - Below 1/256 every kept blend becomes f(1/n), else f(b/sum). Truncate to n.
  - Retail reads blend[n] unchecked (domain n < size or n = 0; BlendTextureSets always passes size = 2n).
- **Portable** CompactTextureSet.hpp on EnvironmentTextureSet: strict x86/x64/GCC pass; 8/8 controls rejected. A wrong hand default (0.6/0.4, not 0.5/0.875) was caught before the run.
- **Native cut 170:** the native session's header consolidation, 11 files:
  - 5 new headers (containment volume, nav quad-tree node, navigator, physics mesh, steering behaviour);
  - CContainmentVolume.h now forwards to the hand-owned header;
  - virtual declarations on already-polymorphic CBaseObject/CClass/CClassFactory; CRenderTarget/C3DCoordI declarations.
  - It also carries one modernization change: tools/modern/modern_thing_checksum.py FACTORY_BEFORE now matches the new CClassFactory text. FACTORY_AFTER is unchanged (same layout, no factory vtable in the build copy). The cut records source_modified_by_modernization=True.
  - The 2026-10-18 staged-creation receipt pins fable_class.h / fable_thing_manager.h raw among its `originals`; the 170 generator maps that check through the cut chain.
- **Matrix** `work/modern_owned_compact_texture_set_bundle_20261027/validation_matrix`: 177/177 on x86, x64, clang-cl, GCC and headless; inputs 9,908. Freeze 20:13-20:20.
- **Day total:** 152-170 accepted (19 bundles).

## Matrix 171 COMPLETE/PASSED (accepted): CBlendedEnvironmentTheme::SetAsWeightedBlendOfThemes 008873c0
- **Native** `work/modern_weighted_theme_blend_20261027/` (FPCW 0x027f): genuine 0x46b-byte body including its inline STLport push_back growth; new/delete/memmove are an emulated heap and CompactTextureSet is a recorded boundary. 2,048 cases, 5 controls, first run.
- **Audit:** audit.py's fit_check confirms each value word has exactly the one store pattern read from the disassembly (R and Scalers unrounded, G/B/A products stored), then matches every case (1,537 blended).
- Behaviour:
  - Nothing when weights or themes are empty.
  - Colours and Scalers are reset, then each accumulates f(product + value) theme by theme. TimeOfDay is untouched.
  - Each texture set becomes all themes' textures with f(blend * weight), compacted to the first theme's count for that set.
  - Retail reads weights[k] unchecked (port precondition: weights cover themes).
- **Portable** WeightedThemeBlend.hpp (WeightedThemeSource = BlendedThemeValues + 3 EnvironmentTextureSets): strict x86/x64/GCC pass first time; 8/8 controls rejected. An unsafe control that read weights[0] of an empty vector was replaced before the run.
- **Matrix** `work/modern_owned_weighted_theme_blend_bundle_20261027/validation_matrix`: 178/178 on x86, x64, clang-cl, GCC and headless; inputs 9,953. No cut. Freeze 20:37-20:44, released.
- **Session end (user call, 20:45).** 152-171 accepted today: 20 bundles, every one with a native freeze coordinated with fabletlc-bb and released. Nothing committed or pushed.
