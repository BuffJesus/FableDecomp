# Full orchard farm native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | Main | 0x00dcc770 | True | 0 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | Init | 0x00dcc140 | True | 13 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | OnPersist | 0x00dcc720 | True | 0 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | ProcessGameRulesEvil | 0x00dd03d0 | False | 42 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | ProcessGameRulesGood | 0x00dd0f60 | False | 51 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | DoCutsceneIfRequired | 0x00dcfa60 | False | 37 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | WatchForExternalScriptDeactivation | 0x00dccf30 | True | 4 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | MakeTeamMemberComment | 0x00dcda80 | False | 8 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | DoMultiplierCutscene | 0x00dd1af0 | False | 17 |
| Q_OrchardFarmRaid | Q_OrchardFarmRaid | ReplaceQuestCards | 0x00dd0eb0 | True | 0 |
| Q_OrchardFarmRaid | GuardTeamSpawn | Main | 0x00dcd350 | False | 13 |
| Q_OrchardFarmRaid | GuardTeamSpawn | Init | 0x00dcd1d0 | False | 11 |
| Q_OrchardFarmRaid | GuardTeamSpawn | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | GuardTeamSpawn | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_OrchardFarmRaid | BanditTeamSpawn | Main | 0x00dcd350 | False | 13 |
| Q_OrchardFarmRaid | BanditTeamSpawn | Init | 0x00dcd1d0 | False | 11 |
| Q_OrchardFarmRaid | BanditTeamSpawn | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | BanditTeamSpawn | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_OrchardFarmRaid | Artefact | Main | 0x00dcdc50 | True | 21 |
| Q_OrchardFarmRaid | Artefact | Init | 0x00dcfa10 | True | 2 |
| Q_OrchardFarmRaid | Artefact | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | Artefact | OnPredicateFail | 0x00dcf920 | False | 5 |
| Q_OrchardFarmRaid | OrchardFarmWhisper | Main | 0x00dcf0b0 | False | 4 |
| Q_OrchardFarmRaid | OrchardFarmWhisper | Init | 0x00dcf000 | False | 3 |
| Q_OrchardFarmRaid | OrchardFarmWhisper | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | OrchardFarmWhisper | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_OrchardFarmRaid | GuardTeamMember | Main | 0x00dce230 | False | 66 |
| Q_OrchardFarmRaid | GuardTeamMember | Init | 0x00dcdf60 | False | 9 |
| Q_OrchardFarmRaid | GuardTeamMember | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | GuardTeamMember | OnPredicateFail | 0x00dcded0 | True | 5 |
| Q_OrchardFarmRaid | GuardTeamMember | GoOnPatrol | 0x00dcec70 | True | 4 |
| Q_OrchardFarmRaid | GuardTeamMember | IsThingCarryingCrate | 0x00dced10 | False | 1 |
| Q_OrchardFarmRaid | GuardTeamMember | GetNearestCrateToMe | 0x00dcedf0 | False | 8 |
| Q_OrchardFarmRaid | GuardTeamMember | helper_DCEC50 | 0x00dcec50 | True | 4 |
| Q_OrchardFarmRaid | BanditTeamMember | Main | 0x00dce230 | False | 66 |
| Q_OrchardFarmRaid | BanditTeamMember | Init | 0x00dcdf60 | False | 9 |
| Q_OrchardFarmRaid | BanditTeamMember | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | BanditTeamMember | OnPredicateFail | 0x00dcded0 | True | 5 |
| Q_OrchardFarmRaid | BanditTeamMember | GoOnPatrol | 0x00dcec70 | True | 4 |
| Q_OrchardFarmRaid | BanditTeamMember | IsThingCarryingCrate | 0x00dced10 | False | 1 |
| Q_OrchardFarmRaid | BanditTeamMember | GetNearestCrateToMe | 0x00dcedf0 | False | 8 |
| Q_OrchardFarmRaid | BanditTeamMember | helper_DCEC50 | 0x00dcec50 | True | 4 |
| Q_OrchardFarmRaid | FarmRearEntrance | Main | 0x00dcf4c0 | True | 0 |
| Q_OrchardFarmRaid | FarmRearEntrance | Init | 0x00dcf480 | True | 0 |
| Q_OrchardFarmRaid | FarmRearEntrance | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | FarmRearEntrance | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_OrchardFarmRaid | M_WhisperFarmRaidIntro | Main | 0x00dd1ac0 | True | 0 |
| Q_OrchardFarmRaid | M_WhisperFarmRaidIntro | Init | 0x00cdebb0 | True | 0 |
| Q_OrchardFarmRaid | M_WhisperFarmRaidIntro | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | M_WhisperFarmRaidIntro | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_OrchardFarmRaid | MK_OFI_GWLL_WHIS2 | Main | 0x00dd1eb0 | True | 0 |
| Q_OrchardFarmRaid | MK_OFI_GWLL_WHIS2 | Init | 0x00cdebb0 | True | 0 |
| Q_OrchardFarmRaid | MK_OFI_GWLL_WHIS2 | OnPersist | 0x00cdebc0 | True | 0 |
| Q_OrchardFarmRaid | MK_OFI_GWLL_WHIS2 | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_OrchardFarmRaidEvil | Q_OrchardFarmRaidEvil | Main | 0x00dd2010 | True | 0 |
| Q_OrchardFarmRaidEvil | Q_OrchardFarmRaidEvil | Init | 0x00dd2020 | False | 10 |
| Q_OrchardFarmRaidGood | Q_OrchardFarmRaidGood | Main | 0x00dd2390 | True | 0 |
| Q_OrchardFarmRaidGood | Q_OrchardFarmRaidGood | Init | 0x00dd23a0 | False | 10 |

Summary: `{"owners": 12, "functions": 58, "missing": 2, "functionSyntaxPassed": 36, "fileSyntaxPassed": 3, "fileSyntaxChecked": 13, "todo": 469}`
