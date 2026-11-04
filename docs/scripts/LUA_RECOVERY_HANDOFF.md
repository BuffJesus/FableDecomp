# Lua recovery handoff

## Stopped for tonight ? user request, 2026-10-27

**Lua work is stopped at the user's explicit request. Resume only when requested
in the next session.** This supersedes the older ACTIVE/continue-until-stop
entries retained below. No owned validation or packaging jobs remain running.

### Accepted workspace and verified bundle

- **W507 is the latest accepted Lua change.** Adjacent live generated label
  aliases retarget 428 goto operands and prune 75 unused definitions in 39 Lua
  files across 18 registered units. Complete stripped Lua 5.4 bytecode matches
  before/after retargeting and pruning at original line coordinates. Actual
  line deletion changes source/debug coordinates. Early `readable_file` AST and
  all 267 faithful drafts remain unchanged. The change spans 62 reviewed paths.
- Validation: 247 focused checks; **5,225 portable and 5,225 natural-root checks**;
  29 fresh candidate builds; fresh root regeneration of 18 units/207 Lua and
  full reports, all byte-identical to the tested outputs. Paired smoke covers
  183 files with the same 16 existing problems, raw reports exact. Problem
  identities are recorded in `smoke_problem_inventory.json`; do not suppress
  the missing mock bindings or the two default-data modulo-zero cases.
- Fresh Beardy validation: 11,556 Main scenarios, 24 health sites with both
  outcomes, two retained termination guards with both outcomes and one separate
  supplied endpoint; 12,600 speech scenarios including cancellation and the
  persistent ring probe. All four tested Lua hashes match actual generation
  and root outputs. **Beardy remains excluded from the bundle.**
- Current ledger: **371 files, zero syntax failures**; generic temporaries 8,129,
  gotos 4,696, native names 6,380, machine temporaries 4,700, unresolved findings
  826. Other 332 current Lua/audit records remain unchanged.
- [W507 acceptance](../../work/codex_lua_label_aliases_w507_20261027/acceptance.json)
  contains source/receipt pins and the complete validation chain. Its folder
  contains corpus, bytecode, behavior, smoke, promotion preimages and root
  reproduction receipts. Historical Dragon report draft-path metadata was
  normalized only for comparison; its unrelated registered report was retained.
- **Latest verified offline bundle: W509 `label-alias-checkpoint-20261027`.**
  [Verification](../../work/codex_lua_label_alias_bundle_w509_20261027/verification.json):
  203 Lua parse, 208 payload hashes, 25 existing Lua payload updates plus README,
  182 other payloads exact; membership/override/DLLs fixed and parent W505
  unchanged. Twenty-five source changes remain outside fixed membership.
  Changed legacy pending records without an explicit older chain retain their
  verified parent source record; no historical transformation was invented.
  Sidecar DLL SHA256 remains
  `84bd17a61a10b90778aa85d9233c93a2d6eef041457cc7b1aa1826795eb03ca2`.

### Next-session entry point: W511, scratch only

[W510 inventory](../../work/codex_lua_jump_stub_inventory_w510_20261027/scan.json)
found 128 acyclic generated jump stubs with 189 incoming references.
[W511 qualification](../../work/codex_lua_jump_thread_w511_20261027/qualification.json)
proposes retargeting through immediate generated-label/goto stubs. Across 267
registered readable Lua, 37 files had proposals; **33 files passed the complete
stripped-bytecode retarget/pruning checks, with 117 proposed target edits and
85 proposed unused-label removals. None of W511 is promoted or integrated into
the workspace generator.** `propose_jump_threads.py` is a proposal routine;
`qualify.py` supplies its current compiler gate. Do not treat the proposal alone
as an accepted rewrite.

Four files were refused by the bytecode/visibility guard and remain unchanged:
ChickenMaster (chicken_kicking), BordelloLady (bordello), WomanToAttract
(sick_child), and FishermansWife (heros_old_house). Precise causes have not yet
been separated; preserve the refusals rather than weakening the guard to meet
a metric.

On an explicit resume:

1. Re-read the recovery/readability skills and this checkpoint. Verify the W507
   production pins and W509 manifest, branch/HEAD/index, and existing ownership
   holds before editing. Reuse completed accepted checks only with exact inputs.
2. Develop W511's production guard and meaningful scope/visibility/shadowing,
   cycle, close/GC/error, loop and return-arity tests. Retain unsupported cases.
   The current scan is not sufficient acceptance evidence.
3. If the rule qualifies, integrate it only at the appropriate final readable
   stage; preserve earlier APIs and faithful drafts. Run scratch generation,
   exact unrelated-unit/source comparisons, bytecode checks, paired smoke and
   the current audit before any promotion. Refresh relevant Beardy behavior
   whenever its sources change, and retain its bundle exclusion.
4. Preserve preimages and failed attempts, then update only accepted existing
   bundle members after final root reproduction. Do not expand membership.

### Separate retained ABI gate and boundaries

W499 verified that the existing debug-speech integer-set binding passes a modern
`std::set<unsigned long>` layout to a different retail node layout. The exact
bundle DLL, matching CodeView PDB and compiled source checksum were checked.
Original constructor/iterator/clear/destructor helpers passed 1,359 synthetic
BST cases, 20,442 visited nodes and 21,801 supplied normal node/head frees.
This is bounded original-helper evidence, not engine/physical-heap execution.

[W499 packet](../../work/codex_lua_unresolved_inventory_w499_20261027/evidence_packet.json)
and [W501 source-only adapter](../../work/codex_lua_unsigned_set_adapter_w501_20261027/prepared.json)
remain available. W501 has three owned scratch C++ candidate paths and is
**unbuilt/unapplied**. Native-profile guards, Win32 source probes and held SDK/ABI
integration gates remain. The three live debug-speech values stay unresolved.
Do not classify them as dead saved-table staging or use the current binding as
proof of a compatible container ABI.

Shared checkout: `feat/novi-script-recovery`, HEAD
`366c08e64a4323a6147fe4d5caa675e31799d51e`; held index SHA256
`4c196ad6220b1566291c343846c5a26af5eddbfd4cf85aa096ff0a7fa971e382`.
No staging, commits, index/ref changes, reset/stash/switch, game/install/desktop/
save/asset access, sidecar builds, launches or publication occurred. All other
owner/private/native/modern/Forge/evidence/runtime holds persist. Offline Lua,
mock and bytecode evidence does not certify current DLL ABI, physical lifetime,
engine reentry/unwind, saves or gameplay. Preserve CRLF and unrelated dirty work.

## Historical checkpoints ? superseded status, newest first

**ACTIVE until requested to stop; checkpoints do not terminate work.**

Current accepted Lua W503,current verified bundle W505;371audit syntax0,
gotos4696,nativeNames6446,generic8129,machine4700,unresolved826. Beardy excluded.
W507 adjacent-live-label prototype is in scratch validation,NOT promoted.
W506 inventory identified65clusters/75aliases/428goto references in39files,
18units,including Beardy entity Main and quest Main. New generic helper retargets
only identifier tokens of goto targets to the first adjacent live generated
label;all definitions remain until existing unused-generated-label pruning.
Only whitespace may separate unique live labels;comments,user labels,duplicate
names,opaque functions and environment hazards refuse.

39whole-file compiled stripped-bytecode comparisons pass before/after428target
retargets. Pruning75unused alias definitions also matches complete stripped
bytecode at original line coordinates;final blank-line deletion changes only
source/debug coordinates.247 focused Lua checks pass:normal/error/GC/close/arity,
while/numeric/generic-for/repeat/backward jumps,iterator closing,shadowed names,
protected labels,opaque text,cross-line goto comments and CRLF. No game execution.

Fresh relevant Beardy projection behavior rerun (not W483hash reuse):11556 Main
scenarios,24healthsites both results,2retained guard sites both outcomes plus
1separate supplied termination endpoint;12600speech scenarios preserve call,
RNG/frame/check/string order,returns,cancellation and persistent ring probes.
Helpers/harness/source pins explicit. Expected projection only;fresh generated
candidate outputs must still match all four tested Beardy source hashes.

Candidate builder invokes aliasing AFTER the prior late-unused-label prune,
then prunes newly unused aliases. This keeps early phases and prior conservative
scope-exit API unchanged. Broader tests and29fresh candidate builds are running.
Accepted W503fresh baselines are reused only with exact Lua/draft inputs and
report data,normalizing solely the recorded historical Dragon draft path.
No acceptance/promotion claim before these jobs,paired smoke,current audit and
natural-root reproduction finish. Work:
work/codex_lua_label_aliases_w507_20261027/validation_prepared.json;
bytecode_verification.json,callbacks.json,random_speech.json in that folder.

W499 integer-set binding ABI gate remains verified;W501 source-only adapter
unbuilt/unapplied,three live debug-speech values retained. No native/current DLL
ABI,physical lifetime,reentry,exception,save or gameplay certification. No new
conversion/registration/game/install/launch/asset/sidecar-build/publication/Git
action. All owner/private/native/evidence/runtime/index/ref holds persist.
Continue until the user asks stop;earlier entries remain historical checkpoints.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W503 transparent-label jumps ACCEPTED offline;W505 transparent-label-
checkpoint-20261027 verified. Five gotos and3unused generated labels removed
from3Lua in BanditCamp,HerosOldHouse and SummoningTheShip. Every other physical
line,all declarations,scope closers,live labels and calls retained. Default
strict scope-exit API exact across534draft/readable sources/1970function chunks;
only final builder stage opts into crossing intervening labels.11reviewed paths.

