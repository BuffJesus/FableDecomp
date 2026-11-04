# Lua recovery journal — 2026-10-10

User renewed “Proceed with lua work until asked to stop.” Active checkout:
`D:/Documents/FableTLC`, branch `feat/novi-script-recovery`, base
`366c08e64a4323a6147fe4d5caa675e31799d51e`.
Existing dirty recovery/native files are the inherited baseline; unrelated
changes and worktrees are preserved. No game launch or screen input authorized
by this continuation.

Resumed W98 V2 acceptance. Four scratch acceptance-script updates survived the
interruption. Reviewed seven-file gate includes six substantive Expressions
changes and TraderConflictGood leading indentation only. Its 4,344 tokens,
including strings/comments, match; both exact hashes are checked and the original
Trader file is snapshotted under `promotion_baseline/`.

All 2,245 pinned baseline hashes passed. A redundant copied-module raw hash
comparison failed solely because of line endings; decoded text matches and the
root exact hash remains checked against the accepted input manifest. Initial
post-promotion launcher had an unterminated string in its preexisting command;
corrected before running tests. Root regressions pass 100/100.

Root regeneration completed Expressions 14 files and Trader Conflict 16 files.
Serial file checks were slow; owned checks were interrupted and completed with
bounded parallel reads, reusing completed root XML and generated trees after
source hashes were verified. `finish_post_promotion.py` passes all 534 production
hashes, 527 unchanged and seven reviewed changes. All 30 regenerated outputs
match production and tested scratch. The ledger has 371 files, zero syntax
failures and changes only in six Expressions rows; cosmetic Trader row is equal.

Commands:

```powershell
python -u work/codex_lua_returned_callback_readability_v2_20261009/promote.py
python -u work/codex_lua_returned_callback_readability_v2_20261009/post_promotion.py
python -u work/codex_lua_returned_callback_readability_v2_20261009/finish_post_promotion.py
python -m tools.script_recovery.audit_readability --out docs/scripts/CURRENT_LUA_READABILITY_REVIEW.json
python -u work/codex_lua_returned_callback_readability_v2_20261009/prepare_package.py
python -u work/codex_lua_returned_callback_readability_v2_20261009/package.py
```

Packaging/acceptance results follow below when completed. W99 fishing completion
runtime retest and the adult_graduated null-pointer load crash remain unresolved.

## W98 V2 Returned resource callback readability accepted offline (2026-10-10)

Completion: work/codex_lua_returned_callback_readability_v2_20261009/completion.json.
The original W98 candidate was rejected before root writes because its whole-file
frame policy also changed TraderConflictGood. V2 scopes that policy to accepted
callback function positions across helper renaming; unrelated functions retain
the parent frame contract. The original rejection reports remain preserved.
Resumes deferred W93 callback readability with the accepted W97 scalar-store
guards. A sole returned WithRetailResources callback is analyzed as a temporary
named function and restored at the original call site; the temporary function
is never emitted. Ambiguous bindings, generated receiver/parameter names,
anonymous nested callbacks, environment changes and multiline text remain
preserved. Callback and later file passes retain explicit frame termination
queries across both bridge lifetime contracts, regardless of parent frame option.

63 focused/portable regressions and100 post-promotion root regressions pass.
3253 related tests plus543 subtests pass (3796 total,exit0). Actual generated
behavior matches33799 cases: Chest16262, Fish3342, Dig2592, Steal1395,
Follow Main4320, Wait5184 and FollowThread704. Chest normalizes only termination
query events. Follow/Wait originally flagged changed traceback line numbers;
original reports are retained, corrected checks normalize only source locations
in injected exception traces. Other events, hero lookups, state, frame/check
counts, suspension, ownership and cleanup compare exactly.

All29 readable stages regenerate from accepted copied/hash-verified native
drafts;534 Lua parse. Six Expressions readable files change substantively:
Dig/Fish/Follow/Steal/Wait/Global_OpenChest. TraderConflictGood changes leading
indentation only; all4344 non-whitespace tokens including strings/comments match
the accepted baseline. The remaining527 registered Lua stay byte-identical.
All14 root Expressions and16 Trader files reproduce tested scratch exactly. Native lowering
and metadata remain unchanged; prior native regeneration proof is reused,
not claimed freshly rerun. Readability inventory371/syntax0; counts:
{"genericTemporaries": 8424, "gotos": 4879, "machineTemporaries": 4717, "nativeNames": 6656, "unresolvedDiagnostics": 911}.

2293 frozen inputs; package190 Lua/195 manifest hashes and supported
executable preflight pass. Package differs from W97 in seven Lua files (six substantive, one cosmetic)
and README. Existing W96 bridge21b3bbb32bc54be575a277fd262f7dc6cb1fa721,
DLL SHA256 b872be106ffefdda28f0bb109f9d56a46c7b939cfd5dc43011c98353d27fcd63.
Bundle: work/codex_lua_returned_callback_readability_v2_20261009/returned-callback-readability-v2-candidate-20261009.

W98 itself has not been installed or launched. Earlier authorized W97 gameplay
passed saved-game load, locked chest, Jet reward, control return and empty chest;
fishing completion hung. W99 preserves empty completion events in a separate
scratch bridge; original native bytes and exact old-failure/current-pass checks
prove the contract fix, but its gameplay retest stopped when the user returned.
Both test sessions closed normally and all563 original save hashes were restored.
W98 readability does not fix that bridge failure. User home: no further visual
launches or input. Continue offline until stopped; fixed bridge gameplay remains
the first task at the next authorized away window with fresh process guards.

## W100: adult_graduated saved-text schema diagnosis (offline)

Read-only command: python work/codex_lua_gameflow_save_schema_20261010/inspect_save.py.
Result: offline_saved_text_schema_mismatch_identified. Evidence.json records
four exact original save hashes, checked container CRC/lengths, LZO QUESTS text,
exception-context registers/stack assertions and bounded original retail bytes.
AutoSave Gameflow persists PostSavePosition150/CoreQuestWaitingTRUE; AutoSave.qs
persists100/FALSE. AutoSave.qs.hs has no QUESTS section. Profile.bin unchanged.

Retail Gameflow OnPersist atCEF921 calls4106F0 with CoreQuestWaiting literal and
object+48. Exception context (not the handler thread context) has EIP9E5C63,
EAX2/Ecx0/Esi0, PSymbolMap0, with return9E25AF,4107A1,CEF926 and destination
Gameflow+48. Original9E2590 compares PeekNextItemType with2, loads context+4,
then calls Sort9E5C60, which reads null+11. Fault parser.StringPos41 is recorded;
its heap buffer5386FC8 is absent from the minidump. Exact active checkpoint and
writer are therefore not proven. No sidecar-independence claim.

This explains the incompatible boolean-text schema, corroborating the September
bool-loader/retail-born-uint-loader probes. The September journal's one-byte
binary record-overrun explanation is not supported by this crash: mode is text
and tokens are TRUE/FALSE. Do not change faithful PersistTransferUInt to Bool.
No automatic migration, generated Lua change, save write, install or game input.
Next: native-instruction regression and existing compatibility diagnostics.

Native regression now passes4 cases: numeric0/1 read successfully; TRUE/FALSE
each fault exactly at9E5C63 reading0x11. Command:
python work/codex_lua_gameflow_save_schema_20261010/native_schema_regression.py.
It reuses the existing pinned SLONG oracle string/CRT/error boundaries, replaces
only its mocked Sort with original execution and sets PSymbolMap0. Tokenization
and the faulting leaf execute original bytes; full Gameflow/UINT dispatch does
not. native_schema_regression.json records source/function/executable hashes.
Existing retail bool-input oracle separately rerun:12 cases pass. This supports
the typed-text explanation without claiming a full gameplay or save-repair test.

W100 completion.json accepted offline with22 pinned inputs. Scratch-only
audit_saved_schema.py conservatively checks the exact Gameflow SCRIPT_DATA
field in explicit saves.27 tests pass (including actual originals read-only,
CRLF/LF, duplicates, symbolic/expression tokens, oversized input and invalid
container); schema_tests.xml retained. CLI rejects AutoSave and AutoSave.qs as
incompatible and AutoSave.qs.hs as unknown (no QUESTS text), expected exit1;
saved_schema_audit.json retained. Numeric schema is not whole-save proof.
The first diagnostic version missed CRLF field endings; focused tests caught
that and the corrected anchored pattern passes all27. Not promoted to launcher.

Inspection of novi_local_test.py confirms payload preflight does not check
saved field schemas. Future W99 gameplay must use its known-good disposable
trader save, not adult_graduated; the old unresolved save-crash paragraph in
W99 next_runtime_gate.md is superseded by this evidence. No migration authorized.
Resume existing readability work from accepted W98; no game/screen access.

## W101: callback helper lexical boundary hardening (scratch)

Owned work/codex_lua_callback_lexical_boundaries_20261010 snapshots the accepted
callback helper/tests. probe.py proves direct rewrite treats function-shaped
text inside a Lua long string as a real wrapper and changes its runtime string
value. baseline_failure.json retained. The full readable_file entry already
refuses multiline text before calling this helper; this is latent helper safety,
not evidence current production Lua contains the corruption.

Candidate masks string/comment tokens while retaining byte/newline positions
before detecting function boundaries. It does not broaden the sole-wrapper
contract.36 lexical tests cover long strings/comments with0/1/2/4equals, genuine
wrappers before/after, escaped-newline short strings and no-function input.
Accepted baseline fails32/passes4 of these same tests (baseline_focused.xml).
Candidate36 plus existing63 callbacks pass99 (focused.xml). Related9modules
pass498 plus13subtests (related.xml). Initial tests wrongly expected a genuine
wrapper before trailing outer text to be accepted; existing contract refuses
that case, so assertions corrected, with initial XML retained.

All29 readable stages regenerate in owned corpus/ with two bounded subprocesses,
using copied/hash-checked accepted W98 native drafts.2296inputs verified/frozen.
Session88910 owns the corpus supervisor; no shared generator edited. Finish
534Lua comparison/input reverification before any promotion. Native lowering,
DLL/package/install/saves/game input untouched. W98 remains accepted production.

Eight additional existing-entrypoint guard checks pass (entrypoint_guards.xml),
confirming the root readable_file already preserves multiline strings/comments.
Related suite supervisor15198 completed; corpus88910 still active.

