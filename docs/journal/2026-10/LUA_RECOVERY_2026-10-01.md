# Lua recovery - 2026-10-01

Resumed on explicit user request to continue Lua work, with game launches
authorized. Active checkout `feat/novi-script-recovery`, base commit
`366c08e64a4323a6147fe4d5caa675e31799d51e`. Preserve the extensive existing
dirty work in this checkout. Scratch: `work/codex_lua_resume_20261001`.

## v98 assisted Bordello entry

Used a disposable staged copy of `adult_bandit_camp_completed_2026-09-26`,
whose earlier journal records a clean return to Bandit Camp Residential.
The runner observed two consecutive scene-ready checks before map-slot 351
travel. Run `bordello_v98_entry_20261001_1` reached Bordello and found five
BordelloLady entities. The log records all five Lua Main entries and no
Lua runtime error. This proves assisted entry and initial execution only:
the entry-only predicate finished while movie mode was active.

The launcher took time to back up saves. Captures were visible; its two
initial ESC inputs did not skip the remaining opening movie. An additional
ESC, after inspecting the capture, reached the title screen. No quest input
was used for that skip. The runner restored the staged profile after exit;
all 561 original save files matched the fresh prelaunch hash inventory, with
no added files.

Commands:

```powershell
python work/codex_lua_resume_20261001/save_inventory.py record
python tools/script_recovery/ingame_runner.py work/codex_lua_resume_20261001/bordello_entry.json --bundle v98 --tag bordello_v98_entry_20261001_1 --minutes 5 --launch --save adult_bandit_camp_completed_2026-09-26 --close
python work/codex_lua_resume_20261001/save_inventory.py compare
```

The follow-up `bordello_arrival.json` also requires both movie and cutscene
mode to end. Run `bordello_v98_arrival_20261001_2` is being observed. Initial
evidence: `active=true region=true ladies=5 cutscene=false movie=true`;
Madame starts movie resource 5 at readable line 76. The screenshot shows the
hero stationary outside the building. Repeated movement calls belong to
BordelloClient, not the hero. No completion or parity claim yet.

## BordelloLady early movie cleanup candidate

Native shared exit `00E3F737` loads `ecx=[esp+0x28]`, then calls the movie
destructor at `00E3F73B`. Its depth-adjusted receiver is frame+40, the same
movie as the normal `00E4007E` cleanup already reviewed for v98. Raw source
ties it to `ppuStack_1cc` and the movie start; a later temporary answer-string
constructor reuses Ghidra's printed stack name. Generated Lua incorrectly
passed `TEXT_OBJECT_HERO_ANSWER_YES` to this destructor.

Extended the existing hash-pinned resource-lifetime rule to validate the
early call, exact receiver instruction bytes, and lifted operand, then use
the acquired movie handle. Baseline files are preserved in scratch.
Ten focused tests pass, including a full Lua conversation interruption
fixture and rejection of altered receiver evidence or altered lifted text.
The broader reviewed-helper suite passes 235 tests (4723 deselected).
Full 29-unit A/B changes only BordelloLady, with no registered baseline
drift. Readable regeneration also changes only that file. Typed sidecar-aware
smoke is identical, including its five existing problems. Promoted the tested
draft/readable output and reports; the 371-file audit has zero syntax failures.

The second v98 run reached its five-minute timeout with movie=true throughout.
All 561 original files again match, and Fable is closed. Both captures and
archived logs remain in the scratch/run directories. Binding inspection shows
that `resources:StartMovie` and `quest:StartMovieSequence` both invoke the same
native acquisition. Madame executes them consecutively; the log enters the
second wrapper but never reaches its success message. The existing lowering
comment documents this same blocking pattern for ApprenticeSpeedTest.
Captured pre-suppression text shows return-address assignments between the
constructor and call in Madame, and a movie alias assignment in BordelloLady.
These evade the existing adjacency pattern. Next: extend that suppression
with focused adverse cases, regenerate/compare, then retry assisted arrival.

v99 packaging completed: 210 Lua files parse, 216 manifest hashes match,
stock SHA256 `36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea`,
sidecar SHA256 `44aa813957b4635a5f089a18d13b935b7b342ccd6917b36c440a430c348a43a2`.
The bundle copies only manifest payloads; v98 logs, command files and runtime
backups remain in v98. No v99 runtime proof claimed.

## Duplicate movie-start candidate

Scratch `work/codex_lua_movie_start_20261001` snapshots the previous lowering
module and captures Madame/BordelloLady's pre-suppression text. Madame's first
native call at `00E3BD4D` passes the object constructed at `00E3BD29` at
`[esp+0x30]`; BordelloLady's call at `00E3ED52` passes its constructed movie
at `[esp+0x28]`. Exact instruction dump: `native_movie_starts.txt`.
There is one acquisition at each native site, not two.

The candidate permits only a string constructor, preserved literal return-
address assignments, or aliases of the same movie between the synthetic
`RESOURCE_StartMovie` and operandless GSI call. It refuses intervening labels,
branches, unrelated calls, other-object aliases, or overwrites of the movie.
This removes eight duplicates from Madame and one from BordelloLady while
keeping Madame's standalone start. Focused and related tests: 287 passed,
4688 deselected. Full 29-unit A/B is in progress; not promoted yet.

Readable scratch parses all ten files and changes only Madame and BordelloLady.
`movie_probe.py` executes both generated entry paths: baseline attempts two
acquisitions and fails the non-reentrant mock; candidate acquires once and
reaches the following pause operation. Sidecar-aware typed smoke changes
only Madame.Main's call count (1825 -> 1824), preserving all other JSON fields
and all five existing diagnostics. The draft diff consists of nine removed
`quest:StartMovieSequence()` lines and nothing else.

Full A/B completed: only these two Bordello files changed across 29 units,
with no registered baseline drift. Promoted the reviewed outputs. v100
parses 210 package Lua files with 216 matching manifest hashes; both DLLs
are unchanged. Corpus audit: 371 files, zero syntax failures.

### v100 live arrival succeeds

```powershell
python -u tools/script_recovery/ingame_runner.py work/codex_lua_movie_start_20261001/bordello_arrival.json --bundle v100 --tag bordello_v100_arrival_20261001_3 --minutes 5 --launch --save adult_bandit_camp_completed_2026-09-26 --close
```

The same assisted map-slot entry now plays Madame's introduction. Hero walks
through the garden, the camera changes, and the native command log reaches
the closing intro dialogue (`TEXT_CS_B13_INTRO_100`), walking-speed resets,
Madame teleport, and `RemoveExtras FALSE,LIMBO`. Lua resumes non-scripted
entities and destroys movie resource 5. The runner observes scene=false and
its done predicate confirms at least five ladies plus both cutscene and movie
mode false. No Lua runtime error or C++ exception is logged. `intro_scene.png`
shows the garden walk. A later capture attempt occurred after the runner had
already closed Fable; it produced no image and is not a gameplay failure.
All 561 original save files again match, with no added files. This is assisted
arrival/introduction proof, not parity for other Bordello interactions.

## Branch-local boolean byte candidate

The live v98/v100 logs also show unsolicited cower animations. Native
BordelloLady writes `[esp+0x13]` to zero at `00E4017C` and one at `00E40183`,
then tests that byte at `00E401CD..00E401D3`. The zero path bypasses cowering;
the flag is set by a hit or damaging special ability, excluding healing.
The lifter kept mutable byte slices only when they had an entry initializer;
this branch-local flag instead collapsed to its last textual value.

Scratch `work/codex_lua_branch_bytes_20261001` preserves the baseline lifter
and exact native instruction dump. A minimal executable probe returned zero
for both branches before the fix and the selected value afterward. The new
rule admits only repeated literal 0/1 writes and direct zero tests; outgoing
argument uses and aliases remain excluded. Twelve focused tests pass. The
broader suite passes 346 tests and 28 subtests (4634 deselected). Full 29-unit
A/B rejected this generic candidate: the pause-operand evidence guard failed
because taking the byte out of the lifter's temporary pool shifted later
guessed operands. Captured source demonstrates changed pause booleans and
two unresolved calls. Retained `rejected_lifter.py`, `rejected_tests.py`,
`candidate_before_pause.lua`, and the failed A/B artifacts; reverted only
the candidate additions. Both active lifter and mutable-byte test file match
their pre-candidate text exactly. This candidate was never promoted.

## Reviewed hit-flag recovery

Scratch `work/codex_lua_lady_hit_20261001` extends the existing hash-pinned
BordelloLady resource-lifetime rule after lifting. Exact native hit, special-
ability, healing-exclusion, byte write/test and branch instructions are checked.
The rule initializes a local false before the message polls, sets it true at
the native hit label, and replaces only the `if 1 ~= 0` cower gate. The draft
diff is two assignments plus one condition; unrelated operands stay identical.

Fifteen focused tests and 240 reviewed/helper tests pass. Generated readable
probes cover no hit, melee, damaging magic and healing: the baseline cowers
in all four cases; the candidate cowers only for melee and damaging magic.
No-hit and healing paths run four frames and terminate without an error.
Full 29-unit A/B changes only BordelloLady, with no registered baseline
drift. Typed smoke keeps five existing problems; all fields are identical
except Main's call count (3411 -> 1211), reflecting the now-idle path.
Promoted the reviewed output; the 371-file audit has zero syntax failures.
v101 parses 210 Lua files, matches 216 manifest hashes and preserves both
DLLs. The new conditional retains more visible jumps in readable output;
that is not a reason to flatten the native behavior again.

Started `bordello_v101_decline_20261001_4` using the same isolated checkpoint,
with `bordello_decline.json`: finish introduction, talk to a lady, decline,
then require both scene modes to end. A final flag is test-runner bookkeeping
only; it is set after the scene assertions, not a game quest-progress flag.
Live run finished with a failed decline-step timeout. Introduction completed
normally in 87 seconds; the lady started her paid offer and played the declined
line, then a reminder and another question. Movie mode remained true at the
timeout. No cower commands were logged. Screenshot `decline_scene.png` exposes
an incorrect third button labeled `PARTY_PAID_QUESTION`. This run does not prove
conversation cleanup. All 561 original save files were restored unchanged.

## Lady question argument recovery (in progress)

Scratch `work/codex_lua_lady_question_20261001` snapshots v101 sources and adds
a separate hash-pinned question-argument rule. Native calls at E3F1D3 (free)
and E3F9CB (paid), GSI slot 0x1c8, receive the GetLHTSTag result, YES, NO, an
empty third label, and true. The lifter instead supplied the suffix as the
third label in both paths, and a stale temporary for free-path YES. Exact
argument setup windows and referenced string bytes are checked. Seven focused
tests pass, including execution of both full offer branches against baseline
and candidate. All 264 broader tests pass. Full 29-unit A/B changes only
BordelloLady; the registered draft matches the before output. Readable output
changes only that file and parses; paid/free behavioral probes pass on both
draft and readable. Typed smoke is byte-for-byte equivalent as parsed JSON,
including the five existing diagnostics. An initial readable build started
before Bordello conversion completed and was rejected as incomplete; rebuilt
from the completed conversion before validation or promotion. Promoted only
the tested Lady file and reports. Corpus audit: 371 files, zero syntax errors.

The next blocker is independently reproduced by `return_probe.py`: a single
talk event causes three offers and three cowers in four frames, with pause
history `[true, true, true]`. Native E40072/E40074 pushes false to GSI+0x5ec
before movie destruction. E4030A branches back to E3EC80, where the native
talk/crime condition is polled again. The generated copied tail instead jumps
past the initial polling loop and retains an unconditional cower gate. Native
disassembly is saved in `inspect_return.txt`. Question recovery alone does not
fix this return path; no runtime success is claimed for it.

v102 package validates 210 Lua files and 216 manifest hashes, with unchanged
stock and sidecar DLLs. Not launched because the independently evidenced
return-path defect would still prevent the decline acceptance gate.

## Lady conversation return path (in progress)

Scratch `work/codex_lua_lady_return_20261001` snapshots v102 and introduces
`native_reviewed_bordello_lady_return.py`, with a separate unit evidence row.
The post-lift rule preserves operand inference and restores three lost native
operations: unpause before normal movie destruction; hit/healing gating in
the copied idle tail; and the backedge to talk/crime polling instead of the
conversation entry. The copied-tail termination edge still reaches resource
cleanup. Full body/source hashes and four native instruction windows guard it.

Thirteen focused tests pass: paid/free decline crossed with idle, melee,
damaging magic, and healing; a new talk event starts exactly one further
conversation; baseline reproduces three offers/cowers with only one talk poll;
stale evidence and changed backedge are rejected. All 277 broader tests pass.
Full 29-unit A/B changes only Lady, with registered draft baseline identical.
Readable output parses and changes only Lady; smoke JSON is identical to the
baseline (five existing free-global diagnostics). Twenty generated behavior
cases pass, with identical draft/readable traces. Promoted tested outputs and
reports. Corpus audit: 371 files, zero syntax failures. v103 package: 210 Lua
files parsed, 216 manifest hashes matched, both DLLs unchanged. Original saves
still match the 561-file baseline before launch.

Started `bordello_v103_decline_20261001_5` with the isolated adult Bandit Camp
checkpoint and the introduction/decline acceptance config in this scratch.
Live run passed: introduction 75 seconds, paid decline 62 seconds. Exactly one
question was shown, with an empty third button; the declined line played,
the normal unpause call executed, and both scene modes ended. The remaining
runner observation stayed outside movie mode, with no second offer. No logged
Lua/C++ error or exception lines. All 561 original save files restored unchanged.
`offer.png` captures the declined response (the question had already closed),
and `live_summary.json` records the assertions. This proves the assisted paid
decline route, not the unexercised paid/free acceptance paths.

Separately saved native payment-call evidence at E3FB76:
GSI slot 0x1f8 receives the negation of entity field +0x20 (GoldRequired), while
the draft currently passes 0.0. Payment is a subsequent recovery candidate;
no payment change is included in v103.

## Lady payment operand (in progress)

Scratch `work/codex_lua_lady_payment_20261001` snapshots v103. A separate
reviewed rule restores `GiveHeroGold(-GoldRequired)` after lifting, guarded by
body/source hashes, the exact load/negate/push/call bytes at E3FB6B..E3FB7C,
GSI slot 0x1f8, the entity field's Int classification at +0x20, and the lifted
affordability check. Ten focused tests pass: full Init/Main paths debit the
four initialized prices (50/100/200/1000), while decline and insufficient funds
never debit; changed evidence, field type and operands are rejected. The
accepted-path probe stops immediately after capturing the debit; it does not
claim subsequent cutscene correctness. All 287 broader selected tests pass.
Full 29-unit A/B changes only Lady; registered baseline matches. Twenty-four
generated payment cases pass with identical draft/readable results. Smoke
JSON is unchanged (five existing diagnostics). Promoted the one-line change
and reports; 371-file corpus syntax clean. v104 package: 210 Lua files parsed,
216 manifest hashes matched, both DLLs unchanged. Offline validation only;
the paid map-key defect is being recovered before a live acceptance replay.

Read-only evidence for the next paid-path problem is saved in
`native_cutscene_map.txt`: E3FB91 constructs the `$SEXTALK` key and E3FBC3
indexes the quest string map at +0xb4. The generated paid path instead uses
the earlier follow-me dialogue string as its key. The free path already has
the correct key. Keep this separate from payment operand validation.

## Lady paid cutscene map key (in progress)

Scratch `work/codex_lua_lady_cutscene_map_20261001` snapshots v104. A separate
reviewed rule restores only the paid map key to `$SEXTALK`, checking body/source
hashes, the full native setup window at E3FB8F..E3FBCF, literal bytes, map index
and string assignment calls. Thirteen focused tests pass, including eight
full Init/Main entry variants (paid/free, Polly/Hedwig, first/repeat), debit,
both map entries, and cutscene selection. Tests stop at the cutscene helper;
they do not claim playback behavior. An initial test assumed free first-time
visits used the long scene; native E3F4A0/E3F4FD instead always selects the
short scene on the free path. Corrected that expectation using the saved
`native_free_scene.txt` evidence. All 300 broader tests pass. Full 29-unit A/B
changes only Lady; registered baseline matches. Sixteen generated setup cases
pass with identical draft/readable results, including the 2000-gold Hedwig
price. Smoke JSON unchanged; 371 corpus files parse. Promoted and packaged as
v105: 210 Lua files parse, 216 manifest hashes match, both DLLs unchanged.

