"""Fresh VC71 parity gate: CRenderManager2D::Draw2DCircle (2/3*pi*r Draw2DLine segments)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Draw2DLine@CRenderManager2D@@QAE_NABVC2DVector@@0ABVCRGBColour@@K@Z':0x9dd960,  # derived from retail site; not in symbol_index
 '?GFAXToVX@@YIMM@Z':0x9e1da0,
 '?GFAYToVY@@YIMM@Z':0x9e1dd0,
 '?GFVXToAX@@YIMM@Z':0x9e1e10,
 '?GFVYToAY@@YIMM@Z':0x9e1e40,
 '__ftol2':0xbfea70,  # derived from retail site; not in symbol_index
 '__CIasin':0xbfedc4,  # derived from retail site; not in symbol_index
 '__real@3ff0000000000000':0x1237f00,
 '__real@3f2aaaab':0x12544ac,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_Draw2DCircle_009de2a0.cpp'
ROOTS=[
 dict(name='circle',source=F,symbol='?Draw2DCircle@CRenderManager2D@@QAE_NABVC2DVector@@MABVCRGBColour@@K@Z',address=0x9de2a0,size=328,instructions=104,relocations=12),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-circle.json',[],
  'CRenderManager2D::Draw2DCircle (2/3*pi*r Draw2DLine segments)')
 print('NATIVE_RENDER2D_CIRCLE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