342focused checks (138new,including96close/GC/error/arity composition cases),
4978portable/root checks pass.29fresh candidate units/267Lua;264others exact.
26fresh report data records match baselines,with solely historical Dragon draft
path normalized for comparison;its registered report remains untouched. Exact
accepted generation inputs/Lua qualified;no fresh baseline regeneration claim.
Initial historical-path refusal retained. Fresh natural-root3units/34Lua/full
reports reproduce byte-for-byte. Paired smoke31files0problems/raw exact.
371current-script audit syntax0,gotos4701to4696,nativeNames6454to6446,
generic8129/machine4700/unresolved826 unchanged. Other368audit records exact.
All267faithful drafts unchanged. Beardy4Lua exact;W483behavior reused by exact
inputs,not rerun. Acceptance:
work/codex_lua_transparent_labels_w503_20261027/acceptance.json.

W505 refreshes one existing SummoningTheShip payload plus README from W500:
203Lua parse,208hashes,206other payloads exact. DLL/stock DLL/override/membership
fixed,parentW500 unchanged,Beardy excluded.24unbundled source changes retained:
BanditCamp existing source chain appended,FishermansWife newly retained outside
fixed membership. Five older records without chain fields retain exact verified
source pins;initial chain-schema refusal preserved. No new registration/member.
Verification:work/codex_lua_transparent_label_bundle_w505_20261027/verification.json.

Continue W506 adjacent-live-label qualification:read-only inventory267Lua,
65clusters/75potential aliases/428goto references across18units. Only whitespace
between unique generated labels with live in-function uses;no retargeting or
label removal performed yet. Beardy appears in3clusters,so any future accepted
rewrite needs fresh relevant behavior tests,not W483source-hash reuse. W499
integer-set ABI gate remains verified;W501retail-set adapter is source-only,
unbuilt/unapplied. Three live debug-speech values remain unresolved.

No native/current DLL ABI,physical lifetime,reentry,exception,save or gameplay
certification. No game/install/launch/asset/sidecar-build/publication/Git action.
All owner/private/native/evidence/runtime/index/ref holds persist. Continue
until user requests stop;prior entries remain historical checkpoints.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W499 read-only ABI evidence verified for existing Global_DebugCycleThroughSpeech.
Exactly3 current unresolved-value sites remain across267 registered drafts;
all consume the integer-set/text data and remain explicit. Native slot950 targets
8A0CB0;retail nodes are20bytes with color0,parent4,left8,right12,value16.
Current DLL84bd17... matched to its CodeView PDB at W339 sidecar_candidate;
GUID/age and PDB SHA256 source checksum match the actual LuaQuestState.cpp.
Compiled unsigned-set node typeA16F has left0,parent4,right8,color12,isnil13,
value16. The compiled wrapper passes std::set<unsigned long> directly to the
retail out API. This layout mismatch gates use of its Lua table snapshot binding.
Historical manifest sidecar_source points to a different build;the exact matching
CodeView/DLL/PDB/source profile was used for this finding.

Original constructor A394D0 RET8 requests20-byte head;iterator4292C0 cdeclRET0,
clear4CF810 RET4 and destructor4D0E20 RET0 exercised under supplied normal
allocator/free stubs.1359 synthetic BST cases,20442 visited nodes,21801 normal
node/head frees exactly once;callee-saved/ESP contracts checked. Modern-node
negative faults under the original iterator. No actual engine API or heap runtime
execution;balancing/insertion,reentry,exceptions and gameplay unverified.
Packet:work/codex_lua_unresolved_inventory_w499_20261027/evidence_packet.json.

W501 source-only adapter prototype retained in owned scratch:3 C++ candidate
paths,retail storage/owner,typed output pointer and wrapper traversal/cleanup.
Unbuilt/unapplied;native profile guards,Win32 source probe and SDK/ABI/runtime
gates remain explicit. Frozen SDK,current DLL and Lua untouched. No DLL build,
commit,index/ref,install,launch or publication. Source candidates:
work/codex_lua_unsigned_set_adapter_w501_20261027/prepared.json.

W502 loop-break qualification scan267Lua found0 candidates;no production rule.
W503 transparent-label opt-in scan267Lua finds5 removable jumps in3files:
BanditCampBossBattle1,FishermansWife2,STS_BriarRose2. Only labels and pure
if/do scope closers intervened. Default strict helper behavior stays unchanged;
all other physical lines retained until existing unused-label pruning.342 focused
checks pass,including96 new close/GC/error/return-arity composition cases.
Full source pins,candidate final builder and corpus validation are being prepared.
No W503 promotion. Current accepted Lua W498,current verified bundle W500,
371audit syntax0/unresolved826/gotos4701 unchanged. Beardy stays excluded.

Continue W503 broader tests and fresh candidate generation/comparison until
asked stop. No new conversion/registration/game/install/launch/asset/ABI-build/
publication/Git action. All existing lane/private/native/evidence/runtime/
index/ref holds persist;prior entries retained as historical checkpoints.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W498 existing GuildTraining melee saved-table staging ACCEPTED offline;
W500 guild-staging-checkpoint-20261027 verified. One reviewed operation changes
two comments only;nil assignment,terminal cleanup/return,all executable Lua
and line counts exact. Eight paths:unit metadata opt-in,draft/readable Lua,
three reports,two ledger files. Unit TODO59to58;371-file audit syntax0,
unresolved comment findings828to826;generic8129,gotos4701,nativeNames6454,
machine4700 unchanged. Other370 audit records exact. All267 draft sources and
267 registered readable sources qualified;266 others in each remain exact.
Fresh unrelated Book3drafts and legacy whole classification exact.

403 source proof checks pass. Baseline/candidate Guild38-file builds pass;
fresh root unit conversion4Lua/serialized report and fresh root38Lua/full
readable report reproduce exactly. Paired smoke37files0problems,raw reports
exact. Windows report draft-path formatting corrected against the completed
fresh build;original refusals/preimages retained. Source aliases/predicates,
195native epoch instructions,43call contracts and pre/post-hoist liveness checked.
Normal child ABI,in-bounds buffers,parent/frame disjointness remain explicit
premises;engine reentry,exceptions,heap,save,current DLL ABI/gameplay unverified.
Acceptance:work/codex_lua_guild_staging_w498_20261027/acceptance.json.

W500 updates one existing Guild payload plus README from W489 exact preimage:
203Lua parse,208payload hashes,206other payloads exact. Membership/override,
DLL84bd17a61a10b90778aa85d9233c93a2d6eef041457cc7b1aa1826795eb03ca2
and stock DLL fixed;parentW489 unchanged;23unbundled changes retained. Beardy
four readable Lua unchanged and excluded;W483behavior receipts reused by exact
source inputs,not rerun. Bundle verification:
work/codex_lua_guild_staging_bundle_w500_20261027/verification.json.

Continue W499 existing debug speech container/text evidence. The three residual
unresolved-value sites consume a native integer set/text IDs;do not classify them
as dead saved-table staging. Existing API manifest returns a Lua table;binding
source and retail node/iterator ABI need independent qualification before lowering.
No new conversion/registration/game/install/launch/asset/ABI-build/publication/
Git action. All owner/private/native/evidence/runtime/index/ref holds persist.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W498 Guild staging eight reviewed paths applied,final acceptance pending fresh
root readable reproduction. Metadata opt-in,one draft Lua,one readable Lua,
three reports,two ledger files. Only two comments annotate one dead saved-table
operation;nil assignment,cleanup call,return,all executable tokens and line
counts exact. No new conversion/registration. W496/W497 proof support accepted.

Scratch baseline/candidate Guild builds:38 Lua each,all syntax valid;baseline
matches registered Lua;other37 Lua/file reports exact. Paired bounded smoke:
37 files,0 problems each,raw reports identical. All371 current scripts audited:
syntax0,unresolved comment findings828 to826;one function TODO59 to58.
Other counts unchanged:generic8129,gotos4701,nativeNames6454,machine4700.
All370 other audit records exact. Fresh natural root audit matches the tested
ledger exactly;diff check clean. Fresh root unit conversion reproduces all4
owner Lua plus the serialized report exactly. Initial raw-Python-vs-JSON report
comparison refusal retained;explicit serialization comparison passes. Fresh
root38-file readable build still running;do not mark W498 accepted before it
finishes. Receipt folder:work/codex_lua_guild_staging_w498_20261027.

Latest verified bundle remains W489 late-label-checkpoint-20261027;it predates
this annotation. Beardy excluded. W499 read-only residual native inventory
retains three live container/text sites in existing DebugCycleThroughSpeech;
no blanket staging removal. Continue root reproduction then qualified existing
payload bundle refresh and further authorized Lua recovery until asked stop.

Normal child ABI,in-bounds buffers,parent/frame disjointness remain premises;
engine reentry,exceptions,heap/gameplay unverified. No game/install/launch/
asset/sidecar-build/publication/Git action. All owner/private/native/evidence/
runtime/index/ref holds persist. Prior entries are historical checkpoints.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W496 guarded saved-table classifier support ACCEPTED offline:27 new focused,
403 portable/root checks;fixture and fresh registered Book whole classifications
exact (4 aliases,48 call reviews each). Fresh Book conversion's3 generated Lua
files equal the registered drafts. Actual Guild original-image classification
passes195 native epoch instructions,43 call contracts and pre/post-hoist liveness;
one annotation,executable tokens identical. Source-only promotion:2 analysis
paths,no unit metadata,emitted Lua or diagnostics changed;all371 Lua hashes exact.
W497 terminal-closure proof remains accepted. Receipt:
work/codex_lua_saved_fallthrough_w496_20261027/acceptance.json.

W498 existing GuildTraining melee candidate conversion completed in scratch:
4 owners,14 functions,1 pre-existing missing body;all14 functions/4 owner files
parse. One annotation in TheRealGuildmaster Main;unit TODO59 to58;other3 owner
Lua exact and all executable draft tokens exact. Baseline/candidate full Guild
readable builds are running. No output/metadata/ledger promotion before readable,
paired smoke and audit qualification. W485 emitted Lua/W489 verified bundle
remain current,Beardy excluded. W499 read-only residual inventory finds4 current
unresolved-value sites:the pending Guild staging site and3 container/text reads
in existing Global_DebugCycleThroughSpeech. Those live values need separate
container/API/receiver evidence;no blanket dead-staging classification.

