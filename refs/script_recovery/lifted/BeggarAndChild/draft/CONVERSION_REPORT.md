# Full beggar and child native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_BeggarAndChild | V_BeggarAndChild | Main | 0x00e57e60 | True | 12 |
| V_BeggarAndChild | V_BeggarAndChild | Init | 0x00e57c00 | True | 0 |
| V_BeggarAndChild | V_BeggarAndChild | OnPersist | 0x00e57d20 | True | 0 |
| V_BeggarAndChild | LookoutPointBeggar | Main | 0x00e58d40 | True | 29 |
| V_BeggarAndChild | LookoutPointBeggar | Init | 0x00e58d10 | True | 0 |
| V_BeggarAndChild | LookoutPointBeggar | OnPersist | 0x00cdebc0 | True | 0 |
| V_BeggarAndChild | LookoutPointBeggar | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_BeggarAndChild | BeggarBully | Main | 0x00e5b3b0 | False | 17 |
| V_BeggarAndChild | BeggarBully | Init | 0x00e5b380 | True | 0 |
| V_BeggarAndChild | BeggarBully | OnPersist | 0x00cdebc0 | True | 0 |
| V_BeggarAndChild | BeggarBully | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 3, "functions": 11, "missing": 0, "functionSyntaxPassed": 10, "fileSyntaxPassed": 2, "fileSyntaxChecked": 3, "todo": 58}`
