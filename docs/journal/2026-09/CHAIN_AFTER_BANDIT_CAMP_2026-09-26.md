# The chain after Bandit Camp - 2026-09-26

Continues from [Bandit Camp](BANDIT_CAMP_CONVERSION_2026-09-25.md) (completed hands-free, bc15). Gameflow stage
800 (`EGP_MAZE_TELEPORT_TO_WW`) waits on `QS_GuardianTrophyDealerInfo`, 850 on `V_TrophyDealer`, 856/870 on
`Q_WhiteBalverineKnotholeGlade` (reference: Aeon's LUAGameflow port, stages 700-900).

## Three new converter units

| unit | range | quests / entities | draft | notes |
|---|---|---|---|---|
| `guardian_trophy_dealer_info` | 0x00E277E0 .. 0x00E28CA0 | 1 / `GTDI_Maze` + worker `WaitForPieceOver` | 8 fns, 0 missing, smoke 0 | vtable 0x012E2018, allocator 0x00E28CA0 |
| `trophy_dealer` | 0x00EE6C00 .. 0x00EE80A0 | 1 / `TrophyDealerInCave`, `DemonDoorFace` | 12 fns, 0 missing, smoke 0 | shared empty OnPersist 0x00CBD4E0 anchored (typed export) |
| `white_balverine` | 0x00E13C20 .. 0x00E1A770 | 2 (KnotholeGlade + WW) / 9 | 45 fns, 0 missing, readable smoke 0, 28 TODOs | KHG Init/Main sit inside the wasp_boss range (after its dtor 0x00E13BF0) |

Recipe (docs/scripts/PRE_ORCHARD_CONVERSION_PLAN.md) plus: the PDB locals come from
`work/pdb_locals_20260913/pdb-locals.exe "C:/Program Files/Microsoft Visual Studio/2022/Community/DIA SDK/bin/msdia140.dll" debug_build/Ego_r.pdb "<pattern>"`
into `refs/script_recovery/<unit>/pdb/Ego_r-pdb-locals.tsv` (quest_unit_evidence needs it). A family's range
is found from strings: seed-export a candidate range read-only and list the strings (the White Balverine WW
block names `WBWW_*`; its KHG half was found in the wasp translation unit by `KnotholeGlade`).

Checked against Aeon's hand ports: GTDI keeps retail's worker thread and its ON_HIT-then-REPEAT line pair and
leaves out the port's `SetTeleportingAsActive` (retail calls only SetTeleporterAsActive, GSI 0x8CC).

## Converter fixes (generic, all-unit A/B each)

- `rotate_mid_entered_loops` (native_goto_scopes): `while( true ) { A; LAB: B; }` reachable only through
  `goto LAB` is rotated to `LAB: while( true ) { B; A; }`. The tail copy ran B once and jumped PAST the loop:
  GTDI_Maze's hero TryAcquire retry, BanditCampBossBattle's alarm loop over the defensive guards,
  CROWDBANDITS' loop re-entry, DarkwoodAssassinSpawn's wait for the hero within 18. A first attempt (refuse
  every tail copy that leaves a loop) was too broad (it broke a correct backward copy in BanditCamp) and was
  dropped.
- position member reads through `(undefined4 *)` / `(int *)`; any-order member stores with computed values
  -> `ENGINE_Vector3` (all-constant fills excluded: KickedChicken's `0x3f800000` bit pattern); the quest
  interface cached from `this + 0x40` retyped like `this + 4`.

## In-game

- **QS_GuardianTrophyDealerInfo COMPLETE on the converted script (gtdi3, v23, 0 Lua errors)**: Maze's
  CS_MAZE_TROPHY_INFO, card V_TrophyDealer, stage 850. Checkpoint `adult_trophy_dealer_info_completed_2026-09-26`.
  Runner: Maze stands beside a Guild pillar; the 1.5 stand-offs land in geometry and the engine relocates the
  hero to another room (the retail oracle gtdir1 failed identically), so configs can now give `talkSides`
  (`[[1.0, 0], ...]`). The travel hop into the Guild (slot 70) takes no region name (the region check never
  reports `HeroGuildComplex` loaded).
- **V_TrophyDealer COMPLETE on the converted scripts (td5, v25, 0 Lua errors)**: quest start screen,
  Witchwood travel, our DemonDoorFace opens the door (CS_TROPHY_DEALER_DOOR_OPENS), TrophyDealerInCave plays
  CS_TROPHY_DEALER and SetQuestAsCompleted, Gameflow stage 856. Checkpoint `adult_trophy_dealer_completed_2026-09-26`.
  **Assisted:** the Singing Stones puzzle (retail-native V_SingingStones, not converted) is not solved by the
  runner yet; the config sets the flag its correct-tune path sets (`SetMasterGameState('SingingStonesInSync',
  true)`, logged as a step `do`). Retail's winning tune was verified in its Init (0x00ED0EC0): list at +0x4C =
  3,1,0,2 = D B A C (rude tune +0x5C = 2,3,1,0), matching Aeon's port. The runner's `hit` step (teleport beside a
  script-named object, face it, swing) exists but its stand-offs still get relocated or face away (td2-td4
  captures `work/runner/td4_hit_*.png`); drawing with Q toggles the sheath, an attack click draws by itself.
  The sidecar's SetMasterGameState had no setter for `SingingStonesInSync` / `TCGTimeLimitBoastTaken` (the getter
  reads them): `novi-zzzzzzzzzz-master-state-setters.patch`, v25.
- **White Balverine (v27): the Knothole Glade half plays through (wb3, 0 Lua errors)**: card taken by the
  text-bank title ("White Balverine", `game_text`), gate balverines (V_KnotholeGladeGates, retail native:
  `KGG_*`, CREATURE_BALVERINE_01), three drive-offs of `WB_WhiteBalverine` (drained fight, real hits), the ambush
  marker, CS_WBK_CHIEF1-3, MissionSucceeded, Q_WhiteBalverineWW activated. wb2 had died at CS_WBK_CHIEF2 on two
  converter bugs, both fixed (817f92a): a slot reused after serving as a hidden-result slot kept its old result
  variable (the chief's resource lifted as `pCVar6`, CHIEF bound to the hero's resource), and
  canonicalise_stack_objects spliced overlapping regions at stale offsets (`xStack_100 = StartMovie`, a
  duplicated character, so DestroyMovie freed the previous movie). The Knothole half never autosaves at its
  end (Gameflow's stage-870 save is written when the card is taken), so both halves now run as one config
  (`runner_quests/white_balverine.json`).
- **White Balverine COMPLETE on the converted scripts (wb5, v28, 0 Lua errors)**: both halves in one run -- the
  Knothole Glade half as above, then WitchWood_7 (CS_WBW_DRINK, WBWW_WhiteBalverine killed, MonitorBalverine's
  trophy head + objective 08), KG_Chief's CS_WBK_CHIEF4, MissionSucceeded -> WhiteBalverineFinished ->
  SetQuestAsCompleted('Q_WhiteBalverineKnotholeGlade'), Gameflow stage 875. Checkpoint
  `adult_white_balverine_completed_2026-09-26`. wb4 had died at WW Main's first line on a temp-destruction flag
  word seeded from a disguised `!b && b` (fixed 70be5e1).
- Runner: the ENTER in a pause ladder is safe only while frames are paused (no Lua reply): with nothing up it
  opens the pause menu. `wait_for`'s ladder falls back to the quest-wide `pauseSteps`.
- Save hygiene: a diagnostic wrapped in `timeout` was killed before its `finally`, leaving the staged save in
  the live slot; restored by hand from the timestamped backup and verified byte-identical. Never wrap a
  save-staging script in `timeout`.

## Resume here

1. Next: Gameflow stage 875 waits for Q_Arena to be taken ("The Arena"), from
   `adult_white_balverine_completed_2026-09-26`. Unit `arena` is registered and converted as a BASELINE, not
   playable yet: the quest's lifecycle block 0x00CFA660..0x00CFAD30 is small (Init, OnPersist, allocator
   0x00CFABF0) and its code (Main, workers WatchForTermination / CrowdChecker, the entities Cham, Flick, Shadow,
   Needle, Roth, CellWhisper, the cell guards, CagedBalverine, ArenaEnemy / ArenaSpawn / SUMMONED_CREATURE,
   WhisperAlly) is the block 0x00F0F910..0x00F28000, so the unit range is that block and the lifecycle block,
   Q_ArenaHoldingScript (allocator 0x00CF8860, found by the inventory) and the shared entity stubs 0x00CDEBC0 /
   0x00CDEBD0 are anchors. Draft: 22 files, 90 functions, 0 missing, 7 file syntax failures, 682 TODOs (81 are
   `pCVarN = (CScriptThing *)...` thing reads, 32 raw vcalls), readable smoke 10 problems. Arena.lua alone
   has 202 TODOs.
2. White Balverine: review the 22 TODOs (two unresolved PlayAnimation /
   IsPerformingScriptTask receivers in WW SpawnBalverines: the quest-context resource receiver is refused by
   design until its resource is proven; the actor-map build is fixed). The quest plays through regardless.
3. Singing Stones by hand-free strikes (optional): fix the `hit` stand-off.
4. Sidecar for v25-v28 = sidecar-abi-v13 + things-killed + cancel-using-ability + master-state-setters patches.