Continue until user requests stop. Normal ABI,in-bounds buffers,parent/frame
disjointness remain premises;engine reentry,exceptions,heap/gameplay unverified.
No new conversion/registration/game/install/launch/asset/ABI-build/publication/
Git action. All existing lane/private/native/evidence/runtime/index/ref holds persist.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W497 terminal staging proof accepted offline:28 new focused plus16 legacy,
376 portable/root checks. Prior helper AST unchanged. Actual Guild post-hoist
Lua has13 straight-line cleanup closures,40 terminal calls,no alias captures;
the staging write immediately enters terminal cleanup. Projection retains exact
source/line coordinates and checks both branch outcomes. No emitted Lua,
unit metadata or diagnostics changed;all371 Lua hashes unchanged. W485 syntax/
audit reused by exact inputs. Ordinary external callbacks,exceptions and engine
reentry remain unverified. Receipt:work/codex_lua_terminal_staging_w497_20261027/acceptance.json.

W496 guarded-fallthrough staging classifier is applied pending final acceptance:
403 portable/root checks pass,legacy fixture full classification exact (4 aliases,
48 call reviews). Actual original-image scratch classification passes:195 native
epoch instructions,43 independent call contracts,sealed predicate/source/Lua,
pre/post-hoist liveness,one annotation with identical executable tokens.
Fresh registered Book conversion/default comparison is running;no acceptance
claim until it completes. W498 existing GuildTraining melee unit candidate is
converting in scratch with the opt-in review descriptor. Production unit metadata,
draft/readable Lua,reports and diagnostics remain unchanged.

Continue scratch generation,behavior/syntax/report/corpus comparisons before
promotion. W485 emitted Lua and W489 verified late-label-checkpoint-20261027
bundle remain current;Beardy excluded. No game/install/launch/asset/ABI-build/
publication/Git action. All existing owner/private/native/evidence/runtime/
index/ref holds persist. Earlier entries remain historical checkpoints.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

W493/W494/W495 analysis changes accepted offline. Emitted Lua remains W485;
latest verified bundle remains W489 late-label-checkpoint-20261027; Beardy excluded.
W486 GuildTraining staging classification is active and still unresolved.

W493 preserves the default 4096 instruction return-analysis budget and supports
explicit bounded budgets. 17 focused,243 portable/root checks;104 registered plus
5 fixture default return contracts exact. The reached macro has 20435 instructions
and uniform normal RET16 under explicit 32768 budget; child calls remain opaque.

W494 independently profiles all43 epoch calls:29 direct,13 GSI,1 actor member.
24 focused,267 portable/root checks;96 legacy call contracts/reviews exact.
Three macro calls use the explicit budget;two distinct GSI reload equivalence
claims remain false. The 195-instruction cache proof retains coordinate60.

W495 models only exact bare do return end as a return edge.15 focused,324 portable,
348 root checks. All267 draft files/985 function chunks compared:801 prior models
exact,176 prior refusals exact,8 newly supported. Actual Guild alias is unused
across120 modeled nodes. Original source statements and line numbers retained.
No emitted Lua,unit metadata or diagnostics changed across these source checks.
All371 Lua hashes unchanged;W485 syntax/audit reused by exact inputs.

Receipts:work/codex_lua_return_budget_w493_20261027/acceptance.json;
work/codex_lua_call_profiles_w494_20261027/acceptance.json;
work/codex_lua_scoped_return_liveness_w495_20261027/acceptance.json.
Normal child balance,bounded buffers,parent/frame disjointness and target/type
contracts remain premises;no engine reentry,exception,heap or gameplay proof.
Continue native/Lua guard-shape qualification before classifying staging.
No game/install/launch/asset/sidecar-build/publication/Git action;all lane holds persist.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted emitted Lua W485and prior;latest verified bundle W489late-label-
checkpoint-20261027. Beardy UNBUNDLED. W492conditional cache isolation accepted;
W486staging diagnostic unchanged. W493normal-return budget validation active.**

W492checks verified switch CFG edges and exact declared parent Bool writes.
The new premise explicitly places declared parent fields outside caller frame;
normal child stack balance and bounded buffer writes remain assumptions.
35focused/227portable/root checks pass,including omitted/malformed write
contracts,wrong pointer origin/opcode/value,partial switch-case inventories,
direct frame writes,overlapping buffer/copy-ESP escapes and implicit call-return
address cache overwrite rejection.2analysis paths;no unit metadata/diagnostic/
emitted Lua changes. Existing registered proof row and legacy fixture both remain
byte-for-byte-equivalent proof data. All371Lua hashes unchanged;prior W485syntax/
audit reused by exact inputs. Source checks/Git identity/index unchanged.

Actual Guild195instruction/43call epoch,38declared buffers,two parent Bool
stores passes with cache/read coordinate60. Pointer load-site origins,exact
native byte writes,explicit/implicit frame writes and escape coordinates are
checked. Declared CCharString4/CScriptThing12/map12 in-bounds child storage and
parent/frame disjointness are not physical heap/lifetime/reentry/unwind/gameplay
proof. Opaque child contract identity/prefix/liveness checks still gate staging
classification. Receipt:work/codex_lua_cache_parent_writes_w492_20261027/acceptance.json.

W493scratch normal-return checker preserves default4096instruction cap;explicit
bounded integral budget supports the already-reached large macro callee.
Reverse return reachability keeps the same graph property in linear time.
17focused checks pass;actual20435instruction body0xcbfb7d..0xcd1f97 has uniform
normalRET16 under explicit32768budget.104registered/5fixture default return
contracts all prove identically.243portable/current proof checks running;
no W493production promotion yet. Children remain opaque normal-return edges;
this does not prove child balance,target identity,heap safety or termination.

Continue until user asks stop. W489bundle unchanged (203Lua/208hashes,
206otherpayloads exact,23unbundled,DLL/override/membership fixed,parentW487
unchanged). No new conversion/registration/game/install/launch/desktop/save/
asset/ABI-build/publication/Git action. All private/native/modern/Forge/evidence/
runtime/index/ref holds persist. Earlier checkpoints are retained history.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted emitted Lua: W485and prior;latest verified bundle W489
late-label-checkpoint-20261027. Beardy UNBUNDLED. W491parent-pointer provenance
accepted;W486cleanup classification continues through W492cache-write proof.**

W491adds explicit parent-pointer field snapshots with load-site identity;
no invented resource-table profile or equality across reloads.2analysis paths,
22focused/192portable/root checks;1194exported registered default native results
exact (1151analyses/43same refusals;35inherited external bodies absent from
exports explicitly excluded). Native GuildTraining parent flag stores0xd5a4d2
and0xd5a4db trace to member+0x14 and quest fields+0x54MeleeRepeating/
+0x55MeleeRepeatKnown. Metadata/diagnostics/emitted Lua unchanged;all371Lua
hashes exact,prior W485syntax/audit reused by exact inputs. No unit opts into
new parent tracking. Iterator-consumption regressions found in the scratch
prototype are fixed and preserved with2countermodels;never promoted.

Receipt:work/codex_lua_parent_snapshot_w491_20261027/acceptance.json.
W492scratch epoch extension checks verified switch edges and exact declared
parent Bool stores under an explicit parent-field/caller-frame disjoint premise.
Actual195instruction/43call epoch,38declared buffers,two parent writes,cache
coordinate60matches reload. Caller normal-return ABIs,in-bounds buffer writes
and disjoint parent spans remain premises;no physical heap/lifetime/reentry/
unwind/gameplay proof. Source native function/typed locals/field names are pinned.
W492negative/control/legacy validation next;no W492production promotion and
no staging diagnostic classification yet. Continue until user asks stop.

W490call-entry guard hardening and W486verified-switch support remain accepted;
W489packet unchanged (203Lua/208hashes,1existing update,206otherpayloads exact,
23unbundled,parentW487unchanged,DLL/override/membership exact). No new conversion/
registration/game/install/launch/desktop/save/asset/ABI-build/publication/Git
action. Preserve all owner/private/native/modern/Forge/evidence/runtime/index/
ref holds. Older checkpoints are retained history.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted emitted Lua: W485late unused labels plus W484and prior recovery.
Latest verified bundle: W489late-label-checkpoint-20261027. Beardy UNBUNDLED.
W486native cleanup evidence continues;W491parent-pointer snapshot candidate active.**

W485runs the existing token-aware generated-label rule after all late passes.
Two newly unreferenced WhiteBalverine labels disappear;every other physical
line,live jump/call,scope/declaration/annotation stays exact.96composition checks,
4840portable/root,29fresh builds,266otherLua/28reports exact;paired smoke11files/
0problems.267draft unchanged/534parse/full371audit exact. Gotos4701unchanged,
nativeNames6456->6454,unresolved828unchanged. Beardy4Lua exact;W483behavior
receipts are reused by exact source hashes,not rerun.6reviewed paths.

W489updates1existing member from accepted W485exact preimages.203Lua/208hashes,
206otherpayloads/DLL84bd17a6/stock DLL/override/membership exact,23unbundled.
ParentW487unchanged. No game/install/launch/desktop/save/asset/ABI-build/publication/
Git action. Receipt:work/codex_lua_late_label_bundle_w489_20261027/verification.json.

W486verified switch/provenance support ACCEPTED:3analysis-tool paths,no emitted
Lua or diagnostic changes,no registered unit opts in. Explicit original table
bytes,unsigned cmp/ja guard,aligned bounded case targets and guard entry checks
required. Default indirect-jump refusal stays.35focused/168candidate/164portable/
164root;1194exported registered default native analyses exact (1151success,
43same refusals),35inherited/external bodies absent from exports explicitly
excluded. Actual GuildTraining7case switch proves cache-store receiver origin.
All371Lua hashes unchanged;prior W485syntax/audit reused by exact inputs.

