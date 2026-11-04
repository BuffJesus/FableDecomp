"""Fresh VC71 parity gate: CJoystickDX axes, SetAxes, Update, device number and empty rumble slots 0x00AB6AD0..0x00AB7940, exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?Update@CJoystick@@UAEXXZ':0x9e41b0,
 '?UpdateEvents@CJoystickDX@@IAEXXZ':0xab6e40,
}
ROOTS=[
 dict(name='cjoystickdx_getxaxis',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?GetXAxis@CJoystickDX@@MAEMXZ',address=0xab6ad0,size=7,instructions=2,relocations=0),
 dict(name='cjoystickdx_getyaxis',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?GetYAxis@CJoystickDX@@MAEMXZ',address=0xab6ae0,size=7,instructions=2,relocations=0),
 dict(name='cjoystickdx_getxaxis2',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?GetXAxis2@CJoystickDX@@MAEMXZ',address=0xab6af0,size=7,instructions=2,relocations=0),
 dict(name='cjoystickdx_getyaxis2',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?GetYAxis2@CJoystickDX@@MAEMXZ',address=0xab6b00,size=7,instructions=2,relocations=0),
 dict(name='cjoystickdx_update',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?Update@CJoystickDX@@UAEXXZ',address=0xab78d0,size=16,instructions=6,relocations=2),
 dict(name='cjoystickdx_setaxes',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?SetAxes@CJoystickDX@@UAEXABVC2DVector@@@Z',address=0xab78e0,size=24,instructions=6,relocations=0),
 dict(name='cjoystickdx_getjoystickdevicenumber',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?GetJoystickDeviceNumber@CJoystickDX@@UBEJXZ',address=0xab7900,size=3,instructions=2,relocations=0),
 dict(name='cjoystickdx_getrumblewithid',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?GetRumbleWithID@CJoystickDX@@UAEPAVCActiveRumble@CJoystick@@J@Z',address=0xab7910,size=5,instructions=2,relocations=0),
 dict(name='cjoystickdx_peekrumblewithid',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?PeekRumbleWithID@CJoystickDX@@UBEPBVCActiveRumble@CJoystick@@J@Z',address=0xab7920,size=5,instructions=2,relocations=0),
 dict(name='cjoystickdx_addrumble',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?AddRumble@CJoystickDX@@UAEXJABVCInitRumble@CJoystick@@_N@Z',address=0xab7930,size=3,instructions=1,relocations=0),
 dict(name='cjoystickdx_clearallrumbles',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Overrides_00ab6ad0.cpp',symbol='?ClearAllRumbles@CJoystickDX@@UAEXXZ',address=0xab7940,size=1,instructions=1,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-joystick-dx-overrides.json',[],
  'CJoystickDX axes, SetAxes, Update, device number and empty rumble slots 0x00AB6AD0..0x00AB7940, exact after relocation')
 print('NATIVE_JOYSTICK_DX_OVERRIDES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
