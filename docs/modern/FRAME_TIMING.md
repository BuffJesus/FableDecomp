# Game-loop frame timing (retail Fable.exe)

Static RE, 2026-10-16. Answers the MODERNIZATION.md open question "where the game loop reads frame
time, and whether a fixed-timestep split is reachable". Evidence is retail disassembly (capstone)
plus Ego_r.pdb member and method names. Function names marked † come from propagated
`coverage.tsv` labels; they sit inside the `CMainGameComponent` address cluster (0x415E00–0x418C00)
and match its PDB method list, but have not been run through `label_trust.py`.

## Finding: the simulation is already fixed-timestep at 15 Hz

- The world advances in **world frames ("server frames") at a constant 15 per second.** The rate is
  the int32 global `0x1375550` = **15**. It is read at 632 sites in 461 functions and written by none
  (static scan of every function). `GetConstantFPS` on `CObstacleNavigatorExternalInterface`,
  `CGroundNavigatorExternalInterface` and `CGameScriptInterface` simply return it. Gameplay timers
  multiply or divide by it (crime fines, drunkenness, AI deeds, save game).
- `CMainGameComponent` (PDB) is a lockstep server/client loop: `CurrentServerFrame`,
  `InputServerFrame`, `ConvertSecondsToWF` / `ConvertWFToSeconds` / `ConvertGTToWF`,
  `IsTimeForServerUpdate`, `GetRenderInterpolate`, `LastInterpolationInfo` (`CInterpolationInfo`),
  `FrameRateSmoother`, `NoRenderFramesSinceLastGameUpdate`.
- **Rendering is decoupled and interpolated.** `IsTimeForServerUpdate`† (0x41674A) asks
  `GetGameTimeOfNextPresentCompletion`† (0x4166E2: refresh-interval prediction from the display's
  present statistics) and allows a world update only when that time has passed the next world-frame
  boundary (`frame / 15`). Between world frames the renderer draws interpolated states.
  Exactly: update iff `predictedPresentTime * 15 - frame > 1.0`. Two override paths force an update
  on every call: `[0x13B8688] != 0`, or `[0x13B860C]` set with input-manager+0x38 set and
  `[0x13B8629]` clear. All three bytes are `.bss` dev flags that no retail instruction writes (only
  `IsTimeForServerUpdate`, `CMainGameComponent::Render` and `DoRender` read them). So statically,
  retail's world tick cannot run faster than 15 Hz at any frame rate.
- Clock: `GFGetTime` (0x9E1BC0) returns seconds as a double from QueryPerformanceCounter, falling
  back to `timeGetTime` when QPC is unavailable, relative to a stored base. It has 170 direct call
  sites. `GetCurrentGameTime`† (0x416231) = `GFGetTime() - GameStartTime` (`this+0x60`).
- Main loop: `CMainGameComponent::Run` (0x4189C2) loops on either `UpdateForcedTickSpeed`† (0x417747,
  console `ForceUpdateTick` mode with `ForceUpdateTickSpeedDesiredFramerate`) or `NewFrame`
  (0x9F8BA0, a `timeGetTime` frame-rate counter) + `UpdateRegular`† (0x4162B5). `UpdateRegular`
  times its two virtual calls (+0x14, +0x1C) with `GFGetTime` into the globals `0x13B8690` and
  `0x13B8698` (per-phase durations).

## Retail input recording already exists

`CMainGameComponent` carries `EventPackageSet` (`CGameEventPackageSet`), `EventPackageFile`,
`LoadingEventPackages` / `SavingEventPackages`, `BeginInputSaving` / `BeginInputLoading`,
`WorldSeed` / `LocalSeed`, `CheckSync` and `PeekWorldChecksum`. Input arrives as per-world-frame
event packages, which is the lockstep design inherited from Xbox networked play. This is the
"input record/replay" tool that MODERNIZATION "Checking the fork" plans to build. The fork should
reuse it rather than invent one: record event packages on retail or parity, replay them on modern
x86/x64, and compare `PeekWorldChecksum` per world frame.

