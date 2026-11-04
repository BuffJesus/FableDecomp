# Lua recovery - 2026-10-16

User: "Proceed with lua work until asked to stop." Continuous Lua recovery + game launches
authorized (see HANDOFF). No root commits/pushes.

### W155 V_ChickenKicking runtime (completes W154)

W155 runtime verified: work/codex_lua_chicken_kicking_w155_20261016/actual_runtime_results.json.
PID6148 closed via CloseMainWindow; 563 saves restored; W147 bundle copied with manifest
hashes verified (DLL 445fcfff...); no Lua runtime/C++ errors; pause 22/22; no source edits.

- Assisted: GhostFisherman removed (its alive branch + native ghost questline were verified with
  real input in W154); hero placement at CK_HeroStart / facing / camera before kicks.
- First meeting with the ghost already gone takes CS_CHICKING_INITIALWALK1 (W154 took the
  RETURN_MEETING path): KnowGhostHasGone + SpectatorsCreated set, organiser moved to the arena.
- Kicking works with real input: `kick_loop.py` holds W and polls hero-chicken distance over the
  Autopilot channel (0.2s RTT), releases W at <=1.5-2.3 and clicks LMB. All kicks landed in the near
  band (walk-speed run-ups): FRONT_RIGHT/FRONT_LEFT 25, CENTRE 10, two band-0 "weak" lines.
- Foul: hero crossing the white line makes KickedChicken write `SetStateInt("DistanceBand",
  0xffffffff)`; the sol2 int binding reads it back as -1, so the master's `~= -1` test takes the
  CS_CHICKING_FOWL path ("That's a foot fowl!"). The hex literal is safe in the live host.
- Three paid rounds (570->520->470->420): totals 35 / 30 / 65, all matching the per-kick lines.
  Round 1: best 0->35 (NO_PRIZE_AGAIN_HIGH). Round 2: 30 < 35 -> NO_PRIZE_AGAIN, best unchanged,
  tutorials skipped via InfoDisplayed. Round 3: 65 >= CHK_LowPrize 50 -> LOW_PRIZE_FIRST,
  CS_CHICKING_LOWPRIZE awards EXPRESSION_COCK_A_DOODLE_DO, PrizesWon=1.
- Retail quirk, faithful: LOW_PRIZE_FIRST does not raise MaxChickenKickingScore (stayed 35 after a
  65). Retail block 0xe68791-0xe687e4 only sets the line, ORs PrizesWon|=1 and picks the macro.
- Harness notes: menu startup again hit the quit prompt (click NO at 236,671 then `menu_only`);
  CameraResetToViewBehindHero sometimes does not apply, so check `aim.png` before W; a 2.0 lateral
  offset from CK_HeroStart drops the hero under the deck (0.8-1.2 is safe); the autopilot `clear`
  box detector misses the master's dialogue "Next" prompts (click 924,687 instead).
- Not verified: mid/top prizes (need >=150/>=250; far bands never reached), LOW_PRIZE_AGAIN*,
  spectator-attack/RanOff branches; the expression's pre-prize absence was not baselined.

### W156 Guild Seal / Global_TeleportToHeroGuild + Global_ToggleTimeDisplay

W156: work/codex_lua_guild_seal_w156_20261016/actual_runtime_results.json. PID9176 closed;
564 saves restored (0aa/Profile.bin appeared between runs from another session's harness, left
as-is); no Lua/C++ errors; no source edits.

- Real seal use (I -> Inventory -> Items -> Other -> Guild Seal -> Use) opens TLC's World Cullis
  Gates UI and teleports natively. NO Lua host is created: retail CActivateQuestDef special-cases
  the quest name (0x7b5820, flag 0x20000). The Lua quest is off the TLC seal path.
- Explicit ActivateQuest verifies both Lua branches: the recall branch teleports exactly to the
  stored recall and clears it; the outbound branch teleports exactly to HERO_GUILD_TELEPORT_MARKER.
  Each run: host, Main, DeactivateQuestLater, return, destructor.
- The recall binding round-trips exact values, but the ENGINE overwrites the recall with a fixed
  point (3183.612,3575.619,33.437 / 0.6077) on every teleport into the guild, even when the calls
  are replayed from another VM. Native behaviour, not a conversion fault.
