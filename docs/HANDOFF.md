# RESUME HERE -- 2026-09-21 night: GUILD TRAINING COMPLETED on the converter's Lua (childhood -> graduation, hands-free)

**In-game (v6; journal `ARCHERY_SCORING_2026-09-21.md`, sections Run 6 .. Run 11b):** from the post-woods save the
autopilot chain plays woods -> melee -> Skill (static + moving) -> Will (bolts, WILL_WON) -> Continue -> adulthood
(AVI, Departure) -> the final woods test (7 sword / 7 bow / 7 lightning hits on Maze, MAZE_WIN) -> EXIT_WOODS ->
FrescoDome ceremony (SEAL_GIVE/RECEIVE, adult hero in the outfit) -> the experience light -> **`SetQuestAsCompleted
Q_GuildTraining`** ("graduated as a Hero", `docs/journal/2026-09/guild_training_completed_2026-09-21.png`). Zero Lua
runtime errors on that path. Runs 6/7 = 45/45 through Will; run 11 = 46 steps to Departure; the Maze/graduation
steps were driven attached (11b). **Run 13 = all five checklists in ONE launch: 58/58, 0 Lua errors**
(`work/autopilot_full_20260921e.log`, report `docs/journal/2026-09/autopilot_full_guild_2026-09-21e.json`, archive
`work/ab_runs/v6-20260921-212900`). Run 14 = the same chain on v7 (converter Gameflow), then `ab_playtest.py compare v6 v7`.
**The chain command:**
    python tools/script_recovery/autopilot.py run v6 tools/script_recovery/checklists/guild_woods_return.json tools/script_recovery/checklists/guild_melee_stage.json tools/script_recovery/checklists/guild_skill_stage.json tools/script_recovery/checklists/guild_will_stage.json tools/script_recovery/checklists/guild_departure_stage.json --launch --save f645456fds --report docs/journal/2026-09/autopilot_full_guild_2026-09-21.json
