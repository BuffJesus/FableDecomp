//=== ConstructLogbookList @ 00607330 ===

/* WARNING: Removing unreachable block (ram,0x00608137) */
/* WARNING: Removing unreachable block (ram,0x00608141) */
/* [bsim sim=0.6217344599229654 <- ego_r]
   public: class NUISystem::CComponent * __thiscall CTCInventoryMap::ConstructLogbookList(class
   NUISystem::CComponent *) */

CComponent * __thiscall
CTCInventoryMap::ConstructLogbookList(CTCInventoryMap *this,CComponent *param_1)

{
  undefined **ppuVar1;
  CWideString *pCVar2;
  undefined4 *puVar3;
  void *pvVar4;
  CManager *pCVar5;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *this_00;
  bool bVar6;
  CFrontEndManager *pCVar7;
  CComponent *pCVar8;
  int *piVar9;
  int iVar10;
  void *pvVar11;
  CComponent *pCVar12;
  CDefinitionManager *this_01;
  undefined4 *puVar13;
  int *piVar14;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar15;
  CComponent *pCVar16;
  void *pvVar17;
  undefined4 *_Memory;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  uint uVar18;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX_00;
  int iVar19;
  int iVar20;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_26;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_27;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_28;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_31;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_32;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_37;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_38;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_39;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_40;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_41;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_42;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_43;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_44;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_45;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_46;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_47;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_48;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_49;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_50;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_51;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_52;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_53;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_54;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_55;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_56;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_57;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_58;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_59;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_60;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_61;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_62;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_63;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_64;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_65;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_66;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_67;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_68;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_69;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_70;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_71;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_72;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_73;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_74;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_75;
  CManager *unaff_EBX;
  CDefClassBase *this_02;
  LTextTreeWalkThrough *this_03;
  undefined **ppuVar22;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar23;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CComponent *pCVar24;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **pppVar25;
  int *piVar26;
  CCharString *pCVar27;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar28;
  CCharString CVar29;
  CComponent *pCStack_114;
  CManager *pCStack_110;
  CManager *pCStack_10c;
  CManager *pCStack_108;
  CManager *pCStack_104;
  CComponent *pCStack_100;
  CFrontEndManager *local_fc;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_f8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_f4;
  CComponent *pCStack_f0;
  CComponent *pCStack_ec;
  CComponent *pCStack_e8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_e4;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_e0;
  CComponent *pCStack_dc;
  int *piStack_d8;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paStack_d4;
  undefined **ppuStack_d0;
  CTCInventoryMap *local_cc;
  int *piStack_c8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_c4;
  int iStack_c0;
  undefined1 auStack_bc [4];
  int *piStack_b8;
  int *piStack_b4;
  int *piStack_b0;
  undefined *puStack_ac;
  int *piStack_a8;
  CComponent *pCStack_a4;
  CComponent *pCStack_a0;
  CCharString aCStack_9c [4];
  CComponent *local_98;
  int *piStack_94;
  void *pvStack_90;
  int iStack_8c;
  void *pvStack_84;
  undefined **ppuStack_80;
  undefined **ppuStack_7c;
  undefined **ppuStack_78;
  CCharString aCStack_6c [12];
  CCharString aCStack_60 [4];
  undefined1 auStack_5c [4];
  CCharString aCStack_58 [4];
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *local_54;
  int *piStack_50;
  int *piStack_4c;
  int *apiStack_48 [2];
  undefined1 auStack_3e [58];
  
  local_cc = this;
  pCVar7 = NUISystem::CFrontEndManager::GetInstance();
  local_fc = pCVar7;
  Catch_0040d959();
  FUN_00619c50();
  CCharString::CCharString((CCharString *)&local_54,"UI_LOGBOOK_CATEGORIES",-1);
  pCVar8 = NUISystem::CManager::CreateComponent((CManager *)pCVar7,(CCharString *)&local_54,false);
  local_98 = pCVar8;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_54,extraout_EDX
             ,unaff_EDI);
  iVar10 = *(int *)pCVar8;
  ppVar15 = extraout_ECX;
  CCharString::CCharString((CCharString *)&stack0xfffffedc,"UI_SCROLLING_VIEWPORT_TEXT_AREA",-1);
  piVar9 = (int *)(**(code **)(iVar10 + 0xc))();
  piStack_4c = piVar9;
  if (piVar9 != (int *)0x0) {
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffed8,"UI_LOGBOOK_BASICS_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffed4,"UI_LOGBOOK_OBJECTS_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffed0,"UI_LOGBOOK_TOWNS_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffecc,"UI_LOGBOOK_HERO_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffec8,"UI_LOGBOOK_COMBAT_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffec4,"UI_LOGBOOK_QUESTS_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffec0,"UI_LOGBOOK_STORY_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
    iVar10 = *(int *)pCVar8;
    CCharString::CCharString((CCharString *)&stack0xfffffebc,"UI_LOGBOOK_BOOKS_DESC",-1);
    iVar10 = (**(code **)(iVar10 + 0xc))();
    if (iVar10 != 0) {
      (**(code **)(*piVar9 + 0x238))();
    }
  }
  ppuVar22 = ppuStack_d0;
  this_03 = (LTextTreeWalkThrough *)((int)ppuStack_d0 + 0x144);
  if (*(int *)((int)ppuStack_d0 + 0x148) != 0) {
    LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
              (this_03,*(LTextBinNode **)(*(int *)this_03 + 4));
    *(int *)(*(int *)this_03 + 8) = *(int *)this_03;
    *(undefined4 *)(*(int *)this_03 + 4) = 0;
    *(int *)(*(int *)this_03 + 0xc) = *(int *)this_03;
    *(undefined4 *)((int)ppuVar22 + 0x148) = 0;
  }
  ppStack_e0 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  pCStack_dc = (CComponent *)0x0;
  ppStack_f8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  pCStack_f0 = (CComponent *)0x0;
  pCStack_e8 = (CComponent *)0x0;
  ppStack_f4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
  iStack_c0 = 0;
  iVar10 = *(int *)(iStack_8c + 8);
  while (iVar10 != iStack_8c) {
    switch(*(undefined4 *)(iVar10 + 0x10)) {
    case 0:
      ppStack_f4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                   ((*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc);
      break;
    case 1:
      iStack_c0 = (*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc;
      break;
    case 3:
      ppStack_e0 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                   ((*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc);
      break;
    case 4:
      pCStack_dc = (CComponent *)((*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc);
      break;
    case 5:
      ppStack_f8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                   ((*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc);
      break;
    case 6:
      pCStack_f0 = (CComponent *)((*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc);
      break;
    case 7:
      pCStack_e8 = (CComponent *)((*(int *)(iVar10 + 0x18) - *(int *)(iVar10 + 0x14)) / 0xc);
    }
    iVar19 = *(int *)(iVar10 + 0xc);
    if (iVar19 == 0) {
      iVar19 = *(int *)(iVar10 + 4);
      if (iVar10 == *(int *)(iVar19 + 0xc)) {
        do {
          iVar10 = iVar19;
          iVar19 = *(int *)(iVar10 + 4);
        } while (iVar10 == *(int *)(iVar19 + 0xc));
      }
      if (*(int *)(iVar10 + 0xc) != iVar19) {
        iVar10 = iVar19;
      }
    }
    else {
      for (iVar20 = *(int *)(iVar19 + 8); iVar10 = iVar19, iVar20 != 0;
          iVar20 = *(int *)(iVar20 + 8)) {
        iVar19 = iVar20;
      }
    }
  }
  iVar10 = *(int *)((int)ppuStack_d0 + 4);
  ppuVar22 = (undefined **)0x0;
  uVar18 = *(uint *)(iVar10 + 0x20) >> 0x11;
  ppuStack_80 = (undefined **)0x0;
  ppuStack_7c = (undefined **)0x0;
  ppuStack_78 = (undefined **)0x0;
  ppStack_e4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
               (CONCAT13((char)uVar18,ppStack_e4._0_3_) & 0x1ffffff);
  if ((uVar18 & 1) != 0) {
    local_fc = (CFrontEndManager *)0x11;
    piVar9 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar10 + 0x44),(ETCInterfaceType *)&local_fc);
    if ((piVar9 == *(int **)(iVar10 + 0x48)) || (0x11 < *piVar9)) {
      piVar9 = *(int **)(iVar10 + 0x48);
    }
    piStack_b4 = (int *)0x0;
    piStack_b0 = (int *)0x0;
    puStack_ac = (undefined *)0x0;
    NInventory::CTCInventoryBase::GetAllItems
              ((CTCInventoryBase *)piVar9[1],
               (vector<std::pair<long,long>,std::allocator<std::pair<long,long>_>_> *)&piStack_b4);
    piVar9 = piStack_b4;
    if (piStack_b4 != piStack_b0) {
      do {
        bVar6 = CTCInventoryItem::IsAugmentation(*piVar9);
        if (bVar6) {
          if (ppuVar22 == ppuStack_78) {
            FUN_00440990();
            ppuVar22 = ppuStack_7c;
          }
          else {
            if (ppuVar22 != (undefined **)0x0) {
              *ppuVar22 = (undefined *)*piVar9;
            }
            ppuStack_7c = ppuVar22 + 1;
            ppuVar22 = ppuStack_7c;
          }
        }
        piVar9 = piVar9 + 2;
      } while (piVar9 != piStack_b0);
    }
    if (piStack_b4 != (int *)0x0) {
      free(piStack_b4);
    }
    if (ppuStack_80 != ppuVar22) goto LAB_0060782a;
  }
  iVar10 = *(int *)pCVar8;
  CCharString::CCharString((CCharString *)&stack0xfffffed8,"UI_MENU_ENTRY_LOGBOOK_BOOKS",-1);
  piVar9 = (int *)(**(code **)(iVar10 + 0xc))();
  if (piVar9 != (int *)0x0) {
    (**(code **)(*piVar9 + 0x170))();
    iVar10 = *piVar9;
    CCharString::CCharString((CCharString *)&stack0xfffffed8,"UI_SELECTION_ARROW_RIGHT",-1);
    piVar9 = (int *)(**(code **)(iVar10 + 0xc))();
    if (piVar9 != (int *)0x0) {
      pCStack_114 = malloc(0xc);
      *(CComponent **)pCStack_114 = pCStack_114;
      *(CComponent **)(pCStack_114 + 4) = pCStack_114;
      unaff_EBX = (CManager *)0x1;
      pvVar11 = malloc(0xc);
      *(void **)pvVar11 = pvVar11;
      *(void **)((int)pvVar11 + 4) = pvVar11;
      std::list<CClass*,std::allocator<CClass*>_>::
      _Insert<std::_List_const_iterator<std::_List_val<CClass*,std::allocator<CClass*>_>_>_>
                ((list<CClass*,std::allocator<CClass*>_> *)&stack0xfffffed8);
      (**(code **)(*piVar9 + 0xf8))();
      CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&pCStack_114);
      ppuVar22 = ppuStack_7c;
    }
  }
LAB_0060782a:
  CCharString::CCharString((CCharString *)&local_cc,"UI_SUB_LIST",-1);
  CVar29 = (CCharString)0x0;
  pCVar27 = (CCharString *)&local_cc;
  pCVar7 = NUISystem::CFrontEndManager::GetInstance();
  pCStack_a4 = NUISystem::CManager::CreateComponent((CManager *)pCVar7,pCVar27,(bool)CVar29);
  ppVar21 = extraout_EDX_00;
  if ((pCStack_a4 == (CComponent *)0x0) ||
     (pCStack_a0 = ::operator_new(0xc), ppVar21 = extraout_EDX_01, pCStack_a0 == (CComponent *)0x0))
  {
    pCStack_a0 = (CComponent *)0x0;
  }
  else {
    *(undefined4 *)pCStack_a0 = 1;
    *(code **)(pCStack_a0 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
    *(CComponent **)(pCStack_a0 + 8) = pCStack_a4;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_cc,ppVar21,
             ppVar15);
  ppuVar1 = ppuStack_80;
  pCVar8 = (CComponent *)((int)ppuVar22 - (int)ppuStack_80 >> 2);
  pCStack_ec = pCStack_a4;
  (**(code **)(*(int *)pCStack_a4 + 0x270))();
  ppuStack_d0 = ppuVar1;
  if (ppuVar1 != ppuVar22) {
    do {
      puStack_ac = *ppuStack_d0;
      CTCInventoryItem::GetDescription((long)aCStack_9c);
      local_cc = (CTCInventoryMap *)0x0;
      piStack_c8 = (int *)0x0;
      CCharString::CCharString(aCStack_6c,"UI_WEAPON_MENU_ENTRY_TEMPLATE",-1);
      CCharString::CCharString((CCharString *)&stack0xfffffed4,"PC_BUTTON_BASTARD",-1);
      CCharString::CCharString((CCharString *)&stack0xfffffecc,"",-1);
      pCVar5 = pCStack_104;
      pCVar12 = NUISystem::CManager::CreateMenuEntryComponent(pCStack_104);
      ppVar21 = extraout_EDX_02;
      ppVar23 = ppVar15;
      if ((piStack_c8 != (int *)0x0) && (*piStack_c8 = *piStack_c8 + -1, *piStack_c8 == 0)) {
        (*(code *)piStack_c8[1])();
        operator_delete(piStack_c8);
        ppVar21 = extraout_EDX_03;
        ppVar23 = ppVar15;
      }
      piStack_c8 = (int *)0x0;
      local_cc = (CTCInventoryMap *)pCVar12;
      if (pCVar12 != (CComponent *)0x0) {
        piStack_c8 = ::operator_new(0xc);
        ppVar21 = extraout_EDX_04;
        if (piStack_c8 == (int *)0x0) {
          piStack_c8 = (int *)0x0;
        }
        else {
          *piStack_c8 = 1;
          piStack_c8[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_c8[2] = (int)local_cc;
        }
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_6c,ppVar21
                 ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      CCharString::CCharString(aCStack_58,"UI_ITEM_MODEL_TEMPLATE_NEW",-1);
      pCVar12 = NUISystem::CManager::CreateComponent(pCVar5,aCStack_58,false);
      ppVar21 = extraout_EDX_05;
      if ((pCVar12 == (CComponent *)0x0) ||
         (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_06, piVar9 == (int *)0x0)) {
        piVar9 = (int *)0x0;
      }
      else {
        *piVar9 = 1;
        piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
        piVar9[2] = (int)pCVar12;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_58,ppVar21
                 ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      piStack_b4 = (int *)0x0;
      pCVar16 = (CComponent *)0x0;
      if (piVar9 != (int *)0x0) {
        *piVar9 = *piVar9 + 1;
        pCVar16 = pCVar12;
        piStack_b4 = piVar9;
      }
      pCStack_100 = (CComponent *)0x0;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (paStack_d4,extraout_EDX_07,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      this_02 = (CDefClassBase *)0x0;
      if ((0 < (int)puStack_ac) &&
         (CDefinitionManager::GetPDefFromGlobalIndex(this_01,(long)&local_98), pCVar12 = local_98,
         local_98 != (CComponent *)0x0)) {
        *(int *)(local_98 + 4) = *(int *)(local_98 + 4) + 1;
        iVar10 = *(int *)(local_98 + 4);
        pCStack_100 = local_98;
        *(int *)(local_98 + 4) = iVar10 + -1;
        if (iVar10 + -1 == 0) {
          (**(code **)(*(int *)local_98 + 4))();
        }
        local_98 = (CComponent *)0x0;
        this_02 = (CDefClassBase *)pCVar12;
      }
      (**(code **)(*(int *)pCVar16 + 0x23c))();
      CTCInventoryItem::GetStartingRightHandedSet((long)apiStack_48);
      (**(code **)(*(int *)pCVar16 + 0x250))();
      if (this_02[0x7d] == (CDefClassBase)0x4) {
        ppuStack_80 = (undefined **)0x0;
        CDefClassBase::GetSubDef<CAbilityDef>
                  (this_02,(CDefPointer<CAbilityDef_const_> *)&ppuStack_80);
        CCharString::CCharString((CCharString *)&ppuStack_7c,"DEFAULT",-1);
        ppuVar22 = ppuStack_80;
        ppuStack_78 = &PTR_LAB_0122ffdc;
        puVar13 = (undefined4 *)FUN_005dc340();
        ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)*puVar13;
        (**(code **)(*(int *)pCVar16 + 0x248))();
        ppuStack_7c = &PTR_LAB_0122ffcc;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppuStack_80,
                   extraout_EDX_08,ppVar15);
        (**(code **)(*(int *)pCVar16 + 0x24c))();
        if (ppuVar22 != (undefined **)0x0) {
          ppuVar1 = ppuVar22 + 1;
          *ppuVar1 = *ppuVar1 + -1;
          if (*ppuVar1 == (undefined *)0x0) {
            (**(code **)(*ppuVar22 + 4))();
          }
        }
      }
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffedc;
      (**(code **)(*(int *)paStack_d4 + 0xec))();
      CCharString::CCharString(aCStack_60,"UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW",-1);
      pCVar5 = pCStack_110;
      pCStack_e8 = NUISystem::CManager::CreateComponent(pCStack_110,aCStack_60,false);
      ppVar21 = extraout_EDX_09;
      if ((pCStack_e8 == (CComponent *)0x0) ||
         (ppStack_e4 = ::operator_new(0xc), ppVar21 = extraout_EDX_10,
         ppStack_e4 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
        ppStack_e4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
      }
      else {
        *(int *)ppStack_e4 = 1;
        *(code **)(ppStack_e4 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
        *(CComponent **)(ppStack_e4 + 8) = pCStack_e8;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_60,ppVar21
                 ,ppVar15);
      local_54 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
      pCVar12 = (CComponent *)0x0;
      if (ppStack_e4 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
        local_54 = ppStack_e4;
        *(int *)ppStack_e4 = *(int *)ppStack_e4 + 1;
        pCVar12 = pCStack_e8;
      }
      iVar10 = *(int *)pCVar12;
      piVar26 = (int *)0x1;
      CTCInventoryItem::GetDescription((long)&pCStack_a0);
      (**(code **)(iVar10 + 0x240))();
      CCharString::~CCharString((CCharString *)&puStack_ac);
      pppVar25 = &ppStack_f4;
      (**(code **)(*(int *)ppStack_e4 + 0xec))();
      iVar10 = *(int *)param_1;
      ppVar15 = extraout_ECX_00;
      CCharString::CCharString((CCharString *)&stack0xfffffeb8,"UI_SCROLLING_VIEWPORT_TEXT_AREA",-1)
      ;
      piVar14 = (int *)(**(code **)(iVar10 + 0xc))();
      if (piVar14 != (int *)0x0) {
        (**(code **)(*piVar14 + 0x238))(local_fc);
      }
      piVar14 = (int *)CTCInventoryItem::GetEngineGraphic((long)auStack_3e);
      if (*piVar14 == 0) {
        CCharString::CCharString((CCharString *)&pvStack_84,"UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW",-1);
        pCVar12 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pvStack_84,false);
        ppVar21 = extraout_EDX_15;
        if ((pCVar12 == (CComponent *)0x0) ||
           (piVar14 = ::operator_new(0xc), ppVar21 = extraout_EDX_16, piVar14 == (int *)0x0)) {
          piVar14 = (int *)0x0;
        }
        else {
          *piVar14 = 1;
          piVar14[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piVar14[2] = (int)pCVar12;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pvStack_84,
                   ppVar21,ppVar15);
        pCVar16 = (CComponent *)0x0;
        piVar26 = (int *)0x0;
        if (piVar14 != (int *)0x0) {
          *piVar14 = *piVar14 + 1;
          pCVar16 = pCVar12;
          piVar26 = piVar14;
        }
        (**(code **)(*(int *)pCVar16 + 0x240))(auStack_bc,1,1);
        (**(code **)(*(int *)ppStack_f8 + 0xec))(&stack0xfffffec0);
        if ((piVar26 != (int *)0x0) && (*piVar26 = *piVar26 + -1, *piVar26 == 0)) {
          (*(code *)piVar26[1])();
          operator_delete(piVar26);
        }
        if ((pppVar25 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)0x0) &&
           (*pppVar25 = *pppVar25 + -1,
           *pppVar25 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
          (*(code *)pppVar25[1])();
          operator_delete(pppVar25);
        }
      }
      else {
        CCharString::CCharString((CCharString *)&ppuStack_80,"UI_TEXT_ITEM_NAME_TEMPLATE_NEW",-1);
        pCVar12 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&ppuStack_80,false);
        ppVar21 = extraout_EDX_11;
        if ((pCVar12 == (CComponent *)0x0) ||
           (pCVar8 = ::operator_new(0xc), ppVar21 = extraout_EDX_12, pCVar8 == (CComponent *)0x0)) {
          pCVar8 = (CComponent *)0x0;
        }
        else {
          *(int *)pCVar8 = 1;
          *(code **)(pCVar8 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCVar8 + 8) = pCVar12;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppuStack_80,
                   ppVar21,ppVar15);
        pCVar16 = (CComponent *)0x0;
        pCVar24 = (CComponent *)0x0;
        if (pCVar8 != (CComponent *)0x0) {
          *(int *)pCVar8 = *(int *)pCVar8 + 1;
          pCVar16 = pCVar12;
          pCVar24 = pCVar8;
        }
        (**(code **)(*(int *)pCVar16 + 0x240))(auStack_bc,1,1);
        ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffec8;
        (**(code **)(*(int *)ppStack_f8 + 0xec))();
        CCharString::CCharString((CCharString *)&local_98,"UI_ITEM_SPRITE_TEMPLATE",-1);
        pCStack_f0 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&local_98,false);
        ppVar21 = extraout_EDX_13;
        if ((pCStack_f0 == (CComponent *)0x0) ||
           (pCStack_ec = ::operator_new(0xc), ppVar21 = extraout_EDX_14,
           pCStack_ec == (CComponent *)0x0)) {
          pCStack_ec = (CComponent *)0x0;
        }
        else {
          *(int *)pCStack_ec = 1;
          *(code **)(pCStack_ec + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCStack_ec + 8) = pCStack_f0;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_98,
                   ppVar21,ppVar15);
        iVar10 = (**(code **)(*(int *)pCStack_f0 + 0x104))();
        if (iVar10 == 0) {
          puVar13 = (undefined4 *)CTCInventoryItem::GetEngineGraphic((long)auStack_5c);
          (**(code **)(*(int *)pCStack_f0 + 0x244))(*puVar13);
        }
        (**(code **)(*(int *)local_fc + 0xec))(&pCStack_f0);
        if ((pCStack_f0 != (CComponent *)0x0) &&
           (*(int *)pCStack_f0 = *(int *)pCStack_f0 + -1, *(int *)pCStack_f0 == 0)) {
          (**(code **)(pCStack_f0 + 4))();
          operator_delete(pCStack_f0);
        }
        ppStack_f4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
        pCStack_f0 = (CComponent *)0x0;
        if ((pCVar24 != (CComponent *)0x0) &&
           (*(int *)pCVar24 = *(int *)pCVar24 + -1, *(int *)pCVar24 == 0)) {
          (**(code **)(pCVar24 + 4))();
          operator_delete(pCVar24);
        }
        if ((piVar26 != (int *)0x0) && (*piVar26 = *piVar26 + -1, *piVar26 == 0)) {
          (*(code *)piVar26[1])();
          operator_delete(piVar26);
        }
      }
      ppVar15 = ppVar23;
      (**(code **)(*(int *)ppVar23 + 0x260))(&pCStack_100);
      piVar14 = apiStack_48[0];
      if ((apiStack_48[0] != (int *)0x0) &&
         (*apiStack_48[0] = *apiStack_48[0] + -1, *apiStack_48[0] == 0)) {
        (*(code *)apiStack_48[0][1])();
        operator_delete(piVar14);
      }
      if ((piStack_d8 != (int *)0x0) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 == 0)) {
        (*(code *)piStack_d8[1])();
        operator_delete(piStack_d8);
      }
      pCStack_dc = (CComponent *)0x0;
      piStack_d8 = (int *)0x0;
      pCVar12 = pCStack_100 + 4;
      *(int *)pCVar12 = *(int *)pCVar12 + -1;
      if (*(int *)pCVar12 == 0) {
        (**(code **)(*(int *)pCStack_100 + 4))();
      }
      piVar14 = piStack_b4;
      if ((piStack_b4 != (int *)0x0) && (*piStack_b4 = *piStack_b4 + -1, *piStack_b4 == 0)) {
        (*(code *)piStack_b4[1])();
        operator_delete(piVar14);
      }
      if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
        (*(code *)piVar9[1])();
        operator_delete(piVar9);
      }
      if ((piStack_c8 != (int *)0x0) && (*piStack_c8 = *piStack_c8 + -1, *piStack_c8 == 0)) {
        (*(code *)piStack_c8[1])();
        operator_delete(piStack_c8);
      }
      local_cc = (CTCInventoryMap *)0x0;
      piStack_c8 = (int *)0x0;
      CCharString::~CCharString(aCStack_9c);
      ppuStack_d0 = ppuStack_d0 + 1;
    } while (ppuStack_d0 != ppuStack_80);
    CCharString::CCharString((CCharString *)&pCStack_100,"UI_BOOKS_CATEGORIES_TITLE",-1);
    pCVar5 = pCStack_104;
    pCStack_114 = NUISystem::CManager::CreateComponent
                            (pCStack_104,(CCharString *)&pCStack_100,false);
    ppVar21 = extraout_EDX_17;
    if ((pCStack_114 == (CComponent *)0x0) ||
       (pCStack_110 = ::operator_new(0xc), ppVar21 = extraout_EDX_18, pCStack_110 == (CManager *)0x0
       )) {
      pCStack_110 = (CManager *)0x0;
    }
    else {
      *(int *)pCStack_110 = 1;
      *(code **)(pCStack_110 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(pCStack_110 + 8) = pCStack_114;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_100,ppVar21
               ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    ppVar28 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_114;
    (**(code **)(*(int *)ppVar23 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_104,"UI_BACK_FOR_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_104,false);
    ppVar21 = extraout_EDX_19;
    if ((pCVar12 != (CComponent *)0x0) &&
       (puVar13 = ::operator_new(0xc), ppVar21 = extraout_EDX_20, puVar13 != (undefined4 *)0x0)) {
      *puVar13 = 1;
      puVar13[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
      puVar13[2] = pCVar12;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_104,ppVar21
               ,ppVar28);
    (**(code **)(*(int *)ppVar23 + 0x244))();
    iVar10 = *piStack_a8;
    CCharString::CCharString((CCharString *)&stack0xfffffecc,"UI_MENU_ENTRY_LOGBOOK_BOOKS",-1);
    piVar9 = (int *)(**(code **)(iVar10 + 0xc))();
    if (piVar9 != (int *)0x0) {
      (**(code **)(*piVar9 + 0xec))();
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
  }
  ppVar23 = ppStack_e4;
  if (ppStack_e4 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    CCharString::CCharString((CCharString *)&ppStack_e4,"UI_SUB_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&ppStack_e4,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_e4,
               extraout_EDX_21,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar12 + 0x270))();
    CCharString::CCharString((CCharString *)&pCStack_104,"UI_BASICS_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent
                            (pCStack_108,(CCharString *)&pCStack_104,false);
    ppVar21 = extraout_EDX_22;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_23, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_104,ppVar21
               ,ppVar23);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_108,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&pCStack_108,false);
    ppVar21 = extraout_EDX_24;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_25, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_108,ppVar21
               ,ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_10c,"UI_RING_PIC_LOGBOOK_BASICS_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_10c,false);
    ppVar21 = extraout_EDX_26;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_27,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_10c,ppVar21
               ,ppVar23);
    (**(code **)(*(int *)pCVar12 + 0x244))();
    pCStack_f0 = (CComponent *)0x3;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar12;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  ppVar23 = ppStack_e0;
  if (ppStack_e0 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    CCharString::CCharString((CCharString *)&ppStack_e0,"UI_SUB_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&ppStack_e0,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_e0,
               extraout_EDX_28,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar12 + 0x270))();
    CCharString::CCharString((CCharString *)&pCStack_e8,"UI_OBJECTS_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent(pCStack_108,(CCharString *)&pCStack_e8,false)
    ;
    ppVar21 = extraout_EDX_29;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_30, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_e8,ppVar21,
               ppVar23);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_ec,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&pCStack_ec,false);
    ppVar21 = extraout_EDX_31;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_32, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_ec,ppVar21,
               ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_f0,"UI_RING_PIC_LOGBOOK_OBJECTS_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_f0,false);
    ppVar21 = extraout_EDX_33;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_34,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_f0,ppVar21,
               ppVar23);
    (**(code **)(*(int *)pCVar12 + 0x244))();
    pCStack_f0 = (CComponent *)0x4;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar12;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  pCVar7 = local_fc;
  if (local_fc != (CFrontEndManager *)0x0) {
    CCharString::CCharString((CCharString *)&local_fc,"UI_SUB_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&local_fc,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_fc,
               extraout_EDX_35,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar12 + 0x270))();
    CCharString::CCharString((CCharString *)&ppStack_e4,"UI_TOWNS_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent(pCStack_108,(CCharString *)&ppStack_e4,false)
    ;
    ppVar21 = extraout_EDX_36;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_37, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_e4,ppVar21,
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar7);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_e8,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&pCStack_e8,false);
    ppVar21 = extraout_EDX_38;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_39, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_e8,ppVar21,
               ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_ec,"UI_RING_PIC_LOGBOOK_TOWNS_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_ec,false);
    ppVar21 = extraout_EDX_40;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_41,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_ec,ppVar21,
               ppVar23);
    (**(code **)(*(int *)pCVar12 + 0x244))();
    pCStack_f0 = (CComponent *)0x5;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar12;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  ppVar23 = ppStack_f4;
  if (ppStack_f4 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    CCharString::CCharString((CCharString *)&ppStack_f4,"UI_SUB_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&ppStack_f4,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_f4,
               extraout_EDX_42,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar12 + 0x270))();
    CCharString::CCharString((CCharString *)&pCStack_100,"UI_HERO_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent
                            (pCStack_108,(CCharString *)&pCStack_100,false);
    ppVar21 = extraout_EDX_43;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_44, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_100,ppVar21
               ,ppVar23);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_104,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&pCStack_104,false);
    ppVar21 = extraout_EDX_45;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_46, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_104,ppVar21
               ,ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_108,"UI_RING_PIC_LOGBOOK_HERO_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_108,false);
    ppVar21 = extraout_EDX_47;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_48,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_108,ppVar21
               ,ppVar23);
    (**(code **)(*(int *)pCVar12 + 0x244))();
    pCStack_f0 = (CComponent *)0x6;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar12;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  pCVar12 = pCStack_ec;
  if (pCStack_ec != (CComponent *)0x0) {
    CCharString::CCharString((CCharString *)&pCStack_ec,"UI_SUB_LIST",-1);
    pCVar16 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&pCStack_ec,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_ec,
               extraout_EDX_49,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar16 + 0x270))();
    CCharString::CCharString((CCharString *)&ppStack_f8,"UI_COMBAT_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent(pCStack_108,(CCharString *)&ppStack_f8,false)
    ;
    ppVar21 = extraout_EDX_50;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_51, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_f8,ppVar21,
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar12);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar16 + 0x244))();
    CCharString::CCharString((CCharString *)&local_fc,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&local_fc,false);
    ppVar21 = extraout_EDX_52;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_53, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_fc,ppVar21,
               ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar16 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_100,"UI_RING_PIC_LOGBOOK_COMBAT_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_100,false);
    ppVar21 = extraout_EDX_54;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_55,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_100,ppVar21
               ,ppVar23);
    (**(code **)(*(int *)pCVar16 + 0x244))();
    pCStack_f0 = (CComponent *)0x7;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar16;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  ppVar23 = ppStack_f8;
  if (ppStack_f8 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    CCharString::CCharString((CCharString *)&ppStack_f8,"UI_SUB_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&ppStack_f8,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_f8,
               extraout_EDX_56,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar12 + 0x270))();
    CCharString::CCharString((CCharString *)&pCStack_f0,"UI_QUESTS_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent(pCStack_108,(CCharString *)&pCStack_f0,false)
    ;
    ppVar21 = extraout_EDX_57;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_58, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_f0,ppVar21,
               ppVar23);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&ppStack_f4,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&ppStack_f4,false);
    ppVar21 = extraout_EDX_59;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_60, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_f4,ppVar21,
               ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&ppStack_f8,"UI_RING_PIC_LOGBOOK_QUESTS_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&ppStack_f8,false);
    ppVar21 = extraout_EDX_61;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_62,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_f8,ppVar21,
               ppVar23);
    (**(code **)(*(int *)pCVar12 + 0x244))();
    pCStack_f0 = (CComponent *)0x0;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar12;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  if (ppStack_c4 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
    CCharString::CCharString((CCharString *)&ppStack_c4,"UI_SUB_LIST",-1);
    pCVar12 = NUISystem::CManager::CreateComponent(pCStack_104,(CCharString *)&ppStack_c4,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_c4,
               extraout_EDX_63,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    (**(code **)(*(int *)pCVar12 + 0x270))();
    CCharString::CCharString((CCharString *)&local_fc,"UI_STORY_CATEGORIES_TITLE",-1);
    pCStack_110 = (CManager *)
                  NUISystem::CManager::CreateComponent(pCStack_108,(CCharString *)&local_fc,false);
    ppVar21 = extraout_EDX_64;
    if ((pCStack_110 == (CManager *)0x0) ||
       (pCStack_10c = ::operator_new(0xc), ppVar21 = extraout_EDX_65, pCStack_10c == (CManager *)0x0
       )) {
      pCStack_10c = (CManager *)0x0;
    }
    else {
      *(undefined4 *)pCStack_10c = 1;
      *(code **)(pCStack_10c + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CManager **)(pCStack_10c + 8) = pCStack_110;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_fc,ppVar21,
               ppStack_c4);
    ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_110;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_100,"UI_BACK_FOR_LIST",-1);
    pCVar5 = pCStack_10c;
    pCVar8 = NUISystem::CManager::CreateComponent(pCStack_10c,(CCharString *)&pCStack_100,false);
    ppVar21 = extraout_EDX_66;
    if ((pCVar8 == (CComponent *)0x0) ||
       (piVar9 = ::operator_new(0xc), ppVar21 = extraout_EDX_67, piVar9 == (int *)0x0)) {
      piVar9 = (int *)0x0;
    }
    else {
      *piVar9 = 1;
      piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piVar9[2] = (int)pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_100,ppVar21
               ,ppVar15);
    ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffee4;
    (**(code **)(*(int *)pCVar12 + 0x244))();
    CCharString::CCharString((CCharString *)&pCStack_104,"UI_RING_PIC_LOGBOOK_STORY_FOR_LIST",-1);
    pCVar8 = NUISystem::CManager::CreateComponent(pCVar5,(CCharString *)&pCStack_104,false);
    ppVar21 = extraout_EDX_68;
    if ((pCVar8 == (CComponent *)0x0) ||
       (ppVar15 = ::operator_new(0xc), ppVar21 = extraout_EDX_69,
       ppVar15 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0)) {
      ppVar15 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
    }
    else {
      *(int *)ppVar15 = 1;
      *(code **)(ppVar15 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
      *(CComponent **)(ppVar15 + 8) = pCVar8;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_104,ppVar21
               ,ppVar23);
    (**(code **)(*(int *)pCVar12 + 0x244))();
    pCStack_f0 = (CComponent *)0x1;
    puVar13 = (undefined4 *)FUN_00618be0();
    *puVar13 = pCVar12;
    if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
      (*(code *)piVar9[1])();
      operator_delete(piVar9);
    }
    unaff_EBX = (CManager *)0x0;
    if ((pCStack_110 != (CManager *)0x0) &&
       (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
      (**(code **)(pCStack_110 + 4))();
      operator_delete(pCStack_110);
    }
    pCStack_114 = (CComponent *)0x0;
    pCStack_110 = (CManager *)0x0;
    if ((pCStack_108 != (CManager *)0x0) &&
       (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
      (**(code **)(pCStack_108 + 4))();
      operator_delete(pCStack_108);
    }
  }
  pvVar11 = *(void **)((int)pvStack_90 + 8);
  if (pvVar11 != pvStack_90) {
    do {
      puVar13 = (undefined4 *)FUN_00618be0();
      piStack_94 = (int *)*puVar13;
      pCStack_f0 = (CComponent *)*piStack_94;
      (**(code **)(pCStack_f0 + 0x278))();
      iVar19 = *(int *)((int)pvVar11 + 0x18) - *(int *)((int)pvVar11 + 0x14);
      iVar10 = iVar19 >> 0x1f;
      local_fc = (CFrontEndManager *)0x0;
      if (iVar19 / 0xc + iVar10 != iVar10) {
        ppuStack_d0 = (undefined **)0x0;
        do {
          pCVar2 = (CWideString *)((int)ppuStack_d0 + *(int *)((int)pvVar11 + 0x14));
          CWideString::CWideString((CWideString *)&piStack_b8,pCVar2);
          CWideString::CWideString((CWideString *)&piStack_b4,pCVar2 + 4);
          CWideString::CWideString((CWideString *)&piStack_b0,pCVar2 + 8);
          CWideString::CWideString((CWideString *)&ppStack_f4,(CWideString *)&piStack_b4);
          CCharString::CCharString
                    ((CCharString *)&ppStack_c4,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",-1);
          CCharString::CCharString((CCharString *)&stack0xfffffed4,"PC_BUTTON_BASTARD",-1);
          CCharString::CCharString((CCharString *)&stack0xfffffecc,"",-1);
          pCStack_114 = NUISystem::CManager::CreateMenuEntryComponent(pCStack_104);
          ppVar21 = extraout_EDX_70;
          if ((pCStack_114 == (CComponent *)0x0) ||
             (pCStack_110 = ::operator_new(0xc), ppVar21 = extraout_EDX_71,
             pCStack_110 == (CManager *)0x0)) {
            pCStack_110 = (CManager *)0x0;
          }
          else {
            *(int *)pCStack_110 = 1;
            *(code **)((int)pCStack_110 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
            *(CComponent **)((int)pCStack_110 + 8) = pCStack_114;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_c4,
                     ppVar21,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
          CCharString::CCharString
                    ((CCharString *)&ppStack_f8,"UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW",-1);
          pCStack_dc = NUISystem::CManager::CreateComponent
                                 (pCStack_104,(CCharString *)&ppStack_f8,false);
          ppVar21 = extraout_EDX_72;
          if ((pCStack_dc == (CComponent *)0x0) ||
             (piStack_d8 = ::operator_new(0xc), ppVar21 = extraout_EDX_73, piStack_d8 == (int *)0x0)
             ) {
            piStack_d8 = (int *)0x0;
          }
          else {
            *piStack_d8 = 1;
            piStack_d8[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
            piStack_d8[2] = (int)pCStack_dc;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&ppStack_f8,
                     ppVar21,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
          pCStack_108 = (CManager *)0x0;
          (**(code **)(*piStack_50 + 0x238))();
          ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
          if (pCStack_dc != (CComponent *)0x0) {
            pCStack_10c = (CManager *)pCStack_dc;
            *(int *)pCStack_dc = *(int *)pCStack_dc + 1;
            ppVar23 = ppStack_e0;
          }
          piVar9 = (int *)0x1;
          (**(code **)(*(int *)ppVar23 + 0x240))();
          ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_ec;
          (**(code **)(*(int *)ppVar15 + 0xec))();
          CCharString::CCharString
                    ((CCharString *)&pCStack_100,"UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW",-1);
          pCVar12 = NUISystem::CManager::CreateComponent
                              (unaff_EBX,(CCharString *)&pCStack_100,false);
          ppVar21 = extraout_EDX_74;
          if ((pCVar12 == (CComponent *)0x0) ||
             (piVar14 = ::operator_new(0xc), ppVar21 = extraout_EDX_75, piVar14 == (int *)0x0)) {
            piVar14 = (int *)0x0;
          }
          else {
            *piVar14 = 1;
            piVar14[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
            piVar14[2] = (int)pCVar12;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_100,
                     ppVar21,ppVar23);
          pCVar16 = (CComponent *)0x0;
          piVar26 = (int *)0x0;
          if (piVar14 != (int *)0x0) {
            *piVar14 = *piVar14 + 1;
            pCVar16 = pCVar12;
            piVar26 = piVar14;
          }
          (**(code **)(*(int *)pCVar16 + 0x240))();
          (**(code **)(*piVar9 + 0xec))(&stack0xfffffec4);
          (**(code **)(*piStack_b8 + 0x260))(&stack0xfffffec8);
          if ((piVar26 != (int *)0x0) && (*piVar26 = *piVar26 + -1, *piVar26 == 0)) {
            (*(code *)piVar26[1])();
            operator_delete(piVar26);
          }
          if ((unaff_EBX != (CManager *)0x0) &&
             (*(int *)unaff_EBX = *(int *)unaff_EBX + -1, *(int *)unaff_EBX == 0)) {
            (**(code **)(unaff_EBX + 4))();
            operator_delete(unaff_EBX);
          }
          pCVar5 = pCStack_108;
          unaff_EBX = (CManager *)0x0;
          if ((pCStack_108 != (CManager *)0x0) &&
             (*(int *)pCStack_108 = *(int *)pCStack_108 + -1, *(int *)pCStack_108 == 0)) {
            (**(code **)(pCStack_108 + 4))();
            operator_delete(pCVar5);
          }
          if ((piStack_d8 != (int *)0x0) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 == 0)) {
            (*(code *)piStack_d8[1])();
            operator_delete(piStack_d8);
          }
          pCStack_dc = (CComponent *)0x0;
          piStack_d8 = (int *)0x0;
          if ((pCStack_110 != (CManager *)0x0) &&
             (*(int *)pCStack_110 = *(int *)pCStack_110 + -1, *(int *)pCStack_110 == 0)) {
            (**(code **)((int)pCStack_110 + 4))();
            operator_delete(pCStack_110);
          }
          pCStack_114 = (CComponent *)0x0;
          pCStack_110 = (CManager *)0x0;
          CCharString::~CCharString((CCharString *)&ppStack_f4);
          CCharString::~CCharString((CCharString *)&piStack_b0);
          CCharString::~CCharString((CCharString *)&piStack_b4);
          CCharString::~CCharString((CCharString *)&piStack_b8);
          ppuStack_d0 = ppuStack_d0 + 3;
          local_fc = local_fc + 1;
        } while (local_fc <
                 (CFrontEndManager *)
                 ((*(int *)((int)pvVar11 + 0x18) - *(int *)((int)pvVar11 + 0x14)) / 0xc));
      }
      pvVar17 = *(void **)((int)pvVar11 + 0xc);
      if (pvVar17 == (void *)0x0) {
        pvVar17 = *(void **)((int)pvVar11 + 4);
        if (pvVar11 == *(void **)((int)pvVar17 + 0xc)) {
          do {
            pvVar11 = pvVar17;
            pvVar17 = *(void **)((int)pvVar11 + 4);
          } while (pvVar11 == *(void **)((int)pvVar17 + 0xc));
        }
        if (*(void **)((int)pvVar11 + 0xc) != pvVar17) {
          pvVar11 = pvVar17;
        }
      }
      else {
        for (pvVar4 = *(void **)((int)pvVar17 + 8); pvVar11 = pvVar17, pvVar4 != (void *)0x0;
            pvVar4 = *(void **)((int)pvVar4 + 8)) {
          pvVar17 = pvVar4;
        }
      }
    } while (pvVar11 != pvStack_90);
  }
  pCStack_104 = (CManager *)0x0;
  do {
    this_00 = paStack_d4;
    pCVar5 = pCStack_104;
    if (pCStack_104 != (CManager *)0x2) {
      iVar10 = *(int *)(paStack_d4 + 0x144);
      iVar20 = iVar10;
      iVar19 = *(int *)(iVar10 + 4);
      while (iVar19 != 0) {
        if (*(int *)(iVar19 + 0x10) < (int)pCStack_104) {
          iVar19 = *(int *)(iVar19 + 0xc);
        }
        else {
          iVar20 = iVar19;
          iVar19 = *(int *)(iVar19 + 8);
        }
      }
      if ((iVar20 == iVar10) || ((int)pCStack_104 < *(int *)(iVar20 + 0x10))) {
        iVar20 = iVar10;
      }
      if (iVar20 == *(int *)(paStack_d4 + 0x144)) {
        pCVar8 = GetMenuEntryFromCategory
                           ((CTCInventoryMap *)paStack_d4,(ECategory)pCStack_104,pCStack_a0);
        (**(code **)(*(int *)pCVar8 + 0x170))();
        iVar10 = *(int *)pCVar8;
        CCharString::CCharString((CCharString *)&stack0xfffffed4,"UI_SELECTION_ARROW_RIGHT",-1);
        piVar9 = (int *)(**(code **)(iVar10 + 0xc))();
        if (piVar9 != (int *)0x0) {
          _Memory = malloc(0xc);
          *_Memory = _Memory;
          _Memory[1] = _Memory;
          pvVar11 = malloc(0xc);
          *(void **)pvVar11 = pvVar11;
          *(void **)((int)pvVar11 + 4) = pvVar11;
          std::list<CClass*,std::allocator<CClass*>_>::
          _Insert<std::_List_const_iterator<std::_List_val<CClass*,std::allocator<CClass*>_>_>_>
                    ((list<CClass*,std::allocator<CClass*>_> *)&stack0xfffffed4);
          (**(code **)(*piVar9 + 0xf8))();
          puVar13 = (undefined4 *)*_Memory;
          while (puVar13 != _Memory) {
            puVar3 = (undefined4 *)*puVar13;
            free(puVar13);
            puVar13 = puVar3;
          }
          *_Memory = _Memory;
          _Memory[1] = _Memory;
          free(_Memory);
        }
      }
      else {
        iVar10 = *(int *)(iVar20 + 0x14);
        if ((iVar10 == 0) || (piVar9 = ::operator_new(0xc), piVar9 == (int *)0x0)) {
          piVar9 = (int *)0x0;
        }
        else {
          *piVar9 = 1;
          piVar9[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piVar9[2] = iVar10;
        }
        pCVar8 = GetMenuEntryFromCategory((CTCInventoryMap *)this_00,(ECategory)pCVar5,pCStack_a0);
        (**(code **)(*(int *)pCVar8 + 0xec))();
        if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
          (*(code *)piVar9[1])();
          operator_delete(piVar9);
        }
      }
    }
    pCStack_104 = (CManager *)((int)pCStack_104 + 1);
  } while ((int)pCStack_104 < 8);
  if ((pCStack_a4 != (CComponent *)0x0) &&
     (*(int *)pCStack_a4 = *(int *)pCStack_a4 + -1, *(int *)pCStack_a4 == 0)) {
    (**(code **)(pCStack_a4 + 4))();
    operator_delete(pCStack_a4);
  }
  piStack_a8 = (int *)0x0;
  pCStack_a4 = (CComponent *)0x0;
  if (pvStack_84 != (void *)0x0) {
    free(pvStack_84);
  }
  if (iStack_8c != 0) {
    FUN_0047c6a0();
    *(void **)((int)pvStack_90 + 8) = pvStack_90;
    *(undefined4 *)((int)pvStack_90 + 4) = 0;
    *(void **)((int)pvStack_90 + 0xc) = pvStack_90;
    iStack_8c = 0;
  }
  if (pvStack_90 != (void *)0x0) {
    free(pvStack_90);
  }
  return pCStack_a0;
}



//=== ConstructQuestList @ 0061b610 ===

/* WARNING: Removing unreachable block (ram,0x0061c248) */
/* WARNING: Removing unreachable block (ram,0x0061c252) */
/* WARNING: Removing unreachable block (ram,0x0061c330) */
/* WARNING: Removing unreachable block (ram,0x0061c33a) */
/* WARNING: Removing unreachable block (ram,0x0061c130) */
/* WARNING: Removing unreachable block (ram,0x0061c13a) */
/* [bsim sim=0.5349585406518336 <- ego_r]
   public: void __thiscall CTCInventoryQuests::ConstructQuestList(class std::vector<class CThing
   *,class std::allocator<class CThing *> > const &,class NUISystem::CList *,enum
   QuestScreenType,class NUISystem::CList *) */

void __thiscall
CTCInventoryQuests::ConstructQuestList
          (CTCInventoryQuests *this,vector<CThing*,std::allocator<CThing*>_> *param_1,CList *param_2
          ,QuestScreenType param_3,CList *param_4)

{
  int *piVar1;
  ushort uVar2;
  CBaseIntelligentPointer *this_00;
  CManager *pCVar3;
  undefined4 *puVar4;
  CQuestManager *pCVar5;
  CTCInventoryQuests *pCVar6;
  CAIStateGroup_SummonerCharge *pCVar7;
  CRegion *pCVar8;
  bool bVar9;
  undefined4 **ppuVar10;
  CFrontEndManager *pCVar11;
  int *piVar12;
  COptimisedPrimitive *this_01;
  long lVar13;
  CDefinitionManager *this_02;
  undefined4 *puVar14;
  void *pvVar15;
  CWorldMap *pCVar16;
  CBaseObject *pCVar17;
  CComponent *pCVar18;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined3 extraout_var_01;
  CWideString *pCVar19;
  CCharString *pCVar20;
  char *pcVar21;
  CComponent *pCVar22;
  int *piVar23;
  CDataBank *pCVar24;
  CRumbleDef *pCVar25;
  CAction *pCVar26;
  int *piVar27;
  undefined4 *_Memory;
  CDisplayEngine *pCVar28;
  CTCInventoryBase *pCVar29;
  CCategory *pCVar30;
  int iVar31;
  uint uVar32;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar33;
  CTCInventoryMap *this_03;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar34;
  int *extraout_ECX_00;
  int extraout_ECX_01;
  CQuestManager *pCVar35;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_26;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_27;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_28;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_31;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_32;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_37;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_38;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_39;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_40;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_41;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_42;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_43;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_44;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_45;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_46;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_47;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_48;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_49;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_50;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_51;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_52;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_53;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_54;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_55;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_56;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_57;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_58;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_59;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_60;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_61;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_62;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_63;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_64;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_65;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_66;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_67;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_68;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_69;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_70;
  CManager *unaff_EBX;
  CDataBank *pCVar37;
  int iVar38;
  CAIStateGroup_SummonerCharge *this_04;
  undefined1 *puVar39;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CQuestManager *pCVar40;
  int *piVar41;
  int iVar42;
  int iVar43;
  char cVar44;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar45;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar46;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar47;
  CDataBank *this_05;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *this_06;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar48;
  CDataBank *this_07;
  int *piVar49;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *this_08;
  CCharString *pCStack_478;
  CTCQuestCard *pCVar50;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar51;
  int *piVar52;
  int *piVar53;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_428;
  CManager *pCStack_424;
  CTCQuestCard *pCVar54;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar55;
  undefined4 uStack_40c;
  CManager *local_408;
  CManager *local_404;
  CAIStateGroup_SummonerCharge *pCStack_400;
  CCharString aCStack_3fc [4];
  CComponent *pCStack_3f8;
  int *piStack_3f4;
  CFrontEndManager *local_3f0;
  CTCInventoryBase *local_3ec;
  CCharString local_3e8 [4];
  CComponent *local_3e4;
  undefined4 *puStack_3e0;
  CQuestManager *pCStack_3dc;
  int iStack_3d8;
  CTCInventoryQuests *local_3d4;
  CComponent *local_3d0;
  CComponent *local_3cc;
  int *piStack_3c8;
  CComponent *pCStack_3c4;
  CTCInventoryBase *pCStack_3c0;
  int iStack_3bc;
  int *piStack_3b4;
  int *piStack_3b0;
  undefined4 uStack_3ac;
  int *piStack_3a8;
  CComponent *pCStack_3a4;
  undefined4 *puStack_3a0;
  CComponent *pCStack_39c;
  int *piStack_398;
  undefined4 uStack_394;
  int *apiStack_390 [2];
  CRegion *pCStack_388;
  undefined4 *puStack_384;
  int *piStack_380;
  int *piStack_37c;
  int *piStack_378;
  int *piStack_374;
  int *piStack_370;
  CComponent *pCStack_36c;
  int *piStack_368;
  CCharString aCStack_364 [4];
  CBaseObject *pCStack_360;
  CAIStateGroup_SummonerCharge *pCStack_35c;
  CAIStateGroup_SummonerCharge *pCStack_358;
  CRegion *pCStack_354;
  CComponent *local_350;
  undefined4 *local_34c;
  CComponent *local_348;
  undefined4 *local_344;
  CCharString aCStack_340 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_33c [4];
  CCharString aCStack_338 [12];
  CCharString aCStack_32c [8];
  CViewBase *pCStack_324;
  undefined4 uStack_318;
  int *piStack_314;
  int *piStack_310;
  CRegion *pCStack_30c;
  int local_308;
  CCategory *pCStack_304;
  CCharString aCStack_300 [8];
  int aiStack_2f8 [2];
  CCharString aCStack_2f0 [8];
  CWideString aCStack_2e8 [8];
  CCharString aCStack_2e0 [8];
  CCharString aCStack_2d8 [8];
  CWideString aCStack_2d0 [4];
  CCharString aCStack_2cc [4];
  int *piStack_2c8;
  float fStack_2c4;
  CCharString aCStack_2b4 [4];
  CComponent *pCStack_2b0;
  CManager *apCStack_2ac [2];
  CCharString aCStack_2a4 [8];
  CCharString aCStack_29c [24];
  CCharString aCStack_284 [8];
  CCharString aCStack_27c [8];
  CIDrawEnvironment *pCStack_274;
  CCharString aCStack_270 [4];
  CCharString aCStack_26c [4];
  CCharString aCStack_268 [8];
  CCharString aCStack_260 [8];
  undefined **ppuStack_258;
  CCharString aCStack_254 [4];
  CCharString aCStack_250 [8];
  CCharString aCStack_248 [4];
  CCharString aCStack_244 [4];
  CCharString aCStack_240 [8];
  CCharString aCStack_238 [4];
  int *piStack_234;
  int *piStack_230;
  CCharString aCStack_22c [4];
  CCharString aCStack_228 [4];
  CCharString aCStack_224 [4];
  CCharString aCStack_220 [4];
  CCharString aCStack_21c [4];
  CCharString aCStack_218 [4];
  CCharString aCStack_214 [4];
  CCharString aCStack_210 [8];
  CCharString aCStack_208 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_204 [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1fc [4];
  CCharString aCStack_1f8 [32];
  CCharString aCStack_1d8 [4];
  CCharString aCStack_1d4 [8];
  int iStack_1cc;
  CUIState aCStack_1c8 [20];
  undefined4 uStack_1b4;
  int iStack_1b0;
  CCharString aCStack_1ac [8];
  CCharString aCStack_1a4 [4];
  int aiStack_1a0 [6];
  undefined1 local_188 [4];
  undefined4 *local_184;
  undefined4 *local_180 [9];
  float fStack_15c;
  float fStack_158;
  int local_150;
  undefined4 local_14c;
  int local_148;
  undefined4 local_144;
  int local_140;
  int local_13c;
  int iStack_124;
  undefined **appuStack_120 [10];
  void *pvStack_f7;
  void *pvStack_b4;
  undefined **appuStack_9c [10];
  void *pvStack_73;
  void *pvStack_30;
  
  local_408 = (CManager *)0x0;
  ppuVar10 = local_180;
  iVar31 = 3;
  do {
    ppuVar10[-2] = (undefined4 *)0x0;
    ppuVar10[-1] = (undefined4 *)0x0;
    *ppuVar10 = (undefined4 *)0x0;
    ppuVar10 = ppuVar10 + 3;
    iVar31 = iVar31 + -1;
  } while (iVar31 != 0);
  local_3d4 = this;
  pCVar11 = NUISystem::CFrontEndManager::GetInstance();
  pCStack_424 = (CManager *)0x61b65e;
  local_3f0 = pCVar11;
  CCharString::CCharString((CCharString *)&local_3e4,"UI_BACK_FOR_LIST",-1);
  pCStack_424 = (CManager *)0x61b66b;
  local_350 = NUISystem::CManager::CreateComponent
                        ((CManager *)pCVar11,(CCharString *)&local_3e4,false);
  ppVar36 = extraout_EDX;
  if ((local_350 == (CComponent *)0x0) ||
     (local_34c = ::operator_new(0xc), ppVar36 = extraout_EDX_00, local_34c == (undefined4 *)0x0)) {
    local_34c = (undefined4 *)0x0;
  }
  else {
    *local_34c = 1;
    local_34c[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
    local_34c[2] = local_350;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_3e4,ppVar36,
             unaff_EDI);
  pCStack_424 = (CManager *)0x61b6c5;
  CCharString::CCharString((CCharString *)&local_3e4,"UI_SELECT_FOR_LIST",-1);
  pCStack_424 = (CManager *)0x61b6d2;
  local_348 = NUISystem::CManager::CreateComponent
                        ((CManager *)pCVar11,(CCharString *)&local_3e4,false);
  ppVar36 = extraout_EDX_01;
  if ((local_348 == (CComponent *)0x0) ||
     (local_344 = ::operator_new(0xc), ppVar36 = extraout_EDX_02, local_344 == (undefined4 *)0x0)) {
    local_344 = (undefined4 *)0x0;
  }
  else {
    *local_344 = 1;
    local_344[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
    local_344[2] = local_348;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_3e4,ppVar36,
             unaff_EDI);
  iVar31 = *(int *)param_1;
  local_404 = (CManager *)0x0;
  puVar14 = local_184;
  if ((*(int *)(param_1 + 4) - iVar31 & 0xfffffffcU) != 0) {
    do {
      iVar43 = *(int *)(iVar31 + (int)local_404 * 4);
      uVar32 = *(uint *)(iVar43 + 0x2c) >> 0xc;
      uStack_40c = (CComponent *)(CONCAT13((char)uVar32,(undefined3)uStack_40c) & 0x1ffffff);
      if ((uVar32 & 1) != 0) {
        local_3ec = (CTCInventoryBase *)&DAT_0000006c;
        piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                         ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                       *)(iVar43 + 0x44),(ETCInterfaceType *)&local_3ec);
        if ((piVar12 == *(int **)(iVar43 + 0x48)) || (0x6c < *piVar12)) {
          piVar12 = *(int **)(iVar43 + 0x48);
        }
        pCVar54 = (CTCQuestCard *)piVar12[1];
        if (param_3 == 0) {
          bVar9 = CTCQuestCard::IsCore(pCVar54);
          if (bVar9) {
            ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                      (*(int *)param_1 + (int)local_404 * 4);
            if (puVar14 == local_180[0]) goto LAB_0061b8c2;
            if (puVar14 != (undefined4 *)0x0) {
              *puVar14 = *(undefined4 *)ppVar33;
            }
            local_184 = puVar14 + 1;
            puVar14 = local_184;
          }
          else {
            bVar9 = CTCQuestCard::IsOptional(pCVar54);
            if (bVar9) {
              local_408 = (CManager *)((uint)local_408 | 1);
              local_3e4 = *(CComponent **)(pCVar54 + 0xc);
              if (local_3e4 != (CComponent *)0x0) {
                *(int *)((int)local_3e4 + 4) = *(int *)((int)local_3e4 + 4) + 1;
              }
              if (*(char *)((int)local_3e4 + 0x59) != '\0') goto LAB_0061b800;
              bVar9 = true;
            }
            else {
LAB_0061b800:
              bVar9 = false;
            }
            if ((((uint)local_408 & 1) != 0) &&
               (local_408 = (CManager *)((uint)local_408 & 0xfffffffe),
               local_3e4 != (CComponent *)0x0)) {
              piVar12 = (int *)((int)local_3e4 + 4);
              *piVar12 = *piVar12 + -1;
              if (*piVar12 == 0) {
                (**(code **)(*(int *)local_3e4 + 4))();
              }
              local_3e4 = (CComponent *)0x0;
            }
            if (bVar9) {
              FUN_00436c80();
              puVar14 = local_184;
            }
            else {
              piVar12 = *(int **)(pCVar54 + 0xc);
              if (piVar12 != (int *)0x0) {
                piVar12[1] = piVar12[1] + 1;
              }
              iVar31 = piVar12[1];
              cVar44 = *(char *)((int)piVar12 + 0x59);
              piVar12[1] = iVar31 + -1;
              if (iVar31 + -1 == 0) {
                (**(code **)(*piVar12 + 4))();
              }
              if (cVar44 == '\0') {
                FUN_00436c80();
                puVar14 = local_184;
              }
              else {
                FUN_00436c80();
                puVar14 = local_184;
              }
            }
          }
        }
        else {
          ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                    (iVar31 + (int)local_404 * 4);
          if (puVar14 == local_180[0]) {
LAB_0061b8c2:
            ppStack_428 = ppVar33;
            pCStack_424 = (CManager *)((int)&uStack_40c + 3);
            FUN_00436ac0();
            puVar14 = local_184;
          }
          else {
            if (puVar14 != (undefined4 *)0x0) {
              *puVar14 = *(undefined4 *)ppVar33;
            }
            local_184 = puVar14 + 1;
            puVar14 = local_184;
          }
        }
      }
      iVar31 = *(int *)param_1;
      local_404 = (CManager *)((int)local_404 + 1);
    } while (local_404 != (CManager *)(*(int *)(param_1 + 4) - iVar31 >> 2));
  }
  local_3e4 = (CComponent *)0x0;
  pCStack_424 = (CManager *)0x61b90a;
  CCharString::CCharString(local_3e8,"UI_MISC_THINGS_DEF",-1);
  pCVar20 = local_3e8;
  pCStack_424 = (CManager *)0x61b916;
  this_01 = (COptimisedPrimitive *)GFGetPreMainMemoryUsed();
  pCStack_424 = (CManager *)0x61b91d;
  lVar13 = QuadricOptimiserInternals::COptimisedPrimitive::ActualToUniqueIndex
                     (this_01,(ulong)pCVar20);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_3e8,
             extraout_EDX_03,unaff_EDI);
  pCVar55 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_3e4;
  pCStack_424 = (CManager *)0x61b933;
  this_02 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  pCStack_424 = (CManager *)0x61b93a;
  CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_02,lVar13,pCVar55);
  local_404 = *(CManager **)(DAT_013b86a0 + 0x14);
  iVar31 = *(int *)(local_3d4 + 4);
  uVar32 = *(uint *)(iVar31 + 0x20) >> 4;
  local_308 = 0;
  uStack_40c = (CComponent *)(CONCAT13((char)uVar32,(undefined3)uStack_40c) & 0x1ffffff);
  if ((uVar32 & 1) != 0) {
    local_3ec = (CTCInventoryBase *)&DAT_00000004;
    piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                     ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                   *)(iVar31 + 0x44),(ETCInterfaceType *)&local_3ec);
    if ((piVar12 == *(int **)(iVar31 + 0x48)) || (4 < *piVar12)) {
      piVar12 = *(int **)(iVar31 + 0x48);
    }
    local_308 = piVar12[1];
  }
  if (param_4 == (CList *)0x0) {
    param_4 = param_2;
  }
  pCStack_424 = (CManager *)0x61b9af;
  CCharString::CCharString(local_3e8,"UI_WORLD_MAP",-1);
  pCStack_424 = (CManager *)0x61b9be;
  local_3d0 = NUISystem::CManager::CreateComponent((CManager *)local_3f0,local_3e8,false);
  ppVar36 = extraout_EDX_04;
  if ((local_3d0 == (CComponent *)0x0) ||
     (local_3cc = ::operator_new(0xc), ppVar36 = extraout_EDX_05, local_3cc == (CComponent *)0x0)) {
    local_3cc = (CComponent *)0x0;
  }
  else {
    *(undefined4 *)local_3cc = 1;
    *(code **)((int)local_3cc + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
    *(CComponent **)((int)local_3cc + 8) = local_3d0;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_3e8,ppVar36,
             unaff_EDI);
  pCVar22 = local_3d0;
  local_140 = *(int *)(local_3e4 + 0xe0);
  local_13c = *(int *)(local_3e4 + 0xe4);
  piVar12 = &local_150;
  local_148 = 0x3dcccccd;
  local_144 = 0x3dcccccd;
  local_150 = 0x40000000;
  local_14c = 0x40000000;
  (**(code **)(*(int *)local_3d0 + 0x240))();
  pCStack_424 = (CManager *)0x61ba6d;
  (**(code **)(*(int *)pCVar22 + 0x23c))();
  pCStack_424 = (CManager *)&local_148;
  ppStack_428 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x61ba7f;
  (**(code **)(*(int *)pCVar22 + 0x244))();
  iVar31 = *(int *)param_4;
  ppStack_428 = extraout_ECX;
  CCharString::CCharString((CCharString *)&ppStack_428,"UI_SCROLLING_VIEWPORT_TEXT_AREA",-1);
  uStack_1b4 = (**(code **)(iVar31 + 0xc))();
  ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
            (*(int *)(param_1 + 4) - *(int *)param_1 >> 2);
  (**(code **)(*(int *)param_2 + 0x270))();
  CCharString::CCharString(aCStack_3fc,"UI_MINI_MAP_VIEWPORT_FOR_MAP_SCREEN",-1);
  pCStack_36c = NUISystem::CManager::CreateComponent(local_404,aCStack_3fc,false);
  if (pCStack_36c == (CComponent *)0x0) {
    piStack_368 = (int *)0x0;
    ppVar36 = extraout_EDX_06;
  }
  else {
    piStack_368 = ::operator_new(0xc);
    ppVar36 = extraout_EDX_07;
    if (piStack_368 == (int *)0x0) {
      piStack_368 = (int *)0x0;
    }
    else {
      *piStack_368 = 1;
      piStack_368[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      piStack_368[2] = (int)pCStack_36c;
    }
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_3fc,ppVar36,
             ppVar33);
  ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_36c;
  (**(code **)(*(int *)param_2 + 0x244))();
  local_404 = (CManager *)0x0;
  pCStack_400 = (CAIStateGroup_SummonerCharge *)0x0;
  do {
    local_3f0 = (CFrontEndManager *)
                (aiStack_1a0[(int)pCStack_400 * 3 + 1] - aiStack_1a0[(int)pCStack_400 * 3] >> 2);
    pCStack_324 = (CViewBase *)0x0;
    iStack_1b0 = aiStack_1a0[(int)pCStack_400 * 3];
    if (local_3f0 != (CFrontEndManager *)0x0) {
      do {
        local_34c = *(undefined4 **)(iStack_1b0 + (int)pCStack_324 * 4);
        pCVar54 = (CTCQuestCard *)0x0;
        pCStack_424 = (CManager *)
                      (CONCAT13((char)((uint)local_34c[0xb] >> 0xc),pCStack_424._0_3_) & 0x1ffffff);
        if (((uint)local_34c[0xb] >> 0xc & 1) != 0) {
          piVar53 = (int *)local_34c[0x12];
          piVar27 = (int *)local_34c[0x11];
          iVar31 = (int)piVar53 - (int)piVar27 >> 3;
          while (iVar43 = iVar31, 0 < iVar43) {
            iVar31 = iVar43 / 2;
            if (piVar27[iVar31 * 2] < 0x6c) {
              piVar27 = piVar27 + iVar31 * 2 + 2;
              iVar31 = iVar43 + (-1 - iVar31);
            }
          }
          if ((piVar27 == piVar53) || (0x6c < *piVar27)) {
            piVar27 = piVar53;
          }
          pCVar54 = (CTCQuestCard *)piVar27[1];
        }
        CTCQuestCard::GetQuestName(pCVar54);
        FUN_0099bff0();
        CCharString::~CCharString(aCStack_1a4);
        CCharString::CCharString(aCStack_268,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffbcc,"PC_BUTTON_BASTARD",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffbc4,"",-1);
        pCStack_3f8 = NUISystem::CManager::CreateMenuEntryComponent(local_408);
        ppVar36 = extraout_EDX_08;
        if ((pCStack_3f8 == (CComponent *)0x0) ||
           (piStack_3f4 = ::operator_new(0xc), ppVar36 = extraout_EDX_09, piStack_3f4 == (int *)0x0)
           ) {
          piStack_3f4 = (int *)0x0;
        }
        else {
          *piStack_3f4 = 1;
          piStack_3f4[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_3f4[2] = (int)pCStack_3f8;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_268,
                   ppVar36,ppVar33);
        if (param_3 == 0) {
          puVar14 = ::operator_new(0x10);
          if (puVar14 == (undefined4 *)0x0) {
            puVar14 = (undefined4 *)0x0;
          }
          else {
            *puVar14 = 0;
            puVar14[1] = 0;
            pvVar15 = malloc(0xc);
            *(void **)pvVar15 = pvVar15;
            *(void **)((int)pvVar15 + 4) = pvVar15;
            puVar14[1] = pvVar15;
            puVar14[2] = 0;
            pvVar15 = malloc(0xc);
            *(void **)pvVar15 = pvVar15;
            *(void **)((int)pvVar15 + 4) = pvVar15;
            puVar14[2] = pvVar15;
            *(undefined1 *)(puVar14 + 3) = 1;
          }
          puStack_384 = puVar14;
          if ((puVar14 == (undefined4 *)0x0) ||
             (piStack_380 = ::operator_new(0xc), piStack_380 == (int *)0x0)) {
            piStack_380 = (int *)0x0;
          }
          else {
            *piStack_380 = 1;
            piStack_380[1] = (int)CTCMapwho::OnDie;
            piStack_380[2] = (int)puStack_384;
          }
          *puStack_384 = 0x3c;
          (**(code **)(*(int *)pCStack_3f8 + 0x110))();
          if ((piStack_380 != (int *)0x0) && (*piStack_380 = *piStack_380 + -1, *piStack_380 == 0))
          {
            (*(code *)piStack_380[1])();
            operator_delete(piStack_380);
          }
          puStack_384 = (undefined4 *)0x0;
          piStack_380 = (int *)0x0;
        }
        pCVar16 = (CWorldMap *)(**(code **)(**(int **)(DAT_013b86a0 + 0x24) + 0x30))();
        CTCQuestCard::GetRegionName(pCVar54);
        pCStack_274 = (CIDrawEnvironment *)CWorldMap::GetRegionNumberFromName(pCVar16,aCStack_32c);
        if (pCStack_274 == (CIDrawEnvironment *)0x0) {
          pCStack_274 = CWorld::DrawGetEnvironment((CWorld *)pCVar16);
        }
        pCStack_304 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                                ((CTCInventoryBase *)pCVar16,(long)pCStack_274);
        FUN_0099ee20();
        CCharString_OperatorPlus_API();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_1fc,extraout_EDX_10,ppVar33);
        CCharString::CCharString((CCharString *)apCStack_2ac,"UI_QUEST_MENU_MINI_MAP_TEMPLATE",-1);
        pCVar3 = local_408;
        local_3cc = NUISystem::CManager::CreateComponent
                              (local_408,(CCharString *)apCStack_2ac,false);
        ppVar36 = extraout_EDX_11;
        if ((local_3cc == (CComponent *)0x0) ||
           (piStack_3c8 = ::operator_new(0xc), ppVar36 = extraout_EDX_12, piStack_3c8 == (int *)0x0)
           ) {
          piStack_3c8 = (int *)0x0;
        }
        else {
          *piStack_3c8 = 1;
          piStack_3c8[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_3c8[2] = (int)local_3cc;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)apCStack_2ac,
                   ppVar36,ppVar33);
        piVar27 = piStack_370;
        CCharString::CCharString(aCStack_254,"UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE",-1);
        pCStack_39c = NUISystem::CManager::CreateComponent(pCVar3,aCStack_254,false);
        ppVar36 = extraout_EDX_13;
        if ((pCStack_39c == (CComponent *)0x0) ||
           (piStack_398 = ::operator_new(0xc), ppVar36 = extraout_EDX_14, piStack_398 == (int *)0x0)
           ) {
          piStack_398 = (int *)0x0;
        }
        else {
          *piStack_398 = 1;
          piStack_398[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_398[2] = (int)pCStack_39c;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_254,
                   ppVar36,ppVar33);
        iVar31 = (**(code **)(*(int *)pCStack_39c + 0x104))();
        if (iVar31 == 0) {
          iVar31 = *(int *)pCStack_39c;
          NInventory::CTCInventoryBase::GetMiniMapGraphic(local_3ec,(CRegion *)pCStack_304);
          (**(code **)(iVar31 + 0x244))();
        }
        pCVar29 = local_3ec;
        (**(code **)(*(int *)local_3cc + 0xec))();
        pCVar22 = local_3d0;
        (**(code **)(*piVar27 + 0x238))();
        iVar31 = *(int *)(pCVar29 + 4);
        this_03 = (CTCInventoryMap *)0x0;
        if (((byte)((uint)*(undefined4 *)(iVar31 + 0x20) >> 0x1b) & 1) != 0) {
          piVar53 = *(int **)(iVar31 + 0x48);
          piVar27 = *(int **)(iVar31 + 0x44);
          iVar31 = (int)piVar53 - (int)piVar27 >> 3;
          while (iVar43 = iVar31, 0 < iVar43) {
            iVar31 = iVar43 / 2;
            if (piVar27[iVar31 * 2] < 0x1b) {
              piVar27 = piVar27 + iVar31 * 2 + 2;
              iVar31 = iVar43 + (-1 - iVar31);
            }
          }
          if ((piVar27 == piVar53) || (0x1b < *piVar27)) {
            piVar27 = piVar53;
          }
          this_03 = (CTCInventoryMap *)piVar27[1];
        }
        CTCInventoryMap::CreateMiniMapMarkers(this_03,(CComponent *)local_3d4,pCStack_30c);
        pCVar8 = pCStack_354;
        pCStack_360 = (CBaseObject *)pCStack_354;
        pCStack_35c = pCStack_400;
        FUN_00624620();
        CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_258);
        ppuStack_258 = &PTR__vector_deleting_destructor__01238c6c;
        CBaseIntelligentPointer::SetPItem
                  ((CBaseIntelligentPointer *)&ppuStack_258,(CBaseObject *)pCVar8);
        this_00 = *(CBaseIntelligentPointer **)(pCVar29 + 0x17c);
        if (this_00 == *(CBaseIntelligentPointer **)(pCVar29 + 0x180)) {
          FUN_004ac940();
        }
        else {
          if (this_00 != (CBaseIntelligentPointer *)0x0) {
            CBaseIntelligentPointer::CBaseIntelligentPointer(this_00);
            *(undefined ***)this_00 = &PTR__vector_deleting_destructor__01238c6c;
            pCVar17 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)&ppuStack_258);
            CBaseIntelligentPointer::SetPItem(this_00,pCVar17);
          }
          *(int *)(pCVar29 + 0x17c) = *(int *)(pCVar29 + 0x17c) + 8;
        }
        CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_258);
        CCharString::CCharString((CCharString *)apCStack_2ac,"HUD_ORB_QUEST_CORE",-1);
        piVar27 = *(int **)(unaff_EBX + 0x14);
        pCVar40 = *(CQuestManager **)(unaff_EBX + 0x10);
        if (piVar27 != (int *)0x0) {
          *piVar27 = *piVar27 + 1;
        }
        CQuestManager::OnQuestStartScreenDisplayed(pCVar40,(CCharString *)apCStack_2ac);
        ppVar36 = extraout_EDX_15;
        if ((piVar27 != (int *)0x0) && (*piVar27 = *piVar27 + -1, *piVar27 == 0)) {
          (*(code *)piVar27[1])();
          operator_delete(piVar27);
          ppVar36 = extraout_EDX_16;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)apCStack_2ac,
                   ppVar36,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar22);
        ppVar51 = ppStack_428;
        bVar9 = CTCQuestCard::IsCore((CTCQuestCard *)ppStack_428);
        if (bVar9) {
          CCharString::CCharString((CCharString *)&pCStack_274,"UI_QUEST_SPRITE_CORE",-1);
          pCVar18 = NUISystem::CManager::CreateComponent
                              (unaff_EBX,(CCharString *)&pCStack_274,false);
          ppVar36 = extraout_EDX_17;
          if ((pCVar18 != (CComponent *)0x0) &&
             (puVar14 = ::operator_new(0xc), ppVar36 = extraout_EDX_18, puVar14 != (undefined4 *)0x0
             )) {
            *puVar14 = 1;
            puVar14[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
            puVar14[2] = pCVar18;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_274,
                     ppVar36,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar22);
          CCharString::CCharString(aCStack_2a4,"HUD_ORB_QUEST_CORE",-1);
          piVar27 = *(int **)(unaff_EBX + 0x14);
          pCVar40 = *(CQuestManager **)(unaff_EBX + 0x10);
          if (piVar27 != (int *)0x0) {
            *piVar27 = *piVar27 + 1;
          }
          bVar9 = CQuestManager::OnQuestStartScreenDisplayed(pCVar40,aCStack_2a4);
          uStack_394 = CONCAT31(extraout_var,bVar9);
          ppVar36 = extraout_EDX_19;
          if ((piVar27 != (int *)0x0) && (*piVar27 = *piVar27 + -1, *piVar27 == 0)) {
            (*(code *)piVar27[1])();
            operator_delete(piVar27);
            ppVar36 = extraout_EDX_20;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2a4,
                     ppVar36,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar22);
        }
        else {
          piVar27 = *(int **)(ppVar51 + 0xc);
          if (piVar27 != (int *)0x0) {
            piVar27[1] = piVar27[1] + 1;
          }
          iVar31 = piVar27[1];
          cVar44 = *(char *)((int)piVar27 + 0x59);
          piVar27[1] = iVar31 + -1;
          if (iVar31 + -1 == 0) {
            (**(code **)(*piVar27 + 4))();
          }
          if (cVar44 == '\0') {
            CCharString::CCharString(aCStack_26c,"UI_QUEST_SPRITE_OPTIONAL",-1);
            pCVar18 = NUISystem::CManager::CreateComponent(unaff_EBX,aCStack_26c,false);
            ppVar36 = extraout_EDX_25;
            if ((pCVar18 != (CComponent *)0x0) &&
               (puVar14 = ::operator_new(0xc), ppVar36 = extraout_EDX_26,
               puVar14 != (undefined4 *)0x0)) {
              *puVar14 = 1;
              puVar14[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
              puVar14[2] = pCVar18;
            }
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_26c,
                       ppVar36,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar22);
            CCharString::CCharString(aCStack_2f0,"HUD_ORB_QUEST_OPTIONAL",-1);
            piVar27 = *(int **)(unaff_EBX + 0x14);
            pCVar40 = *(CQuestManager **)(unaff_EBX + 0x10);
            if (piVar27 != (int *)0x0) {
              *piVar27 = *piVar27 + 1;
            }
            bVar9 = CQuestManager::OnQuestStartScreenDisplayed(pCVar40,aCStack_2f0);
            uStack_394 = CONCAT31(extraout_var_01,bVar9);
            ppVar36 = extraout_EDX_27;
            if ((piVar27 != (int *)0x0) && (*piVar27 = *piVar27 + -1, *piVar27 == 0)) {
              (*(code *)piVar27[1])();
              operator_delete(piVar27);
              ppVar36 = extraout_EDX_28;
            }
            paVar34 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2f0;
          }
          else {
            CCharString::CCharString(aCStack_244,"UI_QUEST_SPRITE_VIGNETTE",-1);
            pCVar18 = NUISystem::CManager::CreateComponent(unaff_EBX,aCStack_244,false);
            ppVar36 = extraout_EDX_21;
            if ((pCVar18 != (CComponent *)0x0) &&
               (puVar14 = ::operator_new(0xc), ppVar36 = extraout_EDX_22,
               puVar14 != (undefined4 *)0x0)) {
              *puVar14 = 1;
              puVar14[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
              puVar14[2] = pCVar18;
            }
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_244,
                       ppVar36,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar22);
            CCharString::CCharString(aCStack_29c,"HUD_ORB_QUEST_VIGNETTE",-1);
            piVar27 = *(int **)(unaff_EBX + 0x14);
            pCVar40 = *(CQuestManager **)(unaff_EBX + 0x10);
            if (piVar27 != (int *)0x0) {
              *piVar27 = *piVar27 + 1;
            }
            bVar9 = CQuestManager::OnQuestStartScreenDisplayed(pCVar40,aCStack_29c);
            uStack_394 = CONCAT31(extraout_var_00,bVar9);
            ppVar36 = extraout_EDX_23;
            if ((piVar27 != (int *)0x0) && (*piVar27 = *piVar27 + -1, *piVar27 == 0)) {
              (*(code *)piVar27[1])();
              operator_delete(piVar27);
              ppVar36 = extraout_EDX_24;
            }
            paVar34 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_29c;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (paVar34,ppVar36,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar22);
        }
        piVar27 = (int *)&stack0xfffffbe8;
        (**(code **)(*(int *)pCStack_400 + 0xec))();
        (**(code **)(*(int *)param_2 + 0x260))();
        (**(code **)(*(int *)param_2 + 0x244))();
        ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&piStack_374;
        (**(code **)(*(int *)param_2 + 0x244))();
        CCharString::CCharString((CCharString *)aiStack_2f8,"UI_TITLE_QUEST_TEXT",-1);
        pCStack_3c4 = NUISystem::CManager::CreateComponent
                                (unaff_EBX,(CCharString *)aiStack_2f8,false);
        ppVar36 = extraout_EDX_29;
        if ((pCStack_3c4 == (CComponent *)0x0) ||
           (pCStack_3c0 = ::operator_new(0xc), ppVar36 = extraout_EDX_30,
           pCStack_3c0 == (CTCInventoryBase *)0x0)) {
          pCStack_3c0 = (CTCInventoryBase *)0x0;
        }
        else {
          *(int *)pCStack_3c0 = 1;
          *(code **)(pCStack_3c0 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCStack_3c0 + 8) = pCStack_3c4;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aiStack_2f8,
                   ppVar36,ppVar51);
        local_3ec = (CTCInventoryBase *)0x0;
        pCVar22 = (CComponent *)0x0;
        if (pCStack_3c0 != (CTCInventoryBase *)0x0) {
          local_3ec = pCStack_3c0;
          *(int *)pCStack_3c0 = *(int *)pCStack_3c0 + 1;
          pCVar22 = pCStack_3c4;
        }
        piVar53 = (int *)0x1;
        (**(code **)(*(int *)pCVar22 + 0x240))();
        ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_3d0;
        (**(code **)(*piVar12 + 0xec))();
        CCharString::CCharString(aCStack_300,"UI_TITLE_QUEST_SPRITE",-1);
        pCStack_3a4 = NUISystem::CManager::CreateComponent(unaff_EBX,aCStack_300,false);
        ppVar36 = extraout_EDX_31;
        if ((pCStack_3a4 == (CComponent *)0x0) ||
           (puStack_3a0 = ::operator_new(0xc), ppVar36 = extraout_EDX_32,
           puStack_3a0 == (undefined4 *)0x0)) {
          puStack_3a0 = (undefined4 *)0x0;
        }
        else {
          *puStack_3a0 = 1;
          puStack_3a0[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
          puStack_3a0[2] = pCStack_3a4;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_300,
                   ppVar36,ppVar51);
        piVar52 = piStack_3b4;
        (**(code **)(*(int *)pCStack_3a4 + 0x244))();
        ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&piStack_3a8;
        (**(code **)(*(int *)pCStack_424 + 0xec))();
        CCharString::CCharString(aCStack_300,"UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE",-1);
        local_3e4 = NUISystem::CManager::CreateComponent(unaff_EBX,aCStack_300,false);
        ppVar36 = extraout_EDX_33;
        if ((local_3e4 == (CComponent *)0x0) ||
           (puStack_3e0 = ::operator_new(0xc), ppVar36 = extraout_EDX_34,
           puStack_3e0 == (undefined4 *)0x0)) {
          puStack_3e0 = (undefined4 *)0x0;
        }
        else {
          *puStack_3e0 = 1;
          puStack_3e0[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
          puStack_3e0[2] = local_3e4;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_300,
                   ppVar36,ppVar51);
        pCVar22 = local_3e4;
        CCharString::CCharString((CCharString *)&stack0xfffffbb8);
        piVar23 = (int *)piVar53[3];
        if (piVar23 != (int *)0x0) {
          piVar23[1] = piVar23[1] + 1;
        }
        iVar31 = piVar23[0x18];
        iVar43 = piVar23[1];
        piVar23[1] = iVar43 + -1;
        if (iVar43 + -1 == 0) {
          (**(code **)(*piVar23 + 4))();
        }
        if (iVar31 != 0) {
          piVar23 = (int *)piVar53[3];
          iStack_3bc = 0;
          if (piVar23 != (int *)0x0) {
            piVar23[1] = piVar23[1] + 1;
          }
          iVar31 = piVar23[0x17];
          iVar42 = iVar31;
          iVar43 = *(int *)(iVar31 + 4);
          while (iVar43 != 0) {
            if (*(int *)(iVar43 + 0x10) < 0) {
              iVar43 = *(int *)(iVar43 + 0xc);
            }
            else {
              iVar42 = iVar43;
              iVar43 = *(int *)(iVar43 + 8);
            }
          }
          if ((iVar42 == iVar31) || (0 < *(int *)(iVar42 + 0x10))) {
            iVar42 = iVar31;
          }
          if (piVar23 != (int *)0x0) {
            piVar49 = piVar23 + 1;
            *piVar49 = *piVar49 + -1;
            if (*piVar49 == 0) {
              (**(code **)(*piVar23 + 4))();
            }
          }
          piVar23 = (int *)piVar53[3];
          if (piVar23 != (int *)0x0) {
            piVar23[1] = piVar23[1] + 1;
          }
          iVar31 = piVar23[0x17];
          iVar43 = piVar23[1];
          piVar23[1] = iVar43 + -1;
          if (iVar43 + -1 == 0) {
            (**(code **)(*piVar23 + 4))();
          }
          iVar43 = iStack_3bc;
          if (iVar42 != iVar31) {
            iVar43 = *(int *)(iVar42 + 0x14);
          }
          if (*(int *)(local_350 + 0x70) < iVar43) {
            CCharString::CCharString
                      ((CCharString *)aiStack_2f8,"TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH",-1);
            pCVar37 = *(CDataBank **)(DAT_013b86a0 + 0x14);
            FUN_0099b6b0();
            NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_224);
            pCVar19 = (CWideString *)FUN_0099be70();
            CWideString::operator=((CWideString *)&stack0xfffffbb8,pCVar19);
            CCharString::~CCharString(aCStack_1ac);
            CCharString::~CCharString(aCStack_224);
            CCharString::~CCharString((CCharString *)&ppuStack_258);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aiStack_2f8,
                       extraout_EDX_35,ppVar51);
            CCharString::CCharString((CCharString *)&stack0xfffffb98,"",-1);
            (**(code **)(*(int *)pCVar22 + 0x238))();
          }
        }
        CCharString::CCharString(aCStack_2f0,"TEXT_GUI_MENU_QUEST_SUMMARY",-1);
        pCVar19 = (CWideString *)
                  NGameText::CDataBank::GetTextBySymbol
                            (*(CDataBank **)(DAT_013b86a0 + 0x14),aCStack_21c);
        CWideString::operator=((CWideString *)&stack0xfffffbb8,pCVar19);
        CCharString::~CCharString(aCStack_21c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2f0,
                   extraout_EDX_36,ppVar51);
        piVar23 = extraout_ECX_00;
        CCharString::CCharString((CCharString *)&stack0xfffffb98,"",-1);
        pCVar37 = (CDataBank *)0xffc8c814;
        pCVar54 = (CTCQuestCard *)&stack0xfffffbb8;
        (**(code **)(*(int *)pCVar22 + 0x238))();
        pCStack_478 = aCStack_224;
        CTCQuestCard::GetQuestSummary((CTCQuestCard *)ppVar51);
        pCStack_478 = (CCharString *)0x61c823;
        pCVar19 = (CWideString *)FUN_0099bf30();
        pCStack_478 = (CCharString *)0x61c82d;
        CWideString::operator=((CWideString *)&stack0xfffffba8,pCVar19);
        CCharString::~CCharString((CCharString *)&iStack_1b0);
        CCharString::~CCharString(aCStack_224);
        uVar32 = 0;
        CCharString::CCharString((CCharString *)&pCStack_478,"",-1);
        this_08 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffba8;
        (**(code **)(*(int *)pCVar22 + 0x238))();
        if (param_3 == 1) {
          pCVar50 = pCVar54;
          pCVar20 = (CCharString *)CTCQuestCard::GetQuestName(pCVar54);
          lVar13 = CCharString::GetLength(pCVar20);
          CCharString::~CCharString(aCStack_1f8);
          if (lVar13 != 0) {
            CCharString::CCharString((CCharString *)&local_308,"TEXT_GUI_MENU_CURRENT_OBJECTIVE",-1)
            ;
            pCVar19 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_22c);
            CWideString::operator=((CWideString *)&stack0xfffffb98,pCVar19);
            CCharString::~CCharString(aCStack_22c);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_308,
                       extraout_EDX_37,this_08);
            CCharString::CCharString((CCharString *)&stack0xfffffb78,"",-1);
            (**(code **)(*(int *)pCVar22 + 0x238))();
            CTCQuestCard::GetQuestName(pCVar54);
            pCVar19 = (CWideString *)FUN_0099bf30();
            CWideString::operator=((CWideString *)&pCStack_478,pCVar19);
            CCharString::~CCharString(aCStack_1d8);
            CCharString::~CCharString((CCharString *)&piStack_234);
            CCharString::CCharString((CCharString *)&stack0xfffffb68,"",-1);
            (**(code **)(*(int *)pCVar22 + 0x238))();
          }
          pCVar54 = pCVar50;
          pCVar40 = DAT_013b89fc + 0x6c;
          pCStack_3dc = pCVar40;
          CCharString::CCharString((CCharString *)&pCStack_3c4,(CCharString *)(pCVar54 + 0x28));
          CCharString::CCharString((CCharString *)&stack0xfffffb9c);
          piVar49 = *(int **)pCVar40;
          piVar41 = (int *)*piVar49;
          uVar32 = uVar32 & 0xffffff;
          if (piVar41 != piVar49) {
            do {
              pCVar18 = (CComponent *)piVar41[5];
              if (pCVar18 == pCStack_3c4) {
LAB_0061ca55:
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_208);
                CCharString::operator+=
                          ((CCharString *)&stack0xfffffb9c,(CCharString *)(piVar41 + 6));
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_21c);
                CCharString::CCharString(aCStack_300,"TEXT_GUI_MENU_WAGER",-1);
                pCVar20 = (CCharString *)
                          NGameText::CDataBank::GetTextBySymbol(pCVar37,(CCharString *)&pCStack_274)
                ;
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString((CCharString *)&pCStack_274);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_300,extraout_EDX_38,this_08);
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_270);
                CCharString::CCharString((CCharString *)&local_350);
                FUN_0099ba70();
                CWideString::CWideString((CWideString *)aiStack_2f8,(CWideString *)&local_350);
                CCharString::~CCharString((CCharString *)&local_350);
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,(CCharString *)aiStack_2f8);
                CCharString::~CCharString((CCharString *)aiStack_2f8);
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_268);
                CCharString::CCharString(aCStack_2f0,"TEXT_GUI_MENU_REWARD",-1);
                pCVar20 = (CCharString *)NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_260);
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_260);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_2f0,extraout_EDX_39,this_08);
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString((CCharString *)&ppuStack_258);
                CCharString::CCharString((CCharString *)&pCStack_36c);
                FUN_0099ba70();
                CWideString::CWideString(aCStack_2e8,(CWideString *)&pCStack_36c);
                CCharString::~CCharString((CCharString *)&pCStack_36c);
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,(CCharString *)aCStack_2e8);
                CCharString::~CCharString((CCharString *)aCStack_2e8);
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_250);
                uVar32 = CONCAT13(1,(int3)uVar32);
                if (*(char *)((int)piVar41 + 0x21) != '\0') {
                  pCVar20 = (CCharString *)FUN_0099b6b0();
                  CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                  CCharString::~CCharString(aCStack_248);
                  CCharString::CCharString(aCStack_2e0,"TEXT_GUI_MENU_FAILED",-1);
                  pCVar20 = (CCharString *)
                            NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_240);
                  CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                  CCharString::~CCharString(aCStack_240);
                  std::
                  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                             aCStack_2e0,extraout_EDX_40,this_08);
                }
                pCVar20 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb9c,pCVar20);
                CCharString::~CCharString(aCStack_238);
              }
              else if (((pCVar18 != (CComponent *)0x0) && (pCStack_3c4 != (CComponent *)0x0)) &&
                      (*(int *)(pCVar18 + 4) == *(int *)(pCStack_3c4 + 4))) {
                pcVar21 = *(char **)pCVar18;
                iVar31 = *(int *)pCStack_3c4 - (int)pcVar21;
                while( true ) {
                  cVar44 = *pcVar21;
                  if ((cVar44 == '\0') && (pcVar21[iVar31] == '\0')) {
                    iVar31 = 0;
                    goto LAB_0061ca48;
                  }
                  if (cVar44 < pcVar21[iVar31]) {
                    iVar31 = -1;
                    goto LAB_0061ca48;
                  }
                  if (pcVar21[iVar31] < cVar44) break;
                  pcVar21 = pcVar21 + 1;
                }
                iVar31 = 1;
LAB_0061ca48:
                if (iVar31 == 0) goto LAB_0061ca55;
              }
              piVar41 = (int *)*piVar41;
            } while (piVar41 != (int *)*(int *)pCStack_3dc);
            if ((char)(uVar32 >> 0x18) != '\0') {
              CCharString::CCharString(aCStack_2d8,"TEXT_GUI_MENU_BOASTS",-1);
              pCVar19 = (CWideString *)
                        NGameText::CDataBank::GetTextBySymbol(pCVar37,(CCharString *)&piStack_230);
              CWideString::operator=((CWideString *)&stack0xfffffb98,pCVar19);
              CCharString::~CCharString((CCharString *)&piStack_230);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_2d8,extraout_EDX_41,this_08);
              CCharString::CCharString((CCharString *)&stack0xfffffb78,"",-1);
              (**(code **)(*(int *)pCVar22 + 0x238))();
              CCharString::CCharString((CCharString *)&stack0xfffffb68,"",-1);
              (**(code **)(*(int *)pCVar22 + 0x238))();
            }
          }
          CCharString::~CCharString((CCharString *)&stack0xfffffb9c);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_3c4,
                     extraout_EDX_42,this_08);
        }
        CCharString::CCharString((CCharString *)&piStack_2c8,"TEXT_GUI_MENU_MONEY_REWARD",-1);
        CCharString::CCharString((CCharString *)&puStack_384);
        FUN_0099ba70();
        CWideString::CWideString(aCStack_2d0,(CWideString *)&puStack_384);
        CCharString::~CCharString((CCharString *)&puStack_384);
        FUN_0099b6b0();
        pCVar24 = pCVar37;
        NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_218);
        FUN_0099be70();
        pCVar19 = (CWideString *)FUN_0099be70();
        CWideString::operator=((CWideString *)&stack0xfffffb98,pCVar19);
        CCharString::~CCharString(aCStack_228);
        CCharString::~CCharString(aCStack_220);
        CCharString::~CCharString(aCStack_218);
        CCharString::~CCharString(aCStack_210);
        CCharString::~CCharString((CCharString *)aCStack_2d0);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_2c8,
                   extraout_EDX_43,this_08);
        piVar49 = (int *)0x0;
        iVar31 = extraout_ECX_01;
        CCharString::CCharString((CCharString *)&stack0xfffffb78,"HUD_ICON_MONEY_BAG",-1);
        this_07 = (CDataBank *)0xffffffff;
        ppVar48 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffb98;
        (**(code **)(*(int *)pCVar22 + 0x238))();
        CCharString::CCharString((CCharString *)&local_34c,"TEXT_GUI_MENU_RENOWN_REWARD",-1);
        CCharString::CCharString((CCharString *)apiStack_390);
        FUN_0099ba70();
        CWideString::CWideString(aCStack_2d0,(CWideString *)apiStack_390);
        CCharString::~CCharString((CCharString *)apiStack_390);
        FUN_0099b6b0();
        NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_214);
        FUN_0099be70();
        pCVar19 = (CWideString *)FUN_0099be70();
        CWideString::operator=((CWideString *)&pCStack_478,pCVar19);
        CCharString::~CCharString(aCStack_1d4);
        CCharString::~CCharString((CCharString *)&iStack_1cc);
        CCharString::~CCharString(aCStack_214);
        CCharString::~CCharString(aCStack_27c);
        CCharString::~CCharString((CCharString *)aCStack_2d0);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_34c,
                   extraout_EDX_44,ppVar48);
        CCharString::CCharString((CCharString *)&stack0xfffffb68,"HUD_ORB_QUEST_CORE",-1);
        this_06 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_478;
        (**(code **)(*(int *)pCVar22 + 0x238))();
        if (param_3 == 0) {
          CCharString::CCharString((CCharString *)&piStack_2c8,"TEXT_GUI_MENU_NUM_BOASTS",-1);
          piVar41 = *(int **)(ppVar48 + 0xc);
          if (piVar41 != (int *)0x0) {
            piVar41[1] = piVar41[1] + 1;
          }
          CCharString::CCharString((CCharString *)&piStack_398);
          FUN_0099ba70();
          CWideString::CWideString(aCStack_2d0,(CWideString *)&piStack_398);
          CCharString::~CCharString((CCharString *)&piStack_398);
          FUN_0099b6b0();
          NGameText::CDataBank::GetTextBySymbol(this_07,aCStack_21c);
          FUN_0099be70();
          pCVar19 = (CWideString *)FUN_0099be70();
          CWideString::operator=((CWideString *)&stack0xfffffb78,pCVar19);
          CCharString::~CCharString((CCharString *)&piStack_234);
          CCharString::~CCharString(aCStack_284);
          CCharString::~CCharString(aCStack_21c);
          CCharString::~CCharString(aCStack_27c);
          CCharString::~CCharString((CCharString *)aCStack_2d0);
          piVar1 = piVar41 + 1;
          *piVar1 = *piVar1 + -1;
          ppVar36 = extraout_EDX_45;
          pCVar37 = this_07;
          if (*piVar1 == 0) {
            (**(code **)(*piVar41 + 4))();
            ppVar36 = extraout_EDX_46;
            pCVar37 = this_07;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_2c8,
                     ppVar36,this_06);
          CCharString::CCharString((CCharString *)&stack0xfffffb58,"",-1);
          (**(code **)(*(int *)pCVar22 + 0x238))();
          this_07 = pCVar37;
        }
        this_05 = (CDataBank *)&pCStack_424;
        (**(code **)(*piVar23 + 0xec))();
        ppVar46 = ppStack_428;
        (**(code **)(*piStack_230 + 0x238))();
        CCharString::CCharString(aCStack_2a4,"UI_SUB_LIST",-1);
        uStack_40c = NUISystem::CManager::CreateComponent((CManager *)this_08,aCStack_2a4,false);
        ppVar36 = extraout_EDX_47;
        if ((uStack_40c == (CComponent *)0x0) ||
           (local_408 = ::operator_new(0xc), ppVar36 = extraout_EDX_48, local_408 == (CManager *)0x0
           )) {
          local_408 = (CManager *)0x0;
        }
        else {
          *(int *)local_408 = 1;
          *(code **)(local_408 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(local_408 + 8) = uStack_40c;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2a4,
                   ppVar36,ppVar46);
        pCStack_2b0 = (CComponent *)0x0;
        apCStack_2ac[0] = (CManager *)0x0;
        if (local_408 != (CManager *)0x0) {
          pCStack_2b0 = uStack_40c;
          apCStack_2ac[0] = local_408;
          *(int *)local_408 = *(int *)local_408 + 1;
        }
        pCVar22 = pCStack_2b0;
        (**(code **)(*(int *)pCVar54 + 0xec))();
        puVar39 = &stack0xfffffbb8;
        (**(code **)(*(int *)pCVar22 + 0x244))();
        pCVar20 = local_3e8;
        (**(code **)(*(int *)pCVar22 + 0x244))();
        if (param_3 == 0) {
          ppVar45 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_3e4;
          (**(code **)(*(int *)pCVar22 + 0x244))();
          piVar23 = *(int **)(ppVar46 + 0xc);
          if (piVar23 != (int *)0x0) {
            piVar23[1] = piVar23[1] + 1;
          }
          iVar43 = piVar23[0x17];
          iVar38 = iVar43;
          iVar42 = *(int *)(iVar43 + 4);
          while (iVar42 != 0) {
            if (*(int *)(iVar42 + 0x10) < 0) {
              iVar42 = *(int *)(iVar42 + 0xc);
            }
            else {
              iVar38 = iVar42;
              iVar42 = *(int *)(iVar42 + 8);
            }
          }
          if ((iVar38 == iVar43) || (0 < *(int *)(iVar38 + 0x10))) {
            iVar38 = iVar43;
          }
          ppVar47 = ppVar46;
          if (piVar23 != (int *)0x0) {
            piVar41 = piVar23 + 1;
            *piVar41 = *piVar41 + -1;
            if (*piVar41 == 0) {
              (**(code **)(*piVar23 + 4))();
            }
          }
          piVar23 = *(int **)(ppVar46 + 0xc);
          if (piVar23 != (int *)0x0) {
            piVar23[1] = piVar23[1] + 1;
          }
          iVar43 = piVar23[0x17];
          iVar42 = piVar23[1];
          piVar23[1] = iVar42 + -1;
          if (iVar42 + -1 == 0) {
            (**(code **)(*piVar23 + 4))();
          }
          iVar42 = 0;
          if (iVar38 != iVar43) {
            iVar42 = *(int *)(iVar38 + 0x14);
          }
          if (iVar42 <= piStack_3a8[0x1c]) {
            piVar23 = *(int **)(ppVar47 + 0xc);
            if (piVar23 != (int *)0x0) {
              piVar23[1] = piVar23[1] + 1;
            }
            CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)&pCStack_36c);
            bVar9 = CQuestManager::IsActiveQuestStartedInRegion
                              (DAT_013b89fc,(CCharString *)&pCStack_36c);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_36c
                       ,extraout_EDX_49,ppVar45);
            if (piVar23 != (int *)0x0) {
              piVar41 = piVar23 + 1;
              *piVar41 = *piVar41 + -1;
              if (*piVar41 == 0) {
                (**(code **)(*piVar23 + 4))();
              }
            }
            if (!bVar9) {
              CCharString::CCharString((CCharString *)&piStack_314,"TEXT_GUI_MENU_TAKE_QUEST",-1);
              NGameText::CDataBank::GetTextBySymbol(this_05,(CCharString *)&pCStack_354);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         &piStack_314,extraout_EDX_50,ppVar45);
              CCharString::CCharString(aCStack_364,"UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",-1);
              CCharString::CCharString((CCharString *)&stack0xfffffb44,"PC_BUTTON_BASTARD",-1);
              CCharString::CCharString((CCharString *)&stack0xfffffb3c,"",-1);
              pCVar22 = NUISystem::CManager::CreateMenuEntryComponent
                                  ((CManager *)ppVar48,&pCStack_354,0,0x3f800000,aCStack_364,0);
              ppVar36 = extraout_EDX_51;
              if ((pCVar22 != (CComponent *)0x0) &&
                 (puVar14 = ::operator_new(0xc), ppVar36 = extraout_EDX_52,
                 puVar14 != (undefined4 *)0x0)) {
                *puVar14 = 1;
                puVar14[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
                puVar14[2] = pCVar22;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_364,ppVar36,ppVar45);
              piVar23 = ::operator_new(0x10);
              if (piVar23 == (int *)0x0) {
                piVar23 = (int *)0x0;
              }
              else {
                *piVar23 = 0;
                piVar23[1] = 0;
                pvVar15 = malloc(0xc);
                *(void **)pvVar15 = pvVar15;
                *(void **)((int)pvVar15 + 4) = pvVar15;
                piVar23[1] = (int)pvVar15;
                piVar23[2] = 0;
                pvVar15 = malloc(0xc);
                *(void **)pvVar15 = pvVar15;
                *(void **)((int)pvVar15 + 4) = pvVar15;
                piVar23[2] = (int)pvVar15;
                *(undefined1 *)(piVar23 + 3) = 1;
              }
              if ((piVar23 != (int *)0x0) &&
                 (puVar14 = ::operator_new(0xc), puVar14 != (undefined4 *)0x0)) {
                *puVar14 = 1;
                puVar14[1] = CTCMapwho::OnDie;
                puVar14[2] = piVar23;
              }
              *piVar23 = 6;
              uVar2 = *(ushort *)(local_3d4 + 0x8c);
              iVar43 = piVar23[2];
              piVar49 = malloc(0xc);
              if ((uint *)(piVar49 + 2) != (uint *)0x0) {
                piVar49[2] = (uint)uVar2;
              }
              puVar14 = *(undefined4 **)(iVar43 + 4);
              *piVar49 = iVar43;
              piVar49[1] = (int)puVar14;
              *puVar14 = piVar49;
              *(int **)(iVar43 + 4) = piVar49;
              (**(code **)(*(int *)pCVar22 + 0x108))();
              ppVar33 = ::operator_new(0x10);
              if (ppVar33 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
                ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
              }
              else {
                *(undefined4 *)ppVar33 = 0;
                *(undefined4 *)(ppVar33 + 4) = 0;
                pvVar15 = malloc(0xc);
                *(void **)pvVar15 = pvVar15;
                *(void **)((int)pvVar15 + 4) = pvVar15;
                *(void **)(ppVar33 + 4) = pvVar15;
                *(undefined4 *)(ppVar33 + 8) = 0;
                pvVar15 = malloc(0xc);
                *(void **)pvVar15 = pvVar15;
                *(void **)((int)pvVar15 + 4) = pvVar15;
                *(void **)(ppVar33 + 8) = pvVar15;
                ppVar33[0xc] = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>)0x1;
              }
              if ((ppVar33 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) &&
                 (puVar14 = ::operator_new(0xc), puVar14 != (undefined4 *)0x0)) {
                *puVar14 = 1;
                puVar14[1] = CTCMapwho::OnDie;
                puVar14[2] = ppVar33;
              }
              *(undefined4 *)ppVar33 = 0xbbc;
              uVar2 = *(ushort *)(iStack_3d8 + 0x8c);
              iVar43 = *(int *)(ppVar33 + 8);
              piVar49 = malloc(0xc);
              if ((uint *)(piVar49 + 2) != (uint *)0x0) {
                piVar49[2] = (uint)uVar2;
              }
              puVar14 = *(undefined4 **)(iVar43 + 4);
              *piVar49 = iVar43;
              piVar49[1] = (int)puVar14;
              *puVar14 = piVar49;
              *(int **)(iVar43 + 4) = piVar49;
              pCVar24 = ::operator_new(0x10);
              if (pCVar24 == (CDataBank *)0x0) {
                pCVar24 = (CDataBank *)0x0;
              }
              else {
                *(int *)pCVar24 = 0;
                *(int *)(pCVar24 + 4) = 0;
                pvVar15 = malloc(0xc);
                *(void **)pvVar15 = pvVar15;
                *(void **)((int)pvVar15 + 4) = pvVar15;
                *(void **)(pCVar24 + 4) = pvVar15;
                *(int *)(pCVar24 + 8) = 0;
                pvVar15 = malloc(0xc);
                *(void **)pvVar15 = pvVar15;
                *(void **)((int)pvVar15 + 4) = pvVar15;
                *(void **)(pCVar24 + 8) = pvVar15;
                pCVar24[0xc] = (CDataBank)0x1;
              }
              if ((pCVar24 != (CDataBank *)0x0) &&
                 (puVar14 = ::operator_new(0xc), puVar14 != (undefined4 *)0x0)) {
                *puVar14 = 1;
                puVar14[1] = CTCMapwho::OnDie;
                puVar14[2] = pCVar24;
              }
              *(int *)pCVar24 = 0xd8;
              if (uVar32 == 0) {
                iVar43 = 0;
              }
              else {
                iVar43 = uVar32 + 0x18;
              }
              iVar42 = *(int *)(pCVar24 + 4);
              piVar49 = malloc(0xc);
              if (piVar49 + 2 != (int *)0x0) {
                piVar49[2] = iVar43;
              }
              puVar14 = *(undefined4 **)(iVar42 + 4);
              *piVar49 = iVar42;
              piVar49[1] = (int)puVar14;
              *puVar14 = piVar49;
              *(int **)(iVar42 + 4) = piVar49;
              (**(code **)(*piVar53 + 0x108))();
              uVar2 = *(ushort *)(pCStack_3dc + 0x8c);
              iVar43 = *(int *)(pCVar54 + 8);
              piVar53 = malloc(0xc);
              if ((uint *)(piVar53 + 2) != (uint *)0x0) {
                piVar53[2] = (uint)uVar2;
              }
              puVar14 = *(undefined4 **)(iVar43 + 4);
              *piVar53 = iVar43;
              piVar53[1] = (int)puVar14;
              *puVar14 = piVar53;
              *(int **)(iVar43 + 4) = piVar53;
              piVar53 = *(int **)(puVar39 + 0xc);
              if (piVar53 != (int *)0x0) {
                piVar53[1] = piVar53[1] + 1;
              }
              iVar43 = piVar53[1];
              piVar53[1] = iVar43 + -1;
              if (iVar43 + -1 == 0) {
                (**(code **)(*piVar53 + 4))();
              }
              piVar53 = piStack_2c8;
              (**(code **)(*piStack_2c8 + 0x270))();
              (**(code **)(*piVar53 + 0x260))(&stack0xfffffba8);
              pCVar25 = ::operator_new(0x10);
              if (pCVar25 == (CRumbleDef *)0x0) {
                pCVar26 = (CAction *)0x0;
              }
              else {
                pCVar26 = (CAction *)CRumbleDef::CRumbleDef(pCVar25);
              }
              CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                        ((CCountedPointer<NUISystem::CAction> *)&puStack_3e0,pCVar26);
              *puStack_3e0 = 0x15;
              if (ppVar48 == (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) {
                pCStack_424 = (CManager *)0x0;
              }
              else {
                pCStack_424 = (CManager *)(ppVar48 + 0x18);
              }
              iVar43 = puStack_3e0[1];
              piVar49 = malloc(0xc);
              if (piVar49 + 2 != (int *)0x0) {
                piVar49[2] = (int)pCStack_424;
              }
              puVar14 = *(undefined4 **)(iVar43 + 4);
              *piVar49 = iVar43;
              piVar49[1] = (int)puVar14;
              *puVar14 = piVar49;
              *(int **)(iVar43 + 4) = piVar49;
              (**(code **)(*piVar52 + 0x108))(&puStack_3e0);
              pCVar3 = *(CManager **)(this_05 + 0xc);
              if (pCVar3 != (CManager *)0x0) {
                *(int *)(pCVar3 + 4) = *(int *)(pCVar3 + 4) + 1;
              }
              iVar43 = *(int *)(pCVar3 + 4);
              cVar44 = *(int *)(pCVar3 + 0x68) != 0;
              *(int *)(pCVar3 + 4) = iVar43 + -1;
              if (iVar43 + -1 == 0) {
                (**(code **)(*(int *)pCVar3 + 4))();
              }
              if (cVar44 != '\0') {
                CCharString::CCharString(aCStack_2cc,"TEXT_GUI_MENU_TAKE_QUEST_BOAST",-1);
                NGameText::CDataBank::GetTextBySymbol
                          ((CDataBank *)this_06,(CCharString *)&local_308);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_2cc,extraout_EDX_53,
                           (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20);
                CCharString::CCharString
                          ((CCharString *)&pCStack_358,"UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",-1
                          );
                CCharString::CCharString((CCharString *)&stack0xfffffb48,"PC_BUTTON_BASTARD",-1);
                CCharString::CCharString((CCharString *)&stack0xfffffb40,"",-1);
                pCVar22 = NUISystem::CManager::CreateMenuEntryComponent
                                    ((CManager *)this_07,&local_308,0,0x3f800000,&pCStack_358);
                CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                          ((CCountedPointer<NUISystem::CAction> *)&pCStack_3c4,(CAction *)pCVar22);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           &pCStack_358,extraout_EDX_54,
                           (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20);
                (**(code **)(*piVar53 + 0x260))();
                (**(code **)(*piStack_3c8 + 0x108))();
                pCVar25 = ::operator_new(0x10);
                if (pCVar25 == (CRumbleDef *)0x0) {
                  pCVar26 = (CAction *)0x0;
                }
                else {
                  pCVar26 = (CAction *)CRumbleDef::CRumbleDef(pCVar25);
                }
                CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                          ((CCountedPointer<NUISystem::CAction> *)&pCStack_3a4,pCVar26);
                *(int *)pCStack_3a4 = 0xcf;
                (**(code **)(*(int *)local_3cc + 0x108))();
                if ((piStack_398 != (int *)0x0) &&
                   (*piStack_398 = *piStack_398 + -1, *piStack_398 == 0)) {
                  (*(code *)piStack_398[1])();
                  operator_delete(piStack_398);
                }
                pCStack_39c = (CComponent *)0x0;
                piStack_398 = (int *)0x0;
                if ((pCStack_3c0 != (CTCInventoryBase *)0x0) &&
                   (*(int *)pCStack_3c0 = *(int *)pCStack_3c0 + -1, *(int *)pCStack_3c0 == 0)) {
                  (**(code **)(pCStack_3c0 + 4))();
                  operator_delete(pCStack_3c0);
                }
                pCStack_3c4 = (CComponent *)0x0;
                pCStack_3c0 = (CTCInventoryBase *)0x0;
                CCharString::~CCharString((CCharString *)&local_308);
              }
              if ((piStack_3c8 != (int *)0x0) &&
                 (*piStack_3c8 = *piStack_3c8 + -1, *piStack_3c8 == 0)) {
                (*(code *)piStack_3c8[1])();
                operator_delete(piStack_3c8);
              }
              local_3cc = (CComponent *)0x0;
              piStack_3c8 = (int *)0x0;
              if ((ppVar51 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) &&
                 (*(int *)ppVar51 = *(int *)ppVar51 + -1, *(int *)ppVar51 == 0)) {
                (**(code **)(ppVar51 + 4))();
                operator_delete(ppVar51);
              }
              if ((pCStack_424 != (CManager *)0x0) &&
                 (*(int *)pCStack_424 = *(int *)pCStack_424 + -1, *(int *)pCStack_424 == 0)) {
                (**(code **)(pCStack_424 + 4))();
                operator_delete(pCStack_424);
              }
              ppStack_428 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
              pCStack_424 = (CManager *)0x0;
              if ((piVar27 != (int *)0x0) && (*piVar27 = *piVar27 + -1, *piVar27 == 0)) {
                (*(code *)piVar27[1])();
                operator_delete(piVar27);
              }
              if ((piVar23 != (int *)0x0) && (*piVar23 = *piVar23 + -1, *piVar23 == 0)) {
                (*(code *)piVar23[1])();
                operator_delete(piVar23);
              }
              pCVar20 = (CCharString *)&local_350;
              goto LAB_0061ddcd;
            }
          }
          (**(code **)(*(int *)this_08 + 0x170))();
          (**(code **)(*(int *)this_08 + 0xd4))();
          (**(code **)(*(int *)this_08 + 0x100))();
          iVar43 = *piVar49;
          CCharString::CCharString((CCharString *)&stack0xfffffb40,"UI_SELECTION_ARROW_RIGHT",-1);
          piVar27 = (int *)(**(code **)(iVar43 + 0xc))();
          if (piVar27 != (int *)0x0) {
            _Memory = malloc(0xc);
            *_Memory = _Memory;
            _Memory[1] = _Memory;
            puVar14 = malloc(0xc);
            *puVar14 = puVar14;
            puVar14[1] = puVar14;
            std::list<CClass*,std::allocator<CClass*>_>::
            _Insert<std::_List_const_iterator<std::_List_val<CClass*,std::allocator<CClass*>_>_>_>
                      ((list<CClass*,std::allocator<CClass*>_> *)&stack0xfffffb48,*puVar14,*_Memory)
            ;
            (**(code **)(*piVar27 + 0xf8))();
            puVar14 = (undefined4 *)*_Memory;
            while (puVar14 != _Memory) {
              puVar4 = (undefined4 *)*puVar14;
              free(puVar14);
              puVar14 = puVar4;
            }
            *_Memory = _Memory;
            _Memory[1] = _Memory;
            free(_Memory);
          }
        }
        else {
          if (param_3 == 1) {
            CCharString::CCharString(aCStack_2e0,"TEXT_GUI_MENU_ZOOMED_REGION",-1);
            NGameText::CDataBank::GetTextBySymbol(pCVar37,aCStack_300);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2e0,
                       extraout_EDX_55,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20);
            CCharString::CCharString
                      ((CCharString *)&local_348,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb48,"PC_BUTTON_BASTARD",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb40,"",-1);
            pCVar18 = NUISystem::CManager::CreateMenuEntryComponent
                                ((CManager *)this_07,aCStack_300,0,0x3f800000,&local_348);
            CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                      ((CCountedPointer<NUISystem::CAction> *)&piStack_380,(CAction *)pCVar18);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_348,
                       extraout_EDX_56,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20);
            (**(code **)(*(int *)pCVar22 + 0x240))();
            if ((piStack_37c != (int *)0x0) && (*piStack_37c = *piStack_37c + -1, *piStack_37c == 0)
               ) {
              (*(code *)piStack_37c[1])();
              operator_delete(piStack_37c);
            }
            piStack_380 = (int *)0x0;
            piStack_37c = (int *)0x0;
            pCVar20 = aCStack_300;
          }
          else {
            if (param_3 != 2) goto LAB_0061ddd2;
            CCharString::CCharString(aCStack_340,"TEXT_GUI_MENU_ZOOMED_REGION",-1);
            NGameText::CDataBank::GetTextBySymbol((CDataBank *)this_06,aCStack_338);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_340,
                       extraout_EDX_57,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20);
            CCharString::CCharString(aCStack_2b4,"UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb48,"PC_BUTTON_BASTARD",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb40,"",-1);
            pCVar18 = NUISystem::CManager::CreateMenuEntryComponent
                                ((CManager *)this_07,aCStack_338,0,0x3f800000,aCStack_2b4);
            CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                      ((CCountedPointer<NUISystem::CAction> *)&piStack_378,(CAction *)pCVar18);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2b4,
                       extraout_EDX_58,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20);
            (**(code **)(*(int *)pCVar22 + 0x240))();
            if ((piStack_374 != (int *)0x0) && (*piStack_374 = *piStack_374 + -1, *piStack_374 == 0)
               ) {
              (*(code *)piStack_374[1])();
              operator_delete(piStack_374);
            }
            piStack_378 = (int *)0x0;
            piStack_374 = (int *)0x0;
            pCVar20 = aCStack_338;
          }