- ToggleTimeDisplay: two activations with a clean lifecycle, but no visible HUD change. Init runs
  on every activation and there is no OnPersist before Main, so the state may never toggle off.
  Compare against the retail activation lifecycle before calling it a bug (no native activator).

### W157 Global_OpenChest

W157: work/codex_lua_open_chest_w157_20261016/actual_runtime_results.json. Game closed; 563 saves
restored; no Lua/C++ errors; pause 2/2; no source edits.

- Chests open with a real Tab (F1 does nothing). The first interaction showed the native chest
  tutorial and did not start the quest.
- Openable chest with a potion: full lifecycle, reward showcase, potions 2->3, chest emptied.
- Silver-key chest, 0/5 keys: locked message path. With 5 keys: opens, the empty-container path
  skips the reward wait, keys stay 5 (counted, not consumed).
- Harness: GiveHeroObject(def, 5) gave only one key (call once per key). Each gift shows a
  blocking showcase that stalls channel evals until Next is clicked.
- Global_GiveHeroItemsFromRewardChest NOT reached: a spawned OBJECT_CHEST_REWARD_ON_DEATH was taken
  by native walk-over pickup (no Lua host). Needs a real creature death drop.

### W158 reward chest attempt (not reached)

W158: work/codex_lua_reward_chest_w158_20261016/actual_runtime_results.json. Saves restored; no
errors. Killed two spawned CREATURE_BANDIT_GRUNT_LEVEL1 with real LMB (death container enabled;
the second carried a potion). The drop was auto-collected natively (potions 2->3) and no
Global_GiveHeroItemsFromRewardChest host was created. The hero died once and the native Resurrection
Phial revived him while WatchForHeroDeath was live, with no errors.
- Harness gotcha: the frontend helper can reach the quit prompt twice and confirm YES. Recover with
  ESC, ESC, click NO (236,671) to reach the title, then `menu_only`.

### W159 quest state across re-activation (offline, retail bytes)

Answer: retail does NOT keep quest-script state across deactivate/activate, so the Lua host matches.
- CQuestManager::ActivateQuests (0x4b3f17..0x4b3f29) creates the script through the factory, calls
  vtable+8 (Init), then CQuestManager_LoadActiveQuest 0x4b3760. That function does
  LookupPersistentQuestInfo (map at manager+0xc) and, only if an entry exists, builds a load-mode
  CPersistContext and calls vtable+4 (OnPersist).
- Deactivate (0x4b43d0 -> 0x4b3a20) first calls 0x4b39b0, which ERASES the quest's entry from that
  map, then releases the script object (refcount -> destructor at 0x4b3ae1).
- So the map only carries save-loaded state into the first activation after a load. A
  deactivated-then-reactivated quest restarts from Init. Global_ToggleTimeDisplay (member +0x44,
  Init clears it, Main flips it) therefore turns the display ON at every activation in retail too.
  The W156 observation is faithful; this is not a host bug.
- Host parity: the FSE host logs "C++ OnPersist() triggered" via the same vtable slot. None fired on
  mid-game activations (no stored entry), consistent with retail.

### W160 lost SetCombatNearbyBreakOffRange operands (offline, retail bytes)

The shipped W147 bundle has exactly 3 `nil --[[operand lost by the decompiler]]` sites, all
`SetCombatNearbyBreakOffRange(<AllCreatures element>, nil)`:
TraderConflictEvil.lua:94 (Main, call 0xdf66d1), TraderConflictGood.lua:60 (Main, 0xdfaa01) and
TraderConflictGood.lua:176 (WatchForRegionTransitions, 0xdfc9e5).
- Retail pushes `0x41200000` (10.0) before the 3-dword by-value CScriptThing at every one of the 5
  `call [reg+0xcc4]` sites in 0xdf0000-0xe00000 (the two correctly converted sites also pass 10.0).
  The lost operand is 10.0 in all three cases.
- Cause: native_evidence_lowering.py:5149 knowingly emits ENGINE_LostOperand() when Ghidra folds the
  by-value thing into two operands and drops the trailing float. The lowering is text-only (no
  call-site addresses or retail bytes), so a fix must plumb call addresses into that rule and read
  the imm32 push. As shipped, nil reaches the float parameter (0 or a sol2 error), which would
  shorten the raid creatures' combat break-off range.
