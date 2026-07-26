# Runnable reconstruction

This dashboard tracks the shortest honest path from independently verified
functions to a reconstructed executable. It does not count a diagnostic
bootstrap as a running game.

## Current executable milestone

**Stage 1 now reaches the GFMain boundary:** VC7.1 links and runs a
Win32 GUI executable through the recovered `WinMain @ 0x00403480`.
Its 141-byte body is an exact relocation-normalized retail match, and
the fixture proves both first-instance and duplicate-instance paths,
the 200 KiB MicroThread stack handoff, and the fastcall GFMain ABI.

The first Stage-2 leaf is also promoted:
`GFInitialise_SetupProgressDisplay @ 0x00413120` is a 128-byte
relocation-normalized match with allocation, null-allocation, reference
release, and rejected-initialization behavior covered. It is not yet
reachable through the reconstructed GFMain/GFInitialise path.

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
`FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS`, and
`STAGE1_STARTUP PASS`.
Generated products stay under the ignored `rebuild/build/` tree.

Stage 1 still stops at an instrumented GFMain stub. It does **not** yet
initialize Lionhead engine services, open the retail window, load assets,
or enter the game loop.

## Retail boot spine

| Order | Address | Role | Retail size | Current source grade | Source | Blocking fact |
|---:|---:|---|---:|---|---|---|
| 1 | `0x00401067` | CRT entry | 466 | `agent-pass` | [source](../lift/reports/wave3/code/00/40/0x00401067_global_entry.cpp) | Structural candidate only; contains a raw register-fed CRT helper call and has not crossed the VC7.1 behavior/parity gate. |
| 2 | `0x00403480` | WinMain wrapper | 141 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_WinMain_00403480.cpp) | VC7.1 source matches all 141 non-relocation retail bytes and passes first-instance/duplicate-instance behavior, but GFMain is still an instrumented boundary. |
| 3 | `0x00402510` | GFMain | 3952 | `not authored` | — | The 3,952-byte coordinator is split into ten call clusters; Phase 1 has seven of nine direct callees at relocation match and CSystemManagerInit behavior-proven with one moved instruction. |
| 4 | `0x004022B0` | GFInitialise | 311 | `agent-pass` | [source](../lift/reports/wave3/code/00/40/0x004022B0_global_GFInitialise.cpp) | Structural candidate only; incomplete display/primitive types and several global dependencies prevent promotion. |
| 5 | `0x00413120` | GFInitialise_SetupProgressDisplay | 128 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GFInitialiseSetupProgressDisplay_00413120.cpp) | The 128-byte leaf and ownership behavior are proven, but GFMain/GFInitialise do not yet reach it in the reconstructed process. |

## Next dependency closure

1. **CRT entry (`0x00401067`):** Recover the masked CRT helper identity and promote the entry function.
2. **WinMain wrapper (`0x00403480`):** Replace the instrumented GFMain boundary one dependency closure at a time.
3. **GFMain (`0x00402510`):** Close that scheduling residue, then author the callable integration seam with InitialiseConsoleVariables isolated behind a boundary.
4. **GFInitialise (`0x004022B0`):** Recover shared layouts and promote the small callees before the coordinator.
5. **GFInitialise_SetupProgressDisplay (`0x00413120`):** Link its corrected CProgressDisplay/counting dependencies behind the GFInitialise boundary.

## GFMain dependency phases

The 3,952-byte coordinator is split by observed retail call clusters.
These are integration units, not invented retail functions.

| Phase | Address range | Role | Direct calls | Unique targets | Proven | Anchors |
|---:|---|---|---:|---:|---:|---|
| 1 | `0x00402510`-`0x004025A6` | runtime and project bootstrap | 9 | 9 | 7 | CSystemManagerInit; GetProjectPath; SetCurrentPath; InitialiseConsoleVariables |
| 2 | `0x004025A6`-`0x00402668` | failure-handling bootstrap | 7 | 7 | 0 | CSystemManager::Get; Draw; SetEnableFailureHandling |
| 3 | `0x00402668`-`0x0040284E` | settings, persistence, and IME | 34 | 23 | 0 | GetActionName; PathExists; LoadFromFile; CPersistContext; LoadIMESettings |
| 4 | `0x0040284E`-`0x004029DC` | root child hierarchy | 29 | 5 | 0 | EnableNavigator; AddChild |
| 5 | `0x004029DC`-`0x00402CE6` | retail banks and INI files | 65 | 15 | 0 | OpenRetailBank; OpenIniFile; GetDVDDialogueDir |
| 6 | `0x00402CE6`-`0x00403082` | save paths, fonts, and display resources | 61 | 24 | 0 | LoadTable; MyDocuments_CheckWritePermissions; GetFontBankName; AddChildPrimitive |
| 7 | `0x00403082`-`0x0040329C` | command line and window configuration | 15 | 10 | 0 | GetWindowTitle; CCharString assignments |
| 8 | `0x0040329C`-`0x004032D5` | EULA and hardware configuration | 5 | 4 | 0 | DoEULAThings; GFConfigDetection; GFFreeConfigDetection |
| 9 | `0x004032D5`-`0x00403389` | engine primitive assembly | 11 | 10 | 0 | AddChildPrimitive; Initialise; GFMain_ProcessSaveFileMetadata |
| 10 | `0x00403389`-`0x00403480` | GFInitialise, launch, error handling, and cleanup | 21 | 13 | 0 | GFInitialise; GFHandleSystemInitError; CSystemRegistry |

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

Phase closure order:
1. **runtime and project bootstrap:** Close CSystemManagerInit's one-instruction scheduling residue, then make this the first callable GFMain phase with InitialiseConsoleVariables isolated behind a boundary.
2. **failure-handling bootstrap:** Type the returned system-manager object and its temporary allocation/deletion path.
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
