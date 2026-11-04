# Lua recovery — 2026-09-30

## Magicman beer-award operands (offline v61)

Resumed the incomplete marathon24 constructor-slot candidate. Retail calls
`00E42907` and `00E4448D` push the constructed beer name, `-1`, and `true` for
`GiveHeroObject`. The old draft omitted the last two operands; its readable
projection rendered five beer-award branches with amount `0` or `scriptThing`.

`recover_dropped_operands` now accepts a retained printed CCharString array as
the recovered native stack address when the latest preceding constructor for that
printed array has the same native `ecxStack` slot. The alias is limited to the
`GiveHeroObject` binding. An initial broad candidate changed Yes/No question
calls in Beggar and Bordello; that candidate was stopped after the first A/B
rows and was not promoted. Focused tests reject absent or mismatched constructor
slots. The exact retail Magicman function recovers both calls as `(name,-1,1)`
in intermediate C. Fresh generated draft and readable Magicman calls use
`GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)`; the Lua test executes all
two draft and five readable call sites against an argument-checking mock.

The final full 29-unit A/B changed only
`V_Bordello/Entities/Magicman.lua` in draft and readable outputs, with zero
registered baseline drift. Forty before/after Bordello Lua files parse;
readable syntax is clear. Before/after Bordello smoke JSON is identical (six
pre-existing problems, zero new). The 371-file readability corpus has zero
syntax failures. Promoted only the Magicman draft/readable pair and their
reports; refreshed `CURRENT_LUA_READABILITY_REVIEW.md`.

Bundle `work/new-oakvale-original-fse-20260912/local-candidate-v61` derives
from v60. Only its Magicman Lua payload changed; 210 package Lua files parse,
216 manifest hashes match, and the stock and compatibility DLL hashes are
unchanged. This is offline evidence, not Magicman gameplay proof. Scratch
scripts and reports are under `work/codex_lua_marathon25_20260930`.

## Book Collecting live replay gate

The retained marathon22 checkpoint has 22 saved donations, 16 good books, the
good hat and zero silver keys. Its two AutoSave hashes matched its receipt.
The runner staged that checkpoint in a separate temporary profile and launched
v59 with `key_resume_22.json`; startup logged stock registration and the
compatibility hook. All frontend captures were black, so the driver classified
each as `other` and stopped before the Load Game screen. No quest steps ran;
no silver-key or fresh-reload claim follows. The desktop itself captured
normally during diagnosis, so this is a game-window capture/frontend gate.

The runner restored the live save slot. The temporary profile was removed,
and all 561 original save files match their pre-run hashes exactly. The
runner log is `work/runner/v59_key_resume22_20260930_FableScriptExtender.log`.
The book22 checkpoint remains under marathon22 for a later replay when the
game window can be captured. Other GUI work was visible on the desktop during
the failed launch; avoid another blind retry while that shared GUI is active.

## Next offline work

Magicman still has unverified movie/resource lifetimes and other health operands.
Book Collecting's final three donations and silver-key receipt/reload remain
the next live gate. The v61 package has not been installed or launched.

## GetScriptThing health operand investigation (unpromoted)

Magicman has 30 native `GetScriptThing` calls before `GetHealth`, using three
receiver slots: entry ESP -868 (13 sites), -852 (15), -832 (2). In 23 export
rows, the register used to push the hidden result is also reported as an EDX
operand. At `00E4168A` the bytes push EDX from a stack LEA, set ECX to the
resource receiver and call `0x7E7490`, whose callee purges four bytes. The
ordinary stack aligner rejects the duplicate operand; Ghidra's drifted name
therefore survives and yields suspect Lua resource identities.

A generic callee-purge alignment candidate corrected that receiver in
intermediate C and passed four focused tests, but scratch Bordello regeneration
also rewrote unrelated movie and pause arguments. In one cleanup path, the
existing `PauseAllNonScriptedEntities(false)` became state-dependent. It was
rejected before full A/B or promotion. Candidate source, test, diff and draft:
`work/codex_lua_marathon26_20260930`. The active converter was restored
byte for byte to the validated v61 baseline. This is evidence for the next
targeted investigation, not a health-operand fix.

An exact native audit then checked all 30 sites: each sets ECX with
`lea ecx,[esp+0x28]` immediately after pushing the hidden result, and all
share the same printed Ghidra receiver name. The exporter splits them into
entry-relative offsets -868 (13), -852 (15), and -832 (2). The per-site JSON
under marathon26 pins native/source hashes and records the discrepancy;
resource lifetime across all branches still needs proof.

## GetScriptThing resource receiver correction (offline v62)

Reviewed native evidence now pins the Magicman resource constructor at
`00E40EE3`, its entry-relative stack slot, the full native/source hashes, and
all 30 `GetScriptThing` call sites. Each getter site pushes the hidden result
and immediately sets ECX from `[esp+0x28]`. The new
`native_resource_thing_receivers.py` helper fails closed on stale evidence and
changes only the receiver operand after ordinary stack restoration. It leaves
the movie and pause operands from the rejected broad alignment candidate alone.
The evidence row is in `V_Bordello.json`, keyed by Magicman's function address.

Six focused native-evidence tests pass, including stale hash/slot/site cases.
The fresh readable Magicman first-health test reaches `GetHealth` with the
acquired resource; the registered v61 version fails at the resource identity
assertion. All five Magicman behavior tests pass. The broader suite passed
79 tests before the added first-health test. Full 29-unit scratch A/B changed
only Magicman. Both Bordello stages match registered v61 everywhere in the
before build, and both after stages change only Magicman. Before/after smoke
JSON is byte identical (seven existing problems). The 371-file readability
audit reports zero syntax failures. Promoted only Magicman draft/readable and
their reports, then refreshed the readability ledger.

`local-candidate-v62` derives from v61 and changes only packaged Magicman Lua.
All 210 package Lua files parse; 216 manifest hashes match; both DLL hashes
are unchanged. No v62 install or game run has occurred. Scratch evidence and
validation are under `work/codex_lua_marathon27_20260930`. The native receiver
is established at the calls, but resource lifetime and all downstream health
branches still require review. Book Collecting's live gate is unchanged.

## Follow-up: Magicman chatter counter alias (unresolved)

After v62 promotion, inspected the remaining readable `ScriptThing(1)` call.
The source-order map ties it to retail `GetScriptThing` at `00E41A74`.
Disassembly at `00E41A2A` loads the chatter counter from `[esp+0x7c]`,
compares it with four, and may store one back; `00E41A70` sets the getter
receiver from `[esp+0x28]` after a hidden-result push. Restored C correctly
has separate `iStack_358` and `xStack_364` names. A traced lowering pass shows
`canonicalise_stack_objects` merges the counter into the resource at line 4320
of `native_evidence_lowering.py`. The draft then assigns `1` to the resource,
and its readable projection passes `1` to `ScriptThing`. The issue already
existed before v62; the receiver correction does not cure this call. Preserve
the trace and restored/lowered C under marathon27. Next step: establish the
counter's full native lifetime and a narrow scalar alias rule, then run fresh
scratch A/B and branch behavior tests before any additional promotion.

## Magicman chatter counter (offline v63)

Native `00E40FAA` loads one and `00E40FAF` stores the chatter counter at
`[esp+0x7c]`. `00E41A2A` reads that slot, compares with four, and may reset it
to one at `00E41A38`. `00E41B63` reads the counter again before its increment.
The adjacent `00E40FB3` initializes a different counter. At the chatter
health query, `00E41A70` supplies the resource receiver from `[esp+0x28]`
after the hidden-result push. Ghidra called the initial counter `uStack_318`
and later uses `iStack_358`; the lowerer merged those later uses into the
resource. Other `iStack_358 = 0` statements are constructor member zeroes and
remain untouched. The reviewed evidence row and
`native_reviewed_stack_aliases.py` separate only the exact counter uses;
native/source hash and instruction checks reject stale evidence.

The full 29-unit scratch A/B changed only Magicman. Its before draft and
readable builds matched registered v62 with zero drift; its after builds
changed only Magicman. The readable diff after normalizing renumbered
temporary locals changes counter init/wrap/increment, the chatter tag number,
and `ScriptThing(1)` to `ScriptThing(resource)`. Before/after Bordello smoke
JSON is byte identical (seven existing problems). The broad converter suite
passed 87 tests; the updated Magicman test drove five consecutive chatter
interactions, observing tags 1, 2, 3, 4, 1 and the acquired resource at each
health query. The 371-file readability audit has zero syntax failures.
Promoted only the Magicman draft/readable files and reports, and refreshed
the readability ledger.

`local-candidate-v63` derives from v62 and changes only packaged Magicman Lua.
All 210 package Lua files parse; 216 manifest hashes match; both DLL hashes
are unchanged. This is offline evidence, not an in-game run. Scratch source,
native trace, comparison, smoke, tests and package validation are under
`work/codex_lua_marathon28_20260930`. Magicman's other dialogue counter for
`TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0` still folds into a resource and is
the next offline target. The Book Collecting live gate remains open.

## Magicman talking-out-loud counter (offline v64)

