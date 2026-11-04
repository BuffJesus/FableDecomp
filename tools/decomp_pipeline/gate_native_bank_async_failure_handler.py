"""Fresh VC71 parity gate for SetLoadingMemoryFailureHandler 009D66A0 CLoadingMemoryFailureHandler::RegisterBank 009D6420 and its vector growth 009D7FC0."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# UnregisterBank is landed (00/9d/CLoadingMemoryFailureHandler_UnregisterBank_009d5a30.cpp);
# RegisterBank is the PDB-named sole other handler member taking the bank; 0xBFE9BC is
# operator delete, pinned by many accepted gates.
ADDRESS={
 '?UnregisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z':0x9d5a30,
 '?RegisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z':0x9d6420,
 '??3@YAXPAX@Z':0xbfe9bc,
 '??_7CLoadingMemoryFailureHandler@@6B@':0x129b8c4,  # slots: deleting dtor 9D8140, HandleFailedAllocation 9D5F10
 '__imp__InitializeCriticalSection@4':0x143fee4,  # KERNEL32 import slot (PE import table)
 '?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ':0x9d5bf0,
 '__imp__DeleteCriticalSection@4':0x143fde4,  # KERNEL32 import slot
 '??_7CFailedAllocationHandler@@6B@':0x129b874,  # RTTI .?AVCFailedAllocationHandler@@
 '_malloc':0xbfea0e,'_free':0xbfea14,'_memmove':0xbfeae6,
 '??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z':0xa62920,
 '??1CCriticalSectionLock@@QAE@XZ':0xa629a0,
 # POD growth path for vector<CBankFileAsync*> (manifest generic Vector_InsertElementRange).
 '?_M_insert_overflow@?$vector@PAVCBankFileAsync@@V?$allocator@PAVCBankFileAsync@@@_STL@@@_STL@@QAEXPAPAVCBankFileAsync@@ABQAV3@ABU__true_type@2@I_N@Z':0x9d7fc0,
}
ROOTS=[dict(name='set_failure_handler',source=R/'rebuild/src/compiled/00/9d/CBankFileAsync_SetLoadingMemoryFailureHandler_009d66a0.cpp',symbol='?SetLoadingMemoryFailureHandler@CBankFileAsync@@QAEXV?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@@Z',address=0x9d66a0,size=136,instructions=49,relocations=4),
 dict(name='register_bank',source=R/'rebuild/src/compiled/00/9d/CLoadingMemoryFailureHandler_RegisterBank_009d6420.cpp',symbol='?RegisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z',address=0x9d6420,size=97,instructions=35,relocations=4),
 dict(name='bank_vector_growth',source=R/'rebuild/src/compiled/00/9d/STLBankPointerVector_InsertOverflow_009d7fc0.cpp',symbol='?_M_insert_overflow@?$vector@PAVCBankFileAsync@@V?$allocator@PAVCBankFileAsync@@@_STL@@@_STL@@QAEXPAPAVCBankFileAsync@@ABQAV3@ABU__true_type@2@I_N@Z',address=0x9d7fc0,size=216,instructions=85,relocations=4),
 dict(name='handler_ctor',source=R/'rebuild/src/compiled/00/9d/CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp',symbol='??0CLoadingMemoryFailureHandler@@QAE@XZ',address=0x9d5eb0,size=34,instructions=13,relocations=2),
 dict(name='handler_failed_allocation',source=R/'rebuild/src/compiled/00/9d/CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp',symbol='?HandleFailedAllocation@CLoadingMemoryFailureHandler@@UAE_NK@Z',address=0x9d5f10,size=104,instructions=40,relocations=4),
 dict(name='handler_deleting_dtor',source=R/'rebuild/src/compiled/00/9d/CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp',symbol='??_GCLoadingMemoryFailureHandler@@UAEPAXI@Z',address=0x9d8140,size=63,instructions=21,relocations=5),
 dict(name='base_handler_deleting_dtor',source=R/'rebuild/src/compiled/00/9d/CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp',symbol='??_GCFailedAllocationHandler@@UAEPAXI@Z',address=0x9d5140,size=31,instructions=11,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bank-async-failure-handler.json',[],
  'Six genuine roots, byte-identical to retail after audited relocation patching; counted-pointer assignment and by-value parameter destruction inlined. No separate behavior fixture: equivalence rests on byte identity.')
 print('NATIVE_BANK_ASYNC_FAILURE_HANDLER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
