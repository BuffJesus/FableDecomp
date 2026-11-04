# Lua recovery resumed - 2026-10-05

The user authorized continuous Lua work until stopped, including launches when
Fable 2 is not open. This replaces the earlier request to ask before visual
launches. Native reconstruction remains outside this session's scope.

Active checkout: D:/Documents/FableTLC, feat/novi-script-recovery, base 366c08e.
No AGENTS.md was found in the workspace search. Existing dirty Lua and native
files remain the baseline. The previous Trader Escort scratch snapshot matches
every recorded converter, evidence and generated Lua hash.

## Trader Escort sound candidate extended before promotion

Resumed work/codex_lua_trader_sound_20261004. Review found an additional silent
operand error: DarkwoodAssassinSpawn.Main's first PlaySoundOnThing call used the
literal DarkwoodAssassin, while retail 00E031F8..00E0320C passes the retained
CStack_30 string. All three native distance gates reuse that sampled string.
The scratch adapter now checks the exact first-call instruction window and
corrects this literal as well as the two unresolved later operands.

The full Main fixture now expects three SOUND2 calls, retains the sound-selection
and targeting random draws, and tests termination before selection. Adverse
cases reject changed native bytes, missing lifted operands and a changed literal.
`python -m unittest work.codex_lua_trader_sound_20261004.test_trader_sound -v`:
four tests pass. `python work/codex_lua_trader_sound_20261004/smoke_controlled.py`:
before/after both 16 files, zero problems. These use a three-entry native vector
mock; the stock mock's equal first/last pointers are not real vector evidence.

The original comparison was stopped after this new finding; the revised
fixed-seed full29 comparison is running. The target unit is complete and changes
only BanditCronies.lua and DarkwoodAssassinSpawn.lua in both styles. Other units
must finish before promotion. A prematurely copied scratch output caused a
Windows sharing error during a standalone regeneration; the completed target
regeneration was subsequently copied and all focused tests rerun successfully.
No root converter/evidence/output changes, package, installation or game launch
have been made from this candidate. Checked promotion script remains unexecuted.

Selection boundary coverage was added after the original four cases: residues
0, 2, 3 and 32767 select the correct index modulo the three-entry vector, with
one read and the expected sound reuse. The focused suite now has five passing
tests. The promotion marker records the exact tested scratch input hashes.

## Independent scratch: by-value actor resource acquisition

TurnToBalv (retail 00E0A820) passes the by-value Me actor's address into two
slot-0x20 acquisition calls at 00E0A965 and 00E0A998. The existing lowering
prints `RESOURCE_TryAcquire(xStack_30, &native_arg_Me, 4)`, which the pseudo-call
lifter leaves unresolved. That stalls the generated acquisition loop.

work/codex_lua_resource_parameter_20261005 contains a candidate lifter that
accepts the reference only for a declared function parameter whose known kind
is Thing, only in TryAcquire's actor position. Unknown local addresses, untyped
parameters and non-Thing parameters remain unresolved. Native/source hashes
and exact instruction windows are saved in evidence.json.

Three low-level tests pass actor identity, success/failure and adverse inputs.
Two full TurnToBalv fixtures pass in draft/readable: immediate or delayed
acquisition reaches the macro with HERO/BALV/TRADER correctly bound; termination
during failed acquisition skips the macro and releases both resources exactly
once. Five total tests. Scratch generation parses 65 functions / 16 Lua files,
removes precisely the two acquisition TODOs (60 -> 58), and changes only
TraderEscort.lua in each style. Both standard 16-file smoke reports have zero
problems. Full unrelated-unit comparison is still required; nothing promoted.

Resource-parameter smoke traces differ only for TurnToBalv: the candidate now
calls TryAcquire for the trader, progresses through the macro and reaches the
comment loop. The baseline spins in the unresolved acquisition loop instead.
This is an expected recovery difference, not an equal-trace claim. Full custom
fixtures check retry/termination behavior and cleanup beyond the smoke mock.

The existing lifter unittest module also passes against the candidate Lifter:
87 tests, no failures (`run_lifter_suite.py`). This exercises broader lowering
behavior without changing the shared root module used by the sound comparison.

## Trader sound recovery promoted and packaged

The revised fixed-seed comparison completed all 29 registered units. Only
BanditCronies.lua and DarkwoodAssassinSpawn.lua differ in draft/readable;
all unrelated units are unchanged. The reviewed converter diff adds only the
sound adapter hook; evidence changes only reviewedTraderSounds. Baseline hashes
still matched before promotion. Adapter, converter hook, evidence and generated
reports/output are promoted. No native-lane changes or outer commit.

The promotion helper first failed before mutation because its root import path
was missing. Corrected. A later check exposed Windows default cp1252 decoding
in the helper; promotion now explicitly uses UTF-8. The initial verification
was stopped and restarted after correcting the promoted converter encoding.

Five focused tests pass using the root adapter. verify_promoted.py regenerated
the whole unit from root source and checked exact equality for all 17 Lua files
per style against tested scratch and promoted output. Conversion: 65 functions
parsed, 16 implementation files parsed, TODO 60 -> 53. Readability audit:
371 files, syntax0, diagnostics1017 -> 1013, generic6106 -> 6108; goto/native-name
totals unchanged. Two additional generic locals hold the recovered sound values.

Named bundle:
work/codex_lua_trader_sound_20261004/trader-sound-candidate-20261005.
37 Lua files parse; 42 manifest hashes verify. Sidecar baseline commit
07774e98c5675fb66aba7c23b4f1e4453b0434c0, patch SHA256
17cc739922a886cc39df9311b115a925a1fadd87847470c750c3e17a4f098b40,
DLL SHA256 e777ce0ef6d82822f41ab7371f80f37ebec9786effc99c47c4de616ace42ec2c.
No installation, game launch or gameplay proof.

Next: work/codex_lua_resource_parameter_20261005/compare.py is running full29
with the scratch Lifter against the completed sound candidate outputs. Reuse
checks exact normalized converter source, adapter bytes and target unit evidence
against the promoted root. Keep root Lifter/converter inputs unchanged while
it runs. Expected change: TraderEscort.lua only in each style. Finish full29,
rebuild target on the sound baseline, rerun full helper fixtures, then promote
only if all differences are reviewed. Continue Lua until stopped.

## Acquisition comparison continuation

Confirmed the owned comparison process live (parent 10792, exec session 3860).
The completed target was copied into owned candidate/candidate_readable scratch
and the five tests rerun: PASS on the promoted sound baseline. Target report now
has TODO51 (sound baseline53); only TraderEscort.lua differs in each style.
The scratch permanent regression test and checked promotion helper are prepared,
but neither has been copied into root. Sound-baseline source/output hashes and
tested candidate hashes are recorded; promotion remains gated on full29.

The stock smoke on both sound baseline and acquisition candidate has one identical
DarkwoodAssassinSpawn.Main `n%0` failure because its mock supplies equal vector
first/last pointers. The only stock trace difference is TurnToBalv. Controlled
three-entry-vector smoke is 16 files / zero problems before and after. Do not
alter retail semantics to accommodate the stock mock's nonexistent vector data.

## Actor acquisition lowering promoted

The full29 comparison completed: only TraderEscort/TraderEscort.lua changes in
draft/readable; all unrelated units are unchanged. Checked baseline hashes and
candidate input hashes match. Promoted the narrow TryAcquire actor-reference
handling into lift_native_lua.py, its permanent regression test into
test_resource_parameter_actor.py, and the reviewed target reports/output.

Root unittest suite: 90/90 (`test_resource_parameter_actor` plus
`test_lift_native_lua`). Full TurnToBalv draft/readable immediate/retried
acquisition and termination cleanup fixtures: 2/2. Exact root regeneration:
17 Lua files per style match tested scratch and promoted output. Conversion
TODO53 -> 51; audit371 syntax0, diagnostics1013 -> 1010, generic6108 unchanged.

Named bundle:
work/codex_lua_resource_parameter_20261005/trader-actor-acquisition-candidate-20261005.
37 parsed Lua, 42 verified manifest hashes, same e777ce0e sidecar DLL and
17cc7399 patch as the sound bundle. Not installed or game tested; no outer commit.
The comparison process is terminal and active_run.json records completion.

Next evidence target: registered BookCollecting BS_Teacher.Main 00E54E90.
Retail pause calls at 00E559E8 and 00E55A26 push false to slot0x5ec; decompile
loses all operands on those calls. Generated cleanup LAB_00e55a1e and one
inline termination path omit unpausing. Recover receiver provenance and exact
call pairing before changing generic lowering; retain unrelated vector/field
diagnostics. Native row/head pairing and full cleanup path behavior are still
unproven; this is an investigation, not a promoted fix.

Book pause scratch established: work/codex_lua_book_pause_20261005. Baseline
converter, interface adapter, unit evidence and generated outputs were hashed.
capture.py saved the pre-lowering restored source and its 143 paired call entries;
baseline parses 14 functions / three implementation files, TODO51.

Exact pairs: 00E559E8 has printed head CVar12+0x5ec and recovered argument `0`,
but lacks receiver identity; 00E55A26 has xStack_7c+0x5ec and no arguments.
Retail 00E551C6 loads GSI from [esi+4], then 00E551CF stores it to pushed-SP+18
(balanced-SP+14). 00E5579C restores EBX from that balanced slot before the first
missing false call; the second reloads ECX directly from SP+14. Each site pushes
zero and calls slot0x5ec. Original entity this is bound to ESI at 00E54E9A.
The existing interface table maps slot0x5ec to PauseAllNonScriptedEntities at
00890AB0 (one stack word). Caller/body hashes are saved in investigation.json.
Next implement reviewed site-specific operand recovery using this provenance,
then test both full Main cleanup paths and adverse byte/source/site changes.
No BookCollecting root changes or candidate have been promoted.

## Book teacher cleanup candidate

Further pipeline evidence corrected the initial two-call hypothesis. The member
resource recovery stage already restores 00E559E8 to unpause false. Only native
00E55A26 remains missing; its cleanup block is duplicated/interned in generated
Main, accounting for several affected exits. The scratch fix targets only this
site. Source hashing uses the pipeline function after existing AddGossip operand
repair; raw export and pipeline function are retained separately.

New generic reviewed adapter validates full native/pipeline source hashes, exact
GSI origin/save/reload and call setup, interface table/target/bytes, stack purge,
and unique native/source pairing. It records a reviewed edit before helper and
resource recovery, then applies it just before lowering. Applying immediately
removed an indirect head and invalidated downstream resource call pairing; that
approach was rejected. The two-stage plan preserves pairing until its consumers
finish. No generated Lua is hand-edited.

Candidate generation: 14 functions and three implementation files parse, TODO
51 -> 50. Four full Main termination fixtures (before possession response,
after AskForBook, lost-book task, completed look dialogue) each leave the old
VM paused and the candidate unpaused, with false pause immediately before movie
destruction. All other emitted events are identical. Normal and pre-movie early
exits match. Three tests cover these paths and adverse source/body/instructions,
target/purge/site/printed-head and late-shape mutations; all pass in both styles.

Both standard three-file smoke reports are exactly equal with three baseline
unknown-method findings: SetStateFlagVectorAt (AskForBook), StateFlagVectorResize
(Init), PersistStateFlagVector (OnPersist). Four baseline functions also report
pending sidecar calls. These are not hidden or treated as runtime passes.

Fixed-seed full29 comparison is live via
work/codex_lua_book_pause_20261005/compare.py (exec session42714). Target compares
only BS_Teacher.lua in both styles. Wait for all unrelated units before promotion;
baseline source/output hashes currently match. Keep shared converter unchanged.

## Book teacher cleanup promoted

Full29 completed: only V_BookCollecting/Entities/BS_Teacher.lua differs in
draft/readable. Hash-guarded promotion copied the reviewed adapter, converter
hook and unit evidence plus generated outputs. Promoted adapter passes the
three full-function/adverse tests; root regeneration matches all four Lua
files per style. TODO51 -> 50; global readable audit371 syntax0 and unresolved
1010 -> 1009, generic locals6108. Offline bundle
work/codex_lua_book_pause_20261005/book-teacher-unpause-candidate-20261005
parses24 Lua and validates29 manifest hashes, exact staged sources and e777ce0e
DLL. Nothing installed or game tested.

The default smoke source list points to old sidecar-abi-v2. Verified packaged
LuaManager.cpp hash, include and RegisterNoviUnitBindings call; candidate header
registers all three flag-vector APIs. Controlled before/after smoke against
those three candidate sources is exactly equal with zero problems. Source
hashes and DLL hash retained in smoke_current_sidecar_source.json. Original
three-problem reports remain intact. This establishes mock execution/static
registration only, not live behavior. Next: explicit smoke source selection.

## Explicit smoke source profile and spectator greeting

Book root lifter/actor suite completed90 tests and28 subtests. Smoke now accepts
--sidecar-source-dir, requiring the three source files when explicitly chosen.
Default method inventory and complete Book reports remain unchanged; explicit
current profile reports0 and preserves all behavior traces. Incomplete profiles
are rejected. Validation/source hashes: work/codex_lua_smoke_profile_20261005.
Current-source corpus29 units /207 implementation files reports6 findings.
Two Bordello free globals are intended g_PresentedItemName exports: candidate
LuaEntityAPI.cpp:671 explicitly sets the global after item presentation. The
Guild ScorpionHome error is an intentional ownership-review block; SickChild
and Trader modulo failures come from empty mock vector counts. Original reports
retained without suppression. The remaining actionable free global was the
Chicken Kicking Spectator's unaff_EBX greeting flag.

Retail Main00E63890 initializes a local byte at balanced ESP+12 to0 at
00E638D5 (ESP+22 during a16-byte Thing/bool argument); SetIsPushableByHero
slotD30 maps to008A6DD0 with4 purge words. It reads/tests that byte at
00E63B24/28 and branches to tips at00E63B2C when nonzero.00E63C66 stores1
after greeting. Ghidra instead prints an uninitialized register's high bits
and the generated greeting branch faults when reached.

Existing reviewedStackScalarAliases now separates native_greeting_done.
Initialization uses the last generated declaration as an insertion anchor
before executable statements; no existing executable statement is removed.
Full source/body hashes and14 instruction anchors retained. Four tests run
full Main in both styles: first greeting then two tips, reset across a second
Main, no-talk/attack baseline equivalence, termination/failed-speech cleanup
exactly once, and adverse native/source/ambiguous-read rejection. All pass;
root adapter tests9/9. Six-file smoke1 -> 0, with only the freeGlobals entry
removed and all mock traces identical. TODO65 -> 64.

No shared lowering changed, so only Chicken Kicking was regenerated. Converter
and lifter still match completed Book full29 inputs. Candidate differs only
in Spectator.lua per style; existing unrelated Guild readable overlays and
Arena/Arena.lua root-versus-generated differences were explicitly retained in
preserved_readable_overlays.json. The first comparison guard rejected those
pre-existing overlays; it was corrected to preserve and report them rather
than overwrite them or claim fresh unrelated regeneration.

Hash-guarded promotion and exact seven-Lua-file regeneration per style pass.
Latest audit371 syntax0, diagnostics1008, generic6108, machine4649. Candidate
work/codex_lua_spectator_flag_20261005/chicken-spectator-greeting-candidate-20261005
parses27 Lua and validates32 manifest hashes, exact staged sources and e777ce0e
DLL. Not installed or game tested. Next native evidence target: remaining Book
AskForBook packed scalar assignment; no fix selected yet.

## Book dialogue selector and donation validation

The packed-argument TODO is in GetBookSpecificArgs00E57020, not AskForBook.
Retail keeps the map handle in EBP while reusing a stack parameter byte for
the comparison result. The generated helper keeps its handle in this_00 and
branches on cVar2; the unresolved write is bookkeeping. No lowering change
was justified. The apparent hat/key nesting in AskForBook00E55CE0 also matches
the retail export's if/else-if partition; duplicated unreachable cleanup
suffixes were inspected, not altered for lower diagnostic counts.

New scratch native_dialogue.py executes all1295 original selector bytes with
narrow CString/map/scheduler doubles and genuine x87 fld/ret getter stubs.
It verifies body hash, map receiver, CString destruction, stack balance and
saved EBX/EBP.212 native/draft cases match map outputs and query counts across
story thresholds, Twinblade flag, Teddy strings, attractiveness/scariness
boundaries including infinities, unsupported IDs and query cancellation.
106 readable cases match effects with termination stable during the invocation.
Default map entries remain intact, and all new writes target the same map.

Initial readable per-query cancellation doubles failed because they flipped
termination on a pure getter sequence whose repeated queries readable_style's
prune_dead_termination_checks intentionally removes. The runtime contract
requires scheduler advance before termination changes: the selector's getters
read fields and SetString is a direct native map write (LuaRetailResources.h
1159), with no frame/yield. Tests now keep that state stable for readable and
explicitly exclude query-count identity from its equivalence claim; draft
still receives every per-query adverse case. A missing readable module preload
was also corrected by loading the actual native_quest_helpers module.

Two full AskForBook tests cover14 style/path combinations: good/evil hat,
silver-key threshold, already-rewarded/no-threshold paths and wanted/unwanted
refusal. Actor map handles retain hero/teacher resource identity. Donation
updates totals and flags, consumes the selected book, restores camera, destroys
both maps and releases hero exactly once, then starts BookReaction with the
same book ID. Refusal does not allocate movie resources, consume a book or
spawn a reaction. These are behavior expectations checked against native
branch evidence, not full native execution of the donation helper.

Artifacts: work/codex_lua_book_rewards_20261005/{dialogue_validation,
completed_tests,investigation}.json. Root generator/Lua unchanged this turn;
no audit rerun, package replacement, install or launch. Next: populated
BookReaction00E566F0 and DoConversation00E569D0 worker/resource lifetime tests.

## Populated BookReaction composed retail regressions

BookReaction00E566F0 (721 bytes) and DoConversation00E569D0 (823 bytes) now
execute together in an owned Unicorn fixture. Full body hashes are pinned.
Narrow boundaries cover CString copy/destruction/comparison, thing lookup,
GSI conversations, frame/termination state and resource virtual calls. The
fixture verifies native stack balance and destruction of every local string
and thing. It reads native stride5C, Lines+28 and four CString vectors at
2C/38/44/50; fixtures use nonzero book/line indices and distinct child/marker
identities. The packaged sidecar GlobalConversations layout agrees with these
native accesses, and its CCharString is one pointer in the32-bit build.

192 standalone conversation cases cover boy/girl, dialogue or NULL, one-shot
or looping/empty animation, conversation/task completion, ReadingBook
interruption, frame termination and initial termination.192 composed cases
cover0/1/3 lines with alternating speakers, both acquisition results, initial
or subsequent reading and termination at different outer/inner frames. Both
draft/readable match exact effect traces:384 native runs and768 Lua cases.
Native caller ignores acquisition failure, still teleports/sets pushability,
then clears actions/commands. Normal/reading exits restore both children and
prepare both member resources; termination follows the native early exits.
Acquisition cases prove caller behavior at the wrapper boundary only; expert
internals and in-game ownership remain outside this fixture's claim.