LAB_0061ddcd:
          CCharString::~CCharString(pCVar20);
        }
LAB_0061ddd2:
        piStack_2c8 = (int *)0x0;
        fStack_2c4 = 0.0;
        CTCQuestCard::GetRegionName((CTCQuestCard *)this_05);
        if (0 < aiStack_2f8[0]) {
          puVar14 = (undefined4 *)CRegion::GetNameGraphicOffset(pCStack_388);
          piStack_2c8 = (int *)*puVar14;
          fStack_2c4 = (float)puVar14[1];
        }
        NUISystem::CUIStateDef::CUIStateDef((CUIStateDef *)aCStack_1a4);
        fStack_15c = 444.0 - (float)piStack_2c8;
        iStack_124 = iVar31 + 8;
        fStack_158 = 186.0 - fStack_2c4;
        local_13c = 0x3e4ccccd;
        NUISystem::CUIStateDef::CUIStateDef((CUIStateDef *)appuStack_120,(CUIStateDef *)aCStack_1a4)
        ;
        iStack_1cc = iStack_124;
        NUISystem::CUIState::CUIState(aCStack_1c8,(CUIStateDef *)appuStack_120);
        (**(code **)(*(int *)pCVar24 + 0x148))();
        (**(code **)(*(int *)pCVar24 + 0x148))();
        FUN_0042d2c6();
        local_404 = (CManager *)(iVar31 + 1);
        if (pvStack_30 != (void *)0x0) {
          free(pvStack_30);
        }
        if (pvStack_73 != (void *)0x0) {
          free(pvStack_73);
        }
        appuStack_9c[0] = &PTR__vector_deleting_destructor__01230ba0;
        CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)appuStack_9c);
        if (pvStack_b4 != (void *)0x0) {
          free(pvStack_b4);
        }
        if (pvStack_f7 != (void *)0x0) {
          free(pvStack_f7);
        }
        appuStack_120[0] = &PTR__vector_deleting_destructor__01230ba0;
        CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)appuStack_120);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_204,extraout_EDX_59,ppVar33);
        piVar27 = piStack_234;
        if ((piStack_234 != (int *)0x0) && (*piStack_234 = *piStack_234 + -1, *piStack_234 == 0)) {
          (*(code *)piStack_234[1])();
          operator_delete(piVar27);
        }
        if ((apiStack_390[0] != (int *)0x0) &&
           (*apiStack_390[0] = *apiStack_390[0] + -1, *apiStack_390[0] == 0)) {
          (*(code *)apiStack_390[0][1])();
          operator_delete(apiStack_390[0]);
        }
        uStack_394 = 0;
        apiStack_390[0] = (int *)0x0;
        CCharString::~CCharString((CCharString *)&stack0xfffffbe8);
        ppVar36 = extraout_EDX_60;
        if ((piStack_3b0 != (int *)0x0) && (*piStack_3b0 = *piStack_3b0 + -1, *piStack_3b0 == 0)) {
          (*(code *)piStack_3b0[1])();
          operator_delete(piStack_3b0);
          ppVar36 = extraout_EDX_61;
        }
        piStack_3b4 = (int *)0x0;
        piStack_3b0 = (int *)0x0;
        if ((piStack_378 != (int *)0x0) && (*piStack_378 = *piStack_378 + -1, *piStack_378 == 0)) {
          (*(code *)piStack_378[1])();
          operator_delete(piStack_378);
          ppVar36 = extraout_EDX_62;
        }
        pCVar6 = local_3d4;
        piStack_37c = (int *)0x0;
        piStack_378 = (int *)0x0;
        if ((local_3d4 != (CTCInventoryQuests *)0x0) &&
           (*(int *)local_3d4 = *(int *)local_3d4 + -1, *(int *)local_3d4 == 0)) {
          (**(code **)(local_3d4 + 4))();
          operator_delete(pCVar6);
          ppVar36 = extraout_EDX_63;
        }
        if ((piStack_3a8 != (int *)0x0) && (*piStack_3a8 = *piStack_3a8 + -1, *piStack_3a8 == 0)) {
          (*(code *)piStack_3a8[1])();
          operator_delete(piStack_3a8);
          ppVar36 = extraout_EDX_64;
        }
        uStack_3ac = 0;
        piStack_3a8 = (int *)0x0;
        if ((uStack_40c != (CComponent *)0x0) &&
           (*(int *)uStack_40c = *(int *)uStack_40c + -1, *(int *)uStack_40c == 0)) {
          (**(code **)(uStack_40c + 4))();
          operator_delete(uStack_40c);
          ppVar36 = extraout_EDX_65;
        }
        unaff_EBX = (CManager *)0x0;
        uStack_40c = (CComponent *)0x0;
        if ((piStack_398 != (int *)0x0) && (*piStack_398 = *piStack_398 + -1, *piStack_398 == 0)) {
          (*(code *)piStack_398[1])();
          operator_delete(piStack_398);
          ppVar36 = extraout_EDX_66;
        }
        pCStack_39c = (CComponent *)0x0;
        piStack_398 = (int *)0x0;
        if ((piStack_3c8 != (int *)0x0) && (*piStack_3c8 = *piStack_3c8 + -1, *piStack_3c8 == 0)) {
          (*(code *)piStack_3c8[1])();
          operator_delete(piStack_3c8);
          ppVar36 = extraout_EDX_67;
        }
        local_3cc = (CComponent *)0x0;
        piStack_3c8 = (int *)0x0;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_338,
                   ppVar36,ppVar33);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_32c,
                   extraout_EDX_68,ppVar33);
        if ((piStack_3f4 != (int *)0x0) && (*piStack_3f4 = *piStack_3f4 + -1, *piStack_3f4 == 0)) {
          (*(code *)piStack_3f4[1])();
          operator_delete(piStack_3f4);
        }
        pCStack_3f8 = (CComponent *)0x0;
        piStack_3f4 = (int *)0x0;
        CCharString::~CCharString((CCharString *)&piStack_310);
        pCStack_324 = pCStack_324 + 1;
      } while (pCStack_324 != (CViewBase *)local_3f0);
    }
    pCStack_400 = pCStack_400 + 1;
    if (pCStack_400 == (CAIStateGroup_SummonerCharge *)0x3) {
      if (param_3 == 0) {
        ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
      }
      else {
        ppVar33 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                  ((*(int *)(param_1 + 4) - *(int *)param_1 >> 2) + -1);
      }
      (**(code **)(*(int *)param_2 + 0x278))();
      iVar31 = *(int *)(local_3f0 + 4);
      if ((*(uint *)(iVar31 + 0x20) & 0x8000000) != 0) {
        piStack_3f4 = (int *)0x1b;
        piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                         ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                       *)(iVar31 + 0x44),(ETCInterfaceType *)&piStack_3f4);
        if ((piVar12 == *(int **)(iVar31 + 0x48)) || (0x1b < *piVar12)) {
          piVar12 = *(int **)(iVar31 + 0x48);
        }
        local_408 = (CManager *)piVar12[1];
        if (local_408 != (CManager *)0x0) {
          uVar32 = 0;
          pCStack_3dc = (CQuestManager *)0x0;
          pCStack_3dc = malloc(0x14);
          iStack_3d8 = 0;
          *pCStack_3dc = (CQuestManager)0x0;
          *(int *)(pCStack_3dc + 4) = 0;
          *(CQuestManager **)(pCStack_3dc + 8) = pCStack_3dc;
          *(CQuestManager **)(pCStack_3dc + 0xc) = pCStack_3dc;
          pCStack_35c = (CAIStateGroup_SummonerCharge *)0x0;
          pCStack_358 = (CAIStateGroup_SummonerCharge *)0x0;
          pCStack_354 = (CRegion *)0x0;
          CTCInventoryMap::GetRegionsToDisplay
                    ((CTCInventoryMap *)local_408,
                     (set<long,std::less<long>,std::allocator<long>_> *)&pCStack_3dc);
          iVar31 = *(int *)param_1;
          if (*(int *)(param_1 + 4) - iVar31 >> 2 != 0) {
            do {
              iVar31 = *(int *)(iVar31 + uVar32 * 4);
              pCVar54 = (CTCQuestCard *)0x0;
              if ((*(uint *)(iVar31 + 0x2c) & 0x1000) != 0) {
                piStack_3f4 = (int *)0x6c;
                piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                 ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                               *)(iVar31 + 0x44),(ETCInterfaceType *)&piStack_3f4);
                if ((piVar12 == *(int **)(iVar31 + 0x48)) || (0x6c < *piVar12)) {
                  piVar12 = *(int **)(iVar31 + 0x48);
                }
                pCVar54 = (CTCQuestCard *)piVar12[1];
              }
              pCVar28 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)local_3f0);
              pCVar20 = (CCharString *)CTCQuestCard::GetRegionName(pCVar54);
              pCVar16 = (CWorldMap *)(**(code **)(*(int *)pCVar28 + 0x34))();
              piVar12 = (int *)CWorldMap::GetRegionNumberFromName(pCVar16,pCVar20);
              piStack_3f4 = piVar12;
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        (aaStack_33c,extraout_EDX_69,ppVar33);
              if (0 < (int)piVar12) {
                FUN_00512da0();
              }
              iVar31 = *(int *)param_1;
              uVar32 = uVar32 + 1;
            } while (uVar32 < (uint)(*(int *)(param_1 + 4) - iVar31 >> 2));
          }
          pCVar11 = local_3f0;
          pCVar40 = *(CQuestManager **)(pCStack_3dc + 8);
          if (pCVar40 != pCStack_3dc) {
            do {
              pCVar28 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar11);
              iVar31 = *(int *)(pCVar40 + 0x10);
              pCVar29 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar28 + 0x34))();
              pCVar30 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar29,iVar31);
              if (pCVar30[0x54] != (CCategory)0x0) {
                pCVar28 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar11);
                iVar31 = *(int *)(pCVar40 + 0x10);
                pCVar29 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar28 + 0x34))();
                pCVar30 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar29,iVar31);
                if (pCStack_358 == (CAIStateGroup_SummonerCharge *)pCStack_354) {
                  FUN_0051cf10();
                }
                else {
                  if (pCStack_358 != (CAIStateGroup_SummonerCharge *)0x0) {
                    CRegion::CRegion((CRegion *)pCStack_358,(CRegion *)pCVar30);
                  }
                  pCStack_358 = pCStack_358 + 0x58;
                }
              }
              pCVar35 = *(CQuestManager **)(pCVar40 + 0xc);
              if (pCVar35 == (CQuestManager *)0x0) {
                pCVar35 = *(CQuestManager **)(pCVar40 + 4);
                if (pCVar40 == *(CQuestManager **)(pCVar35 + 0xc)) {
                  do {
                    pCVar40 = pCVar35;
                    pCVar35 = *(CQuestManager **)(pCVar40 + 4);
                  } while (pCVar40 == *(CQuestManager **)(pCVar35 + 0xc));
                }
                if (*(CQuestManager **)(pCVar40 + 0xc) != pCVar35) {
                  pCVar40 = pCVar35;
                }
              }
              else {
                for (pCVar5 = *(CQuestManager **)(pCVar35 + 8); pCVar40 = pCVar35,
                    pCVar5 != (CQuestManager *)0x0; pCVar5 = *(CQuestManager **)(pCVar5 + 8)) {
                  pCVar35 = pCVar5;
                }
              }
            } while (pCVar40 != pCStack_3dc);
          }
          CTCInventoryMap::CreateMapMarkers
                    ((CTCInventoryMap *)local_408,(CComponent *)local_3ec,
                     (vector<CRegion,std::allocator<CRegion>_> *)&pCStack_35c);
          pCVar7 = pCStack_358;
          for (this_04 = pCStack_35c; this_04 != pCVar7; this_04 = this_04 + 0x58) {
            CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(this_04);
          }
          if (pCStack_35c != (CAIStateGroup_SummonerCharge *)0x0) {
            free(pCStack_35c);
          }
          if (iStack_3d8 != 0) {
            LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
                      ((LTextTreeWalkThrough *)&pCStack_3dc,*(LTextBinNode **)(pCStack_3dc + 4));
            *(CQuestManager **)(pCStack_3dc + 8) = pCStack_3dc;
            *(int *)(pCStack_3dc + 4) = 0;
            *(CQuestManager **)(pCStack_3dc + 0xc) = pCStack_3dc;
            iStack_3d8 = 0;
          }
          if (pCStack_3dc != (CQuestManager *)0x0) {
            free(pCStack_3dc);
          }
        }
      }
      CCharString::CCharString((CCharString *)&local_408,"UI_MAP_VIEWPORT",-1);
      pCVar22 = NUISystem::CManager::CreateComponent
                          ((CManager *)uStack_40c,(CCharString *)&local_408,false);
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&piStack_310,(CAction *)pCVar22);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_408,
                 extraout_EDX_70,ppVar33);
      (**(code **)(*piStack_310 + 0xec))();
      (**(code **)(*(int *)param_2 + 0x244))();
      if ((piStack_314 != (int *)0x0) && (*piStack_314 = *piStack_314 + -1, *piStack_314 == 0)) {
        (*(code *)piStack_314[1])();
        operator_delete(piStack_314);
      }
      uStack_318 = 0;
      piStack_314 = (int *)0x0;
      if ((piStack_378 != (int *)0x0) && (*piStack_378 = *piStack_378 + -1, *piStack_378 == 0)) {
        (*(code *)piStack_378[1])();
        operator_delete(piStack_378);
      }
      piStack_37c = (int *)0x0;
      piStack_378 = (int *)0x0;
      if ((local_3f0 != (CFrontEndManager *)0x0) &&
         (*(int *)local_3f0 = *(int *)local_3f0 + -1, *(int *)local_3f0 == 0)) {
        (**(code **)(local_3f0 + 4))();
        operator_delete(local_3f0);
      }
      piStack_3f4 = (int *)0x0;
      local_3f0 = (CFrontEndManager *)0x0;
      if (local_408 != (CManager *)0x0) {
        pCVar3 = local_408 + 4;
        *(int *)pCVar3 = *(int *)pCVar3 + -1;
        if (*(int *)pCVar3 == 0) {
          (**(code **)(*(int *)local_408 + 4))();
        }
      }
      if ((piStack_368 != (int *)0x0) && (*piStack_368 = *piStack_368 + -1, *piStack_368 == 0)) {
        (*(code *)piStack_368[1])();
        operator_delete(piStack_368);
      }
      pCStack_36c = (CComponent *)0x0;
      piStack_368 = (int *)0x0;
      if ((piStack_370 != (int *)0x0) && (*piStack_370 = *piStack_370 + -1, *piStack_370 == 0)) {
        (*(code *)piStack_370[1])();
        operator_delete(piStack_370);
      }
      piStack_374 = (int *)0x0;
      piStack_370 = (int *)0x0;
      puVar39 = local_188;
      iVar31 = 3;
      do {
        puVar14 = (undefined4 *)(puVar39 + -0xc);
        puVar39 = puVar39 + -0xc;
        if ((void *)*puVar14 != (void *)0x0) {
          free((void *)*puVar14);
        }
        iVar31 = iVar31 + -1;
      } while (iVar31 != 0);
      return;
    }
  } while( true );
}



