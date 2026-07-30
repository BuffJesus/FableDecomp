# Runnable reconstruction

This dashboard tracks the shortest honest path from independently verified
functions to a reconstructed executable. It does not count a diagnostic
bootstrap as a running game.

## Boot-path progress

| Measure | Proven | Scope |
|---|---:|---|
| GFMain direct-call sites | 257/257 (100.00%) | All ten call clusters; repeated call sites count separately |
| Callable authored GFMain phases | 10/10 (100.00%) | Integration checkpoints, not retail GFMain byte parity |
| Current Phase 10 direct calls | 21/21 (100.00%) | GFInitialise, launch, error handling, and cleanup |

These are dependency counters, not an estimate of engineering time or total
game completion. Whole-executable verified parity remains the stricter public
percentage in the root README.

## Current executable milestone

**The Stage 3 checkpoint now runs all ten GFMain phases:** VC7.1 links and runs a
Win32 GUI executable through the recovered `WinMain @ 0x00403480`
and authored integration units for retail `0x00402510-0x00403480`.
Its 141-byte body is an exact relocation-normalized retail match, and
the fixture proves both first-instance and duplicate-instance paths,
the 200 KiB MicroThread stack handoff, and the fastcall GFMain ABI.
Phase 1 constructs the recovered system defaults, establishes the
executable/project path with promoted retail functions, crosses a
parity-gated console-variable registrar boundary, then continues through the basic-install
and failure-policy sequence, then constructs and applies the optional
language-settings path, the five basic retail-bank aliases, and the decoded
retail-bank/INI path sequence, definition-table setup, checkpoint/save
permission precedence, and the display-error branch before stopping at
the recovered CSystemManagerInit option block, EULA/configuration policy,
engine-root/IME initialization, GFInitialise/launch policy, system-error
handling, registry persistence, and final cleanup through GFMain return.

**A visible authored checkpoint is now runnable:**
`FableTLC-Reconstruction-VisualCheckpoint.exe` follows that same matched
`WinMain` and reconstructed Phase 1-10 path, invokes the retail-matched full
`GFInitialise` coordinator and its retail-matched progress-display leaf through
an explicit engine boundary, then opens a responsive 1280x720 Win32 window.
When a retail `frontend.big` is available, the build decodes
`FRONTEND_BACKDROP_01` through the recovered Lionhead-LZO/DXT1 tooling,
crops it to its authored 640x480 frame, and embeds it in the executable.
The authored project image remains a dependency-safe fallback. This is the
first genuine game-derived image in the reconstructed process. An authored
D3D9 bridge now uploads it as a managed texture. Two retail-shaped triangle
records pass through the recovered Render2D batch planner, whose triangle-list
flush is executed on a real Win32 D3D9 device; GDI is retained only as a
failure fallback. Texture binding now executes the exact recovered 79-byte
`CRenderManagerCore::AttachTextureToStage` body, including its cache and
active-stage bookkeeping. The exact 167-byte
`CRenderStateManager::RealiseRenderState` body also drains ten queued render,
texture-stage, and sampler states for the live draw. Other dependency dispatch
now includes the recovered 11-byte `CRenderManagerCore::SetAWindow` ownership
hop and direct relocation-matched 112-byte `CDisplayManager::SetViewport`
float conversion into a compact integer D3D9 endpoint. The exact 79-byte
`Render2DDrawList::CopyBlock` full-clear path then resets the live 0x20-byte
vertex queue. Exact 98-byte `CTexture::operator=` and 34-byte
`CTexture::Uninitialise` bodies now execute the temporary/current/candidate
wrapper lifetime events. The relocation-matched 74-byte preallocated initializer
and behavior-gated `CalcByteLength` null path create the temporary wrapper, while
exact 121-byte `RestoreCaptureBlock` closes the compact capture sentinel.
Remaining dispatch is still authored rather than the
complete
Lionhead coordinator, runtime archive loader, or game loop.

The first parent-coordinator seam is now canonical too:
`FableBuildRender2DBatchPlan` models the recovered 0x3C-byte draw-record
layout and exact normal/text batching decisions from
`CRenderManager2D::Render2DDrawList @ 0x009DA9F0`. Its VC7.1 fixture covers
empty and stable queues, state/topology splits, text interruption and shader
reapplication, output overflow, and retail's exact 2001+1 primitive edge.
This is a behavior-proven integration seam, not a byte-parity claim for the
3,344-byte parent coordinator.

A second behavior seam now gates its surrounding renderer lifecycle: the
empty-list epilogue, 13 tracked state requests, conditional shader changes,
dirty uploads, attach/realise/DrawPrimitiveUP flush order, queue cleanup,
capture/layout restoration, and texture ownership teardown. The gate also
locks the recovered combined-projection dirty bit to `0x80`.
The visible D3D9 checkpoint now executes both seams: planner output is
translated into lifecycle flushes, and successful presentation requires the
adapter to observe and complete a real `DrawPrimitiveUP` event.

The full GFInitialise coordinator is promoted and connected:
`GFInitialise @ 0x004022B0` is a 311-byte relocation-normalized match with
root discovery, texture-depth fallback, display clamping, player registration,
progress setup, and both return paths covered. Its 128-byte
`GFInitialise_SetupProgressDisplay @ 0x00413120` leaf independently matches
and covers allocation, null-allocation, reference release, and rejected
initialization. Authored boundary objects still stand in for the unrecovered
engine singleton graph and renderer.

