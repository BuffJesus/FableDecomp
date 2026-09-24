# Orchard Farm entry and combat ABI fault ? 2026-09-23

From `adult_maze_completed_2026-09-23`, v11 displayed both Orchard cards in the
real guild UI. Protect Orchard Farm was accepted using Take Quest, with no
forced activation. The Good wrapper and shared OrchardFarmRaid hosts started.

## Entry findings

Guild map slot 70 is confirmed by installed FinalAlbion.wld. MainGuildMap's
north side (map position + y2, z1) opened the card UI after dismissing the
inventory tutorial. Earlier attempts selected Guildmaster; the diagnostic
SetHeroGuideToShowQuestCardsWhenSpokenTo(true) was also applied before the
successful approach, so this is not a minimal fresh-entry recipe.

The initial five entry checks passed but center-of-map arrival triggered
HeroAtWrongEntrance. Source DoCutsceneIfRequired requires distance <10 from
GuardTeamSpawn for the Good quest, then requires leaving the region after a
wrong entrance. A return to the guild cleared the flag naturally. Arrival at
(3203.57,3206.19,41.02), near GuardTeamSpawn (3200.57,3206.19,40.02), ran the
farmer intro and Quest Start screen. The corrected harness uses the shared
OrchardFarmRaid host and does not press ESC blindly.

The full initial farmer movie played. After Enter and tutorial dismissal the
game exited before combat. Initial log archive:
`work/ab_runs/v11-20260923-172044/FableScriptExtender.log`.
The accepted-card save was preserved separately as
`adult_orchard_accepted_2026-09-23`; fresh reload verified both quest hosts and
an unstarted introduction. The clean replay skipped the farmer movie and
reproduced the fault after the inventory tutorial pages.

## Native diagnosis

The existing FableForge `tools/ingame/crash_catcher.py` captured access violation
EIP 0x008A54EC, ECX 0x40800000 (float 4.0). This is
SetCombatNearbyBreakOffRange at 0x008A54E0, reached by crate team AI.
The DLL typedef passed CScriptThing* plus float, while the decorated native
symbol takes CScriptThing by value. The callee interprets the float as the
second word of the missing 12-byte thing and dereferences it.

Evidence: `work/orchard_native_crash.json`, native interface decompile at
`ghidra_out/scriptvm_decomp.c`, and raw retail disassembly retained in
`work/orchard_value_abi_disassembly.json`. SetCombatNearbyBreakOffRange,
SetStealStealableItems, and SetRecoverStealableItems return with ret 0x10;
ResetCombatNearbyBreakOffRange returns with ret 0x0c. All four consume their
thing copy, including a reference-count release. These four APIs are used by
Orchard's crate team AI and need value signatures plus a retained copy.

## Corrected candidate

Patch: `tools/script_recovery/sidecar_patches/novi-zzz-combat-range-value.patch`.
All four signatures now pass CScriptThing by value; each wrapper increments the
copied handle's Info reference count before the consuming call. The patch
reverse-checks cleanly against the built sidecar source. Release/x86 MSBuild
succeeded (`work/orchard_sidecar_build.log`), and 18 harness tests pass.

Bundle v12 DLL SHA256:
`4570a54e555488048530562366e2d3c585ba3188457d8bbab4e62a347baec564`.
All 98 Lua files match v11 byte for byte. v11 is preserved. Corrected entry
passed 5/5 in `autopilot_orchard_v12_entry_2026-09-23.json`.
After skipping the farmer movie, acknowledging Quest Start and dismissing both
inventory tutorial pages, v12 proceeds into real bandit AI with three crates.
Real sword hits reduced a bandit's health 20 -> 17 -> 4. Travel and hero-health
assistance are used; enemy health and outcome flags are not assigned.
The combat probe defeated the initial three bandits and the next group. At
probe step 27: BanditWavesSpawned=2, CrateCount=3, and the third group was
starting to spawn (one member observed). Step 28 found DoneIntroduction=false.
The intervening log shows a native region-load event, all quest hosts torn down,
and gameflow restarted at saved stage 450; the hero was back in the guild.
The cause of that reset is unresolved. No claim of Whisper or quest completion.
The probe was stopped rather than forcing progress. Evidence is
`orchard_v12_combat_2026-09-23.json`. Quest Start artwork remains visibly corrupt.

Next replay should monitor hero health and target coordinates continuously,
keep assisted movement inside the combat area, and capture any death/load UI
before further attack or tutorial input. The broad `clear` heuristic and
teleport-to-newly-spawned-target path are not established safe across this
boundary. Do not assume the reset was a converter failure or a solved issue.



Final v12 archive: `work/ab_runs/v12-20260923-173829/FableScriptExtender.log`.
Debugger reproduction archive: `work/ab_runs/v11-20260923-173011/FableScriptExtender.log`.
No Lua runtime errors in the three archived runs. Native crash and Lua error
counts are separate: v11 did crash. The test game is closed; all three original
save files are restored byte for byte. Checkpoint and archive hashes are in
`orchard_value_abi_evidence_2026-09-23.json`.
