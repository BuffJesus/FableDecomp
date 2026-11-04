# Native recreation continuation, 2026-10-01

Resumed at the user's request from the native September 30 shutdown checkpoint.
Preserve the unrelated dirty working tree and the separate Lua work. No commits,
deployments, or Lua changes are part of this lane. Scratch evidence lives under
`work/codex-native-startup`.

## Consolidated starting-state validation

`consolidated-gates-20261001.log` ends in `ALL GATES PASS`: 123/123 boot leaves,
component ABI, native GFMain (3952 bytes, 391 relocation targets, 58 scenarios,
five callbacks), zero regressions across 645 dependent catalog entries,
bootstrap references, bank link ring, and visual bootstrap all pass. This run
finished before today's message-loop promotion and consolidates yesterday's
previously separate successful checks.

## Native window-message loop promoted

Recovered `CSystemManager::ProcessWindowMessages` at `009a4f20` as genuine C++.
The previous scratch parity failure was register allocation: 141 bytes and the
same 54 instructions, with normalized instruction similarity 1.0. Accepted at
NEAR_MATCH under the functional-first standard; all five import targets are
checked against retail, not merely masked.

The source enters the manager's critical section, drains `PeekMessageW` with
the manager's current HWND and PM_REMOVE, calls the optional fastcall filter,
translates and dispatches unfiltered messages, and releases the critical section.
The fields in both manager and init structure now use the actual callback types.
Native GFMain takes the typed IME callback addresses directly; the authored
phase-7 coordinator and initialization fixture use typed address casts.

Evidence:

- `window-message-loop/gate-report.json` verifies the staged source and fixture.
- `window-message-header-audit-5mw9smqs/results.json`: zero regressions across
  all 66 dependent catalog entries; staged GFMain retains 391/58 checks.
- `before-window-message-loop-20261001/manifest.json`: hashes and backups for
  the 12 promoted files. The one-shot landing script has executed; do not rerun.
- Production `tools/decomp_pipeline/gate_window_messages.py` passes 96 compiled
  fixture scenarios and 96 comparative retail/candidate instruction executions.
  Production report: `rebuild/compile-gate/window-messages.json`.
- Production native GFMain still passes 3952/391/58 plus its five callbacks.

The fixture checks empty/multiple queues, null HWND, absent/passing/suppressing/
mixed filters, message mutation, callback removal, HWND changes, WM_QUIT queue
handling, both TranslateMessage outcomes, dispatch continuation and lock release.
Its kernel32 critical-section imports require explicit test-process IAT overrides:
linker alternates alone lose to the CRT imports. The fixture restores those slots
before reporting; the production candidate object is unchanged. The comparison
also checks preserved registers, stack balance, object writes and call traces.

The aggregate gate now includes this message-loop gate. The production catalog
check also passes NEAR_MATCH/PASS, and both affected boot leaves (009a6610 and
00403b10) remain RELOCATION_MATCH/PASS. The later full run in
`message-loop-final-gates-20261001.log` ends in ALL GATES PASS, including 646
dependent entries and the visual bootstrap. This run includes the base-destructor
linkage change below. Executable integration of native GFMain remains open; the
runnable visual checkpoint still uses its authored coordinator.

## Dependency closure investigation

A fresh scratch closure probe in `native-gfmain-closure-20261001` reached four
rounds: the final checked link compiled 237 sources, found 543 unresolved symbols
and seven duplicate-symbol groups; 367 sources were selected for the next round.
These are dependency inventories, not a working native executable. No closure
executable was run. `link-diagnostics.txt` retains the concrete duplicate pairs.

Next: distinguish redundant selected providers from definitions that must be
made link-compatible, preserving behavior and parity for each affected source.

### Reviewed duplicate providers

Scratch `link-provider-staging/exact-provider-proof.json` compares every exported
symbol, full section bytes, symbol offsets and relocation targets for four
redundant objects: the char-string destructor, wide-string length getter,
WriteULONG, and the duplicate CInitBaseClass destructor translation unit. Each
has an identical provider already in the selected set.

Promoted one linkage-only source edit: CBaseClass's destructor is inline in its
family translation unit, matching the inline definitions needed by the derived
class bodies. Five catalog fixtures retain RELOCATION_MATCH/PASS. Backup and
hashes: `link-provider-staging/base-promotion.json`. Its symbol index is refreshed.

The wide literal constructor requires a separate comparison: the operator+
translation unit expands the allocation helper, producing a 183-byte constructor
instead of the standalone 104-byte body. A scratch reference with the allocation
helper out of line emits a constructor identical to the canonical object. Both
versions pass 21 direct-constructor scenarios (null, empty, ASCII/Unicode,
lengths through 511, independent ownership, input mutation and destruction)
and the three existing concatenation scenarios. The original operator+ source
is unchanged. This justifies the scratch runtime provider selection, not a new
constructor parity or coverage claim. Proof: `wide-provider-proof.json`.

Rejected scratch alternatives removed either the constructor body or the inline
allocator body from operator+. Both altered its code generation below the
near-match threshold. Neither change was promoted. Marking its constructor
inline also left an emitted duplicate and was not promoted.

The resulting `coalesced-all.rsp` has 232 objects and zero duplicate definitions;
unresolved engine symbols still prevent linking. These selections are currently
scratch-only, not changes to the production closure algorithm. The continuation
script seeds the selected providers and preserves the omitted canonical sources.

Two deeper rounds compiled 461 sources at the last checked link, leaving 640
unresolved symbols and four new duplicate groups; 496 sources were selected for
the next round. New duplicates: CBaseClass constructor, the two CWorld path
helpers, and CSurface::GetPixelFormat. Evidence:
`native-gfmain-closure-deeper-20261001` and its log. No executable was run.

## Critical-message dependency promoted

`CSystemManager::DisplayCriticalMessage` at 009a4ed0 now has a genuine 75-byte
RELOCATION_MATCH implementation. The native body constructs a named narrow
string, converts it to a temporary wide string, gets its characters and calls
MessageBoxW with null owner, zero flags and the literal `Application Error`.
It then destroys the wide temporary and narrow local in that order. The initial
all-in-one expression changed VC7.1 argument evaluation order and was rejected;
the named local matches the retail body exactly after relocation.

Native disassembly/import table, the mapped UTF-16 literal, FableWin decorated
name and the BSim donor audit establish the identity and arguments. All seven
relocations are checked, including the literal content. The fixture runs 45
scenarios with mocked conversion and MessageBoxW; it verifies source pointer,
length -1, converted-text pointer, caption, flags, ignored dialog result,
destruction order and unchanged manager state. No dialog opens.

`tools/decomp_pipeline/gate_critical_message.py` passes on canonical production
source; report `rebuild/compile-gate/critical-message.json`. The aggregate gate
includes this check. The preceding ALL GATES PASS predates this isolated leaf's
promotion; no production headers or existing runtime consumers changed with it.
Backups: `before-critical-message-20261001/manifest.json`. Its one-shot landing
script has executed. The native symbol index includes the new method.

## Input-focus propagation promoted

Recovered `CInputManager::SetHasInputFocus` at 009f4e20, a direct unresolved
dependency of UpdateActive. Candidate is 79 bytes against retail's 75, with
39 versus 37 instructions and normalized similarity 0.973684: NEAR_MATCH.
No relocations occur in this body. The new gate verifies the three retail base
vtable slots against their named callback targets as well as comparative behavior.

The named input-manager fields replace its opaque bytes at 0x24..0x37: mouse and
keyboard scoped-pointer storage, then the three STLport vector pointers to
eight-byte counted joystick entries. This is a storage view; it adds no implicit
ownership or destructor behavior. Names/types come from Ego_r layouts, adjusted
for retail's smaller vectors and verified against the retail instructions.
Generated CMouse/CKeyboard/CJoystick headers remain unchanged.

VC7.1 rejects nonmember `__thiscall` function pointers. The focus vtable views use
its four-byte single-inheritance member-function pointers on the complete,
flattened generated classes, with size/offset assertions. This preserves the
actual ECX receiver and stack bool without assembly or a fastcall adapter in
production. The linked fixture and retail comparison both exercise that ABI.

`gate_input_focus.py` passes 128 compiled cases and 128 comparative instruction
executions. Coverage includes focus on/off, absent mouse/keyboard, zero through
three joysticks, mouse replacing keyboard, keyboard replacing the joystick
vector, and the first joystick replacing vector storage and length. Retail
snapshots joystick count after keyboard notification, but reloads the begin
pointer for each joystick. Call order, receivers, arguments, object writes,
counted-pointer info storage, saved registers and stack balance are checked.

`input-focus-header-audit-58pibm37/results.json`: zero regressions across all
52 dependent catalog entries. Production gate report:
`rebuild/compile-gate/input-focus.json`. Backups and hashes are under
`before-input-focus-20261001`; its one-shot landing script has run. The aggregate
gate and native symbol index now include this method. A later full aggregate
run has not yet been completed for this header change; its targeted audit passed.

## Exclusive-mode helper promoted

Recovered `CSystemManager::SetExclusiveMode` at 009a59a0. The retail instruction
span through RET 4 is 137 bytes, including its internal three-byte alignment;
the earlier Ghidra coverage row counts 134. Candidate is 129 bytes, 50 versus
52 instructions, normalized similarity 0.921569: NEAR_MATCH. Three call/import
targets are independently pinned to GetCurrentProcess, SetPriorityClass and
CDisplayManager::SetExclusiveMode. FableWin provides the latter's nested enum
return signature, now declared in the shared display-manager header.

Retail's behavior is unusual but unambiguous: unless changes are prevented,
it makes two attempts, setting the manager flag to the requested mode and then
the previous mode. Each pass sets NORMAL_PRIORITY_CLASS, calls the input
manager's virtual mode method if present, and calls the display method only
when input is absent or accepts the mode. Display results are ignored and the
function always returns false. No inferred success exit or different policy
was introduced. Input callbacks can change the flag before display reads it.

`gate_exclusive_mode.py` passes 768 compiled cases and 768 comparative retail
executions, checking both modes, the guard, absent/present input, independent
acceptance of both attempts, priority results, all three display results,
callback mutation, traces, object writes, saved registers and stack balance.
Process-priority imports and display/input methods are mocked: the gate changes
neither host process priority nor display settings.

The staged display-header declaration passed zero regressions across 160
dependent catalog entries (`exclusive-mode-header-audit-y213xt0e/results.json`).
Production report: `rebuild/compile-gate/exclusive-mode.json`. Backups:
`before-exclusive-mode-20261001/manifest.json`; the one-shot landing script ran.
The aggregate gate and native symbol index include the new method. Latest full
aggregate validation still predates the input-focus/exclusive-mode header edits;
both have passed their targeted staged audits.

## Window initialization promoted

The subsequent `system-dependencies-final-gates-20261001.log` records ALL GATES
PASS, including input focus, exclusive mode, 649 dependent catalog entries and
the visual bootstrap.

Recovered `CSystemManager::InitWindow` at 009a64b0: 348 candidate bytes against
346 retail bytes, 108 versus 107 instructions, normalized similarity 0.920930
(NEAR_MATCH). The native WNDCLASSEXW storage now has named fields in
`fable_window_class.h`, retaining its 48-byte layout and manager offset 0xA4.
The registered callback is declared with its FableWin stdcall signature.

`gate_init_window.py` checks all 12 relocations and passes 720 compiled plus
720 comparative retail cases: window dimensions, screen metrics, centering,
show codes, failed resource/window creation, prior validity, and callbacks
changing the show code or window handle. All window APIs are mocked. Retail
sets WindowValid and returns true even after failed window creation; this
behavior is preserved. The staged header audit reports zero regressions over
68 entries (`init-window-header-audit-026tdlqg/results.json`). Production catalog
grading independently passes NEAR_MATCH/PASS. Report:
`rebuild/compile-gate/init-window.json`; backups:
`before-init-window-20261001/manifest.json`. Its landing script has run.

The aggregate gate now includes window initialization. Native WinProcedure
009a5b60 remains missing from the reconstruction and manifest. Read-only Ghidra
decompilation confirms its entry and full message switch; evidence is in
`win-procedure/headless-decompile.log`. Further recovery is in progress, with
the callback's activation/sound dependencies being traced before promotion.

## Activation helper promoted

`CSystemManager::ActivateWindow` 009a5a50 is now RELOCATION_MATCH: all 268 bytes
match after 12 checked relocations, with 83 instructions. The FableWin static
fastcall signature and retail callers establish ECX manager / DL active.
Shared sound slots 46, 47 and 49 now have caller/callee-verified signatures;
the CSoundManagerLH vtable targets are pinned in `gate_system_activation.py`.
The saved-volume and deactivated globals retain address-qualified descriptive
names because their original names have not been recovered.

The gate passes 512 candidate-linked cases and 512 comparative executions,
covering activation history, restore flags, input/sound absence, already-muted
state, float return/output handling, and callbacks replacing input or sound
receivers. No real audio or input devices are touched. The header audit first
caught obsolete empty-signature sound mocks in the frontend Run fixture; after
correcting those mocks, zero regressions remain across 69 entries
(`activation-header-audit-nm48cg35/results.json`). Production catalog grading is
RELOCATION_MATCH/PASS. Backups: `before-activation-20261001/manifest.json`;
the one-shot lander has run. The aggregate gate includes this helper.

The direct sound helper 009915e0 has an existing, incorrect CWorld label and a
local synthetic CWorld declaration. Native activation and window callbacks
both pass PSoundManager (+0x7C); the callee toggles sound storage at +0x25 and
dispatches sound slot 18 on disable. The new activation source declares it as
`CASoundManager::UnknownSetEnabled_009915e0`; migrating the old implementation
and metadata to that owner remains open. No false claim of resolved native
linkage is made. WinProcedure code spans 009a5b60..009a5f7b, followed by its
dispatch tables through 009a60c5; do not truncate the function at an indirect
jump or treat table bytes as executable instructions.

### Sound-owner correction and bootstrap follow-up

The first aggregate activation run caught two additional bootstrap fixtures
using the old sound-slot signatures (00418c3b and 009a6610). Both mocks were
updated, as was the authored system-manager runtime boundary. The two leaves
then pass individually with unchanged relocation parity. That aggregate run
passes 651 header dependents, native GFMain, all new helper gates, bank ring and
VISUAL_BOOT_CHECKPOINT; its overall result remains FAILED because its initial
bootstrap-leaf stage preceded those fixture fixes. The corrected aggregate
rerun is `window-activation-corrected-gates-20261001.log` (without rebuilding
the already-passing visual checkpoint).

The 009915e0 migration is now complete: `CASoundManager::UnknownSetEnabled_009915e0`
uses a shared partial storage view and matches all 32 retail bytes. Its new
candidate-linked fixture checks all eight combinations of prior flag, requested
flag and callback mutation, including unchanged surrounding storage and the
receiver/argument/state visible to slot 18. Standalone fixture linking must fail.
The old separate-model fixture and synthetic CWorld source were backed up and
replaced. Catalog, manifest, oracle, artifact index and symbol index now name
the sound owner. No additional function coverage is claimed for this correction.
Gate: `gate_sound_enabled.py`; proof: `rebuild/compile-gate/sound-enabled.json`;
backup: `before-sound-enabled-20261001/manifest.json`.

## Window callback promoted

The corrected preceding aggregate run is ALL GATES PASS: 123 bootstrap leaves,
652 header-dependent entries, all native helper gates and the bank ring. The
visual checkpoint had already passed after the sound-signature fixes.

Recovered `CSystemManager::WinProcedure` 009a5b60 at NEAR_MATCH: 1,078 executable
bytes / 371 instructions versus retail's 1,052 / 372, similarity 0.923284.
The candidate also contains two alignment bytes and 330 dispatch-table bytes,
for 1,410 bytes total against retail's 1,382. Compiler flags include /Oa.
No further parity tuning followed acceptance.

This was a manifest gap. InitWindow's instruction at 009a64d9 stores 009a5b60
as its WNDCLASSEXW callback; the immediate lives at 009a64df. FableWin provides
`?WinProcedure@CSystemManager@@CGJPAUHWND__@@IIJ@Z`; read-only Ghidra decompilation
and complete retail disassembly confirm the entry, stdcall return and message
dispatch. The manifest row records `agent_source=native_callback_xref` and
points to the verifier that pins the registration reference. No Ghidra project
changes were saved. The full oracle includes every table through 009a60c5.

`gate_window_procedure.py` validates 63 candidate relocations, all internal
table targets, both selector-byte tables, explicit code/table boundaries and
3,253 comparative executions. The candidate-linked VC7.1 fixture checks the
same retail-derived state and call-trace hashes; standalone linking must fail.
Coverage includes every message 0..0x220, create/destroy, focus and activation,
gamma restore/minimize, paint, system keys, power/system commands, character
input, mouse buttons/wheel signs, default dispatch, IME interception and IME
replacement of the current manager. Callback mutations require global-manager
reloads. Null-manager creation/default cases are included. No real windows,
input devices, display settings or audio are touched.

A wrong system-key constant still clears structural validation but fails the
retail behavior comparison (`win-procedure/negative-key.log`). Validator dispatch
also rejects unsupported validators and structural changes; evidence is under
`win-procedure/window-validator-dispatch-k_ex_x2u`. Catalog header grading now
explicitly routes this entry through its complete-body gate, avoiding accidental
instruction scoring of embedded table data.

The shared display header names Gamma at retail +0x2C4 and declares its methods.
The new character-buffer header follows Ego_r's 0x1A0 layout, independently
checked against retail AddChar: 100 four-byte CCharEvent entries, FirstChar
+0x198 and LastChar +0x19C. The staged display-header audit reports zero
regressions across 162 entries (`win-procedure-header-audit-mr03nqbk/results.json`).
The expected fixture table is regenerated and checked against retail on every
gate run. Backup: `before-win-procedure-20261001/manifest.json`; landing completed.
The normal selected-catalog build and a new aggregate/visual run are in progress.

### Callback validation completed; remaining leaves

The selected normal catalog build passes all three requested objects. The new
`window-procedure-final-gates-20261001.log` is ALL GATES PASS, including 653
header dependents and VISUAL_BOOT_CHECKPOINT.

Promoted `SetAsActive` 009a5270 (60 bytes, 19 instructions, NEAR_MATCH 0.947368),
retyped `SetHasInputFocus` 009a5a30 onto the real shared class/member ABI
(26-byte RELOCATION_MATCH), and recovered `CInkeyBuffer::AddChar` 00a5fa70
(76-byte MATCH). The focus source previously exported a synthetic fastcall
wrapper and described the input receiver through a local padded class; its
replacement resolves the native member used by WinProcedure. No added coverage
is claimed for that correction.

