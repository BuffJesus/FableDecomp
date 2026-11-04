# Lua recovery resumed - 2026-10-01

User requested continuous Lua work and authorized game testing. Goal remains
active. Journal: `docs/journal/2026-10/LUA_RECOVERY_2026-10-01.md`.
Preserve the dirty baseline and unrelated native lane; no commits this session.

v100 fixes duplicate movie acquisition in Madame and BordelloLady. Assisted
Bordello introduction now completes and exits both scene modes with no logged
Lua errors. v101 restores the initial idle hit flag. Live decline replay
`bordello_v101_decline_20261001_4` completed the introduction, reached the offer
and declined response, but timed out after another offer. All 561 original
save files restored unchanged. Use `adult_bandit_camp_completed_2026-09-26`.

v102 recovers native question arguments: empty third button, explicit true,
and correct free-path Yes operand. Seven focused and 264 broader tests pass;
full 29-unit A/B changes only Lady. Readable paid/free probes pass; smoke JSON
identical (five existing diagnostics). Corpus 371 files, zero syntax failures.
Package 210 parsed Lua files, 216 matching hashes, unchanged DLLs. Offline
only; v102 does not fix the automatic conversation restart.

Current candidate: `work/codex_lua_lady_return_20261001`. Separate reviewed
rule restores the native post-conversation unpause, copied-tail hit gate, and
backedge to interaction polling. Native E40074, E401D3, E4030A/E3EC80 checked.
Thirteen focused and 277 broader tests pass. Full 29-unit A/B changes only
Lady. Twenty generated behavior cases pass with identical draft/readable
traces; smoke JSON unchanged. Promoted and packaged as v103: 371 corpus files
parse, 210 package Lua files parse, 216 hashes match, DLLs unchanged.
Live test `bordello_v103_decline_20261001_5` passed: introduction 75 seconds,
paid decline 62 seconds, exactly one question with an empty third button,
normal unpause and both scene modes ended. No logged errors. All 561 original
save files restored unchanged. One extra ESC skipped the opening film after
visual inspection; map travel and proximity targeting were assisted.
v104 payment recovery is promoted and packaged: -GoldRequired replaces zero.
Ten focused/287 broader tests, all 29-unit A/B (only Lady changed), 24 generated
payment cases, unchanged smoke JSON. 371-file corpus and 210-file package
parse; 216 manifest hashes match; DLLs unchanged. Offline only.

Current candidate: `work/codex_lua_lady_cutscene_map_20261001`, restoring the
paid `$SEXTALK` map key. Thirteen focused and 300 broader tests pass; native
free-path scene selection is always QUICKIE, including first visits. Full
29-unit A/B changes only Lady. Sixteen generated setup cases pass; smoke JSON
unchanged. Promoted/package v105: 371 corpus files parse, 210 package Lua files
parse, 216 matching hashes, unchanged DLLs. Live paid replay
`bordello_v105_paid_20261001_6` hit Grope instead of Lady and was interrupted.
Tool Ctrl-C bypassed finally; the game closed, the run was archived, and the
exact backup manually restored. All 561 original save files verified unchanged.
Runner now has optional talkTarget predicate and talkData selector; 26 runner
tests pass. Run 7 safely rejected wrong targets; run 8 timed out in frontend.
All original save hashes restored after each. Run 9 now uses deterministic
Polly selection plus native IsEqualTo before TAB.

v106 is promoted/package-ready: generic low-byte C character normalization
restores integer 4 in the costume predicate. Sixteen focused tests, 583 broader
tests/140 subtests, full 29-unit A/B (only two Bordello files), 132 generated
costume cases pass. Smoke diagnostics drop 5 -> 3 only by removing x04 free
globals; function traces identical. Unicorn exception reports reproduced
equally in passing baseline/candidate native-oracle tests. Corpus 371 files,
package 210 Lua/216 hashes valid, unchanged DLLs. Scratch:
`work/codex_lua_char_bytes_20261001`. No live costume proof.

