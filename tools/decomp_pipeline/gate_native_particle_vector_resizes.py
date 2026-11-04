"""Fresh VC71 parity gate: STLport vector resize instantiations used by CParticleProgram (0x00AE7830, 0x00AE7AA0, 0x00AE7B10, 0x00AE7D10, 0x00AE8540), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0C3DGroup2@@QAE@XZ':0xac40c0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?_M_fill_insert@?$vector@U?$pair@VCCharString@@K@_STL@@V?$allocator@U?$pair@VCCharString@@K@_STL@@@2@@_STL@@QAEXPAU?$pair@VCCharString@@K@2@IABU32@@Z':0xae7340,
 '?_M_fill_insert@?$vector@VC2DVector@@V?$allocator@VC2DVector@@@_STL@@@_STL@@QAEXPAVC2DVector@@IABV3@@Z':0x475ed0,
 '?_M_fill_insert@?$vector@VC3DClothRenderVertex@@V?$allocator@VC3DClothRenderVertex@@@_STL@@@_STL@@QAEXPAVC3DClothRenderVertex@@IABV3@@Z':0xae6f90,
 '?_M_fill_insert@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXPAVC3DGroup2@@IABV3@@Z':0xae82c0,
 '?_M_fill_insert@?$vector@VC3DQuad2@@V?$allocator@VC3DQuad2@@@_STL@@@_STL@@QAEXPAVC3DQuad2@@IABV3@@Z':0xae6e60,
 '?erase@?$vector@U?$pair@VCCharString@@K@_STL@@V?$allocator@U?$pair@VCCharString@@K@_STL@@@2@@_STL@@QAEPAU?$pair@VCCharString@@K@2@PAU32@0@Z':0xae6a20,
 '?erase@?$vector@VC2DVector@@V?$allocator@VC2DVector@@@_STL@@@_STL@@QAEPAVC2DVector@@PAV3@0@Z':0x440e00,
 '?erase@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEPAVC3DGroup2@@PAV3@0@Z':0xae7f30,
 '?erase@?$vector@VC3DQuad2@@V?$allocator@VC3DQuad2@@@_STL@@@_STL@@QAEPAVC3DQuad2@@PAV3@0@Z':0xae6430,
 '_free':0xbfea14,
}
ROOTS=[
 dict(name='quad_resize',source=R/'rebuild/src/compiled/00/ae/STLVectorQuad2_resize_00ae7aa0.cpp',symbol='?resize@?$vector@VC3DQuad2@@V?$allocator@VC3DQuad2@@@_STL@@@_STL@@QAEXI@Z',address=0xae7aa0,size=75,instructions=33,relocations=2),
 dict(name='uv_resize',source=R/'rebuild/src/compiled/00/ae/STLVector2DVector_resize_00ae7b10.cpp',symbol='?resize@?$vector@VC2DVector@@V?$allocator@VC2DVector@@@_STL@@@_STL@@QAEXI@Z',address=0xae7b10,size=75,instructions=33,relocations=2),
 dict(name='render_vertex_resize',source=R/'rebuild/src/compiled/00/ae/STLVectorClothRenderVertex_resize_00ae7830.cpp',symbol='?resize@?$vector@VC3DClothRenderVertex@@V?$allocator@VC3DClothRenderVertex@@@_STL@@@_STL@@QAEXIABVC3DClothRenderVertex@@@Z',address=0xae7830,size=76,instructions=35,relocations=1),
 dict(name='export_resize',source=R/'rebuild/src/compiled/00/ae/STLVectorExportParticle_resize_00ae7d10.cpp',symbol='?resize@?$vector@U?$pair@VCCharString@@K@_STL@@V?$allocator@U?$pair@VCCharString@@K@_STL@@@2@@_STL@@QAEXI@Z',address=0xae7d10,size=116,instructions=43,relocations=5),
 dict(name='group2_vector_resize',source=R/'rebuild/src/compiled/00/ae/STLVectorGroup2_resize_00ae8540.cpp',symbol='?resize@?$vector@VC3DGroup2@@V?$allocator@VC3DGroup2@@@_STL@@@_STL@@QAEXI@Z',address=0xae8540,size=101,instructions=43,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-particle-vector-resizes.json',[],
  'STLport vector resize instantiations used by CParticleProgram (0x00AE7830, 0x00AE7AA0, 0x00AE7B10, 0x00AE7D10, 0x00AE854')
 print('NATIVE_PARTICLE_VECTOR_RESIZES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
