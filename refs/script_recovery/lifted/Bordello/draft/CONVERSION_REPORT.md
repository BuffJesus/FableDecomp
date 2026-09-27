# Full bordello native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_Bordello | V_Bordello | Main | 0x00e39b40 | True | 0 |
| V_Bordello | V_Bordello | Init | 0x00e399d0 | True | 0 |
| V_Bordello | V_Bordello | OnPersist | 0x00e3b8f0 | True | 0 |
| V_Bordello | V_Bordello | WatchForBordelloStatus | 0x00e3a030 | True | 0 |
| V_Bordello | V_Bordello | WatchForDeedStatus | 0x00e3a150 | True | 1 |
| V_Bordello | V_Bordello | CreateCustomers | 0x00e3a350 | True | 2 |
| V_Bordello | V_Bordello | AdjustTavernPrices | 0x00e3a6b0 | True | 0 |
| V_Bordello | V_Bordello | null | 0x00e44980 | True | 0 |
| V_Bordello | V_Bordello | helper_E3E320 | 0x00e3e320 | True | 0 |
| V_Bordello | V_Bordello | helper_E3E720 | 0x00e3e720 | True | 3 |
| V_Bordello | V_Bordello | helper_E3E6B0 | 0x00e3e6b0 | True | 0 |
| V_Bordello | V_Bordello | helper_E44A40 | 0x00e44a40 | True | 0 |
| V_Bordello | V_Bordello | helper_E44CC0 | 0x00e44cc0 | True | 0 |
| V_Bordello | Madame | Main | 0x00e3bb70 | True | 17 |
| V_Bordello | Madame | Init | 0x00e3ab60 | True | 0 |
| V_Bordello | Madame | OnPersist | 0x00e3ba50 | True | 1 |
| V_Bordello | Madame | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_Bordello | BordelloLady | Main | 0x00e3eb10 | True | 55 |
| V_Bordello | BordelloLady | Init | 0x00e3ac70 | True | 1 |
| V_Bordello | BordelloLady | OnPersist | 0x00e3ba80 | True | 2 |
| V_Bordello | BordelloLady | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_Bordello | BordelloLady | helper_E403D0 | 0x00e403d0 | True | 0 |
| V_Bordello | BordelloGuard | Main | 0x00e40420 | True | 2 |
| V_Bordello | BordelloGuard | Init | 0x00e3af30 | True | 0 |
| V_Bordello | BordelloGuard | OnPersist | 0x00cdebc0 | True | 0 |
| V_Bordello | BordelloGuard | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_Bordello | Magicman | Main | 0x00e40e80 | True | 40 |
| V_Bordello | Magicman | Init | 0x00e3b060 | True | 0 |
| V_Bordello | Magicman | OnPersist | 0x00e3bad0 | True | 1 |
| V_Bordello | Magicman | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_Bordello | BordelloClient | Main | 0x00e44ea0 | True | 12 |
| V_Bordello | BordelloClient | Init | 0x00e3b1a0 | True | 0 |
| V_Bordello | BordelloClient | OnPersist | 0x00e3bb00 | True | 3 |
| V_Bordello | BordelloClient | OnPredicateFail | 0x00e3b230 | True | 1 |
| V_Bordello | M_HeroDownstairs | Main | 0x00e3b320 | True | 0 |
| V_Bordello | M_HeroDownstairs | Init | 0x00cdebb0 | True | 0 |
| V_Bordello | M_HeroDownstairs | OnPersist | 0x00cdebc0 | True | 0 |
| V_Bordello | M_HeroDownstairs | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_Bordello | BordelloEntrance | Main | 0x00e3b4a0 | True | 0 |
| V_Bordello | BordelloEntrance | Init | 0x00cdebb0 | True | 0 |
| V_Bordello | BordelloEntrance | OnPersist | 0x00cdebc0 | True | 0 |
| V_Bordello | BordelloEntrance | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 8, "functions": 42, "missing": 0, "functionSyntaxPassed": 42, "fileSyntaxPassed": 9, "fileSyntaxChecked": 9, "todo": 141}`
