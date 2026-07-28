# Runnable reconstruction

This dashboard tracks the shortest honest path from independently verified
functions to a reconstructed executable. It does not count a diagnostic
bootstrap as a running game.

## Boot-path progress

| Measure | Proven | Scope |
|---|---:|---|
| GFMain direct-call sites | 49/257 (19.07%) | All ten call clusters; repeated call sites count separately |
| Callable authored GFMain phases | 2/10 (20.00%) | Integration checkpoints, not retail GFMain byte parity |
| Current Phase 3 direct calls | 29/34 (85.29%) | Settings, persistence, and IME cluster |

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
`FRONTEND_BACKDROP_01` and the two ARGB title halves through the recovered
Lionhead-LZO/DXT tooling, crops them to their authored frames, joins the title
at its retail `+256` child offset, and embeds them in the executable.
The authored project image remains a dependency-safe fallback. This is the
first genuine game-derived title frame in the reconstructed process. An authored
D3D9 bridge uploads the backdrop and alpha title as managed textures and places
the title at the decoded UI parent coordinate `(70,30)`. The 640x480 frontend
view is stretched independently over the client width and height, as observed
in the retail 1280x720 reference; the artwork's own dark edges remain intact.
The build also
resolves the press-start widget's `CDefString` font reference to
`ENG_ARIAL_24`, decodes that font's embedded retail TGA atlas and glyph
metrics from fonts.big, and draws the localized line centered at its decoded
`(320,240)` coordinate at the retail two-thirds glyph scale with its dark edge.
The legal widget now uses the PC streaming-font
metadata and shared 25.9 MB `GlyphData`: its packed Unicode lookup, LZO metric
chunks, and 32x32/64x64 alpha blocks render all 144 localized characters,
including the retail copyright symbol, in a 420-pixel wrapped block centered
at `(320,340)`. ASCII retains the static atlas's retail advances while the
streaming bank supplies Unicode fallback, giving the reference three-line
wrap. PBGRA title composition is unpremultiplied before straight-alpha D3D9
blending. The default backdrop now decodes and stitches all 42 retail
forest/sunbeam tiles into four-frame and three-frame D3D texture sheets.
The press-start root's graphic-362 `MOUSE_POINTER_SPRITE_FE` is decoded into
the active 32x32 cursor with its retail top-left hotspot.
Non-repeating randomized choices crossfade with the widget tree's decoded
8/8/8/2-second forest and 2-second sunbeam durations underneath the title and
text; a changing frame-hash smoke gates the live animation. Retail-shaped triangle
records pass through the recovered Render2D batch planner, whose triangle-list
flush is executed on a real Win32 D3D9 device; GDI is retained only as a
failure fallback. Texture binding now executes the exact recovered 79-byte
`CRenderManagerCore::AttachTextureToStage` body, including its cache and
active-stage bookkeeping. The exact 167-byte
`CRenderStateManager::RealiseRenderState` body also drains ten queued render,
texture-stage, and sampler states for the live draw. Other dependency dispatch
now includes the recovered 11-byte `CRenderManagerCore::SetAWindow` ownership
hop and direct relocation-matched 112-byte `CDisplayManager::SetViewport`
float conversion. Its endpoint is now the full relocation-matched 479-byte
integer overload, including clamping, retail viewport/cache updates, the real
D3D9 call, exact one-byte `$E2`, and exact 11-byte post-viewport shader
notification. The exact 79-byte
`Render2DDrawList::CopyBlock` full-clear path then resets the live 0x20-byte
vertex queue. Exact 98-byte `CTexture::operator=` and 34-byte
`CTexture::Uninitialise` bodies now execute the temporary/current/candidate
wrapper lifetime events. The relocation-matched 74-byte preallocated initializer
and behavior-gated `CalcByteLength` null path create the temporary wrapper.
Calc's nonnull path is fully link-resolved through relocation-matched
`CPixelFormat::Initialise` and `GetColourDepth` bodies plus the exact
1,692-byte retail pixel-format table; the temporary visual wrapper remains
null by design. The
exact 121-byte `RestoreCaptureBlock` closes the compact capture sentinel.
Remaining dispatch is still authored rather than the
complete
Lionhead coordinator, runtime archive loader, or game loop.

The authored checkpoint now continues through the first retail frontend
interaction. Left-button release follows
`UI_FRONTEND_BUTTON_INVISIBLE`'s compiled action 229 into
`UI_FRONTEND_MAIN_MENU`. That root replaces the forest layers with the decoded
four-frame/three-sunbeam coastal animation while retaining the retail title.
Its seven `ENG_ARIAL_24` rows, exact y offsets, 400/280-pixel button widths,
and `TS_BUTTON_L/M/R` selection ornaments come from frontend.bin and
frontend.big. Mouse motion maps back to the 640x480 design view and moves the
selection through all seven compiled hit regions. The `-VerifyMainMenu` smoke
gates press-start-to-menu and Continue-to-Quit hover as three distinct hashes.

