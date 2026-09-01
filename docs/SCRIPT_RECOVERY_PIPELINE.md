# Fable game-script recovery pipeline

## Scope

This pipeline targets all gameplay script forms rather than treating every script as a quest:

- native quest and repeatable-quest classes (`Q_*`, `QS_*`, `QR_*`);
- native village/activity controllers (`V_*`);
- global, personal, gameflow, and master scripts;
- native scripted-entity behavior owned by those scripts;
- spawned helper functions and region-scoped threads;
- textual cutscene/region macros, which are parsed rather than decompiled.

The first end-to-end vertical slice is ForgeFSE Lua quest/entity packages because ForgeFSE already
implements the engine's native quest and entity host contracts.

## Evidence levels

Generated artifacts must retain one of these labels:

1. `registry-fact`: identity, allocator, section, or ID recovered from the retail registration table.
   `registry-operand` is the stronger allocator subtype: an exact script-name xref was followed to
   the executable allocator operand stored in `CScriptManager_RegisterAllScripts`.
2. `native-decompile`: operation or control flow recovered from a specified executable address.
3. `runtime-observed`: behavior captured from an instrumented retail execution.
4. `reconstructed-source`: third-party or hand-produced Lua that has not been proven equivalent.
5. `verified-port`: Lua whose static structure and relevant runtime trace agree with retail evidence.

The six seed archives are `reconstructed-source`. They are hypotheses and test inputs, not original
Lionhead source.

## Current importer

`tools/script_recovery/recover_scripts.py` safely reads package ZIPs, preserves every source byte,
hashes the archive and members, merges package registrations, emits generic script IR, and validates
API names and selected lifecycle hazards against `refs/fse_api_manifest.json`.

Example:

```powershell
python tools/script_recovery/recover_scripts.py `
  D:\Downloads\MeetSister.zip `
  D:\Downloads\MazeResearch.zip `
  --output refs/script_recovery/seed_corpus
