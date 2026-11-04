"""Fresh VC71 parity gate: CParticleSimulation::RelocateData (0x00ADFC60), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='simulation_relocate',source=R/'rebuild/src/compiled/00/ad/CParticleSimulation_RelocateData_00adfc60.cpp',symbol='?RelocateData@CParticleSimulation@@UAEXJPAX0J@Z',address=0xadfc60,size=83,instructions=35,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-simulation-relocate.json',[],
  'CParticleSimulation::RelocateData (0x00ADFC60), exact after relocation')
 print('NATIVE_PARTICLE_SIMULATION_RELOCATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
