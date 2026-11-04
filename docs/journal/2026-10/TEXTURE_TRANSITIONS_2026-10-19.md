# Oakvale texture transitions and view distance

## Objective and sequence

User's primary observation: while running through Oakvale, props abruptly switch
from low-detail textures to very sharp textures, with little visible intermediate
detail. Also investigate the softer right-hand weapon hilt in Discord.zip and
provide controllable draw/culling/fade distance improvements. Geometry detail and
texture detail must remain separate in the diagnosis.

1. Trace original retail request, allocation, upload, eviction and distance code,
   including the four bytes changed by the upstream 2K DLL.
2. Implement bounded, read-only runtime sampling of pool capacity/failures,
   requested/loading/current mip levels and renderer distance/preload fields.
3. Compare original and corrected 3,641-image preview using the same scratch save,
   profile and Oakvale route. Correlate visual changes with resident mip changes.
4. Implement only evidence-supported streaming changes and independently tunable
   draw/fade settings. Preserve vanilla defaults and reversible installation.
5. Validate image transitions, memory, timing, settings reapplication, and restore;
   prepare a private test package with explicit remaining limitations.

## Evidence gathered

- Extracted Desktop/Discord.zip to build/discord-intake-20261019-1720. Seven
  screenshots only; no source hilt PNG or recipient bank. Right-hand hilt looks
  softer; flat-colour weapon screenshots cannot demonstrate resolution retention.
- Standalone importer sets imageSize=true/maxDimension=2048. PNG entry rebuilding
  generates the lower mip chain. TextureSizeFix adjusts UI geometry, not sampling.
- Existing parity source and fresh retail disassembly agree: BeginLoading at
  009FDE80 tries the requested resolution first and halves dimensions only after
  allocation failure. It does not require sequential one-level promotions.
- FreeTextureMemory at 009FE460 reduces unused textures by one mip after the bank
  reduction delay; this is distinct from ordinary sampler mip filtering.
- Original CGraphicDataBank initialization at 009FD4E0 sets reduction delay=3,
  pointer-validity delay=1 and unload delay=30 frames. Live overrides not yet read.
- Fresh decompile of static-prop PreloadPrimitive at 00BA4C10 calculates texture
  mip requests from submesh distance, radius/scale, texture frame width and
  renderer+0xB0. Renderer+0xB4/+0xB8 are preload tolerance/max size.
- Upstream 2K DLL patches pool construction, not simply a size validation guard.
  Exact changes and native replay are being measured; do not infer capacity from
  its name or from the 16 MiB video preset alone.
- DrawDistanceMultiplier/Minimum/Maximum, RepeatedMeshDrawDistanceFactor and
  ZSprite distances are applied by SetDrawDistanceQuality. Local-detail fade
  modifiers are updated separately. Increasing visibility alone does not ensure
  earlier high-resolution texture residency.

## Artifacts and boundaries

Fresh read-only Ghidra exports and original PE disassembly:
build/texture-popin-re-20261019. The first strict export rejected an uncarved
function; its log is retained. Follow-up used read-only temporary function creation.
No Ghidra project changes saved or public binaries changed. Runtime work below
uses only the exclusively leased texture scratch installation.
Corrected balanced bank verification exists in texture checkout's
build/texture-ai-expanded-qc-pack-20261018b. Reverify inputs before runtime use.

The softer hilt cannot be attributed to a specific source-image or import fault
without its actual input/output pixels. Engine-side mechanisms can be established
independently with the local corpus and retail execution.

## Implemented controls and validation, continued

Texture checkout: `C:/Users/Cornelio/AppData/Local/FableForgeDevelopment/texture-project-20261014`.

- `tools/prepare_texture_distance_trial.py`: forgecore-backed authoring of
  distance multipliers/maxima, static mesh LOD error tolerance, and an optional
  nonzero high-resolution pool budget floor. Controls are independent. Reopens
  every one of 14,764 definitions independently and requires all unrequested
  payload bytes and identities to match; names.bin stays byte-identical.
- Distance scale 1.5 multiplies the named native distance fields, not a guaranteed
  universal world-space culling radius. Class-specific factors can compound.
- Mesh distance scale 1.5 divides `LODErrorTolerance` by 1.5. Static-mesh preload
  at BA4C10 compares distance/(radius*scale*projectedVirtualScreen)*error against
  authored LOD thresholds. Highest's zero error stays zero. Character LOD is
  separate and unchanged. This delays switches; it does not synthesize missing
  mesh detail or blend mesh silhouettes.
- `tools/prepare_texture_transition_fixture.py`: freezes definition/DLL/config
  inputs with SHA256 and emits a command for the existing leased, restoring
  harness. The harness accepts only the exact definition pair and private
  streaming DLL/config pair, verifies originals, and restores hashes and saves.
- `tools/texture-runtime/texture_retention_probe.cpp` and `retention_patch.hpp`:
  private scratch-only startup experiment, independently configurable reduction
  delay, unload delay, and request multiplier. Defaults preserve stock behavior.
  Bounds: reduction3..60 frames, unload30..240, request100..200 percent.
  Exact instruction verification, transactional writes, page restoration,
  instruction-cache flush, and refusal on a different host/site.