Unsupported speaker definitions are not treated as supported: native dereferences
the null member receiver on an inactive-conversation task query. The sidecar's
Get(0) likewise rejects an invalid handle. No arbitrary fallback was introduced.

Promoted regression files: tools/script_recovery/book_reaction_native.py,
book_reaction_native_witness.json and test_book_reaction_composed.py. Unicorn
runs in a separate subprocess on Windows, following the existing lowering
test's ctypes isolation rule; pytest hosts only Lua. Repository run completed
2 tests and768 subtests in133.13 seconds. Source/witness hashes, scratch and
promoted results retained under work/codex_lua_book_reaction_20261005.
No generator/Lua change, audit rerun, package replacement, install or launch.
Two external FableForge processes were observed; they were left untouched.
Next: trace GuildTrainingWoodsMelee ScorpionHome's deliberate ownership guard
against its existing registered evidence before choosing any recovery edit.

## Smoke generated-stage selection

ScorpionHome's ownership guard is in Guild Training's deliberately disabled
reviewed artifact, as documented by readable/README.md. Its complete generated
stage is readable_converter, already selected by the playtest packager. Smoke
now reuses that selector when --stage is omitted; explicit stages remain usable.
No generated Lua or curated review artifact was changed.

Default and explicit readable_converter Guild reports are identical:37 files,
zero findings. Explicit readable preserves the12-file report with its deliberate
guard. The29-unit current-source corpus now covers232 files with4 findings:
two verified Bordello host globals and two empty-mock-vector modulo cases.
Book source-profile validation still passes: historical default report unchanged,
complete candidate source profile zero findings, incomplete profile rejected.
Evidence: work/codex_lua_smoke_generated_stage_20261005/corpus_summary.json and
work/codex_lua_smoke_profile_20261005/guild_default_generated.json. These are
static/mock checks, not live-game proof. Next: controlled populated-vector checks.

## Populated definition-vector checks

Both remaining modulo findings are reproduced by the historical mock returning
the same value for vector begin and end. Eighteen controlled full Main cases pass
for DarkwoodAssassinSpawn and SickChildsSister: counts1/3/7, random values0/17/32767.
The definition lookup asserts its offset and exact random modulo count index.
Both complete unit smoke reports have zero findings under those fixtures.
The Sister evidence source anchor explicitly uses begin0x74c/end0x750,
four-byte stride and native random modulo count; generated offsets1868/1872 agree.
No empty-vector fallback was added to gameplay code or the shared smoke harness.
Evidence: work/codex_lua_populated_vectors_20261005/validation.json. This proves
controlled mock execution and index arithmetic, not retail definition contents
or full native equivalence. An initial trace assertion used the CLI's twelve-call
summary; the corrected check reads the target runner's forty-call capture.

## Assassin cleanup operand investigation

DarkwoodAssassinSpawn Main E02E50..E03395 leaves generated p0 unassigned,
then passes it to RemoveThing. Retail E02E58 sets EBP=this+8; E02E69 saves
that handle and the only subsequent EBP writes reload it at E02F87/E02F8D.
E03367..E0336C pushes true,false,EBP to GSI slot1B0. Thus cleanup targets the
spawn marker, not the created assassin. Full original body disassembled and
anchor bytes verified in work/codex_lua_assassin_cleanup_20261005/cleanup_witness.json.
Lowering, adverse tests and regeneration remain pending; no gameplay edit yet.
The inspection helper initially shadowed Python's inspect module; renamed to
trace_cleanup.py and completed successfully.

Scratch candidate restores the misidentified CCharString_bv local as a
CScriptThing marker alias through existing reviewedStackScalarAliases. It pins
both hashes and nine instruction anchors; three exact source edits restore the
declaration, self assignment and removal operand. Shared generator unchanged.
Draft/readable generation started in exec session87094 and remains confirmed
live. Poll this handle before any restart; inspect generated removal and run
full-function/adverse tests and unrelated-file comparisons before promotion.

Session87094 completed:65 functions/16 draft files and17 readable Lua files
parse. Eight full Main cleanup cases pass, with normal paths changing nil to
marker and frame1 exit removing nothing. Other captured sound/random effects
match. However output inspection exposed remaining p0 reads orphaned by the
partial alias rename; the permissive distance fixture had missed this. Candidate
now renames every use of the marker lifetime, and the fixture rejects nil distance
operands. Revised generation live in session46837; first candidate is rejected
for promotion. Root evidence and generated Lua remain untouched.

Revised session46837 completed:65 functions pass, draft16 files parse, readable17
files parse, TODO51. Both17-file Lua sets differ only in the assassin file.
The stronger behavioral gate fails because saved-handle reassignment invalidates
the lifter's alias; removal still receives nil. Candidate rejected, no promotion.
Existing native_me_call_operands proves direct LEA reaching definitions but
declines EBP definitions restored from the stack. Next recover that native
call operand without renaming the whole decompiler lifetime. comparison.json
and both diffs are retained; prior behavioral_validation.json proves only the
first, rejected partial candidate and must not be used as revised acceptance.

New owned native_reviewed_self_operand scratch adapter pins full native/source
hashes, nine anchors and all EBP mov/lea definitions, pairs E0336C and restores
only its entity-self argument. Source pairing lacks ecxValue and pushedStack
identities, so initial guard rejected it; explicit exported receiver and actual
E03362/E03365 ECX/vtable instructions now establish that receiver directly.
Generation live in session83873 (previous attempts46116/78640 terminal failed).
Existing native_me_call_operands regression completed22 tests in74.80 seconds.
Next inspect operand_candidate outputs, adapt behavioral/comparison scripts to
those paths, test changed/ambiguous native witnesses, then decide integration.
No shared generator, unit evidence or generated root Lua edited.

Call-operand generation83873 completed. Eight full Main cases pass for
draft/readable, including frame1 exit without removal and normal removal of
the marker with false/true flags. Distinct spawned creature rejects accidental
assassin removal; distance operands must be nonnil. Captured sound/random
effects unchanged. All17 Lua files per style compared: only assassin changes,
two RemoveThing operands p0->me and readable unused declaration pruning.
Evidence tests2 pass with7 adverse subcases (body/source hashes, anchor,
missing/ambiguous pairing, receiver, argument); no-row behavior unchanged.
Call, slot, register definitions, receiver instructions and printed arguments
now stored in witness metadata rather than adapter-specific constants. Retail
hash/source proof rechecked and evidence tests rerun successfully. Evidence:
operand_behavioral_validation.json, operand_comparison.json and test_operand_evidence.py
under work/codex_lua_assassin_cleanup_20261005. Next integrate reviewed metadata
and adapter, complete full29 comparisons and candidate package before promotion.
All output remains scratch; no game launch or install.

Integrated tools/script_recovery/native_reviewed_self_operand.py and entity-only
converter hook, plus reviewedSelfOperands row in Q_TraderEscort unit. Owned
pre-integration converter/unit snapshots and hashes retained. Root regression
test_native_reviewed_self_operand.py passes8 tests (one exact replacement/no-row
check and seven evidence rejection cases). Generated root Lua unchanged.
Full29 before/after scratch comparison launched in exec session21744 using
compare_corpus.py; baseline disables only new adapter. Expected only assassin
Lua difference; any additional change fails. Per-unit comparison reports are
written as each completes. Poll same handle before restart; package, full corpus
completion, exact promotion and audit still pending.

Extended full Main behavioral coverage to42 cases:original8 plus32 interruption
cases (termination query1..16 per style) and2 already-close removals. Captured
effects differ only by nil->marker removal operand; flags preserved. This is
same-style mock coverage, not cross-style termination-query or native runtime proof.
Integrated Trader Escort corpus comparison completed:17 Lua files/style, only
assassin changed. Integrated files exactly match tested operand scratch output.
prepare_package.py prepared the owned package script with corpus/promotion gates;
it has not assembled or installed anything. Corpus session21744 remains live
for remaining units; continue polling that handle.

Additional ABI check verifies retail slot1B0 targets8910D0 and purges three
stack words. Candidate GameInterface typedef and slot108 initialization agree;
LuaQuestState accepts optional flags and forwards explicit values. Its null
target guard silently skips native removal, confirming the functional impact
of generated nil. Candidate source file hashes retained in remove_abi_validation.json.
This is retail/source ABI evidence, not an installed DLL or gameplay result.
Corpus21744 remains confirmed live; no shared input edits made during its run.

Arena23 and Bandit Camp20 Lua files/style completed with no differences.
prepare_promotion.py verifies both root Trader Lua sets equal regenerated before
baseline, then preserves all those Lua files, reports and converter/evidence
inputs with hashes under promotion_baseline. promote_checked.py is prepared but
not executed; it requires every registered unit's completed comparison and all
captured root hashes unchanged, then copies only assassin Lua and generation
reports. No generated output changed. Corpus21744 remains active.

Beggar/Child4 Lua files/style completed unchanged. Added original retail sequence
execution in native_cleanup_sequence.py, kept separate from Lua to avoid Windows
Unicorn/Lupa ctypes mixing. Nine cases run the actual prologue save, optional
E02F87/E02F8D reload and E03362..E03372 call with three owner addresses. All
recover owner+8 marker, forward GSI receiver/false/true and balance stack. Callee
is a recording RET12 stub; intervening native control flow and full Main are not
executed. Results in native_sequence_validation.json. No corpus inputs edited;
corpus21744 remains live.

Promoted native instruction-window regression to tools/script_recovery/
assassin_cleanup_native.py and test_assassin_cleanup_native.py. Test runs Unicorn
in a subprocess, writes nine cases to an owned temporary directory under work,
then verifies both reload sites/three marker addresses/flags. Repository test1
passes in1.20 seconds. Initial copied root derivation was one parent too high;
corrected to helper-directory parents[1] and reran successfully. Scope remains
disjoint original windows with recording callee, not full native Main.
Chicken7, Dragon6, Expressions14 and Bordello10 Lua files per style now compare
unchanged. Corpus21744 confirmed live. No generated Lua promoted or installed.

Reviewed remaining assassin TODO xStack_20=CVar3:retail E02F2E fild signed DWORD,
E02F32 fstp float and E02F36 reload implement the distance tunable conversion.
Twelve original x87 cases match float32 conversion, including signed extremes,
negative values and2^24 rounding boundaries. Sidecar ReadGlobalGameData returns
signed int; IsDistanceBetweenThingsUnder takes float. No additional gameplay
fix justified under default rounding. Diagnostic retained; this check does not
prove arbitrary FPU modes or actual distance behavior. Evidence:
distance_conversion_validation.json. Gameflow Assistance2 and Guardian Sister
Info5 files per style compare unchanged; corpus21744 remains active.

Completed-unit audit now independently rereads before/after Lua, verifies recorded
differences, parses every after file and checks readable syntax reports plus all
promotion baseline hashes. It passes19/29 completed units, capturing after hashes
in completed_gate_audit.json. Promotion additionally requires complete audit and
matching after hashes. Initial audit used stdout summary syntaxOk key; corrected
to persistent report syntax.ok schema and reran. Orchard12 and Picnic4 files
per style unchanged. Corpus21744 still live; rerun audit after full completion.

Package input preflight checks candidate DLL and patch hashes, all six recorded
build-source hashes, and all four source hashes from RemoveThing ABI review.
Existing Oakvale/base package inputs exist. package_inputs_validation.json records
success; this adds provenance checks, not new build or live-game proof. No shared
inputs edited. Corpus21744 confirmed live, currently after Sick Child generation.
RockTrollTrigger's remaining TODO appears to be the same integer-to-float tunable
shape; no new recovery edit selected from that diagnostic alone.

Controlled integrated package smoke now runs both corpus before/after Trader
readable trees with explicit conversion reports/current sidecar source. The
three-entry sound vector returns a string and checks its exact sampled index;
RemoveThing mock rejects nil to expose the real sidecar's skipped cleanup.
Before16 files/1 finding (assassin nil removal); after16 files/0 findings,
JSON results asserted. Default harness unchanged. Reports controlled_package_
before.json/after.json retained under owned cleanup scratch. Full corpus21744
still live, promotion/package assembly remains gated.

Corpus21744 re-polled and confirmed live. Statue6, Summoning9 and Tour Guide5
files/style completed unchanged. Completed gate audit24/29 passes, syntax and
promotion baseline hashes unchanged. Remaining Guild Training, Trader Conflict,
Trophy Dealer, Wasp Boss and White Balverine. Guild second pass log live since
8:21; preserve the existing process. Next after all comparisons finish:
audit_completed.py -> promote_checked.py -> package.py -> audit_readability
with owned report and ledger refresh. No extra full audit before promotion.

Continuation classified as verified wait with new completed evidence:corpus21744
confirmed live, Trader Conflict16 and Trophy Dealer4 files/style completed
unchanged. Completed gate audit26/29 passes; source/output baseline hashes still
match. Remaining Guild Training, Wasp Boss and White Balverine. Preserve same
process and all scratch; promotion remains gated on terminal full success.

Completed full29 corpus21744: only assassin Lua changes, all other units
unchanged in both styles. Complete audit passes syntax and input/output hashes.
Promoted only assassin draft/readable Lua and generation reports; exact17-file
sets/style match tested scratch. Bundle37 Lua/42 manifest hashes verified,
e777ce0e DLL/source provenance unchanged. No install or game launch.
Readability audit13160 completed:371 files, syntax0, diagnostics1008; other
metrics unchanged. Ledger refreshed and final_validation.json saved. Direct
audit script invocation lacked module path; reran with python -m successfully.
All owned comparison/package/audit workers completed. Next continue registered
evidence-backed lowering and resource recovery. Continuous goal remains active.

Next defect identified:MakeTraderComment E01900..E02389 loses the local retained
speaker copy in both generated master and helper bodies. Six baseline calls
(types0/1/2 in each scope) return false before conversation. Helper test supplies
alive actor and alive fallback; master prologue likewise always tests unresolved
nil and returns false. Retail E01912/E01915 load speaker words4/8, E01924/E01928
store them into the local CScriptThing, E0192E increments its control block,
E01938 calls slot12C IsAlive. Dead speaker path selects nearest DarkwoodTrader
and assigns retained words before second IsAlive. Slot12C target4AB130 confirmed
from authoritative thing vtable TSV. Full native/source hashes and instructions
retained under work/codex_lua_trader_comment_20261005. No lowering edit selected
yet; inspect native_split_thing_words for the missing generic copy shape.

Copy-shape investigation:existing split-word pass only handles Data at base+4,
not Info sibling local_28 at local_30+8. Direct application of four copy passes
to normalized native export leaves it unchanged; copy_shape_gap.json records
this limited source-shape check. Prologue stores Data directly, split Info and
vtable then retains; fallback loads returned Info/Data, releases old Info and
stores/retains new words. Existing folds require different staging order or an
explicit CScriptThing object cast absent here. Do not broaden on naming alone.
Next capture actual annotated/restored source, prove both retained-copy windows
and predicates, then implement a complete reviewed copy recovery. No shared
generator/evidence/Lua edit for this helper yet. Initial scratch import assumed
unwrap_statements belonged to lowering; corrected to converter and check passes.

Captured actual first registered helper lowering boundary via scratch-only stop:
lowering_input.c/output.c and hashes in lowering_capture.json. Output incorrectly
merges control-block xStack_28 into xStack_30 and then initializes that whole
handle to QUESTTHING_Empty after raw speaker copies. This confirms the failure
inside lowering, not just an export-pattern mismatch.
Exploratory scratch candidate at lower boundary restores complete speaker copy,
fallback assignment and both IsAlive calls, with exact intermediate hash and
occurrence checks. Generation live in exec session92990. No shared input edited.
Native copy-window validation, ownership adverse tests, full helper behavioral
checks and unrelated comparisons remain required before any promotion. Candidate
does not yet constitute reviewed native recovery evidence.

Session92990 completed:65 functions/draft16 files parse; readable17 parse,
TODO45. Exploratory candidate passes16 full helper basic cases in both scopes
and styles:alive supplied speaker, alive fallback, dead fallback and cooldown.
Checks assert conversation actor, exact ordinary-comment text and timer.
No native equivalence claim yet; reply/introduction branches and retained copy
ownership remain pending. Candidate emits three declaration TODOs absent from
baseline; investigate declaration-block continuity introduced by removing the
Info sibling declaration before further generation. Evidence basic_validation.json.
No shared generator/evidence/root Lua promotion for this helper.

Revised scratch removes the complete split-Info declaration line and trailing
blank lines, preserving declaration block. New destinations candidate_v2 and
candidate_v2_readable preserve first candidate evidence. Generation live in
session39035; poll before restarting.
Four probes on first candidate (both styles, comment types1/2) expose additional
whole-helper recovery gaps:raw handle Data slices become nil IsEqualTo operands
even when both traders are the same object; introduction also faults on
CONCAT31/string-field bookkeeping. remaining_branch_findings.json records traces
and errors. Do not promote the ordinary-comment-only repair. Next prove native
identity comparisons and packed flag sites, restore all reached handle copies,
then rerun full reply/introduction/ownership cases.

Session39035 completed:65 functions/draft16 files parse, readable17 parse;
TODO42 after removing three accidental declaration diagnostics. Sixteen basic
cases rerun against revised outputs pass (basic_validation_v2.json). No owned
worker live. Reply/introduction and native ownership still unresolved, no promotion.

Captured operand windows for introduction identity (E01AA3), rock-troll compare
byte assignment(E01B98), reply byte stores(E0220C/E02213) and read(E0226E).
Base wrapper slot138 target4AA8A0 is merely false/RET4; it is not the actual
polymorphic implementation used by the Data receiver. Sidecar IsEqualTo unwraps
both actors' pImp.Data and invokes the implementation slot, with CGameScriptThing
8D46A0 identified in binding comments. Do not infer identity semantics from the
base vtable target alone. operand_windows.json preserves bytes/disassembly.
Next verify concrete implementation and packed-byte lifetime before correcting
the nil comparison operands or eliding packed bookkeeping.

Concrete identity8D46A0 verified with original A01B50 intelligent-pointer
accessor:ten native cases cover same/different targets, either/both expired,
refcounts1/3. Original code releases expired control blocks, clears member,
and returns equality of targets; allocator free alone is a recording stub.
Stack balance and release effects asserted. native_identity_validation.json
records results; implementation/accessor original bytes retained separately.
Scratch candidate_v3 now restores both raw Data comparisons to IsEqualTo calls
on whole handles, matching candidate sidecar's actual unwrapping semantics.
Generation launched; packed-byte lifetime and other copies still unresolved.
No shared converter/evidence/root Lua change for this helper.

Scratch candidate_v3 generation32746 completed:65 functions/draft16 files,
readable17 parse; TODO42. Candidate_v4 separates reply flag stores/read
E0220C/E02213/E0226E from the earlier CString lifetime and preserves the
rock-troll comment snapshot while isolating packed compare-byte bookkeeping.
Generation51007 completed with same syntax counts/TODO42. Eighty full reply
mock cases pass across draft/readable and master/helper scopes: same/dead
other actor, missing/present text, termination checks1..4 and no termination.
Evidence flag_validation_v4.json; no full-native-helper equivalence claim.

