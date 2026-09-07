//=== 0x006061b0 : NO FUNCTION ===
//=== CTCInventoryMap_Serialise @ 00606300 (seed 0x00606300) ===

/* WARNING: Removing unreachable block (ram,0x00606a44) */
/* WARNING: Removing unreachable block (ram,0x006064a8) */
/* autoname-t5 [high]: Thiscall on CTCInventoryBase, CPersistContext for save/load, strings
   'logbook_name_tag', 'logbook_abbreviated_name_tag', 'LogBookCategory', 'ActiveTeleportUID',
   'RegionsFrom', 'Reg */

void CTCInventoryMap_Serialise
               (CPersistContext *param_1,CCategory **param_2,CPersistContext *param_3,
               CTCInventoryBase *param_4,undefined4 param_5,undefined4 param_6,int param_7,
               int *param_8)

{
  CCountedPointer<CIPhysicsFacesSegmentBase_const_> *this;
  CTCInventoryBase *pCVar1;
  CThing *this_00;
  CWideString *pCVar2;
  int iVar3;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar4;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar5;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar6;
  bool bVar7;
  CDisplayEngine *pCVar8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar9;
  int iVar10;
  int *piVar11;
  CThingManager *this_01;
  CIPhysicsFacesSegmentBase *pCVar12;
  CCategory *pCVar13;
  CWideString *pCVar14;
  CTCInventoryBase *in_ECX;
  ulong uVar15;
  CCharString *pCVar16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *unaff_EBX;
  CPersistContext *pCVar18;
  char *unaff_EBP;
  char *unaff_ESI;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar20;
  char *unaff_EDI;
  uint uVar21;
  __uint64 _Var22;
  undefined8 uVar23;
  bool bVar24;
  CCategory **in_stack_ffffffb0;
  CCategory *pCVar25;
  list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *in_stack_ffffffb4;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar26;
  CTCInventoryBase *pCStack_44;
  undefined8 uStack_40;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_38;
  CCategory *pCStack_34;
  CCharString *pCStack_30;
  CCharString *pCStack_2c;
  undefined4 uStack_28;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_24;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_20;
  undefined4 uStack_1c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_18;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_14;
  undefined4 uStack_10;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_8;
  CCategory **ppCStack_4;
  
  pCVar18 = param_1;
  ppVar26 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  bVar7 = SUB41(param_2,0);
  NInventory::CTCInventoryBase::OnSerialise(in_ECX,param_1,bVar7);
  pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
  if (pCVar8[0x102] == (CDisplayEngine)0x0) {
    CPersistContext::Transfer<signed_char>
              (pCVar18,"MiniMapMarkers",(char *)(in_ECX + 0x18c),unaff_EDI);
    CPersistContext::TransferVector<unsigned_long>
              (pCVar18,"VisitedRegions",
               (vector<unsigned_long,std::allocator<unsigned_long>_> *)(in_ECX + 0x134));
    CPersistContext::Transfer<signed_char>
              (pCVar18,"UIRegionsInfo",(char *)(in_ECX + 0x198),unaff_ESI);
  }
  if (bVar7 != false) {
    if ((*(int *)(pCVar18 + 0x18) == 1) || (*(int *)(pCVar18 + 0x18) == 3)) {
      pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      if (pCVar8[0x102] == (CDisplayEngine)0x0) {
        ppCStack_4 = (CCategory **)0x0;
        param_1 = (CPersistContext *)0x0;
        ppVar9 = malloc(0xc);
        *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)ppVar9 = ppVar9;
        *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(ppVar9 + 4) = ppVar9;
        param_4 = (CTCInventoryBase *)0xdd;
        ppVar26 = ppVar9;
        pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
        CThingSearchTools::
        GetAllLeafThings<CThingBuilding_const_,std::vector<CThingBuilding_const*,std::allocator<CThingBuilding_const*>_>,CThingFilter_HasTC>
                  (*(CThingSearchTools **)(pCVar8 + 0x20),(CThingFilter_HasTC *)&param_4,
                   (vector<CThingBuilding_const*,std::allocator<CThingBuilding_const*>_> *)
                   &stack0xffffffb8);
        param_4 = *(CTCInventoryBase **)ppVar9;
        while (param_4 != (CTCInventoryBase *)ppVar9) {
          this_00 = *(CThing **)(param_4 + 8);
          iVar10 = 0;
          if ((*(uint *)(this_00 + 0x38) & 0x20000000) != 0) {
            pCStack_44 = (CTCInventoryBase *)0xdd;
            piVar11 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                             ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                           *)(this_00 + 0x44),(ETCInterfaceType *)&pCStack_44);
            if ((piVar11 == *(int **)(this_00 + 0x48)) || (0xdd < *piVar11)) {
              piVar11 = *(int **)(this_00 + 0x48);
            }
            iVar10 = piVar11[1];
          }
          ppVar6 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_4;
          if (*(char *)(iVar10 + 0x10) != '\0') {
            iVar10 = *(int *)(this_00 + 0x1c);
            if ((iVar10 != 0) || (*(int *)(this_00 + 0x18) != 0)) {
              uVar15 = (ulong)*(short *)(this_00 + 0x8e);
              bVar7 = CThing::IsLevelLess(this_00);
              bVar24 = true;
              this_01 = (CThingManager *)GFGetPreMainMemoryUsed();
              _Var22 = CThingManager::RemapUIDForSerialisation
                                 (this_01,bVar24,bVar7,uVar15,CONCAT44(unaff_EBP,iVar10));
              uStack_40 = _Var22;
              Vector_PushBack_Pair(&uStack_40);
            }
          }
          pCVar18 = param_3;
          param_4 = *(CTCInventoryBase **)ppVar6;
        }
        CPersistContext::Transfer<signed_char>
                  (pCVar18,"ActiveTeleportUID",(char *)&ppCStack_4,unaff_EBP);
        unaff_EBP = (char *)0x6064a0;
        CGuiControlTreePane::SortTreeRecursively
                  ((CGuiControlTreePane *)&pCStack_44,
                   (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EBX);
      }
      pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      pCVar18 = param_3;
      if (pCVar8[0x102] == (CDisplayEngine)0x0) {
        uStack_10 = 0;
        paStack_c = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        paStack_8 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        uStack_40._0_4_ = 0;
        uStack_40._4_4_ = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        paStack_38 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        piVar11 = (int *)**(int **)(in_ECX + 0x24c);
        if (piVar11 != *(int **)(in_ECX + 0x24c)) {
          do {
            pCVar16 = (CCharString *)(piVar11[2] + 0x18);
            pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
            param_4 = (CTCInventoryBase *)
                      CWorldMap::GetRegionNumberFromName((CWorldMap *)pCVar8,pCVar16);
            if (paStack_c == paStack_8) {
              Vector_InsertN_uint32(paStack_c,&param_4,&param_3,1,1);
            }
            else {
              if (paStack_c != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0)
              {
                *(CTCInventoryBase **)paStack_c = param_4;
              }
              paStack_c = paStack_c + 4;
            }
            pCVar16 = (CCharString *)(piVar11[3] + 0x18);
            pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
            param_4 = (CTCInventoryBase *)
                      CWorldMap::GetRegionNumberFromName((CWorldMap *)pCVar8,pCVar16);
            if (uStack_40._4_4_ == paStack_38) {
              Vector_InsertN_uint32(uStack_40._4_4_,&param_4,&param_3,1,1);
            }
            else {
              if (uStack_40._4_4_ !=
                  (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
                *(CTCInventoryBase **)uStack_40._4_4_ = param_4;
              }
              uStack_40._4_4_ = uStack_40._4_4_ + 4;
            }
            piVar11 = (int *)*piVar11;
          } while (piVar11 != (int *)*(int *)(in_ECX + 0x24c));
        }
        pCVar18 = param_3;
        CPersistContext::Transfer<signed_char>(param_3,"RegionsFrom",(char *)&uStack_10,unaff_EBP);
        unaff_EBP = (char *)((int)&uStack_40 + 4);
        CPersistContext::Transfer<signed_char>(pCVar18,"RegionsTo",unaff_EBP,(char *)unaff_EBX);
        if (paStack_38 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
          unaff_EBP = (char *)0x6065ce;
          unaff_EBX = paStack_38;
          free(paStack_38);
        }
        if (paStack_8 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
          unaff_EBP = (char *)0x6065df;
          unaff_EBX = paStack_8;
          free(paStack_8);
        }
      }
      uVar21 = 0;
      uStack_1c = 0;
      paStack_18 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      paStack_14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      uStack_28 = 0;
      paStack_24 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      paStack_20 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      pCStack_34 = (CCategory *)0x0;
      pCStack_30 = (CCharString *)0x0;
      pCStack_2c = (CCharString *)0x0;
      if (*(int *)(in_ECX + 0x154) - *(int *)(in_ECX + 0x150) >> 2 != 0) {
        do {
          uVar23 = CWideString__ConvertToCharString(&param_3);
          ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar23 >> 0x20);
          if (paStack_18 == paStack_14) {
            std__vector__Reallocate(paStack_18,(CCharString *)uVar23,&param_3,1,1);
            ppVar17 = extraout_EDX_00;
          }
          else {
            if (paStack_18 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0)
            {
              CCharString::CCharString((CCharString *)paStack_18,(CCharString *)uVar23);
              ppVar17 = extraout_EDX;
            }
            paStack_18 = paStack_18 + 4;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
                     ppVar17,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EBP);
          uVar21 = uVar21 + 1;
        } while (uVar21 < (uint)(*(int *)(in_ECX + 0x154) - *(int *)(in_ECX + 0x150) >> 2));
      }
      uVar21 = 0;
      if (*(int *)(in_ECX + 0x160) - *(int *)(in_ECX + 0x15c) >> 2 != 0) {
        do {
          uVar23 = CWideString__ConvertToCharString(&param_3);
          ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar23 >> 0x20);
          if (paStack_24 == paStack_20) {
            std__vector__Reallocate(paStack_24,(CCharString *)uVar23,&param_3,1,1);
            ppVar17 = extraout_EDX_02;
          }
          else {
            if (paStack_24 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0)
            {
              CCharString::CCharString((CCharString *)paStack_24,(CCharString *)uVar23);
              ppVar17 = extraout_EDX_01;
            }
            paStack_24 = paStack_24 + 4;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
                     ppVar17,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EBP);
          uVar21 = uVar21 + 1;
        } while (uVar21 < (uint)(*(int *)(in_ECX + 0x160) - *(int *)(in_ECX + 0x15c) >> 2));
      }
      uVar21 = 0;
      if (*(int *)(in_ECX + 0x16c) - *(int *)(in_ECX + 0x168) >> 2 != 0) {
        do {
          uVar23 = CWideString__ConvertToCharString(&param_3);
          ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar23 >> 0x20);
          if (pCStack_30 == pCStack_2c) {
            std__vector__Reallocate(pCStack_30,(CCharString *)uVar23,&param_3,1,1);
            ppVar17 = extraout_EDX_04;
          }
          else {
            if (pCStack_30 != (CCharString *)0x0) {
              CCharString::CCharString(pCStack_30,(CCharString *)uVar23);
              ppVar17 = extraout_EDX_03;
            }
            pCStack_30 = pCStack_30 + 4;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
                     ppVar17,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EBP);
          uVar21 = uVar21 + 1;
        } while (uVar21 < (uint)(*(int *)(in_ECX + 0x16c) - *(int *)(in_ECX + 0x168) >> 2));
      }
      CPersistContext::Transfer<signed_char>
                (pCVar18,"logbook_name_tag",(char *)&uStack_1c,unaff_EBP);
      CPersistContext::Transfer<signed_char>
                (pCVar18,"logbook_abbreviated_name_tag",(char *)&paStack_24,(char *)unaff_EBX);
      CPersistContext::Transfer<signed_char>
                (pCVar18,"logbook_text_tag",(char *)&pCStack_2c,(char *)in_stack_ffffffb0);
      CPersistContext::Transfer<signed_char>
                (pCVar18,"LogBookCategory",(char *)(in_ECX + 0x174),(char *)in_stack_ffffffb4);
      paVar4 = paStack_20;
      ppVar17 = extraout_EDX_05;
      for (paVar19 = paStack_24; paVar19 != paVar4; paVar19 = paVar19 + 4) {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar19,ppVar17,ppVar26);
        ppVar17 = extraout_EDX_06;
      }
      paVar19 = paStack_18;
      paVar4 = paStack_14;
      if (paStack_24 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
        free(paStack_24);
        ppVar17 = extraout_EDX_07;
        paVar19 = paStack_18;
        paVar4 = paStack_14;
      }
      for (; paVar5 = paStack_14, paVar19 != paStack_14; paVar19 = paVar19 + 4) {
        paStack_14 = paVar4;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar19,ppVar17,ppVar26);
        ppVar17 = extraout_EDX_08;
        paVar4 = paStack_14;
        paStack_14 = paVar5;
      }
      paStack_14 = paVar4;
      paVar19 = paStack_c;
      paVar5 = paStack_8;
      if (paStack_18 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
        free(paStack_18);
        ppVar17 = extraout_EDX_09;
        paVar19 = paStack_c;
        paVar5 = paStack_8;
      }
      for (; paVar4 = paStack_8, paVar19 != paStack_8; paVar19 = paVar19 + 4) {
        paStack_8 = paVar5;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar19,ppVar17,ppVar26);
        ppVar17 = extraout_EDX_10;
        paVar5 = paStack_8;
        paStack_8 = paVar4;
      }
      if (paStack_c != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
        paStack_8 = paVar5;
        free(paStack_c);
        return;
      }
    }
    else {
      pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
      if (pCVar8[0x102] == (CDisplayEngine)0x0) {
        pCVar12 = operator_new(0xc);
        if (pCVar12 == (CIPhysicsFacesSegmentBase *)0x0) {
          pCVar12 = (CIPhysicsFacesSegmentBase *)0x0;
        }
        else {
          *(undefined4 *)pCVar12 = 0;
          *(undefined4 *)(pCVar12 + 4) = 0;
          *(undefined4 *)(pCVar12 + 8) = 0;
        }
        this = (CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(in_ECX + 0x200);
        CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset(this,pCVar12);
        pCVar18 = param_3;
        CPersistContext::Transfer<signed_char>(param_3,"ActiveTeleportUID",*(char **)this,unaff_EBP)
        ;
        CTCInventoryMap::RemapUIDsForLoading
                  ((CTCInventoryMap *)in_ECX,
                   *(vector<unsigned___int64,std::allocator<unsigned___int64>_> **)this);
        uVar21 = 0;
        paStack_c = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        paStack_8 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
        ppCStack_4 = (CCategory **)0x0;
        param_1 = (CPersistContext *)0x0;
        param_2 = (CCategory **)0x0;
        CPersistContext::Transfer<signed_char>
                  (pCVar18,"RegionsFrom",(char *)&paStack_c,(char *)unaff_EBX);
        unaff_EBX = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1;
        unaff_EBP = "RegionsTo";
        CPersistContext::Transfer<signed_char>
                  (pCVar18,"RegionsTo",(char *)unaff_EBX,(char *)in_stack_ffffffb0);
        in_stack_ffffffb0 = (CCategory **)0x606912;
        CGuiControlTreePane::SortTreeRecursively
                  ((CGuiControlTreePane *)(in_ECX + 0x24c),in_stack_ffffffb4);
        if (-(int)ppCStack_4 >> 2 != 0) {
          do {
            if ((uint)((int)param_3 - (int)param_2 >> 2) <= uVar21) break;
            pCVar25 = param_2[uVar21];
            pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
            pCVar13 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                                ((CTCInventoryBase *)pCVar8,(long)pCVar25);
            pCVar25 = ppCStack_4[uVar21];
            pCVar8 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)in_ECX);
            pCStack_34 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                                   ((CTCInventoryBase *)pCVar8,(long)pCVar25);
            in_stack_ffffffb0 = &pCStack_34;
            unaff_EBX = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x606972;
            pCStack_30 = (CCharString *)pCVar13;
            CList_PushBack(in_stack_ffffffb0);
            uVar21 = uVar21 + 1;
          } while (uVar21 < (uint)(-(int)ppCStack_4 >> 2));
        }
        if (param_2 != (CCategory **)0x0) {
          unaff_EBX = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x606992;
          in_stack_ffffffb0 = param_2;
          free(param_2);
        }
        if (ppCStack_4 != (CCategory **)0x0) {
          unaff_EBX = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x6069a3;
          in_stack_ffffffb0 = ppCStack_4;
          free(ppCStack_4);
        }
      }
      param_4 = in_ECX + 0x168;
      pCStack_34 = (CCategory *)0x0;
      pCStack_30 = (CCharString *)0x0;
      pCStack_2c = (CCharString *)0x0;
      uStack_28 = 0;
      paStack_24 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      paStack_20 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      uStack_1c = 0;
      paStack_18 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      paStack_14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
      Std_Vector_Erase_Range(*(undefined4 *)(in_ECX + 0x168),*(undefined4 *)(in_ECX + 0x16c));
      pCStack_44 = in_ECX + 0x15c;
      Std_Vector_Erase_Range(*(undefined4 *)(in_ECX + 0x15c),*(undefined4 *)(in_ECX + 0x160));
      pCVar1 = in_ECX + 0x150;
      Std_Vector_Erase_Range(*(undefined4 *)(in_ECX + 0x150),*(undefined4 *)(in_ECX + 0x154));
      pCVar18 = param_3;
      *(undefined4 *)(in_ECX + 0x178) = *(undefined4 *)(in_ECX + 0x174);
      ppVar26 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)(in_ECX + 0x174);
      CPersistContext::Transfer<signed_char>
                (param_3,"logbook_name_tag",(char *)&pCStack_34,unaff_EBP);
      CPersistContext::Transfer<signed_char>
                (pCVar18,"logbook_abbreviated_name_tag",(char *)&paStack_24,(char *)unaff_EBX);
      CPersistContext::Transfer<signed_char>
                (pCVar18,"logbook_text_tag",(char *)&paStack_14,(char *)in_stack_ffffffb0);
      CPersistContext::Transfer<signed_char>
                (pCVar18,"LogBookCategory",(char *)(in_ECX + 0x174),(char *)in_stack_ffffffb4);
      uVar21 = 0;
      if ((int)paStack_20 - (int)paStack_24 >> 2 != 0) {
        do {
          pCVar14 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
          pCVar2 = *(CWideString **)(in_ECX + 0x154);
          if (pCVar2 == *(CWideString **)(in_ECX + 0x158)) {
            Std_Vector_Insert_Range_Copies(pCVar2,pCVar14,&param_7,1,1);
          }
          else {
            if (pCVar2 != (CWideString *)0x0) {
              CWideString::CWideString(pCVar2,pCVar14);
            }
            *(int *)(in_ECX + 0x154) = *(int *)(in_ECX + 0x154) + 4;
          }
          CCharString::~CCharString((CCharString *)&param_7);
          uVar21 = uVar21 + 1;
        } while (uVar21 < (uint)((int)paStack_20 - (int)paStack_24 >> 2));
      }
      uVar21 = 0;
      if ((int)paStack_14 - (int)paStack_18 >> 2 != 0) {
        do {
          pCVar14 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
          pCVar2 = *(CWideString **)(in_ECX + 0x160);
          if (pCVar2 == *(CWideString **)(in_ECX + 0x164)) {
            Std_Vector_Insert_Range_Copies(pCVar2,pCVar14,&param_7,1,1);
          }
          else {
            if (pCVar2 != (CWideString *)0x0) {
              CWideString::CWideString(pCVar2,pCVar14);
            }
            *(int *)(in_ECX + 0x160) = *(int *)(in_ECX + 0x160) + 4;
          }
          CCharString::~CCharString((CCharString *)&param_7);
          uVar21 = uVar21 + 1;
        } while (uVar21 < (uint)((int)paStack_14 - (int)paStack_18 >> 2));
      }
      piVar11 = param_8;
      uVar21 = 0;
      if ((int)paStack_8 - (int)paStack_c >> 2 != 0) {
        do {
          pCVar14 = (CWideString *)CPersistContext::GetDefaultVal<CAnimationSet>();
          pCVar2 = (CWideString *)piVar11[1];
          if (pCVar2 == (CWideString *)piVar11[2]) {
            Std_Vector_Insert_Range_Copies(pCVar2,pCVar14,&param_7,1,1);
          }
          else {
            if (pCVar2 != (CWideString *)0x0) {
              CWideString::CWideString(pCVar2,pCVar14);
            }
            piVar11[1] = piVar11[1] + 4;
          }
          CCharString::~CCharString((CCharString *)&param_7);
          uVar21 = uVar21 + 1;
        } while (uVar21 < (uint)((int)paStack_8 - (int)paStack_c >> 2));
      }
      ppVar20 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
      ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0;
      paVar19 = paStack_c;
      paVar4 = paStack_8;
      if (*(int *)(in_ECX + 0x154) - *(int *)pCVar1 >> 2 != 0) {
        do {
          ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                    (*(int *)(pCStack_34 + 4) - *(int *)pCStack_34 >> 2);
          paVar19 = paStack_c;
          paVar4 = paStack_8;
          if (((ppVar17 <= ppVar20) ||
              (ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                         (param_8[1] - *param_8 >> 2), ppVar17 <= ppVar20)) ||
             (ppVar17 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                        (*(int *)(paStack_38 + 4) - *(int *)paStack_38 >> 2), ppVar17 <= ppVar20))
          break;
          iVar10 = *param_8;
          iVar3 = *(int *)paStack_38;
          param_7 = *(int *)pCStack_34;
          pCStack_30 = *(CCharString **)pCVar1;
          Catch_0040d959();
          CHeroLogBook_AddLogEntry
                    ((int)pCStack_30 + (int)ppVar20 * 4,param_7 + (int)ppVar20 * 4,
                     iVar10 + (int)ppVar20 * 4,*(undefined4 *)(iVar3 + (int)ppVar20 * 4),0);
          ppVar20 = ppVar20 + 1;
          ppVar17 = extraout_EDX_11;
          paVar19 = paStack_c;
          paVar4 = paStack_8;
        } while (ppVar20 < (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                           (*(int *)(in_ECX + 0x154) - *(int *)pCVar1 >> 2));
      }
      for (; paVar5 = paStack_8, paVar19 != paStack_8; paVar19 = paVar19 + 4) {
        paStack_8 = paVar4;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar19,ppVar17,ppVar26);
        ppVar17 = extraout_EDX_12;
        paVar4 = paStack_8;
        paStack_8 = paVar5;
      }
      paVar19 = paStack_18;
      paVar5 = paStack_14;
      paStack_8 = paVar4;
      if (paStack_c != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
        free(paStack_c);
        ppVar17 = extraout_EDX_13;
        paVar19 = paStack_18;
        paVar5 = paStack_14;
      }
      for (; paVar4 = paStack_14, paVar19 != paStack_14; paVar19 = paVar19 + 4) {
        paStack_14 = paVar5;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar19,ppVar17,ppVar26);
        ppVar17 = extraout_EDX_14;
        paVar5 = paStack_14;
        paStack_14 = paVar4;
      }
      paVar19 = paStack_24;
      paVar4 = paStack_20;
      paStack_14 = paVar5;
      if (paStack_18 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
        free(paStack_18);
        ppVar17 = extraout_EDX_15;
        paVar19 = paStack_24;
        paVar4 = paStack_20;
      }
      for (; paVar5 = paStack_20, paVar19 != paStack_20; paVar19 = paVar19 + 4) {
        paStack_20 = paVar4;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar19,ppVar17,ppVar26);
        ppVar17 = extraout_EDX_16;
        paVar4 = paStack_20;
        paStack_20 = paVar5;
      }
      if (paStack_24 != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
        paStack_20 = paVar4;
        free(paStack_24);
      }
    }
  }
  return;
}


