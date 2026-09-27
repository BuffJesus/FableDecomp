# All current Lua scripts: readability review

User expanded the Expressions continuation to every current script, specifically
including Arena and older ports with LAB labels. Aeon's readable Lua is the style
target. Syntax success and mechanical cleanup do not certify a finished port.

## Inventory

`python -m tools.script_recovery.audit_readability --out work/readability_marathon_20260926/current_before_promotion.json`

The initial inventory includes 357 Lua files: every lifted `readable*` directory,
older lifted `FSE` packages, and authored Lua. Drafts, native evidence and scratch
candidates are excluded. Alternative generations are deliberately included and
identified by path. Of these files, 26 fail Lua 5.4 syntax checks; 99 more have
unresolved diagnostics, 106 more have mechanical readability findings, and 126
have no detected findings but still require human review. There were 5,006 gotos.

The JSON includes findings with line numbers; its Markdown companion lists every
file. The auditor excludes literal/comment occurrences from executable-name counts,
records unresolved diagnostics separately, and never marks a file fully readable.

## Generator changes

- Nested and implicit function-exit jumps become void returns.
- Jumps immediately past the innermost loop become `break`; multi-loop exits and
  intervening cleanup remain intact.
- Short straight cleanup tails become explicit calls and early returns. Binding
  checks reject shadowed locals, loop variables, closure-local jumps and comments
  that would swallow inlined statements.
- Existing tail-return folding now refuses variable-dependent return expressions
  without a binding proof. Moving `return value` into a scope with another local
  `value` was unsafe.
- Targeted API roles name the expression target, progress and opinion-deed handle.
- Readable build reports now include per-file readability findings.
- Native string concatenation retains its hidden destination when the caller
  ignores the returned pointer. The previous conversion emitted bare concatenation
  statements and stale dialogue keys. Calls are identified by their known native
  helper addresses, not a guessed function name.

Native gaps remain visible. Arena's round-data pointer expressions, unresolved
handles and entity fields require recovery; they cannot be fixed by formatting.

## Validation and promotion

Work is isolated under `work/readability_marathon_20260926/`, including snapshots
of every current Lua file and all 23 registered units' drafts. Rebuilds use these
snapshots; only Book Collecting, Bordello and Chicken Kicking receive freshly
converted drafts for the string construction fix. Guild Training's curated
`readable` stage must remain separate from its generated `readable_converter`.

Final focused checks: **307 tests plus 65 subtests passed**, including readability,
Expressions, string destinations, native division/spawn cases, pending Pickpocket
operand rejection/countdown checks, Trader Escort/Orchard cases and Wasp recovery.
The checks were rerun after generated-file promotion.

All **23 registered units** rebuilt (229 Lua files), with no raw-draft fallback.
Before/after smoke checks cover **200 scripts**, with no newly failing previously
passing function, new load error, new free global or new unknown method. These
bounded mock paths do not establish gameplay parity or prove the already-failing
scripts correct. Results: `work/readability_marathon_20260926/smoke_comparison.json`.

Promoted **107 changed readable files**, the three units' recovered drafts and
generated reports. Before copying, every destination Lua file and every source
draft was checked against the snapshot to detect concurrent edits; syntax checks
also rejected any newly invalid file. Guild Training's curated `readable` stage
was preserved; its generated `readable_converter` was refreshed instead.

Net changes across the complete 357-file inventory:

| Finding | Before | After |
| --- | ---: | ---: |
| Syntax failures | 26 | 23 |
| Goto occurrences | 5,006 | 4,195 |
| Native-name occurrences | 7,004 | 6,035 |
| Unresolved diagnostic occurrences | 2,783 | 2,776 |
| Machine temporary occurrences | 6,190 | 6,189 |
| Generic temporary occurrences | 7,409 | 7,502 |

Counts are occurrences, not unique variables. Inlining short cleanup tails can
repeat a generic-name use; lower counts alone are not the acceptance criterion.
The three newly parsing files are Chicken Kicking's KickedChicken and Spectator,
and Sick Child's TalkingTrader1. Other recovered strings expose later existing
syntax blockers, which remain on the ledger.

