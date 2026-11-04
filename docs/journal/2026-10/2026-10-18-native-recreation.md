# Native recreation - 2026-10-18

User requested continued recreation work until asked to stop; Lua is excluded.
Resumed from the October 17 journal. No commits or pushes.

## Movable-resource pool construction and binding

- Added `CMovableResourceMemoryPool_Construct_00a62e20.cpp`: the constructor at
  A62E20 calls the base constructor and installs the derived vtable; the typed
  SetMovableResource wrapper at A62EB0 forwards to InternalSetResource A028B0.
- Ownership/signatures: Ego_r PDB names; retail disassembly independently confirms
  the base constructor call, derived vtable 129D6A0 and forwarding target.
- `gate_native_movable_pool_construct.py`: both roots MATCH after checked
  relocation; registered in gate_all, symbol_index, overrides and artifact index.
- `movable_pool_construct_oracle.py`: 272 differential cases (16 construction,
  256 binding), checking all mapped pool/handle/header storage, callee-saved
  registers, stack cleanup, constructor result and base-constructor call order.
  The base constructor is controlled, with deterministic full-object writes and
  caller-saved register clobbering. Binding executes the genuine compiled and
  retail InternalSetResource helpers. No full allocator/startup claim.
- Receipt: `rebuild/compile-gate/native-movable-pool-construct.json`.
- Existing movable-pool policy regression passed: 2,000 compiled and 2,000
  comparative scenarios, eight providers and eight vtable slots.

## Scoped allocation and destruction

- AllocateResource A62E40 is exact (98 bytes). The scoped guard is now visible
  inline outside the base-pool TU, while that TU retains its standalone copies.
  Header sweep before/after: all 19 pre-existing includer TUs have unchanged
  named-symbol bytes/relocations. The twentieth TU is the new allocation source.
- `gate_native_movable_pool_allocate.py` passes 1,728 differential cases:
  successful/failed allocation, try-lock refusal, flag bits, size boundaries,
  handle selection, null/non-null resource values, full storage comparison and
  preserved registers/stack. Trace checks require binding before releasing the
  lock and no allocation/binding/unlock after lock refusal. Lock and allocation
  calls are controlled; InternalSetResource is genuine on both sides.
- The derived destructor is implicit. No Ego_r destructor declaration exists;
  retail vtable slot 0 points at A62F90, whose 30-byte deleting destructor calls
  the base destructor directly, then conditionally operator delete. Removing the
  model's explicit destructor declaration reproduces it exactly. Header sweep
  over 20 includers changes only this newly emitted deleting destructor in the
  new constructor TU. No existing provider changed.
- The construction gate now checks that destructor too: 400 differential cases
  total, including flags 0/1/2/3/80/81/FFFFFFFE/FFFFFFFF, call ordering, no derived
  vptr reset, preserved storage and this-pointer return. Base destruction and
  operator delete are controlled boundaries.
- All five base-pool exact gates pass (34 roots; the earlier journal's stated
  39 total is not reproduced by the current five ROOTS lists). Free/coalescing:
  2,592 compiled + 2,592 comparative scenarios pass. No full gate_all run.

## Typed accessors and candidate-required tests

- Retyped GetPointerFromHandle A026B0 and Unlock A02710 to the shared pool/header
  declarations. Both remain exact. These replace existing catalog providers,
  rather than adding two newly reconstructed functions.
- Their old tests defined local copies of the implementation. Replaced these
  with calls to the linked candidates, full-object write guards and (Unlock)
  repeated-call/idempotence checks. Both new gates require linking without the
  provider to fail and require the test COFF not to define the tested symbol.
- `gate_native_pool_access.py` / `gate_native_pool_unlock.py`: compiled fixtures
  and 48 differential cases each pass. Test tables deliberately permute handle
  order; unlock covers flag bytes 0/1/2/255 and counts -10/0/4/INT_MAX.
- The first native test builds stopped at a fixture mistake: STLport's capacity
  field is `_M_end_of_storage._M_data`, not a raw pointer field. Corrected before
  the passing runs; no production implementation change was needed.
- Publishing the typed symbols exposed two historical conflicting providers:
  DCEC50 edits reference counts at +28/+2C; C283E0 reads a table at +440. The
  verified pool's table is +438. Added RETIRED_POOL_SOURCES to symbol_index for
  fresh and incremental indexing; historical sources/tests/catalog rows remain
  untouched, and no replacement owner is invented. Current index has unique
  A026B0/A02710 providers for these typed symbols.

## Composed real allocation/free lifecycle

- `gate_native_movable_pool_lifecycle.py`, in gate_all, passes 384 scenarios /
  2,308 full 128-KiB memory-and-call checkpoints. Fourteen genuine compiled
  providers run against their retail counterparts: the movable wrapper, actual
  BeginCritical/EndCritical, base Allocate/InternalAllocate/InternalAllocateHandle,
  CreateArea/ResizeArea/FindFreeArea/UpdateLargestFreeArea, InternalSetResource,
  Free/InsertFreeArea/CombineAreas. No allocation/helper substitutes are used.
- The sequence allocates twice, frees the first allocation, reallocates and
  checks handle reuse, then frees everything. Assertions cover alignment,
  resource binding, unchanged memory on failure, final coalescing/accounting,
  stack cleanup and preserved registers. Both sides compare every saved memory
  checkpoint and callback trace.
- Matrix: pool capacities 128/256/512/1024; zero/small/alignment-edge/oversized
  requests; flags 0/1/2/3; single-threaded and controlled recursive Win32 locking;
  first or nested try-lock refusal. Depth returns to zero after every call.
- All 14 provider omissions are detected. An initial attempt listed the
  standalone scoped-guard destructor too; the omission check demonstrated it was
  redundant because this caller expands its body. Removed it from the claimed
  provider set. Initial composition also stopped at a missing InternalSetResource
  body; added the real provider and filled unprovided code with traps.
- Boundaries: Win32 lock callbacks (no concurrent threads), a preallocated
  four-slot handle table, no vector growth, failure-handler or defragmentation.
  Existing Free is a previously accepted near match; this is composition evidence,
  not a new root or an exactness upgrade for Free.
- Receipt: `rebuild/compile-gate/native-movable-pool-lifecycle.json`.

The lifecycle test now includes the genuine payload/handle accessors, resource
getter, Lock and Unlock: 19 required providers, 384 scenarios / 6,168 checkpoints,
and all 19 omission checks pass. Every successful allocation checks the pointer
round trip, bound resource and repeated lock/unlock idempotence before freeing.
Payload bytes are written within each requested allocation. All providers except
the already accepted near-match Free also pass full patched-byte equality.

## Scoped ownership and size access

- Restored CScopedMemoryHandle's missing inline constructor/destructor:
  FableWin 03363260 initializes Pool=0/Handle=-1; 03369910 calls Clear. Retail
  Initialise A039F0 independently confirms the old allocation is released before
  resetting/rebinding. This fixes uninitialized ownership and missing automatic
  release; it adds no standalone retail-root credit.
- `gate_scoped_memory_handle.py` / `ScopedMemoryHandle_test.cpp`, in gate_all:
  36 handle-pair cases pass, with canonical inline methods and only Free controlled.
  Checks poisoned initial storage, explicit Clear, repeated Clear, rebinding,
  automatic destruction, null-pool behavior and ownership during the Free callback.
  Before/after header sweep: all 22 existing includer TUs retain identical named
  symbol bytes/relocations. Related access/allocation/lifecycle gates pass again.
- GetSizeFromHandle A026D0 is exact (19 bytes), in the typed access TU and gate.
  Ego_r names the method; retail reads Header.Size without adding Padding.
  Candidate-required compiled fixture and 48 differential cases pass, including
  permuted handles and high-bit sizes. Registered as a companion provider; added
  to the connected test and required by its omission check.

## Dynamic handle-table growth

- The actual emitted STLport CHandleInfo vector `_M_insert_overflow` A03900
  matches all 227 bytes after two checked CRT relocations. Gate
  `gate_native_pool_handle_growth.py` checks exactness and eight genuine
  InternalAllocateHandle-driven differential scenarios. Scope is append-by-one
  as used by that caller; general fill/range insertion is not claimed.
- Lifecycle matrix now runs both empty/growing and preallocated handle tables:
  768 scenarios / 12,336 full 256-KiB memory-and-call checkpoints; 21 required
  providers, all 21 omission checks detected. It executes real vector growth,
  copies existing handle entries and releases old storage. Controlled CRT
  malloc/free track allocations and poison freed blocks. The remaining live
  allocation is checked to be exactly the pool's owned handle-table capacity.
- Current boundaries: recursive Win32 lock and CRT allocation callbacks;
  no concurrent threads, failure-handler or defragmentation. No full gate_all,
  full-engine startup or coverage dashboard claim.

## Scoped allocation and original debug ownership connected

- Final lifecycle gate: **1,152 scenarios / 15,024 checkpoints, 22 genuine
  providers, all 22 provider omissions detected**. The additional production
  provider is Allocate(CScopedMemoryHandle&, size, flags) A032F0.
- The scoped sequence constructs an owner, allocates and rebinds it, checks
  failed allocation preserves the old owner, explicitly clears it twice, then
  allocates again and destroys it while live. Accounting returns to zero and the
  free areas coalesce after both explicit and destructor-driven release.
- Candidate construction/Clear/destruction use fixture wrappers around the
  canonical header. Reference ownership uses actual FableWin bodies at
  03363260 / 0319B0B0 / 03369910 and its checked Clear jump thunk 01805FA2.
  The debug Free(long) thunk 017E0A90 is explicitly redirected to the corresponding
  retail-layout Free provider, keeping this, arguments and return address. Thus
  both reference and candidate ownership operate on the same retail pool layout.
  This route substitution is a boundary, not a claim that debug Free equals
  retail Free. The three fixture wrappers are excluded from provider/root counts.
- Final header sweep: all 22 includers compile; only the new GetSizeFromHandle
  symbol differs from the preceding snapshot. No pre-existing code changes.
- Final focused checks pass: construction/binding/destruction, scoped allocation,
  payload/size access, unlock, scoped ownership, handle growth, base allocation
  policy and the connected lifecycle. Earlier free/coalescing and movable-policy
  regressions also passed. Whitespace checks pass. Full gate_all was not run.

## Resource relocation connected

- `gate_native_pool_relocation.py`, in gate_all: **192 scenarios / 1,312 full
  256-KiB memory-and-call checkpoints, 23 required providers; all 23 omissions
  detected**. Actual Defragment A02730 calls the movable pool's actual
  MoveResource A62F30; surviving allocations keep their data and handle identity
  after the first allocation is freed and the remaining allocations compact.
- Cases include zero/small/oversized requests, three pool capacities, resources
  present/null, single/multi-threaded modes, disabled defrag, shutdown and locked
  allocations. Callback checks see the copied payload and the temporarily
  invalid handle slot used by retail during relocation. All remaining allocations
  free/coalesce afterward. Existing exact providers are not recounted.
- memmove, CRT allocation, Win32 locks and resource RelocateData are controlled
  boundaries. No concurrent threads, failure-handler, MoveArea or partial-defrag
  acceptance is inferred from this full Defragment test.
- Initial fixture stopped when oversized requests left no live allocation to
  lock: a nominal `locked` case then correctly ran defragmentation. The fixture
  now treats it as blocked only when a live handle was actually locked. No
  production-code change was made.
- Receipt: `rebuild/compile-gate/native-pool-relocation.json`.

MoveArea and CombineAdjacentAreas are now connected too: the final relocation
gate passes **256 scenarios / 1,824 checkpoints, 25 required providers and all
25 omission checks**. The added 64 cases move a live allocation into a freed
earlier area, verify handle/resource/payload transfer and the different callback
contract (the old handle remains valid during MoveArea), coalesce the old area,
then free the survivors. Zero-sized payloads and present/null resources are
covered. This still does not claim PartialDefragment.

Provider ownership audit passed for all eight added/retyped exact roots, with
one index provider each and both retired mislabels excluded from fresh and
incremental selection. Receipt:
`work/native-recreation/rendering/20261018/pool_provider_ownership.json`.
Final current-input audit checked 200 recorded input hashes across eight native
receipts with no drift, including the final relocation extension. Ownership and
whitespace audits also pass.

Next: compose partial defragmentation/failure-handler policy or trace the next
native resource-loader dependency. Existing deferred byte-match attempts remain
deferred. No game launch, Lua work, commits or pushes in this session.
CheckBufferedTextures is still unlocated in retail; the Ego_r/FableWin copies
alone do not prove that a retail out-of-line body exists. Existing deferred
pool InitialiseFromPreallocatedBuffer/RunRandomRelocationTest attempts remain
deferred. No coverage dashboard edits or full-engine startup claim.

## Continued at the user's request: partial defragmentation

- `gate_native_pool_partial_defragment.py`: **864 scenarios / 14,688 full
  256-KiB memory/call checkpoints, 31 required providers; all 31 omissions
  detected, 120 relocation callbacks**. Actual PartialDefragment, GrowFreeArea,
  TestIfAreaIsGrowable and CanMoveResource join the existing real allocator,
  area movement, coalescing, data access and cleanup chain.
- Eight allocations fill each 512/1024-byte pool; tests free alternating, edge
  or middle allocations. Matrix covers request sizes, candidate limits 0/1/16,
  locks, accepted/denied movement, single/multi-threaded modes and permission
  revoked after the first relocation. Remaining payload/handle/resource identity,
  successful-request capacity, accounting and full cleanup are asserted.
- The initial 648 static-permission cases left the stuck-area vector cold;
  that did not count as necessity evidence for those two helpers. The permission
  revocation case reaches the actual stuck branch, including temporary-vector
  allocation and destruction. No engine-source algorithm was changed.
- The emitted stuck vector push_back A038C0 (50B) and POD overflow A03780 (216B)
  are exact; `gate_native_pool_stuck_queue.py` also passes 20 differential cases
  (eight real stuck-branch cases plus twelve independent append/alias cases).
  Alias tests grow through capacities 1/2/4/8 with freed storage poisoned.
  Both template roots are registered in gate_all, symbol_index and overrides.
- Boundaries remain Win32 locks, CRT allocation/memmove and resource callbacks.
  No concurrency or failure-handler-driven allocation claim for this test.
- Next test in progress: allocation-failure policy using a controlled callback
  that releases queued handles through the actual Free provider. The first run
  trapped at missing standalone CScopedCritical destruction A03530; this path
  was not needed by the previous allocation matrix. Added that genuine provider.

The failure-policy gate now passes **524 scenarios / 8,688 full memory/call
checkpoints, 28 required providers, all 28 omissions detected**. It makes 460
failure callbacks and releases 304 handles through genuine Free. Matrix covers
release queues, thresholds, oversized requests, movement permission, flags 0-3,
shutdown and outer/nested try-lock refusal. Recursive lock depth reaches three
while the callback frees memory. The two stuck-vector helpers were not exercised
by this matrix and were removed from its provider list; their separate partial
defrag and queue evidence above is unchanged. The failure callback is compiled
fixture code on both sides, not the bank handler implementation.

## Bank allocation-failure recovery (in progress)

- The saved October 15 HandleFailedAllocation draft is present at
  `work/native-recreation/rendering/009d5bf0/20261015-evening/`.
