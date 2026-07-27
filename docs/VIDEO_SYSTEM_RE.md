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
`CVideoSys::GetTexture @ 0x00A3B320` is now promoted as an exact
10-byte relocation match with a behavior fixture proving the
`CVideoSys +0x20 -> CTextureRenderer +0x164` handoff.
`WaitForState @ 0x00A3B0F0` and `AttemptToPlay @ 0x00A3B1A0` are also
exact relocation matches. Their fixtures prove the bounded 100-poll filter
state wait and the successful/failed `IMediaControl::Run` transitions.
`Pause @ 0x00A3B1C0` and `Stop @ 0x00A3B1F0` are exact 43-byte matches;
their gates prove the COM calls, requested filter states, and logical
playback-state updates.

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

The `SetMovie` tail-call leaf is promoted as an exact 11-byte relocation
match, and its fixture proves assignment of the recovered `CWideString` field.
The `IsPlaying` leaf is an exact 7-byte relocation match whose fixture proves
both stopped and playing states.

The current reconstructed checkpoint still uses a compatibility DirectShow
child window for visible playback. Its ownership and end-of-stream sequencing
now follow the recovered contracts. Independently, the native renderer probe
implements the recovered sample handoff into a real D3D9 texture; the remaining
visible integration step is to wrap that texture in the reconstructed
`CTexture` contract and feed it through recovered `CMovie::Draw`.

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
`FablePublishDecodedVideoFrame` now supplies the recovered outer contract:
it enters `CVideoSys +0x64`, invokes the writer that locks, converts, and
unlocks the destination, publishes the texture at `+0x80` only after that
writer returns, signals the frame event at `+0x7C`, and leaves the critical
section. Its fixture proves the ordering, the successful publish/event path,
and rejection without publication.

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
samples through `DoRenderSample`. It creates a managed A8R8G8B8
`IDirect3DTexture9`, locks it inside the recovered publication critical
section, converts each RGB24 sample, unlocks it, and only then publishes the
texture pointer and signals the frame event. Against untouched
`lionhead_logo.wmv`, the gate received and converted all 419 frames at
640x480, observed changing texture content, and received `EC_COMPLETE`,
without an `IVideoWindow` child. A second managed D3D9 texture now exercises
the recovered `CMovie::Draw` consumption prefix as well: each auto-reset frame
event is consumed, the published decoder texture is read under
`CVideoSys +0x64`, and its rows are copied into the presentation texture.

The remaining work is D3D/UI integration: replace the probe's direct
`IDirect3DTexture9` ownership with the reconstructed `CTexture` wrapper, feed
that wrapper through the sprite-construction/submission tail of `CMovie::Draw`,
and retire the visible compatibility bridge.

The immediate wrapper boundary is now instruction-mapped rather than generic.
`CTexture::CopyFromTexture @ 0x009FA4E0` iterates the smaller source/destination
mipmap count and, for each level, obtains both `IDirect3DSurface9` objects,
wraps them as temporary `CSurface` values, calls
`CSurface::CopyFromSurface @ 0x009F37E0` with color `0xFFFFFFFF`, and releases
both temporaries. Its four direct helper boundaries are the `CSurface` copy
constructor at `0x009F2D60`, `InitialiseFromTextureMipmap @ 0x009F2F10`,
`Uninitialise @ 0x009F2E20`, and `CopyFromSurface`. All four are now canonical
retail matches with linked behavior fixtures. `CopyFromSurface` is a
446/446-byte relocation-normalized match covering direct D3DX copies and the
temporary-surface signed-format conversion branch. The remaining shortest
native-visible movie path is the `CopyFromTexture` coordinator itself, the
reconstructed `CTexture` wrapper connection, and the 2D-sprite submission
tail.

## Live compatibility closure

The current DirectShow bridge deliberately mirrors the recovered outer
lifecycle while native texture presentation is reconstructed:

- it owns graph, media-control, media-event, and video-window interfaces;
- it drains completion/error events every checkpoint tick;
- it tears down one graph before constructing the next boot record;
- Escape completes only the active record;
- final completion releases the child video window and leaves the D3D9
  frontend checkpoint visible. That checkpoint now draws the genuine
  two-part retail Fable title over the backdrop with source alpha, using the
  decoded `UI_TITLE` parent position `(70,30)` and the second child's exact
  `+256` X offset.

The focused `-VerifyBootToFrontend` smoke skips all three records, requires the
post-movie title marked `Post-Movie Startup Ordered`, verifies that no
DirectShow child remains attached, and samples the revealed frame for real
color variation. Before publishing that title, the executable crosses the
recovered bank-open, init, first-clear/swap, and `ChangeStateFirstTime` order
through explicit service boundaries.

`CTexture::CopyFromTexture @ 0x009FA4E0` is now a 289-byte
relocation-normalized match with a linked behavior fixture covering mip-count
minimums, per-level acquisition, filter forwarding, temporary surface
ownership, and cleanup order. Together with its exact `CSurface` helpers, this
closes the texture-copy coordinator immediately below the remaining
`CMovie::Draw` sprite submission tail.
