"""Fresh VC71 parity gate: CDisplayManager::IsAntiAliasingModeValid (CheckDeviceMultiSampleType == D3D_OK; format looked up first)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GD3DFormats@@3QBVCD3DFormatInfo@@B':0x129ba40,
}
F=R/'rebuild/src/compiled/00/9b/CDisplayManager_FormatChecks_009be8f0.cpp'
ROOTS=[
 dict(name='antialiasing_mode',source=F,symbol='?IsAntiAliasingModeValid@CDisplayManager@@QAE_NW4ESurfaceMultisampleType@@ABVCPixelFormat@@@Z',address=0x9be3c0,size=53,instructions=24,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-display-antialiasing-mode.json',[],
  'CDisplayManager::IsAntiAliasingModeValid (CheckDeviceMultiSampleType == D3D_OK; format looked up first)')
 print('NATIVE_DISPLAY_ANTIALIASING_MODE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
