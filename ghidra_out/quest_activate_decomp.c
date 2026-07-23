//=== ActivateMultipleQuests @ 004b4260 ===

/* [bsim sim=0.8597446507845358 <- ego_r]
   public: bool __thiscall CQuestManager::ActivateMultipleQuests(class CArray<class CCharString>
   const &,bool,bool) */

bool __thiscall
CQuestManager::ActivateMultipleQuests
          (CQuestManager *this,CArray<CCharString> *param_1,bool param_2,bool param_3)

{
  CCharString *pCVar1;
  CArray<CCharString> *pCVar2;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *_Memory;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar3;
  bool bVar4;
  int iVar5;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  bool bVar7;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  uint local_2c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *local_24;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *local_20;
  undefined4 local_1c;
  CCharString local_18 [4];
  int local_14;
  undefined1 local_10;
  undefined1 local_f;
  CCharString local_c [4];
  undefined4 local_8;
  undefined1 local_4;
  undefined1 local_3;
  
  pCVar2 = param_1;
  bVar7 = false;
  local_24 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
  local_20 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
  local_1c = 0;
  local_2c = 0;
  if (*(int *)(param_1 + 4) - *(int *)param_1 >> 2 != 0) {
    do {
      CCharString::CCharString((CCharString *)&param_1,"QuestManager: Activate Quest",-1);
      NProgressDisplay::DisplayProgress
                ((CCharString *)&param_1,(float)(extraout_EDX & 0xffffff00),false,false);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_00,unaff_EDI);
      pCVar1 = (CCharString *)(*(int *)pCVar2 + local_2c * 4);
      bVar4 = IsQuestRegistered(this,pCVar1);
      if (bVar4) {
        iVar5 = FUN_00cb5ad0(pCVar1);
        if (iVar5 == 0) {
          CCharString::CCharString(local_c,pCVar1);
          local_3 = param_3;
          local_8 = 0;
          local_4 = param_2;
          FUN_004bb720(local_c);
          paVar8 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c;
          ppVar6 = extraout_EDX_02;
        }
        else {
          CCharString::CCharString(local_18,pCVar1);
          local_f = param_3;
          local_10 = param_2;
          local_14 = iVar5;
          FUN_004bb720(local_18);
          paVar8 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18;
          ppVar6 = extraout_EDX_01;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar8,ppVar6,unaff_EDI);
        bVar7 = true;
      }
      local_2c = local_2c + 1;
    } while (local_2c < (uint)(*(int *)(pCVar2 + 4) - *(int *)pCVar2 >> 2));
  }
  FUN_004b3ce0(&local_24);
  paVar3 = local_20;
  _Memory = local_24;
  ppVar6 = extraout_EDX_03;
  for (paVar8 = local_24; paVar8 != paVar3; paVar8 = paVar8 + 0xc) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (paVar8,ppVar6,unaff_EDI);
    ppVar6 = extraout_EDX_04;
  }
  if (_Memory != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
    free(_Memory);
  }
  return bVar7;
}



//=== ?ActivateMultipleQuests@CGameScriptInterface@@UBEXABV?$CArray@VCCharString@@@@@Z @ 00892ea0 ===

void _ActivateMultipleQuests_CGameScriptInterface__UBEXABV__CArray_VCCharString_____Z
               (CArray<CCharString> *param_1)

{
  CQuestManager::ActivateMultipleQuests(DAT_013b89fc,param_1,true,true);
  return;
}



//=== ?ActivateMultipleQuestsWithoutLoadingResources@CGameScriptInterface@@UBEXABV?$CArray@VCCharString@@@@@Z @ 00892ee0 ===

void _ActivateMultipleQuestsWithoutLoadingResources_CGameScriptInterface__UBEXABV__CArray_VCCharString_____Z
               (CArray<CCharString> *param_1)

{
  CQuestManager::ActivateMultipleQuests(DAT_013b89fc,param_1,true,false);
  return;
}