Live `bordello_v106_paid_identity_20261001_9` completed introduction and paid
scene, with visible 10120 -> 10070 debit and control returned. Runner failed
after a redundant targeting retry; counter/HadSex not verified. Partial live
evidence only. Game closed and all 561 original saves verified unchanged.
Runner now starts completion timeout after blocking setup/targeting; regression
fails on baseline's second TAB and passes with fix. All 27 runner tests pass.
Run 10 passed introduction but rejected every target approach; no paid offer.
Game closed and all 561 original save files unchanged. Live targeting deferred.
v107 promoted/packaged: native concat constructor-result recovery fixes Guard
and Madame attacked dialogue prefixes. Eight focused/367 broader tests and
10 subtests pass; 64 generated Main cases pass, including healing/idle silence.
Full 29-unit A/B: Bordello plus one proven ParentClass prefix in unresolved
BeardyBaldy worker setup; that unrelated output stays in scratch. BookCollecting
unchanged. Smoke 3 -> 2, all function traces identical. Corpus 371 parses;
package 210 Lua/216 hashes valid, DLLs unchanged. Offline only, not launched.
Scratch: `work/codex_lua_bordello_prefix_20261001` (ab2 accepted, ab rejected).
v108 promoted/packaged: generic retained self-copy folding restores BeardyBaldy
WatchForAttack with exact entity argument. Five focused/335 broader tests,
31 broader subtests, full 29-unit A/B (only BeardyBaldy entity), 27 generated
behavior cases pass. Seeded smoke sole change is one CreateThread call; zero
problems. Corpus 371 parses; package 210 Lua/216 hashes valid.
New generic aggressive-spell event binding: native slot 0xAC, nine x86 probe
cases and Win32 Release build pass. Isolated sidecar commit 22305659ac60d2cac3e57c47d978478756e28b85;
sidecar SHA 538c47bbad2b58ba5713a4b697237286cc78828ab9cd8c5ae710c804d3a9c0fa.
Stock DLL unchanged. Source patch exported from committed diff and apply-checked.
Scratch `work/codex_lua_beardy_worker_20261001`; live config beardy_attack.json
prepared for isolated-save travel/one real melee hit/read-only flag observer.
v108 run11: real watcher registered with one argument; native binding idle-false
check passed. Positive hit unproven (test observer environment error/missed swing).
Revised run12 stopped at obstructed frontend; another Fable2 session is active.
Owned game closed and all561 original save files restored unchanged.
v109 promoted/packaged: BeardyBaldy native CRT speech RNG via existing binding.
Four focused tests/5 subtests,138 generated cases,zero smoke problems; full traces
match except added native RNG calls. Full29-unit A/B changes four units; only
BeardyBaldy promoted. Arena/BanditCamp/TourGuide random deltas remain scratch-only
with native instruction evidence. Corpus371 parses; package210 Lua/216 hashes.
Both DLLs unchanged fromv108; no v109 launch. Scratch codex_lua_beardy_random_20261001.
Broad baseline/candidate each296 passed/10 subtests, same existing wife animation
witness failure and order-dependent BookTrader fixture setup error. Next: resolve
those test evidence gaps and validate remaining random deltas offline.
Readability isolation accepted: explicit converter naming profile; full29-unit
readable A/B zero changes;66 tests/283 subtests. Reviewed exact text-key inventory
permits current bank hash alongside original; ten wife tests pass. Legacy
NewOakvale A/B changes only wife draft,51 functions/18 files compile; scratch only.
Final broader selection301 tests/12 subtests all pass; previous failures resolved.
v110 promoted/packaged: remaining five native RNG sites validated163840 inputs;
Arena/BanditCamp/TourGuide outputs promoted. Full smoke traces unchanged apart
from native RNG calls; TourGuide's one pre-existing missing unload binding remains.
Only BanditCamp is already bundled: v110 updates its crowd Lua,210 Lua/216 hashes
valid, both DLLs unchanged. Arena/TourGuide remain outside bundle. No live launch.
v111 promoted/packaged: generic region-unload binding and TourGuide boolean
result recovery. Native slot54->893360;97 tests/35 subtests,8 x86 binding cases,
Win32 build,20 candidate watcher cases pass (baseline fails4 unload scenarios).
Full29-unit A/B changes TourGuide root and Beggar entity; only TourGuide promoted.
Smoke1->0 problems; corpus371 parses. TourGuide remains outside bundle; v111
updates only DLL/README/STAGING,210 Lua/216 hashes valid. No runtime proof.
Sidecar commit906a5fc39bd82d5208b5ff88b47125925413adb5, SHA
7331252ec13a5babe4a56730eb6a7798fe953ddea2464db7fca589f1621ee54a.
Beggar receiver fix now promoted offline: typed-pointer vtable spelling uses
existing native CFG receiver proof. Four reply-wait queries restored to entity.
104 tests/35 subtests,64 generated candidate cases pass; baseline fails16.
Full29-unit A/B only LookoutPointBeggar, smoke unchanged3 files/1 known Bully
free-global problem. Corpus371 parses. No new bundle (Beggar absent from v111).
BeggarBully high-byte flag recovery now promoted offline: six predicates,
571 tests/34 subtests,120 generated finite/infinite cases pass; baseline fails42.
Full29-unit A/B only BeggarBully; smoke1->0, exact free-global diagnostic removal.
Corpus371 parses; no new bundle/game. Native NaN equivalence remains unresolved.
v112 ready offline: readable relational negation preserves NaN/metamethod semantics.
363 tests/551 subtests and173 focused cases pass. Full29-unit A/B56 files/18 units;
all deltas finite comparison complements, all18 unit smoke traces identical.
Preserved GuildTraining separately reviewed slice; promoted40 files/17 units.
Corpus371 parses. Bundle210 Lua/216 hashes:22 Lua+metadata updated,8 absent paths
excluded,10 older bundle paths deferred (STAGING.json). Both DLLs unchanged from
v111, no launch/runtime proof. Scratch codex_lua_float_negation_20261001.
Native ordered-health fix promoted offline: all9 BeggarBully gates reject NaN.
18 focused tests,423 broader/644 subtests,198 generated cases pass. Full29-unit
native A/B only BeggarBully; smoke3 files0problems identical; corpus371 parses.
Standard CLI readable rebuilt after guard caught prototype report-format mismatch;
original backups preserved. No new package (Beggar absent), v112 remains current.
Scratch codex_lua_ordered_health_20261001. Resource aliases/third-byte flag unresolved.
Equality-negation token fix accepted:416 tests/551 subtests, focused238/8 pass.
Full29-unit readable A/B zero Lua deltas; no promotion/new package. v112 remains.
Scratch codex_lua_negation_tokens_20261001/accepted.json pins tested source.
BeggarBully first-friend-reply flag now promoted: reviewed scalar-alias row with
body/source hashes and12 native instruction anchors.17 focused tests;48 generated
steps pass vs17/24 failures per baseline; smoke3files0problems identical. Unit A/B
only Bully, corpus371 parses; readable TODO3->1. No new bundle/game; v112 current.
Scratch codex_lua_beggar_lifetimes_20261001. Resource aliases/quest-card operand/
final counter were unresolved at that checkpoint.
Quest-card recovery now promoted: native getter RET4/card RET12 proves bool0
belongs to outer call and fixes card/quest argument order in both beggar scripts.
Generic guarded native_quest_card_operands.py;581 tests/674 subtests pass;12
actual generated call/state cases pass vs0 baseline.29-unit A/B only two intended
files; smoke3files0problems equal;371 corpus parses. No new bundle/game, v112 current.
Scratch codex_lua_beggar_card_20261001/promoted.json.
Active scratch codex_lua_beggar_resources_20261001: retained resource atESP1C is
conflated with separate GetThing outputESP2C;9 getters and reset/destructor all
prove same resource. Prototype5 startup/cancel cases pass vs4 baseline failures.
Resource fix now promoted via generic reviewed resource lifetime lowering:
636 tests/1593 subtests pass,12 lifecycle and18 getter probes pass; full29-unit
A/B only BeggarBully; smoke3files0problems equal;371 corpus parses. No bundle/game.
Active scratch codex_lua_beggar_counter_20261001: separate native ESP58 belch
counter from ESP43 initial-reply flag and movie. Draft/readable prototype built;
Counter now promoted:451 tests/644 subtests,8 switch and16 flag probes pass;
full29-unit A/B only Bully;smoke unchanged;371 corpus parses;Bully TODO1->0.
No package/game action. Active scratch codex_lua_lookout_resources_20261001:
companion resource entry-210 has13 direct getters and one expanded prepare guard.
Companion resource now promoted:642 tests/1593 subtests,14 lifecycle and28
getter/guard cases pass;full29-unit A/B only LookoutPointBeggar;smoke equal;371 parse.
No package/game. Active scratch codex_lua_lookout_expression_20261001: recover
alternate (void *)*value string-buffer spelling in fold_name_compare. Native
load/Compare calls checked;14 generated branch cases pass;baseline4 focused fails.
Expression fix now promoted:770 unique tests covered across broader/targeted
runs,644subtests.17 old fixture failures exactly reproduced on baseline;fixed
upvalue discovery in2 tests,57 targeted passes.29-unit A/B only LookoutPointBeggar,
14 branch probes,unchanged smoke,371 parse. Details expression/test_validation.json.
Lookout first-reply scalar row also promoted:74 durable tests,384 generated steps,
5 evidence guards pass;unit-only A/B only companion,smoke unchanged,371 parse.
No new bundle/game;v112 current. Lookout response counter now also promoted:
reviewed ESP40 scalar,84 durable tests,40 wrap/sequence cases;unit-only delta,
smoke unchanged,371 parse. Scratch codex_lua_lookout_counter_20261001.
Raw CRT fallback promoted:418 tests/33 subtests,32/32 readable compiled equality,
14-unit paired draft/readable smokes match;29 files per stage/13 units promoted.
GuildTraining excluded. Corpus371 parse. v113 now current:14 Lua payload updates,
210 parsed/216 hashes, unchanged stock and sidecar DLLs; offline only. Four older
bundle payloads deferred,11 absent; see STAGING.json. Scratch codex_lua_raw_random_20261001.
Beggar conversation operands promoted:818 tests/1007 subtests;52 final focused
checks; full29-unit A/B only LookoutPointBeggar, final guard29-unit zero deltas.
512 actor/cancel cases and4 facing checks pass; resource probes/smoke unchanged.
Corpus371 parse. No bundle/game action; v113 current. Scratch
codex_lua_lookout_conversation_20261001 retains rejected broad EDX regression.
Beggar initial pushability promoted from codex_lua_beggar_pushable_20261001:
37 focused and451 broader tests/35 subtests pass;26 lifecycle cases pass.
Full29-unit A/B changes only both Beggar entities. Corpus371 parse. Smoke3/0,
only expected extra call per Main. No bundle/game action;v113 current.
Partial native RNG completion promoted:438 tests/33 subtests;full29 A/B only
Arena(5 draws),CROWDBANDITS(14),TourGuideGuide(1). Paired draft/readable smokes
match;3/3 compiled equality;131072 conditional generated cases,371 corpus parse.
v114 current:only CROWDBANDITS payload updated,210 Lua/216 hashes,DLLs unchanged;
Arena/TourGuide absent. Offline only. Scratch codex_lua_partial_random_20261001.
Lookout cleanup groups promoted:24 focused,432 broader tests/35 subtests;
full29 A/B only Lookout,6 draft TODOs removed;371 corpus parse. Readable local/
non-yield guard simplifications validated with1536 equal full-function traces;
512 retain existing nil movie cleanup failures. Scratch codex_lua_lookout_cleanup_20261001.
Lookout movie receivers now promoted:52 focused,473 broader/35 subtests;
full29 A/B only Lookout;paired smokes3/0 unchanged,371 corpus parse.18432 actual
Main scenarios pass;baseline3260 fail.512 extra completed-speech polls reviewed.
Scratch codex_lua_lookout_movies_20261001 preserves rejected late-lift approach.
Bully movie receivers also promoted:60 focused,481 broader/35 subtests;
full29 A/B only Bully,paired smokes3/0 unchanged,371 corpus parse.27648 actual
Main scenarios pass;baseline4872 fail;all other22776 traces equal. Scratch
codex_lua_bully_movies_20261001. v114 unchanged (Beggar absent).
Bully cached taunt listener promoted:9 focused,490 broader/35 subtests;
full29 A/B only Bully,27648 actual Main cases pass (4608 listener-only deltas),
paired smoke3/0 identical,371 corpus parse. codex_lua_bully_listener_20261001.
Parent tutorial condition matches native; no change. Gate1GuardOuter hero
listeners promoted:20 focused,510 broader/35 subtests,full29 A/B only guard;
3888 behavior cases pass,paired smoke19/0,371 corpus parse. v115 staged:only
guard Lua changed,210 Lua/216 hashes,DLLs unchanged. Scratch
codex_lua_gate_guard_listener_20261001. Bandit defensive-guard list repair
promoted:31 focused,543 broader/35 subtests;full29 A/B only bossquest,320 actual
helper cases pass (40 baseline arithmetic failures),paired smoke19/0 identical,
371 corpus parse. Scratch codex_lua_bandit_guard_lists_20261001. Boss bundle
baseline differs;v116 not created,v115 remains current. D10897 loop-head
cancellation now promoted:21 focused,564 broader/35 subtests;320 cases pass,
18 expected cancellation deltas,final29 A/B only bossquest,paired smoke19/0,
371 corpus parse. Scratch codex_lua_bandit_loop_poll_20261001 keeps rejected
early C relocation (actor-map regression);accepted artifacts late_* and ab_final.
Typed-null map constructor now promoted (codex_lua_bandit_actor_map_20261001):
27 focused,591 broader/35 subtests;320 cases/80 nil-map repairs;29 A/B only boss,
paired smoke19/0,371 parse. HERO map stores now promoted in
codex_lua_bandit_health_threshold_20261001:21 focused,612 scoped tests/35 subtests;
864 intro/cancel replays,432 repairs;ab_final29 only boss;paired smoke19/0,371 parse.
Whole-directory Unicorn crash retained separately,not a passing suite. Health
threshold unchanged. Hostage string cleanup now promoted via unit-only evidence:
codex_lua_hostage_string_cleanup_20261001;41 anchors,12 evidence guard cases,
1296 identical helper traces,612 scoped tests/35 subtests,paired smoke19/0,371 parse.
Boss quest has zero unresolved diagnostics,not yet runtime-proven. Latest staged
bundle now v116:codex_lua_bandit_bundle_v116_20261001 reviewed exact older payload;
8496 old/new helper cases pass with native repairs,210 Lua/216 hashes,only boss
payload+notes change,both DLLs unchanged. Not installed,no game action.
Gate3Guard cancellation now promoted:codex_lua_gate3_cleanup_branch_20261001;
15 focused,627 scoped tests/35 subtests;1536 cases/96 repairs;29 A/B only guard;
smoke19/0 with expected+200 readable polls;371 parse. Older bundle guard reviewed
(192 cleanup-word crashes repaired);latest staged v117,only Gate3Guard+notes,
210Lua/216hashes,both DLLs unchanged,not installed. Other Fable2 PID6556 observed.
BanditForger pass-string cleanup now promoted (codex_lua_forger_string_cleanup_20261001):
32anchors,7guard cases;draft compiled equivalence after unused-local normalization,
25 actual readable predicate/call-order cases;627 scoped tests/35subtests;
paired smoke19/0,371parse. v117 unchanged;older Forger bundle review deferred.
Bandit kill-group loop reentry now promoted and v118 staged
(codex_lua_bandit_kill_groups_20261001):100anchors;723 scoped tests/43subtests;
3360 generated and v117 helper scenarios,1728 trace repairs/370 boast-state repairs;
accepted ab_final29 onlyroot,paired smoke19/0,371parse,210packageLua/216hashes.
Rejected ab/Bordello attempts preserved;final rule excludes nested loops and complex headers.
v119 now staged after capacity-initializer promotion (codex_lua_bandit_capacity_init_20261001):
17focused/740scoped/43subtests,29unit A/B onlyroot,paired smoke19/0,371parse.
3360helper cases preserve nonpolltrace/state;2272 omit one known-false poll per
nonempty query. Explicit normalization needed for bytecode proof;raw equality not claimed.
v119 includes Forger cleanup after exact bundle/baseline identity closes deferral;
210packageLua/216hashes,unchangedDLLs. Staged only. OthergameFable2PID6188.
Forger D0D3C8 already has correct generated timer/resource cleanup;no change.
CampHostageGuard movie cleanup now promoted and v120 staged
(codex_lua_hostage_guard_movies_20261001):131anchors,6restoredreleases,
21new/761scoped/43subtests;full29 onlyguard,pairedsmoke19/0,371parse.
5760Maincases repair1620leakingpaths;23040v119bundlecases repair6480,
all othercalls identical.210packageLua/216hashes,unchangedDLLs,stagedonly.
Broad candidate_draft is unpromoted;accepted reviewed_draft/reviewed_readable.
Gate3Guard movie cleanup now promoted/v121staged(codex_lua_gate3_movies_20261001):
unit-only row,80anchors,8evidencecases,64focusedtests;6144Maincases/240repaired,
1024leaksfixed,allothercallsidentical. v120baselineexact;unit-onlycomparison,
smoke19/0,371parse,210packageLua/216hashes,unchangedDLLs. Stagedonly.
User status question answered;continuous work remains authorized.
Assassin2/3 promoted, v122 staged (codex_lua_assassin23_movies_20261001).
Unit-only two rows,95anchors each,64focusedtests,7680Maincases/520repairs/
760leaksfixed;all other calls/state identical. Smoke19/0,371parse;
v121baselineexact,210packageLua/216hashes,unchangedDLLs. Offline only.
User asked in-game progress: at least10 completed chains/sections plus separate
BookCollecting checkpoint proof and partial Bordello; version/assistance caveats.
Assassin1 promoted/v123 staged (codex_lua_assassin1_movies_20261001):127anchors,
64focusedtests,131072Maincases/10048repairs/16576leaks;state/othercalls same.
v122 older actor had 3 nil event stubs: reviewed native calls plus separate
raw131072/26624changed and normalized comparison; no raw equivalence claimed.
Smoke19/0,371parse,210packageLua/216hashes,unchangedDLLs;offline only.
v123 live scratch codex_lua_assassin_live_v123_20261001: attempt2 passed gate1,
second guard, Forger pass, gate2; Assassin targeted but offer not reached. No
Assassin live pass. Attempt1 opening-movie timeout; owned guard distinguishes
24movie captures/10nonmovie fixtures. Attempt3 checkpoint Gate2Open false,
stopped at readiness guard. Attempt4 user closed game before frontend.
User subsequently made screen available again and explicitly authorized game
testing. Finish offline validation before next owned launch. All runners finished; all561originalsavefiles unchanged,
zero additions. Next offline Assassin interaction evidence investigation.
Continuous work and game testing remain authorized.
AssassinMarker promoted/v124 staged: reviewed native resource receivers,145anchors,
34focused/795scoped/43subtests,full29onlyMarker;6048Maincases/945repairs/
1620misdirectedspeeches corrected,oracle equivalence (not raw changed traces).
Actualv123 bundle same coverage;smoke19/0,371parse,210packageLua/216hashes.
Broad ab/ab_final superseded (Arena changed);accepted ab_reviewed.
v124 live attempt5 Assassin1 intro/decline passed (talked true,hire false,
scene/movie false,movie17 destroyed,no Lua errors,561 saves unchanged).
Route assisted;one No input after visible question. Perstep answer config fixed.
Paid attempt6 game path completed:4620->2620,Gate3Open true,scene/movie false,
all5moviesclosed. Two harness diagnostics retained (nil target;gold wrong VM);
independent status proves debit. Config corrected;561 saves unchanged.
Attack7 missed;attack8 completed44s butno speech logs. Attack9 completed48s,
16captures;001/006/011 show3attacklines/speakers/camera in sequence.
Last3acquisitions repeat earliercontrol handles (not pImp.Data identity);
all4moviesclosed,no errors,561saves
unchanged. Live Marker delivery now supported;close positioning/facing assisted.
Next ArenaCellDoorGuard resource ctor/early cleanup:owned scratch
codex_lua_arena_guard_resources_20261001,prototype2 and16anchorwitness,
newmodule integrated/promoted OFFLINE,41focused/821scoped/43subtests,full29onlyguard.
512Maincases native resource identity/early cleanup oracle;no conversation
coverage or raw equality. Smoke16 perstyle onlyexitRelease+1;readable no actual
errors,2pendingroundhelpers. Draft2preexistingfaults unchanged. Corpus371parse.
Arena absent v124 bundle;no Arena package/live claim. Nextguard lostspeech
operands and named receivers. No game active. Scratch
codex_lua_assassin_marker_receivers_20261001. Both DLLs unchanged.
v114 live Beardy retry13 aborted at frontend:other Fable2 session reappeared.
Only owned Fable closed;runner finally restored backup125356;561 originals match.
No positive hit proof. Avoid foreground tests while other session active.
No owned processes/game active;v115 staged,not installed.

