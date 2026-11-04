# Lua recovery continuation — 2026-10-19

User steering: proceed with Lua until asked to stop. This supersedes the older
native lane's “no Lua” scope for this session. Work remains in
`D:/Documents/FableTLC`, branch `feat/novi-script-recovery`, HEAD `366c08e`.
Existing dirty files and other worktrees are preserved. No desktop or game action
is authorized by this continuation; the existing fresh screen authorization gate
remains in effect.

## W222: integrate the accepted W221 Book dispatch classifier

Owned task: `work/codex_lua_dispatch_promotion_w222_20261019`.
Starting output baseline: W216 `current_corpus.json`, 534 Lua files.
Latest offline bundle: W217 `signed-counter-candidate-20261018`.

W221 already completed 29 fresh conversions with 2034 unchanged input pins,
28 unrelated conversion reports exact, 38 focused and 239 adjacent tests, and
2016 controlled Lua comparisons. Its only generated changes are annotations in
the Teacher draft/readable pair; significant Lua tokens are identical.

`prepare.py` verifies the W221 corpus pins and saves owned source/evidence
preimages before copying the candidate classifier, optional resource-table facts,
loop-break liveness support, and production converter import into the root.
Durable fixtures now live under
`tools/script_recovery/fixtures/dispatch_staging/book_comment`.
Evidence and generated output have not yet been promoted.

Validation in progress: production imports must reproduce all eight Book Lua
files and the accepted conversion report. The focused/adjacent suite additionally
tests partial table-register clobbers, intervening control transfers, resource
disagreement at CFG joins, and optional-fact isolation.

Review identified an additional requirement: a branch entering between the table
load and spill must reject classification. The original extent has no such edge.
Three mutants redirect the original health-exit branch at E57794 into the table
lifetime, with freshly computed native hashes, to test this guard independently
of stale-input rejection. Results and any correction will be recorded below.

No SDK, DLL, package, installation, save, game, commit, or push changes.


### W222 complete: classifier and stronger entry guard promoted

All 295 adjacent/integration tests passed. Review then exposed a missing guard:
three native branch redirects into the load/spill interval were accepted despite
freshly computed hashes. The stricter classifier rejects every branch entering
after the table load through the spill. The original function has no such edge.
All 109 guarded tests and 109 production tests pass, including those three
regressions, partial-register clobbers, control transfers, and receiver joins.

Both original and guarded production-import generation reproduce all eight
accepted W221 Book Lua files and conversion report exactly. Full W221 29-unit
validation remains applicable: all 2034 original pins are checked except the
three exact candidate-to-production module copies/import change; the classifier
adds only the tested restrictive guard, and only Book opts into classification.
After evidence/output promotion, fresh root generation again reproduces all
eight files/report; all 534 corpus hashes verify, with only the Teacher pair
changed and 532 files exact. Significant executable Lua tokens are identical.
The existing 2016 controlled before/after comparisons therefore remain applicable.

Ledger: 371 files, zero syntax failures, 853 unresolved (two fewer: the TODO
and its inline marker). Only subsequent Teacher finding line numbers shift.
All other counts/statuses and all other file rows remain exact. Sources, durable fixture,
one Book metadata entry, Teacher pair, reports and ledger are promoted. No prior
ownership/native operand certificates were altered.

Commands and evidence: `prepare.py`, `generate.py`, `guarded_checks.py`,
`promote.py`, `root_verify.py`, `finalize.py`; logs and SHA receipts are in W222.
The first promotion preflight stopped before writes on UTF-16 log decoding;
the next stopped before writes on the Windows default source encoding. Explicit
log decoding and `-X utf8` resolved those tooling issues; failed logs are retained.
The initial audit assertion expected a single finding removal and omitted derived
line shifts. The fresh audit proved only the Teacher row changed; the corrected
assertion accounts for both TODO/inline markers and each subsequent line shift.
The resumed preflight explicitly accepts only the exact already-reviewed Book
metadata/output, preserving the original preimages and all other input pins.

No SDK/DLL/install/save/game/commit/push changes. These are static and controlled
mock checks, not new retail gameplay proof. Next refresh W217 into a separate
W223 offline bundle, then inspect the remaining Book argument-byte staging
diagnostic against original instructions before selecting a generic correction.


## W223 complete: verified offline Book annotation refresh

Owned `work/codex_lua_dispatch_bundle_w223_20261019`.
`dispatch-staging-candidate-20261019` derives from W217. All 203 Lua files parse,
all 208 payload hashes verify, and the standard `novi_local_test` CLI preflight
passes. Only `NoviCompatibility/V_BookCollecting/Entities/BS_Teacher.lua` and
README differ; the other 206 payloads, including DLL c6ef, stock FSE, launchers
and override membership, retain parent hashes. Parent payload/manifest and
installed EXE/QST pins remain exact.

Manifest SHA-256:
`5f0916fd4a23672230e02c09149b34bea88f57ce38ddf6cea3e1036896eb8cd0`.
No installation, launch, save writes, or new gameplay acceptance.

Next W224: inspect E57020 Book21 CONCAT31 argument-byte spill beyond W187's
existing comparison boundary. W187 already covers 1572 native cases and 522 Lua
calls; retain that proof and its constructor/map/destructor exclusions. Recover
the spill's subsequent overwrite/read/escape lifetime from original instructions
before deciding whether any diagnostic can be classified. Do not infer that a
successful A/B comparison establishes the caller's complete stack lifetime.


## W224 complete: native Book21 argument-slot lifetime, evidence only

Owned `work/codex_lua_book_argument_lifetime_w224_20261019`.
816 original native executions / 64 current draft/readable Lua calls pass.
Sixteen ASCII/UTF-8 strings, allocated/null empty representations, continuation
and termination, two stacks, three fills, and four stale argument DWORDs vary.

Original E57324 through RET8, original 40-byte comparator, complete 61-byte
CCharString constructor at 99EBF0, and 12-byte destructor at 99EAE0 execute.
The real constructor zeros all four bytes before its first read of the scratch
slot. Allocation, map lookup/assignment, and final key release follow. Terminating
paths never read that slot; the caller discards it at return. Saved EBP map
identity, all callee-saved registers, RET8 stack purge and instance-count changes
are checked. Constructor/map/release events agree across all stale DWORDs.

Four adverse controls are detected: missing, partial or bypassed constructor zero,
and wrong map receiver. Full original run passes again after adding those controls.
Initial scratch assertion used a 43-byte constructor extent; corrected to the
actual 61-byte RET8 boundary before any successful evidence claim. Failed log kept.

This extends W187's comparison boundary; it starts with explicit post-ARG2 state.
Allocation/release, map implementation, string assignment and termination are
explicit ABI doubles. Earlier ARG1/ARG2 native execution, exception/unwind, actual
heap, compiled SDK, scheduler and gameplay remain outside proof. The native
CONCAT31 diagnostic is retained. All 534 root Lua remain exact to W222, ledger
unchanged; W223 remains latest offline payload. No source/SDK/DLL/install/save/
game/commit/push changes.

Next: build a fail-closed CFG proof that the one-byte argument spill has no read
or escape before full constructor overwrite/return, with pinned helper contracts;
then assess whether generic comment-only classification is justified. A bounded
execution packet alone does not establish every reachable static path.


## W225 complete: conditional static stack-spill liveness

Owned `work/codex_lua_stack_byte_cfg_w225_20261019`. Scratch verifier follows
23 native states after E573A4. Both conditional successors reach either a full
constructor overwrite at E5735B/E573C0 or RET8 discarding through offset28.
34 synthetic tests pass: direct/aliased reads, partial writes, pointer spills/
arguments/return escapes, unknown ESP/memory/callee/receiver origins, overlapping
receiver objects, caller-stack access contracts, loops and live returns.

This is conditional evidence: initial EBX is the script receiver; zero-argument
termination and disjoint CString release use explicit access contracts. The
constructor's actual first full-zero store is pinned. No production classification
is justified from helper names/stack purge alone; diagnostic and root Lua stay
unchanged. No generated/SDK/DLL/install/save/game/commit/push changes.

Next W226: replace W224's termination and CString-release doubles with original
F35B30 wrapper, registered Teacher GetParentScript E54620, CB7940 termination
predicate, and 99E9B0 release body. Keep only map/allocation/CRT boundaries doubled.
Vary null/zero/nonzero thread states and shared/last CString references; inspect
raw memory access and once-only release order before reconsidering the contracts.


## W226 complete: original termination and CString-release closure

Owned `work/codex_lua_book_helper_closure_w226_20261019`.
6120 native executions / 480 draft/readable Lua calls pass. Original E57324 caller,
comparator, complete constructor/destructor, 87-byte 99E9B0 release, 12-byte
F35B30 termination wrapper, registered Teacher vtable getter E54620 and 14-byte
CB7940 predicate execute. The real Teacher vtable slot12 is checked against
E54620; the actual getter reads this+14, and predicate follows parent+2C/thread+5.

Five thread states (null, zero, 1, 2, 255), reference counts1/2/9, sixteen ASCII/
UTF-8 strings, allocated/null empty representations, two stacks, three fills and
four stale DWORDs vary. 2448 actual constructor-zero/key-release paths, 6120 Teddy
release calls and 2736 once-only text/control free pairs pass. Shared controls
decrement to1/8 without freeing; last references free text then control and clear
the CString slot. Instance counts, caller ESP/RET8 and all saved registers restore.
Key controls with extra references are deliberate factory-boundary injections;
ordinary reachability of a shared freshly allocated key is not asserted.

Seven adverse controls reject: missing/partial/bypassed constructor zero, wrong
map receiver, missing release decrement, wrong thread byte and wrong parent offset.
The initial trace-equivalence assertion incorrectly grouped null and allocated
empty representations, whose actual frees differ. Corrected to compare within
each representation; exact expected free sets are checked for both. Final full
run passes after all controls were added. All failed logs and exact source diff
from W224 are retained.

Map lookup/value assignment, allocation and CRT free remain explicit boundaries;
parent/thread/control graphs are synthetic. Earlier ARG1/ARG2, real heap,
exception/unwind, compiled SDK, scheduler and gameplay remain outside proof.
No diagnostic is classified. All534 root Lua remain exact to W222; W223 remains
the latest verified offline bundle. No production/SDK/DLL/install/save/game/
commit/push changes.

Next W227: statically validate helper access summaries from original bodies and
the registered vtable, replacing W225's manually declared assumptions. Preserve
valid-object/heap and allocator boundary limits. Then separately evaluate a
generic comment-only classifier with stale/ambiguous/escape rejection.


## W227 complete: original helper shapes derive static access summaries

Owned `work/codex_lua_helper_access_w227_20261019`.
65 tests pass (31 helper/access cases plus34 W225 CFG cases). Original registered
getter/vtable, wrapper, predicate, constructor prefix, complete destructor/release
and invariant caller EBX origin are checked by exact supported instruction shapes.
All six helper extents decode completely; each original body receives a SHA pin.
The derived contracts reproduce W225's 23-state proof, two constructor overwrites
and caller-discard return. Caller receiver partial writes and ambiguous origins
reject; altered helper fields/widths/branches/callees/stack accesses reject.

Initial scratch checks exposed Capstone's decimal formatting for small operand
displacements and the counter's virtual .data padding. Corrected formatting;
counter validation now checks its original PE writable section mapping and the
matching constructor INC/destructor DEC operands. No absent file bytes are treated
as initialized data. Failed preflight logs retained; the original RData reader
and its file-backed bounds remain unchanged.

Valid disjoint script/CString/control/data objects remain required. The actual
release body touches the four-byte object plus its separate packed17B heap control
and owned data; CRT frees and allocator validity remain external boundaries. No
exception/unwind, full-helper or gameplay proof is inferred. No diagnostic or
production change yet; W222 output/W223 bundle remain current.

Next W228: scratch generic post-review classification of the one redundant
byte-spill diagnostic, gated by exact native/exported-C/Lua phase pins and the
derived access/CFG proof. Preserve all executable Lua tokens and retain a reviewed
annotation. Add stale/ambiguous/observable-read/escape negative cases, regenerate
Book and compare unrelated registered units before any promotion.


## W228 in progress: scratch generic byte-spill classification

Owned `work/codex_lua_byte_spill_classifier_w228_20261019`. Captured exact final
E57020 Lua phase, native/exported-C pins, current unit and production preimages.
Generic optional classifier derives original helper access summaries and native
CFG liveness, validates registered entity/getter identity and all helper body pins,
then changes only the reviewed redundant-spill annotation. No instructions or Lua
assignments are removed. Stale/ambiguous/native-observable cases reject.

95 tests pass (30 classifier plus65 helper/CFG), and660 current/candidate controlled
Book helper comparisons match map arguments and query counts across termination
stops. Fresh8 Book Lua parse; only Teacher draft/readable annotations change, all
significant Lua tokens identical. Existing LookForBook dispatch proof is preserved.

The first scratch conversion rejected a diagnostic-length mismatch: the report
truncates this long template diagnostic, while Lua retains its full comment. The
candidate pins both exact forms separately, verifies their prefix correspondence
and multiplicity, and matches the full Lua marker. No existing guard is relaxed.

All29 registered-unit comparisons are running in owned session98899; progress is
in `corpus.log`/`corpus_progress.json`. Do not infer completion from partial output.
No production code/evidence/output is promoted. W222 generator/output, ledger
371/0syntax/853unresolved and verified W223 offline payload remain current.
No SDK/DLL/install/save/game/commit/push changes. Next collect all29, verify input
pins and every report delta, then prepare durable production fixtures and tests.