Original native copy4ABE90, assignment8AB980, destructor4AA840 and empty
constructor6E7B40 execute in native_handle_copies.py:8 composed swap/clear
cases pass with distinct/shared control blocks, refcounts1/3, final payload
release/free and stack balance. Base lifecycle/payload-destructor/allocator
boundaries are stubs; call order comes from E01ADF..E01B62 instruction window.
Native handle evidence retained in handle_operand_windows.json and
native_handle_copy_validation.json. Candidate_v5 generation74983 completed:
65 functions/draft16/readable17 parse, TODO39. Full introduction mock test
FAILED swap: one assignment remains TODO, producing BOLD twice instead of
OATH/BOLD. Do not promote v5 or call its whole helper validated.

Candidate_v6 names the two retained lookup handles and gives them whole-handle
declarations before lowering to preserve the value swap. Generation session36995
is live at this checkpoint; poll it, do not restart. Next run
validate_introduction.py v6 and validate_flags.py v6, then inspect any remaining
TODO/call operands and build full-helper native comparison before integration.
Only owned scratch files changed for this helper; no shared converter/evidence,
root generated Lua, package installation or game launch.

Session36995 candidate_v6 completed:65 functions/draft16/readable17 parse,
TODO40. Both full introduction and reply validations FAIL before comparison:
named native_nearest_handle/native_furthest_handle remain nil while lookup
results are emitted into synthetic r1/r2/r3. Renaming broke hidden-result-slot
mapping; do not promote or repeat equivalent rename experiments.
lift_native_lua.py interface_call lines1940..2054 associates recognized hidden
slots with synthetic results; slot_name/expr recognition and reassignment
invalidation are the next generic lowering boundary to investigate. Prefer
explicit named lookup result assignments backed by native output slots, or a
generic mutable-handle result recovery, instead of changing generated Lua.
v4's80 reply cases and original native8 ownership cases still pass; v5/v6
whole introduction remains unvalidated. No owned generation worker live.

Candidate_v7 generation93124 completed:65 functions/draft16/readable17 parse,
TODO40. Named lookup outputs now bind to actual handles via scratch Lifter
slot_name/interface_call wrappers; reply80 pass, introduction fails because
thing_call classifies a staged known actor as a string and drops its operand.
probe_staged_thing_operand.py independently reproduces this classification
bug (staged_thing_operand_probe.json). Candidate_v8 generation95248 completed,
TODO39; removes named actor storage from legacy staging pool. Reply80 and
introduction24 mock cases pass, but inspection finds intro AddNewConversation
actor nil --[[missing]]. The original test allowed empty actors, so v8's passing
introduction report is insufficient for promotion and is superseded by v9.

Retail E01C24..E01C38 reads original speaker argument [ESP+58] and pushes it
with false,false into GSI slot5B0. Four original call-site executions pass with
deliberately distinct retained/lookup actors, RET12 recording boundary and
stack balance (native_conversation_operand_validation.json). Candidate_v9
snapshots original speaker before CString slot reuse and uses it only for
that proven intro conversation call. Generation9835 completed:65 functions,
draft16/readable17 parse, TODO38. No TODO/missing/CONCAT31 remains in helper.
Tightened introduction24 tests assert exact conversation speaker identity;
reply80 and basic16 also pass. Expanded192 intro cases cover query stops1..7,
same actor clear, retained swap, scared ordering, normal/rock-troll text and
sound/pause, with expected exit and exact side-effect prefixes. Evidence:
introduction_validation_v9.json, flag_validation_v9.json,
basic_validation_v9.json, introduction_interruption_validation_v9.json.

Native handle8 cases strengthened to execute original helper instruction
windows E01ADF..E01AFB / E01B2F..E01B62, not manually composed call order,
with genuine copy/assignment/destructor/empty constructor bodies. Still pass
shared/distinct refcounts1/3, final releases and stack balance. Base lifecycle,
payload destructor and allocator are explicit recording boundaries.

Next: full native-helper comparison with narrow engine/string boundaries,
then integrate the reviewed source recovery and generic mutable hidden-result
handling/staged actor classification instead of copying scratch monkeypatches.
Capture full29 before/after for shared generator edits; promote only after
adverse/unit regressions and evidence validation. Current helper changes stay
scratch only. No live owned generator; no game launch/install/root Lua promotion.

Whole native MakeTraderComment oracle implemented in native_full_helper.py.
Executes original full E01900..E02389 body with original copy/assignment,
empty constructor, destructor, GetDataString and IsAlive wrapper code. Engine
methods, concrete actor queries, CString operations and base lifecycle are
explicit ABI/value doubles; no whole-engine or in-game equivalence claim.
Initial harness failures corrected from native instructions: GetDataString
body ended mid RET4 operand (correct length2A), and E01B86 passes char* ECX,
not implementation address, into Compare. boundary_instruction_windows.json
retains inspected ABI evidence. Retail whole-body SHA pinned on every run.

768 native full-helper cases now complete with stack/refcounts balanced:
types0/1/2; scared/swap/same/dead/fallback/dead-fallback; normal/rock-troll
comments; termination stops0..7; absent response text; timer-1/0/2. Native
result and exact conversation/person/line/text/actor/target/flag/sound/pause/
timer traces compare against both Lua styles and helper/master scopes:
3072 comparisons pass for candidate_v9 (full_helper_comparison_v9.json).
No Lupa and Unicorn mixed in one process; native and Lua tools run separately.

Candidate_v10 removes bespoke slot_name/interface_call hooks: reviewed source
names local_nearest_handle/local_furthest_handle and explicitly binds all three
hidden-result lookup calls to their output handle. Generic candidate thing_call
classifies known staged things/vectors/numbers/bools as explicit operands,
preserving ambiguous string/unknown behavior. Focused regression reproduces
old dropped actor and validates identity true/false, with negative string and
unknown classifications unchanged (staged_classifier_validation.json).
Generation69968 syntax passes/TODO38, but full comparison FAILS because a
known handle assignment is treated as a compiler alias and discarded.

Candidate_v11 adds generic emitted typed-handle stores before slot alias
folding: existing thing local assigned a known thing preserves a real store,
reads old source before forgetting target, keeps slot-result identity.
Generation21745 completed:65 functions/draft16/readable17 parse, TODO38.
All3072 full native/Lua comparisons pass (full_helper_comparison_v11.json).
Source-recovery plus these two generic edits still scratch only. Next build
reviewed fail-closed evidence adapter/regressions and integrate the shared
changes only after the baseline worker finishes, then compare full29.

Fresh root full29 baseline started via baseline_corpus.py, live session80894.
Poll this handle before restarting. Worker snapshots generator/evidence hashes,
uses two child processes, explicit scratch drafts for readable builds, and
asserts inputs unchanged on completion. Do not edit shared generator/evidence
while it runs. Candidate_v11 worker terminal; no installation/game launch.

Full29 baseline80894 terminal0; after85157 terminal0. Input hashes unchanged
within each run; supplemental unit-review metadata captured before integration.
Integrated reviewed source adapter, two generic fixes (typed staged operand
classification and initialized actor stores), unit witness, pinned parameter
input fixture and focused tests. Root25 tests pass. Initial unknown-write test
mistook TODO comment for executable assignment; corrected after saved baseline
comparison proved unknown write unchanged (unknown_store_baseline_comparison).
Integrated generation14481 terminal0,65 functions/draft16/readable17 parse,
TODO38; integrated3072 native/Lua comparisons pass. No scratch lifter hooks.

Broader existing tests:112 pass /28 subtests (lifter, thing release/flags,
receiver reaching),19 pass /7 subtests (self-copy/stack-collision). Mixed
string-return pytest runs76012/41487/7603 emitted Windows access violations
despite eventually reporting pass; those reports are NOT accepted evidence.
No failed process killed: identified PID16604 exited before termination check.
Fully isolated runner executes all18 existing string-return assertions unchanged
against completed full-corpus Bordello output, with native fixture AST in six
separate child processes; clean pass (isolated_string_regression.json).
Importing test module loads Lupa; standalone native child avoids that import.

Full29 changes:Trader master/helper in both styles; Bordello Madame/Magicman
draft, Madame readable; Sick Child WomanToAttract draft. All unrelated changes
are p0/pCVar9 -> me in proven typed actor stores; exact surrounding calls/CFG
unchanged, two Lua identity cases pass (unrelated_change_review.json). Other
26 units unchanged. 534 generated Lua files parse; before/after/report/input
hashes and live-root affected outputs checked. Promoted8 Lua +8 reports only;
all hashes verified (promotion_plan.json/promoted.json). Readability refreshed:
371 files,0 syntax failures,974 diagnostics,6091 generic temporaries,4649
machine temporaries,4603 gotos,6344 native names.

New offline trader-comment-candidate-20261005 bundle:37 Lua/42 manifest hashes,
e777ce0e sidecar, original baseline07774e98 and patch17cc7399; build source and
unchanged RemoveThing ABI hashes verified. Bindings GetNearest/GetFurthest own
heap output buffers; host me remains non-owning, cross-VM arguments are copied.
Host teardown/yield and sidecar execution remain runtime boundaries; this pass
does not establish game equivalence or precise Lua-GC/native-release timing.
Packaging, audit and current tests/generation terminal; no install/game launch.
Next existing recovery:DarkwoodTrader Main retained actor swap nearE07CAC.

## DarkwoodTrader cutscene actor selection: scratch recovery

Existing registered Main E07640..E0A4FF native SHA256 d097208424b5f57f1b669fdc81681e532f346ccf6f23be8149782eb7aa4ad050; original source ff196c236a4eac9b051782ba31687fd07bcca3df242a2eaa9c9b321d874f9455. Work: work/codex_lua_trader_actor_swap_20261005. Native operands establish nearest ESP40, furthest ESP58 and retained temporary ESPC4. IsEqualTo caller loads their Data at +4; same-actor path constructs empty via6E7B40 and assigns furthest; conditional swap uses4ABE90/8AB980/4AA840. F35B30 and CB7940 hashes match prior reviewed active-thread termination wrapper; decompiler Initialise label is misleading.

Eight original retained swap/clear windows composed with original lifecycle bodies pass, shared/distinct Info references1/3, payload release/free and stack balance. Original selection decision E07C27 through E07D15/cancellation endpoints passes80 cases: same/distinct, scared/bold, furthest alive/dead, stop queries0..4, references1/3. Termination, concrete actor, CString, base lifecycle and allocator are explicit boundary doubles. Exact generated decision fragments compare160 cases in both styles, including actor identities, short-circuit effects, cancellation endpoint and same-actor resource cleanup order. This is bounded-region proof, not complete Main or compiled sidecar/game proof. Native/Lupa run in separate processes.

Scratch-only reviewed transform restores whole-handle equality, empty clear, retained swap and explicit lookup outputs. Generation session50262 terminal0:65 functions,16 draft files and17 readable files parse, TODO35 (baseline38). No root generator/evidence or generated outputs changed for this target. First pattern failed closed on native constructor's __at6e7b40 suffix; corrected before successful candidate. Comparison harness first omitted outer cancellation edge, then replaced a SCARED literal while substituting configuration; both harness errors corrected, earlier failed outputs are not passing evidence.

Next: full Main actor/resource scenarios and adverse lookup/termination cases; tighten recovery to reviewed stack lifetimes, add fail-closed root evidence adapter/tests, compare full29 against reusable previous after corpus only if its input hashes still match, then promote/package/audit if all gates pass. Current target remains scratch/unpromoted. No installation or game launch.

## DarkwoodTrader guarded adapter integrated; full29 comparison running

Full Main testing found an additional existing cutscene refusal bug: cStack_169 is initialized from termination false (E07AB4 stores AL=0 after E07AAE rejects nonzero), set to1 only by accepted invitation at E082DE (ESP1F after one push means original ESP1B), and read/tested at E0837E/E08382. Generated Lua false then compared with0 wrongly entered join continuation on refusal. Reviewed scratch transform now initializes this byte to numeric0. Twelve original composed initialization/answer/final-branch cases pass, including cancellation and default refusal. CString/termination boundaries doubled; intervening cleanup excluded from native flag proof.

Current scratch passes170 full Main yes/no refusal/cancellation runs and340 additional yes/default-refusal/cancellation runs (510 total, both styles). Strict actor-map bindings and exactly-once resource/map/movie cleanup checks pass for scared, swap, same, dead and empty furthest handles. Joins terminate via a later mock active-thread cancellation after ClearThingHasInformation; this does not prove subsequent following behavior. Resource waits:80 additional full Main runs,68 actually reach a failed acquisition/retry or cancellation,12 selected resource IDs are unused by their actor branch. Assertions check all resource/movie/map releases and exact style effects. Generated bounded actor decision comparisons160 still pass. Harness empty-actor setup initially used Lua and/or nil fallback and accidentally supplied an actor; corrected to explicit nil before acceptance. Wait assertion initially treated unused resources as cancellation, corrected to require a reached wait. Earlier failed harness outputs are not accepted evidence.

Verified all prior work/codex_lua_trader_comment_20261005/corpus_after_inputs.json hashes before integration; reusing that completed29-unit after corpus as the new baseline. Pinned source fixture, native body, six instruction anchors and eight native dependency hashes integrated in tools/script_recovery/native_reviewed_trader_actor_selection.py and Q_TraderEscort reviewedTraderActorSelection row. Converter hook applies before lowering; unreviewed rows are unchanged and require no native reads. Focused actor selection/storage27 pass; combined comment/actor/self/stack regressions62 pass plus7 subtests. No generic lowering change this pass. Root generated outputs remain unpromoted.

Full29 after_corpus.py session26233 remains live, input hashes frozen. Trader Escort completes with only Entities/DarkwoodTrader.lua changed in each style. Its registered integrated output equals the tested scratch byte-for-byte,510 full Main integrated traces match. Unaffected Main prefix matches exactly except hoisted declarations; post-cutscene suffix matches after generated temporary r11->r9 normalization (unaffected_main_regions.json). Several unrelated units already complete unchanged; do not claim the whole corpus gate until session26233 is terminal and corpus_comparison.json exists. No other current-target worker live (29347/89622 terminal).

Next: poll26233 without restarting; audit every completed corpus result/input hash and parse, run prepared audit_promotion_gates.py, then promote only two DarkwoodTrader Lua files and three reports if gates pass. Prepared promotion/package scripts point to unique trader-actor-selection-candidate-20261005. Refresh readability once after promotion; verify package sidecar/DLL/source provenance and manifest hashes. No commit, installation or game launch.

## DarkwoodTrader actor/refusal recovery promoted offline

Session26233 terminal0. Full29 comparison completes: only DarkwoodTrader.lua changes in each style; other28 units unchanged. All534 generated Lua parse; all corpus output, baseline and generator/evidence input hashes checked by audit_promotion_gates.py. Promoted two Lua outputs plus CONVERSION_REPORT.json/md and READABLE_REPORT.json only; promoted hashes verified. Actor selection/native flag/full Main scopes and boundary limits remain as documented above, not expanded by parse/package success.

New offline bundle work/codex_lua_trader_actor_swap_20261005/trader-actor-selection-candidate-20261005 contains37 Lua and42 verified manifest hashes, e777ce0e DLL, baseline source07774e98 and patch17cc7399. All six build sources, four RemoveThing ABI sources, four binding-source hashes and unchanged DLL/patch provenance revalidated (package_inputs_validation.json). No sidecar change/build/install/game test.

Readability audit refreshed:371 files,0 syntax failures,973 unresolved diagnostics,6088 generic temporaries,4649 machine temporaries,4603 gotos,6344 native names. Status62 needs-recovery /154 needs-readability /155 needs-human-review. CURRENT_LUA_READABILITY_REVIEW.json/md now match this completed audit. Current pass workers terminal. Retained prior failed scratch/harness artifacts are not acceptance evidence.

Next existing registered recovery: AssassinSpawn E02E50 and RockTrollTrigger E0AD40 distance spills, work/codex_lua_trader_distance_spills_20261005/investigation.json. Current TODOs are xStack_20=CVar3 and xStack_34=CVar2; typed exporter calls values CCharString_bv, but retail E02F2E/E02F32 and E0AD9D/ADA6 execute FILD/FSTP integer-to-single conversion in the same scalar slots. Investigate exact float storage/argument behavior and fail-closed recovery rather than deleting unused-looking diagnostics. New target currently read-only scratch evidence; no new generator/evidence changes.

Distance spill evidence: work/codex_lua_trader_distance_spills_20261005/native_float_spills.py passes22 original x87 window executions. Signed int32 inputs include min/max, -1/0/1, retail7/30 and single rounding boundaries around2^24. Original FILD/FSTP results and retained EDI bits match expected IEEE single storage; stack balances. No external engine stubs needed in these arithmetic windows (RockTroll GSI pointer loads use mapped recording data only). This establishes storage type/rounding, not generated binding/game behavior. Capture of actual two registered lowering inputs is running as owned session60768, scratch-only deliberate stop once both reached; do not restart on timeout. Current root generator/evidence/output stays at the completed DarkwoodTrader promotion. Next: inspect captured spill aliases and float marshalling, reproduce the unresolved assignments in a focused lowerer fixture before choosing generic versus reviewed recovery.

Capture60768 terminal0; both exact lowering inputs/outputs and diagnostics saved. Root native_evidence_lowering.py float-store rewrite (around1501) renames float assignments to f_CVarN but only rewrites bare aliases whose target later has an explicit float read. Assassin lowers f_CVar3=xStack_20 then xStack_20=CVar3 (unset old spelling); RockTroll lowers f_CVar2=ENGINE_GlobalGameData then CVar6=f_CVar2, but xStack_34=CVar2 remains old spelling. These saved native scalar spills do not later appear as explicit cast float reads in decompiler source, so existing target_is_float filter excludes them. Next: focused before/after fixtures for immediate straight-line float-copy stores, with adverse source reassignment, pointer/string reuse and control-flow boundaries; recover only proven scalar lifetime instead of globally renaming all CVar uses. New target no shared edit yet, no worker live.

## Float-spill snapshot recovery integrated; corrected-input corpus pending

Shared normalise_typed_decompile now recovers immediate plain copies following the first explicit (CCharString_bv)(float) store. Leading straight-line copies only; stops at any other statement, source self-assignment, calls, labels, branches, address uses or returns. Existing target_is_float handling remains for earlier supported float-read aliases. Root test_float_spill_copies.py passes30 cases (20 emitted Lua snapshots after numeric/string source reuse,8 boundary guards,2 unproven source cases). Scratch six shape checks also pass; original native22 signed/single spill cases remain separate.

Scratch generation1721 terminal0:65 functions,16 draft/17 readable files parse,TODO33 (was35). Whole AssassinSpawn before/after88 comparisons pass; RockTrollTrigger660 two-invocation scenarios pass, covering11 signed/single values, delayed proximity, cancellation, already-shown and persistent spawned guards in both styles. Bound distance mocks narrow to float; sidecar LuaQuestState::IsDistanceBetweenThingsUnder signature uses float and passes it unchanged to native API (source/hash in integrated_validation.json). These tests do not establish compiled sidecar/game proof. Registered integrated outputs in both styles equal all four tested scratch files.

