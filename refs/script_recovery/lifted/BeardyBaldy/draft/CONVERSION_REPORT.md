# Full beardy baldy native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_BeardyBaldy | V_BeardyBaldy | Main | 0x00e4fa20 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | Init | 0x00e4f620 | True | 10 |
| V_BeardyBaldy | V_BeardyBaldy | OnPersist | 0x00e4f7f0 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForQuestFinished | 0x00e50100 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForQuestCardConditions | 0x00e50910 | True | 5 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForBarberLadyDeath | 0x00e504c0 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForBeardyBaldyDeath | 0x00e503d0 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForNewHairdo | 0x00e50150 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForNewBeard | 0x00e50210 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForNewTash | 0x00e502e0 | True | 1 |
| V_BeardyBaldy | V_BeardyBaldy | WatchForAttack | 0x00e53d90 | True | 11 |
| V_BeardyBaldy | V_BeardyBaldy | GoTalkToBeardyBaldy | 0x00e50650 | True | 1 |
| V_BeardyBaldy | V_BeardyBaldy | IsHeroWearingAnyTash | 0x00e538f0 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | IsHeroWearingAnyOddHairdo | 0x00e53ad0 | True | 0 |
| V_BeardyBaldy | V_BeardyBaldy | GoTalkToBarber | 0x00e50e40 | True | 1 |
| V_BeardyBaldy | V_BeardyBaldy | helper_E53C70 | 0x00e53c70 | True | 0 |
| V_BeardyBaldy | BB_BeardyBaldyMan | Main | 0x00e50fd0 | True | 35 |
| V_BeardyBaldy | BB_BeardyBaldyMan | Init | 0x00e50890 | True | 0 |
| V_BeardyBaldy | BB_BeardyBaldyMan | OnPersist | 0x00e508d0 | True | 0 |
| V_BeardyBaldy | BB_BeardyBaldyMan | OnPredicateFail | 0x00e508e0 | True | 0 |
| V_BeardyBaldy | BB_BeardyBaldyMan | helper_E53670 | 0x00e53670 | True | 0 |
| V_BeardyBaldy | BB_BeardyBaldyMan | helper_E53CB0 | 0x00e53cb0 | True | 4 |
| V_BeardyBaldy | BB_BeardyBaldyMan | helper_E53F60 | 0x00e53f60 | True | 5 |

Summary: `{"owners": 2, "functions": 23, "missing": 0, "functionSyntaxPassed": 23, "fileSyntaxPassed": 3, "fileSyntaxChecked": 3, "todo": 73}`
