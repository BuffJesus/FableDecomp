# Runnable reconstruction

This dashboard tracks the shortest honest path from independently verified
functions to a reconstructed executable. It does not count a diagnostic
bootstrap as a running game.

## Boot-path progress

| Measure | Proven | Scope |
|---|---:|---|
| GFMain direct-call sites | 40/257 (15.56%) | All ten call clusters; repeated call sites count separately |
| Callable authored GFMain phases | 2/10 (20.00%) | Integration checkpoints, not retail GFMain byte parity |
| Current Phase 3 direct calls | 21/34 (61.76%) | Settings, persistence, and IME cluster |

These are dependency counters, not an estimate of engineering time or total
game completion. Whole-executable verified parity remains the stricter public
percentage in the root README.

## Current executable milestone

**The Stage 3 checkpoint now runs GFMain Phases 1 and 2:** VC7.1 links and runs a
Win32 GUI executable through the recovered `WinMain @ 0x00403480`
and authored integration units for retail `0x00402510-0x00402668`.
Its 141-byte body is an exact relocation-normalized retail match, and
the fixture proves both first-instance and duplicate-instance paths,
the 200 KiB MicroThread stack handoff, and the fastcall GFMain ABI.
Phase 1 constructs the recovered system defaults, establishes the
executable/project path with promoted retail functions, crosses a
counted console-variable boundary, then continues through the basic-install
and failure-policy sequence to the Phase 3 boundary.

**A visible authored checkpoint is now runnable:**
`FableTLC-Reconstruction-VisualCheckpoint.exe` follows that same matched
`WinMain` and reconstructed Phase 1/2 path, invokes the retail-matched full
`GFInitialise` coordinator and its retail-matched progress-display leaf through
an explicit engine boundary, then opens a responsive 1280x720 Win32 window.
When a retail `frontend.big` is available, the build decodes
`FRONTEND_BACKDROP_01` through the recovered Lionhead-LZO/DXT1 tooling,
crops it to its authored 640x480 frame, and embeds it in the executable.
The authored project image remains a dependency-safe fallback. This is the
first genuine game-derived image in the reconstructed process. An authored
D3D9 bridge now uploads it as a managed texture and presents an aspect-fitted
triangle strip through a real Win32 D3D9 device; GDI is retained only as a
failure fallback. The bridge is still scaffolding rather than the recovered
Lionhead draw-list submission, runtime archive loader, or game loop.

The full GFInitialise coordinator is promoted and connected:
`GFInitialise @ 0x004022B0` is a 311-byte relocation-normalized match with
root discovery, texture-depth fallback, display clamping, player registration,
progress setup, and both return paths covered. Its 128-byte
`GFInitialise_SetupProgressDisplay @ 0x00413120` leaf independently matches
and covers allocation, null-allocation, reference release, and rejected
initialization. Authored boundary objects still stand in for the unrecovered
engine singleton graph and renderer.

GFMain Phase 1 now has its filesystem pair promoted:
`CAFile::GetProjectPath @ 0x00997510` (146 bytes) and
`CAFile::SetCurrentPath @ 0x009974F0` (30 bytes) are
relocation-normalized matches. Focused fixtures prove executable-directory
discovery and the recovered update ordering: the OS working directory
changes before the engine updates its cached current-path string.
The folded `CWideString` and `CCharString` lifetime pairs are proven
too and grouped in a shared string-domain header. The PDB lineage
resolves the one-byte retail no-op as `NProfileTimer::StartProfile`,
bringing Phase 1 to seven of nine direct callees at relocation match.
The nested default `CCharString` constructor is retail-matched, while
`CSystemManagerInit` now passes a focused layout/defaults/construction-order
fixture. Its 265-byte object has one documented instruction-scheduling
residue: `lea ecx,[esi+0x5c]` is emitted earlier than retail.
Phase 2 now has all seven direct calls proven: the repeated string/profile
leaves, `NProfileTimer::EndProfile`, the async failure-policy encoder,
the TLC startup-latch clear, and the 69-byte counted `CFileInstaller`
singleton retrieval path.
The authored Phase 2 integration fixture proves setting propagation,
optional installer setup, startup-latch handling, async failure policy,
and balanced counted ownership on both enabled and skipped paths.
Phase 3 has 21 of 34 direct calls proven. The first correction replaces
a false donor `GetActionName` label with a TLC-specific no-argument
default-language factory. Its readable 19-byte body constructs `"English"`
in the fastcall hidden return slot and now has a real lifetime fixture.
The next corrected call is `CCharString::ToWideString`: its 45-byte body
and the 13-byte conversion factory plus 66-byte counted-storage copy
constructor all relocation-match and pass focused ownership fixtures.
The shared string/profile targets, two seven-byte text-alignment setters,
and two folded seven-byte CBase vtable restores remain proven as well.

