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

## 2026-08-16 — visual build pipeline repaired + Redefine baked-scroll fully mapped

**Build pipeline was broken by accumulated drift; now green again** (checkpoint exe
rebuilds, `VISUAL_BOOT_CHECKPOINT PASS`). Four independent breakages fixed this session:
1. Leaf-sweep auto-landing clobbered two `build_bootstrap.ps1`-owned fixtures
   (`CMovie::IsPlaying` 0x548520, `CVideoSys::GetTexture` 0xa3b320) — restored
   (commit "restore bootstrap-owned CMovie/CVideoSys fixtures").
2. Pre-existing `CVideoSys::AttemptToPlay` behavior fixture (from 5500606) redefined
   the free function the linked source obj defines (always-LNK2005) and printed the
   wrong pass pattern — never build-validated because upstream was already broken.
   Fixed the test to forward-declare + emit `FABLETLC_CVIDEOSYS_ATTEMPT_TO_PLAY PASS`.
3. `FABLETLC_WINMAIN_BEHAVIOR FAIL code=2` when **retail Fable.exe is running** — WinMain
   calls the real `OpenMutexW` single-instance guard; the fixture is non-hermetic w.r.t.
   the OS mutex. Close Fable before building. See memory `winmain-fixture-needs-fable-closed`.
4. `frontend_save_rows.obj` + its `fable_inflate.obj` (FableZlibInflate) dep were missing
   from the two behavior-exe links that link the checkpoint obj (`VisualBoot-Behavior`,
   `SaveViewportQuads-Behavior`) after the checkpoint driver gained a SaveRows call —
   added them (commit "link frontend_save_rows + fable_inflate into visual behavior exes").

Navigate+capture harness confirmed working (`scratchpad/vnav.ps1`): title → Select Profile
→ profile → main menu → Options → Redefine Keys; scroll via down-arrow clicks at client
(640,548). Captures: `cap_redef_page0.png` (matches retail RedefineKeys1),
`cap_redef_scroll4.png` (scrolled).

**Redefine baked-scroll (task #11) — structure fully decoded; ONE datum blocks native.**
The scrolled list (`g_RedefineListSelection > 0`) blits a baked 3200×3360 page atlas
(`g_RedefineScrollPagesTexture`, visual_boot_d3d9.cpp ~L4076) — a bake AND the "flash on
scroll" cause. Decode:
- The 9 grey row **pills are STATIC** across all pages (retail RedefineKeys1 vs 5 pixel-
  identical; confirmed live) and come from the **static detail component overlay** quad
  (~L3597, drawn for all detail screens regardless of selection). So the baked page is
  redundant for the pills.
- Action + key **text already renders natively** at the scroll offset (AppendRedefineAction
  Text/AppendRedefineKeyText, L1334-1337/1386-1389 use `RedefineScrollPageOffset`).
- The **only content unique to the baked page is the up/down scroll arrows.** The down
  arrow on page 0 is baked into the static overlay at design (304,350) and gets erased by
  a forest-frame repaint on scrolled pages (~L4037-4064); the **up arrow exists only inside
  the baked page atlas**.

**Native-conversion plan (remaining):** drop the baked-page blit (L4076-4091); rely on the
static overlay for pills + native text; draw the scroll arrows natively per
`kFableFrontendRedefineListArrowLayout` (frontend_list_layout.h): arrowX=304, upY=80,
downY=350, 32×32; up when selection>0, down when not last page; keep the page-0 down-arrow
erase.

**RESOLVED 2026-08-16 (RE):** the "arrow sprite atlas UVs" premise was a misdiagnosis —
there is **no shared atlas** for these arrows. `FE_SCROLL_*_SPRITE` entries 379..382 are each a
**standalone 32×32 A8R8G8B8 texture entry** in `frontend.big` sub-bank `GBANK_FRONT_END_PC`
(entry ids 379/380/381/382, matching front_end_bank.h `FE_SCROLL_UP_SPRITE=379`,
`_DOWN_SPRITE=380`, `_UP_HOVERED_SPRITE=381`, `_DOWN_HOVERED_SPRITE=382`; also 383/384 = CLICKED
variants). The frontend graphic-index → frontend.big entry-id mapping is direct (same number),
so each arrow's **UV is simply full-frame (0,0)→(1,1)** of its own texture — no sub-rect lookup.
Format is uncompressed A8R8G8B8; on disk mip0 is chunked-LZO (MipSize0≈1165 B), mips 1–3 raw —
a normal frontend.big texture, decodable with the proven pipeline
(`python tools/texture_build.py decode <frontend.big> FE_SCROLL_UP_SPRITE out.png --crop-real`).
Decoded glyphs verified: up = grey circle + up-triangle; hovered = tan-highlighted circle.

**IMPLEMENTED + visual-QA verified 2026-08-16** (`visual_boot_d3d9.cpp`): the full-page baked
blit (was L4076-4091) is dropped. On scrolled pages (`g_RedefineListSelection > 0`) the renderer
now draws the up/down arrows as **two discrete 32×32 quads** at their authored
`kFableFrontendRedefineListArrowLayout` positions (up (304,80), down (304,350)); the nine grey row
pills come from the static detail-component overlay and the action/key text renders natively below,
so nothing baked is lost. The arrow pixels are the real FE_SCROLL sprites the subscreen renderer
already bakes into each page cell (at the same (304,80)/(304,350)), so this needed **zero texture
plumbing** — no new upload/attach/init-param/BMP. The up arrow is always present while scrolled; the
down-arrow footprint is transparent on the final page, so its quad draws nothing (matching retail's
"no down arrow on the last page"); the page-0 down-arrow forest-repaint erase is retained so the
static overlay's baked down arrow is removed on the final page. Verified headlessly (`scratchpad/
vnav.ps1`): page 0 = down only; middle scrolled page = both arrows; last page = up only. Captures:
`scratchpad/nav_redef_page0.png` / `nav_redef_scroll3.png` / `nav_redef_scrollend.png`.

Follow-up (optional, not blocking): the 3200×3360 `g_RedefineScrollPagesTexture` atlas is still
generated/loaded purely as the arrow pixel source. A later cleanup can retire it in favour of a
small dedicated arrows texture (FE_SCROLL 379..382 packed into a ~128×32 sprite) threaded through
the `FableInitialiseVisualD3D9` upload chain + `RENDER2D_ADAPTER_ATTACH_TEXTURE` attach chain, which
would drop the 43 MB atlas BMP entirely.

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
