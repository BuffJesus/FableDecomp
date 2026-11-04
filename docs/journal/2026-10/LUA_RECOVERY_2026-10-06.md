# Lua recovery continuation - 2026-10-06

User steering: proceed with Lua work until asked to stop. Resumed the interrupted
conditional flag join in `work/codex_lua_conditional_flag_join_20261005` on
`feat/novi-script-recovery`, HEAD `366c08e64a4323a6147fe4d5caa675e31799d51e`.
Native lane edits and its handoff remain separate.

Preflight confirmed both pending source files exactly match `night_stop.json`:
`readable_style.py` SHA256 `08ceaa928ad90d3405882271cc91790186f17d380dc049afc26716fa154a3ce4`
and `test_readable_conditional_flag_join.py` SHA256
`dab201afd121d339b65f5ff15c4fa01491e1eca9e9163e74e026488e27bbdcd4`.
The preserved 390-test / 21-subtest report is reusable for these unchanged inputs.

Started fresh `python work/codex_lua_conditional_flag_join_20261005/corpus.py`
(session 95360). It verifies all baseline input/output hashes, reuses unchanged
drafts, and regenerates all 29 readable units. The prior interrupted output remains
under `paused_after_20261005` and is excluded from acceptance.

Prepared independent exact-diff and entity-branch acceptance in `verify_flag_join.py`:
WillDummy lightning/ordinary-hit short circuit and WomanToAttract talked-to/object
state, including predicate exceptions and call traces. Native windows identify
the actual flag writes at D43561/D43568, ED06CD/ED06D4 and E0220C/E02213.
Hash-gated promotion and a new three-unit package script are prepared but unexecuted.
No new output has been promoted, packaged, installed or game-tested yet.


## Prefix bindings and query cancellation under review (W20, scratch only)

work/codex_lua_prefix_bindings_20261006/pending_review.json is the current checkpoint.
Root and promoted output remain W19. Candidate v3 preserves global/callee/method
lookup order and stops treating query-named methods as cancellation-neutral.
3313tests/51modules pass;29units/534Lua parse.189readable Lua differ across28units;
drafts unchanged. Both corpus and broad-test workers are terminal. No promotion,
package, installation or game run. V1 superseded and V2 rejected (OpenChest missing
check); superseded_candidates.json records both. Only v3/after is under review.

28-unit smoke findings unchanged. Two nontermination trace exceptions are bounded:
Bandit massacre loading prefix1728 cases matches draft (1210 W19 differences);
debug-speech nil-target exit8 cases matches draft (3 W19 method-rebinding differences).
Stable GetHero/RetailResources excluded; no later combat/speech/cutscene coverage.
Query cancellation sweep6472 default-mock cases:0 newly divergent cases where W19
matched draft,26 improvements. This is not equivalence for pre-existing differences.
OpenChest prefix1296, reward chest2304, expression duration1620, SickChild180 also pass;
native OpenChest cancellation windows saved. Full review.diff is18107 lines.

NEXT: finish changed branch/cleanup review, using api_inventory.json to prioritize
removed API sites and expanded exit tails. Artefact's shared frame site and Chicken
CreateSpectators loop need targeted scenarios; guild-skill master, LookoutPointBeggar,
Pickpocket/OpenChest cleanup factoring also need inspection. Do not integrate until
reviewed; then root recheck, hash promotion, new offline bundle, audit and final seal.

## Conditional flag join accepted

Completed `work/codex_lua_conditional_flag_join_20261005/completion.json`.
The generic fold retains predicate evaluation, initial flag-write order and local
scope, rejecting close-handler, shadowing and alternate-entry ambiguity. Preserved
390-test / 21-subtest results match the unchanged source hashes at resume.

Full corpus: 29 registered units / 534 Lua parsed. Four exact readable branch
replacements across TraderEscort (master/shared), GuildTraining/WillDummy and
V_SickChild/WomanToAttract, plus two newly enabled existing skip folds, remove
14 gotos and 10 labels; all other Lua and drafts
remain identical. Native body hashes reverified. The 32 entity snippet cases
cover boolean combinations, predicate errors and short-circuit call order;
complete entity lifecycles remain outside that focused fixture. Full three-unit
smoke findings, complete API traces and frame counts agree with the baseline.
Trader's second style round encloses the reply path in `if commentType == 1`:
retail E0213B/E02140 branches to E02355 for other kinds, and final resource
retirement remains outside the guard. The exact diff check permits only these
two additional wrappers; no enclosed local declarations or alternate edges.

Fresh Trader SDK checks: 4,608 cases / 7,544 termination-reference snapshots;
216 normal and 216 controlled resumed-Pause-error fiber suspension/parked-GC
checks; 352 returning empty-query cases and 80 isolated native fault boundaries.
Each probe records the final generated-file hashes. The ownership-only resource
fixture and modeled engine/scheduler/copy/destructor boundaries do not establish
production host teardown or natural quest completion.

Four Lua files and three reports promoted by hash, including GuildTraining's
registered readable_converter destination. All three affected units packaged
with the existing Oakvale prerequisite; syntax, every manifest member and
sidecar DLL hashes verified. No installation, game launch, input or save changes.

Package: {"bundle": "work/codex_lua_conditional_flag_join_20261005/conditional-flag-join-candidate-20261006", "game_tested": false, "installed": false, "overrides": ["Q_NewOakValeIntro", "Q_GuildTraining", "Q_GuildTrainingDeparture", "Q_GuildTrainingMelee", "Q_GuildTrainingPreMelee", "Q_GuildTrainingSkill", "Q_GuildTrainingWill", "Q_GuildTrainingWoodsDeparture", "Q_GuildTrainingWoodsMelee", "Q_GuildTrainingWoodsWill", "V_SickChild", "Q_TraderEscort"], "parsed_lua": 87, "sidecar_baseline_commit": "07774e98c5675fb66aba7c23b4f1e4453b0434c0", "sidecar_dll_sha256": "967e7dc3b31a5be8523253f31b886b4d06ba1b73e9535354b4b1f0e1cad34a99", "sidecar_patch_sha256": "b885fe7f8cd9b36d08d74ddcf1c5fe276fd16ab907d6d56893032909e392dc70", "sidecar_source_commit": "161c39c4ad167ed23b63704b46afe60e8dd63c35", "units": ["guild_training", "sick_child", "trader_escort"], "verified_manifest_hashes": 92}.

