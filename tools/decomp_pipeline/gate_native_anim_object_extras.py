"""Fresh VC71 parity gate: Anim-file object setters, sub-chunk ctors, movement vector and convex-volume AddPlane."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?_M_insert_overflow@?$vector@VCPlane@@V?$allocator@VCPlane@@@_STL@@@_STL@@QAEXPAVCPlane@@ABV3@ABU__false_type@2@I_N@Z':0xa13720,
 '??0CChunkedFileChunk@@QAE@PBD@Z':0xaa7590,
 '??0CChunkedFileSuperChunk@@QAE@PBD@Z':0xaa8360,
 '??_C@_04GEKNONIJ@HLPR?$AA@':0x129dfcc,
 '??_C@_04NKNNJHIJ@MVEC?$AA@':0x129e7a4,
 '??_7C3DAnimFileHelpersChunk@@6B@':0x129e7c0,
 '??_7C3DAnimFileMovementVectorChunk@@6B@':0x129e83c,
 '??_C@_04EIEHPPNM@AMSK?$AA@':0x129e8f8,
 '??_7C3DAnimFileBoneMaskChunk@@6B@':0x129e980,
}
F0=R/'rebuild/src/compiled/00/aa/C3DFileContainerChunks_00aa9670.cpp'
F1=R/'rebuild/src/compiled/00/aa/C3DAnimFileMovementVectorChunk_Methods_00aae0f0.cpp'
F2=R/'rebuild/src/compiled/00/ab/C3DAnimFileObjectChunk_Methods_00ab0090.cpp'
F3=R/'rebuild/src/compiled/00/ab/C3DAnimFileBoneMaskChunk_Methods_00ab0110.cpp'
F4=R/'rebuild/src/compiled/00/aa/C3DMeshFileConvexVolumeChunk_Methods_00aa9ad0.cpp'
ROOTS=[
 dict(name='anim_helpers_ctor',source=F0,symbol='??0C3DAnimFileHelpersChunk@@QAE@XZ',address=0xaae090,size=23,instructions=8,relocations=3),
 dict(name='movement_vector_set_vector',source=F1,symbol='?SetVector@C3DAnimFileMovementVectorChunk@@QAEXABVC3DVector@@@Z',address=0xaae110,size=26,instructions=9,relocations=0),
 dict(name='movement_vector_ctor',source=F1,symbol='??0C3DAnimFileMovementVectorChunk@@QAE@XZ',address=0xaae2a0,size=23,instructions=8,relocations=3),
 dict(name='object_set_name',source=F2,symbol='?SetName@C3DAnimFileObjectChunk@@QAEXABVCCharString@@@Z',address=0xab0040,size=8,instructions=2,relocations=1),
 dict(name='object_set_sub_mesh_index',source=F2,symbol='?SetSubMeshIndex@C3DAnimFileObjectChunk@@QAEXJ@Z',address=0xab0050,size=10,instructions=3,relocations=0),
 dict(name='object_set_parent_index',source=F2,symbol='?SetParentIndex@C3DAnimFileObjectChunk@@QAEXJ@Z',address=0xab0060,size=10,instructions=3,relocations=0),
 dict(name='object_set_first_child_index',source=F2,symbol='?SetFirstChildIndex@C3DAnimFileObjectChunk@@QAEXJ@Z',address=0xab0070,size=10,instructions=3,relocations=0),
 dict(name='object_set_next_sibling_index',source=F2,symbol='?SetNextSiblingIndex@C3DAnimFileObjectChunk@@QAEXJ@Z',address=0xab0080,size=10,instructions=3,relocations=0),
 dict(name='bone_mask_ctor',source=F3,symbol='??0C3DAnimFileBoneMaskChunk@@QAE@XZ',address=0xab0700,size=34,instructions=12,relocations=3),
 dict(name='convex_volume_add_plane',source=F4,symbol='?AddPlane@C3DMeshFileConvexVolumeChunk@@QAEXABVCPlane@@@Z',address=0xaaaac0,size=75,instructions=29,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-anim-object-extras.json',[],
  'Anim-file object setters, sub-chunk ctors, movement vector and convex-volume AddPlane')
 print('NATIVE_ANIM_OBJECT_EXTRAS_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