The [complete per-file ledger](../../scripts/CURRENT_LUA_READABILITY_REVIEW.md)
contains 23 invalid files, 101 other files needing recovery, 92 other files with
readability findings and 141 with no mechanical findings but no human certification.
Detailed line findings: `work/readability_marathon_20260926/after.json`.
No game installation, live bundle or save has been changed by this review.
Changes remain in the shared working tree; no combined commit was made over the
other session's separately prepared Expressions checkpoint.

## Remaining work

Every remaining label, native identifier, unresolved diagnostic and syntax failure
is still open. A clean mechanical audit also needs a human review of names, flow,
constants and purpose. The recovered scripts must not be presented as uniformly
Aeon-quality yet. Pickpocket's event payload binding remains a separate unfinished
experiment; the sidecar patch has not been built or deployed.


## Continuation: second readability pass

Round-two artifacts: `work/readability_marathon_20260926_round2/`.
Rebuilt all 23 registered units and repeated the 200-script bounded before/after
smoke comparison: no new regressions on exercised paths. Promoted 24 changed
readable files after snapshot ownership checks. Four additional files now parse:
ArenaCellDoorGuard2, BS_Teacher, BordelloGuard and TalkingTrader2.

Generator changes preserve effectful compound expressions instead of turning them
into invalid bare statements; sanitize persistence local identifiers while retaining
exact transfer keys; recover Arena's proven parent CellsVillage handle assignment;
and fold isolated boolean branch chains into short-circuit conditions. Runtime
checks cover evaluation order, boolean truthiness, persistence keys/context, and
both draft/readable cell-door guard state writes before interruption.

Focused validation: **315 tests and 52 subtests pass**. Complete 357-file inventory
now has 19 syntax failures, 4,111 gotos, 5,896 native-name occurrences, 2,768 unresolved
diagnostics, 6,191 machine-temporary occurrences and 7,427 generic-name occurrences.
The ledger remains an inventory of blockers, not a certification of readability.

Next recovery issue found: scalar-array sizing assumed four bytes for bool arrays.
PDB and Bordello's native persistence operands agree on byte stride. Arena, Bordello
and Guild Master Village metadata need a scoped correction, with actual native
operand and persistence-state checks before promoting regenerated output.


## Continuation: third pass, scalar arrays and integer parsing

Artifacts: `work/readability_marathon_20260926_round3/`. Reconverted every registered
unit, rebuilt all 23 readable packages, then verified the final native outputs again
after tightening concatenation recovery. The only additional readable refresh from
that verification was Bordello. The final 200-script smoke comparison reports no
new regressions on its bounded mock paths. Snapshot ownership and syntax checks
passed before promoting 12 changed readable files and the regenerated drafts/reports.
Magicman is the additional newly parsing readable file in this pass.

Recovered behavior and evidence:

- Scalar array layout now uses actual byte/short widths. Scoped unit metadata changes
  correct ArenaSpawnNeeded, ClientInUse and GuildMasterDialogue, preserving all helper
  mappings and unrelated fields. ArenaSpawn Main at `0x00F1B690` uses byte-indexed
  parent reads/stores at `+0xe2`; Bordello OnPersist at `0x00E3B8F0` transfers consecutive
  bytes at `+0x58`, `+0x59`, `+0x5a`. PDB layouts agree.
- Boolean arrays keep constant field addresses for persistence. Coalesced zero stores
  expand only inside a proven boolean-array extent. Named reads/writes replace the
  recovered byte accesses. A byte cursor becomes an index only when every use is a
  zero comparison or unit increment; pointer escapes/writes reject the rewrite.
- Bordello Init and OnPersist now share ClientInUse_0..2 state keys while serialized
  keys remain exactly ClientInUse[0]..[2]. Both draft and readable client Init reserve
  the first available slot, including after two occupied slots. Correcting the false
  array overlap also restores the BordelloVillage handle assignment.
- GFCharStringToInt (`0x0099E7F0`) is not Lua tonumber. The generated local parser
  collects digits, records a minus sign, stops at a period and preserves signed
  32-bit overflow. Tests execute the retail routine in Unicorn and compare draft and
  readable Lua over ASCII edge cases. This resolves the new Arena smoke failure
  exposed by an empty GetDataString, which the native routine parses as zero.
