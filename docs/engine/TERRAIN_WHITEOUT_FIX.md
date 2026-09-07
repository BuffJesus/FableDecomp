# Custom-region terrain white-out — root cause + fix

*From engine map-open decomp analysis 2026-07-21 (`terrain_whiteout_decomp.txt`, 55 fns),
cross-checked against the write-side RE in NATIVE_TERRAIN_BAKE.md. HIGH confidence.*

## It is NOT a reference-resolution bug — three long-standing theories REFUTED
- **REFUTED: by-name bank miss** (StdMap_FindLE returns sentinel -> garbage offset). Forge
  already writes the `__STATIC_MAP_COMMON_HEADER__` name->offset manifest byte-exact (STB
  entry 426, full-path key), so `std::map::find` HITS ForgeTest's own chunk offset. No miss.
- **REFUTED: InfoBlock BankFileIndex (426) points at the wrong bank.** `CreateNewBankMap`
  @0x009d58d0 takes NO name and does NO FindIndexBySymbol — it binds the manifest-resolved
  entry by OFFSET. The InfoBlock BankFileIndex field is written for parity but **never read**
  on the load path. Red herring.
- **REFUTED: theme index out of range -> null bank -> white.** Landscape textures are baked
  **INLINE** into the chunk (CEngineTexturePalette, DXT1, zero external name-strings —
  stbvalidate confirms). Themes resolve against the **global, install-wide ENGINE_THEME**
  table (`BuildThemes` @0x00b687d0), which is name-agnostic and identical for every map on the
  install. The InfoBlock has no theme field; the .lev's 256 ground-theme names are author-time
  only and degrade to a walk-default, never a white.

So a byte-identical donor clone on the same install resolves the exact same global themes and
carries its own inline textures. **Registration and refs resolve natively.**

## ROOT CAUSE — content mismatch, not reference mismatch
The ForgeTest chunk is a byte-clone of donor **Darkwood9_Leadout_01** whose inline per-patch
palettes, quadtree AABBs, and compressed mesh bodies were baked for the DONOR's heightfield and
coords. Forge only rebases placement floats + the 5 absolute-offset dwords (+0x20,+0x24,+0x58,
+0x5C,+0x71) and rewires the background-LOD quadtree. It does **not regenerate the inline
landscape mesh/palette** for ForgeTest's authored heights, because the LZO1X-999 and
CRangeCompressor mesh-body codecs are un-ported (stbbake.cpp emitChunk = RawPassthrough only).
Net: refs resolve; the authored hill still needs a real mesh bake.

## Can forge author a resolvable chunk without the editor? PARTIAL
- **YES for reference RESOLUTION** — no editor bake needed: textures are inline (nothing to
  register), themes are global (nothing per-map to emit), and the one load-bearing external ref
  (the name->offset manifest) forge already emits byte-exact. The donor-cloned terrain re-serves
  with correctly-resolving refs. (This also fixes the mandatory-STB-entry crash.)
- **NO for authoring NEW terrain content** (a custom hill) — forge cannot yet GENERATE
  ForgeTest's own inline mesh + palettes for authored heights. That needs the un-ported
  LZO1X-999 + CRangeCompressor mesh codecs + CTexture::SaveToDataStream, then reproducing
  `CEngineMap::GenerateStaticMapEntry` (write-side @0x02d66c80 UpdateStaticMapPass1). Until then,
  the interactive editor bake (UnifiedFable/ego_r, per BAKE_FORGETEST_TERRAIN.md) is the path.

## Fix
1. **Stop chasing ref fixes** — do NOT edit InfoBlock BankFileIndex, add a BuildThemes clamp, or
   a theme registrar, or ship the FSE map-open name-alias. All are red herrings / unnecessary.
2. **Promote the proven Python STB emission to native `forge::stb::save()`** (stb.cpp): append
   entry row {426, type0, full-path name, size, page-aligned offset, crc}; bump COMMON_HEADER
   398->399 + append (nameNUL)(SLONG relOffset), re-deriving all StaticMap absolute offsets;
   rebase the 5 abs dwords. Keep `emitChunk` in RawPassthrough for the load-safe donor-clone.
3. **To render an AUTHORED hill**: port LZO1X-999 + CRangeCompressor mesh-body codecs +
   CTexture::SaveToDataStream, then reproduce GenerateStaticMapEntry's inline-palette write —
   OR use the interactive editor bake until those land. This is the "own the terrain bake"
   engineering task (backlog ranks 3-4 territory), not more RE.

## Validation (byte-level, no in-game run)
`forge stb list` on the emitted STB: 399 static maps, entry 426 = full-path ForgeTest.lev at the
correct page-aligned offset. Byte-identity gate: every untouched payload byte identical vs source
(398 donor chunks reserialize==raw). `forge stbvalidate` on the ForgeTest chunk: S1 coverage/
roundtrip PASS.

## Bottom line
The donor-clone path was never a resolution problem — it re-serves the DONOR terrain with
resolving refs (so it should render Darkwood9's terrain, textured; any residual void is a
placement/quadtree rebase bug in the clone, a separate small fix). Rendering YOUR custom heights
is gated on native inline-mesh bake (codec port) or the editor bake — nothing else.
