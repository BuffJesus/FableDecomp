"""Fresh VC71 parity gate: CHandleBasedMemoryPool PartialDefragment, Allocate (handle and CScopedMemoryHandle), CScopedCritical."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FindLargestFreeArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@XZ':0xa01f50,
 '?UpdateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ':0xa01f80,
 '?TestIfAreaIsGrowable@CHandleBasedMemoryPool@@AAE_NPAVCHeader@1@J@Z':0xa02020,
 '?Free@CHandleBasedMemoryPool@@QAEXJ@Z':0xa02b30,
 '?GrowFreeArea@CHandleBasedMemoryPool@@AAE_NAAPAVCHeader@1@@Z':0xa02c00,
 '?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z':0xa02df0,  # derived from retail site; not in symbol_index
 '?InternalAllocate@CHandleBasedMemoryPool@@AAE_NPAJK@Z':0xa030d0,
 '?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z':0xa03180,  # derived from retail site; not in symbol_index
 '??1CScopedCritical@CHandleBasedMemoryPool@@QAE@XZ':0xa03530,  # derived from retail site; not in symbol_index
 '?push_back@?$vector@PAVCHeader@CHandleBasedMemoryPool@@V?$allocator@PAVCHeader@CHandleBasedMemoryPool@@@_STL@@@_STL@@QAEXABQAVCHeader@CHandleBasedMemoryPool@@@Z':0xa038c0,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '__imp__TryEnterCriticalSection@4':0x143fd30,  # derived from retail site; not in symbol_index
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='partial_defragment',source=F,symbol='?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z',address=0xa02df0,size=585,instructions=193,relocations=11),
 dict(name='allocate',source=F,symbol='?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z',address=0xa03180,size=358,instructions=137,relocations=8),
 dict(name='allocate_scoped',source=F,symbol='?Allocate@CHandleBasedMemoryPool@@QAE_NAAVCScopedMemoryHandle@@KK@Z',address=0xa032f0,size=84,instructions=32,relocations=2),
 dict(name='scoped_critical_ctor',source=F,symbol='??0CScopedCritical@CHandleBasedMemoryPool@@QAE@AAV1@_N@Z',address=0xa034e0,size=68,instructions=26,relocations=2),
 dict(name='scoped_critical_dtor',source=F,symbol='??1CScopedCritical@CHandleBasedMemoryPool@@QAE@XZ',address=0xa03530,size=29,instructions=11,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-allocation-policy.json',[],
  'CHandleBasedMemoryPool PartialDefragment, Allocate (handle and CScopedMemoryHandle), CScopedCritical')
 print('NATIVE_POOL_ALLOCATION_POLICY_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
