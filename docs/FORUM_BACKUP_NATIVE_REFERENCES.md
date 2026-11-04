# Forum backup references for native reconstruction

Inspected 2026-10-04. FMP content is excluded from this assessment.

## Useful materials

- **Community SDK:** runtime interface names, player-field offset candidates, constructor capture hooks and callback examples. Four patch-site instruction anchors match the project's retail executable (SHA-256 `41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`). Evidence: `work/forum-reference-review/sdk-retail-anchors.json`. This confirms those instructions, not complete runtime compatibility or pointer-chain validity.
- **Defsaac1.zip:** 252 ZIP entries; extracted files include 175 `.def` files, three `.tpl` templates and 67 headers. Contains definition syntax, component lists, template inheritance examples, constants and generated mesh/texture/text/particle identifiers. `Defs/creature.tpl` contains `#definition_template`, `Components.Add`, component blocks and typed constructor expressions. `Defs/engine.def` supplies a compact scalar/symbol example. These are useful inputs for recovering `ParseStringForSymbols` and definition loading. Provenance/build correspondence remains unverified.
- **Nine def*.xml variants and Xboxdef29b8.zip:** community Fable Explorer schemas with control IDs, field types, arrays and cross-archive links. Useful for naming binary fields and comparing interpretations. Preserve variants separately; unknown fields and editor hacks are explicitly present.
- **Wiki text:** BIG/BIN/LEV/STB/WAD and related format descriptions. Useful secondary evidence for loaders. Some pages are only stubs/links, so evaluate each page individually.
- **Albion Explorer:** compiled LEV/TNG/WLD/WAD helper assemblies. Potential parser evidence through static inspection; the inspected ZIP contains binaries rather than source.
- **Fable Explorer variants:** compiled archive/content-management libraries, one bundled definition schema and legacy dependencies. Useful reference implementations for static inspection, not engine source.
- **AEResources2999.zip:** editor minimaps, object icons and TNG placement templates. Useful for placement vocabulary/editor presentation; these do not establish engine behavior.
- **tngse416.zip:** 397 entries of TNG placement data. Potential world-loader fixtures after version/modification checks.

## SDK qualification

The retail constructor instructions at `0x0057CDC8` and `0x004E3745` install the SDK's expected CTCHeroStats and CHeroMorphDef vtables. The other patch sites are inside GFMain (`0x00403354`) and CPlayerGui::SetGuildSealLevelUpPos (`0x0043A53A`). Consequently, the SDK callback associated with the GUI site is not yet established as a universal per-frame callback. No SDK binary was executed.

The donor CTCHeroStats header's money/stamina/renown offsets differ from the SDK's retail candidates. Keep those candidates separate until native retail accesses confirm the layout; do not replace generated donor headers using this SDK alone.

## Local artifacts and next use

Archive inventory: `work/forum-reference-review/inventory.json` (32,926 entries).
Selected non-FMP members: `work/forum-reference-review/selected_members.json`.
Extracted references: `work/forum-reference-review/selected/`.
Definition bundle: `work/forum-reference-review/defs/Defs/`.
Existing SDK extraction: `D:/Downloads/FableTLC-SDK-Inspection/`.

Prioritize definition syntax and schema comparisons for the native definition-manager recovery. Legacy editor binaries are lower-priority static references. This assessment does not establish original engine source or a preview-disc image in the archive.
