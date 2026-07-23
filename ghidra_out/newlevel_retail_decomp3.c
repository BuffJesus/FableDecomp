 '0x507650' '0x501150' '0x4fe030'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== FUN_00507650 @ 00507650 ==================== 

void FUN_00507650(CWideString *param_1)

{
  CWorldMap *pCVar1;
  CEngineManager *pCVar2;
  void *pvVar3;
  uint uVar4;
  undefined4 ****ppppuVar5;
  CThingCreatureBase *this;
  uint uVar6;
  int iVar7;
  CWorldMap *pCVar8;
  vector<CAnimationEntry_const*,std::allocator<CAnimationEntry_const*>_> *unaff_EDI;
  CCharString *pCVar9;
  int iVar10;
  CCharString *local_a8;
  uint local_a4;
  undefined4 ***local_a0;
  undefined1 *local_9c;
  undefined1 *local_98;
  CWorldMap *local_94;
  CMemoryDataInputStream local_90 [4];
  uint local_8c;
  uint *local_84;
  int local_7c;
  undefined **local_74 [3];
  int local_68;
  CCharString aCStack_5c [4];
  CEngineInit local_58 [88];
  
  CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)local_74,param_1,1,2);
  uVar6 = local_68 + 1;
  local_a0 = (undefined4 ***)0x0;
  local_9c = (undefined1 *)0x0;
  local_98 = (undefined1 *)0x0;
  if (uVar6 == 0) {
    local_a0 = (undefined4 ****)0x0;
  }
  else {
    local_a0 = malloc(uVar6);
  }
  local_98 = (undefined1 *)(uVar6 + (int)local_a0);
  ppppuVar5 = (undefined4 ****)local_a0;
  for (uVar4 = uVar6 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *ppppuVar5 = (undefined4 ***)0x0;
    ppppuVar5 = ppppuVar5 + 1;
  }
  for (uVar4 = uVar6 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined1 *)ppppuVar5 = 0;
    ppppuVar5 = (undefined4 ****)((int)ppppuVar5 + 1);
  }
  local_9c = (undefined1 *)(uVar6 + (int)local_a0);
  ppppuVar5 = (undefined4 ****)local_a0;
  if ((int)local_9c - (int)local_a0 == 0) {
    ppppuVar5 = &local_a0;
  }
  CDiskFileWin32::_Read_CDiskFileWin32__UAEXPAXJPAVCASuspendableProcess___Z
            ((CDiskFileWin32 *)local_74,ppppuVar5,(int)local_9c - (int)local_a0,
             (CASuspendableProcess *)0x0);
  ppppuVar5 = (undefined4 ****)local_a0;
  if ((int)local_9c - (int)local_a0 == 0) {
    ppppuVar5 = &local_a0;
  }
  CMemoryDataInputStream::CMemoryDataInputStream(local_90,ppppuVar5,(int)local_9c - (int)local_a0);
  if (local_8c + 4 < 0x80000000) {
    if (local_7c < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)local_90,(uchar *)&local_a8,4);
    }
    else {
      local_a8 = (CCharString *)*local_84;
      local_84 = local_84 + 1;
      local_7c = local_7c + -4;
      local_8c = local_8c + 4;
    }
  }
  pCVar9 = local_a8;
  this = (CThingCreatureBase *)(local_94 + 0x20);
  CThingCreatureBase::GetAnimsWithNameStartingWith(this,local_a8,unaff_EDI);
  if (1 < (int)pCVar9) {
    iVar7 = 0x48;
    pCVar9 = pCVar9 + -1;
    do {
      CMapInfo::LoadBinary((CMapInfo *)(*(int *)this + iVar7),(CDataInputStream *)local_90);
      iVar7 = iVar7 + 0x48;
      pCVar9 = pCVar9 + -1;
    } while (pCVar9 != (CCharString *)0x0);
  }
  uVar6 = local_a4;
  if (local_8c + 4 < 0x80000000) {
    if (local_7c < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)local_90,(uchar *)&local_a4,4);
      uVar6 = local_a4;
    }
    else {
      uVar6 = *local_84;
      local_84 = local_84 + 1;
      local_7c = local_7c + -4;
      local_8c = local_8c + 4;
    }
  }
  pCVar1 = local_94;
  pCVar8 = local_94 + 0x2c;
  local_a4 = CEngineInit::CEngineInit(local_58);
  iVar7 = *(int *)(pCVar1 + 0x30);
  iVar10 = *(int *)pCVar8;
  if (uVar6 < (uint)((iVar7 - iVar10) / 0x58)) {
    FUN_0051cc80(uVar6 * 0x58 + iVar10,iVar7);
  }
  else {
    iVar7 = *(int *)(pCVar1 + 0x30);
    FUN_0051d0a0(iVar7,uVar6 - (iVar7 - iVar10) / 0x58,local_a4);
  }
  CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge
            ((CAIStateGroup_SummonerCharge *)local_58);
  if (1 < (int)uVar6) {
    iVar7 = 0x58;
    iVar10 = uVar6 - 1;
    do {
      CRegion::LoadBinary((CRegion *)(*(int *)pCVar8 + iVar7),(CDataInputStream *)local_90);
      iVar7 = iVar7 + 0x58;
      iVar10 = iVar10 + -1;
    } while (iVar10 != 0);
  }
  if (1 < (int)local_a8) {
    iVar10 = 0x48;
    iVar7 = 1;
    do {
      CWorldMap::SetMapPlacement(local_94,(C2DBoxI *)(*(int *)(local_94 + 0x20) + iVar10),iVar7);
      if (DAT_013ba854 == (CEngineManager *)0x0) {
        pCVar2 = operator_new(0x14);
        if (pCVar2 == (CEngineManager *)0x0) {
          pCVar2 = (CEngineManager *)0x0;
        }
        else {
          *(undefined ***)pCVar2 = &PTR__vector_deleting_destructor__01231584;
          *(undefined4 *)(pCVar2 + 4) = 0;
          pvVar3 = malloc(0xc);
          *(void **)pvVar3 = pvVar3;
          *(void **)((int)pvVar3 + 4) = pvVar3;
          *(void **)(pCVar2 + 4) = pvVar3;
          *(undefined4 *)(pCVar2 + 8) = 0;
          *(undefined4 *)(pCVar2 + 0xc) = 0;
          *(undefined4 *)(pCVar2 + 0x10) = 0;
        }
        if ((DAT_013ba858 != (int *)0x0) && (*DAT_013ba858 = *DAT_013ba858 + -1, *DAT_013ba858 == 0)
           ) {
          (*(code *)DAT_013ba858[1])();
          operator_delete(DAT_013ba858);
        }
        DAT_013ba858 = (int *)0x0;
        DAT_013ba854 = pCVar2;
        if (pCVar2 != (CEngineManager *)0x0) {
          DAT_013ba858 = operator_new(0xc);
          pCVar2 = DAT_013ba854;
          if (DAT_013ba858 == (int *)0x0) {
            DAT_013ba858 = (int *)0x0;
          }
          else {
            *DAT_013ba858 = 1;
            DAT_013ba858[1] = (int)&LAB_004361e0;
            DAT_013ba858[2] = (int)pCVar2;
          }
        }
      }
      CEngineManager::AddMap(DAT_013ba854,iVar7);
      iVar7 = iVar7 + 1;
      iVar10 = iVar10 + 0x48;
    } while (iVar7 < (int)local_a8);
  }
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)local_90);
  if ((undefined4 ****)local_a0 != (undefined4 ****)0x0) {
    free(local_a0);
  }
  local_74[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)local_74);
  CCharString::~CCharString(aCStack_5c);
  NHeroInformationScreens::CBase::CBase((CBase *)local_74);
  return;
}

 
==================== FUN_00501150 @ 00501150 ==================== 