- Not fixed yet; no Lua was hand-edited (the converter owns these files). Converter work item: recover
  the trailing pushed imm32 for by-value-thing calls, regenerate both units, then run the audit and
  acceptance gates.

NEXT W161: implement the lost-operand recovery in the converter (needs call-site evidence), or
runtime-test QS_GuardianSisterInfo / SummoningTheShip, which have no runtime evidence.

### W161 QS_GuardianSisterInfo runtime VERIFIED

work/codex_lua_guardian_sister_w161_20261016/actual_runtime_results.json. Bowerstone transition
(assisted), real walk-in triggers Maze's call-over line, and a real Tab plays CS_GUARDIANSISTER_BOWERSTONE
in full. Then SetQuestAsCompleted, deactivation and destructor run; the quest reads completed and Maze
is removed. No errors; pause 1/1; game closed; 563 saves restored. Hit branch untested.

NEXT: converter fix for the W160 lost operands (10.0); runtime QS_GuardianSisterInfo2 / SummoningTheShip.

### W162 lost-operand converter fix (W160 follow-up)

- New `lost_trailing_float_operand(spec, slot)` in native_evidence_lowering.py: where the by-value-thing
  rule would emit ENGINE_LostOperand, it reads the retail bytes of the function (spec.byte_at), and
  for every indirect site of that slot whose straight-line setup is exactly `push imm32; sub esp, 0xc;
  ...; call [reg+slot]` (no call or other push in between) it takes the imm32. All shaped sites must
  agree; any doubt keeps ENGINE_LostOperand. Test: tools/script_recovery/test_lost_trailing_float_operand.py
  (4 cases, incl. the 0x00DF689A call-in-between shape and disagreeing sites).
- Regenerated trader_conflict in place (draft reproduced the backup byte for byte first). Lua diff is
  exactly the 3 sites, now `SetCombatNearbyBreakOffRange(..., 10.0)`, in both draft and readable. The
  READABLE_REPORT.json also picks up `callbackScopes` and new function entries from the
  readable-builder change already pending in the tree (not from this fix). Backup:
  scratchpad tc_refs_backup.
