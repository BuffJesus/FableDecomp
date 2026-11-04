"""Fresh VC71 parity gate: CDisplayManager::SetVirtualCoordsAsResolutionIndependant (InitVirtualPixels, or the fixed 800x600 scales floored)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?InitVirtualPixels@CDisplayManager@@AAEXXZ':0x9be980,
 '_floor':0xbfeb8a,  # derived from retail site; not in symbol_index
 '__real@405b4e81b4e81b4f':0x129b3e0,  # derived from retail site; not in symbol_index
 '__real@40547ae147ae147b':0x129b3e8,  # derived from retail site; not in symbol_index
 '?g_FableVirtualWidth_013961E8@@3NA':0x13961e8,
 '?g_FableVirtualHeight_013961F0@@3NA':0x13961f0,
 '?FableVirtualPixelScaleX_013ca800@@3NA':0x13ca800,
 '?FableVirtualPixelScaleY_013ca808@@3NA':0x13ca808,
}
F=R/'rebuild/src/compiled/00/9b/CDisplayManager_SetVirtualCoordsAsResolutionIndependant_009bea10.cpp'
ROOTS=[
 dict(name='virtual_coords',source=F,symbol='?SetVirtualCoordsAsResolutionIndependant@CDisplayManager@@QAEX_N@Z',address=0x9bea10,size=136,instructions=31,relocations=15),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-display-virtual-coords.json',[],
  'CDisplayManager::SetVirtualCoordsAsResolutionIndependant (InitVirtualPixels, or the fixed 800x600 scales floored)')
 print('NATIVE_DISPLAY_VIRTUAL_COORDS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
