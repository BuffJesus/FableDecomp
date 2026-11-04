"""Fresh VC71 parity gate: CRenderManager2D::Draw2DBoxOutline (four Draw2DLine edges, snapped, right/bottom inset one pixel)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z':0x9dd960,
 '?GFAXToVX@@YIMM@Z':0x9e1da0,
 '?GFAYToVY@@YIMM@Z':0x9e1dd0,
 '__ftol2':0xbfea70,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DBoxOutline_009de140.cpp'
ROOTS=[
 dict(name='box_outline',source=F,symbol='?Draw2DBoxOutline@CRenderManager2D@@QAE_NABVC2DBoxF@@ABVCRGBColour@@K@Z',address=0x9de140,size=337,instructions=110,relocations=10),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-box-outline.json',[],
  'CRenderManager2D::Draw2DBoxOutline (four Draw2DLine edges, snapped, right/bottom inset one pixel)')
 print('NATIVE_RENDER2D_BOX_OUTLINE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