ArenaCellDoorGuard speech operands and resource task receivers promoted OFFLINE.
Scratch codex_lua_arena_guard_speech_20261001: 28 speech calls,60 task queries,
2 command clears;379 native anchors. Exact native/source/lowered/helper gates,
lexical call pairing (not machine address order), CFG-reaching ECX/EDI proof.
Preserves native hero-return listener variable; named guard/self listeners and
all six speech operands recovered. Reviewed module replays the full witness
inventory before rewriting lowered C; generated Lua never hand-edited.
23 focused positive/adverse tests;844 broader tests/43subtests passed. Pytest
constructor emulation emitted Windows Unicorn access-violation diagnostics
although the first run ultimately passed; final fixture isolates unrelated
constructor defaults. Full29 A/B uses actual constructors, only Guard changed.
Complete22draft/23readable scratch identities verified. Actual generated Main
3072cases PER STYLE: all26distinct keys, exact native speaker/listener oracle,
waits reject wrong resource, pending command clear, dialogue/attack/early repeat
and cancellation. All tested resources/movies close. No raw baseline trace
equivalence or full retail behavior claim. Smoke16files perstyle JSON identical;
existing draft extraout_EAX/ArenaEnemy nil arithmetic and both styles' pending
InitialiseArenaRounds remain. Corpus371 parses,88/128/155statuses unchanged.
Arena still ABSENT from v124: no Arena gamepackage/live claim. User renewed
game authorization while showering. Next clean paid Assassin replay using the
fixed nil-safe target query and LUAGameflow final gold probe; attempt6 had two
harness diagnostics despite paid-path completion. Continuous work authorized.

v124 Assassin paid replay10 PASSED cleanly in game. Native game-written Gate2
checkpoint; assisted travel/proximity/targeting and real Yes input. Nil-safe
target query and LUAGameflow final gold probe fixes verified. Guard2/Forger/
Gate2 route repeated; Forger5620->4620, Assassin4620->2620 exactly2000. INTRO
and YES speech on Assassin1 verified by logged actor data. Gate3Open true,
TalkedToAssassin/Triggered true, movie/cutscene false; final finish step_done
and finished done=true/failed=false. Five movie starts/destroys match; ZERO
quest/harness errors or tolerated diagnostics. run10.py,runner10.log and
assassin_v124_paid_20261001_10_review.json retained. All561original saves
restored unchanged; no additions. Game closed. No Arena livepass added.
Next existing ArenaCellDoorGuard2 native constructor/cleanup prototype:
frame220,slot-200; constructor F19BC6,base destructor F19DE7,resource destructor
F1A793 incorrectly lowered as DestroyMovie. Scratch codex_lua_arena_guard2_
resources_20261001;30anchors,6acquisitions. Generic constructor module has
unpromoted extensions for native stack pushes and EBX zero reaching proof;
first guard26existing tests still pass. Guard2 row NOT registered/promoted.
Need adverse evidence/actual Main tests/full29 A/B/readable/smoke before promotion.
Arena InitialiseArenaRounds source binding IS present in the owned packaged
sidecar source. Smoke's pending label is hard-coded and its scanner omits the
included NoviArenaRounds header; it does not prove the deployed method absent.
Actual Arena round setup runtime proof remains outstanding; do not invent an
additional missing-binding fix from that mock diagnostic alone.

ArenaCellDoorGuard2 constructor/early+idle exit resource cleanup PROMOTED OFFLINE.
30native anchors,6acquisitions,frame220/slot-200. Generic reviewed constructor
extends native stack-push proof (2pushes before vtable vs firstguard1) and EBX
zero reaching definitions at base cleanup. Native resource destructor7E74D0
clears resource fields+8/+12 and tails99A430; F1A793 wrong DestroyMovie repaired.
51focused (25new+26firstguard) and869broader/43subtests passed. Full29 final A/B
only Guard2 changed; first A/B rejected because baseline module saw the newly
opted-in Guard2 row. Corrected before-mode deep-copies unit metadata and removes
ONLY that new row; complete22Lua baseline identity verified. Complete23readable
identity and onlyGuard2 output delta. Source/body/helper gates preserved.
Actual Main2304draft cases: trueframe/query termination; normalized storage+
cleanup oracle, allresourcesclose. Readable2304 cases use frame-return termination
semantics; predicate-query traces normalized, allresourcesclose. No raw trace
equivalence or crossstyle rawclaim. Initial constanttrue/query-counter readable
fixture exposed different query timing: cleanup-bearing initial loop retains
native F19D0A/F19D24 separately; replaced with physical-frame predicate model.
NewOakValeIntro lifetime always-true frames still require separate package
validation; do not infer compatibility from these frame-return cases alone.
Smoke16files perstyle JSONidentical; existing Arena diagnostics unchanged.
Corpus371parse/88recovery128readability155review unchanged. Arena NOT packaged
or game tested; currentgamebundle v124, clean Assassin paid replay10 done and
561original saves unchanged. No game/owned constructor jobs running.
Next scratch codex_lua_arena_guard2_speech_20261001: existing reviewed speech
module replays native4Speak/12Task/5Clear calls with70anchors, no module changes
needed. Scratchgenerated Main has four correctly rendered lines but existing
cancel exits still TODO/early returns. Need actualconversation/cancellation
tests and native cleanup repair before promotion; no Guard2 speech unitrow yet.

