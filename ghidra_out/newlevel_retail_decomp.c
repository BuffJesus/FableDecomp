 '0x506f30' '0x502620' '0x502850' '0x5063e0' '0x502500' '0x500eb0' '0x501990' '0x500230' '0x4fc8a0' '0x4feec0' '0x506d40' '0x4fb4f0' '0x4fb880' '0x4fbf60' '0x4fdbc0' '0x4fe2a0' '0x638310' '0x6381a0' '0x51a240' '0x4382c0' '0x4ff560'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== AddLevelFromFile @ 00506f30 ==================== 

/* [bsim sim=0.9809966364128789 <- ego_r]
   protected: long __thiscall CWorldMap::AddLevelFromFile(class C2DCoordI const &,class CCharString
   const &) */

long __thiscall CWorldMap::AddLevelFromFile(CWorldMap *this,C2DCoordI *param_1,CCharString *param_2)

{
  CThingCreatureBase *this_00;
  int *piVar1;
  ulong uVar2;
  CGameJoystickManager *this_01;
  uint uVar3;
  char *pcVar4;
  int *piVar5;
  vector<CAnimationEntry_const*,std::allocator<CAnimationEntry_const*>_> *unaff_EDI;
  uint uVar6;
  CCharString local_10 [16];
  
  this_00 = (CThingCreatureBase *)(this + 0x20);
  uVar3 = 1;
  if (1 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    pcVar4 = (char *)(*(int *)this_00 + 0x6c);
    do {
      if (*pcVar4 == '\0') {
        piVar5 = (int *)(*(int *)this_00 + uVar3 * 0x48);
        if (piVar5 != (int *)0x0) goto LAB_00506fd8;
        break;
      }
      uVar3 = uVar3 + 1;
      pcVar4 = pcVar4 + 0x48;
    } while (uVar3 < (uint)((*(int *)(this + 0x24) - *(int *)this_00) / 0x48));
  }
  CThingCreatureBase::GetAnimsWithNameStartingWith
            (this_00,(CCharString *)((*(int *)(this + 0x24) - *(int *)this_00) / 0x48 + 1),unaff_EDI
            );
  uVar3 = (*(int *)(this + 0x24) - *(int *)this_00) / 0x48 - 1;
  piVar5 = (int *)(*(int *)this_00 + uVar3 * 0x48);
LAB_00506fd8:
  *(undefined1 *)(piVar5 + 9) = 1;
  CCharString::operator=((CCharString *)(piVar5 + 6),param_2);
  *(undefined1 *)((int)piVar5 + 0x25) = 0;
  piVar1 = (int *)GetLevelDimensions(this,local_10);
  *piVar5 = *piVar1;
  piVar5[1] = piVar1[1];
  piVar5[2] = piVar1[2];
  piVar5[3] = piVar1[3];
  piVar5[2] = piVar5[2] + (*(int *)param_1 - *piVar5);
  piVar5[3] = piVar5[3] + (*(int *)(param_1 + 4) - piVar5[1]);
  *piVar5 = *(int *)param_1;
  piVar5[1] = *(int *)(param_1 + 4);
  uVar2 = AllocateMapUID(this,uVar3);
  piVar5[8] = uVar2;
  uVar6 = uVar3;
  this_01 = CGameJoystickManager::Get();
  CEngineManager::AddMap((CEngineManager *)this_01,uVar6);
  LoadLoadableGlobalThingsForLevel(this,uVar3);
  return uVar3;
}

 
==================== LoadLevel @ 00502620 ==================== 

/* [bsim sim=0.7179064342404959 <- ego_r]
   protected: void __thiscall CWorldMap::LoadLevel(long,enum CWorldMap::ELoadType) */

void __thiscall CWorldMap::LoadLevel(CWorldMap *this,long param_1,ELoadType param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  CWADFile *pCVar4;
  CIPhysicsFacesSegmentBase *pCVar5;
  CDiskFileWin32 *pCVar6;
  CWideString *pCVar7;
  int iVar8;
  undefined8 uVar9;
  FileAccessType FVar10;
  ulong uVar11;
  CWideString local_1c [4];
  CWADBank *local_18;
  undefined4 local_14;
  undefined4 local_10;
  int *local_c;
  undefined4 local_8;
  int *local_4;
  
  local_14 = 0;
  iVar8 = param_1 * 0x48;
  CPersistContext::GetDefaultVal<CAnimationSet>();
  GetThingFilenameFromLevelFilename(local_1c);
  CCharString::~CCharString((CCharString *)&local_18);
  local_8 = 0;
  local_4 = (int *)0x0;
  local_10 = 0;
  local_c = (int *)0x0;
  if (this[0xa8] == (CWorldMap)0x0) {
    pCVar6 = ::operator_new(0x1c);
    if (pCVar6 == (CDiskFileWin32 *)0x0) {
      pCVar5 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      uVar11 = 2;
      FVar10 = 1;
      pCVar7 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
      pCVar5 = (CIPhysicsFacesSegmentBase *)
               CDiskFileWin32::CDiskFileWin32(pCVar6,pCVar7,FVar10,uVar11);
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_8,pCVar5);
    if (pCVar6 != (CDiskFileWin32 *)0x0) {
      CCharString::~CCharString((CCharString *)&local_14);
    }
    pCVar6 = ::operator_new(0x1c);
    if (pCVar6 != (CDiskFileWin32 *)0x0) {
      pCVar5 = (CIPhysicsFacesSegmentBase *)CDiskFileWin32::CDiskFileWin32(pCVar6,local_1c,1,2);
      goto LAB_00502772;
    }
  }
  else {
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CWADFile *)0x0) {
      pCVar5 = (CIPhysicsFacesSegmentBase *)0x0;
    }
    else {
      local_18 = *(CWADBank **)(this + 0xa0);
      CPersistContext::GetDefaultVal<CAnimationSet>();
      pCVar5 = (CIPhysicsFacesSegmentBase *)CWADFile::CWADFile(pCVar4,local_18);
    }
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
              ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_8,pCVar5);
    if (pCVar4 != (CWADFile *)0x0) {
      CCharString::~CCharString((CCharString *)&local_14);
    }
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 != (CWADFile *)0x0) {
      pCVar5 = (CIPhysicsFacesSegmentBase *)CWADFile::CWADFile(pCVar4,*(CWADBank **)(this + 0xa0));
      goto LAB_00502772;
    }
  }
  pCVar5 = (CIPhysicsFacesSegmentBase *)0x0;
LAB_00502772:
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
            ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_10,pCVar5);
  piVar3 = local_4;
  if (local_c != (int *)0x0) {
    *local_c = *local_c + 1;
  }
  if (local_4 != (int *)0x0) {
    *local_4 = *local_4 + 1;
    *local_4 = *local_4 + 1;
  }
  FUN_00501150(param_1,local_8,local_4,local_8,local_4,local_10,local_c,param_2);
  iVar1 = *(int *)(this + 0x20);
  pCVar7 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
  uVar9 = CAFile::GetFileModifiedTime(pCVar7);
  *(int *)(iVar8 + 0x30 + iVar1) = (int)uVar9;
  *(int *)(iVar8 + 0x34 + iVar1) = (int)((ulonglong)uVar9 >> 0x20);
  CCharString::~CCharString((CCharString *)&param_1);
  iVar1 = *(int *)(this + 0x20);
  uVar9 = CAFile::GetFileModifiedTime(local_1c);
  piVar2 = local_c;
  *(int *)(iVar8 + 0x38 + iVar1) = (int)uVar9;
  *(int *)(iVar8 + 0x3c + iVar1) = (int)((ulonglong)uVar9 >> 0x20);
  if ((local_c != (int *)0x0) && (*local_c = *local_c + -1, *local_c == 0)) {
    (*(code *)local_c[1])();
    operator_delete(piVar2);
  }
  if ((piVar3 != (int *)0x0) && (*piVar3 = *piVar3 + -1, *piVar3 == 0)) {
    (*(code *)piVar3[1])();
    operator_delete(piVar3);
  }
  CCharString::~CCharString((CCharString *)local_1c);
  return;
}

 
==================== CacheMaps @ 00502850 ==================== 