Started `bordello_v105_paid_20261001_6` using `bordello_paid.json`. The runner
records gold and global counter before talking, answers Yes, and requires a
50-gold debit, counter increment, both scene modes ended, then checks the
Bordello quest's HadSex flag. These baseline values are runner bookkeeping;
no quest progress or currency is injected. Original saves matched all 561
baseline hashes before launch. Run 6 was interrupted after visual inspection
showed Grope's conversation instead of the paid offer. No paid-question log
appeared and gold stayed 10120. A blind TAB plus generic movie acknowledgement
is insufficient in the crowded room. This run provides no paid-path proof.
Tool-level Ctrl-C killed the process group without the runner's finally block;
after confirming the game was closed, archived the log and restored the exact
`save_backup_1234234_20261001-065709` backup using the normal restore function.
All 561 original files then matched, with none added. Do not assume tool-level
interruption executes Python cleanup.

Added optional `talkTarget` to the runner: evaluate a host predicate immediately
before TAB and require explicit true, otherwise try another approach. Existing
label and conversation-start checks remain. Three regressions cover wrong actor
despite matching OCR, successful repositioning before the first TAB, and missing
target data with no blind fallback. All 25 runner tests pass. Prior runner/test
files are snapshotted in this scratch. `bordello_paid_targeted.json` requires the
hero's targeted thing to have `GetDataString() == 'POLLY'`, reporting other data
values when rejected. No Lua package changes for this runner improvement.

Started `bordello_v105_paid_targeted_20261001_7` with the guarded config and the
same isolated checkpoint. The guard refused every approach (NULL/empty/none
and one SOPHIA target), so no conversation was accepted. Normal cleanup restored
all 561 original files. Added `talkData` to deterministically select one actor
from a shared script group, and changed the replay predicate to native
`IsEqualTo` against the selected Polly handle. All 26 runner tests pass.
`bordello_v105_paid_identity_20261001_8` then exhausted the frontend attempt
budget before reaching Load Game; no gameplay test occurred. Normal cleanup
again restored all original hashes. Handle startup promptly after launch.
Read-only next-candidate native
evidence in `work/codex_lua_char_bytes_20261001` confirms helper E3E320 ends with
`cmp edi,4` at E3E698; Lua currently compares the count to a string `\\x04`.
The guarded retry remains on the immutable v105 package.

## Low-byte C character constants (in progress)

Scratch `work/codex_lua_char_bytes_20261001` snapshots the current lifter and
Bordello output. Native E3E698 is `cmp edi,4`, followed by `sete al`; the costume
predicate must compare its integer component count with 4. The general expr
lowerer previously handled only `\\0` and `\\x01`, leaving `\\x04` as a Lua
string. Added a token-aware low-byte literal normalizer: ordinary hexadecimal
character constants through 0x7f become numbers, while quoted strings, wide
character forms and high bytes requiring signedness evidence stay unchanged.
Sixteen focused tests pass, executing lifted comparisons and checking quoted
text preservation. All 583 selected broader tests and 140 subtests pass. The
Unicorn native tests emit Windows access-violation reports while handling native
execution: isolated baseline/candidate runs of `test_bully_literal_scopes.py`
both return zero, pass 3 tests/5 subtests, and emit the same six reports. Saved
full logs and return codes in `oracle_comparison.json`; not a new lifter failure.

Full 29-unit A/B changes only Bordello's quest file and helper file. All 132
generated costume cases pass across both files in draft/readable form: the 27
complete dress combinations are recognized, missing items rejected, dialogue
tags correct, and clothing query order unchanged. Baseline rejects every outfit.
Smoke differences are only removal of two x04 free-global diagnostics (5 -> 3);
all function results/traces are identical. Promoted and packaged as v106:
371 corpus files parse, 210 package Lua files parse, 216 manifest hashes match,
both DLLs unchanged. No live costume proof is claimed.

Started `bordello_v106_paid_identity_20261001_9` using the deterministic Polly
selector/native identity guard and the same isolated checkpoint. Cancelled a
Windows Firewall network-access prompt, then visually confirmed/skipped the
opening film promptly; frontend reached title with ample attempt budget left.
Live paid acceptance result pending.


## v106 paid replay and runner completion deadline

`bordello_v106_paid_identity_20261001_9` completed introduction in 75 seconds.
Native identity targeting eventually started Polly's paid offer; the question,
follow-me dialogue, paid cutscene and unpause are recorded in the archived log.
Screenshot `work/codex_lua_char_bytes_20261001/paid_scene.png` shows control
returned and gold 10070 (initial 10120). Tutorial overlays interrupted command
replies; a visible innkeeper trade prompt was declined with RMB. The paid scene
advanced through the runner's automatic answer handling, without a separate
manual Yes. No counter/HadSex completion proof was captured, so this is partial
live evidence, not an acceptance pass. Run failed on the unnecessary second
targeting attempt. Normal finally closed the game, archived the log and restored
`scratchpad/save_backup_1234234_20261001-072231`. Save inventory comparison:
all 561 files unchanged, no additions.

The step completion deadline previously included all blocking setup/targeting
time. A late successful TAB could immediately exhaust that deadline and retry
the interaction while it was completing. `ingame_runner.py` now begins the
completion window after setup/actions finish. Regression simulates a 200-second
targeting delay with an initially false completion predicate: baseline presses
TAB twice; fixed runner presses once and waits for completion. All 27 runner
tests pass. Baseline failure is the expected `AssertionError: 2 != 1`, captured
in `work/codex_lua_runner_deadline_20261001/baseline_regression.log`.

Read-only next Lua evidence: Guard E40C02 constructs
`TEXT_QST_B13_GUARD_ATTACKED`, E40C13 copies constructor EAX into EDX, and
E40C1C calls concat. Decompiled concat's free `a` loses that left operand.
Madame has a similar `TEXT_QST_B13_MADAME_ATTACKED` site. Native/decompile
snapshots in `work/codex_lua_bordello_prefix_20261001`; no Lua changes yet.


## Prefix recovery in progress; paid run 10 targeting failed

`bordello_v106_paid_deadline_20261001_10` passed introduction in 79 seconds,
then rejected all six approaches twice because native target identity was
false. No paid offer or debit; normal runner cleanup archived the log and
restored backup `save_backup_1234234_20261001-073543`. All 561 original save
files verified unchanged/no additions; Fable is closed. Live targeting is
deferred, rather than repeating the same approach again.

Unpromoted candidate `native_concat_literal_operands.py` and converter hook:
native Guard E40C00..E40C1C and Madame E3CD0A..E3CD26 prove the concat left
operand is the immediately preceding string-constructor result. Require exact
native instruction sequence, matching constructor call-site pairing, literal,
length -1 and a simple named stack temporary. Preserve that temporary in the
intermediate representation, so later structural thread recognition still works.

Rejected initial literal-substitution version is saved in
`work/codex_lua_bordello_prefix_20261001/rejected_literal_substitution.py`.
Its completed 29-unit A/B (`ab/comparison.json`) changed Bordello as intended,
plus BeardyBaldy and BookCollecting: the latter lost BookReaction spawn
recognition because the matcher expects a temporary operand. Never promoted.
Revised version uses the paired constructor destination; eight focused tests
and ten subtests pass, broader 367 tests/10 subtests pass (`tests.log`).
Unicorn diagnostics in `test_native_villager_talk_temporaries.py` reproduce
identically baseline/candidate: return 0, one test passes, 24 exception reports.
Revised full 29-unit A/B running from `ab2/compare.py`, exec 66535. Do not mutate
converter/module while that comparison is reading them. Next: verify comparison,
build readable output using scratch draft, run Main behavior probes and inspect
smoke JSON before any promotion. No game or other test process remains active.


## v107 dialogue prefixes promoted and packaged

Revised full 29-unit comparison completed. Only Bordello Guard/Madame and the
already unresolved BeardyBaldy worker setup differ; BookCollecting is unchanged.
BeardyBaldy delta is exactly `a` -> `"ParentClass."` in that worker's concat;
its output stays in scratch pending worker recovery. Promoted only Guard/Madame
draft/readable files and corresponding reports. No generated Lua hand edits.

Eight focused tests/10 subtests and 367 broader tests pass. Generated Main
probes: 64 cases across both entities and draft/readable, four suffix strings,
melee/magic/healing/idle. Baseline attack paths fail on nil global `a`; candidate
produces the expected attacked dialogue and original flags at the speech
boundary. Healing and idle remain silent. Smoke diagnostics 3 -> 2 (only the
two `g_PresentedItemName` file diagnostics remain); all function results/traces
identical. Corpus audit 371 Lua files, zero syntax failures.

`local-candidate-v107`: 210 Lua files parse, 216 manifest hashes match. Only
Guard/Madame and README/STAGING differ from v106; stock/sidecar DLLs unchanged.
Offline validation only; v107 has not been launched. Summary and native evidence
in `work/codex_lua_bordello_prefix_20261001`. No live game remains active.

Next existing-port recovery: BeardyBaldy Main 0xE50FD0 loses the WatchForAttack
worker carrying the entity's CScriptThing. Native E5100A allocates 0x48;
E5101E..E5103D copies/retains this+0xC/+0x10; E51086 stores worker E53D90;
E5108D stores parent; E51090 copies captured thing into +0x3C; E510B7 registers
with parent. Read-only snapshots and lowering inputs captured in
`work/codex_lua_beardy_worker_20261001`. No new worker implementation changes
yet. Both prefix A/B and lowering-inspection processes have finished.


## v108 BeardyBaldy watcher and aggressive-spell binding

Generic `native_entity_self_copies.py` preserves the proven retained entity-self
copy before vtable-only constructor folding can replace it with an empty thing.
Requires both native member offsets, CScriptThing vtable, adjacent stack Info
word, exact retain, known thing use, harmless flags and no remaining live Info
read. Lifter recognizes that preserved self alias as the worker capture.
Generated Main now starts WatchForAttack with `{args={me}}` instead of dropping
the native worker. Five focused tests/7 subtests pass; broader 335 tests and
31 subtests pass. Full 29-unit A/B changes only BeardyBaldy entity Main.

Restoring the watcher exposed an absent runtime binding. Native CScriptThing
slot 0xAC -> wrapper 0x004AAD60: null implementation returns false/ret4, otherwise
tail-dispatches through implementation slot 0xAC. Decorated symbol and header
agree on bool __thiscall(CScriptThing*, const CCharString*). Isolated sidecar
clone adds `MsgIsHitByAnyAggressiveSpecialAbilityFrom(name)` using that slot and
the existing FableString RAII conversion. Exact binding lambda compiled/tested
in x86: nine null/slot/name/result/lifetime cases pass. Win32 Release build passes.
Source commit `22305659ac60d2cac3e57c47d978478756e28b85` over
`ab268435b8e8491871c1c3c0ea8982fcbdb9c22e`. Patch exported from committed diff:
`tools/script_recovery/sidecar_patches/novi-zzzzzzzzzzzzzzzzzzzzzz-aggressive-spell-event.patch`;
clean apply-check against baseline passes. Main workspace remains uncommitted.

27 generated baseline/draft/readable cases verify one spawn with exact actor,
watcher melee/aggressive/idle behavior, null/dead actors and health threshold.
Standard smoke has zero problems both sides; random speech changes call counts.
With identical seed 417, sole behavior difference is one CreateThread call;
all other results match. Promoted tested entity draft/readable and reports;
371-file corpus parses. v108 package: 210 Lua parse, 216 hashes match, stock FSE
unchanged; sidecar hash
`538c47bbad2b58ba5713a4b697237286cc78828ab9cd8c5ae710c804d3a9c0fa`.
Only that DLL, entity Lua and README/STAGING change from v107.

Prepared live test `work/codex_lua_beardy_worker_20261001/beardy_attack.json`.
Retail WAD TNG places the actor at local (45.028076,103.007813,24.255814) in
map slot3 BowerstoneSlumsWarehouses, origin (3104,4000). Test uses isolated adult
BanditCamp save, map-slot travel and one real melee input. A separate read-only
observer records transient AttackedByHero into Runner.attackSeen so the actor's
normal reaction/reset cannot hide it. No quest state or hit event is injected.
Checks native binding idle result, attack observed, scene cleanup, then restores
the save. No v108 live proof yet. All offline processes finished.


## v108 live gates and next native random recovery

Run11 `beardy_v108_attack_20261001_11` loaded the isolated adult BanditCamp
save and reached BeardyBaldy. Actor readiness and the native aggressive-spell
binding's idle false result passed. Actual WatchForAttack registered as region
thread slot9 with one argument. No positive hit/reaction proof: the test observer
was defined in the command environment, so CreateThread could not find it;
one melee swing also missed the moving NPC. Do not count observer failure as
game-script failure. Game log archived under work/runner with the run tag.

Run12 `beardy_v108_attack_probe_20261001_12` uses supported per-frame Runner.probe,
normal movement/melee, and fightDrain=false; no quest state or hit injection.
It timed out in the frontend before loading any save: another Fable2 session
and its firewall overlay obstructed captures. No gameplay claim for run12.
Normal finally closed the owned Fable process and restored backup
scratchpad/save_backup_1234234_20261001-081801. Inventory comparison: all561
original save files unchanged, zero additions. Further foreground tests deferred
while the other session is active.

Next scratch: work/codex_lua_beardy_random_20261001, baseline HEAD
366c08e64a4323a6147fe4d5caa675e31799d51e. BeardyBaldy GetRandomSpeech E53CB0:
native E53CE2 calls BFEB16; E53CE7 CDQ, E53CE8 divisor10, E53CED IDIV;
EDX remainder becomes EDI, checked against last two indices and used to index
speech strings. Existing lowering turns rand into ENGINE_Rand before the native
random pass; the pass recognized neither this spelling nor an in-place remainder.
Candidate extends existing exact-target guard to ENGINE_Rand and permits
in-place modulo where divisor is independent of the draw. Source snapshots,
native bytes/disassembly and pre-pass input captured. Candidate unpromoted;
focused tests and full29-unit A/B running. No game remains owned by this lane.


## v109 BeardyBaldy native random stream

Promoted BeardyBaldy GetRandomSpeech's native CRT draw/modulo10 through the
existing RetailRandModulo binding. native_random.py now accepts the lowered
ENGINE_Rand spelling, still verifies every rand call's native BFEB16 target and
call bytes/count, and permits in-place remainder when the divisor does not
depend on the drawn temporary. Distinct-result folding still requires a dead
raw temporary. Comments/intervening uses/invalid literal divisors remain rejected.
Smoke harness now returns numeric0 for RetailRandModulo, matching binding shape.

Focused suite:4 passed/5 subtests. Generated baseline/draft/readable behavior:
138 cases cover all10 indices, rejection of either last index, yields, cancellation
at each native check, and exact remembered-state writes. Baseline uses a supplied
Lua random sequence; candidate uses the same sequence via the native API mock.
This proves surrounding selection semantics and binding selection, not live RNG.
Full smoke traces with both RNG mocks fixed0 have zero problems; only added
RetailRandModulo calls differ, all other calls/frames/outcomes identical.

Full29-unit A/B:25 unchanged; BeardyBaldy helper changed once, Arena CrowdChecker
once (mod3), BanditCamp CROWDBANDITS Main twice(mod3), TourGuide Main twice(mod5).
Disassembly confirms other sites F1CE4A, D0B85D/D0BF91, EE5F82/EE634F, each
BFEB16 followed by CDQ/IDIV with matching divisor. Those three generated units
remain scratch-only pending dedicated behavioral validation. Only BeardyBaldy
draft/readable and reports promoted;371-file corpus parses.

Broad selection `(random or entity_self or native_reviewed)` initially gave
297 passed/12 subtests,1 failure,1 error. Identical baseline/candidate reruns
excluding the new regression each give296 passed/10 subtests, the SAME
AffairWifeAnimationRandom selector witness failure and BookTrader readability
fixture setup error. Narrow two-file comparison reproduces the wife failure;
BookTrader passes there, indicating order-dependent test state. Logs retained.
These are not passing tests and have not been hidden or attributed to candidate.

v109 package validates210 Lua and216 hashes. Payload changes only Beardy entity
Lua, README, STAGING fromv108. Stock/sidecar DLLs unchanged; sidecar SHA
538c47bbad2b58ba5713a4b697237286cc78828ab9cd8c5ae710c804d3a9c0fa.
No v109 game launch, no unpromoted generator changes, no processes running.
Next: investigate those existing witness/fixture failures and validate the
remaining native-random output changes without competing for the game foreground.


## Readability profile isolation candidate (in progress)

BookTrader's suite-only fixture error reproduces with collection of all tests
even when only its one node runs. Root cause: importing build_readable_unit
mutates readable_lua.GENERATED globally. BookTrader's separate reviewed pipeline
then renames its condition temporary, violating its exact shape witness.
Candidate makes converter-generated spellings an explicit parameter across
renaming, role inference, local splitting/pruning, and literal inlining.
Shared module globals remain unchanged. Source snapshots in
work/codex_lua_fixture_audit_20261001/baseline.

