"""Fresh VC71 parity gate: CRenderManager2D::Draw2DBoxColours (per-corner colours, depth 0)."""
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
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DBoxColours_009dda50.cpp'
ROOTS=[
 dict(name='box_colours',source=F,symbol='?Draw2DBoxColours@CRenderManager2D@@QAE_NABVC2DBoxF@@QBVCRGBColour@@K@Z',address=0x9dda50,size=515,instructions=140,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-box-colours.json',[],
  'CRenderManager2D::Draw2DBoxColours (per-corner colours, depth 0)')
 print('NATIVE_RENDER2D_BOX_COLOURS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
