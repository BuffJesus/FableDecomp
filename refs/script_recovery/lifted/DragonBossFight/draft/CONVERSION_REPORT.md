# Full dragon boss fight native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| Q_DragonBossFight | Q_DragonBossFight | Main | 0x00d255e0 | True | 0 |
| Q_DragonBossFight | Q_DragonBossFight | Init | 0x00d254b0 | True | 0 |
| Q_DragonBossFight | Q_DragonBossFight | DoMission | 0x00d26190 | True | 0 |
| Q_DragonBossFight | Q_DragonBossFight | RunEnemySpawning | 0x00d26ba0 | True | 0 |
| Q_DragonBossFight | Q_DragonBossFight | JackTaunts | 0x00d26a50 | True | 1 |
| Q_DragonBossFight | Q_DragonBossFight | SpawnMinions | 0x00d26cc0 | True | 0 |
| Q_DragonBossFight | Q_DragonBossFight | SpawnSummoners | 0x00d26e60 | True | 4 |
| Q_DragonBossFight | Q_DragonBossFight | helper_D25C20 | 0x00d25c20 | True | 0 |
| Q_DragonBossFight | Q_DragonBossFight | helper_D27050 | 0x00d27050 | True | 0 |
| Q_DragonBossFight | Dragon | Main | 0x00d25a00 | True | 3 |
| Q_DragonBossFight | Dragon | Init | 0x00d25880 | True | 2 |
| Q_DragonBossFight | Dragon | OnPersist | 0x00cdebc0 | True | 0 |
| Q_DragonBossFight | Dragon | OnPredicateFail | 0x00d259a0 | True | 0 |
| Q_DragonBossFight | Dragon | helper_D258C0 | 0x00d258c0 | True | 9 |
| Q_DragonBossFight | DBMinion | Main | 0x00d25d80 | True | 0 |
| Q_DragonBossFight | DBMinion | Init | 0x00d25d10 | True | 0 |
| Q_DragonBossFight | DBMinion | OnPersist | 0x00cdebc0 | True | 0 |
| Q_DragonBossFight | DBMinion | OnPredicateFail | 0x00d25d20 | True | 0 |
| Q_DragonBossFight | DBSummoner | Main | 0x00d25f50 | True | 0 |
| Q_DragonBossFight | DBSummoner | Init | 0x00d25ee0 | True | 0 |
| Q_DragonBossFight | DBSummoner | OnPersist | 0x00cdebc0 | True | 0 |
| Q_DragonBossFight | DBSummoner | OnPredicateFail | 0x00d25ef0 | True | 0 |

Summary: `{"owners": 4, "functions": 22, "missing": 1, "functionSyntaxPassed": 22, "fileSyntaxPassed": 5, "fileSyntaxChecked": 5, "todo": 19}`
