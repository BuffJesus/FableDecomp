# Modernization journal, 2026-10-29

Owner: the modern lane. Continues `2026-10-28-modernization.md`. The user's standing instruction is to continue
the 64-bit/modernization work until told to stop.

## 172 level theme: native cut, then matrix
- The resumed preflight (`preflight_pre_resume_20261029/` holds the 2026-10-28 pass) failed on native drift:
  `engine/CSB.h` changed while it ran. The failed output is kept as
  `work/modern_owned_level_theme_bundle_20261028/preflight_failed_drift_csb_20261029/`.
- The native session `fabletlc-3e` landed two header batches: the wf-a2 steering family, then the mesh-data-bank
  family. To avoid two cuts, it was asked to signal after the second batch, and it then froze rebuild/include,
  rebuild/src and the tests.
- **172 cut** (`snapshot_requalification/`): 21 changed inputs (1,732 -> 1,738). The review is in `review_notes.json`.
  No rebuild/modern or tools/modern file references any changed header or class.
  `qualify_source_cut.py` waits on the runtime lease, which the external Fable2Native session holds intermittently.

## 006b2ca0 x87 trig evidence policy (decided)
- 006b2ca0 is CEnvironment's sun/moon/diffuse vector update (the forge autoname `CGameCamera_UpdateViewMatrix` is wrong,
  and it was already reverted).
  - Reads +3d ForcedNonColourValues, +40 DiffuseClampAngle, +c0c Time, and BlendedTheme[0] Scalers 5 and 8-13 (+19c, +1a8..+1bc).
  - Writes Sun/Moon/DiffuseVector at +c30/+c3c/+c48.
  - 13 inline fsin/fcos and an fsqrt, plus 4 calls to CVector::Normalise 00a14440.
- **Policy:** Unicorn's fsin/fcos are not the x87's 64-bit-significand transcendentals, so they are not evidence. The
  oracle instead runs the **genuine retail bytes natively on this CPU's x87**:
  - `work/modern_sun_moon_20261029/hw_harness.cpp` is built with VC7.1 as a 32-bit exe.
  - The retail spans sit at their original addresses inside a 15 MB .bss of the harness image. The CRT heap reserves
    0x630000+, so VirtualAlloc at 0x6b2000 failed.
  - FPCW is 0x027f, with fnclex per case and fnstenv afterwards (stack empty, CW unchanged).
- fsin/fcos results are a property of the processor (x87 units may differ in final bits). The owner therefore takes
  them as a host boundary returning the 80-bit register image. The harness records, on the same hardware, the PC53
  operand and the fsin/fcos register values for every angle, so the replay host serves recorded hardware values.
- **Capture** (`capture.json`): 10,240 cases. That is 8,192 random cases plus a 2,048-case cancellation family that
  witnesses the extended precision (first tilt 0, second angle near Time, clamp near a quarter turn).
  - Run B repeats every case over a different random background: equal outputs, so nothing else is read.
  - Writes land only in +c30..+c54; forced cases write nothing; the trig table repeats exactly.
- **Controls** (`controls.json`): 12/12 patched retail bytes or constants change hardware outputs. They cover the fcos/fsin
  swaps, operand/scaler swaps, the removed Normalise call, the inverted clamp branch, fdivp->fdivrp, fsqrt->fabs,
  and the 0.0f and 2pi constants.

## 173 candidate: SunMoonUpdate owner (private qualification in progress)
- Lane `work/modern_owned_sun_moon_20261029/`. Staged headers:
  - `X87Extended.hpp`: 80-bit register image; round53, direct binary32 store, PC53 ext*ext and ext+ext, ordered less;
  - `SunMoonUpdate.hpp`: `update_sun_moon(state, host)`, with host `cosine/sine(double operand) -> x87::Extended`.
- **Domain** (input-only predicate, in export.py):
  - all inputs finite, and the six angle inputs at most 2^60 in magnitude, so every trig operand is below 2^63;
  - Scalers[10]/[13] at most 2^64, and the weight at most 2^20.
  - Outside it, retail produces infinities or x87-indefinite NaNs (an overflowing weight, or an out-of-range fsin
    returning its operand so that 1 - s^2 < 0). The owner does not model those.
- Export: 5,819 cases (411 forced, 5,408 computed); 4,421 out-of-domain cases are counted and excluded.
- The replay test checks:
  - the exact 13-call fcos/fsin order and every PC53 operand;
  - all 9 output words bit for bit;
  - that inputs are untouched and forced cases leave the outputs untouched.
- The first owner draft matched every finite-output hardware case on its first run. Strict gcc
  (-Wconversion -Wpedantic -Werror) passes 5,819/5,819.
- **Mutations v1** (`mutations_v1_survivors/`): 15/20 killed. The five survivors:
  - `y_double_rounded` is an **equivalent** mutant: r53(+-0 + r53(s)) == r53(+-0 + s). It was dropped.
  - `sin_first/second_rounded_to_53`, `narrow_double_rounds` and `normalise_left_associated` reach the binary32
    outputs with probability about 2^-29 per case. No practical corpus witnesses them, so they got crafted witnesses
    in the defaults:
    - a scripted host returning the sine 0xaaaaab5555555ab0 x 2^-64 with fcos(day) stored as 0.75. The product stores
      as 0x3f000001; rounding the sine to 53 bits first gives 0.5. Found by exact-rational search; it kills both sine mutants.
    - the register 0x8000017ffffffffe/0x3fff, just below a binary32 tie: a direct store gives 0x3f800001, via 53 bits 0x3f800002.
    - the Normalise vector 0x3f09264d/0x3f2e7389/0x325111ce, found by a focused numpy search: retail's x*x + (y*y + z*z)
      stores z = 0x3271305a, while the left association stores 0x3271305b.
