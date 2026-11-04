"""Fresh VC71 parity gate: C3DClothPrimitive::GetUsedBones / RemapBoneIndices forwarders (0x00AC3AE0, 0x00AC3AF0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetUsedBones@CParticleProgram@@QBEXAAV?$CArray@_N@@@Z':0xae15d0,
 '?RemapBoneIndices@CParticleProgram@@QAEXAAV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@@Z':0xae3a30,
}
ROOTS=[
 dict(name='get_used_bones',source=R/'rebuild/src/compiled/00/ac/C3DClothPrimitive_BoneQueries_00ac3ae0.cpp',symbol='?GetUsedBones@C3DClothPrimitive@@QBEXAAV?$CArray@_N@@@Z',address=0xac3ae0,size=8,instructions=2,relocations=1),
 dict(name='remap_bone_indices',source=R/'rebuild/src/compiled/00/ac/C3DClothPrimitive_BoneQueries_00ac3ae0.cpp',symbol='?RemapBoneIndices@C3DClothPrimitive@@QAEXAAV?$map@KKU?$less@K@_STL@@V?$allocator@U?$pair@$$CBKK@_STL@@@2@@_STL@@@Z',address=0xac3af0,size=8,instructions=2,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-cloth-primitive-bone-queries.json',[],
  'C3DClothPrimitive::GetUsedBones / RemapBoneIndices forwarders (0x00AC3AE0, 0x00AC3AF0), exact after relocation')
 print('NATIVE_CLOTH_PRIMITIVE_BONE_QUERIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
