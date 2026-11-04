"""Fresh VC71 parity gate: CJoystickDX::SetExclusiveMode 0x00AB6DE0 (release the device, re-enumerate attached game controllers through EnumAndInit), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?EnumAndInit@CJoystickDX@@KGHPBUDIDEVICEINSTANCEW@@PAX@Z':0xab6b10,
}
ROOTS=[
 dict(name='cjoystickdx_setexclusivemode',source=R/'rebuild/src/compiled/00/ab/CJoystickDX_SetExclusiveMode_00ab6de0.cpp',symbol='?SetExclusiveMode@CJoystickDX@@UAE_N_N@Z',address=0xab6de0,size=91,instructions=28,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-joystick-dx-exclusive-mode.json',[],
  'CJoystickDX::SetExclusiveMode 0x00AB6DE0 (release the device, re-enumerate attached game controllers through EnumAndInit')
 print('NATIVE_JOYSTICK_DX_EXCLUSIVE_MODE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
