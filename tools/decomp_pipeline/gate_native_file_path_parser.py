"""Fresh VC71 parity gate: CAFile private path-parser readers: ParserIsSeparatorNext / ParserReadPastSeparator (symbol \\ or /), ParserReadPastDriveLabel (string then ':' symbol, CCharString == expanded)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??8?$CBasicString@D@@QBE_NPBD@Z':0x4115a0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '?GetSymbol@CParsedItem@@QBEABVCCharString@@XZ':0x9b9870,
 '?ReadNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba080,
 '?PeekNextItem@CStringParser@@QAE_NAAVCParsedItem@@@Z':0x9ba8c0,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_01JLIPDDHJ@?3?$AA@':0x124c528,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CAFile_PathParser_00998170.cpp'
ROOTS=[
 dict(name='separator_next',source=F,symbol='?ParserIsSeparatorNext@CAFile@@CI_NAAVCStringParser@@@Z',address=0x997da0,size=236,instructions=83,relocations=5),
 dict(name='read_separator',source=F,symbol='?ParserReadPastSeparator@CAFile@@CI_NAAVCStringParser@@@Z',address=0x997e90,size=236,instructions=83,relocations=5),
 dict(name='drive_label',source=F,symbol='?ParserReadPastDriveLabel@CAFile@@CI_NAAVCStringParser@@AAVCParsedItem@@@Z',address=0x998170,size=96,instructions=40,relocations=7),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-path-parser.json',[],
  "CAFile private path-parser readers: ParserIsSeparatorNext / ParserReadPastSeparator (symbol \\ or /), ParserReadPastDriveLabel (string then ':' symbol, CCharString == expanded)")
 print('NATIVE_FILE_PATH_PARSER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
