# Native recreation continuation - 2026-10-19

User resumed continuous native recreation, excluding Lua, until asked to stop.
Existing dirty workspace preserved. No game launches, installed-file/save changes,
commits/pushes, full gate_all or dashboard regeneration.

## Immediate worker retirement composition in progress

Following the October18 bedtime handoff, added a separate fixture and gate:
- rebuild/tests/integration/NativeThreadedWorkerImmediateRetirement_test.cpp
- tools/decomp_pipeline/gate_native_threaded_worker_immediate_retirement.py

The actual AddObject / worker / APC / OnFinishObject graph now receives
QueueWhenFinished=false. Completion callbacks inspect the live overlap before
retirement; the main thread polls using a copied UID, then inspects independent
buffer/storage and empty manager trees without dereferencing freed requests.
A GlobalFree IAT observer forwards to Windows and an operator delete observer
forwards to CRT free; each request/overlap must retire once before shutdown and
remain retired after worker join / manager destruction. No canonical root changes
or new root credit. Existing callback priority moves/refusal/retry remain covered.
Three additional defects retain completion, omit UID erase or omit request delete.
First gate launch caught a Python argument separator typo before compilation;
corrected before running validation. Validation currently in progress; no PASS yet.

Final validation PASS:672 cases,336 reads/336 writes,336 completion callbacks,
672 immediate request deletes and672 overlap releases; no second retirement on
shutdown or manager destruction.168 read-begin queue moves/refusals/retries remain
covered.50 linked bodies verified,36 required source omissions,one canonical
rebalance selection control and12 runtime controls; attached360/2016 pending-map
original/rebuilt/model corpus and13 removed-code controls also pass.
Independent final audit confirms198 input pins and51 object pins unchanged
(see receipt). Registered focused gate in gate_all;
full gate_all not run. No root or coverage credit added.
Receipt: rebuild/build/native-worker-immediate-retirement/00/98/0098ecd0/20261019T133323144721Z/receipt.json.

Next: compose several real worker requests in one manager, mixing retained and
immediately retired ownership, to exercise nonempty UID/pending maps and worker
shutdown with both ownership policies present. Preserve this passing fixture.

## Mixed worker batches in progress

New independent NativeThreadedWorkerMixedBatch_test.cpp and
 gate_native_threaded_worker_mixed_batch.py queue28 actual AddObject requests in
one production-created manager, with both devices/all seven priorities present.
24 batches cover672 real requests, mixed read/write, callback/no-callback and
retained/immediate ownership. Callback contexts bind a request-specific slot.
UID red-black invariants, ordering, values, extrema and count checked after each
submission and at completed/shutdown states. Independent guarded buffers and full
owned write file checked. Release observers track each request and overlap and
require immediate retirements before shutdown and retained releases only at exit.
Existing13 runtime defects include forced retirement of retained requests.
Validation in progress; no PASS claim yet. Previous passing fixture untouched.

Mixed batch final PASS:24 manager lifetimes/672 requests,336 reads/336 writes,
336 callbacks,336 immediate retirements/336 exit-owned requests,672 overlap frees.
168 callback priority moves/refusals/retries;50 selected linked bodies,36 required
omissions,one selection control and13 runtime controls all pass. Existing360/2016
pending-map original/rebuilt/model and13 code-removal controls retained. Independent
198 input/51 object hashes checked with zero drift. Gate registered in gate_all;
no full run or new root credit. No canonical roots changed.
Receipt: rebuild/build/native-worker-mixed-batch/00/98/0098ecd0/20261019T133751562800Z/receipt.json.

Next: replace the pre-signaled installer acknowledgment with a genuine installer
worker entry/ThreadMain pause/resume loop and owned OS events. Idle installer
processing remains a declared boundary; no installer filesystem operations.

## Real installer acknowledgment composition in progress

Separate NativeThreadedWorkerInstallerHandshake_test.cpp / matching gate now use
actual FableInstallerWorker_009C5870 and CFileInstaller::ThreadMain with real
unsignaled auto-reset Event/PauseStartEvent/PauseFinishEvent. Each batch starts
an owned installer CRT thread before the actual manager thread;672 pause waits,
installer acknowledgments and resume signals are observed through forwarding
Windows IAT observers with issuing-thread checks. Installer startup list heads
and object storage are fixture-owned; ProcessReads/ProcessAlways are controlled
idle methods and active completion/write/decompression methods trap. No production
installer construction or filesystem/decompression claim.
Existing runtime13 controls plus absent installer worker/acknowledgment/pause wait.
Callback lock check uses owner thread rather than RecursionCount to permit real
installer-thread contention. Validation in progress; no PASS yet.
Independent retail disassembly confirms Lock00A62980 ends at00A62998 (25 bytes);
first draft gate hard-coded29 and will need correction before acceptance.

Corrected the Lock extent25B and retained first failed build134129165596Z.
Second build134256931773Z positive672 requests/672 real acknowledgments PASS,
but a negative-control executable named no-installer-worker.exe hit Windows
installer filename detection (WinError740), requiring elevation before execution.
Renamed negative executables no-worker-entry/no-pause-ack, preserving failed
build. No elevation requested and no privileges/settings changed. Fresh full
focused gate rerun follows; positive alone is not final gate acceptance.

Installer handshake final PASS:672 read/write requests/672 real pause waits,
acknowledgments and resume signals;24 installer plus24 manager threads joined.
53 selected linked bodies,39 required omissions,one rebalance selection control,
16 runtime controls pass. Installer ThreadMain remains accepted NEAR_MATCH468B
vs469B original; worker entry exact24B, Lock exact25B. Existing constructor and
processing boundaries remain explicit; no new root credit.
Independent201 input/54 object hashes checked without drift. Focused gate registered;
full gate_all not run. Failed builds preserved.
Receipt: rebuild/build/native-worker-installer-handshake/00/98/0098ecd0/20261019T134423085007Z/receipt.json.

## Actual blocking wait composition in progress

Separate NativeThreadedWorkerBlockingWait_test.cpp / matching gate adds actual
WaitForObjectToFinish to the accepted real installer/worker/batch chain. The main
thread waits on each copied UID, repeats each completed/missing UID and waits an
unrelated UID:1368 calls. First installer acknowledgment each batch waits on a
fixture scheduling event signaled by the main thread entering real SleepEx(0,true),
which forwards to Windows. This guarantees all24 batches execute blocking waits.
The callback priority checks allow legitimate concurrent promotion to priority6;
callback mutations lock the actual manager critical section. Fixture-only main
SleepEx observer fails after5s to bound defective-control waits. No production
wait timeout or canonical root changes. Validation in progress; no PASS yet.

First blocking-wait gate draft confused retail181B with the existing near
candidate's187B selected body. Frozen provider42.obj independently confirms187B;
corrected selected-body assertion, retaining original181B comparison extent.
Failed build20261019T134551036568Z preserved. No production source change.

Second blocking gate reached native execution but fixture observer falsely treated
KeepUpdating's existing SleepEx(0,true) as a blocking-wait sleep before deadline/UID
initialization. Scratch diagnostics134850/134938 captured UID0/no wait progress;
volatile observation fields did not change the outcome. Actual KeepUpdating source
0098E190 confirms SetEvent followed by SleepEx. Removed that extra fixture wake;
actual AddObject/Wait provide their own wake signals. Scratch diagnostic135002 now
PASS24/672 with1368 wait calls and24 forced blocking batches. No production bug
or source change. Rerunning permanent gate with full controls/selected-code audit.

Blocking wait final PASS:1368 actual WaitForObjectToFinish calls/24 forced blocking
batches with672 real worker requests; existing672 installer acknowledgments/resumes,
336 immediate/336 exit-owned releases all pass.54 selected bodies,40 required
omissions,one selection control and17 runtime controls pass. Wait remains near187B
vs181B original; no new root credit. Independent202 input/55 object pins unchanged.
Registered in gate_all, no full run.
Receipt: rebuild/build/native-worker-blocking-wait/00/98/0098ecd0/20261019T135014741942Z/receipt.json.

Next: actual CThreadedFile AddObject/priority/wait wrappers backed by production
manager construction in the exact global symbol's fixture-owned storage. Preserve
passing graph; file-wrapper objects remain controlled descriptors in this step.

## Actual file wrappers / global manager storage in progress

Separate NativeThreadedWorkerFileWrappers_test.cpp / matching gate routes672
submissions,168 callback priority changes and1368 waits through accepted actual
CThreadedFile wrappers (exact19B/10B/10B). Production manager new/delete is paired
with a fixture allocator that returns the4096B storage bound to the original
manager global symbol; constructor/worker/destructor are real and trailing storage
canaries must remain intact. Request new/delete still forward to CRT allocation.
File object descriptors/OS handles remain fixture-owned; no file open/close/rooted
path or file lifetime claim. No canonical source changes or new root credit.
Scratch diagnostic135224 passes24/672; full selected-code/20-control gate running.

First wrapper full gate compiled/parity/model checks passed but data export used
COFF underscore decoration in /export alias; VC71 link adds decoration itself for
this C identifier and could not resolve the double-decorated name. Isolated frozen
object links independently prove FixtureGlobalManagerStorage succeeds and its
underscore-prefixed form fails. Corrected export spelling; added exported storage
RVA equality with the unique .livemgr section. Failed build135234628980Z preserved.

File wrappers final PASS:672 real CThreadedFile submissions,168 file priority
changes,1368 file waits/24 forced blocking batches and24 manager globals created/
destroyed through actual production methods.57 selected bodies,43 required omissions,
one selection control and20 runtime controls pass. Exported LiveManager RVA equals
the unique.livemgr storage; each owner also equals the file-wrapper global symbol.
205 input/58 object pins independently unchanged. Focused gate registered;
no full run, canonical source changes or new root credit.
Receipt: rebuild/build/native-worker-file-wrappers/00/98/0098ecd0/20261019T135437239917Z/receipt.json.

Next: actual CThreadedFile query/RemoveObject routes in the live worker chain,
including main-thread retirement of retained requests and pending_locked refusal.

## Caller retirement / actual file queries in progress

New NativeThreadedWorkerCallerRetirement_test.cpp / matching gate routes actual
file ObjectExists/IsObjectFinished and RemoveObject against the constructed global
manager.168 read-begin pending_locked removals must refuse without changing64B
request storage.168 retained completed requests are removed by the main thread,
with actual manager RemoveObject and UID iterator erase;192 repeated/unrelated
missing removals succeed. Remaining168 retained requests retire at ThreadMain exit;
336 still retire immediately. Release observers distinguish all three phases and
UID invariants are rechecked after each caller removal. Extra25 runtime controls
include caller missing-delete/UID-erase/return defects, unlocked-state removal and
file removal wrapper failure. Scratch diagnostic135713 passes24/672; full gate
running. Existing passing wrapper receipt preserved; no canonical changes/credit.

Caller retirement final PASS:672 requests with336 immediate,168 caller-thread and
168 exit-owned retirements;168 protected refusals/192 missing removals. Actual
file query/removal wrappers execute against constructed global manager.60 selected
bodies,45 required omissions,one selection control and25 runtime controls pass.
Standalone UID iterator erase source is redundant: an identical selected body is
also supplied elsewhere in the linked graph; this is recorded, not called a required
omission failure. Model360/2016/13 controls retained. Independent208 input/61 object
hashes intact. Gate registered in gate_all; no full run or new root credit.
Receipt: rebuild/build/native-worker-caller-retirement/00/98/0098ecd0/20261019T135722443143Z/receipt.json.

Next: actual manager pending/finished statistics against live submission,
completion and caller retirement, with independent expected count/byte models.

## Actual worker statistics in progress

New NativeThreadedWorkerStatistics_test.cpp / matching gate adds four existing
exact manager statistics getters to the accepted live file/caller-retirement
chain.912 independent checkpoints/3648 calls cover empty manager, each submitted
descriptor, full completion, each caller retirement and final missing removal.
Expected counts/bytes derive from fixture descriptor sizes and retained/nonremoved
slot policy, not production request fields/getter internals. No post-ThreadMain
statistics query touches freed exit-owned requests. Four zero-statistics runtime
controls join the existing25. Scratch diagnostic135959 passes24/672 and912 checks;
full permanent gate running. No canonical source changes/new root credit.

Statistics final PASS:912 independent checkpoints/3648 actual getter calls,
64 selected linked bodies,49 required omissions,one selection control and29 runtime
controls. Full live graph672 requests/24 managers/24 installer threads, all release
phases and real waits/acknowledgments preserved.213 input/65 object pins independently
unchanged. Focused gate registered, no full run or new root credit.
Receipt: rebuild/build/native-worker-statistics/00/98/0098ecd0/20261019T140007153312Z/receipt.json.

Next: actual Windows failed read/write submission policy. ReadFileEx failure is
ignored by the recovered worker while WriteFileEx failure invokes hard-disk error
callback and installer Unpause; both requests remain reading until shutdown.
Use invalid fixture-owned file handle values and forward real APIs, not forced
return substitutes; validate no completion/UID retirement and safe exit cleanup.
Keep fixture-only recovery of an unmatched read installer pause explicitly separate
from production policy. Preserve all passing success fixtures.

## Real failed OS submissions in progress

NativeThreadedWorkerSubmissionFailures_test.cpp / matching gate reuses the actual
constructed global manager, file submission/query wrappers, installer worker and
pending/UID trees.24 batches/672 allocated requests contain fixture invalid file
handle values. Real forwarding Windows API observers verify ERROR_INVALID_HANDLE:
12 read/12 write submissions fail; each leaves the selected request reading and
27 others pending, with no completion or UID retirement. Failed write calls the
hard-disk policy callback/Unpause; failed read leaves installer Paused1, as original
worker source specifies. After actual ThreadMain releases all28 requests, fixture
cleanup explicitly Unpauses only the read installer, then joins it. This is not
claimed as production recovery.720 independent pending/finished totals checks;
full output file must remain unchanged. Scratch diagnostic140419 passes.
Full64 linked bodies remain byte-audited but wait/priority/caller-retirement roots
are cold here; their execution claims stay in prior success receipts.19 focused
runtime defects cover actual failure policy, missing queue edit, missing callback/
unpause, wrong synthetic completion and exit request/overlap ownership. Gate running.

Submission failures final PASS24 failed OS submissions (12 reads/12 writes),672
exit-owned request/overlap releases and720 statistics checkpoints. All19 failure-
specific runtime controls pass;64 selected bodies/49 required omissions/one selection
control preserved.213 input/65 object pins independently unchanged. Gate registered,
no full run or new root credit. No actual OS completion occurs for these failures.
Receipt: rebuild/build/native-worker-submission-failures/00/98/0098ecd0/20261019T140553745894Z/receipt.json.

Next: actual APC-reported read errors from successful ReadFileEx submission at EOF,
including both device policies (hard-disk callback/DVD dirty flag), installer Unpause
and original OnFinishObject(true) behavior despite the error. Preserve submission
failure gate separately; no invented error-policy correction.

## Real EOF APC error composition in progress

NativeThreadedWorkerEOFErrors_test.cpp / matching gate submits672 actual reads at
or beyond synthetic EOF. Real Windows ReadFileEx succeeds and its APC reports
ERROR_HANDLE_EOF38/transferred0. Forwarding APC observer records this then calls
the unchanged production OnFileReadCompletion.336 HDD errors invoke actual policy
callback with empty text;336 DVD errors set the real manager dirty flag. Every
APC unpauses actual installer. Original OnFinishObject still calls virtual
completion(true) on336 callbacks despite the error; fixture asserts this original
behavior, not invented failure propagation. Buffers/output remain unchanged.
Normal ownership phases,336 callback priority moves/protected refusals,1368 waits,
912 statistics checkpoints retained. Dirty-disc state fixture-reset per batch.
Scratch diagnostic140827 passes; full64-body/35-control gate running. Worker write
branch is cold here; earlier success/submission-failure gates preserve its claims.

EOF error final PASS672 real EOF APCs/336 hard-disk callbacks/336 DVD dirty errors.
All336 virtual completion(true),672 real installer pause/resume cycles,1368 waits,
336 callback priority/protected-refusal cases and912 statistics checkpoints pass.
64 linked bodies/49 required omissions/one selection control/35 runtime controls.
213 input/65 object pins independently unchanged. Gate registered; no full run or
new root credit.
Receipt: rebuild/build/native-worker-eof-errors/00/98/0098ecd0/20261019T140906142322Z/receipt.json.

Next: real pending cancellation plus refusal in reading/read_completed callbacks,
completing live proof across all three protected loading states. Preserve EOF and
successful-IO fixtures separately.

## Pending cancellation / all protected states in progress

NativeThreadedWorkerCancellation_test.cpp / matching gate cancels96 pending
requests through actual file/manager RemoveObject before worker release;48 queue
cursors explicitly point to the removed node.120 independent pending-tree walks
verify ordering, parents, black heights/red children, extrema, counts and cursor
membership after full submission and each cancellation. The576 surviving requests
exercise real EOF APCs and original completion/retirement behavior.
Actual RemoveObject refuses288 pending_locked,576 reading and288 read_completed
probes (1152 total), preserving64B request storage. Probing flag makes premature
free controls fail before Windows sees a freed APC context. Ownership totals:
96 pending cancellations,288 immediate completions,144 caller finished retirements,
144 exit-owned releases.264 missing removals,984 statistics checks/3936 getters.
First scratch141247 retained old28-ack expectation despite four cancellations;
corrected to24 acknowledgments per batch. Scratch141305 passes24/672 and all new
invariants. Full64-body/40-control gate running; no canonical source/credit changes.

Cancellation final PASS:96 pending cancellations (48 current-cursor cases),576
real EOF reads,1152 protected refusals across all three states,264 missing removals,
120 full pending-tree validations and984 statistics checks/3936 getter calls.
Ownership is96 canceled/288 immediate/144 caller finished/144 exit-owned, all672
request/overlap releases once.64 linked bodies/49 required omissions/one selection
control/40 runtime controls pass.213 input/65 object hashes independently unchanged.
Focused gate registered; no full gate_all, dashboard or new root credit.
Receipt: rebuild/build/native-worker-cancellation/00/98/0098ecd0/20261019T141404825667Z/receipt.json.

## Next continuation: live UID wraparound

User's continuous native-only goal remains active; no stop was requested.
Latest permanent fixture/gate are NativeThreadedWorkerCancellation_test.cpp and
 gate_native_threaded_worker_cancellation.py. All validations started this turn
have finished; preserve immutable successful and failed receipts. No Lua,
installed-file/save changes, game launches, commits/pushes or canonical root edits.

Next meaningful composition is UID wraparound in this live manager graph:
- Start NextObjectUID at0xFFFFFFF0 and0xFFFFFFFE across two existing variants before
  submitting28 requests; check returned UID equals unsigned(start+n) and final
  counter equals unsigned(start+28). Existing constructor normally starts1.
- ValidateUID currently assumes submission order equals unsigned key order; update
  ONLY a new fixture's independent expected slot ordering to sort by unsigned UID.
  Preserve full red-black/header/count validation and skip canceled/retired slots.
- Those starts make UID0 live in every batch: variant0 UID0 at slot16 (immediate),
  variant1 UID0 at slot2 (retained/caller-retired). Neither is among canceled slots
  0/7/14/21. Verify24 zero submissions/12 immediate zero deletes/12 caller zero deletes.
- Exercise a deterministic queued priority change on UID0 under initial manager
  lock, updating its fixture expected priority for pending-tree validation. Both
  zero slots have no callback. Existing callback priority moves total288;24 extra
  zero moves should make312 real file-priority changes.
- Run copied UID0 through actual file query/wait/removal wrappers; missing UID
  remains0x7FFFFFFF. Add controls which reject zero UID return or ignore zero priority.
- Keep existing cancellation/EOF/error-policy/installer/ownership boundaries; no
  production workaround or new root count. Scratch compile/link against frozen
  accepted objects first, then full new permanent gate and final hash audit.

Deferred CloseAllStreams0098E7D0, GetSubString0099BDB0 and predecessor exact recovery
still require genuinely new original evidence before retrying. SetObjectPriority
remains NEAR_MATCH151B; predecessor remains functional-only. Installer idle processing
and raw file descriptors remain controlled. Current success/failure graphs do not
establish complete game startup or a playable recreated world.

## Cleanup coordination / live UID wraparound in progress

At the explicit coordination request from cleanup session01a1545c-ccad-77f3-ab79-
4be4aae0dcce, appended CURRENT protections to
D:/Documents/FableTLC-maintenance/coordination/native-recreation.txt. Protect all
native source/header/tool/index/receipt/evidence/build/scratch paths and all dirty
branches/refs, including current feat/novi-script-recovery. No exact artifacts
released. No demonstrated inventory contention; STOP-INVENTORY not created.
Coordination message supplies no new run/implementation authority; existing user
continuous native-recreation authorization remains the basis for this work.

New NativeThreadedWorkerUIDWrap_test.cpp / matching gate starts actual manager
NextObjectUID at0xFFFFFFF0/0xFFFFFFFE before28 submissions. Returned IDs and next
counter compared as unsigned32-bit values. Independent expected UID tree keys are
sorted unsigned across wrap; full tree/header/count checks preserved. Each batch
contains UID0: slot16 immediate in variant0, slot2 caller-retired in variant1.
24 zero priority moves execute through actual file wrapper while pending; total
file priorities312 (288 callback moves plus24 zero moves). Zero queries, waits,
protected removals and retirement remain real production calls.
Scratch diagnostic141740 passes24/672 with24 zero submissions/moves and12 immediate/
12 caller zero deletes. Full64-body/45-control gate running, including zero-only
wrong return, ignored priority, false existence, false completion and removal defects.
Prior cancellation fixture and immutable receipts unchanged; no canonical root edits
or new root credit.

UID wrap final PASS24 managers/672 requests, actual unsigned counter wrap at both
starts,24 UID0 submissions/priority changes and12 immediate/12 caller UID0 deletes.
Previous576 EOF reads,1152 protected refusals,96 pending cancels,1368 waits,
984 statistics checks and all672 releases retained.64 selected bodies,49 required
omissions,one selection control and45 runtime controls pass.213 input/65 object
hashes independently unchanged. Gate registered; no full gate_all/dashboard,
canonical-root edits or new root credit. No task-owned validation still running.
Receipt: rebuild/build/native-worker-uid-wrap/00/98/0098ecd0/20261019T141812929225Z/receipt.json.

