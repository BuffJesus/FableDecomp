# Clothing-strip RE — workflow conclusion + EMPIRICAL CORRECTION (2026-07-24)

RE'd the appearance/clothing path via the Claude Workflow loop (`appearance-clothing-re`:
5 decode agents → synthesis → adversarial verify, verdict **CONFIRMED 7/0/3**) to find how to
show a custom creature's bare custom body instead of the trader's clothing. The workflow produced
a correct-but-misapplied answer; an empirical scan of retail game.bin corrected the target. Both are
recorded here because the correction is the load-bearing fact.

## What the workflow got RIGHT (verified, keep)
- `CCreatureDef` has an `InitialAppearanceModifiers` field: crc0 tag **`0x8F20181F`** (LE bytes
  `1F 18 20 8F`), on-disk `[u32 tag][u32 count][count × i32 entryId]`, each id → a `CAppearanceModifierDef`.
  Serialized by `CCreatureDef::Transfer` @ `0x00675E20` at member `+0x54` (the vector<int> immediately
  after `InitialActions` @ `+0x48`; helper pair `0xFFDF0C55`/`0xFFDF0C87`). Triple-corroborated
  (disasm structure + `ghidra_out/def_schema.json:18688` type `Vector_J` + community
  `chocolatebox_def.xml:5107` Control id `1F18208F`).
- Field tags are stored **inline** as LE u32 immediately followed by the value (same premise the shipping
  `03_inplace_modelid.cpp` uses to find `Graphic` = `0x2E6B63C8`).
- A `CAppearanceModifierDef` carries a `Graphics` list: crc0 tag `0x5C9D0167`, `u32 n` + `n × 24-byte
  CEntry {i32 statA, i32 statB, i32 modelId, f32 scale, f32 appear, f32 disappear}`.

## The EMPIRICAL CORRECTION (retail game.bin scan — supersedes the strip recipe's target)
The workflow assumed the trader's clothing comes from `InitialAppearanceModifiers`. **It does not.**
Scanning all 14,761 retail entries (`work/appearance_re/*.cpp`):
- **`InitialAppearanceModifiers` lives on the `CCreatureDef` *sub-def*, not the `CREATURE` wrapper.**
  A `CREATURE` entry (e.g. `CREATURE_TRADER_01`, def type `CREATURE`) is a wrapper that references a
  component `CCreatureDef` (TRADER_01 → index **10744** at CREATURE-payload offset **297**; also
  `CAppearanceDef` **10745** @21, `CEnemyDef` **10746** @189). The tag `0x8F20181F` appears in **330
  `CCreatureDef` entries and ZERO `CREATURE` entries** — so the earlier attempt to scan the CREATURE
  payload for it correctly found nothing.
- **TRADER_01's `CCreatureDef`[10744] has `InitialAppearanceModifiers` present @ offset 461 but
  `count = 0`.** The trader ships with an EMPTY modifier list. Therefore clothing is NOT layered via
  this field for traders — emptying it is a no-op.
- ⇒ **`count = 0` is a proven engine-valid, retail-shipped state** — this resolves the workflow's one
  residual risk (whether the load helper tolerates an empty vector): retail itself ships it.
- **The trader's real appearance is `CAppearanceDef`[10745]** — an **18,103-byte** structure containing
  its own `Graphic` tag and ~20 graphics.big mesh-id-range u32s (the composited body/clothing meshes).
  This is the actual lever for a bare-body result, and it needs its own dedicated RE pass (structure not
  yet mapped) before any safe edit.

## Consequence for the custom-NPC work
- The `Graphic.modelId` repoint (Track A/B) sets the **base body** mesh. Whether the custom body shows
  bare or with layered meshes depends on `CAppearanceDef`[10745], NOT `InitialAppearanceModifiers`
  (which is already empty). **Do not implement an InitialAppearanceModifiers strip for traders — it
  would do nothing.**
- **Next step is observational, not speculative:** deploy Track B, spawn `CREATURE_MESHY_HUNTER`, and
  SEE what renders. If the custom body is obscured by `CAppearanceDef`[10745] meshes, RE that 18 KB
  `CAppearanceDef` structure next (a focused workflow on `CAppearanceDef::Transfer` `0x0046a174` region
  + the on-disk 10745 layout) to learn how to null/repoint its mesh list. If it already shows the bare
  custom body, no strip is needed.
- For a creature that DOES use `InitialAppearanceModifiers` (330 CCreatureDefs do, count>0), the
  workflow's strip recipe is valid — edit the **`CCreatureDef` sub-def** (not the CREATURE wrapper):
  scan its payload for `1F 18 20 8F`, then either set count=0 + drop entry bytes (via
  `setEntryData`, safe — retail ships count=0) or size-preserving repoint each id to
  `NULLDEF_CAppearanceModifierDef`.

*Artifacts: `work/appearance_re/` (bundles, `mechanism_md.txt`, `verify.txt`, and the scan tools
`scan_iam.cpp`/`where_iam.cpp`/`check_component.cpp`/`dump_appdef.cpp`). Full workflow synthesis in the
task output journal.*
