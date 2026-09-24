# Orchard Farm guarded combat replay

Follow-up to `ORCHARD_COMBAT_ENTRY_2026-09-23.md`, same v12 DLL and Lua.
Started from `adult_orchard_accepted_2026-09-23`; corrected entry passed 5/5
(`work/orchard_guarded_entry.json`). Actual farmer intro, Quest Start, and two
inventory tutorial pages acknowledged before drawing the sword.

The experimental driver `work/orchard_guarded_combat.py` logs hero health and
position before assistance, wave/crate/member counts, and chosen enemy health
and position. It replenishes hero health below 85%, uses real sword clicks,
and only approaches enemies inside a conservative farm rectangle. It does not
alter enemy health or outcome flags, and does not dismiss popups during combat.
It stops on a scene, Whisper, failure, success, intro regression, bounds exit,
or missing channel response, capturing the screen at those boundaries.

Initial segment defeated groups one and two, then recorded wave 2 with three
members and three crates, without the previous reset. Hero health remained
positive in every sampled row. At the eastern movement limit the enemies
stayed outside the eligible rectangle; stopped the driver and assisted a
retreat to (3280,3246,41), then resumed. Segment one telemetry is preserved in
`work/orchard_guarded_combat_part1.json`; screenshots under
`scratchpad/orchard_guarded_part1/`.

Installed `OrchardFarmEast.tng` places BanditTeamSpawn at local
(5.478027,124.281982,45.161427); `FinalAlbion.wld` places that map at
(3296,3168). Thus the spawn is east of the main farm map. This explains why
unbounded target teleports can leave the selected combat rectangle; it does
not establish the cause of the earlier guild reset.

Pre-Whisper API spot check: native decorated signatures in
`ghidra_out/scriptvm_decomp.c` and installed retail disassembly agree that
EntitySetAsToAddToComboMultiplierWhenHit (0x88ebc0, ret 8),
AddQuestInfoBarHealth (0x891ad0, ret 0x10), and EntitySetCombatType
(0x89e080, ret 8) take thing references. Existing sidecar pointer declarations
match these, unlike the four by-value calls fixed in v12.

The resumed segment reached wave 3, zero remaining bandits, and three crates;
Whisper's challenge scene ran. The actual flourish tutorial specifies Right
Mouse Button. After four melee clicks and a right-button press, the native
conversation log queued `TEXT_QST_051_WHISPER_HAS_BEEN_HIT_WITH_FLOURISH_10`
and her health bar appeared. The scripted flourish gate was passed by input.

One initial Whisper query timed out while the game was not advancing; the
following capture focused the game and the queued command then executed.
Preserved that response separately, refocused, and resumed with a 20-second
query timeout. Real melee/flourish inputs reduced Whisper from 60 health to
2, then her script set MissionSucceeded and launched the concession/outro.
Hero health and movement assistance continued; no enemy-health or success
assignments were sent through the diagnostic channel.

Combined telemetry: `orchard_guarded_combat_2026-09-23.json`. Entry report:
`autopilot_orchard_guarded_entry_2026-09-23.json`. New read-only handoff
checklist checks native Good completion, stage 500, and CoreQuestWaiting.
Combat/outcome waits no longer issue automatic UI clicks. Twenty targeted
checklist, launch, and driver tests pass (the printed guarded_menu FAIL is
the expected negative mock case).

The completion screen showed Protect Orchard Farm, Whisper's Brooch, 750
gold, 424 total renown (400 reward plus 24 extra), and 511 experience. After
Enter, all three handoff checks passed. Preserved the newly written engine
AutoSave as `adult_orchard_completed_2026-09-23`, closed the process, restored
the original profile, then staged the new checkpoint for a fresh v12 process.
All four reload checks passed: native Good completion, stage 500,
CoreQuestWaiting, and persistent OFBR_NoCratesWereStolen.

Completed-run archive: `work/ab_runs/v12-20260923-181526`; reload archive:
`work/ab_runs/v12-20260923-182940`. Both contain zero Lua runtime errors.
Reports: `autopilot_orchard_handoff_2026-09-23.json` (3/3) and
`autopilot_orchard_reload_2026-09-23.json` (4/4). Hashes and original-profile
restoration proof: `orchard_guarded_evidence_2026-09-23.json`. Both test game
processes are closed; all three original save files match their backup hashes.

No DLL or generated Lua edits were needed in this replay. The earlier reset
cause remains unresolved, and this does not validate Evil-path or unassisted
combat. Quest Start artwork remains corrupt. The next gameflow quest is
Q_TraderEscort, whose native cluster/operation IR exists but whose Lua package
is not included in v12; native fallback progression would not validate a Lua
conversion. V_IntroductionToTrophies likewise remains outside the bundle.
