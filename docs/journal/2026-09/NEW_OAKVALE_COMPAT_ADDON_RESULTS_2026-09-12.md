# Original FSE add-on: offline candidate ready

## Current results ? September12 overnight update

[Overnight handoff and checklist](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md) is the
current restart point. Earlier ?no live evidence? / ?awaiting first launch?
statements below are superseded. Original FSE and the add-on initialized together;
the New Oakvale native identity was overridden once, and the user completed the
entire childhood. Logs record quest completion, and the user reported retail-like
behavior except camera-facing quiet speech. Save/reload and other custom-quest
coexistence remain unverified; no distribution approval or release is implied.

V4 established the six-channel/disabled-position-update state. V5 main-menu
tracing captured successful DirectSound config8, unsupported by retail's mapping.
The V5 add-on differs from the successful childhood add-on only by an isolated
audio-query diagnostic include/install; original FSE, launcher and Lua remain
unchanged. It is not an audio fix or a newly completed childhood validation.
User stopped for sleep; do not ask for another launch tonight.


Candidate: `work/new-oakvale-original-fse-20260912/local-test-ready`.
NOT launched in Fable. No distribution or game-directory changes. All25 installed
v23 files still match deployment hashes. Upstream checkout is clean. Shared runtime
and converter sources were untouched; add-on built from an isolated runtime snapshot.
Plan: NEW_OAKVALE_COMPAT_ADDON_PLAN_2026-09-12.md. Live gates remain open.

## Implemented architecture

A substantial sidecar with separate quest hosts/VMs, not eight small binding fixes.
Original FSE stays loaded and its callback runs. No sol/VM/C++ host ownership is
shared across DLLs. The game remains their common native interface.

Local launcher creates the game suspended, loads original FSE then the add-on,
calls its start export, and resumes only on success. Add-on validates original
hook ownership/bytes, chains its callback, and installs the identity-preserving
AddScript override. It does not install renderer probes, card-clear, speech hooks
or map aliases. The custom-map queued-transition API is unused by this quest.

Config/log paths are separate: FSE/ and NoviCompatibility/ beside the test DLLs.
Original FSE registry is empty for this FIRST test; other original-FSE quests
remain a later coexistence gate. Add-on has one Q_NewOakValeIntro override and16
entities, with no legacy quest entry. No installer is required.

Rollback: close the process and use installed FSE_Launcher.exe for v23. Saves are
shared; wrapper backs up Saves under runs/ before each launch. Closing the game
does not revert written saves. Wrapper starts the read-only audio/cutscene observer
for game lifetime or30minutes; inspect observer.log for startup errors.

## Pinned binaries

- Upstream commit b2432e82b2ef1e5fd50b8fac2a8809bd2bd107d6.
- Original DLL 36ffb32ce4f1689d08cd5deef37829760d9f2a840aa9e801e79c86135fdb95ea.
  Locally compiled WITHOUT source changes; not a maintainer release binary.
- Add-on 74e2b482d12196be91c743152becf20243db43b5089a0e9df6c6f8108ded5755.
- Launcher 98fdf6a3469641c13b8551449c8bb16263bbfd0b8d5ebe2ae68e938608dee68a.
- Fable.exe 41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10.

Both DLL builds passed. Source snapshot/transform hashes:
sidecar-final-source/snapshot.json. Candidate manifest pins30files.

## Validation and limits

-9 initialization/gating cases and3 callback machine-state runs pass, actual
 included chain code with OS patch operations/registration bodies doubled.
-6 executions of BOTH built DLL hook bodies pass: two image-base arrangements,
 three flag patterns, original callback and displaced retail epilogue. Registration
 bodies doubled; this is not a game lifecycle test.
- Windows fixture: both fixture DLLs initialize before primary execution. Missing
 original/add-on paths reject without running the primary code.
