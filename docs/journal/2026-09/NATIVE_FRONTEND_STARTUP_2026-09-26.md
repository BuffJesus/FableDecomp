# Native frontend startup - 2026-09-26

The user's target is retail recreated through readable recovered C++, including
runtime loading from the installed retail containers. The visual checkpoint's
boundary GFMain and screen adapters do not satisfy it.

Added `play_seam.py --entry startup`: call recovered WinMain using the current
module handle, an empty command line, and normal show mode. It links only the
reconstruction closure plus explicit missing-function traps/zero-data placeholders;
no authored engine boundary or Play scaffold is supplied. This remains a diagnostic,
not a complete executable: static data/startup coverage must also be recovered.
Returning from WinMain is explicitly not reported as frontend success (the retail
single-instance mutex can cause an early return).

The new probe exposed a closure parser bug: C fastcall names such as @GFMain@12
were dropped from the unresolved list. The parser now recognizes both underscore
and at-sign C decoration. A regression test covers actual VC7.1 startup diagnostics.

Recovered MicroThread::SetStack at 009D86B0 (27 bytes, RELOCATION_MATCH). Evidence:
retail's six instructions, a fresh read-only Ghidra decompilation, and Ego_r's
matching function/signature in the bsim audit.
It writes the CDCDCDCD sentinel at stackTop-4 and stores current stack pointer,
base and size at 013CA8A0/013CA8A4/013CA89C. The candidate-linked fixture verifies
only that sentinel changes, all three globals, repeated calls, zero and high-bit sizes.
A changed sentinel mutation fails. No empty SetStack stand-in enters this probe.

Corrected the WinMain call declaration from unsigned int to retail unsigned long
(the same machine ABI, different C++ symbol); matching existing integration/test
stand-ins were corrected so their isolated builds remain valid.

Result: two genuine sources, five unresolved data symbols, and one missing function.
The probe runs WinMain -> SetStack and traps at @GFMain@12 (00402510). The recovered
GFMain body is the next primary target; stage2_engine_boundary.cpp is an authored
implementation and must not silently satisfy that dependency. The Play-only null
font manager is a later symptom, not evidence of a working startup.

Scratch: work/codex-native-startup. No native frontend window is claimed.

Fresh GFMain decompilation is saved in work/codex-native-startup/GFMain_00402510_decomp.c.
Its old automatic labels are unreliable; use the existing gfmain_calls.tsv address
ledger and actual call ABI when recovering the whole body, not guessed label names.

Validation: candidate RELOCATION_MATCH + required-candidate PASS; sentinel mutation
FAIL; linker-diagnostic regression test PASS. Full gates PASS: 123 boot leaves,
488 header dependents with zero regressions, component ABI, bootstrap references,
ring link, and full visual bootstrap. The latter verifies compatibility only; it
is not native frontend completion. Native startup still traps at GFMain.

## Shutdown handoff - 2026-09-26

User is shutting the computer down. Native implementation checkpoint: 74b2a78.
All work is saved; no native reconstruction build, probe, or game window remains
running from this session. No task depends on keeping the computer on.

Resume on land/boot-debake without switching the shared Lua checkout. Use a separate
Git index for native commits. Start with the native startup journal:
`docs/journal/2026-09/NATIVE_FRONTEND_STARTUP_2026-09-26.md`.

Next implementation: genuine retail GFMain at 00402510. Its fresh decompilation is
`work/codex-native-startup/GFMain_00402510_decomp.c`; cross-check the call addresses
against `rebuild/integration/gfmain_calls.tsv` and retail disassembly. Existing
GFMain phase adapters are reference material, not the implementation to link.
Reproduce the frontier with:
`python tools/decomp_pipeline/play_seam.py --entry startup --out work/codex-native-startup`.
Expected: WinMain -> recovered SetStack -> trap @GFMain@12 (exit 86).

Requirement: the visible frontend must run recovered retail C++ and load real
containers at runtime. Do not extend/relaunch the authored visual checkpoint as a
substitute. LIVE removal is committed and visually verified. The native frontend
is not yet visible; the later system CPUID and font startup gaps remain open.
Latest checks: all gates PASS, 123 boot leaves, 488 header dependents, zero
regressions, full bootstrap; SetStack parity/behavior and mutation checks passed.