Verified completed prior actor-swap corpus input hashes before this edit and reused its29-unit outputs as baseline. Integrated only generic normalizer and focused test file; no generated output promotion. Broad175 tests /28 subtests pass (lifter, flags/releases, receiver reaching, signed casts, float32, scalar tail and new spill tests). First full29 session6439 terminal0: only AssassinSpawn/RockTrollTrigger change in each style, other28 units unchanged. Four draft/readable diffs retained.

Encoding write mistake: initial integration used Windows default read encoding on the UTF-8 generator file, changing a few comments and canonicalise_stack_objects docstring outside the intended function. Candidate_normalise.py scratch itself is CP1252; it must be read explicitly as CP1252. Restored generator from exact UTF-8 baseline plus tested14-line rule, after session6439 was confirmed terminal. Executable AST excluding docstrings is equal before/after correction; original comment/docstring text preserved, diff now21 lines containing only14 inserted rule lines. encoding_correction_review.json and generator_change_corrected.patch pin this correction. Earlier attempted UTF-8 read of CP1252 scratch failed safely before mutation. Broad175 /28 subtests pass again on corrected file.

Fresh after_corpus_final.py session15209 live under corpus_final/after and corpus_final_inputs/comparison.json. This will be the promotion gate; first corpus artifacts remain intact but superseded for promotion. Inputs frozen; do not restart or modify generator/evidence while running. Next: poll15209, require all29 results and input/output hashes, inspect every changed unit, parse all files, then promote four expected Lua files and reports only if gates pass. New unique package and readability audit remain pending. Current root outputs/package/ledger still describe completed DarkwoodTrader actor/refusal recovery. No install/game launch.

## Float-spill copy recovery promoted offline

Corrected-input final corpus session15209 terminal0. All29 registered units complete, only AssassinSpawn/RockTrollTrigger changed in each style; other28 units unchanged. All534 Lua parse, baseline/output/root input hashes checked, four final files equal tested scratch outputs. Promoted four Lua and three reports; promoted hashes rechecked. Root UTF-8 text preserves original comments/docstrings with only the14-line rule inserted. First corpus retained but superseded for promotion; no stale-input acceptance.

New offline bundle work/codex_lua_trader_distance_spills_20261005/trader-distance-spills-candidate-20261005:37 Lua/42 manifest hashes, e777ce0e DLL, baseline07774e98/patch17cc7399. Build/source/RemoveThing ABI/binding hashes revalidated. Readability371 files/syntax0, diagnostics971, generic6088, machine4649, gotos4603, native names6344; status60 recovery/155 readability/156 human. Ledger refreshed from completed audit. No install/game test. Current pass workers terminal.

Next existing registered WatchForSurprisingBalverines E06050 local vector clear; work/codex_lua_trader_vector_clear_20261005. Call E060AC targets8AC9D0, full-range std::vector<CScriptThing> erase. Original complete erase + empty tail-copy helper8AC2A0 + actual CScriptThing scalar deleting destructor4AB170 flags0 passes20 cases:0/1/2/5/11 actors, shared/distinct controls, outside references0/2, correct final Info/payload release, capacity retained and stack balance. Base lifecycle, payload deleter and allocator are explicit recording boundaries; no native fill or Lua binding execution yet.

GetAllThingsWithScriptName slot12C reads target8A8570 from original GSI vtable. Complete caller body8A8570..8A86B5 covers both returns/all internal branch targets. It reserves at8A85E3 through8ACD30, builds temporary ScriptThing through4C7E50, increments vector-owned Info at8A861E and releases temporary Info at8A8648. External calls not yet executed. Existing sidecar GetAll creates/reserves host std::vector<CScriptThing>, copies each element to a heap buffer and WrapScriptThingOutput(...,true), then returns. FableAPI.h CScriptThing/CCountedPointer are plain structures without destructors. Review retained vector references and allocator ownership in a composed native/binding fixture before treating local clear as fully recovered. Source comparison raises a concrete ownership question; compiled sidecar lifetime/game behavior remains unproved. No new generator/evidence/output edits for this next target.

## Actor-vector ownership leak reproduced before clear lowering

Original complete GetAll8A8570..8A86B5 composed with original erase/destructor passes24 cases (0/1/2/5/10/11 actors, shared/distinct controls, outside refs0/2). Query, owning temporary construction, reserve/allocation, CString and deletion boundaries doubled; vector inline retain and temporary decrement are real instructions. Final erase returns each control to outside references. native_get_all_clear_validation.json.

Win32 binding_ownership_before.vcxproj built with VS2022 MSBuild/v143, terminal0; executable terminal0. Exact production LuaQuestState::GetAllThingsWithScriptName and WrapScriptThingOutput bodies extracted and hashed, actor12-byte POD layout/trivially-destructible assertion compiled. Table/shared-pointer ownership, native query, allocator and IsNull boundaries doubled. The query's owned-copy contract follows separately executed native body. Six counts0/1/2/5/10/11: all five nonempty cases leave RefCount3 after Lua handles released, expected outside2. GetAll's host std::vector<CScriptThing> POD destructor frees storage but does not release native element references; Wrap shallow-copy retain adds a separate Lua-owned reference. Leak now reproduced in compiled production bodies, not just inferred from comments. binding_ownership_before_validation.json/binding_extraction.json. No whole DLL/game execution.

Next: create an owned sidecar candidate, recover engine-compatible temporary vector allocation and native element destruction (reserve10 host buffer must also be reviewed against native reserve/reallocation), test success/empty/shared/final-reference/failure cases and compiled ownership cleanup. Keep current e777ce0e packages/source evidence unchanged. Only after binding ownership is sound, recover the existing local full-range clear via generator/evidence and its full-function/corpus gates. No shared generator, sidecar or generated output change for this target yet. All current workers terminal; no install/game launch.


## Active Novi vector binding and exception cleanup

Correction to preceding stock-binding investigation: LuaManager.cpp:1266 registers RegisterNoviUnitOverrides after the stock GetAll registration at355. NoviUnitBindings.h:650 routes GetAllThingsWithScriptName through NoviFillThingVector. Existing native-allocated12-byte vector and retail copies/destructors balance success; the stock leak is real but is not the active Novi lookup path. binding_registration_review.json records the order. The active helper lacks cleanup when fill/copy/table insertion throws.

Owned scratch sidecar clone at work/codex_lua_trader_vector_clear_20261005/sidecar_candidate, baseline07774e98, inherited camera patch applied. Copied66 vendor files with hashes. Exact active-helper Win32 fixture144 cases (counts0/1/2/5/10/11, shared/distinct controls, outside0/2; success, insert1/3 throws, copy2 throw/null, partial-fill throw): before68 leak cases, scope-cleanup candidate0. Query/copy/destructor/allocator/table boundaries doubled; no native GC/game claim. active_vector_validation.json preserves helper/source/executable hashes. Only NoviUnitBindings.h has the new scope owner; previous candidate/package untouched.

Original complete vector destructor8AC970..8AC9A1 plus original actor scalar destructor passes20 lifetime cases, confirms BFEA14 buffer free and per-element destruction order. Native_vector_destroy_validation.json; external payload/base/free recording boundaries remain explicit. First full isolated DLL build passed. Git apply normalized seven mixed line endings in four inherited files; all six inherited source files restored byte-for-byte against old build hashes; inherited_encoding_review.json. Final forced Rebuild active owned session59143 (incremental no-op is not accepted as final proof). Poll without restarting. Export patch from actual owned-repo committed diff after final build/hash checks. No outer commits, installs, game launch or generator/output changes.

Remaining Lua clear must preserve retained-list refresh/thread-exit lifetimes, not simply drop the TODO under nondeterministic Lua GC. Native clear keeps capacity and drops prior element refs before the next lookup. Existing Lua lookup returns independently retained table handles; a scoped native list primitive may be needed.


Final forced Rebuild59143 terminal0. All six inherited source hashes match previous DLL inputs byte-for-byte; new seventh NoviUnitBindings.h recorded. Clean isolated source commit33b64e802e3982c30d2a6746a9f8f6cc663c89fa. Built DLLf3f22d4164f31b95f86db53d0a0686d46acd7debefd7262ba01c6af518d861b7. Exported actual baseline-to-commit patch novi-camera-vector-scope.patch SHA94bab780133ca4248b2786c29f8c8a77b35a9f4be530e7b3670f00b24e6a1d59. sidecar_build.json preserves seven source hashes; no package/install/game change. Current owned build workers terminal.

Next generic scoped native Thing vector: Clear preserving capacity, FillScriptName append, Count, GetPos(index) without extra Lua-retained handles, Close. Resource callback owns deterministic cleanup across yields/returns. Existing ReadRetailThingPosition in LuaRetailResources.h provides native implementation-position fallback policy. Native CScriptThing GetPos target4AA980 reads Data+4, returns relocated0143E8E0 if empty, otherwise tail-jumps implementation vtable slot18. Full two-path instructions captured (first-return-only capture superseded). Validate original GetPos and compiled scoped-vector refresh/exception/closed-state behavior before sidecar integration and reviewed caller lowering. No root generator changes yet.


Generic RetailThingPositionList scratch proposal passes72 compiled lifecycle cases: first fill/count/positions, full clear releases references, refill capacity reuse, partial-fill and CString-destroy exceptions, index/closed guards and idempotent close. Exact class body with API declarations substituted; native query/erase/destroy/CString are doubles. position_list_validation.json. Original complete CScriptThing GetPos4AA980 six forward/fallback cases pass; implementation result boundary doubled. native_thing_getpos_validation.json.

Position list integrated in isolated sidecar: new retail_thing_position_list.h plus LuaRetailResources generic factory, entry cleanup and Count/Clear/FillScriptName/GetPos/Close binding. PositionSource pointer consumed immediately by existing ReadRetailThingPosition; no actor userdata/extra retained copies. Build91044 terminal0, source/DLL hashes sidecar_position_list_build_pending_review.json. This extension remains uncommitted/unexported; prior33b64e8 patch/DLL metadata is an earlier exception-only checkpoint, not current working source.

Existing caller capture5781 completed draft65 functions/16 parse/33 TODO, then scratch assertion mistakenly expected lowercase address. Corrected assert, completed readable-only67290 terminal0 (17 parse, one master diagnostic). No converter rerun needed. Scratch caller_lua_candidate replaces exact full-range erase, fill/count and two position expressions with native scoped list; callback ensures close across all returns. Whole draft before/after2592 trace/final-state comparisons pass, including1036 multiple-refresh cases and termination checks1..24, needed0/1/2, counts0/1/3, onscreen/offscreen/mixed, alive0/2frames, random0/1. First mismatch was mock trace integer-vs-float string rendering at GetPos; fixture now formats integral indices consistently. Lupa process separate from all native Unicorn runs. Caller_mock_validation.json explicitly limits this to recording mocks, not native host/game behavior. No root generator/evidence/output change. Next: native caller lifetime oracle before fail-closed restoration integration and full29 validation; then final sidecar commit/export/package. All current owned workers terminal.


Original complete WatchForSurprisingBalverines240 lifetime executions pass on all-screen-visible/needed-zero paths, counts0/1/3/11 and termination checks1..30. Real outer body, erase/tail-copy/vector/scalar destructors/GetPos; query appends owned references under a doubled boundary and asserts native clear before every refill; actual final release/free and stack balance. native_caller_list_lifetime_validation.json explicitly excludes spawn branch.

Scoped list extension now clean isolated commit a3898117ff4c1553b41c573e040133e62cc778bf. DLLd9d7f4b97d8648ad702042ae4394135bbb51e8fd0f57226cfded3d87eade136c, combined actual patch0c11c1c07e043a2fbb66ed8949b9125783f3f669132e1237e51c74e8f41111fc; nine source hashes in final sidecar_build.json. Previous exception-only checkpoint retained.

Integrated root native_reviewed_trader_vector.py, converter hook after sound restoration, reviewedTraderVectors existing unit row and exact lowering fixture.14 fail-closed tests pass; related trader suite50 plus2subtests. First success test rejected recovered hash because fixture-file hash used Windows CRLF bytes while restored source is LF; corrected metadata to normalized source-text hash, preserving native/sidecar raw-byte hashes. baseline_input_review.json proves removal of exactly new hook/row recovers prior completed full29 input hashes; other prior inputs unchanged.

Scratch20151 terminal0:65 functions/16draft and17readable parse, TODO32 from33; only marker helper changes. Readable callback limits existing presentation simplifications, increasing master generic temporary count; this is not behavior evidence. Emitted draft/readable each2592 exact recording comparisons pass (1036 repeated refresh), retaining existing style frame-check/cached-Hero policies. Scratch extraction initially matched function names inside generated thread comments; now anchors newline function declarations. caller_*_mock_validation.json. Smoke JSON checked with explicit empty position-list fixture/new source profile: readable retains only default empty-vector assassin modulo finding; draft also retains extraout_EAX_00 free-global finding. No new findings.

Full29 owned65806 running; trader master only differs each style so far and completed unrelated units unchanged. Do not edit generator/evidence inputs while it runs. Prepared audit_promotion_gates.py verifies full output/input hashes,534 syntax expectations, native/compiled/caller/smoke gates and sidecar provenance before five-file promotion plan. Prepared promote_reviewed.py/package.py target two master Lua plus three reports and unique trader-vector-list-candidate-20261005. Neither promotion nor package executed yet. Poll exact65806; then gate/promotion/package/audit. No outer commits/install/game launch.


## Surprise-marker native vector lifetime promoted offline

Full29 owned65806 terminal0: only TraderEscort/TraderEscort.lua changes each style, all other28 units unchanged. Gates verify inputs/output/baseline hashes, all534 Lua parse and exact previously tested scratch outputs. Promotion90981 terminal0, two master outputs plus CONVERSION_REPORT.json/md and READABLE_REPORT.json only; five hashes rechecked. No generated Lua hand edits.

Additional real production-header/sol+bundled-Lua Win32 probe16 cases passes: exact five-method binding token sequence matches production registration, forward and relocated fallback xyz values, counts0/1/3/11, reference release with retained Lua userdata and closed-state Lua errors. Uses actual headers and ReadRetailThingPosition; query/erase/destroy/CString/implementation GetPos are doubles, fallback vector read from allocated relocated memory. Resource factory/host coroutine/game not executed. position_list_sol_validation.json. First extended fixture joined Lua end/end without whitespace, failing syntax assertion and producing unhandled C0000409; rejected source retained, corrected snippet and added main exception reporting; corrected build/executable terminal0. No acceptance from the failed run or a shell's subsequent exit0.

Package38144 terminal0: work/codex_lua_trader_vector_clear_20261005/trader-vector-list-candidate-20261005,37 Lua/42 manifest hashes, DLLd9d7f4b97d8648ad702042ae4394135bbb51e8fd0f57226cfded3d87eade136c. Clean isolated commita3898117ff4c1553b41c573e040133e62cc778bf and actual baseline-to-commit patch0c11c1c07e043a2fbb66ed8949b9125783f3f669132e1237e51c74e8f41111fc. Nine build sources and four previous RemoveThing ABI sources match; three unchanged prior binding sources verified, NoviUnitBindings retained-copy/release bodies unchanged except scope cleanup. package_inputs_validation.json/final_validation.json preserve scopes. Previous bundles unchanged. No outer commit/install/game launch.

Readability74527 terminal0:371 files, syntax0, unresolved970, generic6107, gotos4603, native names6344, machine4649; status59 recovery/156 readability/156 human. Ledger refreshed once from complete audit. Extra generic locals stem from existing readability pass handling callback scope; reduced diagnostic count does not certify readability. All current pass workers terminal.

Next existing registered Lua recovery: same WatchForSurprisingBalverines CreateCreature final flag. Native decompile supplies bVar5=true, current emitted Lua supplies only three arguments, stock LuaQuestState::CreateCreature hardcodes false. Verify original stack/callee, actual registration/overrides and generic marshalling before choosing binding/evidence/lifter fix. Read-only scoped search75151 pending; no spawn-flag generator/evidence/sidecar change. New scratch destination work/codex_lua_trader_spawn_flag_20261005. User continuous authorization and Fable2-closed launch condition persist.


Spawn-flag read-only evidence under work/codex_lua_trader_spawn_flag_20261005/investigation.json: native caller push1 E0619F precedes script/position/definition/output pushes and callE061B7 slot16C. Original GSI vtable points8A9100, complete532-byte body ends8A9314 with RET20 and contains all direct jump targets. Flag at [esp+174] read8A9248; nonzero executes74DBB0/6644F0/62E900, false skips to8A926C. Flag operation meaning unclassified, preserve unknown name. Initial caller window started mid-instruction, rejected/superseded by full-body disassembly filtered to exact boundaries. Initial callee0x200 slice had no return; superseded by complete0x214 extent. LuaManager:308 binds stock CreateCreature; Novi overrides have no CreateCreature replacement. Old manifest has three Lua params; lifter raw_args trimming around1987 discards native final bool. Stock binding hardcodesfalse.

Read-only search75151 had malformed Windows quoting: rg argv combined pattern and intended file paths, causing broad workspace search. Verified exact owned rg18736 argv through psutil and terminated only that process; remaining search command terminal0. Narrow Python source/manifest/native reads51385 and follow-ups terminal0. No flags-pass root/sidecar edits. Next prove call stack/native flag window and exact compiled binding loss, then extend owned sidecar optional bool/generic converter signature, update current reviewed vector shape only when new intermediate behavior is tested, and full29 compare all CreateCreature callers.


## CreateCreature final native flag recovery in scratch

W=work/codex_lua_trader_spawn_flag_20261005. native_spawn_call.py7 original caller executions stop atE061B7: receiver/output/definition/script/position verified and final word1, offscreen indices0/middle/last at marker counts1/3/11. native_flag_window.py6 original branch/call-frame executions; zero low byte skips, nonzero runs constructor/apply/destructor recording boundaries, stack balanced. Dependency first-return decoder initially crossed destructor62E900 tail jump into next function; rejected RET4 interpretation. Actual destructor ends tail jump693EF0, whose RET0 captured; window boundaries use4/4/0 purges. Noncanonical raw words illustrate byte read only; canonical Lua booleans remain target. Operation meaning still unclassified.

Exact stock CreateCreature body extracted/hashes: Win32 before6 calls reproduces hardcodedfalse. Owned sidecar clone92145 terminal0, baselinea389811; restored nine parent input bytes and copied66 vendor hashes. Generic binding adds sol::optional<bool> nativeFlag={} and forwards value_or(false); matching generated registry parameter added. Preserve all inherited mixed line endings outside edits. Exact after22 compiled default/true/false/prerequisite/malloc failure cases and12 real sol/bundled-Lua missing/nil/true/false/position/adverse cases pass. Query/CString/wrapping/class are explicit doubles; no native engine/game/actor lifetime claims. binding_validation.json. Full DLL build28110 terminal0. Clean isolated b05e856a13c56e559194bb73aa70a9812dad600b, DLL9f096cc99c27bd4cce2c315ce7fec4cab742a80b972eb8e5037fbfdf4d90d9bb, actual combined patch09fc84a3fef256c3080911c6c171bb20f4fecaf0dd2bf6d47f446e3ee37a1fcc; nine source hashes sidecar_build.json. Git initially reported LuaManager dirty but empty diff (stat/mixed line-ending refresh); git add solely refreshed unchanged owned input, clean tree then verified. No outer commits/install.

