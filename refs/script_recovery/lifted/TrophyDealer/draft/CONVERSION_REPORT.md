# Full trophy dealer native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_TrophyDealer | V_TrophyDealer | Main | 0x00ee6e90 | True | 0 |
| V_TrophyDealer | V_TrophyDealer | Init | 0x00ee6d60 | True | 0 |
| V_TrophyDealer | V_TrophyDealer | OnPersist | 0x00cbd4e0 | True | 0 |
| V_TrophyDealer | V_TrophyDealer | HilightGuildTeleporter | 0x00ee71a0 | True | 0 |
| V_TrophyDealer | TrophyDealerInCave | Main | 0x00ee76f0 | True | 0 |
| V_TrophyDealer | TrophyDealerInCave | Init | 0x00ee73f0 | True | 0 |
| V_TrophyDealer | TrophyDealerInCave | OnPersist | 0x00cdebc0 | True | 0 |
| V_TrophyDealer | TrophyDealerInCave | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_TrophyDealer | DemonDoorFace | Main | 0x00ee7a20 | True | 2 |
| V_TrophyDealer | DemonDoorFace | Init | 0x00ee74e0 | True | 0 |
| V_TrophyDealer | DemonDoorFace | OnPersist | 0x00cdebc0 | True | 0 |
| V_TrophyDealer | DemonDoorFace | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 3, "functions": 12, "missing": 0, "functionSyntaxPassed": 12, "fileSyntaxPassed": 3, "fileSyntaxChecked": 3, "todo": 2}`