GFMain Phase 1 now has all nine direct calls proven. Its filesystem pair,
`CAFile::GetProjectPath @ 0x00997510` (146 bytes) and
`CAFile::SetCurrentPath @ 0x009974F0` (30 bytes) are
relocation-normalized matches. Focused fixtures prove executable-directory
discovery and the recovered update ordering: the OS working directory
changes before the engine updates its cached current-path string.
The folded `CWideString` and `CCharString` lifetime pairs are proven
too and grouped in a shared string-domain header. The PDB lineage
resolves the one-byte retail no-op as `NProfileTimer::StartProfile`.
The nested default `CCharString` constructor is retail-matched.
`CSystemManagerInit` now matches all 265 retail bytes and passes focused
layout/defaults/construction-order coverage. The final 4,158-byte
`InitialiseConsoleVariables` registrar matches with 403 relocations; its
fixture proves null-allocation, successful-allocation, and existing-singleton
entry paths without borrowing retail code.
Phase 2 now has all seven direct calls proven: the repeated string/profile
leaves, `NProfileTimer::EndProfile`, the async failure-policy encoder,
the TLC startup-latch clear, and the 69-byte counted `CFileInstaller`
singleton retrieval path.
The authored Phase 2 integration fixture proves setting propagation,
optional installer setup, startup-latch handling, async failure policy,
and balanced counted ownership on both enabled and skipped paths.
Phase 3 has all 34 direct calls proven. The first correction replaces
a false donor `GetActionName` label with a TLC-specific no-argument
default-language factory. Its readable 19-byte body constructs `"English"`
in the fastcall hidden return slot and now has a real lifetime fixture.
The next corrected call is `CCharString::ToWideString`: its 45-byte body
and the 13-byte conversion factory plus 66-byte counted-storage copy
constructor all relocation-match and pass focused ownership fixtures.
The signed-char persistence specialization now closes three call sites;
its exact dispatcher and five mode bodies cover default, binary, and text
transfer behavior. The exact 765-byte CIME settings loader exercises eight
byte settings and twelve colour transfers. A false GUI tree-sort label is
corrected to the CStringParser comment-list destructor, whose exact body
proves node and sentinel ownership. Shared string/profile targets, two
alignment setters, and two folded CBase vtable restores remain proven.
The authored Phase 3 integration now executes the exact language/string/
profile/alignment leaves, locks the recovered settings pathname, and proves
present/open, present/open-failed, and absent optional-file behavior.
Disk/parser ownership and the IME settings path now cross exact, focused
retail leaves while the authored phase keeps broader engine state isolated.
Phase 4 has all 29 direct calls proven. Its corrected manager identities
replace false COMDAT donor labels, and its exact navigator setter and alias
wrapper drive five decoded PC-to-generic bank mappings. The wrapper's nested
`CCharString::operator=` is also a relocation-normalized match.
Phase 5 has all 65 direct calls proven. Exact OpenRetailBank and OpenIniFile
bodies cover retail BIGB loading, development INI parsing, directory/header
selection, bank-map population, and disk/retail/threaded ownership beneath
every GFMain branch.
Phase 6 is callable with all 61 direct calls proven. Complete Stage 3 and
visual builds execute the exact definition-table pathname setter and loader
through file open, 16 KiB stream construction, read, flag publication, and
balanced cleanup. They also execute the exact My Documents permission probe
through its successful memory-only create/open/delete route. Exact My
Documents and save-directory helpers normalize and assemble the user save
root with balanced wide-string lifetimes, while
the exact physics-faces counted reset gates shared/last-owner/allocation-
failure behavior. The game-text bank constructor, counted group acquisition,
and five-path write-permission probe close checkpoint/save permission
precedence and the font/text/group/display error path.
Phase 7 is callable with all 15 direct calls proven. Exact cache-directory
and streaming-font factories join the already-proven string/profile leaves;
the fixture maps the recovered 0xC8-byte CSystemManagerInit option writes,
alternate display defaults, region modes, and the static-map Phase 8 bypass.
GetWindowTitle now independently proves localized lookup, fallback, and ownership.
Phase 8 is callable with all 5 direct calls proven. Exact EULA loading,
hardware detection, configuration cleanup, and the shared CRT exit thunk
cover success, both retail exit-on-failure branches, config skip, and
the static-map bypass.
Phase 9 is callable with all 11 direct calls proven. The exact 998-byte
CSystemManager initializer covers bootstrap skip, window failure, drive
failure propagation, and full subsystem ownership; exact singleton,
movie-layout, profile, string, CIME, and LUG-to-MET leaves complete the phase.
Phase 10 closes the authored GFMain coordinator with all 21 direct
calls. Four scenarios cover GFInitialise success with and without startup
text, GFInitialise failure, system-initialization error handling, launch
cleanup, final profile closure, registry persistence, and the recovered
cross-phase string cleanup cardinality. The exact counted profile-manager
singleton/dispatcher pair, GFUninitialise shutdown coordinator, complete
initialization-error dispatcher, wide comparison, and registry closure are
independently retail-matched;
registry behavior tests use injected memory-only imports.

