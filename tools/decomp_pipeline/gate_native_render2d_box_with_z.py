"""Fresh VC71 parity gate: CRenderManager2D::Draw2DBoxWithZ (flat box at depth z, two tris, blend from flags/alpha)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z':0x9dc030,
 '?GFVXToAX@@YIMM@Z':0x9e1e10,
 '?GFVYToAY@@YIMM@Z':0x9e1e40,
 '__real@3f800000':0x122ded8,
 '__real@00000000':0x122dedc,
 '__real@bc23d70a':0x1231564,  # derived from retail site; not in symbol_index
 '__real@3c23d70a':0x129b944,  # derived from retail site; not in symbol_index
 '?g_FableVertexReciprocalLimit_0139652C@@3MA':0x139652c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DBoxWithZ_009ddc60.cpp'
ROOTS=[
 dict(name='box_with_z',source=F,symbol='?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z',address=0x9ddc60,size=899,instructions=224,relocations=34),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-box-with-z.json',[],
  'CRenderManager2D::Draw2DBoxWithZ (flat box at depth z, two tris, blend from flags/alpha)')
 print('NATIVE_RENDER2D_BOX_WITH_Z_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
