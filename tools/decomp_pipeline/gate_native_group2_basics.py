"""Fresh VC71 parity gate: C3DGroup2 default ctor (0x00AC40C0) and AddVertexBlend (0x00AC4110), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@VC3DVertexBlend2@@V?$allocator@VC3DVertexBlend2@@@_STL@@@_STL@@QAEXPAVC3DVertexBlend2@@ABV3@ABU__false_type@2@I_N@Z':0xabeb40,
}
ROOTS=[
 dict(name='group2_default_ctor',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_Basics_00ac40c0.cpp',symbol='??0C3DGroup2@@QAE@XZ',address=0xac40c0,size=20,instructions=7,relocations=0),
 dict(name='group2_add_vertex_blend',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_Basics_00ac40c0.cpp',symbol='?AddVertexBlend@C3DGroup2@@QAEXABVC3DVertexBlend2@@@Z',address=0xac4110,size=63,instructions=25,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-group2-basics.json',[],
  'C3DGroup2 default ctor (0x00AC40C0) and AddVertexBlend (0x00AC4110), exact after relocation')
 print('NATIVE_GROUP2_BASICS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