- Readability closure cleanup now requires straight-line bodies before propagating
  literals or localizing outer assignments. A conditional write inside the new parser
  exposed the previous unsafe assumption; separate conditional/loop tests cover it.
- Concatenation preserves declared local string destinations, including recovered
  string-array storage. Split-line assigned/returned calls remain expressions and do
  not acquire a second assignment. Magicman's dialogue strings now retain their
  destination. GetLHTSTag's separate hidden string return remains unrecovered.

Cumulative inventory after all three passes: **116 current Lua files changed**, 895
fewer gotos, and syntax failures reduced from **26 to 18** (eight newly parsing files).
Current totals: 357 files; 4,111 gotos; 5,894 native-name occurrences; 2,750 unresolved
diagnostics; 6,191 machine-temporary occurrences; 7,433 generic-name occurrences.
The ledger contains 18 invalid files, 104 other recovery blockers, 92 other readability
blockers, and 143 files still requiring human review despite no mechanical findings.

Further work remains substantial: Arena round/wave vectors, unresolved helper return
operands, packed control flags, and the older unregistered FSE draft packages still
contain native constructs. The unchanged old packages remain in the ledger; they have
not been hidden or relabeled as complete. Generated syntax and bounded smoke tests
are not in-game parity. No game install or save was changed, and no shared-tree commit
was made over the other session's work.

Final post-promotion focused suite: **401 tests and 52 subtests pass**.
`git diff --check` passes for the recovery code and review documentation.

## Fourth pass: hidden string return and all dialogue callers

The next syntax blocker was BordelloLady.GetLHTSTag (`0x00E403D0`). Ghidra prints
the hidden CCharString return buffer as the only explicit argument and loses the
actual suffix at stack +8. The reviewed CCharString-value prototype, native output
construction, RET 8, and all caller push records agree. Native instructions save the
suffix before reusing its stack slot for an intermediate concatenation.

`native_string_returns.py` now recovers this narrow ABI shape only when every caller
can be proven. It pairs callers in exported token order, finds each pushed suffix's
unmodified string constructor, and preserves output-slot/EAX result identity. Missing
stack evidence, escapes, intervening control flow and generated-name collisions
leave the original evidence unchanged. The lifter emits ordinary string returns.
This is a generator correction, not a hand-authored replacement quest body.

All 15 calls now use their actual suffixes (including distinct paid/free question,
follow, decline and cutscene end-line variants). Before this correction some used
undefined scratch values, the movie handle, or a suffix from another branch.
The readable helper is now:

```lua
function GetLHTSTag(quest, me, dialogueSuffix)
    return (("TEXT_QST_B13_" .. name) .. "_") .. dialogueSuffix
end
```

Validation in `work/readability_marathon_20260926_round4/`:

- All 23 native conversions and readable builds completed. Exactly one draft and
  readable Lua file changed: BordelloLady. No readable-stage fallbacks.
- All 200 registered scripts received before/after bounded smoke checks, with no
  new regressions. BordelloLady now compiles and loads.
- 424 focused tests plus 52 subtests pass; a subsequent 10-test targeted run covers
  the final rejection guards (including two additional negative cases).
- Tests check every caller suffix, execute both generated helper stages, and compare
  their strings with the retail helper running in Unicorn. Only its external string
  primitives are mocked; native stack handling, operand order, output pointer and
  RET cleanup execute from retail bytes. No retail bytes are embedded in tests.
- Final-generator Bordello reconversion is byte-identical to the promoted Lua.
  Promotion verifies snapshot ownership, inventory and syntax before writing.

Final audit: 357 files, **17 syntax failures**, 4,111 gotos, 5,894 native-name
occurrences, 2,750 unresolved diagnostics, 6,190 machine-temporary occurrences and
7,409 generic-name occurrences. Cumulatively 116 current Lua files differ from the
initial snapshot, 895 gotos have been removed, and nine formerly invalid files parse.

BordelloLady is still **needs-recovery**, not playable or Aeon-quality. Her Init
(`0x00E3AC70`) loses the Name member assignment through a local CCharString pointer
alias, and its subsequent comparisons/prices remain unresolved. Helper tests supply
that native field explicitly. Main still has missing operands, resource copies and
unstructured flow. Actor-map syntax blockers elsewhere also involve resource IDs
retained across callbacks; the current host resource pool closes at callback exit,
so merely storing those IDs in quest state would be incorrect. No game install,
save or sidecar binary was changed.

