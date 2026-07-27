# Optional AI-upscaled menu/UI presentation

Status: planned presentation enhancement. Retail assets remain the canonical
decomp/parity source.

## Goal and boundary

Menu backdrops, logos, buttons, icons, HUD sprites, and font atlases can be
enhanced locally as each asset enters the reconstructed renderer. This should
mirror the video policy:

- never modify the installed game or the decoded retail source;
- write enhanced files only to an ignored build cache;
- select enhanced assets only through an explicit runtime option;
- fall back independently to the retail asset when an enhanced file is
  absent, invalid, or incomplete;
- never count an AI result as byte parity, rendering parity, or RE evidence.

The proposed cache is
`rebuild/build/bootstrap-Release/upscaled-ui/`. The proposed entry point is
`rebuild/upscale_retail_ui.ps1`, with a future runtime switch
`--retail-ui-upscaled`. Neither interface exists yet.

The existing local Video2X/Real-ESRGAN NCNN installation used by
`rebuild/upscale_retail_videos.ps1` is reusable and free/local. Image-oriented
Real-ESRGAN models should be compared on representative Fable art before one
is pinned. Organic backdrops and hard-edged UI sprites may need different
models; the chosen model and its hash belong in every output manifest.

## Asset-specific rules

### Opaque backdrops

Decode to lossless PNG, upscale at 2x, and retain the original 4:3 logical
layout. `FRONTEND_BACKDROP_01` therefore becomes a 1280x960 physical texture
while still occupying the 640x480 design canvas. The renderer must use the
enhanced texture dimensions for sampling, not for widget coordinates.

Animated forest backgrounds are tiled in retail data. For the checkpoint,
stitch each six-tile 640x480 frame before upscaling so the model sees both
sides of every internal seam. If the native bank path later requires separate
tiles, split the enhanced result back on exact scaled boundaries and generate
consistent edge gutters/mips.

### Transparent logos, buttons, icons, and HUD sprites

Do not pass premultiplied RGBA directly through an RGB model. The pipeline
must:

1. extract the source alpha plane;
2. unpremultiply/extrude RGB beneath translucent edges;
3. upscale RGB with the pinned model;
4. upscale alpha deterministically, initially with Lanczos;
5. recombine and validate the transparent boundary;
6. preserve the authored logical pivot, child offset, and design-space size.

This avoids black/white halos and stops the model inventing opaque pixels in
transparent padding. DXT recompression is deferred for the reconstructed
checkpoint: cached PNGs should stay lossless. When bank repacking is needed,
the enhanced and retail mip chains must remain separate.

### Text and font atlases

Text needs stricter treatment than artwork. Generic AI can deform repeated
glyphs, punctuation, and small counters, so the preferred order is:

1. render from the decoded retail font at a higher physical resolution;
2. if a matching font source is available, rebuild a 2x atlas with the same
   metrics and character mapping;
3. use AI on the retail atlas only as an optional comparison, never as the
   default text path.

The static `ENG_ARIAL_24` atlas and `<ffffhhh>` metrics are already decoded by
`tools/render_fable_static_font.py`. The next font task is the streaming
Unicode `GlyphData` path required by the legal string's copyright symbol.

## Cache manifest and acceptance gates

Each enhanced asset should have a sidecar manifest containing:

- source bank/path, entry name/id, and source SHA-256;
- decoded dimensions/format and alpha presence;
- model name, model hash, tool version, scale, tile/overlap settings;
- alpha strategy and output SHA-256;
- completion status and validation results.

An output is publishable only when:

- width and height are the exact requested scale;
- source and retail cache files are unchanged;
- alpha coverage and transparent-border bleed remain within a fixed gate;
- stitched/split tile borders have no discontinuity;
- repeated UI states retain identical dimensions and pivots;
- the retail mode still passes its existing screenshot/parity gates;
- the enhanced mode passes boot-to-frontend, maximize, and stable-frame smoke
  tests with a visible `AI UI 2x` marker in the window title.

## Integration order

1. `FRONTEND_BACKDROP_01`.
2. `FRONTEND_TITLE_01_SPRITE` and `FRONTEND_TITLE_02_SPRITE`, treated as one
   composed transparent logo during enhancement.
3. Press-start forest frames, graphic ids 206-229, and sunbeam overlays
   230-247.
4. Main-menu buttons, arrows, panels, mouse pointer, and other visible
   `frontend.big` sprites as their widgets become live.
5. Static font atlases and then streaming Unicode fonts.
6. In-game HUD sprites from `textures.big`, only after the frontend path and
   alpha gates are stable.

## Resume checklist

1. Add `rebuild/upscale_retail_ui.ps1` using the pinned Video2X/NCNN install
   and per-entry manifests.
2. Implement lossless alpha separation/recombination plus seam-aware
   stitch/upscale/split helpers.
3. Produce and inspect 2x backdrop/title candidates without changing the
   default renderer.
4. Add `--retail-ui-upscaled` with per-asset fallback and an `AI UI 2x`
   window-title marker.
5. Extend `smoke_visual_checkpoint.ps1` with enhanced-asset selection,
   maximize, alpha-edge, and retail-fallback gates.
6. Continue the accurate startup path: streaming legal text, animated forest
   layers, left-click transition, then the first main-menu widget set.
