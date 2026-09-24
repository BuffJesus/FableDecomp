# Native bank paths and string storage - 2026-09-23

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [archive registry lookup](FRONTEND_BANK_REGISTRY_2026-09-23.md).

Eighteen readable implementations recovered or upgraded: eleven wide-string
storage/lifetime/concatenation bodies, five narrow append/copy-on-write/concatenation
bodies, the character getter and complete registry path lookup. Existing wide
assembly recoveries in the original checkout were inspected, not copied as assembly.
All eighteen native implementations are functional DIFFER; no new byte-match claim.

## Compiler evidence

Sizes are individual bodies; small wrappers call other recovered bodies.

| Address | Compiled / retail bytes |
| --- | --- |
| 0099B4D0 | 59 / 62 |
| 0099B510 | 14 / 68 |
| 0099B3C0 | 83 / 121 |
| 0099B560 | 46 / 128 |
| 0099B6B0 | 54 / 104 |
| 0099B720 | 19 / 66 |
| 0099B7D0 | 49 / 47 |
| 0099B8D0 | 103 / 104 |
| 0099BE70 | 64 / 177 |
| 0099C670 | 105 / 97 |
| 0099C7E0 | 357 / 305 |
| 009A7CA0 | 159 / 144 |
| 0099E4C0 | 15 / 15 |
| 0099EAF0 | 46 / 78 |
| 009A01C0 | 211 / 316 |
| 009A04E0 | 111 / 107 |
| 0099F100 | 92 / 122 |
| 0099F600 | 64 / 130 |

## Behavior recovered

Path lookup 009A7CA0 resolves one alias, retains the resulting narrow name, searches
the path map and concatenates registry BasePath with the node's wide path at +0x14.
It destroys the retained name on success and returns the caller's hidden result.
No separator is inserted or normalized. Both empty and nonempty base/path strings
are valid concatenation inputs, and UTF-16 code units are preserved.

Wide storage is **begin pointer / end pointer / allocation-end pointer / owners**,
16 bytes total. `CWideStringData::unknown04` and `unknown08` in the shared legacy
header hold addresses, not lengths/capacities. A new view header documents this
without changing the existing layout. Wide buffers use 00BFEA0E / 00BFEA14;
narrow buffers use 00BFEB22 / 00BFEB1C. Both string-record families use
00BFEA1A / 00BFE9BC. Do not interchange the buffer allocator families.

Wide append preserves capacity when possible; otherwise capacity becomes
old length + max(old length, appended length) + 1 UTF-16 units. Copy-on-write
clones the NUL-terminated prefix, even when an explicit-length source stored
additional code units after an embedded NUL. Null-left append shares the right
record instead of allocating. Failed record allocation during MakeUnique releases
the old share and leaves null storage; no invented recovery in subsequent append.

Narrow reserve supports exact four-byte alignment and power-of-two growth selected
by flags0C bit 0, preserving capacity bit 31. Append maintains the terminating NUL
and zeroes alignment padding. The expanded narrow-string gate observes reuse,
growth, allocation failures, shared storage, flags and guards.

## Missing bank paths

The missing-path branch really constructs `<resolved name> bank not found!`, obtains
its character pointer, then calls the C++ exception runtime. It does **not** return
an empty path or false. ThrowInfo **013692F8** points to two catchable types:
`CGenericException` and its `CExceptionBase`, each with size 1 and no destructor.
The diagnostic pointer is not an exception payload. The copied byte in retail is
empty-class padding; native uses zero padding, and comparisons deliberately do
not require that unspecified byte to match.

There is no string cleanup before this throw and no local EH registration in the
retail body. The native explicit views preserve the observed retained-name and
diagnostic lifetimes. The gate stops retail at 00BFEB84 and uses a native longjmp
boundary: it verifies the requested throw type, memory and counts, **not real C++
stack unwinding or production exception bindings**. Length-error service 0099C550
also remains external; gigantic/invalid-range exception execution is not covered.

## Verification and integration

```powershell
python tools/decomp_pipeline/check_ui_wide_strings.py
python tools/decomp_pipeline/check_ui_bank_path.py
python tools/decomp_pipeline/check_ui_strings.py
python tools/decomp_pipeline/check_ui_bank_open.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

All pass: **720 wide-string**, **384 path**, **324 expanded narrow-string**,
**2,048 connected bank-opening**, and **324 connected manager** cases. The first
two families add **1,104 cases**; fourteen saved families now total **14,249**.
That total combines saved reports, not a fresh run of every older family.

Wide tests compare every allocated record/buffer and guard bytes after construction,
explicit ranges, uniqueness, appending twice, self-append, concatenation, assignment
and destruction. Inputs include null/empty, high UTF-16 units, embedded NUL, poison
patterns and record-allocation failures. Path tests cover map misses/hits, alias
targets including null/empty, real wide allocation and real missing-name narrow
concatenation; they also validate null/non-null character getter identity.

Opening now links real path lookup and real wide lifetime rather than their prior
doubles. Its fixture supplies a registered filename with a null base; file GetPath
uses real wide copying, and final path owners are compared. Manager startup supplies
two actual path nodes (GBANK_FRONT_END / GBANK_MAIN), with empty wide paths and a
controlled false-returning OpenPath service. The empty registered-file list still
exercises real archive misses. These fixtures prove connection and failure-path
behavior, not successful disk loading. The independent path gate covers allocating
concatenation and the missing-path branch.

Bootstrap now includes both new gates. Function identity overrides updated;
Python compile checks and PowerShell bootstrap parsing pass. Work remains saved
uncommitted; full bootstrap and live GPU/game/presenter launches were not run.
The original checkout and its prior recoveries remain untouched.

Reports/disassemblies are in `work/ui_wide_strings_check/`, `work/ui_bank_path_check/`,
`work/ui_strings_check/`, `work/ui_bank_open_check/`, and
`work/ui_manager_construction_check/`. Retail oracle SHA-256 remains
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Next

Recover archive entry decoding **009CFBC0**, bank finalization and real threaded
file Open **0098E1E0**, then connect frontend Run/actions and persistent rendering.
Production bindings must unify registry receiver **013CA79C** and its mode byte
**013CA7B0 = registry +0x14**, and provide native exception/length-error runtime
services. The prior D3D DEVICELOST result remains unresolved. This is not yet a
playable standalone game.
