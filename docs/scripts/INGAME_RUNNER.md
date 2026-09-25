# Quest runner and campaign replay

The runner tests recovered Lua through the game's real quest-card UI,
conversations, combat inputs, and completion flow. It assists travel with
teleports and protects hero health. These are assisted script playtests,
not unassisted gameplay runs.

Run one quest from a checkpoint:

```powershell
python tools/script_recovery/ingame_runner.py tools/script_recovery/runner_quests/orchard_farm_good.json --bundle v15 --tag orchard_test --launch --save adult_maze_completed_runner_2026-09-25 --harvest orchard_test_completed
```

`--save` requires `--launch`. The runner rejects an existing Fable process
before staging anything. It backs up the protected frontend profile,
stages the input checkpoint, and closes the game before restoring the
original save files, even on failure. `--close` is automatic with `--save`.
Do not start a second driver against the same bundle: its command channel
has one pending batch and concurrent writers can overwrite commands.

`--harvest` requires a newer engine-written AutoSave. A completed quest
without a new save is a failed run for chaining purposes. Reusing a harvest
directory removes optional companions absent from the current live save.
The runner does not force an autosave or write quest outcomes.

The campaign wrapper runs each quest in a fresh process, feeding its
harvested checkpoint into the next stage:

```powershell
python tools/script_recovery/run_campaign.py tools/script_recovery/runner_campaigns/adult_good.json --bundle v15 --tag adult_chain_01
```

The adult campaign starts at `adult_graduated`: Wasp Menace, Guardian Sister,
Protect Orchard Farm, then Trader Escort. This does not yet cover New Game
or Guild training. To start later, supply the matching input checkpoint:

```powershell
python tools/script_recovery/run_campaign.py tools/script_recovery/runner_campaigns/adult_good.json --bundle v15 --tag orchard_chain_01 --start-at orchard_farm_good --save adult_maze_completed_runner_2026-09-25
```

Use a new tag for each campaign. Existing reports/checkpoints are rejected,
and a failed stage or missing checkpoint stops the chain. Evidence lives in
`work/runner/<tag>_campaign.json` and each stage's JSONL/screenshots. Game
logs are copied synchronously to `work/runner/<tag>_FableScriptExtender.log`
before the next stage starts. The background launcher's `work/ab_runs/`
collection is supplementary and can miss a rapid restart.

Travel waits for arrival tutorials to clear and observes two consecutive
scene-ready checks before the next crossing. A loaded-region flag alone
does not mean that arrival UI and dialogue have finished. Failed travel
waits stop the stage. Completion predicates also wait for the next Gameflow
stage before harvesting a checkpoint.

Quest files define card titles, region crossings, completion predicates,
and combat targets. `fightDrain: false` preserves real damage throughout
combat. `fightBounds` restricts eligible targets and teleport landings;
Orchard excludes the bandit spawn's region-exit volume. `fightInputs`
selects actual key/mouse actions by target name, including Whisper's RMB
flourish. `fightWhen` and `stateStatus` read conditions/diagnostics in
`stateHost`; they do not assign quest state.

NPC interactions require an exact normalized OCR target-name match. A
processed crop handles the small outlined labels; screenshots preserve
each attempted approach. Missing labels cause a failed start, not a blind
interaction with another character.
