# CMainGameComponent recovered, 2026-09-25

Continues land/boot-debake after `930d708`. Six parallel agents authored and verified;
the lead landed sequentially. All genuine C++ at RELOCATION_MATCH with behaviour fixtures
that fail under mutation (every function, 2-7 mutations each).

| addr | function | notes |
|---|---|---|
| 004184bd | Init (slot 1) | 1285 B; progress/profile strings are named locals, not temporaries |
| 004189c2 | Run (slot 2) | per-frame driver; calls UpdateRegular / UpdateForcedTickSpeed, not the virtuals |
| 00418b7a | ~CMainGameComponent | `if (!Quit) Shutdown();` + member teardown |
| 004197b3 | deleting destructor | replaced a "CLandscapeBackgroundPatch" stand-in |
| 00418289 | Update (slot 5) | /GS; function-local static BRequireUnpause |
| 004162b5 | UpdateRegular | 0x9E9FB0 is NProgressDisplay::IsActive; CIEngine::IsPreloaded slot 59 |
| 00417747 | UpdateForcedTickSpeed | Sleep via real import |
| 00417001 | Render (slot 7) | inlines ConvertGTToWF |
| 004164e0 | ForceRender(long) | frames = fps * (1/15) |
| 004166e2 | GetGameTimeOfNextPresentCompletion | vsync timer at CDisplayManager +0x294 |
| 00416953 | PostInit (slot 8) | 738 B (manifest said 823; 0x416C35 is another function) |
| 00416c8a | InitGraphics | |
| 00416832 / 004168dc | InitialiseText / InitialiseFonts | InitialiseFonts is protected (PDB `IAE`) |
| 004175e5 | Shutdown | |
| 00416e78 | GetInputs (slot 6) | /GS |
| 00417418 / 004174f1 | InitDisplayEngine / InitParticleEngine | InitParticleEngine is 119 B, not 233 |
| 004163b7 / 004165e8 | SetExclusiveMode / CheckSync | CheckSync /GS; debug mismatch report compiled out |

## New shared models

- `fable_iengine.h`: CIEngine : CInitBaseClass with all 85 virtuals in retail order
  (RTTI vtables 0x012A0D24 / CEngine 0x012A0F3C; slots matched by `ret N` byte counts).
  MSVC reverses overload order in the vtable (SetChildPrimitive pair).
- `fable_world.h` (CWorld : CBaseClassNonCopyable, CIDrawWorld; primary vtable 0x012390F0,
  size 0x198), `fable_world_map.h`, `fable_display_engine.h`, `fable_player_manager.h`,
  `fable_memory_allocator.h`, `fable_quest_manager.h`, `fable_particle_engine.h`,
  `fable_camera.h`, `fable_game_event.h`, `fable_player_gui.h`, `fable_checksum.h`,
  `fable_graphic_appearance.h`, `fable_game_init_statics.h`, `fable_progress_display.h`,
  `fable_fpu.h`. Several must not share a TU with the generated `engine/<Class>.h` of the
  same class (noted in each header).
- CNetworkClient derives from CInitBaseClass (retail calls IsValid 0x99A330 on it).

## Purity exception: fable_fpu.h

Update and UpdateForcedTickSpeed inline Lionhead's GFFPUSetDoublePrecision /
GFFPUSetRoundingNearest (`fnstcw ... fldcw`). VC7.1 has no intrinsic for these
(`_control87` is a CRT call), so the original source used inline asm. The two helpers keep
that `__asm` in `fable_fpu.h`; both callers are otherwise compiler-emitted C++. Note the
purity gate (crawl/purity.py) reads only the landed .cpp, so header asm is invisible to
it; this is the only such header today, and the gate should learn to follow includes.

## Identity fixes

- 0x43368D / 0x4336B0 are CStartupDefinitionManager::Get/Set (singleton 0x13B8760); the
  frontend's InitialiseDefs and Shutdown (f248b60) now name that class. CGameDefinitionManager
  is Get 0x44C6B0 / Set 0x44C71F / singleton 0x13B879C.
- Manifest relabels: 0x435F70 CDisplayEngine::Render, 0x49D870 CWorld::GetFrame,
  0x416047 SignalFrameUpdate, 0x4167A7 GetFrameDifferenceFromCurrent, 0x4FB150
  CWorldMap::GetLoadedRegion, 0x41A060 NResourceDirectoryNames::GetLevelDir (landed source is
  still an old stand-in), 0x9E9FB0 NProgressDisplay::IsActive.
- Not yet reconciled: 0x13B86A0 is both `g_FableProgressTextBankSlot` (fable_boot.h) and
  `CMainGameComponent::PMainGameComponent`; 0x13B8648 is FableWin `GOnlyBuildStaticMaps`
  (repo: CompileFrontendDefinitions); 0x13B8616 is `GUseRetailBanks` (fable_gfmain.h also
  calls it NavigatorEnabled); 0x415260 is NLocalisation::GetTextBankName.
- Weaker names (empty bodies placed by neighbourhood): 0x98E1B0 ProcessDirtyDiscError,
  0x9AC9E0 ScrambleDefs, 0x4AE9C0 WaitForSync, 0x9F8C10 CProfileManager::Update.

## Gotchas

- Parity cannot catch a swapped callee or a wrong string literal (only relocation targets
  change); several agents' mutations proved only the fixtures catch them.
- Fixtures must call a virtual under test non-virtually (`c->Class::Method()`).
- From Git Bash, pass cl flags to cmp.py with `MSYS_NO_PATHCONV=1`.

## Verification

122/122 boot leaves; ABI gate 5/10/5; 0 regressions across 81 dependents (0x9e1440 already
failing); full bootstrap exit 0; link smoke 0 unresolved.

## Next

Play seam: the frontend and main-game components are now reconstructed; wiring GFMain to the
real CGame::Play needs their callee ring (integration lists in the agents' reports, captured
above in part) supplied by runtime boundaries.
