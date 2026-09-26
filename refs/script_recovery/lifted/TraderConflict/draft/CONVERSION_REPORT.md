# Full trader conflict native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| Q_TraderConflictEvil | Q_TraderConflictEvil | Main | 0x00df6010 | True | 6 |
| Q_TraderConflictEvil | Q_TraderConflictEvil | Init | 0x00df5cd0 | True | 0 |
| Q_TraderConflictEvil | Q_TraderConflictEvil | OnPersist | 0x00cbd4e0 | True | 0 |
| Q_TraderConflictEvil | Q_TraderConflictEvil | WatchTimeLimit | 0x00df7980 | True | 0 |
| Q_TraderConflictEvil | Q_TraderConflictEvil | UpdateLiveEnemies | 0x00df9b80 | True | 0 |
| Q_TraderConflictEvil | Q_TraderConflictEvil | helper_DF9E00 | 0x00df9e00 | True | 0 |
| Q_TraderConflictEvil | TC_GuardSpawnPoint | Main | 0x00df7bf0 | True | 1 |
| Q_TraderConflictEvil | TC_GuardSpawnPoint | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictEvil | TC_GuardSpawnPoint | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictEvil | TC_GuardSpawnPoint | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFollower | Main | 0x00df80b0 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFollower | Init | 0x00df8040 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFollower | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFollower | OnPredicateFail | 0x00df8050 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFighter | Main | 0x00df8970 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFighter | Init | 0x00df8940 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFighter | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictEvil | TC_BanditFighter | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictEvil | TC_Villager | Main | 0x00df9180 | True | 0 |
| Q_TraderConflictEvil | TC_Villager | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictEvil | TC_Villager | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictEvil | TC_Villager | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictEvil | IsAGuard | Main | 0x00df9710 | True | 0 |
| Q_TraderConflictEvil | IsAGuard | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictEvil | IsAGuard | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictEvil | IsAGuard | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | Main | 0x00dfa450 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | Init | 0x00dfa0e0 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | OnPersist | 0x00cbd4e0 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | WatchTimeLimit | 0x00dfaeb0 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | WatchForRegionTransitions | 0x00dfc920 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | WatchForHittingEnemies | 0x00dfc630 | True | 1 |
| Q_TraderConflictGood | Q_TraderConflictGood | WatchForTradersFreed | 0x00dfcc10 | True | 7 |
| Q_TraderConflictGood | Q_TraderConflictGood | WatchForKilledPeople | 0x00dfc290 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | UpdateLiveEnemies | 0x00dfc320 | True | 0 |
| Q_TraderConflictGood | Q_TraderConflictGood | AttackPeople | 0x00dfd600 | True | 8 |
| Q_TraderConflictGood | Q_TraderConflictGood | helper_DFDED0 | 0x00dfded0 | True | 0 |
| Q_TraderConflictGood | TraderToRescue | Main | 0x00dfe0f0 | True | 3 |
| Q_TraderConflictGood | TraderToRescue | Init | 0x00dfb1b0 | True | 0 |
| Q_TraderConflictGood | TraderToRescue | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | TraderToRescue | OnPredicateFail | 0x00dfb100 | True | 0 |
| Q_TraderConflictGood | TC_BanditGuard | Main | 0x00dfb320 | True | 0 |
| Q_TraderConflictGood | TC_BanditGuard | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictGood | TC_BanditGuard | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | TC_BanditGuard | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictGood | TC_BanditHostageKeeper | Main | 0x00dfba60 | True | 0 |
| Q_TraderConflictGood | TC_BanditHostageKeeper | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictGood | TC_BanditHostageKeeper | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | TC_BanditHostageKeeper | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictGood | BCMTrader | Main | 0x00dfbda0 | True | 0 |
| Q_TraderConflictGood | BCMTrader | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictGood | BCMTrader | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | BCMTrader | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictGood | BCGameMaster | Main | 0x00dfbe90 | True | 0 |
| Q_TraderConflictGood | BCGameMaster | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictGood | BCGameMaster | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | BCGameMaster | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictGood | BanditExtra | Main | 0x00dfca90 | True | 0 |
| Q_TraderConflictGood | BanditExtra | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictGood | BanditExtra | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | BanditExtra | OnPredicateFail | 0x00cdebd0 | True | 0 |
| Q_TraderConflictGood | CampHostageDoor | Main | 0x00dfc050 | True | 0 |
| Q_TraderConflictGood | CampHostageDoor | Init | 0x00cdebb0 | True | 0 |
| Q_TraderConflictGood | CampHostageDoor | OnPersist | 0x00cdebc0 | True | 0 |
| Q_TraderConflictGood | CampHostageDoor | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 14, "functions": 65, "missing": 0, "functionSyntaxPassed": 65, "fileSyntaxPassed": 15, "fileSyntaxChecked": 15, "todo": 26}`
