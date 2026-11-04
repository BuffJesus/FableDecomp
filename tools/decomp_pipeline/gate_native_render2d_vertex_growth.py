"""Fresh VC71 parity gate for vector<CTVertexRHWColSpecTex1>::_M_insert_overflow 009E14B0."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={'_malloc':0xbfea0e,'_free':0xbfea14}  # CRT thunks pinned by many accepted gates
SYMBOL='?_M_insert_overflow@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@_STL@@@_STL@@QAEXPAVCTVertexRHWColSpecTex1@@ABV3@ABU__false_type@2@I_N@Z'
ROOTS=[dict(name='vertex_insert_overflow',source=R/'rebuild/src/compiled/00/9e/STLRender2DVertex_InsertOverflow_009e14b0.cpp',symbol=SYMBOL,address=0x9e14b0,size=294,instructions=111,relocations=2)]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render2d-vertex-growth.json',
  ['work/native-recreation/rendering/009e14b0/20261015/native-first (44,160 cases / 262,272 snapshots, aliasing values)'],
  'One genuine STLport template instantiation, byte-identical to retail after audited relocation patching; uninitialized copy/fill inlined.')
 print('NATIVE_RENDER2D_VERTEX_GROWTH_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
