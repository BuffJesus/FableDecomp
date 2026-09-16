# Script converter resume checklist

Active [marathon checkpoint](../journal/2026-09/NEW_OAKVALE_MARATHON_2026-09-14.md).
User requested sustained marathon work. Continue through the remaining gates.

- [x] Repair prior regression failures; first fresh full suite passes 1,511 tests.
- [x] Validate and integrate StartBarrelTimer and PostAttackStuff capabilities/body.
- [x] Compose emitted entity capabilities, quest adapters, live deeds and mission scopes.
- [x] Correct gold objective regions, structure Main/progress, and represent native host callbacks in the ledger.
- [x] Build isolated x86 runtime; validate 281 required methods in 8 actual Lua types.
- [x] Validate actual-host Main/thread registration, allocation rollback, Init/speech ownership, persistence and scalar destruction with engine doubles.
- [x] Regenerate disabled readable package: 51-function ledger, 18/18 syntax, zero scratch locals.
- [x] Finish native query-placement integration in quest/entity hosts.
- [x] Second full regression run passes 1,528 tests; persistence/ownership deltas pass separately.
- [x] Validate native process-drain ordering, Windows fibers, entity ownership and activation-tail restore order offline.
- [ ] Complete live gameplay, save/reload and unload parity; deferred by explicit offline-only direction.

Current runtime proposal chain ends at work/oakvale_entity_scalar_abi_integration (built).
Windows fiber/native process-drain and activation-tail restore checks pass.
Final review bundle: work/oakvale_offline_candidate_20260914; no pending processes.
Canonical runtime and installed game are unchanged; registration remains disabled.
The counts above do not imply whole-game behavioral completeness.

## Historical checklist (superseded; not current completion claims)

## Verified checkpoint

- [x] Whole New Oakvale inventory emitted: 51 functions, zero missing bodies.
- [x] Latest report checked: 47/51 functions, 14/18 files compile; 1,296 TODOs.
- [x] Draft registration remains disabled (`Quests = {}`).
- [x] Husband home movement, question entry, and conversation continuation fixes tested.
- [x] Native resource map verifies 57 resource events and ten temporary Thing lifetimes.
- [x] Dynamic animation-byte loads remain symbolic in argument IR.
- [x] Eight blocking-Speak mismatches remain explicit diagnostics.
- [x] Four-operation resource extension prepared; compiled forwarding harness passes.
- [x] Extension patch passes `git apply --check`; runtime checkout remains unchanged.
- [x] Full suite checkpoint recorded: 870 tests, known Bully two failures/two errors only.
- [x] Later temporary-Thing additions passed ten focused resource/lifetime tests.
- [x] No converter-owned process remains pending.
- [x] 2026-09-13: separate DISABLED resource-aware husband candidate generated under `refs/script_recovery/lifted/NewOakValeIntro/candidates/`; initially 6/6 tests; review follow-up 11/11.
- [x] 2026-09-13: extension proposal now carries five methods (adds `ThingIsDistanceFromPositionOver`, helper 0x00CBE45C); still unapplied.

- [x] Recovered 11 completed verifier verdicts; critic and 12 other agents failed at the spend limit.
- [x] Fixed hit pause operand and three dead SUB41 calls; added talk/retry/cancellation tests (focused set 24/24).
- [x] Movie/pause calls use the existing resource scope; native movie witness and CFG rechecked; Lua error cleanup tested.
- [ ] Resolve scope storage growth before runtime integration.

## Resume in this order

