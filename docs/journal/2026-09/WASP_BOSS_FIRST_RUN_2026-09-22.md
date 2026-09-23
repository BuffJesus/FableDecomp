# Wasp Menace: first post-guild run on our converted quest (2026-09-22)

The adult save finally exists and `Q_WaspBoss` runs from it. Two blockers were found and fixed; a third is
open. Everything below is from autopilot runs 34-37 against
`work/new-oakvale-original-fse-20260912/local-candidate-v8` then `-v9`.

## The adult save

The user played the guild training by hand and pressed Continue at the graduation card. The save is
`Saves/1234234/AutoSave` (20:23), copied to two safe places:

* `Saves/adult_graduated` -- the source profile runs stage from (`autopilot.py run ... --save adult_graduated`)
* `Saves/adult_graduated_2026-09-22_2023` -- untouched backup

It is genuinely post-graduation: LUAGameflow persisted `PostSavePosition = 150` (`EGP_WAITING_FOR_WASP_BOSS`)
and the screenshot shows the adult hero at the guild map table with the "press Tab at the Map Table" prompt.
It loaded on v8 and v9 with no persist crash, so a v6 save is compatible with those bundles.

## Blocker 1 (fixed): the quest parks on LookoutPoint, not PicnicArea

`Q_WaspBoss::Main` waits on `IsRegionLoaded("LookoutPoint")` BEFORE it binds anything -- retail's route out
of the guild is Lookout Point first, Picnic Area second. `AddQuestRegion('Q_WaspBoss', 'PicnicArea')` in
`Init` is about the quest's own region, not the arming point, and reading it as the entry was the mistake.

Run 35 teleported straight to PicnicArea (map slot 2) and the quest sat on that first wait forever: host
alive, `list` reporting `WaspBoss/WaspBoss`, zero entity binds, hero standing still. That is what the user
saw as "your automation isn't doing anything".

Map slot **1** = `LookoutPoint` (`forge world`: origin 3232,3488, 128x128), ground z 39.04 at map-local
64,64 (`forge heights LookoutPoint 64,64`) -> `GoToMapSlotRetailTransition(1, 3296.0, 3552.0, 39.0)`.

Past that wait `Main` calls `GiveHeroTutorial(TUTORIAL_CATEGORY_CAMERA)`, and the tutorial box gates script
frames, so the exec channel times out until it is dismissed -- `input: clear` -- and only then do the nine
`AddEntityBinding` calls land (`GratefulVillagerSpawn`, `WaspChaser`, `WaspChaseWoman`, `WaspAttacker`,
`WaspVictim`, `FleeingWoman`, `WaspHelper`, `QueenHornet`, `HornetDrone`).

`checklists/wasp_boss.json` now runs channel -> activate -> `wasp_lookout` -> `wasp_camera_tutorial` ->
`wasp_region` -> entities -> intro -> helper -> queen -> outcome.

## Blocker 2 (fixed, commit 6489a52): member zero-inits clobbered resource handles

See the commit and `tools/script_recovery/test_resource_member_zero_init.py` for the full story. Short
version: Ghidra spells an inlined constructor as a vtable store plus one zero per member;
`canonicalise_stack_objects` folds every slot name of an object onto one name, so the zeroes landed on the
handle and won. The lifter then dropped the construction as dead and folded later reads to the constant,
shipping `resources:TryAcquire(0, ...)` / `SetActor(..., 0)` / `ReleaseResource(0)`, and the sidecar threw
`LUA RUNTIME ERROR in thread 'DoMission': Invalid or released retail resource`.

Two shapes, both handled by `drop_member_zero_inits`:

* zeroes straight after the construction -- `Q_WaspBoss::WaspIntro` 0x00E12F20
* zeroes behind the alias copy `hoist_object_aliases` moves down -- `DoMission_WaspMenaceOutro` 0x00E137B0

Regenerating all 16 units moves only WaspBoss and SickChild. SickChild's `if ppuVar4 ~= xStack_10` is the
faithful read: the native at 0x00ECE460 really is `if (ppuVar4 != local_10)`, a comparison against the slot.

## Open: "Retail resource scope already owns a movie" (run 37)

Run 37 got five steps in -- channel, activate, lookout, tutorial + all nine binds, picnic crossing -- then:

    !!! LUA RUNTIME ERROR in thread 'DoMission': Retail resource scope already owns a movie
        [C]: in method 'StartMovie'
        WaspBoss.lua:252: in global 'WaspIntro'
        WaspBoss.lua:141: in function <WaspBoss.lua:108>

The guard is `LuaRetailResources.h` `StartMovie` / `NewMovie`: the scope refuses a second live movie. The
package has three `StartMovie` sites (181 in `WatchForCutscene`, 252 in `WaspIntro`, 343 in the outro) and
each has a matching `DestroyMovie`, so a leaked movie means one of those paths left its scope without
running its epilogue. Two hypotheses, neither confirmed:

1. `WatchForCutscene` (line 181, the `CS_WASPBOSS_QUEEN` movie) or another thread holds a live movie while
   `DoMission` reaches the intro -- i.e. our thread ordering differs from retail's.
2. `WaspIntro` ran twice, the first attempt dying before its `DestroyMovie`.

The log carries no StartMovie/DestroyMovie tracing, which is why neither could be separated. **First thing
to do next session: add movie create/destroy logging to the sidecar scope (or a Lua-side wrapper) and
re-run** -- that single line of evidence picks between the two.

## Also open: the teleport lands during the Lookout Point cutscene

The user watched run 37 and reported the hero stuck: the `wasp_lookout` transition fired while the Lookout
Point arrival cutscene was playing. The step needs to wait for the cutscene to finish (or be armed before
it starts) rather than crossing mid-scene. Not yet addressed.

The user added a second half to this: **pop-ups have to be cleared with a left click both BEFORE and AFTER
a teleport.** An undismissed box gates script frames, so the transition never runs and the hero stands
still -- the same symptom as the LookoutPoint park, from a different cause, which is why run 37 looked
stuck at the crossing rather than at a script wait. Both transition steps now do
`input: clear` twice, then the single `GoToMapSlotRetailTransition`, then `input: clear` again; the entity
confirmation clears too. `input: clear` (never a bare `lmb`) because it screenshots first and clicks the
box's own icon, and a bare click with a weapon drawn is an attack. The crossing is still armed exactly
once -- the clears live inside the step's `do`, not behind repeats.

## Closed: Maze's lightning phase is retail behaviour

`FinalMaze` 0x00D647F0: each damage tick of a channelled `HERO_ABILITY_LIGHTNING_SPELL` counts as its own
hit, so the 7-hit counter fills in seconds and only one or two `ON_HIT` / `SPARRING` lines clear the 5s
`SetTimer` gate. The log shows `MsgIsHitBySpecialAbilityFrom(11, SCRIPT_NAME_HERO)` returning true 8 times
and two `ON_HIT` before `VICTORY_10`. The user checked a retail recording and confirmed retail does the
same. Do not "fix" the binding to report one hit per cast.
