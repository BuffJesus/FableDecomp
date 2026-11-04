"""Fresh VC71 parity gate: CParticleSimulation::CopyFrom (0x00ADF610) and GetBoundingBox (0x00ADF730), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AllocateArrays@CParticleSimulation@@AAE_NAAVCMovableResourceMemoryPool@@@Z':0xadf2a0,
}
ROOTS=[
 dict(name='copy_from',source=R/'rebuild/src/compiled/00/ad/CParticleSimulation_CopyFrom_00adf610.cpp',symbol='?CopyFrom@CParticleSimulation@@QAE_NABV1@AAVCMovableResourceMemoryPool@@@Z',address=0xadf610,size=288,instructions=111,relocations=1),
 dict(name='get_bounding_box',source=R/'rebuild/src/compiled/00/ad/CParticleSimulation_CopyFrom_00adf610.cpp',symbol='?GetBoundingBox@CParticleSimulation@@QBE?AVC3DBoundingBox@@XZ',address=0xadf730,size=46,instructions=15,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-simulation-copy-from.json',[],
  'CParticleSimulation::CopyFrom (0x00ADF610) and GetBoundingBox (0x00ADF730), exact after relocation')
 print('NATIVE_PARTICLE_SIMULATION_COPY_FROM_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
