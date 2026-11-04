"""Fresh VC71 parity gate: CEnvironment::UpdateInterpolation (0x006B32A0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FrameUpdate@CEnvironmentSpellEffectInterface@@QAEXXZ':0x885010,
}
ROOTS=[
 dict(name='update_interpolation',source=R/'rebuild/src/compiled/00/6b/CEnvironment_UpdateInterpolation_006b32a0.cpp',symbol='?UpdateInterpolation@CEnvironment@@QAEXXZ',address=0x6b32a0,size=11,instructions=2,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-environment-update-interpolation.json',[],
  'CEnvironment::UpdateInterpolation (0x006B32A0), exact after relocation')
 print('NATIVE_ENVIRONMENT_UPDATE_INTERPOLATION_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