Further adversarial review found direct calls could bypass that new switch guard.
W490ACCEPTED fix:2analysis paths,7call-entry cases (4failed/3safe controls before
fix),171portable/root. Reject calls after compare or into mid-instruction;
aligned compare/case entries and external ordinary calls remain supported.
Actual Guild7case table still verifies. Prior1194default comparison explicitly
reused:analyzer and native inputs exact,table helper unused in default mode.
All371Lua/diagnostics/unit metadata/Git identity unchanged. Preserve red tests.
Initial acceptance text lookup refused the PowerShell UTF16redirected red log;
original BOM-qualified decoding fixes the lookup without changing/rerunning it.

Receipts:W485acceptance.json,W486switch_support_acceptance.json,W490acceptance.json.
W486cleanup placeholder remains unresolved. Last-cache epoch194instructions/
43calls has2explicit parent Bool stores (+0x54MeleeRepeating,+0x55MeleeRepeatKnown).
W491scratch parent-pointer load-site tracking will support their bounded write
contracts without inventing resource-table profiles. No W491promotion yet.
Keep normal MSVC/borrowed-pointer/in-bounds premises explicit;no engine heap,
vtable lifetime,reentry,unwind,save,current DLL ABI or gameplay certification.
No new conversion/registration. All private/native/modern/Forge/evidence/runtime/
index/ref holds persist. Continue until user asks stop;older checkpoints are history.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W484 and prior offline recovery. Latest verified bundle:
W487 scope-exit-checkpoint-20261027. Beardy remains UNBUNDLED.
W486GuildTraining melee cleanup receiver evidence investigation continues.**

W487refreshes2existing payloads from accepted W484exact preimages.203Lua parse,
208hashes match;205otherpayloads,DLL84bd17a6,stock DLL,override/membership exact.
23unbundled source records retained. ParentW482unchanged. W485initial zero-label
scan predates W484; rechecking2changed inputs now finds newly unused targets.
Preserve initial checkpoint refusal of the stale zero-candidate assumption.
No late label cleanup promoted yet. No game/install/
launch/desktop/save/asset/sidecar build/publication/Git action. Verification:
work/codex_lua_scope_exit_bundle_w487_20261027/verification.json.

W486native cache0xd5a1de dominates cleanup receiver reload0xd5a95c on all original
CFG edges,including7switch cases. Conditional stack proof2504instructions,
6slot overlaps yields receiver cache entryoffset-572always initialized at reload.
Ordinary callees and expected dynamic interface ABIs remain premises. An initial
GSI-only ABI hypothesis correctly failed9joins:0xd5a2e1uses actor EBP=script+8,
CScriptThing slot0x10c,not quest GSI. Corrected actor identity restores all joins.
Preserve rejected hypothesis receipt. Bounded linear+CFG callee scan20435native
instructions proves uniform explicit normalRET16for0xcbfb7d;no engine execution.
No production classification or diagnostic removal yet. Native dynamic target,
borrowed interface lifetime,reentry,unwinding and physical ownership remain
unverified. Continue generic evidence-backed recovery without a stop checkpoint.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W484 scope-exit jump pass plus W483/W481/W479 and prior
accepted offline recovery. Latest verified bundle remains W482
normalized-tail-checkpoint-20261027. Beardy remains UNBUNDLED.**

W484 removes3redundant jumps in2Lua/2units (WhiteBalverine and SickChild).
Every other physical source line is exact:all scope closers,declarations,
labels,annotations and calls retained. Unique label required;loop/function exits,
repeat conditions,duplicate labels,intervening code and opaque text refuse.
108focused/4744portable/root checks pass,including close/GC/error/arity/label
shadow controls.29fresh candidates:265otherLua/27reports exact. Paired smoke
24files/3retained old problems,raw reports exact. Beardy4Lua exact;W483Main11556,
random12600 and guard coverage are explicitly reused by source hashes,not rerun.
267draft unchanged/534Lua parse/full371audit exact;gotos4704->4701,unresolved828
unchanged.9reviewed production paths;Git HEAD/branch/index unchanged.

Receipt:work/codex_lua_scope_exit_w484_20261027/acceptance.json.
W485read-only existing late-label-pruning scan found0candidates in267Lua;
no new pass or production change. Sister modulo-zero default smoke result was
already explained by W463empty vector fixtures;retain that qualification.
Continue bounded existing recovery;no user stop request. No new native/engine/
physical heap/reentry/exception/save/current DLL ABI/gameplay proof. No new
conversion/registration/sidecar build/game/install/launch/desktop/asset/publication/
Git action. Preserve all owner/private/native/modern/Forge/evidence/runtime holds.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W483 normalization plus W481/W479/W477/W475/W473/W472/
W471/W469,offline. Latest verified bundle: W482 normalized-tail-checkpoint-20261027.
Beardy remains UNBUNDLED. W484 transparent scope-exit jump recovery continues.**

W483 normalizes25empty-then/nonempty-else guards in19Lua/12units. Branch bodies,
locals,close boundaries,labels,annotations/strings remain exact. Only complete
unary-not operands can cancel;annotated empty branches,closures and multiline
opaque text refuse.788focused/4636portable/root,Main11556/24healthsites,2retained
guard sites both outcomes+1separate supplied adversarial endpoint,random12600,
12unit smoke129files/10old problems pass.248other Lua/17reports exact;all counters
and diagnostics unchanged,267draft/534parse/full371audit exact. Main100gotos;
one empty-then shape is now a direct IsWearingHairstyle guard.

Initial behavior attempt ran before candidate copy completed and refused missing
files. Completion/source-hash qualification was added;final behavior inputs match
the completed generation receipt. An initial bundle command likewise ran before
its plan completed and refused missing prepared.json;final build ran after both
prerequisite exit statuses were checked. No production or bundle mutation from
those refused attempts. Preserve all raw evidence and initial attempt receipts.

W482 combines exact accepted W481/W483source chains into25existing runtime-member
payload updates.203Lua parse/208hashes match;182otherpayloads,DLL84bd17a6,stock DLL,
override/membership exact;23changed sources outside membership stay unbundled.
ParentW480unchanged. No game/install/launch/desktop/save/asset/ABI-build/publication/
Git action. Observations are bounded emitted-Lua/API/resource evidence,not native/
engine/physical heap/reentry/exception/save/current DLL ABI/gameplay certification.

Receipts:W483acceptance.json and W482verification.json in their owned work folders.
W484 scratch scope-exit jump matcher108focused checks pass;read-only scan2files/
3jumps,none in Beardy. Retain every scope closer/declaration/label;refuse loop/
function exits,repeat conditions,live intervening code and duplicate label names
(an inner label can shadow the apparent outer target). Full qualification next;
no W484promotion yet. All owner/private/native/modern/Forge/evidence/index/ref/
runtime holds persist. Older checkpoints are history;continue until user asks stop.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W481 tail/else regions plus W479/W477/W475/W473/W472/W471/
W469,offline. Latest verified bundle: W480 guard-region-checkpoint-20261027.
Beardy remains UNBUNDLED. W483 empty-then normalization active;W482refresh waits.**

W481 replaces72then-tail gotos with else regions across41functions/41Lua/20units.
Condition headers,then scopes,all live calls,declarations,strings,annotations and
target labels remain. No false-region outer declarations,body labels,closures,
opaque text,outer else or ambiguous scope boundaries admitted.66reviewed paths;
226other Lua/9unit reports exact. Source maps identify only72deleted tail gotos
and end->else delimiters;all other statements stay exact apart from indentation.

3848portable/root,187focused,Main11556/24healthsites,2retainedW479guard sites both
outcomes plus1separate supplied adversarial endpoint,random12600 pass.20unit smoke
184files/11old problems retained. All267draft unchanged/534parse/full371audit exact;
unresolved unchanged. Direct Beardy Main counters102->100;helpers/other3Lua exact.
Unique handle IDs,finalizable roots,__close failures,parent/end-of-block close,
then-shadowing and nested-loop controls pass. These are emitted-Lua/bounded API
observations,not native/engine/physical heap/reentry/exception/save/DLL/game proof.

Review exposed an empty-then form in one Beardy branch. It is semantically valid;
W483normalizes empty then/nonempty else before publishing any refreshed offline
packet.788focused checks pass,including finalizable condition values,short-circuit
evaluation,branch-local cleanup and close failures. Read-only scan19files/25guards
parse;refuse commented empty branches and unsafe unary-not cancellation. No W483
promotion yet. W482existing-member refresh will combine accepted W481/W483chains,
keep DLL/override/membership fixed and Beardy excluded. W480packet unchanged.

Receipt:work/codex_lua_tail_else_w481_20261027/acceptance.json. No conversion,
registration,ABI-build/install/launch/desktop/save/publication/Git action. All
private/native/modern/Forge owner,evidence,index/ref/runtime holds persist.
Older checkpoints below are history;continue until user asks to stop.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W479 guard regions plus W477/W475/W473/W472/W471/W469,offline.
Latest verified bundle: W480 guard-region-checkpoint-20261027.
Beardy remains UNBUNDLED. W481 bounded tail/else region investigation continues.**

W480 refreshes29existing member Lua payloads from exact accepted W479preimages.
203Lua parse/208payload hashes match;178otherpayloads,DLL84bd17a6,stock DLL,
override and membership are byte exact.16changed source scripts outside membership
remain unbundled;no new override or registration. ParentW478-final unchanged.
No game-side preflight/install/launch/desktop/save/asset/ABI-build/publication/Git
action. verification.json pins final bundle,parent,source receipts and all hashes.

