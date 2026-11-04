"""Fresh VC71 parity gate: PartialDefragment stuck-area vector append and growth."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@PAVCHeader@CHandleBasedMemoryPool@@V?$allocator@PAVCHeader@CHandleBasedMemoryPool@@@_STL@@@_STL@@QAEXPAPAVCHeader@CHandleBasedMemoryPool@@ABQAV34@ABU__true_type@2@I_N@Z':0xa03780,  # derived from retail site; not in symbol_index
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '_memmove':0xbfeae6,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='push_back',source=F,symbol='?push_back@?$vector@PAVCHeader@CHandleBasedMemoryPool@@V?$allocator@PAVCHeader@CHandleBasedMemoryPool@@@_STL@@@_STL@@QAEXABQAVCHeader@CHandleBasedMemoryPool@@@Z',address=0xa038c0,size=50,instructions=19,relocations=1),
 dict(name='insert_overflow',source=F,symbol='?_M_insert_overflow@?$vector@PAVCHeader@CHandleBasedMemoryPool@@V?$allocator@PAVCHeader@CHandleBasedMemoryPool@@@_STL@@@_STL@@QAEXPAPAVCHeader@CHandleBasedMemoryPool@@ABQAV34@ABU__true_type@2@I_N@Z',address=0xa03780,size=216,instructions=85,relocations=4),
]
def main():
 from gate_native_pool_partial_defragment import compare_stuck_queue
 behavior=compare_stuck_queue()
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-pool-stuck-queue.json',[behavior],
  'PartialDefragment stuck-area vector append and growth')
 print('NATIVE_POOL_STUCK_QUEUE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
