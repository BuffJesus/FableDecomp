"""Fresh VC71 parity gate: CParticleSimulation::CParticleSimulation (0x00AE0600), exact after relocation; defined before CParticleProgram's constructor, which expands it"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??_7CParticleSimulation@@6B@':0x129f0c4,
 '?Clear@CParticleSimulation@@QAEXXZ':0xadf1e0,
}
ROOTS=[
 dict(name='simulation_ctor',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_CParticleProgram_00ae3550.cpp',symbol='??0CParticleSimulation@@QAE@XZ',address=0xae0600,size=36,instructions=12,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-simulation-construct.json',[],
  "CParticleSimulation::CParticleSimulation (0x00AE0600), exact after relocation; defined before CParticleProgram's constru")
 print('NATIVE_PARTICLE_SIMULATION_CONSTRUCT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