W479 source acceptance:58paths/46functions/40Lua/13units,82gotos removed while
retaining all target labels,body/condition calls,strings,annotations/declarations.
3661portable/root,219focused,Main11556/24healthsites,2guard sites both outcomes
plus1separate supplied adversarial termination endpoint,random12600 pass.13unit
smoke159files/16old problems retained.227other Lua/16reports exact;267draft/
534parse/full371audit exact. Direct function-scoped counters show Beardy Main
104->102gotos;an earlier stale remaining-count estimate was rejected before
acceptance,retained as accept_initial_remaining_goto_assumption.py. Use pinned
beardy_metrics.json. This is not native/engine/physical heap/reentry/exception/
save/current DLL ABI/gameplay certification;endpoint is not physical scheduling.

W481 investigates existing then-branch tail jumps over a declaration-free false
region. Preserve then-local close/GC boundaries,all lookups/calls and target labels;
refuse existing outer else,label entry,opaque text,closures or ambiguous scopes.
No W481promotion yet. All private/native/modern/Forge owner,evidence,index/ref,
runtime holds persist. Older checkpoints are history;continue until user asks stop.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W479 guard regions plus W477/W475/W473/W472/W471/W469,offline.
Latest verified bundle: W478 goto-tail-checkpoint-20261027-final unchanged.
Beardy remains UNBUNDLED. W480 existing-payload refresh and next bounded region work continue.**

W479's token-aware conditional-skip matcher removes82gotos in46functions/40Lua/
13registered units. Target labels,all original condition/body call text,strings,
annotations and declaration lines stay. No moved outer locals,body label entry,
opaque text,closures or scope boundaries are admitted. Nested body scopes stay
nested.58reviewed paths;227other Lua/16unit reports byte exact. Source-provenance
maps retain every original line and identify82rewritten condition headers.

3661portable/root tests,219focused,Main11556/24healthsites and random12600 pass.
Both rewritten Beardy Main conditions exercise true/false in both versions;
one separate supplied adversarial termination endpoint fills a missing canonical
outcome. That is not a physical scheduling claim. All Main calls,state,cleanup,
health decisions and mapped condition outcomes match. Beardy Main104->102gotos;
helpers/other3Lua byte exact.13unit smoke159files/16existing problems retained,
including diagnostic shifts mapped to exact original statements/proved headers.
All267draft unchanged/534Lua parse/full371audit exact;unresolved count unchanged.

Proof is lexical emitted-Lua control/nearest-scope preservation plus bounded
truthiness,multiple-return,error,GC,__close,loop/break/backward-jump observations.
No native/engine/physical ownership/reentry/exception/save/current DLL ABI/game
certification. Receipt:work/codex_lua_guard_region_w479_20261027/acceptance.json.
W480 must update only changed existing W478payload members from exact tested
preimages;keep DLL/override/membership fixed and Beardy excluded. No conversion/
registration/ABI-build/install/launch/desktop/save/publication/Git action. All
owner/private/native/modern/Forge/evidence/index/ref/runtime holds persist.
Older checkpoints below are history;continue until user asks to stop.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W477 opaque-control guard plus W475/W473/W472/W471/W469,
offline. Latest verified bundle: W478 goto-tail-checkpoint-20261027-final.
Beardy remains UNBUNDLED. W479 bounded guard-region recovery continues.**

W478 refreshes9existing member Lua payloads/23unreachable call-return leaves from
exact accepted W475preimages.203Lua parse/208payload hashes match;198otherpayloads,
DLL84bd17a6,stock DLL,override and membership are byte exact.8changed source
scripts outside membership remain unbundled;no new override or registration.
ParentW474 unchanged. No game-side preflight/install/launch/desktop/save/asset/
ABI-build/publication/Git action. Source checkpoint W477 changes no generated Lua.

Initial scratch bundle selected W470's6entry unbundled ledger instead of parent
W474's8entry ledger and failed an assertion. Keep original builder/incomplete
folder(marker INCOMPLETE.txt). Final builder validates current parent ledger before
copying and writes a separate -final folder. verification.json names the verified
bundle and source receipts;do not treat the incomplete scratch attempt as accepted.

Source validation:W4753425portable/root,40focused,Main11556/random12600,8unit smoke
109files/2old unknown gaps retained. W47717focused/3442portable/root,29fresh builds/
7767actual fold calls/0opaque inputs;all267Lua/29reports/ledger unchanged. W475
behavior receipts reused only under exact input pins,not rerun for W477. All267
draft unchanged/534parse/full371audit exact. Emitted-Lua observations and supplied
API/resource contracts are not engine/current DLL ABI/physical ownership/reentry/
exception/save/gameplay proof. Original evidence and prior qualifications remain.

W479 scratch token-aware conditional-skip matcher retains target labels and all
lookup/call text;refuses moved outer declarations,body label entry,opaque text,
closures or scope boundaries.219focused truthiness/error/arity/GC/__close/loop/
backward-jump checks pass;40files/82regions parse in read-only scan,including2
Beardy Main regions. No W479promotion yet;continue full source/corpus/behavior/root
qualification. All native/modern/Forge owner/private/evidence/index/ref/runtime
holds persist. Older checkpoints below are history;continue until user asks stop.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W477 opaque-control guard plus W475/W473/W472/W471/W469,
offline. Latest verified offline bundle: W474 literal-query-checkpoint-20261027.
Beardy remains UNBUNDLED. W478 offline tail refresh/W479 guard-region work continue.**

W477 fixes a valid synthetic Lua bug:fold_goto_else treated code-like text inside
multiline strings/comments as executable structure and changed literal contents
through full style_function. That fold now refuses opaque multiline tokens.
17focused direct/full-style checks and3442portable/root tests pass. Two source/
test paths only;all267supported Lua/29reports/ledger unchanged.29fresh builds
recorded7767actual fold inputs,0opaque inputs;no current port output affected.
All267draft unchanged/534Lua parse/full371audit exact;no Flow grammar change.

Completed W475Main11556/random12600 and8unit smoke109files/2old mock gaps are
reused only through exact input hashes and source receipt pins. Those scenarios
did not rerun for W477. No new native/engine/physical ownership/exception/reentry/
save/current DLL ABI/gameplay claim. Synthetic corruption artifacts and old bad
outputs remain in work/codex_lua_literal_control_audit_w477_20261027/;acceptance.json
pins the actual corpus,source snapshots,root verification and qualification.

W478 must refresh W475changed existing payloads from exact tested parent preimages
only;DLL/override/membership fixed,Beardy unbundled,no install/launch/publication.
W479 investigates bounded conditional-jump regions with no moved outer locals,
no label entry into the wrapped body and original lookup/cleanup ordering retained.
Do not infer native behavior from parseability. All native/modern/Forge owner,
private,evidence,index/ref/runtime holds persist. Continue until user asks stop.
Older checkpoints below are history.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W475 unreachable goto tails plus W473/W472/W471/W469,offline.
Latest verified offline bundle: W474 literal-query-checkpoint-20261027 unchanged.
Beardy remains UNBUNDLED. W477 opaque-literal control safety audit continues.**

W475 removes30adjacent unreachable call/return leaves in11functions/11files/8units,
through one strict generic late matcher. All jumps/labels/comments/control and
declaration lines stay exact;no scope moves or shared CFG projection. Native/
generic/goto/machine/unresolved counts are unchanged;no diagnostic hidden. Only
immediately adjacent modeled leaves after a standalone unconditional goto qualify;
labels,declarations,comments,controls or unknown syntax end the match.24paths.
3425portable/root tests,40focused,Main11556/random12600 pass. Eight changed-unit
smokes109files/2existing problems retained:TourGuide MsgIsRegionUnloaded and
Bordello SetCutsceneSkippableWhilePaused. All267draft unchanged/534Lua parse/
full371audit exact;256other readable Lua/21unit reports byte exact.

Actual11input/output pairs parse and preserve all annotations/jumps/scopes;
40focused checks include Lua implicit __close cleanup,finalizable roots,error/
return arity,label entry and opaque/invalid syntax refusal. This proves bounded
emitted-Lua behavior only;no new native/engine/physical ownership/exception/reentry/
save/current DLL ABI/gameplay proof. Draft/native evidence stays unchanged.
Initial smoke read GuildTraining while its generator was incomplete;retain
smoke_initial_partial_corpus.py and smoke/. Final smoke_after_corpus requires all
completed unit receipts and replaces that incomplete validation attempt.

Receipt:work/codex_lua_goto_tail_w475_20261027/acceptance.json. W476 read-only
adjacent-target scan found0matches across267files;no conversion/source change.
W477 synthetic valid Lua shows older fold_goto_else mutating code-like text inside
a multiline string,including through full style_function. Scratch refusal guard
for multiline string/comment tokens passes17focused direct/full-style checks;
qualify all registered generation inputs and outputs before promotion. Current
registered outputs are not yet shown affected. No new conversion/registration,
ABI-build/install/game/desktop/save/publication/Git mutation;all owner/private/
native/modern/Forge/evidence holds persist. Older checkpoints below are history.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W473/W472 query-chain recovery plus W471 guards/W469 retry,
offline. Latest verified offline bundle: W474 literal-query-checkpoint-20261027.
Beardy remains UNBUNDLED. W475 unreachable-goto-tail recovery continues.**

W474 refreshes2existing Bordello Lua payloads through accepted W472/W473 exact
tested preimage chains.203Lua parse/208payload hashes match;205otherpayloads,
NoviCompatibility.dll84bd17a6,stock DLL,override and membership are byte exact.
README and manifest record current qualifications.8changed scripts outside
membership remain unbundled(including all3changed Beardy files);no new override,
registration or runtime package membership. ParentW470 is unchanged.

