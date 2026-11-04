"""Fresh VC71 parity gate: CSystemManager GetSize / UninitWindow / InitFromExternalApplicationWindow / UninitFromExternalWindow / GetDebugManager / GetApplicationName."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '__imp__DestroyWindow@4':0x1440340,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9a/CSystemManager_Accessors_009a5010.cpp'
ROOTS=[
 dict(name='get_size',source=F,symbol='?GetSize@CSystemManager@@QBEJXZ',address=0x9a5010,size=6,instructions=2,relocations=0),
 dict(name='init_external',source=F,symbol='?InitFromExternalApplicationWindow@CSystemManager@@AAE_NPAUHWND__@@@Z',address=0x9a5240,size=15,instructions=4,relocations=0),
 dict(name='uninit_external',source=F,symbol='?UninitFromExternalWindow@CSystemManager@@AAEXXZ',address=0x9a5250,size=11,instructions=2,relocations=0),
 dict(name='debug_manager',source=F,symbol='?GetDebugManager@CSystemManager@@QAEPAVCDebugManager@@XZ',address=0x9a52d0,size=4,instructions=2,relocations=0),
 dict(name='application_name',source=F,symbol='?GetApplicationName@CSystemManager@@QBE?AVCCharString@@XZ',address=0x9a5460,size=25,instructions=9,relocations=1),
 dict(name='uninit_window',source=F,symbol='?UninitWindow@CSystemManager@@AAEXXZ',address=0x9a5210,size=41,instructions=12,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-system-manager-accessors.json',[],
  'CSystemManager GetSize / UninitWindow / InitFromExternalApplicationWindow / UninitFromExternalWindow / GetDebugManager / GetApplicationName')
 print('NATIVE_SYSTEM_MANAGER_ACCESSORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
