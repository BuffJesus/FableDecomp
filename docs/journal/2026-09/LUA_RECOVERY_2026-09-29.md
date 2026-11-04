# Lua recovery - 2026-09-29

Resumed the explicit 2026-09-28 bedtime handoff on feat/novi-script-recovery,
base 366c08e. Lua converter/evidence/output paths were clean at preflight;
existing native-engine and roadmap edits belong to other work and were preserved.
Scratch: `work/codex_lua_20260929/`. No new units registered.

## Recovered startup operands

- StatueMaster Main 0x00ED3B30: current typed exports print preserved CPU registers
  as spurious `_CIfmod` arguments. The previous lowering accepted only the empty
  argument spelling; the ST0 result consequently disappeared from Lua. Accept the
  observed preserved-register spelling under the existing instruction proof:
  `fld dword [esp+0x10]; fld qword [0x1237f00]; call 0xBFEB28`. Unknown arguments or
  mismatched instructions remain unresolved. All four angle stores retain their result.
- SickChild Main 0x00EC5DE0 shares this fmod failure. Preserve its float slot's
  self-update through the negative-angle branch until the next string-address use.
  A proven vector local passed by address now retains its kind when placing API
  arguments. EC5F82..EC5F99 proves the teleport's position, angle and two false flags.
- SickChild's animation name was dropped as a supposed implicit second receiver,
  then replaced by a stale bed-result table. A proven acquired resource now uses
  the existing resource PlayLoopingAnimation binding, passing its string, count,
  and all seven native bools. The candidate sidecar's LuaRetailResources.h and
  EntityScriptingAPI.h confirm this contract. No DLL changes.

The same resource rule corrects HornetDrone (E11F96), InfectedBalverine (E02AA3),
and BanditKing (D0AC83). Native call windows are retained in
`other_animation_evidence.json`; exact generated calls are behavior-tested.
Only the generic lowering/lifter were edited; generated Lua was regenerated.

## Validation and promotion

- Complete 29-unit before/after scratch generation: five files in five units
  change; the other 24 units are byte-identical. No registered baseline drift.
  Explicit draft inputs were used for both readable builds.
- `test_startup_angle_bed_operands.py`: 39 passing cases, including generated
  draft/readable negative-angle paths, four statue state stores, exact child
  position/angle/string/flag operands and the three other animation sites.
  Against the unchanged baseline artifacts: 24 fail and 15 pass. Native instruction
  assertions and rejected ambiguous fmod spellings are included.
- Round-12/out-table checks: 14 pass. Compatibility: 66 tests plus 13 subtests pass
  (ST0, arguments, scalar slots, vectors, resources and SkillTarget paths).
  Total final distinct coverage: 119 tests plus 13 subtests.
- 272 affected-unit before/after draft/readable Lua files parse. All five typed
  smokes have zero failures on both sides. Promoted five draft/readable pairs and
  reports; promoted bytes match the reviewed artifacts. Corpus: 371 Lua, zero
  syntax failures. Ledger refreshed once after promotion.
- Initial test-fixture construction outside the checkout failed because the
  readable builder requires root-relative paths; fixed to use owned work scratch.
  `focused.log` is the successful rerun; `final_behavior.log` covers the expanded
  final tests. An early review invocation preceded A/B completion and is superseded
  by the completed `review.log` and `scope_review.json`.

Reproduction: `ab/compare.py`, `review.py`, `validate.py`, `promote.py` in scratch.
The final/baseline tests use STARTUP_OPERANDS_ARTIFACTS pointing to
`test_artifacts/after` or `test_artifacts/before`; both are preserved.
Reports include `ab/comparison.json`, `readable.diff`, `validation.json`,
`promotion.json`, `readability.json`, and test logs.

## Candidate v44

Derived from v43, with SickChild, StatueMaster and InfectedBalverine refreshed.
BanditKing and HornetDrone retain their separately pinned candidate lineage;
those two recovered source changes are not runtime claims for this candidate.
210 Lua parse, 216 manifest hashes match, 38 overrides, no new package smoke
failures (16 existing mock failures remain). Both DLL roles validated and bytes
unchanged: stock b2432e8 SHA-256 36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea;
sidecar e83c39f SHA-256 f0ded2603690b8325fb0565d8aa0829b83887300f62c5553b03b828513e7df2c.
Manifest records the base commit and modified-source hashes. Local changes only.
`stage_v44.py`, `package_validation_v44.json`, `validate_v44.py`,
`package_smoke_v44.json`, and `bundle_payload_diff.json` preserve staging evidence.

## Runtime gate

Replay completed successfully under the existing authorization:

    python -u tools/script_recovery/ingame_runner.py tools/script_recovery/runner_quests/guardian_sister_info.json --bundle v44 --tag v44_guardian_bowerstone_20260929 --minutes 8 --launch --save adult_wasp_completed_2026-09-23 --close

Maze's real conversation completed, PostSavePosition reached 400, and
CoreQuestWaiting was true. Zero Lua runtime errors in the archived log.
StatueMaster Main entered and returned successfully. SickChild Main entered and
performed all three bed lookups, reaching the availability/usability calls after
its teleport and animation; the previous sol argument error is gone.

The frontend initially waited on the startup movie, which the visual classifier
called pausemenu/other. A manual ESC skip let the existing driver finish profile
selection and load the intended save; no harness change was required. Travel,
positioning and talk input were assisted as in yesterday's replay. This was not a
full StatueMaster interaction or SickChild quest-completion/save-reload proof.

Game closed. All 561 save files match the prelaunch hashes, with no added files.
The local launcher loaded both DLLs from the candidate; no shared installation
payload was replaced. No Lua playtest remains running. Nothing pushed or committed
from this turn; reviewed source changes remain in the working tree.

Evidence: `runtime.log`, `runtime_result.json`, `save_hashes_before.json`,
`save_integrity.json`, and `work/runner/v44_guardian_bowerstone_20260929.jsonl`
plus its `_FableScriptExtender.log`. v44 README/status metadata was finalized
only after runtime closed; Lua/DLL bytes did not change, and all 216 manifest hashes
were rechecked. The package smoke report predates this documentation-only update.

## Next scoped work

Continue the earlier-script encounter queue in LUA_PLAYTEST_2026-09-28.md.
Next useful Bowerstone checks: SickChild mother conversation and visible bed pose,
Book Collecting teacher interactions, and BeardyBaldy conversation/state changes.
StatueMaster needs an actual statue/chest interaction beyond startup. Preserve v44
and the original saves; use separate harvested checkpoints for completion/reload
proofs. Witch dynamic speech/cleanup operands remain unresolved. BanditKing and
HornetDrone source improvements require separately reviewed candidate staging
because their current bundle files have pinned lineage.

## Marathon continuation: teacher resources and earlier encounters

The user requested a marathon after the v44 checkpoint. Continued the authorized
earlier-script encounter queue; no new units registered, no native-engine edits,
no DLL changes or shared installation changes. Scratch:
`work/codex_lua_marathon_20260929/`. Source changes remain uncommitted.

### SickChild encounter on v44

`sick_child_encounter.json`, checkpoint `adult_maze_completed_2026-09-23`,
tag `v44_sick_child_encounter_20260929`: the mother's actual introduction completed
and set `MotherIntroDone`; the child was within three units of SickChildBed.
Zero Lua runtime errors. The final camera clipped through furniture: the visible
sleeping pose is still unverified. This is not cure/quest-completion proof.

### Generic resource recovery

Teacher Main 0x00E54E90 keeps the entity resource `seh_me` (self+0x34) in EDI
across acquisition retries, then reuses decompiler aliases later. The previous
straight-line proof discarded this register at branch boundaries; globally
classifying the reused `CCharString*` alias would be unsound.

`native_member_resource_calls.py` now propagates conservative register facts
through the native control-flow graph and intersects them at joins. Retry loops
preserve agreed callee-saved addresses; conflicting/bypassed values, volatile-call
clobbers, partial-register writes, unknown memory writes, indirect branches and
unsupported convergence remain rejected. Direct ESP stack spills preserve
register addresses but discard pending argument facts. Proof is emitted only
after convergence, checking that ECX and the called vtable agree.

Proven resource slot 0x68 calls recover a mistyped alias's zero-argument task
predicate; phantom decompiler operands are discarded. Existing getter and Speak
rewrites now see their true receivers/arguments across retry joins. The teacher
introduction regains its health receiver, exact text and six speech arguments,
and task waits. The book-look, request-again, lost-book and completion paths also
gain proven operands; unresolved dynamic text and wrong-time speech remain
explicit. Full book donation/completion has not been established.

The interrupted introduction also contained an unresolved saved-interface call.
At E559E2..E559ED, `mov eax,[ebx]; push 0; mov ecx,ebx; call [eax+0x5ec]`
unpauses before movie destruction. The same graph proof now recovers this known
interface method only with an agreed interface/vtable and proven bool operand.
Both intro variants are tested during termination after a speech yield.

### Validation and promotion

- Complete final 29-unit scratch A/B changes only
  `V_BookCollecting/Entities/BS_Teacher.lua`; other 28 units are byte-identical.
  No registered baseline drift. Both readable sides built from explicit drafts.
- 92 focused tests pass: generated introduction (24 cases: draft/readable,
  both Lady Grey variants, positive/zero/negative health, normal/interrupted),
  native CFG adversarial cases, exact retail proof and existing member-resource
  behavior. All 24 introduction cases fail against the before artifacts.
- Compatibility: 32 tests plus 13 subtests pass, including the same 14 CFG tests
  already counted in the focused run. Distinct total: 110 tests + 13 subtests.
- 16 affected before/after draft/readable Lua parse; typed smoke JSON is clear
  on both sides. Promoted the reviewed teacher pair/reports. Corpus audit:
  371 Lua and zero syntax failures; readability ledger refreshed.
- Earlier speech-only comparison is retained as `comparison_speech_only.json`;
  final comparison includes interruption cleanup. A temporary alias experiment
  was removed; `native_evidence_lowering.py` matches its marathon baseline bytes
  (retains the earlier v44 changes).

Evidence/recipes: `ab/compare.py`, `ab/comparison.json`, `readable.diff`,
`validation.json`, `final_behavior.log`, `baseline_behavior.log`,
`compatibility.log`, `promotion.json`, `readability.json`. Final tests use
`TEACHER_INTRO_ARTIFACTS` and `MEMBER_RESOURCE_BOOK_TEST_ROOT` pointing to
`test_artifacts/after`; the former points to `test_artifacts/before` for regression.

### Candidate v45 and teacher runtime

v45 derives from v44, replacing only the reviewed teacher Lua. Stock FSE and
NoviCompatibility DLL bytes/roles unchanged. 210 Lua parse, all 216 manifest
hashes match, 38 overrides, 17 existing package-smoke artifacts and no new ones.
Staging's first payload-diff assertion caught the active baseline replay log
changing during the copy. The final verifier excludes only that operational
`NoviCompatibility/FableScriptExtender.log`; every payload hash and Lua delta
remains checked. `package_validation_final.log`, `package_smoke_v45.json`, and
`bundle_payload_diff.json` are the successful final checks.