Before root changes62928 hash reader terminal0; all1824 prior generator/evidence inputs verified (30.6MB; long Windows I/O wait, process live and observed, never restarted). Snapshot generator_baseline_inputs.json/lifter_before.py. Scratch explicit signature10 before/after emission cases prove current lifter drops true/1 under old SDK, four-parameter candidate retains true/false and numeric1/0, omitted staysfalse. Root converter_signatures now narrow exact SDK3-arg shape overlay; shared SDK manifest untouched.12 focused tests pass, including unsupported/newer shapes. Initial four negative tests mistook pre-existing vector nativeKind annotation for a new overlay change; corrected tests check preserved argument contract rather than unrelated annotation.

Scratch capture30670 computes only updated reviewed vector hashes and verifies native/source/sidecar gates with a temporary row:65 funcs/16draft/17readable parse, TODO32. Actual new intermediate differs by exactly adding existing bVar5 as fourth CreateCreature arg, immediately after bVar5=true; all other source text exact. Updated root reviewed vector row/fixture, native callee hash and three new host signature sources pinned; vector_shape_update_validation.json. Integrated pipeline92167 terminal0, same syntax/TODO counts. Both emitted caller styles2592 comparisons (1036 refresh) assert every surprise spawn intentionally changes false-to-true flag; other exact traces/final state and deterministic list close unchanged. caller_*_mock_validation.json. No generated output promotion or new bundle.

Broader signature/vector/animation/movement/camera run9668 terminal1:38 pass/1 failure, test_camera_signature.py normal Sister camera cleanup (abortFalse), Lua assert line43 nil. First baseline/current pair72962/35237 both failed but comparison was insufficient: temporary output directories already deleted so source sets empty, and baseline parent patch did not reach converter subprocess. Reports retained as rejected_incomplete_baseline/rejected_empty_sources, never acceptance. Corrected comparator propagates old converter_signatures through sitecustomize into generator child, retains owned temporary artifacts, preserves full logs; active baseline53429/current43524. Poll exact live handles, do not restart. Need nonempty two-style source hashes and matching identity before classifying pre-existing. Then full29 against complete vector checkpoint; audit every changed CreateCreature flag before promotion/package/audit. Root generator/evidence held stable during readers. User launch condition persists; no launch attempted.

CreateCreature flag continuation: corrected camera comparison workers53429/43524 terminal; both pytest1, same normal Sister cleanup Lua assertion43/nil error. Retained two nonempty draft/readable Sister sources byte-identical; prior converter_signatures propagated into subprocesses through owned PYTHONPATH sitecustomize. Valid camera_regression_comparison.json supersedes rejected empty-source comparison. Full29 after regeneration launched owned session4735 against completed vector corpus; do not edit shared generator/evidence while live. Four Trader smoke runs terminal0, entire before/after JSON identical each style; existing modulo-zero/free-global findings remain, not runtime proof.

CreateCreature corpus first pass session4735 terminal1: DragonBossFight reviewed SpawnSummoners intermediate guard correctly rejected new four-argument shape; partial corpus retained, no promotion. Arena PlayWave original windows F1F847->F1F865/F1FBE1->F1FBFF each stage push1 before zero-argument GetPos;18 original window cases pass. Generic native_create_creature_flag now recovers effect-free SUB41 merged final flag only with exact Quest receiver, native immediate0/1 across one GetPos and five pushes, linear predecessors, pinned GetPos/CreateCreature ABI bodies. Dragon helper exact shape/diagnostic updated to retain existing bVar2=false; no native/source row identity change. Focused27 tests +5 subtests pass. Initial new fixture used function size9232 rather than body extent9262 and lacked switch-table reads; corrected full extent/backing reads, rejected runs retained in tool log, no failed acceptance. Metadata negative now perturbs paired entry; helper rejects missing pairing. Full29v2 new outputs launched session53610 against complete vector baseline; generator/evidence inputs frozen while live. Original corpus_final scratch remains rejected/partial; accepted output not promoted.

Native CreateCreature inventory retained registered_create_call_native_inventory.json:85 slot16C sites across14 registered units;57 immediate and28 saved-register constants, CFG reaching definitions all uniquely0/1. Source preliminary76 literal/nearest bool assignments agree; Arena SUB41 now emitted true, remaining8 numeric CCharString casts need full emitted audit (Chicken CVar27/TraderConflict CVar15 source values0, not semantic flags). Broader v2 suite owned34274 terminal1:65 pass+5 subtests, only known normal Sister camera failure. Full29v2 session53610 remains live; Trader/Arena/Bandit/Beardy/Book/Beggar/Chicken/Dragon completed so far; Dragon updated exact guard passed. No promotion/package/install/launch. Next poll53610 then run audit_call_diffs_v2.py; review all native flag operands and check destination baselines, preserving hand-reviewed readable variants.

## CreateCreature final native flag promoted

Full29v2 session53610 terminal0:12 changed units,26 Lua files each style,78 draft call additions only;17 other units unchanged;534 parse. Readable local-version canonical review26/26 after explicit immutable grade-copy normalization; strict alpha review24/26 retained, intermediate rejected normalizers retained. Actual grade fragments19 cases agree; no complete Guild runtime claim. All48 smoke runs terminal0,24 exact full-JSON before/after comparisons unchanged. Native inventory85 sites all0/1 (57 direct,28 saved-register CFG); source review85 agrees including numeric casts and repaired Arena merged byte operand. Root27+5subtests and broader65+5 with one known baseline camera fixture failure.

Promotion gate terminal0 after correcting PowerShell redirected broad-log decoding to UTF16 (initial UTF8 reader rejected before any promotion).88 destinations reviewed:52 Lua+36 reports. Destination drift only pre-existing Arena immutable timerId5=timerId two reads, both names assigned once, documented prior2026-10-02; equivalence reviewed. Hand-reviewed Guild tree separate from readable_converter and preserved. Promoted files hash-verified. New package trader-create-flag-candidate-20261005:37 Lua/42 hashes, b05e856 sidecar clean, DLL9f096cc9 and committed patch09fc84a3; build and RemoveThing body hashes checked. Readability371 syntax0,970 diagnostics,6111 generic temps,4603 gotos. final_validation.json is authoritative offline checkpoint; no install/game/outer commit.

Next observed existing task: Sister camera test mocks me:Speak with no return. LuaManager production overloads return Speak_Blocking bool; generated if-not-Speak cleanup is therefore taken on normal mocknil, and abort mock case also exits before its intended task loop. Verify actual blocking binding/body, then repair fixture to cover true normal/abort plus explicit false speech cancellation; no generator fix inferred from stale fixture. Full29v2 corpus is next baseline; preserve completed package and rejected first corpus.

## Sister camera recording contract repaired

Owned work/codex_lua_sister_camera_fixture_20261005 retains before/after fixture, binding_contract.json, input_equivalence.json and validation.json. LuaManager:200-206 returns Speak_Blocking bool; LuaEntityAPI.h:109 and cpp2789-2894 confirm true normal and false missing prerequisites/command failure/exception/wait termination. Mock previously returned nil, incorrectly taking generated false-return cleanup. Old abort case therefore did not exercise intended caller wait. Four cases now model successful blocking speech, native-host wait abortfalse, later caller wait interruptiontrue and issue failurefalse; exact camera/movie/timer/resource trace and frame-path counters checked in both styles.

Fresh camera worker27617 terminal0:10pass64.78sec. Related non-camera suite58pass+5subtests4.09sec,68 total tests. No failure remains in this selected suite. Only test_camera_signature.py differs from complete full29v2 input hashes; all generator/evidence/othertests unchanged. No generatedLua/sidecar/package/install/game changes, no repeated ledger/full29 audit. Previous failed logs retained as historical evidence; flag final_validation.json links this resolving follow-up.

Next reached existing helper DarkwoodTrader SetBrainState E0A510..E0A81A: draft freeglobal extraout_EAX_00 at terminal assignment after AddQuestInfoBarHealth; readable removes it, but draft/native signature is int due stale autoname. Unit maps helper through PDB string-set match; PDB locals function evidence available at refs/script_recovery/trader_escort/pdb/Ego_r-pdb-locals.tsv. Confirm genuine return type/retail callers before any lowering change. Separate substantive option: full surprising-balverine original caller spawn-branch oracle, currently excluded from prior list lifetime cases.

SetBrainState return investigation progressed: query_pdb_oracle.ps1 exact NScript::CQ_TraderEscortScript::CDarkwoodTrader::SetBrainState returns void(long brain_state), Ego_r GUID774B3052-BDD8-43B4-808D-48481CC576CE age4,debug965 bytes; debug addresses are not retail. Captured UTF8 oracle text and original retail778-byte body E0A510..E0A81A/source hashes in work/codex_lua_trader_brain_state_return_20261005. Current typed/autoname int is wrong. No generator/evidence edits yet for this pass. Next add guarded generic reviewed-prototype override rather than fabricate a bsim comment; audit native return/callers and helper outputs, then full29 promotion gates. Current workers terminal.

## SetBrainState reviewed void prototype in scratch

Generic native_reviewed_function_prototype helper and unit reviewedFunctionPrototypes row now pin source/native body,778-byte extent, existing int brain_state parameter, genuine UTF8 PDB oracle artifact and void(long) signature. All four converter helper-signature sites use it; unreviewed units retain original function_parameters behavior. Explicit reviewedVoid lets existing return-value cleanup remove stale scalar results without invented bsim comments. Effectful return expressions refused.13 contract/adverse tests pass. Unit before/converter snapshots retained.

Caller conversion22296/readable72965 terminal0,65 funcs/16draft/17readable parse,TODO32 unchanged. Draft delta is four stale return values only; effect calls preserved. Readable helper becomes guard flow with no false integer return.504 full emitted before/after comparisons each style: exact effects, entity final fields and termination count agree, new result alwaysnil. Original778-byte native helper504 cases: four RET4 sites, direct Main caller overwrites EAX; mutating API effects, actual native colour stack bytes, fields and termination match both emitted styles504 comparisons. Native GSI/CString/compare/termination/actor doubles explicit; getter read traces excluded, no host/engine/refcount/game proof. Initial native fixture missed two literal pages for null-string inline comparisons, diagnosed atE0A6C6/012676D4; rejected source retained, mapped all reached literal pages then full504 passed. Colour argument is by-reference stack BGRA bytes, correctly normalized to RGBA trace; return EAX intentionally arbitrary after CString destructor, PDBvoid ignores it.

Full29 scratch session92057 live against completed CreateCreature corpus_final_v2; only DarkwoodTrader changes so far. Freeze generator/evidence while live. No promotion, package, install, ledger refresh or game launch for this pass yet. Next poll92057, related tests/smoke/gates, guarded promotion and new unique Trader package reusing proven b05e856 optional-flag sidecar. Preserve completed packages and hand-reviewed Guild tree.

## SetBrainState void result promoted

Full29 session92057 terminal0:only DarkwoodTrader differs eachstyle;534 parse,28 unrelated units identical. Guarded promotion checks source/evidence/oracle and test input hashes; native504/full emitted504/style comparisons and90 related tests pass. Four smoke runs terminal0/exact JSON before-after equality. Existing Assassin modulo-zero and draft extraout_EAX_00 freeglobal remain classified, not falsely resolved. Draft delta exactly four stale scalar return values; no side effects dropped. Readable helper now early guards and side-effect calls followed by plain returns.

Five outputs/reports promoted and hash-verified. Package terminal0:trader-brain-state-void-candidate-20261005,37 Lua/42 manifest hashes; reused b05e856 source,DLL9f096cc9/actual patch09fc84a3 and nine hashes all match completed flag candidate. No sidecar source/build change. Readability audit86028 terminal0:371 files/syntax0,970 diagnostics,6093 generic temps,4603 gotos,6344 native names,4649 machine temps. final_validation.json records scope and exclusions. No install/game/outer commit. Current owned pass workers terminal.

Next authorized existing proof work: original full WatchForSurprisingBalverines spawn branch beyond E061B7 staged CreateCreature call. Current previous native lifetime240 cases exclude spawn; flag7/window6 prove operands/bit behavior only. Extend original caller with explicit GSI creation/resource/frame/actor doubles, preserve native erase/destructor references and compare generated effects/state/lifetimes. No new conversion selection or generator edits inferred yet.


## WatchForSurprisingBalverines full spawn caller proof

Owned scratch `work/codex_lua_trader_spawn_caller_20261005/final_validation.json`: 26 original temporary-destructor/IsAlive cases, 2,400 complete native caller cases, and 2,400 ordered native/current-Lua comparisons per style pass. Original actor temporary cleanup and IsAlive wrapper compose with original vector helpers. Termination checks 1..25 cover needed1/2, lists1/3, offscreen positions, alive0/1/3 and both random-target branches. References return to initial2; vector free/stack balance checked on every exit. Engine factories, implementation IsAlive, CString, comment callee, allocator/base and host remain doubled; hero/position/camera getter reads excluded from ordered comparison. Rejected camera fixture retained; per-fill index reset corrected. No root code, generation, sidecar, package, install or game change; full29 baseline stays brain_state_return/corpus_final/after. Owned sessions 38997/90357/85110/59251 terminal0. Next: empty creation/nearest results and host ownership boundaries.


## Generic empty enemy-target binding recovered

Owned `work/codex_lua_trader_empty_target_20261005/final_validation.json`: native GSI8A0260 with empty target can reach component8 setter(null) and actor notification; 30 original full-callee/wrapper cases pass with dependency doubles. Stock binding skipped nil target, and real-sol const shared_ptr& nil arguments entered the method then crashed during shared_ptr access (verified by retained entry diagnostic). Generic raw CScriptThing* signature preserves owned/borrowed userdata and forwards nil target as a borrowed zero-data/info wrapper with actual Thing vtable.48 exact method direct/real-sol owned/borrowed/nil/guard cases pass; earlier 96 progress count was wrong.2400 complete native caller empty-nearest cases and 2400 ordered emitted comparisons per style pass; comment callee/factories doubled. 90 related pytest tests pass (7-test unittest invocation was only a narrow check). Owned clean sidecar commit5a7d2e1, parentb05e856; only method/declaration/registry changed. Nine source hashes and66 vendor files audited. One LuaManager checkout newline conversion restored to prior exact bytes and rebuilt: session23391 terminal0; earlier82258 terminal0 logs retained. New DLL4423cdf9/cumulative patch 1bff1af2; new trader-empty-target-candidate-20261005 bundle37 Lua/42 hashes verified.1829 full29 inputs and5 prior promoted outputs unchanged; no generator change/regen/Lua promotion/ledger change. All current-pass workers terminal. No install, game launch or outer commit. Next: complete original89A510 empty relative GetNearest lookup proof and repair that generic binding if justified.


## Generic empty-relative nearest lookup recovered

Owned `work/codex_lua_trader_empty_relative_20261005/final_validation.json`:24 complete original89A510 callee cases compose original relative GetPos fallback, scalar/temporary destructors and x87 integer conversion. Empty relative still queries and selects nearest from current relocated fallback position; stack and list/output counts balance. Query/candidate implementation/base/allocator doubled. Generic GetNearestWithScriptName now accepts owned/borrowed raw Thing pointers and forwards Lua nil through borrowed empty wrapper;72 exact direct/real-sol guard/ allocation/valid/empty-output cases pass.800 complete original failed-creation caller cases compose original GSI cutscene/facing/target guards;800 ordered current-Lua comparisons each style pass with factories/comment/host doubled. Recording fixture syntax separator and C++ proxy nil result probe were corrected and rejected inputs retained; emitted runner pins Lua5.4. Clean sidecar df094fc parent5a7d2e1, build9993 terminal0, DLLfda5b7c0/cumulative patchfbc7fd48. 37-Lua/42-hash trader-empty-relative-candidate-20261005 offline package verified. All1829 full29 inputs/5 prior promoted outputs and66 vendor files unchanged; previous root90-test report reused with hash rather than rerun; new binding independently compiled/tested. No root generator/unit/Lua changes, promotion, ledger regeneration, install, game launch or outer commit. All owned pass workers terminal. Next: actual WrapScriptThingOutput count1 versus original native temporary destructor, then deterministic spawned-actor retirement; marker list scope does not yet prove actor/nearest userdata GC/host timing.


## Generic owned Thing output final cleanup recovered

Owned `work/codex_lua_trader_actor_ownership_20261005/final_validation.json`:16 exact stock wrapper diagnostics reproduce missed count1 implementation/info cleanup and4 owned-null-filter reference leaks. Original scalar flags0 destructor 10 cases plus16 IsAlive runs confirm native cleanup/RET4 protocol. New generic wrapper calls current virtual scalar destructor(flags0), then captured heap free; API-owned null results retire, unretained shallow copies do not release borrowed references. Pending unique owner handles IsNull exceptions; shared-control allocation failure retires once.16 exact candidate/33 adverse/16 actual SDK x86/ 16 actual Lua-sol forced-GC cases pass,81 compiled cases total. Native callbacks, base and allocator remain doubled; malformed nonzero ownership with missing vtable not established as valid engine output. Clean sidecar cfdc974 parentdf094fc, one FableAPI.cpp method changed; build19159 terminal0. Separate retained ABI source checks and nine source/66 vendor hashes pass.37-Lua/42-hash trader-actor-owner-candidate-20261005 offline bundle verified.1829 corpus inputs and5 prior promoted outputs unchanged; prior root90 test report reused with hash; new wrapper independently compiled/tested. No generator/unit/Lua changes, promotion/ledger, install, game launch or outer commit. All owned workers terminal. This proves eventual cleanup with forced GC, not native actor release before the next frame. Next: actual-sol emitted coroutine/native frame snapshots for actor/nearest lifetime; recover deterministic scoped output retirement if needed0 looping retains actors.


## Emitted actor frame ownership gap confirmed

Owned `work/codex_lua_trader_frame_ownership_20261005/final_validation.json`: original complete caller needed1/count1/dead actor/random1/stop25 snapshots show actor count2 before every needed0 frame. Current complete draft/readable function in actual Lua5.4/sol coroutine with exact cfdc974 wrapper keeps count3 across4 needed0 frames/style despite forced GC; thread end/drop/GC returns2. Factory/GSI/actor liveness/resource scope/base/allocator doubles explicit; native actor/vector/helper bytes executed. This changes the next action: eventual GC cleanup is insufficient; native E0635F retirement precedes next frame. No root generator/evidence/sidecar/Lua changes, package/install/game launch or outer commit. Owned probes terminal. `ownership_requirements.json` records next implementation invariants: identify only owned API outputs, preserve borrowed and by-value helper lifetimes, retire once with safe surviving aliases, native nearest/actor/list ordering and every early exit/yield. First inspect actual 99A2E0 base destructor/vtable and helper copy ownership; then implement generic owned-output metadata/scope retirement, reviewed lowering and full29 ladder.

Follow-up: original99A2E0 seven-byte base destructor composed with original scalar4AB170 in10 scalar/16 IsAlive cases. It changes wrapper vtable to01231710; therefore deterministic retirement must restore a valid empty Thing vtable for surviving Lua aliases and suppress duplicate native cleanup at later GC. Native destruction/free/implementation remain doubled. Evidence:frame_ownership/native_base_vtable_validation.json.


