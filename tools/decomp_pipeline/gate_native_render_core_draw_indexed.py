"""Fresh VC71 parity gate: CRenderManagerCore IsRendering + GetCurrentAWindowInt + DrawCurrentBuffersIndexed (both) + AsTriFan + AsPointList."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?RealiseRenderState@CRenderStateManager@@QAEXXZ':0xa058c0,
}
F=R/'rebuild/src/compiled/00/a0/CRenderManagerCore_DrawCurrentBuffersIndexed_00a0aab0.cpp'
ROOTS=[
 dict(name='awindow_int',source=F,symbol='?GetCurrentAWindowInt@CRenderManagerCore@@QBEABVC2DBoxI@@XZ',address=0xa0aab0,size=12,instructions=3,relocations=0),
 dict(name='indexed_textures',source=F,symbol='?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXPAPBVCTexture@@KKKKK@Z',address=0xa0aca0,size=147,instructions=57,relocations=1),
 dict(name='indexed',source=F,symbol='?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXKKKK@Z',address=0xa0ad40,size=61,instructions=23,relocations=1),
 dict(name='tri_fan',source=F,symbol='?DrawCurrentBuffersAsTriFan@CRenderManagerCore@@QAEXJJ@Z',address=0xa0ae40,size=36,instructions=13,relocations=1),
 dict(name='point_list',source=F,symbol='?DrawCurrentBuffersAsPointList@CRenderManagerCore@@QAEXJJ@Z',address=0xa0ae70,size=36,instructions=13,relocations=1),
 dict(name='is_rendering',source=F,symbol='?IsRendering@CRenderManagerCore@@QAE_NXZ',address=0xa0aa60,size=7,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render-core-draw-indexed.json',[],
  'CRenderManagerCore IsRendering + GetCurrentAWindowInt + DrawCurrentBuffersIndexed (both) + AsTriFan + AsPointList')
 print('NATIVE_RENDER_CORE_DRAW_INDEXED_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
