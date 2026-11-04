"""Fresh VC71 parity gate: Threaded-file manager shutdown."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '__imp__SetEvent@4':0x143fde0,  # derived from retail site; not in symbol_index
 '__imp__DeleteCriticalSection@4':0x143fde4,  # derived from retail site; not in symbol_index
 '__imp__EnterCriticalSection@4':0x143fe0c,  # derived from retail site; not in symbol_index
 '__imp__LeaveCriticalSection@4':0x143fe10,  # derived from retail site; not in symbol_index
 '__imp__Sleep@4':0x143fe1c,  # derived from retail site; not in symbol_index
 '__imp__CloseHandle@4':0x143fe30,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/98/CThreadedFileManager_Uninitialise_0098e0d0.cpp'
ROOTS=[
 dict(name='uninitialise',source=F,symbol='?Uninitialise@CThreadedFileManager@@QAEXXZ',address=0x98e0d0,size=150,instructions=46,relocations=7),
]
def main():
 from threaded_shutdown_oracle import compare_shutdown
 behavior=compare_shutdown()
 assert behavior['cases']==384 and behavior['checkpoints']==768
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-manager-shutdown.json',[behavior],
  'Threaded-file manager shutdown')
 print('NATIVE_THREADED_MANAGER_SHUTDOWN_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
