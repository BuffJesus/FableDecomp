# Autopilot: first live runs (2026-09-20, afternoon/evening)

Design: `docs/scripts/AUTOPILOT_DESIGN.md`. Driver: `tools/script_recovery/autopilot.py`; window driver
`tools/script_recovery/gamewin.ps1` (our copy of FableForge `tools/ingame/gamewin.ps1` + scancode keys, `lmb`, `hold`);
checklist `tools/script_recovery/checklists/guild_woods_return.json`. Everything below was seen in the v6 bundle
(Aeon LUAGameflow + our converted Guild units) on the woods-entry save `f645456fds`.

## What works now (proven in-game)

* **Exec channel**: `list` / `dump` / `eval` / `<quest>: <lua>` / `hero teleport` all answer on the FSE log within
  ~0.3 s while script frames run. First live `list` answered `LUAGameflow`, `GuildTraining`, then `GuildTrainingPreMelee`
  as it activated.
* **Hands-free front half** (`run --launch --save <profile>`): the profile's AutoSave is staged into `1234234` (what
  the frontend's `0atlas` row actually loads; originals backed up to `scratchpad/save_backup_1234234/` and restored
  in a `finally`), the bundle is launched through `ab_playtest launch` (archives the log on exit), and the frontend is
  driven by **capture -> classify -> act**: `screen_kind()` (title / menu / profiles / load / other, pinned by
  `testdata/frontend/*.png`), the profile row is picked by **hover + highlight-bar verification** (the DirectInput
  cursor walk lands ~one row / 44 px off; the list ignores arrow keys), the Load Game header is pixel-checked against
  `load_1234234.png` before the AutoSave click. Ends when the first `LuaQuestHost ... created` line appears.
* **Real input through DirectInput**: keys need `KEYEVENTF_SCANCODE` (a bare virtual key is ignored in-world; the
  frontend accepts both). TAB talks, SPACE locks, LMB attacks, Q equips, ENTER advances menu screens (Quest Start /
  Quest Completed), any click clears a game-info "Next" box, ESC skips a scene.
* **The Guild path up to the beetles quest completion ran under the driver**: talk -> PUNCH cutscene -> two tutorial
  boxes -> 7 real punches (`[HitEvidence]`, counter 2/7 seen) -> STICK -> item box + equip box -> 7 stick hits ->
  PASSED -> XP box -> ALARM -> quest card -> Quest Start (ENTER) -> `GoToMapSlot(84, ...)` into GuildWoods
  (`ScorpionHome` bound, counter 0/10) -> 10 real beetle kills (teleport onto the nearest `GuildScorpions`, SPACE,
  4 strikes; 11 rounds) -> `SetQuestAsCompleted Q_GuildTrainingWoodsMelee` -> Quest Completed screen.

## Facts learned (each from the log or a capture)

| Fact | Evidence |
|---|---|
| A game-info box (tutorial "Next", item received) **pauses script frames**, so the channel times out while one is up | `send list` TIMEOUT with the box on screen; answered right after the click |
| Cross-region: things of an unloaded region are not findable (`no thing 'ScorpionSpawn'` from the Guild) | `[Autopilot] error hero teleport ScorpionSpawn` |
| `GoToMapSlotRetailTransition` is never drained in the sidecar (`InstallRetailUpdateTransitionHook` is defined, never called) | log: `queued for the next engine update boundary`, nothing after; grep of the sidecar tree |
| `GoToMapSlot(84, 4839.04, 3689.16, 26)` (map 84 origin 4832,3648 + `GuildWoodsHSP` local 7.04,41.16) loads GuildWoods and puts the hero at the HSP | `LoadRegion(region=67, immediate)` ... `ScorpionHome` bound, counter 0/10 on screen |
| `SetThingAsKilled` / `ModifyThingHealth(t, -1000)` do **not** kill a beetle (`IsAlive` stays true, `GetHealth` reads 0.0 before and after) | `eval` batches |
| **Returning by `GoToMapSlot(70, ...)` into the Guild crashed the game** at the WoodsMelee teardown: WOODSWON setup ran (`FadeOut`, `PutUpYourSwords`, `UseCamera CAM_GTM_WD_START`), then `DeactivateQuestLater('Q_GuildTrainingWoodsMelee', 0)`, a burst of `shared_ptr Deleter` lines, no `Tearing down` line, process gone | `work/ab_runs/v6-20260920-152023/FableScriptExtender.log` lines 1563-1646 vs the walked return in `v6-20260920-134524` (903-940) where `Tearing down` follows immediately |
| Teleporting the hero while the arrival cutscene (Maze walk) runs stalls it | second launch: 8 teleports during the scene, no BOOHOO until ESC |
| The frontend menu opens on the last profile, or on the profile list after a crash | captures `fe_02.png` vs `fe_03.png` |

