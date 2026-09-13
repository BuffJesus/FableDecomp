# Guardian Trophy Dealer Info — disabled review package

The six Lua files under `FSE` reconstruct `QS_GuardianTrophyDealerInfo`, its
`WaitForPieceOver` task, and `GTDI_Maze`. The root initializes and persists
`PieceOver`; the entity gives the Find the Archaeologist card and retains
retail's first meeting, repeat speech, and cancellation order.

`retail_override.lua` is disabled and `quests.lua` adds no legacy quest.
Entity ID 69 is the seed Lua registry assignment. The root uses the native
quest name, not seed quest ID 1005.

This package requires the source runtime's explicit resource, owned Thing,
and bound-alive-condition APIs. Installed v23 does not contain those additions.
This directory is a review artifact, not a drop-in update to the installed
New Oakvale profile. Any later combined profile must retain single authority
for each quest and recheck entity IDs against the then-current configuration.

Offline evidence lives in
`refs/script_recovery/guardian_trophy_dealer_info/runtime_evidence`.
Compiled checks exercise the packaged entrypoints with engine API doubles.
They do not establish in-game scheduling, streaming, macro playback,
camera/audio behavior, or save-file round trips.

Live checks still needed on a disposable profile with the newer runtime:

- First meeting via hit, special hit, and talk; normal and Escape paths.
- Card, objective, teleporter, logbook, and completion effects occur once.
- Repeat hit/talk speech and termination clean up acquired resources.
- Watcher survives frames, region changes, entity death/removal, and cancellation.
- Save/load preserves `PieceOver` and does not replay completed rewards.
