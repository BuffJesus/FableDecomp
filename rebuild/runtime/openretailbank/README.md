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

| status | count | meaning |
|--------|-------|---------|
| landed byte-exact | 16 | already in `rebuild/src/compiled` catalog, MATCH/RELOCATION_MATCH |
| faithful behavior | 9 | staged under `faithful/`, behavior-PASS, DIFFER on bytes (RTM-3077 vs retail QFE-4035 codegen wall) — execute identically |
| MISSING | 2 | `CThreadedFile::CThreadedFile` (0098dfd0) + `CThreadedFile::Open` (0098e1e0) |

Note: `faithful/CharConstruct_0099aed0.cpp` is actually RELOCATION_MATCH (byte-exact)
— it was mis-deferred during the leaf crawl and can be promoted to the byte-parity
catalog. `faithful/ContainedBankIndex_009ac530.cpp` is the one soft spot: its authored
test LINK_FAILed, so its behavior is not yet independently verified — re-verify before
trusting the runtime parse.

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
