"""Fresh VC71 parity gate: CJoystickDX SetHasInputFocus 0x00AB6D10 and UpdateAcquired 0x00AB6D60 (DirectInput acquire/unacquire on focus), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?GFGetSystemManager@@YIPAVCSystemManager@@XZ':0x9a4ec0,
 '?IsApplicationActive@CSystemManager@@QAE_NXZ':0x9a57b0,
 '?IsApplicationInputFocus@CSystemManager@@QAE_NXZ':0x9a4fb0,
}
ROOTS=[
 dict(name='cjoystickdx_sethasinputfocus',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Focus_00ab6d10.cpp',symbol='?SetHasInputFocus@CJoystickDX@@UAEX_N@Z',address=0xab6d10,size=70,instructions=25,relocations=0),
 dict(name='cjoystickdx_updateacquired',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_Focus_00ab6d10.cpp',symbol='?UpdateAcquired@CJoystickDX@@QAEXXZ',address=0xab6d60,size=57,instructions=24,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-joystick-dx-focus.json',[],
  'CJoystickDX SetHasInputFocus 0x00AB6D10 and UpdateAcquired 0x00AB6D60 (DirectInput acquire/unacquire on focus), exact af')
 print('NATIVE_JOYSTICK_DX_FOCUS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