-21 Python preflight/observer/audio checks pass.
-123/123 packaged Lua fixtures pass without syntax/API/scope errors or warnings.
-12 static control ABI checks pass on sidecar source.
- Actual profile checked: empty legacy, one override,16entities.
- Preflight verifies payload/game hashes and absence of duplicate legacy activation.

Evidence under work/new-oakvale-original-fse-20260912:
local-readiness.json, loader-checks-final/result.json, binary-chain.json,
package-fixture-v2.json, integrity.json. Final candidate Lua bytes match the
tested fixture view exactly.

## Preserved failed attempts

Kernel32 bootstrap failed before either fixture DLL loaded in a suspended process.
Checked ntdll LdrLoadDll bootstrap fixed it. loader-checks and loader-checks-v2
retain failures; v3/final pass.

Source-derived original hook prefix initially omitted assembler WAIT. Actual
upstream RVA33500 starts6083EC289BD93424. Final sidecar checks those exact bytes;
actual binary chain proof passes.

First package copied installed v23 Lua:90/123 current fixtures passed,21errors,
including older Bully cancellation behavior. It is preserved and superseded.
Final package uses reviewed current Bully/Guard/LiveFather/Villager updates and
passes123/123. It is NOT identical to previously playtested v23.

Legacy validator expects a quests.lua entry; its CLI threw KeyError on the actual
empty override registry. Exact payload Lua was copied to a temporary fixture view
with the standard test registry. Actual payload keeps empty legacy and its override
profile is checked separately. Shared validator/converter files unchanged.

## Next live run, when user requests launch

    python work/new-oakvale-original-fse-20260912/local-test-ready/local_test.py --game-dir "C:/Programs/Steam/steamapps/common/Fable The Lost Chapters" --launch

Saves path defaults to C:/Users/Cornelio/Documents/My Games/Fable/Saves; unavailable
path rejects and requires --save-dir pointing to the actual directory.

Check BOTH module paths and separate startup logs, then fresh profile, Father and
Escape, Bully skip, deeds, Theresa/chocolates, raid and save/reload. Father's quiet
audio remains unresolved. Do not distribute or claim gameplay compatibility until
these local gates pass. Keep the compatibility goal active pending real-game tests.

## Additional packaged-launcher failure audit

The next goal turn made progress through additional Windows-process evidence.
The exact launcher from local-test-ready passed the expanded fixture, including
an add-on start export returning99 and an export stalled for45seconds. Launcher
rejected both with result9; the stalled call reached its30second timeout. The
child process was verified terminated via Windows process handles (or confirmed
absent), and an entry marker at the start of fixture main remained absent.
Missing original/add-on paths still reject; successful two-DLL startup still passes.
No actual Fable process was launched. Payload preflight passed again; no Fable or
FixtureGame process remains. No payload bytes were changed by this audit.

Evidence: work/new-oakvale-original-fse-20260912/packaged-launcher-failure-audit/result.json.
The runner now accepts --launcher to test the exact candidate executable, rather
than relying only on recompilation of matching source. The earlier failure audit
is preserved separately. Interactive launch timing has been requested from the
user, per the handoff's coordinated-playtest requirement. Real-game gates remain.

## Completion audit: awaiting coordinated live test

Current-state audit confirms the local candidate remains intact, no runs directory
exists, and neither Fable nor the fixture process is running. No live initialization,
registration/reload, gameplay or save/reload evidence has been produced.
Plan steps1-3 have source/build/isolated evidence; step4 callback mechanics and
step5 loader boundaries have executable fixture evidence, but actual host coexistence
and native registration remain live gates. Step6 uses the revised no-install design:
installed v23 files remain untouched, and save backup is prepared for first launch.
Step7 is unperformed. Therefore the compatibility goal is not complete.

The same pending interactive launch timing has persisted across the initial goal
turn and two continuations. Offline implementation and failure-path checks are
complete for the first live candidate. Stop automatic continuation pending the
user's availability; resume when they request the local test launch. Do not
substitute further synthetic checks for real game compatibility evidence.