### W228 full corpus complete; W229 production root checks running

All29 W228 fresh conversions pass; all input pins verify. 28 unrelated reports
and all unrelated Lua are exact. Book differs only by one classified diagnostic/
proof, two TODO totals and the readable audit's one finding/count (plus draft
path). The initial readable-report expectation omitted its derived finding/count;
exact delta was reviewed and the corrected comparison passes. All534 root Lua
were still W222-exact at the completed scratch checkpoint.

W229 prepares durable production modules/fixtures, with95 prepared tests and8
Book Lua/report exact to W228. Before writing, the full current371-file audit
matched its accepted ledger, then the candidate Teacher row was proved to remove
only the one reviewed finding. Promotion copies tested source, one metadata row,
Teacher pair, reports and ledger; ledger is now371/0syntax/852unresolved.

Fresh actual-root generation reproduces all8 Book Lua/report; all534 current
hashes and promoted-file receipts pass. The full Book/focused production suite
is still running in session38669. Do not mark W229 accepted until its completed
result is collected. W230 bundle builder is prepared but has not run. Latest
verified offline payload remains W223. No install/game/save/SDK/DLL/commit changes.


## W229 complete: native argument-byte spill classifier promoted

95 prepared production tests and368 actual-root tests/768 subtests pass.
All15 Book test modules plus the new classifier/access/CFG tests and prior
dispatch-staging tests ran against the current production metadata and generator.
Fresh actual-root generation reproduces all8 accepted Book Lua/report. All534
current Lua hashes and every promoted source/evidence/output receipt verify.
Only Teacher draft/readable annotations differ;532 other Lua remain byte-exact,
and all executable tokens are identical. Ledger371/0syntax/852unresolved; every
other row/count/status remains exact.

Promotion includes three generic modules (byte-spill annotation, helper access
summaries, native stack liveness), optional converter hook, three durable test
modules/fixtures, one Book evidence row, Teacher pair, reports and ledger. All
existing native/ownership/operand fingerprints and recovered operations remain
unchanged. Full native/exported-C/Lua phase pins, registered owner/getter and
helper-body identities are enforced before classification. Whole source and
compiled token boundaries remain explicit; no generated Lua was hand-edited.

The accepted W228 corpus supplies29 fresh conversions with28 unrelated reports
exact and660 controlled before/after helper comparisons. All original input pins
remain exact except the explicitly receipted production converter/Book metadata.
W226's6120 original helper executions,480 Lua calls, seven controls and2736 free
pairs remain unchanged evidence. Valid disjoint objects, allocator/map/CRT-free
and exception/unwind/gameplay limits are retained. No full-helper/gameplay claim.

Commands/results/pins: W229 prepare/test_prepared/generate/promote/root_verify/
root_tests/finalize scripts and their logs, `promotion.json`, `current_corpus.json`,
`verification.json`; W228 retains the full corpus and reviewed report delta.
No SDK/DLL/install/save/game/commit/push changes. W223 remains the last verified
offline payload until W230 refresh. Next inspect the nine remaining Teacher Main
nil-staging pairs, starting from the named BookDef vector-base load at parent+94,
against native field/use evidence before selecting another generic recovery.


## W230 complete: verified offline Book argument annotation refresh

Owned `work/codex_lua_book_argument_bundle_w230_20261019`.
`book-argument-spill-candidate-20261019` derives from W223. All203 Lua files parse,
all208 payload hashes and the standard `novi_local_test` CLI preflight pass.
Only Teacher Lua annotation and README differ;206 other payloads, stock FSE,
DLLc6ef, launchers and override membership retain parent hashes. Parent manifest/
payload and installed EXE/QST pins remain exact.

Manifest SHA-256:
`e9753822f038e2ef00ba71d9718fc579d5b6cd13da21aa10ffc2e14ebefc97f2`.
No installation, launch, save writes or new gameplay acceptance. Runtime limits
and fresh screen authorization gate remain unchanged.

Next W231: capture the current Teacher Main phase, original full function and
known BookDef vector metadata. Trace the parent+94 vector-base load to its native
index/consumer and current restored GetStateStringVectorAt before attempting any
generic unused-staging classification. Preserve all nine remaining nil pairs.


## W231 complete: Teacher Main vector-base/index/consumer correspondence

Owned `work/codex_lua_book_vector_base_w231_20261019`.
Captured current final Main phase and original3534B E54E90 extent, SHA
`f8c76fa0554a4f69cab714606e14fef55549d7e0263ad1cb6f26aa080b571379`.
Existing CFG member-load analysis proves E5559A reads parent+94. Existing typed
metadata identifies the quest-owned BookDef CString vector, four-byte stride,
Init copy and readonly consumer list; parent+8C is LastBookRequested Int.
Original E55591..E555BD forms base+index*4 in EBP, gets hero through slot118,
then passes the selected CString address and that hero to slot2E0. No branch
enters midway to bypass the base/index definition.

Current Lua uses the restored named vector/index/possession operation. The leftover
iVar1 nil has no live reads across381 phase CFG nodes. Final Main, including all
cleanup closures, has exactly six alias tokens: declaration, initial scalar
assignment/three early reads, and final nil store. No later alias token exists;
early reads precede all loop/label entries. Ten native origin/index/vector/stride/
clobber/bypass/metadata and Lua read/capture controls reject.

The accepted sidecar source registers GetStateStringVectorAt and
IsObjectInThingsPossession; the latter constructs a scoped FableString and passes
CString/Thing pointers to slot2E0. The vector binding returns a copied string with
explicit bounds checking. Source pins are recorded; no new compiled-binding ABI
or gameplay proof is claimed. Malformed indices, mutable-vector effects during
GetHero, native CString validity and engine ownership remain outside this static
packet. All534 Lua and ledger stay W229-exact, W230payload current. No diagnostic
is classified and no SDK/DLL/install/save/game/commit/push change occurs.

Next W232: execute the original vector/hero/possession caller prefix with explicit
valid string vectors, saved-index changes and API boundaries; compare exact current
draft/readable Lua prefix arguments/order/branch. Preserve the immutable snapshot
contract and leave invalid indices as an explicit gap.


## W232 complete: original vector/hero/possession caller-prefix comparison

Owned `work/codex_lua_book_vector_prefix_w232_20261019`.
2688 original native executions /448 current draft/readable Lua prefix calls pass.
Exact source snippets are extracted from both current generated files; every
selected string, hero identity, normalized API call order and true/false branch
matches the original E55591 index/base capture through TEST AL/branch.

Four explicit nonempty vector sizes1/2/4/7, every valid index, two hero identities,
two vector-base/empty-string representations, two stacks/three fills and return
bytes0/1/2/255 vary. Stable and boundary-injected index changes during GetHero both
retain the original captured index. Caller ESP, script receiver, selected EBP
address, saved GSI table and vector header bytes remain correct. Three wrong-index,
stride and callee-saved element-pointer controls reject. Return bytes2/255 are
deliberate ABI-boundary injections, not ordinary C++bool reachability claims.

GetHero and possession are explicit thiscall doubles. Their real implementations,
binding/heap/owner lifetimes, mutable-vector effects, malformed indices, exceptions,
complete Main and gameplay are not proven. The BookDef immutable snapshot contract
remains explicit; no default/empty world data is invented. All534 generated Lua and
ledger remain W229-exact, W230payload current, diagnostic retained. No production/
SDK/DLL/install/save/game/commit/push changes.

Next W233: scratch generic definition-vector staging classifier linked to existing
member-load analysis, exact native field/index/consumer witnesses and Lua phase/
final-cleanup liveness. Retain the nil assignment, all executable tokens and all
other diagnostics. Validate stale/ambiguous/mutable/wrong-consumer/read/capture
cases and all registered units before promotion.


## W233 in progress: generic immutable definition-vector staging classifier

Owned `work/codex_lua_definition_staging_w233_20261019`. Optional post-review
classifier requires exact native/exported-C/Lua phase pins, declared immutable
CString snapshot/initializer/reader metadata, native parent/index/scale/call-span
witnesses, original interface targets/indirect-call metadata, and Lua dead-alias
proof. Wrong storage/element/stride, writers, initializer use, stale/missing/
duplicate evidence, branch bypasses, pointer clobbers, reads and captures reject.

121 focused/adjacent tests pass. Fresh8 Book Lua parse; only Teacher annotations
change and all executable tokens match. The first scratch output intentionally
failed the token gate: deleting the old comment also removed a legacy unused
self0X14 declaration because the entity-field presentation stage scans that text.
Revised candidate retains the original fragment as a reviewed annotation and
retains the nil assignment, limiting this task to annotation changes. The unused
declaration/presentation cleanup remains separate; failed output/log retained.

444 paired before/after snapshots across24 full-Main scenarios pass: complete
good/evil donation sequences, fresh-VM reloads, refusal/lost inventory, classroom/
award interruption and forced serializer reload. Instrumented vector reads, hero/
possession calls, macro arguments/order, state/flags/inventory, resource/camera/
pause exits all agree. Engine, coroutine/serializer and resource boundaries are
explicit doubles; snapshots are not real saves or gameplay acceptance.

All29 scratch registered-unit comparisons are running in owned session3424;
logs/progress are in the task folder. No production code/evidence/Lua is promoted.
W229 remains accepted generator/output (534 Lua), ledger371/0syntax/852unresolved;
W230 remains the verified offline payload. No SDK/DLL/install/save/game/commit/
push changes. Next collect full corpus/pins/report deltas, then prepare durable
production integration and its root checks.


## W234 complete: immutable definition-vector staging classifier promoted

W233 all29 conversions completed;28 unrelated reports/Lua are exact. The Book
conversion delta is exactly one reviewed diagnostic/proof and two TODO totals;
readable audit removes the matching TODO/inline findings and updates their count.
Diagnostic ordering is explicitly preserved. Production preparation passes121
tests and reproduces all8 Book Lua/report. Actual-root suite passes489 tests/768
subtests; fresh8 Book Lua/report, all534 corpus hashes and every promoted-file
receipt verify. Only Teacher draft/readable annotations change,532 other Lua are
exact and all executable tokens remain identical. Ledger371/0syntax/850unresolved,
with all other rows/counts/statuses unchanged.

444 paired snapshots across24 full-Main before/after scenarios match resource,
state/flag/inventory, API argument/order and exit behavior. Four targeted query
checks explicitly reach BookDef index3/BOOK_3 possession after refusal/lost
inventory in both stages. Reload and interruption snapshots remain serializer/
coroutine/resource doubles. W231 static381-node/ten-control and W232 original
2688-prefix/448-Lua/three-control evidence are retained with their original limits.

Promoted one generic definition-staging module, optional converter hook, durable
44-case classifier fixture/test module, one Book evidence row, Teacher pair,
reports and ledger. Existing operand/ownership/hash checks remain enforced. The
nil assignment and reviewed original fragment remain; this preserves a legacy
unused field declaration introduced by comment-scanning presentation code. No
generated Lua was hand-edited and no SDK/DLL/install/save/game/commit/push changes
occurred. W230 remains the last verified payload until W235 refresh.

Next W235 refresh the offline bundle. Then W236 inspect inline_entity_fields and
entity-state presentation lexical boundaries: code inspection shows regex key
discovery/replacement currently scans comments and string literals. Prepare an
isolated generic correction and behavioral counterexamples; do not mix the
presentation fix into this accepted annotation/evidence change.


## W235 complete: verified offline definition-vector annotation refresh

Owned `work/codex_lua_definition_bundle_w235_20261019`.
`definition-vector-staging-candidate-20261019` derives from W230. All203 Lua parse,
all208 payload hashes and the standard CLI preflight pass. Teacher annotations
and README are the only changes;206 other payloads, DLLc6ef, stock FSE, launchers
and overrides retain parent hashes. Parent and installed EXE/QST pins are exact.
Manifest SHA:
`6a8c0aa54cc3bc1924f45e9c0af800a39ab030b396f0ebe02fc9bccf5208441d`.
No installation, launch, save writes or new gameplay acceptance.

Next W236 isolate entity-state presentation lexical boundaries. The accepted
W234 annotation retains the old fragment to preserve executable tokens; unused
declarations/string/comment handling are a separate generic generator task.

## W236 in progress: entity-state lexical and receiver guards

Owned `work/codex_lua_entity_lexical_w236_20261019`; accepted state remains
W234/W235 (534 Lua, ledger371/0/850). No production changes yet.
The baseline fails15 executable/opaque-text counterexamples: state-call text in
strings/comments, fake shims, escaped receivers, shadowed receivers, getter
escapes and setter expressions. Scratch correction tokenizes code, retains
opaque text and conservatively preserves unsupported receiver uses.
Scratch tests currently252 passed, including12 added multiline expression
operator controls. The first corpus attempt was explicitly superseded after
those controls exposed an incomplete setter guard; two completed units were
unchanged, but that interrupted run is not acceptance evidence.
The corrected all29 readable-only regeneration is running in `corpus_v2.log`;
draft inputs are read-only and pinned. Book first-pass output changes only
Teacher;222 paired full Main snapshots across12 scenarios match under explicit
engine/resource/serializer doubles. These initial Book results must be checked
against the corrected final candidate before promotion. No gameplay claim,
installation, launch, save writes or changes to native/ownership authorization.

