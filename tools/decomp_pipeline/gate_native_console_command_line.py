"""Fresh VC71 parity gate: CConsole command-line buffer/history methods in one retail-shaped TU: Reset and the buffer copies auto-inlined into their callers; RunTextCommand expands Reset and SetString; UserInputComplete expands ResetCommandLineBuffers."""
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
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '??4CCharString@@QAEABV0@PBD@Z':0x99efe0,
 '??YCCharString@@QAEABV0@ABV0@@Z':0x99f0a0,
 '??YCCharString@@QAEABV0@PBD@Z':0x99f100,
 '?GetString@CParsedItem@@QBEABVCCharString@@XZ':0x9b9880,
 '?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,
 '?ReadAsStringUntilString@CStringParser@@QAE_NABVCCharString@@AAVCParsedItem@@@Z':0x9ba110,  # derived from retail site; not in symbol_index
 '?RevertToUserEntry@CConsole@@AAEXXZ':0x9ea660,
 '?AddCommandLineToHistory@CConsole@@AAEXABVCConsoleCommandLine@@@Z':0x9eaf80,
 '?RunCommandLine@CConsole@@AAEXXZ':0x9eb430,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_01ICJEACDI@?$DL?$AA@':0x123542c,
}
F=R/'rebuild/src/compiled/00/9e/CConsole_CommandLineBuffers_009ea5a0.cpp'
ROOTS=[
 dict(name='reset_command_line_buffers',source=F,symbol='?ResetCommandLineBuffers@CConsole@@AAEXXZ',address=0x9ea5a0,size=53,instructions=19,relocations=4),
 dict(name='revert_to_user_entry',source=F,symbol='?RevertToUserEntry@CConsole@@AAEXXZ',address=0x9ea660,size=94,instructions=39,relocations=4),
 dict(name='prev_in_history',source=F,symbol='?PrevInHistory@CConsole@@AAEXXZ',address=0x9ea980,size=92,instructions=41,relocations=1),
 dict(name='set_history_mode',source=F,symbol='?SetHistoryMode@CConsole@@AAEXXZ',address=0x9eab80,size=82,instructions=36,relocations=1),
 dict(name='next_in_history',source=F,symbol='?NextInHistory@CConsole@@AAEXXZ',address=0x9ea9e0,size=92,instructions=40,relocations=2),
 dict(name='run_text_command',source=F,symbol='?RunTextCommand@CConsole@@QAEXABVCCharString@@@Z',address=0x9ec710,size=373,instructions=104,relocations=31),
 dict(name='rederive_reset',source=F,symbol='?Reset@CConsoleCommandLine@@AAEXXZ',address=0x9ea4f0,size=26,instructions=10,relocations=2),
 dict(name='rederive_copy_front_to_back',source=F,symbol='?CopyFrontBufferToBackBuffer@CConsole@@AAEXXZ',address=0x9ea620,size=29,instructions=12,relocations=1),
 dict(name='rederive_copy_back_to_front',source=F,symbol='?CopyBackBufferToFrontBuffer@CConsole@@AAEXXZ',address=0x9ea640,size=29,instructions=12,relocations=1),
 dict(name='user_input_complete',source=F,symbol='?UserInputComplete@CConsole@@AAEXXZ',address=0x9eca30,size=194,instructions=72,relocations=10),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-command-line.json',[],
  'CConsole command-line buffer/history methods in one retail-shaped TU: Reset and the buffer copies auto-inlined into their callers; RunTextCommand expands Reset and SetString; UserInputComplete expands ResetCommandLineBuffers')
 print('NATIVE_CONSOLE_COMMAND_LINE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