`gate_window_leaves.py` passes 16 active-state compiled/comparative cases,
16 focus cases with callback mutation, and 120,000 character-ring cases. The
ring fixture exhausts all 100x100 FirstChar/LastChar pairs, six characters
(including zero, high-bit and surrogate values) and pressed/released states;
full-buffer rejection, wraparound and unchanged padding/storage are checked.
All fixtures require the candidate object. The three catalog grades pass.
The new character-buffer header also gained its own stddef include so it
compiles independently under VC7.1. Backup:
`before-window-leaves-20261001/manifest.json`; one-shot promotion completed.
These three leaves postdate the latest aggregate run and have passed their
production gate and catalog grades. The aggregate gate now includes them.

## Gamma-ramp setter promoted

Recovered `CDisplayManager::SetGamma` 009beaa0: RELOCATION_MATCH across all
266 bytes, including nine checked relocations and the /GS cookie path. It
clamps the input to 0.3..6.0, stores Gamma, constructs three identical linear
channels (input step 256, output capped at 65,280), then calls D3D9 slot 21
with swap chain zero and the calibration flag. The shared D3D gamma-ramp
definition is 0x600 bytes; the formerly generic bool parameter is named calibrate.
The native upper constant is 65,280, not the maximum WORD value.

`gate_set_gamma.py` passes 84 candidate-linked cases and 504 retail comparisons.
Inputs cover clamp neighbors, signed zero, subnormals, infinities, quiet and
signaling NaNs; the emulator repeats them across six x87 control settings and
executes retail's actual float-to-integer helper. The D3D call and cookie check
are mocked, verifying all 768 ramp entries, receiver, flags, stored gamma,
callback mutation, unchanged object storage, saved registers and stack balance.
No physical display settings change. Catalog grading also passes.

Production report: `rebuild/compile-gate/set-gamma.json`; backup:
`before-set-gamma-20261001/manifest.json`. The one-shot promotion ran. This leaf
postdates the last full aggregate/visual run and is included in future aggregate
runs. Native dependency closure is being refreshed after the window-chain work.

## Updated dependency closure and duplicate providers

The refreshed inventory (`native-gfmain-window-closure-20261001`) checked 532
objects in its last round, reporting 669 unresolved symbols and six duplicate
groups; 553 sources were selected for the next round. More dependencies are
visible because the recovered window chain is now included. No closure executable
was run.

Five redundant objects were proved identical to bodies already emitted by
selected providers: active/focus setters, the two three-argument world-path
helpers, and surface pixel format. Every defined symbol, full section bytes,
offset and relocation map is checked; the focus object's extra counted-pointer
accessors are supplied identically by UpdateActive. Proof:
`window-link-provider-proof.json`, with object hashes.

The remaining base-constructor conflict was resolved by making the constructor
definition inline, as its copies in derived-class TUs already are. All five
catalog functions sharing the source remain RELOCATION_MATCH/PASS; evidence
`link-provider-staging/base-constructor-proof.json`, backup
`before-base-constructor-inline-20261001/manifest.json`. No function body or
class layout changed.

The reviewed scratch link now contains 527 objects, **zero duplicate definitions**
and the same 669 unresolved symbols (`coalesced-proof.json` and
`coalesced-link-diagnostics.txt`). Redundant-object omission remains a scratch
selection, not a blind change to production source selection. Native GFMain
still does not produce a complete executable; the runnable visual checkpoint
continues to use the authored coordinator.

## Application-name parser promoted

`CSystemManager::InitApplicationName` 009a5340 is NEAR_MATCH: 284 candidate
bytes / 83 instructions versus 274 / 82, similarity 0.969697. All 12 relocation
targets are checked, including the command-line import, four CRT wide-string
thunks, wide/char temporary operations and the /GS cookie. The implementation
preserves retail's first-token parsing: initial quote selects quote as delimiter;
otherwise space is the delimiter. Only backslash paths assign ApplicationName.

The linked fixture passes 24 cases; 96 retail comparisons also vary stack
alignment and prior name storage. Quoting, missing quotes, leading whitespace,
tabs, slash-only paths, UNC paths, Unicode, trailing separators, empty input and
the longest terminated command within the 261-wide-character copy are covered.
String receivers, conversion/assignment/destructor ordering and unchanged
manager storage are checked. The command-line import is redirected locally in
the fixture and restored. Retail does not force termination after an overlong
copy; inputs beyond that native safe bound are not claimed as validated.

Gate: `gate_application_name.py`; production catalog grade NEAR_MATCH/PASS;
backup `before-application-name-20261001/manifest.json`. Its landing script has
run and the aggregate gate includes it. System-colour initialization is staged
next, with the named 25-color PDB layout and 792-byte relocation match; the
shared-header audit is in progress before promotion.

## System colours promoted

`CSystemManager::InitSystemColours` 009a5490 matches all 792 retail bytes
after its GetSysColor relocation. The named CGuiColoursInfo layout comes
from Ego_r.pdb and is checked against all 25 native field offsets. The
candidate-linked fixture passes 512 palette scenarios, checking call order,
all channel values, ignored high COLORREF byte, opaque alpha and unchanged
surrounding state. The shared-header audit passes all 74 affected entries
with zero regressions (`system-colours-header-audit-qa0ck2lb`).

Source, test, shared headers, catalog, oracle and metadata are promoted; normal
catalog build passes. Gate: `gate_system_colours.py`; backup:
`before-system-colours-20261001/manifest.json`. The aggregate gate, including
visual bootstrap, is running in `system-colours-final-gates-20261001.log`.

## Aggregate validation and default error callback

`system-colours-final-gates-20261001.log` passes all individual native gates,
123 bootstrap leaves, component ABI, ring linkage and VISUAL_BOOT_CHECKPOINT.
The broad header audit is **incomplete**: Windows Defender blocked
`Temp/hdrgate_vitnj4ag/009a5490_2fbnjzpa/009a5490.exe` with WinError 225.
Get-MpThreatDetection confirms the 2026-10-01 07:47 event (ThreatID 2147651817).
No security settings were changed, no blocked artifact restored, and the
blocked generic fixture was not rerun. Dedicated system-colour and preceding
74-entry staged-header checks had passed. This aggregate is GATES FAILED,
not a new all-green checkpoint.

`CSystemManager::DefaultErrorFunc` 009a5260 is now promoted at exact MATCH
(three bytes, returns false), with nine candidate-linked enum/boundary cases.
Native Initialise instruction 009a671a stores 009a5260 at ErrorFunc +174;
FableWin.pdb supplies the private static fastcall bool(ESystemCode) signature
(02f8c2e0). Read-only Ghidra confirms the body. This was a manifest gap; the
new row records native-callback discovery. Normal catalog build passes.
Backup: `before-default-error-20261001/manifest.json`.

Shutdown 009a60d0 is staged next. PDB PendingLoadedGlyph (16 bytes) and retail
font Preload confirm the 512-entry pending glyph queue and its counted async
data pointers. No shutdown code is promoted yet.

## System shutdown promoted

`CSystemManager::Uninitialise` 009a60d0 is promoted at NEAR_MATCH: both
bodies are 667 bytes / 218 instructions, similarity 0.981651. All 31
relocations and import identities are checked. The accepted difference is
the entry boolean test versus retail's compare-to-true; validated values
obey the public 0/1 bool ABI. No further byte tuning was performed.

192 compiled cases and 2,304 comparative cases cover valid/invalid managers,
empty/exclusive/shared/underflowed reference counts, destructor replacement
of ownership records, glyph-array replacement within a destructor, zero/short/
512-entry queues, window ownership, scratch state and instance-byte wrap.
The fixture verifies critical-section deletion resolves to its local mock
before executing; no host window or subsystem teardown is performed.

PendingLoadedGlyph's 16-byte PDB layout is shared in a new header; the
drive singleton gains its inline Release method. The 57-entry affected-header
audit has zero regressions (`system-shutdown-header-audit-hf01yf4f`).
Production catalog build passes. Gate: `gate_system_shutdown.py` and
`system_shutdown_oracle.py`; backup `before-system-shutdown-20261001`.
The fixture guard was added while the full comparison gate was running, then
compiled and run separately; `fixture-proof.json` pins its current hash and
192-case result, matching the landing proof.

Next: constructor 009a7440 and destructor 009a5800, read-only Ghidra evidence
in `system-lifetime-decompile.log`. The full aggregate limitation from Defender
remains open; no claim of a new all-green aggregate.

## System-manager lifetime promoted

Constructor 009a7440 (118 bytes) and destructor 009a5800 (416 bytes) match
retail after all 18 relocations. Their genuine C++ bodies rely on ordinary
base/member construction and reverse destruction. The 16 linked scenarios
check untouched scalar bytes, strings, counted-pointer initialization,
exclusive/shared/zero-count owners, destructor ordering, replacement Info
pointers and vtable changes. Production catalog builds both methods and
the adjusted GUID fixture successfully.

The staged declaration audit passed 76 entries with zero regressions. One
entry, 009a5490, was explicitly excluded because of the recorded Defender
block; this is not a complete 77-entry audit. The first audit was interrupted
after identifying the GUID fixture's dependency on the formerly implicit
constructor. That GUID-only test now uses initialized aligned raw storage.
Evidence: `system-lifetime-header-audit-_kaws8ny`, `gate_system_lifetime.py`,
backup `before-system-lifetime-20261001`. A fresh visual-bootstrap build is
running in `system-lifetime-bootstrap-20261001.log`.

Follow-up inspection found Update 009a6460 still uses a synthetic local
observer/system layout. Retail +7C is PSoundManager and virtual slot +0C is
CASoundManager::UnknownSlot03. Retyping this existing body onto the shared
layout is next; no additional function coverage is claimed for that correction.

## Update layout correction and visual checkpoint

Update 009a6460 now uses shared CSystemManager/CASoundManager types and
ESR_OK/ESR_QUIT names. It remains a 65-byte relocation match with the real
UpdateActive target and sound virtual slot +0C. 512 candidate-linked scenarios
cover all initial/post-UpdateActive flag combinations and sound callbacks
that change quit/restore flags. Source/test/catalog marker updated, production
gate and catalog build pass. No new function coverage. Backup:
`before-system-update-20261001`; gate `gate_system_update.py`.

The later `system-lifetime-bootstrap-20261001.log` reaches
VISUAL_BOOT_CHECKPOINT PASS. This still uses the authored GFMain coordinator
and documented subsystem boundaries. The native closure is being rebuilt
from the previously checked 527-object coalesced selection plus the new system
providers, in `native-gfmain-system-closure-20261001`; no resulting native
executable is run. The Defender-related aggregate gap remains open.

## Refreshed native closure

Fresh builds in `native-gfmain-system-closure-20261001` checked 554 objects
on the second link pass, with 667 unresolved symbols and one duplicate.
The extra WriteSLONG body in WriteDateAndTime is byte/relocation-identical
to the standalone WriteCBYTE-named provider, covering all public definitions
of the omitted object. The checked scratch selection is now **553 objects,
zero duplicates, 667 unresolved** (`coalesced-proof.json`). The next-round
selected list contains 567 sources; do not mistake it for the linked count.
All recovered CSystemManager method dependencies are present. Globals and
the wider display/input/resource systems remain unresolved. No native
closure executable was run.

Display callback methods are staged next: AddDeviceResetCallback 009c0190
is registered by Initialise call 009a697e; RemoveDeviceResetCallback 009bf5a0
is called by system shutdown. Their former manifest names are incorrect
(CMainGameComponent_AddComponentToList / LSocketServer::CloseSocket). PDB
method signatures and DeviceResetCallbacks offset +264 corroborate native
list insertion/removal. No display callback method is promoted yet.

## Display reset registration promoted

AddDeviceResetCallback 009c0190 matches 47 relocated bytes; removal
009bf5a0 is NEAR_MATCH (51 versus 59 bytes, 20 versus 22 instructions,
similarity 0.904762). Stop-after-acceptance applies; no additional tuning.
216 candidate-linked list scenarios and 480 retail comparisons verify
empty lists, null/duplicate callback values, first-match removal, link repair,
allocation/free ordering and allocator-time sentinel replacement.

The shared display view names the list sentinel pointer at +264 without
introducing unverified owning-destructor behavior. Its 166-entry header audit
has zero regressions (`device-reset-header-audit-8mc4w1mw`). Catalog builds
both methods successfully. Incorrect manifest owners/names corrected using
PDB signatures, list layout and native system-initializer/shutdown callsites.
Gate: `gate_device_reset_callbacks.py`; backup `before-device-reset-20261001`.

The pre/post-reset dispatchers and their three small system/timer callees
are now staged with exact/relocation matches. Header audit and linked
behavioral fixtures are in progress; they are not promoted yet.

## Reset phases and callback ABI correction promoted

Five bodies (167 bytes total) are promoted: display PreDeviceReset 009bf500
(66 bytes) and PostDeviceReset 009bf550 (78); system OnPreDeviceReset 009a52b0
(one-byte no-op) and OnPostDeviceReset 009a52c0 (three-byte true); timer
OnPreDeviceReset 009f73a0 (19-byte /Od call to ClearAllQueries 009f7250).
All are exact/relocation matches. The display view now names SystemManager
at +4; render-core and callback declarations follow the PDB signatures.

1,680 dispatch scenarios check renderer/timer order, empty and replaced
callback lists, callback-induced early termination and changed system-manager
receivers. Post-reset short-circuits failed listeners but ignores the final
system callback's boolean, as retail does. The tiny system hooks pass 256
linked cases; the timer wrapper passes 512 receiver/mutation cases.
The 180-entry staged-header audit has zero regressions; the known blocked
color fixture remains explicitly excluded (`device-reset-phases-header-audit-5ft50yyx`).

Both tiny system hooks already had catalog artifacts: 009a52b0 used a generic
T::Hook with a print-only fixture, and 009a52c0 used fn_operator with a
self-contained test and incorrect CThingFilter owner. Their old artifacts
were backed up and replaced with actual member ABIs and candidate-linked tests;
no additional function coverage is claimed for those corrections. Timer
009f73a0's incorrect operator_new metadata is corrected.

Gate: `gate_device_reset_phases.py`; all five production catalog builds pass.
Backup `before-device-reset-phases-20261001`. Next: render Init/Uninit and
timer ClearAllQueries. The full aggregate still has the recorded security
block; do not report ALL GATES PASS.

## Render-core reset dependencies promoted

CRenderManagerCore::Init 00a0a940 (130 bytes), Uninit 00a0a9d0 (28), and
CSystemFont::Uninitialise 00a6c120 (five-byte tail call) all match retail
after relocation. Init's 320 linked setup/cleanup scenarios cover device
selection changes inside callees, ignored font/HRESULT failures, capabilities
writes, back-buffer pixel format and invalid/valid font cleanup. The font
thunk passes 512 receiver/state cases.

The render-core view is extended to the PDB/native 0x3D20 layout with named
fields used by these methods; embedded SystemFont retains storage representation
to avoid introducing unverified constructor/destructor behavior. Its 171-entry
header audit has zero regressions (`render-core-reset-header-audit-iece4cc2`).
All three production catalog builds pass. The old five-byte generic Forward/
ForwardTarget source and print-only fixture were backed up and replaced;
00a6c120 is a corrected existing body, not new coverage.

Gate `gate_render_core_reset.py`; backup `before-render-core-reset-20261001`.
Next: timer ClearAllQueries 009f7250. Its retail /Od body uses out-of-line
STLport list helpers and normal COM Release calls; headless decompiler stack
recovery is poor, so native disassembly is required before implementation.

## Timer cleanup promoted

CVsyncCallbackTimer::ClearAllQueries 009f7250 is accepted NEAR_MATCH: 169
compiled bytes versus 211 retail, 59 instructions each, similarity 0.983051.
Eight helper relocations are anchored to native callsites. Its 300 linked
scenarios and 300 retail comparisons cover reverse pending/free cleanup,
ignored COM Release results, and reentrant additions to either list.
The PDB/native timer view now names both STLport lists and the 16-byte query
record. The 173-entry header audit has zero regressions
(`timer-clear-header-audit-k5sumzx1`); production catalog validation passes.
Gate `gate_timer_clear.py`; backup `before-timer-clear-20261001`.

The preceding render-core batch also passed visual bootstrap
(`render-core-reset-bootstrap-20261001.log`). The aggregate security block
recorded above remains unresolved; this is not an all-gates-pass claim.
Next: CRenderStateManager::Init 00a07900 and remaining timer lifetime.

## Render-state initialization promoted

CRenderStateManager::Init 00a07900 is accepted NEAR_MATCH: 117 versus 115
bytes, 33 versus 32 instructions, similarity 0.984615. Four relocation targets
are checked against retail. Candidate-linked and comparative fixtures each
pass 224 scenarios (negative/empty/full state counts, duplicate pointers,
callee mutations, ordering, complete surrounding storage). Retail
SetDefaultRenderStates preserves ECX: its 1,995 decoded instructions contain
no calls and only the identity `lea ecx,[ecx]` writes ECX. The candidate's
extra receiver reload is harmless; the comparative boundary preserves the
native contract. No further parity tuning was performed.

The 174-entry header audit has zero regressions
(`render-state-init-header-audit-o4zk84l9`). Production catalog compilation
passes. Gate `gate_render_state_init.py`; backup
`before-render-state-init-20261001`. Next: native timer lifetime.

## Timer lifetime promoted

CVsyncCallbackTimer constructor 009f70e0 is NEAR_MATCH (342 versus 354 bytes,
84 versus 88 instructions, similarity 0.965116); destructor 009f7330 matches
all 107 relocated bytes. Native construction leaves Device untouched, creates
two 24-byte sentinel nodes, zeroes shared timing data, and sets pending/finished
frames to 1/0. Destruction calls ClearAllQueries then destroys pending and free
lists in that order, re-reading each head after clear. The source uses normal
C++ member construction/destruction with local STLport specializations.

Both linked bodies pass 1,024 lifetime scenarios, including sentinel changes
during clear. The constructor passes 64 retail comparisons with allocation
callbacks mutating object state. Both production catalog grades and behavioral
fixtures pass; no production layout changes were needed. Incorrect prior
Init_ListHeads/global and screen-effect ownership metadata is corrected.
Gate `gate_timer_lifetime.py`; backup `before-timer-lifetime-20261001`.
Next: list clear 009f7830 and refresh the native dependency closure.

## Timer list cleanup promoted

STLport pending-query list clear 009f7830 is NEAR_MATCH: both bodies are
92 bytes / 37 instructions, similarity 0.945946. Its free target is verified
against retail. It frees nodes in forward order and resets both sentinel links;
query payloads have trivial destruction and do not perform COM Release here.
The 512 linked heap cases and 512 comparative cases pass, including poisoning
freed nodes to verify next-pointer capture. Gate `gate_timer_list_clear.py`;
backup `before-timer-list-clear-20261001`. No production header changes.

