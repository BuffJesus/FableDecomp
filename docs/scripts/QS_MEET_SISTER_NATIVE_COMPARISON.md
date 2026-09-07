# QS_MeetSister native comparison

Retail evidence identifies the quest allocator at `0x00E29990`, lifecycle vtable at `0x012E2194`,
main at `0x00E28DE0`, messenger main at `0x00E292C0`, and sister main at `0x00E295E0`.

The reconstructed package agrees with the retail main on the two binding names
`MeetSisterMessenger` and `MeetSisterSister`, the objective
`TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY`, and the level pair `HauntedHouse` / `BarrowFields`.
Retail entity code also anchors the expected `ArenaExterior`, `CS_ARENA_LEAVE_THERESA_01`, and
`CS_ARENA_LEAVE_THERESA_02` flow.

This is strong structural corroboration, but not yet a `verified-port`. The entity control flow,
cutscene participant maps, cleanup ordering, quest completion/deactivation, fade behavior, and
failure paths still need operation-level native IR and trace comparison. Until then this package is
eligible for ForgeFSE shadow execution only.
