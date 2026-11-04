"""Fresh VC71 parity gate: STLport vector<const CVertexShaderConstantLayout*>::_M_fill_insert, _M_insert_overflow and fill (shader constant-layout table growth; memmove shifts, fastcall fill helper)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$fill@PAPBVCVertexShaderConstantLayout@@PBV1@@_STL@@YIXPAPBVCVertexShaderConstantLayout@@0ABQBV1@@Z':0x98c120,
 '?_M_insert_overflow@?$vector@PBVCVertexShaderConstantLayout@@V?$allocator@PBVCVertexShaderConstantLayout@@@_STL@@@_STL@@QAEXPAPBVCVertexShaderConstantLayout@@ABQBV3@ABU__true_type@2@I_N@Z':0x98cde0,
 '_memmove':0xbfeae6,'_malloc':0xbfea0e,'_free':0xbfea14,
}
F=R/'rebuild/src/compiled/00/98/STLShaderLayoutPointerVector_FillInsert_0098d000.cpp'
ROOTS=[
 dict(name='fill_insert',source=F,symbol='?_M_fill_insert@?$vector@PBVCVertexShaderConstantLayout@@V?$allocator@PBVCVertexShaderConstantLayout@@@_STL@@@_STL@@QAEXPAPBVCVertexShaderConstantLayout@@IABQBV3@@Z',address=0x98d000,size=276,instructions=110,relocations=6),
 dict(name='insert_overflow',source=F,symbol='?_M_insert_overflow@?$vector@PBVCVertexShaderConstantLayout@@V?$allocator@PBVCVertexShaderConstantLayout@@@_STL@@@_STL@@QAEXPAPBVCVertexShaderConstantLayout@@ABQBV3@ABU__true_type@2@I_N@Z',address=0x98cde0,size=216,instructions=85,relocations=4),
 dict(name='fill',source=F,symbol='??$fill@PAPBVCVertexShaderConstantLayout@@PBV1@@_STL@@YIXPAPBVCVertexShaderConstantLayout@@0ABQBV1@@Z',address=0x98c120,size=31,instructions=12,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-shader-layout-fill-insert.json',[],
  'STLport vector<const CVertexShaderConstantLayout*>::_M_fill_insert, _M_insert_overflow and fill')
 print('NATIVE_SHADER_LAYOUT_FILL_INSERT_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