W236 update: the v2 corpus and initial broad test attempt were also interrupted
and superseded after discarded-getter/chained-setter controls exposed missing
call-context guards. The old snapshot fold independently fails three executable
probes (`snapshot_probe.json`): compact write formatting, helper write after
capture, and a same-named local in another scope. The final candidate retains
all function-local snapshots rather than claiming whole-call/whole-scope
invariance. This intentionally broadens output changes beyond the initial Book
annotation cleanup. Current focused/adjacent result278 passed (`tests_v4.log`);
final all29 regeneration is `corpus_v4.log`, broader tests
`all_readable_tests_v4.log`. Both are still pending. Final Book proof, affected
smoke comparisons, complete output review and root promotion remain pending.

## W236 scratch gates complete; W237 promoted pending root checks

Final W236 candidate retains local snapshots and uses real tokens for shim,
receiver, literal-key and call-context edits. All168 current emitted shim
preambles satisfy the canonical top-level preamble assumption; arbitrary nested
authored shims are outside this acceptance. Counterexamples and adverse controls
are durable70 cases. Final278 focused/adjacent tests and3510 broader tests plus
521 subtests pass. All29 readable regenerations/267 Lua parse;258 readable files
and all267 accepted drafts are exact. The nine changed files are DarkwoodTrader,
SkillTarget, Teacher, Spectator, BordelloLady, Dragon, SingingStone, CrateTeamMember
and TeamSpawn. Eight units affected. Inspected all475 diff lines: restored
snapshots/consistent backing-field alpha names, plus Teacher's phantom
comment-only self0X14 declaration removal and opaque comment preservation.
Final Book222 paired Main snapshots/12 scenarios match. Eight seeded20-frame
smokes have identical API traces, frame/call counts and failure identities.
TraderEscort's unchanged DarkwoodAssassinSpawn Main still reports default mock
`n%0`; no other smoke faults. This is offline mock evidence, not gameplay proof.

Full report comparison found125 differing file rows, including116 unrelated
historical metadata rows (for example absent versus empty callbackScopes and
older rewrite/source-map counters) and eight old draft-path strings. These are
not attributed to this fix and are not promoted. W237 refreshes only the nine
affected rows inside eight existing reports; other rows and report metadata stay
byte/structurally pinned. No new fallback errors; all29 syntax reports pass.

W237 owned `work/codex_lua_entity_promotion_w237_20261019`. Promoted
readable_style.py,70-case regression/fixture, nine generated readable files,
eight affected reports and ledger. Full534 root baseline pins checked first;
525 Lua remain exact. Ledger371/0syntax/850unresolved; no diagnostics removed.
Fresh root generation of all eight affected units and root regression suite are
running (`root_generation.log`, `root_tests.log`). Until these finish W237 is
pending acceptance; latest fully verified offline payload remains W235. No game,
installation, save, input, commit or push work.

Read-only follow-up inventory is
`work/codex_lua_book_dispatch_inventory_20261019/inventory.json`: original Main
native hash retained,56 resource facts and nine unresolved nil markers. Applying
the current single-phase liveness checker directly to final Main is refused for
semicolon syntax; all nine stay unresolved. Next native gate is capture the
pre-closure lowering phase and trace each table load to an actual dispatch, not
weaken the liveness checker against final generated text.


## W237 accepted: root reproduction and regression checks complete

Fresh root generation of all eight affected units reproduces the complete98 Lua
and reports from the final W236 scratch candidate exactly. All534 accepted root
Lua hashes and syntax verify, as does the entire current audit. Root294 tests and
eight subtests pass; broader scratch3510/521 and focused278 evidence remains
source-pinned. Seventy durable regression cases are in
`tools/script_recovery/test_readable_entity_lexical.py` and its fixture.
W237 verification/current_corpus receipts now mark acceptance. Nine readable
files changed,525 other Lua exact, including all267 faithful drafts. Ledger stays
371/0syntax/850unresolved. No whole-game/native equivalence claim. Next W238
refresh the offline payload from W235, then resume original Book dispatch
evidence from the read-only inventory. No game/install/save/commit/push action.


## W238 accepted: offline entity-state guards bundle

Owned `work/codex_lua_entity_bundle_w238_20261019`.
`entity-state-guards-candidate-20261019` derives from W235. All203 Lua parse,
208 payload hashes and the standard CLI preflight pass. Seven existing Lua
payloads plus README changed;200 other payloads retain exact parent hashes.
Dragon/SingingStone remain absent from existing bundle membership. DLLc6ef,
stock FSE, launchers, override membership, parent files and installed EXE/QST
pins are exact. Manifest SHA:
`9397c25426a16aec0f50fba50d3e043bfacbb27eda9b7dd1e30c02a6898d0988`.
No installation, launch, save writes or gameplay acceptance.

Next W239 continue Book Main dispatch staging. The pre-closure phase captured in
`work/codex_lua_book_dispatch_inventory_20261019` proves all10 nil writes dead
with the existing CFG checker; this alone does not reclassify diagnostics.
Native facts initially suggested resource table-load windows; W239 literal-byte
checks below establish the final LOOK/LOST site mapping and original literal/
flag/hero pushes. Table values remain in a register across GetHero, unlike the accepted
W222 spill pattern. Next prove that bounded register lifetime and original helper
behavior, and validate exact Lua regions/diagnostic multiplicity before proposing
a generic classification. Other GSI pointer/shared-cleanup aliases stay unresolved.


## W239 evidence complete: two Book resource tables survive GetHero

Owned `work/codex_lua_book_register_dispatch_w239_20261019`; no production change.
Literal-byte checks corrected the initial tentative site mapping: E55539 is
REQUEST_AGAIN and remains a control window, not a target diagnostic. LOST is
E55683/E5569B with group0; LOOK is E5573E/E55756 with group2. Both use EBX for the
resource table, self+0x34 for the receiver, and false/true/false flag pushes.
Exact exported fragments, resource/call facts, contiguous native windows and no
branch entry bypassing the load are checked. Their phase Lua aliases CVar12 are
dead (19 CFG nodes for each assignment), and the existing nil statements remain
the intended annotation target.

Complete GetHero173B hash e5f87f...45af writes neither EBX nor EBP. Initial7B
capture was only a prefix and is explicitly not the helper extent. Manager hero
getter487DC0 is an8B adjustment/tail thunk, not a RET-based ABI proof; original
54B weak getter A01B50 and the7B final base cleanup99A2E0 also execute in the
bounded packet. Actual caller windows/173+8+54+7 helper bytes pass288 native
cases and12 exact-region Lua references. Variations cover present/absent hero,
weak-holder states, forced release/free cleanup, resource-vtable mutation during
GetHero, relocated objects/two stacks/three fills/caller-clobber patterns and
Speak return values. Seven controlled failures are detected; baseline passes
before/after. Register and caller-frame ABI violations are explicit negative
controls. Remaining engine/weak-wrapper/assignment/free calls are doubles; forced
weakMode1 release is coverage, not real ownership semantics. No real engine,
heap, reentry, exception or gameplay proof. All source/accepted corpus pins exact.

Next W240 add a generic optional register-dispatch staging classifier with a new
metadata row. Place it after existing definition-staging classification so the
accepted W234 phase seal stays intact. Capture that exact new input phase, check
all native/helper/fact/Lua seals and diagnostic multiplicity, and prove each
assignment dead. Scratch regenerate Book and compare full Main paths, then29
registered units with both stages before promotion. Keep nil assignments and
every executable token; other GSI/shared-cleanup diagnostics stay unresolved.
Accepted state remains W237, latest verified offline payload W238.

## W240 in progress: optional register-dispatch staging classifier

Owned `work/codex_lua_register_classifier_w240_20261019`; all changes scratch.
The exact post-definition-staging phase is captured, preserving the W234 seal.
Candidate checks the complete retail-image/native/exported-C/Lua fingerprints,
the original GetHero identity/bytes/no-table-register-write/RET0 shape, explicit
MSVC child/caller-frame contract and proof limits, current CFG resource facts,
exact native push/call sequence and receiver origin, no branch bypassing the
table definition, exact unique Lua getter/Speak regions and19-node alias
liveness for both writes. Duplicate diagnostic multiplicity is checked before
editing. Both nil assignments and all executable tokens remain exact.

Final focused/adjacent suite209 passed (`tests_v2.log`). Initial eight test
harness failures were recursive overlay delegation, fixed in the harness;
semantic mutation controls now pass. Fresh Book8 Lua parse, only Teacher pair
annotations change. Full Main444 paired snapshots/24 scenarios match both
draft/readable stages under the existing explicit API/resource/serializer
doubles. All29 units are now regenerating BOTH stages in `corpus.log` with
source/evidence/current534 Lua/report inputs pinned; do not edit those inputs
until the run completes. Fresh readable reports compare against W236's complete
current-generator reports, explicitly normalizing only the draft path, so older
unrelated root report metadata is not mistaken for this classifier's effect.
No candidate is promoted yet; current root/ledger remain W237/850unresolved and
latest verified offline payload is W238. No game/install/save/commit/push work.


## W240 scratch gates complete; promotion next

All29 units freshly regenerate BOTH stages:534 Lua parse; only Teacher pair
annotations differ,532 unrelated Lua exact. All28 unrelated conversion and
current-generator readable reports are exact (readable draft path normalized).
Book native report changes only Main's two duplicate diagnostics, two reviewed
staging records and44->42 todo totals. Full Main444 paired snapshots/24 scenarios
match, with16 explicit LOOK and8 LOST argument/order checks. Candidate Book
20-frame smoke JSON has zero faults/unknown methods.209 focused/adjacent and53
prepared durable classifier tests pass. Code/evidence/output pins are exact.

Broader accepted-root Book baseline:487 passed, two failed,768 subtests passed.
Both failures are test_generated_teacher_dispatches_registered_worker... cases
[1-False]/[0-False]: they skip entity Init and set private `booksWanted`, while
W237 correctly retains the snapshot and uses `booksWanted_` backing storage.
Full Main tests already call Init and pass. Staged test setup now calls emitted
Init with explicit definition doubles (wanted0/accepted25) and retains all
original event/inventory/index assertions. All three cases pass when executed
from the staged module with the existing generated fixture. An initial staged
pytest-loader attempt re-collected the unchanged root module and reproduced the
baseline failures; `dispatch_fix_direct.log` is the actual staged-source proof.
No root test/source changes occurred while the pinned corpus ran. W241 will
promote this lifecycle-correct harness setup alongside the optional classifier,
then run the full root suite to validate actual module placement. Do not label
the baseline failures as classifier regressions or suppress them.

W240 verification marks scratch acceptance only. Current production remains
W237/850unresolved, latest verified offline payload W238. W241 expected audit
delta is exactly four mechanical findings (two TODO/two inline markers), not
deleted Lua statements. No game/install/save/commit/push action.

## W241 promoted pending root suite

Owned `work/codex_lua_register_promotion_w241_20261019`. Preimages and all534
accepted Lua pins checked before writes. Promoted optional
native_reviewed_register_dispatch_staging.py, converter hook,53-case durable
fixture/test, the lifecycle-correct dispatch-test setup, one Book metadata row,
Teacher pair annotations, derived Book reports and ledger. Only two Lua files
change;532 other Lua remain exact. Root source already reproduces fresh8 Book
Lua and reports exactly; all534 hashes/syntax, promoted-file pins and fresh
371-file audit verify. Ledger is846 unresolved (exactly four reviewed findings).
No executable Lua token changes. Full root Book/adjacent/classifier suite is
running in `root_tests.log`; do not claim W241 acceptance until it finishes.
Prior W237/W238 remain the latest completed production/payload receipts.
Current branch/HEAD rechecked as feat/novi-script-recovery/366c08e64a43; shared
native worktrees and unrelated dirty changes preserved. No game lease is owned.


## W241 accepted: guarded register-dispatch annotation and lifecycle test repair

Full root542 tests and768 subtests pass. The two baseline dispatch harness
failures are fixed through actual emitted Init; event/inventory/index assertions
remain intact.53 new classifier cases and209 focused/adjacent evidence are
retained. Root8 Book Lua and both reports reproduce the tested candidate; all534
Lua hashes/syntax and371-file audit match. Only Teacher pair annotations changed,
532 other Lua exact. Ledger371/0syntax/846unresolved. No executable tokens changed.
W239's288 bounded native cases/seven controls and W240's444 paired Main
snapshots/24 scenarios/24 explicit LOOK/LOST checks retain all stated child API,
ABI, caller-frame, heap, reentry, exception and gameplay limits. W241 verification
and current_corpus now mark acceptance. Next W242 refresh offline annotations
from W238; then inspect remaining Book interface-pointer/vtable staging without
weakening the phase CFG checker. No game/install/save/commit/push work.


## W242 complete: verified offline register-dispatch annotation refresh

Owned `work/codex_lua_register_bundle_w242_20261019`.
`register-dispatch-staging-candidate-20261019` derives from W238;203 Lua parse,
208 payload hashes and standard CLI preflight pass. Teacher annotations and
README are the only changes;206 other payloads, DLLc6ef, stock FSE, launchers,
overrides, parent files and installed EXE/QST pins are exact. Manifest SHA:
`c0f272287d3eee88f24a6e5fb2de2f705ef3a335f2aac679ab2d1297f597f359`.
No installation, launch, save writes or new gameplay acceptance.

Next W243 capture the late phase after W241 classification and inventory the
remaining Book interface-pointer/table aliases. The initial CVar12 nil/copy is
still in the faithful draft; readability may drop unused staging independently.
Do not assume a readable metric delta. Trace original interface receiver/cache,
stack saves/reloads, shared cleanup joins and actual pause dispatches before
proposing any classification. Keep source phases and proof limits explicit.


## W243 pointer-prefix evidence complete; production unchanged

