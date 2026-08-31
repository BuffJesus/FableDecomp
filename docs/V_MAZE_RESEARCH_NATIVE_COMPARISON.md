# `V_MazeResearch` native/Lua comparison

## Result

The reconstructed package is structurally close, but is not yet a verified retail port. Native PC
evidence establishes the quest lifecycle, both entity mains, persistence order, and the cutscene flag
helper. The strongest discrepancy is sword reveal timing.

## Address map

| Role | Retail address |
|---|---:|
| Allocator | `0x00EA8690` |
| Constructor | `0x00EA76E0` |
| Init | `0x00EA7750` |
| RegisterMain | `0x00EA7760` |
| Main | `0x00EA7800` |
| HistoryBookcase Main | `0x00EA7B40` |
| OnPersist | `0x00EA80A0` |
| EmptyGrave Main | `0x00EA8130` |
| UnLimboSword helper | `0x00EA85E0` |

The imported constructor name at `0x00EA76E0` is a false donor-symbol assignment. The registry
allocator, allocated size, assigned vtable `0x012EE444`, and vtable lifecycle slots establish that
this is the `V_MazeResearch` object cluster.

## Confirmed matches

- Native state bytes are `SwordTaken` at `+0x48` and `BookRead` at `+0x49`; both initialize false.
- Persistence transfers `SwordTaken` first and `BookRead` second.
- Main binds `EmptyGrave` and `HistoryBookcase`, finalizes bindings, and sets objective 01.
- The bookcase transitions the marker to `EmptyGrave`, sets `BookRead`, and displays the four journal
  entries with a click-past wait after each.
- The grave gate is `BookRead && !SwordTaken && PostSavePosition > 1700 && JackBossBattleResult == 2`.
- Morality selects `CS_GET_SWORD_OF_AEONS_SAINT` versus `CS_GET_SWORD_OF_AEONS_REDEEM`.

## Material discrepancy: cutscene flags

Retail does not immediately reveal the sword before running the cutscene. It creates a shared flag
map containing `UNLIMBO=false`, spawns `UnLimboSword`, and starts the macro. The helper yields until
the macro changes `UNLIMBO` to true, then removes the sword from limbo and invokes a second visibility
transition at CGSI vtable offset `0x7F8`.

The reconstructed Lua immediately performs its unlimbo/alpha operations before `PlayCutscene`. Its
third argument `{ UNLIMBO = false }` expresses the intended flag map, but current ForgeFSE
`LuaQuestState::PlayCutscene` passes a null flag-map pointer into `RunCutsceneMacro_Func`. Therefore
the native timing cannot currently be represented faithfully.

ForgeFSE needs shared cutscene flag-map support plus a way for a Lua thread to observe flag changes.
The semantic name of the native `0x7F8` operation remains unverified and must not be guessed.

The machine-readable evidence is in `refs/script_recovery/native_ir/V_MazeResearch.json`.
