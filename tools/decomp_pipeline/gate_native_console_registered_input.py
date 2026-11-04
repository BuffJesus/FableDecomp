"""Fresh VC71 parity gate: CConsole::GetRegisteredConsoleInput (Ego_r parameter key; lower-cased lookup) and RunCommandLine (Ego_r locals parser/parsed_item/input_name; the lookup expanded; explicit CCharString("") parser filename temporary)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1?$list@VCStringParserCommentBoundaries@@@std@@QAE@XZ':0x4046b0,
 '??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z':0x404720,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GetAsLowerCase@CCharString@@QBE?AV1@XZ':0x99ed10,
 '??H@YI?AVCCharString@@PBDABV0@@Z':0x99f690,
 '?GetString@CParsedItem@@QBEABVCCharString@@XZ':0x9b9880,
 '?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,
 '?SendMessageToUser@CConsole@@QAEXABVCCharString@@@Z':0x9eb260,
 '?_M_find@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@_STL@@@3@@_STL@@QAEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z':0x9eed20,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_0BO@NLKNOGFF@command?5line?5was?5not?5a?5string?$AA@':0x129c4a8,  # derived from retail site; not in symbol_index
 '??_C@_0BB@NLLMGGNE@unknown?5input?5?9?5?$AA@':0x129c4c8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsole_GetRegisteredConsoleInput_009eb220.cpp'
ROOTS=[
 dict(name='get_registered_console_input',source=F,symbol='?GetRegisteredConsoleInput@CConsole@@AAEPAVCConsoleInputBase@@ABVCCharString@@@Z',address=0x9eb220,size=59,instructions=24,relocations=3),
 dict(name='run_command_line',source=F,symbol='?RunCommandLine@CConsole@@AAEXXZ',address=0x9eb430,size=345,instructions=105,relocations=28),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-registered-input.json',[],
  'CConsole::GetRegisteredConsoleInput (Ego_r parameter key; lower-cased lookup) and RunCommandLine (Ego_r locals parser/parsed_item/input_name; the lookup expanded; explicit CCharString("") parser filename temporary)')
 print('NATIVE_CONSOLE_REGISTERED_INPUT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
