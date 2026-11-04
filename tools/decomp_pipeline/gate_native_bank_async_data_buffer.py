"""Fresh VC71 parity gate for CBankFileAsyncData::SetupBuffer 009D52D0 and WaitUntilFinished 009D5B60."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# Addresses agree with rebuild/manifest/functions.tsv names and landed providers.
ADDRESS={
 '??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,
 '??1CCriticalSectionLock@@QAE@XZ':0xa629a0,
 '?Release@CCriticalSectionLock@@QAEXXZ':0xa62960,
 '?Align@CBankFile@@QBEKK@Z':0x9cbb90,
 '?g_CBankFileAsyncFailureHandlingMode_013CA868@@3IA':0x13ca868,
 '?AllocateResource@CMovableResourceMemoryPool@@QAE_NPAJPAVCMovableResource@@KK@Z':0xa62e40,
 '?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z':0xa026b0,
 '?g_FableMemoryAllocator_013CAA90@@3PAVCMemoryAllocator@@A':0x13caa90,
 '?Create@CMemoryAllocator@@KIXXZ':0x9f2870,
 '?EnterSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2000,
 '?InternalAllocate@CMemoryAllocator@@IAEPAXJK_N@Z':0x9f2a40,
 '?LeaveSynchronisation@CMemoryAllocator@@KIXXZ':0x9f2010,
 '?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z':0x9d52d0,
 '?RestartAbortedReads@CBankFileAsync@@AAEXXZ':0x9d5a90,
 '?WaitForObjectToFinish@CThreadedFile@@QAEXK@Z':0x98f370,
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '??2@YAPAXI@Z':0xbfea1a,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z':0x9d5d30,
 '?DeleteData@?$CCountedPointer@VCBankFileAsyncData@@@@CIXPAX@Z':0x9a42b0,  # pinned by autoalloc gate
 '?SetupRead@CBankFileAsyncEntry@@AAEXKAAVCBankFileAsyncData@@KKKW4EThreadedFilePriority@@@Z':0x9d5970,
 '??_7CMovableResource@@6B@':0x129b860,
 '??_7CBankFileAsyncData@@6BCThreadedFileCallback@@@':0x129b8b4,
 '??_7CBankFileAsyncData@@6BCMovableResource@@@':0x129b8a0,
 '?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@_STL@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@_STL@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@@_STL@@_N@2@ABQAVCBankFileAsyncData@@@Z':0x9d7cf0,
}
ROOTS=[
 dict(name='setup_buffer',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_SetupBuffer_009d52d0.cpp',symbol='?SetupBuffer@CBankFileAsyncData@@AAE_N_N@Z',address=0x9d52d0,size=310,instructions=115,relocations=17),
 dict(name='wait_until_finished',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_WaitUntilFinished_009d5b60.cpp',symbol='?WaitUntilFinished@CBankFileAsyncData@@QAE_NXZ',address=0x9d5b60,size=130,instructions=43,relocations=7),
 dict(name='buffer_ctor',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp',symbol='??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z',address=0x9d5d30,size=179,instructions=52,relocations=7),
 dict(name='begin_read_data',source=R/'rebuild/src/compiled/00/9d/CBankFileAsyncEntry_BeginReadData_009d6060.cpp',symbol='?BeginReadData@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KPAXKKW4EThreadedFilePriority@@@Z',address=0x9d6060,size=160,instructions=67,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-async-data-buffer.json',
  ['work/native-recreation/rendering/009d52d0/20261015/native-wait-composition-first (65,536 cases / 575,488 snapshots)','work/native-recreation/rendering/009d5b60/20261015/native-first (16,384 cases / 106,496 snapshots)'],
  'Two genuine roots, byte-identical to retail after audited relocation patching. Pool, allocator, lock, restart and threaded-file wait are separate providers; no concurrency or allocation-failure claim beyond byte identity.')
 print('NATIVE_BANK_ASYNC_DATA_BUFFER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