```

Important outputs:

- `corpus_index.json`: package provenance, registry records, file hashes, and IR paths;
- `combined_registry.lua`: deterministic merged ForgeFSE registry without repeated `FSE_Master`;
- `ir/*.json`: source-derived generic quest/entity/village/global IR;
- `validation.json`: machine-readable errors and warnings;
- `sources/`: byte-preserved archive members under per-package directories.

`tools/script_recovery/build_native_catalog.py` converts the retail registration TSV into a typed
catalog and correlates seed packages with retail identities. This matters because reconstructed
package names are not consistently the engine's join keys: `MazeResearch` maps to
`V_MazeResearch`, while `HerosOldHouse` maps to `Q_HerosOldHouse`.

`ResolveScriptAllocatorsFromRegistry.java` avoids trusting Function ID labels on registration
operands. It correlates exact catalog names with the pending `AddScript` record and recovered 160
instruction-level allocator operands; the one script without an exact defined-string match
(`Expression_Follow`) already has a direct registry address. Together these provide allocator
addresses for all 161 catalog entries. All 161 now also have exported five-slot lifecycle clusters
and native operation IR. Only the six anchor-checked seeds are identity-anchored; the other 155
remain address-resolved evidence awaiting runtime trace comparison.

`tools/script_recovery/analyze_compatibility.py` turns the IR call inventory and validation findings
into a ranked ForgeFSE capability backlog. It also emits deterministic lexical trace templates. The
templates intentionally leave arguments, branches, and results unresolved until decompile or runtime
evidence supplies them.

`tools/script_recovery/run_lua_trace.py` complements those templates by executing the actual recovered
Lua inside an isolated mock host. JSON fixtures control API return sequences, opaque quest/entity
handles preserve actor identity, and a VM instruction budget stops accidental infinite loops. Its
normalized event JSON records concrete arguments and branch-driven call order without mutating the
game. `compare_event_traces.py` provides exact regression comparison and ordered-subsequence comparison
for native shadow logs containing extra telemetry. MeetSister fixtures and checked-in traces exercise
both a quest main and an entity wait/yield/cutscene path.

The executable harness requires Python `lupa` (Lua embedded in Python); it does not load ForgeFSE or
retail DLLs. Checked-in traces include the source SHA-256 so stale fixtures can be detected reliably.

`tools/script_recovery/build_conversion_queue.py` produces a deterministic native conversion queue.
Seed-correlated scripts are prioritized, but remain labeled `registry-fact`; possessing reconstructed
Lua never skips the typed-decompile, API, state, persistence, static-validation, or trace-review gates.

`tools/script_recovery/audit_forgefse_runtime.py` compares corpus call sites with the names actually
registered in ForgeFSE's `LuaManager.cpp`. This is intentionally distinct from the complete native
interface manifest: a native method can be known yet still unavailable to Lua in the current build.

## Native conversion design

For each row in `ghidra_out/quest_registry_table.tsv`:

1. classify the registered script without assuming it is a quest;
2. resolve allocator, object vtable, lifecycle methods, spawned helpers, and entity bindings;
3. decompile the complete script-local call graph;
4. replace known `CGameScriptInterface` calls with manifest-backed operations;
5. recover state fields, persistence order, strings, constants, and control-flow edges;
6. emit address-cited native IR with confidence per operation;
7. generate Lua only for constructs supported by the selected ForgeFSE host;
8. route unresolved wrappers into the existing ForgeFSE binding RE queue;
9. compare a deterministic Lua event trace with native runtime observations;
10. promote to `verified-port` only after explicit review.

`tools/ghidra_scripts/ExportNativeScriptCluster.java` automates the first object-boundary step. Given
a registry allocator, it follows the allocator's constructor call, detects the constructor-assigned
five-slot lifecycle vtable, and exports address-cited decompilation for destructor, `RegisterMain`,
`Main`, `Init`, and `OnPersist`. Imported symbol names are recorded only as `currentName`; lifecycle
roles come from vtable position so a false donor name cannot silently redefine the contract.

Pass a script-specific string as the optional fourth argument whenever one is known. The exporter
rejects candidates whose recovered cluster lacks that anchor. This guard caught an initially
plausible `QS_MeetSister` address whose body was actually `QS_GuardianTrophyDealerInfo`; the corrected
cluster is retained under the trophy-dealer identity. A string xref then resolved the actual
MeetSister allocator at `0x00E29990`, and the same guard accepted it only after its lifecycle cluster
contained `TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY`.

`DiscoverScriptAllocatorFromAnchor.java` removes the usual manual address hunt when a distinctive
retail string is available. It walks exact string xrefs to the containing `Main`, recognizes that
function in lifecycle-vtable slot 2, follows vtable assignments to the constructor, and reports its
allocator callers (or the function itself for inline construction). Candidates still pass through
the exporter's independent anchor check; discovery is not treated as verification.
The discovery walk was exercised across all six seeds and both supported construction shapes: inline allocation for
`QS_MeetSister` (`0x00E29990`) and a separate constructor/caller for `V_MazeResearch`
(`0x00EA76E0` / `0x00EA8690`). Expected results live in
`refs/script_recovery/native_discovery_fixtures.json`. When an anchor xref initially sits in
unanalyzed code, discovery conservatively bootstraps `Main` from lifecycle slot 2 and accepts the new
function only if its body owns that exact xref.

`tools/script_recovery/decompile_native_script.py` is the queue-facing orchestration command. It runs
headless discovery, requires exactly one candidate, invokes the anchored cluster exporter, treats
Ghidra script errors as failures even when the launcher returns zero, and verifies the resulting
allocator and evidence anchor before accepting the JSON artifact.

`extract_native_operation_ir.py` converts each accepted lifecycle cluster into ordered calls, string
references, state writes, typed persistence transfers, and indirect dispatch expressions with their
recoverable vtable offsets. The full catalog currently contains 3,870 such indirect calls across 153
scripts and 263 distinct offsets; these must be mapped before native IR can safely generate executable
Lua. `analyze_native_conversion_readiness.py` makes that gate machine-readable rather than emitting
speculative ports. `compare_seed_native_ir.py` correlates those
facts with reconstructed bindings, literals, and persistence keys across all six packages; results and
known parent-vs-entity scope limits are documented in `docs/SEED_NATIVE_OPERATION_COMPARISON.md`.

`ExportScriptLifecycleDirectCalls.java` also walks the actual instructions in all five lifecycle
functions and records every direct call's site and target address. The registry allocator evidence
drives 160 scripts; the separately evidenced direct registry allocator
`Expression_Follow 00eea3e0` supplies the final catalog entry. Joining on allocator address (rather
than provisional script or Function ID names) enriches all 161 operation-IR artifacts with 15,300
call sites targeting 233 exact retail addresses. `nativeDirectCallTargetBacklog` groups those targets
by address while retaining every current Ghidra name, consumer, lifecycle role, and script kind.
Names remain hints—the target address is the stable identity when imported donor labels disagree.
The extractor conservatively correlates matching decompiler call expressions back to these
instruction records, including MSVCR qualification and Microsoft-decorated member spellings. This
currently gives 1,437 of the 1,603 semantic helper call sites an exact target address and covers 117
of the 158 distinct helper labels. Unmatched calls remain explicit instead of being paired by mere
position; the complete instruction-level target list is still retained alongside them.

`build_native_helper_target_queue.py` collapses those correlated calls into a deterministic TSV
queue keyed by target address. `ExportNativeHelperTargets.java` batch-decompiles the resulting 128
targets and captures each helper's own exact direct-call graph. The current pass completes 127 helper
bodies, including 19 leaf helpers and 108 helpers with dependencies, and records 4,967 outbound calls
to 206 targets. `RunCutsceneMacro_Func` at `0x00CBFB7D` is the sole explicit failure: its unusually
large Ghidra function body exceeds the 90-second decompiler limit. Its instruction-level call graph
is retained, but it remains a repair item rather than being represented as converted behavior.
`analyze_native_helper_decompiles.py` reduces successful bodies into calls, literals, state writes,
indirect dispatch, control-flow counts, and body hashes in `native_helper_operation_ir.json`.

Semantic lifting is pattern-gated rather than name-gated. The helper analyzer currently recognizes
three complete leaf patterns: two native-field initializers and the constant-return switch
`CQ_ArenaScript::GetFanfareMusic` at `0x00F14270`. Exact caller statements prove that the initializers
at `0x00E1AD30` and `0x00E9FEC0` receive their respective parent `this` pointers from
`Q_WizardBattle.Main` and `V_KnotholeGladeGates.Main`; this corrects unrelated imported donor labels.
Because field names remain unresolved, their standalone Lua takes width-aware `write_u8` and
`write_u32` callbacks and preserves offsets and values without inventing state names.
`generate_native_helper_lua.py` emits all three helpers, while its manifest keeps
`deploymentEligible` false because none of the parent scripts is complete.
`validate_native_helper_lua.py` executes all nine fanfare cases, three default probes, and all 25
initializer writes through Lupa's embedded Lua runtime; all 37 checks currently pass.

The same typed-reader boundary now lifts four additional complete leaf behaviors without embedding
host pointers in Lua: `C3DMeshInfo::HasPhysicsMesh` reads an unnamed 32-bit field,
`CWorld::DrawGetEnvironment` reads an unnamed pointer field, `GFGetPreMainMemoryUsed` reads the exact
retail global at `0x0143E920`, and `CEngineInternalPrimitiveBase::AddChildPrimitive` returns its
constant retail result. Three compact `V_ArcheryCompetition` helpers are also lifted as ordered
interface-call sequences: removing its three quest-info elements, updating the predicate-failure
counter, and updating both the quest counter and online archery score. Their exact `Main` call sites,
parent fields, parameters, constants, and all seven interface calls are preserved. Together, ten
generated helpers pass 50 native-derived checks. The shared `CTCVillage::OnInitialActivate` helper,
called by 121 scripts (including all six reconstructed parents), is now lifted too: it reads the
exact `+0x25` activation byte, conditionally calls exact target `0x00CBDF70` with the `+0x24` byte,
then clears `+0x25`. Both branch outcomes are executed offline, bringing the total to eleven helpers
and 52 checks. These are still standalone conversion units, not evidence that their parent retail
scripts are deployable.

Helper bodies use the same provenance rule as script lifecycles when resolving indirect engine
dispatch: the base must be the explicit game-script-interface singleton or a proven script `+0x40`
interface field, and the offset must exist in the dumped retail vtable. Helper analysis exposed 29
previously unrequested offsets; extending the authoritative vtable dump from 191 to 219 executable
slots resolves all 273 proven helper-interface calls to 62 exact methods. Calls through entity,
resource, timer, or other unrelated vtables remain raw even when their numeric offsets happen to
collide. The lifecycle totals remain 1,708 calls and 191 used methods because the new offsets occur
inside helper bodies, not the five top-level lifecycle functions.

Comparing those 273 helper calls with the bindings actually registered by ForgeFSE originally found
three runtime method gaps. Forge already resolved all three retail pointers. The isolated Forge
branch now exposes wrappers for `OverrideAutomaticHouseLocking`, `UpdateOnlineScore_Archery`, and
`AddLogBookEntry`, making all 273 helper calls directly callable or host-managed. The log-book wrapper
converts Lua UTF-8 into a plain UTF-16 buffer, then constructs and destroys each opaque four-byte
`CWideString` through the retail executable's exact relocation-matched constructor (`0x0099B6B0`)
and destructor (`0x0099B510`). No modern STL object crosses the VC7 ABI boundary.
The same adapter now backs `DisplayGameInfoText` and `SetReadableObjectText`; their earlier
hand-built modern-STL surrogate and intentional allocation leaks have been removed. Forge's shadow
safety gate rejects reintroduction of that surrogate or bypassing the retail-owned string path.

The reproducible headless export accepts the correlated TSV followed by explicit name/address pairs:

```text
-postScript ExportScriptLifecycleDirectCalls.java registry_allocator_evidence.tsv \
  script_lifecycle_direct_calls.json Expression_Follow 00eea3e0
```

Pass the resulting document to each IR extraction with
`--direct-calls script_lifecycle_direct_calls.json`; lifecycle addresses are checked before evidence
is accepted, so stale or incorrectly joined exports fail regeneration.

Direct-call extraction starts at the decompiled function body's opening brace. This prevents the
function declaration itself from being misclassified as a self-call, and Ghidra p-code operators such
as `SUB41` and `CONCAT44` are retained as expressions rather than invented native dependencies. After
regenerating all 161 artifacts, the readiness gate has zero opaque callees: all 161 scripts are now in
the explicit `map-native-helpers` stage, including every non-quest class. Helper mapping and
control-flow reconstruction remain required; zero opaque callees does not by itself make a Lua port.
The readiness artifact therefore includes a ranked helper backlog with exact call counts, consumer
scripts, and script kinds. Entity registrations are modeled separately as structural IR: all 615
`AddEntityScriptBinding` records across 119 parent scripts recover the entity name, allocation size,
and allocator expression. Of those, 611 expose the inlined 0x1c-byte layout and four use a typed
retail template constructor. With registration removed from the semantic backlog, 158 distinct helper
names remain across 1,603 call sites, allowing shared engine behavior to be handled before one-off
quest helpers.

Those helpers are not treated as one undifferentiated semantic backlog. Role-aware classification
currently separates 164 destructor-only cleanup calls, 761 constructor/destructor lifetime calls,
207 CRT/container-support calls, and two script-runtime calls. The remaining 469 calls across 85
engine-or-script helper names are the higher-value semantic investigation set. Every classified row
retains its original name, lifecycle roles, counts, kinds, and consumers; classification never erases
native evidence, and imported donor names remain provisional.

The retail `CGameScriptInterface` vtable base at `0x01260F0C` is now queried directly by
`DumpVtableSlots.java`. `VerifyScriptInterfaceField.java` independently proves for all 161 allocators
that the fastcall `EDX` interface argument reaches script-object offset `+0x40`, either directly or
through a constructor stack argument. The readiness analyzer applies a method name only when both
that per-script evidence and lifecycle dataflow prove the dispatch base, or when the global
`DAT_0143e8f8` singleton is explicit. This conservative pass maps 1,708 calls across 93 scripts to 191
retail interface methods. Of those calls, 1,521 target methods already present in the ForgeFSE API
manifest. The runtime-aware pass separately inspects the bindings actually registered in
`LuaManager.cpp`: 1,707 of the 1,708 calls are directly callable or host-managed. It maps retail
`PostAddScriptedEntities` to the safer `FinalizeEntityBindings` wrapper and records
`StartScriptingEntity` as host-managed rather than exposing the scheduler primitive to Lua. The only
region-message pollers now copy their returned `CCharString` into Lua-owned memory and destroy the
native result. The sole remaining method, `MsgOnLevelLoaded`, writes a retail
`std::list<CCharString>`; passing a modern MSVC `std::list` into the VC7-era executable would cross an
incompatible STL ABI. The readiness artifact records that blocker explicitly rather than advertising
an unsafe binding. Calls through unrelated entity/resource vtables retain their raw expressions and
are not mislabeled merely because an offset matches.

Non-native macro scripts use a parallel parser and converge on the same operation/reference IR.

## Safe retail integration

Do not replace retail registrations merely because generated Lua loads. The intended progression is:

1. offline mock trace;
2. live shadow mode with mutating calls suppressed;
3. trace comparison against the native script;
4. opt-in allocator override on a disposable save;
5. save/load, region-unload, failure-path, and completion-path validation.

Retail names must be preserved during a true override because `.qst`, WLD, saves, and other scripts
use those names as join keys. New-name copies are useful tests but cannot demonstrate full replacement
compatibility.

`tools/script_recovery/build_override_manifest.py` enforces the evidence gate mechanically:
`reconstructed-source` packages can be emitted in `shadow` mode, while `override` requires the exact
`verified-port` label. The ForgeFSE runtime contract is specified in
`docs/FORGEFSE_RETAIL_SCRIPT_RUNTIME.md`.

## Foundation verification

Run `python tools/script_recovery/verify_foundation.py --output
refs/script_recovery/foundation_audit.json` for the requirement-level audit. It fails unless all six
archives retain provenance hashes, all 16 executable scripts have IR, the merged registry exists,
static API failures reconcile with the live ForgeFSE binding audit, all 161 retail registrations are
queued (including non-quest classes), all reconstructed packages remain mutation-free shadow entries,
executable traces are source-hashed, and at least three native clusters carry independent anchors.
