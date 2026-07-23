# Lift harness — bar #2 (buildable, matching-decomp source)

*Stood up 2026-07-21. This is the FULL_DECOMP bar-#2 oracle: it proves a lifted
`.cpp` recompiles to the ORIGINAL retail machine code, byte-for-byte.*

## Status: PROVEN — scales from 1 function to a whole module
**Module #2 (2026-07-21): the entire `CMouseDX` accessor surface — 10/10 functions
byte-identical.** Getters, setters, a const-ref returner, and a 2-field struct copy
all recompiled to the exact retail bytes under VC7.1, including matched register
allocation on `GetPos` (edx-then-eax) and `__thiscall` ret-4 stack cleanup on the
setters. Run: `bash lift/scripts/build_module_diff.sh lift/proofs/src/cmouse_dx.cpp lift/proofs/oracles/cmouse_dx.expected.tsv`
→ `10/10 functions byte-identical`.

Module #1 (single function): `CTCPhysicsBase::GetAirResistance @0x00662030`:
```
lifted : 8b81e0000000d94078c3
retail : 8b81e0000000d94078c3
RESULT : *** MATCH (byte-identical .text) ***
```
Together these validate the full pipeline (Ghidra pseudo-C → clean C++ → VC7.1 →
matching bytes) AND that it generalizes across calling conventions, return kinds,
and multi-function modules — the decomp is now a gradeable, parallelizable grind.

### Key lift lesson (calling convention)
VC7.1 only emits true `__thiscall` (this in ECX, args on stack, `ret N`) for real
**member functions**. `__fastcall(self, edx)` (the single-function shortcut) matches
only 0-arg leaf getters; any method WITH parameters must be lifted as a member of a
class whose layout places the members at the right offsets. `cmouse_dx.cpp` does this
with a padded `unsigned char m_[...]` at offset 0 (non-virtual members → no vptr shift
→ raw offsets equal the retail `ecx+disp`).

## Pipeline
1. **Get the oracle bytes + decomp** for a target function:
   `analyzeHeadless ... -postScript DumpFuncBytesAndC.java 0x<addr>`
   (prints the decompiled C, the function length, and the exact retail bytes).
2. **Lift** the pseudo-C to clean, compilable C++ (`<name>.cpp`). For leaf functions
   with no engine deps, express member access via raw offset arithmetic (as the
   decomp does) and use `__fastcall(self, edx)` to emulate `__thiscall` (this in ECX).
3. **Compile + diff**: `powershell -File lift/scripts/build_and_diff.ps1 <name>.cpp "<retail hex>"`.
   A `*** MATCH ***` means the lift is byte-exact (matching grade). A DIFFER is either
   a functional-but-not-matching lift (compiler chose different encodings) or a bug.

## Toolchain
- **VC7.1 / MSVC 2003** portable at `D:\Tools\vc71` (`cl 13.10.3077` — Fable's exact
  compiler). Flags: `/O2 /Oy` (era-typical; no `/GS` — VC7.1 has no security cookie).
- **Win32/Direct3D SDK gate is proven:** Windows SDK `10.0.22621.0` x86 headers and
  import libraries compile, link, and run under VC7.1. Reproduce with
  `tools/ProbeVc71Sdk.ps1`; activate via `rebuild/vc71_sdk_env.ps1`.
- **objdump** (WinLibs mingw) extracts `.text` — the portable VC7.1 dumpbin can't
  disassemble (msdis140.dll absent) but section extraction via objdump works fine.

## Grading (per FULL_DECOMP.md)
- **Matching** — byte-identical `.text` (what GetAirResistance achieved). Reserve for
  hot/critical/simple functions where it's cheap to prove.
- **Functional** — recompiles to behaviorally-equivalent code (different byte encodings
  OK). The practical target for the bulk of the 49k functions.

## Scaling this
Each lift needs: the decomp (have it, 99.998% clean), the type layout (`fable_types.h`,
3,807 classes), the module grouping (`decomp_module_scaffold.tsv`, 3,433 class-modules),
and this oracle. The per-function lift is now a gradeable, parallelizable grind — the
`auto-re-agent` (Fable2) reverser/checker loop can drive it with this diff as the parity
gate, replacing its source-parity engine with the byte-diff here.

## Unattended terrain reversal queue

### Directory layout

- `config/` contains queue YAML; `state/` contains progress ledgers, PID/stop/
  completion markers, queue logs, and summaries. Retired/stale control files are
  retained under `state/archive/` rather than deleted.
- `reports/{primary,retry,wave2,wave3}/` contains durable generated code, checker
  reports, and round logs.
- `scripts/` contains PowerShell/Bash entry points, `bin/` contains command
  wrappers, and `proofs/{src,oracles,build}/` separates matching source, expected
  retail bytes, and disposable compiler output.
- Per-attempt transcripts live under `logs/<wave>/YYYY-MM-DD/`. The queue runners
  create these dated directories automatically. `scripts/organize_lift.ps1`
  sweeps only completed root-level `*.stdout.log` and `*.stderr.log` files; it
  defaults to a 30-minute safety window, detects active queue processes, never
  overwrites a collision, and supports `-WhatIf`.
- `.cache/` is replaceable decompiler cache data. Transcript housekeeping does
  not touch reports, state, proof sources, or cache entries.
- `scripts/migrate_wave3_layout.ps1` completes the final Wave 3 cutover only
  after the legacy PID is gone. The scheduled Wave 3 runner invokes it before
  each new batch, so no manual cleanup check is required.

- `scripts/run_re_agent_queue.ps1` processes the 16 terrain targets sequentially and records
  each completed four-round result, including honest `FAIL` results.
- `scripts/supervise_re_agent_queue.ps1` keeps that queue alive after a worker/PowerShell
  failure. After the primary pass, it launches `run_re_agent_failure_queue.ps1`
  exactly once for each primary `FAIL`, using the upgraded type-aware prompts and a
  separate retry ledger/report directory. A retry `FAIL` is terminal, so the loop is
  bounded rather than endless.
- Create the corresponding marker under `state/` (`re-agent-supervisor.stop`,
  `re-agent-queue.stop`, or `re-agent-retry-queue.stop`) to pause a loop.
- The Windows scheduled task `FableTLC Auto RE Queue Supervisor` starts the supervisor
  at user logon, so a reboot does not require a manual restart. Its failure-restart
  settings also recover the supervisor itself.
- Status is append-only in `state/re-agent-queue.log` and
  `state/re-agent-supervisor.log`; live PIDs are beside them in `state/`. At final
  completion it writes `state/re-agent-completion-summary.md`, creates
  `state/re-agent-queue.complete`, and shows a
  desktop notification.

## Legal note
This harness and any lifted source are original work + RE facts (shareable). It requires
the user's own `Fable.exe` for the oracle bytes; never redistribute the retail binary, the
game assets, or the debug PDBs. See the decomp-distribution note in the session HANDOFF.
