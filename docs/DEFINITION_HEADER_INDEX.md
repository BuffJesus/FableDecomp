# Definition Header Index

*Generated from installed `data\Defs\*.h` and `data\Defs\RetailHeaders\*.h` on 2026-07-18.*

## Outputs

- `ghidra_out/installed_game/header_enums.csv`
- `ghidra_out/installed_game/header_enum_counts.csv`
- `ghidra_out/installed_game/header_enum_summary.txt`

## Counts

- Enum constants: 63,540.
- Enums: 63.

Largest enums:

| Enum | Count | Editor Use |
|---|---:|---|
| `EGameText` | 26,807 | text picker, dialogue/object labels, validation |
| `ELipSync` | 12,134 | dialogue/lipsync browser |
| `ELipSync2` | 5,310 | dialogue/lipsync browser |
| `EEngineGraphic` | 3,683 | mesh/graphic references |
| `EAnimType2` | 3,435 | animation picker |
| `EMeshType2` | 3,401 | mesh picker |
| `ELipSync4` | 2,769 | dialogue/lipsync browser |
| `ETextGroup` | 2,105 | grouped text browser |
| `EParticleEmitter` | 1,165 | effects/particle picker |
| `EFrontEndGraphicBank` | 394 | frontend/UI asset picker |

## Editor Meaning

This is the first pass at a definition/name catalog. It should back:

- asset pickers for meshes, textures, animations, particles, text, and lipsync entries.
- validation for TNG `DefinitionType` and CTC fields that point at enum-backed resources.
- Ghidra datatype imports and enum application in decompiled functions.
- cross-checks between EgoCore BIG entries and retail generated headers.

## Next

The stronger version is to parse `CompiledDefs\names.bin` and `CompiledDefs\game.bin` with
`SilverChest.Formats.Bin`, then join concrete BIN entries to these enum/header names. The local
`ChestDefinitionSearch` proof exists, but it has hardcoded paths, so use a project-local extractor
before relying on it.
