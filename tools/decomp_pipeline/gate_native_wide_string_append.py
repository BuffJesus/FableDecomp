"""Fresh VC71 parity gate: CWideString::operator+=(const CWideString&): own copy then STLport append, or the expanded copy assignment when empty."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?MakeOwnCopyOfData@CWideString@@AAEXXZ':0x99b560,
 '??$append@PBG@?$basic_string@GU?$char_traits@G@_STL@@V?$allocator@G@2@@_STL@@QAEAAV01@PBG0ABUforward_iterator_tag@1@@Z':0x99c7e0,
 '??_C@_11LOCGONAA@?$AA?$AA@':0x129a8e0,
}
F=R/'rebuild/src/compiled/00/99/CWideString_AppendWideString_0099b8d0.cpp'
ROOTS=[
 dict(name='append_wide_string',source=F,symbol='??YCWideString@@QAEABV0@ABV0@@Z',address=0x99b8d0,size=104,instructions=42,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-append.json',[],
  'CWideString::operator+=(const CWideString&): own copy then STLport append, or the expanded copy assignment when empty')
 print('NATIVE_WIDE_STRING_APPEND_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