## Render closure and visual checkpoint

The refreshed scratch native closure links 577 objects with zero duplicate
definitions and 659 unresolved symbols. Both closure rounds completed; the
second adds no providers. Inventory is in
`native-gfmain-render-closure-20261001`; this precedes system-font initialization.
The latest visual bootstrap passes (`timer-lifetime-bootstrap-20261001.log`).
Native GFMain remains unlinked; the runnable checkpoint still uses its authored
coordinator and documented boundaries. The broader security-blocked aggregate
remains incomplete.

## System-font initialization promoted

CSystemFont::Initialise 00a6c170 matches all 805 bytes after 154 verified
relocations, including nonzero table/glyph addends. Its 512 linked scenarios
check all 256 mappings, already-initialized behavior, base-initialization
failure, and state changes during the base call. Expected mappings come from
executing the retail initializer independently. PDB confirms the four-byte
CSystemFontCharacter Elements pointer; named extern storage keeps data
providers explicit. No existing production headers were changed.
Gate `gate_system_font_init.py`; normal catalog reports RELOCATION_MATCH/PASS.
Backup `before-system-font-init-20261001`. Next: remaining system-font leaves.

## System-font character access promoted

GetCharacter 00a6c130 (15 bytes), CSystemFontCharacter::Initialise 00a6c140
(11), and IsElementSolid 00a6c150 (26) all match retail (lookup has one
table relocation). The 9,729 linked cases cover all character values, null
and non-null element pointers, and every byte value at all 36 bitmap cells.
The native solidity body and PDB signatures confirm a six-byte row stride.
Four affected header consumers pass without regressions. The functions were
missing from the manifest; identities are corroborated by both PDBs, adjacent
native bodies, and storage used by the recovered initializer. Static Ghidra
xrefs are empty for these small out-of-line copies.

Gate `gate_system_font_leaves.py`; all three normal catalog checks pass.
Backup `before-system-font-leaves-20261001`. Bitmap storage remains an explicit
unresolved data dependency; no guessed provider is introduced. Next:
CRenderStateManager::ForceResendRenderStatesToHardware 00a044e0.

## Forced render-state resend promoted

CRenderStateManager::ForceResendRenderStatesToHardware 00a044e0 is accepted
NEAR_MATCH: 146 compiled versus 151 retail bytes, 61 versus 63 instructions,
similarity 0.951613. Validation includes the second native return path for
empty/nonpositive counts. Both 1,152 linked and 1,152 comparative scenarios
pass across all dispatch types, duplicate states, live count changes, device
replacement, ignored HRESULTs, and callback changes to cached/desired values.
Cached state and dirty flags are written before device calls; callback writes
are retained. No production headers changed. Normal catalog check passes.
Gate `gate_render_state_resend.py`; backup `before-render-state-resend-20261001`.
Next: retype the existing RealiseRenderState partial view to the shared manager.

## Ordinary render-state update retyped

The existing RealiseRenderState 00a058c0 now defines the actual shared
CRenderStateManager member instead of CRenderStateManagerRealiseView. Its
bootstrap storage view forwards through a documented inline adapter. This
corrects linkage and is not new function coverage. The prior exact body used
a separate byte-flag layout; shared bool fields compile to accepted NEAR_MATCH
(161 versus 167 bytes, 65 versus 67 instructions, similarity 0.969697).
No further byte tuning was performed after acceptance.

Both 2,304 linked and 2,304 comparative cases pass, including unchanged values,
unsupported state types, duplicate states, callbacks changing desired values,
live count and device changes, and ignored HRESULTs. Unlike forced resend,
this body reloads DesiredState after a callback before caching CurrentState.
The 180-entry header audit has zero regressions
(`render-state-realise-header-audit-zhttin9j`); normal catalog validation passes.
Gate `gate_render_state_realise.py`; backup `before-render-state-realise-20261001`.
A fresh visual-bootstrap check is running. Next: render-state setup helpers.

## Render-state registration helpers promoted

SetupRenderState 00a04580 (47 bytes), SetupTextureStageState 00a045b0 (54),
and SetupSamplerStageState 00a045f0 (54) all match retail exactly. PDB names
and signatures identify these previously unlisted bodies; native setup inlines
the same registration sequence. All 1,680 linked cases pass, preserving
bookmark/padding fields and testing list positions 0/1/127/255, enum bit
patterns, and signed stage values truncated to one byte. Minimal SDK enum
identities are corroborated by the installed d3d9types.h.

The 180-entry header audit has no regressions
(`render-state-setup-helpers-header-audit-_m_n87m8`). All three normal catalog
checks pass. Gate `gate_render_state_setup_helpers.py`; backup
`before-render-state-setup-helpers-20261001`. The initial harness name containing
Setup triggered Windows installer detection (error 740); using the standard
fixture.exe harness name runs without elevation. No security settings changed.
The earlier system-color malware block remains a separate unresolved gap.

The RealiseRenderState visual bootstrap passes
(`render-state-realise-bootstrap-20261001.log`). Next: larger SetupRenderStates
00a04630; PDB CDeviceState has 77 named fields totaling 0x120C bytes.

## Complete render-state setup promoted

CRenderStateManager::SetupRenderStates 00a04630 matches all 4,609 relocated
bytes (812 instructions, eight wrap-state table relocations). The full
CDeviceState view now names its 77 PDB fields while preserving the 0x120C
layout. Native execution independently established registration order and
capability branches: RasterCaps controls depth bias, slope bias and mip LOD
bias; InitFlags bit 1 controls texture transforms. There are 207 to 225
registrations depending on those flags. The method appends to the existing
list rather than clearing it.

The 3,072 linked cases cover capability combinations, preexisting counts,
initialization flags, ignored HRESULTs, changed wrap-state table values, and
GetDeviceCaps callbacks changing count/flags/device. The 183-entry header
audit has zero regressions (`render-state-setup-header-audit-cvc9f5we`).
The three registration helpers remain exact with their 1,680 linked cases.
All four bodies now share CRenderStateManager_StateSetup_00a04580.cpp; the
old standalone helper source is backed up and retired to avoid duplicate
providers. Gates `gate_render_state_setup.py` and the updated helper gate.
Backup `before-render-state-setup-20261001`. Next: SetDefaultRenderStates.

## Default render states promoted

CRenderStateManager::SetDefaultRenderStates 00a059e0 is NEAR_MATCH:
7,388 compiled versus 7,958 native bytes, 1,870 versus 1,995 instructions,
similarity 0.910220. Its 228 ordered setter operations use named device-state
fields. Native execution independently established the operation sequence,
including the two texture-factor assignments and stage-zero overrides.
The candidate reads the same 21 table entries as retail; compiler reuse
reduces 37 native loads to 33 relocated loads. Table storage remains explicit
extern dependencies rather than guessed providers.

Both 486 linked and 486 retail comparisons pass across current bookmark
masks (including zero), existing bookmark/dirty flags, values already at
defaults, partial stack/update-list prefixes, and alternate lookup values.
Saved register and complete object-memory comparisons pass. No existing
production header changed. Normal catalog reports NEAR_MATCH/PASS.
Gate `gate_render_state_defaults.py`; backup `before-render-state-defaults-20261001`.
Next: composed render-state initialization gate, refreshed closure and bootstrap.

## Composed render-state initialization validated

`gate_render_initialization_chain.py` executes four recovered bodies together
(Init, SetupRenderStates, SetDefaultRenderStates, ForceResendRenderStatesToHardware)
and compares the complete object memory and ordered device-call trace with
the retail chain. All 768 scenarios pass, including old registered states,
capability combinations, initialization flags, device/flag mutations during
GetDeviceCaps, failed HRESULTs, and altered lookup data. Only GetDeviceCaps
and D3D state setter boundaries are mocked; the four recovered methods call
each other directly. Saved registers and stack cleanup are verified.
This is composed x86 validation, not a newly runnable native GFMain executable.

The refreshed link inventory and visual-bootstrap build are running. Next:
CVsyncCallbackTimer::OnPostSwapScreens 009f73c0.

## Render-state closure checkpoint

The refreshed native scratch link checks 583 objects with zero duplicate
definitions and 681 unresolved symbols (`native-gfmain-render-state-closure-20261001`).
The increase from 659 reflects explicit font/lookup-data dependencies exposed
by the new bodies; it is not a claim that native executable integration is
complete. The latest visual bootstrap passes
(`render-state-chain-bootstrap-20261001.log`). No native GFMain executable was
run. The previously recorded generic system-color security block remains.

009f7830 metadata now names its actual STLport specialization rather than
the descriptive source-stem alias; bytes and tests are unchanged. Backup
`before-timer-list-owner-20261001`.

## Timer post-swap callback promoted

CVsyncCallbackTimer::OnPostSwapScreens 009f73c0 is NEAR_MATCH: 603 compiled
versus 732 retail bytes, 184 instructions each, similarity 0.956522. All
35 relocation targets are checked against native instructions. Both 2,304
linked and 2,304 comparative cases pass, covering completed/pending/failed
queries, Sleep(0) retries, strict S_OK creation checks, ignored Issue HRESULTs,
recycling order, frame counters, and timing-buffer changes inside GFGetTime.
A compiled C++ clock stub supplies the x87 return in comparative tests.

Retail leaves new query records' Frame and TimeStamp members uninitialized.
The reconstruction preserves that behavior; fixtures verify only defined
bytes for those newly created records and fully verify initialized records.
D3D9 query slots and CreateQuery slot 118 match the installed SDK. STLport
push_back is exposed, and the 308-entry header audit has no regressions
(`timer-post-swap-header-audit-v602divq`). Normal catalog passes.
Gate `gate_timer_post_swap.py`; backup `before-timer-post-swap-20261001`.
Next: native pending-query node creation and front/back insertion helpers.

## Timer query-node helpers promoted

Recovered node creation 009f7ad0 and front/back insertion 009f7e40/009f7ec0
as ordinary STLport specializations. All three are NEAR_MATCH (131/137,
113/119 and 111/117 bytes; similarities 0.977778, 0.952381 and 0.951220).
The shared header adds only the protected node-creation declaration.
The internal name is inferred from the STLport role, supported by native
allocation/copy behavior and the upstream protected `_M_create_node` member;
it is not a claim that the available upstream revision is retail's exact version.
Reference: https://android.googlesource.com/platform/external/stlport/+/e64e7c849b0b659d3e198e4efaed563601e554be/stl/_list.h

768 linked scenarios cover 24,576 insertions with payload aliases and mixed
front/back order. All 720 retail comparisons pass, including allocation-time
list mutation and saved-register/stack checks. The timer post-swap fixture
also passes all 2,304 cases linked with these actual node operations instead
of its insertion mock. An explicit insert instantiation supplies the helper
needed by existing weak inline push providers; only the three identified
native functions receive coverage. Allocation failure remains undefined as
in retail and is not given invented recovery behavior.

309 header dependents have zero regressions
(`timer-query-nodes-header-audit-ppdcsy1h`). All three normal catalog entries
pass. Gate `gate_timer_query_nodes.py`; backup
`before-timer-query-nodes-20261001`. The earlier aggregate security block
remains; no full aggregate pass is claimed. Next: refresh native link closure
and recover the display-mode/exclusive-mode frontier.

## Display mode setters promoted

SetDisplayMode 009bff40 and SetExclusiveMode 009c0020 match all 222 and
146 relocated bytes. The PDB-listed CCreateDeviceParameters copy constructor
explains the memberwise copy, including its untouched three padding bytes.
2,496 linked cases cover unchanged requests, signed minimum-dimension clamping,
all reset return codes, preserved creation settings, and a format-query callback
that mutates retained settings before the copy. 181 shared-header dependents
have zero regressions (`display-mode-header-audit-nh3y82x2`); both normal catalog
entries pass. C2DExtentsI moved unchanged into a small shared header so the
44-byte nested creation parameters can replace the opaque +0x268 storage.
The parameter default constructor remains a declaration pending recovery.
Gate `gate_display_mode.py`; backup `before-display-mode-20261001`.

The preceding timer link refresh checks 585 objects, zero duplicates and 680
unresolved symbols (`native-gfmain-timer-closure-20261001`). No native executable
was run. Next: display reset/device-creation helpers and their native evidence.

## Presentation and copy-texture helpers promoted

CalculateD3DPresentationInterval 009be2d0 matches the complete 84-byte body,
including its 20-byte jump table. SetupDepthAndBackBufferTextures 009bee40
is NEAR_MATCH (101 bytes each, 34 instructions, similarity 0.941176).
2,056 interval cases and 1,024 linked texture cases pass, as do 1,024 native
texture comparisons with format callbacks changing dimensions/capabilities.
The fixed 1024-square power-of-two branch is preserved. ESurfacePool values
and the texture-initialization signature are PDB corroborated. 183 shared-header
checks have zero regressions (`display-reset-helpers-header-audit-63uutiln`);
normal catalog checks pass. Gate `gate_display_reset_helpers.py`; backup
`before-display-reset-helpers-20261001`.

The subsequent visual bootstrap initially exposed a missing default-constructor
provider in its authored CDisplayManager fixture/boundary after the new typed
member was added. Explicit boundary-only empty definitions now retain that
mock behavior; no native defaults were invented or credited. The rebuild is
running (`display-mode-bootstrap-fixed-20261001.log`). Next: GetClosestMode;
first draft is accepted NEAR_MATCH (463/469 bytes, similarity 0.90625), pending
behavior/comparative tests and header audit.

## Display-mode selection promoted

GetClosestMode 009bf600 is NEAR_MATCH: 463/469 bytes, 159/161 instructions,
similarity 0.90625. 456 linked and 456 comparative scenarios pass. The fullscreen
selection requires both depth difference and resolution difference to be no worse;
equal candidates replace earlier ones. Windowed mode queries the existing device
or adapter, then uses the requested dimensions. HRESULTs are ignored as in retail.
No candidates leave three output fields uninitialized; tests verify only the
explicit D3DFMT_UNKNOWN field in that case. Callback mutation tests verify reads
of requested dimensions after querying pixel format or display mode.

BackBufferFormats now uses the existing retail CArrayVector representation at
+0x298. 185 shared-header checks have zero regressions
(`display-closest-mode-header-audit-lk2361oq`); normal catalog passes. Gate
`gate_display_closest_mode.py`; backup `before-display-closest-mode-20261001`.
The repaired preceding visual bootstrap passes
(`display-mode-bootstrap-fixed-20261001.log`). The newly typed array's subsequent
bootstrap check will follow the depth-format batch. Native GFMain is still not
linked/runnable; the aggregate security block remains recorded.

## Depth-buffer format selection promoted

The misleading CPixelFormat::FindBestMatch label at 009bebb0 is now recovered
as PDB-named CDisplayManager::GetClosestDepthBufferFormat. It is NEAR_MATCH
(249/253 bytes, 92/93 instructions, similarity 0.951351). All 1,536 linked and
1,536 comparative cases pass, covering stencil/type filtering, failed and
positive-success HRESULTs, high-depth tie preference, and callback changes to
adapter settings between compatibility calls. Retail has no safe fallback when
no format qualifies; fixtures keep at least one supported format and do not
invent behavior for its GD3DFormats[-1] access.
186 header checks have zero regressions (`display-depth-format-header-audit-o2bkdgcs`)
and the normal catalog passes. Gate `gate_display_depth_format.py`; backup
`before-display-depth-format-20261001`.

Full ResetDevice 009bfb30 first draft is accepted NEAR_MATCH (833/830 bytes,
254 instructions each, similarity 0.948819); validation remains in progress.
Its thrown CGenericException / CExceptionBase types are both one byte, verified
from PDB layouts and native ThrowInfo/CatchableType metadata at 013692f8.

## Full device reset promoted

ResetDevice 009bfb30 is promoted at NEAR_MATCH (833/830 bytes, 254 instructions,
similarity 0.948819). All 3,456 linked and 3,456 retail comparisons pass, including
cleanup ordering, strict S_OK multisampling checks, positive reset HRESULTs,
device-lost handling, exceptions, surface/setup/post-reset failures, and aliased
creation parameters mutated from callbacks. Full object and parameter state,
ordered calls, and ABI preservation are compared. The actual C++ throw is caught
by the linked fixture; both levels of generated exception metadata match the
native names, sizes, base relation, and copy/unwind descriptors.

187 shared-header checks have no regressions (`display-reset-header-audit-hucqo4pw`).
The ordinary catalog's /Od fixture initially exposed a float expectation held at
x87 extended precision; explicit float rounding corrected that fixture, and the
entire gate was rerun successfully. Production source did not change. Normal
catalog passes. Gate `gate_display_reset.py`; backup `before-display-reset-20261001`.
The last visual checkpoint passes (`display-selection-bootstrap-20261001.log`),
preceding this reset promotion. Next: CreateDevice; accepted draft is 835/846
bytes at similarity 0.901354, with 2,880 linked cases passing and native comparisons
running. No native GFMain execution or full aggregate pass is claimed.

## Device creation promoted

CreateDevice 009bf7e0 is NEAR_MATCH (835/846 bytes, 256/261 instructions,
similarity 0.901354). All 2,880 linked and 2,880 retail comparisons pass.
They cover strict multisampling support, positive/failing creation HRESULTs,
exception/surface/setup failures, parameter aliasing, capability changes during
callbacks, timer attachment, viewport coordinates, and unchanged lost-device state.
Native D3DDEVCAPS_PUREDEVICE selects hardware/pure-device flags; the separate
HWTRANSFORMANDLIGHT bit does not select that path. Both flag combinations include
FPU preservation and multithreading, corroborated against the installed SDK.
The rebuilt compiler merges the two mutually exclusive native interval call sites
into one; every executed path still calls it once, verified comparatively.

188 header checks have no regressions (`display-create-header-audit-gxspfv_k`),
and normal catalog passes. Gate `gate_display_create.py`; backup
`before-display-create-20261001`. The native link inventory and visual bootstrap
are refreshing (`native-gfmain-display-device-closure-20261001`,
`display-device-bootstrap-20261001.log`). Next: ProcessLostDevice and the emitted
73-byte parameter copy constructor; first draft is accepted NEAR_MATCH / MATCH,
pending behavioral validation. No full aggregate pass is claimed.

## Lost-device recovery and presentation updates promoted