Owned `work/codex_lua_book_interface_staging_w243_20261019`. Exact late phase
after W241 captured; all seven remaining nil values are dead under the existing
checker. Initial CVar12 *(this+4) and copied xStack_7c traverse381 CFG nodes.
Native19B E551C6..E551D9 proves ESI+4 GSI pointer load into EBX, table load,
true argument, receiver, saved cache and Pause slot5EC dispatch. The cache is
PREFIX-ENTRY ESP+20 (not function-entry ESP+20): push true shifts ESP by4 before
the [ESP+0x18] store. Existing accepted receiver-origin/table/target evidence is
retained. The100B callee witness is a prefix identity seal, not a claimed complete
pause-helper extent. The pause implementation is not executed in this packet.
48 original-prefix native/Lua pairs and seven negative controls pass. Relocated
receiver/tables, two stacks, three fills and four volatile-clobber patterns are
covered; wrong field/argument/cache offset/cache value/slot and observable Lua
alias/copy reads are detected. Lua nils are deliberately not compared as native
pointer values: their unobservability is proved separately. Actual pause-engine,
whole-lifetime cache, heap, reentry, exceptions and gameplay remain unverified.
All source/evidence/accepted corpus pins exact; no production changes.

Next W244 classify only this initial interface-pointer nil/copy under an optional
guard, after W241's phase. Retain all statements and verify projection through
cleanup/readability; current readable output already drops this initial dead
staging, so no readable metric improvement is assumed. Six shared-cleanup vtable
nil aliases need separate CFG/native saved-receiver evidence and remain open.
Accepted state W241/846unresolved, latest verified offline payload W242.

## W244 in progress: initial interface-pointer classification and projection

Owned `work/codex_lua_pointer_classifier_w244_20261019`; root unchanged.
W243 phase/evidence reused only after exact source/evidence/corpus seal checks.
The optional classifier checks full image/native/exported-C/Lua identities,
registered pause target and witness prefix/RET4 convention, entry-this origin,
entity/quest interface offset, the contiguous19B field/table/true/cache/call
sequence, prefix-relative cache coordinate, current CFG interface receiver
facts, branch-entry exclusion, exact diagnostic/region and381-node alias/copy
liveness. Existing CFG facts conservatively forget pending pushes at a stack
cache write; the contiguous native prefix proves the argument independently.
No underlying CFG model was relaxed or changed.

251 focused/adjacent tests passed. First scratch Book generation exposed a
projection regression: retagging stopped readable_dispatch_scaffolding from
recognizing the dead seed, retaining a new readable nil assignment. That output
is rejected (`book_diff.txt`, initial book_generation.log). Scratch pruner now
accepts only matching reviewed interface-pointer comment/marker pairs and keeps
all original ownership, scope, role/offset, opaque-text and no-reference guards.
23 new paired-marker cases plus adjacent checks pass142 tests; prepared durable
native/pruner tests pass70 cases. Fresh Book8 Lua parse, all executable tokens
exact; ONLY faithful draft Teacher annotations change. All four readable files
are byte-identical. Current readable ledger therefore stays846.

Full Main444 paired snapshots/24 scenarios and16 LOOK/eight LOST checks pass;
candidate Book smoke JSON has zero faults/unknown methods. All29 units are now
regenerating BOTH stages in corpus.log, and the full readability suite is running
in all_readable_tests.log. Sources/evidence/current Lua/reports are pinned; do
not edit inputs while these jobs run. No promotion yet; accepted W241 and offline
payload W242 remain current. Six shared-cleanup table aliases remain unresolved.
No game/install/save/commit/push work.


## W244 scratch acceptance complete

All29 units freshly regenerate BOTH stages;534 Lua parse. Only faithful draft
Teacher annotations change; all267 readable Lua and266 other draft Lua remain
byte-identical. All28 unrelated native/current-readable reports are exact.
Native Book report changes one diagnostic/review and42->41 todo totals; readable
report changes only Main's pruned-seed nativeStatement evidence string. Existing
private-binding/no-use/owner/offset/opaque guards remain unchanged. The initial
projection regression is superseded by the matching-pair pruner candidate.
251 native-focused/adjacent,142 readable-focused/adjacent,70 prepared durable
cases (47 classifier/23 marker), and3533 broad readable tests/521 subtests pass.
Full Main444 paired snapshots/24 scenarios/24 explicit speech checks pass;
Book smoke has zero faults/unknown methods. All source/evidence/output pins exact.
No root changes yet. W245 promotion next; ledger stays846 and W242 runtime
payload stays current because every readable Lua is identical. No new bundle is
needed for this draft-only change. Six shared-cleanup table aliases remain open.

Independent read-only preview at
`work/codex_lua_book_interface_table_facts_20261019/inventory.json` preserves all57
default facts and exposes optional interface-table facts, but still cannot prove
E55316/E55A17: conservative memory invalidation and saved-stack reloads lose the
needed identities. That preview is not promoted and is not classification proof.
Next evidence must trace those identities explicitly rather than relax defaults.


## W245 accepted: guarded initial interface-pointer staging

Root verification reproduces eight Book Lua and reports; all534 corpus hashes,
syntax and371-file audit verify.642 tests/768 subtests pass. W244 evidence retains
47 classifier/23 marker cases,251 native and142 readable focused/adjacent tests,
3533 broad readable tests/521 subtests,29-unit both-stage scratch generation,
444 paired Main snapshots/24 scenarios/24 speech checks and zero-fault smoke.
Only Teacher draft annotations changed;533 other Lua/all267 readable Lua exact.
Ledger remains846 unresolved. W243 prefix48 native cases/seven adverse controls
retain the opaque pause API, ABI and prefix-entry coordinate limits. Whole-lifetime
cache, engine, heap, reentry, exception and gameplay are unverified. W242's verified
offline payload remains current; no readable changes justify a bundle refresh.
Accepted receipts: work/codex_lua_pointer_promotion_w245_20261019/verification.json
and current_corpus.json. Next inspect six remaining cleanup interface-table aliases
with conservative provenance; existing default CFG facts remain unchanged.

Cleanup coordination request was read and a dated current-owner protection note
appended to D:/Documents/FableTLC-maintenance/coordination/lua-script-recovery.txt.
No new artifacts/actions released; all existing holds/policy rejections remain.
No desktop/game/install/save/commit/push or cleanup mutation occurred.


## Cleanup-table provenance evidence checkpoint (no production change)

work/codex_lua_book_interface_table_facts_20261019 now preserves an independent
register-bit provenance experiment.14 focused tests cover partial writes,
volatile clobbers, unknown stack reloads, bypass/different-origin joins, memory
writes and unsupported CFGs. EBX at E55316 traces to E551C6 under ordinary MSVC
callee-saved contract. This is load-site provenance, not current field equality
or heap/vtable lifetime. Existing production resource CFG remains unchanged.
E55A17 saved-cache and common E55C3F join remain unknown; six-alias bulk recovery
is not authorized by this result. Original17 bytes for E55316's branch and shared
false-pause caller pass48 native/final-Lua-prefix pairs and four adverse controls.
The final closure's later DestroyMovie is explicitly excluded from this bounded
comparison. Seeded valid interface/table and opaque RET4 API contract remain
limits, alongside full lifetime, engine, heap, reentry, exceptions and gameplay.
verification.json pins scripts, test log and evidence. Next gate is generic
load/table/receiver/branch/Lua phase plus post-hoist cleanup validation for only
the independently supported EBX alias; saved-cache aliases remain unresolved.

## W246 scratch validation in progress; saved-cache evidence remains separate

W246 work/codex_lua_cleanup_table_w246_20261019 captures the current post-W245
phase and limits classification to the labeled E55316 EBX table alias. The source
has two identical CVar12 diagnostics and four saved-cache diagnostics; only the
labeled one is reviewed. All executable tokens in fresh eight Book Lua match;
Teacher draft/readable annotations alone change. Exact report review confirms
one new staging record, one removed native diagnostic, one cleanup inline marker
and one removed readable finding (prospective ledger845; production still846).
The candidate initially reproduced a new-comment/hoister incompatibility; rejected
phase/output are retained. A guarded marker pair preserves cleanup hoisting and
all executable tokens. A deliberately conservative whole-function write-only
experiment rejects iVar8 because other lifetimes have real reads; it is NOT the
selected proof. The selected post-hoist proof checks the one nil write, six
terminal closure call sites, no helper escape/recursion and no other alias capture.
67 classifier/closure/marker cases and eight resealed native-byte mutations pass.
Book444 paired snapshots/24 scenarios/24 explicit speech checks and zero-fault
smoke pass. Adjacent cleanup tests and fresh29-unit corpus are running; source
inputs remain frozen pending results. No W246 production promotion yet.

Separate work/codex_lua_book_saved_cache_evidence_20261019 inventory records201
calls (108 direct,93 indirect),20 direct ESP stores,65 direct sites unresolved by
the existing conservative RET sweep. Those65 sites reach six unique targets.
Bounded original-byte reachable-return inventory distinguishes adjacent functions
without padding and follows three direct tail references into separately captured
bodies. Child calls remain opaque; neither global ESP nor saved-cache lifetime is
proved. No stack ABI inference, source/diagnostic change or new conversion follows
from this inventory. Native callee_stack_words remains unchanged.


## Saved-cache inventory: bounded original termination chain, no ABI promotion

The separate saved-cache packet now has reproducible inventory.py and sealed
verification.json. Nine bounded native helper extents separate normal reachable
RET operands from tail references and opaque child calls. Main has62 ESP-relative
address instructions in addition to20 direct ESP stores: stack addresses can
escape and relative displacements alone do not identify the cached pointer.
Original F35B30 wrapper12B plus CB7940 tail14B pass60 cases/five adverse controls
with an explicit RET0 child double. Original Teacher vtable012E5F40 slot0xC
points to registered GetParentScript E54620; its original4B reads [ecx+0x14],
not a guessed parent offset. A second experiment executes the entire30B chain,
including this accessor and original vtable slot, with no child/API doubles:
60 seeded valid-object/null-state cases and five adverse controls pass, normal
RET0 observed. Actual engine object/vtable lifetime, ownership, reentry, exceptions,
gameplay and full Main cache identity remain unverified. All current Lua/native
generator code, callee_stack_words and saved-cache diagnostics remain unchanged.


## W246 scratch accepted: one labeled interface-table cleanup alias

Fresh29-unit both-stage corpus:534 Lua parse; only Teacher draft/readable
annotation pair changes,532 other Lua and28 unrelated native/current-readable
reports exact. All executable tokens match. Adjacent7230 tests/1047 subtests
pass. Prepared100 portable byte-backed tests pass, including eight resealed
native byte mutations,25 register-provenance cases (11 unsupported-transfer
controls),36 classifier cases,16 terminal-closure cases and15 marker cases.
Prepared provenance rejects interrupt/system/far transfers; this guard is absent
from the original corpus's immutable source pins, explicitly retained. Independent
Main state/visit equality and fresh eight Book Lua/reports verify prepared sources
after hardening. Other28 units have no table-staging metadata and return before
provenance. Book444 paired snapshots/24 scenarios/24 speech checks and zero-fault
smoke pass. Exact report delta predicts371 files/0syntax/845 unresolved.
One labeled alias is classified; duplicate CVar12 diagnostic and four saved-cache
diagnostics remain. Six post-hoist calls are terminal and no helper alias escapes.
verification.json seals the acceptance packet; production remains W245 until
W247 promotion/root checks. W248 offline annotation refresh follows if accepted.


## W247 accepted: one labeled interface-table cleanup alias

Root742 tests/768 subtests pass. Fresh root eight Book Lua/reports reproduce
prepared W246; all534 hashes/syntax and promoted pins verify, full371-file audit
matches ledger845. Source/output preimages and receipts are retained in W247.
Teacher draft/readable annotations alone changed; all executable tokens and532
other Lua remain exact. W246's100 prepared/7230 adjacent tests+1047 subtests,
29-unit corpus,444 snapshots/24 scenarios/24 speech checks/zero-fault smoke and
native17B cleanup48 pairs/four controls retain their explicit proof limits.
The interrupt/system/far-transfer hardening is separately pinned and verified
against exact Main facts and fresh Book output, without altering old corpus pins.
Duplicate alias and four saved-cache diagnostics remain unresolved. Saved-cache
original Teacher30B termination chain60 cases/five controls is independent
bounded evidence and does not prove full Main cache lifetime. No game/install/
save/commit/push work. Next W248 refresh offline annotation payload from W242;
then trace duplicate alias provenance and remaining saved-cache constraints.


## W248 complete: verified offline cleanup-table annotation refresh

Owned work/codex_lua_cleanup_table_bundle_w248_20261019 bundle
interface-table-staging-candidate-20261019 derives from W242.203 Lua parse,
208 payload hashes and standard CLI preflight pass. Teacher annotations and
README alone change;206 other payloads, DLLc6ef, stock FSE, launchers, overrides,
parent files and installed EXE/QST hashes remain exact. Manifest SHA256:
df95ce57ea08977c6edb8e2cac44e77723f2eac580245822f66831b23b29c849.
No installation, launch, save writes or gameplay acceptance occurred.

Next W249 traces the duplicate CVar12 table alias through existing normalized-C
control-flow passes. Capture shows merge1->1, first hoist1->1, sibling-tail
duplication1->2 and second hoist2->2. A scratch optional trace leaves both default
and traced C output exact, records seven copy events, and ties this alias copy to
LAB_00e55316. Its original conditional goto becomes a preserved if(bVar6) block
containing the four-statement nil-stage/unpause/destroy/return tail. This remains
transformation evidence, not a diagnostic classification or native whole-path
equivalence proof. Source/evidence linkage through the entire lifter is the next
gate; saved-cache aliases stay unresolved. No new conversion or root edit yet.


