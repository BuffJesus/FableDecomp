"""Fresh VC71 parity gate: CRenderManagerCore SetD3DMatrix, SetWorldTransform(index), SetTextureTransform, SetPerspectiveTransform(16 floats)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?g_FableTextureTransformStates_0139703C@@3PAHA':0x139703c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a0/CRenderManagerCore_IndexedTransforms_00a0b1e0.cpp'
ROOTS=[
 dict(name='set_d3d_matrix',source=F,symbol='?SetD3DMatrix@CRenderManagerCore@@AAEXAAU_D3DMATRIX@@MMMMMMMMMMMMMMMM@Z',address=0xa0aad0,size=118,instructions=34,relocations=0),
 dict(name='world_index',source=F,symbol='?SetWorldTransform@CRenderManagerCore@@QAEXKABVCMatrix3x4@@@Z',address=0xa0b1e0,size=157,instructions=41,relocations=0),
 dict(name='texture',source=F,symbol='?SetTextureTransform@CRenderManagerCore@@QAEXKABVCMatrix3x4@@@Z',address=0xa0b280,size=158,instructions=41,relocations=1),
 dict(name='perspective16',source=F,symbol='?SetPerspectiveTransform@CRenderManagerCore@@QAEXMMMMMMMMMMMMMMMM@Z',address=0xa0b3e0,size=160,instructions=42,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-render-core-transforms.json',[],
  'CRenderManagerCore SetD3DMatrix, SetWorldTransform(index), SetTextureTransform, SetPerspectiveTransform(16 floats)')
 print('NATIVE_RENDER_CORE_TRANSFORMS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