/* [bsim sim=0.9189216915381451 <- ego_r]
   protected: void __thiscall CWorldMap::CacheMaps(enum CWorldMap::ELoadType) */

void __thiscall CWorldMap::CacheMaps(CWorldMap *this,ELoadType param_1)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  if ((DAT_013756f6 == '\0') &&
     (iVar5 = 0, 0 < (*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    iVar4 = 0;
    do {
      if ((*(char *)(*(int *)(this + 0x20) + iVar4 + 0x24) != '\0') &&
         (*(char *)(*(int *)(this + 0x20) + iVar4 + 0x28) != '\0')) {
        lVar3 = GetMinPlayerDistanceFromMap(this,iVar5);
        cVar1 = (**(code **)(*(int *)this + 0x28))(iVar5);
        if (cVar1 == '\0') {
          if ((((*(char *)(*(int *)(this + 0x20) + 0x27 + iVar4) == '\0') && (lVar3 < 0x30)) &&
              (LoadLevel(this,iVar5,1), param_1 == 0)) &&
             (bVar2 = CTCUserActionPointBase::IsActionPointFree
                                (*(CTCUserActionPointBase **)(this + 0xbc)), bVar2)) {
            do {
              (**(code **)(**(int **)(this + 0xbc) + 4))();
              bVar2 = CTCUserActionPointBase::IsActionPointFree
                                (*(CTCUserActionPointBase **)(this + 0xbc));
            } while (bVar2);
          }
        }
        else if ((0x3f < lVar3) &&
                (*(char *)(*(int *)(this + 0x20) + 0x27 + param_1 * 0x48) == '\0')) {
          uStack_c = 0;
          uStack_8 = 0;
          uStack_4 = 0;
          CMessageEventManager::AddMessage
                    (*(CMessageEventManager **)(*(int *)(this + 8) + 0x60),0x25,0x32,
                     (C3DVector *)&uStack_c,(CThing *)0x0,(CThing *)0x0,0.0,0.0,iVar5);
          UnloadLevel(this,iVar5);
        }
      }
      iVar5 = iVar5 + 1;
      iVar4 = iVar4 + 0x48;
    } while (iVar5 < (*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48);
  }
  return;
}

 
==================== SetPlayerPos @ 005063e0 ==================== 

/* [bsim sim=0.9074214894175532 <- ego_r]
   public: void __thiscall CWorldMap::SetPlayerPos(long,class C3DVector const &,enum
   CWorldMap::ELoadType) */

void __thiscall
CWorldMap::SetPlayerPos(CWorldMap *this,long param_1,C3DVector *param_2,ELoadType param_3)

{
  undefined4 *puVar1;
  char cVar2;
  int iVar3;
  long lVar4;
  long lVar5;
  
  if (DAT_013756f6 != '\0') {
    iVar3 = (**(code **)(*(int *)this + 0x40))(param_2);
    cVar2 = (**(code **)(*(int *)this + 0x28))(iVar3);
    if (cVar2 == '\0') {
      lVar4 = GetRegionNumberMapIsIn(this,iVar3);
      if (*(char *)(param_1 * 0xd + *(int *)(this + 0x78)) != '\0') {
        lVar5 = (**(code **)(*(int *)this + 0x40))(param_1 * 0xd + 1 + *(int *)(this + 0x78));
        lVar5 = GetRegionNumberMapIsIn(this,lVar5);
        if ((0 < lVar5) && (lVar5 != lVar4)) {
          UnloadRegion(this,lVar5,true);
        }
      }
      if (lVar4 < 1) {
        if (0 < iVar3) {
          LoadLevel(this,iVar3,param_3);
        }
      }
      else {
        LoadRegion(this,lVar4,param_3,true);
      }
    }
  }
  *(undefined1 *)(param_1 * 0xd + *(int *)(this + 0x78)) = 1;
  puVar1 = (undefined4 *)(*(int *)(this + 0x78) + 1 + param_1 * 0xd);
  *puVar1 = *(undefined4 *)param_2;
  puVar1[1] = *(undefined4 *)(param_2 + 4);
  puVar1[2] = *(undefined4 *)(param_2 + 8);
  CacheMaps(this,param_3);
  return;
}

 
==================== StartMovePlayerPos @ 00502500 ==================== 

/* [bsim sim=0.8741666279572439 <- ego_r]
   public: void __thiscall CWorldMap::StartMovePlayerPos(long,class C3DVector const &,enum
   CWorldMap::ELoadType) */

void __thiscall
CWorldMap::StartMovePlayerPos(CWorldMap *this,long param_1,C3DVector *param_2,ELoadType param_3)

{
  undefined4 *puVar1;
  char cVar2;
  long lVar3;
  long lVar4;
  int unaff_EBX;
  int unaff_retaddr;
  
  lVar3 = (**(code **)(*(int *)this + 0x40))(param_2);
  if (DAT_013756f6 == '\0') {
    UnloadAllMapsExceptPlayerPos(this);
    cVar2 = (**(code **)(*(int *)this + 0x28))(lVar3);
    if (cVar2 == '\0') {
      (**(code **)(*(int *)this + 0x20))(lVar3);
    }
  }
  else {
    lVar4 = (**(code **)(*(int *)this + 0x40))(unaff_retaddr * 0xd + 1 + *(int *)(this + 0x78));
    lVar4 = GetRegionNumberMapIsIn(this,lVar4);
    lVar3 = GetRegionNumberMapIsIn(this,lVar3);
    if (lVar4 != lVar3) {
      UnloadRegion(this,lVar4,true);
      LoadRegion(this,lVar3,param_1,true);
    }
  }
  *(undefined1 *)(unaff_EBX * 0xd + *(int *)(this + 0x78)) = 1;
  puVar1 = (undefined4 *)(*(int *)(this + 0x78) + 1 + unaff_EBX * 0xd);
  *puVar1 = *(undefined4 *)param_2;
  puVar1[1] = *(undefined4 *)(param_2 + 4);
  puVar1[2] = *(undefined4 *)(param_2 + 8);
  return;
}

 
==================== LoadThingsWithSectionNames @ 00500eb0 ==================== 

/* [bsim sim=0.7325286036503299 <- ego_r]
   public: void __thiscall CWorldMap::LoadThingsWithSectionNames(class std::set<class
   CCharString,struct std::less<class CCharString>,class std::allocator<class CCharString> > const
   &,enum CWorldMap::ELoadType) */

void __thiscall
CWorldMap::LoadThingsWithSectionNames
          (CWorldMap *this,
          set<CCharString,std::less<CCharString>,std::allocator<CCharString>_> *param_1,
          ELoadType param_2)

{
  undefined4 uVar1;
  int *piVar2;
  int *piVar3;
  bool bVar4;
  CWADFile *this_00;
  CIPhysicsFacesSegmentBase *pCVar5;
  CDiskFileWin32 *this_01;
  undefined1 *puVar6;
  CLevelFiles *pCVar7;
  CLevelFiles *pCVar8;
  CWideString local_44 [4];
  undefined4 *local_40;
  CCharString local_3c [4];
  CSpellContainerList *pCStack_38;
  int *piStack_34;
  undefined4 local_30;
  int *local_2c;
  CLevelFiles *local_28;
  CLevelFiles *local_24;
  undefined4 local_20;
  CLevelFiles local_1c [4];
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  local_40 = *(undefined4 **)(this + 0x90);
  local_28 = (CLevelFiles *)0x0;
  local_24 = (CLevelFiles *)0x0;
  local_20 = 0;
  if (local_40 != *(undefined4 **)(this + 0x94)) {
    do {
      uVar1 = *local_40;
      CPersistContext::GetDefaultVal<CAnimationSet>();
      GetThingFilenameFromLevelFilename(local_44);
      CCharString::~CCharString(local_3c);
      local_30 = 0;
      local_2c = (int *)0x0;
      if (this[0xa8] == (CWorldMap)0x0) {
        this_01 = ::operator_new(0x1c);
        if (this_01 == (CDiskFileWin32 *)0x0) goto LAB_00500f66;
        pCVar5 = (CIPhysicsFacesSegmentBase *)CDiskFileWin32::CDiskFileWin32(this_01,local_44,1,2);
      }
      else {
        this_00 = ::operator_new(0x10);
        if (this_00 == (CWADFile *)0x0) {
LAB_00500f66:
          pCVar5 = (CIPhysicsFacesSegmentBase *)0x0;
        }
        else {
          pCVar5 = (CIPhysicsFacesSegmentBase *)
                   CWADFile::CWADFile(this_00,*(CWADBank **)(this + 0xa0));
        }
      }
      CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
                ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_30,pCVar5);
      local_18 = 0;
      local_14 = 0;
      local_10 = 0;
      local_c = 0;
      local_8 = 0;
      local_4 = 0;
      std::
      vector<CLevelLoader::CObject::CLevelFiles,std::allocator<CLevelLoader::CObject::CLevelFiles>_>
      ::push_back((vector<CLevelLoader::CObject::CLevelFiles,std::allocator<CLevelLoader::CObject::CLevelFiles>_>
                   *)&local_28,local_1c);
      CLevelLoader::CObject::CLevelFiles::~CLevelFiles(local_1c);
      pCVar8 = local_24;
      piVar3 = local_2c;
      *(undefined4 *)(local_24 + -0x1c) = uVar1;
      piVar2 = *(int **)(local_24 + -4);
      pCVar7 = local_24 + -8;
      if (piVar2 == local_2c) {
LAB_00500fe8:
        if ((piVar3 != (int *)0x0) && (*piVar3 = *piVar3 + -1, *piVar3 == 0)) {
          (*(code *)piVar3[1])();
          operator_delete(piVar3);
        }
      }
      else {
        if (piVar2 != (int *)0x0) {
          *piVar2 = *piVar2 + -1;
          if (**(int **)(local_24 + -4) == 0) {
            (*(code *)(*(int **)(local_24 + -4))[1])();
            operator_delete(*(void **)(pCVar8 + -4));
          }
        }
        *(undefined4 *)pCVar7 = local_30;
        *(int **)(pCVar8 + -4) = piVar3;
        if (piVar3 != (int *)0x0) {
          *piVar3 = *piVar3 + 1;
          goto LAB_00500fe8;
        }
      }
      local_30 = 0;
      local_2c = (int *)0x0;
      CCharString::~CCharString((CCharString *)local_44);
      local_40 = local_40 + 1;
    } while (local_40 != *(undefined4 **)(this + 0x94));
  }
  pCStack_38 = ::operator_new(0x20);
  if (pCStack_38 == (CSpellContainerList *)0x0) {
    pCStack_38 = (CSpellContainerList *)0x0;
  }
  else {
    *(int *)pCStack_38 = 0;
    puVar6 = malloc(0x14);
    *(undefined1 **)pCStack_38 = puVar6;
    *(int *)(pCStack_38 + 4) = 0;
    *puVar6 = 0;
    *(undefined4 *)(*(int *)pCStack_38 + 4) = 0;
    *(int *)(*(int *)pCStack_38 + 8) = *(int *)pCStack_38;
    *(int *)(*(int *)pCStack_38 + 0xc) = *(int *)pCStack_38;
    *(int *)(pCStack_38 + 0xc) = 0;
    *(int *)(pCStack_38 + 0x10) = 0;
    *(int *)(pCStack_38 + 0x14) = 0;
    *(int *)(pCStack_38 + 0x18) = 0;
  }
  if ((pCStack_38 == (CSpellContainerList *)0x0) ||
     (piStack_34 = ::operator_new(0xc), piStack_34 == (int *)0x0)) {
    piStack_34 = (int *)0x0;
  }
  else {
    *piStack_34 = 1;
    piStack_34[1] = (int)CTCMapwho::OnDie;
    piStack_34[2] = (int)pCStack_38;
  }
  NUISystem::CSpellContainerList::AddChild
            (pCStack_38,(CCountedPointer<NUISystem::CComponent> *)&local_28);
  FUN_006c2120(&pCStack_38);
  if (param_2 == 0) {
    while (bVar4 = CTCUserActionPointBase::IsActionPointFree
                             (*(CTCUserActionPointBase **)(this + 0xbc)), bVar4) {
      (**(code **)(**(int **)(this + 0xbc) + 4))();
    }
  }
  if ((piStack_34 != (int *)0x0) && (*piStack_34 = *piStack_34 + -1, *piStack_34 == 0)) {
    (*(code *)piStack_34[1])();
    operator_delete(piStack_34);
  }
  pCVar7 = local_24;
  pCStack_38 = (CSpellContainerList *)0x0;
  piStack_34 = (int *)0x0;
  for (pCVar8 = local_28; pCVar8 != pCVar7; pCVar8 = pCVar8 + 0x1c) {
    CLevelLoader::CObject::CLevelFiles::~CLevelFiles(pCVar8);
  }
  if (local_28 != (CLevelFiles *)0x0) {
    free(local_28);
  }
  return;
}

 
==================== UpdateNavMaps @ 00501990 ==================== 

/* [bsim sim=0.7690107425067539 <- ego_r]
   public: void __thiscall CWorldMap::UpdateNavMaps(class std::set<class CCharString,struct
   std::less<class CCharString>,class std::allocator<class CCharString> > const &,enum
   CWorldMap::ELoadType) */

void __thiscall
CWorldMap::UpdateNavMaps
          (CWorldMap *this,
          set<CCharString,std::less<CCharString>,std::allocator<CCharString>_> *param_1,
          ELoadType param_2)

{
  undefined4 uVar1;
  CWADBank *pCVar2;
  int *piVar3;
  bool bVar4;
  int *piVar5;
  bool bVar6;
  CIPhysicsFacesSegmentBase *pCVar7;
  CDiskFileWin32 *this_00;
  CWideString *pCVar8;
  undefined1 *puVar9;
  CCharString *this_01;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  uint extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  uint extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  uint extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBP;
  undefined4 *puVar10;
  CLevelFiles *pCVar11;
  CLevelFiles *pCVar12;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  FileAccessType FVar13;
  ulong uVar14;
  CCharString local_48 [4];
  CWADFile *local_44;
  CCharString local_40 [4];
  CCharString local_3c [4];
  CSpellContainerList *pCStack_38;
  int *piStack_34;
  undefined4 local_30;
  int *local_2c;
  CLevelFiles *local_28;
  CLevelFiles *local_24;
  undefined4 local_20;
  CLevelFiles local_1c [4];
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  puVar10 = *(undefined4 **)(this + 0x90);
  bVar4 = false;
  bVar6 = false;
  local_28 = (CLevelFiles *)0x0;
  local_24 = (CLevelFiles *)0x0;
  local_20 = 0;
  if (puVar10 != *(undefined4 **)(this + 0x94)) {
    do {
      CCharString::CCharString(local_48,"CWorldMap::UpdateNavMaps",-1);
      NProgressDisplay::DisplayProgress(local_48,(float)(extraout_EDX & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_48,
                 extraout_EDX_00,unaff_EBP);
      uVar1 = *puVar10;
      local_30 = 0;
      local_2c = (int *)0x0;
      if (this[0xa8] == (CWorldMap)0x0) {
        this_00 = ::operator_new(0x1c);
        if (this_00 == (CDiskFileWin32 *)0x0) {
          pCVar7 = (CIPhysicsFacesSegmentBase *)0x0;
        }
        else {
          bVar4 = true;
          uVar14 = 2;
          FVar13 = 1;
          pCVar8 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
          pCVar7 = (CIPhysicsFacesSegmentBase *)
                   CDiskFileWin32::CDiskFileWin32(this_00,pCVar8,FVar13,uVar14);
        }
        CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
                  ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_30,pCVar7);
        if (bVar4) {
          bVar4 = false;
          this_01 = local_3c;
          goto LAB_00501acf;
        }
      }
      else {
        local_44 = ::operator_new(0x10);
        if (local_44 == (CWADFile *)0x0) {
          pCVar7 = (CIPhysicsFacesSegmentBase *)0x0;
        }
        else {
          bVar6 = true;
          pCVar2 = *(CWADBank **)(this + 0xa0);
          CPersistContext::GetDefaultVal<CAnimationSet>();
          pCVar7 = (CIPhysicsFacesSegmentBase *)CWADFile::CWADFile(local_44,pCVar2);
        }
        CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
                  ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_30,pCVar7);
        if (bVar6) {
          bVar6 = false;
          this_01 = local_40;
LAB_00501acf:
          CCharString::~CCharString(this_01);
        }
      }
      local_18 = 0;
      local_14 = 0;
      local_10 = 0;
      local_c = 0;
      local_8 = 0;
      local_4 = 0;
      std::
      vector<CLevelLoader::CObject::CLevelFiles,std::allocator<CLevelLoader::CObject::CLevelFiles>_>
      ::push_back((vector<CLevelLoader::CObject::CLevelFiles,std::allocator<CLevelLoader::CObject::CLevelFiles>_>
                   *)&local_28,local_1c);
      CLevelLoader::CObject::CLevelFiles::~CLevelFiles(local_1c);
      pCVar12 = local_24;
      piVar5 = local_2c;
      *(undefined4 *)(local_24 + -0x1c) = uVar1;
      piVar3 = *(int **)(local_24 + -0xc);
      pCVar11 = local_24 + -0x10;
      if (piVar3 == local_2c) {
LAB_00501b4f:
        if ((piVar5 != (int *)0x0) && (*piVar5 = *piVar5 + -1, *piVar5 == 0)) {
          (*(code *)piVar5[1])();
          operator_delete(piVar5);
        }
      }
      else {
        if (piVar3 != (int *)0x0) {
          *piVar3 = *piVar3 + -1;
          if (**(int **)(local_24 + -0xc) == 0) {
            (*(code *)(*(int **)(local_24 + -0xc))[1])();
            operator_delete(*(void **)(pCVar12 + -0xc));
          }
        }
        *(undefined4 *)pCVar11 = local_30;
        *(int **)(pCVar12 + -0xc) = piVar5;
        if (piVar5 != (int *)0x0) {
          *piVar5 = *piVar5 + 1;
          goto LAB_00501b4f;
        }
      }
      puVar10 = puVar10 + 1;
      local_30 = 0;
      local_2c = (int *)0x0;
    } while (puVar10 != *(undefined4 **)(this + 0x94));
  }
  pCStack_38 = ::operator_new(0x20);
  if (pCStack_38 == (CSpellContainerList *)0x0) {
    pCStack_38 = (CSpellContainerList *)0x0;
  }
  else {
    *(int *)pCStack_38 = 0;
    puVar9 = malloc(0x14);
    *(undefined1 **)pCStack_38 = puVar9;
    *(int *)(pCStack_38 + 4) = 0;
    *puVar9 = 0;
    *(undefined4 *)(*(int *)pCStack_38 + 4) = 0;
    *(int *)(*(int *)pCStack_38 + 8) = *(int *)pCStack_38;
    *(int *)(*(int *)pCStack_38 + 0xc) = *(int *)pCStack_38;
    *(int *)(pCStack_38 + 0xc) = 0;
    *(int *)(pCStack_38 + 0x10) = 0;
    *(int *)(pCStack_38 + 0x14) = 0;
    *(int *)(pCStack_38 + 0x18) = 0;
  }
  if ((pCStack_38 == (CSpellContainerList *)0x0) ||
     (piStack_34 = ::operator_new(0xc), piStack_34 == (int *)0x0)) {
    piStack_34 = (int *)0x0;
  }
  else {
    *piStack_34 = 1;
    piStack_34[1] = (int)CTCMapwho::OnDie;
    piStack_34[2] = (int)pCStack_38;
  }
  NUISystem::CSpellContainerList::AddChild
            (pCStack_38,(CCountedPointer<NUISystem::CComponent> *)&local_28);
  CCharString::CCharString((CCharString *)&param_1,"CWorldMap::UpdateNavMaps - SetAsLoading",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&param_1,(float)(extraout_EDX_01 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_02,unaff_EDI);
  FUN_006c2120(&pCStack_38);
  CCharString::CCharString((CCharString *)&param_1,"CWorldMap::UpdateNavMaps - Add",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&param_1,(float)(extraout_EDX_03 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_04,unaff_EDI);
  if (param_2 == 0) {
    while (bVar6 = CTCUserActionPointBase::IsActionPointFree
                             (*(CTCUserActionPointBase **)(this + 0xbc)), bVar6) {
      CCharString::CCharString
                ((CCharString *)&param_1,"CWorldMap::UpdateNavMaps - WAIT_FOR_LOAD",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&param_1,(float)(extraout_EDX_05 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_06,unaff_EDI);
      (**(code **)(**(int **)(this + 0xbc) + 4))();
    }
  }
  if ((piStack_34 != (int *)0x0) && (*piStack_34 = *piStack_34 + -1, *piStack_34 == 0)) {
    (*(code *)piStack_34[1])();
    operator_delete(piStack_34);
  }
  pCVar11 = local_24;
  pCStack_38 = (CSpellContainerList *)0x0;
  piStack_34 = (int *)0x0;
  for (pCVar12 = local_28; pCVar12 != pCVar11; pCVar12 = pCVar12 + 0x1c) {
    CLevelLoader::CObject::CLevelFiles::~CLevelFiles(pCVar12);
  }
  if (local_28 != (CLevelFiles *)0x0) {
    free(local_28);
  }
  return;
}

 
==================== CreateNavMap @ 00500230 ==================== 

/* [bsim sim=0.8930258777530142 <- ego_r]
   public: void __thiscall CWorldMap::CreateNavMap(long,class std::set<class CCharString,struct
   std::less<class CCharString>,class std::allocator<class CCharString> > const &,class CAFile
   &,class CASuspendableProcess &) */

void __thiscall
CWorldMap::CreateNavMap
          (CWorldMap *this,long param_1,
          set<CCharString,std::less<CCharString>,std::allocator<CCharString>_> *param_2,
          CAFile *param_3,CASuspendableProcess *param_4)

{
  CAFile *this_00;
  CAFile *pCVar1;
  bool bVar2;
  int iVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  int unaff_EBX;
  int unaff_retaddr;
  CAFile *pCVar5;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar6;
  CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader> aCStack_44 [17];
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_33;
  undefined4 uStack_2f;
  undefined3 uStack_2b;
  void *pvStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined1 uStack_10;
  CCharString aCStack_c [8];
  int iStack_4;
  
  this_00 = param_3;
  (**(code **)(*(int *)param_3 + 0x14))(0);
  pCVar1 = param_3;
  CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader>::ReadFromFile
            (aCStack_44,this_00,(CASuspendableProcess *)param_3);
  param_2 = *(set<CCharString,std::less<CCharString>,std::allocator<CCharString>_> **)
             (*(int *)(this + 8) + 0x48);
  if (uStack_2f != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    ppVar6 = uStack_2f;
    (**(code **)(*(int *)this_00 + 0x14))();
    CAFile::ReadVar<int>(this_00,(int *)&param_2,(CASuspendableProcess *)pCVar1);
    NHeroInformationScreens::CBase::CBase((CBase *)((int)&uStack_2f + 3));
    uStack_2f = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                CONCAT13(0xfc,(undefined3)uStack_2f);
    uStack_2b = 0x122e0;
    pvStack_1c = (void *)0x0;
    uStack_18 = 0;
    uStack_14 = 0;
    uStack_10 = 0;
    CCharString::CCharString(aCStack_c);
    pCVar5 = pCVar1;
    iVar3 = (**(code **)(*(int *)this_00 + 0x1c))();
    CMemoryFile::OpenFromFile
              ((CMemoryFile *)((int)&uStack_2f + 3),this_00,(int)param_2 - iVar3,
               (CASuspendableProcess *)pCVar5);
    FUN_004fe630(&stack0xffffffb0,(int)&uStack_2f + 3,unaff_retaddr);
    ppVar4 = extraout_EDX;
    if (0 < unaff_EBX) {
      bVar2 = CNavigatorManager::IsNavigationMapLoadedWithSectionName
                        ((CNavigatorManager *)param_1,iStack_4,(CCharString *)&stack0xffffffb0);
      ppVar4 = extraout_EDX_00;
      if (!bVar2) {
        bVar2 = CNavigatorManager::IsNavigationMapLoaded((CNavigatorManager *)param_1,iStack_4);
        if (bVar2) {
          CNavigatorManager::RemoveNavigationMap((CNavigatorManager *)param_1,iStack_4);
        }
        (**(code **)(*(int *)this_00 + 0x14))(unaff_EBX);
        CNavigatorManager::AddNavigationMap
                  ((CNavigatorManager *)param_1,iStack_4,
                   (C2DBoxI *)(*(int *)(this + 0x20) + iStack_4 * 0x48),
                   (CCharString *)&stack0xffffffb0,this_00,(CASuspendableProcess *)pCVar1);
        ppVar4 = extraout_EDX_01;
      }
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffb0,
               ppVar4,ppVar6);
    uStack_2f = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                CONCAT13(0xfc,(undefined3)uStack_2f);
    uStack_2b = 0x122e0;
    CTCGatherPointBuilding::_Close_CMemoryFile__UAEXXZ
              ((CTCGatherPointBuilding *)((int)&uStack_2f + 3));
    CCharString::~CCharString(aCStack_c);
    if (pvStack_1c != (void *)0x0) {
      free(pvStack_1c);
    }
    NHeroInformationScreens::CBase::CBase((CBase *)((int)&uStack_2f + 3));
    return;
  }
  if (ppStack_33 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    bVar2 = CNavigatorManager::IsNavigationMapLoaded((CNavigatorManager *)param_2,unaff_retaddr);
    if (!bVar2) {
      (**(code **)(*(int *)this_00 + 0x14))();
      CCharString::CCharString((CCharString *)&iStack_4,"",-1);
      CNavigatorManager::AddNavigationMap
                ((CNavigatorManager *)param_1,unaff_retaddr,
                 (C2DBoxI *)(*(int *)(this + 0x20) + unaff_retaddr * 0x48),(CCharString *)&iStack_4,
                 this_00,(CASuspendableProcess *)pCVar1);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&iStack_4,
                 extraout_EDX_02,ppStack_33);
    }
  }
  return;
}

 
==================== SetRegionAsLoaded @ 004fc8a0 ==================== 

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CWorldMap::SetRegionAsLoaded(long,class CCharString const &) */

void __thiscall CWorldMap::SetRegionAsLoaded(CWorldMap *this,long param_1,CCharString *param_2)

{
  CMiniMapDisplay *this_00;
  CRegion *pCVar1;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  uint extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  *(long *)(this + 0x9c) = param_1;
  CCharString::CCharString((CCharString *)&param_1,"SetRegionAsLoaded: Initialise MiniMap",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&param_1,(float)(extraout_EDX & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_00,unaff_ESI);
  pCVar1 = (CRegion *)(*(int *)(this + 0x9c) * 0x58 + *(int *)(this + 0x2c));
  this_00 = (CMiniMapDisplay *)CVertexBufferWin32::DoSizeof(DAT_013b8790);
  CMiniMapDisplay::InitialiseOnRegionLoad(this_00,pCVar1);
  CCharString::CCharString((CCharString *)&param_1,"SetRegionAsLoaded: Initialise MiniMap End",-1);
  NProgressDisplay::DisplayProgress
            ((CCharString *)&param_1,(float)(extraout_EDX_01 & 0xffffff00),false,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_02,unaff_ESI);
  return;
}

 
==================== UnloadRegion @ 004feec0 ==================== 

/* [bsim sim=0.8004936896074871 <- ego_r]
   protected: void __thiscall CWorldMap::UnloadRegion(long,bool) */

void __thiscall CWorldMap::UnloadRegion(CWorldMap *this,long param_1,bool param_2)

{
  void *_Src;
  vector<long,std::allocator<long>_> *pvVar1;
  undefined4 *puVar2;
  int *piVar3;
  long lVar4;
  char cVar5;
  bool bVar6;
  CThingManager *this_00;
  CGameJoystickManager *this_01;
  void *_Dst;
  int iVar7;
  uint uVar8;
  int iVar9;
  undefined3 in_stack_00000009;
  vector<long,std::allocator<long>_> *pvVar10;
  undefined4 uVar11;
  int iStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  pvVar1 = (vector<long,std::allocator<long>_> *)(param_1 * 0x58 + *(int *)(this + 0x2c));
  this[0xd4] = (CWorldMap)0x1;
  CQuestManager::OnRegionLoaded(DAT_013b89fc,(CCharString *)(pvVar1 + 0x18));
  pvVar10 = pvVar1;
  uVar11 = _param_2;
  this_00 = (CThingManager *)(**(code **)(**(int **)(this + 8) + 0xc))();
  CThingManager::OnLevelsUnload(this_00,pvVar10,SUB41(uVar11,0));
  iVar7 = *(int *)pvVar1;
  _param_2 = 0;
  if (*(int *)(pvVar1 + 4) - iVar7 >> 2 != 0) {
    do {
      uVar8 = _param_2;
      iVar7 = *(int *)(iVar7 + _param_2 * 4);
      iStack_10 = iVar7;
      cVar5 = (**(code **)(*(int *)this + 0x28))(iVar7);
      if (cVar5 != '\0') {
        iVar9 = iVar7 * 0x48;
        *(undefined1 *)(*(int *)(this + 0x20) + 0x26 + iVar9) = 0;
        this_01 = CGameJoystickManager::Get();
        CEngineManager::RemoveMap((CEngineManager *)this_01,iVar7);
        _Dst = (void *)FUN_0050e540(&iStack_10,&param_1);
        _Src = (void *)((int)_Dst + 4);
        if (*(void **)(this + 0x94) != _Src) {
          memmove(_Dst,_Src,(int)*(void **)(this + 0x94) - (int)_Src);
        }
        *(int *)(this + 0x94) = *(int *)(this + 0x94) + -4;
        puVar2 = (undefined4 *)(*(int *)(this + 0x20) + 0x10 + iVar9);
        piVar3 = (int *)puVar2[1];
        if (piVar3 != (int *)0x0) {
          *piVar3 = *piVar3 + -1;
          if (*(int *)puVar2[1] == 0) {
            (*(code *)((int *)puVar2[1])[1])();
            operator_delete((void *)puVar2[1]);
          }
        }
        puVar2[1] = 0;
        *puVar2 = 0;
        uVar8 = _param_2;
      }
      iVar7 = *(int *)pvVar1;
      _param_2 = uVar8 + 1;
    } while (_param_2 < (uint)(*(int *)(pvVar1 + 4) - iVar7 >> 2));
  }
  iVar7 = *(int *)pvVar1;
  uVar8 = 0;
  if (*(int *)(pvVar1 + 4) - iVar7 >> 2 != 0) {
    do {
      lVar4 = *(long *)(iVar7 + uVar8 * 4);
      NThingMap::CThingMapManager::RemoveGameMap(*(CThingMapManager **)(this + 0x10),lVar4);
      bVar6 = CNavigatorManager::IsNavigationMapLoaded
                        (*(CNavigatorManager **)(*(int *)(this + 8) + 0x48),lVar4);
      if (bVar6) {
        CNavigatorManager::RemoveNavigationMap
                  (*(CNavigatorManager **)(*(int *)(this + 8) + 0x48),lVar4);
      }
      iVar7 = *(int *)pvVar1;
      uVar8 = uVar8 + 1;
    } while (uVar8 < (uint)(*(int *)(pvVar1 + 4) - iVar7 >> 2));
  }
  *(undefined4 *)(this + 0x9c) = 0;
  uStack_c = 0;
  uStack_8 = 0;
  uStack_4 = 0;
  CMessageEventManager::AddMessage
            (*(CMessageEventManager **)(*(int *)(this + 8) + 0x60),0x27,0x32,(C3DVector *)&uStack_c,
             (CThing *)0x0,(CThing *)0x0,0.0,0.0,param_1);
  this[0xd4] = (CWorldMap)0x0;
  return;
}

 
==================== LoadRegionGraph @ 00506d40 ==================== 

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CWorldMap::LoadRegionGraph(class CWideString const &) */

void __thiscall CWorldMap::LoadRegionGraph(CWorldMap *this,CWideString *param_1)

{
  bool bVar1;
  CIPhysicsFacesSegmentBase *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CWideString *pCVar3;
  CWideString local_20 [4];
  undefined **local_1c [6];
  CCharString local_4 [4];
  
  pCVar3 = param_1;
  CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&param_1);
  FUN_0099be70(pCVar3);
  CCharString::~CCharString((CCharString *)&param_1);
  bVar1 = CAFile::PathExists(local_20);
  if (bVar1) {
    CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_1c,local_20,1,2);
    if (*(int *)(this + 0x44) == 0) {
      pCVar2 = ::operator_new(0x10);
      if (pCVar2 == (CIPhysicsFacesSegmentBase *)0x0) {
        pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
      }
      else {
        *(undefined4 *)pCVar2 = 0;
        *(undefined4 *)(pCVar2 + 4) = 0;
        *(undefined4 *)(pCVar2 + 8) = 0;
        pCVar2[0xd] = (CIPhysicsFacesSegmentBase)0x0;
      }
      CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
                ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(this + 0x44),pCVar2);
    }
    CCharString::CCharString((CCharString *)&param_1);
    CCharString::LoadFromFile
              ((CCharString *)&param_1,(CAFile *)local_1c,(CASuspendableProcess *)0x0,-1);
    FUN_00828710(&param_1,this);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX,unaff_ESI);
    local_1c[0] = &PTR__vector_deleting_destructor__0122d06c;
    CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_1c);
    CCharString::~CCharString(local_4);
    NHeroInformationScreens::CBase::CBase((CBase *)local_1c);
  }
  CCharString::~CCharString((CCharString *)local_20);
  return;
}

 
==================== LoadBinary @ 004fb4f0 ==================== 