//=== ConstructQuestListPC @ 0061e6d0 ===

/* WARNING: Restarted to delay deadcode elimination for space: stack */
/* [bsim sim=0.5711427836441418 <- ego_r]
   public: void __thiscall CTCInventoryQuests::ConstructQuestListPC(class NUISystem::CComponent
   *,class std::vector<class CThing *,class std::allocator<class CThing *> > const &,class
   NUISystem::CList *,enum QuestScreenType,class NUISystem::CList *) */

void __thiscall
CTCInventoryQuests::ConstructQuestListPC
          (CTCInventoryQuests *this,CComponent *param_1,
          vector<CThing*,std::allocator<CThing*>_> *param_2,CList *param_3,QuestScreenType param_4,
          CList *param_5)

{
  ushort uVar1;
  CBaseIntelligentPointer *this_00;
  int iVar2;
  undefined1 *puVar3;
  CManager *pCVar4;
  CAIStateGroup_SummonerCharge *pCVar5;
  bool bVar6;
  CRegion **ppCVar7;
  int *piVar8;
  COptimisedPrimitive *this_01;
  long lVar9;
  CDefinitionManager *this_02;
  CFrontEndManager *pCVar10;
  float *pfVar11;
  undefined4 *puVar12;
  void *pvVar13;
  CWorldMap *pCVar14;
  CBaseObject *pCVar15;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined3 extraout_var_01;
  CWideString *pCVar16;
  CCharString *pCVar17;
  CComponent *pCVar18;
  int *piVar19;
  undefined4 *puVar20;
  int *piVar21;
  CAction *pCVar22;
  CRumbleDef *this_03;
  CComponent *pCVar23;
  CFrontEndManager *this_04;
  CDisplayEngine *pCVar24;
  CTCInventoryBase *pCVar25;
  CCategory *pCVar26;
  undefined1 *puVar27;
  int iVar28;
  uint uVar29;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  CRegion *extraout_ECX_00;
  CComponent *pCVar30;
  CTCInventoryMap *pCVar31;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar32;
  int *extraout_ECX_01;
  CManager *extraout_ECX_02;
  int extraout_ECX_03;
  CTCQuestCard *extraout_ECX_04;
  undefined1 *puVar33;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_11;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_13;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_14;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_18;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_20;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_21;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_22;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_26;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_27;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_28;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_31;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_32;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_33;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_35;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_36;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_37;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_38;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_39;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_40;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_41;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_42;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_43;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_44;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_45;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_46;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_47;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_48;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_49;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_50;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_51;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_52;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_53;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_54;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_55;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_56;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_57;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_58;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_59;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_60;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_61;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_62;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_63;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_64;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_65;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_66;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar34;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_67;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_68;
  int *unaff_EBX;
  CManager *unaff_ESI;
  CRegion *pCVar35;
  int iVar36;
  CAIStateGroup_SummonerCharge *this_05;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int iVar37;
  CQuestManager *pCVar38;
  int iVar39;
  CDataBank *pCVar40;
  char cVar41;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar42;
  CManager *pCVar43;
  CTCQuestCard *pCVar44;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar45;
  CDataBank *this_06;
  CManager *pCVar46;
  CManager *pCStack_4a0;
  CBaseIntelligentPointer **ppCVar47;
  CDataBank *pCVar48;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar49;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar50;
  CBaseIntelligentPointer *pCStack_470;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar51;
  CRegion *pCStack_444;
  CRegion **ppCStack_440;
  char *pcVar52;
  CTCQuestCard *pCVar53;
  CManager **ppCVar54;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar55;
  int *piVar56;
  int *piStack_424;
  undefined4 uStack_420;
  CManager *local_41c;
  CManager *pCStack_418;
  CComponent *local_414;
  CComponent *pCStack_410;
  int *piStack_40c;
  CComponent *pCStack_408;
  CManager *local_404;
  CFrontEndManager *local_400;
  CComponent *local_3fc;
  int *piStack_3f8;
  int *apiStack_3f4 [3];
  CQuestManager *pCStack_3e8;
  CComponent *local_3e4;
  CComponent *pCStack_3e0;
  int *piStack_3dc;
  undefined4 *puStack_3c8;
  int *piStack_3c4;
  int *piStack_3c0;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppStack_3bc;
  CComponent *pCStack_3b8;
  int *piStack_3b4;
  CComponent *pCStack_3b0;
  int *piStack_3ac;
  int *piStack_3a8;
  undefined4 uStack_3a4;
  int *piStack_3a0;
  undefined4 uStack_39c;
  undefined4 *puStack_398;
  int *piStack_394;
  float fStack_390;
  CRegion *pCStack_38c;
  CCharString aCStack_388 [4];
  CRegion *pCStack_384;
  CCategory *pCStack_380;
  CBaseObject *apCStack_37c [2];
  CCharString aCStack_374 [4];
  int iStack_370;
  int *piStack_36c;
  int *piStack_368;
  CCharString aCStack_364 [4];
  int iStack_360;
  CAIStateGroup_SummonerCharge *local_35c;
  CAIStateGroup_SummonerCharge *pCStack_358;
  CRegion *pCStack_354;
  undefined4 uStack_350;
  int *piStack_34c;
  int *piStack_348;
  CBaseIntelligentPointer aCStack_33c [24];
  int local_324 [2];
  CCharString aCStack_31c [4];
  CCharString aCStack_318 [4];
  CCharString aCStack_314 [4];
  CComponent *pCStack_310;
  int iStack_30c;
  CCharString aCStack_308 [4];
  CWideString aCStack_304 [8];
  CCharString aCStack_2fc [4];
  CCharString aCStack_2f8 [4];
  CCharString aCStack_2f4 [4];
  CCharString aCStack_2f0 [4];
  int *apiStack_2ec [2];
  CCharString aCStack_2e4 [4];
  CWideString aCStack_2e0 [4];
  CCharString aCStack_2dc [16];
  int *piStack_2cc;
  float fStack_2c8;
  float fStack_2c4;
  CCharString aCStack_2c0 [4];
  CCharString aCStack_2bc [8];
  CCharString aCStack_2b4 [16];
  CCharString aCStack_2a4 [8];
  CCharString aCStack_29c [12];
  CCharString aCStack_290 [4];
  CCharString aCStack_28c [4];
  CIDrawEnvironment *apCStack_288 [2];
  undefined **ppuStack_280;
  CCharString aCStack_27c [4];
  CCharString aCStack_278 [4];
  CCharString aCStack_274 [4];
  CCharString aCStack_270 [4];
  CCharString aCStack_26c [4];
  CCharString aCStack_268 [4];
  CCharString aCStack_264 [4];
  CComponent *apCStack_260 [2];
  CCharString aCStack_258 [4];
  CCharString aCStack_254 [4];
  CCharString aCStack_250 [4];
  CCharString aCStack_24c [4];
  int *piStack_248;
  undefined4 uStack_244;
  CCharString aCStack_240 [4];
  CCharString aCStack_23c [8];
  CCharString aCStack_234 [12];
  CCharString aCStack_228 [4];
  CCharString aCStack_224 [8];
  CCharString aCStack_21c [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_214 [8];
  int iStack_20c;
  CCharString aCStack_208 [8];
  CCharString aCStack_200 [12];
  CCharString aCStack_1f4 [8];
  CCharString aCStack_1ec [8];
  CUIStateDef aCStack_1e4 [36];
  CCharString aCStack_1c0 [4];
  int aiStack_1bc [6];
  undefined4 uStack_1a4;
  undefined4 local_1a0;
  CRegion *local_19c;
  CRegion *local_198 [7];
  CBaseObject *pCStack_17c;
  int *piStack_178;
  undefined4 uStack_164;
  undefined **ppuStack_160;
  undefined4 uStack_15c;
  undefined4 uStack_158;
  CManager *pCStack_154;
  CUIState aCStack_150 [25];
  void *pvStack_137;
  CManager *pCStack_12c;
  CUIStateDef aCStack_128 [52];
  void *pvStack_f4;
  undefined **appuStack_a4 [10];
  void *pvStack_7b;
  void *pvStack_38;
  
  local_41c = (CManager *)0x0;
  ppCVar7 = local_198;
  iVar28 = 3;
  do {
    ppCVar7[-2] = (CRegion *)0x0;
    ppCVar7[-1] = (CRegion *)0x0;
    *ppCVar7 = (CRegion *)0x0;
    ppCVar7 = ppCVar7 + 3;
    iVar28 = iVar28 + -1;
  } while (iVar28 != 0);
  local_3e4 = (CComponent *)this;
  local_400 = NUISystem::CFrontEndManager::GetInstance();
  iVar28 = *(int *)param_2;
  local_414 = (CComponent *)0x0;
  pCVar35 = local_19c;
  if ((*(int *)(param_2 + 4) - iVar28 & 0xfffffffcU) != 0) {
    do {
      iVar39 = *(int *)(iVar28 + (int)local_414 * 4);
      uVar29 = (uint)uStack_420 & 0xffffff;
      if ((*(uint *)(iVar39 + 0x2c) >> 0xc & 1) != 0) {
        local_3fc = (CComponent *)&DAT_0000006c;
        piVar8 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                        ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                      *)(iVar39 + 0x44),(ETCInterfaceType *)&local_3fc);
        if ((piVar8 == *(int **)(iVar39 + 0x48)) || (0x6c < *piVar8)) {
          piVar8 = *(int **)(iVar39 + 0x48);
        }
        pCVar53 = (CTCQuestCard *)piVar8[1];
        if (param_4 == 0) {
          bVar6 = CTCQuestCard::IsCore(pCVar53);
          if (bVar6) {
            ppCVar7 = (CRegion **)(*(int *)param_2 + (int)local_414 * 4);
            if (pCVar35 == local_198[0]) goto LAB_0061e8bc;
            if (pCVar35 != (CRegion *)0x0) {
              *(CRegion **)pCVar35 = *ppCVar7;
            }
            local_19c = pCVar35 + 4;
            pCVar35 = local_19c;
          }
          else {
            bVar6 = CTCQuestCard::IsOptional(pCVar53);
            if (bVar6) {
              local_41c = (CManager *)((uint)local_41c | 1);
              local_404 = *(CManager **)(pCVar53 + 0xc);
              if (local_404 != (CManager *)0x0) {
                *(int *)(local_404 + 4) = *(int *)(local_404 + 4) + 1;
              }
              if (local_404[0x59] != (CManager)0x0) goto LAB_0061e7f3;
              uStack_420 = (CManager *)CONCAT13(1,(undefined3)uStack_420);
            }
            else {
LAB_0061e7f3:
              uStack_420 = (CManager *)((uint)uStack_420 & 0xffffff);
            }
            if ((((uint)local_41c & 1) != 0) &&
               (local_41c = (CManager *)((uint)local_41c & 0xfffffffe), local_404 != (CManager *)0x0
               )) {
              pCVar43 = local_404 + 4;
              *(int *)pCVar43 = *(int *)pCVar43 + -1;
              if (*(int *)pCVar43 == 0) {
                (**(code **)(*(int *)local_404 + 4))();
              }
              local_404 = (CManager *)0x0;
            }
            uVar29 = (uint)uStack_420;
            if (uStack_420._3_1_ == '\0') {
              piVar8 = *(int **)(pCVar53 + 0xc);
              if (piVar8 != (int *)0x0) {
                piVar8[1] = piVar8[1] + 1;
              }
              iVar28 = piVar8[1];
              cVar41 = *(char *)((int)piVar8 + 0x59);
              piVar8[1] = iVar28 + -1;
              if (iVar28 + -1 == 0) {
                (**(code **)(*piVar8 + 4))();
              }
              if (cVar41 == '\0') {
                FUN_00436c80();
                pCVar35 = local_19c;
              }
              else {
                FUN_00436c80();
                pCVar35 = local_19c;
              }
            }
            else {
              FUN_00436c80();
              pCVar35 = local_19c;
            }
          }
        }
        else {
          ppCVar7 = (CRegion **)(iVar28 + (int)local_414 * 4);
          if (pCVar35 == local_198[0]) {
LAB_0061e8bc:
            ppCStack_440 = ppCVar7;
            pCStack_444 = pCVar35;
            FUN_00436ac0();
            pCVar35 = local_19c;
          }
          else {
            if (pCVar35 != (CRegion *)0x0) {
              *(CRegion **)pCVar35 = *ppCVar7;
            }
            local_19c = pCVar35 + 4;
            pCVar35 = local_19c;
          }
        }
      }
      uStack_420 = (CManager *)uVar29;
      iVar28 = *(int *)param_2;
      local_414 = (CComponent *)((int)local_414 + 1);
    } while (local_414 != (CComponent *)(*(int *)(param_2 + 4) - iVar28 >> 2));
  }
  local_35c = (CAIStateGroup_SummonerCharge *)0x0;
  CCharString::CCharString((CCharString *)&local_404,"UI_MISC_THINGS_DEF",-1);
  ppCVar54 = &local_404;
  this_01 = (COptimisedPrimitive *)GFGetPreMainMemoryUsed();
  lVar9 = QuadricOptimiserInternals::COptimisedPrimitive::ActualToUniqueIndex
                    (this_01,(ulong)ppCVar54);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_404,
             extraout_EDX,unaff_EDI);
  pCVar55 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_35c;
  this_02 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_02,lVar9,pCVar55);
  local_414 = *(CComponent **)(DAT_013b86a0 + 0x14);
  iVar28 = *(int *)(local_3e4 + 4);
  uVar29 = *(uint *)(iVar28 + 0x20) >> 4;
  ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)(uVar29 & 0xffffff01);
  local_324[0] = 0;
  uStack_420 = (CManager *)(CONCAT13((char)uVar29,(undefined3)uStack_420) & 0x1ffffff);
  if ((uVar29 & 1) != 0) {
    local_3fc = (CComponent *)&DAT_00000004;
    piVar8 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar28 + 0x44),(ETCInterfaceType *)&local_3fc);
    if ((piVar8 == *(int **)(iVar28 + 0x48)) || (4 < *piVar8)) {
      piVar8 = *(int **)(iVar28 + 0x48);
    }
    local_324[0] = piVar8[1];
    ppVar51 = extraout_ECX;
  }
  if (param_5 == (CList *)0x0) {
    param_5 = param_3;
  }
  iVar28 = *(int *)param_5;
  if (param_4 == 0) {
    pcVar52 = "PC_UI_MAP_FRAME";
  }
  else {
    pcVar52 = "PC_UI_FRAME_QUESTS";
  }
  ppCStack_440 = (CRegion **)0x61e9ba;
  CCharString::CCharString((CCharString *)&stack0xfffffbcc,pcVar52,-1);
  uStack_350 = (**(code **)(iVar28 + 0xc))();
  ppCStack_440 = (CRegion **)0x61e9d6;
  CCharString::CCharString((CCharString *)&pCStack_408,"PC_WORLD_MAP",-1);
  ppCStack_440 = (CRegion **)0x61e9e5;
  local_3e4 = NUISystem::CManager::CreateComponent(local_404,(CCharString *)&pCStack_408,false);
  ppVar34 = extraout_EDX_00;
  if ((local_3e4 == (CComponent *)0x0) ||
     (pCStack_3e0 = ::operator_new(0xc), ppVar34 = extraout_EDX_01, pCStack_3e0 == (CComponent *)0x0
     )) {
    pCStack_3e0 = (CComponent *)0x0;
  }
  else {
    *(undefined4 *)pCStack_3e0 = 1;
    *(code **)((int)pCStack_3e0 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
    *(CComponent **)((int)pCStack_3e0 + 8) = local_3e4;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_408,ppVar34,
             ppVar51);
  pCVar30 = local_3e4;
  piStack_368 = *(int **)(iStack_360 + 0x1fc);
  piStack_36c = *(int **)(iStack_360 + 0x1f8);
  apiStack_2ec[0] = piStack_368;
  piStack_34c = piStack_36c;
  if (DAT_013b8768 != '\0') {
    piStack_34c = (int *)(((float)piStack_36c / DAT_01375cd4) * DAT_013b876c);
    apiStack_2ec[0] = (int *)(((float)piStack_368 / DAT_01375cd8) * DAT_013b8770);
  }
  piStack_348 = apiStack_2ec[0];
  pCVar10 = NUISystem::CFrontEndManager::GetInstance();
  iVar28 = NUISystem::CManager::GetUIScale((CManager *)pCVar10);
  piStack_348 = (int *)(*(float *)(iVar28 + 4) * *(float *)(iStack_360 + 0x200) + (float)piStack_348
                       );
  pCVar10 = NUISystem::CFrontEndManager::GetInstance();
  pfVar11 = (float *)NUISystem::CManager::GetUIScale((CManager *)pCVar10);
  piStack_34c = (int *)(*(float *)(iStack_360 + 0xe8) * *pfVar11 + (float)piStack_34c);
  pCVar10 = NUISystem::CFrontEndManager::GetInstance();
  iVar28 = NUISystem::CManager::GetUIScale((CManager *)pCVar10);
  uStack_15c = 0x3e4ccccd;
  uStack_158 = 0x3e4ccccd;
  piStack_348 = (int *)(*(float *)(iVar28 + 4) * *(float *)(iStack_360 + 0xec) + (float)piStack_348)
  ;
  uStack_164 = 0x3fc00000;
  ppuStack_160 = (undefined **)0x3fc00000;
  (**(code **)(*(int *)pCVar30 + 0x240))();
  ppCStack_440 = (CRegion **)0x61eb6d;
  (**(code **)(*(int *)pCVar30 + 0x23c))();
  ppCStack_440 = &pCStack_354;
  pCStack_444 = (CRegion *)0x61eb7f;
  (**(code **)(*(int *)pCVar30 + 0x244))();
  pCStack_444 = extraout_ECX_00;
  if (param_1 == (CComponent *)0x0) {
    iVar28 = *(int *)param_5;
    CCharString::CCharString((CCharString *)&pCStack_444,"PC_DESCRIPTION_VIEWPORT",-1);
    uStack_244 = (**(code **)(iVar28 + 0xc))();
  }
  else {
    iVar28 = *(int *)param_1;
    CCharString::CCharString((CCharString *)&pCStack_444,"PC_DESCRIPTION_VIEWPORT",-1);
    uStack_244 = (**(code **)(iVar28 + 0xc))();
  }
  pCVar30 = (CComponent *)(*(int *)(param_2 + 4) - *(int *)param_2 >> 2);
  (**(code **)(*(int *)param_3 + 0x270))();
  CCharString::CCharString((CCharString *)&local_41c,"PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT",-1);
  pCStack_3b8 = NUISystem::CManager::CreateComponent(pCStack_418,(CCharString *)&local_41c,false);
  ppVar34 = extraout_EDX_02;
  if ((pCStack_3b8 == (CComponent *)0x0) ||
     (piStack_3b4 = ::operator_new(0xc), ppVar34 = extraout_EDX_03, piStack_3b4 == (int *)0x0)) {
    piStack_3b4 = (int *)0x0;
  }
  else {
    *piStack_3b4 = 1;
    piStack_3b4[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
    piStack_3b4[2] = (int)pCStack_3b8;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_41c,ppVar34,
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar30);
  (**(code **)(*(int *)param_3 + 0x244))();
  ppVar51 = ppStack_3bc;
  (**(code **)(*piStack_368 + 0x1f0))();
  local_41c = (CManager *)0x0;
  piStack_424 = (int *)0x0;
  do {
    pCStack_408 = (CComponent *)
                  (aiStack_1bc[(int)piStack_424 * 3] -
                   *(int *)(aCStack_1c0 + (int)piStack_424 * 0xc) >> 2);
    pCStack_310 = (CComponent *)0x0;
    iStack_20c = *(int *)(aCStack_1c0 + (int)piStack_424 * 0xc);
    if (pCStack_408 != (CComponent *)0x0) {
      do {
        iStack_370 = *(int *)(iStack_20c + (int)pCStack_310 * 4);
        uVar29 = *(uint *)(iStack_370 + 0x2c) >> 0xc;
        pCVar53 = (CTCQuestCard *)0x0;
        ppCStack_440 = (CRegion **)(CONCAT13((char)uVar29,ppCStack_440._0_3_) & 0x1ffffff);
        if ((uVar29 & 1) != 0) {
          piVar56 = *(int **)(iStack_370 + 0x48);
          piVar8 = *(int **)(iStack_370 + 0x44);
          iVar28 = (int)piVar56 - (int)piVar8 >> 3;
          while (iVar39 = iVar28, 0 < iVar39) {
            iVar28 = iVar39 / 2;
            if (piVar8[iVar28 * 2] < 0x6c) {
              piVar8 = piVar8 + iVar28 * 2 + 2;
              iVar28 = iVar39 + (-1 - iVar28);
            }
          }
          if ((piVar8 == piVar56) || (0x6c < *piVar8)) {
            piVar8 = piVar56;
          }
          pCVar53 = (CTCQuestCard *)piVar8[1];
        }
        pCVar44 = pCVar53;
        CTCQuestCard::GetQuestName(pCVar53);
        FUN_0099bff0();
        CCharString::~CCharString((CCharString *)local_198);
        CCharString::CCharString(aCStack_27c,"PC_MENU_ENTRY_BRANCH_TEMPLATE",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffbac,"PC_BUTTON_BASTARD",-1);
        CCharString::CCharString((CCharString *)&stack0xfffffba4,"",-1);
        pCStack_470 = aCStack_33c;
        pCStack_418 = (CManager *)NUISystem::CManager::CreateMenuEntryComponent(uStack_420);
        ppVar34 = extraout_EDX_04;
        if ((pCStack_418 == (CManager *)0x0) ||
           (local_414 = ::operator_new(0xc), ppVar34 = extraout_EDX_05,
           local_414 == (CComponent *)0x0)) {
          local_414 = (CComponent *)0x0;
        }
        else {
          *(int *)local_414 = 1;
          *(code **)((int)local_414 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CManager **)((int)local_414 + 8) = pCStack_418;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_27c,
                   ppVar34,ppVar51);
        if (param_4 == 0) {
          puVar12 = ::operator_new(0x10);
          if (puVar12 == (undefined4 *)0x0) {
            puVar12 = (undefined4 *)0x0;
          }
          else {
            *puVar12 = 0;
            puVar12[1] = 0;
            pvVar13 = malloc(0xc);
            *(void **)pvVar13 = pvVar13;
            *(void **)((int)pvVar13 + 4) = pvVar13;
            puVar12[1] = pvVar13;
            puVar12[2] = 0;
            pvVar13 = malloc(0xc);
            *(void **)pvVar13 = pvVar13;
            *(void **)((int)pvVar13 + 4) = pvVar13;
            puVar12[2] = pvVar13;
            *(undefined1 *)(puVar12 + 3) = 1;
          }
          puStack_398 = puVar12;
          if ((puVar12 == (undefined4 *)0x0) ||
             (piStack_394 = ::operator_new(0xc), piStack_394 == (int *)0x0)) {
            piStack_394 = (int *)0x0;
          }
          else {
            *piStack_394 = 1;
            piStack_394[1] = (int)CTCMapwho::OnDie;
            piStack_394[2] = (int)puStack_398;
          }
          *puStack_398 = 0x3c;
          (**(code **)(*(int *)pCStack_418 + 0x110))();
          if ((piStack_394 != (int *)0x0) && (*piStack_394 = *piStack_394 + -1, *piStack_394 == 0))
          {
            (*(code *)piStack_394[1])();
            operator_delete(piStack_394);
          }
          puStack_398 = (undefined4 *)0x0;
          piStack_394 = (int *)0x0;
        }
        pCVar14 = (CWorldMap *)(**(code **)(**(int **)(DAT_013b86a0 + 0x24) + 0x30))();
        CTCQuestCard::GetRegionName(pCVar53);
        apCStack_288[0] =
             (CIDrawEnvironment *)
             CWorldMap::GetRegionNumberFromName(pCVar14,(CCharString *)local_324);
        if (apCStack_288[0] == (CIDrawEnvironment *)0x0) {
          apCStack_288[0] = CWorld::DrawGetEnvironment((CWorld *)pCVar14);
        }
        pCStack_380 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                                ((CTCInventoryBase *)pCVar14,(long)apCStack_288[0]);
        FUN_0099ee20();
        CCharString_OperatorPlus_API();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_214,extraout_EDX_06,ppVar51);
        CCharString::CCharString(aCStack_2c0,"PC_MAP_MENU_MINI_MAP_TEMPLATE",-1);
        pCStack_410 = NUISystem::CManager::CreateComponent(uStack_420,aCStack_2c0,false);
        ppVar34 = extraout_EDX_07;
        if ((pCStack_410 == (CComponent *)0x0) ||
           (piStack_40c = ::operator_new(0xc), ppVar34 = extraout_EDX_08, piStack_40c == (int *)0x0)
           ) {
          piStack_40c = (int *)0x0;
        }
        else {
          *piStack_40c = 1;
          piStack_40c[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_40c[2] = (int)pCStack_410;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c0,
                   ppVar34,ppVar51);
        piVar8 = piStack_3c0;
        CCharString::CCharString(aCStack_264,"PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE",-1);
        pCStack_3b0 = NUISystem::CManager::CreateComponent(uStack_420,aCStack_264,false);
        ppVar34 = extraout_EDX_09;
        if ((pCStack_3b0 == (CComponent *)0x0) ||
           (piStack_3ac = ::operator_new(0xc), ppVar34 = extraout_EDX_10, piStack_3ac == (int *)0x0)
           ) {
          piStack_3ac = (int *)0x0;
        }
        else {
          *piStack_3ac = 1;
          piStack_3ac[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_3ac[2] = (int)pCStack_3b0;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_264,
                   ppVar34,ppVar51);
        (**(code **)(*piStack_36c + 0x1f0))();
        iVar28 = (**(code **)(*piStack_3b4 + 0x104))();
        if (iVar28 == 0) {
          iVar28 = *piStack_3b4;
          NInventory::CTCInventoryBase::GetMiniMapGraphic
                    ((CTCInventoryBase *)pCStack_408,pCStack_384);
          (**(code **)(iVar28 + 0x244))();
        }
        pCVar18 = pCStack_408;
        (**(code **)(*(int *)local_414 + 0xec))();
        pCVar43 = pCStack_418;
        (**(code **)(*piVar8 + 0x238))();
        iVar28 = *(int *)(pCVar18 + 4);
        pCVar31 = (CTCInventoryMap *)0x0;
        if (((byte)((uint)*(undefined4 *)(iVar28 + 0x20) >> 0x1b) & 1) != 0) {
          piVar56 = *(int **)(iVar28 + 0x48);
          piVar8 = *(int **)(iVar28 + 0x44);
          iVar28 = (int)piVar56 - (int)piVar8 >> 3;
          while (iVar39 = iVar28, 0 < iVar39) {
            iVar28 = iVar39 / 2;
            if (piVar8[iVar28 * 2] < 0x1b) {
              piVar8 = piVar8 + iVar28 * 2 + 2;
              iVar28 = iVar39 + (-1 - iVar28);
            }
          }
          if ((piVar8 == piVar56) || (0x1b < *piVar8)) {
            piVar8 = piVar56;
          }
          pCVar31 = (CTCInventoryMap *)piVar8[1];
        }
        CTCInventoryMap::CreateMiniMapMarkers(pCVar31,(CComponent *)local_41c,pCStack_38c);
        pCVar15 = apCStack_37c[0];
        pCStack_17c = apCStack_37c[0];
        piStack_178 = piStack_424;
        FUN_00624620();
        CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_280);
        ppuStack_280 = &PTR__vector_deleting_destructor__01238c6c;
        CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)&ppuStack_280,pCVar15);
        this_00 = *(CBaseIntelligentPointer **)(pCVar18 + 0x17c);
        if (this_00 == *(CBaseIntelligentPointer **)(pCVar18 + 0x180)) {
          pCStack_470 = this_00;
          FUN_004ac940();
        }
        else {
          if (this_00 != (CBaseIntelligentPointer *)0x0) {
            CBaseIntelligentPointer::CBaseIntelligentPointer(this_00);
            *(undefined ***)this_00 = &PTR__vector_deleting_destructor__01238c6c;
            pCVar15 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)&ppuStack_280);
            CBaseIntelligentPointer::SetPItem(this_00,pCVar15);
          }
          *(int *)(pCVar18 + 0x17c) = *(int *)(pCVar18 + 0x17c) + 8;
        }
        CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)&ppuStack_280);
        piVar56 = (int *)0x0;
        CCharString::CCharString((CCharString *)&fStack_2c4,"HUD_ORB_QUEST_CORE",-1);
        piVar8 = *(int **)(unaff_ESI + 0x14);
        pCVar38 = *(CQuestManager **)(unaff_ESI + 0x10);
        if (piVar8 != (int *)0x0) {
          *piVar8 = *piVar8 + 1;
        }
        CQuestManager::OnQuestStartScreenDisplayed(pCVar38,(CCharString *)&fStack_2c4);
        ppVar34 = extraout_EDX_11;
        if ((piVar8 != (int *)0x0) && (*piVar8 = *piVar8 + -1, *piVar8 == 0)) {
          (*(code *)piVar8[1])();
          operator_delete(piVar8);
          ppVar34 = extraout_EDX_12;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&fStack_2c4,
                   ppVar34,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
        pCVar53 = (CTCQuestCard *)pCVar30;
        bVar6 = CTCQuestCard::IsCore((CTCQuestCard *)pCVar30);
        if (bVar6) {
          CCharString::CCharString(aCStack_28c,"UI_QUEST_SPRITE_CORE",-1);
          pCVar30 = NUISystem::CManager::CreateComponent(unaff_ESI,aCStack_28c,false);
          ppVar34 = extraout_EDX_13;
          if ((piVar56 != (int *)0x0) && (*piVar56 = *piVar56 + -1, *piVar56 == 0)) {
            (*(code *)piVar56[1])();
            operator_delete(piVar56);
            ppVar34 = extraout_EDX_14;
          }
          if ((pCVar30 != (CComponent *)0x0) &&
             (puVar12 = ::operator_new(0xc), ppVar34 = extraout_EDX_15, puVar12 != (undefined4 *)0x0
             )) {
            *puVar12 = 1;
            puVar12[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
            puVar12[2] = pCVar30;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_28c,
                     ppVar34,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
          CCharString::CCharString(aCStack_2bc,"HUD_ORB_QUEST_CORE",-1);
          piVar8 = *(int **)(unaff_ESI + 0x14);
          pCVar38 = *(CQuestManager **)(unaff_ESI + 0x10);
          if (piVar8 != (int *)0x0) {
            *piVar8 = *piVar8 + 1;
          }
          bVar6 = CQuestManager::OnQuestStartScreenDisplayed(pCVar38,aCStack_2bc);
          uStack_39c = CONCAT31(extraout_var,bVar6);
          ppVar34 = extraout_EDX_16;
          if ((piVar8 != (int *)0x0) && (*piVar8 = *piVar8 + -1, *piVar8 == 0)) {
            (*(code *)piVar8[1])();
            operator_delete(piVar8);
            ppVar34 = extraout_EDX_17;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2bc,
                     ppVar34,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
        }
        else {
          piVar8 = *(int **)(pCVar30 + 0xc);
          if (piVar8 != (int *)0x0) {
            piVar8[1] = piVar8[1] + 1;
          }
          iVar28 = piVar8[1];
          cVar41 = *(char *)((int)piVar8 + 0x59);
          piVar8[1] = iVar28 + -1;
          if (iVar28 + -1 == 0) {
            (**(code **)(*piVar8 + 4))();
          }
          if (cVar41 == '\0') {
            CCharString::CCharString((CCharString *)&iStack_30c,"UI_QUEST_SPRITE_OPTIONAL",-1);
            pCVar30 = NUISystem::CManager::CreateComponent
                                (unaff_ESI,(CCharString *)&iStack_30c,false);
            ppVar34 = extraout_EDX_23;
            if ((piVar56 != (int *)0x0) && (*piVar56 = *piVar56 + -1, *piVar56 == 0)) {
              (*(code *)piVar56[1])();
              operator_delete(piVar56);
              ppVar34 = extraout_EDX_24;
            }
            if ((pCVar30 != (CComponent *)0x0) &&
               (puVar12 = ::operator_new(0xc), ppVar34 = extraout_EDX_25,
               puVar12 != (undefined4 *)0x0)) {
              *puVar12 = 1;
              puVar12[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
              puVar12[2] = pCVar30;
            }
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&iStack_30c,
                       ppVar34,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
            CCharString::CCharString(aCStack_308,"HUD_ORB_QUEST_OPTIONAL",-1);
            piVar8 = *(int **)(unaff_ESI + 0x14);
            pCVar38 = *(CQuestManager **)(unaff_ESI + 0x10);
            if (piVar8 != (int *)0x0) {
              *piVar8 = *piVar8 + 1;
            }
            bVar6 = CQuestManager::OnQuestStartScreenDisplayed(pCVar38,aCStack_308);
            uStack_39c = CONCAT31(extraout_var_01,bVar6);
            ppVar34 = extraout_EDX_26;
            if ((piVar8 != (int *)0x0) && (*piVar8 = *piVar8 + -1, *piVar8 == 0)) {
              (*(code *)piVar8[1])();
              operator_delete(piVar8);
              ppVar34 = extraout_EDX_27;
            }
            paVar32 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_308;
          }
          else {
            CCharString::CCharString(aCStack_26c,"UI_QUEST_SPRITE_VIGNETTE",-1);
            pCVar30 = NUISystem::CManager::CreateComponent(unaff_ESI,aCStack_26c,false);
            ppVar34 = extraout_EDX_18;
            if ((piVar56 != (int *)0x0) && (*piVar56 = *piVar56 + -1, *piVar56 == 0)) {
              (*(code *)piVar56[1])();
              operator_delete(piVar56);
              ppVar34 = extraout_EDX_19;
            }
            if ((pCVar30 != (CComponent *)0x0) &&
               (puVar12 = ::operator_new(0xc), ppVar34 = extraout_EDX_20,
               puVar12 != (undefined4 *)0x0)) {
              *puVar12 = 1;
              puVar12[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
              puVar12[2] = pCVar30;
            }
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_26c,
                       ppVar34,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
            CCharString::CCharString(aCStack_2b4,"HUD_ORB_QUEST_VIGNETTE",-1);
            piVar8 = *(int **)(unaff_ESI + 0x14);
            pCVar38 = *(CQuestManager **)(unaff_ESI + 0x10);
            if (piVar8 != (int *)0x0) {
              *piVar8 = *piVar8 + 1;
            }
            bVar6 = CQuestManager::OnQuestStartScreenDisplayed(pCVar38,aCStack_2b4);
            uStack_39c = CONCAT31(extraout_var_00,bVar6);
            ppVar34 = extraout_EDX_21;
            if ((piVar8 != (int *)0x0) && (*piVar8 = *piVar8 + -1, *piVar8 == 0)) {
              (*(code *)piVar8[1])();
              operator_delete(piVar8);
              ppVar34 = extraout_EDX_22;
            }
            paVar32 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2b4;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (paVar32,ppVar34,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
        }
        (**(code **)(*piStack_424 + 0xec))();
        ppVar49 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffbd8;
        (**(code **)(*(int *)param_3 + 0x260))();
        pCStack_470 = (CBaseIntelligentPointer *)0x61f54f;
        CCharString::CCharString(aCStack_308,"PC_TITLE_QUEST_TEXT",-1);
        pCStack_470 = (CBaseIntelligentPointer *)0x61f55f;
        local_3fc = NUISystem::CManager::CreateComponent(unaff_ESI,aCStack_308,false);
        ppVar34 = extraout_EDX_28;
        if ((local_3fc == (CComponent *)0x0) ||
           (piStack_3f8 = ::operator_new(0xc), ppVar34 = extraout_EDX_29, piStack_3f8 == (int *)0x0)
           ) {
          piStack_3f8 = (int *)0x0;
        }
        else {
          *piStack_3f8 = 1;
          piStack_3f8[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_3f8[2] = (int)local_3fc;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_308,
                   ppVar34,ppVar49);
        pCVar26 = pCStack_380;
        (**(code **)(*(int *)pCStack_380 + 0x1f0))();
        apCStack_260[0] = (CComponent *)0x0;
        pCVar10 = (CFrontEndManager *)0x0;
        if (local_3fc != (CComponent *)0x0) {
          apCStack_260[0] = local_3fc;
          *(int *)local_3fc = *(int *)local_3fc + 1;
          pCVar10 = local_400;
        }
        pCStack_470 = (CBaseIntelligentPointer *)0x1;
        (**(code **)(*(int *)pCVar10 + 0x240))();
        ppVar49 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&piStack_40c;
        (**(code **)(*(int *)pCVar44 + 0xec))();
        CCharString::CCharString(aCStack_314,"PC_ICON_INDEPENDANT",-1);
        pCStack_3e0 = NUISystem::CManager::CreateComponent(unaff_ESI,aCStack_314,false);
        ppVar34 = extraout_EDX_30;
        if ((pCStack_3e0 == (CComponent *)0x0) ||
           (piStack_3dc = ::operator_new(0xc), ppVar34 = extraout_EDX_31, piStack_3dc == (int *)0x0)
           ) {
          piStack_3dc = (int *)0x0;
        }
        else {
          *piStack_3dc = 1;
          piStack_3dc[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
          piStack_3dc[2] = (int)pCStack_3e0;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_314,
                   ppVar34,ppVar49);
        (**(code **)(*(int *)pCVar26 + 0x1f0))();
        ppVar50 = ppStack_3bc;
        (**(code **)(*(int *)local_3e4 + 0x244))();
        ppVar49 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_3e8;
        (**(code **)(*(int *)pCVar53 + 0xec))();
        CCharString::CCharString
                  (aCStack_318,"PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT",-1);
        local_414 = NUISystem::CManager::CreateComponent(unaff_ESI,aCStack_318,false);
        ppVar34 = extraout_EDX_32;
        if ((local_414 == (CComponent *)0x0) ||
           (pCStack_410 = ::operator_new(0xc), ppVar34 = extraout_EDX_33,
           pCStack_410 == (CComponent *)0x0)) {
          pCStack_410 = (CComponent *)0x0;
        }
        else {
          *(undefined4 *)pCStack_410 = 1;
          *(code **)((int)pCStack_410 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)((int)pCStack_410 + 8) = local_414;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_318,
                   ppVar34,ppVar49);
        pCVar30 = local_414;
        (**(code **)(*(int *)pCVar26 + 0x1f0))();
        pCVar4 = pCStack_418;
        CCharString::CCharString((CCharString *)&pCStack_470);
        if (piStack_348 != (int *)0x0) {
          piStack_348[1] = piStack_348[1] + 1;
        }
        iVar28 = piStack_348[0x18];
        iVar39 = piStack_348[1];
        piStack_348[1] = iVar39 + -1;
        if (iVar39 + -1 == 0) {
          (**(code **)(*piStack_348 + 4))();
        }
        if (iVar28 != 0) {
          puStack_3c8 = (undefined4 *)0x0;
          if (piStack_348 != (int *)0x0) {
            piStack_348[1] = piStack_348[1] + 1;
          }
          iVar28 = piStack_348[0x17];
          iVar37 = iVar28;
          iVar39 = *(int *)(iVar28 + 4);
          while (iVar39 != 0) {
            if (*(int *)(iVar39 + 0x10) < 0) {
              iVar39 = *(int *)(iVar39 + 0xc);
            }
            else {
              iVar37 = iVar39;
              iVar39 = *(int *)(iVar39 + 8);
            }
          }
          if ((iVar37 == iVar28) || (0 < *(int *)(iVar37 + 0x10))) {
            iVar37 = iVar28;
          }
          if (piStack_348 != (int *)0x0) {
            piVar8 = piStack_348 + 1;
            *piVar8 = *piVar8 + -1;
            if (*piVar8 == 0) {
              (**(code **)(*piStack_348 + 4))();
            }
          }
          if (piStack_348 != (int *)0x0) {
            piStack_348[1] = piStack_348[1] + 1;
          }
          iVar28 = piStack_348[0x17];
          iVar39 = piStack_348[1];
          piStack_348[1] = iVar39 + -1;
          if (iVar39 + -1 == 0) {
            (**(code **)(*piStack_348 + 4))();
          }
          puVar12 = puStack_3c8;
          if (iVar37 != iVar28) {
            puVar12 = *(undefined4 **)(iVar37 + 0x14);
          }
          if (*(int *)(apCStack_37c[0] + 0x70) < (int)puVar12) {
            CCharString::CCharString(aCStack_314,"TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH",-1);
            pCVar48 = *(CDataBank **)(DAT_013b86a0 + 0x14);
            FUN_0099b6b0();
            NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_274);
            pCVar16 = (CWideString *)FUN_0099be70();
            CWideString::operator=((CWideString *)&pCStack_470,pCVar16);
            CCharString::~CCharString(aCStack_200);
            CCharString::~CCharString(aCStack_274);
            CCharString::~CCharString(aCStack_1c0);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_314,
                       extraout_EDX_34,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar30);
            CCharString::CCharString((CCharString *)&stack0xfffffb70,"",-1);
            (**(code **)(*(int *)pCVar4 + 0x238))();
          }
        }
        CCharString::CCharString((CCharString *)&iStack_30c,"TEXT_GUI_MENU_QUEST_SUMMARY",-1);
        pCVar16 = (CWideString *)
                  NGameText::CDataBank::GetTextBySymbol
                            (*(CDataBank **)(DAT_013b86a0 + 0x14),aCStack_23c);
        CWideString::operator=((CWideString *)&pCStack_470,pCVar16);
        CCharString::~CCharString(aCStack_23c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&iStack_30c,
                   extraout_EDX_35,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar30);
        pCVar48 = (CDataBank *)0x0;
        piVar8 = extraout_ECX_01;
        CCharString::CCharString((CCharString *)&stack0xfffffb70,"",-1);
        pCVar53 = (CTCQuestCard *)0xffc8c814;
        ppCVar47 = &pCStack_470;
        (**(code **)(*(int *)pCVar4 + 0x238))();
        pCStack_4a0 = (CManager *)&uStack_244;
        CTCQuestCard::GetQuestSummary((CTCQuestCard *)ppVar49);
        pCStack_4a0 = (CManager *)0x61f953;
        pCVar16 = (CWideString *)FUN_0099bf30();
        pCStack_4a0 = (CManager *)0x61f95d;
        CWideString::operator=((CWideString *)&stack0xfffffb80,pCVar16);
        CCharString::~CCharString(aCStack_228);
        CCharString::~CCharString((CCharString *)&uStack_244);
        piVar56 = (int *)0x0;
        pCStack_4a0 = extraout_ECX_02;
        CCharString::CCharString((CCharString *)&pCStack_4a0,"",-1);
        pCVar46 = (CManager *)0xffffffff;
        ppVar45 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffb80;
        (**(code **)(*(int *)pCVar4 + 0x238))();
        if (param_4 == 1) {
          pCVar44 = pCVar53;
          pCVar17 = (CCharString *)CTCQuestCard::GetQuestName(pCVar53);
          lVar9 = CCharString::GetLength(pCVar17);
          CCharString::~CCharString(aCStack_234);
          if (lVar9 != 0) {
            CCharString::CCharString((CCharString *)local_324,"TEXT_GUI_MENU_CURRENT_OBJECTIVE",-1);
            pCVar16 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_24c);
            CWideString::operator=((CWideString *)&stack0xfffffb70,pCVar16);
            CCharString::~CCharString(aCStack_24c);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_324,
                       extraout_EDX_36,ppVar45);
            CCharString::CCharString((CCharString *)&stack0xfffffb50,"",-1);
            (**(code **)(*(int *)pCVar4 + 0x238))();
            CTCQuestCard::GetQuestName(pCVar53);
            pCVar16 = (CWideString *)FUN_0099bf30();
            CWideString::operator=((CWideString *)&pCStack_4a0,pCVar16);
            CCharString::~CCharString(aCStack_23c);
            CCharString::~CCharString(aCStack_254);
            CCharString::CCharString((CCharString *)&stack0xfffffb40,"",-1);
            (**(code **)(*(int *)pCVar4 + 0x238))();
          }
          pCVar38 = DAT_013b89fc + 0x6c;
          pCStack_3e8 = pCVar38;
          CCharString::CCharString((CCharString *)apiStack_3f4,(CCharString *)(pCVar44 + 0x28));
          CCharString::CCharString((CCharString *)&stack0xfffffb78);
          piVar19 = *(int **)pCVar38;
          piVar21 = (int *)*piVar19;
          ppCVar47 = (CBaseIntelligentPointer **)((uint)ppCVar47 & 0xffffff);
          if (piVar21 != piVar19) {
            do {
              piVar19 = (int *)piVar21[5];
              if (piVar19 == apiStack_3f4[0]) {
LAB_0061fb85:
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString(aCStack_224);
                CCharString::operator+=
                          ((CCharString *)&stack0xfffffb78,(CCharString *)(piVar21 + 6));
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString(aCStack_23c);
                CCharString::CCharString(aCStack_31c,"TEXT_GUI_MENU_WAGER",-1);
                pCVar17 = (CCharString *)NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_21c);
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString(aCStack_21c);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_31c,extraout_EDX_37,ppVar45);
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString(aCStack_1f4);
                CCharString::CCharString(aCStack_388);
                FUN_0099ba70();
                CWideString::CWideString((CWideString *)aCStack_314,(CWideString *)aCStack_388);
                CCharString::~CCharString(aCStack_388);
                CCharString::operator+=((CCharString *)&stack0xfffffb78,aCStack_314);
                CCharString::~CCharString(aCStack_314);
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString(aCStack_290);
                CCharString::CCharString((CCharString *)&iStack_30c,"TEXT_GUI_MENU_REWARD",-1);
                pCVar17 = (CCharString *)
                          NGameText::CDataBank::GetTextBySymbol(pCVar48,(CCharString *)apCStack_288)
                ;
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString((CCharString *)apCStack_288);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           &iStack_30c,extraout_EDX_38,ppVar45);
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString((CCharString *)&ppuStack_280);
                CCharString::CCharString((CCharString *)&uStack_3a4);
                FUN_0099ba70();
                CWideString::CWideString(aCStack_304,(CWideString *)&uStack_3a4);
                CCharString::~CCharString((CCharString *)&uStack_3a4);
                CCharString::operator+=((CCharString *)&stack0xfffffb78,(CCharString *)aCStack_304);
                CCharString::~CCharString((CCharString *)aCStack_304);
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString(aCStack_278);
                ppCVar47 = (CBaseIntelligentPointer **)CONCAT13(1,(int3)ppCVar47);
                if (*(char *)((int)piVar21 + 0x21) != '\0') {
                  pCVar17 = (CCharString *)FUN_0099b6b0();
                  CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                  CCharString::~CCharString(aCStack_270);
                  CCharString::CCharString(aCStack_2fc,"TEXT_GUI_MENU_FAILED",-1);
                  pCVar17 = (CCharString *)
                            NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_268);
                  CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                  CCharString::~CCharString(aCStack_268);
                  std::
                  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                             aCStack_2fc,extraout_EDX_39,ppVar45);
                }
                pCVar17 = (CCharString *)FUN_0099b6b0();
                CCharString::operator+=((CCharString *)&stack0xfffffb78,pCVar17);
                CCharString::~CCharString((CCharString *)apCStack_260);
              }
              else if (((piVar19 != (int *)0x0) && (apiStack_3f4[0] != (int *)0x0)) &&
                      (piVar19[1] == apiStack_3f4[0][1])) {
                pcVar52 = (char *)*piVar19;
                iVar28 = *apiStack_3f4[0] - (int)pcVar52;
                while( true ) {
                  cVar41 = *pcVar52;
                  if ((cVar41 == '\0') && (pcVar52[iVar28] == '\0')) {
                    iVar28 = 0;
                    goto LAB_0061fb78;
                  }
                  if (cVar41 < pcVar52[iVar28]) {
                    iVar28 = -1;
                    goto LAB_0061fb78;
                  }
                  if (pcVar52[iVar28] < cVar41) break;
                  pcVar52 = pcVar52 + 1;
                }
                iVar28 = 1;
LAB_0061fb78:
                if (iVar28 == 0) goto LAB_0061fb85;
              }
              piVar21 = (int *)*piVar21;
            } while (piVar21 != (int *)*(int *)pCStack_3e8);
            if ((char)((uint)ppCVar47 >> 0x18) != '\0') {
              CCharString::CCharString(aCStack_2f4,"TEXT_GUI_MENU_BOASTS",-1);
              pCVar16 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_258);
              CWideString::operator=((CWideString *)&stack0xfffffb70,pCVar16);
              CCharString::~CCharString(aCStack_258);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_2f4,extraout_EDX_40,ppVar45);
              CCharString::CCharString((CCharString *)&stack0xfffffb50,"",-1);
              (**(code **)(*(int *)pCVar4 + 0x238))();
              CCharString::CCharString((CCharString *)&stack0xfffffb40,"",-1);
              (**(code **)(*(int *)pCVar4 + 0x238))();
            }
          }
          CCharString::~CCharString((CCharString *)&stack0xfffffb78);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)apiStack_3f4,
                     extraout_EDX_41,ppVar45);
        }
        CCharString::CCharString(aCStack_2e4,"TEXT_GUI_MENU_MONEY_REWARD",-1);
        CCharString::CCharString((CCharString *)&piStack_3a0);
        FUN_0099ba70();
        CWideString::CWideString((CWideString *)apiStack_2ec,(CWideString *)&piStack_3a0);
        CCharString::~CCharString((CCharString *)&piStack_3a0);
        FUN_0099b6b0();
        pCVar40 = pCVar48;
        NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_240);
        FUN_0099be70();
        pCVar16 = (CWideString *)FUN_0099be70();
        CWideString::operator=((CWideString *)&stack0xfffffb70,pCVar16);
        CCharString::~CCharString(aCStack_250);
        CCharString::~CCharString((CCharString *)&piStack_248);
        CCharString::~CCharString(aCStack_240);
        CCharString::~CCharString(aCStack_1ec);
        CCharString::~CCharString((CCharString *)apiStack_2ec);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2e4,
                   extraout_EDX_42,ppVar45);
        this_06 = (CDataBank *)0x0;
        iVar28 = extraout_ECX_03;
        CCharString::CCharString((CCharString *)&stack0xfffffb50,"HUD_ICON_MONEY_BAG",-1);
        iVar39 = -1;
        ppVar45 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffb70;
        (**(code **)(*(int *)pCVar4 + 0x238))();
        CCharString::CCharString((CCharString *)&piStack_36c,"TEXT_GUI_MENU_RENOWN_REWARD",-1);
        CCharString::CCharString((CCharString *)&piStack_3a8);
        FUN_0099ba70();
        CWideString::CWideString((CWideString *)apiStack_2ec,(CWideString *)&piStack_3a8);
        CCharString::~CCharString((CCharString *)&piStack_3a8);
        FUN_0099b6b0();
        NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_29c);
        FUN_0099be70();
        pCVar16 = (CWideString *)FUN_0099be70();
        CWideString::operator=((CWideString *)&pCStack_4a0,pCVar16);
        CCharString::~CCharString(aCStack_1f4);
        CCharString::~CCharString(aCStack_1ec);
        CCharString::~CCharString(aCStack_29c);
        CCharString::~CCharString(aCStack_240);
        CCharString::~CCharString((CCharString *)apiStack_2ec);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_36c,
                   extraout_EDX_43,ppVar45);
        pCVar53 = extraout_ECX_04;
        CCharString::CCharString((CCharString *)&stack0xfffffb40,"HUD_ORB_QUEST_CORE",-1);
        ppVar42 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&pCStack_4a0;
        (**(code **)(*(int *)pCVar4 + 0x238))();
        if (param_4 == 0) {
          CCharString::CCharString((CCharString *)&piStack_2cc,"TEXT_GUI_MENU_NUM_BOASTS",-1);
          piVar19 = *(int **)(iVar39 + 0xc);
          if (piVar19 != (int *)0x0) {
            piVar19[1] = piVar19[1] + 1;
          }
          CCharString::CCharString((CCharString *)&fStack_390);
          FUN_0099ba70();
          CWideString::CWideString(aCStack_2e0,(CWideString *)&fStack_390);
          CCharString::~CCharString((CCharString *)&fStack_390);
          FUN_0099b6b0();
          NGameText::CDataBank::GetTextBySymbol(this_06,aCStack_29c);
          FUN_0099be70();
          pCVar16 = (CWideString *)FUN_0099be70();
          CWideString::operator=((CWideString *)&stack0xfffffb50,pCVar16);
          CCharString::~CCharString(aCStack_2a4);
          CCharString::~CCharString(aCStack_208);
          CCharString::~CCharString(aCStack_29c);
          CCharString::~CCharString((CCharString *)&piStack_248);
          CCharString::~CCharString((CCharString *)aCStack_2e0);
          piVar21 = piVar19 + 1;
          *piVar21 = *piVar21 + -1;
          ppVar34 = extraout_EDX_44;
          pCVar48 = this_06;
          if (*piVar21 == 0) {
            (**(code **)(*piVar19 + 4))();
            ppVar34 = extraout_EDX_45;
            pCVar48 = this_06;
          }
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_2cc,
                     ppVar34,ppVar42);
          CCharString::CCharString((CCharString *)&stack0xfffffb30,"",-1);
          (**(code **)(*(int *)pCVar4 + 0x238))();
        }
        puVar27 = &stack0xfffffba8;
        (**(code **)(*piVar8 + 0xec))();
        (**(code **)(*piStack_2cc + 0x238))();
        CCharString::CCharString(aCStack_2f8,"PC_SUB_LIST",-1);
        pCVar4 = pCStack_4a0;
        pCVar30 = NUISystem::CManager::CreateComponent(pCStack_4a0,aCStack_2f8,false);
        ppVar34 = extraout_EDX_46;
        if ((pCVar30 == (CComponent *)0x0) ||
           (pCStack_444 = ::operator_new(0xc), ppVar34 = extraout_EDX_47,
           pCStack_444 == (CRegion *)0x0)) {
          pCStack_444 = (CRegion *)0x0;
        }
        else {
          *(int *)pCStack_444 = 1;
          *(code **)(pCStack_444 + 4) = CCountedPointer<CEditTransactionBase>::DeleteData;
          *(CComponent **)(pCStack_444 + 8) = pCVar30;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2f8,
                   ppVar34,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar43);
        pCStack_408 = (CComponent *)0x0;
        local_404 = (CManager *)0x0;
        if (pCStack_444 != (CRegion *)0x0) {
          local_404 = (CManager *)pCStack_444;
          *(int *)pCStack_444 = *(int *)pCStack_444 + 1;
          pCStack_408 = pCVar30;
        }
        pCVar18 = pCStack_408;
        ppVar42 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffbb8;
        (**(code **)(*ppCVar47 + 0xec))();
        if (param_4 == 0) {
          piVar19 = *(int **)(pCVar53 + 0xc);
          iVar39 = 0;
          if (piVar19 != (int *)0x0) {
            piVar19[1] = piVar19[1] + 1;
          }
          iVar37 = piVar19[0x17];
          iVar36 = iVar37;
          iVar2 = *(int *)(iVar37 + 4);
          while (iVar2 != 0) {
            if (*(int *)(iVar2 + 0x10) < 0) {
              iVar2 = *(int *)(iVar2 + 0xc);
            }
            else {
              iVar36 = iVar2;
              iVar2 = *(int *)(iVar2 + 8);
            }
          }
          if ((iVar36 == iVar37) || (0 < *(int *)(iVar36 + 0x10))) {
            iVar36 = iVar37;
          }
          pCVar44 = pCVar53;
          if (piVar19 != (int *)0x0) {
            piVar21 = piVar19 + 1;
            *piVar21 = *piVar21 + -1;
            if (*piVar21 == 0) {
              (**(code **)(*piVar19 + 4))();
            }
          }
          piVar19 = *(int **)(pCVar53 + 0xc);
          if (piVar19 != (int *)0x0) {
            piVar19[1] = piVar19[1] + 1;
          }
          iVar37 = piVar19[0x17];
          iVar2 = piVar19[1];
          piVar19[1] = iVar2 + -1;
          pCVar53 = pCVar44;
          if (iVar2 + -1 == 0) {
            (**(code **)(*piVar19 + 4))();
            pCVar53 = pCVar44;
          }
          if (iVar36 != iVar37) {
            iVar39 = *(int *)(iVar36 + 0x14);
          }
          if (iVar39 <= (int)puStack_3c8[0x1c]) {
            piVar19 = *(int **)(pCVar53 + 0xc);
            if (piVar19 != (int *)0x0) {
              piVar19[1] = piVar19[1] + 1;
            }
            CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)apCStack_37c);
            bVar6 = CQuestManager::IsActiveQuestStartedInRegion
                              (DAT_013b89fc,(CCharString *)apCStack_37c);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)apCStack_37c
                       ,extraout_EDX_48,ppVar42);
            if (piVar19 != (int *)0x0) {
              piVar21 = piVar19 + 1;
              *piVar21 = *piVar21 + -1;
              if (*piVar21 == 0) {
                (**(code **)(*piVar19 + 4))();
              }
            }
            if (!bVar6) {
              CCharString::CCharString((CCharString *)local_324,"TEXT_GUI_MENU_TAKE_QUEST",-1);
              NGameText::CDataBank::GetTextBySymbol((CDataBank *)ppVar45,aCStack_364);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_324,
                         extraout_EDX_49,ppVar42);
              CCharString::CCharString(aCStack_374,"PC_MENU_ENTRY_TEMPLATE",-1);
              CCharString::CCharString((CCharString *)&stack0xfffffb28,"PC_BUTTON_BASTARD",-1);
              CCharString::CCharString((CCharString *)&stack0xfffffb20,"",-1);
              pCVar18 = NUISystem::CManager::CreateMenuEntryComponent
                                  (pCVar46,aCStack_364,0,0x3f800000,aCStack_374,0);
              ppVar34 = extraout_EDX_50;
              if ((pCVar18 != (CComponent *)0x0) &&
                 (puVar12 = ::operator_new(0xc), ppVar34 = extraout_EDX_51,
                 puVar12 != (undefined4 *)0x0)) {
                *puVar12 = 1;
                puVar12[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
                puVar12[2] = pCVar18;
              }
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_374,ppVar34,ppVar42);
              (**(code **)(*(int *)pCVar18 + 0xec))();
              piVar56 = ::operator_new(0x10);
              if (piVar56 == (int *)0x0) {
                piVar56 = (int *)0x0;
              }
              else {
                *piVar56 = 0;
                piVar56[1] = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                piVar56[1] = (int)pvVar13;
                piVar56[2] = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                piVar56[2] = (int)pvVar13;
                *(undefined1 *)(piVar56 + 3) = 1;
              }
              if ((piVar56 != (int *)0x0) &&
                 (puVar12 = ::operator_new(0xc), puVar12 != (undefined4 *)0x0)) {
                *puVar12 = 1;
                puVar12[1] = CTCMapwho::OnDie;
                puVar12[2] = piVar56;
              }
              *piVar56 = 6;
              uVar1 = *(ushort *)(piStack_3f8 + 0x23);
              iVar39 = piVar56[2];
              piVar19 = malloc(0xc);
              if ((uint *)(piVar19 + 2) != (uint *)0x0) {
                piVar19[2] = (uint)uVar1;
              }
              puVar12 = *(undefined4 **)(iVar39 + 4);
              *piVar19 = iVar39;
              piVar19[1] = (int)puVar12;
              *puVar12 = piVar19;
              *(int **)(iVar39 + 4) = piVar19;
              (**(code **)(*(int *)ppVar50 + 0x108))();
              puVar12 = ::operator_new(0x10);
              if (puVar12 == (undefined4 *)0x0) {
                puVar12 = (undefined4 *)0x0;
              }
              else {
                *puVar12 = 0;
                puVar12[1] = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                puVar12[1] = pvVar13;
                puVar12[2] = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                puVar12[2] = pvVar13;
                *(undefined1 *)(puVar12 + 3) = 1;
              }
              if ((puVar12 != (undefined4 *)0x0) &&
                 (puVar20 = ::operator_new(0xc), puVar20 != (undefined4 *)0x0)) {
                *puVar20 = 1;
                puVar20[1] = CTCMapwho::OnDie;
                puVar20[2] = puVar12;
              }
              *puVar12 = 0xbbc;
              uVar1 = *(ushort *)(local_3fc + 0x8c);
              iVar39 = puVar12[2];
              piVar19 = malloc(0xc);
              if ((uint *)(piVar19 + 2) != (uint *)0x0) {
                piVar19[2] = (uint)uVar1;
              }
              puVar12 = *(undefined4 **)(iVar39 + 4);
              *piVar19 = iVar39;
              piVar19[1] = (int)puVar12;
              *puVar12 = piVar19;
              *(int **)(iVar39 + 4) = piVar19;
              puVar12 = ::operator_new(0x10);
              if (puVar12 == (undefined4 *)0x0) {
                puVar12 = (undefined4 *)0x0;
              }
              else {
                *puVar12 = 0;
                puVar12[1] = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                puVar12[1] = pvVar13;
                puVar12[2] = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                puVar12[2] = pvVar13;
                *(undefined1 *)(puVar12 + 3) = 1;
              }
              if ((puVar12 == (undefined4 *)0x0) ||
                 (piVar19 = ::operator_new(0xc), piVar19 == (int *)0x0)) {
                piVar19 = (int *)0x0;
              }
              else {
                *piVar19 = 1;
                piVar19[1] = (int)CTCMapwho::OnDie;
                piVar19[2] = (int)puVar12;
              }
              *puVar12 = 0xd8;
              if (pCVar40 == (CDataBank *)0x0) {
                pCVar40 = (CDataBank *)0x0;
              }
              else {
                pCVar40 = pCVar40 + 0x18;
              }
              iVar39 = puVar12[1];
              piVar21 = malloc(0xc);
              if (piVar21 + 2 != (int *)0x0) {
                piVar21[2] = (int)pCVar40;
              }
              puVar12 = *(undefined4 **)(iVar39 + 4);
              *piVar21 = iVar39;
              piVar21[1] = (int)puVar12;
              *puVar12 = piVar21;
              *(int **)(iVar39 + 4) = piVar21;
              (**(code **)(*(int *)ppVar49 + 0x108))();
              uVar1 = *(ushort *)(local_400 + 0x8c);
              iVar39 = *(int *)(ppVar50 + 8);
              piVar21 = malloc(0xc);
              if ((uint *)(piVar21 + 2) != (uint *)0x0) {
                piVar21[2] = (uint)uVar1;
              }
              puVar12 = *(undefined4 **)(iVar39 + 4);
              *piVar21 = iVar39;
              piVar21[1] = (int)puVar12;
              *puVar12 = piVar21;
              *(int **)(iVar39 + 4) = piVar21;
              piVar21 = *(int **)(puVar27 + 0xc);
              if (piVar21 != (int *)0x0) {
                piVar21[1] = piVar21[1] + 1;
              }
              iVar39 = piVar21[1];
              iVar37 = piVar21[0x1a];
              piVar21[1] = iVar39 + -1;
              if (iVar39 + -1 == 0) {
                (**(code **)(*piVar21 + 4))();
              }
              pCVar43 = pCStack_418;
              (**(code **)(*(int *)pCStack_418 + 0x270))((iVar37 != 0) + '\x01');
              (**(code **)(*(int *)pCVar43 + 0x260))(&stack0xfffffb74);
              pCVar22 = ::operator_new(0x10);
              if (pCVar22 == (CAction *)0x0) {
                pCVar22 = (CAction *)0x0;
              }
              else {
                *(undefined4 *)pCVar22 = 0;
                *(undefined4 *)(pCVar22 + 4) = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                *(void **)(pCVar22 + 4) = pvVar13;
                *(undefined4 *)(pCVar22 + 8) = 0;
                pvVar13 = malloc(0xc);
                *(void **)pvVar13 = pvVar13;
                *(void **)((int)pvVar13 + 4) = pvVar13;
                *(void **)(pCVar22 + 8) = pvVar13;
                pCVar22[0xc] = (CAction)0x1;
              }
              CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                        ((CCountedPointer<NUISystem::CAction> *)&piStack_3f8,pCVar22);
              *piStack_3f8 = 0x15;
              if (iVar28 == 0) {
                unaff_EBX = (int *)0x0;
              }
              else {
                unaff_EBX = (int *)(iVar28 + 0x18);
              }
              iVar28 = piStack_3f8[1];
              piVar21 = malloc(0xc);
              if (piVar21 + 2 != (int *)0x0) {
                piVar21[2] = (int)unaff_EBX;
              }
              puVar12 = *(undefined4 **)(iVar28 + 4);
              *piVar21 = iVar28;
              piVar21[1] = (int)puVar12;
              *puVar12 = piVar21;
              *(int **)(iVar28 + 4) = piVar21;
              (**(code **)(*piVar8 + 0x108))(&piStack_3f8);
              piVar8 = *(int **)(pCVar53 + 0xc);
              if (piVar8 != (int *)0x0) {
                piVar8[1] = piVar8[1] + 1;
              }
              iVar28 = piVar8[1];
              cVar41 = piVar8[0x1a] != 0;
              piVar8[1] = iVar28 + -1;
              if (iVar28 + -1 == 0) {
                (**(code **)(*piVar8 + 4))();
              }
              if (cVar41 != '\0') {
                CCharString::CCharString
                          ((CCharString *)aCStack_2e0,"TEXT_GUI_MENU_TAKE_QUEST_BOAST",-1);
                NGameText::CDataBank::GetTextBySymbol((CDataBank *)ppVar45,aCStack_31c);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_2e0,extraout_EDX_52,ppVar42);
                CCharString::CCharString((CCharString *)&piStack_36c,"PC_MENU_ENTRY_TEMPLATE",-1);
                CCharString::CCharString((CCharString *)&stack0xfffffb28,"PC_BUTTON_BASTARD",-1);
                CCharString::CCharString((CCharString *)&stack0xfffffb20,"",-1);
                pCVar18 = NUISystem::CManager::CreateMenuEntryComponent
                                    (pCVar46,aCStack_31c,0,0x3f800000,&piStack_36c,0);
                CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                          ((CCountedPointer<NUISystem::CAction> *)&local_3fc,(CAction *)pCVar18);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           &piStack_36c,extraout_EDX_53,ppVar42);
                (**(code **)(*(int *)local_3fc + 0xec))();
                (**(code **)(*(int *)pCVar43 + 0x260))();
                (**(code **)(*(int *)local_404 + 0x108))();
                this_03 = ::operator_new(0x10);
                if (this_03 == (CRumbleDef *)0x0) {
                  pCVar22 = (CAction *)0x0;
                }
                else {
                  pCVar22 = (CAction *)CRumbleDef::CRumbleDef(this_03);
                }
                CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                          ((CCountedPointer<NUISystem::CAction> *)&puStack_3c8,pCVar22);
                *puStack_3c8 = 0xcf;
                (**(code **)(*(int *)pCStack_408 + 0x108))(&puStack_3c8);
                if ((pCStack_3b8 != (CComponent *)0x0) &&
                   (*(int *)pCStack_3b8 = *(int *)pCStack_3b8 + -1, *(int *)pCStack_3b8 == 0)) {
                  (**(code **)(pCStack_3b8 + 4))();
                  operator_delete(pCStack_3b8);
                }
                ppStack_3bc = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
                pCStack_3b8 = (CComponent *)0x0;
                if ((piStack_3f8 != (int *)0x0) &&
                   (*piStack_3f8 = *piStack_3f8 + -1, *piStack_3f8 == 0)) {
                  (*(code *)piStack_3f8[1])();
                  operator_delete(piStack_3f8);
                }
                local_3fc = (CComponent *)0x0;
                piStack_3f8 = (int *)0x0;
                CCharString::~CCharString(aCStack_31c);
              }
              if ((pCStack_3e0 != (CComponent *)0x0) &&
                 (*(int *)pCStack_3e0 = *(int *)pCStack_3e0 + -1, *(int *)pCStack_3e0 == 0)) {
                (**(code **)(pCStack_3e0 + 4))();
                operator_delete(pCStack_3e0);
              }
              local_3e4 = (CComponent *)0x0;
              pCStack_3e0 = (CComponent *)0x0;
              if ((pCStack_470 != (CBaseIntelligentPointer *)0x0) &&
                 (*(int *)pCStack_470 = *(int *)pCStack_470 + -1, *(int *)pCStack_470 == 0)) {
                (**(code **)(pCStack_470 + 4))();
                operator_delete(pCStack_470);
              }
              pCStack_470 = (CBaseIntelligentPointer *)0x0;
              if ((piVar56 != (int *)0x0) && (*piVar56 = *piVar56 + -1, *piVar56 == 0)) {
                (*(code *)piVar56[1])();
                operator_delete(piVar56);
              }
              if ((ppVar51 != (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0) &&
                 (*(int *)ppVar51 = *(int *)ppVar51 + -1, *(int *)ppVar51 == 0)) {
                (**(code **)(ppVar51 + 4))();
                operator_delete(ppVar51);
              }
              ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
              if ((piVar19 != (int *)0x0) && (*piVar19 = *piVar19 + -1, *piVar19 == 0)) {
                (*(code *)piVar19[1])();
                operator_delete(piVar19);
              }
              pCVar17 = aCStack_364;
              goto LAB_00620ea5;
            }
          }
          (**(code **)(*piVar56 + 0x170))();
          (**(code **)(*piVar56 + 0xd4))();
          (**(code **)(*piVar56 + 0x100))();
        }
        else {
          if (param_4 == 1) {
            CCharString::CCharString(aCStack_2f0,"TEXT_GUI_MENU_ZOOMED_REGION",-1);
            NGameText::CDataBank::GetTextBySymbol(pCVar48,aCStack_314);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2f0,
                       extraout_EDX_54,ppVar42);
            CCharString::CCharString((CCharString *)&local_35c,"PC_MENU_ENTRY_TEMPLATE",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb28,"PC_BUTTON_BASTARD",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb20,"",-1);
            pCVar23 = NUISystem::CManager::CreateMenuEntryComponent
                                (pCVar4,aCStack_314,0,0x3f800000,&local_35c,0);
            CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                      ((CCountedPointer<NUISystem::CAction> *)&pCStack_3b0,(CAction *)pCVar23);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_35c,
                       extraout_EDX_55,ppVar42);
            (**(code **)(*(int *)pCVar18 + 0x240))();
            (**(code **)(*piStack_3b4 + 0xec))();
            if ((piStack_3ac != (int *)0x0) && (*piStack_3ac = *piStack_3ac + -1, *piStack_3ac == 0)
               ) {
              (*(code *)piStack_3ac[1])();
              operator_delete(piStack_3ac);
            }
            pCStack_3b0 = (CComponent *)0x0;
            piStack_3ac = (int *)0x0;
            pCVar17 = aCStack_314;
          }
          else {
            if (param_4 != 2) goto LAB_00620eaa;
            CCharString::CCharString((CCharString *)&pCStack_354,"TEXT_GUI_MENU_ZOOMED_REGION",-1);
            NGameText::CDataBank::GetTextBySymbol(pCVar48,(CCharString *)&piStack_34c);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_354
                       ,extraout_EDX_56,ppVar42);
            CCharString::CCharString(aCStack_2dc,"PC_MENU_ENTRY_TEMPLATE",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb28,"PC_BUTTON_BASTARD",-1);
            CCharString::CCharString((CCharString *)&stack0xfffffb20,"",-1);
            pCVar23 = NUISystem::CManager::CreateMenuEntryComponent
                                (pCVar4,&piStack_34c,0,0x3f800000,aCStack_2dc,0);
            CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                      ((CCountedPointer<NUISystem::CAction> *)&uStack_3a4,(CAction *)pCVar23);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2dc,
                       extraout_EDX_57,ppVar42);
            (**(code **)(*(int *)pCVar18 + 0x240))();
            (**(code **)(*piStack_3a8 + 0xec))();
            if ((piStack_3a0 != (int *)0x0) && (*piStack_3a0 = *piStack_3a0 + -1, *piStack_3a0 == 0)
               ) {
              (*(code *)piStack_3a0[1])();
              operator_delete(piStack_3a0);
            }
            uStack_3a4 = 0;
            piStack_3a0 = (int *)0x0;
            pCVar17 = (CCharString *)&piStack_34c;
          }
