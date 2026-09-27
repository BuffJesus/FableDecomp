# Full singing stones native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_SingingStones | V_SingingStones | Main | 0x00ed10a0 | True | 0 |
| V_SingingStones | V_SingingStones | Init | 0x00ed0ec0 | True | 4 |
| V_SingingStones | V_SingingStones | OnPersist | 0x00ed1010 | True | 0 |
| V_SingingStones | V_SingingStones | WatchForCompleteTune | 0x00ed2bb0 | True | 11 |
| V_SingingStones | V_SingingStones | WatchForSpokenToDemonDoors | 0x00ed13b0 | True | 0 |
| V_SingingStones | V_SingingStones | WatchForRegionLeaving | 0x00ed2a50 | True | 2 |
| V_SingingStones | SingingStone | Main | 0x00ed3370 | True | 25 |
| V_SingingStones | SingingStone | Init | 0x00ed14e0 | True | 0 |
| V_SingingStones | SingingStone | OnPersist | 0x00ed14d0 | True | 0 |
| V_SingingStones | SingingStone | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SingingStones | ManWithDoorName | Main | 0x00ed17d0 | True | 5 |
| V_SingingStones | ManWithDoorName | Init | 0x00ed1790 | True | 0 |
| V_SingingStones | ManWithDoorName | OnPersist | 0x00cdebc0 | True | 0 |
| V_SingingStones | ManWithDoorName | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 3, "functions": 14, "missing": 0, "functionSyntaxPassed": 14, "fileSyntaxPassed": 3, "fileSyntaxChecked": 3, "todo": 47}`