Native `00E40FB3` initializes this dialogue counter to one at `[esp+0x44]`.
The four `GFIntToCharString_API` sites `00E4140A`, `00E414B7`, `00E42D01`
and `00E42DAE` all read that counter for
`TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0`. The later two printed C values
look like the resource (`ppuStack_364`); at their call sites native loads EDI
from `[esp+0x44]`. Native `00E41464`/`00E42D5B` reset the counter to one when
a text entry is absent, and `00E41506`/`00E42DFD` increment it before the
timer reset. The reviewed evidence row and `native_reviewed_stack_values.py`
pin the native/source hashes, instruction bytes and all four conversion
sites. Only two scalar snapshots and two conversion arguments change in
restored C. The prior Lua converted a resource to text and attempted
`resource + 1`; the candidate uses the numeric counter.

The fresh 29-unit A/B changed only Magicman. Both before Bordello stages
match registered v63; the readable after build changes only Magicman. The
before/after Bordello smoke JSON is byte identical (seven existing problems).
The broad suite passed 94 tests, including native stale-evidence cases and a
timer dialogue test that reaches tags 1 and 2. A separate scratch run also
exercised a missing second text entry and observed the native reset. The
371-file readability audit has zero syntax failures. Promoted only Magicman
draft/readable and reports; refreshed the readability ledger.

`local-candidate-v64` derives from v63. Only its Magicman Lua payload changed;
all 210 package Lua files parse, 216 manifest hashes match, and stock and
sidecar DLL hashes are unchanged. No v64 install or game run has occurred.
Scratch evidence and results: `work/codex_lua_marathon29_20260930`. The
Book Collecting 22-donation live gate is unchanged. Magicman's remaining
native gates include a nil `p0` flag in the chatter path, movie cleanup, and
other health branches.

## Magicman chatter temporary cleanup (offline v65)

Native `00E418A5`/`00E418AC`/`00E418B3` construct the temporary
`CCharString` for `OBJECT_HERO_HAT_PIMP`. `00E418B8` sets bit `0x20` in
`[esp+0x20]`; `00E418E8` tests that bit, `00E418F3`/`00E418FD` clear it,
and `00E41901` calls its destructor. The `p0 & 32` branch in generated Lua
was this C++ cleanup flag, not quest state. Lua owns the string value, so the
reviewed rule removes only the pinned flag set and matching destructor guard.
The evidence row pins the native body, exported source, instruction bytes,
and source tokens; six focused tests reject stale evidence.

The chatter behavior test now reaches five interactions without supplying a
`p0` value, observing tags 1, 2, 3, 4, 1. The broader suite passed 100 tests.
Full 29-unit scratch A/B changed only Magicman; before Bordello draft and
readable match registered v64 with zero drift. Before/after smoke JSON is
byte identical (seven existing problems). The 371-file readability audit has
zero syntax failures. Promoted only Magicman draft/readable and their reports,
then refreshed the readability ledger.

`local-candidate-v65` derives from v64 and changes only packaged Magicman
Lua. All 210 package Lua files parse, 216 manifest hashes match, and both
DLL hashes are unchanged. No v65 install or game run has occurred. Scratch
evidence and validation: `work/codex_lua_marathon30_20260930`. The Book
Collecting live gate remains open. Next offline target is the incorrect
Magicman movie/resource cleanup operands in several health branches.

## Follow-up: attacked branch movie cleanup (under review)

The attacked branch starts a movie at `00E41D20` through virtual slot
`+0x5c8`. Its movie argument is selected by `lea eax, [esp+0x90]` at
`00E41D10`, before two argument pushes. The early cleanup at `00E42023`
calls counted-pointer destructor `0x006E7AB0` with `lea ecx,
[esp+0x98]` at `00E4201C`. The typed export reports entry-relative
slots `-760` for the movie argument and `-732` for that cleanup receiver.
A **third** movie starts at `00E420EC` on the presented-item path, with
its cleanup at `00E4293C`; its typed slots are `-816` and `-788`.
Both pairs differ by 28 bytes, suggesting a repeated stack-depth error
across a vtable call. The converter emits the first pair as `movie2` then
`DestroyMovie(movie)`, and the second as `movie6` then
`DestroyMovie(movie5)` in readable Lua. The apparent aliases need
stack-pointer and exit-path review. No correction has been promoted.
The scratch probe `work/codex_lua_marathon31_20260930/probe_attacked.py`
drives a hit, beard present, and zero health through the generated readable
Lua. v65 calls `DestroyMovie(nil)` after `StartMovie` returned a movie
handle. A scoped prototype that substitutes the attacked branch's
`movie2` handle reaches the expected cleanup. This demonstrates the Lua
failure and a possible repair, but does not establish all native exits.
`movie_calls.json` inventories six native starts and 25 direct movie
cleanup calls with instruction bytes and typed stack slots.

## Magicman attacked and beer movie members (offline v66)

The raw Ghidra body constructs the attacked movie at `appuStack_304` and
cleans its counted-pointer member `uStack_2fc`, eight bytes into that
object. The presented-item movie likewise pairs `appuStack_33c` with
member `uStack_334`. The restored C had split those objects into different
stack names: attacked `xStack_2f8` versus `xStack_2dc`/`xStack_2e4`, and
presented-item `xStack_330` versus `xStack_314`/`xStack_31c`. The reviewed
rule rejoins only these two scopes, checking the raw source and native body
hashes, start/cleanup call operands, and instruction bytes. Its six focused
evidence tests pass. No generic stack-depth adjustment was introduced.

The attacked-path mock first reproduced `DestroyMovie(nil)` in v65 and now
observes the movie handle returned by `StartMovie`. A second scratch probe
showed the same before/after result for the presented-item movie. That probe
supplies the unrelated unresolved item-comparison result to enter the path;
the comparison itself remains unreviewed. The broader converter suite
passed 107 tests. Full 29-unit scratch A/B changed only Magicman, with both
Bordello before stages matching registered v65 exactly. Before/after smoke
JSON is byte identical (six existing problems). The 371-file readability
audit reports zero syntax failures. Promoted only Magicman draft/readable
and their reports, then refreshed the readability ledger.

`local-candidate-v66` derives from v65 and changes only packaged Magicman
Lua. All 210 package Lua files parse; 216 manifest hashes match; both DLL
hashes are unchanged. No v66 install or game run has occurred. Evidence,
probes, comparisons, and validation are in
`work/codex_lua_marathon31_20260930`. Book Collecting's live gate remains
open. Magicman's later movie scopes and the presented-item comparison still
need recovery.

## Follow-up: presented-item comparison (under review)

The two retail sites `00E42048` and `00E43C08` call the thing vtable slot
`+0x8c` with an output address at `[esp+0x14]`. Each reads that output,
compares its string with `OBJECT_BEER_TANKARD` at `00E4207F` or
`00E43C3F`, and enters the beer path only on equality. The generated Lua
instead tests an uninitialized `unaff_EBP` global and a stale numeric
temporary, so the comparison can block the path. The sidecar's
`LuaEntityAPI::MsgIsPresentedWithItem` returns the boolean and publishes
the item name as `g_PresentedItemName` when it has text. A later recovery
must account for the sidecar leaving that global unchanged when text is
absent, then verify both native sites and branch behavior. v66 does not
change this gate.

## Magicman presented-item name (offline v67)

The reviewed rule uses both native `+0x8c` item polls (`00E42048` and
`00E43C08`), output-address loads at `[esp+0x14]`, the two comparisons
against `OBJECT_BEER_TANKARD`, and the matching branch instructions.
It pins the native/exported C hashes and the sidecar source snapshot that
publishes the name through `g_PresentedItemName`. The generated Lua clears
that global immediately before each poll, so a true poll with no item text
cannot reuse the previous name. Only an actual tankard name enters the beer
movie. Both invalid `unaff_EBP`/stale numeric comparisons and their two
TODOs are removed. Six focused evidence tests pass.

The focused Magicman suite passed 16 tests, including both item-poll paths
with tankard, other item, no text with a stale tankard global, and a false
poll. v66 skipped the tankard path in both probes; v67 reaches it. The
broader converter suite passed 121 tests. Full 29-unit A/B changed only
Magicman and the before Bordello draft/readable matched registered v66
with zero drift. Bordello smoke retained six existing problems; its only
JSON change is Magicman's `freeGlobals` entry from `unaff_EBP` to the
sidecar output `g_PresentedItemName`. The 371-file readability audit reports
zero syntax failures. Promoted only Magicman draft/readable and their
reports, then refreshed the ledger.

`local-candidate-v67` derives from v66 and changes only packaged Magicman
Lua. All 210 package Lua files parse, 216 manifest hashes match, and both
DLL hashes are unchanged. No v67 install or game run has occurred. Scratch
evidence and validation are under `work/codex_lua_marathon32_20260930`.
Book Collecting's 22-donation live gate and Magicman's later movie cleanup
scopes remain open.

## Follow-up: HeroTricking talk movie (under review)

The next talk branch in v67 calls `StartMovie("")` without retaining its
result, then uses a previous beer-movie variable at many cleanup exits and
passes the hero to `DestroyMovie` at its normal exit. Scratch
`work/codex_lua_marathon33_20260930/probe_tricking_movie.py` drives
`HeroTricking`, a hero talk, beard present, and zero health; v67 starts one
movie and then calls `DestroyMovie(hero)`.

The native start is `00E42E9F` (virtual slot `+0x5c8`). Raw Ghidra C
constructs `ppuStack_360` and its counted-pointer member `iStack_358`.
At `LAB_00e437b3`, the normal cleanup destroys `iStack_358` and resets
the `ppuStack_360` vtable before destroying the base. The transformed C
spells that constructor and cleanup differently, allowing the Lua lifter
to lose the movie handle. Other exits and reused stack names need review
before a correction is promoted.

