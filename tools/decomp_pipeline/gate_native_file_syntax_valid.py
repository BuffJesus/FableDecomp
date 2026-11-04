"""Fresh VC71 parity gate: CAFile::IsFilePathSyntaxValid (CExpectedTextType state machine) with the look-aheads it expands: ParserIsFinished, ParserIsComponentNext, ParserIsDriveLabelNext."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1?$list@VCStringParserCommentBoundaries@@@std@@QAE@XZ':0x4046b0,
 '??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z':0x404720,
 '?ParserIsSeparatorNext@CAFile@@CI_NAAVCStringParser@@@Z':0x997da0,
 '?ParserReadPastSeparator@CAFile@@CI_NAAVCStringParser@@@Z':0x997e90,
 '?ParserReadPastComponent@CAFile@@CI_NAAVCStringParser@@AAVCCharString@@@Z':0x998010,  # derived from retail site; not in symbol_index
 '?ParserReadPastDriveLabel@CAFile@@CI_NAAVCStringParser@@AAVCParsedItem@@@Z':0x998170,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '?ToCharString@CWideString@@QBE?AVCCharString@@XZ':0x99b2c0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?SetStringPos@CStringParser@@QAEXK@Z':0x9b9940,
 '?NextItemExists@CStringParser@@QAE_NXZ':0x9ba4f0,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F=R/'rebuild/src/compiled/00/99/CAFile_IsFilePathSyntaxValid_00998a30.cpp'
ROOTS=[
 dict(name='syntax_valid',source=F,symbol='?IsFilePathSyntaxValid@CAFile@@SI_NABVCWideString@@_N@Z',address=0x998a30,size=512,instructions=140,relocations=29),
 dict(name='parser_finished',source=F,symbol='?ParserIsFinished@CAFile@@CI_NAAVCStringParser@@@Z',address=0x9975d0,size=11,instructions=5,relocations=1),
 dict(name='component_next',source=F,symbol='?ParserIsComponentNext@CAFile@@CI_NAAVCStringParser@@@Z',address=0x9981d0,size=62,instructions=25,relocations=5),
 dict(name='drive_label_next',source=F,symbol='?ParserIsDriveLabelNext@CAFile@@CI_NAAVCStringParser@@@Z',address=0x998210,size=59,instructions=23,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-file-syntax-valid.json',[],
  'CAFile::IsFilePathSyntaxValid (CExpectedTextType state machine) with the look-aheads it expands: ParserIsFinished, ParserIsComponentNext, ParserIsDriveLabelNext')
 print('NATIVE_FILE_SYNTAX_VALID_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
