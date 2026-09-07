//=== GetRegionTeleporter @ 005fbea0 (seed 0x005fbea0) ===

/* [bsim sim=0.8660894513788454 <- ego_r]
   private: class CThing * __thiscall CTCInventoryMap::GetRegionTeleporter(class CRegion const
   &)const  */

CThing * __thiscall CTCInventoryMap::GetRegionTeleporter(CTCInventoryMap *this,CRegion *param_1)

{
  CThing *pCVar1;
  bool bVar2;
  CDisplayEngine *pCVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  CThing *pCStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  int iStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 *puStack_8;
  undefined4 *puStack_4;
  
  pCVar3 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  uStack_1c = (**(code **)(*(int *)pCVar3 + 0x34))();
  iVar4 = *(int *)param_1;
  uVar6 = 0;
  if (*(int *)(param_1 + 4) - iVar4 >> 2 != 0) {
    puStack_8 = &uStack_18;
    puStack_4 = &uStack_10;
    do {
      uStack_c = *(undefined4 *)(iVar4 + uVar6 * 4);
      pCStack_20 = (CThing *)0x0;
      uStack_10 = uStack_1c;
      uStack_18 = 0xdd;
      pCVar3 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      bVar2 = CThingSearchTools::
              GetPFirstThing<CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InMap>,CThing>
                        (*(CThingSearchTools **)(pCVar3 + 0x20),
                         (CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InMap> *)&puStack_8,
                         &pCStack_20);
      pCVar1 = pCStack_20;
      if (bVar2) {
        if ((*(uint *)(pCStack_20 + 0x38) & 0x20000000) != 0) {
          pCStack_20 = (CThing *)0xdd;
          piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                          ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                        *)(pCVar1 + 0x44),(ETCInterfaceType *)&pCStack_20);
          if ((piVar5 == *(int **)(pCVar1 + 0x48)) || (0xdd < *piVar5)) {
            piVar5 = *(int **)(pCVar1 + 0x48);
          }
          iStack_14 = piVar5[1];
        }
        if ((iStack_14 != 0) && (*(char *)(iStack_14 + 0x10) != '\0')) {
          return pCVar1;
        }
      }
      iVar4 = *(int *)param_1;
      uVar6 = uVar6 + 1;
    } while (uVar6 < (uint)(*(int *)(param_1 + 4) - iVar4 >> 2));
  }
  return (CThing *)0x0;
}


// CALLEES of GetRegionTeleporter:
//   00686a80  GetDisplayEngine
//   00615750  GetPFirstThing<CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InMap>,CThing>
//   004365b0  LowerBound

//=== GetRegionsToDisplay @ 005fd410 (seed 0x005fd410) ===

/* [bsim sim=0.6414051294461935 <- ego_r]
   public: void __thiscall CTCInventoryMap::GetRegionsToDisplay(class std::set<long,struct
   std::less<long>,class std::allocator<long> > &) */

void __thiscall
CTCInventoryMap::GetRegionsToDisplay
          (CTCInventoryMap *this,set<long,std::less<long>,std::allocator<long>_> *param_1)