- A new scratch candidate appends that method to the current lifetime TU and
  includes the canonical tree-bounds definitions. All seven existing lifetime
  roots remain exact in the scratch context. The new bank method is a passing
  near match: 305 vs 314 bytes, 109 vs 113 instructions, 92.7928% normalized
  similarity. No byte-shape tuning is planned; behavior remains to be tested.
- Scratch source/parity receipt:
  `work/native-recreation/bank/009d5bf0/20261018/candidate-parity.json`.
  The method has not yet been promoted into canonical source.

The bank method has now passed 2,592 differential selection cases and was
promoted into the existing lifetime TU. Its permanent gate also composes the
actual CLoadingMemoryFailureHandler caller: **3,169 cases, four required
providers, all four omissions detected**, with pool FreeResource and restart-tree
insertion still controlled. The matrix covers empty/read sets, ordered traversal
shapes, eligibility masks, cursors before/between/after entries, wrapping, null
pools and invalid handles. Flags and cleared buffers are checked at callback
boundaries, with full-object storage and register/stack guards. Both genuine
critical-section wrappers run; the composed caller checks nested handler/bank
lock order. Existing seven lifetime roots remain exact after promotion.

The near method is indexed at 009D5BF0 as a companion of the lifetime TU, with
NEAR_MATCH evidence in overrides and an artifact row. No byte tuning or exact
credit is claimed. Next: connect this real loading-handler/bank path to actual
pool release and async-data relocation instead of the controlled FreeResource.

## Real loading-handler/pool failure composition

- `gate_native_loading_pool_failure.py`: **384 scenarios / 6,350 full 256-KiB
  memory/call checkpoints, 36 genuine providers; all 36 omissions detected**.
  352 handler calls queued 208 reads for restart and caused six actual async-data
  relocations. Recursive lock nesting reaches five (pool, handler, bank, pool).
- Actual CLoadingMemoryFailureHandler calls the new near bank selector, whose
  inlined FreeBuffer calls real FreeResource/Free. Allocator thresholds and
  partial defrag run the real CanRelocateData and RelocateData overrides through
  the retail secondary vtable. Relocation updates Buffer and interior Data
  pointers; original payload bytes, handles and resource identity are checked.
- Matrix covers eligibility masks, locked buffers, request sizes, flags 0/2,
  thresholds, pool threading modes and empty/populated handler registration.
  Surviving old buffers are cleaned through genuine FreeBuffer, the newly
  allocated buffer through Free, and accounting/coalescing return to empty.
- The bank selector remains NEAR_MATCH; all other providers except the already
  accepted near Free are byte-identical. Restart-tree insertion, Win32 and CRT
  services are controlled. No actual restarted disk reads or concurrency claim.
- Next: replace the restart-tree insertion boundary with actual pointer-tree
  insertion/rebalancing, then connect RestartAbortedReads if feasible.

## Actual restart-tree insertion and balancing

- `gate_native_loading_restart_tree.py` passes **386 scenarios / 6,382 full
  memory/call checkpoints, 42 executed providers and 42 detected omissions**.
  Real unique insertion, node creation, red-black balancing and both rotations
  replace the earlier insertion substitute. Ordered insertions and duplicate
  calls supplement the eviction matrix. Independent checks validate ordering,
  parent links, colors, black heights, extrema, counts and node ownership.
- `gate_native_async_bank_insert_unique.py` additionally confirms exact 180-byte
  unique insertion and runs the two ordered behavioral cases (32 insert calls).
  This is strengthened evidence for an existing provider, not new recovery credit.
  Its emitted predecessor is behavior-only (78 vs 79 bytes); no exact grade or
  exclusive historical ownership is claimed for that folded clone.
- First 20,000-instruction stops occurred in both original and rebuilt code.
  Saved diagnostics under `work/native-recreation/bank/009d5bf0/20261018/`
  establish finite repeated 256-bucket free-list scans, not corrupted links.
  This larger composition uses a fixed 100,000-instruction limit; engine code
  was unchanged. Both permanent gates are registered in `gate_all.py`.
- Focused receipt audit after fresh runs: **594 input hashes, zero drift**;
  ownership audit: ten exact roots uniquely selected, two old sources retired.

## REA evaluation

User authorized using REA if useful while continuing native work. Official
`morluto/rea` source was inspected at commit
`3537e58c1ab29309f06eabf50bc52adba9817a0c`. Published `rea-agents@6.7.0`
was installed with scripts disabled under `work/native-recreation/rea-evaluation/runtime`;
no global install or agent registration/configuration changes. Node 24.14.1,
existing Ghidra 12.1 and JDK 21 pass the selected provider prerequisites,
including its Windows native controls. A read-only function query against
retail RestartAbortedReads is in progress; no game execution or installed-file
modification. General doctor reports other unconfigured providers/clients,
which are outside this CLI-only evaluation.

## Actual restart resubmission and cleanup

`gate_native_loading_restart_submit.py` passes **384 cases / 7,118 checkpoints,
208 submissions, 44 required providers and 44 detected omissions**, with no cold
providers. Real RestartAbortedReads visits the genuine ordered restart tree,
forwards each object's unchanged 24-byte descriptor to controlled AddObject,
stores its returned UID, and clears the tree through actual recursive erase.
CRT free validates ownership and poisons released nodes; independent red-black
checks confirm the reset empty sentinel. A second empty update neither submits
nor frees anything. Untouched objects keep their old UIDs and descriptors.
Bank lock ownership, callee-saved registers and stack cleanup are checked.
Both added providers are exact against retail. No new recovery credit, actual
disk IO or concurrency claim. Registered in gate_all; next useful composition
is actual read-begin/SetupBuffer/completion on resubmitted entries.

## Restarted read-begin, allocation, completion and release

`gate_native_loading_restart_read.py` passes **256 scenarios / 6,500 checkpoints,
608 restarted reads and 380 refused lock attempts; 48 required providers and 48
detected omissions**. The real OnFileReadBegin invokes real SetupBuffer(false),
the shared Align provider and actual movable-pool allocation/access. Each
restarted object receives its new Buffer, Data plus SkipBytes and handle/resource
binding. Deterministic payload delivery stands in for disk IO; actual completion
sets Finished, clears ThreadedFileUID and permits restart, then genuine FreeBuffer
returns the allocation. Both completion statuses and three priorities execute;
lengths and skipped prefixes vary by entry. Bank outer try, nested SetupBuffer
try and threaded-pool try refusals preserve output/invalid-handle state and all
retry successfully. Final pool accounting returns to empty. All four added
providers are exact. No forced heap fallback, actual IO or concurrency claim.
Gate registered in gate_all; no engine source changes or recovery-count credit.

## Threaded-manager candidates and shutdown recovery

- New scratch AddObject 0098F0F0 ordinary C++ uses the shared manager/object
  layouts and a scratch-only CFileObject constructor that zeroes Overlapped.
  Both preserved candidates are 244 bytes / 82 instructions vs 240 / 79 retail,
  **88.1988%** similarity, below acceptance. Each passes 288 original/rebuilt
  cases over both devices, six operational priorities, UID wraparound,
  GlobalAlloc null/success and ignored SetEvent results. Full storage/call-order
  comparisons and ABI guards pass; new/Win32/map slots remain controlled.
  No canonical object-header change, promotion, ownership or coverage credit.
  Two candidates exhausted; further attempts require new evidence.
  [Superseded by debug-key/helper evidence and accepted0098F0F0/0098ECA0 below,021446UTC.]
- New WaitForObjectToFinish 0098F1E0 drafts are 200 bytes / 70 instructions vs
  181 / 68 retail, **78.2609%** similarity. Both preserved, unaccepted; the
  compiler duplicates the loop lookup. No third attempt without new evidence;
  no behavior or recovery claim.
- CThreadedFileManager::Uninitialise 0098E0D0 first source is **exact 150 bytes /
  46 instructions / seven audited fixups**. Source promoted; permanent gate
  `gate_native_threaded_manager_shutdown.py` compares original/rebuilt/independent
  full-storage and call-order models in **384 cases / 768 checkpoints**.
  Cases cover uninitialized/repeated shutdown, already-finished workers,
  0/1/2/8 controlled wait iterations, absent/present event and ignored Win32
  event/close failures. Locks are dropped before Sleep, regained for state
  checks and handle close, then released before DeleteCriticalSection. All
  unrelated storage, callee-saved registers and stack guards remain intact.
  Strict missing-provider trap detected. Win32 IAT identities independently
  verified from PE imports. Actual worker scheduling/Win32 handles controlled.
  Source, gate_all, root-only index, override and artifact row integrated;
  fresh canonical gate passes. No dashboard counts hand-edited.

REA default-profile query timed out at 330 seconds; its JSON retains bounded
diagnostics and target identity. A single second trial is running with a
900-second deadline and Decompiler Switch Analysis/WindowsResourceReference
disabled, explicitly changing the recorded analysis profile. No third identical
retry planned. Existing labeled database is not modified by either trial.

## Threaded time-slice / worker-wake methods

FableWin PDB method listing confirms public SetDeviceTimeSlice(device,float)
and KeepUpdating(), matching the neighboring retail routines 0098E170/0098E190.
Both first C++ bodies are exact: **18 bytes / four instructions / no fixups**
and **24 bytes / seven instructions / two fixups**. Permanent update gate
passes **120 cases**: 84 time-slice storage cases (both operational devices;
zeros, normals, extrema, subnormals, infinities and quiet NaNs) plus 36 event/
alertable-SleepEx cases with ignored failure/APC returns. Storage/call models,
original/rebuilt code, ABI guards and both provider omissions pass.

Initial emulator-model test expected signaling NaNs to quiet; Unicorn instead
preserves their signaling payload. Those two patterns are explicitly excluded
from this emulator contract, not used to alter engine code or infer hardware
behavior. Added public declarations preserve all layout asserts. Before/after
header sweeps compile 121 TUs: the new provider needs the new declarations;
**120 existing TUs have unchanged emitted bytes and relocation targets**.
Debug path records are excluded; compiler-renumbered internal $L/$T labels are
resolved to their recorded section hashes/offsets, retaining every relocation
site/kind and local record. Proof: threaded/20261018/header-impact.json.
Fresh exact gates, gate_all, unique root/companion index rows, overrides and
artifact row pass. Three manager roots added this continuation; no dashboard
numbers edited.

## REA trial result

The reduced-analysis query succeeded after about eight minutes. REA reports the
correct native x86 target SHA-256 and 200-byte RestartAbortedReads body. All
72 instruction addresses and four direct call targets agree with independently
decoded retail bytes. The dossier exposes the UID write and actual tree clear;
fresh import lacks our recovered names/types and cannot select a PDB companion.
Its returned pseudocode omits some receiver details that are present in assembly,
so source/ABI conclusions still require the established evidence and gates.

A repeated snapshot query started a new provider rather than an observed cache
hit; it was terminated at its separate 60-second bound. Owned Java process
cleanup verified. No cache-speed claim, further full-binary import retry, global
installation, agent config changes or labeled-project modification. Local CLI
is available if a future investigation benefits; current recommendation is
supplemental use, retaining existing labeled Ghidra and parity/behavior tools.
Evaluation receipt: `work/native-recreation/rea-evaluation/evaluation.json`.

## Worker main loop and exit cleanup

FableWin PDB confirms private ThreadMain/ProcessObjects; retail 0098F2A0 was
misnamed `CThreadedFileManager_Shutdown` in the old broad manifest. First source
is **exact 176 bytes / 67 instructions / five audited fixups** and is promoted
with permanent `gate_native_threaded_worker_exit.py`. Independent full-storage,
release-order and ABI checks pass **1,728 original/rebuilt scenarios**, including
0/1/3/8 entries in balanced/left/right ordered trees, null/mixed/live pointers,
null/mixed/present OVERLAPPED blocks, 0/1/3/8 controlled ProcessObjects iterations,
old finished-flag values and ignored GlobalFree failure results. Owned blocks
are poisoned on release, duplicate/wrong frees rejected, and finished is published
only after cleanup under the real enter/leave call boundaries. Map nodes remain
untouched for later owner destruction, matching retail. ProcessObjects, Win32,
CRT delete and scheduling remain controlled; no full IO/worker runtime claim.

Root-only index, gate_all, override and artifact row integrated. Four new manager
roots have unique index owners. Private declarations preserve 121 pre-existing
TUs' emitted bytes and relocation targets in a second 122-TU sweep (new source
needs the declarations); internal COFF label renaming normalized as above.
Fresh manager gates pass; focused receipt audit **899 hashes, zero drift**.

## Worker entry callback

Private static BeginThread(void*) is confirmed by FableWin PDB, while retail's
old generic manifest called 0098F380 InitiatePriority. First ordinary source is
**exact 24 bytes / six instructions / three fixups**. Independent PE imports
identify GetCurrentThread and SetThreadPriority: it requests priority +1, then
tail-transfers to actual ThreadMain with the supplied manager. Permanent gate
`gate_native_threaded_worker_begin.py` runs **3,456 direct/entry-path cases**
over the same worker corpus; both provider omissions are detected. API stubs
clobber volatile registers, validate pseudo-handle/priority order and leave the
cdecl context argument for the caller. Win32/scheduling remain controlled.

The worker oracle now models GlobalFree failure faithfully: a nonzero return
retains the allocation untouched, while original/rebuilt code still delete the
containing object and publish finished. Only successful frees poison/release
the block. This changes fixture accounting, not engine source. All worker gates
freshly pass after the refinement.

New callback source/gate/root-only index/override/artifact row integrated, with
explicit private-static cdecl metadata. Final header sweep compares 121 existing
TUs unchanged and two new TUs (ThreadMain/BeginThread), all 123 compiling.
Proof: `work/native-recreation/threaded/20261018/final-header-impact.json`.
No engine startup, game launch, native OS worker or disk-IO claim.

## Manager owning destruction

First CThreadedFileManager destructor 0098E690 is a passing **NEAR_MATCH:
214 bytes / 77 instructions / 98.7013% similarity**. Promoted without byte tuning
after **1,440 original/rebuilt full-arena cases with five required providers and
five detected omissions**. Real Uninitialise, UID/pending recursive erase and
base destructor execute. Fifteen owning maps release nodes and headers in the
retail order (UID first, fourteen pending maps in reverse); pointed-to file
objects remain untouched. Tests vary empty/UID-only/one-queue/striped/full maps,
null empty headers, 1/3 nodes, balanced/left/right topology, initialized/finished
state, controlled wait iterations and optional event handles. Win32 and CRT free
remain controlled. Full storage and ABI guards pass.

Typed owning map destructors and the POD pending-node erase definition are
visible only in this manager lifetime TU, following the existing contextual
map-lifetime pattern. The emitted pending erase helper is COMDAT; its established
provider retains ownership. New source selects only the genuine manager destructor
in the index; no incidental helper/vtable/deleting-destructor recovery credit yet.
Near metadata, artifact row and gate_all integrated; fresh canonical gate passes.

First omission audit caught zero-filled executable slack allowing an absent
null-node pending eraser to fall through into the equivalent UID helper. The
harness now fills the complete two-page code region with traps before installing
providers; all five omissions then fail. Engine code unchanged. Focused audit
after promotion: **1,012 input hashes, zero drift**; six new manager roots have
unique owners. Exact/near grades remain explicit; no dashboard update.

