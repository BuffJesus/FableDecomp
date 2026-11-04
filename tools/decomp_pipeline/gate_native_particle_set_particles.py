"""Fresh VC71 parity gate: CParticleSimulation::SetParticles (0x00AE1030) and CParticleProgram::SetData (0x00AE1150), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AllocateArrays@CParticleProgram@@AAE_NKAAVCMovableResourceMemoryPool@@@Z':0xadf460,
 '?AllocateArrays@CParticleSimulation@@AAE_NAAVCMovableResourceMemoryPool@@@Z':0xadf2a0,
}
ROOTS=[
 dict(name='set_particles',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_SetParticles_00ae1030.cpp',symbol='?SetParticles@CParticleSimulation@@QAEXABV?$CArray@VC3DVector@@@@ABV?$CArray@M@@AAVCMovableResourceMemoryPool@@@Z',address=0xae1030,size=286,instructions=98,relocations=1),
 dict(name='set_data',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_SetParticles_00ae1030.cpp',symbol='?SetData@CParticleProgram@@QAE_NABV?$CArray@VC3DVector@@@@AAVCMovableResourceMemoryPool@@@Z',address=0xae1150,size=177,instructions=67,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-set-particles.json',[],
  'CParticleSimulation::SetParticles (0x00AE1030) and CParticleProgram::SetData (0x00AE1150), exact after relocation')
 print('NATIVE_PARTICLE_SET_PARTICLES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