ProcessLostDevice 009c00c0 is NEAR_MATCH (211/207 bytes, similarity 0.974359),
and its emitted CCreateDeviceParameters copy constructor 009c11a0 matches all
73 bytes. 1,936 linked and 1,936 retail comparisons pass, including native-copy
execution, early cooperative-level results, two/three reset attempts, disabling
multisampling, clamping each dimension independently, and callback mutation of
retained/local settings. Normal catalog checks pass for both. No shared headers
changed. Gate `gate_display_lost.py`; backup `before-display-lost-20261001`.

ChangePresentationInterval's descriptor overload 009bfe70 matches 160 relocated
bytes. The existing 41-byte value overload 009bff10 is consolidated into the same
source without new coverage credit. Both pass 2,400 linked cases. A change only
to PresentImmediateIfSyncMissed deliberately does not reset in retail; both
fields of the descriptor are copied when another tested field changes. The old
wrapper source/test were archived and retired, and both catalog entries pass.
Gate `gate_display_presentation.py`; backup `before-display-presentation-20261001`.

The display-device closure checks 592 objects, zero duplicates and 682 unresolved
symbols (`native-gfmain-display-device-closure-20261001`); the latest visual
bootstrap passes (`display-device-bootstrap-20261001.log`). These checkpoints
precede the lost-device/presentation promotion. Next: replace the already-exact
render-target getter's temporary view owner with the real CDisplayManager ABI.

## Render-target dimension getter retyped

The existing 009bedc0 getter now belongs to CDisplayManager instead of the
standalone render-target view. PDB signature confirms the method is non-const
and returns const C2DExtentsI. All 24 native bytes remain exact; 4,096 linked
value/unchanged-object cases pass. 193 header dependents have zero regressions
(`display-dimensions-header-audit-dshp6ktq`), including staged corrections to the
two callers' fixtures. The authored stage-2 boundary now has the same declaration.
Normal catalog passes. Gate `gate_display_dimensions.py`; backup
`before-display-dimensions-20261001`. This corrects an existing provider's ABI,
with no new function coverage. Next: remaining display construction/init frontier.


### Display window resizing (009BE490)

Recovered the PDB-named public `CDisplayManager::ResizeWindowToBackBufferSize` through the shared display/system types. Fullscreen and externally attached windows return without touching Win32. Other calls read horizontal/vertical frame metrics and caption height, convert the client origin to screen coordinates, then resize to the current back-buffer dimensions plus those margins. The native code ignores API failure returns and rereads dimensions/window handle after ClientToScreen; the reconstruction preserves that behavior.

The ordinary VC7.1 C++ compiles to a 148-byte RELOCATION_MATCH, with all three import identities and operands checked against the retail PE. A linked fixture passed 4,096 cases, covering skip conditions, signed dimensions/metrics, both API result values, callback mutations, and untouched object bytes. All 193 display-header dependents had zero regressions (`display-resize-header-audit-atfh30jl`). Candidate build passed; source, fixture, metadata and catalog promoted with backups in `before-display-resize-20261001`. Dedicated gate registered in gate_all. This adds one native function; native GFMain remains an unresolved link closure.


### Virtual-pixel initialization (009BE980)

Recovered private `CDisplayManager::InitVirtualPixels`: set the resolution-independent flag, divide 65536.0 by each signed render-target dimension, floor each stored scale, and multiply back into the virtual width/height globals. All 130 bytes match retail after 14 verified data/call relocations; both floor calls target 00BFEB8A and the constant payload is the native 65536.0. No clamp or fallback was added. Linked tests use independent integer-floor expectations for 1,250 dimension/fill pairs, including zero (infinite scale/NaN extent), negative, 65536 boundaries and signed extrema. Whole-object writes are checked.

All 194 affected header dependents had zero regressions (`display-virtual-pixels-header-audit-lh76un79`); normal catalog reports RELOCATION_MATCH/PASS and the candidate build passed. Shared externs live in `fable_virtual_pixels.h`; the per-pixel scale names are descriptive address-qualified names, not claimed PDB data names. No storage provider was invented. Promotion backups: `before-display-virtual-pixels-20261001`; dedicated gate registered. Existing coordinate helpers still have partial ABI views and one inconsistent volatile declaration; those are not silently rewritten by this batch.


### Device teardown (009BE390)

Recovered private `CDisplayManager::DestroyDevice` with shared surface and D3D interfaces: uninitialize back buffer, uninitialize depth buffer, then read and release the device if non-null. Native leaves the device pointer unchanged unless the callbacks change it; no implicit reset/null store was added. RELOCATION_MATCH, 34 bytes and two independently checked CSurface::Uninitialise call targets. The linked fixture passed 512 cases covering initial null/non-null devices, pointer changes during both surface calls and Release, call order, and all remaining object bytes. All 195 affected header dependents had zero regressions (`display-destroy-header-audit-sfrj7dzm`); normal catalog RELOCATION_MATCH/PASS and candidate build PASS. Backups: `before-display-destroy-20261001`; dedicated gate registered.


### Coordinate helper ABI cleanup (009E1D00, 009E1D50, 009E1DA0)

Retyped existing GFRoundVXToNearestPixel, GFRoundVYToNearestPixel and GFAXToVX through the shared CSystemManager/CDisplayManager/C2DExtentsI declarations and real GFGetSystemManager symbol. Consolidated the three providers and fixtures, backing up and retiring their former partial-view translation units. All three remain RELOCATION_MATCH (76/77/40 bytes), with native system getter, dimension getter, floor, ftol2, and global operands independently verified. This is ABI cleanup, not three new recovered functions.

The linked shared fixture passed 6,144 cases over positive/negative fractional coordinates, varying dimensions/scales, system-getter display-pointer mutation and dimension-getter virtual-extent mutation. GFAXToVX retains its prior volatile load through an explicit volatile lvalue access to the shared double storage, avoiding the previous conflicting global declaration while preserving native x87 operand order. No concurrency semantics for that storage are inferred. All three normal catalog entries and candidate builds pass. No shared header changed. Backups: `before-display-coordinate-abi-20261001`; dedicated gate registered.

The preceding native link checkpoint (`native-gfmain-display-recovery-closure-20261001`, before teardown and this ABI cleanup) used 596 objects, with zero duplicate providers and 685 unresolved symbols. This remains a native dependency frontier, not a runnable GFMain build.


### Busy-resource construction and cleanup (00A61880, 00A61A80)

Corrected generic linked-list labels to the PDB-backed `CBusyResourceManager` constructor and `ClearAllQueries`. CDisplayManager's native constructor calls A61880 at 009C0935 for the member at +2B0. The new shared `fable_busy_resource_manager.h` uses two one-pointer STLport lists, PendingFrame/FinishedFrame, and D3DDevice (20-byte retail class, versus 36-byte Ego_r class). CPendingQuery is the PDB's Query pointer plus Frame, 8 bytes; native sentinels are 16 bytes.

Both methods are RELOCATION_MATCH (66 and 131 bytes) with verified malloc/free targets. Constructor allocates two self-linked sentinels, sets frame counters to 1/0 and leaves D3DDevice untouched. Cleanup releases and removes pending queries from the back first, then free queries; it retains sentinels and counters. 2,048 linked cases passed, including callback-added free work and repeated cleanup. All normal catalog entries and candidate builds pass. This new header has no preexisting consumers, so no existing-header audit is required. Backups: `before-busy-resource-20261001`; dedicated gate registered. Destructor and OnPostSwapScreens are declarations only, not claimed recovered.

The bootstrap/visual checkpoint passed after resize, virtual-pixel initialization, teardown, and coordinate ABI cleanup (`display-init-helpers-bootstrap-20261001.log`). It remains the authored coordinator with recovered modules, not native GFMain execution. The earlier Defender-blocked palette fixture was not rerun or bypassed; this is not an aggregate all-gates claim.


### Busy-resource post-swap queue (00A618D0)

Recovered `CBusyResourceManager::OnPostSwapScreens`, replacing the generic deque label with its PDB identity. Ordinary VC7.1 C++ is NEAR_MATCH: 421 bytes and 172 instructions on both sides, 0.988372 opcode similarity. Accepted without further byte tuning. Five malloc/free operands independently match native calls; COM calls use the shared query ABI.

3,456 linked cases and 3,456 native/candidate Unicorn comparisons passed. Scenarios cover 0/1/8/9/10/16 pending queries; 0/1/2 free queries; success, busy, failure and alternating polls; exact-zero query creation success versus positive/negative failures; ignored Issue result; and frame counter changes in each external callback. Emulator executes actual list manipulations, comparing ordered boundary events, linked records, counters and allocation lifetime. Newly created Frame is undefined in native and only that field is masked; no initialization invented.

Native retires the oldest pending query on success or when at least nine are pending. On creation failure it writes FinishedFrame=PendingFrame-2 and returns without incrementing PendingFrame. Normal catalog NEAR_MATCH/PASS and candidate build PASS. Backups: `before-busy-post-swap-20261001`; dedicated gate registered. Shared header unchanged.


### Busy-resource destructor and corrected list providers (00A61B10, 00A61C40, 00A61D20)

Recovered CBusyResourceManager::~CBusyResourceManager (24 bytes), which calls ClearAllQueries and destroys pending then free lists. Retyped the existing 47-byte clear and 62-byte list destructor as the retail `_STL::list<CBusyResourceManager::CPendingQuery>` specialization; the former GUI-tree names/fast-call helper identities were misleading. The display destructor at 009C01C0 calls A61B10 for +2B0, corroborating ownership. Shared/identical machine bodies may serve other trivial list types; no exclusive original-template provenance is claimed.

All three are exact RELOCATION_MATCH, with each call/jump to ClearAllQueries, list destructor, or free independently validated. 1,024 linked cases and 1,536 native/candidate comparisons passed, including ClearAllQueries callback swaps of list heads, empty/nonempty lists, repeat clear, precise free order, and intact unrelated fields. Only A61B10 adds native coverage; the two list entries are corrected existing providers. No header changes. Candidate build and all three normal catalog entries pass. Backups: `before-busy-destructor-20261001`; retired two misleading source/fixture pairs, symbol index refreshed, dedicated gate registered.


### Display mode vector growth (009C2CA0, 009C2E30)

Recovered the two out-of-line STLport `_M_insert_overflow` specializations called by CDisplayManager construction: ESurfaceMultisampleType and CPixelFormat. Both instantiate the existing shared generic vector implementation and produce 227-byte RELOCATION_MATCH bodies. Constructor call positions and four-byte element types establish the specialization roles; metadata replaces generic vector labels without claiming an original source-level typedef spelling for retail CArray.

The linked fixture passed 2,160 cases for both types, covering empty and nonempty vectors, zero/one/multiple insertions, every insertion position, reserved spare capacity, end fast path, and inserted values aliased to old first/last elements. Growth capacity, preserved prefix/suffix, inserted records and final size are checked. Normal catalog and candidate builds pass; both also pass with the staged display-ownership headers. No header changes. Backups: `before-display-vector-growth-20261001`; dedicated gate registered.


### Owning display type and constructor (009C0880)

Recovered CDisplayManager::CDisplayManager(CSystemManager&) and replaced the generic NHeroInformationScreens label. The final ordinary C++ is NEAR_MATCH: 1,478 candidate / 1,475 retail bytes, 500 / 497 instructions, 0.990973 similarity. All 34 relocations and native constructor/growth call targets are checked. 3,072 linked cases and 192 full native/candidate comparisons passed. Tests vary prior object bytes, vector-growth behavior and callback mutations; they verify format order 25/31/21/23/24/22, multisampling 0/2..16, owning sentinels, returned this, and untouched fields. The comparison executes the actual recovered busy-manager constructor body.

CDisplayManager now inherits CBaseClassNonCopyable and owns its four CSurface members, CTexture, CWideString, callback list, multisample vector and CBusyResourceManager at verified native offsets. Geometry/surface types were split to remove the former header cycle; all existing accessors retain their ABI. CTexture's inline ctor/dtor are corroborated by native display construction/destruction. CCreateDeviceParameters' empty default constructor is justified by the native owner leaving the entire record untouched; it is not assigned an invented native address or counted as a separate function.

PDB evidence also exposed a class-key mismatch: CSystemManager is a class, while the shared header said struct, producing incorrect decorated parameter/return/data symbols. Corrected the shared definition/forward declarations, activation leaf metadata, singleton fixture aliases, and authored boundary declaration. Byte behavior stays unchanged. Window/coordinate validators accept the legacy spelling only for historical before/after audits; the activation provider uses the canonical class spelling. Existing reset/create/callback sources and three relevant fixture families were mechanically updated to the owning fields.

The final shared-header audit had zero regressions across 214 entries (`display-ownership-header-audit` path recorded in the scratch stage); the two subsequently added vector-growth entries also pass under both old and new headers, for 216 checked entries. The previously Defender-blocked 009A5490 fixture remains explicitly excluded and untouched. Separate activation checks passed 512 linked + 512 comparative cases, coordinate checks passed 6,144, and the standalone system initializer passed. The staged full bootstrap/visual checkpoint passed (`display-ownership-bootstrap-fixtures-20261001.log`). Its mock display constructor uses explicit test-only lifetime support; those boundary stubs are not native coverage. A texture-copy fixture also supplies its own mock destruction dependency.

Promotion backups: `before-display-ownership-20261001` (30 files). Normal catalog confirms NEAR_MATCH/PASS for the constructor and RELOCATION_MATCH/PASS for activation and the global system getter; constructor candidate build passed. Native display Init and destructor still remain unrecovered, and native GFMain is not runnable. Symbol-index and native-link refresh follow this promotion.

The ownership symbol refresh recompiled 226 affected catalog/bootstrap entries (25,401 indexed definitions). The native GFMain closure then reached a fixed point in five rounds: 623 objects, zero duplicate providers, 689 unresolved symbols (`native-gfmain-display-ownership-closure-20261001`; `display-ownership-closure-final-20261001.log`). Newly linked bodies expose further real dependencies; this is not a runnable-native-startup claim.


### Display initialization and lifetime (009C0E50, 009C01C0, 009C2220)

Promoted CDisplayManager::Init (500 bytes, 160 instructions, NEAR_MATCH 0.98125) and its destructor (268 bytes, 82 instructions, NEAR_MATCH 0.926829). Initialization reproduces capability fallback, signed minimum dimensions, the original exclusive argument, all creation parameters, failure exceptions, timer/render-owner replacement, optional shader initialization and virtual pixels. Destruction preserves callback-visible order, member rereads, non-cleared device storage, and a second renderer deletion if cleanup repopulates the scoped pointer. No byte tuning continued after accepted similarity.

The callback-list destructor at 009C2220 is a retyped existing 62-byte RELOCATION_MATCH, replacing the misleading GUI-tree source/fixture. It frees pointer-list nodes and sentinel without deleting pointed-to callbacks; shared identical bodies do not establish exclusive original-template provenance. Only initialization and the owning display destructor add functions in this batch.

Shared ABI now models scoped render-manager/timer pointers, CRenderStateManager's virtual base hierarchy, CRenderManager2D/CRenderManager's native 0x3EA4 size, named CanBeginRender/RenderManagerInitFlags, and the PDB class spelling of CShaderRenderManager. The empty creation-parameter constructor moved inline without claiming an invented native function. Raw-storage fixtures construct scoped pointers explicitly. Existing native call sites use get() where they need raw pointers.

The dedicated gate passed 2,304 initialization and 6,144 destruction linked cases, plus 4,608/1,536 native comparisons. Relocation operands and generic-exception metadata are independently checked. The 211-entry header audit has zero regressions; security-blocked 009A5490 remains excluded and untouched. Normal catalog checks and the four-address candidate build pass. The staged full bootstrap and visual checkpoint pass (display-lifetime-bootstrap-fixtures-20261001.log); this remains the authored coordinator, not runnable native GFMain.

Bootstrap exposed two preexisting boundary issues: placement new now uses a weak named fallback to coexist with standard inline definitions, and the old shader-reset assembly oracle now includes the shared class header instead of a local struct. Its entire 131-byte body and relocation set are unchanged; no new ordinary-C++ recovery or behavioral-test credit is claimed for that legacy oracle. Promotion backups: before-display-lifetime-20261001, 32 files. Dedicated gate: gate_display_lifetime.py. Symbol refresh recompiled 221 affected entries, indexing 25,519 definitions; native-link refresh follows.

### Render-state destructor (00A044B0)

Ordinary CRenderStateManager destruction is RELOCATION_MATCH, 21 bytes. It unconditionally clears the current-render-state global and its device pointer before the native base destructor. PDB virtual inheritance/signature and native stores/tail jump corroborate the implementation. The 1,536 linked cases vary prior object bytes, null/self/other current registration and base callback mutations, checking call order and untouched fields. No additional header changes. Candidate build passes; dedicated gate gate_render_state_destructor.py. Backup: before-render-state-destructor-20261001.

Native link refresh after the display-lifetime batch reached a fixed point in one round: 626 objects, zero duplicate providers, 693 unresolved symbols (native-gfmain-display-lifetime-closure-20261001). The added real bodies expose further native dependencies; no native startup execution is claimed.


### Renderer constructors and core destruction (00A05970, 00A079B0, 00A0AFE0, 00A0B060)

Promoted four ordinary C++ functions in CRenderManager_Constructors_00a05970.cpp. State construction (108 bytes), core construction (116), and core destruction (50) are RELOCATION_MATCH; CDeviceState construction is exact MATCH, 4,950 bytes. The latter's prior CPSCOrbit label was incorrect: its 231 consecutive 20-byte render-state records and the state-owner call/PDB nested type corroborate its role. The large initializer comes entirely from ordinary member/array construction, with no baked instructions or constants fitted to byte output. Once accepted, no byte tuning was performed; relocation validation established the final exact grades.

The shared CStateInfo constructor initializes only CurrentState, DesiredState, BookmarkMask, DirtyListFlag and StateType. StateParam, TextureStage and padding remain untouched. The owner initializes its bookkeeping and bookmark dummy state while retaining device/storage bytes that native leaves unchanged. Core now owns its actual CPixelFormat and CSystemFont, stores the display reference, and clears eight selected-texture pointers. Destruction checks font validity, conditionally uninitializes it, destroys its base and then the state base. CRenderStateManager's constructor is explicitly noinline to retain the independently recovered native call boundary within the consolidated source.

The fixture passed 10,752 linked cases, including all prior fill-byte values, high-bit initialization flags, callback mutations, all three construction entry points and core destruction. 1,536 native comparisons execute the linked constructor chain and previously validated state destructor. Every relocation is checked against the corresponding native operand. Normal catalog and candidate builds pass. Shared-header audit: 216 entries, zero regressions, with the existing Defender-blocked color fixture excluded. Staged full bootstrap/visual checkpoint passed (render-constructors-bootstrap-fixtures-20261001.log). The old core Init/Uninit provider and fixture use typed font/pixel members without body changes; display lifetime fixtures now mock the explicit core constructor chain. Backups: before-render-constructors-20261001, 12 files. Dedicated gate: gate_render_constructors.py. Next: renderer 2D owned resources and their destruction; native GFMain still unresolved.

