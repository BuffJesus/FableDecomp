# CREATURE_MESHY_HUNTER — a fully custom NPC (custom creature def + custom body mesh)

A brand-new CREATURE definition whose **base body mesh** is our custom skinned Meshy
model — distinct from any retail creature. Cloned from `CREATURE_TRADER_01` (so AI,
stats, combat and the 63-bone Bip01 animation set are all sane and voiced), with only
the body-mesh reference repointed.

Status: **SUCCESS (offline-verified).** Not yet live-tested in-game (requires deploying
the copies and spawning via ForgeFSE — steps below).

---

## The mechanism (DEFINITIVE, empirically verified this session)

A `CREATURE` def references its base body mesh through its **`Graphic` field**
(compiled-def stream tag `2e6b63c8`). The field payload is a `CEngineGraphic`:

```
struct CEngineGraphic {   // at the Graphic field's value bytes
    u32 kind;             //  4 = static/skinned body
    u32 modelId;          //  <-- graphics.big MBANK_ALLMESHES entry *id* (NOT index, NOT crc)
    u32 zero;             //  0
    f32 scale;            //  1.0
    u8  flag;
};                        // optional CRC-tagged nested sub-graphic blocks follow (eyes/attachments)
```

`modelId` resolves 1:1 to the graphics.big mesh whose **`id` field == modelId**
(keyed on `id`, verified with `fable_core.find_entry`). The engine chain is
`CEngineGraphic (0x00434b50) -> CTCGraphicAppearance::GetRenderMeshObject (0x004bc750)`.

### Proof (retail, verified here)
`CREATURE_TRADER_01`'s Graphic payload = `{kind=4, modelId=5149, 0, 1.0}` and
graphics.big id **5149 = `MESH_BS_MALE_MIDDLE_UNCLOTHED_01`** — the bare base-skin
humanoid body (clothes are layered separately via `InitialAppearanceModifiers` ->
`CAppearanceModifierDef.Graphics[]`). This is exactly why the earlier
`CREATURE_X -> MESH_X` name guess failed and why `MESH_TRADER_01` (id 5370) was NOT
in the def: humanoids point `Graphic` at a shared naked base body and dress it;
non-humanoids point `Graphic` directly at their unique body mesh. Nine creatures
were spot-checked; every non-zero modelId resolved to the correct graphics.big mesh.

In `CREATURE_TRADER_01`'s 1288-byte payload the Graphic tag sits at byte **offset 1114**
and `modelId` at byte **offset 1122**.

> EgoCore models no CREATURE field (defs are opaque text there); it only confirms a
> def references a mesh by its bank-entry identity. The def-diff investigation
> (running-code evidence) is the authoritative source and is what we verified.

---

## What was built (all under this folder)

| File | What it is |
|------|-----------|
| `graphics_meshy_hunter.big` | COPY of retail graphics.big + our skinned mesh `MESH_MESHY_HUNTER` (type-5, 63 Bip01 bones, id **8113**, diffuse tex id 6291). |
| `textures_meshy.big` | COPY of retail textures.big containing `TEX_MESHY_TRADER` (id **6291**). |
| `CompiledDefs/game.bin` + `CompiledDefs/names.bin` | COPY of retail defs + new entry `CREATURE_MESHY_HUNTER` (def CREATURE, 1288 B, Graphic.modelId = 8113). Donor untouched. |
| `01_recompose_mesh.py` | Recomposes the skinned mesh under the name `MESH_MESHY_HUNTER` into a graphics.big copy; prints its assigned id (writes `mesh_id.txt`). |
| `02_add_creature.cpp` / `02_build.sh` / `add_creature.exe` | Clones `CREATURE_TRADER_01`, patches Graphic.modelId (offset 1122) to the mesh id, `addEntry`s it as `CREATURE_MESHY_HUNTER`, saves a new game.bin+names.bin. |
| `mesh_id.txt` | The mesh id (`8113`) piped from step 1 into step 2. |

