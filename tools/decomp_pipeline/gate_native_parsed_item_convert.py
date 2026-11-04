"""Fresh VC71 parity gate: CParsedItem::ConvertToString (switch with embedded jump table; NULL default) and CStringParser::MoveStringPos."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '?GFIntToCharString@@YI?AVCCharString@@J@Z':0x99f830,
 '?GFFloatToCharString@@YI?AVCCharString@@NJ@Z':0x9a08b0,  # derived from retail site; not in symbol_index
 '?CalcInQuotes@CStringParser@@IBE_N_NPBD1@Z':0x9b9570,
 '$L2413':0x9b98c0,  # derived from retail site; not in symbol_index
 '$L2415':0x9b98d4,  # derived from retail site; not in symbol_index
 '$L2417':0x9b98f3,  # derived from retail site; not in symbol_index
 '$L2590':0x9b9920,  # derived from retail site; not in symbol_index
 '??_C@_04HIBGFPH@NULL?$AA@':0x12393e4,
}
F=R/'rebuild/src/compiled/00/9b/CParsedItem_ConvertToString_009b98b0.cpp'
ROOTS=[
 dict(name='convert_to_string',source=F,symbol='?ConvertToString@CParsedItem@@QBE?AVCCharString@@XZ',address=0x9b98b0,size=128,instructions=39,code_size=0x70,relocations=10),
 dict(name='move_string_pos',source=F,symbol='?MoveStringPos@CStringParser@@IAEXK@Z',address=0x9b9980,size=53,instructions=25,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-parsed-item-convert.json',[],
  'CParsedItem::ConvertToString (switch with embedded jump table; NULL default) and CStringParser::MoveStringPos')
 print('NATIVE_PARSED_ITEM_CONVERT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