LAB_00620ea5:
          CCharString::~CCharString(pCVar17);
        }
LAB_00620eaa:
        CTCQuestCard::GetRegionName(pCVar53);
        if (0 < iStack_30c) {
          CRegion::GetNameGraphicOffset((CRegion *)local_404);
        }
        NUISystem::CUIStateDef::CUIStateDef(aCStack_1e4);
        pCVar10 = local_400;
        NUISystem::CUIScreenUnitConverter::ConvertX((float)&fStack_2c8);
        this_04 = NUISystem::CFrontEndManager::GetInstance();
        iVar28 = NUISystem::CManager::GetUIScale((CManager *)this_04);
        fStack_2c4 = *(float *)(iVar28 + 4) * *(float *)(pCVar10 + 0x200) + fStack_2c4;
        pfVar11 = (float *)CRegion::GetNameGraphicOffset((CRegion *)local_404);
        pCStack_38c = (CRegion *)(*(float *)(pCVar10 + 0xec) - pfVar11[1]);
        fStack_390 = *(float *)(pCVar10 + 0xe8) - *pfVar11;
        local_19c = (CRegion *)fStack_390;
        local_198[0] = pCStack_38c;
        pCVar10 = NUISystem::CFrontEndManager::GetInstance();
        pfVar11 = (float *)NUISystem::CManager::GetUIScale((CManager *)pCVar10);
        local_19c = (CRegion *)((float)local_19c * *pfVar11);
        pCVar10 = NUISystem::CFrontEndManager::GetInstance();
        iVar28 = NUISystem::CManager::GetUIScale((CManager *)pCVar10);
        local_19c = (CRegion *)(fStack_2c8 + (float)local_19c);
        pCStack_12c = pCStack_4a0 + 8;
        pCStack_17c = (CBaseObject *)0x3f000000;
        local_1a0 = 3;
        local_198[0] = (CRegion *)((float)local_198[0] * *(float *)(iVar28 + 4) + fStack_2c4);
        uStack_1a4 = 0x67;
        NUISystem::CUIStateDef::CUIStateDef(aCStack_128,aCStack_1e4);
        pCStack_154 = pCStack_12c;
        NUISystem::CUIState::CUIState(aCStack_150,aCStack_128);
        (**(code **)(*(int *)ppVar49 + 0x148))();
        (**(code **)(*(int *)ppVar49 + 0x148))();
        FUN_0042d2c6();
        local_41c = pCStack_4a0 + 1;
        if (pvStack_38 != (void *)0x0) {
          free(pvStack_38);
        }
        if (pvStack_7b != (void *)0x0) {
          free(pvStack_7b);
        }
        appuStack_a4[0] = &PTR__vector_deleting_destructor__01230ba0;
        CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)appuStack_a4);
        if (pvStack_f4 != (void *)0x0) {
          free(pvStack_f4);
        }
        if (pvStack_137 != (void *)0x0) {
          free(pvStack_137);
        }
        ppuStack_160 = &PTR__vector_deleting_destructor__01230ba0;
        CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)&ppuStack_160);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_21c,
                   extraout_EDX_58,ppVar51);
        pCVar35 = pCStack_384;
        if ((pCStack_384 != (CRegion *)0x0) &&
           (*(int *)pCStack_384 = *(int *)pCStack_384 + -1, *(int *)pCStack_384 == 0)) {
          (**(code **)(pCStack_384 + 4))();
          operator_delete(pCVar35);
        }
        if ((piStack_3c4 != (int *)0x0) && (*piStack_3c4 = *piStack_3c4 + -1, *piStack_3c4 == 0)) {
          (*(code *)piStack_3c4[1])();
          operator_delete(piStack_3c4);
        }
        puStack_3c8 = (undefined4 *)0x0;
        piStack_3c4 = (int *)0x0;
        CCharString::~CCharString((CCharString *)&stack0xfffffbc8);
        ppVar34 = extraout_EDX_59;
        if ((piStack_3dc != (int *)0x0) && (*piStack_3dc = *piStack_3dc + -1, *piStack_3dc == 0)) {
          (*(code *)piStack_3dc[1])();
          operator_delete(piStack_3dc);
          ppVar34 = extraout_EDX_60;
        }
        pCStack_3e0 = (CComponent *)0x0;
        piStack_3dc = (int *)0x0;
        if ((piStack_3b4 != (int *)0x0) && (*piStack_3b4 = *piStack_3b4 + -1, *piStack_3b4 == 0)) {
          (*(code *)piStack_3b4[1])();
          operator_delete(piStack_3b4);
          ppVar34 = extraout_EDX_61;
        }
        piVar8 = piStack_248;
        pCStack_3b8 = (CComponent *)0x0;
        piStack_3b4 = (int *)0x0;
        if ((piStack_248 != (int *)0x0) && (*piStack_248 = *piStack_248 + -1, *piStack_248 == 0)) {
          (*(code *)piStack_248[1])();
          operator_delete(piVar8);
          ppVar34 = extraout_EDX_62;
        }
        if ((local_3e4 != (CComponent *)0x0) &&
           (*(int *)local_3e4 = *(int *)local_3e4 + -1, *(int *)local_3e4 == 0)) {
          (**(code **)(local_3e4 + 4))();
          operator_delete(local_3e4);
          ppVar34 = extraout_EDX_63;
        }
        pCStack_3e8 = (CQuestManager *)0x0;
        local_3e4 = (CComponent *)0x0;
        if ((unaff_EBX != (int *)0x0) && (*unaff_EBX = *unaff_EBX + -1, *unaff_EBX == 0)) {
          (*(code *)unaff_EBX[1])();
          operator_delete(unaff_EBX);
          ppVar34 = extraout_EDX_64;
        }
        unaff_ESI = (CManager *)0x0;
        unaff_EBX = (int *)0x0;
        if ((piStack_3ac != (int *)0x0) && (*piStack_3ac = *piStack_3ac + -1, *piStack_3ac == 0)) {
          (*(code *)piStack_3ac[1])();
          operator_delete(piStack_3ac);
          ppVar34 = extraout_EDX_65;
        }
        pCStack_3b0 = (CComponent *)0x0;
        piStack_3ac = (int *)0x0;
        if ((piStack_40c != (int *)0x0) && (*piStack_40c = *piStack_40c + -1, *piStack_40c == 0)) {
          (*(code *)piStack_40c[1])();
          operator_delete(piStack_40c);
          ppVar34 = extraout_EDX_66;
        }
        pCStack_410 = (CComponent *)0x0;
        piStack_40c = (int *)0x0;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_314,
                   ppVar34,ppVar51);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_324,
                   extraout_EDX_67,ppVar51);
        if ((local_414 != (CComponent *)0x0) &&
           (*(int *)local_414 = *(int *)local_414 + -1, *(int *)local_414 == 0)) {
          (**(code **)(local_414 + 4))();
          operator_delete(local_414);
        }
        pCStack_418 = (CManager *)0x0;
        local_414 = (CComponent *)0x0;
        CCharString::~CCharString((CCharString *)aCStack_33c);
        pCStack_310 = pCStack_310 + 1;
      } while (pCStack_310 != pCStack_408);
    }
    piStack_424 = (int *)((int)piStack_424 + 1);
    if (piStack_424 == (int *)0x3) {
      if (param_4 == 0) {
        ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
      }
      else {
        ppVar51 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                  ((*(int *)(param_2 + 4) - *(int *)param_2 >> 2) + -1);
      }
      (**(code **)(*(int *)param_3 + 0x278))();
      iVar28 = *(int *)(pCStack_408 + 4);
      if ((*(uint *)(iVar28 + 0x20) & 0x8000000) != 0) {
        piStack_40c = (int *)0x1b;
        piVar8 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                        ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                      *)(iVar28 + 0x44),(ETCInterfaceType *)&piStack_40c);
        if ((piVar8 == *(int **)(iVar28 + 0x48)) || (0x1b < *piVar8)) {
          piVar8 = *(int **)(iVar28 + 0x48);
        }
        pCVar31 = (CTCInventoryMap *)piVar8[1];
        if (pCVar31 != (CTCInventoryMap *)0x0) {
          uVar29 = 0;
          piStack_3ac = (int *)0x0;
          piStack_3ac = malloc(0x14);
          piStack_3a8 = (int *)0x0;
          *(undefined1 *)piStack_3ac = 0;
          *(undefined4 *)((int)piStack_3ac + 4) = 0;
          *(int **)((int)piStack_3ac + 8) = piStack_3ac;
          *(int **)((int)piStack_3ac + 0xc) = piStack_3ac;
          local_35c = (CAIStateGroup_SummonerCharge *)0x0;
          pCStack_358 = (CAIStateGroup_SummonerCharge *)0x0;
          pCStack_354 = (CRegion *)0x0;
          CTCInventoryMap::GetRegionsToDisplay
                    (pCVar31,(set<long,std::less<long>,std::allocator<long>_> *)&piStack_3ac);
          iVar28 = *(int *)param_2;
          if (*(int *)(param_2 + 4) - iVar28 >> 2 != 0) {
            do {
              iVar28 = *(int *)(iVar28 + uVar29 * 4);
              pCVar53 = (CTCQuestCard *)0x0;
              if ((*(uint *)(iVar28 + 0x2c) & 0x1000) != 0) {
                piStack_40c = (int *)0x6c;
                piVar8 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                              *)(iVar28 + 0x44),(ETCInterfaceType *)&piStack_40c);
                if ((piVar8 == *(int **)(iVar28 + 0x48)) || (0x6c < *piVar8)) {
                  piVar8 = *(int **)(iVar28 + 0x48);
                }
                pCVar53 = (CTCQuestCard *)piVar8[1];
              }
              pCVar24 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCStack_408);
              pCVar17 = (CCharString *)CTCQuestCard::GetRegionName(pCVar53);
              pCVar14 = (CWorldMap *)(**(code **)(*(int *)pCVar24 + 0x34))();
              piVar8 = (int *)CWorldMap::GetRegionNumberFromName(pCVar14,pCVar17);
              piStack_40c = piVar8;
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         aCStack_318,extraout_EDX_68,ppVar51);
              if (0 < (int)piVar8) {
                FUN_00512da0();
              }
              iVar28 = *(int *)param_2;
              uVar29 = uVar29 + 1;
            } while (uVar29 < (uint)(*(int *)(param_2 + 4) - iVar28 >> 2));
          }
          pCVar30 = pCStack_408;
          puVar27 = *(undefined1 **)((int)piStack_3ac + 8);
          if ((int *)puVar27 != piStack_3ac) {
            do {
              pCVar24 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar30);
              lVar9 = *(long *)(puVar27 + 0x10);
              pCVar25 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar24 + 0x34))();
              pCVar26 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar25,lVar9);
              if (pCVar26[0x54] != (CCategory)0x0) {
                pCVar24 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)pCVar30);
                lVar9 = *(long *)(puVar27 + 0x10);
                pCVar25 = (CTCInventoryBase *)(**(code **)(*(int *)pCVar24 + 0x34))();
                pCVar26 = NInventory::CTCInventoryBase::GetCategoryWithIndex(pCVar25,lVar9);
                if (pCStack_358 == (CAIStateGroup_SummonerCharge *)pCStack_354) {
                  FUN_0051cf10();
                }
                else {
                  if (pCStack_358 != (CAIStateGroup_SummonerCharge *)0x0) {
                    CRegion::CRegion((CRegion *)pCStack_358,(CRegion *)pCVar26);
                  }
                  pCStack_358 = pCStack_358 + 0x58;
                }
              }
              puVar33 = *(undefined1 **)(puVar27 + 0xc);
              if (puVar33 == (undefined1 *)0x0) {
                puVar33 = *(undefined1 **)(puVar27 + 4);
                if (puVar27 == *(undefined1 **)(puVar33 + 0xc)) {
                  do {
                    puVar27 = puVar33;
                    puVar33 = *(undefined1 **)(puVar27 + 4);
                  } while (puVar27 == *(undefined1 **)(puVar33 + 0xc));
                }
                if (*(undefined1 **)(puVar27 + 0xc) != puVar33) {
                  puVar27 = puVar33;
                }
              }
              else {
                for (puVar3 = *(undefined1 **)(puVar33 + 8); puVar27 = puVar33,
                    puVar3 != (undefined1 *)0x0; puVar3 = *(undefined1 **)(puVar3 + 8)) {
                  puVar33 = puVar3;
                }
              }
            } while ((int *)puVar27 != piStack_3ac);
          }
          CTCInventoryMap::CreateMapMarkers
                    (pCVar31,(CComponent *)local_404,
                     (vector<CRegion,std::allocator<CRegion>_> *)&local_35c);
          pCVar5 = pCStack_358;
          for (this_05 = local_35c; this_05 != pCVar5; this_05 = this_05 + 0x58) {
            CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge(this_05);
          }
          if (local_35c != (CAIStateGroup_SummonerCharge *)0x0) {
            free(local_35c);
          }
          if (piStack_3a8 != (int *)0x0) {
            LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
                      ((LTextTreeWalkThrough *)&piStack_3ac,*(LTextBinNode **)((int)piStack_3ac + 4)
                      );
            *(int **)((int)piStack_3ac + 8) = piStack_3ac;
            *(undefined4 *)((int)piStack_3ac + 4) = 0;
            *(int **)((int)piStack_3ac + 0xc) = piStack_3ac;
            piStack_3a8 = (int *)0x0;
          }
          if (piStack_3ac != (int *)0x0) {
            free(piStack_3ac);
          }
        }
      }
      (**(code **)(*(int *)param_3 + 0x244))();
      if ((piStack_3c4 != (int *)0x0) && (*piStack_3c4 = *piStack_3c4 + -1, *piStack_3c4 == 0)) {
        (*(code *)piStack_3c4[1])();
        operator_delete(piStack_3c4);
      }
      puStack_3c8 = (undefined4 *)0x0;
      piStack_3c4 = (int *)0x0;
      if ((local_404 != (CManager *)0x0) &&
         (*(int *)local_404 = *(int *)local_404 + -1, *(int *)local_404 == 0)) {
        (**(code **)(local_404 + 4))();
        operator_delete(local_404);
      }
      pCStack_408 = (CComponent *)0x0;
      local_404 = (CManager *)0x0;
      pCVar35 = pCStack_384 + 4;
      *(int *)pCVar35 = *(int *)pCVar35 + -1;
      if (*(int *)pCVar35 == 0) {
        (**(code **)(*(int *)pCStack_384 + 4))();
      }
      puVar12 = &uStack_1a4;
      iVar28 = 3;
      do {
        puVar20 = puVar12 + -3;
        puVar12 = puVar12 + -3;
        if ((void *)*puVar20 != (void *)0x0) {
          free((void *)*puVar20);
        }
        iVar28 = iVar28 + -1;
      } while (iVar28 != 0);
      return;
    }
  } while( true );
}



