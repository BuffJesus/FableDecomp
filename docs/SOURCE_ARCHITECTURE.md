# Source architecture and modernization

The reconstruction has three deliberately separate layers. They serve different
purposes and must not be collapsed into one directory.

| Layer | Location | Contract |
|---|---|---|
| Agent intake | `lift/reports/<wave>/code/<aa>/<bb>/` | Generated evidence; never counted as reconstructed merely because its structural checker passed |
| Retail-parity source | `rebuild/src/compiled/<aa>/<bb>/` and `rebuild/tests/<aa>/<bb>/` | One function per translation unit, compiled with VC7.1, behavior-gated, and compared with retail `.text` |
| Modern source | `rebuild/modern/<subsystem>/` | Human-facing C++23 modules built from proven behavior and layouts; not used to claim retail byte parity |

`<aa>/<bb>` are the first two byte pairs of the eight-digit retail address. For
example, `0x0089E710` lives under `00/89/`. Generated build and verification
products add an address leaf (`00/89/0089e710/`) so every function's object,
executable, and logs stay together.

## Why the parity layer has many small files

One function per translation unit is intentional in the retail-parity layer.
VC7.1 code generation depends on translation-unit context, optimization, inline
visibility, and COMDAT behavior. Isolating a function lets the pipeline compare
its object with a single retail oracle without unrelated compiler decisions
moving the target. Combining thousands of parity functions into large source
files would make them look tidier while making byte matching less reproducible.

Human organization comes from metadata and the modern layer instead:

- `rebuild/ARTIFACT_INDEX.tsv` maps an address and module to its generated source,
  curated source, test, snapshot, and local build directory.
- `rebuild/build_candidates.ps1` is the authoritative behavior-test catalog.
- subsystem headers in `rebuild/include/` centralize layouts, calling conventions,
  imports, and stable names instead of repeating guesses in each source file;
- `rebuild/modern/` groups proven behavior into ordinary subsystem-sized C++23
  files with namespaces, classes, ownership types, and descriptive APIs.

## Making functions human-readable

Readability changes should move through evidence, not erase it:

1. Correct the decorated signature, owner type, calling convention, and address in
   the analysis exports or correction tables.
2. Give the curated function a stable `Class_Method_address.cpp` name and use
   shared declarations from `rebuild/include/`.
3. Replace unexplained offsets with named fields only after a layout fact is
   proven. Keep an address/offset comment where the relationship matters.
4. Add a focused behavior test that describes intent rather than implementation.
5. Build a cohesive C++23 subsystem facade after the ABI, layout, call graph, and
   representative behavior are stable.

Do not perform style refactors inside a byte-matching function and assume they
are harmless. Run behavior and retail comparison after every parity-layer
change. A nicer spelling can change VC7.1 register allocation or scheduling.

## When to begin C++23

The directory and rules can start now; wholesale conversion should not. The
original-codegen lane must remain VC7.1-compatible for the life of the parity
effort. Start a C++23 implementation subsystem-by-subsystem when all of these are
true:

- owner and calling convention are stable;
- relevant class sizes/field offsets are proven;
- the important call edges have real names and signatures;
- behavior tests cover normal, boundary, and failure paths;
- enough retail-parity functions exist to serve as a differential oracle.

The co-op event/package codecs are a strong early candidate because their wire
format and sequence behavior are becoming bounded. Small terrain/theme access
modules are another. Large world, AI, or renderer classes should wait until
their ownership and lifetime rules are clearer.

Modern code should use C++23 where it improves the model—`std::span` for buffers,
strong enums, RAII ownership, `std::expected`-style error flow, ranges, and
explicit serialization—not as a mechanical syntax translation. Tests must
cross-check the modern behavior against the retail-parity layer or captured
retail fixtures.

## Workflow safeguards

The unattended workflow enforces the lessons from earlier runs:

- queue runners organize each completed target immediately, using its address and
  start timestamp so one target cannot claim another target's reports;
- new transcripts are written directly to their function leaf and the
  decompiler cache is selected by address shard before the worker starts;
- all candidate, curated, test, and fingerprint discovery is recursive;
- the refresh waits for queue ownership of Ghidra, then runs the organizer before
  fingerprinting and again after report generation;
- migrations refuse to run beside an active worker unless the lock-owning refresh
  explicitly invokes them;
- collisions are preserved and fail the organizing step for human resolution;
- the candidate gate records semantic quarantine flags (including end-sentinel
  dereferences and unrelated address reinterpret calls) independently of the
  agent's structural verdict;
- candidate snapshots and compiler products are reproducible local data and are
  not tracked in Git;
- README metrics are synchronized from canonical coverage/parity reports during
  every successful refresh;
- Git is updated only at reviewed checkpoints. A structural agent PASS is still
  uncurated intake, never an automatic promotion.

Run `python tools/organize_decomp_artifacts.py --root .` to preview drift, add
`--apply` to repair it while queues are stopped, or use
`tools/organize_workspace.ps1 -WhatIf` for the combined workspace preview.