## Magicman HeroTricking talk movie (offline v68)

Raw Ghidra C constructs the movie at `ppuStack_360`, with its counted-pointer
member `iStack_358`. Both shared cleanup labels `LAB_00e43023` and
`LAB_00e448cc` destroy that object, and the normal cleanup at
`LAB_00e437b3` releases the same member and base. Native sites
`00E42E9F` (start), `00E43033`/`00E448DC` (shared exits), and
`00E437D0` (normal release) are pinned with instruction bytes and call
operands. The first pre-lowering source prototype fixed the normal path
but also changed unrelated later operands; it was rejected. The reviewed
post-lift rule changes only this movie start and its 17 cleanup uses within
the lexical scope. Seven focused evidence tests pass.

The v67 talk probe passed the hero to `DestroyMovie` after starting a
movie. v68 passes the acquired movie at the normal exit and the
speech-failure shared exit. The final focused Magicman suite passed 18
tests. The broader suite passed 129 tests before the added shared-exit case;
the source and production helper did not change between those runs.
Full 29-unit scratch A/B changed only Magicman. Both before Bordello
stages matched registered v67; smoke JSON was byte identical (six existing
problems). The 371-file readability audit has zero syntax failures.
Promoted only Magicman draft/readable and reports, then refreshed the ledger.

`local-candidate-v68` derives from v67 and changes only packaged Magicman
Lua. All 210 package Lua files parse, 216 manifest hashes match, and both
DLL hashes are unchanged. No v68 install or game run has occurred. Scratch
evidence and validation: `work/codex_lua_marathon33_20260930`. Book
Collecting's live gate remains open. Magicman's remaining movie scopes
still need review.

## Magicman attacked movie early exits (offline v69)

The movie started at native `00E43965` is the `appuStack_314` object with
counted-pointer member `uStack_30c`. Its early-exit cleanup at
`00E43AFA`/`00E448F9` destroys that object; normal cleanup at `00E43BE3`
releases its member. The v68 Lua retained the started movie as `movie4`
but passed an older `movie` variable to `DestroyMovie` at six exits in
the beard and non-beard branches. The reviewed post-lift rule pins the
native/exported source hashes, instruction bytes, call operands, raw
object/member layout, two shared labels, and the exact draft scope.
It changes those six cleanup operands only. Seven focused evidence tests
and 20 Magicman tests pass; the latter cover termination in both beard
branches. The reviewed-helper suite passed 44 tests.

Full 29-unit scratch A/B changed only Magicman; registered v68 Bordello
draft and readable output matched the before snapshot. The smoke runner
reported an existing `BordelloLady:GetLHTSTag` error in both registered
v68 and v69 scratch; their JSON is byte identical. The older v68 saved
smoke JSON did not contain that error, so it was not used as a current
baseline. The 371-file readability audit has zero syntax failures.
Promoted only Magicman draft/readable and reports, then refreshed the
ledger. `local-candidate-v69` derives from v68 and changes only packaged
Magicman Lua. All 210 package Lua files parse; 216 manifest hashes match;
both DLLs are unchanged. No v69 install or game run has occurred.
Scratch evidence and validation: `work/codex_lua_marathon34_20260930`.
Book Collecting's live gate and Magicman's final beer movie scope remain
open.

## Magicman final beer movie cleanup (offline v70)

Native start `00E43D3B` and all five cleanup calls use `[esp+0x68]`
for the same movie object. Raw Ghidra C constructs it as `ppuStack_360`
and destroys it through the `aCStack_32c` alias. The v69 draft already
uses the started handle for some exits but uses another stack variable
for the overdrunk termination exit and normal cleanup. The reviewed
post-lift rule pins the native/source hashes, instruction bytes, stack
operands, raw cleanup count and labels, plus the exact draft scope.
It corrects those two `DestroyMovie` operands. Seven evidence tests,
22 Magicman behavior tests (including both normal and termination exits),
and 51 reviewed-helper tests pass.

Full 29-unit scratch A/B changed only Magicman with no registered v69
Bordello baseline drift. Current registered v69 and v70 scratch smoke
JSON are byte identical, including the existing BordelloLady mock error.
The 371-file readability audit has zero syntax failures. Promoted only
Magicman draft/readable and reports, then refreshed the ledger.
`local-candidate-v70` derives from v69 and changes only packaged Magicman
Lua. All 210 package Lua files parse, 216 manifest hashes match, and both
DLLs are unchanged. No v70 install or game run has occurred. Scratch
evidence and validation: `work/codex_lua_marathon35_20260930`.
Book Collecting's 22-donation live gate remains open.

## Magicman conversation lines (offline v71)

Two timer branches built the `TALKING_OUT_LOUD` text key and a conversation
but left native slot `+0x5b8` (`AddLineToConversation`) as TODOs. Native
calls `00E414DA` and `00E42DD1` pass the conversation ID, key, false
subtitle flag, Magicman (`ebx`, set to `this+8`), and hero. The Forge
Lua binding takes ID, key, speaker, listener, then the subtitle flag.
A reviewed post-lift rule pins both native calls and argument setup,
the native/source and binding-source hashes, and the exact draft
operands. It emits two Lua calls and removes their six unlifted
diagnostics. Both timer routes pass two successive dialogue-line
behavior checks with actor order asserted. Seven focused evidence
tests, 23 Magicman tests, and 58 reviewed-helper tests pass.

Full 29-unit scratch A/B changed only Magicman with no registered v70
Bordello baseline drift. Registered v70 and v71 scratch smoke JSON are
byte identical, including the existing BordelloLady mock error. The
371-file readability audit has zero syntax failures. Promoted only
Magicman draft/readable and reports, then refreshed the ledger.
`local-candidate-v71` derives from v70 and changes only packaged
Magicman Lua. All 210 package Lua files parse, 216 manifest hashes
match, and both DLLs are unchanged. No v71 install or game run has
occurred. Scratch evidence and validation:
`work/codex_lua_marathon36_20260930`. Book Collecting's live gate
remains open.

## Magicman first beer movie task loop (offline v72)

The last explicit Magicman native TODO was a tail loop at retail
`00E428C7`: when the overdrunk speech task ended, generated Lua returned
from `Main` instead of following `LAB_00e428d4` to award the tankard and
release the movie. Native instructions at `00E428B0`/`00E428C7` loop
while the task runs, and the termination path releases the movie member
at `00E4484C` and its base at `00E4485D`. The reviewed post-lift rule
pins the native/source hashes, branch instructions, cleanup calls, and
exact lifted loop. It inlines that loop, releases the first beer movie
on termination, and removes the obsolete tail plus its diagnostic.
Six evidence tests, 25 Magicman behavior tests covering speech completion
and termination, and 64 reviewed-helper tests pass.

Full 29-unit scratch A/B changed only Magicman with no registered v71
Bordello baseline drift. Registered v71 and v72 scratch smoke JSON are
byte identical, including the existing BordelloLady mock error. The
371-file readability audit has zero syntax failures. Promoted only
Magicman draft/readable and reports, then refreshed the ledger.
`local-candidate-v72` derives from v71 and changes only packaged
Magicman Lua. All 210 package Lua files parse, 216 manifest hashes
match, and both DLLs are unchanged. No v72 install or game run has
occurred. Scratch evidence and validation:
`work/codex_lua_marathon37_20260930`. Book Collecting's live gate
remains open.

## v72 frontend capture preflight

With no Fable process or other visible application active, desktop
capture was normal. The v72 local launcher loaded both DLLs from its
test bundle and started Fable (`pid 9440`); it did not install files in
the game directory. Once Fable was foreground, both the 1024x768 game
window capture and full desktop capture were entirely black. No profile,
save or quest was loaded. Fable was stopped at the frontend and desktop
capture returned to normal. The launcher's save backup recorded 561
files; the live save folder still has those same 561 paths and hashes,
with no additions. Evidence: the run under
`work/new-oakvale-original-fse-20260912/local-candidate-v72/runs/20260930-120439-745302`
and `work/codex_lua_marathon37_20260930/game_frontend_preflight.png`.
The Book Collecting silver-key receipt and reload gate remains open.

A second frontend-only v72 launch temporarily changed `SetFullscreen`
in the installed `userst.ini` to false. The game and desktop captures
were still black. Fable was stopped; the original `userst.ini` bytes
were restored and all 561 save paths/hashes again matched the launch
backup. This setting cannot affect retail graphics:
`docs/engine/CONSOLE_COMMAND_SYSTEM.md` already establishes that
`SetFullscreen` and `SetResolution` are absent from retail's command
registry. Do not retry those ini switches. Evidence is the second
launcher run under v72 and
`work/codex_lua_marathon37_20260930/game_windowed_preflight.png`.

## Bordello smoke parameter check

The extra `BordelloLady:GetLHTSTag` error in package-dir smoke runs was
caused by omitting `--conversion-report`: the generic harness supplied
a table for its `CCharString` suffix parameter. Using each candidate's
draft report gives that parameter a string. Typed v71/v72 Bordello smoke
JSON is byte identical and reports six baseline diagnostics, with no
`GetLHTSTag` error. Evidence:
`work/codex_lua_marathon37_20260930/smoke_typed_v71.json` and
`smoke_typed_v72.json`.

## Magicman pause arguments (offline v73)

