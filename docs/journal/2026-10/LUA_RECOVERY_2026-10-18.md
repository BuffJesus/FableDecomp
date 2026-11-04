# Lua recovery continuation - 2026-10-18

User resumed continuous Lua work, then said they would be away and asked that game
launches be coordinated around the 64-bit/modernization work. This authorizes
unattended Lua testing within the existing lane. Before any launch, check current
processes and modernization notes; do not interrupt another lane's game, input,
graphics test, install, or saves. Owned launch/restoration outcomes are recorded below; installed files remain pinned.

## W171: repair drift in existing behavioral test mocks (passed)

Resumed from W170 in `LUA_RECOVERY_2026-10-16.md`. Active root is
`D:/Documents/FableTLC`, branch `feat/novi-script-recovery`. Existing converter,
evidence, generated Lua, and native/modernization edits are baseline peer work.
No root commits/pushes. W166 remains an offline candidate, not installed.

Owned scratch: `work/codex_lua_mock_drift_w171_20261018/`; original four test files
and base commit preserved there before editing. Scope is the four W170 failures:
expression progress/cancellation, Arena guard village copy, Bordello boolean array
reservation, and HornetDrone animation operands. Generated output must stay intact.

Validation setup found two separate environment issues:

- Default pytest collection stalls in `_pytest.main._in_venv` while probing the
  large checkout. `baseline_diagnostic.log` preserves the traceback. Bound future
  runs with `--rootdir=tools/script_recovery --collect-in-virtualenv`.
- With collection bounded, pytest's faulthandler reports a fatal native exception
  inside Unicorn `mem_map` (`baseline_focused.log`). Python 3.13 and Unicorn
  2.1.3 reproduced that report. A standalone complete conversion passes, and
  pytest progresses when its faulthandler plugin is disabled. Thus the early
  Python-version diagnosis was not established. Current isolated validation uses
  Python 3.13 / Unicorn 2.1.3, `-p no:faulthandler`, and no plugin auto-loading.
  Only owned processes were stopped. No converter behavior was bypassed.

Baseline replay: all 961 relevant W166 generator/evidence input pins match the
current root, and every Lua artifact hash in the four replayed corpus units was
checked. `replay_baseline.py` executes the four unchanged test bodies against
those artifacts and records the exact missing contracts in `baseline_replay.json`:
`WithRetailResources`, `ReleaseResource`, `RetailRandModulo`, and
`PlayAnimationWithNativeArgument5`. Fresh representative Picklock and Arena tests
also reproduced their failures before the slow regeneration was stopped.

Test repairs add explicit mocks and strengthen checks for one-time target/movie
retirement, callback-error cleanup, Arena resource release on cancellation, one
retail RNG draw, and all animation arguments. Full focused plus adjacent ownership
and operand regression run is in `repaired_pytest.log`. Startup tests use their
existing `STARTUP_OPERANDS_ARTIFACTS` override for the hash-verified W166 corpus;
the other fixtures regenerate normally. Result: **214 passed in 379.48 seconds**.
All 76 W170 mock-drift cases now pass, plus four callback-error cleanup cases.
`verification.json` and `owned_changes.patch` preserve the reviewed four-file
delta against the dirty starting tree. The verifier checks all 961 generation
input pins again and all 534 Lua hashes in all 29 W166 corpus units. No generator,
evidence, production Lua, SDK, or packaged payload was changed for W171.

Next recovery task is the W172 movie handoff replay below. W162's current focused
tests pass; its three retail 10.0 operands remain packaged in the W166 candidate.

## W172: SummoningTheShip movie re-entrancy runtime replay (bounded gate verified)

The modernization lane's new 2026-10-18 handoff explicitly says no game launch is
planned and it will keep validation offline while the shared desktop is in use.
No Fable/title/input-probe process was present. Acquired the exclusive coordination
file `.agents/runtime-lease.json` for Lua W172 before touching saves. Keep other
launches/input tests out until this lease is released.

Owned scratch `work/codex_lua_ship_movie_w172_20261018/`: copied all hash-verified
W166 payloads into a new runtime bundle (candidate DLL c6ef7f50..., parent untouched).
Backed up and hashed all 563 current save files; pinned installed Fable.exe and
FinalAlbion.qst. Staged the previously used maze numeric checkpoint through the
existing save-staging helper. No installed payload changed. Next: launch, replay
the real lighthouse-use/movie handoff that failed W164, close the owned game,
restore and hash-verify every original save, and release the runtime lease.

Result: `work/codex_lua_ship_movie_w172_20261018/actual_runtime_results.json`.
Owned PID 9696 closed normally by WM_CLOSE. All 563 original save hashes and exact
file inventory restored; Fable.exe/qst pins unchanged; runtime lease released.
Both runtime-copy and untouched parent bundle's 208 payload hashes verified.

Real Tab on the natively targeted OBJECT_HKC_LIGHTHOUSE_PANEL_01 sets
LighthouseStarted. Panel movie id4 is destroyed before DoMission movie id7 starts
in the same scope. The intro completes: SummonerAttacksStarted=true, player
control=true, movie=false; summoner attack APIs and Briar Rose combat commentary
run. Three movie start/destroy pairs and three pause/unpause pairs; zero Lua/C++
runtime errors or movie refusals. W164's movie re-entrancy failure is resolved on
this path. This does not establish cross-thread waiting or same-thread refusal.

Limits: the weak checkpoint dies in combat, revives using a native phial, then
dies again before the movement check could run. No subsequent-wave, movement, or
completion claim. Tutorial/death/quest-start modals caused command timeouts;
the successful state reads are in the saved final log. Startup reached a quit
confirmation; No selected before the menu-only load. No gameplay state flags
were forced to pass; assistance was activation, region/hero positioning, and real
menu/tutorial input. SDK, Lua and installed payloads unchanged. W166 now has this
specific runtime proof, while W147 remains the last broadly accepted bundle.

## W173: reject float inference after stack-pointer clobbers (passed)

Owned scratch `work/codex_lua_float_stack_guard_w173_20261018/` snapshots the
pre-change dirty lowering/test files and base commit. Existing unit remains
`trader_conflict` (Evil Main DF6010, Good Main DFA450, Good transition DFC920).

Reproduced a converter proof bug: the backwards scan checked
`ins.mnemonic.startswith('add esp')`, but Capstone separates mnemonic `add` from
its operands. Consequently an intervening `add esp,4` still yielded the earlier
10.0 even though the argument slot had moved. Seven stack-write instructions and
a mixed valid/clobbered-site case all failed before the fix (8 failing adverse
cases, saved in `baseline_adverse_tests.log`).

Generic fix enables Capstone instruction details and rejects ESP/SP writes while
scanning the setup. A detected clobber invalidates function-wide inference even
when another site matches. Tests cover add/pop/lea/mov/inc/16-bit SP/leave, mixed
sites, harmless arithmetic on another register, and all four relevant original
retail functions (three recovered 10.0 values and the intentionally unshaped
fourth function). Eight test methods, including eleven subcases, pass in
`guard_tests.log`. No generated Lua was edited.

Fresh full existing 29-unit draft/readable regeneration is running through
`corpus.py` into `corpus_checked/`; compare against W166's 534 Lua hashes.
961 generator/evidence input pins were checked before generation: only the owned
lowering differs, and its saved baseline hash matches W166 exactly. Source inputs
must stay unchanged until this job finishes. Two workers; partial initial harness
attempt was stopped after review found a comparison-variable shadowing bug,
preserved under `corpus/`. The corrected collector uses a new output tree.

Result: `verification.json` PASS. Fresh 29 units / 534 Lua files are byte-identical
to W166. All 58 JSON reports compare exactly except readable draft-directory
provenance; no readable fallback or syntax errors. All 961 generation input pins
match. Typed TraderConflict smoke with the actual W169 sidecar sources: 15 files,
zero problems, exact baseline/candidate JSON equality. Initial smoke used the
legacy default sidecar and reported RetireThingOutput unknown; explicit current
source directory resolves that mismatch without a code change.

Readability audit: 371 files, zero syntax failures, 857 unresolved diagnostics;
fresh Markdown exactly matches the existing ledger, so no no-op rewrite needed.
Owned two-file diff against the dirty baseline is `owned_changes.patch`. Original
sources and initial failed harness attempt preserved. Read-only profiler stack
shows the long GuildTraining job doing normal readability flow analysis, not
waiting on the float guard. W171's current-root verifier is intentionally
superseded by this lowering edit; its saved successful result remains baseline
evidence. No bundle/output promotion needed because every Lua byte is unchanged.

## W174: constant-false branch chains (revision two promoted)

Owned scratch `work/codex_lua_constant_false_chain_w174_20261018/`. Shared
`readable_style.py` was held unchanged while candidate corpus pins were active.
Candidate extends the existing literal-false fold across an elseif/else chain
only when the removed arm has no labels, comments/diagnostics, or opaque text.
Promote the next elseif to if, or retain the else body in an explicit do scope.
This preserves the Lua lexical lifetime of locals and close variables.

Isolated probes: 24 branch/arity/truthiness/close-order cases and four refusal
cases pass. Direct fold removes five unreachable WomanToAttract jumps (88 -> 83).
All 3,672 whole accepted-vs-folded readable Main trace cases match using the
preserved W139 harness (only read; no old producer rerun). Adjacent readability
plus Book conversation/definition tests: 234 passed in 32.19 seconds.

Full readable-only 29-unit corpus regeneration ran in scratch from all
hash-verified W166 drafts, with the candidate module loaded only in child VMs.
Direct impact scan finds WomanToAttract and BookCollecting DoConversation only.
The latter fold drops one literal-false outgoing jump while retaining its else
scope. Drafts, evidence, native conversion reports, SDK, packages, and runtime
remained unchanged until the final promotion recorded below.

First full candidate: all 29 readable stages regenerated; only BookCollecting
DoConversation and WomanToAttract change. Whole generated Woman Main's 3,672
comparisons pass. Book smoke 3 files/0 problems; SickChild 13 files/1 existing
Sister modulo-zero mock failure, with exact baseline/candidate smoke JSON.
Output review found extra nested do blocks, so candidate one is NOT promoted.

Revision two, `readable_style_candidate_v2.py`, keeps required lexical scopes but
inlines a kept else only when its entire body has no local declarations, returns,
labels, closures, strings, or comments. Scope-free nested do wrappers may then
flatten. Preflight Woman output is 491 -> 477 lines and 88 -> 83 jumps, with all
3,672 whole generated Main comparisons still matching. Expanded branch/scope
regressions: 36 pass. Full revision-two regeneration scheduled the three largest
units first while holding shared source unchanged.

Final result: **29 units / 267 readable Lua; only the two intended files change,
265 unrelated readable and all 267 native drafts unchanged.** All 1,525 input
pins verified before promotion. 270 tests and 3,672 whole generated Woman Main
comparisons pass. Typed smoke JSON remains exact against baseline: Book 3/0;
SickChild 13/1 unchanged Sister mock failure (vector begin/end both default to one; equal pointers give zero length).
Report changes occur only in the two changed files. Native evidence/diagnostics
remain intact.

Promoted exact candidate style source, the new 36-case branch regression file,
two readable Lua files and their reports. Root integration: 36 passed.
`promotion_receipt.json` PASS verifies promoted hashes and all other inputs;
only the explicitly reviewed style-source pin is superseded. Book's report also
gains four empty callbackScopes maps from existing builder metadata, already
present in W166 scratch; no Lua effect.

Refreshed ledger: 371 files, zero syntax failures, 857 unresolved, 4,881 jumps
(six fewer), 6,655 native-name occurrences. Status categories unchanged; no
whole-file readability or retail-equivalence certification. Baseline snapshots,
rejected first candidate, tests, corpus, reports and owned diff preserved.
Producers/promoters are terminal and should not be rerun.

## W175: lazy mask-analysis construction (promoted with W174 revision two)

Profiler evidence in W173 found `prune_dead_bitmask_staging` constructing a full
Flow object for each integer assignment before knowing whether a matching group
needs it. Candidate in `work/codex_lua_mask_analysis_w175_20261018/` creates Flow
only for the distinct-target binding proof and reuses it while the input lines
remain unchanged. The function returns immediately after its first mutation.
No safety predicate or lowering rule was weakened.

