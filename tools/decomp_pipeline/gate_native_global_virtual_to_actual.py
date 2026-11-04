"""Fresh VC71 parity gate: lib_global_tools.cpp virtual-to-actual conversions GFVXToAX / GFVYToAY, GFVCoordToACoord (C2DVector and C2DCoordI), GFVBoxToABox (C2DBoxF and C2DBoxI, the latter fimul on the integer corner)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ':0x9bedc0,
 '__ftol2':0xbfea70,  # derived from retail site; not in symbol_index
 '?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,
 '?g_FableVirtualHeight_013961F0@@3NA':0x13961f0,
}
F=R/'rebuild/src/compiled/00/9e/Global_GFVirtualToActualScalars_009e1e10.cpp'
ROOTS=[
 dict(name='vx_to_ax',source=F,symbol='?GFVXToAX@@YIMM@Z',address=0x9e1e10,size=40,instructions=12,relocations=3),
 dict(name='vy_to_ay',source=F,symbol='?GFVYToAY@@YIMM@Z',address=0x9e1e40,size=41,instructions=12,relocations=3),
 dict(name='vbox_to_abox',source=F,symbol='?GFVBoxToABox@@YIXPBVC2DBoxF@@PAV1@@Z',address=0x9e1e70,size=209,instructions=65,relocations=12),
 dict(name='vcoord_to_acoord',source=F,symbol='?GFVCoordToACoord@@YIXABVC2DVector@@AAV1@@Z',address=0x9e22c0,size=98,instructions=33,relocations=6),
 dict(name='vbox_to_abox_i',source=F,symbol='?GFVBoxToABox@@YIXABVC2DBoxI@@AAV1@@Z',address=0x9e1f50,size=203,instructions=61,relocations=16),
 dict(name='vcoord_to_acoord_i',source=F,symbol='?GFVCoordToACoord@@YIXABVC2DCoordI@@AAV1@@Z',address=0x9e2330,size=108,instructions=35,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-virtual-to-actual.json',[],
  'lib_global_tools.cpp virtual-to-actual conversions GFVXToAX / GFVYToAY, GFVCoordToACoord (C2DVector and C2DCoordI), GFVBoxToABox (C2DBoxF and C2DBoxI, the latter fimul on the integer corner)')
 print('NATIVE_GLOBAL_VIRTUAL_TO_ACTUAL_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