Ten first-beer movie exits lifted `PauseAllNonScriptedEntities(0x126008c)`.
That number is a movie vtable address assigned to `esi` later in unrelated
cleanup code. The actual pause sites at `00E4461A`, `00E44842`, and
`00E44892` push `esi`; the paths reaching them zero `esi` at
`00E417D2`, `00E4195F`, or `00E4282D`. The only jumps to the shared
cleanup sites are `00E41869`/`00E419F2` -> `00E44615`,
`00E428C1` -> `00E4483D`, and `00E428DD` -> `00E4488D`.
The reviewed post-lift rule pins the body/source hashes, the branch and
call bytes, checks for intervening writes to `esi`, and requires exactly
ten lifted calls. It changes those ten arguments to `false` in Magicman.

Thirty-seven focused and Magicman tests plus 70 reviewed-helper tests
pass. Full 29-unit scratch A/B changes only Magicman; the v72 before
files match promoted v72. Draft and readable diffs are exactly ten
argument replacements. Typed smoke JSON matches byte for byte with six
existing diagnostics. The 371-file readability audit has zero syntax
failures. Promoted Magicman draft/readable and reports, refreshed the
ledger, and staged `local-candidate-v73` from v72. All 210 package Lua
files parse, 216 manifest hashes match, and both DLLs are unchanged.
No v73 in-game run has occurred. Evidence:
`work/codex_lua_marathon38_20260930`.

Next offline target: inspect the first `BordelloLady.Main` missing-operand
diagnostics (`SetCreatureBrain` and `SetWanderCentrePoint`) against
retail calls and the binding signatures. Keep the Book Collecting live
replay gated on a usable Fable display; the black frontend preflight did
not load a save.

## BordelloLady operand investigation started

`BordelloLady.Main` (`00E3EB10`) starts with five Lua calls whose
receiver is missing. Retail prologue `00E3EB1C` sets `esi = this+8`,
and call `00E3EB97` pushes `esi` immediately before virtual slot
`+0x934` (`SetCreatureBrain`). `00E3EBAE` obtains the home position
through the entity's slot `+0x1c`; `00E3EBB1`-`00E3EBD9` builds a
12-byte CScriptThing value from members `this+0xc/+0x10` and calls
quest slot `+0xbec` (`SetWanderCentrePoint`). The following calls
`00E3EC08` (`SetWanderMinDistance`) and `00E3EC3B`
(`SetWanderMaxDistance`) copy the same CScriptThing and pass zero and
16.0 respectively. Forge binding signatures require a thing argument
for all four methods and a vector table for the centre point. Other
registered entity scripts use `me` and `me:GetHomePos()` for this
pattern. No BordelloLady correction has been made yet; inspect slot
`+0xc40` and the remaining setup before a reviewed patch.

## BordelloLady opening setup (offline v74)

Native `00E3EB1C` establishes `esi = this+8`, passed to brain slot
`+0x934` at `00E3EB97`. The entity's `+0x1c` call at `00E3EBAE`
returns its home position into a 12-byte stack value. Quest slots
`+0xbec`, `+0xbf8`, `+0xc04`, and `+0xc40` each receive a copied
CScriptThing made from `this+0xc/+0x10`, matching the entity's `me`
handle. Their other operands are the home position, zero, 16.0, and
native group 4 respectively. The original Lua passed nil things,
discarded the home position before use, and passed group 0. The reviewed
post-lift rule pins the native/source body hashes, these call and setup
bytes, and the exact six draft fragments. It fixes five calls, keeps
the home position alive until the centre call, and removes five stale
missing-argument diagnostics. Retain-count TODOs outside this scope
remain visible.

Seven focused evidence/signature tests and 77 reviewed-helper tests
pass. A Lua mock executing the generated Main with a terminating thread
records all five calls in native order, with `me`, the actual home
position, distances zero and 16.0, and group 4; the v73 baseline records
nil things/position and group 0. Full 29-unit scratch A/B changes only
BordelloLady; both pre-change draft/readable Lua trees match promoted
v73. Typed Bordello smoke retains the same six baseline diagnostics; its
only row delta is an extra `arg1:GetHomePos` in BordelloLady.Main (419 to
420 calls). The 371-file readability audit has zero syntax failures.
Promoted BordelloLady draft/readable and reports, refreshed the ledger,
and staged `local-candidate-v74` from v73. All 210 package Lua files
parse, 216 manifest hashes match, and both DLLs are unchanged. No v74
in-game run has occurred. Evidence: `work/codex_lua_marathon39_20260930`.

Next operand: BordelloLady's `IsReportedOrUnreportedCrimeKnown` call at
retail `00E3ECC1` pushes `(this+0x14)->0x5c`; the registered quest
layout names its `0x5c` thing `BordelloVillage`, and the Forge binding
requires `spVillage`. The current Lua passes nil. Trace ownership and
restore that exact thing argument in the next reviewed pass.

## BordelloLady village crime operand (offline v75)

At retail `00E3ECB5` the entity reads its owning Bordello quest from
`this+0x14`, adds `0x5c` at `00E3ECBD`, and pushes that address at
`00E3ECC0` to quest slot `+0x220` at `00E3ECC1`. The registered quest
thing-field map identifies offset `0x5c` as `BordelloVillage`; Forge's
`IsReportedOrUnreportedCrimeKnown` binding requires `spVillage`.
The prior Lua passed nil. A reviewed post-lift rule pins the native and
hydrated-source hashes, the call bytes, the quest field mapping, and
the exact missing-operand diagnostic. It passes
`quest:GetStateThing("BordelloVillage")` and removes that diagnostic.

Seven focused evidence tests and 84 reviewed-helper tests pass. In a
Lua mock of the generated readable Main, the talked-to branch now
passes the exact village handle, while the v74 baseline passes nil.
When not talked to, both versions skip the crime query. The readable
pass folds the condition and renames locals; the branch mock checks
the short-circuit result. Full 29-unit scratch A/B changes only
BordelloLady; both pre-change Lua trees match promoted v74. Typed
Bordello smoke JSON is byte identical, with six baseline diagnostics.
The 371-file readability audit has zero syntax failures. Promoted
BordelloLady draft/readable and reports, refreshed the ledger, and
staged `local-candidate-v75` from v74. All 210 package Lua files
parse, 216 manifest hashes match, and both DLLs are unchanged. No v75
in-game run has occurred. Evidence: `work/codex_lua_marathon40_20260930`.

Next operand: `ClearThingHasInformation(nil)` in BordelloLady.Main.
Retail `00E400AE` sets `esi = this+8`, `00E400B1` pushes `esi`, and
`00E400B2` calls quest slot `+0x5a4`. The generated receiver is missing.
Check the binding and guarded branch before restoring `me`.

## BordelloLady information flag operand (offline v76)

The compiled nunnery transition sets `esi = this+8` at `00E400AE`,
pushes it at `00E400B1`, and calls quest slot `+0x5a4` at
`00E400B2`. Forge `ClearThingHasInformation` accepts the entity thing.
The decompiler duplicates this one native call into two Lua branches,
both previously passing nil. A reviewed rule pins the native/hydrated
source hashes, the call bytes and sole native slot site, and requires
exactly two lifted calls and diagnostics. It changes both to
`quest:ClearThingHasInformation(me)` and removes the two stale
missing-argument diagnostics.

Seven focused evidence/signature tests and 91 reviewed-helper tests
pass. A Lua mock of the generated nunnery branch confirms the old call
receives nil and the new call receives the entity handle. Full 29-unit
scratch A/B changes only BordelloLady; draft and readable diffs are
exactly the two replacements, with v75 baseline files matching the
registered tree. Typed Bordello smoke JSON is byte identical, with six
baseline diagnostics. The 371-file readability audit has zero syntax
failures. Promoted BordelloLady draft/readable and reports, refreshed
the ledger, and staged `local-candidate-v76` from v75. All 210 package
Lua files parse, 216 manifest hashes match, and both DLLs are unchanged.
No v76 in-game run has occurred. Evidence:
`work/codex_lua_marathon41_20260930`.

Next call on the same branch: `00E400BA` pushes the string address for
`OPINION_PERSONALITY_EX_PROSTITUTE`, `00E400D8` pushes `esi` (`me`),
and `00E400D9` calls slot `+0x9ac`. The generated
`EntitySetPersonalityOverride()` has no arguments and carries a
missing-binding TODO. Forge already exposes
`EntitySetPersonalityOverrideByString(spThing, personalityName)`.
Review the native string lifetime and binding before lowering this to
that existing method; no sidecar change is implied by the current evidence.

## BordelloLady personality override (offline v77)

After clearing her information flag, retail `00E400BA` passes
`OPINION_PERSONALITY_EX_PROSTITUTE` to a CCharString constructor at
`00E400C6`. The code pushes the string address at `00E400D7` and the
entity handle held in `esi` at `00E400D8`, calls quest slot `+0x9ac` at
`00E400D9`, then destroys the temporary string at `00E400E6`. `esi`
was set to `this+8` at `00E400AE` and is not rewritten along that
stretch. Forge's already available
`EntitySetPersonalityOverrideByString(spThing, personalityName)`
accepts the same thing and string order, and its C++ binding constructs
a FableString before invoking the native API. The generated Lua had two
zero-argument `EntitySetPersonalityOverride()` placeholders and two
false missing-binding diagnostics.

