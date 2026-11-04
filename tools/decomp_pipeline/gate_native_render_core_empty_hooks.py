"""Fresh VC71 parity gate: CRenderManagerCore empty PC hooks: SetDirectionalLight, SetPointLight, DiscardTextureManagerBytes, OnPostViewportChanged."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
F=R/'rebuild/src/compiled/00/a0/CRenderManagerCore_EmptyHooks_00a0a9f0.cpp'
ROOTS=[
 dict(name='directional',source=F,symbol='?SetDirectionalLight@CRenderManagerCore@@QAEXJABVCRGBColour@@0ABVC3DVector@@@Z',address=0xa0a9f0,size=3,instructions=1,relocations=0),
 dict(name='point',source=F,symbol='?SetPointLight@CRenderManagerCore@@QAEXJABVCRGBColour@@0ABVC3DVector@@MMMM@Z',address=0xa0aa00,size=3,instructions=1,relocations=0),
 dict(name='discard',source=F,symbol='?DiscardTextureManagerBytes@CRenderManagerCore@@QAEXK@Z',address=0xa0aa50,size=3,instructions=1,relocations=0),
 dict(name='post_viewport',source=F,symbol='?OnPostViewportChanged@CRenderManagerCore@@QAEXXZ',address=0xa0aac0,size=1,instructions=1,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render-core-empty-hooks.json',[],
  'CRenderManagerCore empty PC hooks: SetDirectionalLight, SetPointLight, DiscardTextureManagerBytes, OnPostViewportChanged')
 print('NATIVE_RENDER_CORE_EMPTY_HOOKS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
