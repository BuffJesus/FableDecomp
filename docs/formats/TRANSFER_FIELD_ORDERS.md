# Transfer<T> field orders — extraction for `jamen/fable-defs`

**Artifact:** `refs/transfer_field_orders.json` — **268** def classes, **5,133** named+typed+ordered
field controls. Generated 2026-08-10; sub-component collection + base-class flatten + namespaced-def
recovery landed by 2026-08-11.

**Status: complete.** **267/268** classes exact-match fable-defs field order. The single remainder
(`COpinionSourceDef`) is an array-unroll artifact, not a data gap — see the residual section below.

## Why this exists

`jamen/fable-defs` (the def compiler now inside EgoCore) derives each def class's binary layout from
the game's `CDefBase::Transfer` functions: *"the order of `Transfer<…>` calls IS the binary field
order,"* with `Transfer<long>⇒i32`, `Transfer<float>⇒f32`, `Transfer<CDefString>⇒DefString`. Its
`AGENTS.md` names its field-order oracle as a private decomp `~/git/fable-decomp`, marked
**"Absent at time of writing."** We *have* that oracle. This file is our contribution back.

See docs/journal/2026-07/EGOCORE_ASSESSMENT_20260731.md §B5 for the relationship.

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
- **267 / 268 classes** are an exact order-preserving supersequence of fable-defs' `#[def("…")]`
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

## Base-class flatten — DONE (2026-08-11)

The 10 base-marker classes (9x `CThing*Def` + `CTavernGameShoveHaPennyDef`) are now **exact**.
`tools/transfer_extract/tx_flatten.py` (pure JSON transform, no disasm) recursively expands each
`{"type": "base:<Class>"}` marker in place into that base's field list, tagging every inlined field
with `"from_base": "<Class>"` for provenance. The base classes (`CThingBaseDef` @ 32 fields,
`CTavernGameDef`) remain as their own top-level entries. `_meta.base_fields_inlined` records this.
Re-run: `python tools/transfer_extract/tx_flatten.py [--write]` (dry-run report by default).

## Residual: `COpinionSourceDef` — array-unroll, NOT a data gap

The one class still `partial`. All field **names** are present in our order; the subsequence check
fails only because fable-defs' `opinion_source.rs` unrolls two array fields — it emits
`#[def("BinaryReaction")]` ~79x and `#[def("BinaryOpinion")]` 5x, one `#[def]` per array element —
whereas our linear disasm sees the single `CPersistContext::Transfer` call inside the emitting loop
**once** (`BinaryReaction`:bool@off11 loop-temp, `BinaryOpinion`:f32). The flag reads
`partial(array-unrolled:BinaryReactionx79,BinaryOpinionx5)`. Unrolling would require recovering the
loop trip count from the struct; the compressed single entry is the faithful Transfer-order record.

## Namespaced UI/dialogue defs — RECOVERED (2026-08-11)

The 6 defs previously "not located" were simply **namespaced** — the extractor's discovery regex
required `@@` immediately after the `Def` name, so it skipped symbols carrying `@N<Namespace>@@`:
- `NUISystem`: `CUIDef` (@0x6758f0, 109 fields), `CUIIconsDef`, `CUIMiscThingsDef` (133),
  `CUIStateDef`, `CUILocaleGraphicsDef`.
- `NSpeechGainManager`: `CDialogueLayerDef` (@0x473179).

`tx_extract.py`'s regex now accepts an optional namespace (`_CLS_RE`), and `tx_ui_merge.py` merges
these 6 into the JSON keyed by the **binary** class name with a `"namespace"` field. Where the binary
casing differs from fable-defs' struct (`CUIDef` vs fable-defs `CUiDef`), the entry carries a
`"fable_defs_class"` alias so consumers/`tx_flatten` join correctly. All 6 score **exact**.

## Resume / next steps (core deliverable is DONE — these are optional polish)

1. ~~**Base-chain flatten:**~~ **DONE 2026-08-11** (`tx_flatten.py`; 251→261 exact). See the
   "Base-class flatten" section above.
2. ~~**6 UI defs:**~~ **DONE 2026-08-11** (`tx_ui_merge.py`; they were namespaced, not missing). See
   "Namespaced UI/dialogue defs" above.
3. ~~**Ship it:**~~ **DONE 2026-08-11** — issue opened: https://github.com/jamen/fable-defs/issues/1
   (offers the JSON; awaiting jamen's preferred delivery form — gist/PR/file). NOTE: a bulk gist upload
   of the JSON was blocked by the harness data-exfil guard; deliver the file only with explicit user
   sign-off (or hand it off manually). Await maintainer reply before sending the 456KB artifact.

## How to regenerate

`python tools/transfer_extract/tx_extract.py --all` (writes `scratchpad_out.json`; regex now also
captures namespaced defs) → move to the scratchpad as `transfer_orders.json` →
`python tools/transfer_extract/tx_finalize.py` (writes `refs/transfer_field_orders.json`) →
`python tools/transfer_extract/tx_ui_merge.py --write` (merges the 6 namespaced UI/dialogue defs) →
`python tools/transfer_extract/tx_flatten.py --write` (inlines base markers + re-scores). `tx_compare.py`
prints the fable-defs agreement report. Needs `debug_build/ego_r.exe` + `ghidra_out/egor_pdb_names.tsv`
+ the `fable-defs` clone at `C:\Users\Cornelio\Documents\EgoCoreInspect\fable-defs`. `pip install capstone`.
