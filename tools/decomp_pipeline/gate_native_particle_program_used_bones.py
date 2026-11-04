"""Fresh VC71 parity gate: CParticleProgram::GetUsedBones (private static 0x00AE1210, public 0x00AE15D0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetUsedBones@CParticleProgram@@CIXAAV?$CArray@_N@@ABV?$CArray@VC3DGroup2@@@@@Z':0xae1210,
}
ROOTS=[
 dict(name='used_bones_helper',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_GetUsedBones_00ae1210.cpp',symbol='?GetUsedBones@CParticleProgram@@CIXAAV?$CArray@_N@@ABV?$CArray@VC3DGroup2@@@@@Z',address=0xae1210,size=79,instructions=33,relocations=0),
 dict(name='get_used_bones',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_GetUsedBones_00ae1210.cpp',symbol='?GetUsedBones@CParticleProgram@@QBEXAAV?$CArray@_N@@@Z',address=0xae15d0,size=33,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-used-bones.json',[],
  'CParticleProgram::GetUsedBones (private static 0x00AE1210, public 0x00AE15D0), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_USED_BONES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
