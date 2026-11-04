"""Fresh VC71 parity gate: QuadricOptimiserInternals::COptimisedFace index queries (0x00AC6F50..0x00AC7030), exact after relocation"""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
}
ROOTS=[
 dict(name='remap_index',source=R/'rebuild/src/compiled/00/ac/COptimisedFace_Queries_00ac6f50.cpp',symbol='?RemapIndex@COptimisedFace@QuadricOptimiserInternals@@QAEXKK@Z',address=0xac6f50,size=35,instructions=13,relocations=0),
 dict(name='has_vertex',source=R/'rebuild/src/compiled/00/ac/COptimisedFace_Queries_00ac6f50.cpp',symbol='?HasVertex@COptimisedFace@QuadricOptimiserInternals@@QBE_NK@Z',address=0xac6f80,size=28,instructions=11,relocations=0),
 dict(name='has_unique_vertex',source=R/'rebuild/src/compiled/00/ac/COptimisedFace_Queries_00ac6f50.cpp',symbol='?HasUniqueVertex@COptimisedFace@QuadricOptimiserInternals@@QBE_NK@Z',address=0xac6fa0,size=29,instructions=11,relocations=0),
 dict(name='find_vertex',source=R/'rebuild/src/compiled/00/ac/COptimisedFace_Queries_00ac6f50.cpp',symbol='?FindVertex@COptimisedFace@QuadricOptimiserInternals@@QBEJK@Z',address=0xac6fc0,size=23,instructions=9,relocations=0),
 dict(name='is_identical',source=R/'rebuild/src/compiled/00/ac/COptimisedFace_Queries_00ac6f50.cpp',symbol='?IsIdentical@COptimisedFace@QuadricOptimiserInternals@@QBE_NABV12@@Z',address=0xac6fe0,size=71,instructions=30,relocations=0),
 dict(name='is_degenerate',source=R/'rebuild/src/compiled/00/ac/COptimisedFace_Queries_00ac6f50.cpp',symbol='?IsDegenerate@COptimisedFace@QuadricOptimiserInternals@@QBE_NXZ',address=0xac7030,size=26,instructions=13,relocations=0),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-quadric-optimised-face.json',[],
  'QuadricOptimiserInternals::COptimisedFace index queries (0x00AC6F50..0x00AC7030), exact after relocation')
 print('NATIVE_QUADRIC_OPTIMISED_FACE_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
