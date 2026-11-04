"""Fresh VC71 parity gate: CMemoryAllocatorFixedSize statistics/query virtuals (0xA65D30..0xA66550) over the promoted hand-owned fable_memory_allocator_fixed_size.h, and CFontBankStateBlock GetDefault/Set/CreateCRC (0xA632D0, 0xA632F0, 0xA63890). Journal 2026-10-29."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Calc@CCRC@@QBEIPBXI@Z':0xa00950,
}
ROOTS=[
 dict(name='cfontbankstateblock_getdefault',source=R/'rebuild/src/compiled/00/a6/CFontBankStateBlock_GetDefault_00a632d0.cpp',symbol='?GetDefault@CFontBankStateBlock@@UAEXAAVCBankStateBlock@@@Z',address=0xa632d0,size=22,instructions=6,relocations=0),
 dict(name='cfontbankstateblock_set',source=R/'rebuild/src/compiled/00/a6/CFontBankStateBlock_Set_00a632f0.cpp',symbol='?Set@CFontBankStateBlock@@MAEXPBVCBankStateBlock@@@Z',address=0xa632f0,size=25,instructions=8,relocations=0),
 dict(name='cfontbankstateblock_createcrc',source=R/'rebuild/src/compiled/00/a6/CFontBankStateBlock_CreateCRC_00a63890.cpp',symbol='?CreateCRC@CFontBankStateBlock@@UAEKXZ',address=0xa63890,size=12,instructions=5,relocations=1),
 dict(name='cmemoryallocatorfixedsize_consolidatefreeareas',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_ConsolidateFreeAreas_00a65d30.cpp',symbol='?ConsolidateFreeAreas@CMemoryAllocatorFixedSize@@UAEXXZ',address=0xa65d30,size=1,instructions=1,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getallocationsize',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetAllocationSize_00a65d40.cpp',symbol='?GetAllocationSize@CMemoryAllocatorFixedSize@@UAEKPAX@Z',address=0xa65d40,size=9,instructions=2,relocations=0),
 dict(name='cmemoryallocatorfixedsize_gettotalmemorycontrolledbyallocator',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetTotalMemoryControlledByAllocator_00a65d50.cpp',symbol='?GetTotalMemoryControlledByAllocator@CMemoryAllocatorFixedSize@@UAEKXZ',address=0xa65d50,size=32,instructions=10,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getheaderoverhead',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetHeaderOverhead_00a65d70.cpp',symbol='?GetHeaderOverhead@CMemoryAllocatorFixedSize@@UAEKXZ',address=0xa65d70,size=36,instructions=14,relocations=0),
 dict(name='cmemoryallocatorfixedsize_gettotalmemoryallocated',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetTotalMemoryAllocated_00a65da0.cpp',symbol='?GetTotalMemoryAllocated@CMemoryAllocatorFixedSize@@UAEKXZ',address=0xa65da0,size=52,instructions=19,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getallocationsizeincludingheader',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetAllocationSizeIncludingHeader_00a65de0.cpp',symbol='?GetAllocationSizeIncludingHeader@CMemoryAllocatorFixedSize@@UAEKPAX@Z',address=0xa65de0,size=9,instructions=2,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getlargestfreeareasize',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetLargestFreeAreaSize_00a65df0.cpp',symbol='?GetLargestFreeAreaSize@CMemoryAllocatorFixedSize@@UAEKXZ',address=0xa65df0,size=48,instructions=21,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getnofreeareas',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetNoFreeAreas_00a65e20.cpp',symbol='?GetNoFreeAreas@CMemoryAllocatorFixedSize@@UAEKXZ',address=0xa65e20,size=39,instructions=17,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getnoallocatedareas',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetNoAllocatedAreas_00a65e50.cpp',symbol='?GetNoAllocatedAreas@CMemoryAllocatorFixedSize@@UAEKXZ',address=0xa65e50,size=29,instructions=11,relocations=0),
 dict(name='cmemoryallocatorfixedsize_getfragmentation',source=R/'rebuild/src/compiled/00/a6/CMemoryAllocatorFixedSize_GetFragmentation_00a66550.cpp',symbol='?GetFragmentation@CMemoryAllocatorFixedSize@@UAEMXZ',address=0xa66550,size=7,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-allocator-font-state.json',[],
  'CMemoryAllocatorFixedSize statistics/query virtuals (0xA65D30..0xA66550) over the promoted hand-owned fable_memory_alloc')
 print('NATIVE_ALLOCATOR_FONT_STATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