- **Independent review** (one general-purpose refuter, given only the code): no SunMoonUpdate discrepancy. It verified
  the scalers, trig order, precision per operand, the old/new slots after `pop edi`, the strict-ordered clamp branch
  and the Normalise order.
  - It found a real bug in `x87::add`: a same-sign sum of two 64-bit significands lost its carry out of 128 bits.
    The owner's outputs were unaffected, since the only same-sign use is multiplied by 0.0f and keeps its sign.
  - Fixed: the carry is shifted back in, the lost bit becomes sticky and the exponent goes up by one. Covered by
    defaults (1+1, -0.5-0.75, (2-2^-63)*2, a carry with sticky) and a new `add_carry_exponent_lost` mutant.
- Pending: mutations v2 and strict x86/x64/gcc (both after the 172 matrix), then the bundle.

## 172 matrix
- First cut that **removes** a snapshot input: older receipts still pin `engine/_quarantine/C3DMeshLODInfo.h`, and the
  chain maps its hash to None, but the validators called digest() on the missing file. The first preflight with the cut
  failed on this; it is kept as `preflight_failed_removed_input_20261029/`.
- Fix in `prepare_matrix.py` (cut block): `digest` returns None only for paths listed under the 172 cut's `removed`
  when they are absent, and those paths leave `sources` before the snapshot is hashed. Any other missing file still fails.
  The superseded generated scripts are kept under `attempts/pre_cut_20261029/` and `attempts/cut_no_removal_20261029/`.
- Preflight passed: 10,032 inputs, 1,738 snapshot inputs. The full matrix is running under the native freeze.
- **172 ACCEPTED**: `work/modern_owned_level_theme_bundle_20261028/validation_matrix` passed. Each of x86/x64/clang/gcc/headless
  ran 179 checks, all with return code 0; inputs, external headers and the 1,738 snapshot inputs are unchanged.
  "native unfrozen" was sent to fabletlc-3e afterwards.
- The newest pinned native cut is now **172's** (`work/modern_owned_level_theme_bundle_20261028/snapshot_requalification`),
  after the 170 (CX) cut. Its validators carry the `removed_inputs` digest rule, which later bundles inherit by deriving
  from the 172 validator.

## 173 sun/moon update ACCEPTED
- Private qualification:
  - mutations_v2: 20/20 killed;
  - strict_x86/x64/gcc pass with the exact marker;
  - export 5,819 cases;
  - closure: 8 reports, 136 pins. The hardware lane is admitted through capture.json's own file hashes, the harness
    exe/source/capture hashes, controls.json (12/12) and disassembly.txt.
- Bundle `work/modern_owned_sun_moon_bundle_20261029/` (prefix `sm_` after `lv_`) is a four-file promotion:
  X87Extended.hpp, SunMoonUpdate.hpp, SunMoonUpdateTests.cpp and CMake.
- **173 cut:** the native WAD/zlib, lip-sync and suspendable-process header batches; 10 changes, 1,738 -> 1,741 inputs,
  nothing removed, taken under a native freeze. The generator unions the removed-input lists of the 172 and 173 cuts.
- Preflight passed with 10,103 inputs. **The full matrix PASSED** (`validation_matrix`): x86/x64/clang/gcc/headless each
  ran 180 checks, all with return code 0. "native unfrozen" was sent.
- Every CEnvironment::Update callee is now owned except the composition of the 161-173 owners into one CEnvironment owner.

## 174 candidate (in progress): composed theme calculation
- First CEnvironment composition slice: the theme pipeline end to end. Call-tree inventory of CEnvironment::Update
  006b3ff0 (capstone walk, depth 4): the theme subtree (ProcessTransitions, CalculateTheme and everything under it) is
  all owned. The real externals are the spell-effect interpolator (006b8640 tree), rain particle things (006e0880,
  006e0db0, 004c9b80 trees), the camera virtual, GFGetTime (indirect via 0x143fe04) and the allocator.
- Native lane `work/modern_theme_calculation_20261029/` (being built by a delegated agent): CalculateTheme 008889c0 with
  the WHOLE genuine callee tree in Unicorn (only the MSVCR71 allocator/memmove thunks emulated), plus 10 retail-byte controls.
- Portable lane `work/modern_owned_theme_calculation_20261029/`: staged `ThemeCalculation.hpp`.
  - Theme types: `EnvironmentTheme` (= WeightedThemeSource), `EnvironmentThemeSet` (per-id keyed themes) and
    `EnvironmentPriorityStack` (contiguous entries plus parallel Themes arrays).
  - One `Pipeline` host binds CalculateTheme -> GetLevelTheme/BlendInLevel -> GetBlendedTheme -> Blend -> BlendTextureSets
    and SetAsWeightedBlendOfThemes -> CompactTextureSet to the owned functions; no theme boundary remains.
  - Strict gcc compiles it, and a smoke check passes (keyed 0h/12h themes at 6h give time of day 6).
- **Composed native lane done** (the delegated agent): `work/modern_theme_calculation_20261029/validation/`.
  - 2,048 cases, 40 genuine spans (the whole tree: theme functions, theme object helpers, vector<theme>/<long>/<float>
    STLport helpers); only new/delete/memmove are emulated. 10/10 retail-byte controls change outputs.
  - The generator drops cases outside CompactTextureSet's domain (n == 0 or n < size).
  - Findings:
    - GetBlendedTheme adds a day only when earlier > later strictly; equal keys divide by zero, matching the owner's
      code, though its comment said "not before" (see below).
    - BlendInLevel's copy path cannot be reached through CalculateTheme.
    - CompactTextureSet reaches the long fill_insert helper even with n == 0.