- Request multiplier changes renderer constructor store B35CE9 only. FrameStart
  B32660 uses `min(screenWidth,640)*cameraProjection*MipmapQuality` for requests;
  mesh projection remains `640*cameraProjection`. Native replay at widths320,
  640,1024,2560 and multipliers1,1.5,2 confirms these are separate calculations.
- Retention constructor alone is insufficient: FrameEnd B3B870 restores3 via
  PUSH3 at B3B8AE and setter9FD100, which updates bank26C and resource19C.
  Latest probe also changes that specific FrameEnd argument. Temporary0/1 delays
  used by weather, resource resizing and rendering remain available. Ghidra's
  inherited name for9FD100 is wrong; exact bytes and callers establish behavior.
- `test_retention_hardware`: applies and executes the same C++-generated hooks on
  real x86 hardware, checks unchanged other state/EAX, duplicate refusal and page
  protection. Passed. Transaction/backend tests passed before the final clean
  rebuild; rerun with final build as required.
- `tools/texture_transition_native_probe.py` in FableTLC: real retail x86 replay
  with bounded external stubs:6 pool constructions,36 requested-mip allocation
  decisions,3 retention initializations,3 FrameEnd resets,12 request calculations.
  Native request test executes the real `_ftol2`. Evidence
  `build/texture-popin-re-20261019/native-probe-streaming.json`.

## Runtime observations and rejected attempts

All runtime directories below are in the texture checkout's `build/`.

1. `texture-transitions-ai-20261019a`: corrected3,641-image bank,300 residency
   samples,17-step route. Successful driver/helper and graceful close; scratch
   and563 original save files restored. Live settings were **Normal**, not Highest:
   draw1, static-mesh error2, virtual screen640, request scale552.0677. Main bank
   reduction3/unload30. Largest populated pool9 contains35 slots, all35 present
   in the first sampled frame; zero allocation failures does not prove no eviction.
   Coherent ordinary-texture analysis:77 route transitions,3 promotions spanning
   at least two levels.16 inconsistent frame records excluded. OV_FIREPLACE
   (id3223) exhibited0->2->0 twice,1024->256->1024.
2. `texture-transitions-stock-20261019a`: original bank, same save/route.
   Route/helper and300 samples completed, but final capture failed when the game
   window was unavailable; cleanup/restoration succeeded. Partial evidence,
   not a fully passing control. Do not compare raw counts as a visual score.
3. `texture-transitions-retention-20261019a`: initial integration omitted required
   LocalCodeMemory.prepare; refused/fatal startup before gameplay. Corrected,
   covered with actual hardware patch-application test. No benefit claim.
4. `texture-transitions-retention-20261019b`: patch applied; capture failed before
   gameplay. Gamewin's minimized-window size filter prevented its restore path.
5. `texture-transitions-retention-20261019c`: same older probe, gameplay/route,
   300 samples, graceful exit0, scratch/saves restored. Live reduction stayed3
   because of the FrameEnd reset; unload120 applied.69 coherent route transitions,
   still3 multi-level promotions. This only tests longer unload retention, not
   the corrected reduction delay. Peak observed RSS309,321,728 bytes. Temporary
   delay1/1 states also observed. Matched route screenshots were inspected:
   similar position, small position/NPC differences; not pixel-aligned captures.
6. `texture-transitions-streaming-20261019a`: corrected FrameEnd plus request2,
   reduce30/unload120. Applied log, black startup captures, then foreground guard
   failed before gameplay. Withheld; neither efficacy nor engine regression proven.
7. `texture-transitions-distance-mesh-20261019a` and `...19b`: no streaming DLL,
   same foreground failure before gameplay. All files/saves restored. This
   establishes that the foreground failure is not exclusive to the DLL candidate.

Read-only desktop check found Windows Search (`SearchHost`, PID7052) holding the
foreground on the Default desktop. Asked user to dismiss the Search panel; offline
work continues. Do not send route input to Search or weaken foreground validation.

## Interpretation and remaining acceptance

Importer mip generation, runtime texture residency, mesh selection and object
culling are separate mechanisms. UploadFrame replaces resident surfaces without
a blend; BeginLoading can promote directly to the requested highest mip. Higher
draw distance alone cannot remove this. Increasing request distance while a pool
is full can increase churn. Latest optional memory floor uses at most the stock
Highest definition value16384, leaves Low's zero budget untouched, and must be
validated live: after the2K pool-construction patch those KiB are not actual VRAM.

Remaining: complete definitions-only live field/route acceptance, validate earlier
requests with sufficient pool slots and stock retention, examine corrected delay
separately if useful, assess rock silhouettes on a wider route, and record memory/
visual limits. No frame-time/GPU measurement or complete pop-in elimination claim.
Terrain settings/code have not been changed in this pass. User noted more consistent
ground during the stock-control run; cannot attribute that to a new terrain fix.

Final offline checkpoint while desktop blocked: final C++ hardware, transactional
patch and local page-protection tests3/3 pass. Python syntax checks pass. A temporary
AttachThreadInput activation attempt did not resolve Search ownership and was
removed; minimized-window recognition remains. `desktop_preflight` now detects
foreground SearchHost before staging, with no input sent to that process.

