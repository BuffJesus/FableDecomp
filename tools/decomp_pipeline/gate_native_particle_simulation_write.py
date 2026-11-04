"""Fresh VC71 parity gate: CParticleSimulation::WriteToFile (0x00AE2050), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?WriteEBOOL@CDataOutputStream@@QAEX_N@Z':0x993e30,
 '?WriteFloat@CDataOutputStream@@QAEXM@Z':0x993ee0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
 '?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z':0xae18c0,
}
ROOTS=[
 dict(name='simulation_write',source=R/'rebuild/src/compiled/00/ae/CParticleSimulation_WriteToFile_00ae2050.cpp',symbol='?WriteToFile@CParticleSimulation@@QBEXAAVCDataOutputStream@@@Z',address=0xae2050,size=171,instructions=66,relocations=12),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-simulation-write.json',[],
  'CParticleSimulation::WriteToFile (0x00AE2050), exact after relocation')
 print('NATIVE_PARTICLE_SIMULATION_WRITE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