## Next continuation: callback-driven follow-up reads

Continuous native-only goal remains active; no user stop. Latest base is
NativeThreadedWorkerUIDWrap_test.cpp / gate_native_threaded_worker_uid_wrap.py.
Preserve accepted wrappers/cancellation/error/UID fixtures and all immutable receipts.

A useful next engine composition is actual AddObject re-entry from the unlocked
virtual completion callback, as a loader can submit its next resource read:
- In a NEW fixture, let each of288 parent completion callbacks submit exactly one
  EOF child through actual CThreadedFile::AddObject. Child has no callback and
  QueueWhenFinished=false, inherits device, owns an independent guarded buffer.
  Expected24 batches now40 logical slots each:28 original plus12 children.
- Expand slot/expected/tree-walk arrays to56 or another sufficient bound. Register
  each child UID/request/overlap while the callback owns the manager lock, before
  returning; actual AddObject is the only production creator. Assert worker callback
  did not own the manager lock before entry and request/queue/UID insertion is real.
- Current release observers search the FIRST matching raw pointer. Child allocations
  can reuse canceled/retired request or overlap addresses. Search active ownership
  generations (unfreed overlap / undeleted request) instead; preserve double-free,
  phase and probing checks. Record actual address reuse if observed, do not assume it.
- Current ValidateUID(false) includes every noncancelled slot even if an earlier
  immediate request retired. For callback-time UID checks model currently live slots,
  excluding observed immediate/caller retirements, but still include exit-owned UID
  nodes after ThreadMain (original keeps dangling values until destructor). Never
  dereference freed requests. Sort expected unsigned UID keys as in wrap fixture.
- Parent waits establish all callback admissions have returned; then wait on copied
  child UIDs before final completion/retirement assertions. Children have no callback,
  so g_count is stable after all parent waits. Keep original1368 waits plus288 child
  waits; expected1656 total. Initial cancellation/tree checks remain120.
- At parent completion callback, independent stats model can use fixture event state:
  current callback request is still pending/read_completed; other slots whose APC
  already ran have settled through OnFinish before this single worker selected the
  current request. Retained settled requests count finished; settled immediate and
  canceled requests are absent. No callback-internal alertable waits. Check model
  before/after child admission (one additional pending request/known descriptor bytes).
- Expected totals to verify:960 allocations/releases,96 pending cancels,864 EOF reads
  (432 HDD/432 DVD),576 immediate releases (288 parents/288 children),144 caller and
  144 exit-owned releases. Callback count remains288; protected refusals become1440
  (288 locked/864 reading/288 read_completed). Zero parent retirement counts stay12/12.
  Treat these as expectations, not accepted results. Add controls for skipped/wrong
  child admission and retain earlier relevant controls.
- Scratch compile/link against frozen accepted objects first; then independent new
  permanent gate, selected-body/link/control proof, final input/object hash audit,
  focused gate registration and journal/status update. No new root count from reuse.

Coordination protections remain in maintenance/coordination/native-recreation.txt;
no artifacts released and no inventory contention observed. No game/save/lease
changes, commits, pushes, Lua or full gate_all. Deferred CloseAllStreams/GetSubString/
predecessor exact work still needs genuinely new original evidence before retrying.

## Callback-driven actual data reads in progress

Previous goal turn made verified progress: UID-wrap graph accepted with frozen
receipt/pins. Current continuation adds NativeThreadedWorkerCallbackSubmission_test.cpp
and gate_native_threaded_worker_callback_submission.py, preserving that fixture.
Each288 parent completion callbacks submits an independently described child through
actual CThreadedFile/AddObject with the manager lock unowned at API entry and return.
Fixture locks only before/after admission for statistics and node observations; the
production API owns admission. Child descriptor/node/UID/overlap and unchanged64B
parent verified. No callback-local alertable wait or recursive child callback.

Improvement over previous recipe: children read actual valid synthetic bytes rather
than more EOF.288 child reads/6216 bytes must match the independent payload formula;
parent576 EOF APCs still exercise original device error/completion policy.960 total
requests,864 actual reads,1656 waits,1440 protected refusals and1560 statistics checks
(expected6240 getter calls). Observers select active ownership generations so child
allocations may legitimately reuse previously retired request/overlap addresses.
Logical UID model excludes settled immediate requests but preserves exit-owned
nodes until manager destruction. Callback-time totals use single-worker event state,
not production getter internals or freed request bytes.

Scratch142442 passed with fixture-held lock around admission. Tightened API entry/
return to unowned callback lock, then scratch142546 independently passed:24 managers,
960 requests,288 child admissions/6216 checked bytes,281 request/43 overlap address
reuses in that run. Full64-body/53-control gate running. Eight child-specific controls
exercise fake/wrong UID admission, absent pending node, wrong retention/buffer,
short/shifted actual read and failure to release the completion callback lock.
No canonical roots changed; no new root credit. Original other boundaries remain.

First callback-admission full build142804116652Z positive960/864/6216 PASS,
but skip-child-pending negative control reached malformed native queue/wait state
and subprocess timed out after180s. Confirmed process terminal/missing before
retry; no duplicate job restart. Strengthened fixture with immediate child
pending-node/key/count membership check after actual AddObject, before subsequent
wait/worker use. Scratch admission-check143428 recompiled only fixture against frozen
objects: positive PASS960/864/6216 with284 request/50 overlap reuses; skipped pending
insertion now fails cleanly with code80 at admission. Preserved failed full build.
Rerunning permanent full gate; no production queue/timeout semantics changed.

Potential next actual source recovery found while validation ran:
CBankFileAsyncData_StartRead_009d5650.cpp is a legacy local-struct/Reader facade.
Original retail disassembly009D5650..009D5669 is26B/10 instructions: load BankFile+28,
ThreadedFile data pointer+110, push address LoadDesc+2C, call real AddObject0098F350,
store return atThreadedFileUID+18. Both donor names independently confirm PRIVATE
thiscall void StartRead(): Ego_r00B296C0 and FableWin02FF9B30 use decorated
?StartRead@CBankFileAsyncData@@AAEXXZ. No current function_overrides row or dedicated
gate found. Shared fable_game_text_bank.h has no StartRead declaration yet. A typed
private method can likely replace the facade using actual CThreadedFile wrapper;
first inspect current source/index ownership and fresh compile original-compatible
candidate with isolated header overlay before any canonical source/header change.
This is not accepted yet and no root/count claim is made.

Callback submission final PASS24 managers/960 allocations and releases,864 real
reads (576 parent EOF/288 child success),6216 independent child bytes,1656 waits,
1440 protected refusals,1560 independent statistics checkpoints/6240 getter calls.
64 selected bodies/49 required omissions/one selection control/53 runtime controls
pass. Actual request/overlap address reuse recorded in receipt; active generations
avoid attributing a new allocation's destruction to an earlier retired request.
213 input/65 object pins independently unchanged. Focused gate registered in
 gate_all; full gate_all/dashboard not run. No canonical-root edits/new root credit.
Receipt: rebuild/build/native-worker-callback-submission/00/98/0098ecd0/20261019T143457527364Z/receipt.json.

## Next continuation: typed StartRead repair

Continuous native-only goal remains active. No validation remains running at this
checkpoint. Next source recovery can replace the legacy StartRead facade and then
connect a genuine bank-data producer to the actual worker chain.

Scratch candidate:
work/native-recreation/loading/start-read/20261019/typed-candidate-143552/
 CBankFileAsyncData_StartRead_009d5650.cpp and parity.json.
Candidate uses shared fable_threaded_file_object.h and private class method:
ThreadedFileUID=BankFile.ThreadedFile->AddObject(LoadDesc).
Private StartRead declaration added ONLY to37-header isolated include snapshot.
First compile MATCH26B/one named REL32 actual CThreadedFile AddObject0098F350.
Both donor PDB names confirm private void thiscall; retail10 instructions confirm
all field offsets. Project header hashes unchanged. This is MATCH-ONLY evidence:
no behavioral gate, canonical promotion, compiler/EXE pin completeness or new root
acceptance claimed. Fresh durable validation must supply those before promotion.

Ownership found:
- rebuild/build/symbol_index.tsv currently indexes public fake
  ?StartRead@CBankFileAsyncData@@QAEXXZ ->009d5650 strong in
  00/9d/CBankFileAsyncData_StartRead_009d5650.cpp.
- Actual private identity is ?StartRead@CBankFileAsyncData@@AAEXXZ.
- ARTIFACT_INDEX row009d5650 points to the existing source, legacy test
  rebuild/tests/00/9d/CBankFileAsyncData_StartRead_009d5650_test.cpp and
  rebuild/build/candidates-Release/00/9d/009d5650.
- Other public overload ?StartRead@CBankFileAsyncData@@QAE_NH@Z at0065914b is a
  separate legacy row; do not overwrite or silently credit it.
- Current fable_game_text_bank.h has no StartRead declaration. Private section
  before caller-buffer constructor is the candidate insertion point; no layout or
  virtual surface change. Existing facade/test/catalog must be inspected and
  archived before replacement. No function_overrides row exists for009d5650 yet.
- symbol_index.py outputs rebuild/build/symbol_index.tsv. Any accepted replacement
  must migrate only the stale public symbol to the independently verified private
  identity and preserve every unrelated row; no full reindex or dashboard by default.

Needed proof: fresh original/rebuilt behavior with actual19B file AddObject wrapper
and independently controlled manager return/argument/side-effect observations;
private method ABI caller, full bank/data/file storage guards, descriptor const
preservation, UID extremes including0 and strict root/provider controls. Stop tuning
once passing near or exact. Before shared-header promotion, compile relevant existing
header consumers and preserve their symbols/bytes/relocations, including the current
actual worker graph. Then exercise typed bank-data StartRead in that graph using a
fixture-owned bank/data context (no unsupported constructor/lifetime claim), and
register dedicated gate/corrections/source ownership without artificial root credit.

Protect current/failed callback admission builds and all scratch StartRead evidence.
No Lua, installed/save/lease changes, game launches, commits/pushes, full gate_all
or dashboard. Cleanup coordination protections unchanged; no artifacts released.


### Typed StartRead behavior and live worker scratch validation (2026-10-19 UTC)

- Cleanup acknowledged native tree exclusions including parent traversal. Appended
  coordination/native-recreation.txt: no metadata idle window, no artifacts released.
  No observed contention; no STOP-INVENTORY requested.
- Legacy test defines its own fake StartRead and does not exercise production TU.
  Canonical source/header/test/catalog/index remain unchanged pending promotion.
- Scratch validator: work/native-recreation/loading/start-read/20261019/validate_typed.py.
  Latest authoritative behavior receipt: behavior-144445/receipt.json beneath that
  directory. Exact original26B root and19B actual file AddObject; private AAEXXZ
  identity.96 original/rebuilt cases across four guarded layouts, four seeds and
  six UID returns including0,7FFFFFFF,80000000,FFFFFFFE,FFFFFFFF. Entire data/bank/
  file arena and manager guards, const descriptor, pre-store observation, child
  side effect, callee-saved registers and stack cleanup checked. Six controls fail:
  root/file omissions, wrong bank/descriptor/UID offsets, omitted UID store.
  325 original/source/toolchain/header/PDB pins and two object hashes intact.
  Manager AddObject remains independently controlled in this differential check.
- Header validator validate_header.py: header-144335/receipt.json PASS57 consumers,
  including all53 live worker TUs and four relevant bank producers/callbacks.
  Nonlocal symbols, bodies and relocations unchanged. VC71 compiler-local $L labels
  normalize by section+offset; same compiler output filename avoids debug OBJNAME
  noise. Earlier attempts retained: local-name comparison, debug filename, missing
  extra bank header closure. Fresh final snapshots include extra header closure.
- Live scratch validate_worker.py: worker-144432/receipt.json PASS24 managers,
  960 requests/864 reads/288 typed bank StartRead submissions/6216 checked child
  bytes,960 releases once,24 installer/24 manager joins. Actual private thiscall ABI
  via fastcall linker alias, actual file wrapper/manager/worker; bank/data contexts
  are fixture-owned guarded typed storage, no constructor/lifetime claim. Guard
  check allows only UID+18 write; descriptor copied to real LoadDesc+2c and real
  bank ThreadedFile+110. Observed281 request/55 overlap address reuses, not stable.
  Frozen accepted provider objects reused; no full new gate acceptance claimed.
- Initial differential attempt had a wrong explicit manager target98F370; exact
  byte comparison rejected it before execution. Retail call and existing gate map
  independently establish98F0F0; corrected run passes. Preserve failed evidence.

NEXT: make the typed StartRead native fixture/gate durable with strict omission,
linked body/relocation verification and focused runtime mutations (inherit accepted
worker53 controls as appropriate). Archive legacy source/test/catalog and migrate
ONLY obsolete public QAEXXZ row to private AAEXXZ after full proof. Replace legacy
self-testing test with a real TU/API fixture. Check candidate catalog and source
ownership/function_overrides without touching the separate0065914B overload.
No artificial coverage/root credit. Then promote shared declaration/source and run
relevant gates. All scratch passes are explicitly NOT_PROMOTED. No Lua, game/save/
lease, Git mutation, deletion, full gate_all or dashboard. Native goal stays active.


### Typed StartRead promoted; permanent ABI and worker gates PASS (2026-10-19 UTC)

Previous turn was progress:96-case differential,57-consumer header check and live
scratch typed producer materially supplied required promotion evidence. This turn:

- Archived seven exact legacy inputs and SHA256 manifest at
  work/native-recreation/loading/start-read/20261019/legacy-before-promotion:
  source,self-defining test,shared header,catalog,symbol index,artifact index,
  function_overrides. No deletion or root-credit increase.
- Canonical CBankFileAsyncData_StartRead_009d5650.cpp now uses real shared classes:
  ThreadedFileUID = BankFile.ThreadedFile->AddObject(LoadDesc). Private nonvirtual
  declaration added to fable_game_text_bank.h. Exact26B and one typed REL32.
- Legacy test replaced with real production-linked24-case typed fixture, no root
  redefinition. Guarded data/bank/file storage, descriptor identity, one call and
  UID values0,1,7FFFFFFF,80000000,FFFFFFFE,FFFFFFFF checked across4 seeds.
  Fresh standalone compile /O2 root + /Od test passes at
  work/native-recreation/loading/start-read/20261019/typed-unit-144644/.
- Canonical permanent ABI gate gate_native_bank_data_start_read_abi.py PASS96cases,
  six controls (omit root/file,wrong UID/bank/desc offsets,skip store), exact26B root
  and actual19B file AddObject; full arena/manager side-effect/register/stack guards.
  Explicit both-donor private decorated-name checks. Controlled manager boundary.
  Latest receipt: D:\Documents\FableTLC\rebuild\build\native-bank-data-start-read-abi\00\9d\009d5650\20261019T144814574979Z/receipt.json.
  All325 input hashes and two object hashes verified unchanged. Both source bodies
  are fresh-compiled from canonical files into immutable snapshots.
- Permanent real Windows gate gate_native_bank_data_start_read.py PASS24 managers,
  960 requests/864 real reads/288 typed StartRead children/6216 checked bytes,
  all960 request+overlap releases once,24 installer/24 manager threads joined.
  65 linked bodies,50 required omission failures,1 rebalance selection control,
  all58 runtime controls (existing53 +5 root-specific no-call/no-store/wrong-UID/
  descriptor mutation/bank mutation). Attached360sequence/2016removal model retained.
  Latest receipt: D:\Documents\FableTLC\rebuild\build\native-bank-data-start-read\00\9d\009d5650\20261019T144816271746Z/receipt.json.
  Independently verified214 input/66 object hashes unchanged after final run.
  Runtime address reuse recorded in receipt; never a stable expected count.
- Precisely one row each changed in symbol_index.tsv (public QAEXXZ to private
  AAEXXZ at009D5650 only), ARTIFACT_INDEX.tsv (archive/current gate path), catalog
  (stronger STARTREAD_OK typed_cases=24 marker). Separate0065914B overload unchanged.
  Added one function_overrides row explaining identity and limits. Both new gates
  registered in gate_all; AST syntax checked. No full gate_all/dashboard/reindex.
- Existing constructor source contains caller-buffer009D5D30 and pool009D5DF0
  overloads. Preliminary fresh caller-buffer audit found exact179B after7 named
  zero-addend relocations (base,3vtables,lock,insert_unique,unlock), original RET12.
  Evidence work/native-recreation/loading/bank-buffer-construction/20261019/144948/
  parity.json. This is AUDIT_ONLY/behavior pending, not new acceptance/root credit.
  Known source CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp. Pool constructor
  permanent gate is gate_native_bank_async_data_constructor.py (183B/1200cases).

NEXT: validate real caller-buffer constructor with registration (actual pointer-tree
insert_unique/rebalance) and bridge to StartRead/worker callbacks rather than manual
bank-data storage. Preserve complete original/rebuilt guards, ABI and strict controls.
Actual table/helper providers matter: ctor emits movable/base callback tables and
real callback methods; do not silently accept executed linkage-only traps. Constructor
registers this in BankFile.AsyncDataInstances under bank critical section; delayed
InternalAllocatedBufferSize initialization observable. Plan lifecycle/unregistration
proof and qualify fixture-owned bank context before any lifetime claim. Existing
native loading restart tree graph has real pointer-tree insertion/rebalancing;
canonical async pointer key/range/equal erase gates also exist. No same-evidence
retry of CloseAllStreams/GetSubString/predecessor exact. Lua/game/save/lease/Git and
all other owner protections remain intact. Goal remains active, none released.


### Caller-buffer constructor model/actual registration accepted; missing SetupRead found

Previous turn was progress: promoted typed StartRead, verified permanent gates and
corrected source identity. Current turn completes prerequisite constructor proof:

- Added gate_native_bank_async_buffer_constructor.py and independent fixture
  NativeBankAsyncBufferConstructor_test.py plus linkage-only required.cpp. Existing
  root009D5D30 (no new credit) exact179B/52instructions/7audited relocations, real9B
  base.600cases/3000full32KiB snapshots EACH original/compiled;3DWORD args/RET12,
  callee-saves/original-this return, buffer+length extremes, live bank reload after
  lock mutation, cached lock identity, descriptor padding, delayed size store,
  registration/unlock changes and copied buffer fields. Six fresh source defects
  detected: null buffer,wrong length,nonzero skip,wrong UID,omit registration,early
  InternalAllocatedBufferSize store. Strict typed root omission; required executable
  remains linkage-only, aliases never executed. Latest build D:\Documents\FableTLC\work\native-recreation\loading\bank-buffer-construction\20261019\gate-145202.
  Fixed initial copied instruction-count assertion before model; saved failed build.
- Actual constructor registration/submission graph accepted separately at
  D:\Documents\FableTLC\rebuild\build\native-bank-buffer-registration\00\9d\009d5d30\20261019T145444883860Z/receipt.json.
  Gate gate_native_bank_buffer_registration.py compiles12 genuine providers and
  compares original/rebuilt through8scenarios/56constructions/56submissions with112
  full64KiB snapshots, four insertion orders/two fills. Independent object expected
  fields, whole-arena submission guards, actual redblack ordering/parents/colors/
  blackheights/root/extrema/nodecounts, correct allocator20B blocks and lock depth,
  ABI registers/stack and all12required omission detections. UID0/signed boundaries
  flow through constructed object and actual StartRead/file AddObject.141input/9object
  pins independently unchanged. Predecessor remains behavior-only. Win32 locks,CRT
  allocation and manager admission controlled; no WindowsIO/lifetime/destruction.
  First scratch validator incorrectly read4B tree color instead of bool1B; corrected
  independent model, preserving failed scratch. No production code change for it.
- Both gates registered in gate_all, syntax parsed; full gate_all not run. Existing
  pool constructor gate rerun PASS1200cases/6000snapshots. Updated only009D5D30
  correction evidence (old text called solecaller SetupReadRequest; real source is
  BeginReadData009D6060, whose constructor call is009D6084). No root/source credit.
- Constructor object provider-audit saved in latest constructor build. Virtualtable
  emitted methods need actual or explicitly cold/trapping providers for Windows
  linking; do not mistake weak E/G destructors for missing new native roots.

NEW upstream gap: CBankFileAsyncEntry::SetupRead009D5970 declared in real shared
header and called by actual BeginReadData/AutoAlloc, but no indexed canonical source
or ARTIFACT_INDEX row found. Both donor PDB names independently private:
?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z
Ego_r00B299F0, FableWin02FF8A70. Original191B/66instructions; evidence disassembly
and donor extract at work/native-recreation/loading/bank-setup-read/20261019/.
First ordinary candidate candidate-145642:189B/65instructions,complete_decode,
opcode similarity0.9312977, near_match=true. Seven typed calls. STOP TUNING at this
passing near candidate; behavior/promotion still pending, not new credit yet.

Native SetupRead behavior inferred from original (must prove independently):
DisableAsync bank+149: SetupBuffer(true) return ignored; ReadData of
GetEntryDataOffset(Index)+offset, current Buffer, requested size, then Finished=true.
Async: LoadDesc.PDst=Buffer; FilePos=GetEntryDataOffset(Index)+offset;
Size=NonCached?Align(size):size; callback=&data; priority supplied, QueueWhenFinished
false; actual data.BankFile.ThreadedFile AddObject stores UID. Fourth and fifth args
(bufferSize/skip in known caller header) ignored. Entry BankFile and data.BankFile
may differ; must test separately and reload aliases/callback mutations from actual
instructions. Source calls protected CBankFile::ReadData, requiring inferred friend
CBankFileAsyncEntry in CBankFile: candidate adds ONLY scratch declaration and clearly
labels inferred access, not a claimed PDB friendship breadth. Real bank-data friend
already permits SetupBuffer. Candidate includes real threaded_file_object header
first (specialization ordering), then bank_async_entry header.

