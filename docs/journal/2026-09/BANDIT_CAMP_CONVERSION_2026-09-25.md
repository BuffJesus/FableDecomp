# Bandit Camp conversion bootstrap ? 2026-09-25

Offline recovery; no Bandit Camp override has been deployed or activated.

The retail lifecycle anchors bound the family at 0x00D006D0 through
0x00D12D00 (the next Bounty Hunt Init). The three scripts are Q_BanditCamp,
Q_BanditCampBossBattle and Q_BanditCampHoldingScript. Read-only Ghidra
export converged after three passes: 131 function bodies, all entity
lifecycle exports present, and no inventory errors. The family contains
16 entity bindings and 12 registered workers. Evidence currently lives in
`work/bandit_camp_bootstrap`; export logs in `work/bandit_camp_export`.
The x86 DIA tool extracted 31,708 bytes of PDB locals. Typed export applied
1,355 overrides across the 131 functions.

## Worker-name scope

The evidence builder rejected two legitimate WatchForTermination workers:
0x00D01290 registered by Q_BanditCamp Main, and 0x00D04260 registered by
Q_BanditCampBossBattle Main. It had validated unique names across the whole
translation unit before determining which functions a quest reaches.
Name uniqueness is now checked within the reached quest closure. Conflicting
names for one address still fail globally; different bodies with the same
name still fail if both are reached by the same quest.

Five focused tests pass, including two sibling quests retaining their own
worker and a transitive registration collision still being rejected.
An in-memory before/after evidence build across all 17 existing units found
identical results wherever the old builder succeeded. Previously blocked
Guild Training (nine scripts), Wasp and Trader Conflict (two scripts) now
build successfully. Evidence: `work/thread_scope_ab.json`. No existing
unit evidence was rewritten by this comparison.

## Registered draft

Registered `bandit_camp` and promoted the recovery evidence to
`refs/script_recovery/bandit_camp`. The typed export explicitly includes
shared empty OnPersist at 0x00CBD4E0 (132 bodies including this anchor).
The first tracked draft has 19 owners, 85 converted functions, no missing
bodies, 81/85 function syntax passes, 15/19 file syntax passes and 180 TODOs.
All 16 entity bindings and 12 worker bodies are covered by the inventory;
10 inventory/thread-name tests pass.

Remaining syntax failures are CheckAnyBanditsKilled (nested sequence
expressions), BCGameMaster Main (a comma expression in a while condition),
AssassinMarker Main (switch cases cast to CCharString), and BanditKing Main
(unsigned literals). BanditKing also has unresolved x87 truncation operands
(`value`/`value_00`) and needs arithmetic/shift semantics reviewed, not just
syntax repair. Registration remains disabled in the generated package.
The existing live v16 bundle is unchanged.

Generated readable output retains the four syntax failures. Readable smoke:
19 files, 8 problems (four load errors, two free-global reports, plus
Gate1GuardOuter bitwise nil and BanditKingMissionProcess boolean arithmetic).
Evidence: `work/bandit_camp_readable_smoke.json`. This is a recovery baseline,
not a playable package.

## False exception-handler injection

CheckAnyBanditsKilled's typed decompile introduced a stack-space warning
that the untyped export did not have. At 0x004502EB the retail bytes are
`mov eax,ecx; mov ecx,[esp+8]; mov [eax],ecx; ret 8`, but the inherited name
was __EH_epilog3. Ghidra injected exception-stack teardown at its call.
A reviewed helper specification now explicitly clears that call fixup;
the exporter honors this only for helpers carrying `clearCallFixup: true`.

A full read-only typed re-export changed exactly one decompiled body,
CheckAnyBanditsKilled. Both the injected-epilogue and stack-space warnings
disappeared. Regeneration reduces this function's TODOs from 51 to 8, and
the unit total from 180 to 137. Syntax remains 81/85 functions and 15/19
files: its nested conditional stack assignment still needs lowering.
Evidence: `work/bandit_camp_fixup/export.log` and the before/after typed
exports. No other helper receives the opt-in flag.

## Boolean complement arithmetic

