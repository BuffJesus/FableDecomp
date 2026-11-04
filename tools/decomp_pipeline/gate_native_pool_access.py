"""Fresh VC71 parity gate: CHandleBasedMemoryPool typed handle payload accessor."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
import pool_access_behavior
ADDRESS={
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_GetPointerFromHandle_00a026b0.cpp'
ROOTS=[
 dict(name='get_pointer',source=F,symbol='?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z',address=0xa026b0,size=19,instructions=5,relocations=0),
 dict(name='get_size',source=F,symbol='?GetSizeFromHandle@CHandleBasedMemoryPool@@QAEKJ@Z',address=0xa026d0,size=19,instructions=5,relocations=0),
]
def main():
 behavior=pool_access_behavior.verify([ROOTS[0]],ADDRESS,'GetPointerFromHandle',0xa026b0,'GPFH_00a026b0_OK')
 size_behavior=pool_access_behavior.verify([ROOTS[1]],ADDRESS,'GetSizeFromHandle',0xa026d0,'POOL_SIZE_FROM_HANDLE_OK')
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-access.json',[behavior,size_behavior],
  'CHandleBasedMemoryPool typed handle payload accessor')
 print('NATIVE_POOL_ACCESS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