**Stage 0 remains the smallest linker proof:** VC7.1 links a console
PE containing
`MemCmp_Unsigned16 @ 0x00403C60`, whose function body is byte-identical
to retail. The executable invokes that function and verifies its behavior.

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/build_bootstrap.ps1
```

Expected terminal markers include `FABLETLC_BOOTSTRAP_STAGE0 PASS`,
`FABLETLC_CRC_CALC_BEHAVIOR PASS`,
`FABLETLC_WINMAIN_BEHAVIOR PASS`,
`FABLETLC_PROGRESS_SETUP_BEHAVIOR PASS`,
`FABLETLC_SET_CURRENT_PATH_BEHAVIOR PASS`,
`FABLETLC_GET_PROJECT_PATH_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_WIDE_LITERAL_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_DESTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_CONVERSION_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_COPY_ASSIGNMENT_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_DESTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_PROFILE_START_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_DEFAULT_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_CHAR_STRING_COPY_ASSIGNMENT_BEHAVIOR PASS`,
`FABLETLC_RETAIL_BANK_NAVIGATOR_BEHAVIOR PASS`,
`FABLETLC_RETAIL_BANK_ALIAS_BEHAVIOR PASS`,
`FABLETLC_GET_SYSTEM_MANAGER_BEHAVIOR PASS`,
`FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS`,
`FABLETLC_INITIALISE_CONSOLE_VARIABLES PASS`,
`FABLETLC_PROFILE_END_BEHAVIOR PASS`,
`FABLETLC_ASYNC_FAILURE_HANDLING_BEHAVIOR PASS`,
`FABLETLC_STARTUP_LATCH_BEHAVIOR PASS`,
`FABLETLC_FILE_INSTALLER_GET_BEHAVIOR PASS`,
`FABLETLC_DEFAULT_LANGUAGE_NAME_BEHAVIOR PASS`,
`FABLETLC_FONT_BANK_NAME_BEHAVIOR PASS`,
`FABLETLC_MISC_DIRECTORY_A_BEHAVIOR PASS`,
`FABLETLC_MISC_DIRECTORY_B_BEHAVIOR PASS`,
`FABLETLC_GRAPHICS_DIRECTORY_BEHAVIOR PASS`,
`FABLETLC_LANGUAGE_DIRECTORY_A_BEHAVIOR PASS`,
`FABLETLC_SHADERS_DIRECTORY_BEHAVIOR PASS`,
`FABLETLC_LANGUAGE_DIRECTORY_B_BEHAVIOR PASS`,
`FABLETLC_CHECKPOINT_DIRECTORY_BEHAVIOR PASS`,
`FABLETLC_DEF_TABLE_PATH_BEHAVIOR PASS`,
`FABLETLC_BANK_HANDLE_BEHAVIOR PASS`,
`FABLETLC_VECTOR_MAP_LOOKUP_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_FROM_CHAR_FACTORY_BEHAVIOR PASS`,
`FABLETLC_WIDE_STRING_COPY_CONSTRUCTOR_BEHAVIOR PASS`,
`FABLETLC_CHAR_TO_WIDE_STRING_BEHAVIOR PASS`,
`FABLETLC_PRIMARY_LEFT_ALIGNMENT_BEHAVIOR PASS`,
`FABLETLC_SECONDARY_LEFT_ALIGNMENT_BEHAVIOR PASS`,
`FABLETLC_CBASE_RESTORE_A_BEHAVIOR PASS`,
`FABLETLC_CBASE_RESTORE_B_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE1_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE2_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE3_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE4_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE5_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE6_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE7_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE8_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE9_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_PHASE10_BEHAVIOR PASS`,
`FABLETLC_GFMAIN_COMPLETE PASS`,
`FABLETLC_GFINITIALISE_PROGRESS_PHASE_BEHAVIOR PASS`,
`FABLETLC_VISUAL_BOOT_BEHAVIOR PASS`,
`FABLETLC_RENDER2D_BATCH_PLAN PASS`,
`FABLETLC_RENDER2D_DRAW_LIST_ADAPTER PASS`,
`STAGE3_STARTUP PASS`, and
`GFINITIALISE_PROGRESS_INTEGRATION PASS`, and
`VISUAL_BOOT_CHECKPOINT PASS`.
Generated products stay under the ignored `rebuild/build/` tree.

After the Release build, the live GPU presentation gate is:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1
```

It requires a `D3D9 Presented via Render2D` window title, sends `WM_CLOSE`,
and requires a clean zero exit.

Stages 2 and 3 use explicit integration boundaries and are not claimed as a
retail-matching GFMain. The visual checkpoint can now present one build-time
extracted retail asset through D3D9 and recovered Render2D batching/lifecycle
seams, but it does **not** yet execute the complete Lionhead parent coordinator,
initialize the complete engine-service
graph, load archives at runtime, play video, or enter the game loop.

## Retail boot spine

