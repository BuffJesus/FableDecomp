# Fable TLC save format — doc index

The `.sav` ("FableSave!") format is documented across five files (read in this order):

| doc | scope |
|---|---|
| `SAVEGAME_FORMAT.md` | container: magic, header dwords, zlib chunks, CRC primitive, persist-stream `[tag][value]` grammar (field-tag seed **0**) |
| `SAVE_HEADER_MAP.md` | HEADER + PLAYER section tag→name map (world name, PlayerCharacterUID, …) |
| `SAVE_HERO_STATS.md` | hero CTCHeroStats field map (Money/Morality/Age/Fatness/RenownLevel; experience-pool tags) |
| `SAVE_ENTITY_GRAPH.md` | SAVED_ENTITIES entity graph: cell records (36-byte descriptor, §9.5.1), entity/component framing, inventory grammar (§9.5.2), QUESTS/FACTIONS/REGIONS, the productized editor (§9.5.3) |
| `SAVE_SIG_ENFORCEMENT.md` | load-time signature verdict: trailer CRC (seed 0, over `file[0:total_data_len]`), exact 0x4B000 size, zero padding — ENFORCED unconditionally in retail |
| `SAVE_WRITER.md` | write-side recipe validation (gates A–D) |

Tooling: `tools/save_edit.py` (container read/verify/rebuild + hero report/set-gold/set-stat/
set-qty/add-item, all re-signed), `tools/parse_save_entities.py` (read-only graph walker +
quest-flip PoC), `tools/save_tools/savecrc.py` (CRC helpers).

Key invariants for any writer: re-sign trailer (seed-0 CRC), keep file exactly 307200 bytes,
zero slack; when editing a SAVED_ENTITIES cell also patch the cell descriptor
`recLen/clen/ulen` (see SAVE_ENTITY_GRAPH.md §9.5.1 CORRECTION), the section length dword, and
`chunk1_ulen`.