Against W174 candidate one's exact same drafts, all 267 readable Lua files and
29 complete reports are identical. Adjacent tests 234 pass; then-current chain
tests 29 pass. Real GuildTraining generation (38 Lua unchanged) observed 288.34s
versus 460.27s for the earlier non-lazy run: about 1.60x in this machine's shared
load, not a general performance guarantee. `verification.json` preserves hashes,
results, timing qualification, and comparison. The lazy change is included in
W174 revision two. Combined promotion is complete; `promotion_receipt.json`
pins actual root source and checks AST identity of the tested mask-analysis
function, Flow class and binding predicate. No global cache or weakened proof.

## W176: reviewable bundle (offline candidate verified)

`work/codex_lua_false_chain_bundle_w176_20261018/false-chain-readability-candidate-20261018`
derives from untouched W166. 203 Lua parse; all 208 payload hashes and local
preflight pass. Exactly three payloads differ: BookCollecting master,
WomanToAttract, README. DLL remains c6ef7f50... from W166/W172. No install/game
launch/save changes for this package. Packaging does not add gameplay proof.
Receipt: `work/codex_lua_false_chain_bundle_w176_20261018/package_validation.json`.

Next W177: investigate the Sister smoke modulo-zero path's global definition
string vector at 0x74C/0x750. Classify native/PDB representation and existing
count/index bindings before considering a generic recovery improvement. Do not
clamp mock zeroes or invent successful behavior. No game/runtime lease; all 563
saves remain restored. Root commits/pushes remain outside authorization.


## W177: retail named definition-member evidence (promoted)

Owned scratch `work/codex_lua_script_def_fields_w177_20261018/`. No production
metadata/source/output changes yet. CScriptDef constructor store at F28FB9
selects vtable 012FB5C8; slot18 selects F2D6E0. Slot20 selects F29500, whose
`mov eax,0x1088; ret` supplies retail object size. No RTTI identity claim.
Transfer range F2D6E0..F333B0 SHA256
`e2710eb7d1091926075533eb7f7e5c5d131644d955c07f34d89cd9259e604560`.
The method passes each literal field name with its ESI-relative member address.
ESI is stable except initial receiver assignment, final +0x1084 tail member,
and epilogue restore. All943 named PDB fields have unique native offsets.
One additional TemplePrayerFactorHighest transfer reuses EBX; its earlier
explicit LEA supplies the primary anchor, not a second field.

Read-only inventory revisions retained. Bounded Unicorn execution of every
primary captured setup: 943 fields x2receiver placements x2fill patterns =
3772 cases PASS. Checks helper-boundary ECX/context, stack name/member pointers,
and untouched receiver arena. Helpers/full transfer/live game not executed.
Candidate extractor pins transfer bytes, constructor/vtable/size and native
name-string/address associations; unknown/incomplete evidence fails closed.
Focused tests: five methods/eight adverse subcases PASS, including swapped
strings with the same complete name set and changed executable profiles.

Candidate metadata has943 rows versus928 old guessed rows:15 formerly omitted
fields recovered;606 previously unverified fields directly anchored;11 old
verified late names corrected. Every Lua read still uses its original numeric
runtime address. Sister vector start is0x74C;0x750 is its end pointer, not a
separate named field. Stock snapshot vector decodes to three tokens:
ST_TELLING_YOU_TO_COME, STANDARD_WAVE_INFO, STANDARD_WAVE. Existing smoke's
ReadGlobalGameData returns1 for both addresses, so its vector length is zero.
Earlier W174 proof capsules said both defaults were zero; that explanation is
incorrect, while the recorded identical baseline/candidate smoke result stands.
No clamp, invented default vector, SDK change or native behavior fix is proposed.

Scratch29-unit readable regeneration is running with two workers and1796 input
pins. Acceptance still requires complete syntax/reports, expanded-name numeric
comparison and adjacent tests before any promotion. No game/installation/save
changes or runtime lease; modernization remains free to use the install.


### W177 accepted promotion

Final scratch corpus:29 units/267 readable Lua parse;27 files in11 units change,
240 unrelated generated files exact, all267 drafts retained. Expanding generated
SCRIPT_DEF aliases back to their original numeric values gives identical code
tokens and all non-table comments across267 files.1796 inputs pinned and verified.
1606 adjacent tests +8 production-test candidates PASS;8 normal root integration
tests PASS after promotion. All943 primary native setups pass3772 bounded cases.
Production metadata rebuild exactly matches generated JSON; no snapshot tunable
is substituted for a runtime read.43 owned paths promoted (sources/tests/metadata,
27 Lua and11 reports). Single-use receipts/producers retained; do not rerun.

Current-SDK typed smoke comparisons across all11 changed units pass, allowing
only source-line shifts in otherwise identical errors. Existing problems remain:
Bordello Madame/Magicman g_PresentedItemName free-global reports; Sister and
TraderEscort DarkwoodAssassinSpawn equal-endpoint vector modulo-zero mock paths.
The first smoke comparator handled Lua chunk line numbers but not filename line
numbers; attempt1 retained, comparator corrected, complete rerun passed. No Lua
error identity or trace was suppressed.

Reports also refresh already-existing metadata drift: empty callbackScopes maps,
three parentheses rewrite counters, and scratch draft provenance. These occur in
the accepted W174 baseline corpus too, so they are not attributed to field naming.
Exact per-report deltas in promotion_receipt.json and preexisting_report_drift.json.
All267 root generated outputs match tested scratch, using package selector's
GuildTraining/readable_converter; historical GuildTraining/readable preserved.
Initial generic-path root check was superseded by root_output_verification.json.

Full ledger refresh:371 files/0syntax/857unresolved/4881gotos; all other counts and
statuses unchanged. Audit Markdown is byte-exact to existing ledger, so no rewrite.
Historical W171/W173/W174 current-root hash checks are intentionally superseded
only at W177's listed promoted paths; saved original proofs remain historical.
Metadata scope is direct name/member offset evidence for this x86 retail profile;
PDB type descriptors, helper implementations and full quests are not newly proven.

## W178: native definition-field review bundle

`work/codex_lua_definition_bundle_w178_20261018/native-definition-fields-candidate-20261018`
derives from unchanged W176:203 Lua parse/208 payload hashes/preflight PASS.
19 packaged Lua and README change. Eight changed generated sources belong to
units absent from the parent bundle and remain deliberately unpackaged; exact
list in package_validation.json. DLL remains c6ef7f50, no install/gameplay retest.
W147 remains last broadly runtime-accepted bundle; W172 bounded movie handoff
proof belongs only to the unchanged DLL. No runtime lease or game launched.

Next W179: investigate a generic, bounded definition-vector smoke model using
proven native offsets and decoded stock definitions for Sister and Darkwood
Assassin. Preserve malformed/empty-vector evidence; no modulo clamp or fabricated
live-game proof. Continue existing units only; no root commits or pushes.


## W179: explicit stock definition-vector smoke fixture (promoted)

Owned scratch `work/codex_lua_definition_smoke_w179_20261018/`. New generic
`definition_vector_fixture.py` decodes the twelve anchored Vector_VCDefString__
metadata fields against the stock SCRIPT_DEF/names snapshot. Strict count/size,
native-anchor, name/token, offset/overlap, item-type and source-hash checks fail
closed. ArcheryNormalPrizes is genuinely empty and remains empty. Synthetic
x86 begin/end/capacity words model only vector reads; scalar smoke defaults stay1.
No fixture enters generated Lua, game globals, SDK or bundle.

`smoke_run_unit --definition-fixture <json>` opts into this bounded model. Default
smoke behavior remains byte-exact across all29 registered-unit JSON reports.
With stock fixture, only Sister.Main and DarkwoodAssassinSpawn.Main change:
previous identical-pointer modulo-zero errors -> bounded API-shape smoke success.
Sister201frames/4792calls; Darkwood1frame/33calls. Other functions/files unchanged.
This does not establish native event, timer, animation/sound or whole-quest behavior.

28 regressions PASS before and after promotion: malformed/truncated/oversized
serialized vectors, missing tokens/anchors, overlapping/unaligned/wrong-type
members, wrong item types/names/schema, source drift, invalid indices, genuine
empty-vector modulo failures, and the actual two generated Main functions.
2224 input pins verified; default29 reports unchanged; all generated Lua, metadata,
SDK, DLL and W178 package unchanged. Root fixture CLI reproduces exact fixture;
normal typed Sister CLI13files/0problems with explicit fixture. Source/test/CLI
promotion_receipt.json PASS. Earlier W177's smoke-tool source pin intentionally
superseded; saved baseline reports and generated/native payload proof stand.

Useful commands (current SDK source profile still required for accurate method
registration checks):
`python -m tools.script_recovery.definition_vector_fixture --out work/<task>/vectors.json`
`python -m tools.script_recovery.smoke_run_unit --unit sick_child --definition-fixture work/<task>/vectors.json --sidecar-source-dir work/codex_lua_movie_destroy_reentry_20261016/sidecar_candidate/FableScriptExtender`

Next W180: Bordello's two free-global reports name g_PresentedItemName, which the
existing LuaEntityAPI::MsgIsPresentedWithItem binding explicitly writes. Verify
this host-global contract before changing static analyzer classification; no
Lua/SDK workaround or launch required.


## W180: presented-item host-global classification (promoted)

Owned scratch `work/codex_lua_host_global_w180_20261018/`. Both the old default
and current movie-reentry SDK source profiles implement identical
LuaEntityAPI::MsgIsPresentedWithItem bodies: when presented and text is valid,
sol::state_view writes g_PresentedItemName from a copied std::string, then destroys
the temporary CCharString. Source/body SHA pins and complete body in binding_evidence.
This establishes a declared host-global API contract, not a new retail ABI proof.

Static smoke analyzer now explicitly recognizes that one host global. Arbitrary
g_* variables and near-spelling mistakes remain findings. Four regressions PASS,
including both actual Madame/Magicman generated consumers and negative globals.
All29 runtime smoke JSON results remain exactly equal; only the two Bordello
freeGlobals entries disappear.2230 input pins verified. Combined root integration:
40 tests/eight adverse subtests PASS (metadata, vector fixture and host globals).
No generated Lua, metadata, SDK, DLL, bundle, game or save changed.

Promotion copied the two tested source files, then cp1252 failed writing the
review diff because an existing comment contains a Unicode arrow. Verified both
copied hashes, original backups, all remaining pins and tests; reconstructed exact
UTF-8 owned_changes.patch without rerunning promotion. Final promotion_receipt
PASS records recovery and the intentionally superseded W177/W179 smoke-tool pin.
W178 remains latest offline candidate; default Sister/Darkwood modulo failures
remain unless W179's explicit fixture is selected. No runtime lease/launch.

Next W181: inspect existing registered-unit recovery debt using native evidence.
Arena's F14250 four-byte helper is mislabeled DrawGetEnvironment by BSim; native
caller uses its result as the master-state base for WhisperKilledByHero, already
lowered to GetMasterGameState in Lua. Keep its unresolved pointer-return diagnostic
until identity/representation is proven; do not remove it merely to reduce counts.
Teacher's retained pointer/vtable staging is another bounded candidate.


## W181: low-byte split observation guard (scratch validation running)

Owned scratch `work/codex_lua_low_byte_guard_w181_20261018/`. No production
source promotion yet. Existing fold_low_byte_flags splits CONCAT31's low byte
into a new local, but previously did so even when the complete word was observed.
Synthetic x86 MOV EAX,0x12345678; MOV AL,1 leaves0x12345601; old split left an
Observe(value) reading0x12345678. Bounded Unicorn probe stops before RET and confirms
this mismatch. This is an architectural adverse case, not a claimed retail fault.

Candidate refuses observable whole-word tails, escaped addresses, pre-store byte
reads that would use an uninitialized split local, and prefix observations reached
through backward labels or enclosing loops. Separate literal-colour lifetimes are
checked using the existing colour fold only for analysis; actual pass order stays.
Known byte-only behavior and full input-parameter reads before a byte lifetime
remain supported.17 regressions PASS (14 failed/3passed on original);67 adjacent
flag tests PASS.1973 raw exported functions scanned, only one low-byte pattern
function, and that retail function's raw fold output is identical.

Fresh two-worker full native+readable29-unit generation running with2011 inputs
(including installed retail image) pinned. Compare267 drafts to W166 and267 readable
to accepted W177; parse all534 files. Shared production lowering held unchanged.
Promotion still requires complete corpus/reports and focused current-SDK smoke.
Do not remove existing pointer-return or staging diagnostics to reduce metrics.