### How it is switched on (and why retail can't)

Static read, 2026-10-16:

- `CMainGameComponent::Run` 0x4189C2 calls `BeginInputSaving` 0x416268 if byte `[0x13B85F6]` is set,
  else `BeginInputLoading` 0x41627F if `[0x13B85F5]` is set. Both pass the `CWideString` global
  `0x13B865C` as the file name. They forward to the input manager `[0x13B8388]`, to `StreamInputsToFile` 0x9F4CF0 and
  `StreamInputsFromFile` 0x9F5E10 respectively, together with the object at `0x13B85E4`.
  `coverage.tsv` labels both wrappers "BeginInputLoading"; the callees prove 0x416268 is Saving.
- `CMainGameComponent::InitWorld` 0x41735A calls `InitEventPackageLoading` 0x416056 if `[0x13B85F8]`
  is set, and `InitEventPackageSaving` 0x4160CC if `[0x13B85F7]` is set. Both use the file name
  `GEventPackageFileName` (`CWideString` at `0x13B8660`). The file is opened in the stream at
  `this+0x16154`. Its header is one 4-byte version word (`this+0x16174`, written as 1). Per-frame
  packages then go through `SaveEventPackage` 0x9F53C0 / `LoadEventPackage` 0x9F5650.
- **The four flags sit in `.bss` (zero at load), and no retail instruction writes them.** The only
  references are the two readers above. They are not registered with `InitialiseConsoleVariables` or
  `GFParseCommandLine`, which do handle the neighbouring dev globals in the same block. The file-name
  globals are only constructed by their static initializers. So the recorder is compiled in but
  unreachable. To use it on retail, a launcher or debugger must set the flag byte and the
  `CWideString` before `Run`/`InitWorld`. That is the planned route for the fork's replay oracle.
  `CWideString` is one pointer to a ref-counted `CWideStringData`. In retail that is **16 bytes**,
  a `std::vector<wchar_t>` {first, last, end} plus `NoRefs` at +0xC (allocator 0x99B3C0), not the
  28-byte `std::wstring` Ego_r.pdb shows. The readers take `operator const wchar_t*` (0x99AF00,
  `[[s]]`) and copy the text, so the launcher allocates one block with a huge refcount.
- **Launcher: `tools/modern/recorder_poke.py <pid> --save-events <abs path>`** (also `--load-events`,
  `--save-inputs`, `--load-inputs`, `--status`, `--clear`), run while the game sits at the frontend.
  Verified live 2026-10-16: starting a world runs `InitEventPackageSaving`, which creates the file
  and writes the 4-byte version word `1`. The round trip is still untested. Every world load in the
  shared install crashed before frame 1, recorder on or off (New Game and a Continue):
  null physics mesh in `HasPhysicsMesh` 0xA23C40, reached from `OnCreate` 0x724010 ←
  `LoadSingleThing` 0x51FD80 (FINDINGS_LOG 2026-10-16). It needs a launch that can load a world.
- Per world update (`CMainGameComponent::Update` 0x418289): when saving, after the world steps,
  `AddEventPackageSetToSave` 0x4161A7 compresses the frame's `CGameEventPackageSet` (`this+0xA4`)
  into the scratch region `0x13CB498` and writes `u32 size` plus `size` bytes. When loading,
  `GetEventPackageSetFromSave` 0x416148 reads the same pair whenever `IsTimeForServerUpdate`
  allows, decompresses it (0x9F1AC0) into `this+0xA4` and applies it (0x41726D) in place of
  local input. File = `u32 version (1)`, then one record per world frame.
- Record encoding (`CGameEventPackageSet::CompressIntoBuffer` 0x9F19A0). In memory, the set is
  {u32 count, packages[] stride 0x648}, a package is {u32 id @0, u8 eventCount @4, events[40]
  @8 stride 0x28}, and an event is {u16 type @0, u8 @4, payload @5, u8 payloadLen @0x25, u8 flag
  @0x27}. On disk: `u8 packageCount`, then per package `u8 eventCount, u32 id`, then per event
  `u16 type | (flag ? 0x8000 : 0)`, `u8 [@4]`, `u8 len`, `len` payload bytes.