Supplemental W100 known_good_save_check.json confirms the pre-existing disposable
codex_trader_integer_repair_20261005 AutoSave has CoreQuestWaiting0 and SHA35ec0409,
matching W99 archived AutoSave.qs. No new staging/load claim. October5 journal
lines1079-1087 independently captured FALSE at parser.StringPos41 in a prior
trader-save crash, decoded its LZO QUESTS field, and verified its single-field
numeric repair loaded in normal retail and candidate gameplay. That was a
different save/input and earlier authorized test; do not claim a new repair or
adult_graduated writer provenance. Current saves remain untouched.

W101 accepted offline: all29 stages completed terminal0,534Lua byte-identical;
2296 frozen inputs reverified. review.py verifies534 production Lua hashes and
existing named W98 bundle195manifest hashes/190Lua syntax. Promoted only helper
and new36-case test_readable_callback_lexical_boundaries.py using apply_patch.
Root107 tests pass (36new+63existing callbacks+8entrypoint guards). complete.py
reverified inputs/534production hashes; completion.json pins2319inputs. No
native regeneration, emitted Lua change, sidecar/DLL/package/install/save write
or game input. W98 ledger counters and named payload reused without duplicate
regeneration/audit/package. git diff --check scoped paths passes.

Next observed existing-recovery lead: Fish draft contains native_fishingAtSpot1
on its near-spot branch, while current readable Main appears to retain only0.
The reviewed metadata already pins native byte write EE9AAD. Determine actual
behavior and pass responsible; do not guess a generated Lua edit or conflate
this with W99 empty completion event ABI fix.

That apparent Fish flag lead is rejected: focused Select-String proves readable
line104 contains native_fishingAtSpot1 and line102 preserves100.0 comparison.
The earlier long excerpt was truncated at this region; do not infer missing code
from truncated output. Existing complete-fishing modes tests run session79378
for verification only (two2412-case draft/readable harnesses), no fix planned.
Its supplemental XML fish_existing_modes.xml is outside W101 acceptance pins.

Fish verification completed terminal0:2tests pass/17deselected in132.70s,
2412scenarios per draft/readable style,4824 total. Includes near/far fishing
mode, success/failure rewards and cleanup, termination/waits/null-info cases.
These are existing mock-engine harnesses, not new gameplay or W99 DLL proof.
No fishing generator/evidence/output change. False flag lead closed.

## W102: negative literal grouping (scratch)

Owned work/codex_lua_negative_literal_precedence_20261010 snapshots accepted
readable_style. A single-use negative staging probe is protected by earlier
full-pipeline inlining, but branch-local negatives with multiple reads reproduce
wrong exponent sign in propagate_literals, style_source and readable_file.
baseline_single_use_probe.json and baseline_probe.json retained. -2^2 differs
from(-2)^2; adjacent subtraction can become a Lua -- comment. No current emitted
exponent site was found by scoped search; do not claim a gameplay bug.

Candidate groups negative literal replacements, leaving other literals as-is.
118focused tests pass at direct/style/full-pipeline stages with both branches,
integer/fractional/negativezero values, power/unary/subtraction/concatenation,
floor/bitwise operations, return arity, comments/literals, and preserved errors.
Accepted baseline49fail/69pass on identical tests. Initial12skips were replaced
with explicit adverse error checks; three exact local-name diagnostic differences
normalize only the known Lua integer-representation wording. Original XML kept.
Related10modules running on scratch candidate. Shared style/generator/Lua/DLL
remain unchanged; no corpus/promotion/package/ledger/game/save write yet.

Related10modules completed terminal0:647tests+13subtests pass in10.90s. Focused
118 rerun passes with explicit exception-class comparisons as well as the narrow
message normalization. All29 isolated readable stages now running session53772;
2322 inputs verified/frozen from accepted W101 and the negative-literal candidate.
Native drafts copied/hash-checked. Final all-unit differences are not presumed
empty; review each and prove actual behavior before promotion. Shared style and
all production Lua remain unchanged while workers run.

Extended9modules completed terminal0:1158 tests pass in14.00s, covering closure
boundaries/constants, compound stores, conditional joins/evaluation, literal
return jumps, implicit cancellation and Book/Trader conditions. No child
interpreter overlay needed: scoped search finds no subprocess usage in these
modules. Corpus first Arena/Bandit outputs byte-identical; other units pending.

Eight additional integer-bound/returned-resource-callback checks pass in0.33s,
including INT32/INT64 minima, fractional and negativezero staging. boundaries.xml
retained. Corpus27units now byte-identical; GuildTraining/WhiteBalverine pending.
Prepared review.py accepts only zero emitted differences; if any appear it must
stop promotion for explicit review/behavior comparison. No shared edits yet.

W102 accepted offline:29stages terminal0,534Lua unchanged,2322frozen inputs
reverified. review.py checks534production Lua andW98bundle195manifest hashes,
190Lua syntax. Promoted only readable_style negative replacement grouping and
test_readable_negative_literal_grouping.py (118main+8bounds/callback cases),
with apply_patch. Root225 tests pass (126new+63existing callback+36lexical).
Complete.py first referenced the copied W101 baseline_failure filename; corrected
to W102 baseline_probe/single-use witnesses, without rerunning completed tests or
generation. Final complete.py terminal0,2349inputs pinned and534production hashes
reverified. Root test ASTs exactly match tested scratch functions. Scoped git
diff --check passes (only CRLF normalization warning).

No emitted Lua, native lowering, metadata, sidecar/DLL, installed file or save
changed. Existing named W98 payload and ledger reused after byte/hash verification.
The regression proves full-pipeline Lua semantics, not a new in-game/native bug;
no exponent site found in current registered artifacts. W99 runtime gate remains.

## W103 Book definition-storage classification (no production change)

Owned scratch work/codex_lua_book_definition_storage_20261010/audit.py reads
the original retail executable and asserts its known SHA plus12 exact Init
instruction sites. E54990 uses global+4ec count, default string temporary,
quest+94 storage fill, global+4d4/index*4 string assignment, then separate
quest+ac/+a0 byte resets. evidence.json pins9 source/generated inputs and the
bounded disassembly. First audit attempt used top-level definitionSnapshots;
corrected to the actual quest child and reran successfully. No source hypothesis
or production code was changed to make the audit pass.

python -m pytest tools/script_recovery/test_book_definition_snapshots.py
tools/script_recovery/test_book_flag_storage.py -q --junitxml=work/codex_lua_book_definition_storage_20261010/tests.xml
completed session92621,exit0:38pass in28.17s. Existing generated tests exercise
definition lookup/removal operands and flag initialization/persistence dispatch;
these are mock-engine checks, not actual donation/save/gameplay proof.

The initial missing-definition-read lead is closed: consumers already use the
evidenced immutable global snapshot. Unresolved Init copy-storage and teacher
pointer bookkeeping remain explicit because allocation/refcount/exception
equivalence has not been established. No generator, metadata, emitted Lua,
sidecar, save or installed payload changes; no screen/game use. Full regeneration
would add no evidence for this no-change audit, so not repeated. Continue the
existing registered recovery lane and keep W99 runtime gate distinct.

## W104 Book21 master-string branch (no correction)

work/codex_lua_book21_string_branch_20261010/check.py executes original bytes
E57324..E573A9 and CBasicString comparator411570..411597 in Unicorn. No call
is mocked within this bounded branch; both original literals are checked.
Null representation follows original empty-vs-A repe cmpsb and reachesE57345/B;
non-null strings call original comparator and chooseA only for exactA.
13native cases (both null/non-null empty, ASCII boundaries and validUTF8) match
26 actual root draft/readable helper results.48 generated early termination
cases preserve prefix writes and avoid ARG3. Lua resource/state APIs remain
mocked; rest of native helper, allocation/destruction and gameplay are not proven.

Command python -m work.codex_lua_book21_string_branch_20261010.check exits0,
results.json pins executable/branch/comparator/source hashes. First run passed
assertions but report writing failed on defaultWindows cp1252 for the Unicode
fixture; made scratch report encoding explicitUTF8 and reran. Numeric-zero
comparison was suspicious presentation, not a demonstrated behavior defect.
EmbeddedNUL, invalidUTF8 and non-string API returns excluded; no new contract
invented for them. No generator/emittedLua/sidecar/save/game changes, no full
corpus rerun warranted. Continue authorized existing offline recovery.

## W105 BordelloLady cleanup calls - scratch V3, corpus active

Existing registered bordello/V_Bordello/CBordelloLady MainE3EB10 has missing
unpause calls atE3EEF0,E3F731,E403BF and a wrong float-derived flag atE403A4.
Native slot5ec dispatches interface target890AB0; three immediate arguments are
zero, alternate E403AF->E3F731 and E4039F->E403A4 pushEDI. Retail prologue,
GSIowner+4 load, saved balancedESP+14 slot and native setups are byte-pinned.
Owned scratch work/codex_lua_bordello_cleanup_calls_20261010/pin_baseline.py
snapshots26 inputs, exact executable SHA and bounded original disassembly.
Original helper named inspect.py shadowed Python inspect; moved it to
pin_baseline.py before oracle imports. Oracle initially mapped only first vtable
page; corrected explicit adjacent page for table+5ec, without changing evidence.

oracle.py/native_cleanup_paths.json:20 original tail paths execute and unpause
exactly once before Movie destructor boundary. GSI call is doubled with ret4;
EDIzero is supplied, so this oracle alone is not argument provenance. Separate
register_provenance.json conservatively analyzes both branch outcomes, treats
unsupported writes as unknown, and proves EDIzero at2 register pushes and EBX
interface provenance at4 sites under Win32 nonvolatile-callee ABI. Called engine
bodies, stack aliases beyond byte-reviewed save/reload, exceptions and gameplay
are not proven. Existing binding manifest has exactly one bool parameter.

Baseline capture first used nonexistent module-level convert API; inspected
UnitConverter.convert/write_reports and corrected owned script. Completed
session23239 baseline regeneration:10 readable files parse. Converter.native
already restores dialogue literals, so its source SHA differs from raw export;
candidate row intentionally pins that reviewed phase SHA as well as original
export/native evidence retained separately. No source hash bypass or native edit.

Scratch generic reviewed_lifted_interface_calls.py validates native body/source/
final-Lua phase hashes, interface table/target/callee purge/binding, complete call
predecessors, native receiver setups, register provenance and diagnostic counts.
Only exact evidenced statements are recovered. Primitive local numeric-flag
spelling is required when replacing an existing wrong flag; no arbitrary RHS
effects are removed. All other diagnostics remain explicit.