//=== InitialiseQuestCardList @ 00621a20 ===

/* [bsim sim=0.7340809997236609 <- ego_r]
   public: void __thiscall CTCInventoryQuests::InitialiseQuestCardList(void) */

void __thiscall CTCInventoryQuests::InitialiseQuestCardList(CTCInventoryQuests *this)

{
  long lVar1;
  int iVar2;
  CBaseIntelligentPointer *pCVar3;
  CBaseIntelligentPointer *pCVar4;
  CBaseIntelligentPointer *pCVar5;
  char cVar6;
  pair<CCharString,CCharString> *ppVar7;
  CBaseObject *pCVar8;
  CIDrawEnvironment *pCVar9;
  int *piVar10;
  int iVar11;
  CCategory *this_00;
  CTCHeroCentre *this_01;
  int *piVar12;
  undefined **ppuVar13;
  allocator<std::pair<unsigned_long,CAnimationEntry>_> local_1d;
  undefined **appuStack_1c [2];
  CBaseIntelligentPointer *local_14;
  CBaseIntelligentPointer *local_10;
  CBaseIntelligentPointer *local_c;
  
  ppVar7 = std::
           _Copy_impl<std::pair<CCharString,CCharString>*,std::pair<class_CCharString,class_CCharString>*>
                     (*(undefined4 *)(this + 0x24),*(undefined4 *)(this + 0x24),
                      *(undefined4 *)(this + 0x20),&local_1d,0);
  std::_Destroy_range<std::allocator<std::pair<unsigned_long,CAnimationEntry>_>_>
            ((pair<unsigned_long,CAnimationEntry> *)ppVar7,
             *(pair<unsigned_long,CAnimationEntry> **)(this + 0x24),&local_1d);
  *(pair<CCharString,CCharString> **)(this + 0x24) = ppVar7;
  local_14 = (CBaseIntelligentPointer *)0x0;
  local_10 = (CBaseIntelligentPointer *)0x0;
  local_c = (CBaseIntelligentPointer *)0x0;
  cVar6 = (**(code **)(*(int *)(this + 0x130) + 0xc))();
  if (cVar6 == '\0') {
    cVar6 = (**(code **)(*(int *)this + 300))();
    if ((cVar6 != '\0') &&
       (pCVar9 = CWorld::DrawGetEnvironment((CWorld *)this), pCVar9 != (CIDrawEnvironment *)0x0)) {
      this_01 = (CTCHeroCentre *)0x0;
      if ((*(uint *)(pCVar9 + 0x2c) & 0x400) != 0) {
        appuStack_1c[0] = (undefined **)0x6a;
        piVar10 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                         ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                       *)(pCVar9 + 0x44),(ETCInterfaceType *)appuStack_1c);
        if ((piVar10 == *(int **)(pCVar9 + 0x48)) || (0x6a < *piVar10)) {
          piVar10 = *(int **)(pCVar9 + 0x48);
        }
        this_01 = (CTCHeroCentre *)piVar10[1];
      }
      CTCHeroCentre::GetQuestCards
                (this_01,(vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing>_>_>
                          *)&local_14);
    }
  }
  else {
    piVar10 = (int *)**(int **)(DAT_013b89fc + 0x5c);
    piVar12 = (int *)(DAT_013b89fc + 0x5c);
    if (piVar10 != *(int **)(DAT_013b89fc + 0x5c)) {
      do {
        pCVar8 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(piVar10 + 2));
        CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)appuStack_1c);
        appuStack_1c[0] = &PTR__vector_deleting_destructor__01238c6c;
        CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)appuStack_1c,pCVar8);
        pCVar3 = local_10;
        if (local_10 == local_c) {
          FUN_004ac940(local_10,appuStack_1c,&local_1d,1,1);
        }
        else {
          if (local_10 != (CBaseIntelligentPointer *)0x0) {
            CBaseIntelligentPointer::CBaseIntelligentPointer(local_10);
            *(undefined ***)pCVar3 = &PTR__vector_deleting_destructor__01238c6c;
            pCVar8 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)appuStack_1c);
            CBaseIntelligentPointer::SetPItem(pCVar3,pCVar8);
          }
          local_10 = local_10 + 8;
        }
        CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)appuStack_1c);
        piVar10 = (int *)*piVar10;
      } while (piVar10 != (int *)*piVar12);
    }
  }
  iVar11 = (int)local_10 - (int)local_14 >> 3;
  *(int *)(this + 0x150) = iVar11;
  appuStack_1c[0] = (undefined **)0x0;
  if (0 < iVar11) {
    do {
      ppuVar13 = appuStack_1c[0];
      pCVar8 = CBaseIntelligentPointer::GetPItem(local_14 + (int)appuStack_1c[0] * 8);
      if ((*(uint *)(pCVar8 + 0x2c) & 0x1000) != 0) {
        piVar12 = *(int **)(pCVar8 + 0x48);
        piVar10 = *(int **)(pCVar8 + 0x44);
        iVar11 = (int)piVar12 - (int)piVar10 >> 3;
        while (iVar2 = iVar11, 0 < iVar2) {
          iVar11 = iVar2 / 2;
          if (piVar10[iVar11 * 2] < 0x6c) {
            piVar10 = piVar10 + iVar11 * 2 + 2;
            iVar11 = iVar2 + (-1 - iVar11);
          }
        }
        if ((piVar10 == piVar12) || (0x6c < *piVar10)) {
          piVar10 = piVar12;
        }
        piVar10 = *(int **)(piVar10[1] + 0xc);
        if (piVar10 != (int *)0x0) {
          piVar10[1] = piVar10[1] + 1;
        }
        iVar11 = piVar10[1];
        lVar1 = piVar10[0x10];
        piVar10[1] = iVar11 + -1;
        if (iVar11 + -1 == 0) {
          (**(code **)(*piVar10 + 4))();
        }
        NInventory::CTCInventoryBase::AddCategory
                  ((CTCInventoryBase *)this,(CTCInventoryBase *)this,lVar1,(CThing *)pCVar8);
        ppuVar13 = appuStack_1c[0];
      }
      appuStack_1c[0] = (undefined **)((int)ppuVar13 + 1);
    } while ((int)appuStack_1c[0] < *(int *)(this + 0x150));
  }
  pCVar3 = local_14;
  pCVar4 = local_10;
  if (0 < *(int *)(this + 0x150)) {
    this_00 = NInventory::CTCInventoryBase::GetCategoryWithIndex((CTCInventoryBase *)this,0);
    NInventory::CCategory::SetAsSelected(this_00,true);
    pCVar3 = local_14;
    pCVar4 = local_10;
  }
  for (; pCVar5 = local_10, pCVar3 != local_10; pCVar3 = pCVar3 + 8) {
    local_10 = pCVar4;
    (*(code *)**(undefined4 **)pCVar3)(0);
    pCVar4 = local_10;
    local_10 = pCVar5;
  }
  if (local_14 != (CBaseIntelligentPointer *)0x0) {
    local_10 = pCVar4;
    free(local_14);
  }
  return;
}