- Input stream shape (from `StreamInputsFromFile` / `LoadAllEventPackages` 0x9F5D70 /
  `LoadEventPackage` 0x9F5650): a 12-byte header, then package records read until end of file into
  a vector of 0x30-byte entries at `this+0x4C`. Each entry's +0xC is the input frame. Replay applies
  entry `[this+0x58]` when its frame equals `this+0x5C`. `SaveEventPackage` 0x9F53C0 builds each
  package from the input state (events are 0x34 bytes each) plus two floats from the device
  `[this+0x2C]` (vcalls +0xC/+0x10, probably analog axes). Field-level layout is still to be read.

## Consequences for the fork

1. **Uncapped frame rate is reachable without a new timestep.** The world is already fixed at 15 Hz;
   only render-side code must stop assuming the retail frame rate. The reported frame-rate bugs
   (FABLE_TLC_REPORTED_BUGS: movement, multi-hit spells, physics/animation, menu repeat) therefore
   point at code that reads *render* time (`GFGetTime`, `LastFrameRenderDuration`, `NewFrame`'s
   counter, `GetRenderInterpolate`) and feeds it back into state. The 170 `GFGetTime` call sites and
   the `LastFrameRenderDuration` / `FrameRateSmoother` readers are the audit list.
2. **Replay determinism is per world frame.** x87→SSE2 drift (MODERNIZATION "x64 hazards") is
   measurable with the engine's own `PeekWorldChecksum` at each of the 15 Hz frames.
3. The world-frame constant must stay 15 in the fork. 461 functions are tuned in world frames.

## GFGetTime callers classified so far

| Function | Use | Frame-rate dependent? |
| --- | --- | --- |
| `UpdateFrame`† 0x4A5A40 (world) | wall-clock throttle: action once `GFGetTime() - [0x13B89F0] > 20.0` s; also stamps `this+0x180` | No (real seconds) |
| `CInputManager::Update` 0x9F57A0 | timestamps each input event with `GFGetTime()` before queueing (0xA03CB0 / 0xA03CE0) | No (real-time stamps) |
| `UpdateRegular`†, `Run`, `IsTimeForServerUpdate`†, `GetCurrentGameTime`† | loop scheduling and phase timing | No (scheduler) |
| `UpdateInterpolation`† 0x42DBFA | `(GFGetTime() - this+0xB8) * 15.0f` = render interpolation fraction in world frames | No (render only) |
| frontend `Update`† 0x42DC94 | `dt = GFGetTime() - this+0xC0`, **clamped to 0.1 s**, passed to the UI update 0x599E3F | Only below 10 fps (clamp slows UI); variable-dt above |
| frontend `Draw`† 0x42DF9E | stores the draw time (`this+0xBC`) | No |

**2026-10-17 sweep of the render-side callers** (185 direct `GFGetTime` sites in 101 functions;
the instructions after each call read with capstone). No site feeds a render-rate count into state:

| Pattern | Functions | Frame-rate dependent? |
|---|---|---|
| GUI container update: `dt = now - last; last = now;` then `child->vcall(+4)(dt)` for each child widget, run from `DrawPreparePrimitivesForRendering` | 0x47D1AC, 0x56F090, 0x581EBD, 0x59C7C7, 0x5C2FE1, 0x5E3EA0 (mislabelled `IsHeroControlledByPlayer`), 0x5FAF50, 0x61AD60, 0x625940, 0x7E49C0 | No by design: variable-dt widget animation at render rate. Each child's use of `dt` is not yet read. |
| Real-time threshold (`now - start` compared with a constant or a field) | `ChangeSelection` 0x494380 (selection repeat), `GlobalUpdate` 0x47E9AE, UI action delay 0x599E3F/0x59A238, speech/text timeout 0x716DE0, `ProcessTransitions`/`StartThemeTransition` (theme blend), `GFProcessBreath` (microthread budget), `ProcessObjects` 0x98ECD0 | No |
| dt-scaled accumulation (`acc += (now - last) * rate`) | `UpdateOffGameTickProcessInfo` 0xB23A90 | No |
| Profiling and fps statistics | world `Update` 0x49DFB0 (+0x90/+0x98 durations), `OnPostSwapScreens` (frame-time history), `FrameEnd` | No (display only) |
| Seconds converted to world frames (`× [0x1375550]`) | `SetUpTextAndSpeech` 0x716B30 | No |