First partial fix propagated only through renaming: A/B found local splitting
differences (e.g. Assassin1 gold temporary); rejected, never promoted. Interrupted
its comparison and broad test selection once the omitted dependency was found.
Those interrupted runs are not completed validation. Revised source also passes
the pattern to lua_local_versions/lua_literal_values. Focused66 tests/283 subtests
pass, and whole-suite collection with only BookTrader's formerly failing node
now passes (1 passed/5059 deselected). Revised full29-unit readable A/B running
under ab2; inputs are the immutable v109 native A/B after drafts. No Lua promoted.

Wife failure traced through fixture stages: text sequence first rejects the
installed text bank hash; later reply/health/animation hashes reject cascading
unrecovered input. Native bytes/source pins remain valid. Installed bank SHA
f1b76255aaf469a00cdb484cac5bcf68a65f5a370cc011d185985dd1def26f0c differs from
reviewed531c4514...33fc (already documented since2026-09-28). Read-only TextBank
inventory confirms exact same four type0 prefix keys10/20/30/40, key50 absent;
evidence text_bank_inventory.json. No bank/witness change made yet.

Separate scratch work/codex_lua_other_random_20261001 executes all32768 CRT
outputs at each of the five additional random/modulo sites (163840 evaluations),
one draw per evaluation, equal residue, and verifies entire draft differences
are only those replacements. Units Arena/BanditCamp/TourGuide remain unpromoted
pending readable/smoke checks.


## Accepted profile isolation, reviewed bank compatibility, v110 random follow-through

Revised readability profile fix: complete29-unit A/B in
codex_lua_fixture_audit_20261001/ab2 changes ZERO Lua files. Focused66 tests/
283 subtests pass. Builder now passes CONVERTER_GENERATED explicitly through
role inference, local versioning/pruning and literal inlining. Importing it no
longer mutates the independent BookTrader pipeline. No generated-output promotion
needed for this source-only isolation fix. First partial candidate remains rejected.

Accepted text-bank compatibility after exact prefix inventory review. Original
531c4514...33fc bank pin retained; compatibleTextBankSha256 adds only audited
f1b76255...f26f0c. Native/source pins and unknown-bank rejection remain enforced.
Five failing/five passing focused wife tests become ten passing, including
changed-bank and changed-native rejection. Scratch full legacy NewOakvale A/B:
only AffairWife.lua changes;51/51 functions and18/18 files parse both sides;
1215->1203 diagnostics. This legacy draft remains scratch; no runtime Lua or
installed asset overwritten. Final broader selection
`random or entity_self or native_reviewed or profile_isolation`:
301 passed,12 subtests,4759 deselected,zero failures/errors (40.45s).

Promoted remaining native-random outputs: Arena CrowdChecker mod3,
BanditCamp CROWDBANDITS Main two mod3 sites, TourGuideGuide Main two mod5 sites.
All163840 possible CRT-input/site combinations agree on residue and one draw.
Entire native-draft deltas contain only those five call/modulo replacements;
readable changes include consequent local renaming. Five controlled RNG values
per unit, before/after full smoke traces: zero differences except native RNG
calls, zero Arena/BanditCamp problems. TourGuide retains one unchanged diagnostic:
root WatchForNoFollowers calls unbound MsgIsRegionUnloaded; all smoke functions
execute without Lua errors. Dedicated delta check confirms exactly the expected
one file per unit in draft/readable. Corpus371 parses after promotion.

Packaging correction: Arena/TourGuide are not in the existing v109 bundle;
they were not implicitly added. First staging assertion caught that assumption
before producing a manifest; partial tree preserved under
codex_lua_other_random_20261001/rejected_staging_manifest_mapping.
Accepted v110 changes only BanditCamp crowd Lua plus README/STAGING fromv109.
210 Lua parse;216 hashes match. Both DLLs unchanged, sidecar SHA
538c47bbad2b58ba5713a4b697237286cc78828ab9cd8c5ae710c804d3a9c0fa.
No v109/v110 game launches. No owned game or test processes remain.

Next evidence: codex_lua_region_unload_20261001. TourGuide WatchForNoFollowers
EE6A40 calls interface slot54 atEE6ABC with HeroGuildComplexInside;
EE6AC3 savesAL inBL, EE6ACA testsBL, EE6ACC branches on true. Native interface
vtable1260F0C+54 ->893360; decorated signature and SDK agree bool thiscall with
one const CCharString reference. Generated Lua currently discards that result
because binding metadata is absent, then tests stale termination state.
Plan: generic sidecar binding plus manifest bool signature, native/behavioral
verification, full unrelated-unit comparison. No changes for this next target yet.


## v111 generic region-unload binding / TourGuide result recovery

Native GSI slot54 at vtable1260F0C resolves893360. Header/decorated signature:
bool thiscall(interface,const CCharString*). TourGuide watcher EE6ABC receives
HeroGuildComplexInside and explicitly saves/tests AL. Added generic sidecar
MsgIsRegionUnloaded(regionName), guarded null interface/API and RAII FableString;
converter signature overlay now preserves the boolean result/branch. Existing
manifest entries still take precedence; missing region remains diagnostic.

Three focused tests pass. Broader97 tests/35 subtests pass. Exact candidate lambda
compiled/run x86:8 cases cover null guards,receiver,3 strings,true/false,lifetime.
Win32 Release build succeeds. Owned sidecar commit906a5fc39bd82d5208b5ff88b47125925413adb5
over22305659ac60d2cac3e57c47d978478756e28b85; clean clone, actual committed patch
novi-zzzzzzzzzzzzzzzzzzzzzzz-region-unload-event.patch, SHA
05308f1506177d33185e6c00e0c13a18f4c0940efd3b0c71bf63c2bd6999b3d8.
Apply-check against baseline clone passes. New DLL SHA
7331252ec13a5babe4a56730eb6a7798fe953ddea2464db7fca589f1621ee54a.

30 generated watcher cases (baseline,draft,readable): both candidate forms pass
all10 scenarios; baseline fails4 actual unload-result scenarios. Covers immediate
and delayed unload,live/dead/unconscious/empty follower lists,tour finished and
initial cancellation. In particular unload exits before AllFollowersDead write.
TourGuide smoke1->0 problems; only removed unknown-method diagnostic, all calls,
frames,outcomes identical with default no-unload mock. Full29-unit native A/B
changes TourGuide root and one Beggar entity; only TourGuide promoted. Corpus371
parses. v111 package210 Lua/216 hashes valid, changes DLL/README/STAGING only;
TourGuide/Beggar remain outside existing bundle. Stock DLL unchanged. No launch.

Excluded Beggar delta needs separate receiver recovery, not a region-string fix.
E58F5E/E5A837/E5AD63 all call thing vtable54 with SCRIPT_NAME_HERO; the first/last
already lower to MsgIsHitByHero, but middle call is misclassified as GSI.
Native E5A824 loads vtable from EDI, E5A831 setsECX=EDI; this is the thing
receiver. Current generated branch incorrectly calls MsgIsRegionUnloaded.
Sidecar signature overlay exposes the stale assignment problem but does not
make that caller correct. No Beggar output promoted. Evidence beggar_slot54.txt
and beggar_function.json. Next investigate annotate_interface_slots streaming
receiver aliases/branch-local definitions. No owned process remains active.


## Beggar reply-wait receiver recovery (offline promotion)

Extended native_receiver_reaching's vtable-call matcher to accept typed-pointer
`*piVar1` alongside `*(int *)piVar1`. Existing native CFG proof establishes this+8
for the four reply-wait calls, including E5A837; the textual GSI assignment came
from unrelated branches. Generated calls now query entity talk, melee hit,
any special ability hit, and healing ability 14 instead of four quest methods.
No receiver is inferred without the existing native call-site proof.

Focused regression and broader104 tests/35 subtests pass. Extracted generated
poll-block probe:64 candidate cases pass across draft/readable,32 per form;
baseline fails16. Covers talk/melee/spell/healing/cancel combinations, call order,
BeggarHit writes and cancellation; surrounding resource cleanup is outside probe.
Default smoke identical:3 files/1 pre-existing problem in BeggarBully (free globals
int3p1/int3scratchValue16). Full29-unit native A/B changes only LookoutPointBeggar.
Promoted its draft/readable and reports; corpus371 parses. No game launch and no
new package: Beggar remains absent from the existing v111 bundle. Evidence/tests,
A/B and promotion record: work/codex_lua_beggar_receiver_20261001.
Next investigate BeggarBully cross-variable high-byte health flags from native
instructions; no high-byte candidate edits yet.


## BeggarBully cross-variable high-byte flags (offline promotion)

Native nine health sites use GetHealth(slot420), FCOMP zero, FNSTSW AX/test AH,41,
byte esp+13 set1/conditionally0, cleanup and byte reload/test. The cross-variable
flag pass missed multi-pointer casts and unparenthesized/cast-free low-byte masks.
Extended only those printshape matches, retaining its existing bounded-use guard.
Six Bully health flags now lower, three TODO assignments and two free globals
removed. Cleanup-mask dead-assignment TODO unaff_EBP=uVar12 disappears after
its spurious cross-variable low-byte consumers disappear; native cleanup remains.

12 focused tests pass; broader571 tests/34 subtests pass. Actual generated predicate
blocks:120 candidate cases pass across draft/readable (six sites, ten finite,
signed-zero and infinite values); baseline fails42 of60. Probes cover predicate
calculation/query count only, not whole quest/resource lifetimes. Smoke3 files,
1->0 problems: only removed int3p1/int3scratchValue16 diagnostics; all mock function
traces/outcomes unchanged. Smoke comparison helper initially KeyError on removed
freeGlobals key; corrected comparison of complete JSON verifies exact delta.
Full29-unit native A/B changes only BeggarBully. Promoted its draft/readable/reports;
corpus371 parses. No game launch/new package; Beggar absent from current v111.

NaN equivalence is explicitly not established. Native rejects unordered health;
decompiler's <= printshape does not. Separately reproduced readability _negate
changing not(a<=b) into a>b, which changes Lua results for NaN. Standalone36-case
probe has4 mismatches; larger regression169 cases has41 failures/128 passes on
baseline. Next fix readability negation generally, validate across29 units, then
recover native ordered health conditions. Scratch codex_lua_float_negation_20261001;
no production negation edits yet. Current high-byte evidence/tests/A/B/promoted.json
are under codex_lua_beggar_flags_20261001.


## In progress: readable relational negation / ordered health follow-up

readable_style._negate candidate keeps explicit not for relational comparisons;
equality flips remain. Regression baseline41 failed/128 passed; candidate173 tests
pass including four metamethod/order tests. Focused style+initial regressions185
pass/8 subtests. Broader363 tests/551 subtests pass; Unicorn emits pre-existing
Windows access-violation diagnostics while harness continues, exit0 with assertions
passing (tests.log retains full stderr). Beggar default smoke3files0problems identical.
29-unit readable A/B currently running against immutable latest native A/B drafts;
partial delta audit36 files all differ only in finite comparison complements.
No readability candidate output promoted or packaged yet. Shared style input frozen
until A/B finishes. Active scratch codex_lua_float_negation_20261001; prepared
promote.py/stage_v112.py are not run. Package plan defers older bundled files whose
baseline differs, to avoid unrelated updates. Both current DLLs remain v111.

Parallel read-only/scratch native inspection: all9 BeggarBully health comparisons
use FCOMP/FNSTSW/test AH,41 and byte esp+13, so unordered health is false. Exact
callOrder maps all9. Scratch C-source prototype gives198 generated predicate cases
passing including NaN, nine sites/draft/readable. Generic post-lift prototype verifies
uniform native patterns, threshold and complete Lua producer/consumer inventory;
1 positive/9 rejection probes pass. Not integrated. Scratch
codex_lua_ordered_health_20261001 (native_correspondence.json, ordered_candidate.py).
Separate pre-existing _negate equality-literal bug recorded: lookup("a == b")==nil
can modify the argument literal; follow up with token-position replacement after
current frozen A/B. No game launch or save changes this phase.


## v112 readable relational negation accepted

Full29-unit readable A/B complete:56 files across18 units change; token-aware
finite-complement audit explains every change, no unrelated content deltas.
Default smoke for every affected unit is exactly equal before/after, including
all pre-existing diagnostics; no new problems. Tests363/551 subtests pass, plus
173 focused float/metamethod cases (the latter includes4 added after broad run).

Promotion preflight correctly rejected GuildTraining before any copy: its registered
tree is separately reviewed native slices, unlike the generic scratch baseline
(38 different/absent paths). Preserved that lane; excluded its16 candidate files.
Promoted40 readable files across17 units and reports; no draft changes. Corpus371
parses. Generic generator change remains applicable on future generation.

v112 bundle210 Lua parse/216 hashes valid. Updated22 matching-baseline Lua files
plus README/STAGING. Eight promoted paths remain outside existing bundle; ten
bundled paths have older nonmatching baseline content and are deferred rather than
importing unrelated changes. Exact lists in v112/STAGING.json. Both DLLs unchanged:
sidecar7331252ec13a5babe4a56730eb6a7798fe953ddea2464db7fca589f1621ee54a.
No game launch/install/runtime claim. Scratch codex_lua_float_negation_20261001.
Next integrate/test generic native ordered-health flag recovery from prepared
codex_lua_ordered_health_20261001 scratch; separate literal-equality negation
follow-up remains documented. No owned process currently active.


## BeggarBully native ordered-health flags accepted (offline)

Added generic native_ordered_health_flags post-lift recovery after reviewed fixes.
Requires every native slot420 call and every emitted GetHealth producer/byte-flag
consumer to agree; all native calls must immediately match FCOMP dword zero,
set byte1,FNSTSW/test AH41,JE over clear byte0. Rewrites <=zero to not(>zero),
preserving unordered rejection. No quest-name/address dispatch or guessed operands.
Source ambiguity, missing actors, changed native gate/threshold or call inventory
leaves the entire source unchanged. Integrated convert_quest_unit via two-line hook.

18 focused tests pass (numeric/native-pattern/source rejection cases). Broader423
tests/644 subtests pass; known Unicorn Windows diagnostics remain in full log with
exit0/assertions passing. Full29-unit native A/B only BeggarBully.198 generated
predicate cases pass across nine sites/draft/readable including NaN. Default smoke
3files0problems identical. This proves predicate logic/query counts, not actor
resource lifetimes or whole-quest gameplay.

Initial direct UnitConverter scratch prototype produced unwrapped conversion report;
readable builder therefore omitted native address comments/quests.lua. Promotion
guard caught the metadata-only mismatch after validated draft copy, before any
readable copy. Rebuilt from standard CLI integrated_draft into integrated_readable,
reran198 cases and smoke, then resumed preserving original backups. Final promoted
change only BeggarBully draft/readable and reports; corpus371 parses. No new package
(Beggar absent from v112), no launch. Scratch codex_lua_ordered_health_20261001.

Next equality-negation token fix: current raw regex can alter a comparison inside
an argument literal or nested expression; unspaced equality can remain uninverted.
Scratch codex_lua_negation_tokens_20261001 has32 passing candidate cases, baseline
24 failures; not yet integrated. Other BeggarBully resource aliases and packed
third-byte flag remain explicitly unresolved.


## Equality negation token fix accepted; generated corpus unchanged

readable_style._negate now flips the top-level equality token by lexer position.
It preserves literal contents, nested comparisons and unspaced ==/~= forms.
Scratch32 behavior cases pass versus24 baseline failures; production regression
adds49 cases for values/argument traces/nested operator preservation. Focused
238 tests/8 subtests pass; broader416/551 subtests pass (full known-Unicorn stderr
retained, exit0). Full29-unit readable A/B has zero generated Lua changes, so no
output promotion, ledger regeneration or new package. v112 remains current.
Scratch codex_lua_negation_tokens_20261001; accepted.json pins tested source.

Next BeggarBully first-friend-reply local flag: byte ESP+42 initialized0 at E5B4E6
(after four pushes, printed+52), testedE5BD4F, set1E5BD7B(after one push,+46).
Ghidra merged it into resource-typed xStack_164/uStack_158. Scratch reviewed scalar
alias row pins whole body/source/instruction anchors and projects it to
friendReplySelected.48 generated first/repeat/cancellation steps pass; both draft
and readable baselines fail17/24. Scope includes dominating cancellation guard;
readable properly folds its duplicate inner check. Default smoke3files0problems
identical. Real row1 positive/3 changed-evidence rejection probes pass. Candidate
removes two TODOs; resource aliases and final counter remain unresolved. Production
unit JSON not yet changed. Scratch codex_lua_beggar_lifetimes_20261001.


## BeggarBully first-friend-reply flag accepted (offline)

Applied a reviewedStackScalarAliases row in existing V_BeggarAndChild unit JSON.
Whole native body/export hashes plus12 instruction anchors isolate the local byte
at balanced ESP+42. It startsfalse, gates the first friend reply, and becomestrue
after cancellation check but before health/speech. Resource-typed decompiler storage
is replaced by friendReplySelected; existing generic recovery code is unchanged.

