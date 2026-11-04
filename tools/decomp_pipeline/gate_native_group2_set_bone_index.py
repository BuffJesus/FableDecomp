"""Fresh VC71 parity gate: C3DGroup2::SetBoneIndex (0x00AC4070), exact"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='set_bone_index',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_SetBoneIndex_00ac4070.cpp',symbol='?SetBoneIndex@C3DGroup2@@QAEXJ@Z',address=0xac4070,size=9,instructions=3,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-group2-set-bone-index.json',[],
  'C3DGroup2::SetBoneIndex (0x00AC4070), exact')
 print('NATIVE_GROUP2_SET_BONE_INDEX_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
