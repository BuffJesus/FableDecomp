"""Fresh VC71 parity gate: CSymbolMap::ParseBuffer (Ego_r locals parser_item/parser; the CStringParser const char* constructor expanded; loops SkipPastString(enum) into ParseEnum)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1?$list@VCStringParserCommentBoundaries@@@std@@QAE@XZ':0x4046b0,
 '??0CBaseClass@@QAE@XZ':0x99a2d0,
 '??1CBaseClass@@UAE@XZ':0x99a2e0,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '?Init@CStringParser@@QAEXPBDP6IXABVCCharString@@@Z1J@Z':0x9b9530,
 '?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z':0x9b9ba0,
 '?ParseEnum@CSymbolMap@@IAE_NAAVCStringParser@@@Z':0x9e55a0,  # derived from retail site; not in symbol_index
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
 '??_7CStringParser@@6B@':0x122d874,
 '??_C@_04DBJPMPLO@enum?$AA@':0x129b144,
}
F=R/'rebuild/src/compiled/00/9e/CSymbolMap_ParseBuffer_009e5ad0.cpp'
ROOTS=[
 dict(name='parse_buffer',source=F,symbol='?ParseBuffer@CSymbolMap@@IAE_NPBD@Z',address=0x9e5ad0,size=349,instructions=100,relocations=29),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-symbol-map-parse-buffer.json',[],
  'CSymbolMap::ParseBuffer (Ego_r locals parser_item/parser; the CStringParser const char* constructor expanded; loops SkipPastString(enum) into ParseEnum)')
 print('NATIVE_SYMBOL_MAP_PARSE_BUFFER_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
