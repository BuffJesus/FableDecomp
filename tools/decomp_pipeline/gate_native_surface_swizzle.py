"""Fresh VC71 parity gate: CSurface::Swizzle / Unswizzle (empty rect forms; flag forms lock, expand them, unlock); supersede the E2 stand-ins at 9F2E40/9F2E50."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Lock@CSurface@@QAE?AVCSurfaceLockedRect@@K@Z':0x9f33e0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9f/CSurface_Swizzle_009f4330.cpp'
ROOTS=[
 dict(name='swizzle_rect',source=F,symbol='?Swizzle@CSurface@@QAEXAAVCSurfaceLockedRect@@@Z',address=0x9f2e40,size=3,instructions=1,relocations=0),
 dict(name='unswizzle_rect',source=F,symbol='?Unswizzle@CSurface@@QAEXAAVCSurfaceLockedRect@@@Z',address=0x9f2e50,size=3,instructions=1,relocations=0),
 dict(name='swizzle_flags',source=F,symbol='?Swizzle@CSurface@@QAEXK@Z',address=0x9f4330,size=39,instructions=16,relocations=1),
 dict(name='unswizzle_flags',source=F,symbol='?Unswizzle@CSurface@@QAEXK@Z',address=0x9f4360,size=39,instructions=16,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-surface-swizzle.json',[],
  'CSurface::Swizzle / Unswizzle (empty rect forms; flag forms lock, expand them, unlock); supersede the E2 stand-ins at 9F2E40/9F2E50')
 print('NATIVE_SURFACE_SWIZZLE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
