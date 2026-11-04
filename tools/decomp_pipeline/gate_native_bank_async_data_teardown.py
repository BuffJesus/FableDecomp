"""Fresh VC71 parity gate for CBankFileAsyncData FreeBuffer 009D5410, destructor 009D61F0 and Abort 009D6350.

Both roots compile from canonical rebuild/include to retail bytes once every
relocation is patched with an independently pinned symbol address. Behavior was
exercised in the work lane (native-base-cleanup / native-first fixtures, 131,072
cases each); this gate re-proves byte identity, so no model is re-run here.
"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ERASE='?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAEIABQAVCBankFileAsyncData@@@Z'
# Symbol identities pinned by other accepted gates/headers, not derived from these roots.
ADDRESS={
 '??_7CBankFileAsyncData@@6BCThreadedFileCallback@@@':0x129b8b4,
 '??_7CBankFileAsyncData@@6BCMovableResource@@@':0x129b8a0,
 '??_7CMovableResource@@6B@':0x129b860,
 '??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,
 '??1CCriticalSectionLock@@QAE@XZ':0xa629a0,
 '?Release@CCriticalSectionLock@@QAEXXZ':0xa62960,
 '?Lock@CCriticalSectionLock@@QAEXXZ':0xa62980,
 ERASE:0x9d7f40,
 '?RemoveObject@CThreadedFile@@QAE_NK@Z':0x98ec90,
 '__imp__Sleep@4':0x143fe1c,
 # 0xA62ED0 is a 5-byte jmp thunk to CHandleBasedMemoryPool::Free 0xA02B30.
 '?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z':0xa62ed0,
 '?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,
 '?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,
 '?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z':0x9f2920,
 '?GetNoAllocations@CMemoryAllocator@@QAEKXZ':0x9f1f10,
 '??1CMemoryAllocator@@IAE@XZ':0x9f2310,
 '??3CMemoryAllocator@@KIXPAX@Z':0x9f1f90,
 '?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,
 '?g_FableAllocatorShutdownRequested_013CAA94@@3_NA':0x13caa94,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??1CBankFileAsync@@UAE@XZ':0x9d6490,  # landed 00/9d/CBankFileAsync_Destructor_009d6490.cpp
 '??3@YAXPAX@Z':0xbfe9bc,
 '??1CBankFileAsyncData@@UAE@XZ':0x9d61f0,
 '??_7CBankFileAsyncEntry@@6B@':0x129b880,
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '?GetEntryDataSize@CBankFile@@QBEKK@Z':0x9cc240,
 '?Align@CBankFile@@QBEKK@Z':0x9cbb90,  # entry vtable, bound by work typed parity (journal 2026-10-15)
 # VC71 weak alias ??_E -> ??_G (see gate_native_bank_async_data_constructor); retail slot value 0x9D8180.
 '??_ECBankFileAsyncData@@UAEPAXI@Z':0x9d8180,
}
ROOTS=[
 dict(name='free_buffer',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp',symbol='?FreeBuffer@CBankFileAsyncData@@AAEXXZ',address=0x9d5410,size=47,instructions=15,relocations=1),
 dict(name='destructor',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp',symbol='??1CBankFileAsyncData@@UAE@XZ',address=0x9d61f0,size=348,instructions=102,relocations=25),
 dict(name='abort',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp',symbol='?Abort@CBankFileAsyncData@@QAE_NXZ',address=0x9d6350,size=196,instructions=62,relocations=7),
 dict(name='data_deleting_dtor',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp',symbol='??_GCBankFileAsyncData@@UAEPAXI@Z',address=0x9d8180,size=30,instructions=11,relocations=2),
 dict(name='data_deleting_thunk',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_Lifetime_009d5410.cpp',symbol='??_ECBankFileAsyncData@@W3AEPAXI@Z',address=0x9d80a0,size=8,instructions=2,relocations=1),
 dict(name='entry_deleting_dtor',source=R/'rebuild/src/compiled/00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp',symbol='??_GCBankFileAsyncEntry@@UAEPAXI@Z',address=0x9d6fa0,size=36,instructions=12,relocations=3),
 dict(name='entry_ctor',source=R/'rebuild/src/compiled/00/9d/CBankFileAsync_GetAsyncEntry_009d58d0.cpp',symbol='??0CBankFileAsyncEntry@@AAE@AAVCBankFileAsync@@K@Z',address=0x9d5260,size=52,instructions=17,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-async-data-teardown.json',
  ['work/native-recreation/rendering/009d61f0/20261015/native-base-cleanup (131,072 cases / 1,899,008 snapshots)','work/native-recreation/rendering/009d6350/20261015/native-first (131,072 cases / 1,085,440 snapshots)'],
  'Three genuine roots from one retail-shaped TU (FreeBuffer auto-inlined into destructor and Abort); byte-identical to retail after audited relocation patching. Callees (tree erase, RemoveObject, pool Free, allocator, lock) are separate providers; no concurrency, threaded-file or allocator-shutdown claim beyond byte identity.')
 print('NATIVE_BANK_ASYNC_DATA_TEARDOWN_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