{
  int *piVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  CDisplayEngine *pCVar5;
  int *_Memory;
  CBaseObject *pCVar6;
  CCharString *pCVar7;
  CWorldMap *this_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  CTCQuestCard *this_01;
  undefined4 *puVar8;
  int *piVar9;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined1 *puVar10;
  int *local_20;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_1c [4];
  CTCInventoryMap *local_18;
  long lStack_14;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_10 [8];
  undefined1 auStack_8 [8];
  
  iVar4 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
  local_18 = this;
  pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  local_20 = (int *)CWorldMap::GetRegionNumberMapIsIn((CWorldMap *)pCVar5,iVar4);
  BinarySearchTree_Insert(local_10,&local_20);
  iVar4 = *(int *)(this + 0x134);
  if (iVar4 != *(int *)(this + 0x138)) {
    do {
      BinarySearchTree_Insert(local_10,iVar4);
      iVar4 = iVar4 + 4;
    } while (iVar4 != *(int *)(this + 0x138));
  }
  puVar8 = (undefined4 *)(DAT_013b89fc + 0x5c);
  _Memory = malloc(0x10);
  *_Memory = (int)_Memory;
  _Memory[1] = (int)_Memory;
  Campfire_InsertRegionNode(_Memory,*(undefined4 *)*puVar8,(undefined4 *)*puVar8,&param_1);
  this_01 = (CTCQuestCard *)param_1;
  for (local_20 = (int *)*_Memory; local_20 != _Memory; local_20 = (int *)*local_20) {
    pCVar6 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(local_20 + 2));
    if ((*(uint *)(pCVar6 + 0x2c) & 0x1000) != 0) {
      piVar1 = *(int **)(pCVar6 + 0x48);
      piVar9 = *(int **)(pCVar6 + 0x44);
      iVar4 = (int)piVar1 - (int)piVar9 >> 3;
      while (iVar2 = iVar4, 0 < iVar2) {
        iVar4 = iVar2 / 2;
        if (piVar9[iVar4 * 2] < 0x6c) {
          piVar9 = piVar9 + iVar4 * 2 + 2;
          iVar4 = iVar2 + (-1 - iVar4);
        }
      }
      if ((piVar9 == piVar1) || (0x6c < *piVar9)) {
        piVar9 = piVar1;
      }
      this_01 = (CTCQuestCard *)piVar9[1];
    }
    puVar10 = &DAT_0122d70e;
    CTCQuestCard::GetRegionName(this_01);
    cVar3 = CCharString__NotEqual(puVar10);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (local_1c,extraout_EDX,unaff_EDI);
    if (cVar3 != '\0') {
      pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)local_18);
      pCVar7 = (CCharString *)CTCQuestCard::GetRegionName(this_01);
      this_00 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
      lStack_14 = CWorldMap::GetRegionNumberFromName(this_00,pCVar7);
      BinarySearchTree_Insert(auStack_8,&lStack_14);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (local_10,extraout_EDX_00,unaff_EDI);
    }
  }
  piVar9 = (int *)*_Memory;
  while (piVar9 != _Memory) {
    piVar1 = (int *)*piVar9;
    (**(code **)piVar9[2])(0);
    free(piVar9);
    piVar9 = piVar1;
  }
  *_Memory = (int)_Memory;
  _Memory[1] = (int)_Memory;
  free(_Memory);
  return;
}


// CALLEES of GetRegionsToDisplay:
//   00686d30  GetDisplayEngine
//   007025a0  GetRegionName
//   00a01b50  GetPItem
//   00686a80  GetDisplayEngine
//   0099e960  CCharString::NotEqual
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00618100  Campfire_InsertRegionNode
//   004fc190  GetRegionNumberMapIsIn
//   00512da0  BinarySearchTree_Insert
//   00bfea0e  malloc
//   00bfea14  free
//   004fc210  GetRegionNumberFromName

//=== ConstructRegionDescription @ 00605570 (seed 0x00605570) ===

/* [bsim sim=0.586921810847056 <- ego_r]
   protected: void __thiscall CTCInventoryMap::ConstructRegionDescription(class
   NUISystem::CComponent *,class CRegion &,bool) */

void __thiscall
CTCInventoryMap::ConstructRegionDescription
          (CTCInventoryMap *this,CComponent *param_1,CRegion *param_2,bool param_3)