| Order | Address | Role | Retail size | Current source grade | Source | Blocking fact |
|---:|---:|---|---:|---|---|---|
| 1 | `0x00401067` | CRT entry | 466 | `agent-pass` | [source](../lift/reports/wave3/code/00/40/0x00401067_global_entry.cpp) | Structural candidate only; contains a raw register-fed CRT helper call and has not crossed the VC7.1 behavior/parity gate. |
| 2 | `0x00403480` | WinMain wrapper | 141 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_WinMain_00403480.cpp) | VC7.1 source matches all 141 non-relocation retail bytes and passes first-instance/duplicate-instance behavior; Stage 3 now carries its GFMain handoff through all ten authored phases, whose 257 direct calls are proven. |
| 3 | `0x00402510` | GFMain | 3952 | `INTEGRATION_COMPLETE` | [source](../rebuild/integration/gfmain_complete.cpp) | The 3,952-byte coordinator is represented by one behavior-gated parent seam over ten callable clusters, and all 257 observed direct-call sites are proven. Success plus every phase-specific early return and WinMain input propagation are covered. Complete Stage 3 now executes exact Phase 6 definition-table path/load and write-permission ownership, Phase 7 window-title ownership, Phase 8 EULA/configuration/release, CSystemManager::Initialise, CIME::Initialise, optional LUG-to-MET generation, GFInitialise, and GFUninitialise coordinators; remaining gaps are subordinate ownership boundaries rather than missing control-flow edges. |
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
2. **WinMain wrapper (`0x00403480`):** Consolidate the authored GFMain parent and replace execution-boundary ownership shims.
3. **GFMain (`0x00402510`):** Replace the remaining execution-boundary ownership shims with recovered objects before the C++23 modernization pass.
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
| 1 | `0x00402510`-`0x004025A6` | runtime and project bootstrap | 9 | 9 | 9 | CSystemManagerInit; GetProjectPath; SetCurrentPath; InitialiseConsoleVariables |
| 2 | `0x004025A6`-`0x00402668` | basic-install and failure-policy bootstrap | 7 | 7 | 7 | CFileInstallerSingleton::Get; startup latch; SetEnableFailureHandling |
| 3 | `0x00402668`-`0x0040284E` | settings, persistence, and IME | 34 | 23 | 34 | FableGetDefaultLanguageName_00415530; PathExists; LoadFromFile; CPersistContext; LoadIMESettings |
| 4 | `0x0040284E`-`0x004029DC` | basic retail-bank alias map | 29 | 5 | 29 | CBasicRetailBankManager::SetNavigatorEnabled; CBasicRetailBankManager::AddBankAlias |
| 5 | `0x004029DC`-`0x00402CE6` | retail banks and INI files | 65 | 15 | 65 | OpenRetailBank; OpenIniFile; resource-directory factories |
| 6 | `0x00402CE6`-`0x00403082` | save paths, fonts, and display resources | 61 | 24 | 61 | LoadTable; MyDocuments_CheckWritePermissions; GetFontBankName; GFGetSystemManager |
| 7 | `0x00403082`-`0x0040329C` | command line and window configuration | 15 | 10 | 15 | GetWindowTitle; CCharString assignments |
| 8 | `0x0040329C`-`0x004032D5` | EULA and hardware configuration | 5 | 4 | 5 | DoEULAThings; GFConfigDetection; GFFreeConfigDetection |
| 9 | `0x004032D5`-`0x00403389` | engine primitive assembly | 11 | 10 | 11 | AddChildPrimitive; Initialise; CMainGameComponent::GenerateMetFilesFromLugFiles |
| 10 | `0x00403389`-`0x00403480` | GFInitialise, launch, error handling, and cleanup | 21 | 13 | 21 | GFInitialise; CTBaseSingleton<CUserProfileManager>::Get; CUserProfileManager::SetProfileName; CGame::Play; GFUninitialise; GFHandleSystemInitError; CSystemRegistry |

### Verified GFMain dependencies

