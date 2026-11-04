"""Fresh VC71 parity gate: CConsoleCommandLine private editing members (lib_console2.cpp out-of-line copies; Ego_r AAE/ABE names)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?Insert@CCharString@@QAEXJD@Z':0x99f3d0,  # derived from retail site; not in symbol_index
 '?Erase@CCharString@@QAEXJJ@Z':0x99f400,
}
F=R/'rebuild/src/compiled/00/9e/CConsoleCommandLine_Editing_009ea4b0.cpp'
ROOTS=[
 dict(name='get_string',source=F,symbol='?GetString@CConsoleCommandLine@@ABE?AVCCharString@@XZ',address=0x9ea4b0,size=22,instructions=9,relocations=1),
 dict(name='set_as_user_entry',source=F,symbol='?SetAsUserEntry@CConsoleCommandLine@@AAEXXZ',address=0x9ea4d0,size=5,instructions=2,relocations=0),
 dict(name='is_user_entry',source=F,symbol='?IsUserEntry@CConsoleCommandLine@@AAE_NXZ',address=0x9ea4e0,size=4,instructions=2,relocations=0),
 dict(name='decrement_cursor',source=F,symbol='?DecrementCursorPosition@CConsoleCommandLine@@AAEXXZ',address=0x9ea510,size=12,instructions=6,relocations=0),
 dict(name='get_cursor',source=F,symbol='?GetCursorPosition@CConsoleCommandLine@@AAEJXZ',address=0x9ea520,size=4,instructions=2,relocations=0),
 dict(name='remove_before',source=F,symbol='?RemoveCharBeforeCursor@CConsoleCommandLine@@AAEXXZ',address=0x9ea5e0,size=23,instructions=10,relocations=1),
 dict(name='remove_after',source=F,symbol='?RemoveCharAfterCursor@CConsoleCommandLine@@AAEXXZ',address=0x9ea910,size=31,instructions=13,relocations=1),
 dict(name='set_string',source=F,symbol='?SetString@CConsoleCommandLine@@AAEXABVCCharString@@@Z',address=0x9ea930,size=42,instructions=19,relocations=1),
 dict(name='increment_cursor',source=F,symbol='?IncrementCursorPosition@CConsoleCommandLine@@AAEXXZ',address=0x9ea960,size=26,instructions=12,relocations=0),
 dict(name='add_char_at_cursor',source=F,symbol='?AddCharAtCursor@CConsoleCommandLine@@AAEXJD@Z',address=0x9eab40,size=49,instructions=22,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-command-line-editing.json',[],
  'CConsoleCommandLine private editing members (lib_console2.cpp out-of-line copies; Ego_r AAE/ABE names)')
 print('NATIVE_CONSOLE_COMMAND_LINE_EDITING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
