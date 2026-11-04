"""Fresh VC71 parity gate: GFIsCharStringNumber / GFCharStringToInt / GFCharStringToUHUGE (Ego_r locals found_dp / is_negative; CRT isdigit, __allmul)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '__allmul':0xbfeb50,  # derived from retail site; not in symbol_index
 '_isdigit':0xbfeda6,  # derived from retail site; not in symbol_index
 '?g_FableEmptyNarrowString_0129AAF4@@3PADA':0x129aaf4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/99/GFCharStringNumbers_0099e620.cpp'
ROOTS=[
 dict(name='is_number',source=F,symbol='?GFIsCharStringNumber@@YI_NABVCCharString@@@Z',address=0x99e620,size=104,instructions=51,relocations=3),
 dict(name='to_int',source=F,symbol='?GFCharStringToInt@@YIJABVCCharString@@@Z',address=0x99e7f0,size=122,instructions=53,relocations=3),
 dict(name='to_uhuge',source=F,symbol='?GFCharStringToUHUGE@@YI_KABVCCharString@@@Z',address=0x99e870,size=140,instructions=62,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-numbers.json',[],
  'GFIsCharStringNumber / GFCharStringToInt / GFCharStringToUHUGE (Ego_r locals found_dp / is_negative; CRT isdigit, __allmul)')
 print('NATIVE_STRING_NUMBERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