Renderer-constructor symbol refresh recompiled 225 affected entries, producing 25,579 indexed definitions. Native GFMain closure is stable at 627 objects, zero duplicate providers, 693 unresolved symbols (native-gfmain-render-constructors-closure-20261001).

### Vertex shader lifetime (00A62FB0, 009BE210, 00A62FC0, 00A62FF0, 009C1AE0, 00A630D0)

Promoted the shared CShaderBase/CVertexShader/CVertexShaderData hierarchy and six verified bodies: handle ctor 16 bytes, reference-count query 7, data ctor 48, data dtor 43, handle deleting dtor 55, data deleting dtor 64. The query is MATCH; the other five are RELOCATION_MATCH. The query replaces an old CVertexShader_Methods::M provider and print-only fixture, without claiming new coverage for it. The remaining five are new entries. Deleting destructors are emitted by VC7.1 from normal C++ ownership, never hand-authored ABI machine code.

Data construction retains the declaration, shader and borrowed declaration-format pointer with reference count one; the two name arguments are unused in retail. Data destruction releases the shader first, rereads the declaration after its callback, releases that declaration if non-null, and destroys the base without freeing FormatDecl. Handle destruction caches its data pointer, decrements the unsigned count and deletes only on the transition to zero. Native zero-count underflow is preserved. The compiler's handle destructor is forced inline to preserve the native deleting-destructor call boundary; no tuning continues after accepted bodies.

The new fable_d3d9_shader.h adds shader/declaration COM declarations only for the lightweight fallback, preserving SDK definitions. The shared shader header replaces the landscape accessor header's two-word placeholder. Generated engine headers are untouched; the former reference-count source/fixture is backed up and retired.

Validation: 5,632 linked cases and 3,300 native comparisons, including null resources, count zero/one/multiple/max, scalar deletion flags, release ordering and callback replacement/removal of the declaration. All three vtables (seven slots) and VC7.1 vector-to-scalar weak fallbacks are independently checked against native targets. Six existing landscape-header dependents have zero regressions. All six normal catalog entries and candidate builds pass. Backups: before-vertex-shader-lifetime-20261001, 12 files. Dedicated gate: gate_vertex_shader_lifetime.py; its vtable check was additionally run as vtable-proof.json. This batch does not change the existing authored bootstrap paths or claim native startup execution.


### Shader debug name and shader-bank state helpers

Promoted CShaderBase::GetDebugName (00987CF0), 25-byte RELOCATION_MATCH. It constructs the native "(Unknown)" string with the default length argument. Its dedicated gate verifies literal bytes, relocation targets, 256 linked cases and candidate-required linking. Normal catalog and candidate build pass. Backups: before-shader-debug-name-20261001 (7 files).

Retyped and consolidated the existing CShaderBankStateBlock::Set (00A61F40, 19-byte MATCH) and CreateCRC (00A62580, 12-byte RELOCATION_MATCH) providers using a shared 12-byte polymorphic owner with its 8-byte state. Set copies optimization before target, preserving padding; CRC delegates all eight state bytes, including padding, to the native CRC helper. 4,096 linked cases cover aliasing, high-bit targets, padding and callback mutation. Normal catalog and candidate builds pass. The CBankStateBlock class-key correction matches PDB decoration; 36 existing header dependents showed zero regressions, and staged full bootstrap/visual checkpoint passed (shader-bank-state-bootstrap-fixtures-20261001.log). Symbol refresh rebuilt 38 affected entries. Backups: before-shader-bank-state-20261001 (13 files). Dedicated gate: gate_shader_bank_state.py. This is corrected existing coverage, not two new recovered addresses.

The 00A61F20 default initializer remains scratch-only: equivalent ordinary C++ has not met the required byte grade. No assembly or volatile tuning was introduced. The earlier Defender-blocked color fixture remains excluded; native GFMain is still unresolved.


### Quick-draw vector destruction (009C2F50)

Promoted the 58-byte RELOCATION_MATCH STLport vector<CQuickDrawTriInfo> destructor, emitted by ordinary explicit template instantiation. New shared CQuickDrawTriInfo matches the 60-byte PDB layout, with owned CWideString at +0x14 and CTexture at +0x18. The native body caches the element range, destroys each texture before its string, and rereads the allocation pointer before free; vector fields remain untouched except for callback mutations. Validation passed 3,841 linked cases and 75 native comparisons, including null/allocated empty storage, replacement/null allocation pointers, changing finish during callbacks, preserved element bytes and callee-saved registers. Dedicated gate: gate_quick_draw_lifetime.py; normal catalog and candidate build pass. New header has no prior consumers. Backups: before-quick-draw-lifetime-20261001 (8 files). Continuing shader-bank/2D renderer ownership; native startup remains unresolved.

Shader-ownership native link refresh reached a fixed point after two rounds: 635 sources, zero duplicate providers, 689 unresolved symbols (native-gfmain-shader-ownership-closure-20261001). Shader-bank constructor/destructor compile to native-shaped 48/51-byte bodies in scratch, but remain unpromoted pending complete shared bank virtual declarations and dependent validation.


### Shader-bank lifetime staged; generic fixture execution blocked

CShaderDataBank constructor 00A62020 (48 bytes) and destructor 00A620D0 (51) pass exact relocation checks, 1,536 dedicated compiled cases and 60 native comparisons in scratch. The staged base-bank contract has all 20 virtual slots, including the pure format-version method; declarations for shader/font/streaming-font/text/particle derived overrides match seven native vtables (140 slots). The final 113-entry header audit has zero regressions, with 25 fixture overrides, two concrete constructor-test subclasses and shared unused-virtual boundaries. The staged full bootstrap and authored visual checkpoint pass.

Promotion is deferred: the normal generic catalog runner raised WinError 225 while launching 00A62020, and Defender recorded ThreatID 2147651817 at 2026-10-01 13:56:07, ActionSuccess=True, resource C:/Users/Cornelio/AppData/Local/Temp/shader-bank-catalog-pojl714c/00a62020_35j32y9o/00a62020.exe. No retry, restoration, security change or alternative build to evade detection was attempted. 00A620D0 generic validation was not reached. The two addresses and bank ABI changes are NOT promoted or registered in gate_all. Experimental gate_shader_bank_lifetime.py, gate_bank_vtables.py, bank_vtable_contract.py and shader_bank_lifetime_oracle.py currently require the staged include tree. Evidence: work/codex-native-startup/shader-bank-lifetime; shader-bank-lifetime-header-audit-final.log; shader-bank-lifetime-bootstrap-fixtures-20261001.log. Continue independent shader-resource reconstruction while preserving this batch for later security review.


### Shader-resource construction and closing (00A5D870, 00A5D750, 00A5D7A0)

Promoted CShaderResource_ConstructionAndClose_00a5d870.cpp with two new entries and one corrected existing provider. Construction is 51-byte RELOCATION_MATCH, CloseShaderBank is 69-byte MATCH, and Uninitialise is 17-byte RELOCATION_MATCH. The last replaces the mislabeled CMemoryDataOutputStream_Clear provider and its self-contained fixture. The shared 60-byte CShaderResource now names the borrowed bank, owned shader-pointer vector, manager, 28-byte vertex-input owner, declaration pointer and vertex type. CShaderBase grants the owner access to its protected destructor; all 13 affected header dependents pass.

CloseShaderBank deletes non-null handles, rereads the array before clearing the slot, and rereads the size after every iteration. It preserves vector capacity/storage and clears the bank pointer. Uninitialise then clears the manager pointer. Construction initializes the native fields around the external vertex-input constructor in native order. Verification: 896 linked cases, 650 native comparisons, native vtable slot, all three generic catalog checks and all three candidate builds pass. Callback cases replace, shrink or grow the array, change the bank/manager and preserve unrelated bytes. No bank-lifetime changes from the Defender-blocked staged batch are included. Backups: before-shader-resource-lifetime-20261001 (12 files). Dedicated gate: gate_shader_resource_lifetime.py. Continuing independent vertex-input ownership.


### Vertex-input constructor and stream layout (00AB3B10)

Promoted the 38-byte RELOCATION_MATCH CVertexShaderInput constructor. Its original AI-state label was incorrect; the shader-resource call, two native STLport arrays and vtable identify the input owner. The shared header now completes the 20-byte CStreamInfo and 16-byte CVertexStreamFormat (8-byte DataElement); the donor's newer 16-byte vectors explain its larger sizes. EVertexStreamDataType names/values come from the PDB enum. Constructor validation passed 512 compiled cases with sentinels and base-callback writes; native vtable slot, generic catalog and candidate build pass. Three header dependents have zero regressions. Backups: before-vertex-input-lifetime-20261001 (9 files); gate_vertex_input_constructor.py. Stream-format inline destruction expresses member/base ownership but no additional address coverage is claimed. Continuing vertex-input destruction.


### Compiler-generated vertex-input destruction (00A5EB70, 00A5E900, 00AB4690)

Promoted implicit member/base destruction by removing user-declared empty destructors from CVertexShaderInput and CVertexStreamFormat. VC7.1 now emits the exact native bodies from the existing CVertexShaderInput constructor provider: owner destructor 62 bytes, stream-range helper 55, scalar deleting destructor 30 (all RELOCATION_MATCH). Explicit empty destructor bodies had introduced vtable stores absent in retail; implicit C++ destruction reproduces the native ownership naturally, without assembly. The first two addresses are new; the scalar thunk replaces the mislabeled CLandscapeBackgroundPatch wrapper and print-only fixture. Constructor bytes remain unchanged.

Validation: 2,400 compiled cases and 672 native comparisons test declaration/element/vector/base cleanup order, null element allocations, cached stream ranges, live allocation-pointer rereads, callback replacement/shrinking, and scalar flags 0/1/2/3/max. The fixture object must not define any validated target, ensuring it calls the candidate bodies. All three generic catalog checks, all four candidate builds including the constructor, and four header dependents pass. Shader-resource fixtures gained only a never-called base-destructor boundary required by newly emitted inline ownership. Backups: before-vertex-input-destructor-20261001 (12 files). Dedicated gate: gate_vertex_input_destructor.py. Continuing shader-resource destruction; the earlier shader-bank security block remains untouched.


### Shader-resource destruction (00A5D800, 00A5EBE0)

Promoted the 108-byte resource destructor and compiler-generated 30-byte scalar deleting destructor, both RELOCATION_MATCH, by extending the existing typed resource provider with `~CShaderResource() { Uninitialise(); }`. VC7.1 inlines the small uninitialization and implicit vertex-input destruction exactly as retail. The destructor address is new; the scalar thunk replaces an unrelated landscape label and synthetic wrapper. The previously recovered constructor, close and uninitialise bodies remain unchanged.

Validation: 4,096 compiled cases and 1,152 full-chain native comparisons execute real CloseShaderBank and stream-range cleanup, with callback replacement of shader arrays at shader, stream-base and input-base boundaries. The sequence releases shader handles, declaration storage, stream element allocations, stream storage, input base, the current shader allocation, and owner base; scalar flags conditionally delete the owner. All five generic catalog/candidate checks pass. No shared headers changed. The older fixture gained an unused base-destructor fallback for the newly linked owner body. Backups: before-shader-resource-destructor-20261001 (10 files); gate_shader_resource_destructor.py. Native link refresh follows. Shader-bank lifetime remains unpromoted after its Defender block.


### Shader-resource bindings and counted assignment (00A5D4D0, 00A5D4E0, 00A5D4F0, 00A5D500, 009DE7C0)

Promoted three 10-byte setters and the 4-byte bank accessor as exact MATCH, plus the typed 72-byte CCountedPointer<CShaderResource> assignment as RELOCATION_MATCH. The setters replace synthetic observer labels; the assignment replaces a generic C ABI wrapper under a disk-file label. The native renderer call and PDB signatures identify the shader-resource specialization. PeekShaderBank is newly recovered; its independent four-byte native entry was missing from the function manifest, so promotion appended a corroborated manifest row after a metadata-only checkpoint rather than rerunning completed file writes. The eight superseded source/test files are backed up and retired.

Validation passed 1,324 compiled cases, 300 assignment comparisons, all five generic catalog/candidate checks and five header dependents. Tests cover self/same-control-block assignment, null blocks, zero/max refcounts, source mutation during release, destination control-block replacement/removal before delete, returned references, pointer/enum setters and preservation of unrelated bytes. Backups: before-shader-resource-bindings-20261001 (16 files); initial land script stopped on the missing manifest row, and resume_shader_resource_bindings_metadata.py completed only the remaining metadata/retirement/gate registration. Dedicated gate: gate_shader_resource_bindings.py.

The preceding shader-resource link refresh reached a fixed point at 637 sources, zero duplicate providers and 689 unresolved symbols (native-gfmain-shader-resource-closure-20261001). This checkpoint predates the binding batch. Shader-bank lifetime remains scratch-only after its Defender block.


### Shader-render-manager resource factory (0098B860, 0098BA10)

Promoted ordinary C++ CreateResource and the typed counted-pointer DeleteData. The factory is accepted NEAR_MATCH: 211 built bytes / 205 native bytes, 80 / 78 instructions, similarity 0.9746835443. The implementation was frozen on its first accepted grade; the final score accounts for an independently proven absolute deleter pointer in the relocation-stripped retail image. DeleteData is an exact 11-byte MATCH and replaces the generic delete wrapper. The manager remains a partial view; only its factory declaration and the native Resources vector at +0x3A8 are used. No blocked shader-bank ABI changes are included.

Validation passed 289 linked cases and 144 native comparisons, both generic catalog checks/candidate builds and all 11 header dependents. Tests execute actual vector growth, including resource-allocation failure's native access violation, control-block allocation failure, spare/full capacity and refcount callback mutations. Four naturally emitted vector helpers (218/50/46/76 bytes) match exactly and execute in the chain, but are not separately claimed as new address coverage. Dedicated gate: gate_shader_resource_create.py. Backups: before-shader-resource-create-20261001 (10 files); promotion completed. Native factory link refresh follows.

Native factory closure refresh complete: 639 sources, zero duplicate providers, 689 unresolved symbols; fixed point in one round. Evidence: work/codex-native-startup/native-gfmain-shader-factory-closure-20261001.


### Typed resource-vector helpers (0098DC30, 0098BFF0, 0098C030, 0098CEE0)

Promoted the factory-emitted vector overflow (218-byte RELOCATION_MATCH), uninitialized copy (50-byte MATCH), fill (46-byte MATCH) and destruction (76-byte RELOCATION_MATCH). No implementation or shared-header changes were needed. The existing copy provider's unrelated pair/active-script label and local two-pointer stand-in are retired; all four catalog entries use the typed manager factory provider and a dedicated helper fixture.

Validation: 899 linked cases, 638 direct native comparisons and 144 factory comparisons; all four normal catalog checks and candidate builds pass. Tests cover insertion at every position, zero/one/multiple fills, aliased input, distinct/shared/null control blocks, unsigned refcount wrap, empty ranges, null placement destinations, and cleanup callbacks replacing or clearing the live Info pointer before delete. The gate asserts the fixture defines none of the four validated helper symbols. An initial fixture emitted template copies; explicit link aliases and raw vector storage corrected the fixture before acceptance. The null-callback cleanup test also distinguishes delete(nullptr) from freeing a real control block.

Dedicated gate: gate_shader_resource_vector.py; oracle: shader_resource_vector_oracle.py. Backups: before-shader-resource-vector-20261001 (9 files), promotion completed. Symbol index refreshed (six entries for the factory source). The latest native closure remains the preceding 639-source / zero-duplicate / 689-unresolved checkpoint. Continuing SetVertexShaderInput.


### Shader-resource vertex-input binding (00A5D8B0)

Promoted SetVertexShaderInput as 66-byte accepted NEAR_MATCH (27/27 instructions, similarity 0.962962963). Ordinary implicit assignment copies streams before declarations; the native empty-declaration fallback points to the declaration-array object itself. Source tuning stopped when the early-return C++ form met the grade. The compiler-emitted vector assignments remain explicit external boundaries, not newly claimed implementations.

Validation passed 288 linked cases, 288 native comparisons, the generic catalog and candidate build, and all 16 header dependents. Cases cover separate/self input, assignment order, source/declaration mutation during the first assignment, empty/allocated/null declaration storage, final pointer selection and preservation of unrelated fields. Shared header adds only the method declaration. Dedicated gate: gate_shader_resource_input.py; backups before-shader-resource-input-20261001 (9 files); symbol index refreshed. Continuing typed stream/declaration vector assignment recovery; native GFMain and the shader-bank security block remain unresolved.


### Stream-vector assignment (00A5E9C0)

Promoted the 347-byte RELOCATION_MATCH vector<CVertexShaderInput::CStreamInfo> assignment using an ordinary explicit instantiation of the shared STLport assignment implementation. Its prior skeletal-morph label was incorrect. The source declares the nested DataElement assignment as a separate boundary, preventing incidental emission of that still-unaccepted implementation; the accepted stream body is unchanged. No shared headers changed.

Validation: 540 linked cases and 540 full-chain native comparisons cover self-assignment, allocation, reuse/shrink, growth within capacity, nested deep-copy content, preservation of reused vtable pointers, new-element vtables and cleanup counts. The 61-byte stream construction loop, 75-byte overwrite loop and previously recovered 55-byte destruction loop match exactly and execute in the comparison; only the main assignment is newly claimed. Nested element-array constructor/assignment remain tested external boundaries. Native format vtable 0129D578 / deleting slot 00A5E670 is checked; the fixture cannot define validated targets. Generic catalog and candidate build pass. Dedicated gate gate_shader_input_vector_assign.py; backup before-shader-input-vector-assign-20261001 (8 files); symbols refreshed.

Declaration-vector assignment 00A5E2E0 remains scratch under shader-input-vector-assign: current accepted-shape candidate is 186/198 bytes, similarity 0.9277108434, pending behavior proof. Its private counted-copy helper has a VC7.1 optimized internal calling convention; complete caller/helper pairs must be compared. No catalog promotion yet.


### Declaration-vector assignment (00A5E2E0)

Promoted vector<D3DVERTEXELEMENT9> assignment as accepted NEAR_MATCH: 186 built / 198 retail bytes, 80 / 86 instructions, similarity 0.9277108434. Shared STLport assignment is instantiated with an inline wrapper around the native counted element-copy algorithm. VC7.1 specializes the private static helper to pass its destination in EAX and remove unused stack arguments. The native and compiled complete caller/helper pairs are compared independently; this helper is not separately claimed as a recovered public ABI or address. No changes followed acceptance.