Arena sidebar primary evidence: F14250 bytes8B4144C3 load[this+0x44]. CallerF13DB8
sets ECX=EBP, calls helper, compares byte[EAX+0x74] with1. Master metadata maps74
to WhisperKilledByHero. Current Lua already reads GetMasterGameState for this path;
standalone BSim-named DrawGetEnvironment pointer return remains explicitly TODO.
Packet arena_helper_inspection.json records bytes/caller SHA. No Lua/native helper
identity or pointer-return API invented. No game/lease/install/save changes.


### W181 accepted promotion

Fresh29 units/534Lua exact (267draft W166 +267readable W177); all parse.58 JSON
reports exact except scratch readable draft paths.2011 generation/image pins PASS.
Default TraderEscort smoke16files/1existing Darkwood-vector mock failure and
explicit stock-fixture16/0 both exactly match accepted baseline JSON. Promoted
only native_evidence_lowering.py guard call, new low_byte_guard.py and17-case test.
84 root integration tests PASS; original14-failure evidence retained. Review patch,
backups, promotion plan and final promotion_receipt PASS. No generated payload,
SDK, DLL or package change. Existing ledger hash unchanged (371/0syntax/857unresolved).
Earlier native-lowering source hash pins intentionally superseded; original saved
proofs remain historical. Scope: tested canonical decompiler spellings, not a
general C alias/control-flow equivalence proof. No observed retail fault claimed.

## W182: BookCollecting current-DLL runtime gate (window unavailable; not launched)

Prepared scripts only in `work/codex_lua_book_movie_w182_20261018/`, derived read-only
from preserved W172 runtime and W147 Book actions, targeting unchanged W178 bundle.
No runtime bundle copied, lease acquired, saves staged, install edits or game launched.
Preflight discovered `.agents/runtime-lease.json` owned by Texture Archive4GB,
state running, token830e68c63be44a17be48404e18a46f31; live Fable PID18424.
Leave that lease, process and saves untouched. Recheck live ownership and the
modernization/texture handoffs before any future prepare/launch. W182's helper
also refuses any fable* process, copies parent payload by manifest, backs up the
current complete save inventory, and verifies restoration before lease release.

Planned bounded gate: current c6ef7f50 DLL +W178 readability payload; complete
normal teacher donation and nonempty BookReaction/DoConversation using physical
Tab/Yes, verify control/movement and counts, then unload/deactivate lifetime path.
Assisted noon/position/book grant are explicit; do not bypass movie/control flags.
Prior W147 baseline has these checks on DLL445fcfff, W172 covered c6ef only for
ship intro/attacks. Existing W178 package remains offline-only for changed Lua.
While Texture owns runtime, continue useful offline recovery and leave W182 pending.


## W183: definition-string smoke return shapes (promoted while runtime occupied)

Owned scratch `work/codex_lua_string_smoke_w183_20261018/`. Current NoviUnitBindings.h
explicitly declares std::string returns for ReadGlobalGameDataString,
ReadGlobalGameDataStringAt and ReadGlobalGameDataDefStringAt. Old generic Read*
fallback returned numeric0; three focused type/length probes failed. Mock now
returns empty strings for successful generic defaults of these three exact names.
Optional CDefString fixture runs first and retains its strict range/errors.
Numeric APIs and similarly named unknown methods retain their defaults/traces;
static missing-binding checks are unchanged. No stock/live value is invented.

Five regressions PASS (3failed/2passed before),37 root integration tests PASS.
All29 default and all29 explicit stock-fixture reports remain byte-exact across
baseline/candidate;2228 inputs verified. Promoted smoke tool +new5-case test only.
Review patch/backups and promotion_receipt PASS. No generated Lua, metadata, SDK,
DLL or W178 package changed. Prior smoke source hash pins intentionally superseded;
saved historical proof remains available. Declared SDK return shape only, no
native error/gameplay-value equivalence claim.

Runtime recheck still belongs to Texture Archive4GB: lease changed to baseline
control trial tokena2e748680b1943738454dd4a3f1ce591, live Fable PID19128 observed.
Do not rely on old PIDs/tokens; always read current shared lease/processes. W182
remains scripts-only with no staged saves, owned lease or launch.
Next W184 offline: inspect existing native_dispatch_scaffolding's all-or-nothing
unique-name guard against reused Teacher interface/vtable temporaries. Capture
pre-residue lines and prove reaching-definition liveness before considering any
cleanup; retain every ambiguous/observed value and diagnostic.


## W184: Teacher dispatch-stage review (evidence only; no promotion)

Owned scratch `work/codex_lua_dispatch_versions_w184_20261018/`. Captured seven
Teacher lift stages before raw C residue becomes TODO/nil. Main472 lines has
CVar12 interface seed and repeated copies/dereferences; both locals and hoisted
sets contain CVar12. Existing all-or-nothing unique-name prune correctly declines.
Unmasked CFG rejects Main; masking comments makes it parse, but16unresolved native
goto markers encode control flow not represented by that graph. Do not use the
masked graph as a native liveness proof. review.json preserves this explicit gate.
All production dispatch logic, native drafts and diagnostics remain unchanged.

Next W185 narrow readability check: canonical this+4(entity)/this+0x40(quest)
interface-load TODO followed by nil may be retired only if its final readable
private binding has no reads, captures, other assignments or duplicate declarations.
This uses final Lua binding evidence after existing value splitting, with the
existing native interface-field role; it does not infer unrepresented native CFG.
Preserve all other pointer reads and diagnostics. Runtime W182 remains pending
Texture's shared lease; no Lua staging or launch.


## W185: orphan dispatch placeholder after private binding proof (promoted)

Owned scratch `work/codex_lua_orphan_dispatch_w185_20261018/`. Kept native-stage
pruning unchanged after W184's unresolved-CFG finding. New readable helper accepts
only canonical script-interface slots (+4 entity/+0x40 quest), one real private
Lua declaration, exact paired native TODO/nil marker, and no other assignments,
reads, captures or opaque references. Existing lexical binding guard protects
late/out-of-scope/global/shadowed declarations. Other pointer slots and unknown
owner contexts stay explicit. Recognition uses the existing interface-field role;
no pointer value/API invented. Native E54E90 body f8c76fa0... plus four bounded
argument setups establish entity+4 dispatch context, entity+8/true arguments for
slot774 SetThingPersistent. Stop before virtual call; not full engine execution.

Candidate33 guard/integration cases and313 adjacent readability/Book progression,
interruption, snapshot and argument tests PASS (346total);33 normal root tests PASS.
Fresh29-unit/267-readable corpus: only BS_Teacher changes;266 unrelated readable
and267 accepted native drafts exact.2607 inputs verified. Independent token check
allows exactly one unread private nil assignment, its two markers and declaration
rewrapping; all remaining code/comments exact. Teacher23 ->21 diagnostics.
Report preserves original CVar12 field4 statement and unused-binding proof.

First probe's two-round builder lost new evidence when recovery occurred in round2;
kept unpromoted candidate_book, added merge handling and regression, then regenerated
candidate_book_v2/corpus with identical Lua and retained evidence. Initial candidate
smoke accidentally named a nonexistent draft report in readable-only scratch;
explicitly excluded its guessed-signature result and reran using pinned W181 input
report. Typed current-SDK Book3files/0problems exactly matches baseline JSON.

Promoted helper/style/builder,33-case tests, Teacher/report, and ledger (8owned paths).
Backups/review patch/plan/final promotion_receipt PASS. All267 root readable files
match tested corpus. Ledger371files/0syntax/855unresolved/4881gotos/8422generic temps;
other metrics/status counts unchanged. Native drafts retain their original markers.
No whole-file or whole-quest certification; all ambiguous pointer/control diagnostics
retained. Older style/builder current-root pins intentionally superseded; original
proof snapshots retained. No DLL/install/game/save changes during this offline pass.

## W186: orphan-dispatch review bundle (offline verified)

`work/codex_lua_orphan_dispatch_bundle_w186_20261018/orphan-dispatch-candidate-20261018`
derives from intact W178.203Lua parse/208payload hashes/preflight PASS. Exactly
Teacher and README differ. DLL c6ef7f50 unchanged. Manifest records current style,
builder, dispatch-helper and native-lowering hashes, plus W185 receipt. No install
or new gameplay proof yet. W147 remains last broadly runtime-accepted candidate.

W182 prepared runtime now targets W186. Texture journal explicitly released the
shared runtime after its packed-corrected Oakvale trial and restored563 saves;
it is offline for app/near4GiB work. Recheck current lease/processes before prepare.
An observed fabletextures_import_probe was an offline CLI and exited; do not confuse
that with the actual game/title/input desktop owners. No Lua save staged/lease held
as of this checkpoint. Next: claim window only when free, bounded Book donation/
nonempty conversation/control/lifetime gate, graceful owned close and full restore.


## W182 runtime window: current launch

Claimed exclusive Lua W182 lease after peer release; backed up563 current saves,
staged numeric checkpoint, recorded installed exe/qst pins. W186 runtime copy
launched owned PID17448 with c6ef7f50 DLL. Initial UI attempts were too early,
then used a venv without Pillow; switched to installed Python3.14. Repeated
unconditional ShowWindow restoration stalled title input. Read peer texture
journal equivalent finding; made owned gamewin_guarded.ps1 (only restore iconic,
ShowWindowAsync, preserve foreground) and selected it only in W182 runtime.py.
Held1500ms title click reached profile selection. No quest test result yet.
Shared game/tooling untouched; lease remains exclusive through close/full restore.

W182 TERMINAL frontend-only: correct0atlas menu visually confirmed; OCR timed out,
then guarded driver move timed out60s before reaching save list. No checkpoint
load/quest host; no donation/conversation/control/lifetime proof claimed. Graceful
owned WM_CLOSE succeeded; original563 saves verified, three staged autosave files
restored, no new save files, installed exe/qst hashes exact. Own lease released.
Runtime_result.json/save-restoration.json preserve limits. Modernization/texture
may reclaim free window. Continue offline Lua; W147 broad runtime status unchanged.

## W187: current Teacher string-branch revalidation (evidence only)

Owned work/codex_lua_book_args_native_w187_20261018. This extends W104's already
recovered Book21 branch; no new recovery credit. Original E57020 body9f2cfeaf
and original411570 comparator3538aba2 execute the bounded E57324 A/B selection.
261 ASCII strings, null/allocated empty, two stack placements and three fills:
1572native cases/522current draft+readable calls PASS. Added assertions for
callee-saved registers, ESP, untouched argument slot and upper3bytes of the
CONCAT31 spill. ARG1/ARG2 checked only in generated Lua; no native allocation,
map operation, destructor, unwind or whole-helper proof. Diagnostic retained.
W104 original13cases/UTF8/termination evidence stays historical. No code change.

## W188/W190 in flight (offline)

W188 parenthesized low-byte address-escape guard in owned scratch: five new
baseline failures;84candidate/adjacent tests PASS; eight synthetic x86 alias
counterexamples PASS. Fresh29-unit pipeline running one worker; GuildTraining
76Lua and Arena exact so far. No root source promoted until full verification.

W189 short profiling capsule only:946samples/52nonblocking errors crossed
native/readability phases. No evidence sufficient to choose a disassembly-cache
optimization, no code changed.

W190 future bounded Book runtime prepared in owned scratch. Python ctypes/Pillow
desktop driver requires exact Lua W190 running lease/PID, foreground guard,
held clicks and capture containment. Import,5key mappings,4invalid keys and
missing-ownership refusal verified without window/input calls. No game/save
staging or lease yet; actual UI driver remains unverified. Latest texture
journal is retrying c after Lua release/Forge UI; let that lane finish and
restore before any W190 prepare/claim. Sealed W182 attempt remains preserved.

## W188 promoted: parenthesized address escapes

Canonical &(value), &((value)), and intervening comment/space forms now preserve
an earlier escaped storage location before low-byte splitting. Five failures
before;84candidate/adjacent tests and27root tests PASS. Eight synthetic x86
pointer-store cases show why a separate flag must not replace the pointed store.
No affected retail fault claimed; arbitrary C alias/CFG syntax remains outside
this guard. One production line plus10regressions promoted, review patch/backups/
plan/final receipt preserved in work/codex_lua_parenthesized_escape_w188_20261018.
Fresh29units/534Lua byte-identical,58reports exact except draft path,2017pins
verified before promotion. All534root generated files match fresh scratch;
all pins retained except intentional owned guard delta; ledger hash unchanged.
No payload, SDK, DLL, install, gameplay or save changes. Old W181 guard current-
root pin intentionally superseded; its original capsule is historical.