Options and Quit now continue beyond hover. Recovered
`CFrontEndManager::Action` mappings route action 297 to
`UI_FRONTEND_OPTIONS_SUB_MENU` and action 314 to
`UI_FRONTEND_QUIT_PROMPT`. Options uses its exact four-row list, forest
background, title/rule, and Back control; Quit uses its background-free black
composition and exact localized question/No/Yes controls. Back and No execute
recovered action 86, while Yes executes the checkpoint equivalent of retail
action 296's main-component quit request.

The next-level dispatcher is live: Gameplay action
9 maps to `UI_FRONTEND_OPTIONS_MENU`, Audio action 12 to
`UI_FRONTEND_AUDIO_OPTIONS_MENU`, Video action 13 to
`UI_FRONTEND_SCREEN_VIDEO_OPTIONS_PC`, and Redefine Keys action 283 to
`UI_FRONTEND_SCREEN_REDEFINE_KEYS_PC`. Their retail title rules, localized
text, slider/arrow/slot sprites, Apply/Cancel/Defaults or Reset helpers, and
forest/coastal choices render in the runnable. Video is seeded from the
compiled defaults and Redefine from the shipped WASD control-scheme records;
Gameplay, Audio, and Video values mutate live with Cancel/Apply/Defaults
transactions. Redefine rows use their exact 26-pixel list spacing and recovered
`CKeyRedefiner::OnHovered` state 3/state 4 transitions to switch the paired
retail slot art between ON and OFF. Their generated children use the compiled
`ENG_ARIAL_12` font and `(0,3)`/`(380,3)` text offsets. The build prefers the
untouched installed `frontend.big`; the intentionally gold-tinted
`work/ui_proto/art` experiment is only a fallback.

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
Windowed presentation now handles `WM_SIZE` by resetting the D3D9 backbuffer
to the new client dimensions, restoring the tracked-state metadata, and
redrawing. `smoke_visual_checkpoint.ps1 -VerifyMaximizedScale` maximizes the
window and compares a 30-point rendered sample grid with the decoded retail
backdrop, preventing the old fixed-1280x720 clipping/stretch regression.

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
Phase 3 has 29 of 34 direct calls proven. The first correction replaces
a false donor `GetActionName` label with a TLC-specific no-argument
default-language factory. Its readable 19-byte body constructs `"English"`
in the fastcall hidden return slot and now has a real lifetime fixture.
The next corrected call is `CCharString::ToWideString`: its 45-byte body
and the 13-byte conversion factory plus 66-byte counted-storage copy
constructor all relocation-match and pass focused ownership fixtures.
The shared string/profile targets, two seven-byte text-alignment setters,
and two folded seven-byte CBase vtable restores remain proven as well. The
two corrected wide-string concatenation overloads at `0x0099BF30` and
`0x0099BFF0` are now 177-byte and 134-byte relocation-normalized matches with
linked hidden-result/refcount behavior fixtures.
The optional settings-file branch now continues through exact
`CCharString::LoadFromFile @ 0x0099F2E0`, `CStringParser @ 0x00404720`, and
the text `CPersistContext` constructor at `0x009BADD0`.

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

The retail press-start/main-menu interaction gate posts the decoded
left-button release, requires the coastal screen transition, then hovers Quit
through the compiled list geometry:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -VerifyMainMenu -TimeoutSeconds 12
```

The full first-menu branch gate additionally enters Options, proves a Gameplay
control mutates live, Cancel restores its entry value, Apply survives re-entry,
and Defaults restores the recovered profile seed. It then visits every detail
screen, proves the first Redefine row enters recovered hover state 3, returns
through Back, enters Quit, returns through No, and re-enters to confirm Yes
exits cleanly:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -VerifySubscreens -TimeoutSeconds 25
```

The retail text remains the default parity path. An opt-in joke sheet keeps
the same recovered geometry, assets, font metrics, actions, and hitboxes while
replacing only the seven main-menu strings. Its normal launch now includes the
recovered Lionhead, Microsoft, and intro movies before revealing the humorous
menu:

```powershell
rebuild/build/bootstrap-Release/FableTLC-Reconstruction-VisualCheckpoint.exe `
  --buff-jesus

powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -BuffJesus -VerifyMainMenu -TimeoutSeconds 20
```

The smoke command adds `--skip-boot-videos` internally so it can exercise the
menu immediately. Use that switch directly when you want the same fast
developer launch. The end-to-end boot-plus-BuffJesus menu gate is:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -BuffJesus -RetailVideo -Movie boot -VerifyBootToFrontend -VerifyMainMenu `
  -OriginalVideo -TimeoutSeconds 35
```

The first changing retail-video checkpoint remains opt-in for the unmodified
retail-text path:

```powershell
rebuild/build/bootstrap-Release/FableTLC-Reconstruction-VisualCheckpoint.exe --retail-video
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 -RetailVideo
```

The executable resolves the read-only Steam install and follows the recovered
boot order: `lionhead_logo.wmv`, `microsoft_logo.wmv`, then
`intro_comp.wmv`. It advances on DirectShow end-of-stream events and reveals
the static frontend checkpoint after the intro completes. Escape skips only
the current movie and advances to the next one, matching the retail interaction.
The parent D3D9 surface remains black while the movie graph starts and plays,
so the title artwork cannot flash before the Lionhead movie. The frontend is
presented only after the final movie completes or is skipped.
The smoke gate
requires the playback graph to enter the running state and proves that two
captured window frames 600 ms apart differ. This stronger gate verifies both
pre-intro transitions:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -RetailVideo -Movie boot -VerifyBootSequence -TimeoutSeconds 45
```

The skip gate sends Escape to movies 1 and 2 and requires movie 3 to start:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -RetailVideo -Movie boot -VerifyEscapeSkip -TimeoutSeconds 15
```

The complete visible handoff gate skips all three, requires the frontend
checkpoint, verifies that the DirectShow child window was released, and checks
the revealed frame for real color variation:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -RetailVideo -Movie boot -VerifyBootToFrontend -OriginalVideo `
  -TimeoutSeconds 25
```

`--retail-video=microsoft`, `--retail-video=lionhead`,
`--retail-video=attract`, and `--retail-video=intro` select one shipped movie.
The `CVideoSys` and `CMovie` method map, ownership, playback state, critical
section, frame event, and texture handoff are now recovered and documented in
`docs/VIDEO_SYSTEM_RE.md`. Their verified binary layouts compile from
`rebuild/include/fable_video_system.h`. The live compatibility path still uses
a DirectShow child window for presentation. The separate native graph probe
already receives every decoded sample into a real managed D3D9 texture without
a child window; reconstructed `CTexture` ownership plus native `CMovie::Draw`
remain to make that path visible.
The first seven native handoff/state leaves are promoted with exact retail object
parity and behavior gates: `CMovie::SetMovie @ 0x00548510`,
`CMovie::IsPlaying @ 0x00548520`,
`CVideoSys::WaitForState @ 0x00A3B0F0`,
`CVideoSys::AttemptToPlay @ 0x00A3B1A0`,
`CVideoSys::Pause @ 0x00A3B1C0`,
`CVideoSys::Stop @ 0x00A3B1F0`, and
`CVideoSys::GetTexture @ 0x00A3B320`.

### Optional AI-enhanced boot movies

The shipped startup movies are only 640 pixels wide. A reproducible, opt-in
preprocessing path now generates 2x copies with the video-specific
`realesr-animevideov3` model through portable Video2X/NCNN/Vulkan:

```powershell
rebuild/upscale_retail_videos.ps1 -Movie boot -InstallVideo2X
```

The script pins Video2X 6.4.0 and verifies its downloaded archive against the
recorded SHA-256 before extraction. It streams frames through the GPU, retains
the original frame count and stereo WMA audio, encodes a DirectShow-compatible
WMV2 result at 8 Mbit/s, and validates dimensions, video-frame count, and
audio-packet count before atomically publishing each file under the ignored
`rebuild/build/bootstrap-Release/upscaled-video/` cache. Retail files are never
modified.

`--retail-video-upscaled` opts into each completed enhanced file and falls
back independently to its retail source when absent. The title includes
`AI 2x` when an enhanced file is live. Untouched retail files remain the
default parity source. The focused selection gate is:

```powershell
powershell -ExecutionPolicy Bypass -File rebuild/smoke_visual_checkpoint.ps1 `
  -RetailVideo -Movie lionhead -VerifyUpscaled -TimeoutSeconds 15
```

This is a presentation enhancement, not a retail-parity claim. Super-resolution
can infer plausible edge detail and may introduce temporal shimmer or altered
fine texture. The untouched-file override remains the accuracy oracle.

### Planned AI-enhanced menu/UI assets

