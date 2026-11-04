"""Fresh VC71 parity gate for CSymbolMap::FindFirstSymbolWithVal overloads 009E5090 / 009E50C0."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
# CBasicString<char>::Compare, retail 0x00411570 (fable_basic_string.h, FableWin name).
ADDRESS={'?Compare@?$CBasicString@D@@KIJPBD0@Z':0x411570}
F=R/'rebuild/src/compiled/00/9e/CSymbolMap_FindFirstSymbolWithVal_009e5090.cpp'
ROOTS=[
 dict(name='find_first_with_val',source=F,symbol='?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@J@Z',address=0x9e5090,size=39,instructions=15,relocations=0),
 dict(name='find_first_with_group_val',source=F,symbol='?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@ABVCCharString@@J@Z',address=0x9e50c0,size=95,instructions=45,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-symbol-map-queries.json',[],
  'Two genuine PDB-named roots, byte-identical to retail after audited relocation patching; CCharString/CBasicString equality inlined. Header operators verified not to change any of 1,439 includer TUs (section bytes, labels normalised).')
 print('NATIVE_SYMBOL_MAP_QUERIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