V1 focused26pass/1fail:duplicate predecessor metadata was not rejected. Retained
moduleV1 and focused_v1.xml; added guard after first generator session47497
completed. V2 focused27pass,10files parse,three TODO calls recovered. Actual
Main scenario owned=true/answerNO/POLLY/termination boundary3 still paused(true)
at cleanup because reused fVar18 health1. behavior_v2_failure.json and V2 module/
review/body retained; three-call candidate is not accepted. Added fourth native
call recovery to V3. Initial behavior harness incorrectly changed termination
at each poll (violates existing stable-between-blocking-calls contract); corrected
to explicit acquisition/speech/move/animation/macro/frame boundaries. Member
resource3 is a valid PrepareResource operand alongside local resource1; strict
mock corrected after inspecting actual callers, not suppressing an engine fault.

V3 session45157 native/readable regeneration complete exit0:10Lua parse.27focused
tests pass in2.47s, including rewitnessed native instruction mutations rejected
by semantic guards even after recomputed hashes. Whole actual Init/Main traces
pass6048 scenarios/24192 executions;690 cleanup traces corrected. Covers owned/
unowned, three names, health0/1, acquisition success/waits, speech cancellation,
crime exclusion, unanswered/no/yes offers and21 termination boundaries. All
other modeled actions, frames and blocking boundaries match; candidate pause
sequence is exactly true,false per movie. No hero-hit or native exception proof.
Existing30Bordello/Madame modules pass226 tests in21.37s,session90817 exit0.

All29 native/readable corpus session77242 now running on2364 frozen inputs.
Current task V3 Bordello regeneration reused after completion; other28 units
regenerate both stages against accepted W102 corpus. Shared root sources and
production artifacts remain unchanged. Do not edit frozen inputs while workers
run. Next: inspect all diffs/reports, smoke JSON, portable focused fixtures and
final phase integration before scoped promotion/package/ledger. No installed
payload, sidecar, save, screen or game changes. W99 runtime remains separate.

W105 smoke reports inspected, not accepted from exit0 alone. Unprofiled package
smoke reports4 problems: old-source missing method registrations, two static
globals, and a helper supplied a table where native signature requiresCString.
--conversion-report removes that wrong helper argument (native recorded type),
but older default bridge profile still lacks SetCutsceneSkippableWhilePaused.
--sidecar-source-dir pointing at accepted W96/21b3bbb actual sources leaves only
two static g_PresentedItemName warnings. Baseline and candidate current-profile
JSON are exactly equal:9files/42functions,no execution/pending/unknown errors.
review_smoke.py/smoke_review.json pins both reports and bridge sources/DLL.
Registered LuaManager MsgIsPresentedWithItem delegates to LuaEntityAPI, whose
actual body writes g_PresentedItemName in the caller VM after a delivered string;
those static warnings remain visible, classified as an intentional bridge global.
Actual PauseAllNonScriptedEntities bool binding forwards the value unchanged to
native GSI. Relevant bridge files git-clean at21b3bbb; DLL SHA b872be10 matches
accepted W96. No bridge change or actual event-delivery/gameplay proof claimed.

W105 completed scoped offline acceptance. All29 native/readable corpus finished:
534 Lua parse,only BordelloLady draft/readable change,532 unchanged. Extended
tricking/nunnery/helper-boundary models add6656 executions to24192,30848 total
(scenario sets overlap; no claim of unique combined scenarios).27 focused and
226 related scratch checks pass. Generic recovery integrated after the final
resource-lifetime phase; portable hash-pinned phase fixture and27 tests added.
Root focused/readability regressions252 and related cleanup54 pass. Integrated
20 Bordello Lua files exactly match the tested V3 scratch. Only new reviewed
interface metadata was added to the unit; other baseline fields match exactly.
Conversion report records four recovered native call sites separately from the
older lifter inventory, which is not a native execution trace.

Promoted exactly two generated Lua files and generated reports, after hash
checks; no hand-edited generated Lua. Readable report draft path rebased only.
Readability audit371 files/zero syntax errors:only BordelloLady row changes,
unresolved17 to14,total911 to908. finalize.py verified prior2364 pins,allowing
only reviewed converter/unit changes; integrated sources/output/report hashes
match. Fresh offline bundle contains prior13 units plus already-registered
Bordello and Oakvale;199 Lua parse,204 manifest hashes verified. W96 bridge
21b3bbb/DLL b872be10 unchanged; W99 scratch fishing DLL deliberately absent.
accepted_inputs.json seals2401 current inputs; completion/package validation
record offline-only limits and remaining warnings. Do not rerun finalization.
No install/launch/input/save changes. Continue existing Lua evidence offline;
fresh explicit away window required for pending W99 runtime verification.

W105 final read-only verify_acceptance.py reverified2401 inputs, all534 current
production Lua (exactly two BordelloLady changes),204 bundle hashes and supported
retail executable preflight. No game launched or installed files changed.

W106 BordelloLady temporary-mask audit, no production change. audit.py pins four
current inputs and original executable41dc9109, inventories seven mask windows.
Original hit predicate E400EB..E401D3 executed48 cases (all three boolean message
results,two unrelated-flag baselines,three stack/object variants). CString
constructor99EBF0/destructor99EAE0 and engine message boundaries doubled with
explicit receivers,args,purges. Bits2/4/8 at balanced stack+38 gate the three
temporary SCRIPT_NAME_HERO strings; destruction is exactly reverse construction,
all temporary bits clear, unrelated bit10 survives, stack balances. Independent
hit byte at stack+13 matches hit OR(special AND NOT heal), with native short
circuiting. This is not allocator, exception/unwind or actual event proof.
15 existing hit/resource-lifetime regressions pass in2.19s. Keep remaining
temporary-mask diagnostics rather than treating them as persisted script state.
No generator/output/sidecar/package/install/save/game changes for W106.

W107 scratch terminal BordelloLady local retirement. reproduce.py executes six
actual-generated cases: initial-frame refusal creates no local, while setup
and poll termination retain one until modeled quest-scope Close (four cases).
Initial mock incorrectly allowed NewScriptFrame false while termination false;
fixed to change termination only at real setup/frame boundaries and keep the
return consistent. Current bridge NoviUnitData resources are quest-wide; entity
return does not Close that scope. Original E40310 inline local retirement and
7E74D0/99A430 compare equal in12 null/shared/last-owner/info-handle states; normal
instructions execute, imp callbacks and game free doubled. No heap/exception
or actual control-delivery proof.

V1 polling-only scratch parses/regenerates but strict model catches a baseline
defect: acquisition interruption emits DestroyMovie twice and never releases
the local. Both baseline andV1 draft/readable have it; earlier W105 pause model
did not assert movie/local lifetimes. V1 artifacts preserved separately and
v1_behavior_failure.json records the exact case. Original E40364 split movie
counted pointer6E7AB0,base99A430,and local7E74D0 execute identically to full movie
6E7B80 then local destructor in36 bounded comparisons. V2 adds one polling
release and replaces the second DestroyMovie with local ReleaseResource at two
exact reviewed exits. Fresh baseline20 Lua matches current production exactly.
Native/source/phase hashes, receivers,branch/call instructions and destructor
certificates pin recovery.28 focused checks pass in1.09s; direct all-parameter
runner also passes28 (not substituted for the completed pytest result).

Root-directory pytest collection stalled on three owned attempts (default,
diagnostic,plain/importlib); stopped only exact owned command lines. Running
the task-local file with --rootdir=. avoids parent work-tree collection and
completes normally. Initial related batch252 passes/one failure retained: the
late-phase monkeypatch was imported as the isolated interface restore helper,
so that test incorrectly observed both recoveries. RelatedV2 runs the30 Bordello
modules under the patch;27 interface tests run separately without it. This is a
harness correction, not relabeling a production assertion as a pass.

V2 strict model6048 scenarios/24192 executions pass;4746 correct retained locals.
Create/release state forbids duplicate local release; candidate movie destruction
also strictly requires a live movie. All other modeled calls/frames/boundaries
match, aside from the independently witnessed retirement corrections. Full29
native/readable corpus session4135 frozen2425 inputs,relatedV2 session36253 and
extended/isolated session91462 running. No frozen source edits while running.
No root promotion/package/install/launch/input/save change. W105 remains accepted
offline but is not a full Bordello lifetime/gameplay-equivalence claim.

W107 corpus completed29 units/534 parsed Lua,exactly two Lady changes versus
fresh acceptedW105 corpus;2425 inputs reverified. New repeated conversations
exercise up to three distinct movie IDs in one Main:252 scenarios/1008 executions
pass,192 correct retained locals. Extended nunnery/tricking/hit model1664 scenarios/
6656 executions pass,708 corrected; all models31856 executions (overlapping
scenario sets). RelatedV2 passes226 in24.16s; unpatched interface27 passes2.81s.
Typed actualW96-profile smoke9files/42functions has no errors/pending/unknown
methods. Not byte-identical JSON: Lady Main calls1210 to1211,exactly the additional
polling release; two existing bridge-written globals unchanged. Actual bridge
binds MovieResource_Destroy_API6E7B80 andCSGROSTB_Destroy_API7E74D0; scope teardown
only at quest host destruction. register_provenance proves EDI0 at six original
cleanup comparison/store sites,under Win32 nonvolatile-register contract.

First review incorrectly required fresh all-unit scratch layout to equal every
promoted path. GuildTraining's long-standing partial readable layout has13
differing and25 missing fresh paths. Root actual534 baseline remains W102 plus
the reviewed W105 Bordello hashes; separately verified exactly. Review retains
and inventories that pre-existing layout distinction,does not promote or remove
any Guild file. Fresh scratch534 compared only with previous fresh534; both
layers show no unexplained W107 change. review.json now ready for integration.

Integrated generic native_reviewed_terminal_resource_cleanup after W105's final
interface phase,new reviewedTerminalResourceCleanup metadata only,portable phase
fixture/testdata and28 focused tests (no ignored-work dependency). Snapshot
root_before records existing dirty converter/unit/ledger and HEAD366c08e64a43.
Root focused/readability280 passes8.02s; related54 passes3.13s. Integrated native
regeneration75500 finishes42function/9file syntax checks,48 unresolved unit
diagnostics unchanged. Readable regeneration currently running before exact20
file hash comparison and output promotion. No production Lua/ledger/new package
promotion yet,no bridge/install/launch/input/save change. W105 remains latest
accepted bundle; its known Bordello lifetime defects are not gameplay eligible.

W107 scoped offline acceptance complete. Integrated readable finishes12194;
check_integration.py confirms all20 native/readable files exact against V2,
metadata adds only the reviewed terminal row,helper/fixture match decoded tested
sources. Promoted only two generated Lady files plus native/readable reports;
readable draft path rebased.534 production hashes checked before promotion.
Full audit371 files/zero syntax failures,summary unchanged908 unresolved;
only Lady JSON finding line numbers/line count change. Markdown ledger bytes
remain unchanged because its columns contain unchanged counts,not line numbers.