W473 source acceptance:15paths/5exported Boolean query functions/3Lua/2units,
3385portable/root and1525focused tests. W472 source:11paths/3exported integer-
flag query functions/3Lua/2units,1860portable/root and936focused tests.
Main11556/random12600 cases,all267readable/267draft/534parse/full371audit exact.
Beardy smoke3files/0problems,Bordello9files/1existing unknown
SetCutsceneSkippableWhilePaused retained. Native comparisons execute original
helper bytes under supplied normal API/string contracts only;no engine bodies,
physical ownership,exception,reentry,save,current DLL ABI or game-runtime proof.

Receipts:work/codex_lua_query_bundle_w474_20261027/verification.json and source
acceptances W472/W473. No game-side preflight/install/launch/save/desktop/asset/
ABI-build/publication/Git action for this refresh. W475 scratch lexical tail
matcher preserves all jumps/targets/labels/comments/declarations/scopes and
refuses unknown syntax;40focused checks pass,11files/30adjacent unreachable
call/return leaves found read-only. Continue full staged qualification before
promotion. All private/native/modern/Forge owner and evidence holds persist.
Older checkpoints below are history;continue until user asks to stop.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W473 hair/tash Boolean-query chains plus W472/W471/W469,offline.
Latest verified offline bundle: W470 readable-flow-checkpoint-20261027 unchanged.
Beardy remains UNBUNDLED. W474 existing-payload offline refresh continues.**

W473's strict generic Boolean-construction query matcher changes5existing exported
functions in3Lua files/2units(Beardy and Bordello).130private Boolean flag stores,
23empty flag tests and5gotos are removed. Every local declaration, nested hero
handle scope, ordered lookup/query and single Boolean return is retained. No
lookup is hoisted and no resource ownership operation is removed.15reviewed paths.
Beardy Main/GetRandomSpeech/entity file are byte exact;264other registered readable
Lua/27unit reports exact.3385portable/root tests,1525focused,Main11556/random12600
pass. Beardy smoke3files/0problems;Bordello9files/1existing unknown
SetCutsceneSkippableWhilePaused problem retained. All267draft unchanged/534Lua
parse/full371audit exact;unresolved diagnostics remain explicit.

Three original native bodies:BeardyTash478bytes@e538f0,OddHair402@e53ad0 and
BordelloTash478@e44cc0 with original literal operands/interface+40 are pinned.
320bounded native cases compare all truth masks under stable/changing borrowed
hero pointers with all5Lua copies(512comparisons);3semantic result-store controls
complete normally and differ. Supplied normal string/API callees,ESP/saved-register/
volatile-clobber/reverse-destructor checks only;no engine/string implementations,
physical ownership,exception,reentry,save or game execution proof.

Receipt:work/codex_lua_beardy_boolean_queries_w473_20261027/acceptance.json.
All5portable fixtures match the source-pinned native comparison inputs. Two
previously exposed quest-file duplicates and registered Bordello helper were
included after the full inventory scan;no new conversion/registration was added.
W474 must refresh only changed existing W470 payloads from exact tested preimages;
keep DLL/override/membership fixed and Beardy unbundled. No install/game/desktop/
save/ABI-build/publication/Git mutation;all private/native/modern/Forge owner and
evidence holds persist. Continue until user asks to stop. Older checkpoints below.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W472 beard query chain, W471 guards and W469 retry, offline.
Latest verified offline bundle: W470 readable-flow-checkpoint-20261027 unchanged.
Beardy remains UNBUNDLED. W473 existing hair/tash query recovery continues.**

W472's generic strict complete-shape pass simplifies Beardy IsHeroWearingBeard
and both exported Bordello helper_E44A40 copies.24closed uint8 flag stores,
21integer mask groups and3gotos/labels are removed. Every local declaration,
nested hero-handle scope and ordered getter/query
is retained; final query is explicitly coerced to one Boolean return. No lookup
is hoisted and no resource ownership operation is removed. Main/GetRandomSpeech
and other3Beardy Lua are byte exact;264other registered readable Lua/27unit reports
exact.1860portable/root checks,936focused,Main11556/random12600 pass. Beardy smoke
3files/0problems;Bordello9files/1existing unknown SetCutsceneSkippableWhilePaused
problem unchanged.11reviewed paths.All267draft unchanged/534Lua parse/full371audit exact.

Raw632bytes at0xe53670,8constructor literal addresses/values and two actual entity
callsites are pinned. Receiver is entity[0x14] parent quest;helper uses parent+0x40
interface,distinct from entity+4.512bounded Unicorn cases compare all256truth
patterns with stable/changing hero addresses against baseline/candidate Lua;
2semantic result-store controls complete normally and falsify behavior. Additional
Bordello632bytes at0xe44a40/8literals are pinned;512native cases compare both
exported copies(1024Lua comparisons),608extra root/truthiness/error cases and
2semantic result-store controls pass. ESP,
callee-saved registers and reverse construction cleanup checked under supplied
normal string/API stubs with volatile poisoning. No actual engine/string bodies,
physical ownership,exception,reentry,save or game execution is certified.

Receipt:work/codex_lua_beardy_beard_predicate_w472_20261027/acceptance.json.
Initial next-header-comment refusal is preserved;final matcher preserves trailing
native helper headers byte exact and still refuses body diagnostics/unknown effects.
W473 read-only evidence captures registered tash6queries/478bytes and odd-hair5/
402bytes;strict scratch Boolean-query candidates pass610focused tests and192
native/Lua normal-contract cases plus2semantic controls. Not promoted yet;
continue staged corpus/source/root qualification. No new conversion/registration,
ABI/DLL build,install/game/desktop/save/publication or Git mutation;all private,
native/modern/Forge owner and evidence holds persist. Earlier checkpoints below.

**ACTIVE until requested to stop; checkpoints do not terminate work.**

**Accepted source: W471 guards plus W469 structured retry,offline. Latest verified
offline bundle: W470 readable-flow-checkpoint-20261027. Beardy remains UNBUNDLED.**

W470 refreshes14existing Lua payloads only after exact tested parent preimage and
current source checks. All code/comment/literal/semicolon tokens equal the tested
W467 layout projection.203Lua parse/208manifest hashes match;193otherpayloads,
NoviCompatibility.dll84bd17a6,stock DLL,override and membership are byte exact.
README changes and manifest links W467/W468/W469/W471 receipts. Six changed source
scripts outside existing membership remain unbundled;no new override/registration.
ParentW462 is unchanged. No game-side preflight,install,launch,save,desktop,asset,
build,publication or Git operation was performed for this offline refresh.

Bundle and receipt: `work/codex_lua_offline_refresh_w470_20261027/verification.json`;
payload directory `readable-flow-checkpoint-20261027` alongside it. These are syntax,
hash,source/token-correspondence checks,not current DLL registration/engine/ABI/
gameplay proof. W4711532root/staged and W4691517root/staged plus native-byte/behavior
receipts retain their exact qualifications. SharedHEAD366c08e/branch/index unchanged.

Continue the next bounded existing Beardy helper readability target,including
native branch/constructor evidence and lookup/resource lifetime preservation.
Keep all private evidence,failed attempts and other-owner/ref/index/runtime/
publication holds. Earlier source/bundle checkpoints below remain history.

**ACTIVE until the user requests a stop. Checkpoints do not end work.**

**Accepted local source: W471 conservative initial-flow admission guards,offline.
W469 structured retry remains accepted. W470 offline bundle refresh continues;
latest verified bundle is still W462 until its new receipt closes. Beardy unbundled.**

W471 corrects two valid synthetic unsupported-input bugs in the new initial-literal
pass:an initialized shadow could hide a later outer read,and an opaque compound
loop prefix could hide a prior populated-slot definition. The accepted Beardy
transformation has neither shape. New guards count initialized/uninitialized locals
and parameters,exclude nested functions,and permit only modeled simple prefixes.
Unsupported input now remains byte exact. No shared Flow grammar change.

15focused/1532portable/1532natural-root checks pass. All267supported pre-initial
phase files/985function chunks replay with identical output/proofs;the one clean
Beardy proof remains. Fresh natural-root Beardy/report and all267registered outputs
match W469,267draft unchanged,534parse/full371audit exact. No generated Lua,
report or ledger was changed. Four reviewed paths:module,test,two fixture copies.
W469 native-byte/behavior evidence remains inherited,not reexecuted for these
admission-only guards. Both original bad inputs/outputs and audit are retained.

Receipts: `work/codex_lua_initial_flow_safety_w471_20261027/acceptance.json`,
`audit.json`,`corpus_qualification.json`,`root_tests.json`,`root_verification.json`.
W470 current plan:208parent hashes,14matchingexistingLua updates,6unbundled,
0preimage mismatches. Preserve DLL/override/membership;offline-only refresh.
No game/install/desktop/save/publication/Git action. All other-owner/private-source/
evidence/runtime/ref/index holds persist;sharedHEAD366c08e/branch/index unchanged.
Continue the bundle and bounded Lua recovery. Earlier checkpoints are history.

**ACTIVE until requested to stop. Accepted checkpoints are progress updates.**

**Accepted local source: W469 structured random-speech retry loop, offline.
W471 conservative initial-flow guards and W470 offline payload refresh continue.
Latest verified offline bundle: W462; Beardy remains UNBUNDLED.**

W469 replaces the complete GetRandomSpeech emitted jump shape with a structured
while/retry/return through a generic matcher. All reachable random/frame/termination/
string calls,history writes,local scopes and return expression/arity are preserved.
Only graph-proven unreachable tails are removed. Four gotos removed;Beardy Main
and other3Lua byte exact. Full29unit corpus:one exact generic projection/266Lua
and28unit reports exact;all267draft unchanged,534parse/full371audit exact.
Beardy now105jumps/126native names/78generic occurrences;still needs readability.
1517portable/root checks,Main11556 and randomspeech12600 bounded cases pass;
Beardy smoke3files/0problems exact. Eight reviewed/changed source/report/ledger paths.

