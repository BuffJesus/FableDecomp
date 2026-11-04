"""Fresh VC71 parity gate: CParticleSimulation::Clear (0x00ADF1E0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FreeResource@CMovableResourceMemoryPool@@QAEXJ@Z':0xa62ed0,
 '__real@0000000000000000':0x122ed70,
}
ROOTS=[
 dict(name='simulation_clear',source=R/'rebuild/src/compiled/00/ad/CParticleSimulation_Clear_00adf1e0.cpp',symbol='?Clear@CParticleSimulation@@QAEXXZ',address=0xadf1e0,size=82,instructions=24,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-simulation-clear.json',[],
  'CParticleSimulation::Clear (0x00ADF1E0), exact after relocation')
 print('NATIVE_PARTICLE_SIMULATION_CLEAR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
