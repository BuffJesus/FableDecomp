"""Fresh VC71 parity gate: CKeyboardDX::IsKeyPressed 0x00AB6220 (DirectInput key state through the EInputKey map), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='ckeyboarddx_iskeypressed',source=R/'rebuild/src/compiled/00/ab/CKeyboardDX_IsKeyPressed_00ab6220.cpp',symbol='?IsKeyPressed@CKeyboardDX@@UAE_NW4EInputKey@@@Z',address=0xab6220,size=27,instructions=6,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-keyboard-dx-is-key-pressed.json',[],
  'CKeyboardDX::IsKeyPressed 0x00AB6220 (DirectInput key state through the EInputKey map), exact after relocation')
 print('NATIVE_KEYBOARD_DX_IS_KEY_PRESSED_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
