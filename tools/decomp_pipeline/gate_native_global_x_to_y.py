"""Fresh VC71 parity gate: GFXToY / GFYToX (render-target width/height ratio; two display-manager fetches each)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ':0x9bedc0,
}
F=R/'rebuild/src/compiled/00/9e/Global_GFXToY_009e23a0.cpp'
ROOTS=[
 dict(name='x_to_y',source=F,symbol='?GFXToY@@YIMM@Z',address=0x9e23a0,size=70,instructions=24,relocations=4),
 dict(name='y_to_x',source=F,symbol='?GFYToX@@YIMM@Z',address=0x9e23f0,size=64,instructions=23,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-x-to-y.json',[],
  'GFXToY / GFYToX (render-target width/height ratio; two display-manager fetches each)')
 print('NATIVE_GLOBAL_X_TO_Y_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
