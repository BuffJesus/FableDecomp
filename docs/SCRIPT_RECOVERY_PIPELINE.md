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

The retail `CGameScriptInterface` vtable base at `0x01260F0C` is now queried directly by
`DumpVtableSlots.java`. `VerifyScriptInterfaceField.java` independently proves for all 161 allocators
that the fastcall `EDX` interface argument reaches script-object offset `+0x40`, either directly or
through a constructor stack argument. The readiness analyzer applies a method name only when both
that per-script evidence and lifecycle dataflow prove the dispatch base, or when the global
`DAT_0143e8f8` singleton is explicit. This conservative pass maps 1,708 calls across 93 scripts to 191
retail interface methods. Of those calls, 1,521 target methods already present in the ForgeFSE API
manifest. The runtime-aware pass separately inspects the bindings actually registered in
`LuaManager.cpp`: 1,687 of the 1,708 calls are directly callable or host-managed. It maps retail
`PostAddScriptedEntities` to the safer `FinalizeEntityBindings` wrapper and records
`StartScriptingEntity` as host-managed rather than exposing the scheduler primitive to Lua. The only
remaining runtime methods are the three engine-message pollers `MsgOnLevelLoaded`,
`MsgOnRegionLoaded`, and `MsgOnRegionUnloaded`; their engine-owned output containers still require
ABI and ownership proof. Calls through unrelated entity/resource vtables retain their raw expressions
and are not mislabeled merely because an offset matches.

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
