# Native read-only bank opening and streams - 2026-09-23

Worktree: `D:\Documents\FableTLC-native-play`, branch `wip/native-cgame-play`.
Continues [bank files and textures](FRONTEND_BANK_FILES_AND_TEXTURES_2026-09-23.md).
User asked to continue the standalone reconstruction, accepting reasonable
compiler differences. Changes are saved uncommitted; main quest checkout untouched.

## Implementation

Both complete read-only opening routines now use readable C++:
CBankFile::OpenReadOnly **009D06F0** and CBankFileAsync::OpenReadOnly **009D56C0**.
The base routine retains the name, resets opening state, branches on global retail
mode and flags, checks registered bank type, acquires the disk-file reference,
constructs/seeks the buffered input stream, dispatches entry reading/finalization
and cleans up. Its other branch resolves a path, invokes the bank's path-opening
virtual method and preserves that result across temporary destruction.

The async routine calls the recovered base opener, announces progress, then
acquires an existing registered threaded file or constructs/opens a new one.
The threaded-file Open bool is intentionally ignored: retail returns true after
that call even when it returns false. RetailMode is sampled after progress, so a
callback changing it changes the branch. Registered lookup failure after base
opening returns false without undoing the earlier base state. These are tested
contracts, not attempts to improve retail failure handling.

Recovered dependencies include filename forwarding 009CBF10; threaded/disk
reference assignment 009D6FD0 / 009A9BF0; threaded/disk/registered reference
release 009A9C40 / 009A9BB0 / 009A9D60; threaded reset/delete 009A9C80 / 009A9040.
These use the previously verified shared ownership implementation where behavior
is identical. Consequently several wrappers are much shorter than retail.
The existing threaded-file constructor 0098DFD0 is reconnected using the shared
CThreadedFile header and actual base/empty-wide-string constructors; it is not
counted as a newly discovered routine.

The buffered-stream dependency was still assembly in the older checkout. This
increment supplies readable construction 00994700, seek 00993BC0, close 00994300,
destruction 00994780, base destruction 0099A300 and shared position getter 00405FA0.
Shared CFileDataInputStream and CDataInputStream layouts provide the fields.
No new assembly bodies were added. This is sixteen recovered/upgraded bodies
plus the reconnected threaded constructor, not seventeen new byte matches.

Seek retains the source window when the target lies within its inclusive bounds;
otherwise it invalidates the window. Arithmetic follows 32-bit unsigned wrapping.
Close frees an existing buffer and restores the file position only for a seekable
file, reloading the file pointer after callbacks. If Buffer is null, BufferSize
is left untouched. A zero-sized constructor leaves both buffer-tail fields
untouched. These poison-memory behaviors are preserved, not advertised as safe
use of an uninitialized zero-buffer stream.

## Validation

```powershell
python tools/decomp_pipeline/check_ui_bank_stream.py
python tools/decomp_pipeline/check_ui_bank_open.py
python tools/decomp_pipeline/check_ui_manager_construction.py
python tools/decomp_pipeline/check_ui_bank_factory.py
python tools/decomp_pipeline/check_ui_bank_runtime.py
```

All pass: **1,024 stream**, **2,048 opening**, **324 connected manager**,
**576 factory regression** and **1,088 bank-runtime regression** cases.
Two new gates add **3,072** cases; the eleven saved construction/ownership gate
families now total **11,488**. Unchanged older families were not rerun needlessly.
Both gates are included in bootstrap before the aggregate manager gate. Full
bootstrap was not run. Python syntax, PowerShell parsing and scoped whitespace
checks are part of final verification.

Opening tests cover registered/path branches, first and second lookup failure,
type mismatch, entry-count wraparound, reference-control allocation failure,
old owner destruction, with/without reference records, progress mutating mode,
false threaded Open, full bank/reference/threaded object bytes and cleanup order.
The opening gate links real buffered stream construction/seek/destruction; only
archive registration/path resolution, entry decoding/finalization, wide temporary
destruction, virtual file methods and threaded OS opening are controlled there.

The full manager gate now invokes the actual async/base OpenReadOnly functions
instead of its prior virtual-open stand-in. Registered-bank lookup failure and
path-backed open failure are explicit fixture outcomes. The factory still caches
the bank and assigns references because retail ignores Open's result. This is a
verified failure path, **not successful disk/archive loading**. Successful opening
branches are exercised in the separate 2,048-case gate with controlled backends.
The bank now retains BankHandle, whose pointer and extra string owner are included
in aggregate normalization and comparison. D3D COM and Win32 services remain
controlled. No game, presenter or live-device test was launched in this increment.
The prior live D3D DEVICELOST result remains unresolved.

## Compiler margin

| Routine | Compiled / retail bytes |
| --- | --- |
| Base OpenReadOnly | 456 / 385 |
| Async OpenReadOnly | 381 / 348 |
| Bank path getter | 22 / 36 |
| Threaded reset/delete | 59 / 103; 13 / 11 |
| Three reference-release wrappers | 7 / 53 each |
| Two reference-assignment wrappers | 29 / 72 each |
| Reconnected threaded constructor | 48 / 41 |
| Stream constructor / seek | 121 / 113; 79 / 79 |
| Stream close / destructor | 92 / 86; 33 / 28 |
| Base destruction / position getter | 8 / 7; 4 / 4 |

Only the four-byte shared getter relocation-matches. All others are behavior-tested
DIFFER; equal seek sizes do not imply byte parity. Explicit readable calls and
factored ownership account for many differences. Reports contain compiled
fingerprints and disassemblies under `work/ui_{bank_open,bank_stream}_check/`.
Generated coverage was not edited. Function overrides record the actual opening
and stream identities and current manager evidence.

Retail executable SHA-256:
`41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10`.

## Next work

1. **Registered-bank lookup 009A7F80** (270 bytes): alias tree at registry +0x18,
   registered-file list head at +0x10, per-file bank map at +0x18. Retains each
   visited registered object, finds the first match, copies its 20-byte header
   and assigns the output counted reference. Missing lookup leaves outputs
   unchanged. Name-copy/destruction and shared ownership are already recovered.
2. **Map find helpers 009AB4F0 / 009AB560 / 009AB5D0** (110 bytes each) implement
   the same lower-bound/exact-equivalence walk, using key storage at node +0x10,
   left/right +8/+0x0C, header root +4. They call string-data less-than **00429950**.
   Check null-storage ordering; it cannot be replaced with ordinary empty-string
   comparison without evidence.
3. **Path lookup 009A7CA0** resolves aliases and the path map, then constructs a
   wide path via **0099BE70** with registry +0x24. Its missing-path branch builds
   a string ending in ` bank not found!` and calls the exception runtime through
   **00BFEB84**, rather than returning an empty path. Keep this distinct from
   virtual OpenPath returning false for an unavailable backing file.
4. **Entry decoding 009CFBC0**, bank virtual finalization, wide-string services and
   **CThreadedFile::Open 0098E1E0** remain external. The latter normalizes a path,
   opens a Win32 handle, gets file size, assigns the original filename and advances
   the global physical-sort key; constructor recovery alone does not implement it.

Then connect actual archive contents, frontend Run/actions and persistent
rendering. The standalone game is not yet playable through this recovered chain.
