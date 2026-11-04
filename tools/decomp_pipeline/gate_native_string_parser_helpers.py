"""Fresh VC71 parity gate: CStringParser CountLinesParsed / ReadStringBetween / CharSeperatesString and CParsedItem InitAsSymbol / InitAsString (both forms; the CCharString one is inline for callers)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?SetAsSubString@CCharString@@QAEXPBDJ@Z':0x99ef40,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '_isdigit':0xbfeda6,  # derived from retail site; not in symbol_index
 '_isalpha':0xbfedb8,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9b/CStringParser_Helpers_009b95a0.cpp'
ROOTS=[
 dict(name='count_lines',source=F,symbol='?CountLinesParsed@CStringParser@@QBEJXZ',address=0x9b95a0,size=30,instructions=16,relocations=0),
 dict(name='read_between',source=F,symbol='?ReadStringBetween@CStringParser@@IAE?AVCCharString@@PBD0@Z',address=0x9b95c0,size=30,instructions=12,relocations=1),
 dict(name='char_separates',source=F,symbol='?CharSeperatesString@CStringParser@@IAE_ND@Z',address=0x9b96a0,size=54,instructions=24,relocations=2),
 dict(name='init_as_symbol',source=F,symbol='?InitAsSymbol@CParsedItem@@QAEXPBDK@Z',address=0x9b96e0,size=31,instructions=11,relocations=1),
 dict(name='init_as_string_text',source=F,symbol='?InitAsString@CParsedItem@@QAEXPBDK@Z',address=0x9b9830,size=31,instructions=11,relocations=1),
 dict(name='init_as_string',source=F,symbol='?InitAsString@CParsedItem@@QAEXABVCCharString@@@Z',address=0x9b9850,size=26,instructions=9,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-string-parser-helpers.json',[],
  'CStringParser CountLinesParsed / ReadStringBetween / CharSeperatesString and CParsedItem InitAsSymbol / InitAsString (both forms; the CCharString one is inline for callers)')
 print('NATIVE_STRING_PARSER_HELPERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
