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

See `docs/SOURCE_ARCHITECTURE.md` for the promotion and modernization policy.