The compiler-emitted scalar deleting destructor at **00991330** is now separately
accepted exact (30 bytes / 11 instructions / two audited fixups). Its gate runs
the actual near destructor/shutdown/eraser/base chain in **5,760 cases, six
required providers and six detected omissions**, over flags 0/1/2/FFFFFFFF.
Only bit zero controls deleting the manager allocation; deletion occurs after
all maps and base destruction, exactly once, and the original pointer is returned
even after its storage is poisoned. Ret4 stack cleanup and saved registers pass.
Permanent gate, verified companion index and metadata integrated. The manager
destructor retains its NEAR_MATCH grade; this wrapper alone receives MATCH.

## Manager construction and complete empty-owner lifetime

First constructor candidate 0098F3A0 was byte-close (334 bytes / 99 instructions,
93.0693%) but failed behavior on the second malloc failure: the previous pending
queue's cursor was still uninitialized. Original instructions initialize each
CPendingObjectMap cursor to null before constructing the next map. Added that
genuine nested constructor, preserving the first object/header/parity receipt
under `work/native-recreation/threaded/0098f3a0/20261018/first*`.

Revised source is **350 bytes / 103 instructions / 96.1165% NEAR_MATCH** and passes
**696 original/rebuilt cases, five required providers and five omissions**.
576 normal cases vary eight storage patterns, four stack alignments, optional
event handles, ignored thread-start returns (including FFFFFFFF) and 0/1/3
controlled shutdown waits. Actual base/manager construction, fifteen header-node
allocations/initializations, settings, stack-aligned x87 time-slice initialization,
Uninitialise and owning destruction execute. All headers are freed; padding and
unrelated fields remain checked. No actual thread starts: _beginthread validates
the real callback, 0x3000 stack request and manager context then returns a
controlled value. Win32 initialization/event/lock/close/delete are controlled.

The other 120 cases fault at byte address zero on each of fifteen null malloc
positions, matching original memory, retained prefix allocations and call traces.
These are observed original faults, not an invented recovery path. Full cleanup
cases have empty trees, so two cold recursive eraser providers were removed
from this composition; their nonempty destruction gate remains mandatory.

Canonical promotion initially exposed VC71 C2908 when the nested constructor's
inline body in the shared header instantiated the pending-map destructor before
its owning specialization. Its declaration stays in the shared type; the inline
body is visible in the constructor TU. This preserves the established lifetime
specialization and the correct early cursor writes. Earlier failed include
snapshots are retained; corrected before/after sweeps compile 125 TUs and prove
**124 pre-existing TUs unchanged**, with the new constructor needing its new
declarations. COFF internal labels normalized as documented above.

Canonical near gate, source-only index, override, artifact row and gate_all
integrated. Eight manager roots uniquely selected; no byte tuning after the
functional fix. Prior manager gates are being refreshed against the final header.

All six earlier manager gate refreshes now pass against the final constructor
header. Focused receipt audit reached 1,145 hashes with zero drift.

## New evidence resolves the deferred manager wait

FableWin original code at 0300E900 was inspected after the two deferred drafts.
It supplies materially new control-flow evidence: separate initial/current
iterators, an early return for initial completion/absence and another early
return inside the wait loop (not assignment to one iterator plus shared exit).
Saved disassembly: threaded/0098f1e0/20261018/fablewin-wait-evidence.txt. A fresh
candidate following that structure passes **NEAR_MATCH 187 bytes / 68 instructions
vs 181 / 68 retail; 95.5882%**. No further byte tuning.

Permanent `gate_native_threaded_manager_wait.py`: **1,440 cases, three genuine
providers and three detected omissions**. Actual UID find and the already accepted
near SetObjectPriority execute. Cases cover statuses 0-4, absent/present UIDs,
pending/locked priority changes, active/completed requests, blocking priority,
both devices, UID 0/1/FFFFFFFF, balanced/left/right lookup shapes, 1/3 alertable
waits and completion/removal at the scheduling boundary. Pending queue removal/
insertion, Win32 and worker progress are controlled. Worker progress occurs
only with the manager lock dropped; recursive priority locks reach depth two.
Full storage, exact call-order model, stack and saved registers pass. No actual
concurrency, pending-tree removal recovery or IO claim.

Canonical source, near metadata, artifact, root-only index and gate_all integrated;
fresh canonical gate passes. Nine new manager roots uniquely selected. Latest
focused receipt audit: **1,187 hashes, zero drift**. AddObject remains deferred;
the earlier wait deferral is now resolved by new original evidence.

## Loading wait and real manager completion composed

Permanent `gate_native_loading_wait_completion.py` connects the real bank wait
009D5B60, forced buffer allocation, restart cleanup, bank lock release, threaded
file wrapper 0098F370 and manager wait/priority/UID lookup. Initial composition
passes **256 scenarios / 5,632 checkpoints / 168 waited reads / 53 providers and
53 detected omissions**. Its scheduling boundary publishes status and dispatches
the real data callback directly; OnFinishObject is explicitly excluded there.

New `gate_native_loading_manager_completion.py` replaces that adapter with actual
manager OnFinishObject 0098EBC0 and the primary virtual callback. Same corpus
passes with **54 providers / 54 detected omissions**; completion releases and
reacquires the manager lock, clears the callback/current object, and publishes
finished status. Primary callback vtable slot verified against retail. Original
and rebuilt full storage/call traces compare; independent assertions require the
callback to run outside both locks and finished publication after callback.
All completed requests use QueueWhenFinished=true and remain owned by the UID
map; nonretained cleanup is not composed here. Worker scheduling and outer worker
lock, pending queue edits, Win32/CRT and AddObject remain controlled. No actual
IO/concurrency or new recovery credit for existing OnFinishObject. Both gates
registered in gate_all. No Lua/installed-game/save changes or game launches.

## Completion cleanup and four exact manager statistics getters

Permanent `gate_native_loading_completion_cleanup.py` extends the loading/wait/
OnFinishObject chain through genuine UID key/range erase, node destruction,
rebalance and extrema helpers. Canonical run passes **2,304 scenarios / 50,688
checkpoints / 1,512 waited reads / 744 request deletions**, with **62 executed
providers and 62 detected omissions**. Retained, mixed and nonretained requests,
null/present OVERLAPPED blocks, successful/failed GlobalFree, both devices,
priority changes and delayed completion compare against original code.
Independent checks require callback/status/current-object ordering, valid red-black
UID ownership, full 0x40-byte request poisoning, no duplicate releases and
untouched allocations on GlobalFree failure. Scheduling, pending edits, AddObject,
Win32/CRT and disk delivery remain controlled; no actual IO/concurrency claim.

Three rejected fixture drafts are retained under
`work/native-recreation/loading-completion-cleanup/20261018/attempt1` through
`attempt3`: recursive request insertion order was mistaken for node order;
overlapped storage collided with the pool buffer; and a cold iterator-erase
wrapper was included although range erase inlines it. Assertions caught each;
engine source unchanged. Original retained and direct-wait gates freshly pass
256/5,632 with 54/53 providers and all omissions. A validation hook lets the
cleanup composition check poisoned requests without reading former status fields.

Four first ordinary C++ drafts are promoted **exact**: manager
GetTotalObjectsPendingCount 0098E390 (117B/49 instructions), GetTotalBytesPendingCount
0098E410 (118B/48), GetTotalObjectsFinishedCount 0098E490 (117B/49), and
GetTotalBytesFinishedCount 0098E510 (118B/48). FableWin PDB owner/const signatures
and retail loops correct the old CFileInstaller labels. Canonical permanent gate
`gate_native_threaded_manager_statistics.py` passes **12,600 original/rebuilt
cases / four omissions**, with independent totals, full storage and ABI checks.
Empty/1/3/8/17-node balanced/left/right traversals, all statuses plus unknown,
zero/high-bit/wrapping sizes and mutations at both lock boundaries are covered.
The shared STLport subset lacks const iterators; a documented const_cast obtains
an iterator without mutating map values. No broad STLport change.

The declarations preserve **126 existing TUs**, including every emitted named/
local symbol and relocation target; all 130 current TUs compile. Proof:
`work/native-recreation/threaded/statistics/20261018/header-impact/report.json`.
Four root-only index owners, correction rows, artifacts and gate_all entries are
integrated; no incidental helper credit or dashboard refresh. Thirteen manager
roots now selected across this continuation. No Lua or game-install changes.

Next callback 0098E320: first scratch source is relocation-exact 102B/31
instructions/seven fixups. FableWin PDB confirms the stdcall callback and
CFileInstaller::Unpause identity; debug _OVERLAPPED layout confirms +16 hEvent.
Original/rebuilt behavior is running; not yet promoted. Evidence:
`work/native-recreation/threaded/0098e320/20261018/first`.

Read-completion callback is now promoted **exact 102B/31 instructions/seven
audited fixups**, with a permanent **3,456-case original/model/rebuilt gate and
one required root omission**. Two separate error-state data providers are a
single writable five-byte COFF BSS contribution: callback pointer at 013BC864
and dirty-disc bool at 013BC868, both original-zero, with no relocations or
initialized payload. These are data providers, not two more functions.
FableWin field/method evidence and Ego_r OVERLAPPED layout saved locally.
Fastcall error argument, device-specific dirty marking, ignored transferred
bytes/API returns, status-before-event ordering and callback-sensitive manager/
installer reloads pass independently. Installer Get/Unpause and Win32 remain
controlled. Fourteen manager function roots plus two data providers indexed.
Declaration/OVERLAPPED changes preserve all **147 existing TUs**, with both new
TUs compiling (149 total); strong proof at
`threaded/0098e320/20261018/header-impact/report.json` under the work root.
New actual OS-callback -> manager -> bank-data -> UID-cleanup composition is
running; no actual IO/APC scheduling claim. Statistics/cleanup refreshes running.

## Actual OS completion chain passes; installer Unpause recovered

Permanent `gate_native_loading_os_completion.py` passes **3,072 original/rebuilt
scenarios / 67,584 checkpoints / 2,016 completed reads / 992 request deletions**,
with **63 executed providers and 63 detected omissions**. The scheduling adapter
first dispatches real stdcall read completion without either bank/manager lock,
then observes status 3 and takes the outer worker lock before executing actual
OnFinishObject, bank callback and UID destruction. Success/error delivery, both
devices, retained/mixed/nonretained requests and GlobalFree failures compare.
Installer is absent and hard-disk error callback null in this composition;
standalone callback gate covers their branches. No actual OS/APC/IO claim.
Canonical statistics and earlier cleanup gates freshly pass after that header.

`CFileInstaller::Unpause` 009C3250 is newly promoted **NEAR_MATCH 89B/34
instructions, 97.0588% similarity**. First draft was 83B/32, below threshold and
failed the independent volatile-read observer even though ordinary final values
agreed. FableWin PDB independently identifies both Running and Paused as
volatile. Restoring those qualifiers fixes observable reads; the C++ function
body remains identical to the first draft. The revised code passes **720
original/model/rebuilt cases with three actual providers/omissions** (Unpause,
scoped critical-section constructor and destructor). No further byte tuning.
The one remaining branch-opcode difference receives near, not exact, credit.

Tests include zero/one/multiple/high-bit/wrapping pause counts, zero/nonzero
running bytes, null/aliased event handles, ignored event failures, lock-entry/
exit mutations and first-event mutation of the later event and pause state.
Volatile member reads are checked in original order, alongside full storage,
call trace and ABI. Win32 locks/events controlled; no real concurrency.
PDB names and retail accesses recover CriticalSection, Running, Paused, Event
and PauseFinishEvent in the shared installer header; unrelated storage and
the existing startup flag offset are preserved. All **19 existing installer
TUs** retain emitted symbols/relocations; new TU compiles. Strong proof:
`work/native-recreation/threaded/009c3250/20261018/header-impact/report.json`.
First rejected and PDB-qualified drafts retained under `first`/`pdb-volatile`.
Canonical gate, root-only index, metadata and gate_all integrated. New source
credit: one installer method; existing lock/Get providers are not recounted.

Full existing-singleton Get + actual Unpause + OS/manager/bank/cleanup composition
is running in `gate_native_loading_installer_completion.py`. Its six focused
eight-read cases pass for pause counts 0/1/3 and running false/true. Existing raw
installer storage only; constructor/allocator/counted reset and installer worker
runtime remain outside this composition. No Lua or installed-game changes.

The full installer composition now passes **2,304 scenarios / 50,688 checkpoints /
6,048 completed reads / 2,976 request deletions / 864 installer event signals**,
with **65 executed providers and 65 detected omissions**. Actual existing-
singleton Get, near Unpause, scoped locks and OS/manager/bank/UID chains execute.
Pause counts 0/1/3 and running false/true exercise countdown across repeated
completions; wake events occur once at zero and outside bank/manager locks.
Installer Get's allocation/construction/reset paths are trapped/excluded;
installer storage is an existing raw object, not a recovered owned worker.
Registered in gate_all; canonical standalone callback freshly passes 3,456 cases.

PDB qualifier check does not resolve AddObject: NextObjectUID and
BlockPendingObjectProcessingRefCount are nonvolatile in FableWin. No new draft;
that two-attempt deferral is preserved. Next Pause 009C31E0 evidence identifies
retail global 013CA814 as CFileInstaller::DirtyDisc, corroborated by the exact
FableWin global RVA referenced by Pause. No Pause draft yet.

## Installer Pause, complete bounded pause cycle and real Windows APC delivery

Pause 009C31E0 is now promoted **exact 103B/34 instructions/six fixups**, first
natural draft, with a permanent **4,860-case original/model/rebuilt gate and
four actual providers/omissions**. Real scoped constructor/Release/destructor
execute. Volatile read order, dirty-disc bypass, counter wrap, lock-entry/unlock/
event/wait mutations and ignored failure/APC returns pass. Original increments
FFFFFFFF to zero and still takes the <=1 wait branch. It releases the lock
before event/wait and performs one infinite alertable wait, without adding a
retry for APC returns. Separate protected static DirtyDisc 013CA814 is one
verified writable original-zero BSS byte, not another function. Named
PauseStartEvent +8C added; all **20 existing installer TUs** preserve emitted
symbols/targets and both new TUs compile.

The first attempted before sweep ran late and captured 22 post-edit sources;
it is explicitly rejected, retained unchanged. The strong comparison instead
uses the already frozen Unpause-after 20-TU receipt as its pre-Pause baseline.
Provenance and final proof are under
`work/native-recreation/threaded/009c31e0/20261018/header-impact/`.

Permanent `gate_native_loading_pause_cycle.py` now passes **3,456 scenarios /
79,488 checkpoints / 12,096 reads / 5,952 deletions / 6,912 installer wake signals /
2,016 pause waits**, with **66 executed providers/omissions**. Real Pause,
read callback, existing-singleton Get, Unpause, locks, manager/data completion
and UID cleanup execute. Per-read counter model checks 0/1/FFFFFFFF, running
false/true and DirtyDisc false/true. Read initiation invokes Pause at an explicit
fixture boundary; actual ProcessObjects/call-site recovery and OS waits remain
outside this composition. First 2,304-case corpus passed behavior but left the
maximum helper cold; mixed retention adds that genuine path. Rejected corpus
saved under `work/native-recreation/threaded/pause-cycle/20261018/first`.

A separate **real Windows x86 ReadFileEx/APC/SleepEx/SetEvent** proof now passes.
Permanent `gate_native_threaded_read_apc.py` freshly compiles frozen source/header
copies and executes the original private 102-byte callback oracle and rebuilt
C++ against the same synthetic 36-KiB scratch file. **Nine real IO completions
per mode (18 total)** include zero/short/full reads and **two EOF/error
completions per mode**. The calling thread observes no completion during a
nonalertable sleep and receives completion in SleepEx. Both callbacks publish
status 3 and signal the correct owned event; the real fastcall error handler
can redirect the manager, exercising its later reload. Request, manager/file
guards and buffer bounds pass. **24 direct native controls per mode** are
recorded separately from actual IO. Loader-zero error-state storage is checked.