- Export: 1,984 cases (the 64 excluded have non-finite native output, all special-key). **The composition replay passed
  every case on its first run.**
- Mutations: 11/11 killed (`mutations/`). The v1 run (`mutations_v1_equivalent/`) had two survivors, both equivalent
  inside CalculateTheme: out passed as BlendInLevel's source (CalculateTheme always passes out as both), and a consistent
  texture-set index reversal (it only reorders three independent blends). Both were dropped, and a destination-only shift
  replaced the second.
- **174 v1 failed preflight:** it also promoted a comment-only GetBlendedTheme.hpp correction. Older closures (the 168
  Blend lane's) pin that production file through expected-hash chains that end at their own bundle, so editing an
  older owner breaks them.
  - Production was restored byte-exact from the promotion preimage, and the v1 artifacts are kept under
    `attempts/v1_with_getblendedtheme/` in both lanes.
  - The correction stays open: the GetBlendedTheme comment still says "not before".
- **174 ACCEPTED** (`work/modern_owned_theme_calculation_bundle_20261029/validation_matrix`): strict x86/x64/gcc pass;
  closure has 7 reports and 100 pins; preflight had 10,148 inputs; x86/x64/clang/gcc/headless each ran 181 checks,
  all with return code 0. No native cut was needed (native confirmed no snapshot-input change since 173).

## 175 composed priority-stack transitions ACCEPTED
- Native lane `work/modern_priority_transitions_20261029/` (delegated agent): ProcessTransitions 00887b00 with its whole
  tree genuine (16 spans). GFGetTime 009e1bc0 also runs genuine, with its QueryPerformanceCounter/Frequency and timeGetTime
  imports recorded. 2,048 cases, 9/9 retail-byte controls. Current weight 00886160 is not in this tree; only
  StartThemeTransition reaches it.
- Owner `PriorityStackTransitions.hpp`: a `Weights` host binds ThemePriorityStack's boundaries to the ThemeWeights
  functions on the entry's own Themes array. Game time is the one external (`GameClock`).
- **Finding:** NaN weights. Retail propagates a NaN operand's own payload and sign (0x7fc00000 stays positive), while the
  accepted ThemeWeights/ThemePriorityStack helpers return the x87 indefinite (0xffc00000) for any non-finite operand.
  The per-function corpora behind 164/165 held no NaN weights.
  - Accepted owner files are not edited (see 174), so the composition states the domain instead: no NaN among the weights
    and durations. 107 cases were excluded, and the remaining 1,941 match bit for bit.
  - Open: a later dedicated lane could add retail NaN propagation to those helpers, after extending the generators'
    closure chain mapping.
- My two hand-written defaults were wrong at first: RebalanceWeights unlocks the base and every theme. They were fixed
  from the owner's code and the retail behaviour, not by copying the output. A current-weight default witnesses that
  binding, since the capture tree never reaches it.
- Mutations: 9/9 killed. Strict x86/x64/gcc pass. **175 cut:** the allocator_font_state batch; 4 changes,
  1,741 -> 1,742 inputs, nothing removed.
- Matrix PASSED: x86/x64/clang/gcc/headless each ran 182 checks, all with return code 0 (`validation_matrix`).

## 176 candidate (in progress): CEnvironment composition
- **Whole-tree native capture** `work/modern_environment_tree_20261029/` (delegated agent): CEnvironment::Update 006b3ff0
  with 66 genuine spans. These are the tick-state operator=, the whole ProcessTransitions and CalculateTheme trees,
  006b2ca0 + Normalise, UpdateWind, lightning and lightning's environment callees (SetThemeWeightAllChannels 006b38d0,
  StartThemeTransition, GetThemeWeight, TransitionToDefaultTheme), plus the wind seed-table loop as an init span.
- Externals, all recorded:
  - spell-effect FrameUpdate, the CIntelligentPointer functions, CThing::Create/AttachToCamera/Kill, the camera virtual,
    GFFloatRandom;
  - rotation 00a14260 (fpatan/fsin/fcos and a CRT call);
  - definition lookup 009acca0;
  - lightning's world/sound/thing/string calls.
- **Hardware trig injected into Unicorn** (two-pass):
  - pass 1 records the fsin/fcos operands at the 13 sites;
  - `work/modern_sun_moon_20261029/hw_harness.exe` computes them on the real x87 (5,705 angles, repeatable);
  - pass 2 writes the 80-bit results into ST(0) and skips the instruction.
  - All 10,595 injected operands matched the hardware operands bit for bit. A self-test confirmed that FP0+TOP addresses
    ST(0) and that setting EIP skips the instruction.
  - Unicorn's own trig differed in 10,591 80-bit results but in no float output on this corpus.
- 1,024 cases, 10/10 retail-byte controls. Stacks are always 5 entries, because lightning addresses priority 4 in every channel.
- Portable composite (`work/modern_owned_environment_20261029/`): a delegated agent is writing it against the frozen
  accepted owners, under an input-only domain rule.
- **Portable composite** (delegated agent, reviewed here): `EnvironmentComposition.hpp`, a thin binding layer over the
  frozen accepted owners.
  - Per channel: ProcessTransitions -> process_environment_transitions, and CalculateTheme -> calculate_environment_theme.
  - Tick copies and operator= are value copies; the disassembly shows they copy every field.
  - The trailing updates go to update_sun_moon, update_wind and update_lightning. Lightning's channel-wide calls go through
    set_theme_weight_all_channels / transition_to_default_theme on entry 4, via a host that finds each entry's channel.
  - The rain particle goes through its intelligent pointer and is re-read for every use, as retail does.
  - All other subsystems are `EnvironmentExternals`: spell effects, thing fields, the camera, three random generators,
    the rotation, definitions, player/sound, GFGetTime's result and fsin/fcos.
  - The test checks every external call's arguments in order. Replay: 1,024/1,024 with **no exclusions**.