What is left of the audit is the child widgets' `vcall(+4)(dt)` bodies, and any per-render-frame
code that counts calls without reading the clock. Ego_r names give the complete render-rate hook
list to read:

- GUI: the `NUISystem` components' `Update(float dt)` (28 classes: `CComponent`, `CList`, `CText`,
  `CSlider`, `CScrollBar`, `CMovie`, `CMouseCursor`, `CMorphingSprite`, …) plus
  `CComponent::UpdateZoom/UpdateColour/UpdatePosition(float)`. These are the container children above.
- World: `CWorld::RenderUpdate` → `CThingManager::RenderUpdateThings` → `CThing::RenderUpdate`,
  and `CThingSoundEmitter::RenderUpdate`, `CPlayerGui::RenderUpdateGuiTimers` and
  `CDisplayEngine::RenderUpdate`. All of these take `CInterpolationInfo` (a fraction between world
  frames), not a count. `CTCBase::RenderUpdate(float)` exists, but no thing component overrides it
  in Ego_r, so components do not get render-rate updates.
- Environment and detail: `CEnvironment::RenderUpdate(float)` and the `CLocalDetailCacheMap`
  `RenderUpdate` family (foliage and detail LOD).

**2026-10-17: render-rate hooks read (static; vtables recovered from RTTI, capstone).**

- **Things.** `CThingManager::RenderUpdateThings` 0x51EBD0 calls vtable slots +0x54 and +0x68
  (the `RenderUpdate` pair, `CInterpolationInfo`) on every thing in each update list. In retail,
  every CThing class's slot 26 is an empty `ret 4` (base 0x4CD1D0, creature 0x661A40, and player
  0x6A9550, which jumps to the creature stub). Slot 21 is `return false` (0x4C7460) for all but
  `CThingSoundEmitter` (0x836AD0). That one moves the emitter's sound to the owner's
  interpolated facing/position, and kills the emitter once its sound has stopped. So no thing,
  creature or component changes state at render rate.
- **Environment.** `CEnvironment::RenderUpdate` 0x6B42F0 clamps the interpolation fraction to
  [0, 1] and blends the previous and current theme values. Pure interpolation.
- **Camera.** `CGameCameraManager::GetRenderCamera(float)` 0x697F30 →
  `GetInterpolatedCamera` 0x6978D0 lerps position, FOV and orientation between two stored
  cameras (+0x04 and +0x3C). `CGameCameraManager::Update()` takes no dt. Camera motion is
  world-tick, and the render only interpolates it.
- **GUI.** Slot 1 of every `NUISystem` class is `Update(float dt)`. `CComponent::Update` 0x531EC0
  adds dt to the component age (+0x30), then calls UpdateZoom 0x52F5C0, UpdateColour 0x52FFD0
  and UpdatePosition 0x52F900 with dt. Those evaluate curves of elapsed/duration. An automated
  scan of all 50 slot-1 bodies (`dtscan`: member counters, constant float steps in bodies that
  never read dt) found no per-call stepping. The overrides forward dt to their base and
  otherwise act on one-shot pending flags (CScrollBar, CDraggable, CListArrow,
  CSwappingStateComponent). CList's two constant adds are the compiler's unsigned-to-float
  fix-up (2^32).