{
  CRegion *pCVar1;
  int *piVar2;
  CTCInventoryMap *pCVar3;
  int *piVar4;
  CDisplayEngine *pCVar5;
  CDisplayEngine *pCVar6;
  CWorldMap *pCVar7;
  long lVar8;
  COptimisedPrimitive *this_00;
  CFrontEndManager *this_01;
  CComponent *pCVar9;
  CDefinitionManager *this_02;
  CCharString *pCVar10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar11;
  int iVar12;
  int iVar13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  uint3 unaff_EBP;
  CRegion *pCVar14;
  CCharString *pCVar15;
  CManager *unaff_ESI;
  uint uVar16;
  CComponent **ppCVar17;
  CTCHeroStats *this_03;
  int *piVar18;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int iVar19;
  undefined4 *puVar20;
  CRegion *unaff_retaddr;
  undefined *puVar21;
  undefined4 uVar22;
  bool bVar23;
  undefined4 uVar24;
  CManager *pCStack_7c;
  CUIRegionInfo *pCVar25;
  CUIRegionInfo *pCVar26;
  int **ppiVar27;
  int *piVar28;
  int *piVar29;
  CCharString aCStack_5c [4];
  CComponent *local_58;
  CFrontEndManager *pCStack_54;
  int *piStack_50;
  int *apiStack_4c [2];
  int *local_44;
  long lStack_40;
  int *piStack_3c;
  undefined4 uStack_38;
  CComponent *pCStack_34;
  CRegion *pCStack_30;
  undefined4 uStack_2c;
  CComponent *apCStack_28 [3];
  CRegion *pCStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  int iStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  iVar19 = 0;
  local_58 = (CComponent *)0x0;
  local_44 = (int *)((uint)local_44 & 0xffffff00);
  pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  pCVar6 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  iVar12 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
  pCVar14 = param_2 + 0x18;
  pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar6 + 0x34))();
  lStack_40 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar12);
  pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
  lVar8 = CWorldMap::GetRegionNumberFromName(pCVar7,(CCharString *)pCVar14);
  pCVar3 = this + 0x198;
  uVar16 = 0;
  if (lVar8 == lStack_40) {
    iVar13 = *(int *)(this + 0x19c) - *(int *)(this + 0x198);
    iVar12 = iVar13 >> 0x1f;
    local_44 = (int *)CONCAT31(local_44._1_3_,1);
    if (iVar13 / 0x28 + iVar12 != iVar12) {
      do {
        pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
        lStack_40 = *(long *)pCVar3;
        pCVar14 = param_2 + 0x18;
        pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
        lVar8 = CWorldMap::GetRegionNumberFromName(pCVar7,(CCharString *)pCVar14);
        if (*(int *)(iVar19 + lStack_40) == lVar8) goto LAB_006056b0;
        uVar16 = uVar16 + 1;
        iVar19 = iVar19 + 0x28;
      } while (uVar16 < (uint)((*(int *)(this + 0x19c) - *(int *)pCVar3) / 0x28));
    }
  }
  else {
    iVar13 = *(int *)(this + 0x19c) - *(int *)(this + 0x198);
    iVar12 = iVar13 >> 0x1f;
    if (iVar13 / 0x28 + iVar12 != iVar12) {
      do {
        pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
        lStack_40 = *(long *)pCVar3;
        pCVar14 = param_2 + 0x18;
        pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
        lVar8 = CWorldMap::GetRegionNumberFromName(pCVar7,(CCharString *)pCVar14);
        if (*(int *)(iVar19 + lStack_40) == lVar8) goto LAB_006056b0;
        uVar16 = uVar16 + 1;
        iVar19 = iVar19 + 0x28;
      } while (uVar16 < (uint)((*(int *)(this + 0x19c) - *(int *)pCVar3) / 0x28));
    }
  }
