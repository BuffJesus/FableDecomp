"""Fresh VC71 parity gate: CMovableResourceMemoryPool scoped allocation and resource binding."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
import movable_pool_construct_oracle
ADDRESS={
 '?BeginCritical@CHandleBasedMemoryPool@@AAE_N_N@Z':0xa02140,
 '?EndCritical@CHandleBasedMemoryPool@@AAEXXZ':0xa02170,
 '?InternalSetResource@CHandleBasedMemoryPool@@QAEXJPAX@Z':0xa028b0,
 '?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z':0xa03180,
}
F=R/'rebuild/src/compiled/00/a6/CMovableResourceMemoryPool_AllocateResource_00a62e40.cpp'
ROOTS=[
 dict(name='allocate_resource',source=F,symbol='?AllocateResource@CMovableResourceMemoryPool@@QAE_NPAJPAVCMovableResource@@KK@Z',address=0xa62e40,size=98,instructions=43,relocations=5),
]
def main():
 behavior=movable_pool_construct_oracle.compare_allocate(ROOTS,ADDRESS)
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-movable-pool-allocate.json',[behavior],
  'CMovableResourceMemoryPool scoped allocation and resource binding')
 print('NATIVE_MOVABLE_POOL_ALLOCATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