## Deterministic Thing ownership candidate

Owned `work/codex_lua_trader_deterministic_owner_20261005/checkpoint.json`: clean source 5559d671673122cf1da935ee3c0a952d1665743f, parent cfdc974, Win32 Release v143 build terminal0. 124 compiled cases (SDK24, actual-sol16, extracted production scope20, exact factory/adverse64) pass. Raw pointer registry identifies API outputs without shared_ptr storage probes; native ref retired once separately from wrapper heap lifetime, empty Thing vtable restored for surviving aliases. Scopes hold host metadata without native retain; cross-scope rejection, idempotent owner retirement, native copy, nil/borrowed handling and callback multiple/false returns covered. Other resource kinds in scope test omitted; native callbacks and heap boundaries doubled/CRT; production game/host/yield proof remains. No root generator/evidence/Lua changes, package/install/launch or outer commit. 1829 baseline root input hashes unchanged. Neighboring comment_ownership native oracle executes1152 complete original MakeTraderComment bodies with per-instruction refs: initial speaker1->2 retained copy and all refs return1 on exit, including termination paths. Next implement reviewed caller retirement; then separate helper local copies and full29 acceptance.


## Visual-testing priority corrected; user now at computer

User asked why the authorized nine-hour absence was not used for visual testing. Agent acknowledged wrong prioritization: repeated offline recovery instead of using authorized launch window. Process check found no Fable.exe, Fable2 or xenia process; unrelated FableForge asset-check process left alone. No game launched or installation changed. User then stated they are now at their computer. Continue Lua offline; defer launching or taking screen/input while they are using it. Overall continuous Lua goal remains active. Original Fable2-closed launch condition still applies when unattended testing resumes.

Deterministic caller pass: shared generic lowering/evidence added, full29 first pass complete; only Trader Escort caller file/style changed.534 parse, 106 related tests plus16 final lowering tests,5600 ordered comparisons/style, actual SDK/sol actor/nearest counts2 before4 needed0 frames/style,16 injected caller errors. Scope Add allocation rejection probe12 found0/8 immediate failures; corrected scope passes8/8, source5142c4b. Two Lua plus three reports promoted; readability371/syntax0 with counters unchanged. Offline initial bundle37 Lua/42 hashes at trader-deterministic-owner-candidate-20261005, DLL8f3d92c5; superseded for future runtime use by cold-registry correction. No full transitive comment-helper ownership proof;1152 complete retail helper-copy traces retained separately.

Cold registry: first probe unintentionally initialized registry after allocation0 diagnostic before testing first publication; retained as rejected fixture, no cold-allocation proof claimed. Corrected first-publication probe starts allocation1 and exits3 under old metadata (rejected_cold_registry_first_publication). Unpublished owner dtor now skips Registry unless successfully registered; 64 cases in each fresh process starting registry-object failure1 and map-head failure2 pass. Clean owned sourcec63bd05, parent5142c4b; one metadata header changed. New Win32 Release DLL build98584 terminal0. Unit source hash pin updated, Lua ownership operations unchanged. Regeneration82087 and focused compiled reruns58155/92914 pending; poll exact handles, do not restart. Next verify regenerated Lua/report identity, current input hashes and final compiled results, export actual patch/new uniquely named registry-safe bundle and final record. Existing normalized full29 corpus baseline preserved under corpus_final/validated with validated_corpus_comparison.json; current final inputs at final_inputs.json. No outer staging/commit.

## Registry-safe final offline checkpoint and runtime harness gap

Final reruns completed; registry-safe bundle has 37 parsed Lua files and 42
verified manifest hashes. final_validation.json pins current reports. Runtime
host-discovery checklist and runtime_plan.md preserve the missing LUAGameflow
host/front-end/observer gates. No launch or input while user is at computer.
Next: explicit-path test driver and existing helper local-copy recovery.

Explicit-path runtime_discovery.py prepared offline: list-only live mode, no launch/input; seven parser checks pass. Archived v15_run1 host list valid but run errors retained as failure; October4 startup has no host reply. Current candidate remains visually untested. See runtime_plan.md.

Helper ownership boundary audit: 1152 original executions, 58 distinct engine-boundary reference profiles. Results/queries/effects match retained native oracle exactly. Current Lua speaker/fallback aliases confirmed; generic native assignment remains required before helper lowering. Evidence: codex_lua_trader_comment_ownership_20261005/boundary_ownership_requirements.json. No production changes or game launch.

Helper ownership boundary audit: 1152 original executions, 58 distinct engine-boundary reference profiles. Results/queries/effects match retained native oracle exactly. Current Lua speaker/fallback aliases confirmed; generic native assignment remains required before helper lowering. Evidence: codex_lua_trader_comment_ownership_20261005/boundary_ownership_requirements.json. No production changes or game launch.

User reauthorized visual launch. Fable2/xenia/Fable absent at preflight. Verified package launched PID3468, chained original/addon start_status1. Full save backup runs/20261005-153458-529099/save-backup. Captures confirm profile-selection frontend. Game live; escort save not yet loaded. Continue exact owned process; do not relaunch. active_visual_session.json records state. Assignment scratch candidate separately prepared, not built/promoted.

Visual test failed at accepted-save load: owned PID3468 exited, Windows Application1000 exception c0000005 in Fable RVA005e5c63 (VA009e5c63). Frontend reached LoadGame, then native crash before hosts. Archived log and unconsumed discovery batch in visual_load_failure_20261005. Staged save restored; full backup comparison changed files: []. No quest/sound proof. Investigate native fault before retrying.

Crash dump preserved and parsed offline. Fault is read0x11 with ECX/ESI0 at009E5C63. Direct stack return009E25AF matches native call009E25AA in CPersistTraitsSLONG::TransferIn009E2590, receiver from persistence context [EBP+4]. Null persistence symbol-map receiver; upstream cause unproven, no helper execution established. dump_evidence.json records raw context/modules/stack candidates and distinction from full unwind. Next inspect parser/context and controlled baseline load.

Persistence dump inspection confirms context009E2590 receiver at0017D640 has PSymbolMap slot+4 zero. Saved caller parser0017D5A4 position41; input pointer114158B8 not captured, so token/content is unknown. Raw context/parser words preserved in persistence_evidence.json. Do not infer malformed save or sidecar root cause. Next controlled baseline save load with same profile and retained save restoration.

Baseline retail retry deferred: Fable2.exe PID2780 is running; left untouched. Offline assignment scratch candidate passes24 actual SDK/sol ownership-only scope cases (different/self/nil/invalid/retired/other-scope destination, deterministic retirement and GC safety), native calls doubled. Full sidecar build/native binding proof/helper lowering remain outstanding. Existing packaged candidate unchanged. See codex_lua_trader_helper_assignment_20261005/assignment_scope_validation.json.

Assignment binding native-byte fixture passes24 SDK/sol cases with original008AB980 bytes and relocated CRT free call; positive counts only, copy/destructor doubled, zero-count path separately covered by12 Unicorn cases. Full candidate sidecar build live exact session56462, last LuaQuestState.cpp; poll same session, do not restart. Fable2 PID2780 observed running, no new launch. Helper lowering still outstanding.

Assignment full sidecar build56462 terminal0. assignment_candidate.json records DLL hash; not packaged/installed. Helper call-site witness records3 direct assignments,1 directcopy,1 emptyconstructor,17 temporarydestructors; initial copy and fallback assignment are inline atE0191C..2E/E019A0..D6 and must also be lowered. Fallback native release-old/retain-new order agrees with008AB980 tests. Existing retail crash remains unresolved.

Scratch helper lowering prepared through existing generic restore with10 pinned operations; Lua parse passes after moving ownership tokens before goto scope. Source/dependencies/sidecar hashes retained. Initial speaker copy, fallback assignment/temp retirement, nearest/furthest tracking, empty assignment, native-style swap, trigger retirement before pause. No registry/root/generated promotion. Next ownership/effect comparison1152, nil output/local semantics, both styles/scopes, then full29. See helper_lowering_status.json.

Scratch draft helper1152 result/effect/final-reference-balance comparisons pass against retained complete native executions using Lua ownership/GSI/actor recording doubles. Per-boundary timing not yet compared; no promotion. Artifacts helper_effect_balance_comparison.json and compare_helper_ownership.py. Next native event-linked snapshots, nil-output cases, SDK callback forms, readable/master integration and full29.

Observable-event ownership gate found type1 response retained through timer despite native early cleanup. Rejected comparison preserved. Added native labelE02355 cleanup before timer (11 scratch operations); all1152 result/effect/finalbalance/event-reference comparisons now pass. Lua recording doubles vs original native event snapshots, not production SDK execution. Non-event boundaries, nil outputs, styles/scopes and full29 remain.

Empty lookup gate:216 original nearest/furthest/both-empty cases,192 native-returning effect/balance comparisons pass.24 genuine native faults retained: type2 empty nearest dereferenced directly atE01AA0. Lua candidate avoids native fault, explicit unresolved parity gap; not counted as passes. Query-local nil fallback wrapper preserves scoped destination assignments.1152 normal observable-event gate still passes. Fable2 absent at latest process check; baseline launch remains authorized and next runtime action when rechecked.

Termination ownership gate1152 cases passes 1886 native/Lua reference snapshots atCB7940, plus observable events/results/effects/finalbalance. Recording Lua model remains boundary double; SDK generated-helper execution pending. Fable2 PID14560 active prevented baseline launch.

Complete scratch draft helper now passes144 actual SDK/sol userdata cases with exact extracted scope and original assignment bytes. Actor/query/copy/destructor/GSI boundaries doubled; refcounts1 before and after GC. Cases3kinds*6modes*8terminationpoints, CONTROLLED/exists=true/timer0 only. No effect or engine equivalence claim from fixture. sdk_helper_validation.json. Fable2 PID14560 remains active; no retail launch. Broader styles/scopes and native fault parity remain pending.

Full scratch helper SDK/sol fixture passes1152 cases, native return/effect traces and reference balance before/after GC. Original assignment bytes, exact scope; copy/destructor/GSI/actor queries doubled. Build warning shared owned intermediate dir fixed in driver for future runs; previous144 artifact retained. No production promotion; native empty-nearest fault gap/styles/master/full29/retail load crash remain.

Scratch helper/master draft/readable forms pass4608 native result/effect/finalbalance comparisons, using production readable transform with converter generated regex. Four forms derived from scratch lowering, not full unit generation; signature difference checked. helper_forms_validation.json pins sources/mappings. SDK readable/allform event timing and full29 integration remain. Fable2 PID14560 active; no launch.

Baseline visual save load PID18696: no injected candidate and no FSE/Novi modules observed, same native exceptionc0000005 at009E5C63. Dump preserved baseline_load_failure_20261005, staged save restored, installed DLL hashes unchanged. Failure occurs independently of candidate; shared save/install path unresolved. Readable SDK helper1152 result/effect/refbalance cases pass separately. No retail quest/sound proof yet.

Baseline dump comparison confirms identical full x86 exception register context to candidate (null receiver, read0x11), no FSE/Novi in baseline module list. Read-only accepted-save zlib inventory: AutoSave/qs identical SHA876222c7, valid outer header/payload streams; semantic correctness not proved. Preserved sections and comparison artifacts. No save edits/install changes.

Shared crash call-site trace identifies START_SCRIPT_DATA/END_SCRIPT_DATA extraction and persistence dispatchCB7CB0, field literalCoreQuestWaiting. Native bytes match candidate stack return words; exact token/source remains unknown. Nested read-only save scan12 streams/318956bytes finds no literal matches, not proof field absent. script_persistence_fault_path.json records positive/negative evidence. No install/save edits.

Diagnostic baseline under read-only debugger session99714/PID3656 terminal0, staged save restored. Live fault text captured: CoreQuestWaiting FALSE; position41. CPersistTraitsSLONG interprets FALSE as symbolic string and dereferences null PSymbolMap. live_persistence_fault.json records exact text. No injection/save/install modifications. Next locate source START_SCRIPT_DATA text (quest initialization config) and substantiate numeric replacement before any repair.

Initializer origin inspection: no CoreQuestWaiting bytes in450 loose config/binary files, current WAD and two retained WAD baselines, or CompiledDefs. Read-only archive hashes/extractions in gameflow_initializer_inspection. Existing registered Gameflow unit confirms retail offset0x48 CoreQuestWaiting Int. Live FALSE is incompatible symbolic text for this integer/null-map transfer; origin likely packed/serialized path but not established. No repair applied; next trace parent script-data source or save deserialization rather than guess asset replacement.

Root input located using existing LZO QUESTS decoder: accepted save Gameflow block contains PostSavePosition550 and CoreQuestWaiting FALSE. Scratch-only single replacement to0 preserves header/quest binary trailer/othersections, checksums pass, original hash876222c7 unchanged. Candidate hash35ec0409. Owned diagnostic profilecodex_trader_integer_repair_20261005 staged and normal baseline launchedPID14120 after game-exclusion check. Live startup, not yet loaded. integer_repair_visual_session.json has staged backup; continue exact process and restore after terminal. No installed changes.

Repaired-save normal retail baseline PID14120 loaded into actual Lookout Point gameplay (repair_world_verified.png), whereas original save crashed at009E5C63. Scratch repair changes only persisted CoreQuestWaiting FALSE to integer0. Owned process closed, staged profile restored, installed DLL hashes unchanged. This establishes save-load repair, not candidate Lua/Trader sound proof. Latest user explicitly authorizes visual launches subject to Fable2/xenia exclusion. Next: candidate launch with repaired diagnostic profile and actual live host discovery.

Repaired candidate PID18856: real gameplay loaded; own host list has TraderEscort only (no errors). Quest active, PSP550, boast9 confirmed. Retail transition into Darkwood18 succeeded. Natural introduction played with trader subtitles/camera/animations; no skip. Quest-start and four tutorial pages dismissed visually. After scene flags false/false, two traders alive, quest active. Natural W3.5s walk advanced hero and frames; no heal/enemy clear/follower teleport. Intro query first timed out behind tutorial, same retained batch later replied; not a quest failure. Live process and staged backup in repaired_candidate_visual_session.json. Continue this owned session, do not relaunch; full escort/combat/sound still unproven.

First repaired candidate natural escort closed: PID18856 progressed on real WASD input through injured-trader scene (RMB no) and region exit into Darkwood Marshes; natural balverine scene completed. Speech log queued banter/response keys while frames advanced. Low-health hero died/resurrected; SCARED trader MsgIsKilledBy true in native log. No heal, enemy clearing, follower teleport or scene skip. Combat query timed out behind resurrection tutorial, preserved owned batch if still present. Run incomplete, no full quest/audio proof, combat cause unisolated. Archived first_natural_escort_runtime, exact owned process stopped, staged profile restored, installed DLL hashes unchanged. Next: controlled combat validation with explicit assistance grading, and generic helper ownership integration still gated by 24 empty-nearest native fault cases.

Master scratch helper compiled SDK/sol validation added: draft1152 and readable1152 result/effect/refcounts pre/post-GC passed, both owned MSBuild/run sessions terminal0. Combined helper/master x draft/readable4608 actual SDK cases pinned in helper_assignment/sdk_four_forms_validation.json. Four derived scratch forms do not establish full-unit generation or production engine/fiber behavior; copy/destructor/query/GSI boundaries remain doubles. Readable helper report inherited draft wording; consolidated evidence identifies actual readable source hash. Sidecar/helper not registered/promoted/packaged. Remaining24 type2 empty-nearest native faults retain explicit parity gap; root integration requires real exported identity/source anchors/full29.

Controlled retail PID19020 closed/profile restored/DLL hashes unchanged. Health API15/20 topped to20/20; one balverine hit20 still caused resurrection. Hero reposition/direct Marshes transition assisted; traders lookup0 versus quest count2, so sound target setup incomplete. Live SurpriseBalverine CREATURE_BALVERINE_EASY hp25 confirmed. Native SetThingAsKilled returned but live1/needed1 unchanged after unfreeze and delayed observation; not cleanup success. Subsequent health/fade command unconsumed after focus loss, archived/removed only owned batch; desktop capture excluded. Session log sliced against exact launch previous-log prefix; old BALVERINE_SURPRISE cue must not be attributed to current run. Runtime evidence in controlled_probe_runtime. User called out fighting: acknowledged wrong prior weapon input (bow) and slow automation; next natural run requires visually verified melee/block/target handling, not repeated scripted substitute. Generic helper registration still unpromoted.

User correction supersedes earlier bow explanation: user observed no bow equipped; assistant stood idle getting hit. Prior screenshot inference was unreliable and wrong; combat-driving failure cannot be attributed to equipped bow. Before another natural combat test, require visible melee stance/target/block and continuous input instead of leaving hero idle between long tool calls.
Helper scratch export gate now reads real typed NScript::CQ_TraderEscortScript::MakeTraderComment E01900..E02389, no placeholder export hash. Added22 native ownership call anchors plus initial inlined-copy bytes (23total). Positive lowering unchanged against prior compiled helper raw-file hash; six altered export/extent/native body/anchor/sidecar/intermediate probes rejected. Initial comparison mixed LF semantic hash with CRLF raw-file hash; corrected to like-for-like, final probe terminal0. Root generator/unit registration/output unchanged; full29 not rerun without shared changes. Evidence helper_assignment/helper_evidence_gate_validation.json; full generation and24 type2 empty-nearest native fault cases remain outstanding.

Helper fallback inlined assignment E01997..E019D8 reviewed: same-Info jumpE019A6, oldreleaseE019AC, deleteE019BA/freeE019C2, Data/Info storesE019CC/E019D0, retainE019D6, query destructorE019DC. Pinned24 ownership anchors; six negative evidence gates still pass, compiled helper file unchanged. Captured actual MakeTraderComment function at existing converter hook during full registered Trader baseline generation (owned process1156/session17919 terminal0):65functions/16files parse, no missing functions. All16 emitted files match prior caller_registry_final baseline bytewise. Prepared master/shared per-function rows at real generation boundary; candidate bodies exactly match already SDK-tested derived forms. Found concrete integration gap: shared helper emitter lifts separately and bypasses ownership hook. Root unchanged; candidate full generation/full29 and24 native empty-nearest fault cases remain open. Artifacts helper_assignment/function_scope_rows.json and function_scope_row_validation.json.

Combat controls correction: official Microsoft PC manual lists Q melee draw, Space target lock, left mouse attack, middle mouse block/roll; right mouse is flourish or bow first-person targeting. Earlier RMB-block assumption was wrong. User observation of no equipped bow stands; idle automation failure remains the recorded cause of inadequate fighting. Verify actual configured bindings and visible action before another combat run.

Full Trader Escort scratch candidate generated with generic shared ownership hook in an owned converter copy: worker83834 terminal0; 65 functions/16 Lua files parse, zero missing functions. Exactly quest master and shared helper change; other14 files byte-identical to baseline. Both emitted function bodies match previously compiled SDK-tested forms. Root converter and registry unchanged. First attempt used unpadded address keys and correctly failed changed-file assertion; corrected to actual 0x00e01900. Evidence helper_assignment/full_unit_candidate_validation.json. Readable full-unit generation, actual-output behavioral checks, generic root/full29 validation and24 native empty-nearest fault cases remain; no promotion claim.