210 original helper bytes at0xe53cb0,unit helper identity and history offsets1c/20
are pinned.12600 bounded Unicorn cases plus4endpoints match baseline/candidate Lua
result,state and call order;6semantic byte mutations complete and falsify behavior.
Synthetic normal thiscall/callee-saved/volatile-clobber/CRT/frame/termination/string
contracts supply every external callee.3string destructor calls/hidden-result/stack/
saved registers are checked per case. This does not execute actual engine/CRT/string
bodies or certify current heap,exceptions,reentry,DLL ABI/gameplay. History is
per-entity storage across helper calls,not proven game-save serialization;see
field_scope_qualification.json. No new runtime override or installation.

Receipts: `work/codex_lua_beardy_retry_loop_w469_20261027/acceptance.json`,
`native_comparison.json`,`root_verification.json` and the pinned raw reports/logs.
Initial generic-name test replaced the substring me inside NewScriptFrame;the
failed fixture/log are retained and token-aware rename fixes only the test.

W471 audit found2 valid synthetic unsupported inputs mishandled by the generic
initial-literal pass:initialized local shadow and opaque compound-loop prefix.
The accepted Beardy Main input has neither. Stronger scratch guards count all
local declarations/parameters,refuse nested functions,and reject opaque prefixes.
15focused controls and1532portable tests pass;267pre-initial input files/985function
chunks replay with every supported output/proof exact. No W471 promotion yet;
continue root closure of the guard fix before expanding that pass.

W470 offline plan verified all208parent payload hashes:14 matching existing Lua
updates,6changed scripts outside membership,0preimage mismatches. Bundle must keep
DLL/override/membership fixed;no install/launch/publication. Preserve all evidence,
failed attempts,private source,other-owner/index/ref/runtime/publication holds.
SharedHEAD366c08e/branch/index unchanged. Earlier checkpoints below are history.

**ACTIVE until the user requests a stop; checkpoints do not end the work.**

**Accepted local source: W468 initial-nil literal cleanup, offline. W469 existing
Beardy GetRandomSpeech retry-loop investigation is already active. Latest verified
offline bundle remains W462; payload refresh pending; Beardy remains UNBUNDLED.**

W468 removes one dead initial `scratchValue = false` from Beardy Main through a
generic pass after layout/final naming. Every incoming path must still hold the
unique local's initial nil and no read may precede redefinition. No host return
type is assumed. Captures, introspection/environment, ambiguous/unsupported flow,
duplicate labels, comments and populated/unknown slots are preserved. The second
dead false store follows host values and remains intact. 30 focused/1430 portable/
1430 natural-root checks pass; Main11556 and random-speech12600 observations exact.
29-unit corpus: one exact single-store projection,266Lua and28unit reports exact.
Beardy smoke3files/0problems both modes;all267readable equal candidates,267drafts
unchanged,534Lua parse,371file audit exact(syntax0/unresolved828). Generic-local
occurrences80->79;native names130/jumps109 retained. Seven reviewed/changed paths,
including JSON/Markdown ledger. No native/lowering/Flow/resource/ABI change.

Current receipts: `work/codex_lua_beardy_boolean_staging_w468_20261027/acceptance.json`
and `root_verification.json`; the capsule records matching accepted W467 baseline
reuse plus29 fresh candidate builds and all raw logs/reports. A copied promotion
driver initially expected an obsolete regressions.json,refused before writing,
and premature root checks also refused;originals retained. Corrected driver checks
actual1430portable receipt and snapshot pins;all final gates pass. An earlier
focused fixture raced mutable regeneration;it now uses an immutable corpus copy.

W469 owned scratch: `work/codex_lua_beardy_retry_loop_w469_20261027/`. Registered
helper0x00e53cb0 and persisted fields0x1c/0x20 are anchored;210 original bytes,
typed-decompile and exact Lua chunk captured. Branch bytes show duplicate draws
yield then check termination;accepted draws check termination before and after
history stores. Next: generic structured retry/return proof preserving call order,
return arity,state writes,lifetimes and cancellation,with adverse/native-byte/Lua
comparisons before promotion. No W469 source change yet. Continue that work.

Preserve all current private evidence,failed attempts,source freezes,other-owner,
runtime/publication/ref/index holds. SharedHEAD366c08e/branch/index unchanged;
no install/game/desktop/save/publication/Git mutation. No new engine/DLL/native ABI/
physical heap/reentry/exception/gameplay certification. Earlier checkpoints follow.

**ACTIVE: the user reaffirmed on 2026-10-27 that work must continue until they request a stop. Checkpoints are progress updates, not stop conditions.**

**Accepted local source: W467 token-preserving compound-branch layout, offline.
W468 initial-nil literal cleanup is in scratch validation. Latest verified offline
bundle remains W462; a new payload refresh is pending. Beardy remains UNBUNDLED.**

W467 expands 33 compound branches across 20 Lua files/15 existing registered units
using physical whitespace only. Every code token, comment, literal and semicolon
is exact; 247 other registered readable Lua files are unchanged. Runs after
reviewed phase-sensitive passes and final helper naming. Beardy Main and
GetRandomSpeech now pass the unchanged Flow grammar, including explicit terminal
branches. W466's full review/phase/unit/manifest scalar guard remains intact;
no native operand, resource cleanup, yield, return or state operation was removed.

Receipts: `work/codex_lua_beardy_flow_layout_w467_20261027/acceptance.json`,
`root_verification.json`, `portable_tests.json`, `corpus_final/verification.json`,
`corpus_comparison.json` and `smoke_full_verification.json`. 1400 final portable
and 1400 natural-root tests pass. Main11556 scenarios/24healthsites/bothoutcomes
retain prior observations. GetRandomSpeech12600 scenarios retain query/random/
frame/string/return order, cancellation and persistent history probes. Full changed
unit smoke: 15 units/156 files per mode/16 existing problems,
all corresponding results exact; source-token maps qualify diagnostic line shifts.
267 registered readable outputs match candidates,267 drafts unchanged,all534Lua
parse;371file audit matches ledger,syntax0/unresolved828. 39 changed paths from40
reviewed preimages;Markdown ledger unchanged. Shared HEAD/branch/index unchanged.

Initial full58pipeline receipts are retained. A comparison caught report lines
preceding bsim's extra helper-name comments; layout moved after final naming and
29 fresh final candidate builds verified all267Lua exact to the first candidates.
The29 fresh baselines were reused only after all source/evidence pins matched.
Two portable fixture-root failures and the first one-branch assumption are
retained with their corrections; no failure was relabeled as success. Current
final proof rows exactly match source projection on the final helper-named input.

W468 owned scratch: `work/codex_lua_beardy_boolean_staging_w468_20261027/`.
30 focused checks pass, including finalizable-slot retirement and unknown host
return/error controls. Candidate removes one initial false store in Beardy Main
only when every incoming path still holds initial nil and no read precedes a
redefinition. The second dead store has prior host definitions and is retained.
Scratch Beardy4Lua parse,other3Lua unchanged,W466 guard intact;whole Main,
portable1430tests and29unit corpus validation continue. No W468 promotion yet.

No new DLL/native ABI/current engine lifetime/gameplay proof. Preserve all private
evidence, old/failed attempts, source freezes, runtime and publication holds.
No Git staging/index/ref/commit, installation/game/desktop/save or other-lane action.

Earlier checkpoint material below is retained history; this active pointer governs.

**ACTIVE at the user's explicit instruction on 2026-10-27: "Proceed with lua until asked to stop".**

**Accepted local source: W466 integer zero-store cleanup, offline. Latest verified
offline bundle: W462; Beardy remains UNBUNDLED.** W466 closes the pending W464/W465
hypothesis: 24 unread zero stores to addNewConversation removed from the existing
registered BB_BeardyBaldyMan.Main (0x00e50fd0), through a guarded readable pass.
Complete review/phase/unit-metadata/manifest pins and scalar liveness proof are
required; defaults and unsupported inputs preserve source. The analysis-only
compound-jump projection never changes emitted calls or assignments. Global Flow
grammar, native lowering, drafts, resource clears and prior lifetime guards remain
byte exact. Beardy is 1058 -> 1034 lines, still needs-readability-work, with
109 jumps/130 native names/80 generic-local occurrences retained.

W466 work/receipts: `work/codex_lua_beardy_scalar_w466_20261027/`;
`acceptance.json`, `root_verification.json`, `root_tests.json`,
`registration_guard/validation.json`, `corpus_comparison.json` and
`smoke_verification.json` are completed receipts. Eight changed paths were applied
from nine exact reviewed paths (Markdown audit ledger is unchanged). The stricter
complete-registration guard passes 1360 staged and 1360 natural-root tests.
11556 whole Main scenarios retain prior observations/query order and both outcomes
at all 24 health sites. Fresh 29-unit baseline/candidate corpus: 267 readable Lua,
one exact 24-line projection, 266 unchanged; all 28 unrelated unit reports exact.
The stricter guard has identical Beardy Lua/reports and is unreachable in the other
28 units. Fresh natural-root generation/report equal the tested candidate, all
267 registered readable outputs match, 267 drafts unchanged, 534 Lua parse, and
the full 371-file audit matches its ledger (syntax 0/unresolved 828). Beardy smoke
is exact: three files per mode/zero reported problems. Existing other-unit smoke
problems, including Sister's default modulo-zero failure, are not reclassified.

This relies on the fixed normal Quest:AddNewConversation integer-return contract
from the pinned manifest and W465's recorded source; no new DLL registration,
native ABI, engine acquisition/current lifetime, exception/reentry or gameplay
proof. W461/W458 native evidence is inherited unchanged, not reexecuted here.
No install, game/desktop/save action, publication, Git staging/index/ref/commit
or other-lane work. HEAD/branch/index were checked unchanged. Preserve all source
freezes, private evidence, failed attempts and coordination reservations.