// CALLEES of CTCInventoryMap_Serialise:
//   006192f0  Transfer<signed_char>
//   00618660  Reset
//   00617230  CList_PushBack
//   004fb490  GetCategoryWithIndex
//   00bfea0e  malloc
//   00686d30  GetDisplayEngine
//   00619710  Transfer<signed_char>
//   005fae90  RemapUIDsForLoading
//   00412450  Std_Vector_Erase_Range
//   00613b20  SortTreeRecursively
//   0099b510  ~CCharString
//   00464ace  Vector_InsertN_uint32
//   00686a80  GetDisplayEngine
//   00bfea14  free
//   0099e480  GetDefaultVal<CAnimationSet>
//   0099b2c0  CWideString::ConvertToCharString
//   00619890  Transfer<signed_char>
//   005e9840  Transfer<signed_char>
//   0051e210  RemapUIDForSerialisation
//   005815d6  TransferVector<unsigned_long>
//   00615360  SortTreeRecursively
//   0049b8d0  Transfer<signed_char>
//   00449970  Catch@0040d959
//   00bfea1a  operator_new
//   004c7930  IsLevelLess
//   005bfd5e  OnSerialise
//   00686ae0  GFGetPreMainMemoryUsed
//   006197d0  Transfer<signed_char>
//   006181a0  Vector_PushBack_Pair
//   004365b0  LowerBound
//   004fc210  GetRegionNumberFromName
//   00617970  GetAllLeafThings<CThingBuilding_const_,std::vector<CThingBuilding_const*,std::allocator<CThingBuilding_const*>_>,CThingFilter_HasTC>
//   0099ec30  CCharString
//   0099b720  CWideString
//   0047a9f0  CHeroLogBook_AddLogEntry
//   00433530  std::vector::Reallocate
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00412330  Std_Vector_Insert_Range_Copies

