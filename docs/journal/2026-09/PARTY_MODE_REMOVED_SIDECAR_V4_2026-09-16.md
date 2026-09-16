# Party Mode removed; clean sidecar v4 verified through childhood; Discord zip rebuilt — 2026-09-16

## What happened

1. Launched the staged Party Mode v2 sidecar (`work/party-mode-v2/sidecar`). Scripts ran with zero
   Lua errors, but the user reported: "worked for a bit, music stopped, things aren't loading fully"
   and that the `MAZE_TELEPORT_OUT_01` visual played twice. The log agrees: the activation banner
   and the first award (`BIRTHDAY QUEST ACCEPTED`) each call
   `CreateEffectAtPos("MAZE_TELEPORT_OUT_01", pos, 0, independent=true, alwaysUpdate=false)` —
   two plays, then the native breakage. No Aeon port ever passes `independent=true`; all use the
   2-arg form. Not root-caused further because:
2. **User decision: remove Party Mode entirely.** ("We broke something along the way. Let's not
   worry about party mode. Just remove it.")
3. Removed at the source so regeneration cannot bring it back:
   - `build_readable_new_oakvale.py`: no `instrument_source`/`write_overlay`; writes `Quests = {}`.
   - Markers stripped from `readable_oakvale_main.py`, `bully_main_structure.py`,
     `start_barrel_timer.py`, `watch_barrels_loop.lua`; `PartyMode`/`PartyModeMark` hooks removed
     from `refs/script_recovery/new_oakvale_intro/runtime_playtest/retail_override.lua`.
   - Deleted `party_mode.py`, `party_mode/` templates, `build_party_mode_bundle.py`,
     `test_party_mode.py`, `test_party_mode_runtime.py`. `test_start_barrel_timer_readable.py` now
     asserts no overlay and no `PARTY_MODE` text.
   - Readable package regenerated (`refs/script_recovery/lifted/NewOakValeIntro/readable`): 0 Party
     Mode references, 18/18 syntax. Whitespace-insensitive diff vs the previous package = marker
     removals only. Side benefit: the old interaction-marker regex had been stripping leading
     indentation from every Speak/StartMovie/RunMacro/conversation line; indentation is restored.
4. Assembled **`work/new-oakvale-original-fse-20260912/local-candidate-v4`** with
   `build_novi_compat_bundle.py --skip-build --readable work/oakvale_readable_clean_20260916/FSE`
   (staging = regenerated readable + `retail_override.lua` with `enabled = true`). Both DLLs are
   byte-identical to v3 (`FableScriptExtender.dll` 36ffb32c…, `NoviCompatibility.dll` b99ae76f…).
5. **User played v4 New Game through the end of childhood: "All seems to work properly."**
   Log: 16/16 entity hosts, zero Lua errors, no overlay quest, clean teardown.
6. Rebuilt the Discord playtest package from v4 with the new
   `tools/script_recovery/build_discord_playtest_zip.py`:
   `work/NewOakValeIntro-sidecar-playtest-20260916.zip` (2.5 MB). Layout: `NewOakValeIntro/`
   (both DLLs + launcher + Lua) + `Launch New Oakvale Intro.bat` + README + `bundle_manifest.json`.
   Testers extract into the game root; **nothing in their install is replaced**. Verified by
   extracting into the real install, launching through the .bat (injection result 0, both sidecar
   logs initialised), then removing the extracted files. The old
   `work/NewOakValeIntro-readable-discord-playtest.zip` (Forge DLL + banner overlay + README with
   literal `\n`) is superseded — do not ship it.

## Known, not fixed

- `tools/script_recovery/test_watch_barrels_loop.py` (78 subcases) fails: it expects a
  `resources:RewardRemainingBarrel` binding; the Lua (already in v3, which played clean) uses
  `quest:GetThingWithScriptName` + `AddItemToContainer`. Pre-existing at the 09-14 checkpoint,
  untracked files, not a regression from today.
- Full `python -m unittest discover -s tools/script_recovery` produced no output in 15+ minutes;
  run targeted modules instead.
- `work/party-mode-v2/` and the old Discord zip are still on disk (work/ is not committed).
- Quiet locked-camera Father speech remains as documented on 09-12.

## Resume

- Ship `work/NewOakValeIntro-sidecar-playtest-20260916.zip` when the user says so.
- Still user-verified only on v4: save/reload across a session, adult transition after childhood.