ArenaCellDoorGuard2 speech + shared cancellation tail PROMOTED OFFLINE.
Native4Speak/12Task/5Clear calls (70anchors) recovered by existing reviewed
speech module with no changes. New reviewed cleanup-break module proves native
F1A7ED movie destructor -> F1A7F2 actor resource LEA -> F1A7F6 resource destructor
-> pops/frame restore/RET (9anchors,220frame/-200resource), full native/source/
post-speech lowered/helper hashes. Changes nested C cleanup break to explicit
goto resource epilogue, enabling existing generic goto lowering; no Lua hand edits.
Initial speech-only scratch384cases had18cancel movie+resource leaks. Prototype2
fixes all384; final actualgenerated Main6912cases PER STYLE, all4keys/native
speaker-listener/flags, dead speaker, early/cham repeat, pending clears and
termination1..48 with0/1/3task polls. Alltested movies/resources close. Readable
fixture initially retained per-entity fields between cases; fixed setupvalue
reset (no generated code changes) before final all4-key coverage. No raw baseline
trace equivalence or complete retail/Arena game proof claimed.
23cleanup native/adverse tests;892broad/43subtests plus22additional Guard2speech
tests passed. Tail-boundary adverse initially exposed missing exact-end gate;
fixed before final focused/broad checks. Full29 final A/B onlyGuard2; before-mode
removes ONLY new speech/cleanup opt-in rows, keeps promoted constructor rows.
Complete22draft/23readable identities verified; smoke16files perstyle identical,
existing Arena findings retained. Registered Guard2 has no TODO cancellation
exits; corpus371parse,87recovery/129readability/155review (6diagnostics removed).
Arena absent v124; no newbundle/in-gameclaim. Last live proof is clean v124
Assassin paid replay10; all561savesunchanged and gameclosed. Noownedjobsrunning.
Next existing ArenaCellExitGuard Main F1B9C0 resource construction/exit cleanup
evidence: readable has NewResource assigned to resource, Prepare/TryAcquire use
nil meControl. Gather native frame/slot and destructor witnesses before a fix.

ArenaCellExitGuard resource storage + both base cleanup phases PROMOTED OFFLINE.
Scratch codex_lua_arena_exit_guard_resources_20261001;25native anchors. Slot-300,
initialframe312; saved EBP F1BAD3 creates316phase. Generic constructor recovery
supports reviewed additional base destructors with CFG dominance of stack save,
matching immediate pop after cleanup, perphase ECX/depth/vtable/zero proofs.
Native ctorF1B9D5,earlybaseF1BABF,latebaseF1C9A3; two acquisition sites. New
24phase tests (including real non-dominating push EBP F1C937 rejection), existing
51constructor tests pass; full938broad/43subtests pass. Full29 A/B onlyExitGuard;
before-mode removes ONLY new opt-in row, retains promoted guard1/2 rows. Exact
22draft/23readable baseline and tested output identities verified.
Actualdraft Main512cases: strictbaseline rejects nilresource all512; candidate
closesall, receiver+basecleanup normalized oracle;280missingcleanup repairs.
Readable512physical-frame cases compare body calls to recovereddraft, both
bool-return and always-true frame modes. Allclose;28baseline extra-acquisition
body paths repaired. Normalize predicate queries and unchanged leading Hero
read; NO raw equivalence. Initial ordinal-query readable fixture rejected
because preserved native queries alter synthetic counter timing; replaced
with frame-based predicate and recorded separately (not hidden as equal).
Smoke16files/style onlyExitGuard.Main countdelta. FULL traces prove draft
1423->1424 adds finalRelease; readable1422->1424 adds native idle termination
query at16 and finalRelease. Initial equality smoke assertions intentionally
failed; reviewed delta/proof accepted. Existing findings remain unchanged.
Corpus371parse,87recovery/129readability/155review,temporaries6330->6328.
No Arena package/live claim; v124 remains gamebundle. Last gamecleanAssassin
paid10 complete;561original saves unchanged, gameclosed. Noownedjobsrunning.
Next existing ExitGuard dialogue/resource helper operand recovery: native
frames312/316 and varied hero-return variables; inspect remaining speech/task
receivers and movie cancellation diagnostics before selecting a guarded fix.

---

# Lua recovery handoff â€” 2026-09-30

Active checkout `feat/novi-script-recovery`. The repository has extensive
pre-existing dirty work across multiple lanes; preserve it. Current Lua
details: `docs/journal/2026-09/LUA_RECOVERY_2026-09-30.md` and yesterday's
`LUA_RECOVERY_2026-09-29.md`.

Offline v61 fixes Magicman's beer-award operands: both retail sites pass
constructed name, `-1`, `true`. The generic constructor-slot alias is limited
to `GiveHeroObject`. Full 29-unit A/B changes only Magicman; before/after
Bordello smoke JSON is identical and 371 corpus Lua files are syntax-clear.
v61 has 210 package Lua files parsed, 216 manifest hashes matched, unchanged
stock/sidecar DLLs. No v61 in-game proof. Scratch and reports:
`work/codex_lua_marathon25_20260930`.

The v59 Book Collecting fresh-reload attempt from checkpoint_22 stopped at
frontend navigation: every game-window capture was black. No quest step ran.
The runner restored the live save slot; all 561 original files match the
pre-run hashes. The temporary staged profile was removed. Checkpoint_22 and
`key_resume_22.json` remain under `work/codex_lua_marathon22_20260929`.
Silver-key receipt and reload are still unproved. Avoid repeating the same
blind GUI launch while the shared desktop is in use; inspect capture/focus
conditions before retrying.

Offline v64 corrects Magicman's second dialogue counter. The four native
string conversion sites all read `[esp+0x44]`; the old Lua converted the
resource to text and attempted `resource + 1` in two timer branches. A
reviewed native/source evidence row recovers the numeric counter values.
The full 29-unit A/B changed only Magicman with zero registered v63 drift;
Bordello smoke is byte identical. The broader suite passed 94 tests, and
the focused timer test reached successive dialogue tags. The 371-file
readability corpus has zero syntax failures. v64 parses 210 package Lua
files with 216 matching manifest hashes; both DLLs are unchanged. No v64
in-game proof. See the current journal and
`work/codex_lua_marathon29_20260930`.

Offline v65 removes Magicman's reviewed temporary `CCharString` cleanup flag
and destructor guard from Lua. Native `00E418B8` sets bit `0x20` after
constructing `OBJECT_HERO_HAT_PIMP`; `00E418E8` tests it and `00E41901`
destroys that temporary. The chatter test now reaches five successive tags
without injecting `p0`. The broader suite passed 100 tests, the full
29-unit A/B changed only Magicman with zero Bordello baseline drift, smoke
is byte identical, and 371 corpus Lua files are syntax-clear. v65 parses
210 package Lua files, matches 216 manifest hashes, and keeps both DLLs
unchanged. It has no in-game proof. See the current journal and
`work/codex_lua_marathon30_20260930`.

Offline v66 rejoins two reviewed Magicman movie objects with their
counted-pointer cleanup members: the attacked scope and the presented-item
beer scope. A mock showed v65 calling `DestroyMovie(nil)` after a movie
start in each; the candidate passes the acquired movie handle. The beer
probe supplies an unrelated unresolved item-comparison result to enter
that branch. Raw/native hashes, call operands, and instruction bytes pin
the two scopes. Six focused evidence tests and 107 broad tests pass. Full
29-unit A/B changes only Magicman with zero Bordello baseline drift;
smoke is byte identical and 371 corpus Lua files are syntax-clear. v66
parses 210 package Lua files, matches 216 manifest hashes, and keeps both
DLLs unchanged. It has no in-game proof. See the current journal and
`work/codex_lua_marathon31_20260930`.

Offline v67 recovers Magicman's two presented-item tankard comparisons.
Both native `+0x8c` polls output an item name and compare it with
`OBJECT_BEER_TANKARD`; the sidecar exposes that name as
`g_PresentedItemName`. The Lua clears the global before each poll to reject
stale names. Both beer paths now reach their movie while other, absent,
and stale names skip it. Six focused evidence tests, 16 Magicman tests,
and 121 broad tests pass. Full 29-unit A/B changes only Magicman with
zero Bordello baseline drift. Smoke has the same six problems and only
changes the expected free-global diagnostic from `unaff_EBP` to
`g_PresentedItemName`. The 371-file readability corpus has zero syntax
failures. v67 parses 210 package Lua files, matches 216 manifest hashes,
and keeps both DLLs unchanged. It has no in-game proof. See the current
journal and `work/codex_lua_marathon32_20260930`.

Offline v68 recovers Magicman's HeroTricking talk movie. Raw Ghidra C ties
the `ppuStack_360` movie to both shared cleanup labels and its normal
counted-pointer release. A pre-lowering prototype altered later unrelated
operands and was rejected; the reviewed post-lift rule changes only the
movie start and 17 scoped cleanups. The v67 mock passed the hero to
`DestroyMovie`; v68 passes the movie at the normal and speech-failure
exits. Seven focused evidence tests, 18 final Magicman tests, and 129 broad
tests pass (the broad run preceded the extra shared-exit case). Full
29-unit A/B changes only Magicman, with zero Bordello baseline drift;
smoke is byte identical and 371 corpus Lua files are syntax-clear. v68
parses 210 package Lua files, matches 216 manifest hashes, and keeps both
DLLs unchanged. It has no in-game proof. See the journal and
`work/codex_lua_marathon33_20260930`.

Offline v69 corrects six early exits from Magicman's attacked movie.
Native `00E43965` constructs `appuStack_314`; both early cleanup sites
destroy that same movie object, while normal cleanup releases its
counted-pointer member. Seven evidence tests, 20 Magicman tests, and
44 reviewed-helper tests pass. Full 29-unit A/B changes only Magicman
with no registered v68 drift. Current registered v68 and v69 scratch
smoke JSON match exactly; both show the existing BordelloLady
`GetLHTSTag` mock error (the saved v68 JSON predates that current
result). The 371-file audit has zero syntax failures. v69 parses 210
package Lua files, matches 216 manifest hashes, and keeps both DLLs
unchanged. It has no in-game proof. See the journal and
`work/codex_lua_marathon34_20260930`.

