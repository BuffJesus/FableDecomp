# Hero's Guild training recovery

Active offline work, requested 2026-09-14 while New Oakvale awaits live verification.
Scope is the nine `Q_GuildTraining*` quests, including departure and Woods training.
The separate `V_GuildMaster` donor port is not a replacement for these native quests.

## Native inventory

`refs/script_recovery/guild_training/inventory.json` anchors 28 entity bindings to
retail name pushes, allocator stores, registration calls, and seven-slot entity
vtables. All 196 slot references have exported bodies (shared bodies count once in
the translation unit). The 16 thread registrations include five nested workers.
The read-only Ghidra export contains 277 function bodies; this is evidence coverage,
not reconstructed C++ or verified Lua behavior.

The generic lifecycle cluster misattributes the main Guild constructor to shared
code. Actual vtable writes identify `0x00D3B390` and the allocator at `0x00D50600`.
Propagated class/function names are not used as ownership proof.

Reproduce the native export and ownership checks:

```powershell
python -m tools.script_recovery.export_guild_training
python -m tools.script_recovery.guild_training_inventory
python -m unittest tools.script_recovery.test_guild_training_inventory -v
```

`guild_state_access_report.py` writes `refs/script_recovery/guild_training/state_access.json`.
It covers 90 exported lifecycle/entity functions and records 289 indirect state
accesses. This ledger is the input for naming parent fields in WillDummy,
PreMeleeDummy, and the Woods entities.

Cross-checked quest field names are recorded in
`refs/script_recovery/guild_training/native_field_maps.json`. It maps the native
0x48–0x54 tutorial fields for Skill/Will and the PreMelee/Melee mode and hit fields;
WillDummy's separate entity-data offsets remain intentionally unnamed.

Ghidra runs with `-readOnly -noanalysis`; missing function definitions are discarded
after export. Only addresses, metadata, hashes and decompiled source are saved here.
Original instruction bytes are loaded from the local executable for offline tests.

## First readable behavior

`refs/script_recovery/lifted/GuildTraining/readable/FSE/GuildTraining/Entities/RaceMarker.lua`,
`SpeedFriend.lua`, `KillBird.lua`, `HeroBed.lua`, and
`GuildTrainingPreMelee/Entities/PreMeleeDummy.lua` are now reviewed slices. RaceMarker checks the hero's distance, sets parent quest
`ReachedPlatform`, removes the minimap marker, and yields with the native
termination-query order. `quests.lua` remains empty; this is a partial recovery.

```powershell
python -m tools.script_recovery.guild_race_marker
python -m unittest tools.script_recovery.test_guild_race_marker -v
```

The original Main instructions execute under Unicorn and match Lua traces across
128 proximity/cancellation/initial-state/raw-boolean cases. The test also checks
the one-byte parent write and neighboring memory, stack balance, argument order,
and frame return independence. Engine calls are explicit test boundaries. This
does not establish Guild runtime integration, save/load, or live gameplay parity.

SpeedFriend is covered by `tools/script_recovery/test_guild_speed_friend.py`: its
control acquisition retry and termination boundaries are exercised with Lua 5.4
stubs. The temporary native mesh probe is intentionally omitted because it has no
script-visible result; the script-visible control and pushability calls remain explicit.

KillBird also has a small readable slice: Init marks the spawned bird hostile to
the hero, followed by the same native control and yield boundaries. Its syntax and
Init checks are covered by `test_guild_kill_bird.py`.

HeroBed covers the three native bed-definition scans and their persistence/usability
updates. PreMeleeDummy covers the mode-1 ordinary-hit and mode-2 stick-hit state
machine, including `DummyHits` and the two native animations. Both have focused
tests, while larger tutorial entities remain diagnostic drafts.

`GuildTrainingMelee/Entities/MeleeThunder.lua` is also reviewed. It preserves the
native `TutorialState` 4→6 gates, two priority-4 control phases, and final yield
loop; its state/control test is `test_guild_melee_thunder.py`.

The Skill and Will quest `Init` functions are now readable as well, with native
field names and constants cross-checked against the PDB layout. Their full `Main`
functions remain outside the package until entity/resource ownership is converted.

