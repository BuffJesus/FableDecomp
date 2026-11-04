"""Fresh VC71 parity gate: GFABoxToVBox (C2DBoxF and C2DBoxI); extents read through the returned pointer."""
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
F=R/'rebuild/src/compiled/00/9e/Global_GFABoxToVBox_009e2020.cpp'
ROOTS=[
 dict(name='abox_to_vbox',source=F,symbol='?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z',address=0x9e2020,size=199,instructions=61,relocations=12),
 dict(name='abox_to_vbox_i',source=F,symbol='?GFABoxToVBox@@YIXABVC2DBoxI@@AAV1@@Z',address=0x9e20f0,size=219,instructions=65,relocations=16),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-global-actual-to-virtual-box.json',[],
  'GFABoxToVBox (C2DBoxF and C2DBoxI); extents read through the returned pointer')
 print('NATIVE_GLOBAL_ACTUAL_TO_VIRTUAL_BOX_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
