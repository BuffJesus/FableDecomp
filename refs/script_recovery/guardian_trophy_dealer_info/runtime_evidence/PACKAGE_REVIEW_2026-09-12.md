# Guardian disabled package and root validation

Completed the offline package gate for `QS_GuardianTrophyDealerInfo`. Six Lua
files are staged under `refs/script_recovery/reconstructed/GuardianTrophyDealerInfo/FSE`.
Activation and unverified-disposable flags are false; no legacy quest is added.
No installed file or converter-owned file changed in this batch.

## Confirmed adapter defect and correction

The reviewed root's abstract `bind("GTDI_Maze", 0)` reached the adapter's
variadic `Quest:AddEntityBinding` unchanged. The real runtime method requires
`entityScriptName, scriptFileName, optionalFlags`. Sol converted the numeric
second argument into string `"0"`, queuing the wrong script file.

Reproduction uses the actual extracted C++ method, its real sol registration,
and packaged root/adapter. The finalization double inspects the actual pending
binding fields. It failed with:

```
lua: error: Guardian binding has wrong name/path/flags: GTDI_Maze / 0
```

Preserved reproduction: `work/runtime-guardian-binding-repro-20260912/`.
The adapter now maps the abstract entity name to
`GuardianTrophyDealerInfo/Entities/GTDI_Maze`, preserving flags 0. The same check
passes. The earlier root Lua facade accepted the wrong signature; that weaker
check alone was insufficient. No C++ runtime behavior needed changing.

## Native and actual-code comparisons

| Coverage | Result |
| --- | --- |
| Native Init E27870 / Main E27920 | Two initial flag states; seven native events each |
| Packaged root against native events | Both pass; the two consecutive PostAdd calls map to one Finalize operation |
| WaitForPieceOver E27AC0 | Ten native cases match packaged task through actual scoped Thing APIs |
| OnPersist E27E60 | Eight native cases match actual PersistTransferBool and packaged OnPersist |
| GTDI_Maze Init/Main | All 28 cases / 1,377 events still match through packaged entity entrypoints |
| Existing Guardian reference checks | Five test methods pass, preserving 87 native branch/watcher/entity comparisons |
| Clean compiled suite | All nine binaries pass; 32 combined Maze/Guardian persistence cases |
| Package, registry, source and asset audit | All 22 checks pass |

Watcher cases cover absent Maze, alive-to-dead transitions, waiting for the
flag, and cancellation before lookup, during waiting, or before deactivation.
Actual RetailResources wrappers are created and destroyed; counted Thing
ownership returns to baseline after every case, including the absent lookup.
Persistence covers write true/false, read true/false, and absent storage using
native default false. Storage and ordinary engine services remain doubles.

Native root replay checks the allocated binding's name, allocator E27BB0,
parent, enabled byte 1 and flags 0. It also checks the spawned callback E27AC0,
parent and empty region before the objective call. It executes successful
allocations, not an out-of-memory equivalence test. Root/engine finalization
and scheduling are doubled; live fibers do not run in these checks.

The first integration attempt exited with an access violation after printing
all 28 successful cases in a terminal. The harness held static sol references
past VM teardown. Explicitly releasing its tables/callback before destroying
the VM removes the failure in the clean run. This was a test-harness lifetime
fix, not a diagnosed game crash. The failed run remains preserved at
`work/runtime-guardian-package-20260912/`.

## Identity and deployment boundary

Native registry evidence and installed FinalAlbion.qst agree on
`QS_GuardianTrophyDealerInfo`; qst contains one inactive AddQuest registration.
The seed registry supplies entity ID 69 and its Lua file path. Seed quest ID
1005 is not used as a native identity. No conflicting quest name, entity name,
or entity ID was found in the installed or other reconstructed profiles listed
in `package-validation-20260912.json`. Recheck when merging a future profile.

Installed macro, placement, card and text inputs still match reviewed hashes.
Both installed DLLs remain v23:
`c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31`.
The game process was absent when checked during this batch; nothing was launched.

This port requires the newer source runtime's resource/Thing/condition APIs.
The prior Release build remains the candidate; its tracked runtime sources
were unchanged by this batch. It was not reinstalled or rebuilt for Lua/test
changes. Candidate DLL hash remains
`682bc0b4906ceb1f343a6c1d38d02652ca73632d1f95fd79e9ce52b6b7e14bf2`.

## Reproduction and remaining work

```
python tools/script_recovery/run_meet_sister_runtime_checks.py --forge-root D:/Code/ForgeFSE-retail-shadow --output work/NEW_GUARDIAN_CHECKS
python tools/script_recovery/validate_guardian_package.py --runtime-report work/NEW_GUARDIAN_CHECKS/result.json --output work/NEW_GUARDIAN_VALIDATION.json
```

Final compiled report: `work/runtime-guardian-package-final-20260912/result.json`.
Native additions: `native-root-cases-20260912.json` and
`native-persistence-cases-20260912.json`, generated by
`work/runtime-20260912-video-comparison/prove_guardian_root.py` and
`prove_guardian_persistence.py`. Both execute the pinned retail EXE's bytes.
Generators reject overwriting existing evidence. The package validator checks
compiled input/output hashes, native root comparisons, registration, file
copies and installed inputs.

Still unverified live: scheduler/fiber integration, region streaming, save-file
round trips, card display/activation, normal and Escape macro playback, camera
and speech. Father Escape and Bully skip acceptance on installed v23 stand.
The separate quiet/distant dialogue report and missing retail fades are not
claimed resolved by this package work.
