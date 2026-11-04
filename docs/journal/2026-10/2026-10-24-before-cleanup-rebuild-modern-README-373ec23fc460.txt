# Modern C++23 reconstruction layer

This directory is the human-facing implementation lane. It groups stable,
evidence-backed behavior by subsystem and may use C++23, cohesive classes,
namespaces, RAII, strong types, and modern tests.

It is not the retail byte-parity lane. Original-codegen work remains under
`rebuild/src/compiled/<address shard>/`, one function per VC7.1 translation
unit. Never replace a parity source with a modernized implementation or count
modern code as a retail match without the existing compile, behavior, and
oracle gates.

Create a subsystem here only after its signatures, layouts, call edges, and
representative behavior are stable. Each subsystem README should identify:

- the retail address/function set it models;
- proven layouts and remaining assumptions;
- parity and fixture tests used as its oracle;
- intentional API or ownership improvements;
- unsupported retail behavior.

See `docs/ARCHITECTURE.md` for promotion policy and
[ENGINE_ARCHITECTURE](../../docs/modern/ENGINE_ARCHITECTURE.md) for runtime ownership,
dependencies and the headless-session gates. Disable desktop probes and the graphics
viewer with `-DFABLE_BUILD_DESKTOP_TOOLS=OFF`; portable libraries remain available.
Python 3.10+ is required for the architecture gate. Configure and normal builds
check target dependencies and direct includes; `fable_architecture_check` runs it
alone. New libraries require an explicit policy in
`tools/modern/check_modern_boundaries.py`. See the architecture contract for scope
and limitations; `fable_architecture.Rejections` tests forbidden cases.

## Current modules

- `multiplayer/` — first x64 C++23 proof of concept: a safe, byte-compatible
  codec for individual cut-co-op `CGameEvent` records, with boundary and stream
  tests, bounded package codecs and retail event recording. Full gameplay remains
  evidence-gated work.
- `save/` — retail saves and profiles: the `FableSave!` container (zlib, retail load gates),
  every body section typed (HEADER, clock, PLAYER, ENTITIES cells, NPC names, REGIONS,
  QUESTS/FACTIONS text and boasts) and `Profile.bin`. Every game-written save (402) and profile
  (153) re-encodes byte-identically. LZO for the text blobs is an optional GPL download. See
  `save/README.md`.
- `codec/` — the tree's own LZO1X decoder (MIT), shared by `save/` and `texture/`.
- `texture/` — GBANK textures (descriptor, LZO1X mip 0, A8R8G8B8/DXT1/DXT3 to RGBA, PNG). It
  decodes 394/394 frontend and 6,319/6,324 world textures. `fable_title_view` presents the
  retail title logo in a 64-bit window through D3D12 or Vulkan (pixel-identical captures): the
  modern tree's first frame. See `texture/README.md`.
- `integration/` - explicit reconstructed-object to portable-behavior adapters.
  The generic thing-checksum bridge runs in the aggregate replay and matches the
  full retail corpus on MSVC/clang-cl. See `integration/README.md`.
- `simulation/` - bounded generic thing-checksum result calculation, verified against
  109,608 retail records across all 12 numeric modes. Uses caller-owned values;
  also owns persistent-ID index records with distinct slot/generation handles,
  matching 8,941 native index operations and 93,167 mapping observations;
  `ThingStore` owns initialized state and validates owner/slot/generation handles,
  matching native registration order and numeric corpora. `ThingInitialization`
  now owns generic parameter/OnCreate orchestration with delayed UID publication
  and valid nested component attachment. `ThingDefinitionConstruction` now
  traverses owned component-set data through the real Store/factory path, with
  captured replacement policy and final kill-lock assignment. `ThingActivation`
  connects both initial-activation phases to owned update/render membership,
  preserving callback order and physics-before-insertion timing. `ThingKill` now
  connects generic Kill and same-sweep queued disposal to actual owned memberships,
  components and UID cleanup. `ThingTick` adds normal/bullet-time component passes,
  stable-node pending removal and retention queries with explicit world-frame data.
  Decoded definitions, derived
  gameplay and a gameplay session remain open. `ThingUid` supplies recovered
  UID allocation, map-relative serialization remapping and single-pass collision
  repair through narrow map capabilities. `ThingUidMapTable` owns map identity
  metadata and sequence counters. Generic components now use unique ownership,
  validated handles and a session weak-removal queue; entity teardown releases
  them before UID/list removal. The adapter is in `integration/`. See
  `simulation/README.md`.
