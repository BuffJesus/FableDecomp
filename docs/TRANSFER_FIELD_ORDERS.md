# Transfer<T> field orders — extraction for `jamen/fable-defs`

**Artifact:** `refs/transfer_field_orders.json` — 262 def classes, **4,465** named+typed+ordered field
controls. Generated 2026-08-10; sub-component collection pass landed same day.

**Status: essentially complete.** 251/262 classes exact-match fable-defs field order. See
"Resume / next steps" at the bottom for the small optional remainder.

## Why this exists

`jamen/fable-defs` (the def compiler now inside EgoCore) derives each def class's binary layout from
the game's `CDefBase::Transfer` functions: *"the order of `Transfer<…>` calls IS the binary field
order,"* with `Transfer<long>⇒i32`, `Transfer<float>⇒f32`, `Transfer<CDefString>⇒DefString`. Its
`AGENTS.md` names its field-order oracle as a private decomp `~/git/fable-decomp`, marked
**"Absent at time of writing."** We *have* that oracle. This file is our contribution back.

See docs/EGOCORE_ASSESSMENT_20260731.md §B5 for the relationship.

## Source & method

- **Binary:** `debug_build/ego_r.exe` — the Fable Anniversary (2013-02-25) **debug build**, ImageBase
  `0x400000`, which ships full PDB symbols (`ghidra_out/egor_pdb_names.tsv`, demangled).
- **Method** (`scratchpad/tx_extract.py` → `tx_finalize.py`): linear-disassemble each
  `?Transfer@C*Def@@UAEXAAVCPersistContext@@@Z`. Every call to
  `CPersistContext::Transfer<T>(const char* name, T& field, const T& deflt)` is one `game.bin` field
  control, emitted **in call order**. The **name** is the 1st-arg string literal (`push <rdata ptr>`);
  the **wire type T** is the template arg of the call-target symbol.
- **Wire-type legend:** `J`=i32(long) · `H`=i32(int) · `_N`=bool · `M`=f32 · `N`=f64 ·
  `K`=u32(ulong) · `I`=u32(uint) · `F`=i16 · `G`=u16 · `E`=u8 · `D`=i8 ·
  `VCCharString`=String/DefString · `VCDefIndex`=DefIndex · `W4X`=enum X (i32 wire) ·
  `TransferVector<T>`=Vec<T>.

## Validation (why it's trustworthy)

- **`CChestDef` 6/6** vs fable-defs `chest.rs` — names, types, and order all match, **including
  `OpenerObject` at field offset `0x34`**, which we had independently RE-verified (CLAUDE.md chest
  facts). The offset falling out of a totally separate derivation cross-checks the extractor.
- **`CAbilityDef`** → `Ability : enum EHeroAbility` = fable-defs `ability.rs` (`ability: HeroAbility`).
- **211 / 262 classes** are an exact order-preserving supersequence of fable-defs' `#[def("…")]`
  order. Each class carries `"fable_defs_order_match": "exact" | "partial(...)"`.

## Sub-component collection fields — DONE (2026-08-10)

Initial pass matched only `Transfer/TransferVector/TransferEnum/...`. Turned out the `Vec`-of-sub-def
collection fields (`Abilities`, `Waves`, `Creatures`, `Meshes`, ...) **are transferred inline** in
`Transfer()` via `CPersistContext::TransferVectorOfSubComponents<T>(const char* name, vector<T>&)` —
name string and all — just under a symbol family the regex didn't catch. Verified on
`CArenaRoundDef::Transfer @0xf93320`: `NumWaves` (i32) then `Waves` (`vector<subdef CArenaWaveDef>`).
Fix widened the family regex to `\?\?\$(Transfer\w*)@...@CPersistContext@@` (see
`tools/transfer_extract/tx_extract.py`). Result: exact matches **211 -> 251**, +92 collection fields
recovered. **No constructor pass was needed.**

## Residual 11 "partial" classes — NOT a data gap

They differ only by **base-class flattening**: fable-defs inlines base fields into each concrete class,
while we chain via a `base:<Class>` marker so those fields live under that base's own record.
- 9x `CThing*Def` (`CThingObjectDef`, `CThingCreatureDef`, ...) — the missing list
  (`Components`/`PersistenceFlags`/`Health`/...) is the `CThingBaseDef` base; each carries a leading
  `base:CThingBaseDef` entry.
- `CTavernGameShoveHaPennyDef` — same, base tavern-game class.
- `COpinionSourceDef` — all fields present, only a minor order tie-break.

6 fable-defs classes have no matching `*Def::Transfer` symbol (UI/frontend: `CUiDef`, `CUiIconsDef`,
`CUiMiscThingsDef`, `CUiStateDef`, `CUILocaleGraphicsDef`, `CDialogueLayerDef`) — different transfer
signature; not yet located.

## Resume / next steps (core deliverable is DONE — these are optional polish)

1. **Base-chain flatten (nice-to-have):** post-process `refs/transfer_field_orders.json` to expand each
   `base:<Class>` marker inline (recursively) so consumers get a fully-flattened list matching
   fable-defs 1:1. All base classes are already in the JSON — pure JSON transform, no new disasm. Takes
   the 11 partials to exact.
2. **6 UI defs:** grep `ghidra_out/egor_pdb_names.tsv` for `Transfer.*CUiDef` etc.; they likely use a
   non-`UAEXAAVCPersistContext` signature. Low value (frontend-only).
3. **Ship it (session option (b), not yet done):** attach `refs/transfer_field_orders.json` to jamen or
   open an issue on `jamen/fable-defs` referencing the per-class `fable_defs_order_match` flags.

## How to regenerate

`python tools/transfer_extract/tx_extract.py --all` (writes `scratchpad_out.json`) → move to the
scratchpad as `transfer_orders.json` → `python tools/transfer_extract/tx_finalize.py` (writes
`refs/transfer_field_orders.json`). `tx_compare.py` prints the fable-defs agreement report. Needs
`debug_build/ego_r.exe` + `ghidra_out/egor_pdb_names.tsv` + the `fable-defs` clone at
`C:\Users\Cornelio\Documents\EgoCoreInspect\fable-defs`. `pip install capstone`.
