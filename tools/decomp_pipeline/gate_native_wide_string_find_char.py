"""Fresh VC71 parity gate: CWideString::FindCharPos (wcschr over the expanded text pointer; manifest labelled it CCharString)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '_wcschr':0xbfed8e,  # derived from retail site; not in symbol_index
 '??_C@_11LOCGONAA@?$AA?$AA@':0x129a8e0,
}
F=R/'rebuild/src/compiled/00/99/CWideString_FindCharPos_0099b070.cpp'
ROOTS=[
 dict(name='find_char_pos',source=F,symbol='?FindCharPos@CWideString@@QBEJG@Z',address=0x99b070,size=72,instructions=31,relocations=3),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-wide-string-find-char.json',[],
  'CWideString::FindCharPos (wcschr over the expanded text pointer; manifest labelled it CCharString)')
 print('NATIVE_WIDE_STRING_FIND_CHAR_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
