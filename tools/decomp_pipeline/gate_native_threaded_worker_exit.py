"""Fresh VC71 parity gate: Threaded-file worker loop and exit cleanup."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ProcessObjects@CThreadedFileManager@@AAEXXZ':0x98ecd0,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '__imp__GlobalFree@4':0x143fdf8,  # derived from retail site; not in symbol_index
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/98/CThreadedFileManager_ThreadMain_0098f2a0.cpp'
ROOTS=[
 dict(name='thread_main',source=F,symbol='?ThreadMain@CThreadedFileManager@@AAEXXZ',address=0x98f2a0,size=176,instructions=67,relocations=5),
]
def main():
 from threaded_worker_exit_oracle import compare_exit
 behavior=compare_exit()
 assert behavior['cases']==1728
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-worker-exit.json',[behavior],
  'Threaded-file worker loop and exit cleanup')
 print('NATIVE_THREADED_WORKER_EXIT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
