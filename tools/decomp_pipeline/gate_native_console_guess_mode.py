"""Fresh VC71 parity gate: CConsole guess mode: SetGuessMode / NextCommandGuess (lower_bound on the lower-cased entry, StartsWith via strncmp, the unchanged-entry test via _stricmp) and the input-map iterator increment."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GetAsLowerCase@CCharString@@QBE?AV1@XZ':0x99ed10,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?SetGuessMode@CConsole@@AAEXXZ':0x9eaff0,
 '??E?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@_STL@@QAEAAU01@XZ':0x9ee470,
 '??0CConsoleCommandLine@@QAE@ABVCCharString@@@Z':0x9eea10,
 '?_M_lower_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@@_STL@@QBEPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@2@ABVCCharString@@@Z':0x9eec10,
 '_strncmp':0xbfeaf8,  # derived from retail site; not in symbol_index
 '__stricmp':0xbfeba8,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsole_GuessMode_009eaff0.cpp'
ROOTS=[
 dict(name='set_guess_mode',source=F,symbol='?SetGuessMode@CConsole@@AAEXXZ',address=0x9eaff0,size=221,instructions=73,relocations=13),
 dict(name='next_command_guess',source=F,symbol='?NextCommandGuess@CConsole@@AAEXXZ',address=0x9eb0d0,size=327,instructions=114,relocations=21),
 dict(name='input_iterator_increment',source=F,symbol='??E?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@_STL@@QAEAAU01@XZ',address=0x9ee470,size=62,instructions=30,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-guess-mode.json',[],
  'CConsole guess mode: SetGuessMode / NextCommandGuess (lower_bound on the lower-cased entry, StartsWith via strncmp, the unchanged-entry test via _stricmp) and the input-map iterator increment')
 print('NATIVE_CONSOLE_GUESS_MODE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
