"""Fresh VC71 parity gate: C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk&) (0x00AC4150), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetBoneIndex@C3DMeshFileGroupChunk@@QBEJXZ':0xab9f60,
 '?GetCount@C3DMeshFileGroupChunk@@QBEKXZ':0xaba350,
 '?GetList@C3DMeshFileGroupChunk@@QBEPBVC3DVertexBlend2@@XZ':0xabc370,
 '?_M_insert_overflow@?$vector@VC3DVertexBlend2@@V?$allocator@VC3DVertexBlend2@@@_STL@@@_STL@@QAEXPAVC3DVertexBlend2@@ABV3@ABU__false_type@2@I_N@Z':0xabeb40,
}
ROOTS=[
 dict(name='group2_ctor',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_Construct_00ac4150.cpp',symbol='??0C3DGroup2@@QAE@ABVC3DMeshFileGroupChunk@@@Z',address=0xac4150,size=116,instructions=50,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-group2-construct.json',[],
  'C3DGroup2::C3DGroup2(const C3DMeshFileGroupChunk&) (0x00AC4150), exact after relocation')
 print('NATIVE_GROUP2_CONSTRUCT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
