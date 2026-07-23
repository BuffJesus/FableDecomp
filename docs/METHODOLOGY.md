# RE methodology (inherited from Fable2RE)

*How Fable2RE actually drove Ghidra, distilled for reuse here. The mechanics transfer verbatim;
only the loader (no XEX) and the absence of a recomp track differ.*

## The labelling pipeline (the backbone)
Fable2RE reverse-engineered 21 MB of code by iterating a **TSV-driven labelling loop**, keeping the
Ghidra database *reproducible from text*:

1. **Anchor** a subsystem via one of: a diagnostic/asset **string** (`FindStrXref`), a named
   **import** (`.idata`), an **FSE call site** (a wrapper that calls a fixed engine address), a
   **Lua-native table** (`FindLuaNatives*`), or a **vtable/RTTI** class.
2. **Fan out** — follow xrefs/callees; propose names.
3. **Record names as a TSV** (`address<TAB>name<TAB>...`) in `ghidra_out/labels_<subsystem>.tsv`.
4. **Apply** with `LabelApply.java` (bulk). Re-dump the current state with `LabelDump.java`.
5. **Decompile to files** (`DecompFuncs.java`) for offline reading + documentation.
6. **Commit the TSV**, not the `.rep`. The DB is rebuildable: fresh `analyzeHeadless` import +
   `LabelApply` on the TSVs. (Fable2RE tracked `labels_session_*.tsv`, `labels_loaders_*.tsv`,
   `lua_natives*.tsv`, `labels_subsys.tsv`, etc.)

This beats hand-renaming in the GUI: it's diffable, reviewable, scriptable, and survives a re-import.

## AI-driven RE via GhidraMCP
- Ghidra runs the **GhidraMCP** HTTP plugin (port 8089); Claude drives it through the MCP bridge
  (`decompile`, `rename_function`, `set_function_prototype`, `get_xrefs`, `search_strings`, P-code
  emulation, data-flow — ~249 tools). This is how the bulk of the naming/decompiling actually happens.
- Batch aggressively: ask for a whole subsystem's decompilation + proposed names in one pass, then
  review, then apply via TSV.

## Evidence discipline (non-negotiable, from both projects' CLAUDE.md)
- **Never assert from a single source.** Cross-check every claim against ≥2 of: FSE manifest,
  fabletlcmod wiki byte layout, live behaviour (x64dbg / FSE Lua probe), sibling Fable2 findings.
- **Log every change** for rollback; the TSV is the change log.
- **Loop prevention:** don't repeat a failing call >2× with the same args; after 2 failed attempts
  at a goal, stop and document the gap.
- **Document as you go:** `FINDINGS.md` (technical truth, cited), `SYSTEMS_ANALYSIS.md` (per-subsystem
  map + moddability verdict), `PROGRESS.md` (chronological), `HANDOFF.md` (resume point).

## Techniques worth stealing from Fable2RE specifically
- **RTTI/vtable first** — on an MSVC PE this names a large fraction of classes for free (Fable2 lacked
  clean RTTI; TLC's PC MSVC build should have it — a bonus over the Xbox binary).
- **`_PDATA` for function bounds** — TLC keeps the Xbox-style `_PDATA` section; `ParsePdataInBlobs`/
  `DumpAllPdata` use it to recover accurate function boundaries (Ghidra sometimes mis-splits without it).
- **String-hash resolvers** — Fable's resource system indexes by string hash (the Fable2 `luaV_gettable`
  chase found the hash const `0x5BD5E995` = a FNV/Murmur-style seed). Expect the same in TLC; naming
  the hash function unlocks the whole resource-lookup subsystem.
- **The "custom-compiled oracle"** (Phase 4) — compile a known function with the era MSVC (VC7.1 here)
  and diff its codegen against the binary to validate a reversed function.
- **Sibling cross-reference** — TLC (Fable 1 PC) and Fable 2 (Xbox) share a Lionhead engine lineage;
  when a TLC subsystem is opaque, the Fable2RE `SYSTEMS_ANALYSIS.md` / `FINDINGS.md` for the same
  system is a strong prior (e.g. Will/spells, Babel text, appearance-GDB, Havok/anim).