## W249 evidence accepted: copied table alias and native WRONG_TIME exit

Existing normalized-C pipeline changes the alias count1->2 only in sibling-tail
duplication. The optional scratch trace records input/prepared/output hashes,
original jump and exact copied ranges, without changing default or traced output.
Seven Main copies include one LAB_00e55316 copy: its if(bVar6) guard remains
around the four-statement table-stage/unpause/destroy/return body. Reused trace
lists preserve prior events. Historical-only test selections initially exercised
zero intended pass calls (29/eight, then47/eight); these are explicitly excluded
from transformation coverage. Strategy changed to direct registered Book
conversion:13 actual pass calls/28 copies compare byte-exact with original output;
four fresh draft Lua and conversion report match root. Final47 tests/eight subtests
pass. Root native_goto_scopes and every other production source remain unchanged.

The original WRONG_TIME literal at E5583B leads through task polling to the
termination predicate at E55882 and conditional jump E5588B->E55316. Original
15B predicate/caller,30B wrapper/Teacher-accessor/tail,9B table/receiver/branch
and8B false-pause caller (62B total) pass60 exact Lua-clone-prefix comparisons:
36 pause exits and24 continuations, plus five adverse controls. Termination chain
has no child doubles; pause remains an explicit RET4 API double. EBX is seeded as
the independently proven earlier pointer snapshot. Comparison stops at pause;
subsequent DestroyMovie, full Main cache/lifetime, actual engine/heap, reentry,
exceptions and gameplay remain unverified. No new diagnostic classification.
verification.json seals this packet. Next wire copy provenance into a scratch
lifter/classifier, then validate the duplicate alias through native/C/Lua seals,
guard preservation, phase liveness and unrelated-unit regeneration. Production
remains W247, offline payload W248, ledger845. Saved-cache aliases remain open.

## W250 scratch in progress: opt-in native copy lineage and duplicate alias review

Owned work/codex_lua_clone_staging_w250_20261019 derives from accepted W247.
Native sibling-tail tracing records exact input/prepared/output hashes and
per-call output ranges; reused lists preserve prior events. Lifter trace state
resets each function, and converter enables it only for configured copied aliases.
The actual Main->classifier boundary carries seven events matching W249's probe;
its Lua phase seal and unique copied region are exact. A generic copy reviewer
requires the real unique event, original guard and cleanup order, original
predicate/caller window, wrapper/accessor/tail bytes and vtable slot, source
uniqueness, normal-CFG alias liveness and unchanged post-hoist executable tokens.
Only the duplicate CVar12 diagnostic is proposed; four saved-cache aliases stay.
Fresh eight Book Lua parse with all executable tokens exact. Exact reports change
native TODO40->39 and remove readable findings at lines141/142 only; prospective
ledger843 (production remains845).162 focused tests/eight subtests pass; the
430-test lifter/Book adjacent suite passes804 subtests. Prepared44 portable
byte-backed and two-mode integration cases pass. Two harness mismatches were
retained: missing write_reports/quests.lua step, then in-memory tuple versus JSON
list comparison. Full serialized reports and the four-file assertions are now
checked unchanged. No generator failures were hidden or scope narrowed.
Book444 paired snapshots/24 scenarios/24 speech checks and zero-fault smoke pass.
The29-unit both-stage corpus is running with immutable inputs; prepared-source
regeneration reproduces the same eight Book Lua/reports. No W250 source/output
promotion yet. Current production W247 and offline W248 remain protected/current.
After corpus acceptance: W251 promotion/root verification, then W252 offline
annotation refresh. No game/desktop/install/save/commit/push or cleanup action.


## W253 independent cache-overlap probe (W250 validation still running)

Original WRONG_TIME caller E5582E..E55854 (38B) spills the resource table at
[ESP+0x28] after five pushes, then reloads it from [ESP+0x2C] after the sixth:
both are this prefix's entry ESP+20.48 seeded native cases/four adverse controls
pass. A word seeded with GSI changes to the resource table while EBX retains
GSI; GetHero/Speak operands and stack restoration are checked. GetHero RET0
and Speak RET24 are explicit API doubles; EBP zero and valid seeded objects are
assumptions. The original GSI slot0x118 target is independently byte-checked.
This is a local role-change counterexample, not a proof that this prefix's ESP
equals the initial cache prefix's ESP. Global frame coordinates, path-sensitive
cache identity/lifetime, real getter/ownership, engine/heap/reentry/exceptions
and gameplay remain open. No classifier/source/generated Lua changes or release
of saved-cache diagnostics follows. Packet is sealed in
work/codex_lua_book_cache_overlap_w253_20261019/verification.json.


## W250 scratch accepted: copied interface-table alias

Fresh29-unit both-stage corpus:534 Lua parse; only Teacher annotation pair
changes,532 other Lua and28 unrelated native/current-readable reports exact.
All executable tokens match.162 focused tests/eight subtests,430 lifter/Book tests
and804 subtests pass. Prepared44 portable/regression/integration cases pass and
their fresh eight Book Lua/reports exactly reproduce the candidate. Tracing is
opt-in per configured function, clears between functions and preserves old output
and report schemas when disabled. Source inputs remained frozen across corpus.
Book444 snapshots/24 scenarios/24 speech checks and zero-fault smoke pass.
W249 native62B exit/predicate/cleanup60 Lua-prefix pairs/five controls retain all
stated seeded-object and pause-API limits. The duplicate CVar12 read is reviewed;
four saved-cache aliases remain unresolved. Exact reports predict ledger843.
verification.json seals all acceptance artifacts; no W250 promotion yet. W251
will promote five generic source modules, three regression modules/fixtures,
metadata/Teacher annotations/reports/audit, then perform fresh root verification.
W252 offline refresh follows acceptance. W253 local overwrite evidence remains
separate. No game/desktop/install/save/commit/push or cleanup actions.


## W251 accepted: duplicate alias through actual copy provenance

Root786 tests/768 subtests pass. Fresh eight Book Lua/reports reproduce prepared
W250; all534 hashes/syntax and promoted pins verify, full371-file audit matches
ledger843. Five generic source modules, three regression modules/fixtures,
metadata/Teacher annotations/reports/audit are promoted with preimages/receipts.
All executable tokens and532 other Lua remain exact. W250's162 focused/eight
subtests,430 lifter/Book tests/804 subtests,44 prepared cases,29-unit corpus,
444 snapshots/24 scenarios/24 speech checks/zero-fault smoke retain their scopes.
W249 native62B exit/predicate/cleanup60 pairs/five controls keep seeded-object,
opaque pause, later DestroyMovie, engine/heap/reentry/exception/gameplay limits.
No saved-cache diagnostic is removed. W253's38B local spill48 cases/four controls
shows why a global cache-value assumption is unsafe; relationship of frame
coordinates remains unproved. Next W252 offline annotation refresh from W248;
then path-sensitive stack/cache evidence. No game/install/save/commit/push work.


## W252 complete: verified offline copied-table annotation refresh

Owned work/codex_lua_clone_bundle_w252_20261019 bundle
copied-interface-table-staging-candidate-20261019 derives from W248.203 Lua parse,
208 payload hashes and standard CLI preflight pass. Teacher annotations and
README alone change;206 other payloads, DLLc6ef, stock FSE, launchers, overrides,
parent files and installed EXE/QST hashes remain exact. Manifest SHA256:
26d9b333d8b8694af851199ddc04fab1e7f100b7c497abf12897c297d9ca04c9.
No installation, launch, save writes or gameplay acceptance occurred.

Current source baseline W251, ledger843, runtime payload W252. The four remaining
saved-cache aliases require path-sensitive frame and value provenance. W253's
original38B wrong-time spill probe shows a local entryESP+20 role change under
explicit getter/Speak API doubles, without establishing equality to the earlier
cache prefix frame. Next inventory native call stack effects and exact register/
constructor evidence before choosing a generic cache proof. Do not assume the
slot always holds GSI or infer purges from nearby unrelated calls.

## W254 sealed: original constructor/time chains and cache epoch

work/codex_lua_book_stack_coordinates_w254_20261019/verification.json seals
59 original constructor bytes across factory/member/base:48 cases/four adverse
controls, no child doubles. Member offset34 is tied to vtable0127094C only under
explicit constructor/current-lifetime contracts; production operand recovery is
unchanged. Original GSI time getter/local conversion/shipped CRT floor execute
40 finite-input/alignment/CRT-dispatch combinations/four adverse controls with
no child doubles and explicit FPCW037F/FPSW0/FPTAGFFFF/MXCSR1F80. Only the private
VM IAT is patched; DLL/game files are unchanged. An initial adverse patch was
hidden by Unicorn's translated-code cache; explicit ctl_remove_cache after each
patch and restore fixes the harness. Only final rejecting controls count.

Normal direct CFG shows cache storeE551CF dominates readE55A17; wrong-time
spillE55840 cannot reach it without cache refresh. The last-store/read epoch has
261 instructions/48 calls, excluding that spill.39 conditional stack summaries
leave9 sites unresolved. Four further table-only GSI profiles identify candidate
purges without claiming equivalence of separately loaded receiver pointers.
Remaining direct helpers need explicit child-call/stack contracts; none is
silently caller-cleaned.25 accepted W251 source/output pins remain exact, ledger
843 and all four cache diagnostics stay. No promotion or full cache/engine/
lifetime/reentry/exception/gameplay proof. W252 remains offline, not installed.

FQT-OBJ-PRECHECK owner reply appended to maintenance coordination: manifest SHA
fd893ab7b70fda48d466a861a71ea9a0c33022ef4684439262c97b479ffc45d5 verified;
no Lua dependency on the exact four FQT Release obj files. Batch-only no-objection
to lossless in-place compression is conditional on coordinator lease/stop/hash/
identity/source checks and other owner holds. No cleanup executed here; all held
Lua trees and prior policy rejections persist. Continue offline stack evidence.

## W255 sealed: conditional coordinates, original empty-resource health path

The normal261-instruction epoch has consistent ESP equations under48 explicit
call contracts. Cache store/read both resolve to coordinate24; two direct byte
writes are at23, and modeled caller pushes/return addresses do not overlap24..27.
Four controls reject changed/missing purges. The first pass stopped at native
LEA ESP,[ESP]; explicit identity-LEA modeling completes all261 nodes. Four helper
sites retain ASSUMED normal balanced opaque-child contracts, so this is not a
recovered production ABI or whole-cache proof. Six escaped addresses are at
96/108/120, paired result/destructor uses; write extents still need evidence.

execute_empty_result.py runs original Book E554F1..E55530 and all reached native
helpers/vtables in a private VM, explicitly seeding an empty resource member.
32 alignment/fill/cache-guard cases and four adverse controls pass,60 distinct
original instructions, no child doubles. Result destruction writes12B at the
expected out slot, flag byte19 relative to this window's ESP, while the cache
guard at20 stays intact. Active-resource result/release/free callbacks remain
unverified. verification.json pins this evidence;25 W251 source/output pins exact,
ledger843/four cache diagnostics unchanged; W252 stays offline. Next inspect the
reached wrapper accessor/active-resource targets and remaining opaque contracts,
without assuming all callbacks preserve the caller's cached memory. No game,
installation, save, source promotion, commit, push or cleanup mutation.

## W256 sealed: bounded outer RET contracts, opaque children explicit

The scratch return_contracts.py follows only explicit nonoverlapping original
helper regions, direct branch/fallthrough edges and reviewed tail regions;
rejects indirect/external exits, overlapping entries, padding, truncated code,
nonuniform returns and regions without any normal RET path. Three reached
helper families CD23B9/CD2770/4AA840->99A2E0 have uniform outer RET0 operands.
Four portable positive fixtures/eight adverse controls pass. Child calls stay
opaque; this does not prove their balance or memory preservation. Replacing
five W255 site contracts with this evidence reproduces all261 coordinate states
and all four frame controls exactly.25 W251 source/output pins remain exact,
four saved-cache diagnostics and ledger843 unchanged. No production resolver
change. Next trace the four Lua cache-alias copies to their original E55A17 read
at the actual converter boundary, keeping ABI/lifetime limits explicit. No game,
source promotion, install/save, commit/push or cleanup mutation.

## W257 sealed: four real predicate branches and dead Lua cache aliases

Actual accepted converter capture proves one exported C read of CStack_7c,
one observed restore_stack_operands rename to xStack_7c, three actual sibling-tail
copy events and four emitted nil staging aliases. Four original call/test/JNE
branches reach E55A13; register snapshot analysis proves entry-this in ECX at
each F35B30 call, including callers without an immediately preceding MOV ECX.
All four pre-hoist assignments pass normal-CFG liveness; four inserted observable
reads reject. Post-hoist each assignment remains outside closures: three exact
Pause/Destroy/return suffixes, one alias-free zero-argument delegate then return.
The first probe incorrectly chose the inside-closure proof helper; actual emitted
topology corrects the harness without broadening acceptance. The initial exported
C normalized-name search was also corrected by observing the real rename pass.
Fresh four Book draft Lua remain byte-exact,25 W251 pins exact; no source,
diagnostic or runtime change. verification.json seals captures, negative controls
and correction receipt. Next local native/Lua cleanup-prefix comparison at all
four branches with cache seeded explicitly; global cache identity remains open.