Two v44 teacher attempts did not complete a conversation (blind targeting, then
an incomplete OCR target label plus tutorial prompts). They establish no
before/after runtime bug claim. Native evidence and generated baseline tests
establish the generator defect independently. The second attempt revealed the
actual target label `Mr. Gout - Teacher`.

Successful command:

    python -u tools/script_recovery/ingame_runner.py work/codex_lua_marathon_20260929/book_collecting_daytime_intro.json --bundle v45 --tag v45_teacher_intro_20260929 --minutes 4 --launch --save adult_maze_completed_2026-09-23 --close

Assistance: set world time to noon, teleport/facing around teacher, OCR-confirmed
TAB, tutorial/Next clicks; no quest-state writes. Log records
`TEXT_QST_B16_INTRO`, the 420-frame speech wait, unpause and movie destruction.
The retry then naturally entered `TEXT_QST_B16_BOOK_LOOK` and
`TEXT_QST_B16_BOOK_NOT_FOUND`; that movie also closed. Final `DoneIntro=true`,
`BooksDonated=0`, scene=false, done=true, zero Lua runtime errors. No book donation
or quest completion/reload claim. Logs/screenshots: `work/runner/v45_teacher_intro_20260929*`.

## Marathon continuation: BeardyBaldy, rotating statue, and profile restoration

### BeardyBaldy encounter

Installed world data establishes map slot 3, BowerstoneSlumsWarehouses, with
BB_BeardyBaldyMan near world (3149, 4103, 24.3). The first assisted replay played
all three introduction speeches and closed its movie, but a 70-second step retry
opened another conversation before completion was checked. It is recorded as a
harness timeout, not a passing quest check. The uninterrupted rerun used a
300-second step timeout and completed the scene in 146 seconds.

    python -u tools/script_recovery/ingame_runner.py work/codex_lua_marathon_20260929/beardy_baldy_intro.json --bundle v45 --tag v45_beardy_intro_patient_20260929 --minutes 7 --launch --save adult_maze_completed_2026-09-23 --close

Actual state: `QuestActivated=true`, `IntroComplete=true`,
`Phase1RequirementsComplete=true`, `RequiredHairdo=OBJECT_HERO_HAIR_BASIN_01`.
Movie closed, runner done=true, zero Lua runtime errors. Assistance was travel,
position/facing, TAB, real Yes/Next UI input; the only harness write was its own
completion marker after the actual quest-state check. No haircut purchase or
later phases/completion/reload were tested. Logs/screenshots:
`work/runner/v45_beardy_intro_patient_20260929*`.

### Rotating statue angle

Static encounter preparation exposed another lost x87 result in
StatueMasterStatue.Main (ED41D0). Unlike the root's register-only spelling, this
export prints `_CIfmod(1,unaff_EDI,unaff_ESI,unaff_EBP)`. Native ED4249..ED426A
stores the time fraction plus 0.5, pushes bool 1 for the later facing API, loads
the dividend and double 1.0 onto x87, calls BFEB28, then stores ST0 as the facing
angle. That CPU-stack bool is not a CIfmod operand.

`lower_cifmod` now accepts this leading bool only when the adjacent native PUSH
matches, under the existing two-load/x87 divisor proof. Mismatched pushes remain
unresolved. No address-specific generated-Lua edits. New tests cover both push
values, mismatches, exact retail bytes, and generated draft/readable rotation
and text through morning/noon/afternoon wraparound.

- Full 29-unit A/B changes only `V_StatueMaster/Entities/StatueMasterStatue.lua`;
  other 28 units identical, no baseline drift. Both readable builds use explicit
  drafts. Promoted that pair and reports; corpus still 371 Lua, syntax clear.
- 36 focused tests pass; before artifacts fail all 30 generated behavior cases
  (six native/guard tests pass). Final startup/ST0 compatibility run: 99 tests
  pass, including those 36. Initial draft fixture lacked the StatueAltar lookup
  mock; corrected before the recorded baseline/final runs.
- 24 affected before/after draft/readable files parse; typed smokes clear on
  both sides. Evidence under `work/codex_lua_statue_20260929/`: `ab/comparison.json`,
  `readable.diff`, `validation.json`, `baseline_tests.log`, `focused_tests.log`,
  `behavior_compatibility.log`, `promotion.json` and `readability.json`.
- v46 derives from v45, changing only the rotating statue entity Lua and package
  metadata. Both DLLs unchanged; 210 Lua parse, 216 hashes match, 38 overrides,
  zero new package-smoke failures. Staging/verification recipes and reports are
  `stage_v46.py`, `validate_v46.py`, `package_validation_v46.json`,
  `package_smoke_v46.json`, and `bundle_payload_diff.json`.

### Profile integrity gap repaired

The post-replay audit found 560/561 files unchanged; `1234234/Profile.bin` had
gained `GivenTutorialCategories[6] 47` after dialog/tutorial interaction.
`stage_save`/`restore_save` previously preserved only AutoSave and its optional
companions. The local launcher separately retained a full original save backup.
Recovered Profile.bin from v44's
`runs/20260929-063739-558576/save-backup/1234234/Profile.bin`, verified against the
pre-marathon SHA-256 `47ff930c7d7c45ff13157cfd00bc250d34a3745346e7f6d165b849f45423c291`
before restoring it. All 561 files then matched exactly, no additions.

