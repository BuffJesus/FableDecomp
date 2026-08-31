# Seed native-operation comparison

All six reconstructed packages now have anchor-verified retail lifecycle clusters and normalized
native operation IR. Across the parent lifecycle functions, the comparison finds 35 shared literals
and every reconstructed entity binding:

| Package | Native allocator | Confirmed parent bindings | Confirmed persistence |
|---|---:|---|---|
| GuardianSisterInfo | `0x00E26780` | `MazeAtTavern` | `GuardianSpokeToHero` |
| GuardianSisterInfo2 | `0x00E277E0` | `MazeAtTavern` | `GuardianSpokeToHero` |
| MeetSister | `0x00E29990` | `MeetSisterMessenger`, `MeetSisterSister` | none |
| MazeResearch | `0x00EA8690` | `EmptyGrave`, `HistoryBookcase` | `SwordTaken`, `BookRead` |
| HerosOldHouse | `0x00D8D5A0` | `ExtraBooty`, `FishermansWife`, `GhostFisherman` | `Helping`, `Helped`, `BootyDugUp` |
| GuardianTrophyDealerInfo | `0x00E28CA0` | `GTDI_Maze` | `PieceOver` |

The comparison is deliberately asymmetric. Entity-local persistence such as Hero's Old House
`HaveTalked`, `LeavingHappy`, and `TalkedTo` is not expected in the parent lifecycle cluster; it will
be evaluated after entity-main export is added. A missing parent-cluster literal is therefore not
automatically a semantic mismatch.

Generated artifacts:

- `refs/script_recovery/native_operation_ir/*.json`
- `refs/script_recovery/seed_native_comparison.json`

The operation IR preserves call order and decompile offsets, but current imported function names can
still be false donor names. Addresses, literal references, vtable roles, state offsets, and persistence
keys remain the stronger evidence.
