# ForgeFSE retail-script shadow and override runtime

## Safety contract

Retail script replacement must be opt-in and identity-preserving. The runtime manifest keys entries
by the original native name because WLD, `.qst`, saves, and cross-script activation all use that name.

Modes:

- `disabled`: native allocator and behavior are untouched.
- `shadow`: native script remains authoritative; Lua executes against a tracing facade whose mutating
  calls and persistence writes are suppressed.
- `override`: ForgeFSE replaces the registered native allocator with a Lua host allocator and allows
  mutations. Tooling refuses this mode unless the package is labeled `verified-port`.

`tools/script_recovery/build_override_manifest.py` currently emits the machine-readable manifest.
All six seed packages are `reconstructed-source`, so only `disabled` or `shadow` is legal.

## Required ForgeFSE runtime work

1. Add a registration-postprocess hook that can locate a retail script record by original name and,
   only in `override`, swap its allocator while preserving name, ID, section, and save key.
2. Add a shadow `LuaQuestState`/entity facade. Read-only calls may reach the game; mutating calls must
   be recorded and return deterministic fixture-compatible results without changing game state.
3. Emit structured events containing frame, native name, Lua package, host/thread/entity identity,
   API scope/name, normalized arguments, result, and mutation disposition.
4. Never write shadow state into retail save persistence. Shadow state is ephemeral or written to a
   separate diagnostic file keyed by save/profile fingerprint.
5. Require an explicit disposable-save acknowledgement for `override` until each script's save/load,
   unload, failure, completion, and cross-activation paths are validated.

## Compatibility requirements found by the seed corpus

- Preserve and round-trip the mutable cutscene flag map passed to `RunCutsceneMacro_Func`.
- Allow Lua helper threads to observe cutscene flag changes (`UNLIMBO` is the first proven case).
- Expose entity `GetAngleXY` using the already-declared entity vtable slot 10.
- Expose retail global helper `FadeOutAndKillEntity` at `0x00CBE716` only after ABI review.
- Resolve and bind `AddLogbookStoryEntry`; donor symbols prove the function but the retail address is
  not yet established in the current binding artifacts.
- Do not implement `StartCutscene`/`RunCutscene`/`EndCutscene` merely from reconstructed names. Recover
  their required session semantics from native users such as `Q_HerosOldHouse` first.

## Trace comparison

Shadow traces are compared at semantic-operation level, not raw call count. Permitted normalization
includes destructor noise, reference-count operations, string temporaries, and compiler-generated
yield checks. State changes, objective transitions, entity bindings, cutscene maps/flags, thread
creation, markers, rewards, and activation/completion calls must remain ordered and equivalent.