/* WARNING: Removing unreachable block (ram,0x004fb869) */
/* [bsim sim=0.9999999999999999 <- ego_r]
   public: void __thiscall CMapInfo::LoadBinary(class CDataInputStream &) */

void __thiscall CMapInfo::LoadBinary(CMapInfo *this,CDataInputStream *param_1)

{
  undefined4 uVar1;
  CDataInputStream *this_00;
  CCharString *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined4 local_8 [2];
  
  this_00 = param_1;
  pCVar2 = (CCharString *)CDataInputStream::ReadPresizedString(param_1);
  CCharString::operator=((CCharString *)(this + 0x18),pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,extraout_EDX,
             unaff_EDI);
  pCVar2 = (CCharString *)CDataInputStream::ReadPresizedString(this_00);
  CCharString::operator=((CCharString *)(this + 0x1c),pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_00,unaff_EDI);
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[0x24] = (CMapInfo)((char)param_1 != '\0');
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[0x28] = (CMapInfo)((char)param_1 != '\0');
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[0x40] = (CMapInfo)((char)param_1 != '\0');
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)this = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)(this + 8) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)(this + 4) = param_1;
  if (*(int *)(this_00 + 4) + 4U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,4);
    }
    else {
      param_1 = (CDataInputStream *)**(undefined4 **)(this_00 + 0xc);
      *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -4;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 4U;
    }
  }
  *(CDataInputStream **)(this + 0xc) = param_1;
  if (*(int *)(this_00 + 4) + 1U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 1) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)&param_1,1);
    }
    else {
      param_1 = (CDataInputStream *)CONCAT31(param_1._1_3_,**(undefined1 **)(this_00 + 0xc));
      *(undefined1 **)(this_00 + 0xc) = *(undefined1 **)(this_00 + 0xc) + 1;
      *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -1;
      *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 1U;
    }
  }
  this[0x25] = (CMapInfo)((char)param_1 != '\0');
  if (*(int *)(this_00 + 4) + 8U < 0x80000000) {
    if (*(int *)(this_00 + 0x14) < 8) {
      CDataInputStream::ReadWithSrcChunkOverflow(this_00,(uchar *)local_8,8);
      *(undefined4 *)(this + 0x20) = local_8[0];
      return;
    }
    uVar1 = **(undefined4 **)(this_00 + 0xc);
    *(int *)(this_00 + 0x14) = *(int *)(this_00 + 0x14) + -8;
    *(undefined4 **)(this_00 + 0xc) = *(undefined4 **)(this_00 + 0xc) + 2;
    *(uint *)(this_00 + 4) = *(int *)(this_00 + 4) + 8U;
    *(undefined4 *)(this + 0x20) = uVar1;
    return;
  }
  *(undefined4 *)(this + 0x20) = local_8[0];
  return;
}

 
==================== GetMapNumberFromScriptName @ 004fb880 ==================== 

