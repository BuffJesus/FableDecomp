# The scripts between Guild Training and Orchard Farm (2026-09-22)

The order is not guesswork: it is what our converted **Gameflow** does
(`refs/script_recovery/lifted/Gameflow/readable/FSE/Gameflow/Gameflow.lua`, stages 0 -> 400).

| Gameflow stage | what it does | script |
|---|---|---|
| 0 | `GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_OAKVALE_INTRO", "Q_NewOakValeIntro")`, waits on it | **converted** |
| 100 | waits on `Q_GuildTraining`; then `AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss")` + three dummy cards; activates `GameflowAssistance`, `CS_OakValeRevisited`, `V_BeggarAndChild`, `V_GuildMaster`, `Q_OrchardFarm_Barricade`, `V_SickChild`, `V_BookCollecting`, `V_ChickenKicking`, `V_Bordello` | **Q_GuildTraining converted**; the rest below |
| 200 | waits on `Q_WaspBoss`; then `GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_FIRST", "QS_GuardianSisterInfo")`, activates `V_TourGuide` | **Q_WaspBoss** |
| 300 | waits on `QS_GuardianSisterInfo`; then `Hook_BowerstoneTeleportTutorial`, `V_PicnicAreaAfterWaspBoss`, and `AddQuestCard` for `Q_OrchardFarmRaidGood` / `Q_OrchardFarmRaidEvil` | **QS_GuardianSisterInfo** |
| 400 | polls `IsQuestActive` on either farm card = the player accepted one | **OrchardFarm converted** |

So the main path to convert is exactly two quests -- **Q_WaspBoss** and **QS_GuardianSisterInfo** -- plus the
ambient village scripts stage 100/200/300 switch on, which are optional for a play-through but are what makes
the world feel alive on the way.

`Q_OrchardFarm_Barricade` needs nothing: `script_units.py` already records that it has no script class (it is a
resource-section quest the raid activates).

## Units registered (ranges from the vtables, read-only against retail)

| unit | range | evidence |
|---|---|---|
| `wasp_boss` | 0x00E0E820 .. 0x00E183B0 | vtable 0x012E00E4: Init 0x00E0E820, RegisterMain 0x00E0E8E0, Main 0x00E0EA40, OnPersist 0x00E0E9A0, dtor 0x00E13BF0; next family's allocator (Q_WhiteBalverineKnotholeGlade) 0x00E183B0. PDB names the entity classes: `CWaspChaser`, `CWaspChaseWoman`, `CHornetDrone`, `CFleeingWoman`, `CGratefulVillagerSpawn` |
| `guardian_sister_info` | 0x00E25A00 .. 0x00E277E0 | vtable 0x012E1DAC: Init 0x00E25A00, RegisterMain 0x00E25A10, Main 0x00E25AB0, OnPersist 0x00E25F40, dtor 0x00E267C0. WIDENED from 0x00E267D0 to the next family allocator after the inventory failed on 0x00E268C0 -- the entity bindings sit above the destructor; the wider range also covers `QS_GuardianSisterInfo2_SisterInBanditCamp` |

**Careful:** `QS_GuardianSisterInfo`'s `Init` sits BELOW its own allocator (0x00E26780) *and* below the previous
family's allocator (`QR_EscortTrader_Manager` 0x00E25970), so allocator order is not a bound here -- the vtable
is. Both ranges stay hypotheses until the inventory step lists the classes inside them.

## The pipeline that was run (kept as the recipe for the next unit)

    python tools/script_recovery/export_guild_training.py --unit wasp_boss
    python tools/script_recovery/guild_training_inventory.py --unit wasp_boss      # CONFIRM the range here
    python tools/script_recovery/quest_unit_evidence.py --unit wasp_boss
    python tools/script_recovery/ghidra_typing_spec.py --unit wasp_boss
    python tools/script_recovery/infer_helper_prototypes.py --unit wasp_boss        # MUST run (helpers vanish otherwise)
    # typed export (the range comes from script_units.py):
    analyzeHeadless ghidra_proj FableTLC -process Fable.exe -readOnly -noanalysis \
      -scriptPath tools/ghidra_scripts -postScript ExportTypedTranslationUnit.java \
      0x00E0E820 0x00E183B0 refs/script_recovery/wasp_boss/translation_unit_typed.json \
      refs/script_recovery/wasp_boss/define_addresses.txt refs/script_recovery/wasp_boss/typing_spec.json
    python tools/script_recovery/convert_quest_unit.py --unit wasp_boss
    python tools/script_recovery/build_readable_unit.py --unit wasp_boss
    python tools/script_recovery/smoke_run_unit.py --unit wasp_boss --stage draft

Then the same seven commands with `--unit guardian_sister_info` (range 0x00E25A00 0x00E277E0).