BanditKingMissionProcess used a native `1 - bool` expression for its level
wait. Lua rejects arithmetic on booleans. The lifter now converts a known
boolean to 0/1 before the subtraction, preserving a numeric result for
both comparisons and later arithmetic. The runtime regression covers both
boolean values and a numeric subtraction that must remain unchanged.
Focused tests: 98 passed, 35 subtests. The before/after comparison over all
18 units changes only BanditCampBossBattle.lua; all other units are identical.
Evidence: `work/boolean_complement_ab/summary.json` (baseline fab3fbf).

## Conditional stack snapshots

The remaining CheckAnyBanditsKilled syntax error came from a stack-slot
assignment nested in an AND condition. The slot previously held
HUD_QUEST_ICON_BANDIT, so inlining also substituted that string into a
numeric comparison. The sequence-condition pass now accepts direct named
stack assignments, retains the initial numeric value on skipped branches,
and copies known numeric locals through stale casts. The runtime regression
checks skipped and taken branches, both comparison outcomes, and source
mutation after the copy. Pointer-write sequences remain rejected.

After regeneration, syntax is 82/85 functions and 16/19 files, with 137
TODOs. This fixes condition evaluation and slot lifetime; the worker's
killed-thing list and counter argument recovery remain unresolved.

The final 18-unit comparison changes BanditCamp.lua, BS_Teacher.lua and
ChickenMaster.lua only. The latter two have the same conditional stack-write
pattern; their committed sources matched the comparison baseline before
regeneration. Their existing unresolved operations and compile counts are
unchanged. Focused runtime suites: 100 passed, 35 subtests; related Guild,
branch-join and gift regression suites: 28 passed, 32 subtests (Unicorn printed
its existing Windows exception diagnostics but the tests exited successfully).
Evidence: `work/stack_sequence_v2_ab/summary.json`. The earlier incomplete
`work/stack_sequence_ab` run was superseded and stopped.

Readable Bandit Camp smoke is now 19 files / 6 problems: three remaining
load errors, two free-global reports and Gate1GuardOuter's nil bitwise
operand. Evidence: `work/bandit_camp_stack_sequence_smoke.json`.

## Last syntax failures: ST0 operands, integer division, typed case labels

Four generic lowering fixes close the remaining load errors. Each was
checked against the typed decompile, not only for syntax.

- BanditKing Main 0x00D0A830 read `__ftol2(value)` / `__ftol2(value_00)`
  from never-assigned `float10` locals: the export's name for `__ftol2`'s
  ST0 operand when Ghidra lost the preceding GetHealth vcall's float
  result. `bind_st0_results` now binds such locals like `extraout_ST0`
  (`fret_v0` / `fret_v00`), so the free globals `value` / `value_00` are gone.
- The same function's taunt thresholds use MSVC's signed `x / 4` idiom
  `(x + (x >> 31 & 3)) >> 2` (the `3U` Lua could not parse; Lua's `>>` is
  also a logical 64-bit shift) and `(int)x / 2`. Both now truncate toward
  zero like C: `KingHealth < trunc(initial * 3 / 4)` and `< trunc(initial / 2)`.
  Lua's float `/` would have compared against 2.5 instead of 2 for odd health.
- BCGameMaster Main 0x00D067A0 waits in
  `while ((GetBestTimeGuessTheAddition(), K == ST0) || (b = IsHeroInTavernGame(), b))`.
  The ST0 binding picked the nearest preceding statement, the void
  SetQuitTavernGame, so the loop compared a stale nil. A read inside a comma
  expression now binds the call in that expression. Loop heads with call
  sequences (including a call on the left of `||` / `&&`, which the
  single-operator form rendered as a raw expression) lower to
  `while true do <tree> if not c then break end`, re-evaluating the calls on
  every test in C's short-circuit order.
- AssassinMarker 0x00D11930 and BanditKing switch on a counter the export
  typed as a string slot (`case (CCharString)0x0:`), which the switch lowerer
  rejected. The cast on an integer literal is dropped and the label printed in
  decimal: a hex `== 0x0` is the lifter's null-pointer (nil) test, so case 0
  would never have matched the counter's initial 0.

