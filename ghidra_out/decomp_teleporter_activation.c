//=== ConstructQuestsScreen_CurrentQuestsPC @ 005fc8a0 (seed 005fcb00) ===

/* [forge-autoname3 conf=high] Creates PC_SUB_LIST, PC_TITLE_QUEST_CURRENT; calls
   ConstructQuestListPC with Campfire_InsertRegionNode. PC platform variant of current quests. */

CComponent * ConstructQuestsScreen_CurrentQuestsPC(undefined4 param_1,int *param_2)

{
  int *piVar1;
  COptimisedPrimitive *this;
  long lVar2;
  CDefinitionManager *this_00;
  CFrontEndManager *this_01;
  CComponent *pCVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *in_ECX;
  int iVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar5;
  CTCInventoryQuests *unaff_EBX;
  int *piVar6;
  undefined1 *puVar7;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar9;
  int *piStack_28;
  int iStack_24;
  CBaseObject *pCStack_20;
  int *piStack_1c;
  CComponent *pCStack_18;
  undefined4 *puStack_14;
  void *pvStack_10;
  CComponent *pCStack_c;
  CComponent *pCStack_8;
  CList *pCStack_4;
  
  iVar4 = *param_2;
  ppVar9 = in_ECX;
  CCharString::CCharString((CCharString *)&stack0xffffffc4,"PC_UI_FRAME_QUESTS",-1);
  piStack_1c = (int *)(**(code **)(iVar4 + 0xc))();
  iVar4 = *(int *)(in_ECX + 4);
  iStack_24 = 0;
  if ((*(uint *)(iVar4 + 0x20) & 0x2000000) != 0) {
    pCStack_20 = (CBaseObject *)0x19;
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar4 + 0x44),(ETCInterfaceType *)&pCStack_20);
    if ((piVar1 == *(int **)(iVar4 + 0x48)) || (0x19 < *piVar1)) {
      piVar1 = *(int **)(iVar4 + 0x48);
    }
    iStack_24 = piVar1[1];
  }
  piVar6 = (int *)(DAT_013b89fc + 0x5c);
  pvStack_10 = (void *)0x0;
  pCStack_c = (CComponent *)0x0;
  pCStack_8 = (CComponent *)0x0;
  piVar1 = *(int **)*piVar6;
  if (piVar1 != (int *)*piVar6) {
    do {
      pCStack_20 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(piVar1 + 2));
      if (pCStack_c == pCStack_8) {
        Vector_InsertN(pCStack_c,&pCStack_20,&param_1,1,1);
      }
      else {
        if (pCStack_c != (CComponent *)0x0) {
          *(CBaseObject **)pCStack_c = pCStack_20;
        }
        pCStack_c = pCStack_c + 4;
      }
      piVar1 = (int *)*piVar1;
    } while (piVar1 != (int *)*piVar6);
  }
  piVar6 = (int *)*piVar6;
  piVar1 = (int *)*piVar6;
  iVar4 = 0;
  if (piVar1 != piVar6) {
    do {
      piVar1 = (int *)*piVar1;
      iVar4 = iVar4 + 1;
    } while (piVar1 != piVar6);
    if (iVar4 != 0) {
      pCStack_20 = (CBaseObject *)0x0;
      CCharString::CCharString((CCharString *)&stack0xffffffd4,"UI_MISC_THINGS_DEF",-1);
      puVar7 = &stack0xffffffd4;
      this = (COptimisedPrimitive *)GFGetPreMainMemoryUsed();
      lVar2 = QuadricOptimiserInternals::COptimisedPrimitive::ActualToUniqueIndex
                        (this,(ulong)puVar7);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffd4,
                 extraout_EDX,ppVar9);
      pCVar8 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&pCStack_20;
      this_00 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
      CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_00,lVar2,pCVar8);
      this_01 = NUISystem::CFrontEndManager::GetInstance();
      CCharString::CCharString((CCharString *)&stack0xffffffd4,"PC_SUB_LIST",-1);
      pCVar3 = NUISystem::CManager::CreateComponent
                         ((CManager *)this_01,(CCharString *)&stack0xffffffd4,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffd4,
                 extraout_EDX_00,ppVar9);
      CCharString::CCharString((CCharString *)&piStack_28,"PC_TITLE_QUEST_CURRENT",-1);
      pCStack_18 = NUISystem::CManager::CreateComponent
                             ((CManager *)this_01,(CCharString *)&piStack_28,false);
      ppVar5 = extraout_EDX_01;
      if ((pCStack_18 == (CComponent *)0x0) ||
         (puStack_14 = operator_new(0xc), ppVar5 = extraout_EDX_02, puStack_14 == (undefined4 *)0x0)
         ) {
        puStack_14 = (undefined4 *)0x0;
      }
      else {
        *puStack_14 = 1;
        puStack_14[1] = CCountedPointer<CEditTransactionBase>::DeleteData;
        puStack_14[2] = pCStack_18;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&piStack_28,ppVar5
                 ,ppVar9);
      (**(code **)(*(int *)pCVar3 + 0x244))(&pCStack_18);
      (**(code **)(*(int *)pCStack_20 + 0x1f0))(piStack_1c);
      CTCInventoryQuests::ConstructQuestListPC
                (unaff_EBX,pCStack_8,(vector<CThing*,std::allocator<CThing*>_> *)&pCStack_18,
                 (CList *)pCVar3,1,pCStack_4);
      if ((piStack_1c != (int *)0x0) && (*piStack_1c = *piStack_1c + -1, *piStack_1c == 0)) {
        (*(code *)piStack_1c[1])();
        operator_delete(piStack_1c);
      }
      pCStack_20 = (CBaseObject *)0x0;
      piStack_1c = (int *)0x0;
      if (piStack_28 != (int *)0x0) {
        piVar1 = piStack_28 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*piStack_28 + 4))();
        }
      }
      if (pCStack_18 != (CComponent *)0x0) {
        free(pCStack_18);
      }
      return pCVar3;
    }
  }
  if (pvStack_10 != (void *)0x0) {
    free(pvStack_10);
  }
  return (CComponent *)0x0;
}