For a unit whose evidence dir is empty, `export_guild_training.py` fails in `recover()` before it can export (it reads entity bindings out of a translation unit that does not exist yet): run `ExportScriptTranslationUnit.java` over the range once by hand first, with an anchor file containing just a comment line, then the seven commands work.

Gates after each: the Oakvale draft stays identical, guild / orchard / gameflow / trader regenerate unchanged,
smoke 0 on every unit, and the full suite still reports the pre-existing baseline (403 failures / 45 errors).

## Aeon's wasp port

Aeon has already hand-ported the wasp quest. Ours is still worth doing: the hand port is the **oracle** we
check the converter against (the same way `audit_port_against_pdb.py` and the Guild ports were used), and a
disagreement is evidence about one side or the other -- exactly how Aeon's bool `CoreQuestWaiting` turned out
to be the deviation from retail's ulong today.

## Testing along the way

`tools/script_recovery/checklists/orchard_farm_raid.json` already exists and documents the entry:
map slot 9 `OrchardFarm` at (3248, 3232, 42.5) (`forge world` + `forge heights OrchardFarm 48,64`), the quest
started with `ActivateQuest('Q_OrchardFarmRaidGood')` exactly as accepting the card does. It needs an **adult
save** -- the chain's graduation autosave is the candidate. Wasp and sister-info checklists follow once their
units convert and we can read their real speech keys and cutscene macros.

## Both units converted (2026-09-22)

| unit | owners / functions | missing | smoke draft / readable | `TODO(native)` |
|---|---|---|---|---|
| `wasp_boss` | 10 / 46 | 0 | 0 / 0 | 23 |
| `guardian_sister_info` | 4 / 14 | 0 | 0 / 0 | 2 |

The ranges held up, with one correction the inventory step caught: `guardian_sister_info` needed widening to
0x00E277E0 because the family's entity bindings sit ABOVE its destructor (the first `recover()` failed on
0x00E268C0). Widening it also pulled in `QS_GuardianSisterInfo2_SisterInBanditCamp`, which Gameflow stage 550
needs later, so both are declared and both converted. The wasp range needed no correction: the inventory
listed exactly the nine PDB-named entity classes (`WaspHelper`, `QueenHornet`, `HornetDrone`, `WaspChaser`,
`WaspChaseWoman`, `WaspAttacker`, `WaspVictim`, `FleeingWoman`, `GratefulVillagerSpawn`).

`QS_GuardianSisterInfo` reads like a quest script: set the card objective at BowerstoneSlums, wait for that
region to load, `SetTimeOfDay(10.0)`. All its content is the `MazeAtTavern` entity
(`CS_GUARDIANSISTER_BOWERSTONE`, `TEXT_QST_027_MAZE_CALL_HERO_OVER_10`).

Neither unit's PDB locals exist in `Ego_r.pdb` (`pdb-locals.exe` reports 0 matches for both, and for the
known-good `CQ_OrchardFarmRaidScript` control too, so the invocation differs from whatever produced the
committed TSVs). Both converted anyway with a header-only locals file; the cost is PDB-derived parameter
names, which the converter infers instead.

## Checklists ready (both need an ADULT save)

* `checklists/wasp_boss.json` -- the quest declares its own region, `AddQuestRegion("Q_WaspBoss", "PicnicArea")`:
  map slot **2** at (3168, 3568, 33.8) (`forge world` origin 3104,3520 96x128... 128x96; `forge heights
  PicnicArea 64,48` = 33.78). Drives the intro, the helper's guidance lines, the queen (`StartChase` /
  `QueenHornetAttacks`) and the outro via `MissionSucceeded`.
* `checklists/guardian_sister_info.json` -- region BowerstoneSlums is owned by map `BowerstoneSlums_v2`,
  slot **339** at (3808, 4288, 29.2) (`forge world --regions`; `forge heights BowerstoneSlums_v2 64,64`).
  Drives Maze at the tavern and `GuardianSpokeToHero`.

## Converter fixes this conversion produced (all generic, all gated)

1. Thing vcalls spelled through the object's first dword (`(**(code **)(X._0_4_ + 0x12c))()`), which is the
   vtable -- the "while the victim is alive" loops.
2. The RAW hidden-return out-param as a thing alias:
   `(**(code **)(**(int **)(this + 4) + 0x120))(recv, &slot, &name)` makes `slot` a thing. `annotate` runs on
   the raw decompile (it is what produces the `GSI->` names), and the hidden return is the SECOND argument,
   after the repeated receiver -- two earlier attempts missed on both counts.