- **Loop bookkeeping.** `CMainGameComponent+0x16138` counts render frames (incremented by
  `Render` 0x417001 and `ForceRender` 0x4164E0) and is reset per world frame, but nothing reads
  it. The world period is 1 / (15 × forced tick speed at +0x38). The global 15 at 0x1375550
  converts seconds to world frames, and no render-rate input changes it.

So the whole render path is interpolation, real-dt GUI curves, or real-time thresholds. Static
reading finds no code that advances game state per render frame. That leaves three ways a
high-fps report can be real:
(1) interpolation jitter, since the fraction comes from the predicted present-completion time
(`GetGameTimeOfNextPresentCompletion` 0x4166E2), which uncapped or VRR presentation can mispredict;
(2) a driver or vsync effect on the world tick's own scheduling, which only a live
`CurrentServerFrame`-vs-wall-clock sample settles; or
(3) the report is wrong. Further static reading is not the way to settle it.

Before this audit, the timing code read so far was real-time or world-frame based, and none of
it explained a bug that appears only at *high* frame rates. The reported high-fps bugs must sit in code that counts render
frames (per-frame increments without dt) rather than code that reads the clock. That needs a
different scan: per-frame counters in render/UI update paths, starting under the UI update 0x599E3F.

### "Per frame" tunables (PDB member names)

Of the PDB members named per-frame, most sit on thing components (`CTCDivineWrath::NumFramesBetweenDamageHit`,
`CTCTurncoatSpell::PerFrameStaminaModify`, `CTCForcePushed::DistanceMovingPerFrame`, ...). Those
advance on the world tick and are frame-rate-safe, because the world cannot run faster than 15 Hz
(static result above). So if the "multi-hit spell damage changes with frame rate" report is real,
the cause is not the world tick. A live sample of `CMainGameComponent::CurrentServerFrame` against
wall time at uncapped vs vsynced rates would confirm this.

**Cleared: `CPlayerMovementDef::LeanAngleStepPerFrame` is per world frame.** Retail chain:
`CThingPlayerCreature::WorldUpdate` (vtable 0x12457FC slot 19) → `UpdatePlayerControl` 0x6AD970 →
`ControlledMove` 0x6AD700 → 0x6AD260 → tail jump to `ResolveFacingDirection` 0x6AB820 →
`GFSmoothTowards<float>` 0x6ADC20, which moves `LeanAngle` toward its target by at most
StepPerFrame. Retail offsets: def pointer `this+0x1EC`, `LeanAngle` `this+0x1FC` (Ego_r +0x218 /
+0x228). The def fields are Ego_r −4: TurnFactor +0x70, LimitAngle +0x74, StepPerFrame +0x78,
each scaled by 1/360 (degrees to turns). The lean is zeroed when the hero's position delta is
below a threshold, and clamped to ±LimitAngle. So the confirmed "camera wobble" bug is about
how the camera uses the lean, not frame rate.

Stock values (`MOVEMENT_DEFAULT`, `PLAYER_MOVEMENT` entry 7076 of `game.bin`; field keys are
CRC32 seed 0 of the member name): TurnFactor 60, LimitAngle 18°, StepPerFrame 3.6° per world
frame. That is full lean in 5 world frames (1/3 s). The Nexus mod "Camera Wobble Remover"
(`FableTLC-CameraWobbleRemover.fmp`, a FableMod patch of that one entry) sets exactly these three
to 0 and leaves the other 13 fields stock. That confirms the wobble is the lean. In Ego_r, only
`CThingPlayerCreature`'s movement and facing code reads `LeanAngle` (`ResolveFacingDirection`,
`Resolve/ApplyRelativeMovementAcceleration`, `OnCreate`), and no camera or render function does.
The lean bends the hero's facing, and the follow camera inherits it. For the fork this is a
settings toggle (scale or zero the three def fields), not a timing fix.

**Cleared (static, 2026-10-17): in-game mouse look is stepped per world frame.**