LAB_006056c7:
  puVar20 = *(undefined4 **)(this + 0x19c);
  apCStack_28[0] = (CComponent *)0x0;
  apCStack_28[1] = (CComponent *)0x0;
  apCStack_28[2] = (CComponent *)0x0;
  pCStack_1c = (CRegion *)0x0;
  uStack_18 = 0;
  uStack_14 = 0;
  uStack_10 = 0;
  iStack_c = 0;
  uStack_8 = 0;
  uStack_4 = 0;
  if (puVar20 == *(undefined4 **)(this + 0x1a0)) {
    pCStack_7c = (CManager *)apCStack_28;
    std__vector_insert_40byte();
  }
  else {
    if (puVar20 != (undefined4 *)0x0) {
      ppCVar17 = apCStack_28;
      for (iVar12 = 10; iVar12 != 0; iVar12 = iVar12 + -1) {
        *puVar20 = *ppCVar17;
        ppCVar17 = ppCVar17 + 1;
        puVar20 = puVar20 + 1;
      }
    }
    *(int *)(this + 0x19c) = *(int *)(this + 0x19c) + 0x28;
  }
  piVar18 = (int *)(*(int *)pCVar3 + -0x28 +
                   ((*(int *)(this + 0x19c) - *(int *)pCVar3) / 0x28) * 0x28);
  piStack_50 = piVar18;
  pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  pCVar14 = param_2 + 0x18;
  pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
  lVar8 = CWorldMap::GetRegionNumberFromName(pCVar7,(CCharString *)pCVar14);
  *piVar18 = lVar8;
LAB_00605770:
  CCharString::CCharString((CCharString *)&piStack_3c);
  lStack_40 = 0;
  CCharString::CCharString((CCharString *)apiStack_4c,"UI_MISC_THINGS_DEF",-1);
  ppiVar27 = apiStack_4c;
  this_00 = (COptimisedPrimitive *)GFGetPreMainMemoryUsed();
  lVar8 = QuadricOptimiserInternals::COptimisedPrimitive::ActualToUniqueIndex
                    (this_00,(ulong)ppiVar27);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)apiStack_4c,
             extraout_EDX,unaff_EDI);
  this_01 = NUISystem::CFrontEndManager::GetInstance();
  pCStack_30 = (CRegion *)0x0;
  uStack_2c = 0;
  pCStack_54 = this_01;
  if (param_3) {
    CCharString::CCharString((CCharString *)apiStack_4c,"PC_TEXT_CONTAINER_INSIDE_CONTAINER",-1);
  }
  else {
    CCharString::CCharString((CCharString *)apiStack_4c,"UI_TEXT_CONTAINER_INSIDE_CONTAINER",-1);
  }
  pCVar9 = NUISystem::CManager::CreateComponent
                     ((CManager *)this_01,(CCharString *)apiStack_4c,false);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)&pCStack_30,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)apiStack_4c,
             extraout_EDX_00,unaff_EDI);
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_30;
  (**(code **)(*(int *)param_1 + 0xec))();
  this_02 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  if (0 < lVar8) {
    pCStack_7c = (CManager *)0x60582f;
    CDefinitionManager::GetPDefFromGlobalIndex(this_02,(long)&piStack_50);
    if (piStack_50 != (int *)0x0) {
      piStack_50[1] = piStack_50[1] + 1;
      local_44 = piStack_50;
      iVar12 = piStack_50[1];
      piStack_50[1] = iVar12 + -1;
      if (iVar12 + -1 == 0) {
        (**(code **)(*piStack_50 + 4))();
      }
      piStack_50 = (int *)0x0;
    }
  }
  piStack_3c = (int *)0x0;
  uStack_38 = 0;
  if ((char)param_2 == '\0') {
    pCStack_7c = (CManager *)0x60588e;
    CCharString::CCharString(aCStack_5c,"UI_TEXT_INSIDE_CONTAINER_TEMPLATE",-1);
  }
  else {
    pCStack_7c = (CManager *)0x60587c;
    CCharString::CCharString(aCStack_5c,"PC_TEXT_INSIDE_CONTAINER_TEMPLATE",-1);
  }
  pCStack_7c = (CManager *)0x60589b;
  pCVar9 = NUISystem::CManager::CreateComponent((CManager *)this_01,aCStack_5c,false);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)&piStack_3c,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_5c,
             extraout_EDX_01,ppVar11);
  piVar4 = piStack_3c;
  (**(code **)(*(int *)pCStack_34 + 0xec))();
  piVar18 = *(int **)(unaff_retaddr + 0x24);
  piVar29 = (int *)0x1;
  pCVar14 = unaff_retaddr;
  if (piVar18 != (int *)0x0) {
    piVar18[1] = piVar18[1] + 1;
    pCVar14 = *(CRegion **)(unaff_retaddr + 0x24);
    piVar29 = (int *)0x3;
    if (pCVar14 != (CRegion *)0x0) {
      *(int *)(pCVar14 + 4) = *(int *)(pCVar14 + 4) + 1;
    }
    if (*(int *)(pCVar14 + 0xa0) != 0) {
      piVar28 = (int *)CONCAT13(1,unaff_EBP);
      goto LAB_00605903;
    }
  }
  piVar28 = (int *)(uint)unaff_EBP;