- Mutations: 13/13 wiring mutants killed. Strict x86/x64/gcc pass. Closure: 7 reports, 107 pins.
- **176 cut:** the nav_quad_tree_nodes batch; 7 changes, 1,742 -> 1,743 inputs. The 22 removed stand-in sources are not
  snapshot inputs.
- **176 ACCEPTED** (`work/modern_owned_environment_bundle_20261029/validation_matrix`): x86/x64/clang/gcc/headless each ran
  183 checks, all with return code 0. CEnvironment::Update now runs end to end through owned code.

## 177 selection: screen-effect interpolator (after 176)
- Native cut pending for 177: fabletlc-3e changed `rebuild/include/fable_small_vector.h` (CSmallVector::Size
  unsigned char -> signed char) and removed the stand-in CNavNavigableLeafNode_GetMemoryUsage_00a76500.cpp. No
  rebuild/modern or tools/modern file names CSmallVector or the header (searched); it reaches the snapshot only as a hashed input.
- WorldFrame callee survey: engine pump (640320/640360), particle sweep (aeb4a0), thing world update (51fcc0), deaths
  (51f070), conversations (6e60f0), bullet time, music, messages, script timers and CEnvironment::Update (6b3ff0) are owned.
  Not owned: update_environment 006b32a0 (= `[world+1960]` CEnvironmentSpellEffectInterface::FrameUpdate 00885010, which
  copies the current effect blocks +4..+84 to the previous blocks +84..+104), combat 006ed200 (446+204 B callees), the
  atmosphere theme 006b1d30 (507 B), free camera 004884d0, the player update 00488ab0 and the save/region loaders.
- **177 candidate:** the spell-effect interpolator 006b8640, which 176 leaves as an external. It runs 00880e50 Process
  (three `Process<list<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry>>` instantiations with functor vtables
  0127803c/01278014/01278028), then four interface setters with interpolate=1 (00884e90/00884f60/00884f90/00884fe0).
  Owning it plus the interface FrameUpdate closes both WorldFrame's update_environment and 176's interpolator boundary.
- The whole-tree capture is delegated: `work/modern_screen_effect_tree_20261029/`.
- **Capture done** (`work/modern_screen_effect_tree_20261029/`, delegated agent, spot-checked against 00883530 here):
  - 29 genuine spans (3,373 B); 2,816 cases (2,048 FrameUpdate incl. 128 tagged NaN/inf, 256 interface FrameUpdate,
    128 per setter); all control groups pass: span bytes, determinism, stub kind swap, stub pointer misuse, execution
    scope and 16 byte mutations. An independent Python model reproduces all 22,733 weights and 10,566 removals.
  - Corrections to the brief: the three Process<> bodies are the radial (00883530), displacement (00883640) and filter
    (00883750) lists; functor +4 is the priority stack (interpolator +0x90); 0xbfea14 is `free`.
  - Removal resumes from the previous node's next, so back-to-back removals never skip an entry.
  - With no radial entries, the two track enums keep their stale values and reach SetRadialBlurPositionParams.
  - Unicorn's x87 does not set the invalid flag, so recorded FPSW sticky bits are not evidence, and the corpus does not
    distinguish PC53 from PC64.
- The portable owner is delegated: `work/modern_owned_screen_effect_20261029/`.
- **Pending 177 cut, second batch** (fabletlc-3e): gate_native_lipsync_entries (22 MATCH). The STLport vector headers
  are now non-POD aware: fable_stlport_vector.h, fable_stlport_vector_impl.h, fable_stlport_algobase.h,
  fable_stlport_uninitialized.h and fable_lipsync_data_bank.h changed; two stand-in sources were removed and two added
  under rebuild/src/compiled/00/a4. Modern contact (searched): only tools/modern/modern_fixture_support.py, which patches
  fable_stlport_tree.h and anchors an algobase include on the algo.h line. The snapshot tests in qualify_source_cut.py
  will exercise it.
- **Pending 177 cut, third batch** (fabletlc-3e): gate_native_lipsync_construct (2 MATCH). Declarations were added to
  fable_game_text_bank.h (CBankFileAsync::SetNonCached) and fable_lipsync_data_bank.h (constructor, SetPhonemeBank), and two
  a4 sources were extended in place. No removals. No modern file names these headers or classes (searched).
- **Portable owner** (delegated agent, reviewed here): `ScreenEffectStack.hpp`.
  - Owns Process and the three list walks, the radial/displacement/filter functors (including the filter Lerp, the
    thing-map bypass restore 00883ae0 and the graphic bypass counter 004bfb00), the entry destructors, the four interface
    setters, the interface FrameUpdate 00885010 and the interpolator FrameUpdate 006b8640.
  - Externals: the graphic dirty notification 004c7ac0 and control-block delete. Memory is owned by std::list/vector;
    export.py asserts the retail `free` trace equals the removed entries' storage in retail order.
  - x87 NaN behaviour is modelled (quieting, the larger significand wins, the indefinite for 0*inf and inf-inf), so all
    2,816 cases replay bit-exactly with **no exclusions**.
  - Per-field lerp rounding follows the disassembly: products kept on the x87 stack, the current one spilled, or both spilled.
- Mutations: 24/24 killed (the corpus alone also kills all 24). Strict x86/x64/gcc pass. Closure: 8 reports, 120 pins.
- **Bundle** `work/modern_owned_screen_effect_bundle_20261029/`: generated by `derive_from_176.py` (prefix `se_` after
  `en_`); the native admission reads the new lane's capture.json and controls.json.
