"""Fresh VC71 parity gate: CRenderManager2D::Add2DLine (clip-space line vertices + line tri-info)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CQuickDrawTriInfo@@QAE@ABV0@@Z':0x9de890,
 '?_M_insert_overflow@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@_STL@@@_STL@@QAEXPAVCTVertexRHWColSpecTex1@@ABV3@ABU__false_type@2@I_N@Z':0x9e14b0,
 '?_M_insert_overflow@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@_STL@@@_STL@@QAEXPAVCQuickDrawTriInfo@@ABV3@ABU__false_type@2@I_N@Z':0x9e1750,
 '?Uninitialise@CTexture@@QAEXXZ':0x9f9f70,
 '??4CTexture@@QAEABV0@ABV0@@Z':0x9fa1c0,
 '?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ':0xa0aaa0,
 '__real@3f800000':0x122ded8,
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Add2DLine_009dc2c0.cpp'
ROOTS=[
 dict(name='add2dline',source=F,symbol='?Add2DLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z',address=0x9dc2c0,size=509,instructions=153,relocations=17),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-add2dline.json',[],
  'CRenderManager2D::Add2DLine (clip-space line vertices + line tri-info)')
 print('NATIVE_RENDER2D_ADD2DLINE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