## W258 sealed: all four cached cleanup prefixes, native versus actual Lua

Original101 distinct bytes comprise four call/test/JNE windows, cached-pointer
read/branch, false-pause caller and original30B Teacher predicate chain.240 native
cases match960 comparisons against the four exact post-hoist Lua suffixes;
20 native adverse outcomes and four inverted Lua guards reject. Entry ECX=this
is independently established by W257; the native cached pointer is explicitly
seeded and deliberately differs from Teacher's current interface field. All
cases preserve the cached guard, ESP and callee-saved registers. The predicate
has no child doubles; Pause is an explicit RET4 API double and Lua suspends there.
Subsequent DestroyMovie is outside the comparison. No whole-epoch cache memory
provenance, actual engine/heap/lifetime/reentry/exception/gameplay proof follows.
verification.json seals evidence;25 W251 pins exact, ledger843/four diagnostics
unchanged, W252 still offline. Next inventory every epoch memory write and escaped
buffer extent before deciding whether a generic sealed cache-staging proof is
supported under the existing ordinary MSVC/caller-frame preservation contract.

Cleanup completion FYI received: exact four-file FQT manifest batch complete,
all identities/hashes/source WIP retained, STOP-COMPRESSION restored. No broader
release or Lua action follows; existing source/work/build/runtime/ref holds stay.

## W259 conditional cache proof; W260 original empty string evidence

The generic scratch cache_epoch_proof.py checks native pointer load-site origin,
store dominance, full normal CFG, all261 epoch ESP states/48 call contracts,
two direct neighboring byte writes, caller pushes/return addresses, equal24-byte
cache store/read coordinates and declared buffer nonoverlap.18 semantic adverse
controls reject after recomputing mutated native hashes; an equivalent SIB scale
encoding for LEA ESP,[ESP] correctly remains accepted. The first intended LEA
control changed only that irrelevant scale field; the corrected negative changes
the displacement. control_correction.json records both outcomes.

The verifier initially REJECTED the baseline because W255/W256's six escaped
addresses were specifically LEA-derived. A seventh escape is MOV ECX,ESP atE555D1,
for a by-value CCharString object at epoch coordinate0. W260 executes its original
61B constructor:32 null/empty cases/four controls, two explicit DWORD receiver
stores at offset0, original accepted helper SHA matches,64-byte object guards and
counter guards preserved, no child calls on these paths. Main's real literal is
nonempty: allocation/control/data heap behavior is explicitly not covered.

W259 now passes under declared ordinary MSVC, normal child balance, caller-frame
preservation except in-bounds declared objects, and valid static target profiles.
Three12-byte result objects at96/108/120 and one4-byte string at0 do not overlap
cache24..27; seven address formations are checked. Active resource callbacks and
nonempty string allocation/disjoint heap bounds remain assumptions, not measured
engine facts. Both verification.json receipts seal these exact limits;25 W251
source/output pins exact, ledger843/four cache diagnostics unchanged. Next scratch
annotation candidate must consume the actual lineage/liveness plus these sealed
contracts, preserve executable Lua, reject adverse cases and pass fresh both-stage
corpus/behavior checks before promotion. No production source or Lua was changed.

Second FQT manifest f71fc59f3a2b0c520eb9244c3c9905fd4afef67a163832ae26d5a2936ad02e92
owner review appended:41 exact paths/44797996 bytes/hash/no overlap with prior batch
verified, no Lua dependency/timing hold; coordinator resource/lease/STOP/source
checks and all other holds retained. Completion FYI subsequently reports all
identities/source preserved and STOP-COMPRESSION restored. No scope expansion,
Lua cleanup mutation, source/Git/game/save or runtime-lease change follows.

## W261 scratch candidate in validation; production remains W251

Six staged generic sources (converter/member-provenance changes plus bounded RET,
cache epoch, call-contract and saved-table review modules) consume W259/W260 ABI
contracts and actual W257 lineage. All48 target/purge contracts are independently
re-derived from original bytes, including time wrapper/tail/local-conversion RET0.
EDI is defined as member+34 atE551D9 after the first pause, not assumed at entry;
EBP at the cache store is member+8. Copied-source tracing is enabled only when
old or saved-table copies need it and reset for every function. No current-field
equality is invented for four separately reloaded GSI receiver pointers.

Initial classifier and fresh Book8 Lua pass: only Teacher draft/readable comments
change, all executable tokens exact. Conversion JSON changes only four diagnostic
removals plus the new saved-table evidence field (unit total39->35, Main31->27).
Portable byte-backed fixture has40 regions/4374 bytes.206 focused/adjacent tests
pass, including14 rehashed native semantic mutations, an equivalent encoding
positive, bounded RET positives/adverses, optional member roles and classifier
failure boundaries. A new member fixture used address1009 inside an instruction;
it now locates the decoded call. Initial1-failed/205-passed receipt is preserved
as focused_initial_member_coordinate.*, with the correction kept explicit.

Candidate source/evidence inputs are frozen while fresh29-unit both-stage corpus
and broader lifter/Book tests run. Additional pipeline-mode tests are isolated
under validation/, outside the frozen generator input set. These check disabled
metadata preserves accepted output/reports and cache-only tracing works when the
older copy option is absent. No promotion, ledger refresh, install, game/save,
commit/push or cleanup mutation. All four root diagnostics remain unresolved
until validation/promotion is complete. Normal-child balance, in-bounds declared
objects, active-resource/nonempty-string heap/vtable lifetime, reentry/exceptions,
later DestroyMovie and gameplay limits remain explicit in candidate reports.

Forge coordination: exact metadata-only six-root scope and then exact26-object
manifest7cec5cf5620969ca91815c2f2b1b1eb86acb7f1acc59381931220e2f2f2c0ad0
received bounded no-objection owner replies, including stated read-only validation
reads, with other-owner/lease/STOP/source preservation gates retained. Completion
FYI reports identities/source unchanged and STOP-COMPRESSION restored. Two-bank
retain-untouched, all Lua/work/evidence/ref holds and prior policy rejections stay.


## W262 accepted: four saved-table annotations, conditional native cache proof

Root865 tests/768 subtests pass. Fresh root8 Book Lua/reports, all534 hashes/
syntax, promoted pins and full371-file audit verify ledger835. W261's29-unit
both-stage corpus has532 other Lua/28 unrelated reports exact.206 focused,
430 lifter/Book tests/804 subtests,79 prepared cases/seven pipeline-mode checks,
444 snapshots/24 scenarios/24 speech checks and zero-fault smoke pass. Six generic
sources/two regression modules, compact byte fixture/pinned disabled baseline,
unit review metadata, Teacher annotations/reports/audit are promoted with preimages.
All executable tokens remain exact. Native101B four cleanup prefixes240 cases/
960 Lua-prefix pairs/20 native+4 Lua adverse outcomes keep seeded-cache, explicit
pause RET4 and later-DestroyMovie exclusions.261-instruction/48-call epoch and
seven buffer escapes use explicit ordinary ABI, normal-child, caller-frame and
in-bounds/disjoint-object contracts; active resource/nonempty CString heap/vtable
lifetime, reentry, exceptions and gameplay remain unverified. W260 null/empty
constructor32/4 is not nonempty allocation proof. Initial member-test coordinate
and EDI-entry assumptions were corrected and retained. One separate wrong-time
table-spill annotation remains. W263 offline refresh next, then local spill
lineage/native-Lua evidence. No game/install/save/commit/push/cleanup mutation.


## W263 complete: verified offline saved-table annotation refresh

Owned work/codex_lua_cache_bundle_w263_20261019 bundle
saved-interface-table-staging-candidate-20261019 derives from W252.203 Lua parse,
208 payload hashes and standard CLI preflight pass. Teacher annotations and
README alone change;206 other payloads, DLLc6ef, stock FSE, launchers, overrides,
parent files and installed EXE/QST hashes remain exact. Manifest SHA256:
5fee15eda97a3095d5246dbf79dccd768092e8e59d02c129bc97b27616bb9317.
No installation, launch, save writes or gameplay acceptance occurred. Current
source/output W262, ledger835, offline payload W263. Continue the remaining
wrong-time stack-table spill annotation; W264 local native/Lua-prefix evidence
does not replace whole-function lifetime or engine/gameplay proof.


## W264 sealed: wrong-time local spill and actual Lua prefix

Original38B E5582E..E55854 executes48 local caller cases against the exact
emitted GetHero/Speak prefix; four native/two Lua adverse families reject.
Actual GSI/member static slots and member+34/saved-interface origins are checked.
GetHero RET0 and Speak RET24 remain explicit API doubles; EBP zero, valid objects
and slot contents are seeded. The initial private-VM single-page GSI-vtable map
failed before execution because slot+118 crossed its page; two pages fixed it.
Only completed final cases count. Actual dispatch-classifier phase capture
proves a hypothetical local spill has no observable reads over169 CFG nodes;
two injected observable/conditional reads reject. No assignment is emitted.
General resource inference returns no table-load fact here after temporary
cleanup. Any review must opt in to existing checked parent lifetime plus an
independent entry-this member-origin proof; no global slot-constant inference.
Accepted W262 pins and four Book draft Lua remain exact. W264 is evidence only.
W265 stages the generic opt-ins and exact downstream phase re-pinning; first
candidate generation correctly rejected stale later-phase hashes, now compared
to exact single-comment/diagnostic deltas at all five later boundaries. Full
corpus/adjacent tests remain pending. W262/W263 remain current accepted artifacts.


## W266 accepted: wrong-time resource-table spill annotation

W266 PROMOTED the Book Teacher wrong-time resource-table spill annotation. Root1001 tests/768 subtests and325 focused/adjacent cases pass. Fresh29 units/534 Lua, root8 Book Lua/reports,532 other Lua/28 unrelated reports, all promoted pins and full371-file audit verify ledger834. One generic classifier adds explicit comment-only/checked-member-lifetime opt-ins; two regression modules and portable3799-byte native fixture pin enabled/disabled output/report behavior. Five downstream phase hashes change only for the exact reviewed comment/diagnostic. All executable Lua tokens remain exact; Teacher readable has no unresolved diagnostic markers. The corpus driver retained a pre-existing Book preflight-directory collision; identical frozen inputs and all29 completed results are independently joined and checked. W264 original38B local caller prefix matches48 native/Lua pairs with four native/two Lua adverse families. GetHero RET0/Speak RET24 remain explicit API doubles; valid objects/EBP0 are seeded. Checked parent receiver lifetime plus independent entry-this member+34 origin and169-node virtual Lua-value liveness support the annotation. No virtual assignment is emitted. Book444 snapshots/24 scenarios/24 speech checks and zero-fault smoke match. No globally constant slot, active resource/heap/vtable lifetime, exceptions/reentry or gameplay proof is claimed. Earlier W262/W251/W247/W245/W241/W237 work remains accepted. Latest verified offline payload W263; W267 annotation refresh next. Continuous Lua active; journal docs/journal/2026-10/LUA_RECOVERY_2026-10-19.md. No desktop/game action without fresh screen authorization; no commits/pushes authorized.


## W267 complete: verified offline wrong-time annotation refresh

Owned work/codex_lua_wrong_time_bundle_w267_20261019 bundle
wrong-time-table-staging-candidate-20261019 derives from W263.203 Lua parse,
208 payload hashes and standard CLI preflight pass. Teacher comment and README
alone change;206 other payloads, DLLc6ef, stock FSE, launchers, overrides,
parent files and installed EXE/QST hashes remain exact. Manifest SHA256:
49d6535de0cc1dfdbea81511666220ab9cbaa7a7d7e8c5344d4cb488ba3939ab.
Current source/output W266; ledger371/0 syntax/834 unresolved, Teacher readable
has no unresolved diagnostic markers. No installation/launch/save/gameplay
acceptance. W268 existing Witch work continues: separate ESP+13 flag byte and
EBX/ESP+14 CString cleanup mask,64 original212B local cases/five native controls,
64 actual Lua-prefix pairs/two Lua controls. Actual14-Lua SickChild draft capture
matches accepted output. Initial hypothetical-local CFG proof rejected unsupported
syntax; no checker change/CFG proof is claimed. Whole-function executable alias
absence plus four injected observable-read controls passes. Both packed diagnostics
remain explicit pending native exported-marker/CString cleanup correspondence.
No source/Git/game/lease or cleanup change from this evidence work.


W266 audit correction receipt: candidate initially removed the final finding
but retained a zero category key and the old file status; preparation rejected
that mismatch. Correcting the file status exposed copied aggregate status totals
at fresh root audit. Only totals41 needs-recovery/180 needs-readability-work/150
needs-human-review were corrected; all371 file rows and834 diagnostic count were
already exact. Initial candidate/log and audit_summary_correction.json are retained
under W266. Root verification then passed;1001 tests/768 subtests passed330.07sec.


## W269 sealed; W270 staged: preserve both Witch hit-query string lifetimes

W269 covers two existing Witch scopes,424 native caller bytes/128 local cases,
128 native/host-C++ helper lifetime comparisons,16 symbolic boolean paths/24
semantic controls and two equivalent no-index SIB controls. Source/call-order
mapping identifies18 constructor/query/destructor calls and independent entry-this
origins. Native flags at SP13 and adjacent cleanup mask SP14 remain distinct.
Each symbolic mask preserves arbitrary other29 incoming bits under its explicit
three-temporary-bits-clear precondition and normal child/frame/object contracts.
Existing resources:IsHitByHeroExceptAbility is already bound in the c6ef sidecar.
Its extracted C++ body matches native normal reverse-cleanup topology. Separate
old LuaEntityAPI calls destroy each hero-name temporary too early on4/8 boolean
paths, despite agreeing on final bool. No sidecar implementation/DLL change is
needed. C++ host/doubles are not retail32 heap/ABI or gameplay acceptance.