- **177 cut:** the three native batches; 7 header changes, 1,743 inputs, none removed. Five snapshot tests pass.
- **177 ACCEPTED** (`.../validation_matrix`): x86/x64/clang/gcc/headless each ran 184 checks, all with return code 0.
  Native unfrozen.
- 176's `update_spell_effect_interpolator()` external and WorldFrame's `update_environment()` (006b32a0 -> 00885010) now
  have owned implementations (`update_spell_effect_interpolator(stack,face,host)`, `spell_effect_interface_frame_update`).
  Binding them into a session-level host is integration work; no accepted file needs to change for it.
- **Pending 178 cut, first batch** (fabletlc-3e, after the 177 unfreeze): gate_native_physics_faces_segment (8 MATCH).
  - New hand-owned fable_physics_faces_segment.h. engine/CPhysicsFacesSegmentBranch.h and CPhysicsFacesSegmentLeaf.h
    become shims; the retail Leaf is 0x10 bytes.
  - fable_carray.h gains CArray<T>::Sizeof.
  - Removed stand-ins in 00/a8: five physics-faces stand-ins and four duplicate CNavSwitchableLeafNode roots.
  - Modern references (searched): CArray appears only in comments (EnvironmentComposition/ThemeCalculation) and in
    signature strings in tools/modern (font manager constructor, persisted_types). No modern code reads the changed headers.

## 178 candidate: atmosphere sounds mute (006b1d30)
- Capture `work/modern_atmosphere_theme_tree_20261029/` (delegated, spot-checked against the entry and both exits):
  - this = `[CWorld+0x24]` CAtmosProcess; the function is effectively SetAtmosSoundsMuted(byte, float).
  - Muted: for themes 1..N-1, fade DAY and NIGHT to 0 over `time`, then period=0 and +9 = the raw flag byte.
  - Unmuted: 006b1a20, then +9 = 0. 006b1a20 reads +9 first, so the cross-fade starts one frame after unmuting.
  - 9 genuine spans (1,699 B) equal the capstone tree. 2,176 cases, 48 of them retail fault rows (an unchecked null def
    from the by-index lookup). All controls pass, including 17 byte mutations.
  - Wrong manifest labels: 006b1d30 Sound_SetThemeByName, 006b1a20 CGameScriptInterface_SetSoundTheme, 006b1900
    SetQuestInfoName, 009ad3b0 GetIDFromSymbol (it actually counts the defs of a class).
- The owner is delegated: `work/modern_owned_atmosphere_mute_20261029/`. It composes DayNightThemesUpdate and SoundThemeFade.
- **Owner** (delegated agent, reviewed here): `AtmosphereSoundsMute.hpp`, `set_atmosphere_sounds_muted(state, muted_argument, fade_time, x)`.
  - It owns 006b1d30's own logic plus IsDay/IsNight (1/6 < t <= 0.875; NaN counts as night). It composes the accepted
    DayNightThemesUpdate (006b1a20) and SoundThemeFade (006b1900) owners through host bindings, without editing them.
  - Two API workarounds are documented in the header:
    - a ThemeChannel token preserves retail's order (the period string is built before GetString);
    - SoundThemeFadeHost members that only 006b1600 uses are declared but not defined, so any use fails to link.
  - Replay: 2,128/2,176. The 48 retail fault rows are excluded by an input-only predicate (the by-index lookup is null at
    an index the walk reaches). The selection was cross-checked against the fault rows only after it was made.
  - Mutations: 23/23 killed. Strict x86/x64/gcc pass. Closure: 8 reports, 120 pins (`derive_from_177.py`, prefix `am_`).
- **178 cut:** physics_faces_segment and the nav duplicate retirement; 5 changes, 1,743 -> 1,744 inputs, none removed.
- **178 ACCEPTED** (`work/modern_owned_atmosphere_mute_bundle_20261029/validation_matrix`): x86/x64/clang/gcc/headless each
  ran 185 checks, all with return code 0. Native unfrozen.

## 179 candidate: combat frame (CCombatManager::Update 006ed200, slices A+B)
- Scope (delegated agent; `work/modern_combat_frame_tree_20261029/NOTES.md`):
  - this = `CWorld+0x4c` scoped_ptr<CCombatManager>.
  - The full closure is 589 functions and 79 KB, with subclass-dependent creature/sequence virtuals, so the whole tree is
    not feasible now.
  - The successor is computed after the callback, so erasing the current node reads freed memory.
- Wrong manifest labels:
  - 006ed200 (really CCombatManager::Update);
  - 006ed030 (a guess);
  - 006f2680 (STLport _M_insert_overflow);
  - 0049d870/004167c8 (the frame-counter getter and its thunk).
  - egor_pdb_names.tsv addresses are Ego_r-build addresses, not retail.
- Slices taken:
  - **A:** the walk 006ed200 genuine, with the per-combatant update 006ed030 stubbed.
  - **B:** CombatDialogueManager::Update 008af780 genuine, with AttemptToSpeak 008af500 stubbed.
  - The capture is being built in the same directory.
- **Capture built** (slices A+B): 11 genuine spans (658 B) equal to the capstone walk; 2,496 cases (192 tagged
  dangling_current_erase, 158 missing-talk-component rows). All controls pass, including 18 byte mutations.
  - Walk: the successor is the smallest live key above the current one after the callback. Inserts above the current
    key are visited the same frame. Erasing other nodes is tolerated.
  - Erasing the current node reads freed memory, so the walk is unsafe there; the harness-chosen tree shape means the
    capture can't say which outcome retail would actually get.
  - Dialogue: a dead talker drops one reference and deletes only at exactly 0. When key 15 is absent, the component
    lookup reads one past the end of the vector. IsTalking uses world+0x48, while LastSpeechFrame uses [013b89bc].
