"""Fresh VC71 parity gate: CRenderManager2D::Draw2DBox -> Draw2DBoxWithZ at z 0."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Draw2DBoxWithZ@CRenderManager2D@@QAE_NABVC2DBoxF@@MABVCRGBColour@@K@Z':0x9ddc60,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DBox_009de3f0.cpp'
ROOTS=[
 dict(name='box',source=F,symbol='?Draw2DBox@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z',address=0x9de3f0,size=25,instructions=9,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-draw-box.json',[],
  'CRenderManager2D::Draw2DBox -> Draw2DBoxWithZ at z 0')
 print('NATIVE_RENDER2D_DRAW_BOX_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