The return crash is unresolved: the map-slot teleport back is a non-retail path (no region transition state machine),
so the checklist's `woods_return` walks out through the gate instead (`hero teleport GuildWoodsHSP` + `hold S`), which
is how the third human run reached WOODSWON. If the walk also crashes, attach `crash_catcher.py` for the faulting
address; if not, the crash belongs to the teleport path and is not a converter bug.

## Converter defect found while reading the generated Guildmaster (not fixed yet)

`GuildTrainingPreMelee/Entities/TheRealGuildmaster.lua` after PASSED_SETUP:
```lua
quest:CreateExperienceOrb(newActorMap, 1)            -- retail: vcall 0x19c(out auStack_60, pos = auStack_118+4, 1)
-- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
quest:EntitySetCutsceneBehaviour(nil, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
...
while nil ~= nil and (nil):IsAlive() do              -- retail: while the orb (auStack_160) is alive: XP nag every 10 s
```
Retail (0x00D52E90): the orb comes back through the hidden-result slot `auStack_60`, is copied into `auStack_160`
by `CCountedPointer<...>::operator=((CCountedPointer *)(auStack_160 + 4), &pCVar6->field_0x4)` (the `+ 4` / array
spelling is what the existing `fold_counted_pointer_assign` does not match; the same idiom on a plain `&piStack_14`
in ScorpionHome 0x00D67270 IS folded and yields `guildStagBeetle`), then `EntitySetCutsceneBehaviour(auStack_160, 2)`
and the wait loop use it. Effect in-game: the orb position operand is wrong (the actor map is passed), the
cutscene-behaviour call gets nil, and the "pick up the orb" wait + nag are skipped -- the ALARM follows PASSED at once.
Fix = extend the counted-pointer copy fold to the `(X + 4)` / `xStack_` spellings and re-run the operand audit.

## Result (17:40): the whole Guild childhood path runs hands-free, 18/18

`python tools/script_recovery/autopilot.py run v6 tools/script_recovery/checklists/guild_woods_return.json --launch --save f645456fds`
-> launch, frontend, Guild arrival, BOOHOO/WAKEUP (ESC), talk, PUNCH, 7 real punches, STICK, 7 stick hits, PASSED, ALARM,
quest card, retail transition into GuildWoods, 10 real beetle kills, Quest Completed, retail transition back, WOODSWON,
END_QUESTION YES (left click), AVI, Melee stage host up, teen hero at the run tutorial. No crash, no Lua error, no
tattoo pickups (`GiveHeroObject` silent-give fix holds). Step table: `autopilot_guild_run_2026-09-20.json` (the last
step was re-run after its input was corrected: YES is the left mouse button, not ENTER).