The equivalent UI path is documented but not implemented yet. It will use an
ignored `upscaled-ui/` cache and explicit `--retail-ui-upscaled` selection,
with independent fallback for every asset. Backdrops can be enhanced directly;
transparent sprites must separate/recombine alpha, tiled backgrounds must be
stitched before inference, and text should be rerendered from decoded retail
glyph metrics where possible instead of allowing AI to reshape letters.

See `docs/UI_UPSCALE_PLAN.md` for the proposed command, cache manifests,
alpha/seam validation gates, exact first asset set, and next-session resume
checklist.

Stages 2 and 3 use explicit integration boundaries and are not claimed as a
retail-matching GFMain. The visual checkpoint can now present one build-time
extracted retail asset through D3D9 and recovered Render2D batching/lifecycle
seams, but it does **not** yet execute the complete Lionhead parent coordinator,
initialize the complete engine-service
graph, load archives at runtime, run the native retail movie subsystem, or
enter the game loop.

## Remaining accurate-startup closure

The visual checkpoint is an evidence-backed checkpoint, not yet the complete
retail startup. The remaining work, in execution order, is:

1. **Close GFMain Phases 3-10.** Only authored Phases 1 and 2 are callable.
   The unresolved path covers settings/persistence/IME, the root child
   hierarchy, retail banks and INI files, save paths/fonts/display resources,
   command-line/window policy, EULA/hardware configuration, engine primitive
   assembly, and the final GFInitialise/launch/error/cleanup branches.
2. **Replace integration-owned startup objects.** GFInitialise and the visible
   progress leaf execute, but the root engine, display, progress-resource, and
   service-singleton graph are still controlled boundary objects rather than
   the retail ownership graph.
3. **Replace the post-movie service boundaries.** The visual executable now
   crosses the recovered `CNewFrontendGameComponent::Run @ 0x0042EC7C` order:
   runtime opens of `frontend.big`, localized `text.big`, and `shaders.big`,
   then `Init2`, `InitialiseEngine`, frontend construction, first clear/swap,
   and `CFrontEndManager::ChangeStateFirstTime`. A focused fixture proves the
   nine-step bank path, six-step no-bank path, and failure short-circuiting.
   The individual bank/engine/frontend actions remain authored boundaries.
4. **Enter the real frontend loop.** The retail loop performs system update,
   input, update, interpolation, and draw; it also clears stale input after
   attract playback and owns the title/menu transition into either the main
   game or the legacy frontend component. The current post-movie image is a
   static build-time extraction, not this interactive loop.
5. **Replace the movie presentation seam.** Playback order, completion,
   ownership, state, synchronization, and Escape behavior are recovered, but
   frames still use a DirectShow child window. Native parity requires the
   recovered UI texture copy and `CMovie::Draw` sprite submission. The exact
   `0x180` renderer subclass now negotiates the untouched Lionhead graph and
   receives all 419 changing RGB24 frames without a child window; its
   `DoRenderSample` path locks, converts, and unlocks a real managed
   A8R8G8B8 `IDirect3DTexture9`, then publishes it and signals the recovered
   event. The probe consumes that auto-reset event and copies the published
   texture into a second managed D3D9 texture under the video critical section,
   matching the synchronization/copy prefix of `CMovie::Draw`. Reconstructed
   `CTexture` wrapping, sprite submission, and visible-path replacement are the
   remaining seam. All four helpers directly under
   `CTexture::CopyFromTexture` are now exact retail matches; its coordinator
   and the `CMovie::Draw` sprite tail remain.
6. **Close renderer/resource lifecycle.** Runtime archive/texture ownership,
   the complete `Render2DDrawList` parent, input/audio/localization services,
   device-reset/error paths, and orderly frontend shutdown are not yet live.

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
| 3 | `0x00402668`-`0x0040284E` | settings, persistence, and IME | 34 | 23 | 29 | FableGetDefaultLanguageName_00415530; PathExists; LoadFromFile; CPersistContext; LoadIMESettings |
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
| 3 | `0x00402724` | `0x0099F2E0` | CCharString::LoadFromFile | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_LoadFromFile_0099f2e0.cpp) |
| 3 | `0x00402735` | `0x0099EBF0` | CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Constructor_0099ebf0.cpp) |
| 3 | `0x00402745` | `0x00404720` | CStringParser | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/40/CStringParser_Constructor_00404720.cpp) |
| 3 | `0x0040274E` | `0x0099EAE0` | ~CCharString | `RELOCATION_MATCH` | [source](../rebuild/src/compiled/00/99/CCharString_Destructor_0099eae0.cpp) |
| 3 | `0x00402765` | `0x009BADD0` | CPersistContext | `MATCH` | [source](../rebuild/src/compiled/00/9b/CPersistContext_TextConstructor_009badd0.cpp) |
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