Offline v70 corrects the two remaining cleanup operands in Magicman's
final beer movie. Native start `00E43D3B` and all five cleanup calls
use the same stack address. Seven evidence tests, 22 Magicman tests,
and 51 reviewed-helper tests pass. Full 29-unit A/B changes only
Magicman with no registered v69 drift; current registered and scratch
smoke JSON match byte for byte. The 371-file audit has zero syntax
failures. v70 parses 210 package Lua files, matches 216 manifest hashes,
and keeps both DLLs unchanged. It has no in-game proof. See the journal
and `work/codex_lua_marathon35_20260930`.

Offline v71 restores two Magicman `AddLineToConversation` calls that
were left as TODOs. Native slot `+0x5b8` and the Forge binding establish
the conversation, key, speaker, listener, and subtitle order. The
reviewed rule removes six unlifted diagnostics. Seven evidence tests,
23 Magicman tests, and 58 reviewed-helper tests pass. Full 29-unit A/B
changes only Magicman with no registered v70 drift; current registered
and scratch smoke JSON match byte for byte. The 371-file audit has zero
syntax failures. v71 parses 210 package Lua files, matches 216 manifest
hashes, and keeps both DLLs unchanged. It has no in-game proof. See the
journal and `work/codex_lua_marathon36_20260930`.

Offline v72 rejoins Magicman's first beer movie overdrunk task loop.
The old Lua returned from `Main` when speech ended, skipping the beer
award and movie cleanup; the reviewed rule follows the native loop and
releases the movie on termination. Six evidence tests, 25 Magicman
tests, and 64 reviewed-helper tests pass. Full 29-unit A/B changes only
Magicman with no registered v71 drift; current registered and scratch
smoke JSON match byte for byte. The 371-file audit has zero syntax
failures. v72 parses 210 package Lua files, matches 216 manifest hashes,
and keeps both DLLs unchanged. It has no in-game proof. See the journal
and `work/codex_lua_marathon37_20260930`.

Offline v73 corrects ten Magicman pause operands in first-beer movie
exits. Three native cleanup sites push `esi` to slot `+0x5ec`; all their
incoming script-task paths zero `esi` before the branch. A reviewed
rule changes the ten lifted vtable-address arguments to `false`.
Thirty-seven focused/Magicman tests and 70 reviewed-helper tests pass.
Full 29-unit A/B changes only Magicman with no registered v72 drift;
typed smoke JSON matches byte for byte with six baseline diagnostics.
The 371-file audit has zero syntax failures. v73 parses 210 package Lua
files, matches 216 manifest hashes, and keeps both DLLs unchanged. It
has no in-game proof. See the journal and
`work/codex_lua_marathon38_20260930`.

Offline v74 recovers five `BordelloLady.Main` opening calls from retail
argument setup: the entity `me`, its home position, zero/16.0 wander
distances, and scripting group 4. The previous Lua passed nil things
and position and group 0. Seven focused tests and 77 reviewed-helper
tests pass. The generated Main mock records the corrected five-call
sequence. Full 29-unit A/B changes only BordelloLady with no registered
v73 drift. Typed smoke retains six baseline diagnostics; its only row
delta is the recovered `GetHomePos` call (419 to 420 calls). The
371-file audit has zero syntax failures. v74 parses 210 package Lua
files, matches 216 manifest hashes, and keeps both DLLs unchanged. It
has no in-game proof. See the journal and
`work/codex_lua_marathon39_20260930`.

Offline v75 recovers BordelloLady's village crime-query argument.
Retail slot `+0x220` receives the quest's `BordelloVillage` thing at
`0x5c`; the generated call now passes that state thing. Seven focused
and 84 reviewed-helper tests pass. A Lua branch mock verifies the
village handle and skipped-query path. Full 29-unit A/B changes only
BordelloLady with no registered v74 drift; typed smoke JSON is byte
identical with six baseline diagnostics. The 371-file audit has zero
syntax failures. v75 parses 210 package Lua files, matches 216
manifest hashes, and keeps both DLLs unchanged. It has no in-game
proof. See the journal and `work/codex_lua_marathon40_20260930`.

Offline v76 restores BordelloLady's entity handle in the two lifted
copies of `ClearThingHasInformation`. Both map to retail slot `+0x5a4`
at `00E400B2`, which pushes `this+8`. Seven focused and 91 reviewed-
helper tests pass; a generated Lua nunnery-branch mock confirms `me`.
Full 29-unit A/B changes only BordelloLady with no registered v75
drift; typed smoke JSON is byte identical with six baseline diagnostics.
The 371-file audit has zero syntax failures. v76 parses 210 package Lua
files, matches 216 manifest hashes, and keeps both DLLs unchanged. It
has no in-game proof. See the journal and
`work/codex_lua_marathon41_20260930`.

Offline v77 restores BordelloLady's personality override using the
existing Forge `EntitySetPersonalityOverrideByString` binding. Native
slot `+0x9ac` receives `me` and
`OPINION_PERSONALITY_EX_PROSTITUTE`; the two lifted placeholders and
false missing-binding diagnostics are gone. Nine focused and 100
reviewed-helper tests pass. A generated Lua nunnery-branch mock verifies
the entity and string. Full 29-unit A/B changes only BordelloLady with
no registered v76 drift; typed smoke JSON is byte identical with six
baseline diagnostics. The 371-file audit has zero syntax failures.
v77 parses 210 package Lua files, matches 216 manifest hashes, and
keeps both DLLs unchanged. It has no in-game proof. See the journal and
`work/codex_lua_marathon42_20260930`.

Offline v78 corrects BordelloLady's post-control movie pause operand.
Retail `00E3EDD5 -> 00E40392` pushes zeroed `edi` to slot `+0x5ec`
at `00E40397`; the separate `00E3EDB2 -> 00E40359` loop exit was
already false in Lua. Eight focused and 108 reviewed-helper tests pass.
A generated Lua branch mock verifies false and both movie cleanup calls.
Full 29-unit A/B changes only BordelloLady with no registered v77
drift; typed smoke JSON is byte identical with six baseline diagnostics.
The 371-file audit has zero syntax failures. v78 parses 210 package Lua
files, matches 216 manifest hashes, and keeps both DLLs unchanged. It
has no in-game proof. See the journal and
`work/codex_lua_marathon43_20260930`.

Offline v79 restores the entity receiver for eleven BordelloLady
`GetHealth` calls. Each retail slot `+0x420` site receives the
`GetScriptThing` result from the same entity wrapper used by
`ClearCommands`; the Lua calls now pass `me`. Seven focused and 115
reviewed-helper tests pass. A generated Lua mock observes eleven
entity receivers after the change. Full 29-unit A/B changes only
BordelloLady with no registered v78 drift; typed Bordello smoke JSON
is byte identical with six baseline diagnostics. The 371-file audit
has zero syntax failures. v79 parses 210 package Lua files, matches
216 manifest hashes, and keeps both DLLs unchanged. It has no in-game
proof. See the journal and `work/codex_lua_marathon44_20260930`.

Offline v80 corrects all four BordelloLady `SetCutsceneSkippable`
flags. Retail slot `+0x60c` pushes `false, true, false, true` in
exported-source order; generated v79 Lua had `nil, nil, true, nil`.
Seven focused and 122 reviewed-helper tests pass. A generated Lua mock
confirms all four arguments. Full 29-unit A/B changes only BordelloLady
with no registered v79 drift; typed smoke JSON is byte identical with
six baseline diagnostics. The 371-file audit has zero syntax failures.
v80 parses 210 package Lua files, matches 216 manifest hashes, and
keeps both DLLs unchanged. It has no in-game proof. See the journal
and `work/codex_lua_marathon45_20260930`.

Offline v81 restores BordelloLady's hero sex count increment.
Retail `00E3FD81` gets the count, `00E3FD8A` increments `eax`,
and `00E3FD8C` sets the result; the readable Lua now performs one
getter and one setter with `+ 1`. Seven focused and 129 reviewed-
helper tests pass. Generated Lua mocks cover prior counts `-1`,
`0`, and `7`. Full 29-unit A/B changes only BordelloLady with no
registered v80 drift; typed smoke JSON is byte identical with six
baseline diagnostics. The 371-file audit has zero syntax failures.
v81 parses 210 package Lua files, matches 216 manifest hashes, and
keeps both DLLs unchanged. It has no in-game proof. See the journal
and `work/codex_lua_marathon46_20260930`.

Offline v82 restores BordelloLady's `Name` field reload in the free
party cutscene branch. `Init` assigns the data string to native offset
`+0x28`; Main reloads that field before comparing with `HEDWIG`.
The old readable branch compared nil and always missed the Hedwig
variant. Six focused and 135 reviewed-helper tests pass; generated
Lua mocks cover nil, `HEDWIG`, and `POLLY`. Full 29-unit A/B changes
only BordelloLady with no registered v81 drift; typed smoke JSON is
byte identical with six baseline diagnostics. The 371-file audit has
zero syntax failures. v82 parses 210 package Lua files, matches 216
manifest hashes, and keeps both DLLs unchanged. It has no in-game
proof. See the journal and `work/codex_lua_marathon47_20260930`.

Offline v83 adds the `SetCutsceneSkippableWhilePaused(bool)` LuaQuestState
forwarder and registration in an isolated sidecar source commit
`ab268435b8e8491871c1c3c0ea8982fcbdb9c22e`. Retail
`V_Bordello.PlayCutscene` slot `+0x610` passes true then false; the
existing Lua calls already did so. The Win32 Release sidecar built,
and the exported source patch is tracked under
`tools/script_recovery/sidecar_patches/`. v83 only changes the
NoviCompatibility DLL; it has no in-game proof.

Offline v84 removes two stale generator TODO comments and diagnostics
now that the sidecar method is registered. Five focused and 140
reviewed-helper tests pass. Full 29-unit A/B changes only
`V_Bordello/V_Bordello.lua`; sidecar aware typed smoke has five
existing problems rather than six, only removing this unknown method.
The 371-file audit has zero syntax failures. v84 parses 210 package
Lua files, matches 216 hashes, and keeps the stock DLL unchanged.
It has no in-game proof. See the journal and
`work/codex_lua_marathon48_20260930` plus
`work/codex_lua_marathon49_20260930`.

