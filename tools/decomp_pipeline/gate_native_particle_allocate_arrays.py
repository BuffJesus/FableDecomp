"""Fresh VC71 parity gate: CParticleSimulation::AllocateArrays (0x00ADF2A0) and CParticleProgram::AllocateArrays (0x00ADF460), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AllocateResource@CMovableResourceMemoryPool@@QAE_NPAJPAVCMovableResource@@KK@Z':0xa62e40,
 '?GetPointerFromHandle@CHandleBasedMemoryPool@@QAEPAXJ@Z':0xa026b0,
}
ROOTS=[
 dict(name='sim_allocate',source=R/'rebuild/src/compiled/00/ad/CParticleSimulation_AllocateArrays_00adf2a0.cpp',symbol='?AllocateArrays@CParticleSimulation@@AAE_NAAVCMovableResourceMemoryPool@@@Z',address=0xadf2a0,size=86,instructions=37,relocations=2),
 dict(name='program_allocate',source=R/'rebuild/src/compiled/00/ad/CParticleProgram_AllocateArrays_00adf460.cpp',symbol='?AllocateArrays@CParticleProgram@@AAE_NKAAVCMovableResourceMemoryPool@@@Z',address=0xadf460,size=101,instructions=41,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-allocate-arrays.json',[],
  'CParticleSimulation::AllocateArrays (0x00ADF2A0) and CParticleProgram::AllocateArrays (0x00ADF460), exact after relocati')
 print('NATIVE_PARTICLE_ALLOCATE_ARRAYS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