// CALLEES of ConstructQuestsScreen_CurrentQuestsPC:
//   00bfe9bc  operator_delete
//   0099ebf0  CCharString
//   0041e5f2  GetInstance
//   0040f020  LowerBound
//   0041db1d  CreateComponent
//   009ad390  ActualToUniqueIndex
//   00436ac0  Vector_InsertN
//   00bfea14  free
//   0044c6b0  GFGetPreMainMemoryUsed
//   0061e6d0  ConstructQuestListPC
//   0042b312  GetDef<CSpecialAbilitiesDrunkennessDef>
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00a01b50  GetPItem
//   00bfea1a  operator_new

//=== ?SetTeleporterAsActive@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z @ 0089ee00 (seed 0089ee00) ===

/* [bsim sim=0.8080735803435295 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::SetRegionEntranceAsActive(class
   CScriptThing const &,bool)const  */

void __thiscall
CGameScriptInterface::_SetTeleporterAsActive_CGameScriptInterface__UBEXABVCScriptThing___N_Z
          (CGameScriptInterface *this,CScriptThing *param_1,bool param_2)

{
  CScriptThing *pCVar1;
  char cVar2;
  int iVar3;
  int *piVar4;
  undefined3 in_stack_00000009;
  
  pCVar1 = param_1;
  cVar2 = (**(code **)(*(int *)param_1 + 300))();
  if (cVar2 != '\0') {
    iVar3 = (**(code **)(*(int *)pCVar1 + 0x2c))();
    if ((*(uint *)(iVar3 + 0x38) & 0x20000000) != 0) {
      param_1 = (CScriptThing *)0xdd;
      piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar3 + 0x44),(ETCInterfaceType *)&param_1);
      if ((piVar4 == *(int **)(iVar3 + 0x48)) || (0xdd < *piVar4)) {
        piVar4 = *(int **)(iVar3 + 0x48);
      }
      CTCTeleporter::SetAsActive((CTCTeleporter *)piVar4[1],SUB41(_param_2,0));
    }
  }
  return;
}


// CALLEES of ?SetTeleporterAsActive@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z:
//   0040f020  LowerBound
//   007dfeb0  SetAsActive

