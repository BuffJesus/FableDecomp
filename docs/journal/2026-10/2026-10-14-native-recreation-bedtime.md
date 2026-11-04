# Native recreation paused for bedtime

The user asked to stop on October 14, 2026, at approximately 21:36 Edmonton time.
Do not resume until asked. No Lua work. The review agent was interrupted; no new
build or native invocation should start during the pause.

## Check the existing test first on resume

The corrected standalone layout test was still running at the handoff:

- Runner session: `98118`; native process: `15544`, started at 21:31:31 local.
- Last verified poll: `eb6bca`; process CPU time: 311.23 seconds.
- Last output: 36,864 completed cases, 811,008 checkpoint comparisons and
  1,021,952 callback comparisons. This is progress, not a pass.
- Output directory: `work/native-recreation/frontend/0042df9e/20261014/shader-layout-chain/layout_observer_native_FIRST/`.
- Read `result.json`, `process.json`, `after_native_guard.json`, `stdout.raw`
  and `stderr.raw` if present. Otherwise poll the same session/process.
- The existing run was left undisturbed. Its fixed timeout is 1,800 seconds,
  approximately 22:01:31 local. **Never restart it merely because the session
  handle is unavailable. Inspect the saved terminal evidence first.**

A pass requires all 196,608 cases, 4,325,376 checkpoint comparisons, 8,650,752
paired checkpoint snapshots, 192 ordered progress records and the exact final
marker. The runner validates these plus callback totals, exit status, stderr,
input hashes and lookup inventories. Independently review the saved result
before closing this work-only milestone.

The previous standalone attempt timed out and remains unaccepted. The corrected
test removes repeated whole-heap guard scans inside each masked-byte snapshot;
it retains every case and comparison. Engine provider instructions are unchanged.

## Evidence timing qualification

The corrected executable and runner passed independent review. Root signoff
`51d1bf78...` verified the review receipts, seals and invocation relationships,
but omitted actual reviewer/sealer source hashes and exact command/cwd checks.
The reviewer identified this after launch. The immediate postlaunch audit found
no drift; its receipt is `root_observer_review_sources_postlaunch_audit.json`,
SHA256 `c993342444ad61f421a007bcbe595e91670302747748a4ce5e442665069da926`.
Preserve this timing qualification. Do not describe that audit as preflight or
replace the original signoff. Nothing in the active test was modified or restarted.

## Frontend registration build

The new five-scenario frontend fixture compiled and linked, but has not run.
It replaces mocked shader-layout registration with genuine retained code and
adds an empty-table allocation scenario. Planned matrix: 2,949,120 cases and
5,898,240 snapshots.

All paths below are under `shader-layout-chain/`:

- Build: `frontend_registration_link_FIRST/`; result hash `6f191200...`.
- Executable hash: `165de7392d51936dedf6b30bf56fa9357c342f93ebd15bc53e1a5169ececb4be`.
- Main executable review: `independent_FRONTEND_REGISTRATION_PE_review.json`,
  hash `b9346b1d2610e880066ebf82396aaa6fc0e59198f90ba2b090a3637a32099b9e`.
  The reviewer reported 194 mapped code symbols, 213 whole selected sections,
  82 original arrays, 32 absolute data bindings and all three registration callers.
- Focused pixel/table/import checks, final review seals and native runner review
  were still pending when the reviewer was interrupted. Inspect actual files;
  do not infer completion from this handoff.
- `run_frontend_registration_native_once.py` is prepared, unrun, with a
  prospective 5,400-second first-execution limit.
- `sign_frontend_registration_native_once.py` is prepared, unrun. It verifies
  actual reviewer and sealer files before signoff. **One additional dependency
  still needs binding:** the frontend PE reviewer extracts the `coff()` AST from
  `independent_review_saved_layout_pe.py`. Check and pin that file against the
  PE receipt's `coff_parser_source_sha256` field before frontend launch. Preserve
  the initial signoff script and derive a fresh amended file.

Reuse the single authorized review agent `/root/native_review` after the user
resumes. Do not create another reviewer. Its last task was the focused frontend
PE checks and native-runner review.

## Integration gap to carry forward

The separate four-scenario frontend/layout baseline passed 2,359,296 cases and
4,718,592 snapshots. Its saved result hash is `95290424...`; it still mocked
registration.

The newer menu/queue frontend branch is not a superset of the earlier frame/clock
branch. Its frozen source and actual map still define `BeginRender`, `EndRender`,
`SwapScreens` and `GFGetTime` as fixture services. Earlier genuine frame/core/clock
providers are present in
`vertex-shader-chain/first/FIRST_native_behavior/fixture.map`; that older saved
result is PASS. After current validation, investigate combining these branches
while retaining both sets of cases and qualifications. Projection, physical
graphics/UI services and other documented boundaries remain; no complete game
boot or running world is claimed.

Exact earlier hashes, scripts, failed attempts and review details remain in
`2026-10-14-native-recreation-continuation.md` and the preserved work directories.
Do not edit canonical headers, coverage/index/grades, HANDOFF or ROADMAP as part
of this checkpoint. No commits, GPU/game launch or Lua recovery occurred here.