A reviewed post-lift rule pins the native/hydrated-source and binding
hashes, the literal address, call/cleanup instructions, sole slot site,
and both lifted copies. It emits
`quest:EntitySetPersonalityOverrideByString(me,
"OPINION_PERSONALITY_EX_PROSTITUTE")`, removes the two TODO comments
and diagnostics, and records the actual binding name in the call report.
Nine focused evidence tests and 100 reviewed-helper tests pass. A Lua
mock of the generated nunnery branch confirms the entity and exact
string are passed after the information clear. Full 29-unit scratch A/B
changes only BordelloLady; before draft/readable match promoted v76.
Typed Bordello smoke JSON is byte identical with six baseline
diagnostics. The 371-file readability audit has zero syntax failures.
Promoted BordelloLady draft/readable and reports, refreshed the ledger,
and staged `local-candidate-v77` from v76. All 210 package Lua files
parse, 216 manifest hashes match, and both DLLs are unchanged. No v77
in-game run has occurred. Evidence: `work/codex_lua_marathon42_20260930`.

Next operand: BordelloLady's one missing
`PauseAllNonScriptedEntities` argument after acquiring control.
Retail `00E3EDD5` jumps to `00E40392`, which pushes `edi` at
`00E40394` for slot `+0x5ec` at `00E40397`. The prologue zeros `edi`
at `00E3EB1F`, with no intervening write on that early path. The
separate loop exit `00E3EDB2 -> 00E40359` is already lifted as false.
The remaining Lua call passes nil. Recover `false` from the post-control
site in the next reviewed pass.

## BordelloLady post-control pause operand (offline v78)

The missing Lua pause operand belongs to the branch after control is
acquired. Retail `00E3EDD5` jumps to `00E40392`; that block pushes
`edi` at `00E40394` for virtual slot `+0x5ec` at `00E40397`.
`00E3EB1F` zeroes `edi`, and no instruction rewrites it before this
branch. The earlier loop-termination branch at `00E3EDB2 -> 00E40359`
also pushes zeroed `edi`, but its Lua call already passes false. A
reviewed post-lift rule pins the native/hydrated-source hashes,
post-control branch and call bytes, target site, register writes, and
the single missing-argument diagnostic. It changes only the remaining
call to `PauseAllNonScriptedEntities(false)`.

Eight focused evidence/signature tests and 108 reviewed-helper tests
pass. Executing the generated termination branch with a movie stub
shows the old call passes nil and the new call passes false; both retain
the two movie cleanup calls. Full 29-unit scratch A/B changes only
BordelloLady. The draft and readable diffs are exactly one operand
replacement, with v77 baseline files matching the registered tree.
Typed Bordello smoke JSON is byte identical with six baseline
diagnostics. The 371-file readability audit has zero syntax failures.
Promoted BordelloLady draft/readable and reports, refreshed the ledger,
and staged `local-candidate-v78` from v77. All 210 package Lua files
parse, 216 manifest hashes match, and both DLLs are unchanged. No v78
in-game run has occurred. Evidence: `work/codex_lua_marathon43_20260930`.

Next offline target: trace BordelloLady's `GetHealth(nil)` calls to the
native `+0x420` sites and distinguish self, hero, and resource things
before changing any receiver. The first visible call follows a
`GetScriptThing` temporary; its ownership must be checked in retail
bytes. Keep the remaining missing operands and TODOs explicit.

## BordelloLady health receivers (offline v79)

The eleven retail `GetHealth` slot `+0x420` calls at `00E3EE1B`,
`00E3EF5A`, `00E3F07E`, `00E3F289`, `00E3F526`, `00E3F653`,
`00E3F77B`, `00E3F877`, `00E3FA9E`, `00E3FE0B`, and `00E3FF0A`
each push the return value of native `GetScriptThing` at `007E7490`.
The call uses `[esp+0x1c]` after a push, the same stack entity
wrapper used by `ClearCommands` at `00E3EDDB`. The exported decompile
names this wrapper `stack0xfffffe0c`, also used for the entity's
`PlayAnimation` and other methods. Forge `GetHealth` requires one
`CScriptThing*` argument. The generated Lua previously passed nil at
all eleven sites; the reviewed generator rule now passes `me`.

The rule pins the retail body and exported-source hashes, all eleven
slot sites, the `GetScriptThing` sequence, the entity wrapper, and
all eleven lifted diagnostics. Seven focused tests and 115 reviewed-
helper tests pass. The generated Lua mock sees eleven nil receivers
before and eleven entity receivers after. Full 29-unit scratch A/B
changes only BordelloLady. Its draft removes exactly eleven
missing-argument diagnostics (Bordello unit total 94 to 83); the
readable pass folds some temporary assignments into conditions while
keeping one health call per site. The baseline draft/readable match
the registered v78 tree. Typed Bordello smoke JSON is byte identical
with six existing diagnostics. The 371-file readability audit has
zero syntax failures. Promoted the tested BordelloLady draft/readable
and reports, refreshed the ledger, and staged `local-candidate-v79`
from v78. All 210 package Lua files parse, 216 manifest hashes match,
and both DLLs are unchanged. No v79 in-game run has occurred. Evidence:
`work/codex_lua_marathon44_20260930`.

Next offline target: trace BordelloLady's three missing
`SetCutsceneSkippable` boolean operands to their native call sites.
Keep the remaining missing operands and TODOs explicit. The live
capture/save gate remains as described in the handoff.

## BordelloLady cutscene skip flags (offline v80)

The four retail `SetCutsceneSkippable` slot `+0x60c` sites occur in
exported-source order at `00E3F467`, `00E3FD6B`, `00E3FC58`, and
`00E3F4B9`. Their native pushes are zeroed `edi`, `1`, `0`, and `1`.
`edi` is zeroed at `00E3EB1F` with no intervening write before the
first site. The generated Lua had `nil, nil, true, nil`: three missing
arguments and an already generated true that disagreed with retail's
explicit `push 0`. The reviewed rule changes the sequence to
`false, true, false, true`. Sidecar `LuaManager.cpp` registers the
`LuaQuestState::SetCutsceneSkippable` binding; its implementation
forwards the boolean to the game API. The packaged DLLs were not
changed.

Seven focused tests and 122 reviewed-helper tests pass. A generated
Lua mock confirms the four-argument sequence before and after.
Full 29-unit scratch A/B changes only BordelloLady; both draft and
readable diffs are exactly four operand replacements, and registered
v79 files match the scratch baseline. The three missing-argument
diagnostics are removed (Bordello unit total 83 to 80). Typed Bordello
smoke JSON is byte identical with six existing diagnostics. The
371-file readability audit has zero syntax failures. Promoted tested
BordelloLady draft/readable and reports, refreshed the ledger, and
staged `local-candidate-v80` from v79. All 210 package Lua files parse,
216 manifest hashes match, and both DLLs are unchanged. No v80
in-game run has occurred. Evidence:
`work/codex_lua_marathon45_20260930`.

Next offline target: recover the integer argument for
`SetNumberOfTimesHeroHasHadSex(nil)` after the party sequence.
Do not infer the count from the method name; trace the adjacent
`GetNumberOfTimesHeroHasHadSex` retail call and its stack result.

## BordelloLady hero sex count (offline v81)

After the party sequence, retail `00E3FD81` calls quest slot `+0x368`
(`GetNumberOfTimesHeroHasHadSex`), increments the returned `eax` at
`00E3FD8A`, pushes it at `00E3FD8B`, then calls slot `+0x36c`
(`SetNumberOfTimesHeroHasHadSex`) at `00E3FD8C`. The generated draft
already retained the getter result in `r12`, but passed nil to the
setter. A reviewed rule pins the retail/hydrated-source hashes,
exact call and operand bytes, and the single diagnostic. It sets
`r12 + 1`; the readable pass folds this to
`SetNumberOfTimesHeroHasHadSex(GetNumberOfTimesHeroHasHadSex() + 1)`.
Both methods are registered in the sidecar and their Forge signatures
return/accept an integer.

Seven focused tests and 129 reviewed-helper tests pass. Generated
draft and readable Lua mocks each call the getter and setter once,
passing `0`, `1`, and `8` for prior counts `-1`, `0`, and `7`.
Full 29-unit scratch A/B changes only BordelloLady; draft/readable
diffs are one setter argument, and registered v80 files match the
scratch baseline. The missing-argument diagnostic is removed
(Bordello unit total 80 to 79). Typed Bordello smoke JSON is byte
identical with six existing diagnostics. The 371-file readability
audit has zero syntax failures. Promoted the tested BordelloLady
outputs and reports, refreshed the ledger, and staged
`local-candidate-v81` from v80. All 210 package Lua files parse, 216
manifest hashes match, and both DLLs are unchanged. No v81 in-game
run has occurred. Evidence: `work/codex_lua_marathon46_20260930`.

Next offline target: investigate BordelloLady's unresolved `Name`
string load and `HEDWIG` comparison in the party cutscene branch.
The current readable `if nil == nil` there needs native field and
string lifetime evidence before replacing it.

## BordelloLady Name field reload (offline v82)

`Init` obtains the entity data string and assigns it to the
BordelloLady CCharString at native offset `+0x28` (`00E3AC96` through
`00E3AC9F`). The registered entity field map calls this `Name`, and
the readable output already used it in the first paid cutscene branch.
For the later free cutscene branch, retail `00E3FB9F` loads the field
address into `esi`, and `00E3FD12` reloads its pointer on the target
path. The comparison literal at `012E3AE4` is `HEDWIG`. The generated
draft instead assigned unresolved nil to `CVar3`, which made the
readable branch `if nil == nil` and prevented the Hedwig variant.
A reviewed rule pins the Main/Init native and source hashes, offset,
assignment/read bytes, literal, field mapping, and one diagnostic.
It reads the existing `Name` state string at this site. The readable
pass consistently names that per-entity local `name_` across `Init`,
`Main`, and `GetLHTSTag`.

