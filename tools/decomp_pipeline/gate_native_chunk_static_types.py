"""Fresh VC71 parity gate: Out-of-line GetStaticChunkType copies, assigned by their returned literal."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??_C@_04OGLBMPFB@AOBJ?$AA@':0x129dfc4,
 '??_C@_04IMHECKIP@SUBM?$AA@':0x129e06c,
 '??_C@_04DFMHHGNM@MTLS?$AA@':0x129e074,
 '??_C@_04DOIACMFC@HPNT?$AA@':0x129e51c,
 '??_C@_04FGBDCMIO@HDMY?$AA@':0x129e524,
 '??_C@_04OMACLOLC@HCVL?$AA@':0x129e52c,
 '??_C@_04NKNNJHIJ@MVEC?$AA@':0x129e7a4,
 '??_C@_04HIAFEPA@TMEV?$AA@':0x129e7ac,
 '??_C@_04FAAHPKGJ@ASEQ?$AA@':0x129e7b4,
 '??_C@_04EIEHPPNM@AMSK?$AA@':0x129e8f8,
 '??_C@_04KEIKGGPK@TRFM?$AA@':0x129eaec,
 '??_C@_04FNMOPFIH@VGRP?$AA@':0x129eaf4,
 '??_C@_04JDKLPIAI@VERT?$AA@':0x129eafc,
 '??_C@_04HMAFHOHI@SMTH?$AA@':0x129eb04,
 '??_C@_04CBEPCLMK@UNIV?$AA@':0x129eb0c,
 '??_C@_04OOCFPGDI@MTRL?$AA@':0x129edc4,
 '??_C@_04CJFPMDAL@MTLE?$AA@':0x129edcc,
 '??_C@_04DLCDKOFJ@MMAP?$AA@':0x129edd4,
}
F0=R/'rebuild/src/compiled/00/a9/C3DMeshFileRootChunk_Finders_00a98fa0.cpp'
F1=R/'rebuild/src/compiled/00/ab/C3DMeshFileSubMeshChunk_Finders_00aba4a0.cpp'
F2=R/'rebuild/src/compiled/00/ac/C3DMeshFileMaterialChunk_Maps_00ac0c00.cpp'
ROOTS=[
 dict(name='gsct_anim_file_object',source=F0,symbol='?GetStaticChunkType@C3DAnimFileObjectChunk@@SIPBDXZ',address=0xa98830,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_sub_mesh',source=F0,symbol='?GetStaticChunkType@C3DMeshFileSubMeshChunk@@SIPBDXZ',address=0xa9a7f0,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_material_list',source=F0,symbol='?GetStaticChunkType@C3DMeshFileMaterialListChunk@@SIPBDXZ',address=0xa9a800,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_helper_point',source=F0,symbol='?GetStaticChunkType@C3DMeshFileHelperPointChunk@@SIPBDXZ',address=0xaa9620,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_dummy_object',source=F0,symbol='?GetStaticChunkType@C3DMeshFileDummyObjectChunk@@SIPBDXZ',address=0xaa9630,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_convex_volume',source=F0,symbol='?GetStaticChunkType@C3DMeshFileConvexVolumeChunk@@SIPBDXZ',address=0xaa9640,size=6,instructions=2,relocations=1),
 dict(name='gsct_anim_file_movement_vector',source=F0,symbol='?GetStaticChunkType@C3DAnimFileMovementVectorChunk@@SIPBDXZ',address=0xaae060,size=6,instructions=2,relocations=1),
 dict(name='gsct_anim_file_timing_event',source=F0,symbol='?GetStaticChunkType@C3DAnimFileTimingEventChunk@@SIPBDXZ',address=0xaae070,size=6,instructions=2,relocations=1),
 dict(name='gsct_anim_file_sequence',source=F0,symbol='?GetStaticChunkType@C3DAnimFileSequenceChunk@@SIPBDXZ',address=0xaae080,size=6,instructions=2,relocations=1),
 dict(name='gsct_anim_file_bone_mask',source=F0,symbol='?GetStaticChunkType@C3DAnimFileBoneMaskChunk@@SIPBDXZ',address=0xab0010,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_group',source=F0,symbol='?GetStaticChunkType@C3DMeshFileGroupChunk@@SIPBDXZ',address=0xab9a30,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_material',source=F0,symbol='?GetStaticChunkType@C3DMeshFileMaterialChunk@@SIPBDXZ',address=0xabfaf0,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_map',source=F0,symbol='?GetStaticChunkType@C3DMeshFileMapChunk@@SIPBDXZ',address=0xabfb10,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_transformation',source=F1,symbol='?GetStaticChunkType@C3DMeshFileTransformationChunk@@SIPBDXZ',address=0xab9a20,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_vertices',source=F1,symbol='?GetStaticChunkType@C3DMeshFileVerticesChunk@@SIPBDXZ',address=0xab9a40,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_smoothing_groups',source=F1,symbol='?GetStaticChunkType@C3DMeshFileSmoothingGroupsChunk@@SIPBDXZ',address=0xab9a50,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_unique_vertices',source=F1,symbol='?GetStaticChunkType@C3DMeshFileUniqueVerticesChunk@@SIPBDXZ',address=0xab9a60,size=6,instructions=2,relocations=1),
 dict(name='gsct_mesh_file_material_ext',source=F2,symbol='?GetStaticChunkType@C3DMeshFileMaterialExtChunk@@SIPBDXZ',address=0xabfb00,size=6,instructions=2,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-chunk-static-types.json',[],
  'Out-of-line GetStaticChunkType copies, assigned by their returned literal')
 print('NATIVE_CHUNK_STATIC_TYPES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
