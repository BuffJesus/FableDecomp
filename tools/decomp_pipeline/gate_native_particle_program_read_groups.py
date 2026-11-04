"""Fresh VC71 parity gate: CParticleProgram::ReadGroups (private static, 0x00AE3A60), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetVertexBlends@C3DGroup2@@QAEPAVC3DVertexBlend2@@XZ':0xac4080,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '?Resize@C3DGroup2@@QAEXK@Z':0xac41d0,
 '?SetBoneIndex@C3DGroup2@@QAEXJ@Z':0xac4070,
 '?resize@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXI@Z':0xae8540,
}
ROOTS=[
 dict(name='read_groups',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_ReadGroups_00ae3a60.cpp',symbol='?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z',address=0xae3a60,size=456,instructions=169,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-read-groups.json',[],
  'CParticleProgram::ReadGroups (private static, 0x00AE3A60), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_READ_GROUPS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
