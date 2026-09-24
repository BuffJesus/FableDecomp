# Graphics-bank ownership recovery — 2026-09-23

Continues the native-engine work in `D:\Documents\FableTLC-native-play`, branch
`wip/native-cgame-play`, from yesterday's [manager checkpoint](FRONTEND_EVENTS_AND_MANAGER_2026-09-22.md).

## Completed increment

Recovered three complete functions as readable VC7.1 C++ and linked them into
the existing manager-construction gate:

| Retail address | Function | Compiled / retail bytes | Acceptance |
| --- | --- | --- | --- |
| 00419108 | Counted-reference release and clear | 53 / 44 | Functional DIFFER |
| 00419134 | Counted-reference ShareData | 42 / 38 | Functional DIFFER |
| 0042A9B7 | CManager::SetGraphicsBank | 34 / 28 | Functional DIFFER |

Source uses `fable_ui_bank_ownership.h`, the existing manager layout and shared
`FableReferenceCount`. Retail instructions establish the calling convention and
ownership behavior; the original checkout's `ghidra_out/struct_layouts_egor.tsv`
independently supplies the 12-byte CCPPointerInfo offsets (RefCount +0,
DeleteFunc +4, Data +8) and eight-byte counted graphics-bank reference.
The donor's callback convention is not copied: retail passes its object in ECX.

Release decrements the control record, invokes its delete callback at zero,
reloads the reference's control-record pointer after the callback, frees that
record, and clears both reference words. ShareData compares control-record
identity, not data identity: an equal Info leaves Data unchanged, including
the null-Info case. Otherwise it releases the old owner before assigning and
incrementing the new one. SetGraphicsBank shares into manager +0x10 and then
releases its already-owned by-value argument. The factory output in the
constructor is consequently retained once, not leaked or destroyed early.

Compiler residues are reviewed and pinned by normalized instruction hashes in
the new gate: release uses longer zero stores and stack cleanup, ShareData has
different register scheduling plus the explicit unused EDX argument, and the
setter stages arguments in registers and clears that unused EDX argument.
These are not new byte-match claims; no generated coverage totals were changed.
The inherited global catalog has old propagated identities for the two shared
helpers; the dedicated native gate explicitly compiles the new sources and does
not rely on those old catalog entries.

## Validation

Run from the native worktree:

```powershell
python tools/decomp_pipeline/check_ui_bank_ownership.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

- Ownership: **974 retail differential cases pass**, covering null data/control
  records, same/different control records, same/different data pointers, live
  owner counts, repeated release/share/set calls, callback/free ordering, and
  two callback-replacement cases. Manager bytes outside the reference remain
  untouched. The oracle checks return and stack cleanup as well as traces.
- Construction: **324 connected retail comparisons pass** with the actual
  setter/share/release on both sides. The old BIND pointer-copy double is gone.
  Comparison includes all 208 manager bytes, container bytes, service ordering,
  bank control count, and a second cached singleton lookup. The unchanged
  baseline gate also passed before replacing its ownership double.
- `build_bootstrap.ps1` now includes the new ownership gate immediately before
  the connected manager-construction gate.

Reports and disassemblies are under `work/ui_bank_ownership_check/` and
`work/ui_manager_construction_check/`. The oracle SHA-256 is pinned to
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

Callbacks and deallocation are observed services, not the real allocator.
Callback-replacement tests deliberately retain fixture storage to inspect the
native pointer-reload contract; they do not prove arbitrary destruction or
reentrancy safe. Malformed pointers and reference-count overflow are outside
the tested contract. Factory, strings, system/display queries and allocation
remain controlled construction services. This is not a live asset-load test.

## Resume

Next: replace constructor string services 0099EBF0/0099EAE0 and investigate
graphics-bank factory 009F83D0 plus display services; then native frontend
creation/actions and persistent renderer integration. The SetGraphicsBank and
counted-reference recovery listed as next yesterday is complete.

No game or GUI launched, no presenter changes, and no full bootstrap/visual
parity claim. Earlier uncommitted work was preserved; this increment remains
uncommitted too. The separate script-recovery checkout was not edited.
