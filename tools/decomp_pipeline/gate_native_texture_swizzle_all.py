"""Fresh VC71 parity gate: CTexture::SwizzleAll / UnswizzleAll (every level; GetSurfaceLevel expanded via FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CSurface@@QAE@ABV0@@Z':0x9f2d60,
 '?Uninitialise@CSurface@@QAEXXZ':0x9f2e20,
 '?InitialiseFromTextureMipmap@CSurface@@QAEXPAUIDirect3DSurface9@@@Z':0x9f2f10,
 '?Swizzle@CSurface@@QAEXK@Z':0x9f4330,
 '?Unswizzle@CSurface@@QAEXK@Z':0x9f4360,
 '??_7CSurface@@6B@':0x122f84c,
}
F=R/'rebuild/src/compiled/00/9f/CTexture_SwizzleAll_009f9fa0.cpp'
ROOTS=[
 dict(name='swizzle_all',source=F,symbol='?SwizzleAll@CTexture@@QAEXJ@Z',address=0x9f9fa0,size=143,instructions=54,relocations=6),
 dict(name='unswizzle_all',source=F,symbol='?UnswizzleAll@CTexture@@QAEXJ@Z',address=0x9fa030,size=143,instructions=54,relocations=6),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-texture-swizzle-all.json',[],
  'CTexture::SwizzleAll / UnswizzleAll (every level; GetSurfaceLevel expanded via FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE)')
 print('NATIVE_TEXTURE_SWIZZLE_ALL_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