LAB_00605903:
  if ((((uint)piVar29 & 2) != 0) &&
     (piVar29 = (int *)((uint)piVar29 & 0xfffffffd), pCVar14 != (CRegion *)0x0)) {
    pCVar1 = pCVar14 + 4;
    *(int *)pCVar1 = *(int *)pCVar1 + -1;
    if (*(int *)pCVar1 == 0) {
      (**(code **)(*(int *)pCVar14 + 4))();
    }
  }
  if ((((uint)piVar29 & 1) != 0) && (piVar18 != (int *)0x0)) {
    piVar2 = piVar18 + 1;
    *piVar2 = *piVar2 + -1;
    if (*piVar2 == 0) {
      (**(code **)(*piVar18 + 4))();
    }
  }
  if ((char)((uint)piVar28 >> 0x18) == '\0') {
    iVar12 = *piVar4;
    pCVar25 = (CUIRegionInfo *)0x1;
    pCStack_7c = (CManager *)0x1;
    pCVar10 = CRegion::GetDisplayName(unaff_retaddr);
    (**(code **)(iVar12 + 0x244))();
  }
  else {
    pCStack_7c = (CManager *)0x605950;
    CCharString__AssignFromWide();
    piVar18 = *(int **)(unaff_retaddr + 0x24);
    if (piVar18 != (int *)0x0) {
      piVar18[1] = piVar18[1] + 1;
    }
    pCVar10 = (CCharString *)piVar18[0x28];
    pCVar25 = (CUIRegionInfo *)0x1;
    pCStack_7c = (CManager *)&stack0xffffffa0;
    (**(code **)(*piVar4 + 0x23c))();
    piVar4 = piVar18 + 1;
    *piVar4 = *piVar4 + -1;
    if (*piVar4 == 0) {
      (**(code **)(*piVar18 + 4))();
    }
    CCharString::~CCharString((CCharString *)&stack0xffffff94);
  }
  iVar12 = *(int *)(this + 4);
  this_03 = (CTCHeroStats *)0x0;
  if ((*(byte *)(iVar12 + 0x20) & 0x10) != 0) {
    piVar4 = *(int **)(iVar12 + 0x48);
    piVar18 = *(int **)(iVar12 + 0x44);
    iVar12 = (int)piVar4 - (int)piVar18 >> 3;
    while (iVar19 = iVar12, 0 < iVar19) {
      iVar12 = iVar19 / 2;
      if (piVar18[iVar12 * 2] < 4) {
        piVar18 = piVar18 + iVar12 * 2 + 2;
        iVar12 = iVar19 + (-1 - iVar12);
      }
    }
    if ((piVar18 == piVar4) || (4 < *piVar18)) {
      piVar18 = piVar4;
    }
    this_03 = (CTCHeroStats *)piVar18[1];
  }
  CCharString::CCharString(aCStack_5c);
  pCVar15 = (CCharString *)(iStack_c + 0x18);
  CTCHeroStats::GetCrimeReport(this_03,pCVar15,(CWideString *)aCStack_5c,true);
  if ((char)uStack_8 == '\0') {
    CCharString::CCharString((CCharString *)&stack0xffffff94,"UI_TEXT_INSIDE_CONTAINER_TEMPLATE",-1)
    ;
  }
  else {
    CCharString::CCharString((CCharString *)&stack0xffffff94,"PC_TEXT_INSIDE_CONTAINER_TEMPLATE",-1)
    ;
  }
  pCVar9 = NUISystem::CManager::CreateComponent(unaff_ESI,(CCharString *)&stack0xffffff94,false);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)apiStack_4c,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff94,
             extraout_EDX_02,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar10);
  (**(code **)(*local_44 + 0xec))();
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xffffffa0;
  (**(code **)(*apiStack_4c[0] + 0x240))();
  ConstructRegionDescriptionQuests(this,apCStack_28[2],pCStack_1c,SUB41(uStack_18,0));
  pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
  pCStack_7c = (CManager *)CWorldMap::GetRegionNumberFromName(pCVar7,pCVar15);
  iVar12 = std__find_Linear(&pCStack_7c,&pCStack_1c);
  if ((char)uStack_18 == '\0') {
    CCharString::CCharString((CCharString *)&pCStack_7c,"UI_TEXT_CONTAINER_INSIDE_CONTAINER",-1);
  }
  else {
    CCharString::CCharString((CCharString *)&pCStack_7c,"PC_TEXT_CONTAINER_INSIDE_CONTAINER",-1);
  }
  pCVar9 = NUISystem::CManager::CreateComponent
                     ((CManager *)pCVar25,(CCharString *)&pCStack_7c,false);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)&pCStack_54,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_7c,
             extraout_EDX_03,ppVar11);
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_54;
  (**(code **)(*(int *)apCStack_28[2] + 0xec))();
  pCVar26 = pCVar25;
  if (iVar12 != *(int *)(this + 0x138)) {
    bVar23 = SUB41(unaff_EDI,0);
    ConstructRegionDescriptionShops
              (this,local_58,(CRegion *)apCStack_28[2],pCVar25,bVar23,SUB41(pCStack_1c,0));
    ConstructRegionDescriptionBuyableBuildings
              (this,local_58,(CRegion *)apCStack_28[2],pCVar25,bVar23,SUB41(pCStack_1c,0));
  }
  if ((char)pCStack_1c == '\0') {
    CCharString::CCharString((CCharString *)&stack0xffffff94,"UI_TEXT_INSIDE_CONTAINER_TEMPLATE",-1)
    ;
  }
  else {
    CCharString::CCharString((CCharString *)&stack0xffffff94,"PC_TEXT_INSIDE_CONTAINER_TEMPLATE",-1)
    ;
  }
  pCVar9 = NUISystem::CManager::CreateComponent(pCStack_7c,(CCharString *)&stack0xffffff94,false);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
             &stack0xffffffa0,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff94,
             extraout_EDX_04,ppVar11);
  (**(code **)(*(int *)local_58 + 0xec))(&stack0xffffffa0);
  CCharString::CCharString((CCharString *)&stack0xffffff90,"TEXT_GUI_MENU_MAP_KEYS",-1);
  iVar12 = *piVar29;
  uVar24 = 1;
  uVar22 = 1;
  puVar21 = &DAT_01231728;
  NGameText::CDataBank::GetTextBySymbol
            (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&stack0xffffff80);
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
            CCharString__AssignFromValue(puVar21);
  (**(code **)(iVar12 + 0x240))(ppVar11,uVar22,uVar24);
  CCharString::~CCharString((CCharString *)&stack0xffffff78);
  CCharString::~CCharString((CCharString *)&stack0xffffff74);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_7c,
             extraout_EDX_05,ppVar11);
  ConstructRegionDescriptionMapKeys(this,pCStack_34,pCStack_30,SUB41(uStack_2c,0));
  CCharString::~CCharString((CCharString *)&stack0xffffff80);
  if ((unaff_EDI != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) &&
     (*(int *)unaff_EDI = *(int *)unaff_EDI + -1, *(int *)unaff_EDI == 0)) {
    (**(code **)(unaff_EDI + 4))();
    operator_delete(unaff_EDI);
  }
  if ((piVar28 != (int *)0x0) && (*piVar28 = *piVar28 + -1, *piVar28 == 0)) {
    (*(code *)piVar28[1])();
    operator_delete(piVar28);
  }
  if (pCVar26 != (CUIRegionInfo *)0x0) {
    pCVar25 = pCVar26 + 4;
    *(int *)pCVar25 = *(int *)pCVar25 + -1;
    if (*(int *)pCVar25 == 0) {
      (**(code **)(*(int *)pCVar26 + 4))();
    }
  }
  CCharString::~CCharString((CCharString *)&stack0xffffff8c);
  return;