The Departure and WoodsDeparture `Main` setup paths are also readable: they retain
level/quest activation waits, binding finalization, worker-thread creation, objective
setup, and the final maze marker spawn. Their workers remain pending and registration
stays disabled.

WoodsDeparture's `WatchForTermination` worker is now reviewed too. It maps the
native success/failure flags, restores experience spending, completes or fails the
active quest with the native arguments, and deactivates the Woods quest on delay 0.

Its `TeleportOutHero` worker is reviewed as well: below-health threshold 6.0,
teleport to `GuildWoodsTeleportExitHSP`, pause, native conversation text, and the
1000-point health restore are retained with the original termination boundaries.

`WatchForLeaving` is reviewed too. It monitors hero liveness until success/failure,
then sets the native `MissionFailed` flag only when the mission did not succeed.

The `DoMission` worker is now reviewed through its completion boundary: objective
creation, Guild Woods load wait, worker creation, mission-over wait, and native
completion flags/arguments are preserved.

WoodsMelee setup is reviewed through its scorpion lifecycle: `ScorpionsAlive`,
mission flags, Guild Woods load, `ScorpionHome` binding, worker creation, and the
success transition after the scorpion phase.

Its `DoMission` worker is now reviewed through the native objective, Guild Woods
wait, nested worker creation, termination gate, and `EndMission` boundary. The
nested `WatchForLeaving` and `TeleportOutHero` workers retain their native hero
liveness, 6.0 health threshold, exit marker, conversation text, and health restore.
The shared `WatchForTermination` body remains pending until its nested worker state
layout is mapped to the WoodsMelee parent fields.

`ScorpionHome` now has a readable evidence stub for its native `Init` storage
boundary and documented `Main` anchors. Its private counter/byte fields and
creature-list/spawn wrappers are intentionally pending; the stub is not enabled
for registration.

The Guild lifter now consumes the read-only translation-unit worker anchors for the
Woods quests. Direct and nested workers (`WatchForTermination`, `DoMission`,
`WatchForLeaving`, and `TeleportOutHero`) are emitted as real lifted bodies in the
diagnostic packages rather than unavailable-body stubs. This is export coverage only;
their Lua cleanup and state behavior still require review.

## Remaining conversion work

Run `python -m tools.script_recovery.build_guild_training_baseline` to regenerate
the diagnostic draft under `work/guild_training_baseline`. It currently reports
6/28 entity files passing syntax and 4,045 entity diagnostics. Even the six syntax
passes are unreviewed; the baseline is not the readable candidate.

- Recover parent/entity fields, persistence types, constructors and timer ownership.
  PDB field names are useful; debug offsets require retail confirmation.
- Fix the generic lift's missing `ScorpionHome` binding in `GuildTrainingWoodsMelee`.
  The native inventory already includes its factory and all lifecycle slots.
- Convert the 16 anchored thread bodies and their helpers, retaining cancellation
  and cleanup behavior; build an explicit completeness ledger.

The native `RunTutorials` controller confirms the chained handoff: Melee reaches
`Complete Melee` before staging Skill, Skill reaches `Complete Skill` before staging
Will, and Will reaches `Complete Will` before staging Departure. Each boundary yields
and rechecks quest activity; `TheRealGuildmaster` is the associated NPC evidence.
The sequence is recorded in `refs/script_recovery/guild_training/followup_quest_chain.json`;
live verification remains deferred.

Native Main anchors for the three training quests are now recorded as well:
Melee `0x00D55E90`, Skill `0x00D5AE70`, and Will `0x00D5E0C0`, each tied to its
teacher marker and `TutorialState` completion field. These are evidence contracts,
not enabled runtime replacements.
- Continue readable entity recovery and native comparisons, starting with the
  small `SpeedFriend` behavior before the larger apprentice tutorials.
- Audit Guild runtime bindings and ownership separately. The Oakvale-only native
  lifetime policy has not been enabled for Guild.

Installed files, profiles, saves, and canonical runtime sources are unchanged.
New Oakvale live verification remains deferred by user direction.