//=== GetQuestName @ 00702310 ===

/* [bsim sim=0.9476119764453593 <- ego_r]
   public: class CWideString __thiscall CTCQuestCard::GetQuestName(void)const  */

CWideString * __thiscall CTCQuestCard::GetQuestName(CTCQuestCard *this)

{
  int iVar1;
  char cVar2;
  CWideString *in_stack_00000004;
  CTCQuestCard *local_4;
  
  local_4 = this;
  cVar2 = FUN_0099e960(&DAT_0122d70e);
  if (cVar2 != '\0') {
    NGameText::CDataBank::GetTextBySymbol
              (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&local_4);
    cVar2 = FUN_0099b150(&DAT_0122d70c);
    if (cVar2 == '\0') {
      CPersistContext::GetDefaultVal<CAnimationSet>();
      CCharString::~CCharString((CCharString *)&local_4);
      return in_stack_00000004;
    }
    CWideString::CWideString(in_stack_00000004,(CWideString *)&local_4);
    CCharString::~CCharString((CCharString *)&local_4);
    return in_stack_00000004;
  }
  if ((*(int *)(*(int *)(this + 0xc) + 0x28) < 1) ||
     (NGameText::CDataBank::GetPGroupEntry
                (*(CDataBank **)(DAT_013b86a0 + 0x14),(ulong)&local_4,
                 SUB41(*(int *)(*(int *)(this + 0xc) + 0x28),0)), local_4 == (CTCQuestCard *)0x0)) {
    CWideString::CWideString(in_stack_00000004,(CWideString *)&DAT_013bca24);
    return in_stack_00000004;
  }
  CBankFile::GetBankHandle((CBankFile *)local_4);
  if (local_4 == (CTCQuestCard *)0x0) {
    return in_stack_00000004;
  }
  iVar1 = *(int *)(local_4 + 4);
  *(int *)(local_4 + 4) = iVar1 + -1;
  if (iVar1 + -1 != 0) {
    return in_stack_00000004;
  }
  (**(code **)(*(int *)local_4 + 4))();
  return in_stack_00000004;
}



