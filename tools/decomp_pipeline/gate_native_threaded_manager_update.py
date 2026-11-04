"""Fresh VC71 parity gate: Threaded-file manager time slices and worker wake."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '__imp__SleepEx@8':0x143fddc,  # derived from retail site; not in symbol_index
 '__imp__SetEvent@4':0x143fde0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/98/CThreadedFileManager_SetDeviceTimeSlice_0098e170.cpp'
ROOTS=[
 dict(name='set_device_time_slice',source=F,symbol='?SetDeviceTimeSlice@CThreadedFileManager@@QAEXW4EThreadedFileDeviceID@@M@Z',address=0x98e170,size=18,instructions=4,relocations=0),
 dict(name='keep_updating',source=F,symbol='?KeepUpdating@CThreadedFileManager@@QAEXXZ',address=0x98e190,size=24,instructions=7,relocations=2),
]
def main():
 from threaded_update_oracle import compare_update
 behavior=compare_update(ROOTS,ADDRESS)
 assert behavior['cases']==120
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-threaded-manager-update.json',[behavior],
  'Threaded-file manager time slices and worker wake')
 print('NATIVE_THREADED_MANAGER_UPDATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