`autopilot.py` now backs up/restores the target Profile.bin (without installing
the checkpoint's profile), records original absence, and refuses a missing
required backup before mutating the live files. Legacy backups without the new
metadata leave Profile.bin alone. Old-backup pruning verifies resolved paths
stay inside scratchpad. Launch/driver tests: 12 pass + seven subtests, including
profile round trips, absent profiles, missing backup, and legacy behavior.
Evidence: marathon scratch `Profile_original.bin`, `Profile_after_replays.bin`,
`profile_delta.diff`, `matching_profile_backups.json`, `profile_backup_tests.log`,
`save_integrity_before_v46.json`, and `runtime_results_before_v46.json`.

### v46 statue replay and final checkpoint

    python -u tools/script_recovery/ingame_runner.py work/codex_lua_statue_20260929/statue_rotation.json --bundle v46 --tag v46_statue_rotation_20260929 --minutes 4 --launch --save adult_maze_completed_2026-09-23 --close

Passed both actual entity-facing assertions: 0.75 at 06:00 and
0.025000018998981 at 12:36. Native-derived Greatwood angle state is
0.52315944433212; GetStatuePointingPosition returns 3. TAB interaction visibly
displayed "The statue is pointing to Greatwood." in `reading_live.png`.
Assistance: travel to LookoutPoint, stop/set world clock, position/facing and
TAB/Next; no quest-state spoofing. Final runner done=true, zero Lua runtime errors.
Chest/cellar interaction or reward acquisition remains untested.

The game closed and the updated runner restored Profile.bin automatically.
Final pre-marathon comparison: all 561 save files match, no additions or changes.
`work/codex_lua_marathon_20260929/save_integrity_final.json` and
`runtime_results_final.json` retain the audit. v46 README/status finalized;
216 manifest hashes and recorded source hashes match; DLL roles validated.
Logs: `work/runner/v46_statue_rotation_20260929*`; package evidence remains in
`work/codex_lua_statue_20260929/`.

Current local candidate: v46. All changes remain uncommitted; nothing pushed.
Other native-engine work and roadmap edits were preserved. Next recovery/runtime
gates: visible SickChild sleeping pose and Witch/cure path; book donation and
completion; BeardyBaldy hairstyle purchases/later phases; StatueMaster chest/cellar
interaction; save/reload of those completed paths. Earlier blanket startup-only
coverage statements are superseded by the encounter evidence above.

## Continued marathon: native template signatures and encounter limits

The next shared-parser fault was in Book Collecting `GetBookSpecificArgs`
(retail 0x00E57020). Its exported signature contains a nested `std::map` type.
The ordinary call-operand splitter treated template commas as parameter
separators and rejected the entire signature, dropping both the book index and
argument map. `split_arguments` now has an explicit declaration-only template
mode; expression callers retain comparison/shift semantics. Invalid template
signatures fail closed. Reviewed prototype parsing uses the same mode.

Validation in `work/codex_lua_marathon2_20260929/`:

- 30 focused tests plus 13 subtests passed. The eight new regressions have
  seven failures against the saved baseline (the ordinary-expression case
  already passed).
- All 29 registered units regenerated before/after. Only
  `BookCollecting/BS_Teacher.lua` changed; 28 units were byte-identical. No
  baseline drift in the promoted file. Both draft/readable pairs and their
  reports were reviewed and promoted. Sixteen affected-unit Lua files parse;
  before/after smoke problem lists are empty. Corpus audit: 371 Lua, no syntax
  failures.
- Generated draft and readable book-15 argument selection each passed ten
  master-state boundary cases plus early termination. All ten selection cases
  failed on each baseline. This proves that limited helper behavior, not a
  complete donation.

Book donation still has concrete recovery gates: `BookDef` quest +0x94 copies
global +0x4D4; byte vectors +0xA0/+0xAC represent donated/owned state. The
decompiler reuses the parent/vector-begin alias across different control-flow
branches, requiring native CFG proof before lowering. Vector persistence,
Teacher actor-map insertion, and later helper branches remain unresolved.
Some later branches still compare the reused string temporary `$ARG1` with a
book index. See `book_array_evidence.md` and the four native snapshots; no
speculative array lowering or generated hand edits were made.

BeardyBaldy encounter results must remain qualified. The first v46 run bought
The Pudding Basin through the actual barber UI: gold changed from 570 to 527.
Its polling condition wrongly required a transient `AcquiredNewHairdo` flag,
which the quest can clear after updating its objective, and the run timed out.
The second run used equipped-style checks but shop input remained unreliable;
it also stopped at the haircut step. A scratch helper gave manual inputs sole
cursor ownership while briefly suspending and finally resuming only the owned
runner. Later attempted `SetHeroAsWearing` / `GiveHeroObject` assistance did not
satisfy the equipped-style check. Commands are in `assistance.log`; no quest
progress or completion flags were set. Later phases and full completion are
unverified. Both runs logged zero Lua errors; after each, all 561 save files
matched their original hashes, with no additions.

Runtime tags: `v46_beardy_purchases_20260929` and
`v46_beardy_purchases_stable_20260929`. The failed steps are harness/encounter
limits, not passing full-quest claims.


The v46 statue reward replay also passed (`v46_statue_chest_20260929`):
rotation checks at 06:00 and 12:36 passed, direction was 3, the Piercing
Augmentation was absent before travel, and a real TAB interaction opened
StatueMasterChest in Greatwood_1. The pickup displayed Piercing Augmentation;
the subsequent possession check was true. No enemy-clearing or reward-giving
assistance was used. Travel and clock were assisted. Screenshot:
`work/codex_lua_marathon2_20260929/statue_piercing_reward.png`.
Zero Lua runtime errors. Cellar interaction and save/reload remain unverified.

**v47 is the new local candidate**, derived from v46 with only the reviewed
teacher helper changed. Both DLL hashes are unchanged. Offline validation:
210 Lua parse, all 216 manifest hashes match, 38 overrides, no new smoke
failures (17 pre-existing package smoke fixture failures remain). The affected
Book Collecting unit itself has empty before/after smoke problem lists.
v47 was not launched; the encounter results above belong to v46. Candidate
location: `work/new-oakvale-original-fse-20260912/local-candidate-v47`.

Final save audit: all 561 original files hash-identical, no additions. Game
closed. No shared-install changes, new units, commit, or push. Next: recover
book array/alias/persistence behavior and string temporary lifetimes; resolve
Beardy barber UI automation before claiming later-phase purchase checks;
continue SickChild cure/visible-bed and statue cellar/reload coverage.

## Marathon continuation: v48 book helper and actor recovery

Scratch: `work/codex_lua_marathon3_20260929/`; baseline HEAD `366c08e` plus
existing uncommitted Lua lane work. Other lanes preserved. No new unit.

Recovered three generic lowering behaviors:

- Parameter stack slots reused by CCharString now receive runtime assignments.
  A construction in a returning branch no longer replaces the incoming book
  index on other paths. Book indices 17/19/22/23 are reachable again.
- Master-state CCharString copy constructors lower only for a known String
  field and proven master base. Native E5727D-E57289 copies master +0x54,
  TeddySolution, into the book-21 suffix. Unknown/wrong-type fields reject.
- Actor-map insertion removes an erroneous address marker only when a local
  was assigned a known member resource in uninterrupted control flow. This
  recovers the Teacher actor in AskForBook. Branches, labels, address escapes,
  intervening writes, increments and assignments after use reject.

Validation: 155 compatibility tests pass; earlier lifter/parameter suite
101 tests + 28 subtests passes. The new generated helper suite has 39 cases,
executed against both fresh draft and readable outputs: special books
15/17/19/21/22/23, boundary comparisons, Teddy A/B/empty/other, six unsupported
indices, six early exits, map identity and getter counts. Eight parameter
lifetime regressions and 15 member-operand cases cover the generic fixes.
Initial parameter-only baseline failed six of eight lifetime cases; before
master-copy recovery all four book-21 cases failed.

Full 29-unit scratch A/B: four Lua files in two units change. Book Collecting
changes BS_Teacher.lua and its root; Trader Escort changes its root and
native_quest_helpers.lua. The Trader Escort changes explicitly assign the
same string operands to reused parameters; existing unresolved thing/copy
paths remain. The other 27 units are byte-identical. Draft and readable
baselines match registered output with no drift. All four diffs reviewed;
84 affected-unit Lua files across both stages/sides parse. Both units have
empty smoke problem lists before and after. Reviewed pairs/reports promoted;
one corpus audit: 371 Lua, zero syntax failures. See validation.json,
readable.diff, scope_review.json and promotion.json.

**v48 is the current local candidate**, derived from v47. All four Lua payloads
have matching baseline lineage; no pinned file was overwritten. 210 packaged
Lua parse, all 216 hashes match, 38 overrides. Package smoke adds no failures;
17 pre-existing fixture failures remain and were inspected. Both DLLs remain
identical to v47: stock FSE 36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea;
sidecar f0ded2603690b8325fb0565d8aa0829b83887300f62c5553b03b828513e7df2c.
Candidate: `work/new-oakvale-original-fse-20260912/local-candidate-v48`.
v48 was not launched. The previous actual encounter evidence remains v44-v46.

Full donation/reload is still blocked by BookDef and donated/owned array
access, reused parent/vector aliases, inventory operands, and persistence.
Native vector transfer 0xCDCF80 now has a recorded five-mode dispatch and
callee list; mode identities and ownership/allocation ABI still need proof.
See persistence_gate.md and byte_vector_transfer.asm. No speculative vector
binding, generated hand edit, or sidecar change was made.

Final save audit again matches all 561 originals, with no additions. No game
launch, shared-install changes, commit, or push in this batch. Resume the
array/CFG/persistence evidence gate; teacher actor and special-book argument
selection are no longer open lowering tasks. Beardy purchases, SickChild
visible bed/cure and statue cellar/reload runtime coverage remain open.

## Marathon continuation: v49 inventory operands and native vector proof

Scratch `work/codex_lua_marathon4_20260929/`, v48 dirty baseline preserved.
Generic immutable CCharString snapshots now recover BookDef reads in Teacher
Main, AskForBook and LookForBook. Main uses LastBookRequested, removal uses
the offered index, and the scan uses its loop index. Native CFG proof repairs
only the ambiguous vector-load RHS; no global decompiler-alias replacement.
Reader evidence excludes initialization/writes and other functions. Typed
snapshot kinds are separated from CConversation snapshots.

Full 29-unit before/after comparison changes only BS_Teacher.lua; 28 units
unchanged. After strengthening adverse alias-write guards, all 29 final outputs
were regenerated and matched the first after-side hashes. Both draft/readable
baselines match registered output. Reviewed pair/reports promoted; one corpus
audit: 371 Lua parse. Book unit smoke problem lists empty on both sides.

Tests: final focused suite 103 passes; 18 BookReaction tests pass after adding
an exact definition-read/removal assertion to its existing teacher mock;
39 special-book helper cases remain passing, and 24 teacher intro cases pass
against final scratch artifacts. New generated ownership queries and offer
removal boundaries fail 12/20 stage/cases on v48 and pass all 20 with the fix.
No full-donation claim: native donated/owned flag reads/writes and signed
sentinels remain unresolved. Native persistence investigation also advances:
21 isolated retail executions establish binary count/boolean encoding, clear,
resize/grow and allocator call contracts. See native_evidence.md and both
vector_*_proof.json reports. This is not in-game persistence validation.

v49 derives from v48 with only the Teacher Lua payload changed. 210 package
Lua parse, 216 manifest hashes match, 38 overrides; 17 known fixture failures
unchanged, zero new failures. Both DLLs remain byte-identical to v48.
Candidate: `work/new-oakvale-original-fse-20260912/local-candidate-v49`.

Actual v49 replay: first launch failed in the frontend and restored saves.
Retry used `key ESC` to skip the startup film, then loaded the existing Maze
checkpoint. First/last BookDef reads and negative-index guard passed in-game;
BooksInGame=40, first definition OBJECT_BOOK_RAISE_AGREEABILITY. Teacher intro
and two BOOK_LOOK/BOOK_NOT_FOUND follow-ups completed with all three movies
closed. DoneIntro=true, BooksDonated=0; zero Lua error lines. Travel/clock and
startup skip assisted; no inventory, donation or completion flags injected.
Tag `v49_book_definitions_retry_20260929`; first failed launch tag lacks retry.
All 561 original save files hash-identical, no additions; game closed.

Continuing in `work/codex_lua_marathon5_20260929/sidecar_candidate`, an owned
clone of clean e83c39f. A generic native flag-vector bridge is being built and
tested before any converter activation or packaging. Main checkout remains
uncommitted; shared installation untouched.


### Marathon5 / v50: quest flag vectors

Recovered generic byte-per-flag vector storage, resize, indexed reads/writes,
and native persistence dispatch. Explicit BookDonated +A0 and BookOwned +AC
metadata gates native 467686 resize and CDCF80 transfer. Incorrect widths,
stride, owners, values, callee or persistence name stay unresolved. Reads
preserve numeric C boolean semantics in Lua. Both root Init and OnPersist
now call the sidecar; Teacher marks donation and tracks owned flags.

Owned sidecar clone `work/codex_lua_marathon5_20260929/sidecar_candidate`:
base e83c39f, committed source 73d359eda8eb31f0238605f2d9ca4d64d3da7a31.
Retail temporary storage uses Game_malloc/free, not modern STL ownership;
RAII accounts for native buffer replacement and copies normalized flags back.
Portable C++ ownership tests and Win32 Release build pass. Exported patch
`novi-zzzzzzzzzzzzzzzzzzzz-state-flag-vectors.patch` is the actual committed
diff. See sidecar_build.json for complete source, patch and DLL identities.
Marathon4's native binary/load/resize execution evidence supports the bridge.

81 focused tests pass (native_flag_vectors, book_flag_storage,
book_reaction_lowering, book_definition_snapshots). Full29unit A/B changes
only BookCollecting root and Teacher;28 units identical, no baseline drift.
Both draft/readable stages parse; smoke JSON empty before/after. Reviewed
outputs/reports promoted; corpus371 syntax clear. Generation input hashes
rechecked unchanged after validation. Main checkout not committed.

Candidate v50:210 Lua parse,216 hashes match,38 overrides;17 known package
fixture failures unchanged,zero new. Stock FSE b2432e8 stays unchanged.
Sidecar SHA256 14d4e2e1d4468bf29d1a307fb5f1b595aafb80799de57dc6981bad10e50e253b.
Only two Lua payloads plus NoviCompatibility.dll change relative to v49.
The shared installation is untouched.

Save probe v50_flag_save_20260929 passed: both vectors initialized to40 false
entries, assisted writes set donated indices0/39 and owned17, then AutoSave.
No Lua error lines; harvested save stays in marathon5/persistence_checkpoint.
All561 original save files restored unchanged,no additions. Fresh-process
load verification PASSED (v50_flag_load_20260929): all80 entries matched the
saved pattern with no writes, both vectors remained40 elements, zero errors.
Both game processes closed; final561 original saves unchanged,no additions.
This is real native persistence roundtrip proof for assisted flag state.
Full donation remains unproven: LookForBook still needs signed sentinel and
stack lifetime recovery; mutable xStack_18_b3 currently folds into a constant.


### Marathon6 / v51: mutable teacher scan byte

Recovered entry-initialized stack-byte storage in lift_native_lua. The native
E57549 initializer, E575D0 read and E5761E write show that the teacher's first
new-book choice flag persists across scan iterations. Seven behavioral tests
cover first-choice retention, later scans and constant-only negative scope.
Final109 tests plus33 subtests pass;81 book/persistence tests also pass.
Ten isolated retail scan executions cover new/seen/donated/requested books,
empty lists and category boundaries; see marathon6/check_native_scan.json.

A first broad multiple-write rule affected BordelloLady and ManInLove; its
Bordello temporary tracking changed missing call operands. That version is
REJECTED, archived in marathon6/ab_rejected_broad, never promoted. The accepted
rule requires an initializer before the first control-flow boundary. Final
full29unit A/B changes only Teacher;28 other units byte-identical, no drift.
Both draft/readable parse and smoke JSON is empty. Reviewed pair/reports
promoted; corpus371 Lua remains syntax clear. Package v51:210 Lua parse,
216 hashes,38 overrides,17 known fixture failures unchanged,zero new.
Both DLLs unchanged from v50 (sidecar73d359e). Teacher runtime regression
v51_teacher_regression_20260929 PASSED: intro plus no-book follow-ups,
movies closed, zero Lua errors. All561 original save files unchanged, no
additions; game closed. Travel, clock and conversation input assisted.

Native scan investigation also confirms saved index entryESP-18. E57909
reloads that cell before E5791F AskForBook; the original decompiler instead
prints value=CStack_1c. This needs native operand recovery in addition to the
converter's local_18 lifetime splitting. See marathon6/native_evidence.md.

### Active marathon7: explicit signed scalar reads

Owned snapshot in work/codex_lua_marathon7_20260929. Prototype preserves
explicit(int) reads of scalar locals initialized with high-bit32-bit literals
using signed32 conversion, leaving raw equality/unsigned reads unchanged.
This addresses native TEST/JGE on the -1 sentinel without replacing every hex
literal globally. Focused110 tests plus33 subtests pass; full29unit A/B is
running. Not promoted or packaged. Zero hex-equality/null normalization is
an unrelated existing issue and is not part of this change.


Marathon7 validation complete and reviewed output promoted. Full29unit A/B
changes Teacher plus ApprenticeSpeedTest;27 units unchanged. 168 Lua parses
across both draft/readable sides, both unit smoke JSONs empty. Five unrelated
Guild readable baseline drifts are preserved and explicitly recorded in
validation.json; only the reviewed speed-test pair/report entries promoted.
Final110 focused tests plus33 subtests and85 book/persistence tests pass.
Generated empty-definition comparison: v51 fails2/8 stage/cases; new passes8/8.
Seven actual retail signed-dispatch executions and64 draft/readable Guild handle
ABI traces pass. Guild timer creation/cleanup use signed int in binding bodies.

Candidate v52 is staged and verified:210 Lua parse,216 hashes,38 overrides,
17 known package fixture failures unchanged,zero new. Only Teacher payload
changes; Guild package stays pinned because its existing lineage differs.
Both DLLs unchanged from v50/v51. Corpus371 Lua syntax clear after promotion.
Generation hashes rechecked unchanged. Teacher replay v52_teacher_signed_20260929
is now running; runtime result pending. Full donation still needs saved-index
stack operand recovery, including the raw decompile's wrong E57909 reload.


Actual v52 replay PASSED after one frontend-only failed launch. The first
attempt never loaded a save; retry `v52_teacher_signed_retry_20260929` loaded
the same Maze checkpoint as v51. Retail log: INTRO, BOOK_LOOK, BOOK_NONE_FOUND,
BOOK_LOOK, BOOK_NONE_FOUND. No BOOK_NOT_FOUND; zero Lua error lines. Movies
close, DoneIntro=true,BooksDonated=0,BooksInGame=40. No inventory, progress or
catalogue-count injection; only travel, clock, startup skip and conversation
input assisted. All561 original save files hash-identical,no additions;
game closed. See runtime_validation.json and save_integrity_final.json.
This proves the corrected signed no-book branch in a real40-definition game;
full donation remains open at the selected-index stack alias/reload gate.
Final promoted Lua pairs still match tested scratch and package hashes match.


## Marathon8: reviewed saved-index operands (v53)

Native evidence confirms eight exported scalar tokens use entryESP-0x18.
The original export confused the post-dialogue E57909 reload with CStack_1c;
object lifetime renaming also split local_18's scalar stores and reads.
Added generic native_scalar_operands.py and reviewed unit evidence: whole
native/export hashes, individual MOV bytes, token boundaries and stack-offset
witnesses guard operand-only renaming. entryEspDelta is reviewed evidence,
not an automatic CFG/purge inference. Stale evidence stops conversion.
Object/vtable/string uses and the neighboring first-new byte stay separate.

Owned scratch: work/codex_lua_marathon8_20260929. Ten actual native scans plus
isolated post-dialogue continuations verify stack addresses and helper argument
recovery after deliberate EBP clobbering; intervening speech is not emulated.
180 regression tests pass, including20 new selection/evidence cases. Generated
native-reference comparisons: before8/20 pass, after20/20 pass across both stages,
including inventory reads, owned flags and chosen index. Full29unit A/B changes
Teacher only,28 units unchanged; no baseline drift.16 affected-side/stage Lua
parses and both smoke JSONs clean. Reviewed pairs/reports promoted,corpus371
syntax clear. v53 staging/runtime checks in progress; donation not yet proven.


v53 completed:210 packaged Lua parse,216 manifest hashes,38 overrides,
17 known package smoke fixture failures unchanged,zero new. Only Teacher
payload changed; both DLLs identical to v50/v52. After runtime, all source
input hashes, promoted pair bytes and package manifest hashes still match.

Actual first donation PASSED in retry v53_teacher_donation_retry_20260929.
First v53 run timed out on the intro-only step before granting a book, with
no Lua errors and saves restored. Retry granted one first-definition book
before introduction and used the yes-answer input ladder. After intro, the
second interaction did not start a scene; manual TAB/W/S approaches and the
third runner interaction reached BOOK_LOOK and BOOK_REQUEST_0. The exact
OFFER_BOOK_0 prompt was accepted. BooksDonated became1,BookDonated[0] and
BookOwned[0] became true, and all nine TEXT_CS_B16_BOOK0_10..90 classroom
lines ran. BookReaction registered with one bound argument. The runner's
finished predicate proves ReadingBook=false and cutscene/movie flags false.
Zero error/exception lines. All561 original savefiles hash-identical,no
additions;game closed. Inventory removal was not separately queried; no
actual donation checkpoint harvested/reloaded. Full collection/rewards and
comment-only books remain open. No quest-progress/donation flags injected.
Assistance:one inventory book,travel,clock,startup skips,conversation input,
and manual movement; manual_inputs.jsonl records the latter. Runtime, native,
generated comparison and save-integrity JSONs are in marathon8.

Next:consumed-inventory query plus actual donation harvest/reload; then further
categories/rewards. The comment-only LookForBook resource-vtable operand is
still unresolved. v53 does not change the earlier prologue-frame estimate lead.


## Marathon9: real donation inventory and persistence

Reusing unchanged v53:preflight source hashes and216 package manifest hashes
match marathon8; no generator/sidecar/package edits. Runtime configs live in
work/codex_lua_marathon9_20260929. First replay v53_donation_save_20260929
PASSED with no manual approach corrections: grant verified in inventory,
BOOK_REQUEST_0/OFFER_BOOK_0 accepted, classroom scene finished, inventory0
absent,BooksDonated1,GoodBooksDonated1,DoneIntrotrue,ReadingBookfalse. Both
40-entry flag vectors contain only index0=true. These checks precede AutoSave;
checkpoint harvested into the workspace donation_checkpoint directory.
All561 original saves restored unchanged; no additions. Fresh-process reload
and second donation now running. No quest-progress/donation flags injected.

Independent read-only next-gate work:five native comment-call setup executions
verify dynamic suffixes,Teacher receiver,hero and all six arguments at E577E3.
The vtable spill at E577B2 and reload E577DD share entryESP-0x14. String helpers
are substituted in the harness; their RET/RET4/RET conventions are verified
from retail bytes in comment_string_helpers.asm. This isolates native call
setup, not comment dialogue or a recovered Lua implementation. The current
resource pass neither tracks this spill nor accepts a dynamic speech key;
comment_branch_lead.md records the two missing pieces. No fix promoted.


Marathon9 completed. v53_second_donation_save_20260929 first verified the real
book0 checkpoint in a fresh process, before granting anything. It then gave
one book1, reached BOOK_REQUEST_1/OFFER_BOOK_1/BOOK_ACCEPTED, completed all12
classroom lines, and verified both inventory removals,BooksDonated2,
GoodBooksDonated2 and exact80 flags (only0/1 true) before AutoSave/harvest.
v53_two_donations_load_20260929 loaded the second checkpoint in a third process
and passed the same counters/inventory/flags checks without progress writes.
All three runs finished done=true,zero error/exception lines;game closed.
No manual approach corrections were needed. Assistance:test inventory grants,
initial travel,clock,startup skips and runner conversation input only.

All561 original saves hash-identical after each run,no additions. Both workspace
checkpoint manifests still match. Source input hashes and all216 package hashes
match preflight:v53 unchanged. Prior180-test/full29unit validation was reused
only after matching its source hashes;no unnecessary corpus regeneration.
The three successful configs were preserved byte-for-byte as runner_quests/
book_collecting_donation_save.json,book_collecting_second_donation_save.json,
and book_collecting_two_donations_load.json,with BOOK_DONATION_REPLAY.md.
No new script units,no shared-install change,no commit/push.

Next:duplicate rejection and additional categories/rewards. The native comment
spill/dynamic key is isolated by five call-setup cases but not yet lowered or
played. Two good-book donations with two genuine reloads are now proven;full
collection remains unverified. See marathon9/runtime_validation.json and both
checkpoint manifests for exact artifacts.


## Marathon10: reviewed comment-call operand recovery (runtime in progress)

Owned scratch:work/codex_lua_marathon10_20260929. Dirty v53 generator/evidence
snapshot and base commit recorded before edits; other/native worktrees untouched.
Generic reviewed_resource_operands.py restores only Speak receiver/operands from
explicit reviewed source/native correspondence. E57769 CFG anchor identifies
Teacher self+34; E577B2/E577DD spill/reload share entryESP-14; E577E3 dispatches
slot34. Native six arguments are hero,dynamic COMMENT suffix,0,false,true,false.
The exporter omitted the final false and lost the spilled vtable. Full caller/
source hashes, stack MOV/dispatch/PUSH witnesses, top-level string helper hashes,
source token identity and successful call-site matching reject stale evidence.
This is reviewed evidence, not automatic interprocedural stack alias inference.
The dead vtable-assignment TODO remains visible. No generated Lua hand edits.

Five native Unicorn call-setup cases rerun (string helpers/GetHero substituted,
RET conventions checked). Ten before/after generated traces (both stages) improve
from0/10 to10/10 for receiver/hero/text/flags and no donation request. Final focused
regressions:298 passed; initial125 passed/2 failed because two OLD health tests
read unchanged v53 generated files but expected NOT_FOUND for zero book definitions.
Retail -1 is NONE_FOUND; expectation corrected, failures retained in initial_test_results.json.
Six post-promotion health tests also pass. Full29unit A/B:Teacher only,28 unchanged,
no baseline drift. Sixteen side/stage parses,clean both unit smoke JSONs. Promoted
only Teacher pair/reports;corpus371 syntaxclear. Source hashes still match.

Staged v54 fromv53:only Teacher payload differs,210Lua parses,216manifest hashes,
38overrides,17known fixture smoke failures unchanged,0new. Both DLLs unchanged:
stockFSE SHA36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea;
sidecar73d359e SHA14d4e2e1d4468bf29d1a307fb5f1b595aafb80799de57dc6981bad10e50e253b.
Guild stays on its prior package lineage. Build/package checks are not live proof.

First live attempt v54_comment_save_20260929 failed before dialogue:book25 grant
opened an expression tutorial; initial TAB was swallowed, retry approach missed
Teacher and landed against classroom furniture. One manual S500 movement; later
W700 attempt did not execute because runner had already closed game (TAB then not
attempted). Zero B16 speech lines. Original561 saves restored/hashidentical.
Retry separates grant/tutorial from approach and uses wider stand-offs; native
runtime bounds observed BooksAccepted25,BooksComment35. Runtime still in progress.


Marathon10 completed. v54_comment_retry_20260929 separates grant/tutorial from
approach and passes on its first Teacher attempt (46sec dialogue):BOOK_LOOK then
BOOK_COMMENT_0,book25 retained,counters2/2,BookDonated exactly0/1,BookOwned0/1/25,
Readingfalse,scene/movieclosed,LastBookRequested unchanged. AutoSave harvested to
marathon10/comment_checkpoint. No manual approach correction in this successful run.

v54_second_comment_save_20260929 freshly reloads and verifies the above before
granting26. First targeted TAB did not start dialogue; recorded W250 then TAB
closed interaction distance. BOOK_LOOK then BOOK_COMMENT_1 proves unseen26 selected
over still-present,already-seen25. Both inventory books retained,donations2/2,
exact80 flags,LastBookRequested-1,Readingfalse,movieclosed before AutoSave. Harvested
two_comments_checkpoint. v54_two_comments_load_20260929 verifies all that state in
another fresh process without inventory/progress writes.

v54_duplicate_book_20260929 independently loads marathon9/two_donations_checkpoint,
verifies it before granting anotherbook0,and reaches BOOK_LOOK/BOOK_NOT_FOUND.
Runner.probe observes movie entry per frame (read-only); completion requires actual
scene closure. Duplicate0 retained,book1 absent,counters2/2,exact80 flags unchanged,
Readingfalse,LastBookRequested-1. No manual approach correction; second stand-off
(-1.5,0) targets Teacher. No save harvest for this rejection-only check.

All four successful runs finish done=true,zero error/exception lines. Each restores
all561 original files hash-identically,no additions. Failed pre-dialogue attempt
also restored all561. Game closed. Checkpoint manifests,process IDs,assistance log,
archived game logs and runtime_validation.json record these distinctions. No quest
progress flags injected. Native helper substitutions/offline proof limits remain.
All seven source/evidence hashes match; all216 final package hashes match. Only
bundle README/manifest metadata updated after runtime; Lua/DLL payload unchanged.

Successful configs copied byte-for-byte to runner_quests/book_collecting_comment_save.json,
book_collecting_second_comment_save.json,book_collecting_two_comments_load.json and
book_collecting_duplicate_book.json,with BOOK_COMMENT_REPLAY.md. The second replay's
manual W250/TAB assistance is documented; this is not a hands-free certification.
No new units,shared-install edits,commit or push. Main remains dirty by ownership.

Next:first grudging donation,hat/key rewards and full collection. Read-only lead:
AskForBook E55CE0 calls target009AACE0 at E56134/E5623A/E56350,ecxStack-48,printed
CTCCarryable::OnKill before prize-map writes; semantic identity not yet recovered.
Out-of-hours Main speech still has unresolved resource operands. See next_gate.md.

## Marathon11: grudging donation and reward-map clear (v55)

Owned scratch: work/codex_lua_marathon11_20260929, base366c08e. Prior dirty Lua
work preserved. No new registered units, sidecar changes, shared-install changes,
commit or push. Both DLLs retain v50 bytes; Guild keeps its pinned lineage.

Native AskForBook E55CE0 clears the same local string map at E56134/E5623A/E56350,
target9AACE0, as constructor E56081 and destructors E560D5/E5643E use (entryESP-48).
The target resets root/count and sentinel links, retaining the map head. Generic
fold_resource_objects now lowers known-target clears with explicit local operands.
Misleading CTCCarryable::OnKill labels no longer leave five generated TODOs before
prize macros. No generated Lua hand edits. Existing sidecar ClearStringMap is used.
See native_evidence.md, AskForBook.c and aligned reward_map.asm. Node-erasure itself
was not emulated; generated resource APIs are mocked in offline comparisons.

14 new tests cover good/evil hats, key, no award, already awarded, interruption,
explicit receivers, wrong target and unresolved compound/missing operands. Full
regression312passed; six additional promoted-output health checks pass. Old generated
reward traces0/6 have clean maps; new6/6 across draft/readable contain only PRIZE.
Classroom ARG1 remains present for its own macro, then clear removes it for rewards.
Synthetic thresholds do not establish actual retail reward receipt.

Full29-unit A/B: Teacher only,28unchanged, no baseline drift.16 stage/side parses,
unit smoke JSON clean before/after. Promoted only Teacher draft/readable plus reports;
corpus371 syntaxclear. Candidatev55:210Lua,216hashes,38overrides,17known package
fixture failures unchanged,0new. Initial staging verifier retained multi-GB archive
contents in memory; stopped that owned verifier after copying and completed hash
comparison in finish_stage_v55.py. Package code delta is only Teacher; README,
STAGING and manifest account for metadata. Final README/manifest status update leaves
tested Lua/DLL bytes unchanged. Source/evidence hashes still match generation_inputs.

Live first grudging donation ran unchangedv54 from marathon9/two_donations_checkpoint.
Two frontend attempts timed out before quest entry; third used active startup capture
and observed ESC skips. Third replay granted actual BooksWanted index16, selected
Teacher from south stand-off, played BOOK_REQUEST_16/BOOK_ACCEPTED_GRUDGINGLY and
book16 classroom scene. Donation finished in120seconds: BooksDonated3,GoodBooks2,
inventory0/1/16 absent, exact80flags with0/1/16true,Readingfalse,movie/sceneclosed.
AutoSave harvested bad_book_checkpoint. No manual approach correction or quest flag
injection. Assistance is inventory grant,clock,runner positioning/dialogue,startup.

Fresh v55 process v55_bad_book_load_20260929 loaded that real checkpoint and verified
the same counters,flags,consumed inventory and scene cleanup. No manual keys were
needed on this load (initial tiny-window capture emitted frontend classifier warnings;
normal profile/menu/load detection followed). Both successful runs done=true,zero
game-log errors. Original561 save files restored hash-identically,no additions;
game closed. Checkpoint hashes recorded separately. Failed startup traces retained.
Replay configs copied byte-for-byte into runner_quests with BOOK_GRUDGING_REPLAY.md.

Next: real hat/key receipt and full collection. Use the three-donation checkpoint;
observe actual thresholds1264/1268 and award inventory before/after. Reach thresholds
through donations, never inject progress flags. Consider book15 as the threshold
donation to exercise its classroom ARG1 followed by the new clear. Teacher out-of-
hours speech still has unresolved resource operands. Current replay does not execute
a reward branch. Previous v54 comment/duplicate proof remains unchanged.

## Marathon12: offline reward proof and wrong-time call investigation

User requests no visual/game launches for now. This constraint persists: continue
offline until the user changes it. No game launch, input, install, save staging,
new package or generator change in this pass. v55 stays the current candidate.

Scratch:work/codex_lua_marathon12_20260929. New reusable test module
tools/script_recovery/test_book_reward_boundaries.py adds12 threshold/precedence
cases against freshly generated draft/readable. Together with test_book_reward_maps,
26tests pass in17.19seconds (rewards.xml). These include below/equal/above hat
thresholds, both thresholds qualifying (good wins), key below/equal/above the
accepted total, already rewarded, and hat selection deferring a simultaneous key.

native_reward_probe.py executes retail E560B8..E56422 after donation bookkeeping,
substituting only termination=false and stopping at clear/award entry. All12 native
branch cases agree with24 generated traces. Thresholds9/9/25 are synthetic, not
claims about current retail global data. Native E5611C/JL chooses good first,
E5621D subtracts good from total for evil, E56334/JNE requires exact accepted total.
Both hat branches jump to E56422, so key selection occurs only with HatRewarded
already true on entry. These are selection checks, not actual item delivery.

native_probe.py:4 retail9AACE0 map-header cases pass (0,1,7,FFFFFFFF count), with
erase9AA830 substituted. native_tree_probe.py then executes actual recursive9AA830
too:5 shapes(empty,single,left-chain,right-chain,balanced7nodes) pass. Every node's
two string destructors precede exactly one free; clear retains head, resets links/
root/count, preserves surrounding canaries,stack and callee-saved registers.
Only string destruction99EAE0 and allocator BFEA14 are substituted in the latter.
Native code hashes and events are recorded. This strengthens the v55 identity
evidence but does not prove allocator behavior or sidecar resource lifetime in-game.

TeacherMain.c/asm isolate the out-of-hours gap. E551D9 forms EDI=this+34; resource
getter E557C2 still has CFG evidence. Temporary CScriptThing cleanup decrements
an indirect reference count at E557F8; generic analysis conservatively drops
resource facts at this unknown write. Speech also spills/reloads its vtable at
E55840/E5584B, and pushes the proven-zero EBP rather than immediate boolean zeros.
Thus current reviewed comment-call annotation cannot simply be copied here.
wrong_time_probe.py runs E5582E..E5585B with explicit reviewed ESI/EDI/EBP
preconditions and substituted GetHero/Speak/task calls:3cases verify teacher
resource+34, hero, TEXT_QST_B16_BOOK_WRONG_TIME and flags2,0,1,0, followed by slot68
on the same receiver. No full-function dataflow proof or lowering was promoted.
Next offline fix must prove cleanup alias/lifetime and stack spill, preserve task
waiting/termination, and reject mismatched inputs. Do not weaken unknown-write
invalidation just to recover this branch. Existing generated TODOs remain honest.

integrity.json:all8 previous source/evidence hashes unchanged,216v55 package hashes
unchanged,561 original save files unchanged. No full A/B regeneration/corpus audit
needed because the production generator, evidence and generated output are unchanged.
No commit/push. Live hat/key/full-collection gates remain pending under no-launch
constraint; next work should use the documented offline receiver proof gap.

## Marathon13: wrong-time teacher speech and task polls recovered (offline v56)

User no-launch constraint remains active. No game launch, input, install or save
staging. Owned scratch:work/codex_lua_marathon13_20260929,base366c08e. Other lanes and
all existing dirty work preserved. Native/live worktree changes were not modified.

Generic reviewed_resource_operands now supports a reviewed receiver lifetime across
temporary CScriptThing cleanup, literal speech operands, callee-saved zero flags,
and contiguous resource task polls. Native_member_resource_calls requires every
reviewed task site to match transformed source, just as reviewed speech does.
The BookCollecting unit explicitly annotates E54E90's wrong-time branch: getter
E557C2 anchors this+34, cleanup E557E6..E55826, Speak E55851 and polls E55858/E5587B.
Whole native/prepared-source,region,cleanup and helper hashes plus instruction/stack/
register witnesses fail closed. Unknown-write CFG invalidation is unchanged; its
unreviewed evidence still stops at the reference-count write. This is explicit
reviewed correspondence, not automatic lifetime/alias inference.

Initial annotation pinned raw export and failed source-hash validation: UnitConverter
already restores two AddGossip argument lists. prepared_source.diff records those
two changes; final annotation pins the actual prepared input. No guard was bypassed.
The unused vtable-assignment TODO remains. Only speech and two polls are recovered;
the existing wait/termination/movie cleanup flow is preserved.

Native evidence: native_evidence.md and native_cleanup_probe.py/results. Nine native
temporary-cleanup/speech/task cases cover no ref-info,shared ref-info and final release,
with task result0/1/2. Native instructions preserve teacher resource bytes,clear the
temporary,retain zero EBP,balance stack and deliver hero/WRONG_TIME/2,0,1,0 correctly.
Actor release,allocator,GetHero,Speak and task polling are substitutes. These prove
call setup under reviewed temporary-result preconditions, not visible game dialogue.

New test_teacher_wrong_time.py:40tests (15 evidence mutations,one conservative-CFG
check,24behavior cases) across fresh draft/readable. Full19-module regression364pass
in142.29seconds. Before/after48 traces each: old32/48 correct (16 missed healthy
speech/waits),new48/48 correct; health<=0 cases unchanged. Tests require correct
speaker/text/flags,polling,interruption/movie cleanup and unchanged donation state.
Post-promotion teacher intro and entity-health checks30pass.

Full29-unit A/B changes only Teacher,28unchanged. Draft/readable baseline drift0,
16stage/side parses,clean unit smoke JSON before/after. Promoted Teacher pair/reports
only; corpus371 syntaxclear. v56 staged offline:210Lua parse,216manifest hashes,
38overrides,17known package fixture failures unchanged,0new. Both DLLs matchv55
(stock36ffb32c...,sidecar14d4e2e1...,sidecar73d359e). Guild lineage preserved.
Only payload code delta is Teacher;README/STAGING/manifest are metadata. Historical
runs/save-backups are excluded fromv56 and remain untouched inv55 (documented in
STAGING). No v56 runtime certification. All8generation inputs match; both candidate
manifests match; all561 original saves unchanged. No commit/push.

Next offline scope: extend adverse task-wait/termination coverage or inspect other
remaining Teacher native diagnostics with evidence. Live wrong-time dialogue,
hat/key receipt and full collection wait until the user changes the no-launch
constraint. v55's prior grudging donation reload remains valid for those bytes;
v56 is an offline candidate only.

## Marathon14: composed collection, persistence and completion checks (offline)

User asked to keep working while away for a run; no-launch constraint retained.
No game launch,input,save staging or production generator/output/package changes.
v56 stays the offline candidate; its364-test regression and30post-promotion checks
from marathon13 remain valid. Eight source/evidence hashes and216package hashes
still match. All561 original save files unchanged. No commit/push.

New tools/script_recovery/test_book_collection_sequence.py has16test cases. It loads
fresh generated quest and Teacher code in separate Lua VMs with a shared state
double. Actual generated Init,Main,LookForBook,AskForBook,GetBookSpecificArgs,opinion
helper and OnPersist execute. Main stays alive in a Lua coroutine between movie
closures; each interaction resumes that same coroutine. Checkpoint cases create
fresh VMs and run actual generated initialization and persistence loading.
Inventory,resource objects,macros,engine calls and serializer are doubles. The
BookReaction thread request is checked, but its worker is not run by this harness.

Good-first and evil-first orders each donate25 distinct accepted books. Both
without reloads and with fresh VMs after1/8/9/24 donations pass in draft/readable
(eight full25-donation sequences,200donations). The ninth donation exercises
actual GetBookSpecificArgs output then clearing before hat; the final donation
does the same for key. Exactly one appropriate hat and one silver-key macro request,
all inventory consumed,exact flags,counters16good/25total,empty transient resource
ledger and released camera/NPC pause are required. Continuous and reloaded VMs
then reach SetQuestAsCompleted and COMPLETE speech; a second completion interaction
neither completes again nor takes inventory or requests another reward.

Other cases: good/grudging refusal,save/reload,retry via real Main; duplicate
rejection and newly-seen comment priority with retained inventory; requested book
lost across reload,with/without another available book; schedule844/845/846 and
1614/1615/1616. Talk is allowed at845/1615 per native JL/JG; outside uses WRONG_TIME.
All scenarios check cleanup and prevent inappropriate donations or reward requests.

Bulk inventory check starts with all40definitions held. The reused retail E57530
scanner (native_scan.py from marathon8,without its top-level test side effects)
executes50native scans across both stages,plus its isolated saved-index reload and
AskForBook call setup. Each chosen index and the entire BookOwned vector matches
composed generated Main. Order is0,15..1,16,24..17: first unseen wins,then last seen
eligible in the category; good precedes grudging. Native inventory/termination and
string-constructor calls are substituted; dialogue/donation itself is generated
Lua using engine doubles. Both stages donate25,retain25..39,request good hat at9
and key at25,then reach completion. Four fresh-VM reloads per bulk sequence preserve
this behavior. This adds50composed donations; no actual item-delivery/save proof.

Initial fixture run failed6cases because the opinion-deed binding was omitted;
the real helper always posts to Teacher even with no children. Added that event
double; no production code changed. Initial failure XML retained. Intermediate
7/76/84 reports are superseded by final_regression.xml:85tests pass across collection,
wrong-time,reward boundary/map and flag-persistence modules. Native50-case agreement
is in bulk_native_comparison.json. Resource allocation/destruction is checked by
identity/type and a live ledger; reward maps must contain only PRIZE after clear.

This is stronger composed offline evidence, not runtime certification. Live wrong-
time speech,hat/key delivery,full collection and real persistence of completed state
still wait for the user to change the no-launch constraint. Next offline work can
target interruption during classroom/reward macros or remaining native diagnostics;
never infer a live completion from these doubles.

## Marathon15: interruption timing and persisted progress (offline)

No-launch constraint remains active. No game/input/install/save staging,production
generator/output edits or new candidate. v56 remains unchanged; all8source/evidence
hashes,216package hashes and561original save files match. No commit/push. Main's
dirty baseline and other worktrees remain untouched.

New test_book_collection_interruptions.py adds8 cases (both generated stages):
classroom/award interruption at good-hat,evil-hat and key thresholds,plus interrupted
unanswered questions for good/grudging books. Thresholds are reached through the
actual generated Main coroutine/donation functions using marathon14's Collection
harness. No progression state is manually injected into those sequence setups.
Resource/macro/engine/serialization calls remain doubles; no runtime certification.

Native AskForBook E560B8 checks termination after the classroom returns. Early
E560D1 cleanup destroys string map,actor map and hero resource,then exits without
resetting ReadingBook or explicitly calling FixMovieSequenceCamera(false). Donation
inventory removal,count and flag writes already happened and are not rolled back.
For normal return from an award macro, native E56202/E56308/E5641E writes the award
flag and proceeds through E56422 fade/camera reset and resource destruction. E5645A
clears ReadingBook before reaction allocation. There is no additional termination
check between reward macro return and these writes. Thus a termination predicate
set by the macro double does not prevent the flag write; this does not prove the
macro actually delivered an item in the engine.

native_interruptions.py executes six retail post-classroom/post-award paths with
termination,macro/map setup,resource destructors and engine callbacks substituted.
It stops at early string cleanup E560F5 or before reaction allocation E5645E. On
the latter path ESP has the pending 0x40 allocation argument,which is explicitly
checked. Real allocated objects,final return and reaction constructor are not run.
The initial probe needed bounded RET hook stubs to prevent translation past a
zero-filled callback page; its first stack expectation also omitted that outgoing
allocation argument. Both harness defects corrected before six cases passed.
AskForBook.asm was decoded from its aligned function start. after_reward.asm is
the aligned normal cleanup suffix; an earlier unaligned display was not used as
evidence. Native question loop E55DA5 yields,E55DAA checks termination and JNE
E55DCF returns false before donation writes beginning E55E08/E55E1E/E55E2D.

Tests require exact resource-ledger cleanup,movie/NPC unpause,expected award/thread
requests and retained donation progress. Early interruption leaves ReadingBook true
and no explicit camera-false call; the test records that API fact,not engine camera
behavior (movie destruction may itself reset the camera). Native persistence omits
ReadingBook; fresh generated Init+OnPersist load clears it while retaining counters,
inventory and flags. Deferred hats are requested on the next donation; returned
award macros are not requested twice after reload. Interrupted unanswered questions
leave inventory/counts/donation flags unchanged and can be retried after reload.
BookOwned may already be set by selection,which is expected.

Important synthetic edge: forcing the serializer double after interruption of the
25th classroom retains BooksDonated25 with KeyRewardedfalse. Fresh Main enters its
completion branch without retrying the key. The ordinary Collection.checkpoint guard
refuses ReadingBooktrue; this test deliberately bypasses it to inspect the schema.
This is NOT proof that real game saving is allowed at that interruption point or
that a player can lose a key. No compensating grant or behavior change was added.
Keep it as a native/engine save-lifecycle question for later validation.

Initial question cases stopped on the outer frame before any question; fixture
now arms termination only after a question event. Initial failure XML retained.
Eight focused interruption cases pass; regression.xml has93passing tests across
interruption,composed sequences,wrong-time,reward maps/boundaries and persistence.
Native six-case results and final hashes are retained alongside the test reports.

Next offline evidence gate: inspect movie destruction/camera reset ownership and
save eligibility during a terminating macro before interpreting the forced-save
edge as an engine defect. Live wrong-time speech,hat/key receipt and full collection
remain pending until the user changes the no-launch constraint.

## Marathon16: native movie final-release ownership (offline)

No visual/game launch, install staging, save write, production edit, candidate build,
commit or push. Existing v56 remains the offline candidate. All8 generation-input,
216 package and561 original-save hashes still match. This pass adds scratch native
evidence and updates the handoff; previous93-test regression remains the last Lua
regression, not a new run in this pass.

Pinned sidecar73d359eda8eb31f0238605f2d9ca4d64d3da7a31 delegates DestroyMovie to
Release(Kind::Movie), whose native destructor is006E7B80. StartMovieSequence0089B110
creates implementation006E7630 (vtable01260F00) and stores it via008AB970/008AB2B0.
The reference-info callback008AA7F0 calls its virtual deleting destructor006E7C90,
which invokes006E7550. That body calls0089B210 unconditionally before optional
player-control restoration and nested/base destruction. Thus final movie release
has engine cleanup beyond the quest's explicit camera call.

0089B210 clears GameScriptInterface+2C, invokes slots5E8,634,684(0),680 and the
manager+38 callback. Sidecar names these EndLetterBox,ResumeAbilitiesForCutscene,
CameraResetToViewBehindHero and CameraDefault. The actual retail vtable01260F0C
maps62C to0088E9E0, which reads exactly the +2C movie-active byte. It maps684 to
00890FC0 and680 to00890FF0. CameraDefault can return early without an eligible
entity; otherwise it reaches camera-manager work and may yield. These dependencies
remain outside the emulated path; do not equate a callback request with visual proof.

The separate FixMovieSequenceCamera slot5CC maps0088E500: it writes the inverse
input boolean to global01380C30. Neither the shown destructor nor CameraDefault
directly writes that global. Calls made by engine/manager callbacks have not all
been traced. Therefore the evidence proves native movie-end/camera requests, not
the ultimate fixed-camera flag state or absence of an engine camera issue.

native_movie_release.py executes six retail destructor-chain cases: null,final and
retained references, each with the player-control flag clear/set. Final release
alone clears movie-active and issues teardown; retained release decrements without
ending the movie. Resource pointers clear in all cases; final release frees the
implementation then reference-info. Stack balance, saved ESI and a stack canary
are checked. Engine callbacks, allocator free and entity lookups are substituted;
the entity-component lookup returns null. Nested/base reference fields are null.
All6 cases passed on the first run. Aligned disassemblies, byte hashes, slot mapping,
sidecar-source hashes and case reports are retained in this directory.

Remaining evidence gate: real manual/autosave admission and termination scheduling
around the already-consumed25th donation. The forced serializer-double checkpoint
from marathon15 does not establish an admissible engine save. This pass establishes
movie teardown ownership only; it does not solve save eligibility. Do not add a
compensating key grant from that synthetic edge. A follow-up should trace the actual
save admission caller and its cutscene/movie predicates, plus whether persistence
can interleave before/after final movie release. Live wrong-time speech, hat/key
delivery and full collection remain pending under the user's no-launch constraint.

## Marathon17: good wizard hat receipt and reload; runner false pass fixed

User reauthorized launches and explicitly asked to keep working. This supersedes
older no-launch constraints. v56 payload is unchanged and now has live good-hat
receipt plus fresh-process persistence proof. No shared-install edit, new DLL,
generated Lua edit, commit or push. All8generation inputs,216package payload files
and561original saves remain hash-identical. Game closes before original-slot restore.

The first wrong-time replay loaded the real3-donation checkpoint but failed to target
Teacher. It misleadingly reported done because the runner continued to an already
satisfied state predicate. No WRONG_TIME speech appeared; this is NOT a speech pass.
The script clears its information marker outside school hours; actual cause/reliable
live trigger for the wrong-time path still needs investigation.

ingame_runner.py now retries failed required targeting within the configured tries
before evaluating the step predicate, and fails on exhausted attempts. Three tests
execute the real play loop with channel/UI doubles: failed target with true state,
failed target then successful retry, and exhausted retries. Combined runner lifecycle,
travel,combat and launch coverage initially passed28tests plus4subtests. Optional
talkStarted now checks a caller-provided conversation-start expression after TAB,
trying another side if no acknowledgement arrives within10seconds. Three more real
play-loop tests cover target recognition without acknowledgement, successful next-side
acknowledgement and blind input with missing acknowledgement:31tests+4subtests pass.
The ongoing live hat retry predates this optional enhancement; it is not live proof
of the acknowledgement option. Existing configs remain unchanged by default.
The first test run had
a fixture missing legs, causing KeyError and Windows cleanup failure; fixed fixture,
initial failure XML retained. No generator A/B is needed for this runner-only change.

Initial hat replay completed six actual donations (indices2..7), reaching9total and
8good, then hit its20-minute cap while granting15. It did not harvest a checkpoint
or prove hat receipt. The retry uses35minutes, saves after each completed donation,
and copies settled saves to separate scratch checkpoints. Repeated NPC targeting
misses are retained in traces; assistance includes inventory grants, clock control,
position/facing, dialogue input and observed classroom skips, never progress flags.
An early manual opposite-side approach and clock cycle in the first hat run did not
start the conversation; their command logs remain. Concurrent channel assistance
could time out a runner query, so later assistance uses only observed UI input.

Successful retry starts from marathon11's3-donation save (0/1/16). Real donations
2,3,4,5,6,7,15 reach BooksDonated10 and GoodBooksDonated9. The good hat was absent
before entry, is present after the actual award macro, and HatRewarded is true.
The evil hat remains absent. All80 BookDonated/BookOwned entries match exactly
0..7,15,16; these10 donated inventory objects are absent. ReadingBook/movie/cutscene
are false at completion. No reward or quest-progress state was injected.

The retry's scratch UI helper uses OCR to press ESC only on a visible classroom
skip prompt, excludes book15 and stops with the run. Its gate accepts a captured
classroom prompt and rejects ordinary Next dialogue. Screenshots and JSONL record
actual skips; the initial replay's manual skips are separately logged. Book15's
classroom and hat scene are not manually skipped. This reaches the string-map
reuse/clear reward path with a real classroom ARG1 before the prize.

good_hat_checkpoint is retained outside the original save tree. A fresh v56 process
loads it and repeats actual hat inventory, counters,80flags,donated-object absence
and closed-scene assertions. The hat run has a separate newly observed runtime error:
SickChildsSister.lua142 calls CameraUseCameraPoint with a missing argument. The
BookCollecting task checks pass, but that whole run is NOT clean; the audit preserves
passed=false/run_clean=false and reports task_passed separately. This is not relabeled
as a baseline failure. Follow-up recovery must trace and fix that native operand.
Full collection,
silver-key delivery and evil-hat receipt remain separate live gates.

Offline save evidence: IsSavingEnabled0088F950 reads World+D7;0088F940 writes it.
AutoSave0088F930 calls004A0AE0, which can reach004A0850. Twenty-four emulated paths
through004A0850 cover null/invalid/valid hero, busy flag, enabled flag and mode. It
rejects null/invalid hero or World+100 already set, sets/clears that re-entry byte
around004A05C0, and does not read+D7. Calls to entity lookup, mode and save body are
substitutes; this does not execute UI admission/full persistence/termination timing
and does NOT prove the marathon15 forced interrupted save is player-admissible.

Evidence and exact replay configurations:work/codex_lua_marathon17_20260929.
Next: recover the live SickChildsSister camera-argument fault; then continue from
this real10-donation checkpoint toward full25 and silver-key
receipt/reload; preserve the saved state and do not inject quest progression.
Wrong-time interaction and interrupted-save admission remain explicit open questions.

## Marathon18: camera-name overload recovery; v57 sister live replay passed

User continues the ultra marathon; game launches are allowed. Owned work directory:
work/codex_lua_marathon18_20260929, baseline366c08e. Existing dirty inputs preserved;
only lift_native_lua.py changed among preflight production sources. Added
test_camera_signature.py. No DLL change, commit or push.

Recovered the actual v56 SickChildsSister Main runtime failure at readable142.
Native EC6F08 uses GSI+678 NameAndTarget with CAM_SC_GIRL,me,-1.0,0,-1.
The binding already supports this overload; the manifest lists ThingAndTarget.
Generic lowering now chooses the five-operand string overload when the first
translated operand has known string type, without mutating the manifest. Unknown
inputs keep diagnostics; six-operand offset overload recovery is outside this fix.
Native body hashes and aligned call-site disassembly are in native_camera_evidence.json
and *_camera_sites.asm. The three affected functions have12slot678 calls total.

Validation:155tests+28subtests across camera/lifter/movement/animation/SickChild
string helpers, plus16readability tests+8subtests:171tests+36subtests pass (207XML
cases). Actual generated Sister Main runs through camera,speech,normal cleanup and
interruption in both draft/readable. The baseline fails the camera assertion.
Initial camera_tests.xml records a fixture setup failure: readable builder requires
scratch beneath the repo; corrected fixture uses a work temporary directory.

Full29-unit A/B changes exactly12draft camera statements in3files: Sister1,
MansLover5,Magicman6. Other27units unchanged, baseline drift0. Readable Magicman
also loses the now-unused getHero local and its assignments, inlines adjacent
ScriptThing/GetHealth reads and flattens the resulting else-if; no extra engine
calls or frame/termination checks change. Two Magicman camera calls retain unknown
target/duration operands and remain unresolved. No whole-quest certification.
96stage/side Lua parses pass; both affected units have clean before/after smoke
JSON. Promoted only the3file pairs and their reports; corpus371syntaxclear.

v57 derives fromv56 with only those3Lua payload changes; pinned Guild lineage and
bothDLLs unchanged. Stock36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea;
sidecar14d4e2e1d4468bf29d1a307fb5f1b595aafb80799de57dc6981bad10e50e253b,
commit73d359e. Package210Lua parse,216manifest hashes,38overrides. Package smoke
with SickChild report retains16fixture failures,0new; this context differs from
the previous BookCollecting17-failure report, so it is not a claimed fixed failure.

Live: initial relative --save path was rejected before staging; rerun used absolute
checkpoint path. First launched attempt v57_sister_camera_20260929 was blocked by
a Windows Firewall dialog before save load and timed out; game closed and originals
restored. Retry encountered the same prompt. ESC/AppActivate did not visibly dismiss
it and UIAutomation could not find Cancel; clicking the observed Cancel button
closed it without granting network access. ESC skipped the startup logo, then the
normal frontend driver selected the protected3-donation checkpoint.

v57_sister_camera_retry_20260929 passed3steps with0Lua runtime/Autopilot errors.
MotherIntroDone was false initially. Assisted positioning and blind TAB reached
the sister; talkStarted's movie predicate acknowledged the real scene. The native
speech log and camera_scene.png show TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20 and
the sister close-up ('Quick, follow me!'). Normal Next input completed speech,
movie destruction occurred, movie/cutscene were false afterward, and MoveToPosition
started toward SickChildsMother. This also supplies live proof for the optional
talkStarted acknowledgement's successful path; failure/retry cases remain offline.
No quest-progress injection or reward grants. No save harvested in this camera test.
All561original saves restored and both216-file v56/v57 manifests verified afterward.

Scope limits: this proves the sister camera conversation and cleanup, not full
SickChild completion or Magicman/MansLover live paths. v56 good-hat receipt/reload
proof remains in marathon17. Next concrete progression gate: remaining15BookCollecting
donations, actual silver-key award and fresh reload from the real10-donation good-hat
checkpoint. Wrong-time Teacher dialogue and forced save-admission edges stay open.

## Marathon19: offline reward-macro skip accounting

User asked to keep working while gaming, until told to stop. Continue offline:
no launch, input, installation or save staging. Current handoff updated accordingly.
Owned work/codex_lua_marathon19_20260929; baseline366c08e. No production converter,
Lua or DLL changes. All216v57 manifest hashes and preflight source inputs unchanged.
User saves were not accessed or compared because normal user gameplay may change them.

Decoded retail script.bin definitions149/150: CS_SCHOOLBOOK_TEACHER_GIVES_HAT
and CS_SCHOOLBOOK_TEACHER_GIVES_KEY. Both normal and skip command lists contain
one GiveHero $PRIZE. Key's normal path gives the item before PRIZE_KEY_30;
hat gives after PRIZE_HAT_30. Both skip lists are FadeOut,GamePause0.5,GiveHero,
FadeIn. Both use GIVEHAT camera and the two blackboard teleport markers. Exact
payload/script/schema hashes and decoded commands retained in asset_validation.json.

Aligned native RunCutsceneMacro disassembly establishes local award-count map
at EBP-128, initialized at CBFC7F and released at CD1EFD. GiveHero branch
CC6476..CC6562 looks up the resolved item, subtracts its recorded count, calls
GSI+1E4 once per positive remaining count, then records the count. It skips grant
when requested count <= recorded count. The map is per macro invocation, not an
inventory query or quest-persisted field. Native skip transition CC012E..CC0196
replaces the command vector at EBP-38 and resets its index; it does not clear this
award map. Skip is processed only once using EBP-15.

reward_native.py executes those original instruction ranges with explicit container
and engine substitutes:14award cases and16skip-transition cases pass. Coverage:
all3book prizes; skip before/after normal award; repeated commands; requested counts
0/1/3 vs previous0/1/3; separate item identities; skip false/true, already handled,
empty/nonempty skip list and active/inactive action. Actual native arithmetic,
branches,map count stores,stack balance and canaries are checked. Map/accounting
memory survives all skip transitions; repeating the same prize requests one native
grant total. This resolves the apparent double-GiveHero command issue under these
bounded preconditions. No compensating Lua reward code is justified.

Limits: map/string operations, action/frame calls and GiveHeroObject are doubles.
This is not live inventory, full interpreter or skip-input proof, and does not model
termination inside GiveHeroObject itself. Actual silver-key receipt/reload remains
pending until gameplay testing is appropriate. No fresh corpus regeneration needed
for this evidence-only work. Continue offline with remaining converter diagnostics.

## Marathon20: identity-LEA operand tracing; offline v58

Continuing offline while user games; no launch,input,install or save access.
Owned work/codex_lua_marathon20_20260929,base366c08e. Generic register tracing
in convert_quest_unit.py now skips exact lea R,[R+0] without consuming alias depth.
Indexed/displaced LEA, clobbers, unknown definitions and actual depth bounds retain
prior behavior. No quest-specific address special case. Magicman E40E80 holds me
in EBX from E40E8C; six identity-LEA padding instructions exhausted the old tracer.
All actor-register definitions are retained in actor_register_definitions.txt.

Full29-unit A/B changes exactly7draft/readable calls in Magicman,28units unchanged:
2camera actor/flags,4facing calls and PlayerOwned RemoveThing. Baseline drift0.
40stage/side parses pass; Bordello smoke clean before/after.21focused padding cases
cover 2/3/6byte encodings,0/1/4/8/32repeats,nonzero displacement,indexing,missing actor,
clobber,real depth bound and all6native camera actors. Initial failure was a test
expectation missing harmless parentheses; corrected, failure XML retained.
178regression tests+28subtests pass. Two additional tests run fresh draft/readable
Magicman Main on PlayerOwned normal/terminating paths: correct actor/flags, removal
suppressed during termination, resource released once. Baseline removal fails this
assertion; baseline termination passes. Total180tests+28subtests (208XML cases).

Eight native call-setup cases execute affected camera/facing/removal instructions,
including termination suppression; native pushes,flags,stack balance and canaries
checked. GetHero,string constructor,termination and target APIs are substitutes.
Native_calls.json contains range hashes. A separate pre-existing alias gap remains:
the first late facing target is p0 in Lua, while native passes GetHero's result.
This pass recovers actor/flags, not whole-call or whole-Magicman parity. Other
resource/movie identity gaps remain. Next offline target: overwritten hero alias.

Promoted only Magicman pair/reports;371corpus files syntaxclear. v58 derives fromv57
with only Magicman payload changed:210Lua parse,216manifest hashes,38overrides,
16baseline package fixture failures with Bordello report,0new. BothDLLs and pinned
Guild lineage unchanged (stock36ffb32c...,sidecar14d4e2e1...,commit73d359e).
All216v57 and216v58 hashes verified. No live v58 certification,commit or push.
Prior v57 sister live and v56 hat evidence remain separate. Final integrity script
initially compared Windows backslashes against a slash path; normalized paths and
verified the sole input delta. No generated output was changed by that check.

## Marathon21: returned hero survives stack-object cleanup; offline v59

Native E43993..E439AB returns GetHero and passes me,hero,false. The stack-object
canonicalizer merged a neighboring outgoing-argument slot onto the returned hero.
Generic lowering now snapshots that returned value for the immediately following
GSI call when the intervening neighboring assignment would collide. Original
assignments and other lifetimes remain; no quest-specific address rule.

Rejected candidate A broadly exempted casted GSI result slots. Despite passing
191tests+28subtests, it changed health/pause/award operands without sufficient
evidence. Its source, A/B and results remain under candidate_a; never promoted.
Candidate B full29-unit A/B changes only Magicman,28units unchanged,baseline drift0.
Draft changes only the facing setup and its new local. Readable changes three
local names; after simultaneous alpha normalization the sole difference is the
facing target p0 -> hero (readable_alpha_review.json). Forty stage/side parses
pass, Bordello smoke is clean before/after. Fourteen focused cases cover casted
and uncasted direct/indirect calls, explicit same-slot overwrite, nonoverlap,
branch boundaries, unknown call, real member zero and distinct multiple snapshots.

192regression tests+28subtests pass, plus3fresh draft/readable behavioral tests:
all6scene-opening facing setups use the hero sentinel; normal/terminating
PlayerOwned removal remains correct. Baseline fails the facing assertion.
Total195tests+28subtests. Eight original-x86 call setup cases pass, with target
APIs substituted; not whole-quest/runtime equivalence. Magicman resource/movie
lifetime and other operand gaps remain.

Promoted only Magicman draft/readable and reports;371corpus files syntaxclear.
v59 derives fromv58 with only Magicman Lua payload changed.210Lua parse,
216manifest hashes,38overrides;16baseline package smoke fixture failures,0new.
BothDLLs and pinned Guild lineage unchanged. No live v59 run during this batch,
no installation/save interaction,commit or push. Latest user permits launches.
Next: prepared remaining15 BookCollecting donations and exact silver-key receipt,
then fresh reload, from actual10-donation good-hat checkpoint.
Evidence: work/codex_lua_marathon21_20260929.

## Marathon23: exact timer destructor operands; offline v60

Completed alongside marathon22's live v59 BookCollecting replay; its running
package was not edited. Magicman registers two timer IDs at E40F7D/E40F8F, stores
them at entry-ESP -0x36c/-0x370, then calls CTimer destructors E44511/E4451A on
second then first. Three original-x86 cases execute registration stores, cleanup
and CD4470 destructor with GSI timer API doubles, proving distinct IDs and balance.
The generated normal/terminating PlayerOwned cleanup had deregistered the first
ID twice. Baseline behavioral assertion fails with distinct IDs11/22.

native_timer_operands.py now traces an immediate EAX result store, requiring
matching prologue/export depth and the known GSI RegisterTimer expression. It
pairs an exact native destructor receiver slot with one singly assigned timer
result. Unknown/clobbered/branched/mismatched evidence remains unchanged. No
quest-specific addresses. convert_quest_unit invokes recovery before ordinary
stack renaming; native evidence dictionaries remain inputs, not edited exports.

15focused cases plus existing timer tests included in213regression tests+28subtests,
all pass. Three fresh draft/readable tests pass: normal and terminating cleanup
release IDs22,11 exactly once and the resource once, plus six distinct-hero facing
setups. Total216tests+28subtests. Full29-unit A/B: Magicman only,28units unchanged,
baseline drift0. Draft and readable each change exactly one DeregisterTimer operand.
40stage/side parses pass; Bordello smoke clean. Only Magicman pair/reports promoted;
371corpus files syntaxclear. Owned source inputs verified unchanged after testing.

v60 derives fromv59, only Magicman Lua payload changes.210Lua parse,216manifest
hashes,38overrides;16baseline package fixture smoke failures,0new. BothDLLs and
pinned Guild lineage unchanged. No v60 launch or installation,commit or push.
Marathon22 continues against v59; its reward proof does not certify Magicman.
Remaining Magicman gaps include movie/resource lifetimes, health operands and
beer-award count. Evidence:work/codex_lua_marathon23_20260929.

## Marathon22: v59 book replay stopped for shutdown

User requested docs update and stop before bed. All owned runner, classroom UI
assistant, launch helper and Fable processes were stopped. Runner was terminated
explicitly, so its cleanup was performed manually: archived runtime log, closed
game, restored scratchpad/save_backup_1234234_20260929-195631. All561original save
file hashes exactly match saves_before.json. No remaining replay is authorized
until the user resumes work. No silver-key receipt or final quest completion claim.

Live v59 loaded the real10-donation checkpoint: Books10,Good9,hat present,keys0.
Normal teacher interactions progressed through22 completed/saved donations,Good16,
keys0. Donation23 was observed in progress when stopped; its scene was unfinished.
Latest retained stable disk snapshot is checkpoint_22, not fresh-reload certified.
Earlier checkpoints11,12,14,15,17,20 are also retained. key_resume_22.json has10steps:
first assert the full22-book/16good flags, consumed inventory,hat,scene state and
zero keys after reload, then remaining indices23,24,21 and exact one-key receipt.
prepare_reload.py requires actual final receipt evidence; key award/reload pending.

The first four added classroom readings played normally; observed Esc-Skip Scene
UI assistance began at book12 and excluded final book21. skip_log.jsonl records
each observed prompt and input. Books were supplied as assisted inputs; quest
progress/reward flags were not injected. Native award and all final inventory
checks remain pending. Runtime errors observed before stop: 0.
Evidence:work/codex_lua_marathon22_20260929 and archived work/runner log.

Marathon23 completed concurrently: tested/staged offline v60 fixes distinct timer
cleanup; that code did not enter this live v59 replay. Marathon24's unvalidated
partial-string operand candidate was preserved in candidate_convert_quest_unit.py
and unvalidated_candidate.diff, then active convert_quest_unit.py restored byte
for byte to marathon24 baseline (tested v60 state). No ongoing tests/conversions.