## Fifth pass: BordelloLady initialization

Recovered the typed local alias of the Name CCharString member. The write now keeps
both native member state and the local string value, so Init reads `me:GetDataString()`
once, saves Name and chooses the correct GoldRequired. The five prices are 50, 100,
200, 1000 and 2000 for POLLY, AMELIA, LUCREZIA, SOPHIA and HEDWIG. Unknown/empty names
leave the price at zero. Retail `0x00E3AC96..0x00E3AC9F` confirms member +0x28,
GetDataString slot +0xC and CCharString::operator=; the price stores target +0x20.

`native_string_members.py` refuses escaping/rebound aliases, duplicate member
accesses, arbitrary buffer-identity tests and helper calls receiving this. The
represented null branches are compiler-generated literal-comparison fallbacks.
Tests run full generated Init followed by the dialogue helper, in draft and readable
stages, for all five names, unknown/empty strings and both nunnery states.

Evidence: `work/readability_marathon_20260926_round5/`. All 23 native/readable units
rebuilt; only BordelloLady changed; 200-script before/after smoke comparison has no
new regressions. **463 tests plus 52 subtests pass**, with a subsequent 11-case run
covering final alias guards (two additional rejection cases). Final-generator
Bordello output matches the promoted draft. The audit still has 17 syntax failures;
unresolved diagnostics decreased from 2750 to 2740. Main's missing operands and
resource ownership problems remain; the recovered initializer does not establish
whole-quest in-game parity.

## Sixth pass: readable shared exits and string locals

Two empty branches that jump to the same label now combine into a short-circuit
condition. Existing structured-flow recovery can then remove private destination
labels. Both condition order and Lua truthiness are preserved; branches with extra
statements, different destinations, or multiline protected text are declined.
Native pOther locals proven to contain only GetDataString results receive the name
dataString; unrelated pOther variables remain unchanged. Renaming retains the
existing reversible maps and collision checks.

Changes affect five readable files: BordelloLady, Global_OpenChest, TraderToRescue,
DarkwoodTrader and Arena. The last two receive naming improvements only. BordelloLady
Init now has no LAB labels. Her unresolved PlayCutscene call remains diagnostic;
removing its surrounding empty dispatch does not recover that missing operand.
Open Chest's hit/cancel loop is structured while retaining its cleanup calls.

Validation in `work/readability_marathon_20260926_round6/`:

- All 23 readable units rebuilt from the fifth-pass native drafts, without fallback.
- All 200 registered scripts received before/after smoke checks; no new regressions.
- **497 focused tests plus 52 subtests pass**. A final presentation/guard run passes
  51 tests plus 13 subtests, including two additional multiline-protection cases.
- Seven separate full Open Chest before/after execution traces agree: normal,
  failed opening, cancellation, ordinary hit, special-ability hit, both hits and
  shutdown. They compare all mocked method calls, including movie/resource cleanup,
  and verify the special-ability test is skipped after an ordinary hit.
- No native draft contains multiline protected tokens, so the final protection
  guard leaves these rebuilt artifacts unchanged. `git diff --check` passes.
- Snapshot ownership, file inventory and no-new-syntax-failure checks pass before
  promotion. Generated outputs and the all-current-files review ledger are updated.

Final totals: 357 current Lua files, 17 syntax failures, 4,101 gotos, 5,880 native-name
occurrences, 2,740 unresolved diagnostics, 6,190 machine temporaries and 7,403 generic
names. The six passes cumulatively changed 116 current files and removed 905 gotos.
Old unregistered FSE packages remain in the inventory. Arena's wave/vector recovery,
cross-callback resources and the remaining diagnostics still require substantive
work; no claim of full readability or in-game parity is made.

## Seventh pass: Arena integer counters

Four-byte int/long arrays no longer enter struct-element pointer lowering. Static
addresses remain available to normal named-field recovery; explicit scaled indexes
use the same state keys. Arena's reviewed three-element TotalCreatures array also
recovers its byte-addressed class-pointer cursors, including every reset, read,
increment and clear. Other cursor types and escaping or rebound pointers are declined.