Texture c trial receipt confirms scratchRestored=true/all563 saves restored and
released token67b09526 at20:08UTC; then texture reserved GPU-only194-texture AI
preview. W190 stays unlaunched while that shared lease is held. No GUI/input tests
opened by Lua over the peer runtime. W190 driver/helpers prepared only in scratch.

## W191 promoted: explicit stock conversation smoke fixture

Owned work/codex_lua_conversation_fixture_w191_20261018. Optional export flag
--include-conversations adds the anchored BookReactions member0x4c8. Decoder reads
serialized transfer tags, signed Lines and four independent inline CString lists;
it does not use donor offsets. Native field callF2F77A remains pinned to W177's
transfer body; current SDK snapshot bodies/header and38unchanged prior native-copy
evidence inputs checked. No new native engine coverage claimed.

Stock3602bytes decode25records, five nonempty reactions at2/12/17/20/24 with
7/9/6/10/7lines. All3602 truncations and trailing/tag/count/string faults rejected.
Current generated BookReaction/DoConversation execute all39lines with mock APIs.
Optional harness models deep global copies, owned snapshots and deep returned
copies; unknown state stays empty, empty vectors/index errors are not padded.
No quest scheduler/heap/gameplay/exception proof inferred.

Review caught schema1's older reader silently ignoring the new key. Initial
unpromoted schema1 prototype preserved under schema1_probe and excluded from
current proof. Extended exports now require schema2; old readers reject them.
Ordinary exports remain exactly schema1 and byte/data equivalent. Fresh revised
81tests PASS;2240inputs verified;29default+29legacy-vector smoke JSON reports
exact,29extended reports also unchanged (ordinary callback args still select an
empty record; nonempty39line coverage is the explicit regression). Public default/
optional export CLI exact;81root tests and typed root Book3files/0problems exact.

Promoted4owned paths: decoder, fixture exporter/validator, harness, regression.
Review patch/baselines/plan/final promotion_receipt PASS. All534root generated Lua
still match W188 corpus and ledger hash unchanged. No Lua/metadata/SDK/DLL/bundle/
installation/save/game changes. Previous fixture/harness root pins intentionally
superseded; historical proofs retained. W147 broad runtime status unchanged.

Commands (use owned output path and current SDK source profile):
python -m tools.script_recovery.definition_vector_fixture --include-conversations --out work/<task>/stock.json
python -m tools.script_recovery.smoke_run_unit --unit book_collecting --definition-fixture work/<task>/stock.json --sidecar-source-dir work/codex_lua_movie_destroy_reentry_20261016/sidecar_candidate/FableScriptExtender

## W192 in-flight: inline value-string fixture evidence

Owned work/codex_lua_inline_string_fixture_w192_20261018. Read-only stock decode:
14anchored Vector_VCCharString__ fields/190strings, including40BookDefNames and
25boy/25girl marker names; Book2 is OBJECT_BOOK_STORY_13 with MK_BSAS_BOY/GIRL.
21decoder guards/roundtrip/truncation tests PASS; all3271 prefix truncations
rejected. Scratch only; no exporter/harness integration or root source promotion.
These inline strings differ from the12CDefString token vectors. Current SDK's
ReadGlobalGameDataStringAt returns empty on negative/out-of-range indices;
ReadGlobalGameDataDefStringAt throws. Do not merge their error contracts.
Future integration needs an explicit new schema accepted by new readers and
rejected by schema2 tools, preserving default/schema1/schema2 reports. Scalar
tunables remain ordinary defaults; no broad state/scheduler mock inferred.
W190 remains deferred while Texture holds native-resolution gameplay comparison
lease (PID15080/token7d2a8c14). No Lua game or save staged, no desktop actions.

## W192 complete: opt-in inline stock-string values PROMOTED

Owned work/codex_lua_inline_string_fixture_w192_20261018. Schema3 adds14anchored
inline CCharString vectors/190strings, separately from12CDefString token vectors
and25conversation records. Stock bytes roundtrip; all3271prefix truncations
rejected. Full generated Book reaction tests exercise39lines and the three actual
BSAS/BSAF/BSAL marker layouts. Initial4failed expectations incorrectly assumed
one marker layout; corrected expectations from independent stock decoding,
preserved failure/review artifacts. Production candidate needed no correction.

142candidate and142root checks PASS;2243input pins unchanged before promotion.
All87existing default/schema1/schema2 reports exact across29units;29schema3
reports also unchanged. Legacy schema2 reader rejects schema3; public CLI exact.
Promoted4owned paths: inline decoder, fixture exporter, smoke harness and tests.
All534root generated Lua and readability ledger unchanged; typed root Book3/0.
Review patch, baseline, verification and final promotion_receipt PASS retained.
Earlier W191 fixture/harness root pins intentionally superseded; proofs retained.

SDK StringAt/DefStringAt contracts remain distinct; ordinary inline invalid
indices return empty, unknown fixture offsets error. Scalar defaults unchanged.
This is an explicit bounded mock-value oracle, not native pointer-overflow,
scheduler, full quest or gameplay proof. No Lua/metadata/SDK/DLL/bundle/save/game
changes. W190 waits for current Texture lease (PID17792/token5ae9a3d6); latest
user reconfirmed continuous work while away. Continue offline while peer runs.

## W193 complete: signed integer-state ABI evidence only

Owned work/codex_lua_signed_state_w193_20261018. Compiled current SDK exact
Set/GetStateInt and global integer storage bodies, actual LuaQuestState class/
constructor and exact sol registration with vendored sol/Lua in Win32 Release.
8,251assertions PASS;110source/input pins. 0xffffffff becomes -1 and the
generated Book Init LastBookRequested<0 condition passes. Ordinary range
-4096..4096 plus explicit signed/unsigned word boundaries roundtrip as expected.
Generated Init's40 book names/count are explicit oracle values; noninteger
container/bool/conversation writes counted doubles. This is numeric ABI/caller
evidence, not full initializer binding behavior, engine/host/persistence/timer or
gameplay proof. Namespace/singleton controlled. Overflow observations specific
to this compiler/configuration; no broader C++ overflow portability claim.
Initial link omitted a known CCharString destructor fixture global: archived
failure, added null global, reran clean. No production source/output changes.
No lowering or mock fix warranted. Next useful offline composition: actual
generated Init through current SDK string/flag/conversation storage bindings.
Texture AI matched trial holds current shared lease; W190 remains unlaunched.

## W194 complete: generated Book Init/current SDK composition PASS

Owned work/codex_lua_book_init_bindings_w194_20261018. Unmodified current readable
Book Init runs through exact current SDK integer/bool state, string/byte-flag
vectors, global scalar/inline strings, conversation snapshot/copy bindings,
actual LuaQuestState class/constructor and vendored Lua/sol in Win32 Release.
394assertions/114source pins PASS. Synthetic stock-shaped definition memory:
40book names/25reactions, all39x4conversation strings compared. Returned copy and
native source mutation leave owned snapshots independent; per-state SIDE-TABLE
isolation and explicit release tested. Repeat Init resets donation/flag/bool
state. Count0 shrinks vectors; negative count raises explicit size error after
partial integer-state writes; null globals yield empty snapshots/vectors; valid
definitions recover. No rollback/whole native exception behavior claim.

ASLR slot and native objects are controlled Win32 memory; CString payload read
is a fixture callback. Namespace/singleton controlled. Explicit side-table
release is not actual host destruction or global manager lifetime. No native
constructor, persistence, scheduler, resource construction or gameplay proof.
Prior W147 native evidence retained, not rerun or counted as new native cases.
Initial test TU omitted unordered_set include; archived failure, matched source
include requirements, rebuilt/ran clean. Final verification/artifact pins saved.
No production/generator/Lua/SDK/DLL/bundle/install/save changes. W190 live Book
gate stays pending while Texture matched comparisons own shared runtime lease.

## W195 complete: Book speaker-domain review, no tolerant rewrite

Owned work/codex_lua_book_speaker_domain_w195_20261018. All39stock reaction
speakers are supported:19boy0/20girl0. Reviewed tempting numeric-zero/nil resource
path; did not invent a tolerant fallback. Original E56C30..E56C53 wait block
executed32bounded scenarios:8null-resource inactive-conversation cases fault at
E56C40 reading0, even when ReadingBook=true; supported receiver/control cases
reach expected stop before frame/termination paths. Stack/callee inputs checked.
GSI/task returns and object memory are doubles; no whole unknown-speaker or
engine/gameplay claim. Whole original body SHA cfc9ffbbb4a518749f309f67fcbbc5dc886174503942439de0f4820b606c1a11;
block SHA0343f292b4ddb21917894f5584faffb4e6412d1f7528fca7586c46f820cf81b2.
SDK Get(0) also rejects an invalid handle; earlier W43 proof explicitly only
boy0/girl0. Current stock domain fits; no production/generated/SDK change.
Continue useful composed stock reaction validation while Texture meme matched
runtime lease remains active. W190 stays unlaunched.

## W196 complete: all stock reactions/current SDK storage composition PASS

Owned work/codex_lua_book_reaction_bindings_w196_20261018. W194 exact SDK storage
composition extended with actual stock boy/girl marker vectors and unmodified
BookReaction/DoConversation: all5nonempty reactions/39lines/156modeled frames,
1,256assertions/116input pins PASS. Strict argument-checking engine doubles
check speaker/dialogue/animation/loop tokens, conversation participants, exact
animation boolean vectors, resource IDs, marker selection/teleports and final
pushability/preparation. Conversation lasts2frames/task3; animation argument5
is explicitlytrue. These are doubles, not game engine, physical frames or
scheduler proof. Snapshot and inline getters stay real current Win32 SDK/sol.
Full W194 stock/copy/empty/negative/null/reinit/release assertions included.
No new original whole-function native coverage; earlier W43 remains scoped
separately and was not rerun. W195 bounded unsupported-receiver finding retained.
First build/run clean; receipt, source/evidence/executable hashes retained.
No production/generator/generated Lua/SDK/DLL/bundle/install/save changes.
W190 Book live gate remains open; Texture meme matched trial currently holds
shared runtime token94ecb485/PID5404(wrapper); no Lua UI actions or game staged.

## W190 runtime lease CLAIMED: Book desktop attempt active

Texture matched-comparison lease released; no Fable process remained. Atomic
exclusive LuaW190 lease claimed. W186208payload pins verified, own runtime-bundle
copied, all563current original saves/hash backed up and installed exe/qst pinned.
Numeric checkpoint staged. Preparing current c6ef DLL Book gate; OWNED game only.
Other lanes: defer desktop/game launches until LuaW190 closes/restores/releases.

## W190 CLOSED/RESTORED/RELEASED: title-only, no Book gameplay proof

Owned PID15260 launched W186/c6ef; native startup movies naturally reached title.
All attempted key/click jobs failed ownership/foreground settling BEFORE input.
First screenshot/startup and later title capture succeeded. Read-only focus trace
found the game iconic while modernization terminal/taskbar held foreground.
Changed strategy after repeated failures: hidden serialized Python UI worker,
then owned restore/temporary AttachThreadInput/stable-focus sequence. Both title
jobs still failed before any completed action; did not bypass foreground guard,
minimize peer windows, kill peer processes or send blind/repeated input.

Owned game closed gracefully through WM_CLOSE; no game remains. Both hidden
workers stopped through queued stop commands. Full563original save hashes
verified restored;3staged1234234 autosave files replaced from backup, no new files.
Installed exe/qst pins exact. Shared lease copied to receipt then released.
runtime_result.json is terminal frontend-only; no profile/save selected or Book
questhost, donation/movie/conversation/control/lifetime proof. W147 broad runtime
status unchanged. This producer/attempt is closed; use a NEW owned capsule for a
future attempt after desktop/modernization console activity is coordinated.
Continue existing registered recovery offline; user remains away/continuous work
authorized. Other lanes may claim the now-free runtime window.

## W197 complete: pinned29-unit mock triage