- `CMouseDX::Update` 0xAB58E0 (vtable 0x129EA14 slot +0x14) drains the DirectInput buffer
  (`GetDeviceData`, 0x14-byte records, up to 256 per read, re-`Acquire` on `DIERR_INPUTLOST`), sums
  the X/Y/Z deltas, and with filtering on (`+0x484B`) averages them with the previous call's deltas
  (×0.5 constant 0x122F59C). It queues **one** input event per call, and when re-centring
  (`+0x343E`) is on it calls `SetCursorPos` to the client centre. The deltas are DirectInput relative
  data, so re-centring loses no motion.
- `CInputManager::Update` 0x9F57A0 (vtable 0x129C79C slot +8) calls the mouse (`[this+0x24]`, +0x14),
  the keyboard (`[this+0x28]`, +4) and each pad in `[this+0x2C..0x30]` (+0x28).
- In-game, its only driver is `CMainGameComponent::GetInputs` 0x416E78 (vtable 0x122F180 slot +0x18,
  calls `[0x13B8388]` +8). `CMainGameComponent::Update` 0x418289 calls `GetInputs` only inside the
  `GetGameEventPackageSet` 0x4AEBA0 branch, which is gated by `IsTimeForServerUpdate`. So the device
  is read **once per world frame (15 Hz)**, and each event carries all motion since the previous world
  frame.
