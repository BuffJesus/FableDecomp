"""Fresh VC71 parity gate: Threaded-file worker entry callback."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?ThreadMain@CThreadedFileManager@@AAEXXZ':0x98f2a0,
 '__imp__GetCurrentThread@0':0x143fdcc,  # derived from retail site; not in symbol_index
 '__imp__SetThreadPriority@8':0x143fdd0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/98/CThreadedFileManager_BeginThread_0098f380.cpp'
ROOTS=[
 dict(name='begin_thread',source=F,symbol='?BeginThread@CThreadedFileManager@@CAXPAX@Z',address=0x98f380,size=24,instructions=6,relocations=3),
]
def main():
 from threaded_worker_exit_oracle import compare_exit
 behavior=compare_exit(bootstrap_source=F)
 assert behavior['cases']==3456 and behavior['missing_provider_checks']==2
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-worker-begin.json',[behavior],
  'Threaded-file worker entry callback')
 print('NATIVE_THREADED_WORKER_BEGIN_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
