"""Fresh VC71 parity gate: CStringParser ReadLineAsString (relies on SkipPastNewline in the same unit leaving ecx intact) and ReadAsStringUntilWholeString."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?SkipUntilWholeString@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9aa0,
 '?SkipPastNewline@CStringParser@@QAEJXZ':0x9b9d70,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F=R/'rebuild/src/compiled/00/9b/CStringParser_Readers_009ba2d0.cpp'
ROOTS=[
 dict(name='read_line',source=F,symbol='?ReadLineAsString@CStringParser@@QAE?AVCCharString@@XZ',address=0x9ba390,size=71,instructions=27,relocations=4),
 dict(name='read_until_whole',source=F,symbol='?ReadAsStringUntilWholeString@CStringParser@@QAE?AVCCharString@@ABV2@@Z',address=0x9ba440,size=81,instructions=33,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-parser-readers.json',[],
  'CStringParser ReadLineAsString (relies on SkipPastNewline in the same unit leaving ecx intact) and ReadAsStringUntilWholeString')
 print('NATIVE_STRING_PARSER_READERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