Durable generated-code/evidence regression plus existing alias tests:17 pass
(11.93s; constructor-emulator Windows diagnostics appeared, process exit0).
Scratch48 generated first/repeat/cancel steps pass across draft/readable; old forms
each fail17/24. Probe includes dominating cancellation guard; readable removes the
redundant inner check. Health/speech/resource cleanup remain outside probe scope.
Default smoke3files0problems exactly equal. Unit-specific draft/readable A/B changes
only BeggarBully; other two unit files and registration unchanged. No shared-lowering
change, so this data-only row did not require another29-unit regeneration.
Promoted tested draft/readable/reports; corpus371 parses. Readable Bully TODO3->1;
resource lifetime aliases and missing quest-card operand remain unresolved even
where no TODO is emitted. No new bundle or game launch; v112 remains current.
Scratch codex_lua_beggar_lifetimes_20261001/promoted.json and focused_tests.json.

Next native evidence targets: GiveHeroQuestCardDirectly currently emits reversed
card/quest strings and uninitialized third operand (xStack_78). Manifest signature
(cardObjectName,questName,bool), native calls E5BC58 GetActiveQuestName(slotA3C),
E5BC6A card(slot49C). Need native push/hidden-return proof before changing operands.
Resource getters all target7E7490 but printed ECX stack aliases differ; native
physical receiver is stable. Initial GetThing(slot120,E5B4CE) records output slot
-340 yet restored source reuses resource-named xStack_164. Inspect alignment and
object lifetime aliasing, not blanket name substitution. Final counter increment
also remains unresolved. No owned game/process active at this checkpoint.


Quest-card candidate checkpoint (scratch codex_lua_beggar_card_20261001):
Native E59AEB/E59AFD and E5BC58/E5BC6A share the same nested string-return ABI.
Getter RET4 consumes its hidden output; earlier push0 belongs to card RET12.
Constructor storage and both quest-interface receivers checked. Generic guarded
native_quest_card_operands.py integrated before stack restoration. Seventeen
focused cases include both real functions, changed-native/source rejection and
true-boolean preservation. Generated draft/readable call-state probe:12/12 pass,
baseline0/12. Smoke3files0problems identical. Broader tests and29-unit A/B running;
no output promoted yet. No package/game action; v112 remains current.

Independent next-candidate native inspection: resource local is balancedESP1C;
all9 ScriptThing getters use ESP20 after one hidden-result push. Constructor
E5B407 targets99A380; reset E5CD9E/CDAB and destructor E5CE47 target same1C;
GetThing output is separate2C, destroyed E5CE3E. Scratch resource prototype holds
entityControl immediately after NewResource and rewrites only proven resource
uses, preserving the separate beggar lookup. Not integrated/promoted/validated.


Quest-card recovery accepted/promoted:581 tests and674 subtests pass in352.99s,
exit0 (known Unicorn Windows diagnostic stack dumps retained in tests.log).
Full29-unit native A/B changes only BeggarBully and LookoutPointBeggar. Both draft
and readable outputs plus reports promoted from standard candidate pipeline;
371 corpus files parse. No bundle change because Beggar is absent from v112.
No game launch/runtime claim. Resource lifetime remains the active scratch task.


Resource candidate checkpoint:generic native_reviewed_resource_lifetimes.py plus
reviewedResourceLifetimes row now integrated, pending promotion. Native/source
hashes, receiver stack identities and byte anchors cover constructor, two prepares,
two acquisitions, nine ScriptThing getters and one native cleanup. Separate2C
GetThing output/destructor stays separate. Eleven focused guard tests pass.
Actual generated Main:12 draft/readable startup/cancel/normal-reset cases pass,
registered baseline fails5/6. First combined mock hung because draft checks native
termination separately from NewScriptFrame; stopped only owned probe PID2828,
fixed consistent cancellation state, added Lua instruction limit, reran cleanly.
Getter operand probes18/18 pass vs0/9 baseline. Default smoke3files0problems equal.
Broader tests and29-unit A/B running. Candidate matches intended native lifecycle;
not yet promoted/package-installed/live-tested.

Next scratch codex_lua_beggar_counter_20261001: native counter is balancedESP58,
zero E5B4F0 with four pending pushes, load/increment/store E5CC0D/16/19, switch
load E5CC42. Separate initial-reply byteESP43 is zero E5B4EB, checked E5B6EA,
set E5B717 before movie construction. Decompiler conflates zero with packedflag
and later counter with movie. C-source scratch splits belchCount and initialReplySelected;
prototype draft built, readability building. No production changes for counter.


Resource lifetime accepted/promoted:636 tests and1593 subtests pass in240.02s,
exit0; known Unicorn Windows diagnostics retained. Full29-unit native comparison
changes only BeggarBully. Promoted standard draft/readable/reports,371 corpus
files parse. Prototype and candidate behaviors agree:12 lifecycle cases and18
getter operands pass; default smoke3files0problems unchanged. No new bundle or
live claim (Beggar absent from current v112). Remaining counter scratch prototype
now builds readable with zero Bully TODOs, but that metric is not behavior proof.


Counter candidate integrated: native_reviewed_stack_aliases accepts optional
additionalAliases within the same pinned evidence row. BeggarBully adds belchCount
and initialReplySelected with native byte/word initialization, branch, increment,
and switch anchors. Actual generated probes:8 four-step belch response cases and
16 initial-reply/taught-state cases pass. Smoke3files0problems exactly equal.
Initial focused guard test had a test-only if/else KeyError for duplicate aliases;
corrected to elif. Broader451 tests/644 subtests now pass in86.36s, exit0, known
Unicorn Windows diagnostic dumps retained. Full29-unit comparison nearly finished;
only BeggarBully changed so far. No output promotion yet at this checkpoint.

Next scratch codex_lua_lookout_resources_20261001: same retained-resource fault
in LookoutPointBeggar. Native localESP14 (entry-210),13 direct ScriptThing calls
useESP18 after one push. Source has14 ScriptThing uses because native CD23B9
resource validity check expands to not ScriptThing(...):IsNull before final reset.
Scratch extension validates this separately against following PrepareResource;
no production changes. Prototype building with reviewed per-site row.

Counter accepted/promoted: full29-unit A/B only BeggarBully;371 corpus files parse.
Readable Bully TODO1->0; other semantic gaps still require review, not parity claim.
No package/game action; v112 current. Scratch counter/promoted.json records outputs.


Lookout resource candidate integrated, pending validation completion:
generic reviewed lifetime helper now accepts explicitly reviewed prepareGuards.
It separates13 direct native getters from one not ScriptThing(...):IsNull guard
whose next resource operation must be its matching PrepareResource. Full native
receiver/source inventories remain checked;Bully row needs no guard.17 focused
cases pass including malformed guard/index/intervening-call rejection.
Generated lifecycle14cases and getter28cases pass; baseline6/7 lifecycle failures
and0/14 getter matches. Includes null-resource final-reset skip. Default smoke
3files0problems identical. Full29-unit comparison and broader tests running.
Only LookoutPointBeggar changed so far. No new package/game action.

Expression next scratch codex_lua_lookout_expression_20261001:
fold_name_compare recognizes *(void **)value but misses equivalent (void *)*value.
Native E5AEBE loads [EAX] into ESI, passes same ESI to411570 atE5AEC7 andE5B021
for BELCH/FART; results tested only for equality. Scratch regex alternate recovers
both comparisons. New spelling rejects intervening labels, source writes, pointer
escapes/arithmetic; six focused spelling/guard probes pass. Prototype draft built,
readable building; production native_evidence_lowering.py unchanged for this task.


Lookout resource accepted/promoted:642 tests/1593 subtests pass in273.81s,
exit0; known Unicorn diagnostics retained. Full29-unit native A/B only companion
LookoutPointBeggar. Standard draft/readable/reports promoted;371 corpus parses.
No package/game action, v112 remains current. Scratch lookout_resources/promoted.json.
Expression baseline focused regression:4fail/7pass; six scratch pattern probes
and14 actual generated branch-selection cases pass. Broader integration begins.


Expression candidate validation checkpoint:
Focused17pass; full29-unit native A/B finished, only LookoutPointBeggar changed.
Actual generated expression-selection14cases pass;smoke3files0problems identical.
Broader suite753pass/17fail with644subtests,272.81s. All17 failures reproduce on
saved pre-change native_evidence_lowering:17fail/40pass in the two affected files
(test_native_string_members and test_native_string_returns). Exact failure IDs
stored baseline_failure_comparison.json. Fixtures assumed readable Name upvalue
was named 'name'; current output uses 'name_' due naming collision. Fixed fixtures
resolve the member used by the generated GetLHTSTag helper while keeping golden
price/string and native-oracle assertions. Targeted57-test rerun in progress.
Original failing logs preserved; no expression output promoted yet.
An extra baseline string_matrices run passed64tests (wrong initial failure-location
inference from last fixture traceback, recorded, not substituted for real failures).

Next evidence-only first-reply candidate: scratch codex_lua_lookout_first_reply_20261001.
Native byte balancedESP3B zeroE58E76 with16pending bytes, readE5905D, setE5908E.
Existing scalar alias mechanism separates it from overlapping resource pointer.
Scratch unit only (production config unchanged for this flag); standard candidate
outputs built.384 generated cancellation/lesson/state steps pass vs81/192 failures
in each registered baseline stage. Five exact-row/stale-evidence guard probes pass.
Pending expression acceptance, durable regression and unit-specific promotion.


Additional pending lead (no changes): LookoutPointBeggar still has native rand
calls rendered as math.random. Printed C rand calls carry stale operands;
three mask0x80000003 conditions are not the existing modulo-pair matcher.
Native random helper currently folds reviewed pairs to RetailRandModulo; raw
ENGINE_Rand still maps to Lua RNG. Existing sidecar exposes RetailRandModulo,
so a proven CRT raw draw could use modulus32768 without a new binding, but this
needs exact call identity/range/source-side-effect evidence and separate A/B.
Do not infer RNG stream equivalence from matching output ranges.


Expression accepted/promoted: original753 suite passes plus57 targeted repaired
fixture passes cover770 unique tests,644subtests; exact17baseline failures verified.
No full-suite rerun claim. Full29-unit native A/B only LookoutPointBeggar;14 actual
branch-selection cases pass;default smoke equal.371 corpus parses. Source fixture
repairs are test-only upvalue discovery; golden expectations/native oracle retained.
No bundle/game action. Scratch expression/test_validation.json and promoted.json.

First-reply flag accepted/promoted via existing scalar row:74 durable tests pass,
384 scratch generated state/cancel/lesson steps pass,5 stale-evidence checks pass.
Unit-specific draft/readable comparison only LookoutPointBeggar;no shared-code
change requiring another29-unit run. Default smoke identical,371 corpus parses.
No new bundle/game;v112 remains current. Scratch first_reply/promoted.json.
Next investigate companion response counter conflated with beggarBully lookup;
raw native RNG stream recovery remains another documented lead.


Lookout response counter accepted/promoted: nativeESP40 zeroE58E7B, loadE5B045,
INC EBX E5B055, storeESP4C with12pending bytes E5B057, reloadE5B07D then two DEC
zero tests. Reviewed row isolates battleCryReplyCount from resource/lookup slots;
explicit mask preserves32-bit wrap.84 durable tests and40 generated sequence/wrap
cases pass;unit comparison only LookoutPointBeggar;smoke unchanged;371 corpus
parses. No shared-code change,new bundle or game. Scratch lookout_counter/promoted.json.
Next active investigation: native raw rand calls not matched by modulo-pair
lowering. Existing RetailRandModulo(32768) can preserve exact bounded CRT draw;
need raw-call evidence and full shared-lowering validation before integration.


### Raw native CRT draws: validated lowering, readable review in progress

Scratch: `work/codex_lua_raw_random_20261001`. Added conservative raw-rand fallback to `native_random.py` and the lifter, only when existing folds return unchanged source with no evidence. Exact E8 call inventories and BFEB16 import thunk are required; side-effecting stale arguments are rejected. Retail MSVCR71 rand export and DLL hash establish the 0..32767 range, so the existing sidecar RetailRandModulo(32768) retains the entire draw and native stream advance. No DLL change.

19 focused tests and broader 418 tests/33 subtests pass. Actual generated Beggar animation probes cover 131072 cases; taunt/cancellation probes cover 512 cases. Full 29-unit native A/B changes 32 files in 14 units. Thirty-one are RNG-provider replacements only; TraderToRescue also selects an equivalent short-circuit lowering and renumbers temporaries. Its actual before/after condition snippets pass 196608 branch/draw-count cases. Arena retains five Lua random sites in functions outside the conservative fallback; this is not a claim that all RNG sites are recovered.

All 14 paired draft mock smokes have identical JSON using seed417, equal draw streams and omission of RNG-provider log entries. Existing diagnostics are preserved in smoke_all/diagnostics.json, including ArenaEnemy's nil arithmetic error and existing free globals/pending methods. These smokes are not native runtime proof. Readable before/after builds are in progress. Registered draft preflight matches except BeardyBaldy's stale MsgIsHitByAnyAggressiveSpecialAbilityFrom TODO, whose binding already exists in the owned sidecar. GuildTraining's separately reviewed readable slices must be preserved. No raw-RNG generated outputs promoted or bundle staged yet; v112 remains current. No game launched.


### Raw CRT fallback promoted; offline candidate v113 staged

Readable generation completed for all 14 affected units. Every one of the 32 changed readable files compiles to identical stripped Lua 5.4 bytecode after replacing the native RNG provider with the old provider and removing source comments/whitespace (`compiled_equivalence.py`, `compiled_equivalence.json`). Thus local-name changes and the TraderToRescue short-circuit presentation introduce no additional compiled behavior. Both draft and readable paired mock smokes match for all 14 units. Readable diagnostics retain Arena InitialiseArenaRounds and pre-existing global references; all diagnostics are recorded separately from draft diagnostics.

Promoted 29 draft and 29 readable files across 13 units, plus reports. GuildTraining's three changed generic files remain scratch-only to preserve the separately reviewed registered slices. BeardyBaldy's registered baseline included a stale MsgIsHitByAnyAggressiveSpecialAbilityFrom TODO; regenerated readable watcher folds its now-uninterrupted condition. Explicit reconciliation passed 320 trace/exit cases per version (null, cancellation, direct hit, special ability and health including NaN), with actual binding confirmed at owned NoviUnitBindings.h:531. Full registered corpus: 371 Lua files, zero syntax failures.

Staged `work/new-oakvale-original-fse-20260912/local-candidate-v113` from v112: 14 Lua payload updates, 11 changed paths absent from the bundle, four older payloads deferred (TraderConflictGood BanditExtra, TC_BanditHostageKeeper, TraderToRescue; WaspBoss HornetDrone). No older payload was overwritten without matching reviewed baseline. Package checks: 210 Lua parsed, 216 manifest hashes match. Stock DLL remains 36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea; sidecar remains 7331252ec13a5babe4a56730eb6a7798fe953ddea2464db7fca589f1621ee54a. Offline only; no install/game action. This fallback deliberately excludes functions already partly folded or carrying rejected evidence, so unrecovered Lua RNG remains.

Promotion preflight initially stopped before any copy because the readable smoke helper wrote its aggregate to the draft results path. Corrected the helper and regenerated both aggregate summaries from the preserved per-unit JSON, all equal; no test result was relabeled or suppressed. The first provider-only text checker also over-expected Arena's excluded calls and missed equivalent Trader lowering; the final bytecode check establishes the readable result independently.

Next scratch: `work/codex_lua_lookout_conversation_20261001`. Native taunt calls push EDI whose sole CFG-reaching definition is E58EFE `lea edi,[ebp+8]`, proving the speaker is me. Cached BeggarBully lookup E58E5E writes balanced ESP24/entry-512, separate from resource ESP14/entry-528. `_align` incorrectly treats recorded edxStack as an extra vtable register argument even where native EDX merely feeds a push. Scratch ignoring that EDX field restores the cached actor but deliberately triggers existing resource inventory guard because its prepare-guard handle becomes the correctly distinct resource. No conversation production change yet; validate generic thiscall alignment and native me operands, re-review resource row before integration. Evidence and snapshots are retained in that scratch.


### Beggar conversation operands: recovery validated, final guard check pending

Active scratch `work/codex_lua_lookout_conversation_20261001`. Added native `me` operand recovery for entity vtable arguments: exact exported/source argument inventory, matching native call displacement, adjacent push sequence without incoming interior edges/calls/stack mutation, immutable callee-saved owner register, and all CFG-reaching definitions of the pushed callee-saved register must be owner+8. The companion's taunt initiator and four speakers are proven this way; listeners refer to the cached BeggarBully lookup at entry-512. Native E58E5E GetThingWithScriptName uses EDX only to feed a pushed hidden return pointer. Resource row spelling updates replace accidental cached r1 handles with xStack_210 (Lookout) and xStack_164 (Bully); every native byte/hash/lifetime witness remains unchanged and the checked restoration still emits entityControl.