Offline v85 restores Madame's self receiver for
`SetIsPushableByHero(false)`. Retail slot `+0xd30` takes a by-value
script thing assembled from Madame's own `this+0xc` and `this+0x10`
fields. Five focused and 145 reviewed-helper tests pass; generated
draft/readable mocks capture Madame and false. Full 29-unit A/B
changes only Madame; typed smoke is byte identical with five existing
problems. The 371-file audit has zero syntax failures. v85 parses 210
package Lua files, matches 216 manifest hashes, and keeps both DLLs
unchanged from v84. It has no in-game proof. See the journal and
`work/codex_lua_marathon50_20260930`.

Offline v86 restores Madame's six `CameraUseCameraPoint` calls.
Retail slot `+0x678` uses literal `CAM_B_MADAM`, Madame's self
script thing, `-1.0`, `0`, and `-1` at every site; the sidecar has
the matching string-and-target overload. Four focused and 149
reviewed-helper tests pass. Full 29-unit A/B changes only Madame;
typed smoke is byte identical with five existing problems. The
371-file audit has zero syntax failures. v86 parses 210 package Lua
files, matches 216 hashes, and keeps both DLLs unchanged from v85.
Its Madame calls lack in-game proof. See the journal and
`work/codex_lua_marathon51_20260930`.

v86 live Book Collecting replay: a visible frontend launch showed
the earlier black capture was session-specific. The runner now skips
both opening movies with Escape. The retained 22-book checkpoint
passed its fresh-load guard, then normal teacher interactions donated
books 23, 24, and 21. The final scene set `KeyRewarded=true` and
raised the silver-key count from 0 to exactly 1; all 10 guarded steps
passed. Stable 23- and 24-book checkpoints and the harvested final
AutoSave are under `work/codex_lua_marathon52_20260930`. The result
AutoSave SHA-256 is
`8bdd6b8a5346fe04b437f911f143621e7c17d483ef292fb2a4292b3d5a315bf0`.
No v86 bundle was installed permanently.

Read-only decoding of the 22/23/24/25-book AutoSaves confirms
the saved QUESTS data and 80 book flags at each count. The harvested
save has `KeyRewarded=true` and exactly one `OBJECT_SILVER_KEY` in
hero inventory; the earlier saves have none. In-engine inventory
proof after fresh reload remains open. On the first reload the
completed `V_BookCollecting` host was absent, so its per-quest check
could not attach. A LUAGameflow check is prepared as
`work/codex_lua_marathon52_20260930/global_key_reload.json`; the user
closed Fable before its save loaded because another agent needed the
debug editor. Do not relaunch while that editor is in use. Both
temporary test profiles were moved out of live Saves into the work
directory; all 561 original save paths and hashes match the pre-run
snapshot. Fable is closed. The marathon26 broad alignment candidate
remains rejected. No commit or push this session.

Offline v87 restores BordelloClient's hero sex count increment
in its paid encounter. Retail Main `0x00E44EA0` gets the count via
slot `+0x368`, increments `eax`, and passes it to setter slot
`+0x36c`; generated v86 Lua passed the old count. Five focused and
154 reviewed-helper tests pass. Full 29-unit A/B changes only
BordelloClient; sidecar aware typed smoke is byte identical with five
existing problems. The 371-file audit has zero syntax failures.
v87 parses 210 package Lua files, matches 216 manifest hashes, and
keeps both DLLs unchanged from v86. It has no in-game proof. See
the journal and `work/codex_lua_marathon53_20260930`. Fable remains
closed while another agent uses the debug editor. Continue offline
recovery; BordelloClient's unresolved cleanup pause calls are the
next native operand target.

Offline v88 restores BordelloClient's four cleanup unpause calls.
Retail slot `+0x5ec` has `push 0` immediately before each call at
`00E465CB`, `00E465F9`, `00E46627`, and `00E46655`. Six focused
and 160 reviewed-helper tests pass. Full 29-unit A/B changes only
BordelloClient; sidecar aware typed smoke remains byte identical
with five existing problems. The 371-file audit has zero syntax
failures. v88 parses 210 package Lua files, matches 216 manifest
hashes, and keeps both DLLs unchanged from v87. It has no in-game
proof. See the journal and `work/codex_lua_marathon54_20260930`.
Fable remains closed while another agent uses the debug editor.
Next offline target: the unresolved BordelloClient `seh_Whore`
resource assignment at native `00E45FCD`.

Offline v89 restores BordelloClient's `seh_Whore` resource copy
and declined dialogue health receiver. The native assignment at
`00E45FCD` targets the parent quest's offset `+0xa4`; the source
and later `GetScriptThing` call use the constructed/acquired client
resource. Nine focused and 169 reviewed-helper tests pass, including
a Lua handle mock. Full 29-unit A/B changes only BordelloClient;
sidecar aware typed smoke remains byte identical with five existing
problems. The 371-file audit has zero syntax failures. v89 parses
210 package Lua files, matches 216 manifest hashes, and keeps both
DLLs unchanged from v88. It has no in-game proof. See the journal
and `work/codex_lua_marathon55_20260930`. The shared debug editor remains reserved for another agent; do not
launch Fable until that reservation is released.
Next offline target: BordelloClient's remaining native diagnostics.

Offline v90 restores three BordelloClient byte stores and the
hero-hit flag test. Retail stores 1 for arrival at `00E450C5`, and
0/1 for the hit response at `00E453CB`/`00E453D2`; `00E4541A`
tests the latter byte. Six focused and 175 reviewed-helper tests
pass. Full 29-unit A/B changes only BordelloClient. Sidecar aware
typed smoke keeps five existing problems; the no-hit mock's Main
call count drops from 5,214 to 2,614 over the same 201 frames,
with all other results identical. The 371-file audit has zero
syntax failures. v90 parses 210 package Lua files, matches 216
manifest hashes, and keeps both DLLs unchanged from v89. It has
no in-game proof. See the journal and
`work/codex_lua_marathon56_20260930`. The shared debug editor is
still reserved for another agent; do not launch Fable until it is
released. Next offline target: BordelloClient's unresolved
indirect `+0x18` call that supplies a movement position.

Offline v91 restores BordelloClient's second controlled-resource
movement position. Native calls at `00E45085` and `00E45735` use
the same `[esp+0x2c]` pointer, null fallback, and `+0x18` slot;
the generated v90 second path instead read an uninitialized value
from a reused dialogue variable. Seven focused and 182 reviewed-
helper tests pass. Full 29-unit A/B changes only BordelloClient;
sidecar aware typed smoke is byte identical with five existing
problems. The 371-file audit has zero syntax failures. v91 parses
210 package Lua files, matches 216 manifest hashes, and keeps both
DLLs unchanged from v90. It has no in-game proof. See the journal
and `work/codex_lua_marathon57_20260930`. The shared debug editor
remains reserved for another agent; do not launch Fable until it is
released. Next offline target: review the remaining BordelloClient
native jump target and then choose the next substantive Lua gap.

Offline v92 restores BordelloClient.OnPredicateFail's indexed
ClientInUse clear. Native `00E3B230` decrements `ClientsAlive`, then
stores zero to the parent quest's `+0x58+ClientID` bool. The first
scratch candidate exposed a nil ClientID in standalone callback
smoke and was not promoted; the corrected readable code defaults
to ID 0 when no ID is available. Nine focused and 191
reviewed-helper tests pass. Full 29-unit A/B changes only
BordelloClient; sidecar aware typed smoke keeps five existing
problems, with only the intended SetStateBool call added to
OnPredicateFail. The 371-file audit has zero syntax failures.
v92 parses 210 package Lua files, matches 216 manifest hashes,
and keeps both DLLs unchanged from v91. It has no in-game proof.
See the journal and `work/codex_lua_marathon58_20260930`. The
shared debug editor remains reserved for another agent; do not
launch Fable until released. Next offline target: select another
substantive Bordello Lua gap; BordelloClient Main's remaining label
diagnostic represents a structured jump target, not an unlifted
gameplay call.

v93 restores Madame.Main's two retail hit-response byte flags.
Eight focused and 199 reviewed-helper tests pass; the full 29-unit
A/B changes only Madame. Typed smoke keeps five existing problems,
with only Madame.Main's no-hit frame/call totals changed. The
371-file audit has zero syntax failures. v93 parses 210 package Lua
files, matches 216 manifest hashes, and keeps both DLLs unchanged.
v93 itself has no in-game proof. In a separate v92 launch, the
completed Book Collecting save passed a fresh-load inventory check:
one silver key, good wizard hat, all 25 donated books absent, quest
complete and inactive. The runner finished without Lua/C++ exception
markers; Fable closed and all 561 original save files matched their
pre-run hashes. See the current journal and
`work/codex_lua_marathon59_20260930`. Current user steering allows
launches for testing; the debug editor is no longer in use.

Offline v94 restores Madame.Main's presented beer comparison.
Retail `00E3BEDC` polls the item; `00E3BF13` compares its name with
`OBJECT_BEER_TANKARD`; `00E3BF28` takes the beer response on equality.
The reviewed rule clears the sidecar item-name global before polling
and sets the native equality flag from the returned name. Seven
focused and 206 reviewed-helper tests pass. Full 29-unit A/B changes
only Madame. The selected readable stage has a 16-line Madame diff,
and typed smoke retains five existing problems with the expected
item-name free global only. The 371-file audit has zero syntax
failures. v94 parses 210 package Lua files, matches 216 manifest
hashes, and keeps both DLLs unchanged from v93. No v94 in-game
presentation proof exists. See the current journal and
`work/codex_lua_marathon60_20260930`.

Offline v95 restores Madame.Main's health gate before the
returned-beer speech. Native `00E3BFC1` reads health, the shared
byte at `[esp+0x13]` is cleared for nonpositive or unordered health,
and `00E3BFF0` skips speech but still reaches the tankard return.
Six focused and 212 reviewed-helper tests pass. A presented-tankard
Lua probe shows the zero-health speech removed while the tankard is
returned; positive-health full call traces match v94. Full 29-unit
A/B changes only Madame; ordinary typed smoke is byte-identical
with five existing problems. The selected readable stage parses,
the 371-file audit has zero syntax failures, and v95 parses 210
package Lua files with 216 matching manifest hashes. Both DLLs are
unchanged. There is no v95 in-game tankard proof. See the journal
and `work/codex_lua_marathon61_20260930`.