//=== FindTeleports @ 005fc620 (seed 0x005fc620) ===

/* WARNING: Removing unreachable block (ram,0x005fc647) */
/* [bsim sim=0.5373931830728663 <- ego_r]
   private: void __thiscall CTCInventoryMap::FindTeleports(void) */

void __thiscall CTCInventoryMap::FindTeleports(CTCInventoryMap *this)

{
  undefined4 *puVar1;
  int *piVar2;
  long *plVar3;
  undefined4 *puVar4;
  int iVar5;
  CDisplayEngine *pCVar6;
  CCategory *pCVar7;
  undefined4 *_Memory;
  int iVar8;
  int *piVar9;
  undefined1 uStack_39;
  long lStack_38;
  uint uStack_34;
  CTCInventoryMap *local_30;
  CTCInventoryBase *pCStack_2c;
  undefined4 uStack_28;
  undefined4 *puStack_24;
  CCategory *pCStack_20;
  int iStack_1c;
  CTCInventoryBase *pCStack_18;
  undefined4 uStack_14;
  undefined4 *puStack_10;
  CTCInventoryBase **ppCStack_c;
  
  *(undefined4 *)(this + 0x20c) = *(undefined4 *)(this + 0x208);
  local_30 = this;
  pCVar6 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  pCStack_2c = (CTCInventoryBase *)(**(code **)(*(int *)pCVar6 + 0x34))();
  lStack_38 = 1;
  if (0 < *(int *)(this + 0x23c)) {
    do {
      pCVar7 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCStack_2c,lStack_38);
      uStack_34 = 0;
      pCStack_20 = pCVar7;
      if (*(int *)(pCVar7 + 4) - *(int *)pCVar7 >> 2 != 0) {
        puStack_10 = &uStack_28;
        ppCStack_c = &pCStack_18;
        do {
          _Memory = malloc(0xc);
          *_Memory = _Memory;
          _Memory[1] = _Memory;
          uStack_14 = *(undefined4 *)(*(int *)pCVar7 + uStack_34 * 4);
          pCStack_18 = pCStack_2c;
          uStack_28 = 0xdd;
          puStack_24 = _Memory;
          pCVar6 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
          CThingSearchTools::
          GetAllLeafThings<CThingAICreature,std::vector<CThingAICreature*,std::allocator<CThingAICreature*>_>,CFilter_Bind2<CThingFilter_EnemyCreature,CThingFilter_InMap>_>
                    (*(CThingSearchTools **)(pCVar6 + 0x20),
                     (CFilter_Bind2<CThingFilter_EnemyCreature,CThingFilter_InMap> *)&puStack_10,
                     (vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *)&puStack_24);
          for (puVar1 = (undefined4 *)*_Memory; puVar1 != _Memory; puVar1 = (undefined4 *)*puVar1) {
            iVar8 = puVar1[2];
            if ((*(uint *)(iVar8 + 0x38) & 0x20000000) != 0) {
              piVar2 = *(int **)(iVar8 + 0x48);
              piVar9 = *(int **)(iVar8 + 0x44);
              iVar8 = (int)piVar2 - (int)piVar9 >> 3;
              while (iVar5 = iVar8, 0 < iVar5) {
                iVar8 = iVar5 / 2;
                if (piVar9[iVar8 * 2] < 0xdd) {
                  piVar9 = piVar9 + iVar8 * 2 + 2;
                  iVar8 = iVar5 + (-1 - iVar8);
                }
              }
              if ((piVar9 == piVar2) || (0xdd < *piVar9)) {
                piVar9 = piVar2;
              }
              iStack_1c = piVar9[1];
              pCVar7 = pCStack_20;
            }
            if (*(char *)(iStack_1c + 0x10) != '\0') {
              plVar3 = *(long **)(local_30 + 0x20c);
              if (plVar3 == *(long **)(local_30 + 0x210)) {
                std__vector__InsertMultipleAndGrow(plVar3,&lStack_38,&uStack_39,1,1);
              }
              else {
                if (plVar3 != (long *)0x0) {
                  *plVar3 = lStack_38;
                }
                *(int *)(local_30 + 0x20c) = *(int *)(local_30 + 0x20c) + 4;
              }
              break;
            }
          }
          puVar1 = (undefined4 *)*_Memory;
          while (puVar1 != _Memory) {
            puVar4 = (undefined4 *)*puVar1;
            free(puVar1);
            puVar1 = puVar4;
          }
          *_Memory = _Memory;
          _Memory[1] = _Memory;
          free(_Memory);
          uStack_34 = uStack_34 + 1;
          this = local_30;
        } while (uStack_34 < (uint)(*(int *)(pCVar7 + 4) - *(int *)pCVar7 >> 2));
      }
      lStack_38 = lStack_38 + 1;
    } while (lStack_38 <= *(int *)(this + 0x23c));
  }
  return;
}


