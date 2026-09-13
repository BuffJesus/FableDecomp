# Guardian encounter and resource review

The non-registered Lua reference now includes the first hit/talk encounter and
the repeat-hit/talk bodies. Forty-nine instrumented native cases match its
ordered events, completion state and remaining resource ownership. The existing
ten watcher comparisons also pass. This is not an executable FSE entity package.

## Converter evidence used read-only

`audit_guardian_resource_lifetimes.py` uses the converter's call-window analysis
and single-resource CFG checker without editing either. The full aligned native
Main range is E27E90..E28C9B exclusive. Forty-three records recover acquisitions,
map entries/copies, macros, movie starts/ends and resource destruction.

All native CFG paths have at most one active movie. An additional identity
analysis visits 1,121 instruction/state pairs and checks the active stack slot
against each destruction. The shared destructor at E28C85 receives either
ESP+68 or ESP+78 (hex) according to its incoming branch. It cannot safely be
assigned a single constant resource. The five negative mutations cover each
incorrect constant shared slot, a wrong ordinary destructor, a wrong start slot
and a missing destructor. All are rejected.

`native-resource-lifetimes-20260912.json` includes the full bytes, event operands,
analysis-source hashes and results. This checks movie identities and balance;
it does not prove engine internals, actor-map node reference counts or exceptions.

## First encounter

Replay entries are E2814A (hit) and E28411 (talk); ordinary execution stops at
E2878A, before the repeat loop. The caller's Maze resource is initially acquired.
The 27 cases cover entry cancellation, Hero and Maze acquisition failure/retry,
post-acquisition cancellation, cancellation during each individual macro,
post-sequence cancellation and post-completion cancellation.

Hit establishes ally relations in both directions. Both paths prepare/acquire
Hero at priority 4, check termination, reset/reacquire Maze at priority 4, and
check termination again. The actor map copies Maze then Hero and persists across
the complete macro sequence. Both paths start an unnamed movie, pause entities,
fix the camera, and run SETUP with skippable=false. Hit additionally invokes
HITME; INFO follows on both paths. HITME and INFO use skippable=true. All three
other macro arguments are zero. There are no native termination queries between
these macro calls; the replay records invocation, not macro interpreter playback.

Cleanup unfixes the camera, unpauses, destroys the movie, destroys the actor map,
and destroys Hero's local resource. A termination check then guards completion.
Cancellation injected during any macro reaches this cleanup and awards nothing.
Successful completion performs these native effects in order:

1. Give OBJECT_QUEST_CARD_FIND_TROPHY_DEALER for V_TrophyDealer, false.
2. Look up WitchwoodTeleporter, activate it, destroy the lookup wrapper.
3. Clear Maze's information flag and remove his marker.
4. Restore the captured initial angle, with the final argument false.
5. Add story entry 130; complete the active quest with false/false/false.
6. Write PieceOver=true, then check termination again.

There is no global SetTeleportingAsActive call in this range. The seed's extra
call is not carried into the reviewed reference. Termination after completion
releases Maze but does not undo the already-issued completion or PieceOver write.
Success retains Maze for the repeat loop. Pre-movie cancellation after Hero
construction destroys Hero then Maze; entry cancellation destroys only Maze.

## Repeat interactions

Replay entries are E28864 (hit) and E28AF8 (talk), after trigger selection.
Normal exits stop before the following use check or outer-loop frame. Twenty-two
cases cover entry/acquisition cancellation, retries, positive/zero/negative
health, busy waits, each post-speech cancellation boundary, and changing health
between the two hit-branch lines.

Hit reestablishes ally relations, resets/reacquires Maze and checks termination.
Talk uses the existing Maze resource without reacquisition. Each starts an
unnamed movie, pauses and turns Maze towards Hero with the final argument false.
Native GetHealth uses a temporary Thing copied from that exact resource; its
wrapper is destroyed before branching. The x87 comparison against zero executes
in the replay rather than being substituted with a preselected branch.

Hit conditionally speaks TEXT_QST_077_MAZE_ON_HIT, then independently rechecks
health before TEXT_QST_077_MAZE_REPEAT. Talk has only the latter. Each Speak uses
Hero, text selection 0, listen=false, sound2D=true, overFade=false on the outer resource
(7E7390). Task polling is 7E7450, with frame/query pairs while busy and another
query after task completion. Normal cleanup unpauses and destroys that branch's
movie; cancellation additionally releases Maze. No extra camera-fix call is added.

## Assets and remaining gates

`installed-dialogue-card-20260912.json` reads the installed files. All three
macros and all 11 requested localized text keys resolve. Their payloads and
source hashes are archived. The quest-card OBJECT entry exists, but the generic
schema decoder raises `unpack requires a buffer of 4 bytes`. Its raw payload is
archived with the error. Downstream card components and speech waveform playback
are not certified by this inventory. No parser or asset was modified.

Before staging an executable entity package:

- Replay initial entity acquisition, trigger predicates and complete outer-loop
  scheduling, including initial-angle Init; compose the reviewed branch bodies.
- Add/prove direct Speak, task polling and Thing access on explicit resources.
  Cached actor Speak must not silently substitute a different resource.
- Resolve the quest-card schema/component decoding and check registry identity.
- Build the actual adapter against current sol/C++ code and compare these traces.

Native replay API calls, storage and resource/movie operations are doubles.
Full native termination helper instructions execute, but real macro playback,
fiber switching, streaming, save/load and perceived audiovisual behavior do not.
No runtime source, installed DLL, quest activation, seed or converter file was
changed in this batch. Both installed DLLs still hash to v23:
`c3900b3db85423e1a7e4035825b265225e4892eb26522b92580aa2a055539d31`.

## Reproduction

```powershell
python work/runtime-20260912-video-comparison/prove_guardian_encounter.py
python work/runtime-20260912-video-comparison/prove_guardian_repeat.py
python -m unittest tools.script_recovery.test_guardian_encounter tools.script_recovery.test_guardian_repeat tools.script_recovery.test_guardian_trophy_waiter tools.script_recovery.test_guardian_resource_lifetimes -v
python -m tools.script_recovery.audit_guardian_resource_lifetimes --output work/guardian-lifetimes-new.json
python -m tools.script_recovery.audit_guardian_assets --output work/guardian-assets-new.json
```

The lifetime audit requires a fresh output path. Current result: 27 encounter,
22 repeat and ten watcher cases pass; the four unittest methods include the
archived CFG positive case and five rejected mutations. No runtime build was
needed because this batch changes only reference Lua, evidence tooling and docs.