Pending commands are frozen in texture build:
`texture-distance-mesh-fixture-20261019a/command.json` (choose a fresh output name),
and `texture-request-pool-fixture-20261019a/command.json` (earlier requests2x,
stock retention3/30, nonzero high pool budget floor16384). The latter dry run
verifies all pins and reports the Search overlay. No installed-game application
or release should be inferred from these private fixtures.


## Next session starts here: user video review (2026-10-19 evening)

The user requested a handoff so the next session begins by examining their recorded
Oakvale test video, then investigating whether additional RAM/address-space capacity
is feasible. The video has not yet been supplied or reviewed. Locate the supplied
recording (ask for its location only if needed), watch the full clip, then extract
before/after frames and timestamps for the clearest transitions. Separate texture
mip/residency changes, rock mesh/silhouette LOD switches, object culling/fades and
possible recording artifacts. Use visible landmarks to define a repeatable route;
the user previously had no specific rock landmark. Do not launch another automated
route before reviewing this evidence, or claim that the video confirms a fix unseen.

### Configuration of the user's recorded test

Checkout: `C:/Users/Cornelio/AppData/Local/FableForgeDevelopment/texture-project-20261014`.
Successful manual run: `build/texture-user-test-20261019b` (command and guardian
receipt in `build/texture-user-test-launch-20261019b`). Frozen definitions fixture:
`build/texture-distance-mesh-fixture-20261019a/fixture.json`. Native distance factors
scaled 1.5 and static mesh LOD error tolerance divided by 1.5; these do not imply
one universal 50% world-distance increase. Corrected upscaled bank SHA256
`733e4af19c3a48b7375315647e56e107ef329227c66a7bb2575d183a50de4a8b`.
2K patch verified in live memory. No experimental retention/request DLL in this run,
no texture-pool budget increase and no terrain changes. Profile fixture was 0aa;
actual user-selected profile/settings/route were not instrumented, so verify from
the recording rather than assuming the suggested AutoSave route was followed.

The manual-session option disables automated navigation and timed closure and
waits for the user to exit. First attempt `texture-user-test-20261019a` verified
patch bytes but never obtained a visible window and was restored. Retry explicitly
sets FSE launcher STARTUPINFO to SW_SHOWNORMAL, allowing the interactive game to be
visible under its hidden guardian. Retry produced visible Fable window PID12864
and `launch/manual-session-ready.json`; user-controlled session then ended normally.
`launch/launch-report.json`: exit0, manualSessionEnded=true; peak sampled RSS
357,068,800 bytes (~340.5 MiB), VMS377,769,984 bytes (~360.3 MiB).
Harness trial report returncode0, saves restored (563 original files), new profiles
preserved in evidence. Runtime lease absent and no Fable process at handoff check.
Recheck live ownership before any future launch. Installed Steam game untouched.

### RAM feasibility investigation, after video review

Read-only PE inspection of scratch Fable.exe: machine0x14c (x86), characteristics
0x10f, IMAGE_FILE_LARGE_ADDRESS_AWARE bit0x20 absent; launch receipt independently
reports largeAddressAware=false. Original EXE SHA256
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.
No executable flag or memory patch was applied in this session.

1. Establish the operating-system/address-space configuration and consult primary
   platform documentation for the actual non-LAA/LAA limits. Assess an isolated,
   reversible Large Address Aware experiment, including pointer assumptions,
   loader/mod compatibility, executable hash guards and restoration. Never silently
   weaken the existing exact-host verification to accommodate an altered EXE.
2. Measure committed/private bytes, virtual address usage/fragmentation, allocation
   failures and texture-pool occupancy/eviction along the video route. Sampled RSS
   and VMS alone do not establish address-space exhaustion or a safe new limit.
   This run's modest observed usage does not demonstrate hitting the process ceiling.
3. Keep process address-space capacity distinct from engine heap/pool budgets,
   GPU residency and the BIG archive file-size limit. Prior Normal-profile runtime
   had 35 occupied high-resolution pool slots; increasing address-space capacity
   alone would not raise that engine budget. Trace ResizeHiresTextureMemory and
   the 2K patch's pool sizing before choosing an effective bounded budget.
4. Test the already frozen request/pool experiment separately from distance/mesh
   and LAA: `build/texture-request-pool-fixture-20261019a/command.json` uses
   request200%, stock retention3/30 and nonzero budget floor16384. Confirm actual
   live pool sizes/counts; nominal KiB do not directly represent allocation after
   the 2K constructor patch. Earlier requests may increase churn in a full pool.
5. Compare the same route/settings with one variable changed at a time; record
   visual transition distances, frame-time effects, memory and stability, and
   retain restoration receipts. No claim of eliminated pop-in or release readiness.

Full native evidence and prior trial limitations:
`D:/Documents/FableTLC/docs/journal/2026-10/TEXTURE_TRANSITIONS_2026-10-19.md`.
Older next-step and blocked-desktop notes below are historical; this checkpoint
supersedes them. Begin with the video, not the earlier Search-overlay retry plan.