PDB metadata gives long[3], retail offset 0xD0 and stride four. PlayWave's typed
decompile and retail instructions agree: resets at F1FF4C/F2039C/F20579/F206AE,
four-byte increments at F1FFFF/F203B4/F205CB/F206CB. The separate spawning cursor
uses the same array. This restores initialization and summoned-creature increments
and decrements as well as wave counts; it does not recover the nested wave vectors.

Artifacts: work/readability_marathon_20260926_round7. All 23 units rebuilt and
smoke-compared without new regressions; a second native conversion matches every
generated Lua file. Two readable files change, Arena and SUMMONED_CREATURE. The
new module passes 24 tests, including actual generated spawn/death callbacks in
both draft/readable forms, interruption, hero-kill points and independent buckets.
The related scalar/parent/lifter/readability suites pass 163 other tests plus 52
subtests. Final callback tests use the NewScriptFrame/termination contract used
by readable scheduling. These mocked tests are not whole-quest gameplay validation.

The inventory remains 357 files and 17 syntax failures. Diagnostics fall to 2729;
gotos remain 4101. Seven passes cumulatively change 117 files and remove 905 gotos.
Arena's first remaining syntax blocker is reused stack storage for local counter
arrays; lifetime recovery must separate it from earlier creature objects and later
position vectors rather than reinterpret the entire stack slot as one Lua value.

## Eighth pass: PlayWave local counter arrays

The reviewed counter lifetime now has separate initialCreatureCounts and
creatureCounterIds Lua tables. Indexed dword reads/writes, first-counter removal,
extra-creature count accumulation and replacement-ID storage are recovered. The
signed invalid-ID sentinel remains -1. Earlier creature-object storage and later
position-vector storage are unchanged, including their remaining diagnostics.

Retail evidence: F20593 reads counts at ESP+EBX+88; F205BA reads IDs at ESP+EBX+80
after two pushes (original base +78). ADD EBX,4 / CMP EBX,0xC establishes the
three-entry extent. F205FE loads the first ID, F20604 pushes that value, and F20605
calls slot 0x548. This resolves Ghidra's misleading `(int)auStack_f4` cast as the
first ID value rather than the former creature object or an array address.
The decompile's matching indexed stores, loop bounds and slot lifetimes supply
the independent structural evidence. Recovery is gated on native address F1EED0
and rejects unknown aliases, changed extents, missing markers and name collisions.

PlayWave now compiles independently. The Arena file still fails in helper_F25980,
the round-vector copy helper; nested wave-title/vector expressions remain explicit
diagnostics. Parsing the recovered function is not a claim of runnable Arena parity.

Artifacts: work/readability_marathon_20260926_round8. All 23 units reconverted and
rebuilt, with no new regressions in the 200-script smoke comparison. Only Arena's
root Lua changes. The combined relevant suite passes **204 tests and 52 subtests**,
including 17 new array cases and 24 scalar/callback cases from pass seven. Tests
cover lifetime separation, unknown-use rejection, native instruction anchors,
three independent table entries, first-entry replacement and full conversion.
An additional direct compile verifies the final generated PlayWave function.
Snapshot ownership, inventories, no-new-syntax-failure checks and diff whitespace
checks pass before promotion. The complete 357-file review ledger is refreshed.

Final totals: 17 syntax failures, 4101 gotos, 5880 native-name occurrences, 2722
unresolved diagnostics, 6190 machine temporaries and 7406 generic-name occurrences.
The two Arena passes remove 18 diagnostics; eight passes cumulatively change 117
files and remove 905 gotos. The extra generic-name references in this pass come
from restored indexed accesses, not additional hidden native operations.

Next Arena evidence targets: nested Rounds/CArenaRoundDef and wave-vector copy
helpers, followed by two remaining ArenaSpawnNeeded byte stores. Retail F1FA9F and
F1FE2D are `mov byte ptr [eax + esi + 0xe2],1`; the decompiler currently spells them
as class-pointer subscripts. Recover those against the existing PDB bool[16]
metadata, preserving dynamic indices. Older unregistered FSE packages remain in
the all-current inventory and are still part of the user's readability objective.

## Ninth pass: Arena spawn flags and crowd-sound matrix

