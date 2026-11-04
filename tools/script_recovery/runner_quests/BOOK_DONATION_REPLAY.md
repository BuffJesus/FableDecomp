# Book donation persistence replay

Validated with unchanged candidate v53 on 2026-09-29. These three scenarios
donate books 0 and 1 through the teacher's dialogue, verify inventory removal,
and check both 40-entry flag vectors before saving and after fresh-process loads.
Each donation grants one test book. Travel, clock and dialogue input are assisted;
quest progress and donation flags are never injected.

Run from the repository root. Keep checkpoints in a new workspace directory:

```powershell
$donationRun = Join-Path (Get-Location) 'work/book-donation-replay'
$firstDonation = Join-Path $donationRun 'first'
$secondDonation = Join-Path $donationRun 'second'

python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_donation_save.json --bundle v53 --tag book_donation_save --minutes 12 --launch --save adult_maze_completed_2026-09-23 --harvest $firstDonation --close
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_second_donation_save.json --bundle v53 --tag book_second_donation_save --minutes 10 --launch --save $firstDonation --harvest $secondDonation --close
python -u -m tools.script_recovery.ingame_runner tools/script_recovery/runner_quests/book_collecting_two_donations_load.json --bundle v53 --tag book_two_donations_load --minutes 3 --launch --save $secondDonation --close
```

Run each command only after the preceding run succeeds and its checkpoint exists.
The runner restores the staged-over profile and AutoSave files after closing the
game. The validated runs also compared all 561 original save files against their
pre-run hashes. Startup films sometimes need ESC before frontend navigation can
finish; do not send another autopilot command-channel batch while the runner owns it.

The second scenario checks the first donation without progress writes before
granting book 1. The final scenario grants nothing and only reads quest/inventory
state. Success requires donation and good-book counts of 2, both books absent,
and exactly indices 0 and 1 set in each flag vector.

Evidence: `work/codex_lua_marathon9_20260929/runtime_validation.json` and the
three `work/runner/v53_*donation*_20260929` logs. This covers the first two
good-book donations; duplicate rejection, other categories and rewards remain open.
