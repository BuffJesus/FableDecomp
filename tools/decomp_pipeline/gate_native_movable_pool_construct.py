"""Fresh VC71 parity gate: CMovableResourceMemoryPool constructor and resource binding."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
import movable_pool_construct_oracle
ADDRESS={
 '?InternalSetResource@CHandleBasedMemoryPool@@QAEXJPAX@Z':0xa028b0,
 '??0CHandleBasedMemoryPool@@QAE@XZ':0xa02d20,
 '??_7CMovableResourceMemoryPool@@6B@':0x129d6a0,  # derived from retail site; not in symbol_index
 '??1CHandleBasedMemoryPool@@UAE@XZ':0xa02db0,
 '??3@YAXPAX@Z':0xbfe9bc,
}
F=R/'rebuild/src/compiled/00/a6/CMovableResourceMemoryPool_Construct_00a62e20.cpp'
ROOTS=[
 dict(name='ctor',source=F,symbol='??0CMovableResourceMemoryPool@@QAE@XZ',address=0xa62e20,size=18,instructions=7,relocations=2),
 dict(name='set_resource',source=F,symbol='?SetMovableResource@CMovableResourceMemoryPool@@QAEXJPAVCMovableResource@@@Z',address=0xa62eb0,size=5,instructions=1,relocations=1),
 dict(name='deleting_dtor',source=F,symbol='??_GCMovableResourceMemoryPool@@UAEPAXI@Z',address=0xa62f90,size=30,instructions=11,relocations=2),
]
def main():
 behavior=movable_pool_construct_oracle.compare(ROOTS,ADDRESS)
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-movable-pool-construct.json',[behavior],
  'CMovableResourceMemoryPool constructor and resource binding')
 print('NATIVE_MOVABLE_POOL_CONSTRUCT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
