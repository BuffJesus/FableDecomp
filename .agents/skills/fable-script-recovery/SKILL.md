---
name: fable-script-recovery
description: Resume FableDecomp Lua recovery, trace registered scripts to native evidence, and recover generic lowering or sidecar ABI behavior. Use for existing script recovery work, not native engine reconstruction or unsolicited new conversions.
---

# Fable script recovery

## Preflight and scope

1. Resolve the active checkout with `git rev-parse --show-toplevel`.
   Inspect `git branch --show-current`, `git worktree list`, and a scoped dirty-state summary.
   Do not assume the main checkout owns the current lane.
2. Read applicable AGENTS.md, then [handoff](../../../docs/HANDOFF.md),
   the Lua portion of [roadmap](../../../docs/ROADMAP.md), and the linked current journal.
   Prefer the latest explicit user steering when older task lists conflict.
3. Establish ownership of dirty converter/evidence files before continuing them.
   Preserve other lanes' edits and scratch; never reset or regenerate another worktree.
4. Check existing project/user skills before adding procedures; coordinate shared paths.
   If the handoff gives no authorized task, report the ambiguity after setup.

## Evidence workflow

1. Select the existing registered unit in `tools/script_recovery/script_units.py`.
   Inspect only its registry entry, unit JSON, reached native functions and relevant tests.
   Setup alone does not authorize registering or converting new scripts.
2. Use [pipeline evidence levels](../../../docs/scripts/SCRIPT_RECOVERY_PIPELINE.md).
   Retail addresses/bytes establish operations; PDB layouts establish candidate field meaning;
   `refs/fse_api_manifest.json` and binding bodies establish the callable interface.
   [Aeon ports](../../../docs/scripts/AEON_LUA_PORTS.md) are reconstructed-source comparisons,
   not original source or sufficient proof of retail equivalence.
3. Classify each field before lowering: persisted state, constructor default, scalar,
   thing/list, resource, flag map, string map, or immutable definition snapshot.
   Confirm retail/PDB offset differences and quest-versus-entity ownership.
4. Recover operands from the call-site instructions when decompiler output loses them.
   Check receiver, stack purge, hidden return slot, by-value copies and boolean representation.
   Do not infer an argument from a plausible name or an adjacent unrelated call.
5. Fix generic evidence/lowering/generation rather than generated Lua.
   Keep unresolved operands and diagnostics explicit; never invent a passing behavior.
   Preserve native addresses and the draft as an auditable trail.
6. For helper naming, cross-check registered callbacks, unit helper metadata, native identity,
   and callers. A parseable function with the wrong exported name is still a broken port.

## Sidecar ABI when needed

- Read [upstream requirements](../../../docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md)
  and the relevant entries in [gotchas](../../../docs/pipeline/GOTCHAS.md).
- Inspect the stock/core FSE binding and native signature before extending the sidecar.
  Prefer generic bindings; avoid quest-specific glue and accidental forks.
- Track resource ownership across quest/entity VMs, yields, region unload and host destruction.
  A copied CScriptThing, pointer, temporary string and retained resource are not interchangeable.
- Build in an owned scratch sidecar repository. Export patches from its actual committed diff;
  do not hand-edit a patch independently of the source used to build the DLL.
- Record candidate build, packaged DLL, installed DLL and in-game proof separately.
  A successful build does not establish ABI correctness or runtime behavior.

## Validation and completion

Use [fable-readability-pass](../fable-readability-pass/SKILL.md) for generator changes.
Run the smallest test that can falsify the recovered behavior, including adverse inputs.
Read smoke JSON problems even when the command exits successfully.
Syntax and mock traces do not prove retail equivalence; state what was actually exercised.
Follow the current authorization for game launches; shared-install ownership matters.
Put changing status, exact commands/results and the next task in the lane journal/handoff,
not here. Stage only reviewed owned paths when committing.
Stop when the scoped acceptance bar is met, or record a concrete missing evidence/runtime gate.

## Project traps and efficient inspection

Search filenames and symbols before reading long files; count/filter large inventories.
Batch independent reads, preserve reports, and reuse unchanged findings.
Scripts must derive the active checkout from their location or an explicit root argument.
Inspect scratch scripts for hard-coded roots before reuse; never link over another lane's work/.
Preserve existing CRLF when patching documents and sidecar diffs.
Use PowerShell literal paths and proper quoting; avoid Git Bash conversion of compiler `/` flags.
Never create a reserved Windows device filename such as CON.
Do not remove index.lock until its owning Git process is known to have stopped.
After two materially identical failures, change strategy and record the unresolved gap.