// CALLEES of FindTeleports:
//   00617780  GetAllLeafThings<CThingAICreature,std::vector<CThingAICreature*,std::allocator<CThingAICreature*>_>,CFilter_Bind2<CThingFilter_EnemyCreature,CThingFilter_InMap>_>
//   00440990  std::vector::InsertMultipleAndGrow
//   004fc180  GetCategoryWithIndex
//   00bfea0e  malloc
//   00686a80  GetDisplayEngine
//   00bfea14  free
//   00bfeae6  memmove

//=== TeleportHeroToRegionTeleporter @ 005fc840 (seed 0x005fc840) ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCInventoryMap::TeleportHeroToRegionTeleporter(long)const  */

void __thiscall CTCInventoryMap::TeleportHeroToRegionTeleporter(CTCInventoryMap *this,long param_1)

{
  int extraout_EAX;
  CDisplayEngine *this_00;
  CCategory *pCVar1;
  CThing *pCVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  undefined8 uVar3;
  long in_stack_00000008;
  
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  uVar3 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),unaff_ESI);
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    this_00 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    pCVar1 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                       ((CTCInventoryBase *)this_00,in_stack_00000008);
    pCVar2 = GetRegionTeleporter(this,(CRegion *)pCVar1);
    if ((pCVar2 != (CThing *)0x0) && (((byte)pCVar2[0x91] & 1) == 0)) {
      TeleportHeroToTeleporter(this,pCVar2);
    }
  }
  return;
}


// CALLEES of TeleportHeroToRegionTeleporter:
//   005fbea0  GetRegionTeleporter
//   00487dd0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   004fc180  GetCategoryWithIndex
//   00686d20  GetDisplayEngine
//   005fbd20  TeleportHeroToTeleporter
//   00449960  Catch@0040d959
//   00686d50  GetDisplayEngine

