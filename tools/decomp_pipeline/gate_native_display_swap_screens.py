"""Fresh VC71 parity gate: CDisplayManager::SwapScreens (Present, lost-device flag, screen index) and FlipToGDISurface."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?OnPostSwapScreens@CVsyncCallbackTimer@@QAEXXZ':0x9f73c0,
}
F=R/'rebuild/src/compiled/00/9b/CDisplayManager_SwapScreens_009beeb0.cpp'
ROOTS=[
 dict(name='swap',source=F,symbol='?SwapScreens@CDisplayManager@@QAE_NXZ',address=0x9beeb0,size=97,instructions=36,relocations=1),
 dict(name='flip_gdi',source=F,symbol='?FlipToGDISurface@CDisplayManager@@QAEXXZ',address=0x9bf2d0,size=128,instructions=41,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-display-swap-screens.json',[],
  'CDisplayManager::SwapScreens (Present, lost-device flag, screen index) and FlipToGDISurface')
 print('NATIVE_DISPLAY_SWAP_SCREENS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
