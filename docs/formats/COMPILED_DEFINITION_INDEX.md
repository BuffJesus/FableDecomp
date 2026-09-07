# Compiled Definition Index

*Generated from installed `data\CompiledDefs` on 2026-07-18.*

## Outputs

- `ghidra_out/installed_game/compiled_game_entries.csv`
- `ghidra_out/installed_game/compiled_script_entries.csv`
- `ghidra_out/installed_game/compiled_frontend_entries.csv`
- `ghidra_out/installed_game/compiled_defs_summary.csv`

## Counts

| File | Entries | Unique Definitions | Named Entries |
|---|---:|---:|---:|
| `game.bin` | 14,761 | 249 | 8,888 |
| `script.bin` | 611 | 3 | 611 |
| `frontend.bin` | 810 | 8 | 810 |

## Chest/Key Proof Points

The project-local dumper found concrete `game.bin` entries for the object definitions that appear in
TNG placement data:

- `OBJECT_CHEST_OPENABLE_TPL`
- `OBJECT_CHEST_OPENABLE`
- `OBJECT_SILVERKEY_CHEST_5`
- `OBJECT_SILVERKEY_CHEST_10`
- `OBJECT_SILVERKEY_CHEST_25`
- `OBJECT_SILVERKEY_CHEST_50`
- `OBJECT_SILVERKEY_CHEST_75`
- `OBJECT_SILVERKEY_CHEST_100`
- `OBJECT_CHEST_REWARD_ON_DEATH`
- `OBJECT_SILVER_KEY`
- `OBJECT_CHOCOLATE_BOX_01`
- `OBJECT_CHOCOLATE_BOX_UNGIVEABLE`

## Editor Meaning

This is the missing definition-resolution layer for the first real gameplay editors:

- chest/reward editor can validate that a placed TNG chest points at a real `game.bin` object.
- object placement UI can offer definitions from compiled data instead of hardcoded lists.
- FSE quest tooling can validate `Quest:GiveHeroObject(...)` and similar calls against real objects.
- Ghidra work can use definition names and object classes as search terms around inventory/reward code.

## Placement Validation

`tools/TestGameplayEntityDefinitions.ps1` joins
`ghidra_out/installed_game/chest_key_reward_entities.csv` against
`ghidra_out/installed_game/compiled_game_entries.csv`.

Current result:

- Checked gameplay entities: 391.
- Unresolved `DefinitionType` references: 0.

That means every chest/key/shop/container-like placement currently indexed from loose TNGs resolves
to a real `game.bin` entry.

## Implementation Note

`tools/bin_dump` is a tiny local .NET wrapper over `SilverChest.Formats.Bin`; it replaces the
hardcoded `ChestDefinitionSearch` executable and reads only from the installed game path.