Result: 85/85 functions, 19/19 files, 119 TODOs (was 82/85, 16/19, 137).
Readable smoke: 19 files / 1 problem (was 6).
Evidence: `work/bandit_camp_st0_switch_smoke.json`.

All 18 units, before (HEAD worktree) vs after
(`work/st0_division_switch_ab/summary.json`): only these 5 files change.
- The three Bandit Camp entities above.
- Trader Escort DarkwoodTrader: the old double binding also assigned
  `fret_0` from the void FadeOutAndKillEntity, and GetHealth overwrote it
  before any read. Its readable Lua is byte-identical; only report counters
  changed, so there is no live-behaviour change.
- Sick Child TalkingTrader1: its chat switch has the same typed labels. It
  now lowers and matches the native flow (cases 0-2 break to the increment;
  case 3 and values above 3 reach the wrap label), giving 47/49 functions and
  10/13 files (was 46/49, 9/13). Its `CStack_ac` counter stores
  (wrap to 0, `CVar4 + 1`) were already TODOs before this change and still are.

The A/B harness must see `work/new-oakvale-original-fse-20260912/sidecar-abi-v2`
from the HEAD worktree (the converter reads host bindings from it). Without it,
the baseline falsely reports MsgGetThingsKilled as unbound.

Regression tests: `test_native_st0_division_switch.py`, 8 tests. The 6
behaviour tests fail on HEAD and the 2 guard tests pass on both.

## Flag words parked in a reused stack slot

The last smoke problem was Gate1GuardOuter Main 0x00D01630. Its
temp-destruction flag word alternates between two registers
(`CVar13 = CVar12 | 1 ... CVar12 = CVar13 | 0x20`) and is parked in
`CStack_124`, a slot also reused for the "Gate1GuardInner" string and a
vtable pointer. `drop_eh_state_flags` missed it: its generic copy groups also
join through literal inits (`X = 0`), which merged 14 unrelated locals, and
the slot's non-flag uses made the group impure. `scratchValue2 | 32` then read
nil on the path that skips the first phase.

`_relay_slot_flags` builds its own components, seeded by bit-set register
copies (`A = B | K`) and closed over register copies only. Stack slots join
only as relays: a store `S = R [& K]` or a reload `R = S`. A component is one
flag when every register line is a flag shape, a copy between members, or a
relay. The slot must also be safe: after each flag store, nothing but a
reload may read it before its next real definition. The relay lines are then
dropped, the registers are spelled as one name, and the existing single-flag
rule removes it. Slot-free groups keep the old path unchanged.

All-unit A/B against HEAD (`work/st0_division_switch_ab`): beyond the earlier
five files, the same shape is removed from Bandit Camp's Assassin1 and
CampHostageGuard, one Guild Training function and Trader Conflict's
TC_BanditFighter. Each diff is pure flag removal: no removed name has a
remaining reference, and the removed `if` blocks only updated the flag.
CampHostageGuard's real `u_stk_b4` switch state is kept. Guild Training's
readable stage is hand-reviewed and was not regenerated; only its draft
changed.