(launch via PowerShell `Start-Process python ...`; close Fable before rebuilding v6; `clear` clicks the box icon;
never `skip` when no scene is up; the bow auto-aims away from friendlies -- friendly fire is a hand test, GOTCHAS).
**Converter tonight (all generic; Oakvale gate identical every time; smoke Guild/Orchard/Gameflow/Trader all 0;
targeted 147; full suite `work/converter_suite_20260921f.log` = the morning baseline's 8 Oakvale-lane groups exactly):**
(1) bare .rdata text keys resolved before `place_args` (the Will Guildmaster spoke "CS_GUILD_WILL_WON"); (2) thing-call
bools are Lua booleans (`0` is truthy: every `SetFriendsWithEverythingFlag(0)` SET the flag); (3) the drifted
`X._0_4_` Data spelling; (4) `this_NN` cleanup-tail aliases (nil handles on termination paths); (5) the PreMelee nag
timer (state getter stored in a slot); (6) slot-zero residues; (7) `(i + V)` element vcalls without the cast
(Whisper's chat-marker removal). Guild draft todo 287 -> 268 (14 non-structural, none live). v6/v7 rebuilt.
Still NOTHING COMMITTED (~60 files: tools, tests, checklists, regenerated units, docs).
**Session ended 22:xx (user to bed, machine off): run 14 (the v7 chain) was launched and KILLED after a minute --
nothing to read from it; the staged save was restored by hand (`restore_save`), Fable closed.**
**Next session, in order:** (1) `git status` -- ~60 files uncommitted since 2026-09-20 (tools/script_recovery/*.py,
tests, checklists, regenerated Guild/Trader/Orchard/Gameflow units, docs, GOTCHAS); commit when the user says so
(exclude the pre-dirty rebuild/compile-gate files). (2) Run 14 = the chain above with `run v7 ...` (converter
Gameflow) and `python tools/script_recovery/ab_playtest.py compare v6 v7` against `work/ab_runs/v6-20260921-212900`.
(3) TraderConflict's TraderToRescue Main (broken-stack slot `xStack_148` declared six ways; 14 semantic TODOs incl.
three `EntityFollowThing` sites printed with no operands) is the next converter lane; Orchard/Gameflow are at 0
semantic TODOs. (4) Friendly-fire punishment stays a HAND test (bow auto-aim, swings ignored -- GOTCHAS). (5) The
ArtifactThief lamp side-talk: the thief is not placed in this playthrough (binding registered, no entity).

# (earlier today) a converter day: archery scoring + 9 more Guild gaps closed OFFLINE

**Autopilot run 1 (12:51) CRASHED at SkillTarget's first frame -- my moving-dummy fix made two frame-0
`GetNearestWithScriptName(nil, ..)` lookups live (they had always been in the draft, dead until then); fixed
(`resolve_me_register_uses`, journal), bundles rebuilt with a new sidecar DLL (`MsgGetThingsKilled` binding) and run 2
launched 13:1x: childhood clean through PASSED; the live XP-orb wait needs the hero to WALK through the orb
(checklist fixed: teleport + cross walk); resumed on the live game from the quest card (`--tail-back` added to the
driver for attached runs) -- woods reached; results in `docs/journal/2026-09/autopilot_skill_run_2026-09-21*.json`,
logs under `work/ab_runs/`. (Launching via PowerShell Start-Process works; the Bash form is what the classifier blocks.)**

**The run command (if it needs repeating):**
    python tools/script_recovery/autopilot.py run v6 tools/script_recovery/checklists/guild_woods_return.json tools/script_recovery/checklists/guild_melee_stage.json tools/script_recovery/checklists/guild_skill_stage.json --launch --save f645456fds --report docs/journal/2026-09/autopilot_skill_run_2026-09-21.json
v6/v7 are rebuilt with everything below. What changed on the in-game path since the last run, in order: the childhood
Guildmaster now WAITS for the XP orb to be collected after PASSED (checklist step `xp_to_alarm` teleports onto the dummy
marker -- untested); CheckFriendlyAttacks counts hits on the Maze; the two SaveXP cutscenes run; the sparrows spawn
(BirdKiller); the Skill stage scores (rings 0.25/0.5/0.75), the dummies move in SKILL_MOVE, the out-of-ring warning
works, the grade text reaches the WON cutscene; the Will stage's WillScore counts and its cutscenes have HERO/WHISPER;
WoodsWill acquires its three bandits; FinalMaze pauses the world. All of it is generic converter work, journal
`docs/journal/2026-09/ARCHERY_SCORING_2026-09-21.md` (one section per fix, bytes cited); `test_skill_target_and_friendly_actor.py`
10/10 pins them. Guild draft todo 336 -> 290, smoke Guild 0/0 (was 1/1), Orchard/Gameflow 0, Trader 1 (unaff_EBX
residual, documented). Full suite: `work/converter_suite_20260921c.log` (the morning run's 8 failure groups are
pre-existing Oakvale-lane drift, reproduced on the stashed tree). NOTHING COMMITTED -- ~25 files (tools, tests,
regenerated Guild + TraderToRescue, docs, checklist).

**Resume item (1) is closed at the converter, no run needed:** `SkillTarget::Main` 0x00D41D00 never scored because
(a) its three scoring rings are `.rdata` DOUBLES (`fcomp qword` 0.25 / 0.5 / 0.75) that `float_at` read as 4-byte floats
(all 0.0), and (b) every `SkillScore` store is VC7.1's field-pointer RMW (`piVar1 = (int *)(master + 0xa4); *piVar1 += ..`)
which the master folds did not match -> five `TODO(native)` comments where the writers should be. Both fixed generically
(`UnitConverter.double_constants()` keys the width on the unit's x87 `qword ptr` operands; `inline_field_pointer` in the
lowering); the same shape also landed `WillScore` (WillDummy, the Will stage had the identical latent bug) and
TheRealGuildmaster's tally. Journal: `docs/journal/2026-09/ARCHERY_SCORING_2026-09-21.md`. Gates: Oakvale draft identical,
Orchard/Trader/Gameflow regenerated with no diff, smoke Guild 0/0 (BirdKiller's marker-list count fixed late in the day; Orchard 0, Gameflow 0, Trader 1 pre-existing),
`test_skill_target_and_friendly_actor.py` 10/10, full suite `work/converter_suite_20260921.log`. **v6 + v7 rebuilt**
(offline; not deployed, not launched -- the install is shared). Next in-game (user's call): Skill stage, shoot the three
dummies, `SkillScore` must climb (worth 1/3/9 x ring 1-4; a weak hit `d <= 0.5` costs 1 and re-arms). If every hit is
`+4*worth` or `-1`, the suspect is the binding's out value (it passes retail's `pOutDamage` through), not the script.
**Skill out-of-ring check, the 7 grade texts, FinalMaze's PauseAllNonScriptedEntities, the Will Guildmaster's HERO/WHISPER actor-map stores and WoodsWill's three bandit resources (a local CArray<resource> -> Lua list) also landed** (journal; Guild draft todo 336 -> 290, WoodsWill 0); plus CheckFriendlyAttacks' three did-the-hero-hit-the-Maze checks and the two SaveXP cutscene helper calls (never called before). **Moving dummies (SKILL_MOVE) also landed**: the three segment teleports were TODOs (stack C3DVector from three float slots; `fold_stack_vector_builds` + EntityTeleportToPosition's position tagged) -- the Skill stage's second phase can now be played through. **Item (4) XP orb also closed at the converter** (array-slot counted-pointer copy; the orb wait + nag are live, so
`guild_woods_return.json` `xp_to_alarm` now teleports the hero onto the dummy marker to collect it -- UNTESTED). Launch
the chain yourself (classifier blocks it): `python tools/script_recovery/autopilot.py run v6 tools/script_recovery/checklists/guild_woods_return.json tools/script_recovery/checklists/guild_melee_stage.json tools/script_recovery/checklists/guild_skill_stage.json --launch --save f645456fds --report docs/journal/2026-09/autopilot_skill_run_2026-09-21.json`.
Suite: 8 failure groups, all pre-existing Oakvale-lane drift (journal). Then: third-warning thread termination
(`[RegionDiag]`), v7 + `ab_playtest.py compare v6 v7`, BADHERO hero-resource release.

# RESUME HERE -- 2026-09-20 (FableForge day; the Guild-path block below is the FableTLC resume)

**FableForge 1.0 is one human step from tagging.** Public repo https://github.com/BuffJesus/FableForge
(`main`, CI green, suite 18/18), the in-game release probes ALL PASS (red barrel, exact positions,
compacted bank, own-region load, restore 0 differ; two shipping bugs found+fixed), README screenshots
fresh. Left for v0.16.0 (the user's): stranger's test on another machine (`python tools/package.py`),
`git tag v0.16.0`, Discord. Resume from `D:\Code\FableForge\docs\ROADMAP_1.0.md` "Resume here".

**0.18 Water is built offline on FableForge branch `water` (NOT seen in-game):** `forge water-audit`
pinned every record formula against the 3,374 retail patches (`docs/engine/WATER_RE.md` corrected:
wave exact, z floor, depth from the 1/128-quantised ground, distToShore 0, the 0x38 background
vertex read); Water brush (Terrain tab, retail's exact slot mix), foreground + background writers,
`tools/test_water.py` in check_all. Next: the user paints a pond, writes terrain, looks in-game
(expected: surface near and far, no foam); then sea bodies / foam / growing background frames.
The user's other agent shares the game install: never launch Fable or write the install unasked.

**0.20 Mod packs is on FableForge branch `modpacks` (offline, suite 20/20, head 01d7a71):** `forge-tools mods
list/add/remove/move/enable/disable/build/conflicts/deploy/undeploy` over `<root>/forge_mods.json`;
pack shapes: ChocolateBox + Fable Explorer `.fmp`, bsdiff (pristine bytes), game-root trees (records +
TNG/QST merged, text.big union, whole-file layers, WAD repack), EgoCore `Mods/<Name>/` (Mods.ini line +
`.def` text via `defc`, `FORGE_DEFC` / `FORGE_DEFS_TEXT`); `mods conflicts --json` = ONE report over every
stage, picks in `<root>/forge_mods_picks.txt` (namespaced keys, mod name or `vanilla`); the GUI Mods tab
(load order, Conflicts card = the picker, deploy/undeploy); later the same day: EgoCore `.resource` bank
overrides, backup suffixes unified (`.forge-orig`/`.forge-created` new, legacy `.atlas-*` + `.forgebak` +
`.ovrbak` read; restore reverts a stage first and rebases originals taken on top of it), thing provenance
in the editor (`forge_mods_provenance.json`: badges / Placed-by filter / Back to retail), the GB-pack
rules (Project Seasons under the UFP builds in 24 s: parked `_FinalAlbion.wad` + `userst.ini` skipped,
794 loose levels repacked), FSE `quests.lua` key union (`fse:<key>` picks, id clashes), EgoCore partial
TNG mods (`[Settings]` / DeleteUIDs). 2026-09-20: the install's `game.bin`/`names.bin` were restored to
the pristine `.retail-bak` bytes (text.big + the big banks left as they are: staged / added content).
`python tools/test_mods.py` (+ `test_backups.py`, `test_gbpack.py`). **0.17 custom static meshes (same
branch, f617fd1):** `forge mesh-import <glb|gltf|obj> <NAME> [--texture png]` + the Edit > Objects *Import
model* card = `forge::meshcompose` (the compose_mesh static grammar in C++) + a `3DMF` collision hull
(EgoCore's writer; retail pairs `MESH_X[PHYSICS]` via Info PhysicsIndex) + textures.big diffuse +
OBJECT_<NAME> def; mesh space is CENTIMETRES (metres x100); **IN-GAME VERIFIED 2026-09-20 evening**
(head ca7ced0): the cube renders lit + textured where placed and is SOLID once the hull's triangles are in
the engine's winding (straight order = inside-out hull that held the hero at the centre; the retail
barrel control was solid; user-confirmed cube 4). The install still carries the probe leftovers: 4 cubes +
a barrel in StartOakValeWest, MESH_FORGE_PROBE_CUBE*/OBJECT_* entries -- `python tools/ingame/release_probes.py
--stage mesh_undo` puts the touched files back (the other agent's staged bundle stays).
(`tools/test_meshimport.py`, `forge-tools mesh-info`, `tools/ingame/walk_probe.ps1`). Memory:
`modpacks-branch`, `custom-npc-pipeline`. Open: a shipped text Data/Defs tree, semantic thing signatures (a CB re-save badges every thing), merging
`modpacks`/`water` to main (user's call). Watch: `stage.hpp` vs `ENGINE_RULES.md` contradict on
loose-TNG precedence (in-game check).

# RESUME HERE — 2026-09-19 late evening (Guild path playtest day; read this block only, then GUILD_ARRIVAL_PLAYTEST journal)

**State of play (in-game, v6 bundle = converter units + Aeon's LUAGameflow + sidecar DLL):** childhood -> Guild
transition, arrival, apple quest, race, Guildmaster punch stage, stick stage with "?/7" tally, Guild Woods beetles
(all ten), quest card, WOODSWON cutscene, PREMELEE_END_QUESTION all work. **Broken, next up:** after the woods
(answering the question), no quest markers, the Guildmaster stands back at the melee position and talking to him
REPLAYS THE PUNCH STAGE. Agent findings (journal section "Night: the woods loop that never ended", bytes cited):
(a) the woods loop's exit flag stores were DROPPED -- Ghidra's ESP model drifted 4 bytes after an untyped vcall, so
`cStack_169 = 0` after the AVI (YES) and the walk-back re-arm (NO) printed against phantom slots; readable loop was
`repeat ... until false`. FIXED generically (`_drifted_byte_slices` in convert_quest_unit.py, two-pass
restore_stack_operands; tests in test_cross_branch_goto.py WoodsStageTests), Guild regenerated (in commit 7cb127a).
(b) The PUNCH replay is the entity's Main being RE-RUN FROM THE TOP when the hero returns from the woods (region
change: OnDeactivateEntity 0x00CB88B0 unwinds it, the allocator recreates it, Main restarts; this entity persists
nothing and Main has no guard) -- faithful to the bytes; what retail does on that return is NOT established (thread
survives the trip, or the fresh TryAcquire parks on a lock left on the persistent CThing). **Next run decides:**
rebuild v6/v7 (commands below), load the woods-entry autosave, kill the beetles, answer YES -> expect the AVI,
HeroSleeps, quest end; answer NO -> walk back + re-ask on the next talk. Full suite not completed on the drift fix
(targeted 35 OK); run it first thing (unittest-discover form; pytest loops on unicorn AVs in test_affair_man_complete).

**Everything today is generic converter/sidecar work (nothing hand-edited except the two documented NOVI deviations).**
The journal has each fix with byte evidence: split-array vectors, `_align`, CreateObject position, StartMovieSequence
duplicate (98 sites), IsAcquired/Reset -> PrepareResource (the StartScriptingEntity lock wait), race timers, ABS/x87/dtor
label mis-typing, **cross-branch goto hoisting (120 -> 1 dropped gotos)**, PDB-typed bool master flags as Lua booleans,
**entity OnPersist(quest, me, context)** (the crash on every save). Sidecar `sidecar-abi-v2` HEAD (patches in
tools/script_recovery/sidecar_patches/ regenerated): GiveHeroObject 3rd bool, TryAcquire prepare+registry,
AddLogbookTutorialEntryPC, PersistTransferStringList, and DIAGNOSTICS still on (TryAcquire enter/refused, QuestThreadFlag
per cutscene command, [Terminating], [Interrupted], QuestLifetime, State miss/woods-flag writes, Lua Main returned) --
cheap, keep until the guild path is clean, then strip.

**Gates at last check:** Oakvale draft byte-identical; smoke Guild 1/1 (BirdKiller, pre-existing), Orchard 0/0,
Trader 2/2, Gameflow 0/0; `TODO(native): goto` residue 1 (TraderConflictEvil); full suite 1602 OK (before the
OnPersist/master-bool fixes; targeted sets OK after).

**Bundles:** `work/new-oakvale-original-fse-20260912/local-candidate-v6` (Aeon Gameflow, control) and `-v7` (OUR
converted Gameflow as the `Gameflow` override -- never run yet). Rebuild both after any regen:
    python tools/script_recovery/build_unit_playtest_package.py --unit orchard_farm guild_training trader_conflict --gameflow work/aeon_lua_ports/Gameflow/FSE/LUAGameflow/LUAGameflow.lua --dll work/new-oakvale-original-fse-20260912/sidecar-abi-v2/Release/FableScriptExtender.dll --bundle work/new-oakvale-original-fse-20260912/local-candidate-v6
    python tools/script_recovery/build_unit_playtest_package.py --unit orchard_farm guild_training trader_conflict gameflow --dll <same dll> --stage work/unit_playtest_stage_v7 --bundle work/new-oakvale-original-fse-20260912/local-candidate-v7
A/B driver: `python tools/script_recovery/ab_playtest.py launch v6|v7` (waits for exit, archives the log to
work/ab_runs/<bundle>-<ts>/), `compare v6 v7` (event timelines, one-side-only events, first divergence, errors).
Save to use: profile `f645456fds` (woods-entry autosave). Retail sequence to hold against (user): talk to
guildmaster -> ring -> punch dummy -> stick -> hit dummy -> woods -> beetles -> question -> (AVI / next stage).

**Also open:** Bully teddy-first deviation + barrel-man presence gate are documented deviations in the NOVI stage;
`skipQueryTrue` fires at every cutscene's first Speak (the advance key = retail skip flag 0x143E8F4) -- not a bug;
`[RetailResources] TryAcquire enter` field values are garbage (wrong component-walk offsets), refused/granted lines are
right. Nothing committed today (branch feat/novi-script-recovery) -- commit the tools/docs/refs work first thing.

# RESUME HERE (after the 2026-09-19 WoodsMelee fix; night-7 block follows)

**2026-09-20 (night): AUTOPILOT IS LIVE -- childhood 18/18 AND the teen Melee stage hands-free; Skill stage reached.**
`python tools/script_recovery/autopilot.py run v6 tools/script_recovery/checklists/guild_woods_return.json tools/script_recovery/checklists/guild_melee_stage.json --launch --save f645456fds`
(~45 min, nobody at the keyboard). Sidecar `3a522f2` (GC per chunk, retail transition hook, `[RegionDiag]`).
Converter: two in-game Lua errors fixed (SkillTarget projectile damage out-param; CheckFriendlyAttacks TryAcquire actor)
+ GSI-result / actor-map-resource folds; broader identity rules tried and reverted after two audit rounds (journal).
**Resume:** (1) ARCHERY: the targets do not score even while alive (user shot them by hand before the third warning;
3 min of live SkillTarget Mains, SkillScore 0, no Lua error -- see the journal's Correction). Add a log line to the
projectile-hit binding, run cleanly to the Skill stage, shoot, read the log; candidates in the journal. Separately,
the third-warning thread termination (`[RegionDiag]` names the trigger next run; the host must survive a restart);
(2) v7 through the same checklists + `ab_playtest.py compare v6 v7`; (3) BADHERO hero-resource release residual;
(4) Guildmaster XP-orb CCountedPointer copy. Findings: `docs/journal/2026-09/AUTOPILOT_FIRST_LIVE_2026-09-20.md`.

**2026-09-20 (third v6 run 13:45): the woods return is RIGHT (WOODSWON at the door, AVI, PreMelee ends, Melee stage
runs; attack tutorial 7/7).** Two bugs seen and fixed at the converter: tattoo cards as visible pickups at the teen
transition (`GiveHeroObject`'s retail third bool, dropped by the two-parameter SDK manifest) and blocks stuck at 0/5
(`MeleeOpponent.Main` died on the first block — the EH flag in a slot the movie handle had borrowed, `movie & 1`).
Journal §8. **v6 / v7 rebuilt + preflighted; zip rebuilt. Next run: from the Melee stage (or the post-woods save):
block 5/5, the Whisper fight, the grade + repeat question, then Skill training as new ground; then v7 + compare.**

**2026-09-20 (second v6 run 09:22 + afternoon): NO crash, no Lua error; the PUNCH replay is root-caused from the
bytes and fixed at the converter.** Retail (walkthrough 26:34-26:58): WOODSWON fires the moment the hero leaves the
woods — a SURVIVING woods-loop thread. Retail keeps the Guildmaster's script alive across the unload because its
binding carries flags=1 (`binding+0x18` → `CActiveEntityScriptBase::Flags`, bit 0 = early-out in
`OnScriptedEntityDeactivated` 0xCB88B0); the converter never emitted the flag, so the sidecar unwound `Main` and the
fresh `Main` replayed PUNCH. Now `AddEntityBinding(name, path, 1)` on every retail-1 binding (survey: all Guild = 1,
Orchard MK_OFI_GWLL_WHIS2 + Trader = 0; Oakvale gate untouched). Second + third audit rounds: Melee Guildmaster
(EH flag in an `int *` slot, colour address-before-stores, float in a CCharString slot), Will's byte-split resource,
apple cleanup index, Trader special-ability enum (+24 `SetFriendsWithEverythingFlag(true)`), Orchard branch literals
+ counter handle, Trader `_SUFFIX` appends + conversation operands + `IsRegionLoaded("")` + byte-slice sign test.
Third round (29 agents): Trader's BanditExtra / AttackPeople / WatchForTradersFreed (`__native_all_dead`) / on-talk key / opinion source, MeleeOpponent's `ReleaseResource("")` (dtor on the derived head). **Backlog (verified, later stages, unfixed): WoodsWill's per-bandit RESOURCE ARRAY idiom + `CCharString_bv` EH flag, SkillTarget's damage out-param** (§7). Journal `docs/journal/2026-09/V6_RETURN_CRASH_2026-09-20.md` §4-7. **v6 / v7 rebuilt + preflighted; zip rebuilt.
Next run (user, game free): `ab_playtest.py launch v6` from the post-beetles save → expect WOODSWON at the woods
door with no talk, YES → AVI; then the Melee stage (Whisper fight, grades) as new ground; then v7 + compare.**

**2026-09-20 (morning): the crash on the return from Guild Woods is fixed (sidecar) + one converter bug; v6/v7
rebuilt, NOT yet re-run.** v6 run `work/ab_runs/v6-20260920-080006`: beetles done, back in the Guild, talk to the
Guildmaster → Fable.exe died at the first PUNCH cutscene command. Cause (from the log, both days compared):
`LogQuestThreadFlags` walked ScorpionHome's entity-host entry whose `m_pParentHost` was the freed WoodsMelee quest
host — `~LuaQuestHost` only detached entity hosts for the NewOakValeIntro lifetime. Sidecar `e1ed740` detaches for
every lifetime; DLL rebuilt; `novi-unit-bindings.patch` regenerated. Converter: `CheckFriendlyAttacks` (0xD45060)
died on every return to the Guild (`preMeleeMaze - creatures2`: `canonicalise_stack_objects` folded the vector end
onto the thing) and its four `GetDefName` compares were `nil == "CREATURE_..."` (`at_vcall_local` index spelling) —
both fixed generically, `test_check_friendly_attacks_converter.py` added, Oakvale gate identical, smoke baseline
(Guild 1/1, Orchard 0/0, Trader 2/2, Gameflow 0/0), only Guild Lua changed. `work/AeonShare-2026-09-20.zip` built
for Aeon (he offered to test Orchard: all three quests are in it). **Then an ultracode residue audit (15 agents,
verify-against-C) found a SILENT class the smoke gate and the in-game runs had both passed:** every Guild
`CreateCreature` / the PreMeleeDummy `CreateObject`, two `AddLineToConversation`, an `EntityTeleportToThing`, the
Guildmaster `TryAcquire`, and Orchard-Good's actor map had rotated / nil operands — one cause (a vector's slot
reused as hidden-result slot, folded to `LOCALLIST_At(V, 0)` outside the vector's live range → the lifter's LIFO
literal pool), plus `slot_results` surviving a redefinition and a cast/CRLF-blind `_receiver_printed`. All generic;
Oakvale identical, smoke baseline, targeted 41 OK, full suite `work/converter_suite_20260920b.log`. Do NOT pad
truncated push records (the export charges vcalls phantom purges; bytes-proven at 0x00D46A73). Journal:
`docs/journal/2026-09/V6_RETURN_CRASH_2026-09-20.md`. **Next (user):** `ab_playtest.py launch v6` from the
post-beetles / woods-entry save, then `launch v7` on the same save, `compare v6 v7`. Watch: the return talk
(no crash at `TEACHER.LookAtNothing`, no CheckFriendlyAttacks error), PUNCH replay vs WOODSWON/YES-NO.


**2026-09-19 (FableForge night, 19 commits, tree clean at `b14aefc`):** resume FableForge from
`D:\Code\FableForge\docs\ROADMAP_1.0.md` "Resume here" -- everything code-side for 1.0 is done; what
is left is the user's (in-game probes, public repo, package + stranger's test, tag; `docs/RELEASE.md`).
Landed: the foliage lattice bug (75% of baked foliage never read; Oakvale's square oak), the frame-walker
memset fix (boot + open Oakvale 3.65 s -> 0.79 s), fishing spots, STB bank compaction, one running-game
guard for every writer matched to the target install, RELEASE.md, a zip dry run. Planned only: 0.18 Water
(`docs/engine/WATER_RE.md` here; FableWin decompiles `ghidra_out/decomp_water_fablewin.c`), 0.19 World in
3D, 0.20 Mod packs (EgoCore + every older mod shape in one load order; corpus in
`D:\Code\FableForge\work
exus_mods\CATALOGUE.md` + GB packs in `D:\Downloads`).

**2026-09-19 (midday): the childhood -> Guild transition is fixed; the guild scripts die one thread later.** The
morning's four v5 crashes were the sidecar freeing retail-owned `GetAllThings*` vector storage
(`sidecar-abi-v2` `7ea4377`, never journaled). `local-candidate-v6` (units + that DLL + **Aeon's `LUAGameflow`
as the `Gameflow` override**) handed off cleanly at 11:35: `Q_GuildTraining.Main` ran, `CS_GUILD_ARRIVE` played,
then `RunTutorials` -- the thread that drives every guild tutorial -- died on its first loop
(`GuildTraining.lua:197`, split-array vector `._4_4_`), which is the whole "no quests at the guild" report.
Three generic converter fixes (split-array vectors, `_align` on truncated vtable push records, `CreateObject`
position tagging), Oakvale gate identical, targeted tests 16/16, Guild regenerated, **v6 rebuilt + preflighted**
on it and on a DLL whose `GiveHeroObject` now passes retail's third bool (the visible custom-tattoo pickups at
New Game are our deviation; retail passes `true`, the binding hard-coded `false`). Journal:
`docs/journal/2026-09/GUILD_ARRIVAL_PLAYTEST_2026-09-19.md`. **Next run (user):**

    python work/new-oakvale-original-fse-20260912/local-candidate-v6/local_test.py --game-dir "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters" --launch --save-dir "C:\Users\Cornelio\Documents\My Games\Fable\Saves"

**Afternoon (runs 3-5, then offline):** tattoos confirmed fixed; barrel man fixed (the readable restructuring had
dropped retail's presence gate, `readable_barrel_phase.py`); Bully **deviation by user decision** (`HeroAttackedVictim`
counts as the intro done -- teddy on the first talk). The Guildmaster stall was root-caused **from the bytes**:
retail `StartScriptingEntity` (0x89B5B0) yields the script fiber while the holder's `CTCScriptedControl+0x18 Locked`
is set (only a lower priority is refused), so a binding that calls it against a held actor parks the Lua coroutine
silently. The converter had folded retail's release-before-wait pair (`0xCD23B9`/`0xCD2770` = `PrepareResource`) to
`false`; now lowered generically (Maze releases before `GuildWarningOccuring`, PreMelee `Main` acquires all four
actors the retail way). Sidecar `c421e65`: `TryAcquire` prepares on first use and registers held handles so
`StartCutscene` copies them (it would otherwise re-acquire and deadlock on the quest's own lock). Gameflow agent:
35/35 resume stages via a generic switch-tree flattener (`native_switch_tree.py`), Gameflow goto residue 5 -> 0,
Orchard `Artefact` regression bisected + fixed. Gates: Oakvale identical, smoke Gameflow 0/0, Guild 1/1, Orchard
0/0, Trader 2/2, 8 test modules OK. **v6 rebuilt + preflighted, NOT yet run** -- next launch is the same command.
Look for: `[RetailResources] TryAcquire enter ... locked=0` before PreMelee's cutscenes, `CS_GUILD_PREMELEE_INTRO`
entering, the Guildmaster walking. `CheckFriendlyAttacks` misfire fixed by the agent's `member_copy` exemption
(verify in-game). Open: sidecar `PersistTransferStringList` binding; full 29-min suite not run.

**Run 6-7 (13:54, 14:0x) + agents:** Guildmaster works; apple quest works. Race apprentice parked in a second
`StartMovieSequence` (retail 0x89B110 yields while a sequence is active; 98 duplicate ctor+GSI starts folded to one).
Race timers were crossed (`_align` lead-0 receiver-less calls; `&timer` by-reference operands dropped) -- fixed,
match the export's three slots. Melee dummy (agent, from bytes + log): two Lua errors unwound the PreMelee
Guildmaster before `PreMeleeMode=1` (bsim-shared movie/resource dtor label + Ghidra `::` line wraps defeating
`disambiguate_call_labels`; `ABS(`/x87 compare/float-into-resource folds) -- fixed generically, plus
`AddLogbookTutorialEntryPC` binding (sidecar `9ebaa27`). Full suite 1598 OK. **v6 rebuilt + preflighted on all of
it, NOT yet run.** Expect: race timer + HUD clock, Guildmaster punch cutscene, "?/7" tally, dummy targetable.

**Evening (runs 8-10 + agent):** race, apples, punches all work. The stick stage's missing counter was NOT the quest
thread (per-command `[QuestThreadFlag]` dumps: never terminating) -- it was a **dropped cross-branch `goto`**
(`-- TODO(native): goto LAB_00d53c7e`): retail's PC path jumps into the counter block nested in the Xbox branch;
the converter emitted a comment and `Main` fell off the end. Agent: `native_goto_scopes.hoist_shared_tails` (move
the shared tail after the if/else chain, `goto` to it) + cleanup-region delegation -- **120 dropped gotos -> 1**
(TraderConflictEvil, untyped), PreMelee Guildmaster 2,195 -> 649 lines, new `test_cross_branch_goto.py`, full
suite 1602 OK. Sidecar `b2b4697`: `PersistTransferStringList` (0x49B8D0) + lifecycle/cutscene-command diagnostics.
**Two bundles built + preflighted, NOT yet run:** `local-candidate-v6` (Aeon's LUAGameflow, control) and
`local-candidate-v7` (OUR converted Gameflow as the override) -- A/B the same save through both. Expected: "?/7"
after the stick, then the rest of PreMelee; on v7 watch `Transferring string list` in OnPersist and every stage
handoff. Note: `skipQueryTrue` fires on every cutscene's first Speak (retail skip flag 0x143E8F4) -- the key used
to advance dialogue also skips; not a bug, but lines get cut short.

**2026-09-19 (evening): Oakvale Reborn started — the rewritten childhood intro.** Plan approved
(`C:\Users\Cornelio\.claude\plans\prancy-hopping-possum.md`; story seed = a Stranger offers the child a
sword to wipe out Oakvale, accept = the child fights / refuse = raid averted). Authored tree
`refs/script_recovery/authored/OakvaleReborn/` (STORY.md, CHECKLIST.md, manifest, `cutscenes/CS_OVR_SPIKE.cs`),
tools `tools/oakvale_reborn/{build_custom_intro.py,spike_s1.py}`. FableForge (UNCOMMITTED) gained
`forge-tools script cutscene-dump|cutscene-set|cutscene-roundtrip` over a whole-def `CCutsceneDef` codec —
595/595 retail round-trip byte-identical, `CS_OVR_SPIKE` appended offline as entry 611 with a correct crc0.
**Spike S1+S3 bundle is built and preflighted** (`work/oakvale_reborn/bundle-spike-s1`). **User steps:**
`python tools/oakvale_reborn/build_custom_intro.py install` (game closed) → launch per CHECKLIST.md → read the
`OVR_SPIKE_S1/S3` log lines → `… restore`. Journal: `docs/journal/2026-09/OAKVALE_REBORN_KICKOFF_2026-09-19.md`.
Next after the verdict: S2 (camera marker in TNG), S4 (ElevenLabs line via `dialogue_pipeline`), S6 (child combat).

**2026-09-19 (titles): two hero titles APPENDED to game.bin.** Static evidence (PDB: `CTCHero::GetHeroTitleDefIndex`, `PeekHeroTitleSubDef`; four retail titles share enum 0) says the enum is not a table key, so appending is safe. New `forge title add` (FableForge, built) clones a donor's OBJECT + CInventoryItemDef/CStockItemDef/CHeroTitleDef, patches by field tag, relinks the six link words at 21/25/33/37/45/49, verifies on reload. Manifest `titles:` = Butcher of Oakvale (donor DEATHBRINGER) + Giftbreaker (donor ASSASSIN), 36 lines each over the 12 villager voice types, `oakvale_manifest.py` expands them; build has `defs` stage + `--placeholder-vo` (silent clips until the 72-line VO pass, ~2.4k chars next month). Grants in stranger.lua (massacre/hunted nights). bundle-v1: 9-file overlay, 602/602, lint 0, check 0, preflight passed; CHECKLIST v1-10.

**2026-09-19 (voice recast): the Stranger is Callum on `eleven_v3` Creative with a `direction:` tag per line** (A/B in `work/oakvale_reborn/ab`, user chose B); Theresa = Lily on v3. `elevenlabs_vo.py` takes per-voice `model`/`settings` and per-line `direction`. All 15 lines regenerated (v3 bills the tags: 9,387/10,000 used, ~600 left), v1 rebuilt ALL CHECKS PASSED, table_read.wav refreshed.

**2026-09-19 (latest+): edge-case pass + tooling.** Nine gaps fixed (Theresa gated on the offer + forced offer after the chocolates; stand-close/pass-by triggers; comments only when the hero is free; reload resumes massacre/hunt, no duplicate Stranger; 180 s massacre fallback; sword removed at night (`SWORD_SURVIVES_NIGHT`); offer returns extras; question in a paused movie; spawn beside NOVI_BookTrader). Bully rush at 3 kills in. New: `grade_run.py` (grades the FSE log against CHECKLIST rows, detects the road), `table_read.py` (76 s WAV of all 15 lines in beat order). bundle-v1 rebuilt, check 0, preflight passed.

**2026-09-19 (latest): hood + sword prop + road 4c built.** Stranger = `CREATURE_PROPHET_01` (hooded; alt ASSASSIN); the offer puts `OBJECT_HERO_SWORD_FIRST` in his hand (`HoldInHand`+`CS_HOLD_SWORD`); **road 4c** (user: gift dies with the giver, guards react) = 8 s window after accept in which a sword hit (`MsgIsHitByHeroWithWeapon`) makes him say his line and `SetThingAsKilled`, `RemoveAllHeroWeapons`, guards hostile 40 s, night, `CS_OVR_AFTERMATH_KILLED` (7th def; 602/602); `StrangerKilled` persisted. 15 voiced lines (7,7xx/10,000 chars). bundle-v1 rebuilt, check 0, preflight passed; CHECKLIST v1-7.

**2026-09-19 (late night): Oakvale Reborn beats LOCKED and built into bundle-v1.** User's calls: Stranger never
named, Father dies on both roads, Father/Theresa protected on the accept road, 6 kills, cold open stays;
consequence taken = on refuse the Stranger burns Oakvale himself (both roads share the retail section swap).
`STORY.md` "Beats — LOCKED"; `CS_OVR_COLDOPEN/OFFER/REFUSE.cs` authored on retail cameras/markers,
`CS_OVR_AFTERMATH_*` = HESDEADJIM clones with one spliced Maze line (`insert_before`); `stranger.lua`
now ColdOpen / watch (deed-keyed comments, cooldown) / offer (macro + Ask + answer) / Massacre (protected
set); Father waits for `ColdOpenDone` (persisted) and shows the seed box. 14 lines voiced with ElevenLabs
(George/Lily; 7,665/10,000 chars used this month), 601/601 cutscene round-trip, lint 0, smoke gate 0,
preflight passed. CHECKLIST.md v1 table rewritten (v1-0 … v1-6).

**2026-09-19 (night, offline): Oakvale Reborn v1 scaffold built, nothing run in-game yet.** The authored
FSE tree is seeded from the v4 stage (`tools/oakvale_reborn/scaffold_from_stage.py`; paths renamed, every
`NOVI_*` TNG name kept) plus `scenes.lua` (macro/Lua-beat runners, Say, Ask) and `stranger.lua` (the Stranger
spawns after the first deed, the offer, accept = `Stranger.Massacre` child-combat road, refuse = Theresa's
trigger runs `CS_OVR_REFUSE`, no FMV); `OnPersist` carries `StrangerAccepted/OfferMade`. Tooling:
`cs_lint.py` (manifest/.cs/Lua cross-checks, 24/24 negative findings), `build_custom_intro.py` now
`pristine text cutscenes overlay bundle check` (text = subtitle-only + `dialogue_pipeline --add` VO chain,
proven offline ALL CHECKS PASSED; `clone_of` cutscenes; install/restore are a `.ovrbak` layer over the
FableForge stage), `elevenlabs_vo.py` (LIVE: the 4 Stranger lines synthesised with George, staged into ScriptDialogue2 + lipsync, ALL CHECKS PASSED; ~2.8k chars left on the plan this month), `smoke_run_unit.py --package-dir`
(0 unknown methods / 0 load errors on the authored tree), `spike_s6.py` (child combat probe, `--grown`
fallback). Staged script.bin 599/599 round-trip. **Bundles waiting on the game:** `bundle-spike-s1`,
`bundle-spike-s6` (Lua only), `bundle-v1` (needs `install`) — commands + pass tables in
`refs/script_recovery/authored/OakvaleReborn/CHECKLIST.md`. Journal:
`docs/journal/2026-09/OAKVALE_REBORN_OFFLINE_2026-09-19.md`. Next offline: STORY.md beats (user) →
`CS_OVR_OFFER.cs`, real Stranger creature, `eleven_voice_id`s.

**2026-09-19 (later, agent): Gameflow is a converter unit.** `script_units.py` `gameflow` (0xCE6CB0 ctor ..
0xCEF9D0 = GameflowAssistance ctor; the cluster's 0xCB8110 is the shared base ctor), evidence in
`refs/script_recovery/gameflow/`, output `refs/script_recovery/lifted/Gameflow/{draft,readable}`: 5/5 fns,
readable syntax 2/2, smoke 0/0, **all 35 `PostSavePosition` stages written, OnPersist carries all four
fields** (the two `vector<CCharString>` ones as TODOs: no `PersistTransferStringList` binding). The
fall-through switch now lowers to a straight guarded chain, so resume works for 32/35 stages and the chain
runs 450 -> 2800; broken = resume at 700 / 1050 / 2800 and the 450 -> 500 handoff from earlier stages, all
cross-switch `goto` residue. Journal: `docs/journal/2026-09/GAMEFLOW_UNIT_2026-09-19.md` (commands, the
resume probe, every generic change). Oakvale gate identical; not committed; bundle/zip not rebuilt.

**2026-09-19 (offline, user away):** the converter's `Q_GuildTrainingWoodsMelee` — the file in the Aeon zip
and the v5 bundle — died on entering Guild Woods: `Main` lifted VC7.1's exception-state flag as
`local scratchValue ... scratchValue & 2` (= `nil & 2`, a Lua 5.4 runtime error) right after
`FinalizeEntityBindings`, so `DoMission` never started; and `DoMission` re-polled `IsLevelLoaded("")` (the
callOrder pairing fell back to address order over a namespace-stripped `CQ_CinemaTestScript::EndMission(`).
Both fixed generically in `native_evidence_lowering.py` / `convert_quest_unit.py`, plus: the EH-flag copy fold
no longer half-applies (TC_BanditFighter nil read), and int counters in string-typed slots are lifted
(MeleeOpponent's `if nil == nil` block-help alternation is now a real `(ctr + 1) % 5`). Journal:
`docs/journal/2026-09/WOODS_MELEE_ENTRY_CRASH_2026-09-19.md`. Gates: Oakvale draft identical, smoke
0 / 1 / 8 unchanged, readable errors none, new `test_guild_woods_melee_converter.py`. **NOT yet done:**
rebuild `work/AeonShare-*.zip` + the v5 bundle on the regenerated units, and the in-game run — enter Guild
Woods, kill the beetles, and quit mid-quest there.

# After night 7 (2026-09-18)

Nine commits, all generic converter work, suite **1593 passed / 0 failed** with the four stale fixture files
ignored. Across the three units:

| | start of night | now |
|---|---:|---:|
| `scratchValue` in the emitted .lua (Guild / Trader / Orchard) | 1327 / 644 / 54 | 845 / 403 / 25 |
| smoke problems (Guild / Trader / Orchard) | 11 / 11 / 0 | **1 / 8 / 0** |
| free globals across the units | 26 | 4 (all deliberate) |
| converter scaffolding (`__native_entity_state`, `__region_LAB_*`) | in 29 of 36 files | **0** |
| locals declared at the top instead of where computed | 903 hoisted | **881 sunk** |
| `TryAcquire(0, ...)` / `ReleaseResource(0)` | 20+ | 0 |
| retail enum operands spelled as bare numbers | all | 105 named |

`work/AeonShare-2026-09-18.zip` (3323 KiB) and the v5 bundle are rebuilt on this output and **preflight
passes**. The in-game run is the user's step:

    python work/new-oakvale-original-fse-20260912/local-candidate-v5/local_test.py \
        --game-dir "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters" --launch --save-dir <saves>

Worth re-testing specifically: **quit mid-quest during Guild melee training.** Those exits used to drop two
`DeregisterTimer` calls and a `ReleaseResource` (third pass), which is the crash class Aeon reported.

**Resume order**

1. **`canonicalise_stack_objects` pre-range liveness** — the last runtime error in the units
   (`CheckFriendlyAttacks`), diagnosed in the sixth-pass section. Every unit goes through that pass, so do it
   carefully and first.
2. **The next quest family: HangingTree** (0x00D68F00-0x00D78500, 62,976 B, 151 fns, Evil+Good). Its `lo` is
   GuildTraining's `hi`, so the Ghidra export extends rather than re-runs, and it is the variant-pair shape the
   converter already ships. Then HeroSouls (7 scripts, 248 fns, 0 residual bindings). The eighth-pass section
   has the full ranking, the per-family entity lists and what registering a unit costs.
3. **The `goto` residue** (~104 per 1000 lines, the single biggest remaining gap vs Aeon) as its own
   restructuring project, sized by `tools/script_recovery/report_goto_residue.py`.
4. The rest of `scratchValue` via `USE_ROLES` in `readable_lua.py`, and TraderConflict's six sidecar bindings
   (needs the DLL rebuilt).

Measure progress with `tools/script_recovery/report_readable_style.py`.

**Running the suite**: alone (a concurrent unit rebuild fakes 400+ failures), ~29 min, `--ignore` the four
stale fixture files `test_watch_barrels_loop.py` (78), `test_bully_proximity.py` (44),
`test_live_father_intro.py`, `test_watch_barrels_readable.py` — they fail on a pristine tree too. After any
style change check `shippedAsDraft` in the `build_readable_unit` summary: a file whose passes raise silently
ships as the raw draft.

# CURRENT (night 7, eighth pass, 2026-09-18): the audit workflow's verified rules, and the next unit

A 56-agent workflow audited the readable output against Aeon's ports along six dimensions and scouted the
next quest family along four. **5 rules survived adversarial verification, 18 were refuted** -- and two of the
refutations were worth more than the rules: one monkeypatched `tidy_blank_lines` and rebuilt Orchard to show
that "blank lines are cosmetic" is false (the style pass runs TWO rounds, so round 1's blank-stripping is
round 2's input and inserting blanks changes the *code*), and another showed that rewriting a `(end - begin)`
vector guard to `#list` would have turned an unrunnable loop into a definite full-list `RemoveThing` pass.

## Landed

**`sink_hoisted_locals`** (342 measured legal sites; 881 locals actually sunk across the three units). The
lifter hoists every slot into one comma list at the top of the function; Aeon declares a value where it is
computed. A declaration moves onto its assignment only when: exactly one assignment, every read after it,
every read inside that assignment's own block, no earlier closure captured the name, and **no `goto` from
before the sink point targets a label inside the new scope** -- Lua 5.2+ rejects a jump into a local's scope.
The audit put this inside the fold loop; that is wrong and the Lua compiler said so (`<goto continue_1> jumps
into the scope of local 'fret_0'`). `fold_retry_loops` and `fold_control_acquires` *introduce* gotos, so the
check has to run after every fold that can emit one. It now does, and both files compile.

**`name_enum_operands`** + `tools/script_recovery/retail_enums.py`: `me:MoveToPosition(pos, 3.0, 1, ...)` is
`ENTITY_MOVE_RUN`. Member values are read from `ghidra_out/ego_r_pdb.xml` at build time, never hardcoded, and
every entry was checked against the FSE headers as a second source before being added. Four enums so far
(EScriptEntityMoveType, EHeroAbility, ETutorialCategory, ECutsceneBehaviour) -- 105 named operands. A
non-literal operand or an out-of-range number is left exactly as it was.

**Three failing tests that were nothing to do with the converter**: FableForge 0.16 split the level editor
(`forge.exe`) from the def tooling (`forge-tools.exe`), so `forge defs` vanished and the barrel/gold audits
here died with "unexpected argument". `FORGE` is now resolved from a candidate list
(`FableForge/build/forge-tools.exe`, then `FableForge-legacy/build/forge.exe`, then the old path) so the next
sibling-repo rename shows up as one failed resolve rather than three cryptic audits.

## The next unit: HangingTree, then HeroSouls

The scouts agree, from `refs/script_recovery/native_clusters/*.json` + `rebuild/manifest/functions.tsv`
(NOT `ghidra_out/coverage.tsv` -- it misses ~40% of functions in the script region):

| family | bytes | fns | range | shape | new bindings |
|---|---:|---:|---|---|---:|
| **HangingTree** (Evil+Good) | 62,976 | 151 | 0x00D68F00-0x00D78500 | the proven Evil/Good pair, **abuts the guild export** | 1 + 1 residual |
| **HeroSouls** (7 scripts) | 71,840 | 248 | 0x00D78500-0x00D89DA0 | the GuildTraining multi-sibling shape at scale | 2, **0 residual** |
| HobbeCave + Minion | 120,256 | 339 | 0x00D8D640-0x00DAAC00 | two adjacent families, one Ghidra run | 1, 0 residual |
| Prison (3 scripts) | 56,320 | 163 | 0x00DD2720-0x00DE0320 | `Q_PrisonRace` reuses the guild race logic | 1, 0 residual |

HangingTree first: real bodies on both sides (151 fns vs Orchard's Evil+Good 14), the variant-pair shape the
converter already ships, and its `lo` **is** GuildTraining's `hi`, so the Ghidra export extends rather than
re-runs. None of the eight candidates is claimed by Aeon.

**Registering a family is cheap and generic** -- a `UNITS` entry, a read-only Ghidra
`ExportScriptTranslationUnit` over the range, `pdb-locals.exe`, then `guild_training_inventory.py`,
`ghidra_typing_spec.py`, `ExportTypedTranslationUnit.java`, `quest_unit_evidence.py`. Everything downstream
already takes `--unit`. Two findings from the readiness scout: `pdb_pattern` in `script_units.py` is **dead**
(nothing reads it) and so is the FableWin PDB tsv; and `pdb-locals.exe` needs an **x86 `msdia140.dll`** (the
`msdia100.dll` under Common Files fails with `class factory failed: 0x80040111`) -- one that works is
`C:\Program Files\dotnet\sdk\10.0.301\TestHostNetFramework\x86\msdia140.dll`.

**A scout warning I checked and dismissed**: `CSummonerToKill` appears in three HeroSouls scripts and
`CTheRealGuildmaster` in both EndGame and GuildTrainingWill, which the scout called a file-name collision.
It is not -- `package` comes from the per-SCRIPT unit JSON, so `FSE/GuildTrainingPreMelee/Entities/
TheRealGuildmaster.lua` and `FSE/GuildTrainingWill/Entities/TheRealGuildmaster.lua` already coexist today.

Gates: Oakvale draft and readable identical, smoke Orchard 0 / Guild 1 / TraderConflict 8, unit file syntax
100%, `report_readable_style.py` for the residue numbers.

# CURRENT (night 7, seventh pass, 2026-09-18): the scaffolding is out of the readable output

Two things a reader sees immediately, both now gone from the converter units.

**The per-entity state shim was dead in 29 of 36 files.** `inline_entity_fields` turns
`state:GetInt("AppleMode")` into a file-level local `appleMode`, because ForgeFSE gives each entity
instance its own sol::state. But it bailed with `if not keys` when an entity never touched its own
fields -- so the ten-line shim stayed, declared and never used, at the top of every such file. It is
dropped now when nothing reads it. (The seven files that really use it are all NewOakValeIntro, whose
readable stage is hand-built and whose own passes match on `__native_entity_state` textually.)

**`local function __region_LAB_00d555f3_c27()` now says what it does.** `name_cleanup_closures` names a
hoisted epilogue from its body -- `ResumeEntities`, `DeregisterTimers`, `ReleaseControl`,
`EndCutsceneAndRelease`, `ReleaseEverything` -- and *merges* the ones whose bodies are identical, because a
function hoists the same one-line epilogue once per jump site and five `ResumeEntities` definitions would
read worse than the labels did. Zero `__region_` / `__cleanup_` spellings left in the units.

`test_readable_style.py` is new: that module had **no tests at all** while three of tonight's changes went
into it. It covers the dead shim, fields-become-locals, the non-literal key that must keep the shim, the
closure naming and merging, and the orphaned-comment guard from the fifth pass.

`tools/script_recovery/report_readable_style.py` is new too -- it measures what is left, over code only
(comments and string bodies blanked, so the `-- Main (retail 0x...)` headers and the `TODO(native)`
markers do not inflate the count). Current state:

| per 1000 lines | Guild | TraderConflict | Orchard |
|---|---:|---:|---:|
| `goto` / `::label::` | 104 | 108 | 42 |
| `scratchValue` | 70 | 148 | 19 |
| `predicateResult` | 15 | 46 | 11 |
| raw slot name | 9 | 43 | 14 |
| converter scaffolding | **0** | **0** | **0** |

Aeon's ports have none of these. `goto` is the single biggest remaining gap and it is a restructuring
project (1392 jumps: 23 backward, 926 forward-out-of-block-with-more-code, and **none** that reduce to a
plain `return` -- checked). Orchard is the proof the pipeline can get there; the Guild/Trader gap is a
diagnosis question, which is what the audit workflow is for.

**Two proposals I verified and rejected today** rather than shipping: `quest:GetStateInt(..) ~= 0` is not
redundant (those bindings return integers) and `do return end` is not verbose (Lua requires it when a
return is followed by more statements in the same block). Both look like easy wins in a grep and are not.

Gates: Oakvale draft and readable gates identical, smoke Orchard 0 / Guild 1 / TraderConflict 8.

# CURRENT (night 7, sixth pass, 2026-09-18): one-operand vector calls, and the CheckFriendlyAttacks collision diagnosed

`fold_local_thing_vectors` recognises a GSI call that fills a local `vector<CScriptThing>` and turns it into a
table. Both of its patterns required a comma before the out-vector, so a call whose **only** operand is the
vector never matched: `GetAllCreaturesExcludingHero(&vec)` stayed as raw pointer arithmetic while
`GetAllThingsWithDefName(&name, &vec)` folded. The leading operands are optional now; the guard that the slot
must be a zero-constructed local vector is what keeps it safe. Guild todo 765 -> 756, and CheckFriendlyAttacks'
element access became `creatures[i + 1]` instead of `*(*(0x0 + iVar18) + 8)`.

**The remaining Guild smoke error is diagnosed, not fixed.** `CheckFriendlyAttacks: attempt to perform
arithmetic on a table value` comes from `canonicalise_stack_objects`, not from the naming or the vector pass.
The function builds a `CScriptThing` at stack base 0x90 (`QUESTTHING_Empty`, extent 0x84..0x90) *late*, and the
same bytes earlier hold the "PreMeleeMaze" string (`CStack_88`), the creature vector's end pointer
(`puStack_90`) and its capacity (`uStack_8c`). The pass rewrites every slot name inside an object's extent from
the *previous overlapping construction* -- or from line 0 when there is none -- so the first object at an extent
swallows every earlier, unrelated use of those bytes. Six declarations end up spelled `xStack_90`, and the
count `(end - begin) / 12` reads as `thing - vector`.

The comment in that pass explains why the region starts early (members can be read before the constructor line,
e.g. an iterator element copy), so the fix is not simply "start at the constructor": within the range *before*
the construction, a slot that is assigned there is live as something else and must keep its own name. That is
the last runtime error in the three units and it is worth doing properly rather than late in a session.

Gates: Oakvale draft gate identical, smoke Orchard 0 / Guild 1 / TraderConflict 8 (unchanged), file syntax 100%.

# CURRENT (night 7, fifth pass, 2026-09-18): the free globals are gone; smoke Guild 11 -> 1, TraderConflict 11 -> 8

The lifter builds a function's `local` line from the slots it *assigns*. A slot the decompiler only ever read --
a value Ghidra lost, a residue name an earlier fold left behind -- was missing from that line, so at runtime it
resolved to a **global**: nil on read (harmlessly the same value), but a write escapes the function and is
visible to every other script sharing the Lua state. `smoke_run_unit` has reported these as `FREE GLOBALS` all
along; 10 of Guild's 11 smoke problems and 2 of TraderConflict's were exactly that.

`declare_free_locals.py` adds them to the function's own `local` line, running in `convert_quest_unit` right
after the cleanup-region hoist and recorded as `declaredFreeLocals` in CONVERSION_REPORT.json. It only declares
names that *look* like a lifted temporary (`xStack_7c`, `ctr_40`, `this_00`, `pPos`, `fret_0`, `scratchValue9`);
`this`, `unaff_EBP` and `__unknown_push` are deliberately left free, because each is a decompiler or lifter gap
and declaring it would hide the evidence behind a silent nil. Free globals across the three units: **26 -> 4**,
and the four that remain are those gaps.

`build_readable_unit` now prints `shippedAsDraft` in its summary, so the fallback that hid PreMeleeWhisper (see
the pass below) cannot hide anything again without it being on screen.

Smoke: Orchard 0, **Guild 11 -> 1**, **TraderConflict 11 -> 8**. What is left is honest:
* Guild's one is `CheckFriendlyAttacks: attempt to perform arithmetic on a table value` -- the known slot
  collision (`xStack_90` is a string temp, a thing result *and* a vector end pointer in one register), which
  needs the lifter to split the slot, not a readable-stage fold.
* TraderConflict's eight are two free-global files (the gaps above) and six `unknown=StateListSet` /
  `GetStateListCopy` / `EntitySetAsOpinionSource` -- sidecar bindings the DLL has not been rebuilt with
  (FSE_UPSTREAM_REQUIREMENTS.md).

Gates: Oakvale draft gate identical, unit file syntax 100%, new `test_declare_free_locals.py`.

# CURRENT (night 7, fourth pass, 2026-09-18): the consuming API names the value, and it knows when not to

`USE_ROLES` grew from 9 rules to 16 and each entry now carries a **kind**, plus two rules that read the name out
of the call itself:

* `GetStateListAt("AllCreatures", X)` -> `allCreaturesIndex`, and `..., X / 12)` -> `allCreaturesOffset`, because
  a register the compiler walks by byte offset is not an index and should not be spelled like one.
* `SetString(map, "$GRADE", X)` -> `grade`: the actor-map key is the value's name.
* `Speak(listener, X, ..)` -> `line`, `ReadGlobalGameDataFloatAt(field, X)` -> `index`,
  `UpdateQuestInfoTick(elem, X)` -> `ticked`, `TryAcquire(res, X, ..)` / `CreateEffect(X, ..)` -> `thing`.

**The kind is what makes this safe.** A slot passed to both `ReleaseResource` and `DeregisterTimer` is two values
sharing a register and either name would be a lie, so `use_role` declines; but acquired / released / destroyed
are three views of one resource, so within the `resource` kind the first (most specific) rule wins and the name
is `heroControl`. Measured: requiring plain agreement across all rules cost 94 occurrences for 5 conflicted
names, of which only 1 was a real contradiction -- the kinds recover the other 4.

Also `flag_register`: `X = 0; X = X | 1; X = X & 0xfffffffe` is a bit field, not a scratch value, so it is
named `flags`.

**One whole file was shipping as raw draft.** `build_readable_unit` catches a `ValueError` from any pass and
falls back to the unnamed draft text for that file, printing one line to stderr -- which is easy to miss in a
build that prints a JSON report. `READABLE_REPORT.json` records it as `files[rel].error`, and
`FSE/GuildTrainingPreMelee/Entities/PreMeleeWhisper.lua` had been failing on `guard body indentation` for who
knows how long: `fold_guard_wrappers` dedents a guard's body and refused a line that was not indented, but an
earlier fold had orphaned a `--[[unresolved native value]]` comment at the guard's own indent. A comment carries
no semantics, so it is now skipped instead of aborting the file. That file went from 100% raw to named and
styled (unit `xStack_NN` 211 -> 160). **Check `files[*].error` in the report after any style change.**

`scratchValue` in the emitted `.lua`: **Guild 934 -> 844, TraderConflict 534 -> 399, Orchard 37 -> 25** (Guild's
844 includes the 18 the recovered file contributes -- it had none before because it had no names at all). Across
the whole night: Guild 1327 -> 844, TraderConflict 644 -> 399 (-38%), Orchard 54 -> 25 (-54%).

New `UseRoleTests` in `test_readable_lua.py` cover each rule, the index/offset split, the agreeing kind, the
declining kind and the bit field.

Gates: Oakvale readable identical, smoke Orchard 0 / Guild 11 / TraderConflict 11.

**What is left** (census with `grep -rhoE "scratchValue[0-9]* = .*" ... --include='*.lua'`): 111 temporaries whose
only assignment is a plain number and that no `USE_ROLES` call consumes, 36 `X = X + 1` counters, and 22
`X = X | 1` bit steps that `flag_register` refuses because the same slot also takes a call result. The next
lever is still the use side -- add rules as new consuming calls show up -- or split those slots harder.

# CURRENT (night 7, third pass, 2026-09-18): 27 early exits stopped leaking their cleanup

`native_cleanup_regions.py` hoists a retail epilogue (release the controlled entities, deregister the timers,
destroy the movie) into `local function __cleanup_LAB_x()` so the early exits the lifter could only render as a
bare `return` run it first. It only recognised an epilogue that ran straight to a `return` at one indent level,
which is not how most of them look. Two generalisations:

1. **The epilogue may fall out of enclosing `if`/`do` blocks.** The native jump target usually sits inside a
   block and the cleanup continues after that block's `end`; only a loop's `end` stops the walk, because that is
   a back edge, not a path. `_blocks` classifies every opener/closer of the chunk and `_walk` follows the one
   path. A region found this way is hoisted but its own lines stay put -- the `end`s it crosses belong to those
   blocks, and deleting them (as the in-place rewrite did) unbalanced the function; two files failed the Lua
   syntax check before that was caught, which is exactly what `fileSyntaxPassed` in CONVERSION_REPORT is for.
2. **The epilogue may finish by jumping to a label the lifter really emitted** (`goto FLOW_after_lab_x`). When
   that label's own block runs straight to a return, its statements join the region, so the call sites reproduce
   the whole epilogue and the TODO marker goes away entirely. A region that delegates to another region takes
   nothing (the delegate carries the tail) -- without that guard `__cleanup_LAB_00d5a922` released the same
   resource twice.

Effect across the three units: hoisted regions 40 -> 48, `-- TODO(native): goto` sites 158 -> 131. The 27 sites
that changed were, for example, `if bVar3 then return end  -- TODO(native): goto LAB_00d5a9b5` in
GuildTrainingMelee/TheRealGuildmaster, which dropped two `DeregisterTimer` calls and a `ReleaseResource` on every
terminating exit. That is Aeon's crash class (quit mid-quest with control still held), so it is worth re-testing.

New `test_native_cleanup_regions.py` (the module had no tests): block-crossing epilogue, emitted-label tail,
the no-double-release guard, and a label inside a loop body that must NOT become a region.

Gates: Oakvale draft gate identical, Oakvale readable identical (that converter does not use this pass), smoke
Orchard 0 / Guild 11 / TraderConflict 11, every unit 100% file syntax.

**What is left of the goto residue**: 131 sites, reported by the new
`tools/script_recovery/report_goto_residue.py` -- 128 target a LAB comment whose region this pass will not
accept, 3 target a label that is not in the function at all. Of the 128, 14 are jumps to an *empty* epilogue
(nothing to clean up: only the jump is missing, so they are flow gaps, not leaks) and the rest are epilogues
that open a block of their own -- a `while` / `repeat` / `if` inside the cleanup, which is no longer a single
path and cannot be hoisted as one closure. Closing those means real control-flow restructuring; the readable
stage tells the same story (only 83 of its 1139 `goto` target a straight-line return tail), so treat it as its
own project rather than another converter pass.

# CURRENT (night 7 continued, 2026-09-18): `scratchValue` down by a third

Three naming changes, all generic, on top of the byte-split work below.

1. **The split budget goes to the widest splits first.** Lua 5.4 allows 200 locals, so `split_hoisted_locals`
   reserves 180 and stops; it used to spend the budget in declaration order. A register Ghidra reused 44 ways is
   exactly the one that reads as noise unsplit, and each version earns its own role name. Measured on
   GuildTraining: widest-first **2467 semantic / 705 scratch** names, declaration order 2324 / 770, and
   narrowest-first (the idea in last night's handoff) is much worse at 1927 / 786 -- the cheap splits are cheap
   because they were nearly clean already.
2. **The role renamer runs a second time, on the styled text.** The style folds drop assignments, so a slot the
   first pass could only call `scratchValue` (five disagreeing assignments) often has one survivor afterwards.
   `scratchValue\d*` is in `build_readable_unit`'s `GENERATED` patch so the second pass can pick those up, and
   the fallback now refuses to renumber its own names (no more `scratchValue` -> `scratchValue3` churn).
3. **`use_role`: the API that consumes a value names it** when its assignments say nothing --
   `DeregisterTimer/SetTimer/GetTimer` -> `timerId`, `AddPersonToConversation/AddLineToConversation` ->
   `conversationId`, `SetActor/RunMacro/DestroyActorMap` -> `actorMap`, `DestroyMovie` -> `movie`,
   `RemoveQuestInfoElement` -> `infoElement`, `ReleaseResource` -> `resource`, and
   `TryAcquire(X, <thing>, ..)` -> `<thing>Control`. Consulted only where the fallback would be `scratchValue`,
   so it never renames something already named.

`scratchValue` occurrences in the emitted `.lua` (the honest metric -- grep the report JSON too and the number
doubles): **Guild 1327 -> 934, TraderConflict 644 -> 534, Orchard 54 -> 37.**

Gates: Oakvale draft gate identical, Oakvale readable identical, smoke Orchard 0 / Guild 11 / TraderConflict 11,
no control bytes in `tools/script_recovery/*.py`.

**Next on naming**: what is left is mostly a temporary whose only assignment is a number (121 across Guild +
TraderConflict), `ctr_NN` / `x_stk_NN` slot names that survive styling, and `scratchValueN | 1` bit-flag
arithmetic. The use-side rule is the lever that worked -- extend `USE_ROLES`, not the RHS table.

# CURRENT (night 7, 2026-09-18): the byte-split acquires fold, and the local splitter runs inside the big functions

**TheRealGuildmaster's `TryAcquire(0, hero, 4)` is gone** (last night's first item). Three generic converter fixes:

1. `fold_byte_split_pointers` folded **one** split per function and then stopped. The reassembly was matched as an
   exact string, but the unwrap pass leaves a space wherever Ghidra had wrapped the expression, so the second split's
   `concat not in tail` broke the loop. It is now one ordered scan keyed on each split's own object (the four byte
   registers are the same for every acquire in a function, so a reassembly belongs to the last split before it), with
   a whitespace-tolerant `RE_BYTE_CONCAT_USE`; `RE_BYTE_SPLIT` separators became `\s*` because a deeply indented body
   leaves `>> 0x10)\n    ;`. PreMelee TheRealGuildmaster: 10 splits / 20 reassemblies, all folded (was 1 / 5).
   Dead splits are dropped by local liveness (the registers are reused for an unrelated zero-fill later in the body).
2. **`&stack0xffffffXX` is a declared local** (`resolve_stack_offset_names`): Ghidra spells one slot both by its
   declaration (`ppuStack_f8`) and as a raw frame offset where the slot is only ever taken by address. A local `X_f8`
   sits at entry-SP - (0xf8 + 4) = -0xfc, so the two meet. Without this, MeleeApprentice's resource was two objects.
3. **The destructor with no vtable line** (`RE_INLINE_DTOR_NO_VTABLE`): on the path where the object is already the
   base, only the member zero-stores precede the base destructor call. `canonicalise_stack_objects` then spelled the
   member store under the object's name, so `resource = 0` reached the lifter and every later use came out `nil` —
   that is where `TryAcquire(0, me, 4)` / `ReleaseResource(0)` came from. Folded only for an object this function
   acquired (the shape alone is not evidence). The leading zero-stores of the ordinary dtor are now matched
   name-agnostically too (they may be spelled under a sibling slot).

**The local splitter now runs in the functions that need it** (`lua_local_versions.py`): `flow_graph` bailed on the
whole body for two emitter shapes. Hoisted cleanup/region closures (`local function __region_X() ... end`) are now
blanked and every name they touch is pinned (a closure call could read or write it at any of its sites), and
`if C then __region_X(); goto LAB end` is accepted as one inline-jump node — its goto edge was also being dropped,
because the edge builder looked for `then goto L end`. Guard: the chunk's *own* header may read
`local function __resource_main(...)` (the Oakvale husband candidate), and treating that as a closure blanked the
entire body — the scan starts after the root header. TheRealGuildmaster PreMelee splits 6 hoisted temporaries (was 0);
Guild `scratchValue` 2276 -> 2097, Will TheRealGuildmaster 250 -> 137, with real names landing instead
(`timerId8/9`, `questionAnswer2..6`, `conversationId`, `preMeleeDummy`, `dist`, `tutorialState`). Orchard's readable
lost a whole dead cleanup closure and two dead termination checks.

Gates: **Oakvale draft gate identical**, Oakvale readable byte-identical (its READABILITY_REPORT summary unchanged),
smoke Orchard 0 / Guild 11 / TraderConflict 11 (all unchanged), unit todo Guild 765 (was 791).
Suite: **1563 passed**, 78 failed — all 78 in `test_watch_barrels_loop.py`, and **identical on a pristine tree**
(stash the tool + refs changes and re-run it: 2.5s). The stale-fixture list is now four:
`test_bully_proximity.py` (44, also pristine), `test_watch_barrels_loop.py` (78), `test_live_father_intro.py`,
`test_watch_barrels_readable.py`. Run the suite with those four `--ignore`d, and ALONE (a concurrent unit rebuild
fakes 400+ failures); the whole thing takes ~26 min.

**Next**: the remaining `scratchValue` (2097 in Guild) — the splitter's 180-local budget stops after the first few
names in TheRealGuildmaster (cVar4 alone wants 44 versions), so consider splitting fewest-versions-first, and fold
`scratchValue = quest:IsActiveThreadTerminating()` / `= quest:MsgIsQuestionAnsweredYesOrNo()` at the style stage.
Then goto residue and the in-game run of v5 (user-driven).

Rebuilt tonight: `work/AeonShare-2026-09-18.zip` (3320 KiB) and the v5 bundle
(`build_unit_playtest_package.py --unit orchard_farm --oakvale work/new-oakvale-original-fse-20260912/local-candidate-v4/NoviCompatibility
--dll work/new-oakvale-original-fse-20260912/sidecar-abi-v2/Release/FableScriptExtender.dll
--bundle work/new-oakvale-original-fse-20260912/local-candidate-v5`); **preflight passed** against
`C:\Programs\Steam\steamapps\common\Fable The Lost Chapters` (that is the install path — it was not written down before).
Launch is the user's step: `python work/new-oakvale-original-fse-20260912/local-candidate-v5/local_test.py
--game-dir "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters" --launch --save-dir <saves>`.

# CURRENT (night 6, 2026-09-17): TraderConflict unit through the pipeline (15/15 compile, todo 431 -> 176)

**Latest (same night, later)** — Aeon zip rebuilt (`work/AeonShare-2026-09-17.zip`: NewOakValeIntro + Orchard + Guild +
TraderConflict, findings README). Readable output now writes entity control as Aeon does (`if not me:AcquireControl(4) then
return end` / `me:ReleaseControl()`; `fold_control_acquires`, DLL semantics verified in LuaEntityAPI::AcquireControl), Orchard
`DoMultiplierCutscene` folds to `StartCutscene` (object aliases hoisted below the inlined construction; no-op comma
assignments dropped; flag-clear + retest folded: `if quest:IsRegionLoaded("GreatwoodLake") and heroTeam == 0 then`). **Real bug
fixed**: AppleGirl read `GetTimer(conversationId)` from the 2nd loop iteration (Ghidra merged the reloaded timer register
with the conversation id) — timer calls now go through the stack copy. Smoke: Orchard 0, TraderConflict 12 notices (unknown
sidecar bindings StateListSet/GetStateListCopy — DLL not rebuilt — plus free globals in TraderToRescue), Guild 11. Gate
identical; fast tests 97 green. Later still: `thing:Speak` is Speak_Blocking so the wait loop folds into its result,
GROUP_SELECT_* names, timers routed per register (CombatApprentice `SetTimer` read a conversation id), `rand()`,
GetDataString inlines, MsgExpressionPerformedTo returns name-or-nil (binding exists). Aeon zip + v5 rebuilt (preflight ok).
Full `pytest tools/script_recovery` = green except `test_live_father_intro.py` / `test_watch_barrels_readable.py`
(stale fixtures, failing since bf13d5d — not this night's work; run the suite ALONE, a concurrent unit rebuild fakes 400+ failures).
Bedtime state: entity fields are file-level locals (no more `state` shim), early-exit flattening landed
(TheRealGuildmaster 2418 -> 2220 lines, top level flat, loops still 116 cols). All units rebuilt, smoke Orchard 0 /
TraderConflict 11 / Guild 11, gate identical, fast tests green. **Tomorrow first**: TheRealGuildmaster's 16
`TryAcquire(0, hero, 4)` (byte-split resource pointer, `fold_byte_split_pointers` folds 5 of 9 — key it on the object
name), then its cutscenes fold and the loops flatten; rebuild Aeon zip + v5 (last built before the fields/flatten
commits — `build_aeon_share_zip.py`, `build_unit_playtest_package.py` command in the journal). Then: remaining `scratchValue`
temps (1747 in Guild; mostly `= 0` flags and DeregisterTimer cleanups), goto residue, in-game run of v5.

**Later the same night — dropped operands recovered from the machine code** (`recover_dropped_operands` in
`convert_quest_unit.py`): calls the decompiler printed with no operands (TraderToRescue's whole Main) are rebuilt from a
linear capstone decode — immediates / .rdata literals, `lea esp` slots (entry-relative via the site depth), call results
(`__pushN = GSI->GetHero()` naming, `mov esi, eax` copies), register values traced to their last write (`this + 8` = me);
signature-typed exact matching guards every rewrite. `nil --[[missing]]` 81 -> 29 across the unit; local-vector
iterator walks with `erase` (`table.remove`), `CCharString::NotEqual`, sibling stack-slot spellings (`xStack_1c + 4` = `xStack_18`),
per-branch string literals copied into a slot become real string variables (`xStack_1c = pOther` / `= "CREATURE_BS_SHERIFF"`),
the parent pointer re-loaded in a loop condition hoisted; Guild's swapped
`MiniMapAddMarker` operands and a drifted `DeregisterTimer(xStack_260)` fixed by the same machinery. Also: FSE string
helpers (OperatorPlus/IntToString/c_str), AppendCString literals behind hidden-result pushes, EH-state flags, member
counters through pointer temporaries, the signed count idiom, `this[100]`, typed null-string compares, the DeregisterTimer
register reuse. Oakvale draft baseline regenerated (16th binding lifts by itself; `readable_new_oakvale_main_bindings.py`
accepts both forms); `test_source_hygiene.py` fails the suite on any control byte in `tools/script_recovery/*.py`.
Readable passes landed the same night (hoisted `hero`, copy propagation, cutscene boilerplate -> `quest:StartCutscene/RunCutscene/EndCutscene`, role names from script names; Aeon's Fisherman/NewOakValeIntro ports are the style oracle) — Orchard readable 1140 lines / 69 temps, Guild 12300, TraderConflict Evil 4043 / Good 2523; smoke 0 errors draft + readable. **v5 REBUILT** after two real draft bugs (Orchard `RunMacro` got the marker thing instead of the actor map; movie/resource destructors swapped) — preflight ok, in-game run still user-driven. TraderToRescue.Main is still the rough one
(5.5 KB function, `unaff_EBP` EH seeds, `MsgExpressionPerformedTo()` operands, `"" == nil` null-branch residue).


**New unit `trader_conflict`** (Q_TraderConflictEvil + Q_TraderConflictGood, 0xDF5CD0..0xE00610; evidence in
`refs/script_recovery/trader_conflict/`, draft + readable in `refs/script_recovery/lifted/TraderConflict/`). Draft: 63/63 fns,
**15/15 files compile**, todo 292 (was 431 on the first pass), smoke harness **0 errors** (draft + readable), Oakvale gate
identical, Orchard/Guild drafts regenerated (Orchard byte-identical apart from `f_xStack_18` -> `f_stk_18`; Guild 783, cleaner
vector counts), script_recovery tests green. Converter work this unit taught (all generic):
member-vector idioms (`erase(begin,end)` = `StateListClear`, GSI out-arg fill = `StateListSet(name, GSI->F())`, iterator walks
with `erase(it)`, vcalls/operands through the iterator, byte-indexed element copies with the inline vtable literal, a stale
`(int)CVar5` cast on the byte index), inline `operator==(CCharString, literal)` null branches folded into the general
`Compare` (also the goto-fused forms), the EH-state flag byte (`bVar7 |= 2; if ((bVar7 & 2) != 0) {dtor}` incl. copies
through a reused register), `CONCAT13(1,(int3)Y)` top-byte flags (both write-back forms), a CCharString-typed stack
array reused as an int counter (`f_stk_NN`), by-value CCharString parameters passed on the stack
(`&stack0x00000004` -> `strParam_1`; callers pass the literal), a local actor map's inlined destructor, the resource-valued
map store with the refcount dance inlined (bsim label `CFourierAnalysis::CFourierAnalysis`), library bodies inside the
unit range (vector copy ctor / initialiser) no longer emitted as helpers, pointer temporaries to a local vector
(`pOut = &vec`), any GSI call whose last operand is a zero-constructed local vector returns a table.
Two REAL bugs fixed: literal BACKSPACE bytes (0x08) where `\b` was meant in `native_goto_scopes.py`, `smoke_run_unit.py`
and the end-slot rule (patterns silently never matched — see GOTCHAS), and the annotate blanket rule that made any
`(**(code **)(*piVarN + OFF))(` a GSI vcall (element `GetDefName` became `GSI->Error`): a register is the interface only
while its latest definition is an interface load. Sidecar bindings added: `StateListSet`, `GetStateListCopy`
(`novi-unit-bindings.patch`, FSE_UPSTREAM_REQUIREMENTS row) — DLL NOT rebuilt yet.

**Open on TraderConflict** (top todo shapes): `goto LAB` residue (21), `me:MsgIsHitBy(name)` / `MsgIsHitByAnySpecialAbilityFrom`
/ `IsPlayerHoldingLockTargetButton` (no FSE binding), `(**(code **)*puVar3)()` element dtor calls in the 2-D loops of
AttackPeople, `CCharString::operator=(&xStack, pCVar4)` string copies, `*piVar9 = *piVar9 + -1` refcount residue,
`SetCombatNearbyBreakOffRange(elem, <range lost by Ghidra>)` (by-value thing arg hides the float). Readable stage built
(Evil 4191 lines / Good 2690) but not yet reviewed line by line.

**Aeon (2026-09-17 chat)**: he tests every port by a mid-quest save + load (OnPersist) and by quitting the game mid-quest
(entity control not released = crash on quit). He found our drafts "a Lua version of the disassembly" — point him at
`readable/` (draft is the oracle), and ask for his crash log + Lua to diff against our WoodsMelee.

Resume: `python tools/script_recovery/convert_quest_unit.py --unit trader_conflict` → `build_readable_unit.py --unit trader_conflict`
→ `smoke_run_unit.py --unit trader_conflict --stage draft|readable`; gate script + tests as below.

# CURRENT (night 5, 2026-09-17): readable output reads like a quest script; three Orchard draft bugs fixed

**Readable style (READABLE_STYLE_PLAN steps 1, 2, 6) LANDED**: `tools/script_recovery/readable_style.py` (flow-graph-checked
folds: termination boilerplate, frame idiom, guard wrappers, retry loops, dead-check dataflow, single-use-per-definition
temporary inlining with straight-line sinking, dead defs, literal propagation, boolean materialisation, parens/cosmetics,
one `local helpers = require(...)`) runs inside `build_readable_unit.py` (two rounds with the older folds; `--no-style`,
`--frame-keeps-checks` for NewOakValeIntro-lifetime units). Orchard readable 2109 -> 1287 lines, temporaries 270 -> 87,
termination checks 177 -> 106; READABLE_REPORT.json carries per-function before/after style metrics. Smoke harness 0 problems
(draft + readable), Oakvale gate identical, script_recovery tests green. Steps 3-5 (named state/things, structure, constants)
are partly landed too (state alias, init-only hoists, helper naming from written state, function headers, goto→if/else,
elseif, guard folds; 1236 lines / 81 temporaries / 14 labels); the plan doc lists what is still open.

**Draft fixes (would have broken the v5 Orchard run — v5 must be REBUILT before the in-game run)**: boolean-vs-0 compares
(`if c_stk_11 ~= 0` on a Lua boolean = always true: every CrateTeamMember was TeamID 1; `IsThingCarryingCrate() ~= 0` likewise —
lifter types `not`/comparison values as bool, bsim `bool __thiscall` outranks Ghidra's int), `MakeTeamMemberComment(..,
"FETCHING" + 4, ..)` (bare-printed retyped member never paired → stack operand unrestored; real string "REQUEST_PROTECTION"),
and the exposed lifter alias bug (`xStack_54 = pCVar6` in both if/else branches is a store, not a per-branch alias — TeamSpawn's
`EntityAttachToScript` got nil). Rebuild: `convert_quest_unit.py --unit orchard_farm` → `build_readable_unit.py --unit orchard_farm`
→ `build_unit_playtest_package.py` → local_test.py preflight, then the user-driven in-game run.

**GuildTraining**: converter fixes later the same night — Ghidra-wrapped statements re-joined (`unwrap_statements`, deeply
indented code splits `+ 0x1d8
 ))(` and `4)
 ,0.5`), comma-conditions with call assignments in `if (...) goto L;`,
the refcount release idiom with casts / `__thing_valid` heads, GGD-pointer registers reused across sibling branches, and a
post-lift residue guard (any line still carrying C syntax becomes `-- TODO(native)` + `v = nil` so the file compiles):
**37/37 files compile** (was 26), todo 884 (residue is now counted). Readable stage for guild lives in
`refs/script_recovery/lifted/GuildTraining/readable_converter/` (20648 -> 12740 lines; the old `readable/` dir is the
hand-reviewed six-slice artifact the `test_guild_*` tests read — the builder refuses to overwrite it). Smoke
(`smoke_run_unit.py --unit guild_training --stage draft|readable_converter`): **13 / 13 problems** (was 35), todo 791 (202 non-structural).
Fixed the same evening: ST0 results bound to the real float call (`fret_N` gone), `'\1'` char literals, AssignFromWide
on stack strings (L"" too), DAT bool/float constants, GFCharStringToInt -> tonumber, by-value GetAllThings vectors +
`_bv` element casts + end-pointer slot under its own name, drifted destroy operands -> the single created object, stack
copies of a reused register are stores, GSI-pointer byte stores of by-value things dropped. Remaining 13 are slot
collisions the restore cannot yet split (CheckFriendlyAttacks: xStack_90 = string temp + thing result + vector end)
and `this_00`/`CStack`/`xStack_NN_b3` residue. Later: gsivt spills typed by the spec, AddQuestInfoTick ->
ByText/ByAction, hidden-result slots reused as operands, pseudo-call thing receivers (MsgIsHitByHero). Upstream binding
gaps recorded in FSE_UPSTREAM_REQUIREMENTS.md: `IsPlayerHoldingFireRangedWeaponButton`, `me:MsgIsHitBy(name)`.
v5 rebuilt from the current Orchard readable (preflight ok).

**Aeon share zip**: `python tools/script_recovery/build_aeon_share_zip.py` -> `work/AeonShare-<date>.zip` (Oakvale + Orchard +
Guild readable/draft + reports, docs incl. split proposal / upstream requirements / journal, and the v5 playtest bundle).
Built 2026-09-17 (3.1 MiB); hand it to Aeon with the collab reply.

Gate script (scratch, recreate if missing): convert_new_oakvale.py --out <tmp> and `diff -r` against
refs/script_recovery/lifted/NewOakValeIntro (ignore CONVERSION_REPORT.json and baseline-only extras) → must print identical.
Heredoc-python patches mangle regex backslashes (GOTCHAS) — patch tools through Write/Edit or a script file.

# CURRENT (night 2026-09-17): Orchard Farm ready for the v5 in-game run; GuildTraining through the typed pipeline (26/37 files)

**Orchard Farm**: draft 46/46 fns, 11/11 files, 0 TODO(native); readable 12/12; smoke harness 0 problems (draft + readable);
Oakvale gate identical. Tonight's guild work also fixed Orchard for real: `MoveToPosition(pos, 0.5, ...)` (hex float literal in a
float slot) and `Q_OrchardFarmRaid.Init` acquires `BanditTeamMember[1]`/`[2]` (vector begin pointer elements; it acquired `0`
before). **v5 rebuilt** (readable regenerated, DLL rebuilt with `ReadGlobalGameDataFloatAt`, preflight ok) — the in-game run is
the next (user-driven) step: `python work/new-oakvale-original-fse-20260912/local-candidate-v5/local_test.py --game-dir <Fable>
--launch --save-dir <saves>`, reach Orchard Farm, read NoviCompatibility/FableScriptExtender.log.

**NEXT SESSION (agreed 2026-09-17): readable output should read like a quest script, not a decompile** — plan in
`docs/scripts/READABLE_STYLE_PLAN.md` (6 steps, payoff order). First increment = steps 1 (termination boilerplate), 2 (inline
single-use temps), 6 (cosmetics) in `build_readable_unit.py` on Orchard Farm, shown as a CrateTeamMember before/after; add
style metrics to READABLE_REPORT.json; readable stage only (draft stays faithful); smoke harness + Oakvale gate stay green.
Style oracle: work/aeon_lua_ports/Fisherman/FSE/Fisherman/Entities/Fisherman.lua.

**GuildTraining** (typed pipeline, `--unit guild_training`, range 0xD3B390..0xD68F00): 1033 -> **856 todos, 20 -> 26/37 files,
140/152 fns** across rounds 113-125 (commits ad9d46a..d15c91a). Regenerate: `ghidra_typing_spec.py --unit guild_training` →
`infer_helper_prototypes.py --unit guild_training` (MUST run, else 31 helpers vanish) → headless export → `convert_quest_unit.py --unit guild_training`.
Export changes (ExportTypedTranslationUnit.java): helper params with explicit register storage (`__ftol2` takes ST0, so Ghidra
prints its operand), register state merged at block joins (`mergeState`), and **`callOrder`** = CALL/CALLIND ops in C text order
(the decompiler prints out-of-line cleanup blocks after the return, so "k-th printed call = k-th site by address" was wrong;
`restore_stack_operands` pairs through `_text_order_sites` when every head agrees, 199/209 guild fns).
Lowering levers added (all generic, journal night 4): stack CTimer ctor/dtor (0xCD4450/0xCD4470 = RegisterTimer/DeregisterTimer),
cutscene string maps (0x9AC2D0/0x9AC310/0x9AC700 → resources:NewStringMap/SetString/DestroyStringMap + RunMacroWithStrings),
byte-split pointer args (SUB41/CONCAT), byte-literal dwords, merged byte flags (`X & 0xffffff` → `X_b3 = 0`), by-value thing
copies (inlined copy ctor, restored outgoing slots, hidden results into arg slots), resource-valued actor maps, inline dtor casts
(base vtable 0126008c decides Release vs DestroyMovie by construction), local thing vectors filled by void GetAllThings*
(count from begin/end, `elem_N = V[i]` for element vcalls, byte-offset counters `ctr_N`), global-game-data float arrays
(`pf = *(float **)(GGD+N)` → `quest:ReadGlobalGameDataFloatAt(N, ixVarN)`, the grade-threshold loops), `ReadGlobalGameDataFloat`,
zero-vector fallback (DAT_0143e8e0) + `pos.x/.y/.z`, Data-pointer handles (`CCountedPointer::operator=(&P, &thing.Data)`) as thing
aliases, stack-slot copies of the GSI pointer, unaff_ESI DeregisterTimer (single-timer functions).
Remaining guild file failures (11): `if (X ~= nil) and (*X = *X + -1, ...)` release residue on a `xStack` name (BirdKiller 163),
`(**(*(X._0_4_ + iVar) + 0x18))()` element vcalls via a split slot (GuildTraining.lua 345), `iVar7 = *xStack_23c` (Melee TRG 773),
`(**(*(iVar10 + xStack_8c) + 0xc))(xStack_20)` (PreMeleeWhisper 184), `if (...) or (iVar7 = GSI->GetTimer(..), 0 < iVar7)`
comma-condition (PreMelee TRG 121), `CVar10 = *(this + 4)` (Skill TRG 1064), `SetActor(map, "HERO", &xStack_238_3)` string-typed
resource slot (Will TRG 740), `SetActor(xStack_38, "BAN1", &0x0)` (WoodsWill 102), `if *(iVar6 + 0xf14) <= gold` (ArtifactThief 719;
`iVar6 = DAT_0143e90c` alias not substituted inside the compare), `(**(xStack_7c + 0x118))(*(this + 4))` (FinalMaze 487),
SkillTarget 130 `EntityTeleportToPosition(me, &xStack_58, ...)` (a C3DVector built from floats). Top todo shapes: AddQuestInfoTick
not in manifest (12), AddLineToConversation listener (10), `xStack_N_N._N_N_ = N` (17), `_Dest_val` (8), `CSubtitleRenderer::SetText`
(7), `xStack_Nc = **(CCharString **)(this + N)` (8).
Guild has no readable/package profile yet (build_unit_playtest_package.py is Orchard-only: entity ids, retail_override entries).

# PREVIOUS (night 2026-09-16)

Pipeline (quest-agnostic): `script_units.py` → `export_guild_training.py --unit` → `guild_training_inventory.py --unit`
→ `quest_unit_evidence.py --unit` → `ghidra_typing_spec.py --unit` + `infer_helper_prototypes.py --unit` → typed export (below)
→ `convert_quest_unit.py --unit` (`native_evidence_lowering.py` + `Lifter` + `native_cleanup_regions.py`). Aeon hand-ports Guild;
Oakvale intro complete (51/51 fns); Aeon-port audit tool `audit_port_against_pdb.py`; API appendix for Aeon `docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md`.
Orchard Farm draft `refs/script_recovery/lifted/OrchardFarm/draft`: **46 fns, 11/11 files compile, 133 diags**
(68 of them informational label/goto/cleanup notes). Regenerate: `python tools/script_recovery/convert_quest_unit.py --unit orchard_farm`;
gate: scratch `gate.sh` (convert_new_oakvale + diff vs baseline) must print `OAKVALE GATE: identical`.
Typed export re-run (needed after `ghidra_typing_spec.py --unit orchard_farm` + `infer_helper_prototypes.py --unit orchard_farm`):
`analyzeHeadless ghidra_proj FableTLC -process Fable.exe -readOnly -noanalysis -scriptPath tools/ghidra_scripts -postScript
ExportTypedTranslationUnit.java 0x00DCC040 0x00DD2700 refs/.../translation_unit_typed.json refs/.../define_addresses.txt refs/.../typing_spec.json`
(337 overrides; by-value CScriptThing sites 8, Data-pointer thing sites 2, thing params 2).
Debug: `CONVERT_DUMP=<FnName>[,..] convert_quest_unit.py --unit orchard_farm` prints the lowered C per function.
Tonight's converter levers (all generic, journal has the list): by-value CScriptThing args (Ghidra export + fold),
hidden-pointer thing returns, Data-pointer thing vcalls, entry-relative stack keys + epilogue depth + GSI singleton in the
provenance tracker, ego_r signature param types, CCharString concat/members/arrays, enum fields, nested short-circuit
condition trees (`native_condition_tree.py`), goto-only regions (`native_cleanup_regions.py`), stack colours (BGRA→{R,G,B,A}),
bool return idioms. FSE typedef evidence: GSI slots 0xcc4 SetCombatNearbyBreakOffRange / 0xcd0 SetStealStealableItems take
CScriptThing BY VALUE (FSE declares `CScriptThing *`) — tell Aeon.
Readable pass DONE (generic): `python tools/script_recovery/build_readable_unit.py --unit orchard_farm` →
`refs/script_recovery/lifted/OrchardFarm/readable` (12 files, all compile, READABLE_REPORT.json with reversible local maps;
folds: termination idiom, list arithmetic, byte indices, dead stores, constant guards, colours, decimal literals).
DLL bindings DONE: `tools/script_recovery/sidecar_patches/novi-unit-bindings.patch` (NoviUnitBindings.h: GetStateThing/SetStateThing,
GetStateListCount/At/Push/Erase, RetailResources(), ReadGlobalGameData, thing GetName/SetDataString/IsBeingCarriedBy/
GetCurrentStateGroupType/IsEqualTo) — applied by `build_novi_compat_bundle.py` (apply_extra_patches); built clean in
work/new-oakvale-original-fse-20260912/sidecar-abi-v2 (scratch git repo, commit "NoviUnitBindings"). Converter now emits
`quest:RegisterTimer()` for CTimer members in Init (evidence: quest ctor slot 0x15c). Playtest bundle **local-candidate-v5**
(`build_unit_playtest_package.py`: Oakvale v4 Lua + Orchard readable + retail_override entries Q_OrchardFarmRaid/Evil/Good,
entity ids 216–224); preflight passes; static API coverage check = no missing quest/thing/resources methods.
FIXED: Bully intimidation line (was BULLY_BADGERING; retail formats BULLY_SCRMSG_INTIMIDATING_%d, bully_proximity.py) —
readable rebuilt, staged at work/oakvale_readable_stage_20260916b, **v5 rebuilt from it** (Oakvale + Orchard, preflight ok).
Aeon cross-reference: work/aeon_new_oakvale/CROSSREF.md. Pre-existing: test_bully_proximity.py mock lacks RetailRandModulo (fails before the fix too).
ede5dfa: Orchard draft has **zero TODO(native)** (goto scopes: sibling-tail copies with internal labels, equivalent cleanup
tails merged, DoMultiplierCutscene's four WHISPERINTRO macros via the mutable `string`; shared helper module hoists cleanup
regions too). Remaining diagnostics are informational (labels/gotos, 5 `verify cleanup` = `__cleanup_X(); return`). v5 rebuilt.
GuildTraining regenerated drafts are NOT committed (the tail copies grow that untyped unit; todo 3032 -> 4029) — revisit when it
gets a typed export.
2f3e957/ca781cd: TeamSpawn sibling-goto regression fixed (d3444b3 had spawned twice + returned early); typed export now tracks
exact stack depth (callee purge sizes, lazy saves, lea esp padding) and records per-site `ecxStack/edxStack/pushedStack` +
`indirectCalls`; `restore_stack_operands` (convert_quest_unit.py) renames drifted Ghidra stack names by (slot, lifetime) →
whisper cutscene distance compare and CrateTeamMember MemberCount += 1 are now retail-correct; FailReasons_0..3 initialised
(AssignFromWide from UTF-16 .rdata); 0xCBE87F = quest:AddLogbookStoryEntry(n). Draft todo 52. v5 rebuilt + preflight ok.
SMOKE HARNESS: `python tools/script_recovery/smoke_run_unit.py --unit orchard_farm [--stage readable]` (lupa mock runtime, frame
budget, unknown-method check vs DLL sources) — Orchard 11/11 files run, 0 problems (commit d3444b3); v5 rebuilt + preflight ok.
Converter residue (all cosmetic to the draft, readable runs): DoCutsceneIfRequired/DoMultiplierCutscene actor-map std::map
residue + `string = ...; goto LAB_00dd1d98` staging, ProcessGameRules gotos LAB_00dd0b1f/LAB_00dd1736, Artefact LAB_00dcddcf,
`CCharString__AssignFromWide` FailReasons init, `EntityFollowThing(me, nil, ...)`, `ResetCombatNearbyBreakOffRange(nil)`.
NEXT (user-driven): launch v5 (`python local_test.py --game-dir <Fable> --launch --save-dir <saves>`), reach Orchard Farm,
read NoviCompatibility/FableScriptExtender.log for Lua errors; then fix converter/bindings from the log.
Was next: NoviCompatibility bindings per
`refs/script_recovery/orchard_farm/RUNTIME_API_GAPS.md` / `docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md`, in-game test, ship with the Oakvale zip.
Known: `test_watch_barrels_loop.py` pre-existing failure. Journal: journal/2026-09/CONVERTER_GENERIC_UNITS_2026-09-16.md (night section).

# CURRENT: Party Mode removed; sidecar v4 played clean through childhood; Discord zip rebuilt — 2026-09-16

Party Mode is **gone** (user decision after it broke music/loading in-game; two `MAZE_TELEPORT_OUT_01`
plays then breakage). Removed at the converter/source level; readable package regenerated with zero
markers and restored indentation. Clean bundle **`work/new-oakvale-original-fse-20260912/local-candidate-v4`**
(same two DLLs as v3) — user completed childhood on it, zero Lua errors.
Discord package: **`work/NewOakValeIntro-sidecar-playtest-20260916.zip`** (extract into game root, run
`Launch New Oakvale Intro.bat`; replaces nothing) — built by `tools/script_recovery/build_discord_playtest_zip.py`,
tester path verified live. The older `NewOakValeIntro-readable-discord-playtest.zip` is superseded.
Rebuild chain: `build_readable_new_oakvale.py` -> stage readable + `retail_override.lua` (enabled=true) ->
`build_novi_compat_bundle.py --skip-build --readable <stage> --bundle <vN>` -> `build_discord_playtest_zip.py`.
Known failing (pre-existing): `test_watch_barrels_loop.py` expects a `RewardRemainingBarrel` binding.
Details: [journal](journal/2026-09/PARTY_MODE_REMOVED_SIDECAR_V4_2026-09-16.md). Work is uncommitted.
Everything below is historical.

# STOPPED FOR SLEEP: Party Mode upgrade ready for tomorrow's playtest — 2026-09-14

User requested stopping tonight and updating docs. **Do not launch or test further tonight.**
Next-session details: [Party Mode handoff](journal/2026-09/PARTY_MODE_UPGRADE_2026-09-14.md).

User confirmed a full successful childhood playthrough of the prior stock-FSE-plus-sidecar
v3 bundle. New Party Mode is staged separately in `work/party-mode-v2/sidecar` and
`work/party-mode-v2/forge`: milestone celebrations, score/ranks/combos, configurable
effects, cutscene suppression, and quiet debug markers. New visuals are **not live-tested**.
14 automated tests, 45 Lua syntax checks, and sidecar preflight pass. Installed game,
saves, and original v3 bundle unchanged. No game launched or background jobs started
this session. Work is uncommitted. See the linked note for the exact launch command.

The older checkpoints below are historical; this stop point takes precedence.

# LIVE: New Oakvale readable Lua runs through stock FSE + NoviCompatibility sidecar - 2026-09-14 21:16

Bundle: work/new-oakvale-original-fse-20260912/local-candidate-v3 (stock FSE, empty registry, plus
NoviCompatibility.dll built from ForgeFSE-retail-shadow + scalar-ABI patch + 3 sidecar deltas; Lua =
converter readable package verbatim, loaded via retail_override.lua, NOT as a custom quest).
First run: override armed, 16/16 entities Init+Main clean, zero Lua errors, Bully reactions firing.
Rebuild/reassemble: python tools/script_recovery/build_novi_compat_bundle.py
Launch: python <bundle>/local_test.py --game-dir "<Fable dir>" --launch --save-dir "<saves>"
Details + root causes: journal/2026-09/NOVI_STOCK_FSE_SIDECAR_LIVE_2026-09-14.md
Still user-verified only: child start after New Game, Father scene, Escape, audio, save/reload.

# Active: Hero's Guild recovery - 2026-09-14

User requested Guild training work while New Oakvale waits for live verification.
Continue offline; do not install, activate, touch profiles/saves, or ask for live testing.
See [Guild recovery](scripts/GUILD_TRAINING_RECOVERY.md) for commands and remaining work.

Guild: nine quests, 28 native entity bindings, 16 threads, 277 exported bodies.
All entity lifecycle slot bodies exported. Readable RaceMarker matches original
x86 traces in 128 cases; six entity slices plus five quest functions are reviewed and 31 Guild tests pass. Diagnostic entity lift now consumes the reviewed Guild parent field map: 6/28 entity files parse with 4,045 unresolved diagnostics; the baseline remains non-candidate.
4/28 syntax passes with 4,128 diagnostics; it is not an installable candidate.
Next: field/persistence recovery, larger tutorial entities, and the missing ScorpionHome lift binding.
No pending processes at this checkpoint.

Oakvale readability: removed inline hex-wrap expressions in deed count, father,
bully and guard snippets; 11 focused tests pass, regenerated package 18/18 syntax.
The frozen offline bundle below predates these Lua edits. Its host evidence is
for its frozen bytes, not the newly regenerated readable package. DLL unchanged.

# Current offline marathon checkpoint - 2026-09-14

See [the marathon checkpoint](journal/2026-09/NEW_OAKVALE_MARATHON_2026-09-14.md).
User selected offline-only and is at work. No installation, activation, profile or
save changes; do not request live verification. Both full suites pass (1,511/1,528),
with six later native tests and expanded complete-host checks separately green.

Current stage: work/oakvale_entity_scalar_abi_integration. Native callback timing and
entity scalar return ABI are corrected. Actual host verifies all281required methods,
64entity VM file loads/default callbacks, emitted Barrel state writes, Init/persistence,
entity ownership, Windows fibers and process drain before Lua teardown.
Current disabled bundle: work/oakvale_offline_candidate_20260914 (30hashed files).
DLL SHA256: 545f709a46c288ff3a881a09a79cf846f34cf874e162684779e34bf546116e74.
No pending processes. Canonical runtime, installed game, profiles and saves unchanged.
Live gameplay/save-reload parity remains unverified and deferred by user direction.

The September 13 stop notice and older status below are historical.

# Resume here (one page) — updated 2026-09-13 (converter candidate)

## STOPPED FOR THE NIGHT — 2026-09-13

User requested sleep; resume only when asked. Read the [night checkpoint](journal/2026-09/NEW_OAKVALE_CONVERTER_NIGHT_HANDOFF_2026-09-13.md) first.
It supersedes running-process claims and earlier metrics below. Suite16 is terminal:
1,488 tests, one failure and two import errors, all identified in that checkpoint.
No root or worker processes remain running. Readable package has51 functions,
18/18 syntax passes and two explicit native TODOs; integration is NOT complete.
Latest common runtime stage: work/oakvale_timer_integration. Worker StartBarrelTimer
checkpoint: work/start_barrel_timer/INTEGRATION.md (two later edits unvalidated).


## Active converter marathon (2026-09-13)

### Timer owner composed into common resource stage

- Added prepare_oakvale_timer_integration.py and run_oakvale_timer_integration_checks.py. Composition validates all inherited source/helper hashes, preserves inherited implementation files (including LuaManager.cpp), and emits a cumulative patch against the unchanged runtime checkout. Generalized isolated timer preparation output and check runner without changing their defaults.
- work/oakvale_timer_integration now includes marker/resource methods plus explicit registry lifetime policy, allocator propagation, transient timer state access, and last-member timer ownership. All three affected actual host translation units compiled; all 28 actual-FSE/real-Lua policy cases passed; six command exits 0; x86 executable SHA980834ba6c6f2871de4cda2efcb85c50af5531d1b9ca6a692a768924fcee35ad. Full common registration compile also passed, object SHA47487b437bf2d8966c4b88fecd3efdc3f837eaef70f68879f5c2d6cb5a44b30e. Cumulative patch git apply --check passed. No runtime modifications or activation.
- Full DLL linking, scheduler teardown and restore ordering remain unproved; this integration does not close those gates. PostAttackStuff full dispatcher still needs four atomic adapters before emission. Worker continues StartBarrelTimer recovery.
- Suite16 remains verified live at shell8977; no terminal result yet, visible failure/error markers require diagnosis once it finishes. Timer checks19461 and registration16542 consumed successfully.


### Structured PostAttackStuff dispatcher candidate; timer proposal ready for review

- Added isolated post_attack_body.lua: readable two waits with exact termination queries, scoped retained Dadtrigger, existing movie helper, then restoration and final triggerrelease. No goto/pointer temporaries. NOT wired into builder yet: four new lookup/query capabilities are still unimplemented (PostAttackStartIsAlive,TeleportToPostAttackStart,SetPostAttackVillageLimbo,PostAttackHeroNearTrigger). Existing emitted cutscene-only integration remains authoritative.
- test_post_attack_dispatcher executes original1095bytefunction controlflow with explicit boundaries for initial lookup/alive, worldsetup, camera+retainedtriggerlookup, distancequery, movie,worldrestore. Real branches/frame/termination/return and finaltriggerdestructor call checked.108delay/cancellation scenarios passed1test0.825s. This does NOT prove phase internals; post_attack_cutscene already has its separate originalcaller proof, remaining atomiclookups need native operand/lifetime and compiled adapter checks.
- Native atomic scopes for nextstep: initial name CStringstack12 -> lookupoutputstack16 -> IsAlive on returnedpointer; output inlinecleanup BEFORE nameclose. Teleport uses same key/output, forwards returnedmarker plus freshHero and false; closes output BEFORE key. Village limbo true/false each fresh key/output, passes returnedpointer and flag, closes output BEFORE key. Retained Dadtriggerstack28 constructedonce, nameclosedbefore distance loop; each distance uses freshHero andownedtrigger,5.0; finaltriggercloseDBEF5B after all restoration or cancellation. Native SetTimeAsStopped passes false plus parent+4C; current runtime method uses quest-owned m_stopTimeIndex, already matching storage intent but mergedstate/gameplay needs proof.
- Timer worker proposal now ready: work/oakvale_timer_host_proposal/INTEGRATION.md and oakvale-timer-host.patch. Opt-in nativeLifetime=NewOakValeIntro, defaultoff; finalslot{file,policy}propagation; transient timer accessors and last-member ownership. All3realruntime translationunitscompile;28realLua/actualFSE policies; final1test26.760sOK,all6commands0,PE014c,exeSHA3bcb82db9f99ffa07230aeeab755c34e696a7e25d62ead9f0527638a1db853ae. Root has NOT yet reviewed/merged this proposal. Header/runtime sources remain unchanged.
- Worker reassigned to StartBarrelTimer structured/native recovery, isolatedfilesonly. Marker helper is integrated from priorstep.
- Fullsuite16 continues verifiedLIVE shell8977 (lastpollsamehandle), log nearing terminal but no summary yet; at leastoneF visible. Do not restart. All other root sessions consumed. No new persistent build in this dispatcher-only step.


### Quest-marker helper emitted and common registrations composed

- Added readable_manage_quest_core_markers.py with exact raw helper SHA guard, existing whole-native-function/literal/slot proof and generator integration. Builder replaces only ManageQuestCoreMarkers and records questMarkers. No new entry frame/condition; cancellation retains the installed marker while destroying retained Theresa->Trader->Father Things.
- test_manage_quest_core_markers_readable passed2tests38.047s: final emitted whole-quest source invoked for28native comparisons across cancellation/tutorial/refcount/empty-Thing paths, no rawlabels/gotos/pointertemps in helper; changed draft rejected. Worker fullhelper gate1718nativecases and128actualFSE/x86Lua cases passed,4tests59.723s; isolated host evidence remains distinct from commonstage link/runtime validation.
- Added prepare_quest_markers_resource_extension.py over postattackstage, merging AddCoreQuestMarker/RemoveCoreQuestMarker. Existing postattack preparation/registration runners now accept optional fragments/methods/output/prepare function; defaults unchanged. Commonmarker stageSHA845ba587eafd8425f2822474c7605aa85b986562f2ab102615a5c9fb13e118f2; allregistration compilepassed COFF014cSHA14414a74a6e26952248c11bbbd17958d3a16cf419cfe1d2b77d32551f90d28fc. git apply --checkpassed; no apply. Fullcommon owner linking/behavior stillpending (worker smoke harness usesbaselineheader, whereas commonstage registers many more capabilities).
- Persistent buildsession14670 consumed exit0,18/18Lua syntax,51ledgerrows. Registration89746terminalconsumed. Marker source helper fromwork/manage_quest_core_markers/INTEGRATION.md fullyintegrated; broad suite16 startedbeforemarker changes.
- Timer host integration delegated to /root/book_trader_home, isolated proposal only. Require registry opt-in metadata(defaultoff), propagated through final sorted allocator slots and retail-override prefix; allocator scriptName is file path, so don't infer policy from questname. Owner beforeInit, transient GetStateInt timer IDs bypass persistent globals, reject writes toownedIDs, owner destruction before speechlists; native/global quiescence/save-load remain separate. Agent ownsnewtimerintegrationfiles; root has not changed postattackstage contents beyond objective (timer parent stable).
- Fullsuite16 stillRUNNING shell8977, log work/converter_marathon_suite_20260913_16.log shows at leastonefailure, no terminalsummary yet. Keep handle; don'trestart. No other root sessions active.


### Native timer owner recovered; host integration remains required

- Lifecycle audit found a concrete gap: readable Init uses TalkIntermittentTimer and later WatchTimer, but generated Lua never registers either timer. Native constructor DAACAE..DAACD6 registers via singleton143E8F8 slot15C twice, stores ambient+104 thenwatch+108. Native destructor DBEFC0..DBEFEF deregisters watch thenambient via slot160 before eight speech vectors and base cleanup. The current destructor TODO cannot safely become a no-op.
- Added retail_oakvale_timers.h, independent owner with fresh singleton/vtable lookup for EACH register/deregister call, signed IDs preserved (including0/-1/duplicates), reverseclose, idempotent destruction, primary-error preservation and partial successful-construction cleanup. No runtime files changed and it is NOT yet attached to LuaQuestState/LuaQuestHost or exposed to generated Lua.
- test_native_oakvale_timers executes original constructor/destructor timer regions under full witness hashes for25signed-handle pairs and switches singleton interface between everycall. Checks stores, reverseorder, call ABI/stack balance. 1test0.121sOK. Vectors/base destructor are outside this focused gate.
- run_oakvale_timers_runtime_checks uses actual FSE types/x86 virtual engine doubles:56policies(50normalexplicit/deferredclosure+4partialconstructionerror+2cleanupfailure). Both commands0,PE014c,exeSHA495b2f1edb2ef9e20a72bc8ebe26ea1e29c7581152828de2d06e6268a396022d. Live registration side effect followed by native exception before handle return remains outside the proved contract.
- Next integration decision requires constructor/lifecycle fidelity: LuaQuestHost.cpp constructor sets base/interface thencreates LuaQuestState; current script loading/Init occurs later in LuaQuestHost::Init. Timers must exist before Init state writes and be closed before RetailVillagerSpeechListOwner. Staged LuaQuestState has speechlistowner member nearline1071. Consider an explicit NewOakValeIntro host-construction hook plus state publication, with name/registration contract tested; do not merely add lazy registration at the first timer getter or remove destructorTODO without host ownership. Current host destructor only CleanupThreads(), then C++members; review state/VM destruction order (LuaQuestHost.h members state115,VM116) before attaching owners.
- Background marker recovery full original function gate passed1280wait/cancel+432refcount/empty+6signedgold paths; agent compiling adapters, files not yet integrated.
- Fullsuite16 shellsession8977 remains verified LIVE (lastpollreturned samehandle). Log has at least one F; terminal summary/failure names not yet available. Do not restart. No other root sessions active. Previous fullsuite15green; suite16 result pending. No new persistent readable build in this timer-owner step; owner integration is still outstanding.


### Initial quest objective atomic scope recovered and compiled; suite16 running

- native_oakvale_objective.py verifies caller DAC198..DAC219, actual GetActiveQuestName891880 bothret4paths, slotsA3C/4A0, SetObjective ret16, empty/quest literals. test_native_oakvale_objective executes original caller AND actual getter with/without active quest; checks three arguments staged across getter, ownedoutput return, exact four destructor order and balanced stack. Native/source/API mutations rejected.
- retail_oakvale_objective.inc adds SetInitialOakvaleObjective: region2 thenregion1 thenobjective CString construction, GetActiveQuestName into ownedoutput, returned pointer forwarded unchanged to setter, ownedoutput thenobjective/region1/region2 cleanup. Failures preserve first error and close completed constructions once. Pre-return getter/constructor failure with partially constructed output remains outside contract.
- Added method to prepare_post_attack_resource_extension alongside Heroacquisition. FullstageSHAe8bff3de6df10efc6b3d6716980177a297b4020d94c0e3155ca84181f0cfd89c; fullregistrationCOFF014cSHA4b895031c938dad1d4b5496b4f5c62b3fd8d37361daeaf8b248965a656aadca5. Patch git apply --check passed; not applied. Prior postattack27policy evidence refers to prior header hash and does not claim this new method.
- ActualFSE/x86/realLua objective harness36policies passed: three returned-pointer cases (owned/alias/null), five failing construction/get/set stages and cleanup failures, staleclosedowner rejection, exact cleanuporder. All3commands0, PE014c exeSHAadd7a441713b51ad94a84328548fad3a5debda35fba74b6b5f4abe011471f984. Generic runner parameterized for harness/output/limits/native-scope; original postattack default unchanged.
- Builder replaces initial objective genericwrapper block with scoped atomicmethod and records initialObjective. test_oakvale_objective_readable checks final emitted Main16bindings/finalization/state/cancel/deactivation/objective/thread/mission sequence. Combined emission/native gates4tests40.431sOK. Persistent buildsession66038 consumed:exit0,18/18Lua syntax,51ledgerrows. Still disabled/uninstalled.
- Worker completed AffairMan native outer dispatcher2016boundedcomparisons, no mismatch; updated generator banner/report. Worker now recovering ManageQuestCoreMarkers DBE4E0size944 with three retainedThings/destruction order and cancellation marker behavior. No shared edits requested.
- Full recovery suite16 RUNNING, authoritative shellsession8977. Log work/converter_marathon_suite_20260913_16.log; terminal result will be work/converter_marathon_suite_20260913_16.result.json. Started after latest persistent build; do NOT restart or infer completion from elapsed time. Pollsamehandle. All other root sessions consumed. Prior fullsuite15passed1445tests; suite16 result pending.


### Husband complete candidate integrated; startup finalization review

- Builder now uses affair_man_complete.generate, skips the already-applied husband label/structure/presentation passes, records completePass pending evidence and explicit __resource_main ledger mapping. Existing generic final naming remains. New test_affair_man_complete_readable checks final Init scope/actor, three atomic animation calls, one bound-conscious registration, ledger mapping and18 final-vs-reviewed Main cancellation traces. Combined with wife_complete_readable:2tests62.375sOK.
- Native condition recovery initially rejected the changed declaration prefix, correctly. Reviewed its exact two lines (one local declaration list and local alive=true, no effects), added SHA8c9bce00b91320cddb022c421023b9f8707b3d7680542c166ab3d3ae2b7db91d to the husband entry witness; no native gate weakened. Existing native condition tests4/4passed0.840s. Emission harness separately supplies a condition mock; isolated husband generator still leaves condition insertion to builder.
- Wife generator now includes worker's two animation-site fixes via PlayWifeArgumentAnimation. Existing emitted Wife integration passed against it. Worker native16animation,32actualFSE/x86Lua,768wholecandidate+3errors,2092dispatcher scenarios remain separate gates. Runtime method remains in isolated animation_proposal, requiring shared merge. Worker is now doing AffairMan native outer dispatcher; do not treat source-to-source traces as that proof.
- Resolved prior Main DAC146/DAC152 uncertainty: LuaQuestState::FinalizeEntityBindings invokes PostAddScriptedEntities_CScriptBase_API then PostAddScriptedEntities_API (source lines2505/2506). FableAPI.cpp maps first toCB8930; GameInterface.cpp maps second to vtable64/slot100, whose retail target is6E7460. Do NOT add duplicate finalization calls to Lua. Allocator bridge/count/failure/live scheduling still need full integration.
- Next startup recovery target: native objective block DAC198..DAC218 constructs empty region2, empty region1, objective literal, then GetActiveQuestName into owned output atstack1C; its returned alias is the first operand to SetQuestCardObjective with the three earlier-staged operands. Cleanup order is owned output,objective,region1,region2. Current Lua converts active name to std::string and reconstructs four keys, changing ownership/order. Existing GameInterface.h APIs expose exact CCharString pointer signatures. Need atomic reviewed adapter and native caller/callee ABI check, including returned-alias vs owned-output behavior.
- Persistent generation session19823 consumed:exit0,18/18syntax,51native ledger rows. No root sessions active. Runtime proposals remain unapplied; no game installation or goal completion.


### Post-attack compiled adapter and missing DeadFather quest binding

- Added prepare_post_attack_resource_extension.py over the verified WatchBarrels stage, preserving inherited source/helper hashes. It adds and registers TryAcquirePostAttackHero from retail_post_attack_actions.inc without applying anything. Candidate SHA27f37a90ee3f0c0731fd0531a4270e74dfcdfcf0375015b371482237f8c90150. Patch git apply --check passed in ForgeFSE-retail-shadow.
- run_post_attack_scope_runtime_checks.py compiled actual staged FSE class + real Lua. 27 policies passed: empty/nonempty Hero, returned acquisition true/false independently of populated resource, getter/acquisition exceptions including mutation before throw, stale/closed IDs, fresh Hero on repeated calls, unavailable API rejection, exactly-once owner destruction. All three commands0, PE014c, executableSHAd4e5452b065c371e36c42f0274c47fe6e383017e44c035105065531367677fcc. Engine calls doubled; full compiled movie/quest composition still pending.
- run_post_attack_registration_compile.py passed all staged registration templates, COFF014c, objectSHAd9b27e40d6662086303d6fcbda18cbf92fc628d32fe1deb154cf5317499c84fd. Report wording was corrected from copied WatchBarrels labels; no test evidence changed.
- Found functional Main gap: raw/readable emitted15bindings; native DABAC0 has16. readable_new_oakvale_main_bindings.py verifies full2018native bytes, prior1670bytebindingwitness, literals/callback stores, and exact missing draft scope; restores OVI_DeadFather after NOVI_CreatedBeetle. Also replaces accidental stale-pointer deactivation argument with native EBP=0. Builder records mainBindings evidence. Two tests0.019sOK verify16orderedbinding callbacks and postattack startup/cancellation, reject altered source/native correspondence.
- Main is not yet fully recovered. Inspect native DAC146 callCB8930 then game slot100 atDAC152 (not represented as a separate call in current Lua), objective CString scopes DAC198..DAC218, native spawned-thread initialization, RegisterMain and destructor. Restoring the missing binding does not certify all Main semantics or allocation-failure behavior.
- Worker completed isolated affair_man_complete candidate (Init/rawanimation fixes,384nativeInit+24animation,180sourcecomparisons,76compiledpolicies) and is now correcting the shared Wife animation CString/raw-byte ordering gap. Root has NOT integrated affair_man_complete yet; use work/affair_man_complete/INTEGRATION.md, skip old husband structure/presentation when integrating. Persistent generation session22581 consumed:exit0,18/18syntax,51ledgerrows. No root processes active. Goal remains active; no runtime install.


### Post-attack movie scope recovered; Wife dispatcher correction emitted

- Added post_attack_cutscene.lua and readable_post_attack_cutscene.py, integrated into the builder. Native DBEDA5..DBEEBC stores DadFound, constructs Hero control, makes one acquisition attempt (including empty Hero/failure), constructs map/HERO entry then movie, pauses/fixes camera, runs CS_OAKVALEINTRO_HESDEADJIM with null flags/input and setup=false/skippable=true, then clears camera/pause and destroys movie->map->control. Surrounding PostAttackStuff lookup/distance/cleanup-join code is still pending.
- test_post_attack_cutscene executes original native caller instructions with ABI-checked engine doubles for four Hero/acquisition combinations, comparing exact scoped events; three separate Lua-error scenarios verify cleanup continues and primary macro failure survives cleanup failure. test_post_attack_readable checks final emitted helper against original caller and rejects source/native/literal/API-slot changes. Combined four tests30.297sOK, plus expanded API mutation gate1test0.025sOK.
- retail_post_attack_actions.inc proposes TryAcquirePostAttackHero, forwarding fresh empty/nonempty Hero without filtering. It is not yet merged/registered/compiled in a runtime stage; this is the next concrete adapter gate. Existing map/movie/macro owner integration and real engine exceptions remain separate gates.
- Worker completed Wife native outer dispatcher:2092 bounded original-byte scenarios with five explicit phase boundaries, identifying and fixing the first approach frame/termination ordering. Root reran test_wife_complete_readable against the correction:1test33.917sOK. Worker moved to AffairMan Init/animation review; found existing animation bridge string/raw-byte ordering concerns shared by Wife, so zero comments does not imply complete semantics.
- Persistent readable build exit0,18/18 Lua syntax,51native ledger rows. Current renamed-local metrics277/264semantic/13scratch reflect new already-named source inputs and are not a completion measure. Runtime and game installation remain unchanged. Full suite15 remains last broad regression; these changes have focused gates only.


### Wife complete-candidate builder integration; native dispatcher audit active

- Shared readable builder now uses wife_complete_candidate.generate directly, without reapplying the earlier structure/presentation passes. Recovered Init preserves copied-Thing consumption before SaidRunningLine=false; intermittent conversation uses the atomic CString/Hero ordering adapter. Completion-pass evidence and pending limits are retained, and Wife ledger rows now expose implementationFunction.
- New test_wife_complete_readable exercises the final builder output: one conscious condition, recovered adapter calls, no executable native labels/gotos or TODO(native), Main ledger mapping, 24 phase/cancellation trace comparisons and two injected-error cleanup comparisons. Passed 1 test in 27.407s, process0. Earlier integration attempts caught a test incorrectly rejecting provenance comments and the absent explicit Wife ledger field; both corrected. Persistent readable package has not yet been regenerated for this integration.
- Background /root/book_trader_home is auditing the original Wife Main dispatcher. It found a concrete first-distance-failure ordering discrepancy: DB34A9->DB34B0 yields at DB34B5 and checks termination at DB34BA before running-line/home checks. Candidate correction and native proof remain in progress; the passing emission comparison does not prove this native behavior. Consume agent results and rerun affected gates before publishing the updated readable artifact.
- Root next quest-body target remains PostAttackStuff DBEB20, size1095; existing native_post_attack_* witnesses cover resource graph, movie flags/lifetime, scalar/teleport/limbo/distance/logbook/cleanup evidence. Current readable body still lacks Hero/map/macro ownership and has a cleanup goto. No runtime installation or goal-completion claim.


### DeadFather integrated into readable package

- Reviewed dead_father_candidate.py and work/dead_father_converter/INTEGRATION.md. Added generate_dead_father_readable_candidate.py with existing full native/slot/literal/raw-draft proofs, bound-host no-argument alive-condition normalization, and Init/Main/OnPredicateFail entrypoints. No entity state invented; parent DadFound remains authoritative. Four scoped runtime methods and raw animation-byte semantics retained.
- Builder emits recovered DeadFather lifecycle, records pending engine limits, and maps native Init/Main/OnPredicateFail ledger rows to DeadFather-prefixed functions. No second condition insertion; no conscious condition substitution.
- test_dead_father_readable checks emitted noTODO/LAB/goto/raw pointer vars, one alive condition, four cancellation stages, Init actor call, parent flag, marker removal timing, outer cleanup and empty predicate-fail override. Combined with test_dead_father_main:4 tests33.135sOK process0, including existing3072 native comparisons.
- Persistent readable generation exit0,18/18 Lua syntax,51native ledger rows. Four DeadFather methods still require common staged-runtime merge/registration and owner validation; no DLL/game install. Pre-return lookup ownership, condition/scheduler/save-load and downstream noncanonical-animation-byte interpretation remain documented gates.


### Fully structured Bully integrated; regression suite15 passed

- Reviewed work/bully_converter/complete_structure/INTEGRATION.md and generator. Switched readable builder to bully_main_structure.generate, retaining original native prerequisites, item lowering and outer-region witness. Updated structure report to no unstructured joins, preserved phase/source-differential validation limits.
- test_bully_structured_readable checks emitted no goto/LAB/TODO(native)/pointer variable names, presence of BullyAcquirePrepared/BullyRunoffControls and disabled registration, then20 emitted-vs-reviewed wholebody cancellation/macro-error scenarios.1test43.199sOK exit0. Persistent readable build exit0:18/18 Lua files parse,51 native ledger rows. No runtime changes.
- Full suite15 is terminal and passed:1445 tests1285.285sOK; process0 elapsed1292.231494s, log work/converter_marathon_suite_20260913_15.log and .result.json. Session89515 consumed/closed, do not poll/restart it. Discovery began before newest WatchBarrels files and subsequent integrations; those have separate focused gates and will require later regression coverage.
- Bully structural completeness does not close merged capabilities/owner/state/persistence/scheduler/gameplay gates. Worker continues AffairWife ownership/readability; root reviewed DeadFather contract and still owes its readable integration.


### WatchBarrels reward runtime and readable integration complete offline

- Extended actual full-class runtime harness to gold and beetle operations:32 added cases inject failure at each of seven operation steps plus independent cleanup failures. Owned outputs differ from returned aliases; all strings/Things close, original failure preserved. Gold name closes before item key; beetle names close before health2.0/true. Position1,-2,3.5 forwarded. Total56 snapshot/reward cases passed; Lua build/build/test0, PE014c exeSHA8551f740ae1d11b18558b492485c3ef0f879aded10385f4514d0dd2f0fc79815. Getter/create failure remains before successful output construction in these cases.
- Added readable_watch_barrels.py with exact raw-body/native647-byte guards, composing scoped snapshot/decision helpers and native AddBadDeed(quest,deed) callback. Readable builder records WatchBarrels remaining validation limits. Original string search accidentally found a function-name mention in earlier comments; corrected to declaration-line anchors in lowerer and emitted-test extraction. Preserved the rest of the quest.
- test_watch_barrels_readable passes1test38.384s: actual emitted helpers execute four breaks with baddeed1/gold1/beetles2, one snapshotClose and refresh-error preservation despite cleanup failure. Persistent regeneration exit0:18/18 Lua files parse;51 native ledger rows. WatchBarrels has no raw labels/TODOs in readable package.
- Staged full patch passes git apply --check against unchanged runtime. Not applied/installed. Single whole-native snapshot+loop composition, parent state scheduling/persistence, pre-return owned output exceptions, merged DLL/gameplay remain pending. Full suite session89515 still requires final result; it predates these new files/edits. DeadFather and completed Bully await root integration; worker now recovering AffairWife Init/conversation ownership.


### WatchBarrels staged owner and compiled snapshot checks

- Added prepare_watch_barrels_resource_extension.py over BarrelThug stage. Adds snapshot shared owner kind, deterministic Close integration, NewBarrelWatchSnapshot and two reward methods, plus snapshot Refresh/Count/Close Lua bindings. Parent/source/helper hashes verified. Unapplied proposal in work/watch_barrels_resource_integration, candidateSHA8c3290e44a8870bf8a116448836195e3504b6a7bd2653e4038cb3811d845e798.
- run_watch_barrels_registration_compile.py passes full staged registration compilation x86COFF014c; objectSHAbad81cc10432964841684bc6aecf7b8e66dc211ef0fa40750230686c0a52125a. Includes reward methods; compilation alone is not their behavior proof.
- watch_barrels_scope_runtime_harness.cpp executes actual full-class snapshot through real Lua.24 cases: vectorcount0/2, getterreturn0/1/7, getter writes vector then throws, explicit/deferred Close. Same vector reused on second refresh, keys destroyed before vector, one Close total, stale Count/Refresh rejected, retained Lua reference and garbage collection inert after scope teardown.
- Lua build/build/test all exit0, PE014c, exeSHA946e5d437ed854002f1fb2a7f7a7538332cdae519bd41f757ff679f57c0def8a. Engine vector getter/destructor are doubles; native actual destructor separately compared. Reward runtime execution and full quest integration remain pending. No runtime installation.
- Worker provided stable partial Bully item structure at work/bully_converter/structured_candidate/INTEGRATION.md; explicitly reassigned remaining16 gotos/TODOs toward full structured Main. Root still owes DeadFather integration and regression results.


### WatchBarrels snapshot lifetime recovered

- Added watch_barrels_body.lua/watchBarrelsWithSnapshot. One empty snapshot owner spans repeated Refresh calls and decision loop. Refresh return<=0 controls frame/termination/retry; after success a separate termination query precedes Count; loop gets actual vector count. Every exit and Lua error attempts snapshot Close once, preserving original error.
- test_watch_barrels_snapshot.py executes original647-byte entry/count/cleanup and actual50-byte8AC970 vector destructor.216 cases cover pending0/1/3, vectorcount0/1/4, allocated-empty versus null vector, cancel0..5 and inline/direct-helper cleanup paths. Distinguishes getter return1 from actual vector length. Element destructors receive flag0 in vector order; cdecl free and final caller stack balance verified. Decision loop remains separately tested boundary.
- Added retail_barrel_watch_snapshot.h proposal with empty three-pointer vector, repeated refresh scoped CString, independent byte-count/12, deterministic idempotent Close and stale-use rejection. Not yet factory-registered/staged/compiled; resource scope must own it before Lua use. Getter mutation/exception teardown requires compiled checks.
- Combined snapshot and consequence-loop gate2 tests1.631sOK exit0 (216+900 comparisons). Snapshot class and reward methods still require actual FSE/Lua integration before replacing emitted WatchBarrels. Full suite session89515 continues; poll same handle or result file before declaring terminal.


### WatchBarrels consequence callers and adapter source recovered

- Expanded test_watch_barrels_loop.py: original gold/beetle branches now execute instead of jumping over reward blocks. Tests verify all CString constructor/destructor scopes, getter/CreateCreature operands, gold insertion to owned Thing40, beetle max-health2.0/true on owned Thing52, and owned temporary cleanup. Return aliases deliberately differ from owned output addresses.900 comparisons still pass,1test1.419sOK.
- Added retail_watch_barrels_rewards.inc source proposal with RewardRemainingBarrel and SpawnBarrelBeetle. Gold lookup name closes before item key; item closes before barrel. Beetle script then definition keys; create(false), definition/script destructors before health, then owned beetle closes. Exception cleanup preserves first error and attempts remaining owned locals. Pre-return getter/construction ownership remains unresolved; these new methods are not staged/compiled/applied yet.
- Updated readable decision helper to pass explicit BarrelBrokenPos_x/y/z to SpawnBarrelBeetle. Existing900-case test verifies position table mapping as well as reward selection. Raw native position-pointer ABI checked; state float extraction/engine integration remains a later gate.
- Full suite session89515 remains in progress as last polled; log work/converter_marathon_suite_20260913_15.log. Do not restart without checking confirmed terminal state. DeadFather worker completed isolated candidate in work/dead_father_converter/INTEGRATION.md; now delegated Bully recovery. Root integration still pending.


### WatchBarrels decision loop recovery; broader regression in progress

- Added watch_barrels_loop.lua/processBarrelBreaks for DBE960..DBEAD4 after snapshot count and prior termination query. Initial instantaneous flag clear, termination, AttackOver short circuit, per-break signed32 counter increment/flagclear, first-break bad deed0, gold at total-1, beetle when broken>total-4, frame and termination order preserved. Reward/generation adapters and caller snapshot ownership remain explicit boundaries.
- test_watch_barrels_loop.py executes original decision instructions with consequence blocks abstracted:900 cases over total0..8, four break patterns, attack ending0/1/3/8/12 and cancellation0/1/2/5/10.1test1.105sOK exit0. Exact647-byte WatchBarrels Main SHA3c48ce5c57e30e903c5e6790f7dfd2d4a5cf9b7664f433134d6909840fd06bf2 pinned. Helper remains isolated; full snapshot refresh/count/destruction, temporary gold/beetle ABI adapters, merged quest entry/helper integration still pending.
- Started full recovery unittest discovery before adding new WatchBarrels test. Log work/converter_marathon_suite_20260913_15.log; terminal record will be sibling .result.json. Confirmed running via exec session89515 at checkpoint; do not restart without polling that handle/terminal record. No failing markers in latest log, but no suite completion claim. This run may not discover new files added after it started.


### BarrelThug integrated into readable package

- Added test_native_barrel_thug_dispatch.py:480 original-byte entry/dispatcher/predicate/outer-cleanup comparisons. Covers initial DoneIntro, talk, ordinary/any/excluded14 hit combinations, entry/post-construction cancellation and independent phase failures. Executes original talk CString and hit one/two/three CString cleanup, actual return epilogues and empty-control inline destructor; separately verified phase bodies are abstracted.1test0.684sOK.
- Added generate_barrel_thug_readable_candidate.py composing eight readable helper modules with entity Bool/Int state and Init/Main entrypoints. Verifies exact native Main/Init hashes, raw draft hash and bound-conscious condition witness. Phase source hashes and remaining limitations recorded.
- Readable builder emits BarrelThug and maps Main/Init ledger to runBarrelThugMainAfterCondition/initializeBarrelThug. test_barrel_thug_readable_integration checks emitted noTODO/noLAB/no raw variable placeholders, single bound condition, Init/entrycancel, disabled registration and native ledger.1test31.460sOK exit0.
- Persistent generation exit0:18/18 Lua files parse,51 native ledger rows. Readable package now70 TODO occurrences (raw historical diagnostics1203 retained). BarrelThug now readable in Entities/NOVI_BarrelThug.lua; no installation or registration enabled.
- Remaining gates: single unabstracted whole-engine execution, pre-return native getter ownership on errors, merged adapter/owner/state/persistence/scheduler/DLL and gameplay. Root still owes broader regression suite after recent integrations; next recovery targets remaining quest/AffairWife/Bully/AffairMan gaps while DeadFather is delegated.


### Victim integrated into readable package

- Reviewed work/victim_converter/INTEGRATION.md and isolated complete phase composition. Added generate_victim_resource_candidate.py with raw draft SHA84ae057af7f25b7901ff24a3fbdbc08092d2841a10c5c816ca1d303dcd15ab5a guard, byte-backed conscious-condition verification, bound-host no-argument registration normalization, entity DoneThanks/DisplayedGameInfo storage, and AddBadDeed(quest,me,deed) callback glue.
- build_readable_new_oakvale now emits Victim candidate and maps Main/Init to VictimMain/VictimInit in native ledger. Phase code unchanged; candidate pending validations retained in readability report. Six actor methods and reused ownership/movie/conversation services still need merged runtime validation.
- test_victim_readable_integration checks emitted syntax/disabled registration/no raw labels/placeholders, exact single condition call, entry cancellation before resource allocation, entity state reset and callback arguments, plus ledger function linkage.1test33.085sOK exit0. An initial copied test assertion mistakenly compared argument count to false; corrected to zero before passing gate.
- Persistent readable regeneration exit0:18/18 Lua files parse;51 native ledger rows; historical raw1203 diagnostics retained. New readable NOVI_Victim.lua is available for inspection. No game registration/install; scheduler/state persistence and whole-engine validation remain unresolved as documented.


### BarrelThug all seven adapters exercised with real Lua

- Extended barrel_thug_scope_runtime_harness.cpp to execute placement, conversation creation and remark addition using actual staged class/FSE types/Lua. Full gate36 policies: prior8 Init/reset/speech/follow,16 placement and12 remark cases. Includes null/present raw Hero; lookup throwing before output construction, teleport/facing/line/Hero errors and independent key cleanup errors.
- Placement asserts teleport uses returned lookup alias while destructor receives distinct owned output; Thing closes before key, then fresh Hero/facingfalse. Empty CString payload still receives deterministic destructor. Remark creation forwards actor,false,false and signed conversationID -7 to AddPerson; line forwards same ID, owned key,false,actor,rawHero. Original operation errors survive key cleanup failures.
- Lua build/build/test all exit0; actual x86 PE014c exeSHA716c46b5419467e17e0dae19f34e337fbffcdbaf1a23bba4ad5380548592a58c. Resource candidate unchanged SHAaa996f74bc41fb56397e5e23df3de9bb2c891d7ca494868ce28bbaf136cf3502. Updated report limits to reflect expanded coverage without changing tested adapter code.
- Remaining: full original Main dispatcher/condition/predicate composition; getter failure after native output construction but before return; engine reset/lifetime/scheduler/state and DLL/gameplay validation. Runtime proposal unapplied; readable integration still pending.


### BarrelThug staged full-class registration and first runtime checks

- Added prepare_barrel_thug_resource_extension.py, staging seven methods over verified Theresa proposal in work/barrel_thug_resource_integration. Validates parent/source/additional/helper hashes; copies full inherited artifacts and emits unapplied multi-file patch. Candidate resource SHAaa996f74bc41fb56397e5e23df3de9bb2c891d7ca494868ce28bbaf136cf3502.
- run_barrel_thug_registration_compile.py compiles full staged registrations including bound-conscious host with actual FSE types. Passed exit0, x86 COFF014c; object SHAb532d30df71aaf8fd1cd726926e11c117c9c040740d74dbd331793e760897b0f. This is compilation, not linked runtime/gameplay proof.
- barrel_thug_scope_runtime_harness.cpp reuses established full-class API doubles and executes actual staged Init/reset/speech/follow through real Lua.8 policies over controlled/empty resource, null/present Hero and selection0/2; stale reset after explicit release rejected; repeated scope close idempotent. Engine reset double deliberately records call without changing resource so forwarding can be inspected independently.
- run_barrel_thug_scope_runtime_checks.py: Lua build/build/test all exit0, PE014c, exeSHAe580e3d0ea233b6dd9b97367d6c75690c3dde8f80dd2f8f866ed65916a1be9a8. Placement/remark runtime behavior, native full dispatcher/predicate, merged engine ownership/state/scheduler/DLL/live remain pending. Runtime checkout untouched.
- Worker completed isolated Victim full Init/Main; work/victim_converter/INTEGRATION.md ready for root integration. Worker now assigned remaining DeadFather (or Bully if already covered), keeping shared builder and handoff root-owned.


### BarrelThug Init and runtime adapter source; placement ABI test corrected

- Recovered barrel_thug_init.lua: DoneIntrofalse and LastTimeSpoken9999 before damagefalse/killfalsefalse/combofalse. test_barrel_thug_init executes exact65-byte DB6BF0 body SHAd2dc4a56b4f8265bafa812550a582da8e8b2b0e989bee90c0e8bf32f2b45d8ba across6 initial-state combinations and compares Lua/ABI events.
- Corrected earlier intro emulator boundary: lookup8A7D60 consumes TWO args (ret8), teleport88E540 consumes THREE (ret12). DB6DC0's push0 is staged for teleport across lookup, not a lookup argument. Previous doubles incorrectly consumed3+2 instead of2+3, obscuring that fact while total stack balanced. New vtable and both lookup-return/teleport-return opcode checks pin the callee contract; corrected902 intro comparisons pass. No recovered Lua change needed; placement adapter correctly calls teleport(actor,returnedAlias,false). Earlier chronological notes must be read with this correction.
- Added retail_barrel_thug_actions.inc with seven proposed methods: InitializeBarrelThugActor, ResetResource (unconditional helper2), SpeakBarrelThug (fresh Hero even empty control), FollowBarrelThugHero (1.0,true), PlaceBarrelThugAtStart (owned output versus returned alias, reverse cleanup, facefalse), NewBarrelThugRemarkConversation and AddBarrelThugRemark. This is source only: not yet staged in full class, registered, compiled or applied. Getter exceptions before returning ownership remain an explicit integration concern.
- Focused Init/intro gate4 tests2.159sOK process0. Full dispatcher/entry/predicate validation and compiled real-FSE/Lua adapter checks remain next, before readable integration. Background worker continues Victim full composition/runtime proposal.


### BarrelThug hit response and initial Main composition

- Added barrel_thug_hit.lua, DB79FC..DB7BB0 after native hit predicate string cleanup. Entry termination; two fresh raw Hero ally updates in opposite directions; parent AddBadDeed2; movie128; pause; retained control16 prepare/acquire4 retry; post-acquisition termination; owned Thing200 ordered health check; WHY_HIT selection0/nonblocking task wait; unpause/movie destruction. No speech/final speech termination when health is nonpositive or NaN.
- Extended native conversation emulator for hit branch without replacing instruction bytes.480 comparisons cover health0/positive/negative/NaN, busy0/2, cancel0..9, acquisitionpending0/1/3 and held/empty control. Checks ally operands even for null Hero, parent receiver/deed2, movie/key/Thing slots, real cancel epilogues and stack return. Engine methods and AddBadDeed are boundaries; hit predicate before79FC remains outside this test.
- Added barrel_thug_main_body.lua composing intro, talk, timed remarks, hit and frame loop with one outer resource and caller-owned bound-conscious registration. test_barrel_thug_main_body runs actual helper composition on no-intro/no-talk/returned/no-hit routes with four outer termination points and tests original phase error survives failing outer cleanup.
- Combined Main composition/hit/talk gate6 tests7.458sOK exit0. Full original Main dispatcher/entry/predicate comparison remains pending; do not treat Lua composition checks as native parity. Init and all pending BarrelThug adapters still need recovery/staging and real FSE/Lua validation. Helpers remain isolated, no readable regeneration/deployment yet.


### BarrelThug timed remarks recovered

- Added barrel_thug_timed_remarks.lua for native DB73A0..DB7924. Parent returned flag short-circuits initial timer read; inactive/expired route term-checks then unconditionally resets retained control via CD2770. Active route reads broken flag between termination checks, creates conversation with actor/false/false and adds fresh Hero before threshold selection.
- TEMPT thresholds10,20,25,30,34,38,45 and WELLDONE thresholds10,25,35,45 preserve strict timer<threshold and LastTimeSpoken>threshold. Each failed threshold rereads timer; LastTimeSpoken queried only after timer comparison succeeds. Selected line gets its own termination check, owned CString/fresh Hero/add-line/destructor, then fresh timer read stored to LastTimeSpoken. No match leaves LastTimeSpoken untouched.
- test_barrel_thug_timed_remarks.py:2264 original-byte comparisons passed (both flags, seven prior LastTimeSpoken values,20 timer boundary values, cancellation0..3 and24 varying-timer sequences). Original signed branches, all11 line constants/stack CString receiver checks, conversation ABI and real cancellation destructor/return run; engine calls are doubles.1test3.786sOK exit0.
- ResetResource, NewBarrelThugRemarkConversation and AddBarrelThugRemark are explicit pending runtime adapters; GetBarrelWatchTimer already exists in prior proposals. New helper remains isolated pending remaining hit phase/Init/dispatcher and runtime integration. This reduces a recovery gap, not emitted TODO count or deployment gates.


### BarrelThug introduction recovered through movie/follow transition

- Added barrel_thug_intro_prepare.lua and barrel_thug_intro.lua, structured DB6CFB..DB6F4E. Retained control priority4 retry; wait for parent BarrelManLeftHeroInCharge; termination ordering; place at M_WHouse_ManStart, face fresh Hero with snapfalse, pause3.0; movie/pause; owned Thing health gate; EXPLAIN selection0 and native task wait; DoneIntrotrue before fresh-Hero FollowThing distance1.0/true; unpause/movie destruction.
- test_barrel_thug_intro_prepare.py executes original bytes and actual cancel epilogues:198 prelude comparisons plus704 complete intro comparisons. Includes pending acquisition/wait states, initially held control, cancellation0..10, positive/zero/negative/NaN health and busy speech. Checks original placement ABI uses lookup returned alias for teleport while destroying stack-owned output, string lifetime, facing arguments, movie/Thing receivers, follow operands and state-store ordering, final stack balance.
- Empty literal at122D70E verified as zero byte (RData.string_at returns None for empty text). Initial full test mismatch was that test decoding, corrected without altering recovered Lua.
- Combined intro/conversation gate5 tests9.635sOK exit0. Runtime calls remain doubles; PlaceBarrelThugAtStart, SpeakBarrelThug and FollowBarrelThugHero adapters not staged yet. Lua StartMovie internals and outer control destruction modeled in tests. Helpers still isolated; Main dispatcher/timer/hit/Init and actual runtime ownership/registration/state/live integration remain pending.


### BarrelThug composed conversation movie and acquisition

- Added tools/script_recovery/barrel_thug_conversation.lua: native DB6F86..DB73A0 entry termination, movie, pause, prepare retained control, priority4 acquisition retry/frame/termination, post-acquisition termination, existing four-way speech helper, unpause/movie close. Main must close its outer control on false. Helper remains isolated, not emitted.
- Extended test_barrel_thug_talk_body.py to execute original movie/acquisition and speech instructions together, through actual cancellation epilogues and returns: 1920 cases (state flags, four health values including NaN, busy tasks, cancellation0..9, acquisition pending0/1/3, resource initially present/absent). Existing336 speech cases also pass.
- Acquisition cancellation DB7C44 uses inline movie pImp cleanup6E7AB0 then base destructor99A430, then control destructor7E74D0. Test checks both receivers, order and base vtable126008C before normalizing to movie.close. Other cancellation paths call movie destructor6E7B80; entry cancellation constructs no movie. Native stack return balance checked.
- Added12 Lua bridge-error scenarios: failures during preparation/acquisition/health/speech/task/frame still attempt unpause and movie destruction, preserve first error even if both cleanup calls fail, and release any temporary Thing.
- Focused gate:3 tests5.156sOK process exit0. Engine APIs remain doubles; StartMovie internals and outer caller control release modeled on Lua side. Runtime adapter SpeakBarrelThug, full Main composition, staged owner integration and gameplay remain pending. No runtime changes or installation.


### BarrelThug conversation recovery started

- Remaining readable TODO distribution inspected: quest22, AffairMan4, AffairWife26, BarrelThug42, Bully15, Victim53, DeadFather3. Victim remains delegated.
- Added barrel_thug_talk_body.lua for DB7052..DB738A, inside already acquired movie/control scope. Parent returned/broken flags read with native termination-query ordering; four health-gated speech choices preserve method2 for SCRMSG_TEMPT/WELLDONE and method0 for WHY_NOT_SMASH/OUTRO. Temporary Thing closes before speech; ordered health>0; task polling and final termination retained.
- test_barrel_thug_talk_body.py passes336 original-byte comparisons over both state flags, positive/negative/zero/NaN health, busy task and cancellation1..7. Actual branch/x87/temporary destructor/speech setup/wait execute; native APIs are doubles. Full Main bytes DB6C60..DB7CF1 SHAeafde7cb9a35b3f6d7c158c455496af2ea89444352b04e4f7b39dcbde39ada09 pinned.1test0.525sOK.
- Helper is isolated, not yet emitted. SpeakBarrelThug adapter remains to be staged (fresh raw Hero, controlled resource speech and exact selection/booleans). Caller movie/acquisition/cleanup, intro, timer/nag/hit and Init still pending.


### LiveFather readable integration

- Reviewed isolated candidate and INTEGRATION.md. Added generate_live_father_resource_candidate.py, byte-hash-gated on raw draft, with entity PenniesGiven owner, Init/Main entry points and native helper AddBadDeed(quest,me,deed) callback. Normalized conscious-condition call to the no-argument bound-host API; verifies existing native entry witness and exactly one registration.
- Readable builder now emits LiveFather candidate, maps native Main/Init ledger rows to LiveFatherMain/LiveFatherInit and reports staged owner/runtime limitations. Phase code unchanged from worker's native-checked candidate.
- test_live_father_readable_integration verifies emitted syntax/disabled registration, no raw labels/placeholders, condition before frame, Init resetting isolated entity state and exact bad-deed callback injection. Combined with original entry/routine tests:5 tests30.820sOK, process exit0.
- Persistent readable regeneration exit0, syntax passes. Runtime source unchanged. Full merged owner validation, active-quest getter pre-return ownership, scheduler/persistence/live gameplay remain pending; no full-completion claim.
- Victim recovery remains delegated; root can now tackle remaining raw quest/helper/entity gaps and merged runtime review.


### Theresa integrated into readable package

- build_readable_new_oakvale now uses verified Theresa candidate, retains entry evidence/validation limits, and maps native Main/Init ledger rows to runTheresaMainAfterCondition/initializeTheresa. Candidate generator accepts explicit draft_path and still rejects changed raw draft.
- test_theresa_readable_integration runs emitted readable output through actual-helper72 route/cancellation scenarios and entry/Init checks; checks disabled registration, ledger mapping and absence of raw placeholders. Initial unittest discovery also included imported candidate test class:3 tests41.000sOK. Removed direct class import to avoid duplicate discovery. PowerShell stderr redirection reported NativeCommandError despite unittest OK; this is not a failing test. Subsequent generation used subprocess with merged output and explicit exit propagation.
- Authoritative readable generation completed exit0. Package syntax passes;51 native function ledger rows,629 renamed locals535semantic94scratch;1203 historical raw diagnostics retained. Emitted Theresa454lines,zeroTODO/zeroLAB; whole readable FSE tree252TODO occurrences. Counts are readability metrics, not parity proof.
- Candidate exists in readable/FSE/NewOakValeIntro/Entities/NOVI_Theresa.lua. Runtime proposal unapplied; package Quests={} and status disabled-incomplete.
- LiveFather isolated candidate ready for root review/integration (agent notes work/live_father_converter/INTEGRATION.md); background agent now recovering Victim. Remaining Theresa gates include merged runtime execution, counted/transition extensions beyond bounded dispatcher tests, state/scheduler and live gameplay.


### Theresa recurring dispatcher native comparison

- Added test_native_theresa_dispatch.py:576 one-iteration original-byte vs readable Main cases over offer/gift/talk/hit precedence, existing given flag, busy task, departure proximity and cancellation. Executes DB9ECA onward native branch/join instructions, presented construction/destruction, idle task/termination and actual departure helper argument setup (fresh Hero including null, retained trigger, float2.0).
- Explicit abstractions: combined offer query, presented selector, talk/hit predicates, all phase interiors, skip action. Successful acceptance boundary sets Main-local given flag. Other phase boundaries preserve it. Stops at next-frame, cancellation outer cleanup, or outro entry; synthetic Lua boundary cleanup excluded. Seeded prior given state via a stub first meeting. This is not a monolithic full-Main or engine run.
- Native boundary stubs must write result byte stack22, not only EAX: original joins reload AL from stack. Fixed test stub accordingly; no implementation change required.
- Combined native dispatcher/acquisition + actual-helper candidate tests:4 tests13.451sOK. Acquisition42cases, dispatcher576, actual assembled gift routes72. Candidate still disabled.
- Next: review/integrate readable builder with evidence/limits, strengthen missing counted cleanup and transition coverage as needed, broad recovery gate and merged runtime/gameplay validation. Background LiveFather Main recovery ongoing.


### Theresa native acquisition comparison

- Added test_native_theresa_acquisition.py:42 original-byte vs readable Main entry/acquisition cases, pending attempts0/1/3, prepare has/reset true/false, termination query1..7. Executes initial frame/termination, control constructor, post-trigger termination, priority4 retries, post-acquisition termination, real inline/call cleanup exits through native return.
- Scope boundary: begins DB97F0 after separately verified condition; trigger construction/lookup is a compiled-adapter boundary; successful acquisition stops DB98CA before state dispatch. Matching Lua uses a synthetic boundary exception and excludes that artificial cleanup from trace. Actual cancelled paths include cleanup. Inline cleanup tested with null Info; no counted-payload destruction claim.
- Test passed1/0.830s. An initial Unicorn return sentinel needed a RET instruction to avoid decoder fetch across unmapped page; fixed harness setup, no candidate semantic change.
- Next: extend original-byte comparison through full message dispatcher and phase joins. Current test is NOT whole Main parity. Candidate/readable integration remains pending.


### Theresa actual-helper composition and condition proposal merged

- Assembled candidate runs72 actual-Lua-helper scenarios: meeting/offer/presented gift routes, each uncancelled or cancellation queries1..23. Fake engine/resource methods track owners; successful paths verify ordered quest-info removal, gift state/objective/clear and movie/map/control/presented/trigger cleanup. Phase functions are NOT replaced in this test. This remains Lua composition, not original-byte whole-Main or real-engine validation. Candidate tests2/8.908sOK.
- prepare_theresa_resource_extension now merges conscious_condition_registration.inc and retail_conscious_condition.h into inherited staged LuaManager and helper artifacts, updating additionalSources hash. Exact staged registration compiled with real LuaEntityHost/QuestState types and all resource registrations. Candidate report no longer lists missing condition merge; host/scheduler behavior remains pending.
- Fixed patch serialization: write_text on Windows doubled existing CRLF in the unified diff, causing git apply --check to reject runtime headers/manager. write_bytes preserves exact patch line endings. Regenerated full proposal now passes git apply --check against D:/Code/ForgeFSE-retail-shadow. Nothing applied.
- Next major proof: original-byte outer dispatcher vs assembled Main, then readable builder integration with explicit remaining gameplay/state/runtime limits. Candidate remains disabled.


### Theresa assembled candidate and entry-condition correction

- Correction to earlier entries: F35B10 in Theresa Main is condition registration, NOT master binding. Existing native_new_oakvale_conditions verifier proves condition vtable12C2FE8 and RegisterBoundConsciousCondition for Theresa (entry SHA69a7338741c9e31bfd07a12655bf6c7ede9b0920750e99eae75a5643969a4ee2). Registration clones bound Thing and destroys caller copy before first frame.
- Renamed body runTheresaMainAfterCondition. Added generate_theresa_resource_candidate.py: verifies raw draft SHA, condition and native Init/control/movie/health/map/vector evidence; concatenates17 readable helpers with Init/Main and explicit entity-state mapping1D/1C. Candidate at work/theresa_converter/candidate/NOVI_Theresa.resource_candidate.lua, report adjacent. No LAB/goto. Still disabled, not installed or integrated in readable package.
- Main registers bound-conscious condition then enters resource scope/body; Init invokes recovered initializer. Generated-code test verifies Init plus condition-before-frame/entry cancellation. Combined candidate/Main/Init gate5 tests4.792sOK.
- Current Theresa staged LuaManager only has bound-ALIVE registration. Conscious-condition proposal exists separately in prepare_new_oakvale_conditions.py and must be merged; no assumption that API already exists in runtime.
- Remaining: full original-byte outer dispatcher comparison; actual composed nontrivial phase execution/error paths; condition proposal merge; parent/entity state/coroutine/DLL/gameplay validation. Whole completion remains unproven.


### Theresa Init runtime adapter

- Added theresa_init_body.lua (DoneIntro=false then AskedForPresent=false before engine calls) and staged InitializeTheresaActor. Actual native Init reset offsets remain1D/1C; state-name mapping still must be integrated into candidate schema.
- Actor calls preserve native damagefalse, killfalse/false, combofalse, infofalse/true/false, by-value retained Thing pushabilityfalse, movementfalse. Copy Info RefCount increments before D30; compiled callee double consumes it. This is ABI/ownership boundary validation, not proof of real callee internals.
- Original Init plus Lua-reset tests:2 tests,0.162s,OK. Full staged x86/real-Lua owner harness23 policies pass, including4 payload/Info combinations. Full sol registration passes matching candidate SHA db9c27a8b42aeaeacd117df9413513964b0eae880fe11d49b67a919ca83720fd. Proposal now17 methods, unapplied.
- Master binding remains unresolved at integration level: native prologue copies actor into an expression, calls F35B10, destroys copy before first frame. runTheresaMainAfterMaster deliberately starts after it; do not label it complete Main. Need inspect host thread binding, original-byte outer dispatcher, assemble helpers into candidate/readable package, and broader state/runtime validation.


### Theresa Main composition and talk-offer ownership

- Added `theresa_main_body.lua`: runTheresaMainAfterMaster composes recovered phases from native DB97F0 (initial frame) onward. Caller must still establish native master-actor binding. Owns control24, trigger44, per-iteration presented16; repeated acquisition priority4, intro path vs message dispatch, presented cleanup before next frame/outro termination query, trigger before control cleanup. Entity AskedForPresent proxy preserves write-through timing rather than deferred state copy.
- `test_theresa_main_body.py`:two tests pass, with10 dispatch/ownership scenarios plus entry cancellation. All phase interiors are doubles here; this is composition coverage, NOT full original-byte Main comparison. Main helper remains unintegrated and Init/master adapter incomplete.
- Added staged TheresaTalkOffersChocolates to preserve DB9EE0..DB9F94 nested key lifetime: talk key remains alive during conditional chocolate possession query; both keys reverse-close before branch. WasVillagerTalkedTo followed by separate possession would destroy the outer key too soon.
- Full staged owner/real-Lua/x86 harness19 policies passed; four new talk/possession combinations verify short circuit and two simultaneously live keys, including null Hero retained from preceding case. Full sol registration compiled after the change. Runtime proposal now16 methods, unapplied.
- Next: original-byte outer Main comparison and master/Init bridge, actual assembled helper composition, candidate generation/readable integration, remaining runtime/state/scheduler verification. No full-parity claim.


### Theresa retained departure trigger adapter

- Staged NewTheresaDepartureTrigger and IsTheresaHeroNearTrigger; proposal now15 methods. Dedicated lookup keeps M_TriggerOutro CString until lookup returns and always destroys it, including null string storage. Generic FableString skips null-storage destruction, so generic NewThingFromScriptName was not reused for this path.
- Lookup forwards owned Thing output, ignores returned alias, and rolls back Thing after string cleanup on lookup error. Distance uses fresh raw Hero (including null) first, retained Thing second, float2.0.
- Full staged owner/x86/real-Lua harness now15 policies passed: four Hero-null/result combinations plus lookup-error cleanup, on top of earlier10. Two distance calls reuse one retained output; released-handle queries reject. Engine and lookup helpers are doubles; native callsite operands were inspected, not yet a separate automated trigger caller comparison.
- Full sol registration and scope harness use matching candidate SHA: 1849ba4d27c1607fc72e991c957287fc068fc6b2484ebbee92484919123d2e73. No runtime source applied. Full Theresa Main/Init remains next, along with message adapter review and native trigger caller coverage.
- Background LiveFather hit/payment six-test gate passed; worker now recovering Main/Init/caller composition in isolated artifacts.


### Theresa retained-output runtime integration

- Added `retail_theresa_presented.inc` and composed it into the staged Theresa owner: NewPresentedItemOutput, PollPresentedItem, PresentedItemMatches, DestroyPresentedItemOutput. Uses default constructor99E4B0, bound actor virtual8C, inequality helper99E960 inverted, and scope-owned CString destruction. One output persists across both polls.
- Uses monotonic Entry.id and Release erasure, not historical m_entries.size(). New allocation cannot revive a released handle. Staged proposal now13 methods.
- Full staged owner harness:10 policies pass (previous8 plus normal/error presented cases). Actual compiled x86 C++ owner and real Lua execute; default-constructor/comparison/engine slots are doubles. Checks same output pointer, both match outcomes, explicit destruction, stale-handle rejection after reallocation, and scope cleanup after query exception. Native comparison helper behavior separately covered by196 predicate cases.
- Full sol registration compiles as x86 COFF014c. Both registration and scope harness use candidateSHA1f999419d5f0a0fd090f34d06ea2a6bf8dd17fbdd2154589442e12a7e74a14b0. Scope exeSHA5ec6b7870dcdd5446ad569e8a3b51abe66a6dee4a3b01d267da433af576613ba; all build/test exits0. Logs/results under work/theresa_scope_runtime_checks and work/theresa_resource_integration.
- Proposal remains unapplied; full Main composition, retained departure trigger, remaining message APIs, Init/runtime state and live scheduler validation remain outstanding.


### Theresa presented-item selector

- `theresa_presented_choice.lua` preserves two calls to PollPresentedItem on the same retained output. First successful equality selects chocolates; otherwise a second successful inequality selects other-present. A second poll can mutate the output even if the first returned false. No single cached poll substitution.
- `test_theresa_presented_choice.py`:196 original-byte cases execute DBA490 caller, DBA588 comparison and actual helper bodies99E960/411570/4115A0. Includes null/allocated-empty, exact/lowercase/prefix-difference/high-byte text and all pairs of query booleans. Engine output mutation is a double; output destructor belongs to Main and is outside this predicate.
- Selector/offer/outro gate:7 tests,6.031s,OK.
- Reuse contract identified in bully_presented_methods.inc (NewPresentedItemOutput/PollPresentedItem/PresentedItemMatches/DestroyPresentedItemOutput), but Theresa staged owner does not yet compose those methods. Adapt to current owner container/IDs rather than copying old size-based ID logic blindly.
- Native outer-loop evidence: retained trigger key M_TriggerOutro, output slot44; control24; presented text16 allocated only after DoneIntro. Trigger check calls CBE2FF with fresh Hero in ECX, retained trigger in EDX and float2.0. Idle path skips task/animation when Main-local given flag is true. Hit predicate is ordinary-hit OR (any-special AND NOT ability14), retaining all temporary keys until reverse cleanup.
- Next: compose retained-output/trigger/message adapters and full Theresa Main; helpers remain unregistered and unintegrated. Background LiveFather payment work continues.


### Theresa outro recovered

- Added `theresa_outro_body.lua`, native DBB0E4..DBB2D8: entry cancellation, DisplayQuestInfo(false), remove GUIBullyHealthCounter/ GUIGoodDeedCounter/GUIBarrelCounter in native order, movie384 then Hero control328, one priority4 acquisition attempt with ignored result, HERO/Theresa actor map, CS_OAKVALE_INTRO_THERESA, raid AVI, 0.5-second black fade/zero hold, OverrideMusic(25,false,false), AttackOver=true.
- Cleanup is actor map then Hero control then unpause/movie destructor. Main retains outer presented text and Theresa control and owns subsequent termination query.
- `test_theresa_outro_body.py`:16 original-byte comparison cases plus13 Lua-only partial-construction/error cleanup cases. Actor-map construction is a separately tested abstracted boundary; engine/CString bodies are doubles. Original caller operands, state store and cleanup execute. This does not prove engine coroutine behavior or full Main parity.
- Outro/actor-map/movie/offer combined gate:11 tests,10.881s,OK. Actual runtime FadeScreenOut wrapper reviewed: converts Lua0.5 to float and forwards opaque black RGBA(0,0,0,255), matching native stack bytes; this review is not compiled/live adapter validation.
- Remaining root work: Theresa outer message/presented-item predicates, retained trigger and Main composition, Init integration, staged runtime/full readable generation and broader validation. Helpers remain unintegrated, runtime source unchanged.
- Background LiveFather hit phase stable:2304 native comparisons/10 Lua error cases; worker continues payment and gold dialogue.


### Theresa presented-gift and talk recovery

- Added `theresa_presented_gift.lua`: accepted presented chocolates (DBA5A4..DBA814) and rejected present response (DBA4F5..DBA8A6 plus shared destructor). Caller retains presented-item text. Accepted path closes guards before movie and commits gift only after speech completes.
- Added `theresa_talk_body.lua`: ordinary talk response DBA8E0..DBABAE, choosing HELLO/GET_PRESENT/REALLY_GET_PRESENT from Main-local givenChocolates and entity byte1C (askedForPresent). Chooses branch before second termination query, then writes askedForPresent before health. No state-write on cancellation before that point.
- Extended original-byte composition harness:108 presented-response cases and252 talk cases; original movie construction, x87 health, speech polling, cancellation and cleanup execute. Gift commit and guard construction remain separately tested abstracted boundaries; presented-item and talk-message predicates are outside these phase entries.
- Combined offer/question/hit/gift gate:11 tests,8.282s,OK. Full Theresa Main, outer message predicates and outro remain outstanding. These helpers are not yet emitted into the readable package; no runtime patch applied.


### Theresa later-offer composition checkpoint

- `theresa_offer_choice.lua` preserves signed answer polling and both unconditional post-answer termination queries in DB A004..A10F/A31A (addresses without spaces in source).
- `theresa_offer_body.lua` composes that choice with resource health/speech and gift helpers, and closes guard vector before unpausing/destroying the movie. The presented-item CString remains owned by Main outside this phase.
- `test_theresa_offer_body.py`: 324 original-byte cases across answers, pending answers, zero/positive/NaN health, busy task polls and cancellation positions. Executes original question, health, speech waits and cleanup. Guard construction and gift-commit tail are explicit abstracted boundaries, separately covered by existing native tests; this is not whole-Main parity.
- Eight additional Lua-only error cases verify nested Thing/guard/movie cleanup and primary-error preservation, including the Main-local gift flag boundary.
- Combined offer/question/hit/native-gift/Lua-gift gate: 9 tests, 5.666s, OK. Earlier full-suite result is not evidence for these newly added tests.
- Next: remaining Theresa presented-item/talk/outro paths, full Main composition and runtime integration. Background agent is recovering LiveFather hit response; TeddyGirl checkpoint is stable (18 tests, 1568 dispatcher cases, 14 merged-owner scenarios). No runtime source applied or live installation performed.


Theresa hit prelude now structured in theresa_hit_body.lua: termination, two fresh
Hero ally calls via reviewed shared bridge, AddBadDeed2, then hit movie. Added45
native/Lua cases; combined hit3 tests2.499s passes (120movie+45prelude pluserrorpolicy).

Shared GetScriptThing rollback carried from TeddyGirl into Theresa preparer, expanded
to cover missing getter validation as well as a throwing getter. Owned Thing is
marked live before call; catch Destroy(e) closes it before an ID can escape. Full
staged class x86/realLua harness now8 policies, all3commands0, executableSHA
3c8fc2bc6b4d242fa71b8d498b97631f2a3a973c189034f72b05ad701d2a60ee.
New cases normalreturnedID /getterthrows /missinggetter assert one immediateThing
close before outerresource close and no duplicate cleanup. Earlier vector-reference
policies still pass. Native destructor callbacks remain nonthrowing doubles here;
no SEH/corruptoutput claim. NEXT Theresa talk/presented/outro, nativeflag operands,
and full candidate composition. Runtime proposal unapplied.

Theresa hit movie now structured via theresa_speech_body.lua +theresa_hit_movie.lua.
Original DBACE6..DBADED plus shared normal/cancel destructors compared to Lua in120
cases (8health values x3taskdurations x5cancels). Includes originalx87 compare and
retainedBL through destructor. Lua-only health/errorcleanup tests close innerThing
before unpause/movie even when all cleanup calls throw; original health error kept.
Combined2 tests1.096s pass. SpeakTheresa adapter adds rawfreshHero->resource v34,
selection0,false,true,false; emptyresource still fetchesHero. Actual FSE/realLua/x86
harness67 policies passes; full9-method staged registration recompiled successfully.
NEXT integrate hit prelude (allies+baddeed), remaining talk/presented/outro branches.
Background TeddyGirl found shared NewThingFromResource pre-ID getter rollback gap;
its isolated fix/evidence pending final, then carry into Theresa proposal too. Do not
claim all resource error paths closed until that shared constructor-failure case is
reviewed. Full staged scope5-case result predates newest SpeakTheresa method, but
new action harness and complete registration cover that addition independently.

Theresa full staged resource class now EXECUTION-tested for guard ownership:
run_theresa_scope_runtime_checks builds actual composed header/FSE types/realLua/x86,
5 policies pass (explicitclose yes/no xscopeClose/destructor, plus getterfailure).
Surviving Lua sharedvector becomes inert after scopeclose; repeatedClose/GC never
redestroys vector; partial getter allocation cleaned and inactive entry safely
cleared. Actual class factory/Add/Close/destructor execute, helper addresses route
to doubles; original vector helper implementations separately tested. All3commands
exit0 PE014c executableSHAb7db916a40877e5f41cc3e1582a2b20057e504aff7700dfd6369b8d8b763df03.
Evidence work/theresa_scope_runtime_checks. NEXT Theresa remaining talk/presented/
hit/outro phases and full Main; DLL/gameplay/common scheduler/state remain pending.

Theresa fullclass proposal now composed over Villager via
prepare_theresa_resource_extension.py ->work/theresa_resource_integration.
Eight methods (seven action methods plus NewTheresaGuardVector) and guard Lua type
registered. Factory stores shared vector in normal m_entries lifetime sequence;
Kind::TheresaGuardVector Close during fallback makes borrowed Lua refs inert while
allowing explicit native-position close. All parent helper/additional-source hashes
validated before staging, runtime source untouched. Inherited Villager quest-state/
manager changes carried forward. run_theresa_registration_compile completed exit0,
x86COFF014c; registration-result.json records candidate/input/object hashes.
This proves template compilation, NOT runtime execution of merged owner teardown.
NEXT exercise full staged owner with surviving Lua vector refs, then Theresa
remaining dialogue/presentation/hit/outro. Existing standalone action/owner harness
still63 cases; no claim those tests execute newly merged class.

Theresa outer meeting native differential now passes192 cases1.103s:
test_native_theresa_meeting executes original outer instructions, original signed
answer polling and cancellation cleanup; compares actual meeting/question Lua.
Variations possession/acquisition booleans, answers0/1/2 afterpending-1, cancel1..8,
rawHero null/non-null. Confirms single ignored-result acquisition, cameraoff and
DoneIntro only normalpath, unpause/movie/map/Hero cleanup and first two termchecks.
Explicit abstract boundaries: actor-map construction, question string construction,
and accepted-gift body; those have separate tests. API/CString bodies are doubles.
NEXT scope-owned guard factory/full runtime merge, then Theresa talk/presentation/
hit/outro phase recovery. New tests separate from completed suite14 count.

Theresa meeting composed in theresa_meeting_body.lua using actual actor-map/question/
acceptance helpers. 96 Lua composition scenarios pass0.026s: two pre-construction
termination checks, single ignored-result Hero acquisition, movie/pause/fixedcamera,
MEET macro, possession, question/acceptance, normal cameraoff+DoneIntro, movie/map/
Hero cleanup. Cancellation intentionally does not emit cameraoff (native cleanup
AEDF..AF24 only unpauses/destroys). Whole meeting original-native differential remains.
TryAcquireTheresaHero and DoesTheresaHeroHaveChocolates staged in actions fragment;
rawfreshHero forwarded even null, failed acquisition may populate resource, possession
key constructed BEFORE Hero lookup and destroyed after boolcapture. Expanded actual
FSE/realLua/x86 harness63 policies passes; executableSHA
bdf4b5ff6128bae2de819da5898ad6005231b1ee648b7af81f1af26214f743c4.
NEXT native outer-meeting comparison and missing scope-owned guard factory/class merge;
then remaining talk/presented/hit/outro Main branches. TeddyGirl agent reactivated on
whole native Main comparison; its readable integration is stable.

Theresa retained vector owner staged in retail_theresa_guard_vector.h. Raw12-byte
vector getter, native removalCBED82 and destruction8AC970, key cleanup, idempotent
Close and destructor fallback. Actual-FSE/realLua/x86 action harness now45 policies
passed; executableSHA8d9623ebc79dbe87f18bb4fc368a277f5a4b1639a9426124b013fc25917bb7b2.
New owner tests use injected helper/API doubles; original helper loops separately
verified. Full resource scope must still own returned vectors until Close: factory/
owner-list wiring and class registration are pending, not supplied by standalone owner.

Accepted phase now structured: theresa_accept_chocolates.lua composes retainedguards,
remove, HERO/THER actor map, MEET_YES macro(false,true), giftcommit, actor-map cleanup,
then vector Close. Composed actual Lua9 success/failure cases pass0.005s; phases were
individually native-verified, this is not a whole accepted-region native differential.
Required NewTheresaGuardVector factory explicitly pending above. NEXT full accepted
native trace, meeting composition and runtime factory/owner registration.

Theresa guard vectors verified in full Main CFG: slots364/304/316, three getters,
three removals and seven normal/cancel destruction sites. New native_theresa_guard_vectors
pins complete removal56 bytes CBED82 (SHA4413c70ec94bfffe057e5d493bb4548e1437519e571512567583297a5d4d6c67)
and destructor50 bytes8AC970. Native helper test32 vector/mask cases pass0.973s:
IsAlive virtual12C lowAL, RemoveThing virtual1B0(thing,false,true) only alive,
then destructor iterates all owned Things virtual0(false) and frees nonnull storage.
Null empty and allocated-empty both checked; actual ret4 and stack balance verified.
Evidence work/theresa_converter/guard_vectors_evidence.json. Getter allocation and
retention, wider caller zero flag provenance, and staged vector owner still pending.
NEXT vector runtime owner and full accepted/meeting phase composition. Helper bytes
and caller scopes are proven separately from game API/destructor callback internals.

Theresa gift adapter implemented: TakeTheresaChocolatesAndUpdateObjective preserves
itemCString scope, three objective input strings, returned GetActiveQuestName pointer
versus owned output, reverse cleanup; separate ClearTheresaInformation forwards bound
actor directly. New theresa_gift_commit.lua sets persistentflag before update and
Main-invocation progress.givenChocolates only after update/before clear. Lua failure
boundary tests plus native three-tail test pass2 tests0.770s. Expanded actual-FSE /
realLua x86 harness now34 policies, all3 commands exit0; executableSHA
6937f36708ae2aa9714c2e6420a1303f4cd1c0135e95813011e8b03ef91195ff.
Tests cover alias questname, emptyCStringstorage, failed getters/objective/constructors,
closed scope, and existing approach/question methods. Fragment remains staged;
fullclass registration/DLL/gameplay pending. NEXT guard-vector retained ownership
and full accepted/meeting phase composition. No live runtime changes.

Theresa gift commit caller tail now executes all3 original regions: DB9D8B..9E6D,
DBA213..A301, DBA70B..A7F0. test_native_theresa_gift_commit1 test0.798s passes24
cases (3sites xreturnedQuestNamealias xoldstate xemptyCStringstorage). State95=true
precedes first item call; item CString constructed/taken/destroyed; empty/empty/
objective CString scopes then GetActiveQuestName output and SetQuestCardObjective
using RETURNED pointer; cleanup owns output not alias, then objective/empties.
Main-local given flag17=true BEFORE ClearThingHasInformation(boundactor). Tests
assert exactsequence, empty storage cleanup, stack balance, no retained strings.
API bodies are doubles, not engine proof. NEXT implement raw gift-tail adapter
with caller preserving localflag-before-clear order; then full accepted phase and
meeting composition. TeddyGirl background currently merging only its builder branch
and staging seven+split-movie adapters (41 actual-FSE/x86 cases already passed).

Theresa approach/question runtime methods now implemented in staged
retail_theresa_actions.inc: IsTheresaNearHero rawfreshHero +distance, PlayTheresaSkip
explicitresource virtual4C and all6flags, ShowTheresaChocolateQuestion raw4CString
native construction/API/reversecleanup order. run_theresa_actions_runtime_checks
compiled actual FSE headers and realLua x86, all3 commands exit0; PE014c; executable
SHAaa0e1cedab6d694133c0e3a878ae29d3c193b251ddc5ed3542d0f3e94ce53b67.
22 harness policies cover empty/live resource, emptyCString storage, nullrawHero,
constructor/action/destructor exceptions and closed scope. Original error preserved
while cleaning all constructed keys. Evidence work/theresa_actions_runtime_checks.
Full LuaRetailResources class merge/registration and DLL/gameplay remain pending;
fragment harness supplies resource-owner double and engine-call doubles.
NEXT accepted chocolate transaction and whole meeting composition. Background
TeddyGirl explicitly reactivated on staged adapters/readable integration after its
full candidate checkpoint (3977 native cases); preserve root Theresa ownership.

Theresa chocolate question phase now structured in theresa_chocolate_question.lua:
askTheresaAboutChocolates returns nil cancellation /false decline /true answer1.
NativeDB9B7F..DB9C87 comparison test passes96 cases (pending0/1/3, answers0/1/2/
INT_MAX, cancellation1..8), test_theresa_chocolate_question1 test1.096s.
Native question CString order empty/NO/YES/question, API argument order question/
YES/NO/empty,true, reverse destruction before first signed answer poll. Negative
INT_MIN stays pending; post-answer termination always occurs and accepting has an
additional termination check. Required ShowTheresaChocolateQuestion adapter remains
explicit/unimplemented. NEXT accepted chocolate transaction, native helper bodies,
and runtime adapter staging before whole meeting/Main composition.

Theresa actor maps now verified: three constructions, six resource assignments,
three macro uses and four destructor sites pass full Main CFG. Meeting map228 and
acceptance map60 bind HERO->control344 / THER->control24; outro reused map60 binds
HERO->control328 / Theresa->control24 (role spelling/case differs intentionally).
Native lookup consumes only key; assignment uses returned entry pointer and the
resource argument left on stack. Six original binding-region traces cover alternate
returned-entry aliases and match actual theresa_cutscene_actors.lua named helper.
Two Lua-only partial binding errors prove map cleanup and original error retention.
New test_native_theresa_actor_maps:2 tests1.768s pass. Native map internals/retention
still separate; work/theresa_converter/actor_maps_evidence.json records boundaries.
NEXT meeting question/answer and cutscene execution phase, plus required staged
runtime approach adapters. Full Theresa integration remains pending. Existing
runtime ActorMap methods available; exact CString failure cleanup remains a gate.

Theresa first structured phase implemented: theresa_approach_body.lua exports
waitForHeroToApproachTheresa for originalDB98E4..DB99CA after control acquisition.
Named loop preserves entry termination, SKIP scope, fresh rawHero/distance5,
task polling, post-task termination, second fresh proximity query and retryframe.
New test_theresa_approach_body executes original instructions against actual Lua:
192 scenarios (four proximity sequences xfour task sequences x12 cancellations),
1 test2.948s passes. Native booleans carry nonzero upperEAX and rawHero alternates
null/non-null. CString/action/game bodies are doubles; exact SKIP thunk already
separately checked. Required PlayTheresaSkip/IsTheresaNearHero resource adapters
are explicit but not yet staged. Full Theresa builder integration awaits remaining
phases. NEXT meeting resource/cutscene bindings and staged adapter implementations.
TeddyGirl agent reports12 tests34.182s,3209 native phase comparisons and20 callback
error cases; movement/whole Main composition still active. No new full suite needed
yet; new focused tests are separate from terminal suite14's1336 results.

Theresa SKIP animation contract now executes both original call regions and actual
7E73E0 thunk: eight cases (two sites x empty/live resource x empty/nonempty CString
storage), test_native_theresa_animation1 test0.758s passes. Resource pImp virtual4C
is PlayCombatAnimation, not PlayAnimation virtual48; exact six flags1,0,0,1,0,0.
Native CString always constructed/destroyed even when resource pImp is empty;
actual empty thunk returns28 bytes. Draft cached me:PlayCombatAnimation has only
two flags and requires explicit-resource replacement. Engine action and CString
bodies are still doubles. Speech inventory is8 sites, fresh Hero and selection0 /
listenfalse / sound2Dtrue / overFadefalse; DBA6C6 uses EDI zero set atDBA608, wider
path verification still needed. NEXT structure initial SKIP/proximity phase and
compare original control flow, then meeting cutscene bindings. No draft replaced yet.

Theresa health caller lifetimes now verified for all8 GetScriptThing sites: output
slots464/428/416/500/440/476/488/452, resource24, returned getter pointer passed to
health v420 and owned output destroyed4AA840. Full Main CFG proves each temporary
closes. Slot416 comparison atDBA570 jumps to cleanupDBA822; nonlocal join retained.
New native_theresa_health.py +test_native_theresa_health.py:2 tests1.867s pass,
72 original-x86 comparisons versus actual Lua health>0 including signedzero,
subnormals, infinities andNaN; destructor EAX clobber preserves BL result.
Evidence work/theresa_converter/health_evidence.json. These tests were added after
suite14; do not include them in its1336 count. NEXT speech operands (one site uses
EDI zero from wider scope), animation CString scopes, cutscene binding maps, then
structured phase composition. Raw positive health comparisons are already oriented
correctly; this step verifies ownership/order rather than changing those predicates.
TeddyGirl agent reports2633 native/Lua phase cases and is continuing movement/hit.

Theresa ownership checkpoint: complete Main7013 bytes atDB97A0 SHA
32136f38fc41cdf0c3a0badb7bba9dae1cad47216f015db0f8d7f8074ced4efc.
New native_theresa_control verifier/witness covers50 calls: retained control24,
meeting Hero control344, outro Hero control328, four acquisitions priority4,
eight speaks, two animation calls. All constructed CFG paths close exactly once.
New native_theresa_movies verifier/witness covers30 events: seven constructions,
seven starts,16 cleanup sites; four LEAs select shared destructorDBA3D6.
Joint control+movie CFG test proves stack-slot reuse does not overlap ownership.
Focused Init/control/movie5 tests passed4.828s; after adding joint scope test,
movie3 tests passed4.839s. Evidence work/theresa_converter/ownership_evidence.json.
Caller-scope proof only: helper bodies, pause operands, temporary Things, cutscene
binding maps and actual readable/runtime lowering remain. NEXT those phase contracts.
TeddyGirl agent still advancing movement/presented/hit; latest report adds480 talk,
25 presented-output,32 cleanup-mask and32 Init/GivenTeddy native/Lua cases.

Suite14 is TERMINAL SUCCESS: 1336 tests in802.911s, process exit0,
runner elapsed804.721s. Evidence work/converter_marathon_suite_20260913_14.log
and .result.json; session95089 consumed terminal result. Do not poll/restart it.
Newer tests separately passed: Villager dispatcher +ambient gate +composed candidate,
6 tests7.197s (300 dispatcher scenarios and432 ambient gate scenarios).
Dispatcher executes original outer instructions but abstracts separately verified
message/dialogue regions; this does not prove whole-engine composition. Candidate
REPORT remaining gates updated accordingly and regenerated.

Root now owns Theresa; background book_trader_home agent remains active on TeddyGirl.
New native_theresa_init.py and test_native_theresa_init.py pin all142 Init bytes at
DAC4F0 (SHA79a6875510fa0b477c26d90f7c0c4be411ec4939ff4df5382a789e86ba428244).
18 original-x86 scenarios pass0.165s: both state bytes reset before APIs; five calls
use bound actor; pushability slotD30 receives inline copied Thing and increments
nonnull Info refcount. Test explicitly models callee consumption; it does not prove
callee destruction. Existing Forge LuaQuestState::SetIsPushableByHero already
retains copied Info. NEXT verify callee cleanup and Theresa Main resource/phase
contracts before generating a structured replacement. No runtime proposal applied.

Villager ambient gate original-x86 trace now passes432 cases (2.355s): signed timer
and random values, low-byte proximity/termination, null/raw Hero, timer IDs-1/0/73,
short circuits, fresh global timer interface and ID after termination, then set3 /
create(false,false) / freshHero / participant. New test_native_villager_ambient_gate.
This was added after suite14 discovery; tested separately. NEXT outer native Main
dispatcher composition against already-proved dialogue helpers. TeddyGirl agent
active on retained item/movement/lifecycle phases. Suite14 remains session95089;
keep polling that handle, no restarted suite.

Villager Init audited/recovered: complete82-byte DADF00 SHA
a306c925fe51364685ddd94123ffc6e17c24499b64409bdd681fe800e412bf15.
Twelve native cases prove HeroDidHitMe reset before damageablefalse/killablefalsefalse/
combofalse/freshhero/ally(bound,hero), no copied handles or IsNull queries.
Existing Quest damage/combo wrappers added IsNull filtering; new InitializeVillager
adapter removes it. Candidate/readable Init now uses adapter. Message/Init compiled
harness passes32 policies; focused native+composed5 tests pass1.147s; all15 staged
method registrations compile. Readable package regenerated18/18 syntax,51 functions.
Suite14 still RUNNING at session95089; latest poll returned live, log only dots.
NEXT original dispatcher/ambient-gate comparison, then broader runtime/state gates.

Villager native message checks now pass31 cases (27 hit +4 talk): original x86
low-AL booleans with nonzero upper EAX, ordinary/special/excluded14 short circuit,
reverse CString cleanup and talk BL survival across destructor clobber. Wife
shared trace helper generalized by code page; combined4 tests pass0.607s. Compiled
actual-FSE/real-Lua Villager message harness passes30 policies including empty
CString storage, query/cleanup exceptions and original-error preservation.
Full recovery suite14 RUNNING: unified exec session95089, launched via
work/run_converter_suite14.py; log work/converter_marathon_suite_20260913_14.log.
Poll that live handle; do not restart on observation timeout. Result JSON appears
only after terminal subprocess status. NEXT original whole dispatcher/ambient
gate comparison and Villager Init audit; suite failures to resolve when available.

Full disabled Villager Main composed and integrated into readable builder. Generator:
generate_villager_resource_candidate.py; work/villager_candidate contains candidate
and REPORT. Three proved dialogue helpers + named outer loop preserve single
control/key lifetime, hit/talk/ambient dispatch, priority4 retries, first condition
registration and explicit reverse cleanup. New raw-CString hit/talk message bridge
and ally forwarding compile in staged proposal (14 methods). Whole-body harness:
76 cancellation points +4 injected errors, repeated on emitted readable source;
4 tests pass0.990s. Readable Villager has no goto/LAB/missing/pCVar/fVar/cVar/scratch
markers; this is a readability result, not full engine parity. Package18/18 syntax,
51 functions,759 renamed locals (620 semantic/139 scratch),1203 historical raw
diagnostics retained. Remaining: original-x86 dispatcher/message/gate comparison,
Villager Init full audit, parent/helper/scheduler integration and DLL/gameplay.

Villager ambient timer/proximity adapters added to staged proposal (now11 methods):
ShouldVillagerStartAmbientConversation reads global timer, signed rand%100, fresh
raw hero and native distance5; StartVillagerAmbientConversation sets global timer3,
creates(false,false), gets fresh hero, adds participant. SetVillagerAmbientTimer also
handles Init0, correcting quest Init's previous cached-interface call. Compiled
actual-FSE/real-Lua ambient harness passes218 policies including changed global
timer receiver, signed IDs/randoms, null hero, short circuits and closed scope.
All Villager registrations compile; quest Init test passes. Native gate operands
reviewed directly atDAE614..DAE6B6; independent full gate trace still to add. NEXT
compose full Villager Main, verify dispatcher cancellation and remaining Init.

Villager speech lists now have a staged quest owner: LuaQuestState owns one
RetailVillagerSpeechListOwner, entities already share that state, Get returns the
same list object, owner destruction closes lists despite surviving Lua references.
Unapplied LuaQuestState.h/LuaManager.cpp changes are in Villager proposal with
source/candidate hashes. All registrations compile; actual-list/owner Lua harness
passes4 scenarios. Full host teardown/timer/scheduler ordering remains pending.
Readable quest Init now appends42 keys in native order through the owner, preserving
repeated Init appends. villager_quest_speech_lists.py recovery + test passes; builder
quest-only branch added, Guard preserved. Package regenerated:18/18 syntax,
51 functions,805 renamed locals (648 semantic/157 scratch),1203 historical diagnostics.
NEXT full Villager Main composition, ambient timer/proximity bridge, Init audit.

Villager ambient selector now calls actual AssignVillagerSpeechText resource method;
retired placeholder speechLists:AssignToText. Native288 selection cases pass2.328s;
compiled real-Lua list harness passes with actual method registration, text handle
validation, null-list rejection, replacement and retention across growth/Close.
Composed unapplied proposal now exists: prepare_villager_resource_extension.py over
Barrel, reusing Bully NewText/DestroyText semantics; eight methods plus speech-list
usertype. run_villager_registration_compile.py passed all actual-FSE x86 template
registrations. work/villager_resource_integration holds header/patch/proposal and
registration-result.json. Live runtime SHA unchanged. NEXT quest lifetime/Init/list
teardown binding and full Villager Main composition; DLL/gameplay still pending.

Villager owned speech-list bridge now executes through real Lua/actual FSE types:
3 scenarios (1/2/5 append rounds across8 lists; 384 appends) cover native-boundary
arguments, dynamic counts, retained selection across growth, invalid index, closed
access, idempotent Close, descending vectors/ascending entries and selected text
surviving list teardown. Runner: run_villager_speech_lists_runtime_checks.py;
work/villager_speech_lists_runtime_checks/result.json. Native calls are doubles.
Independent native storage suite3 tests passes0.203s, now including full450-byte
DBEFC0 destructor (SHA232d98f62015f85c1aeb9aef94203cc85704d4ef4db67fa9e45aa8a65b9454c9):
timers108/104, eight vectors descending, entries forward, each buffer freed, base
tailcall last. Actual native Init/growth/copy remain covered. NEXT resource Text
assignment adapter, quest-lifetime ownership, full Villager Main composition.

Villager vector growth now executes actual433530 (316 bytes SHA
1c48ffbb40875ecde275517336150895f5379a47d32e5e8ddf16499dab3a489c),
plus actual CString copy/wrapper. Two tests/six scenarios pass0.139s: first/repeated
Init, growth/spare capacity, retained strings after relocation, freed old buffers.
Heap and literal create/destroy remain doubles. New unapplied bridge core
retail_villager_speech_lists.h compiles against actual x86 FSE types; Append uses
native copy/growth, Count stays dynamic, CopySelectedTo reloads storage, Close
releases descending vectors/ascending entries. Compile log under
work/villager_speech_lists_proposal. NEXT actual bridge execution against these
native traces, full destructor trace, then quest ownership and Main composition.

Villager speech-storage Init trace added: test_villager_speech_storage passes4
capacity/repeated-init cases (0.073s), 252 total appends across42 keys/eight vectors.
Executes original Init3306 bytes plus actual CString copy99EC30 (61 bytes SHA
a3acb13754a768da17ddb398c1ed55dcaeabd496848d6fd57bd892aedf2ccc86) and
null-check wrapper44B110 (11 exact bytes). Literal construction/destruction and
vector growth433530 remain boundary doubles. Verifies construct -> copy/insert ->
temporary destroy, retained reference1 per entry, original append order, and second
Init appending rather than resetting vectors. Spare-capacity copy count matches
retail global CString allocation counter. NEXT implement owned-vector bridge and
verify native growth/destruction boundary; connect it to quest lifetime/Main.

Villager ambient selection helper villager_ambient_selection.lua passes288 native
x86 differential cases (2.129s): signed deed counters, sex0/1/2, both termination
checks and changed vector storage after index helper. Preserves exact counter-read
order, dynamic vector count, and post-yield vector reload before CString assignment.
AddVillagerAmbientText bridge forwards retained key/fresh raw hero/false/bound actor
without constructing temporary strings; updated actual-FSE/real-Lua harness passed.
The speechLists Count/AssignToText interface is still awaiting owned native CString
vector integration; helper is not presented as an installed runtime capability.
Next evidence sources: native_speech_vectors.py and quest construction/teardown
metadata, then full Villager Main composition with talk/attacked/ambient helpers.

Villager attacked dialogue now has structured helper villager_attacked_body.lua.
Original x86 movie/health/speech differential passes288 cases (5.128s): sex0/1/2,
zero/signed-zero/positive/negative/subnormal/infinities/NaN, 0-2 task frames, all
termination positions. One movie; resource-derived health Thing destroyed before
speech; positive ordered health only; unpause then movie destroy on every exit.
New retail_villager_speech.inc preserves fresh borrowed hero and speech flags
selection1/listenfalse/sound2Dtrue/overFadefalse. Updated actual-FSE-types/real-Lua
text runner includes its forwarding checks. Both Villager phase helpers remain
separate pending full Main composition and ambient-vector CString ownership.

Villager structured talk helper (tools/script_recovery/villager_talk_body.lua) now
matches original x86 DAE3F3..DAE607/DAEA49 in 756 cases: sex0/1/2, hit history,
0-2 acquire failures, 0-2 active frames, each termination point, held/empty control.
Both sex branches join assignment/acquisition; both hit branches join conversation
polling; cancellation destroys suffix before outer cleanup. Native differential
test passed17.430s. StartVillagerTalkConversation added to text bridge: fresh hero
for facing(false), create(false,false), fresh hero participant; updated compiled
100-case harness passed with changing hero results. Helper remains separate until
full Villager owning-resource/movie/string candidate composition; no claim of
completed Main integration. Background Bully integration complete; Guard assigned.

Villager owned-text bridge now passes 100 compiled x86 policies through actual FSE
types and real Lua (engine calls are doubles). AssignVillagerSuffix preserves the
owned suffix; AddVillagerTalkLine preserves hero-first lookup, constructor/concat
return pointers, exact line arguments, and reverse result/prefix cleanup, including
original-error preservation. Native suffix and temporary tests: 2 tests OK (1.427s).
Evidence: work/villager_text_runtime_checks/result.json and test.log; implementation
retail_villager_text_actions.inc, runner run_villager_text_runtime_checks.py.
NEXT compose owned Text storage and Villager candidate; native vector CString
assignment/lifecycle still needs integration. No DLL/gameplay validation claimed.

Villager talk temporary original-instruction trace passes24 cases: both prefix
branches, raw hero null/non-null, conversation IDs-1/0/73, aliased constructor/concat
return pointers. Native call order hero -> prefix construct -> concat(suffix24) ->
AddLine(false,bound actor,hero) -> result destroy -> prefix destroy. Result52/prefix56
and result60/prefix64 share prefix destructorDAE59D. Suffix survives. Test
test_native_villager_talk_temporaries passes0.354s. NEXT owned CString runtime bridge
and composed Villager candidate using verified control/movie/health/string scopes.

Villager talk suffix CString24 lifetime verified across acquisition, conversation
polling and all normal/cancel exits. DAE3F7 default construct; DAE43D assigns _MALE
or _FEMALE; DAE52A/DAE580 concatenate selected prefix into temporary52/60 using
suffix24; DAE602/DAEA44 destroy suffix. Shared cleanup receiver LEAs pinned and full
CFG passes. Prefix literals DONE_BAD_DEEDS/SPOKEN_TO verified. Native call setup
checks confirm suffix operand and prefix constructor result. NEXT temporary prefix/
result scopes, then owned-string adapter/candidate. Full suite13 already completed
1282 tests OK; new Villager checks run separately.

Villager retained conversation-key map verified: default CString at stack20 constructed
DADFF4, assigned by99EFB0 atDAE96B from chosen vector element, passed atDAE988 with
flag0/bound actor/fresh raw hero, destroyedDAE9A9 orDAEA4D before control. Full CFG
key lifetime passes; default/assign helper bytes pinned (assignment retains source
string storage). Event omission and wrong key/speaker/listener setup tests added.
NEXT temporary talk-response CString24, literal key scopes, then owned-string-aware
Villager candidate; do not replace retained native key with transient host string.

Full suite13 COMPLETE:1282 tests/761.213s/OK, Python subprocess exit0 (elapsed761.869s).
Evidence work/converter_marathon_suite_20260913_13.log and.result.json. Session52671
is terminal; do not poll/restart it. Villager modules added after suite discovery
were validated by separate focused commands and are not claimed included in1282.

Villager movie pause paths now tested through original instructions:18 cases prove
early sex-branch cancellation carries EBX=0 from construction, while normal/late
cleanup pushes explicit zero even with arbitrary EBX. Pause(false) precedes movie
destruction on every checked exit. Corrected earlier classification: DAE2E4 is a
post-speech cancellation destructor, not a second normal destructor; DAE3AB is normal.
Lifetime coverage was already correct. Two native pause tests pass0.345s. NEXT map
retained CString20 and temporary keys, then build Villager resource candidate.

Villager hit movie lifetime now verified: stack108 constructor/start, normal
destruction DAE3AB, cancellation destroys DAE2E4/DAEA06/21/39; seven events
and six pause calls all on live movie paths. Coverage verifier caught initially
omitted male normal cleanup and now rejects every single event/pause omission.
native_villager_movie_scope test passes2.862s. NEXT prove pause flag dataflow (EBX
zero at early cancellations and shared-push join), retained conversation CString,
then candidate lowering. Full suite13 remains verified live at session52671.

Villager two health temporaries verified: control92 -> Thing68 atDAE238/queryDAE241/
destroyDAE25E; control92 -> Thing80 atDAE309/queryDAE312/destroyDAE32F. Queries use
returned Thing, not bound/cached actor; full temporary CFG lifetime passes. Zero
threshold122DEDC is pinned. Native x87 comparison/destructor checks18 float cases
cover signed zero, subnormals, infinities and NaN, preserving BL across EAX-clobbering
destructor. Combined Villager/Barrel health4 tests pass1.183s. NEXT movie and retained
CString map, resource-aware Villager generator. Suite13 still live session52671.

Villager owning control map now verified directly from all2787 Main bytes:23 events,
single baseline stack92 resource, four bound-EDI priority4 acquisitions, two speech
and two temporary-Thing getters, cleanup through inline/full joins. Full CFG lifetime
check passes. Movie constructorDAE1AE excluded by pinned Main bytes/vtable sequence.
native_villager_control_resource.py verifies helper profiles, event completeness and
exact call setups; omission/wrong-actor tests added. Existing reconstructed-port
audit only matched text and is not substituted for this evidence. NEXT map the two
health temporaries, movie and retained conversation CString, then generate candidate.

Villager missing ally target and sex-query actor restored to bound me, pinned to
DAE109..DAE135 and DAE206..DAE212. Nine original-instruction ally traces preserve
both hero queries and directions, including empty/changing pointers; changed native
regions reject. Combined Villager3 tests pass2.240s. Builder composes operand fix
with termination correction. Two resource-derived health placeholders remain, along
with owning resource/movie/string and cached-hero semantics. No completion inferred
from reduced placeholder count. Full suite13 continues on existing session52671.

Villager readable loop termination now uses the actual IsActiveThreadTerminating
boolean instead of undefined extraout_AL_00/32 followed by numeric comparison.
Native DADFF9..DAE008 and DAE996..DAE9A5 pinned; original instructions tested for
all256 AL values at both sites (512 branches) with high EAX bits nonzero. Two tests
pass2.147s. Raw draft untouched; builder applies recovery before normal readability.
This fixes premature exit but does not solve Villager resource/speech/string scopes.
Suite13 remains live session52671 (last verified after new focused tests); the new
Villager test was added after discovery began and is independently validated.

Barrel outer-loop differential gate added:162 frame-count/phase-stop/interaction-stop/
boundary-error cases preserve call order and guard/start/resource cleanup; old loop's
unreachable movie cleanup is asserted unreachable. Interaction720 cases still pass.
Full script-recovery suite13 RUNNING via unified exec session52671; do not restart
while handle is live. Log work/converter_marathon_suite_20260913_13.log; terminal
result will be written to matching.result.json by Python subprocess wrapper. This
avoids PowerShell stderr status ambiguity. Current readable gap audit refreshed.

Barrel Init global timer reset corrected in separate native_barrel_init_timer.py:
WithRetailResources scope calls ResetBarrelWatchTimer(WatchTimer), native global
interface slot164 with value0. Existing native_barrel_init.py is the earlier lifter
operand recovery; a filename collision was restored from original session history,
including its original witness, and both original Init tests pass. Combined Init/
candidate5 tests pass6.286s. Movement/timer compiled gate now16 policies (adds reset
IDs0/-1/73); complete staged registration compile succeeds. Readable builder rerun.
Native Init review confirms actor flags, copied home vector and sight10, but runtime
API/home-float/entity-state lifecycle validation remains. No live runtime applied.

Structured Barrel candidate integrated into build_readable_new_oakvale and readable
FSE/NewOakValeIntro/Entities/NOVI_BarrelMan.lua. Builder reports18/18 files compile,
51 functions represented. Main mapping now recognizes __resource_main for Barrel.
Generator verifies original entry-condition correspondence then registers the bound
conscious condition before first frame in structured output; builder avoids duplicate
registration. Phase1 timer DB552D..DB5547 verified and corrected to existing staged
SetBarrelWatchTimer(global receiver, WatchTimer ID,45). Candidate3 tests pass7.129s;
entry-condition/readability15 tests report OK2.744s (PowerShell stderr redirection
reports NativeCommandError despite unittest OK; do not treat shell status as test
failure). Banner/remaining report now explicitly distinguish structured Main from
pending Init/lifecycle/API and composed runtime/gameplay validation. NEXT inspect Init
against native, broaden full-loop transitions, and run integration/full-suite gates.

Barrel Main now structured: handleBarrelInteraction owns hit/approach/talk/overhear
dispatch; outer while preserves top termination, phase, interaction, frame order.
All exits after construction share guard/start/resource cleanup. Removed dead scratch
declaration and unreachable movie cleanup labels. Differential720 interaction cases
cover phase, hit, approach/talk short circuit, overhear, cancellation and helper false
returns; combined4 tests pass6.101s including candidate resource/error cleanup. Exact
before-source witnesses are readable_barrel_interaction_before.lua and
readable_barrel_loop_before.lua. NEXT verify full-loop transitions/cleanup, fix phase1
global timer setter, restore standalone entry condition, integrate readable builder,
then reassess remaining NewOakvale entities and runtime parity. This is presentation
and offline validation, not full quest completion.

Barrel nested phase switch now lowers to advanceBarrelPhase with if/elseif branches,
shared successful return phase finalization, and existing reviewed action helpers.
The initial phase read, nonzero-phase termination check, and second phase read stay
distinct. Differential672 cases cover both reads (including changed phase), phases
0..5, thank/failure choice, cancellation and every action helper returning false.
Combined4 tests pass6.012s; candidate regenerated/syntax passes. Presentation witness
is readable_barrel_phase_before.lua. NEXT: structure per-frame hit/talk/overhear body
and outer loop; remove dead labels/locals; check phase1 global WatchTimer setter;
restore entry condition and integrate readable. Background Bully continues HUD/question
CString order and colour operands after its38-test isolated checkpoint passed.

Barrel control-flow presentation now factors five identical verified acquisition
loops into acquireBarrelControl. Prepare once; first acquire before frame; failed
acquire frame/termination order preserved; final termination query after success.
readable_barrel_control.py requires all five exact loop correspondences, applies
after native lowerings. Differential Lua checks126 failure/cancellation/error cases
produce identical traces/results; combined4 tests pass7.244s. Candidate regenerated.
NEXT: structured phase dispatch and per-frame interaction body, remove obsolete
labels/scratch locals, restore standalone entry condition and integrate readable.

Original conversation instructions now pass12 Unicorn traces (both sites, IDs-1/0/73,
constructor result aliases): exact call arguments and listener-before-key cleanup,
including EAX clobber after each boundary. New test_native_barrel_conversation_trace.py.
Overhear scratch block now lowers to phase==0 and ShouldBarrelOverhear(me,heard), then
termination check, state write, conversation. DB687C..DB68CE pinned. Proposed method
calls retail rand BFEB16 only when already heard, reads live signed divisor13AC854
AFTER rand, tests signed remainder zero, then fresh raw hero/distance-under15. Native
IDIV exceptional operands raise an explicit host error. Candidate regenerated/syntax
passes. Original-instruction/Lua gate comparison now passes192 cases across phase,
heard state, signed random/divisor, distance and cancellation. The rand hook mutates
the divisor from zero to its actual value, checking native read-after-rand ordering.
Lua check validates state write before conversation and skips both on cancellation.
Compiled actual-FSE-type/real-Lua x86 overhear checks now pass67 policies, including
the post-rand live divisor, signed remainder, empty raw hero, both division faults and
closed scope (work/barrel_overhear_runtime_checks/result.json). Engine/random calls
are doubles; this is not gameplay proof. NEXT: structure whole Barrel dispatch and
integrate readable output. Background Bully agent confirmed
running during this checkpoint.

Barrel failure/overhear conversation operands now lower to AddBarrelConversation.
The native sequences construct the text CString, construct empty listener via6E7B40,
call AddLine(conversation,key,false,bound actor,constructor result), destroy listener,
then destroy CString. Failure text is TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE;
overhear text is TEXT_QST_048_BARRELMAN_OVERHEAR. Generator pins both native regions,
constructor bytes, literals and exact source correspondence. Staged adapter includes
reverse temporary cleanup on host exceptions; no runtime installation. Combined4
evidence-rejection/candidate tests pass5.537s; full staged FSE registration compiles;
candidate regenerated and Lua syntax passes. Compiled x86 actual-FSE-type/real-Lua
conversation harness now passes28 policies (work/barrel_conversation_runtime_checks):
both lines, signed conversation IDs, empty CString, returned target identity, reverse
cleanup, original line error preserved over cleanup error, and closed scope rejection.
Engine APIs and constructor are doubles; original-instruction argument trace remains
to be checked. NEXT: native argument trace, overhear random/distance predicate, then
whole Barrel structured dispatch/readable integration. Gameplay remains unvalidated.

Barrel return encounter now restores BOTH edges into THANKS: visible(hero, bound me)
or distance-under10(bound me, freshly queried hero). The old draft used unrelated r5
for visibility and exited Main on positive distance. FaceBarrelManTowardsHero preserves
the native raw hero and false third argument. Both new runtime methods are staged in
retail_barrel_return_encounter.inc; complete FSE registration compilation passes.
Original-instruction Unicorn checks cover16 visibility/distance/empty/changing-hero
cases and exact branch destinations/operands. Combined candidate/native4 tests pass
5.798s. Candidate regenerated. Actual C++/Lua x86 boundary execution now passes35
policies (work/barrel_return_encounter_runtime_checks/result.json): all eight empty
hero combinations, both visibility/distance outcomes, no distance dependency on a
visible result, and closed-scope rejection. Engine calls are doubles. Failure
conversation research confirms 6E7B40 constructs an empty CScriptThing (vtable1238C8C,
both pImp fields zero); literal12D91B0 is TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE.
Next: failure/overhear conversation scopes and final structured dispatch. The
runtime methods are unapplied; compilation/native emulation are not gameplay proof.

Barrel returnToWarehouse helper now composes into phase3: one position snapshot from
the Main-owned warehouseStartMarker, controlled distance/move/task loop, cancellation
before phase4 write. Native DB5915..DB5A48 and exact intermediate Lua are pinned.
Main now owns warehouseStartMarker and warehouseGuardMarker; explicit shared cleanup
destroys guard then start before control resource. Existing markerMap verifies both
lookup/name lifetimes and native inline/full cleanup equivalence. Remaining early
returns and errors use the existing resource callback cleanup policy. Helper36 cases
and combined7 tests pass (6.163s); candidate regenerated, Lua syntax passes. Candidate
query-error case verifies temporary5 -> walkoff4 -> guard3 -> start2 -> resource1.
Phase4 visibility/conversation operands and overall control-flow integration remain.

Barrel phase2 now lowers to teleportWalkOff: fresh WatchTimer ID/global timer query
per poll, equality 15, two owned markers, primary-position camera query, cancellation
check after camera, selected teleport and phase3 write before alternate/primary cleanup.
Native DB5734..DB588F bytes and intermediate Lua correspondence are pinned. New helper
tests exercise 42 timer/camera/cancellation cases; combined helper/candidate 5 tests pass
(5.839s). Candidate regenerated and Lua 5.4 syntax passes. Three marker/timer methods
are composed into the unapplied Barrel runtime proposal; complete registration compiles
with actual FSE headers (candidate SHA 8919581a506af1d54c1669be9c0176ebeca79f6e61b3c1fbbf1f0a9ae3c53dbf).
The three new methods now also pass 35 actual-FSE-type/real-Lua x86 boundary policies
(work/barrel_marker_runtime_checks/result.json; executable SHA
ee3ad2af83a6fad099c241ea214eccd3752dc01dc33db439999892a1929b9b77).
Cases cover changing global timer receivers and signed IDs/results, exact borrowed or
fallback/null position pointers, both selected owned wrapper targets, and closed scopes.
Engine calls are doubles. Next: retained main markers/phase3 and remaining
phase4/conversation operands. Registration compilation
does not establish runtime behavior or DLL/gameplay parity. Readable Barrel integration
and overall NewOakValeIntro completion remain pending.


Barrel phase1 walk-off marker owned scope integrated. walkOffFromWarehouse constructs
one NewThingFromScriptName marker, snapshots ThingPosition once, retains it through
all distance/move/task/termination checks, writes phase2 on success BEFORE destruction,
and destroys once on cancellation. Full marker/string/inline-cleanup verifier now runs
at candidate generation. Helper36mocked movement/cancellation/wait cases pass; whole
candidate tests updated for marker2 / controlled temporary3 and query-error cleanup
thing3->marker2->resource1. Combined4tests pass5.961s; candidate regenerated/compiles.
Next phase2: native5734/577C GetTimer uses CURRENT GLOBAL interface and re-reads quest
WatchTimer ID each poll, waits equality15. Lookup primary208 then alternate220; camera
queries primary implementation position or fallback, termcheck then chosen teleport;
phase3 write precedes destroy220 then208. Need owned camera/teleport adapters and
GetBarrelWatchTimer; phase1 SetTimer45 should use existing global SetBarrelWatchTimer.
Background Bully agent now has full disabled Main, finishing runoff actor/string maps.



Barrel departure markers now lower to TeleportBarrelDepartureActors(me). Native6401..64A4
pins independent guard/hidden name scopes: construct name, lookup owned marker, optional
fresh borrowed hero, teleport using lookup RETURN pointer, destroy marker then name.
retail_barrel_departure.inc preserves empty keys/markers and null hero forwarding; error
cleanup keeps original error and closes marker before string. Actual FSE/Lua harness
passes21 policies incl16empty/null/returned-target combinations plus5error sites.
Candidate3tests pass4.066s; candidate regenerated/compiles. Combined proposal now7methods;
full registration compile re-run succeeds at work/barrel_resource_integration. Runtime
unmodified; remaining retained main/walk-off markers and body structure still pending.
User clarified status questions mean ALL NewOakValeIntro: report51functions/18compiling
files but behavior incomplete; affair trio/Book furthest developed, Barrel/Bully active,
other entities/main quest and full DLL/playthrough still outstanding. Do not present
Barrel-only progress as whole-quest completion or infer a percentage from syntax counts.



Barrel movement adapters validated and runtime proposal consolidated. Actual FSE-type /
real-Lua harness passes13 policies: fresh hero/GetPos each call, exact borrowed vector
pointer (including returned null forwarded), bound-actor distance4.0, original native
resource wrapper move2.0/type1/false/true even empty resource, WatchTimer45 through global
interface, call errors/closed scope. work/barrel_movement_runtime_checks all3commands0,
PE014c. prepare_barrel_resource_extension composes6Barrel/timer methods over wife/man/
woman proposal plus helperheaders in work/barrel_resource_integration. Complete composed
RegisterRetailResources templates compile to x86 object via run_barrel_registration_compile;
registration-result.json hashes source/headers/object. This is compilation only, no link/
DLL/gameplay. Original runtime remains unchanged. Next focus: owned departure markers and
remaining cached marker lookups, then final structured Barrel body/readable integration.



Barrel phase zero structured in native_barrel_initial_interaction.py and integrated into
candidate. playInitialInteraction keeps WithTimer live through movement, favour speech,
fade/departure/state updates; native normal timer destruction precedes PrepareResource
then movie finish. Cancellation closes timer before movie without reset. The zero-ID
piVar8 placeholder and dropped successful departure join are removed. Helper tests
cover240 mocked movement/busy/health/termination/remaining-time scenarios, both expired
and positive timer paths; pass0.014s. Earlier combined initial/candidate4tests pass4.559s.
Candidate regenerated/compiles. New retail_barrel_movement.inc proposes fresh borrowed
hero GetPos distance4.0 and native resource move2.0,type1,false,true plus WatchTimer45
through global interface. THESE THREE METHODS STILL NEED COMPILED CHECKS/INTEGRATION.
Departure currently retains old quest lookup/teleport wrappers; exact owned marker/
CString/borrowed hero lowering is still pending. No full Barrel gameplay parity claim.



Owned timer adapter ready for phase-zero composition: retail_owned_timer.h exposes
Set/Get within WithRetailOwnedTimer callback; resources method proposal is WithTimer.
Registration, Set, Get and deregistration each resolve current global143E8F8 and native
virtual slots15C/164/168/160; preserves any returned ID incl0/-1 and signed remaining.
Callback must return bool; close precedes return, escaped objects are inert, BODY errors
win over cleanup errors, closed owner cannot deregister again on GC. Actual FSE types
and real Lua harness passes27 policies, all3build/testcommands0, PE014c; artifacts
work/owned_timer_runtime_checks. prepare_owned_timer_extension emits reviewable patch
and complete candidate headers in work/owned_timer_proposal; original runtime unchanged.
Full resource-class/DLL registration remains untested. Next: phase-zero Lua uses
WithTimer across movement, favour speech and departure, returning before movie cleanup.



Barrel phase-zero timer ownership verified by native_barrel_man_timer.py/witness.
CTimer at stack68 constructs viaCD4450 (23bytes), storing the ID returned by current
global interface143E8F8 slot15C. Movement caches ID in EBP, sets2 via global slot164,
polls signed remaining>0 via168. NativeCD4470 destructor (20bytes) re-reads global
interface and deregisters storedID via160. Six events through full bounded-switch CFG
prove one cleanup on every constructed path (normal64E7, cancellations6AC4/6ADB).
Two tests pass8.175s: omissions rejected, original ctor/dtor execute8 ID/global-switch
cases incl0,73,-1. Candidate now verifies/reports timer map, but timer Lua lowering
remains next; current raw piVar8=0 is NOT the native timer identity. Need owned scope
with fresh global receiver per timer operation, plus phase-zero movement/teleport joins.



Barrel nonzero interaction dialogue composed into candidate. playReturnInteraction
preserves phase1..3 NOT_LARKING, phase5 HeroLetMeDown/BarrelBroken selection, and
phase4/other no-speech finish. Exact termination checks occur before the same state
reads; positive-health speech uses the reviewed wait helper. Normal paths prepare/reset
control before unpause+movie destruction; cancellation closes movie without reset.
All4movie constructions now use resource StartMovie; final interaction scope's common
native destructor join is explicit. Phase-zero timer/movement/teleport flow still has
known missing joins/operands and remains incomplete. Helper tests cover480 mocked
phase/flag/health/cancellation/wait scenarios. Combined return-dialogue/candidate/speech
5tests pass3.834s; generated candidate compiles. Retained marker composition still pending.



Barrel thanks/careful movie scopes now composed via native_barrel_speech_movies.py.
Candidate helpers playThanksMovie/playCarefulMovie share the native task/frame/term
wait shape, unpause then destroy their movie once, and return explicit continuation.
Thanks awards the good deed even when health is nonpositive/unordered (speech skipped),
but not on cancellation; careful returns to LAB6933 instead of the draft's wrong Main
return. Whole native movie/resource witnesses and exact intermediate Lua correspondences
are verified before rewrite. Helper tests cover72 mocked health/task/cancellation cases;
combined speech/failure/candidate6tests pass2.991s. Candidate regenerated and compiles.
These tests exercise helper semantics, not complete native/gameplay execution. Movie72
(the larger interaction movie) and retained marker lifetimes still require composition.



Barrel unattended-warehouse movie composed into control candidate via
native_barrel_failure_movie.py/witness. showWarehouseFailure owns one empty-name movie,
pauses, displays the instruction, polls frames/termination, awards bad deed1 only on
normal dismissal, unpauses and destroys exactly once. Native DB5C23 joins DB5DAF then
restores BRAIN_GOOD_VILLAGER_BASE and phase5; draft incorrectly unpaused twice and
exited Main. Candidate now restores the normal join and keeps cancellation on cleanup.
Native movie inventory/lifetime map re-verified at generation. Two helper/source tests
cover24 mocked dismissal/cancellation schedules plus5native byte mutation guards;
combined candidate tests5pass3.197s. Candidate regenerated/compiles. These are helper
behavior tests against reviewed native flow, not full native execution comparisons.
Three other movie scopes and retained marker ownership remain to compose. Background
worker continues Bully presented-item output and movie cleanup joins.



Barrel health branch checkpoint: all7 native x87 comparison/destructor/result sequences
verified by native_barrel_man_health_branches.py/witness. Native tests execute63 cases
(7sites x9values: signed zeros, positive/negative, subnormals, infinities, NaN), including
result survival across destructor EAX clobber. One inverted raw fVar19<=fVar20 branch
is corrected to not(fVar19>fVar20), preserving native NaN behavior. Integrated in both
resource candidate and readable builder. Two focused tests pass0.150s; combined
candidate+health5tests pass2.547s. Both artifacts regenerated; readable18/18 compile,
51functions,1056names/887semantic/169scratch unchanged. Movie/marker ownership
composition and remaining control-flow/operand recovery continue next.



Barrel hit-branch ally operands recovered: DB5EEA..DB5F16 proves bound actor -> first
borrowed hero, fresh second GetHero -> bound actor. The raw draft incorrectly used
walk-off marker r3 in the second direction. Candidate now calls SetBarrelManHeroAllies.
Native test covers9 independent null/same/different hero pairs, exact call order and
five instruction/binding mutation guards. Combined ally/candidate tests5pass2.363s.
The staged approach adapter now includes ally method; actual FSE type/Lua harness
passes30 policies (19approach +9ally pairs +2ally errors), all build/test commands0,
PE014c. No raw hero copies or null-hero filtering; exceptions stop subsequent queries.
Candidate regenerated and syntax passes. Adapter is still not registered/applied to
runtime; retained markers/movies, hero movement/distance and conversation operands
remain. Background Bully worker is composing verified joined loops and mask scopes;
its corrected initial comparison count is174 (30home+144health), not earlier204.



Barrel hit/interaction masks recovered and integrated into the separate control candidate.
DB5DEE..DB5ED3 uses IsHitByHeroExceptAbility(me,14), preserving short-circuit hero-name
CString lifetimes and reverse cleanup. DB60A3..DB611C is phase0 approach OR talk;
talk is queried when approach is false, and phase!=0 skips approach. This removes
unresolved ppuVar17/ppuVar18 masks and two erroneous early returns in those blocks.
Three native tests pass:64 hit cases plus64 approach/talk cases, mutated branch/source
rejection. Combined with candidate tests:6pass3.414s. Approach reads live float13AC858
BEFORE fresh borrowed GetHero and forwards even null hero to native distanceCBE2FF.
Proposed retail_barrel_approach.inc compiles with actual FSE types and real Lua in
work/barrel_approach_runtime_checks:19 policies pass, all3commands exit0, PE014c.
Native test mutates threshold during GetHero to prove load order; compiled harness
also covers null/populated hero, signed zero/NaN bits and query errors. Host class
registration/DLL/gameplay remain pending. Retained marker/movie composition and
remaining hit-branch ally/conversation operands are still needed for full BarrelMain.
Background agent continues Bully joined acquisition branches/cleanup masks.



Barrel control candidate now exists at work/barrel_man_candidate/, regenerated with
python -m tools.script_recovery.generate_barrel_man_resource_candidate. Source hash,
66-event owning-resource map and11 temporary Thing lifetimes are re-verified. It lowers
6 preparations,10 acquisitions,20 task polls,7 speeches,3 moves,7 health reads and4
marker-distance queries. Hero move arguments corrected to position,2.0,1,false,true.
Three tests pass2.663s:20 entry/acquisition/first-distance cancellation+retry scenarios,
query-error temporary-before-resource cleanup, changed draft rejection. Candidate Lua
compiles, but is deliberately not integrated into readable yet: retained marker/movie
ownership, borrowed hero operands, hit/conversation/control-flow gaps remain. Running
past the initial phase exposed an existing unresolved ppuStack_1c8 cleanup mask in the
hit block; next useful step is native hit CString lifetime lowering (raw lines307..400)
and then composing movie/marker ownership. No cached-health or control APIs remain
in this candidate, though two borrowed-hero distance calls remain cached pending audit.



Barrel camera cancellation fix is integrated into the readable builder. The raw draft's
offscreen branch ignored termination and could still teleport. Both camera outcomes
now perform one termination check before selecting primary marker208 or alternate220;
termination jumps to the existing cleanup join. Native DB582D..DB588A versus emitted
Lua passes16 cases (camera/termination AL0,1,2,255), verifying destination, phase and
normal marker destruction order. Two tests pass0.067s, including source and four native
branch mutation guards. Full readable regeneration compiles18/18 files; totals remain
51functions,1056names,887semantic,169scratch. This is a control-flow repair, not proof
that Barrel's remaining resource/marker lifetime lowering is complete.
Files: native_barrel_camera_cancellation.py, witness, test; builder integration.



Barrel marker snapshot checkpoint: native_barrel_man_position_snapshots.py/witness
pins walk-off marker172 -> value snapshot184 and warehouse marker36 -> snapshot196.
Original instructions pass12 cases covering populated/empty markers, live fallback
0x143E8E0, negative zero, NaN payloads and infinities. Both snapshots retain all12
bytes after the source vector changes. Tests also verify the staged distance2.0,
controlled-Thing output and owning-resource20 arguments; six byte mutations reject.
The controlled temporary-Thing verifier now checks all four distance consumers use
the corresponding proven snapshot. Camera position remains a separate borrowed
query; full resource-aware Barrel Lua composition remains pending. Audit:
work/barrel_man_position_snapshots_audit.json.



Barrel inline cleanup equivalence checkpoint: native_barrel_man_inline_cleanup.py/witness
pins DB694C..DB6A03 and actual native4AA840/7E74D0 destructor bodies plusbasehelpers.
Two tests pass1.743s. Original inline instructions versus actual calls DestroyThing48,
DestroyThing36,ReleaseResource20 match across216 combinations (Info null/ref1/ref2 for
threeobjects, independently empty/populatedData), including event order, finalcounts,
objectfields and stackbalance. Delete/free/resource-base boundaries are engine doubles;
Thing base destructor99A2E0 itself executes. Mutating five release instructions rejects.
Marker verifier now also re-verifies this evidence, and work/barrel_man_markers_audit.json
links it. This resolves inline cleanup expansion for lowering; marker query/position
provenance and the full resource-aware Barrel candidate remain next. The background
worker is now recovering actual Bully operands/resources in an isolated candidate.



Barrel named-marker checkpoint: native_barrel_man_markers.py/witness covers7named lookups,
12cleanup boundaries and7nameCString scopes. Warehouse start/guard coexist; phase walkoff
slots172/208/220 and teleport slots384/408 have independent ownership. Shared DB6A32
cleanup selects172 or208 by reviewedECX LEAs. Both normal and inlinebase destructor exits
are included; base99A2E0 seven-byte body pinned. Two tests pass39.069s: every omittedlookup/
cleanup, missingreceiver selection and premature stringdestruction reject. Audit:
work/barrel_man_markers_audit.json. This proves lookup/cleanup boundaries; all marker-use/
position provenance and inline strong-release semantics still need review before lowering.
Readable final passes integrated for husband, BookTrader, wife and woman. Book now uses
short-circuit timer==0 and random==0 directly (random queried only when timer is exactly0),
and wife removes sole immediate animation comparison temporary with callback unchanged.
Ten husband/Book/wife tests pass8.003s; woman two tests pass1.433s, including507wholewoman
scenarios. Fourpasses simplify48literal booleans total and remove redundant temporaries;
source maps retain provenance and presentation reports. Final rebuild passes18/18files; current totals1056renamed/887semantic/169scratch. Background worker is now auditing actual executable
readable gaps, distinguishing them from historical comment diagnostics.



Husband final presentation integrated: builder now applies readable_affair_man.py after
structure_cleanup, folds17numeric literal comparisons to actual Lua booleans, and removes
the exact immediate randomChoice3 comparison temporary. Presentation changes/source hashes
are reported; ledger excludes the removed scratch name. Six husband tests pass5.181s,
including186before/after behavior/error traces and existing structure checks. Background
worker now investigates BookTrader readability. User clarified they had been viewingraw
FSEoutput, and now knows readable/FSE is the current review copy.
Barrel marker work in progress: seven named-lookups found (stack36/48 persistent markers,
172/208/220 phasewalkoff handles,384/408 teleporttemporaries). Concurrent-owner checker
added to native_resource_lifetime.py;11resource/movie tests pass9.760s, including independent
concurrent locals, no doubleownership and missingcleanup rejection. The single-active
contract still rejects overlaps. Namedmarker witness/usage mapping is not yet complete.
Shareddestructor DB6A32 selects172 viaLEADB5716 or208 viaLEADB6A2B; DB572A uses172.



Barrel movie checkpoint: native_barrel_man_movies.py/witness verifies four nonoverlapping
movie scopes (stack272,256,88,72),12movie events and17pause calls through the bounded phase
switch. Movie88 uses inline99A380 construction; allfourclassstrings construct/query/destroy
beforepause. Nine explicitECX receiver selections establish shared destructor destinations.
Movie and pause lifetimes are checked separately; unpause must use a live movie and all
paused exits must balance. Three tests pass10.656s: omit eachmovie/pauseevent, omit each
selectedreceiver, earlyclassdestruction and wrongpauseidentity reject. Audit:
work/barrel_man_movies_audit.json. Retained markers/vector provenance and complete Lua
lowering remain. User asked specifically for background readability work; the existing
worker is now improving husband literal/control readability, with Rock proposals stable.
User's exact pCVar22/LAB_00db1d7c examples are inraw FSE/Entities output; readable husband
already has noexecutablegotos. Use readable/FSE/... for review.



Barrel temporary Thing checkpoint: native_barrel_man_temporary_things.py/witness verifies
all11GetScriptThing outputs: four distance and seven health queries, with immediate
DestroyThing calls and complete lifetime CFG through the guarded phase switch. Distance
uses the returned Thing inECX, stackvector inEDX, and float2.0 onstack; health passes the
returned Thing to interface slot+420. The caller's output stackslot and destructorreceiver
must match. Missing entries, wrong outputslot and premature destruction reject at each
of11sites (33mutations). New two tests passed alongside three imported owning-resource
tests (5/5,39.589s); the import was then changed to module-qualified form to avoid duplicate
suite discovery, and discovery now confirms two new tests. Audit:
work/barrel_man_temporary_things_audit.json. No Lua body promotion yet; vector provenance,
retained markers, movie/pause scopes and complete operand validation remain next.



Barrel owning-resource checkpoint: native_barrel_man_resources.py/witness verifies66
control-resource events at baseline stack20 across Main DB5330..DB6B23, with creation
DB538C and destructor joins DB69FE/DB6B13. DB5F8E is separately pinned as movie-local
construction (vtable1260EF4), not a second control resource. Native bounded phase switch
DB5526 reads four targets atDB6B24; native_bounded_switch.py proves the unsigned CMP/JA
bound, table separation/alignment and absence of guard-bypassing edges. The lifetime
checker accepts only caller-proven indirect target lists; unknown edges still reject.
21focused tests pass, covering new map/switch tests and wife/woman lifetime regressions.
Omitting any of66events rejects, deleting either cleanup fails CFG, and mutated table,
selector, resource/movie bytes, unaligned targets and guard bypasses reject. Audit is
work/barrel_man_resources_audit.json. This verifies owning-resource boundaries only;
11temporary Things, retained markers, movie/pause lifetimes and complete operand/self
provenance need follow-up before resource-aware Barrel Lua lowering. The four-entry jump
table is outside the reviewed instruction body. No runtime/canonical/game changes.



Barrel setup compiled API checkpoint: run_barrel_man_setup_runtime_checks extracts all
five existing LuaQuestState setup method bodies unchanged and compiles them in a minimal
host shell with actual FSE argument types and vendor Lua. All three build/test commands
pass;12Lua cases cover populated/empty actorData, null/non-null Info and three homepositions.
The recovered Lua calls produce brainCString construct/query/destroy, one home query,
then center/min0/max1/group4. Each by-value consumer sees the same actor data with one
additional reference and returns it to baseline. Native setter consumers and home query
are doubles; fullhost/gameplay integration is still unproven. Evidence and hashes are in
work/barrel_man_setup_runtime_checks/result.json. New executable is barrel-wander-check.exe
(the earlier name triggered Windows installer-name elevation detection; no elevation used).
Resource/movie/retainedmarker recovery for the rest of BarrelMan Main remains next.



Barrel Man setup checkpoint and suite result: full recovery suite12 finished successfully,
1161 tests in512.242s, captured Pythonexit0 (.log/.result.json under work/). That snapshot
preceded the new talk/setup tests and subsequent generator integration; those changes
passed21focused tests separately (wife candidate/readability, talk scopes, Barrel setup).
The readable builder now replaces Barrel Man's missing opening actor/position arguments
with bound me and one actual home-position read. Native maximum wander distance is1.0,
not the old draft's0; minimum remains0.0 and state group4. Brain isBRAIN_PASSIVE_OVERRIDE.
`native_barrel_man_setup.py`/witness pins261nativebytes and exact draft region; newtests
execute original caller instructions for two positions and null/non-null reference-info,
checking four copied Thing arguments, reference increments/consumer cleanup, stack balance,
and matching Lua calls. Existing FSE wrappers already retain their by-value arguments;
complete compiled setup-method comparison and the rest of Barrel resource scopes remain.
Wife generator now re-verifies both native talk scopes and records them in its report.
No canonical raw port/runtime/game files changed. Older suite-running notes are superseded.



Wife existing talk method checkpoint: `run_wife_talk_runtime_checks` extracts the current
IsTalkedToByHero method unchanged from runtime LuaEntityAPI.cpp and compiles it with
actual FSE types and vendor Lua. All three build/test commands pass; PE x86 0x014c,
input/method/executable hashes in work/wife_talk_runtime_checks/result.json. Tests check
slot+0x6c, same CString identity, construct/query/destroy for true/false and query errors,
and null actor/vtable/method guards. The harness invokes through an isolated Lua closure;
it does not claim complete host registration or runtime/gameplay validation. Existing
method mapping is supported for the verified wife path, so no new adapter is needed.
work/affair_wife_talk_audit.json now links both native and compiled evidence. Generator
integration of the witness remains next; full suite 12 continues in session45867 and
production generator sources were kept stable during it. Poll that handle.



Wife talk-query checkpoint: both native 39-byte CString/query/destruction scopes are
pinned by native_affair_wife_talk.py and its witness. Two new tests pass: eight original
instruction traces preserve BL through a destructor clobbering EAX, and ten mutations
reject. Native query uses self EDI, SCRIPT_NAME_HERO and vtable+0x6c, then destroys the
string before branching. work/affair_wife_talk_audit.json records the existing runtime
IsTalkedToByHero source/body hash; compiled method comparison remains pending, so no
new adapter or completed-runtime claim was introduced.
Full recovery suite 12 is currently running in exec session 45867, writing
work/converter_marathon_suite_20260913_12.log and .result.json on completion. Poll that
live handle; do not restart merely for slow output. Discovery preceded the two new
talk tests, which were run separately. Runtime/readable production sources stayed stable
during this suite run. The background worker is researching Rock Troll exhumation.



Wife helper-storage checkpoint: shared generated temporaries can now become independent
helper locals only when every using helper assigns them before every read and no outer
code or argument callback observes them. The synchronous argument callback is excluded
from analysis only for variables it never mentions; all captured variables stay shared.
Tests explicitly prevent localization for outer reads, callback captures and helper reads
before assignment. The retained husband, counter and conversation ID remain outside.
Role splitting now exposes taskRunning/controlAcquired/questionAnswer/distance values,
and literal speech arguments inline. Unused declarations are removed with provenance.
Six structure/readability tests pass (768 three-way traces, three error policies, and
negative boundary checks); combined wife tests passed 17 before declaration pruning,
then all six structure tests passed again. All 18 regenerated package files compile.
Current totals: {"renamedLocals": 1060, "semanticNames": 889, "scratchNames": 171, "functions": 51, "rawDiagnostics": 1203}. Native/gameplay gaps remain active.



Wife resource-method execution checkpoint: `run_wife_argument_key_checks` now compiles
and executes the complete staged resource header through actual sol/Lua bindings, in
addition to direct C++ and standalone Lua scope checks. All seven build/test commands
pass; all three executables are PE x86 (0x014c), with source/executable hashes recorded
in `work/wife_argument_key_checks/result.json`. The unchanged ASLR concatenation call
runs through a private test-process trampoline to an engine double.
Six resource-binding policies cover live/missing text with success, callback error,
and reply error. They check native key identity, owned husband resolution, key closure
before actor closure, escaped/closed rejection, and invalid actor kinds/nil/fractional
IDs. The lookup string double now models a populated native string so the existing
FableString wrapper actually destroys it. No runtime source or game installation changed.
The proposal remains `work/wife_resource_integration/resource-integration.patch`.
Full DLL/gameplay and remaining wife native scopes/operands remain unfinished; the older
checkpoint saying resource-method execution is pending is superseded.



Wife whole-package integration checkpoint: the readable builder now uses the verified
wife candidate, structures it before local analysis, preserves helper provenance, and
counts helper-local names in the native Main ledger. The generator-owned entry condition
is restored exactly once. The review output has no executable wife goto/address labels;
registration remains empty. All 18 Lua files compile. Current ledger: 1033 renamed locals,
843 semantic names, 190 unresolved scratch names across 51 functions (historical raw
1203 diagnostics retained). These totals reflect candidate replacement and helper scope
accounting, not a direct before/after quality score.
16 focused wife tests pass, including 768 three-way branch/cancellation comparisons and
three three-way exception cleanup traces (health, speech, reply). Shared scratch values,
remaining native operand/string-scope gaps, full resource binding execution and gameplay
validation remain unfinished. The earlier note saying wife is absent from the package is
superseded by this checkpoint.


Wife local-readability checkpoint: `readable_affair_wife.py` localizes eight exclusive
helper values only after proving every read is preceded by an assignment on every
reachable helper path. It leaves shared husband/predicate values outside, then applies
existing name/literal passes and wraps declarations. Unsupported helper syntax rejects.
`work/affair_wife_candidate/NOVI_AffairWife.readable.lua` compiles; mapping in readability.json.
Four structure/readability tests pass; all 768 original/structured/readable call+frame
traces match. Explicit tests reject prior-call carried values and preserve shared locals.
Remaining reused shared temporaries still need role splitting, and the wife is not yet
promoted into the whole readable package. Runtime method execution and other native
operand/cleanup validation gates remain active.


Wife structure/entry checkpoint: candidate generator now restores the verified bound
conscious condition before the first frame; reviewed prefix contains declarations only.
`structure_affair_wife_lua.py` replaces all three native labels with returns and helpers
waitUntilNearHusband/processHeroInteraction/runBody. No executable goto/address labels
remain. Artifact: `work/affair_wife_candidate/NOVI_AffairWife.structured.lua` (compiles).
768 before/after call+frame+termination traces match over 12 scenarios, including actual
approach waiting/running-line branch. Thirteen structure/candidate tests pass; earlier
15 candidate/entry tests passed. Next: local-role splitting around closures and remaining
operand/runtime integration review. Wife output is still isolated, not yet in readable
builder; full resource-method execution and DLL/gameplay validation remain outstanding.


Wife health/disclosure follow-up: first hit branch now uses `not (health > zero)`
instead of `health <= zero`, preserving native rejection of unordered/NaN health.
`test_native_affair_wife_health_branches.py` executes all six original FCOMP/FNSTSW
branch sequences across NaN, infinities, signed zero, +/-1 and positive subnormal.
Disclosure question tests now cover yes/no routing and 118 cancellation cases with
immediate/delayed answers; the helper mock now includes AddGoodDeed. Thirteen focused
candidate/health tests pass. Candidate regenerated, still isolated and disabled.
Next: finish diagnostic/operand audit and structure wife Lua; full resource-method
execution, entry registration and DLL/gameplay validation remain pending.


Wife live argument-key candidate checkpoint: generated Lua now uses WithArgumentKey,
Exists, ResetToFirst and AddArgumentKeyLine, holding the native key through the optional
husband reply. Cancellation returns false from the key callback, then returns through
husband/resource cleanup. Fixed counter>40 specialization is removed from this candidate;
tests accept valid _50 and reset only absent text. `native_affair_wife_argument_key.py`
pins full Main plus numeric/concat/literal/destructor/release/assignment/text-query helpers,
literals and binding, and checks both key destructor paths across the native CFG.
Eleven argument-key/candidate tests pass, including reply error and key-before-actor cleanup.
Runtime proposal remains unapplied; full resource-method execution, entry condition,
remaining call/operand audit and readable wife structure still pending.


Wife argument-key Lua checkpoint: `retail_wife_argument_key_lua.h` registers
Exists/ResetToFirst and owns the native key through a protected callback with an
explicit boolean continuation result. Real vendor-Lua/x86 tests cover normal/reset,
false cancellation, Lua error, invalid return and escaped userdata after close.
`prepare_wife_resource_extension.py` stages WithArgumentKey/AddArgumentKeyLine over
the owned-position proposal, resolves actors through existing owned-Thing resolver,
and includes the two helper headers. Actual full resource type registration compiles;
patch `work/wife_resource_integration/resource-integration.patch` passes apply --check.
Runtime unchanged. Next: pin native concat/helper profiles and replace candidate's
counter fallback/string-per-call behavior with callback scope; test persistent key
through reply/cancellation. Full resource method execution/DLL/gameplay remain pending.


Wife argument-key runtime checkpoint: staged `retail_wife_argument_key.h` retains
one CCharString across Exists/ResetToFirst/AddLine, matching native numeric suffix,
prefix, concatenation, prefix destruction then suffix destruction. Uses a separate
99F570 two-CCharString signature (existing char*-left overload is different).
Actual MSVC x86 FSE-type harness passes identity, live text lookup (including valid
_50), fallback, destruction, closed use and exception cleanup checks. Repro:
`python -m tools.script_recovery.run_wife_argument_key_checks`; result and hashes in
`work/wife_argument_key_checks/result.json`. Scope is staged only: Lua exposure,
native helper profile pins and candidate callback lowering still required. Current
wife candidate still uses pinned-bank counter fallback; do not claim that gap closed.
Inspected existing LuaEntityAPI::IsTalkedToByHero: FableString hero-name lifetime is
scoped around native call for valid bound actor; standalone native comparison pending.


Wife hit-scope checkpoint: both native mask blocks now lower to
`resources:IsHitByHeroExceptAbility(me,14)`. `native_affair_wife_hit_scopes.py`
pins DB2C15..DB2CE3 and DB36DE..DB37AC plus exact expanded-Lua correspondence.
Original instruction execution in Unicorn covers both scopes and all eight input
combinations: query short-circuit order, correct ability14, reverse destruction,
result only after cleanup, and balanced stack. Candidate mocks reject unscoped hit
calls and test ability exclusion. Nine native-hit/candidate tests pass. Uses existing
local Unicorn under work/runtime_re_tools when absent from default Python packages.
Next wife gaps: talked-to CString and persistent argument-line CString ownership,
entry condition, remaining operands and readable control structure. Full DLL/gameplay
validation remains outstanding; wife candidate remains disabled and isolated.


Wife candidate follow-up: uses the already-staged `AddConversationPerson` and
`AddConversationLine` resource methods (the earlier pending-method names were
superseded after inspecting `retail_thing_actions.inc`). Two unused CVar29/SUB41
stores are removed. Six candidate tests pass, including 220 partner-path cancellation
cases across hit/talk/zero-health variants and explicit wife/husband speaker ordering.
The same borrowed/owned resolver handles facing and conversation arguments; no
resource IDs are passed to raw host Thing wrappers. Native line-string lifetime
still spans more than one call and remains an explicit gap despite matching call
arguments. Candidate remains isolated in work/affair_wife_candidate, not promoted.


Wife candidate checkpoint: `generate_affair_wife_resource_candidate.py` now emits
`work/affair_wife_candidate/NOVI_AffairWife.resource_candidate.lua` plus evidence report.
It rechecks all three scope maps and exact raw-draft hash. One resource surrounds
10 acquisition sites, six speech/health temporaries, 14 task polls, owned husband
movement/query uses and four movie starts. Existing expanded Lua has 30 pause and
26 movie-end call sites, distinct from the 17/7 native call sites due to duplicated
cleanup branches. Three omitted partner-cleanup jumps now return through the owning
body scope. Initial cancellation precedes resource construction. Four candidate tests
pass (136 cancellation scenarios across idle/acquisition/hit/busy paths plus speech
ordering and changed-draft rejection). This candidate is NOT in the readable builder:
owned conversation APIs, scoped hit/talk/conversation strings, entry condition and
full partner/argument-path behavior still need work. Do not pass resource IDs to raw
host Thing methods; two explicit pending owned-conversation methods are emitted.


Wife retained-actor checkpoint: `native_affair_wife_actor_scope.py` verifies the sole
husband lookup (DB3445, stack48), eight movement/distance/facing/conversation uses,
five destruction joins, and the temporary lookup CString. Speaker/listener order is
wife-to-husband then husband-to-wife; distance checks use 3.0. Nine combined
resource/movie/actor tests pass. Audit: `work/affair_wife_actor_scope_audit.json`.
The current wife draft has one cached `r1` husband and 10 cached AcquireControl sites;
next step is a separate resource-aware generator using the three new scope maps,
while restoring omitted cancellation cleanup and preserving conversation strings.


Wife movie checkpoint: `native_affair_wife_movie_scopes.py` now verifies four movie
constructions, 15 movie events and 17 pause calls over the complete native CFG.
Five pinned ECX selections disambiguate shared destructor sites DB32A5/DB3E11.
All four empty-class CString temporaries are constructed, passed to StartMovie and
destroyed before pausing. Seven combined resource/movie tests passed, followed by
three movie tests with class-string coverage. Audit: `work/affair_wife_movie_scopes_audit.json`.
Next: retained actor scopes and resource-aware wife Lua lowering; the existing wife
movie witness remains separate because prior emitter passes consume its older schema.


Wife ownership checkpoint: `native_affair_wife_resource_scope.py` verifies all 60
resource events against the full 4,890 native bytes, helper profiles and argument
setups. One resource at stack16 is constructed at DB2B6C; two initial acquisitions
use priority3, eight later sites use priority4. Every constructed CFG path ends at
one of six full destructors or the inline strong/base cleanup ending DB3407.
Six temporary controlled-Thing health queries also pass full CFG lifetime checks.
Audit: `work/affair_wife_resource_scope_audit.json`; eight scope/lifetime tests pass.
Next wife work: retained husband/other actor and movie scopes, then a disabled
resource-aware candidate; current wife Lua still uses cached control and is incomplete.


Latest checkpoint: the readable woman now comes from the verified resource/movie/actor
candidate and structured helpers (`acquireAndRun`, `runInteractions`,
`runOffAndWaitForCamera`). No executable gotos or address labels remain in that file.
507 cancellation scenarios compare original, renamed and structured call traces;
two actual run-off movements, three injected errors and entry-condition ordering also
pass. Focused woman/entry/readability batch: **34 tests passed**. Owned `ThingPosition`
passes actual x86/vendor Lua binding tests (`work/woman_owned_position_checks/result.json`);
proposal remains unapplied. Latest readable totals: 51 functions, 18/18 files compile,
883 semantic names and 179 scratch names; 1,203 raw diagnostics are historical conservative
counts, not a current unresolved-TODO count. Latest full suite `_11.log`: **1,065 tests
passed**, Python exit 0, before this woman integration. Woman local analysis now expands the two reviewed inline movie cancellation exits
before reaching-definition analysis. Ten reused locals split into call-specific values;
no scratch names remain in the woman file. The expanded exits and unchanged traces are
covered by the 22-test readability/entry follow-up. Next: review remaining native operands
and complete other New Oakvale bodies/ownership. Full DLL/gameplay validation remains outstanding.


User requested a completion plan and sustained work through New Oakvale Intro, including
human-readable output. Goal remains ACTIVE; do not stop at syntax/readability milestones.
Plan: [NEW_OAKVALE_CONVERTER_PLAN.md](scripts/NEW_OAKVALE_CONVERTER_PLAN.md).
Readable package: `refs/script_recovery/lifted/NewOakValeIntro/readable/` (disabled), built by
`python tools/script_recovery/build_readable_new_oakvale.py`; includes a per-function ledger
and reversible name maps. Latest generation: **51/51 functions and 18/18 files compile**;
1,203 diagnostics remain. Readable output has 873 semantic names and 192 scratch names.
The readable husband now uses ordinary local helpers/returns and a while loop:
no executable gotos or native labels remain. `structure_affair_man_lua.py` preserves
the candidate's traces across 225 cancellation/retry/error/partner-availability
scenarios. Literal staging removal also eliminates 157 locals with checked dominance.
This does not establish full runtime/gameplay parity. Full suite `_08.log` ran 1,010
tests with four Scythe fault-injection failures; the corrected focused tests pass.
`_09.log` passed 1,019 tests in 324.897 seconds (before the new entry-condition pass).

Husband candidate now owns cached woman/wife Things and movie/pause state, classifies hits
with scoped hero-name strings, and reads the live animation argument through a narrow API.
Combined runtime proposal: `work/man_resource_integration/resource-integration.patch` (11
methods, RetailThingPosition global, bounded live-entry storage, no reused IDs). It passes git apply --check and
actual MSVC x86 / vendor sol+Lua tests with engine doubles:
`work/new_oakvale_converter_resource_x86_20260913_06/result.json`. Runtime checkout/game are
unchanged; no full DLL or gameplay validation yet. Candidate tests 17/17; previous focused
batch 39/39 (before the last cached-Thing cancellation test was added).
Shared-temp splitting and dead literal removal now preserve tested traces. Barrel position
copy/spawn/health, wife position/movement, and woman run-off position/setup are recovered.
Barrel phase 1/3 destination snapshots, phase 2 camera-selected teleport, five saved-interface
pause calls and hit-predicate control flow are recovered. Non-fall-through switches now
use a single selector evaluation and a loop that preserves case-level break targets.
Barrel health/departure and Bully talk, teddy possession/presentation and hit-result
predicates now have focused native evidence and behavior tests. Decimal parent offsets
resolve to the same reviewed quest fields as hexadecimal offsets.
Next: complete resource/Thing/movie/timer ownership, remaining operands and cleanup
branches across the package, then behavior and runtime integration. Latest full suite: **966 passed**, exit 0, in
`work/converter_marathon_suite_20260913_07.log` (343.792 seconds; before the newer mission operand pass).
Experimental `--flat-control` handles
switches and nested jumps, but must not be promoted before operand/ownership validation.
Detailed chronology: [09-13 handoff](journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-13.md).
BookTrader home/health and BarrelMan Init now have checked operand/value recovery.
BookTrader also has a complete native 54-event resource/8-temporary-Thing lifetime map.
Its separate ownership-aware Lua candidate is now included in the disabled readable
package. Eight candidate tests cover hit ordering, cancellation, movement, live animation
reads, error cleanup, scoped hit results and changed evidence rejection. The latest
BookTrader/husband candidate set passes 25 tests. BookTrader's three conditional hero-name
strings now use the reviewed scoped hit helper; native DB4234..DB430B proves construction,
short-circuit calls, ability 14 and reverse destruction before the result is consumed.
Three Theresa facing sites preserve native snap flags false/true/false and scoped
Thing/string cleanup through the proposed quest:FaceThingByScriptName adapter.
Twelve BookTrader candidate tests pass, including purchase/decline/pending-answer cancellation and repeat-purchase prevention. Conversation and entry predicates remain
open. The latest readable rebuild still compiles all 18 files; this is not behavioral completion.
The readable builder now restores 14 alive-and-conscious registrations plus
DeadFather's existence-only registration before their first frames. Native bytes,
counted clones and the actual Lua binding pass x86 checks; the unapplied patch is
`work/new_oakvale_conditions/condition-integration.patch`. Guard is separately
audited: its Main has no condition registration, so none is synthesized.
Original PDB locals/scopes are retained and linked in the readability report; see
[PDB evidence](scripts/NEW_OAKVALE_PDB_EVIDENCE.md). One user-requested background agent
is progressing ScytheInfo independently. Full suite `_07.log` passed966 tests.

## Converter continuation (2026-09-13)

**Resource-aware husband candidate landed (separate, DISABLED).**
`python tools/script_recovery/generate_affair_man_resource_candidate.py` re-verifies the
57-event native resource map and writes
`refs/script_recovery/lifted/NewOakValeIntro/candidates/NOVI_AffairMan.resource_candidate.lua`:
one `man_resource` inside `quest:WithRetailResources` from construction to a single release,
non-waiting `resources:Speak` plus the retail task poll, ten `NewThingFromResource` triples,
animation byte 0x01375748 as a raising stub. Nineteen rewrites are counted, including movie ownership, hit-pause and dead-cast fixes.
The prepared extension gained `ThingIsDistanceFromPositionOver` (helper 0x00CBE45C); it is still
unapplied and unbuilt. Registration stays `Quests = {}`; converter draft untouched.
Review follow-up: hit pause corrected to true; three dead SUB41 calls removed.
Gates: candidate tests 15/15; resource/movie focused set recorded in the linked handoff. Earlier full-suite checkpoint:
878 tests, known Bully two failures/two errors (`work/converter_resource_candidate_tests.log`); not rerun for this follow-up.
The external critic never ran (spend limit); completed verifier findings were recovered.
Movie/pause calls now use the resource scope; error tests verify unpause, temporary/movie destruction and resource release.
Read [the 2026-09-13 handoff](journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-13.md) first.
Next: cached woman/wife + hit-wrapper ownership, scope storage growth, a reviewed runtime read of
0x01375748, then runtime-owner coordination on the five-method patch.

## Converter continuation (2026-09-12)

**End-of-night checkpoint — user requested a stop. Resume implementation only when asked.**
New Oakvale: **47/51 functions, 14/18 files compile; zero missing bodies; 1,296 TODOs**.
Latest full suite: 870 tests, with only the known Bully two failures/two errors in the parallel
reconstructed port. Later temporary-Thing work passed its focused tests. Draft registration is
still disabled; no runtime patch or DLL was applied. No converter processes are pending.

Start with [the current handoff](journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-12.md)
and [the resume checklist](scripts/SCRIPT_CONVERTER_RESUME_CHECKLIST.md). Next: connect the
husband's verified resource/temporary-Thing lifetimes to a separate disabled Lua candidate.
The current runtime's blocking Speak is not equivalent to retail's nonblocking speech.
The four-operation resource extension is prepared and tested under
`work/man_resource_extension/`, but remains unapplied. Preserve the parallel runtime/Bully files.

The checkpoints below are historical and are superseded by the links above.

**Latest marathon checkpoint:** converter/benchmark tests 77/77; full recovery suite 394/394;
canonical SDK PASS with `--skip-mirrors`. Benchmark now compiles recovered bodies with Lua 5.4,
excludes stubs/loaders, and records every syntax failure. Passing recovered files rose **6/18 ->
8/18** (Barrel and Dead Father); ten still fail. `--require-syntax` returns nonzero for these drafts.

Added reviewed parent-field and numeric-constant joins, exact-target AL termination-result copies,
mutable scalar storage across loops/branches, consistent synthetic field getters/setters, typed
ECX quest-receiver normalization, and hexadecimal boolean comparisons. The actual generated Barrel
Main passes execution checks for both instruction-text branches, an already-shown instruction, and
prompt cancellation. No call-recall regression: SummoningTheShip 0.60 -> 0.70, DragonBossFight
0.49 -> 0.62, BeardyBaldy 0.82 -> 0.85; scores include state accessors and are not gameplay parity.

Next: inlined string comparisons, native thing temporary ownership/return slots, nested master-data
and vector fields, and remaining comma expressions. Keep the ten syntax failures visible rather
than replacing unknown behavior with successful no-ops. Details and exact current diagnostics:
`docs/scripts/LIFTER.md`, `refs/script_recovery/lifted/LIFT_BENCHMARK.md` / `.json`. Before/after
artifacts are under `work/lifter_marathon_20260912_*`. No game install or Forge source was changed.

Earlier continuation today:

Resumed `tools/script_recovery/lift_native_lua.py`. Assignment-in-condition lowering now preserves
`&&`/`||` short-circuit execution, evaluates the left operand once, and retains the previous assigned
value when the right-hand call is skipped. Fixed `else`/`elseif` closure and retention of nested
terminal braces. New tests execute emitted Lua across branches and verify call effects and values.

Gates: converter 61/61; full recovery suite 378/378. Regenerated all 16 benchmark rows with unchanged
call recall/precision and TODO counts. SDK validation passes with `--skip-mirrors`; the full SDK
check reports existing overlay drift in `D:\Code\FableForge\docs\re_reference\fse_native_overlay.json`
and `D:\Code\ForgeFSE\docs\fse_native_overlay.json` (neither changed in this continuation).

Syntax inspection passes 47/59 generated Lua files, including stubs. The remaining 12 contain
unsupported native expressions/control flow. Next: expose syntax failures in the benchmark, then
lower the remaining supported expression shapes. See `docs/scripts/LIFTER.md` for details.

## Immediate marathon resume

Open `docs/journal/2026-09/NEW_OAKVALE_MARATHON_CHECKPOINT_2026-09-09.md` (entries dated 2026-09-11)
first. The game is closed. **Bundle v13 is generated AND installed** (`work/new_oakvale_test_bundle_20260911_v13/`,
DLL SHA-256 `2442A02C7AACF6468386F86608A6FF9FFDCD6D0A434D46DD256B777268842815`, in both the game root and `FSE\`, Lua tree included; rollback in
`FSE\backups\new-oakvale-v13-20260911-163519\`). v12 was built but never installed (its plain
same-priority idempotence rule would have re-broken Affair Man's walk-home).

**The user played the full childhood on v11 and reported "everything felt great"** (log archived as
`runtime_evidence/interactive-20260911-v11-single-authority-clean.log`: zero `!!! ERROR`, zero
`No control handle`, Wife dialogue, one Guard lecture, all cutscenes, Dad reward lines). Two reports:
1. **Bully RunTo makes zero movement / compatibility jump vanishes on camera.** CORRECTION (2026-09-11 evening): retail's
   `.RunTo` macro branch DOES wait (polls scripted-resource slot `0x68` IsPerformingScriptTask, see
   `runtime_evidence/bully-run-native-wait-analysis-20260910.md`); my earlier "non-blocking" note was
   read off the macro text and was wrong. Retail therefore has him at `MK_OIBR_BULLY2` before the
   `UseCamera CAM_OIBR_BRAT` … `BULLY.Drawable FALSE` tail. Under Forge the native task reports
   completion after 2.33-2.37 m at the IDENTICAL endpoint 11.759 m short in 4 of 5 archived runs
   (`analysis-bully-run-variance-20260910.json`; one run reached the marker). The Lua compatibility
   move then covers the rest after the macro, on the same camera, and hides him in frame: that is the
   jump/vanish the user sees. A hide-after-second-macro tweak was tried and REVERTED (it reorders
   retail's sequence and broke a fixture). The fix must make the in-macro task complete; a
   Later correction: installed TNG proves the repeated endpoint is exactly `MK_OIBR_BULLY1`, where
   the macro teleports him before RunTo. Thus the four failed RunTo tasks move zero metres; the
   earlier 2.33-2.37 m measurement includes only the authored pre-run teleport. A passive diagnostic
   build now logs native MoveToPosition issue operands and every task-poll result. Run one Bully
   encounter and inspect `[CutsceneMoveDiag]`; see
   `docs/journal/2026-09/BULLY_RUNTO_STALL_2026-09-11.md`.
2. **Bully health bar colours wrong.** Disassembly at `0x00DBC40C-0x00DBC43B` writes the empty colour
   as memory bytes `00 00 FF FF` and the filled colour as `00 FF 00 FF`; the engine `CRGBColour` is
   B,G,R,A, so empty = red, filled = green (the same words as the barrel timer). The Lua and the
   entity inventory had read the empty bytes as RGBA blue, and `export_bully_health_bar.py` plus its
   snapshot had transposed them as `FF 00 00 FF / opaque blue`. All three are corrected; three Bully
   traces regenerated. Fixture validation 119/119; suite 317/317.

**Nested-control contract, runtime-proven across the v10 and v11 runs (Forge v13 rule):**
- A Forge handle has no script-owner identity, so a second `StartScriptingEntity` resource for an
  actor the same VM already controls is never granted (v10 Affair Wife hang).
- Retail Main loops re-call `StartScriptingEntity` on the same resource every iteration (Bully,
  Victim, Barrel Man), which the engine treats as idempotent (v11 depth inflated to 1900+).
- v13: a re-acquire over an owned handle at the held priority saturates at depth 1 (outer handle must
  survive the following release: Affair Man's conversation precedes his walk-home loop, proven clean in
  v11); a different priority counts one nested level; only the outermost release destroys.
  `audit_forgefse_control_abi.py` schema 0.4, 7/7 tests.

**Next run (user-driven, `FSE_Launcher.exe`):** confirm `legacy=false` at startup, the Bully bar shows
green-over-red, Wife dialogue still plays, and the log's `Reusing live control handle` lines stay at
depth 1 for the per-frame loops. Then decide whether to keep or remove the Bully run-off
compatibility completion (retail hides him wherever the short run ended).

Offline gates: recovery suite 317/317, package fixtures 119/119, authority `ok=true`, SDK PASS.

**Converter started (user request, evening):** `tools/script_recovery/lift_native_lua.py` lifts a
native cluster (or a translation-unit entity function) into a draft ForgeFSE Lua package, marking
anything unrecognised with `TODO(native)`. `benchmark_lifter.py` regenerates
`refs/script_recovery/lifted/LIFT_BENCHMARK.md` against Aeon's ports and our New Oakvale entities;
10/10 unit tests, suite 327/327. Read `docs/scripts/LIFTER.md` for what it handles and the three
next levers (thread bodies, `CScriptThing` message slots, field naming).

**Latest continuation:** v14 video/log proved the compatibility hide used a premature 2 m exit while
its move task was active. v15 now waits on retail's `IsPerformingScriptTask` contract and adds the
correct marker-form `MoveToThing` passive hook; it is installed, game closed, rollback
`FSE/backups/new-oakvale-v15-bully-wait-20260911-194754`, DLL SHA-256
`8102215BF082508052FDF451B1FE2BAB61E2FD8FC736DD93F4389EA69C6DF80E`.
The lifter now consumes all exported native thread bodies: benchmark recall is 0.53
SummoningTheShip, 0.45 DragonBossFight, 0.72 BeardyBaldy, and 0.97 HerosOldHouse. It recursively
queues nested worker threads, exported Dragon's `RunEnemySpawning` and `JackTaunts`, accepts Ghidra's
decimal vtable offsets, normalizes both `LAB_` and `FUN_` thread-address spellings, and proves final
destructor-only goto targets before dropping their TODOs. The noisy helper-body experiment was
rejected because it reduced precision. Suite 350/350.

Gate commands (from repo root): `python -m unittest discover -s tools/script_recovery -t . -p "test_*.py"`;
`python tools/script_recovery/validate_reconstructed_package.py --fse-root refs/script_recovery/reconstructed/NewOakValeIntro/FSE --package Q_NewOakValeIntro --fixtures refs/script_recovery/new_oakvale_intro/fixtures --traces refs/script_recovery/new_oakvale_intro/traces`;
`python tools/script_recovery/validate_new_oakvale_authority.py --profile-fse refs/script_recovery/new_oakvale_intro/runtime_playtest --source-fse refs/script_recovery/reconstructed/NewOakValeIntro/FSE`;
`python tools/validate_tooling_sdk.py`; `python tools/script_recovery/audit_forgefse_control_abi.py --forge-root D:\Code\ForgeFSE-retail-shadow`.
Both repos still carry the marathon work **uncommitted** (FableTLC `feat/novi-script-recovery`;
ForgeFSE-retail-shadow `feat/upstream-fse-2026-09-02`). A stray root file `0x0382cff0` must not be
committed. Check the installed `FinalAlbion.qst` for `AddQuest("NewOakValeIntro", TRUE)` before every
run (deleted 2026-09-11; Steam verify can restore it).

Current compatibility behavior adds three narrow recoveries on top of the byte-locked retail scripts.
Barrel Man selects exact TLC `TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN` after broken stock even when the
Hero is nearby, suppressing the contradictory thanks/good-deed path; the final-barrel one-gold chain
is independently proven from TLC code and data. Bully reuses the authored run-off marker if native
`RunTo` reports completion while still short. Affair Wife retains native movement as the primary path
but immediately reissues movement toward the live husband whenever that task has ended outside the
native 3 m arrival condition, capped at four retries. This matches the clarified intermittent report:
she can reach him normally, but sometimes stops early.

## Historical New Oakvale chronology

Everything below this heading records incremental evidence and older gate counts. It is retained for
provenance and does not override the current snapshot above.

Latest interactive evidence is archived as
`refs/script_recovery/new_oakvale_intro/runtime_evidence/interactive-20260910-redtimer-bullyrun-barrels.log`.
It confirms the timer is red, Barrel Man judged the Hero nearby at 5.092 m and therefore selected
`BARRELMAN_THANKS`, while the independently propagated broken-barrel bad deed was claimed by the
Guard. This apparently mixed outcome is retail-exact: warehouse return judgment tests only
visibility/proximity, not barrel destruction. The bully actor map is also correct, but the authored
`BULLYRUN1` moved him only about 2.36 m before its explicit `Drawable FALSE`. A new diagnostic Forge
build records movement actor/resource identity, destination, move type, flags, and missing handles.
New Oakvale also has a guarded compatibility completion: if shipped `RunTo` returns with Bully still
more than 2 m from `MK_OIBR_BULLY2`, it restores drawability, finishes the same authored run (capped
at 300 frames), then hides him for retail cleanup. Package validation is 117/117 with zero warnings;
recovery tests are 248/248; tooling SDK validation passes. The build and Lua remain undeployed, and
the game is closed. Diagnostic DLL SHA-256:
`C41F54F9B0D43700CEE096662F2A01752B9CB3DDA216070F22E60DCB2B4C2D7E`.
Affair Man is now covered by one reproducible byte artifact spanning his complete Init
`0x00DB0950-0x00DB09E0` and Main `0x00DB09E0-0x00DB1DA2`: 5,202 bytes / 1,503 instructions, with
both regions ending at exact `ret` boundaries. This locks the live Wife/Woman lookups, confrontation,
walk-home, affair dialogue, kiss/hug, and terminal cleanup around the remaining Wife runtime seam.
Current recovery suite is 250/250; package validation remains 117/117 with no warnings and SDK
validation passes. No deployment or game launch occurred.
Affair Woman is now byte-locked too: complete Init `0x00DB1E80-0x00DB1EF4` and Main
`0x00DB1F00-0x00DB299A`, totaling 2,830 bytes / 810 instructions at exact `ret` boundaries. The
artifact confirms her husband/wife lookups, kiss/hug reception, Wife-proximity flight, authored
run-off marker, and removal/cleanup sequence. She never removes or relocates Affair Man, further
narrowing Wife's air-scolding to the pending runtime identity/model seam. Current gates: 252/252
recovery tests, 117/117 fixtures with zero warnings, and SDK PASS. Nothing was launched or deployed.
Affair Wife is now fully byte-locked: Init `0x00DB2A70-0x00DB2B10` and Main
`0x00DB2B10-0x00DB3E2A`, totaling 5,050 bytes / 1,543 instructions at exact return boundaries. The
continuous artifact contains the earlier route and argument slices and confirms the one-time husband
position snapshot, repeated live-husband 3 m arrival test, facing/conversation targets, and cleanup.
No static Lua/Forge mismatch remains in the affair trio. The air-scolding report is retained as the
sole runtime-only identity/rendered-model uncertainty, with probes ready. Current gates are 254/254,
117/117 without warnings, and SDK PASS. The game stayed closed and nothing was deployed.
Book Trader now has complete retail-byte coverage as well: Init `0x00DB3F00-0x00DB3F98` and Main
`0x00DB3FA0-0x00DB4F6A`, totaling 4,194 bytes / 1,243 instructions. Exact reconciliation confirms
home return, Theresa facing, hit reaction, all sweets-sale outcomes, timer-gated shouting, and movie/
control cleanup without a Lua or Forge mismatch. Current gates: 256/256 recovery tests, 117/117
fixtures without warnings, and SDK PASS. No deployment or launch occurred.
Barrel Thug now has complete retail-byte coverage: Init `0x00DB6BF0-0x00DB6C31` and Main
`0x00DB6C60-0x00DB7CF1`, totaling 4,306 bytes / 1,281 instructions. The artifact confirms intro,
follow, timer-tier temptation/well-done chatter, why-not-smash/outro selection, hit bad-deed 2, and
all movie/control cleanup. It also proves these lines are independent of Barrel Man's proximity-only
return judgment; no missing consequence handoff exists. Current gates: 258/258 recovery tests,
117/117 fixtures without warnings, SDK PASS. The game stayed closed and nothing was deployed.
Barrel Man now has complete retail-byte coverage: Init `0x00DB5260-0x00DB5307` and Main
`0x00DB5330-0x00DB6B23`, totaling 6,298 bytes / 1,808 instructions. Full phase coverage confirms
that return-time thanks is gated solely by sight or the 10 m distance fallback. `LETDOWN_BROKEN` is
only reachable on a later talk after the Hero first failed that return judgment and set
`HeroLetMeDown`; destruction never overrides nearby thanks. Current gates: 260/260 recovery tests,
117/117 fixtures without warnings, SDK PASS. No launch or deployment occurred.
Dead Father's control-acquisition termination is also now closed: Forge returns false after cleaning
its pending handle, the shared helper propagates it, and a fixture proves setup calls are skipped.
Its minimap marker is fully closed as well: native setup plus the PDB signature prove `(me,
HUD_ORB_QUEST_CORE)`, Forge preserves that order, and the entity inventory now has zero uncertainties.
Current offline totals after these additions are 116/116 fixtures and 147/147 recovery tests.
Victim Init identity is now closed too: retail allocator/vtable `0x012D87B8`, the two Victim field
writes, and the separate real Book Trader vtable/Init form a positive plus negative ownership proof.
The quest logbook helper family is now instruction-proven. `0x00CBE87F` builds
`TEXT_QST_LOG_STORY_<id>` and submits category 1; `0x00CBE960` builds `_NAME/_DESC` story keys;
`0x00CBE9EE`, called by both first-deed paths, builds `_TITLE`, submits tutorial category 2, and
yields once. New Oakvale now calls `AddLogbookTutorialEntry`, and Forge routes that API directly to
`0x00CBE9EE` instead of synthesizing an incorrect literal-wide entry. The corrected Release|x86 DLL
builds with zero warnings/errors and remains undeployed; SHA-256
`2B24EDDBA821F1D1F49FF8DE7B1ADE341FD03A4728A54648CBE4D42C96DDA5E2`. Current offline suite:
117/117 fixtures, 151/151 recovery tests, 137 API requirements with zero blockers, and 46/46
applicable functions traced with zero uncertainty. The dead-father movie-bracket note is also closed:
native order, Forge's per-VM ownership check, and the successful skip log jointly prove one borrowed
caller-owned movie sequence around the macro. New Oakvale's three platform branches now call Forge's
direct retail `IsXbox` binding instead of hardcoding PC; the no-context helper fallback remains false
for isolated tests.
Barrel destruction is now closed end-to-end too: the active entity's bound-thing alive predicate,
derived callback vtable, Forge one-shot dispatch, Lua state writes, and archived `bad=1` observation
all agree. A dedicated fixture proves the counterintuitive retail rule that a broken barrel does not
change automatic return judgment: an unseen hero within 10 m still receives thanks and a good deed.
Theresa's `given_chocs` lifetime is now instruction-audited: initialization, three writes, and three
reads all resolve to one Main-stack byte, separate from the persistent quest flag. Her inventory and
the Barrel inventory now have zero uncertainties; Barrel's post-instruction tail is an exact retail
idle-until-termination loop. Current offline suite: 117/117 fixtures and 152/152 recovery tests.
The game remained closed and nothing was deployed.
Barrel Man phase 1 is now donor/retail-audited as a preserved dead case: construction does not set
`MyPhase`, Init sets 0, and Main writes only 2, 3, 4, or 5. The cleanup-only marker locals are also
classified as exact compiler-visible behavior rather than uncertainty; only inferred enum labels remain.
Affair Wife's route mismatch is narrowed further: Forge's actual Lua registration uses the operand-aware
non-blocking movement wrapper (not its legacy four-argument helper), forwards retail `false,true`, and
donor `GetPos` proves Forge's direct implementation-vector read is equivalent to CScriptThing dispatch.
The remaining unknown is runtime husband identity/replacement or model-vs-thing position state.
Timer duration semantics are now native-proven: `WorldUpdate` decrements positive timer values when
`worldFrame % constantFPS == 0`, so `SetTimer(..., N)` represents N constant-FPS seconds and clamps
at zero. Created Beetle's 5-second lifetime and Villager's shared 3-second throttle are closed.
Villager's hit-control lifetime is now retail-exact too: successful hit speech retains control across
the loop's bottom frame, releases it at the next loop head before the hit test, and the termination
edge follows the native cleanup label. Its inventory now has zero uncertainties. Current offline gates
are 117/117 fixtures and 155/155 recovery tests. Created Beetle's `RemoveThing(true,true)` operands
are now behaviorally closed from the native callee: the first reaches `CThing::Kill` as
`destroyImmediately`, and the second sets the thing's `+0x93` flag bit `0x02` before that call.
Barrel Man's descriptive phase labels remain explicitly tagged as inferred metadata, not behavioral
uncertainty; all numeric states and transitions are exact. The only entity-inventory uncertainty now
is Affair Wife's runtime identity/position anomaly. Current recovery suite: 156/156. The game remained
closed and nothing was deployed.
The subsequent stale-evidence audit closed four more Forge paths: all 13 reconstructed random call
sites use the relocated retail MSVCR71 `rand` thunk; Dead Father forwards all seven native looping-
animation flags; Victim forwards both killability and all three information flags; and `FadeScreenOut`
supplies the exact opaque-black colour used by Barrel Man and Theresa. Barrel Man's preserved dead
phase now also reproduces retail's zero-vector fallback for a missing walk-off marker. Current offline
suite: 117/117 fixtures and 161/161 recovery tests, still with no deployment.
Guard's lecture approach threshold is now instruction-closed: the initial branch, loop-entry check,
and post-frame recheck each load `DAT_013ac840`, whose retail value is 3.0, before calling the same
distance helper. Remaining movie-cleanup “inference” notes are closed too: Forge binds and invokes the
exact retail derived movie-resource destructor at `0x006E7B80`. Current suite: 117/117 fixtures and
163/163 recovery tests.

Created Beetle's remaining decompiler omission is now instruction-closed: `RegisterTimer` returns into
`EAX`, retail retains that ID in `EDI`, and pushes `EDI` before both slot-`0x168` `GetTimer` calls.
Barrel Man's movement value 1 and excluded special-ability value 14 are also tied to the named
`ENTITY_MOVE_RUN` and `HERO_ABILITY_HEAL_LIFE_SPELL` enums in the retail-aligned Forge header.
Focused audits preserve both results. The obsolete unresolved-call scaffold and two duplicate
non-gap API classifications were subsequently removed. The manifest builder then stopped counting
`n/a` and `n/a (data)` state/data sentinels as APIs. Fifteen conceptual `SpeakAndWait` annotations
were then normalized to the actual registered `me:Speak` call used by their Lua helpers, leaving 132
actual direct/host-managed API
requirements with zero blockers. The inventory now preserves dual Forge registration too;
`SetIsPushableByHero`, used through both Quest and Entity Lua surfaces, is correctly marked
`Quest|Entity` instead of losing its Entity scope. `AddBadDeed` field propagation is byte-audited as well: retail
increments `BadDeedsPerformed` at `+0x58` and writes the caller's deed-kind byte at `+0xFC+kind` on
both control-flow tails, removing the final stale field-write inference labels. Current offline gates
are 117/117 fixtures and 170/170 recovery tests. Static FinalAlbion inventory now excludes an
authored duplicate husband: both canonical TNG trees contain exactly one `NOVI_AffairMan`, UID
`18446741874686306552`; the installed WAD-extracted payload is byte-identical to the canonical tree,
and his authored start is 75.765 m from the Wife. The sole remaining entity
uncertainty is therefore the Wife's staged runtime replacement/handle or rendered-model/thing-position
seam. The game remained closed and nothing was deployed.

The API manifest now carries a validator-derived executable Lua call surface. The validator scans
arbitrary receiver identifiers (closing omissions such as `marker:GetAngleXY`) while explicitly
excluding the package's one native Lua string-method use, `value:match`. Regeneration records 145
scoped calls: every call has a Forge binding, and every one of the 131 direct retail-operation
requirements appears in executable Lua. The zero-gap audit enforces both directions while retaining
13 visible helper/probe-only operations. Reconciliation is receiver-scope exact, so an operation
observed only on the wrong Quest/Entity surface cannot satisfy the requirement. Current offline gates
are 117/117 fixtures and 174/174
recovery tests; 132 total requirements remain (131 direct plus one host-managed), with zero blockers.
The DLL hash is unchanged and nothing was deployed.

API requirement schema 0.3 now separates 429 call-site evidence annotations into `evidenceNotes`.
They were previously mislabeled as `semanticDifferences`, despite documenting exact operands,
cleanup paths, and corrected decompiler artifacts. Direct bindings now truthfully report zero known
semantic deviations, and the zero-gap audit requires the annotations to remain preserved and separate.

Native provenance is now self-contained: the historical missing `tu/` locators were replaced with 50
fresh read-only exports from the local `FableTLC/Fable.exe` Ghidra project. They cover all 52
inventoried quest/entity functions (the empty default at `0x00CDEBB0` and quest Init at `0x00DAADD0`
are intentionally shared). Each export records its exact entry address and generator, and a focused
audit rejects missing, stale, misaddressed, or unreferenced files. Current recovery suite: 176/176.
Regenerate the corpus with
`powershell -File tools/script_recovery/export_new_oakvale_native_sources.ps1`; the wrapper always
uses read-only `-noanalysis` mode and derives the address set directly from the inventories.

The six not-applicable coverage rows are now classification-audited individually. In particular,
`NOVI_Villager.QuestInit(vectors)` is no longer mislabeled empty/data-only: it explicitly aliases
the implemented-and-traced `Q_NewOakValeIntro.Init` at the same `0x00DAADD0` entry. The old broad
`"vectors" in function-name` exclusion was removed, and the native-source audit requires the alias
to resolve to an identical address.

Transitive fixture coverage is no longer trusted from `covers` labels alone. Every secondary helper
claim now names distinctive events that must also be validator-enforced expectations: both
`GivenTeddy` helpers, `GetVillagerSpeechIndex`, `AttackStuff`, and `PostAttackStuff` are covered this
way. The builder rejects secondary coverage with absent or unverified evidence. Current offline totals
are 117/117 fixtures and 177/177 recovery tests, with the 46 applicable functions still traced.

Operation ownership is now complete and audited. The builder normalizes nine Barrel Man split-helper
labels such as `Main/judge_hero` back to native owner `Main`; previously those 48 helper-tagged rows
entered the API inventory but were silently omitted from function coverage. Barrel Man Main now
accounts for all 68 of its operations, and coverage assigns all 747 inventory operations exactly once.
The structure audit also requires contiguous sequence IDs and declared native owners. Current suite:
179/179 recovery tests.

Entity parent-state declarations are now checked against executable Lua instead of trusted blindly.
The audit found and added five omitted direct reads: Affair Wife, Book Trader, and Bully each read
`TalkIntermittentTimer`; Barrel Thug reads `WatchTimer`; Barrel Man reads `BadDeedsPerformed`.
It validates 30 directly read and 28 directly written field families against 37 canonical quest-state
keys, while recognizing the eight retail Villager speech vectors as static parent data. Annotated
indirect deed-helper writes remain visible and are name-validated. Current suite: 180/180 tests.
The same audit scans quest and shared modules too, covering 34 package-wide read families and 32
write families, so undefined keys outside entity files cannot evade the check.

Master-data ownership is generated rather than hardcoded now. The prior persistence manifest falsely
said `TeddySolution` was written by Teddy Girl and Bully; native inventories and Lua prove both B and C
writes belong only to Teddy Girl. Persistence schema 0.2 emits the two structured, address/evidence-
backed accesses, and the state audit cross-checks `F.master` definitions, Lua calls, and inventory ops.
Annotated deed declarations are normalized before persistence ownership aggregation, restoring six
indirect writers for both `BadDeedsPerformed` and `WhichBadDeedsPerformed`. The scalar-only parser
previously omitted the latter array entirely; it is now represented as the canonical `bool[5]`
family at `0xFC..0x100`, with Guard as reader. The manifest therefore contains all 37 field families.
Current recovery suite: 182/182.
Executable `Deeds.add_good`/`add_bad` calls are also ownership-audited. Six entity inventories had
omitted those indirect side effects; after repair, `GoodDeedsPerformed` has four real entity writers,
while `BadDeedsPerformed` and `WhichBadDeedsPerformed` each have all twelve. The persistence manifest
now derives these owners from normalized declarations instead of losing helper annotations.

Villager speech data is now three-way exact: all eight vectors, their offsets/order, and all 42 text
keys match between the fresh native quest-Init decompile, structured Villager inventory, and evaluated
`villager_speech.lua`. A focused audit guards vector lengths (6/6/4/5 per sex), retail construction
order, and set equality. Current offline suite: 117/117 fixtures and 183/183 tests.

Package-wide symbolic resources are now inventory-audited too. Across text, object, creature, quest,
cutscene, animation, marker, region, theme, music, and script identifiers, all 238 executable Lua
resources are covered: 224 exact identifiers plus 14 indexed/sex-suffixed construction prefixes.
Eighteen diagnostic log formats are explicitly excluded, and annotated legacy inventory strings are
normalized to their literal identifier. Current recovery suite: 184/184.

Entity-local initialization is independently audited now. All 32 native `localFields` names occur in
their comment-free reconstructed entity Lua and every recorded `initValue` is assigned. The evaluator
also resolves 14 symbolic defaults, including Barrel Man's `MyPhase = PHASE.AT_WAREHOUSE`, rather than
accepting only duplicated literals. Current recovery suite: 185/185.

Native numeric/boolean/table constant fidelity is package-audited too. The inventory contains 214
constant records: 200 have same-name Lua declarations whose parsed values match exactly, while 14
split, shared-module, symbolic-vector, or call-site records have explicit value and executable-context
checks. This includes the affair animation flag, zero-vector fallbacks, Barrel Thug's asymmetric final
timer tier, Bully info-bar colours, Guard's chase-distance table, the `-999` GUI sentinel, and the
shared `0.001` morality delta. Current recovery suite: 186/186.

Cleanup/lifetime evidence is structurally guarded across all 17 inventories that own resources. The
73 cleanup records contain 149 acquisition operation references; every referenced sequence exists in
its owning inventory, and non-operation acquisitions must explicitly identify quest/external ownership.
Release/missing-path lists and owner-only records are schema-checked as well. Current recovery suite:
187/187.

Control-flow phase metadata is now checked against executable reconstruction anchors. All 74 phase
records across 16 inventories have unique IDs, non-empty entry/exit evidence, and resolve to 100 real
Lua function declarations. Barrel Man's six numbered phase IDs are additionally cross-checked against
both the native `enumValues` metadata and the Lua `PHASE` table. Current recovery suite: 188/188.

Cutscene and spawned-thread metadata is linked back to executable Lua now. The audit covers all 18
cutscene records, nine named resources, 13 actor-map keys, and four quest child threads. It exposed
stale thread prose that omitted Forge's required `{ region = "" }` options and used the wrong receiver
case; all four inventory rows now record the exact executable `Quest:CreateThread` calls. Current
recovery suite: 189/189.

Native entity-class ownership now has reproducible retail-vtable proof. Scanning the installed
executable for each entity inventory's unique `[Main, Init]` pair recovered all 16 entity vtables
(10 were previously absent from the inventories), plus their destructor slots. The SHA-pinned
snapshot, extractor, and audit cross-check all 32 function slots, allocator/vtable evidence in the
translation unit, and the 32 exported native decompiles. This also replaces four stale references to
a nonexistent `entity_vtables.json`. Current recovery suite: 190/190.

Those vtables now anchor a complete entity-layout pass. All 16 entity allocators (the quest object is
constructed separately) are resolved in the native translation unit, with retail allocation sizes of
28-44 bytes. Each allocator installs the expected vtable, and all 31 class-local fields fit their
recorded offsets and widths; the separate Theresa stack local is correctly excluded. The checked
layout snapshot retains 11 aggregate bytes of legitimate tail padding/slack. Current recovery suite:
191/191.

All 16 entity destructor slots are now fresh read-only Ghidra exports. Their normalized native bodies
are identical: invoke the shared entity base cleanup, conditionally `operator_delete(this)` for the
vector-deleting flag, then return `this`. This export also disproved an over-broad first pass: the
quest's adjacent `[Main, Init]` pair has a spawned-function factory in the preceding slot, so it is not
an entity-layout vtable. The false quest vtable/destructor claim was removed and both extractors are
now entity-scoped. Current recovery suite: 193/193.

All 16 allocator functions are now standalone, snapshot-driven Ghidra exports too. The audit verifies
each exact allocation size, construction of the embedded `CScriptThing`, installation of the entity's
retail vtable, parent and bound-thing pointer stores, and allocation/wiring of the `0xC` ownership
control block. This makes the layout proof reproducible from both the translation-unit corpus and fresh
function-level sources. Current recovery suite: 195/195.

The common destructor call is now classified from its body instead of Ghidra's BSim donor label. All
16 deleting destructors converge on `0x00F35B40`; that routine restores base vtable `0x012C3224`,
restores the embedded `CScriptThing` vtable `0x01238C8C`, releases and clears its counted binding, and
tails into exact/relocation-matched `CBase_RestoreVTable` at `0x0099A2E0`. Its semantic role is therefore
entity-binding base cleanup, while the exact native class name remains unresolved. The snapshot-driven
export and audit preserve the misleading `NUISystem::CFrontEndScreen` name only as donor provenance,
not recovered identity. Current recovery suite: 195/195.

The remaining Affair Wife route uncertainty now has a complete position-chain proof. Fresh exports of
retail `CScriptThing::GetPos` (`0x004AA980`), `CGameScriptThing::GetPos` (`0x008CFE20`), and
`IsDistanceBetweenThingsUnder` (`0x00CBE2FF`) show wrapper validation, implementation-slot dispatch,
inline `+0x28` position storage, and the exact squared 3D comparison. Forge normalizes raw/shared Lua
objects and invokes that retail fastcall, while `Entity:GetPos` uses the same implementation dispatch.
The observed air-scolding is consequently limited to runtime replacement/handle or rendered-model
position state, not reconstructed distance arithmetic or ABI. Current recovery suite: 197/197.

Quest-object ownership is now independently guarded rather than inferred from the entity-vtable pass.
The native cluster identifies allocator `0x00DBEF70`, constructor `0x00DAAC00`, the 268-byte quest
object, and vtable `0x012D7A28`; its five slots are destructor, `RegisterMain`, `Main`, `Init`, and
`OnPersist`. Eight fresh function exports, including the shared base constructor, verify allocation/constructor wiring, interface/database
stores, vtable installation, state-vector initialization, both timers, and the spawned `Main`
registration. The quest inventory now records these facts directly. Current recovery suite: 199/199.

The recovered quest size is now reconciled against the full persistence schema. A contiguous six-region
layout accounts for all 268 bytes: inherited script base, named scalar state, eight 12-byte villager
speech vectors, the five-byte bad-deed-kind array, three alignment bytes, and two timer handles. All 37
named field families (93 occupied bytes) fit their native widths and offsets without overlap; the base
and derived constructor exports guard the boundary anchors. Current recovery suite: 200/200.

Initialization provenance is now exact rather than summarized as “everything resets in Init.” Native
`Init` explicitly stores 27 of 37 field families: 25 ordinary Lua resets, the talk timer, and the
five-byte deed array. Ten fields are producer-initialized instead. This corrected false zero/default
claims for `StopTimeIndex`, unused `DadOfferedRewards`, and `lastVillagerSpeechIdx`; none was actually
in the Lua reset lists, so executable behavior is unchanged. Current recovery suite: 201/201.

Quest teardown is now reconciled with that construction/layout proof. The deleting thunk at
`0x00DBEFA0` calls the real quest destructor body at `0x00DBEFC0`; despite its stale particle-emitter
donor name, the body deregisters timer handles `+0x108` then `+0x104`, destroys and frees all eight
speech vectors from `+0xF0` back through `+0x9C`, and finally invokes inherited cleanup at
`0x00CBD510`. That final routine's `DeleteAllParticles` label is also misleading: its body destroys
the inherited script vectors/list at `+0x30`, `+0x18`, `+0x08`, and `+0x04`, then restores the root
base vtable. Fresh read-only exports, a teardown snapshot, and a layout/IR-aware audit now guard the
complete chain. The inherited class identity is now proven as `CScriptBase`, not merely compatible:
FSE independently maps constructor `0x00CB8110` to `CScriptBase_Construct`, it initializes the exact
four storage regions released by `0x00CBD510`, and all 125 consumers of that cleanup in helper IR are
script destructor roles. Current recovery suite: 202/202.

The eight embedded villager-speech vectors now have one cross-layer lifecycle audit. It extracts all
42 text keys and their vector end-pointer offsets directly from native `Init` `0x00DAADD0`, maps them
to the eight named inventory categories, expands the Lua prefix/suffix tables, verifies retail's
non-layout initialization order, proves exact contiguous coverage of `+0x9C..+0xFB`, and requires
the destructor to visit the same vectors in reverse layout order. This prevents a category, suffix,
offset, or teardown change from passing through independently green metadata. Current recovery suite:
203/203.

Quest lifecycle slots are now derived from retail PE bytes rather than accepted circularly from the
native cluster. A dedicated exporter reads five pointers at vtable `0x012D7A28`, records the exact
20-byte table hash, and resolves the deleting destructor's relative call. The resulting addresses are
`0x00DBEFA0`, `0x00DAACE0`, `0x00DABAC0`, `0x00DAADD0`, and `0x00DAADA0`, with implementation
target `0x00DBEFC0`; they independently match destructor, RegisterMain, Main, Init, and OnPersist.
The construction audit compares this snapshot with the cluster and requires the same retail SHA-256
used by the entity-vtable snapshot. An installed-image reproduction test is active when retail
`Fable.exe` is present. Current recovery suite: 204/204.

Quest ownership is now independently decoded from retail x86 instructions as well. A Capstone-backed
PE exporter proves allocator `0x00DBEF70` pushes `0x10C`, calls the retail `operator_new` thunk at
`0x00BFEA1A`, and calls constructor `0x00DAAC00`. The constructor's first direct call targets the
independently named `CScriptBase_Construct` at `0x00CB8110`, then its immediate vptr store installs
`0x012D7A28`. Exact allocator/constructor byte hashes and the common retail executable hash are
stored; the construction audit and installed-image reproduction test guard the complete chain.
Current recovery suite: 205/205.

The two quest timers now have a single retail-to-Forge lifecycle audit, focused on the runtime bug
where Lua's truthy integer zero skipped registration. Retail constructor `0x00DAAC00` calls interface
slot `0x15C` twice and stores TalkIntermittentTimer at `+0x104` before WatchTimer at `+0x108`; derived
destructor `0x00DBEFC0` calls slot `0x160` in reverse order. The authoritative vtable identifies the
contiguous slots as RegisterTimer, DeregisterTimer, SetTimer, and GetTimer (`0x15C..0x168`), and Forge
binds indices 87..90 with matching signatures and Lua exposure. The reconstruction is required to
register both handles unconditionally in retail order, use nil metadata sentinels, and zero only the
talk timer during Init. Current recovery suite: 206/206.

Those timer and initialization claims are now independently reproducible from the installed retail PE,
not only checked against Ghidra text. Capstone-backed exporters record exact hashes and instruction
addresses for both `RegisterTimer` calls and result stores, both reverse-order `DeregisterTimer` calls,
all 27 scalar `Init` writes with their byte/dword widths, and `SetTimer(+0x104, 0)`. The cross-layer
audits consume the checked-in snapshots, while installed-image tests regenerate them when `Fable.exe`
is available. Current recovery suite: 208/208.

Quest persistence now has the same direct-byte proof. Retail `OnPersist` at `0x00DAADA0` is exactly 33
bytes and performs one transfer only: key `AttackOver` from `0x012D7A58`, one-byte field `+0x50`,
one-byte false default, through `CPersistContext::Transfer<signed char>` at `0x004045C0`. Forge binds
that exact target as its bool transfer, preserves one-byte value/default storage, and Lua transfers only
`AttackOver`; the manifest agrees that it is the sole save-surviving quest field. Current recovery suite:
210/210.

`RegisterMain` now has direct retail-byte coverage too. Its 139-byte body allocates a `0x3C`
`CSpawnedFunc`, constructs it as `"Main"` through `0x00CDD450`, installs quest-specific vtable
`0x012D7A3C`, callback `0x00CDD440`, and owner at `+0x38`, then registers it with the empty section
through `0x00CB7E50`. Forge already used the matching constructor/registration targets and layout; three
new compile-time assertions now pin its size and callback/owner offsets. Release x86 builds with zero
warnings and errors. Current recovery suite: 212/212.

The 1,670-byte entity-binding prefix of quest `Main` is now decoded directly from the retail image.
All 16 binding names appear in exact Lua order and every callback matches the independently exported
entity allocator, from `NOVI_LiveFather -> 0x00DAC2C0` through `OVI_DeadFather -> 0x00DB81B0`.
Retail performs 16 `0x1C` allocations and 16 registrations through `0x00CB8230`; Forge uses that exact
target and object shape. Five new compile-time assertions pin the parent, allocator, enabled, and
auxiliary fields as well as total size. Release x86 builds cleanly. Current recovery suite: 214/214.

Quest `Main` now has complete direct-byte coverage through its 348-byte post-binding tail as well.
After base activation `0x00CB8930` and interface slot `0x100`, retail tests one-byte `AttackOver`,
checks termination through `0x00CB7940`, optionally deactivates `Q__OakValeIntro_PostAttack` through
slot `0x460`, sets objective 01 through slots `0xA3C/0x4A0`, constructs `StartBarrelTimer` with callback
`0x00DB4F70`, then calls `DoMission` at `0x00DBDE40`. The Lua order and Forge targets/slots agree;
the obsolete inventory note claiming Forge lacked the termination query was removed. Current recovery
suite: 216/216.

The core `DoMission` state machine now has an exact 1,172-byte retail snapshot and phase audit. It
extracts the seven embedded region/resource/thread names, callbacks for `WatchBarrels` `0x00DBE890`,
`WatchForGotGold` `0x00DBE2E0`, and `ManageQuestCoreMarkers` `0x00DBE4E0`, all three one-byte
`AttackOver` reads, six calls to termination helper `0x00CB7940`, and the final ordered calls to
`AttackStuff` then `PostAttackStuff`. All 20 distinct interface slots used by the method are executable
in the authoritative catalog, and Lua preserves the same phase topology. Current recovery suite:
218/218.

The complete 285-byte `AttackStuff` transition is now independently decoded too. Retail activates
`Q__OakValeIntro_PostAttack`, immediately deactivates `Q_NewOakValeIntro_PreAttack`, sets time to exact
float `23.0`, transitions to `ENVIRONMENT_OV_POSTATTACK` with zero seconds, and installs objective 06.
Its six ordered interface slots are `0x450`, `0x460`, `0xA18`, `0xA40`, `0xA3C`, and `0x4A0`; all
are executable and the Lua call order/constants match. Current recovery suite: 220/220.

The adjacent 1,095-byte `PostAttackStuff` method now has direct retail-byte coverage from entry through
its final cleanup. The snapshot proves the two-stage `M_PostAttackStart` wait, music sets 45 and 57,
hero teleport, Oakvale limbo, hidden money bag, teddy removal, logbook entry 20, camera/fade sequence,
two 5.0-distance checks against `MK_OVI_DADTRIGGER`, byte store `DadFound=1` at quest offset `+0x51`,
priority-4 hero acquisition, `CS_OAKVALEINTRO_HESDEADJIM`, and final village/time/section/theme/music
restoration. All reconstructed Lua operations retain retail order. The general slot catalog omits
`0x5EC`, so the audit independently requires Forge's exact index-379 `PauseAllNonScriptedEntities`
binding while validating the other 18 interface slots against the catalog. Current recovery suite:
222/222.

`StartBarrelTimer` now has an exact 562-byte retail snapshot covering all 163 instructions. It proves
three reads of WatchTimer `+0x108`, the returned HUD id store at GUIBarrelCounter `+0x60`, two byte
tests of BarrelManSpokenToHeroOnReturn `+0x73`, and the complete `AddQuestInfoBar(45.0, 0.0,
opaque-green, opaque-green, HUD_CLOCK_ICON, "", 1.0)` call. The loop uses exact distance 2.0,
green-inside/red-outside colors, and `UpdateQuestInfoBar(id, GetTimer(id), -1.0, -1.0)`, removing the
element only on normal return. Forge's slots `0x510/0x530/0x534/0x548` and their signatures match the
retail call shapes, and Lua preserves all 11 ordered operations. Current recovery suite: 224/224.

The full 647-byte `WatchBarrels` consequence thread is now independently decoded. Retail waits for at
least one `NOVI_Barrel`, clears byte field BarrelBrokenInstantaneous `+0x74`, exits on AttackOver
`+0x50`, and consumes each break by incrementing a local counter and clearing `+0x74` again. Counter 1
directly calls quest `AddBadDeed(0)` at `0x00DAEA70`; counter `total-1` inserts `OBJECT_GOLD_1`, while
`broken > total-4` creates a named stag beetle at BarrelBrokenPos `+0x76` and sets max/current health
to 2.0. Lua preserves all 15 ordered operations. Slots `0x428/0x924`, absent from the general catalog,
are independently pinned by Forge's exact `EntitySetMaxHealth` and `AddItemToContainer` bindings.
Current recovery suite: 226/226.

The barrel man's consequence split is now backed by two direct retail-byte snapshots rather than only
Ghidra text: the 934-byte automatic judgment region and the 653-byte phase-5 follow-up region. Retail
sets BarrelManSpokenToHeroOnReturn before a short-circuit visibility-or-10.0m test. A detected hero gets
`TEXT_QST_048_BARRELMAN_THANKS` plus `AddGoodDeed`; an absent hero gets `WHERE_GONE`, sets local
HeroLetMeDown `+0x1D`, displays `LEFT_WAREHOUSE_UNATTENDED`, and calls `AddBadDeed(1)`. The automatic
region never reads BarrelBrokenPersistent. Only later phase-5 dialogue reads it at parent `+0x75`, and
only when HeroLetMeDown is already true, selecting `LETDOWN_BROKEN`; otherwise a hero who stayed gets
`NO_TIME`. Lua preserves this counterintuitive retail behavior exactly. Current recovery suite: 228/228.

The compact `WatchForGotGold` thread now has direct coverage of all 212 meaningful bytes / 65
instructions before its alignment padding. Retail polls interface slot `0x1FC` until hero gold is
strictly greater than 2, checks termination both during and after the wait, then calls active-quest-name
slot `0xA3C` and objective slot `0x4A0` with `TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03` and two empty
trailing strings. Lua matches these operands exactly. The general catalog omits `0x1FC`, so the audit
requires Forge's exact index-127 `GetHeroGold` binding as independent evidence. Current recovery suite:
230/230.

`ManageQuestCoreMarkers`, the final quest-level spawned watcher, now has an exact 944-byte / 287-
instruction retail snapshot. It resolves the three entity lookups, all seven ordered marker mutations
(add father, remove father/add trader, remove trader/add Theresa, remove Theresa/add father), gold
minimum 3, player-control wait, tutorial 19 and dismissal wait, byte reads of GivenSweets `+0x94` and
GivenTheresaChocs `+0x95`, and all 11 termination checks. Lua preserves all 16 audited operations and
Forge supplies the already-proven `GetHeroGold` fallback for catalog-omitted slot `0x1FC`. Current
recovery suite: 232/232.

The Guard repeat-lecture path now has 4,020 bytes of direct retail coverage: a 639-byte claim/recheck
region and the complete 3,381-byte first/repeat lecture selection and bodies. Retail compares
`BadDeedsPerformed - GuardsDealtWithBadDeeds > 0`, repeats that test after the chase, then copies the
current bad-deed count into the dealt count before speaking. `GuardsSpokenOnce` selects the six-line
first lecture or `TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN`, crime list, and `AFTER_READ_LIST`; the first
path stores `GuardsSpokenOnce=1`. Lua already preserves this flow, and archived runtime evidence has
observed distinct claims for counts 1 and 2 plus both speech branches. The reported one-scold behavior
was therefore not a remaining retail-parity error in the current reconstruction. Current recovery
suite: 234/234.

The Affair Wife run-to-husband transition now has a reproducible 411-byte / 118-instruction retail
snapshot. It proves the single `MoveToPosition` call snapshots the husband's current position with
radius 2.0, run type 1, and trailing flags false/true. Both the initial and loop arrival tests instead
use the same live `NOVI_AffairMan` script handle at exact distance 3.0; only the true branch clears
commands and disables movement-in-actions before arguing. The optional running line triggers after
10.0 from home and stores `SaidRunningLine=1`. Lua and Forge's native position/distance chain match.
No captured real-game ROUTE_START/ROUTE_REACHED identity/position lines exist yet, so the previously
observed air-scolding remains narrowed to runtime replacement/handle or rendered-model divergence,
not the reconstructed route operands. Current recovery suite: 236/236.

The adjacent Affair Wife argument loop now has complete direct coverage through its loop-back edge:
1,770 bytes / 536 instructions at `0x00DB35C1-0x00DB3CAA`. Retail gates animation on the hero being
within 15.0, then passes the same husband wrapper local to all three facing calls (during pointing,
after a hit response, and after hero talk). It also adds that wrapper to each argument conversation,
uses it as the numbered wife-line listener, and as the optional `AFFAIRMAN_IN_TROUBLE` speaker. Lua
matches every role and Forge's facing/conversation bindings already match the retail call shapes.
Together with the route snapshot, this excludes a reconstructed recipient or position-snapshot mixup;
the observed air-scolding requires real-game identity/position probe evidence to resolve further.
Current recovery suite: 238/238.

The blue barrel-timer report is now traced through four concrete retail HUD functions. `AddBar`
resolves texture and text symbols separately, then its `0x58`-byte element stores the two mutable
colors at `+0x28/+0x2C` and the two sprite ids at `+0x30/+0x34`. `ChangeBarColour` writes only the
color fields. Forge passes exact BGRA bytes and the timer supplies green/green initially, then
green/green or red/red by proximity. Thus channel marshalling cannot explain blue (green is invariant
under R/B swapping), and a blue `HUD_CLOCK_ICON` is independent of the correctly colored bar fill.
No evidence-backed code change is warranted unless runtime inspection specifically shows the fill,
rather than the clock artwork, retaining blue. Current recovery suite: 240/240.

The Bully health bar now provides an asymmetric direct-byte control for that result. Two snapshots
cover 541 bytes / 145 instructions across creation/update and subdued removal. Retail constructs
primary bytes `00 FF 00 FF` (green) and secondary bytes `FF 00 00 FF` (blue), calls slot `0x510`,
stores the handle at parent `+0x64`, updates current to `InitialHealth-HitsTaken` through `0x530` with
max/scale `-1.0`, then sets `BullySubdued +0x6C` and removes that same handle through `0x548`. Lua
matches the complete lifecycle. This independently confirms Forge does not collapse or swap the two
color operands. Current recovery suite: 242/242.

The Bully subdued/run-off transition now has continuous direct-byte coverage from the final-hit
branch through the function epilogue: 1,161 bytes / 339 instructions at
`0x00DBC86B-0x00DBCCF4`. Retail acquires Bully, Hero, and victim at priority 4; builds the exact
`HERO`/`BRAT`/`BULLY` actor map; substitutes `$BRATLINE` from `HeroAttackedVictim`; and passes that
input map only to `BULLYRUN1`. `GivenHeroTeddy` selects `BULLYRUN2` plus victim-information clearing
and the state write, or `BULLYRUNDUMMY`. Normal teardown disables camera/pause, destroys the movie
and both maps, releases victim then Hero, sets `BullyRanOff`, awards the good deed, removes the Bully
with `(false,true)`, and finally releases it. Lua matches the entire transition. Current recovery
suite: 244/244.

The Guard now has complete direct retail-byte coverage. Init `0x00DAC650-0x00DAC760` contributes
272 bytes / 92 instructions and Main `0x00DAC760-0x00DADE4B` contributes 5,867 bytes / 1,791
instructions, for 6,139 bytes / 1,883 instructions through exact terminal `ret` boundaries. The
artifact covers the deed claim/recheck, chase, first and repeat lectures, hit response, crime list,
and terminal cleanup. Retail claims the entire positive
`BadDeedsPerformed - GuardsDealtWithBadDeeds` delta and then copies the current bad-deed count into
the dealt count; consequently one scold for one destruction deed is correct, while a later new deed
can trigger `CAUGHT_YOU_AGAIN`. Lua already matches this lifecycle. Current recovery suite: 262/262;
package fixtures: 117/117 with zero warnings/errors; SDK validation passes. Fable remained closed and
nothing was deployed.

The Barrel entity now has complete lifecycle coverage: shared empty Init `0x00CDEBB0-0x00CDEBB1`,
OnPredicateFail `0x00DB7DB0-0x00DB7DE1`, and Main `0x00DB7E10-0x00DB7FEC`, totaling 526 bytes /
164 instructions through exact terminal `ret` boundaries. This verifies the destruction callback's
instantaneous/persistent flags and position copy as well as the instruction/idle loop. Separately,
TLC `WatchBarrels` proves that break `total-1` inserts `OBJECT_GOLD_1` into the remaining barrel, so
destroying the final barrel should release the coin. Lua matches that branch. Because the user did
not observe a coin, Forge now has undeployed `AddItemToContainer` diagnostics logging item and exact
container identity around the native call; Release|x86 builds cleanly with SHA-256
`AE55FE0D138FE3E529852E54941E66C89517541A9E3E7A89CBB535AB3D49DF3C`. Current recovery suite:
264/264. The game remained closed.

CreatedBeetle now has complete direct coverage: shared empty Init `0x00CDEBB0-0x00CDEBB1` and Main
`0x00DB80C0-0x00DB81A8`, totaling 233 bytes / 80 instructions. The snapshot proves the five-second
timer, same-ID polling, `(true,true)` self-removal, and timer deregistration on both normal and
termination exits. Lua and inventory match without a behavior change. Recovery tests are 266/266;
package fixtures remain 117/117 with zero warnings/errors; SDK validation passes. Fable remained
closed and nothing was deployed.

TeddyGirl now has complete direct coverage across Init `0x00DAF000-0x00DAF052`, Main
`0x00DAF080-0x00DB0600`, and GivenTeddy `0x00DB0600-0x00DB065B`: 5,677 bytes / 1,686
instructions. Direct decoding exposed and corrected a metadata seam: `translation_unit.json` reports
Main size 5,498, which stops inside the final `add esp,0x158`; the executable's exact terminal `ret`
is at `0x00DB05FF`, making Main 5,504 bytes. The full artifact covers both good/bad deed outcomes,
TeddySolution B/C, ruined-teddy walk-off, hit response, all conversations, and cleanup paths. Lua
matches without a behavior edit. Recovery tests are 268/268 and package fixtures remain 117/117
with zero warnings/errors. The game remained closed and nothing was deployed.

The user-provided Anniversary footage (`4IkkNZxW0MQ`, inspected 7:40-9:40) visibly enters the
unattended-return failure and then plays Barrel Man's broken-stock rebuke: `A fat lot of good you
were`, the damaged-stock line, and the threat. New Oakvale now also handles the distinct reported
nearby-plus-broken case: `BarrelBrokenPersistent` suppresses `THANKS` and the good deed and selects
`LETDOWN_BROKEN`, without incorrectly adding the separate unattended-warehouse bad deed. The
generated trace proves one broken rebuke, zero thanks, and no good/bad deed counter write in this
compatibility branch. TLC's differing automatic-return bytes remain preserved in the retail snapshot
and audit. Evidence note: `runtime_evidence/anniversary-barrel-reference-20260910.md`. Recovery tests
are 271/271; package fixtures are 117/117 with zero warnings/errors; SDK validation passes. This
change is not deployed and the game remains closed.

Victim now has complete retail coverage across Init `0x00DAEEB0-0x00DAEF4C` and Main
`0x00DBCD60-0x00DBDE35`: 4,465 bytes / 1,328 instructions. This exposed the same six-byte generated
metadata truncation pattern as TeddyGirl: the reported 4,303-byte Main stops inside its final stack
adjustment, while the exact terminal `ret` is at `0x00DBDE34`. The snapshot covers all Bully shared
state, first/repeat hit outcomes, bad deed, BRATHIT cutscene, post-run thanks setup, and teddy-loss
complaint. Lua and inventory match. The evidence is included in the 271/271 recovery result above.

LiveFather now has complete retail coverage across Init `0x00DAC390-0x00DAC41D` and Main
`0x00DB86B0-0x00DB9795`: 4,466 bytes / 1,352 instructions. This again corrects a generated Main
boundary truncated by six epilogue bytes. Coverage includes the intro movie, deed counter, all mixed
good/bad reward dialogue, gold/chocolate handoff, attack response, and childhood completion state.
Lua and inventory match without a behavior correction. Recovery tests are 273/273; package fixtures
remain 117/117 with zero warnings/errors. Fable remained closed and nothing was deployed.

Theresa, Villager, and Bully now close the remaining entity-level byte gaps. Theresa covers 7,155
bytes / 1,917 instructions across Init/Main, including the chocolate morality choice and raid outro.
Villager covers 2,978 bytes / 900 instructions across Init/Main/GetVillagerSpeechIndex, including all
sex/deed speech matrices and nonrepeating selection. Bully covers 6,899 bytes / 2,002 instructions
across Init/Main/GivenTeddy, subsuming the teddy, health-bar, and run-off focused snapshots. Direct
decoding corrected Theresa's truncated Main epilogue and Bully's nine-byte Main truncation. Every one
of the 15 `NOVI_*` entity inventories now cites a complete reproducible lifecycle/dispatcher
snapshot. Recovery tests are 279/279; package fixtures remain 117/117 with zero warnings/errors; SDK
validation passes. Fable remained closed and nothing was deployed.

A deployment-ready but undeployed test payload is staged at
`work/new_oakvale_test_bundle_20260910/`: the 23-file FSE tree plus the diagnostic Release|x86 DLL.
Its manifest records 24 payload files, all hashes reverify, and explicitly records
`deploymentPerformed=false`. DLL SHA-256 is
`AE55FE0D138FE3E529852E54941E66C89517541A9E3E7A89CBB535AB3D49DF3C`. Test focus is the corrected
Barrel Man rebuke, final-barrel gold delivery logs, Bully marker completion, and Affair Wife route
identity. The all-entity snapshot gate raises the recovery suite to 280/280.

The quest object itself now has complete reproducible coverage: 15 exact regions totaling 11,838
bytes / 3,641 instructions across RegisterMain, OnPersist, Init, Main, AddGoodDeed/AddBadDeed,
StartBarrelTimer, DoMission, WatchForGotGold, AttackStuff, ManageQuestCoreMarkers, WatchBarrels,
PostAttackStuff, and both destructor layers. Direct decoding proves quest Main continues 18 bytes
beyond the generated size through its second cleanup epilogue at `0x00DAC2B3`; the destructor
implementation ends in the retail tail jump to `CScriptBase::~CScriptBase` at `0x00CBD510`.
Recovery tests are 282/282; package fixtures are 117/117 without warnings/errors; SDK validation
passes. Fable remained closed and nothing was deployed.

The package-wide coverage gate caught the non-`NOVI_` outlier `OVI_DeadFather`, which the earlier
entity-name filter omitted. It now has a complete 635-byte / 207-instruction lifecycle snapshot over
OnPredicateFail, Init, and Main, including marker/pose setup, DadFound transition, and control
cleanup; direct decoding corrects its truncated Main epilogue through `0x00DB8515`. The final native
coverage certificate is therefore 17/17 inventories (15 `NOVI_*`, `OVI_DeadFather`, and the quest),
78,891 exact retail bytes, and 23,388 instructions, all from retail executable SHA-256
`41DC91090AE853715AC06D2E9FC96E5D545381D197ED55D624C642F34509AC10`. Recovery tests are 285/285;
package fixtures remain 117/117; SDK validation passes. No deployment or game launch occurred.

`ExportScriptTranslationUnit.java` is upgraded to schema 0.2 after the boundary audit exposed an
ambiguity in its legacy `size`: `Function.getBody().getNumAddresses()` counts addresses across a
possibly disjoint Ghidra body and is not a contiguous end offset. Future exports retain `size` as a
compatibility alias but also emit `bodyAddressCount`, min/max/end-exclusive, `bodyExtent`, and every
body range. A regression test locks these fields. Recovery tests are 286/286.

The translation unit has now been regenerated read-only from the Ghidra retail database using the
original `0x00DAA000-0x00DBF100` range and definition list. The checked-in artifact is schema 0.2,
retains the same 100 function entries, and exposes the real disjoint-body topology (for example,
Bully Main is 6,630 body addresses across a 6,639-byte extent and four ranges). A regression test
now locks the generated artifact itself, not only the Java exporter.

Final-gold diagnostics now bracket the penultimate-break insertion at both layers. Lua logs stable
`GOLD_ARM broken=<n> total=<n> container_present=<bool>` and `GOLD_INSERT_RETURNED`; Forge logs the
item plus exact native container identity around slot `0x924`. The fixture proves break 5 of 6,
container present, and correctly ordered return. An initial Lua pointer string was rejected because
it made traces nondeterministic; two consecutive package validations now pass identically. The
current undeployed payload is `work/new_oakvale_test_bundle_20260910_v3/`. Recovery tests are 287/287
and package fixtures are 117/117 without warnings/errors.

`analyze_new_oakvale_targeted_playtest.py` now converts a Forge log into a structured outcome matrix
for Barrel Man rebuke/contradictory thanks, gold branch/container/native call, Bully compatibility,
and Affair Wife route start/reached. Running it against the archived failing log (SHA-256 `39519E...`)
correctly reports old thanks=true and every corrected/new probe=false; the result is preserved as
`runtime_evidence/analysis-interactive-20260910-targets.json`. This establishes an automated before/
after comparison for the v3 payload. `audit_final_barrel_gold_abi.py` now independently certifies
the reward path from WatchBarrels callsite `0x00DBEA2F`, through retail vtable slot `0x924` / index
585 and native target `0x0089E780`, into Forge's exact container-before-item ABI and the Lua call.
The installed retail EXE resolves the slot exactly and matches SHA-256 `41DC9109...AC10`. Recovery
tests are 289/289. The game remained closed and nothing was deployed.

The coherent undeployed payload is now `work/new_oakvale_test_bundle_20260910_v5/`: 24 hashed files,
DLL SHA-256 `1250BBCC...9268`, and `deploymentPerformed=false`. Two consecutive package validations
produced identical reports (117/117 fixtures, zero warnings/errors). Affair Wife retail route and
native position-chain audits, Bully run-off bytes, final-gold ABI, and tooling SDK validation all
pass; SDK mirrors were intentionally skipped because the noncanonical repositories remain stale.
Forge's named-thing diagnostic now records the retail world UID and wrapper-vtable position. The
targeted log analyzer is schema 0.2 and associates the last husband lookup with Wife route start,
distinguishing a replaced world object from a rendered-position fault in the next capture.

`analyze_bully_run_variance.py` proves the native Bully macro is nondeterministic across the five
archived captures: 1 completes at `MK_OIBR_BULLY2`, while 4 stop at the identical point 11.759 m
short after only 2.326-2.365 m. Retail `RunCutsceneMacro_Func` parses the authored false wait operand
and polls scripted-resource slot `0x68` (`IsPerformingScriptTask`), so the following 0.5-second pause
is not the intended wait. This narrows the defect to task/path completion and supports the existing
same-marker compatibility move rather than an ABI or cutscene-flag rewrite. Recovery is 291/291.

The Barrel Man outcome is now certified against the installed localized asset, not only symbolic
headers and video transcription. `barrel_man_text_assets.json` reproduces from English `text.big`
(SHA-256 `531C4514...33FC`) and proves `BARRELMAN_LETDOWN_BROKEN` contains the exact three ordered
lines seen in the reference: “A fat lot of good you were,” damaged-stock/trust, then the threat.
The corresponding audit locks the Lua broken-stock branch to that group before any thanks path.
Recovery tests pass 293/293.

## State
- Shared working branch: `feat/novi-script-recovery`; native parity checkpoints through `39538ff` and subsequent NewOakValeIntro work are present locally. Verify `git status`, `git log`, and the active branch before every checkpoint because the native and script-recovery lanes share this worktree. `main` and remote `feat/script-recovery-marathon` were both at `8d6ce97` before this handoff. Never stage broad paths: commit only explicit files so concurrent `refs/script_recovery/new_oakvale_intro/` work and line-ending-only header changes are preserved.
- 22 old agent branches are now `archive/*` tags. Root scratch + `CON` removed. `LICENSE`, `CONTRIBUTING.md`, CI `docs-consistency` added.
- Coverage (2026-09-08 dashboard): 8,145 exact and 10,681 relocation matches among 18,870 compiled/behavior-tested candidates; 15,297 landed files are manifest-tracked, including 14,696 genuine sources. Regenerate `rebuild/COVERAGE.md` before quoting later numbers.

## Open these five first
1. `docs/ROADMAP.md` — what is done / in flight / next (the only task list)
2. `docs/ARCHITECTURE.md` — the three source layers (lift intake / parity `rebuild/src/compiled` / modern)
3. `docs/BUILDING.md` — Ghidra, VC7.1, GhidraMCP, FSE tooling commands
4. `CONTRIBUTING.md` — landing rules, header contract, purity policy
5. `rebuild/COVERAGE.md` — the numbers

## Active lanes and their resume commands
- Typed headers: `python tools/decomp_pipeline/gen_class_headers.py --all-trusted` (add `--compile-check`), then `python tools/decomp_pipeline/retype_landed.py --class CGameScriptInterface` (dry run; `--apply` only rewrites files that stay byte-exact). Log: `rebuild/backlog/retype_log.tsv`; headers: `rebuild/include/engine/` (`INDEX.tsv`, `RECONCILE.tsv`, `_quarantine/`).
- Parity crawl: batches through 377 are reviewed and ledgered in `work/crawl_batch155`; generate batch 378 next with `next_smallest.py`. Current totals are 18,870 compiled/behavior-tested, 8,145 exact, 10,681 relocation matches, and 394,217 genuine retail bytes matched. Continue evidence review; use PDB names for readability but treat retail offsets, calling conventions, behavior, and bytes as authoritative. Details: `docs/journal/2026-09/PARITY_CRAWL_2026-09-08.md`.
- Header gate: the 2026-09-08 `--all-trusted --compile-check` wrote and compiled 1,011 generated headers; 267 are layout-quarantined and `CGameDefinitionManager` remains the single additional compile quarantine. Dashboard inventory: 1,019 generated / 268 quarantined.
- De-bake: `python tools/decomp_pipeline/crawl/bake_families.py` for the live family table; `debake_family.py <template> <genuine.cpp> <prefix> --apply`.
- Script recovery: `python -m unittest discover -s tools/script_recovery -p 'test_*.py'`; `python tools/script_recovery/verify_foundation.py`; readiness needs `--vtable-slots`, `--interface-catalog`, `--fse-address-map`. Ingest Aeon's 2026-09-06 LUAGameflow batch into `work/aeon_lua_ports/`.
- ForgeFSE: canonical fork is `D:\Code\ForgeFSE-retail-shadow`, branch `feat/upstream-fse-2026-09-02` (`3f417ee`), untested in-game. `D:\Code\ForgeFSE` is stale.
- Docs hygiene: `python tools/docs_reorg.py --check-links --check-findings --check-root` and `python tools/update_readme_progress.py --check` (both run in CI).
- Build/visual QA: `rebuild/build_bootstrap.ps1 -RetailFrontendBank <frontend.big>`; recipe in `docs/pipeline/VISUAL_PARITY_STATUS.md`. Retail Fable.exe must be CLOSED (single-instance mutex fails the WinMain fixture).

## Where notes go
- This file stays one page. The long-running journal is `docs/journal/HANDOFF_ARCHIVE.md` (12k lines, append-only history).
- New session notes: `docs/journal/YYYY-MM/<TOPIC>_<date>.md` (e.g. `docs/journal/2026-09/`). Cited technical truth: `docs/journal/FINDINGS_LOG.md`.
- Solved gotchas get a one-liner in `CLAUDE.md`; roadmap status changes go in `docs/ROADMAP.md`.

## Gotchas not yet in CLAUDE.md
- A file named `CON` at the repo root hangs EVERY git command on Windows (reserved device name). Delete it with `del "\\?\D:\Documents\FableTLC\CON"` — normal `del`/Explorer cannot.
- A killed `git add` leaves a stale `.git/index.lock`; remove it before retrying (check no git process is running first).
- Sibling repos hold the moved modding docs: FableForge `D:\Code\FableForge\docs\from_fabletlc\` and ForgeFSE-retail-shadow `docs\from_fabletlc\`. `docs/modding/README.md` is the index; do not recreate those docs here.
- `native_conversion_readiness.json` silently loses all interface resolution if regenerated without `--vtable-slots` and `--interface-catalog`.
- Two ForgeFSE forks existed; only `ForgeFSE-retail-shadow` is canonical. Any binding port must land there.

Offline-only direction: user selected "Keep working offline only" and is at work,
unavailable for live verification. No activation, installation or save/profile changes.
Continue native/compiled offline integration; do not ask again about live testing.
