"""Fresh VC71 parity gate: GFACoordToVCoord (C2DVector and C2DCoordI); extents read through the returned pointer."""
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
F=R/'rebuild/src/compiled/00/9e/Global_GFACoordToVCoord_009e21d0.cpp'
ROOTS=[
 dict(name='acoord_to_vcoord',source=F,symbol='?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z',address=0x9e21d0,size=106,instructions=35,relocations=6),
 dict(name='acoord_to_vcoord_i',source=F,symbol='?GFACoordToVCoord@@YIXABVC2DCoordI@@AAV1@@Z',address=0x9e2240,size=116,instructions=37,relocations=8),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-actual-to-virtual-coord.json',[],
  'GFACoordToVCoord (C2DVector and C2DCoordI); extents read through the returned pointer')
 print('NATIVE_GLOBAL_ACTUAL_TO_VIRTUAL_COORD_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
