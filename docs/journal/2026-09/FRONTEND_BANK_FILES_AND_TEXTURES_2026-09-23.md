# Native bank files and texture services - 2026-09-23

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues the [31-function construction checkpoint](FRONTEND_CONSTRUCTION_SERVICES_2026-09-23.md)
at the user's request to keep building the standalone engine with reasonable
compiler differences. This increment adds **14 complete readable recoveries**
and restores the previously recovered empty CWideString constructor into this
worktree. The restored constructor is not counted as new recovery.

## Recovered code

| Function | Address | Compiled / retail bytes | Grade |
| --- | --- | --- | --- |
| Surface release | 009F2E20 | 32 / 32 | RELOCATION_MATCH |
| Surface attach | 009F2F10 | 28 / 66 | functional DIFFER |
| Surface copy | 009F2D60 | 56 / 55 | functional DIFFER |
| Texture surface query | 009F9E00 | 104 / 96 | functional DIFFER |
| Surface lock | 009F33E0 | 133 / 141 | functional DIFFER |
| Pixel-format bit count | 009E3820 | 13 / 13 | RELOCATION_MATCH |
| Surface clear | 009F40A0 | 126 / 120 | functional DIFFER |
| Texture release | 009F9F70 | 34 / 34 | RELOCATION_MATCH |
| Texture byte-length update | 009F9EE0 | 142 / 143 | functional DIFFER |
| Blank texture creation | 009FA280 | 156 / 181 | functional DIFFER |
| Bank-file construction | 009CD480 | 423 / 402 | functional DIFFER |
| Async bank-file construction | 009D5F80 | 226 / 224 | functional DIFFER |
| Checksum-cache construction | 00A60C90 | 77 / 73 | functional DIFFER |
| Packed-integer-array construction | 00A629C0 | 16 / 16 | RELOCATION_MATCH |

The shorter attach/create bodies call recovered release helpers where retail
inlines that work. Byte-size differences do not measure semantic accuracy.
Reports record disassemblies and masked hashes. No assembly bodies were added.
Generated coverage was not edited.

## Contracts and corrected identities

Texture creation's fifth stack argument is a complete D3D memory-pool value,
not a boolean. The declaration and earlier fixtures now retain all 32 bits.
Nonpositive mip counts halve both unsigned dimensions while both are at least
eight, then add the final level. Successful creation sets allocation source one
and computes the 28-bit byte length from the level descriptions, preserving the
upper source nibble. Failed HRESULT behavior and output-pointer mutations follow
retail, including existing texture release before creation.

Surface attachment adopts its input without AddRef; copy adds a reference and
copies PAllocatedMemory only for allocation source two. GetSurfaceLevel transfers
its returned reference through the local surface/copy/release sequence. Release
clears source and pointer only when a native pointer was present. Lock failure
zeros all output fields. Descriptor callbacks can change the engine object's
native pointer, so later calls reload it. Clear uses a contiguous byte count
from width, height and format bits, not pitch times height. It does not add
recovery from a failed lock with positive size; that retail path can fault.

Retail RTTI proves 0129B54C is CBankFile and 0129B8D4 is CBankFileAsync.
The corresponding retail layouts are **0x110 and 0x164**. The generated
CBankFile donor header is 0x174 and must not be used for these objects.
`fable_ui_bank_file.h` supplies explicit reconciled views, donor field names where
established and compile-time offset checks. Shared CPackedUIntArray, CSurface,
CTexture and string definitions are reused.

Bank construction builds four packed arrays, four tree heads including the
checksum cache, two narrow and two wide empty strings. Async construction adds
three counted-reference pairs, two set heads, the disposal-list head and a
critical section. Tests inspect memory at every allocation, all final object
bytes, heap bytes and guard regions over 256 nonuniform poison patterns per
constructor. The checksum expression is `(size + 8) & ~8u`, not conventional
round-to-eight alignment; low bits and overflow are tested. Allocator failure
for required tree heads is not treated as a supported retail path.

Metadata overrides correct the async/cache/wide-string identities and update
manager-construction evidence. The generated manifest was not regenerated.

## Connected verification

```powershell
python tools/decomp_pipeline/check_ui_texture_surfaces.py
python tools/decomp_pipeline/check_ui_bank_file.py
python tools/decomp_pipeline/check_ui_manager_construction.py
python tools/decomp_pipeline/check_ui_bank_runtime.py
```

All pass: **1,168 texture/surface cases**, **1,024 bank-file cases**, **324 full
manager-construction cases**, and **1,088 previous isolated bank-runtime cases**.
The two new gates add 2,192 cases to the previously green families, bringing the
saved nine-family construction/ownership reports to **8,416 cases**. The old
unaffected gates were not unnecessarily rerun in this increment.

The full manager gate now links the recovered bank-file/async/cache and
texture/surface implementations. It compares all manager, graphics-bank and
texture-manager bytes, allocated tree/list contents, narrow/wide string counts,
reference counts, D3D call order and full-buffer pixel hashes. Only allocator,
D3D COM, Win32 critical-section and virtual bank Open boundaries are controlled.
Both independent gates were added before the aggregate gate in bootstrap;
PowerShell parsing and scoped whitespace checks pass. Full bootstrap was not run.

Offline reports: `work/ui_{texture_surfaces,bank_file,manager_construction,bank_runtime}_check/`.
Oracle executable SHA-256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Live-device check is not yet passing

```powershell
python tools/decomp_pipeline/check_ui_texture_device.py
```

Added an explicit, separate live test for nine format/dimension combinations.
It uses an owned hidden window, real D3D9 HAL texture creation, mip byte counts,
poison/clear/readback, and recovered surface/texture release. It is deliberately
not an unconditional offline bootstrap gate.

On this machine CreateDevice returned **0x88760868 (D3DERR_DEVICELOST)**, before
any texture case ran. The report has `accepted: false`. The owned window and
libraries were cleaned up; no presenter or game was launched. No live GPU success
or visual-fidelity claim is made. Retry on an available interactive D3D desktop;
do not change or terminate other running applications to force this test through.

## Next dependencies

The graphics-bank vtable's Open slot points to **CBankFileAsync::OpenReadOnly 009D56C0**
(348 bytes, bool result). It calls **CBankFile::OpenReadOnly 009D06F0** (385 bytes), then
announces progress and branches on RetailMode. Retail mode obtains a registered
bank's threaded-file reference; the other branch constructs/opens a CThreadedFile.
The factory ignores Open's bool result in the recovered path.

Important dependency addresses: bank registration lookup 009A7F80; path lookup
009A7CA0; threaded-file ctor 0098DFD0 and Open 0098E1E0; filename getter 009CBF10;
reference operations 009D6FD0, 009A9C40/80 and 009A9D60; buffered file construction
00994700, seek 00993BC0, bank data read 009CFBC0 and buffered cleanup 00994780.
Base Open also invokes bank virtual slots +0x0C, +0x20 and +0x2C. Resolve their
actual owners before inventing interfaces. File/archive opening is still unimplemented
in this connected chain; it is not replaced by an asserted successful disk load.

After archive services, frontend Run/actions and persistent renderer integration
remain open. Changes are saved and uncommitted; prior work is preserved and the
main quest-recovery checkout was not edited.
