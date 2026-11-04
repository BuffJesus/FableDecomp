"""Fresh VC71 parity gate: CDisplayManager::GetCurrentWindowedSettings (window outer size, desktop colour depth)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GetColourDepth@CPixelFormat@@QBEKXZ':0x9e3820,
 '?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z':0x9e3830,
 '__imp__GetWindowRect@8':0x1440308,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9b/CDisplayManager_GetCurrentWindowedSettings_009bed10.cpp'
ROOTS=[
 dict(name='windowed',source=F,symbol='?GetCurrentWindowedSettings@CDisplayManager@@AAEXPAUHWND__@@AAVC2DExtentsI@@AAJ@Z',address=0x9bed10,size=103,instructions=35,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-display-windowed-settings.json',[],
  'CDisplayManager::GetCurrentWindowedSettings (window outer size, desktop colour depth)')
 print('NATIVE_DISPLAY_WINDOWED_SETTINGS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