LAB_006056b0:
  piStack_50 = (int *)(*(int *)pCVar3 + uVar16 * 0x28);
  if (piStack_50 == (int *)0x0) goto LAB_006056c7;
  goto LAB_00605770;
}


// CALLEES of ConstructRegionDescription:
//   0099b510  ~CCharString
//   0099bf30  CCharString::AssignFromValue
//   006001e0  ConstructRegionDescriptionShops
//   005fb7e0  ConstructRegionDescriptionMapKeys
//   0099b6b0  CCharString::AssignFromWide
//   00429c6f  Reset
//   0041e5f2  GetInstance
//   009c95e0  GetTextBySymbol
//   0044c6b0  GFGetPreMainMemoryUsed
//   00600b60  ConstructRegionDescriptionBuyableBuildings
//   004fc210  GetRegionNumberFromName
//   0099aed0  CCharString
//   00686a80  GetDisplayEngine
//   0050e540  std::find_Linear
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00618420  std::vector_insert_40byte
//   005fd5e0  ConstructRegionDescriptionQuests
//   004fc190  GetRegionNumberMapIsIn
//   0099ebf0  CCharString
//   00bfe9bc  operator_delete
//   009ad390  ActualToUniqueIndex
//   009ad9e0  GetPDefFromGlobalIndex
//   006bbf20  GetDisplayName
//   0041db1d  CreateComponent
//   0057dc22  GetCrimeReport