The first broad alignment attempt ignored EDX for every vtable layout. It passed 817 tests/1007 subtests, but the full29-unit A/B exposed a GuildTraining CheckFriendlyAttacks vector alias regression (first fill xStack_94, later uses xStack_84). That attempt was rejected, not promoted, and all its trees/logs are preserved under rejected_broad_alignment. Madame was only a temporary rename; GuildTraining had three changed files. The final alignment is limited to the complete native GetThingWithScriptName hidden-result/name form (slot120, interface receiver4/40, two arguments/two pushes, EDX equals hidden result). Separate vector-lifetime recovery is required before widening it.

Second full29-unit A/B completed: only LookoutPointBeggar changes; every other registered unit, including Madame/GuildTraining, is identical. 33 focused tests pass; the broader rerun is still in progress. Candidate Beggar Lua and conversion reports exactly match the already tested readable inputs, so readable output was reused only after byte equality checks. Actual generated taunt probes pass all256 cases per stage (512 total), including cached participant identity, me speaker, all four replies, short-circuit draw counts and cancellation. Before versions fail all256 per stage on nil initiator. Both facing calls pass in both stages, baseline fails; native zero snap matches sidecar snap.value_or(false). Resource lifecycle/getter probes pass for both entities. Standard readable smoke:3 files/0 problems, JSON identical.

An initial scratch probe incorrectly declared the cached readable local inside a synthetic block ending before the hoisted third reply:238/256 passed. Corrected the probe scope to match actual function-scope declaration, not the generated output; the retained failure record explains it. An additional adverse form shows why explicitly printed source receivers must be rejected when an early unrelated push makes the operand counts coincide. guarded_me_operands.py adds that rejection using _receiver_printed; its Beggar outputs are byte-identical. A separate full29-unit guarded-output comparison is running before integrating that last guard. No conversation outputs promoted and no bundle/game action yet;v113 remains current.


### Beggar conversation operands promoted

The narrowed implementation passed818 tests/1007 subtests in333.36s. Integrated the final explicitly-printed-receiver rejection after its independent full29-unit comparison produced zero Lua deltas. Final focused tests including the two adverse receiver shapes and resource rows:52 pass. Full before/after comparison remains only LookoutPointBeggar. Promoted its draft/readable Lua and reports;371 registered Lua parse, zero failures. Restored taunt initiator/speaker me, cached BeggarBully participant/listener, and two facing directions. 512 actual generated taunt/cancel cases and four facing checks pass, prior outputs fail actor checks. Resource lifetime/getter probes and unchanged3-file/0-problem smoke retained. Native resource row witnesses unchanged; only accidental pre-restoration handle spellings were reconciled.

No package action because Beggar is absent from v113. No game launch/install. v113 remains current. Next lead: initial SetIsPushableByHero native by-value copy at E58E39 (and possibly companion Bully) is missing from Lua. Existing native_self_wrapper requires dead copied temporaries; this function reuses them. The unsimplified call contains `_scalar_deleting_destructor` and may hit generic ignored-line suppression. Investigate exact pipeline before changing it; six unrelated C++ temporary cleanup bit diagnostics remain explicit.


### Beggar initial pushability recovery: validation in progress

Scratch `work/codex_lua_beggar_pushable_20261001`. New `native_inline_self_argument.py` restores the native by-value self argument at Lookout E58E39 and Bully E5B4A9. Both construct CScriptThing's12-byte inline copy from entity+0xc/+0x10 with vtable01238c8c, conditional reference-count increment, and zero boolean from a proven callee-saved register. Exact15-instruction shape and internal CFG edges are checked; the owner must reach the copy solely from its prologue definition (Bully legitimately overwrites EBP later during cleanup). GSI table entry01260f0c+d30 must resolve to008a6dd0, whose0xda-byte body hash is8270d47d77b1cf84d679faa32498b92bfa2f6f13d05ce925b00c6a699e67fe9d and ends RET16. Existing host binding creates a retained CScriptThing copy. The source rewrite touches only the pure argument list, preserving reused temporaries for later passes.

Root cause: existing native_self_wrapper fold declines reused source temporaries, and the remaining GSI argument contains `_scalar_deleting_destructor`, matching generic NOISE suppression and dropping the entire operation. Native-backed argument recovery removes that accidental suppression without widening ignored-line handling or guessing operands.

37 focused tests pass, including modified fields/vtables/reference counts/null branches/interior entry/callee/metadata. Broader suite451 tests/35 subtests passes in28.96s. Actual generated lifecycle probes:26/26 candidate cases pass across both entities and stages, prior versions12/26 (all14 paths reaching actor lookup fail because pushability was omitted). Early cancellation/acquisition failure make zero calls; normal paths make exactly one call on me with false before lookup. Readable diffs are exactly one added `quest:SetIsPushableByHero(me,false)` per entity. Default smoke3 files/0 problems has one extra call per Main (Bully6982->6983, Lookout3400->3401), same frames/outcomes; paired logs omitting only this newly restored call match exactly. Full29-unit A/B is still running; completed units outside Beggar are unchanged. No outputs promoted/package/game action yet;v113 remains current.


### Beggar pushability promoted

Full29-unit A/B completed with only BeggarBully and LookoutPointBeggar changed. Promoted their draft/readable Lua and reports after all validation above. Corpus371 parses, zero syntax failures. Both scripts now execute the native initial unpushable setting after successful acquisition, with cancellation order preserved. No package/game action; Beggar is not in v113. No owned processes remain. Next recovery lead is the remaining native rand calls in functions partly handled by existing modulo folds; the v113 fallback deliberately left those alone. Preserve existing rejected-evidence guards and native draw counts while investigating a complete original-call inventory proof.


### Partial native RNG completion promoted; v114 staged

Scratch: `work/codex_lua_partial_random_20261001`. The new wrapper around
`fold_retail_random` completes raw draws only after generic modulo-pair folds,
with complete original source/native call inventory, exact import thunk and E8
targets, pure discarded arguments, pair/draw/API-count accounting. Reviewed
special-case and rejected evidence retain their prior gates.

45 focused tests/5 subtests and 438 broader tests/33 subtests pass. Full29-unit
A/B changes exactly Arena/Arena.lua (5 draws), BanditCamp CROWDBANDITS (14),
TourGuideGuide (1). Draft differences are provider substitutions only. Paired
draft and readable smoke JSON are identical using the same draw stream. Existing
Arena draft nil arithmetic in ArenaEnemy.OnPredicateFail, extraout_EAX, and
pending InitialiseArenaRounds remain baseline; readable retains only the pending
binding. All3 readable chunks compile identically after RNG provider normalization.
Generated conditional/mask/remainder probe covers all32768 draws with both branch
states:65536 cases per version, exact draw counts and timing. Scratch setup first
omitted finish_lua; an inline-call fixture also exposed unsupported nested GSI
expression emission, so the passing probe uses the registered source's separate
draw assignments. No claim of arbitrary inline-call support. Initial promotion
preflight normalized only candidate and stopped on existing native APIs; corrected
to normalize both sides before any copies.

Promoted3 draft and3 readable files plus reports;371 corpus Lua files parse.
v114 derives from v113 and updates only CROWDBANDITS, whose bundled baseline
matches exactly. Arena/TourGuide payloads are absent and excluded. 210 bundled
Lua files parse,216 manifest hashes match, both DLLs unchanged. Not installed or
launched; offline evidence only. Next investigate Lookout's three remaining C++
temporary cleanup-bit groups from native construction/destruction flow.


### Lookout temporary-string cleanup promoted; live retry deferred

Scratch `work/codex_lua_lookout_cleanup_20261001`. Extended the existing reviewed
temporary cleanup pass with exact multi-group source spans and per-group native
anchors, preserving the old single-group form. Lookout Main E58D40 has three
unconditional SCRIPT_NAME_HERO CCharString constructions followed by hit message
slot54 and matching bit-guarded destruction. Bits1/8/40, balanced-ESP string
slots164/14c/d8; constructors E58F42/E5A81B/E5AD47 (99EBF0), destructors
E59016/E5A8F3/E5AE2A (99EAE0). Full native/source hashes and54 instructions pinned.
Only first-set store pairs and exact final destructor guards removed; other
message conditions untouched. Draft loses exactly6 TODO comments.

24 focused tests pass;432 broader tests/35 subtests pass. Full29-unit A/B changes
only Lookout. Paired draft/readable smoke JSON identical,3 files/0 problems.
Readable compiled equality intentionally fails: removing the TODO barriers lets
existing readability passes elide non-yield termination rechecks, rename/coalesce
locals and split conversation locals. Expanded actual whole-function replay:
1536 before/after scenarios have identical calls, arguments, exits and errors,
omitting only redundant termination queries. Covers four polling paths, all8
hit combinations, talk/no-talk, yes/no,6 frame-cancellation limits and health0/100.
1024 scenarios complete without error;512 expose the same pre-existing nil movie
cleanup in both versions. These are recorded failures, not passing gameplay.
Initial probe omitted me:Speak; fixed the mock, then normalized only Lua error
line numbers. Promoted Lookout draft/readable+reports;371 corpus files parse.
No bundle change (Beggar absent);v114 remains current. Next: native movie stack
receiver drift causing the512 talk-path failures; also one repeat YesNo call
has four rather than five emitted arguments and needs native review later.

Live retry `beardy_v114_attack_probe_20261001_13` used v114 preflight and the
existing read-only Runner.probe, normal melee, fightDrain=false. No game was
running at preflight, but a separate Fable2 process appeared after launch and
frontend capture stayed `other`. Closed only owned Fable PID8832; next capture
failed, runner finally archived log/restored backup
scratchpad/save_backup_1234234_20261001-125356. Inventory verifies all561 original
save files unchanged, zero additions. No save-loaded/gameplay proof for this
run. Scratch `work/codex_lua_beardy_live_v114_20261001/runner.log`; runner finished.
Do not retry foreground testing while the other session is active.


### Lookout movie receiver/ECX join recovery promoted

Scratch `work/codex_lua_lookout_movies_20261001`. Extended existing
native_reviewed_movie_members with guarded typed stack-receiver aliases.
Four native movie objects at balanced ESP64/74/44/54, frame548, correspond to
entry slots-448/-432/-480/-464. Ghidra cleanup spellings drift20 bytes. Row pins
full native/source hashes and109 instructions, exact source occurrence counts,
receiver LEAs/stack identities and adjacent constructor calls. Three ECX join
stores (E594DC/E5959F/E59A9F -> shared destructor E59AA3) now retain their
assignments instead of matching the generic C++ destructor noise filter.
Only typed movie casts are rewritten, not unrelated thing/resource operands.
Resource getter source spellings updated for four constants exposed by existing
folding; all13 native getter witnesses and lifetime guards remain unchanged.

Initial early-cast prototype stopped on strict resource spelling inventory;
later scratch prototype after lifting fixed512 basic scenarios but left224
speech-cancellation failures because cleanup tails had already merged around
the wrong handles. Rejected that late pass. The final pass runs before lowering/
cleanup deduplication, keeping distinct native movie exits separate. Its output
equals the validated early prototype exactly. Conversion TODO count24->25 is
one retained native target annotation LAB_00e5a527_c2, not a missing operation.

52 focused tests and473 broader tests/35 subtests pass. Actual readable Main
replay18432 scenarios: candidate all pass movie ownership checks (no unknown,
repeated, or leaked movies); baseline3260 failures. 14660 complete traces equal.
512 previously successful cases add only a completed IsPerformingScriptTask
poll because the existing readable Speak/wait folding no longer absorbs that
loop shape; other calls/arguments/exits equal. Scenarios include all hit masks,
four polling paths, talk/no-talk,yes/no,frame limits1/2/3/4/7/12,health0/100,
initial belch counts0/1/3,taught/not-taught,and cancellation during speech.
Reproducible probe now reads saved pre_promotion baseline; early_validation.json
records both repaired baseline failures and extra pure poll cases separately.

Full29-unit A/B changes only Lookout. Reused readable outputs only after exact
full draft inventory equality against registered baseline and validated early/
integrated candidates; generator unchanged. Paired draft/readable smoke JSON
equal,3 files/0 diagnostics. Promoted only Lookout draft/readable+reports;
371 corpus Lua files parse. Beggar remains absent from bundle;v114 unchanged.
No additional game action. Omitted fifth argument at Lookout question E59DB6
is native push1 and binding defaults true (LuaQuestState.cpp:2423), so no current
behavior gap there. Next: BeggarBully movie cleanup receivers; evidence capture
in work/codex_lua_bully_movies_20261001, no Bully production changes yet.


### BeggarBully movie receiver recovery promoted

Scratch `work/codex_lua_bully_movies_20261001`. Extended guarded movie receiver
aliases with exact source constructor boundaries, preventing one reused Ghidra
name from joining different native objects. Four scoped substitutions repair
six typed destructor references for two native movie objects: balanced ESP48/5c,
frame384, entry slots-312/-292. Exports drift20 or40 bytes. Native/source hashes
and70 instructions pinned. Constructor E5B71C has an intervening byte flag store
after LEA E5B713; acceptance now requires pinned intervening instructions that
preserve ECX/ESP and cross no call/control-flow instruction. Lookout's immediate
constructor setup retains the same acceptance. Bully's nine resource getter
spellings/native witnesses are unchanged. Typed casts for unrelated objects stay
untouched. Recovery occurs before cleanup-tail merging.

60 focused tests pass;481 broader tests/35 subtests pass. Actual readable Main
replay27648 scenarios includes health100/0/NaN, all hit masks, polling/expression
paths, talk/no-talk,yes/no,6 frame limits,initial belch counts,taught/not-taught,
and cancellation during speech. Candidate all pass exact live-movie ownership
checks;baseline4872 fail. All22776 previously successful traces are identical.
Full29-unit A/B changes only BeggarBully; integrated draft exactly matches tested
prototype. Readable output reused after exact full draft inventory proofs.
Paired draft/readable smoke JSON equal,3 files/0 problems. Net one additional
conversion label diagnostic reflects retained native cleanup targets.

Promoted only Bully draft/readable+reports;371 corpus Lua files parse. Bundle
remains v114 because Beggar is absent; no new game action. Reproducible trace
probe reads its saved pre_promotion baseline. Next: missing cached listener in
Bully's TEXT_QST_015_BULLY_TAUNT line. Scratch codex_lua_bully_listener_20261001
captures lowered source and Lifter state. Native call E5C8DE has cached Thing
operand, and lowered C preserves &xStack_154; AddPerson consumes the fallback
result, leaving AddLine listener nil. Investigate stale zero-staged hidden output
slot at the original GetThingWithScriptName instead of guessing the listener.


### Bully cached conversation listener promoted

Scratch `work/codex_lua_bully_listener_20261001`. Narrow Lifter rule recognizes
zero-staged hidden Thing output in GetThingWithScriptName with exactly two
operands and an independently known string name, only in typed accessor mode.
Native E5B4CE writes balanced ESP2c/entry-340, then AddPerson E5C8AF and AddLine
E5C8DE use that same object. Lowered source names it xStack_154 but also stages
an earlier zero under that name; no claim those are the same native lifetime.
Previously AddPerson consumed fallback r1 and left the taunt listener missing.
Cached output now survives both uses, including an empty lookup result.

9 focused tests,490 broader tests/35 subtests pass. Actual readable Main27648
cases all pass;23040 traces identical,4608 differ only in taunt listener nil ->
cached Beggar. Full29-unit A/B changes only BeggarBully; exact draft inventories
prove readable inputs. Paired draft/readable smoke JSON identical,3 files/zero
problems. Promoted Bully draft/readable and reports;371 corpus files parse.
Final production edit after tests only clarifies a comment about source staging.
Bundle v114 unchanged (Beggar absent); no game action.

Parent quest tutorial branch investigated in codex_lua_beggar_tutorial_20261001:
E58343 loads byte+5a, E58348 JE E58459 exactly matches the unusual generated
skip-when-false behavior. Preserve retail condition; no change justified.
Next: Gate1GuardOuter MainD01630 unresolved typed interface alias pCVar11 and
GetHero slot118 leave CALL_OVER_FIRST listener as cached inner guard and SECOND
listener missing. Scratch codex_lua_gate_guard_listener_20261001 holds native
source/disassembly and lowered source. Investigate alias classification with
native receiver evidence, not guessed replacement arguments.


### Gate1GuardOuter hero listeners promoted; v115 staged

Scratch `work/codex_lua_gate_guard_listener_20261001`. Added narrow cast-vtable
annotation for (int)*local with an explicit nearby interface object load from
this/param_1 +4/+40, matching call receiver, and only intervening interface loads.
Rejects missing/wrong/reassigned receivers, branches/labels/calls, and unknown
slots. Native D0183A/D0188A GetHero slot118 results EAX are pushed as listeners
at AddLine D01853/D018A3. This removes wrong cached Gate1GuardInner for FIRST and
missing SECOND listener. Draft retains explicit native lookups. Readable existing
dead-value pass drops now-unused Gate1GuardInner lookup and caches Hero as before;
host lookup owns a temporary handle and logs, no quest state write. Trace claims
explicitly normalize that dropped lookup; no live-game equivalence claim.

