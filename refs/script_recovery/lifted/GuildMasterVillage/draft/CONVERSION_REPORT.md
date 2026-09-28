# Full guild master village native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_GuildMaster | V_GuildMaster | Main | 0x00e90830 | True | 0 |
| V_GuildMaster | V_GuildMaster | Init | 0x00e90780 | True | 0 |
| V_GuildMaster | V_GuildMaster | helper_E91F20 | 0x00e91f20 | True | 47 |
| V_GuildMaster | GuildMasterGameFlow | Main | 0x00e90be0 | True | 8 |
| V_GuildMaster | GuildMasterGameFlow | Init | 0x00e909a0 | True | 0 |
| V_GuildMaster | GuildMasterGameFlow | OnPersist | 0x00cdebc0 | True | 0 |
| V_GuildMaster | GuildMasterGameFlow | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 2, "functions": 7, "missing": 1, "functionSyntaxPassed": 7, "fileSyntaxPassed": 3, "fileSyntaxChecked": 3, "todo": 55}`
