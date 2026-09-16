# New Oakvale Intro converter completion plan

Marathon resumed at user request on 2026-09-14. Current results and the live-test
approval state are in the [marathon checkpoint](../journal/2026-09/NEW_OAKVALE_MARATHON_2026-09-14.md).
The finish line below remains unchanged; offline results do not establish gameplay parity.

Started 2026-09-13 at the user's request to marathon through completion and make the
generated Lua human readable. This supersedes earlier stop/checkpoint instructions.

## Finish line

- All 51 inventoried functions across 17 owners are generated from native evidence.
- No unresolved executable operands, omitted behavior, unsupported control flow or
  cleanup substitutions remain. Syntax success alone is insufficient.
- The review package uses meaningful local names, readable state/control flow and
  small helpers where semantics permit. Native addresses and original names remain
  available in a separate source map; unknown meaning is never invented.
- Resource, Thing, condition, movie, timer and persistence lifetimes match native
  behavior on normal, retry, cancellation and error paths as applicable.
- Offline behavior checks, runtime build/API checks, and New Oakvale playthrough
  comparisons are recorded separately. Registration waits for integration gates.

## Work sequence

User clarification during review: semantic renaming alone is insufficient. Native
jump labels must ultimately become ordinary Lua control flow or meaningful local
helpers, and native calls must use verified ForgeFSE APIs/adapters. The intermediate
husband label map is reversible presentation work, not completion of this requirement.

1. **Inventory and readable output.** Add reproducible per-function gap/readability
   reports and a separate readable package. Preserve raw output and evidence. Start
   with safe local renaming and source maps; follow with proven control-flow cleanup.
2. **Common semantics.** Finish husband cached Thing/hit ownership and dynamic byte
   access; prepare bounded runtime storage and API integration. Generalize useful
   resource/condition/vector lowering with evidence checks.
3. **Complete bodies.** Work from small quest helpers and entities through the affair
   actors, barrel scenario, bully/teddy/victim, father/Theresa/guard, and quest phases.
   Resolve the four syntax failures by recovering their missing semantics, not by
   commenting out instructions. Update the function ledger after each batch.
4. **Readable structure.** Separate reused temporaries where data flow proves it,
   remove dead staging, name branch states and helpers, and simplify redundant
   constructs without changing effects or cancellation behavior.
5. **Integration.** Produce a reviewable runtime patch/build and complete package,
   validate bindings and persistence, then perform trace/playthrough comparisons.
   Preserve unrelated work in the shared runtime, reconstructed ports and game.

## Baseline

51 functions, zero missing bodies, 47 function syntax passes; 14/18 files compile;
1,296 diagnostics. Four syntax failures: AffairWife, AffairWoman, BarrelMan, Bully.
The disabled husband candidate has explicit resource and movie/pause ownership;
15 candidate tests and 36 combined focused tests pass. Other gaps remain explicit.

Progress is tracked in generated reports and the current handoff. Do not declare
completion from reduced diagnostic counts, readability changes, or mocks alone.

## Syntax milestone reached

All 51 functions and 18 files now compile, including the separate readable package.
Latest checkpoint: 1,203 remaining diagnostics; 873 semantic and 192 scratch local
names. Thirty-one functions have no converter diagnostics, which still does not
by itself establish behavioral parity. Continue the complete-body and ownership
work above; the package remains disabled.

## Active control-flow work

The structured draft now lowers non-fall-through switches using a single selector
evaluation and a single-iteration loop; outer-loop continues and fall-through are
rejected. Some cleanup jumps still become explicit return diagnostics.
An opt-in `--flat-control --out <separate directory>`
backend now lowers structured control to one label scope. It preserves switch
fall-through, break/continue destinations, nested-entry jumps and short-circuit
comma assignments in focused tests. Synthetic labels and native-label mappings
are reported separately.

This is experimental, not the human-readable release: native pointer/refcount
conditions remain unresolved, and operand alias/staging data flow across the new
joins must be validated before promotion. Do not infer correctness from fewer
diagnostics. The canonical readable artifact still uses the structured draft.

## One background port worker

User explicitly requested sustained work by one background agent on other
incomplete Lua ports while the primary agent continues New Oakvale. Current
assignment: ScytheInfo, starting with its quest timer/operand gaps and missing
ScytheMarker, ScytheNearOracle and PlayCutscene bodies.

The worker owns separate `scythe_*`/`native_scythe_*` tools, witnesses, tests and
`work/scythe_converter/` output. It sends shared converter integration requests to
the primary agent. Registration stays disabled until actual package validation.
Keep this worker productively assigned across resumptions; do not multiply agents
or change runtime/game files as a side effect of this assignment.
