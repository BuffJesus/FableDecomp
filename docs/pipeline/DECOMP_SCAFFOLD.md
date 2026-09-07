# Decomp source-tree scaffold (task #12)

*2026-07-21.* The debug PDBs do NOT carry per-function **source-file/line** records into the
Ghidra DB (probed via SourceFileManager: 0 rows on ego_r.exe). So the original TU/file tree
cannot be reconstructed directly. **Fallback (correct proxy):** group by CLASS — one class ~= one
module/TU-cluster — using the demangled `?Method@Class@@` names.

- **3,433 class-modules** identified across the donor. Index: `ghidra_out/decomp_module_scaffold.tsv`.
- Largest (decomp-order candidates): CGameScriptInterface (935), CEditControlCentre (252),
  NGlobalConsole (223), CWorld (206), CWorldMap (200), CThingCreatureBase (164), CPlayer (148),
  CTCInventoryBase (140), CComponent (137), CUserProfileManager (123), CThing (118), CQuestManager (102).
- Pair with `fable_types.h` (member layouts) + `engine_api.tsv` (signatures) to seed each class's
  header. This is the skeleton a functional decomp is organized into (FULL_DECOMP bar-2), now
  compilable against the newly-installed VC7.1 oracle.

For a true file/line tree, a follow-up would parse the raw PDB (cvdump / DIA) outside Ghidra — the
line info exists in the .pdb, just isn't imported. Deferred; class-module grouping is sufficient to
start the module-by-module lift.
