"""Fresh VC71 parity gate: CParticleProgram::ReserveGroups / RemapBoneIndices (0x00AE3330, 0x00AE3A30), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RemapBoneIndices@CParticleProgram@@CIXAAV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@AAV?$CArray@VC3DGroup2@@@@@Z':0xae3920,
 '?reserve@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXI@Z':0xae80f0,
}
ROOTS=[
 dict(name='reserve_groups',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_GroupLists_00ae3330.cpp',symbol='?ReserveGroups@CParticleProgram@@QAEXK@Z',address=0xae3330,size=37,instructions=13,relocations=2),
 dict(name='remap_bone_indices',source=R/'rebuild/src/compiled/00/ae/CParticleProgram_GroupLists_00ae3330.cpp',symbol='?RemapBoneIndices@CParticleProgram@@QAEXAAV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@@Z',address=0xae3a30,size=39,instructions=13,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-program-group-lists.json',[],
  'CParticleProgram::ReserveGroups / RemapBoneIndices (0x00AE3330, 0x00AE3A30), exact after relocation')
 print('NATIVE_PARTICLE_PROGRAM_GROUP_LISTS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
