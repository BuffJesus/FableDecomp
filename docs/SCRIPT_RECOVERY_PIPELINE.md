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

`tools/script_recovery/analyze_compatibility.py` turns the IR call inventory and validation findings
into a ranked ForgeFSE capability backlog. It also emits deterministic lexical trace templates. The
templates intentionally leave arguments, branches, and results unresolved until decompile or runtime
evidence supplies them.

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
cluster is retained under the trophy-dealer identity, while MeetSister remains unresolved instead of
receiving false evidence.

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