Six focused tests and 135 reviewed-helper tests pass. Generated draft
and readable Lua mocks select the Hedwig variant for `HEDWIG`, and
the ordinary variant for nil and `POLLY`. Full 29-unit scratch A/B
changes only BordelloLady; registered v81 draft/readable match the
scratch baseline. The Bordello unit TODO count falls from 79 to 78.
Typed Bordello smoke JSON is byte identical with six existing
diagnostics. The 371-file readability audit has zero syntax failures.
Promoted tested BordelloLady output and reports, refreshed the ledger,
and staged `local-candidate-v82` from v81. All 210 package Lua files
parse, 216 manifest hashes match, and both DLLs are unchanged. No v82
in-game run has occurred. Evidence:
`work/codex_lua_marathon47_20260930`.

Next offline target: inspect the `V_Bordello.PlayCutscene` smoke
unknown `SetCutsceneSkippableWhilePaused` against retail slot `+0x610`
and the packaged sidecar registration. Retain the smoke diagnostic
until the callable boundary is proved.

## Bordello paused cutscene skip binding (offline v83 and v84)

Retail `V_Bordello.PlayCutscene` calls slot `+0x610` at `00E3E9FA`
with a pushed `1`, and at `00E3EA54` with `ebp` zeroed on that path.
The lifted Lua already passed `true` then `false`, but the packaged
sidecar had no LuaQuestState registration. The owned source candidate
adds a bool forwarder to the existing GameInterface API pointer and
registers `SetCutsceneSkippableWhilePaused` with Lua. The Win32 Release
build succeeds; the isolated source commit is `ab268435b8e8491871c1c3c0ea8982fcbdb9c22e`
and the exported patch is
`tools/script_recovery/sidecar_patches/novi-zzzzzzzzzzzzzzzzzzzzz-cutscene-skip-paused.patch`.
The v83 sidecar SHA-256 is `44aa813957b4635a5f089a18d13b935b7b342ccd6917b36c440a430c348a43a2`.

A guarded generator rule pins the native body, exported source, six
instruction bytes, and source patch hash. It removes the two stale
binding TODO comments and two conversion diagnostics without changing
the Lua calls. Five focused and 140 reviewed-helper tests pass. Full
29-unit scratch A/B changes only `V_Bordello/V_Bordello.lua`; baseline
draft matches the registered v82 output. The typed Bordello smoke with
the candidate sidecar registration has five existing problems versus
six with the old registration, solely removing the unknown method.
The 371-file readability audit has zero syntax failures. Promoted the
tested draft/readable output and reports, then staged v84 from v83.
All 210 package Lua files parse, 216 manifest hashes match, and the
stock DLL is unchanged. v84 has no in-game proof. Evidence:
`work/codex_lua_marathon48_20260930` and
`work/codex_lua_marathon49_20260930`.

Next offline target: trace another unresolved Bordello call or state
operand from native evidence. A live replay still waits on a usable
display/capture path and the actual 22-book checkpoint.

## Madame pushability receiver (offline v85)

Madame Main calls `SetIsPushableByHero` through retail slot `+0xd30`
at `00E3BC28`. Before that call, `00E3BBFF` pushes false, and
`00E3BC11` through `00E3BC21` construct the by-value script thing
from Madame's own fields at `this+0xc` and `this+0x10`. The draft
had a missing first argument. A guarded rule pins the full native
body/exported source and the argument-building instruction bytes,
then restores `me` as the Lua receiver. It removes one missing-
argument diagnostic.

Five focused and 145 reviewed-helper tests pass. Generated draft and
readable Lua mocks capture Madame's own entity and false at the first
pushability call. The typed Bordello smoke remains byte identical
with five existing problems. Full 29-unit scratch A/B changes only
Madame; the registered v84 draft/readable match the baseline. The
371-file readability audit has zero syntax failures. Promoted the
tested output/reports and staged `local-candidate-v85` from v84.
All 210 package Lua files parse, 216 manifest hashes match, and both
DLLs are unchanged from v84. No v85 in-game run has occurred.
Evidence: `work/codex_lua_marathon50_20260930`.

Next offline target: investigate Madame's unresolved camera point
arguments, starting from the retail call sequence and LuaQuestState
ABI rather than guessing from generated parameters.

## Madame camera point calls (offline v86)

All six Madame Main calls through retail slot `+0x678` construct the
literal `CAM_B_MADAM`, push Madame's `this+8` script thing as target,
and push `-1.0, 0, -1`. The sidecar registers the string-and-target
`CameraUseCameraPoint` overload. The generated v85 Lua instead put
`me` in the name position, omitted the target at several sites, and
used unrelated values for the two flags. A guarded rule pins the
native body/exported source, literal bytes, six call sites, scalar
pushes, and typed slot. It restores the same five operands at each
site and removes six missing-argument diagnostics.

Four focused and 149 reviewed-helper tests pass. Full 29-unit scratch
A/B changes only Madame, with the registered v85 draft/readable
matching the baseline. Typed Bordello smoke is byte identical with
five existing problems. The 371-file readability audit has zero
syntax failures. Promoted the tested output/reports and staged
`local-candidate-v86` from v85. All 210 package Lua files parse,
216 manifest hashes match, and both DLLs are unchanged from v85.
No Madame-specific in-game proof is available. Evidence:
`work/codex_lua_marathon51_20260930`.

The earlier v72 frontend run is being rechecked because the same
capture script produced normal Fable menu frames on 2026-09-29 but
black game and desktop frames on 2026-09-30. That run cannot be used
as gameplay evidence.

## v86 Book Collecting live replay and frontend correction

The earlier black v72 capture was a failed test session, not a proven
game-display limitation. A new v86 frontend-only launch visibly reached
the title screen through both GDI and DXGI capture, loaded stock FSE
and the candidate sidecar, and left all 561 original live save hashes
unchanged. Fable has two opening movies. The runner's first attempt
timed out while classifying their frames; the frontend launcher now
sends Escape twice before screen recognition. Old scratch save backups
are retained rather than pruned when staging a replay.

With the corrected startup, v86 loaded the retained 22-book checkpoint.
The fresh-load guard verified 22 donated, 16 good, the good hat, zero
keys, all 40 BookDonated/BookOwned flags, and consumed-book inventory.
Normal teacher interactions donated the remaining books 23, 24, and
21 in that order. The runner autosaved stable checkpoints at 23 and
24, then observed `Books=25 Good=16 KeyRewarded=true KeyCount=1`
after the final scene. Its exact-one-key assertion, all book flags,
hat and consumed inventory assertions passed, then it autosaved and
harvested the result. The archived runtime log has no Lua runtime or
C++ exception markers. The result AutoSave SHA-256 is
`8bdd6b8a5346fe04b437f911f143621e7c17d483ef292fb2a4292b3d5a315bf0`.

A reload of that harvested save reached the game, but the completed
`V_BookCollecting` quest no longer had a live host, so its original
per-quest reload checklist could not attach. A new LUAGameflow
inventory/quest-status check was prepared. The user closed Fable for
another agent's debug-editor work before that check could load a save;
the disappearance was user-initiated, not a crash. The fresh-reload
key inventory check remains open. All temporary test profiles were
moved into `work/codex_lua_marathon52_20260930/save_profiles`; the
561 original live save paths and hashes match the pre-run snapshot.
Fable is closed. Evidence: `work/codex_lua_marathon52_20260930` and
`work/runner/v86_book_resume22_retry_20260930.jsonl`.

Read-only save decoding also checked the retained 22-, 23-, 24- and
25-book AutoSaves. Their QUESTS records contain the exact donation
counts, 16 good books, and all 40 entries of both book flag vectors.
`KeyRewarded` stays false through 24 and becomes true at 25. Decoding
the hero inventory finds zero silver keys in the first three saves
and one `OBJECT_SILVER_KEY` in the harvested result; the good wizard
hat is present in each. This proves the reward and quest flags were
written to the saved file. The in-engine inventory query after a
fresh load is still pending. Evidence:
`work/codex_lua_marathon52_20260930/saved_reward_audit.json`.

## BordelloClient hero sex count (offline v87)

Retail BordelloClient Main `0x00E44EA0` calls getter slot `+0x368`
at `00E45FA1`, increments `eax` at `00E45FAA`, pushes that value,
then calls setter slot `+0x36c` at `00E45FAC`. The generated Lua had
stored the unincremented getter value. A guarded generator rule pins
the native body, exported source, call slots, and operand instructions;
the readable Lua now stores the old value plus one.

Five focused and 154 reviewed-helper tests pass. Full 29-unit scratch
A/B changes only `V_Bordello/Entities/BordelloClient.lua`; the
registered v86 output matches its baseline. Sidecar aware typed smoke
is byte identical with five existing problems. The 371-file audit has
zero syntax failures. Promoted the tested draft/readable output and
reports, and staged `local-candidate-v87` from v86. All 210 package
Lua files parse, 216 manifest hashes match, and both DLLs are
unchanged. Fable stayed closed because another agent is using the
debug editor; this client branch has no in-game proof. Evidence:
`work/codex_lua_marathon53_20260930`.