Five required-source link omissions fail, and a no-op rebuilt callback fails
the runtime object-state guard. First permanent run detected that ExitProcess
bypassed the buffered error diagnostic; explicit stderr flush makes the expected
negative-control failure reviewable. Positive native IO passed on both runs;
failed and passing build directories remain separate. Latest receipt:
`rebuild/build/native-read-apc/00/98/0098e320/20261018T213056413620Z/receipt.json`.
No original bytes are embedded in tracked code; oracle stays in the private
build. API context use follows [Microsoft's ReadFileEx contract](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfileex).

This native probe uses raw typed manager/file contexts and one outstanding IO
operation at a time. It executes the OS read-completion callback, rather than
an owned engine manager/worker or bank submission path. Installer is absent;
linked Unpause/lock dependencies have separate executed composition gates.
Only owned unnamed events and synthetic scratch files are used; no game,
installed data, saves, Lua, external account or desktop interaction. The new
compositions/probe receive no additional function-recovery credit. All new
permanent gates are registered; full gate_all and dashboard refresh not run.


### Worker scheduling and read/write submission (0098ECD0)

Promoted `CThreadedFileManager::ProcessObjects` as NEAR_MATCH: 792 bytes,
258 instructions versus the original 786/257, similarity 90.8738%. The first
compiling draft was 828/274 and 58.0038%; it stays in
`work/native-recreation/threaded/0098ecd0/20261018`. The second natural draft
uses a common wait exit and ordinary memset for OVERLAPPED initialization;
it passes the threshold, so no further parity tuning was attempted. No shared
header change was needed. Earlier noncompiling scaffold is retained too.

Permanent `gate_native_threaded_process.py` passes 960 original/model/rebuilt
cases with full 32 KiB storage prediction and comparison, preserved registers
and stack, lock/event ordering, all seven priorities and both devices, live,
expired, equal and NaN time-slice comparisons, begin-callback refusal/mutation,
blocked/current/completed requests, installer presence, submission failure and
hard-disk callbacks. Missing/no-op root controls both fail. API imports are
independently checked against retail PE names. Source/header/original-input
hashes remain pinned. One unique provider indexed; gate_all entry registered.

The original has distinct WriteFileEx (IAT 0143FDD8) and ReadFileEx
(0143FDF4) paths. Writes bypass OnFileReadBegin and handle failed submission
by invoking the nullable hard-disk error callback and existing installer
Unpause; reads ignore the submission return. Both wait alertably afterward.
This is preserved, including unsuccessful-read behavior. GFGetTime is the
already-recovered 009E1BC0 helper. PDB OpenedForWrite naming corroborates the
retail byte at file+18 despite debug layout shifts.

This gate controls pending removal, finish, timer, installer calls, callbacks
and Win32 APIs. It does not establish live worker scheduling, actual I/O, or
new helper recovery. Earlier real Windows APC proof remains separate. The
pending-removal and AddObject failures remain deferred, untouched.


### Real Windows read/write submissions from ProcessObjects

`gate_native_threaded_worker_read.py` now passes 84 original/rebuilt worker
cases per mode: 28 actual reads, 42 actual writes and 14 refused read callbacks.
Across the two modes this is 140 actual asynchronous file operations and 28
refusals. All seven priorities and both devices are exercised. Original parent
bytes are loaded privately with 27 independently decoded and checked fixups;
the rebuilt parent is freshly compiled from frozen source/header copies and
has 28 COFF fixups. Both parents use the exact rebuilt completion callback.

The calls execute real Windows Initialize/Enter/Leave/DeleteCriticalSection,
ReadFileEx/WriteFileEx, alertable WaitForSingleObjectEx, completion APC and
SetEvent. Native critical-section recursion counts verify release during the
begin callback, held removal boundary, and balanced return. Writes bypass the
begin callback, including when refusal is configured. Entire private output
file is checked independently for exact write ranges and unchanged remainder;
read input remains unchanged. Buffer guards, bytes, status/current request,
OVERLAPPED context and output hashes agree between parent versions.

Current frozen receipt:
`rebuild/build/native-worker-read/00/98/0098ecd0/20261018T214759322233Z/receipt.json`.
Three required link omissions and a no-op worker runtime control are detected.
The gate is registered in gate_all; ArtifactIndex points at the native fixture.
No full gate_all run is claimed. Earlier read-only 42-case receipts remain.

The first native prototype failed because its pending-removal fixture assumed
status zero even after an accepted begin callback. Original evidence preserves
status one until publication as READING; corrected fixture passes, with both
attempts retained in native-first/native-second. This was a fixture correction,
not an engine-source change. Earlier fixup-scaffold assertion expected 28 like
the candidate; original has 27 because the candidate emits an extra shared-exit
IAT reference. Complete original decode and target-set equality now pin this.

Limits: pending removal is a controlled no-op; finish is fail-only; time is
fixed, installer absent with trap stubs. No manager owner, engine worker thread,
retail files, game process, saves or installed files are used. All IO targets
are owned synthetic files in the fresh build directory. This advances the
submission call-site evidence; it does not recover pending removal/AddObject.


### Actual CRT worker entry, IO loop and concurrent shutdown

Permanent `gate_native_threaded_worker_thread.py` passes 56 owned CRT thread
lifecycles: 28 real reads and 28 writes, all priorities/devices, optional read
begin callback. Each _beginthreadex thread invokes actual BeginThread ->
ThreadMain -> ProcessObjects. IO completion APC runs on that initiating worker;
real priority above normal (1), distinct thread IDs, critical-section state and
callback ordering are observed. Main calls actual Uninitialise after a fixture
finish signal. It sets exit, wakes the worker, waits for finished publication,
closes the event/deletes the section, then the fixture joins the CRT thread.
All 56 joins/shutdowns pass, with no active probe processes left behind.

Fresh frozen BeginThread 24B, ThreadMain 176B and Uninitialise 150B bodies
remain exact after independently checked fixups. Worker parent remains the
accepted near source. Six missing link providers and a no-op BeginThread
runtime control are detected. Input pins include the VC71 header tree, compiler
and CRT/import libraries. Whole private output and read-input immutability
checked. Current receipt:
`rebuild/build/native-worker-thread/00/98/0098ecd0/20261018T215015739123Z/receipt.json`.
Gate registered in gate_all. No full gate_all run claimed. First successful
prototype retained in threaded/lifecycle-native/20261018/first.

Limits stay explicit: raw owner storage, fixed timer, existing installer absent;
pending-removal and finish are fixture boundaries. Pending removal empties the
single-node queue; finish validates status3, publishes status4, clears current
and signals the main observer. UID map is empty at actual worker exit, so this
does not establish native heap request cleanup or production OnFinishObject.
Other compositions cover those helper bodies independently. No new recovered
function credit for linking/executing existing lifecycle providers.


### Actual OnFinishObject on an owned worker thread

`gate_native_threaded_worker_finish.py` passes 56 native CRT thread lifecycles
with real read/write/APC submission and actual OnFinishObject 0098EBC0. Its
121B body is freshly exact alongside BeginThread, ThreadMain and Uninitialise.
28 virtual completion callbacks run on the worker with the manager lock
released and READ_COMPLETED status still visible. Main observes actual status4
publication under the critical section, then performs actual shutdown/join.
Callback pointer clearing, current clearing, output bytes and all 56 shutdowns
pass. Seven required link omissions and no-op entry runtime control detected.

Latest receipt:
`rebuild/build/native-worker-finish/00/98/0098ecd0/20261018T215155680731Z/receipt.json`.
Gate registered in gate_all; no full gate_all run. First prototype preserved in
threaded/lifecycle-finish-native/20261018/first.

This removes finish as an executed fixture boundary. Requests are retained
(QueueWhenFinished=true), UID map is empty at exit; cold UID erase is a fail-only
fixture dependency and GlobalFree/request delete are not exercised. Pending
removal still controlled, timer fixed, installer absent. Raw owner storage and
only private synthetic IO; no game/install/save changes. Existing helper reuse
is not additional recovered-function credit.


### Actual retained-request cleanup at native worker exit

`gate_native_threaded_worker_exit_cleanup.py` passes the same 56 native IO
thread lifecycles with one real retained request in each UID tree. Requests
are allocated with CRT new and OVERLAPPED blocks with real GlobalAlloc. Actual
ThreadMain exit traverses that non-empty tree and executes its real GlobalFree
and operator-delete branches before publishing ThreadFinished. Requests and
OVERLAPPED state are validated before Uninitialise; no freed pointers are read
afterward. All 56 threads join. 28 unlocked completion callbacks and read/write
output guards still pass; seven link omissions/no-op entry control detected.
The original ignores GlobalFree's return, so no separately observed free-result
claim is made. Header/node storage remains fixture-owned, preserving original
ThreadMain's uncleared/dangling UID values without dereferencing after join.

Receipt:
`rebuild/build/native-worker-exit-cleanup/00/98/0098ecd0/20261018T215341994619Z/receipt.json`.
Prototype kept in lifecycle-exit-cleanup-native/20261018/first. Gate registered;
no full gate_all run. Finish retains requests; key erase remains fail-only and
pending removal/fixed timer/absent installer stay explicit fixture boundaries.
No new function recovery credit. All allocation, threads and IO are owned by
the offline probe; no retail assets/game/install/save activity.


### Actual installer pause/unpause around owned native IO

`gate_native_threaded_worker_installer.py` passes 672 owned thread lifecycles:
336 reads, 336 writes, 336 unlocked virtual completions, 672 exit-object cleanup
paths, 672 shutdowns and joins. Actual existing-singleton Get, Pause, Unpause,
scoped lock constructor/Release/destructor execute on the IO thread around the
actual manager submit/APC/finish chain. Twelve initial count/running/dirty
combinations per base case cover 0,1,FFFFFFFF and overflow. Native pause-count,
lock state, Event and PauseFinishEvent observations match the independent
model. The acknowledgment event is an owned pre-signaled fixture event; no
installer worker/production acknowledgment claim. Null-singleton construction,
allocation and counted assignment remain fail-only traps.

Get/Pause/scoped helpers freshly exact; Unpause stays accepted near. Fourteen
provider link omissions and three runtime controls (no-op entry, Pause,
Unpause) detected. Full synthetic output checked. No function source changes or
additional recovery credit; these are stronger execution compositions. Current
receipt:
`rebuild/build/native-worker-installer/00/98/0098ecd0/20261018T215553239483Z/receipt.json`.
Prototype preserved in lifecycle-installer-native/20261018/first. Registered in
gate_all, which has not been run in full. Final six new worker receipts checked
with zero input drift; audit at 0098ecd0/20261018/lifecycle-receipt-audit.json.
Pending removal remains controlled, timer fixed, cold UID key erase fail-only;
retained objects are freed by actual ThreadMain exit. Raw owners and owned
synthetic files/threads/events only; no installed game/save/Lua modifications.


### Real performance clock in the installer/worker composition

The latest installer gate removes the fixed timer boundary: recovered GFGetTime
009E1BC0 is freshly exact (168B,14 fixups), with its real loader-zero state and
actual QueryPerformanceCounter/Frequency initialization on the owned IO thread.
High-resolution initialized state, positive frequency and returned time are
observed. WinMM timeGetTime fallback is linked/import-checked but not exercised.
All 672 native IO/finish/cleanup/shutdown cycles still pass; now 16 provider
omissions plus the three runtime controls. Latest receipt:
`rebuild/build/native-worker-installer/00/98/0098ecd0/20261018T215830358448Z/receipt.json`.

VC71 distribution lacks winmm.lib. First link failure is retained, followed by
an isolated compatibility link proving Windows SDK 10.0.26100 x86 winmm.lib
works with the same VC71 linker. The permanent driver now uses and hashes that
specific import library; compiler/linker remain VC71. No installed toolchain or
system library was changed. Earlier fixed-clock receipt remains historical.
No new recovery credit for the previously recovered clock function/data.


### Actual manager owner, original CRT startup and map destruction

`gate_native_threaded_worker_owner.py` passes 672 lifecycles with actual
CThreadedFileManager construction/destruction. Constructor allocates 15 distinct
real map headers, initializes Windows section/event and starts its own CRT
_beginthread with actual BeginThread/ThreadMain/ProcessObjects. Initial headers,
counts/cursors and owner fields checked under lock. The fixture populates owned
pending/UID nodes under that lock and calls actual KeepUpdating. Worker handle
is duplicated on the IO thread and joined after actual Uninitialise. Actual
ThreadMain frees retained requests/OVERLAPPED, actual deleting destructor frees
UID nodes and all headers. Existing installer Get/Pause/Unpause, real clock,
APC/finish/unlocked callback and output checks stay active.

All 336 reads/336 writes, 336 callbacks, 672 exits/joins pass. 23 link omissions
and no-op entry/Pause/Unpause controls detected. Constructor/destructor stay
accepted near; KeepUpdating freshly exact. No engine source changes or new
recovery count. Current receipt:
`rebuild/build/native-worker-owner/00/98/0098ecd0/20261018T220418939939Z/receipt.json`.
Prototype attempts retained in lifecycle-owner-native/20261018/first..fifth:
missing node-erase filename, then real UID helper/link closure, then extrema,
then a raw-fixture count assertion. Final links use real UID erase/rebalance/
extrema dependencies; retained path does not execute key erase/rebalance.

The constructor exposed the raw-fixture pending count at the wrong offset
(+8 comparator instead of +4 count). Corrected all five permanent native
fixtures and the process oracle, also making root/parent/extrema links valid.
Parent worker had never read that word, so engine code was unaffected. All six
prior worker gates refreshed successfully with corrected fixtures; fresh build
stamps start 20261018T220242. Historical snapshots retained. Seven current
receipts now have zero hash drift in owner-final-receipt-audit.json.

Remaining boundaries: insertion and pending removal are fixture operations;
installer owner is raw and acknowledgment pre-signaled, constructor cold paths
trap; test callbacks/owned synthetic files. No actual AddObject or pending-remove
recovery, production installer worker, game/install/save/Lua modifications.
No full gate_all run claimed. Gate registered there.


### CloseAllStreamsForFile 0098E7D0: deferred at integration gate

New scratch first source reached 820B/283 instructions versus original
840B/290, NEAR_MATCH90.0524%, and passed 604 original/rebuilt cases with actual
vector overflow/pending erase/UID rebalance/extrema/both rotations (13 providers).
Corpus includes up to63 requests, status0..5/mixed, two devices, target-file
filtering, busy completion polling, failed GlobalFree, wrapping block counter,
callback self-deletion/other captured-UID deletion and RB/storage/ABI checks.
Initial scaffold used truncated838B boundary; corrected to actual840B RET4
extent, retaining both reports. Helper/page setup failures are retained too.

Whole-object link then exposed a duplicate unsigned-long vector overflow
specialization: opt-in generic vector body emitted its own copy alongside the
existing out-of-line provider. Second source explicitly declares that existing
specialization, fixes the duplicate and links cleanly, but parent scheduling
changes to816B/283ins89.7033%, below90%. Both forms and link artifacts retained
under 0098e7d0/20261018; deferred.json records rejection. No canonical source,
index entry or accepted-function credit. Two source forms tried; stop tuning
until new original evidence. This is distinct from deferred pending remove
0098E770 and AddObject0098F0F0, which remain untouched.

The 604-case result does not override the failed admission gate. CloseAllStreams
remains the child boundary in accepted wrappers and is not claimed recovered.


### Six exact manager error-policy APIs and callback state

New bundled canonical source ErrorCallbacks_00401460 recovers six exact first
forms: public fastcall SetErrorCallback00401460 (7B), SetHardDiskErrorCallback
00401470 (7B), SetDirtyDiscError0098DED0 (7B), DoHardDiskErrorCallback0098F9B0
(12B), protected GetHardDiskErrorCallback0098DEE0 (6B) and protected unchecked
CallHardDiskErrorCallback0098DEF0 (6B). Independent FableWin access/signatures
and original data references corroborate identity. New private m_errorCallback
013BC860 is4B writable loader-zero BSS without payload/relocations. Existing
hard-disk/dirty data providers retain ownership. Shared aliases do not change
layout. Seven symbols uniquely indexed at actual physical addresses.

`gate_native_threaded_error_policy.py` passes196 original/model/rebuilt cases
and6 omissions. Valid/absent callbacks, raw low-byte bool, opaque/null text,
callback mutation, fastcall registers and tail return/stack checked. Protected
unchecked null dispatch intentionally faults in the bounded emulator; public
guard returns. No invented null safety or native crash test.

Native ReadAPC fixture now runs all6 policy functions using typed public API
and a derived access fixture for protected methods, checking loader-zero error
word, setter/getter roundtrips, two native dispatches and guarded null return.
All original/rebuilt Windows reads and24 native controls per mode still pass;
real EOF hard-error callback installation goes through the recovered setter.
Fresh snapshot audits all6 policy bodies exact;6 required source link omissions
and no-op callback control detected. Current native read-APC receipt:
`rebuild/build/native-read-apc/00/98/0098e320/20261018T222242810364Z/receipt.json`.

Header-impact proof:133 existing TUs including compiler-local code/data and
fixups unchanged, one new TU. Initial comparator falsely rejected because it
compared JSON lists to in-memory tuples; immutable before/after receipts were
both loaded and compared correctly (zero differences), with no baseline
replacement/recompilation. Report preserved in error-callbacks/20261018/header-
impact/report.json. Six function/one data rows, dedicated gate and source-symbol
companions registered. Original metadata lacks these tiny function starts, so
accepted gate/index evidence is explicit; no generated dashboard rewrite.

All seven previous worker gates refreshed against the new shared declaration
and pass; current owner build is20261018T222444994747Z. Statistics/read callback
refreshes are running separately; no full gate_all run or extra helper credit.
No Lua, installed-game or save edits.

Policy follow-up: statistics12,600/read-completion3,456 refreshes now pass too. Eleven current threaded/native-policy receipts audited with zero input drift; error-callbacks/20261018/final-receipt-audit.json.


### Const UID lookup and manager/file query APIs

Five qualified canonical roots now pass6366 original/model/rebuilt cases and5
omissions. Manager IsObjectFinished0098E590 (95B/39ins) and file wrapper0098E630
(10B/2ins) are exact. Const UID find00990320 is71B/32ins93.75% near;
ObjectExists manager0098E5F0 is65B/24ins91.3043%, file0098E640 is74B/22ins90.4762%.
No tuning beyond passing near. Const helper's first emitted normalization form
was66B/28ins (instruction delta12.5%); direct standard lookup/iterator return
passes. Finished query's first literal-return form was88B/34ins; explicit bool
snapshot before unlock yields exact original. Failed forms retained.

Shared reduced STLport gains a const view, const begin/end/find and mixed
iterator comparisons without changing existing iterator names/layout. This is
local model compatibility, not recovery of retail internal template names.
[Archived STLport source](https://android.googlesource.com/platform/external/stlport/+/e64e7c849b0b659d3e198e4efaed563601e554be/stl/_tree.h)
was consulted for const-access semantics, not as evidence of retail version or
ABI. Original code supplies four-byte hidden return and the existing pointer-
header model. Compile contract checks4B size, mutable-to-const conversion,
comparisons and mutable pointed-to payload; const pair assignment and reverse
conversion must fail compilation.

Behavior covers unsigned keys/boundaries,0/1/3/7/15-node balanced/left/right
search shapes, all statuses, missing and mapped-null values, full64KiB/global
storage and ABI. Missing UID is finished; mapped null exists but finished query
faults exactly at+1C, preserving original behavior. Entry tree swap and exit
status/header/poison mutations check result snapshots and recursive lock balance.
File receiver is intentionally invalid to prove global dispatch ignores it.
First emulator layout let larger near ObjectExists overlap the next retail
entry. Rebuilt bodies now execute in independently relocated slots; originals
stay at original addresses and call/IAT identity audited. No engine-source edit
was needed for this fixture correction.

Strong before/after header proof:1121 existing TUs (tree/manager) unchanged,
all compile;81 file-header TUs unchanged;2 new TUs. Compiler-local sections and
relocation targets included, no rebinding of snapshots. Five unique symbol
owners/metadata rows and gate_all entry registered. Whole-object native owner
link requires cold file-wrapper global manager storage; its dedicated4KiB page
is PAGE_NOACCESS, so no second owner is constructed or used.

The actual owner probe now calls const ObjectExists/IsObjectFinished during
recursive setup, progress monitoring and missing-UID checks. All672 actual
IO/clock/installer/finish/exit/owner teardown cycles pass with25 provider link
omissions and4 runtime controls, including false ObjectExists. File global
wrappers are executed by the differential gate, not this native owner probe.
Current native receipt:
`rebuild/build/native-worker-owner/00/98/0098ecd0/20261018T224751811958Z/receipt.json`.
Rejected link (cold global missing) and old raw-status observer retained.
No full gate_all, actual pending insert/remove, installer worker or retail/save/
Lua modification claim. Other header-dependent receipts will be refreshed;
zero code changes in1121 old TUs already established.


### Request ownership restored from original C++ type evidence

FableWin CFileObject confirms public nonvirtual ctor/dtor and private copy
constructor/assignment; retail constructor0098DF00 initializes descriptor defaults
and Overlapped=0 only. Destructor0098DF20 frees its owned global block. The old
shared-header comment saying no destructor was wrong. Header now models this
ownership and disallows copies. RemoveObject, OnFinishObject and ThreadMain
remove their manual frees and rely on ordinary delete/destruction; all three
remain byte-exact. No duplicate free, no invented field clearing/free retry.

Three new exact roots: ctor30B/10ins, destructor15B/6ins and automatically
emitted nonvirtual scalar deleting helper0098F50039B/15ins. The helper is ordinary
VC71 compiler output, no authored opcode wrapper. It tests flag bit0, preserves
original return pointer and ignores other flags/free failure.280 original/model/
rebuilt cases with3 omissions pass; all16KiB storage, padding, null block and
free failure/flags/ABI covered. Positive64B/noncopy contract compiles; copy and
assignment must fail with private access. Initial scalar instruction-count
scaffold corrected from16 to original15; engine body unchanged.

Header proof:19 existing TUs preserve all named code/data and relocation
closures, one new ctor TU. New destructor/helper COMDATs are allowed additions
in the three deleting callers. Synthetic repeated section-name symbols and
unused local-label identities are not mistaken for function identities; full
raw/local receipts retained. Ownership index selects the destructor/helper
from OnFinishObject only, avoiding duplicate provider claims.

All7 native Windows probes refreshed and pass with actual constructor linked.
Stack-context fixtures explicitly null their borrowed Overlapped before the
real destructor runs; heap-owned requests are freed by actual ThreadMain as
before. Current owner672-cycle receipt is
`rebuild/build/native-worker-owner/00/98/0098ecd0/20261018T231139961187Z/receipt.json`,
now26 source omissions/four controls. ReadAPC has7 omissions; direct worker4,
thread7, finish/exit8, installer17. No extra native functional credit for those
reused callers. Lifetime gate and updated expected counts registered in gate_all.

Manager removal parity, worker exit, queries6366, policies196, worker960 and
read completion3456 refreshes pass. Statistics12600 refresh remains in progress.
Thirteen current receipts audited with zero input drift; object-lifetime/
20261018/current-receipt-audit.json. No full gate_all, AddObject/pending removal
retry, game/save/Lua change. Additional raw PDB qualifier/access evidence saved
in pdb-evidence.json; LoadingStatus/Overlapped are nonvolatile in that build.


Four public file leaves recovered: IsOpenForWrite0098DF40, PeekDeviceID0098DF50,
PeekPhysicalSortKey0098DF60 (each4B/2ins), KeepUpdating0098E1C0 (23B/7ins,
three checked global/IAT fixups), all exact. Same TU as existing manager update
pair; both retain exact parity. Device enum moved verbatim to its own shared
header to support the real public enum return type without manager includes.
Strong header audit:195 existing TUs unchanged, four expected new functions.
PDB method evidence saved; FableWin's different CWideString size/layout is NOT
used to replace retail offsets. Retail getters read +18/+4/+14 respectively.

Permanent file-accessors gate:870 original/rebuilt cases, four root omissions,
unaligned storage, full8KiB memory guards, all256 raw bool bytes, unsigned
patterns, preserved registers/stack, null ignored file-this for global wake,
SetEvent/SleepEx order and ignored results. SetEvent clobbers caller-saved
registers and changes the manager event before SleepEx. APIs remain controlled.
Gate registered, four unique symbol owners indexed and correction/artifact rows
added. Work file-accessors/20261018 retains first object/parity and before/after
header evidence. Manager update120 cases pass; actual-owner Windows probe672
cycles passes again at20261018T232054648672Z,26 omissions/four controls.
Statistics12600 refresh from prior lifetime phase also passed. No full gate_all.


ResizeFile0098E090 recovered from adjacent retail instructions and public PDB
signature: first natural source62B/25ins versus58B/23,91.6667% opcode similarity,
8.6957% instruction delta, NEAR_MATCH. No second source form or further tuning.
Three exact-prefix zero-addend IAT fixups independently identify SetFilePointer,
SetEndOfFile, GetFileSize. Ignores first two returns; reloads live handle each
call, stores final size and returns equality, including requestedFFFFFFFF when
GetFileSize fails. Source: CThreadedFile_ResizeFile_0098e090.cpp. One new native
root;85 old file-header TUs unchanged, one new TU. Work0098e090/20261018 retains
first source/object/grade and complete before/after header proof.

Permanent emulator gate480 cases: unsigned/signed boundary patterns, final
matching/different/error sizes, ignored API failures, each/both live handle
changes, GetFileSize side effects, full16KiB storage and ABI. Missing root and
no-op controls detected. Permanent Windows x86 probe passes20 cases across
original private58B blob and fresh compiled62B body:12 bounded truncates/extensions
and8 null/invalid-handle cases. File pointer/size/preserved prefix and raw96B
file-context guards verified. Three original host IAT cells checked; real
Win32 imports, frozen sources/includes/toolchain inputs, one root link omission
and false-return control. Receipt:
rebuild/build/native-file-resize/00/98/0098e090/20261018T232411667529Z/receipt.json.
Only the gate-created file is modified. No file ctor/Open, manager or game runs
in this probe. Gates registered; unique symbol/index and correction/artifact
owner added. No full gate_all or Lua work.


OpenForWrite0098E000 recovered: first ordinary source is relocated-exact143B/
44ins/eight fixups. PDB public signature const CWideString&,bool; original
CreateFileW disposition2 for true (overwrite),4 for false (open-or-create),
write access/share0/flags68000000. It converts the path, sets device0, stores
the returned handle and rejects onlyINVALID_HANDLE_VALUE, records GetFileSize,
allocates next physical key, sets both flags and destroys the local path.
Failure preserves old size/key/mode flags; both paths preserve stored filename.
[CORRECTED below at 013228 UTC: this shared-counter assertion was false.]
Same TU and original local zero-BSS4B counter013BC9E8 as read-open; both actual
functions have exactly two named references to that same counter. Local source
name is not claimed as original PDB variable identity.

Permanent gate864 cases with full20KiB storage/thiscall guards: raw bool bytes,
failure/null/live handles, final0/17/FFFFFFFF sizes, counter0/7FFFFFFF/FFFFFFFF,
path/CreateFile/GetFileSize/destructor mutations and publication order. Missing
root/no-op controls detected. Strong86-TU audit preserves all previous named
code/data and relocation closures, adds only new write-open function. Existing
read-open native fixture also rebuilt/passed. No source tuning beyond first form.

Owned Windows x86 probe24 cases across privately relocated original and fresh
compiled143B body:16 successful opens/eight missing-parent failures, both
initial mode flags and overwrite modes, existing/absent files, whole96B context
including retained filename/padding, physical-key progression and payload checks.
CreateFileW/GetFileSize/CloseHandle are real; string/path methods reuse established
controlled recorders. Legacy included fixture main/fixed-temp helper is cold;
new main uses only the gate's unique owned filename/missing-parent path. One
link omission and false-return control pass. First probe compile correctly
rejected untyped overloaded helper address; explicit typed pointer fixes test
only, failed frozen build retained. Receipt:
rebuild/build/native-file-open-write/00/98/0098e000/20261018T232933137939Z/receipt.json.

Both permanent gates registered. Source index now selects its two verified
open roots with correct addresses/profile, removes four old incidental inline/
literal symbols mislabeled as0098E1E0; index-before/after evidence retained.
One new native engine root credited. Correction/artifact row added. Work
0098e000/20261018 retains before/after/first object/code/grade. No game, manager,
file constructor, exception-unwind, or Lua claim; no full gate_all.


File lifetime continuation: original one-slot vtable0129A158 points to00991450,
not the constructor address incorrectly attached to the prior generated helper
in the source index. First natural destructor `Close();` appended to the real
Close TU generates an exact88B/27ins scalar deleting body after seven named
fixups. Initial raw opcode score92.59% was an incomplete relocation-mask result;
full relocation equality is authoritative, no source tuning. Ordinary destructor
found at0098F090 by relocated prefix search, then independently matched full67B
and six fixups. Constructor0098DFD0 remains exact41B/three fixups. No constructor
source/header change. These are two new engine function roots, constructor reused.

Vtable COFF contains weak external vector-name reference, SEARCH_LIBRARY2,
fallback to actual scalar helper; raw auxiliary index/default audited. No second
vector body invented. Constructor index now owns ctor and table(correct0129A158),
Close TU owns Close, ordinary destructor0098F090 and deleting helper00991450.
Wrong ctor-address helper owner removed. Before/after index receipts retained.

Permanent lifetime gate512 original/model/rebuilt cases:8 constructor,432
scalar deletion,72 ordinary teardown also checked against original scalar flag0.
All roots exact. Three omissions and six missing/no-op controls; full20KiB
storage, constructor callback publication, only-1 handle skip(null is attempted),
manager callback handle reload including replacement-1, ignored CloseHandle
failure, flag bit0/deallocation, member/base cleanup order, vptr changes and ABI.
CFileObject request lifetime work is separate. Close's old144-case/360-checkpoint
gate still passes exact47B; its cold link adapter gained explicit string/base
lifetime stubs for new TU dependencies, link EXE remains unexecuted. Initial
scaffold corrections and source newline check retained under file-lifetime/
20261018. No native OS lifetime probe yet, no production manager or Lua claim.


Native file lifetime OS proof now passes52 cases:48 direct original/rebuilt
constructor+deleting paths across six flag words/four handle policies, plus4
actual C++ virtual deletes through the linked vtable. Real owned Windows
handles and CRT malloc/free through a guarded deletion adapter; manager/string/
base lifetime remain controlled. Each owned handle closure is checked using
GetHandleInformation; replacement leaves original handle live and closes the
new one. Skip-1/null cases and whole96B guards/cleanup traces verified. No leaked
allocations/handles; read-only1234B synthetic file remains unchanged.

Gate audits all three linked bodies at exported host addresses, masks exactly
checked zero-addend relocations and requires actual table target==scalar body.
This explicitly prevents the constructor TU's semantically equivalent30B
out-of-line deleting COMDAT from silently winning over the exact88B provider.
Canonical lifetime TU precedes constructor in this probe; future linked parity
must preserve/verify this selection too. Constructor vtable's weak vector-name
fallback resolves to scalar at runtime. Two source omission links and an empty
Close/destructor runtime control detected. Receipt:
rebuild/build/native-file-lifetime/00/99/00991450/20261018T234247572154Z/receipt.json.
Gate registered. Index's older Close-only filter initially suppressed newly
admitted destructor rows; corrected it and verified both unique address/owners;
incomplete first index-after receipt retained separately. No source-form retry.


Post-file-lifetime refresh: all7 earlier native Windows callback/worker/installer/
owner probes pass again using current shared headers. Current owner672-cycle
receipt20261018T234407554455Z (26 source omissions/four runtime controls).
New field870, manager update120, resize480, write-open864 and file lifetime512
emulator gates refreshed/pass. Resize OS20 and write-open OS24 refreshed with
current input hashes (20261018T234507822178Z /234507820823Z). Lifetime OS52
unchanged current receipt234247572154Z. Sixteen selected current receipts audited:
1822 input/compiler/object/linked hash checks, zero drift; file-lifetime/
20261018/current-receipt-audit.json. This is a focused native checkpoint, not a
full gate_all run or a claim that every older historical receipt is current.

Next continuation started: strengthen owned write-open proof by linking actual
CWideString construction/copy/assignment/unassign/allocation/destruction and
its native instance counter, leaving only absolute-path resolution controlled.
No new string root credit planned; reuse existing canonical providers. No Lua.


Recovered wide-string ownership is now composed into native write-open:
NativeThreadedFileOpenWriteStrings_test.cpp and permanent gate_native_threaded_
file_open_write_strings.py.96 real Windows original/rebuilt cases:48 opens/
48 expected failures over existing/absent/missing-parent/empty paths, overwrite
modes, initial flags, and0/1/3 aliases. Actual default/literal/copy construction,
assignment, UnassignString, AllocStringData, destructor, wchar conversion and
STL range construction execute, with actual canonical zero-BSS instance counter.
Nine reused string function roots independently relocated-exact596B total;
parent143B exact, all10 linked exported bodies additionally checked. No new
engine/string-root credit. Original CRT import thunks checked by name; shared
empty wchar word matches original2B fixture boundary. Length-error provider
linked cold; narrow read-open helpers are explicit cold traps.

104 CWideStringData allocations/releases tracked through checked operator new/
delete adapters backed by real CRT malloc/free. Actual STL string buffers use
real CRT allocation/free. Name/alias refcounts and instance counter return to
zero after every case; alias replacement executes real unassignment; input
filename/raw96B file context/padding, sizes, preserved payload and physical key
progression verified. All11 required provider omissions fail and false-return
parent control detected. Absolute-path conversion remains an explicit identity-
copy boundary using the real copy constructor. No manager/file owner/game/save/
exception-unwind claim. Receipt:
rebuild/build/native-file-open-write-strings/00/98/0098e000/20261018T235314519872Z/receipt.json.

First frozen scaffolds retained: direct private-field access correctly rejected
by VC71 (fixture now inspects4B object representation with memcpy); missing
cold CBasicString<char> equality supplied as a typed abort trap. Initial functional
receipt235124354124Z explicitly says helper audit pending; final above carries
nine exact helper audits and linked-body verification. Earlier source forms
unchanged; helper inspection retained in first functional build. Gate registered.


Path continuation found missing CWideString::GetSubString const provider at
0099BDB0 from original ExpandPath call/PDB signature. Original179B body passes
end-start to AllocStringData, no length clamp. First178B natural SetAsSubString
form misses near72.31%; second direct fresh-data fill203B/88.57% and12.12%
instruction delta also misses. Both retained in file-path/0099bdb0/20261018;
deferred.json, no canonical/index/count credit or further source tuning.
Current header parameter spellingcount is not semantic evidence for this root.

Actual path resolution composition can still cover absolute/plain-relative/
rooted paths using recovered ConvertPath/ExpandPath/current-directory/cache,
while unexercised dot/parent branches retain explicit cold traps. ExpandPath
baseline fresh474B equals original extent; full relocation audit pending.


Actual path-resolution composition now passes192 Windows original/rebuilt
write-open cases:96 successful opens/96 expected failures, absolute and plain
relative inputs, existing/absent/missing-parent/empty names, overwrite/initial
flags and0/1/3 aliases. Actual ConvertPathToFullPath41B, ExpandPath474B,
GetCurrentPath132B, CalcCurrentPath73B(/GS), process directory read and canonical
4B cache object/instance counter execute. Explicitly compare resolved absolute
names before every open, so an identity-resolver control fails on relative
input. Whole96B file context, filename/padding, size/payload and key progression
remain checked.833 data allocations/releases balance, including cache fill then
clear; one legitimate empty process-lifetime cache instance remains until exit.
Original/rebuilt modes allocate equally after cache warmup. No working directory
or installed-game state is modified; each process cwd is its unique build dir.

Twenty reused helper roots plus parent are relocated-exact. Two existing
operator+ COMDAT variants pass near: MakeOwnCopyOfData124B vs128B,94%/3.92%
instruction delta; wide append104B vs104B,92.857%/zero delta. No source tuning.
Independent variant oracles compare original and compiled instructions with
full16KiB models and ABI:112 copy-on-write cases (actual original range/unassign
bodies, controlled successful nonmutating CRT allocations),576 append cases
(controlled typed child contracts, self alias/null/empty source/owner wrapping/
live replacement). Two controls each. All23 actual linked exported bodies are
checked against their provider bytes with precisely named relocation masks.
Security-cookie binding is checked against original defaultBB40E64E and actual
cmp-cookie/conditional-failure helper bytes, source profile/GS pinned. CRT import
thunks and literal words checked, including separate empty-wide addresses for
operator conversion vs current-path literal.

Whole-object link initially exposed duplicate strong allocator definitions in
standalone AllocStringData and operator+ literal TU. Their121B code and named
fixups are identical: standalone object retained as parity reference, only the
complete operator+ TU is linked. No canonical code/header edit, object stripping
or forced duplicate suppression. Other interchangeable COMDATs disclosed as
redundant source omissions.17 required source omissions fail, plus two native
runtime controls(parent false, identity path resolver). Two variant controls
are additional. No new engine/string/path function credit. Rooted/dot/parent
and narrow read-open paths remain explicit cold traps; length error linked cold.
No constructor/manager/exception-unwind or all-path claim in this probe.

Permanent files: NativeThreadedFileOpenWritePaths_test.cpp,
gate_native_threaded_file_open_write_paths.py, wide_string_own_copy_variant_oracle.py,
wide_string_append_variant_oracle.py. Registered in gate_all. Frozen intermediate
functional receipt explicitly marked path-helper-audit pending; source-only
fixture failures/pin corrections retained. Final receipt:
rebuild/build/native-file-open-write-paths/00/98/0098e000/20261019T001510208565Z/receipt.json.
New UTC day, still local October18 session. Substring0099BDB0 stays deferred after
two failed forms, no canonical promotion. No Lua or full gate_all.


Character append0099B9A0 and its missing shared support admitted. First natural
source205B/69ins vs203B/71: NEAR92.857%,2.817% instruction delta, stop tuning.
Append to existing data always adds one code unit, including NUL; an empty
CWideString stays empty when appending NUL. It calls copy-on-write first, reloads
live data, checks max/capacity, reserves growth, calls fill with zero remaining
count, terminates and advances finish. Rooted drive creation uses its empty case.

Generic counted append/reserve added to the canonical shared basic_string
header. Reserve0099C990 is exact152B/63ins/four named fixups on first form,
always allocates max(request,size)+1 and frees old storage, even shrink requests.
Wchar fill005123A0 exact32B/14ins/no fixups; returns the advanced end pointer and
reloads value per write. Uses existing reduced-model uninitialized_fill_n<T>
family, explicit wchar specialization; no claim of original template spelling.
The first scratch generic loop was made a specialization for ODR correctness,
not byte tuning; exact32B unchanged. No naked/asm/opcode source implementation.

Permanent gate1756 original/model/rebuilt cases:784 append,804 reserve,168 fill,
three missing-root and three no-op controls. Full32KiB model/ABI: embedded NUL/
word values/dirty argument high bits, copy replacement, real original/rebuilt
reserve and fill child instructions, tight/spare capacity, shrink, source reload
at controlled allocation, free-callback field mutation, unsigned large requests,
expected null-allocation faults and length-error termination. Huge requests are
bounded virtual allocator/pointer-metadata probes, not actual allocations. Raw
null/unaligned overlapping fill probes are bounded ABI stress. Copy-on-write and
fresh data creation controlled here; no new native OS proof yet. Two model fixes
(no free callback for absent old buffer; fill returns end) retained in work.

Strong header audit:1600 existing TUs compile and preserve all named code/data/
relocation closures, three new TUs; no old emission changes. Scratch sources,
objects/grades, specialization proof, complete before/after objects, raw PDB
method evidence and new symbol owners retained in wide-string/0099b9a0/20261018.
Three unique canonical source/index/correction/artifact roots registered. Existing
manifest mislabel at0099B9A0 is overridden; generated dashboards left alone.
Gate registered; no full gate_all, no Lua. Next: native rooted-path composition
plus direct hot character append/reserve/fill on real owned string buffers.


Native rooted-path proof now passes. NativeThreadedFileOpenWriteRooted_test.cpp
and gate_native_threaded_file_open_write_rooted.py extend real path/string/cache
composition with the three newly admitted character/reserve/fill providers.
288 original/rebuilt write-open cases:144 opens/144 expected failures, absolute,
plain-relative and drive-rooted paths, retained file context/filename/padding,
mode flags, payload/size/key checks, aliases and real directory cache.

Additionally128 original203B/rebuilt205B direct character-appender cases use
real CWideString storage, null/one/multi-character inputs, four code units
including NUL/surrogate/FFFF, shared/unique data and tight/spare capacity. Check
reference return, explicit length including embedded NUL, terminator and old
alias preservation. Six actual reserve checks prove same-capacity requests
still allocate before freeing and reserve0 shrinks to current length;16 actual
fastcall fill calls check end pointer, every word and both guards. Original
character blob loaded privately with all five named targets checked. First
fixture compile needed a typed function pointer to disambiguate the template
helper; frozen failure retained, engine source unchanged.

All26 linked bodies checked:23 exact/three passing near (two reused operator+
COMDATs and the new character appender).17 prior required source omissions grow
to20 with new providers; three runtime controls detected(parent false, identity
path resolver, no-op character appender), plus688 variant emulator cases.
Allocator/reference-only TU handling unchanged, no stripping/force-multiple.
Data blocks and instance counters balance per scenario; actual cache data is
released before main exit, one empty global cache instance then destructs at
process exit. Native rooted composition adds no function credit beyond the
three separately admitted roots. Dot/parent/narrow read-open branches remain
cold traps; no file constructor/manager/game/save/exception-unwind claim.

Final rooted receipt:
rebuild/build/native-file-open-write-rooted/00/98/0098e000/20261019T004009212907Z/receipt.json.
Permanent gate registered. Shared-header audit1600 old TUs unchanged; component
1756-case gate current. Other older receipts predate that header extension;
retain as historical phase evidence until specifically refreshed. No full
gate_all, no Lua, no deferred substring retry.


Rooted write-open now composes actual CThreadedFile construction and destruction.
Permanent NativeThreadedFileOpenWriteLifetime_test.cpp and gate_native_threaded_
file_open_write_lifetime.py pass720 Windows cases:360 opens/360 expected failures,
144 real virtual deletes,432 checked owned-allocation deletions,360 manager-close
boundary calls. Original mode executes private41B ctor/143B write-open/88B deleting
blobs with every named fixup checked; rebuilt mode uses canonical native methods
and actual linked vtable dispatch. The manager stream-cleanup method and base-class
lifetime remain explicit controlled boundaries; wide-string ctor/assignment/dtor,
filename ownership, path/cache and Windows close behavior are real.

Every file receives the input name by actual assignment after construction, keeps
its filename/name aliases through the open, then releases it during actual file
destruction. Both deletion flags0/1 execute on original/rebuilt paths; rebuilt also
uses virtual delete. Whole96B allocation/guards checked before open and after
cleanup (snapshot before free for deleting cases), handles proved invalid after
successful cleanup, preserved payload checked after close, all alias refcounts
restored.4192 data allocations/releases balance; cache storage released before
exit. Manager sentinel storage unchanged. Earlier128 character/six reserve/16fill
and688 variant comparisons remain in this composed gate.

All29 selected linked function bodies verified:26 exact/three prior near variants;
file vtable points to actual88B deleting body, not constructor's30B alternate.
22 required source omissions, four runtime controls(parent false/identity path/
no-op character/no-op lifetime) detected. Gate registered. No new function credit,
canonical engine/header changes, full gate_all, Lua, game or installed-data writes.
Receipt: rebuild/build/native-file-open-write-lifetime/00/98/0098e000/20261019T005424395388Z/receipt.json.

Frozen fixture failures retained: a two-argument PutWrite call failed compilation;
initial payload read correctly met Windows sharing violation while write handle
was live, now checked after cleanup; base marker initially overwritten by compiler's
implicit root-base destructor, now written in the controlled root-base destructor.
No engine source adjustment or deferred-substring retry. Next continuation can
replace the base-class lifetime boundary with existing canonical base providers,
then audit linked base bodies and retest the owned chain. Manager stream cleanup
still deferred at its existing acceptance/ODR boundary.


Owned rooted file lifetime now uses actual canonical CBaseClass/NonCopyable TU.
All720 Windows cases,144 virtual deletes and432 owned frees pass again,4192 data
allocations/releases balanced. Actual noncopyable ctor0099A2F0 and dtor0099A300
execute on both original/rebuilt file paths; final real CBaseClass vtable is checked
before each free and after retained-allocation destruction. All three linked file/
noncopyable/root vtable targets checked. Manager stream cleanup remains controlled.

Five additional emitted base bodies relocated-exact, two hot and three cold.
Root scalar deleting helper004371F0 is a cold compiler variant:31B inlined destructor
vs30B original call-through. Initial audit correctly rejected treating it as exact;
no canonical tuning. New base_class_delete_variant_oracle.py passes256 independent
original/model/compiled cases, flags/unaligned objects/full4KiB memory/return-this/
callee saves and two damaged-code controls, with original7B root dtor and controlled
mutating delete callback. It is FUNCTIONAL_COLD_VARIANT, not a new accepted root.
All35 linked bodies checked:31exact/three existing near/one bounded cold variant.
23required source omissions and five native controls, including damaged base dtor,
pass. Four other base bodies stay cold in the native file chain. No engine/header
changes, coverage credit, full gate_all, Lua or game launch.

Current receipt supersedes controlled-base phase:
rebuild/build/native-file-open-write-lifetime/00/98/0098e000/20261019T011541374607Z/receipt.json.
Earlier005424receipt is frozen historical phase evidence; current fixture/gate now
include real bases. gate_all marker updated. Next inspect owned ResizeFile integration
or a nondeferred missing read-path provider; do not retry deferred substring/manager
CloseAllStreams forms without new evidence.


Owned native open/resize/lifetime composition passes on first build/run.
NativeThreadedFileOpenResizeLifetime_test.cpp and permanent gate add actual
ResizeFile to the720-case real path/string/base/file lifecycle.4320 resize calls:
2160 on opened handles and2160 on failed-open INVALID_HANDLE_VALUE. Six requests
per file cover shrink/grow/repeat/unaligned/zero plus failed-handle0xffffffff.
Actual volume sector query reports512B; this Windows/filesystem accepts all360
unaligned seeks/resizes in this corpus. No assumption that unaligned seeks always
fail: original/rebuilt outcomes compare return, stored/physical length, position
and last-error per scenario; all96B context/guards and ownership checked after each
operation. Final payload length and retained preexisting prefix checked after close;
newly extended bytes are not assumed zero. No unsupported arbitrary large write.

180 failed-handle calls intentionally return true for requested0xffffffff because
GetFileSize returns the same error sentinel. Preserve this retail behavior; the
routine ignores seek/end errors. Original58B resize privately loaded with three
checked IAT cells; compiled62B existing near provider linked and inspected. Actual
OpenForWrite supplies its original write-only/exclusive/overlapped/no-buffer/write-
through flags; no manual substitute open handle. Full720 original/rebuilt lifetimes,
144virtual deletes,432heap deletes,4192string allocations/releases remain balanced.

36linked bodies checked:31exact/four existing near/one cold functional base variant.
24required source omissions and six native controls pass, including no-op resize;
all944 prior helper-variant comparisons retained. Current receipt286 input/object/
linked hashes rechecked, zero drift. No new function credit, canonical source/header
change, full gate_all, Lua/game/install/save writes. Gate registered. Receipt:
rebuild/build/native-file-open-resize-lifetime/00/98/0098e000/20261019T011807958169Z/receipt.json.
Next inspect actual counted file ownership and its DeleteData provider; manager
cleanup/dot-parent normalization remain the same controlled/deferred boundaries.


Typed counted-file DeleteData009A9040 replaces the historical generic Obj/
DeleteIfSet body and print-only test at the same source/test paths. Retail Reset
009A9C80 embeds callback009A9040 at immediate009A9CD0; FableWin02FD5CF0 names
?DeleteData@?$CCountedPointer@VCThreadedFile@@@@SIXPAX@Z. First typed delete source
using the existing shared header is exact11B/no relocations.48native controlled
virtual-slot cases and256 independent original/model/rebuilt cases pass: null,
unaligned objects, two virtual targets, flag1, full4KiB guards, stack/callee-save
ABI and callback object/table mutation with volatile poisoning. Omission and native/
emulator no-op controls pass; selected linked11B body checked.

Permanent gate_threaded_file_counted_delete.py registered, catalog module/pass
pattern and correction/artifact owner updated. Source index refresh changes exactly
one removed generic @DeleteIfSet@4 to one typed symbol; all other40622 rows retained.
No header changes or new root credit. Frozen old source/test and evidence/index diff
in work/native-recreation/threaded-file-owner/009a9040/20261018/typed-first. First
metadata note miswrote immediate009A9CCF; v2 corrects to009A9CD0 and gate checks bytes.
Receipt: rebuild/build/native-threaded-file-counted-delete/00/9a/009a9040/20261019T012158637996Z/receipt.json.
Next connect real counted Reset/12B control allocation/shared release/final virtual
deletion to the owned native file chain; no Lua, full gate_all or game launch.


Actual counted-file ownership now composes native path/open/resize/destruction.
NativeThreadedFileCountedOwner_test.cpp and permanent gate pass1008 Windows file
cases (504opens/504failures),6048 real resize calls,720 actual file heap deletions
including144 direct virtual deletes and288 counted-owner final releases. Real
Reset009A9C80 is relocated-exact103B; private original Reset checks its three named
fixups and uses the same actual typed11B DeleteData009A9040. All288 real12B control
allocations/releases balance via guarded CRT adapters.192 explicit shared-owner
setups release an intermediate reference through original/rebuilt Reset while
retaining live handle/filename/control data; final Reset invokes real virtual file
destruction before freeing the zero-reference control. No recovered copy-API claim:
sharing setup assigns public pointers and increments RefCount in the fixture.

Full40B owner storage,44B control allocation guards,96B file storage, physical size/
payload/cursor, filename refcounts, singleton sentinel and final empty cache checked.
5800 string-data allocations/releases balanced. Three inherited direct/deleting
lifetime styles retained, plus counted Reset on both original/rebuilt paths; only
empty owner destructor executes afterward.252 invalid-handle0xffffffff resize calls
return true as retail does. All38 linked bodies checked:33exact/four near/one cold
functional base variant.26required source omissions/eight native runtime controls
including no-op Reset and no-op DeleteData pass;944 helper-variant cases retained.
292 current receipt input/object/linked hashes audited with zero drift.

Registered gate. Existing typed callback recovery is reused, not recounted; no
additional canonical engine/header changes, full gate_all, Lua/game/install writes.
Manager stream cleanup stays controlled, dot/parent path normalization cold. Receipt:
rebuild/build/native-file-counted-owner/00/98/0098e000/20261019T012428391280Z/receipt.json.
Next inspect narrow read-open path dependencies to extend actual ownership into reads;
preserve all previous two-attempt deferrals unless fresh original evidence resolves them.


CORRECTION: read/write physical sort-key counters are separate in retail.
Original OpenForWrite0098E000 operands+107/+117 address013BC9E8; original read
Open0098E1E0 operands+265/+275 address013BC9EC. Both original words are zero-BSS.
The earlier1252-line shared-counter assertion checked only two COFF references per
function, without verifying read operands against retail. That test was circular
and its claim is withdrawn. The one C++ TU incorrectly shared its write counter.

Preserved old source and old-object mixed-sequence failure in
work/native-recreation/threaded-file-counters/0098e1e0/20261018/separate-first.
Old object fails the first read after one write. Source now retains the existing
write variable and adds a distinct read variable; only Open increments the new one.
No header change. Full143B write and310B read bodies now relocated-exact with both
BSS identities, every call/data relocation and both original counter words checked.
Permanent native counter gate executes1024 six-operation publication sequences /
6144 blocks original/model/rebuilt, four independent starting-counter pairs,
wraparound, zero/FFFFFFFF size and full8KiB storage/ABI guards. Explicit shared-
counter control fails. This is bounded post-GetFileSize code, not full-call/OS proof.

Corrected write-open oracle validates both original operands and distinct COFF
storage rather than assuming sharing. Fresh864 write cases pass. Owned counted-
file gate freshly passes1008Windows/6048resize cases with26omissions/eight controls.
Counter receipt: rebuild/build/native-file-open-counters/00/98/0098e1e0/20261019T013228515890Z/receipt.json.
Current counted receipt: rebuild/build/native-file-counted-owner/00/98/0098e000/20261019T013228531236Z/receipt.json.
Other older source-pinned receipts are historical until refreshed; no claim of full
gate_all. No new root credit, Lua or game launch. Continue actual read-path string/
SplitPath integration; its native read-after-write sequence will also guard this fix.


Actual read-open now joins the native owned path/string/base/counted-file chain.
NativeThreadedFileReadPaths_test.cpp and permanent gate pass1152 total Windows file
cases:1008 previous write lifetimes plus144 read lifetimes (36opens/108failures).
Both caching flags, absolute/plain-relative/drive-rooted names, existing/absent/
missing-parent/empty inputs and0/1/3 name aliases execute real SplitPath168B, wide
literal assignment92B, ToCharString177B, GetAsUpperCase277B near and actual narrow
storage/constructor/allocator/unassign/destructor/equality. Actual process-lifetime
narrow instance counter returns zero.28 direct original/rebuilt narrow/upper cases
include empty/ascii/high code units/high narrow bytes and low-byte NUL truncation;
input strings and owner counts preserved. Original read310B, narrow177B and upper
278B blobs have all21/4/12 named fixups checked. No new helper function credit.

36 real ReadFile/GetOverlappedResult transfers use aligned owned buffers, explicit
OVERLAPPED/event, sector-sized requests and exact payload comparisons. The complete
read Open method runs after hundreds of writes and starts its independent key at1.
Old shared-counter source is a detected native runtime control; the prior counter
fix now has whole-call Windows evidence in addition to bounded publication blocks.
All144 read owners release through actual counted Reset -> typed DeleteData ->
virtual file destructor -> real string/base/CloseHandle.432 total control blocks
and864 file heap deletes balance. Manager stream cleanup remains controlled; no
queue submission/APC manager chain or production installer worker claim here.

Total 6400 wide data blocks, 336 narrow data blocks, 336 narrow arrays and432
control blocks allocate/release equally; actual narrow arrays use checked malloc/
free adapters because CRT new[] delegates to overridden scalar new. Original first
allocation scaffold correctly trapped array size outside typed16/17/12B data blocks;
separate array adapters fix the fixture, engine sources unchanged. Initial whole-TU
link caught five duplicate strong helpers. Full code/named-relocation equality is
proved for ConvertPath, wide length/array and narrow unassign/dtor; whole containing
TUs supply those bodies, standalone providers remain parity-only. No stripping or
force-multiple. Existing allocator-parity-only handling retained. Frozen failures
and early functional-only phase receipts retained; final adds five read controls.

All50 selected linked bodies checked:44exact/five existing near/one cold functional
base variant.33 required source omissions;13 native runtime controls including
missing read-open, old shared counter, empty split/narrow and broken uppercase.
Previous944 helper-variant and128wide-character/six reserve/16fill checks remain.
328 current input/object/linked hashes audited with zero drift. Gate registered;
no further canonical edits beyond typedDeleteData and separate-counter fix, no full
gate_all/Lua/game/install/save writes. Current receipt:
rebuild/build/native-file-read-paths/00/98/0098e000/20261019T014314942366Z/receipt.json.

Read-only revisit of deferred CloseAllStreams ODR issue: preserved first parent's
vector overflow is216B vs canonical188B with differing code/relocs, so the identical-
provider selection used above cannot resolve that deferral. Original call98E851
still targets464ACE. No retry, promotion or canonical change; preserve deferral.


Native live FinishAndGetFileHandle composition, 2026-10-19 014859 UTC:
Reused exact25B canonical0098EC70 in the actual read/write file ownership gate.
4608 original/rebuilt calls across1152 live or failed-open objects;2160 controlled
manager callbacks,1080 callback mutations to INVALID_HANDLE_VALUE. Each call
checks returned post-callback handle and complete96B guarded storage; fixture
restores the actual handle before normal ownership cleanup. Invalid handles skip
the callback. This remains a controlled manager boundary, no stream-worker claim.
Original two operands and linked selected provider verified;34 source omissions
and15 runtime controls pass, including no-cleanup and stale-return mutations.
51 linked bodies:45exact/five near/one cold functional base variant. Existing
1152 file cases,36 actual reads,6048 resizes and432 counted owners still pass;
all allocations balance. No canonical engine edit or added root credit.
Receipt: rebuild/build/native-file-read-paths/00/98/0098e000/20261019T014859560300Z/receipt.json.
Current input/object/linked hashes audited without drift; gate_all marker updated,
full gate_all not run. No Lua, game or install/save modifications.


Native PathExists ownership composition, 2026-10-19 015101 UTC:
Replaced cold PathExists fixture with canonical00999230 provider, exact111B and
all10 original relocation targets verified, selected linked body checked.48
original/rebuilt comparisons use actual Windows attributes on the owned file,
missing file, existing build directory and empty path (resolves current directory),
absolute/relative/rooted forms and sole/shared wide-string ownership. Inputs,
refcounts and all allocations balance. Always-false/always-true controls fail.
No canonical engine changes or root credit; no dot/parent normalization claim.
Existing live read/write/resize/counted/finish tests all remain green.52 linked
bodies:46exact/five near/one cold functional variant;35 required source omissions,
17 runtime controls. Input/object/linked receipt hashes rechecked without drift.
Current receipt: rebuild/build/native-file-read-paths/00/98/0098e000/20261019T015101252807Z/receipt.json.
Gate_all marker updated; full suite not run. No Lua/game/install/save writes.


Native file-attribute composition, 2026-10-19 015342 UTC:
Added complete canonical CAFile_FileQueries TU; four selected roots are exact:
modified-time137B/read-only103B/set-read-only126B/is-file123B. All original
relocation operands/import identities and selected linked bodies checked.
36 snapshots/108 calls compare actual Windows attributes/timestamps, including
18 read-only changes on owned fixture files and18 timestamp reads. Existing file,
missing file, directory and empty/current-directory inputs; shared/sole ownership,
absolute/relative/rooted forms. Missing-path read-only=true retail behavior kept.
No failed modified-time call: retail leaves output undefined on API failure.
Unrelated narrow full-path conversion remains a named cold ToWideString trap.
Four controls detect absent attribute writes, inverted readonly/directory results
and altered low timestamp bit. All previous lifecycle cases still pass; allocation
balance and receipt hashes checked.56 linked bodies (50exact/five near/one cold
functional base variant),36 required omissions,21 runtime controls. No root credit
or canonical engine changes. Fresh standalone five-root file-query parity gate also
passes exact; full gate_all not run. Current receipt:
rebuild/build/native-file-read-paths/00/98/0098e000/20261019T015342904809Z/receipt.json.
No Lua/game/install/save changes. Narrow path conversion is the next reachable
unresolved child if its existing constructors/providers meet the parity gates.


Native narrow-path ownership composition, 2026-10-19 015721 UTC:
Removed cold ToWideString trap; linked canonical45B conversion0099E480,
13B factory0099B6A0,135B private constructor0099B440,118B STL fill constructor
0099CA70 and85B narrow full-path conversion009990B0. All exact with each retail
relocation target and selected linked bytes verified.14 original/rebuilt widening
cases include empty, ASCII, high bytes and explicit embedded NUL;32 direct fill
constructor cases cover zero/odd/even counts and zero/space/high wchar values.
Four narrow full-path results match literal expected absolute/base/rooted/empty
Windows paths. Inputs, refcounts, wide/narrow/array allocations all balance.
Three controls catch empty widening, sign extension and corrupted fill character.
Two initial audit-only failures (missing instance-counter and length-error address
entries) frozen in build snapshots; fixed auditor mappings, no engine-source edits.
All existing file/resize/ownership/finish/attribute cases remain passing.61 linked
bodies:55exact/five near/one cold functional base variant.40 required omissions,
24 runtime controls. Current hashes audited zero drift; gate_all marker updated.
Receipt: rebuild/build/native-file-read-paths/00/98/0098e000/20261019T015721891926Z/receipt.json.
No new root credit, full gate_all run, Lua or game/install/save modifications.


Native request submission recovered, 2026-10-19 021446 UTC:
New original FableWin evidence: AddObject0300D360 calls AddObjectToPendingMap
0300D990, which captures device then default-constructs a key and assigns Object.
Default key0300DBA0 writes zero. Retail0098F5B0 is the implicit copy constructor,
not the pointer convenience constructor previously claimed by a header comment.
Image hash and original bytes saved under threaded/0098f0f0/20261018/debug-key-evidence.
New direct-key trial246B/81ins/85% passes288 behavior cases but fails parity.
Second helper-boundary trial244B/82ins versus240B/79ins reaches90.6832%,3.7975%
instruction delta; passes288 original/rebuilt/full24KiB independent-model cases.
This is new-evidence recovery, not a third unsupported tuning attempt.
Two scratch declaration-placement compile failures preserved; corrected duplicate
helper declaration (already existed). Stop tuning after the accepted form.

Canonical CThreadedFileManager_AddObject_0098f0f0.cpp promoted NEAR_MATCH.
Shared object header holds the evidence-based inline AddObjectToPendingMap;
its out-of-line44B/15ins body is EXACT with one independently verified relocation,
now newly accepted0098ECA0. Direct48-case original/rebuilt/model/ABI check and
no-op control pass. CFileObject constructor can inline under a TU-local macro;
ordinary declaration/standalone30B constructor remains unchanged elsewhere.
Added default key ctor, corrected pointer-constructor comment. Removed obsolete
helper definition from SetObjectPriority TU; that existing root improves from
98.31% near to100% opcode similarity at151B/59ins, still NEAR_MATCH: two stack-local displacement differences remain. The initial exact label was incorrect; corrected during actual priority integration. No layout change.

Permanent gate_native_threaded_manager_add.py:288 model comparisons,48 direct
pending comparisons,144 real Windows cases/144 CRT heap frees,288 controlled map
slot calls. Native cases use actual CriticalSection, event SetEvent/wait/reset,
GlobalAlloc zeroed20B overlap and GlobalFree via real request destruction.
UID wrap, both devices and six priorities covered; emulator also covers null
GlobalAlloc and ignored event returns. Full objects/manager/file guards checked.
Map slots stay controlled: no real queue insertion or submission-to-worker claim.
Provider omission fails; no-event/wrong-UID native controls and two no-op emulator
controls fail. First native negative log needed fflush before ExitProcess; failed
receipt retained, fixture-only correction. Current receipt:
rebuild/build/native-manager-add/00/98/0098f0f0/20261019T021446077712Z/receipt.json.
213 current input/object hashes audited without drift.

Header impact:20 sibling TUs,225 unrelated existing symbols byte/relocation-identical.
Only intended helper/priority bodies changed; unused pointer-key ctor emission
removed from priority TU. Initial audit treated removal as mutation; preserved and
corrected to distinguish no-longer-emitted unused code. Request lifetime280 cases
with three existing deletion callers and manager-wait1440 cases pass. Actual worker
owner672 native cases passes (336 reads/336 writes,672 joins/shutdowns/exit objects,
26 omissions/four controls): native-worker-owner/00/98/0098ecd0/20261019T021410988614Z.
Live file gate refreshed1152 cases/61 linked bodies/40 omissions/24 controls:
native-file-read-paths/00/98/0098e000/20261019T021303326518Z;397 hashes checked.
Worker owner208 hashes checked. No full gate_all run.

Registered new root/helper in corrections, artifact catalog and dedicated symbol
index. Exact two symbol rows added;40623 unrelated symbol rows plus header remain
unchanged. Gate_all entry added. No dashboard regeneration, Lua/game/install/save
changes or commits. RemoveObjectFromPendingMap0098E770 and CloseAllStreams0098E7D0
remain deferred; AddObject0098F0F0 deferral is now resolved. Next useful step is
actual UID/pending insertion providers in the new submission composition, keeping
manager queue mutation and worker handoff scoped until verified.


Actual submission map insertion, 2026-10-19 023243 UTC:
Eight canonical roots across five new TUs now accepted. Pending mapoperator[]
00991350:124B,98% near; UID009913D0:115B,97.9167% near after retail evidence
showed lower-bound walk is inline. Pending hinted insert00990DC0 exact610B;
UID00991030 built520B/original518B near98.8662%. Pending/unhinted00990900 exact263B
and UID00990AD0 exact180B. Node-link pending00990830 exact201B, UID00990A10 exact191B
after original bytes showed node creation must inline. First no-inline link forms
and first UID wrapper preserved. No further tuning after passing forms.

Full tree closure:12 exact bodies,three near,one reused FUNCTIONAL_VARIANT.
Predecessor generic helper is78B vs79B retail;91.8919% similarity,10.2564% instruction
delta fails near. Two isolated direct forms (80B/88%;78B/91.89% butdelta10.2564%)
remain deferred, no shared-header or canonical helper changes. Original copies
98FDD0 and98FE20 are byte-identical79B; named relocation mapping pins each call's
actual address, rather than masking an unexplained alias. No helper root credit.
Original/rebuilt 180 sequences/4500 full64KiB state checkpoints cover both key types,
ascending/descending/mixed/tied keys, duplicates, direct unique insertion and
arbitrary hints. Independent ordered keys/values, parent links, black height,
red-child rules, counts, allocations and untouched storage checked. All17 code
addresses including both predecessor aliases execute;17 code-removal controls fail.
Only malloc24 is controlled in this oracle. Explicit const input preservation added.

Permanent gate_native_threaded_submission.py passes60 native scenarios with336
actual AddObject calls, both devices, six priorities, UID wrap and sort-key/file-pos
ties. Actual map constructors and all insertion/rotation/rebalance code execute.
Real CRT nodes/requests, Win32 critical section/events, zeroed20B GlobalAlloc blocks,
and request destructor GlobalFree are exercised. Fixture manually frees trees after
validation:336 requests,672 nodes,900 headers all released. This is not production
manager initialization/removal/worker handoff. Allocation failure not newly claimed.
17 linked bodies verified including existing parent; predecessor remains qualified.
Six providers required by link. Omitting canonical rebalance links a different435B
inline COMDAT, but selected-body audit rejects it (canonical190B must be selected).
Four native controls catch wrong UID, double node count, missing UID/pending maps;
with17 emulator removals and one selection control,22 controls total.
Receipt: rebuild/build/native-threaded-submission/00/98/0098f0f0/20261019T023243558363Z/receipt.json.
Saved JSON files parse; input/object hashes verified with no drift. Registered eight
roots (five exact/three near) in corrections and dedicated index; five artifact rows;
eight symbol rows added with40625 unrelated rows unchanged. Gate_all entry added;
full gate_all not run. No Lua/game/install/save changes or commits.
Next: queue consumption still encounters deferred RemoveObjectFromPendingMap and
CloseAllStreams; investigate fresh original evidence before retrying either.


### Pending-map removal recovered from new original evidence

Debug0300DBE0 confirms default sort-key construction, assignment and named find
iterator. Retail0098E770 captures the current queue iterator address before find.
The first default-key candidate remained94B/75.36%; explicitly capturing the cursor
reference yields91B/35 instructions,97.142857% similarity,zero instruction delta.
Stopped tuning once near and behavior gates passed. Prior failed forms retained.
Canonical CThreadedFileManager_RemoveObjectFromPendingMap_0098e770.cpp registered.
No shared header changes. Scratch evidence under0098e770/20261018/debug-local-iterator.

Actual insertion/find/key erase/range erase/extrema/rebalancing execute in360
original/rebuilt/model sequences with2016 removals. Full64KiB storage guards,
cursor successor/end/other-node behavior, exact node free, tree ordering/parent/
red-black invariants and x86 ABI checked;13 removed-code controls detected.
Native fixture composes actual AddObject with actual pending removal:60 cases,
336 submissions/removals,336 request destructions,336 UID nodes manually released,
900 headers released. Real CRT pending-node frees and Windows synchronization/
overlap allocation; all other queues, UID map, requests and files checked unchanged.
27 linked bodies verified,including reused functional-only predecessor; no new
helper credit. Six provider omissions fail linking; one redundant iterator provider
omission selects identical bodies and still passes native execution. Three native
controls detect no removal,stale cursor and wrong cursor branch (16 total controls).
Initial permanent gate wrongly required redundant source omission to fail; corrected
classification and retained failed receipt directory. Production startup,worker
handoff and UID retirement are not claimed by this fixture.
Receipt: rebuild/build/native-threaded-submission-removal/00/98/0098e770/20261019T024751295996Z/receipt.json.
Input/object hashes checked with zero drift; one index row added and40633 unrelated rows unchanged. Gate_all registered, full gate_all not run. No Lua/game/install/save changes or commits.


### Actual request retirement composed

NativeThreadedRetirement_test.cpp and gate_native_threaded_retirement.py extend
the accepted actual-submission/removal graph with existing exact RemoveObject156B,
UID find71B and UID iterator erase50B. Fresh compilation and selected linked code
checked for30 bodies; no new root credit.60 native scenarios/336 requests include
168 pending cancellations through actual pending removal and168 finished retirements
after explicit pending removal.672 protected-state refusals (pending_locked and
reading/read_completed),336 repeated missing-UID calls, guarded manager/request
bytes, all queue/UID tree invariants and untouched surviving requests/files checked.
Requests and UID/pending nodes retire through production routines; fixture manually
frees900 empty headers only. Real Windows synchronization and overlap allocations.
360/2016 original/rebuilt pending-removal model remains attached, not a new full
RemoveObject emulator claim.30 selected bodies,9 required source omissions,one
redundant iterator omission,13 emulator controls and4 native defects caught.
Controls omit destruction/UID erasure or change protected/missing return behavior.
First gate used a mistaken53B UID-erase extent; corrected to independently accepted
50B. First missing-UID-erasure control reached repeated delete and timed out;
fixture now checks UID count before retrying removal and fails cleanly. Failed
build directories retained. Existing predecessor remains functional-only.
Production manager startup/worker handoff are not claimed by this fixture.
Receipt: rebuild/build/native-threaded-retirement/00/98/0098eb20/20261019T025156313521Z/receipt.json.
Input/object hash audit passed; gate_all registered, full gate_all not run. No index/count changes. Next replace controlled worker pending removal and hand-built submission with actual providers. No Lua/game/install/save changes or commits.


### Actual submission-to-worker ownership composed

NativeThreadedWorkerSubmission_test.cpp removes the old native worker fixture's
controlled pending-removal function and manually built request/overlap/UID/pending
nodes. Actual AddObject creates everything under the actual manager critical section.
Actual constructor starts CRT worker; actual ProcessObjects removes pending nodes,
issues read/write APCs, completes requests, and actual Uninitialise/ThreadMain/
deleting destructor retire objects,UID nodes and15 headers. Fixture observes the
new process-owned thread through Toolhelp snapshots and an owned OpenThread handle,
verifies priority1 and joins after shutdown.672 real I/O cases (336 reads/336 writes),
336 callbacks and672 request exits pass. Entire synthetic output checked; no game
launch/assets/install/save changes. Installer existing-singleton fixture still has
pre-signaled pause acknowledgment; cold allocation paths trap. File wrapper global
manager is still PAGE_NOACCESS. No new root count and predecessor remains functional.

Initial combined link exposed duplicate pending _M_erase and standalone request
constructor definitions. Removing the redundant standalone constructor provider is
sufficient because actual AddObject supplies its visible constructor. A scratch
forward-declared pending eraser changed destructor214B to166B/84.29% and was rejected.
Canonical destructor is untouched. Instead, the existing key-erase specialization
is now inline COMDAT with __declspec(noinline), preserving retail out-of-line calls
while allowing the identical destructor-emitted body to share a definition.
All34 key-erase symbols and25 destructor symbols retain bytes/named relocations;
node eraser remains exact51B. Symbol index changes only its strong->comdat strength,
with40633 other rows unchanged. Existing manager-remove exact gate refreshed, as
were actual pending-removal360/2016+336native and retirement336/672refusal gates.

Permanent worker gate verifies48 selected bodies,34 required source omissions,
one canonical-rebalance selection control and six runtime controls: no worker entry,
no installer Pause/Unpause,false ObjectExists,no pending removal,wrong submission UID.
The attached original/rebuilt pending model contributes360 sequences/2016 removals
and13 removed-code controls. Fresh AddObject original call targets also checked.
Initial linked-body audit masked actual timer relocations but not original addends;
corrected symmetric masking after independent retail relocation validation.
Rejected/failed build directories preserved. All final input/object hashes intact.
Receipt: rebuild/build/native-worker-submission/00/98/0098f0f0/20261019T030037657118Z/receipt.json.
Gate_all registered; full gate_all not run. Next actual SetObjectPriority queue moves in worker composition. No Lua or commits.


### Bedtime checkpoint: actual queue-priority transitions

User asked to stop for bedtime and update continuation docs. No further new work
after the running callback-priority validation completes.

Permanent gate_native_threaded_worker_priority.py passes672 actual worker requests
with672 moves from a different priority queue:336 pending and336 pending_locked
states during serialized setup.2016 same-priority/missing-UID/finished-state no-op
calls checked.50 linked bodies,36 required omissions,one rebalance selection control,
seven runtime controls; all reads/writes/callbacks/shutdowns and owned output pass.
SetObjectPriority retains NEAR_MATCH151B/59 instructions,100% opcode similarity,
zero instruction delta; it is NOT byte-exact. Two stack-local displacement bytes
differ at offsets116/128 (original18/1c hex, rebuilt1c/20 hex). All named call and
IAT targets equal original. The earlier exact151B claim in corrections and docs
was wrong and has been explicitly corrected. No canonical priority code changed.

A callback-priority scratch fixture then exposed a test race: checking manager
RecursionCount==0 during a callback can observe a concurrent main-thread query
holding the same lock. The relevant invariant is that the callback thread does
not own the lock. New submission/priority/callback fixtures now check OwningThread
against GetCurrentThreadId. Both permanent submission and priority gates were
rerun and pass. Existing older worker-owner fixture was not changed.

Scratch callback-priority scenario passes672 cases:168 read-begin callbacks invoke
actual SetObjectPriority while genuinely pending_locked with manager lock released,
move the request to another queue, refuse once, then accept on retry. The actual
ProcessObjects failure branch restores pending/signals/sleeps and the next selection
finds the moved request. Remaining504 requests exercise ordinary paths. The first
scratch failed only the racy RecursionCount assertion; corrected ownership check
passes. Permanent gate_native_threaded_worker_callback_priority.py is the final
validation started before the user's stop; its final status is recorded below.
submission receipt: rebuild/build/native-worker-submission/00/98/0098f0f0/20261019T030548066986Z/receipt.json (245 input/object hashes intact).
priority receipt: rebuild/build/native-worker-priority/00/98/0098eff0/20261019T030518330531Z/receipt.json (249 input/object hashes intact).
Priority gate registered in gate_all; full gate_all not run. No Lua,game launch,install/save changes,commits or pushes.

Final running validation completed PASS before stopping: callback-priority672 cases,
168 actual callback queue moves/refusals/retries,50 linked bodies,36 required
omissions,one selection control,nine runtime controls (including ignoring locked
priority changes and ignoring read-begin refusal). Registered in gate_all.
Receipt: rebuild/build/native-worker-callback-priority/00/98/0098ecd0/20261019T030642460547Z/receipt.json; 249 input/object hashes checked with zero drift.
Next session should compose QueueWhenFinished=false with actual OnFinishObject
UID erase/immediate destruction, using callback-time live overlap observations and
missing-UID polling without touching freed requests. Current worker fixtures retain
requests until exit. All source/fixture failures and immutable receipts retained.
User bedtime stop honored; no further recovery work started. No active task-owned
validation remains after this checkpoint.
