"""Fresh VC71 parity gate: CWideString::Sprintf (8000-char stack buffer or CArray) and SprintfDynamic; UnassignString and operator=(const wchar_t*) expanded."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_fill_insert@?$vector@GV?$allocator@G@_STL@@@_STL@@QAEXPAGIABG@Z':0x5149f0,  # derived from retail site; not in symbol_index
 '?AllocStringData@CWideString@@AAEPAVCWideStringData@@PBGJ@Z':0x99b3c0,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '@__security_check_cookie@4':0xbfe9f9,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '__chkstk':0xbfea30,  # derived from retail site; not in symbol_index
 '_wcslen':0xbfed94,  # derived from retail site; not in symbol_index
 '__vsnwprintf':0xbfed9a,  # derived from retail site; not in symbol_index
 '___security_cookie':0x139c8a8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CWideString_Sprintf_0099ba70.cpp'
GS=['/nologo','/c','/O2','/GS','/MT','/W3']
ROOTS=[
 dict(name='sprintf',source=F,symbol='?Sprintf@CWideString@@QAAXPBGZZ',address=0x99ba70,size=265,instructions=89,relocations=10,flags=GS),
 dict(name='sprintf_dynamic',source=F,symbol='?SprintfDynamic@CWideString@@QAAXPBGZZ',address=0x99bb80,size=197,instructions=77,relocations=7,flags=GS),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-sprintf.json',[],
  'CWideString::Sprintf (8000-char stack buffer or CArray) and SprintfDynamic; UnassignString and operator=(const wchar_t*) expanded')
 print('NATIVE_WIDE_STRING_SPRINTF_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
