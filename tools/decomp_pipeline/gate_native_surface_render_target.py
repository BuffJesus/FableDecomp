"""Fresh VC71 parity gate: CSurface::InitialiseAsRenderTarget (release, CreateRenderTarget on the display device, AllocationSource 1)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,
}
F=R/'rebuild/src/compiled/00/9f/CSurface_InitialiseAsRenderTarget_009f3310.cpp'
ROOTS=[
 dict(name='render_target',source=F,symbol='?InitialiseAsRenderTarget@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@_N3@Z',address=0x9f3310,size=115,instructions=45,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-surface-render-target.json',[],
  'CSurface::InitialiseAsRenderTarget (release, CreateRenderTarget on the display device, AllocationSource 1)')
 print('NATIVE_SURFACE_RENDER_TARGET_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
