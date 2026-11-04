"""Fresh VC71 parity gate: CHandleBasedMemoryPool GrowFreeArea, constructor/destructors, InternalAllocateHandle, InternalAllocate."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBaseClassNonCopyable@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?CombineAreas@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z':0xa01e40,
 '?FindFreeArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@J@Z':0xa01ed0,
 '?UpdateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ':0xa01f80,
 '?Uninitialise@CHandleBasedMemoryPool@@QAEXXZ':0xa02190,
 '?ResizeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@J@Z':0xa02410,
 '?CombineAdjacentAreas@CHandleBasedMemoryPool@@AAEXAAPAVCHeader@1@@Z':0xa02470,
 '?MoveArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z':0xa028f0,
 '?InternalAllocateHandle@CHandleBasedMemoryPool@@AAEJXZ':0xa03040,
 '?_M_insert_overflow@?$vector@VCHandleInfo@CHandleBasedMemoryPool@@V?$allocator@VCHandleInfo@CHandleBasedMemoryPool@@@_STL@@@_STL@@QAEXPAVCHandleInfo@CHandleBasedMemoryPool@@ABV34@ABU__false_type@2@I_N@Z':0xa03900,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_7CHandleBasedMemoryPool@@6B@':0x129c964,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='grow_free_area',source=F,symbol='?GrowFreeArea@CHandleBasedMemoryPool@@AAE_NAAPAVCHeader@1@@Z',address=0xa02c00,size=285,instructions=127,relocations=8),
 dict(name='constructor',source=F,symbol='??0CHandleBasedMemoryPool@@QAE@XZ',address=0xa02d20,size=129,instructions=35,relocations=3),
 dict(name='destructor',source=F,symbol='??1CHandleBasedMemoryPool@@UAE@XZ',address=0xa02db0,size=52,instructions=15,relocations=5),
 dict(name='internal_allocate_handle',source=F,symbol='?InternalAllocateHandle@CHandleBasedMemoryPool@@AAEJXZ',address=0xa03040,size=136,instructions=43,relocations=1),
 dict(name='internal_allocate',source=F,symbol='?InternalAllocate@CHandleBasedMemoryPool@@AAE_NPAJK@Z',address=0xa030d0,size=176,instructions=68,relocations=4),
 dict(name='scalar_deleting_destructor',source=F,symbol='??_GCHandleBasedMemoryPool@@UAEPAXI@Z',address=0xa03aa0,size=73,instructions=22,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-allocate.json',[],
  'CHandleBasedMemoryPool GrowFreeArea, constructor/destructors, InternalAllocateHandle, InternalAllocate')
 print('NATIVE_POOL_ALLOCATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