Fresh package derivation initially used an erroneous extra FSE path component;
the incomplete copy was preserved with a validated workspace-contained rename
to bordello-terminal-cleanup-incomplete-v1-20261010,no deletion/install/launch.
Actual assembler payload is NoviCompatibility/V_Bordello/Entities/... . Corrected
fresh bundle uses that verified path. A second fail-closed gate correctly found
only JSON ledger changed (Markdown identical); gate now permits only the
reviewed converter/unit and exact before/after ledger states,not arbitrary pins.
Finalization completes:199 package Lua parse,204 hashes verified,all14 normal
readable units plus Oakvale compare with staged sources. Only Lady Lua andREADME
change versus W105. Retail executable preflight passes without launch/install.
W96 bridge21b3bbb/DLL b872be10 unchanged; W99 fishing scratch DLL not included.

accepted_inputs.json seals2989 current inputs,including all534 production Lua,
root sources/metadata/fixtures/reports/ledger,tests and evidence. Separate final
verify_acceptance.py rechecks the seal,payload and exact two production changes,
and repeats read-only preflight. completion.json records three corrected source
exits,280 root regressions/54 related,28 focused/226 related/27 isolated checks,
31856 model executions and48 native destructor comparisons. No gameplay,
exception/unwind,actual event delivery or new DLL proof claimed. Two static
bridge-written globals and14 unresolved Lady diagnostics remain. No installed
file/input/save/game changes. Continue existing registered Lua evidence offline;
fresh explicit away window required for W99 runtime gate. Do not rerun promoters.

W108 continuation: prior goal turn is substantive progress (W105/W107 integrated
and validated),goal remains active. Fresh checkout/worktree/skill/handoff review;
branch feat/novi-script-recovery,HEAD366c08e. Native lane and other worktrees
preserved. W1072989 input/payload/534 production hashes and read-only preflight
reverified. Latest user steering continues Lua despite older native exclusion.

Existing registered V_Bordello/Madame Main E3BB70..E3E315 selected for receiver
and interruption evidence. Actual draft/readable starts movie5 but destroys
movie4(nil) in the beer branch. reproduce.py runs Init,OnPersist(mock true to
select previously-introduced state),Main under explicit strict resource/movie/
timer contracts. Eight cases cover health0/1 and speech success/cancellation;
all fail WRONG_MOVIE_HANDLE:nil,with no unknown-method fallback. No persistence
or game execution. Cancellation additionally prints stale timer/movie operands,
but first fault prevents treating those as independently executed yet.

pin_native.py snapshots seven current dirty inputs,records HEAD/executable41dc9109
and exact full native body hash,eight call-site windows. Retail beer start E3BF86
pushes movie pointer at ESP+B0; normal counted-pointer6E7AB0 E3C09D uses+B8,
base99A430 E3C0B4 uses+B0. Interrupted task branch E3C032 targetsE3D078:
unpause,6E7B80(+B0),temporary CString(+14),DeregisterTimer globalGSI slot160
with timer(+18),7E74D0 local(+1C). Shared cleanup entry E3E2EA calls6E7B80,
but jumps into that call can carry an already prepared ECX,skipping its +D0
fallthrough lea. Exported depth/stack annotations are hints,not sufficient for
alias recovery. Next capture final Lua phase and derive a generic receiver
correction with shared-tail proof,negative cases and scratch validation.
No converter/output/sidecar/package/install/launch/input/save changes forW108.
W107 remains latest scoped offline bundle,not whole-Bordello gameplay proof.

W108 failed-speech mock refined to set termination at that boundary,with
NewScriptFrame return consistent with the state (no per-query toggles). Eight
cases still reproduce. capture.py launched as owned session4047; final phase
hook only observes Madame source,then fresh native/readable baseline20-file
comparison. It reads current root sources without patching production. Re-poll
same session until authoritative terminal; no candidate or promotion yet.
Final phase file now captured and inspected: LAB_E3BF28 span constructsxStack_dc
but two retirements usexStack_cc. Its interruption path deregistersxStack_160
and destroysxStack_160 (movie/local confusion); real timeraliasi_stk_1a8 is
assigned immediately after RegisterTimer. Shared tail retirespCVar10,then correct
timer/local, but beer post-task termination never selects that movie operand.
Use bounded native-anchored span edits/receiver selection,never global temporary
renaming. Later emitter adds speech cancellation guards from the cleanup plan;
fix its phase input and verify all emitted paths rather than editing generated Lua.

W108 capture session4047 reached authoritative exit0. Both fresh Bordello stages
match all20 current production Lua; baseline_regeneration.json pins each hash.
No production files changed. Original post-speech E3D0C0 instructions explicitly
prepare ECX=ESP+B0 before jumping directly to E3E2EA; the unrelated fallthrough
ESP+D0 instruction is skipped. Original CD4470 reads the timer from its receiver
and dispatches globalGSI slot160 with one argument (ret4 binding boundary).
native_shared_cleanup.py executes original interrupted and post-speech paths
for timer0/44/FFFFFFFF:6 pass, same ordered unpause/movie/CString/timer/local,
balanced stack. Those callees use explicit ABI doubles, except original timer
helper executes; this is receiver/order proof, not destructor or gameplay proof.
native_normal_cleanup.py executes original E3C096..E3C0B9 split destruction and
original6E7B80 counterpart with6E7AB0/99A430 bodies. All12 null/shared/last-owner
and info-state comparisons match object effects/events; imp callback and free
are doubles. Reports native_shared_cleanup_results.json and
native_normal_cleanup_results.json preserve boundaries and results. Next generic
native/hash-guarded bounded beer cleanup receiver recovery, adverse tests and
scratch pipeline. No game/install/input/save/production changes in this increment.

W108 scratch-only bounded operand restorer and reviewed JSON prepared. Native
witnesses include start,normal/interrupted/shared cleanup,timer helper and
destructor bodies; full native/source/phase hashes fail closed. Four replacement
rows cover two movie handles,one timer,one local-resource operation and shared
selector. No global temporary renaming or generated-Lua editing. Generic
restorer remains in owned work folder until acceptance. Ten focused tests
include changed native/exported/phase hashes,witnesses and resealed-but-ambiguous
label/count/operand/selector inputs. Pytest task cwd --rootdir=. session33704
currently live,no output. Scratch both-stage generation session52863 currently
live,no output. Poll owned handles,do not restart due observation timeout.
Pending actual generated execution and emitted cancellation cleanup inspection,
broader tests/all-unit comparison before production integration or packaging.
Subsequent observation:focused session33704 terminal exit0,10 passed54.73s.
Generation52863 remains live with no output; continue polling the same handle.

W108 follow-up is progress plus verified wait:session52863 terminal exit0.
Both stages complete,10 readable Lua parse,only Madame2 files change;18 other
Bordello Lua unchanged. candidate_generation.json records comparison. Strict
reproduce.py accepts explicit tree/stages/expect/report and speech timing mode.
Baseline8 still reproduce after adding exact retirement checks. Candidate8
immediate,8 loop-interrupted and8 post-task-terminated executions pass. Each
asserts ordered pause true/false,movie22,timer44,local11; all retired once.
Beer consumed only with health0 or successful immediate speech. Normal path
termination is modeled at next frame after movie destruction,not invented
mid-query state. Reports candidate_model.json,candidate_loop_model.json and
candidate_post_model.json retain hashes/traces. Draft-only4 also passed before
readable completed. Initial relative --tree failed path provenance check before
Lua; resolved tree argument and reran. No suppressed Lua faults. Generated
failed-speech guard now destroys movie5,deregisters timerId2,releases local.
Still no root converter/metadata/generated production changes,no launch/save/
install/input/sidecar/package changes. No liveW108 session remains. Pending
broader adverse handle/termination scenarios,related tests,all29 both-stage
corpus and integration/reporting/packaging gates;offline mocks are not gameplay.

W108 continuation progress:expanded strict generated models72 pass (nine runs
of8) across distinct nonzero resource/movie identities and timer0/44/FFFFFFFF,
immediate outcomes/loop interruptions/post-task termination. The prior24 overlap;
do not characterize total as96 distinct paths. Reports pin generated Lua and
show exact retirement/consume assertions. Expanded_models session58675 exit0.
Typed scratch readable smoke with W96 actual sidecar source registrations exits0
with2 warnings. review_smoke.py inspected all42 callback reports:zero errors,
unknown or pending methods;only expected Madame/Magicman g_PresentedItemName.
candidate_smoke_review.json pins raw JSON. Initial --help printed Unicode arrow
under cp1252 and failed; reran -X utf8 successfully,not a script/game failure.
Full29 native/readable corpus launched owned session41473;3018 hashes frozen,
including W107 accepted2989 baseline and recovery/candidate witnesses. First
bandit_camp result unchanged;process confirmed live at latest poll. Related
Madame/movie/resource regressions run via scratch final-phase hook session21850,
still live and printing passing dots. Poll same handles,no restart due silence.
Neither result is complete validation yet. Preserve frozen inputs while running.
NoW108 root integration/promotion/package/game/install/save/input mutations.

W108 follow-up progress and verified wait:related21850 terminal exit0,154 pass
117.11s. XML inspected154 tests,zero failures/errors/skips;related result JSON
complete. Exact draft/readable git diffs reviewed. Git diff session53692 exit1
means expected differences,not an error. Added independent review_generation.py:
all20 scratch files parse and the full actual diff equals only expected bounded
cleanup operands/selector,including emitted false-speech guard. Four generated
beer cleanup exits corrected;18 unrelated files unchanged. Generated diff review
pins each output. No control/yield/state/speech/consume changes beyond reviewed
cleanup selection. Full corpus41473 confirmed live;arena,bandit_camp,
beardy_baldy,beggar_and_child completed unchanged. Keep original handle and
frozen inputs;no integration/promotion until all29 completion and seal recheck.

W108 next continuation progress:3018 frozen corpus inputs rehashed,zero mismatch.
Owned41473 live throughout;14 units now complete,only expected Madame draft/
readable2 differ. Extend unpinned strict behavioral harness with2/3 normal beer
rounds;each StartMovie returns a fresh handle within the same Main invocation.
repeated_models_results.json:144 executions pass across immediate/loop/post
timing and timer0/44/FFFFFFFF. Interrupted cases exit on first movie;rounds
request does not imply multiple interrupted paths. Exact assertions require
each acquired movie retired once,beer consumption per normal round,one final
timer/local retirement. Existing expanded72 models rerun after harness extension
pass;20-file independent diff/parse rerun passes. No frozen source/candidate/root
inputs changed. Still pending complete29-unit seal/output review and subsequent
root integration/regressions/reporting/package. No game/input/save/install changes.

