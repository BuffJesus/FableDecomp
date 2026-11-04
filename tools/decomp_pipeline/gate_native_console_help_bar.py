"""Fresh VC71 parity gate: CConsole::SetHelpBarString (Ego_r locals parser/parsed_item; inline CCharString::operator==(char) against the GetFormattedName bracket)."""
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
 '?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ':0x9b9870,
 '?GetString@CParsedItem@@QBEABVCCharString@@XZ':0x9b9880,
 '?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,
 '?GetRegisteredConsoleInput@CConsole@@AAEPAVCConsoleInputBase@@ABVCCharString@@@Z':0x9eb220,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
 '?g_013caa48@@3VCCharString@@B':0x13caa48,
}
F=R/'rebuild/src/compiled/00/9e/CConsole_SetHelpBarString_009eb2c0.cpp'
ROOTS=[
 dict(name='set_help_bar_string',source=F,symbol='?SetHelpBarString@CConsole@@AAEXXZ',address=0x9eb2c0,size=368,instructions=121,relocations=23),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-help-bar.json',[],
  'CConsole::SetHelpBarString (Ego_r locals parser/parsed_item; inline CCharString::operator==(char) against the GetFormattedName bracket)')
 print('NATIVE_CONSOLE_HELP_BAR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