## BordelloClient cleanup unpause (offline v88)

Four unresolved `BordelloClient.Main` cleanup calls through retail
slot `+0x5ec` occur at `00E465CB`, `00E465F9`, `00E46627`, and
`00E46655`. Each has an immediately preceding `push 0`; the slot is
`PauseAllNonScriptedEntities(bool)`. A guarded generator rule pins the
native body, exported source, call sites, and push instructions. The
draft now calls `PauseAllNonScriptedEntities(false)` on all four
paths and drops their diagnostics. The readable pass also folds a
redundant adjacent termination check; the sidecar query only reads
the current flag, and there is no frame advance between the checks.

Six focused and 160 reviewed-helper tests pass. Full 29-unit A/B
changes only BordelloClient, and the sidecar aware typed smoke trace
is byte identical with five existing problems. The 371-file audit has
zero syntax failures. Promoted the tested output and reports, then
staged `local-candidate-v88` from v87. All 210 package Lua files
parse, 216 manifest hashes match, and both DLLs are unchanged.
Fable remained closed while another agent uses the debug editor; no
v88 in-game proof exists. Evidence: `work/codex_lua_marathon54_20260930`.

## BordelloClient seh_Whore resource alias (offline v89)

Retail BordelloClient Main constructs and acquires a stack resource for
the client. Its `seh_Whore` assignment at `00E45FCD` copies that resource
to the parent quest member at offset `+0xa4`. The subsequent declined
dialogue checks the same resource through `GetScriptThing`. Exported
stack names caused the generator to label these two uses `xStack_120`
although only `xStack_118` was constructed; the old Lua left the
assignment as a TODO and passed an uninitialized handle to the health
query. A guarded rule pins the retail body, source export, native calls,
stack operands and member offset. It now uses the acquired resource at
both sites.

Nine focused and 169 reviewed-helper tests pass, including a Lua mock
that sees the same handle in `AssignResource` and `ScriptThing`. Full
29-unit scratch A/B changes only BordelloClient; sidecar aware typed
smoke is byte identical with five existing problems. The 371-file
readability audit has zero syntax failures. Promoted the tested output
and reports, then staged `local-candidate-v89` from v88. All 210
package Lua files parse, 216 manifest hashes match, and both DLLs
remain unchanged. Fable was in use by another agent, so no v89 live
test was run. Evidence: `work/codex_lua_marathon55_20260930`.

## BordelloClient byte flags (offline v90)

Retail BordelloClient Main stores byte 1 at `00E450C5` when the client
reaches its destination. The generated Lua had left that store as a
TODO, so its loop kept taking the pre-arrival path. At `00E453CB` and
`00E453D2`, retail stores 0 or 1 for the hero-hit response, then tests
the byte at `00E4541A`. The old Lua left both stores unresolved and
compared the unset value to numeric zero; in Lua, nil differs from
zero, so the hit response ran in the clear case. A guarded rule pins
the retail body, source export, stores, load, and branch. It emits
Lua booleans and tests the value as a boolean.

Six focused and 175 reviewed-helper tests pass. Full 29-unit A/B
changes only BordelloClient. The sidecar aware typed smoke still has
five existing problems and all other results match. In the no-hit
mock, BordelloClient Main makes 2,614 calls over 201 frames, versus
5,214 calls before the flag recovery; the initial trace, success,
and frame count match. The 371-file readability audit has zero
syntax failures. Promoted the tested output/reports and staged
`local-candidate-v90` from v89. All 210 package Lua files parse,
216 manifest hashes match, and both DLLs are unchanged. No v90
in-game proof exists while the shared debug editor is reserved for
another agent. Evidence: `work/codex_lua_marathon56_20260930`.

## BordelloClient controlled position (offline v91)

BordelloClient Main's two movement-position paths use the same native
stack pointer: `00E45074` and `00E45724` each load `[esp+0x2c]`,
test it for null, use static vector `0x0143E8E0` when null, and call
the pointed object's `+0x18` slot otherwise. The first path was
already lowered to the acquired client resource. The second was
misidentified as the reused dialogue string `xStack_e4`; its position
was unresolved and became nil. A guarded rule pins both native call
sites, pointer tests, fallback bytes, body and source export. It
uses `resources:ScriptThing(resource):GetPos()` or a zero vector on
the second path too.

Seven focused and 182 reviewed-helper tests pass, including Lua
position and null fallback cases. Full 29-unit A/B changes only
BordelloClient; sidecar aware typed smoke is byte identical with
five existing problems. The 371-file audit has zero syntax failures.
Promoted the tested output/reports and staged `local-candidate-v91`
from v90. All 210 package Lua files parse, 216 manifest hashes
match, and both DLLs are unchanged. No v91 in-game proof exists
while the shared debug editor is reserved for another agent.
Evidence: `work/codex_lua_marathon57_20260930`.

## BordelloClient indexed release (offline v92)

Retail `BordelloClient.OnPredicateFail` at `00E3B230` decrements the
parent quest's `ClientsAlive` field at `+0x54`, then stores byte zero
at parent `+0x58+ClientID`, with `ClientID` from the entity's `+0x20`
field. The unit layout identifies those three bytes as the persisted
`ClientInUse[0..2]` bools. The old Lua decremented the count but left
the indexed flag clear as a TODO. A guarded rule pins the complete
native body, exported source, fields and array layout. The readable
Lua now clears `ClientInUse_<id>`.

The first scratch candidate failed the standalone callback smoke:
the readable `clientID` upvalue was nil before Init. That candidate
was not promoted. The corrected rule uses zero when entity state has
no ID; initialized IDs retain their values. Nine focused and
191 reviewed-helper tests pass, including IDs 0, 1, 2 and the nil
default. Full 29-unit A/B changes only BordelloClient. Sidecar aware
typed smoke still has five existing problems; its only changed trace
is the intended `quest:SetStateBool` call in OnPredicateFail. The
371-file audit has zero syntax failures. Promoted the corrected
output/reports and staged `local-candidate-v92` from v91. All 210
package Lua files parse, 216 manifest hashes match, and both DLLs
remain unchanged. No v92 in-game proof exists while the shared
debug editor is reserved for another agent. Evidence:
`work/codex_lua_marathon58_20260930`.

## Madame hit-response flags and fresh silver-key reload (v93)

Madame.Main has two hit-response byte flags. Retail stores zero or one
to `[esp+0x13]` at `00E3CB35`/`00E3CB3C` and `00E3E063`/`00E3E06A`,
then tests the byte at `00E3CB96` and `00E3E0C7`. The generated Lua
left those stores as TODOs and treated nil as nonzero, taking the hit
branch in the no-hit path. A reviewed rule pins the native body and
source hashes, store instructions, and branch targets; it restores
false/true stores and boolean tests in those two branches. An earlier
unrelated byte TODO remains explicit. Eight focused and 199 reviewed
helper tests pass. The full 29-unit A/B changes only Madame. Sidecar
aware typed smoke keeps the five existing problems; Madame.Main's
no-hit mock changes from 202 frames/2434 calls to 201 frames/1825
calls, with its first 12 calls and all other results unchanged. The
371-file readability audit has zero syntax failures. The reviewed
draft and readable output were promoted, and local-candidate-v93 was
staged from v92. All 210 package Lua files parse, all 216 manifest
hashes match, and both DLLs are unchanged. v93 itself has no in-game
proof. Evidence: `work/codex_lua_marathon59_20260930`.

The completed Book Collecting save was also reloaded in Fable with
the v92 bundle. Through the always-live LUAGameflow host, the game
reported one silver key, the good wizard hat, and no copies of any
of the 25 donated books in inventory. Book Collecting was complete
and inactive. The runner finished successfully; its game log had no
Lua runtime error or C++ exception markers. Fable closed and all 561
original save files matched their pre-run hashes. The temporary
profile was retained in the scratch evidence directory. Runtime log:
`work/runner/v92_book_key_fresh_reload_20260930.jsonl`.

## Madame presented beer item (offline v94)

Retail Madame.Main polls presented items at `00E3BEDC` through entity
vtable `+0x8c`. With a nonnull item name it calls
`CBasicString<char>::Compare` at `00E3BF13` against
`OBJECT_BEER_TANKARD`, stores the equality byte at `00E3BF1E`, and
branches to the beer response at `00E3BF28` only on equality. The
generated Lua read an unrelated uninitialized stack value and left
the equality flag as a TODO. The reviewed rule pins the native body,
exported decompile, call sites, comparison operand and sidecar item
name bridge. It clears the bridge's global before each poll, then
uses the returned item name to set the equality flag.

The first direct rewrite was kept in scratch but not promoted: typed
smoke caught `_G` unavailable in this quest VM, and after fixing that,
full readable regeneration rewrote unrelated files. The accepted
source-faithful draft retains the comparison's temporary values and
produces a 16-line Madame readable diff. Seven focused and 206
reviewed-helper tests pass, including stale item-name and wrong-item
cases. Full 29-unit A/B changes only Madame. The selected readable
stage carries that generated Madame file over the registered v93
baseline; its 10 Lua files parse. Sidecar aware typed smoke keeps five
existing problems, has an identical no-item Main result, and changes
only Madame's expected `g_PresentedItemName` free-global listing.
The 371-file readability audit has zero syntax failures. Promoted the
reviewed draft/readable outputs and reports, and staged
`local-candidate-v94` from v93. All 210 package Lua files parse,
all 216 manifest hashes match, and both DLLs are unchanged. The
presented-item path has focused Lua mock proof; no v94 in-game
presentation proof exists. Evidence: `work/codex_lua_marathon60_20260930`.

