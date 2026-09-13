# Maze Research: native recovery, disabled

Selected as the next recovery target after Meet Sister: the root lifecycle is
identified in `native_clusters/V_MazeResearch.json` and there are two bound entity
scripts. Guardian Trophy Dealer has a larger dialogue/spawn surface; Dragon Boss
Fight needs combat/camera acceptance. This selection does not assert that Maze
Research is ready to activate. A disabled executable source package is now staged
under reconstructed/MazeResearch/FSE and has passed the offline checks below.
The seed corpus remains unchanged and is not a parity oracle.

## Provenance and reproduction

Retail Fable.exe SHA256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.
Addresses below assume image base 00400000.

Run from the repository root:

```
python work/runtime-20260912-video-comparison/recover_maze_research_entities.py
python work/runtime-20260912-video-comparison/prove_maze_research_unlimbo.py
```

The first archives bytes, reachable instructions, branch edges, calls, vtables
and literal strings in native-entity-inventory-20260912.json and
native-entities-20260912.asm. All branch successors stay within the reviewed
function ranges. It does not follow calls or prove their runtime effects.
The second executes actual Init/UNLIMBO instructions under Unicorn, including
CB7940 termination queries. String/map/engine calls and frame scheduling are
doubles; six cases pass. Rendering and cutscene flag production are untested.

## Native contract recovered so far

Root allocator EA8690; constructor EA76E0; Init EA7750; RegisterMain EA7760;
Main EA7800; OnPersist EA80A0; destructor EA86C0. Root Init clears SwordTaken
at +48 and BookRead at +49. OnPersist transfers those two booleans, default false.
Main binds EmptyGrave then HistoryBookcase, both flags zero, then sets objective
01 in HeroGuildComplexInside. Entity allocators EA7990/EA7A60 install vtables
12EE4C0/12EE4DC respectively. Native entities use their bound thing at entity+8;
the seed's additional self-name lookup and null-abort path are not native.

HistoryBookcase Init EA7B10 is empty. Main EA7B40..EA7EE9:

- Already BookRead: frame/termination idle loop.
- Otherwise checks termination, makes the bound bookcase usable, adds quest
  marker, waits for MsgIsUsedByThing("SCRIPT_NAME_HERO") with frame/query loops.
- Checks termination again, sets objective 02, removes bookcase marker and adds
  EmptyGrave marker; writes BookRead=true before journal entry 10.
- Displays entries 10, 20, 30, 40. Each wait polls clicked-past, yields and checks
  termination. Additional queries occur after each wait, including when the
  initial clicked-past result is already true. The seed omits these guards.
- Enters the frame/query idle loop; does not deactivate the quest here.

EmptyGrave Init EA80F0 initializes parent+58 map["UNLIMBO"] to false. This is
missing from the seed. EmptyGrave Main EA8130..EA85D6:

- If BookRead, checks termination and adds its marker. Looks up GoodSword into
  the parent's retained thing at +4C; sets limbo(true,true), then checks termination.
- Waits for Hero use. Requires BookRead, !SwordTaken, master+4 > 1700 and
  master+88 == 2, with termination queries around the gated action.
- Constructs a resource, performs one Hero acquisition at priority 4, constructs
  HERO actor map and empty-named movie; pause(true), cutscene-skippable(true).
- Allocates and schedules ParentClass.UnLimboSword via CB7E50, callback EA85E0,
  capturing the parent at task+38. The parent+58 boolean map is also passed to
  the cutscene macro. Master+8C selects SAINT (true) or REDEEM (false).
- Each macro branch has a pre-call termination guard. Both invoke CBFB7D with
  the shared map, no input map, setup false, skippable true.
- After macro: skippable(false), remove marker, unpause, destroy movie/map/Hero
  resource, SwordTaken=true, remove marker again, deactivate later(0), then
  resume frame/query loop. The seed's immediate return differs. No added
  post-macro termination guard is justified by these instructions.
- Pre-macro cancellation unpauses and destroys locals without awarding the
  sword state/deactivation. Full body execution/replay remains to be implemented.

UnLimboSword EA85E0..EA868E waits for parent map UNLIMBO, querying termination
after each frame and again after observing the flag. Only then it calls
EntitySetInLimbo(parent sword,false,true), EntitySetAlpha(parent sword,0.0,true).
Six native cases cover Init overwriting true, immediate flag, delayed flag,
cancellation while waiting, flag+cancel together and already-cancelled true flag.
Cancellation cases perform neither engine call. The seed currently makes those
calls before the macro, losing the native flag-triggered timing.

## Dependencies and remaining support work

Assets/names recovered directly from literals: EmptyGrave, HistoryBookcase,
GoodSword, SCRIPT_NAME_HERO, HERO, HUD_ORB_QUEST_VIGNETTE,
HeroGuildComplexInside, objectives 01/02, TEXT_QST_B01_JOURNAL_ENTRY_10/20/30/40,
CS_GET_SWORD_OF_AEONS_SAINT and CS_GET_SWORD_OF_AEONS_REDEEM. Actual placement,
macro flag-writing command and skip-path behavior still need asset recovery.

