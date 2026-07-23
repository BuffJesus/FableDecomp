# Naming straggler resolver

Raw function naming is complete (`49,553 / 49,553` non-default labels), but the final autonomous
tranche contains a quality tail: 391 functions are low-confidence, address-suffixed, or both.

The unattended resolver is deliberately proposal-only. It never applies labels to Ghidra.

## Workflow

1. `tools/resolve_naming_stragglers.py --manifest-only` creates the current quality manifest from
   `ghidra_out/labels_forge_autoname_t6_all.tsv`.
2. `tools/run_naming_straggler_pipeline.ps1` waits for `lift/re-agent-queue.complete`, ensuring the
   terrain auto-RE queue owns its workstream until it is finished.
3. `ExportInstructionSignatures.java` exports address-independent instruction shapes for retail
   `Fable.exe`, symbolized `ego_r.exe`, and symbolized `FableWin.exe`.
4. The offline resolver proposes an original donor name only when the normalized signature maps to
   one donor name. A strict match from both donors is high-confidence; a strict unique match from
   one donor or an agreeing cross-donor loose match is medium-confidence.
5. Results are written under `ghidra_out/naming_stragglers/`. `proposals.tsv` is a review queue,
   not an apply file. `unresolved.tsv` retains ambiguous functions for vtable/call-graph review.

## Current manifest

- 391 total quality-tail functions
- 214 initializer/destructor artifacts
- 105 allocator wrappers
- 25 semantic-logic functions
- 18 container/template helpers
- 16 field accessors
- 12 thunks/stubs/wrappers
- 1 probable invalid function boundary

## Controls and status

- Scheduled task: `FableTLC Naming Straggler Resolver`
- PID: `ghidra_out/naming_stragglers/pipeline.pid`
- Log: `ghidra_out/naming_stragglers/pipeline.log`
- Graceful stop: create `ghidra_out/naming_stragglers/pipeline.stop`
- Completion: `ghidra_out/naming_stragglers/pipeline.complete`
- Human-readable status: `ghidra_out/naming_stragglers/summary.md`

The task starts at user logon, has unlimited runtime, and restarts after failure. It shows a desktop
notification when proposals are ready. Applying any proposal remains an explicit reviewed step.
