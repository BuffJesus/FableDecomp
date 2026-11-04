"""Fresh VC71 parity gate: CKeyboardDX::SetHasInputFocus 0x00AB6060 (acquire/unacquire, flush DirectInput's key buffer on re-acquire), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '__chkstk':0xbfea30,
}
ROOTS=[
 dict(name='ckeyboarddx_sethasinputfocus',source=R/'rebuild/src/compiled/00/ab/CKeyboardDX_SetHasInputFocus_00ab6060.cpp',symbol='?SetHasInputFocus@CKeyboardDX@@UAEX_N@Z',address=0xab6060,size=160,instructions=50,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-keyboard-dx-focus.json',[],
  "CKeyboardDX::SetHasInputFocus 0x00AB6060 (acquire/unacquire, flush DirectInput's key buffer on re-acquire), exact after ")
 print('NATIVE_KEYBOARD_DX_FOCUS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
