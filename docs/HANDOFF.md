# Resume here (one page) — updated 2026-09-11 (evening, after the v11 playthrough)

## Converter continuation (2026-09-12)

**End-of-night checkpoint — user requested a stop. Resume implementation only when asked.**
New Oakvale: **47/51 functions, 14/18 files compile; zero missing bodies; 1,296 TODOs**.
Latest full suite: 870 tests, with only the known Bully two failures/two errors in the parallel
reconstructed port. Later temporary-Thing work passed its focused tests. Draft registration is
still disabled; no runtime patch or DLL was applied. No converter processes are pending.

Start with [the current handoff](journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-12.md)
and [the resume checklist](scripts/SCRIPT_CONVERTER_RESUME_CHECKLIST.md). Next: connect the
husband's verified resource/temporary-Thing lifetimes to a separate disabled Lua candidate.
The current runtime's blocking Speak is not equivalent to retail's nonblocking speech.
The four-operation resource extension is prepared and tested under
`work/man_resource_extension/`, but remains unapplied. Preserve the parallel runtime/Bully files.

The checkpoints below are historical and are superseded by the links above.

**Latest marathon checkpoint:** converter/benchmark tests 77/77; full recovery suite 394/394;
canonical SDK PASS with `--skip-mirrors`. Benchmark now compiles recovered bodies with Lua 5.4,
excludes stubs/loaders, and records every syntax failure. Passing recovered files rose **6/18 ->
8/18** (Barrel and Dead Father); ten still fail. `--require-syntax` returns nonzero for these drafts.

Added reviewed parent-field and numeric-constant joins, exact-target AL termination-result copies,
mutable scalar storage across loops/branches, consistent synthetic field getters/setters, typed
ECX quest-receiver normalization, and hexadecimal boolean comparisons. The actual generated Barrel
Main passes execution checks for both instruction-text branches, an already-shown instruction, and
prompt cancellation. No call-recall regression: SummoningTheShip 0.60 -> 0.70, DragonBossFight
0.49 -> 0.62, BeardyBaldy 0.82 -> 0.85; scores include state accessors and are not gameplay parity.

Next: inlined string comparisons, native thing temporary ownership/return slots, nested master-data
and vector fields, and remaining comma expressions. Keep the ten syntax failures visible rather
than replacing unknown behavior with successful no-ops. Details and exact current diagnostics:
`docs/scripts/LIFTER.md`, `refs/script_recovery/lifted/LIFT_BENCHMARK.md` / `.json`. Before/after
artifacts are under `work/lifter_marathon_20260912_*`. No game install or Forge source was changed.

Earlier continuation today:

Resumed `tools/script_recovery/lift_native_lua.py`. Assignment-in-condition lowering now preserves
`&&`/`||` short-circuit execution, evaluates the left operand once, and retains the previous assigned
value when the right-hand call is skipped. Fixed `else`/`elseif` closure and retention of nested
terminal braces. New tests execute emitted Lua across branches and verify call effects and values.

Gates: converter 61/61; full recovery suite 378/378. Regenerated all 16 benchmark rows with unchanged
call recall/precision and TODO counts. SDK validation passes with `--skip-mirrors`; the full SDK
check reports existing overlay drift in `D:\Code\FableForge\docs\re_reference\fse_native_overlay.json`
and `D:\Code\ForgeFSE\docs\fse_native_overlay.json` (neither changed in this continuation).

Syntax inspection passes 47/59 generated Lua files, including stubs. The remaining 12 contain
unsupported native expressions/control flow. Next: expose syntax failures in the benchmark, then
lower the remaining supported expression shapes. See `docs/scripts/LIFTER.md` for details.

## Immediate marathon resume