W108 continuation progress/verified wait:focused tests rerun10 pass0.03s with
focused_tests.xml saved. check_scratch_gates.py independently audits both XML
results,original cleanup reports,expanded/repeated runs and each generated input
hash,exact diff/parse,raw smoke hash and all3018 frozen inputs. Snapshot status
pending_full_corpus intentionally distinguishes live partial inventory from pass.
Corpus41473 remains live:18 completed units,only Madame2 changed. Package builder
CLI inspected read-only;no packaging yet. Attempted read of nonexistent old
integrate.py failed harmlessly;filename discovery found old finalize.py,not run.
Next terminal corpus audit,then generic root helper/unit/report integration and
portable fixture/regression plus exact20 regeneration. Prior root mutations
stay frozen until corpus finishes. No promotion/game/install/input/save changes.

W108 latest turn verified wait:original corpus41473 polled live,now21 units
complete and only Madame2 changed. Prior W107 finalizer read-only inspection
clarified named-bundle/seal and partial GuildTraining-layout preservation;
not executed. Root portable regression conventions inspected for subsequent
integration. No frozen input or root edits. Full corpus still pending.

W108 verified wait:original41473 polled live across two bounded30-second waits.
Now24 units complete,two expected Madame changes. SummoningShip,TourGuide and
TraderConflict freshly complete unchanged. No restart,input mutation,promotion
or game/install/save action. Remaining corpus and integration gates unchanged.

W108 substantive progress:original corpus41473 terminal exit0,29/534 complete,
only Madame2 changed. Scratch gate rerun passes all3018 frozen hashes and full
output inventory. Before root edits prepare_root.py preserved dirty converter/
unit/ledger snapshots and pinned534 current production Lua. Integrated generic
helper,final-phase hook and exact unit review row;old unit metadata deep-equal.
Report recovery metadata remains separate from original lifter call inventory.
First dynamic evidence patch failed missing plus prefixes;fixed apply_patch,
verified valid JSON and original metadata unchanged. Root code py_compile passes.
Portable fixture is exact captured phase (hash matches),not production-Lua edits.
11 root focused checks pass23.89s,including actual native evidence/reapplication.
Integrated60010 exit0:both stages regenerate20 Lua exactly matching tested
scratch;all534 production Lua unchanged. integration_review.json records pins.
Broader root regressions without old scratch monkeypatch launched session77465;
current sources frozen until terminal. No promotion yet;next root results and
report review,scoped2-file promotion/ledger/new package/seal. No DLL/game/install/
input/save changes. Pre-integration checker must not be rerun against deliberately
changed root source hashes;retain its historical accepted scratch evidence.

W108 latest verified wait:integrated regression77465 remains live,passing dots
through first10% with no failure output;not a completed result. Integrated
conversion report has exactly one recoveredCleanupOperands entry;exact20-file
integration status remains pass. Readability audit CLI confirmed --out scratch
JSON/MD for subsequent promotion review. Root sources remain frozen;no generated
production promotion,ledger replacement,packaging or game/save/install action.

W108 substantive progress:77465 terminal exit0,674 integrated regressions pass
115.02s,no failure/error/skip. review_integrated_reports.py proves the sole
conversion difference vs tested scratch is explicit Madame Main recovery entry
with5 source replacements;all original lifter inventory/summary unchanged and
readable report equal except intended draft input path. promote.py backed up
the2 prior Lua and3 reports under promotion_before,then promoted only Madame
draft/readable pair and generated reports (readable draft path rebased).
20 Bordello outputs exact integrated scratch;532 other production Lua unchanged.
promotion_review.json records actual changes. Owned command session66506 now
runs readability audit to scratch JSON/MD;no terminal result yet. Do not rerun
promoter. Next ledger comparison/refresh,new named bundle/preflight/seal. No
installed-file/game/DLL/input/save mutations. PriorW107 root seal intentionally
superseded by this integration,not reusable against current root.

W108 continuation progress:audit66506 exit0,371 files/zero syntax failures/908
unresolved;summary unchanged,only Madame JSON finding locations change. New
finalize.py verifies integrated source/output pins and promotion scope,refreshes
ledger then derives new madame-cleanup-candidate-20261010 from validated W107
payload. Owned48964 currently live;bundle and package/completion reports not
yet present at actual filesystem check. Do not rerun active finalizer. Current ledger
equals reviewed audit. Remaining199-Lua/204-hash/W96 DLL/read-only preflight/seal
gates still executing;independent final verifier required after terminal. No
game/install/input/save changes;offline scoped acceptance not yet verified.

W108 progress:outstanding tool560 completed;finalizer48964 stayed live and was
polled rather than restarted. Native process15088 confirmed with read-only CIM;
artifact checks showed bundle absent first,then205 files including manifest.
It reached terminal exit0,completion records3100 sealed inputs,199 parsed Lua/
204 verified payload hashes and passed read-only preflight. Only Madame readable
and README payload differ from W107;validated W96 DLL remains unchanged,W99
fishing scratch excluded. Ledger summary371/zero syntax/908 unchanged,only
Madame finding locations changed. Independent verify_acceptance.py authored
and launched owned35535;live,no terminal result yet. It checks seal,payload
inventory,root534 exact2 changes,Bordello20 exact,reports/ledger and read-only
preflight. No promoter/finalizer reruns. Final independent acceptance status
pending;no gameplay/install/input/save/exception/event-delivery proof claimed.

W108 accepted offline,independently verified:35535 terminal exit0. Verifier
rehashes all3100 accepted inputs,204 payload files,199 parsed Lua,manifest/seal
link and exact205-file inventory;534 production scope has exactly Madame2
changes,532 unchanged;20 Bordello outputs equal integrated scratch. Reports and
ledger equal reviewed artifacts,read-only preflight passes again. No installed
files/game/input/save changes. Native guard/fixture/portable tests remain owned
root integration,not hand-edited generated production. Scoped outcome fixes
four generated beer exits through five recovered source operand substitutions.
674 integrated tests and11 portable checks pass;scratch154 related/10 focused,
expanded72/repeated144 strict generated contracts and12 original destructor/
6 receiver-order checks have explicit boundaries,not gameplay proof.
New bundle madame-cleanup-candidate-20261010 keeps W96 bridge and excludes W99
fishing DLL;only Madame readable/README payload change from W107. Ledger
371/zero syntax/908 unresolved unchanged;Madame4 diagnostics/two global warnings
remain. NoW108 live processes or tool cells. Active user goal continues,not
marked complete. Stop this accepted scoped increment;next registered Madame
PlayerOwned polling timer receiver/lifetime evidence offline. No new script
registration/conversion or inferred game-launch authority. Do not rerun promoters.

W109 next scoped evidence progress:existing V_Bordello/Madame timer polling.
Current root draft PlayerOwned branch GetTimer(4),SetTimer(4,fVar19),where
fVar19 is distance radius8. Native full-body aligned disassembly:both polling
E3BE74/E3CE20 use ESI loaded from balanced ESP+18 and global GSI slot168.
Reset E3CE61 pushes4(duration),then ESI(timer),slot164. First raw bounded
decode began mid-instruction and included nonsense prefix bytes;discarded,
all retained evidence decoded from true full function entryE3BB70. No inference
from malformed prefix or exported depth annotations. Existing C++ declarations
confirm integer timer ID/time interfaces;slot helpers corroborate164/168.
Owned W109 pin_native.py snapshots six current dirty inputs,HEAD366c08e and
EXE41dc9109/body hash;native_evidence.json is inventory,not candidate acceptance.
Next original-byte receiver/argument proof and strict generated PlayerOwned
reproduction,then generic recovery/negative tests/scratch ladder if supported.
W108 accepted increment remains untouched. No root/game/install/input/save changes.

W109 progress:reproduce.py executes actual generated Init/OnPersist/Main in
strict Lua contract model selecting PlayerOwned and prior DoneIntro state.
32 executions across draft/readable,timer0/4/44/FFFFFFFF,remaining0/1,near/far:
24 WRONG_TIMER_ID:4 faults,2 WRONG_TIMER_DURATION:8 faults independently exposed
by timerID4,6 passing controls that retire timer/local once. Initial readable
module load lacked helper preload;explicit strict empty module supplied,any
unexpected helper remains fatal. No method fallback or hidden save I/O.
reproduction.json pins both current Lua and captures errors/events. Aligned
CE13..CE67 instructions retain ESI from balanced ESP+18 across query/distance/
termination boundaries;native radius8 push41000000 is distinct from reset
duration4 pushCE5E and timerESI pushCE60. Next original-byte conditional/ABI
execution proof,then candidate only if supported. No root/package/DLL/game/
install/input/save mutations;W108 remains accepted prior increment.

W109 progress:native_timer.py executes original E3CE13..E3CE67 bytes with
explicit timer/distance/termination ABI doubles.32 cases pass across timer
0/4/44/FFFFFFFF,expired/not,near/far,terminating/not. GetTimer receiver is global
GSI,argument actualESP+18 timer;SetTimer args sameID,4 only expired+near+live.
Expected exitsCE67/CE7F/E3E2EF,balanced ESP and retained ESI all asserted.
native_timer_results.json records original region hash and events;not actual
timer/geometry/event/gameplay effects. Registration window independently shows
globalGSI15C returnEAX->ESI->ESP18,then SetTimer(ESI,2);CString constructor
boundary lies between store and initial reset,needs bounded proof next.
Fresh baseline capture.py observes source AFTER acceptedW108 operand recovery
without changing root,then both-stage Bordello regeneration/20-file comparison.
Owned session60888 currently live,no output;poll same handle. No candidate/
root/install/game/input/save modifications this increment.

W109 progress:baseline60888 exit0,both stages20 exact current production Lua.
Four original registration/store/reset2 executions pass with actual99E4B0 CString
constructor (clearsESP14,counter increment,leaves timerESP18 untouched);timer
registration/reset dispatch remain explicit ABI doubles. Native registration
report distinct from prior32 polling branch checks. Post-W108 phase captured,
reviewed_timer_operands.json created with native/source/phase hashes,three byte
witness regions and unique bounded query/reset span. Scratch reuses generic
existing restorer after originalW108 recovery;two operand calls only,radius8/
predicates/yields untouched.10 focused tests actual-native restoration and
negative hash/witness/bounds/count/query/reset/reapplication pass0.43s,XML saved.
Owned generation73815 live,no output;poll original handle,no restart. Pending
generated models,related tests,all-unit corpus and later integration gates.
No root/generated production/package/DLL/game/install/input/save mutations.