### Tooling notes
- Mesh id is `max(id in MBANK_ALLMESHES)+1` at inject time (currently **8113**). If you
  re-inject into a different graphics.big the id may change — re-run step 1, read
  `mesh_id.txt`, and pass it as the 3rd arg to `add_creature.exe`.
- `add_creature.exe` links the prebuilt `D:/Code/FableForge/build/libforgecore.a`
  (`forge::bin::File::addEntry` + `save()` — the same primitive `forge fmp apply` and
  `forge defs merge` use to add new defs; there is no CLI `defs add` verb yet).
  **Must build with `-std=c++20` and `-static -static-libgcc -static-libstdc++`**
  (the lib is C++20; a C++17 TU crashes on ABI mismatch, and a non-static link exits
  0xC0000139 for want of the runtime DLLs). Rebuild with `bash 02_build.sh`.
- The whole donor payload is cloned byte-for-byte; only the 4 modelId bytes change.
  All Components / stats / appearance-modifier references are global indices into
  entries we only appended to (never reordered), so they stay valid.

---

## Reproduce from scratch

```bash
cd D:/Documents/FableTLC/work/meshy_npc_mesh/custom_creature
python 01_recompose_mesh.py          # -> graphics_meshy_hunter.big, mesh_id.txt (8113)
bash 02_build.sh                     # -> add_creature.exe
./add_creature.exe "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters" \
     ./CompiledDefs "$(cat mesh_id.txt)"
```

## Offline verification (all PASS)
- `forge defs list <root>` shows `CREATURE_MESHY_HUNTER` (entry 14781; total 14782).
- `forge defs decode <root> def_schema.json CREATURE_MESHY_HUNTER` → `all_tags_ok=true`,
  **Graphic → kind=4, modelId=8113, scale=1.0**.
- `fable_core.build_model` on `MESH_MESHY_HUNTER` in the graphics copy →
  `has_skeleton=True, bones=63`, id=8113, type=5.
- `TEX_MESHY_TRADER` id 6291 present in `textures_meshy.big`.
- Donor `CREATURE_TRADER_01` still resolves to modelId 5149 (unchanged).

---

## DEPLOY (use copies; NEVER edit the live files in place)

Game root: `C:/Programs/Steam/steamapps/common/Fable The Lost Chapters`

Back up the three retail files, then drop in our copies:

```
data/CompiledDefs/game.bin      <-  custom_creature/CompiledDefs/game.bin
data/CompiledDefs/names.bin     <-  custom_creature/CompiledDefs/names.bin
data/graphics/graphics.big      <-  custom_creature/graphics_meshy_hunter.big
data/textures/textures.big      <-  custom_creature/textures_meshy.big   (path per your install; the copy that holds TEX id 6291)
```

game.bin and names.bin MUST be deployed together (names.bin carries the new
`CREATURE_MESHY_HUNTER` name row that game.bin's entry references).

## SPAWN (ForgeFSE / Lua)

Same call already proven for the trader NPC, just the new creature name:

```lua
Quest:CreateCreatureNearby("CREATURE_MESHY_HUNTER", pos, radius, scriptName)
```

It inherits `CREATURE_TRADER_01`'s AI/stats/animation set and voice; only the visible
body mesh is our custom skinned Meshy model (textured with TEX_MESHY_TRADER).

## What is left / caveats
- Not yet live-tested in-game (offline chain fully verified; needs a deploy + spawn run).
- The cloned Graphic keeps `CREATURE_TRADER_01`'s nested sub-graphic block(s) after the
  primary modelId (attachments/eyes if any). They were left intact; if the Meshy body
  needs none, they are harmless extras. Only the primary body-mesh id was repointed.
- If this creature is meant to be *undressed* by the base mesh alone, note the donor also
  carries `InitialAppearanceModifiers` (clothing layers) — they still apply. To strip
  clothing, additionally clear that field (separate edit; not required for a working spawn).
```