- The owner is delegated (`work/modern_owned_combat_frame_20261029/`):
  - current-node erase is out of domain, excluded by an input-only predicate;
  - the stray component is replayed through a host lookup query.
- **Pending 179 cut, first batch** (fabletlc-3e): 291 duplicate stand-in sources retired, each an address that is already
  an exact gate root elsewhere; list in work/native-recreation/wf-ac/retired.txt. No headers changed. symbol_index.py
  gained two companion rows. Checked here: none of the 291 (or their _test files) is among the 178 cut's 1,744 snapshot
  inputs, and no rebuild/modern or tools/modern file names any of them.
- **Owner** (delegated agent, reviewed here): `CombatFrame.hpp`.
  - `update_combatants`: successor = upper_bound(current key) after each callback.
  - `update_combat_dialogue`: three Get sites through the accepted IntelligentPointer.hpp; owns IsTalking (signed
    world-frame delta < duration).
  - `update_combat_frame`: the walk, then the dialogue update.
  - Replay: 2,304/2,496. The 192 current-node-erase rows are excluded by an input-only predicate (the side-effect script
    erases the current key). The 150 missing-talk-component rows are replayed through the host's component_talk(end).
  - Mutations: 23/23 killed (the corpus alone kills all 23). Strict x86/x64/gcc pass. Closure: 8 reports, 125 pins
    (`derive_from_178.py`, prefix `cf_`). The test echoes the corpus path, so the matrix marker is built from that path.
- **No 179 cut:** the 291-source retirement changed no snapshot input (prepare_source_cut asserts changes). The 178 cut stays pinned.
- **179 ACCEPTED** (`work/modern_owned_combat_frame_bundle_20261029/validation_matrix`): x86/x64/clang/gcc/headless each ran
  186 checks, all with return code 0. Native unfrozen.
- **Pending 180 cut, first batch** (fabletlc-3e): gate_native_mesh_x_chunk_descriptions (7 MATCH).
  - fable_chunked_file.h gains GetChunkDescription overrides on five X-chunk classes.
  - One 00/aa source was added, and seven 00/aa stand-ins were removed.
  - No rebuild/modern or tools/modern file names the header, the X-chunk classes or GetChunkDescription (searched).

## 180 candidate: CPlayer::Update 00488ab0 + main-player free camera (00449970 -> 004884d0)
- Capture `work/modern_player_update_tree_20261029/` (delegated):
  - Update: 183 B genuine in 5 spans; SetupInitialPlayerMode and SetPlayerPos are stubbed. The CWorld vtbl+0x30 call
    resolves to GetWorldMap 004ae8b0 and runs genuine.
  - Free camera: 1,185 B genuine in 9 spans, entered from the retail UpdateFrame fragment 004a5cc5.
  - 2,656 cases; all controls pass, including 31 byte mutations.
- Retail layouts:
  - CPlayer fields after PlayerModes sit 8 lower than the PDB (the STLport list is a 4-byte pointer); CPlayerManager sits
    4 lower; CThing PhysicsTC is at +0x60.
- Retail hazards:
  - an uninitialised `this` for the focal-point getter when thing bit 27 is clear and tracking is on (53 rows, tagged
    `stale_ebp`);
  - a past-end TC 0x5b read;
  - an unchecked Players[PlayerNeutral] index.
- Wrong labels: 004349e0 (the InitialFadeActive getter), 006a4d00 (CThing::GetPosition), 00449970 (GetMainPlayer),
  00a14440 (in-place Normalise), 00a0c570 (camera set-heading).
- **Held:** the free camera's fpatan (and any fsin/fcos) values came from Unicorn's host libm. Per the 173 policy, the
  agent is adding two-pass hardware x87 injection (a lane-local copy of the sun_moon harness) before any owner is written.
- **Hardware trig injected:**
  - 9 sites: fpatan x3, fsin x3, fcos x3, all in the free-camera tree. fsqrt and fprem are exact, so they stay emulated.
  - Lane-local harness `hw/` (a VC7.1 copy of the sun_moon harness with an ops mode; each batch is run twice and must
    match byte for byte). The fpatan results feed later fsin/fcos operands, so injection iterates to a fixed point (3 passes).
  - 4,869 injections and 0 operand mismatches; the semantic model reads the hardware table.
  - Hardware vs libm: 3,129/3,449 80-bit results differ, but 0 float outputs, because every result is stored as binary32.
- The owner is delegated (`work/modern_owned_player_update_20261029/`):
  - trig is a host boundary returning x87::Extended, as in SunMoonUpdate;
  - the stale_ebp rows are excluded by an input-only predicate;
  - the past-end reads are replayed through host queries.
- **Pending cut, second batch** (fabletlc-3e): gate_native_mouse_dx_accessors (11 MATCH).
  - New hand-owned fable_mouse_dx.h (CMouseDX, size 0x4860); engine/INDEX.tsv marks the row HAND_OWNED.
  - One 00/ab source was added, and eleven stand-ins were removed.
  - Modern contact: only comments name CMouseDX (MouseAccumulator.hpp, fable_rawmouse_probe.cpp, mouse_buffer_patch.py's
    docstring), and no modern file includes the header. Hashed inputs only.
- **Pending cut, third batch** (fabletlc-3e): gate_native_joystick_dx_overrides (11 MATCH).
  - New hand-owned fable_joystick_dx.h; engine/CJoystickDX.h becomes a shim; INDEX.tsv marks the row HAND_OWNED.
  - One 00/ab source was added, and eleven stand-ins were removed.
  - Modern contact: only a comment in platform/Pad.hpp names CJoystickDX, and no modern file includes the header.
