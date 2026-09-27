# Full sick child native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_SickChild | V_SickChild | Main | 0x00ec54e0 | True | 0 |
| V_SickChild | V_SickChild | Init | 0x00ec5420 | True | 0 |
| V_SickChild | V_SickChild | OnPersist | 0x00ecd7b0 | True | 0 |
| V_SickChild | V_SickChild | helper_ECE460 | 0x00ece460 | True | 24 |
| V_SickChild | SickChildsMother | Main | 0x00ecd9e0 | True | 35 |
| V_SickChild | SickChildsMother | Init | 0x00ec5c90 | True | 0 |
| V_SickChild | SickChildsMother | OnPersist | 0x00cdebc0 | True | 0 |
| V_SickChild | SickChildsMother | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SickChild | SickChild | Main | 0x00ec5de0 | True | 5 |
| V_SickChild | SickChild | Init | 0x00ec5da0 | True | 0 |
| V_SickChild | SickChild | OnPersist | 0x00cdebc0 | True | 0 |
| V_SickChild | SickChild | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SickChild | SickChildsSister | Main | 0x00ec68a0 | True | 34 |
| V_SickChild | SickChildsSister | Init | 0x00ec6860 | True | 0 |
| V_SickChild | SickChildsSister | OnPersist | 0x00ecd860 | True | 1 |
| V_SickChild | SickChildsSister | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SickChild | Witch | Main | 0x00ece7f0 | True | 84 |
| V_SickChild | Witch | Init | 0x00ec80c0 | True | 0 |
| V_SickChild | Witch | OnPersist | 0x00ecd890 | True | 2 |
| V_SickChild | Witch | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SickChild | ManInLove | Main | 0x00ec8300 | True | 34 |
| V_SickChild | ManInLove | Init | 0x00ec8210 | True | 0 |
| V_SickChild | ManInLove | OnPersist | 0x00ecd8e0 | True | 2 |
| V_SickChild | ManInLove | OnPredicateFail | 0x00ec8230 | True | 0 |
| V_SickChild | MansLover | Main | 0x00ec9d30 | True | 7 |
| V_SickChild | MansLover | Init | 0x00ec9cb0 | True | 0 |
| V_SickChild | MansLover | OnPersist | 0x00ecd930 | True | 1 |
| V_SickChild | MansLover | OnPredicateFail | 0x00ec9cd0 | True | 0 |
| V_SickChild | IngredientOwner | Main | 0x00ecb0e0 | True | 2 |
| V_SickChild | IngredientOwner | Init | 0x00ecb050 | True | 0 |
| V_SickChild | IngredientOwner | OnPersist | 0x00ecd960 | True | 2 |
| V_SickChild | IngredientOwner | OnPredicateFail | 0x00ecb080 | True | 0 |
| V_SickChild | TalkingTrader1 | Main | 0x00ecbdf0 | True | 13 |
| V_SickChild | TalkingTrader1 | Init | 0x00ecbd50 | True | 0 |
| V_SickChild | TalkingTrader1 | OnPersist | 0x00cdebc0 | True | 0 |
| V_SickChild | TalkingTrader1 | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SickChild | TalkingTrader2 | Main | 0x00eccad0 | True | 2 |
| V_SickChild | TalkingTrader2 | Init | 0x00ecca30 | True | 0 |
| V_SickChild | TalkingTrader2 | OnPersist | 0x00cdebc0 | True | 0 |
| V_SickChild | TalkingTrader2 | OnPredicateFail | 0x00cdebd0 | True | 0 |
| V_SickChild | WomanToAttract | Main | 0x00ecf7f0 | True | 112 |
| V_SickChild | WomanToAttract | Init | 0x00ecd380 | True | 0 |
| V_SickChild | WomanToAttract | OnPersist | 0x00ecd9b0 | True | 1 |
| V_SickChild | WomanToAttract | OnPredicateFail | 0x00ecd390 | True | 0 |
| V_SickChild | WomanToAttract | helper_ED0B10 | 0x00ed0b10 | True | 10 |
| V_SickChild | SickChildFishingSpot | Main | 0x00ecd530 | True | 0 |
| V_SickChild | SickChildFishingSpot | Init | 0x00ecd500 | True | 0 |
| V_SickChild | SickChildFishingSpot | OnPersist | 0x00cdebc0 | True | 0 |
| V_SickChild | SickChildFishingSpot | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 12, "functions": 49, "missing": 0, "functionSyntaxPassed": 49, "fileSyntaxPassed": 13, "fileSyntaxChecked": 13, "todo": 371}`