NEXT: fresh original/rebuilt SetupRead behavior with independently controlled
SetupBuffer/GetEntryDataOffset/Align/ReadData/AddObject boundaries, both branches,
ignored args, UID0, branch/member mutations, descriptor/bank/data/entry guards and
strict omissions/meaningful source controls. First candidate already passes near;
no byte tuning. Then composition with actual constructor+registration+actualfile/
manager/worker and upper BeginReadData with counted-pointer lifetime proof as scope
allows. This resolves missing real caller above ctor and avoids silently bypassing
SetupRead in a claimed upper-level chain. Do shared-header consumer compatibility
checks before promoting inferred CBankFile friendship; archive ownership metadata.
All native paths protected, no idle metadata scan window or released artifacts;
Lua,game/save/lease,Git,full gate_all/dashboard untouched. Goal active.


### Missing SetupRead provider recovered, behavior-proved and promoted

Previous goal turn was progress: caller-buffer independent model/actual registration
and missing upstream SetupRead evidence made promotion possible. Current turn:

- Preserved first near candidate189B/65instructions against retail191B/66instructions,
  opcode similarity0.9312977, complete decode. No tuning after passing first form.
  Both donor PDBs independently identify private six-argument AAEX thiscall. New
  canonical source: rebuild/src/compiled/00/9d/CBankFileAsyncEntry_SetupRead_009d5970.cpp.
- Added inferred friend CBankFileAsyncEntry to CBankFile for protected ReadData call;
  explicitly labeled inferred access, not PDB friendship breadth. Scratch header
  comparison header-145934/receipt.json under loading/bank-setup-read/20261019
  preserves58 existing consumers' public symbols/bodies/relocations. Compiler-local
  labels resolve by section+offset, same output filename avoids OBJNAME debug noise.
- Original/compiled independent model gate gate_native_bank_entry_setup_read.py
  PASS2304cases/8640full32KiB snapshots EACH. Sync/async, noncached/cached, unsigned
  sizes/offset wrap, UID0, ignored args, distinct Entry.BankFile and Data.BankFile,
  priority, false SetupBuffer result ignored; explicit child mutations of bank,
  index,buffer,descriptor,UID and Finished at all five boundaries. Child-entry state,
  complete final storage, event argument order, RET24 and callee saves checked.
  All14 source controls detected (wrong buffer,omit setup,respect setup failure,
  omit read,force async/sync,always/never align,wrong bank,missing callback,retain
  request,wrong priority/UID/missing Finished). Positive typed ABI caller24cases,
  strict private-symbol root omission and linkable wrong-UID runtime rejection.
  Latest receipt: D:\Documents\FableTLC\rebuild\build\native-bank-entry-setup-read\00\9d\009d5970\20261019T150339742163Z/receipt.json.
  Independently verified327 input/16 object hashes unchanged. Snapshot source/test/
  project headers + VC71 tools/headers/libs + both donor PDBs and original EXE.
- Native linkage omission initially diagnosed alias shim @FixtureSetupRead@32 rather
  than private target in LNK2019. Saved failed receipt directory; /include:private
  root now explicitly requires independently proven symbol. Positive/mutant caller
  execute production root through linker alias. No production behavior changes.
- Archived five prior ownership/header inputs and SHA256 manifest under
  work/native-recreation/loading/bank-setup-read/20261019/before-promotion.
  Added exactly one symbol-index row, artifact-index row and override row; verified
  every prior row unchanged. Added typed source/test catalog entry and two gates
  in gate_all. First catalog insertion assertion found two catalog arrays; inserted
  only into initial main array, preserving second filtering array. No full reindex,
  hand-edited coverage, dashboard, full gate_all or artificial root credit.
- Additional actual composition gate gate_native_bank_entry_setup_composition.py
  PASS8scenarios/56constructed objects/56submissions/112full64KiB snapshots, all14
  genuinely exercised provider omissions. Actual ctor/base/lock RAII/unique pointer
  insertion/rebalance, new SetupRead, genuine GetEntryDataOffset36B and Align, and
  actual file AddObject wrapper. Independent expected descriptor uses real Buffer,
  runtime entry offset+wrapped requested offset, normal/noncached aligned size,
  callback=this, priority, Queue=false; UID return extremes and object/arena guards.
  Latest receipt: D:\Documents\FableTLC\rebuild\build\native-bank-entry-setup-composition\00\9d\009d5970\20261019T150550537443Z/receipt.json.
  144 input/11 object pins independently unchanged. RuntimeData offset path only;
  packed lookup/synchronous SetupBuffer/ReadData cold in this composition. CRT
  allocation, Win32 locks and manager admission remain controlled. Predecessor
  behavior-only, no new credit for shared roots; no WindowsIO/destruction/lifetime.

NEXT UPSTREAM BRIDGE AUDIT: actual BeginReadData009D6060 source already recovered at
CBankFileAsyncEntry_BeginReadData_009d6060.cpp. Fresh exact160B confirmed at
work/native-recreation/loading/bank-begin-read/20261019/audit-150604/parity.json.
Six relocation identities: operator new BFEA1A twice (72B object then12B CCPPointerInfo),
caller-buffer ctor9D5D30, DeleteData callback DIR32 9A42B0, SetupRead9D5970, operator
 delete BFE9BC. Return CCountedPointer uses hidden8B output plus5explicit args,
RET24. Normal path info words RefCount1, DeleteData ptr, data ptr; copying returned
counted pointer increments/decrements to1. DeleteData/deallocate branch should stay
cold on normal ownership case, but must be qualified and tested if scope expands.

NEXT: extend actual14-provider composition with160B BeginReadData so real upper API
allocates72B object, invokes ctor/registration, allocates12B control block, calls new
SetupRead and returns observed8B counted pointer with correct refcount. Independent
allocation arena/return/descriptor/tree/ownership guards and original/compiled model;
no allocation-failure/lifetime claim without proof. Then join actual Windows worker,
real bank callbacks and teardown/unregistration rather than manually initialized
storage. Actual bank OnFileReadBegin/SetupBuffer with caller Buffer nonnull keep
pool/allocator branches cold; completion sets Finished and UID0. Lifetime TU
CBankFileAsyncData_Lifetime_009d5410.cpp contains destructor9D61F0, Abort, FreeBuffer,
HandleFailedAllocation. Destructor unregisters both trees under bank lock, removes
outstanding UID only if nonzero, frees pool only if set and allocator only if internal
size>0. Preserve original zero UID behavior, do not invent a fix or lifetime claim.
All source/header/evidence/build trees protected; no idle metadata scan window or
artifacts released. No Lua/game/save/lease/Git changes. Goal active.


### BeginReadData counted-return chain accepted; actual worker bank lifetime scratch PASS

Previous turn was progress: recovered missing SetupRead and proved actual14-provider
ctor/registration/setup/metadata/file chain. Current accepted advance:

- gate_native_bank_entry_begin_composition.py adds existing genuine160B public
  BeginReadData009D6060 to chain, original/rebuilt8scenarios/56begins/56constructors/
  56submissions/112full64KiB snapshots. Actual72B data allocation and12B info block,
  hidden8B counted-return output,5explicitargs/RET24/EAX=output, final Data/Info/
  DeleteData pointer and independently observed refcount writes1/2/1. Constructor
  state checked at actual SetupRead entry; complete data/padding, all prior owners,
  tree node topology/color/blackheight/extrema/counts, allocation sizes/order and
  every immutable arena byte checked. UID extremes including0 flow through actual
  upper API. All15 executed-provider omissions fail; all7 source controls fail:
  wrong buffer/length/offset/priority,omit setup,wrong ref increment,missing info.
  Latest accepted receipt D:\Documents\FableTLC\rebuild\build\native-bank-entry-begin-composition\00\9d\009d6060\20261019T150924204714Z/receipt.json.
  145input/19object hashes independently intact. Registered gate_all (not full run).
  Updated only existing009D6060 correction evidence; prior row archived at
  work/native-recreation/loading/bank-begin-read/20261019/begin-override-before.tsv.
  No new source/root credit. CRT/Win32lock/manager admission controlled; normal async
  runtime-entry path only, owners retained at end. No failures/destruction/IO claim
  for this accepted emulated chain.

REAL WINDOWS SCRATCH (not permanent gate acceptance yet):
- Driver work/native-recreation/loading/bank-begin-read/20261019/validate_worker.py.
  Latest successful build worker-151900/receipt.json beneath same directory.
  Positive24managers/960requests/864real reads;288 public BeginReadData child reads
  transfer6216 independently checked bytes. Existing parent576 EOF APC error policy,
  pending cancels/zeroUID manager cases/statistics/ownership all preserved.24 manager
  and24 installer threads joined; all960 requests+overlaps freed once.
- Each of288 child calls now does actual BeginReadData -> caller-buffer data ctor ->
  real pointer-tree registration -> SetupRead/GetEntryDataOffset -> actual file/
  manager submission -> real bank OnFileReadBegin and SetupBuffer(nonnull buffer) ->
  Windows ReadFileEx/APC -> actual bank OnFileReadCompletion -> manager retirement.
  Fixture-held counted owner stays alive. After worker joins, genuine info DeleteData
  invokes compiler-selected virtual deleting wrapper and actual native data destructor;
  real key/equal/range/clear tree erase unregisters and releases node. Actual template
  counted-owner destructor drops info block; pointers clear. Observed exactly288 each
  bank completions/data deletes/info deletes/registration-node frees; real bank critical
  sections initialized/deleted, both bank trees end empty with sentinel invariants.
- Added guarded physical allocation interfaces for72B data/12B info/20B tree node,
  exact once release identity and16B guards. Only new pointer-tree Minsert malloc and
  range/clear free calls use per-object /Dmalloc=FixtureBankTreeMalloc and
  /Dfree=FixtureBankTreeFree bridges backed by actual CRT allocation. This changes
  named relocation boundary identity explicitly; function source/body algorithm stays
  canonical. Permanent linked-body/fixup proof must acknowledge these bindings, not
  claim literal retail CRT identity or silently reuse unbridged object hashes.
- Requires actual bank callback publication: queue descriptor PDst cleared by fixture
  AFTER independent native admission checks, BEFORE worker can select child. Native
  OnFileReadBegin must republish bank Buffer. Data72B expected snapshot allows only
  final UID0 and Finished=true; caller buffer guarded/independently checked bytes.
- Added filtered Enter/Leave/TryEnterCriticalSection IAT observers for bank contexts.
  Each bank must show3 enters (ctor/completion/dtor),5leaves (ctor+2begin+completion+
  dtor),2 successful recursive tries (OnFileReadBegin then SetupBuffer). Check counts
  before real ReadFileEx, at request retirement and data/info release; max counters
  fail promptly on retries. Bank CS recursion/owner zero checked before teardown.
  Thus no-op begin/setup/lock bodies cannot quietly pass this fixture's normal path.
- Full bank/entry/file constructors remain fixture-owned raw contexts, not native bank
  lifecycle. Pool/allocator branches cold because Buffer nonnull/MemoryPool null/
  MemoryHandle-1/InternalAllocatedBufferSize0; explicit reviewed cold function aliases
  end probe immediately if executed. Actual ReadData40B source linked but cold (async
  only); packed lookup/GetEntryDataSize and default movable CanRelocate cold traps.
  No full startup/game or allocation-failure claim. Scratch131 input pins intact;
  full toolchain/original image/model pins and linked bodies still required permanently.
- Preserved failures: initial teardown insertion matched CaptureWorker rather than
  post-join marker (compile rejected missing earlier declaration), then explicit cold
  unresolved linkage, then access violation from accidentally prepending fields before
  mock Slot.vtable. Fixture corrected to preserve vtable as first field. Next descriptor
  comparison failed because native ctor preserves3padding bytes initializedCD vs old
  fixture memset0; independently established ctor behavior justified fixture expectedCD.
  No production source changes for these corrections. All positive/failed scratch
  builds protected; latest allocation address reuse283requests/52overlaps is observed,
  not stable expected count.

NEXT: make worker-151900 NativeBankBeginReadWorker_test.cpp and driver into a proper
permanent native gate with fresh complete source/header/toolchain snapshots and pins,
selected linked bodies plus named relocation targets including explicit malloc/free
bridges, strict source omissions and focused runtime mutations. Preserve earlier
58worker controls as applicable; remove unused StartRead standalone root from new
chain's required/executed-provider claims (SetupRead inlines its admission body).
Counted-return15provider model can attach as separate accepted chain evidence, do not
substitute it for new actual worker proof. New bank callback/UID/Finished/node-free/
registration/refcount/owner/data/info destruction defects should fail promptly via
current independent counts/guards; no multiple same-evidence restarts on timeout.
No Lua,installed game/save/runtimelease/Git mutation,full gate_all/dashboard. Broad
native coordination protections remain; no idle scan window/no artifacts released.
Goal active.


### Permanent native public bank-read/owner lifetime gate PASS

Previous goal turn was progress:15-provider BeginReadData model plus actual Windows
bank lifetime scratch exposed the complete runnable chain. Current permanent proof:

- Added rebuild/tests/integration/NativeBankBeginReadWorker_test.cpp from the passing
  scratch. Added gate_native_bank_begin_worker.py with complete fresh snapshots of
  all75 source objects, headers, tools, original image and fixtures. No production
  source/header change. Existing standalone StartRead is absent from this new graph;
  its admission body is inlined inside real SetupRead. No extra/shared root credit.
- Added bank_begin_worker_link_oracle.py:18 bank function audits (one global iterator
  overlaps an existing selection) and3 exact typed vtable contributions. Final graph
  has81 selected bodies,71 required source omission failures and1 rebalance selection
  control. LINK /map resolves actual final functions/data/IAT cells; all96 bank named
  fixups/table-slot pointers independently verified, including weak/explicit aliases.
  Original target map complete; preserved initial missing GetEntryDataOffset/ReadData
  audit failures, then validated frozen objects quickly before whole gate rerun.
- Explicit bank pointer-tree heap bindings: only Minsert malloc and range/clear free
  names are changed using per-object /D flags to guarded real-CRT-backed fixture
  interfaces. Literal instruction bodies compare to retail after independently named
  target patching. Final linked operands must bind those guard functions, not merely
  pass a byte mask. Pool/allocator/packed/default-movable cold aliases are explicit
  fail-fast methods; no executed bank-lifetime function is replaced by one.
- Positive24 actual managers/960requests/864real reads,288 public BeginReadData calls
  and6216 independently checked child bytes. Actual data ctor/registration/SetupRead/
  file+manager wrappers, bank OnFileReadBegin/SetupBuffer(nonnull Buffer)/completion,
  real data destructor/key/equal/range/clear erase and template counted-owner release.
  Guarded72B data/12B info/20B registration node each release once:288/288/288. Both
  raw bank trees empty after teardown,8B returned owner clears. Data72B expected final
  snapshot permits only UID0/Finished=true. Bank mutex role observations require each
 3enters/5leaves/2recursive successful tries. Queued PDst deliberately cleared AFTER
  independent admission descriptor check, requiring native callback publication.
- Strengthened independent admission observations: Length/SkipBytes/Aborted/buffer
  flags and nonnull owner Info/DeleteFunc checked. At native constructor lock entry,
  InternalAllocatedBufferSize must still equal allocation-fillCDCDCDCD; final value0.
  This distinguishes actual delayed initialization from merely compatible end state.
- Added bank_begin_worker_controls.py:23 new source defects, plus53 existing worker/
  installer controls; all76 linkable runtime negatives fail cleanly80 with a fixture
  assertion (no crashes/timeouts/elevation). New defects cover buffer/length/info/
  refcount/DeleteFunc/request submission/UID/retention/callback/offset/begin result/
  buffer publication/SetupBuffer execution/result/Finished/UIDclear/restart flag/
  unregister/node free/node value/early size store/registration/destructor lock role.
  Preflight23/23 at work/native-recreation/loading/bank-begin-read/20261019/
  preflight-153807/receipt.json; whole final gate recompiled and reran all76.
- Initial missing-info control compiled away DeleteData and failed an export link
  rather than runtime. Modified defect to omit info for nonzero Length, retaining
  its ordinary template helper for the zero-Length branch. Null DeleteFunc control
  likewise retains helper availability. No primary implementation change. Windows
  filename heuristics refused bank-omit-setup.exe with740; renamed cases to avoid
  setup keyword (bank-skip-request/bank-begin-skip-buffer), no elevation requested.
  All failure and preflight directories preserved. Two slow PowerShell read handles
  later completed successfully; clean cmd shell continued work. No inventory worker
  contention attributed, no STOP-INVENTORY or idle window offered.

Latest accepted receipt: D:\Documents\FableTLC\rebuild\build\native-bank-begin-worker\00\9d\009d6060\20261019T153822685949Z/receipt.json.
Independent verification file same directory:249 input/87 object hashes unchanged,
76 separate negative run receipts all80,81 bodies/3 tables/96 final bank fixup bindings,
288 each data/info/node releases. Fresh attached BeginReadData15-provider model
8scenarios/56begins/7controls and original removal360sequences/2016removals retained.
Registered new gate in gate_all with81/71/1/76 marker; full gate_all not run.
Only existing009D6060 override evidence and ARTIFACT_INDEX row updated; every prior
unrelated row verified byte-equivalent. Prior metadata archived under bank-begin-read/
20261019/before-lifetime-metadata. No catalog route for this75-object integration
through the two-object candidate driver; dedicated registered gate is authoritative.

SCOPE: bank/entry/file contexts remain fixture-owned raw contexts with real bank
critical sections. Caller-supplied buffers, cached asynchronous RuntimeData-offset
path; engine pool/allocator, packed metadata, synchronous disk path and full bank
construction/close are cold. Owners released only after worker joins/completion;
active-owner destruction and allocation failure not proven. Each bank currently
contains a SINGLE read registration, so erase uses the singleton/whole-tree path.
Do not extrapolate to shared-bank partial erasure or full game startup.

NEXT: extend this accepted chain to shared bank contexts (at least one per device)
with multiple simultaneous constructed data owners. Real insertion should create
multi-node trees; release in varying orders and validate redblack invariants/counts/
header extrema/physical node identity+guarded frees after each actual destructor.
Use a BankContext carrying shared raw bank/entry/file/header records and aggregate
bank lock-role counts; per-slot BankForSection assumptions currently identify unique
mutexes and must be revised explicitly. Preserve per-data72B snapshots/refcount1,
returned owner clears, actual read byte guards and old76controls. Native requests
must reference the shared bank's file context; update independent expected sort key
and pointer checks rather than silently retaining each slot's old file identity.
Cached async buffers remain first scope; no full-bank lifetime claim. This exercises
actual key/range/rebalance partial erase beyond current singleton clear without
inventing new recovery credit. All protected paths/branches and no-Lua/game/save/
lease/Git/full-gate-all/dashboard constraints persist. Goal remains active.


### Shared-bank multi-owner Windows read/partial teardown gate PASS

Previous goal turn was progress: permanent singleton bank lifetime passed81bodies/
96bindings/76controls. This turn replaces the per-read bank fixture with TWO shared
bank contexts per batch (one per device) while native data/owner methods stay intact.

- New NativeBankSharedWorker_test.cpp and gate_native_bank_shared_worker.py retain
  actual public BeginReadData/data ctor/SetupRead/real read callbacks/native data
  destructor/owner release and guarded72B data/12B info/20B pointer node allocation.
  Each shared raw bank holds multiple simultaneously live native data registrations.
  Native requests point at its shared file context; independent descriptor/sort-key
  checks were updated to its actual shared file identity, not left as per-slot fakes.
  Raw entry/runtime offset metadata also shared; cached async caller-buffer scope.
-24batches/48contexts/288 bank reads/6216checked bytes. Original manager/installer
  aggregate960requests/864reads/24+24joined threads retained. Native teardown frees
  exactly288 data,288 info and288 original physical registration nodes with16B guards.
  Both shared bank trees end empty; every8B owner clears and each pre-release info
  RefCount1/data72B state checked. Real bank mutex counts aggregate3enters/5leaves/
 2recursive tries per owner, with stage equations for ctor/read/completion/release.
-624 independent tree validations after every insertion/destruction plus final context
  checks: count, root/extrema/header links, unsigned pointer-key ordering, unique/live
  physical allocation identity, parent pointers, red-child rules, blackheight and guards.
  Membership is validated BEFORE dereferencing any child/color/header-extreme pointer;
  controls that leave stale links therefore fail without reading retired memory.
-Four release orders, six batches each:creation order,reverse,address median and
  root-first.240 partial deletions plus48 final clears;88 observed two-child deletions
  in accepted run (only >=12 required, not an address-layout-dependent fixed count).
  This executes genuine key/equal/range/rebalance physical erasure beyond prior
  singleton clear. Exact pointer-node free must match the particular releasing owner.
- Added bank_shared_worker_controls.py with7 scoped ordinary-C++ defects in the bank
  range/iterator erase instantiation: skip partial erase,clear all owners,erase another
  owner,omit partial free,omit count decrement,corrupt extrema,free without detaching.
  Header-defined iterator defects use isolated expanded header bodies in scratch
  control TUs; canonical headers unchanged. All7 plus existing76 runtime controls
  link and fail cleanly80/assertion. No crash/timeout/elevation or production source fix.
- Passing first scratch shared-154943/receipt.json and permanent first full run;
  no byte tuning or private ABI/header changes. New CPP fixture only extends observed
  scope. Complete fresh75-source/header/toolchain snapshots,81selected linked bodies,
 3vtable contributions/96 final bank named targets,71strict required source omissions,
 1global rebalance selection control,83negative runtime receipts. Native standalone
 15-provider56begin/7control model and original pending-removal model retained.

Latest accepted receipt: D:\Documents\FableTLC\rebuild\build\native-bank-shared-worker\00\9d\009d6060\20261019T155340426945Z/receipt.json.
Independent verification same directory:250 input/87 object pins intact,83 negative
run receipts all80,96 final bindings,624 tree checks/240 partial/88 two-child deletions.
Registered gate_all marker81/71/1/83 (full gate_all not run). Updated exactly the
existing009D6060 override and ARTIFACT_INDEX rows, all other rows preserved; archive
bank-begin-read/20261019/before-shared-metadata. No new source/root recovery credit.
No Lua/game/save/lease/Git mutation, dashboard or cleanup actions/artifact releases.

