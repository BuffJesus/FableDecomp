# Full New Oakvale native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---:|
| NOVI_AffairMan | Init | 0x00DB0950 | True | 0 |
| NOVI_AffairMan | Main | 0x00DB09E0 | True | 36 |
| NOVI_AffairWife | Init | 0x00DB2A70 | True | 0 |
| NOVI_AffairWife | Main | 0x00DB2B10 | False | 62 |
| NOVI_AffairWoman | Init | 0x00DB1E80 | True | 0 |
| NOVI_AffairWoman | Main | 0x00DB1F00 | False | 45 |
| NOVI_Barrel | Init | 0x00CDEBB0 | True | 0 |
| NOVI_Barrel | Main | 0x00DB7E10 | True | 0 |
| NOVI_Barrel | OnPredicateFail | 0x00DB7DB0 | True | 3 |
| NOVI_BarrelMan | Init | 0x00DB5260 | True | 5 |
| NOVI_BarrelMan | Main | 0x00DB5330 | False | 109 |
| NOVI_BarrelThug | Init | 0x00DB6BF0 | True | 0 |
| NOVI_BarrelThug | Main | 0x00DB6C60 | True | 95 |
| NOVI_BookTrader | Init | 0x00DB3F00 | True | 0 |
| NOVI_BookTrader | Main | 0x00DB3FA0 | True | 30 |
| NOVI_Bully | Init | 0x00DAED30 | True | 0 |
| NOVI_Bully | Main | 0x00DBB310 | False | 143 |
| NOVI_Bully | GivenTeddy | 0x00DBCD00 | True | 0 |
| NOVI_CreatedBeetle | Init | 0x00CDEBB0 | True | 0 |
| NOVI_CreatedBeetle | Main | 0x00DB80C0 | True | 0 |
| NOVI_Guard | Init | 0x00DAC650 | True | 0 |
| NOVI_Guard | Main | 0x00DAC760 | True | 133 |
| NOVI_LiveFather | Init | 0x00DAC390 | True | 0 |
| NOVI_LiveFather | Main | 0x00DB86B0 | True | 130 |
| NOVI_TeddyGirl | Init | 0x00DAF000 | True | 0 |
| NOVI_TeddyGirl | Main | 0x00DAF080 | True | 133 |
| NOVI_TeddyGirl | GivenTeddy | 0x00DB0600 | True | 0 |
| NOVI_Theresa | Init | 0x00DAC4F0 | True | 0 |
| NOVI_Theresa | Main | 0x00DB97A0 | True | 182 |
| NOVI_Victim | Init | 0x00DAEEB0 | True | 0 |
| NOVI_Victim | Main | 0x00DBCD60 | True | 100 |
| NOVI_Villager | Init | 0x00DADF00 | True | 0 |
| NOVI_Villager | Main | 0x00DADF80 | True | 49 |
| NOVI_Villager | GetVillagerSpeechIndex | 0x00DAEBF0 | True | 0 |
| OVI_DeadFather | Init | 0x00DB8290 | True | 3 |
| OVI_DeadFather | Main | 0x00DB8300 | True | 3 |
| OVI_DeadFather | OnPredicateFail | 0x00DB8260 | True | 0 |
| Q_NewOakValeIntro | RegisterMain | 0x00DAACE0 | True | 0 |
| Q_NewOakValeIntro | Main | 0x00DABAC0 | True | 6 |
| Q_NewOakValeIntro | Init | 0x00DAADD0 | True | 0 |
| Q_NewOakValeIntro | OnPersist | 0x00DAADA0 | True | 0 |
| Q_NewOakValeIntro | DoMission | 0x00DBDE40 | True | 8 |
| Q_NewOakValeIntro | AttackStuff | 0x00DBE3C0 | True | 2 |
| Q_NewOakValeIntro | PostAttackStuff | 0x00DBEB20 | True | 9 |
| Q_NewOakValeIntro | ManageQuestCoreMarkers | 0x00DBE4E0 | True | 1 |
| Q_NewOakValeIntro | StartBarrelTimer | 0x00DB4F70 | True | 0 |
| Q_NewOakValeIntro | WatchBarrels | 0x00DBE890 | True | 8 |
| Q_NewOakValeIntro | WatchForGotGold | 0x00DBE2E0 | True | 0 |
| Q_NewOakValeIntro | AddGoodDeed | 0x00DB0660 | True | 0 |
| Q_NewOakValeIntro | AddBadDeed | 0x00DAEA70 | True | 0 |
| Q_NewOakValeIntro | destructor | 0x00DBEFA0 | True | 1 |

Shared native helper module: `FSE/NewOakValeIntro/native_quest_helpers.lua`; functions: AddBadDeed, AddGoodDeed. Its diagnostics are included in file syntax totals.

Summary: `{"owners": 17, "functions": 51, "missing": 0, "functionSyntaxPassed": 47, "fileSyntaxPassed": 14, "fileSyntaxChecked": 18, "todo": 1296}`