W109 progress:generation73815 terminal exit0,10 readable Lua parse,only
Madame draft/readable2 change,18 other Lua unchanged. Harness now accepts
explicit candidate tree/stages/expect/report and termination at distance
boundary (persistent state transition,not query toggles). Baseline32 recheck
still26 faults/6 controls. Candidate draft16 normal and16 termination pass,
then complete draft/readable32 normal+32 termination pass. Preliminary16+16
overlap final runs;do not sum as96 unique paths. Exact assertions require
registerTimerID,initial reset2,queryID,radius8 only when expired,reset4 only near
and live,then one deregister/local release. Candidate reports retain hashes and
events;all boundaries remain mocks,not engine gameplay. Related existing13
files/154-test scratch-hook run69724 live,passing through46%,not accepted yet.
Root/acceptedW108/package/game/install/input/save unchanged. Next related
terminal,independent diff/API smoke and all29 corpus before root integration.

W109 progress:69724 terminal exit0,154 related tests pass106.81s. Exact generated
diff inspected and independently asserted by review_generation.py:all20 Lua
parse,only two timer calls change in each Madame stage,18 other files unchanged.
Predicates/radius/yields/cleanup byte-identical. Typed scratch smoke actualW96
source profile JSON reviewed42 callbacks;zero error/unknown/pending,two deliberate
Madame/Magicman bridge-global warnings. Full29 native/readable corpus launched
owned67844;3128 hashes frozen from W108 accepted baseline and new recovery/
candidate inputs. Script reuses generic existing operand restorer after W108
only for reviewed Madame;Bordello reuses current completed scratch generation.
Poll same handle,keep frozen inputs;no restart or integration before completion.
Root/production/package/game/install/input/save unchanged. Next full corpus
output/seal audit,then integrated-root/reporting/promotion ladder if accepted.

W109 progress/verified wait:3128 frozen hashes rechecked,zero mismatch;67844
still live with18/29 units completed at latest observation,only expected Madame
draft/readable differences. Added72 generated
candidate checks using sealedW108 beer harness,writing reports only intoW109.
Three normal interactions use distinct movieIDs;interrupted paths exit first.
All prior strict beer retirement/consume/timer/resource contracts pass. New
check_scratch_gates.py audits XML10/154,native polling32/registration4,generated
timer32+32,beer72,exact20 diff/parse,typed42 smoke and frozen hashes. Status
pending_full_corpus accurately preserves incomplete all-unit gate. No source
pins changed. Corpus header docstring inherited older generation labels;actual
P path/seal/baseline point W108,inspected and verified,not changed mid-run.
Prepared unexecuted W109 prepare_root.py/integrated.py: snapshot refuses a
pending corpus gate; integration compares all20 against tested scratch. No
frozen inputs changed. Next full corpus gate,then root property/hook/report and portable fixture/tests,
exact integrated20 output before promotion. No production/game/install/save/
input/package/DLL mutation;goal remains active.

W109 continuation: corpus67844 confirmed live; GuildTraining worker6464 and
SickChild3116 independently observed live with CPU159.70/95.81 seconds. Latest
completed inventory18/29 still only Madame2; no terminal report. Prepared
unexecuted root_regressions.py (includes prior cleanup and new timer suites)
and review_integrated_reports.py (only separate timer metadata permitted).
Preparation does not establish integration acceptance. Fresh3128-pin/status
audit68451 subsequently returned exit0:3128 pins,zero mismatch,18 completed at
its snapshot. Corpus67844 then reported SickChild unchanged (19th unit),still
live; GuildTraining CPU275.28 seconds. Audit68451 is terminal,do not repoll.
Corpus inputs unchanged; root/game untouched.

W109 corpus67844 terminal exit0:29 units,534 Lua,only Madame draft/readable2
changed;3128 pins reverified. check_scratch_gates terminal scratch_gates_pass
audits all focused/native/generated/beer/typed-smoke/corpus outputs. prepare_root
snapshots dirty accepted sources/ledger and534 actual production Lua. Added
reviewedTimerOperands property exact tested row (all prior unit data identical),
converter final-phase restoration after W108 cleanup with separate recovery
report entry;existing generic helper unchanged. Portable phase fixture exact
e6339e934a0b2b068cab9a5ff6aef879b1c52bfb2127831783b94ed7f2fb931b;
10 portable tests pass0.96s. Integrated both-stage generation19580 now live.
No production Lua promotion/package/game/input/install/save/DLL changes. Frozen
corpus pins deliberately superseded only by scoped integration source edits;
do not rerun old scratch hooks/gates after integration. Next exact20 integration
and report comparison,then direct root regressions before scoped promotion.

W109 integration19580 terminal exit0:all20 output hashes equal tested scratch,
all534 production Lua still unchanged. review_integrated_reports passes one
separate timer recovery entry2 replacements,original inventory and summaries
unchanged;readable report differs only draft path. Converter scoped diff is
exactly8 added lines,unit prior evidence deep-identical,existing helper hash
unchanged. Direct root regressions69389 live,no scratch hook. Preliminary
inspect_source comparison confirms Madame generic references586->585 only;
nativeNames136,gotos120,unresolved4 unchanged. Expected ledger summary8424->8423
generic references,371 syntax-clean files/unresolved908 unchanged. Prepared
unexecuted promote.py gates684 clean root tests and all integrated hashes;
finalize.py derives explicit madame-timer-candidate-20261010 from W108,only
MadameReadable/README payload changes,W96 DLL unchanged. Independent verifier
created before seal collection (will be sealed). All3 prepared scripts parse.
No promotion/package/game/save/install/input/DLL changes;next terminal root
tests,scoped promotion,audit,seal and independent acceptance verification.

W109 accepted offline. Direct69389 terminal exit0:684 tests pass118.61s,zero
failures/errors/skips. First promotion attempt stopped before writes because
new all-lifted inventory assertion incorrectly equated646 total files with534
registered baseline. Inspected112 extras (legacy FSE,partial GuildTraining,
unregistered prior lifts),none part of prior registered pins;all preserved.
Snapshot lifted_inventory_baseline.json before writes;corrected guards check
exact646 inventory/hashes rather than dropping extras. Promotion7731 terminal
exit0,only Madame2 and reports changed,all644 other lifted files unchanged.
An audit taken while promotion was live saw old8424 count;discarded as acceptance
input and reran after terminal promotion. Completed audit371 syntax-clean,
unresolved908,only Madame generic586->585;summary8424->8423,other fields exact.
Finalizer27460 terminal exit0 derives madame-timer-candidate-20261010 from accepted
W108,only MadameReadable/README payload changes,W96 DLL unchanged. Independent
verify_acceptance.py terminal exit0 rechecks3303 source/output/report pins,
199 Lua parse,204 payload hashes/exact205 files,root646 inventory/only2 changes,
Bordello20 exact,ledger/reports and read-only installed-game preflight. Verifier
itself included in seal. completion.json accepted_offline_madame_timer_operands;
acceptance_verification.json pass. No game/install/input/save/DLL changes or
gameplay claims. AllW109 handles terminal;do not rerun promoters/finalizer or old
scratch hooks/gates. Stop this scoped accepted increment;overall goal active.
Next W110 existing BordelloLady indirect-call/resource operand review around
draft541/689,retail receivers/stack and lifetime evidence before selecting fix.
No new conversions;W99 runtime proof still requires fresh explicit away window.

W110 existing Lady vtable residue review:owned
work/codex_lua_lady_vtable_residue_20261010/review.py snapshots6 accepted inputs
and reviews original MainE3EB10..E403CA. Draft541/689 ppuVar13 assignments have
no consumers after accepted W105 E3F731 PauseAllNonScriptedEntities(false)
restoration. E3F729 loads receiver balancedESP14,vtable,[5EC],push0;E403AF loads
vtable fromEBX,pushEDI,movECXEBX,jumpsE3F731. Six original-byte bounded dispatch
executions (2 entries x3 supplied receivers) pass with explicit interface ret4
double,argument0/balancedESP. Does not reexecute actualengine unpause/cleanup or
prove supplied receiver provenance independently (priorW105 packet provides it).
Initial harness failed unmapped-write due to0x12600000 vs0x01260000 map;no retail
execution occurred. Corrected map,validated retained snapshots,reread original
bytes. review_results.json reviewed_no_production_change;diagnostics remain.
No generic lowering change justified solely to erase unused vtable residues.
Independent W1093303-pin check still passes. AllW110 handles terminal.
Next W111 existing Magicman Main0x00e40e80 early movie-abort timer operands at
draft229/230 and241/242:generated DeregisterTimer(me)/DeregisterTimer(iVar18)
looks type-inconsistent;must prove timer identity/stack/receiver from original
bytes and reproduce generated exits before any recovery. Its earlier polling
aliases explicitly iVar21=i_stk_378 and use timerId;do not conflate branches.
Overall goal active,no new conversions or game/install/input/save/DLL actions.

W111 inventory:pin_inventory.py snapshots6 accepted inputs/aligned Magicman Main
E40E80..E4494F,bodySHA b5ea2b9a96be2fc16dbb3c29523ffcb4c890b2abcf070f16fa40e5321601397b.
E40F7D first registration stores balancedESP1C(i_stk_378);E40F8F second stores
transientESP20 after two pushes,balancedESP18(timerId),then initial reset40.
Loop-abort E4468C unpauses through ESI/EDI,then movie memberESP98 6E7AB0/base90
99A430,globalGSI slot160 timer18 atE446C8 then1C atE446DB,CStringESP14 99EAE0,
counted localESP30 callback/free+BFE9BC,baseESP24 99A430 and epilogueE44724.
Generated early speech-failure/interruption passes me/iVar18 to timer retirement
and returns without movie/local cleanup;complete cleanup gap,not timer-only fix.
Post-loop E44677 (EDI interface,push0,jumpE4489C,movie48) is distinct from
loop-abort E4468C (ESI interface,EDI0 candidate). Register/stack/provenance,
movie/local identity and destructor equivalence still pending. Failed-Speak guard
is synthetic for native void Speak and needs separate contract testing.
native_inventory.json inventory_not_a_candidate,not behavior proof. Next bounded
original registration/complete cleanup execution and actual-generated adverse
reproduction before generic recovery. No root/production/package/game/install/
save/input/DLL changes;allW111 handles terminal,overall goal active.

