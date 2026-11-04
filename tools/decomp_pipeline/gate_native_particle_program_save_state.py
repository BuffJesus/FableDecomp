"""Fresh VC71 parity gate: CParticleProgram::SaveStateToInterpolateFrom (0x00ADF5B0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='save_state',source=R/'rebuild/src/compiled/00/ad/CParticleProgram_SaveStateToInterpolateFrom_00adf5b0.cpp',symbol='?SaveStateToInterpolateFrom@CParticleProgram@@QAEXAAVCParticleSimulation@@@Z',address=0xadf5b0,size=37,instructions=16,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-save-state.json',[],
  'CParticleProgram::SaveStateToInterpolateFrom (0x00ADF5B0), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_SAVE_STATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
