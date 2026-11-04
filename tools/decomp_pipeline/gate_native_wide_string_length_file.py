"""Fresh VC71 parity gate: CWideString::SetLength (resize or new spaces) and LoadFromFile (retail's reversed default length kept); UnassignString expanded."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?MakeOwnCopyOfData@CWideString@@AAEXXZ':0x99b560,
 '??0?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAE@IGABV?$allocator@G@1@@Z':0x99ca70,
 '?resize@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEXIG@Z':0x99cd60,  # derived from retail site; not in symbol_index
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??2@YAPAXI@Z':0xbfea1a,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/CWideString_SetLength_0099bc50.cpp'
ROOTS=[
 dict(name='set_length',source=F,symbol='?SetLength@CWideString@@QAEXJ@Z',address=0x99bc50,size=162,instructions=69,relocations=6),
 dict(name='load_from_file',source=F,symbol='?LoadFromFile@CWideString@@QAEXAAVCAFile@@J@Z',address=0x99bd00,size=171,instructions=72,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-length-file.json',[],
  "CWideString::SetLength (resize or new spaces) and LoadFromFile (retail's reversed default length kept); UnassignString expanded")
 print('NATIVE_WIDE_STRING_LENGTH_FILE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
