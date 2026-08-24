# The Things editor — state and next steps

Written 2026-08-23, end of session. Companion to `docs/CUSTOM_LEVEL_AUTHORING_UX.md`
(the whole authoring journey). This file is the resume point for **level object
editing**: placing, arranging, configuring and scripting the things in a level.

## 0. Where it stands

Working in the GUI today (`Things` rail entry, separate from `World > Landscape`):

- **Shipped levels can be opened at all.** Retail levels live inside
  `FinalAlbion.wad`, so double-clicking one used to dead-end at
  `lev: cannot open ...`. `App::CheckoutLevelFromWad` extracts the `.lev` **and**
  its `.tng` into a folder of the user's choosing and edits those. The install is
  never written to; deleting the folder undoes everything.
- **One shared 3D world, two editors.** `App::BeginTerrainViewport` owns the
  camera, the ground mesh and the cursor ray; `App::ProjectPoint` projects a
  map-local point. Landscape (sculpt/paint/foliage) and Things each draw only
  what they own, so switching panels does not move the camera.
- **Every thing is drawn where it is.** Greatwood_1 renders **304 of its 313**
  things; the 9 skipped have no `CTCPhysicsStandard`, i.e. no position. Pins are
  colour-coded by kind (Object / Marker / AICreature / Building / TrackNode /
  Village), depth-sorted, distance-scaled.
- **Select, drag, inspect.** LMB picks the nearest pin within 18 px; dragging
  moves it across the ground and re-snaps Z to terrain; the inspector has exact
  position, facing and "Drop to ground". Edits go back through
  `tng::File::setCtcProperty`, and facing writes the retail-quantized basis
  (`* 0.999994`), never a unit vector.
- Filter by type / def / script name, hide markers, `Save .tng`.

## 1. The reference design (ChocolateBox / FableMod)

`D:\Code\Fable\ChocolateBox\ChocolateBox\DecompiledDLLs\FableMod.Gfx.Integration`
is the only one of the three references that has a thing editor. EgoCore's 3D
view is **mesh-only** (`Meshes/MeshProperties.h`) with no level or thing editing,
and SilverChest is format bridges with no viewport. So FableMod is the model:

| FableMod (`GfxThingView.cs`) | Meaning for us |
|---|---|
| `EditorMode { Normal, Walk, Player, Create, Navigation, Pick }` | Modes are separate tools, not tabs on one brush panel. Matches the Landscape/Things split. |
| `ThingSelect` = LMB, `ThingAddSelect` = **Ctrl+LMB** | Multi-select is expected. We only have single select. |
| `ThingRotate` = **RMB**, `ThingCopyRotate` = Ctrl+RMB | Rotate is a drag, and Ctrl+drag duplicates. Our RMB is camera freelook — this conflict must be resolved deliberately (suggestion: RMB rotates when a thing is selected and the cursor is over it, else flies). |
| `SelectionBox` + `Device.DrawBox(min,max)` | Multi-selection draws one wire box around the whole set. |
| `DirectionAxes` | Orientation gizmo, not just our single facing line. |
| `GfxThingInterface::Create` → real model node, else **`ImpostorNode`** | A marker is not a *kind of thing*; it is the fallback for anything with no renderable mesh. Draw meshes where they exist, pins only where they do not. |

## 2. Mesh previews — both missing links are now closed

This is the top of the next session's list, and it is no longer a research task.

**Link A, def → mesh id.** An `OBJECT` def carries a `Graphic` field of type
`CEngineGraphic`. Decoded for `OBJECT_BW_SIGN_POST_DIRECTION_01` (entry 3701):

```
Graphic  CEngineGraphic  05000000 dd140000 00000000 0000803f 00
                         ^kind=5  ^id=5341 ^0        ^scale=1.0f
```

id 5341 resolves in `graphics.big` `MBANK_ALLMESHES` to
**`MESH_OBJECT_WOODEN_SIGNPOST_01`** — verified. So the chain is
`TNG DefinitionType -> game.bin def -> Graphic.modelId -> MBANK_ALLMESHES entry`.
The same def also carries `MeshHeight 2`, `MeshRadius 1`, `MeshHeightOffset`,
`ApproxMaxMeshHeight` and `RenderFadeDistance 45` — enough to draw a correctly
sized proxy box even before any geometry is decoded, which is a good first
increment.

**Link B, mesh → geometry.** `tools/blender_addon/io_scene_fable/fable_core.py`
`_prim_geometry` (line 711) already extracts `(vertices, faces, per-loop uvs)`
from a compiled primitive, ported from EgoCore's `GltfExporter.h`, handling
compressed positions/normals, bone strides, stride-36 instanced meshes, and
strip-vs-list triangulation. It is proven by the Blender round-trip.
**It is Python; forgecore has no geometry decoder** — `foliage.cpp` only walks
primitive *headers* for triangle counts. Porting a read-only `decodeLod0` into
forgecore is the work.