Owned work/codex_lua_smoke_triage_w197_20261018. Reused W192schema3 reports only
after2243source/evidence pins and explicit promoted-file substitutions verified.
955exercised callbacks allok,0unknown methods. Only2pending callback annotations:
Arena Main/InitialiseVariables call InitialiseArenaRounds. This is cached mock
triage, not fresh execution or whole callback/quest coverage. No sources changed.
Initial inference that the Sept26 Arena patch remained outside the current SDK
was incorrect; W198 confirms registration already present. DLL/runtime status
must be established separately. Next: current-header compile and
snapshot checks in NEW owned output, preserving pending/install status. Do not
pretend installing the old patch or masking pending diagnostics is proof.
W190 game/worker cleanup terminal;563saves restored, lease released.

## W198 in-flight: correct stale included-header registration diagnostics

Correction to W197 inference: current SDK DOES already include NoviArenaRounds.h
and call RegisterNoviArenaRounds(quest). Pending flag was shallow source discovery,
not absent integration. Historical staging runner refused already-present hook;
new owned current-header compile/probe passes unchanged:47pins, full SDK register
instantiation, portable synthetic Arena checks. Portable/current headers differ
only CRLF/LF, independently reviewed; no SDK patch or DLL build/install made.

Scratch candidate follows local quoted headers and called Register* helper
functions, keeping historical seed-file extraction. Excludes uncalled/ambiguous
helpers, comments/strings/raw strings/read-only indexing/outside profile paths;
include/call cycles terminate. Current source method set grows1272->1273, only
InitialiseArenaRounds; historical default profile set exact. 18new cases PASS
(2fail/16pass baseline),160candidate integration tests PASS. Full29unit default/
schema3 before-after smoke comparison running; no root source promotion yet.
This remains static source inspection, not C++ preprocessing/liveness or evidence
that a DLL was built/installed or Arena gameplay executed. Keep runtime gate open.

## W198 complete: included registration source discovery PROMOTED

Owned work/codex_lua_arena_binding_w198_20261018. Promoted3owned paths: new static
local registration-helper discovery, smoke integration and18regressions. Current
SDK already registers Arena; no API implementation/DLL/installation change.
18candidate cases (2fail before)/160candidate+160root integration checks PASS.
Fresh29units xdefault/schema3 before-after comparisons PASS: only4stale pending
annotations removed (Arena Main/InitialiseVariables in each mode),56unrelated
unit reports exact; ALL runtime results/calls/traces exact.2288input pins verified.
Current source methods1272->1273, onlyInitialiseArenaRounds; historical default
source-profile method set unchanged. Root Arena16files/0problems in both modes,
JSON exact to tested candidate. All534generated Lua/readability ledger unchanged.
47current SDK/header inputs verified for unchanged full registration compile and
portable synthetic snapshot runtime check. No new native/whole Arena runtime proof.
Static helper discovery intentionally is not C++ preprocessor/liveness analysis
or evidence that a DLL was built/installed/executed from the selected profile.
Review patch, backups, ready verification and final promotion_receipt PASS saved.
W192 smoke-module root pin intentionally superseded; historical reports retained.
W190 remains closed/restored/released; continuous Lua recovery proceeds offline.

## W199 complete: seed-registration lexical audit, no further fix

Owned work/codex_lua_registration_lexical_w199_20261018. All1272legacy seed-file
method names correspond to lexical code assignments; no comment/string/raw-text
or read-only-only names, no assignments missed by the old pattern.3SDKsource
pins verified. W198 preserving seed extraction therefore hides no observed
lexical discrepancy in this current profile. No source changes warranted.
Receiver ownership, C++ preprocessor/conditional registration and DLL/runtime
availability remain outside this lexical audit. Continue existing native/data
evidence for Arena rather than widening checker policy without an actual case.

## W200 complete: stock Arena values/current SDK initializer composition PASS

Owned work/codex_lua_arena_stock_w200_20261018. Strict tagged decoder reads
8rounds/23waves/37creature groups from3351stock bytes; entry597 raw field matches
metadata, byte roundtrip and independent schema boundary match.9decoder tests
PASS including all3351prefix truncations, tags/counts/bools/UTF8/empty/signed and
independent declared-versus-actual counts. Donor offsets not used. Raw SHA
9762a2f675adde7a9219d4c4b1ea629ad0bfd7459f2a32f5c90fc1ddf15f08e8.

Actual current RegisterNoviArenaRounds/Read/Store, real LuaQuestState int/bool/
string bindings and unmodified generated InitialiseVariables run in Win32/sol:
270assertions/122input pins PASS. All stock flattened values +20crowd tags checked.
Native source mutation leaves owned state unchanged. Malformed/null/overflow/
missing-CString API failures precede state writes; empty snapshot resets count
but leaves old flattened keys (count-bounded access), valid recovery succeeds.
ASLR/native object memory and CString read callback/namespace/singleton controlled;
no actual native vector constructor, host lifecycle, persistence/combat/gameplay.

Original creature-copy VALUE TAIL F25F1E..F25F49 executes744cases:37stock scalar
pairs+25signed boundary pairs, self/disjoint,2stack places/3fills. Actual scalar
loads/stores/receivers/stack/callee restoration pass;2CString assignment calls
are pointer-value doubles. Base-definition copy deliberately outside boundary;
no inherited pool/identity, native refcounts/allocation/destruction/full-vector
proof. Whole57B body SHAe01481ca446f9310754d274d2a2810af7a1997e14b0264b8fc864f0778401cd3;
tail SHA99980baca552d918f6f80f12c8f06a2ac96cb9850b8ecfc698705715b74b5dad.
Pinned CScriptDef transfer setup F332D3..F332E1 confirms ArenaRounds +1044 directly.
Initial inspection helper shadowed stdlib inspect; renamed to inspect_stock.py
then byte/native checks ran clean. Decoder/compiled inputs unchanged. No root
production/generator/Lua/SDK/DLL/bundle/install/save/game changes; standalone EXE
only. Next useful existing-unit gate: explicit opt-in Arena value smoke snapshot
from this validated stock data, preserving schema1/2/3 and native counts.

## W201 in-flight: explicit Arena schema4 fixture; empty-world limit retained

Owned work/codex_lua_arena_fixture_w201_20261018. Scratch only. New
--include-arena-rounds implies schema3 value families, adds schema4 ArenaRounds
and a scoped Rounds_* typed mock state initialized through InitialiseArenaRounds.
All234stock flattened values/8rounds/23waves/37groups preserve actual/declared
counts, signed scalars and empty vectors. Ordinary scalar/default/mock policies
outside Rounds_* unchanged. Old schema1/2/3 exports exact; old reader rejects4.
41new cases/201candidate integration checks PASS, including3351truncations.

Fresh29units x4existing modes:116JSON comparisons exact,2289pins. Schema4 changes
only Arena PlayWave, which newly reports modulo-zero with empty ordinary world
mocks (no ArenaSpawn marker list). This NEW-MODE failure is retained explicitly,
not relabeled baseline or repaired with invented counts/objects. All28unrelated
schema4 reports exact. API qualification/help state no level spawn-marker data.
Original bounded F1FADB..F1FAF4 increment/IDIV block:222cases,216valid-index positive
count matches and6zero-divisor faults. Whole PlayWave hash e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc;
block dba74957b30d0c647a4173ea4dd3167e2d10d60ae74cf10179c00694dc98034a.
This is NOT a whole zero-world native run or claim about actual Arena levels.

Separate EXPLICIT synthetic world controls with1/2/4markers, integer-counter/HUD
doubles and stock definition fixture reach exactly5hornet creation calls/stock
HUDcounter and correct alternating positions, intentionally stopping at HUD.
They do not change the default definition-only fixture or prove engine/combat.
Candidate promotion review must preserve this known diagnostic and boundaries.
No root source/output/SDK/DLL/install/save changes yet; W190 stays closed/restored.

## W201 complete: opt-in Arena definition fixture PROMOTED, world limit retained

Owned work/codex_lua_arena_fixture_w201_20261018. Promoted4owned paths: strict
Arena decoder/validator/flattening, fixture schema4 exporter, scoped Rounds_*
mock state and41regressions. 201candidate+201root checks PASS;2289pins verified.
8stock rounds/23waves/37groups/234typed values; independent declared counts,
signed scalars, actual lengths and empty vectors preserved. Snapshot overwrite
retains other flattened keys like current SDK. Existing unrelated state/scalar
mock defaults remain unchanged; schema1/2/3 exports exact, old reader rejects4.
Fresh29units x4OLD modes=116JSON comparisons exact. Schema4 only changes Arena
PlayWave;28unrelated reports exact. New schema4 report retains1PlayWave modulo0
error with no ordinary-world ArenaSpawn markers: NEW MODE LIMIT, not a baseline
failure or whole-game bug. Never invented marker counts or repaired empty lists.
Explicit1/2/4marker/counter/HUD mock scenarios reach5stock hornet creation calls,
correct alternating positions and stock HUD, intentionally stop at display.
Native increment/IDIV block222cases (216positive matches/6zero faults) is bounded;
not a whole zero-world native PlayWave execution or stock-level evidence.

Root public schema4 export exact; Arena schema4 report16files/1known world-context
problem and old schema3 report16/0 both equal tested candidates. All534rootLua and
readability ledger unchanged. No generator/metadata/SDK/DLL/bundle/install/save/
game changes. Prior W198 smoke and W192 fixture root pins intentionally superseded;
historical capsules retained. W190 closed/restored/released; broad W147 runtime
acceptance unchanged. Next: bounded explicit-world stock wave scenarios and
native operand recovery only where a genuine lowering discrepancy is found.

Export: python -m tools.script_recovery.definition_vector_fixture --include-arena-rounds --out work/<task>/stock.json
Smoke: python -m tools.script_recovery.smoke_run_unit --unit arena --definition-fixture work/<task>/stock.json --sidecar-source-dir work/codex_lua_movie_destroy_reentry_20261016/sidecar_candidate/FableScriptExtender
This supplies definition values, NOT level spawn markers; the bare smoke reports
the documented PlayWave empty-world error. Do not interpret it as a gameplay run.

## W202 complete: all23stock wave spawn/HUD prefixes under explicit world doubles

Owned work/codex_lua_arena_stock_waves_w202_20261018. Unmodified readable PlayWave
with promoted schema4 values:23actual round/wave pairs x1/4small-marker contexts
=46bounded cases PASS,13source/data pins. Explicit1big/2multiple-big marker actors,
integer-counter/countdown-timer/HUD doubles. Strict creature-type order/count,
native-shaped position branch and each stock HUD tag/count/update checked.
Stops intentionally at DisplayQuestInfo BEFORE combat; no real creatures,
stock level geometry, scheduler, full native function or gameplay evidence.
Big-marker choice remains exactly the current/native source behavior; no guessed
"fix" based on an assumed arena layout. Default definition-only empty-world
diagnostic remains. No production/generated/SDK/DLL/install/save/game changes.
Initial driver template accidentally replaced HUD/error identifiers and failed
to load; archived source/pins/log. Delimited tokens + explicit compile check
corrected the driver; no conversion change, all46cases clean after correction.
Next existing-unit gate: combat counter updates/first-counter replacement/UI
cleanup under explicit injected death/additional-creature events, bounded before
round advance/reward. Do not equate event doubles with ArenaEnemy/engine behavior.

## W203 complete: stock wave counter events/UI cleanup under explicit doubles

Owned work/codex_lua_arena_counter_events_w203_20261018.23stock waves x1/4marker
contexts x0/3injected additional group0 creatures=92bounded scenarios PASS;
12input pins,46first-counter replacement cases. Unmodified PlayWave/schema4
stock definitions + W202 explicit actors/counters/timers, Arena-loaded=true.
Injected events decrement each active group per frame; optional first-tick
additional births update TotalCreatures_0/ExtraCreatures. Strict live counter
IDs/counts, old first-ID retirement/new ID creation, ExtraCreatures clearing,
final zero updates and single retirement of all counters checked. Stops at
DisplayQuestInfo(false), BEFORE round advance/reward; timer/host teardown later
is not claimed. These are injected counter events, NOT actual ArenaEnemy death
callbacks, engine scheduling/real births/combat or gameplay. No source changes
needed; no production/generator/Lua/SDK/DLL/install/save/game changes.
Next inspect actual ArenaEnemy event handling or post-wave state/reward native
evidence before extending the composed scenario; do not invent engine behavior.

## W204 complete: actual ArenaEnemy Lua callbacks + original native failure caller