W111 evidence progress:native_abi.py terminal pass6 registration/18 abort cases.
Six timer pairs include0/44/FFFFFFFF and coincident IDs as adverse ABI inputs;
not claims that retail RegisterTimer duplicates IDs. Null/shared2/last1 local
cases execute original counted decrement/control;callback/free boundaries are
doubled. Movie member/base andCString destructors,GSI register/reset/unpause/
deregister also doubled. Supplied ESI receiver/EDI0,full provenance pending.
Checks real balancedESP18/1C stores/loads,initial second timer40,cleanup order
unpause,movie member98/base90,timer second/first,CString14,conditional local
callback/free,base24 and balancedESP. native_abi_results.json pass.
reproduce.py executes actual draft/readable Main in strict no-fallback mock APIs;
invalid timer args deliberately recorded instead of thrown to observe leak state.
Initial readable run stopped at missing explicit IsHeroWearingBeard helper alias;
mapped verified readable name to same true predicate double,no generic fallback.
Terminal rerun12 cases (2stages x immediate/loop x3 timer pairs) reproduces wrong
actor/hero IDs and all4 tracked handles live. reproduction.json statusreproduced.
Native void Speak has no immediate-failure branch;synthetic guard is explicitly
separate. No source/generator/production/package/game/install/save/input changes.
Next resource/movie construction origin,nonvolatile receiver/zero provenance and
actual split/full movie destructor comparison before complete guarded recovery.

W111 path correction caught before lowering:actual first PIMPING Speak atE4172E,
task loop E41737/E4174A,termination branch E41751->E445B8. Initial decompiler-
adjacent E4468C cleanup selection belongs a later movie,not first branch. Old
native_abi_results/native_movie_equivalence_results preserved but not first-path
proof. StartMovie hidden resultESP48 atE415A8;first abortE445B8 unpauses through
EDI/push0,destroys member50/base48,retire timer18/1C,CString14,xorEDI0 E4460E,
jumpE446EA inline local cleanup/base24. New native_first_abort_results pass6
registrations/18 actual first-aborts;native_first_movie_equivalence_results pass12
original split/full comparisons. Actual destructor bodies execute,imp/free double.
Full generic register_facts correctly refused2 indirect switches;owned provenance
analysis validates cmpEAX5/JA default and6 table entries atE44950/E44968,then
includes all edges. Initial EBP-whole-function-invariant assertion failed due
E444FB rebind;tracked EBP state instead,proves script atE415BA and EDI interface
atE41751/E445B8/E445BE. No unsupported assumptions ignored. provenance_results
records original byte windows,tables/hashes and scope/ABI limitations. Generated
12 adverse reproductions still valid;no candidates/production changes. Next
inline/full local destructor comparison,then guarded full first-abort recovery
with separate synthetic failed-Speak cleanup contract. Overall goal active.

W111 local destructor gate: native_local_equivalence.py passes12 original-body
comparisons E446EA..E44724 inline vs7E74D0/99A430 full,objectESP24,counted null/
shared2/last1 and info-pointer/ref variants;imp callback and free doubled.
Provenance byte windows/correct first-abort tests remain authoritative;earlier
later-movie packets excluded. capture53185 live confirms frozen baseline by
regenerating Bordello both stages/20 Lua and capturing Magicman phase;no terminal
result yet. Prepared prepare_candidate.py gates terminal baseline/native reports
and certifies2 bounded full cleanup replacements with native witnesses;existing
guarded generic helper supports them unchanged. generate_candidate.py uses a
scratch-only unit copy adding reviewedCleanupOperands MagicmanMain,not a global
hook (root invokes generic helper twice after W109;avoid double application).
Replacement keeps unpause/return,adds first-movie destruction,timer second/first,
local resource release. Original task interruption and synthetic failed-Speak
guard distinctly documented;other predicates/yields/normal cleanup unchanged.
Neither prepared script executed yet. Next same53185 terminal observation,then
scratch candidate and strict12 contracts plus adverse tests/typed smoke/all29
native/readable impact comparison before root evidence integration. No root/
production/package/game/install/save/input/DLL changes;overall goal active.

W111 current phase:53185 terminal exit0,fresh20 Lua exact accepted W109 baseline.
Initial prepare_candidate.py assumed final generated2 return blocks existed at
operand hook;assertion rejected. Subsequent generation saw absent review file,
stopped before writes. Captured phase inspection shows ONE native interruption
block ending --TODO gotoE446EA;UnitConverter later hoists cleanup and emits
synthetic speech cancellation guard. Corrected certificate replaces that one
block (unpause,movie destroy,timer second/first,local release,return). Later
guard inheritance independently requires generated behavior verification.
reviewed_abort_cleanup.json created,11 focused/adverse tests pass0.45s including
source/native/witness hash,label,count,actor/hero/jump mutations and reapply.
generate_candidate91100 live,uses scratch unit reviewedCleanupOperands property
with existing helper unchanged. related_tests94641 live,runs154 established tests
via deep-copied UnitConverter unit overlay;not helper patch/double application.
All root snapshots/pins unchanged;no production edits. Next same live handles,
strict12 generated candidate contracts,exact20 diff/parse/typed smoke and all29
both-stage impact comparison before root evidence integration/promotion/seal.

W111 scratch generation91100 terminal exit0,only Magicman draft/readable2 change,
18 other Bordello Lua exact. Candidate strict model66452 terminal pass12,retire
movie202,timer second/first,local11 in order on synthetic immediate failure and
original loop interruption;no wrong IDs/live handles. Related94641 terminal
154 tests pass114.83s,zero errors/failures/skips. Exact diff review independently
parses20 and reconstructs expected fullfile changes:two early executable cleanup
blocks plus2 later draft-only --LAB_E446EA native-target annotations,caused by
hoister moving its remaining unresolved-target marker after originalfirstjump
restored. Later executable cleanup unchanged;comments retained,not suppressed.
Typed actualW96 sidecar smoke JSON independently reviewed42 callbacks,zero
runtime/unknown/pending,two intentional PresentedItemName globals remain.
Started full29 native/readable corpus97787;3331 frozen hashes from acceptedW109
seal/current reviewed row,phase/generator/candidate;Bordello reuses current
completed regeneration explicitly,other28 fresh both stages. Corpus code has
correctW111/W109 labels,no generic helper patch. Root production/source/package/
game/save/input/install/DLL unchanged. Next same97787 observation,full534 terminal
diff/hash audit,then owned root evidence integration/portable tests/promotion.

W111 expanded actual-generated contracts:64 distinct matched cases (edge32,
delayed2 loop16,delayed3 loop16) pass candidate complete retirement and reproduce
baseline actor/hero timer faults/all4 handles live. Baseline21145 terminal exit0;
candidate expansions terminal. Inputs include0,4,44,FFFFFFFF,80000000 in both
registered roles,loop interruption after1/2/3 speech frames. Four original12
overlap,72 distinct total rather than76;keep report counts separate. Strict last
events unpause,movie202,timer second/first,release11.3331 corpus hashes rechecked
zero mismatch;harness edits outside running corpus inputs,no candidate changes.
check_scratch_gates audits XML11/154,correct native6/18,movie12/local12,provenance,
initial12/matched64 models,exact20 diff/parse,typed42 smoke;explicitly excludes
wrong-movie packets. Statuspending_full_corpus,1 completed/BanditCamp unchanged.
97787 polled again,confirmed live;all otherW111 handles terminal. No root/game/
production/package/source edits. Next same corpus handle,terminal29/534 audit.

W111 verified wait:97787 polled live,now8/29 complete,only Magicman2 expected
changes. Prepared unexecuted prepare_root.py/integrated.py,AST parse pass. Gates
terminal scratch_gates_pass before dirty-root snapshot;registered534 identities
come from W109 production_baseline,not naive filter of current3331 seal (which
contains all646 lifted files). Snapshot/check all646,including112 legacy/partial
extras. Integration requires root exact reviewed row,existing converter/helper
unchanged,all20 candidate hashes exact,and646 production Lua unchanged. No root
or frozen-source edits. Same corpus process remains next authoritative handle.

W111 report metadata review:review_reports.py passes. Baseline/candidate original
function/lifter/call inventory exact after removing separate Magicman cleanup
certificate;29 synthetic guard entries same,count unchanged. First exit metadata
lists6 corrected cleanup statements;entry4 differs only native-target comment;
line numbers shift consistently with independently exact generated-source diff.
All other report contents equal. report_review.json pass and added scratchgate.
--integrated mode requires literal candidate/native report equality and readable
report equality except draft path;prepared,not executed. Corpus97787 polled live,
18/29 complete,only expectedMagicman2 changes. No frozen inputs/root edits.

W111 verified wait:97787 live,20/29 complete after SickChild/SingingStones
unchanged. GuildTraining worker11108 live CPU168.47s;do not restart. Prepared
unexecuted root_regressions.py,AST pass,prior684 + newportable11 expected695;
no scratch overlay,requires exact integration/report gates and checks646 Lua
before/after. No frozen inputs/source/production changes. Next same97787 handle.

W111 verified wait:97787 repeatedly confirmed live,24/29 complete (StatueMaster,
SummoningShip,TourGuide,TraderConflict unchanged);only Magicman2 expected changes.
No terminal corpus report yet,no source/pin/integration/promotion changes.

W111 corpus97787 terminal exit0:29 units,534 Lua,only Magicman draft/readable2;
3331 frozen pins reverified. Terminal scratch_gates_pass audits XML/native/models/
diff/reports/smoke/corpus. prepare_root snapshots accepted dirty sources/ledger,
534 registered and646 full lifted inventory. Root unit receives exact tested
reviewedCleanupOperands MagicmanMain row only;all other parsed unit data equal
snapshot. Converter/helper untouched. Portable fixture exact captured phaseSHA
86db0c247958215b521f45fdba2ba188d88e221924438c7ff869333c387af61b;portable11
tests pass1.17s. Integrated39675 now live,without scratch overlay;root production
646 Lua still unchanged. Earlier scratch pins deliberately superseded only for
root-unit evidence;do not rerun scratch hooks/gates after integration. Next
exact20 integrated output and literal candidate report review,direct695 root
regressions,then scoped promotion/audit/bundle/seal and independent verification.
No production/package/game/install/save/input/DLL changes;overall goal active.