/* [bsim sim=1.0 <- ego_r]
   public: long __thiscall CWorldMap::GetMapNumberFromScriptName(class CCharString const &)const  */

long __thiscall CWorldMap::GetMapNumberFromScriptName(CWorldMap *this,CCharString *param_1)

{
  undefined4 *puVar1;
  char *_Str2;
  int iVar2;
  char *_Str1;
  int iVar3;
  uint uVar4;
  
  uVar4 = 1;
  if (1 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    iVar3 = 0x48;
    do {
      if (*(undefined4 **)param_1 == (undefined4 *)0x0) {
        _Str2 = "";
      }
      else {
        _Str2 = (char *)**(undefined4 **)param_1;
      }
      puVar1 = *(undefined4 **)(iVar3 + 0x1c + *(int *)(this + 0x20));
      if (puVar1 == (undefined4 *)0x0) {
        _Str1 = "";
      }
      else {
        _Str1 = (char *)*puVar1;
      }
      iVar2 = _stricmp(_Str1,_Str2);
      if (iVar2 == 0) {
        return uVar4;
      }
      uVar4 = uVar4 + 1;
      iVar3 = iVar3 + 0x48;
    } while (uVar4 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48));
  }
  return 0;
}

 
==================== LoadLoadableGlobalThingsForLevel @ 004fbf60 ==================== 