Validation passed 1,200 linked cases and 1,200 full-chain native comparisons, covering self-assignment, empty/allocated storage, reallocations, growth within capacity, shrink/equal sizes, exact element bytes, preserved trailing bytes and callee-saved registers. The 44-byte construction and 78-byte allocation helpers match retail exactly; they execute in the proof but have no new catalog claims. Generic catalog and candidate build pass. No shared headers changed. Dedicated gate gate_vertex_declaration_assign.py; backup before-vertex-declaration-assign-20261001 (8 files); symbol index refreshed. Continuing nested CVertexStreamFormat::CDataElement vector copy/assignment to close the input-copy chain.


### Nested stream-element vectors (00A5E0A0, 00A5E4D0) and input-copy integration

Promoted vector<CVertexStreamFormat::CDataElement> copy constructor (118-byte RELOCATION_MATCH) and assignment (186/198-byte accepted NEAR_MATCH, similarity 0.9277108434). Shared STLport bodies and the native counted-copy algorithm reproduce behavior; the private copy helper has the same compiler-specialized internal ABI as the declaration-vector batch and is not a separate address claim. Source tuning stopped on first accepted grades. No shared headers changed.

Validation passed 1,254 linked and 1,254 complete-chain native comparisons, including copied data independence, source capacity exceeding size, empty allocated source storage, assignment storage reuse/growth/shrink, sentinels, and callee-saved registers. Both generic catalog and candidate builds pass; symbols refreshed. Dedicated gate gate_vertex_stream_elements.py; backup before-vertex-stream-elements-20261001 (8 files).

New integration gate gate_vertex_input_copy_chain.py links the resource binder, stream assignment, declaration assignment and nested element-vector provider together: 2,160 compiled cases pass with real nested copy/assignment bodies. The fixture defines none of the five main targets; four negative links each omit a required provider and all fail as expected. Remaining execution boundaries are CRT allocation/free and the root-class destructor. This integration adds no address coverage; individual native comparison proofs remain in their dedicated gates. Fixture rebuild/tests/integration/VertexInputCopyChain_test.cpp; backup before-vertex-input-copy-chain-20261001 (3 files). Native link refresh follows.

Vertex-input copy closure: fixed point in one round, 643 sources, zero duplicates, 689 unresolved symbols (native-gfmain-vertex-input-copy-closure-20261001). New evidence identifies 009CC350 as CBankFile::GetMaxEntryIndex (backward validity scan), not GetSize; PDB symbol and native code agree. Continuing that independent helper using the existing bank layout; blocked shader-bank lifetime/ABI batch remains untouched.


### Bank maximum valid entry (009CC350)

Promoted CBankFile::GetMaxEntryIndex as 77/82-byte accepted NEAR_MATCH, similarity 0.9210526316, 36/40 instructions. The first compiled ordinary C++ scan met acceptance and was frozen. Native/PDB evidence corrects the earlier informal GetSize identification: this helper scans backward, excludes reserved index zero, and chooses packed or runtime validity. Signed initial-index edge behavior is preserved, including negative returns other than -1 when Size exceeds 0x80000000.

Validation passed 1,741 linked and 1,741 native comparisons, generic catalog/candidate checks, and all 113 affected header dependents. Cases cover packed/unpacked validity, nonzero validity bytes, live Size/mode/runtime-pointer changes at packed lookup callbacks, empty/reserved entries and high unsigned Size values. Header change adds only the nonvirtual method declaration; no blocked shader-bank ABI or lifetime code is included. Dedicated gate gate_bank_max_entry.py; backup before-bank-max-entry-20261001 (9 files); symbols refreshed. Continuing shader-pointer resize used by bank setup.


### Shader-pointer resize and fill-insert (00A5E6A0, 00A5E3B0)

Promoted typed vector<CShaderBase*> resize (87-byte RELOCATION_MATCH), then its fill-insert provider (289/276-byte accepted NEAR_MATCH, similarity 0.9155555556). Resize uses the native separately compiled fill-insert boundary; shrinking updates finish, while equal/growing size delegates insertion, including zero count. Fill-insert specializes only the POD backward-copy wrapper to match native inlining and otherwise uses shared STLport bodies. No changes followed accepted grades. No shared headers changed.

Resize validation passed 2,064 linked and 2,064 native boundary comparisons, including aliased value pointers and callback changes. Fill-insert passed 2,736 linked and 2,736 complete-chain native comparisons across every insertion position, zero/one/multiple values, allocated-empty storage, spare capacity, overlapping copies, growth, null/max pointer values and aliased input. The 216-byte growth helper is exact and executes in the proof; its address is not newly claimed. The shared fill helper uses cdecl while retail uses fastcall, so only complete matching caller/helper chains are compared; no independent fill-helper ABI/address claim. Generic catalog and candidate checks pass for both; symbols refreshed.

Gates: gate_shader_pointer_resize.py and gate_shader_pointer_fill.py. Backups: before-shader-pointer-resize-20261001 and before-shader-pointer-fill-20261001 (8 files each). Continuing OpenShaderBank with the existing CBankFile base and forward-declared shader bank, without promoting the blocked shader-bank lifetime/virtual-layout batch.


### Shader-resource bank setup (00A5D7C0) and complete setup integration

Promoted OpenShaderBank(const CShaderDataBank*) as 60-byte RELOCATION_MATCH. It stores the borrowed bank, obtains GetMaxEntryIndex()+1, resizes the shader-pointer vector with null fill and clears the cached count of slots. PDB inheritance and native calls prove the CBankFile base at offset zero; the source accesses that existing base through the forward-declared derived pointer. No shader-bank lifetime or virtual-layout code from the security-blocked batch is included. Only the resource's method declaration is added to shared headers.

Validation passed 180 linked/native boundary cases, generic catalog/candidate and all 17 header dependents. Boundary tests verify bank storage before lookup, callback replacement of the stored bank, live vector-start rereads, clearing the cached count despite a changed finish and zero-length handling. A separate 1,584-case integration links five real recovered providers: OpenShaderBank, GetMaxEntryIndex, pointer resize, fill-insert and CPackedUIntArray::operator[]. It covers packed/unpacked validity and real allocation/shrink/grow/clear behavior; removing any one provider fails the link. Only CRT allocation/free/memmove remain execution boundaries. No bank constructor/destructor is executed.

Gates gate_shader_resource_open_bank.py and gate_shader_resource_open_chain.py; fixture rebuild/tests/integration/ShaderResourceOpenChain_test.cpp. Backup before-shader-resource-open-bank-20261001 (11 files); promotion completed, symbols refreshed. Native link refresh follows.


### Vertex-shader COM construction (00A63020)

Promoted the private bytecode/declaration constructor by extending the existing CVertexShader_Lifetime provider. Accepted NEAR_MATCH: 168 built / 167 native bytes, 61/62 instructions, similarity 0.9593495935 after independently proving the stripped vtable-pointer relocations (initial accepted score 0.9268292683; implementation unchanged thereafter). The retail CArray<unsigned long> argument uses the existing 12-byte CArrayVector shared representation.

New fable_d3d9_shader_create.h provides a sparse typed view of device slots 86/91, with offset assertions. Local Windows SDK 10.0.22621.0 d3d9.h independently confirms signatures/order; native A63020 confirms offsets 0x158/0x16C. No existing device virtual interface was extended. Creation calls vertex shader before declaration, caches the device but reloads its vtable, ignores HRESULTs, then allocates the 20-byte counted owner. Native allocation failure leaves both created interfaces unreleased; preserved and tested.

Validation passed 1,728 linked creation/cleanup cases and 1,728 native creation comparisons. Cases include empty allocated/unallocated bytecode arrays, null/non-null declarations and returned interfaces, failed HRESULTs with written outputs, allocation failure, display-device replacement and vtable replacement during callbacks. Existing lifetime proof remains exact: 5,632 linked / 3,300 native comparisons and seven vtable slots. All 33 affected header dependents, new generic catalog and all seven candidate builds pass. Older lifetime fixture gained an abort-on-use weak system-manager boundary for the uncalled new constructor.

Gate gate_vertex_shader_create.py; backup before-vertex-shader-create-20261001 (11 files); promotion completed and seven source entries indexed. The preceding native link refresh reached 647 sources, zero duplicate providers and 689 unresolved symbols (native-gfmain-shader-bank-open-closure-20261001); this predates the shader-creation extension. Shader-bank lifetime/virtual-layout changes remain scratch-only after their security block.

## Vertex declaration generation promoted

- Recovered `CVertexShaderInput::GenerateD3DDeclaration` at `00AB3870` as ordinary VC7.1 C++. Native code has 612 executable bytes plus a 56-byte jump table; candidate has 622 executable bytes, two alignment bytes, and 38 bytes of switch tables. Code-only NEAR_MATCH is 0.9346246973 (207 versus 206 instructions). Implementation frozen at first accepted near match.
- PDB stream/input types and native disassembly/decompilation establish ordinal stream indices, offsets accumulated across streams, cached element types, declaration type mapping, unsupported-format clearing, and one extra logical element per stream. Empty input still writes the terminator; null storage faults as retail does. No invented guard was added.
- `gate_vertex_declaration_generation.py` validates all table pointers and all fourteen type-size dispatch results separately from executable instructions. The compiled fixture passes 292 cases against retail-derived state hashes; Unicorn compares 293 native/candidate cases including the null-storage fault. Allocation/fill insertion remains an explicit boundary at `00AB4260`.
- Shared-header audit: 26 entries, zero regressions. Dedicated validation is registered in `gate_header_dependents.py`; generic catalog grading returns NEAR_MATCH/PASS, candidate build passes, symbol index refreshed. The fixture cannot link without the recovered provider.
- Reproducible evidence: `work/codex-native-startup/vertex-declaration-generation/{proof.json,scratch-validation.json,audit-path.txt}`; rollback manifest `before-vertex-declaration-generation-20261001/manifest.json` (12 files). No shader-bank blocked lifetime/ABI artifact was rerun.
- Next: declaration-vector fill insertion (`00AB4260`) and composed declaration-generation tests.

## Declaration insertion and generation chain promoted

- `00AB4260` declaration-vector fill insertion now matches all 280 bytes after relocation. The recovered non-POD STLport path constructs/copies class elements and preserves insertion-value aliasing; the shared POD implementation remains unchanged.
- Complete native/candidate insertion chains pass 2,736 comparisons and the compiled fixture passes the same 2,736 cases. Four helper bodies also match (44-byte construction copy, 37-byte fill, 40-byte construction fill, 259-byte overflow); no separate helper coverage claims were added. The backward-copy helper differs (49 versus 43 bytes), with matching ABI and behavior verified through the whole chain.
- `gate_vertex_declaration_generation_chain.py` links generation plus real fill/growth and passes 292 cases with CRT allocation/free as the only boundaries. Omitting either provider fails the link; the fixture defines neither target.
- Catalog grading is RELOCATION_MATCH/PASS, candidate build passes, and symbol index is refreshed. One-shot archive `before-vertex-declaration-fill-20261001/manifest.json` covers ten changed files. Proofs: `vertex-declaration-fill/{proof.json,chain-proof.json,catalog-proof.txt}`.
- Native link refresh running in `native-gfmain-declaration-generation-closure-20261001`; continuing stream-format/input construction evidence.

## Vertex stream format construction and sizing promoted

- Four PDB-confirmed methods recovered in `CVertexStreamFormat_Building_00ab37f0.cpp`: constructor `00AB37F0` (29 bytes), AddDataElement `00AB3810` (83 bytes), CalculateVertexSize `00AB37B0` (58 bytes), all relocation matches; GetDataTypeSize `00AB3740` is NEAR_MATCH 0.9714285714 (18 versus 17 instructions), with all fourteen dispatch targets validated separately from table bytes.
- The native non-POD append overflow at `00AB4150` also matches 259 bytes and is executed in the comparison chain; no separate coverage claim was added. The size helper remains out of line as in retail, allowing the caller to preserve the same register behavior. All accepted bodies were frozen when accepted.
- `gate_vertex_stream_building.py`: 2,062 compiled and 2,062 native comparisons, including empty vectors, every byte-valued type, signed/large invalid enums, spare capacity, allocation/growth, register indices and post-append sizing. The fixture requires the recovered provider. Shared-header audit: 28 entries, zero regressions. Three ordinary generic entries and the table-aware entry pass; all four candidate builds pass.
- Backup: `before-vertex-stream-building-20261001/manifest.json` (ten files). Proofs in `vertex-stream-building/`. Prior native closure completed at 649 sources, zero duplicates, 689 unresolved (`native-gfmain-declaration-generation-closure-20261001`); this precedes stream-format promotion.
- Next: `CVertexShaderInput::AddStream` (`00AB3B40`), traced through native call sites and PDB AddStream(unsigned long,const CVertexStreamFormat&,bool). Scratch implementation ready for validation.

## AddStream promoted

- `CVertexShaderInput::AddStream` (`00AB3B40`) recovered with the PDB constructor for its `CStreamInfo` temporary. The 160-byte body differs only in one stack displacement used for the unused STLport trait-tag temporary; executable instruction similarity is 1.0 after proved relocation normalization (54/54 instructions). It remains NEAR_MATCH, not an exact-byte claim.
- Temporary format copy, append copy/growth boundary, temporary data free, base destruction, and optional declaration generation match retail ordering. Generate runs after temporary destruction. Existing-stream format aliases and empty/nonempty nested data are covered.
- Dedicated gate: 552 compiled plus 552 native cases. Vector copy, stream growth, and generation are explicit boundaries in this method-level gate. The fixture's free hook is active only while a case owns the input, excluding CRT startup cleanup. Generic catalog grading NEAR_MATCH/PASS, candidate build passes, and 28 shared-header dependents show zero regressions.
- Nine-file rollback manifest: `before-vertex-input-add-stream-20261001/manifest.json`. Proofs in `vertex-input-add-stream/`; symbol index refreshed. Next: stream-vector overflow `00AB4540`, then composed input building with real copies/generation.

## Stream growth and complete input-building chain promoted

- `00AB4540` stream-vector overflow matches 242 bytes after relocation; linked copy/fill/destruction helpers at `00AB4450`, `00AB4490`, and `00A5E900` also match (61/57/55 bytes). The method is genuine shared STLport instantiation with the previously recovered nested element-vector constructor kept as a separate dependency.
- Growth gate passes 870 native comparisons and 870 compiled cases. The compiled fixture includes the real element-vector provider; the native comparison keeps that nested constructor as an explicit boundary. Insertion positions, zero/multiple fills, end-only insertion, aliased source streams, nested storage, capacity and destruction are checked. Fixture cannot supply the growth target itself.
- Seven-provider `gate_vertex_input_build_chain.py` passes 2,240 cases with a failed-link check for each omitted provider. It covers format construction/appends/sizing, temporary and nested copies, stream growth, optional declaration generation, unsupported types, source mutation independence, existing-stream aliases through reallocation, and cleanup. Only root base-class lifetime and CRT remain boundaries.
- The composed link exposed duplicate public GetDataTypeSize definitions between the generator's inline expansion and the public lookup provider. Fixed by giving the generator's identical local helper internal linkage. Its entire accepted 662-byte code/table section is byte-identical before/after; generator gate remains 292 compiled / 293 comparative. The alternative of changing the public lookup's inline linkage changed CalculateVertexSize output and was discarded without promotion. One-file backup `before-declaration-inline-linkage-20261001/manifest.json`.
- Stream growth generic/candidate checks pass. Ten-file backup `before-vertex-stream-vector-growth-20261001/manifest.json`; direct and chain proofs in `vertex-stream-vector-growth/`. Landed seven-provider chain rerun passes. Native closure refresh running at `native-gfmain-input-building-closure-20261001`.

## Render-state initial data promoted; shutdown remains scratch

- Added `01/39/CRenderStateManager_StaticData_01396da8.cpp`: twenty addressed default-table entries, the three mip-filter entries, eight wrap-state IDs, and the current-manager null pointer. These are 23 data symbols / 128 bytes; no new function coverage is claimed.
- `gate_render_state_static_data.py` checks compiled COFF data (including uninitialized sections) against retail initial values. Current-manager storage is proved to lie in the zero-filled virtual tail of retail .data. Enum identities are cross-checked against local Windows SDK d3d9types.h. Real linked SetDefaultRenderStates passes 486 cases; removing either the data provider or caller fails the link. Fixture defines none of the recovered data symbols.
- Indexed as an explicit dedicated-gate provider outside the function catalog. Five-file backup `before-render-state-static-data-20261001/manifest.json`; proof in `render-state-static-data/proof.json`. Native closure refresh running at `native-gfmain-render-data-closure-20261001`.
- Prior input-building closure completed: 652 sources, zero duplicates, 689 unresolved. Shader-bank security-blocked lifetime/ABI batch remains untouched.
- `CShaderRenderManager::Uninitialise` at 0098A7A0 is not promoted: native walks counted Resources at +3A8, rereads its storage/size after each CShaderResource::Uninitialise callback (00A5D7A0), then zeros device +1D4. Ordinary scratch vector-view loop emitted 75 bytes versus native 79, similarity 0.7917; CArray GetAsCArray variant emitted 77 bytes / 0.6923. /Oa, /Og-, /Os probes did not improve acceptance. Scratch `shader-manager-uninit/`, native evidence `shader-manager-uninit-evidence-20261001.log`. No behavioral/coverage claim for it. Full Initialise 0098ACF0 and ClearRenderTarget 009D8CF0 evidence is in render-followup-evidence-20261001.log; both remain unrecovered.

## Typed font pointer sharing promoted

- Corrected four placeholder entries using native font initialization callers and PDB signatures: ShareData<CFontBank> 004190E2, ShareData<const CFontBank> 0042F9F2, assignment 00419463, and converting assignment 0042FE34. Shared typed implementation matches all four bodies after relocation (38/38/23/23 bytes); eight old mislabeled source/test files retired with backup.
- gate_font_pointer_sharing.py passes 1,200 compiled and 1,200 native ownership cases through the recovered release bodies, including identical control blocks, refcount wrap, null pointers and callbacks mutating source/destination fields. All 354 shared-header dependents show zero regressions. Four catalog/candidate checks pass; symbol index refreshed.
- Backup before-font-pointer-sharing-20261001/manifest.json covers 17 files. Native closure native-gfmain-font-sharing-closure-20261001 completes at 654 sources, zero duplicates, 664 unresolved. Preceding render-data closure was 653 / zero / 666. These are scratch link diagnostics, not a runnable native executable.
- Next: CCountedPointer<CFontManager>::Reset in CSystemManager::Initialise. Recorded shader-bank and color-fixture security blocks remain untouched.

