"""Fresh VC71 parity gate: C3DGroup2::GetVertexBlends 0x00AC4080 / PeekVertexBlends 0x00AC40A0, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='c3dgroup2_getvertexblends',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_VertexBlends_00ac4080.cpp',symbol='?GetVertexBlends@C3DGroup2@@QAEPAVC3DVertexBlend2@@XZ',address=0xac4080,size=20,instructions=9,relocations=0),
 dict(name='c3dgroup2_peekvertexblends',source=R/'rebuild/src/compiled/00/ac/C3DGroup2_VertexBlends_00ac4080.cpp',symbol='?PeekVertexBlends@C3DGroup2@@QBEPBVC3DVertexBlend2@@XZ',address=0xac40a0,size=20,instructions=9,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-3d-group2-vertex-blends.json',[],
  'C3DGroup2::GetVertexBlends 0x00AC4080 / PeekVertexBlends 0x00AC40A0, exact after relocation')
 print('NATIVE_3D_GROUP2_VERTEX_BLENDS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
