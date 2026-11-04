"""Fresh VC71 parity gate: CParticleSimulation::ApplyDragging (0x00ADF300), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AlignedSetInterpolate@CMatrix4x4@@QAEXABV1@0M@Z':0xa5cee0,
 '?AlignedTransformTransposedAssumeW@CMatrix4x4@@QBEXPBVC4DVector@@PAV2@KM@Z':0xa5cb90,
 '?Identity@CMatrix4x4@@2V1@A':0x13d2890,
 '__real@3f800000':0x122ded8,
}
ROOTS=[
 dict(name='apply_dragging',source=R/'rebuild/src/compiled/00/ad/CParticleSimulation_ApplyDragging_00adf300.cpp',symbol='?ApplyDragging@CParticleSimulation@@AAEXABVCParticleContext@@@Z',address=0xadf300,size=256,instructions=71,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-simulation-apply-dragging.json',[],
  'CParticleSimulation::ApplyDragging (0x00ADF300), exact after relocation')
 print('NATIVE_PARTICLE_SIMULATION_APPLY_DRAGGING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
