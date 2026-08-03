# OpenRetailBank link worklist

Authoritative unresolved-externals from the ring link smoke-test
(`link_smoke.py`: anchor + 27 ring objects plus host closure, strict link).
These three buckets are everything between "ring reconstructed" and "module runs".

### Status as of 2026-08-02

- Bucket 1 (27 ABI-transparent ring thunks): **complete** in `ring_thunks.cpp`.
- Bucket 2: **runtime-open boundary complete** for disk seek/refill,
  buffered-stream ABI, memory-stream setup, path conversion, counted disk
  ownership, contained-bank map key/value parity, and full threaded payload
  parity.
- Bucket 3: **link closure complete** in `runtime_globals.cpp`; threaded-file
  callbacks remain host adapters rather than byte-parity engine globals.
- Strict `link_smoke.py`: **32/32 objects compile, 0 unresolved externals**
  (31 functional objects plus one link-only CRT shim).

The DLL produced by that smoke is a structural link artifact. The separate
`runtime_probe.py` is the executable evidence that a real `.big` is opened;
its oracle comparison covers the header/footer subbank count and each contained-bank
key plus five stored values, then reads all 26 entry payloads through the reconstructed
threaded handle.

## 1. Ring thunk adapters — forward each to its reconstruction (mechanical) (27)

- `_FableOpenRetailBankConstruct_009A8840`
- `_FableOpenRetailBankListInsert_009A8840`
- `_FableOpenRetailBankOwnerConstruct_009A8840`
- `_FableOpenRetailBufferAllocate_009A8840`
- `_FableOpenRetailCharAssign_009A8840`
- `_FableOpenRetailCharConstruct_009A8840`
- `_FableOpenRetailCharDestroy_009A8840`
- `_FableOpenRetailContainedBankIndex_009A8840`
- `_FableOpenRetailDiskBaseConstruct_009A8840`
- `_FableOpenRetailDiskOwnerAssign_009A8840`
- `_FableOpenRetailDiskOwnerConstruct_009A8840`
- `_FableOpenRetailDiskOwnerDestroy_009A8840`
- `_FableOpenRetailFileStreamConstruct_009A8840`
- `_FableOpenRetailFileStreamDestroy_009A8840`
- `_FableOpenRetailFree_009A8840`
- `_FableOpenRetailMemoryStreamConstruct_009A8840`
- `_FableOpenRetailMemoryStreamDestroy_009A8840`
- `_FableOpenRetailOperatorDelete_009A8840`
- `_FableOpenRetailOperatorNew_009A8840`
- `_FableOpenRetailPathExists_009A8840`
- `_FableOpenRetailReadNullString_009A8840`
- `_FableOpenRetailReadOverflow_009A8840`
- `_FableOpenRetailSetPosition_009A8840`
- `_FableOpenRetailThreadedConstruct_009A8840`
- `_FableOpenRetailThreadedOpen_009A8840`
- `_FableOpenRetailThreadedReset_009A8840`
- `_FableOpenRetailWideToChar_009A8840`

Each `_FableOpenRetail*_009A8840` is an ABI-transparent forwarder to the matching
ring source (see `thunk_map.json`). A naked `jmp` to the reconstruction symbol
preserves ecx/stack. Fully determined — no new RE.

## 2. Second-layer helper functions — the ring's own callees (needs RE or faithful stub) (32)

- `"int`
- `"private:`
- `"public:`
- `"void`
- `@CFileDataInputStream_Close_00994780@8`
- `@CFileDataInputStream_ConstructBase_00994700@8`
- `@CFileDataInputStream_DestroyBase_00994780@8`
- `@CFileDataInputStream_SetPosition_00994700@12`
- `@CThreadedFileBaseConstruct_0099A2F0@4`
- `@CThreadedFileMemberConstruct_0099AED0@4`
- `@ExpandPath@8`
- `@FableContainedBankMapInsert_009AC530@20`
- `@FableContainedBankMapLowerBound_009AC530@12`
- `@FableContainedBankMapStringCopy_009AC530@12`
- `@FableContainedBankMapStringDestroy_009AC530@8`
- `@FableContainedBankMapStringLess_009AC530@12`
- `@FableGetCurrentPath_00997F80@0`
- `@FableThreadedCharConstruct_0098E1E0@4`
- `@FableThreadedCharDestroy_0098E1E0@4`
- `@FableThreadedCharToUpper_0098E1E0@4`
- `@FableThreadedConvertFullPath_0098E1E0@8`
- `@FableThreadedFileObjectDelete_009A9040@4`
- `@FableThreadedSplitPath_0098E1E0@20`
- `@FableThreadedWideAssign_0098E1E0@8`
- `@FableThreadedWideConversion_0098E1E0@4`
- `_CFileDataInputStream_AllocateBuffer_00994700`
- `_CFileDataInputStream_BaseVTable_00994780`
- `_CFileDataInputStream_DerivedVTable_00994780`
- `_CFileDataInputStream_VTable_00994700`
- `_FableThreadedFileControlAllocate_009A9C80`
- `_FableThreadedFileControlDelete_009A9C80`
- `_operator_new_impl`

Each ring source declared these `extern` stubs for the functions IT calls. They are
the next closure layer. Options per symbol: reconstruct byte-faithfully, or provide a
behavior-faithful stub (the module still runs identically for bank-open).

## 3. Global data — vtables, string literals, IAT function pointers (define directly) (12)

- `_g_CBaseVTable_009AC4D0`
- `_g_FableEmptyWideString_0129A8E0`
- `_g_FableFreeThunkTarget_00440154`
- `_g_FableGetFileAttributesW_0143FDBC`
- `_g_FableOpenRetailDiskVTable_0122D06C`
- `_g_FableOpenRetailMagic_0129B07C`
- `_g_FableRetailBankVTable_0129B010`
- `_g_FableThreadedCreateFileW_0143FE2C`
- `_g_FableThreadedDDrive_0129A15C`
- `_g_FableThreadedEmptyChar_0122D70E`
- `_g_FableThreadedGetFileSize_0143FDF0`
- `_g_FableThreadedPhysicalSortKey_013BC9EC`

- vtables (`_g_*VTable*`): define the slot layout the code indexes (mostly dtor/read slots).
- string literals (`_g_FableThreadedDDrive_0129A15C` = L"D:\\", `_g_FableThreadedEmptyChar`,
  `_g_FableOpenRetailMagic` = the bank magic dword).
- IAT pointers (`_g_FableThreadedCreateFileW`, `_g_FableThreadedGetFileSize`,
  `_g_FableGetFileAttributesW`): point at the real Win32 exports so file reads actually happen.
- `_g_FableThreadedPhysicalSortKey_013BC9EC`: a global monotonic file-id counter (init 0).

The raw open path now links into `oab_ring.dll` and is also driven by
`runtime_probe.py` on a real `.big`. Threaded handle and full payload parity are
verified; the remaining increment is engine-level entry-metadata consumption.
