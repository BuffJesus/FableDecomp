"""Fresh VC71 parity gate: QuadricOptimiserInternals::CVertexPairContraction RemapIndex (0x00AC7940), HasVertex (0x00AC7990), CleanUp (0x00AC7B00), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='contraction_remap_index',source=R/'rebuild/src/compiled/00/ac/CVertexPairContraction_Methods_00ac7940.cpp',symbol='?RemapIndex@CVertexPairContraction@QuadricOptimiserInternals@@QAEXKK@Z',address=0xac7940,size=75,instructions=31,relocations=0),
 dict(name='contraction_has_vertex',source=R/'rebuild/src/compiled/00/ac/CVertexPairContraction_Methods_00ac7940.cpp',symbol='?HasVertex@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NK@Z',address=0xac7990,size=72,instructions=31,relocations=0),
 dict(name='contraction_clean_up',source=R/'rebuild/src/compiled/00/ac/CVertexPairContraction_Methods_00ac7940.cpp',symbol='?CleanUp@CVertexPairContraction@QuadricOptimiserInternals@@QAEXXZ',address=0xac7b00,size=188,instructions=73,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-vertex-pair-contraction.json',[],
  'QuadricOptimiserInternals::CVertexPairContraction RemapIndex (0x00AC7940), HasVertex (0x00AC7990), CleanUp (0x00AC7B00),')
 print('NATIVE_VERTEX_PAIR_CONTRACTION_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
