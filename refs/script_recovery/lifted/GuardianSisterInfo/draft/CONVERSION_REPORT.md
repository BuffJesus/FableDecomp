# Full guardian sister info native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| QS_GuardianSisterInfo | QS_GuardianSisterInfo | Main | 0x00e25ab0 | True | 0 |
| QS_GuardianSisterInfo | QS_GuardianSisterInfo | Init | 0x00e25a00 | True | 0 |
| QS_GuardianSisterInfo | QS_GuardianSisterInfo | OnPersist | 0x00e25f40 | True | 0 |
| QS_GuardianSisterInfo | MazeAtTavern | Main | 0x00e25f70 | True | 1 |
| QS_GuardianSisterInfo | MazeAtTavern | Init | 0x00e25d20 | True | 0 |
| QS_GuardianSisterInfo | MazeAtTavern | OnPersist | 0x00cdebc0 | True | 0 |
| QS_GuardianSisterInfo | MazeAtTavern | OnPredicateFail | 0x00e25d30 | True | 0 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | QS_GuardianSisterInfo2_SisterInBanditCamp | Main | 0x00e268c0 | True | 0 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | QS_GuardianSisterInfo2_SisterInBanditCamp | Init | 0x00e26810 | True | 0 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | QS_GuardianSisterInfo2_SisterInBanditCamp | OnPersist | 0x00e26e00 | True | 0 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | MazeAtTavern | Main | 0x00e26e30 | True | 3 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | MazeAtTavern | Init | 0x00e26bd0 | True | 0 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | MazeAtTavern | OnPersist | 0x00cdebc0 | True | 0 |
| QS_GuardianSisterInfo2_SisterInBanditCamp | MazeAtTavern | OnPredicateFail | 0x00e26ba0 | True | 0 |

Summary: `{"owners": 4, "functions": 14, "missing": 0, "functionSyntaxPassed": 14, "fileSyntaxPassed": 4, "fileSyntaxChecked": 4, "todo": 4}`