Owned work/codex_lua_arena_enemy_stock_w204_20261018.37stock creature groups x
hero/nonhero=74generated Main/OnPredicateFail cases PASS (11pins). Per-case
coroutine reaches post-initialization frame, resolves last matching def group,
sets faction/optional exact yellow health bar, failure decrements correct counter,
adds stock DeathScore only for hero, retires big health bar; controlled termination
then drains Main. Actor/world/messages/UI and host ordering explicitly doubled.
Default sandbox omits coroutine; initial74driver errors preserved, owned ONLY
test-harness facade adds bounded coroutines (20k instructions); no root harness
change. No native Main or actual host/death predicate/scheduling claim.

Entire original147B failure CALLER F1A8F0..F1A983 executed1332cases:74stock cases x
2stack places x3fills x3message-return high words. Counter/score stores, exact
CString literal/message/UI arguments/receivers, low-byte return, memory footprint,
stack/callee registers pass;108health removals.9pins. CString ctor/dtor, message
and UI calls are narrow doubles, not engine/string ownership. Native body SHA
3d52ef8ad46a767a812daeea304651d878e15023efa1bdd8c3409f7539abaec7.

Initial native discrepancy was TEST FIXTURE code caching: same Uc's immediate
stub bytes read back MOV EAX,1 but executed old return0. Failure/source/pins
retained. Immutable code reading a mutable data word, with each observed return
asserted, fixes fixture and passes1332cases (local Unicorn2.1.3). Not a Lua/game
bug; no production fix warranted. Documented in pipeline gotchas. No root code/
generator/Lua/SDK/DLL/bundle/install/save/game changes. Next evidence review:
entity constructor/pre-Main failure contract or post-wave state/reward boundaries;
do not fill unknown constructor state based on a plausible default.

## W205 complete: ArenaEnemy constructor boundary, no default invented

Owned work/codex_lua_arena_enemy_ctor_w205_20261018. Current finder identifies
factoryF1A810 via actual final vtable012F9C50 (bsim TriangleStripifier name not
class evidence).145B SHA360bfd24d21b12ea818e2c3c2f8f03e17dca09db3434a3386720a984bf5785be.
Existing skipped-call/tainted constructor emulation returns no writes/constants
for private health index1C/big flag20/group24. Init F1A8C0 is RET only. Thus no
proven scalar default is missing from generated Init under this evidence model.
Synthetic fresh module Init->OnPredicateFail before Main errors on uninitialized
local group; actual host scheduling/reachability and allocator/member-call
initialization are NOT established. Do not infer retail zero state or add a
plausible default to make an unproved schedule pass. W204 proves post-init
ordering only. No production/source/Lua/SDK/DLL/save/game changes. Keep pre-Main
host ordering as an evidence boundary, not a claimed live defect.
Next bounded existing-unit candidate: enemy hit-priority/cooldown crowd events
or post-wave state/reward branches, with original-byte comparison before fixes.

## W206 complete: ArenaEnemy hit priorities and integer cheer cooldowns

Owned work/codex_lua_arena_hit_priority_w206_20261018. Actual generated Main
passes1024 cases: all128 combinations of seven hit predicates, eight signed
integer timer values MIN/-1/0/1/8/9/10/MAX. Exact short-circuit query/effect order,
crowd score, sound/target receivers/arguments and controlled termination pass.
Original hit iteration F1ACB5 through first NewScriptFrame boundary passes18432
cases: same1024 x2stack placements x3fills x3message-return highwords. Native
GetTimer returns EAX signed integer (JG0 / CMP8,JG), not a floating timer domain.
Native Main1888B SHA3e90bbdcea7944a862f6d7ad5ee62e5db058270d56d98655628917cd652dc9b3.
Native flourish/decap suppress lower-priority paths even during cooldown; Lua
agrees. Score writes, receiver/literal/argument stack shape and boundary balance
checked. API calls/strings/Thing temporaries/messages are explicit doubles;
not original initialization, engine behavior, scheduler, actual host or gameplay.
Initial native driver read heal argument14 as a pointer before decoding its
two-argument shape; preserved source/log/failure, corrected driver only. No
executable bytes rewritten between cases (W204 cache pitfall avoided).
12Lua/4native pins verified; all534 generated Lua and readability ledger exact.
No root code/generator/Lua/SDK/DLL/payload/install/save/game changes. No runtime
lease or game present at read-only check; modernization lane remains busy with
native worker/installer builds. Keep launch coordination/quiet desktop gate.
Next: bounded active-thread termination boundaries or post-wave reward branches.

## W207 complete: ArenaEnemy hit-loop termination boundaries

Owned work/codex_lua_arena_hit_termination_w207_20261018. Actual generated Main
passes4096 cases: W206's128hit masks x8signed timers xterminate-on-guard1/2/3
or remain active. Original bounded post-init iteration passes36864cases, same
4096 x3stack fills x3message/termination highwords. Exact independently modeled
guard/query/effect prefixes, score and no later effects after termination pass.
Native normal terminating epilogue restores four saved registers and exact return
stack; active cases stop at first frame API. No executable code rewritten.
9Lua/4native pins verified. No driver failures. Original Main SHA unchanged.
Termination/helper/string/Thing/message/timer APIs are controlled doubles, not
actual host scheduling or engine ownership. No production/root Lua/SDK/DLL/
payload/install/save/game changes. W206's current534Lua/ledger baseline retained.
Next: PlayWave end-round question caller, where current emitted call uses an
unresolved scratch argument; recover original ABI/operands before any fix.

## W208 complete/promoted: Arena end-round question operands

Owned work/codex_lua_arena_round_question_w208_20261018. Found generated caller
shifted Yes/No/empty/nil into four string operands and used returned question as
bool. Native F2104A..71 leaves pending yes/no/empty/true across hidden-return
GetEndRoundQuestion; returned CString EAX is FIRST question argument. Existing
SDK binding already accepts four strings plus optional bool; no SDK change.
New native_reviewed_arena_question.py recovers only exact lifted call, guarded
by original PlayWave9262B and helper171B SHA, original literals and single helper
evaluation. Three converter lines wire post-lift recovery. No hand Lua edits.
Caller/helper SHA e6c92c85661ca4ea5e0e31459825a85c68ef90a21fb10542cf6ebc7347ef72bc /
b27e9860e22f1341a249fd14f967fc08ca3e0c0fa6803592397155dc56fa9ad1.
Original caller F21012..A1 plus entire helper/jump table passes78 cases. Exact
four text args/bool/hidden return/stack and temporary destruction checked with
CString/API doubles. Full generated PlayWave reaches question in first7stock
rounds x13signed multiplier values:91baseline mismatches/91candidate matches.
World/combat/timer/resource/fanfare/prize/UI boundaries controlled, stop before
answer. Native UI/host/lifetime/gameplay not proved.
Initial raw-source inline recovery still shifted arguments; preserved candidate,
replaced with post-lift recovery. Initial guard-test IDs embedded whole source,
exceeding Windows32767environment limit; preserved log, explicit short IDs fix
driver.15scratch guard tests pass;27portable root tests pass, including emitted
helper+caller args for all13values. Existing Arena suite2567PASS in600.38s.
First generation covered Q_Arena only; detected separate Holding companion
BEFORE promotion. Completed fresh baseline/candidate FULL23-file package; only
Arena.lua draft/readable differ,44other package Lua exact. Full-package default,
schema3 and schema4 smoke JSON exact; documented empty-world schema4 diagnostic
retained. Fresh Gameflow+Book12files exact. Fresh ROOT regeneration46files exact.
All534corpus checked: only2Arena files changed/532others exact. Current534pins
in current_corpus.json supersede W188 only for those2files. Ledger371syntaxclean,
855unresolved/4881gotos/6655native/4717machine/8421generic (one fewer generic).
Promoted converter/helper/test, generated2Lua/reports and ledger. No root commit/
push, SDK/DLL/bundle/install/save/game changes. W186 remains latest offline bundle;
Book runtime gate open; quiet desktop/modernization coordination still required.
Next W209 inspects GivePrizeFund's defined outside0..7 empty-string fallback;
native2808cases and generated156cases identify nil second line only outside the
intended8multipliers. No claim that out-of-range state is reachable in gameplay.

## W209 in progress: generic conditional string-copy storage

Owned work/codex_lua_arena_prize_fund_w209_20261018. Native whole GivePrizeFund
612B SHA11fc8c6894f26d6aad837535adbc96a29a2b7af84854968bc75821ad09699b1d plus
original jump table passes2808cases/156logical controls. Full caller executes,
string/hero/conversation/frame/termination calls doubled. Exact strings, gold
stores incl signed wrap, low-byte results, destruction, stack/register restoration
checked.13multipliers x3initial golds x4conversation/termination plans;2stacks/
3fills/3highwords. Intended0..7 cases generate matching calls; outside range,
native constructs BOTH lines empty and bypasses assignment. Current generated
second line is nil (96valid matches/60fallback mismatches). No reachable-gameplay
out-of-range state is established.
Generic candidate extends lifter's materialized string destinations to copies
from mutable locals across supported labels. Constructor value persists when
copy skipped, and copy value survives later source writes. No quest-specific
default added. Existing GivePrizeFund-only candidate matches all156native traces;
full Arena regeneration changes only that function and body equals exercised
candidate.72focused candidate tests PASS vs60fail/12pass baseline;230adjacent
tests/28subtests PASS. Initial focused fixtures used untyped actor operands,
then incorrectly assumed music's first operand was a string. Both versions failed
argument classification, not semantic tests. Replaced driver consumer with an
explicit one-string ObserveString test boundary; final72 cover literal/default
construction, empty/nonempty first source and later source mutation. Old drivers/
logs retained. Native zero-filled assignment boundary faulted before callback;
immutable RET sentinels at external boundaries fix probe, no per-case code writes.
Fresh29unit corpus running sequential hidden children with2028pinned inputs.
Arena changes only draft/readable quest; GuildTraining exact so far. Parent
tool session95577, corpus.log/corpus_progress.json. Do not alter pinned root code
or candidate_setup/candidate_lift/corpus.py while it runs. No W209 root/source/
Lua/SDK/DLL/payload/install/save/game changes. W208 remains current promoted root.
Next: finish corpus, review ALL changed functions/reports, then smoke/regression
checks before any promotion. No game launch while desktop/modernization gate open.

## W209 complete/promoted: conditional CString storage and scoped local copies

Owned work/codex_lua_arena_prize_fund_w209_20261018. Generic lifter now identifies
new nonliteral copy destinations across supported labels, materializes their
constructor values, and permits copy construction from those newly modeled
locals. Existing literal-assignment recovery paths remain unchanged. No quest-
specific defaults or hand Lua edits. Arena outside0..7 prize second line and
TourGuide outside-sex1/2 first-dialogue line now preserve native empty string.

Native Arena whole612B caller2808cases/156logical; actual generated candidate
matches156traces vs96valid matches/60fallback failures baseline. Native Guild
repeat-dialogue tail E923FC..E924EC9600cases/1600logical: CString identity/null/
length comparison, RNG, flags, last string/output copy and ABI restoration;
root and final candidate agree, first candidate returns wrong empty string.
Native helper1484B SHA8360aa3c52638c0b041992391957f7a83dc135fa9bc696ab486a67f988eba4ba.
TourGuide original prologue +actual15B default CString ctor and EE506B..EE50FC
fallback caller90cases. Actual constructor zeroes value and increments diagnostic
counter (including wrap); correct same stack CString reaches line API.10full Lua
fallback controls show nil baseline vs empty candidate. Sex1/2 raw parent-array
reads remain UNRECOVERED; no guessed field data supplied. All other native APIs/
string primitives are explicit boundaries, not actual engine ownership/host/UI.
Sister240owned guard/frame/external-parent-flag cases have equal clean traces,
228reach changed string lifetime. Complete timer arguments compared. STS Briar
Rose and Chicken Spectator readable changes are exact alpha renames; other draft
initializations either map to reviewed readable changes or readable output exact.

First candidate full29 completed but exposed Guild copy-constructor gap. Wider
copy support passed150tests/native Guild controls yet correctly failed existing
ChickenMan x87 luaSha256 gate. Pre-guard capture isolated only two previously
unresolved copy sites plus local declaration. Narrowing to newly classified slots
restores EXACT certified preimage0d207ed3dee983fc61db4eb54c39ad2cd3d3818f099625093df0eaf2a306ddde.
No certificate weakened or metadata hash bumped. Versions/logs/failed outputs
retained. Old72-case checkpoint test restored byte-exact to its recorded digest
in checkpoint_72_case_test.py; extended150case test is current.

