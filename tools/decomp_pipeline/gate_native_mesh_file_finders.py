"""Fresh VC71 parity gate: C3DMeshFileSubMeshChunk / C3DMeshFilePrimitiveChunk typed sub-chunk finders (SetStaticCast)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z':0xaa7f40,
 '?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QBE?BV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z':0xaa7ff0,
 '?FindFirstSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@PBD@Z':0xaa80e0,
 '?FindNextSubChunkOfType@CChunkedFileSuperChunk@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABV?$CCountedPointer@$$CBVCChunkedFileChunk@@@@@Z':0xaa8190,
 '??3@YAXPAX@Z':0xbfe9bc,  # derived from retail site; not in symbol_index
 '??_C@_04CPJDFACK@BONE?$AA@':0x129dedc,  # derived from retail site; not in symbol_index
 '??_C@_04FKFGIHAH@PRIM?$AA@':0x129dee4,  # derived from retail site; not in symbol_index
 '??_C@_04HLJHBOBI@TRIS?$AA@':0x129df28,  # derived from retail site; not in symbol_index
 '??_C@_04KEFJIOJP@CLTH?$AA@':0x129df30,  # derived from retail site; not in symbol_index
 '??_C@_04KEIKGGPK@TRFM?$AA@':0x129eaec,  # derived from retail site; not in symbol_index
 '??_C@_04FNMOPFIH@VGRP?$AA@':0x129eaf4,  # derived from retail site; not in symbol_index
 '??_C@_04JDKLPIAI@VERT?$AA@':0x129eafc,  # derived from retail site; not in symbol_index
 '??_C@_04HMAFHOHI@SMTH?$AA@':0x129eb04,  # derived from retail site; not in symbol_index
 '??_C@_04CBEPCLMK@UNIV?$AA@':0x129eb0c,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/ab/C3DMeshFileSubMeshChunk_Finders_00aba4a0.cpp'
ROOTS=[
 dict(name='find_transformation_const',source=F,symbol='?FindTransformation@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileTransformationChunk@@@@XZ',address=0xaba4a0,size=132,instructions=55,relocations=4),
 dict(name='find_first_bone',source=F,symbol='?FindFirstBone@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileBoneChunk@@@@XZ',address=0xaba530,size=132,instructions=55,relocations=4),
 dict(name='find_next_bone',source=F,symbol='?FindNextBone@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileBoneChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileBoneChunk@@@@@Z',address=0xaba5c0,size=209,instructions=79,relocations=4),
 dict(name='find_first_bone_const',source=F,symbol='?FindFirstBone@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileBoneChunk@@@@XZ',address=0xaba6a0,size=132,instructions=55,relocations=4),
 dict(name='find_next_bone_const',source=F,symbol='?FindNextBone@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileBoneChunk@@@@ABV2@@Z',address=0xaba730,size=209,instructions=79,relocations=4),
 dict(name='find_first_primitive',source=F,symbol='?FindFirstPrimitive@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFilePrimitiveChunk@@@@XZ',address=0xaba810,size=132,instructions=55,relocations=4),
 dict(name='find_next_primitive',source=F,symbol='?FindNextPrimitive@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFilePrimitiveChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFilePrimitiveChunk@@@@@Z',address=0xaba8a0,size=209,instructions=79,relocations=4),
 dict(name='find_first_primitive_const',source=F,symbol='?FindFirstPrimitive@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFilePrimitiveChunk@@@@XZ',address=0xaba980,size=132,instructions=55,relocations=4),
 dict(name='find_next_primitive_const',source=F,symbol='?FindNextPrimitive@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFilePrimitiveChunk@@@@ABV2@@Z',address=0xabaa10,size=209,instructions=79,relocations=4),
 dict(name='find_first_cloth',source=F,symbol='?FindFirstCloth@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileClothChunk@@@@XZ',address=0xabaaf0,size=132,instructions=55,relocations=4),
 dict(name='find_next_cloth',source=F,symbol='?FindNextCloth@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileClothChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileClothChunk@@@@@Z',address=0xabab80,size=209,instructions=79,relocations=4),
 dict(name='find_first_cloth_const',source=F,symbol='?FindFirstCloth@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileClothChunk@@@@XZ',address=0xabac60,size=132,instructions=55,relocations=4),
 dict(name='find_next_cloth_const',source=F,symbol='?FindNextCloth@C3DMeshFileSubMeshChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileClothChunk@@@@ABV2@@Z',address=0xabacf0,size=209,instructions=79,relocations=4),
 dict(name='prim_get_vertices',source=F,symbol='?GetVertices@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileVerticesChunk@@@@XZ',address=0xabaf50,size=132,instructions=55,relocations=4),
 dict(name='prim_peek_vertices_const',source=F,symbol='?PeekVertices@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileVerticesChunk@@@@XZ',address=0xabafe0,size=132,instructions=55,relocations=4),
 dict(name='prim_get_triangles',source=F,symbol='?GetTriangles@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileTrianglesChunk@@@@XZ',address=0xabb070,size=132,instructions=55,relocations=4),
 dict(name='prim_peek_triangles_const',source=F,symbol='?PeekTriangles@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileTrianglesChunk@@@@XZ',address=0xabb100,size=132,instructions=55,relocations=4),
 dict(name='prim_find_first_triangles',source=F,symbol='?FindFirstTriangles@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileTrianglesChunk@@@@XZ',address=0xabb190,size=132,instructions=55,relocations=4),
 dict(name='prim_find_next_triangles',source=F,symbol='?FindNextTriangles@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileTrianglesChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileTrianglesChunk@@@@@Z',address=0xabb220,size=209,instructions=79,relocations=4),
 dict(name='prim_find_first_triangles_const',source=F,symbol='?FindFirstTriangles@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileTrianglesChunk@@@@XZ',address=0xabb300,size=132,instructions=55,relocations=4),
 dict(name='prim_find_next_triangles_const',source=F,symbol='?FindNextTriangles@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileTrianglesChunk@@@@ABV2@@Z',address=0xabb390,size=209,instructions=79,relocations=4),
 dict(name='prim_has_smoothing_groups',source=F,symbol='?HasSmoothingGroups@C3DMeshFilePrimitiveChunk@@QBE_NXZ',address=0xabb470,size=118,instructions=49,relocations=4),
 dict(name='prim_get_smoothing_groups',source=F,symbol='?GetSmoothingGroups@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileSmoothingGroupsChunk@@@@XZ',address=0xabb4f0,size=132,instructions=55,relocations=4),
 dict(name='prim_peek_smoothing_groups_const',source=F,symbol='?PeekSmoothingGroups@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileSmoothingGroupsChunk@@@@XZ',address=0xabb580,size=132,instructions=55,relocations=4),
 dict(name='prim_has_unique_vertices',source=F,symbol='?HasUniqueVertices@C3DMeshFilePrimitiveChunk@@QBE_NXZ',address=0xabb610,size=118,instructions=49,relocations=4),
 dict(name='prim_get_unique_vertices',source=F,symbol='?GetUniqueVertices@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileUniqueVerticesChunk@@@@XZ',address=0xabb690,size=132,instructions=55,relocations=4),
 dict(name='prim_peek_unique_vertices_const',source=F,symbol='?PeekUniqueVertices@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileUniqueVerticesChunk@@@@XZ',address=0xabb720,size=132,instructions=55,relocations=4),
 dict(name='prim_find_first_group',source=F,symbol='?FindFirstGroup@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileGroupChunk@@@@XZ',address=0xabb7b0,size=132,instructions=55,relocations=4),
 dict(name='prim_find_next_group',source=F,symbol='?FindNextGroup@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileGroupChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@@Z',address=0xabb840,size=209,instructions=79,relocations=4),
 dict(name='prim_find_first_group_const',source=F,symbol='?FindFirstGroup@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@XZ',address=0xabb920,size=132,instructions=55,relocations=4),
 dict(name='prim_find_next_group_const',source=F,symbol='?FindNextGroup@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@ABV2@@Z',address=0xabb9b0,size=209,instructions=79,relocations=4),
 dict(name='prim_find_first_cloth',source=F,symbol='?FindFirstCloth@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileClothChunk@@@@XZ',address=0xabba90,size=132,instructions=55,relocations=4),
 dict(name='prim_find_next_cloth',source=F,symbol='?FindNextCloth@C3DMeshFilePrimitiveChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileClothChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileClothChunk@@@@@Z',address=0xabbb20,size=209,instructions=79,relocations=4),
 dict(name='prim_find_first_cloth_const',source=F,symbol='?FindFirstCloth@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileClothChunk@@@@XZ',address=0xabbc00,size=132,instructions=55,relocations=4),
 dict(name='prim_find_next_cloth_const',source=F,symbol='?FindNextCloth@C3DMeshFilePrimitiveChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileClothChunk@@@@ABV2@@Z',address=0xabbc90,size=209,instructions=79,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-file-finders.json',[],
  'C3DMeshFileSubMeshChunk / C3DMeshFilePrimitiveChunk typed sub-chunk finders (SetStaticCast)')
 print('NATIVE_MESH_FILE_FINDERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