- [ ] Re-read [the 2026-09-13 handoff](../journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-13.md) (then the 09-12 one), current report, and runtime source before relying on old assumptions.
- [x] Implement a **separate disabled** resource-aware husband Lua candidate (`generate_affair_man_resource_candidate.py`).
- [x] Keep one resource ID across construction, preparation, explicit acquisition attempts, actions, task queries and deterministic destruction (counted against the witness).
- [x] Lower all ten returned Thing wrappers through their real query and destructor; the Thing distance query is in the prepared (unapplied) extension.
- [ ] Resolve cached woman/wife and hit-condition wrapper ownership; movie scope/error cleanup is now covered offline.
- [ ] Provide a reviewed runtime read of animation byte `0x01375748`; do not freeze it to true.
- [ ] Coordinate integration of `work/man_resource_extension/resource-actions.patch` (now five methods) with the runtime owner. It is **not applied**, built as a full x86 runtime, or deployed.
- [ ] Test real generated resource flow against native traces for normal, retry and cancellation paths. Stub forwarding and snippet tests do not prove engine ABI or gameplay parity.
- [ ] Continue remaining entity/quest/helper recovery, including four syntax-failing entity files.
- [ ] Complete full-scope behavior and integration validation before enabling any draft.

## Boundaries and useful artifacts

Preserve the dirty shared workspace. Do not reset/stash/clean broadly. Runtime checkout,
reconstructed ports, installed game files and shared API manifests belong to the parallel session.
The known Bully failures concern missing camera cleanup after run1 termination; do not modify that
session's port or tests to make the converter suite green.

- Converter entry: `tools/script_recovery/convert_new_oakvale.py`.
- Husband candidate generator: `tools/script_recovery/generate_affair_man_resource_candidate.py` (tests: `python -m unittest tools.script_recovery.test_generate_affair_man_resource_candidate`).
- Report: `refs/script_recovery/lifted/NewOakValeIntro/CONVERSION_REPORT.json`.
- Native resource evidence: `tools/script_recovery/native_affair_man_resources_witness.json`.
- Resource mapper: `tools/script_recovery/native_post_attack_resources.py`.
- Speech gap: `tools/script_recovery/native_affair_man_speech.py`.
- Extension generator: `python -m tools.script_recovery.prepare_man_resource_extension`.
- Extension tests: `python -m unittest tools.script_recovery.test_retail_resource_actions`.
- Resource tests: `python -m unittest tools.script_recovery.test_native_affair_man_resources tools.script_recovery.test_native_resource_lifetime`.
- Full checkpoint log: `work/converter_resource_action_ir_tests.log`.

Do not mark the goal complete based on syntax counts, decreasing TODOs or passing narrow tests.

## BookTrader structured readability and conversation audit

The readable builder now applies `structure_book_trader_lua.py` after local analysis.
BookTrader has no executable goto or native labels: `prepareAndReturnHome`,
`processInteraction`, `finishTrade`, and `tryIntermittentLine` preserve the phase
boundaries and release the resource after the outer loop. Its original scoped
cleanup helper is retained. Before/after event traces match for 240 scenarios
covering purchase, refusal, hit, movement, acquisition and cancellation, plus
three injected-error cases. Shared local analysis now understands inline loop
breaks and names known conversation/random/distance/Thing-alive results.

Current generated package: 51 functions, 18/18 files compile, 861 semantic local
names and 210 unresolved scratch names. Raw diagnostic count remains 1,203.
The focused readability/BookTrader/husband/entry-condition set passes 44 tests.
The last full suite remains `_09.log` (1,019 passed), predating this batch.

Conversation audit: `work/book_trader_conversation_audit.json` records native
call operands and byte hashes. Creation uses self/false/false, and the subsequent
person addition consumes that conversation and the native GetHero result.
A remaining ordering gap is concrete: native constructs the line key before
GetHero, whereas current Lua obtains the listener before the host constructs its
FableString. A scoped line-to-hero binding needs native GetHero ownership review
and runtime validation before this gap can be closed. Host participant-validity
checks also require parity review. Runtime proposals remain unapplied; full DLL
and gameplay validation remain outstanding.

## Scoped BookTrader conversation line

The native intermittent line now lowers to `quest:AddConversationLineToHero(...)`.
`native_book_trader_conversation.py` checks pinned call windows, vtable bindings,
text literal, and symbolic call operands. GetHero returns the interface-owned
Thing at interface+0x30 (891CB8/891D46), so the binding borrows it without a Lua
copy or caller destruction. The scoped binding constructs the key first, queries
the hero, queues the line, and destroys the key in native order.

