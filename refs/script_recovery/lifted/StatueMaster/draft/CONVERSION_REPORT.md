# Full statue master native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_StatueMaster | V_StatueMaster | Main | 0x00ed3b30 | True | 0 |
| V_StatueMaster | V_StatueMaster | Init | 0x00ed3a80 | True | 0 |
| V_StatueMaster | V_StatueMaster | GetStatuePointingPosition | 0x00ed4420 | True | 0 |
| V_StatueMaster | V_StatueMaster | helper_ED43D0 | 0x00ed43d0 | True | 0 |
| V_StatueMaster | StatueMasterStatue | Main | 0x00ed41d0 | True | 0 |
| V_StatueMaster | StatueMasterStatue | Init | 0x00ed4170 | True | 0 |
| V_StatueMaster | StatueMasterStatue | OnPersist | 0x00cdebc0 | True | 0 |
| V_StatueMaster | StatueMasterStatue | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_StatueMaster | StatueMasterCellarDoors | Main | 0x00ed45a0 | True | 0 |
| V_StatueMaster | StatueMasterCellarDoors | Init | 0x00ed4570 | True | 0 |
| V_StatueMaster | StatueMasterCellarDoors | OnPersist | 0x00cdebc0 | True | 0 |
| V_StatueMaster | StatueMasterCellarDoors | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_StatueMaster | StatueMasterChest | Main | 0x00ed4830 | True | 0 |
| V_StatueMaster | StatueMasterChest | Init | 0x00ed4820 | True | 0 |
| V_StatueMaster | StatueMasterChest | OnPersist | 0x00cdebc0 | True | 0 |
| V_StatueMaster | StatueMasterChest | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 4, "functions": 16, "missing": 1, "functionSyntaxPassed": 16, "fileSyntaxPassed": 5, "fileSyntaxChecked": 5, "todo": 0}`