Result: Bandit Camp readable smoke is 19 files / 0 problems, 118 TODOs.
Trader Conflict smoke is 15 files / 0 problems. Evidence:
`work/bandit_camp_relay_flag_smoke.json` and
`work/trader_conflict_relay_flag_smoke.json`. Tests: two more in
`test_native_st0_division_switch.py` (the flag is removed while the
slot's string life survives; a slot read as a real value keeps the flag).

## Kill groups: MsgGetThingsKilled's word list

Bandit Camp's main gameplay reads the words MsgGetThingsKilled returns.
The existing sidecar binding returns only the bool and frees the vector.

Retail semantics, from the disassembly of `CGameScriptThing::MsgGetThingsKilled`
0x008D3DD0: it walks the thing's event list. For each type-0x22 event in the
current frame's time window, it pushes the event's +0x8 word, which is
`CEventKilledCreature::CreatureGroupOfKilledThing` (PDB layout). It returns
true exactly when it pushed one.

- **CheckAnyBanditsKilled 0x00D032F0:** counts the words with bit 4 (the
  bandit creature group). The first bandit kill clears
  `BanditCampKillNoBandits`; reaching global data 0xE88
  (`BAC_BoastBanditKill`) sets `BanditCampKillManyBandits` and completes
  the boast's info counter.
- **Both area-massacre checks** (Outer 0x00D0EE70, Residential 0x00D0F640):
  add the word count for the hero, each follower and each summon, and
  compare the total with 0xE78 / 0xE7C.

The draft had lowered the bit test to `if false`, so no kill was ever
counted.

- **Sidecar:** a new binding, `thing:MsgGetThingsKilledGroups()`, returns the
  words as a Lua list (empty = false). Retail's CRT fills and frees the
  vector, as in the GetAll* overrides. It is saved as
  `tools/script_recovery/sidecar_patches/novi-zzzzzzzz-things-killed-groups.patch`,
  diffed against `sidecar-abi-v13`, the source of the live v16
  NoviCompatibility.dll (hashes match). A scratch copy of that tree compiles
  Release|x86 with 0 errors. It is NOT deployed, and the pinned v16 bundle
  is unchanged. `build_novi_compat_bundle.py`'s staging path is stale: its
  `novi-unit-bindings.patch` no longer applies to the current
  ForgeFSE-retail-shadow.
- **Converter:** `fold_things_killed_vectors` runs after the element-call
  fold (follower calls are then `MsgGetThingsKilled(elem, &V)`).
  - The call fills the list, and the result is `#V ~= 0`; a char-typed
    result is tested as a boolean.
  - `E - V >> 2` becomes `#V`. Element reads through register copies of
    V / E become `V[i + 1]`.
  - The vector constructor, zeroing and erase become `{}`; the free is dropped.
  - Any other use of a copy leaves the function untouched.
  - The massacre total, kept in a string-typed slot, gets its own local for
    its integer life. The lifter leaves a stack slot's self-update unlifted.
  - The Trader Conflict bool-only use keeps the old strip rule.
- **Smoke:** the stub now returns `{}` for this binding.

## GSI vtable scope and interleaved colours

Two more operand losses in the same unit:

- **AddQuestInfoCounter lost its id.** CheckAnyBanditsKilled loads the GSI
  vtable into `iVar5` and reuses `iVar5` for the result:
  `iVar5 = (**(iVar5 + 0x51c))(..)`. `isolate_gsi_vtable_temps` ended the
  alias scope at that same line, so the call stayed raw and
  `UpdateQuestInfoCounter` lost its id (the lifter back-filled `scale`).
  The scope now includes a reassignment whose right side is the vcall. The
  same fix lifts BanditKing's Twinblade `AddQuestInfoBar`, and its
  `UpdateQuestInfoBar` / `RemoveQuestInfoElement` now get the bar id instead
  of `0`.
- **The Twinblade bar's colours were unresolved.** The two colours'
  byte stores are interleaved with each other and with setup lines.
  `gather_colour_byte_stores` moves each stack slot's four stores
  together inside a run of plain assignments (unless the slot is read
  inside that run), so `fold_stack_colours` folds both. They are
  `{R=255,G=0,B=0,A=255}`, from the native bytes.

Result: Bandit Camp is 85/85 functions and 19/19 files with 91 TODOs;
readable smoke 19 files / 0 problems (`work/bandit_camp_kill_groups_smoke.json`).
All-unit A/B (`work/st0_division_switch_ab/summary.json`): outside Bandit
Camp, only the four files already reviewed above change, and they are
identical to the pre-kill-groups comparison. 16 regression tests.

Full script-recovery suite: the failing tests (Maze host correspondence,
Father intro emulation, New Oakvale barrel and Bully cases, install-state
audits) fail identically on HEAD code. This was checked with a scratch
overlay root: the same tools with my four modules reverted, and junctions
to the checkout's data.

## Parameter operands and bound-value worker spawns

- **OpenGate 0x00D0EBC0** called `GetThingWithScriptName(&result, &door_name)`
  with its own by-value parameter. Every `&` operand was dropped as an out
  slot, so the gate lookup went out with nil. A function parameter passed
  by address is now the operand. A `(CCharString *)&` cast on it types
  the parameter as a string, because Ghidra typed it `int`.
- **Gate2Guard1 Main 0x00D0D910** is the only caller. It spawns
  `ParentClass.OpenGate` with bound values: `operator new(0x44)`, +0x3C =
  2.0 (the delay) and +0x40 = a copy of the "Gate2Outer" temporary. The
  lifter left it as TODOs, so the gate never opened.
  - `RE_THREAD_VALUES` / `_bound_thread_values` lower this to
    `quest:CreateThread("OpenGate", {args = {2.0, "Gate2Outer"}})`, like the
    existing captured-thing spawns.
  - Fields must be literals or literal-built strings that exactly fill the
    object from +0x3C. The member name is the literal just before
    "ParentClass.", not the first literal in the block.
- **Trader Escort changed behaviour (a fix).** The same operand rule changes
  MakeTraderComment 0x00E01900. It reuses its parameter slots for the
  "RockTrollTrigger" / "EARTH_TROLL_OFFSCREEN_ROAR" temporaries. The old Lua
  looked up a thing named EARTH_TROLL_OFFSCREEN_ROAR and passed a wrong
  operand as the sound, so the troll's off-screen roar never played. It now
  plays on RockTrollTrigger, as in retail. The pinned v16 bundle is
  unchanged; the next Trader Escort bundle should listen for the roar
  before the troll encounter.

All-unit A/B: beyond the files already listed, only Gate2Guard1 and
Trader Escort's two MakeTraderComment copies change. Smoke: Bandit Camp
19/0, Trader Escort 16/0. 85 TODOs. Tests: 22 in
`test_native_st0_division_switch.py` plus the existing spawn-capture tests.

Next for Bandit Camp: an in-game run needs a sidecar build that includes
the kill-groups patch (v17) and a Bandit Camp bundle entry. Both wait on
the user freeing the game install. The remaining 91 TODOs are mostly native
labels, cleanup-order notes and the hostage cutscene slots.

## Live runs (v17 / v18, from `adult_maze2_completed_2026-09-25`)

Config: `tools/script_recovery/runner_quests/bandit_camp.json`. Bundles are
staged with `tools/script_recovery/stage_bundle_with_units.py`: v16 byte for
byte, plus the scratch sidecar build (sidecar-abi-v13 + the kill-groups patch)
and Bandit Camp's readable packages as overrides. The manifest is recomputed.

- **bc1:** the card's table title is "Find The Bandit Seeress", not
  "Bandit Camp". OCR listed all rows.
- **bc2:**
  - Evidence: all three Bandit Camp scripts and their entities load and run.
    The status expression calls `quest:GetHero():MsgGetThingsKilledGroups()`
    in the BanditCamp VM and gets a list back, so the new binding is live.
  - Failure: GiveHeroObject's item boxes paused the game, and the
    SetHeroAsWearing statements sent meanwhile did not take effect.
  - Failure: the runner's hostile-clearing reflex killed the gate guard,
    which ended his entity script.
  - Runner fixes: `setup` runs before travel and clears boxes, `setupCheck`,
    and `clear: false`.
- **bc3:** the pause ladder's RMB "no" answer fires whenever a status eval
  times out. With no question up, that is an attack in the world. Runner fix:
  a `pauseSteps` override; Bandit Camp uses `["clear"]`.
- **bc4:** the guard still attacked on arrival with no input.
  - The World Cullis Gates menu came from the hero standing on the camp's
    Cullis pedestal. The landing point (1945,2112) is beside
    OBJECT_GUILD_PEDESTAL_TELEPORT_01 (1937.0,2112.7 world). The landing is
    now the retail arrival spot BanditCampHSP (1944.7,2109.7).
  - Re-applying FACTION_TWINBLADE_CAMP_BANDITS and clearing the guard's enemy
    target changed nothing. All 9 retail references to that faction string
    are EntitySetInFaction calls in this family.
  - `quest:ClearHeroEnemyOfGuards()` killed the game process (the log ends
    at the batch, with no ok/error line). Do not call it.
- **bcr1 (retail oracle):** v16 has no Bandit Camp override. Same save,
  disguise and teleport: the guards stay peaceful for three minutes and the
  hero never moves.
- **Cause:** Gate1GuardOuter Main's `if (AttackedOuterGateGuards == 0) goto
  code_r0x00d0283a;` (keep waiting) was unlifted, because the lifter only
  knows `LAB_` labels. An unprovoked hero fell into
  `GiveThingBestEnemyTarget(me, hero)`. `normalise_typed_decompile` now
  spells `code_r0xADDR` as `LAB_ADDR`.
  - All-unit A/B vs 783df67: Gate1GuardOuter changes, plus a pure label
    rename in Bordello's Magicman (its `code_r` label had been synthesised
    as FLOW_native_label_1).
  - Smoke 19/0, 83 TODOs. v18 carries the fix.

Also seen: Q_BanditCampBossBattle's gate watcher calls
`GetThingWithScriptName('Gate1')` every frame, as retail does. The sidecar's
adopted reference count on that thing climbed to 2,540, so returned handles
are not collected between frames. This is a slow sidecar leak.



## Gates 1 and 2 on the converted scripts (bc6 / bc7, v18)

- **bc6:** the disguised hero talks to Gate1GuardOuter. The full retail
  conversation plays ("Hello, mate. Nice Bandit gear." ... "Another one coming
  in, Joe. Open the gates."). Then the experience tutorial appears and
  `Gate1Open` becomes true. No hostility, 0 Lua errors.
- **bc7** uses the runner's new `steps` (talk to a script-named NPC, then
  wait for a Lua condition in the quest's VM, with a per-step pause ladder
  for yes/no questions), all hands-free:
  - gate1_open: 118 s.
  - guard2_first: 13 s. `SpokenToSecondGuard` is set.
  - forger_pass: 94 s. The Forger asks for 1,000 gold, the step's LMB ladder
    answers yes, "Excellent choice, my friend", and the hero holds
    OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS.
  - gate2_open: 23 s. `Gate2Open` is set. The log shows
    `Thread 'OpenGate' registered ... with 2 argument(s)`, then the lookup of
    'Gate2Outer'. This is the bound-value worker spawn converted this session.
    The region caption "Twinblade Elite's Camp" follows as the hero crosses
    into the residential camp.
  - 0 Lua errors.
- **Assistance used:** the disguise is given and worn, and 3,000 gold is
  given (the Forger costs 1,000).
- **Checkpoint copies** (profile folders; AutoSave plus the `.cp` checkpoint):
  `adult_bandit_camp_gate1_2026-09-25`, `adult_bandit_camp_gate2_2026-09-25`.
  The launcher loads AutoSave, and whether it restores the `.cp` state is
  untested.

Next leg: the residential camp and Q_BanditCampBossBattle (the hostages,
the Twinblade fight and its spare/kill choice), then MissionSucceeded.

## Boss leg attempt (bc8, v19)

- Two conversion bugs found before the run, both fixed in 136abc2:
  - BanditKingMissionProcess lost Twinblade: CreateCreature's result
    reaches the stack thing through 0x00704580, a counted-pair assignment
    left as a TODO.
  - The quarter-health threshold became "" in its float copy-back.
- Steps 1-4 replay hands-free as in bc7:
  - gate 1: 119 s
  - guard 2: 10 s
  - Forger: 89 s
  - gate 2: 25 s
- kill_hostage_guard, first try: `fightStep` found no CampHostageGuard,
  because the residential level is not streamed in while the hero stands at
  gate 2. A manual GoToMapSlotRetailTransition(15, ...) fixed it, and the
  step now travels there itself.
- kill_hostage_guard, second try: the guard exists as a script thing at
  his spawn point (1713.6, 2075.6), alive at 1 health, 1.6 to 6 m from the
  hero. But he never appears in any captured frame, and 36 fight iterations
  of real sword input never hit him. His Init ran (the key went into his
  inventory) and his Main is acquiring its resources. Open question:
  hidden, not drawn, or not physically placed? A TC_BanditHostageKeeper is
  placed at nearly the same spot (1715.2, 2076.7).
- Retail oracle bcr2 (v16, same route, conditions checked from LUAGameflow)
  is running to see whether the retail guard is visible and killable.
- Retail also lets the hero pickpocket the key: the guard's loop polls
  MsgOnHeroPickedPocket.
- **Retail oracle bcr3** (v16, native Bandit Camp scripts, the same route with
  conditions checked from LUAGameflow): gate 1 took 124 s, guard 2 10 s,
  the Forger 95 s and gate 2 22 s (ours: 119/10/89/25). The retail
  CampHostageGuard is equally out of reach: a script thing 1.5 m away,
  never in frame, and the sword hits nothing. So this is not a conversion
  defect; the fight route is wrong for this guard at this hour. The config
  now gives the key as logged assistance (`do` step action), and the cage
  door frees the hostages because the guard is more than 5 m away.
- **bc10 (v19, key assist):**
  - Steps 1-4 replay (117/10/92/27 s), and the key arrives in the
    residential camp.
  - The cage door never registers the hero's use. The TAB prompt shows and
    TAB was pressed several times (focused), but `HostagesRescued` and
    `PlayGuardTooCloseCutscene` both stay false. The door's entity Main is
    running (entered, never exited), and MsgIsUsedByHero matches retail
    (CScriptThing slot 0x50 forwards to the implementation's +0x50).
  - 0 Lua errors.
- **Retail oracle bcr4** (v16, the same key assist and door TAB): the door
  reacts at once and plays "Twinblade's hostages have escaped! Grab them
  before they get away!". This is likely the guard-too-close branch, since
  the unseen guard stands about 3 m from the door.
- **Next:** a diagnostic run that polls the door thing's MsgIsUsedByHero from
  a runner VM every frame around the TAB, to see whether the engine reports
  the use at all under our bundle, and whether our door script's loop is
  actually ticking.
- **Correction (user, watching the game):** the hostage guard is visible;
  "never in frame" was my captures' framing, not the game. The sword
  misses are the known friendly-fire limit: the disguised hero is
  friendly to the camp, and swings on a friendly NPC register nothing
  (the Guild runs found the same). The retail oracle agrees because it
  is the same engine behaviour.
- **The door's "guard too close" is correct.** The guard patrols between
  GuardFirstMarker and GuardSecondMarker on a 10 s timer, and the intended
  play is to wait until he has walked off (or distract him), then open
  the cage. bc11 emulates that by teleporting him away after giving the
  key; a retail-faithful lure is still open.

## bc11 (v19): hostages freed; the boss thread dies on a missing binding

- Steps 1-4 as before. hostage_key gives the key and moves the guard away
  from the cage (emulating the retail lure). free_hostages: the converted
  door takes the rescue branch and Gate3Open is set in 48 s.
- enter_boss fails. The log shows
  `LUA RUNTIME ERROR in thread 'BanditKingMissionProcess': BanditCampBossBattle.lua:95:
  attempt to call a nil value (method 'CancelUsingAbility')`.
  Retail calls GSI slot 0x638 twice (`CancelUsingAbility(0xf)`,
  `CancelUsingAbility(10)`) on entering the boss level. The sidecar already
  resolves `CancelUsingAbility_API = pVTable[398]` (0x638, GameInterface.cpp)
  but never binds it to Lua.

## 2026-09-26: Bandit Camp completes (bc12 / bc13 / bc14)

- **CancelUsingAbility bound** (`sidecar_patches/novi-zzzzzzzzz-cancel-using-ability.patch`,
  GSI 0x638 = `pVTable[398]`, range-checked against `MAX_NUMBER_OF_HERO_ABILITIES`). The
  converter reads the patch as binding evidence, so the two `TODO(native)` lines are gone
  from the regenerated unit.
- **bc12 (v21): quest completed.** It ran hands-free from `adult_maze2_completed_2026-09-25`
  through the boss: BanditKingMissionProcess survives arrival, the runner's `fightDrain`
  assistance brings the King to his 25% threshold (so the boss fight is not a real fight),
  and then the Theresa cutscene and the full `3_theresa_flashback_comp` video play, followed by
  `MissionSucceeded` and `SetQuestAsCompleted('Q_BanditCamp')` (Bandit Seal, 4,500 + 6,550 gold,
  1,000 + 1,238 renown). The Quest Completed screen had no box for `clear`; one ENTER was sent
  by hand through the runner's input channel, then the runner finished and harvested
  `adult_bandit_camp_completed_2026-09-25`.
- **v21 had lost `MsgGetThingsKilledGroups`.** bc12 logged 2 runtime errors
  (CheckForFirstAreaMassacre / CheckForSecondAreaMassacre: nil method). The v17-v19 DLLs came
  from the previous session's scratch copy, and `sidecar-abi-v13` never had the
  things-killed patch. **The live sidecar = sidecar-abi-v13 + every patch after
  novi-zzzzzzz (things-killed, cancel-using-ability), applied in a scratch copy**; the base
  itself stays as v16's source (GOTCHAS).
- **Hostage guard: a converter bug, not a missing lure.** CampHostageGuard.Main (0x00D09010)
  reuses the "CampHostage" string temporary's stack slot as the patrol-leg byte
  (1 = GuardFirstMarker, 2 = GuardSecondMarker, flipped every 10 s). The lifter dropped both
  stores and folded the test to `1 == 1`, so he never left the door. `split_scalar_slot_tail`
  (native_evidence_lowering.py, `test_scalar_slot_tail.py`) recovers it; all-unit A/B:
  only CampHostageGuard.lua changes (3b9602b). The readable pass got slightly worse in that
  function (two temps merged into `getStateBool`, four GetHealth results no longer inlined):
  cosmetic, open.
- **bc13 (v22 = both bindings + the patrol fix; runner teleport removed): the guard walks
  off by himself** (hostage_key passes in 69 s on distance > 6 from the door), the hostages
  are freed through retail's rescue branch, and the log has 0 Lua errors. Two runner faults:
  the quest-wide `key ENTER` I had added skipped the flashback video (84 s vs 182 s), and the
  Quest Completed screen paused frames right on arrival, so `travel()`'s region wait got no
  replies and failed the step with the quest already complete (nothing harvested). Fixes:
  ENTER now lives only in `back_to_residential`'s `pauseSteps`, and `wait_for` runs the
  current step's pause ladder while its checks go unanswered.
- **bc14:** **done, 0 Lua errors.** Every step on retail mechanics except the key gift and the drained boss: the guard walks off by himself again (hostage_key 55 s), the Theresa scene plays the full video (179 s), and `back_to_residential` crosses cleanly. The Quest Completed screen still needed one hand ENTER, because past the last step the main loop fell back to the quest-wide `["clear"]` ladder; both ladder sites now keep the last step's `pauseSteps`. Harvested `adult_bandit_camp_completed_2026-09-26` (use this checkpoint). 
- **bc15 (the same config, fixed runner): zero manual input.** done, 0 Lua errors; the runner pressed ENTER once on the Quest Completed screen and nowhere else (Theresa scene 166 s, video intact).

## Resume here (next session)

1. Rebuild the sidecar if needed: copy `work/new-oakvale-original-fse-20260912/sidecar-abi-v13`
   to the scratchpad, `git apply` `novi-zzzzzzzz-things-killed-groups.patch` then
   `novi-zzzzzzzzz-cancel-using-ability.patch` there, MSBuild Release|x86 (`-p:` switches).
   Bundle v22 already has this DLL (local-candidate-v22).
2. Next quest after Bandit Camp in the adult chain: start it from the harvested
   `adult_bandit_camp_completed_*` checkpoint with a new runner_quests config.
3. Open follow-ups:
   - the hostage key is still given by the runner (`do`); find the retail source (pickpocket
     / the Forger) and drive it;
   - the boss fight is drained, not fought: an unassisted King fight is untested;
   - CampHostageGuard readable-pass regressions (see above);
   - the sidecar's per-frame GetThingWithScriptName reference climb;
   - `ClearHeroEnemyOfGuards` crashes the game (do not call it);
   - ForgeFSE canonical fork (D:\Code\ForgeFSE-retail-shadow) lacks the whole
     NoviUnitBindings layer and the new bindings; port on a fresh branch
     (its uncommitted changes are another session's);
   - Codex's two untracked runner files (runner_checkpoints/second_maze_completed.json,
     runner_quests/guardian_sister_info_2.json) are still uncommitted.