First-scope cleanup locals bVar7/bVar8 are read again, so their false exit values
must be preserved. Two initial prefix tests seeded them nil/numeric and failed;
full-function false initialization and closed bool-only writes exclude those
inputs.1024 valid-prefix/exit-local checks pass. Four other scratch-result
lifetimes are dead over normal Lua CFG;8 injected reads reject. One exact inline
IF BREAK expands only in analysis; shared CFG checker remains unchanged.

W270 optional generic hook/new role/recovery modules and unit metadata are staged
only. Disabled hook28 SickChild Lua/full conversion report exact. Enabled scratch
28 Lua parse:26 exact, only Witch draft/readable change; two helper calls preserve
numeric0/1 projection and first-scope false locals. Two packed diagnostics are
removed; call accounting changes six query calls to two helper calls. Three
unchanged cancellation sites move324->303,386->365,463->426; initial report check
omitted those coordinates and was corrected using unchanged source-line blocks.
76 portable classifier cases pass. Initial44 legacy passes were baseline-only:
the persistence fixture spawned a root subprocess bypassing staged imports. Its
existing ENTITY_PERSIST_TEST_ARTIFACT override now points to W270 scratch.
Actual candidate initially32 failures/12 passes because test resource doubles
lacked the existing helper; explicit API doubles then pass44 actual candidate
hit-gate cases. All failure logs/runner corrections are retained.

Nine existing test adaptations are staged, preserving shared files. Focused
fixtures retain parent registrations required by the full-phase review, supply
actual generated helper assets to standalone Lua VMs, and disable the new review
only for an intentionally older movie-baseline reconstruction. First broad run
was stopped after duplicated fixture errors (exact owned PIDs only); second ran
8355 cases successfully before missing helper assets caused10 readable-fixture
failures. Those assets are now supplied; broad suite and a fresh29-unit both-stage
corpus are active. No acceptance is claimed for interrupted/failed runs. Current
accepted source/output remains W266 (1001/768 root); verified offline W267,
ledger834. W270 remains unpromoted; no game/install/save/commit/push/lease/cleanup
change. Native heap/object lifetime, exceptions/reentry and gameplay gates remain.


## W270 sealed / W271 accepted: two Witch hero-hit lifetime scopes

W271 PROMOTED two existing Witch hero-hit query scopes using the already bound IsHitByHeroExceptAbility helper. Root23225 tests pass, including full enabled/absent-metadata pipelines. Fresh29 units/534 Lua,532 unrelated Lua/28 fresh reports, fresh root28 SickChild Lua/reports and all promoted pins verify. Full371-file audit has0 syntax failures/832 unresolved; Witch retains one separate speech-table base diagnostic. Original two212-byte native scopes,128 native caller traces and corrected host C++ enum comparison establish normal short-circuit/CString cleanup topology under explicit valid-object/heap/child-ABI contracts. Numeric0/1 results and outgoing boolean cleanup locals are preserved. Portable adverse tests reject stale metadata, changed native semantics and escaping Lua scratch. Existing fixture/API doubles were updated explicitly; earlier failing logs and fresh-report baseline corrections remain retained. Witch mock smoke passes; Sister modulo-zero fault matches absent-metadata baseline. No active heap, exceptions/reentry or gameplay proof is claimed. Sidecar source/DLL unchanged. Earlier Book W266 and prior accepted work remain. Latest verified offline payload W267; W272 Witch refresh next. Continuous Lua active; journal docs/journal/2026-10/LUA_RECOVERY_2026-10-19.md. No desktop/game action without fresh screen authorization; no commits/pushes authorized.

W270 candidate:23059 existing tests100.00sec,78 portable/full-pipeline cases145.92sec and88 adjacent tests0.91sec. Corrected host C++ enum explicitly contains ability14 with int underlying type; supersedes only W269 host enum placeholder evidence. Native traces and extracted helper/binding bodies remain unchanged. Full corpus original comparator failures retained; accepted fresh W265 reports and frozen inputs independently verified all29 results. Legacy stored reports have omissions and other recorded drift; no blanket empty-key-only claim. Pipeline initial external pytest temp failed repository-relative draft precondition; retained unique owned work scratch fixes the fixture. Root driver initially retained Book unit filename and failed before output; corrected driver receipt retained. No production guard was loosened for any fixture correction.


## W272 complete: offline Witch lifetime refresh; W273 next evidence

Latest verified offline payload W272 witch-hit-lifetimes-candidate-20261019:203 Lua/208 hashes/standard CLI preflight; Witch Lua/README alone differ from W267,206 other payloads and DLLc6ef unchanged. Manifest 1709b86ec09e4879773f2f00ab9e62ba79ba859cfd6843b15da9bbd405f34bc6. Not installed/launched. W273 next existing Witch speech-table spill evidence is unclassified:52-byte caller/actual15-byte borrowed-string getter,64 native/Lua argument pairs and seven adverse families; source marker remains explicit.

W273 compares argument topology only. GetHeroRET0/SpeakRET24 are explicit doubles, with EBP0, valid parent+58 object/static vtable and CString handle/storage seeded. Actual15B borrowed-string getter runs for empty/nonempty strings. Whole Main ESI entry-this origin checked. Native Speak result/yield/poll semantics, active heap/string/resource lifetime, reentry/exceptions/gameplay are not proved. No production classification from W273 yet.

## W273 sealed evidence; W274 member-table classifier scratch pending

Accepted source/output remains W271 (23225 root tests; ledger832); latest verified offline bundle remains W272 (manifest1709b86ec09e4879773f2f00ab9e62ba79ba859cfd6843b15da9bbd405f34bc6). W273 work/codex_lua_witch_speech_spill_w273_20261019 is sealed bounded evidence only. Original52B ECF0F9..ECF12D and actual15B borrowed-string getter match64 native/Lua argument pairs/seven adverse families. Fresh normalized C and actual Lua phase capture reproduce14 draft Lua/full report and35 accepted pins. Inference independently identifies ECF0FC as parent+58 resource table, feeding reviewed speech ECF12A. Actual dispatch phase SHA e030b4e2d5580d80d19fe3af52a2301f80c4226b1c1c160ff738e83a88a26fae still matches W268 before the later W271 hit hook. Alias xStack_60 has0 executable identifiers. Final emitted restricted-CFG proof rejected semicolon cleanup syntax; earlier phase proof rejected absence of a declared alias. These refusals are retained, with no CFG liveness claim or checker change. The C stack slot is also used earlier for a string object: no whole-function slot type/lifetime inference is valid.

W274 work/codex_lua_witch_member_table_w274_20261019 stages an optional generic member-table comment classifier plus converter hook; shared production files unchanged. It repeats the existing checked member-speech operand/receiver/stack replay against actual normalized C, requires independent inferred parent resource-table load, checks unique original/normalized table path and source call, rejects branch entries, requires0 executable Lua alias identifiers, and changes comments only while asserting executable-token equality. Current49 portable positive/adverse tests pass with retail image opens blocked and explicit child purge facts. Whole-unit draft/readable generation remains pending; no promotion or corpus pass claim yet. Initial portable spec omitted required LoweringSpec attributes; corrected fixture uses actual LoweringSpec, retained failure. Initial whole-unit generation caught CRLF->LF normalization in metadata preparation (not production): exact normalized C is now read with newline empty, metadata/fixture regenerated, exact hash guard unchanged; partial draft/metadata/logs retained in initial_crlf_mismatch. Current generation retry active. One older-baseline initial-movie fixture is staged to opt out of the new current-phase review alongside its existing hit-query opt-out; no behavior assertion removed. Prepared corpus driver compares unchanged unrelated readable reports against sealed fresh W270 products, with current readability generator source hashes checked, avoiding known stale stored-report baseline drift. No new corpus started yet.

No install/game/desktop/save/commit/push/runtime lease or cleanup mutation. All W273/W274 and earlier source/work/build/runtime/evidence/game/save/ref artifacts held; no inactive releases. Existing419/spent/two-bank/Choice41 metadata/policy holds persist.


## W274 sealed / W275 accepted: Witch member speech-table annotation

W275 PROMOTED the remaining Witch member-speech table comment classification. Root23198 tests, fresh28 SickChild Lua/reports, all534 hashes/syntax and full371-file audit pass; ledger831. Fresh29-unit corpus changes only one Witch comment in each stage; all534 executable Lua token streams,532 other Lua and28 unrelated fresh reports remain exact. Witch readable has no unresolved diagnostic markers and still needs readability work. The optional generic classifier repeats checked member-speech operand/receiver replay against exact normalized C, independently checks parent+58 table provenance, unique source correspondence and no entering branches, and requires zero executable Lua alias identifiers. No assignment is emitted or removed. W273 original52B caller plus actual15B borrowed-string getter match64 native/Lua argument pairs/seven adverse families. GetHeroRET0/SpeakRET24, valid storage/objects and EBP0 remain explicit contracts. The C slot has an earlier string role; no global slot type/lifetime or active heap/reentry/exceptions/yield/gameplay proof is inferred. W27449 portable adverse tests block retail image opens; full enabled/absent-metadata pipelines and23059 existing candidate tests pass. All13 mock smoke entries equal the accepted trace, including the unchanged Sister modulo-zero fault; Witch passes. Earlier W271 hit-query lifetime recovery and Book W266 remain accepted. Latest verified offline payload W272; W276 annotation refresh next. Continuous Lua active; journal docs/journal/2026-10/LUA_RECOVERY_2026-10-19.md. No desktop/game action without fresh screen authorization; no commits/pushes authorized.

Exact C-input hashes preserve original CRLF. Initial metadata preparation normalized it to LF and whole-unit generation refused; correction changes only file-reading fixture/preparation, with original hash guard intact. Initial partial outputs/metadata/failure notes retained. Portable tests use actual LoweringSpec after the first incomplete test double lacked resource fields. Initial-movie older-baseline fixture opts out of the new current-phase review alongside its previous hit-query opt-out. Prior CFG refusals remain explicit; lexical absence supports only a comment classification, with no virtual-local/CFG proof claimed.


## W276 complete; W277 TalkingTrader1 native counter evidence

Latest verified offline payload W276 witch-member-table-candidate-20261019:203 Lua/208 hashes/standard CLI preflight. Witch comment/README alone differ from W272,206 other payloads and DLLc6ef unchanged; all executable Lua exact. Manifest cc790346dcf76e0f26513d1a4c138430e2842106400efab276ee437010dfa2e6. Not installed/launched. W277 existing TalkingTrader1 native counter evidence continues: original25B increment and23B reset each192 cases/six controls; full-loop scalar origin/phi and missing later conversation operands remain unresolved. No counter implementation/promotion yet.

W277 work/codex_lua_talking_trader_counter_w277_20261019 inventories registered TalkingTrader1.Main ECBDF0,2529 owned bytes/2910-byte extent. The original export body ranges omit381 bytes including378-byte conversation case block span ECC07A..ECC1F4 and three padding bytes; this discrepancy is retained, not treated as absent code. Original25B ECC299..ECC2B2 executes INC EBX/store ESP+40 only after a nonterminating query. Original23B ECC1F4..ECC20B clears that slot only on nontermination while preserving the incoming EBX. Each192 cases includes signed boundary bit patterns, two addresses/stacks/fills and termination bytes0/1/255; each rejects six fault families. QueryRET0/volatile clobber/callee-saved/frame behavior is explicit API double evidence only. Full-loop initialization/reset/reload/phi and earlier overlapping CString lifetime still need native correspondence. Later conversation lines currently have missing receiver/participant operands, so counter-only promotion is not cleared. Existing stack-scalar review machinery may be applicable after those proofs; no new conversion/registration or implementation yet. Primary existing AddLineToConversation API/binding signatures inspected read-only. No game/install/save/commit/push/lease or cleanup change.


W277 SEALED NATIVE EVIDENCE / W278 ACQUISITION / W279 UNPROMOTED CANDIDATE
Last accepted root remains W275 (ledger831/23198 tests); latest verified OFFLINE bundle remains W276 manifest cc790346dcf76e0f26513d1a4c138430e2842106400efab276ee437010dfa2e6, not installed/launched.
W277 sealed work/codex_lua_talking_trader_counter_w277_20261019: original reset/increment prefixes each192 cases/six controls; original four conversation scopes128 trials/288 line events/seven controls. Independent argument replay agrees on all27 scoped calls,21 absent export records and all170 source/native heads. Preserve native TRADERA_CHAT3_30 speaker OTHER/listener ME. Late EDI push accounts for early ESP+3c/later ESP+40 scalar slot; earlier CString lifetime ends before numeric initialization. Registered vtable/owner/parent thunk and original child purge bytes checked. Real heap/ownership/exception/reentry/yield/gameplay remain unproved.
W278 work/codex_lua_talking_trader_metadata_w278_20261019 evidence/probe only: original171-byte acquisition prefix64 cases/seven controls and192 parent-flag variants/nine controls. Actual registered parent getter and null-thread query branch execute; resource/find/remove/CString footprints are explicit doubles. Scratch TU diagnostic retained, original TU unchanged.
W279 work/codex_lua_talking_trader_candidate_w279_20261019 unpromoted: optional scoped missing-call overlay validates original bytes/source/metadata, registered owner, frame/counter/participant/ID roles and independently replayed21 missing records. Index restored on success/error. Early scalar aliases separate numeric counter and cleanup-mask uses before global stack aliasing; existing CString uses preserved. Three retained-array CString arguments have checked single-entry native live-object proofs. Original TU untouched. Focused53 tests pass; final28 draft/readable Lua parse,26 unrelated files exact. Actual emitted Main/Init in both stages passes24 ambient-loop cases, original nine-line/four-conversation sequence and participant quirk. Timers/near actors/no-talk/no-hit/resources remain explicit doubles. No blanket TalkingTrader1 equivalence or hit-string-lifetime claim. Full enabled/absent-metadata pipelines and frozen29-unit corpus ACTIVE; no promotion/acceptance claim. Source inputs frozen during these jobs.
No game/desktop/install/save/commit/push/runtime lease or cleanup mutation. All source/dirty checkout/work/build/runtime/evidence/game/save/ref artifacts through W279 stay held. Exact inactive releases NONE. No exclusive idle guarantee; no disk emergency or cleanup dependency. All419 spent paths, both RETAIN UNTOUCHED banks, Choice41 exact saved-cursor metadata budget/gates and prior policy rejections remain unchanged. Exact Choice41 metadata-only NO OBJECTION remains conditional on its per-directory lease/STOP/reparse gates; no content/mutation or scope expansion cleared.