Final v4 converts all29units/534Lua;17changed (7readable/10draft),517exact to W208.
Every changed function reviewed. Readable reused only when fresh draft Lua AND
complete conversion JSON equal a prior verified generation with pinned readable
tools; provenance per unit. Fresh ROOT conversion independently matches all29/
267drafts and reports; all534root Lua match final candidate.150focused +407root
tests/28subtests PASS (230adjacent included).2031candidate inputs verified.
Seven affected units xdefault/schema3 plusArena schema4:15report comparisons,
14raw JSON exact. SickChild default's EXISTING missing-definition modulo0 moves
line75->76 only; all other fields/calls/traces exact. Schema3 exact; do not hide
this baseline diagnostic. No new unknown method/global or mock-world defaults.
Ledger371syntaxclean/855unresolved,4884gotos,6659native,4717machine,8438generic;
extra explicit stores/joins preserve values; statuses42recovery/179readability/
150human unchanged. current_corpus.json is new534file reference, superseding W208
only for17reviewed Lua paths. Promoted generic lifter/new150case regression,
17generatedLua and affected reports/ledger. No commits/pushes, SDK/DLL/install/
save/game changes. W186 remains latest offline payload until W210 refresh checked.
Next: stage Lua-only W210 bundle, preserve c6ef DLL/parent/install pins, then
continue existing-unit recovery while runtime desktop coordination remains open.

## W210 complete: accepted string-storage offline bundle

Owned work/codex_lua_string_storage_bundle_w210_20261018.
Final string-storage-candidate-v2-20261018 derives from W186.203Lua parse and
208payload hashes/standard CLI preflight PASS. Five existing Lua payloads refreshed:
Arena quest (W208 question +W209 prize), Guild-unrelated STS BriarRose/Spectator
alpha names, TourGuideFollower empty fallback, SickChildsSister constructed string
lifetime. README also changes; other202payload hashes exact, including c6ef sidecar,
stock FSE, launcher and retail override configuration. Parent208hashes/manifest
and installed EXE/QST pins exact after assembly. No installation/launch/save writes.
GuildMaster quest/shared helper are verified ROOT outputs but absent from the
accepted parent override set; intentionally not added to this payload. First
builder assumed they were present and stopped at assertion after a partial copy;
initial source/log and incomplete directory preserved. New v2 destination used,
no overwrite/deletion. W147 broad runtime acceptance/W172 bounded lighthouse proof
unchanged; W190 frontend-only closed/restored/released and Book gate still open.
Bundle/manifest hashes and exact six-file delta: package_validation.json and
verification.json. Latest payload supersedes W186 for future coordinated launch;
do not reuse the closed W190 attempt. Next existing-unit recovery: TourGuide's
sex1/2 parent CString-array operands, with original native/definition evidence.

## W211 in progress: TourGuide parent CString-array reference recovery

Owned work/codex_lua_tour_parent_strings_w211_20261018. Scratch-only candidate;
no root generator/generated output promoted yet. Main EE4DD0 original1946B hash
32b145c4ff78fed5285c6c375b23a0d62c9289b41cee3a44c9f3e7bdb5d61d9b.
Parent pointer entity+14, male array parent+13c/female+150, stride4/count5.
Model read-only array-element addresses as state keys; defer existing state-string
getter until native CString consumer at EE50AB shared join. This preserves branch
selection and reads after intervening writes; no public SDK API or definition copy.
Unknown alias uses/escapes, mixed state ownership and ambiguous metadata refuse
rewrite.34focused adverse tests PASS.60native first-conversation caller cases
(two sexes/five indices/two stacks/three fills) plus160full Lua controlled cases
PASS; baseline normal paths fail nil arithmetic, candidate returns correct tags.
Actual prologue/default ctor execute, CString copy/guard/conversation APIs doubled.
Original Init ten push literal/lea destination/call assignment triples match
generated stock state keys/tags; static evidence separately pinned. No engine,
resource lifetime, scheduler or gameplay claim. Full29fresh draft/unrelated audit
and adjacent tests running; preserve corpus/adjacent sessions until completion.
Runtime lease currently belongs to Texture AI GPU preview; no game/UI/save writes.

## W211 complete: parent CString-array reference lowering promoted

Full29 fresh conversions PASS; only TourGuideFollower draft/readable change,
532other Lua exact. All other complete conversion reports exact, permitting
readable reuse with pinned toolchain. TourGuide readable regenerated fresh;
all other eight TourGuide Lua exact.34focused adverse +321adjacent/31subtests
PASS;68root tests PASS. Fresh root10TourGuide Lua and conversion report exact,
all534root Lua match new current_corpus.json. No native certificate weakened.
60native caller cases/160first-dialogue Lua +24termination +128reentry controls
PASS. Three-conversation controls cover64sex sequences (0/1/2/3), random0/32767/4,
parent state changed at each yield. Valid sex rereads fields; other values retain
prior copied text, initially empty. Six termination guards release/deregister
exactly once before getter/conversation. These use explicit external doubles;
no engine/resource lifetime/scheduler/gameplay claim. Initial reentry driver
syntax typo fixed; failed log retained. Baseline Lua archived by promotion backup;
behavior drivers select it after promotion rather than claiming new root fails.
Original Init span52e62258d081c260896f5519b7dfdfa94c5b927adf0455fdeb618f5048788041
contains ten decoded literal/destination/assignment triples matching stock keys.
Default/schema3 smoke JSON exact. Ledger371/0syntax/855unresolved,4884gotos,
6657native/4717machine/8434generic; statuses42/179/150 unchanged. Promoted generic
native_member_string_refs module, two-line pipeline hook,33case root regression,
two generated Lua and affected reports/ledger; verification.json records receipts.
No commits/pushes, SDK/DLL/install/game/save changes. W210 remains latest verified
offline bundle until W212 refresh; runtime lease owned by texture GPU preview.
Next stage accepted W211 follower into new W212 bundle preserving208payload/203Lua
and unchanged DLL/override set, then continue registered-unit native recovery.

## W212 complete: TourGuide parent-string offline bundle

Owned work/codex_lua_tour_parent_bundle_w212_20261018.
tour-parent-strings-candidate-20261018 derives from accepted W210.203Lua syntax,
208payload hashes and standard novi_local_test CLI preflight PASS. Exactly two
payload changes: NoviCompatibility/V_TourGuide/Entities/TourGuideFollower.lua and
README.md. Every other206payload exact, including c6ef sidecar, stock FSE,
launcher and override set. Parent208files/manifest and installedEXE/QST pins exact.
ManifestSHA6dd587a5db68ad14c17139852cf4972fd70fc75ab77cd69ac68ed4cacfb628fe.
No installation, game launch, save writes or gameplay acceptance. GuildMaster
remains outside inherited override set. W147/W172 evidence unchanged; W190 closed
frontend-only attempt remains sealed. Next coordinated runtime capsule must use
this new payload, not reopen W190. Texture GPU preview owns shared runtime lease;
Book gameplay gate/desktop coordination remain open. verification.json and
package_validation.json retain exact payload/parent/pin receipts. Continue offline
registered-unit recovery; inspect TourGuide MoveToNextWaypoint loop/target update
and termination evidence without changing current accepted resource ABI.

## W213 complete: TourGuide waypoint search helper bounded equivalence

Owned work/codex_lua_tour_waypoint_w213_20261018. Original EE6850..EE6A36 span
487B (484 instruction bytes +3B alignment), SHA7159a93be10d1f5a9e9a07289266540c10ed5b3084605ed4873228e801e6ff0a.
5040native cases:420logical x2stacks x2fills x3reference-count baselines. Whole
entry/return execute; callee stack8purge, saved registers and all synthetic
reference counters restored. Null/dead/alive initial target, counter0/3/16/17,
five lookup plans including repeated/dead/switching references and18missing
waypoints then success. Seven guard stops including neverterminate; exact frame,
counter increment/wrap stores, lookup addresses/indices, alive/position/move
operands match420current-Lua controlled traces. Mutable copied-Thing holder double
models existing AssignThingOutput updating the object behind Lua aliases; all
tracked outputs retired. Original inline same/different/null-control copy and
reference operations execute; engine APIs, termination/frame, temporary initializer
and final destructor are explicit doubles. Control baselines1/9/7ffffffe keep
zero-count delete paths outside this proof. No real resource lifetime/scheduler/
engine/gameplay claim. Fixed alive state per synthetic identity; dynamic alive
transitions and malformed/out-of-range counters remain separate boundaries.
No production correction required. All534current Lua hashes and ledger exact to
W211. Native metadata/decompile/span, driver/probe/results and SDK header pinned
in verification.json. No SDK/DLL/install/game/save/commit changes. W212 remains
latest offline payload. Shared runtime lease now Texture Archive4GB staging;
continue offline. Next inspect dynamic target liveness and source identity changes
at the waypoint helper's yielded frame/lookup boundaries using bounded evidence.

## W214 complete: dynamic waypoint frame-boundary equivalence

Owned work/codex_lua_tour_waypoint_dynamic_w214_20261018. Derived native/Lua
drivers from pinned W213 with checked textual adaptations; original487B helper
span unchanged.10752native executions/896controlled Lua traces PASS. Eight
dynamic target plans, counter0/3/16/17, four external counter schedules (none,
16,17,-1/16/17), seven termination stops; x2stacks/2fills/3refcount baselines.
Frame boundaries change external liveness so an existing dead target becomes
alive, a formerly live returned target becomes dead, same-identity references
become usable, or an alive old copy is replaced by an empty/new lookup result.
External counter writes before increment exercise wrap and reread behavior.
Exact world injection/counter/lookup/frame/guard/position/move traces match;
all saved native registers, callee stack and synthetic refcounts restored.
Lua copied-Thing mutable owner adapter is unchanged from W213 except external
world injection; tracked outputs retire. No production correction required.
Boundary injection is deliberate evidence, not real scheduler or engine proof.
Zero-count deletion/final destructor remain doubled; malformed/out-of-range
counter overflow remains outside the normal0..17/sentinel evidence. All534Lua
and ledger exact toW211; no SDK/DLL/install/save/game/commit changes. W212 stays
latest offline payload. Next inspect native signed counter arithmetic boundaries
before deciding whether any generic lowering correction is justified.

## W215 complete: signed counter boundary gap recorded, no promotion

Owned work/codex_lua_tour_counter_boundary_w215_20261018.540native/90Lua prefix
cases,80match/10INT_MAX mismatches. Original EE690B increment/signed compare/
guard/reset executes; stop EE694B before any out-of-range array address or
EE6A24 before cleanup. Nine signed boundary inputs x5saved counters x2guard
answers; native x2stacks/3fills. Exact extracted current Lua prefix executes
with an explicitly modeled signed32 persisted store; retain outgoing computed
local value to distinguish a caller arithmetic gap from any later binding cast.
At INT_MAX, native INC yieldsINT_MIN and signed JL takes lower branch; Lua
local becomes2147483648 and takes upper branch. Even narrowing persisted store
does not correct the subsequent local comparison. Other80logical cases match.
This malformed counter is not established reachable in normal0..17waypoint
gameplay; no array/engine/SDK cast/resource/scheduler/gameplay claim. Existing
W213/W214 normal/dynamic proofs unchanged. No production/generated/SDK/DLL/
install/save/game/commit changes; W212 remains latest accepted offline payload.
Next W216 scratch generic correction restricted to explicit signed int local,
known persisted Int field load +1/same-field store/immediate signed comparison;
adverse tests and all-unit audit must establish scope before promotion.

## W216 in progress: separately evidenced signed member increment recovery

