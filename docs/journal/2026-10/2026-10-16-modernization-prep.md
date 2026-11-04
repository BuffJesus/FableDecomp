# Modernization prep — 2026-10-16 (Claude)

User: "Proceed with 64 bit/modernization until asked to stop." This session works the ROADMAP (h)
"can start now" items, which are prerequisites for MODERNIZATION.md steps 1–4. No fork tree, no
game launch, no parity-tree changes.

## Done

- **Frame timing** → `docs/modern/FRAME_TIMING.md`. The world already runs fixed-timestep at a
  constant **15 world frames/s** (int `0x1375550`, 632 reads / 461 functions, never written), with
  render interpolation gated by `IsTimeForServerUpdate` against predicted present time. Retail also
  has per-world-frame input event packages + `PeekWorldChecksum`/`CheckSync`: the replay tool the fork
  needs already exists in the engine. Uncapped frame rate = audit render-time readers (170
  `GFGetTime` callers), not a new timestep.
- **Persisted types** → `tools/modern/persisted_types.py`, `docs/modern/PERSISTED_TYPES.md/.tsv`.
  Saves/defs persist member by member through `CPersistContext::Transfer<T>` / `CPersistTraits<T>`.
  195 leaf types; width hazards: `long`/`unsigned long` (LP64 only), `CWideString` (wchar_t on
  Linux), pointer leaves `COpinionDeed *` / `COpinionPersonalityTraits *` (+ list/pair forms) whose
  traits must be read before the fork serializer. 388 persisting classes. File loaders outside
  CPersistContext (CMap, CLandscapeLayerMesh, CThingManager, CWorld::Load) listed as separate work.
- **D3D9 call-site census** → `tools/modern/d3d9_device_census.py`,
  `docs/modern/D3D9_DEVICE_CENSUS.md/.json`. Typed register model from the PDB layouts seeded at the
  `CRenderManagerCore*` singleton `[0x1436E18]`, plus a tier B by retail device offset. 252 device sites / 126 functions / 30 of 119 methods (142 tier A, 110 tier B). Lower bound.

## Findings worth keeping

- `CShaderRenderManager::D3DDevice` is **+0x1D4 in retail**, +0x1E4 in Ego_r.pdb: the PDB layout is
  off by 16 bytes for that class (shader-manager methods push `[this+0x1D4]` as the COM `this` of
  SetVertexShaderConstantF/SetPixelShader). Treat `engine/CShaderRenderManager.h` offsets past
  ~0x1C0 as suspect until checked.
- `CRenderManagerCore` derives from `CRenderStateManager` (base fills offsets 0..14908); the device
  is held twice, at +4 (state manager) and +0x3CF0.
- Untyped field-offset matching is useless for COM classification: `[r+4]`/`[r+8]` mix vertex-buffer
  Lock/Unlock with engine virtuals. Only PDB typing or a unique offset is trustworthy.

## Live session (user permitted the launch)

- Steam relaunches a debugger-created Fable.exe outside the debugger, so the tracer now attaches to
  a normally started game (`--attach`).