PlayWave's two remaining class-pointer subscript stores now set the selected
ArenaSpawnNeeded boolean. Both are MOV BYTE PTR [ESI+EAX+E2],1 at retail F1FA9F
and F1FE2D. Recovery requires the reviewed function address and bool[16] metadata;
it preserves both forms of the dynamic index and declines other offsets/classes.

The PDB's CCharString[4][5] CrowdLoopTags member now expands into twenty named
String fields at retail offsets 48..94. Matrix descriptors preserve row-major
order: row stride 20, column stride 4. Dynamic reads in CrowdChecker, ArenaEnemy
and SUMMONED_CREATURE become keyed string reads, including cheer column three.
Static member addresses remain available to ordinary field lifting, rather than
being incorrectly treated as struct-array pointers.

InitialiseVariables' twenty CCharString assignments match retail F25840..F2596A:
each literal PUSH, destination LEA and call to 99EFE0 is checked. The final assigned
operator= result is unused after the BSIM-proven void return is removed, but its
write must execute. Dead-result recovery now preserves that member write. Any
other reference to the result, including one before the assignment that might be
revisited by a loop, declines this rewrite. The previously omitted twentieth tag
(CrowdLoopTags_3_4, ARENA_AWWW) is consequently initialized.

Validation and promotion: work/readability_marathon_20260926_round9.

- **325 tests plus 52 subtests pass**, including 18 spawn-flag cases and 64 matrix
  cases. Tests execute all twenty row/column combinations, all twenty initializer
  writes in draft/readable forms, and the actual summoned-creature flourish path
  through sound playback and its five-point crowd-score update at every crowd row.
- The initializer test explicitly stubs the still-unresolved round-vector-copy
  boundary; it establishes the string writes, not whole-initializer/gameplay parity.
- All 23 units receive native and readable comparisons. The final native generator
  changes only Arena root, ArenaEnemy and SUMMONED_CREATURE. Those readables are
  refreshed after the final native comparison; all 200 registered-script smoke
  comparisons have no new regressions. Snapshot ownership and inventory checks pass.
- The 357-file ledger is refreshed. Syntax failures remain 17. Diagnostics fall
  from 2722 to 2700; native-name occurrences are 5874, machine temporaries 6184,
  generic-name occurrences 7418 and gotos 4101. Newly restored indexed accesses
  can add references to existing generic variables; these counts are not a parity
  or readability certification. Nine passes cumulatively remove 905 gotos.

### Remaining Arena vector boundary

F25840 InitialiseVariables calls F25980 at F25854 with destination this+98 and
source *(0143E90C)+1044. This copies the runtime global-definition round vector;
the current converter wrongly models some container helpers as quest methods.
F25980's other recorded caller is CScriptDef::operator= (F2A250), outside this unit.
Its propagated Conversation/ClanMember labels are not trustworthy type evidence.

The PDB and retail strides establish the nested schema: CArenaRoundDef has NumWaves
at +28 and Waves at +2C (retail element stride 38); CArenaWaveDef has NumWaveCreatures
at +28 and Creatures at +2C (retail stride 3C); CArenaCreatureDef has CreatureType at
+28, NumCreatures at +2C, HUDType at +30 and DeathScore at +34 (stride 38). These
offsets/strides are hexadecimal. Debug vectors carry an extra iterator pointer,
so later offsets must still be checked against retail accesses.

The existing recovery binding patches expose scalar global-data reads and float
array reads, not a nested Arena-round snapshot. The next recovery needs a reviewed
runtime data loader and a representation shared across the quest's callbacks,
then correct copy-helper argument/receiver handling. Do not replace this live
definition data with guessed constants, erase the helper bodies, or claim Arena
is loadable merely because PlayWave parses. The root still fails in helper_F25980.

## Tenth pass: Arena round snapshot and nested definition reads

The runtime boundary anticipated above is implemented in
`runtime_bindings/arena_round_snapshot.h` and `NoviArenaRounds.h`. It reads the
retail global definitions, copies the nested script-visible fields into owned
C++ strings/scalars, then uses existing namespaced quest Int/Bool/String state.
Separate Lua callback states see the same primitive state without sharing a Lua
table or retaining native vector/string pointers. Declared counts remain distinct
from actual vector lengths. Empty allocated vectors are supported; malformed
extents and null/overflowed definition addresses fail before writing state.
Opaque inherited definition internals are not exposed as Lua fields.