- `compat/` — types that keep retail (VC7.1) layouts under current compilers (`retail_bitset`).
- `platform/` — step-3 platform services: Raw Input mouse/keyboard, XInput pads, the world clock,
  and portable per-tick input recording/playback. See `platform/README.md`.
- `reconstructed/` — 50 reconstructed event/container/font/wire/main-game source units, a safe bridge
  between portable wire records and engine events, and bounded player/ownership replay tests.
  MSVC/clang-cl support the gameplay targets; Windows GCC also builds the wire/bridge targets.
  Build-only snapshots adapt declarations and fixtures. This is engine integration groundwork;
  see `reconstructed/README.md`.
- `bank/` — the BIGB `.big` bank container (TOCs, payload views, minimal-repack writer). All 17
  bank files in the install round-trip byte-identically, and resize/revert edits hold. See
  `bank/README.md`.


The [owned manager traversal](../../docs/modern/THING_WORLD_UPDATE_STORE.md) now passes 1,241 native cases,
2,448 calls and 9,926 observable events plus final states. All five builds pass
(50 MSVC/clang tests, 33 GCC, 30 desktop-free). The assembled lifecycle uses
actual manager-to-generic-tick delegation and final death sweep. Bullet policy
and derived gameplay remain explicit controlled boundaries.


The [portable full-update policy](../../docs/modern/BULLET_FULL_UPDATE_STORE.md) now matches all 30,840 native
records in four host rounding modes and drives the assembled lifecycle fixture.
All five builds pass (51 MSVC/clang, 34 GCC, 31 desktop-free tests). The broader
affected-entity predicate and real multiplier registration remain open.


The [owned affected-entity policy](../../docs/modern/BULLET_AFFECTED_STORE.md) now matches 3,597 native cases
and 32,040 comparable events plus final states. The assembled lifecycle uses
both recovered bullet policies. All five builds pass (52 MSVC/clang, 35 GCC,
32 desktop-free tests); component/player reference and script capabilities
remain explicit integration boundaries.


The [owned script-control predicate](../../docs/modern/THING_SCRIPT_CONTROL_STORE.md) now matches 1,288 native cases
and 2,574 queries, with protected delegation from the affected policy. All five
builds pass (53 MSVC/clang, 36 GCC, 33 desktop-free tests). Action execution and
component/player reference ownership remain separate integration milestones.


The [owned affected/script composition](../../docs/modern/BULLET_SCRIPT_COMPOSITION.md) matches 4,109 native cases,
9,834 calls and 42,573 comparable observations. All five builds pass
(54 MSVC/clang, 37 GCC, 34 desktop-free tests). Component/player reference
ownership remains the next integration boundary; full gameplay is still open.


The [owned entity-reference value](../../docs/modern/THING_REFERENCE_OWNERSHIP.md) validates targets against its borrowed
Store and expires safely across teardown and slot reuse. Five-build regression
passes (55 MSVC/clang, 38 GCC, 35 desktop-free tests). Component/player target
adapters remain open.


[Guarded component observations](../../docs/modern/THING_COMPONENT_ACCESS.md) now permit exact delegated reads from
affected/script policies while protecting owner and component lifetimes. All
five builds pass (56 MSVC/clang, 39 GCC, 36 desktop-free tests). Production
component/player target adapters remain open.


The [owned player target table](../../docs/modern/THING_PLAYER_TARGETS.md) matches 6,912 native cases and 13,824
selection/target pairs and feeds the bullet-policy fixture. All five builds pass
(57 MSVC/clang, 40 GCC, 37 desktop-free tests). Full player creation and
component71/75 target integration remain open.


[Owned carryable/particle target queries](../../docs/modern/THING_COMPONENT_TARGETS.md) now drive the combined replay with
real player/entity/component references: 4,109 cases and 41,884 comparable
observations match. Five builds pass (58 MSVC/clang, 41 GCC, 38 desktop-free).
Full carrying/attachment commands and lifecycle side effects remain open.
