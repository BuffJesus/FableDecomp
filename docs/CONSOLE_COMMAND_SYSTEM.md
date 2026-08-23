# Retail console command system — complete registry

Evidence-derived from retail `Fable.exe` (Steam, 16,666,624 B, ImageBase 0x400000).
Method: locate the single registry-insert function, enumerate its callers, recover
each call site's name literal. Reproduce with the scripts noted at the bottom.

## Mechanism

| Piece | VA | Notes |
|---|---|---|
| `CConsole::Initialise` | `0x009ED190` | called from `CNewFrontendGameComponent::Init` `0x0042F75E`+0x28 — the console **is** constructed in the shipping build |
| registry insert (manifest: `AddInput`) | `0x009EC5E0` | really *AddCommand*; wraps the command object in a 12-byte node. **Single insert path** — 43 callers = the whole registry |
| `CConsole::RunTextCommand` | `0x009EC710` | thiscall; tokenises on `;` then calls `RunCommandLine` |
| `CConsole::RunCommandLine` | `0x009EB430` | name lookup + dispatch |
| `CConsole_ExecuteConfigFile` | `0x009EC890` | opens a file, strips `//` and `/* */`, runs each line |
| `RunScript` handler | `0x009ECC53` region | appends `.ini` to its argument |
| config exec at boot | `0x00414C00` | runs `default_userst.ini` (wide str `0x122E68C`) if present, then `userst.ini` (`0x122E674`) gated on flag byte `[0x01375444]` |
| live `CNewFrontendGameComponent*` | `0x013B871C` | stored by `Init`: `mov [0x13b871c], esi` |
| console-family singleton | `0x013CAA40` | loaded as `this` by `0x00414C90` |

## The complete registry (43)

### Boot / data-pipeline (registered by `InitialiseConsoleVariables` `0x00413C50`)
| Site | Name |
|---|---|
| `0x00413D2D` | `SetLevel` |
| `0x00413DD1` | `SetStartingHolySite` |
| `0x00413E79` | `EnableCodeSectionLoading` |
| `0x00413F10` | `UsePhysicalDVD` |
| `0x00413FA7` | `UseRetailSaveGameSystem` |
| `0x0041403E` | `RunFromDVD` |
| `0x004140D5` | `UseRetailBanks` |
| `0x0041416F` | `UseCompiledAnimationEvents` |
| `0x0041420B` | `UseCompiledGlobalThings` |
| `0x004142A8` | `UseCompiledDefs` |
| `0x00414345` | `UseCompiledWorldFiles` |
| `0x004143E2` | `UseCompiledSoundSymbols` |
| `0x0041447F` | `AllowDataGeneration` |

### Memory pools (same initialiser) — **tunable knobs**
| Site | Name |
|---|---|
| `0x0041451C` | `InstallerBufferSize` |
| `0x004145B9` | `NoInstallBuffers` |
| `0x00414656` | `LandscapePoolSize` |
| `0x004146F7` | `LandscapePhysicalMemoryRatio` |
| `0x00414797` | `LandscapeVirtualMemoryMinSize` |
| `0x00414837` | `MeshPoolSize` |
| `0x004148D7` | `MeshStatsPoolSize` |
| `0x00414978` | `MeshPhysicalMemoryRatio` |
| `0x00414A18` | `HiresTextureMemory` |
| `0x00414AB8` | `PhysicsMeshPoolSize` |
| `0x00414B58` | `AnimationPoolSize` |
| `0x00414BF8` | `ClothPoolSize` |

### Gameplay
| Site | Name |
|---|---|
| `0x00419E39` | `ActivateQuest` |
| `0x006BBD52` | `SetTimeOfDay` |
| `0x006BBE0A` | `SetDaySpeed` |

