# Retail video system recovery

*Recovered from the PC `Fable.exe`, with names cross-checked against the
`Ego_r.pdb` donor. Addresses are retail virtual addresses.*

## Startup sequence

`CNewFrontendGameComponent::Run @ 0x0042EC7C` constructs three movie records
and plays them in this order before opening the frontend banks:

1. `Data\Video\lionhead_logo.wmv`
2. `Data\Video\microsoft_logo.wmv`
3. `Data\Video\intro_comp.wmv`

The donor still spells the assets `.xmv`; the PC retail path and shipped files
are `.wmv`. The reconstructed visual checkpoint now follows this order and
advances on DirectShow completion events. Escape completes only the active
movie and advances to the next record; the compatibility child video window
drains keyboard messages to the parent checkpoint window.
`fable_attract_english.wmv` belongs to later frontend attract mode, not the
initial boot chain.

## `CVideoSys`

`CVideoSys` is a `0x8C`-byte DirectShow owner. Its recovered fields are:

| Offset | Meaning |
| --- | --- |
| `+0x00` | `IGraphBuilder*` |
| `+0x04` | `IMediaControl*` |
| `+0x08` | `IMediaPosition*` |
| `+0x0C` | queried graph interface |
| `+0x10` | `IMediaEvent*` |
| `+0x14` | `IBasicAudio*` |
| `+0x18`, `+0x1C` | additional graph interfaces |
| `+0x20` | `CTextureRenderer*` |
| `+0x24` | loop/restart policy used by `Process` |
| `+0x28` | playback state (`1` playing, `2` paused, `3` stopped) |
| `+0x30` | `CMovieSubtitles` |
| `+0x64` | `CRITICAL_SECTION` guarding decoded texture transfer |
| `+0x7C` | frame-ready event handle |
| `+0x80` | decoder/render-target texture |
| `+0x84`, `+0x88` | decoded height and width |

`Init @ 0x00A3B9D0` creates the filter graph, installs a custom
`CTextureRenderer`, renders `.wmv`/`.asf` files directly, queries the control,
position, event, and audio interfaces, and creates the critical section plus
frame event. `Play`, `Pause`, and `Stop` wait for the corresponding DirectShow
filter state. `Process` drains media events; completion either restarts or
stops according to `+0x24`. `IsFinished` treats only playing/paused as active
and stops once progress reaches the retail near-one threshold. `Close` releases
all graph interfaces and destroys the synchronization objects.

The full method map is preserved in
`ghidra_out/labels_video_system_recovery.tsv`.
The verified `0x8C` `CVideoSys`, `0x180` `CTextureRenderer`, and recovered
`CMovie` tail are encoded as VC7.1-compatible layout views and static
assertions in `rebuild/include/fable_video_system.h`; the compatibility player
compiles that header on every visual-checkpoint build.

## `CMovie`

`NUISystem::CMovie` is the UI component that owns the logical movie lifecycle;
the actual decoder is the process-wide `CVideoSys* @ 0x013B8ABC`.

- `ChangeState(3) @ 0x00547C90` lazily constructs the `0x8C`-byte
  `CVideoSys`, closes any previous active movie, resolves the configured path,
  calls `CVideoSys::Init`, reads the decoded dimensions, and creates the UI
  texture.
- `Update @ 0x00547E40` calls `AttemptToPlay` once, then `Process`. When the
  component leaves its playing UI state, it closes and deletes the singleton
  video system and releases textures.
- `Draw @ 0x00547F60` waits up to 100 ms on the frame event, enters the
  `CVideoSys +0x64` critical section, copies the decoder texture into the UI
  texture, and submits a `CEnginePrimitive2DSprite`.
- `SetMovie @ 0x00548510` assigns the filename at `CMovie +0x174`;
  `IsPlaying @ 0x00548520` returns the byte at `+0x15C`.
- The destructor performs the same graph/texture teardown when the instance is
  the active global movie.

