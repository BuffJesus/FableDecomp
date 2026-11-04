"""Fresh VC71 parity gate: CMouseDX SetPos 0x00AB4700, SetHasInputFocus 0x00AB47D0, UpdateAcquired 0x00AB4830, PeekScreenPos 0x00AB4870 (Windows cursor placement, DirectInput acquire/unacquire), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?IsApplicationActive@CSystemManager@@QAE_NXZ':0x9a57b0,
 '?IsApplicationInputFocus@CSystemManager@@QAE_NXZ':0x9a4fb0,
 '__ftol2':0xbfea70,
 '__imp__ClientToScreen@8':0x1440304,
 '__imp__GetClientRect@8':0x1440338,
 '__imp__OffsetRect@12':0x14403a4,
 '__imp__SetCursorPos@8':0x14403a0,
}
ROOTS=[
 dict(name='cmousedx_setpos',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Focus_00ab4700.cpp',symbol='?SetPos@CMouseDX@@UAEXPBVC2DVector@@@Z',address=0xab4700,size=174,instructions=49,relocations=7),
 dict(name='cmousedx_sethasinputfocus',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Focus_00ab4700.cpp',symbol='?SetHasInputFocus@CMouseDX@@UAEX_N@Z',address=0xab47d0,size=83,instructions=31,relocations=0),
 dict(name='cmousedx_updateacquired',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Focus_00ab4700.cpp',symbol='?UpdateAcquired@CMouseDX@@QAEXXZ',address=0xab4830,size=57,instructions=24,relocations=4),
 dict(name='cmousedx_peekscreenpos',source=R/'rebuild/src/compiled/00/ab/CMouseDX_Focus_00ab4700.cpp',symbol='?PeekScreenPos@CMouseDX@@UBE?AVC2DVector@@XZ',address=0xab4870,size=154,instructions=45,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mouse-dx-focus.json',[],
  'CMouseDX SetPos 0x00AB4700, SetHasInputFocus 0x00AB47D0, UpdateAcquired 0x00AB4830, PeekScreenPos 0x00AB4870 (Windows cu')
 print('NATIVE_MOUSE_DX_FOCUS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
