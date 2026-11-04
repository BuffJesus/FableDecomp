"""Fresh VC71 parity gate: CRenderManager2D::Draw2DTriangle (flat triangle, depth 0, blend from flags/alpha)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Add2DTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z':0x9dc030,
 '?GFVXToAX@@YIMM@Z':0x9e1e10,
 '?GFVYToAY@@YIMM@Z':0x9e1e40,
 '?g_FableVertexReciprocalLimit_0139652C@@3MA':0x139652c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DTriangle_009ddff0.cpp'
ROOTS=[
 dict(name='triangle',source=F,symbol='?Draw2DTriangle@CRenderManager2D@@QAE_NABVC2DVector@@00ABVCRGBColour@@K@Z',address=0x9ddff0,size=329,instructions=91,relocations=10),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-triangle.json',[],
  'CRenderManager2D::Draw2DTriangle (flat triangle, depth 0, blend from flags/alpha)')
 print('NATIVE_RENDER2D_TRIANGLE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