W280 SCOPED REVIEW CORRECTION - UNPROMOTED
W279 frozen first corpus remains active, no source inputs changed while it runs. Its first broad-regression attempt lacked the readable artifact at the configured test root (failure retained); the corrected artifact exposed an implementation refusal when a unit was narrowed to Witch but still carried TalkingTrader1 metadata. Separate W280 work/codex_lua_talking_trader_scope_w280_20261019 filters reviews by selected function addresses before validation. Excluded reviews cause no native read/state reset; active reviews retain all native/source/owner guards.56 focused tests pass, including three narrowed-unit cases. Fresh28 Lua and normalized reports are identical to W279. Full58-case portable/pipeline and broader existing regressions ACTIVE. Fresh final corpus will use the corrected frozen sources; no accepted root claim.
Additional native/effect checks: original talk/movie scopes128 cases/four instruction-preserving controls; generated draft/readable match64 cases with ordinary Speak success, health/task/termination doubles and exact movie cleanup. Original hit/query/health/mask/cleanup scope128 cases/two controls; generated stages match32 query/argument/short-circuit/flag cases. Native strings stay nested across queries/health, whereas existing separate method bindings end each string sooner: retained recovery gap, no lifetime/whole-entity/gameplay equivalence claim. W279 initial empty-string reader assertion is corrected with original NUL byte (RData printable reader intentionally returns None for empty strings). Initial controls that damaged addressing were replaced by instruction-preserving condition/offset mutations; old receipts retained.
Smoke initial200-frame run is retained separately; comparable20-frame smoke retains only the known Sister modulo-zero fault. Comparison receipt still pending. W275 remains accepted root (ledger831); W276 remains latest verified offline bundle. No game/desktop/install/save/commit/push/runtime lease or cleanup mutation. All source/work/build/runtime/evidence/game/save/ref paths through W280 held; exact inactive releases NONE. All419/spent/two-bank/Choice41 exact-cursor metadata/policy protections persist. Heavy checks active, no exclusive idle guarantee.


W284 REVIEW-STATE FIX ACTIVE; W280 CHECKS UNPROMOTED
W28058 portable/pipeline and23259 existing regressions pass255.35sec. Final W28029-unit corpus remains active; no production changes. Static review found retained_cstring_reviews survived a scoped conversion and could leak into a later unreviewed report on the same converter. W284 work/codex_lua_talking_trader_review_state_w284_20261019 now restores previous presence/value by identity in finally, alongside original metadata index, after success/error.60 focused tests pass, including four state/restoration/reuse cases. Fresh emitted unit identity/behavior tests ACTIVE. Prepared full pipeline adds disabled->enabled->disabled reuse on the same actual UnitConverter, asserting no proof attribute remains and comparing both stages/reports; expected63 tests. No acceptance or promotion claim. W281 prospective promotion scaffolding is unexecuted and superseded; root remains accepted W275/ledger831, offline W276.
W283 work/codex_lua_talking_trader_health_w283_20261019 evidence only: original hit scope128 cases with explicit child/x87-health doubles measures one or three live CString temporaries during selected GetHealth calls. Actual existing query-only helper source compiled on host using an enum that explicitly includes14 has matching16 control results, but a separate health call observes zero live strings in all10 selected cases. Query/cleanup events otherwise match. Existing query helper alone therefore cannot close TalkingTrader1 health lifetime gap. No new helper/SDK/DLL/Lua implementation or gameplay claim. Primary source/header pins and original native bytes retained.
No game/desktop/install/save/commit/push/runtime lease or cleanup mutation. Heavy candidate checks active; no exclusive idle guarantee. All source/dirty checkout/work/build/runtime/evidence/game/save/ref paths through W284 held; exact inactive releases NONE. All419/spent paths, both RETAIN UNTOUCHED banks, exact Choice41 saved-cursor metadata budgets/gates and prior policy protections remain unchanged.


## W285 accepted: TalkingTrader1 scalar and conversation recovery

W285 PROMOTED TalkingTrader1 counter and conversation-operand recovery:21 missing call records complete all170 native/source heads; signed32 increment/reset/reload and numeric cleanup-mask lifetimes restore without editing the original TU. All nine lines/four conversation cases retain native actor/listener order, including the third-line speaker quirk. Optional generic reviews apply only to selected functions and restore original metadata/proof state after success/error. W28423259 existing regressions,63 portable/full-pipeline cases and fresh29-unit/534-Lua corpus pass;532 other Lua/28 unrelated fresh reports exact. Root63 tests include disabled-enabled-disabled reuse on one converter; fresh28 SickChild Lua/reports, all534 hashes/syntax and full371-file audit pass. Ledger828; three diagnostics recovered. Both stages pass24 ambient,64 talk/movie and32 query/control comparisons under explicit API doubles. All13 smoke entries equal baseline, including the known Sister modulo-zero fault. Prior Witch/Book recoveries retained. This remains partial entity recovery: existing nested hit CString/health lifetime and unordered NaN predicate gaps remain explicit. W283 actual46-byte GetHealth/896 cases and W287896 host predicate pairs/four controls/two dead aliases support the next step; no SDK/DLL/helper integration yet. Latest verified offline payload remains W276; W286 counter refresh next. Continuous Lua active. No desktop/game action without fresh screen authorization; no install/save/commit/push or runtime lease changes. Gameplay gates remain open.

Sealed W279/W280 full-unit checks are superseded only for their scoped wrapper/proof-state issues; their artifacts and initial driver/fixture/control failures remain retained. W284 includes the corrections and exact reuse test. W283 initial receipt-import error was corrected and896 actual-child cases rerun. W287 shows20/1792 candidate NaN mismatches and16/896 physical-only accepted-baseline mismatches: existing Lua <=0 clear logic disagrees with native ordered >0 for NaN. Its host predicate fixes tested control/lifetime topology but has no sidecar binding/build/retail32/runtime acceptance.


## W286 offline bundle verified; W288 exact health-query phase

Latest verified offline payload W286 talking-trader-counter-candidate-20261019:203 Lua/208 hashes/standard CLI preflight. Only TalkingTrader1/README differ from W276;206 other payloads/DLLc6ef/parent/game hashes exact. Manifest b89e550283a35541f9888366a2845b9c2dd7c8553272eb85eec5fc90bcb6a269. Not installed/launched. W288 captures the exact next hit-review phase; three existing later speech-cancellation guards explain its difference from final Main. Native entry/flag roles and SDK predicate integration remain next; no new helper/DLL integration yet.

W288 work/codex_lua_talking_trader_health_phase_w288_20261019 captured actual generic hit-review hook source/native function/calls/TODO from current TalkingTrader1. Initial equality-to-final assertion failed because the unchanged later native_blocking_speech_cancellation pass adds three existing Speak cancellation guards. Original script/log/diff retained; applying that exact pass yields current full Main, and narrowed final output independently equals full-unit Main. No shared checker relaxation, source rewrite, helper metadata row or SDK integration. Native registered entry/frame/flag/health-local roles and future full-function transform checks remain required.


## User stop checkpoint: W291, 2026-10-19 evening

User explicitly requested: "Let's call it here for today. Update docs etc for next session." Lua work is PAUSED. No owned recovery processes remain; no SDK build or new verification run was started after this request. Native lane status is independent and unchanged. Branch feat/novi-script-recovery, HEAD 366c08e64a4323a6147fe4d5caa675e31799d51e; shared dirty work remains uncommitted. No commit or push.

Accepted state: W285 root recovery, ledger 828, 23,259 existing candidate regressions, 63 portable/pipeline and 63 root tests passed. Fresh 29-unit/534-Lua corpus; 532 unrelated files/28 unrelated reports exact. W286 latest verified offline payload talking-trader-counter-candidate-20261019, manifest b89e550283a35541f9888366a2845b9c2dd7c8553272eb85eec5fc90bcb6a269, unchanged DLL c6ef7f5093a163422542c4982f02974c98e912135bf332263062a81d00d641ce. 203 Lua parse/208 hashes/standard CLI preflight passed. Not installed or launched. Current accepted Lua still has the nested hit CString/health lifetime and ordered-positive/NaN gaps. Known Sister modulo-zero fault remains; no whole-entity or gameplay acceptance.

Pending evidence and candidate locations:

- work/codex_lua_talking_trader_health_phase_w288_20261019: actual current hit-review phase/calls/native metadata. capture.json passes. Three later existing speech-cancellation guards explain the difference from final Main; complete_capture.py reconstructs exact final Main. Initial equality assumption/script/log/diff retained. No generic rewrite or metadata helper row implemented.
- work/codex_lua_talking_trader_health_roles_w289_20261019: owned native_hit_health_query_roles.py, proof.json (32 category paths vs 896 original caller/actual GetHealth cases), controls (22 rejected faults, two neutral variants), entry_frame.json and seven rejected entry controls. Scope ECC54A..ECC63C, 242 bytes; registered entry, this/actor roles, depth 236, flag/mask/CString topology checked. Stack solver uses 170 exported pre-call depth anchors and explicit child normal-return ABI contracts; it is conditional proof, not all child-body execution. Initial pushedStack-as-purge error and log retained: nested calls can retain pending outer Speak arguments. Neutral variants have not yet run in native VM. W288/W289 remain owned evidence, not production promotion; stop receipt hashes their top-level files without rewriting them.
- work/codex_lua_talking_trader_health_sdk_w290_20261019: prepared.json says prepared_owned_sdk_candidate_not_built. Independent clone sidecar_candidate, branch codex/talking-trader-hit-health-20261019, base 58182377795548b2f171750e1575ca0c18b06138. Exact W287 host-tested IsHitByHeroExceptAbilityAndHealthAbove method plus sol binding added only in owned clone. Header SHA256 5685bcdbcfd9a637fef3b22e3021afd41bde67660592835f4daf55783da392a3; full working patch 5e0b7546b9d115a7d44325a18cc8b009059083586400f897f2b66a2abb191f9b includes preserved prior SDK edits. Parent SDK work/codex_lua_movie_destroy_reentry_20261016/sidecar_candidate stays dirty only in LuaRetailResources.h; original HEAD/status/header rechecked unchanged at stop. No build or DLL produced. Source/binding preparation alone is not retail ABI acceptance.

Next session sequence:

1. Read current handoff and this checkpoint; inspect W291 stop_receipt.json and exact pending files. Do not rerun sealed W277/W278/W279/W280/W283/W284/W287 scripts or obsolete W281 promotion scaffolding. W287 historical root source pins now map to preserved W285/preimages; document provenance if reused rather than changing sealed receipts.
2. Review W288 phase and W289 conditional entry/frame/flag evidence; complete necessary adverse/native neutral checks and exact-phase local liveness/default/refusal/report accounting before any generic transform. W287 final-Main dead aliases are not yet exact-hook proof. Native strings must remain nested across conditional GetHealth; a query-only helper followed by health does not suffice. Preserve native ordered health > 0 (NaN false).
3. Find installed v143/MSBuild via C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe (not yet queried; MSBuild absent from PATH). Build ONLY W290 owned clone Release|x86 (maps Win32), /m:1, with owned empty UserRootDir and ImportDirectoryBuildProps=false / ImportDirectoryBuildTargets=false to avoid ambient imports. Project has no reviewed post-build copy/game action. Verify 32-bit calling conventions/stack/x87 result handling, binding, source pins and unchanged parent. Host prototype 896 exact pairs/four controls is not SDK or retail execution proof.
4. Only after ABI and exact generic lowering evidence: scratch both generators, compare native/actual emitted behavior including NaN, full narrowed/enabled/absent/reuse pipelines and unrelated registered corpus. Promote only with full required checks and refresh offline bundle afterward. No hand-editing generated Lua or new conversion selection.

No game/desktop/focus/input/capture without fresh screen authorization. No installation, saves, runtime lease, cleanup mutation, source reset, deletion/move, commit or push. All source/work/build/runtime/evidence/game/save/ref and both parent/new SDK trees through W291 held; exact inactive releases NONE. All 419 spent paths, two-bank RETAIN UNTOUCHED decision, Choice41 saved-cursor metadata budgets/lease/STOP/reparse exclusions and prior policy rejections remain. Owned lane idle does not clear another lane or lease. No cleanup dependency or resource urgency identified.
