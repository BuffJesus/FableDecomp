# Reconstructed-engine integration

This module contains deliberate connections between reconstructed compatibility
objects and portable runtime behavior. `fable_thing_checksum_bridge` currently
builds with the MSVC/clang-cl reconstructed gameplay targets on Windows. Portable
simulation and format libraries do not depend on it.

## Generic thing-checksum adapter

`reconstructed_thing_checksum(thing, manager, mode)` reads typed class metadata,
definition index, signed player number and optional position/velocity values,
then calls `fable::simulation::thing_checksum`. It receives the manager explicitly
instead of consulting the original process-global current manager. It preserves
the local-type early return before reading the physics pointer.

The caller supplies valid borrowed reconstructed prefixes and a type-info vector
whose pointers describe one valid array (or an empty/null vector). References and
values are consumed only during the call. The adapter retains nothing, allocates
nothing, mutates nothing and owns no engine resources. Missing type metadata and
invalid numeric modes return distinct errors. It is not an arbitrary-pointer
validator. Physics values are copied by representation, preserving signaling-NaN
bits and the host FP environment.

The public integration header forward-declares the old classes. Their definitions
stay private; simulation sees only owned scalar/optional values. Build-only copied
headers name the measured `CThing` fields and add narrow access friends. Canonical
parity headers/sources remain unchanged, and immutable snapshot manifests record
every declaration patch.

## Layout evidence

Retail `CThing::GetChecksum` is `0x004C7DA0`, vtable slot 23. Its instruction operands
establish the retail fields below. Donor layouts establish member names/order;
retail STLport containers determine their actual x86 representation. Retail RTTI
at physics vtable `0x0125D2C4` confirms the primary chain `CTCPhysicsBase -> CTCBase
-> CBaseClassNonCopyable -> CBaseClass`, all at displacement zero.

| Field/size | Retail x86 | Typed modern x64 |
|---|---:|---:|
| CClass type byte | 0x10 | 0x20 |
| CThing physics pointer | 0x60 | 0x88 |
| CThing definition index | 0x8C | 0xB8 |
| CThing signed player number | 0x90 | 0xBC |
| CClassTypeInfo size / Local / Name | 0x14 / 0x09 / 0x10 | 0x20 / 0x0D / 0x18 |
| CTCBase size | 0x0C | 0x18 |
| Physics size / position / velocity | 0x40 / 0x0C / 0x28 | 0x58 / 0x18 / 0x34 |
| Partial CThing size | 0x98 | 0xC0 |

These are checked before fixture execution. The x64 component map is 32 bytes,
including comparator/dirty tail alignment; using only its vector size would put
the later thing fields eight bytes too early. Type-info allocator and physics
definition/flag tails are represented only to preserve layout and are not used.
Factory/type-name/physics allocation, destruction and full class behavior gain no
credit from these prefixes.

## Validation and replay

`fable_integration.ThingChecksum` covers literal native numeric observations,
local types with deliberately inaccessible physics pointers, missing/short metadata
and invalid modes. It checks unchanged object/type/physics storage and preserves
the host rounding mode and existing FP flags. The optional native-corpus command is:

```powershell
work/modern_build_x64/integration/fable_thing_checksum_bridge_tests.exe <oracle-proof>/corpus.bin results.bin
```

All 109,608 retail records match on MSVC x86/x64 and clang-cl x64, each under four
host rounding modes. Computed output streams exactly match the original retail
words. The prefixes use borrowed aligned storage; this is a field/behavior proof,
not constructed object lifetime evidence.

`MainGameAggregateReplay` now routes original slot-23 traversal through this adapter
and the portable numeric implementation. Its test-only helper supplies exact
kinematic/definition/player inputs for the existing expected words; the virtual
callback no longer returns a stored result. All 12,800 scenarios preserve event,
frame, world/checksum/client ordering, dead-entry filtering and wrapping sums:
240,020 callbacks, 12,180 replay thing queries, 7,000 direct thing queries, 25,600
direct client updates, and **19,180 actual adapter queries**. Inputs are checked
immediately around each adapter call so later stimuli cannot hide mutations.

The separate original 80-case traversal fixture retains its callback values to
isolate traversal, including sentinel mutation. The aggregate still uses a test
vtable shim and controlled world updates. It does not establish live entities,
derived-class checksum overrides, general physics, RNG, rendering, or a complete
headless game session.

Proof: `work/modern_thing_bridge_20261018/validation/summary.json`, archived complete
CTest/record-comparison logs and target graphs; RTTI evidence is
`work/modern_thing_bridge_20261018/physics_rtti.json`. See
[THING_CHECKSUM_NUMERICS](../../../docs/modern/THING_CHECKSUM_NUMERICS.md) and
[ENGINE_ARCHITECTURE](../../../docs/modern/ENGINE_ARCHITECTURE.md).
