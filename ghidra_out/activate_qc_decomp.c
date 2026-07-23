//=== ActivateQuestCard @ 004b4aa0 ===

/* [bsim sim=0.7908786592255969 <- ego_r]
   public: bool __thiscall CQuestManager::ActivateQuestCard(class CThing *,bool) */

bool __thiscall CQuestManager::ActivateQuestCard(CQuestManager *this,CThing *param_1,bool param_2)

{
  CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_> *this_00;
  int *piVar1;
  int iVar2;
  CThing *this_01;
  CThing *this_02;
  char cVar3;
  bool bVar4;
  int *piVar5;
  CCharString *pCVar6;
  CDefString *this_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *unaff_EDI;
  undefined3 in_stack_00000009;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  CGuiControlTreePane::SortTreeRecursively((CGuiControlTreePane *)(this + 0x74),unaff_EDI);
  this_01 = param_1;
  if ((param_1 != (CThing *)0x0) && (((byte)param_1[0x91] & 1) == 0)) {
    if ((*(uint *)(param_1 + 0x2c) & 0x1000) != 0) {
      this_00 = (CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                 *)(param_1 + 0x44);
      param_1 = (CThing *)&DAT_0000006c;
      piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound(this_00,(ETCInterfaceType *)&param_1);
      if ((piVar5 == *(int **)(this_01 + 0x48)) || (0x6c < *piVar5)) {
        piVar5 = *(int **)(this_01 + 0x48);
      }
      param_1 = (CThing *)piVar5[1];
    }
    this_02 = param_1;
    cVar3 = FUN_00702810();
    if ((cVar3 != '\0') && (piVar5 = *(int **)(this_02 + 0xc), piVar5 != (int *)0x0)) {
      piVar5[1] = piVar5[1] + 1;
      pCVar6 = (CCharString *)CTCQuestCard::GetRegionName((CTCQuestCard *)this_02);
      bVar4 = IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(this,pCVar6);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_c,
                 extraout_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EDI);
      if ((!bVar4) && (cVar3 = FUN_004b4a10(param_1 + 0x28,1,_param_2), cVar3 != '\0')) {
        ::CIntelligentPointer<CThing_const_>::CIntelligentPointer<CThing_const_>
                  ((CIntelligentPointer<CThing_const_> *)&local_c,this_01);
        FUN_004ba4e0(&local_c);
        CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)&local_c);
        CThing::SetInLimbo(this_01,true);
        CThing::SetAsGameStatePersisted(this_01,true);
        iVar2 = *(int *)(this + 0x7c);
        local_c = 0;
        *(undefined4 *)(iVar2 + 0x154) = 0;
        local_8 = 0;
        *(undefined4 *)(iVar2 + 0x158) = 0;
        local_4 = 0;
        *(undefined4 *)(iVar2 + 0x15c) = 0;
        *(undefined4 *)(*(int *)(this + 0x7c) + 0x160) = 0;
        this_03 = (CDefString *)CThing::GetDefName(this_01);
        CDefString::operator_class_CCharString(this_03);
        RemoveAvailableQuestCardType(this,(CCharString *)&param_1);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                   extraout_EDX_00,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EDI);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (DAT_013b8790,extraout_EDX_01,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0);
        piVar1 = piVar5 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*piVar5 + 4))();
        }
        return true;
      }
      piVar1 = piVar5 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar5 + 4))();
      }
    }
  }
  return false;
}



//=== ?AddQuestCard@CGameScriptInterface@@UBEXABVCCharString@@0_N1@Z @ 008913f0 ===

void _AddQuestCard_CGameScriptInterface__UBEXABVCCharString__0_N1_Z
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,CCharString param_4)

{
  int iVar1;
  int *in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CCharString aCStack_4 [4];
  
  iVar1 = (**(code **)(*in_ECX + 0xa34))();
  if (DAT_013baf9c != iVar1) {
    if (param_4 == (CCharString)0x0) {
      CCharString::CCharString(aCStack_4,"HUD_ORB_QUEST_CORE",-1);
      CCharString::CCharString(&param_4,"TEXT_QST_078_GM_MSG_NEW_QUEST",-1);
      (**(code **)(*in_ECX + 0x448))(&param_4,aCStack_4,1,0);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_4,
                 extraout_EDX,unaff_ESI);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4,
                 extraout_EDX_00,unaff_ESI);
    }
    DAT_013baf9c = (**(code **)(*in_ECX + 0xa34))();
  }
  FUN_004b1670(param_1,param_2,param_3);
  return;
}



