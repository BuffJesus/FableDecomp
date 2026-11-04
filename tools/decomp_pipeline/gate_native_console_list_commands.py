"""Fresh VC71 parity gate: CConsole list command bindings (Ego_r static fastcall members): Ego_r inline CCharString::Contains and GetTypeAsString()==literal; operator==(const char*) empty case as an if/return pair."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??8?$CBasicString@D@@QBE_NPBD@Z':0x4115a0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GetAsLowerCase@CCharString@@QBE?AV1@XZ':0x99ed10,
 '?SendMessageToUser@CConsole@@QAEXABVCCharString@@@Z':0x9eb260,
 '_strstr':0xbfeb3a,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_08LODIKCGI@variable?$AA@':0x122e5dc,
 '??_C@_07KMMNMMFF@command?$AA@':0x122e650,
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsole_ListCommands_009eb590.cpp'
ROOTS=[
 dict(name='console_list_containing',source=F,symbol='?ConsoleConsoleListContainingString@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9eb590,size=251,instructions=93,relocations=10),
 dict(name='command_list',source=F,symbol='?ConsoleCommandList@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9eb690,size=196,instructions=75,relocations=6),
 dict(name='command_list_containing',source=F,symbol='?ConsoleCommandListContainingString@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9eb760,size=376,instructions=133,relocations=15),
 dict(name='variable_list',source=F,symbol='?ConsoleVariableList@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9eb8e0,size=196,instructions=75,relocations=6),
 dict(name='variable_list_containing',source=F,symbol='?ConsoleVariableListContainingString@CConsole@@SIXAAV1@ABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z',address=0x9eb9b0,size=376,instructions=133,relocations=15),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-list-commands.json',[],
  'CConsole list command bindings (Ego_r static fastcall members): Ego_r inline CCharString::Contains and GetTypeAsString()==literal; operator==(const char*) empty case as an if/return pair')
 print('NATIVE_CONSOLE_LIST_COMMANDS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
