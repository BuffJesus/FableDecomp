# Original New Oakvale Intro local symbols

The debug PDBs contain original local-variable names, types and lexical scopes.
These complement the retail byte witnesses; they do not establish retail stack
offsets or permit copying debug-build addresses into the converter.

The retained evidence is under `refs/script_recovery/new_oakvale_intro/`:

- `Ego_r-pdb-locals.tsv`: 100 function symbols, 317 data symbols.
- `FableWin-pdb-locals.tsv`: 133 function symbols, 465 data symbols, richer scopes.
- `pdb_locals.json`: parsed scopes, local symbols, PDB/dump SHA-256 identities and
  qualified-name correspondence for 49 of the converter's 51 functions.

The two unmatched functions are `NOVI_Barrel::Init` and
`NOVI_CreatedBeetle::Init`. The query does not establish why they are absent.
Counts include parameters, compiler locals and constructor/destructor helpers;
they are not counts of unique gameplay variables.

Useful original names include:

| Function | Original names and roles |
|---|---|
| AffairMan Main | `AffairWoman`, `AffairWife`, `AffairConversation`, `badger_string_him`, `badger_string_her` |
| AffairWife Main | `HaranguingConversation`, `husband`, `badger_index`, `badger_string` |
| BarrelMan Main | `guard_hero_start_position`, `guard_man_start_position`, `walk_away_pos`, `return_pos_1`, `return_pos_2`, `WaitTimer` |
| BookTrader Main | `local_position`, `local_movetype`, `local_range`, `YNQAnswer`, `HLStyleConversationIndex` |
| Theresa Main | `leaving_sister_switch`, `gift_given`, `nearby_guards`, `item_presented` |
| Villager Main | `tagCount`, `speechTag`, `speechIdx`, `convIdx`, `suffix` |

Common `seh_me` locals have type
`CScriptGameResourceObjectScriptedThingBase`; `speech_movie` has type
`CScriptGameResourceObjectMovieBase`; `PauseMyEntitiesPlease` has type
`CWideScreenMagicPauseEntities`. Repeated names remain separate scope entries,
even when the compiler reused their stack offsets. A retail correspondence must
still prove construction, uses and destruction on each relevant path.

## Reproduction

`tools/script_recovery/runtime_checks/pdb_locals.cpp` uses the installed Visual
Studio DIA SDK directly through `DllGetClassObject`, without COM registration or
writing to a PDB. Build it with MSVC x86, the DIA SDK include directory, and
`ole32.lib oleaut32.lib`. Invoke the resulting executable with three arguments:

```text
pdb-locals.exe <DIA SDK/bin/msdia140.dll> <PDB> *CQ_NewOakValeIntroScript*
```

Capture stdout as UTF-8 TSV. The extractor exits nonzero for a failed query or no
matches. The parser rejects incomplete dumps and inconsistent scope/count rows.
The successful local build and its log are in `work/pdb_locals_20260913/`.

Rebuild the retained catalog from PowerShell:

```powershell
python tools/script_recovery/catalog_pdb_locals.py `
  --input debug_build/Ego_r.pdb refs/script_recovery/new_oakvale_intro/Ego_r-pdb-locals.tsv `
  --input debug_build/FableWin.pdb refs/script_recovery/new_oakvale_intro/FableWin-pdb-locals.tsv `
  --out refs/script_recovery/new_oakvale_intro/pdb_locals.json
```

The native LLVM PDB reader failed on these two files with a directory-block error;
DIA successfully read both. The earlier exact BarrelMan Main query against
`Ego_d.pdb` returned no match. That result does not claim all Ego_d symbols are
useless.