3. `drop_free_suffixes` was renaming a CALL: it strips a numeric suffix when the base name is free, and its
   "is this a local?" test matched any line starting with the name, so `helper_E12F20(quest)` became
   `helper_E12F(quest)` and `DoMission` called a nil global. Pre-existing; surfaced here because this is the
   first unit with a `helper_` call at statement level.
4. `AreAllThingsInVectorDead` on a LOCAL vector (the wasp polls its own `GetAllThingsWithScriptName` lists),
   plus stripping the element index the list rewrite added to that argument -- the helper takes the vector.
5. A staged bool/number literal survives a block boundary instead of escaping as a free global
   (`KickOffQuestStartScreen(..., false)`, not `..., isGold`). Deliberately NOT extended to strings: a staged
   string is usually the name a thing is looked up by, and substituting it rewrote `PrepareResource(slot)`
   into `PrepareResource("BanditCampEntrance")` in TraderConflict.

## Still open on this path

The ambient scripts stage 100/200/300 switch on (`V_TourGuide`, `V_BeggarAndChild`, `V_GuildMaster`,
`V_SickChild`, `V_BookCollecting`, `V_ChickenKicking`, `V_Bordello`, `V_PicnicAreaAfterWaspBoss`,
`GameflowAssistance`, `CS_OakValeRevisited`, `Hook_BowerstoneTeleportTutorial`). All are in the conversion
queue with an allocator address and a native cluster; each needs a `UNITS` entry with a vtable-derived range
and the same seven-command pipeline. None is on the critical path to Orchard Farm.

## V_TourGuide converted (2026-09-22) -- first of the ambient scripts, and a rougher one

Registered as unit `tour_guide`, range 0x00EE42A0 .. 0x00EE80A0 (cluster vtable lifecycle filtered to the
family's own block; the next family's allocator is V_TrophyDealer 0x00EE80A0). The inventory confirmed it:
`V_TourGuide` with entities `TourGuideGuide` and `TourGuideFollower`.

15 functions, **0 missing**, all three files compile -- but unlike the wasp and the sister this one is not
close to done: **79 `TODO(native)`** and 4 smoke problems. That is worth stating plainly rather than counting
it as converted.

What the 79 actually are (normalised by shape):

| count | shape | verdict |
|---|---|---|
| ~48 | `CCharString::operator=((CCharString *)__element("WaypointInfo…"/"Random…"), …)` | ONE generic gap: assigning into a quest-state string list element. Worth a single fix |
| 4 | `IsDistanceFromThingToPositionOver(v, &stackVector)` | the stack-C3DVector fold does not reach this callee |
| ~6 | field reads `X = *(pCVar7 + N)` and resource-object casts | the usual counted-pointer/field shapes |
| rest | helper calls with operands printed as `*(this + N)` | operand recovery |

Smoke (4 problems), split by whose gap it is:

* **ForgeFSE bindings missing** (sidecar work, NOT converter work): `MsgIsRegionUnloaded`,
  `EntitySetPersonalityOverride`.
* **Converter gaps** (real): `TourGuideGuide.lua:276` arithmetic on a nil value, and `helper_EE6850`
  indexing a number (`native_arg_param_1`), i.e. a helper prototype whose parameter kind is wrong.

## Candidate ranges for the remaining ambient scripts

Derived the same way (cluster vtable lifecycle, filtered to the family's own block, upper bound = the next
family's allocator). Each is a hypothesis until its inventory runs.

| script | lo | hi (next family) |
|---|---|---|
| `GameflowAssistance` | 0x00CEFA00 | 0x00CF8860 (Q_ArenaHoldingScript) |
| `CS_OakValeRevisited` | 0x00EE8210 | 0x00EE90A0 (Global_WatchForHeroDeath) |
| `V_BeggarAndChild` | 0x00E57C00 | 0x00E62730 (V_BodyGuard) |
| `V_GuildMaster` | 0x00E90780 | 0x00E93CF0 (V_HiddenBooty) |
| `V_SickChild` | 0x00EC5420 | 0x00ED39A0 (V_SingingStones) |
| `V_BookCollecting` | 0x00E543B0 | 0x00E5D0B0 (V_BeggarAndChild) |
| `V_ChickenKicking` | 0x00E628B0 | 0x00E6E230 (V_ChapelOfEvil) |
| `V_Bordello` | 0x00E399D0 | 0x00E477A0 (V_BanditCampPath) |
| `V_PicnicAreaAfterWaspBoss` | 0x00EC1240 | 0x00EC3BC0 (V_RandomPopulationSim) |

**Do not take a cluster's lowest vtable slot as `lo`**: three of these (CS_OakValeRevisited, V_GuildMaster,
V_PicnicAreaAfterWaspBoss) have slots pointing at shared base code near 0x00CBD4D0, far below their own
family, and an unfiltered `min()` would produce a range spanning half the binary.

