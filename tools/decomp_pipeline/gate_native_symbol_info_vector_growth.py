"""Fresh VC71 parity gate: vector<CSymbolMap::CSymbolInfo> growth: _M_insert_overflow (STLport _M_clear/_M_set ending), out-of-line _M_clear, uninitialized copy and fill_n."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??0CCharString@@QAE@ABV0@@Z':0x99ec30,
 '??$__uninitialized_copy@PAVCSymbolInfo@CSymbolMap@@PAV12@@_STL@@YIPAVCSymbolInfo@CSymbolMap@@PAV12@00ABU__false_type@0@@Z':0x9e5fa0,  # derived from retail site; not in symbol_index
 '??$__uninitialized_fill_n@PAVCSymbolInfo@CSymbolMap@@IV12@@_STL@@YIPAVCSymbolInfo@CSymbolMap@@PAV12@IABV12@ABU__false_type@0@@Z':0x9e5fe0,  # derived from retail site; not in symbol_index
 '?_M_clear@?$vector@VCSymbolInfo@CSymbolMap@@V?$allocator@VCSymbolInfo@CSymbolMap@@@_STL@@@_STL@@QAEXXZ':0x9e7250,  # derived from retail site; not in symbol_index
 '_malloc':0xbfea0e,  # derived from retail site; not in symbol_index
 '_free':0xbfea14,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/9e/STLVector_SymbolInfo_M_insert_overflow_009e76f0.cpp'
ROOTS=[
 dict(name='insert_overflow',source=F,symbol='?_M_insert_overflow@?$vector@VCSymbolInfo@CSymbolMap@@V?$allocator@VCSymbolInfo@CSymbolMap@@@_STL@@@_STL@@QAEXPAVCSymbolInfo@CSymbolMap@@ABV34@ABU__false_type@2@I_N@Z',address=0x9e76f0,size=229,instructions=88,relocations=7),
 dict(name='clear',source=F,symbol='?_M_clear@?$vector@VCSymbolInfo@CSymbolMap@@V?$allocator@VCSymbolInfo@CSymbolMap@@@_STL@@@_STL@@QAEXXZ',address=0x9e7250,size=57,instructions=26,relocations=3),
 dict(name='uninitialized_copy',source=F,symbol='??$__uninitialized_copy@PAVCSymbolInfo@CSymbolMap@@PAV12@@_STL@@YIPAVCSymbolInfo@CSymbolMap@@PAV12@00ABU__false_type@0@@Z',address=0x9e5fa0,size=64,instructions=29,relocations=2),
 dict(name='uninitialized_fill_n',source=F,symbol='??$__uninitialized_fill_n@PAVCSymbolInfo@CSymbolMap@@IV12@@_STL@@YIPAVCSymbolInfo@CSymbolMap@@PAV12@IABV12@ABU__false_type@0@@Z',address=0x9e5fe0,size=60,instructions=28,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-symbol-info-vector-growth.json',[],
  'vector<CSymbolMap::CSymbolInfo> growth: _M_insert_overflow (STLport _M_clear/_M_set ending), out-of-line _M_clear, uninitialized copy and fill_n')
 print('NATIVE_SYMBOL_INFO_VECTOR_GROWTH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
