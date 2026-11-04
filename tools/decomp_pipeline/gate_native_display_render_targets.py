"""Fresh VC71 parity gate: CDisplayManager::SetRenderTarget / SetRenderTargetAsBackScreen (adopt surfaces, virtual pixels, bind, full viewport)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?InitVirtualPixels@CDisplayManager@@AAEXXZ':0x9be980,
 '?SetViewport@CDisplayManager@@QAEXABVC2DBoxI@@@Z':0x9bef80,  # derived from retail site; not in symbol_index
 '??4CSurface@@QAEAAV0@ABV0@@Z':0x9f2e70,
 '?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ':0x9f2f90,
}
F=R/'rebuild/src/compiled/00/9b/CDisplayManager_RenderTargets_009bf160.cpp'
ROOTS=[
 dict(name='set_target',source=F,symbol='?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z',address=0x9bf160,size=189,instructions=71,relocations=5),
 dict(name='back_screen',source=F,symbol='?SetRenderTargetAsBackScreen@CDisplayManager@@QAE_NXZ',address=0x9bf220,size=168,instructions=61,relocations=5),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-display-render-targets.json',[],
  'CDisplayManager::SetRenderTarget / SetRenderTargetAsBackScreen (adopt surfaces, virtual pixels, bind, full viewport)')
 print('NATIVE_DISPLAY_RENDER_TARGETS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
