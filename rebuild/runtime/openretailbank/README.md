# OpenRetailBank runtime module

Goal: link Fable.exe's **own** `CBankFileManager::OpenRetailBank` reconstruction and
its direct-callee ring into a callable module that opens a real `.big` at runtime —
the reconstruction runs the engine's actual bank-open code path, byte-for-byte where
the compiler allows and behavior-faithful everywhere else.

## Anchor

`CBankFileManager::OpenRetailBank(const CWideString&, bool)` @ retail `0x009A8840`.
Source: `rebuild/src/compiled/00/9a/CBankFileManager_OpenRetailBank_009a8840.cpp`
(naked, byte-pure). Verified **RELOCATION_MATCH, 1565/1565 bytes** — identical to
retail after masking its 44 relocations (42 call sites + 2 global data refs).

The body keeps every external reference as a real COFF relocation via 27 distinct
`extern "C" FableOpenRetail*_009A8840` thunks plus two globals:

- `g_FableOpenRetailDiskVTable_0122D06C` — the disk-file-stream vtable pointer written
  into the temporary `CFileDataInputStream` at `[esi]`.
- `g_FableOpenRetailMagic_0129B07C` — the expected bank magic compared against the
  first dword read from the stream.

## Direct-callee ring (27 thunks)

`link_manifest.tsv` is the authoritative table (address, thunk symbol, role, status,
source). The thunk→address mapping is derived by pairing the ordered `__asm call`
symbols in the anchor with the ordered E8 targets in the retail bytes (see
`tools/` scratch `pair_thunks.py`). Status split at time of writing:

**Ring is 27/27 reconstructed.**

| status | count | meaning |
|--------|-------|---------|
| landed byte-exact | 20 | in `rebuild/src/compiled` catalog, MATCH/RELOCATION_MATCH |
| faithful behavior | 7 | staged under `faithful/`, behavior-PASS, DIFFER on bytes (RTM-3077 vs retail QFE-4035 codegen wall) — execute identically |

`CThreadedFile::CThreadedFile` (0098dfd0) landed byte-exact; `CThreadedFile::Open`
(0098e1e0) now compiles to the full 310-byte retail-sized body and its behavior is
verified across both drive branches, both caching flags, and the CreateFileW-fails
path. The strict object gate now reports `RELOCATION_MATCH` for the full 310-byte
body with 21 relocations; no compiler residue is masked or accepted by the canonical
build.

`CWideString::CWideString` (0099aed0),
`CContainedBankMap::operator[]` (009ac530), and
`CCountedPointer<CThreadedFile>::Reset` (009a9c80) are now wired through their canonical
`rebuild/src/compiled` byte-parity sources; the old faithful copies remain only as
traceability. Both canonical behavior fixtures pass, and the real-bank probe also
verifies the resulting key/value map against the clean-room oracle.

The runtime host adapter is now verified against a real bank: all three contained-bank
keys and all five stored values agree with the clean-room oracle.

## Linking (next step)

Each `FableOpenRetail*` thunk is an ABI-transparent `jmp` forwarder to the matching
reconstruction (this-in-ecx and stack args pass straight through). The direct ring is
not the whole story: every callee has its own transitive callees (the full ~107-fn
closure) and OpenRetailBank operates on a live `CBankFileManager` instance reading
through a real `CThreadedFile`, so a *running* link also needs the engine's disk
vtable, the bank-manager singleton, and the global allocators wired up. The ring being
27/27 reconstructed is the milestone that unblocks that link.

## Validation

`fable_bank_reader.h` (clean-room BIGB reader, already proven to byte-match Python on
real 53 MB banks) is the oracle: drive the linked module on the same `.big` and diff
its parsed TOC / entry table against the clean-room reader's output.

## Current runtime milestone (2026-08-02)

The 27 ABI-transparent forwarders are now supplied by `ring_thunks.cpp`. The
standalone link closure is represented by `runtime_globals.cpp` and
`runtime_helpers.cpp`, and `link_smoke.py` compiles the 31 functional ring
objects plus one link-only CRT shim and performs a strict DLL link with
**zero unresolved externals**.

`runtime_probe.py` now links the same x86 ring as a console executable and runs
the raw `OpenRetailBank` body against the installed `frontend.big`. The probe
passes with one contained bank, 394/394 entry records, and 13,287,366/13,287,366
entry bytes matching the oracle.

The host layer now has working disk seek/refill, buffered-stream ABI handling,
memory-stream setup, path conversion, safe counted disk ownership, and the
contained-bank map insertion/ownership path. The probe compares the header/footer
subbank count plus every contained-bank key and its five stored values. The standalone
`FableBankReader_test.cpp` remains the TOC metadata oracle. The threaded handle
state and all 26 oracle entry payloads now match byte-for-byte; the remaining
boundary is engine-level entry-metadata consumption rather than file-read parity.
