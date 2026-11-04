"""Fresh VC71 parity gate for CRenderManager2D::Add2DTri 009DC030."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# 0x99AED0 is pinned as the CWideString default ctor by gfmain-native/display-constructor;
# its manifest CCharString label is a byte-identical fold.
ADDRESS={
 '?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ':0xa0aaa0,
 '__real@3f800000':0x122ded8,
 '?_M_insert_overflow@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@_STL@@@_STL@@QAEXPAVCTVertexRHWColSpecTex1@@ABV3@ABU__false_type@2@I_N@Z':0x9e14b0,
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??4CTexture@@QAEABV0@ABV0@@Z':0x9fa1c0,
 '??0CQuickDrawTriInfo@@QAE@ABV0@@Z':0x9de890,
 '?_M_insert_overflow@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXPAVCQuickDrawTriInfo@@ABV3@ABU__false_type@2@I_N@Z':0x9e1750,
 '?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,
 '??1CWideString@@QAE@XZ':0x99b510,
}
ROOTS=[dict(name='add_2d_tri',source=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Add2DTri_009dc030.cpp',symbol='?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z',address=0x9dc030,size=648,instructions=195,relocations=22)]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-add-tri.json',
  ['work/native-recreation/rendering/009dc030/20261015/native-* triangle-queue compositions (vertex growth, info copy/growth, texture, string, window chains)'],
  'One genuine private root, byte-identical to retail after audited relocation patching. Vector growth, texture assignment/teardown, string and window accessor are separate providers.')
 print('NATIVE_RENDER2D_ADD_TRI_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
