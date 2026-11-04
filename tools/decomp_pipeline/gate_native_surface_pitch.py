"""Fresh VC71 parity gate: CSurface::GetPitch (colour depth x width / 8; GetPixelFormat and GetDimensions expanded)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820,
 '?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,
}
F=R/'rebuild/src/compiled/00/9f/CSurface_GetPitch_009f3390.cpp'
ROOTS=[
 dict(name='pitch',source=F,symbol='?GetPitch@CSurface@@QBEKXZ',address=0x9f3390,size=72,instructions=27,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-surface-pitch.json',[],
  'CSurface::GetPitch (colour depth x width / 8; GetPixelFormat and GetDimensions expanded)')
 print('NATIVE_SURFACE_PITCH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
