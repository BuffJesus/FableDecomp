# Grudging book donation and reload

The save config is the successful v54 marathon11 replay; the load config passed
in a fresh v55 process. Start from marathon9's real two-donation checkpoint.
Each runner restores the original saves and closes the game. Run sequentially:

```powershell
$bookReplayRoot = (git rev-parse --show-toplevel).Trim()
$bookReplayTag = Get-Date -Format 'yyyyMMdd-HHmmss'
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_bad_book_save.json --bundle v54 --tag "grudging_$bookReplayTag" --minutes 10 --launch --save "$bookReplayRoot/work/codex_lua_marathon9_20260929/two_donations_checkpoint" --harvest "$bookReplayRoot/work/grudging_replay_$bookReplayTag/save" --close
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_bad_book_load.json --bundle v55 --tag "grudging_load_$bookReplayTag" --minutes 4 --launch --save "$bookReplayRoot/work/grudging_replay_$bookReplayTag/save" --close
```

The first replay verifies the checkpoint, grants the first grudging book using
BooksWanted (ReadGlobalGameData1248, actual index16), clears inventory UI, sets noon,
and talks to Teacher. It requires actual inventory removal, BooksDonated3,
GoodBooksDonated2, both40-entry vectors exactly0/1/16, ReadingBookfalse and closed
movie/cutscene, then saves. The fresh load repeats the state and inventory checks.

Inspect archived logs for BOOK_REQUEST_16, BOOK_ACCEPTED_GRUDGINGLY and the book16
classroom scene, done=true and zero runtime errors. Verify all original save hashes
after each launch and retain the harvested checkpoint separately.

Two initial attempts timed out at the frontend. The successful donation run used
observed ESC skips for startup movies and active capture monitoring until Load Game.
The load run needed no manual keys. The runner supplied inventory, positioning,
clock and dialogue input; no quest-progress flags were written. Donation needed no
manual approach correction and completed in120seconds.

v55 changes only Teacher's local string-map clear before reward macros; DLLs retain
v50 bytes. This replay verifies donation persistence and v55 load compatibility.
It does not execute a hat/key reward branch or prove full collection. See marathon11
native evidence, tests and package/runtime reports for separate offline coverage.
