"""Fresh VC71 parity gate: GFHexCharStringToInt (skip spaces/tabs, hex digits either case)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/Global_GFHexCharStringToInt_0099e750.cpp'
ROOTS=[
 dict(name='hex_to_int',source=F,symbol='?GFHexCharStringToInt@@YIKABVCCharString@@@Z',address=0x99e750,size=152,instructions=66,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-hex-to-int.json',[],
  'GFHexCharStringToInt (skip spaces/tabs, hex digits either case)')
 print('NATIVE_STRING_HEX_TO_INT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