Staged proposal: `work/book_trader_conversation/conversation-integration.patch`.
It passes git apply --check and the actual MSVC x86/API-type/vendor sol+Lua harness,
including populated/empty hero values and lookup/line exceptions. Native evidence,
BookTrader candidate/readability and entry-condition tests: 21 passed.
All 18 readable files compile; 860 semantic names and 210 scratch names remain.
One temporary disappeared with the scoped binding. Runtime is unchanged; the
creation/person validity guards, full DLL build and gameplay parity remain open.

## Conversation setup runtime proposal

The staged BookTrader adapter now also provides `StartConversationWithHero`:
native CreateConversation -> GetHero -> AddPerson -> return conversation ID.
It avoids the existing host IsNull query and preserves participant addition for
zero/negative IDs and actual empty hero values. Updated two-method patch passes
git apply --check. The x86/API-type/vendor Lua harness verifies both flags,
IDs -1/0/73, populated/empty participants, call-stage errors, and no extra host
validity query. See `work/book_trader_conversation/proposal.json` and build logs.

The converter still emits the separate setup calls pending the running full
suite10 source freeze. Next: strengthen native setup callee pins, lower the
three-call setup to this binding, add caller tests, regenerate readable output.
Full suite10 is confirmed live at this checkpoint (session25209); poll that
handle and read its log rather than starting another suite.

## Conversation setup lowering and BookTrader readability

BookTrader now uses both tested scoped bindings: `StartConversationWithHero` and
`AddConversationLineToHero`. Native create/person callees are pinned in addition
to the caller windows and GetHero/line callees. Creation/person tests preserve
both flags, empty participants, IDs -1/0/73, error propagation and cleanup. The
converter no longer emits the separate host calls with extra IsNull queries.
Both runtime methods remain staged in the conversation integration patch.

BookTrader's readable output has no executable labels/gotos, TODO markers, or
unresolved scratch local names. Question-answer and guarded-distance results
now get semantic names; a second checked literal cleanup removes staging stores
made provably unused by literal inlining. All 18 files compile; current package
counts are 873 semantic names and 192 unresolved scratch names. Historical raw
diagnostics remain 1,203 and do not equal current surviving TODO markers.

Full suite10: unittest reports 1,042 tests / 262.565s / OK. The PowerShell wrapper
returned 1 after wrapping stderr as NativeCommandError; the separately recorded
result JSON distinguishes that from the unittest verdict. After the setup and
readability changes, 47 focused tests pass. Runtime/gameplay parity is unproven.
Next substantial work: remaining ownership/control-flow gaps in the other New
Oakvale actors, then combined runtime integration and live trace validation.

## AffairWoman loop and departure operands

Readable builder now applies `native_affair_woman_control.py` against the pinned
2714-byte Main and exact raw draft. It repairs the loop's unresolved extraout_AL
values using the already computed termination boolean. The two camera queries
now consume the bound woman's current GetPos result (native DB290D/DB293D ->
DB2914/DB2944), and removal uses self/false/true atDB2965. The prior draft passed
missing camera positions and removed the run-off marker instead of the woman.

Nine focused control/position/entry-condition tests pass, including18 departure
camera/cancellation traces and mutated native/draft/binding rejection. Resource
reset and final temporary cleanup are explicitly outside those trace assertions.
Woman resource/movie/named-Thing ownership, home/run-off controlled distance
queries, partner distance operands and remaining body branches are still open.
Do not treat readable syntax or these partial operand checks as completed parity.

## AffairWoman controlled-resource map

`native_affair_woman_resources.py` now verifies all39 native resource events,
including6 acquisitions, five temporary-Thing getters, action/task calls and
cleanup. Full aligned CFG validation proves one stack+16 constructorDB1F5C and
one destructorDB298A on every constructed path; entry cancellation bypasses both.
`work/affair_woman_resource_audit.json` records the verified operands and map.
The map does not yet lower the Lua or prove temporary/named-Thing/movie lifetimes.

