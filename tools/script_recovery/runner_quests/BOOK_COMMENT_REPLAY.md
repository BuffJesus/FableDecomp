# Book comment and duplicate replays

These configs are byte-for-byte copies of the successful v54 scenarios in
`work/codex_lua_marathon10_20260929`. They grant test inventory and use clock,
positioning and dialogue input. They do not write quest-progress flags.

Start with the real two-donation checkpoint from marathon9. If it is absent,
follow `BOOK_DONATION_REPLAY.md` first. Each command launches a fresh game process,
restores the original saves when finished, and closes the game. Use absolute
checkpoint paths. Run sequentially from the checkout root:

```powershell
$bookReplayRoot = (git rev-parse --show-toplevel).Trim()
$bookReplayTag = Get-Date -Format 'yyyyMMdd-HHmmss'
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_comment_save.json --bundle v54 --tag "comment0_$bookReplayTag" --minutes 9 --launch --save "$bookReplayRoot/work/codex_lua_marathon9_20260929/two_donations_checkpoint" --harvest "$bookReplayRoot/work/comment_replay_$bookReplayTag/one" --close
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_second_comment_save.json --bundle v54 --tag "comment1_$bookReplayTag" --minutes 8 --launch --save "$bookReplayRoot/work/comment_replay_$bookReplayTag/one" --harvest "$bookReplayRoot/work/comment_replay_$bookReplayTag/two" --close
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_two_comments_load.json --bundle v54 --tag "comments_load_$bookReplayTag" --minutes 4 --launch --save "$bookReplayRoot/work/comment_replay_$bookReplayTag/two" --close
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_duplicate_book.json --bundle v54 --tag "duplicate_$bookReplayTag" --minutes 7 --launch --save "$bookReplayRoot/work/codex_lua_marathon9_20260929/two_donations_checkpoint" --close
```

The first comment index is read from `ReadGlobalGameData(1252)` (25 in this
retail data; the comment boundary is35). The second scenario verifies the first
checkpoint before granting index26. It keeps book25 present, so comment1 also
checks that an unseen book takes priority over an already-seen one. Both books
must remain in inventory. BooksDonated and GoodBooksDonated stay2; BookDonated
contains exactly0/1, while BookOwned ends with exactly0/1/25/26. Both40-entry
vectors, scene closure, ReadingBook and LastBookRequested are checked.

The duplicate scenario deliberately reloads the earlier two-donation checkpoint,
without retained comment books. It grants an extra book0 and uses the runner's
read-only per-frame probe to observe a movie before requiring its closure. The
duplicate must remain present and all donation state must stay unchanged.

Inspect the archived game logs as well as `finished done=true`: comment runs
must show BOOK_COMMENT_0 and BOOK_COMMENT_1 respectively; duplicate rejection
must show BOOK_NOT_FOUND. Require zero runtime errors and verify original save
hashes after each run. `runtime_validation.json` records the accepted runs.

Startup movies sometimes need ESC. Book grants can open a tutorial; the grant
step is separate so the runner clears it before approaching Teacher. The accepted
second-comment run additionally needed `game_input('hold W 250')`, then
`game_input('key TAB')`, when the teacher was targeted but the conversation had
not started. That assistance is recorded; the recipe is not certified fully
hands-free. Use observed keyboard input without a competing Channel writer.
The first unsuccessful combined grant/talk attempt is preserved in the journal.

These checks do not establish grudging donations, hat/key rewards, full collection
or out-of-hours dialogue. Both DLLs are unchanged fromv53; only Teacher Lua changes
in v54. Native operand evidence and offline comparisons are in the marathon10
scratch directory and the 2026-09-29 Lua journal.