- **Pending cut, fourth batch** (fabletlc-3e): gate_native_keyboard_dx_is_key_pressed (1 MATCH). New hand-owned
  fable_keyboard_dx.h; INDEX.tsv marks the row HAND_OWNED; one 00/ab source was changed in place; no removals.
  Modern contact: only a comment in platform/KeyboardAccumulator.hpp names CKeyboardDX.
- **Owner** (delegated agent, reviewed here): `PlayerUpdate.hpp`.
  - Owns CPlayer::Update, GetMainPlayer, UpdateFreeCamera and the 004a5cc5 fragment, with PointAt, set-heading and
    Normalise.
  - Arithmetic is integer-exact PC53, plus a correctly rounded square_root and an exact _CIfmod(x,1). Cameras are held as
    binary32 bit images.
  - fpatan/fsin/fcos are host boundaries returning x87::Extended, answered from the hardware table.
  - Replay: 2,603/2,656. The 53 stale_ebp rows are excluded by an input-only predicate; the past-end reads are replayed
    through host queries. NaNs are compared by class, as in the native model.
  - Mutations: 29/29 killed. Dropped as equivalent: Normalise regrouping (at most 1 binary64 ulp, absorbed by the float
    store), a second GetPItem, and the first PointAt normalise. Strict x86/x64/gcc pass.
  - Closure: 8 reports, 134 pins (`derive_from_179.py`, prefix `pu_`; it pins hw_table.jsonl and the hw harness sources).
- **180 cut** against the 178 cut (179 had none): mesh-X descriptions, mouse/joystick/keyboard DX and the 0xABC420 root.
  6 changes, 1,744 -> 1,747 inputs, none removed.
  - The first run hit a lease window in qualify, and a piped exit code let the old pin through. Preflight rejected it
    correctly; the outputs are archived in `attempts/no_cut_lease_window_20261029` (see GOTCHAS).
- **180 ACCEPTED** (`work/modern_owned_player_update_bundle_20261029/validation_matrix`): x86/x64/clang/gcc/headless each
  ran 187 checks, all with return code 0. Native unfrozen.
- **Pending cut, first batch after 180** (fabletlc-3e): gate_native_mouse_dx_focus (4 MATCH).
  - New headers fable_dinput8.h and fable_win32_cursor.h.
  - fable_system_manager_state.h gains IsApplicationInputFocus and an inline GetApplicationWinHandle (no layout change);
    fable_mouse_dx.h gains UpdateAcquired.
  - One 00/ab stand-in was replaced.
  - Modern contact: tools/modern/templates/{ActualToVirtualCoord,IMEFontDraw,IMEFontExtent}_test_SOURCE_ONLY.cpp
    include fable_system_manager_state.h. The added declarations don't change the layout; the next qualify and matrix
    will compile them.
- **Pending cut, second batch after 180** (fabletlc-3e): gate_native_joystick_dx_focus (2 MATCH). fable_joystick_dx.h
  gains the UpdateAcquired declaration, and one 00/ab source was added. No modern file includes the header (checked for 180).
- **Pending cut, third batch after 180** (fabletlc-3e): gate_native_keyboard_dx_focus (1 MATCH). fable_dinput8.h gains
  vtable slots 9-10 and DIDEVICEOBJECTDATA; one 00/ab source was added. No modern file includes fable_dinput8.h (searched).

## 181: CWorld::PrepareAtmosBanksForLoadedRegion (0049e5e0)
- One delegated agent did both the capture and the owner (`work/modern_atmosphere_banks_tree_20261029/`,
  `work/modern_owned_atmosphere_banks_20261029/`).
  - If PAtmosProcessor is non-null, it calls SetAtmosSoundsMuted(region.SoundThemesEnabled == 0, 1000.0f) on the world
    map's loaded region (CRegion is 0x58 bytes in retail; SoundThemesEnabled is at +0x56).
  - WorldFrame.hpp's `spawn_player()` name is wrong: this call spawns nothing.
  - Wrong labels: 004fb150 is CWorldMap::GetLoadedRegion and 004fc180 is CWorldMap::GetRegion.
- Capture: 13 genuine spans (this function plus the 178 tree, with the 178 stubs reused through its pinned capture.py).
  2,304 cases, 28 of them inherited fault rows; all controls pass, including 28 byte mutations.
- Owner `AtmosphereBanksPrepare.hpp` calls the accepted set_atmosphere_sounds_muted.
  - Replay: 2,276/2,304. The 28 fault rows are excluded by an input-only predicate.
  - Mutations: 13/13 killed. Strict x86/x64/gcc pass. Closure: 8 reports, 115 pins (`derive_from_180.py`, prefix `ab_`).
- **181 cut** against 180: mouse/joystick/keyboard DX focus; 5 changes, 1,747 -> 1,749 inputs, none removed. The modern
  fixture templates that include fable_system_manager_state.h compile.
- **181 ACCEPTED** (`work/modern_owned_atmosphere_banks_bundle_20261029/validation_matrix`): x86/x64/clang/gcc/headless each
  ran 188 checks, all with return code 0. Native unfrozen.
- **Pending cut, first batch after 181** (fabletlc-3e): gate_native_keyboard_dx_get_key_name (1 MATCH). fable_dinput8.h
  gains DIPROP types only (SDK-absent branch), and one 00/ab source was added. No modern file includes fable_dinput8.h.

## 182: first replacement of an accepted owner (GetBlendedTheme.hpp comment)
- The comment now says the later key moves a day on when the earlier key is AFTER it (the code tests `early > late`), and
  that equal keys, a single theme included, divide by zero.
- Lane `work/modern_owned_get_blended_theme_comment_20261029/`: check_comment_only.py copies the accepted header to
  preimage/ and proves the code tokens are unchanged (3 comment lines differ). The receipt pins the copy, not the
  production path.
