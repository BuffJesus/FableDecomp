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
| `guardian_sister_info` | 0x00E25A00 .. 0x00E267D0 | vtable 0x012E1DAC: Init 0x00E25A00, RegisterMain 0x00E25A10, Main 0x00E25AB0, OnPersist 0x00E25F40, dtor 0x00E267C0 |

**Careful:** `QS_GuardianSisterInfo`'s `Init` sits BELOW its own allocator (0x00E26780) *and* below the previous
family's allocator (`QR_EscortTrader_Manager` 0x00E25970), so allocator order is not a bound here -- the vtable
is. Both ranges stay hypotheses until the inventory step lists the classes inside them.

## To run when the machine is free (Ghidra headless; nothing below runs on its own)

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

Then the same seven commands with `--unit guardian_sister_info` (range 0x00E25A00 0x00E267D0).

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