/* [bsim sim=0.9161056999102758 <- ego_r]
   protected: void __thiscall CWorldMap::LoadLoadableGlobalThingsForLevel(long) */

void __thiscall CWorldMap::LoadLoadableGlobalThingsForLevel(CWorldMap *this,long param_1)

{
  int *piVar1;
  CWADFile *this_00;
  CIPhysicsFacesSegmentBase *pCVar2;
  CDiskFileWin32 *this_01;
  undefined4 uVar3;
  int *piVar4;
  long lVar5;
  undefined4 local_8;
  int *local_4;
  
  lVar5 = param_1;
  CPersistContext::GetDefaultVal<CAnimationSet>();
  GetThingFilenameFromLevelFilename((CWideString *)&param_1);
  CCharString::~CCharString((CCharString *)&local_8);
  local_8 = 0;
  local_4 = (int *)0x0;
  if (this[0xa8] == (CWorldMap)0x0) {
    this_01 = ::operator_new(0x1c);
    if (this_01 != (CDiskFileWin32 *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)
               CDiskFileWin32::CDiskFileWin32(this_01,(CWideString *)&param_1,1,2);
      goto LAB_004fbff6;
    }
  }
  else {
    this_00 = ::operator_new(0x10);
    if (this_00 != (CWADFile *)0x0) {
      pCVar2 = (CIPhysicsFacesSegmentBase *)CWADFile::CWADFile(this_00,*(CWADBank **)(this + 0xa0));
      goto LAB_004fbff6;
    }
  }
  pCVar2 = (CIPhysicsFacesSegmentBase *)0x0;
LAB_004fbff6:
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset
            ((CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)&local_8,pCVar2);
  piVar1 = local_4;
  if (local_4 != (int *)0x0) {
    *local_4 = *local_4 + 1;
  }
  uVar3 = local_8;
  piVar4 = local_4;
  (**(code **)(**(int **)(this + 8) + 0xc))(local_8,local_4,lVar5);
  FUN_005223f0(uVar3,piVar4,lVar5);
  if ((piVar1 != (int *)0x0) && (*piVar1 = *piVar1 + -1, *piVar1 == 0)) {
    (*(code *)piVar1[1])();
    operator_delete(piVar1);
  }
  CCharString::~CCharString((CCharString *)&param_1);
  return;
}

 
==================== LoadLoadableGlobalThings @ 004fdbc0 ==================== 

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [bsim sim=0.9179749984544325 <- ego_r]
   protected: void __thiscall CWorldMap::LoadLoadableGlobalThings(void) */

