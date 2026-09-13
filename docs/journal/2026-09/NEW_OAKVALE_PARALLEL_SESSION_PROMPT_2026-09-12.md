# Parallel session: New Oakvale runtime fixes and retail script ports

## Latest restart point: overnight stop

Read [the overnight handoff and checklist](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md)
before this historical session prompt. User stopped for sleep; no automatic work
or launch until they return. Childhood has been completed locally with unchanged
original FSE plus the isolated add-on. Installed baseline remains v23; audio
config8 failure path is confirmed and no audio correction is installed.


Work from `D:\Documents\FableTLC` and the runtime implementation in
`D:\Code\ForgeFSE-retail-shadow`. Own the retail-script reconstruction and live-test work while
another session continues development of the native-to-Lua converter. Work persistently through
diagnosis, implementation, offline validation, and coordinated live playtesting.

## First objective: finish New Oakvale Intro bug testing

Read, in this order:

1. `docs/journal/2026-09/BULLY_RUNTO_STALL_2026-09-11.md`
2. The runtime sections of `docs/HANDOFF.md`, especially **Latest continuation** (v15). The top v13
   description and older gate counts are historical; verify actual installed files before acting.
3. `docs/journal/2026-09/NEW_OAKVALE_MARATHON_CHECKPOINT_2026-09-09.md` (latest September 11 entries).
4. `docs/scripts/NEW_OAKVALE_INTRO_RECONSTRUCTION.md` and the relevant artifacts under
   `refs/script_recovery/new_oakvale_intro/runtime_evidence/`.

Outstanding user-visible bugs:

- Bully run-off: verify movement, premature disappearance, and the camera transition/pan. Preserve
  retail's authored sequence. Retail `RunTo` waits on `IsPerformingScriptTask`; the repeated failed
  endpoint is `MK_OIBR_BULLY1`, the authored pre-run teleport, so the failed task moved **zero metres**.
  The old claim that it ran 2.3 m before stalling was corrected. Incomplete captures stopped 11.759 m
  short of `MK_OIBR_BULLY2`. Moving `Drawable FALSE` after the second macro was tried, broke retail
  ordering/a fixture, and was reverted; do not reapply it as an unexamined fix.
- New user report (September 12): **when the camera faces the speaking NPC, the dialogue becomes
  quiet and distant**. Reproduce and trace this independently of the run-off issue. Compare the same
  actor/line across camera orientations, camera cuts, and actor/listener positions. Inspect the
  native and Forge speech operands, 2D/3D flags, listener/camera transform, attenuation, and channel
  routing as hypotheses, not established causes. If needed compare native-only behavior with the
  reconstructed path using an isolated/reversible setup. Do not globally raise volume or change
  unrelated speech flags to conceal the defect.

Last recorded runtime build is **v15**, installed after the v14 video showed compatibility movement
being cut short by a 2 m threshold while its task was still active. v15 waits on the native task
predicate, retaining a 300-frame guard, and adds passive `MoveToThing` instrumentation at `0x00903E20`.
The earlier probes cover `MoveToPosition` at `0x00903BC0` and `IsPerformingScriptTask` at `0x00904EE0`.
Inspect `[CutsceneMoveDiag]` issue/poll/controller identities to determine whether the native
in-macro task is rejected, cancelled, or moving. Handle reuse also occurred in a successful capture;
it is a mismatch to investigate, not a proven cause by itself.

Last recorded v15 DLL SHA-256:
`8102215BF082508052FDF451B1FE2BAB61E2FD8FC736DD93F4389EA69C6DF80E`.
Rollback: `FSE/backups/new-oakvale-v15-bully-wait-20260911-194754` under the game install. The game
was last recorded closed; check current process/install state rather than assuming this is current.

Keep the successful childhood behavior intact: green-filled/red-empty Bully health bar, Affair Wife
dialogue and travel, Affair Man walk-home, one appropriate Guard lecture, warehouse outcome/reward,
Theresa/chocolates, raid and post-raid cutscenes. Preserve the v13 control contract: same-priority
reacquisition saturates at depth 1, different priorities add a nested level, and only the outermost
release destroys the handle. Plain same-priority idempotence previously broke Affair Man's walk-home.

Before a live run, verify single authority (`native=true legacy=false`) and inspect the installed
`FinalAlbion.qst` for `AddQuest("NewOakValeIntro", TRUE)`; Steam verification can restore that
duplicate activation. Use `FSE_Launcher.exe` and coordinate launch timing with the user for interactive
tests. Archive the exact build/Lua hashes, logs, relevant video timings, and observations. Distinguish
reproduced defects, supported fixes, and outcomes still awaiting a live test. Preserve rollback copies
for install changes and do not overwrite files while the game is using them.

## Validation and continued porting

Relevant commands from the FableTLC root:

```powershell
python -m unittest discover -s tools/script_recovery -t . -p "test_*.py"
python tools/script_recovery/validate_reconstructed_package.py --fse-root refs/script_recovery/reconstructed/NewOakValeIntro/FSE --package Q_NewOakValeIntro --fixtures refs/script_recovery/new_oakvale_intro/fixtures --traces refs/script_recovery/new_oakvale_intro/traces
python tools/script_recovery/validate_new_oakvale_authority.py --profile-fse refs/script_recovery/new_oakvale_intro/runtime_playtest --source-fse refs/script_recovery/reconstructed/NewOakValeIntro/FSE
python tools/script_recovery/audit_forgefse_control_abi.py --forge-root D:\Code\ForgeFSE-retail-shadow
python tools/validate_tooling_sdk.py --skip-mirrors
```

Latest converter-session recovery suite: 394/394. Last runtime package fixture count: 119/119;
rerun it for your changes. The canonical SDK passes, but full cross-repository SDK validation reports
pre-existing overlay drift in the FableForge and ForgeFSE mirrors. Do not confuse those mirrors with
the `ForgeFSE-retail-shadow` runtime checkout.

Once these New Oakvale defects have been verified fixed, continue retail-script porting using the
roadmap, existing native clusters, reviewed inventories, and trace fixtures. Select and document
the next supported porting target based on those dependencies. Converter outputs remain drafts:
review native TODOs, reconstruct missing behavior, and validate before enabling any package. If
live testing is waiting for the user, continue independent evidence recovery and port preparation.

## Shared-workspace ownership

Both repositories contain substantial uncommitted work. Preserve it; no reset/clean/stash sweep or
blanket commit. The stray root file `0x0382cff0` must not be committed.

This session owns New Oakvale reconstructed scripts, runtime evidence, related fixtures/audits,
test bundles, Forge runtime fixes, and subsequent reviewed retail ports. The converter session owns
`tools/script_recovery/lift_native_lua.py`, `benchmark_lifter.py`, their tests, converter-specific
support modules, `refs/script_recovery/lifted/`, and `docs/scripts/LIFTER.md`. Read those if useful,
but do not edit or regenerate them from this session. Record converter defects as evidence for that
session instead. Avoid changing the shared API manifest/overlay solely for a converter requirement;
coordinate any runtime-driven binding changes before regenerating them.

Keep runtime findings in a dedicated journal/handoff file such as
`docs/journal/2026-09/NEW_OAKVALE_RUNTIME_HANDOFF_2026-09-12.md`. Avoid replacing shared
`docs/HANDOFF.md` or converter status while both sessions are active. Keep the user updated and
continue the authorized work without repeated general confirmation requests.
