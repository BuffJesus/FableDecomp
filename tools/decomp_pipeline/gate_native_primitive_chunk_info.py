"""Fresh VC71 parity gate: SmoothingGroups/UniqueVertices GetChunkInfo (empty strings)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??0CCharString@@QAE@PBDJ@Z':0x99ebf0,
 '??_C@_00CNPNBAHC@?$AA@':0x122d70e,
}
F0=R/'rebuild/src/compiled/00/ab/C3DMeshFileSmoothingGroupsChunk_Methods_00abbfc0.cpp'
F1=R/'rebuild/src/compiled/00/ab/C3DMeshFileUniqueVerticesChunk_Methods_00abc040.cpp'
ROOTS=[
 dict(name='smoothing_groups_info',source=F0,symbol='?GetChunkInfo@C3DMeshFileSmoothingGroupsChunk@@UBE?BVCCharString@@XZ',address=0xab9f80,size=25,instructions=9,relocations=2),
 dict(name='unique_vertices_info',source=F1,symbol='?GetChunkInfo@C3DMeshFileUniqueVerticesChunk@@UBE?BVCCharString@@XZ',address=0xab9fa0,size=25,instructions=9,relocations=2),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-primitive-chunk-info.json',[],
  'SmoothingGroups/UniqueVertices GetChunkInfo (empty strings)')
 print('NATIVE_PRIMITIVE_CHUNK_INFO_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
