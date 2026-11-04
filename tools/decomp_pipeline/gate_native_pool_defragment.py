"""Fresh VC71 parity gate: CHandleBasedMemoryPool Defragment, InternalGetResource, MoveArea, ValidateResources."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?InsertFreeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@@Z':0xa01da0,
 '?ResizeArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@J@Z':0xa02410,
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='defragment',source=F,symbol='?Defragment@CHandleBasedMemoryPool@@QAEXXZ',address=0xa02730,size=380,instructions=124,relocations=3),
 dict(name='internal_get_resource',source=F,symbol='?InternalGetResource@CHandleBasedMemoryPool@@QAEPAXJ@Z',address=0xa028d0,size=18,instructions=5,relocations=0),
 dict(name='move_area',source=F,symbol='?MoveArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z',address=0xa028f0,size=163,instructions=66,relocations=2),
 dict(name='validate_resources',source=F,symbol='?ValidateResources@CHandleBasedMemoryPool@@QAEXXZ',address=0xa02ad0,size=84,instructions=30,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-defragment.json',[],
  'CHandleBasedMemoryPool Defragment, InternalGetResource, MoveArea, ValidateResources')
 print('NATIVE_POOL_DEFRAGMENT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