**Suggested increments, cheapest first:**

1. **Proxy boxes** from `MeshRadius` / `MeshHeight` / `MeshHeightOffset` — no new
   decoding at all, and immediately shows buildings as buildings.
2. **LOD0 wireframe** via a ported `decodeLod0`, cached per mesh id (many things
   share a def; Greatwood_1's 304 things use far fewer distinct meshes).
3. **Flat-shaded triangles** with the existing depth-sort, reusing the terrain's
   painter's algorithm. Cap by `RenderFadeDistance` and a triangle budget; fall
   back to a pin when over budget, exactly as FableMod falls back to an impostor.

Creatures follow the same path through their own def's Graphic field; buildings
are `Building` things and also have defs.

## 3. Adding entities

Today `forge tng place` and the GUI's older Place form both work, but adding is
still "type a DefinitionType". What it should be:

- **A palette, not a text field.** `forge::gamedata` already reads the def bank;
  group `OBJECT_*` / `CREATURE_*` / `MARKER_*` by category, filterable, with the
  mesh proxy as the thumbnail once (2) lands.
- **Drag from palette into the world**, dropping onto the terrain ray hit —
  the same hit `BeginTerrainViewport` already returns.
- **Validate the def exists** before writing. `forge tng place` will currently
  write a bogus DefinitionType happily; cross-check against `names.bin`.
- Reuse `forge::thingplacer` for the block text: it emits retail field order
  (PhysicsStandard → Targeted → ActionUseReadable → Editor, `Health` last) and
  allocates a UID at max+1 in the `0xFFFFFE00` namespace, per-file unique.

## 4. Editing settings

The inspector shows position and facing only. Every other property is already in
the parsed model (`Thing::properties`, `Thing::ctcBlocks`) and writable via
`setThingProperty` / `setCtcProperty`. The gap is presentation:

- Show the thing's **CTC blocks as sections** with typed editors, not raw text.
- Common ones first: `CTCActionUseReadable.GameTextDefName` (the sign's text),
  `CTCDRegionExit.EntranceConnectedToUID` (the doorway link),
  `CTCDoor.Open`, `CTCTargeted.Targetable`, `CTCChest`, `CTCSearchableContainer`.
- `ScriptName` / `ScriptData` belong at the top: they are the handle everything
  else binds to.
- Text tags should be a **picker over text.big**, not a typed string — 175
  `TXT_SIGN_*` tags already exist.

## 5. Editing scripts (ForgeFSE / nodes / FQT)

Decided earlier this session and unchanged: **Fable has no quest VM** (161-entry
name→allocator table, compiled C++), so a node graph cannot compile to bytecode.
Its output is:

1. a **ForgeFSE Lua handler** bound to the thing's `ScriptName`, and/or
2. a `.qst` entry, remembering that `AddQuest(name, TRUE)` in
   `data/Levels/FinalAlbion.qst` is what actually activates a custom quest — and
   that Steam's "verify files" wipes it.

FableForge already has the quest node editor (`forge::questnodes`, ax::NodeEditor
canvas, Blueprint grammar) and the deploy path (`questdeploy::planDeploy`). The
missing seam is small: **select a thing → Edit Script → open the graph bound to
that ScriptName → Compile writes the Lua and sets `ScriptName` on the thing in
one action.** Give a thing with no script name a generated one
(`<Level>_<Def>_<n>`) rather than making the user invent it.

## 6. Known gaps and traps

- **RMB conflict** between camera freelook and FableMod-style rotate (see §1).
- **No multi-select / no SelectionBox.** Ctrl+LMB is the expected binding.
- **No undo in Things.** Landscape has undo/redo; Things does not. Dragging a
  thing is currently unrecoverable except by not saving.
- **9 positionless things are invisible** and cannot be selected at all; they
  should still be reachable from the list.
- **`insertThingBlock` vs `addThing`:** `addThing` round-trips through the model
  and reorders retail's trailing `Health` line to the top. For byte-faithful
  appends use `insertThingBlock` (verbatim text).
- **The WAD is the load path.** Editing the checked-out `.tng` is not enough to
  see it in game — the level must be repacked into `FinalAlbion.wad`
  (`forge wad repack`), and the installed WAD is **not** pristine retail (it
  carries the ForgeTest experiment).
- **Positions are map-relative** to the WLD `MapX`/`MapY`; `PositionZ` is world
  height, and retail sets it to the bilinear terrain height at that local XY.

## 7. Immediate next steps

1. Proxy boxes from def mesh dimensions (§2.1) — biggest visual payoff per hour.
2. Port `decodeLod0` to forgecore; wireframe, then flat-shaded (§2.2, §2.3).
3. Palette + drag-to-place, with def validation (§3).
4. CTC property inspector, text-tag picker (§4).
5. Thing → node graph → ForgeFSE Lua round trip (§5).
6. Then, finally, the in-game test that is still outstanding: repack, install,
   launch, and read the sign in Greatwood_1.
