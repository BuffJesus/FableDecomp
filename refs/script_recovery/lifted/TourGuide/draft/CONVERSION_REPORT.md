# Full tour guide native conversion coverage

Compilation is not behavioral parity. Missing bodies and unresolved operations prevent completion.

| Script | Owner | Function | Address | Compiles | TODO |
|---|---|---|---|---|---:|
| V_TourGuide | V_TourGuide | Main | 0x00ee47b0 | True | 0 |
| V_TourGuide | V_TourGuide | Init | 0x00ee42a0 | True | 18 |
| V_TourGuide | V_TourGuide | OnPersist | 0x00ee5720 | True | 0 |
| V_TourGuide | V_TourGuide | WatchForGuideKilled | 0x00ee4a70 | True | 0 |
| V_TourGuide | V_TourGuide | WatchForClosingTime | 0x00ee4a00 | True | 0 |
| V_TourGuide | V_TourGuide | NativeThread_00ee6a40 | 0x00ee6a40 | True | 2 |
| V_TourGuide | TourGuideGuide | Main | 0x00ee57b0 | True | 16 |
| V_TourGuide | TourGuideGuide | Init | 0x00ee4c60 | True | 0 |
| V_TourGuide | TourGuideGuide | OnPersist | 0x00cdebc0 | True | 0 |
| V_TourGuide | TourGuideGuide | OnPredicateFail | 0x00ee4cc0 | True | 0 |
| V_TourGuide | TourGuideGuide | helper_EE6850 | 0x00ee6850 | True | 11 |
| V_TourGuide | TourGuideFollower | Main | 0x00ee4dd0 | True | 8 |
| V_TourGuide | TourGuideFollower | Init | 0x00ee4da0 | True | 0 |
| V_TourGuide | TourGuideFollower | OnPersist | 0x00cdebc0 | True | 0 |
| V_TourGuide | TourGuideFollower | OnPredicateFail | 0x00cdebd0 | True | 0 |

Summary: `{"owners": 3, "functions": 15, "missing": 0, "functionSyntaxPassed": 15, "fileSyntaxPassed": 3, "fileSyntaxChecked": 3, "todo": 55}`