- `CInputProcessCameraLookAround::ProcessInput` 0x68D790 reads analog action 0x4D or 0x3D (or digital
  0x3E/0x3F as ±1) and **sets** the look vector with 0x697B10 (`+0xCC/+0xD0` = value × 0x125C1E4/E8,
  then the camera's +0x90 via 0x84CF40). That is last-wins, which would lose motion if events
  arrived faster than they are consumed. With one event per world frame it doesn't.
- The per-render-frame `CMouseDX::Update` calls in the live trace were taken at the frontend, which
  is not in this path. The second "reference" at 0x129D628 is slot 2 of `CInputManagerDX`'s own
  vtable (0x129D620, from RTTI), and it is the frontend's driver: `Input` 0x42E3EE calls
  `[0x13B8388]->vtbl[2]` (Update) at 0x42E434, then `EventScanStart`. The frontend is driven by
  `CNewFrontendGameComponent::Run` 0x42EC7C, which calls `Input` 0x42E3EE (→ `[0x13B8388]` +8),
  `Update` 0x42DC94, `UpdateInterpolation` and `Draw` once per **render** frame, so menu input is
  render-rate. Its key and stick repeat uses the real-time `CStopWatch` at `+0xD8` (0.333 s initial
  delay `0x3EAA7EFA`, then 0.085 s `0x3DAE147B`), so it is not frame-rate dependent.
  (`CFrontendGameComponent::Run` 0x496CF0 → `Input` 0x4963A0 is the other frontend component; its repeat is not yet read.)
- Consequence for the fork: raising the render rate leaves mouse sensitivity unchanged, as long as the
  input service keeps sampling at the world-frame boundary. A per-render-frame camera (smoother
  mouse look) would need the look vector to accumulate instead of being set: a switch, not the default.
- **Buffer size: 256 records** (`CMouseDX::SetExclusiveMode` 0xAB5710 creates the device, then
  `SetCooperativeLevel` 5 = exclusive|foreground or 0xA = non-exclusive|background,
  `SetProperty(DIPROP_BUFFERSIZE, 0x100)`, `SetDataFormat(c_dfDIMouse2 0x12ADDFC)` falling back to
  `c_dfDIMouse` 0x12ADBF4, then `Acquire`). The buffer is shared by axes and buttons and is drained
  once per world frame (66.7 ms). A 1000 Hz mouse moving diagonally fills about 133 records per frame
  and fits. A 4000/8000 Hz mouse fills about 530/1,070 and overflows (`DI_BUFFEROVERFLOW`), so mouse
  look loses motion on modern high-polling mice, and on any hitch longer than about 128 ms at 1000 Hz.
  Fork: the input service accumulates raw-input deltas with no record cap and hands the world one
  summed delta per world frame. That keeps retail behaviour and removes the overflow. Retail
  workaround: set the mouse to 1000 Hz or lower.
- **Candidate retail patch (not applied, untested in-game).** The read helper 0xAB4910 refills its
  256-record cache with `GetDeviceData` whenever the cache is exhausted, and `CMouseDX::Update`
  calls it until it returns nothing. So the per-call 256 is not a cap, and only the device buffer
  size limits a frame. Raising both `DIPROP_BUFFERSIZE` immediates in `SetExclusiveMode` is enough:
  the dword at VA 0xAB57BA (file 0x6B57BA, exclusive branch) and VA 0xAB5871 (file 0x6B5871,
  non-exclusive branch), each `00 01 00 00` (256) in Steam `Fable.exe` (SHA-256 prefix
  `41dc91090ae85371`, 16,666,624 bytes), changed to e.g. `00 20 00 00` (8,192, enough for an 8 kHz
  mouse at 15 Hz with headroom for hitches).
  Tool: `python tools/modern/mouse_buffer_patch.py --exe <Fable.exe> --out <copy.exe> [--records N]`.
  It accepts only the unmodified Steam executable (full SHA-256), checks both `C7 44 24 24 00 01
  00 00` sites, writes a copy (it never modifies the input), and changes exactly 2 bytes. `--check`
  reports a file's current buffer size. Not yet run in-game.

**Cleared: the GUI, sprite and speech-gain per-frame tunables** (Ego_r readers by member offset,
then the retail counterpart by call order):

- `NumberOfCharactersScreenMessageAddsPerFrame` (screen-message typing) is read only by
  `CPlayerGui::Update(bool)` / `CScreenMessage::Update`. `CPlayerGui::Update` (retail 0x43A080 on
  singleton `[0x13B8790]`) is called only from `CWorld::UpdateFrame` (retail 0x4A5A40, call
  0x4A5E4D; same call sequence as Ego_r 0x4AF3D0). In Ego_r, `CWorld::UpdateFrame`'s only caller
  is `CWorld::EAUpdateFrame(const CGameEvent*)`: the world frame is itself a game event, so this
  runs per world frame.
- `SmackAlphaFadePerFrame`, `SmackScaleFactorPerFrame` and the other `Smack*` fields: in Ego_r
  nothing besides `CPlayerGuiDef::Transfer` and `operator=` touches them, so they are loaded but unused.
- `CTCGraphicAppearanceSprite::RotationAmountPerFrame`: `FrameUpdate` copies `RotationAngle` to
  `OldRotationAngle` and `AnimStage` to `LastAnimStage`, then advances them, and the draw
  interpolates (`GetInterpolatedAnimStage`). That is the engine's world-step + render-interpolate
  pattern.
- `CResponseTimes::Attack/ReleaseMaxDeltaPerFrame` belong to `NSpeechGainManager` (speech
  ducking), not input. `NSpeechGainManager::Update` (Ego_r 0x70E0F0) computes
  `GetCurrentServerFrame() - lastFrame` and steps each gain by that many world frames, so its
  call rate does not matter.

No named per-frame tunable has turned out to be render-frame dependent. The remaining candidates
are code that counts render frames without a name saying so (the frontend `Update` variable dt;
`CMouseDX::Update` is cleared above).

About 40 of the rest are loaders, decompressors and profiling (`LoadFromFile`, `Decompress`,
`PreloadCompletedBank`, `LoadNewScene`, save/load), which are frame-rate neutral by construction.

## Next

- ~~Classify the remaining `GFGetTime` callers~~ done 2026-10-17 (table above). Next is the GUI
  child widgets' `vcall(+4)(dt)` bodies.
- Event-package record/replay on retail: the launcher and record format are done (above). Still
  needed is a launch that loads a world (the shared install's plain launch crashes in region thing
  load). Then record a short session, relaunch with `--load-events`, and compare world state.