20 focused alias tests;107 combined/28 subtests and510 broader/35 subtests pass.
Actual readable Main3888 cases (cancellation,acquire delays,gate states,hits,
health100/0/NaN) all pass.864 traces identical,3024 change only dropped dead lookup
and repaired listeners;2016 FIRST and651 SECOND lines target Hero. Full29 A/B
only Gate1GuardOuter. Paired smoke19 files,zero problems; draft+200 GetHero calls
on200 loop iterations,readable-1 unused lookup,all other JSON identical after
explicit reviewed fields. Promoted only Gate1GuardOuter draft/readable+reports;
371 corpus files parse. v115 derives v114:exact baseline payload match,only
Gate1GuardOuter Lua changed;210 Lua parse,216 hashes verify,DLLs unchanged.
Package staged only. No game launched; no owned processes remain.

Next scratch codex_lua_bandit_guard_lists_20261001: existing BanditCampBossBattle
ProcessHostageCutscenes D0FE80 and CheckForSecondAreaMassacre D0F640 lose list
elements when a CCharString-typed scalar actually holds begin+byteOffset before
GiveThingBestEnemyTarget. D0FE80 also leaves a vector-minus-itself sign fix in Lua
and native pre-first-iteration termination check needs review (D10897). Native
source/disassembly and lowered captures saved; no list production changes yet.


### Bandit defensive-guard list operands/counts promoted

Scratch `work/codex_lua_bandit_guard_lists_20261001`. Generic local Thing-vector
fold accepts a CCharString-cast scalar element address only with integer begin
declaration,no pointer declaration,known Thing vector fill,typed Thing consumer
before slot reassignment,and matching offset increment12. Existing byte-element
lowering then preserves table actor identity. Count sign correction now accepts
the uncast canonical begin/end spelling. Native LEA ECX+EBP and increment12 at
D108A6/E2 and D0FCC3/FF prove byte indexing. Slot70c calls D108C1/D0FCDE push Hero
as enemy and the element as actor.125 instruction witnesses saved.

31 focused tests;543 broader/35 subtests pass. Synthetic Lifter+finish_lua tests
cover empty/list lengths1,2,5 and cancellation limits0,1,3,9,plus adverse shapes.
Fixture initially used unsupported single-line return,then omitted converter's
live_termination=True; corrected to the actual registered pipeline settings.
No production termination behavior changed by those fixture corrections.
Actual readable helper replay320 cases passes:232 identical,40 baseline table
arithmetic errors fixed,48 other traces only repaired actor/enemy operands.
Existing nil actorMap in massacre mock is explicitly recorded/allowed; no native
resource-lifetime claim. Existing missing first-iteration termination query in
ProcessHostageCutscenes is preserved and remains next target.

Full29 A/B changes only BanditCampBossBattle.lua. Paired draft/readable smoke
19 files/zero problems,JSON identical. Promoted only that draft/readable file and
reports;371 corpus parse. Bundle not updated: v115 boss payload differs from
registered baseline beyond this repair (bundle_baseline.diff). stage_v116.py
was prepared but NOT run;its baseline assertion would reject. package_deferred
records this;v115 stays current,210 Lua/216 hashes from prior candidate. No game
action,no owned processes remain. Next: native D10897 loop-head termination
currently exported at bottom;move that proven call/test to the loop head using
native/source guarded evidence before lowering.


### Hostage guard loop-head cancellation promoted

Scratch `work/codex_lua_bandit_loop_poll_20261001`. Added generic reviewed loop
termination pass and unit row for D0FE80. Full native/source hashes,47 instruction
anchors,CB7940 direct target/AL test/JNE cleanup,nonempty fallthrough and JB
backedge prove poll at D10897 on every iteration. Native owner ESI is proven by
CFG reaching definitions restricted to entry-reachable blocks; global-write
rejection was too conservative because a separate exit branch reuses ESI,and
linear CFG includes unreachable roots. Original incoming-this definition is
the only entry-reachable definition at the poll. Changes outside evidence fail.

IMPORTANT rejected early approach: moving raw-C guard before operand recovery
changed unrelated HOSTAGEFREE actor-map resources (HERO1/HOST1=2/HOST2=3 became
HERO3/HOST1=1/HOST2=2). Actual full-function traces caught it. Stopped only owned
early A/B processes12712/3524/15172. Early ab/candidate/integrated artifacts and
rejected_early_validation.json retained; they are NOT accepted validation.
Final pass operates on finished draft before readable generation. Native/source
guards retained, exact Lua loop block moved without any other draft operation
change. Final artifacts late_draft,late_readable,late_trace_probe.py,ab_final.

21 focused tests;564 broader/35 subtests pass.320 complete helper cases pass;
18 traces change only native cancellation-tail suppression,16 prevent an extra
bandit command after cancellation. All unrelated hostage actor-map mappings
unchanged. Final29-unit A/B only boss quest. Paired draft/readable smoke19 files,
zero problems,identical JSON. Promoted only bossquest draft/readable+reports;
371 corpus parse. v115 unchanged:boss bundled baseline still older,no v116.
No game action or owned process remains.

Next codex_lua_bandit_actor_map_20261001: scratch-only RE_MAP_NEW prototype accepts
equivalent *(undefined **)(map+4)=(undefined *)0x0 store. Native D0FB82 alloc36,
zero parent and self-linked children match existing empty actor-map constructor.
Only five constructor TODOs become NewActorMap.320 actual helper cases with
strict map ownership pass;80 baseline nil-map failures fixed,240 traces equal.
No production map-regex edit yet; capture baseline after this poll promotion,
add adverse tests,integrate and run full29 A/B before promotion.


### Typed-null actor-map constructor promoted

Scratch work/codex_lua_bandit_actor_map_20261001. RE_MAP_NEW accepts the equivalent
four-byte typed-null parent store while retaining allocation size36, zero flag,
same-head self children and map identity checks. D0F640 native evidence records
48 instructions: D0FB82 malloc36, D0FB96 parentzero, self children D0FBA0/D0FBB2,
D0FBC7 macro and D0FC1D destructor. Five unresolved constructor operations now
become NewActorMap, preserving allocation/use/destruction ownership.

21 new adverse/positive tests;27 focused combined;591 broader/35 subtests pass.
320 actual-helper cases:240 identical,80 baseline nil-map failures repaired,
candidate errors zero. Full29 A/B only boss quest; complete input identity
verified for readable reuse. Paired draft/readable smoke19 files/zero problems,
identical JSON. Only boss draft/readable and reports promoted;371 corpus parse.
No game action. v115 remains staged; older boss bundle baseline still deferred.

Next evidence scratch codex_lua_bandit_health_threshold_20261001: health threshold
is already correct (truncated health*0.25), no change. Two HERO map stores in
BanditKingMissionProcess D109F0 remain unresolved. Native D10CBA/D10DA5 assign
the counted-pointer member at resource+8 into map value+8; hero resource base
is balanced ESP+48 (GetHero D10C4C / acquire D10C56), sources ESP+50 at both
assignments. Investigate guarded field-address support in fold_counted_map_stores.
No production change for this next item yet.


### Bandit King intro HERO actor-map stores promoted

Scratch codex_lua_bandit_health_threshold_20261001 (initial threshold lead was
correct and unchanged). fold_counted_map_stores accepts explicit resource+8
field address only for proven resource/map constructors with no intervening
reassignment, release, branch or label. Native79 anchors capture D10C4C GetHero,
D10C56 acquire at balanced ESP+48, D10CBA/D10DA5 counted member ESP+50 into
map-value+8. Preserves existing plain-resource spelling and native target8AB1E0.

First integration incorrectly treated C++ namespace :: as a label, recovering
only first store. Corrected negative lookahead and repeated-store regression.
Initial ab directory incomplete/rejected; stopped only owned matching Python
processes. Accepted ab_final completes29 units, only boss quest changed.
Whole-directory pytest crashed in Unicorn mem_map/test_affair_man_complete;
unicorn_crash.log retained, owned hung Python7688 stopped. This is NOT counted
as a passing whole suite. Established scoped generator test set plus new tests
passes612 tests/35 subtests, including21 focused field-store tests.

Actual complete helper intro/cancellation paths:864 cases,432 unchanged and432
gain exactly two HERO stores; actor/control cleanup and all other traced calls
equal. Readable hero control shorthand normalized to corresponding resource
acquire/release for comparison. Cases cover region delay,early cancellation,
wall counts/cancellation,acquire result and health. Later combat paths outside
this probe's execution. Prototype/integrated draft all19 inputs byte-identical.
Paired draft/readable smoke19 files/zero problems and identical JSON. Promoted
only boss draft/readable+reports;371 corpus parse. No game action;v115 unchanged.

Next codex_lua_hostage_string_cleanup_20261001: scratch-only reviewed temporary
cleanup row for D0FE80. Two empty CCharString arguments at ESP+10/+44, bit flags
1/2 in EBX/ESP+1c, matched ctor99EBF0/message48/dtor99EAE0. Existing generic
reviewedTemporaryCleanup groups support it; candidate removes two TODOs. No
unit evidence mutation or promotion yet. Validate complete helper behavior.


### Hostage temporary-string cleanup promoted

Scratch codex_lua_hostage_string_cleanup_20261001. Unit-only reviewedTemporaryCleanup
row D0FE80 uses existing grouped pass; no shared generator changes.41 native anchors,
full body/source hashes: empty string byte122D70E, constructors D0FFDA/D1000B,
stack strings ESP+10/+44 passed to message48 and matching destructors D1005E/D10049;
EBX/ESP+1c bits1/2 track construction. Exact source spans remove flags and guarded
destructors only; constructors/message evaluation and short circuits remain.
RData printable-string helper excludes empty strings, so evidence validates NUL
byte directly. Same xStack78 name later reused for another string stays intact.

Two newline variants and10 adverse evidence cases pass;612 scoped tests/35 subtests.
1296 complete hostage traces identical, covering null/alive/killed combinations,
rescue/gate/initial-killed state,region delay,list sizes,cancellation. All19 draft
inputs integrated/prototype byte-identical; unit JSON differs only new evidence row.
Unit-only19-file comparison changes only boss quest; no29-unit rerun required
because shared lowering unchanged. Paired draft/readable smoke19/0 identical.
Only boss draft/readable+reports promoted;371 corpus parse. Boss quest now has
zero unresolved diagnostics (not proof of complete runtime equivalence). No game.

Next review older boss payload in v115 before staging v116. Earlier packaging was
deferred because pre-existing readable changes differ from bundled baseline.
Need account for those exact changes plus newly promoted list/poll/map/hero/cleanup
repairs; preserve both DLLs and all other payloads. v115 still current.


### Older Bandit boss bundle reviewed; v116 staged

Scratch codex_lua_bandit_bundle_v116_20261001 pins v115 and current boss payload
bytes/hashes and exact diff. Four changed helpers only: BanditKingMissionProcess,
ProcessHostageCutscenes,CheckForFirstAreaMassacre,CheckForSecondAreaMassacre.
Resolved earlier packaging deferral by testing complete old/new payloads.

Boss1584 cases:288 identical,1296 gain only two intro HERO stores;300 complete
missions. Covers region delay,cancellation through combat/timer/flashback,
wall lists,health,acquire outcomes,boss alive/killed morality. First fixture
incorrectly gave sister same object as boss; fixed harness identity,not Lua.
Other helpers6912 cases:hostage1280 identical/1024 old failures repaired;
firstarea1008 identical/1296 native loop repairs;secondarea1152 identical/1152
old nil-map failures repaired. All candidate cases succeed. Old errors:256
table arithmetic,768 nil cleanup-word bitops,1152 nil maps; exact trace prefixes
retained. First-area delta checks exact bandit list identity/order and native
cancellation/leader tail. Native D0F4D5 nonempty fallthrough to D0F4D7 poll,
D0F4EF element address,D0F505 slot70c command,D0F526 stride12,D0F52B backedge
confirm current loop; v115 bypassed it. Source body hash/bytes captured separately.

v116 derives v115; only NoviCompatibility/BanditCampBossBattle/BanditCampBossBattle.lua
payload changes,plus README/STAGING metadata. 210 Lua parse,216 manifest hashes;
stock36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea and
sidecar7331252ec13a5babe4a56730eb6a7798fe953ddea2464db7fca589f1621ee54a unchanged.
Staged only,not installed;no game launched. Earlier guard-lists stage_v116 helper
remains unused/superseded by this reviewed bundle staging script. Runtime proof
still separate. Continue native recovery on remaining registered Bandit entities.


### Gate3Guard cancellation cleanup promoted; v117 staged

Scratch codex_lua_gate3_cleanup_branch_20261001. New native_export_labels.py
normalizes unique override_prt definitions and goto references to unused LAB
names. Collision-safe bijection; strings/comments opaque; duplicate/missing
definitions,unknown uses and malformed names stay unresolved. Lifter integration
preserves the source branch that previously became a TODO. Native D07709 F35B30
query,AL test,D07719 JNE D07C09 proves cancellation bypasses timer getter168.
Target deregisters staged EBP timer via global interface160,releases resource
at ESP+54 through7E74D0,returns.21 instruction anchors/bodyhash saved.

15 focused tests;627 scoped tests/35 subtests.1536 actual Main replays:1440
identical,96 stop immediately at restored cancellation gate with exact timer
deregister/resource release. Attack/special/heal/gate/conversation/acquisition,
timer,distance,health and frame cancellation paths covered. Full29 A/B only
Gate3Guard;integrated/prototype draft equal except canonical label spelling.
Paired smoke19/0: draftJSON equal;readable+200 calls from restored poll across
200 iterations,frames201 unchanged;all other JSON fields equal after explicit
normalization. Original unequal results retained;review_smokes records delta.
Only Gate3Guard draft/readable+reports promoted;371 corpus parse.

Older v116 Gate3Guard payload reviewed separately:1536 cases,1248 identical,
96 cancellation repairs and192 pre-existing physical-hit nil cleanup-word
failures repaired. Native D0762C/D07657/D07681 construct HERO strings at
ESP+20/+24/+28; bits1/2/4 track those lifetimes; D076BE/D076D3/D076E8 destroy
them,leaving short-circuit message54/A8/A4 semantics. Other older changes are
cleanup return inlining and dead locals. Exact bundled baseline bytes retained.
v117 derivesv116:only Gate3Guard payload+README/STAGING;210Lua/216hashes;both
DLL hashes unchanged. Staged only,not installed. No game launched. Read-only
processcheck found other Fable2.exe PID6556 under Fable2RE;do not interfere.

Next codex_lua_forger_string_cleanup_20261001: scratch-only cleanup row removes
two remaining BanditForger TODOs around pass-name string. D0D281 signed-byte
test/JNS,D0D29A destructor correspond bit80. Capture/candidate ready; no unit
evidence mutation yet. Baseline/candidate drafts include just-promoted Gate3 fix.
Validate behavior before integration; preserve active v117 package.


### BanditForger pass-name temporary cleanup promoted

Scratch codex_lua_forger_string_cleanup_20261001. Added one reviewedTemporaryCleanup
row at D0C410 to existing unit map,preserving hostage row.32 native anchors:
Gate2Open D0D239 skips construction;literal12C8450 pass object name,ctorD0D246
at ESP+74,bit80 of ESP+28,GetHero/possession queryD0D26D,signed-byte test/JNS
D0D287,clearbit and matching dtorD0D29A. Only compiler cleanup spans removed.
Two newline variants/five adverse evidence cases pass. Unit-only,no shared code.

Draft changes only unused CVar8/CVar9/xStack8c declarations,sole zero store,and
two TODO comments. Reference counts prove no other uses after ignoring comments.
All20 draft chunks compile identically after excluding those unused locals/store.
19/20 readable chunks compile identically;Forger differs because cleanup removal
enables existing short-circuit simplifier. Exact readable diff constrained to
predicate block and local declaration. Actual before/after predicate and query
order identical in25 cases including false/true/nil/zero/throwing queries.
compiled_initial.json retains unnormalized readable-bytecode inequality; this
is not claimed as raw bytecode equality or full-Main behavioral replay.

627 scoped tests/35 subtests pass. Integrated/prototype all20 draft chunks equal;
unit JSON differs only one row. Unit-only comparison changes just BanditForger.
Paired smoke19files/zero problems,identical JSON;promoted Forger draft/readable
and reports,371 corpus parse. v117 unchanged;Forger older bundle baseline not
yet reviewed for packaging. No game action. Last other game Fable2PID6556.

