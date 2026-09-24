# Native construction services marathon — 2026-09-23

Worktree: `D:\Documents\FableTLC-native-play`, `wip/native-cgame-play`.
User requested sustained continuation and explicitly accepted reasonable
instruction differences when behavior is correct. This continues the
[ownership recovery](FRONTEND_BANK_OWNERSHIP_2026-09-23.md).

## Checkpoint: 31 complete functions connected

| Area | Retail addresses |
| --- | --- |
| CCharString construction, destruction, unassign, data allocation | 0099EBF0, 0099EAE0, 0099E9B0, 0099EA60 |
| Basic string initialization and byte assignment | 009A0590, 009A0300 |
| Empty/copy string construction and assignment | 0099E4B0, 0099EC30, 0099EFB0 |
| Six texture-format selectors | 009BE830, 009BE870, 009BE8B0, 009BE6C0, 009BE610, 009BE590 |
| Pixel-format lookup, render-target dimensions, system getter | 009E3830, 009BEDC0, 009A4EC0 |
| Graphics-bank factory, signed string comparison, deletion and progress dispatch | 009F83D0, 00411570, 00419036, 009E9F40 |
| Graphics-bank constructor/initializer, resource bank/base, texture-manager ownership and bank policy | 009FEA20, 009FD4E0, 009FC5F0, 0099A2F0, 00A002E0, 009FFD20, 009D5230 |
| Texture manager and resource-list constructors | 00A6A360, 009FC570 |

The singleton -> manager constructor gate now links all these functions, the
previous bank reference ownership, and yesterday's observable/maps/configuration
code. The string, format-query, global-system, dimension and graphics-bank
factory doubles have been removed, together with bank construction/initialization
and texture-manager/pool construction. Remaining controlled services are allocation,
D3D9 capability results, bank-file base construction, texture/surface operations
and virtual bank Open.
This is deeper native initialization, not yet disk/GPU asset loading.

## ABI correction exposed by connection

Yesterday's constructor fixture used `ret 4` for CCharString's text constructor,
which actually consumes text and length (`ret 8`). This left the caller's `-1`
length on the stack, where a second incorrect double treated it as a bank-index
argument to the factory and consumed it with `ret 24`. The two mistakes cancelled
in that fixture. Actual factory 009F83D0 uses five stack words, including the
hidden result pointer, and `ret 20`. There is no bank-index argument.

Both readable caller declarations and the fixture were corrected before linking
the actual strings and factory. The independent gates check stack cleanup and
return values against retail, and the connected gate checks its final stack.
Earlier passing doubled results did not prove these service ABIs.

## Contracts now exercised

Strings preserve the 17-byte packed data record, length, capacity high flag,
fast-extend bit, unaligned reference count, terminator and alignment padding.
Tests cover sharing, repeated/same-owner assignment, self-assignment, overwrite
of existing buffers, explicit lengths including embedded NULs, null/empty input,
poisoned allocation bytes and failed record allocation. Raw buffer allocation
failure and invalid negative lengths other than -1 are not claimed safe.

Format selection uses the real retail 46-row format table plus adversarial
tables. DXT selectors ignore requested depth; failed capability checks leave
the output untouched. Non-alpha uncompressed selection prefers a strictly
smaller alpha channel, preserving ties; alpha selection takes the first valid
candidate; signed selection takes the first supported type-six format. Tests
compare every D3D argument and HRESULT decision, including display mutation
between queries and first/last/missing table entries. The D3D device remains a
controlled COM boundary, not a real device in these offline tests.

The factory traverses the name cache before allocating. Identical string storage
is a hit; distinct storage must have equal lengths and signed-byte string
comparison. Cache hits ignore changed initialization values and the two unused
boolean arguments. Misses perform both progress announcements, construct and
initialize a bank, sample the global Open flags before the second announcement,
open it, append a 64-byte cache node and preserve list/name/bank reference owners.
Progress callbacks that change the Open-mode global prove its sampling order.

Failed allocation of the bank's counted-reference record is preserved: the
temporary/cache reference can retain null Data/Info while the returned raw bank
is non-null. Manager SetGraphicsBank subsequently ignores a null-Info assignment
when its current Info is also null. Tests observe this retail behavior; it is not
claimed as desirable recovery from allocation failure. Failed bank or list-node
allocation is not covered as a valid path.

Bank construction now uses a reconciled 0x30C retail layout, including both
resource lists, state defaults, counted texture-manager ownership and all eleven
blank texture slots. Initialization expands seven requested formats into eleven
slots, skips -1 formats, clears the returned surface and releases the local one.
Tests cover all 128 input-format masks, allocation failures, callbacks changing
the next format, alternate returned surfaces and unsuccessful texture creation.

The real 0x5D4 texture manager constructs sixteen 84-byte resource pools, including
self-linked resource lists. The donor 0x614 layout is incompatible. Its complete
1,492-byte result, including untouched preallocation fields, matches retail for
256 poisoned-memory patterns. The connected gate now compares all manager, bank
and texture-manager bytes, cache links, reference counts and service ordering.

## Validation and compiler margin

```powershell
python tools/decomp_pipeline/check_ui_strings.py
python tools/decomp_pipeline/check_ui_display_formats.py
python tools/decomp_pipeline/check_ui_bank_factory.py
python tools/decomp_pipeline/check_ui_bank_runtime.py
python tools/decomp_pipeline/check_ui_texture_manager.py
python tools/decomp_pipeline/check_ui_manager_construction.py
```

All pass: **324 string**, **2,682 display**, **576 factory**, **1,088 bank
runtime**, **256 texture manager**, **324 connected construction** cases
(**5,250 comparisons**). The previous ownership gate also passes all **974**
cases after these changes, giving **6,224** across the seven gates. Bootstrap
runs all five new independent gates before its expanded construction gate.
Reports and disassemblies live under `work/ui_*_check/`. The full bootstrap
and a live game launch were not run for this checkpoint.

Three additions match after relocation normalization: empty string construction
15/15 bytes, global system getter 6/6, and empty bank policy 3/3. The other 28
are functional DIFFER. Example compiled/retail sizes: byte assignment 123/135,
uncompressed selectors 124/140 and 149/171, factory 584/693, bank constructor
307/337, initializer 355/343, texture manager 220/219, resource list 75/77.
The factory factors repeated ownership/string operations into recovered helpers,
accounting for much of the shorter body. These are behavior comparisons, not a
percentage-of-instructions parity claim. Generated coverage is unchanged.

Evidence: retail executable SHA-256
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`, native instruction
inspection, existing shared layouts and original checkout PDB fields. CCharString
and CDisplayManager offsets are cross-checked against those PDB fields. Graphics
bank's donor header is quarantined (0x388 donor versus 0x30C retail); it must not
be used as a retail object layout.

## Next boundary

Recover bank-file base construction 009D5F80 and its underlying constructor
009CD480, then texture creation 009FA280, surface query 009F9E00, surface clear
009F40A0 and release 009F2E20. Bank-file construction includes allocated tree/list
sentinels and a critical section; reconcile its retail fields before using donor
headers. Virtual bank Open still needs archive/file services. The independent
factory gate deliberately isolates bank construction, and the independent bank
runtime gate isolates texture-manager construction; the aggregate manager gate
links the recovered implementations of both.

Keep remaining services explicit until recovered; cache creation does not prove
actual asset/device readiness. Native frontend actions, persistent renderer
integration and visual fidelity remain open.

No game or GUI launched. No presenter edits. Changes are saved and uncommitted;
earlier work is preserved. The main script-recovery checkout was not edited.
