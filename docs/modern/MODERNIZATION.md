# Modernization fork — x64, D3D12 + Vulkan, ffmpeg, moddability

Plan, 2026-09-26. Status: **design agreed, nothing built**. Owner decisions (user, 2026-09-26) are
marked **[decided]**. Evidence for the platform list: [PLATFORM_SURFACE](PLATFORM_SURFACE.md)
(generated). Scripting runtime: [SCRIPTING_REDESIGN](../engine/SCRIPTING_REDESIGN.md). Layer rules:
[ARCHITECTURE](../ARCHITECTURE.md).

Contents

- [Goal](#goal)
- [Decisions](#decisions)
- [Reconstruction standard: functional first, byte-close](#reconstruction-standard-functional-first-byte-close)
- [The fork gate](#the-fork-gate)
- [Steps](#steps)
- [x64 hazards](#x64-hazards)
- [Graphics](#graphics)
- [Audio, video, input, platform](#audio-video-input-platform)
- [Player-facing features](#player-facing-features)
- [Linux without a Linux machine](#linux-without-a-linux-machine)
- [Mods across the fork](#mods-across-the-fork)
- [Checking the fork](#checking-the-fork)
- [Work that can start now](#work-that-can-start-now)
- [Open questions](#open-questions)

---

## Goal

Once the reconstruction plays TLC through, turn it into our own program: 64-bit, modern graphics
(Direct3D 12 and Vulkan), ffmpeg for video, first-class modding. It still loads the player's retail
data and retail saves.

**Not goals:** shipping retail assets; changing gameplay by default (fixes go behind switches); a
rewrite from scratch. The fork starts from the reconstructed source and changes it step by step.

## Decisions

| Question | Answer |
|---|---|
| Byte purity | **[decided]** Functionally identical is the requirement; byte parity within a reasonable margin (see next section). `__asm`/naked bakes still do not count. |
| Platforms | **[decided]** Windows first. Linux is wanted, kept buildable, not yet tested (no Linux machine or VM). |
| Graphics | **[decided]** Hand-written D3D12 and Vulkan backends behind our own render interface (RHI). No third-party abstraction layer. |
| Location | **[decided]** Separate CMake tree in this repo (`modern/`), apart from the VC7.1 parity tree. |
| Fork gate | Source-complete, not byte-complete (below). |

## Reconstruction standard: functional first, byte-close

This replaces "every function must be a byte-exact MATCH" as the bar for counting a function as
reconstructed. It applies to the parity tree, not just the fork.

A function is **reconstructed** when all of these hold:

1. **Genuine C++** written from the decompilation. `__declspec(naked)`, `__asm`, and `_emit` remain
   `asm_bake`: an oracle, never reconstruction.
2. **Functionally identical**: a behaviour fixture passes against the retail oracle (same outputs,
   same side effects on the modelled state, same calls in the same order).
3. **Byte-close** under VC7.1: the new `near_match` grade, proposed defaults (tune after the first
   sweep):
   - instruction count within ±10% of retail, and
   - ≥ 90% opcode-sequence similarity after masking register numbers, relocations, and stack
     offsets.

`MATCH` / `RELOCATION_MATCH` stay the best grades and are still worth taking when a spelling gets
there. What changes is how far to chase: once a function is `near_match` and its fixture passes, stop
unless the remaining diff hides a behaviour difference. Register-allocation and scheduling dead ends
(the documented VC7.1 cases) are `near_match`, not open work.

What stays forbidden: invented algorithms, fitted constants, and hand-made tables in place of the
engine's own code (see the data-writer rule in the project memory). "Functionally the same" means
the same algorithm, compiled.

Needed tooling: a similarity score in `verify_and_land.py` (today it records only
`DIFFER(built v retail)` lengths) and a `near_match` column in the coverage dashboard.

## The fork gate

The parity build links reconstructed functions next to retail bytes. A 64-bit build cannot contain
any retail x86 bytes, so the fork needs **C++ for every function it links**. With the standard above,
`near_match` and `functional` source both qualify. The gate is:

- **Source-complete** for the code the game runs (boot → frontend → world → every quest), and
- the reconstructed x86 build **plays through** on the autopilot/runner (the existing
  in-game harness), and
- retail saves load in it.

The VC7.1 parity tree does not stop at the fork. It remains the oracle the fork is compared against.

## Steps

Each step changes one thing, keeps the game playable, and is compared against the step before it.

| # | Step | Changes | Exit test |
|---|---|---|---|
| 0 | Seams (now, in the parity tree) | Platform calls sit behind named services; no D3D9/DInput/OpenAL/DirectShow types in gameplay headers | Header rule holds; PLATFORM_SURFACE functions all tagged with their service |
| 1 | Modern compiler, still x86 | CMake tree, current MSVC and clang-cl, `/W4`; retail CRT use becomes standard library | Replay (below) matches the parity build |
| 2 | x64, still D3D9 | Pointer-size fixes, explicit serializers (hazards below). 64-bit `d3d9.dll`, OpenAL Soft, DirectShow still present | Replay matches; retail saves load; a save round-trips |
| 3 | Platform layer | Window, input, timing, files, config, paths, audio device, video decode on our own interfaces; ffmpeg replaces DirectShow; OpenAL Soft replaces `wrap_oal` | Replay matches; no Win32 calls outside `platform/win32/` |
| 4 | RHI + new backends | D3D9 calls behind the RHI; D3D9 backend first (identical frames), then D3D12, then Vulkan | Frame captures match the D3D9 backend within tolerance |
| 4b | Settings + controllers | Settings screen and config file; controller support, graphics options, gameplay/accessibility switches ([Player-facing features](#player-facing-features)) | Every setting round-trips through the config; defaults reproduce retail |
| 5 | Moddability | SCRIPTING_REDESIGN steps 4–7, mod loader, FableForge as the editor | Converted quests and FSE-shim mods pass their runs |

Compatibility fixes (the AMD fade/audio reports, reported-bug list) land behind switches, default
off, after step 2.

## x64 hazards

The API calls are the easy part. These are where 64-bit breaks things quietly:

- **Persisted layouts.** Any struct read or written as raw bytes with a pointer, `long`, or
  pointer-sized member changes size. Candidates: saves (`CPersistContext`), game.bin defs
  (`Transfer<T>`), bank/`.big` headers, `.lev`/`.tng`/`.stb` readers. Each gets an explicit
  serializer that reads the retail 32-bit layout. Retail saves must load; our saves stay
  retail-readable until a documented format bump.
- **PDB layouts are 32-bit.** `rebuild/include/engine/*.h` and `fable_types.h` are parity-only. The
  fork needs its own types; offset asserts become 32-bit-only or move to the serializers.
- **Pointers in integers.** `DWORD` handles, `SetWindowLong(GWL_USERDATA)`, pointer→int hashing and
  sorting, `int` sizes. `/W4` + `-Wshorten-64-to-32` in step 1 find most of them.
- **x87 → SSE2.** Float results change slightly. Physics, animation, navmesh, and AI thresholds may
  drift, so replay comparison needs tolerances and a divergence report, not exact equality.
- **Struct packing and alignment** change with pointer size; anything shared with data files must be
  explicit.
- **Retail COM, CRT, and STLport-era idioms** (VC7.1 `std::` layouts, `_Uninit_copy` families)
  become the modern standard library.

## Graphics

**[decided]** Hand-written RHI with D3D12 and Vulkan backends, so we control it.

- The RHI is shaped by what Fable actually does, measured from the reconstruction: the
  `IDirect3DDevice9` call-site census (open task), vertex/pixel shader set (`VSHADER_*` names in the
  exe), render targets, fixed-function states in use.
- A D3D9 backend comes first and must reproduce retail frames. It is the reference the new backends
  are measured against, and it lets the RHI land without changing the picture.
- Shaders: retail D3D9 shader programs (assembly or compiled bytecode) get re-authored in HLSL,
  compiled with DXC to DXIL (D3D12) and SPIR-V (Vulkan). Fixed-function paths become shaders.
- D3D12 before Vulkan on Windows (debug layer and PIX); Vulkan next, since it is the Linux path.
  RenderDoc covers both.
- D3DX9 (`d3dx9_25.dll`, 11 imports) is replaced by our own code: matrix math, texture loading
  (the DXT/surface loader is engine code anyway), shader compilation at build time.

## Audio, video, input, platform

From [PLATFORM_SURFACE](PLATFORM_SURFACE.md):

- **Audio is OpenAL**, loaded at runtime (`OpenAL32.dll` / Creative `wrap_oal.dll`, DirectSound3D
  underneath). Not Miles. OpenAL Soft keeps the same API and already runs on x64 and Linux, so audio
  is mostly a library swap. Xbox ADPCM decode stays engine code (ffmpeg decodes `adpcm_ima_xbox` as
  an oracle; `tools/xbadpcm.py` is byte-exact against it).
- **Video is DirectShow** via COM (`CVideoSys`, see [VIDEO_SYSTEM_RE](../engine/VIDEO_SYSTEM_RE.md)).
  ffmpeg decodes the retail `.wmv` (WMV + WMA) into the same texture handoff `DoRenderSample`
  performs today. Link ffmpeg as LGPL shared libraries.
- **Input is DirectInput 8** plus IME (`imm32`). The platform input service does raw input and
  XInput/GameInput on Windows, evdev later. This is where the shelved gamepad-redefine work
  ([GAMEPAD_REDEFINE_PATCH](../engine/GAMEPAD_REDEFINE_PATCH.md)) gets revisited.
- **OS**: `kernel32` (518 calling functions, mostly critical sections, files, threads),
  `advapi32` registry → a config file, `winmm` timing → the platform clock, `wsock32` only for cut
  co-op, `ConfigDetect.dll` hardware tiers → our own probe on the RHI device.
- Platform code lives in `modern/platform/win32/` (later `modern/platform/posix/`); nothing else
  includes `<windows.h>`.

## Player-facing features

What players actually notice. All of it lives in the fork. Retail defaults stay the defaults.
Everything is stored in one config file (step 3), editable from an in-game settings screen built
on the engine's own frontend UI, so it looks like Fable, not like a launcher.

**Controller support** (lands with the step 3 input service):

- Full gamepad play through XInput / GameInput on Windows and evdev on Linux. Hot-plug, several pad
  types, button prompts matching the pad.
- Rebinding for keyboard, mouse, and pad on the full action list. The retail action enums are
  proven from FableWin.pdb ([CONTROLLER_ENUMS](../engine/CONTROLLER_ENUMS.md)); the Xbox disc has the
  original pad bindings as evidence ([XBOX_DISC_ASSESSMENT](../engine/XBOX_DISC_ASSESSMENT.md)). The
  shelved retail patch ([GAMEPAD_REDEFINE_PATCH](../engine/GAMEPAD_REDEFINE_PATCH.md)) and the
  missing `EGameAction` name table stop mattering once the input layer is ours.
- Stick dead zones, sensitivity, invert axes, right-stick camera (`AllowRightStickRotation` /
  `AllowRightStickZoom` already exist as engine settings).

**Graphics settings** (land with step 4, the RHI):

- Any resolution, borderless / exclusive fullscreen / windowed, monitor selection, vsync,
  frame-rate cap.
- Widescreen done properly: horizontal-plus FOV, UI laid out for the aspect ratio, mouse mapping
  that follows the resolution (a reported bug: resolution changes break UI scaling and mouse
  mapping).
- Quality: texture filtering (anisotropic), MSAA or a post AA, shadow resolution, draw and LOD
  distance, foliage density, render scale. Retail quality tiers from `ConfigDetect.dll` become
  presets.
- Later: HDR output, ultrawide HUD placement.

**Frame rate.** [FABLE_TLC_REPORTED_BUGS](../engine/FABLE_TLC_REPORTED_BUGS.md) lists movement,
multi-hit spell damage, physics/animation, and menu repeat as changing with frame rate. Before the
cap can be raised, the simulation has to run at a fixed timestep with rendering interpolated between
steps. That needs the game loop's timing RE first (the step-2 replay will show where the logic reads
frame time). Until then, the cap stays at the retail rate.

**Gameplay and accessibility settings** (switches, after step 2, default = retail):

- Subtitles on/off, text size, speaker names; HUD elements on/off; camera distance and FOV.
- Separate audio volumes (music, effects, voice, ambient) and output device selection.
- The reported-bug fixes, each as its own switch, grouped as "retail behaviour" / "fixed".
- Optional quality-of-life changes (e.g. difficulty or combat tuning) only as clearly labelled
  switches, so a default run is still TLC.

## Linux without a Linux machine

- **CI builds Linux** on a GitHub Actions `ubuntu` runner from step 1 on: compile, unit tests,
  headless tests (script host, serializers, replay without rendering). Breakage shows up per commit.
- **WSL2** on this Windows 11 machine can run the same headless build and tests locally without a VM.
- Real Linux play-testing (Vulkan on a GPU, audio, input) waits for a Linux install; until then the
  docs call Linux "builds, untested".

## Mods across the fork

| Mod kind | Carries over | Notes |
|---|---|---|
| Data: `.big`, defs, `.lev`/`.tng`/`.wad`, text, `.qst` | Yes | Same retail formats |
| FableForge output (levels, NPCs, quest cards, mod packs) | Yes | FableForge writes retail formats |
| Converted quest Lua / ForgeFSE Lua mods | Yes, via the FSE shim | SCRIPTING_REDESIGN "Compatibility" |
| Native hooks (ForgeFSE detour at `0xCDB355`, EgoCore DLL mods, byte patches) | **No** | They patch x86 addresses that do not exist in the fork; their features become engine code or script API |

## Checking the fork

- **Input record/replay** is the fork's first tool: record inputs and RNG seeds on one build, replay
  on another, compare the world state stream (hero position, quest states, script events) and
  sampled frames. Runs across retail, parity, modern-x86, and x64 builds.
- The existing autopilot / in-game runner, visual-parity screenshot tools, and save checkpoints
  (`adult_*_completed_*`) are the test corpus.
- Save compatibility: every checkpoint loads in the fork; fork saves load in retail until a format
  bump.

## Work that can start now

These help the current work and cost nothing later. They are tracked in
[ROADMAP](../ROADMAP.md) section (h).

- [x] Platform surface inventory: `tools/modern/platform_surface.py` →
  [PLATFORM_SURFACE](PLATFORM_SURFACE.md).
- [ ] `near_match` grade: similarity score in `verify_and_land.py`, dashboard column, rerun
  over existing `DIFFER` rows (many should reclassify).
- [ ] Header rule in the parity tree: no D3D9/DInput/OpenAL/DirectShow types in gameplay-facing
  headers; add a verifier warning.
- [x] Persisted types worklist: [PERSISTED_TYPES](PERSISTED_TYPES.md) (2026-10-16).
- [~] `IDirect3DDevice9` call-site census: static lower bound in [D3D9_DEVICE_CENSUS](D3D9_DEVICE_CENSUS.md).
- [x] Modern-compiler census (steps 1-2 readiness): every reconstructed TU through VS 2022 x86 and x64,
  [MODERN_COMPILE_CENSUS](MODERN_COMPILE_CENSUS.md) (2026-10-17).
- [ ] SCRIPTING_REDESIGN "Now" steps 1–3 on ForgeFSE.

## Open questions

- **Reuse FableForge `forgecore` for level/world formats?** (Recommendation 2026-10-17: yes.)
  `D:\Code\FableForge\libs\forgecore` is MIT C++ by the same owner. It already reads and
  writes `.lev`, `.tng`, `.wld`, `.wad`, `.stb` (with bake/validate), `.bwd`, `.qst`, game.bin
  defs, `.lut` and lipsync, and its LZO follows the same licence rule as `rebuild/modern/save`
  (clean-room MIT LZO1X, `lzo1x_999` only via an optional system liblzo2). Re-deriving those
  serializers here would duplicate months of validated work. The fork's own modules cover what
  forgecore does not: `save/` is byte-exact read+write of every body section (forgecore's save
  reader is read-only: HEADER plus hero stats), and `bank/` is a layout-preserving `.big` round
  trip with payload views. Proposed split: the fork links forgecore for level/world/def formats,
  and forgecore can adopt `save/` and `bank/`. The open part is how the fork tree pulls it in (git
  submodule, FetchContent of a tagged release, or copying a pinned snapshot).
- Thresholds for `near_match` (the ±10% / 90% defaults) after the first sweep over existing
  `DIFFER` rows.
- Whether to record the replay stream from retail itself (a small ForgeFSE-side recorder) or only
  from the parity build.
- Vulkan minimum version and D3D12 feature level (proposal: Vulkan 1.3, D3D12 FL 11_0 + DXIL SM 6.0).
- Save format bump policy once the fork adds state retail cannot read.
- ~~Where the game loop reads frame time~~ answered in [FRAME_TIMING](FRAME_TIMING.md): the world is
  already a fixed 15 Hz step with render interpolation; the remaining work is auditing render-time readers.
