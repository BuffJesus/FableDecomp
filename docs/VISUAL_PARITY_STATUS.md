# Frontend visual parity — status & resume guide (2026-08-07)

Live visual QA of the reconstruction's D3D9 frontend against retail screenshots
(`resources/UIScreenshots(Retail)/`). This doc is the resume point for the visual
lane: how to build/run/drive the UI, what's fixed, and the one open artifact.

## Build, run, and DRIVE the UI (automated visual testing)

The reconstruction can be driven headlessly with synthetic input + framebuffer
screenshots — no need to ask the user to click.

1. **Build** (compiles `visual_boot_d3d9.cpp` + checkpoint, stages loose assets):
   ```powershell
   $bank='C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\graphics\pc\frontend.big'
   & 'D:\Documents\FableTLC\rebuild\build_bootstrap.ps1' -RetailFrontendBank $bank
   ```
   Success line: `VISUAL_BOOT_CHECKPOINT PASS ... FableTLC-Reconstruction-VisualCheckpoint.exe`.
   The exe is asset-free (~1.4 MB); it loads the 19 atlases from `data\frontend\*.bmp`
   staged next to it.

2. **Launch** detached and grab the client rect (client is **1280x720**):
   ```powershell
   $exe='D:\Documents\FableTLC\rebuild\build\bootstrap-Release\FableTLC-Reconstruction-VisualCheckpoint.exe'
   $p=Start-Process $exe -WorkingDirectory (Split-Path $exe) -PassThru
   ```
   Use Win32 `GetClientRect`+`ClientToScreen` on `$p.MainWindowHandle` for the
   screen origin; the window opens at a varying position each run, so recompute.

3. **Click** via `SetForegroundWindow`+`SetCursorPos`+`mouse_event(0x2/0x4)` at
   `origin + (clientX, clientY)`. **Screenshot** via `Graphics.CopyFromScreen` of
   the client rect into a `Bitmap` → PNG, then Read the PNG. To inspect glyphs,
   crop + upscale 3x with `InterpolationMode=NearestNeighbor` (see scratchpad
   `shot*_zoom` recipe). NOTE: `$PID` is read-only in PowerShell — use another var.

   Menu map (client coords, 1280x720): first click anywhere dismisses the title →
   main menu. Main menu items ~x=640: Continue Game y~320, Change Profile y~360,
   Options y~407. Options submenu: Gameplay/Video/Audio/Redefine ~y 224/265/307/350.
   Detail screens have Back/Cancel bottom-left (~x300 y655/631), Apply bottom-right.
   Profile flow: Change Profile → click a profile row (LoadProfile) sets the active
   profile and returns to main menu.

## FIXED this session (verified on-screen)

- **Text ghosting on the detail-title font** — was point-sampled at the non-integer
  640→1280 stretch. Forced LINEAR min/mag on sampler 0 after BeginScene
  (`visual_boot_d3d9.cpp`, commit 352a684). Audio/Video Options titles + option
  rows ("Music Volume" etc.) verified clean at 3x.
- **Sliders** — single clean pill bar + knob (no stacked double-texture band).
  Verified on Audio Options (Music/Sound/Dialogue Volume).
- **Load Game title** — shows "<profile> - Load Game" once a profile is selected
  (retail flow: title → Change Profile → select → Continue). Verified
  "112121212 - Load Game". No hardcoded fallback (commit e00b614).
- **Save row names** — AutoSave / Save 1 / Save 2 / Save 3 (not garbled "Manual save").
- Title screen + main menu (coastal backdrop, menu items, selection scrollwork)
  match retail.

## OPEN — profile-name / save-row font still doubled (NOT YET FIXED)

The **profile-name font** (`AppendProfileNameText`, the ENG_ARIAL glyphs used for
save rows "AutoSave/Save 1/2/3", profile names, and File Information) renders with
a doubled/ghosted echo on every glyph. Retail's are smooth (soft AA + shadow) —
see `resources/UIScreenshots(Retail)/ContinueGameScreen.png` vs a live capture of
the Continue → Load Game screen.

Ruled out this session:
- **Not a double-draw** — `AppendProfileNameText` emits ONE quad per glyph
  (visual_boot_d3d9.cpp ~L740); the save-row loop (~L4501) calls it once per row.
- **Not the API sampler filter** — setting `SetSamplerState(0, MIN/MAGFILTER,
  LINEAR)` both after BeginScene AND per-batch after `RealiseRenderState()` made
  **no visible difference** to these glyphs (the per-batch attempt was reverted as
  a no-op). The detail-title font DID look clean, but that may be independent.

