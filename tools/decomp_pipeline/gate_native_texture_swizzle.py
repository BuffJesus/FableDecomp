"""Fresh VC71 parity gate: CTexture::Swizzle / Unswizzle forwarding to the level surface (temporary CSurface destroyed inline)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,
 '?Swizzle@CSurface@@QAEXAAVCSurfaceLockedRect@@@Z':0x9f2e40,  # derived from retail site; not in symbol_index
 '?Unswizzle@CSurface@@QAEXAAVCSurfaceLockedRect@@@Z':0x9f2e50,  # derived from retail site; not in symbol_index
 '?Swizzle@CSurface@@QAEXK@Z':0x9f4330,  # derived from retail site; not in symbol_index
 '?Unswizzle@CSurface@@QAEXK@Z':0x9f4360,  # derived from retail site; not in symbol_index
 '?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z':0x9f9e00,
 '??_7CSurface@@6B@':0x122f84c,
}
F=R/'rebuild/src/compiled/00/9f/CTexture_Swizzle_009fa0c0.cpp'
ROOTS=[
 dict(name='swizzle_flags',source=F,symbol='?Swizzle@CTexture@@QAEXJJ@Z',address=0x9fa0c0,size=51,instructions=15,relocations=4),
 dict(name='unswizzle_flags',source=F,symbol='?Unswizzle@CTexture@@QAEXJJ@Z',address=0x9fa100,size=51,instructions=15,relocations=4),
 dict(name='swizzle_rect',source=F,symbol='?Swizzle@CTexture@@QAEXJAAVCSurfaceLockedRect@@@Z',address=0x9fa140,size=51,instructions=15,relocations=4),
 dict(name='unswizzle_rect',source=F,symbol='?Unswizzle@CTexture@@QAEXJAAVCSurfaceLockedRect@@@Z',address=0x9fa180,size=51,instructions=15,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-texture-swizzle.json',[],
  'CTexture::Swizzle / Unswizzle forwarding to the level surface (temporary CSurface destroyed inline)')
 print('NATIVE_TEXTURE_SWIZZLE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