- The census seed `[0x1436E18]` was **0 at the frontend**. It is CEngine's copy of the render core,
  set in `CEngine::Initialise`. The frontend-live singleton is `[0x13CB508]`, whose +4 and +0x3CF0
  both held the device (confirming the census's two offsets). Both are now census seeds.
- The live device's vtable is a heap copy; QI/Release/CreateAdditionalSwapChain/Reset are
  apphelp.dll shims and the other 115 slots point into d3d9.dll.
- Incident: the first attach crashed before detaching and left all 45 game threads suspended.
  They were resumed with ResumeThread, and the game recovered. The tracer now detaches in `finally`.
  A second bug: pybag `go(timeout)` takes seconds, so 2000 meant 33 minutes. Both are in GOTCHAS.
- Pointer persist leaves resolved from `ego_r.exe`: all deep-serialize, none writes an address
  ([PERSISTED_TYPES](../../modern/PERSISTED_TYPES.md)).
- Retail input/event-package recording is compiled in but unreachable: four `.bss` flags
  (`0x13B85F5`–`F8`) are only read. Details and the launcher plan are in FRAME_TIMING.md.

- Live frontend trace (attach, about 33 min because of the timeout bug; capped at 3000 hits per method):
  40 methods. Of 55 exe call sites, 36 are in the static census and 19 are not (Create*,
  BeginScene/EndScene, Clear, `CMouseDX::Update` SetCursorPosition, 2D SetVertexDeclaration). 57 more
  calls come from inside d3d9.dll. `tools/modern/d3d9_live_report.py` → `docs/modern/D3D9_LIVE_CENSUS.md`.
  The game was left running and responsive after detach.
- Static: the world tick can't exceed 15 Hz in retail. Packed raw file-format structs were added
  to PERSISTED_TYPES.

## Open

- In-game live trace (load a save) for world-render methods. The only static recall gap left on the
  frontend trace is the stack-spilled CreateQuery.
- `LeanAngleStepPerFrame` is cleared: it is applied per world frame (WorldUpdate → UpdatePlayerControl
  → ControlledMove → ResolveFacingDirection → GFSmoothTowards). Found via Ego_r: the
  `LeanAngle` member, then the retail function of the same name. The retail def fields are
  Ego_r −4. Next suspects are GUI per-frame tunables and the mouse/camera render path.

- Census recall vs the live frontend trace is now 54/55 (98%). Levers added: live RTTI globals
  (`rtti_live_globals.py` → `ghidra_out/rtti_live_globals.tsv`), static instances and immediate
  getters, embedded-struct loads, member pointers (`add r, imm`), COM tail jumps (`[esp+4]` this),
  `this` typing from the file's RTTI vtables (11,311 methods), and a dominant-caller rule (≥3
  typed callers, ≥80% agreement; ClearRenderTarget had 5 CDisplayManager callers vs 1 mistyped).
  The census now has 341 D3D9 calls, 303 device sites and 45 methods. Still missed: `CreateQuery` in
  OnPostSwapScreens 0x9F73C0, via a stack-spilled pointer.
- Header-rule verifier warning: inventory exists (`header_surface.py`); the warning in
  `verify_and_land.py` is deferred while parity sessions share that file.

## Evening: recorder launcher (user: "Game should be free for you now")

- `tools/modern/recorder_poke.py` arms the compiled-in recorder at the frontend: VirtualAllocEx
  a retail `CWideStringData` (16 bytes: vector<wchar_t> + NoRefs at +0xC, not the PDB's
  wstring), store it in `GEventPackageFileName` 0x13B8660, set flag 0x13B85F7. Starting a world
  then runs `InitEventPackageSaving`, which creates the file and writes version `1` (three runs,
  `rec*.evp` = 4 bytes each).
- Record format decoded statically ([FRAME_TIMING](../../modern/FRAME_TIMING.md)): `u32 size` plus a `CompressIntoBuffer`
  0x9F19A0 image of the frame's `CGameEventPackageSet`, once per world update.
- Blocked: every world load in the shared install crashed with a plain `Fable.exe` launch, recorder
  on or off. That covered New Game twice with the recorder, once without (control), and a
  Continue of 1234234. The AV is in `HasPhysicsMesh` 0xA23C40 (null mesh) ← `OnCreate` 0x724010 ←
  `LoadSingleThing` (region thing load). Not caused by the recorder. Stopped per the loop rule, and
  logged in FINDINGS_LOG. Throwaway profiles `zrec*` were deleted. Fable is closed.
- Exit-catcher recipe: break on the 32-bit `KiUserExceptionDispatcher` / `NtTerminateProcess` /
  `ExitProcess` (SysWOW64 export RVA + module base from the attach log) and scan the stack for
  Fable return addresses. The stock dbgeng first-chance AV filter did not stop on this one.
- Static, after the live block: every named per-frame tunable is cleared. Screen-message typing runs
  in `CPlayerGui::Update` ← `CWorld::UpdateFrame` (retail 0x43A080 ← 0x4A5A40), which Ego_r shows
  is driven by the `EAUpdateFrame` game event. The Smack* fields are unused. Sprite rotation is
  world step + interpolated draw. `CResponseTimes` is speech ducking, stepped by elapsed world
  frames. Only the mouse path is left ([FRAME_TIMING](../../modern/FRAME_TIMING.md)).
- User pointed at the Nexus "Camera Wobble Remover" (.fmp). It is one game.bin entry,
  `MOVEMENT_DEFAULT`, with `LeanAngleTurnFactor/LimitAngle/StepPerFrame` set to 0 (stock 60 / 18 /
  3.6, decoded from game.bin.retail-bak entry 7076). The other fields are unchanged. Confirms that
  the wobble is the hero lean bending the facing the camera follows. Fork option: a lean-scale
  setting. The game was launched briefly to look for the def in memory (not resident at the
  frontend) and is still running at the frontend.