Offline v96 gives Madame.Main one stable local resource handle.
Native constructor `00E3BC4D` and all 22 `GetScriptThing` calls use
the same controlled stack resource; the generated Lua used several
unset or overwritten temporaries. The first scratch candidate lost
an early stack assignment and was rejected. The corrected version
retains that assignment and uses a stable local for resource calls.
Six focused and 218 reviewed-helper tests pass. A strict tankard
probe rejects v95's nil receiver and accepts v96's constructed
handle at zero and positive health. Full 29-unit A/B changes only
Madame; ordinary typed smoke is byte-identical with five existing
problems. The 371-file audit has zero syntax failures. v96 parses
210 package Lua files, matches 216 manifest hashes, and preserves
both DLLs. No in-game Madame proof exists. See the journal and
`work/codex_lua_marathon62_20260930`.

Offline v97 restores two BordelloLady parent `seh_Whore` resource
copies and three lifted `GetScriptThing` receivers. Native constructor
`00E3EB67`, both copy calls, and eleven getter sites point to one
controlled stack resource; generated Lua had discarded its handle.
Seven focused and 225 reviewed-helper tests pass. Sidecar aware smoke
is unchanged with five existing problems; full 29-unit A/B changes
only BordelloLady. The 371-file audit has zero syntax failures. v97
parses 210 package Lua files, matches 216 manifest hashes, and keeps
both DLLs unchanged. No in-game BordelloLady proof exists. A read-only
scan found no original save profile already in Bordello. See the
current journal and `work/codex_lua_marathon63_20260930`.

Next BordelloLady gap: native `00E3ED9A` and `00E3EDC5`
acquisition calls pass the same controlled resource, while generated
Lua still calls `me:AcquireControl` and uses unset prepare/release
temporaries. Review task methods and cleanup with acquisition before
promotion.

Offline v98 restores BordelloLady's local resource acquisition
and cleanup. Four native slot-`+0x20` sites pass the same controlled
resource and priority 4; generated Lua used a separate entity handle
and one false priority 8. The corrected rule also distinguishes the
conversation movie destructor from local resource release. A first
scratch candidate got that destructor wrong and was rejected. Seven
focused and 232 reviewed-helper tests pass. Generated-Lua probes
cover cower, conversation, interruption release, and movie cleanup
with reacquisition. Sidecar aware smoke keeps five existing problems;
full 29-unit A/B changes only BordelloLady. The 371-file audit has
zero syntax failures. v98 parses 210 package Lua files, matches 216
manifest hashes, and keeps both DLLs unchanged. No in-game
BordelloLady proof exists. See the current journal and
`work/codex_lua_marathon64_20260930`.

v100 update: the duplicate movie-start fix passes the full 29-unit comparison
and is packaged (210 Lua files, 216 hashes, unchanged DLLs). Assisted Bordello
arrival now plays the complete introduction, resumes entities, destroys its
movie, and exits both scene modes with zero logged Lua errors. All 561 original
save files were restored unchanged. Fable is closed. Current next work is the
branch-local hit flag behind unconditional cowering; 346 tests and 28 subtests
pass, full A/B pending in work/codex_lua_branch_bytes_20261001. See today's journal.

Tonight's v98 live test did not reach Bordello. The first attempt exposed a
frontend driver issue after the save loaded; `autopilot.py` now stops menu
navigation when quest hosts appear. The second attempt used a map regex that
left `Runner.regionOf` empty; the scratch entry config is corrected. The third
attempt installed the runner but the copied Barrow Fields checkpoint remained
in a scene with its camera fixed behind a wall and never became ready for a
map-slot transition. The Fable window disappeared during that wait. See the
current journal and `work/codex_lua_marathon65_20260930` for the exact run
commands and evidence. All 561 original save files matched their prelaunch
hashes after each attempt, and Fable is closed. The v98 README manifest hash
was refreshed; all 216 package entries match. Next: select a checkpoint
outside an active scene, verify the loaded camera and cutscene state, then
retry assisted Bordello entry before claiming any BordelloLady runtime proof.
The user paused work for tonight; resume only when asked.

For package-dir Bordello smoke, pass the matching draft
`CONVERSION_REPORT.json` with `--conversion-report`. Without it the
harness gives `BordelloLady:GetLHTSTag` a table instead of its native
`CCharString` parameter. Typed v71/v72/v73 smoke JSON matches exactly and
has six baseline diagnostics.


Native recreation checkpoint (2026-10-01): display Init/destructor and callback-list ABI promoted; gate_display_lifetime.py passes 8,448 linked / 6,144 native comparisons. Renderer-state destructor A044B0 also promoted (21-byte RELOCATION_MATCH, 1,536 cases). Header audit 211 entries / zero regressions, staged visual bootstrap PASS. Native GFMain still unresolved. See docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-01.md and work/codex-native-startup/display-lifetime. Symbol refresh complete; refresh_display_lifetime_closure.py running. Next: renderer constructors A05970/A0AFE0 and ownership. Preserve the separate Lua lane and dirty baseline.

Native continuation: renderer state/device-state/core constructors and core destructor promoted, all exact MATCH/RELOCATION_MATCH; 10,752 linked + 1,536 comparative cases, 216-entry clean header audit and staged visual bootstrap PASS. Source CRenderManager_Constructors_00a05970.cpp; scratch render-constructors. Refresh symbol index before refresh_render_constructor_closure.py. Next owned 2D renderer members: native destructor 009C2FD0 (mislabelled), ctor 009D9A30; PDB render-2d-ownership-pdb.txt and render-owned-members-decompile-20261001.log.

Native continuation: shared vertex-shader lifetime promoted (five new entries plus retyped CountReferences), all exact; 5,632 linked / 3,300 comparative cases and seven verified vtable slots. Scratch vertex-shader-lifetime; symbols refreshed. Last native link checkpoint 627 objects / 0 duplicates / 693 unresolved before this shader batch. Next: CShaderDataBank ownership (A62020 ctor, native size 0x128 = CBankFile 0x110 + vector12 + stateblock12), remaining shader-base helper 987CF0, then renderer 2D ownership.


Native recreation checkpoint (2026-10-01): shader debug-name construction now passes 256 cases and exact relocation parity. Shader-bank state copy/CRC providers are consolidated under shared native types (4,096 cases, exact parity); 36 header dependents and staged bootstrap pass. Default initialization remains scratch-only pending byte acceptance. See docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-01.md. Continuing renderer-owned draw data; native GFMain remains unresolved.

Native renderer checkpoint: quick-draw vector destruction (009C2F50) promoted with exact relocation parity, 3,841 compiled cases and 75 native comparisons. Native GFMain closure refreshed: 635 sources, zero duplicate providers, 689 unresolved symbols (native-gfmain-shader-ownership-closure-20261001). Shader-bank lifetime remains staged while its 20-slot bank vtable is completed and validated.

Shader-bank lifetime promotion deferred: Defender blocked generic fixture 00A62020 (WinError 225, ThreatID 2147651817). Dedicated proof, 113-entry header audit and staged bootstrap passed, but all bank ABI/lifetime changes remain scratch-only. No blocked artifact retry or security changes. See native journal for evidence and exact path. Continuing independent shader-resource work.

Native shader-resource construction/close/uninitialise promoted: exact 51/69/17-byte bodies, 896 compiled cases, 650 native comparisons, 13 header dependents and generic catalog/candidate builds pass. Corrects prior mislabeled 00A5D7A0 provider. Shader-bank lifetime remains scratch-only after Defender blocked its generic fixture. Continuing vertex-input ownership.

Native shader-resource ownership checkpoint: vertex-input ctor/destructor/range/deleting thunk and shader-resource destructor/deleting thunk are promoted with exact relocation parity. Latest resource cleanup passes 4,096 compiled cases, 1,152 native comparisons and all five catalog/candidate checks. See native journal. Shader-bank lifetime remains unpromoted after Defender blocked generic 00A62020; no retry or security bypass. Native link refresh is in progress.


Native checkpoint 2026-10-01: promoted shader-resource bindings (00A5D4D0/E0/F0, 00A5D500), typed counted assignment 009DE7C0, and manager resource factory 0098B860 (accepted NEAR 97.5%) / deleter 0098BA10 (MATCH). Dedicated, catalog, candidate and affected-header checks pass; details and backup manifests are in docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-01.md. Factory link refresh is pending. Shader-bank lifetime remains scratch-only after Defender blocked its generic constructor fixture; no retry or security change. Native startup remains unresolved.

Native factory closure refresh complete: 639 sources, zero duplicate providers, 689 unresolved symbols; fixed point in one round. Evidence: work/codex-native-startup/native-gfmain-shader-factory-closure-20261001.

Native resource-vector helpers promoted: 0098DC30/BFF0/C030/CEE0, exact parity, 899 linked cases and 638 direct native comparisons plus factory comparisons. Catalog/candidate checks pass; no implementation/header change. See native journal. Continuing vertex-input binding; shader-bank security block remains untouched.

Native input binding 00A5D8B0 promoted (66-byte NEAR 96.3%); 288 linked/native comparisons, generic/candidate and 16 header dependents pass. Next: typed stream/declaration vector assignment. Details in native journal.

Native stream-vector assignment 00A5E9C0 promoted: exact 347-byte parity, 540 linked/full-chain native comparisons and catalog/candidate pass. Declaration-vector assignment remains scratch pending complete-chain tests. See native journal.

Native declaration-vector assignment 00A5E2E0 promoted (NEAR 92.8%); 1,200 linked and full-chain native comparisons, catalog/candidate pass. Private optimized helper has no independent address claim. Next: nested stream-element vector copy/assignment.

Nested stream-element vector ctor/assignment promoted (00A5E0A0 exact / 00A5E4D0 NEAR): 1,254 linked/native comparisons, catalog/candidates pass. Full resource vertex-input copy integration passes 2,160 compiled cases with all four recovered providers required. Native link refresh follows; shader-bank block remains untouched.

Vertex-input copy closure: fixed point in one round, 643 sources, zero duplicates, 689 unresolved symbols (native-gfmain-vertex-input-copy-closure-20261001). New evidence identifies 009CC350 as CBankFile::GetMaxEntryIndex (backward validity scan), not GetSize; PDB symbol and native code agree. Continuing that independent helper using the existing bank layout; blocked shader-bank lifetime/ABI batch remains untouched.