W111 completed, superseding the in-progress integration note above. Integrated
39675 terminal exact20;literal candidate report review pass. Direct-root17861
terminal695 tests pass111.01s,zero failures/errors/skips. Scoped promoter
terminal:Magicman2 plus reports,532 registered/112 other lifted Lua unchanged.
Audit371 syntax-clean/908 unresolved/8423 generic references unchanged;only
Magicman row line/finding positions differ. Finalizer98906 terminal seals3394
inputs and derives magicman-abort-candidate-20261010 from W109;199 Lua/204 hashes,
only MagicmanReadable/README payload changes,W96 DLL unchanged. Independent
verifier terminalpass all3394 pins,205-file payload,all646 lifted Lua,exact20
outputs,reports/ledger and read-only preflight. No game/install/save/input changes.
One native first-loop cleanup certificate feeds two generated exits;synthetic
failed-Speak guard separately modeled. Actual-byte6 registration/18first abort/
12movie/12local cases use explicit doubles. Original12+expanded64 overlap4:
72 distinct matched cases,not76. Earlier wrong later-movie packets excluded.
All W111 handles terminal;do not rerun sealed promoters/finalizer/scratch hooks.
Next bounded existing recovery:later Magicman abort E4468C with movieESP90;
first-movie proof must not cross lifetime boundaries. Goal remains active.

W112 work/codex_lua_magicman_attacked_abort_20261010:current W111 six-source
snapshot and native inventory terminal. FemaleE41EC6->E4468C uses movieESP90;
maleE41FE1->E4474C distinct cleanup with same movie lifetime. Original36 bounded
abort runs pass under explicit dispatch/destructor/callback/free doubles.
Actual accepted draft/readable24 strict-model exits reproduce female actor/hero
and male-1/actor timer mistakes;movie retired but local/two timers live. No
unmodeled API fallback. Whole-function provenance with both conditional outcomes
and two verified six-entry switch tables proves ESI interface both branches and
female EDI0,assuming nonvolatile call preservation. All handles terminal.
No new lowering/promotion/package changes. Next gates:actual movie/local destructor
equivalence,ownership/lifetime evidence,bounded phase certificate and scratch
adverse tests/full unrelated-unit comparison. W111 remains accepted root.

W112 further progress:both later movie split/full comparisons24 and local
inline/full24 pass under original bodies,with explicit imp/free doubles.
Ownership witnesses prove text literals/movieESP90/timerESP18+1C/localESP24
identity under reviewed purge/hidden-result contracts;not engine/exception proof.
Baseline22466 terminalexact20. Existing W111 certificate extended in scratch:
first replacement preserved,two native attacked-loop blocks added,four generated
guarded exits. Pre-W111 phase86db... restored under accepted row equals captured
post-W111 phase403be05e2475fdbe9f28faa432e149696f1939b4d764a2da4e5786b8bd290dae.
Focused25 pass0.48s;candidate44907 terminalonlyMagicman2/other18 byte-identical.
Related6563 terminal154 pass105.39s,zero failures/errors/skips. Exact20 syntax
and complete draft/readable difference checks pass:only four attacked timer/local
retirements;first-PIMPING,normal paths and later repeated attacked speech unchanged.
Original report call inventories unchanged;cleanup metadata1->3,two guard exits
4/5 updated,29 guards retained. Typed smoke actualW96 profile42 functions passes
with only two known bridge-written global static warnings (parent-profile request
correctly rejected as incomplete,correct FableScriptExtender profile used).
Baseline edge64/delayed2loop32/delayed3loop32 reproduce;candidate matching128
distinct cases pass,initial24 subset. Candidate first-PIMPING12+32+16+16 overlap4
=>72 distinct retained passes. Runner wording corrected to report selected
reproduction/retirement rather than claiming candidate leaks;candidate128 rerunpass.
Corpus59218 now live,3422 frozen inputs,29 registered units. W111 all3394 pins
validated at corpus start. No W112 root integration/promotion/ledger/package/DLL
changes. Next terminalfullcorpus,independent scratch gate,portable/root695+new
focused integration suite,scoped promotion and sealed offline bundle. Goalactive.

W112 corpus59218 terminalexit0:29 units/534 Lua,onlyMagicman2,3422 pins verified.
Independent scratch_gates_pass checks36native/24movie/24local,25/154XML,128matched
attacked cases with identicalprecleanup eventprefixes,72 exactfirstabort event
traces,20fullfilediff/report/smoke/corpus. Older accepted original12 first-abort
packet has no delay field:normalized only schema to original first-frame loop;
sealed packet untouched. prepare_root snapshot7owned sources,534registered/all646
Lua. Root propertyrow updated only;allotherparsedunitdata equal snapshot.
Initial PowerShell row-end lookup found earlier Madame timer row and rejected
before any write;bounded next-row lookup then applied reviewed patch successfully.
Existing portable11 strengthened to validate all certified replacements;new25
portable tests reuse accepted86db... fixture. Combined36 pass1.32s. Converter,
generichelper,fixture untouched;no production/package/ledger changes. Integrated
44921 live without overlay;prepared suite720/promoter/finalizer/verifier unexecuted.
Next terminal20/report exactcandidate,720directroot then scoped promotion/audit/
new magicman-attacked-abort-candidate-20261010 bundle/seal andindependentverify.
Do not rerun oldscratchgates after deliberate root-unit/portable-test integration.

W112 completed,superseding in-progress notes above. Integrated44921 terminal
exact20;integrated native/readable JSON review passes. Gitnoindex17152 exit1
shows Markdown title-only difference,body exact. Reportreview34040 terminalpass;
root68499 terminal720 pass119.81s,zero errors/failures/skips. Prepromotion check
all3422 frozeninputs allows only reviewed unit/oldportable-test source changes.
Scoped promoterterminalonlyMagicman2+reports,532 registered/112 other lifted
Lua untouched. Fresh audit371syntaxclean/908unresolved/8423generic unchanged,
onlyMagicman finding/line positions shift. Finalizer terminalseals3487 current
inputs and derives magicman-attacked-abort-candidate-20261010 from W111;199Lua/
204payloadhashes,onlyMagicmanReadable/README changed,W96 DLL unchanged.
Independent verifier terminalexit0:all3487 pins,205-file inventory,all646 Lua,
exact20/regression720/portable36/reports/ledger and read-only preflight pass.
Allhandles terminal,no installed/game/input/save changes. Offline evidence only:
native36abort/24movie/24local use explicit doubles;128matched attacked models
and72 exactpreserved first-abort traces do not establish host/gameplay behavior.
New increment must trace remaining later Magicman cleanup/lifetimes separately;
zero diagnostics is not completeness. Goal remainsactive untiluserstop.

W113 work/codex_lua_magicman_first_movie_tail_20261010 started after W112
acceptance. Six currentW112 inputs snapshotted. Native inventory proves exactly
two original branchesE41869(Touting)/E419F2(PimpHat)->E44615,firstmovie ESP48
member50 cleanup then E448A1 timers18/1C and E4451F/E44528 CString/local24.
Root sharedtail currentlyDestroyMovie(xStack_330),while firstlivehandle stored
xStack_340;the laterbeer movie has not been created. Actualdraft/readable24
cases reproduce WRONG_MOVIE_ID:nil,live movie and retired local/timers.
Explicitfalse beard predicate and logicalOnPersist DoneToutIntro false/true
select both speeches without source/upvalue/save edits;invalidmovie double
records ratherthanthrows,so subsequent cleanup remains observable. Not stock
nil tolerance or engine exception proof. Whole-function two-switch conservative
CFG proves EDI interface/ESI0 unpause atboth originalbranches/sharedtail under
nonvolatile-call assumption. Allhandles terminal,no W113 codegen/root/package
changes. Next actualwhole-tail/movie destructor equivalence and bounded native
stack/phase certificate before any operand fix. W112 remains accepted root.

W113 scratch progress supersedes the initial note above. Actual native whole-tail
comparisons pass1080 branch runs against540 separately executed retail bridge
destructor sequences;4 AL-false returns correctly bypass cleanup. Frame/term,
timers,CString and release remain explicit doubles. Source phase captured after
accepted3 replacements SHA f09ed96c32b1baaa155e5d6051e4785169c7237b97b4f8b652193527029a4160.
Reviewed row preserves those3 exactly and adds one bounded330->340 movie operand.
Scratch generation terminal20:18 Bordello files unchanged;draft one operand,
readable same identity plus lexical declaration hoist/assignment at creation.
Focused11 pass0.64s;related154 pass134.57s. Actual generated models pass144
distinct Touting/Hat termination cases,including timer0/sentinel/highbit and
delayed frames. Exact accepted first-PIMPING72 and attacked128 event traces
preserved. Original report inventories and29 guard bodies unchanged;certificate
3->4/spanEnd only. W96-source typed smoke42 functions passes;two known globals
only. Corpus40869 LIVE,3515 frozeninputs,29 registered units;do not mutate pins.
Portable test proposals are scratch-only. No root production/unit/test,ledger,
bundle,DLL,game,install,input or save changes. Next terminal all29/534 and
independent scratch gates,then direct root tests/integration before promotion.

USER PAUSED 2026-10-10: "Let's call it here for now. Update docs etc for next session."
No further recovery authorized until user resumes. Corpus40869 stopped at user
request after28 completed units:only Bordello Magicman2 differs,27 completed
unrelated units unchanged. Exact remaining owned coordinator19168 and worker
18244(guild_training) stopped via scoped process checks;session terminalexit1.
This is interrupted validation,not a completed corpus or regression failure.
No owned corpus processes remain. Retained guild_training scratch is incomplete;
corpus_results.json and scratch_gates.json absent. Independent scratch audit
reached the explicit pending-full-corpus guard after passing all earlier checks.
W113 remains unpromoted scratch;W112 accepted root unchanged. Integration/root
portable proposals,promoter/finalizer/verifier prepared but not executed. No
production/unit/test/ledger/bundle/DLL,install/game/input/save changes for W113.
Resume instructions are at the top of LUA_RECOVERY_HANDOFF.md:revalidate3515
frozen pins and completed28 inventories,finish only guild_training child mode,
use a reviewed owned resume wrapper to finalize29/534,then scratch gates. Do
not rerun corpus top-level or earlier sealed promoters/finalizers. Root expected
portable47/direct731,integrated20,ledger/package/independent verify remain future
gates,not completed claims. All evidence and partial outputs retained.
Final checkpoint recheck:W112 independent verifier terminalexit0,3487 sealed
inputs/646 liftedLua/199 packageLua/204 payload hashes/preflight pass. Root
still exact acceptedW112;no game launched or installed files changed.
Machine checkpoint:W113/pause_checkpoint.json. Goal paused at explicit request.

