"""Fresh VC71 parity gate: CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData (0x00AB3640), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CBoneInfo@C3DAnimationBuilder@@QAE@XZ':0xab34c0,
 '??3@YAXPAX@Z':0xbfe9bc,
}
ROOTS=[
 dict(name='deletedata_cboneinfo',source=R/'rebuild/src/compiled/00/ab/CCountedPointer_BoneInfoDeleteData_00ab3640.cpp',symbol='?DeleteData@?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@CIXPAX@Z',address=0xab3640,size=23,instructions=10,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-bone-info-delete-data.json',[],
  'CCountedPointer<C3DAnimationBuilder::CBoneInfo>::DeleteData (0x00AB3640), exact after relocation')
 print('NATIVE_BONE_INFO_DELETE_DATA_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
