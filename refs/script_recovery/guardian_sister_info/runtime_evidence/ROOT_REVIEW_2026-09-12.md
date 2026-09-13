# Guardian Sister Info — reviewed root and entity lifecycle

Selected `QS_GuardianSisterInfo` after the disabled Guardian Trophy Dealer
package. Its root and lifecycle fit the current explicit resource/state APIs.
This directory contains a non-registered reference and adapter; entity Main
is not implemented or activated.

## Executed native contract

`native-root-lifecycle-20260912.json` contains 19 executions of pinned retail
EXE bytes, all matching the reference/adapter tests:

- Eight root cases: loaded, delayed load, cancellation while waiting, and
  cancellation immediately before setting time. A noncanonical nonzero region
  result also follows the native boolean branch.
- Eight persistence cases: both initial values, writes, saved true/false reads,
  and absent `GuardianSpokeToHero` using false as the native default.
- Two predicate-failure cases, with the parent flag false and true.
- Entity Init clears +1C dword, +20 byte and +24 dword, preserving padding.

Root Init E25A00 clears quest+48. Main E25AB0..E25C56 binds `MazeAtTavern`
through allocator E25C70, enabled 1 and flags 0; performs base and game PostAdd;
sets the first objective with regions empty/BowerstoneSlums; waits for that
region; checks cancellation again; then sets time to 10.0 and returns.

Root OnPersist E25F40 transfers `GuardianSpokeToHero`, false default, at +48.
The entity vtable is 12E1E28: destructor E25D70, Main E25F70, Init E25D20,
parent getter E25D10, empty persistence CDEBC0, predicate failure E25D30,
interruption CDEBE0. Predicate failure deactivates the active quest with delay
0 only when the parent's `GuardianSpokeToHero` flag is already set.

The seed's root waits for the conversation and Maze's disappearance, then
completes/deactivates the quest. Native root Main does none of those things.
Those seed operations were not copied. The reference keeps the verified
predicate callback separate and uses explicit boolean state, cancellation
checks and the correct Lua binding file path.

Engine allocation, string storage, game calls and persistence storage are
doubles. Native branching and the termination helper execute. This is not
an engine save-file or scheduling test.

## Asset evidence and remaining entity work

`installed-assets-reviewed-20260912.json` decodes the authored macro's 101 normal
and nine skip commands; all 14 required text keys resolve. Nineteen relevant
placements are present in BowerstoneSlums_v2.tng. FinalAlbion.wld explicitly
includes that map in region BowerstoneSlums; the region name is not the TNG basename.

Two authored normal-path references, `MK_GSI_FAN1END` and `MK_GSI_FAN2END`,
are absent from the checked Bowerstone placement file. A read-only `rg` search
also found neither token in installed FinalAlbion TNGs. Do not invent replacement
markers or silently change the macro. Runtime resolution/playback of those fan
teleports remains unproven. The earlier broad asset inventory included unnamed
NULL placements; use the reviewed inventory, which excludes them.

Native Main is archived for the next recovery. Reviewed entry anchors already
show why the seed entity cannot be promoted unchanged:

- E25F9E installs bound-alive condition 12C32F0 before the initial frame.
- E25FFF acquires Maze at priority 3, with explicit retry/cancellation checks.
- E26089 looks up `M_MazeExit` and E26093 queries its position.
- The call-over distance at E26151 is read from game-object+0C68, then converted
  to float; the seed's literal 30 has not been validated.
- E261AF submits `ST_HELLO` through the acquired resource helper 7E73D0 with
  eight operands, including byte 1375748. Do not substitute a cached animation
  call without verifying dispatch and those operands.
- The call-over conversation uses game slots 5B0, 5B4, and 5B8, with the bound
  Maze and Hero as participants. That motivates an actual binding test for
  borrowed participants before adapting this branch.

Seed registry assigns this package entity ID 70. That is a Lua registry fact,
not a native vtable identity, and no override configuration has been assembled.

```
python work/runtime-20260912-video-comparison/prove_guardian_sister_root.py --output work/NEW_SISTER_NATIVE.json
python -m unittest tools.script_recovery.test_guardian_sister_reference
python -m tools.script_recovery.audit_guardian_sister_assets --output work/NEW_SISTER_ASSETS.json
```