void __thiscall
FUN_00501150(int *param_1,undefined4 param_2,undefined4 param_3,int *param_4,int *param_5,
            int *param_6,int *param_7,int *param_8)

{
  int *piVar1;
  CLevelFiles *pCVar2;
  int *piVar3;
  CLevelFiles *pCVar4;
  bool bVar5;
  int *piVar6;
  undefined1 *puVar7;
  CSpellContainerList *unaff_EBX;
  CLevelFiles *pCVar8;
  CLevelFiles *local_38;
  CLevelFiles *local_34;
  CLevelFiles *local_30;
  CLevelFiles *local_2c;
  int aiStack_28 [3];
  CLevelFiles local_1c [4];
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  piVar6 = operator_new(0x20);
  if (piVar6 == (int *)0x0) {
    piVar6 = (int *)0x0;
  }
  else {
    *piVar6 = 0;
    puVar7 = malloc(0x14);
    *piVar6 = (int)puVar7;
    piVar6[1] = 0;
    *puVar7 = 0;
    *(undefined4 *)(*piVar6 + 4) = 0;
    *(int *)(*piVar6 + 8) = *piVar6;
    *(int *)(*piVar6 + 0xc) = *piVar6;
    piVar6[3] = 0;
    piVar6[4] = 0;
    piVar6[5] = 0;
    piVar6[6] = 0;
  }
  if ((piVar6 == (int *)0x0) || (local_38 = operator_new(0xc), local_38 == (CLevelFiles *)0x0)) {
    local_38 = (CLevelFiles *)0x0;
  }
  else {
    *(undefined4 *)local_38 = 1;
    *(code **)(local_38 + 4) = CTCMapwho::OnDie;
    *(int **)(local_38 + 8) = piVar6;
  }
  local_34 = (CLevelFiles *)0x0;
  local_30 = (CLevelFiles *)0x0;
  local_2c = (CLevelFiles *)0x0;
  local_34 = malloc(0x1c);
  local_2c = local_34 + 0x1c;
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  local_c = 0;
  local_8 = 0;
  local_4 = 0;
  local_30 = local_34;
  std::
  _Uninit_fill_n<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>*,unsigned_int,std::pair<unsigned___int64,class_CCountedPointer<class_CActiveEntityScriptBase>_>,std::allocator<std::pair<unsigned___int64,CCountedPointer<CActiveEntityScriptBase>_>_>,std::pair<unsigned___int64,class_CCountedPointer<class_CActiveEntityScriptBase>_>_>
            (local_34,1,local_1c,&param_2);
  CLevelLoader::CObject::CLevelFiles::~CLevelFiles(local_1c);
  pCVar2 = local_34;
  piVar3 = param_6;
  *(undefined4 *)local_34 = param_2;
  piVar1 = *(int **)(local_34 + 0x10);
  pCVar8 = local_34 + 0xc;
  if (piVar1 != param_6) {
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + -1;
      if (**(int **)(local_34 + 0x10) == 0) {
        (*(code *)(*(int **)(local_34 + 0x10))[1])();
        operator_delete(*(void **)(pCVar2 + 0x10));
      }
    }
    *(int **)pCVar8 = param_5;
    *(int **)(pCVar2 + 0x10) = piVar3;
    if (piVar3 != (int *)0x0) {
      *piVar3 = *piVar3 + 1;
    }
  }
  pCVar2 = local_34;
  piVar3 = param_8;
  piVar1 = *(int **)(local_34 + 0x18);
  pCVar8 = local_34 + 0x14;
  if (piVar1 != param_8) {
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + -1;
      if (**(int **)(local_34 + 0x18) == 0) {
        (*(code *)(*(int **)(local_34 + 0x18))[1])();
        operator_delete(*(void **)(pCVar2 + 0x18));
      }
    }
    *(int **)pCVar8 = param_7;
    *(int **)(pCVar2 + 0x18) = piVar3;
    if (piVar3 != (int *)0x0) {
      *piVar3 = *piVar3 + 1;
    }
  }
  pCVar2 = local_34;
  piVar3 = param_4;
  piVar1 = *(int **)(local_34 + 8);
  pCVar8 = local_34 + 4;
  if (piVar1 != param_4) {
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + -1;
      if (**(int **)(local_34 + 8) == 0) {
        (*(code *)(*(int **)(local_34 + 8))[1])();
        operator_delete(*(void **)(pCVar2 + 8));
      }
    }
    *(undefined4 *)pCVar8 = param_3;
    *(int **)(pCVar2 + 8) = piVar3;
    if (piVar3 != (int *)0x0) {
      *piVar3 = *piVar3 + 1;
    }
  }
  (**(code **)(*param_1 + 0x68))(aiStack_28);
  NUISystem::CSpellContainerList::AddChild
            (unaff_EBX,(CCountedPointer<NUISystem::CComponent> *)&local_38);
  FUN_006c2120(&stack0xffffffc0);
  if (param_8 == (int *)0x0) {
    while (bVar5 = CTCUserActionPointBase::IsActionPointFree
                             ((CTCUserActionPointBase *)param_1[0x2f]), bVar5) {
      (**(code **)(*(int *)param_1[0x2f] + 4))();
    }
  }
  if (aiStack_28[0] != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              ((LTextTreeWalkThrough *)&local_2c,*(LTextBinNode **)(local_2c + 4));
    *(CLevelFiles **)(local_2c + 8) = local_2c;
    *(undefined4 *)(local_2c + 4) = 0;
    *(CLevelFiles **)(local_2c + 0xc) = local_2c;
    aiStack_28[0] = 0;
  }
  pCVar2 = local_38;
  pCVar8 = local_34;
  if (local_2c != (CLevelFiles *)0x0) {
    free(local_2c);
    pCVar2 = local_38;
    pCVar8 = local_34;
  }
  for (; pCVar4 = local_34, pCVar2 != local_34; pCVar2 = pCVar2 + 0x1c) {
    local_34 = pCVar8;
    CLevelLoader::CObject::CLevelFiles::~CLevelFiles(pCVar2);
    pCVar8 = local_34;
    local_34 = pCVar4;
  }
  local_34 = pCVar8;
  if (local_38 != (CLevelFiles *)0x0) {
    free(local_38);
  }
  if ((piVar6 != (int *)0x0) && (*piVar6 = *piVar6 + -1, *piVar6 == 0)) {
    (*(code *)piVar6[1])();
    operator_delete(piVar6);
  }
  if ((piVar3 != (int *)0x0) && (*piVar3 = *piVar3 + -1, *piVar3 == 0)) {
    (*(code *)piVar3[1])();
    operator_delete(piVar3);
  }
  piVar6 = param_5;
  if ((param_5 != (int *)0x0) && (*param_5 = *param_5 + -1, *param_5 == 0)) {
    (*(code *)param_5[1])();
    operator_delete(piVar6);
  }
  piVar6 = param_7;
  if ((param_7 != (int *)0x0) && (*param_7 = *param_7 + -1, *param_7 == 0)) {
    (*(code *)param_7[1])();
    operator_delete(piVar6);
  }
  return;
}

 
==================== CompileLoadableGlobalThingsToSingleFile @ 004fe030 ==================== 

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [bsim sim=0.8133256946165351 <- ego_r]
   protected: void __thiscall CWorldMap::CompileLoadableGlobalThingsToSingleFile(class CWideString
   const &) */

