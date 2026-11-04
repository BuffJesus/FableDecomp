"""Fresh VC71 parity gate: mesh data bank / steering / navigator roots in 0xA20000..0xA30000 - STLport
instantiations of the mesh data bank (entry vector fill, counted-pointer copy, animation tree clear, mesh pointer assign)
and CNavigatorAStar::GetCurrentDestination, correctly owned replacements for misattributed stand-ins. Identities from workflow
range-exact-roots run wf_951982f1-122 (adversarially verified; journal 2026-10-28)."""
import sys
from pathlib import Path
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'tools/decomp_pipeline'))
import native_exact_roots
ADDRESS={
 '??$?4VC3DMesh2@@@?$CCountedPointer@$$CBVC3DMesh2@@@@QAEAAV0@ABV?$CCountedPointer@VC3DMesh2@@@@@Z':0xa2acc0,
 '??$__uninitialized_copy@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@@_STL@@YIPAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@00ABU__false_type@0@@Z':0xa2a610,
 '??$fill@PAPAVCMeshDataBankEntry@@PAV1@@_STL@@YIXPAPAVCMeshDataBankEntry@@0ABQAV1@@Z':0xa291a0,
 '??3@YAXPAX@Z':0xbfe9bc,
 '?Close@CBankFileAsync@@UAEXXZ':0x9d58a0,
 '?CloseMeshes@CMeshDataBank@@AAEXXZ':0xa26900,
 '?IsFinished@CBankFileAsyncData@@QBE_NXZ':0x9d54e0,
 '?SetResourceSize@CResource@@QAEXK@Z':0x9fbf20,
 '?Sizeof@CAnimationData@C3DAnimationInfo@@QBEKXZ':0xa25b20,
 '?_M_erase@?$_Rb_tree@KU?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@U?$_Select1st@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@@2@@_STL@@QAEXPAU?$_Rb_tree_node@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@@2@@Z':0xa2b9d0,
 '?_M_insert_overflow@?$vector@PAVCMeshDataBankEntry@@V?$allocator@PAVCMeshDataBankEntry@@@_STL@@@_STL@@QAEXPAPAVCMeshDataBankEntry@@ABQAV3@ABU__true_type@2@I_N@Z':0xa2be00,
 '_free':0xbfea14,
 '_malloc':0xbfea0e,
 '_memmove':0xbfeae6,
}
ROOTS=[
 dict(name='fill_cmeshdatabankentry',source=R/'rebuild/src/compiled/00/a2/STLMeshDataBankEntryVector_Fill_00a291a0.cpp',symbol='??$fill@PAPAVCMeshDataBankEntry@@PAV1@@_STL@@YIXPAPAVCMeshDataBankEntry@@0ABQAV1@@Z',address=0xa291a0,size=31,instructions=12,relocations=0),
 dict(name='fill_insert_cmeshdatabankentry',source=R/'rebuild/src/compiled/00/a2/STLMeshDataBankEntryVector_Fill_00a291a0.cpp',symbol='?_M_fill_insert@?$vector@PAVCMeshDataBankEntry@@V?$allocator@PAVCMeshDataBankEntry@@@_STL@@@_STL@@QAEXPAPAVCMeshDataBankEntry@@IABQAV3@@Z',address=0xa2c0f0,size=276,instructions=110,relocations=6),
 dict(name='uninitialized_copy_ccountedpointer_cmeshdatabankentry',source=R/'rebuild/src/compiled/00/a2/CMeshDataBankEntryCountedPointer_UninitializedCopy_00a2a610.cpp',symbol='??$__uninitialized_copy@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@@_STL@@YIPAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@00ABU__false_type@0@@Z',address=0xa2a610,size=50,instructions=20,relocations=0),
 dict(name='ccountedpointer_const_c3dmesh2_assign_c3dmesh2',source=R/'rebuild/src/compiled/00/a2/CCountedPointerConstC3DMesh2_AssignC3DMesh2_00a2acc0.cpp',symbol='??$?4VC3DMesh2@@@?$CCountedPointer@$$CBVC3DMesh2@@@@QAEAAV0@ABV?$CCountedPointer@VC3DMesh2@@@@@Z',address=0xa2acc0,size=72,instructions=32,relocations=1),
 dict(name='cnavigatorastar_getcurrentdestination',source=R/'rebuild/src/compiled/00/a2/CNavigatorAStar_GetCurrentDestination_00a224c0.cpp',symbol='?GetCurrentDestination@CNavigatorAStar@@UBEABVC3DVector@@XZ',address=0xa224c0,size=4,instructions=2,relocations=0),
 dict(name='c3danimationinfo_animationstree_clear',source=R/'rebuild/src/compiled/00/a2/C3DAnimationInfo_AnimationsTree_clear_00a2c0b0.cpp',symbol='?clear@?$_Rb_tree@KU?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@U?$_Select1st@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKV?$CCountedPointer@VC3DAnimation2@@@@@_STL@@@2@@_STL@@QAEXXZ',address=0xa2c0b0,size=51,instructions=19,relocations=1),
]
def main():
 results=native_exact_roots.run(__file__,ROOTS,ADDRESS,'native-mesh-steering.json',[],
  'mesh data bank STLport instantiations and A* navigator destination (0xA20000..0xA30000)')
 print('NATIVE_MESH_STEERING_PASS',' '.join(r['name']+'='+r['grade'] for r in results))
if __name__=='__main__':main()
