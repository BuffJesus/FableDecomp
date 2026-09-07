//=== UpdateVisitedRegions @ 00605ce0 (seed 0x00605ce0) ===

/* [bsim sim=0.7271827020007923 <- ego_r]
   protected: void __thiscall CTCInventoryMap::UpdateVisitedRegions(long,long) */

void __thiscall
CTCInventoryMap::UpdateVisitedRegions(CTCInventoryMap *this,long param_1,long param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  long *plVar3;
  int *piVar4;
  undefined4 *_Memory;
  CDisplayEngine *pCVar5;
  int iVar6;
  CWorldMap *pCVar7;
  CFrontEndManager *pCVar8;
  CComponent *pCVar9;
  CDisplayEngine *pCVar10;
  long lVar11;
  CTCInventoryBase *pCVar12;
  CCategory *pCVar13;
  int iVar14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString *pCVar16;
  bool bVar17;
  undefined1 uVar18;
  int *piStack_14;
  long local_10;
  int *local_c;
  long *local_8;
  long *local_4;
  
  _Memory = malloc(0xc);
  local_10 = param_1;
  local_c = (int *)param_2;
  local_8 = &param_1;
  local_4 = &local_10;
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  param_1 = 0x26;
  pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  CMessageEventManager::
  FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_HasExtraData,CMessageEventFilter_OccuredBetween>_>
            (*(CMessageEventManager **)(pCVar5 + 0x60),
             (CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_HasExtraData,CMessageEventFilter_OccuredBetween>
              *)&local_8);
  for (puVar1 = (undefined4 *)*_Memory; puVar1 != _Memory; puVar1 = (undefined4 *)*puVar1) {
    iVar14 = *(int *)(this + 0x138);
    iVar6 = std__find_Linear(puVar1[2] + 0x3c,&param_1);
    *(undefined4 *)(this + 0x180) = 0xf;
    if (iVar6 == iVar14) {
      puVar2 = *(undefined4 **)(this + 0x138);
      if (puVar2 == *(undefined4 **)(this + 0x13c)) {
        std__vector__InsertMultipleAndGrow(puVar2,(undefined4 *)(puVar1[2] + 0x3c),&param_1,1,1);
      }
      else {
        if (puVar2 != (undefined4 *)0x0) {
          *puVar2 = *(undefined4 *)(puVar1[2] + 0x3c);
        }
        *(int *)(this + 0x138) = *(int *)(this + 0x138) + 4;
      }
    }
  }
  if (this[0x1a8] != (CTCInventoryMap)0x0) {
    pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    iVar14 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
    pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
    param_1 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar14);
    iVar14 = *(int *)(this + 0x138);
    iVar6 = std__find_Linear(&param_1,&param_1);
    if (iVar6 == iVar14) {
      pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      iVar14 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
      pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
      param_1 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar14);
      plVar3 = *(long **)(this + 0x138);
      if (plVar3 == *(long **)(this + 0x13c)) {
        std__vector__InsertMultipleAndGrow(plVar3,&param_1,&param_1,1,1);
      }
      else {
        if (plVar3 != (long *)0x0) {
          *plVar3 = param_1;
        }
        *(int *)(this + 0x138) = *(int *)(this + 0x138) + 4;
      }
      CCharString::CCharString((CCharString *)&param_1,"UI_MAP_MENU_MINI_MAP_TEMPLATE",-1);
      bVar17 = false;
      pCVar16 = (CCharString *)&param_1;
      pCVar8 = NUISystem::CFrontEndManager::GetInstance();
      pCVar9 = NUISystem::CManager::CreateComponent((CManager *)pCVar8,pCVar16,bVar17);
      ppVar15 = extraout_EDX;
      if ((pCVar9 == (CComponent *)0x0) ||
         (local_c = ::operator_new(0xc), ppVar15 = extraout_EDX_00, local_c == (int *)0x0)) {
        local_c = (int *)0x0;
      }
      else {
        *local_c = 1;
        local_c[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
        local_c[2] = (int)pCVar9;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,ppVar15,
                 unaff_EDI);
      pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      pCVar10 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      iVar14 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
      pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
      lVar11 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar14);
      pCVar12 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar10 + 0x30))();
      pCVar13 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar12,lVar11);
      CreateMiniMapMarkers(this,pCVar9,(CRegion *)pCVar13);
      CCharString::CCharString
                ((CCharString *)&param_1,"UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE",-1);
      bVar17 = false;
      pCVar16 = (CCharString *)&param_1;
      pCVar8 = NUISystem::CFrontEndManager::GetInstance();
      pCVar9 = NUISystem::CManager::CreateComponent((CManager *)pCVar8,pCVar16,bVar17);
      ppVar15 = extraout_EDX_01;
      if ((pCVar9 == (CComponent *)0x0) ||
         (piStack_14 = ::operator_new(0xc), ppVar15 = extraout_EDX_02, piStack_14 == (int *)0x0)) {
        piStack_14 = (int *)0x0;
      }
      else {
        *piStack_14 = 1;
        piStack_14[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
        piStack_14[2] = (int)pCVar9;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,ppVar15,
                 unaff_EDI);
      pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      pCVar10 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
      iVar14 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
      uVar18 = 0;
      pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
      lVar11 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar14);
      pCVar12 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar10 + 0x30))();
      pCVar13 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar12,lVar11);
      ConstructRegionDescription(this,pCVar9,(CRegion *)pCVar13,(bool)uVar18);
      if ((piStack_14 != (int *)0x0) && (*piStack_14 = *piStack_14 + -1, *piStack_14 == 0)) {
        (*(code *)piStack_14[1])();
        operator_delete(piStack_14);
      }
      piVar4 = local_c;
      if ((local_c != (int *)0x0) && (*local_c = *local_c + -1, *local_c == 0)) {
        (*(code *)local_c[1])();
        operator_delete(piVar4);
      }
    }
    this[0x1a8] = (CTCInventoryMap)0x0;
  }
  iVar14 = *(int *)(this + 0x180);
  if (iVar14 == 0) {
    *(undefined4 *)(this + 0x180) = 0xffffffff;
    CCharString::CCharString((CCharString *)&param_1,"UI_MAP_MENU_MINI_MAP_TEMPLATE",-1);
    bVar17 = false;
    pCVar16 = (CCharString *)&param_1;
    pCVar8 = NUISystem::CFrontEndManager::GetInstance();
    pCVar9 = NUISystem::CManager::CreateComponent((CManager *)pCVar8,pCVar16,bVar17);
    ppVar15 = extraout_EDX_03;
    if ((pCVar9 == (CComponent *)0x0) ||
       (piStack_14 = ::operator_new(0xc), ppVar15 = extraout_EDX_04, piStack_14 == (int *)0x0)) {
      piStack_14 = (int *)0x0;
    }
    else {
      *piStack_14 = 1;
      piStack_14[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piStack_14[2] = (int)pCVar9;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,ppVar15,
               unaff_EDI);
    pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar10 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    iVar14 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
    pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
    lVar11 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar14);
    pCVar12 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar10 + 0x30))();
    pCVar13 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar12,lVar11);
    CreateMiniMapMarkers(this,pCVar9,(CRegion *)pCVar13);
    CCharString::CCharString
              ((CCharString *)&param_1,"UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE",-1);
    bVar17 = false;
    pCVar16 = (CCharString *)&param_1;
    pCVar8 = NUISystem::CFrontEndManager::GetInstance();
    pCVar9 = NUISystem::CManager::CreateComponent((CManager *)pCVar8,pCVar16,bVar17);
    ppVar15 = extraout_EDX_05;
    if ((pCVar9 == (CComponent *)0x0) ||
       (local_c = ::operator_new(0xc), ppVar15 = extraout_EDX_06, local_c == (int *)0x0)) {
      local_c = (int *)0x0;
    }
    else {
      *local_c = 1;
      local_c[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      local_c[2] = (int)pCVar9;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,ppVar15,
               unaff_EDI);
    pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar10 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    iVar14 = (int)*(short *)(*(int *)(this + 4) + 0x8e);
    uVar18 = 0;
    pCVar7 = (CWorldMap *)(**(code **)(*(int *)pCVar5 + 0x34))();
    lVar11 = CWorldMap::GetRegionNumberMapIsIn(pCVar7,iVar14);
    pCVar12 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar10 + 0x30))();
    pCVar13 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar12,lVar11);
    ConstructRegionDescription(this,pCVar9,(CRegion *)pCVar13,(bool)uVar18);
    piVar4 = local_c;
    if ((local_c != (int *)0x0) && (*local_c = *local_c + -1, *local_c == 0)) {
      (*(code *)local_c[1])();
      operator_delete(piVar4);
    }
    if ((piStack_14 != (int *)0x0) && (*piStack_14 = *piStack_14 + -1, *piStack_14 == 0)) {
      (*(code *)piStack_14[1])();
      operator_delete(piStack_14);
    }
  }
  else if (0 < iVar14) {
    *(int *)(this + 0x180) = iVar14 + -1;
  }
  puVar1 = (undefined4 *)*_Memory;
  while (puVar1 != _Memory) {
    puVar2 = (undefined4 *)*puVar1;
    free(puVar1);
    puVar1 = puVar2;
  }
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  free(_Memory);
  return;
}


// CALLEES of UpdateVisitedRegions:
//   00440990  std::vector::InsertMultipleAndGrow
//   00bfea14  free
//   004fb490  GetCategoryWithIndex
//   00686a70  GetDisplayEngine
//   00603000  CreateMiniMapMarkers
//   0041db1d  CreateComponent
//   004fc190  GetRegionNumberMapIsIn
//   0099ebf0  CCharString
//   0041e5f2  GetInstance
//   00bfe9bc  operator_delete
//   0050e540  std::find_Linear
//   00686a80  GetDisplayEngine
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00bfea1a  operator_new
//   00bfea0e  malloc
//   006178f0  FindMostRecentMessage<CFilter_Bind3<CMessageEventFilter_IsOfType,CMessageEventFilter_HasExtraData,CMessageEventFilter_OccuredBetween>_>
//   00605570  ConstructRegionDescription