void __thiscall
CWorldMap::CompileLoadableGlobalThingsToSingleFile(CWorldMap *this,CWideString *param_1)

{
  float fVar1;
  float fVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar3;
  void *pvVar4;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar5;
  int iVar6;
  undefined4 uVar7;
  CCharString *pCVar8;
  CThingManager *pCVar9;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBP;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  void *pvVar11;
  undefined *puVar12;
  vector<CCharString,std::allocator<CCharString>_> *pvVar13;
  CCharString local_48 [4];
  int iStack_44;
  CCharString aCStack_40 [4];
  void *pvStack_3c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_38 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_34 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_30 [4];
  undefined4 uStack_2c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_28;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_24;
  undefined4 uStack_20;
  undefined **appuStack_1c [6];
  CCharString aCStack_4 [4];
  
  CCharString::CCharString(local_48);
  iVar6 = (**(code **)(**(int **)(this + 8) + 0xc))();
  uStack_2c = *(undefined4 *)(iVar6 + 0x80);
  iVar6 = (**(code **)(**(int **)(this + 8) + 0xc))();
  *(undefined4 *)(iVar6 + 0x80) = 1;
  pvStack_3c = (void *)0x1;
  if (1 < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48)) {
    iStack_44 = 0x48;
    do {
      pvVar4 = pvStack_3c;
      CCharString::CCharString(aCStack_40,"Compiling global things",-1);
      fVar2 = (float)(int)pvStack_3c;
      pvStack_3c = (void *)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48 + 1);
      fVar1 = (float)(int)pvStack_3c;
      if ((int)pvStack_3c < 0) {
        fVar1 = fVar1 + _DAT_0122dcb4;
      }
      NProgressDisplay::DisplayProgress
                (aCStack_40,(float)((uint)pvStack_3c & 0xffffff00),SUB41(fVar2 / fVar1,0),false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_40,
                 extraout_EDX,unaff_EBP);
      if ((*(char *)(*(int *)(this + 0x20) + 0x24 + iStack_44) != '\0') &&
         (*(char *)(*(int *)(this + 0x20) + iStack_44 + 0x28) != '\0')) {
        pvStack_3c = malloc(0xc);
        *(void **)pvStack_3c = pvStack_3c;
        *(void **)((int)pvStack_3c + 4) = pvStack_3c;
        LoadLoadableGlobalThingsForLevel(this,(long)pvVar4);
        puVar12 = &DAT_012397f8;
        uVar7 = GFIntToCharString_API(&DAT_012397f8);
        CCharString_OperatorPlus_API(uVar7);
        pCVar8 = (CCharString *)FUN_0099f600(puVar12);
        CCharString::operator+=(local_48,pCVar8);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_38,extraout_EDX_00,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_34,extraout_EDX_01,unaff_EBP);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_30,extraout_EDX_02,unaff_EBP);
        pvVar13 = (vector<CCharString,std::allocator<CCharString>_> *)&paStack_28;
        pCVar8 = local_48;
        paStack_28 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        paStack_24 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        uStack_20 = 0;
        pvVar11 = pvVar4;
        pCVar9 = (CThingManager *)(**(code **)(**(int **)(this + 8) + 0xc))();
        CThingManager::SaveToScript(pCVar9,pCVar8,(long)pvVar11,pvVar13);
        CCharString::operator+=(local_48,"\r\nENDMAP\r\n");
        pCVar9 = (CThingManager *)(**(code **)(**(int **)(this + 8) + 0xc))();
        CThingManager::DeleteAllThings(pCVar9);
        paVar5 = paStack_24;
        ppVar3 = extraout_EDX_03;
        for (paVar10 = paStack_28; paVar10 != paVar5; paVar10 = paVar10 + 4) {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (paVar10,ppVar3,unaff_EBP);
          ppVar3 = extraout_EDX_04;
        }
        if (paStack_28 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
          free(paStack_28);
        }
        CGuiControlTreePane::SortTreeRecursively
                  ((CGuiControlTreePane *)&pvStack_3c,
                   (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EBP);
      }
      pvStack_3c = (void *)((int)pvVar4 + 1);
      iStack_44 = iStack_44 + 0x48;
    } while (pvStack_3c < (uint)((*(int *)(this + 0x24) - *(int *)(this + 0x20)) / 0x48));
  }
  iVar6 = (**(code **)(**(int **)(this + 8) + 0xc))();
  *(undefined4 *)(iVar6 + 0x80) = uStack_2c;
  CDiskFileWin32::CDiskFileWin32((CDiskFileWin32 *)appuStack_1c,param_1,4,2);
  CCharString::SaveToFile(local_48,(CAFile *)appuStack_1c);
  appuStack_1c[0] = &PTR__vector_deleting_destructor__0122d06c;
  CDiskFileWin32::_Close_CDiskFileWin32__UAEXXZ((CDiskFileWin32 *)appuStack_1c);
  CCharString::~CCharString(aCStack_4);
  NHeroInformationScreens::CBase::CBase((CBase *)appuStack_1c);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_48,
             extraout_EDX_05,unaff_EDI);
  return;
}

 