The shared argument verifier now retains literal/address arguments across writes
to provably disjoint stack slots; overlapping slots and memory-derived arguments
remain invalidated. This recovers DB280B's output stack136 and resource stack16
without relaxing unknown-pointer alias rejection. Twenty call-setup tests pass,
including disjoint/overlapping writes and memory-loaded argument rejection.
Next: map five temporary getter/query/destruction lifetimes, movie and named-Thing
ownership; generate the separate disabled woman resource candidate, then structure.

## AffairWoman temporary and movie lifetimes

The resource verifier now checks five temporary getter/query/destruction lifetimes
against the full CFG and pinned resource helper bodies. Three distance queries
use the returned Thing with threshold2; two health queries also consume the
returned Thing, not the cached husband currently emitted by the draft. The home
temporary's inline strong cleanup and final weak cleanupDB20EE are accounted for.
Changed output/missing temporary/early destructor evidence is rejected.

`native_affair_woman_movies.py` verifies two nonoverlapping movie scopes (stack148
and96), ten constructor/start/destructor events, and eight pause calls. Separate
CFG checks prove pause pairing and that unpause occurs while its movie is live,
before destruction. Both starts pass true; all six normal/cancel exits pass false.
The raw draft's false starts and missing destructors remain to be lowered.
Resource/movie tests:5passed; temporary-map/call-setup set:23passed.
Artifacts: `work/affair_woman_resource_audit.json`, `work/affair_woman_movie_audit.json`.
Next: retained wife/man/run-off Thing scopes and their operands, hit predicate,
then explicit resource candidate using these maps and structural/readability pass.

## AffairWoman retained actors and hit predicate

`native_affair_woman_actors.py` pins three named-Thing lookups and checks each
retained lifetime against the native CFG. Wife stack48 is the radius5 run-off
trigger partner. Man stack36 faces the hero then self atDB24A5/DB25C1 (snapfalse),
not the draft's hero/wife pair. Run-off marker stack72 supplies the copied position
and is destroyed before man then wife; the removed actor remains bound self.

The hero-hit byte window DB217C..DB224B proves the existing scoped helper pattern:
direct hit OR(any hero ability AND NOT ability14), with conditional CString
construction and reverse destruction before consuming the result. The byte-backed
witness retains the exact old Lua block for counted replacement. Two actor tests
pass, including wrong partner/omitted use/destructor rejection.

Next: generate a separate woman resource candidate from control, resource,
temporary, movie and actor maps. It must repair home snapshot/movement, health
queries, hit result, movie flags/cleanup, retained facing/distance operands and
all release joins; owned run-off Thing position needs a scoped position binding.

## Separate AffairWoman resource candidate generated

`python -m tools.script_recovery.generate_affair_woman_resource_candidate` now
writes the disabled candidate to `work/affair_woman_candidate`. Counted rewrites
consume all four native maps before output: one resource, five temporary Things,
three retained Things, two movie scopes, native hit/facing/distance/health/movement
operands, live animation reads, pause flags and normal/cancel cleanup joins.
No unresolved TODO/missing/extraout/SUB41 operands remain in this intermediate
candidate, but structured readability and full operand/runtime parity remain open.

Four candidate tests pass, including203 cancellation scenarios, hit/talk movie
ordering, reversed partner release, home/run-off behavior, animation flags and
injected errors. The mock was corrected to accept the woman's documented second
animation flag true (BookTrader's is false); source animation flags were unchanged.

Owned-marker position now has a staged12-method resource proposal at
`work/woman_resource_integration/resource-integration.patch`, generated from the
hash-checked combined11-method proposal. New `ThingPosition` resolves the live
owned wrapper and invokes existing ReadRetailThingPosition, returning only a vector
copy. git apply --check passes; compiled validation of the new member remains next.
Do not add woman candidate to readable builder until this binding and entry
condition insertion are verified. Runtime checkout/game remain unchanged.
Full suite11 completed:1,065 tests/347.828s/OK, captured Python exit0. It predates the new woman candidate tests.

Offline-only direction: user selected "Keep working offline only" and is at work,
unavailable for live verification. No activation, installation or save/profile changes.
Continue native/compiled offline integration; do not ask again about live testing.