CBankFile::GetMaxEntryIndex 009CC350 promoted (NEAR 92.1%); 1,741 linked/native comparisons, generic/candidate and 113 header dependents pass. Nonvirtual declaration only; blocked shader-bank batch unchanged. Next: shader-pointer resize used by bank setup.

Shader-pointer resize/fill-insert promoted (00A5E6A0 exact / 00A5E3B0 NEAR 91.6%): 2,064 and 2,736 linked/native cases respectively; generic/candidate checks pass. Growth helper exact in the chain but no new helper coverage claim. Continuing resource bank setup; blocked shader-bank lifetime/ABI batch remains untouched.

OpenShaderBank 00A5D7C0 promoted: exact 60-byte parity, 180 linked/native comparisons and 17 header dependents pass. Five-provider bank setup integration passes 1,584 compiled cases, including packed reads and real vector operations. No shader-bank lifetime/virtual-layout promotion. Native link refresh follows.

Vertex-shader COM constructor 00A63020 promoted (NEAR 95.9%): 1,728 linked/native comparisons; existing lifetime 5,632/3,300 and 33 header dependents pass; seven candidates pass. Typed SDK-confirmed COM creation slots preserve ignored HRESULT/allocation-failure behavior. Latest preceding closure: 647 sources / zero duplicates / 689 unresolved. Details in native journal.

Vertex declaration generation 00AB3870 promoted (NEAR 93.5%): 292 compiled / 293 comparative cases; tables validated separately; 26 header dependents and catalog/candidate pass. Next: declaration-vector fill insertion 00AB4260 and composed generation. Shader-bank security block remains untouched.

Declaration fill insertion 00AB4260 promoted (280-byte exact relocation match), 2,736 compiled/native cases. Composed generation passes 292 cases using both recovered providers. Native closure refresh running; next stream-format/input construction.

Stream-format constructor/append/size promoted: three exact entries plus type-size lookup NEAR 97.1%, 2,062 compiled/native cases, 28 header dependents, four candidates pass. Next AddStream 00AB3B40. Latest preceding native closure: 649 sources, zero duplicates, 689 unresolved.

AddStream 00AB3B40 promoted (NEAR; only unused trait-tag stack displacement differs), 552 compiled/native cases, 28 header dependents pass. Next stream-vector overflow 00AB4540 and composed input building.

Stream growth 00AB4540 promoted (242-byte exact relocation match), 870 compiled/native cases. Seven-provider input-building chain passes 2,240 cases; duplicate inline lookup linkage fixed with generator bytes unchanged. Native input-building closure refresh running.

Render-state initial data promoted (23 symbols / 128 bytes; 486 linked default-state cases). Native render-data closure refresh running. Shader-manager Uninitialise 0098A7A0 remains below threshold in scratch; see native journal for probes. Prior closure 652 sources / zero duplicates / 689 unresolved.

Native font sharing checkpoint: four typed replacements promoted; 1,200 compiled/native cases and 354 header dependents pass. Latest scratch closure 654 sources / zero duplicates / 664 unresolved. Next CFontManager counted-pointer Reset. Full evidence in docs/journal/2026-10/NATIVE_RECONSTRUCTION_2026-10-01.md; existing security blocks remain untouched.

Native font-manager Reset 009A7340 promoted: exact 103 bytes, 90 compiled/native cases, candidate and landed gate pass. Next streaming-font InitStreamingEntry 00A5EF50; reconcile PDB reference parameter with existing pointer declaration. Native closure refresh running; security-blocked artifacts remain untouched.

Native streaming-font entry setup 00A5EF50 promoted (NEAR 99.2%, 450 compiled/native cases, 115 header checks). Next bank symbol lookup 009CCDC0 and async entry creation 009D58D0. Latest completed font-reset closure: 655 sources / zero duplicates / 663 unresolved; streaming-entry refresh running. See native journal.

Native bank symbol lookup 009CCDC0 and unsigned string-tree search 009D2DD0 promoted: exact 35/112 bytes, 1,458 compiled/native chain cases. Next async-entry creation 009D58D0; PDB layout/lifetime investigation underway. Latest preceding streaming-entry closure: 656 sources / zero duplicates / 664 unresolved.

Native async bank factory 009D58D0 and alignment 009CBB90 promoted: exact 152/24 bytes, 1,134 compiled/native cases, 118 header dependents pass. Next composed streaming-font setup. Latest preceding bank-symbol closure: 657 sources / zero duplicates / 663 unresolved; async-entry refresh running.

Native streaming-font setup now has a five-provider composed gate: 360 cases pass, all five providers required. Latest scratch closure 658 sources / zero duplicates / 662 unresolved. Native executable still incomplete. Continue font-bank construction and remaining native startup dependencies; recorded security blocks remain untouched.

Native symbol-map DeallocateMemory 009E7680 promoted (111-byte exact relocation match, 276 compiled/native cases); composed trim/release gate passes 340 cases. Next symbol-map lifetime/string-set cleanup or full bank virtual ABI; research logs recorded in native journal. Native memory-closure refresh running; prior 658 sources / zero duplicates / 662 unresolved.

Native symbol-map lifetime promoted and validated: 1,458 compiled / 2,916 native cases; full 380-entry header audit with five resolved fixture regressions, nine candidates pass. Latest native closure 660 sources / zero duplicates / 660 unresolved. ACTIVE NEXT: distinguish base CArrayVector::DeallocateMemory (009E7680, preserves Dirty) from true CVectorMap::DeallocateMemory (009E79E0, clears Dirty), then CSymbolMap::Clear (009E5250). Detailed correction plan and evidence in native journal; no correction edits yet.

Array/base DeallocateMemory correction COMPLETE: existing 111/152-byte bodies unchanged; true map reset 009E79E0 and CSymbolMap::Clear 009E5250 promoted (103/62 bytes). 552 reset and 1,458 compiled / 2,916 native clear cases; 439 header dependents, four candidates pass. Native reset-closure refresh running. Next CBankFile virtual ABI / CreateSymbolMap 009CC530; see journal.

Native bank CreateSymbolMap 009CC530 and 20-slot bank ABI promoted: 576 compiled/native cases, 120 vtable slots and 625 header entries pass; landed gates/candidate pass. Previous reset closure 661 sources / zero duplicates / 660 unresolved; bank closure running. Next character search 0099E5A0 and conversion dependencies. Security blocks remain untouched; see native journal.

Native character search 0099E5A0 promoted (69 bytes, 2,304 compiled/native cases); three-provider bank enumeration chain passes 576 cases. Latest bank ABI closure: 663 sources / zero duplicates / 693 unresolved, reflecting newly exposed virtual methods. Next FastCompare 009E6530, then remaining symbol-map sort helpers. See native journal.

FastCompare 009E6530 promoted: 108-byte NEAR, 2,592 compiled/native cases. ACTIVE scratch symbol-sort-helpers: three insertion helpers match native bytes, ownership/behavior validation pending. Latest closure remains bank ABI 663/0/693; refresh after helper promotion.

Symbol insertion helpers 009E6FA0/009E66F0/009E6D10 promoted; 576 compiled/native cases with real comparator and four required providers pass. Closure refresh running. Next symbol partition 009E6C80 and pair swap 009E6480; 009E65A0 is unrelated vector swap. No new header changes yet.

Symbol partition 009E6C80 and pair swap 009E6480 promoted (133/168 exact bytes, 768 compiled/native cases). Latest completed closure 668/0/689. ACTIVE scratch symbol-heap: six heap helpers match instruction sequences; full relocation/behavior tests pending. See native journal for addresses and sizes.

Heap fallback COMPLETE: six helpers promoted, 3,174 compiled/native cases; full 15-provider sort chain passes 1,170 cases. Closure 676 sources / zero duplicates / 687 unresolved. LowerBound 009E6910 promoted (77 bytes, 2,040 compiled/native cases), landed gate running. Next name-set unique insertion for AddSymbolDirty; see native journal.

String-set insertion 004AC380 / node link 004ABC70 promoted (254/206 bytes); 1,088 compiled/native cases and 398 header dependents pass, candidates and landed gate pass. Predecessor helper is behavior-only, no coverage claim. ACTIVE NEXT: full bank-to-symbol-map composed gate; all required leaf families are now present. Closure refresh running; prior 676/0/687.

Full bank-to-symbol-map chain COMPLETE: 4,352 compiled cases with 32 required real providers, landed gate passes. Closure 679 sources / zero duplicates / 685 unresolved. ACTIVE NEXT: virtual bank enumeration 009CBD10 / font 00A63380 / streaming font 00A5EF10 / text 009C79E0; trace native bodies before edits. Security blocks untouched.

Typed bank enumeration COMPLETE: four exact methods, 640 compiled/native cases, all candidates and landed gate pass. Old misnamed base source retired; font assembly wrappers replaced. Closure refresh next. ACTIVE NEXT: full bank-to-symbol-map composition with real enumeration methods.

Real enumeration chain COMPLETE: 17,408 cases, 36 required providers, landed gate passes. Latest closure 683/0/681. ACTIVE NEXT: remaining bank policy/callback virtuals; use validated bank_virtual_layout.json and native evidence.

Bank update path COMPLETE: 468 compiled/native cases, ten providers, 125 header dependents, nine candidates pass. Latest closure 692/0/674. ACTIVE NEXT: async Close 009D58A0 and ProcessDisposalList 009D6730; RetailMode is bank +0x8C per PDB/retail evidence.

Async bank close/disposal COMPLETE: 1,224 compiled/native cases, 375 header dependents, candidates and landed gate pass. Closure 694/0/673. ACTIVE NEXT: 009D5A90 real PDB name is private RestartAbortedReads; rename provisional ProcessReadsToRestart in header, AsyncUpdatePending and its fixture, preserving caller bytes.

RestartAbortedReads COMPLETE: 200 exact bytes, 900 compiled/native cases with four real providers, 140 header dependents, candidates/landed gate pass. Closure 695/0/673. ACTIVE NEXT: Abort 009D6350 and typed boundaries. 00A62ED0 is FreeResource thunk to 00A02B30; avoid mistaking Ghidra followed-thunk output for raw body. See journal and bank-abort evidence.