//=== Campfire_InsertRegionNode @ 00618100 (seed 0x00618100) ===

/* [forge-autoname conf=medium <- wf] Non-thiscall; malloc/SetPItem in loop; inserts region refs
   into linked list; used by GetRegionsToDisplay, GetPNearCampfire. Campfire-specific naming is
   supported by callers. */

void Campfire_InsertRegionNode(int param_1,undefined4 *param_2,undefined4 *param_3)

{
  CBaseIntelligentPointer *this;
  undefined4 *puVar1;
  int *piVar2;
  CBaseObject *pCVar3;
  
  for (; param_2 != param_3; param_2 = (undefined4 *)*param_2) {
    piVar2 = malloc(0x10);
    this = (CBaseIntelligentPointer *)(piVar2 + 2);
    if (this != (CBaseIntelligentPointer *)0x0) {
      CBaseIntelligentPointer::CBaseIntelligentPointer(this);
      *(undefined ***)this = &PTR__vector_deleting_destructor__01238c6c;
      pCVar3 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(param_2 + 2));
      CBaseIntelligentPointer::SetPItem(this,pCVar3);
    }
    puVar1 = *(undefined4 **)(param_1 + 4);
    piVar2[1] = (int)puVar1;
    *piVar2 = param_1;
    *puVar1 = piVar2;
    *(int **)(param_1 + 4) = piVar2;
  }
  return;
}


// CALLEES of Campfire_InsertRegionNode:
//   00a01b10  CBaseIntelligentPointer
//   00a01b50  GetPItem
//   00bfea0e  malloc
//   00a01b90  SetPItem