## Madame health-gated beer speech (offline v95)

In Madame.Main's presented-tankard response, retail calls the health
slot `+0x420` at `00E3BFC1`, compares the result with the zero float
at `0122DEDC`, stores one to `[esp+0x13]` at `00E3BFCD` and clears it
at `00E3BFD9` unless health is strictly positive. The byte test at
`00E3BFEA` skips `Speak` when false, rejoining at `00E3C054` before
the tankard return. The v94 Lua left the clear as a TODO and treated
the speech block as unconditional. A reviewed rule now restores the
reused byte flag and guards speech on positive health, including the
unordered/NaN case implied by native `test ah,0x41`.

Six focused and 212 reviewed-helper tests pass. A tankard-presented
Lua probe on the generated readable files confirms that at zero
health the candidate omits exactly `Speak`, its task poll, and one
termination check, while both versions return one tankard. At
positive health their full call traces match. The ordinary typed
smoke remains byte-identical with five existing problems. Full
29-unit A/B changes only Madame. The selected readable stage uses
the generated Madame file over the registered v94 baseline; all 10
files parse. The 371-file audit has zero syntax failures. Promoted
the reviewed output and staged `local-candidate-v95` from v94.
All 210 package Lua files parse, 216 manifest hashes match, and
both DLLs are unchanged. No v95 in-game presented-tankard proof
exists. Evidence: `work/codex_lua_marathon61_20260930`.

## Madame controlled resource alias (offline v96)

Retail Madame.Main constructs its local counted resource at
`00E3BC4D` from `[esp+0x1c]`. Every one of its 22
`GetScriptThing` calls targets `007E7490` with receiver
`[esp+0x20]`, the controlled subobject four bytes into that
resource; stack-depth-adjusted receiver offsets all resolve to
the same slot. The native resource copy to parent `seh_Madam`
is at `00E3BCD8`. The generated Lua instead passed six different
temporary names to `resources:ScriptThing`, some unset and others
overwritten with unrelated values. Two release calls also read
an overwritten temporary.

A reviewed rule pins the complete native body/source, constructor,
parent copy, all 22 getter sites, and each generated receiver.
It retains the original initial stack assignment and captures the
new resource in a stable Lua local for acquisition, parent copy,
all getters, and local release. The first scratch candidate replaced
the initial stack assignment; inspection showed it changed an
early movie value to an unset temporary. That candidate's A/B run
was stopped and its outputs retained as rejected evidence.

Six focused and 218 reviewed-helper tests pass. A stricter
presented-tankard Lua probe rejects the old nil ScriptThing receiver
at both tested health values; the corrected Lua passes the actual
constructed handle, returns one tankard, speaks only at positive
health, and completes without an error. Ordinary sidecar aware
typed smoke is byte-identical with five existing problems. Full
29-unit A/B changes only Madame. The selected readable stage uses
the generated Madame file over the v95 baseline and parses all
10 files. The 371-file readability audit has zero syntax failures.
Promoted the reviewed output and staged `local-candidate-v96` from
v95: 210 package Lua files parse, 216 hashes match, and both DLLs
are unchanged. No v96 in-game Madame proof exists. Evidence:
`work/codex_lua_marathon62_20260930`.

## BordelloLady controlled resource copies (offline v97)

Retail BordelloLady.Main constructs its local resource at `00E3EB67`
from `[esp+0x18]`. Its eleven `GetScriptThing` calls target
`007E7490`; depth-adjusted receivers all resolve to the controlled
subobject at frame offset 28. At `00E3F369` and `00E3FB8A`, native
copies the local resource into the parent `seh_Whore` member at
`+0xa4`. The generated Lua discarded the constructed handle before
these paths, used unstable stack names for three lifted
`GetScriptThing` calls, and left both parent copies as diagnostics.

The reviewed rule pins the retail body/source, constructor and copy
sites, member offset, all eleven getter sites, and exact lifted uses.
It captures the constructed handle in a stable local, uses it for
the three generated getters and both parent copies, and leaves the
other resource calls for separate review. Seven focused and 225
reviewed-helper tests pass. Sidecar aware typed smoke is identical
before and after with five existing problems. Full 29-unit draft A/B
changes only BordelloLady; readable regeneration also changes only
that file. The 371-file readability audit has zero syntax failures.
Promoted the draft, readable output, and reports; staged
`local-candidate-v97` from v96. All 210 package Lua files parse,
216 manifest hashes match, and both DLLs are unchanged. There is
no v97 in-game BordelloLady proof. No original save profile was
already in the Bordello region, so no direct replay was available;
the save-region scan was read-only. Evidence:
`work/codex_lua_marathon63_20260930`.

Next native gap: BordelloLady.Main's `00E3ED9A` and `00E3EDC5`
slot-`+0x20` acquisition calls pass the same local resource
subobject at frame offset 28. The current Lua still calls
`me:AcquireControl` and has several unset resource temporaries on
prepare/release paths. Review acquisition, task methods, and
resource cleanup as one lifetime before another promotion.

## BordelloLady controlled acquisition and cleanup (offline v98)

Native BordelloLady.Main calls game slot `+0x20` at `00E3ED9A`,
`00E3EDC5`, `00E40219`, and `00E40244`. Each pushes priority 4
and the same controlled local resource subobject. The generated
Lua used `me:AcquireControl`, which creates a separate entity
handle in the sidecar, including one false priority 8 fallback.
The sidecar's `resources:TryAcquire` registers its resource for
later `me:` task methods, so the reviewed rule binds all six
generated acquire expressions to the stable v97 `ladyResource`
at priority 4.

Native local prepare calls at `00E3ED88`, `00E40067`, `00E40207`,
and `00E402F4` use the same stack resource; its destructors are
at `00E3F744` and `00E40382`. Several generated prepare/release
operands were nil or stale. The first scratch rewrite mapped an
apparent `ReleaseResource` after the conversation to the local
resource. Further native review showed `00E4007E` is the movie
destructor at a different stack slot; that candidate was preserved
as rejected and its partial A/B stopped. The corrected rule sends
that cleanup to `DestroyMovie(movie)` and keeps local releases on
`ladyResource`.

Seven focused and 232 reviewed-helper tests pass. Strict generated
Lua probes reject v97's nil prepare operand and accept v98's
constructed handle on cower, conversation, interruption release,
and the movie cleanup/reacquisition path. Sidecar aware typed smoke
has five existing problems on both versions; its BordelloLady Main
call count differs because the mock handles acquisition differently,
with no new error or unknown binding. Full 29-unit A/B changes only
BordelloLady; draft and readable output for the other registered
units remain identical. The 371-file readability audit has zero
syntax failures. Promoted the reviewed output and staged
`local-candidate-v98` from v97. All 210 package Lua files parse,
216 manifest hashes match, and both DLLs are unchanged. No v98
in-game BordelloLady proof exists. Evidence:
`work/codex_lua_marathon64_20260930`.

## v98 live entry attempt and pause point

Three isolated launches used `work/codex_lua_marathon65_20260930/bordello_entry.json`
and the copied `adult_trader_escort_completed_v15_2026-09-25` AutoSave.
The two startup movies were skipped by the existing launcher. The first
launch loaded a save and initialized `V_Bordello`, but the frontend driver
continued navigating after gameplay began and timed out. The launcher now
recognizes a quest host during frontend navigation. The second launch loaded
the save and installed the command channel, but the test's anchored `maps`
pattern produced an empty `Runner.regionOf`; the pattern is corrected in the
scratch config. The third launch loaded the save, installed the runner, and
then repeatedly reported `IsInCutscene() or IsInMovieSequence()` while waiting
to travel. Its capture shows a fixed camera behind the Barrow Fields wall,
matching the user's observation. Fable's window disappeared during that wait;
the runner reported `Fable.exe has no main window`. No Bordello crossing or
BordelloLady runtime behavior was observed. See `work/runner/bordello_v98_entry_1*`
through `_3*` and `scratchpad/autopilot_clear.png` for logs and the scene.

Before the launches, a 561-file original-save hash inventory was recorded in
`work/codex_lua_marathon65_20260930/pre_save_hashes.json`. All 561 files
matched after each launch, including the final run; Fable is closed. The v98
package README had been corrected after manifest creation, so its manifest
hash was refreshed and all 216 entries were checked again. No Lua or DLL
payload changed.

Resume with a disposable copy of a checkpoint that is outside an active
scene, then retry the corrected Bordello entry config. Check the loaded
camera and `IsInCutscene()` before attempting the map-slot transition. Keep
the original-save hash comparison and label any direct map-slot entry as
assisted proof. The user called a stop for tonight; do not launch until they
resume the work.

The last run command, for reference, was:

```powershell
python tools/script_recovery/ingame_runner.py work/codex_lua_marathon65_20260930/bordello_entry.json --bundle v98 --tag bordello_v98_entry_3 --minutes 5 --launch --save adult_trader_escort_completed_v15_2026-09-25 --close
```

Replace the `--save` checkpoint and `--tag` after finding a save outside
the stuck scene. Recheck Fable and the debug editor are idle and record a
fresh save-hash baseline before any launch.
