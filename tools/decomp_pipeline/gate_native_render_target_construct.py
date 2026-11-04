"""Fresh VC71 parity gate: CRenderTarget(CSurface&, CSurface&) and SetZBufferAsScreenZBuffer (adopts the device depth-stencil)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CBaseClass@@QAE@XZ':0x99a2d0,
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,
 '??4CSurface@@QAEAAV0@ABV0@@Z':0x9f2e70,
 '?InitialiseFromPreallocatedSurface@CSurface@@QAEXPAUIDirect3DSurface9@@_N@Z':0x9f2ec0,
 '??_7CSurface@@6B@':0x122f84c,
 '??_7CRenderTarget@@6B@':0x129c800,
}
F=R/'rebuild/src/compiled/00/a1/CRenderTarget_Construct_00a10dc0.cpp'
ROOTS=[
 dict(name='ctor',source=F,symbol='??0CRenderTarget@@QAE@AAVCSurface@@0@Z',address=0xa10dc0,size=76,instructions=25,relocations=6),
 dict(name='screen_zbuffer',source=F,symbol='?SetZBufferAsScreenZBuffer@CRenderTarget@@QAEXXZ',address=0xa10ec0,size=62,instructions=24,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render-target-construct.json',[],
  'CRenderTarget(CSurface&, CSurface&) and SetZBufferAsScreenZBuffer (adopts the device depth-stencil)')
 print('NATIVE_RENDER_TARGET_CONSTRUCT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
