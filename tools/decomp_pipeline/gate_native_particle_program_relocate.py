"""Fresh VC71 parity gate: CParticleProgram::RelocateData (0x00AE04C0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='program_relocate',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_RelocateData_00ae04c0.cpp',symbol='?RelocateData@CParticleProgram@@UAEXJPAX0J@Z',address=0xae04c0,size=27,instructions=7,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-relocate.json',[],
  'CParticleProgram::RelocateData (0x00AE04C0), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_RELOCATE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
