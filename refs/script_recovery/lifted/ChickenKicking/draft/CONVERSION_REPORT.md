# Full chicken kicking native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_ChickenKicking | V_ChickenKicking | Main | 0x00e62a00 | True | 0 |
| V_ChickenKicking | V_ChickenKicking | Init | 0x00e628b0 | True | 0 |
| V_ChickenKicking | V_ChickenKicking | OnPersist | 0x00e629a0 | True | 0 |
| V_ChickenKicking | V_ChickenKicking | CreateSpectators | 0x00e62d60 | True | 0 |
| V_ChickenKicking | V_ChickenKicking | LookAfterOrganiser | 0x00e631f0 | True | 0 |
| V_ChickenKicking | V_ChickenKicking | helper_E68B20 | 0x00e68b20 | True | 0 |
| V_ChickenKicking | ChickenMaster | Main | 0x00e64fb0 | True | 38 |
| V_ChickenKicking | ChickenMaster | Init | 0x00e63560 | True | 0 |
| V_ChickenKicking | ChickenMaster | OnPersist | 0x00e641b0 | True | 3 |
| V_ChickenKicking | ChickenMaster | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_ChickenKicking | ChickenSign | Main | 0x00e64df0 | True | 0 |
| V_ChickenKicking | ChickenSign | Init | 0x00e68dc0 | True | 0 |
| V_ChickenKicking | ChickenSign | OnPersist | 0x00cdebc0 | True | 0 |
| V_ChickenKicking | ChickenSign | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_ChickenKicking | KickedChicken | Main | 0x00e64210 | True | 16 |
| V_ChickenKicking | KickedChicken | Init | 0x00cdebb0 | True | 0 |
| V_ChickenKicking | KickedChicken | OnPersist | 0x00cdebc0 | True | 0 |
| V_ChickenKicking | KickedChicken | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_ChickenKicking | Spectator | Main | 0x00e63890 | True | 12 |
| V_ChickenKicking | Spectator | Init | 0x00e63730 | True | 0 |
| V_ChickenKicking | Spectator | OnPersist | 0x00cdebc0 | True | 0 |
| V_ChickenKicking | Spectator | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 5, "functions": 22, "missing": 0, "functionSyntaxPassed": 22, "fileSyntaxPassed": 6, "fileSyntaxChecked": 6, "todo": 69}`
