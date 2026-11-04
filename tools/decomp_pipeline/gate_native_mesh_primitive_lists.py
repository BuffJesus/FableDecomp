"""Fresh VC71 parity gate: Mesh-file primitive sub-chunk list accessors (lib_3d_mesh_file_submesh.cpp)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?_M_insert_overflow@?$vector@JV?$allocator@J@_STL@@@_STL@@QAEXPAJABJABU__true_type@2@I_N@Z':0x440990,
 '?_M_insert_overflow@?$vector@KV?$allocator@K@_STL@@@_STL@@QAEXPAKABKABU__true_type@2@I_N@Z':0x464ace,
 '?_M_insert_overflow@?$vector@VC3DTriangle2@@V?$allocator@VC3DTriangle2@@@_STL@@@_STL@@QAEXPAVC3DTriangle2@@ABV3@ABU__false_type@2@I_N@Z':0xa71550,
 '?erase@?$vector@VC3DVertex2@@V?$allocator@VC3DVertex2@@@_STL@@@_STL@@QAEPAVC3DVertex2@@PAV3@0@Z':0xa96a90,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??1CChunkedFileChunk@@UAE@XZ':0xaa75c0,
 '?_M_insert_overflow@?$vector@VC3DVertexBlend2@@V?$allocator@VC3DVertexBlend2@@@_STL@@@_STL@@QAEXPAVC3DVertexBlend2@@ABV3@ABU__false_type@2@I_N@Z':0xabeb40,
 '_free':0xbfea14,
 '_memmove':0xbfeae6,
 '??_C@_04JDKLPIAI@VERT?$AA@':0x129eafc,
 '??_C@_04HMAFHOHI@SMTH?$AA@':0x129eb04,
 '??_C@_04CBEPCLMK@UNIV?$AA@':0x129eb0c,
 '??_7C3DMeshFileSmoothingGroupsChunk@@6B@':0x129ec28,
 '??_7C3DMeshFileUniqueVerticesChunk@@6B@':0x129ec6c,
 '??_7C3DMeshFileVerticesChunk@@6B@':0x129ecf8,
}
F0=R/'rebuild/src/compiled/00/ab/C3DMeshFilePrimitiveSubChunks_Accessors_00abc370.cpp'
F1=R/'rebuild/src/compiled/00/ab/C3DMeshFileSmoothingGroupsChunk_Methods_00abbfc0.cpp'
F2=R/'rebuild/src/compiled/00/ab/C3DMeshFileUniqueVerticesChunk_Methods_00abc040.cpp'
F3=R/'rebuild/src/compiled/00/ab/C3DMeshFileGroupChunk_Methods_00abae50.cpp'
F4=R/'rebuild/src/compiled/00/ab/C3DMeshFileVerticesChunk_Methods_00abbd70.cpp'
ROOTS=[
 dict(name='group_get_count',source=F0,symbol='?GetCount@C3DMeshFileGroupChunk@@QBEKXZ',address=0xaba350,size=10,instructions=4,relocations=0),
 dict(name='vertices_get_count',source=F0,symbol='?GetCount@C3DMeshFileVerticesChunk@@QBEKXZ',address=0xaba360,size=10,instructions=4,relocations=0),
 dict(name='vertices_get_list_const',source=F0,symbol='?GetList@C3DMeshFileVerticesChunk@@QBEPBVC3DVertex2@@XZ',address=0xabbdd0,size=20,instructions=9,relocations=0),
 dict(name='triangles_get_list_const',source=F0,symbol='?GetList@C3DMeshFileTrianglesChunk@@QBEPBVC3DTriangle2@@XZ',address=0xabbf00,size=35,instructions=17,relocations=0),
 dict(name='unique_vertices_find_highest',source=F0,symbol='?FindHighestUniqueVertexIndex@C3DMeshFileUniqueVerticesChunk@@QBEKXZ',address=0xabc010,size=34,instructions=17,relocations=0),
 dict(name='triangles_get_list',source=F0,symbol='?GetList@C3DMeshFileTrianglesChunk@@QAEPAVC3DTriangle2@@XZ',address=0xabc440,size=35,instructions=17,relocations=0),
 dict(name='vertices_get_list',source=F0,symbol='?GetList@C3DMeshFileVerticesChunk@@QAEPAVC3DVertex2@@XZ',address=0xabc420,size=20,instructions=9,relocations=0),
 dict(name='group_get_bone_index',source=F0,symbol='?GetBoneIndex@C3DMeshFileGroupChunk@@QBEJXZ',address=0xab9f60,size=4,instructions=2,relocations=0),
 dict(name='group_set_bone_index',source=F0,symbol='?SetBoneIndex@C3DMeshFileGroupChunk@@QAEXJ@Z',address=0xab9f70,size=10,instructions=3,relocations=0),
 dict(name='triangles_get_count',source=F0,symbol='?GetCount@C3DMeshFileTrianglesChunk@@QBEKXZ',address=0xaba370,size=23,instructions=9,relocations=0),
 dict(name='smoothing_groups_clear_list',source=F0,symbol='?ClearList@C3DMeshFileSmoothingGroupsChunk@@QAEXXZ',address=0xabc4d0,size=42,instructions=22,relocations=1),
 dict(name='unique_vertices_clear_list',source=F0,symbol='?ClearList@C3DMeshFileUniqueVerticesChunk@@QAEXXZ',address=0xabc5c0,size=42,instructions=22,relocations=1),
 dict(name='primitive_add_material_index',source=F0,symbol='?AddMaterialIndex@C3DMeshFilePrimitiveChunk@@QAEXJ@Z',address=0xabc740,size=53,instructions=20,relocations=1),
 dict(name='smoothing_groups_add_face_group_mask',source=F0,symbol='?AddFaceGroupMask@C3DMeshFileSmoothingGroupsChunk@@QAEXK@Z',address=0xabc940,size=53,instructions=20,relocations=1),
 dict(name='unique_vertices_add_index',source=F0,symbol='?AddUniqueVertexIndex@C3DMeshFileUniqueVerticesChunk@@QAEXK@Z',address=0xabcab0,size=53,instructions=20,relocations=1),
 dict(name='triangles_add_triangle',source=F0,symbol='?AddTriangle@C3DMeshFileTrianglesChunk@@QAEXABVC3DTriangle2@@@Z',address=0xabcec0,size=65,instructions=25,relocations=1),
 dict(name='smoothing_groups_ctor',source=F1,symbol='??0C3DMeshFileSmoothingGroupsChunk@@QAE@XZ',address=0xabc470,size=34,instructions=12,relocations=3),
 dict(name='smoothing_groups_dtor',source=F1,symbol='??1C3DMeshFileSmoothingGroupsChunk@@UAE@XZ',address=0xabc4a0,size=33,instructions=12,relocations=3),
 dict(name='unique_vertices_ctor',source=F2,symbol='??0C3DMeshFileUniqueVerticesChunk@@QAE@XZ',address=0xabc560,size=34,instructions=12,relocations=3),
 dict(name='unique_vertices_dtor',source=F2,symbol='??1C3DMeshFileUniqueVerticesChunk@@UAE@XZ',address=0xabc590,size=33,instructions=12,relocations=3),
 dict(name='group_clear_list',source=F3,symbol='?ClearList@C3DMeshFileGroupChunk@@QAEXXZ',address=0xabc6b0,size=44,instructions=21,relocations=0),
 dict(name='group_add_vertex',source=F3,symbol='?AddVertex@C3DMeshFileGroupChunk@@QAEXABVC3DVertexBlend2@@@Z',address=0xabcc20,size=91,instructions=37,relocations=1),
 dict(name='vertices_ctor',source=F4,symbol='??0C3DMeshFileVerticesChunk@@QAE@XZ',address=0xabc780,size=34,instructions=12,relocations=3),
 dict(name='vertices_clear_list',source=F4,symbol='?ClearList@C3DMeshFileVerticesChunk@@QAEXXZ',address=0xabc7e0,size=17,instructions=7,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-primitive-lists.json',[],
  'Mesh-file primitive sub-chunk list accessors (lib_3d_mesh_file_submesh.cpp)')
 print('NATIVE_MESH_PRIMITIVE_LISTS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
