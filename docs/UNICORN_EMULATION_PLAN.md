# Unicorn-assisted x86 behavior oracle

*Assessment date: 2026-07-27. Status: recommended as an optional, local-only
reverse-engineering pilot; not yet implemented and not part of the canonical
build or parity totals.*

## Decision

[Unicorn](https://github.com/unicorn-engine/unicorn) is useful to this project,
but as a focused **function-level x86 execution and instrumentation layer**.
It should not become the game runtime, the renderer, or a replacement for the
VC7.1 byte-parity gate.

The first pilot should execute authoritative retail function bytes in a
controlled 32-bit x86 address space, provide explicit fixture memory and call
stubs, and emit deterministic JSON describing:

- final registers and stack delta;
- basic blocks visited;
- memory ranges read and written;
- external/internal calls reached;
- the first unmapped/protected access or unsupported instruction;
- bounded output-memory snapshots selected by the fixture.

That evidence can strengthen behavior tests, reveal ABI/structure contracts,
and explain promising `DIFFER` rows. A Unicorn result alone must never promote
a function or increase the public reconstruction percentage.

## Why it fits Fable TLC

The retail executable is 32-bit x86 at image base `0x00400000`. Unicorn exposes
an explicit `UC_ARCH_X86` + `UC_MODE_32` mode, page-backed memory, register
control, execution limits, context save/restore, and code/block/memory hooks.
Version 2.1.4 also guarantees a valid program counter when emulation stops,
which makes fault reports and bounded traces substantially more reliable.

The project already has most of the inputs a function emulator needs:

- authoritative address/length/byte rows in
  `rebuild/oracles/auto-re-candidates.tsv`;
- a checked retail PE lookup path in
  `tools/decomp_pipeline/pe_oracle.py`;
- recovered prototypes, calling conventions, symbols, and structure overlays;
- address-sharded behavior tests and candidate sources;
- retail-vs-object parity reports with relocation metadata.

A mechanical screen of the current parity ledger finds:

- **2,390** `MATCH` functions from 2 through 64 bytes with zero recorded COFF
  relocations. These are a large validation pool, not a claim that every row is
  safe to emulate without fixtures.
- **174** `DIFFER` functions of at most 128 retail bytes. These are a useful
  second-stage pool for comparing branch, stack, register, and memory behavior
  after the harness is proven on known matches.

## Highest-value uses

### 1. Generate behavior evidence for leaf functions

Map the retail bytes at their original virtual address, construct a synthetic
stack/heap/object fixture, execute to a sentinel return address, and record
observable outputs. This is especially suitable for:

- field getters/setters and bit tests;
- small arithmetic, clamp, colour, vector, and coordinate helpers;
- short container/list/tree operations with bounded fixtures;
- empty functions, thunks, and `ret N` ABI leaves;
- pure frontend geometry and state-selection helpers.

The existing C++ behavior test remains the durable gate. Unicorn supplies the
retail-side expected values instead of relying only on a hand-written reading
of the decompilation.

### 2. Probe ABI and hidden state

Seed `ECX`, `EDX`, the stack, and candidate object memory with distinct marker
values. Repeated runs can expose:

- `thiscall`, `fastcall`, and caller/callee stack cleanup;
- hidden structure-return pointers;
- returned register and preserved-register behavior;
- exact object offsets read or written;
- null, sentinel, and boundary branches.

This can directly improve the prototype/calling-convention override ledgers.
It is evidence, not an automatic type declaration: aliases and coincidental
marker reads still require static cross-checking.

### 3. Explain source-level `DIFFER` rows

For a small same-purpose retail/candidate pair, compare normalized:

- block/branch path;
- return value and stack delta;
- selected memory writes;
- stubbed-call order and arguments.

If behavior agrees but bytes differ, the row remains `DIFFER`; it may be a
compiler register-allocation or scheduling residue. If behavior differs, the
trace gives the authoring/refine loop a concrete semantic defect to fix.

Candidate-object execution is a later phase. COFF relocations must first be
applied correctly or the candidate must be loaded from a deliberately linked
test image. Zeroing or ignoring relocation operands is acceptable for byte
scoring, but not for execution.

### 4. Recover CPU-side rendering and particle contracts

Unicorn cannot render D3D9 pixels or execute shader-model bytecode as a GPU.
It can, however, execute isolated CPU helpers that:

- build billboard/crossed-sprite vertices;
- calculate camera-facing or x87 transform values;
- choose blend, depth, cull, texture-stage, or shader constants;
- walk particle arrays and generate batched draw inputs;
- sample or interpolate RSA/environment colour data before an API call.

D3D and engine callees would be registered stubs. Their ordered arguments and
memory inputs become the output. This is directly useful to the particle,
lighting, sky, and shadow viewer lane while keeping GPU output validation in
the existing shader dumps and screenshot/image-diff tools.

### 5. Bounded differential fuzzing

After a deterministic fixture passes, vary only declared inputs and compare
retail-emulated outputs with the native reconstructed helper. Preserve each
failure as a small JSON fixture and generated C++ regression case.

Fuzzing must have fixed seeds, per-run instruction/time limits, bounded mapped
memory, and a declared comparison mask. Uninitialized flags, padding, allocator
addresses, and unspecified floating-point residue must not become accidental
contracts.

## What Unicorn does not solve

- It does not decompile code or produce byte-identical C++.
- It does not replace `compare_candidate_objects.py`; retail byte comparison
  remains the authoritative parity measurement.
- It is a pure CPU emulator, not a Windows process emulator. Win32, DirectX,
  DirectShow, file I/O, TLS, SEH, threads, timers, allocators, and engine
  services must be mapped or stubbed deliberately.
- It is not a practical route to whole-game boot, live video, D3D9 presentation,
  audio, or complete scene rendering.
- Heavy global state, indirect callbacks, self-modification, complex exception
  paths, and system-facing code can cost more to model than a focused native
  fixture.
- x87/MMX/SSE instruction families used by the game must be validated with
  known-answer fixtures before their results are trusted. CPU support does not
  guarantee that our synthetic floating-point environment matches retail
  Windows defaults.

Unicorn's own FAQ warns that per-instruction and per-memory-access hooks are
expensive. Default batch runs should therefore use block hooks and enable
instruction/memory detail only for a single diagnostic case.

## Proposed tool boundary

Implement an optional `tools/unicorn_oracle.py` with no import from canonical
build scripts. Suggested command shape:

```powershell
py -3.13 tools/unicorn_oracle.py `
  --address 0x00662020 `
  --fixture work/unicorn/fixtures/00662020.json `
  --output work/unicorn/results/00662020.json
```

Inputs:

1. address and authoritative bytes from the checked-in oracle ledger;
2. prototype/calling-convention metadata;
3. a fixture describing registers, stack arguments, mapped memory, pointer
   relations, output ranges, and registered call stubs;
4. maximum instructions, timeout, and permitted code/data ranges.

Execution model:

1. Open `UC_ARCH_X86` / `UC_MODE_32`.
2. Map code at its original retail virtual address plus separate stack, heap,
   globals, stub, and return-sentinel pages.
3. Initialize all compared registers and bytes explicitly.
4. Use mapped `ret` stubs for known call targets. A stub records arguments,
   writes declared side effects/return registers, and returns through the
   emulated stack.
5. Stop at the sentinel return, instruction limit, timeout, unexpected target,
   invalid instruction, or unmapped/protected access.
6. Serialize canonical lowercase hex addresses and sorted memory ranges so
   results are stable and diffable.

Outcomes must be one of:

- `PASS`: reached the sentinel and satisfied the fixture contract;
- `MISMATCH`: completed but a declared output differed;
- `UNSUPPORTED`: required an unmodeled call, instruction, segment, exception,
  or environment feature;
- `FAULT`: violated a mapped-memory or execution-safety invariant;
- `TIMEOUT`: exceeded the explicit bound.

`UNSUPPORTED` is not a failure and must not be silently converted to `PASS`.

## Pilot order and acceptance

### Phase U0 — harness proof

1. Pin Unicorn **2.1.4** in a separate Python 3.13 virtual environment.
2. Start with five known exact leaves:
   - one `ret N` no-op;
   - one `this` field getter;
   - one field setter;
   - one boolean/bit test;
   - one small integer or float helper.
3. Add tests for stack cleanup, mapped-memory rejection, instruction limit,
   deterministic output, and explicit unsupported calls.
4. Cross-check each result against its existing C++ behavior test.

Acceptance: repeated runs produce byte-identical JSON, all five known contracts
agree, deliberate bad fixtures fail, and no retail asset or executable byte is
written into the repository.

### Phase U1 — trace-assisted residue diagnosis

1. Select ten same-length, sub-128-byte `DIFFER` rows.
2. Prefer self-contained functions with proven boundaries and no SEH/TLS.
3. Record whether each mismatch is semantic, ABI/environmental, or
   source-spelling-only.
4. Feed only semantic/ABI findings back into the existing author/refine loop.

Acceptance: at least one trace gives a reproducible correction or the pilot is
stopped as lower value than native fixture work.

### Phase U2 — particle/frontend CPU helpers

1. Stub D3D/engine calls and trace one frontend geometry helper.
2. Trace one particle billboard/constant/state helper.
3. Export call arguments and output buffers for viewer golden tests.

Acceptance: the trace replaces a guessed constant or transform with a proven
retail contract. It is not accepted merely for reproducing known static
disassembly.

## Dependency, version, and legal boundary

- Latest release checked during this assessment: **2.1.4**, published
  2025-09-09. The upstream `master` branch was still active in 2026.
- The local default interpreter is Python 3.14.4, while the current PyPI
  classifiers list Python through 3.13. Use a separate 3.13 environment for
  the pilot or explicitly prove the 3.14 wheel before documenting support.
- Do not vendor Unicorn source, DLLs, wheels, or generated retail memory
  snapshots into this repository.
- Do not link Unicorn into the reconstructed game executables.
- Upstream licensing metadata is inconsistent at a glance: the repository
  README describes GPLv2, the public C API header carries an LGPL2 notice, and
  PyPI metadata labels the package BSD while its description again says GPLv2.
  Treat redistribution or linkage as requiring a deliberate license review.
  A user-installed, optional local analysis process keeps the initial pilot
  cleanly separated.

## Authoritative upstream references

- [Unicorn repository and feature summary](https://github.com/unicorn-engine/unicorn)
- [2.1.4 release notes and PC synchronization guarantee](https://github.com/unicorn-engine/unicorn/releases/tag/2.1.4)
- [Public C API and x86-32 mode definitions](https://github.com/unicorn-engine/unicorn/blob/master/include/unicorn/unicorn.h)
- [Official FAQ: pure-CPU boundary, instrumentation cost, faults, and cache behavior](https://github.com/unicorn-engine/unicorn/blob/master/docs/FAQ.md)
- [Official Python package](https://pypi.org/project/unicorn/)