Next inspect Q_BanditCamp.CheckAnyBanditsKilled D032F0: remaining __EH_epilog3
TODO and apparent backedge after vector clear may bypass fresh kill-group query.
Do not infer bug solely from readable output;trace native/export first. Known
helper4502EB already has reviewed no-callfixup signature in infer_helper_prototypes.
No production change for this next lead yet.


### Bandit kill-group loop reentry promoted and v118 staged

Scratch codex_lua_bandit_kill_groups_20261001. Native D032F0 CheckAnyBanditsKilled:
D03556 backedge returns to D03417 vector initialization, GetHero D03433 and
kill-group query D03442 every frame. Old sibling-tail copying returned to the
post-loop block after the first nonempty list, permanently skipping fresh queries.
100 native instruction anchors and body hash retained in native_evidence.json.

New native_loop_reentry.flatten runs before sibling-tail duplication. Conservative
constant-true loop shape: backward external entry, direct tail label, straight-line
header ending in a conditional break, no nested loops/switch. Fresh explicit head
and exit labels retain backedges and rewrite own break/continue transfers.
21 focused tests;723 scoped tests/43 subtests pass. Integrated/prototype complete
20-chunk draft equality proves reuse of actual candidate behavior tests.
3360 actual generated-helper cases match the independent native control model:
1632 unchanged,1728 repaired traces,370 corrected boast states. Successive-frame
kill groups, group-bit filtering, boasts, thresholds, region delay and frame exits.
Exact v117 helper replay gives the same counts;older root delta stays in this helper.

Rejected broad prototype modified extra Bandit files. First production attempt
failed BordelloLady pinned operand review;ab is incomplete/rejected. Linear-header
guard alone still changed BordelloGuard with nested loops. Final shape excludes
nested loops;ab_final is accepted full29 comparison,only root BanditCamp.lua changes.
Pinned Bordello reviews unchanged. No evidence checks were weakened.
Paired draft/readable smoke19 files/zero problems, identical JSON. Promoted only
root draft/readable and reports;371 corpus files parse. v118 derives v117 with
only root payload plus notes;210 Lua files parse,216 manifest hashes,unchanged DLLs.
Staged only,not installed or proven in game. Forger older bundle review still deferred.

Next: separate remaining __EH_epilog3 TODO. capacity_helper_native.json proves
4502EB is mov eax,ecx;mov ecx,[esp+8];mov [eax],ecx;ret8,not exception teardown.
Caller D03429 writes the vector capacity word; prove receiver/stack/header pairing
before replacing its unresolved diagnostic. No production change for that lead.


### Bandit list capacity initialization promoted; v119 staged with Forger cleanup

Scratch codex_lua_bandit_capacity_init_20261001. native_reviewed_list_capacity
plus reviewedListCapacityInit row D032F0 removes the mislabeled __EH_epilog3 call
only after vector-to-list lowering. Native4502EB is an11byte destination-store
helper,secondstackword value,firstignored,ret8. CallerD03429 receiver is capacity
at balancedESP24;begin/endESP1c/20 zero stores;EBP reaching definitions prove zero;
same header goes to hero messageDC D03442. Helper bytes,fullbody/source/lowered
hashes,16anchors,adjacency,message slot and nonescaping capacity/allocator all checked.
17 focused cases cover two newline forms,stale hashes,escaping variables,changed
helper,zero definition,header adjacency,message slot and missing witnesses.

740 scoped tests/43subtests. Full29 A/B onlyrootBanditCamp.lua. Complete20chunk
prototype/integrated draft equality. Draft delta only diagnostic comment. Readable
also loses one known-false termination poll after the loop exit label: existing
readability dataflow can now see through the non-yielding GetHero/message queries.
First compiled probe failed;compiled_raw_line_delta.json preserves original unequal
hashes. Exact source delta constrained to comment and this poll. All40 draft/readable
chunks compile identically after comment/whitespace normalization and the single
explicitly reviewed poll normalization. Do not claim raw bytecode equality.
3360 actual helper scenarios preserve state and all non-poll calls:1088 exact,
2272 with only one fewer poll per nonempty query (7360 total). Scheduler/frame exits
remain covered. Binding inspection confirms ordinary synchronous query/list copy;
no new scheduler or readability code. Paired smokes19/0 identical,371 corpus parses.

Promoted root draft/readable and reports. v119 derivesv118 with root and Forger
payloads plus notes. Exact v118 Forger file equals earlier Forger baseline, current
file equals its validated candidate;25predicate/call-order cases therefore apply.
This closes the deferred older Forger bundle review.210packageLua parses,216hashes,
DLLs unchanged. Staged only,not installed/live-proven. Last othergameFable2PID6188;
no game launched or interfered with.

Next inspect BanditForger D0C410 cancellation cleanup at D0D3C8: converter reports
one cleanup lifted as early return. Establish actual native movie/unpause/resource
effects and generated reachability before changing anything.


### CampHostageGuard shared movie cleanup promoted; v120 staged

Forger D0D3C8 lead was not a bug: D0C811 termination branch reaches native timer
deregistration D0D3D5 and resource destructor D0D3DF; generated cleanup closure
already performs both. No change made for that diagnostic.

Scratch codex_lua_hostage_guard_movies_20261001 instead confirms six omitted
CampHostageGuard D09010 movie releases. Native movie bases entryESP-152/-176,
counted members +8 at -144/-168. Normal releases D098FC,D09A47,D0A1F1,D0A32C;
cancellation releases D09DA7,D09DF7. Three shared base-destructor joins D09A58,
D09E08,D0A33D.131native anchors and full source/body hashes retained.
New generic native_movie_phi_cleanup recognizes release+base-vtable+mergedreceiver
only for proven movie constructions and adjacent counted members, with every join
variable use restricted to its declaration/assignments/base destructors. Activated
only through reviewedMovieMembers sharedBaseDestructors evidence; native stack,
start/release identities,witnesses and source inventories validated before edits.

Broad scratch candidate_draft changed five actors; NOT promoted. Retained
broader_draft_delta.json documents Assassin1/2/3,Gate3Guard and CampHostageGuard.
Accepted reviewed_draft/reviewed_readable changes only CampHostageGuard. Full29
A/B confirms only that file. Baseline overlay suppresses only the newly added
review row; existing unit JSON differs solely that row. Complete20chunk integrated
and reviewed-prototype draft equality verified.21newtests/64combinedmovie tests;
761 scoped tests/43subtests pass.

Actual Main5760cases:4140 identical,1620movie cleanup repairs,3170 leaks fixed.
Candidate closes every movie it starts,immediately after unpause;all nonmovie
calls identical. Covers early/late and transitioning hostages state,talk,health,
speech duration,frame cancellation,acquire retry,physical/special/healing attacks,
hostage death,timers. Older v119 file has short-circuit readability differences;
expanded23040bundle scenarios add movement/proximity and query-call ordering:
16560identical,6480repairs,12912leaks fixed;all other calls identical. No raw-bytecode
equivalence claim for this behavioral repair. Paired smoke19/0 identical.
Promoted guard draft/readable plus reports;371corpus files parse. v120 derivesv119
onlyguardpayload/notes;210packageLua,216hashes,unchangedDLLs. Staged only,
not installed/live-proven. No game activity;last otherFable2PID6188.

Next: Gate3Guard D07490 has four analogous missing releases in broader scratch
output. Capture its native movie identities/joins before adding a separate review
row. Existing Gate3 cancellation harness can be extended to track movie lifetime;
its prior harness checked timers/resources only and did not implement DestroyMovie.
Assassin1/2/3 broader candidates remain unreviewed/unpromoted.


### Gate3Guard shared movie cleanup promoted; v121 staged

Scratch codex_lua_gate3_movies_20261001. Existing reviewedMovieMembers mechanism,
new unit-only D07490 row;no shared generator change.80native anchors: movie bases
entryESP-32/-16,counted members-24/-8,frame132. StartsD07934/D07AD0;normal
releasesD07A09/D07BA4,cancellationD07CB3/D07CE6,base joinsD07BB5/D07CF7.
Two newline positives/six adverse evidence cases;64 focused movie tests pass.
Unit JSON differs solely new row and unit-only20chunk draft/readable comparison
changes onlyGate3Guard. No new full29 A/B claimed; shared mechanism checked in
the immediately preceding CampHostageGuard task.

Extended prior actualMain harness to track each movie,retaining timer/resource
checks.6144cases:5904identical,240moviecleanup repairs,1024leaksfixed;candidate
allmoviesclosed immediately afterunpause,allothercallsidentical. Covers hit,
special/heal,gate,cancellationatthegate,timer,distance,talk,acquire retry,
frame limits,health andspeech duration. v120payload byte-identical to tested
baseline;no older bundle gap. Paired smoke19/0identical,371corpus filesparse.
Promoted guard and reports;v121derivesv120onlyGate3Guardplusnotes.210packageLua,
216hashes,unchangedDLLs. Staged only,not installed or live-proven.

User asked overall progress during work;answered with current registry57parent
scripts/29units,out of161 inventoried parent scripts;371auditfiles includesentity,
support andauthored files.88needrecovery,128readability,155humanreview;zero syntax
failures. Explained mechanically clean is not behavioral certification, andlive
milestones cover specific paths/versions. User did not stopcontinuouswork.

Next: native review of Assassin2/Assassin3 analogous shared movie cleanups;
their similar Main bodies may share a behavioral harness. Assassin1 remains
unreviewed. Broad scratch artifacts fromHostageGuard are not accepted outputs.


### Assassin2 / Assassin3 movie cleanup promoted; v122 staged

Scratch work/codex_lua_assassin23_movies_20261001. Unit-only reviewedMovieMembers
rows D05A60 and D06130; existing shared mechanism unchanged. Each actor has 95
native anchors and two positive/six adverse evidence cases; 64 focused movie
tests passed. Movie bases entryESP -32/-16, members -24/-8, frame 120.
Assassin2 normal releases D05DF8/D05F23 join D05F34; Assassin3 D064C8/D065F3
join D06604. Existing cancellation cleanup retained.

Actual generated Main before/after: each 3840 cases, 3580 identical, 260 repaired
paths and 380 movie leaks fixed. Total 7680 cases/520 repairs/760 leaks. Every
candidate movie closes immediately after unpause; quest state and all other
calls identical. Covers speech duration, health, attack/heal/special events,
cutscene termination, talk, frame limits, and resource acquisition retries.
Unit JSON differs only these rows; complete 20-chunk draft/readable comparisons
change only these two actors. Paired smoke 19 files/zero problems/identical.
Promoted only reviewed outputs and reports; corpus 371 files, zero syntax failures.
v121 payloads byte-identical to tested baseline. v122 changes only the two actors
and notes: 210 parsed Lua files, 216 hashes, unchanged stock/sidecar DLLs.
Staged only, not installed or live-proven.

User clarified progress question meant autonomous IN-GAME testing. Reviewed live
journals and reported at least ten completed chains/sections: childhood, Guild,
Wasp, Maze sister info, good Orchard, Trader Escort, Bandit Camp, Maze trophy
info, Trophy Dealer, White Balverine. Book Collecting final checkpoint/reward
and reload separately; Bordello partial scenarios. Assistance and version/path
limits explicit; no verified individual-script numerator claimed. Continuous
work remains authorized. Next native review: Assassin1 shared movie cleanup.


### Assassin1 movie cleanup promoted; v123 staged with older-bundle review

Scratch work/codex_lua_assassin1_movies_20261001. Unit-only D04AE0 row;
127 native anchors, 2 positive/6 adverse evidence cases, 64 focused tests.
Frame248, movie bases entryESP-216/-196/-176, counted members+8.
Normal releases D05390/D05794/D04EB7 share D057A5 base destructor;
existing three cancellation destructors retained. Only Assassin1 changes in
complete unit draft/readable comparison; readable adds exactly three DestroyMovie
calls after unpause. Actual Main matrix 131072 cases: 121024 identical,
10048 repaired, 16576 leaks fixed. Candidate all movies closed; other calls and
state identical, including gold and XP. Speech boolean result/duration, question
answer/delay, gold below/equal/above cost, gate, health, attack/heal, acquisition,
frame limits, initial attack and external cutscene state covered. Paired smoke
19/0 identical. Promoted; corpus371 parses with unchanged status counts.

Initial v123 staging STOPPED before writes: v122 actor differs from current
baseline. older_bundle_delta.diff retained. It has three nil stubs for hero
special attack, heal exclusion, and talk plus earlier readability changes.
Native D04C9F slotA8, D04CCB slotA4 with ability14, D04D60 slot6C and hero-string
arguments reviewed against existing bindings. Raw bundle replay131072:
104448 identical/26624 changed; candidate all movies closed; unaffected paths
identical after removing added event queries. Initial probe filter omitted
Heal|14, corrected in harness (no production change). Separate normalized probe
substitutes ONLY three missing calls in scratch old Lua:121024identical/
10048cleanup repairs, all other calls/state match. This is explicitly NOT raw
bundle equivalence. bundle_baseline_review.json pins hashes and native anchors.

v123 now staged: only Assassin1 plus notes versus v122,210Lua/216hashes,
unchanged stock and sidecar DLLs. Not installed or live-proven. Last read-only
process check found no Fable or Fable2 process. No game launched this task.
Next: inspect existing BanditCamp live runner/checkpoints for targeted Assassin
conversation validation; retain automated-vs-assisted proof distinctions.


### v123 live Assassin attempt in progress (owned scratch)

Scratch work/codex_lua_assassin_live_v123_20261001. Attempt1 reached only opening
movie and timed out: brightness frontend detector misclassified movie frames.
Captured24frames retained; owned front_movie_guard.py distinguishes all24 from
10existing nonmoviefixtures using letterbox regions. Scratch wrapper checks other
Fable2 processes before input and retries one invalid-size capture before aborting;
no shared driver edit. Original save inventory561 identical after attempt1.

Attempt2 loaded old maze2 checkpoint, accepted Bandit card through UI, applied
recorded disguise/gold assistance, passed gate1(122s), second guard, Forger pass
(65s), gate2(13s), region readiness. Verified Assassin1 target on second approach,
but no offer/movie began and TalkedToAssassin stayedfalse; timed out. Background
LISTENED dialogue ran. No Assassin pass claimed. Earlier empty setup replies were
item UI timeout artifacts; full log records commands processed after dismissal.
Game-written checkpoint copied stably to owned gate2_checkpoint. Game closed;
all561 originals unchanged. Logs work/runner/assassin_v123_decline_20261001_2*.

Attempt3 used retained checkpoint and added talkStarted movie acknowledgement.
Readiness check caught Gate2Open false after fresh load, so stopped before talk.
Game closed; all561 originals unchanged. BossBattle OnPersist is empty; checkpoint
is NOT a proven ready-to-talk save. Do not assume gate2 persists across reload.

Attempt4 ACTIVE: run4.py / runner4.log, tagassassin_v123_decline_20261001_4.
Owned runner session2359, launches v123 from owned checkpoint, repeats guard2,
Forger purchase, gate2 before Assassin region and guarded/acknowledged talk.
No parallel input or exec channel while runner owns game. Let runner finally close
its Fable and restore saves; verify inventory after exit. All prior owned sessions
finished. No bundle or shared Lua mutations during live attempt.


User steering: "Wanna use screen so I closed game for now". No further game
launches or screen/input use until user says screen available. Continue authorized
OFFLINE Lua work. Attempt4 exited during initial ESC after user closed game;
runner finally archived log and restored backup1234234_20261001-164901.
Session2359 finished. Save inventory561before/561after,zerochanged/zeroadded.
No active owned game or runner. Attempt4 is user-interrupted, not gameplay failure.
Next offline: investigate Assassin1 event/interaction path using retained live
logs and native evidence; do not claim Assassin live cleanup proof.

User subsequently said: "You can launch again for testing if you need".
Screen restriction lifted; game testing authorized again. Finish reviewed
AssassinMarker receiver validation before the next owned launch.

AssassinMarker native resource receivers promoted and staged as v124.
Scratch: work/codex_lua_assassin_marker_receivers_20261001.
Three Speak and six task queries now use their acquired assassin resource;
145 native anchors, pinned source/lowered/native hashes and helper bytes.
Two newline positives, absent-row preservation, twelve adverse evidence cases.
34 focused tests; 795 scoped tests /43 subtests passed. Final full29 A/B
(ab_reviewed) changes only AssassinMarker. Broad ab and ab_final designs are
superseded: they also changed ArenaCellDoorGuard and were not promoted.
6048 actual generated Main cases: 5103 identical /945 receiver repairs /
1620 misdirected speeches corrected. Candidate matches native receiver oracle;
changed raw traces are not equivalent. Same coverage against actual v123 Lua.
Complete20chunk identity permits reuse of readable and smoke proof (19 files,
zero problems). Corpus371 parses. v124:210 Lua parse/216 manifest hashes;
only Marker plus notes differ; both DLLs unchanged. No Marker live pass yet.
Earlier missed Assassin1 offer remains unexplained by this attack-path fix.
User again requested continuous work; next owned v124 live replay.