Leading hypothesis for next session: the frontend samples the atlas through a
**pixel shader** (window title: `...PixelShaderAndWorldTransformState`), so
fixed-function `SetSamplerState` is bypassed for the atlas fetch — the shader's
own sampler/`tex2D` controls filtering. Investigate:
1. The Render2D pixel shader (`CShaderRenderManager` / the recovered PS) and which
   sampler register the atlas binds to; check its declared filter.
2. Whether the ENG_ARIAL profile-glyph atlas cells (`kFableProfileGlyphMetrics`,
   origin `kFableProfileGlyphAtlasOriginX/Y`) are tightly packed (the earlier
   space-abuts-'!' bleed fix confirms tight packing) so any filtering bleeds
   neighbors — may need half-texel UV inset.
3. Compare the extracted ENG_ARIAL atlas glyph bitmaps to retail's on-screen AA to
   confirm whether the doubling is sampling or baked atlas content.

## Other deferred visual items (older, unchanged)
- Coastal backdrop base brightness/vignette is a structural boundary (see memory
  `audiooptions-backdrop-not-sunbeam`), not a sunbeam-alpha issue.
- Redefine live-scroll still uses the baked page atlas (flashing during scroll);
  the byte-pure native-scroll path is task #11 (CRedefinerList Refresh/OnActivate/
  Initialise — Refresh RE'd in REDEFINE_INPUT_SYSTEM.md work).
- Gamepad "Redefine Keys (Keyboard)/(Gamepad)" menu-row split not wired yet (only
  the detail-screen title split is in); see GAMEPAD_REDEFINE_PATCH.md.

## 2026-08-10 — profile-name font-doubling: static root-cause narrowing (build-light)

Narrowed the OPEN doubling bug via static analysis (no build; canonical refresh had
VC7.1 busy). Findings:
- A GLOBAL half-texel screen offset IS already applied (`left=top=-0.5f`, visual_boot_d3d9.cpp ~L3041),
  so it is NOT a missing global -0.5.
- `AppendProfileNameText` (L757-783) emits glyph UVs spanning the cell EXACTLY
  `[atlasX, atlasX+width]/OptionsWidth` with **no half-texel inset**, and the screen quad is
  `glyph.width * scaleX` (design->screen ~2x magnify) — so LINEAR magnification samples past the
  cell edge into the tightly-packed neighbour glyph => ghost/double. This is why the sampler-mode
  toggle had no effect (UVs reach the neighbour texel regardless of filter).
- BUT the clean detail-title path (`AppendDetailTitleGlyphText`) uses the SAME non-inset UV formula,
  so packing/UV alone is not the whole story. The distinguishing factor: detail-title scales the
  quad by `glyph.width * glyphScale` (glyphScale ~2/3, net ~1:1 or minified), while profile-name
  uses raw `glyph.width` (net magnified). => the doubling correlates with net MAGNIFICATION ratio.

CANDIDATE FIX (apply + screenshot-verify once VC7.1 frees): inset the profile-name glyph UVs by
half a source texel — `u0 += 0.5/OptionsWidth; u1 -= 0.5/OptionsWidth; v0 += 0.5/OptionsHeight;
v1 -= 0.5/OptionsHeight` (and mirror in the detail-title outline path if it regresses). If that
doesn't fully clear it, confirm the ENG_ARIAL atlas has zero inter-cell gutter and add a 1px gutter
to the atlas bake instead. Verify against resources/UIScreenshots(Retail)/ContinueGameScreen.png.

### RESOLVED 2026-08-10 (visual-QA verified)

FIXED via the half-texel UV inset in `AppendProfileNameText` (sample from texel
CENTRES: `(atlasX+0.5)/W` .. `(atlasX+width-0.5)/W`, same for V). Root cause
confirmed by evidence, not guess:
- The ENG_ARIAL atlas region (origin 1024,2704 in visual_boot_options_menu.bmp) is
  CLEAN — crisp single glyphs, so the doubling was NOT baked content.
- The live Load Game save rows showed a hard ~1px horizontal double-column echo on
  every glyph (classic half-texel signature: edge-sampled UVs straddle two source
  texels under the 2x design->screen LINEAR magnify). The detail-title path escaped
  it because it draws at ~1:1 (glyphScale ~2/3), not 2x.
- After the inset, rebuilt + re-captured the same Continue->Load Game screen: the
  double-columns are gone and `AutoSave`/`Save 1/2/3` render as clean single strokes
  matching resources/UIScreenshots(Retail)/ContinueGameScreen.png.
Screenshots: scratchpad qa_loadgame_baseline.png / qa_loadgame_fixed.png /
qa_compare_beforeafter.png / qa_retail_saverows.png. This is a reconstruction-side
D3D9 draw correctness fix (visual match to retail), not a byte-parity function.
