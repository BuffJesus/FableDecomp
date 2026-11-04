"""Fresh VC71 parity gate: C3DAnimationManager::Initialise / Uninitialise (0x00A9D730, 0x00A9D740), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='initialise',source=R/'rebuild/src/compiled/00/a9/C3DAnimationManager_Initialise_00a9d730.cpp',symbol='?Initialise@C3DAnimationManager@@QAEXPAVCMeshDataBank@@@Z',address=0xa9d730,size=10,instructions=3,relocations=0),
 dict(name='uninitialise',source=R/'rebuild/src/compiled/00/a9/C3DAnimationManager_Initialise_00a9d730.cpp',symbol='?Uninitialise@C3DAnimationManager@@QAEXXZ',address=0xa9d740,size=8,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-animation-manager-bank.json',[],
  'C3DAnimationManager::Initialise / Uninitialise (0x00A9D730, 0x00A9D740), exact after relocation')
 print('NATIVE_ANIMATION_MANAGER_BANK_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