v124 live attempt5 passed Assassin1 introduction and decline. Identity check
true at [0,1.5], INTRO, 2000-gold question, NO response, TalkedToAssassin true,
AssassinCutsceneTriggered false, movie/cutscene false; movie17 destroyed.
Runner finished done; no logged Lua errors. Saves561/561 zero changes/additions.
Assisted route teleports and one model-triggered RMB No after visible offer:
global clear-only pause ladder did not answer. Owned config corrected to
per-Assassin question ladder for next replay. This proves conversation path,
not repaired Marker attack receivers. Next owned attempt6 paid path verifies
2000 debit, macro completion and Gate3Open, without state flag setup.

v124 attempt6 paid path completed in-game: actual INTRO and YES speech,
4620 ->2620 gold, TalkedToAssassin true, AssassinCutsceneTriggered true,
Gate3Open true, movie/cutscene false. Macro commands ran through removal and
fade-in; all5 logged movie starts match destruction. Assisted route only;
question answered by configured LMB. Saves561/561 unchanged,zero additions.
Two harness eval diagnostics (not quest Lua faults): target query saw nil,
safely rejected; final gold query referenced Runner in wrong VM. Runner's
finished flag alone was too weak for that final query. Do not claim clean
harness completion. Independent quest_status balances establish exact debit;
review_live.py preserves diagnostics and verifies observed state/movie proof.
Future owned config now nil-safe target, finish stateHost LUAGameflow and
global done includes exact debit. Next attack replay tests repaired receivers.

v124 attack attempt7 failed before speech path: eleven+ real swing inputs,
Assassin1 health remained110, distance4.8-5.4, AssassinsUnderAttack false.
No ATTACKED speech or Lua error. Test placement could relocate outside reach;
fightStep teleports and sets facing in same batch (teleport may overwrite).
Owned attempt8 wrapper adds close [0,1.2] placement and separate acknowledged
facing batch before input. No health drain or flag setup. Saves561 unchanged.
Arena existing guard inspection captured Main F17C70 native/lowered source:
work/codex_lua_arena_guard_resources_20261001. No edits/promotions there.

v124 attack attempt8 completed: closer [0,1.2] teleports plus later facing
acknowledged, real input triggers AssassinsUnderAttack; movie true observed,
44-second attack scene then movie/cutscene false, movie17 destroyed, runner done.
No quest Lua or harness errors;561 original saves unchanged,zero additions.
Resource Speak emits no EntitySpeechDiag/key log, so individual attack speech
identity/delivery is not yet independently observed. Do not label allthree
speeches live-verified from completion alone. Next owned attempt9 captures
scene frames; source fix and bundle unchanged. No flag or health setup.

v124 attack attempt9 completed (48seconds): real hit, attack flag, movie
started/ended, control returned, all4moviesclosed, no quest/harness errors.
16 scene captures retained; manually inspected001/006/011 show three native
attack lines and camera/speakers in sequence. attack9_visual_review.json pins
frame hashes and captions. Last3 acquisitions repeat the preceding three control-resource handles in
matching order. A direct comparison to CScriptThing pImp.Data failed: different
pointer representations. No direct native-data identity proof claimed. No flag setup/health drain; close placement and
facing assisted. Saves561/561 unchanged,zero additions. This adds live delivery
proof for reviewed Marker receiver repair, beyond attempt8 completion-only.
Assassin decline/paid/attack covered; paid attempt6 harness caveats remain.
Next existing ArenaCellDoorGuard resource construction/early cleanup recovery:
prototype2 native lowering only, no promotion. New reviewed constructor module
not integrated yet;16anchors witness;need focused/adverse tests, behavior,
shared full29 A/B, readable/smoke before promotion/package. No live game now.

ArenaCellDoorGuard resource construction + early cleanup promoted OFFLINE.
Scratch codex_lua_arena_guard_resources_20261001: new gated native-reviewed
constructor module/unit row;16anchors,fullbody/source/lowered/helper witnesses.
41focused (26new+15existing named resource),821scoped/43subtests passed.
Final full29 A/B changes only guard;registered draft baseline exact;complete
22draft/23readable identity verified. Actual generated Main512cases across
acquire delays, idle termination and adverse false frame return: all strict
baseline cases reject nil resource; candidate all resources close. Normalized
receiver+early-destructor oracle matches within each style; NO raw equality,
NO full retail equivalence or conversation coverage. Early cleanup repairs:
draft240/readable247 (perstyle256cases); false frame return preserves baseline
readable short-circuit behavior, not a new change. Initial fixture setup and
missing hit methods corrected before final pass.
Smoke16files perstyle: only guard Main call count+1. Fulltrace proves exact
added exit ReleaseResource. Draft unchanged existing free global extraout_EAX
and ArenaEnemy.OnPredicateFail arithmetic nil; readable has no actual errors,
but Main/InitialiseVariables retain pending InitialiseArenaRounds binding.
Corpus371 parses;statuses88recovery/128readability/155review unchanged.
Arena is ABSENT from v124 bundle: no package/in-game claim for this fix.
Next existing guard recovery: lost speech operands/other-guard receivers;
keep excluded until native evidence and behavioral validation support it.

ArenaCellDoorGuard speech operands and resource task receivers promoted OFFLINE.
Scratch codex_lua_arena_guard_speech_20261001: 28 speech calls,60 task queries,
2 command clears;379 native anchors. Exact native/source/lowered/helper gates,
lexical call pairing (not machine address order), CFG-reaching ECX/EDI proof.
Preserves native hero-return listener variable; named guard/self listeners and
all six speech operands recovered. Reviewed module replays the full witness
inventory before rewriting lowered C; generated Lua never hand-edited.
23 focused positive/adverse tests;844 broader tests/43subtests passed. Pytest
constructor emulation emitted Windows Unicorn access-violation diagnostics
although the first run ultimately passed; final fixture isolates unrelated
constructor defaults. Full29 A/B uses actual constructors, only Guard changed.
Complete22draft/23readable scratch identities verified. Actual generated Main
3072cases PER STYLE: all26distinct keys, exact native speaker/listener oracle,
waits reject wrong resource, pending command clear, dialogue/attack/early repeat
and cancellation. All tested resources/movies close. No raw baseline trace
equivalence or full retail behavior claim. Smoke16files perstyle JSON identical;
existing draft extraout_EAX/ArenaEnemy nil arithmetic and both styles' pending
InitialiseArenaRounds remain. Corpus371 parses,88/128/155statuses unchanged.
Arena still ABSENT from v124: no Arena gamepackage/live claim. User renewed
game authorization while showering. Next clean paid Assassin replay using the
fixed nil-safe target query and LUAGameflow final gold probe; attempt6 had two
harness diagnostics despite paid-path completion. Continuous work authorized.

v124 Assassin paid replay10 PASSED cleanly in game. Native game-written Gate2
checkpoint; assisted travel/proximity/targeting and real Yes input. Nil-safe
target query and LUAGameflow final gold probe fixes verified. Guard2/Forger/
Gate2 route repeated; Forger5620->4620, Assassin4620->2620 exactly2000. INTRO
and YES speech on Assassin1 verified by logged actor data. Gate3Open true,
TalkedToAssassin/Triggered true, movie/cutscene false; final finish step_done
and finished done=true/failed=false. Five movie starts/destroys match; ZERO
quest/harness errors or tolerated diagnostics. run10.py,runner10.log and
assassin_v124_paid_20261001_10_review.json retained. All561original saves
restored unchanged; no additions. Game closed. No Arena livepass added.
Next existing ArenaCellDoorGuard2 native constructor/cleanup prototype:
frame220,slot-200; constructor F19BC6,base destructor F19DE7,resource destructor
F1A793 incorrectly lowered as DestroyMovie. Scratch codex_lua_arena_guard2_
resources_20261001;30anchors,6acquisitions. Generic constructor module has
unpromoted extensions for native stack pushes and EBX zero reaching proof;
first guard26existing tests still pass. Guard2 row NOT registered/promoted.
Need adverse evidence/actual Main tests/full29 A/B/readable/smoke before promotion.
Arena InitialiseArenaRounds source binding IS present in the owned packaged
sidecar source. Smoke's pending label is hard-coded and its scanner omits the
included NoviArenaRounds header; it does not prove the deployed method absent.
Actual Arena round setup runtime proof remains outstanding; do not invent an
additional missing-binding fix from that mock diagnostic alone.

ArenaCellDoorGuard2 constructor/early+idle exit resource cleanup PROMOTED OFFLINE.
30native anchors,6acquisitions,frame220/slot-200. Generic reviewed constructor
extends native stack-push proof (2pushes before vtable vs firstguard1) and EBX
zero reaching definitions at base cleanup. Native resource destructor7E74D0
clears resource fields+8/+12 and tails99A430; F1A793 wrong DestroyMovie repaired.
51focused (25new+26firstguard) and869broader/43subtests passed. Full29 final A/B
only Guard2 changed; first A/B rejected because baseline module saw the newly
opted-in Guard2 row. Corrected before-mode deep-copies unit metadata and removes
ONLY that new row; complete22Lua baseline identity verified. Complete23readable
identity and onlyGuard2 output delta. Source/body/helper gates preserved.
Actual Main2304draft cases: trueframe/query termination; normalized storage+
cleanup oracle, allresourcesclose. Readable2304 cases use frame-return termination
semantics; predicate-query traces normalized, allresourcesclose. No raw trace
equivalence or crossstyle rawclaim. Initial constanttrue/query-counter readable
fixture exposed different query timing: cleanup-bearing initial loop retains
native F19D0A/F19D24 separately; replaced with physical-frame predicate model.
NewOakValeIntro lifetime always-true frames still require separate package
validation; do not infer compatibility from these frame-return cases alone.
Smoke16files perstyle JSONidentical; existing Arena diagnostics unchanged.
Corpus371parse/88recovery128readability155review unchanged. Arena NOT packaged
or game tested; currentgamebundle v124, clean Assassin paid replay10 done and
561original saves unchanged. No game/owned constructor jobs running.
Next scratch codex_lua_arena_guard2_speech_20261001: existing reviewed speech
module replays native4Speak/12Task/5Clear calls with70anchors, no module changes
needed. Scratchgenerated Main has four correctly rendered lines but existing
cancel exits still TODO/early returns. Need actualconversation/cancellation
tests and native cleanup repair before promotion; no Guard2 speech unitrow yet.

ArenaCellDoorGuard2 speech + shared cancellation tail PROMOTED OFFLINE.
Native4Speak/12Task/5Clear calls (70anchors) recovered by existing reviewed
speech module with no changes. New reviewed cleanup-break module proves native
F1A7ED movie destructor -> F1A7F2 actor resource LEA -> F1A7F6 resource destructor
-> pops/frame restore/RET (9anchors,220frame/-200resource), full native/source/
post-speech lowered/helper hashes. Changes nested C cleanup break to explicit
goto resource epilogue, enabling existing generic goto lowering; no Lua hand edits.
Initial speech-only scratch384cases had18cancel movie+resource leaks. Prototype2
fixes all384; final actualgenerated Main6912cases PER STYLE, all4keys/native
speaker-listener/flags, dead speaker, early/cham repeat, pending clears and
termination1..48 with0/1/3task polls. Alltested movies/resources close. Readable
fixture initially retained per-entity fields between cases; fixed setupvalue
reset (no generated code changes) before final all4-key coverage. No raw baseline
trace equivalence or complete retail/Arena game proof claimed.
23cleanup native/adverse tests;892broad/43subtests plus22additional Guard2speech
tests passed. Tail-boundary adverse initially exposed missing exact-end gate;
fixed before final focused/broad checks. Full29 final A/B onlyGuard2; before-mode
removes ONLY new speech/cleanup opt-in rows, keeps promoted constructor rows.
Complete22draft/23readable identities verified; smoke16files perstyle identical,
existing Arena findings retained. Registered Guard2 has no TODO cancellation
exits; corpus371parse,87recovery/129readability/155review (6diagnostics removed).
Arena absent v124; no newbundle/in-gameclaim. Last live proof is clean v124
Assassin paid replay10; all561savesunchanged and gameclosed. Noownedjobsrunning.
Next existing ArenaCellExitGuard Main F1B9C0 resource construction/exit cleanup
evidence: readable has NewResource assigned to resource, Prepare/TryAcquire use
nil meControl. Gather native frame/slot and destructor witnesses before a fix.

ArenaCellExitGuard resource storage + both base cleanup phases PROMOTED OFFLINE.
Scratch codex_lua_arena_exit_guard_resources_20261001;25native anchors. Slot-300,
initialframe312; saved EBP F1BAD3 creates316phase. Generic constructor recovery
supports reviewed additional base destructors with CFG dominance of stack save,
matching immediate pop after cleanup, perphase ECX/depth/vtable/zero proofs.
Native ctorF1B9D5,earlybaseF1BABF,latebaseF1C9A3; two acquisition sites. New
24phase tests (including real non-dominating push EBP F1C937 rejection), existing
51constructor tests pass; full938broad/43subtests pass. Full29 A/B onlyExitGuard;
before-mode removes ONLY new opt-in row, retains promoted guard1/2 rows. Exact
22draft/23readable baseline and tested output identities verified.
Actualdraft Main512cases: strictbaseline rejects nilresource all512; candidate
closesall, receiver+basecleanup normalized oracle;280missingcleanup repairs.
Readable512physical-frame cases compare body calls to recovereddraft, both
bool-return and always-true frame modes. Allclose;28baseline extra-acquisition
body paths repaired. Normalize predicate queries and unchanged leading Hero
read; NO raw equivalence. Initial ordinal-query readable fixture rejected
because preserved native queries alter synthetic counter timing; replaced
with frame-based predicate and recorded separately (not hidden as equal).
Smoke16files/style onlyExitGuard.Main countdelta. FULL traces prove draft
1423->1424 adds finalRelease; readable1422->1424 adds native idle termination
query at16 and finalRelease. Initial equality smoke assertions intentionally
failed; reviewed delta/proof accepted. Existing findings remain unchanged.
Corpus371parse,87recovery/129readability/155review,temporaries6330->6328.
No Arena package/live claim; v124 remains gamebundle. Last gamecleanAssassin
paid10 complete;561original saves unchanged, gameclosed. Noownedjobsrunning.
Next existing ExitGuard dialogue/resource helper operand recovery: native
frames312/316 and varied hero-return variables; inspect remaining speech/task
receivers and movie cancellation diagnostics before selecting a guarded fix.


## User-requested shutdown and exact resume point

User explicitly requested shutdown. Resume Lua work only when asked. Preserve
the dirty feat/novi-script-recovery tree and the separate native lane; no commit
or reset. Detailed evidence: docs/journal/2026-10/LUA_RECOVERY_2026-10-01.md.

Latest promoted offline work: ArenaCellDoorGuard1 speech/task/clear receivers;
Guard2 constructors, speech and shared cancellation cleanup; ExitGuard resource
construction and both native base-destructor frame phases. Latest broad check:
938 tests plus 43 subtests passed. Full 29-unit comparisons isolated each change;
371 corpus Lua files parse. Status: 87 needs-recovery, 129 needs-readability,
155 human-review; these are not completion or in-game pass percentages.

Current game bundle remains v124; Arena changes are NOT packaged or game tested.
Clean live Assassin paid replay assassin_v124_paid_20261001_10 passed: exact
2000-gold payment, Gate3Open, control returned, five movies closed, no quest or
harness errors. All 561 original saves unchanged. Game is closed; no owned Lua
test jobs remain running. Preserve unrelated processes.

Exact resume point: work/codex_lua_arena_exit_guard_resources_20261001,
ArenaCellExitGuard.Main native 0x00f1b9c0. Constructor promotion is complete.
inspect_speech_next.py finds 10 Speak / 24 task / 2 Clear native calls, but
existing native_reviewed_resource_speech._recover rejects the witness with
"native resource speech witness mismatch". No ExitGuard speech opt-in row or
new speech fix has been registered. Diagnose the precise failed check first;
hero variable pCVar6 is currently hardcoded, while this function has varied
return variables, and native frames are 312/316. That is a hypothesis, not a
confirmed cause. prototype_lowered.c is the postconstructor input; lowered.c
is preconstructor. function.json/native_evidence.asm/snapshots retain evidence.
Two readable cancellation diagnostics remain. Recover only native-proven
operands/cleanup, regenerate scratch, run adverse and actual generated behavior
tests, full 29-unit comparisons and reviewed smoke before promotion. Never
hand-edit generated Lua or claim complete Arena/live equivalence from idle tests.

Arena InitialiseArenaRounds pending smoke label is hardcoded mock metadata;
v124 owned sidecar source already includes/registers NoviArenaRounds. Actual
Arena runtime proof remains outstanding; do not infer a missing deployed binding.

