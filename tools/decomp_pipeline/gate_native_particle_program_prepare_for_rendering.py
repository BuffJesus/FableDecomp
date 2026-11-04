"""Fresh VC71 parity gate: CParticleProgram::PrepareForRendering overloads (0x00AE1260 skinned, 0x00AE1600 static), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?DELTA_NOTIONAL_ZERO@@3MB':0x129ba3c,
 '?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@K1KKM@Z':0xa5bba0,
 '?GFInterpolateVectors@NVectorMath@@YAXPAVC4DVector@@KPBV2@KKM@Z':0xa5b870,
 '?GetVectorBuffer@CParticleProgram@@CIPAVC4DVector@@K@Z':0xae0540,
 '?TransformVertices@CParticleProgram@@ABEXPAVC4DVector@@PBV2@KPBVCMatrix4x4@@ABV?$CArray@VC3DGroup2@@@@@Z':0xae0e60,
}
ROOTS=[
 dict(name='prepare_skinned',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_PrepareForRendering_00ae1260.cpp',symbol='?PrepareForRendering@CParticleProgram@@QAEXPAVC4DVector@@AAVCParticleSimulation@@MMABVC3DAnimationBlendState@@2MAAVC3DAnimationManager@@PBVC3DVector@@PBVCMatrix4x4@@ABVC3DMesh2@@@Z',address=0xae1260,size=187,instructions=72,relocations=6),
 dict(name='prepare_static',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_PrepareForRendering_00ae1260.cpp',symbol='?PrepareForRendering@CParticleProgram@@QAEXPAVC4DVector@@AAVCParticleSimulation@@MM@Z',address=0xae1600,size=181,instructions=70,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-prepare-for-rendering.json',[],
  'CParticleProgram::PrepareForRendering overloads (0x00AE1260 skinned, 0x00AE1600 static), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_PREPARE_FOR_RENDERING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
