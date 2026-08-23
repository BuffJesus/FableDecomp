# ForgeTest64 — minimap, transition screen, region presentation (2026-08-22)

State at end of session. Terrain itself is DONE and confirmed in-game; this file
covers the presentation layer around it.

## Region record fields (measured across all 141 retail regions)

`forge bwd info <bwd> regions` now prints these.

| field | meaning | retail values |
|---|---|---|
| `RegionDef` | game.bin `REGION_*` def | every region with a minimap has one; fillers have none |
| `minimapGraphic` | in-game round minimap texture, by NAME | `MINIMAP_<REGIONNAME>` |
| `MiniMapScale` | minimap zoom | 1.000 almost everywhere (BowerstoneSlums is 1.100) |
| `MiniMapOffsetX/Y` | small per-region texel pan | varies: (23,-4), (0,0), (-1,-36), (25,5), (-12,31)... |
| `WorldMapOffsetX/Y` | **position of the region on the WORLD MAP** | large: (1014,887), (949,888), (963,1239), (839,1660)... |

## Minimap — WORKING

Graphic resolves from `minimapGraphic` against **textures.big / GBANK_MAIN_PC**.

**A newly APPENDED bank entry is never found.** Adding `MINIMAP_FORGETEST64`
(id 6293, past retail's 6292) silently did nothing. Overwriting an EXISTING entry
works. Only three retail `MINIMAP_*` region graphics are referenced by no region
and are therefore free to repurpose:

- `MINIMAP_PRISONCOURTYARD2` (6067)
- `MINIMAP_TELEPORTERGREATWOOD` (6074)  <- currently ours
- `MINIMAP_BORDER` (5984, not a region map)

`RegionDef` is NOT the gate — `REGION_BATTLE` was tried and made no difference.

### Art contract (measured against `MINIMAP_GREATWOOD`)

- 256x256 DXT3, RGBA.
- Alpha: fully opaque to r=0.85, feathers to r=1.0, gone past 1.0. 16 alpha
  levels. Retail **fills its frame edge to edge** and slightly overlaps the bezel
  — that is intended. An inset disc (tried at r=0.62) under-fills and looks wrong.
- Opaque mean RGB ~ (182,188,123): light olive parchment, cream drawn features.
- **Vertical flip required**: world Y runs opposite to image rows. Unflipped, the
  hero arrow appeared at the far end of the map from where the hero stood.

Generator: `scratchpad/mkmm4.py` (hillshade + contour lines + retail alpha).

### OPEN: zoom / arrow range

The minimap is zoomed too far in to show the whole 64x64 map, and the hero arrow
can walk off the drawn content. Relevant measured facts:

- Retail maps are typically **128x128 world units** (LookoutPoint
  (3232,3488)-(3360,3616)); regions contain several. ForgeTest64 is **64x64** —
  much smaller than anything retail.
- `MiniMapScale` is 1.0 for essentially all retail regions, so it is not used to
  compensate for region size.

Two competing models (region-bounds-normalised vs fixed world-units-per-texel)
predict different fixes, and the observed arrow travel is not clearly consistent
with either. **Do not guess again** — reverse the minimap render path in
`Fable.exe` and read the mapping. Two blind attempts have already been spent.

## Region transition screen — art staged, NOT working

The loading screen (spotlight circle over the world map + region name) uses the
**`_FRONT_END` variant in frontend.big / GBANK_FRONT_END_PC**, e.g.
`MINIMAP_GREATWOOD_FRONT_END` (150), `MINIMAP_ORCHARDFARM_FRONT_END` (179).
`MINIMAP_TELEPORTERGREATWOOD_FRONT_END` (192) is free and now holds our art.

**Still black at runtime.** Prime suspect: our region's `WorldMapOffsetX/Y` is
`(0,0)`, which places the spotlight at the world-map origin rather than over the
region. Retail values are large, e.g. Greatwood (963,1239), LookoutPoint
(1014,887). Next step: add a `--worldmap-offset` option to
`forge bwd set-region-name` and set a plausible position.

## Exit signpost — spawn FAILED silently

`FSE_Master.lua` gained `ForgeReturnPos` capture + a signpost + `ForgeExitWatch`
proximity thread. The log shows `stored return pos (3107.3,2986.2,62.3)` but
**no `exit signpost at ...` line**, so the `pcall` around `CreateObject` threw and
swallowed it.

- `CreateObject`, `CreateObjectOnEntity`, `CreateCreature`, `GetGroundHeightAt`,
  `GiveHeroYesNoQuestion` are all confirmed PRESENT as strings in the deployed
  `FableScriptExtender.dll`, so the binding exists.
- Prime suspect: `OBJECT_SIGNPOST_TEMPLATE` is an abstract template def and not
  spawnable. Concrete alternatives exist: `OBJECT_BS_SIGN_POST_DIRECTION_01`,
  `OBJECT_OK_SIGNPOST_DIRECTION_01`, `OBJECT_BW_SIGN_POST_DIRECTION_01`,
  `OBJECT_BHF_SIGNPOST_01`.

**Fix the logging first.** The current block wraps the Log call inside the same
pcall as the CreateObject call, so a failure produces silence. Log before, log
the pcall's error message, and try several defs in turn.

Requested but not designed: marking the signpost on the minimap. Retail has
`MINIMAP_ICON_*` textures (`MINIMAP_ICON_HOUSE` 5966, `MINIMAP_ICON_TELEPORT`
5967, `MINIMAP_ICON_DEMON_DOOR` 5964, `MINIMAP_ICON_HERO` 5969), so icons are a
per-thing engine feature; no authoring path is known yet.

## New forge commands added this session

- `bwd set-owner <in> <out> <level> --region <name>` — strict-partition ownership
- `bwd add-region <in> <out> <name>...` — append empty regions
- `bwd set-region-name <in> <out> <old> <new> [--minimap <g>] [--def <d>]`
- `stb patchverts <chunk> <frame> [limit]` — dump a patch VB in stored order
- `stb create-terrain ... [--slope-tex <id>] [--height-tex <id>]`
- `bwd info ... regions` now prints minimap graphic, scale and offsets