| Phase | Call site | Target | Function | Grade | Source |
|---:|---:|---:|---|---|---|
| 1 | `0x00402535` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 1 | `0x0040253F` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 1 | `0x00402547` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 1 | `0x00402553` | `0x00403B10` | CSystemManagerInit | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CSystemManagerInit_Constructor_00403b10.cpp) |
| 1 | `0x0040255C` | `0x0099AED0` | CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp) |
| 1 | `0x00402583` | `0x00997510` | GetProjectPath | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CAFile_GetProjectPath_00997510.cpp) |
| 1 | `0x0040258A` | `0x009974F0` | SetCurrentPath | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CAFile_SetCurrentPath_009974f0.cpp) |
| 1 | `0x00402593` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 1 | `0x004025A1` | `0x00413C50` | InitialiseConsoleVariables | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_InitialiseConsoleVariables_00413c50.cpp) |
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
| 3 | `0x004026B5` | `0x0099BFF0` | operator+(wchar_t const *,CWideString const &) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddLiteralAndWideString_0099bff0.cpp) |
| 3 | `0x004026C0` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 3 | `0x004026C9` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 3 | `0x004026D2` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 3 | `0x004026DB` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x004026E4` | `0x00999230` | PathExists | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CAFile_PathExists_00999230.cpp) |
| 3 | `0x004026FE` | `0x0099AD80` | CDiskFileWin32 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CDiskFileWin32_Constructor_0099AD80.cpp) |
| 3 | `0x00402713` | `0x0099E4B0` | CCharString::CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_DefaultConstructor_0099e4b0.cpp) |
| 3 | `0x00402724` | `0x0099F2E0` | LoadFromFile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_LoadFromFile_0099f2e0.cpp) |
| 3 | `0x00402735` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 3 | `0x00402745` | `0x00404720` | CStringParser | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CStringParser_Constructor_00404720.cpp) |
| 3 | `0x0040274E` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x00402765` | `0x009BADD0` | CPersistContext | `MATCH` | [source](../rebuild/src/compiled/00/9b/CPersistContext_TextConstructor_009badd0.cpp) |
| 3 | `0x00402785` | `0x004045C0` | Transfer<signed_char> | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CPersistContext_TransferSignedChar_004045c0.cpp) |
| 3 | `0x004027A5` | `0x004045C0` | Transfer<signed_char> | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CPersistContext_TransferSignedChar_004045c0.cpp) |
| 3 | `0x004027C5` | `0x004045C0` | Transfer<signed_char> | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CPersistContext_TransferSignedChar_004045c0.cpp) |
| 3 | `0x004027D1` | `0x00405350` | NUISystem::CIME::LoadIMESettings | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CIME_LoadIMESettings_00405350.cpp) |
| 3 | `0x004027DA` | `0x004046B0` | std::list<CStringParserCommentBoundaries>::~list | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CStringParserCommentList_Destructor_004046b0.cpp) |
| 3 | `0x004027E3` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x004027EC` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x004027F5` | `0x0099A2E0` | FableRestoreCBaseVTable_0099A2E0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CBase_RestoreVTable_0099a2e0.cpp) |
| 3 | `0x004027FE` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x0040280F` | `0x0099A920` | CDiskFileWin32::Close | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CDiskFileWin32_Close_0099A920.cpp) |
| 3 | `0x00402818` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 3 | `0x00402821` | `0x0099A300` | FableRestoreCBaseVTable_0099A300 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CBase_RestoreVTable_0099a300.cpp) |
| 3 | `0x0040282C` | `0x009BC890` | NTextLayoutConfig::SetPrimaryLeftAlignment | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9b/TextLayout_SetPrimaryLeftAlignment_009bc890.cpp) |
| 3 | `0x00402837` | `0x009BC8A0` | NTextLayoutConfig::SetSecondaryLeftAlignment | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9b/TextLayout_SetSecondaryLeftAlignment_009bc8a0.cpp) |
| 3 | `0x0040283E` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 4 | `0x0040284E` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x00402859` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 4 | `0x00402862` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x00402875` | `0x009A76D0` | CBasicRetailBankManager::SetNavigatorEnabled | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBasicRetailBankManager_SetNavigatorEnabled_009a76d0.cpp) |
| 4 | `0x00402885` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x00402895` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x004028A9` | `0x009A8150` | CBasicRetailBankManager::AddBankAlias | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBasicRetailBankManager_AddBankAlias_009a8150.cpp) |
| 4 | `0x004028B2` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x004028BB` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x004028CB` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x004028DB` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x004028EF` | `0x009A8150` | CBasicRetailBankManager::AddBankAlias | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBasicRetailBankManager_AddBankAlias_009a8150.cpp) |
| 4 | `0x004028F8` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x00402901` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x00402911` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x00402921` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x00402935` | `0x009A8150` | CBasicRetailBankManager::AddBankAlias | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBasicRetailBankManager_AddBankAlias_009a8150.cpp) |
| 4 | `0x0040293E` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x00402947` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x00402957` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x00402967` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x0040297B` | `0x009A8150` | CBasicRetailBankManager::AddBankAlias | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBasicRetailBankManager_AddBankAlias_009a8150.cpp) |
| 4 | `0x00402984` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x0040298D` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x0040299D` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x004029AD` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 4 | `0x004029C1` | `0x009A8150` | CBasicRetailBankManager::AddBankAlias | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBasicRetailBankManager_AddBankAlias_009a8150.cpp) |
| 4 | `0x004029CA` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 4 | `0x004029D3` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 5 | `0x004029DC` | `0x0099AED0` | CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp) |
| 5 | `0x00402A04` | `0x0099B6B0` | CWideString::CWideString(const wchar_t*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp) |
| 5 | `0x00402A14` | `0x0041A100` | FableGetGraphicsDirectory_0041A100 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetGraphicsDirectory_0041a100.cpp) |
| 5 | `0x00402A1F` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402A2A` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402A33` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402A3C` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402A51` | `0x0041A100` | FableGetGraphicsDirectory_0041A100 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetGraphicsDirectory_0041a100.cpp) |
| 5 | `0x00402A5C` | `0x0099BE70` | operator+(CWideString const&,CWideString const&) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStrings_0099be70.cpp) |
| 5 | `0x00402A67` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402A72` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402A7B` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402A84` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402A8D` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402A9C` | `0x0041A330` | FableGetLanguageDirectoryB_0041A330 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetLanguageDirectoryB_0041a330.cpp) |
| 5 | `0x00402AA7` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402AB2` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402ABB` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402AC4` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402AD8` | `0x0041A0C0` | FableGetMiscDirectoryB_0041A0C0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetMiscDirectoryB_0041a0c0.cpp) |
| 5 | `0x00402AE3` | `0x0099BE70` | operator+(CWideString const&,CWideString const&) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStrings_0099be70.cpp) |
| 5 | `0x00402AEE` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402AF9` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402B02` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B0B` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B14` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B23` | `0x0041A330` | FableGetLanguageDirectoryB_0041A330 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetLanguageDirectoryB_0041a330.cpp) |
| 5 | `0x00402B2E` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402B39` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402B42` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B4B` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B5A` | `0x0041A0A0` | FableGetMiscDirectoryA_0041A0A0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetMiscDirectoryA_0041a0a0.cpp) |
| 5 | `0x00402B65` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402B70` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402B79` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B82` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402B96` | `0x0041A1E0` | FableGetShadersDirectory_0041A1E0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetShadersDirectory_0041a1e0.cpp) |
| 5 | `0x00402BA1` | `0x0099BE70` | operator+(CWideString const&,CWideString const&) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStrings_0099be70.cpp) |
| 5 | `0x00402BAC` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402BB7` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402BC0` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402BC9` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402BD2` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402BDB` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402BEB` | `0x0041A120` | FableGetLanguageDirectoryA_0041A120 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetLanguageDirectoryA_0041a120.cpp) |
| 5 | `0x00402BF6` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402C01` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402C0A` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402C13` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402C22` | `0x0041A330` | FableGetLanguageDirectoryB_0041A330 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetLanguageDirectoryB_0041a330.cpp) |
| 5 | `0x00402C2D` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 5 | `0x00402C38` | `0x009A8840` | OpenRetailBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp) |
| 5 | `0x00402C41` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402C4A` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402C58` | `0x0099B6B0` | CWideString::CWideString(const wchar_t*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp) |
| 5 | `0x00402C61` | `0x009A76C0` | NBankFileManager::SetHeaderDir | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/NBankFileManager_SetHeaderDir_009a76c0.cpp) |
| 5 | `0x00402C6A` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402C88` | `0x0099B6B0` | CWideString::CWideString(const wchar_t*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp) |
| 5 | `0x00402C99` | `0x0099B6B0` | CWideString::CWideString(const wchar_t*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp) |
| 5 | `0x00402CA8` | `0x009A8170` | OpenIniFile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CBankFileManager_OpenIniFile_009a8170.cpp) |
| 5 | `0x00402CB1` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402CBF` | `0x0099B6B0` | CWideString::CWideString(const wchar_t*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp) |
| 5 | `0x00402CC8` | `0x009A76C0` | NBankFileManager::SetHeaderDir | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/NBankFileManager_SetHeaderDir_009a76c0.cpp) |
| 5 | `0x00402CD1` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 5 | `0x00402CDF` | `0x0099B800` | CWideString::operator=(wchar_t const*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AssignLiteral_0099b800.cpp) |
| 6 | `0x00402CE6` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 6 | `0x00402CF6` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 6 | `0x00402D01` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 6 | `0x00402D0A` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 6 | `0x00402D19` | `0x0099B7D0` | CWideString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp) |
| 6 | `0x00402D2B` | `0x0099BF30` | operator+(CWideString const &,wchar_t const *) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_AddWideStringAndLiteral_0099bf30.cpp) |
| 6 | `0x00402D36` | `0x009D4A40` | CDefStringTableRecovered::SetTablePath | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/CDefStringTable_SetTablePath_009d4a40.cpp) |
| 6 | `0x00402D3F` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402D63` | `0x009D4C30` | CDefStringTable::LoadTable | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/CDefStringTable_LoadTable_009d4c30.cpp) |
| 6 | `0x00402D6A` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 6 | `0x00402D80` | `0x0041A4C0` | FableGetAutoSaveCheckpointDirectory_0041A4C0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetAutoSaveCheckpointDirectory_0041a4c0.cpp) |
| 6 | `0x00402D87` | `0x0099AF10` | CWideString::operator const wchar_t* | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_ConversionOperator_0099af10.cpp) |
| 6 | `0x00402D9B` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402DB6` | `0x0041A4C0` | FableGetAutoSaveCheckpointDirectory_0041A4C0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetAutoSaveCheckpointDirectory_0041a4c0.cpp) |
| 6 | `0x00402DBD` | `0x00402130` | FableCheckMyDocumentsWritePermissions_00402130 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_MyDocuments_CheckWritePermissions_00402130.cpp) |
| 6 | `0x00402DC8` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402DE0` | `0x0041A540` | NResourceDirectoryNames::GetSaveDir | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/NResourceDirectoryNames_GetSaveDir_0041a540.cpp) |
| 6 | `0x00402DE7` | `0x0099AF10` | CWideString::operator const wchar_t* | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_ConversionOperator_0099af10.cpp) |
| 6 | `0x00402DF5` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402E06` | `0x0041A540` | NResourceDirectoryNames::GetSaveDir | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/NResourceDirectoryNames_GetSaveDir_0041a540.cpp) |
| 6 | `0x00402E0D` | `0x0099AF10` | CWideString::operator const wchar_t* | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_ConversionOperator_0099af10.cpp) |
| 6 | `0x00402E17` | `0x0099B6B0` | CWideString::CWideString(const wchar_t*) | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_LiteralConstructor_0099b6b0.cpp) |
| 6 | `0x00402E20` | `0x00402130` | FableCheckMyDocumentsWritePermissions_00402130 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_MyDocuments_CheckWritePermissions_00402130.cpp) |
| 6 | `0x00402E5E` | `0x009C85A0` | NGameText::CDataBank::CDataBank | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/NGameText_CDataBank_Constructor_009c85a0.cpp) |
| 6 | `0x00402E2B` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402E34` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402E69` | `0x0041A430` | NResourceDirectoryNames::GetMyDocumentsDir | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/NResourceDirectoryNames_GetMyDocumentsDir_0041a430.cpp) |
| 6 | `0x00402E70` | `0x00402130` | FableCheckMyDocumentsWritePermissions_00402130 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_MyDocuments_CheckWritePermissions_00402130.cpp) |
| 6 | `0x00402E7B` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402E89` | `0x00403E40` | CCountedPointer<CIPhysicsFacesSegmentBase_const>::Reset | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CCountedPointer_PhysicsFacesSegment_Reset_00403e40.cpp) |
| 6 | `0x00402E9D` | `0x00415260` | GetFontBankName | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetFontBankName_00415260.cpp) |
| 6 | `0x00402EAC` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 6 | `0x00402EB5` | `0x0099AED0` | CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp) |
| 6 | `0x00402EBD` | `0x009CC410` | CVectorMap::LookupValue | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/FableVectorMap_LookupValue_009cc410.cpp) |
| 6 | `0x00402ED0` | `0x009C9070` | NGameText::CDataBank::GetPGroupEntry | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/NGameText_CDataBank_GetPGroupEntry_009c9070.cpp) |
| 6 | `0x00402EE2` | `0x009C7A40` | CBankFile::GetBankHandle | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/CBankFile_GetBankHandle_009c7a40.cpp) |
| 6 | `0x00402EEC` | `0x0099B7D0` | CWideString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp) |
| 6 | `0x00402EF5` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402F1C` | `0x0099AED0` | CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp) |
| 6 | `0x00402F2C` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 6 | `0x00402F4A` | `0x004014A0` | CCRC::Calc | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_CCRC_Calc_004014a0.cpp) |
| 6 | `0x00402F52` | `0x009CC410` | CVectorMap::LookupValue | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/FableVectorMap_LookupValue_009cc410.cpp) |
| 6 | `0x00402F65` | `0x009C9070` | NGameText::CDataBank::GetPGroupEntry | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/NGameText_CDataBank_GetPGroupEntry_009c9070.cpp) |
| 6 | `0x00402F77` | `0x009C7A40` | CBankFile::GetBankHandle | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9c/CBankFile_GetBankHandle_009c7a40.cpp) |
| 6 | `0x00402F81` | `0x0099B7D0` | CWideString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp) |
| 6 | `0x00402F8A` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402FAD` | `0x009A4EC0` | GFGetSystemManager | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/Global_GFGetSystemManager_009a4ec0.cpp) |
| 6 | `0x00402FBD` | `0x0099AF10` | CWideString::operator const wchar_t* | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_ConversionOperator_0099af10.cpp) |
| 6 | `0x00402FC7` | `0x0099AF10` | CWideString::operator const wchar_t* | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_ConversionOperator_0099af10.cpp) |
| 6 | `0x00402FD8` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 6 | `0x00402FE1` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00402FEA` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x0040300E` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00403017` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00403020` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x0040302C` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00403038` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00403044` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 6 | `0x00403050` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 6 | `0x0040305C` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 6 | `0x00403068` | `0x0099B510` | ~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 7 | `0x00403082` | `0x0099EBF0` | CCharString::CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 7 | `0x0040308D` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 7 | `0x00403096` | `0x0099EAE0` | CCharString::~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 7 | `0x0040318B` | `0x0041A1E0` | FableGetShadersDirectory_0041A1E0 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetShadersDirectory_0041a1e0.cpp) |
| 7 | `0x00403198` | `0x0099B7D0` | CWideString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp) |
| 7 | `0x004031A1` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 7 | `0x004031E5` | `0x004023F0` | GetWindowTitle | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_GetWindowTitle_004023f0.cpp) |
| 7 | `0x004031F2` | `0x0099B7D0` | CWideString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp) |
| 7 | `0x004031FB` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 7 | `0x00403211` | `0x0041A180` | FableGetCacheDirectory_0041A180 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetCacheDirectory_0041a180.cpp) |
| 7 | `0x0040321E` | `0x0099B7D0` | CWideString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_CopyAssignment_0099b7d0.cpp) |
| 7 | `0x00403227` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 7 | `0x00403230` | `0x00415440` | FableGetStreamingFontBankName_00415440 | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/Global_GetStreamingFontBankName_00415440.cpp) |
| 7 | `0x0040323D` | `0x0099EFB0` | CCharString::operator= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_CopyAssignment_0099efb0.cpp) |
| 7 | `0x00403246` | `0x0099EAE0` | CCharString::~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 8 | `0x0040329C` | `0x00401FE0` | DoEULAThings | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/EULA_DoEULAThings_00401fe0.cpp) |
| 8 | `0x004032A7` | `0x00401284` | exit | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_Exit_00401284.cpp) |
| 8 | `0x004032C0` | `0x004017B0` | GFConfigDetection | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_GFConfigDetection_004017b0.cpp) |
| 8 | `0x004032CB` | `0x00401284` | exit | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_Exit_00401284.cpp) |
| 8 | `0x004032D0` | `0x00401B20` | GFFreeConfigDetection | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_GFFreeConfigDetection_00401b20.cpp) |
| 9 | `0x00403302` | `0x009A0EF0` | CXMVPlayer::SetLeftAlignText | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CXMVPlayer_SetLeftAlignText_009a0ef0.cpp) |
| 9 | `0x00403325` | `0x009A4EC0` | GFGetSystemManager | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/Global_GFGetSystemManager_009a4ec0.cpp) |
| 9 | `0x0040332C` | `0x009A6610` | Initialise | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/CSystemManager_Initialise_009a6610.cpp) |
| 9 | `0x00403335` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 9 | `0x0040333A` | `0x009A4EC0` | GFGetSystemManager | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9a/Global_GFGetSystemManager_009a4ec0.cpp) |
| 9 | `0x00403346` | `0x00404A80` | AddChildPrimitive | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CEngineInternalPrimitiveBase_AddChildPrimitive_00404a80.cpp) |
| 9 | `0x0040334D` | `0x00405650` | Initialise | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CIME_Initialise_00405650.cpp) |
| 9 | `0x0040335D` | `0x0099EBF0` | CCharString::CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 9 | `0x00403368` | `0x009D8240` | NProfileTimer::StartProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_StartProfile_009d8240.cpp) |
| 9 | `0x00403371` | `0x0099EAE0` | CCharString::~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 9 | `0x0040337F` | `0x00418C3B` | CMainGameComponent::GenerateMetFilesFromLugFiles | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/CMainGameComponent_GenerateMetFilesFromLugFiles_00418c3b.cpp) |
| 10 | `0x00403389` | `0x004022B0` | GFInitialise | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/global_GFInitialise_004022b0.cpp) |
| 10 | `0x0040339B` | `0x0099B150` | CWideString::operator!= | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_NotEqualWide_0099b150.cpp) |
| 10 | `0x004033AA` | `0x0040D2A0` | CTBaseSingleton<CUserProfileManager>::Get | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CUserProfileManagerSingleton_Get_0040d2a0.cpp) |
| 10 | `0x004033B1` | `0x0040D400` | CUserProfileManager::SetProfileName | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CUserProfileManager_SetProfileName_0040d400.cpp) |
| 10 | `0x004033B8` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 10 | `0x004033C2` | `0x00412F90` | Play | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/41/CGame_Play_00412f90.cpp) |
| 10 | `0x004033C7` | `0x00401B80` | GFUninitialise | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_GFUninitialise_00401b80.cpp) |
| 10 | `0x004033CF` | `0x00401C00` | GFHandleSystemInitError | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/Global_GFHandleSystemInitError_00401c00.cpp) |
| 10 | `0x004033D6` | `0x009D8250` | NProfileTimer::EndProfile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/9d/NProfileTimer_EndProfile_009d8250.cpp) |
| 10 | `0x004033E5` | `0x0099CF10` | CSystemRegistry::CSystemRegistry | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CSystemRegistry_Constructor_0099cf10.cpp) |
| 10 | `0x004033F4` | `0x0099D100` | CSystemRegistry::WriteBOOL | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CSystemRegistry_WriteBOOL_0099d100.cpp) |
| 10 | `0x004033FD` | `0x0099CF70` | CSystemRegistry::~CSystemRegistry | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CSystemRegistry_Destructor_0099cf70.cpp) |
| 10 | `0x00403406` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 10 | `0x0040340F` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 10 | `0x00403418` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 10 | `0x00403424` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 10 | `0x00403430` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 10 | `0x0040343C` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |
| 10 | `0x00403448` | `0x0099EAE0` | CCharString::~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 10 | `0x00403454` | `0x0099EAE0` | CCharString::~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 10 | `0x00403460` | `0x0099B510` | CWideString::~CWideString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CWideString_Destructor_0099b510.cpp) |