## Font manager ownership reset promoted

- CCountedPointer<CFontManager>::Reset at 009A7340 matches 103 relocated bytes; native CSystemManager::Initialise calls it immediately after the CFontManager constructor, and the PDB confirms its typed signature. Corrected the unrelated propagated manifest label. The emitted 11-byte deletion thunk matches 009A6C10; no separate function coverage claim.
- gate_font_manager_reset.py passes 90 compiled and 90 native cases: absent/zero/one/shared/wrapping counts, null/aliased/distinct incoming data, allocation failure, and release callbacks replacing/nulling the live control block. Existing shared headers unchanged. Generic grading RELOCATION_MATCH/PASS, candidate build and landed gate pass; symbol index refreshed.
- Backup before-font-manager-reset-20261001/manifest.json covers eight files. Proofs font-manager-reset/{proof.json,landed-proof.json,catalog-proof.txt}. Native closure refresh running at native-gfmain-font-reset-closure-20261001.
- Next native font dependency: CStreamingFontDataBank::InitStreamingEntry at 00A5EF50. Existing declaration uses a pointer, whereas both PDB entries use CSymbolMap&; reconcile caller and fixture signatures with native evidence before promotion.

## Streaming-font entry initialization promoted

- Recovered CStreamingFontDataBank::InitStreamingEntry at 00A5EF50, frozen at first accepted NEAR_MATCH: 174 versus 178 bytes, 60 versus 61 instructions, similarity 0.9917355372. Native code constructs "GlyphData", looks up its bank index, destroys the string, gets a counted asynchronous entry, assigns it and releases the temporary. The CSymbolMap reference is unused, as in retail.
- Dedicated gate passes 450 compiled and 450 native comparisons with explicit string/bank boundaries. Null/shared/distinct handles, count zero/one/shared/wrap, sentinel indices and ownership-mutating callbacks covered. The initial compiled mock introduced an extra named-return copy; corrected to direct return construction so zero-count edge cases test the native boundary contract. Production code unchanged.
- Added PDB-confirmed CBankFile::FindIndexBySymbol and CBankFileAsync::GetAsyncEntry declarations. Corrected InitStreamingEntry from pointer to reference and updated the existing CFontManager constructor and fixture. All 115 shared-header dependents show zero regressions; generic NEAR_MATCH/PASS and both affected candidate builds pass. Both symbol-index entries refreshed.
- Twelve-file backup before-font-streaming-entry-20261001/manifest.json. Proofs in font-streaming-entry/. Closure refresh running at native-gfmain-font-streaming-entry-closure-20261001; prior font-reset closure completed 655 sources / zero duplicates / 663 unresolved. Next: the genuine bank symbol lookup at 009CCDC0 and asynchronous entry creation at 009D58D0; both old manifest labels are unrelated propagated guesses.

## Bank symbol lookup and real tree search promoted

- CBankFile::FindIndexBySymbol 009CCDC0 and its unsigned-value STLport _M_find 009D2DD0 match all 35/112 bytes after relocation. PDB names and native calls correct two unrelated manifest labels. Shared map currently lacks a const iterator overload; the const bank method uses its existing iterator through a documented const_cast, without mutating any tree data. No shared-header change.
- gate_bank_symbol_index.py passes 1,458 compiled and 1,458 native complete-chain cases: balanced/left/right trees, all sizes zero through eight, same/equivalent/absent keys, null strings, signed high-bit characters, zero and maximal stored indices. Compiled fixture invokes both real methods. Native oracle keeps only string comparison as an explicit boundary; complete object/tree bytes remain unchanged. Both generic/candidate entries pass and symbol index refreshed.
- Eight-file backup before-bank-symbol-index-20261001/manifest.json. Proofs in bank-symbol-index/. Native closure refresh follows. Preceding streaming-entry closure: 656 sources / zero duplicates / 664 unresolved (new implementation exposes two bank dependencies while resolving its own entry). Next CBankFileAsync::GetAsyncEntry 009D58D0 and its CBankFileAsyncEntry layout/lifetime.

## Asynchronous bank entry creation and alignment promoted

- CBankFileAsync::GetAsyncEntry 009D58D0 matches 152 relocated bytes; CBankFile::Align 009CBB90 matches all 24 bytes. PDB/native evidence establishes CBankFileAsyncEntry as a 16-byte CBaseClassNonCopyable descendant with bank reference, index and padded size. Its private constructor is inlined into the factory. The one-slot vtable and emitted 36-byte scalar destruction / 11-byte deletion thunk also match and execute in the native comparison; no separate coverage claims for those helpers.
- Align retains the native out-of-line call boundary. Factory's accepted bytes remain unchanged. Allocation of the entry may fail; failure of the later control block preserves the unowned entry pointer, as retail does. Size lookup reloads the bank reference before alignment. Unsigned arithmetic overflow is preserved.
- Dedicated gate passes 1,134 compiled and 1,134 native creation/virtual-cleanup comparisons, including both allocation failures, seven alignments, size overflow and a size callback that redirects the bank reference. All 118 header dependents show zero regressions; both generic/candidate entries pass; symbol index refreshed. Ten-file backup before-bank-async-entry-20261001/manifest.json. Proofs in bank-async-entry/.
- Native closure refresh running at native-gfmain-bank-async-entry-closure-20261001; prior symbol-index closure completed 657 sources / zero duplicates / 663 unresolved. Next: composed streaming-font setup with real symbol lookup, entry factory, size readers and packed-data access.

## Complete streaming-font bank setup chain validated

- gate_font_streaming_setup_chain.py links five real providers: InitStreamingEntry, symbol lookup/tree search, async entry factory/alignment/lifetime, entry size accessors, and packed UInt reading. All five omission checks fail the link as required. Boundaries are string construction/destruction, root base lifetime, and allocation/free.
- 360 composed cases pass, including two successive initializations, independent allocation failures, absent/runtime/packed/constant-bit sizes, and varied alignment. Cleanup follows actual virtual destruction. Tests preserve the native same-null-control-block assignment behavior: an entry whose control block allocation failed can be discarded without ownership transfer. Leaked fixture allocations are explicitly cleaned after validation.
- Three-file backup before-font-streaming-setup-chain-20261001/manifest.json; chain-proof.json and chain-landed-proof.json in bank-async-entry/. Registered in aggregate, without rerunning blocked generic fixtures. Latest native closure: 658 sources, zero duplicates, 662 unresolved (native-gfmain-bank-async-entry-closure-20261001). Native executable remains incomplete.

## Symbol-map storage release and trimming chain promoted

- CVectorMap<CCharString,CSymbolMap::CSymbolInfo,...>::DeallocateMemory 009E7680 matches 111 relocated bytes. Its emitted copy and string-destruction helpers also match 108/51 bytes (009E6600/006FBCC0), without extra coverage claims. A type-specific non-POD erase specialization performs the native element destruction; shared POD behavior unchanged.
- Dedicated gate passes 276 compiled/native cases covering all element counts zero through sixteen, spare capacity, null/non-null zero-capacity storage and preserved comparator/Dirty bytes. Each entry releases EntryName, GroupName, then key. Zero-capacity storage is left untouched; nonempty capacity is released and all three vector pointers cleared. Catalog/candidate checks pass; symbol index refreshed; no shared-header changes. Eight-file backup before-symbol-map-memory-20261001/manifest.json.
- Existing DeallocateUnusedMemory 009E7C70 links to the actual new release provider in gate_symbol_map_memory_chain.py: 340 cases pass with shared/null string references, exact-capacity copy, unchanged values, idempotent second trim and final ownership cleanup. Both providers are required by omission checks. Three-file backup before-symbol-map-memory-chain-20261001/manifest.json. Native closure refresh follows.
- Static/streaming font constructor evidence remains research in font-instance-construction-evidence-20261001.log; neither promoted. They require proper font-base ABI plus data-stream, glyph decompression and texture work. CBankFile::CreateSymbolMap evidence at 009CC530 needs the full bank virtual slot 19; existing shared base has only the first six slots. Symbol-map lifetime evidence in symbol-map-cleanup-evidence-20261001.log: scalar destructor 009E7BD0 is currently a mislabeled generic composite and needs real string-set/vector cleanup providers.

## Typed symbol-map lifetime promoted

- Replaced mislabeled generic scalar 009E7BD0 with the actual compiler-generated CSymbolMap destructor (48 relocated bytes), emitted by the existing constructor provider. NamesUsed's string-set destructor 004AD4A0 matches 66 bytes; SLONGMap's vector-map destructor 006FCF10 matches 37. Typed recursive tree erasure 004ABA70 matches 59 bytes and string-entry destruction 006FBCC0 matches 51; no extra helper coverage claims. The existing constructor remains exact at 76 bytes.
- Shared set model now declares an empty default destructor to make the CCharString specialization nontrivial to VC7.1. CSymbolMap uses its implicit virtual destructor, with explicit set/map destructor specializations. Other set instantiations retain their existing cleanup model. Full 380-entry header audit had five fixture compilation regressions; corrected their obsolete explicit CSymbolMap destructors and placed the real container provider before dependent instantiation. Focused reruns clear all five regressions. Sixth constructor fixture updated and passed in the full audit.
- Dedicated lifetime gate: 1,458 compiled / 2,916 native comparisons, zero through eight tree nodes and vector entries, three tree shapes, shared/null string references, flags 0..3 in native calls, ordered destruction and return ABI. Both providers required. Nine affected candidate builds pass, two symbol-index providers refreshed, landed gate passes. Twenty-file backup before-symbol-map-lifetime-20261001/manifest.json, old generic scalar source/test archived and retired. Proofs and audit corrections in symbol-map-lifetime/.
- Latest native closure: 660 sources, zero duplicate definitions, 660 unresolved (native-gfmain-symbol-map-lifetime-closure-20261001). Prior memory closure was 659 / zero / 661.

## Active correction: distinguish array storage release from map reset

- Tracing CSymbolMap::Clear 009E5250 (62 bytes) reveals it calls 009E79E0, not 009E7680. The former clears entries, releases storage unconditionally and resets Dirty at +0D; the latter, already recovered, is CArray's capacity-guarded storage release and preserves Dirty. Both donor symbols exist: CArray<pair<CCharString,CSymbolInfo>>::DeallocateMemory and CVectorMap<...>::DeallocateMemory. Existing curated header conflates them under the map name.
- Correct next: declare DeallocateMemory on CArrayVector, move the accepted 009E7680 definition/instantiation to that base type without changing emitted bytes, qualify the existing DeallocateUnusedMemory call to the base, and implement the true map method at 009E79E0. Update typed catalog metadata, tests/gates and shared-header dependents. No edits for this correction yet; existing implementations remain behaviorally verified against their addressed functions. Native bank CreateSymbolMap 009CC530 needs CSymbolMap::Clear and the bank's currently unmodeled virtual slot 19.

## Array release / map reset distinction corrected; Clear promoted

- Moved the accepted 009E7680 storage-release definition onto CArrayVector<SymbolEntry>, with a separate inherited declaration. Existing DeallocateUnusedMemory 009E7C70 now explicitly calls that base operation. Both accepted code sections are byte-identical before/after (111/152 bytes); only linkage/type names change. Corrected catalog/oracle metadata and the isolated trim/base-release fixtures.
- True CVectorMap::DeallocateMemory 009E79E0 matches all 103 relocated bytes: unconditionally erase entries, swap out storage, clear Dirty, then destroy/free old storage. It differs from the base method even for a non-null zero-capacity descriptor. Both variants pass 552 compiled/native cases; base-only gate remains 276/276 and the real trim/release chain remains 340 compiled cases.
- CSymbolMap::Clear 009E5250 matches all 62 relocated bytes. Four-provider composed gate passes 1,458 compiled / 2,916 native cases: vector reset precedes recursive string-set erasure, tree header remains allocated and reset, comparator/duplicate-check flags preserved, repeated Clear is idempotent, and final virtual destruction frees remaining ownership. All 439 affected shared-header entries show zero regressions; three generic entries and four candidate builds pass. Three symbol-index providers refreshed.
- Nineteen-file backup before-symbol-map-reset-20261001/manifest.json. Proofs in symbol-map-reset/, including unchanged-code-proof.json. Landed reset and 340-case chain pass; landed Clear rerun and native closure refresh pending at checkpoint. Next CBankFile::CreateSymbolMap requires the full base-bank virtual method at slot 19; current curated base exposes only the first six slots, so recover that ABI before adding its virtual call.


### Bank symbol-map construction and native virtual ABI

Promoted CBankFile::CreateSymbolMap 009CC530 as ordinary VC7.1 C++: exact 262-byte relocation match. A small inline string-return helper preserves retail construction without extra temporary copies; accepted body frozen. Clears unless appending, skips index zero/invalid entries and names containing `[`, resolves enumeration through virtual slot 19, adds dirty symbols, then updates. Preserves live bank-size reloads after callbacks.

Expanded the shared base bank interface to its 20 retail slots and added proven text/font/streaming-font/particle overrides. Donor-only AbortBankCreation/IsAborted methods are deliberately absent because retail vtables do not contain them. Added CCharString::FindCharPos declaration. Six emitted vtables / 120 slots independently match retail addresses and PDB-decorated identities; particle constructor is an ABI-only unlinked probe, not a reconstruction claim. Shader-bank lifetime was not compiled or promoted, and its security block remains untouched.

Validation: dedicated conversion gate 576 compiled and 576 retail comparisons, checking runtime/packed validity and type, missing symbols, bracket filtering, empty banks, append/replace, callback size changes, temporary lifetime, preserved registers and stack. Explicit boundaries remain string operations, packed reads, symbol-map operations, and enumeration. Existing fixture changes add trap boundaries for newly declared unused virtuals and concrete subclasses for two abstract bank constructor tests; current SymbolMap lifetime and streaming reference changes retained. Full shared-header audit: 625 entries, zero regressions (security-blocked 009A5490 and 00A62020 excluded). Generic catalog: RELOCATION_MATCH/PASS. Candidate build and both landed dedicated gates pass.

Sources/gates: CBankFile_CreateSymbolMap_009cc530.cpp, gate_bank_create_symbol_map.py, bank_create_symbol_map_oracle.py, gate_bank_virtual_layout.py. Scratch evidence: work/codex-native-startup/bank-symbol-map; backup before-bank-create-symbol-map-20261001 (42 files). The earlier reset closure completed: 661 sources / zero duplicates / 660 unresolved; bank construction closure refresh now running. Next investigate CCharString::FindCharPos 0099E5A0 and remaining real conversion dependencies. No retail launch or aggregate gate claim.


### Character search and bank enumeration composition

Promoted CCharString::FindCharPos 0099E5A0: exact 69-byte relocation match, 2,304 compiled/native cases (all 256 byte values across null, empty, repeated, bracketed, embedded-NUL, high-bit and longer strings). CRT strchr is an explicit native-oracle boundary, including signed char extension. Generic catalog, candidate, and landed dedicated gate pass. No header change. Backup before-character-search-20261001 (9 files), evidence character-search/ and bank-character-search-evidence-20261001.log.

Added gate_bank_symbol_map_chain.py: 576 compiled scenarios linking the real bank CreateSymbolMap, FindCharPos, and CPackedUIntArray reader. All three omission checks fail linkage as required. Boundaries remain string allocation/copy/destruction, symbol-map mutation and bank enumeration. Backup before-bank-symbol-map-chain-20261001 (3 files); landed chain passes.

Bank ABI closure completed with 663 sources / zero duplicates / 693 unresolved: the expanded virtual tables exposed additional true engine dependencies, rather than hiding them behind truncated declarations. Character-search closure refresh still to run. Existing symbol-map AddSymbolDirty and UpdateDirty providers are present; full composition is held by real sorting helpers, string FastCompare, and set insertion. Next recover FastCompare 009E6530 from native evidence, then sort support as appropriate.


### Native fast string ordering

Promoted CCharString::FastCompare 009E6530, 108 bytes / NEAR_MATCH 98.11%, 53/53 instructions; first accepted implementation frozen. Complete native oracle has no emulated calls. 2,592 compiled/native cases cover shared/null storage, length ordering, unsigned little-endian words, high-bit values, and changed bytes in the final padded word. Catalog NEAR_MATCH/PASS and candidate pass; no header changes. Backup before-string-fast-compare-20261001 (9 files). Evidence string-fast-compare/ and string-fast-compare-evidence-20261001.log.

Next scratch symbol-sort-helpers has exact relocation matches for __linear_insert 009E6FA0 (185), __unguarded_linear_insert 009E66F0 (173), and __unguarded_insertion_sort_aux 009E6D10 (83), using existing shared STLport implementation headers. Validation/ownership composition not yet complete; do not catalog until it passes. Existing emitted __copy_backward 009E6410 is a 108-byte helper to verify too. Native decompiler labels for these helpers are misleading; native call graph and shared template instantiations establish identity. Evidence symbol-sort-helper-evidence-20261001.log.


### Symbol-map insertion helpers

Promoted __linear_insert 009E6FA0 (185 bytes), __unguarded_linear_insert 009E66F0 (173), and __unguarded_insertion_sort_aux 009E6D10 (83): all exact relocation matches. Emitted copy-backward 009E6410 also verified at 108 bytes; no extra helper coverage claimed. Four-provider gate includes the actual FastCompare and all insertion helpers: 576 compiled/native cases, stable ordering, duplicate keys, front/middle/end insertion, sentinel-bound unguarded insertion, 1..32 prefix lengths, string copies/assignments/destruction, register/stack preservation. All four omission checks fail linkage.

A shared-header include initially caused the auxiliary provider to emit another unguarded insertion COMDAT; replaced only its local inclusion with the same ordinary template loop and the helper declaration. The accepted 83 bytes remain exact; providers now independently required. No shared headers changed. Native boundary stubs contain RET so Unicorn does not decode zero-filled memory across an unmapped page before the hook. Generic combined fixtures triggered a VC7.1 internal compiler error twice; focused single-helper catalog wrappers now pass 192 cases each, and emitted-symbol checks prove each does not provide its target. No compiler/source optimization workaround. Dedicated composed fixture remains optimized, 576 cases.

Generic catalog and three candidates pass; landed gate passes. Backup before-symbol-insertion-20261001 (14 files); stage symbol-sort-helpers. Native closure refresh running. Next partition 009E6C80 and its true 16-byte SymbolEntry swap 009E6480 (Ghidra falsely labels CSymbolInfo); 009E65A0 is a 12-byte vector swap and is unrelated. Native swap evidence symbol-entry-swap-evidence-20261001.log, donor API has pair<CCharString,CSymbolInfo> swap/iter_swap identities.


### Symbol partition and pair swap

