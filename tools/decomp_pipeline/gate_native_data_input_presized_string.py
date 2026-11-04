"""Fresh VC71 parity gate: CDataInputStream::ReadPresizedString (ReadValue<long> length, CArray<char>(n+1) via the out-of-line vector ctor, terminator, named CCharString)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0?$vector@DV?$allocator@D@_STL@@@_STL@@QAE@I@Z':0x411910,
 '?ReadWithSrcChunkOverflow@CDataInputStream@@QAEXPAXJ@Z':0x993ca0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F=R/'rebuild/src/compiled/00/99/CDataInputStream_ReadPresizedString_00996390.cpp'
ROOTS=[
 dict(name='presized_string',source=F,symbol='?ReadPresizedString@CDataInputStream@@QAE?AVCCharString@@XZ',address=0x996390,size=334,instructions=126,relocations=9),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-data-input-presized-string.json',[],
  'CDataInputStream::ReadPresizedString (ReadValue<long> length, CArray<char>(n+1) via the out-of-line vector ctor, terminator, named CCharString)')
 print('NATIVE_DATA_INPUT_PRESIZED_STRING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
