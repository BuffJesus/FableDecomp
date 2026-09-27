"""Registry of native script translation units (address range + quest scripts) for the converter.

A unit is one contiguous retail code range holding a family of quest script classes. The range
bounds come from the native clusters (constructor of the first class .. destructor of the last).
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

UNITS = {
    'bandit_camp': {
        # Q_BanditCamp Init through the next family's Q_BountyHunt Init.
        # Includes BossBattle and HoldingScript; shared lifecycle slots are explicit anchors.
        'evidence': ROOT / 'refs/script_recovery/bandit_camp',
        'lo': 0xD006D0, 'hi': 0xD12D00,
        'ir_glob': 'Q_BanditCamp*.json',
        'scripts': ['Q_BanditCamp', 'Q_BanditCampBossBattle', 'Q_BanditCampHoldingScript'],
        'pdb_pattern': '*CQ_BanditCamp*',
        'schema': 'bandit-camp-native-inventory/0.1',
        'package': 'BanditCamp',
    },
    'trader_escort': {
        # Quest vtable 0x012DF16C: Init 0xE006D0 through destructor 0xE0AFB0.
        # Q_UndeadRising starts at Init 0xE0B020; inventory checks entity slots.
        'evidence': ROOT / 'refs/script_recovery/trader_escort',
        'lo': 0xE006D0, 'hi': 0xE0B020,
        'ir_glob': 'Q_TraderEscort.json',
        'scripts': ['Q_TraderEscort'],
        'pdb_pattern': '*CQ_TraderEscort*',
        'schema': 'trader-escort-native-inventory/0.1',
        'package': 'TraderEscort',
    },
    'guild_training': {
        'evidence': ROOT / 'refs/script_recovery/guild_training',
        'lo': 0xD3B390, 'hi': 0xD68F00,
        'ir_glob': 'Q_GuildTraining*.json',
        'scripts': ['Q_GuildTraining', 'Q_GuildTrainingDeparture', 'Q_GuildTrainingMelee',
                    'Q_GuildTrainingPreMelee', 'Q_GuildTrainingSkill', 'Q_GuildTrainingWill',
                    'Q_GuildTrainingWoodsDeparture', 'Q_GuildTrainingWoodsMelee', 'Q_GuildTrainingWoodsWill'],
        'pdb_pattern': '*CQ_GuildTraining*',
        'schema': 'guild-training-native-inventory/0.1',
        'package': 'GuildTraining',
    },
    'wasp_boss': {
        # Gameflow stage 100 -> 200: the WASP_MENACE card, then stage 200 waits on Q_WaspBoss.
        # Q_WaspBoss vtable 0x012E00E4: Init 0x00E0E820 .. dtor 0x00E13BF0; next family's allocator
        # (Q_WhiteBalverineKnotholeGlade) is 0x00E183B0, and the wasp entity classes sit in between.
        'evidence': ROOT / 'refs/script_recovery/wasp_boss',
        'lo': 0xE0E820, 'hi': 0xE183B0,
        'ir_glob': 'Q_WaspBoss*.json',
        'scripts': ['Q_WaspBoss'],
        'pdb_pattern': '*CQ_WaspBoss*',
        'schema': 'wasp-boss-native-inventory/0.1',
        'package': 'WaspBoss',
    },
    'guardian_sister_info': {
        # Gameflow stage 200 -> 300: handed directly after the wasp boss, and stage 300 waits on it before
        # the Orchard Farm cards. Q_ vtable 0x012E1DAC: Init 0x00E25A00 .. dtor 0x00E267C0 -- its Init is
        # BELOW its own allocator 0x00E26780, so the vtable bounds it, not the allocator order.
        'evidence': ROOT / 'refs/script_recovery/guardian_sister_info',
        'lo': 0xE25A00, 'hi': 0xE277E0,
        'ir_glob': 'QS_GuardianSisterInfo*.json',
        'scripts': ['QS_GuardianSisterInfo', 'QS_GuardianSisterInfo2_SisterInBanditCamp'],
        'pdb_pattern': '*CQS_GuardianSisterInfo*',
        'schema': 'guardian-sister-info-native-inventory/0.1',
        'package': 'GuardianSisterInfo',
    },
    'guardian_trophy_dealer_info': {
        # Gameflow stage 800 (EGP_MAZE_TELEPORT_TO_WW, right after Bandit Camp) waits on QS_GuardianTrophyDealerInfo.
        # Manifest: the entity CGTDI_Maze Init 0x00E27C60 / Main 0x00E27E90; the block opens after the
        # guardian_sister_info unit's hi (allocator 0x00E277E0) and ends at the next family's allocator 0x00E28CA0.
        'evidence': ROOT / 'refs/script_recovery/guardian_trophy_dealer_info',
        'lo': 0xE277E0, 'hi': 0xE28CA0,
        'ir_glob': 'QS_GuardianTrophyDealerInfo*.json',
        'scripts': ['QS_GuardianTrophyDealerInfo'],
        'pdb_pattern': '*CQS_GuardianTrophyDealerInfo*',
        'schema': 'guardian-trophy-dealer-info-native-inventory/0.1',
        'package': 'GuardianTrophyDealerInfo',
    },
    'trophy_dealer': {
        # Gameflow stage 850 (EGP_TROPHY_DEALER) waits on V_TrophyDealer, the card GTDI_Maze hands over. Manifest:
        # CV_TrophyDealerScript Init 0x00EE6D60 / HilightGuildTeleporter 0x00EE71A0; the block opens at the tour guide's
        # allocator 0x00EE6C00 and ends at its own allocator 0x00EE80A0 (the tour_guide unit's range over-covers it).
        'evidence': ROOT / 'refs/script_recovery/trophy_dealer',
        'lo': 0xEE6C00, 'hi': 0xEE80A0,
        'ir_glob': 'V_TrophyDealer*.json',
        'scripts': ['V_TrophyDealer'],
        'pdb_pattern': '*CV_TrophyDealer*',
        'schema': 'trophy-dealer-native-inventory/0.1',
        'package': 'TrophyDealer',
    },
    'white_balverine': {
        # Gameflow stage 856 waits for Q_WhiteBalverineKnotholeGlade to be taken, 870 for it to complete. Strings in
        # the wasp translation unit: its Init 0x00E13C60 / Main 0x00E13F10 sit right after the wasp destructor
        # 0x00E13BF0 (the wasp_boss unit's range over-covers them); its allocator 0x00E183B0 opens the
        # Q_WhiteBalverineWW block (strings WBWW_WhiteBalverine, CS_WBW_DRINK), which ends at the next allocator 0x00E1A770.
        'evidence': ROOT / 'refs/script_recovery/white_balverine',
        'lo': 0xE13C20, 'hi': 0xE1A770,
        'ir_glob': 'Q_WhiteBalverine*.json',
        'scripts': ['Q_WhiteBalverineKnotholeGlade', 'Q_WhiteBalverineWW'],
        'pdb_pattern': '*CQ_WhiteBalverine*',
        'schema': 'white-balverine-native-inventory/0.1',
        'package': 'WhiteBalverine',
    },
    'arena': {
        # Gameflow stage 875 waits for Q_Arena to be taken, 900 for it to complete. Its own block is small: ctor
        # 0x00CFA660, Init 0x00CFA700 (the Arena regions, boast texts), OnPersist 0x00CFAB10 (ArenaState, ArenaRound,
        # GoldMultiplier ...), allocator 0x00CFABF0, dtor 0x00CFAC40; Q_AwakeningTheOracle begins at 0x00CFAD30. The
        # vtable's other slots (Main, workers, entities) live in the code block below.
        'evidence': ROOT / 'refs/script_recovery/arena',
        'lo': 0xF0F910, 'hi': 0xF28000,
        # the lifecycle block 0x00CFA660..0x00CFAD30 is anchored in define_addresses.txt; the range is the quest's
        # code block (Main, workers, the CCham / CShadow / CArenaEnemy / CArenaSpawn entities), after the
        # Q_AmbushTraders family and before Q_HeroSouls
        'ir_glob': 'Q_Arena*.json',
        # the inventory also binds Q_ArenaHoldingScript (allocator 0x00CF8860: entrance guards, fans)
        'scripts': ['Q_Arena', 'Q_ArenaHoldingScript'],
        'pdb_pattern': '*CQ_Arena*',
        'schema': 'arena-native-inventory/0.1',
        'package': 'Arena',
    },
    'expressions': {
        # The hero's expression actions and the global scripts, one contiguous block after CS_OakValeRevisited:
        # Global_WatchForHeroDeath 0x00EE8FE0, Expression_Fish 0x00EE95C0, _Wait 0x00EEA460, _Dig 0x00EEA7C0,
        # _Pickpocket 0x00EEAEF0, _Picklock 0x00EEB7B0, _Steal 0x00EEBDF0, Global_GiveHeroItemsFromRewardChest
        # 0x00EEC410, Global_OpenChest 0x00EEC890 ... up to the next family's allocator 0x00EED220.
        'evidence': ROOT / 'refs/script_recovery/expressions',
        'lo': 0xEE8E80, 'hi': 0xEED220,
        'ir_glob': '[EG][xl]*.json',     # Expression_* and Global_* only (a bare `*.json` loads every script's IR)
        'scripts': ['Global_WatchForHeroDeath', 'Expression_Fish', 'Expression_Follow', 'Expression_Wait', 'Expression_Dig',
                    'Expression_Pickpocket', 'Expression_Picklock', 'Expression_Steal', 'Global_TeleportToHeroGuild',
                    'Global_GiveHeroItemsFromRewardChest', 'Global_OpenChest',
                    'Global_DebugCycleThroughSpeech', 'Global_ToggleTimeDisplay'],   # (debug globals, in the same block)
        'pdb_pattern': '*CExpression_*',   # pdb/Ego_r-pdb-locals.tsv also holds the *CGlobal_* run (two patterns)
        'schema': 'expressions-native-inventory/0.1',
        'package': 'Expressions',
    },
    'tour_guide': {
        # Gameflow stage 200 activates V_TourGuide beside the QS_GuardianSisterInfo card.
        # Cluster vtable lifecycle (own block only): 0x00EE42A0 .. allocator 0x00EE6C00; the next family's
        # allocator is V_TrophyDealer 0x00EE80A0.
        'evidence': ROOT / 'refs/script_recovery/tour_guide',
        'lo': 0xEE42A0, 'hi': 0xEE80A0,
        'ir_glob': 'V_TourGuide*.json',
        'scripts': ['V_TourGuide'],
        'pdb_pattern': '*CV_TourGuide*',
        'schema': 'tour-guide-native-inventory/0.1',
        'package': 'TourGuide',
    },
    'gameflow_assistance': {
        # Gameflow stage 100 activates GameflowAssistance (priority 0 in the conversion queue; Aeon has a
        # hand port as LUAGameflowAssistance, which makes it an oracle for ours).
        # Cluster lifecycle 0x00CEFA00 .. allocator 0x00CF0640; next family Q_ArenaHoldingScript 0x00CF8860.
        'evidence': ROOT / 'refs/script_recovery/gameflow_assistance',
        'lo': 0xCEFA00, 'hi': 0xCF8860,
        'ir_glob': 'GameflowAssistance*.json',
        'scripts': ['GameflowAssistance'],
        'pdb_pattern': '*CGameflowAssistance*',
        'schema': 'gameflow-assistance-native-inventory/0.1',
        'package': 'GameflowAssistance',
    },
    'guild_master_village': {
        # Gameflow stage 100 activates V_GuildMaster -- the Guildmaster outside the training quests.
        # Cluster lifecycle (own block) 0x00E90780 .. allocator 0x00E92900; next family V_HiddenBooty 0x00E93CF0.
        'evidence': ROOT / 'refs/script_recovery/guild_master_village',
        'lo': 0xE90780, 'hi': 0xE93CF0,
        'ir_glob': 'V_GuildMaster*.json',
        'scripts': ['V_GuildMaster'],
        'pdb_pattern': '*CV_GuildMaster*',
        'schema': 'guild-master-village-native-inventory/0.1',
        'package': 'GuildMasterVillage',
    },
    'beggar_and_child': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xe57c00 .. next family's allocator (V_BodyGuard) 0xe62730.
        'evidence': ROOT / 'refs/script_recovery/beggar_and_child',
        'lo': 0xe57c00, 'hi': 0xe62730,
        'ir_glob': 'V_BeggarAndChild*.json',
        'scripts': ['V_BeggarAndChild'],
        'pdb_pattern': '*CV_BeggarAndChild*',
        'schema': 'beggar-and-child-native-inventory/0.1',
        'package': 'BeggarAndChild',
    },
    'book_collecting': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xe543b0 .. next family's allocator (V_BeggarAndChild) 0xe5d0b0.
        'evidence': ROOT / 'refs/script_recovery/book_collecting',
        'lo': 0xe543b0, 'hi': 0xe5d0b0,
        'ir_glob': 'V_BookCollecting*.json',
        'scripts': ['V_BookCollecting'],
        'pdb_pattern': '*CV_BookCollecting*',
        'schema': 'book-collecting-native-inventory/0.1',
        'package': 'BookCollecting',
    },
    'chicken_kicking': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xe628b0 .. next family's allocator (V_ChapelOfEvil) 0xe6e230.
        'evidence': ROOT / 'refs/script_recovery/chicken_kicking',
        'lo': 0xe628b0, 'hi': 0xe6e230,
        'ir_glob': 'V_ChickenKicking*.json',
        'scripts': ['V_ChickenKicking'],
        'pdb_pattern': '*CV_ChickenKicking*',
        'schema': 'chicken-kicking-native-inventory/0.1',
        'package': 'ChickenKicking',
    },
    'bordello': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xe399d0 .. next family's allocator (V_BanditCampPath) 0xe477a0.
        'evidence': ROOT / 'refs/script_recovery/bordello',
        'lo': 0xe399d0, 'hi': 0xe477a0,
        'ir_glob': 'V_Bordello*.json',
        'scripts': ['V_Bordello'],
        'pdb_pattern': '*CV_Bordello*',
        'schema': 'bordello-native-inventory/0.1',
        'package': 'Bordello',
    },
    'sick_child': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xec5420 .. next family's allocator (V_SingingStones) 0xed39a0.
        'evidence': ROOT / 'refs/script_recovery/sick_child',
        'lo': 0xec5420, 'hi': 0xed39a0,
        'ir_glob': 'V_SickChild*.json',
        'scripts': ['V_SickChild'],
        'pdb_pattern': '*CV_SickChild*',
        'schema': 'sick-child-native-inventory/0.1',
        'package': 'SickChild',
    },
    'picnic_after_wasp': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xec1240 .. next family's allocator (V_RandomPopulationSim) 0xec3bc0.
        'evidence': ROOT / 'refs/script_recovery/picnic_after_wasp',
        'lo': 0xec1240, 'hi': 0xec3bc0,
        'ir_glob': 'V_PicnicAreaAfterWaspBoss*.json',
        'scripts': ['V_PicnicAreaAfterWaspBoss'],
        'pdb_pattern': '*CV_PicnicAreaAfterWaspBoss*',
        'schema': 'picnic-after-wasp-native-inventory/0.1',
        'package': 'PicnicAreaAfterWaspBoss',
    },
    'oakvale_revisited': {
        # Gameflow switches this on before Orchard Farm. Cluster vtable lifecycle (own block only):
        # 0xee8210 .. next family's allocator (Global_WatchForHeroDeath) 0xee90a0.
        'evidence': ROOT / 'refs/script_recovery/oakvale_revisited',
        'lo': 0xee8210, 'hi': 0xee90a0,
        'ir_glob': 'CS_OakValeRevisited*.json',
        'scripts': ['CS_OakValeRevisited'],
        'pdb_pattern': '*CCS_OakValeRevisited*',
        'schema': 'oakvale-revisited-native-inventory/0.1',
        'package': 'OakValeRevisited',
    },
    # The six old named-cluster scripts (first-generation lifts under refs/script_recovery/lifted/<X>/FSE, Aeon has
    # hand ports of all of them). Range = the cluster's own lifecycle block up to the NEXT block's first lifecycle
    # function, not the next allocator: a family's allocator sits at the END of its block, so the allocator bound
    # (used for sick_child / book_collecting) swallows the neighbour's code -- sick_child's inventory lists
    # SingingStones' three threads (registered by 0x00ED10A0). 2026-09-27.
    'beardy_baldy': {
        'evidence': ROOT / 'refs/script_recovery/beardy_baldy',
        'lo': 0xE4F620, 'hi': 0xE543B0,
        'ir_glob': 'V_BeardyBaldy*.json',
        'scripts': ['V_BeardyBaldy'],
        'pdb_pattern': '*CV_BeardyBaldy*',
        'schema': 'beardy-baldy-native-inventory/0.1',
        'package': 'BeardyBaldy',
    },
    'dragon_boss_fight': {
        'evidence': ROOT / 'refs/script_recovery/dragon_boss_fight',
        'lo': 0xD254B0, 'hi': 0xD273A0,
        'ir_glob': 'Q_DragonBossFight*.json',
        'scripts': ['Q_DragonBossFight'],
        'pdb_pattern': '*CQ_DragonBossFight*',
        'schema': 'dragon-boss-fight-native-inventory/0.1',
        'package': 'DragonBossFight',
    },
    'heros_old_house': {
        'evidence': ROOT / 'refs/script_recovery/heros_old_house',
        'lo': 0xD89DA0, 'hi': 0xD8D640,
        'ir_glob': 'Q_HerosOldHouse*.json',
        'scripts': ['Q_HerosOldHouse'],
        'pdb_pattern': '*CQ_HerosOldHouse*',
        'schema': 'heros-old-house-native-inventory/0.1',
        'package': 'HerosOldHouse',
    },
    'singing_stones': {
        'evidence': ROOT / 'refs/script_recovery/singing_stones',
        'lo': 0xED0EC0, 'hi': 0xED3A80,
        'ir_glob': 'V_SingingStones*.json',
        'scripts': ['V_SingingStones'],
        'pdb_pattern': '*CV_SingingStones*',
        'schema': 'singing-stones-native-inventory/0.1',
        'package': 'SingingStones',
    },
    'statue_master': {
        'evidence': ROOT / 'refs/script_recovery/statue_master',
        'lo': 0xED3A80, 'hi': 0xED4B10,
        'ir_glob': 'V_StatueMaster*.json',
        'scripts': ['V_StatueMaster'],
        'pdb_pattern': '*CV_StatueMaster*',
        'schema': 'statue-master-native-inventory/0.1',
        'package': 'StatueMaster',
    },
    'summoning_the_ship': {
        'evidence': ROOT / 'refs/script_recovery/summoning_the_ship',
        'lo': 0xDF1230, 'hi': 0xDF4130,
        'ir_glob': 'Q_SummoningTheShip*.json',
        'scripts': ['Q_SummoningTheShip'],
        'pdb_pattern': '*CQ_SummoningTheShip*',
        'schema': 'summoning-the-ship-native-inventory/0.1',
        'package': 'SummoningTheShip',
    },
    'orchard_farm': {
        # Q_OrchardFarmRaid ctor 0x00DCC040 .. Q_OrchardFarmRaidGood dtor 0x00DD26C0 (+ tail)
        # Q_OrchardFarm_Barricade has no script class: it is a resource section quest activated by the raid.
        'evidence': ROOT / 'refs/script_recovery/orchard_farm',
        'lo': 0xDCC040, 'hi': 0xDD2700,
        'ir_glob': 'Q_OrchardFarmRaid*.json',
        'scripts': ['Q_OrchardFarmRaid', 'Q_OrchardFarmRaidEvil', 'Q_OrchardFarmRaidGood'],
        'pdb_pattern': '*CQ_OrchardFarm*',
        'schema': 'orchard-farm-native-inventory/0.1',
        'package': 'OrchardFarm',
    },
    'trader_conflict': {
        # Q_TraderConflictEvil ctor 0x00DF5CD0 .. Q_TraderConflictGood tail 0x00E00610 (CQ_GuildTrainingWill noise label = next family)
        'evidence': ROOT / 'refs/script_recovery/trader_conflict',
        'lo': 0xDF5CD0, 'hi': 0xE00610,
        'ir_glob': 'Q_TraderConflict*.json',
        'scripts': ['Q_TraderConflictEvil', 'Q_TraderConflictGood'],
        'pdb_pattern': '*CQ_TraderConflict*',
        'schema': 'trader-conflict-native-inventory/0.1',
        'package': 'TraderConflict',
    },
    'gameflow': {
        # The master campaign-stage script (not a Q_ quest; no entity bindings, two spawned threads).
        # CGameflowScript ctor 0x00CE6CB0 (stores vtable 0x012C3FA4; disassembly-verified) .. Alloc
        # 0x00CEF950 / dtor 0x00CEF9A0; 0x00CEF9D0 is the CGameflowAssistanceScript ctor (vtable
        # 0x012C5DE0) = the next family. Vtable 0x012C3FA4 = dtor CEF9A0, RegisterMain CE75B0,
        # Main CE7670, Init CE6CF0, OnPersist CEF8E0 (native_clusters/Gameflow.json agrees).
        'evidence': ROOT / 'refs/script_recovery/gameflow',
        'lo': 0xCE6CB0, 'hi': 0xCEF9D0,
        'ir_glob': 'Gameflow.json',
        'scripts': ['Gameflow'],
        'pdb_pattern': '*CGameflowScript*',
        'schema': 'gameflow-native-inventory/0.1',
        'package': 'Gameflow',
    },
}


def unit(name):
    if name not in UNITS:
        raise KeyError(f'unknown script unit {name!r}; known: {sorted(UNITS)}')
    return UNITS[name]