Evidence combines the PDB layouts with retail instruction checks: F25840/F25848/
F2584E/F25854 establish the global +1044 to quest +98 copy; F26B6E/F26B75/F26B7A
establish round count/nested vector/38-byte stride; F25F71/F25F80 establish wave
vector and retail ShortWave at +38 (the debug +3C includes an iterator pointer).
F25F1E..F25F3F establish creature strings/count/death score; F1FF23/F1FF26 and
F1FFF9/F20002 establish the wave stride and three-entry creature cursor.

`native_arena_rounds.py` opts in only for the reviewed InitialiseVariables and
PlayWave addresses. It separates CStack_108's numeric cursor lifetime from its
later string temporaries, and gives the creature-type value its own local.
The latter matters: the earlier candidate reused iVar6, allowing the lifter to
carry a string into a numeric spawn-point loop. The real PlayWave trace rejects
that candidate with `attempt to compare number with string`; the separated
lifetime reaches HUD setup. Unknown aliases, changed offsets/strides/bounds,
additional cursor uses and local-name collisions decline recovery.

Replacing the initializer's single reviewed copy edge makes nineteen container
bodies unreachable within this Lua unit. The converter checks the exact reviewed
library closure and recomputes reachability from every remaining quest/entity
entry, retaining anything another entry still uses. It does not delete native
evidence or unmatched gameplay helpers. Conversion reports record the replaced
edge, omitted bodies and implementation in `runtimeBoundaries`.

`ShortWave` is a Bool state value and is tested with Lua `not`, never `== 0`.
Recovering its condition restores the countdown/interruption branch that was
previously omitted. This adds seven explicit gotos to the current inventory;
they represent recovered control flow, not a justification for hiding labels.
The root becomes syntax-valid while its remaining gameplay diagnostics stay
visible. Whole-Arena gameplay parity is not established.

Runtime validation is reproducible with `run_arena_round_checks.py`. The portable
snapshot executable compiles as x86 C++17 with /W4 /WX and runs successfully.
A separate compile probe instantiates the real sol binding against the ABI-v13
FSE headers. The source patch applies cleanly to an isolated copy and reproduces
the compiled headers exactly. The base checkout and live installation are not
modified. A separate Release x86 candidate DLL now builds and links successfully
with all six patches after ABI-v13, including the earlier event-data/resource
bindings. Its SHA-256 is
`745e151f817ed3a1f2dcd8e12cf5a8c4a2234328fe6d9cfd913b514d4896ceb1`; the patch
manifest and build log are in `sidecar_candidate/`. **It is not deployed or
in-game validated.** Smoke reports `InitialiseArenaRounds` as pending; the
requirement is documented in `FSE_UPSTREAM_REQUIREMENTS.md`.

Validation artifacts: `work/readability_marathon_20260926_round10/`.

The complete spawning trace also exposed an independent exporter stack-depth
error at F1F873. Both native copy alternatives use LEA ECX,[ESP+A4] after pushing
the source (F1F86C/F1F915), and cleanup uses ESP+A0 (F1FA98). The numeric loop
counter is ESP+9C (F1FABA). Ghidra instead printed &iStack_d0 for one destination,
overwriting the Lua counter with a creature handle. A narrowly guarded recovery
uses the same CStack_cc handle as the other alternative and shared cleanup;
changed call targets, joins or counter lifetimes are left unresolved. Before the
fix, round-three traces failed with table arithmetic; the oracle asserts both
the created handle and the numeric counter through HUD setup.

Final focused validation: **527 tests plus 28 subtests pass** (335 general
recovery/readability tests, 123 existing Arena regressions, 69 new Arena cases).
The new cases include 24 real PlayWave traces: normal/short waves, round indices
2/3 (both creation APIs), all three group positions, and draft/readable output.
They verify the THREE/TWO/ONE/GO dialogue order and six timer frames when required,
the selected creature/HUD tag, correct created handle, and all three counters.
They use one spawn point and one active creature group and intentionally stop at
DisplayQuestInfo; general multi-point spawning and subsequent combat/rewards
remain outside that behavioral claim.