//=== GetQuestName @ 00702470 ===

/* [bsim sim=0.9476119764453593 <- ego_r]
   public: class CWideString __thiscall CTCQuestCard::GetQuestName(void)const  */

CWideString * __thiscall CTCQuestCard::GetQuestName(CTCQuestCard *this)

{
  int iVar1;
  char cVar2;
  CWideString *in_stack_00000004;
  CTCQuestCard *local_4;
  
  local_4 = this;
  cVar2 = FUN_0099e960(&DAT_0122d70e);
  if (cVar2 != '\0') {
    NGameText::CDataBank::GetTextBySymbol
              (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&local_4);
    cVar2 = FUN_0099b150(&DAT_0122d70c);
    if (cVar2 == '\0') {
      CPersistContext::GetDefaultVal<CAnimationSet>();
      CCharString::~CCharString((CCharString *)&local_4);
      return in_stack_00000004;
    }
    CWideString::CWideString(in_stack_00000004,(CWideString *)&local_4);
    CCharString::~CCharString((CCharString *)&local_4);
    return in_stack_00000004;
  }
  if ((*(int *)(*(int *)(this + 0xc) + 0x30) < 1) ||
     (NGameText::CDataBank::GetPGroupEntry
                (*(CDataBank **)(DAT_013b86a0 + 0x14),(ulong)&local_4,
                 SUB41(*(int *)(*(int *)(this + 0xc) + 0x30),0)), local_4 == (CTCQuestCard *)0x0)) {
    CWideString::CWideString(in_stack_00000004,(CWideString *)&DAT_013bca24);
    return in_stack_00000004;
  }
  CBankFile::GetBankHandle((CBankFile *)local_4);
  if (local_4 == (CTCQuestCard *)0x0) {
    return in_stack_00000004;
  }
  iVar1 = *(int *)(local_4 + 4);
  *(int *)(local_4 + 4) = iVar1 + -1;
  if (iVar1 + -1 != 0) {
    return in_stack_00000004;
  }
  (**(code **)(*(int *)local_4 + 4))();
  return in_stack_00000004;
}



