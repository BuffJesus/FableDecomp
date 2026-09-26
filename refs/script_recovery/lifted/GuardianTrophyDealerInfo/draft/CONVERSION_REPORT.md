# Full guardian trophy dealer info native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| QS_GuardianTrophyDealerInfo | QS_GuardianTrophyDealerInfo | Main | 0x00e27920 | True | 0 |
| QS_GuardianTrophyDealerInfo | QS_GuardianTrophyDealerInfo | Init | 0x00e27870 | True | 0 |
| QS_GuardianTrophyDealerInfo | QS_GuardianTrophyDealerInfo | OnPersist | 0x00e27e60 | True | 0 |
| QS_GuardianTrophyDealerInfo | QS_GuardianTrophyDealerInfo | WaitForPieceOver | 0x00e27ac0 | True | 1 |
| QS_GuardianTrophyDealerInfo | GTDI_Maze | Main | 0x00e27e90 | True | 2 |
| QS_GuardianTrophyDealerInfo | GTDI_Maze | Init | 0x00e27c60 | True | 0 |
| QS_GuardianTrophyDealerInfo | GTDI_Maze | OnPersist | 0x00cdebc0 | True | 0 |
| QS_GuardianTrophyDealerInfo | GTDI_Maze | OnPredicateFail | 0x00e27c80 | True | 0 |

Summary: `{"owners": 2, "functions": 8, "missing": 0, "functionSyntaxPassed": 8, "fileSyntaxPassed": 2, "fileSyntaxChecked": 2, "todo": 3}`
