# Verification contract and current limits

This page separates the project's intended acceptance standard from gaps in its
current enforcement. It does not promote source, rerun native evidence, or
recompute coverage. Follow [CONTRIBUTING](../../CONTRIBUTING.md) for authoring and
the [working handoff](../HANDOFF.md) for the owning lane's current state.

## What each result establishes

| Result | Evidence it can establish | Evidence it does not establish by itself |
|---|---|---|
| Source/generated output exists | An implementation or draft is available | Compilation, behavior, readability, or reconstruction coverage |
| Syntax/structural check passes | The checked syntax or structural rules hold | Execution or original behavior |
| Compiler succeeds | The selected translation unit compiles under the selected inputs/options | Correct linkage or meaningful behavior |
| Exact instruction bytes match | The compared instruction bytes agree | Complete service ownership, runtime integration, or human-readable design |
| Relocation-masked match | The compared non-relocated bytes agree | Correct identities for masked helpers/globals/vtables |
| Near match | The configured normalized instruction metric passes | Behavioral equivalence; normalization intentionally removes some differences |
| Behavior fixture passes | The fixture's actual assertions hold for its exercised implementation | Unexercised paths, a different implementation, or a complete engine |
| Composition/replay passes | The stated providers, inputs, ordering and controls pass | Arbitrary hardware/services/lifetimes outside the declared boundaries |
| Integrated runtime milestone passes | The explicitly tested runtime contract works | Untested gameplay, release readiness, or every original behavior |

The intended native reconstruction standard is genuine compiler-emitted C++,
meaningful production-bound behavior checks, and accepted exact/relocation or
near-match evidence. Exact byte identity is useful, but the agreed functional-
first policy does not require chasing it after qualified near-match acceptance.
Handwritten instruction bodies remain oracle/reference material rather than
genuine C++ reconstruction, even when their bytes match.

## Admission gaps that remain in the tools

- The central lander's exact/relocation behavior path can accept a self-contained
  test without linking the production object. A literal success-printing test
  proves its marker, not production behavior. The newer near-match path requires
  the candidate, but that protection is not universal across all writers.
- Some retype/residue paths can continue when a paired test is missing. Public
  historical retype logs inspected by the audit all recorded PASS; the missing-
  test code path is not proof that those historical retypes used it.
- Different source writers enforce different purity, header, flag, and staging
  rules. A source file's directory or filename is not proof of admission.
- Relocation masking does not itself verify the identity of every relocated
  target. Original target/provider identity and behavior still need evidence.
- Generated reports and caches do not uniformly pin the complete source, test,
  include/provider, oracle, compiler/options, and verifier dependency closure.
  An unchanged source hash is insufficient when a dependency changes.

When accepting new work, require a fixture that reaches the intended production
implementation, checks its outputs/state/calls, and has a relevant failing
control. Record the exact provider and dependency scope. Keep parity-only
evidence under an honest separate assurance label rather than discarding it or
calling it independent behavioral validation.

## Types, naming, and readable code

Donor symbols and layouts are actively used. They remain evidence to reconcile
with retail, not automatic proof of equivalent owners, ABI, or layout.
Some shared headers are retained despite conflicts because existing users depend
on them; current INDEX `OK` does not always mean conflict-free. Preserve those
users while explicitly recording unresolved members and checking new dependent
uses. Do not equate a shared-header include with a fully readable call chain.

Use proven field/callee names and ownership models. Keep necessary ABI boundaries
explicit. Review secondary objects and lifetimes as well as the first typed
receiver; avoid replacing one unexplained offset with another opaque wrapper.

## Coverage and exported facts

Existing dashboard/README values are generated from the current reporting
inputs. The engineering audit found duplicate-address, assembly-exclusion,
population, and proof-freshness gaps in that path. Treat those values as recorded
dashboard results, not independently certified percentages of behavior-verified
genuine C++ or playable-engine completion. Do not fix them by hand-editing counts.

Reconciliation needs one unique-address ledger with explicit population,
purity, validation level, and dependency/proof identity. Compile/parity counts,
meaningful behavior validation, readability, and runtime milestones are separate
measures. Passing the README consistency check verifies the generator's current
arithmetic agrees with the page; it does not independently certify that ledger.

The SDK exporter currently assigns `MATCH*` to some compiled-source addresses
with no audit row. That is an assumption, not verification. Downstream users
must not treat absent/stale audit evidence as a verified function or type fact.
Default mirroring of such exports is not a substitute for a reviewed proof.

## Current runtime and release boundaries

The frontend checkpoint combines recovered routines with authored adapters.
Modern subsystem policies, original-code replays, native loading/resource gates,
and numeric scheduling policies are useful bounded work. They do not establish
a complete engine, glyph/world rendering, full runtime ownership, or gameplay.
Default CTest cases and separately supplied differential corpora may exercise
different contracts; report both scopes explicitly.

Generated readable Lua and experimental disposable playtest packages likewise
have distinct contracts. Syntax, a `READABLE_REPORT` marker, mocked smoke output,
or an unverified disposable configuration does not establish reviewed release
eligibility or ordinary-save compatibility. Preserve pending diagnostics and
the exact sidecar/runtime boundaries.

Dated reports keep their original measurements and qualifications. A historical
PASS is not a fresh run against changed source. Follow the owning journal and
explicit current acceptance record, not an old pause/resume line copied into a
long history file.