Full-unit readable scratch baseline/candidate build with explicit draft inputs completed worker2782 terminal0:16 files syntax pass, no draft fallback; only quest/shared helper differ,14 files unchanged. Actual generated master/shared x draft/readable pass4608 native result/effect/final-refbalance comparisons (recording ownership/engine doubles). Initial readable-master extraction omitted lexical SCRIPT_DEF and failed; harness corrected to preserve original file prefix, no generated code changes. Reports full_unit_readable_validation.json and generated_forms_validation.json. Real retail sound/completion, actual-output SDK timing and generic root/full29 promotion gates remain;24 empty-nearest native faults remain explicit.

Revalidated all24 original empty-nearest fault cases with per-instruction operand snapshots: E01A98 loads nearest Data[esp+30] intoECX=0; E01AA0 mov eax,[ecx] faults before E01AA3 concrete equality dispatch[vt+138], no observable events. Earlier attribution to GetDataString is superseded. Lua wrapper IsEqualTo(empty) false is a real boundary mismatch, not missing string handling. native_empty_nearest_fault_trace.json pins24cases/bytes; code-hook instrumentation terminal0 (initial exception-in-memory-hook did not preserve snapshot, replaced with preinstruction recording). No generated/registered/root source changes; explicit failure semantics/native concrete equality ABI need review before promotion.

Strengthened original helper oracle with retail CGameScriptThing equality8D46A0 and intelligent-pointer getterA01B50 bytes, modeled valid actor identity controls:216 empty cases now40faults,16 newly exposed (type2/furthestempty atA01B53 reading otherData+10). Prior actor equality double hid these;192-returning comparison superseded for16cases,176native-returning cases remain to recompare. No changes to generated/helper production code, no promotion. helper_lowering_status.json corrected; native_concrete_equality_validation.json preserves allcases/nativecodehashes. Next strengthen nonempty/event ownership oracle with concrete equality and explicitly recover failure boundary, rather than silently accept nil-safe success.

Concrete equality strengthening revalidated1152 ordinary original helper cases: results/events/termination-query counts unchanged, native handle refs balanced. Recompared176 native-returning empty cases with Lua ownership model:0 failures.40 fault cases remain separately unresolved; no narrowed completion claim. Reports native_concrete_ordinary_cases.json, native_concrete_empty_cases.json, helper_concrete_empty_comparison.json; status updated. Sidecar IsEqualTo binding currently handles missing Data with pointer comparison (bothemptytrue), whereas model emptyfalse/native direct dispatchfault differ; next recover distinct direct-implementation equality ABI/failure boundary and actualSDK empty operands before changing lowering.

Actual production IsEqualTo lambda extracted from candidate NoviUnitBindings.h and compiled with SDK/sol:worker75642 terminal0,10casespass (raw/shared userdata, same/different, empty/nil/scalar). Both-emptyuserdata=true, nilargument=false, exactly3 nonempty concrete dispatches (double). Earlier helper SDK/model emptyfalse fixture differs; evidence remains fixture-scoped, not full production equality proof. Source/exe pins sdk_equality_binding_validation.json. No sidecar/root/generated modifications. Native direct call40faultcases need distinct evidence-pinned failure representation rather than global wrapper behavior change.

Original equality8D46A0/getterA01B50 bytes executed isolated Win32 probe with2internalcallrelocations, synthetic thiscall dispatch thunk, modeled live identitycontrols:25operandpairs,16returning/9SEH-observed realaccessviolations. Nullreceiver reads0; nullargument withvalidreceiver reads0x10. Distinct implementation objects sharing actoridentity compareequal; identitycontrol refcounts unchanged. Report win32_native_equality_validation.json native/exe hashes. Probe terminal0; no game/install/sidecar changes. Establishes actual native ABI outcomes, not Lua-error/retail-AV equivalence;40fullhelperfaultcases still block faithful promotion.

Scratch generic DirectThingImplementationEquality.h prepared: SDK12-byte wrappers supply Data operands, direct concrete-vtable IsEqualTo invocation without nil-safe substitution. Isolated candidate probe matches all25 original native operand outcomes including9SEH-observed AVs atsame readaddresses;5 actualsol nonempty/alias/unset-identity casespass with original equality/getterbytes. direct_equality_candidate_validation.json pinsheader/exe. Candidate remains standalone scratch, not bound in sidecar or emitted by lowering; fault-through-Lua/production host behavior and40fullhelpercases unresolved. Original wrapper binding unchanged. Next integrate separate direct operation into owned sidecar/helper evidence and actualfullunit SDK validation, retaining nativefaults explicitly.

Owned scratch sidecar integrated separate RetailImplementationIsEqualTo binding/header; original wrapper IsEqualTo unchanged. Two native directdispatchsites E01A98/E02195 evidence-pinned in master/shared direct_function_scope_rows.json with native equality/getter dependencies and newbinding/header hashes. Lowering restore gates pass both forms; newoutputs notbehaviorallyvalidated/fullgenerated yet. Sidecar build owned session59799 verifiedlive atlink/codegeneration; await samehandle, notterminal. No install/rootregistry promotion. Earlierhelper validation applies oldforms only.

Direct equality sidecar build session59799 terminal0, DLL SHA967e7dc3b31a5be8523253f31b886b4d06ba1b73e9535354b4b1f0e1cad34a99. Full scratch generation session77601 terminal0:65functions/16files parse,14unchanged, newmaster/shared matchevidence rows. Actual generated sharedhelper recording model1368cases:1328native-returning results/effects/refbalance pass,40native-fault paths reach explicit modeldiagnostic beforeevents (no invented success). Modeldiagnostic/cleanup not retailAV semantics; production directbinding intentionally retainsAV, actualSDK fullhelper/faultthroughhost/readable/full29 gates remain. Reports direct_equality_sidecar_integration.json/direct_full_unit_candidate_validation.json/direct_generated_helper_model_validation.json. Notpackaged/installed/promoted; no game launched.

Direct candidate complete readable pass worker5376 terminal0:16files parse, no draftfallback, bothdirectcalls preserved/master/shared;14unrelatedfiles unchanged. Actual readable master/shared each1368 recordingmodel cases (1328returning effects/results/refbalance,40explicit faultdiagnostic paths) pass. Reports direct_readable_validation.json/direct_readable_forms_validation.json. Initial master harness replacement hit quoted marker insidefixture setup and failed syntax; anchored standalone results line, no generated changes. Candidate integration status updated trueusesoperation. Next actual fullgeneratedSDK with original concrete equality/getterbytes, production faultthroughLua/rootfull29/soundruntime unresolved. No installation or promotion.

Actual generated direct draft sharedhelper SDK/sol1152case comparison passes with original assignment/equality8D46A0/getterA01B50 bytes; modeled valid actoridentitycontrols and copy/destructor/query/string/GSI boundaries. Results/events and wrapper/identity refs pre/postGC checked. Initial buildworker49458 terminal0 emitted MSB8028 sharedintermediatedir warning; fixed projectIntDir owned obj_sdk_direct_full, rebuiltworker79023 terminal0, warningabsent. sdk_direct_full_validation.json pinsactualhelper/exe/source. Other3generatedforms and40empty/fault-through-Lua cases outstanding; no runtime/promotion claim.

Actual generated remaining3forms nativeSDK probe worker74686 terminal0: sharedreadable/masterdraft/masterreadable1152each result/event/refbalance checks pass, original assignment/equality/getter executed; modeledactoridentitycontrols/copy/destructor/GSI. All4forms4608cases now pinned sdk_direct_four_forms_validation.json (actualgenerated/extractedhelper/exeSHA); inherited reportwording corrected byconsolidatedformmapping. Isolated builddirs noMSB8028. Nonemptyoutputs only;40nativefault and176emptyreturning SDK paths/productionhost/full29/runtime sound still open. No install/promotion.


Actual generated empty-query SDK completion (continued session): all four draft/readable master/shared forms pass176 native-returning cases each,704 total, original assignment/equality/getter with actual SDK/sol and modeled engine/copy/destructor boundaries. Existing shared-draft176 report discovered and retained; remaining forms freshly compiled/run by probe_sdk_direct_empty_forms.py (session80347 terminal0). All40 native-fault cases per form also pass fresh-process first-chance AV checks,160 total: read0 or read0x10 matches original oracle, helper entered and zero prior observable events. probe_sdk_direct_faults.py session67382 terminal0; handler records then exits process, never resumes damaged Lua/C++ stack. No post-fault cleanup/recovery or production-host equivalence claimed. Reports sdk_direct_empty_four_forms_validation.json and sdk_direct_fault_four_forms_validation.json in helper_assignment. Starting fresh full29 before/after baseline under codex_lua_trader_helper_integration_20261005; before session53693 live, no completion claimed. Root converter/unit untouched so far; owned dirty snapshots preserved with base366c08e. Next generic shared-helper integration and evidence/adverse/full29 checks; no installation/game launch/promotion.


Actual generated event-time SDK ownership proof: probe_sdk_direct_event_refs.py session95946 terminal0,1152 cases per four forms/4608 total and3662 event snapshots per form/14648 total. Each observable event count matches original helper ownership oracle, followed by final and forced-GC balance; original assignment/concrete equality/getter execute, engine/copy/destructor boundaries remain modeled. sdk_direct_event_four_forms_validation.json pins results. Candidate sidecar source audit checks66 unchanged vendor files, both evidence rows, tested headers and exact assignment method, DLL967e7dc3 hash unchanged. Owned sidecar committed161c39c parentc63bd05 (six files/30 insertions); actual committed cumulative and incremental patches exported under helper_integration, sidecar_build.json. No root promotion/install/game launch. Fresh full29 baseline53693 remains live after completing1831-file input capture; trader/arena regeneration in progress.


Generic shared-helper ownership integration in progress: current1831 input hashes equal prior deterministic-owner final_inputs exactly; redundant fresh-before workers9488/14724/1416 stopped, completed normalized full29/534-Lua baseline reverified by reuse_baseline.py/session63239 terminal0. Root converter now calls existing evidence-pinned Thing-output restore for shared helpers, existing Trader unit adds master/shared MakeTraderComment rows; four durable lowering fixtures and positive/adverse tests added. Focused38 tests session44281 and related90 session80251 terminal0. Root Trader generation completed within full29 after session99526 (still live). Three affected outputs byte-match SDK-tested candidate; readable master differs only by eight native-address header comments, strict normalization verified in tested_output_identity.json (initial byte-only assertion rejected and diff retained). Smoke before/after findings identical: draft2/readable1, existing Assassin modulo-zero mock and draft DarkwoodTrader freeglobal; no unknown bindings. All root promoted Lua still untouched pending full29 completion/review. Source changes and baseline captured under helper_integration.


## MakeTraderComment ownership and direct equality promoted

Completed offline acceptance in `work/codex_lua_trader_helper_integration_20261005/offline_validation.json`. Generic shared-helper hook reuses native/source/operand/sidecar-pinned ownership restore; existing registered MakeTraderComment master/shared rows recover local copies, assignments, exact retirement and two direct implementation equality dispatches. No new script registration. Focused38/adverse and related90 tests pass. Full29 after session99526 terminal0; audit72692 terminal0:534 Lua parse, only four intended Trader master/shared draft/readable files differ; unrelated units byte-identical. SDK executable outputs match; readable master has eight native-address comments supplied by full conversion report (initial raw-byte rejection retained). Returning actual SDK5312 cases (4608 ordinary/event and704 empty),14648 event-ref snapshots and160 fresh-process native AV observations pass; native assignment/equality/getter execute with modeled engine/copy/destructor boundaries. Fault probes exit at first chance, no cleanup/recovery/production-host proof claimed. Smoke draft2/readable1 findings exactly unchanged, no unknown bindings. Four Lua plus three reports promoted with exact hashes; ledger371/syntax0 refreshed once.

Clean sidecar161c39c parentc63bd05, DLL967e7dc3 and committed cumulative patchb885fe7f verified;66 vendor files unchanged. New offline `trader-helper-ownership-candidate-20261005` package37 Lua/42 manifest hashes passes (session97667 terminal0). Outer workspace not committed. Candidate is now running for authorized visual validation: PID14652, staged save backup `scratchpad/save_backup_1234234_20261005-175300`, session state `repaired_candidate_visual_session.json`. Game exclusion passed, injected bundle used without replacing installed DLLs. Startup logo visually observed; frontend driver89388 live, save/world/helper runtime not yet verified. Restore staged profile/save and verify installed DLL hashes when closing. Next natural combat requires visibly verified Q melee, Space target and MMB block; earlier idle-input/RMB mistake remains rejected evidence.


Latest user steering: skip visual testing for now; user wants their screen. Ask before any future game launch or screen/input use. Continue Lua OFFLINE under the original ongoing request. Prior launch authorization is superseded. PID14652 had already exited before cleanup (normal entity/quest teardown is present in log); no relaunch. close_runtime.py completed staged save/Profile.bin restoration and verified all installed DLL hashes unchanged. Archive focused_runtime/validation.json and current_session.log. Four controlled master helper calls returnedtrue: type1 banter, nil-speaker fallback, type2 nonempty equality, and type2 swap. Natural shared helper FOLLOWING and banter/response queues also observed. ForcedGC then reports15hp,quest counter2 and allthree actors alive,quest incomplete; no Lua/autopilot errors in current session. This establishes selected live host paths only, not native refcount balance, audible playback, combat or fullquest equivalence. Initial close-script identity query failed because process had already exited; corrected cleanup handles terminal process and restored successfully. Offline bundle remains valid.


## Offline void return-register store recovery in validation

User reaffirmed continue Lua until asked to stop; screen/game launches remain paused and require asking first. Owned work/codex_lua_void_register_store_20261005 baseline matches prior helper integration1832 inputs. Added generic reviewedVoidRegisterStores pass after scalar operand recovery: independently verifies source/native/PDB void prototype, exact intermediate/output hashes, local integer EAX residue, native producer call and pop-only terminal epilogue (EAX pop/read/branch rejected). Trader SetBrainState E0A80D destructor falls directly through pops to RET4; recovered sole iVar4=extraout_EAX_00 store, no native cleanup/effects removed. Focused26 plus related77 tests terminal0. Both emitted styles504 before/after comparisons pass with poisoned old register and new undefined-register read trap; original native helper504 cases match mutating effects/state/termination eachstyle. Comparison initially missed raw stack float3.0 normalization, rejected scratch retained; corrected explicit IEEE bits normalize3.0 and1.0 only. Smoke draft2->1/readable1->1, sole removed finding is DarkwoodTrader freeglobal; Assassin mock failure unchanged. Full29 session71824 is still running; outputs not promoted.

Independent Assassin diagnosis:96 synthetic vector-shape smoke scenarios, all72 nonempty cases pass; six empty cases exit before selection and18 fail modulozero as expected. Original retail selection block E02EC8..E02F03 independently executes24 Unicorn cases with modeled rand:18 nonempty selected pointers exact,6 empty vectors trap x86 divideerror atE02EED. No invented empty-vector fallback; no full Main/host/cleanup equivalence claim. Reports assassin_scenario_smoke.json and assassin_native_vector.json. No game launch, input, installed files or save modifications.


Void-register recovery complete: full29 worker71824 terminal0,534 Lua parse and only one DarkwoodTrader draft line removed; all other Lua byte-identical. Exact generated draft plus two reports promoted.103 tests,504 native cases and504 emitted comparisons/style pass; current draft/readable smoke each retain only known Assassin fixture modulozero. New offline trader-void-register-candidate-20261005 package37 Lua/42 manifest hashes, same sidecar161c39c/DLL967e7dc3; no install or launch. Readability inventory unchanged because every readable Lua byte is unchanged. Reports in work/codex_lua_void_register_store_20261005.

Next offline issue found by actual SDK termination-boundary probe: shared draft1152 passes, but shared readable drops termination queries at early dead-speaker/timer-positive exits. Initial unhandled C++ assertion fastfailed; diagnostic catch identifies missing snapshot count (kind0/scared/timer2). An initial builder-only fix in scratch still fails because readable_style.normalize_alive and simplify_conditions also delete unused scheduler queries. Both rejected outputs retained. Now allthree rewrites preserve bare query calls while discarding only unused values;50 tests and21 subtests pass, full readable29 worker11040 running with independently hash-verified unchanged completed draft baseline. Root generated readable output not promoted yet; latest candidate must pass actual SDK snapshots1886/form. No game or screen use.


Termination-query fix actual SDK gate complete: probe_sdk_retained_termination_refs.py worker94855 terminal0, four actual generated master/shared draft/readable forms1152cases each and1886 termination-boundary ref snapshots each, total4608cases/7544snapshots. Results/events and pre/post-GC native wrapper/identity reference counts agree with original oracle. Original assignment/equality/getter execute; copy/destructor/engine/termination responses remain modeled, no production host proof. Reports sdk_retained_termination_four_forms_validation.json in helper_assignment and copied sdk_termination_validation.json in termination_queries. Both failed predecessors retained; final root readable builder/style fix has not yet been promoted while full29 worker11040 runs.


## Termination queries retained through readable generation

Completed work/codex_lua_termination_queries_20261005/completion.json. Generic build_readable_unit.drop_unused_alive and readable_style.normalize_alive/simplify_conditions preserve scheduler calls when their result is unused. No special quest pattern. Full29 readable worker11040 terminal0 with hash-verified unchanged draft generation/evidence reused;534 Lua parse.28 changed readable files/13 existing units retain49 query sites, all draft and remaining Lua byte-identical. Reviewed all diffs; structural differences retain early-exit cleanup instead of routing exits through a now-observable final query.50 tests+21 subtests pass. Actual four generated SDK/sol helper forms4608cases/7544 termination reference snapshots match original native oracle with modeled engine/copy/destructor boundaries. No production host/lifetime claim.

All13 affected-unit before/after mock findings, complete nontermination call traces and frame counts agree. Initial scratch collector removed Lua40-call limit but missed Python12-call truncation; WillWhisper false mismatch exposed it. Rejected truncated reports preserved, collector corrected at both layers, source/driver-hashed rerun terminal0.20 focused WillWhisper normal/termination/animation scenarios also preserve exactly one acquired-resource cleanup (zero before acquisition). Promotion first refused GuildTraining hand-reviewed/readable path before any output writes; switched to existing readable_dir resolver and preserved that authored tree, promoting generated readable_converter instead.28 Lua plus13 reports promoted with exact hashes. Readability ledger refreshed371/syntax0; counts6213generic/4611gotos/6357native/4649machine/970unresolved. New focused offline trader-termination-queries-candidate-20261005 package37Lua/42manifest hashes passes, sidecar161c39c/DLL967e7dc3 unchanged. No game, screen input, installation or save changes. Continue offline per user; ask before future launch.


## Helper lookup and Win32 fiber ownership probes complete

