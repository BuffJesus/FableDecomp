# Script split proposal for the Aeon collaboration (generated 2026-09-17)

Aeon proposed converting every Fable TLC quest script to FSE Lua, splitting the work and cross-referencing the results.
This is the inventory to split from: the 161 native script clusters in `refs/script_recovery/conversion_queue.tsv`.
Function counts / bytes = retail functions whose bsim donor is a `NScript::C<Name>Script` member
(`ghidra_out/bsim_port_audit_final.tsv` x `ghidra_out/coverage.tsv`; 0 = no bsim-named members, size unknown, not "empty").
140 scripts are unassigned. Our side converts mechanically (`convert_quest_unit.py` -> `build_readable_unit.py`,
`docs/scripts/READABLE_STYLE_PLAN.md`); Aeon hand-ports. Cross-reference tooling: `tools/script_recovery/audit_port_against_pdb.py`,
`work/aeon_new_oakvale/CROSSREF.md`; bindings the converter output needs upstream: `docs/scripts/FSE_UPSTREAM_REQUIREMENTS.md`.

Suggested rule: trade whole `script_units.py` units (one contiguous retail range = one quest family, e.g. Q_OrchardFarmRaid +
Evil + Good); keep Aeon's 20 ported packages as the cross-reference oracle; whoever takes a script owns its entity scripts too.

