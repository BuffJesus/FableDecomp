"""Fresh VC71 parity gate: CParticleProgram::AddGroup (0x00AE37D0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0C3DGroup2@@QAE@XZ':0xac40c0,
 '?AddVertexBlend@C3DGroup2@@QAEXABVC3DVertexBlend2@@@Z':0xac4110,
 '?SetBoneIndex@C3DGroup2@@QAEXJ@Z':0xac4070,
 '?push_back@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXABVC3DGroup2@@@Z':0xae83d0,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='add_group',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_AddGroup_00ae37d0.cpp',symbol='?AddGroup@CParticleProgram@@QAEXABVC3DGroup2@@@Z',address=0xae37d0,size=332,instructions=107,relocations=10),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-add-group.json',[],
  'CParticleProgram::AddGroup (0x00AE37D0), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_ADD_GROUP_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