Promoted pair<CCharString,CSymbolInfo> swap 009E6480 (168 bytes) and unguarded partition 009E6C80 (133), both exact relocation matches. New fable_stlport_pair_swap.h declares the pair-specific fastcall overload without changing existing generic swap users. Three-provider gate (swap, partition, actual FastCompare): 768 compiled/native cases including 1..32 entries, first/middle/last pivot, copied pivot across swaps, null/duplicate keys, sorted/reversed input, all pair/self-swaps, complete-entry permutation and string ownership. All three provider omissions fail linkage. Generic fixtures do not emit their targets; catalog, candidates and landed gate pass. Backup before-symbol-partition-20261001 (13 files). No existing shared header changed.

Latest completed insertion closure: 668 sources / zero duplicates / 689 unresolved. Partition indices refreshed, closure not yet refreshed. Next symbol-heap scratch has __push_heap 009E67C0 (205), __adjust_heap 009E6DB0 (287), __pop_heap 009E7060 (158), __make_heap 009E7100 (122), sort_heap 009E7950 (115), __partial_sort 009E7B40 (134) matching instruction sequences; complete relocation and behavior checks pending. Five accepted on first build; sort_heap corrected decrement ordering before acceptance. Existing accepted code frozen. New declaration-only fable_stlport_heap.h is scratch only. Evidence symbol-heap-evidence, symbol-heap-adjust-evidence and symbol-heap-push-evidence logs. LowerBound 009E6910 also traced as a later missing map-search dependency.


### Complete heap fallback and composed symbol sort

Promoted six heap helpers: __push_heap 009E67C0 (205 bytes), __adjust_heap 009E6DB0 (287), __pop_heap 009E7060 (158), __make_heap 009E7100 (122), sort_heap 009E7950 (115), __partial_sort 009E7B40 (134). All exact relocation matches. New declaration-only fable_stlport_heap.h isolates the six providers and preserves by-value string ownership. Seven-provider gate with real FastCompare passes 3,174 compiled/native cases: all retained prefix sizes for 0..32 entries, sorted/reversed/duplicate/null keys, actual full heap call chain, permutation and balanced references. All seven omissions fail linkage. Six generic fixtures do not provide their target; catalog/candidates and landed gate pass. Backup before-symbol-heap-20261001 (21 files).

Added composed gate_symbol_sort_chain.py linking all 15 real comparator, insertion, partition, swap and heap providers. 1,170 cases pass: 0..64 entries and six patterns under ordinary sort and forced introsort depth 0/1, with real final insertion sorting and all provider omissions required. Backup before-symbol-sort-chain-20261001 (3 files); landed chain passes. Latest native closure: 676 sources / zero duplicates / 687 unresolved. No retail launch or aggregate all-gates claim.

### Symbol-map binary lookup

Promoted const CVectorMap symbol LowerBound 009E6910, exact 77 bytes. Four-provider gate uses actual FastCompare, GetSymbolSLONG and SymbolExists, with no emulated dependency boundaries. 2,040 compiled/native cases cover empty maps, duplicate keys and first matching index, null keys, shared vs distinct key storage, missing keys, zero/positive/negative values, unchanged map memory, preserved registers/stack. Generic fixture corrected to call the const overload; no production change needed. Catalog/candidate pass; landed gate running. No headers changed. Backup before-symbol-lookup-20261001 (10 files), scratch symbol-lower-bound. Next _STL::set<CCharString> unique insertion and its allocation/rebalance path, needed by AddSymbolDirty duplicate tracking.


### String-set insertion and shared key extraction

Promoted set<CCharString> insert_unique 004AC380 (254 bytes) and node linkage 004ABC70 (206), exact relocation matches. Shared fable_stlport_tree.h now applies its declared _KeyOfValue selector instead of hard-coding pair.first, and _Identity has its missing const call operator. All 398 affected existing catalog entries pass without regression; security exclusions retained.

Dedicated gate_string_set_insertion.py links both new providers and the existing 190-byte rebalance provider, with actual rotations and signed CBasicString comparison. 1,088 compiled/native cases cover 0..16 operations, four insertion orders, eight key rotations, shared/distinct storage, duplicate results and no extra allocations, null/negative-byte/empty/prefix keys, root/parent/extreme links, red-black invariants, complete tree structure and reference ownership. Boundaries are malloc and string copy construction. The implicit predecessor helper at 004AA220 is separately compared behaviorally (78-byte build / 79-byte native, 91.89% similarity but 10.26% instruction delta); it receives NO recovered coverage. Other auxiliary comparator/rotations match, but only the two primary entries are newly cataloged.

Node linkage uses an explicit rebalance specialization declaration so it calls the existing recovered provider instead of emitting a competing /O2 COMDAT. Accepted 206-byte body unchanged. Generic single-TU fixtures adapt the same generic rebalance body via an int dummy type; dedicated gate uses the original bool provider, proves all three are required, and executes native rebalance bytes. Generic fixtures cannot supply their target. Catalog generic checks pass; an inherited PassPattern typo (extra CHAIN) caused one candidate false failure despite printed success, corrected, then both candidates pass. Landed gate passes. Backup before-string-set-insertion-20261001 (13 files). Latest prior closure 676/0/687; string-set/lookup index and closure refresh running.

Next assemble the full bank CreateSymbolMap path using actual symbol-map allocation, insertion, sorting, trim, lookup, clear and lifetime providers, with only string memory/basic class and virtual enumeration boundaries. Existing heap, lookup and set dedicated gates all passed after promotion.


### Full bank-to-symbol-map integration

Promoted gate_bank_symbol_map_full_chain.py and BankSymbolMapFullChain_test.cpp: 32 real providers and 4,352 compiled scenarios pass; removing any provider fails linkage. The chain includes bank enumeration/filtering, character search, packed reads, CSymbolMap lifetime, replace/append/reset, dirty insertion, red-black name tracking, vector allocation/growth/shrink, all 15 sorting providers, lower-bound/value/existence lookup, clear and destruction. Covers 0..33 bank entries, distinct/duplicate/bracketed/missing symbol arrays, runtime/packed validity and types, append vs replacement in two successive conversions, and duplicate tracking off/on. Checks every row's key/group/value, sorted order, exact trimmed capacity, duplicate-name count, zero/missing lookup behavior, repeated clear and final string ownership balance. Only string storage/lifetime, root base-class lifetime, and virtual enumeration remain fixture boundaries. Native behavior was independently established by the family gates; this is a compiled composition gate, not a new native oracle or coverage claim.

Backup before-bank-symbol-map-full-chain-20261001 (3 files); landed gate passes. Latest completed scratch link closure after set/lookup recovery: 679 sources / zero duplicates / 685 unresolved. Next native virtual enumeration methods: CBankFile 009CBD10, CFontDataBank 00A63380, CStreamingFontDataBank 00A5EF10, text bank 009C79E0. The full map chain is complete; continue recovering remaining native startup/font-bank dependencies. Both historical security blocks remain untouched.


## Typed bank enumeration complete

Recovered CBankFile::GetEnumerationType 009CBD10 (25 bytes), font 00A63380 and streaming-font 00A5EF10 forwarding overrides (22 bytes each), and NGameText::CDataBank override 009C79E0 (82 bytes). All four exact relocation matches, first accepted bodies frozen. Base returns HeaderEnumerationType at +0xF0; its legacy GetBankHandle label and local fake class were incorrect. Font wrappers replace naked assembly. Text selects EGameText/ETextGroup/empty for types 0/1/other. Native disassembly, shared PDB-derived declarations and validated retail vtable slot 19 agree.

Dedicated gate passes 640 compiled and 640 native comparisons, including hidden return ABI, callee-saved registers, complete unsigned type forwarding and ownership/null cases. Four omitted-provider checks; each generic fixture excludes its target. All four generic grades/candidates and landed gate pass. No shared-header changes. Backup before-bank-enumeration-20261001 (18 files), scratch bank-enumeration; old misnamed source/test retired. Symbol index refreshed. Closure refresh next.

Next: compose the four real enumeration providers into bank symbol-map conversion, replacing its enumeration behavior stub with dispatch adapters to recovered methods.


## Real bank enumeration composed

BankEnumerationChain now links 36 required providers, passing 17,408 compiled cases across base/font/streaming/text enumeration and full bank conversion, duplicate sets, sorting, lookup, append/replace and teardown. Fixture virtual dispatch adapters call qualified recovered methods; string storage and base-class lifetime remain boundaries. Scratch and landed gates pass, backup before-bank-enumeration-chain-20261001. No new function coverage claimed for composition. Latest native closure: 683 sources, zero duplicates, 681 unresolved, at native-gfmain-bank-enumeration-closure-20261001. Next inspect remaining bank policy/callback virtuals from the validated slot table, replacing legacy wrong-type or asm providers where justified.


## Bank entry update path complete

Promoted typed CBankFile EntryExists 009CC190 (57 bytes), EntrySetUpdate 009CC300 (75 bytes), and streaming AdjustUpdateEntries 00A5EEA0 (57 bytes), all exact. Retyped six existing constant policy methods onto their actual protected virtual signatures: base AdjustUpdateEntries 009CBA70/IsDiskFile 009CBC40, font version 00A63340/disk 00A63AC0, streaming version 00A5EE90/disk 00A5F9C0. First accepted bodies frozen.

Added two nonvirtual declarations to fable_game_text_bank.h; new fable_bank_entry_update.h models PDB 0x20-byte CBankFileEntryUpdateData and retail RequiresUpdate +0x14. Ten-provider gate passes 468 compiled/native scenarios with no emulated calls, 10 omitted-provider checks; tests distinguish packed versus runtime validity, shortened runtime arrays, boundary indices, prior/requested flags and repeated update passes. Native comparison also validates register/stack preservation. Generic checks and nine candidates pass; 125 header dependents have zero regressions. Backup before-bank-update-20261001 (28 files), scratch bank-update. Latest closure 692 sources / zero duplicates / 674 unresolved.

Next asynchronous bank Close 009D58A0 (RetailMode +0x8C controls threaded close), ProcessDisposalList 009D6730, then restart-list 009D5A90. Native/PDB evidence logs bank-async-cleanup-evidence-20261001 and bank-disposal-evidence-20261001.


## Async bank close and disposal complete

CBankFileAsync::Close 009D58A0 (36 bytes) and ProcessDisposalList 009D6730 (164 bytes) promoted at exact relocation matches. Native +0x8C RetailMode was named from PDB/retail cross-check; CThreadedFile::Close declared, and STLport list gained front/pop_front. 1,224 compiled/native scenarios cover empty/blocked queues, independent/shared/null ownership, final callback/deletion, list links, callee-saved registers and lock/close ordering. Two omitted-provider checks, generic checks, two candidates and landed gate pass. Header audit: 375 entries, zero regressions. Backup before-bank-async-cleanup-20261001 (15 files). Latest closure 694 sources / zero duplicates / 673 unresolved.

Next native restart loop 009D5A90: PDB class-method evidence establishes real private name RestartAbortedReads (?RestartAbortedReads@CBankFileAsync@@AAEXXZ), replacing provisional ProcessReadsToRestart. Threaded call 0098F350 is AddObject(const CThreadedFileObjectDesc&). Header/caller/fixture rename must preserve the accepted 16-byte AsyncUpdatePending body.


## Async restart path complete

RestartAbortedReads 009D5A90 promoted (200 exact bytes). Corrected provisional ProcessReadsToRestart to the private PDB name in shared header and AsyncUpdatePending; the latter remains 16 exact bytes. CThreadedFile::AddObject declaration and CThreadedFileObjectDesc class spelling follow donor decoration. Four-provider composition uses real disposal, restart, update caller and tree teardown; 900 compiled/native cases cover tree shapes, multiple owning banks, unsigned returned IDs, blocked disposal and disposal-before-restart locks. Generic checks, landed gate and both candidates pass; 140 header dependents, zero regressions. Backup before-bank-restart-20261001 (14 files). Promotion resumed after a pre-write manifest-column error; fixed the new generic fixture include path after candidate compile exposed it. Production bodies unchanged. Closure 695 sources / zero duplicates / 673 unresolved.

ACTIVE NEXT: CBankFileAsyncData::Abort 009D6350. Calls real set key erase 009D7F40, CThreadedFile::RemoveObject 0098EC90, CMovableResourceMemoryPool::FreeResource 00A62ED0. Crucial: Ghidra follows the FreeResource thunk into base Free and prints the latter body; raw 00A62ED0 is a 5-byte jump to 00A02B30, not the full pool Free. Existing misnamed Free_00a62ed0.cpp is a generic ForwardTarget wrapper. PDB methods and retail vtables establish 4-slot pool classes; retail handles vector is 12 bytes, yielding 0x464 pool size (donor 0x468). No abort/pool source edits yet.


## User-requested shutdown and exact resume point

Stopped on the user's request to call it a night and update the docs. No further reconstruction, promotion, compilation, or game launches after shutdown began. No owned reconstruction jobs remain running. No commits or deployments; preserve the large pre-existing dirty native/Lua tree.

Last fully promoted checkpoint is RestartAbortedReads: dedicated landed gate and both candidate builds pass. Latest native closure `work/codex-native-startup/native-gfmain-bank-restart-closure-20261001`: **695 sources, zero duplicates, 673 unresolved**. This is a link-frontier measurement, not a completion percentage. The separate visual bootstrap has passing evidence earlier in this journal; the reconstructed native game has NOT been linked/launched for visual verification. No aggregate-all-gates success is claimed. User asked whether visual-launch progress is 50%; explained that current evidence does not support a reliable percentage.

### Unfinished pool policy batch ? resume here

Scratch: `work/codex-native-startup/movable-pool-policy`. Nothing from this batch is promoted. Eight ordinary typed C++ providers are staged:

| Address | Method | Built bytes |
| --- | --- | --- |
| 00A62ED0 | CMovableResourceMemoryPool::FreeResource | 5 |
| 00A62F30 | CMovableResourceMemoryPool::MoveResource | 53 |
| 00A62F70 | CMovableResourceMemoryPool::CanMoveResource | 26 |
| 00A62EC0 | CMovableResourceMemoryPool::ValidateResource | 12 |
| 00A02120 | CHandleBasedMemoryPool::MoveResource | 26 |
| 00A03A40 | CHandleBasedMemoryPool::CanMoveResource | 5 |
| 00A03A50 | CHandleBasedMemoryPool::ValidateResource | 3 |
| 00A028B0 | CHandleBasedMemoryPool::InternalSetResource | 22 |

First seven probes have matching instruction sequences; the last has an exact raw-byte comparison. Keep accepted bodies frozen while completing full relocation and behavior validation. `probe.json` records decorated symbols/relocations/sizes. Native targets: pool Free is 00A02B30; memmove is 00BFEAE6. 00A62ED0 is a thunk, and Ghidra follows it into the full base Free body: do not mistake that decompile for its five raw bytes.

New scratch `include/fable_handle_memory_pool.h` models PDB-named fields, 0x20-byte CHeader, 4-byte CHandleInfo union, retail 12-byte Handles at +438, FirstUnusedHandle +444, Name +448, critical section +44C, total size 0x464. Pool classes have four virtual slots, with protected Move/CanMove/Validate overrides. CFailedAllocationHandler is a GLOBAL class, not nested. Scratch engine headers redirect to this hand-owned header. Pool virtual tables: base 0129C964 -> A03AA0/A02120/A03A40/A03A50; movable 0129D6A0 -> A62F90/A62F30/A62F70/A62EC0. The unlinked `PoolVirtualLayout_test.cpp` probe compiles and emits expected decorated slot names; formal native slot verification remains pending. Its constructors are ABI probes only, never production providers.

Header audit initially found one regression: old InternalSetResource used a local fake array and cast an opaque byte field. Typed replacement `Handles[handle].Header->Resource=resource` preserves all 22 bytes and fixes the regression. Four affected existing catalog entries now pass without regression. Evidence: `movable-pool-policy-header-audit-fixed.log`, stage `audit-path.txt` / `overrides.json`. Existing fixture for that function mirrors a local implementation; replace it with target-excluding candidate-linked verification during promotion.

`MovablePoolPolicy_test.cpp` is WRITTEN BUT NOT COMPILED OR RUN. Intended 2,000 cases: base/movable overlapping moves, zero sizes, null/present resources, callback-after-copy order, permission forwarding, validation, signed handles, FreeResource forwarding, and handle Resource assignment. Next: compile this fixture against the eight actual providers, build native comparison oracle and dedicated gate (including all eight omitted-provider checks), finish eight-slot ABI proof, then generic catalog checks and backup/promotion. Existing differently named files for FreeResource and ValidateResource must be backed up and retired, not duplicated. No gate/oracle/promotion script for this batch exists yet.

### Deferred Abort candidate

Scratch `bank-abort/CBankFileAsyncData_Abort_009d6350.cpp` remains UNACCEPTED and unpromoted. Initial early-return variant was 194 bytes; goto/direct-this/nested-success variants converge to 191 bytes versus retail 196, latest similarity 0.72 (below NEAR). Compiler keeps zero in EBX and spills success, whereas native keeps success in BL; equivalent source-control-flow probes did not improve it. No behavior gate yet. Avoid repeating these probes without new evidence. Scratch tree `erase(const key&)` and threaded RemoveObject declarations are NOT promoted; pool policy stage was copied from the current landed headers and does not include those additions.

Abort dependencies: set erase 009D7F40 (equal-range/distance/range-erase family still unrecovered), threaded RemoveObject 0098EC90 (manager 013BC870 -> RemoveObject 0098EB20), and movable FreeResource 00A62ED0. Evidence logs `bank-abort-evidence-20261001.log`, `handle-memory-pool-methods-20261001.log`, `movable-pool-policy-evidence-20261001.log`. Native/PDB differences in generated pool sizes were reconciled only in the scratch hand-owned header.

### Preservation and restart cautions

`work/codex-native-startup/shutdown-checkpoint-20261001.json` records current scratch hashes and exact checkpoint. Pre-shutdown HANDOFF, including the separate Lua lane, is preserved verbatim at `docs/journal/2026-10/HANDOFF_SNAPSHOT_BEFORE_NATIVE_SHUTDOWN_2026-10-01.md`. Canonical HANDOFF is now a concise native resume page.

Do not rerun one-shot `land_*.py` or `resume_land_*.py` scripts. Never recursively scan all of `work/codex-native-startup`; use explicit stage paths. Preserve first-accepted NEAR bodies and the unrelated Lua work. Security exclusions remain 009A5490 generic color fixture and 00A62020 shader-bank lifetime generic fixture: no retry, bypass or promotion of blocked shader lifetime sources. No need to rerun completed gates absent new changes; no game launch was authorized during shutdown.
