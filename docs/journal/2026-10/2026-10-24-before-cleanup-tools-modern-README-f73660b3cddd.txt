# Modernization preparation tools

`thing_registration_retail_probe.py` executes bounded original x86 registration
bodies in Unicorn with controlled allocation/free/notification callbacks. It also
audits checksum dispatch through RTTI for every manifest-listed vtable. It needs
the pinned retail image and optional Unicorn package; it never launches Fable.
See [registration evidence](../../docs/modern/THING_REGISTRATION.md) for the
command, measured ABIs, checks and remaining ownership work.
Its optional `--corpus` exports explicit operations and registration-order
snapshots for the owned [ThingStore](../../docs/modern/THING_STORE.md) comparison;
the original trace hashes remain unchanged when observations are enabled.

`thing_lifetime_retail_probe.py` extends that evidence to native empty-component
CThing construction/destruction and factory/container initialization/teardown.
It checks all constructor stores and untouched bytes, resource counts and full
tracked allocation release. See [lifetime evidence](../../docs/modern/THING_LIFETIME.md)
for the command and the component/UID/allocator paths still outside the proof.

`thing_uid_index_retail_probe.py` captures the real manager UID notifications and
tree operations with controlled allocation/free. It exports explicit operation
and mapping observations for the portable C++ `ThingIdIndex` comparison. See
[UID-index evidence](../../docs/modern/THING_UID_INDEX.md) for semantics and scope.

`thing_uid_allocation_retail_probe.py` executes original allocator/remapper/CreateUID
and map-counter helpers, controlling only the map-object lookup. It exports UID,
counter and capability-order observations for the portable `ThingUid` tests. See
[UID-allocation evidence](../../docs/modern/THING_UID_ALLOCATION.md), including the
native definition-schema trace that identifies the LevelLess flag.

`thing_uid_repair_retail_probe.py` executes original CheckAndFixUID and all its
callees without substituted native calls. Explicit map metadata and small index
trees exercise counter advancement, loaded-state flags and single-pass repair.
The exported corpus checks the portable implementation through its owned map UID
table. See [UID-repair evidence](../../docs/modern/THING_UID_REPAIR.md).

`thing_initialization_retail_probe.py` executes native ConstructFromParams and
its UID, registration, reference-management and generic component helpers with
controlled external services. It checks ordered boundary state, successful
attachment, duplicate rejection, stage dispatch and update-list membership.
See [initialization evidence](../../docs/modern/THING_INITIALIZATION.md) for the
controlled edges and the owned C++ lifecycle still to implement.

`thing_components_retail_probe.py` extends that reference through replacement,
pending owner-reference cleanup and prepared CThing/factory teardown. It exports
explicit JSON and numeric replay corpora for the owned component implementation.
See [component ownership evidence](../../docs/modern/THING_COMPONENTS.md).

`thing_creation_retail_probe.py` runs the original generic Create wrapper through
parameter initialization, OnCreate, native UID publication and full destruction.
It checks registered-but-unpublished state, copied component iteration and native
reentrant attachment with controlled external services. See
[creation-phase evidence](../../docs/modern/THING_CREATION.md) for scope and gates.

These tools inventory or stage work before the modernization fork gate. They do
not start the x64 game build, alter canonical parity headers or assign coverage.

## Platform identifiers in headers

`header_surface.py` scans selected gameplay-facing roots and records local include
routes to legacy Direct3D, DirectInput, OpenAL and DirectShow identifiers. Roots
and backend boundary labels are explicit inputs. Comments and string literals
are excluded; all textual preprocessor branches are considered. External SDK
headers, macro/forced includes and compiler-specific lookup are unmodeled.

```powershell
python -B tools/modern/header_surface.py --root-header fable_game.h --root-header fable_cgame.h --root-header fable_iengine.h --root-header fable_input_manager.h --root-header fable_nui_manager.h --boundary-header fable_d3d9.h --boundary-header fable_d3d9_present.h --boundary-header fable_d3d9_shader.h --boundary-header fable_d3d9_shader_create.h --boundary-header fable_d3d9_query.h --boundary-header fable_d3dx9_texture.h --json-out docs/modern/HEADER_PLATFORM_SURFACE.json --markdown-out docs/modern/HEADER_PLATFORM_SURFACE.md
python -B -m unittest tools.modern.test_header_surface
```

The current [report](../../docs/modern/HEADER_PLATFORM_SURFACE.md) covers five
selected roots across 1,669 headers. Its counts are headers/routes with references,
not unique types or platform calls. A transitive dependency does not prove a
public API exposes every identifier. Boundary labels never suppress findings.

## Staged gameplay-header seam

`prepare_game_header_seam.py` copies hash-bound source headers into a new exclusive
stage and proposes replacing the full display backend include in `fable_game.h`
with a display-manager forward declaration. It also generates a worklist of
direct literal `.cpp` consumers. It does not automatically change their includes.

The first proposal's standalone syntax probe found that `AVISrcBox` and
`AVIDstBox` require the existing complete `C2DBoxI` geometry definition.
`amend_game_header_seam_geometry.py` preserves that failure and makes a separate
stage with the explicit plain geometry include.

`check_game_header_seam_syntax.py` runs `/Zs` probes only: original SDK visibility,
staged self-containment and a negative staged SDK visibility control. Header
declarations and inline definitions are parsed; engine implementation units are
not recompiled and no provider object is emitted. Compiler output, observed
includes, input guards and failures are retained in the stage.

