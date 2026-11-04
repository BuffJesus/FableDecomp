"""Fresh VC71 parity gate: Mesh/anim-file chunk typed sub-chunk finders (root, helpers, anim object, material list/material, cloth)."""
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
 '??_C@_04PHMJNJFO@EMTL?$AA@':0x129ded4,  # derived from retail site; not in symbol_index
 '??_C@_04OGLBMPFB@AOBJ?$AA@':0x129dfc4,  # derived from retail site; not in symbol_index
 '??_C@_04GEKNONIJ@HLPR?$AA@':0x129dfcc,  # derived from retail site; not in symbol_index
 '??_C@_04IMHECKIP@SUBM?$AA@':0x129e06c,  # derived from retail site; not in symbol_index
 '??_C@_04DFMHHGNM@MTLS?$AA@':0x129e074,  # derived from retail site; not in symbol_index
 '??_C@_04DOIACMFC@HPNT?$AA@':0x129e51c,  # derived from retail site; not in symbol_index
 '??_C@_04FGBDCMIO@HDMY?$AA@':0x129e524,  # derived from retail site; not in symbol_index
 '??_C@_04OMACLOLC@HCVL?$AA@':0x129e52c,  # derived from retail site; not in symbol_index
 '??_C@_04NKNNJHIJ@MVEC?$AA@':0x129e7a4,  # derived from retail site; not in symbol_index
 '??_C@_04HIAFEPA@TMEV?$AA@':0x129e7ac,  # derived from retail site; not in symbol_index
 '??_C@_04FAAHPKGJ@ASEQ?$AA@':0x129e7b4,  # derived from retail site; not in symbol_index
 '??_C@_04EIEHPPNM@AMSK?$AA@':0x129e8f8,  # derived from retail site; not in symbol_index
 '??_C@_04FNMOPFIH@VGRP?$AA@':0x129eaf4,  # derived from retail site; not in symbol_index
 '??_C@_04OOCFPGDI@MTRL?$AA@':0x129edc4,  # derived from retail site; not in symbol_index
 '??_C@_04CJFPMDAL@MTLE?$AA@':0x129edcc,  # derived from retail site; not in symbol_index
 '??_C@_04DLCDKOFJ@MMAP?$AA@':0x129edd4,  # derived from retail site; not in symbol_index
}
F=R/'rebuild/src/compiled/00/a9/C3DMeshFileRootChunk_Finders_00a98fa0.cpp'
ROOTS=[
 dict(name='anim_file_root_find_first_object',source=F,symbol='?FindFirstObject@C3DAnimFileRootChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileObjectChunk@@@@XZ',address=0xa98fa0,size=132,instructions=55,relocations=4),
 dict(name='anim_file_root_find_next_object',source=F,symbol='?FindNextObject@C3DAnimFileRootChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileObjectChunk@@@@ABV?$CCountedPointer@$$CBVC3DAnimFileObjectChunk@@@@@Z',address=0xa99030,size=209,instructions=79,relocations=4),
 dict(name='anim_file_root_find_first_object_const',source=F,symbol='?FindFirstObject@C3DAnimFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileObjectChunk@@@@XZ',address=0xa99110,size=132,instructions=55,relocations=4),
 dict(name='anim_file_root_find_next_object_const',source=F,symbol='?FindNextObject@C3DAnimFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileObjectChunk@@@@ABV2@@Z',address=0xa991a0,size=209,instructions=79,relocations=4),
 dict(name='anim_file_root_has_helpers',source=F,symbol='?HasHelpers@C3DAnimFileRootChunk@@QBE_NXZ',address=0xa99280,size=118,instructions=49,relocations=4),
 dict(name='anim_file_root_get_helpers',source=F,symbol='?GetHelpers@C3DAnimFileRootChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileHelpersChunk@@@@XZ',address=0xa99300,size=132,instructions=55,relocations=4),
 dict(name='anim_file_root_get_helpers_const',source=F,symbol='?GetHelpers@C3DAnimFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileHelpersChunk@@@@XZ',address=0xa99390,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_root_find_first_sub_mesh',source=F,symbol='?FindFirstSubMesh@C3DMeshFileRootChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileSubMeshChunk@@@@XZ',address=0xa9aec0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_root_find_next_sub_mesh',source=F,symbol='?FindNextSubMesh@C3DMeshFileRootChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileSubMeshChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileSubMeshChunk@@@@@Z',address=0xa9af50,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_root_find_first_sub_mesh_const',source=F,symbol='?FindFirstSubMesh@C3DMeshFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileSubMeshChunk@@@@XZ',address=0xa9b030,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_root_find_next_sub_mesh_const',source=F,symbol='?FindNextSubMesh@C3DMeshFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileSubMeshChunk@@@@ABV2@@Z',address=0xa9b0c0,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_root_get_material_list',source=F,symbol='?GetMaterialList@C3DMeshFileRootChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMaterialListChunk@@@@XZ',address=0xa9b1a0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_root_get_material_list_const',source=F,symbol='?GetMaterialList@C3DMeshFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMaterialListChunk@@@@XZ',address=0xa9b230,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_root_has_helpers',source=F,symbol='?HasHelpers@C3DMeshFileRootChunk@@QBE_NXZ',address=0xa9b2c0,size=118,instructions=49,relocations=4),
 dict(name='mesh_file_root_get_helpers',source=F,symbol='?GetHelpers@C3DMeshFileRootChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileHelpersChunk@@@@XZ',address=0xa9b340,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_root_get_helpers_const',source=F,symbol='?GetHelpers@C3DMeshFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileHelpersChunk@@@@XZ',address=0xa9b3d0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_first_point',source=F,symbol='?FindFirstPoint@C3DMeshFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileHelperPointChunk@@@@XZ',address=0xaaa100,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_next_point',source=F,symbol='?FindNextPoint@C3DMeshFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileHelperPointChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileHelperPointChunk@@@@@Z',address=0xaaa190,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_helpers_find_first_point_const',source=F,symbol='?FindFirstPoint@C3DMeshFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileHelperPointChunk@@@@XZ',address=0xaaa270,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_next_point_const',source=F,symbol='?FindNextPoint@C3DMeshFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileHelperPointChunk@@@@ABV2@@Z',address=0xaaa300,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_helpers_find_first_dummy',source=F,symbol='?FindFirstDummy@C3DMeshFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileDummyObjectChunk@@@@XZ',address=0xaaa3e0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_next_dummy',source=F,symbol='?FindNextDummy@C3DMeshFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileDummyObjectChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileDummyObjectChunk@@@@@Z',address=0xaaa470,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_helpers_find_first_dummy_const',source=F,symbol='?FindFirstDummy@C3DMeshFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileDummyObjectChunk@@@@XZ',address=0xaaa550,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_next_dummy_const',source=F,symbol='?FindNextDummy@C3DMeshFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileDummyObjectChunk@@@@ABV2@@Z',address=0xaaa5e0,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_helpers_find_first_volume_w',source=F,symbol='?FindFirstVolumeW@C3DMeshFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileConvexVolumeChunk@@@@XZ',address=0xaaa6c0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_next_volume_w',source=F,symbol='?FindNextVolumeW@C3DMeshFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileConvexVolumeChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileConvexVolumeChunk@@@@@Z',address=0xaaa750,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_helpers_find_first_volume_w_const',source=F,symbol='?FindFirstVolumeW@C3DMeshFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileConvexVolumeChunk@@@@XZ',address=0xaaa830,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_helpers_find_next_volume_w_const',source=F,symbol='?FindNextVolumeW@C3DMeshFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileConvexVolumeChunk@@@@ABV2@@Z',address=0xaaa8c0,size=209,instructions=79,relocations=4),
 dict(name='anim_file_helpers_has_movement_vector',source=F,symbol='?HasMovementVector@C3DAnimFileHelpersChunk@@QBE_NXZ',address=0xaae4d0,size=71,instructions=26,relocations=3),
 dict(name='anim_file_helpers_get_movement_vector',source=F,symbol='?GetMovementVector@C3DAnimFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileMovementVectorChunk@@@@XZ',address=0xaae520,size=132,instructions=55,relocations=4),
 dict(name='anim_file_helpers_get_movement_vector_const',source=F,symbol='?GetMovementVector@C3DAnimFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileMovementVectorChunk@@@@XZ',address=0xaae5b0,size=132,instructions=55,relocations=4),
 dict(name='anim_file_helpers_find_first_event',source=F,symbol='?FindFirstEvent@C3DAnimFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileTimingEventChunk@@@@XZ',address=0xaae640,size=132,instructions=55,relocations=4),
 dict(name='anim_file_helpers_find_next_event',source=F,symbol='?FindNextEvent@C3DAnimFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileTimingEventChunk@@@@ABV?$CCountedPointer@$$CBVC3DAnimFileTimingEventChunk@@@@@Z',address=0xaae6d0,size=209,instructions=79,relocations=4),
 dict(name='anim_file_helpers_find_first_event_const',source=F,symbol='?FindFirstEvent@C3DAnimFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileTimingEventChunk@@@@XZ',address=0xaae7b0,size=132,instructions=55,relocations=4),
 dict(name='anim_file_helpers_find_next_event_const',source=F,symbol='?FindNextEvent@C3DAnimFileHelpersChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileTimingEventChunk@@@@ABV2@@Z',address=0xaae840,size=209,instructions=79,relocations=4),
 dict(name='anim_file_movement_vector_has_movement_track',source=F,symbol='?HasMovementTrack@C3DAnimFileMovementVectorChunk@@QBE_NXZ',address=0xaae920,size=71,instructions=26,relocations=3),
 dict(name='anim_file_movement_vector_get_movement_track',source=F,symbol='?GetMovementTrack@C3DAnimFileMovementVectorChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@XZ',address=0xaae970,size=132,instructions=55,relocations=4),
 dict(name='anim_file_movement_vector_get_movement_track_const',source=F,symbol='?GetMovementTrack@C3DAnimFileMovementVectorChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileSequenceChunk@@@@XZ',address=0xaaea00,size=132,instructions=55,relocations=4),
 dict(name='anim_file_object_find_first_sequence',source=F,symbol='?FindFirstSequence@C3DAnimFileObjectChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@XZ',address=0xab0260,size=132,instructions=55,relocations=4),
 dict(name='anim_file_object_find_next_sequence',source=F,symbol='?FindNextSequence@C3DAnimFileObjectChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileSequenceChunk@@@@ABV?$CCountedPointer@$$CBVC3DAnimFileSequenceChunk@@@@@Z',address=0xab02f0,size=209,instructions=79,relocations=4),
 dict(name='anim_file_object_find_first_sequence_const',source=F,symbol='?FindFirstSequence@C3DAnimFileObjectChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileSequenceChunk@@@@XZ',address=0xab03d0,size=132,instructions=55,relocations=4),
 dict(name='anim_file_object_find_next_sequence_const',source=F,symbol='?FindNextSequence@C3DAnimFileObjectChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileSequenceChunk@@@@ABV2@@Z',address=0xab0460,size=209,instructions=79,relocations=4),
 dict(name='anim_file_object_has_bone_mask',source=F,symbol='?HasBoneMask@C3DAnimFileObjectChunk@@QBE_NXZ',address=0xab0540,size=69,instructions=25,relocations=3),
 dict(name='anim_file_object_get_bone_mask',source=F,symbol='?GetBoneMask@C3DAnimFileObjectChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileBoneMaskChunk@@@@XZ',address=0xab0590,size=132,instructions=55,relocations=4),
 dict(name='anim_file_object_peek_bone_mask_const',source=F,symbol='?PeekBoneMask@C3DAnimFileObjectChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileBoneMaskChunk@@@@XZ',address=0xab0620,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_list_find_first_material',source=F,symbol='?FindFirstMaterial@C3DMeshFileMaterialListChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMaterialChunk@@@@XZ',address=0xac04c0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_list_find_next_material',source=F,symbol='?FindNextMaterial@C3DMeshFileMaterialListChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMaterialChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileMaterialChunk@@@@@Z',address=0xac0550,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_material_list_find_first_material_const',source=F,symbol='?FindFirstMaterial@C3DMeshFileMaterialListChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMaterialChunk@@@@XZ',address=0xac0630,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_list_find_next_material_const',source=F,symbol='?FindNextMaterial@C3DMeshFileMaterialListChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMaterialChunk@@@@ABV2@@Z',address=0xac06c0,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_material_list_find_first_engine_material',source=F,symbol='?FindFirstEngineMaterial@C3DMeshFileMaterialListChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileEngineMaterialChunk@@@@XZ',address=0xac07a0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_list_find_next_engine_material',source=F,symbol='?FindNextEngineMaterial@C3DMeshFileMaterialListChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileEngineMaterialChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileEngineMaterialChunk@@@@@Z',address=0xac0830,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_material_list_find_first_engine_material_const',source=F,symbol='?FindFirstEngineMaterial@C3DMeshFileMaterialListChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileEngineMaterialChunk@@@@XZ',address=0xac0910,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_list_find_next_engine_material_const',source=F,symbol='?FindNextEngineMaterial@C3DMeshFileMaterialListChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileEngineMaterialChunk@@@@ABV2@@Z',address=0xac09a0,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_material_get_extended_properties',source=F,symbol='?GetExtendedProperties@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMaterialExtChunk@@@@XZ',address=0xac0ae0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_get_extended_properties_const',source=F,symbol='?GetExtendedProperties@C3DMeshFileMaterialChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMaterialExtChunk@@@@XZ',address=0xac0b70,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_find_first_map',source=F,symbol='?FindFirstMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@XZ',address=0xac0ce0,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_find_next_map',source=F,symbol='?FindNextMap@C3DMeshFileMaterialChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileMapChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileMapChunk@@@@@Z',address=0xac0d70,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_material_find_first_map_const',source=F,symbol='?FindFirstMap@C3DMeshFileMaterialChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMapChunk@@@@XZ',address=0xac0e50,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_material_find_next_map_const',source=F,symbol='?FindNextMap@C3DMeshFileMaterialChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileMapChunk@@@@ABV2@@Z',address=0xac0ee0,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_cloth_find_first_group',source=F,symbol='?FindFirstGroup@C3DMeshFileClothChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileGroupChunk@@@@XZ',address=0xac6320,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_cloth_find_next_group',source=F,symbol='?FindNextGroup@C3DMeshFileClothChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileGroupChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@@Z',address=0xac63b0,size=209,instructions=79,relocations=4),
 dict(name='mesh_file_cloth_find_first_group_const',source=F,symbol='?FindFirstGroup@C3DMeshFileClothChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@XZ',address=0xac6490,size=132,instructions=55,relocations=4),
 dict(name='mesh_file_cloth_find_next_group_const',source=F,symbol='?FindNextGroup@C3DMeshFileClothChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileGroupChunk@@@@ABV2@@Z',address=0xac6520,size=209,instructions=79,relocations=4),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-mesh-file-finders.json',[],
  'Mesh/anim-file chunk typed sub-chunk finders (root, helpers, anim object, material list/material, cloth)')
 print('NATIVE_ANIM_MESH_FILE_FINDERS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
