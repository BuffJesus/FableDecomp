"""Fresh VC71 parity gate: CHandleBasedMemoryPool typed idempotent unlock."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
import pool_access_behavior
ADDRESS={
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Unlock_00a02710.cpp'
ROOTS=[
 dict(name='unlock',source=F,symbol='?Unlock@CHandleBasedMemoryPool@@QAEXJ@Z',address=0xa02710,size=30,instructions=9,relocations=0),
]
def main():
 behavior=pool_access_behavior.verify(ROOTS,ADDRESS,'Unlock',0xa02710,'UNLOCK_OK')
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-unlock.json',[behavior],
  'CHandleBasedMemoryPool typed idempotent unlock')
 print('NATIVE_POOL_UNLOCK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