Root causes closed on the way (all from the log, a capture, or the crash catcher):
* **Return crash = the bare `GoToMapSlot` entry**, not the scripts: `crash_catcher.py` caught EIP 0x821878
  (`mov esi,[edi+8]` with edi = 0) called from 0x4fd020 <- 0x6c24d8: the 0x6c2170 loader loop walks a vector of 0x1c-byte
  entries and takes `[[obj+0x20] + idx*72 + 0x10]` per entry, NULL for the map the bare teleport "entered" without the
  transition state machine. Fix: the sidecar now installs `InstallRetailUpdateTransitionHook()` (22affe1) so
  `GoToMapSlotRetailTransition` (CWorld::SetAsLoadingRegion at the CMainGameComponent::Update boundary) is actually
  drained; both crossings use it. 3/3 crashes before, 0/1 after (plus the earlier walked run).
* **Never arm the transition twice**: a repeated `GoToMapSlotRetailTransition` while already in the woods ended
  `LUAGameflow`'s Main loop and tore its host down (frozen unkillable beetle, 0/10 -- the user saw it).
* A background Fable window freezes cutscene waits (5-minute BOOHOO hang); the driver refocuses every 15 s.
* Game-info boxes pause script frames; menu screens take ENTER, boxes take a click, the YES/NO question takes LMB/RMB.
* Markers logged between two steps were lost (PASSED_20 three seconds after PASSED_10): one log tail per checklist
  with the unconsumed remainder carried into the next step.

## Night: Melee stage hands-free, Skill stage reached, two in-game Lua errors fixed at the converter

* **Melee checklist** (`checklists/guild_melee_stage.json`) runs the whole teen melee stage: INTRO -> talk -> 7 sword
  hits (Q once per stage: Q TOGGLES the weapon) -> BLOCK (hold MMB standing still, click the re-popping box) 5/5 ->
  BATTLE -> grade -> Continue -> Skill quest activated. Chained after the childhood checklist from launch:
  `autopilot.py run v6 checklists/guild_woods_return.json checklists/guild_melee_stage.json --launch --save f645456fds`.
  New driver verbs: `skip` (ESC only when the pause menu is not up), `clear` (click only when a game-info box /
  question is detected by its green mouse icon -- a click with a weapon drawn and no box is an ATTACK: three of them on
  the SkillApprentice were the Guild's third friendly-attack warning), `hold LMB|RMB|MMB <ms>`, `focus`.
* **Two converter defects behind the user's teen play-through reports** (archery target dead; Whisper fight
  unfinishable after the "secret trainer"): `SkillTarget.lua:66 attempt to compare number with nil` = the projectile
  damage out-param (`MsgIsHitByHeroWithProjectileWeapon(float&)`; the binding returns damage-or-nil, now
  `OUT_AS_RESULT`), and `CheckFriendlyAttacks: TryAcquire requires an actor` = the PreMeleeMaze thing (one Ghidra
  slot the export spread over -0x90/-0x80/-0xa0; `restore_stack_operands` now keeps a vtable thing-cast name at
  its first slot). Also: a GSI result (`AddNewConversation`) is never a resource member (`canonicalise_stack_objects`),
  and a resource stored in an actor map keeps its own name (the BADHERO HERO actor). Units regenerated; the Oakvale
  gate identical; `test_skill_target_and_friendly_actor.py`.
* **Broader identity rules were tried and REVERTED** (destructor receivers / bare-array ctor receivers / Ghidra-name
  type families): two audit workflow rounds (37 agents each, adversarially verified) found 9 regressed scripts
  (nil actor maps, `DestroyMovie(0)`, unassigned locals). Residual: the BADHERO hero-resource release is still spelled
  by the actor map's slot.
* **Open (evidence from the last run's log)**: after the third friendly-attack warning EVERY quest thread was
  terminated at once (`LUAGameflow: Main loop exited` -> host torn down for good; all GuildTraining entity Mains
  returned -> the archery targets were dead). The same happened when the retail transition was armed twice. The
  sidecar tears a host down when its Main returns instead of restarting the thread as retail does. A passive
  `CWorld::SetAsLoadingRegion` diagnostic hook (`[RegionDiag]`, sidecar 3a522f2) now logs the trigger + caller.