NEXT: actual native counted-owner destruction while request is PENDING, before read
callbacks, to connect data destructor cancellation to actual manager pending/UID erase.
Useful inspected native evidence: CThreadedFileManager constructor0098F3A0
starts its own worker immediately with _beginthread(BeginThread,0x3000,this).
BlockPendingObjectProcessingRefCount is recovered private field+16C (fixture observes its established ABI); real ProcessObjects
checks it after finishing current IO and goes to WaitForSingleObjectEx(Event,INFINITE,
true). Start empty manager, acquire its real CS and set block1 BEFORE adding data.
No pre-existing queued/read work makes initial constructor race harmless. Queue owner
through public BeginReadData on main, recording returned pointers/request/overlap under
outer manager CS before allowing worker selection. Pending block should prevent all
read/bank-begin callbacks; unexpected TryEnter/ReadFileEx can fail promptly.

Keep nonzero initial UIDs1..N for first domain. Real data destructor already unregisters
both bank trees, then if UID nonzero loops actual file RemoveObject until accepted,
releasing bank lock/Sleep(0)/relocking on refusal. Pending removal succeeds immediately:
data still alive but Finished=false/UID nonzero, actual request/20B overlap freed on main
while native bank lock held, then data/info freed. Expected bank mutex counts2enters/
2leaves per owner and0tries, no read/APC/completion, caller buffer unchanged. Pending
map/UID/statistics and per-bank tree invariants should update after each release in
varied orders. Existing pending-removal/request ownership flags can be adapted to mark
slot.removing/cancelling; do not retain success-only bank completion assertions for
this genuinely different domain. Add explicit Sleep import observer to reject unexpected
retry in the pending-success case; constructor admission now main-thread role, rather
than parent worker callback role. Manager constructor/worker and shutdown stay real.

IMPORTANT original behavior to preserve: data destructor guards removal with if(UID),
so zero is special at this bank API despite manager accepting UID0. Do not invent a
source fix or claim zero pending-owner lifetime before separate original evidence.
Start nonzero cancellation, then explicitly investigate controlled zero case without
letting a worker use a freed callback. Next later domain can hold a real ReadFileEx
completion through owned events and observe destructor unlock/Sleep/relock while
READING, then completion/reclamation. Current owners still drop only after worker
joins; do not extrapolate the accepted gate to active cancellation yet. Full bank/
entry/file construction and pool/allocator/sync/packed paths remain outside current
borrowed-context domain. Goal active; broad native coordination deferment/no idle
metadata scan window/all branches and artifacts protected persist.


### Native pending last-owner cancellation accepted; zero sentinel observed separately

Previous goal turn was progress: shared-bank native read/partial erasure passed83
controls. Current turn adds the distinct unfinished/pending cancellation domain:

- Added NativePendingOwner_test.cpp, gate_native_bank_pending_owner.py and
  bank_pending_owner_controls.py. Actual manager constructor starts its own worker;
  fixture controls recovered processing-block field+16C under manager CS BEFORE
  queuing public BeginReadData calls on main. No native source/header changes.
  Worker must produce at least TWO qualifying manager waits with queued requests,
  block1 and no current IO. Event pulses are bounded5s. A pre-admission empty-queue
  wait cannot alone pass this handshake; ignore-block defect reaches a prohibited
  bank try/read immediately and fails. Manager fields are private; raw established
  ABI offsets used without widening class access. Initial fixture compile corrected
  direct private Event access to+164 and added correct ResetEvent import.
-24batches/288 real last-owner drops while requests PENDING, nonzero UIDs from starts
 1,7FFFFFF8,80000000,FFFFFFE0 (signed boundary covered, zero wrap excluded). Native
  info RefCount1->0 invokes actual deleting wrapper/data destructor. Real bank node
  unregister happens first; real file/manager RemoveObject then erases pending+UID
  nodes and frees request+20B overlap on main while data alive and bank CS held.
  Request-deletion observation sees Info.RefCount0/Finished=false/UID unchanged and
  already-freed correct bank registration node. No callbacks,APCs,reads or retry sleeps.
  Data/info/node each288 frees once;24 actual workers joined.48 shared bank contexts,
 624 bank-tree validations,240 partial deletions,111 observed two-child erasures in
  accepted run (minimum12, not stable expected value),four release orders6each.
-312 independent manager validations: UID ordering/count/value identity, all14 pending
  maps including tie-breaks on request pointer when file sort key/offset equal, cursor
  sentinel preservation, immutable complete64B LIVE request snapshots, zeroed20B
  pending overlaps and4real count/byte statistics getters. Complete72B data expected
  state computed from caller fields/descriptor/CD padding (vtables separately bound),
  no copied-default-field blind spot. Caller buffers stay byte-for-byte unchanged.
  Bank aggregate2enters/2leaves per owner and0tries; node free must occur with main
  owning bank lock at recursion1. Explicit Sleep import observer rejects any pending
  success retry while native destructor active; shutdown sleeps remain actual Win32.
-41 focused runtime source defects all link and fail cleanly80. Selected16 applicable
  bank controls and7partial-tree controls, plus18 block/admission/manager removal/
  file removal/owner UID/lock/worker/stats controls. Read-completion-only defects from
  the existing83-control read gate are NOT relabeled as exercised here.23IO-related
  scope statements remain in older registered gate. Preflight41/41 passed at
  pending-preflight-162256; full gate freshly recompiles all75objects and reruns them.
  First full build only failed inherited GetThreadPriority import assertion: unused
  old fixture priority observation optimized out in this no-read domain. Removed
  that stale import requirement; no engine code/ABI changes. Failed build preserved.

Latest accepted receipt: D:\Documents\FableTLC\rebuild\build\native-bank-pending-owner\00\9d\009d6060\20261019T162401650103Z/receipt.json.
Independent verification same directory:251 input/87 object hashes intact,81selected
linked bodies/3typed tables/96 final bank named fixups,71required source omissions,
1rebalance selection control,41negative run receipts all80. Attached original-removal
360sequence/2016removal and BeginReadData15provider models retained. Full gate_all
not run; dedicated pending gate registered. Exactly existing009D6060 override and
ARTIFACT_INDEX rows updated, all unrelated rows preserved; prior metadata archive
bank-begin-read/20261019/before-pending-metadata. No new root/recovery credit.

ZERO-UID ORIGINAL BEHAVIOR OBSERVATION (scratch only, not safe-lifetime acceptance):
- probe_zero_owner.py -> zero-owner-162802/receipt.json. Inject native manager next
  UID0, admit one actual public bank read, confirm two blocked worker waits, then
  drop its actual last counted owner. Native348B exact-retail destructor takes
  if(UID) skip-removal path. Data72B/info12B/bank node20B freed once, both bank trees
  empty, but native UID0 request and overlap remain pending. Complete64B request
  unchanged, its callback pointer equals retired data address, manager ObjectExists0
  true/IsFinished0 false. Only compare the retired pointer; never dereference it.
- Worker stays blocked for entire observation and cleanup. Fixture explicitly calls
  actual file RemoveObject(0) AFTER recording owner behavior, suppresses only the
  success-domain fixture child-data dereference for this manual cleanup, verifies
  actual request/overlap gone and joins worker. No pending callback executes.
  No source fix/workaround added. Full original game not run: original348B body
  hash/disassembly and exact-grade rebuilt graph tie the observation to original
  branch; original-evidence.json saved beside scratch receipt. This remains a real
  scope limitation of bank owner destruction at zero, not a failed nonzero gate.

NEXT: native owner destruction while a real nonzero request is READING, exercising
actual destructor bank unlock -> Sleep(0) -> lock retry until worker completion.
Use owned event handshake in a dedicated fixture/read observer: issue real ReadFileEx,
signal main that request is reading, then hold worker non-alertably until main's
destructor Sleep observer confirms bank lock RELEASED and signals worker to continue.
Main must be able to hold returned owner until this point; choose nonzero UIDs and
no unrelated UID0 entry (callback publishes UID0 before destructor next removal).
Worker can then process actual APC/bank completion while main destructor retries.
Observe conditional g_releasingBank carefully: it is global and can remain set while
WORKER enters the bank completion lock, so old 'if releasing then main' observations
must become explicit thread/phase checks. Both constructor and drop now main-thread
roles; begin/setup/completion remain worker roles. Real bank data destructor installs
its class vtables while waiting, so callback still reaches actual bank methods.

Potential valid reclamation order: bank callback sets Finished/UID0 and releases its
bank lock; main destructor may finish/free data BEFORE worker destroys request.
Do not dereference data from request-delete observer after data retirement. Capture
completion publication while data alive/worker owns bank lock; verify source has no
data reads after that unlock (original completion63B) and use ledger flags afterward.
Do not assert request deleted before data unless synchronization actually proves it.
Manager OnFinish should not use callback data after returning. Start one held read,
then expand shared owners. Bound all waits/retries and detect missing lock-release/
completion/lifetime steps via actual observations. No early zero-UID lifetime claim,
allocation-failure/full bank/game/pool/sync/packed claim. All branch/path/evidence
protections persist; no idle metadata window or released artifacts. No Lua/game/save/
lease/Git mutation/full gate_all/dashboard. Goal active.


## 2026-10-19 native active-read last-owner teardown

Permanent `gate_native_bank_active_owner.py` and `NativeActiveOwner_test.cpp` pass
24 owned processes:24 real Windows reads/888 independent bytes,48 native destructor
retry sleeps,24 exactly-once data/info/registration-node releases,24 manager joins
and24 installer joins. Each process uses one nonzero UID1, a caller buffer and cached
bank metadata. Worker is held nonalertably after actual ReadFileEx until the second
native Sleep(0), so APC delivery cannot hide the protected READING removal boundary.
Both retries require bank mutex unowned, request/overlap live and data/info alive.
Completion takes bank mutex with manager mutex unowned, publishes Finished/UID0,
and its full72B state is checked while protected. Physical data free requires that
publication. Request retirement uses only ledgers after possible data reclamation.
A separate request-retired event plus manager-lock roundtrip prevents shutdown from
racing legitimate worker retirement. Bank lock counts5enters/7leaves/2recursive tries.

75 freshly compiled VC7.1 providers reproduce all81 accepted body/relocation records.
Older tree records omit COFF kinds; these are recovered from hash-pinned accepted
objects rather than inferred from the candidate.70 final linked bodies/tables and322
actual instruction fixups verified, including96bank fixups; direct original bank
audit retained.63 required link omissions,11 unneeded providers,30 runtime defect
controls all pass. Independent verification checks308input/75object hashes,24 positive
receipts and30 explicit80/NATIVE_WORKER_FAILURE receipts. Original63B completion
disassembly confirms only pop/add/ret after lock-destructor call, supporting physical
reclamation before worker callback epilogue finishes. No new root/source credit.

Receipt: rebuild/build/native-bank-active-owner/00/9d/009d6060/20261019T174246147253Z/receipt.json
Independent proof: same directory/independent-verification.json.
Scratch history: work/native-recreation/loading/bank-begin-read/20261019/active-owner-*
and active-controls-*; all preserved. Missing-info control initially exposed a test
null guard; corrected before promotion. Callback-keeps-manager-lock originally hit
the bounded watchdog; explicit callback lock-ownership check now rejects it before
blocking. First permanent compile comparison encountered old relocation schema;
corrected using pinned prior objects, preserving failed evidence.

gate_all registration added but full gate_all/dashboard NOT run. Only009D6060 rows
updated; prior rows archived in before-active-metadata/prior-rows.json and unrelated
TSV rows byte-preserved. Accepted shared-worker/pending-owner gates remain intact.
UID0 original skip-removal limitation remains unchanged. No pool/allocation-failure,
packed metadata,shared active concurrency,whole bank startup or installed-game claim.
Next: last-owner release after actual APC marks READ_COMPLETED but before bank virtual
completion, with native protection/refusal/retry and publication proof. No Lua.

Cleanup response appended to maintenance/coordination/native-recreation.txt:
all native work/build trees including parent traversal remain protected; no artifacts
released and no metadata idle window. No game/save/lease/Git mutations.


## 2026-10-19 native owner drop after APC, before bank callback

Permanent NativeCompletedOwner_test.cpp/gate_native_bank_completed_owner.py PASS:
24 actual Windows reads/888 checked bytes; native APC changes request status to
READ_COMPLETED3 while bank Finished remainsfalse/UID1/callback still live. Worker
is held nonalertably at the end of the APC observer, before native OnFinish dispatch.
Last counted owner unregisters its bank node and retries native RemoveObject twice;
both retries require state3/request+overlap alive/bank mutex unowned/data+info live.
Only second native Sleep(0) releases worker. Native bank callback then publishes
Finished/UID0 before data/info reclamation. No retired-data dereference in request
retirement. All24 owner triples/request+overlap/manager workers/installer workers
retire exactly once; per-process5enters/7leaves/2recursive tries and payload guards.

75fresh VC7.1 providers reproduce81 accepted native body/relocation records;70final
linked bodies/tables and322instruction fixups,including96bank bindings;63required
omissions.31defect controls pass explicit80/NATIVE_WORKER_FAILURE, including illegal
READ_COMPLETED removal.309input/75object hashes independently verified. No root credit.
Receipt: rebuild/build/native-bank-completed-owner/00/9d/009d6060/20261019T174651145509Z/receipt.json
Independent verification: same directory/independent-verification.json.

Shared fresh-provider runner now takes explicit reading/completed phase; separate
fixture,report,build tree and gate entry remain. Reading domain fully refreshed:
24reads/48retries/30controls/308input+75object pins; receipt
rebuild/build/native-bank-active-owner/00/9d/009d6060/20261019T174736934542Z/receipt.json.
The first phase refactor build caught case-sensitive imported SOURCES names; fixed
and failed evidence retained. Completed scratch24runs/31controls retained at
work/native-recreation/loading/bank-begin-read/20261019/completed-owner-174535.

gate_all entry added; full gate_all/dashboard not run. Only009D6060 TSV rows changed,
unrelated rows byte-preserved and prior rows archived before-completed-metadata.
No native production/Lua/game/save/lease/Git mutations. UID0 original limitation,
functional-only predecessor and near151B SetObjectPriority unchanged. Single UID1,
caller buffer/cached metadata only; no pool/failure/shared-active/fullbank claims.
Next: pending-locked owner teardown before native OnFileReadBegin bank-lock attempt.
Original ProcessObjects stores1 and releases manager mutex before callback; failed
begin returns it to0, successful begin advances to2 then APC3. This boundary needs
its own controlled timing and native ownership proof.


## 2026-10-19 pending-locked last-owner teardown

NativeLockedOwner_test.cpp/gate_native_bank_locked_owner.py PASS24 processes:
native worker stores PENDING_LOCKED1 and releases manager mutex before calling
the actual bank begin method. Its first TryEnter is held before acquiring bank
mutex. Last-owner destructor unregisters node and reaches two real Sleep(0) retries,
each with request1 and data/info still live, bank mutex unowned. Second retry permits
native begin to acquire its real recursive locks/publish buffer, then real Windows
ReadFileEx/APC and bank completion publish Finished/UID0 before physical reclamation.
24reads/888 independent bytes/48native retries; all owner triples, requests/overlaps,
manager workers and installer workers retire once.31defect controls reject premature
locked removal,wrong callback/UID/refcount,missing unlock/relock,short transfer,etc.

75fresh VC7.1 providers reproduce81 accepted native bodies.70final linked bodies/tables,
322actual instruction fixups including96bank bindings,63required omissions. Existing
reading/completed phases fully refreshed after shared-runner extension. Gate Python
inputs (including imported local pipeline dependencies) now also snapshot into each
build's python-provenance directory, with hashes, for historical review after edits.
Independent receipts/pin/run/control verification for all three domains:

