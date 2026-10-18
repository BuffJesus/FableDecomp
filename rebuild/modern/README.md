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

See `docs/ARCHITECTURE.md` for the promotion and modernization policy.

## Current modules

- `multiplayer/` — first x64 C++23 proof of concept: a safe, byte-compatible
  codec for individual cut-co-op `CGameEvent` records, with boundary and stream
  tests. Package framing and gameplay dispatch remain future evidence-gated
  work.
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
- `compat/` — types that keep retail (VC7.1) layouts under current compilers (`retail_bitset`).
- `platform/` — step-3 platform services; first: mouse input via Raw Input with a per-tick
  accumulator (no 256-record limit). See `platform/README.md`.
- `bank/` — the BIGB `.big` bank container (TOCs, payload views, minimal-repack writer). All 17
  bank files in the install round-trip byte-identically, and resize/revert edits hold. See
  `bank/README.md`.
