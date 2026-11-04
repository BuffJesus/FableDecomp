"""Fresh VC71 parity gate: Out-of-line retail copies emitted by landed TUs (chunk setters, ctor/dtor copies, pool helpers)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??1CCharString@@QAE@XZ':0x99eae0,
 '??4CCharString@@QAEAAV0@ABV0@@Z':0x99efb0,
 '?Free@CHandleBasedMemoryPool@@QAEXJ@Z':0xa02b30,
 '??1CChunkedFileSuperChunk@@UAE@XZ':0xaa8490,
 '_free':0xbfea14,
 '??_7CFailedAllocationHandler@@6B@':0x129b874,
 '??_7CLoadingMemoryFailureHandler@@6B@':0x129b8c4,
 '__imp__DeleteCriticalSection@4':0x143fde4,
}
F0=R/'rebuild/src/compiled/00/ab/C3DMeshFileSubMeshChunk_Methods_00ab9af0.cpp'
F1=R/'rebuild/src/compiled/00/ab/C3DMeshFileBoneChunk_Methods_00ab9ea0.cpp'
F2=R/'rebuild/src/compiled/00/9d/CBankFileAsyncData_CBankFileAsyncData_009d5df0.cpp'
F3=R/'rebuild/src/compiled/00/99/CAFile_IsFilePathSyntaxValid_00998a30.cpp'
F4=R/'rebuild/src/compiled/00/9d/CLoadingMemoryFailureHandler_Handler_009d5eb0.cpp'
F5=R/'rebuild/src/compiled/00/aa/C3DMeshFileXChunks_00aa4b50.cpp'
F6=R/'rebuild/src/compiled/00/aa/C3DAnimFileSequenceChunk_Methods_00aaf5b0.cpp'
F7=R/'rebuild/src/compiled/00/a0/CHandleBasedMemoryPool_Areas_00a01d20.cpp'
ROOTS=[
 dict(name='submesh_set_name',source=F0,symbol='?SetName@C3DMeshFileSubMeshChunk@@QAEXABVCCharString@@@Z',address=0xab9aa0,size=8,instructions=2,relocations=1),
 dict(name='submesh_set_index',source=F0,symbol='?SetSubMeshIndex@C3DMeshFileSubMeshChunk@@QAEXJ@Z',address=0xab9ab0,size=10,instructions=3,relocations=0),
 dict(name='submesh_set_parent',source=F0,symbol='?SetParentIndex@C3DMeshFileSubMeshChunk@@QAEXJ@Z',address=0xab9ac0,size=10,instructions=3,relocations=0),
 dict(name='submesh_set_first_child',source=F0,symbol='?SetFirstChildIndex@C3DMeshFileSubMeshChunk@@QAEXJ@Z',address=0xab9ad0,size=10,instructions=3,relocations=0),
 dict(name='submesh_set_next_sibling',source=F0,symbol='?SetNextSiblingIndex@C3DMeshFileSubMeshChunk@@QAEXJ@Z',address=0xab9ae0,size=10,instructions=3,relocations=0),
 dict(name='bone_set_index',source=F1,symbol='?SetBoneIndex@C3DMeshFileBoneChunk@@QAEXJ@Z',address=0xab9e30,size=10,instructions=3,relocations=0),
 dict(name='bone_set_parent',source=F1,symbol='?SetParentIndex@C3DMeshFileBoneChunk@@QAEXJ@Z',address=0xab9e40,size=10,instructions=3,relocations=0),
 dict(name='bone_set_first_child',source=F1,symbol='?SetFirstChildIndex@C3DMeshFileBoneChunk@@QAEXJ@Z',address=0xab9e50,size=10,instructions=3,relocations=0),
 dict(name='bone_set_next_sibling',source=F1,symbol='?SetNextSiblingIndex@C3DMeshFileBoneChunk@@QAEXJ@Z',address=0xab9e60,size=10,instructions=3,relocations=0),
 dict(name='bone_set_localization',source=F1,symbol='?SetLocalizationMatrix@C3DMeshFileBoneChunk@@QAEXABVCMatrix3x4@@@Z',address=0xab9e70,size=21,instructions=9,relocations=0),
 dict(name='bone_set_name',source=F1,symbol='?SetName@C3DMeshFileBoneChunk@@QAEXABVCCharString@@@Z',address=0xab9e90,size=8,instructions=2,relocations=1),
 dict(name='threaded_object_desc_ctor',source=F2,symbol='??0CThreadedFileObjectDesc@@QAE@XZ',address=0x98deb0,size=27,instructions=9,relocations=0),
 dict(name='expected_text_type_set',source=F3,symbol='?Set@CExpectedTextType@CAFile@@QAEX_N000@Z',address=0x997410,size=30,instructions=9,relocations=0),
 dict(name='loading_failure_handler_dtor',source=F4,symbol='??1CLoadingMemoryFailureHandler@@UAE@XZ',address=0x9d5ee0,size=43,instructions=15,relocations=4),
 dict(name='vertex_compression_params_ctor',source=F5,symbol='??0CVertexCompressionParams@@QAE@XZ',address=0x9e27b0,size=33,instructions=12,relocations=0),
 dict(name='anim_sequence_dtor',source=F6,symbol='??1C3DAnimFileSequenceChunk@@UAE@XZ',address=0xa55420,size=35,instructions=13,relocations=3),
 dict(name='pool_free_list_index',source=F7,symbol='?GetFreeListIndex@CHandleBasedMemoryPool@@AAEKJ@Z',address=0xa01d00,size=22,instructions=6,relocations=0),
 dict(name='scoped_handle_initialise',source=F7,symbol='?Initialise@CScopedMemoryHandle@@QAEXPAVCHandleBasedMemoryPool@@J@Z',address=0xa039f0,size=65,instructions=22,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-out-of-line-copies.json',[],
  'Out-of-line retail copies emitted by landed TUs (chunk setters, ctor/dtor copies, pool helpers)')
 print('NATIVE_OUT_OF_LINE_COPIES_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
