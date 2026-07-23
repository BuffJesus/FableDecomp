INFO  DecompFuncs.java> ==================== ?OnResourceLoaded@C3DMeshLODInfo@@UAEXXZ @ 00a25470 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall C3DMeshLODInfo::OnResourceLoaded(void) */

void __thiscall C3DMeshLODInfo::_OnResourceLoaded_C3DMeshLODInfo__UAEXXZ(C3DMeshLODInfo *this)

{
  bool bVar1;
  
  if (*(int *)(this + 0x24) != 0) {
    bVar1 = BuildMesh(this,true,false);
    if (!bVar1) {
      C3DMeshInfo::RemoveLOD(*(C3DMeshInfo **)(this + 0x2c),(uint)(byte)this[0x30],false);
    }
  }
  return;
}

 (GhidraScript)  
INFO  DecompFuncs.java> ==================== BuildMesh @ 00a24520 ==================== (GhidraScript)  
INFO  DecompFuncs.java> 
/* [bsim sim=0.8586651218248263 <- ego_r]
   private: bool __thiscall C3DMeshLODInfo::BuildMesh(bool,bool) */

bool __thiscall C3DMeshLODInfo::BuildMesh(C3DMeshLODInfo *this,bool param_1,bool param_2)

{
  CCountedPointer<CIPhysicsFacesSegmentBase_const_> *this_00;
  CBankFile *this_01;
  CHandleBasedMemoryPool *this_02;
  int *piVar1;
  bool bVar2;
  ulong uVar3;
  uint uVar4;
  void *pvVar5;
  C3DMesh2 *pCVar6;
  CIPhysicsFacesSegmentBase *pCVar7;
  C3DPrimitive2 *this_03;
  int iVar8;
  CResourceBank *this_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  uint uVar9;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  byte bVar11;
  byte bVar12;
  int iVar13;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString local_30 [4];
  CGraphicDataBank *local_2c;
  int local_28;
  undefined4 local_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *local_20;
  CMemoryDataInputStream local_1c [8];
  ulong local_14;
  
  this_01 = *(CBankFile **)(*(int *)(this + 0x2c) + 4);
  uVar3 = CBankFile::GetEntryDataType(this_01,*(ulong *)(*(int *)(this + 0x2c) + 8));
  uVar4 = uVar3 & 0xf;
  uVar3 = *(ulong *)(*(CBankFileAsyncData **)(this + 0x24) + 0x10);
  pvVar5 = CBankFileAsyncData::GetData(*(CBankFileAsyncData **)(this + 0x24));
  CMemoryDataInputStream::CMemoryDataInputStream(local_1c,pvVar5,uVar3);
  local_28 = DAT_01397248;
  DAT_01397248 = DAT_01397248 + 1;
  local_24 = DAT_013d2d6c;
  local_20 = DAT_013d2d70;
  DAT_013d2d6c = *(undefined4 *)(this_01 + 0x370);
  DAT_013d2d70 = *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this_01 + 0x368);
  if (this[0x30] == (C3DMeshLODInfo)0x4) {
    CResourceList::MakeFreeSpace
              ((CResourceList *)(*(int *)(*(int *)(this + 0x2c) + 4) + 0x1f0),local_14);
  }
  local_2c = *(CGraphicDataBank **)(this_01 + 0x3d4);
  this_02 = *(CHandleBasedMemoryPool **)(this_01 + 0x370);
  CBankFile::GetEntrySymbol(this_01,(ulong)local_30);
  pCVar6 = ::operator_new(0xec);
  if (pCVar6 == (C3DMesh2 *)0x0) {
    pCVar7 = (CIPhysicsFacesSegmentBase *)0x0;
  }
  else {
    pCVar7 = (CIPhysicsFacesSegmentBase *)C3DMesh2::C3DMesh2(pCVar6);
  }
  this_00 = (CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(this + 0x1c);
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset(this_00,pCVar7);
  bVar2 = C3DMesh2::LoadBinary
                    (*(C3DMesh2 **)this_00,local_1c,(CMovableResourceMemoryPool *)this_02,local_2c,
                     param_2);
  if (!bVar2) {
    pCVar6 = ::operator_new(0xec);
    if (pCVar6 == (C3DMesh2 *)0x0) {
      pCVar7 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      pCVar7 = (CIPhysicsFacesSegmentBase *)C3DMesh2::C3DMesh2(pCVar6);
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset(this_00,pCVar7);
    CHandleBasedMemoryPool::Defragment(this_02);
    uVar3 = *(ulong *)(*(CBankFileAsyncData **)(this + 0x24) + 0x10);
    pvVar5 = CBankFileAsyncData::GetData(*(CBankFileAsyncData **)(this + 0x24));
    CMemoryDataInputStream::OpenMemorySource(local_1c,pvVar5,uVar3);
    bVar2 = C3DMesh2::LoadBinary
                      (*(C3DMesh2 **)this_00,local_1c,(CMovableResourceMemoryPool *)this_02,local_2c
                       ,false);
    ppVar10 = extraout_EDX;
    if (!bVar2) goto LAB_00a24778;
  }
  *(int *)(*(int *)this_00 + 0xe8) = local_28;
  if (local_2c != (CGraphicDataBank *)0x0) {
    C3DMesh2::CalculateTextureChangeFlags(*(C3DMesh2 **)this_00);
  }
  C3DMeshStats::InitialiseMeshGenerators
            (*(C3DMeshStats **)this_00,(CMeshDataBank *)this_01,local_30);
  pCVar6 = *(C3DMesh2 **)this_00;
  bVar11 = 1;
  _param_2 = 0;
  bVar12 = bVar11;
  if (*(int *)(pCVar6 + 0x90) != 0) {
    do {
      bVar12 = 0;
      if (bVar11 == 0) break;
      this_03 = C3DMesh2::LockPrimitive(pCVar6,_param_2);
      uVar9 = 0;
      if (((*(byte *)(*(int *)(this_03 + 4) * 0x30 + 0x20 + *(int *)(*(int *)this_00 + 0x8c)) & 2)
           != 0) && (*(char *)(*(int *)(*(int *)(this + 0x2c) + 4) + 0x3d8) != '\0')) {
        uVar9 = 2;
      }
      if (uVar4 == 5) {
        bVar2 = C3DPrimitive2::UploadAnimatedBuffers(this_03,uVar9 | 8);
      }
      else if (uVar4 == 2) {
        bVar2 = C3DPrimitive2::UploadRepeatedBuffers(this_03,uVar9,0x20);
      }
      else if (uVar4 == 4) {
        bVar2 = C3DPrimitive2::UploadRepeatedBuffers(this_03,uVar9,0x20);
      }
      else {
        bVar2 = C3DPrimitive2::UploadRepeatedBuffers(this_03,uVar9 | 8,(ulong)unaff_EDI);
      }
      bVar11 = bVar11 & bVar2;
      C3DMesh2::UnlockPrimitive(*(C3DMesh2 **)this_00,_param_2);
      pCVar6 = *(C3DMesh2 **)this_00;
      _param_2 = _param_2 + 1;
      bVar12 = bVar11;
    } while (_param_2 < *(uint *)(pCVar6 + 0x90));
  }
  DAT_013d2d6c = local_24;
  DAT_013d2d70 = local_20;
  ppVar10 = local_20;
  if (bVar12 != 0) {
    if (this[0x30] == (C3DMeshLODInfo)0x4) {
      this_04 = (CResourceBank *)(*(int *)(*(int *)(this + 0x2c) + 4) + 0x1ec);
    }
    else {
      this_04 = (CResourceBank *)(*(int *)(*(int *)(this + 0x2c) + 4) + 0x170);
    }
    CResourceBank::AddResource(this_04,(CResource *)this);
    piVar1 = *(int **)(this + 0x28);
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + -1;
      if (**(int **)(this + 0x28) == 0) {
        (*(code *)(*(int **)(this + 0x28))[1])();
        operator_delete(*(void **)(this + 0x28));
      }
    }
    *(undefined4 *)(this + 0x28) = 0;
    *(undefined4 *)(this + 0x24) = 0;
    if (*(C3DMesh2 **)this_00 == (C3DMesh2 *)0x0) {
      iVar13 = 8;
    }
    else {
      uVar3 = C3DMesh2::Sizeof(*(C3DMesh2 **)this_00);
      iVar13 = uVar3 + 8;
    }
    if (*(CVertexBufferWin32 **)(this + 0x24) == (CVertexBufferWin32 *)0x0) {
      iVar8 = 8;
    }
    else {
      uVar3 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 0x24));
      iVar8 = uVar3 + 8;
    }
    CResource::SetResourceSize((CResource *)this,iVar8 + 0x34 + iVar13);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_30,
               extraout_EDX_01,unaff_EDI);
    CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)local_1c);
    return true;
  }
LAB_00a24778:
  piVar1 = *(int **)(this + 0x20);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + -1;
    if (**(int **)(this + 0x20) == 0) {
      (*(code *)(*(int **)(this + 0x20))[1])();
      operator_delete(*(void **)(this + 0x20));
      ppVar10 = extraout_EDX_00;
    }
  }
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)this_00 = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_30,ppVar10,
             unaff_EDI);
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)local_1c);
  return false;
}

 (GhidraScript)  
INFO  REPORT: Save succeeded for processed file: /Fable.exe (HeadlessAnalyzer)  