void __thiscall CWorldMap::LoadLoadableGlobalThings(CWorldMap *this)

{
  float fVar1;
  float fVar2;
  uint uVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int iVar4;
  CCharString local_8 [4];
  uint local_4;
  
  local_4 = 1;
  if (1 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    iVar4 = 0x48;
    do {
      uVar3 = local_4;
      CCharString::CCharString(local_8,"Loading global things",-1);
      fVar2 = (float)(int)local_4;
      local_4 = (*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48 + 1;
      fVar1 = (float)(int)local_4;
      if ((int)local_4 < 0) {
        fVar1 = fVar1 + _DAT_0122dcb4;
      }
      NProgressDisplay::DisplayProgress
                (local_8,(float)(local_4 & 0xffffff00),SUB41(fVar2 / fVar1,0),false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,
                 extraout_EDX,unaff_EDI);
      if ((*(char *)(*(int *)(this + 0x20) + 0x24 + iVar4) != '\0') &&
         (*(char *)(*(int *)(this + 0x20) + iVar4 + 0x28) != '\0')) {
        LoadLoadableGlobalThingsForLevel(this,uVar3);
      }
      local_4 = uVar3 + 1;
      iVar4 = iVar4 + 0x48;
    } while (local_4 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48));
  }
  return;
}

 
==================== LoadAllLoadableGlobalThingsFromSingleFile @ 004fe2a0 ==================== 

