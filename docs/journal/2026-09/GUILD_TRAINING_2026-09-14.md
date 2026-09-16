# Guild training offline recovery, 2026-09-14

User moved active work to Hero's Guild while New Oakvale awaits live verification.
No deployment or game/profile/save writes. No canonical runtime changes.

Recovered nine quest inventories from existing lifecycle exports, then exported
277 native functions read-only from D3B390..D68F00 plus anchored shared callbacks.
The first range export had 182 bodies; missing factories and vtable functions
were defined in the read-only session and exported through two expansion passes.
28 entity bindings and all seven lifecycle slots are accounted for. Native thread
stores resolve 16 registrations, including five nested workers. The generic lift
misses ScorpionHome in GuildTrainingWoodsMelee; inventory retains it explicitly.

Added reproducible export, ownership inventory, and diagnostic baseline tools.
Baseline: 4/28 entity syntax passes, 4,128 diagnostics. No behavior claim for these
drafts. First reviewed Lua slice is RaceMarker; complete native Main vs Lua traces
match 128 cases. Four tests pass, including evidence-change rejection and entity
vtable corruption rejection. Lua uses semantic names and explicit termination
checks rather than the baseline's alive=true placeholder.

See [Guild recovery](../../scripts/GUILD_TRAINING_RECOVERY.md) for resume commands,
limits and next steps. Runtime integration and live verification remain open.

Added a second reviewed entity, SpeedFriend. Its Lua keeps the native initial frame,
termination gates, priority-4 AcquireControl retry, SetIsPushableByHero(false), and
final frame loop. Focused SpeedFriend syntax/control tests pass.

Also reviewed the small KillBird entity: its Init hostility binding and control/yield
loop are readable, with syntax and Init boundary tests passing.

Recovered PreMeleeDummy from its native state machine: mode 1 counts ordinary hits
and plays WOBBLE; mode 2 recognizes OBJECT_HERO_STICK, counts the hit and plays
GET_HIT_SPIN, while other hero hits still wobble. Focused mode/state tests pass.

Recovered MeleeThunder's native TutorialState 4→6 sequence and both control phases;
focused state/control tests pass.

WillDummy is now backed by a native state-access ledger: 90 exported functions and
289 indirect state accesses are recorded in `guild_training/state_access.json`.
This prevents assigning semantic names to the wrong parent/entity pointer layer.

Added the cross-checked `native_field_maps.json` for Skill, Will, PreMelee and Melee
quest fields. WillDummy's entity-data offsets remain deliberately unnamed pending
its data-layout evidence.

Recovered the native Skill and Will quest Init functions with semantic field names
and constants. Their full Main functions remain intentionally excluded pending the
entity/resource conversion.

Recovered Departure and WoodsDeparture setup paths, including native field constants,
activation waits, binding lists, worker creation and FinalMaze marker construction.
Worker thread behavior remains the next unresolved piece.

The generic lifter now consumes the expanded native translation unit for Woods
worker anchors. Three Woods quest packages emit four real worker bodies each,
including nested WatchForLeaving and TeleportOutHero, with no unavailable-body stubs.
Worker cleanup/state semantics remain under review.

Reviewed WoodsDeparture's WatchForTermination worker, including native success and
failure arguments, experience-spending restoration, and delayed deactivation.

Reviewed its TeleportOutHero worker too, including the native 6.0 health threshold,
exit marker, conversation line and 1000 health correction.

Reviewed WatchForLeaving's hero-liveness and success/failure flag transition.

Reviewed DoMission through the Woods load wait, objective/worker setup, mission-over
gate and native completion call.

Reviewed WoodsMelee setup and its ScorpionsAlive-to-MissionSucceeded transition,
including the ScorpionHome binding and worker registration.

Reviewed WoodsMelee DoMission, WatchForLeaving, and TeleportOutHero directly from
the native thread bodies. The mission objective, Guild Woods wait, nested worker
creation, EndMission boundary, hero liveness, 6.0 health threshold, teleport marker,
conversation text, and 1000-point health correction are represented. Its shared
WatchForTermination body remains pending because the nested worker state layout is
not yet joined to the quest parent fields.

ScorpionHome native Main anchors were recorded: HUD_BEETLE_ICON, GuildScorpions,
CREATURE_GUILD_STAG_BEETLE, ScorpionSpawn, and the three-creature completion
threshold. A side-effect-free readable Init stub documents the private field
boundary; registration remains disabled pending wrapper ownership.

RunTutorials native evidence confirmed the post-training chain: Melee -> Skill ->
Will -> Departure, with Complete Melee, Complete Skill, and Complete Will labels and
a frame yield/activity recheck between stages. TheRealGuildmaster is the NPC
boundary; live verification remains deferred.