### Console built-ins (registered by `CConsole::Initialise`)
| Site | Name |
|---|---|
| `0x009ED24E` | `BindString` |
| `0x009ED309` | `RunBoundString` |
| `0x009ED3A5` | `BindKey` |
| `0x009ED427` | `RunScript` |
| `0x009ED493` | `ConsoleListContaining` |
| `0x009ED504` | `CommandList` |
| `0x009ED573` | `CommandListContaining` |
| `0x009ED5E5` | `VarList` |
| `0x009ED654` | `VarListContaining` |
| `0x009ED6DF` | `SetConsoleTextColour` |
| `0x009ED786` | `SetConsoleHistoryWindowColour` |
| `0x009ED82C` | `SetConsoleClientWindowColour` |
| `0x009ED8D9` | `SetConsoleHelpWindowColour` |
| `0x009ED986` | `SetConsoleScrollBarColour` |
| `0x009EDA14` | `ConsoleAlpha` |

## CRITICAL: most of shipped `userst.ini` is DEAD in retail

`userst.ini` and `user.ini` in the Steam install are dev-build leftovers. Testing every
identifier in them against the retail binary splits them cleanly:

**Present (18)** — all data-pipeline/gameplay, all in the registry above:
`ActivateQuest`, `AllowDataGeneration`, `EnableCodeSectionLoading`, `RunFromDVD`,
`RunScript`, `SetLevel`, `SetStartingHolySite`, `UseCompiledAnimationEvents`,
`UseCompiledDefs`, `UseCompiledGlobalThings`, `UseCompiledSoundSymbols`,
`UseCompiledWorldFiles`, `UsePhysicalDVD`, `UseRetailBanks`
(plus `BeginRetailFile`, `EndRetailFile`, `DevHeaderDirectory`, `RetailHeaderDirectory`
— these are **bank-config** keywords near the BIGB strings at `0x0129B030..0x0129B06C`,
NOT console commands; they are not among the 43.)

**Absent (23)** — every graphics/frontend/audio switch. These names do not exist as
strings anywhere in retail `Fable.exe`, so the lines are inert:
`AllowDebugProfile`, `MaxThingDrawDist`, `PresentImmediate`, `SetCompressedTextures`,
`SetDefinitionValidation`, `SetForcePrimaryDevice`, `SetFullscreen`,
`SetMaxAnimatedMeshDist`, `SetMaxAnisotropy`, `SetMaxStaticMeshDist`,
`SetMaxTextureSize`, `SetPlatform2DGain`, `SetPlayCD`, `SetPlayIntro`,
`SetResolution`, `SetRunScripts`, `SetSkipFrontend`, `SetSound`,
`SetTripleBuffering`, `SetUseOldWeaponTrails`, `SetZBufferDepth`, `ShowDevFrontEnd`,
`UseLevelWAD`.

**Consequence:** there is NO console route to skip the intro movie, skip the frontend,
force windowed mode, or auto-load a debug profile. Do not plan automation around them.
`ConsoleSetResolution` @ `0x00417568` and friends are **FableWin.pdb (dev/editor build)
names applied to retail addresses** — the retail build has no such command registered.

## What this IS good for

- `ActivateQuest("...")` from ini arms an FSE quest at boot (the shipped `user.ini`
  already does `ActivateQuest("Gameflow")`) — proven-working config-driven entry point.
- `RunScript("x.ini")` nests config scripts.
- `LandscapePoolSize` / `LandscapeVirtualMemoryMinSize` / `LandscapePhysicalMemoryRatio`
  are live tunables — the cheap way to test a landscape allocator-exhaustion theory
  with no rebuild and no terrain redeploy.
- `BindKey` / `BindString` / `RunBoundString` give deferred + key-triggered execution.
- `CommandList` / `VarList` let the game dump its own registry at runtime, which would
  independently confirm this static enumeration.

## Reproduce

Scratch scripts (PE VA->offset + capstone) used for this pass:
`fdis.py` (disassemble VA), `fstr.py` / `frdata.py` (string dump), `fxref.py`
(`callers <va>` / `names <va>`), `faddr.py` (find dword refs), `inichk.py`
(ini vocabulary vs binary). Regenerate the registry with
`fxref.py callers 0x9ec5e0` then recover each site's nearest preceding name literal.