/* [bsim sim=0.9146326726544947 <- ego_r]
   protected: void __thiscall CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(class CWideString
   const &) */

void __thiscall
CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(CWorldMap *this,CWideString *param_1)

{
  bool bVar1;
  CCharString *pCVar2;
  CBase *this_00;
  undefined1 *puVar3;
  int *piVar4;
  undefined4 uVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  uint extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int *piVar7;
  long lVar8;
  CCharString local_5c [4];
  CCharString local_58 [4];
  CCharString local_54 [4];
  CCharString aCStack_50 [4];
  undefined4 *local_4c;
  CWorldMap *local_48;
  undefined **local_44 [6];
  CCharString aCStack_2c [4];
  CStringParser local_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane local_14 [20];
  
  local_48 = this;
  CCharString::CCharString(local_5c);
  CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_44,param_1,1,2);
  CCharString::LoadFromFile(local_5c,(CAFile *)local_44,(CASuspendableProcess *)0x0,-1);
  lVar8 = 0;
  pCVar2 = (CCharString *)CCharString::CCharString((CCharString *)&param_1,"",-1);
  CStringParser::CStringParser(local_28,local_5c,(_func_void_CCharString_ptr *)0x0,pCVar2,lVar8);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,extraout_EDX,
             unaff_EDI);
  CCharString::CCharString((CCharString *)&param_1,"NEWMAP",-1);
  bVar1 = CThingCreatureBase::SerialiseGameState
                    ((CThingCreatureBase *)local_28,(CPersistContext *)&param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_00,unaff_EDI);
  if (bVar1) {
    do {
      lVar8 = CStringParser::ReadAsInteger(local_28);
      CCharString::CCharString(local_58,"ENDMAP",-1);
      CStringParser::ReadAsStringUntilString(local_28,(CCharString *)&local_4c);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_58,
                 extraout_EDX_01,unaff_EDI);
      CCharString::CCharString(local_54,"LoadAllLoadableGlobalThingsFromSingleFile",-1);
      NProgressDisplay::DisplayProgress(local_54,(float)(extraout_EDX_02 & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_54,
                 extraout_EDX_03,unaff_EDI);
      this_00 = ::operator_new(0x24);
      if (this_00 == (CBase *)0x0) {
        this_00 = (CBase *)0x0;
      }
      else {
        NHeroInformationScreens::CBase::CBase(this_00);
        *(undefined ***)this_00 = &PTR__vector_deleting_destructor__0122e0fc;
        *(undefined4 *)(this_00 + 0x10) = 0;
        *(undefined4 *)(this_00 + 0x14) = 0;
        *(undefined4 *)(this_00 + 0x18) = 0;
        this_00[0x1c] = (CBase)0x0;
        CCharString::CCharString((CCharString *)(this_00 + 0x20));
      }
      if (local_4c == (undefined4 *)0x0) {
        uVar5 = 0;
        puVar3 = &DAT_0129aaf4;
      }
      else {
        uVar5 = local_4c[1];
        puVar3 = (undefined1 *)*local_4c;
      }
      FUN_009e93c0(puVar3,uVar5);
      if (this_00 == (CBase *)0x0) {
LAB_004fe410:
        piVar4 = (int *)0x0;
      }
      else {
        piVar4 = ::operator_new(0xc);
        if (piVar4 == (int *)0x0) goto LAB_004fe410;
        *piVar4 = 1;
        piVar4[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
        piVar4[2] = (int)this_00;
      }
      if (piVar4 != (int *)0x0) {
        *piVar4 = *piVar4 + 1;
      }
      piVar7 = piVar4;
      (**(code **)(**(int **)(local_48 + 8) + 0xc))(this_00,piVar4,lVar8);
      FUN_005223f0(this_00,piVar7,lVar8);
      CCharString::CCharString(aCStack_50,"ENDMAP",-1);
      CThingCreatureBase::SerialiseGameState
                ((CThingCreatureBase *)local_28,(CPersistContext *)aCStack_50);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_50,
                 extraout_EDX_04,unaff_EDI);
      ppVar6 = extraout_EDX_05;
      if ((piVar4 != (int *)0x0) && (*piVar4 = *piVar4 + -1, *piVar4 == 0)) {
        (*(code *)piVar4[1])();
        operator_delete(piVar4);
        ppVar6 = extraout_EDX_06;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4c,ppVar6,
                 unaff_EDI);
      CCharString::CCharString((CCharString *)&param_1,"NEWMAP",-1);
      bVar1 = CThingCreatureBase::SerialiseGameState
                        ((CThingCreatureBase *)local_28,(CPersistContext *)&param_1);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_07,unaff_EDI);
    } while (bVar1);
  }
  CGuiControlTreePane::SortTreeRecursively
            (local_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_08,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_09,unaff_EDI);
  NHeroInformationScreens::CBase::CBase((CBase *)local_28);
  local_44[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_44);
  CCharString::~CCharString(aCStack_2c);
  NHeroInformationScreens::CBase::CBase((CBase *)local_44);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_5c,
             extraout_EDX_10,unaff_EDI);
  return;
}

 
==================== AddGameMap @ 00638310 ==================== 

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall NThingMap::CThingMapManager::AddGameMap(long) */