- NOT yet in a runtime bundle: the W147 package still ships nil. Promotion needs the usual acceptance
  replay plus the converter audit (a multi-agent workflow, which needs the user's opt-in).

### W163 QS_GuardianSisterInfo2_SisterInBanditCamp runtime (explicit activation)

work/codex_lua_guardian_sister2_w163_20261016/actual_runtime_results.json. Game closed; 563 saves
restored; no errors; pause 2/2.
- Setup gotcha: quest 1 and quest 2 both bind script name MazeAtTavern. With quest 1 still active,
  the Oakvale Maze ran quest 1's script. Quest 2 needs quest 1 done, then a region reload.
- Verified: 30-unit call-over repeating on the 5s timer, real Tab -> CS_GUARDIAN_SISTER_2
  (QST_076_MAZE_NEWS_OF_SISTER_10..90), Maze teleports out, completion, deactivation and destructor,
  ships forced visible.
- Open: AddQuestCard(Bandit Camp) not observable via bindings. IsQuestCompleted reads false after
  completing this explicitly activated quest (true for quest 1, which came from the save card).

### W164 SummoningTheShip: host movie re-entrancy bug found, fix built (not runtime-tested)

work/codex_lua_summoning_ship_w164_20261016/actual_runtime_results.json. Game closed; 563 saves restored.
- HookCoast = map slot 31 (forge world). Explicit activation; the empty Quest Start card is dismissed
  with ENTER (mouse clicks ignored). A real Tab on the lighthouse panel sets LighthouseStarted.
- DoMission then died: "Retail resource scope already owns a movie". The entity FireHeartHolder and
  the quest thread share one RetailResources scope. The entity's DestroyMovie logged, and the quest
  thread ran INSIDE MovieResource_Destroy_API, before Destroy() cleared `live`.
- Fix in a new SDK worktree work/codex_lua_movie_destroy_reentry_20261016/sidecar_candidate
  (branch codex/movie-destroy-reentry-20261016 off 5818237, uncommitted): Destroy() clears `live`
  before the native destructor. Vendor copied hash-verified from W147; MSBuild exit 0; DLL
  6e79c546...320a39.
- The runtime rerun (W147 bundle with this DLL swapped in) was blocked by the session's permission
  classifier ("Modify Shared Resources"). Needs the user's go-ahead.
- Wider note: the one-movie-per-scope policy is a host rule, not retail. Two threads legitimately
  holding movies at the same time would still be refused.

### W165 (offline): the one-movie-per-scope refusal is not retail; removed in the candidate

- Retail `CGameScriptInterface::StartMovieSequence` 0x0089B110 does `while (InMovieSequence /*+0x2C*/)
  NewScriptFrame(); /*slot 0x1C*/`, then waits on `[0x13B8790]+0xF4`, then sets InMovieSequence. A second
  movie therefore WAITS for the first (game-wide), it is never refused.
- Release path verified by disassembly: movie dtor 0x006E7B80 drops the counted impl; impl teardown
  0x006E7550 calls 0x0089B210, which clears InMovieSequence (`mov byte [esi+0x2c],0`) and ends the
  sequence. So the waiter resumes when the first movie object is destroyed.
- Candidate SDK worktree (same as W164, still uncommitted): `StartMovie` and `NewMovie` no longer throw
  "Retail (resource) scope already owns a movie"; StartMovie logs `[RetailMovie] waits scope=... live=...`
  instead and lets the native call wait. Together with the W164 Destroy-ordering fix. MSBuild Release x86
  (solution platform is x86, not Win32) exit 0; DLL sha256
  469b716ab1867012c783d9413ad8b195b7d42edcc8221470a99d2b9237153545 (supersedes 6e79c546...).
- Same error was the open item of WaspBoss run 37 (docs/journal/2026-09/WASP_BOSS_FIRST_RUN_2026-09-22.md).
  Its cause there is still unproven (WatchForCutscene only takes a movie after QueenHornetAttacks), but
  with this DLL the refusal cannot fire any more. A real leak (thread died before DestroyMovie) would now
  show as a hang plus the `waits` log line, which is what retail would do with a never-ended movie.
- tools/script_recovery/teddy_girl_methods.inc still carries the old refusal; it is a stale TeddyGirl
  prep template (old vector-style m_entries), left untouched.
- Runtime check still needed (W164b): SummoningTheShip lighthouse -> DoMission, and ideally WaspBoss intro.

### W166 (offline): candidate bundle with the 10.0 operands and the movie host fix

work/codex_lua_lost_operand_w166_20261016/ (package.py, corpus.py, package_validation.json).
- Full 29-unit corpus regenerated from the current tree and diffed against the W147 corpus: only
  trader_conflict moves, exactly the TraderConflictEvil/Good masters (draft + readable), and the Lua diff
  is exactly the three `nil --[[operand lost...]]` -> `10.0` sites. Corpus input pins were limited to
  non-`work/` sources (2871 files); hashing the 17.7k inherited `work/` pins stalled on cold disk I/O.
- Bundle lost-operand-movie-wait-candidate-20261016 = W147 bundle + the two TraderConflict masters + DLL
  469b716a... (sidecar 5818237 + novi-movie-wait.patch: W164 Destroy ordering + W165 movie wait).
  203 Lua parse, 208 payload hashes, local_test.py preflight (no launch) passes. Changed payload: the two
  Lua files, NoviCompatibility.dll, README.md. gameplay_retested=false.
- Runtime needed: SummoningTheShip lighthouse -> DoMission (movie path), TraderConflict combat break-off.

### W167 (offline): IsQuestCompleted false after a card-less activation is retail

- IsQuestCompleted 0x00892F60 -> 0x004B0FC0 walks the quest-card thing list at [DAT_013b89fc+0x60] and
  returns true only if a thing's component 0x6C name (+0x28) equals the quest name.
- SetQuestAsCompleted 0x00892F80 -> 0x004B1D30 finds the card with 0x004B0C80 in the ACTIVE list
  [+0x5C]; with no card (ebp == 0) it jumps to 0x004B216A (flag cleanups, 0x4AF5B0 entry +0x24 = 0) and
  never puts anything in the completed list.
- So an explicitly activated quest with no card (W163 QS_GuardianSisterInfo2) reads not-completed in
  retail too. No host or converter change. The W163 'open' item is closed.

### W168 (offline): how retail reaches Global_GiveHeroItemsFromRewardChest (W158 follow-up)

- Spawn: CTCContainerRewardHero (vtable 0x01269424) slot 13 = 0x0074F080. When the death container is
  enabled (+0x18) and the thing state is not 2/0xD, it calls 0x0074EC50. That gathers the container
  (0x0074E860, resolving OBJECT_DYNAMIC_POTION/RESURRECTION_DUMMY) and, if any items remain, ALWAYS spawns
  OBJECT_CHEST_REWARD_ON_DEATH at the corpse and moves the items into it (0x008ED1F0). Gold goes to a
  separate OBJECT_GOLDBAG_SMALL_WITH_COINS.
- Def (FableForge build/defc_copy/objects_gameplay.def:500): OBJECT_CHEST_REWARD_ON_DEATH specialises
  OBJECT_AUTOPICKUP_ITEM_TEMPLATE (objects.tpl:1042, THING_OBJECT_PROPERTY_AUTO_PICKUP) and adds
  CTCContainerRewardHero plus CTCActionUseActivateQuest {ScriptName Global_GiveHeroItemsFromRewardChest,
  LoadResources FALSE}. It has NO CTCTargeted, so the player cannot use it directly: only the auto-pickup
  touch can reach Use.
- CTCActionUseActivateQuest (vtable 0x012404AC) Use = 0x007EF600: it does not activate directly. It builds
  a hero action (0x00843F50) and queues it on the user (0x006644F0); the quest starts when that action runs.
- The W147+ bundle registers the quest as an override (retail_override.lua:471), so a real activation
  should create a Lua host. In W158 the items arrived (potions 2->3) and no host appeared. Open: does
  the auto-pickup of this def go through Use (and our override fails to start), or does it bypass Use?
  Runtime probe for next time: poll IsQuestActive("Global_GiveHeroItemsFromRewardChest") every frame
  across the walk-over, and grep the sidecar log for the override load.
- Resolved from the W158 log: an override activation always logs `LuaQuestHost for '<quest>' created`
  (W157 Global_OpenChest line 422), because the sidecar replaces the retail allocator, so even a native
  activation would have produced a host. W158 has the arm/allocator lines for
  Global_GiveHeroItemsFromRewardChest (55, 103) but no host, while the potion still arrived. So on PC the
  auto-pickup hands over the bag contents without the CTCActionUseActivateQuest use, and the quest is
  not reached in normal play. Only an explicit ActivateQuest can run it, and then GetMostRecentValidUsedTarget
  covers only the no-target branch. Reward-chest runtime item closed as unreachable-by-design, not a fault.

### W169 (offline): keep the same-thread second movie as an error

- tools/script_recovery/test_duplicate_movie_start.py exists because Ghidra can print one movie start twice.
  With the W165 wait, a stray doubled start on ONE thread would wait forever on its own movie (as retail
  would), turning a visible error into a silent hang. StartMovie now records the starting Lua thread
  (Entry::movieThread). A live movie from another thread is waited for as in retail (`[RetailMovie] waits`);
  one from the same thread is refused with "Retail movie started again by the thread that still owns a
  live movie" (`[RetailMovie] refused`). No legitimate retail script can do that without deadlocking.
- Rebuilt: MSBuild Release x86 exit 0, DLL c6ef7f5093a163422542c4982f02974c98e912135bf332263062a81d00d641ce
  (supersedes 469b716a). W166 bundle regenerated with it; package_validation still offline_candidate_verified
  (203 Lua, 208 hashes, same four changed payload files).

### W170 (offline): converter test check for W162

- The 75 test files that import native_evidence_lowering: 2468 passed, 76 failed
  (work/codex_lua_lost_operand_w166_20261016/pytest_lowering.log; the full suite could not finish under
  machine load from other sessions).
- The failures are four test functions, none in the 2026-09-27 42-node baseline: test_expression_progress
  (64, Picklock/Steal), test_scalar_array_layouts client Init (8), test_parent_thing_copy guard (2),
  test_startup_angle_bed_operands HornetDrone ST_FEED_LOOP (2).
- Not caused by W162: all of them fail identically with lost_trailing_float_operand monkeypatched to
  return None (old behaviour). The errors are Lua mocks missing newer bindings (`WithRetailResources`,
  `PlayAnimationWithNativeArgument5`), i.e. test-mock drift against the generated Lua already in the tree.
  Left for whoever owns those mocks; W166 stays offline_candidate_verified.