{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "locked", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-locked-owner\\00\\9d\\009d6060\\20261019T175102298402Z\\receipt.json", "receipt_sha256": "5fd9cbb9e0d4d439813ccd07a46fb6f824182768d217d2870754e9c53bb4957c", "status": "PASS"}

{"bank_fixups": 96, "controls": 30, "inputs": 355, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "reading", "positive_runs": 24, "python_snapshots": 63, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-active-owner\\00\\9d\\009d6060\\20261019T175144566699Z\\receipt.json", "receipt_sha256": "0d424d0dc2db1a9c2ae4c5c75d4c7b02c8ee40d1784f59e3d9a3b1dc2dfdb5fb", "status": "PASS"}

{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "completed", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-completed-owner\\00\\9d\\009d6060\\20261019T175249762202Z\\receipt.json", "receipt_sha256": "8a54ae83bf5e13fd23243fc0d44e6737860418d9fe52098d030760b169f23271", "status": "PASS"}

Scratch success locked-owner-175000 retained. First fixture observation confused
begin-lock leave with completion during active teardown; publication now checks
request state3 before reading expected completion fields. An initial generator
newline-escaping syntax error was corrected before compiling; no production edits.
No new root credit. No Lua/game/save/lease/Git changes; no full gate_all/dashboard.
Only009D6060 TSV rows changed and old rows archived; unrelated rows byte-preserved.

Next: native begin refusal while teardown still owns bank mutex, then ProcessObjects
returns request to PENDING0 and owner cancellation succeeds without IO. Current locked
gate deliberately releases bank mutex before permitting begin, exercising success
only. Original failed path stores0, releases manager mutex, signals event and invokes
SleepEx(1,true) before returning. Control that timing with owned events, preserve live
callback storage through return and verify cancellation/free on main thread. UID0,
pool/allocation failure/shared-active/fullbank startup remain outside current proof.


## 2026-10-19 native bank begin refusal during last-owner destruction

NativeRefusedOwner_test.cpp/gate_native_bank_refused_owner.py passes24 processes
and19 explicit defect controls. Worker reaches PENDING_LOCKED with manager mutex
released; main enters native last-owner destructor and owns bank mutex. Its tree
node release observer permits the real worker TryEnter to fail. Callback returns
while data/info/request/overlap remain live. Native ProcessObjects restores PENDING,
releases manager mutex and reaches SleepEx(1,true); owned events then permit actual
main-thread RemoveObject cancellation. All14 pending tree roots/counts/extrema/cursors
are empty before overlap/request reclamation. Data/info/node/request/overlap each
retire once; no read, APC, completion, installer pause, buffer change or retry.
Native manager and installer workers join.75 fresh providers reproduce81 admitted
bodies; direct bank binding and final linked instruction audits pass. No new roots.

First19-control run exposed insufficient pending-tree checks: removing native pending
erase still passed shutdown. Added independent queue checks before memory release;
that defect now explicitly fails. First queue check used cursor word+2; corrected
to original layout+3 and reran. Failed builds and every receipt retained.
All prior reading/completed/locked phases refreshed after runner extension.
Independent current-input/object/executable/Python-snapshot/run/control verification:
{"bank_fixups": 96, "controls": 19, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "refused", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-refused-owner\\00\\9d\\009d6060\\20261019T185902312812Z\\receipt.json", "receipt_sha256": "0e6d9f2dc96453b8f8b994dad40865e4f02bc57e8fbf25d6bcf1bff24c2e2596", "status": "PASS"}
{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "locked", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-locked-owner\\00\\9d\\009d6060\\20261019T185929014445Z\\receipt.json", "receipt_sha256": "28b1ffcce1067a57dbfdedf6e45d7e6c1674aa6d57fa9be5f4f2500d77e505e3", "status": "PASS"}
{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "completed", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-completed-owner\\00\\9d\\009d6060\\20261019T185939182332Z\\receipt.json", "receipt_sha256": "99e8ad79d295985203d5c7965071bee2b156f4ef7d535a515957520a70b088a8", "status": "PASS"}
{"bank_fixups": 96, "controls": 30, "inputs": 355, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "reading", "positive_runs": 24, "python_snapshots": 63, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-active-owner\\00\\9d\\009d6060\\20261019T185949439517Z\\receipt.json", "receipt_sha256": "bdd202fb572b26c33226b27fd66676a02d31933552c8c35ce5cab86c374d99b1", "status": "PASS"}

Gate registered; full gate_all/dashboard not run. Only009D6060 metadata rows changed;
prior rows archived, unrelated rows byte-preserved. No production/Lua/game/save/Git
changes. UID0 limitation, functional-only predecessor and near151B SetObjectPriority
remain unchanged. Caller-buffer/cached-metadata/single-owner boundary remains.
Next: bank begin refuses once under contention, then worker retries successfully,
followed by native last-owner release and actual read/APC/completion. Existing refused
gate cancels before retry; prove real worker retry without leaving stale pending state.


## 2026-10-19 refused bank begin followed by actual worker retry

NativeRetriedOwner_test.cpp/gate_native_bank_retried_owner.py PASS24 Windows reads,
888 checked bytes and48 native destructor retries;33 explicit defect controls pass.
First native bank TryEnter fails against main-owned mutex. ProcessObjects restores
pending, releases manager mutex and calls SleepEx(1,true). Full72-byte bank state and
refcount are unchanged. Events release contention and permit actual worker queue
reselection; second callback is PENDING_LOCKED again. Last-owner destruction then
unregisters, retries protected removal twice, and permits successful bank begin,
ReadFileEx/APC/native completion before physical reclamation. All releases once.
Six bank enters/eight leaves, one failed try and two recursive successful tries.
75fresh providers/81reproduced bodies/70linked bodies+tables/322fixups/96bank bindings/
63required omissions. Four previous owner domains refreshed and independently verified.
{"bank_fixups": 96, "controls": 33, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "retried", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-retried-owner\\00\\9d\\009d6060\\20261019T190143169513Z\\receipt.json", "receipt_sha256": "de84c721ad323e52c284a331fb0f0ac5ac3c407c9c0f5e26cff2c5317e361bd4", "status": "PASS"}
{"bank_fixups": 96, "controls": 19, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "refused", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-refused-owner\\00\\9d\\009d6060\\20261019T190230480816Z\\receipt.json", "receipt_sha256": "1518d76d7602046842afd7de172665ab5a868ca1dfb45938f6ced53129f8cb71", "status": "PASS"}
{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "locked", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-locked-owner\\00\\9d\\009d6060\\20261019T190230480525Z\\receipt.json", "receipt_sha256": "f2d487ffdaa3c93644806bdbca053b0956052507e82d8ae0a740bbcd80e22960", "status": "PASS"}
{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "completed", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-completed-owner\\00\\9d\\009d6060\\20261019T190230479498Z\\receipt.json", "receipt_sha256": "619913374ed269b9c519f420aff9900c116cd57411854020bf8654f4666ef787", "status": "PASS"}
{"bank_fixups": 96, "controls": 30, "inputs": 355, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "reading", "positive_runs": 24, "python_snapshots": 63, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-active-owner\\00\\9d\\009d6060\\20261019T190230484185Z\\receipt.json", "receipt_sha256": "727c8bd508a1a69b198e5d84005222924ed4e9ad75a9d4dec9879a8dc84828ea", "status": "PASS"}

Gate registered; no full gate_all/dashboard or new root credit. No production/Lua/
game/save/Git changes. All receipts and source snapshots retained. Next native source
target: ChangeLoadingPriority009D5630 still uses padded stand-in classes and a mirrored
test. Audit original bytes/PDB, replace with actual typed member and candidate-required
test if the first source meets parity; preserve existing accepted source evidence.


## 2026-10-19 typed native bank loading-priority member

ChangeLoadingPriority009D5630 now uses actual CBankFileAsyncData, unsigned UID,
CBankFileAsync and CThreadedFile declarations. Both donor PDBs agree on public
void thiscall with one enum argument. Exact30B after named call relocation to
actual10B file wrapper. No new root credit; replaces existing accepted padded
stand-in facade. Original/rebuilt864cases across UID0/extremes/priorities/receivers,
six defect controls, full state/stack/callee-saved checks. UID0 must not access an
unmapped bank pointer. Replaced mirrored legacy test with compiled-candidate host
864cases and explicit missing-root link rejection. Catalog pass marker corrected.
676 pre-existing header consumers compile unchanged; new typed TU is677th.

First include order hit the existing counted-file specialization constraint; using
threaded-file before game-bank headers matches established TUs. Native fixture first
used generic Data name instead of specialized data_, then offsetof on a reference
member faulted; fixture now populates verified reference storage at+0x28. All failed
evidence preserved. Production body exact on first compilable spelling, no byte tuning.

{"controls": 6, "emulated_cases": 864, "host_cases": 864, "host_omission": true, "old_tus_unchanged": 676, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-priority\\00\\9d\\009d5630\\20261019T190758677811Z\\receipt.json", "receipt_sha256": "16aa61d406b9f4688daf9786f33d117790a405ad5204c0b158ad5a6d80cfd79c", "status": "PASS"}

Permanent gate registered; only009D5630 metadata changed, originals archived.
No full gate_all/dashboard, game/save/Lua/Git changes. Next: compose typed bank priority
with actual manager queue moves after refused begin and before successful worker retry.
Earlier ownership receipts retain frozen headers/Python provenance; a later header
change means their live-input pins are historical until refreshed.


## 2026-10-19 bank-driven priority moves during actual worker retry

NativePriorityOwner_test.cpp/gate_native_bank_priority_owner.py PASS24 reads/888bytes,
48 typed-bank priority moves and48 destructor retry sleeps. First begin refuses under
bank mutex contention. While native request returns to pending, actual bank member
calls file wrapper and manager to move priority3->5->1. Every step checks all14 pending
trees/counts/extrema/cursors, UID/request identity, callback and unchanged full bank
state/refcount. Worker reselects request and begins actual read; owner teardown waits
through APC/completion then reclaims exactly once.36 defect controls pass, including
missing priority call, wrong UID, ignored priority, skipped pending restore and premature
locked removal.76fresh providers/82reproduced bodies/73linked bodies+tables/331fixups/
96bank bindings/65required omissions; added bank member itself required at link.
All five earlier owner gates refreshed after typed header/runner changes.
{"bank_fixups": 96, "controls": 36, "inputs": 357, "linked_bodies": 73, "linked_fixups": 331, "objects": 76, "omissions": 65, "phase": "priority", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-priority-owner\\00\\9d\\009d6060\\20261019T191012624906Z\\receipt.json", "receipt_sha256": "d2cb73b655e8a5c54faa9c66534d2a4376a132aaac892ddc82a5cabd2bf5f60d", "status": "PASS"}
{"bank_fixups": 96, "controls": 33, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "retried", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-retried-owner\\00\\9d\\009d6060\\20261019T191133801138Z\\receipt.json", "receipt_sha256": "a6a85f62233cc38f5c1fc6bb7aed71a8c17beb8ee1c1536ee29d37ea81ac113e", "status": "PASS"}
{"bank_fixups": 96, "controls": 19, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "refused", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-refused-owner\\00\\9d\\009d6060\\20261019T191133765683Z\\receipt.json", "receipt_sha256": "f6ec88aeac21ec00e29b39d96c659ed800a5a7b076a488277d5c38f36b56f8f0", "status": "PASS"}
{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "locked", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-locked-owner\\00\\9d\\009d6060\\20261019T191133762595Z\\receipt.json", "receipt_sha256": "9041fb40cfda0fb0409c7444decbd421e706d0552a791ff8690ce8e94087f95c", "status": "PASS"}
{"bank_fixups": 96, "controls": 31, "inputs": 356, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "completed", "positive_runs": 24, "python_snapshots": 64, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-completed-owner\\00\\9d\\009d6060\\20261019T191133763996Z\\receipt.json", "receipt_sha256": "9f32237cdb84054a4c3927af3bcfa8676a5c128c4c2e6acd051bafbf612e404a", "status": "PASS"}
{"bank_fixups": 96, "controls": 30, "inputs": 355, "linked_bodies": 70, "linked_fixups": 322, "objects": 75, "omissions": 63, "phase": "reading", "positive_runs": 24, "python_snapshots": 63, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-active-owner\\00\\9d\\009d6060\\20261019T191133765110Z\\receipt.json", "receipt_sha256": "a34f3b67b5a41494c47206245c42d9f96ba3731d9debe37de506cbbae62729fd", "status": "PASS"}

No new root credit or production changes in this composition. Gate registered; no
full gate_all/dashboard/game/save/Lua/Git changes. Next: actual static DiscardAsyncData
transfers active owner into native disposal list while Abort refuses, then bank completion
and ProcessDisposalList drain all storage. Scratch under loading/bank-disposal/20261019.


## 2026-10-19 actual active-read disposal ownership

NativeDisposalOwner_test.cpp/gate_native_bank_disposal_owner.py passes24 real Windows
reads/888bytes and33 explicit defect controls. Native static discard/bank method calls
actual Abort while request is READING. RemoveObject refuses; native disposal push
copies the counted owner and caller Reset releases its reference. Initial native
ProcessDisposalList retries Abort and retains the list node and sole reference.
Full bank state remains unchanged; bank registration,request/overlap/data/info alive.
Real APC/bank completion publishes Finished/UID0. Native request retirement occurs
before main calls ProcessDisposalList, whose actual finished Abort permits pop and
data/info/bank-node/list-node reclamation once. Ten real bank enters/twelve leaves,
two recursive tries, no destructor retry. Both native manager and installer join.

Original180B bank discard,16B static wrapper,59B list insertion,164B list processor
and196B Abort directly revalidated with exact relocations; final linked code/targets
verified. Allocator renames only expose list-node guard/count observations. Existing
root classifications retained, no new reconstruction credit. Scratch probe-191346
passed24 runs before permanent fresh-provider audit and fault controls.

{"bank_fixups": 96, "controls": 33, "disposal_node_frees": 24, "disposal_transfers": 24, "fresh_providers": 79, "input_pins": 361, "linked_bodies": 75, "linked_fixups": 345, "object_pins": 79, "omissions": 67, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-disposal-owner\\00\\9d\\009d6060\\20261019T191651812599Z\\receipt.json", "receipt_sha256": "6ff1e9b339dcbe438991e5176f06eb9271025577cfe51f97ee18b9bb878cabf4", "reproduced_bodies": 86, "status": "PASS"}

Gate registered; no full gate_all/dashboard/production/Lua/game/save/Git changes.
Earlier owner phases retain complete frozen inputs and Python provenance; their
runner live-input hash predates disposal extension. No arbitrary shared active bank,
pool,allocation failure,fullbank startup or UID0 safety claim. Next: drive disposal
processing through actual AsyncUpdatePending coordinator and empty restart-tree path,
both while read is protected and after completion. Preserve all earlier evidence.


## 2026-10-19 actual bank update/disposal coordination

NativeUpdateOwner_test.cpp/gate_native_bank_update_owner.py PASS24 Windows reads,
888checked bytes,48 actual AsyncUpdatePending calls and36 explicit defect controls.
During active READING, native update processes the list (actual Abort/RemoveObject
refuse) then visits empty restart tree. After completion and request retirement,
second update drains disposal owner/storage then processes empty restart tree.
Recursive-depth traces prove operation order before and after completion, including
final data destructor recursion. Fourteen bank enters/sixteen leaves; zero destructor
retries. Added16B coordinator/200B RestartAbortedReads directly match original and
are required at link, with final instruction targets checked. No nonempty restart
or new root credit. Original five disposal bodies remain directly verified.

{"bank_fixups": 96, "controls": 36, "disposal_node_frees": 24, "disposal_transfers": 24, "fresh_providers": 81, "input_pins": 363, "linked_bodies": 77, "linked_fixups": 351, "object_pins": 81, "omissions": 69, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-update-owner\\00\\9d\\009d6060\\20261019T191858165946Z\\receipt.json", "receipt_sha256": "3b4aef922a028e12030f682c6377ec5c88c87fe40833beb8139acac9e5ab5bac", "reproduced_bodies": 88, "status": "PASS"}

First control run was interrupted by Windows installer-name detection (WinError740)
for an executable prefixed update-. Renamed only the test-control labels to coord-
so they run as ordinary unprivileged probes; no elevation/lease/security changes.
Final full36-control run passes, including skipped disposal/restart and reversed order.
Gate registered; no full gate_all/dashboard/game/save/Lua/Git changes. All previous
receipts retain frozen sources/headers/Python provenance. Next: actual consumer
WaitUntilFinished plus IsFinished/GetData after real Windows completion, using native
bank/file/manager waiting and scoped owner release. Pool path remains out of scope.

Cleanup coordination FQT-OBJ-PRECHECK-20261019-NATIVE: verified manifest SHA256
fd893ab7b70fda48d466a861a71ea9a0c33022ef4684439262c97b479ffc45d5 and9 native receipts/
1710 dependency/artifact paths. No FQT dependency. Exact four ignored obj files only
received lane no-objection in maintenance/coordination/native-recreation.txt. Existing
native holds remain; no compression or lease/stop action performed by this lane.


## 2026-10-19 native bank consumer wait and data access

NativeConsumerOwner_test.cpp/gate_native_bank_consumer_owner.py PASS24 real reads/
888bytes,48 bank waits,48 IsFinished calls,24 GetData calls and34 explicit controls.
First query returns false. Actual bank WaitUntilFinished calls forced existing-buffer
setup, empty restart processing, releases bank lock and enters actual file/global
manager wait. Its single controlled SleepEx(0,true) requires bank/manager locks
released, request still READING and all owner storage alive. Actual ReadFileEx/APC/
bank completion and manager retirement proceed; native wait returns, IsFinished
returns true, GetData yields caller buffer, and second wait uses finished fast path.
Full bank state changes only Finished/UID; actual owner destructor frees once.
Ten bank enters/twelve leaves/two recursive tries; no teardown retry. Five added
Wait130B/restart200B/GetData77B/IsFinished65B/poolLock30B bodies verified directly and
final linked instructions/calls checked. PoolLock is linked but cold.

{"bank_data_queries": 24, "bank_finished_queries": 48, "bank_fixups": 96, "bank_wait_calls": 48, "controls": 34, "fresh_providers": 80, "input_pins": 362, "linked_bodies": 78, "linked_fixups": 355, "manager_wait_sleeps": 24, "object_pins": 80, "omissions": 69, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-consumer-owner\\00\\9d\\009d6060\\20261019T192326640122Z\\receipt.json", "receipt_sha256": "ded7166531e71fcf15db660673a2089a2580c6c1e26e6227845e3c31ed60a2b7", "reproduced_bodies": 86, "status": "PASS"}

First scratch expected BLOCKING priority after wait began, exposing an incorrect
fixture expectation: native SetObjectPriority only changes pending/pending_locked
requests; READING remains NORMAL. Corrected the observation, no production change.
Scratch probe-192213 passes24 runs; permanent gate includes full defects and hashes.
Gate registered; no full gate_all/dashboard/Lua/game/save/Git or new root credit.
Next: start consumer waiting while native bank begin is PENDING_LOCKED, and prove
actual blocking-priority queue promotion before successful read/completion.


## 2026-10-19 native consumer waiting during pending-locked begin

NativeQueuedConsumerOwner_test.cpp/gate_native_bank_queued_consumer_owner.py passes
24reads/888bytes,48bank waits and36 explicit controls. Worker is held before its first
bank TryEnter at PENDING_LOCKED1. Native bank wait checks buffer/restart tree and
releases mutex; actual global manager wait moves pending priority3->6. Before native
SleepEx permits worker begin, all14 pending trees/cursors/counts, UID identity, live
callback and lock release are checked. Native read/APC/completion/retirement then
finish, consumer query/data access and finished fast wait pass, all owners free once.
Added omitted/wrong blocking-priority controls explicitly fail. No source changes,
new root credit, full gate_all/dashboard, Lua/game/save/Git changes.

{"bank_data_queries": 24, "bank_finished_queries": 48, "bank_fixups": 96, "bank_wait_calls": 48, "blocking_queue_moves": 24, "controls": 36, "fresh_providers": 80, "input_pins": 362, "linked_bodies": 78, "linked_fixups": 355, "manager_wait_sleeps": 24, "object_pins": 80, "omissions": 69, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-queued_consumer-owner\\00\\9d\\009d6060\\20261019T192518102032Z\\receipt.json", "receipt_sha256": "53d3bc7d41763c4ebfd5772dca8ba17a333619700731c5b5993a45290fd4fecb", "reproduced_bodies": 86, "status": "PASS"}

Next: replace fixture-created bank entry with actual GetAsyncEntry factory, native
metadata size/alignment and counted entry lifetime, then run the accepted consumer
read path. Bank object initialization remains controlled; no wholebank startup claim.


## 2026-10-19 native entry factory through consumer read

NativeFactoryOwner_test.cpp/gate_native_bank_factory_owner.py passed 24 real reads/888 bytes and 43 defect controls. Actual GetAsyncEntry constructs the guarded entry and counted info from native cached metadata size37 and alignment8. The returned entry drives BeginReadData, PENDING_LOCKED priority promotion, native wait, Windows IO/APC, completion and consumer queries. After data-owner reclamation, the native counted entry destructor releases entry and info exactly once. Factory152B, Align24B, DeleteData11B, scalar destructor36B, GetEntryDataSize37B, counted destructor53B and entry vtable4B directly match original bytes and named relocations; final linked code and targets verified. Existing roots only, no new reconstruction credit.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 43, "entry_releases": 24, "fresh_providers": 82, "input_pins": 368, "linked_bodies": 85, "linked_fixups": 369, "native_entries": 24, "object_pins": 82, "omissions": 70, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-factory-owner\\00\\9d\\009d6060\\20261019T193252993071Z\\receipt.json", "receipt_sha256": "f17518b317c4f7755a578b88627d793c410cad814c3f2b645cfea86029d1882b", "reproduced_bodies": 93, "status": "PASS"}

Scratch probe-193050 passed24. Failed scratch include closure attempts and fresh omissions remain preserved. Exact accessor is37B (initial expected36 rejected). Factory emits a redundant counted-destructor COMDAT; the selected native body is checked and defect-link ordering explicitly selects its mutated replacement. Removed the fixture cold GetEntryDataSize alias so provider omission must fail. Full gate_all/dashboard, Lua, game/save and Git actions not run. Bank initialization and cached metadata remain controlled; packed metadata and wholebank startup are not covered. Earlier receipts retain their frozen Python/source provenance as shared gate inputs evolve.

Next: indexed cached metadata, then actual packed metadata accessor in factory/read composition.


## 2026-10-19 indexed entry metadata through consumer read

NativeFactoryOwner_test.cpp/gate_native_bank_factory_owner.py passed 24 real reads/888 bytes and 45 defect controls. Actual GetAsyncEntry constructs the guarded entry and counted info from native cached metadata size37 and alignment8. The returned entry drives BeginReadData, PENDING_LOCKED priority promotion, native wait, Windows IO/APC, completion and consumer queries. After data-owner reclamation, the native counted entry destructor releases entry and info exactly once. Factory152B, Align24B, DeleteData11B, scalar destructor36B, GetEntryDataSize37B, counted destructor53B and entry vtable4B directly match original bytes and named relocations; final linked code and targets verified. Existing roots only, no new reconstruction credit.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 45, "entry_releases": 24, "fresh_providers": 82, "indexed_entries": 12, "input_pins": 368, "linked_bodies": 85, "linked_fixups": 369, "native_entries": 24, "object_pins": 82, "omissions": 70, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-factory-owner\\00\\9d\\009d6060\\20261019T193545455974Z\\receipt.json", "receipt_sha256": "012d51ca14aa9f0e7834a4ed3bf88bf81c25db73ef548ff33bcaee71819d67f0", "reproduced_bodies": 93, "status": "PASS", "zero_index_entries": 12}

Scratch probe-193050 passed24. Failed scratch include closure attempts and fresh omissions remain preserved. Exact accessor is37B (initial expected36 rejected). Factory emits a redundant counted-destructor COMDAT; the selected native body is checked and defect-link ordering explicitly selects its mutated replacement. Removed the fixture cold GetEntryDataSize alias so provider omission must fail. Full gate_all/dashboard, Lua, game/save and Git actions not run. Bank initialization and cached metadata remain controlled; packed metadata and wholebank startup are not covered. Earlier receipts retain their frozen Python/source provenance as shared gate inputs evolve.

Twelve runs each use index0/offset0 and index2/offset0x313. Direct request FilePos and Size checks supplement payload checks. An initial0x300 displacement fault escaped because the payload repeats each256 bytes; retained failed gate documents this, corrected check rejects the offset fault. Next: actual packed metadata accessor in factory/read composition.


## 2026-10-19 packed bank metadata through native factory and read

NativePackedFactoryOwner_test.cpp/gate_native_bank_packed_factory_owner.py passes24 reads/888bytes and49 defect controls. Actual entry index3 selects two15-bit fields at bit30, spanning words; nonzero bias produces size37 and offset0x313. Actual packed decoder and literal masks, bank accessors/factory, queued wait, worker IO/APC/completion, consumer queries and counted destruction compose without runtime metadata fallback. Direct request FilePos/Size, payload, lock/state and guards checked. Getter101B and mask table132B match original and final linked bytes. Controlled bank initialization remains; no metadata parser or wholebank startup claim. No new root credit, production edits, Lua, game/save/Git actions or full gate_all/dashboard. Earlier accepted evidence retains frozen provenance.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 49, "cross_word_metadata_reads": 48, "entry_releases": 24, "fresh_providers": 83, "input_pins": 370, "linked_bodies": 87, "linked_fixups": 370, "native_entries": 24, "object_pins": 83, "omissions": 71, "packed_entries": 24, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-packed_factory-owner\\00\\9d\\009d6060\\20261019T193706475506Z\\receipt.json", "receipt_sha256": "14e94018b6c436ad45eaa5eb3507a09388fdf8a8da35e1b0e783097cf2266bc7", "reproduced_bodies": 95, "status": "PASS"}


## 2026-10-19 packed bank metadata through native factory and read

NativePackedFactoryOwner_test.cpp/gate_native_bank_packed_factory_owner.py passes24 reads/888bytes and50 defect controls. Actual entry index3 selects two15-bit fields at bit30, spanning words; nonzero bias produces size37 and offset0x313. Actual packed decoder and literal masks, bank accessors/factory, queued wait, worker IO/APC/completion, consumer queries and counted destruction compose without runtime metadata fallback. Direct request FilePos/Size, payload, lock/state and guards checked. Getter101B and mask table132B match original and final linked bytes. Controlled bank initialization remains; no metadata parser or wholebank startup claim. No new root credit, production edits, Lua, game/save/Git actions or full gate_all/dashboard. Earlier accepted evidence retains frozen provenance.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 50, "cross_word_metadata_reads": 48, "entry_releases": 24, "fresh_providers": 83, "input_pins": 370, "linked_bodies": 87, "linked_fixups": 370, "native_entries": 24, "object_pins": 83, "omissions": 71, "packed_entries": 24, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-packed_factory-owner\\00\\9d\\009d6060\\20261019T193821902489Z\\receipt.json", "receipt_sha256": "b9535d371c632c7ca0f806ec7a707a345d95a027f5a21391658306127de55cd3", "reproduced_bodies": 95, "status": "PASS"}


## 2026-10-19 native bank constructor initialization order corrected

CBankFileAsync009D5F80 now clears NonCached/DisableAsync/LastRestartingData in the
constructor body after disposal-list header allocation. Previous initializer-list
placement cleared LastRestartingData before that allocation. Original224B and6 named
relocations match on the first evidence-directed body spelling. Base constructor402B
and five actual helpers (packed16B,wide15B,char15B,cache73B,base9B) also match exactly.
Original/rebuilt72 cases cover fills, receiver offsets and wrapped string counters;
full64KiB arena snapshots at every malloc/critical-section boundary and final state,
globals, ABI registers/stack, independent header links/defaults/guards checked.
Ten controls reject defaults/counter faults and the early restart assignment. Explicit
prior-source control also rejected in the candidate gate. malloc and critical-section
initialization are deterministic emulation boundaries; no full host startup claim.

Existing candidate-linked host fixture passes actual Windows critical-section and
async headers/list checks, requires candidate omission to fail, and final224B/6fixups
are verified. Its base/unused virtual/lifetime methods remain controlled boundaries.
All six vtables/120slots remain correct. No headers changed. Only009D5F80 catalog,
manifest grade and artifact row updated (NEAR_MATCH to RELOCATION_MATCH), originals
archived. No new root credit, full gate_all/dashboard, Lua, game/save or Git changes.

{"cases": 72, "controls": 10, "host_receipt": "D:\\Documents\\FableTLC\\work\\native-recreation\\loading\\bank-initialization\\20261019\\host-194548\\receipt.json", "host_receipt_sha256": "8b4e14befb79e47dedcd5600fce25896c145ca6ac424b0fa2232cb88a7eedb77", "new_roots": 0, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-initialization\\00\\9d\\009d5f80\\20261019T194416440534Z\\receipt.json", "receipt_sha256": "f99857c8a5e996b92b12acb77f073cb99213522697fc4fe52ba2dca58cebc099", "roots": 7, "slots": 120, "vtables": 6}

Next: native host construction using the actual base/helper chain with guarded allocation observations, then integration with factory/read. Existing worker fixtures still control whole-bank initialization.


## 2026-10-19 actual bank initialization host chain

NativeBankInitializationHost_test.cpp/gate_native_bank_initialization_host.py passes
24 cases,12 actual Windows recursive critical sections,7 missing-provider link
rejections and10 runtime defect controls. Real bank/base plus all five helper
constructors execute. Guarded allocator records require LastRestartingData and both
async flags still contain fill at every allocation, including disposal-list header;
checks final defaults, packed arrays, all tree/list links, allocation/object guards
and wrapped string counters. All7 original bodies and final linked targets verified.
Instruction-preserving malloc alias observes allocations. Unused virtual/destructor/
tree-erase aliases abort if reached. Raw fixture cleanup frees empty headers, so
native whole-bank destruction/startup parser remain outside this proof. Failed SDK
include attempt, missing cold destructor boundaries and buffered-failure-output
attempt retained; final failure reporter flushes stderr before ExitProcess.

{"cases": 24, "controls": 10, "linked_fixups": 24, "new_roots": 0, "omissions": 7, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-initialization-host\\00\\9d\\009d5f80\\20261019T195104290119Z\\receipt.json", "receipt_sha256": "e5707fae97f624b9805d64f2eaa2b2bc49563d1d23ce457f2cdba264ab41fcc6", "roots": 7, "windows_locks": 12}

Next: replace controlled bank initialization in packed factory/read fixture with actual constructor chain. No new root credit, full gate_all/dashboard, Lua/game/save/Git changes.


## 2026-10-19 actual bank initialization through packed factory/read

NativeInitializedFactoryOwner_test.cpp/gate_native_bank_initialized_factory_owner.py passes24 reads/888bytes,24 actual bank constructors/168 guarded initialization allocations and60 defect controls. Real base/async and five helper constructors initialize seven map/tree/list headers and Windows critical section before packed metadata configuration, factory, native queued wait, worker IO/APC/completion, consumer queries and counted entry/data teardown. Every constructor matches original bytes and final linked targets. Initial allocator observations require original late restart/flag stores. Initialization defaults and packed fields checked before configuration, then all existing read/lifetime checks run.

Fixture still supplies owned file and packed metadata and manually releases seven empty bank headers plus critical section after native entry/data lifetimes. Native bank destructor and file/header parsing remain excluded. The inherited packed-phase sentence about controlled bank initialization in this receipt is superseded by its final actual-construction paragraph; file/metadata setup remains controlled. No wholebank startup/new-root credit claim, full gate_all/dashboard, Lua/game/save/Git actions.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 60, "cross_word_metadata_reads": 48, "entry_releases": 24, "fresh_providers": 89, "initialization_allocations": 168, "input_pins": 379, "linked_bodies": 94, "linked_fixups": 394, "native_bank_constructions": 24, "native_entries": 24, "object_pins": 89, "omissions": 76, "packed_entries": 24, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-initialized_factory-owner\\00\\9d\\009d6060\\20261019T195335980689Z\\receipt.json", "receipt_sha256": "5d818fa5d907db6ce07270679fd6d437f7b5080e0a43a1f6d5932a7f13e6c099", "reproduced_bodies": 102, "status": "PASS"}

Next: native bank destructor and its member cleanup dependencies, initially empty-bank construction/destruction before composing read ownership.


## 2026-10-19 actual empty bank construction and destruction

NativeBankLifetimeHost_test.cpp/gate_native_bank_lifetime_host.py passes24 native cases with12 real Windows locks,28 original/final-linked body checks,24 link-or-runtime provider omission rejections and16 defect controls. Both base and async banks use their actual constructor/destructor/member chains. Native frees release four or seven guarded headers in reverse order; async critical section is unlocked and deleted before header reclamation. String counters restore their wrapped seeds, packed defaults and allocation/object guards pass, and native CloseBankFileImpl clears seeded logical Size17 with empty containers.

All20 added cleanup roots match original bytes and named targets, as does the7B base destructor. Three empty-tree erasers remain linked cold; missing providers pass only for those three. Lock retry is linked but not exercised. Inactive populated-bank Abort/Unregister/Save/string-edit helpers and unused virtual slots are explicit fail-if-called aliases. No populated bank teardown, header/parser, active-bank destruction or factory/read composition claim yet. No production edit/new-root credit, full gate_all/dashboard, Lua/game/save/Git changes. Scratch missing cold dependencies and initial missing base-vtable map entry retained.

{"cases": 24, "cold_omissions": 3, "controls": 16, "linked_fixups": 123, "new_roots": 0, "omissions": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-lifetime-host\\00\\9d\\009d5f80\\20261019T200044734877Z\\receipt.json", "receipt_sha256": "d5d499be3dc12ed0279011dd31c084c2191c2d463d4317572e053282591a3a2a", "roots": 28, "windows_locks": 12}

Next: native bank destructor after initialized packed factory/read, with guarded bank-owned runtime/packed metadata allocations so native cleanup can reclaim them.


## 2026-10-19 native bank lifetime through packed factory/read

Actual bank construction, packed entry creation, queued consumer wait, Windows read/APC, counted data and entry teardown, then native whole-bank destruction pass 24 processes with 66 defect controls. All 168 initialization headers and 72 owned metadata allocations are reclaimed; string counters and lock lifetimes checked. Native runtime vector and packed metadata cleanup occurs between async and base header frees. 104 fresh providers reproduce 118 bodies, with 110 linked bodies and 486 linked fixups audited. 82 omissions fail linking; six additional required-provider omissions link through cold aliases but fail the executed path as required.

Receipt inherited boundary prose is historical: its final lifetime paragraph supersedes earlier manually-freed-header and controlled-initialization wording. Actual trace has eleven bank enters/thirteen leaves/two tries. File handle and metadata construction remain fixture-controlled; this does not cover file/header parsing, populated tree erase or active-owner whole-bank teardown. No new-root credit, Lua/game/save/Git action or full gate_all.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 66, "cross_word_metadata_reads": 48, "entry_releases": 24, "fresh_providers": 104, "initialization_allocations": 168, "initialization_header_frees": 168, "input_pins": 398, "linked_bodies": 110, "linked_fixups": 486, "metadata_frees": 72, "native_bank_constructions": 24, "native_bank_destructions": 24, "native_entries": 24, "object_pins": 104, "omissions": 82, "packed_entries": 24, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-lifetime_factory-owner\\00\\9d\\009d6060\\20261019T200617749874Z\\receipt.json", "receipt_sha256": "37c008cf4b4df204808925c7e193daf5d174a4a44a04af2785e5bccde6c062ae", "reproduced_bodies": 118, "runtime_omissions": 6, "status": "PASS"}

Next: inspect native threaded-file construction/open/close dependencies before composing ownership into the bank read fixture.

Lifetime factory gate registered in gate_all with its exact 66-control success marker; Python syntax checked without running the full suite. Initial threaded-file dependency inspection: read Open uses path conversion/splitting and char-string uppercase helpers; current constructor is a raw adapter with hard-coded vtable and opaque base/member calls. Existing open-write-strings gate explicitly excludes file construction and read-only helpers. Next work must establish typed constructor equivalence and real helper targets before composing file ownership.


## Native file close-stream recovery scratch

Corrected dependency finding: typed CThreadedFile constructor already exists in CThreadedFile_Constructor_0098dfd0.cpp with original parity; similarly named CThreadedFile_CThreadedFile_0098dfd0.cpp is an older adapter. No duplicate reconstruction needed. Actual missing composition dependency is manager CloseAllStreamsForFile0098e7d0: existing read-path host fixture supplies it. Scratch ordinary C++ now passes360 original/rebuilt emulated cases covering empty/nonmatching/single requests, pending callbacks retaining/removing their own UID, finished and all three active states with1/3 waits, full16KiB arena, ABI, lock and processing-block counter, deletion order. Four source mutants detected (wrong file, missing block, successful cancellation, stale callback iterator). Candidate925B vs original840B; no canonical promotion or new-root credit. Vector/tree/Win32/deallocation remain controlled boundaries. Evidence and pins: work/native-recreation/loading/file-close-streams/20261019/checkpoint-2140.json. Next multi-ID and disappearing-wait requests before native helper/host composition.


## Threaded-file ownership scratch composition

CloseAllStreams behavioral scratch now passes720 single-request cases (including disappearance during waits) plus280 cases spanning every file-selection subset of2/3/7 finished UID nodes. Five source mutants rejected. The actual ulong-vector growth provider UlongVector_Overflow_00464aab.cpp independently matches188 original bytes/four relocations; evidence vector-2148/receipt.json. Next replace the vector boundary in emulation and exercise its real allocation/copy behavior.

NativeFileBankLifetime_probe.cpp composes typed file constructor, real bank/read/APC/entry teardown, file destructor and bank destructor in24 Windows processes. Initial integration exposed fixture shutdown ordering: native manager Uninitialise deletes its critical section, so file close must precede manager shutdown. Fixed fixture order; production sources unchanged. Three host mutants rejected: missing manager cleanup, missing CloseHandle, bad constructor handle. Final linked constructor41B/3fixups, ordinary destructor67B/6fixups and deleting destructor88B/7fixups exactly match originals; selecting the Close provider first avoids the shorter ctor-COMDAT deleting wrapper. Scratch cleanup925B/19fixups also verified in final PE, but does not match original840B; no canonical promotion/new-root credit. Host exercises empty map after retirement, with actual critical-section entry/leave and handle closure. Vector growth is still cold-trapped in that host fixture. Failed builds/diagnostics retained.

Host receipt work\native-recreation\loading\file-close-streams\20261019\host-214722543421\receipt.json SHA256 f0b234b3d2c347918caead7969ee56fabf2c9882251fcec73b306b6307bdece0; independent-verification.json and close-streams-linked-verification.json contain audits. Main scratch drivers compare.py,multiple.py,controls.py,host.py,verify_host.py under work\native-recreation\loading\file-close-streams\20261019.


## File cleanup typed scratch and host cancellation/wait expansion

Candidate typed.cpp uses canonical headers with no shared-header edits or pending-helper rename;883B vs original840B. It passes720 differential single-request cases,280 multi-node differential cases executing exact188B vector growth,572B tree erase,64B rotations and26B extrema. Native red-black invariants independently checked for every file-selection subset of2/3/7-node trees. Earlier925B candidate retained.

Typed candidate also passes24 real bank/file read lifetime processes; exact constructor/destructor/vector final-link audits and3 host controls at host-215239411801. Dedicated pending host passes24 cases with real Win32 critical section, native pending/UID tree erase and vector allocation, callback retention/self-removal, mixed-file filtering and priorities1/6;4 host mutants rejected, final883B/18fixups audited (pending-host-215511623728). Controlled-worker active host passes48 cases over locked/reading/completed states, finish/removal transitions and actual event/SleepEx/lock handoff; missing processing block and missing wakeup rejected (pending-host-215636895230). Its original receipt inherited a no-worker phrase; independent-verification.json correctly distinguishes the controlled worker from the production read worker. Reentrant host passes32 cases including callback removal of another snapshotted UID and insertion of a new same-file UID: the new UID survives the original snapshot (pending-host-215804613024). Reentrant case currently candidate-host-only, not an additional original differential claim.

Frozen checkpoint work\native-recreation\loading\file-close-streams\20261019\checkpoint-215828331391.json. No canonical source/header promotion, new-root credit, full gate_all, Lua/game/save/Git/lease actions. Next finish exact/near-match acceptance evidence and reproducible fresh gate before any source promotion.


## Canonical CloseAllStreamsForFile near-match accepted

Added rebuild/src/compiled/00/98/CThreadedFileManager_CloseAllStreamsForFile_0098e7d0.cpp, focused gate_native_threaded_close_streams.py, two Python models and three Windows fixtures. Fresh canonical gate passes720 single-request differential cases,280 multi-request differential cases with six exact native container helpers,24 pending/48 controlled-active/32 reentrant Windows cases,5 fault controls and missing-root rejection. Root840B differs from original only at offsets493..500 (cursor compare register/scheduling); explicitly NEAR_MATCH, not exact. Initial UID searches inline, callback search uses real shared find; snapshot IDs preserve new same-file requests inserted by callback. All source/header/compiler/input/object hashes verified.

Receipt D:\Documents\FableTLC\rebuild\build\native-threaded-close-streams\00\98\0098e7d0\20261019T220604551406Z\receipt.json SHA256 edad0636f08e7d5e175a54773066479fb0577354db1f3349c6681ef4c636f8a7. {"controls": ["wrong-file", "no-block", "success-cancel", "truncated-ids", "no-wakeup"], "grade": "NEAR_MATCH", "host": [{"cases": 24, "exact_helpers": 6, "exe_sha256": "43e390131480c6fa2ecfe45c47e911323e72964ce84be75c2602eed67d373c8b", "fixture": "NativeThreadedCloseStreamsPending_test", "root_bytes": 840, "root_fixups": 19}, {"cases": 48, "exact_helpers": 6, "exe_sha256": "6c6ce81366101795206f563d4e71027e58000827ce0d2d5dfeea961a3f00e92c", "fixture": "NativeThreadedCloseStreamsActive_test", "root_bytes": 840, "root_fixups": 19}, {"cases": 32, "exact_helpers": 6, "exe_sha256": "1ea0aa063f95202a1818c44431d2f0c7a354cef4f2c338a11e22e022b9a8fb57", "fixture": "NativeThreadedCloseStreamsReentrant_test", "root_bytes": 840, "root_fixups": 19}], "missing_root_rejected": true, "multiple_cases": 280, "new_roots": 0, "original_differing_offsets": [493, 494, 495, 496, 497, 498, 499, 500], "receipt_sha256": "edad0636f08e7d5e175a54773066479fb0577354db1f3349c6681ef4c636f8a7", "root_bytes": 840, "similarity": {"built_instructions": 290, "complete_decode": true, "instruction_delta": 0.0, "near_match": true, "opcode_similarity": 0.996551724137931, "retail_instructions": 290}, "single_cases": 720, "status": "PASS"}

Only0098e7d0 manifest row corrected from historical ProcessPendingRequests label to verified CloseAllStreamsForFile; artifact row added. Typed symbol catalog and gate_all registration added; dedicated symbol_index.one verified one strong symbol and appended exactly one row, preserving every pre-existing symbol-index byte (index-220756412548/receipt.json). No shared-header edits, full gate_all/dashboard, game/save/Lua/Git or lease actions; no additional root credit claimed. Next compose canonical file ownership/cleanup into fresh bank/read gate, replacing fixture-owned handle lifetime while retaining explicit controlled file opening until native Open integration.


## 2026-10-19 native file and bank read lifetime

Fresh file constructor, bank construction, packed factory, queued wait, Windows read/APC and native owner teardown pass 24 processes. Actual file Close runs before manager shutdown, repeated Close takes its invalid-handle path, then file and bank destructors restore ownership counters. All 69 defect controls and six runtime omissions fail as required; 85 required omissions fail linking. Independent verification checked all 404 input, 108 object and 75 Python provenance hashes, final executable and positive/control run records. CloseAllStreams remains NEAR_MATCH (840 bytes; eight differing scheduling bytes). No new root credit.

Native Open and bank header parsing remain fixture-controlled. Next inspect native read-open composition and its filesystem/string dependencies. Full gate_all not run; no cleanup, game, save, source-Git or lease action.

{"bank_fixups": 96, "blocking_queue_moves": 24, "controls": 69, "cross_word_metadata_reads": 48, "entry_releases": 24, "fresh_providers": 108, "initialization_allocations": 168, "initialization_header_frees": 168, "input_pins": 404, "linked_bodies": 117, "linked_fixups": 529, "metadata_frees": 72, "native_bank_constructions": 24, "native_bank_destructions": 24, "native_entries": 24, "object_pins": 108, "omissions": 85, "packed_entries": 24, "real_bytes": 888, "real_reads": 24, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-file_lifetime_factory-owner\\00\\9d\\009d6060\\20261019T221038607481Z\\receipt.json", "receipt_sha256": "c5e5e4ef2b3b6171ee8b54e6fc00a3e35cfcbf348045d19c329f89f000c7daec", "reproduced_bodies": 125, "runtime_omissions": 6, "status": "PASS"}


## Read-open whole-body boundary coverage

Added focused whole-body differential gate: 1296 original/rebuilt cases, eight semantic fault controls, full 20KiB state, preserved registers/stack, raw caching bools, failure/null/valid handles, drive classification, filename-before-size and cleanup order, mutations and counter wrap. Read Open remains exact310B. Registered gate; no new root credit. {"receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-file-read-open\\00\\98\\0098e1e0\\20261019T221543149706Z\\receipt.json", "sha256": "732a5d09d16cf2929c84401204edadde91e3c55a28d87712a16806d4db852d0e", "cases": 1296, "controls": 8, "inputs": 179}

Scratch Windows probe also passed two cached/uncached reads and two absent-file failures with controlled string/path helpers. Existing native-file-read-paths receipt20261019T021303326518Z already covers actual native path/string dependencies and OS opens; the new model complements its ownership tests rather than replacing them. Next reuse that dependency set in the bank/file worker fixture.


## Native path/open through bank worker lifetime

Fresh native read-open with path conversion and actual string helpers now supplies the bank worker handle and owned filename. 24 processes read888bytes through native packed factory, queued wait and APC, then actual file close/destructor and whole-bank destruction. Final executable verifies151 bodies/706 named relocation operands. All75 controls detected; ten added path-provider omissions require link failure or observed runtime rejection. All providers freshly compiled and compared against pinned original-classified records. No new-root credit.

Process current-directory cache owns one global wide string until CRT shutdown; fixture explicitly clears cached storage and checks all tracked string blocks reclaimed before returning. Native D: drive classification selects device1, so this fixture now exercises the DVD queue. File actual size is0x9000, correcting the older fixture-controlled0x3000 assumption. Near-match grades remain uppercase277B versus278B and CloseAllStreams840B with8 differing bytes. Metadata construction and installer boundaries remain controlled. No full gate_all, game/save/Git/cleanup/lease actions.

{"bodies": 151, "controls": 75, "fixups": 706, "inputs": 571, "omissions": 107, "processes": 24, "providers": 132, "receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-open-file-lifetime\\00\\98\\0098e1e0\\20261019T222600868167Z\\acceptance.json", "redundant": 24, "sha256": "40248f03b6da4ab30db37a22a0ad4776b97db87914f8843a2c4b77eb687d07b2", "status": "PASS"}

Next inspect native bank-open wrapper and header loading dependencies for composition; preserve all failed/successful scratch and canonical builds.


## Base read-open recovery in scratch

CBankFile::OpenFileReadOnly009D1090 reconstructed as204byte exact match with9relocations,432 full-state/ABI original-rebuilt cases and9 semantic fault controls. Scratch declaration ReadFileInformation(bool) plus inline empty CAFile constructor reproduces retail construction. Production headers untouched. This wrapper does not check virtual disk Open result; header parser determines success; failure closes, success invokes virtual NotifyBankOpened.

{"address": "009d1090", "boundary": "Only scratch header adds private ReadFileInformation declaration. No canonical header/source/metadata promotion. Path, allocation, disk virtual open, counted reset, string assignment, header parser, close and notification controlled. Null allocation/exception behavior outside proof.", "bytes": 204, "cases": 432, "controls": 9, "grade": "MATCH", "header_sha256": "8851b184bfe19b8d5836e4ad923786fa6e39adce7d640c1a39ce64f7520644c5", "object": "work\\native-recreation\\loading\\bank-open-read\\20261019\\probe-222830701983\\root.obj", "object_sha256": "f9533f1023b5b62647f0a40f9e4ff6092938ce268f900feec35ceeac530d29fe", "source_sha256": "6aa849b77f915b8abaddd1157ec697c93250a20ed4ffe36f318e7aec54ceba88", "status": "PASS_SCRATCH"}

Next promote through a fresh pinned focused gate only after reconciling the additive header declaration with bank-open build snapshots: current bank-open builder copies old headers then requires exact equality with current include closure. Fresh provider byte comparison allows refreshing copied headers safely, but preserve old receipts as historical. Do not silently overwrite their pins.


## Base bank read-open promoted; integration refreshed

Canonical204B exact CBankFile::OpenFileReadOnly009D1090 passes432 full-state/ABI cases and9 controls. Added private ReadFileInformation(bool) declaration only; no data-layout change. Registered focused gate/catalog/one strong symbol and exact manifest/artifact rows. Catalog compilation independently confirms only that selected root.

Bank/open integration rebuilt with refreshed current headers. Every pre-existing named provider section still reproduces accepted bytes/relocations (anonymous namespace source hashes normalized; debug sections excluded), all24reads/75controls/151linked bodies/706fixups pass again. Prior receipts retained as historical; no retroactive pin changes.

[{"receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-base-open-read\\00\\9d\\009d1090\\20261019T223058448533Z\\receipt.json", "sha256": "ae6267afc636d35a40a0911bdeb4c756074e88840a6b27bddaa0bb45ff5c6c68"}, {"receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-bank-open-file-lifetime\\00\\98\\0098e1e0\\20261019T223121838870Z\\acceptance.json", "sha256": "d0f10772e5cfdb050d5d563b6255554c8957ba3a245115a34a71cec042f64514"}]

Next recover typed disk-file counted Reset dependency or inspect actual ReadFileInformation parser before base-open composition. No claim that header parser is implemented.


## Typed disk counted ownership accepted

CCountedPointer<CDiskFileWin32>::Reset009D1EE0 (103B) and DeleteData009A9030 (11B) exactly reproduce original and final-linked bytes.144 reset snapshots with full32KiB event/memory/ABI checks include callback replacement of owner fields, refcount wrap and failed allocation;256 deletion cases check4KiB state and virtual call flag1. Windows host36cases, six runtime mutations plus three model controls, missing-root rejection pass. Source/model/host Python provenance frozen.

Typed strong Reset and COMDAT callback bindings indexed. Historical generic metadata names retained because these machine bodies can serve merged generic instantiations; no exclusive CIPhysics-to-disk identity rename. Old gapscan DeleteIfSet adapter and old evidence retained. Artifact row009A9030 now points to typed proof, with prior row archived. No new-root credit or full gate_all.

{"receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-disk-counted-reset\\00\\9d\\009d1ee0\\20261019T223725751989Z\\receipt.json", "sha256": "3f3e6675f0b402096ca472c0205ca517f2e308098aa0e88f17c96160593ca71c", "reset_cases": 144, "delete_cases": 256, "host_cases": 36, "controls": 9, "linked_bodies": 2, "linked_fixups": 3}

Next compose base/async bank-open wrappers with native disk lifecycle and counted ownership. ReadFileInformation remains an unrecovered boundary; do not claim header parsing yet.


## Native disk Open exact evidence and Windows branch coverage

Existing CDiskFileWin32::Open0099A6A0 source requires no implementation change:624code bytes+16jump-table bytes reproduce original exactly after51 independently named target/literal/local-label/IAT fixups. Exception descriptor names CBBBFileException.896 full16KiB state/ABI cases cover all access modes, invalid enums/null handles, sequential flags, ignored API failures and helper mutation; seven defect controls pass. Seven Windows cases cover read/readwrite/openalways/create, absent and empty paths with caller-caught type/message checks, and invalid enum. Six host controls return80 as required. Final linked640B/51fixups checked.

Strings, path validation and error-text helpers remain controlled; full exception unwinding/cleanup is not claimed. Adapted host retains every build-owned file, using an absent sibling rather than deletion. Initial missing-open-flag control exposed old fixture accumulating failures and cascading into an uncaught exception; tightened four successful-open observations to terminate at first error, then all six controls passed. Older failed probes retained. Metadata and existing typed index row verified; no new-root credit.

{"receipt": "D:\\Documents\\FableTLC\\rebuild\\build\\native-disk-open\\00\\99\\0099a6a0\\20261019T224412928499Z\\receipt.json", "sha256": "54ece7661f5eff9ae5413a02838a5bf79a2c2d9b20c2ec170cd85427e1f18f3f", "model_cases": 896, "host_cases": 7, "controls": 13, "bytes": 640, "relocations": 51, "inputs": 189}

Next compose disk Open/Close and disk counted lifetime with actual base/async bank-open wrappers; ReadFileInformation remains controlled/unrecovered.


### Native base/async bank open through complete owned-file lifetime

Accepted `gate_native_bank_open_wrapper_lifetime.py`, receipt `rebuild/build/native-bank-open-wrapper-lifetime/00/9d/009d5820/20261019T230203991963Z/acceptance.json` SHA256 `cb0d57c1a6574258d063551d50d5735e693f01196b6f32d0ea14c2956d04a27a`. All 143 providers compiled fresh; 24 processes/24 reads/888 bytes, 169 unique linked bodies, 798 instruction fixups and 16 disk vtable slots audited. All 720 inputs, object hashes, executable and Python provenance copies independently verified. 115 provider omissions reject linking; 27 redundant providers remain explicitly recorded. Required redundant providers also face runtime failure checks.

The native base bank Open and async wrapper now create and own both actual disk and threaded files using native counted reset/deletion callbacks. Native disk Open, worker read, explicit threaded Close plus repeated-close path, counted release, manager shutdown and bank-owned disk destruction pass. Constructor state is observed before Open overwrites it; both underlying handles are confirmed closed and tracked allocations balanced. The temporary notification observer forwards the native notification and restores the native bank vtable.

87 observable defect controls pass. A retained 88th experiment (`read-open-result`) correctly remains neutral: native CBankFileAsync ignores threaded Open's return value. This is no longer counted as defect detection; standalone Open validation covers its return contract. Failed scratch experiment and resumed verification remain preserved. The canonical gate independently reruns every experiment from scratch.

Header decoding remains controlled at ReadFileInformation, and packed metadata is fixture constructed. Inactive disk virtual slots and write/error-only dependencies are explicit fail-fast boundaries. Prior near-match grades (CloseAllStreams and uppercase) remain; this composition adds no root count. Focused gate registered in gate_all; full gate_all was not run. Next: recover native header decoding and its dependency closure, without Lua or game/save changes.


### Native bank-open wrappers and owned file lifetime

Accepted gate_native_bank_open_wrapper_lifetime.py; receipt rebuild/build/native-bank-open-wrapper-lifetime/00/9d/009d5820/20261019T230203991963Z/acceptance.json, SHA256 cb0d57c1a6574258d063551d50d5735e693f01196b6f32d0ea14c2956d04a27a. All 143 providers compiled fresh; 24 processes/24 reads/888 bytes; 169 unique linked bodies, 798 instruction fixups and 16 disk vtable slots. Independently verified all 720 inputs, objects, executable and Python provenance copies. 115 omitted providers fail linking; 27 redundant providers recorded, with required providers additionally checked for runtime rejection.

Native base/async Open creates and owns actual disk and threaded files through counted pointers. Constructor-state observations precede Open; native disk Open, worker read, threaded Close/repeated Close, counted deletion, manager shutdown and bank disk destruction pass. Both handles confirmed closed and allocations balanced. Notification observer forwards actual native method and restores native vtable.

87 defect controls pass. The inherited read-open-result experiment is explicitly neutral: native async wrapper ignores threaded Open's return. Standalone Open gate covers its return contract. Failed scratch experiment retained; canonical gate independently reran all 88 experiments. Header decoding and packed metadata remain controlled; inactive disk slots/write/error dependencies are fail-fast boundaries. Existing near-match grades retained, no new root credit. Focused gate registered; full gate_all not run. Next recover ReadFileInformation dependencies. No Lua or installed game/save changes.


### Header recovery scratch, not promoted

Original ReadFileInformation009D0880 body is1635 bytes (1648-byte manifest span includes13 CC padding),61 calls. Scratch inventory/disassembly and original-only216-case observations retained under work/native-recreation/loading/bank-header/20261019. First typed C++ candidate1611 bytes passes216 nonthrow differential cases at probe-231127752532. This is partial behavioral evidence, not byte parity or accepted parser. Wider nonthrow matrix in progress; exceptions, helper mutation and real buffered stream integration still pending.

Important parameter correction: the bool controls throwing on header errors, not reading symbols. Existing declaration parameter name readSymbols and old fixture local name are provisional/misleading; no ABI or caller-expression change is required. Original magic BBBB, bank format, data format, update-state, alignment and three subsequent dwords were traced. Writable alignment mismatch reports and returns true early; recovery helper failure reports but returns true. Read-only alignment mismatch updates Alignment. HeaderSize receives stream position32 and FileSize receives virtual GetLength before state routing. Exception RTTI at01369470 identifies CBankFileException : CExceptionBase, size4;009D1E70 is its message/type constructor, despite historical unrelated generic label. Enum and progress-interface names in scratch are provisional.


Header scratch expansion: probe-231229966858 candidate1611B passes1512 nonthrow differential cases (magic/empty stream/bank and data versions/update states/alignment/stream chunking/entry and recovery results/callback presence), plus30 throw-boundary comparisons of message/type/state/helper order. Eight original-byte oracle-sensitivity controls detected. Throw tests intentionally stop at CxxThrowException; no unwinding claim. Production parser still unmodified. Candidate initially used inferred error text ", is "; original literal proved " not ", corrected before accepted comparisons. Current scratch is exploring signature byte-comparison structure to reduce1635-versus1611 body mismatch; prior accepted scratch object preserved.


### Native buffered input stream dependency accepted

Receipt rebuild/build/native-file-input-stream-lifetime/00/99/00994700/20261019T232646452985Z/receipt.json SHA256 028c0fea8abcd73d243585c4339aa239ffc64338aa9bc7094aa0ca9b32781941. Existing native sources freshly compiled:13 original bodies exact,8 active final bodies exact,5 separate helper copies discarded because callers contain expansions.24 native host cases,78 backing reads,90 seeks,24 allocations and24 frees;6 defect controls;8 required-provider link omissions and one redundant source-chunk provider. All 242 input pins,10objects,executable and Python provenance copies independently verified. Focused gate registered; full gate_all not run. Backing CAFile and base-class lifetime controlled; unused virtuals fail-fast. No disk/header claim and no new root credit.

Header scratch1611B candidate additionally passed648 helper-mutation comparisons (GetLength, recovery, entry-reader writes retained). Current ReadMagic variants1615B/1677B remain experiments; accepted partial scratch reference remains probe-231229966858 with1512 nonthrow+30throw+648mutation cases. Eight original-byte oracle sensitivity controls. No production header implementation promoted. Next compose tested parser candidate with native file-input dependency, then finish byte/classification and error-path review.


### Header plus native input stream scratch

work/native-recreation/loading/bank-header-stream/20261019/host-232854588483 passes64 native host cases,64 allocations/frees and64 backing reads,11 parser defect controls,9 required link omissions. Eight active native stream bodies verified exact in final executable;1611B header candidate final body matches the bounded differential reference. File positions, window cleanup, entry-reader arguments, recovery and log callback counts checked. Backing file/string helpers/entry decoding/recovery remain controlled; no production parser promotion.

Donor evidence captured in bank-header/20261019/donor-types.json and donor-locals-progress.json resolves names: enable_exceptions, ReadEntryList(CFileDataInputStream&,max_index,entry_count), ReadRecoveryFile(validate); CBankFile::CProgressCallback slot2 NotifyBankLogMessage, with five virtual slots and no virtual destructor. CBankFileException::EBankFileExceptionType values BANK_IS_CORRUPT0, BANK_IS_INCORRECT_VERSION1, ENTRY_NOT_FOUND2, ENTRY_IS_CORRUPT3; Type at0, size4 and CExceptionBase base. Scratch probe-233148288880 uses verified names; header body byte-identical to tested1611B candidate with only constructor relocation decoration changed to same original address. The exception constructor at009D1E70 is33B exact. Donor parameter names confirm bool means enable exceptions; old production declaration name remains untouched pending deliberate header promotion/consumer refresh.


### Native bank exception constructor promoted

rebuild/build/native-bank-file-exception/00/9d/009d1e70/20261019T233638654689Z/receipt.json SHA256 ec428819e82b616864917d6e7c06d293fc8f3bfab25215d6d23bad2bc9ff36d3. CBankFileException constructor009D1E70 exact33B/two fixups;120 full4KiB state/argument/ABI differential cases,20 native host cases,three oracle perturbations+two native source controls. Conversion can mutate incoming type or object; destructor sees assigned Type and may mutate it, preserved on return; guarded object and by-value storage disposal checked. Final linked bytes verified, missing root rejected, all 227 input pins/object/executable/provenance hashes independently verified. Strings controlled; no throw/unwind claim.

Added fable_bank_file_exception.h with donor-verified enum and class/base layout, typed native source, model/fixture/focused gate, catalog profile and one verified strong symbol row. Historical manifest CAIStateGroupBase name/prototype retained; only compiled status/source updated. Artifact row absent previously, now added with typed class. Interrupted metadata registration documented; prior target status fields known empty and reconstructed checkpoint explicitly labeled. No global index regeneration or unrelated row change. Header parser remains scratch; production CBankFile layout/declarations untouched.


### Header real-disk chain and recovery scratch

bank-header/20261019/probe-233925546800 uses the shared production CBankFileException type/constructor and a byte-comparison helper. Header candidate1592B (original1635) passes1512 nonthrow,30throw-boundary and648helper-mutation comparisons. bank-header-stream host-234025623103 revalidates64 host cases/11controls with the native exception constructor. No parser promotion or byte-match claim.

bank-header-disk/20261019/host-234530336673 adds native CDiskFileWin32 Read290B,SetPosition159B,GetPosition24B,Close52B, each original and final bytes exact.64 cases write/read a build-owned synthetic bank;64 stream allocations/frees and64 native disk closes; logical and actual Windows positions checked, handles confirmed closed.11 parser +3 disk controls pass;14 required-provider link omissions. Eight stream bodies and the33B exception ctor also audited;1592B parser tied to differential reference. Handle adoption, length/open observers, strings, entry-list and recovery remain controlled. No installed game mutation or deletion; artifacts retained.

Entry-list original span1984 contains1969B function plus15 CC padding. Recovery manifest span1280 crosses neighbors; actual ReadRecoveryFile009D0380 is679B ending009D0627. Bounded inventory explicitly excludes InitBlockOrder009D0630 and OpenReadOnly009D06F0. New recovery scratch691B candidate at bank-recovery/20261019/probe-235007318249 passes432 full8KiB state/normal cleanup/argument/nonvolatile-register/stack comparisons across existence, BREC magic, validation word, buffer windows, entry-result and helper mutations. Original ignores ReadEntryList's result and returns true after valid metadata. No exception unwinding or production recovery claim yet.

### Native recovery-path lifetime accepted (2026-10-19 local)

Receipt `rebuild/build/native-bank-recovery-path-lifetime/00/9c/009cbfe0/20261020T000207353570Z/acceptance.json` SHA256 `e9e9c316cddfeefa048182209d3b4cf5054ffa7093318eb1ec63b98236308a93`. Independently rechecked all 727 input pins, 144 objects, executable and Python provenance. Existing GetRecoveryFilePath009CBFE0 matches 207 original/final bytes and16 fixups. Fresh native composition passes144 path cases in24 processes,24 reads/888 bytes,170 linked bodies and830 total fixups. Three targeted controls detected; 116 provider omissions fail linking and 27 are redundant. Parent87 controls remain prior evidence, not rerun. Registered focused gate and exact-root evidence without new-root credit. DeleteRecoveryFile is discarded and never executed. Path metadata is restored before read/teardown. Packed metadata and bank parser remain controlled; existing near-match roots retain their grades.

Recovery reader scratch is691B versus original679B, with432 full-state differential cases and four original-byte sensitivity controls passing. Next integrate native stream/disk lifecycle with recovery parsing; entry-list loading remains controlled. Header/disk scratch passes64 cases with native read/seek/position/close,11 parser and3 disk controls; no parser exact-byte or exception-unwinding claim.

### Recovery reader promoted exact, native disk integration (2026-10-19 local)

`CBankFile::ReadRecoveryFile009D0380` now matches all679 original/final bytes and29 relocations (earlier commentary27 was incorrect). Source separates signature and validation failure checks, preserving original cleanup placement. Receipt `rebuild/build/native-bank-recovery/00/9d/009d0380/20261020T001143847495Z/receipt.json` SHA256 `ef244cb9885377df4ab35039ae8063b7336e2644b5a1cc35d895f1d3c0f8f279`. Fresh canonical gate passes432 full8KiB differential cases, four original-byte sensitivity controls,192 native host cases and nine runtime faults. Native cases include128-byte and16416-byte retained synthetic files, buffer crossing, zero/high-bit counts and validation words, absent paths, bad signatures/validation and entry-reader false results. Exact native stream8 and disk4 bodies verified in final PE;13 provider omissions reject linking. Entry-list false is deliberately ignored. Buffers and handles balance on all normal paths. Paths/existence/handle adoption/string helpers and entry-list decoding remain controlled; no exception-unwinding claim. All1921 input pins,15 objects, executable and Python provenance independently verified. Typed symbol/catalog/MATCH manifest/artifact rows registered, checkpoint preserved.

Bank header adds private recovery/entry-list declarations and corrects parameter name to enable_exceptions, with no layout changes. The initial dependent recovery-path rerun correctly stopped at frozen-header equality. Builder now records the bank-header refresh and recompiles/compares every existing provider body/relocation. Full fresh integration then passed170 linked bodies/830 fixups,144 paths/24 processes and3 controls: `rebuild/build/native-bank-recovery-path-lifetime/00/9c/009cbfe0/20261020T001219980117Z/acceptance.json` SHA256 `0b5a70ec12b435ef97cc6371aa42d99fc94fbecb8e9bd93b9a314b49eb902c3e`. All pins/objects/executable/provenance independently rechecked. Old receipts remain retained.

Next compose native recovery path/open with the exact recovery reader, then recover entry-list dependencies. Native header parser remains scratch, despite existing64-case real-disk evidence. No Lua/game/save/cleanup operations.

### Owned recovery chain accepted (2026-10-19 local)

Fresh gate `gate_native_bank_recovery_owned.py` accepted `rebuild/build/native-bank-recovery-owned/00/9d/009d0380/20261020T001801870315Z/acceptance.json`, SHA256 `916c6f483fbc79c7cfdefd2bd4185d1abac4ef21da9db7d94729122b4d2023e6`. All157 linked providers compiled fresh;182 linked bodies and901 fixups verified.24 processes exercise216 recovery cases,192 buffer allocations/frees and normal file closes, plus24 threaded bank reads/888 bytes. Recovery path/existence/disk Open, buffered stream, Read/Seek/Position/Close and679B recovery reader are native. Entry-list decoding and main bank header parser remain controlled. Missing recovery file, bad magic/validation, ignored entry-reader false,16KiB buffer crossing and exclusive reopen after cleanup checked. The fixture retains all build-owned synthetic files. No deletion/Game/save actions.

Twelve runtime controls plus four original-byte sensitivity controls passed. Early scratch file-access control was neutral because the sequential-scan hint does not change read contents; retained evidence exposed that test gap. A forwarding CreateFileW observer now checks native access/share/disposition/flags while preserving actual opens, and the control is detected. The enlarged payload detects wrong refill offsets. 125 provider omissions fail linking; 31 redundant objects recorded with required runtime failures checked. All2344 inputs,157 objects, executable and Python provenance independently rechecked. Existing near-match grades retained; no exception-unwinding or entry-list-decoding claim.

Next entry-list dependency: original009CEAE0 is called as bank buffer allocation with max_index+1, despite historical generic manifest name. Inspect native evidence and donor types before assigning its typed identity.

### Entry-list allocation dependency promoted (2026-10-19 local)

Original call009CEAE0 is typed `CBankFile::AllocateBuffers(unsigned long)` (confirmed donor method, original calls/fields); the historical generic manifest label remains unchanged. New source375B/12fixups exact, with runtime release009D3D9090B, symbol release009D3FB095B and runtime copy009D1C3074B also original/final exact. Explicit local runtime-record initialization preserves padding without changing the shared CRuntimeData type. Retail's class-element copy dispatch replaces the initial scratch POD erase mismatch. Existing standalone symbol-release source remains preserved.

Receipt `rebuild/build/native-bank-allocate/00/9c/009ceae0/20261020T002839680271Z/receipt.json` SHA256 `4a26befa4889c5f2d2e0b5fc35ce0065bc7fab02b148607a7ddf1334901ecffe`.3584 original/rebuilt12KiB differential cases include helper mutations/reloaded flags and map headers,192 native allocation cases,6 guarded nonempty/empty/self-copy cases,8 oracle perturbations and8 native source faults. Native root and release/copy helpers forward actual CRT free through an allocation observer; resize and string/tree erasure remain controlled. Missing root rejects linking. All1908 pins,objects,executable/provenance rechecked. Registered root/MATCH artifact and four verified symbols; helper manifest identities/grades unchanged. No allocation-failure/exception-unwinding claim.

Windows identified the scratch executable named update-flag.exe as requiring elevation before it ran; control executables now use numbered names, with no elevation requested. All controls subsequently passed in fresh retained builds.

The additive private AllocateBuffers declaration was checked with another fresh full recovery composition: `rebuild/build/native-bank-recovery-owned/00/9d/009d0380/20261020T002901638131Z/acceptance.json` SHA256 `cea4104be5559a1a84c991564eb97449d16fe6d12771e6678742fbad771bf053`;157providers/182bodies/901fixups,216recovery cases and16controls passed. Pins,objects,executable/provenance independently rechecked. Next recover native resizing/type-count-array and entry initialization dependencies before linking the entry-list parser.

### Native vector resize dependencies accepted; checksum tag regression restored (2026-10-19 local)

Three typed providers promoted: runtime-record resize009D401087B, update-pointer resize009D3DF087B and string resize0049B760101B; existing unsigned-long resize0046493162B reused without new-root credit. Historical generic manifest labels retained. Canonical gate passes400 full4KiB differential cases,200 guarded native cases,10 original-byte controls and10 stack-preserving final-PE controls. Six provider omissions reject linking. Nine final bodies exact, including runtime copy74B and full native unsigned-long vector chain (resize62/erase52/fill264/copy35/overflow188). Other fill operations and string lifetime/erasure remain controlled. Checksum host cases use spare capacity; the existing full unsigned-long gate covers allocation/growth. All1913 pins,7objects,executable/provenance verified.

During composition, fresh current checksum fill source emitted252B against its prior264B acceptance. The mismatch was unit-specific zero-initialized POD tag forwarding. A scoped FABLE_STLP_ZERO_TRUE_TAG opt-in and by-value CopyTagged forwarding restore all264 bytes; the default tag behavior remains unchanged. Source/header before snapshots and hashes retained in work/native-recreation/loading/bank-vector-resize/20261019/checksum-fix-checkpoint. Full existing gate reran4664cases/12554snapshots; its receipt was frozen as after-gate.json SHA25608e8b2461d09eae490a0051bd4f1d63d135364db69bf854a59b5ac6cda80ab1f. No unrelated source replacements. Initial final-PE skip-call control corrupted stack; corrected fill-call controls retain the12-byte callee stack cleanup, so all ten fail at the explicit host assertions. Earlier failed artifacts retained.

Default-profile regressions checked: allocation gate3584model/192host/6copy/16controls and full owned recovery157fresh providers/182bodies/901fixups/216cases/16controls both pass after the scoped header change. Wrapper builder records both bank and vector header refreshes, then compares every old provider body/relocation and final PE. All refreshed input/object/executable/provenance pins independently verified.

Receipts:
- native-bank-vector-resize: `rebuild/build/native-bank-vector-resize/00/9d/009d4010/20261020T004418643706Z/receipt.json` SHA256 `13cfa882385eee31b5d95841cf8b18202be12da91ac41f27589ec191223630b6`.
- native-bank-allocate: `rebuild/build/native-bank-allocate/00/9c/009ceae0/20261020T004453566863Z/receipt.json` SHA256 `845057dbe222961b29dba71456e7918a4e401d867a24536c7147555e5ff3c06a`.
- native-bank-recovery-owned: `rebuild/build/native-bank-recovery-owned/00/9d/009d0380/20261020T004535800613Z/acceptance.json` SHA256 `b85ddaea8f3d5a560d194aa165cdd4d5cf66ef710c8ed3d881aebc7314780e0a`.

Next recover remaining non-checksum fill insertion and entry-list type-count allocation/initialization. Type-count constructor attempts remain scratch, not accepted. Entry-list decoding and main bank header parser still controlled. No Lua/game/save/cleanup actions.

### Entry-list type-count array constructor accepted (2026-10-19 local)

Typed vector<CBankFileDataTypeCount> constructor009D2AF0 matches94 original/final bytes and one allocator relocation. Donor class hasDataType at0,Count at4,size8. ReadEntryList's CArray wrapper inlines around this vector constructor; earlier exploratory CArray constructor variants were the wrong emitted target and remain unpromoted. No shared-layout modification. Existing generic CSimpleMemoryPool manifest identity retained.

Receipt `rebuild/build/native-bank-type-count-array/00/9d/009d2af0/20261020T004925951644Z/receipt.json` SHA256 `647880c1a440bec29c20c19b2fd5ec995da6a3278a581ac397ce20efc415144b`.35 full12KiB original/rebuilt cases (zero through256 elements) include allocator mutation of object fields, incoming count and surrounding state.14 guarded native allocation cases check exact zero initialization, bounds, return identity and zero-count no-allocation. Five original-byte perturbations and five final-PE controls detect allocation width, both fields, stride and finish-pointer faults; missing root rejects linking. All226 inputs,objects,executable/provenance independently rechecked. Normal allocations only; no OOM/unwinding claim. Source,typed symbol,focused gate andMATCH artifact registered.

Next entry-list initialization/fill dependencies; no change to the accepted owned recovery/header-parser boundary.


Native update-pointer vector accepted: four exact bodies (fill31, growth216, insertion276, resize87), 3680 native/original replay cases, 10802 full16KiB snapshots, 12 fault controls and two provider omissions. CRT callbacks controlled; allocation failure/unwinding unclaimed. Existing generic fill manifest/source retained; two historical vector identities retained. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-update-vector\00\9d\009d3300\20261020T005654068238Z\receipt.json SHA256 192dc6ba04385fec64235b462b69f7880c960a18e05d609d0e466d7a30c74a88. No shared header change. Next runtime-data/string insertion and entry initialization; bank entry-list/header parser remain controlled.


Native runtime-data vector accepted: nine exact bodies including insertion311, growth219, resize87 and six copy/fill helpers; 3680 native/original replay cases, 7724 full16KiB snapshots, 16 fault controls and two provider omissions. Every record byte including padding compared. CRT callbacks controlled; allocation failure/unwinding unclaimed. Historical manifest identities retained; no shared header change. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-runtime-vector\00\9d\009d3a20\20261020T010010871324Z\receipt.json SHA256 bc0dffe26c4c0741a08bd1f707b53c1a78d64fbf245ba93b7cb158192e94c410. Next string insertion and entry initialization; entry-list/header parser remain controlled.


Native InitEntry009CE050 accepted302B/13fixups exact:1728 differential+1728 native host cases,16 controls,missing-root rejection and fixture helper ownership audit. Corrected FilenameIndices to scoped owned multimap (donor confirms multimap; retail insert_equal returns4B iterator). The initial oracle incorrectly wrote an8B map-style return into the original routine's4B iterator slot, overwriting a live string temporary. Correcting the oracle and scoped multimap type produced the exact body. Tree declaration/type correction preserves bank layout; full owned recovery182bodies/216cases, allocation3584+192+6cases and vector resizing400+200cases refreshed successfully. Helpers inside InitEntry remain controlled; duplicate-filename insertion semantics need native tree integration. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-entry\00\9c\009ce050\20261020T011045780561Z\receipt.json SHA256 bed593ee36773bdcf595342d058d4d37b961d9e7aadfb585777460f1d45b5f2a. Historical BankEntry_ProcessPath label retained. Next insert_equal009D34E0/native map helpers and existing string-vector insertion composition.


Native filename multimap accepted: insert_equal93B, node insertion149B, node allocation33B, pair copy27B plus existing comparison/balancing helpers (nine original/final exact bodies).1600 differential+1600 native guarded cases,16 fault controls,three required-provider omissions. Comparator omission links a different body and passes behavior, explicitly rejected by exact-body audit. Stable duplicate keys retain every payload; full128KiB original/rebuilt arenas compared, native allocations/refcounts reclaimed. String copy/destruction controlled. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-filename-tree\00\9d\009d34e0\20261020T012014436395Z\receipt.json SHA256 83a6044787aeb673a2f161b95175c1b906c3fb2fbd896d7684400d0bbd744660. Historical manifest identities and existing less/comparator/balancer grades preserved. Next compose native filename insertion with InitEntry; remaining entry-list/header parser controlled.


Native InitEntry through filename multimap accepted:1600 full128KiB original/rebuilt scenarios+1600 guarded native scenarios,10 original/final exact bodies,22 fault controls,four required-provider omissions and one redundant comparator rejected by exact-body audit. Empty filenames skipped; duplicate filenames retain stable payloads and tree/string ownership balances. Path conversion/lowercase and string copy/destruction remain controlled; symbol/CRC branches fail fast in this composition (covered in standalone InitEntry). Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-tree\00\9d\009ce050\20261020T012530453321Z\receipt.json SHA256 0a0ddc6f0ea83a8eca8ffe18e48b78131335f8a56cf1bb8951380fa740d30566. No new root credit. Next native CCharString ownership through this composition; entry-list/header parser remain controlled.


Native InitEntry filename tree with CCharString ownership accepted:13 original/final exact bodies including copy61B/3fixups,dtor12B/2fixups,Unassign87B/2fixups.1600 differential+1600 native guarded scenarios,28 fault controls,seven required-provider omissions and one redundant comparator rejected by exact-body audit. Native last-owner release frees25600 data blocks+25600 text buffers across host cases, keeps capacity top bit, clears pointer/length and returns global string count to zero. Native malloc/delete boundaries forward to guarded CRT storage; fixture-created anchors are explicit. Path conversion/lowercase still controlled; manual tree teardown uses native string destruction. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-owned\00\9d\009ce050\20261020T013058222276Z\receipt.json SHA256 a51898f8dbfa4d5ae353c30907abcd345f0510b7e4c39c5113f0703db62d2ae7. No new root credit. Next native lowercase/path conversion dependencies; entry-list/header parser remain controlled.


Native InitEntry filename lowercase and ownership accepted: 16 original/final exact bodies, 1600 differential plus 1600 native guarded scenarios, 30 fault controls, seven required-provider omissions and one redundant comparator rejected by exact-body audit. Native lowercase copy-on-write preserves mixed-case input anchors and normalizes duplicate keys while retaining stable payload ordering. Native string destruction releases all guarded anchor and clone allocations and restores the global instance count. Path conversion remains controlled; no allocation-failure or unwind claim. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-lower\00\9d\009ce050\20261020T014246848203Z\receipt.json SHA256 cafc1a6a899caafbb58db80138932ca2b6df9499721cee86f5e785eb26cb0e04. No new root credit. Next: native filename path conversion; entry-list/header parser remain controlled.


Native bank filename path conversion accepted: 25 exact original/final bodies, 241 differential plus 241 native guarded cases, 14 fault controls and five required-provider omissions. Wide/narrow conversion, split-path, extension append and native ownership preserve inputs and return both string counts to zero; all guarded allocations released. Drive, relative, UNC and slash paths, null/empty, high bytes, spaces and multiple extensions covered. CRT split-path is modeled in oracle and real in host; cold siblings fail fast, no filesystem/game calls or allocation-failure/unwind claim. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-filename-path\00\9c\009cbe10\20261020T014939978250Z\receipt.json SHA256 404359eb0545fed3b896b03b35fab00cb494a3080d6b89d2102f931b8873f829. No new root credit. Next: compose native path conversion into InitEntry filename-tree/lowercase fixture; entry-list/header parser remain controlled.


Native InitEntry filename path/tree composition accepted: 37 original/final exact bodies, 1600 differential plus 1600 native guarded scenarios, 44 fault controls and 12 required-provider omissions. Native path conversion, wide/narrow ownership, lowercase copy-on-write and stable filename multimap are composed, including drive/relative/UNC/slash paths, extensions and high bytes. Original/rebuilt full576KiB arena digests and event traces agree; inputs remain unchanged, all guarded allocations are released, both string counters return to zero. CRT split-path modeled in oracle and real in host; fixture anchors/manual tree teardown and symbol/CRC fail-fast boundaries explicit. No allocation-failure/unwind claim. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-path\00\9c\009ce050\20261020T015359927584Z\receipt.json SHA256 0fc5592ad2c1219f26e858c246860c64ce0f853e24255145f42b2bf5620b24ff. No new root credit. Next: native symbol-index map and CRC branches, then ReadEntryList; header parser remains controlled. Symbol map operator[] scratch matches96B/5fixups with size optimization, not yet promoted.


Native InitEntry symbol map accepted: 20 original/final exact bodies, 1600 differential plus 1600 native guarded cases, 28 fault controls and 10 required-provider omissions. Recovered map operator[]96B, lower-bound56B, hinted map wrapper29B, hinted tree insertion456B and unique insertion166B, plus existing iterator helpers44B/57B. Repeated symbols overwrite the original node value without duplication; null/empty/high-byte keys, aliases, insertion order, red-black invariants, owner counts and all final frees verified. Full128KiB original/rebuilt arenas compared. Filename/CRC branches remain separate. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-symbol\00\9c\009ce050\20261020T015908583283Z\receipt.json SHA256 68097896f19515ba5e28badfa5344cede35038728f8194ffce77bf57952abf32. Historical manifest names/prototypes preserved; five source/grade rows updated, existing helper identities/grades retained. Next native CRC-index branch then complete InitEntry composition and ReadEntryList.


Native InitEntry CRC-index branch accepted: entry302B, CRC56B and pair-vector growth259B plus the full1024-byte CRC table match original and final linked bytes. 1056 differential plus 1056 native guarded cases, 16 fault controls and three required-provider omissions. Independent bitwise CRC checks counted strings including embedded zeros/high bytes, null/empty and varying capacities; existing pairs survive repeated growth, Dirty is set, source fields remain unchanged and all guarded allocations are freed. Full128KiB original/rebuilt arenas compared. Growth behavior scoped to actual append(count1,atEnd=true); no arbitrary insertion/allocation-failure/unwind claim. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-crc\00\9c\009ce050\20261020T020441549482Z\receipt.json SHA256 12d4a976e54856efd27b0dfd41b80b77f0248a2fb4c4487d5421ca738d2388f3. Historical009b6300 vector_insert identity retained; typed growth source/grade registered under existing InitEntry source. Next compose all three InitEntry branches, then ReadEntryList.


Complete native InitEntry indexing accepted: 46 original/final exact code bodies plus full1024-byte CRC table, 3072 differential plus 3072 native guarded scenarios across all eight indexing-flag combinations, 70 fault controls and 18 required-provider omissions. Symbol duplicates overwrite one node; filename duplicates retain stable order after native path/lowercase conversion; CRC pairs preserve append order through native growth. Inputs, red-black trees, counters and ownership checked together; full576KiB original/rebuilt arena digests/event traces agree and all guarded allocations are freed. CRT split-path modeled in oracle and real in host; fixture anchors/manual tree-vector teardown/cold siblings explicit, no allocation-failure/unwind claim. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-init-complete\00\9c\009ce050\20261020T021317466816Z\receipt.json SHA256 21ce631f4b9b86d848a38decc85a65a5f33076323cb495f782dcff553154aba0. No new root credit. First combined fault run exposed fixture dereference-before-count checks; fixed to require allocations before reading nodes, all70 controls now clean failure80. Next ReadEntryList dependencies: small string vector Allocate99B/Copy68B already exact in scratch; Clear69B/default15B/assignment40B/destruction ownership also match, 240 native scratch cases. Existing unsigned Size declaration needs scoped review: native Copy sign-extends its length byte.


Native small string vector accepted: Allocate99B/3fixups, Copy68B/2fixups plus existing Clear69B/default15B/assignment40B/dtor12B/Unassign87B, seven original/final exact bodies. 240 differential plus 240 native guarded cases, 16 fault controls and six required-provider omissions cover zero/replacement allocation, shared/null/distinct anchors, self-copy and array-cookie destruction. Allocate tested0..255 including128/255; Copy bounded0..127. Self-copy clears old contents and copies newly null elements. Scoped signed casts reproduce Copy byte reads while preserving shared Size type/layout; only two declarations added. Receipt D:\Documents\FableTLC\rebuild\build\native-small-string-vector\00\9d\009d2160\20261020T022021996630Z\receipt.json SHA256 ac3dedae3c508b3fdac17a5d01635ec823501d856122963cef8613a6667b8b88. Header refresh recompiled all bank lifetime providers: owned recovery182 bodies/216 cases/16 controls passed; D:\Documents\FableTLC\rebuild\build\native-bank-recovery-owned\00\9d\009d0380\20261020T021823703779Z\acceptance.json SHA256 ae1782372e694fe4ddbf9e6584e5fa35ec4c67cec490f3bc92f8cd7f00c82bd4. Historical identities retained; two source/grade rows promoted. Next ReadEntryList and post-entry packing dependencies.


Native packed-array initialization accepted: CalculateBias130B, CountBits285B, PackArray238B and Initialise152B plus existing Get101B/Sizeof22B/Clear29B and full132-byte mask table all match original/final bytes. 3360 differential plus 3360 native guarded cases, 16 fault controls and seven required-provider omissions cover widths0..32/clamping40, zero/positive/negative strides, old storage and source values. Independent bit packing/decoding, full128KiB original/rebuilt arena digests and allocation/free traces agree; all guarded arrays freed, source/padding preserved. Original width32 carry quirk deliberately retained in120 host cases; VC71 x86 shift behavior explicit, no allocation-failure/unwind or large-size arithmetic claim. Receipt D:\Documents\FableTLC\rebuild\build\native-packed-uint-initialise\00\a6\00a62d80\20261020T024120816658Z\receipt.json SHA256 f361b00eee8bc586a5b84549d1cbe7eb13c5ee71738290d34ca93572b6aa7d5f. Header declarations only; owned bank recovery182 bodies/216 cases/16 controls passed: D:\Documents\FableTLC\rebuild\build\native-bank-recovery-owned\00\9d\009d0380\20261020T024134377143Z\acceptance.json SHA256 e014bca5831dbe9a212f7e110e6c938a99993811e133d9dc4b7dcac1c38196a1. Four historical source/grade rows promoted. USER-REQUESTED PAUSE after this task: no build/test process should remain. Resume with PackRuntimeData270B/13fixups scratch work/native-recreation/loading/bank-entry-list/20261019/pack-runtime-022308787665/root0.cpp (exact); it is not promoted or composed with new helpers yet. Then InitBlockOrder and ReadEntryList. All source/work/build/runtime/evidence/ref holds and cleanup protections continue; no lease/STOP/game/save changes or general cleanup release.


Native bank runtime-data packing accepted: PackRuntimeData270B/13fixups (009CD740) original/final exact, composed with the seven accepted packed-array bodies and132-byte mask table. 320 differential plus 320 native guarded cases (80 no-op, 168 keep-packed, 72 keep-runtime) cover counts0..100, reserve slack0..60 including the n=2 threshold, five record shapes and previous packed storage; 9 byte-mutation controls caught by oracle and host (18) and8 required-provider omissions rejected. Runtime release009D3D90 is an observed /alternatename stand-in (accepted separately by native-bank-allocate). Receipt D:\Documents\FableTLC\rebuild\build\native-bank-pack-runtime\00\9c\009cd740\20261020T025025768034Z\receipt.json SHA256 3bbffd396b5e2d8a7a6fd8ef63dd26e3bf2cfc0061f4de31841d46fe05d1637e. One header declaration added; owned bank recovery182 bodies/216 cases/16 controls re-passed: D:\Documents\FableTLC\rebuild\build\native-bank-recovery-owned\00\9d\009d0380\20261020T025141325938Z\acceptance.json SHA256 b4ead02a1565f5cfe727220e680b697a333b93de368355c6618e47b32192a9d6. Gate registered in gate_all. Next InitBlockOrder (009D0630), then ReadEntryList.


InitBlockOrder scratch (not promoted): work/native-recreation/loading/bank-entry-list/20261019/block-order/root10.cpp compiles to the exact192B/3fixups root 009D0630, and its out-of-line comparator CBankFile::CompareDataOffset::operator() matches retail 009CC640 (94B, exact). Shape that matches: inline push_back via fable_stlport_vector_impl.h (needs <new> first), sort expanded by hand (__lg depth, __introsort_loop + __final_insertion_sort), end() loaded before begin(), and an explicit-specialization declaration of __final_insertion_sort<unsigned long*,CompareDataOffset> so VC7.1 keeps it out of line (inline_depth/auto_inline/instantiation-order pragmas all failed to). Header needs InitBlockOrder() (private) plus public nested struct CompareDataOffset; block-order/probe.py patches a scratch copy and walks the call closure against retail. Open helpers: __introsort_loop 009D48C0 (139B, differs only because retail calls __median 009D2E40 as __fastcall; shared fable_stlport_algo.h declares __median cdecl/inline, so adding FABLE_FASTCALL there renames seven dead comdat rows in symbol_index and moves gate pins -> rerun those gates); __final_insertion_sort 009D3ED0 (retail inlines __unguarded_insertion_sort_aux into a loop over __unguarded_linear_insert 009D2FC0, ours calls aux); __insertion_sort 009D3B90, __unguarded_partition 009D2ED0, linear insert 009D2FC0 and the partial-sort heap helpers (009D3040/009D3750/009D3CF0) unprobed; vector<unsigned long>::_M_insert_overflow 00464ACE is landed (UlongVector_Overflow_00464aab.cpp) - link that object, do not let this TU emit its own copy. Then ReadEntryList. No build/test process left running.


Native bank block order accepted: CBankFile::InitBlockOrder (009D0630) plus its whole STLport sort closure, 16 bodies. Root near_match (203B vs192B, 0.949 opcode similarity, same behavior); CompareDataOffset94B, __introsort_loop136B, __median132B, unguarded_partition234B, __partial_sort168B, __make_heap77B, __adjust_heap186B, __push_heap186B, sort_heap80B, __final_insertion_sort75B, __insertion_sort217B, __unguarded_linear_insert127B exact, with packed Get, mask table and landed ulong _M_insert_overflow188B re-verified exact. 408 differential plus 408 native cases (counts0..500, validity/size patterns, ascending/descending/random/equal/few-distinct offsets, runtime and packed offset tables, prefilled no-op); an independent Python STLport 4.x sort predicts exact tie order, and a McIlroy adversary drives six cases through the depth-limit heap path. 7 mutation controls caught by oracle and host (14); 10 required-provider omissions rejected, one documented redundant comdat provider. Receipt D:\Documents\FableTLC\rebuild\build\native-bank-block-order\00\9d\009d0630\20261020T032147467783Z\receipt.json SHA256 f2e3fa5c33d649b819da541da1081931f904746e1aa9de19543b9b49ae469cef. Why the root is near_match: the landed 00464ACE overflow is an explicit specialization, so this unit declares it; without the body VC7.1 cannot see the call leaves the address-taken loop index alone and reloads it. The exact192B form (scratch block-order/no_spec.cpp) needs the generic body in this unit and then collides with the landing (LNK2005). Shared-header changes: fable_stlport_algo.h __median is now FABLE_FASTCALL (retail 009D2E40); a 25-unit census shows no code body changes, only the dead out-of-line __median comdats renamed (symbol_index refreshed). fable_game_text_bank.h gains InitBlockOrder() and the Ego_r CompareDataOffset functor. An algobase-wide STLport trivial copy_backward (memmove) plus inline __linear_insert was tried and reverted: it changes about 30 landed units incl. _M_fill_insert/_M_range_insert; this unit specializes both instead. Regression re-passed: pack-runtime, init-entry, allocate, packed-initialise, recovery-owned (open-wrapper build refresh list now includes fable_stlport_algo.h) D:\Documents\FableTLC\rebuild\build\native-bank-recovery-owned\00\9d\009d0380\20261020T032350311309Z\acceptance.json. Labels ghidra_out/labels_bank_block_order.tsv (the old manifest name CPackedBlockAllocator::RebuildFreeList at 009D0630 was wrong); not yet applied with LabelApply. Next ReadEntryList 009CFBC0 (1969B): every callee is landed except ReadPresizedString 00996390.


USER BEDTIME STOP (no process running). ReadEntryList 009CFBC0 in progress, NOT gated or promoted: rebuild/src/compiled/00/9c/CBankFile_ReadEntryList_009cfbc0.cpp is near_match (691/691 instructions, 0.952 opcode similarity; remaining diffs are register/stack-slot choice: retail caches filenames.Data in ebx, VC7.1 here reloads it). Supporting shared-header changes, each censused identical for every landed user: fable_bank_entry_update.h now retail 0x18 layout (Ego_r StateBlock dropped) with the inline ctor; fable_small_vector.h gains the inline default ctor; new opt-in fable_small_vector_impl.h holds Allocate/Copy bodies (landed AllocateCopy unit not yet switched to it; scratch alt/ copy proves it stays exact). Host fixture rebuild/tests/integration/NativeBankEntryList_test.cpp + case generator NativeBankEntryList_model.py (360 cases, 1850 entries) written, never built/run; link_closure with that fixture closes at 28 sources, zero frontier, zero duplicates (list in work/.../entry-list/closure/fixture/closure_sources.txt). Next: write gate_native_bank_entry_list.py (pack_runtime gate shape: those 28 sources + fixture, run cases.bin, near_match proof for the root, byte-mutation controls, provider omissions), then promote. Done earlier this session: PackRuntimeData MATCH, InitBlockOrder near_match + 15 exact STLport sort bodies (gate_native_bank_block_order.py), both registered in gate_all.