Next gate: select a bounded remaining Beardy control-flow shape and recover its
native branch/call/cleanup correspondence before changing structure. Do not widen
Flow grammar or relax scalar/resource lifetime admission. Future changes to this
function's pre-cleanup phase or complete unit metadata intentionally disable W466
until its review is renewed. W462 needs no payload update: Beardy is absent.

The October 24 pause/resume material below is retained historical evidence;
the explicit October 27 resume and W466 checkpoint above govern current Lua work.

**PAUSED at the user's explicit request on 2026-10-24.** Current receipts and retained
failures: [owning journal](../journal/2026-10/LUA_RECOVERY_2026-10-24.md).

**Accepted local source: W461 readable nil-only local cleanup, offline. Latest
verified offline bundle: W462. Beardy remains unbundled.**

W461 removes82 inert locals across15existing readable files/eight registered
units. Complete token/lexical accounting requires zero reads/non-nil writes;
populated resource clears, ambiguous bindings, annotations/continuations,
environment/introspection and unsupported text are preserved. Runs after final
naming; no native/API/control-flow/diagnostic or draft change. Beardy removes24
slots:1063->1058lines,temporary occurrences128->80;109jumps/130native names
retained,still needs-readability-work. Shared generator/portable tests/outputs/
reports/ledgers applied through28exact reviewed preimages.

Canonical41 plus1257readability regressions:1298pass under both staged and
natural-root imports. Fresh29-unit baseline/candidate READABLE corpus267Lua:
15exact explained source projections,252unchanged. Fresh root eight changed
units/reports match;all267registered readable outputs match,all267draftLua
unchanged,all534current Lua parse. Full371file readability audit matches ledger:
syntax0/unresolved828. Strict Beardy Init/Main11556scenarios cover all24health
sites/both outcomes with exact observations/query order and prior golden hashes.
Eight-unit smoke76files per mode retains14existing problems (unknown bindings
and Sister's default-data modulo-zero failure). Only error location73->72 changes
on the exact unchanged statement;messages/status/traces/results correspond.
Raw reports and initial checker failure are preserved;no new gameplay claim.

W461 retains accepted W458 native/lifetime checkpoint:24Beardy health scopes
use existing ResourceThingHealthAbove after frame/copy/native/predicate and
96individual/96composed liveness proofs. One inverse query0xe52447 independently
replays W455's ordered false/unordered comparison before folding. W453 rejection
and default guards remain intact. W458 primitive52/frame17/bounded-window8,
staged/root1287+4subtests;actual32helper8448Main scenarios/122144calls;prior33792
original native paths/14live regions reverified by pins/bytes,not reexecuted.
These are inherited unchanged receipts,not new W461 native execution.

W462 derives from W459 and passes203Lua/208payload hashes/CLI preflight.
Fourteen existing Lua payloads update only after exact tested-parent-preimage
checks;README also changes,193otherpayloads/DLL/override/membership exact.
Beardy has no new override/registration;source/review hashes explicitly
UNBUNDLED. DLL SHA256 remains
`84bd17a61a10b90778aa85d9233c93a2d6eef041457cc7b1aa1826795eb03ca2`.
Bundle: `work/codex_lua_beardy_nil_locals_bundle_w462_20261024/nil-only-local-readable-checkpoint-20261024`.
Not installed/launched/published. W461acceptance.json/root_verification.json and
W462verification.json are current receipts. Occupied feat/novi-script-recovery
HEAD366c08e/index unchanged;native source/prior proofs/unique history preserved.

**W463 completed with synthetic/static-profile qualification only.** Existing
SickChildsSister draft/readable copied exactly;metadata0x74c/end0x750 with prior
native-transfer anchor0xf309ac supplies the vector shape. Synthetic counts0/1/3
and clearly synthetic strings only;no stock/live-value claim or held asset read.
Retain empty/default modulo-zero failure and all14full-unit smoke problems;
fixture success must not be reported as general unit/gameplay certification.
Owned scratch: `work/codex_lua_sister_vector_smoke_w463_20261024/`.

Normal child ABI/callee-saved EBX/expected current types/disjoint locals/masked
x87,three unproven indirect target identities and nonnull/import normal ABIs
remain conditions. No engine acquisition/current lifetime/real heap/reentry/
exception/gameplay certification. No game/desktop/install/save/ref/index/commit/
publication action. Preserve other owner holds in `.agents/CLEANUP_COORDINATION.md`.
W458/W459 handoff is preserved in W461/handoff_before_acceptance.md;earlier
checkpoints/failures remain in the owning journal and retained snapshots.


## W463 synthetic Sister fixture/static source-profile diagnosis qualified

No production changes. Eight scoped CLI cases and eight observer Init/Main
cases retain default/empty modulo-zero errors in both stages;synthetic counts1/3
pass and selected synthetic strings reach animation calls.112 bounded Init/Main
cases vary7raw random returns,frame budgets0/1/2/8,two stages/two nonempty
counts;indices0 (count1) and0/1/2(count3) observed. Eight malformed/stale-source
fixture controls reject. No asset/stock-string read. Existing generic API/helper
responses remain supplied,not full-unit or gameplay proof.

Nonempty fixtures expose ReadGlobalGameDataDefStringAt as unknown under the
historical default smoke source inventory. Recorded sidecar commit5818237
contains that registration;four explicit-profile smoke cases recognize it with
allother execution results unchanged. Initial attempt required live worktree
bytes equal Git blobs and correctly refused:two files differ only LF/CRLF;
LuaRetailResources.h has16/6 uncommitted lines. Original check preserved. No
dirty source is admitted or changed. Static profile uses exact recorded Git
blobs and bounded local quoted includes in W463/recorded_profile;seed hashes,
worktree distinctions/missing includes recorded. This is source inventory only,
not proof that packaged DLL84bd17a6registers/executes this API or its ABI.

W463/qualification.json pins completed receipts,fixtures and scripts;current
W461source/HEAD366c08e/index exact,W462 latest verified offline bundle. Original
full-unit14default-profile smoke problems and all other owner holds remain.
W464 readonly investigation is paused; W465 preparation remains scratch only.
Do not relax flow/lifetime guards or promote source without a concrete proof.


## Session pause and exact resume pointers - 2026-10-24

User requested stopping here and updating documentation. Lua work is PAUSED;
no further implementation, tests, builds, evidence scans or publication started
for this wrap. No owned Lua Python job remains running at the checkpoint.
W465 prepare.py returned exit 0 before pause. Existing receipts can be inspected
directly; no rerun is needed to inspect status. This pauses the Lua lane only.

Accepted source remains W461, with acceptance.json and root_verification.json
under work/codex_lua_beardy_nil_locals_promotion_w461_20261024/.
W462 verification.json under work/codex_lua_beardy_nil_locals_bundle_w462_20261024/
is the latest offline bundle receipt. Beardy remains UNBUNDLED. W463
qualification.json under work/codex_lua_sister_vector_smoke_w463_20261024/
records synthetic fixture/static source-profile limits, not runtime certification.

Pending W464: work/codex_lua_beardy_flow_staging_w464_20261024/ contains
diagnose.py, diagnosis.json, probe_scalar.py, scalar_probe.json and
hypothetical_scalar_readable.lua. Root Flow refuses the compound conditional
jump at Main chunk line 252 (current file line 282; lua_local_versions.py line
123): NewScriptFrame call and scratchValue assignment before goto. Production
guards remain unchanged. A target-local analysis projection suggests 24 zero
stores to addNewConversation are unread before redefinition. One host definition
and two reads remain. This is a hypothesis, not accepted lowering or behavior.

Pending W465: work/codex_lua_beardy_scalar_candidate_w465_20261024/ contains
prepared.json, prepare.py, baseline/, phase.lua, phase_file.lua, review_row.json,
candidate_unit.json and host_integer_source.txt. Preparation completed only:
NO generic scalar lowerer, builder hook, adverse suite or promotion exists.
The captured phase is after final naming/wrapping/nil-only pruning and before
function headers, for BB_BeardyBaldyMan.Main at 0x00e50fd0. Phase SHA256:
`1cf24645fa591cba660e9347dd5464ba5cede863cdaccc504fece855a7b09236`.
prepared.json pins nine current source/ledger files. candidate_unit.json is
scratch metadata only; registered unit metadata is unchanged. Recorded C++
source commit 58182377795548b2f171750e1575ca0c18b06138 supports static int return
for Quest:AddNewConversation; fixed normal host integer return remains a
condition, with nativeImplementationVerified=false. No current DLL/ABI proof.

On explicit resume, first inspect this handoff, the owning journal and shared
coordination holds, then verify prepared.json pins before touching the candidate.
Next gate: an opt-in, target-specific scalar zero-store proof with exact phase,
registration and host-contract checks. Do not widen global Flow grammar. Any
analysis-only compound-jump projection must exclude target reads/writes in the
omitted prefix and prove the other destination is a unique pinned local; emitted
calls/assignments must remain intact. Prove every removed zero is unread before
redefinition and cannot defer populated userdata retirement. Reject captures,
receiver rebinding, unknown/non-integer/forged contracts, stale metadata,
unsupported control flow and environment/introspection hazards. Require adverse
controls, source correspondence, 11556-case Main behavior, generic default/no-op
controls, full unrelated readable corpus and staged/root checks before promotion.
If the proof is insufficient, retain the unchanged accepted source and the gap.

Preserve every active evidence tree, failed attempt, source freeze and owner
reservation, including W460 collection/checker failures, W461 preparer collision,
W463 rejected live-worktree profile and W464 Flow refusal. No cleanup release.
Protected checkout D:/Documents/FableTLC, branch feat/novi-script-recovery,
HEAD 366c08e64a4323a6147fe4d5caa675e31799d51e and shared index remain reserved.
No ref/index/staging/commit/switch/reset/rebase/stash/prune, private export,
source promotion, installation, game/desktop/save action or publication is
part of this wrap. All prior asset/build/native/private-source/game holds persist.
