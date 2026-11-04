"""Fresh VC71 parity gate: CHandleBasedMemoryPool CHeader ctor, GetHandleFromPointer, CreateArea, FindFreeArea, FindLargestFreeArea, Update/ValidateLargestFreeArea."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='header_ctor',source=F,symbol='??0CHeader@CHandleBasedMemoryPool@@QAE@XZ',address=0xa01d20,size=32,instructions=12,relocations=0),
 dict(name='handle_from_pointer',source=F,symbol='?GetHandleFromPointer@CHandleBasedMemoryPool@@QAEJPAX@Z',address=0xa01d40,size=10,instructions=3,relocations=0),
 dict(name='create_area',source=F,symbol='?CreateArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@PAXKPAV21@1@Z',address=0xa01d50,size=65,instructions=19,relocations=0),
 dict(name='find_free_area',source=F,symbol='?FindFreeArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@J@Z',address=0xa01ed0,size=122,instructions=48,relocations=0),
 dict(name='find_largest_free_area',source=F,symbol='?FindLargestFreeArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@XZ',address=0xa01f50,size=47,instructions=17,relocations=0),
 dict(name='update_largest_free_area',source=F,symbol='?UpdateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ',address=0xa01f80,size=85,instructions=34,relocations=0),
 dict(name='validate_largest_free_area',source=F,symbol='?ValidateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ',address=0xa01fe0,size=57,instructions=23,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-areas.json',[],
  'CHandleBasedMemoryPool CHeader ctor, GetHandleFromPointer, CreateArea, FindFreeArea, FindLargestFreeArea, Update/ValidateLargestFreeArea')
 print('NATIVE_POOL_AREAS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