The current reconstructed checkpoint still uses a compatibility DirectShow
child window rather than the recovered texture-renderer/D3D upload path. Its
ownership and end-of-stream sequencing now follow the recovered contracts; the
remaining integration step is to implement the recovered `CTextureRenderer`
contract and feed its decoded texture through recovered `CMovie::Draw`.

An optional generated cache may supply 2x Real-ESRGAN-enhanced WMV copies to
this compatibility layer. It preserves filenames, frame counts, timing, and
audio closely enough for the same graph and sequence contracts, but it is not
retail evidence. Untouched shipped files remain the default parity source;
`--retail-video-upscaled` opts into completed cache entries.

## `CTextureRenderer`

The custom DirectShow renderer seam is now bounded and named too:

| Address | Method | Recovered behavior |
| --- | --- | --- |
| `0x00A3B510` | constructor | Builds the DirectShow base renderer, stores the caller texture at `+0x164`, and records whether it owns a lazily allocated texture. |
| `0x00A3B590` | `CheckMediaType` | Accepts the expected video major/subtype/formattype GUID combination and rejects other media types. |
| `0x00A3B5F0` | `SetMediaType` | Reads `VIDEOINFOHEADER` width/absolute height and stride, creates or validates the `CTexture`, and accepts decoded formats `0x15` and `0x19`. |
| `0x00A3B730` | `DoRenderSample` | Enters `CVideoSys +0x64`, locks the media sample and texture, converts 24-bit source rows to either 32-bit RGBA or RGB565, publishes the texture at `CVideoSys +0x80`, signals `+0x7C`, and leaves the critical section. |
| `0x00A3B970` | destructor | Releases an internally allocated texture, then tears down the DirectShow base renderer. |

This closes the reverse-engineering gap around the native frame handoff. The
RGB24-to-A8R8G8B8 and RGB24-to-A1R5G5B5 inner loops from
`DoRenderSample` now compile as `FableConvertVideoRgb24Frame`; the visual
behavior fixture proves both formats, source/destination row pitches, alpha,
and untouched padding.

The base-renderer dependency is resolved too. Microsoft’s MIT-licensed
DirectShow base classes at pinned Windows-classic-samples commit
`77f217b3f89d4dac7864a62cc91ff7b569f26a50` compile cleanly with the
project’s VC7.1 + Windows SDK toolchain. The compiled
`CBaseVideoRenderer` is exactly `0x160` bytes—the exact prefix extent observed
before the first `CTextureRenderer` field at `+0x160` in retail.
`rebuild/build_directshow_baseclasses.ps1 -InstallSource` reproduces the
31-source static library and fails unless that layout invariant holds. A real
VC7.1 subclass with the recovered fields at `+0x160` through `+0x17C`
constructs successfully and is exactly `0x180` bytes, matching retail’s
allocation exactly.

The same subclass now runs as an actual DirectShow renderer filter.
`directshow_texture_renderer_graph.cpp` installs it before `RenderFile`, uses
the recovered media-type rules and row-pitch calculation, and receives decoded
samples through `DoRenderSample`. Against untouched `lionhead_logo.wmv`, the
gate received all 419 frames at 640x480, observed changing sample content, and
received `EC_COMPLETE`. This proves graph negotiation and native sample
delivery without an `IVideoWindow` child.

The remaining work is reconstruction/integration: derive the retail subclass
on that now-proven base, connect its sample conversion to the reconstructed
D3D texture lifecycle under the recovered lock/event contract, and replace the
visible compatibility bridge.

## Live compatibility closure

The current DirectShow bridge deliberately mirrors the recovered outer
lifecycle while native texture presentation is reconstructed:

- it owns graph, media-control, media-event, and video-window interfaces;
- it drains completion/error events every checkpoint tick;
- it tears down one graph before constructing the next boot record;
- Escape completes only the active record;
- final completion releases the child video window and leaves the D3D9
  frontend checkpoint visible.

The focused `-VerifyBootToFrontend` smoke skips all three records, requires the
post-movie frontend title, verifies that no DirectShow child remains attached,
and samples the revealed frame for real color variation.