- Bundle `work/modern_owned_get_blended_theme_comment_bundle_20261029/`: hand-adapted scripts (promote records before and
  after hashes for a replaced file). Cut and wait scripts come from `derive_cut_from_181.py`.
- **Closure-chain extension** (prepare_matrix.py): after adding gx_expected, every older bundle's
  `assert digest(root/name)==XX_expected(name,sha),name` closure check is rewritten to gx_expected. That chain maps a pinned
  hash through every later promotion. 29 checks were routed, and preflight passed first time. The older direct-pin loops
  already go through the chained latest_hash.
- **182 cut** against 181: keyboard_dx_get_key_name (fable_dinput8.h types); 1 change, 1,749 inputs.
- **182 ACCEPTED** (`.../validation_matrix`): x86/x64/clang/gcc/headless each ran 188 checks, all with return code 0.
  Accepted owners can now be corrected. The next bundle must keep the rewrite, so carry the `older=` loop forward when deriving.
- **Pending cut, first batch after 182** (fabletlc-3e): gate_native_joystick_dx_exclusive_mode (1 MATCH).
  - fable_dinput8.h gains IDirectInput8W and enum types (SDK-absent branch).
  - fable_input_manager.h: CInputManagerDX::DInput is retyped from void* to IDirectInput8W* (no layout change), and an
    inline GetDInput is added.
  - fable_joystick_dx.h gains the EnumAndInit declaration.
  - Modern contact: rebuild/modern/reconstructed/tests/MainGameReplayTests.cpp includes fable_input_manager.h but never
    names DInput or CInputManagerDX. The next cut's snapshot tests and matrix will compile it.
- **Pending cut, second batch after 182** (fabletlc-3e): gate_native_mesh_cloth_chunk_description (1 MATCH). One 00/ac source and its test changed in place; no headers changed. Not a snapshot input.

## 183 candidate: x87 NaN propagation in theme_priority_detail (fixes the 175 exclusion)
- Lane `work/modern_owned_theme_nan_20261029/` (delegated):
  - Cause: theme_priority_detail add/subtract/multiply/divide return the FIRST NaN operand.
  - 7 of the 2,048 175 cases fail through rebalance_weights: the indefinite meets 0x7fc00000 on a significand tie, and
    the positive one wins.
  - A 512-case native supplement (19 NaN words, run through 175's harness) exposed the missing rule: a signalling NaN in
    memory loses to a NaN already in a register (the SDM rule).
- Staged fix: ThemePriorityStack.hpp detail only (quiet, pick_nan, and m32 overloads that read float bits), plus comment
  corrections in PriorityStackTransitions.hpp and a staged PriorityStackTransitionsTests.cpp (the accepted test hard-codes
  the 1941 count).
- Evidence:
  - the full 2,048 corpus with no exclusions;
  - the 512-case supplement;
  - every dependent accepted test passes on x86/x64/gcc;
  - accepted vs staged helpers are bit-identical on 12,352,882 non-NaN or single-qNaN pairs;
  - 10/10 NaN mutants killed (4 only by the supplement).
- **Held:** the signalling-vs-quiet rule rests on the SDM and Unicorn only, so the agent is now checking it on the real
  x87 (a lane-local hw harness). The same check tells us whether ScreenEffectStack's model (177) is wrong for a
  signalling NaN in memory.
- **Hardware NaN check:** a lane-local hw harness with a nan mode ran 22,525 x87 operations (all fadd/fmul/fsub/fsubr/
  fdiv/fdivr m32/m64 and register shapes, plus fld) twice on this CPU.
  - Hardware vs the staged model: **0 mismatches**.
  - Hardware vs Unicorn: 2,563 result mismatches, all involving a signalling NaN loaded by fld. Unicorn keeps it
    signalling; hardware quiets it. Recorded in GOTCHAS.
  - The 512-case supplement still matches, because none of its deciding instructions hit that difference.
- **177 finding:** the accepted ScreenEffectStack.hpp screen_effect_detail diverges from hardware on 246 pairs of the
  shapes it uses: a register NaN meets a signalling m32 operand, and the model lets the memory NaN compete. This needs a
  correction lane, which should also check the 177 capture's NaN family for fld-sNaN rows.
- **183 bundle** `work/modern_owned_theme_nan_bundle_20261029/` replaces 3 accepted files, using the 182 recipe with the
  `nx_` prefix. Closure: 11 reports, 142 pins. Promoted locally. The matrix adds two checks (the 2,048 corpus and the
  512 supplement), for 190.
- **183 ACCEPTED** (`work/modern_owned_theme_nan_bundle_20261029/validation_matrix`).
  - Native cut taken against the 182 cut while fabletlc-3e was frozen. It has 3 header changes (fable_dinput8.h,
    fable_input_manager.h, fable_joystick_dx.h), 1,749 inputs, and none removed; reviewed in review_notes.json.
  - Qualify passed. prepare_matrix routed 30 older closure checks through nx_expected. Preflight passed.
  - Full matrix: x86/x64/clang/gcc/headless each have 190 checks, all with return code 0. Native unfrozen.
  - The 175 composition now has no NaN exclusions. Next: the 177 ScreenEffectStack signalling-m32 correction.

## Session end (user called it)
- 183 accepted; native unfrozen. Nothing committed.
- The 177 ScreenEffectStack sNaN correction lane (`work/modern_owned_screen_effect_nan_20261029/`) was started and
  then stopped at shutdown. It holds only the skeleton directories and the preimage copies of ScreenEffectStack.hpp and ScreenEffectStackTests.cpp. Resume from NEXT_SESSION "Session end 2026-10-29".
