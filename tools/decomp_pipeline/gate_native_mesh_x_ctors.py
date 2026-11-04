"""Fresh VC71 parity gate: Mesh-file X chunks (lib_3d_mesh_file_x.cpp): ctors, ClearTangents, SetVertexData, C3DMeshFileXLoader, deleting dtors; transformation/bone ctors from data."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '?resize@?$vector@DV?$allocator@D@_STL@@@_STL@@QAEXIABD@Z':0x4122d0,
 '??$__copy@PAVC3DVector@@PAV1@H@_STL@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z':0x74ca20,
 '??0CCharString@@QAE@XZ':0x99e4b0,
 '?DeleteData@?$CCountedPointer@VCChunkedFileChunk@@@@CIXPAX@Z':0xa99ff0,
 '??1C3DMeshFileLoader@@UAE@XZ':0xa9a900,
 '?RegisterRootSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa9b840,
 '?RegisterSubMeshSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa9b870,
 '?RegisterPrimitiveSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa9b8a0,
 '?RegisterTrianglesSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa9b8d0,
 '?RegisterMapSubChunkType@C3DMeshFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z':0xa9ba10,
 '??0C3DMeshFileLoader@@QAE@XZ':0xa9bbe0,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??1CChunkedFileChunk@@UAE@XZ':0xaa75c0,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '??1CChunkedFileSuperChunk@@UAE@XZ':0xaa8490,
 '?RegisterSubChunkType@CChunkedFileSuperChunk@@QAEXABV?$CCountedPointer@VCChunkedFileChunk@@@@@Z':0xaa87e0,
 '??0C3DMeshFileTrianglesChunk@@QAE@XZ':0xabc860,
 '??0CVertexGroupBlock@@QAE@XZ':0xad9030,
 '??3@YAXPAX@Z':0xbfe9bc,
 '_free':0xbfea14,
 '??2@YAPAXI@Z':0xbfea1a,
 '??_C@_04OGKGDHNF@XBDS?$AA@':0x129cef0,
 '??_C@_04LFHPBEMF@XBDB?$AA@':0x129cef8,
 '??_C@_04CPJDFACK@BONE?$AA@':0x129dedc,
 '??_C@_04HLILLJCK@XUVT?$AA@':0x129df38,
 '??_C@_04DINDAPDJ@XVBD?$AA@':0x129df40,
 '??_C@_04IBKIFCME@XMIF?$AA@':0x129df48,
 '??_C@_04PGCHKCNG@XAPR?$AA@':0x129df50,
 '??_C@_04LMOFLAIH@XAPB?$AA@':0x129df58,
 '??_C@_04MFPMBMKE@XTST?$AA@':0x129df60,
 '??_C@_04JOIPCIFD@XTID?$AA@':0x129e1ec,
 '??_7C3DMeshFileXTextureIndexChunk@@6B@':0x129e1f8,
 '??_7C3DMeshFileXMappingInfoChunk@@6B@':0x129e238,
 '??_7C3DMeshFileXAnimatedPrimitiveChunk@@6B@':0x129e278,
 '??_7C3DMeshFileXBoundingSphereChunk@@6B@':0x129e2bc,
 '??_7C3DMeshFileXBoundingBoxChunk@@6B@':0x129e2fc,
 '??_7C3DMeshFileXVertexDataChunk@@6B@':0x129e33c,
 '??_7C3DMeshFileXTriangleStripChunk@@6B@':0x129e380,
 '??_7C3DMeshFileXUVTangentChunk@@6B@':0x129e3c0,
 '??_7C3DMeshFileXAnimatedPrimitiveBlockChunk@@6B@':0x129e404,
 '??_7C3DMeshFileXLoader@@6B@':0x129e450,
 '??_C@_04KEIKGGPK@TRFM?$AA@':0x129eaec,
 '??_7C3DMeshFileTransformationChunk@@6B@':0x129eb54,
 '??_7C3DMeshFileBoneChunk@@6B@':0x129eb9c,
}
F0=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
F1=R/'rebuild/src/compiled/00/ab/C3DMeshFileTransformationChunk_Methods_00ab9c30.cpp'
F2=R/'rebuild/src/compiled/00/ab/C3DMeshFileBoneChunk_Methods_00ab9ea0.cpp'
ROOTS=[
 dict(name='x_texture_index_ctor',source=F0,symbol='??0C3DMeshFileXTextureIndexChunk@@QAE@XZ',address=0xaa4ad0,size=30,instructions=9,relocations=3),
 dict(name='x_mapping_info_ctor',source=F0,symbol='??0C3DMeshFileXMappingInfoChunk@@QAE@XZ',address=0xaa4be0,size=30,instructions=9,relocations=3),
 dict(name='x_bounding_box_ctor',source=F0,symbol='??0C3DMeshFileXBoundingBoxChunk@@QAE@XZ',address=0xaa5000,size=43,instructions=15,relocations=3),
 dict(name='x_triangle_strip_ctor',source=F0,symbol='??0C3DMeshFileXTriangleStripChunk@@QAE@XZ',address=0xaa5a30,size=34,instructions=12,relocations=3),
 dict(name='x_uv_tangent_ctor',source=F0,symbol='??0C3DMeshFileXUVTangentChunk@@QAE@XZ',address=0xaa5ac0,size=43,instructions=15,relocations=3),
 dict(name='x_uv_tangent_clear',source=F0,symbol='?ClearTangents@C3DMeshFileXUVTangentChunk@@QAEXXZ',address=0xaa5af0,size=55,instructions=24,relocations=2),
 dict(name='x_vertex_data_set',source=F0,symbol='?SetVertexData@C3DMeshFileXVertexDataChunk@@QAEXKPBXKKK@Z',address=0xaa6640,size=105,instructions=41,relocations=1),
 dict(name='x_loader_ctor',source=F0,symbol='??0C3DMeshFileXLoader@@QAE@XZ',address=0xaa5c80,size=2144,instructions=598,relocations=100),
 dict(name='x_loader_deleting_dtor',source=F0,symbol='??_GC3DMeshFileXLoader@@UAEPAXI@Z',address=0xaa7440,size=30,instructions=11,relocations=2),
 dict(name='x_vertex_data_deleting_dtor',source=F0,symbol='??_GC3DMeshFileXVertexDataChunk@@UAEPAXI@Z',address=0xaa7250,size=48,instructions=18,relocations=3),
 dict(name='x_triangle_strip_deleting_dtor',source=F0,symbol='??_GC3DMeshFileXTriangleStripChunk@@UAEPAXI@Z',address=0xaa72a0,size=48,instructions=18,relocations=3),
 dict(name='x_uv_tangent_deleting_dtor',source=F0,symbol='??_GC3DMeshFileXUVTangentChunk@@UAEPAXI@Z',address=0xaa7370,size=64,instructions=24,relocations=4),
 dict(name='transformation_ctor_matrix',source=F1,symbol='??0C3DMeshFileTransformationChunk@@QAE@ABVCMatrix3x4@@@Z',address=0xab9bb0,size=43,instructions=16,relocations=3),
 dict(name='bone_ctor_index_matrix',source=F2,symbol='??0C3DMeshFileBoneChunk@@QAE@JABVCMatrix3x4@@@Z',address=0xab9d50,size=70,instructions=24,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-x-ctors.json',[],
  'Mesh-file X chunks (lib_3d_mesh_file_x.cpp): ctors, ClearTangents, SetVertexData, C3DMeshFileXLoader, deleting dtors; transformation/bone ctors from data')
 print('NATIVE_MESH_X_CTORS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
