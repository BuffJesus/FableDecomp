# Party Mode upgrade — resume tomorrow

## Stop point

User is going to sleep. Do not launch the game or start further testing tonight.
The user reported a complete, successful childhood playthrough of the **previous**
stock-FSE-plus-sidecar v3 bundle, with its old Party Mode active. That establishes
the gameplay baseline; it does not validate the new Party Mode visuals.

## Ready for playtest

New staged packages: `work/party-mode-v2/`

- `sidecar/`: original FSE + unchanged NoviCompatibility DLL and local launcher.
- `forge/`: prior ForgeFSE playtest DLL + the same upgraded Lua package in `FSE/`.
- `manifest.json`: hashes, source paths, and instrumentation-only change inventory.

Original v3 bundle, installed game, and saves were not changed. No game was launched
this session. No background jobs were started. Changes are uncommitted; the workspace
also contains substantial unrelated earlier work.

## What changed

Party Mode now observes the existing global store using the exact host namespace
`NewOakValeIntro/NewOakValeIntro:`. Each quest has its own VM, and `GetState*` would
read Party Mode's own state, so direct quest-local polling would have been wrong.
Both runtime sources expose `GetGlobalBool` and `GetGlobalInt` with shared-store semantics.

- Milestones: father's introduction, bully subdued, teddy acquired, barrel duty
  started/completed, infidelity discovered, sweets acquired, good/bad deeds, gold gains.
- Session score, four ranks, and a combo multiplier capped at 4x within 20 seconds.
- Scoreboard appears with milestone titles; no repeating heartbeat banner.
- Existing `MAZE_TELEPORT_OUT_01` visual at the hero, optional gentle camera shake.
  Effect name has an existing use in the local GuardianSisterInfo2 port; appearance
  and lifetime in this overlay still need live verification.
- Suppresses presentation during cutscenes/movie sequences; discards queued events
  older than 20 seconds and limits the queue to eight entries.
- Baselines existing save flags rather than replaying old achievements. Score and
  combos are session-local and restart when the overlay reloads.
- Birthday summary after chocolates; stops at completed childhood/adult state.
- Optional failing/missing features are disabled; missing required APIs stop the overlay.

Config: `sidecar/NoviCompatibility/PartyMode/config.lua` or
`forge/FSE/PartyMode/config.lua`. Defaults: enabled, scoreboard and effects on;
camera shake and debug markers off. Sounds, crowd behavior, weather, and NPC changes
are not implemented.

The converter's Party Mode instrumentation now uses `debug_markers` rather than
`enabled`. The package upgrader replaces that exact initial switch line in 17 existing
generated scripts. It asserts reversibility to the original bytes; gameplay bodies
are preserved. Source generated packages were not regenerated in place.

## Files for continued development

- `tools/script_recovery/party_mode/PartyMode.lua`: canonical overlay template.
- `tools/script_recovery/party_mode/config.lua`: canonical configuration template.
- `tools/script_recovery/party_mode.py`: converter instrumentation and template emission.
- `tools/script_recovery/build_party_mode_bundle.py`: reproducible staging from v3;
  requires a fresh output directory and refuses to overwrite an existing one.
- `tools/script_recovery/test_party_mode_runtime.py`: Lua execution tests and host contracts.

Rebuild to a fresh directory from the repository root:

```powershell
python -m tools.script_recovery.build_party_mode_bundle --out work/party-mode-v2-next
```

Do not use the old `build_novi_compat_bundle.py` defaults to rebuild this upgrade:
they still select the older readable package and v3 output.

## Verification completed tonight

- 14 tests pass: `python -m unittest tools.script_recovery.test_party_mode tools.script_recovery.test_party_mode_runtime`
- All 45 staged Lua files compile through Lupa's Lua runtime.
- Required bindings checked against both ForgeFSE-retail-shadow and sidecar source.
- Key API name strings also present in both packaged DLLs; this is not execution proof.
- Sidecar manifest and installed-game preflight pass with `local_test.py` without `--launch`.

The runtime tests use host doubles, not a running game. Neither new package has been
live-tested. Exact native rendering, timing, effect lifetime, and save/reload behavior
remain unverified. The prior ForgeFSE DLL was reused, not rebuilt this session.

## Tomorrow's next step

Start with the sidecar package and play the childhood milestones:

```powershell
python work/party-mode-v2/sidecar/local_test.py --game-dir "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters" --launch --save-dir "C:/Users/Cornelio/Documents/My Games/Fable/Saves"
```

The launcher requires no existing Fable process, backs up saves, and loads DLLs from
the test folder. Log: `work/party-mode-v2/sidecar/NoviCompatibility/FableScriptExtender.log`.
Look for `PARTY_MODE event=` and Lua runtime errors.

Check readable titles/score, actual effect appearance, no repeated awards while idle,
cutscene silence, teddy/deed/gold milestones, birthday summary and raid silence.
Check save/reload produces no replay of old awards (score resets by design).
Then test the ForgeFSE package using the established Forge deployment/rollback procedure;
the sidecar launcher is only for the sidecar arrangement.

Potential polish: make the debug-marker switch also honor `enabled=false` when
`debug_markers=true`; currently those are independent switches. Defaults are unaffected.
