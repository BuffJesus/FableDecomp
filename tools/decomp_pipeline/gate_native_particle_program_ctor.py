"""Fresh VC71 parity gate: CParticleProgram::CParticleProgram (0x00AE3550), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CMemoryDataOutputStream@@QAE@XZ':0x996af0,
 '??_7CParticleProgram@@6B@':0x129f0d8,
 '??_7CParticleSimulation@@6B@':0x129f0c4,
 '?Clear@CParticleProgram@@QAEXXZ':0xae3360,
 '?Clear@CParticleSimulation@@QAEXXZ':0xadf1e0,
}
ROOTS=[
 dict(name='program_ctor',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_CParticleProgram_00ae3550.cpp',symbol='??0CParticleProgram@@QAE@XZ',address=0xae3550,size=285,instructions=56,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-ctor.json',[],
  'CParticleProgram::CParticleProgram (0x00AE3550), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_CTOR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