Promotion passed snapshot ownership, inventory, syntax and all 23 unit smoke
comparisons (200 registered scripts). Only Arena's root Lua changes in this pass;
the final narrowly scoped PlayWave fix was rebuilt/rechecked after the complete
native comparison. The 357-file ledger now has 16 syntax failures, 4,108 gotos,
5,856 native-name occurrences, 2,561 unresolved diagnostics, 6,182 machine
temporaries and 7,221 generic-name occurrences. This pass resolves 139 diagnostics
and makes the root loadable; ten passes cumulatively improve 118 current files
and remove 898 gotos net. The seven restored jumps are accounted for, not hidden.

## Eleventh pass: older named-cluster roots and suffix scope

The separate named-cluster pipeline was regenerated against all twelve Aeon
reference packages (plus its four New Oakvale entity benchmark cases). Six root
files were selected for presentation after review: MazeResearch, MeetSister,
ScytheInfo, Fisherman, RockTrollFirstEncounter and GuardianTrophyDealerInfo. Five
raw roots reproduce the current files exactly; Rock Troll's only native-generator
change removes an empty exception-cleanup flag check. Other old candidates remain
staged for recovery, including the invalid named-cluster entity files.

`build_readable_cluster.py --script <cluster> --out <new-directory>` now makes
this older path reproducible. It retains native reports, applies the existing
readable pass to valid generated Lua and writes per-file transformation/syntax
reports. Invalid drafts retain their code and diagnostics with `notStyled` in
the report. Only the six reviewed quest-root files are promoted; their existing
entity scripts remain unchanged. Current READABLE_CLUSTER_REPORT files explicitly
record that root-only scope.

The cross-check caught a real cosmetic-pass bug: drop_free_suffixes renamed the
undeclared ppVar5 global to ppVar, potentially changing a global another callback
reads. It now requires a local declaration or function parameter, rejects unknown
or shadowed scopes, checks initializer/out-of-scope uses and ignores declarations
inside comments/strings. Assigned parameters can still be simplified. A Lua test
executes separate global-writer and reader callbacks to establish the distinction.

The same guard restores the native piVar2 spelling in the still-invalid Bordello
and Sick Child helper files; their diagnostics and unresolved behavior remain.
Those sixteen references were previously hidden from the machine-name metric by
the incorrect rename. The six older roots remove 109 machine-temporary references,
for a net reduction of 93 in this pass. No unknown global is localized by guesswork.

Validation: **225 tests plus 101 subtests pass** for this pass (69 scope/style/
legacy tests and 156 Arena/readability recovery regressions). Eighty callback
scenarios across the six roots finish normally with identical API calls and
arguments, quest state and scheduler behavior before/after presentation. Cases
cover immediate, delayed and absent region loading plus completion-state inputs
and region departure. These are presentation-equivalence traces, not a substitute
for native gameplay parity. The separate global-write/read regression guards
cross-callback naming, and invalid-entity evidence retention is checked.

All 23 registered units were rebuilt and smoke-compared; the final parameter-scope
refinement rechecked the three affected units. The six older packages add 20
script-file comparisons. Across these 220 files there are no new smoke regressions
or readable fallbacks. Only the six roots and two helper files change. Promotion
checks source ownership, file inventory and syntax. Artifacts are under
`work/readability_marathon_20260926_round11/`.

The 357-file inventory still has 16 syntax failures. Current totals are 4,108
gotos, 5,856 native-name occurrences, 2,561 unresolved diagnostics, 6,089 machine
temporaries and 7,221 generic-name occurrences. Remaining old-cluster operands,
field names and entity stubs still require native evidence; this is not a claim
that all current scripts are fully readable.

Eleven passes cumulatively change 126 current Lua files; the net goto reduction remains 898.

## Shutdown checkpoint

Stopped at the user's bedtime/shutdown request on 2026-09-26. The eleventh-pass
promotion, current 357-file ledger, test reports and resume instructions are saved
locally. No recovery test/build jobs remain running; changes are uncommitted and
the candidate DLL is not deployed. Next session should start with the remaining
16 syntax failures and older entity stubs, using native evidence before rewriting
operands or control flow. Completed pass reports need not be rerun without changes.
A separate `gate_re_agent_candidates.py` process (PID 11588 at inspection) belongs
to other work and was left untouched; it is not a Lua recovery job.