Open `docs/journal/2026-09/NEW_OAKVALE_MARATHON_CHECKPOINT_2026-09-09.md` (entries dated 2026-09-11)
first. The game is closed. **Bundle v13 is generated AND installed** (`work/new_oakvale_test_bundle_20260911_v13/`,
DLL SHA-256 `2442A02C7AACF6468386F86608A6FF9FFDCD6D0A434D46DD256B777268842815`, in both the game root and `FSE\`, Lua tree included; rollback in
`FSE\backups\new-oakvale-v13-20260911-163519\`). v12 was built but never installed (its plain
same-priority idempotence rule would have re-broken Affair Man's walk-home).

**The user played the full childhood on v11 and reported "everything felt great"** (log archived as
`runtime_evidence/interactive-20260911-v11-single-authority-clean.log`: zero `!!! ERROR`, zero
`No control handle`, Wife dialogue, one Guard lecture, all cutscenes, Dad reward lines). Two reports:
1. **Bully RunTo makes zero movement / compatibility jump vanishes on camera.** CORRECTION (2026-09-11 evening): retail's
   `.RunTo` macro branch DOES wait (polls scripted-resource slot `0x68` IsPerformingScriptTask, see
   `runtime_evidence/bully-run-native-wait-analysis-20260910.md`); my earlier "non-blocking" note was
   read off the macro text and was wrong. Retail therefore has him at `MK_OIBR_BULLY2` before the
   `UseCamera CAM_OIBR_BRAT` … `BULLY.Drawable FALSE` tail. Under Forge the native task reports
   completion after 2.33-2.37 m at the IDENTICAL endpoint 11.759 m short in 4 of 5 archived runs
   (`analysis-bully-run-variance-20260910.json`; one run reached the marker). The Lua compatibility
   move then covers the rest after the macro, on the same camera, and hides him in frame: that is the
   jump/vanish the user sees. A hide-after-second-macro tweak was tried and REVERTED (it reorders
   retail's sequence and broke a fixture). The fix must make the in-macro task complete; a
   Later correction: installed TNG proves the repeated endpoint is exactly `MK_OIBR_BULLY1`, where
   the macro teleports him before RunTo. Thus the four failed RunTo tasks move zero metres; the
   earlier 2.33-2.37 m measurement includes only the authored pre-run teleport. A passive diagnostic
   build now logs native MoveToPosition issue operands and every task-poll result. Run one Bully
   encounter and inspect `[CutsceneMoveDiag]`; see
   `docs/journal/2026-09/BULLY_RUNTO_STALL_2026-09-11.md`.
2. **Bully health bar colours wrong.** Disassembly at `0x00DBC40C-0x00DBC43B` writes the empty colour
   as memory bytes `00 00 FF FF` and the filled colour as `00 FF 00 FF`; the engine `CRGBColour` is
   B,G,R,A, so empty = red, filled = green (the same words as the barrel timer). The Lua and the
   entity inventory had read the empty bytes as RGBA blue, and `export_bully_health_bar.py` plus its
   snapshot had transposed them as `FF 00 00 FF / opaque blue`. All three are corrected; three Bully
   traces regenerated. Fixture validation 119/119; suite 317/317.

**Nested-control contract, runtime-proven across the v10 and v11 runs (Forge v13 rule):**
- A Forge handle has no script-owner identity, so a second `StartScriptingEntity` resource for an
  actor the same VM already controls is never granted (v10 Affair Wife hang).
- Retail Main loops re-call `StartScriptingEntity` on the same resource every iteration (Bully,
  Victim, Barrel Man), which the engine treats as idempotent (v11 depth inflated to 1900+).
- v13: a re-acquire over an owned handle at the held priority saturates at depth 1 (outer handle must
  survive the following release: Affair Man's conversation precedes his walk-home loop, proven clean in
  v11); a different priority counts one nested level; only the outermost release destroys.
  `audit_forgefse_control_abi.py` schema 0.4, 7/7 tests.

**Next run (user-driven, `FSE_Launcher.exe`):** confirm `legacy=false` at startup, the Bully bar shows
green-over-red, Wife dialogue still plays, and the log's `Reusing live control handle` lines stay at
depth 1 for the per-frame loops. Then decide whether to keep or remove the Bully run-off
compatibility completion (retail hides him wherever the short run ended).

Offline gates: recovery suite 317/317, package fixtures 119/119, authority `ok=true`, SDK PASS.

**Converter started (user request, evening):** `tools/script_recovery/lift_native_lua.py` lifts a
native cluster (or a translation-unit entity function) into a draft ForgeFSE Lua package, marking
anything unrecognised with `TODO(native)`. `benchmark_lifter.py` regenerates
`refs/script_recovery/lifted/LIFT_BENCHMARK.md` against Aeon's ports and our New Oakvale entities;
10/10 unit tests, suite 327/327. Read `docs/scripts/LIFTER.md` for what it handles and the three
next levers (thread bodies, `CScriptThing` message slots, field naming).

**Latest continuation:** v14 video/log proved the compatibility hide used a premature 2 m exit while
its move task was active. v15 now waits on retail's `IsPerformingScriptTask` contract and adds the
correct marker-form `MoveToThing` passive hook; it is installed, game closed, rollback
`FSE/backups/new-oakvale-v15-bully-wait-20260911-194754`, DLL SHA-256
`8102215BF082508052FDF451B1FE2BAB61E2FD8FC736DD93F4389EA69C6DF80E`.
The lifter now consumes all exported native thread bodies: benchmark recall is 0.53
SummoningTheShip, 0.45 DragonBossFight, 0.72 BeardyBaldy, and 0.97 HerosOldHouse. It recursively
queues nested worker threads, exported Dragon's `RunEnemySpawning` and `JackTaunts`, accepts Ghidra's
decimal vtable offsets, normalizes both `LAB_` and `FUN_` thread-address spellings, and proves final
destructor-only goto targets before dropping their TODOs. The noisy helper-body experiment was
rejected because it reduced precision. Suite 350/350.

Gate commands (from repo root): `python -m unittest discover -s tools/script_recovery -t . -p "test_*.py"`;
`python tools/script_recovery/validate_reconstructed_package.py --fse-root refs/script_recovery/reconstructed/NewOakValeIntro/FSE --package Q_NewOakValeIntro --fixtures refs/script_recovery/new_oakvale_intro/fixtures --traces refs/script_recovery/new_oakvale_intro/traces`;
`python tools/script_recovery/validate_new_oakvale_authority.py --profile-fse refs/script_recovery/new_oakvale_intro/runtime_playtest --source-fse refs/script_recovery/reconstructed/NewOakValeIntro/FSE`;
`python tools/validate_tooling_sdk.py`; `python tools/script_recovery/audit_forgefse_control_abi.py --forge-root D:\Code\ForgeFSE-retail-shadow`.
Both repos still carry the marathon work **uncommitted** (FableTLC `feat/novi-script-recovery`;
ForgeFSE-retail-shadow `feat/upstream-fse-2026-09-02`). A stray root file `0x0382cff0` must not be
committed. Check the installed `FinalAlbion.qst` for `AddQuest("NewOakValeIntro", TRUE)` before every
run (deleted 2026-09-11; Steam verify can restore it).

Current compatibility behavior adds three narrow recoveries on top of the byte-locked retail scripts.
Barrel Man selects exact TLC `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN` after broken stock even when the
Hero is nearby, suppressing the contradictory thanks/good-deed path; the final-barrel one-gold chain
is independently proven from TLC code and data. Bully reuses the authored run-off marker if native
`RunTo` reports completion while still short. Affair Wife retains native movement as the primary path
but immediately reissues movement toward the live husband whenever that task has ended outside the
native 3 m arrival condition, capped at four retries. This matches the clarified intermittent report:
she can reach him normally, but sometimes stops early.

## Historical New Oakvale chronology

Everything below this heading records incremental evidence and older gate counts. It is retained for
provenance and does not override the current snapshot above.

Latest interactive evidence is archived as
`refs/script_recovery/new_oakvale_intro/runtime_evidence/interactive-20260910-redtimer-bullyrun-barrels.log`.
It confirms the timer is red, Barrel Man judged the Hero nearby at 5.092 m and therefore selected
`BARRELMAN_THANKS`, while the independently propagated broken-barrel bad deed was claimed by the
Guard. This apparently mixed outcome is retail-exact: warehouse return judgment tests only
visibility/proximity, not barrel destruction. The bully actor map is also correct, but the authored
`BULLYRUN1` moved him only about 2.36 m before its explicit `Drawable FALSE`. A new diagnostic Forge
build records movement actor/resource identity, destination, move type, flags, and missing handles.
New Oakvale also has a guarded compatibility completion: if shipped `RunTo` returns with Bully still
more than 2 m from `MK_OIBR_BULLY2`, it restores drawability, finishes the same authored run (capped
at 300 frames), then hides him for retail cleanup. Package validation is 117/117 with zero warnings;
recovery tests are 248/248; tooling SDK validation passes. The build and Lua remain undeployed, and
the game is closed. Diagnostic DLL SHA-256:
`C41F54F9B0D43700CEE096662F2A01752B9CB3DDA216070F22E60DCB2B4C2D7E`.
Affair Man is now covered by one reproducible byte artifact spanning his complete Init
`0x00DB0950-0x00DB09E0` and Main `0x00DB09E0-0x00DB1DA2`: 5,202 bytes / 1,503 instructions, with
both regions ending at exact `ret` boundaries. This locks the live Wife/Woman lookups, confrontation,
walk-home, affair dialogue, kiss/hug, and terminal cleanup around the remaining Wife runtime seam.
Current recovery suite is 250/250; package validation remains 117/117 with no warnings and SDK
validation passes. No deployment or game launch occurred.
Affair Woman is now byte-locked too: complete Init `0x00DB1E80-0x00DB1EF4` and Main
`0x00DB1F00-0x00DB299A`, totaling 2,830 bytes / 810 instructions at exact `ret` boundaries. The
artifact confirms her husband/wife lookups, kiss/hug reception, Wife-proximity flight, authored
run-off marker, and removal/cleanup sequence. She never removes or relocates Affair Man, further
narrowing Wife's air-scolding to the pending runtime identity/model seam. Current gates: 252/252
recovery tests, 117/117 fixtures with zero warnings, and SDK PASS. Nothing was launched or deployed.
Affair Wife is now fully byte-locked: Init `0x00DB2A70-0x00DB2B10` and Main
`0x00DB2B10-0x00DB3E2A`, totaling 5,050 bytes / 1,543 instructions at exact return boundaries. The
continuous artifact contains the earlier route and argument slices and confirms the one-time husband
position snapshot, repeated live-husband 3 m arrival test, facing/conversation targets, and cleanup.
No static Lua/Forge mismatch remains in the affair trio. The air-scolding report is retained as the
sole runtime-only identity/rendered-model uncertainty, with probes ready. Current gates are 254/254,
117/117 without warnings, and SDK PASS. The game stayed closed and nothing was deployed.
Book Trader now has complete retail-byte coverage as well: Init `0x00DB3F00-0x00DB3F98` and Main
`0x00DB3FA0-0x00DB4F6A`, totaling 4,194 bytes / 1,243 instructions. Exact reconciliation confirms
home return, Theresa facing, hit reaction, all sweets-sale outcomes, timer-gated shouting, and movie/
control cleanup without a Lua or Forge mismatch. Current gates: 256/256 recovery tests, 117/117
fixtures without warnings, and SDK PASS. No deployment or launch occurred.
Barrel Thug now has complete retail-byte coverage: Init `0x00DB6BF0-0x00DB6C31` and Main
`0x00DB6C60-0x00DB7CF1`, totaling 4,306 bytes / 1,281 instructions. The artifact confirms intro,
follow, timer-tier temptation/well-done chatter, why-not-smash/outro selection, hit bad-deed 2, and
all movie/control cleanup. It also proves these lines are independent of Barrel Man's proximity-only
return judgment; no missing consequence handoff exists. Current gates: 258/258 recovery tests,
117/117 fixtures without warnings, SDK PASS. The game stayed closed and nothing was deployed.
Barrel Man now has complete retail-byte coverage: Init `0x00DB5260-0x00DB5307` and Main
`0x00DB5330-0x00DB6B23`, totaling 6,298 bytes / 1,808 instructions. Full phase coverage confirms
that return-time thanks is gated solely by sight or the 10 m distance fallback. `LETDOWN_BROKEN` is
only reachable on a later talk after the Hero first failed that return judgment and set
`HeroLetMeDown`; destruction never overrides nearby thanks. Current gates: 260/260 recovery tests,
117/117 fixtures without warnings, SDK PASS. No launch or deployment occurred.
Dead Father's control-acquisition termination is also now closed: Forge returns false after cleaning
its pending handle, the shared helper propagates it, and a fixture proves setup calls are skipped.
Its minimap marker is fully closed as well: native setup plus the PDB signature prove `(me,
HUD_ORB_QUEST_CORE)`, Forge preserves that order, and the entity inventory now has zero uncertainties.
Current offline totals after these additions are 116/116 fixtures and 147/147 recovery tests.
Victim Init identity is now closed too: retail allocator/vtable `0x012D87B8`, the two Victim field
writes, and the separate real Book Trader vtable/Init form a positive plus negative ownership proof.
The quest logbook helper family is now instruction-proven. `0x00CBE87F` builds
`TEXT_QST_LOG_STORY_<id>` and submits category 1; `0x00CBE960` builds `_NAME/_DESC` story keys;
`0x00CBE9EE`, called by both first-deed paths, builds `_TITLE`, submits tutorial category 2, and
yields once. New Oakvale now calls `AddLogbookTutorialEntry`, and Forge routes that API directly to
`0x00CBE9EE` instead of synthesizing an incorrect literal-wide entry. The corrected Release|x86 DLL
builds with zero warnings/errors and remains undeployed; SHA-256
`2B24EDDBA821F1D1F49FF8DE7B1ADE341FD03A4728A54648CBE4D42C96DDA5E2`. Current offline suite:
117/117 fixtures, 151/151 recovery tests, 137 API requirements with zero blockers, and 46/46
applicable functions traced with zero uncertainty. The dead-father movie-bracket note is also closed:
native order, Forge's per-VM ownership check, and the successful skip log jointly prove one borrowed
caller-owned movie sequence around the macro. New Oakvale's three platform branches now call Forge's
direct retail `IsXbox` binding instead of hardcoding PC; the no-context helper fallback remains false
for isolated tests.
Barrel destruction is now closed end-to-end too: the active entity's bound-thing alive predicate,
derived callback vtable, Forge one-shot dispatch, Lua state writes, and archived `bad=1` observation
all agree. A dedicated fixture proves the counterintuitive retail rule that a broken barrel does not
change automatic return judgment: an unseen hero within 10 m still receives thanks and a good deed.
Theresa's `given_chocs` lifetime is now instruction-audited: initialization, three writes, and three
reads all resolve to one Main-stack byte, separate from the persistent quest flag. Her inventory and
the Barrel inventory now have zero uncertainties; Barrel's post-instruction tail is an exact retail
idle-until-termination loop. Current offline suite: 117/117 fixtures and 152/152 recovery tests.
The game remained closed and nothing was deployed.
Barrel Man phase 1 is now donor/retail-audited as a preserved dead case: construction does not set
`MyPhase`, Init sets 0, and Main writes only 2, 3, 4, or 5. The cleanup-only marker locals are also
classified as exact compiler-visible behavior rather than uncertainty; only inferred enum labels remain.
Affair Wife's route mismatch is narrowed further: Forge's actual Lua registration uses the operand-aware
non-blocking movement wrapper (not its legacy four-argument helper), forwards retail `false,true`, and
donor `GetPos` proves Forge's direct implementation-vector read is equivalent to CScriptThing dispatch.
The remaining unknown is runtime husband identity/replacement or model-vs-thing position state.
Timer duration semantics are now native-proven: `WorldUpdate` decrements positive timer values when
`worldFrame % constantFPS == 0`, so `SetTimer(..., N)` represents N constant-FPS seconds and clamps
at zero. Created Beetle's 5-second lifetime and Villager's shared 3-second throttle are closed.
Villager's hit-control lifetime is now retail-exact too: successful hit speech retains control across
the loop's bottom frame, releases it at the next loop head before the hit test, and the termination
edge follows the native cleanup label. Its inventory now has zero uncertainties. Current offline gates
are 117/117 fixtures and 155/155 recovery tests. Created Beetle's `RemoveThing(true,true)` operands
are now behaviorally closed from the native callee: the first reaches `CThing::Kill` as
`destroyImmediately`, and the second sets the thing's `+0x93` flag bit `0x02` before that call.
Barrel Man's descriptive phase labels remain explicitly tagged as inferred metadata, not behavioral
uncertainty; all numeric states and transitions are exact. The only entity-inventory uncertainty now
is Affair Wife's runtime identity/position anomaly. Current recovery suite: 156/156. The game remained
closed and nothing was deployed.
The subsequent stale-evidence audit closed four more Forge paths: all 13 reconstructed random call
sites use the relocated retail MSVCR71 `rand` thunk; Dead Father forwards all seven native looping-
animation flags; Victim forwards both killability and all three information flags; and `FadeScreenOut`
supplies the exact opaque-black colour used by Barrel Man and Theresa. Barrel Man's preserved dead
phase now also reproduces retail's zero-vector fallback for a missing walk-off marker. Current offline
suite: 117/117 fixtures and 161/161 recovery tests, still with no deployment.
Guard's lecture approach threshold is now instruction-closed: the initial branch, loop-entry check,
and post-frame recheck each load `DAT_013ac840`, whose retail value is 3.0, before calling the same
distance helper. Remaining movie-cleanup “inference” notes are closed too: Forge binds and invokes the
exact retail derived movie-resource destructor at `0x006E7B80`. Current suite: 117/117 fixtures and
163/163 recovery tests.

Created Beetle's remaining decompiler omission is now instruction-closed: `RegisterTimer` returns into
`EAX`, retail retains that ID in `EDI`, and pushes `EDI` before both slot-`0x168` `GetTimer` calls.
Barrel Man's movement value 1 and excluded special-ability value 14 are also tied to the named
`ENTITY_MOVE_RUN` and `HERO_ABILITY_HEAL_LIFE_SPELL` enums in the retail-aligned Forge header.
Focused audits preserve both results. The obsolete unresolved-call scaffold and two duplicate
non-gap API classifications were subsequently removed. The manifest builder then stopped counting
`n/a` and `n/a (data)` state/data sentinels as APIs. Fifteen conceptual `SpeakAndWait` annotations
were then normalized to the actual registered `me:Speak` call used by their Lua helpers, leaving 132
actual direct/host-managed API
requirements with zero blockers. The inventory now preserves dual Forge registration too;
`SetIsPushableByHero`, used through both Quest and Entity Lua surfaces, is correctly marked
`Quest|Entity` instead of losing its Entity scope. `AddBadDeed` field propagation is byte-audited as well: retail
increments `BadDeedsPerformed` at `+0x58` and writes the caller's deed-kind byte at `+0xFC+kind` on
both control-flow tails, removing the final stale field-write inference labels. Current offline gates
are 117/117 fixtures and 170/170 recovery tests. Static FinalAlbion inventory now excludes an
authored duplicate husband: both canonical TNG trees contain exactly one `NOVI_AffairMan`, UID
`18446741874686306552`; the installed WAD-extracted payload is byte-identical to the canonical tree,
and his authored start is 75.765 m from the Wife. The sole remaining entity
uncertainty is therefore the Wife's staged runtime replacement/handle or rendered-model/thing-position
seam. The game remained closed and nothing was deployed.

The API manifest now carries a validator-derived executable Lua call surface. The validator scans
arbitrary receiver identifiers (closing omissions such as `marker:GetAngleXY`) while explicitly
excluding the package's one native Lua string-method use, `value:match`. Regeneration records 145
scoped calls: every call has a Forge binding, and every one of the 131 direct retail-operation
requirements appears in executable Lua. The zero-gap audit enforces both directions while retaining
13 visible helper/probe-only operations. Reconciliation is receiver-scope exact, so an operation
observed only on the wrong Quest/Entity surface cannot satisfy the requirement. Current offline gates
are 117/117 fixtures and 174/174
recovery tests; 132 total requirements remain (131 direct plus one host-managed), with zero blockers.
The DLL hash is unchanged and nothing was deployed.

API requirement schema 0.3 now separates 429 call-site evidence annotations into `evidenceNotes`.
They were previously mislabeled as `semanticDifferences`, despite documenting exact operands,
cleanup paths, and corrected decompiler artifacts. Direct bindings now truthfully report zero known
semantic deviations, and the zero-gap audit requires the annotations to remain preserved and separate.

Native provenance is now self-contained: the historical missing `tu/` locators were replaced with 50
fresh read-only exports from the local `FableTLC/Fable.exe` Ghidra project. They cover all 52
inventoried quest/entity functions (the empty default at `0x00CDEBB0` and quest Init at `0x00DAADD0`
are intentionally shared). Each export records its exact entry address and generator, and a focused
audit rejects missing, stale, misaddressed, or unreferenced files. Current recovery suite: 176/176.
Regenerate the corpus with
`powershell -File tools/script_recovery/export_new_oakvale_native_sources.ps1`; the wrapper always
uses read-only `-noanalysis` mode and derives the address set directly from the inventories.

The six not-applicable coverage rows are now classification-audited individually. In particular,
`NOVI_Villager.QuestInit(vectors)` is no longer mislabeled empty/data-only: it explicitly aliases
the implemented-and-traced `Q_NewOakValeIntro.Init` at the same `0x00DAADD0` entry. The old broad
`"vectors" in function-name` exclusion was removed, and the native-source audit requires the alias
to resolve to an identical address.

Transitive fixture coverage is no longer trusted from `covers` labels alone. Every secondary helper
claim now names distinctive events that must also be validator-enforced expectations: both
`GivenTeddy` helpers, `GetVillagerSpeechIndex`, `AttackStuff`, and `PostAttackStuff` are covered this
way. The builder rejects secondary coverage with absent or unverified evidence. Current offline totals
are 117/117 fixtures and 177/177 recovery tests, with the 46 applicable functions still traced.

Operation ownership is now complete and audited. The builder normalizes nine Barrel Man split-helper
labels such as `Main/judge_hero` back to native owner `Main`; previously those 48 helper-tagged rows
entered the API inventory but were silently omitted from function coverage. Barrel Man Main now
accounts for all 68 of its operations, and coverage assigns all 747 inventory operations exactly once.
The structure audit also requires contiguous sequence IDs and declared native owners. Current suite:
179/179 recovery tests.

Entity parent-state declarations are now checked against executable Lua instead of trusted blindly.
The audit found and added five omitted direct reads: Affair Wife, Book Trader, and Bully each read
`TalkIntermittentTimer`; Barrel Thug reads `WatchTimer`; Barrel Man reads `BadDeedsPerformed`.
It validates 30 directly read and 28 directly written field families against 37 canonical quest-state
keys, while recognizing the eight retail Villager speech vectors as static parent data. Annotated
indirect deed-helper writes remain visible and are name-validated. Current suite: 180/180 tests.
The same audit scans quest and shared modules too, covering 34 package-wide read families and 32
write families, so undefined keys outside entity files cannot evade the check.

Master-data ownership is generated rather than hardcoded now. The prior persistence manifest falsely
said `TeddySolution` was written by Teddy Girl and Bully; native inventories and Lua prove both B and C
writes belong only to Teddy Girl. Persistence schema 0.2 emits the two structured, address/evidence-
backed accesses, and the state audit cross-checks `F.master` definitions, Lua calls, and inventory ops.
Annotated deed declarations are normalized before persistence ownership aggregation, restoring six
indirect writers for both `BadDeedsPerformed` and `WhichBadDeedsPerformed`. The scalar-only parser
previously omitted the latter array entirely; it is now represented as the canonical `bool[5]`
family at `0xFC..0x100`, with Guard as reader. The manifest therefore contains all 37 field families.
Current recovery suite: 182/182.
Executable `Deeds.add_good`/`add_bad` calls are also ownership-audited. Six entity inventories had
omitted those indirect side effects; after repair, `GoodDeedsPerformed` has four real entity writers,
while `BadDeedsPerformed` and `WhichBadDeedsPerformed` each have all twelve. The persistence manifest
now derives these owners from normalized declarations instead of losing helper annotations.

Villager speech data is now three-way exact: all eight vectors, their offsets/order, and all 42 text
keys match between the fresh native quest-Init decompile, structured Villager inventory, and evaluated
`villager_speech.lua`. A focused audit guards vector lengths (6/6/4/5 per sex), retail construction
order, and set equality. Current offline suite: 117/117 fixtures and 183/183 tests.

Package-wide symbolic resources are now inventory-audited too. Across text, object, creature, quest,
cutscene, animation, marker, region, theme, music, and script identifiers, all 238 executable Lua
resources are covered: 224 exact identifiers plus 14 indexed/sex-suffixed construction prefixes.
Eighteen diagnostic log formats are explicitly excluded, and annotated legacy inventory strings are
normalized to their literal identifier. Current recovery suite: 184/184.

Entity-local initialization is independently audited now. All 32 native `localFields` names occur in
their comment-free reconstructed entity Lua and every recorded `initValue` is assigned. The evaluator
also resolves 14 symbolic defaults, including Barrel Man's `MyPhase = PHASE.AT_WAREHOUSE`, rather than
accepting only duplicated literals. Current recovery suite: 185/185.

Native numeric/boolean/table constant fidelity is package-audited too. The inventory contains 214
constant records: 200 have same-name Lua declarations whose parsed values match exactly, while 14
split, shared-module, symbolic-vector, or call-site records have explicit value and executable-context
checks. This includes the affair animation flag, zero-vector fallbacks, Barrel Thug's asymmetric final
timer tier, Bully info-bar colours, Guard's chase-distance table, the `-999` GUI sentinel, and the
shared `0.001` morality delta. Current recovery suite: 186/186.

Cleanup/lifetime evidence is structurally guarded across all 17 inventories that own resources. The
73 cleanup records contain 149 acquisition operation references; every referenced sequence exists in
its owning inventory, and non-operation acquisitions must explicitly identify quest/external ownership.
Release/missing-path lists and owner-only records are schema-checked as well. Current recovery suite:
187/187.

Control-flow phase metadata is now checked against executable reconstruction anchors. All 74 phase
records across 16 inventories have unique IDs, non-empty entry/exit evidence, and resolve to 100 real
Lua function declarations. Barrel Man's six numbered phase IDs are additionally cross-checked against
both the native `enumValues` metadata and the Lua `PHASE` table. Current recovery suite: 188/188.

Cutscene and spawned-thread metadata is linked back to executable Lua now. The audit covers all 18
cutscene records, nine named resources, 13 actor-map keys, and four quest child threads. It exposed
stale thread prose that omitted Forge's required `{ region = "" }` options and used the wrong receiver
case; all four inventory rows now record the exact executable `Quest:CreateThread` calls. Current
recovery suite: 189/189.

Native entity-class ownership now has reproducible retail-vtable proof. Scanning the installed
executable for each entity inventory's unique `[Main, Init]` pair recovered all 16 entity vtables
(10 were previously absent from the inventories), plus their destructor slots. The SHA-pinned
snapshot, extractor, and audit cross-check all 32 function slots, allocator/vtable evidence in the
translation unit, and the 32 exported native decompiles. This also replaces four stale references to
a nonexistent `entity_vtables.json`. Current recovery suite: 190/190.

Those vtables now anchor a complete entity-layout pass. All 16 entity allocators (the quest object is
constructed separately) are resolved in the native translation unit, with retail allocation sizes of
28-44 bytes. Each allocator installs the expected vtable, and all 31 class-local fields fit their
recorded offsets and widths; the separate Theresa stack local is correctly excluded. The checked
layout snapshot retains 11 aggregate bytes of legitimate tail padding/slack. Current recovery suite:
191/191.

All 16 entity destructor slots are now fresh read-only Ghidra exports. Their normalized native bodies
are identical: invoke the shared entity base cleanup, conditionally `operator_delete(this)` for the
vector-deleting flag, then return `this`. This export also disproved an over-broad first pass: the
quest's adjacent `[Main, Init]` pair has a spawned-function factory in the preceding slot, so it is not
an entity-layout vtable. The false quest vtable/destructor claim was removed and both extractors are
now entity-scoped. Current recovery suite: 193/193.

All 16 allocator functions are now standalone, snapshot-driven Ghidra exports too. The audit verifies
each exact allocation size, construction of the embedded `CScriptThing`, installation of the entity's
retail vtable, parent and bound-thing pointer stores, and allocation/wiring of the `0xC` ownership
control block. This makes the layout proof reproducible from both the translation-unit corpus and fresh
function-level sources. Current recovery suite: 195/195.

The common destructor call is now classified from its body instead of Ghidra's BSim donor label. All
16 deleting destructors converge on `0x00F35B40`; that routine restores base vtable `0x012C3224`,
restores the embedded `CScriptThing` vtable `0x01238C8C`, releases and clears its counted binding, and
tails into exact/relocation-matched `CBase_RestoreVTable` at `0x0099A2E0`. Its semantic role is therefore
entity-binding base cleanup, while the exact native class name remains unresolved. The snapshot-driven
export and audit preserve the misleading `NUISystem::CFrontEndScreen` name only as donor provenance,
not recovered identity. Current recovery suite: 195/195.

The remaining Affair Wife route uncertainty now has a complete position-chain proof. Fresh exports of
retail `CScriptThing::GetPos` (`0x004AA980`), `CGameScriptThing::GetPos` (`0x008CFE20`), and
`IsDistanceBetweenThingsUnder` (`0x00CBE2FF`) show wrapper validation, implementation-slot dispatch,
inline `+0x28` position storage, and the exact squared 3D comparison. Forge normalizes raw/shared Lua
objects and invokes that retail fastcall, while `Entity:GetPos` uses the same implementation dispatch.
The observed air-scolding is consequently limited to runtime replacement/handle or rendered-model
position state, not reconstructed distance arithmetic or ABI. Current recovery suite: 197/197.

Quest-object ownership is now independently guarded rather than inferred from the entity-vtable pass.
The native cluster identifies allocator `0x00DBEF70`, constructor `0x00DAAC00`, the 268-byte quest
object, and vtable `0x012D7A28`; its five slots are destructor, `RegisterMain`, `Main`, `Init`, and
`OnPersist`. Eight fresh function exports, including the shared base constructor, verify allocation/constructor wiring, interface/database
stores, vtable installation, state-vector initialization, both timers, and the spawned `Main`
registration. The quest inventory now records these facts directly. Current recovery suite: 199/199.

The recovered quest size is now reconciled against the full persistence schema. A contiguous six-region
layout accounts for all 268 bytes: inherited script base, named scalar state, eight 12-byte villager
speech vectors, the five-byte bad-deed-kind array, three alignment bytes, and two timer handles. All 37
named field families (93 occupied bytes) fit their native widths and offsets without overlap; the base
and derived constructor exports guard the boundary anchors. Current recovery suite: 200/200.

Initialization provenance is now exact rather than summarized as “everything resets in Init.” Native
`Init` explicitly stores 27 of 37 field families: 25 ordinary Lua resets, the talk timer, and the
five-byte deed array. Ten fields are producer-initialized instead. This corrected false zero/default
claims for `StopTimeIndex`, unused `DadOfferedRewards`, and `lastVillagerSpeechIdx`; none was actually
in the Lua reset lists, so executable behavior is unchanged. Current recovery suite: 201/201.

Quest teardown is now reconciled with that construction/layout proof. The deleting thunk at
`0x00DBEFA0` calls the real quest destructor body at `0x00DBEFC0`; despite its stale particle-emitter
donor name, the body deregisters timer handles `+0x108` then `+0x104`, destroys and frees all eight
speech vectors from `+0xF0` back through `+0x9C`, and finally invokes inherited cleanup at
`0x00CBD510`. That final routine's `DeleteAllParticles` label is also misleading: its body destroys
the inherited script vectors/list at `+0x30`, `+0x18`, `+0x08`, and `+0x04`, then restores the root
base vtable. Fresh read-only exports, a teardown snapshot, and a layout/IR-aware audit now guard the
complete chain. The inherited class identity is now proven as `CScriptBase`, not merely compatible:
FSE independently maps constructor `0x00CB8110` to `CScriptBase_Construct`, it initializes the exact
four storage regions released by `0x00CBD510`, and all 125 consumers of that cleanup in helper IR are
script destructor roles. Current recovery suite: 202/202.

The eight embedded villager-speech vectors now have one cross-layer lifecycle audit. It extracts all
42 text keys and their vector end-pointer offsets directly from native `Init` `0x00DAADD0`, maps them
to the eight named inventory categories, expands the Lua prefix/suffix tables, verifies retail's
non-layout initialization order, proves exact contiguous coverage of `+0x9C..+0xFB`, and requires
the destructor to visit the same vectors in reverse layout order. This prevents a category, suffix,
offset, or teardown change from passing through independently green metadata. Current recovery suite:
203/203.

Quest lifecycle slots are now derived from retail PE bytes rather than accepted circularly from the
native cluster. A dedicated exporter reads five pointers at vtable `0x012D7A28`, records the exact
20-byte table hash, and resolves the deleting destructor's relative call. The resulting addresses are
`0x00DBEFA0`, `0x00DAACE0`, `0x00DABAC0`, `0x00DAADD0`, and `0x00DAADA0`, with implementation
target `0x00DBEFC0`; they independently match destructor, RegisterMain, Main, Init, and OnPersist.
The construction audit compares this snapshot with the cluster and requires the same retail SHA-256
used by the entity-vtable snapshot. An installed-image reproduction test is active when retail
`Fable.exe` is present. Current recovery suite: 204/204.

Quest ownership is now independently decoded from retail x86 instructions as well. A Capstone-backed
PE exporter proves allocator `0x00DBEF70` pushes `0x10C`, calls the retail `operator_new` thunk at
`0x00BFEA1A`, and calls constructor `0x00DAAC00`. The constructor's first direct call targets the
independently named `CScriptBase_Construct` at `0x00CB8110`, then its immediate vptr store installs
`0x012D7A28`. Exact allocator/constructor byte hashes and the common retail executable hash are
stored; the construction audit and installed-image reproduction test guard the complete chain.
Current recovery suite: 205/205.

The two quest timers now have a single retail-to-Forge lifecycle audit, focused on the runtime bug
where Lua's truthy integer zero skipped registration. Retail constructor `0x00DAAC00` calls interface
slot `0x15C` twice and stores TalkIntermittentTimer at `+0x104` before WatchTimer at `+0x108`; derived
destructor `0x00DBEFC0` calls slot `0x160` in reverse order. The authoritative vtable identifies the
contiguous slots as RegisterTimer, DeregisterTimer, SetTimer, and GetTimer (`0x15C..0x168`), and Forge
binds indices 87..90 with matching signatures and Lua exposure. The reconstruction is required to
register both handles unconditionally in retail order, use nil metadata sentinels, and zero only the
talk timer during Init. Current recovery suite: 206/206.

Those timer and initialization claims are now independently reproducible from the installed retail PE,
not only checked against Ghidra text. Capstone-backed exporters record exact hashes and instruction
addresses for both `RegisterTimer` calls and result stores, both reverse-order `DeregisterTimer` calls,
all 27 scalar `Init` writes with their byte/dword widths, and `SetTimer(+0x104, 0)`. The cross-layer
audits consume the checked-in snapshots, while installed-image tests regenerate them when `Fable.exe`
is available. Current recovery suite: 208/208.

Quest persistence now has the same direct-byte proof. Retail `OnPersist` at `0x00DAADA0` is exactly 33
bytes and performs one transfer only: key `AttackOver` from `0x012D7A58`, one-byte field `+0x50`,
one-byte false default, through `CPersistContext::Transfer<signed char>` at `0x004045C0`. Forge binds
that exact target as its bool transfer, preserves one-byte value/default storage, and Lua transfers only
`AttackOver`; the manifest agrees that it is the sole save-surviving quest field. Current recovery suite:
210/210.

`RegisterMain` now has direct retail-byte coverage too. Its 139-byte body allocates a `0x3C`
`CSpawnedFunc`, constructs it as `"Main"` through `0x00CDD450`, installs quest-specific vtable
`0x012D7A3C`, callback `0x00CDD440`, and owner at `+0x38`, then registers it with the empty section
through `0x00CB7E50`. Forge already used the matching constructor/registration targets and layout; three
new compile-time assertions now pin its size and callback/owner offsets. Release x86 builds with zero
warnings and errors. Current recovery suite: 212/212.

The 1,670-byte entity-binding prefix of quest `Main` is now decoded directly from the retail image.
All 16 binding names appear in exact Lua order and every callback matches the independently exported
entity allocator, from `NOVI_LiveFather -> 0x00DAC2C0` through `OVI_DeadFather -> 0x00DB81B0`.
Retail performs 16 `0x1C` allocations and 16 registrations through `0x00CB8230`; Forge uses that exact
target and object shape. Five new compile-time assertions pin the parent, allocator, enabled, and
auxiliary fields as well as total size. Release x86 builds cleanly. Current recovery suite: 214/214.

Quest `Main` now has complete direct-byte coverage through its 348-byte post-binding tail as well.
After base activation `0x00CB8930` and interface slot `0x100`, retail tests one-byte `AttackOver`,
checks termination through `0x00CB7940`, optionally deactivates `Q__OakValeIntro_PostAttack` through
slot `0x460`, sets objective 01 through slots `0xA3C/0x4A0`, constructs `StartBarrelTimer` with callback
`0x00DB4F70`, then calls `DoMission` at `0x00DBDE40`. The Lua order and Forge targets/slots agree;
the obsolete inventory note claiming Forge lacked the termination query was removed. Current recovery
suite: 216/216.

The core `DoMission` state machine now has an exact 1,172-byte retail snapshot and phase audit. It
extracts the seven embedded region/resource/thread names, callbacks for `WatchBarrels` `0x00DBE890`,
`WatchForGotGold` `0x00DBE2E0`, and `ManageQuestCoreMarkers` `0x00DBE4E0`, all three one-byte
`AttackOver` reads, six calls to termination helper `0x00CB7940`, and the final ordered calls to
`AttackStuff` then `PostAttackStuff`. All 20 distinct interface slots used by the method are executable
in the authoritative catalog, and Lua preserves the same phase topology. Current recovery suite:
218/218.

The complete 285-byte `AttackStuff` transition is now independently decoded too. Retail activates
`Q__OakValeIntro_PostAttack`, immediately deactivates `Q_NewOakValeIntro_PreAttack`, sets time to exact
float `23.0`, transitions to `ENVIRONMENT_OV_POSTATTACK` with zero seconds, and installs objective 06.
Its six ordered interface slots are `0x450`, `0x460`, `0xA18`, `0xA40`, `0xA3C`, and `0x4A0`; all
are executable and the Lua call order/constants match. Current recovery suite: 220/220.

The adjacent 1,095-byte `PostAttackStuff` method now has direct retail-byte coverage from entry through
its final cleanup. The snapshot proves the two-stage `M_PostAttackStart` wait, music sets 45 and 57,
hero teleport, Oakvale limbo, hidden money bag, teddy removal, logbook entry 20, camera/fade sequence,
two 5.0-distance checks against `MK_OVI_DADTRIGGER`, byte store `DadFound=1` at quest offset `+0x51`,
priority-4 hero acquisition, `CS_OAKVALEINTRO_HESDEADJIM`, and final village/time/section/theme/music
restoration. All reconstructed Lua operations retain retail order. The general slot catalog omits
`0x5EC`, so the audit independently requires Forge's exact index-379 `PauseAllNonScriptedEntities`
binding while validating the other 18 interface slots against the catalog. Current recovery suite:
222/222.

`StartBarrelTimer` now has an exact 562-byte retail snapshot covering all 163 instructions. It proves
three reads of WatchTimer `+0x108`, the returned HUD id store at GUIBarrelCounter `+0x60`, two byte
tests of BarrelManSpokenToHeroOnReturn `+0x73`, and the complete `AddQuestInfoBar(45.0, 0.0,
opaque-green, opaque-green, HUD_CLOCK_ICON, "", 1.0)` call. The loop uses exact distance 2.0,
green-inside/red-outside colors, and `UpdateQuestInfoBar(id, GetTimer(id), -1.0, -1.0)`, removing the
element only on normal return. Forge's slots `0x510/0x530/0x534/0x548` and their signatures match the
retail call shapes, and Lua preserves all 11 ordered operations. Current recovery suite: 224/224.

The full 647-byte `WatchBarrels` consequence thread is now independently decoded. Retail waits for at
least one `NOVI_Barrel`, clears byte field BarrelBrokenInstantaneous `+0x74`, exits on AttackOver
`+0x50`, and consumes each break by incrementing a local counter and clearing `+0x74` again. Counter 1
directly calls quest `AddBadDeed(0)` at `0x00DAEA70`; counter `total-1` inserts `OBJECT_GOLD_1`, while
`broken > total-4` creates a named stag beetle at BarrelBrokenPos `+0x76` and sets max/current health
to 2.0. Lua preserves all 15 ordered operations. Slots `0x428/0x924`, absent from the general catalog,
are independently pinned by Forge's exact `EntitySetMaxHealth` and `AddItemToContainer` bindings.
Current recovery suite: 226/226.

The barrel man's consequence split is now backed by two direct retail-byte snapshots rather than only
Ghidra text: the 934-byte automatic judgment region and the 653-byte phase-5 follow-up region. Retail
sets BarrelManSpokenToHeroOnReturn before a short-circuit visibility-or-10.0m test. A detected hero gets
`TEXT_QST_048_BARRELMAN_THANKS` plus `AddGoodDeed`; an absent hero gets `WHERE_GONE`, sets local
HeroLetMeDown `+0x1D`, displays `LEFT_WAREHOUSE_UNATTENDED`, and calls `AddBadDeed(1)`. The automatic
region never reads BarrelBrokenPersistent. Only later phase-5 dialogue reads it at parent `+0x75`, and
only when HeroLetMeDown is already true, selecting `LETDOWN_BROKEN`; otherwise a hero who stayed gets
`NO_TIME`. Lua preserves this counterintuitive retail behavior exactly. Current recovery suite: 228/228.

The compact `WatchForGotGold` thread now has direct coverage of all 212 meaningful bytes / 65
instructions before its alignment padding. Retail polls interface slot `0x1FC` until hero gold is
strictly greater than 2, checks termination both during and after the wait, then calls active-quest-name
slot `0xA3C` and objective slot `0x4A0` with `TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03` and two empty
trailing strings. Lua matches these operands exactly. The general catalog omits `0x1FC`, so the audit
requires Forge's exact index-127 `GetHeroGold` binding as independent evidence. Current recovery suite:
230/230.

`ManageQuestCoreMarkers`, the final quest-level spawned watcher, now has an exact 944-byte / 287-
instruction retail snapshot. It resolves the three entity lookups, all seven ordered marker mutations
(add father, remove father/add trader, remove trader/add Theresa, remove Theresa/add father), gold
minimum 3, player-control wait, tutorial 19 and dismissal wait, byte reads of GivenSweets `+0x94` and
GivenTheresaChocs `+0x95`, and all 11 termination checks. Lua preserves all 16 audited operations and
Forge supplies the already-proven `GetHeroGold` fallback for catalog-omitted slot `0x1FC`. Current
recovery suite: 232/232.

The Guard repeat-lecture path now has 4,020 bytes of direct retail coverage: a 639-byte claim/recheck
region and the complete 3,381-byte first/repeat lecture selection and bodies. Retail compares
`BadDeedsPerformed - GuardsDealtWithBadDeeds > 0`, repeats that test after the chase, then copies the
current bad-deed count into the dealt count before speaking. `GuardsSpokenOnce` selects the six-line
first lecture or `TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN`, crime list, and `AFTER_READ_LIST`; the first
path stores `GuardsSpokenOnce=1`. Lua already preserves this flow, and archived runtime evidence has
observed distinct claims for counts 1 and 2 plus both speech branches. The reported one-scold behavior
was therefore not a remaining retail-parity error in the current reconstruction. Current recovery
suite: 234/234.

The Affair Wife run-to-husband transition now has a reproducible 411-byte / 118-instruction retail
snapshot. It proves the single `MoveToPosition` call snapshots the husband's current position with
radius 2.0, run type 1, and trailing flags false/true. Both the initial and loop arrival tests instead
use the same live `NOVI_AffairMan` script handle at exact distance 3.0; only the true branch clears
commands and disables movement-in-actions before arguing. The optional running line triggers after
10.0 from home and stores `SaidRunningLine=1`. Lua and Forge's native position/distance chain match.
No captured real-game ROUTE_START/ROUTE_REACHED identity/position lines exist yet, so the previously
observed air-scolding remains narrowed to runtime replacement/handle or rendered-model divergence,
not the reconstructed route operands. Current recovery suite: 236/236.

The adjacent Affair Wife argument loop now has complete direct coverage through its loop-back edge:
1,770 bytes / 536 instructions at `0x00DB35C1-0x00DB3CAA`. Retail gates animation on the hero being
within 15.0, then passes the same husband wrapper local to all three facing calls (during pointing,
after a hit response, and after hero talk). It also adds that wrapper to each argument conversation,
uses it as the numbered wife-line listener, and as the optional `AFFAIRMAN_IN_TROUBLE` speaker. Lua
matches every role and Forge's facing/conversation bindings already match the retail call shapes.
Together with the route snapshot, this excludes a reconstructed recipient or position-snapshot mixup;
the observed air-scolding requires real-game identity/position probe evidence to resolve further.
Current recovery suite: 238/238.

The blue barrel-timer report is now traced through four concrete retail HUD functions. `AddBar`
resolves texture and text symbols separately, then its `0x58`-byte element stores the two mutable
colors at `+0x28/+0x2C` and the two sprite ids at `+0x30/+0x34`. `ChangeBarColour` writes only the
color fields. Forge passes exact BGRA bytes and the timer supplies green/green initially, then
green/green or red/red by proximity. Thus channel marshalling cannot explain blue (green is invariant
under R/B swapping), and a blue `HUD_CLOCK_ICON` is independent of the correctly colored bar fill.
No evidence-backed code change is warranted unless runtime inspection specifically shows the fill,
rather than the clock artwork, retaining blue. Current recovery suite: 240/240.

The Bully health bar now provides an asymmetric direct-byte control for that result. Two snapshots
cover 541 bytes / 145 instructions across creation/update and subdued removal. Retail constructs
primary bytes `00 FF 00 FF` (green) and secondary bytes `FF 00 00 FF` (blue), calls slot `0x510`,
stores the handle at parent `+0x64`, updates current to `InitialHealth-HitsTaken` through `0x530` with
max/scale `-1.0`, then sets `BullySubdued +0x6C` and removes that same handle through `0x548`. Lua
matches the complete lifecycle. This independently confirms Forge does not collapse or swap the two
color operands. Current recovery suite: 242/242.

The Bully subdued/run-off transition now has continuous direct-byte coverage from the final-hit
branch through the function epilogue: 1,161 bytes / 339 instructions at
`0x00DBC86B-0x00DBCCF4`. Retail acquires Bully, Hero, and victim at priority 4; builds the exact
`HERO`/`BRAT`/`BULLY` actor map; substitutes `$BRATLINE` from `HeroAttackedVictim`; and passes that
input map only to `BULLYRUN1`. `GivenHeroTeddy` selects `BULLYRUN2` plus victim-information clearing
and the state write, or `BULLYRUNDUMMY`. Normal teardown disables camera/pause, destroys the movie
and both maps, releases victim then Hero, sets `BullyRanOff`, awards the good deed, removes the Bully
with `(false,true)`, and finally releases it. Lua matches the entire transition. Current recovery
suite: 244/244.

The Guard now has complete direct retail-byte coverage. Init `0x00DAC650-0x00DAC760` contributes
272 bytes / 92 instructions and Main `0x00DAC760-0x00DADE4B` contributes 5,867 bytes / 1,791
instructions, for 6,139 bytes / 1,883 instructions through exact terminal `ret` boundaries. The
artifact covers the deed claim/recheck, chase, first and repeat lectures, hit response, crime list,
and terminal cleanup. Retail claims the entire positive
`BadDeedsPerformed - GuardsDealtWithBadDeeds` delta and then copies the current bad-deed count into
the dealt count; consequently one scold for one destruction deed is correct, while a later new deed
can trigger `CAUGHT_YOU_AGAIN`. Lua already matches this lifecycle. Current recovery suite: 262/262;
package fixtures: 117/117 with zero warnings/errors; SDK validation passes. Fable remained closed and
nothing was deployed.

The Barrel entity now has complete lifecycle coverage: shared empty Init `0x00CDEBB0-0x00CDEBB1`,
OnPredicateFail `0x00DB7DB0-0x00DB7DE1`, and Main `0x00DB7E10-0x00DB7FEC`, totaling 526 bytes /
164 instructions through exact terminal `ret` boundaries. This verifies the destruction callback's
instantaneous/persistent flags and position copy as well as the instruction/idle loop. Separately,
TLC `WatchBarrels` proves that break `total-1` inserts `OBJECT_GOLD_1` into the remaining barrel, so
destroying the final barrel should release the coin. Lua matches that branch. Because the user did
not observe a coin, Forge now has undeployed `AddItemToContainer` diagnostics logging item and exact
container identity around the native call; Release|x86 builds cleanly with SHA-256
`AE55FE0D138FE3E529852E54941E66C89517541A9E3E7A89CBB535AB3D49DF3C`. Current recovery suite:
264/264. The game remained closed.

CreatedBeetle now has complete direct coverage: shared empty Init `0x00CDEBB0-0x00CDEBB1` and Main
`0x00DB80C0-0x00DB81A8`, totaling 233 bytes / 80 instructions. The snapshot proves the five-second
timer, same-ID polling, `(true,true)` self-removal, and timer deregistration on both normal and
termination exits. Lua and inventory match without a behavior change. Recovery tests are 266/266;
package fixtures remain 117/117 with zero warnings/errors; SDK validation passes. Fable remained
closed and nothing was deployed.

TeddyGirl now has complete direct coverage across Init `0x00DAF000-0x00DAF052`, Main
`0x00DAF080-0x00DB0600`, and GivenTeddy `0x00DB0600-0x00DB065B`: 5,677 bytes / 1,686
instructions. Direct decoding exposed and corrected a metadata seam: `translation_unit.json` reports
Main size 5,498, which stops inside the final `add esp,0x158`; the executable's exact terminal `ret`
is at `0x00DB05FF`, making Main 5,504 bytes. The full artifact covers both good/bad deed outcomes,
TeddySolution B/C, ruined-teddy walk-off, hit response, all conversations, and cleanup paths. Lua
matches without a behavior edit. Recovery tests are 268/268 and package fixtures remain 117/117
with zero warnings/errors. The game remained closed and nothing was deployed.

The user-provided Anniversary footage (`4IkkNZxW0MQ`, inspected 7:40-9:40) visibly enters the
unattended-return failure and then plays Barrel Man's broken-stock rebuke: `A fat lot of good you
were`, the damaged-stock line, and the threat. New Oakvale now also handles the distinct reported
nearby-plus-broken case: `BarrelBrokenPersistent` suppresses `THANKS` and the good deed and selects
`LETDOWN_BROKEN`, without incorrectly adding the separate unattended-warehouse bad deed. The
generated trace proves one broken rebuke, zero thanks, and no good/bad deed counter write in this
compatibility branch. TLC's differing automatic-return bytes remain preserved in the retail snapshot
and audit. Evidence note: `runtime_evidence/anniversary-barrel-reference-20260910.md`. Recovery tests
are 271/271; package fixtures are 117/117 with zero warnings/errors; SDK validation passes. This
change is not deployed and the game remains closed.

Victim now has complete retail coverage across Init `0x00DAEEB0-0x00DAEF4C` and Main
`0x00DBCD60-0x00DBDE35`: 4,465 bytes / 1,328 instructions. This exposed the same six-byte generated
metadata truncation pattern as TeddyGirl: the reported 4,303-byte Main stops inside its final stack
adjustment, while the exact terminal `ret` is at `0x00DBDE34`. The snapshot covers all Bully shared
state, first/repeat hit outcomes, bad deed, BRATHIT cutscene, post-run thanks setup, and teddy-loss
complaint. Lua and inventory match. The evidence is included in the 271/271 recovery result above.

LiveFather now has complete retail coverage across Init `0x00DAC390-0x00DAC41D` and Main
`0x00DB86B0-0x00DB9795`: 4,466 bytes / 1,352 instructions. This again corrects a generated Main
boundary truncated by six epilogue bytes. Coverage includes the intro movie, deed counter, all mixed
good/bad reward dialogue, gold/chocolate handoff, attack response, and childhood completion state.
Lua and inventory match without a behavior correction. Recovery tests are 273/273; package fixtures
remain 117/117 with zero warnings/errors. Fable remained closed and nothing was deployed.

Theresa, Villager, and Bully now close the remaining entity-level byte gaps. Theresa covers 7,155
bytes / 1,917 instructions across Init/Main, including the chocolate morality choice and raid outro.
Villager covers 2,978 bytes / 900 instructions across Init/Main/GetVillagerSpeechIndex, including all
sex/deed speech matrices and nonrepeating selection. Bully covers 6,899 bytes / 2,002 instructions
across Init/Main/GivenTeddy, subsuming the teddy, health-bar, and run-off focused snapshots. Direct
decoding corrected Theresa's truncated Main epilogue and Bully's nine-byte Main truncation. Every one
of the 15 `NOVI_*` entity inventories now cites a complete reproducible lifecycle/dispatcher
snapshot. Recovery tests are 279/279; package fixtures remain 117/117 with zero warnings/errors; SDK
validation passes. Fable remained closed and nothing was deployed.

A deployment-ready but undeployed test payload is staged at
`work/new_oakvale_test_bundle_20260910/`: the 23-file FSE tree plus the diagnostic Release|x86 DLL.
Its manifest records 24 payload files, all hashes reverify, and explicitly records
`deploymentPerformed=false`. DLL SHA-256 is
`AE55FE0D138FE3E529852E54941E66C89517541A9E3E7A89CBB535AB3D49DF3C`. Test focus is the corrected
Barrel Man rebuke, final-barrel gold delivery logs, Bully marker completion, and Affair Wife route
identity. The all-entity snapshot gate raises the recovery suite to 280/280.

The quest object itself now has complete reproducible coverage: 15 exact regions totaling 11,838
bytes / 3,641 instructions across RegisterMain, OnPersist, Init, Main, AddGoodDeed/AddBadDeed,
StartBarrelTimer, DoMission, WatchForGotGold, AttackStuff, ManageQuestCoreMarkers, WatchBarrels,
PostAttackStuff, and both destructor layers. Direct decoding proves quest Main continues 18 bytes
beyond the generated size through its second cleanup epilogue at `0x00DAC2B3`; the destructor
implementation ends in the retail tail jump to `CScriptBase::~CScriptBase` at `0x00CBD510`.
Recovery tests are 282/282; package fixtures are 117/117 without warnings/errors; SDK validation
passes. Fable remained closed and nothing was deployed.

The package-wide coverage gate caught the non-`NOVI_` outlier `OVI_DeadFather`, which the earlier
entity-name filter omitted. It now has a complete 635-byte / 207-instruction lifecycle snapshot over
OnPredicateFail, Init, and Main, including marker/pose setup, DadFound transition, and control
cleanup; direct decoding corrects its truncated Main epilogue through `0x00DB8515`. The final native
coverage certificate is therefore 17/17 inventories (15 `NOVI_*`, `OVI_DeadFather`, and the quest),
78,891 exact retail bytes, and 23,388 instructions, all from retail executable SHA-256
`41DC91090AE853715AC06D2E9FC96E5D545381D197ED55D624C642F34509AC10`. Recovery tests are 285/285;
package fixtures remain 117/117; SDK validation passes. No deployment or game launch occurred.

`ExportScriptTranslationUnit.java` is upgraded to schema 0.2 after the boundary audit exposed an
ambiguity in its legacy `size`: `Function.getBody().getNumAddresses()` counts addresses across a
possibly disjoint Ghidra body and is not a contiguous end offset. Future exports retain `size` as a
compatibility alias but also emit `bodyAddressCount`, min/max/end-exclusive, `bodyExtent`, and every
body range. A regression test locks these fields. Recovery tests are 286/286.

The translation unit has now been regenerated read-only from the Ghidra retail database using the
original `0x00DAA000-0x00DBF100` range and definition list. The checked-in artifact is schema 0.2,
retains the same 100 function entries, and exposes the real disjoint-body topology (for example,
Bully Main is 6,630 body addresses across a 6,639-byte extent and four ranges). A regression test
now locks the generated artifact itself, not only the Java exporter.

Final-gold diagnostics now bracket the penultimate-break insertion at both layers. Lua logs stable
`GOLD_ARM broken=<n> total=<n> container_present=<bool>` and `GOLD_INSERT_RETURNED`; Forge logs the
item plus exact native container identity around slot `0x924`. The fixture proves break 5 of 6,
container present, and correctly ordered return. An initial Lua pointer string was rejected because
it made traces nondeterministic; two consecutive package validations now pass identically. The
current undeployed payload is `work/new_oakvale_test_bundle_20260910_v3/`. Recovery tests are 287/287
and package fixtures are 117/117 without warnings/errors.

`analyze_new_oakvale_targeted_playtest.py` now converts a Forge log into a structured outcome matrix
for Barrel Man rebuke/contradictory thanks, gold branch/container/native call, Bully compatibility,
and Affair Wife route start/reached. Running it against the archived failing log (SHA-256 `39519E...`)
correctly reports old thanks=true and every corrected/new probe=false; the result is preserved as
`runtime_evidence/analysis-interactive-20260910-targets.json`. This establishes an automated before/
after comparison for the v3 payload. `audit_final_barrel_gold_abi.py` now independently certifies
the reward path from WatchBarrels callsite `0x00DBEA2F`, through retail vtable slot `0x924` / index
585 and native target `0x0089E780`, into Forge's exact container-before-item ABI and the Lua call.
The installed retail EXE resolves the slot exactly and matches SHA-256 `41DC9109...AC10`. Recovery
tests are 289/289. The game remained closed and nothing was deployed.

The coherent undeployed payload is now `work/new_oakvale_test_bundle_20260910_v5/`: 24 hashed files,
DLL SHA-256 `1250BBCC...9268`, and `deploymentPerformed=false`. Two consecutive package validations
produced identical reports (117/117 fixtures, zero warnings/errors). Affair Wife retail route and
native position-chain audits, Bully run-off bytes, final-gold ABI, and tooling SDK validation all
pass; SDK mirrors were intentionally skipped because the noncanonical repositories remain stale.
Forge's named-thing diagnostic now records the retail world UID and wrapper-vtable position. The
targeted log analyzer is schema 0.2 and associates the last husband lookup with Wife route start,
distinguishing a replaced world object from a rendered-position fault in the next capture.

`analyze_bully_run_variance.py` proves the native Bully macro is nondeterministic across the five
archived captures: 1 completes at `MK_OIBR_BULLY2`, while 4 stop at the identical point 11.759 m
short after only 2.326-2.365 m. Retail `RunCutsceneMacro_Func` parses the authored false wait operand
and polls scripted-resource slot `0x68` (`IsPerformingScriptTask`), so the following 0.5-second pause
is not the intended wait. This narrows the defect to task/path completion and supports the existing
same-marker compatibility move rather than an ABI or cutscene-flag rewrite. Recovery is 291/291.

The Barrel Man outcome is now certified against the installed localized asset, not only symbolic
headers and video transcription. `barrel_man_text_assets.json` reproduces from English `text.big`
(SHA-256 `531C4514...33FC`) and proves `BARRELMAN_LETDOWN_BROKEN` contains the exact three ordered
lines seen in the reference: “A fat lot of good you were,” damaged-stock/trust, then the threat.
The corresponding audit locks the Lua broken-stock branch to that group before any thanks path.
Recovery tests pass 293/293.

## State
- Shared working branch: `feat/novi-script-recovery`; native parity checkpoints through `39538ff` and subsequent NewOakValeIntro work are present locally. Verify `git status`, `git log`, and the active branch before every checkpoint because the native and script-recovery lanes share this worktree. `main` and remote `feat/script-recovery-marathon` were both at `8d6ce97` before this handoff. Never stage broad paths: commit only explicit files so concurrent `refs/script_recovery/new_oakvale_intro/` work and line-ending-only header changes are preserved.
- 22 old agent branches are now `archive/*` tags. Root scratch + `CON` removed. `LICENSE`, `CONTRIBUTING.md`, CI `docs-consistency` added.
- Coverage (2026-09-08 dashboard): 8,145 exact and 10,681 relocation matches among 18,870 compiled/behavior-tested candidates; 15,297 landed files are manifest-tracked, including 14,696 genuine sources. Regenerate `rebuild/COVERAGE.md` before quoting later numbers.

## Open these five first
1. `docs/ROADMAP.md` — what is done / in flight / next (the only task list)
2. `docs/ARCHITECTURE.md` — the three source layers (lift intake / parity `rebuild/src/compiled` / modern)
3. `docs/BUILDING.md` — Ghidra, VC7.1, GhidraMCP, FSE tooling commands
4. `CONTRIBUTING.md` — landing rules, header contract, purity policy
5. `rebuild/COVERAGE.md` — the numbers

## Active lanes and their resume commands
- Typed headers: `python tools/decomp_pipeline/gen_class_headers.py --all-trusted` (add `--compile-check`), then `python tools/decomp_pipeline/retype_landed.py --class CGameScriptInterface` (dry run; `--apply` only rewrites files that stay byte-exact). Log: `rebuild/backlog/retype_log.tsv`; headers: `rebuild/include/engine/` (`INDEX.tsv`, `RECONCILE.tsv`, `_quarantine/`).
- Parity crawl: batches through 377 are reviewed and ledgered in `work/crawl_batch155`; generate batch 378 next with `next_smallest.py`. Current totals are 18,870 compiled/behavior-tested, 8,145 exact, 10,681 relocation matches, and 394,217 genuine retail bytes matched. Continue evidence review; use PDB names for readability but treat retail offsets, calling conventions, behavior, and bytes as authoritative. Details: `docs/journal/2026-09/PARITY_CRAWL_2026-09-08.md`.
- Header gate: the 2026-09-08 `--all-trusted --compile-check` wrote and compiled 1,011 generated headers; 267 are layout-quarantined and `CGameDefinitionManager` remains the single additional compile quarantine. Dashboard inventory: 1,019 generated / 268 quarantined.
- De-bake: `python tools/decomp_pipeline/crawl/bake_families.py` for the live family table; `debake_family.py <template> <genuine.cpp> <prefix> --apply`.
- Script recovery: `python -m unittest discover -s tools/script_recovery -p 'test_*.py'`; `python tools/script_recovery/verify_foundation.py`; readiness needs `--vtable-slots`, `--interface-catalog`, `--fse-address-map`. Ingest Aeon's 2026-09-06 LUAGameflow batch into `work/aeon_lua_ports/`.
- ForgeFSE: canonical fork is `D:\Code\ForgeFSE-retail-shadow`, branch `feat/upstream-fse-2026-09-02` (`3f417ee`), untested in-game. `D:\Code\ForgeFSE` is stale.
- Docs hygiene: `python tools/docs_reorg.py --check-links --check-findings --check-root` and `python tools/update_readme_progress.py --check` (both run in CI).
- Build/visual QA: `rebuild/build_bootstrap.ps1 -RetailFrontendBank <frontend.big>`; recipe in `docs/pipeline/VISUAL_PARITY_STATUS.md`. Retail Fable.exe must be CLOSED (single-instance mutex fails the WinMain fixture).

## Where notes go
- This file stays one page. The long-running journal is `docs/journal/HANDOFF_ARCHIVE.md` (12k lines, append-only history).
- New session notes: `docs/journal/YYYY-MM/<TOPIC>_<date>.md` (e.g. `docs/journal/2026-09/`). Cited technical truth: `docs/journal/FINDINGS_LOG.md`.
- Solved gotchas get a one-liner in `CLAUDE.md`; roadmap status changes go in `docs/ROADMAP.md`.

## Gotchas not yet in CLAUDE.md
- A file named `CON` at the repo root hangs EVERY git command on Windows (reserved device name). Delete it with `del "\\?\D:\Documents\FableTLC\CON"` — normal `del`/Explorer cannot.
- A killed `git add` leaves a stale `.git/index.lock`; remove it before retrying (check no git process is running first).
- Sibling repos hold the moved modding docs: FableForge `D:\Code\FableForge\docs\from_fabletlc\` and ForgeFSE-retail-shadow `docs\from_fabletlc\`. `docs/modding/README.md` is the index; do not recreate those docs here.
- `native_conversion_readiness.json` silently loses all interface resolution if regenerated without `--vtable-slots` and `--interface-catalog`.
- Two ForgeFSE forks existed; only `ForgeFSE-retail-shadow` is canonical. Any binding port must land there.