void __thiscall NThingMap::CThingMapManager::AddGameMap(CThingMapManager *this,long param_1)

{
  int iVar1;
  
  iVar1 = (**(code **)(**(int **)(this + 4) + 0x30))();
  AddGameMapWithArea(this,param_1,(C2DBoxI *)(*(int *)(iVar1 + 0x20) + param_1 * 0x48));
  return;
}

 
==================== AddGameMapWithArea @ 006381a0 ==================== 

/* [bsim sim=0.7769179584693068 <- ego_r]
   protected: void __thiscall NThingMap::CThingMapManager::AddGameMapWithArea(long,class C2DBoxI
   const &) */

void __thiscall
NThingMap::CThingMapManager::AddGameMapWithArea
          (CThingMapManager *this,long param_1,C2DBoxI *param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  CThingMap *this_00;
  undefined4 uVar3;
  C2DBoxI *pCVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_1c;
  undefined4 *local_18 [2];
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar6 = (int)(*(int *)(param_2 + 4) + (*(int *)(param_2 + 4) >> 0x1f & 0x7fU)) >> 7;
  if (iVar6 <= (int)(*(int *)(param_2 + 0xc) + (*(int *)(param_2 + 0xc) >> 0x1f & 0x7fU)) >> 7) {
    local_1c = iVar6 << 7;
    pCVar4 = param_2;
    do {
      iVar7 = (int)(*(int *)pCVar4 + (*(int *)pCVar4 >> 0x1f & 0x7fU)) >> 7;
      if (iVar7 <= (int)(*(int *)(pCVar4 + 8) + (*(int *)(pCVar4 + 8) >> 0x1f & 0x7fU)) >> 7) {
        iVar5 = iVar7 << 7;
        do {
          puVar1 = (undefined4 *)(*(int *)(this + 8) + (*(int *)(this + 0x30) * iVar6 + iVar7) * 8);
          local_18[0] = puVar1;
          if (puVar1[1] == 0) {
            this_00 = ::operator_new(0x34);
            if (this_00 == (CThingMap *)0x0) {
              uVar3 = 0;
            }
            else {
              local_c = local_1c;
              local_4 = local_1c + 0x80;
              local_8 = iVar5 + 0x80;
              local_10 = iVar5;
              uVar3 = CThingMap::CThingMap(this_00,this,(C2DBoxI *)&local_10);
            }
            *puVar1 = uVar3;
            piVar2 = *(int **)(this + 0x18);
            if (piVar2 == *(int **)(this + 0x1c)) {
              FUN_00639090(piVar2,local_18,&param_2,1,1);
              pCVar4 = param_2;
            }
            else {
              if (piVar2 != (int *)0x0) {
                *piVar2 = (int)puVar1;
              }
              *(int *)(this + 0x18) = *(int *)(this + 0x18) + 4;
              pCVar4 = param_2;
            }
          }
          puVar1[1] = puVar1[1] + 1;
          iVar7 = iVar7 + 1;
          iVar5 = iVar5 + 0x80;
        } while (iVar7 <= (int)(*(int *)(pCVar4 + 8) + (*(int *)(pCVar4 + 8) >> 0x1f & 0x7fU)) >> 7)
        ;
      }
      iVar6 = iVar6 + 1;
      local_1c = local_1c + 0x80;
    } while (iVar6 <= (int)(*(int *)(pCVar4 + 0xc) + (*(int *)(pCVar4 + 0xc) >> 0x1f & 0x7fU)) >> 7)
    ;
  }
  param_2 = (C2DBoxI *)param_1;
  FUN_00512da0(local_18,&param_2);
  return;
}

 
==================== ReadFromFile @ 0051a240 ==================== 

/* [bsim sim=0.7306728835144047 <- ego_r]
   public: void __thiscall CReplaceSerialise<class CFileFormatEditMapBrushHeader,class
   COldFileFormatEditMapBrushHeader>::ReadFromFile(class CAFile &,class CASuspendableProcess *) */

void __thiscall
CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader>::ReadFromFile
          (CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader> *this,
          CAFile *param_1,CASuspendableProcess *param_2)

{
  CAFile *this_00;
  CASuspendableProcess *pCVar1;
  undefined4 uVar2;
  CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader> *pCVar3;
  int iVar4;
  undefined4 unaff_EBP;
  CReplaceSerialise<CFileFormatEditMapBrushHeader,COldFileFormatEditMapBrushHeader> *pCVar5;
  int unaff_retaddr;
  
  pCVar1 = param_2;
  this_00 = param_1;
  CAFile::ReadVar<int>(param_1,(int *)&param_1,param_2);
  uVar2 = (**(code **)(*(int *)this_00 + 0x1c))();
  CAFile::ReadVar<unsigned_char>(this_00,(uchar *)&param_2,pCVar1);
  (**(code **)(*(int *)this_00 + 0x14))(uVar2);
  if ((char)param_1 == '\x04') {
    *(int *)this = unaff_retaddr;
    (**(code **)(*(int *)this_00 + 0xc))(this + 4,unaff_retaddr + -4,pCVar1);
    return;
  }
  pCVar3 = operator_new__(0x19);
  (**(code **)(*(int *)this_00 + 0xc))(pCVar3 + 4,unaff_retaddr + -4,pCVar1);
  *(undefined4 *)pCVar3 = unaff_EBP;
  FUN_00519510();
  pCVar5 = pCVar3;
  for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)this = *(undefined4 *)pCVar5;
    pCVar5 = pCVar5 + 4;
    this = this + 4;
  }
  *this = *pCVar5;
  operator_delete__(pCVar3);
  return;
}

 
==================== PrepareForLoad @ 004382c0 ==================== 

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CWorldMap::PrepareForLoad(void) */

void __thiscall CWorldMap::PrepareForLoad(CWorldMap *this)

{
  CWorldMap *local_4;
  
  local_4 = this;
  std::_For_each<std::pair<long,CCharString>_const*,CAddDefNamesToComboBox>
            (&local_4,*(undefined4 *)(this + 0x2c0),*(undefined4 *)(this + 0x2c4),
             CThingFilter_IsValid::operator());
  return;
}

 
==================== PrepareForLoad @ 004ff560 ==================== 

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CWorldMap::PrepareForLoad(void) */

void __thiscall CWorldMap::PrepareForLoad(CWorldMap *this)

{
  UnloadRegion(this,*(long *)(this + 0x9c),false);
  return;
}

 