Owned work/codex_lua_trader_helper_fibers_20261005/completion.json records terminal probes. Actual SDK/sol allfour generated helper forms4608cases and4768 reference snapshots at nearest/furthest/script-name output lookups match original native oracle (helper_assignment/probe_sdk_lookup_boundary_refs.py worker68153 terminal0). This excludes intentionally hoisted pure hero/state getters and is not a complete every-instruction boundary claim.

Actual C++ WithRetailResources protected callback now also tested on real standalone Win32 fibers, not a substituted Lua scope callback. Fourforms4608cases with432 suspensions atPause; forced LuaGC while C++/Lua stacks are parked retains exact actor/identity counts, resumption matches native Pause/query snapshots/events and final/pre-postGC balances (first56393 and remaining86835 terminal0). A separate432 controlled C++ errors after resumedPause exercise actual protected callback error closure in allfour forms (worker89804 terminal0): exact oracle event prefix throughPause, no subsequent events, native refs balanced before/afterGC. Model scheduler/API/copy/destructor boundaries remain explicit; these probes do not instantiate production quest/entity hosts, region teardown or delete suspended fibers.

Documented stable generic ownership/assignment/direct-equality contracts in docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md. Exact committed cumulative patch exported to tools/script_recovery/sidecar_patches/novi-scoped-thing-outputs-direct-equality.patch, SHA b885fe7f8cd9b36d08d74ddcf1c5fe276fd16ab907d6d56893032909e392dc70, base07774e98 ->161c39c4. Source clean, exported bytes match prior tested patch, reverse git apply check succeeds. Includes prerequisite reviewed camera/position/create changes, not arbitrary-upstream standalone patch. No source or DLL rebuild was needed for scratch probes; generated Lua remains the validated termination-query candidate. No game launch/screen use/install/save changes. User still requests continued offline Lua work until stopped.


## Literal return jumps recovered inside resource callbacks

Completed work/codex_lua_literal_return_jumps_20261005/completion.json. Generic fold_nested_return_jumps now handles bare/nil/true/false returns, keeps non-tail Lua returns in do blocks, rejects variables/calls/strings/multiple values and duplicate labels/named closure bodies. Actual token inspection also prevents string literals blanked by structural analysis from being mistaken for bare returns.73 tests+88 subtests pass; six full-readable-pipeline string/cleanup cases reproduce three prior early-return value-loss cases across a loop and preserve value/arity after correction. Initial straight guard fixture was optimized before the faulty pass and did not reproduce it; retained rejected fixture, switched to cross-loop jump.

Full29 readable worker42422 terminal0 with unchanged draft/evidence hash reuse,534 parse; only master/shared TraderEscort readable helpers change, each16->11 gotos. Scope-independent false returns expose safe local sinking; native calls/cleanup remain. Actual fourforms SDK4608 result/event/termination comparisons and7544 refs pass (worker95612). Current readable forms also pass216 normal Win32 fiber suspensions and216 controlled resumedPause errors, exact Pause/lookup refs and parked/finalGC balances (worker50150); engine/scheduler/copy/destructor doubles, no production-host teardown claim. Full Trader smoke findings, nontermination traces and frames unchanged. Two Lua plus report promoted by exact hashes. Offline trader-literal-return-candidate-20261005 package37 Lua/42 manifest hashes, same sidecar161c39c/DLL967e7dc3. Ledger371/syntax0,6213generic/4601gotos/6341native/4649machine/970unresolved.

Initial baseline snapshot copy overlapped the first edit after hash verification. Guard rejected the copied hash; recovered original function bytes from the prior owned snapshot and required the exact completed-corpus source hash before proceeding. Rejected copy and baseline_recovery.json retained. No acceptance uses the raced copy. No game, screen/input, install or save changes. Continue offline Lua work until user stops; future launch requires asking.


Next offline readability pass in work/codex_lua_dead_bitmask_staging_20261005: generic prune_dead_bitmask_staging removes only a uniquely declared unused local initialized from uint32 literal followed by closed AND-mask guards. No outside reference/capture/global store, unsafe expression, label or diagnostic is removed.81 tests+21 subtests pass. Completed literal-return baseline inputs and exact source snapshot verified before editing. Full readable29 worker74870 running, draft generator/evidence unchanged; native SDK/fiber checks and promotion remain pending. Latest packaged/promoted artifact remains trader-literal-return-candidate-20261005. No game/screen/install/save actions.


Dead-bitmask pass checkpoint: final88 tests+21 subtests terminal0; scope/string/_ENV adverse cases added. Full29 readable worker79282 running against stable final inputs. Previous completed74870 was rejected for scope gaps,75121/51283 explicitly stopped for later guards; rejected copies retained, no promotion yet. Fourforms SDK4608/7544 refs worker31238 and four readable Win32 fiber probes worker63609 completed; verify_helper_reuse.py confirms rebuilt final Trader helper hashes exactly match all probes. Full Trader smoke unchanged, including known Assassin empty-vector failure. Latest accepted package remains literal-return candidate. Record_complete.py is prepared but must run only after final corpus audit, smoke, hash-gated promotion, packaging and ledger audit. Next candidate scratch inventory/prototype:13 unreachable jumps after unconditional do-return blocks;8 return/cleanup/error and7 adverse cases pass; next work/codex_lua_unreachable_return_jumps_20261005/prepare.py requires this pass completion before taking its baseline. Continue offline, user screen reserved, ask before launching.


## Dead private integer-mask staging removed

Completed work/codex_lua_dead_bitmask_staging_20261005/completion.json. Generic prune_dead_bitmask_staging removes a uint32 local initialization and contiguous closed AND-mask guards only when the local has no outside use. Lexical block tokens prove the declaration still encloses the assignment; whole-chunk tokenization rejects mask-shaped string/comment text. The special _ENV local is excluded because it has implicit global-lookup uses. Captures, live values, globals, unsafe operands, diagnostics, labels and ambiguous declarations remain intact.88 tests+21 subtests pass (worker21151 terminal0).

Full29 readable worker79282 completed with534 parsed Lua and unchanged draft/evidence hash reuse. Only master/shared TraderEscort readable helper files change: each loses the unused scratchValue6 declaration and13-line integer-mask chain. No call, cleanup, query, label or goto changes. Full Trader smoke findings, nontermination call traces and frame counts remain identical, including the already traced empty Assassin vector modulo-zero failure.

Actual SDK fourforms4608 cases/7544 termination-reference snapshots pass (worker31238), and current readable helpers pass216 normal plus216 controlled resumed-Pause-error Win32 fiber suspension/parked-GC checks (worker63609). Generated-file hashes prove these completed probes used exactly the final regenerated helper bytes. Engine/scheduler/copy/destructor boundaries are modeled; the ownership-only C++ scope fixture is not the complete production resource class or quest/entity host teardown. Two Lua files and readable report promoted by exact hashes; trader-dead-bitmask-candidate-20261005 package37 Lua/42 manifest hashes, same sidecar161c39c/DLL967e7dc3.

Initial full corpus74870 rejected when late/closed local declarations exposed a global-store bug. Subsequent scratch rebuilds75121 and51283 stopped before completion for multiline-string and _ENV guards; outputs and reports retained in explicitly rejected directories. One sibling-branch test fixture initially inserted twice through broad string replacement; corrected to one insertion before acceptance. Reusing the original after directory was rejected by its existing-destination guard before writes; archived it within the verified task boundary and restarted fresh. No rejected/incomplete run supports acceptance.

Next concrete candidate: thirteen unreachable goto sites directly after unconditional do-return blocks, across BS_Teacher and the two Trader helper forms. Scratch next_jump_prototype.json passes eight return/cleanup/error cases and seven conservative exclusions; it is not integrated or promoted. Continue offline Lua work until user stops; ask before any future game launch. No game/screen/input/install/save changes.

Current ledger summary: {"counts": {"genericTemporaries": 6189, "gotos": 4601, "machineTemporaries": 4649, "nativeNames": 6341, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.


Unreachable-return-jump active checkpoint: work/codex_lua_unreachable_return_jumps_20261005 baseline is completed dead-bitmask candidate.113 tests+21 subtests terminal0 worker14734. New prune_unreachable_return_jumps is now final style cleanup; initial iterative corpus20180 stopped/preserved because it enabled unrelated Teacher frame/query folds. Focused final Teacher preflight proves nine goto-line deletions only. Full29 final worker69605 running; no generator input edits while live. Final Trader generated hashes equal completed actual SDK4608/7544 refs worker59500 and readable Win32 fiber432 suspensions including216 controlled errors worker58069. Both current units' complete smoke traces including termination queries, frame counts and findings unchanged. Pending final corpus audit, verify_exact_diff.py (exact13 deletions across3 files), smoke, gated promotion, two-unit offline package, ledger and record_complete.py. Nothing from this pass promoted yet; latest accepted package is trader-dead-bitmask-candidate-20261005. Next generated-label scratch inventory/prototype has eight behavior/exclusion cases, not integrated. User wants continued offline Lua work; no screen/input/game/install/save actions; ask before launch.


## Unreachable jumps after literal returns removed

Completed work/codex_lua_unreachable_return_jumps_20261005/completion.json. Generic prune_unreachable_return_jumps removes only an immediate same-indent goto after a bare/nil/true/false do-return block. Whole-chunk tokens reject string/comment lookalikes; intervening labels, diagnostics and arbitrary expressions remain. Runs as the last style cleanup so dead-edge removal cannot trigger unrelated executable folds.113 tests+21 subtests pass (worker14734), including return arity, Lua close handlers and their errors, alternate label entry and the complete callback pipeline.

Initial placement inside the repeated optimization loop enabled Teacher frame/query and temporary rewrites. Worker20180 was stopped before completion; rejected_iterative_candidate retains its outputs and smoke. Moved the rule to final cleanup, and focused Teacher regeneration proved exactly nine jump deletions before restarting the full corpus. Worker69605 completed all29 readable units,534 Lua parse, verified unchanged drafts/evidence reuse. Exact diff gate proves only13 goto-line deletions:9 BS_Teacher,2 shared Trader helper,2 master Trader helper. No other source line changes. Full smoke findings and complete API traces (including termination queries) plus frame counts unchanged for both units; Teacher has no smoke findings, Trader retains its known empty Assassin-vector modulo-zero case.

Actual SDK fourforms4608 cases/7544 termination reference snapshots pass (worker59500); current readable forms pass216 normal and216 controlled resumed-Pause-error Win32 fiber suspension/parked-GC checks (worker58069). Helper hashes match final regeneration exactly, permitting reuse after the pass placement change. Ownership-only C++ scope fixture and modeled engine/scheduler/copy/destructor limits remain; not production host teardown. Three Lua files and two reports promoted by hashes. Offline two-unit trader-book-unreachable-return-candidate-20261005 bundle includes TraderEscort and BookCollecting with the existing Oakvale prerequisite, same sidecar161c39c/DLL967e7dc3; syntax and every manifest hash verified. No installation, game launch, screen/input or save actions.

Next concrete investigation: unreferenced generated LAB_/FLOW_ labels remaining after return recovery; next_cleanup_inventory.json is discovery only and has not changed generated files. Continue offline Lua work until stopped; ask before any future launch.

Package: {"bundle": "work/codex_lua_unreachable_return_jumps_20261005/trader-book-unreachable-return-candidate-20261005", "game_tested": false, "installed": false, "overrides": ["Q_NewOakValeIntro", "Q_TraderEscort", "V_BookCollecting"], "parsed_lua": 40, "sidecar_baseline_commit": "07774e98c5675fb66aba7c23b4f1e4453b0434c0", "sidecar_dll_sha256": "967e7dc3b31a5be8523253f31b886b4d06ba1b73e9535354b4b1f0e1cad34a99", "sidecar_patch_sha256": "b885fe7f8cd9b36d08d74ddcf1c5fe276fd16ab907d6d56893032909e392dc70", "sidecar_source_commit": "161c39c4ad167ed23b63704b46afe60e8dd63c35", "units": ["trader_escort", "book_collecting"], "verified_manifest_hashes": 45}.
Ledger: {"counts": {"genericTemporaries": 6189, "gotos": 4588, "machineTemporaries": 4649, "nativeNames": 6328, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.


## Unreferenced generated labels removed

Completed work/codex_lua_unused_generated_labels_20261005/completion.json. Generic prune_unused_generated_labels scans complete file tokens at the end of each style_source round; no function-chunk boundary can hide a jump. Removes only standalone LAB_/FLOW_ labels with no referenced name anywhere, conservatively preserving duplicate names across scopes. User labels, live jumps including comment-separated operands, multiline text and inline diagnostic comments remain.127 tests+21 subtests pass (worker62184), including close-handler behavior and callback traces.

Full29 readable worker10648 terminal0,534 Lua parse, unchanged drafts/evidence hash reuse. Fifteen unused labels removed across9 files/7 units: TraderEscort2files, BanditCampBossBattle, BS_Teacher, ChickenMaster, Global_OpenChest, GuildTrainingPreMelee/TheRealGuildmaster, GuildTrainingMelee/TheRealGuildmaster, STS_BriarRose. The preliminary inventory predates the accepted jump pass; refreshing against the actual baseline adds Teacher's now-unused FLOW label, yielding15. Exact diff initially rejected one extra BanditCamp goto deletion. Removing unused labels exposes a goto immediately after do-return; the previously tested unreachable-jump rule removes it on the second style round. Independent line-index inventory reconstruction plus that single exact bare-return/goto sequence reproduces every final byte; label_interaction_validation.json and revised exact_diff_validation.json accept only those15 labels plus1 unreachable jump. No call, query, assignment, return or diagnostic changed. Seven-unit complete smoke traces including termination queries, frames and findings identical.

Actual SDK fourforms4608 cases/7544 termination reference snapshots pass (worker83683), readable Win32 fiber216 normal plus216 controlled resumed-Pause-error/parked-GC checks pass (worker9932). Exact helper hashes pinned; ownership-only C++ scope fixture and modeled engine/scheduler/copy/destructor limits remain. Nine Lua and seven reports promoted by exact hashes. All affected units packaged offline as unused-generated-labels-candidate-20261005 with existing Oakvale prerequisite:123 parsed Lua/128 manifest hashes, same sidecar161c39c/DLL967e7dc3. Initial package staging rejected a registry-versus-conversion-report ordering difference for Expressions. Verified identical members/no missing or extra scripts, retained staging, and compared sorted membership while preserving actual generated order in the report. No bundle existed at rejection; final bundle independently checked.

Ledger371/syntax0,6189generic/4587gotos/6312native/4649machine/970unresolved. No game launch/screen/input/install/save changes. Next scratch work/codex_lua_conditional_join_20261005 traces retail E01AA9..E01AFB: unequal actors skip query/assignment, equal+terminating cleans up and returns false, equal+alive assigns empty handle before join. Prototype simplifies the corresponding two-jump condition and matches36 truthiness/close-handler cases; not integrated. Continue offline until user stops, ask before future launch.


Conditional-join active checkpoint: work/codex_lua_conditional_join_20261005 baseline is completed unused-label candidate. Generic fold_literal_exit_join integrated late in style_source;207 tests+21 subtests terminal0 worker53558. Full29 worker84982 still running against stable root inputs; most units complete, no root edits while live. Actual SDK4608/7544 refs worker45688, current readable fibers432 suspensions/216 controlled errors worker17508, and additional current readable352 returning empty cases+80 fresh-process native AV boundaries worker76192 all terminal0. Trader complete smoke traces/findings/frames unchanged. Exact visible helper diff matches native E01AA9..E01AFB branch (native_join_window.json). Pending terminal full corpus then audit_outputs.py, verify_join.py, smoke.py, promote.py, package.py, audit_readability, record_complete.py. Latest promoted/package remains unused-generated-labels-candidate-20261005. No game, screen, install or save actions; continue offline until user stops and ask before future launch.


## Native-backed two-jump conditional join recovered

Completed work/codex_lua_conditional_join_20261005/completion.json. Generic fold_literal_exit_join handles exactly a closed two-jump forward join with a literal exit, straight-line prefix/body and no alternate entries/nested control. It evaluates each condition once, preserves prefix scope and LIFO local close order, and moves only bare/nil/boolean returns. Whole-file tokens reject text/diagnostic lookalikes and duplicate or externally reached labels.207 tests+21 subtests pass (worker53558):36 truthiness cases including Lua zero/empty-string truthiness, literal return arity, condition/call errors, close-handler errors, shadowed close locals and complete callback execution.

Retail helper body SHA pinned in native_join_window.json; E01AA9..E01AFB proves unequal actors skip the termination query and assignment, equal+terminating closes owned temporaries and returns false, equal+alive assigns an empty handle before continuing. Exact generated diff gate reproduces the complete output using only this reviewed conditional replacement in master/shared MakeTraderComment: each9->7 helper gotos, two labels total removed, calls/arguments/order outside the branch unchanged. Full29 readable worker84982 terminal0,534 Lua parse, all unrelated and draft Lua byte-identical with unchanged evidence hash reuse. Full Trader smoke findings and complete API traces/frame counts unchanged, including known native-empty Assassin-vector fault.

Actual SDK fourforms4608 cases/7544 termination reference snapshots pass (worker45688); current readable Win32 fibers216 normal plus216 controlled resumed-Pause-error/parked-GC checks pass (worker17508). Additionally both current readable helpers pass352 returning empty-query cases with exact native results/events and pre/postGC references, and80 fresh-process first-chance native AV cases with expected0/0x10 read addresses and zero prior events (worker76192). Fault observer exits immediately; this establishes the failure boundary, not recovery or post-fault cleanup. Ownership-only resource fixture and modeled engine/scheduler/copy/destructor limits remain; no production host teardown or full quest completion claim.

Two Lua and readable report promoted by exact hashes; focused trader-conditional-join-candidate-20261005 package37 Lua/42 manifest hashes verified with unchanged sidecar161c39c/DLL967e7dc3. No game/screen/input/install/save changes. Continue offline Lua work until user stops; ask before any future launch. Next candidate is the kind1 reply-availability flag join (remaining three jumps), requiring native/control and cleanup-order evidence before a generic fold.

Ledger: {"counts": {"genericTemporaries": 6189, "gotos": 4583, "machineTemporaries": 4649, "nativeNames": 6306, "unresolvedDiagnostics": 970}, "files": 371, "statuses": {"needs-human-review": 156, "needs-readability-work": 156, "needs-recovery": 59}, "syntaxFailures": 0}.


## User night stop and tomorrow authorization

Stopped owned flag-join corpus worker70702 (terminal1) at user request; partial output preserved in work/codex_lua_conditional_flag_join_20261005/paused_after_20261005, not accepted. Root generic flag-join source and new tests remain unpromoted;390 tests+21 subtests pass. Four candidate files across TraderEscort, GuildTraining and SickChild require complete corpus/diff/behavior review. W7 conditional-join completion/package remains latest accepted. Source snapshots, hashes, partial inventory and steering saved in night_stop.json and night_source_snapshot.

Authoritative next-session instructions: [Lua recovery handoff](../../scripts/LUA_RECOVERY_HANDOFF.md). User authorizes in-game testing tomorrow while at work; do not launch tonight, work hours are unspecified, no automatic launch scheduled. Earlier continue-until-stopped and launch-permission notes are superseded within this window. No game/screen/input/install/save actions during offline continuation. Native lane handoff preserved with a separate Lua link.