Current results and exact stage hashes are in
[the seam journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-platform-seam.md).
The later [direct-caller impact journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-consumer-impact.md)
records an important correction: the generated integer-box declaration conflicted
with the original display geometry. Amendment2 imports the original
`fable_display_geometry.h` instead. Both earlier failures and audits remain.

`audit_game_header_seam_consumers.py` checks the 123 direct callers with recorded
native flag vectors using `/Zs`; InitLUA is explicitly excluded. Amendment2 passes
119 unchanged callers. `check_game_header_explicit_display_consumers.py` stages
one explicit backend include in each of the four implementations actually needing
display/render services; all four checks pass. Passing unchanged callers are not
rerun. Source copies and an unapplied five-file patch remain local.

Full original-profile consumer impact, field-layout, code generation and behavior
checks still precede promotion. No engine object is emitted by these syntax tools.

## Layout fingerprints

[Layout results](../../docs/journal/2026-10/2026-10-14-gameplay-header-layout-impact.md)
record 479 matching compiler-emitted constants for seventeen game classes and
their 135 explicit fields, plus both rectangle types. Local friend overlays grant
access without changing fields/access groups. The abstract base alignment uses a
qualified same-size wrapper rather than a direct query.

The probe compiler also emits a 48-byte array-construction helper. Earlier
code-free/no-reference assertions and their failures remain preserved. The final
`check_game_header_layout_retained_baseline_amended1.py` reuses the original probe,
compiles the staged probe, compares the constant tables and full helper bytes,
and records its exact FPO debug reference separately from runtime references.
It provides no engine code-generation, behavior or full ABI acceptance.

## Actual implementation compiler output

`audit_game_header_codegen.py` builds fresh fixed-profile original/staged pairs
for the 123 included callers and retains every object/output. Its ordered
nondebug comparison passes 118 pairs and preserves five differences. It does not
normalize symbols or alter first expectations/results.

`inspect_game_header_codegen_differences.py` records bounded factual witnesses for
the five exact pairs: two three-contribution cycles and three local-label renames
at identical offsets. No executable payload changes were found in those saved
projections. They remain unaccepted differences until required full metadata and
behavior/profile checks close. See the
[code-generation journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-codegen-impact.md).

`audit_game_header_coff_ownership.py` inspects all 246 actual objects and compares
runtime symbol storage/type/value/sections, weak fallback targets and nondebug
section/COMDAT auxiliary metadata. All 123 pairs match using only the existing
five bounded witnesses. It does not rerun compilers or rewrite first results.

`bind_staged_frontend_behavior_evidence.py` binds the full staged 477-byte frontend
draw body and relocations to the exact retained body used by the prior connected
test. It supplies scoped object reuse evidence, not a new runtime execution or an
expanded behavior domain. See the
[ownership journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-ownership-impact.md).

## Indirect and test/runtime consumers

`census_game_header_transitive_consumers.py` finds static literal routes through
four additional headers across six explicit repository roots. The current census
contains 275 sources, including fourteen new implementation callers and 137
test/runtime or other sources. Macro/forced/outside-root closure is not inferred.

`check_game_header_transitive_codegen.py` records fourteen new fixed-profile
compiler pairs, all with identical ordered nondebug sections.
`check_game_header_test_runtime_syntax.py` checks 136 uncapped sources under an
explicitly qualified common syntax profile: 126 pass both sides, three require
display includes and seven fail with the original headers. The InitLUA test stays
capped. `check_game_header_test_display_includes.py` stages and verifies only the
three actual include changes, leaving seven original failures preserved.
See the [transitive impact journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-transitive-impact.md).

`prepare_baseline_fixture_syntax_fixes.py` prepares local fixes for two duplicate
empty destructor definitions and one invalid specialization marker, preserving
logs/assertions. `prepare_primitive_fixture_oracle_headers.py` prepares exact
private retail/alias headers from old gate specifications without invoking their
producer entrypoints or compiling providers. `check_prepared_primitive_fixture_headers.py`
checks those specifications against the old AST and tests syntax readiness.
Six original/staged checks pass for each group. The text-bank fixture's first
staged-only pass remains qualified. See the
[baseline-readiness journal](../../docs/journal/2026-10/2026-10-14-baseline-fixture-readiness.md).

`check_game_header_fixture_links.py` builds four affected original/staged fixture
pairs with retained provider objects: eight links pass without provider recompilation.
`review_game_header_fixture_link_ownership.py` verifies selected engine ownership
and complete relocated bodies in all eight linked PEs. `run_game_header_next_present_hosts.py`
runs only the reviewed next-present-time pair; both six-case mock fixtures pass.
`run_game_header_corrected_fixture_hosts.py` records bounded source/mock review
and runs the remaining three pairs; all six hosts pass their existing assertions.
See the [corrected fixture runtime journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-corrected-fixture-runtime.md).
These are qualified x86 header-seam checks,
not an x64 build or retail differential acceptance. See the
[fixture-link journal](../../docs/journal/2026-10/2026-10-14-gameplay-header-fixture-link-impact.md).

Use a fresh stage directory for any new attempt. Do not regenerate or edit a
previous stage or expectations map. The copied source headers contain no retail
binary/assets and remain local under `work/`.