Existing runtime exposes CreateThread, GetCutsceneFlag, limbo and alpha methods.
However, scoped RetailResources.RunMacro currently passes null flags and inputs.
RunCutsceneWithSetup builds a local flags map; GetCutsceneFlag reads the separate
m_pCutsceneFlagsBuffer used by the older cutscene helper. Those methods cannot
simply be combined to reproduce this shared parent-owned flag map. Do not switch
to a helper with implicit actor-acquisition retries or lifetime changes.

Needed before an executable parity claim:

- Recover macro UNLIMBO writes and behavior on Escape/cancellation.
- Design/test explicit flag-map ownership and thread access beyond a blocking
  macro call; verify task creation ordering and lifetime through deactivation.
- Execute both full entity bodies against engine doubles, then compare reference
  traces including termination at each journal/action boundary.
- Verify master-state field bindings and map/resource copy/destruction helpers.
- Stage disabled Lua only after these contracts are supported. No shared API
  manifest regeneration or converter edits are part of this recovery.

Installed v23 and all game assets remain unchanged. These findings do not resolve
New Oakvale fade visibility, Father Escape acceptance or perceived speech volume.


## Completed offline milestone: source port and runtime support

Recovered both installed macros (indices 332/333, 35 normal and 11 skip commands
apiece). Normal sets UNLIMBO after the first speech, then pauses 0.1 seconds before
the appearance effect and 0.4 before FadeThingIn. Skip sets UNLIMBO, resets walking
speed, yields two script frames, removes GoodSword, then yields and grants the
weapon. Both use the shared flag map. Exact commands and payload hashes are in
installed-macros-20260912.json. Seven unique authored records are archived in
installed-placements-20260912.json, including both cameras and both Hero markers.
The installed script.bin and TNG match their archived hashes.

Native full-body execution now covers 11 HistoryBookcase and 20 EmptyGrave cases,
plus six separate Init/UNLIMBO cases. The main-body execution stubs engine/resource
internals and scheduling; EmptyGrave lookup deliberately uses an empty wrapper to
exercise null-ref copy paths. It does not establish game behavior for missing
actors, failed allocations, real resource internals or streaming. The Lua
reference and staged entry scripts/adapter match all 31 body traces plus five
UNLIMBO outcomes (Init's map write is a separate native case).

Added LuaRetailFlags.h, implementing the eight-byte map header and 0x18 sentinel
from native constructor EA7716..EA773B, bool operator 8ADF10 and destructor 7ADD50.
Quest:RetailFlags(name), RetainRetailThing(name,thing) and GetRetainedRetailThing(name)
provide transient quest-owned objects shared through C++ across isolated entity
and quest Lua VMs. RetailResources:RunMacroWithFlags passes that same map and keeps
a shared owner over the blocking macro. The port schedules UnLimboSword with an
empty region string and no cross-VM Lua arguments. Root persists only SwordTaken
and BookRead. The map is reset by EmptyGrave Init, matching native.

Also fixed SetThingAsUsable to accept a borrowed CScriptThing pointer. Actual sol
previously converted the entity's raw me into an empty shared_ptr; the exploratory
failure is work/runtime-maze-flags-check-20260912/raw-shared-probe.log. The new
check compiles the current method body verbatim and verifies borrowed, owned and
nil arguments. No global coercion or other API parameter conversion changed.

Validation:

- python -m unittest tools.script_recovery.test_maze_research_reference -v:
  eight tests, including 31 reference and 31 staged body scenarios, five reference
  and five staged UNLIMBO scenarios, source-copy identity and disabled activation.
- Clean C++/Lua runner: work/runtime-maze-shared-state-20260912/result.json passed.
  Covers actual shared quest methods across two VMs, retained sword last-owner
  cleanup, exact flag pointer forwarded to the macro, null-map rejection, the
  usable binding, 14 Meet Sister scenarios/255 events and 17 conversation operands.
- Release x86 solution build passed. Initial Win32 solution/project invocations
  were setup failures; final command/log are under work/maze-runtime-release-20260912/.
- New Oakvale package: all 122 fixtures passed, zero errors/warnings.
- Standalone package validator checks traces, Lua syntax, identity, native scenario
  inventory, macro flag ordering and unchanged installed asset hashes.

Source DLL: 2CEA2E887B1646397A13C3B6ED936DFCE5C28944486B45A9392C5A91AFC5352B.
Both installed DLLs still match exact v23 C3900B3D...559D31 (full hashes archived).
No game launch, installation, shared API manifest/overlay, converter or seed edit.

Remaining acceptance boundaries are actual task admission/fiber scheduling,
streaming/reload, engine resource lifetimes, normal and Escape macro playback,
weapon grant, and cancellation during real macro execution. Existing native
instruction tests deliberately double those engine operations. API metadata
changes are prepared separately; do not enable the staging override on the
New Oakvale profile or replace its shared override file.
