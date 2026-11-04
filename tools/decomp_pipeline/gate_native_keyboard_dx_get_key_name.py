"""Fresh VC71 parity gate: CKeyboardDX::GetKeyName 0x00AB6180 (DIPROP_KEYNAME through the EInputKey map; /GS unit), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??4CWideString@@QAEABV0@PBG@Z':0x99b800,
 '@__security_check_cookie@4':0xbfe9f9,
 '___security_cookie':0x139c8a8,
}
ROOTS=[
 dict(name='ckeyboarddx_getkeyname',source=R/'rebuild/src/compiled/00/ab/CKeyboardDX_GetKeyName_00ab6180.cpp',symbol='?GetKeyName@CKeyboardDX@@UBE_NW4EInputKey@@AAVCWideString@@@Z',address=0xab6180,size=146,instructions=32,relocations=4,flags=['/nologo', '/c', '/O2', '/GS', '/MT', '/W3']),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-keyboard-dx-get-key-name.json',[],
  'CKeyboardDX::GetKeyName 0x00AB6180 (DIPROP_KEYNAME through the EInputKey map; /GS unit), exact after relocation')
 print('NATIVE_KEYBOARD_DX_GET_KEY_NAME_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
