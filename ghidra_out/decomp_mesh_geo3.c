INFO  Using log config file: jar:file:/D:/Subuwu/tools/ghidra-public/Ghidra/Framework/Generic/lib/Generic.jar!/generic.log4j.xml (LoggingInitialization)  
INFO  Using log file: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\application.log (LoggingInitialization)  
INFO  Loading user preferences: C:\Users\Cornelio\AppData\Roaming\ghidra\ghidra_12.1_PUBLIC\preferences (Preferences)  
INFO  Searching for classes... (ClassSearcher)  
INFO  Class search complete (1021 ms) (ClassSearcher)  
INFO  Initializing SSL Context (DefaultSSLContextInitializer)  
INFO  Initializing Random Number Generator... (SecureRandomFactory)  
INFO  Random Number Generator initialization complete: SHA1PRNG (SecureRandomFactory)  
INFO  Trust manager disabled, cacerts have not been set (DefaultTrustManagerFactory)  
INFO  Headless startup complete (2384 ms) (AnalyzeHeadless)  
INFO  Class searcher loaded 60 extension points (20 false positives) (ClassSearcher)  
INFO  HEADLESS Script Paths:
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\SwiftDemangler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\GnuDemangler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\BytePatterns\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\JVM\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\SystemEmulation\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\8051\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\FileFormats\ghidra_scripts
    D:\Documents\FableTLC\tools\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\BSim\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\VersionTracking\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\PDB\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\WildcardAssembler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\PIC\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\Base\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Extensions\XEXLoaderWV\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\DecompilerDependent\ghidra_scripts
    C:\Users\Cornelio\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\DATA\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\Decompiler\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Processors\Atmel\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\PyGhidra\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\MicrosoftCodeAnalyzer\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Debug\Debugger-rmi-trace\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Debug\Debugger\ghidra_scripts
    D:\Subuwu\tools\ghidra-public\Ghidra\Features\FunctionID\ghidra_scripts (HeadlessAnalyzer)  
INFO  HEADLESS: execution starts (HeadlessAnalyzer)  
INFO  Opening existing project: D:\Documents\FableTLC\ghidra_proj\FableTLC (HeadlessAnalyzer)  
INFO  Opening project: D:\Documents\FableTLC\ghidra_proj\FableTLC (HeadlessProject)  
INFO  REPORT: Processing read-only project file: /Fable.exe (HeadlessAnalyzer)  
INFO  REPORT: Execute script: DecompFuncs.java '0xa24520' '0xa25d20'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== BuildMesh @ 00a24520 ====================

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


==================== BuildMesh @ 00a25d20 ====================

/* [bsim sim=0.8625164805479172 <- ego_r]
   public: void __thiscall CPhysicsMeshInfo::CMeshData::BuildMesh(void) */

void __thiscall CPhysicsMeshInfo::CMeshData::BuildMesh(CMeshData *this)

{
  int *piVar1;
  CResourceList *this_00;
  C3DAnimFileLoader *this_01;
  int *piVar2;
  void *pvVar3;
  CPhysicsMesh *this_02;
  undefined4 uVar4;
  long lVar5;
  ulong uVar6;
  int iVar7;
  int iVar8;
  CHandleBasedMemoryPool *pCVar9;
  C3DMeshFile *local_8;
  int *piStack_4;
  
  iVar8 = *(int *)(*(int *)(this + 0x28) + 4);
  this_00 = (CResourceList *)(iVar8 + 0x2e8);
  CResourceList::MakeFreeSpace(this_00,*(ulong *)(*(int *)(this + 0x1c) + 0x10));
  pCVar9 = *(CHandleBasedMemoryPool **)(*(CBankFileAsyncData **)(this + 0x1c) + 0x10);
  this_01 = *(C3DAnimFileLoader **)(*(int *)(*(int *)(this + 0x28) + 4) + 0x164);
  pvVar3 = CBankFileAsyncData::GetData(*(CBankFileAsyncData **)(this + 0x1c));
  C3DAnimFileLoader::LoadAnimFileCompressed(this_01,&local_8,(ulong)pvVar3,pCVar9);
  this_02 = ::operator_new(0xc0);
  if (this_02 == (CPhysicsMesh *)0x0) {
    uVar4 = 0;
  }
  else {
    uVar4 = CPhysicsMesh::CPhysicsMesh
                      (this_02,local_8,*(CMovableResourceMemoryPool **)(iVar8 + 0x370));
  }
  piVar2 = *(int **)(this + 0x24);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x24) = 0;
  }
  *(undefined4 *)(this + 0x24) = uVar4;
  piVar2 = *(int **)(this + 0x20);
  if (piVar2 != (int *)0x0) {
    *piVar2 = *piVar2 + -1;
    if (**(int **)(this + 0x20) == 0) {
      (*(code *)(*(int **)(this + 0x20))[1])();
      operator_delete(*(void **)(this + 0x20));
    }
  }
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  if (*(CPhysicsMesh **)(this + 0x24) == (CPhysicsMesh *)0x0) {
    iVar8 = 4;
  }
  else {
    lVar5 = CPhysicsMesh::Sizeof(*(CPhysicsMesh **)(this + 0x24));
    iVar8 = lVar5 + 4;
  }
  if (*(CVertexBufferWin32 **)(this + 0x1c) == (CVertexBufferWin32 *)0x0) {
    iVar7 = 8;
  }
  else {
    uVar6 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 0x1c));
    iVar7 = uVar6 + 8;
  }
  CResource::SetResourceSize((CResource *)this,iVar7 + 0x2c + iVar8);
  CTCActivationTrigger::OnKill((CTCActivationTrigger *)this);
  CResourceList::AddResource(this_00,(CResource *)this);
  CResourceList::MakeFreeSpace(this_00,0);
  if (piStack_4 != (int *)0x0) {
    *piStack_4 = *piStack_4 + -1;
    if (*piStack_4 == 0) {
      (*(code *)piStack_4[1])();
      operator_delete(piStack_4);
    }
  }
  return;
}


INFO  REPORT: Execute script: DecompByName.java 'D:/Documents/FableTLC/ghidra_out/decomp_mesh_geo3.c' 'DoLock@CVertexBufferWin32' 'DoLock@CIndexBuffer' 'CVertexBufferWin32' 'CIndexBufferWin32' 'DoCreate' 'GetStride' 'GetVertexSize' 'C3DMeshLODInfo::Build'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompByName.java (HeadlessAnalyzer)  
DecompByName: 4 matching functions
DecompByName: ok=4 fail=0 -> D:/Documents/FableTLC/ghidra_out/decomp_mesh_geo3.c