**Stage 0 remains the smallest linker proof:** VC7.1 links a console
PE containing
`MemCmp_Unsigned16 @ 0x00403C60`, whose function body is byte-identical
to retail. The executable invokes that function and verifies its behavior.

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/build_bootstrap.ps1
```

Expected terminal markers include `FABLETLC_BOOTSTRAP_STAGE0 PASS`,
`FABLETLC_WINMAIN_BEHAVIOR PASS`,
`FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS`,
`FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS`,
`FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_PROFILE_START_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS`,
`FABLETLC_PROFILE_END_BEHAVIOR PASS`,
`FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR PASS`,
`FABLETLC_STARTUP_LATCH_BEHAVIOR PASS`,
`FABLETLC_FILE_INSTALLER_GET_BEHAVIOR PASS`,
`FABLETLC_DEFAULT_LANGUAGE_NAME_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_FROM_CHAR_FACTORY_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_COPY_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_CHAR_TO_WIDE_STRING_BEHAVIOR PASS`,
`FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR PASS`,
`FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR PASS`,
`FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS`,
`FABLETLC_CBASE_RESTORE_B_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE1_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE2_BEHAVIOR PASS`,
`FABLETLC_GFINITIALISE_PROGRESS_PHASE_BEHAVIOR PASS`,
`FABLETLC_VISUAL_BOOT_BEHAVIOR PASS`,
`STAGE3_STARTUP PASS`, and
`GFINITIALISE_PROGRESS_INTEGRATION PASS`, and
`VISUAL_BOOT_CHECKPOINT PASS`.
Generated products stay under the ignored `rebuild/build/` tree.

Stages 2 and 3 use explicit integration boundaries and are not claimed as a
retail-matching GFMain. The visual checkpoint can now present one build-time
extracted retail asset through D3D9, but it does **not** yet submit that image
through the recovered Lionhead renderer, initialize the complete engine-service
graph, load archives at runtime, play video, or enter the game loop.

## Retail boot spine

| Order | Address | Role | Retail size | Current source grade | Source | Blocking fact |
|---:|---:|---|---:|---|---|---|
| 1 | `0x00401067` | CRT entry | 466 | `agent-pass` | [source](../lift/reports/wave3/code/00/40/0x00401067_global_entry.cpp) | Structural candidate only; contains a raw register-fed CRT helper call and has not crossed the VC7.1 behavior/parity gate. |
| 2 | `0x00403480` | WinMain wrapper | 141 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_WinMain_00403480.cpp) | VC7.1 source matches all 141 non-relocation retail bytes and passes first-instance/duplicate-instance behavior; Stage 2 now carries its GFMain handoff through the authored Phase 1 integration unit. |
| 3 | `0x00402510` | GFMain | 3952 | `INTEGRATION_PHASE1` | [source](../rebuild/integration/gfmain_phase1.cpp) | The 3,952-byte coordinator is split into ten call clusters; Phase 1 is callable with seven direct callees at relocation match, CSystemManagerInit behavior-proven with one moved instruction, and the 4,158-byte console registrar explicitly stubbed. |
| 4 | `0x004022B0` | GFInitialise | 311 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/global_GFInitialise_004022b0.cpp) | The full 311-byte zero-parameter coordinator matches retail outside relocations, passes focused behavior, and now executes before the authored visual checkpoint through an explicit engine boundary. |
| 5 | `0x00413120` | GFInitialise_SetupProgressDisplay | 128 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GFInitialiseSetupProgressDisplay_00413120.cpp) | The 128-byte leaf and ownership behavior are proven and now execute as part of the verified full GFInitialise coordinator on the visual path. |
| 6 | `0x00499CE0` | CProgressDisplay constructor | 163 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/49/CProgressDisplay_Constructor_00499ce0.cpp) | The real 0x88-byte progress object is constructed on the visual path with its recovered string and state layout. |
| 7 | `0x009E9FD0` | SetProgressDisplay retained owner | 133 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9e/Global_SetProgressDisplay_009e9fd0.cpp) | The retail counted owner keeps the progress object alive across the visual window and releases it on shutdown. |
| 8 | `0x009EA060` | GetProgressDisplay counted query | 28 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9e/Global_GetProgressDisplay_009ea060.cpp) | The visual handoff now acquires the retained owner through the exact retail getter and balances the temporary reference after the window closes. |
| 9 | `0x0049B460` | CProgressDisplay active-state query | 4 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/49/CProgressDisplay_IsActive_0049b460.cpp) | The visual window reports the recovered retained display state through the exact four-byte retail query. |
| 10 | `0x00499A70` | CProgressDisplay text-mode state | 47 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/49/CProgressDisplay_SetToDisplayText_00499a70.cpp) | The visual startup now traverses the retail 47-byte state transition, clearing the primary string and progress value for false and routing true through CalculateNextTextTag. |
| 11 | `0x00497B30` | CProgressDisplay text-bank selector | 35 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/49/CProgressDisplay_GetPTextBank_00497b30.cpp) | The primary/fallback bank selection and null behavior are proven, but the visual bridge does not yet populate the retail text-bank owner globals. |

## Next dependency closure

1. **CRT entry (`0x00401067`):** Recover the masked CRT helper identity and promote the entry function.
2. **WinMain wrapper (`0x00403480`):** Extend the integration path from the Phase 2 boundary one dependency closure at a time.
3. **GFMain (`0x00402510`):** Close the constructor scheduling residue and replace the console boundary from recovered registration data while advancing Phase 2.
4. **GFInitialise (`0x004022B0`):** Replace its boundary-owned root, display, and registration test doubles with recovered startup objects.
5. **GFInitialise_SetupProgressDisplay (`0x00413120`):** Replace the instrumented progress-object ownership boundary with recovered engine ownership.
6. **CProgressDisplay constructor (`0x00499CE0`):** Recover the display and texture consumers that render the retained object.
7. **SetProgressDisplay retained owner (`0x009E9FD0`):** Replace the boundary display/resource graph with recovered engine ownership.
8. **GetProgressDisplay counted query (`0x009EA060`):** Connect the returned object to recovered texture and drawing consumers.
9. **CProgressDisplay active-state query (`0x0049B460`):** Integrate the corrected 418-byte StartProgress body and its texture and rendering consumers.
10. **CProgressDisplay text-mode state (`0x00499A70`):** Integrate the corrected 418-byte StartProgress body, then InitialiseTextures and the renderer-backed drawing path.
11. **CProgressDisplay text-bank selector (`0x00497B30`):** Connect runtime bank ownership while recovering the corrected 418-byte StartProgress body and its renderer-backed display path.

## GFMain dependency phases

The 3,952-byte coordinator is split by observed retail call clusters.
These are integration units, not invented retail functions.

| Phase | Address range | Role | Direct calls | Unique targets | Proven | Anchors |
|---:|---|---|---:|---:|---:|---|
| 1 | `0x00402510`-`0x004025A6` | runtime and project bootstrap | 9 | 9 | 7 | CSystemManagerInit; GetProjectPath; SetCurrentPath; InitialiseConsoleVariables |
| 2 | `0x004025A6`-`0x00402668` | basic-install and failure-policy bootstrap | 7 | 7 | 7 | CFileInstallerSingleton::Get; startup latch; SetEnableFailureHandling |
| 3 | `0x00402668`-`0x0040284E` | settings, persistence, and IME | 34 | 23 | 21 | FableGetDefaultLanguageName_00415530; PathExists; LoadFromFile; CPersistContext; LoadIMESettings |
| 4 | `0x0040284E`-`0x004029DC` | root child hierarchy | 29 | 5 | 0 | EnableNavigator; AddChild |
| 5 | `0x004029DC`-`0x00402CE6` | retail banks and INI files | 65 | 15 | 0 | OpenRetailBank; OpenIniFile; GetDVDDialogueDir |
| 6 | `0x00402CE6`-`0x00403082` | save paths, fonts, and display resources | 61 | 24 | 2 | LoadTable; MyDocuments_CheckWritePermissions; GetFontBankName; AddChildPrimitive |
| 7 | `0x00403082`-`0x0040329C` | command line and window configuration | 15 | 10 | 0 | GetWindowTitle; CCharString assignments |
| 8 | `0x0040329C`-`0x004032D5` | EULA and hardware configuration | 5 | 4 | 0 | DoEULAThings; GFConfigDetection; GFFreeConfigDetection |
| 9 | `0x004032D5`-`0x00403389` | engine primitive assembly | 11 | 10 | 1 | AddChildPrimitive; Initialise; GFMain_ProcessSaveFileMetadata |
| 10 | `0x00403389`-`0x00403480` | GFInitialise, launch, error handling, and cleanup | 21 | 13 | 2 | GFInitialise; GFHandleSystemInitError; CSystemRegistry |

### Verified GFMain dependencies

| Phase | Call site | Target | Function | Grade | Source |
|---:|---:|---:|---|---|---|
| 1 | `0x00402535` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 1 | `0x0040253F` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 1 | `0x00402547` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 1 | `0x00402553` | `0x00403B10` | CSystemManagerInit | `SCHEDULING_RESIDUE` | [source](../rebuild/src/compiled/00/40/CSystemManagerInit_Constructor_00403b10.cpp) |
| 1 | `0x0040255C` | `0x0099AED0` | CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp) |
| 1 | `0x00402583` | `0x00997510` | GetProjectPath | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CAFile_GetProjectPath_00997510.cpp) |
| 1 | `0x0040258A` | `0x009974F0` | SetCurrentPath | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CAFile_SetCurrentPath_009974f0.cpp) |
| 1 | `0x00402593` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 2 | `0x004025AC` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 2 | `0x004025BC` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 2 | `0x004025C7` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 2 | `0x004025D0` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 2 | `0x0040260B` | `0x00404440` | CFileInstallerSingleton::Get | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CFileInstallerSingleton_Get_00404440.cpp) |
| 2 | `0x0040261F` | `0x009D81E0` | FableClearStartupLatch_013964A8 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/Global_ClearStartupLatch_009d81e0.cpp) |
| 2 | `0x0040262A` | `0x009D5240` | CBankFileAsync::SetEnableFailureHandling | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/CBankFileAsync_SetEnableFailureHandling_009d5240.cpp) |
| 3 | `0x00402668` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 3 | `0x00402678` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 3 | `0x00402683` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 3 | `0x0040268C` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x0040269F` | `0x00415530` | FableGetDefaultLanguageName_00415530 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetDefaultLanguageName_00415530.cpp) |
| 3 | `0x004026A6` | `0x0099E480` | CCharString::ToWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_ToWideString_0099e480.cpp) |
| 3 | `0x004026C9` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 3 | `0x004026D2` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 3 | `0x004026DB` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x00402713` | `0x0099E4B0` | CCharString::CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_DefaultConstructor_0099e4b0.cpp) |
| 3 | `0x00402735` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 3 | `0x0040274E` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x004027E3` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x004027EC` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x004027F5` | `0x0099A2E0` | FableRestoreCBaseVTable_0099A2E0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CBase_RestoreVTable_0099a2e0.cpp) |
| 3 | `0x004027FE` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x00402818` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 3 | `0x00402821` | `0x0099A300` | FableRestoreCBaseVTable_0099A300 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CBase_RestoreVTable_0099a300.cpp) |
| 3 | `0x0040282C` | `0x009BC890` | NTextLayoutConfig::SetPrimaryLeftAlignment | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9b/TextLayout_SetPrimaryLeftAlignment_009bc890.cpp) |
| 3 | `0x00402837` | `0x009BC8A0` | NTextLayoutConfig::SetSecondaryLeftAlignment | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9b/TextLayout_SetSecondaryLeftAlignment_009bc8a0.cpp) |
| 3 | `0x0040283E` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 6 | `0x00402CE6` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 6 | `0x00402D6A` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 9 | `0x00403335` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 10 | `0x004033B8` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 10 | `0x004033D6` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |

Phase closure order:
1. **runtime and project bootstrap:** Callable in Stage 2; close CSystemManagerInit's one-instruction scheduling residue and replace the console-variable boundary from recovered registration data.
2. **basic-install and failure-policy bootstrap:** Callable in Stage 3 with typed counted ownership; begin Phase 3 settings and persistence recovery.
3. **settings, persistence, and IME:** Separate optional settings-file parsing from default-value initialization.
4. **root child hierarchy:** Recover the five named child definitions and the owner/container type.
5. **retail banks and INI files:** Name each bank path and turn the repeated open sequence into data-backed records.
6. **save paths, fonts, and display resources:** Split filesystem permission checks from font/display resource construction.
7. **command line and window configuration:** Recover the no-call command-line decision block and its persistent option fields.
8. **EULA and hardware configuration:** Isolate the exit-on-failure policy behind an injectable process-exit seam.
9. **engine primitive assembly:** Type the root engine primitive and carry its successful initialization result forward.
10. **GFInitialise, launch, error handling, and cleanup:** Close the GFInitialise return/error branches and persistent registry write.

## Milestone definitions

- **Stage 0 — linked code:** an owned x86 executable runs at least one
  behavior-gated, retail-matching function. **Implemented.**
- **Stage 1 — owned startup:** reconstructed WinMain reaches an
  instrumented GFMain boundary without borrowing retail code.
  **Implemented; exact CRT-entry parity remains a separate task.**
- **Stage 2 — engine bootstrap:** reconstructed GFMain/GFInitialise reaches
  the first visible progress-display state with controlled platform shims.
  **The Phase 1 checkpoint and an authored visual shell are implemented;
  recovered retail progress-display wiring remains ahead.**
- **Stage 3 — data bootstrap:** compiled definitions and core archives load
  far enough to create the main game component.
- **Stage 4 — game loop:** the reconstructed process pumps input, updates a
  minimal world, renders frames, and shuts down cleanly.
- **Stage 5 — playable slice:** a controlled region can be loaded and the
  hero can be moved and saved.

Modern x64 APIs may continue as small oracle-backed experiments, but menus,
installer work, enhancements, and broad port refactors are intentionally
behind Stage 4.

_Generated by `tools/write_runnable_dashboard.py` from the function, parity,
coverage, and boot-chain manifests._
