"""Fresh VC71 parity gate: CConsole::RunScript (Ego_r locals file/dst/parser/parsed_item; GFRemoveComments over the loaded buffer; the unused parser is built from the const char* ctor 0x00414E30)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1?$list@VCStringParserCommentBoundaries@@@std@@QAE@XZ':0x4046b0,
 '??0?$vector@DV?$allocator@D@_STL@@@_STL@@QAE@I@Z':0x411910,
 '??0CStringParser@@QAE@PBDP6IXABVCCharString@@@Z1J@Z':0x414e30,  # derived from retail site; not in symbol_index
 '?PathExists@CAFile@@SI_NABVCWideString@@@Z':0x999230,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??0CAFile@@QAE@XZ':0x99a2f0,
 '??1CBaseClassNonCopyable@@UAE@XZ':0x99a300,
 '?Open@CDiskFileWin32@@UAEXABVCWideString@@W4FileAccessType@@K@Z':0x99a6a0,
 '?Close@CDiskFileWin32@@UAEXXZ':0x99a920,
 '?Read@CDiskFileWin32@@UAEXPAXJPAVCASuspendableProcess@@@Z':0x99a960,
 '??0CWideString@@QAE@XZ':0x99aed0,
 '??1CWideString@@QAE@XZ':0x99b510,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?RunTextCommand@CConsole@@QAEXABVCCharString@@@Z':0x9ec710,
 '?GFRemoveComments@@YIXPAD00@Z':0xa60410,
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_7CDiskFileWin32@@6B@':0x122d06c,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_C@_02FMMEOLBI@?1?$CK?$AA@':0x129b1f8,
 '??_C@_02CHHINNLG@?$CK?1?$AA@':0x129b1fc,
 '??_C@_01LIIJDEN@?$AN?$AA@':0x129b200,
 '??_C@_02CBLDBPFN@?1?1?$AA@':0x129b204,
 '??_C@_01EEMJAFIK@?6?$AA@':0x129b208,
 '??_C@_01LOCGONAA@?$AA?$AA@':0x129c5f0,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/CConsole_RunScript_009ec890.cpp'
ROOTS=[
 dict(name='run_script',source=F,symbol='?RunScript@CConsole@@QAEXABVCWideString@@@Z',address=0x9ec890,size=402,instructions=108,relocations=37),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-console-run-script.json',[],
  'CConsole::RunScript (Ego_r locals file/dst/parser/parsed_item; GFRemoveComments over the loaded buffer; the unused parser is built from the const char* ctor 0x00414E30)')
 print('NATIVE_CONSOLE_RUN_SCRIPT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