Owned work/codex_lua_signed_field_step_w216_20261018; ROOT remains W211/W212.
Initial pre-lift proposal correctly failed reviewed Thing intermediate fingerprint;
candidate/initial log retained, no hash or certificate changed. Revised generic
post-review recovery leaves existing ownership preimages and operations intact.
It requires an explicit int local/member load+1/same-member store/immediate <,
known Int field, unique original MOV/INC/MOV/CMP/store/JL sequence, receiver
proven across reachable x86 branches (including partial-register clobbers and
slow/fast-path joins), plus exact lifted Lua pair. Separate native evidence
reported per function/shared helper. No invented public API or type inference
from Int storage alone. Current module native_signed_field_steps_v2.py.
56focused tests PASS. Candidate generated fresh in candidate_v2_pinned_root;
first candidate_v2 is superseded (ROOT-only override did not pin import-time
SIDECAR_PATCHES). Current setup sets module __file__ to real converter path
before loading, pins ROOT/SIDECAR_PATCHES; copied converter saved for promotion.
No source-root metadata changed. Candidate removes10/10INT_MAX numeric-prefix
mismatches:540fresh native/90Lua allmatch. It also matches1316normal/dynamic Lua
traces against15792already verified native cases; W213/W214 proof inputs exact.
Invalid array access after numeric prefix remains outside proof; no normal-game
reachability ofINT_MAX asserted. Default/schema3 TourGuide smoke JSON exact.
All29 corpus_v2 audit running (session20239); adjacent suite running(session24952),
first adjacent invocation had wrong x87 test filename, no tests ran; log retained
and path corrected. One candidate session34678 completed, collect its result.
Root generator/generated Lua/SDK/DLL/install/save/game remain unchanged. Next
review every changed unit and all final pins; promote only after complete results.

## W216 complete: signed member increment recovery promoted

Final generic post-review recovery in native_signed_field_steps.py; converter
hooks run after all prior reviewed operations for quest/entity and shared copies.
Existing Thing/ownership/x87 preimages, operations, hashes and metadata remain
unchanged and enforced. The first pre-lift candidate correctly failed intermediate
fingerprint; it remains unpromoted. No guard weakened, no certificate hash bumped.
Actual unique MOV/load,INC,MOV,CMP,same-field store,JL anchor plus receiver
CFG proof establishes signed32 local arithmetic. Explicit int local/member pair,
known Int field and immediate lifted comparison required. Reject unsigned branch,
clobbered/partially overwritten receiver, ambiguous native/source sites, segment
overrides, different field or store, metadata/shape mismatch.56focused tests,
363adjacent/3subtests and56root tests PASS. Initial adjacent path error retained.
Pinned-root scratch replaces initial v2 ROOT-only copy (import-time sidecar path
was not pinned). All29fresh conversions PASS;4Lua change (TourGuide quest/shared,
draft/readable),530exact. Reports differ ONLY separate numeric evidence; all
ownership recovery inventories and diagnostics exact. Readable unrelated locals
alpha-shift because the counter result now has explicit arithmetic; full traces
cover both MoveToPosition argument paths. Fresh root10Lua/report andall534hashes
exact. Current baseline now W216 current_corpus.json, superseding W211fourpaths.
540fresh native counter-prefix cases/90candidate Lua allmatch includingINT_MAX;
2632normal/dynamic Lua traces (1316eachquest/shared) match15792already verified
native cases with all W213/W214 proof inputs pinned. Quest helper omits frame
actor; shared helper supplies me as generated; explicit adapters validate both.
Default/schema3 TourGuide smoke JSON exact. Arithmetic prefix equality does not
establish out-of-range array addressing or ordinary gameplay reachability of
INT_MAX. Engine/frame/resource/final cleanup are still bounded doubles.
Ledger371/0syntax/855unresolved,4884gotos,6657native/4717machine/8442generic;
statuses42/179/150 unchanged. Promoted converter/newgeneric module/new56case test,
four generated Lua, reports/ledger. No SDK/DLL/install/save/game/commit changes.
W212 remains latest offline bundle until W217 refresh. Next stage accepted two
readable TourGuide Lua into a new bundle with unchanged c6ef DLL/override set;
then continue native waypoint zero-count cleanup boundary evidence offline.

## W217 complete: signed counter offline payload

Owned work/codex_lua_signed_counter_bundle_w217_20261018.
signed-counter-candidate-20261018 derives from accepted W212.203Lua parse,
208payload hashes and standard novi_local_test CLI preflight PASS. Exactly three
payload changes: TourGuide V_TourGuide.lua, native_quest_helpers.lua and README.
Other205payloads byte-exact, including c6ef sidecar, stock FSE, launchers and
override set; parent208files/manifest and installed EXE/QST pins exact. Sources
and inherited override membership checked before copying. ManifestSHA
2d5aeb99f2e75b8e7e0eadc04ac8294ca07a0ee868db84c4ce3525c482431b2f.
No installation, launch, save writes or new gameplay acceptance. W147 broad/W172
bounded runtime evidence unchanged; W190 remains a sealed frontend-only attempt.
Future launch must use a new coordinated capsule with W217; Book gate open.
Continue offline around shared texture/modernization lease. Next inspect actual
native Thing cleanup/destructor and zero-reference release paths with explicit
owner-deleter/CRT-free doubles; current resource ABI and DLL remain accepted.

## W218 complete: native zero-reference waypoint cleanup equivalence

Owned work/codex_lua_tour_zero_release_w218_20261018. Original whole487B helper
plus actual4AA840..4AA881 cleanup66B (SHA5aa58f8e6b624fb6abbc2496c0f63791f6a7ae480b3e86d9e93f148d23fe4afa)
and99A2E0..E6 base-vtable tail7B (SHAb391b1043a81dbc361b75b6ef3b6f5551ecf4643eb44e742e4f605ce2382115a).
5040native runs/420logical, two stacks/three fills/two caller-ref baselines1/9.
Each nonempty lookup constructs a synthetic ephemeral control with outputref1;
original inline assignment/temp-release and actual final cleanup reach zero.
5472owner-deleter/CRT-delete pairs execute through explicit callbacks, exactly
once per ephemeral control. All counters zero, owners destroyed/freed once,
caller refs and stack/registers restored, final Thing slot fields cleared and
base vtable1231710 installed. Repeated/dead/null target plans, wrap, guard exits
and long lookup cycles covered.1680current Lua traces (840quest/840shared across
two caller refcounts) match exact destroy/free ordering and caller effects using
explicit Copy/Assign/Retire control doubles. No correction required.
Real owner deleter, CRT heap, engine, binding ABI and scheduler are not executed.
Synthetic control payloads are separately identified; this is no engine object
ownership claim. Native complete deallocation decision/callback order established,
not actual heap lifetime. SDK owner header separately pinned: Retire idempotently
destroys and restores callable empty-Thing vtable until final host alias deletion.
All534Lua and ledger exact toW216; no SDK/DLL/install/save/game/commit changes.
W217 remains latest offline bundle. Next examine vtable scalar destructor4AB170
flag0 used by the SDK, contrasting flag1 buffer-delete behavior with original
bytes and explicit deleter/free callbacks; no native/game launch needed.

## W219 complete: native scalar destructor flag and caller contract

Owned work/codex_lua_thing_scalar_flags_w219_20261018. Original vtable1238C8C
first slot ->4AB170 scalar destructor87B, SHA7598bad2f69f53de4ca1f812b9d93cf62cc459891dc0b0c18e24b95d77c36912;
actual99A2E0 base tail7B retained.1200native executions/50logical: tenflag DWORDs
(0/1/2/3/100/101/dead0000/dead0001/80000000/ffffffff), nullcontrol or1/2/9/
ffffffff reference count, two stacks/threefills/twoThing buffers/twoowner pairs.
Original decrement/zero decision, owner callback, control delete, field clear,
base vtable, low flag-byte test, objectbuffer delete, return-this and RET4 execute.
External owner deleter/CRT delete are explicit doubles poisoning EAX/ECX/EDX;
all callee-saved registers and stack exact. Flag0 skips object-buffer delete;
flag1 triggers it; higher bits behave solely by lowbit. Count1 deletes owner then
control exactly once; count2/9/ffffffff decrement without owner/control free.
Static current SDK lambda calls Destructor(p,0); host owner separately frees
buffer at final alias deletion and restores callable empty-Thing vtable on Retire.
Header source pinned, not compiled/executed in this packet. Actual heap, engine,
Lua VM, host GC and 64-bit ABI remain outside this Win32 retail proof.
No correction needed; all534Lua and ledger exact toW216, W217payload current.
No SDK/DLL/install/save/game/commit changes. Historical Maze W143 cleanup was
subsequently accepted on2026-10-15, including actual runtime completion; do not
resume its superseded pause receipt. Next inspect remaining registered-unit
unresolved operands and select a current bounded recovery task.


## W220/W221 checkpoint: current operand inventory and Book dispatch staging

W220 read-only inventory pins all29 current conversion reports and identifies131
functions retaining diagnostics. W221 owns work/codex_lua_book_comment_staging_w221_20261018.
Captured current LookForBook E57530 phase, original1197B native extent SHA
4a030a4c5fba1384e670f46bcb7e6a8ed0de0eb849a1ce7b3b0675eb79085b53,
and unchanged existing Book reviewedResourceOperands row. Scratch optional resource
table-load evidence proves E5779A loads this+34 seh_me vtable through EDI into EAX.
No calls/branches/EAX partial writes intervene before spill E577B2. Existing pinned
reviewed operands validate same entry-stack reload E577DD and Speak E577E3, hero,
dynamic comment text and0/false/true/false flags. Exported fragment is precisely
local_18 = *pCVar12, not an iVar7 assignment (corrected scratch assertion).
Current Lua placeholder x_stk_14 line91 is unused on23 reached CFG nodes; other
physical-slot lifetimes remain intact. Existing liveness checker correctly rejects
earlier bare break syntax. Scratch extension models nearest-loop break targets,
including nil function-exit target and nested loops;14pytest cases pass, including
all50 existing parametrized liveness cases invoked as subcases. Five injected
observable reads/closure/stale-parent-hash cases reject. Original proof checker
remains unchanged. No diagnostic removed or generated output edited/promoted.
All534root Lua hashes still exactlyW216; W217 remains current offline bundle.
Next implement fail-closed generic post-review dispatch-staging classification in
scratch, with exact Lua phase/native/evidence pins and unchanged executable tokens,
then adverse validation and full unrelated-unit comparisons before promotion.
User returned asking status; answered progress and continued authorized offline work.
Lease currently Texture AI GPU preview, no game. Book gameplay gate remains open.
No launch/install/DLL/save/SDK/commit/push changes.


### W221 scratch classifier validated locally; corpus running, no promotion

Generic candidate classifier links CFG-proven resource table load to existing
reviewed spill/reload/dispatch evidence, exact phase/native hashes, unique exported
fragment and dead Lua alias. Retains nil assignment and changes annotation only.
38focused tests pass (including50existing liveness subcases),239adjacent pass.
Complete Book draft/readable pipeline changes only Teacher pair annotation;
all significant Lua tokens identical, six other Lua files exact.2016controlled
before/after Lua traces pass (1008each draft/readable, health/interruption/book
ownership/donation/request/comment combinations). API/scheduler doubles explicit.
Initial one-unit scratch conversion omitted write_reports, so readable helper
names/headers were incomplete. Superseded by corrected complete pipeline plus
fresh diff/trace comparison; no shared output was touched.
All29fresh corpus conversions running under owned session49038, progress/result
files in task folder; do not mark complete from partial progress or promote yet.
Adjacent session25358 and conversion50494 collected successfully. RootW216 and
offlineW217 remain current; Book gameplay gate remains open. User is now present;
no launch/capture/focus/input without fresh screen authorization.


### W221 full scratch corpus accepted; production remains W216/W217

All29fresh conversions completed successfully;2034input pins verified unchanged.
28unrelated conversion reports exact. Only Teacher draft/readable pair changes,
every executable token identical. Book report changes exactly one classified
diagnostic/proof, two derived TODO totals47to46 and two later speech-cancellation
line numbers by minus1; every other report value exact. Full corpus Book Lua hashes
equal corrected locally tested complete pipeline.38focused/239adjacent and2016
controlled Lua comparisons retained; all534root Lua hashes still exactlyW216.
Corpus session49038 collected exit0. No promotion or package changes yet.
Next prepare production classifier/liveness tests with durable fixture, re-review
generic table-flow guards, then promote only accepted code/evidence/Teacher pair.
Cleanup coordination reply written to
D:/Documents/FableTLC-maintenance/coordination/lua-script-recovery.txt.
All active/historical evidence, source, builds, pinned objects and worktrees remain
protected. Only exactW221/.pytest_cache directory released as regenerable cache.
No deletion or worktree release. No game/desktop/install/save/DLL/SDK/commit changes.
