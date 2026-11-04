"""Fresh VC71 parity gate: C3DGroup2::Resize (0x00AC41D0), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_fill_insert@?$vector@VC3DVertexBlend2@@V?$allocator@VC3DVertexBlend2@@@_STL@@@_STL@@QAEXPAVC3DVertexBlend2@@IABV3@@Z':0xac42f0,
 '?erase@?$vector@VC3DVertexBlend2@@V?$allocator@VC3DVertexBlend2@@@_STL@@@_STL@@QAEPAVC3DVertexBlend2@@PAV3@0@Z':0xabea40,
}
ROOTS=[
 dict(name='group2_resize',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_Resize_00ac41d0.cpp',symbol='?Resize@C3DGroup2@@QAEXK@Z',address=0xac41d0,size=79,instructions=34,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-group2-resize.json',[],
  'C3DGroup2::Resize (0x00AC41D0), exact after relocation')
 print('NATIVE_GROUP2_RESIZE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
