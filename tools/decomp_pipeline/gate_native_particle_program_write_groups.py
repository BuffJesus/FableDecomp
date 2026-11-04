"""Fresh VC71 parity gate: CParticleProgram::WriteGroups (0x00AE0680), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?PeekVertexBlends@C3DGroup2@@QBEPBVC3DVertexBlend2@@XZ':0xac40a0,
 '?WriteULONG@CDataOutputStream@@QAEXK@Z':0x993ea0,
}
ROOTS=[
 dict(name='write_groups',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_WriteGroups_00ae0680.cpp',symbol='?WriteGroups@CParticleProgram@@CIXAAVCDataOutputStream@@ABV?$CArray@VC3DGroup2@@@@@Z',address=0xae0680,size=133,instructions=53,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-write-groups.json',[],
  'CParticleProgram::WriteGroups (0x00AE0680), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_WRITE_GROUPS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
