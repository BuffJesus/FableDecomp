"""Fresh VC71 parity gate: CPlayer::GetJoystickDeviceNumber (0x00487510), exact"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='get_joystick_device_number',source=R/'rebuild/src/compiled/00/48/CPlayer_GetJoystickDeviceNumber_00487510.cpp',symbol='?GetJoystickDeviceNumber@CPlayer@@QBEJXZ',address=0x487510,size=7,instructions=2,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-player-joystick-device-number.json',[],
  'CPlayer::GetJoystickDeviceNumber (0x00487510), exact')
 print('NATIVE_PLAYER_JOYSTICK_DEVICE_NUMBER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
