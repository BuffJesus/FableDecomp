"""Fresh VC71 parity gate: STLport CHandleInfo handle-table growth used by InternalAllocateHandle."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='handle_growth',source=F,symbol='?_M_insert_overflow@?$vector@VCHandleInfo@CHandleBasedMemoryPool@@V?$allocator@VCHandleInfo@CHandleBasedMemoryPool@@@_STL@@@_STL@@QAEXPAVCHandleInfo@CHandleBasedMemoryPool@@ABV34@ABU__false_type@2@I_N@Z',address=0xa03900,size=227,instructions=96,relocations=2),
]
def main():
 from gate_native_movable_pool_lifecycle import compare_handle_growth
 behavior=compare_handle_growth()
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-handle-growth.json',[behavior],
  'STLport CHandleInfo handle-table growth used by InternalAllocateHandle')
 print('NATIVE_POOL_HANDLE_GROWTH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