Phase closure order:
1. **runtime and project bootstrap:** Callable in Stage 2 with all nine direct calls proven; the 265-byte CSystemManagerInit constructor and 4,158-byte console-variable registrar relocation-match retail, with constructor branches and registrar singleton allocation/reuse behavior gated.
2. **basic-install and failure-policy bootstrap:** Callable in Stage 3 with typed counted ownership; begin Phase 3 settings and persistence recovery.
3. **settings, persistence, and IME:** Callable in Stage 3 with all 34 direct calls proven; the exact signed-char persistence modes, full CIME settings loader, corrected CStringParser comment-list destructor, and existing string/profile/alignment ownership leaves are independently behavior-gated.
4. **basic retail-bank alias map:** Callable in Stage 3 with all 29 direct calls proven; five PC-to-generic aliases and the persisted navigator flag are behavior-gated.
5. **retail banks and INI files:** All 65 direct calls proven. Exact OpenRetailBank and OpenIniFile bodies now cover retail BIGB loading, development INI parsing, directory/header selection, bank-map population, and disk/retail/threaded ownership beneath every GFMain branch.
6. **save paths, fonts, and display resources:** Callable in Stage 3 with all 61 direct calls proven. Complete Stage 3 and visual runtimes now execute the exact 8-byte definition-table pathname setter and exact 164-byte loader through pathname ownership, file open, 16 KiB input-stream construction, read, loaded/read-only flag publication, and balanced stream/file/string cleanup. They also execute the exact 372-byte My Documents write-permission coordinator through its successful create/open/delete route with no host filesystem effects. Runtime assertions prove both full in-memory routes; focused exact fixtures retain missing-path/read-only table variants plus redirected-folder and create/open/delete permission failures. Exact save-directory ownership, the game-text data-bank constructor and group-entry acquisition, and the physics-faces counted reset independently gate the remaining checkpoint/save/font/text/group/display chain.
7. **command line and window configuration:** Callable in Stage 3 with all 15 direct calls proven. Complete Stage 3 and visual runtimes execute the exact 274-byte GetWindowTitle coordinator through bank allocation/open, CRC lookup, fallback-title assignment, and bank teardown; runtime assertions prove the full live route and normally owned CWideString handoff. The focused exact fixture retains localized group acquisition, bank-handle assignment, counted group release, and fallback coverage. Recovered 0xC8 CSystemManagerInit offsets, alternate display defaults, region selection, cache/shader/window/font strings, and static-map Phase 8 bypass remain behavior-gated.
8. **EULA and hardware configuration:** Callable in Stage 3 with all 5 direct calls proven. Complete Stage 3 and visual runtimes execute the exact 136-byte EULA coordinator, exact 875-byte hardware-detection coordinator, and exact 49-byte configuration-release coordinator through their normal-success route with runtime assertions from export acceptance through release. Focused fixtures retain load/export/video failures, both exit-on-failure branches, config skip, and Phase 7's static-map bypass; process exit remains injectable.
9. **engine primitive assembly:** Callable in Stage 3 with all 11 direct calls proven. One recovered 0xC8 CSystemManagerInit owner now spans Phase 1 construction, Phase 7 population, and Phase 9 consumption. Complete Stage 3 and visual runtimes execute the exact 998-byte CSystemManager coordinator through its recovered default-flags pre-initialization route, including process/scratch setup plus display, input, drive, text, mesh, and colour ownership across six allocations and the display/mesh component handoff; they also execute the exact 206-byte CIME body through a correctly sized 0x54 global owner and the exact 399-byte optional LUG-to-MET coordinator through its safe no-files route. Runtime assertions prove all three exact paths; focused fixtures retain system window/drive failures, the full eight-allocation route, IME failures, and one-file metadata generation/cleanup.
10. **GFInitialise, launch, error handling, and cleanup:** Callable in Stage 3 with all 21 direct calls proven; four scenarios gate GFInitialise success with and without startup text, GFInitialise failure, system-initialization error handling, launch cleanup, final profiles, registry persistence, and cross-phase string cleanup cardinality. The exact counted profile-manager pair, CGame ownership coordinator, GFUninitialise, full initialization-error dispatcher, wide comparison, and three-leaf registry closure are independently parity- and behavior-gated with memory-only registry imports.

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
