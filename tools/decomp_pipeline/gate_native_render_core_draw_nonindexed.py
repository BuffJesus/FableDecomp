"""Fresh VC71 parity gate: CRenderManagerCore::DrawCurrentBuffersNonIndexed (stage textures, RealiseRenderState, DrawPrimitive)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0,
}
F=R/'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersNonIndexed_00a0ad80.cpp'
ROOTS=[
 dict(name='nonindexed',source=F,symbol='?DrawCurrentBuffersNonIndexed@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKW4EPrimitiveType@@@Z',address=0xa0ad80,size=130,instructions=51,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render-core-draw-nonindexed.json',[],
  'CRenderManagerCore::DrawCurrentBuffersNonIndexed (stage textures, RealiseRenderState, DrawPrimitive)')
 print('NATIVE_RENDER_CORE_DRAW_NONINDEXED_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
