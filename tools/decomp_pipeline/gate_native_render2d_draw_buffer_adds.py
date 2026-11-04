"""Fresh VC71 parity gate: CRenderManager2D AddLine/AddTri/AddQuadToDrawBuffer (blend mode 1 -> opaque AddTri, others -> AddAlphaTri; quad = two tris)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?AddTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z':0x9db810,  # derived from retail site; not in symbol_index
 '?AddLine@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@0JPAVCTexture@@@Z':0x9dbae0,  # derived from retail site; not in symbol_index
 '?AddAlphaTri@CRenderManager2D@@AAEXABVCTVertexRHWColSpecTex1@@00JPAVCTexture@@@Z':0x9dbd10,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9d/CRenderManager2D_DrawBufferAdds_009dc4c0.cpp'
ROOTS=[
 dict(name='line',source=F,symbol='?AddLineToDrawBuffer@CRenderManager2D@@QAEXABVCTVertexRHWColSpecTex1@@0KPAVCTexture@@@Z',address=0x9dc4c0,size=26,instructions=6,relocations=1),
 dict(name='tri',source=F,symbol='?AddTriToDrawBuffer@CRenderManager2D@@QAEXABVCTVertexRHWColSpecTex1@@00KPAVCTexture@@@Z',address=0x9dc4e0,size=27,instructions=7,relocations=2),
 dict(name='quad',source=F,symbol='?AddQuadToDrawBuffer@CRenderManager2D@@QAEXABVCTVertexRHWColSpecTex1@@000KPAVCTexture@@@Z',address=0x9dc500,size=119,instructions=55,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-draw-buffer-adds.json',[],
  'CRenderManager2D AddLine/AddTri/AddQuadToDrawBuffer (blend mode 1 -> opaque AddTri, others -> AddAlphaTri; quad = two tris)')
 print('NATIVE_RENDER2D_DRAW_BUFFER_ADDS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