Ledger: {"counts": {"genericTemporaries": 6191, "gotos": 4569, "machineTemporaries": 4649, "nativeNames": 6282, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.


## Constant and empty-branch folding correction in progress

Scratch `work/codex_lua_constant_tokens_20261006` pins accepted W8 inputs, corpus
hashes and exact original copies of both affected generator modules. Discovery
found string/comment corruption, operand-value and return-arity changes, numeric
comparison precedence errors, and lost scope/invalid return placement. The
expanded 132-case behavioral suite reproduces 57 failures on W8 and passes all
132 with the scratch correction (`baseline_tests_v3.xml`, `prototype_tests_v3.xml`).

An earlier prototype had one failure because `build_readable_unit.prune_empty_ifs`
removed empty-if text inside a long string. That pass also discarded predicate
calls/lookups. It now removes only literal true/false empty branches proven to
be code. The constant pass preserves opaque text, keeps operand-valued and/or,
folds only complete literal comparisons and retains true-branch scope with do/end.
Inline else/control ambiguity is excluded. Source and 132 tests are integrated;
focused regression worker 25785 is running. No W9 corpus/promotion/package yet.

A test fixture using an inline local function exposed a separate existing bug:
`prune_unused_closures` treats its following statements through the next same-indent
end as the closure. `empty_trace.json` and `inline_closure_fixture_before.py` retain
the reproducer. Replacing that fixture's local function with the emitted multiline
form isolates predicate evaluation for this batch; no production closure fix is
claimed. The initial rejected prototype reports are retained. Next correction
after this batch should address the inline closure boundary independently.

User's at-work window question remains unanswered; work stays offline. W8 remains
the accepted package and no installation/save/game actions have occurred.


## Constant and empty-condition correction accepted

W9 completion: work/codex_lua_constant_tokens_20261006/completion.json.
Whole-source lexical masking protects strings/comments; constant folding keeps
Lua operand values, return arity, numeric precedence and lexical scope. Empty-if
pruning preserves unknown lookups/predicate calls; closed uint32 private staging
remains removable. Final focused721 and isolated broad1377 cases/34 modules pass.
Four native fixture modules emit136 Unicorn Windows exception diagnostics;
identical counts reproduce on W8, all actual subprocess exits0. Grouped broad
run was interrupted and is not acceptance; first partial corpus/mask regression
and all rejected proof-harness attempts remain preserved.

Full29/534 Lua parse; drafts/evidence unchanged. Reviewed71 readable changes
across28 units, exported functions unchanged. All smoke findings, nontermination
traces and frames agree. HeroBed/TourGuide/TraderConflictEvil restore1/1/21 native
termination queries, verified at D44AB3/F35B30, EE6B91/CB7940 and DF6CE6/CB7940.
115 persistence defaults pass345 cases against actual draft initializers and
sidecar binding; redundant Beggar query removal passes64 stable-thread cases.
Remaining static call-count changes explicitly reviewed, including unreachable
Chicken diagnostic and short-circuited Fish IsNull. SDK/fiber/edge W8 results
reused only after executed helper text, report, source and DLL hash checks.

71 Lua and28 reports promoted by hash. constant-fold-candidate-20261006 contains
256 parsed Lua/261 verified manifest hashes; sidecar161c39c/DLL967e7dc3 unchanged.
Packaged only. Audit371/syntax0, generic6617/gotos4587/native6310/machine4649/
diagnostics971. Higher counts reflect retained scopes/predicates/diagnostics.
No game, input, installation or save changes made by this pass.

User now confirms at work: today's previously authorized in-game window is open.
Next inspect current process/install/save ownership and established disposable
backup/restore workflow before a bounded runtime test. Separate closure-pruning
four-case discovery remains scratch-only in codex_lua_closure_boundaries_20261006.


## Authorized live Trader check completed

User confirmed at work; runtime work/codex_lua_runtime_20261006 used focused
current Trader bundle37 Lua/42 hashes, same sidecar161c39c/DLL967e7dc3.
Owned PID4212 launched, correct disposable profile loaded, assisted retail
Darkwood transition and natural intro observed. Bool persistence callbacks ran.
Four controlled helpers returnedtrue: banter, nil-speaker fallback, nonempty
type2 equality, actor swap. Speech keys queued; no audible playback claim.
Type2 driver timed out during tutorial, but exact original batch later completed
successfully; timeout report preserved. Initial fallback returned timer-active,
distinct retry after expiration succeeded. Final GC verifies three living
traders, hero15hp/counter2, quest incomplete; no runtime/autopilot errors.
Owned process terminated after checks, AutoSave/companions/Profile.bin restored
byte-for-byte, all installed DLL hashes unchanged. No natural combat, quest
completion, production refcount/teardown or fault-recovery proof.
runtime_validation.json pins actual replies/current fresh log. No game remains.

Continue offline closure-boundary recovery: W10 baseline pinned to W9; scratch
prototype corrects four returned/stored/spaced/inline closure failures. No root
change or W10 promotion yet. Current in-game authorization remains today's
at-work window; completed bounded test does not imply all28 units game-tested.


## Closure boundary pass active

work/codex_lua_closure_boundaries_20261006 pins accepted W9 inputs/output hashes.
Initial31-case baseline25fail/6pass; candidate final36 behavioral cases pass.
Generic range finder now rejects inline local functions; pruning counts all
identifier references including callback values and spaced calls. Whole-source
masking protects multiline text while retaining empty line slots/offsets.
Root readable_style.py plus test_readable_closure_boundaries.py changed; generated
outputs remain W9. Full29 corpus session96628 and isolated35-module broad session7697
running on frozen inputs. Inspect terminal results before promotion; prepared
audit_outputs.py/smoke.py point to W9 baseline. No W10 output/package accepted yet.
Completed bounded runtime test is archived separately; no game remains running.


## Closure boundary correction accepted

W10 completion: work/codex_lua_closure_boundaries_20261006/completion.json.
Final36 focused cases and1413 broad cases/35 modules pass; all actual subprocess
exits0 and the same136 Unicorn diagnostics as W9. Final29-unit corpus534 Lua
parses and is entirely byte-identical to W9, drafts included. Thus no generated
promotion, new smoke run, duplicate package or ledger rerun required. W9 promoted
hashes and all261 full-package manifest hashes reverified; current Trader Lua
also matches the focused package used for today's four live helper checks.
Root generic correction accepted; baseline31-case suite had25fail/6pass.
No game running, staged profile restored, no outer commit/staging.

Next scratch codex_lua_cosmetic_semantics_20261006:26 discovery cases16fail/10pass
on current source. Strings/blank lines and assignment/parenthesis return arity
are affected. Scratch prototype25pass/1fail; remaining failure is earlier
pipeline handling of a longstring containing local/return text. No root edit
for this next batch. Retain rejected prototype limitation before integrating.


## Expression value/text pass active

W11 scratch work/codex_lua_cosmetic_semantics_20261006 pins completed W10.
Current generic source preserves single-result call/vararg substitutions only
at expression-list tails, preserves arity-sensitive parentheses, and declines
single-local/multiple-expression inlining so discarded calls keep their effects.
Cosmetics matches masked source and keeps assignment return arity; formatting
does not rewrite multiline text. readable_file conservatively preserves files
with multiline string/longcomment tokens verbatim and reports
preservedMultilineText; none of the registered drafts contains such tokens.

Initial26-case discovery16fail/10pass; first scratch25pass/1fail exposed earlier
line-based handling of embedded local/return text, fixed with explicit boundary.
Broad initial candidate rejected: loop-exit frame fold disabled by overbroad
parentheses. Its partial corpus/tests/source retained in rejected_broad_parentheses;
owned6 python processes stopped, sessions66665/75897 terminal1. Second variant
still failed same test, source retained as rejected_condition_wrapper_v2.py.
Changed strategy: only wrap an expanding expression at an expanding use site;
do not introduce condition/receiver parentheses before the frame fold.
Further8 initializer-list failures retained in expanded_v3_tests.xml; conservative
multi-expression guard resolves them. Final focused120 cases+21 subtests pass,
including61 new expression/text cases. Root source/tests changed, generated
output and accepted package remain W9 (W10 produced no Lua changes).

Fresh final corpus session20388 and isolated36-module broad session91126 running
on frozen inputs. after_inputs.json is definitive; initial prototype/integrated
snapshots are superseded. Do not rerun prepare/integrate/prototype over root or
edit root inputs while workers read them. Next audit full corpus, review every
actual diff, full affected-unit smoke, inspect helper changes and rerun or
hash-justify SDK/fiber/edge reuse, then hash-gated promotion/package/audit.
No W11 acceptance/promotion/package yet. No game running; today's bounded live
Trader validation and exact profile restore remain archived separately.


W11 v6 supersedes the preceding active note. v4 broad1474 cases passed but
single-assignment parentheses impeded readability; corpus20388 stopped, broad91126
terminal0. superseded_assignment_parentheses retains exact source/hash-verified
reconstruction (mixed CRLF/LF required byte-level reversal of the two later edits),
all reports and partial corpus. First directory rename briefly failed; no data
was removed. v5 narrowed single-target assignment guards and passed1474 broad
cases, but added discovery exposed final table-field trailing-comma expansion
(one result became two). corpus2888 stopped, broad60376 terminal0. Sources and
partial outputs copied/hash-verified into rejected_table_tail; original after/
and isolated_tests/ remain superseded, NOT final output. Persistent directory
rename denial was handled with copying into owned scratch, no deletion.

Final v6 handles final table separators and generic-for expression lists.
69 expression/text cases plus3 frame tests pass (refined_v6_tests.xml).
All root inputs now frozen. Final corpus85113 writes after_v6/, logcorpus_v6.log;
isolated broad87659 writes isolated_tests_v6/, logbroad_v6.log. Expected36 modules/
1482 cases. v6_source_hashes.json and newly overwritten after_inputs.json pin
final input. after_hashes.json is valid only after corpus terminal0. Scripts
audit_outputs.py/smoke.py/verify_sdk_reuse.py explicitly point to after_v6; W10
baseline remains codex_lua_closure_boundaries_20261006/after. Do not rerun prototype,
integrate, preservation or prepare_v6_validation scripts over frozen state.
Revalidate helper reuse against FINAL v6 files (earlier reuse is superseded).
Current W11 generated output not promoted; accepted root Lua/package still W9,
with accepted W10 generic closure correction. No game currently running.


## Expression values and opaque text accepted

W11 completion: work/codex_lua_cosmetic_semantics_20261006/completion.json.
FINAL CORPUS after_v7/, inputs after_inputs.json. All29/534 Lua parse; drafts and
native evidence unchanged. Final1487 cases/36 modules pass, including74 expression
cases; actual subprocess exits0, same136 Unicorn diagnostics as W10. Focused
refined_v7_tests.xml77 cases pass. Broad worker35981 and corpus45063 terminal0;
two-unit preflight99500 reused only under identical full inputs/output hashes.

Generic inlining and parentheses stripping preserve single values at Lua result
list tails (return, arguments, assignments, table fields/separators, generic-for).
Discarded initializer expressions keep their effects. Already-grouped results
and single-target assignments avoid redundant guards. Cosmetics protects opaque
text and local-return arity. Files with unsupported multiline text are preserved
verbatim with explicit report metadata; registered drafts contain none. Blank
cleanup still runs with single-line diagnostic comments. No diagnostic removed.

133 readable files across27 units changed:731 paired parentheses insertions;
two expanded loop forms; two immutable timer-alias read substitutions. All other
tokens, including literal/comment contents and exported names, are identical.
Arena GivePrizeFund550 actual-function cases preserve gold/events/frames/exits;
Chicken CreateSpectators1232 cases cover region schedules, ghost timing, flags,
frame/query cancellation and injected API errors. Both native bodies/tails pinned.
PlayWave alias assignments are unique and precede the two changed reads. Chicken
proof initially matched a comment mentioning its function; anchored extraction
corrected that harness-only parse failure before1232-case acceptance.

All27 complete smoke traces, frame counts and findings match baseline; known
mock shortcomings remain in full reports. SDK/fiber/empty/fault reports reused
only after exact executed helper-text and report/source/sidecar hash checks.
No new full-quest retail or production teardown claim. Today's earlier bounded
live Trader check remains in codex_lua_runtime_20261006; this W11 package was not
launched. No game running, earlier disposable save/Profile.bin restored exactly.

133 Lua+27 reports promoted by hash. expression-values-candidate-20261006:
251 parsed Lua/256 manifest hashes verified; source161c39c/DLL967e7dc3 unchanged.
Ledger371/syntax0,6617generic/4587gotos/6310native/4649machine/971diagnostics.
No outer commit/staging, installed files or saves changed in W11.

Rejected/superseded prototypes and partial runs remain preserved. v6 source/
reports in v6_validation; after_v6 and isolated_tests_v6 are NOT final. v6 broad
1482 passed but single-line diagnostic comments disabled cleanup and redundant
groups impeded folds. v7 fixed those interactions; final preflight/diff review
accounts for every remaining structural change. Earlier partial copies and
hash-verified recovery retain exact mixed-newline sources; do not rerun prepare,
integrate, prototype, promotion or sealing scripts over this checkpoint.


## Nil-release correctness work active (W12)

Scratch work/codex_lua_nil_release_20261006; baseline W11 after_v7, all source
and evidence hashes checked before edits. Five concrete failures in discovery.json:
returned nil, global writes before return/end, escaped closure state, close handler.
Early prune_nil_releases now preserves the input; exit proximity is not proof of
an unobservable release. Report slot retained. No generated output promoted yet.
87 focused cases pass in scratch and root (focused_tests.xml). Initial scratch
harness compared table addresses when a field was nil; corrected serialization,
candidate_tests.xml retained and candidate_fixed_tests.xml is final. Prototype
initially left the old callable in PASSES; fixed before candidate validation.
Full29-unit corpus and37 isolated readability test modules running; sessions
95694 and37635, parent Python PIDs11384 and3376 at launch. Inputs frozen in
after_inputs.json. Read output/process state before reuse; empty logs are not proof.
No game launch, installed-file/save changes, or sidecar changes in this checkpoint.
Next: finish corpus and tests, inspect every diff, smoke affected units, verify
helper probe input hashes before reuse, promote/package only after validation.


## W12 v1 rejected; corrected v2 validation active

V1 corpus after/ finished29/534;1574 tests passed, but emitted review found an
Artefact nontermination trace regression. Nothing promoted. v1_validation pins
root source and reports; original after/ and isolated_tests/ remain preserved.
trace_artefact_passes.py identified fold_else_exit treating an inline conditional
return as unconditional. Bandit possession also crossed a termination query.
Original cleanup-region hoisting bound Artefact r1 to a global because its local
declaration followed the closure. All three issues corrected generically after
scratch proofs: conditional exits stay conditional; scheduler queries cannot
cross calls; cleanup definitions follow captured locals without moving initializers.
Ambiguous capture retains the original explicit native gap. Nil-write fix retained.

Root changes: build_readable_unit.py, readable_style.py, native_cleanup_regions.py;
new tests test_readable_nil_releases.py, test_readable_exit_call_order.py,
test_native_cleanup_capture_scope.py. Combined178 focused cases pass. All root
inputs frozen in after_inputs_v2.json. Full fresh baseline AND candidate draft
generation, followed by readable generation, running for29units in corpus_v2.py.
Fresh baseline drafts must equal W11 after_v7 hashes for every unit. Candidate
outputs after_v2/; do NOT use after/ as final. Corpus session94267/PID14588,
41-module broad session74011/PID14096. No edits while workers read root inputs.

Orchard fresh generation finished. verify_artefact.py:3024 actual-function cases
match fresh draft/W11 readable/new readable for events, state, frames and exits;
global sentinels preserved. Frame-based cancellation only, no arbitrary query-side
cancellation claim. W8 SDK/fiber/edge inputs and sidecar reverified against v2
Trader helpers; sdk_reuse_validation_v2.json. Corrected preflight session1763
terminal0; preflight preceded the additional return-prefix ordering guard and is
not final corpus proof. Root final proof must use after_v2 snapshots exclusively.

Next: finish41-module broad/full29 corpus; inspect all draft/readable diffs,
native capture/control evidence and smoke differences. v1 smoke assertion failure
retained (not waived). Promote/package only after v2 review and behavior gates.
No installed-file/save changes, no game, no staging/commit.


## W12 memory recovery; final corpus target after_v3

V2 corpus session94267 terminal1: Chicken Kicking readable generation raised
MemoryError reading the large PDB XML in retail_enums.members. A separate
PowerShell launch also failed with paging-file exhaustion; no command ran there.
No root input changed. Preserve after_v2/baseline_drafts_v2 and their logs.
Do not mistake the incomplete after_v2 tree for completed full-corpus validation.

Broad session74011 terminal0:1668 tests/41 modules; source hashes and same136
baseline Unicorn diagnostics verified in broad_validation_v2.json. Additional
actual-function proofs: Artefact3024 scenarios plus256 exact distance-false query
cancellation traces (restored native call DCDD4A->F35B30), CampHostageDoor5120,
CrateTeamMember.Init459 including128 corrected stale-field initializations.
Smoke v2 current nontermination traces/findings/frames match; complete-trace
assertion correctly flags the restored Artefact query. This is not blanket-waived:
final review must consume the exact native/query proof and enumerate differences.

Recovery session47152 running resume_corpus.py sequentially. FINAL TARGET
after_v3/, final hashes after_hashes_final.json, inputs after_inputs_final.json.
Fresh baseline drafts in baseline_drafts_final/ must equal W11 for every unit.
Completed v2 trees copied only after full file-set/report validation and every-file
hash comparison. Partial/missing stages regenerated fresh in v3, never overwritten.
recovery_statuses.json records actual subprocess exits. Every unit gets recovery.json.
Final corpus parsing/review required after recovery completes. Existing per-function
proofs may be reused only by exact source hash; they refer to v2 files until final
hash checks. No output promoted, packaged, installed or launched in W12 yet.


## Nil writes, cleanup capture and exit/call order accepted (W12)

Completion: work/codex_lua_nil_release_20261006/completion.json. FINAL CORPUS
after_v3/, final input/output hashes after_inputs_final.json and after_hashes_final.json.
Fresh baseline drafts for all29 units equal W11 after_v7. Final534 Lua all parse.
17 readable files plus1 faithful draft changed across10 units. Every diff reviewed;
19 unrelated units unchanged, native evidence unchanged, only Artefact draft capture
placement changed.18 Lua and10 readable reports promoted by hash; conversion reports
were already identical. No hand-editing generated Lua, no staging/outer commit.

Generic corrections preserve nil writes instead of assuming every exit-adjacent
assignment is an unobservable release; reject conditional returns as unconditional
else exits; keep termination queries from crossing calls during inlining; place
cleanup closures after captured locals without moving initialization. Ambiguous
capture keeps the original marked native gap. Artefact's cleanup now binds its
local rather than a same-named global. CrateTeamMember.Init clears ThingToPatrolTo
as the native0xDCDF60 member reset requires. Source changes only build_readable_unit,
readable_style,native_cleanup_regions plus3 focused test modules.

178 focused cases and1668 cases/41 isolated modules pass, all actual exits0. Same
136 baseline Unicorn diagnostics, no new failed/skipped tests.9243 actual-function
scenarios: Artefact3024 +256 distance-false query cancellations; HostageDoor5120;
CrateTeamMember.Init459 (128 stale initial states corrected); Spectator.Init144;
Maze.Main registration prefix240. All cases match their stated faithful-draft
oracle; frame/query limits and stable-GetHero exclusions are explicit in reports.
This is not full-quest retail, native reference-count, audio or teardown proof.

16 isolated call-order restorations plus the Hostage order,1 independent literal
staging,1 renamed Dragon loop counter and2 immutable Arena alias reads accounted
for. All10 smoke findings/nontermination traces/frames match:3 known mock findings
retained. Exactly3 complete trace changes are byte-backed and separately validated:
Spectator's second lookup/distance before query E6385B->F35B30; timer registration
E26F23(vtable0x15c) before query E26F2F->F35B30;21 Artefact distance-false queries
at DCDD4A->F35B30. Strict smoke assertion failures remain archived, not suppressed.
W8 SDK/fiber/edge evidence reused only after exact executed Trader helper extraction,
report/source/DLL hashes; helper behavior unchanged. Sidecar161c39c/DLL967e7dc3.

nil-release-candidate-20261006:151 parsed Lua/156 manifest hashes verified. Not
installed/launched. Ledger371/syntax0;6638 generic,4591 gotos,6315 native names,
4651 machine temporaries,971 diagnostics. Increased staging is deliberate; no
diagnostic hidden to improve metrics. No installed files/saves changed in W12.

Superseded evidence: v1 after/ had534parse/1574tests but Artefact trace failed;
v1_validation pins rejection. V2 after_v2 stopped with MemoryError reading PDB XML
in Chicken Kicking; Windows separately rejected one PowerShell launch for paging
file exhaustion. Sequential recovery copied only verified complete stages and
regenerated missing stages under identical root inputs;29 recovery child exits0.
recovery session47152, broad74011, audit59574 all terminal0. Corpus94267 terminal1
is NOT acceptance. Preflight1763 used an earlier prefix guard and is not final.
Do not rerun integration, recovery, promotion, package or sealing over this state.

Next discovery only: work/codex_lua_declaration_tokens_20261006/discovery.json.
Three of six whitespace variants in local declaration lists become global writes
because tidy_declarations splits only comma-space. No root edit for that next
batch yet. Continue the user's authorized existing-unit Lua correctness work.


## Declaration token correction active (W13)

Scratch work/codex_lua_declaration_tokens_20261006; before_inputs/hashes pinned
to accepted W12 after_v3/final snapshots. Three of six original spacing variants
lost lexical local declarations and wrote globals. Scratch first candidate44/45
passed; styled comma-first multiline declaration still became invalid Lua.
candidate_tests.xml retained. Refined candidate preserves unsupported continued
declarations with explicit preservedMultilineDeclaration metadata; valid single-line
name lists are split by commas with whitespace stripped, and opaque/annotated text
is preserved.76 scratch and root focused cases pass (candidate_final_tests.xml,
focused_tests.xml), including captures, unused names and multiline initializers.

Root changes only build_readable_unit.py and new test_readable_declaration_tokens.py.
No generated promotion yet. Full29-unit readable regeneration from hash-verified
W12 drafts running single-worker to reduce PDB memory pressure: session95549.
42-module broad readability/cleanup run session36183; expected1744 cases if all
complete. Source inputs frozen in after_inputs.json; outputs after/. Do not edit
root inputs while workers read them. Completion requires terminal statuses, all
corpus parsing/diff review, matching source/evidence hashes and relevant evidence
reuse or fresh checks. If all outputs match W12, reuse its package/audit by hashes
instead of rebuilding unchanged artifacts. No game/save/install/staging changes.


## W13 final environment guard validating

Initial W13 workers95549/36183 finished0: all534 Lua byte-identical to W12,
1744 tests/42 modules passed. Superseded before acceptance by the implicit _ENV
edge: an unused-looking environment declaration governs all global lookups.
v1_validation pins that source/reports; after/ is NOT the final corpus.
tidy_declarations now retains _ENV; readable_file preserves explicit environment
code verbatim with preservedEnvironment metadata because flow passes do not model
implicit environment reads/rebindings. Scratch and root107 focused cases pass.

Final corpus target after_env/, snapshots after_inputs_env.json/after_hashes_env.json.
Corpus session86220, broad session37948 live; expected1775 tests/42 modules.
No root edits until both finish. Physical/commit memory headroom was checked above
50GiB, so final regeneration uses3 bounded workers. Prior memory-failed W12 run
remains separate; this round has not encountered a resource failure so far.
No promotion/package/audit yet for W13; reuse W12 artifacts only if complete final
Lua hashes match and all final input hashes/tests validate. No runtime activity.

Next scratch only: codex_lua_random_queries_20261006. Three of four discovery
cases lose/reorder GetRandomThingWithScriptName draws; candidate66 cases pass,
including faults and weak-reference lifetime. Binding evidence pins existing FSE
method/return ownership and vtable74. Candidate changes stay in scratch; no root
random-query edit and no new conversions. Revalidate against accepted W13 before
integration. Preserve W12 sidecar/package/runtime distinctions.


## W13 accepted: declarations and lexical environment

Final workers86220/37948 both exited0. after_env is the sole accepted corpus;
all29 units/534 Lua parsed and are byte-identical to accepted W12 after_v3.
Final1775 tests/42 modules pass with identical136 known Unicorn diagnostics;
107 focused declaration cases cover whitespace, comments, captures, continued
declarations/initializers and implicit _ENV behavior. Explicit environment code
and multiline declarations are preserved verbatim with report metadata.
completion.json pins final input/output and validation hashes. Initial after/
and v1_validation remain superseded. No generated promotion, duplicate package,
audit or runtime run is needed for unchanged Lua. W12 remains latest promoted
output/package; no broader source-based runtime claim is made for W13.

Continuing W14 random-query work in codex_lua_random_queries_20261006. Refine the
scratch candidate to preserve explicit nil release as well as retained result
lifetime before integration. No new conversions or sidecar changes authorized
or attempted by this checkpoint. User's continuous Lua-work instruction remains.


## W14 random-query candidate integrated, validation pending

W13 accepted source/input hashes pinned in codex_lua_random_queries_20261006.
Scratch102 cases pass across dead-definition pruning, single-use inlining and
full readable pipeline: draws/count/order, injected faults, short circuit,
overwrite and weak-reference lifetimes including nil/false/table replacement.
Initial candidate failed2 explicit-release cases; retained candidate_release_initial.xml.
Final candidate pins random-result destinations across flow passes, excludes
GetRandom from disposable queries, and prevents calls crossing random queries.
Binding evidence retains existing native interface74/allocated thing ownership.
Root readable_style.py and new test_readable_random_queries.py integrated;
fresh29-unit corpus and isolated broad validation are next. No promotion,
package, sidecar edit, new conversion or game launch at this point.


## W14 copied-release correction: final validation pending

Initial workers35741/49404 finished0:1877 tests/43 modules,534 parsed Lua;
two readable files changed, complete two-unit smoke traces/findings matched.
WhiteBalverine SpawnBalverines prefix180 cases matched draft (21 baseline GC
observations corrected). NOT accepted: copied-result/explicit-release probe
found local splitting detached r4=nil, extending the temporary lifetime.
v1_validation and after/ retain the superseded source/reports/output; no promotion.

Final scratch134 cases pass. Shared random_result_locals now pins destination
storage in both lua_local_versions and readable_style. Includes32 copied-result
release cases with generated/user locals and split/dead/inline/pipeline stages.
Baseline local-version/literal tests14 +22 subtests passed before integration.
Final corpus after_v2/, snapshots after_inputs_v2/after_hashes_v2; fresh full
regeneration and broad tests required. Root inputs frozen while workers run.
Next scratch-only discovery: codex_lua_call_tokens_20261006/discovery.json;
three spaced-call variants wrongly drop SetStateInt. No root change for that
separate parser issue yet. No sidecar, game, installed-file or save changes.


## W14 scope correction and full-function preflight

V2 corpus46466 intentionally stopped (exit1, owned python PIDs1760/15776/11280/
15816 only); after_v2 incomplete. Broad63495 completed0:1909 tests/43 modules.
New scope probe retained a random result outside its last-use branch/loop;
sink_hoisted_locals narrowed the declaration and changed GC lifetime. Final
candidate applies shared pinning there too. v2_validation archives superseded
sources/reports; no output from v1/v2 was promoted.

Scratch150 cases pass; preflight_scope regenerates WhiteBalverine, OrchardFarm,
TourGuide with candidate sources. Full SpawnBalverines720 actual-function cases
match draft, including both cutscene branches, three waves, nil/object draws,
API faults and GC at every traced call;291 baseline lifetime observations differ.
Stable hero/termination queries excluded; no native-refcount or runtime proof.
Final root sources integrated; final corpus after_final, input/output snapshots
after_inputs_final/after_hashes_final. Fresh full validation next. No sidecar,
installed-file, save or game changes. Spaced-call parser work remains scratch.


## Random calls and result lifetimes accepted (W14)

Completion: work/codex_lua_random_queries_20261006/completion.json. Sole final
corpus after_final/, snapshots after_inputs_final.json/after_hashes_final.json.
Final workers13776/84180 both completed0. All29 units/534 Lua parse; drafts and
26 unrelated units unchanged.3 readable Lua +3 READABLE_REPORTs promoted by hash:
OrchardFarm TeamSpawn, TourGuideGuide, WhiteBalverineWW. Full diff reviewed;
native random sites (all5 across4 functions) and existing binding return ownership
pinned. Generic edits: lua_local_versions/readable_style plus150-case test module.
Random getters retain draw count/order and destination storage across splitting,
dead stores, inlining and scope sinking, including explicit nil releases/reuse.

1925 tests/43 isolated modules pass with identical136 baseline Unicorn diagnostics;
additional14 local-version/literal tests +22 subtests pass.3712 actual scenarios:
WhiteBalverine SpawnBalverines720 full-function, TourGuide Main880 prefix,
Orchard Main1920 prefix and Init192. GC observations/calls/exits match each draft
oracle except32 explicitly pinned PRE-EXISTING Orchard Init data-read fault states:
baseline and candidate reset BanditsLeftID before the read; draft resets after.
Strict initial failure retained.742 baseline lifetime observations corrected.
Stable hero/HeroTeam/termination-query exclusions and bounded paths recorded;
TourGuide tail reuse structurally reviewed, not dynamically certified.
All3 complete smoke traces/frames/findings match;1 known TourGuide mock finding.

random-lifetime-candidate-20261006:47 Lua parsed/52 manifest hashes verified.
Same sidecar161c39c/DLL967e7dc3, source/binding/patch hashes checked. Package is
offline only; no new installation, launch, saves, audio, native-refcount or full
quest runtime claim. Ledger371/syntax0;6652generic/4591gotos/6315native/4651machine/
971diagnostics. No outer staging/commit; HEAD unchanged.

Superseded: initial after/ passed1877tests/534parse but copied-release probe failed;
v1_validation archives it. after_v2 was intentionally stopped (corpus46466exit1)
for scope narrowing; broad63495 passed1909. v2_validation records owned PIDs and
rejection. Do not reuse these corpora or rerun integration/promotion/sealing.

Next W15: work/codex_lua_call_tokens_20261006/discovery.json. Three valid spaced
SetStateInt call variants lose writes because _calls requires adjacent parentheses.
Scratch token-aware candidate fixes reproductions and recognizes argument sugar/
dynamic calls; not integrated or fully tested. Pin W14 baseline before proceeding.
Keep32 Orchard injected-fault state differences as a separate explicit follow-up.
User's instruction remains continuous Lua work until asked to stop.


## W15 call-token candidate integrated

Baseline is accepted W14 after_final; input/output hashes and dirty sources pinned
under work/codex_lua_call_tokens_20261006. _calls now tokenizes code, ignores opaque
text/declarations, recognizes spaced method/field/bare calls, string/table argument
sugar and chained/dynamic callees. Unknown dynamic calls remain effectful. Shared
random destination pinning conservatively recognizes getter references because
masked string-argument syntax has no parenthesis. No generated Lua hand edits.

Scratch139 tests pass: call effects/errors across3 passes, lexical negatives and
random-result lifetimes. Initial sugar3 failures and chained-call18 failures were
retained before correction. Root readable_style/lua_local_versions and new139-case
test module integrated; full regeneration and broad validation pending. Sources
freeze while workers run. No promotion/package/audit yet. W14 remains latest
promoted output and offline package. No sidecar/game/install/save changes.


## Call token recognition accepted (W15)

Completion: work/codex_lua_call_tokens_20261006/completion.json; final corpus
after/, snapshots after_inputs.json/after_hashes.json. Workers48327/33589 finished0.
All29 units/534 Lua parse and are byte-identical to accepted W14 after_final.
2064 tests/44 modules pass with unchanged136 known Unicorn diagnostics; additional
14 local-version/literal tests +22 subtests pass.139 new cases cover spaced calls,
method/field/bare calls, string/table argument sugar, chained/dynamic callees,
opaque text/declaration negatives and random reference retention/release.
_calls is token-based; dynamic callees remain effectful. Random storage pinning
conservatively includes getter references so masked argument sugar is protected.
Initial3 sugar and18 chained-call failures retained in scratch before refinement.

No generated promotion, duplicate audit or package for byte-identical output.
W14 promoted targets reverified by hash; random-lifetime-candidate-20261006 remains
latest offline bundle. No sidecar, installed-file, game, save or outer commit change.

Next W16 scratch: work/codex_lua_completed_calls_20261006/discovery.json.
Three reproduced order failures: state getter moves after same-expression setter;
normal call moves after string/table argument-sugar call. Scratch completed-call
scanner/barrier fixes those reproductions. candidate_tests.xml has104pass/3fail:
global-copy pipeline cases still reread mutated global via propagate_copies,
despite standalone inlining being corrected. Do not integrate this candidate yet.
Review immutable alias scope/dominance, add adverse cases and preflight before
full validation. Preserve W14's32 unrelated Orchard injected-fault state differences.
Continuous Lua work remains authorized until the user asks to stop.


## W16 evaluation-order preflight active, root still accepted W15

Scratch: work/codex_lua_completed_calls_20261006. No root edits/promotion yet.
Candidate v1 fixes completed calls (including string/table sugar) but3 global-copy
pipeline cases fail; candidate_tests.xml retains104pass/3fail. V2 requires unique
top-level local alias bindings, dominating declarations/definitions and no cyclic
source assignment; captured bindings/free globals/upvalues remain unpropagated.
It also blocks nonquery calls crossed by scalar snapshots.139 scratch cases pass.
V2 preflight555 existing tests +43 subtests pass;4 units regenerated in preflight/.

V3 additionally prevents expressions moving across global/upvalue/captured-local
or field/index stores.173 scratch cases and555 existing tests +43 subtests pass.
preflight_v3 is the current candidate output, worker39714 still running4 units
(OrchardFarm, TourGuide, Arena, BookCollecting); worker80901 tests finished0.
Strict Orchard oracle passed Main1920 +Init192 cases: all match draft, correcting
the32 previously recorded Init data-read fault states. Report inherited the old
key/print 'lifetime' for corrected count; rename it to fault-state before sealing.
Raw native function DCD1D0 shows global data load before BanditsLeftID reset.
No integration/full corpus until the preflight diff is inspected. Root is W15,
W14 remains latest generated promotion/offline package. No live game or workers
other than39714; no sidecar/install/save/outer commit changes.


## W16 evaluation-order candidate integrated; full validation pending

Root readable_style.py and new test_readable_completed_calls.py integrated against
accepted W15; sources/baseline hashes pinned in codex_lua_completed_calls_20261006.
Final scratch candidate readable_style_candidate_v3.py:176 focused cases pass.
Existing preflight555 tests +43 subtests pass; preflight_v3 contains4 regenerated
units,44 parsed readable Lua. Full602-line preflight diff reviewed:10 readable
files/3 units changed; BookCollecting unchanged. Orchard2112 actual cases strictly
match draft and correct all32 old data-read fault-state exceptions; report key now
corrected_fault_state_cases (not lifetime). Preflight workers39714/80901 terminal0.

Fixes distinguish completed prefix calls from open outer calls, preserve state/
scalar reads across mutating calls, retain evaluations before observable stores,
and restrict copy aliases to unique top-level locals with dominating declarations
and at most one noncyclic definition. Unknown globals/upvalues/captured bindings
are not substituted. Literal/register staging may increase deliberately.
Full corpus after/, snapshots after_inputs/after_hashes; expected2240 tests/45
isolated modules. No root edits while workers run. No generated promotion/package,
audit, sidecar edit, game launch, install/save change or outer commit yet.
W14 remains latest promoted output/package; W15 last accepted generic source.


## Completed-call and snapshot ordering accepted (W16)

Completion: work/codex_lua_completed_calls_20261006/completion.json; final after/,
snapshots after_inputs.json/after_hashes.json. Corpus10533 and broad88630 finished0.
29 units/534 Lua parse; all drafts and14 unrelated units unchanged.45 readable Lua
and15 reports promoted by hash after full diff review.2240 tests/45 modules pass,
including176 new regressions; unchanged136 known Unicorn diagnostics.

Generic readable_style change recognizes completed calls before a substituted
read, protects scalar snapshots across calls and observable stores, and limits
copy propagation to unique dominating stable function-local bindings. Retained
aliases are intentional. Initial104pass/3fail candidate and v2 previews remain
superseded; final candidate_v3/preflight_v3 and after/ alone are accepted.

9152 actual scenarios: Orchard Main1920+Init192, BordelloClient Init1280,
ScorpionHome Main3456, Maze Main2304; all match draft within recorded bounds.
Corrects32 previously recorded Orchard fault-state cases,128 Bordello fault states,
and144 Scorpion cancellation paths. Additional16 extracted Will frame-block cases
match draft. Native bytes pin Orchard load/reset order, Bordello rand-before-stores,
Scorpion hint/completion/spawn checks D67388/D674F2/D6751E and Will frame/query
D5EA85/D5EA8A. Lua injected errors do not establish native exception equivalence.
All15 complete default smoke traces/frames/findings match baseline;7 known mock
findings retained. Default smoke does not cover every restored query. Maze hit/talk
cutscenes and remaining alias-only changes have structural review, not full runtime
certification; see review_validation.json for every changed file/unit.

evaluation-order-candidate-20261006:172 parsed Lua/177 manifest hashes verified.
Same sidecar161c39c/DLL967e7dc3, source/patch/DLL hashes verified. Not installed or
launched; no save/game/native-lane/outer staging or commit changes. Ledger371files,
syntax0;6668generic/4591gotos/6315native/4661machine/971diagnostics.

Next W17: work/codex_lua_conditional_eval_20261006/discovery.json reproduces12
conditional-evaluation mismatches out of16 comparisons. Single-use inlining moves
an unconditional draw() into the right operand of and/or and can omit its call.
No W17 root edit yet. Preserve short-circuit execution before further promotion.
Continuous Lua work remains authorized until the user asks to stop.


## W17 conditional-evaluation guard integrated; validation active

work/codex_lua_conditional_eval_20261006 pins accepted W16 as baseline. Root edits
are readable_style.py and new test_readable_conditional_evaluation.py. Candidate
passes535 new cases; baseline fails260 of those, retained in baseline_tests.xml/log.
Guard prevents unconditional evaluation moving into a short-circuit operand or
inline conditional body. Calls, index metamethods and arithmetic faults covered;
opaque text and unconditional first operands remain eligible. Existing preflight
731tests+43subtests pass. Four-unit preview44readable Lua parses; only Arena.lua
changes (two equivalent countdown timer aliases), full19-line diff reviewed and
384 actual PlayWave countdown scenarios equal baseline/draft through spawn-list
boundary. Fight/reward tail excluded. All preview workers terminal0.

Full corpus session90750 and broad2775tests/46modules session63607 are running;
do not edit root inputs until both terminal. after_inputs.json pins candidate.
No W17 promotion/package/audit yet. W16 remains latest accepted source/output and
evaluation-order-candidate-20261006 latest offline package. No runtime/install/save
activity or native-lane changes. User continuous-work instruction remains active.


## W17 v1 rejected; v2 preflight active

W17 full workers90750/63607 terminal0:29/534parse,2775tests/46modules with unchanged
diagnostics. Contrary to the early partial view, final v1 changes4Lua/3units.
Complete smoke matches with5 known findings. However SickChild orientation probe
fails36/180 actual Main-prefix cases: retained Y staging permits X to sink after Y.
v1_rejected.json archives hashes/results; after/ is NOT accepted/promoted. Old
seal.py is explicitly disabled. Latest accepted remains W16 and its package.

Scratch readable_style_candidate_v2.py also blocks crossing implicit indexing or
operator evaluations in intervening no-call assignments; only literal or unpinned
declared-local staging can be crossed.607 focused cases pass and existing731tests
+43subtests pass. SickChild v2 preview180 cases now exactly draft-equal, baseline
shape restored. Preview session72128 still generating4 units; session20495 tests
terminal0. Root remains v1 while previews read inputs. No new promotion or runtime.
Actual four changed position predicate blocks already512cases draft-equal,256
baseline nil/getter differences corrected; native GetHomePos/GetPos-before-distance
windows pinned at E3FFFF/E40006,E41026/E4102D,E429A8/E429AF,E911C2/E911D5.
W18 hash-order discovery remains pending; finish W17 v2 acceptance first.


## W17 v3 integrated; final validation active

readable_style_candidate_v3.py and619-case test_candidate.py integrated. Final
corpus is after_final/, snapshots after_inputs_final/after_hashes_final, workers
8097(corpus)/23388(broad2859tests,46modules) running. Do not edit root inputs.
Preview sessions7313/61987 terminal0:731tests+43subtests,32readable Lua/4units parse.
Full121-line v3 preview diff reviewed:6files/4units; fixed-seed corpus may differ
in equivalent aliases. Numeric literals recognized explicitly because tokenizer
has character-level numeric tokens; v2 over-conservative preview is superseded.

180 actual SickChild prefix cases now draft-equal; v1's36 failures corrected.
972 actual Magicman idle cases match baseline/draft (through7frames or beer-cutscene
entry; timers nonzero/talk content excluded).512 actual extracted position-predicate
cases match draft,256 baseline differences corrected.128 extracted Sister hit-block
cases match draft;50 fault-state observations differ from baseline. Native Sister
bytes explicitly show cleanup flags BEFORE message calls, whereas synthesized Lua
draft puts flags AFTER calls: native_sister_hit.json records this limitation.
Those50 observations establish Lua draft agreement ONLY, not native exception or
C++ string cleanup equivalence. Valid numeric-flag API order/results unchanged.
Native position getter/distance order remains independently confirmed at4sites.

No W17 promotion/package/audit/runtime yet. Final full diff/smoke/native scope review
and rerun actual probes against after_final still pending; only then hash-gated
promotion/package/ledger/completion. Old seal.py disabled; do not run it.
after/ v1 rejected and preflight_v2 superseded. W16 remains latest accepted.
W18 copy determinism scratch discovery pending; continuous Lua work still active.


## Conditional and implicit evaluation preserved (W17)

Completion: work/codex_lua_conditional_eval_20261006/completion.json; sole final
after_final/, snapshots after_inputs_final.json/after_hashes_final.json. Final
workers8097/23388 terminal0.29units/534Lua parse; all drafts and19 unrelated units
unchanged.13readable Lua+10reports promoted by hash after full310-line review.
2859tests/46modules pass with unchanged136 known Unicorn diagnostics.619new cases;
preflight731tests+43subtests includes14 local/literal tests+22subtests outside broad.

readable_style now retains unconditional evaluations before short-circuit/inline
conditional uses, and blocks moving them across no-call indexing/operator reads.
Literal or unpinned declared-local staging remains eligible. String/comment tokens
and numeric literal forms tested. Drafts/evidence/sidecar code unchanged.

5272 bounded scenarios:1248 actual functions/prefixes (SickChild180,Magicman972,
Orchard comment96),4024 actual extracted blocks (position predicates512,Sister128,
rescued Trader2304,Picklock/Pickpocket duration1080). Position getters now precede
guards as native at4sites;256 baseline nil/getter observations corrected. Sister/
Trader flag probes match synthesized draft with376 fault-state differences, but
native C++ cleanup flags precede calls whereas synthesized draft flags follow:
explicitly NO native exception/cleanup equivalence claim. Expression durations
match baseline numeric API results/query order; earlier draft global-data query
reordering remains pre-existing and excluded. Full quest/cutscene coverage excluded.
All10 complete default smoke traces/frames/findings identical;6 known findings.
Unused hero aliases, parameter/resource copies and retry aliases reviewed by scope,
dominating definition and preserved underlying references; no native-refcount claim.

conditional-evaluation-candidate-20261006:145 parsed Lua/150 manifest hashes.
Same sidecar161c39c/DLL967e7dc3, source/patch/DLL checked. Offline only; no install,
launch, save/native-lane/outer staging/commit change. Ledger371/syntax0;
6672generic/4591gotos/6315native/4661machine/971diagnostics.

Superseded: v1 after/ had2775tests/534parse but4changedLua/3units included SickChild
coordinate reorder (36/180 failures); v1_rejected.json retained, not promoted.
v2 preview conservatively treated numeric tokens as unknown and was superseded
by v3 numeric recognition. Old seal.py disabled. Only v3/after_final accepted.
Unseeded initial Arena preview alias variation was absent from fixed-seed corpus;
384 countdown probes retained as ancillary evidence, not included in5272 count.

Next W18: work/codex_lua_copy_determinism_20261006/discovery.json reproduces two
copy-pass outputs across8 hash seeds from list(set) traversal of a three-local alias
chain. Refresh its baseline to final W17, then stabilize traversal and validate.
No W18 root edit yet. Continue authorized Lua work until the user asks to stop.


## W18 copy traversal integrated; validation active

work/codex_lua_copy_determinism_20261006 pins accepted W17 after_final inputs and
534Lua hashes. Root change: propagate_copies traverses sorted(declared|initialised)
instead of list(set); new test_readable_copy_determinism.py has6 cases, each across
8 child interpreter seeds, with actual value identity/captured-local/error checks.
Baseline2fail/4pass retained; candidate6pass. seed_comparison.json records current
W17 baseline2outputs versus candidate1; older discovery.json used W17 v1 hash.
Existing preflight1350tests+43subtests pass. Arena+Book preview27readable Lua parse,
all byte-identical to W17; both preview workers97874/85428 terminal0.

Integrated root inputs frozen; full corpus session50155 and broad2865tests/47modules
session50324 running. Final corpus after/, snapshots after_inputs/after_hashes.
Do not edit inputs while workers run. No W18 promotion/package/audit/runtime yet;
W17 conditional-evaluation-candidate-20261006 remains latest accepted bundle.
Finish hash/syntax/diff/behavior review before acceptance. If outputs unchanged,
reverify W17 promoted hashes without duplicate package/audit. Continue Lua work
until asked to stop; native lane and unrelated dirty workspace remain preserved.


## Copy traversal made repeatable (W18)

Completion: work/codex_lua_copy_determinism_20261006/completion.json; final after/,
snapshots after_inputs.json/after_hashes.json. Workers50155/50324 terminal0.
All29units/534Lua parse and are byte-identical to accepted W17 after_final.
2865tests/47modules pass with unchanged136 known Unicorn diagnostics.6new cases
exercise copy pass and full pipeline across8 interpreter hash seeds, plus value
identity/captured-local/error behavior; baseline2fail/4pass retained. Candidate
uses sorted binding traversal instead of unordered set iteration. Seed reproduction
has baseline2outputs versus candidate1; a second chain had4 baseline variants.
Existing preflight1350tests+43subtests includes14local/literal tests+22subtests
outside broad. Arena+Book27Lua preview matches both W17 and final seeded corpus.

No duplicate generated promotion/package/audit for unchanged output. W17 promoted
targets reverified; conditional-evaluation-candidate-20261006 remains latest offline
bundle. No new sidecar/install/game/save/native-lane/outer staging/commit activity.

Next W19: work/codex_lua_prefix_effects_20261006/discovery.json reproduces10/10
inline/pipeline ordering mismatches when a moved call crosses a preceding property
read, indexed read, arithmetic metamethod or arithmetic fault in the SAME return/
argument expression. W17 guards intervening statements and conditional uses, but
these prefix evaluations are not calls. Scratch only; no W19 root edit yet. Pin
accepted W18 and extend focused evaluation-order proof before integration.
User instruction remains continuous Lua work until asked to stop.


## Prefix effects and query ordering preserved (W19)

Completion: work/codex_lua_prefix_effects_20261006/completion.json. Final after/;
FINAL ROOT input snapshot is integrated_inputs.json (after_inputs.json describes
the unchanged W18 root used with injected scratch candidate during generation).
Candidate readable_style_candidate_v2.py is text-identical to integrated style;
only original line endings differ. Workers4887/50703/76076 terminal0.
29units/534Lua parse; drafts and11 unrelated units unchanged.64readable Lua and
18reports promoted by hash after complete3949-line diff review.3209tests/49modules
pass with unchanged136 Unicorn diagnostics;344new regressions,350root recheck.
Preflight1356tests+43subtests includes extra local/literal coverage outside broad.

Inlining now retains values before implicit prefix indexing/arithmetic and before
completed calls; sinking cannot cross query calls merely because their names look
pure. Initial prefix-only v1 passed focused tests but reordered level/max queries
in actual expressions. Superseded v1/preflight retained; only v2/final accepted.
Baseline focused failures312/320 plus12/24 query-order cases retained. More local
temporaries are intentional; no generated Lua hand edits.

8812 actual function/prefix/block scenarios: SickChild orientation180, expression
durations1620, reward chest2304, Orchard comment144, ChickenSign1344, Guild departure
Init40, crate-carrier lookup3072, Guildmaster health grade108. Each matches draft
within its documented scope. Reward chest restores checks at native EEC537/EEC644
calling CB7940 and branching EEC676. Duration data/level/max/FPS sequence now agrees
with draft (W17's earlier global-data-order exclusion corrected here).
All18 default smoke findings unchanged;3 expected query-order trace differences
(Orchard comment, ChickenSign, departure Init) covered by dedicated draft probes.
Prior stable-getter exclusions remain: Orchard caches timer ID, ChickenSign lacks
an in-loop cancellation query; neither is certified for changing those values
between calls. No full quest/cutscene/native exception/refcount/ABI proof.

prefix-evaluation-candidate-20261006:222parsed Lua/227manifest hashes. Same sidecar
161c39c/DLL967e7dc3; source/patch/DLL verified. Offline only, not installed/launched.
Ledger371/syntax0;7066generic/4593gotos/6317native/4669machine/971diagnostics.
No game/save/native-lane/outer staging/commit activity.

Next: investigate bare global/callee reads preceding an inlined effectful value.
Current prefix guards do not prove those reads stable if the moved call changes
their bindings. Reproduce first in owned scratch, retain W19 baseline, then fix
generically and validate. Continue authorized Lua work until asked to stop.

## Prefix binding order and query cancellation preserved (W20)

Completion: work/codex_lua_prefix_bindings_20261006/completion.json. Sole accepted
candidate v3/after; final ROOT snapshot integrated_inputs.json. after_inputs.json
describes unchanged W19 root with injected candidate. V1 superseded; V2 rejected
after OpenChest lost cancellation check (84/1296 draft differences). Neither was
integrated/promoted. superseded_candidates.json records exclusions.

Inlining now preserves mutable global/callee/method resolution order; literal
inert values and proven stable locals/parameters can still inline. Query names
no longer imply cancellation-neutral methods.104new behavior cases; two old positive
test modules use stable callee parameters where global stability was unjustified.
3313tests/51modules pass,136 unchanged diagnostics;1049 root recheck. Preflight
1700tests+43subtests.29units/534Lua parse; all drafts and picnic_after_wasp unchanged.
189readable Lua+28reports promoted by hash. Whole-corpus inventory/syntax/API review,
targeted branch/cleanup review and complete18107-line diff retained; not exhaustive
path coverage. More locals/checks and fewer folded branches are intentional.

16504 bounded actual function/prefix cases pass: SickChild180, expression durations
1620, reward chest2304, OpenChest prefix1296, Bandit massacre loading1728, debug-speech
nil-target exit8, Artefact Main1944, Chicken CreateSpectators3456, Beggar dialogue3968.
Native OpenChest cancellation windows retained.28unit default smoke findings unchanged;
two nontermination trace differences covered by dedicated probes. Query-cancellation
sweep6472 cases found0 newly divergent baseline-equals-draft cases and26 improvements.
Beggar raw ordinal-query probe retains188 pre-existing mismatches (none newly divergent
where W19 matched); scoped API-triggered probe coalesces adjacent polls with no API
between them. Stable getter exclusions and limits documented in individual reports.
No full quest/cutscene/native exception/refcount/ABI/runtime proof.

prefix-binding-candidate-20261006:256parsedLua/261manifest hashes. Same sidecar
161c39c/DLL967e7dc3/patchb885fe verified. Offline only, not installed/launched.
Ledger371/syntax0;7728generic/4807gotos/6551native/4710machine/971diagnostics.
No game/save/native-lane/outer staging/commit activity. Workers31336/56492/67244 terminal0.

NEXT: reproduce implicit operator/index/metamethod effects across cancellation checks.
_pure_line currently looks for explicit calls; a Lua operator or index can execute
metamethods and change cancellation state. Establish adverse baseline in new scratch,
then fix generic proof if reproduced. Continue authorized Lua work until asked to stop.


## Implicit evaluation and cancellation in progress (W21, scratch only)

work/codex_lua_implicit_cancellation_20261006 is owned scratch. Root remains accepted
W20; no W21 integration/promotion/package. Baseline177focused cases:137fail/40pass.
V1 fixed cancellation purity but68pipeline cases still lost unused implicit expressions.
V2 preserves them,177pass,1700preflight+43subtests pass; broad3490cases has2failures:
private integer-mask cleanup no longer eliminated, and existing EndTrader test expected
movie2 when W20 correctly cancels at movie1. V2 after/ is superseded, never promote.
V3 preserves removable integer-only masks with private tail aliases; negative test
caught _ENV alias. V4 rejects _ENV and passes177focused+37mask tests (11new alias cases).
V3 and V2 preflight1700+43 pass; V4 preflight/broad ongoing. Current candidate is
readable_style_candidate_v4.py; final corpus target after_final/, snapshots *_final.

EndTrader existing mock: W20 matches draft's early movie1 cleanup for both speech
results; W19 proceeded incorrectly to movie2. endtrader_existing_mock.json retains
exact comparison. Revised scratch test keeps early exit assertion and explicitly
delays cancellation to speech2 for late cleanup,3tests pass. Root test unchanged
until W21 integration. Important: this test reads promoted output, so rerun it AFTER
future promotion as well as before. W20 broad ran this test against W19 output;
this is a subsequent generated-output expectation correction, not a new W21 bug.

Current workers:92303 V2 corpus finishing,74693 V4 broad; V4 preflight also running.
Do not change frozen candidate/test/root inputs while workers read them. V4 corpus
helper prepared but not launched yet. Finish V2 to preserve its comparison, then
run corpus_v4.py; inspect generated changes and smoke traces, validate actual changed
paths, integrate/recheck, promote/package/audit/seal only after review. All scratch
versions retained. No game/save/native lane/outer staging activity.

Additional unresolved scope: bare global reads and global stores can trigger _ENV
metamethods; query-named calls removed as unused results also need separate evidence.
This candidate addresses explicit indexing/operator evaluation and safe integer masks.


## W21 follow-up: native pointer recovery and self-read preservation (scratch)

This supersedes the earlier W21 v4 progress checkpoint. Root remains accepted W20;
no W21 generator integration, output promotion, package, installation, or game run.
Scratch: work/codex_lua_implicit_cancellation_20261006.

V4 passed3501 tests/53modules but its corpus review exposed retained arithmetic
whose initializer was incorrectly pruned on a self-read. V5 removes that shortcut:
208focused tests pass (177implicit evaluation,11mask aliases,20self-read cases),
1700preflight+43subtests pass. Broad V5:3540tests/55modules pass, including19new
parent-state recovery tests; exception diagnostics remain recorded per module.
after_native/ and after_final/ use V4 and are not promotion candidates.

Native-reviewed SingingStones array rewrites now remove only the verified obsolete
piVar6/puVar7 address cursor setup and increment, after all dereferences are mapped.
Unknown remaining uses fail closed.16tests+27subtests and2048actual tune cases pass.
The old playlist comparison test expected an unsuffixed name; unchanged baseline
also fails it, while the established suffix tests agree with BBCD. Scratch test
corrected; root unchanged. First tune probe used inconsistent frame/cancellation
mock behavior and was terminated; corrected bounded mock passed. Retain this limit.

ChickenMaster E64FB0: three nonescaping parent-pointer OR stores recovered to
PrizesWon Int at parent offset0x5c. Retail OR sites E687D2(mask1),E685C5(mask2),
E683F1(mask4); receiver/load bytes and body hash are in conversion evidence.
native_parent_scalar_updates_candidate.py requires matching adjacent native load/OR,
preserved ESI receiver, mapped Int field, unique store and no escaping alias.
19focused tests and4608actual extracted first-prize branch cases pass, including
cancellation/text/write faults. These are branch tests, not full quest/ABI proof.
The top-prize string assignment remains unresolved. Scope scan:1973functions in
29registered units; only ChickenMaster has this source pattern (3stores).

Current candidates: readable_style_candidate_v5.py, convert_quest_unit_candidate.py,
native_parent_scalar_updates_candidate.py, native_state_pointer_cleanup_candidate.py,
and two native_reviewed_singing_*_candidate.py wrappers. All remain scratch.
V5 corpus worker29456 active: after_v5/, snapshots *_v5. Regenerates ChickenKicking,
SingingStones and unrelated Picnic drafts; other draft hashes verified/reused after
scope scan. On completion run audit_v5.py, smoke_v5.py and query_cancel_corpus_v5.py;
review changed paths and distinguish preexisting unresolved errors. Earlier V4 smoke
findings were unchanged with extra cancellation queries in SkillApprentice.Main,
WillApprentice.Main and CampHostageGuard.Main; strict exact-trace gate remains open.
Do not promote until final corpus/behavior review, root integration/recheck and
postpromotion output-reading native tests pass. No staging or native-lane edits.


## Implicit evaluation and native parent flags preserved (W21 accepted)

Completion: work/codex_lua_implicit_cancellation_20261006/completion.json.
Accepted corpus after_v5/; final root snapshot integrated_inputs.json. Earlier
after/,after_final/,after_native/ are superseded. Generator now preserves implicit
index/operator effects on cancellation and faults, including initializer reads
needed by retained self-updates. Integer-only private flag aliases remain removable.

ChickenMaster three parent PrizesWon OR stores recovered from native bytes;
SingingStones four obsolete verified native array cursor operations removed.
These are the only changed drafts. Parent-pattern scan:1973functions/29units,
only3ChickenMaster stores. Unrelated Picnic separately regenerated byte-identical
(corpus helper used shorthand picnic; its final draft was hash-verified reuse).
239new focused cases;3540tests/55modules pass,136known diagnostics. Root recheck
272tests+27subtests; postpromotion native tests7+27subtests pass. Stale EndTrader
and SingingPlaylistCompare expectations corrected as documented in review.

29units/534Lua parse.49readable+2draftLua across17units promoted,70files including
reports, all hashes verified.7976bounded actual cases:4608prize-branch,2048tune,
1320cleanup/cancellation path cases;1896default query-cancellation cases found no
newly divergent baseline-equals-draft case.17unit smoke findings unchanged; exact
extra termination queries in SkillApprentice,WillApprentice,CampHostageGuard retained
and reviewed. No other smoke API-trace changes. Whole-corpus API inventory and
targeted diff/branch review, not exhaustive path proof. Raw path probe432preexisting
draft differences; consistent-frame/numeric-normalized probe retains408preexisting
differences (WillApprentice264,TraderConflictEvil144); reports preserve all traces.
No full quest, native exception/refcount, sidecar ABI or game proof. Top-prize string
store and unrelated unresolved diagnostics remain visible. Earlier failed/terminated
scratch attempts and obsolete versions explicitly excluded in review_validation.json.

Offline bundle implicit-evaluation-candidate-20261006:207parsedLua/212manifest hashes.
Same sidecar161c39c/DLL967e7dc3/patchb885fe verified; not installed/launched.
Ledger371files/syntax0;7751generic/4831gotos/6575native/4738machine/968diagnostics.
No game/save/native-lane/staging/commit activity. All W21 workers terminal.

NEXT: investigate bare global reads/stores and _ENV metamethod effects in generator
purity/dead-definition proofs; also unused query-named call effects. Start new scratch
from W21 snapshot. Continue authorized Lua work until asked to stop.


## W22 environment effects in progress (scratch only)

Scratch: work/codex_lua_environment_effects_20261006. Root remains accepted W21;
no W22 integration/promotion/package. Bare global access can execute _ENV metatable
hooks. Expanded78case baseline37fail/41pass; v1 fixes reads/stores in cancellation
purity and unused-definition proofs,78pass; preflight1700+43subtests pass.
Additional14scope cases expose6v1 dead-definition failures: local _ENV binding has
implicit users invisible to textual read analysis. V2 pins _ENV in Flow and passes
all92focused cases. V1 is not a promotion candidate; preserve its outputs/results.
Current candidate readable_style_candidate_v2.py. Proven local/parameter and opaque
literal positives retained. Candidate changes remain separate from root.

Workers:40880v1broad and90619v1corpus (after/) active;47452v2preflight active.
Do not mutate their inputs. Finish and record; run prepared v2broad helper afterward.
Verify final candidate corpus (or rigorously record equivalent no-_ENV input reuse),
review syntax/API/smoke/actual changed paths, integrate/root-test then promote only
if acceptance holds. No game/save/native-lane/staging/commit activity.


## W22 scope guard follow-up (scratch; supersedes earlier progress)

Root remains accepted W21. All W22 work is in codex_lua_environment_effects_20261006.
Current combined candidate: readable_style_candidate_v2.py plus
lua_binding_guard_candidate_v3.py, readable_lua_candidate.py and
build_readable_unit_candidate.py; injection inject_binding_guard_v3.py.
candidate_inputs_guard_v3.json seals current candidate/test files. No integration.

Style v1 passed78focused,1700+43preflight,3599broad/55modules,full29unit after/.
Style v2 pins _ENV (6direct dead-def failures fixed),92focused,1700+43preflight,
3613broad/56modules. Neither style-only candidate is final: review found local-name
renaming also changes global references before/outside that local's lexical scope.
8initial name cases failed both direct rename and full pipeline. Guard preserves
ambiguous source in readable_function/readable_source/readable_file.

First guard passed100then120focused and1700+43preflight, but broad3641/58modules
had5Oakvale failures: Wife exact-source witness rejected a changed intermediate.
This was a NEW guard regression, not baseline; witness unchanged. Root stayed W21.
Guard v2 models deferred callback initializer scope and the affected Oakvale deeds
test passes2cases with existing Unicorn diagnostics. Additional table-initializer
case then failed1/123; guard v3 adds table delimiter and function nesting tracking.
Current125focused cases pass. Full source scan identifies only2draft files to
preserve: ArenaSpawn(v_stk_14 read before declaration),TrophyDealer(bVar3 before
declaration). These are existing draft ambiguities now made visible; no native fix
or native equivalence claimed. Inline generated array constructors remain supported.

Workers89300final guard-v3 broad and17266final corpus after_guard_v3/ are active.
Earlier workers40880/90619/47452/35490/1391/84938/92721/26945 terminal; v1after/
and first-guard after_guard/ remain superseded. v2corpus helper was never launched.
Finish final broad/corpus, final preflight, audit/smoke/API and actual path review;
investigate any changed findings. No promotion until current candidate passes.
No game/save/native-lane/staging/commit activity.


## W22 integrated; generated promotion pending

Current W22 source/evidence/tests are integrated; exact inputs: work/codex_lua_environment_effects_20261006/integrated_inputs.json.
W21 generated output remains in place. Final scratch corpus after_native/:29units,
534parsedLua;34readable+1draft changes across16units. Only changed draft is
WomanToAttract: native-gated self reload aliases replace2unaff_EBP reads and1join.
All other28unit corpora hash-reused from guard-v3; no candidate mutation.
Guard-v3 broad3646/58 and1700+43preflight pass. Woman focused suite5314tests had
45dispatch failures reproduced without metadata change (test missed givenObject_);
4response negatives used positional evidence edit lookup. Scratch test corrections
assert state initialization and select exact existing response operands. Rechecked
391dispatch+2152response+10new residual-self negatives pass with actual subprocess
exit0; remaining2771native cases pass. Root isolated validation now running.
Root SickChild plus unrelated Picnic regeneration also running.

16unit smoke findings and nontermination API traces match baseline. Strict smoke
exits1 only for restored termination-query traces in LookoutPointBeggar and WaspHelper;
raw report retained. Earlier smoke's new unaff_EBP free global resolved by native
evidence, not hidden by readable pruning. Entity labeling uses path consistently.
11616bounded Main-path scenarios (1056+2816+7744),0new baseline-equals-draft
divergences;1200default query-cancellation cases also0new. Preexisting draft
differences retained. Not full/native/ABI/game proof. No promotion/package yet.
Next: await root checks, seal review, promote tested outputs, package/audit/document;
then continue authorized Lua work. No game/save/staging/commit activity.


## Environment effects and lexical binding guard (W22 accepted)

Completion: work/codex_lua_environment_effects_20261006/completion.json.
Final corpus after_native/; root snapshot integrated_inputs.json. All previous W22
corpora are superseded. Generator preserves unknown global reads/stores and _ENV
effects; lexical guard retains source when local/global names overlap across scopes.
ArenaSpawn and TrophyDealer retain draft bodies because of existing pre-local reads.
WomanToAttract residual self copies recovered from existing native self spill/reload
evidence with exact counts and unchanged full body/source/instruction gates.
Only WomanToAttract draft changed; all534Lua parse,34readable+1draft across16units.
53files including reports promoted and hash-verified. Root SickChild and unrelated
Picnic scratch regeneration reproduce tested Lua byte-for-byte.

135new focused cases;3646tests/58modules pass,136known Unicorn diagnostics;
1700+43preflight. Root5449tests pass (5314native,10new native,125generator).
Six postpromotion tests pass. Native dispatch test45failures reproduced on baseline:
test setup missed givenObject_; corrected with explicit setup assertion. Response
negative tests now choose exact operands instead of last-six positional edits;
new residual alias edits had exposed4stale positional failures. All raw reports kept.
11616bounded Main scenarios plus1200default query-cancellation cases:0new
baseline-equals-draft divergences.920preexisting mock differences retained in review.
16unit smoke findings and nontermination API traces unchanged. Strict smoke exit1
only for restored termination queries in LookoutPointBeggar/WaspHelper; reviewed
against drafts. Full diff/API inventory reviewed, not exhaustive path/native proof.
No full quest, native exception/refcount, sidecar ABI or in-game equivalence claim.

Offline bundle environment-binding-candidate-20261006:209parsedLua/214manifest
hashes; unchanged sidecar161c39c/DLL967e7dc3/patchb885fe verified. Not installed.
Ledger371files/syntax0;7806generic/4845gotos/6600native/4784machine/967diagnostics.
Conservative retention increases some readability metrics; no unresolved behavior
hidden to improve counts. All W22 workers terminal. No game/save/staging/commit.

NEXT: continue Lua recovery from W22. Investigate the two guarded native pre-local
reads (ArenaSpawn v_stk_14, TrophyDealer bVar3) to resolve their origin at the evidence
layer and permit ordinary readability again if native evidence supports it.


## W23 native pre-local recovery in scratch

Scratch work/codex_lua_prelocal_recovery_20261006; root still accepted W22.
Evidence-only candidates Q_Arena_candidate.json and V_TrophyDealer_candidate.json
use existing reviewedThingOutputScopes adapter. No shared generator edits.
Arena native F1B784 effect call has7words: hidden return buffer, effect name,
position, empty script name, angle0, independent0, alwaysUpdate0. Current draft
wrongly supplies timer+4 as angle and0.0 as independent. Candidate corrects args
and removes unused address-only p0 initialization; preserves timers/result alias.
Trophy EE703B/EE703E guard tests thread-name construction bit4 and only destroys
name atEE7044; allocation/thread lowering had removed producer but left empty
Lua if-not-bVar3. Candidate removes only that native-owned cleanup guard.
Rows pin full native/export/intermediate/recovered hashes, instruction anchors,
dependencies and unchanged sidecar sources. Native emulation verifies Arena's
7stack operands, result cleanup receiver and timer reload; Trophy name cleanup
on allocation success/failure and earlier cleanup bits. Not allocator-failure,
native refcount/exception, sidecar or in-game equivalence proof.

Baseline168behavioral cases:56fail112pass. Candidate199focused pass;
expanded targeted246pass,7Unicorn exception diagnostics, subprocess exit0.
Full2unit regeneration running worker26605 into after/ using copied evidence.
Tests/metadata remain scratch; root untouched. Next compare generated outputs,
inspect smoke and actual paths, integrate evidence/tests only after checks,
promote/package/audit/document. Continue authorized Lua work until asked to stop.


## Native pre-local recovery (W23 accepted)

Completion: work/codex_lua_prelocal_recovery_20261006/completion.json.
Final corpus after/ contains2units; other27 remain W22 after_native/.
Root snapshot integrated_inputs.json. Existing reviewedThingOutputScopes adapter
unchanged; only two unit evidence rows plus tests/fixtures added.
ArenaSpawn F1B784 now receives native angle0, independentfalse, alwaysUpdatefalse;
the hidden result-buffer address no longer becomes timer+4 in Lua's angle slot.
Unused address-only p0 initialization removed. TrophyDealer's already-consumed
thread-name cleanup guard removed (EE703B bit4, destructorEE7044), eliminating
global bVar3 read. Both scripts now pass lexical guard and normal readability.
Rows pin native/export/intermediate/output hashes, instructions, dependencies and
sidecar sources. Exact converter intermediate tests ensure rows still apply.

Baseline168behavioral cases56fail112pass;201newfocused plus47existingtests pass,
248root pass. Seven bounded native windows validate Arena arguments/timer reload/
destructor receivers and Trophy allocation-bit/name cleanup paths.7known Unicorn
exception diagnostics, subprocess exit0.54Lua across2units parse; only2draft and
2readable changed.6files including readable reports promoted, hashes verified.
All534W22rootLua verified before promotion; no shared generator changed. The
Trophy teleporter helper after normal readability equals its acceptedW21 body.
Both smoke findings and complete default API traces unchanged.704bounded Main
scenarios all match corrected drafts;64query-cancellation cases0new divergence.
Full four-file diff reviewed. Mocks/native-window callees are bounded/stubbed;
no allocator failure, native refcount/exception, sidecar ABI or full quest proof.

Offline bundle prelocal-native-candidate-20261006:46parsedLua/51manifesthashes.
Same sidecar161c39c/DLL967e7dc3/patchb885fe verified; not installed/game-tested.
Ledger371files/syntax0;7806generic/4845gotos/6598native/4738machine/967diagnostics.
All W23workers terminal. No game/save/native-lane/staging/commit activity.

NEXT: scan existing registered native exports for other arithmetic CScriptThing
hidden-return operands like ArenaSpawn's local_14+4 and check existing Lua call
operands against their native evidence. Do not select new conversions.


## W24 hidden output scan and Guild spawn flag (scratch)

Scratch work/codex_lua_hidden_output_scan_20261006; root remains accepted W23.
Read-only scan1973functions/29registered units found28functions with arithmetic
stack Thing pointers; these include legitimate copy/list patterns, not28bugs.
Confirmed GuildTrainingWill.TheRealGuildmaster.Main D5E0C0 callD6198C receives
nativeFlag=false (push0 atD61967), but current Lua supplies "MeleeApprentice".
The arithmetic hidden return auStack_64+4 was not recognized and argument placement
shifted a name into Boolean position. Candidate Q_GuildTrainingWill_candidate.json
uses existing unchanged reviewedThingOutputScopes adapter, one exact operand edit.
Full native/export/intermediate/output hashes, native setup anchors, callee8A9100
(5wordpurge), table slot16C and sidecar signatures pinned. No shared generator edit.

Initial literal-Boolean scanner120hits incorrectly removed explicit persistence
context and mixed receivers; literal_bool_arguments.json is superseded raw output.
Corrected explicit-quest scanner keeps context:267files,1finding (this Guild call).
It does not audit arbitrary expressions or overloaded nonquest receivers.
Baseline24graduation-block cases18fail6pass; candidate38focused pass (includes
native call-stack execution and stale-evidence rejection),1Unicorn diagnostic,
exit0. Added converter intermediate check =>39new cases, expanded86targeted run
worker48522 ongoing. Full Guild unit scratch regeneration worker82146 ongoing.
Tests/candidate data not integrated, no outputs promoted or package created.

Next: compare full Guild output/smoke, inspect native operands and bounded paths,
integrate data/tests if clean, promote/package/audit/document. Existing root W23
and game/save/native lanes untouched. Continue authorized Lua work until stopped.


## Guild spawn flag recovery (W24 accepted)

Completion: work/codex_lua_hidden_output_scan_20261006/completion.json.
Final corpus after/ contains Guild; Arena/Trophy remain W23, other26 W22.
Root snapshot integrated_inputs.json. Existing evidence adapter and generator unchanged.
GuildTrainingWill.TheRealGuildmaster.Main CreateCreature D6198C now receives
nativeFlag=false, proven by push0 D61967 and five-word caller stack. Arithmetic
hidden result auStack_64+4 had shifted the script-name string into Boolean position.
Evidence pins native/export/intermediate/output hashes, native anchors, callee,
vtable and sidecar sources; exact converter intermediate and stale evidence tested.

39new cases plus47existing pass in scratch/root (86 total), one known Unicorn
diagnostic, exit0. Baseline24graduation block cases18fail6pass. Native caller
emulation validates five arguments and balanced stack using stubbed callees.
76Lua parse; only two one-line draft/readable changes. Three files promoted,
including readable report; hashes verified. Smoke findings and complete API traces
unchanged.352bounded Main scenarios match draft;32query cancellation cases0new.
These are bounded offline tests, not full graduation, allocator/refcount or ABI proof.

Offline bundle guild-spawn-flag-candidate-20261006:58parsedLua/63manifesthashes.
Same sidecar161c39c/DLL967e7dc3/patchb885fe verified, not installed/game-tested.
Ledger371files/syntax0;7806generic/4845gotos/6598native/4738machine/967diagnostics.
All W24workers terminal. No game/save/native-lane/staging/commit activity.

Scan1973functions/29units found28 arithmetic Thing-pointer candidates, including
valid copy/list cases. Corrected explicit-quest Boolean literal scan267files had
one finding (repaired here); initial mixed-receiver120hits are superseded.
NEXT: inspect remaining registered call operands with native evidence and broaden
typed literal checks. Generic incomplete pushedStack alignment requires independent
native proof; do not pad missing entries (RunTutorials D46A73 export depth drifts).
Continue authorized Lua work until stopped; no new conversions selected.


## W25 integer-to-string result kind (scratch)

Scratch work/codex_lua_typed_operands_20261006; accepted root remains W24.
Typed literal scan267readablefiles found one numeric Boolean argument in existing
Global_DebugCycleThroughSpeech.Main EE91E0 AddScreenTitleMessage EE92FF.
Native EE92EF push1, EE92F1 pushEBX (zero), EE92F6 GFIntToCharString99F830,
EE92FE pushEAX establish message/duration0/true. Current output shifts message
into duration and uses earlier DebugGetAllTextEntries result as the message.
Trace finds ENGINE_IntToString omitted from pseudo-call string-result table;
it defaults to number. Candidate adds it to that table only. Earlier v1 changed
generic assignment numeric inference and had no effect; retained as superseded.
Actor copy and text-entry list remain unresolved in this debug script. Candidate
does not claim to recover the whole script or make its message executable.

Root inputs snapshot before_inputs.json (W24). Candidate root source unchanged.
18focused cases: baseline15fail3pass; candidate focused plus existing concat and
native string return tests running worker96908. Full29unit draft/readable scratch
regeneration running worker73187 (3process pool), inputs/candidate frozen.
No W25 integration, promotion, package, ledger audit or game activity.
Next: inspect terminal test/corpus results and every changed Lua, strengthen
converter regression and stale-message fixture, validate affected paths/smoke,
then integrate/promote/package only if clean. Continue authorized Lua work.


## Integer-to-string result kind (W25 accepted)

Completion: work/codex_lua_typed_operands_20261006/completion.json.
Final corpus after/ contains all29registered units; root integrated_inputs.json.
Shared Lifter pseudo-call type table now classifies ENGINE_IntToString as string.
Existing native helper identity99F830 gate unchanged; no arithmetic/ABI changes.
This prevents a stale string result from displacing the message and shifting later
arguments in Global_DebugCycleThroughSpeech.Main AddScreenTitleMessage EE92FF.
Native EE9206 zeroesEBX, EE92EF pushes1, EE92F1 pushesEBX, EE92F6 convertsinteger,
EE92FE pushesreturnedstring. Correct operands are message, duration0, true.

19new focused cases baseline16fail3pass; candidate passes.233distinct cases
covered across initial55/expanded196runs;196root checks pass. Initial tests8,
expanded1, root1 known Unicorn diagnostics; every candidate/root process exit0.
Tests cover signed/arithmetic integer input, actual stale strings, unresolved
integer memory, wrong helper identity and registered converter output. All29units
regenerated;534Lua parse; only DebugCycleThroughSpeech draft/readable changed.
Full diff reviewed: title argument order and removal of an unused result local.
Default smoke complete traces/findings unchanged. The debug script still exits
before this loop in default smoke; actor copy, list traversal and Speak resource
remain unresolved and explicitly diagnosed. No full script/native lifetime proof.

3files promoted and hash-verified. Offline bundle integer-string-kind-candidate-20261006:
34parsedLua/39manifesthashes. Same sidecar161c39c/DLL967e7dc3/patchb885fe
verified; not installed/game-tested. Ledger371files/syntax0;
7804generic/4845gotos/6598native/4738machine/967diagnostics.
Superseded v1 generic assignment candidate had no effect; retained. Initial18case
source also retained; final19case source strengthens stale-string and converter proof.
All W25workers terminal. No game/save/native-lane/staging/commit activity.

NEXT: review other pseudo-call result kinds against existing lowered operations
and their native evidence. String equality currently defaults to number in the
type table, but its existing callers may already normalize predicates; establish
a real affected registered path before changing it. No new conversions selected.
Continue authorized Lua work until stopped.


## W26 inline receiver assignment guard (scratch)

Scratch work/codex_lua_value_transfers_20261006; accepted root W25 unchanged.
Unresolved word-store inventory267drafts finds debug actor copy and chicken
scoring vector store. Inspection of KickedChicken Main E64210..E64D03 found a
CScriptThing marker-array GetPos call E64A36 emitted as quest:IsXbox().
Native E649D5 leaEBP,[EAX+12], E64A31 movEAX,[EBP], E64A34 movECX,EBP,
E64A36 call[EAX+18], E64B95 addEBP,12 prove iterator receiver, not GSI.
The iterator's assignment occurs inside an if condition. _is_gsi_alias_at only
searched standalone assignments and mistook absence for a hoisted GSI load.

Scratch annotate_interface_slots_candidate.py rejects intervening simple inline
assignments before assuming interface provenance. Quoted text/comments masked;
a later proven standalone interface definition restores provenance. Alias chains
also reject these iterator definitions. Existing unknown/hoisted receiver policy
otherwise unchanged.20new tests baseline12fail8pass;161candidate tests pass.
Raw annotation scan1973functions/29units changes exactly one IsXbox annotation,
KickedChicken Main. No other raw annotation changes; final corpus still pending.

Full29unit scratch draft/readable generation worker40803 ongoing (3process pool).
Root/candidate inputs frozen and pinned; before/ holds534accepted Lua snapshots.
Other workers terminal. No integration/promotion/package/audit/game work yet.
Native scoring captured in chicken_native.json, chicken_instructions.json,
chicken_lowered.c and chicken_intermediate.lua. Landing position is not yet bound
to its output vector; marker list, normalization/rotation/distance remain partly
unresolved. This guard preserves native-call diagnostics instead of inventing an
IsXbox call; it does not repair the whole chicken-scoring function.

Next: add/verify converter regression, inspect terminal corpus/diffs and smoke;
promote only if the exact false annotation disappears without unrelated damage.
After acceptance, recover geometry/landing outputs only with native evidence,
bounded execution and sidecar signature checks. No new conversions selected.
Continue authorized Lua work until stopped.


## Inline receiver assignment guard (W26 accepted)

Completion: work/codex_lua_value_transfers_20261006/completion.json.
Final corpus after/ contains all29registered units; root integrated_inputs.json.
_is_gsi_alias_at now rejects recognized simple inline assignments before assuming
a hoisted interface load or using an earlier standalone definition. Quoted text
and comments ignored; later proven interface loads restore provenance. Alias
chains inherit the guard. Other unknown/hoisted receiver policy unchanged.

Native KickedChicken E64210..E64D03 uses a marker-array iterator: E649D5 advances
12bytes, E64A31 loads current entry vtable, E64A34 puts iterator in ECX, E64A36
calls slot18, E64B95 advances12bytes. Its initialization was inside a condition;
missing that definition caused the Thing call to be misnamed GSI IsXbox.
The corrected output retains this native operation as TODO/nil, without calling
unrelated IsXbox. Marker traversal, geometry and landing output remain unresolved.

21new tests baseline13fail8pass;162candidate and162root checks pass, no exception
diagnostics. Includes exact native witnesses, aliases, comments/quotes, later
definitions, raw exported function and registered converter regression. Scan of
1973raw functions changes exactly one annotation. All29units regenerated;534Lua
parse; only KickedChicken draft/readable changed. Default smoke complete traces
and findings unchanged; these mocks do not reach the unresolved geometry. No full
scoring/quest, native lifetime or sidecar ABI proof. Added diagnostic is intentional.

5files promoted/hash-verified. Offline inline-receiver-guard-candidate-20261006
bundle: 27parsedLua/32manifesthashes. Same sidecar161c39c/DLL967e7dc3/patchb885fe
verified, not installed/game-tested. Ledger371files/syntax0;
7804generic/4845gotos/6598native/4738machine/969diagnostics.
All W26workers terminal. No game/save/native-lane/staging/commit activity.

NEXT: recover existing EntityGetShotStrikePos output-vector binding. Evidence in
landing_output_evidence.json: KickedChicken E647C9 slot71C, target89DF70, pushes
actor and output vector at stack-12; Boolean AL tested E647CF. Sidecar returns
table(x,y,z) on success and nil on failure. Current converter keeps only returned
object in bVar1 and loses xStack_c, which scoring later reads. Bounded callee
89DF70..89DFF0 has two ret8 exits (landing_callee_bounds.json), copies output only
on success. Generic purge helper returnsNone because its linear sweep enters the
unpadded next function. Remaining
vector/list/geometry operations also need native evidence before full scoring.
No new conversions selected; continue authorized Lua work until stopped.


## W27 landing output vector (scratch)

Scratch work/codex_lua_landing_output_20261006; accepted root remains W26.
Candidate lift_native_lua_candidate.py adds optional_vector_output_call for the
reviewed EntityGetShotStrikePos contract in unit mode: known Thing actor, exact
C3DVector stack output, matching manifest. Query table/nil is captured once;
Boolean flag is separate; xyz copied to destination only on success. Failure
preserves prior destination. Unknown/occupied/overlapping output shapes produce
explicit diagnostics, not invented arguments. Proven vector slices ._0/4/8_4_
read as .x/y/z; quoted strings and unknown vector slots remain untouched.
Sidecar unchanged. Existing KickedChicken caller E647C9 outputs native stack-12;
callee89DF70..89DFF0 has two ret8 exits. Native copies three floats from message+8
only on success; false exits preserve output. Full body SHA/native vtable pinned.

53new cases pass (1known Unicorn diagnostic), baseline53fail. Thirty draft/readable
cases compare to bounded execution of actual native callee, with actor lookup,
map lookup and message retrieval stubbed; finite coordinates and four failure modes.
Eight repeated query paths validate last successful vector survives failure.
Also covers native cast spellings, unknown slice/string preservation, invalid or
aliased outputs, generated-name collision and actual registered converter output.
Initial native fixture placed coordinates at message+0, causing4failures; native
89DFD3 addEDX8 proves header skip. Fixed fixture passes; original source/results
retained as *_payload_offset_before. No generator change to mask that failure.

Expanded298checks (including33subtests) had1existing test failure: old creature
spawn mock accepts3args although accepted output already includes false flag.
Same root baseline test fails identically. DBEA70 push0 proves flag; scratch
test_vector_fields_repaired.py captures/assertsfalse. All3repaired tests pass on
baseline. Expanded repaired run worker89390 ongoing; original report retained.
Full29unit scratch regeneration worker85821 ongoing; all inputs/candidate frozen.
before/ contains534accepted Lua; root unchanged. No promotion/package/audit/game.

Probe binds xStack_c and Boolean separately; y component consumer now uses .y.
Unresolved fStack_24 assignment, marker list, normalization, rotation and distance
remain explicit. This is output-contract recovery, not complete chicken scoring.
Next: inspect terminal expanded/corpus results, all diffs/smoke and bounded paths;
integrate generator,53tests and proven stale-test repair only after clean validation.
No new conversions selected; continue authorized Lua work until stopped.


## Landing position output recovery (W27 accepted)

Completion: work/codex_lua_landing_output_20261006/completion.json.
Final corpus after/ contains all29registered units; root integrated_inputs.json.
EntityGetShotStrikePos now produces one optional-vector query, a separate Boolean
flag, and a fresh xyz copy on success; failure preserves the previous output.
Only unit-mode known Thing actor/exact C3DVector stack output/matching manifest
are accepted. Ambiguous or occupied aliases retain explicit diagnostics. Proven
vector byte slices map to xyz; unknown offsets/slots and quoted text preserved.

KickedChicken Main E647C9 calls slot71C target89DF70 with actor and output at
ESP+6C, testsAL atE647CF. Bounded89DF70..89DFF0 body has two ret8 exits and copies
xyz from message+8 only on success. Native body/vtable hashes pinned. Generic
stack-purge heuristic and sidecar unchanged. Thirty new cases execute this real
native body with three external actor/map/message calls stubbed; three positions
and five success/failure modes are compared with draft/readable Lua. Eight more
repeat queries to check preservation. Casts/components/negative provenance/name
collisions and actual registered converter output also covered.

53new cases fail baseline/pass candidate. Expanded candidate and root298checks
(265tests plus33subtests) pass, with2Unicorn Windows exception diagnostics each.
Initial suite had one existing spawn mock arity failure, reproduced on baseline;
native DBEA70 push0/DBEA85 slot16C proves the accepted fourthfalse argument. Mock
now captures/asserts it; all3repaired cases pass baseline. Initial native fixture
used message+0 instead of+8, causing4failures; native89DFD3 proves correction.
Original failure artifacts retained. No generator change hid those failures.

Full29units/534Lua parse, only KickedChicken draft/readable changed. Default smoke
findings and full traces unchanged. 4files promoted/hash-verified.
Offline landing-output-candidate-20261006 bundle: 27Lua/32manifesthashes.
Same sidecar161c39c/DLL967e7dc3/patchb885fe source/build hashes verified. Not installed
or game-tested. Ledger371files/syntax0; 7807generic/4845gotos/
6598native/4738machine/969diagnostics.

Limits: no full scoring/quest, native lifetime or installed ABI proof. Default
smoke does not reach geometry. Scalar x subtraction, marker traversal,
normalization, rotation and distance remain unresolved. NEXT: recover the
discarded fStack_24 x subtraction; native E6488C..E648A9 forms planar displacement
and E648AD calls A14440. Verify scalar storage and this callee before changing
geometry. All W27workers terminal; no staging/commit/native-lane changes.
No new conversions selected; continue authorized Lua work until stopped.


## W28 planar displacement stores (scratch)

Scratch work/codex_lua_planar_displacement_20261006; accepted root remains W27.
Native E6488C..E648A9 subtracts landing/arena x and y, stores float32 at
ESP+54/+58, zeroes z at+5C, then calls A14440. Prior converter drops fStack_24
x subtraction; existing y subtraction retains excess Lua precision. Candidate
v2 recognizes only unit-mode fStack_/f_stk_ destinations with independently
proven vector-component subtraction, rejects occupied aliases/non-scalar kinds,
and emits actual scalar stores with explicit string.pack/unpack float32 rounding.
No normalization or marker-list behavior is inferred from method labels.

52focused cases pass (1Unicorn diagnostic); baseline38fail14pass. Includes26
draft/readable native-block comparisons, x/y halfway rounding boundaries, all9
component pairs, conditional overwrites, negative operands/aliases and actual
registered conversion. Initial44exact-coordinate cases passed v1, then4added
rounding cases failed. v1 preserved, not accepted. Initial test fixture bypassed
literal staging when appending its return; observation now uses lifter.expr,
with the initial18failure artifacts retained. Candidate unchanged for that fix.

v2 expanded350checks initially had one W27 source-shape assertion failure because
the y subtraction is now wrapped for float32. Scratch landing contract assertion
still checks the exact vector-component subtraction; W28 independently requires
the narrowing wrapper. Updated expanded run worker59170 ongoing. Original result
retained in expanded_*_v2_contract_before. V1expanded342passed/3diagnostics.
V1corpus worker92873 and final v2corpus worker33570 ongoing; both have separate
candidate/output/hash artifacts. Root/candidate inputs remain frozen. Final
candidate lift_native_lua_candidate_v2.py; final corpus will be after_v2/.
Do not promote v1. No W28 root integration/promotion/package/audit/game yet.

NEXT: inspect terminal final broad/corpus reports, compare all534Lua, review
changes and smoke. Update helper paths/counts for v2/52new/350checks, preserve
the landing-contract assertion adjustment, then integrate only after validation.
Native next callee A14440..A1447D computes reciprocal vector length and scales
xyz in place; A1447C ret. Zero-vector/precision behavior needs native execution
before recovery. Evidence native_evidence.json includes the caller and callee.
Continue authorized Lua work until stopped; no new conversions or staging.