| script | kind | retail fns (bsim-named) | bytes | status |
|---|---|---:|---:|---|
| BenTestScripts | test | 0 | 0 |  |
| CS_OakValeRevisited | cutscene_host | 6 | 1404 |  |
| CS_PlayCutscene | cutscene_host | 0 | 0 |  |
| DanyalTestScripts | master | 0 | 0 |  |
| Empty | master | 0 | 0 |  |
| Expression_Dig | expression | 0 | 0 |  |
| Expression_Fish | expression | 0 | 0 |  |
| Expression_Follow | expression | 0 | 0 |  |
| Expression_Picklock | expression | 0 | 0 |  |
| Expression_Pickpocket | expression | 0 | 0 |  |
| Expression_Steal | expression | 0 | 0 |  |
| Expression_Wait | expression | 0 | 0 |  |
| Gameflow | global | 0 | 0 |  |
| GameflowAssistance | global | 0 | 0 |  |
| Global_DebugCycleThroughSpeech | global | 0 | 0 |  |
| Global_GiveHeroItemsFromRewardChest | global | 0 | 0 |  |
| Global_OpenChest | global | 0 | 0 |  |
| Global_TeleportToHeroGuild | global | 0 | 0 |  |
| Global_ToggleTimeDisplay | global | 0 | 0 |  |
| Global_WatchForHeroDeath | global | 0 | 0 |  |
| GuildSealTriggerTest | test | 0 | 0 |  |
| HeroBoasts | script | 0 | 0 |  |
| HeroTurningDebugger | test | 0 | 0 |  |
| HitSwitchTest | test | 0 | 0 |  |
| LadyGreyWifeManager | script | 0 | 0 |  |
| MarkTestScripts | test | 0 | 0 |  |
| OracleMinigameTest | test | 0 | 0 |  |
| PersonalScriptMain | personal | 0 | 0 |  |
| PersonalScript_Aardvark | personal | 0 | 0 |  |
| PersonalScript_Empty | personal | 0 | 0 |  |
| PersonalScript_GlobalThings | personal | 0 | 0 |  |
| PersonalScript_alextest | personal | 0 | 0 |  |
| ProximityPodiumTest | test | 0 | 0 |  |
| QR_EscortTrader | repeatable_quest | 0 | 0 |  |
| QR_EscortTrader_Manager | repeatable_quest | 0 | 0 |  |
| QS_GuardianSisterInfo | quest | 0 | 0 | Aeon (hand port) |
| QS_GuardianSisterInfo2_SisterInBanditCamp | quest | 0 | 0 | Aeon (hand port) |
| QS_GuardianTrophyDealerInfo | quest | 1 | 17 | Aeon (hand port) |
| QS_MeetSister | quest | 0 | 0 | Aeon (hand port) |
| QS_ScytheInfo | quest | 2 | 315 |  |
| Q_AmbushTraders | quest | 23 | 11869 |  |
| Q_Arena | quest | 13 | 2599 |  |
| Q_ArenaHoldingScript | quest | 0 | 0 |  |
| Q_AwakeningTheOracle | quest | 3 | 2003 |  |
| Q_BanditCamp | quest | 4 | 3349 |  |
| Q_BanditCampBossBattle | quest | 2 | 3197 |  |
| Q_BanditCampHoldingScript | quest | 1 | 416 |  |
| Q_BountyHunt | quest | 17 | 3206 |  |
| Q_BowerstoneTownLifeIntro | quest | 0 | 0 |  |
| Q_BreakSiege | quest | 4 | 1730 |  |
| Q_CinemaTest | quest | 6 | 1300 |  |
| Q_DragonBossFight | quest | 6 | 1603 | Aeon (hand port) |
| Q_EndGame | quest | 4 | 3871 |  |
| Q_EndGameBossBattle | quest | 2 | 1405 |  |
| Q_EndGameFocalSites | quest | 2 | 2174 |  |
| Q_FireHeart | quest | 14 | 4959 |  |
| Q_GuildTraining | quest | 12 | 5312 | BuffJesus (converter) |
| Q_GuildTrainingDeparture | quest | 0 | 0 | BuffJesus (converter) |
| Q_GuildTrainingMelee | quest | 1 | 633 | BuffJesus (converter) |
| Q_GuildTrainingPreMelee | quest | 1 | 592 | BuffJesus (converter) |
| Q_GuildTrainingSkill | quest | 3 | 86 | BuffJesus (converter) |
| Q_GuildTrainingWill | quest | 3 | 225 | BuffJesus (converter) |
| Q_GuildTrainingWoodsDeparture | quest | 1 | 569 | BuffJesus (converter) |
| Q_GuildTrainingWoodsMelee | quest | 2 | 1078 | BuffJesus (converter) |
| Q_GuildTrainingWoodsWill | quest | 1 | 59 | BuffJesus (converter) |
| Q_HangingTreeEvil | quest | 4 | 2413 |  |
| Q_HangingTreeGood | quest | 21 | 13976 |  |
| Q_HeroSouls | quest | 1 | 5390 |  |
| Q_HeroSoulsArena | quest | 10 | 6696 |  |
| Q_HeroSoulsBriar | quest | 2 | 952 |  |
| Q_HeroSoulsGuildmaster | quest | 6 | 5490 |  |
| Q_HeroSoulsMother | quest | 2 | 1922 |  |
| Q_HeroSoulsNostro | quest | 4 | 2308 |  |
| Q_HeroSoulsThunder | quest | 1 | 1388 |  |
| Q_HerosOldHouse | quest | 0 | 0 | Aeon (hand port) |
| Q_HobbeCave | quest | 9 | 6834 |  |
| Q_HobbeToothContest | quest | 4 | 1599 |  |
| Q_MinionCamp | quest | 21 | 6220 |  |
| Q_MinionClifftopChase | quest | 12 | 3347 |  |
| Q_NewOakValeIntro | quest | 21 | 8256 | BuffJesus (converter) |
| Q_OakValeBanditRaid | quest | 3 | 1216 |  |
| Q_OpeningGraveyardSecretPassage | quest | 5 | 3339 |  |
| Q_OrchardFarmRaid | quest | 8 | 3441 | BuffJesus (converter) |
| Q_OrchardFarmRaidEvil | quest | 2 | 1068 | BuffJesus (converter) |
| Q_OrchardFarmRaidGood | quest | 0 | 0 | BuffJesus (converter) |
| Q_PrisonEscapeRescueMother | quest | 11 | 11408 |  |
| Q_PrisonRace | quest | 1 | 66 |  |
| Q_PrisonWardenGame | quest | 11 | 2861 |  |
| Q_RansomVictim | quest | 4 | 4276 |  |
| Q_RansomVictimChiefsHouse | quest | 0 | 0 |  |
| Q_SecretPassage | quest | 8 | 4802 |  |
| Q_SummoningTheShip | quest | 8 | 3978 |  |
| Q_SunnyvaleMaster | master | 11 | 264 |  |
| Q_TentacleKrakenBossFight | quest | 4 | 1894 |  |
| Q_TraderConflictEvil | quest | 8 | 7036 |  |
| Q_TraderConflictGood | quest | 9 | 7336 |  |
| Q_TraderEscort | quest | 11 | 5827 |  |
| Q_UndeadRising | quest | 6 | 3423 |  |
| Q_WaspBoss | quest | 2 | 313 |  |
| Q_WhiteBalverineKnotholeGlade | quest | 2 | 6022 |  |
| Q_WhiteBalverineWW | quest | 1 | 492 |  |
| Q_WizardBattle | quest | 5 | 1391 |  |
| ShowTargetedThingHealth | test | 0 | 0 |  |
| TestNewNav1 | test | 0 | 0 |  |
| TestNewNav2 | test | 0 | 0 |  |
| TestNewNav3 | test | 0 | 0 |  |
| TestNewNav4 | test | 0 | 0 |  |
| TestNewNav5 | test | 0 | 0 |  |
| TestQuestCard1 | test | 0 | 0 |  |
| TestQuestCard2 | test | 0 | 0 |  |
| TestQuestCard3 | test | 0 | 0 |  |
| TestTheInventory | test | 0 | 0 |  |
| TonyTestScripts | test | 0 | 0 |  |
| V_AmbushScam | village | 0 | 0 |  |
| V_ArcheryCompetition | village | 0 | 0 |  |
| V_AssassinAttacks | village | 0 | 0 |  |
| V_BanditCampPath | village | 0 | 0 |  |
| V_BanditToll | village | 0 | 0 |  |
| V_BeardyBaldy | village | 0 | 0 |  |
| V_BeggarAndChild | village | 0 | 0 |  |
| V_BodyGuard | village | 0 | 0 |  |
| V_BookCollecting | village | 0 | 0 |  |
| V_Bordello | village | 0 | 0 |  |
| V_ChapelOfEvil | village | 0 | 0 |  |
| V_ChickenKicking | village | 0 | 0 |  |
| V_DemonDoors | village | 0 | 0 |  |
| V_ExposeMayor | village | 0 | 0 |  |
| V_Fisherman | village | 0 | 0 |  |
| V_FishingCompetition | village | 0 | 0 |  |
| V_FisticuffsClub | village | 0 | 0 |  |
| V_GhostGrannyNecklace | village | 0 | 0 |  |
| V_GuildMaster | village | 0 | 0 |  |
| V_HauntedHouse | village | 0 | 0 |  |
| V_HeroDolls | village | 0 | 0 |  |
| V_HeroDuel | village | 0 | 0 |  |
| V_HiddenBooty | village | 0 | 0 |  |
| V_IntroductionToTrophies | village | 0 | 0 |  |
| V_KnotholeGladeGates | village | 0 | 0 |  |
| V_LostTrader | village | 0 | 0 |  |
| V_MayorsInvitation | village | 0 | 0 |  |
| V_MazeResearch | village | 0 | 0 | Aeon (hand port) |
| V_MurderTwist | village | 0 | 0 |  |
| V_MurderTwistFinal | village | 0 | 0 |  |
| V_Oracle | village | 0 | 0 |  |
| V_ParanoidWhispers | village | 0 | 0 |  |
| V_PicnicAreaAfterWaspBoss | village | 0 | 0 |  |
| V_RandomPopulationSim | village | 0 | 0 |  |
| V_RockTrollFirstEncounter | village | 0 | 0 |  |
| V_SickChild | village | 0 | 0 |  |
| V_SingingStones | village | 0 | 0 |  |
| V_StatueMaster | village | 0 | 0 | Aeon (hand port) |
| V_SwordInTheStone | village | 0 | 0 |  |
| V_TalentlessBard | village | 0 | 0 |  |
| V_TempleOfLight | village | 0 | 0 |  |
| V_TourGuide | village | 0 | 0 |  |
| V_TravellingHeroes | village | 0 | 0 |  |
| V_TrophyDealer | village | 0 | 0 |  |
| alextest | master | 0 | 0 |  |
| bandit_level_cutscene | script | 0 | 0 |  |
| guy_script | script | 0 | 0 |  |
| joss_test_scripts | master | 0 | 0 |  |

Totals: 372 bsim-named retail functions, 191215 bytes across the queue.
