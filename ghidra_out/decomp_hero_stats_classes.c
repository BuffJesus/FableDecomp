//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 0040e9a0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x1c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 7; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00410820 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00415890 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 004196b2 ===

/* [bsim sim=0.8063763850608386 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar2;
  CCharString *pCVar3;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar2 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar3 = *(CCharString **)param_2;
  if (pCVar3 != param_1) {
    if (pCVar3 != (CCharString *)0x0) {
      pCVar1 = pCVar3 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar3 + 4))();
      }
      *(undefined4 *)pCVar2 = 0;
    }
    *(CCharString **)pCVar2 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  pCVar3 = param_1 + 4;
  *(int *)pCVar3 = *(int *)pCVar3 + -1;
  if (*(int *)pCVar3 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== CTCHeroPodium @ 0041cadc ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall CTCHeroPodium::CTCHeroPodium(class CThing &) */

CTCHeroPodium * __thiscall CTCHeroPodium::CTCHeroPodium(CTCHeroPodium *this,CThing *param_1)

{
  NUISystem::CDraggableInto::CDraggableInto((CDraggableInto *)this,(long)param_1);
  *(undefined4 *)(this + 0x15c) = 0;
  *(undefined4 *)(this + 0x160) = 0;
  *(undefined ***)this = &PTR__vector_deleting_destructor__0122f884;
  *(undefined ***)(this + 4) = &PTR_LAB_0122f85c;
  *(undefined ***)(this + 0x18) = &PTR_LAB_0122f854;
  return this;
}



//=== AddMoralityChange @ 00437650 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CPlayerGui::AddMoralityChange(long) */

void __thiscall CPlayerGui::AddMoralityChange(CPlayerGui *this,long param_1)

{
  *(long *)(*(int *)(this + 0x220) + 0x40) = param_1;
  return;
}



//=== AddMoralityChange @ 00437920 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CPlayerGui::AddMoralityChange(long) */

void __thiscall CPlayerGui::AddMoralityChange(CPlayerGui *this,long param_1)

{
  *(long *)(*(int *)(this + 0x1a8) + 0x28) = param_1;
  return;
}



//=== AddMoralityChange @ 00437930 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CPlayerGui::AddMoralityChange(long) */

void __thiscall CPlayerGui::AddMoralityChange(CPlayerGui *this,long param_1)

{
  *(long *)(*(int *)(this + 0x1a8) + 0x2c) = param_1;
  return;
}



//=== UpdateRenown @ 00439e00 ===

/* [bsim sim=0.9988580390584354 <- ego_r]
   private: void __thiscall CPlayerGui::UpdateRenown(void) */

void __thiscall CPlayerGui::UpdateRenown(CPlayerGui *this)

{
  int iVar1;
  int extraout_EAX;
  CPlayerGuiDef *pCVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  undefined8 uVar3;
  float fVar4;
  CStatBase *pCVar5;
  int iStack_4;
  
  uVar3 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),unaff_ESI);
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    iVar1 = *(int *)(*(int *)(this + 0x1a8) + 0x28);
    pCVar5 = *(CStatBase **)(this + 0x2d4);
    if (pCVar5[8] == (CStatBase)0x0) {
      fVar4 = (float)iVar1;
      *(float *)(*(int *)(this + 0x2d4) + 0xc) = fVar4;
      *(float *)(*(int *)(this + 0x2d4) + 0x10) = fVar4;
      *(float *)(*(int *)(this + 0x2d4) + 0x14) = fVar4;
      *(undefined1 *)(*(int *)(this + 0x2d4) + 8) = 1;
    }
    else {
      iStack_4 = __ftol2();
      if (iStack_4 < 1) {
        iStack_4 = 1;
      }
      pCVar2 = PeekGuiDef();
      fVar4 = *(float *)(pCVar2 + 0x4ec);
      pCVar2 = PeekGuiDef();
      UpdateStat(this,(float)iVar1,0.01,(float)iStack_4,*(float *)(pCVar2 + 0x4e8),fVar4,pCVar5);
      *(undefined4 *)(*(int *)(this + 0x2d4) + 0x4c) =
           *(undefined4 *)(*(int *)(this + 0x2d4) + 0x48);
      *(int *)(*(int *)(this + 0x2d4) + 0x48) = *(int *)(*(int *)(this + 0x2d4) + 0x48) + 1;
    }
    if ((0 < iVar1) && (*(int *)(*(int *)(this + 0x2d4) + 0x28) == 1)) {
      *(undefined4 *)(*(int *)(this + 0x1a8) + 0x28) = 0;
      *(undefined1 *)(*(int *)(this + 0x2d4) + 8) = 0;
    }
  }
  return;
}



//=== UpdateMorality @ 00439f20 ===

/* [bsim sim=0.9426302235083713 <- ego_r]
   private: void __thiscall CPlayerGui::UpdateMorality(void) */

void __thiscall CPlayerGui::UpdateMorality(CPlayerGui *this)

{
  int extraout_EAX;
  CPlayerGuiDef *pCVar1;
  int iVar2;
  int iVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  undefined8 uVar4;
  float fVar5;
  CStatBase *pCVar6;
  int iStack_4;
  
  uVar4 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar4,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar4 >> 0x20),unaff_ESI);
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    iVar3 = *(int *)(*(int *)(this + 0x1a8) + 0x2c);
    pCVar6 = *(CStatBase **)(this + 0x2dc);
    if (pCVar6[8] == (CStatBase)0x0) {
      fVar5 = (float)iVar3;
      *(float *)(*(int *)(this + 0x2dc) + 0xc) = fVar5;
      *(float *)(*(int *)(this + 0x2dc) + 0x10) = fVar5;
      *(float *)(*(int *)(this + 0x2dc) + 0x14) = fVar5;
      *(undefined1 *)(*(int *)(this + 0x2dc) + 8) = 1;
    }
    else {
      iStack_4 = __ftol2();
      if (iStack_4 < 1) {
        iStack_4 = 1;
      }
      pCVar1 = PeekGuiDef();
      fVar5 = *(float *)(pCVar1 + 0x4ec);
      pCVar1 = PeekGuiDef();
      UpdateStat(this,(float)iVar3,0.01,(float)iStack_4,*(float *)(pCVar1 + 0x4e8),fVar5,pCVar6);
      *(undefined4 *)(*(int *)(this + 0x2dc) + 0x4c) =
           *(undefined4 *)(*(int *)(this + 0x2dc) + 0x48);
      *(int *)(*(int *)(this + 0x2dc) + 0x48) = *(int *)(*(int *)(this + 0x2dc) + 0x48) + 1;
    }
    if (iVar3 != 0) {
      if (0.0 <= (float)iVar3) {
        iVar3 = 1;
      }
      else {
        iVar3 = -1;
      }
      iVar2 = -1;
      if (0.0 <= *(float *)(*(int *)(this + 0x2dc) + 0x10)) {
        iVar2 = 1;
      }
      if (iVar3 != iVar2) {
        *(undefined1 *)(*(int *)(this + 0x2dc) + 8) = 0;
      }
      if (*(int *)(*(int *)(this + 0x2dc) + 0x28) == 1) {
        *(undefined4 *)(*(int *)(this + 0x1a8) + 0x2c) = 0;
        *(undefined1 *)(*(int *)(this + 0x2dc) + 8) = 0;
        return;
      }
    }
  }
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 0043ff30 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== CHeroMarriageDef @ 0044c29c ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CHeroMarriageDef::CHeroMarriageDef(void) */

CHeroMarriageDef * __thiscall CHeroMarriageDef::CHeroMarriageDef(CHeroMarriageDef *this)

{
  CShoveHaPennyRegionDef::CShoveHaPennyRegionDef((CShoveHaPennyRegionDef *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01232074;
  CCharString::CCharString((CCharString *)(this + 0x7c));
  CCharString::CCharString((CCharString *)(this + 0xa4));
  CCharString::CCharString((CCharString *)(this + 0xa8));
  CCharString::CCharString((CCharString *)(this + 0xb8));
  CCharString::CCharString((CCharString *)(this + 0xc4));
  return this;
}



//=== ?GetSizeofClass@CHeroMarriageDef@@UAEKXZ @ 0044c2e2 ===

undefined4 _GetSizeofClass_CHeroMarriageDef__UAEKXZ(void)

{
  return 200;
}



//=== ?GetSizeofClass@CHeroTitleDef@@UAEKXZ @ 0044c3a6 ===

undefined4 _GetSizeofClass_CHeroTitleDef__UAEKXZ(void)

{
  return 0x50;
}



//=== ~CHeroMarriageDef @ 0044f81a ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual __thiscall CHeroMarriageDef::~CHeroMarriageDef(void) */

void __thiscall CHeroMarriageDef::~CHeroMarriageDef(CHeroMarriageDef *this)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xc4),in_EDX,
             unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xb8),
             extraout_EDX,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xa8),
             extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xa4),
             extraout_EDX_01,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x7c),
             extraout_EDX_02,unaff_ESI);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01230ba0;
  CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)this);
  return;
}



//=== ?Transfer@CHeroMarriageDef@@UAEXAAVCPersistContext@@@Z @ 00452398 ===

/* [bsim sim=0.6796269797765095 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CHeroMarriageDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  FUN_00431143(in_ECX + 0x7c);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  FUN_00431143(in_ECX + 0xa4);
  FUN_00431143(in_ECX + 0xa8);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  FUN_00431143(in_ECX + 0xb8);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  FUN_00431143(in_ECX + 0xc4);
  return;
}



//=== ?Transfer@CHeroTitleDef@@UAEXAAVCPersistContext@@@Z @ 00452884 ===

/* [bsim sim=0.7746988183602541 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CHeroTitleDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d));
  return;
}



//=== ?Copy@CHeroMarriageDef@@UAEXPBVCDefClassBase@@@Z @ 0045390c ===

void _Copy_CHeroMarriageDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x28) = *(undefined4 *)(param_1 + 0x28);
  *(undefined4 *)(in_ECX + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
  *(undefined4 *)(in_ECX + 0x30) = *(undefined4 *)(param_1 + 0x30);
  *(undefined4 *)(in_ECX + 0x34) = *(undefined4 *)(param_1 + 0x34);
  *(undefined4 *)(in_ECX + 0x38) = *(undefined4 *)(param_1 + 0x38);
  *(undefined4 *)(in_ECX + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  *(undefined4 *)(in_ECX + 0x48) = *(undefined4 *)(param_1 + 0x48);
  *(undefined4 *)(in_ECX + 0x4c) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(in_ECX + 0x50) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(in_ECX + 0x54) = *(undefined4 *)(param_1 + 0x54);
  *(undefined4 *)(in_ECX + 0x58) = *(undefined4 *)(param_1 + 0x58);
  *(undefined4 *)(in_ECX + 0x5c) = *(undefined4 *)(param_1 + 0x5c);
  *(undefined4 *)(in_ECX + 0x60) = *(undefined4 *)(param_1 + 0x60);
  *(undefined4 *)(in_ECX + 100) = *(undefined4 *)(param_1 + 100);
  *(undefined4 *)(in_ECX + 0x68) = *(undefined4 *)(param_1 + 0x68);
  *(undefined4 *)(in_ECX + 0x6c) = *(undefined4 *)(param_1 + 0x6c);
  *(undefined4 *)(in_ECX + 0x70) = *(undefined4 *)(param_1 + 0x70);
  *(undefined4 *)(in_ECX + 0x74) = *(undefined4 *)(param_1 + 0x74);
  *(undefined4 *)(in_ECX + 0x78) = *(undefined4 *)(param_1 + 0x78);
  CCharString::operator=((CCharString *)(in_ECX + 0x7c),(CCharString *)(param_1 + 0x7c));
  *(undefined4 *)(in_ECX + 0x80) = *(undefined4 *)(param_1 + 0x80);
  *(undefined4 *)(in_ECX + 0x84) = *(undefined4 *)(param_1 + 0x84);
  *(undefined4 *)(in_ECX + 0x88) = *(undefined4 *)(param_1 + 0x88);
  *(undefined4 *)(in_ECX + 0x8c) = *(undefined4 *)(param_1 + 0x8c);
  *(undefined4 *)(in_ECX + 0x90) = *(undefined4 *)(param_1 + 0x90);
  *(undefined4 *)(in_ECX + 0x94) = *(undefined4 *)(param_1 + 0x94);
  *(undefined4 *)(in_ECX + 0x98) = *(undefined4 *)(param_1 + 0x98);
  *(undefined4 *)(in_ECX + 0x9c) = *(undefined4 *)(param_1 + 0x9c);
  *(undefined4 *)(in_ECX + 0xa0) = *(undefined4 *)(param_1 + 0xa0);
  CCharString::operator=((CCharString *)(in_ECX + 0xa4),(CCharString *)(param_1 + 0xa4));
  CCharString::operator=((CCharString *)(in_ECX + 0xa8),(CCharString *)(param_1 + 0xa8));
  *(undefined4 *)(in_ECX + 0xac) = *(undefined4 *)(param_1 + 0xac);
  *(undefined4 *)(in_ECX + 0xb0) = *(undefined4 *)(param_1 + 0xb0);
  *(undefined4 *)(in_ECX + 0xb4) = *(undefined4 *)(param_1 + 0xb4);
  CCharString::operator=((CCharString *)(in_ECX + 0xb8),(CCharString *)(param_1 + 0xb8));
  *(undefined4 *)(in_ECX + 0xbc) = *(undefined4 *)(param_1 + 0xbc);
  *(undefined4 *)(in_ECX + 0xc0) = *(undefined4 *)(param_1 + 0xc0);
  CCharString::operator=((CCharString *)(in_ECX + 0xc4),(CCharString *)(param_1 + 0xc4));
  return;
}



//=== ?Copy@CHeroTitleDef@@UAEXPBVCDefClassBase@@@Z @ 00453ce5 ===

void _Copy_CHeroTitleDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x28) = *(undefined4 *)(param_1 + 0x28);
  *(undefined4 *)(in_ECX + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
  *(undefined4 *)(in_ECX + 0x30) = *(undefined4 *)(param_1 + 0x30);
  *(undefined4 *)(in_ECX + 0x34) = *(undefined4 *)(param_1 + 0x34);
  *(undefined4 *)(in_ECX + 0x38) = *(undefined4 *)(param_1 + 0x38);
  *(undefined4 *)(in_ECX + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  *(undefined4 *)(in_ECX + 0x48) = *(undefined4 *)(param_1 + 0x48);
  in_ECX[0x4c] = param_1[0x4c];
  in_ECX[0x4d] = param_1[0x4d];
  return;
}



//=== ?GetSizeofClass@CHeroAbilityDef@@UAEKXZ @ 00456087 ===

undefined4 _GetSizeofClass_CHeroAbilityDef__UAEKXZ(void)

{
  return 0x9c;
}



//=== CHeroStatsDef @ 004560e1 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall CHeroStatsDef::CHeroStatsDef(void) */

CHeroStatsDef * __thiscall CHeroStatsDef::CHeroStatsDef(CHeroStatsDef *this)

{
  CParentDefClassBase::CParentDefClassBase((CParentDefClassBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__012339dc;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0xac) = 0;
  *(undefined4 *)(this + 0xb0) = 0;
  *(undefined4 *)(this + 0xb4) = 0;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  *(undefined4 *)(this + 0xc0) = 0;
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 200) = 0;
  *(undefined4 *)(this + 0xcc) = 0;
  *(undefined4 *)(this + 0xf0) = 0;
  *(undefined4 *)(this + 0xf4) = 0;
  *(undefined4 *)(this + 0xf8) = 0;
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x104) = 0;
  *(undefined4 *)(this + 0x108) = 0;
  *(undefined4 *)(this + 0x10c) = 0;
  *(undefined4 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0x114) = 0;
  *(undefined4 *)(this + 0x118) = 0;
  *(undefined4 *)(this + 0x11c) = 0;
  *(undefined4 *)(this + 0x120) = 0;
  *(undefined4 *)(this + 0x124) = 0;
  *(undefined4 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 300) = 0;
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined4 *)(this + 0x134) = 0;
  return this;
}



//=== ?GetSizeofClass@CHeroStatsDef@@UAEKXZ @ 0045617c ===

undefined4 _GetSizeofClass_CHeroStatsDef__UAEKXZ(void)

{
  return 0x138;
}



//=== ?GetSizeofClass@CHeroPostcardGeneratorDef@@UAEKXZ @ 0045687f ===

undefined4 _GetSizeofClass_CHeroPostcardGeneratorDef__UAEKXZ(void)

{
  return 0x5c;
}



//=== ?Transfer@CHeroPostcardGeneratorDef@@UAEXAAVCPersistContext@@@Z @ 00456883 ===

/* [bsim sim=0.7521279987471763 <- ego_r]
   public: void __thiscall std::deque<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >::insert<class std::_Deque_const_iterator<class NUISystem::CComponent
   *,class std::allocator<class NUISystem::CComponent *> > >(class std::_Deque_const_iterator<class
   NUISystem::CComponent *,class std::allocator<class NUISystem::CComponent *> >,class
   std::_Deque_const_iterator<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >,class std::_Deque_const_iterator<class NUISystem::CComponent *,class
   std::allocator<class NUISystem::CComponent *> >) */

void __thiscall
std::deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>::
_Transfer_CHeroPostcardGeneratorDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x39));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x41));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x49));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x58));
  return;
}



//=== ?GetSizeofClass@CHeroCombatDef@@UAEKXZ @ 00458b7b ===

undefined4 _GetSizeofClass_CHeroCombatDef__UAEKXZ(void)

{
  return 0x148;
}



//=== ?Copy@CHeroAbilityDef@@UAEXPBVCDefClassBase@@@Z @ 0045cb17 ===

void _Copy_CHeroAbilityDef__UAEXPBVCDefClassBase___Z(CParentDefClassBase *param_1)

{
  CParentDefClassBase *in_ECX;
  
  CParentDefClassBase::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  *(undefined4 *)(in_ECX + 0x48) = *(undefined4 *)(param_1 + 0x48);
  *(undefined4 *)(in_ECX + 0x4c) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(in_ECX + 0x50) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(in_ECX + 0x54) = *(undefined4 *)(param_1 + 0x54);
  *(undefined4 *)(in_ECX + 0x58) = *(undefined4 *)(param_1 + 0x58);
  *(undefined4 *)(in_ECX + 0x5c) = *(undefined4 *)(param_1 + 0x5c);
  FUN_00454886(param_1 + 0x60);
  *(undefined4 *)(in_ECX + 0x6c) = *(undefined4 *)(param_1 + 0x6c);
  FUN_00454886(param_1 + 0x70);
  *(undefined4 *)(in_ECX + 0x7c) = *(undefined4 *)(param_1 + 0x7c);
  FUN_00454886(param_1 + 0x80);
  *(undefined4 *)(in_ECX + 0x8c) = *(undefined4 *)(param_1 + 0x8c);
  *(undefined4 *)(in_ECX + 0x90) = *(undefined4 *)(param_1 + 0x90);
  in_ECX[0x94] = param_1[0x94];
  *(undefined4 *)(in_ECX + 0x98) = *(undefined4 *)(param_1 + 0x98);
  return;
}



//=== ?Copy@CHeroPostcardGeneratorDef@@UAEXPBVCDefClassBase@@@Z @ 0045d5ce ===

void _Copy_CHeroPostcardGeneratorDef__UAEXPBVCDefClassBase___Z(CParentDefClassBase *param_1)

{
  CParentDefClassBase *in_ECX;
  
  CParentDefClassBase::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x39) = *(undefined4 *)(param_1 + 0x39);
  *(undefined4 *)(in_ECX + 0x3d) = *(undefined4 *)(param_1 + 0x3d);
  *(undefined4 *)(in_ECX + 0x41) = *(undefined4 *)(param_1 + 0x41);
  *(undefined4 *)(in_ECX + 0x45) = *(undefined4 *)(param_1 + 0x45);
  *(undefined4 *)(in_ECX + 0x49) = *(undefined4 *)(param_1 + 0x49);
  *(undefined4 *)(in_ECX + 0x4d) = *(undefined4 *)(param_1 + 0x4d);
  *(undefined4 *)(in_ECX + 0x51) = *(undefined4 *)(param_1 + 0x51);
  *(undefined4 *)(in_ECX + 0x58) = *(undefined4 *)(param_1 + 0x58);
  return;
}



//=== ?Copy@CHeroCombatDef@@UAEXPBVCDefClassBase@@@Z @ 00461f77 ===

void _Copy_CHeroCombatDef__UAEXPBVCDefClassBase___Z(CParentDefClassBase *param_1)

{
  CParentDefClassBase *in_ECX;
  
  CParentDefClassBase::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  *(undefined4 *)(in_ECX + 0x48) = *(undefined4 *)(param_1 + 0x48);
  *(undefined4 *)(in_ECX + 0x4c) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(in_ECX + 0x50) = *(undefined4 *)(param_1 + 0x50);
  FUN_0046230e(param_1 + 0x54);
  *(undefined4 *)(in_ECX + 0x60) = *(undefined4 *)(param_1 + 0x60);
  *(undefined4 *)(in_ECX + 100) = *(undefined4 *)(param_1 + 100);
  FUN_0046230e(param_1 + 0x68);
  *(undefined4 *)(in_ECX + 0x74) = *(undefined4 *)(param_1 + 0x74);
  FUN_0046230e(param_1 + 0x78);
  *(undefined4 *)(in_ECX + 0x84) = *(undefined4 *)(param_1 + 0x84);
  *(undefined4 *)(in_ECX + 0x88) = *(undefined4 *)(param_1 + 0x88);
  *(undefined4 *)(in_ECX + 0x8c) = *(undefined4 *)(param_1 + 0x8c);
  *(undefined4 *)(in_ECX + 0x90) = *(undefined4 *)(param_1 + 0x90);
  *(undefined4 *)(in_ECX + 0x94) = *(undefined4 *)(param_1 + 0x94);
  *(undefined4 *)(in_ECX + 0x98) = *(undefined4 *)(param_1 + 0x98);
  FUN_0046230e(param_1 + 0x9c);
  *(undefined4 *)(in_ECX + 0xa8) = *(undefined4 *)(param_1 + 0xa8);
  *(undefined4 *)(in_ECX + 0xac) = *(undefined4 *)(param_1 + 0xac);
  FUN_0046230e(param_1 + 0xb0);
  *(undefined4 *)(in_ECX + 0xbc) = *(undefined4 *)(param_1 + 0xbc);
  *(undefined4 *)(in_ECX + 0xc0) = *(undefined4 *)(param_1 + 0xc0);
  FUN_0046230e(param_1 + 0xc4);
  in_ECX[0xd0] = param_1[0xd0];
  *(undefined4 *)(in_ECX + 0xd4) = *(undefined4 *)(param_1 + 0xd4);
  in_ECX[0xd8] = param_1[0xd8];
  *(undefined4 *)(in_ECX + 0xdc) = *(undefined4 *)(param_1 + 0xdc);
  in_ECX[0xe0] = param_1[0xe0];
  in_ECX[0xe1] = param_1[0xe1];
  in_ECX[0xe2] = param_1[0xe2];
  *(undefined4 *)(in_ECX + 0xe4) = *(undefined4 *)(param_1 + 0xe4);
  *(undefined4 *)(in_ECX + 0xe8) = *(undefined4 *)(param_1 + 0xe8);
  *(undefined4 *)(in_ECX + 0xec) = *(undefined4 *)(param_1 + 0xec);
  *(undefined4 *)(in_ECX + 0xf0) = *(undefined4 *)(param_1 + 0xf0);
  *(undefined4 *)(in_ECX + 0xf4) = *(undefined4 *)(param_1 + 0xf4);
  FUN_0046230e(param_1 + 0xf8);
  *(undefined4 *)(in_ECX + 0x104) = *(undefined4 *)(param_1 + 0x104);
  *(undefined4 *)(in_ECX + 0x108) = *(undefined4 *)(param_1 + 0x108);
  *(undefined4 *)(in_ECX + 0x10c) = *(undefined4 *)(param_1 + 0x10c);
  *(undefined4 *)(in_ECX + 0x110) = *(undefined4 *)(param_1 + 0x110);
  *(undefined4 *)(in_ECX + 0x114) = *(undefined4 *)(param_1 + 0x114);
  *(undefined4 *)(in_ECX + 0x118) = *(undefined4 *)(param_1 + 0x118);
  FUN_00454886(param_1 + 0x11c);
  FUN_0045ef2d(param_1 + 0x128);
  in_ECX[0x134] = param_1[0x134];
  in_ECX[0x135] = param_1[0x135];
  in_ECX[0x136] = param_1[0x136];
  *(undefined4 *)(in_ECX + 0x138) = *(undefined4 *)(param_1 + 0x138);
  FUN_004621e9(param_1 + 0x13c);
  return;
}



//=== ?Transfer@CHeroCombatDef@@UAEXAAVCPersistContext@@@Z @ 004665b6 ===

/* [bsim sim=0.6740286750992224 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CHeroCombatDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe1));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe2));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x104));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x108));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x110));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x13c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x135));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x136));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x138));
  return;
}



//=== ?Transfer@CHeroAbilityDef@@UAEXAAVCPersistContext@@@Z @ 00469ab3 ===

/* [bsim sim=0.7377739590628846 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CHeroAbilityDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  return;
}



//=== _Copy_impl<std::_List_unchecked_iterator<std::_List_val<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>,std::back_insert_iterator<std::vector<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>_> @ 00471be9 ===

/* [bsim sim=0.6796773511494192 <- ego_r]
   class std::back_insert_iterator<class std::vector<class CHeroMarriageInfo,class
   std::allocator<class CHeroMarriageInfo> > > __fastcall std::_Copy_impl<class
   std::_List_unchecked_iterator<class std::_List_val<class CHeroMarriageInfo,class
   std::allocator<class CHeroMarriageInfo> > >,class std::back_insert_iterator<class
   std::vector<class CHeroMarriageInfo,class std::allocator<class CHeroMarriageInfo> > > >(class
   std::_List_unchecked_iterator<class std::_List_val<class CHeroMarriageInfo,class
   std::allocator<class CHeroMarriageInfo> > >,class std::_List_unchecked_iterator<class
   std::_List_val<class CHeroMarriageInfo,class std::allocator<class CHeroMarriageInfo> > >,class
   std::back_insert_iterator<class std::vector<class CHeroMarriageInfo,class std::allocator<class
   CHeroMarriageInfo> > >,struct std::_Nonscalar_ptr_iterator_tag) */

void __fastcall
std::
_Copy_impl<std::_List_unchecked_iterator<std::_List_val<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>,std::back_insert_iterator<std::vector<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>_>
          (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int *param_4,int *param_5)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  while (param_4 != param_5) {
    iVar1 = *param_4;
    piVar2 = (int *)param_4[1];
    piVar3 = (int *)*param_4;
    *piVar2 = iVar1;
    *(int **)(iVar1 + 4) = piVar2;
    free(param_4);
    param_4 = piVar3;
  }
  *param_3 = param_5;
  return;
}



//=== ?Copy@CHeroStatsDef@@UAEXPBVCDefClassBase@@@Z @ 0047812d ===

void _Copy_CHeroStatsDef__UAEXPBVCDefClassBase___Z(CParentDefClassBase *param_1)

{
  CParentDefClassBase *in_ECX;
  
  CParentDefClassBase::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x3c) = *(undefined4 *)(param_1 + 0x3c);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  *(undefined4 *)(in_ECX + 0x44) = *(undefined4 *)(param_1 + 0x44);
  *(undefined4 *)(in_ECX + 0x48) = *(undefined4 *)(param_1 + 0x48);
  *(undefined4 *)(in_ECX + 0x4c) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(in_ECX + 0x50) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(in_ECX + 0x54) = *(undefined4 *)(param_1 + 0x54);
  *(undefined4 *)(in_ECX + 0x58) = *(undefined4 *)(param_1 + 0x58);
  *(undefined4 *)(in_ECX + 0x5c) = *(undefined4 *)(param_1 + 0x5c);
  *(undefined4 *)(in_ECX + 0x60) = *(undefined4 *)(param_1 + 0x60);
  *(undefined4 *)(in_ECX + 100) = *(undefined4 *)(param_1 + 100);
  *(undefined4 *)(in_ECX + 0x68) = *(undefined4 *)(param_1 + 0x68);
  *(undefined4 *)(in_ECX + 0x6c) = *(undefined4 *)(param_1 + 0x6c);
  FUN_0047832d(param_1 + 0x70);
  *(undefined4 *)(in_ECX + 0x7c) = *(undefined4 *)(param_1 + 0x7c);
  *(undefined4 *)(in_ECX + 0x80) = *(undefined4 *)(param_1 + 0x80);
  *(undefined4 *)(in_ECX + 0x84) = *(undefined4 *)(param_1 + 0x84);
  *(undefined4 *)(in_ECX + 0x88) = *(undefined4 *)(param_1 + 0x88);
  *(undefined4 *)(in_ECX + 0x8c) = *(undefined4 *)(param_1 + 0x8c);
  *(undefined4 *)(in_ECX + 0x90) = *(undefined4 *)(param_1 + 0x90);
  *(undefined4 *)(in_ECX + 0x94) = *(undefined4 *)(param_1 + 0x94);
  *(undefined4 *)(in_ECX + 0x98) = *(undefined4 *)(param_1 + 0x98);
  *(undefined4 *)(in_ECX + 0x9c) = *(undefined4 *)(param_1 + 0x9c);
  *(undefined4 *)(in_ECX + 0xa0) = *(undefined4 *)(param_1 + 0xa0);
  *(undefined4 *)(in_ECX + 0xa4) = *(undefined4 *)(param_1 + 0xa4);
  *(undefined4 *)(in_ECX + 0xa8) = *(undefined4 *)(param_1 + 0xa8);
  FUN_00454886(param_1 + 0xac);
  FUN_00454886(param_1 + 0xb8);
  FUN_00454886(param_1 + 0xc4);
  *(undefined4 *)(in_ECX + 0xd0) = *(undefined4 *)(param_1 + 0xd0);
  *(undefined4 *)(in_ECX + 0xd4) = *(undefined4 *)(param_1 + 0xd4);
  *(undefined4 *)(in_ECX + 0xd8) = *(undefined4 *)(param_1 + 0xd8);
  *(undefined4 *)(in_ECX + 0xdc) = *(undefined4 *)(param_1 + 0xdc);
  *(undefined4 *)(in_ECX + 0xe0) = *(undefined4 *)(param_1 + 0xe0);
  *(undefined4 *)(in_ECX + 0xe4) = *(undefined4 *)(param_1 + 0xe4);
  *(undefined4 *)(in_ECX + 0xe8) = *(undefined4 *)(param_1 + 0xe8);
  *(undefined4 *)(in_ECX + 0xec) = *(undefined4 *)(param_1 + 0xec);
  FUN_0045ef2d(param_1 + 0xf0);
  FUN_0047857d(param_1 + 0xfc);
  FUN_0045ef2d(param_1 + 0x108);
  FUN_00454886(param_1 + 0x114);
  FUN_0045ef2d(param_1 + 0x120);
  FUN_0045ef2d(param_1 + 300);
  return;
}



//=== ?Transfer@CHeroStatsDef@@UAEXAAVCPersistContext@@@Z @ 00479cba ===

/* [bsim sim=0.6753655655904763 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CHeroStatsDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x70),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0xfc),unaff_EDI);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  return;
}



//=== _Cons_val<std::allocator<CHeroLogBook::CQuestObjectiveDesc>,CHeroLogBook::CQuestObjectiveDesc,CHeroLogBook::CQuestObjectiveDesc> @ 0047b590 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CHeroLogBook::CQuestObjectiveDesc>,class CHeroLogBook::CQuestObjectiveDesc,class
   CHeroLogBook::CQuestObjectiveDesc>(class std::allocator<class CHeroLogBook::CQuestObjectiveDesc>
   &,class CHeroLogBook::CQuestObjectiveDesc *,class CHeroLogBook::CQuestObjectiveDesc &&) */

void __fastcall
std::
_Cons_val<std::allocator<CHeroLogBook::CQuestObjectiveDesc>,CHeroLogBook::CQuestObjectiveDesc,CHeroLogBook::CQuestObjectiveDesc>
          (allocator<CHeroLogBook::CQuestObjectiveDesc> *param_1,CQuestObjectiveDesc *param_2,
          CQuestObjectiveDesc *param_3)

{
  if (param_1 != (allocator<CHeroLogBook::CQuestObjectiveDesc> *)0x0) {
    *(undefined4 *)param_1 = *(undefined4 *)param_2;
    CCharString::CCharString((CCharString *)(param_1 + 4),(CCharString *)(param_2 + 4));
    param_1[8] = *(allocator<CHeroLogBook::CQuestObjectiveDesc> *)(param_2 + 8);
  }
  return;
}



//=== _Move<CHeroLogBook::CQuestObjectiveDesc*,CHeroLogBook::CQuestObjectiveDesc*> @ 0047b8e0 ===

/* [bsim sim=0.6975791281466294 <- ego_r]
   class CHeroLogBook::CQuestObjectiveDesc * __fastcall std::_Move<class
   CHeroLogBook::CQuestObjectiveDesc *,class CHeroLogBook::CQuestObjectiveDesc *>(class
   CHeroLogBook::CQuestObjectiveDesc *,class CHeroLogBook::CQuestObjectiveDesc *,class
   CHeroLogBook::CQuestObjectiveDesc *,struct std::_Nonscalar_ptr_iterator_tag) */

CQuestObjectiveDesc * __fastcall
std::_Move<CHeroLogBook::CQuestObjectiveDesc*,CHeroLogBook::CQuestObjectiveDesc*>
          (undefined4 *param_1,int param_2,CQuestObjectiveDesc *param_3)

{
  int iVar1;
  
  iVar1 = (param_2 - (int)param_1) / 0xc;
  if (iVar1 < 1) {
    return param_3;
  }
  do {
    *(undefined4 *)param_3 = *param_1;
    CCharString::operator=((CCharString *)(param_3 + 4),(CCharString *)(param_1 + 1));
    param_3[8] = *(CQuestObjectiveDesc *)(param_1 + 2);
    param_1 = param_1 + 3;
    param_3 = param_3 + 0xc;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  return param_3;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 0048521a ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x1c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 7; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*> @ 00485372 ===

/* [bsim sim=0.7435733930203144 <- ego_r]
   class CHeroMarriageInfo * __fastcall std::_Copy_backward<class CHeroMarriageInfo *,class
   CHeroMarriageInfo *>(class CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo
   *,struct std::_Nonscalar_ptr_iterator_tag) */

CHeroMarriageInfo * __fastcall
std::_Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*>
          (int param_1,undefined4 *param_2,CHeroMarriageInfo *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  CHeroMarriageInfo *pCVar4;
  
  iVar1 = ((int)param_2 - param_1) / 0x1c;
  if (0 < iVar1) {
    do {
      param_3 = param_3 + -0x1c;
      param_2 = param_2 + -7;
      iVar1 = iVar1 + -1;
      puVar3 = param_2;
      pCVar4 = param_3;
      for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
        *(undefined4 *)pCVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        pCVar4 = pCVar4 + 4;
      }
    } while (iVar1 != 0);
  }
  return param_3;
}



//=== ?MsgOnMoralityChanged@CScriptThing@@UBE_NAAJ@Z @ 004aaf40 ===

uint _MsgOnMoralityChanged_CScriptThing__UBE_NAAJ_Z(void)

{
  uint in_EAX;
  uint uVar1;
  int in_ECX;
  
  if (*(int **)(in_ECX + 4) == (int *)0x0) {
    return in_EAX & 0xffffff00;
  }
                    /* WARNING: Could not recover jumptable at 0x004aaf4e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(**(int **)(in_ECX + 4) + 0xe8))();
  return uVar1;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 004ab990 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetTCDriver<CTCDExperienceOrb> @ 004acb50 ===

/* [bsim sim=0.8897310149776794 <- ego_r]
   public: bool __thiscall CThing::GetTCDriver<class CTCDExperienceOrb>(class CTCDExperienceOrb * *)
    */

bool __thiscall CThing::GetTCDriver<CTCDExperienceOrb>(CThing *this,CTCDExperienceOrb **param_1)

{
  if ((*(uint *)(this + 0x28) & 0x800) != 0) {
    *param_1 = *(CTCDExperienceOrb **)(this + 0x5c);
    return true;
  }
  return false;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 004d0460 ===

/* [bsim sim=0.5526780557404039 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  int iVar1;
  CHeroMarriageInfo *pCVar2;
  CHeroMarriageInfo *pCVar3;
  float *pfVar4;
  CHeroMarriageInfo *pCVar5;
  CHeroMarriageInfo *pCVar6;
  float local_1c [4];
  float local_c;
  float local_8;
  float local_4;
  
  while (param_1 != param_2) {
    local_1c[0] = *(float *)param_1;
    local_1c[1] = *(float *)(param_1 + 4);
    local_1c[2] = *(float *)(param_1 + 8);
    local_1c[3] = *(float *)(param_1 + 0xc);
    local_c = *(float *)(param_1 + 0x10);
    local_8 = *(float *)(param_1 + 0x14);
    local_4 = *(float *)(param_1 + 0x18);
    pCVar6 = param_1;
    while( true ) {
      pCVar2 = pCVar6 + -0x1c;
      iVar1 = 7;
      pCVar3 = pCVar2;
      pCVar5 = pCVar6;
      if (*(float *)pCVar2 <= local_1c[0]) break;
      for (; pCVar6 = pCVar2, iVar1 != 0; iVar1 = iVar1 + -1) {
        *(float *)pCVar5 = *(float *)pCVar3;
        pCVar3 = pCVar3 + 4;
        pCVar5 = pCVar5 + 4;
      }
    }
    param_1 = param_1 + 0x1c;
    pfVar4 = local_1c;
    for (; iVar1 != 0; iVar1 = iVar1 + -1) {
      *(float *)pCVar6 = *pfVar4;
      pfVar4 = pfVar4 + 1;
      pCVar6 = pCVar6 + 4;
    }
  }
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 004d0910 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== ?GetSizeofClass@CHeroDef@@UAEKXZ @ 004d3464 ===

undefined4 _GetSizeofClass_CHeroDef__UAEKXZ(void)

{
  return 0x30;
}



//=== ?GetSizeofClass@CHeroCentreDef@@UAEKXZ @ 004d463c ===

undefined4 _GetSizeofClass_CHeroCentreDef__UAEKXZ(void)

{
  return 0x25;
}



//=== ?GetSizeofClass@CHeroSpecialMovementDef@@UAEKXZ @ 004d5d95 ===

undefined4 _GetSizeofClass_CHeroSpecialMovementDef__UAEKXZ(void)

{
  return 0x38;
}



//=== ?Transfer@CHeroDef@@UAEXAAVCPersistContext@@@Z @ 004ddeaa ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CHeroDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== ?Transfer@CHeroSpecialMovementDef@@UAEXAAVCPersistContext@@@Z @ 004def3d ===

/* [bsim sim=0.7521279987471763 <- ego_r]
   public: void __thiscall std::deque<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >::insert<class std::_Deque_const_iterator<class NUISystem::CComponent
   *,class std::allocator<class NUISystem::CComponent *> > >(class std::_Deque_const_iterator<class
   NUISystem::CComponent *,class std::allocator<class NUISystem::CComponent *> >,class
   std::_Deque_const_iterator<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >,class std::_Deque_const_iterator<class NUISystem::CComponent *,class
   std::allocator<class NUISystem::CComponent *> >) */

void __thiscall
std::deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>::
_Transfer_CHeroSpecialMovementDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x34));
  return;
}



//=== ?Copy@CHeroDef@@UAEXPBVCDefClassBase@@@Z @ 004e0331 ===

void _Copy_CHeroDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x28) = *(undefined4 *)(param_1 + 0x28);
  *(undefined4 *)(in_ECX + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
  return;
}



//=== ?GetSizeofClass@CExperienceDef@@UAEKXZ @ 004e088a ===

undefined4 _GetSizeofClass_CExperienceDef__UAEKXZ(void)

{
  return 0x50;
}



//=== ?GetSizeofClass@CHeroSuitDef@@UAEKXZ @ 004e091a ===

undefined4 _GetSizeofClass_CHeroSuitDef__UAEKXZ(void)

{
  return 0x34;
}



//=== ?Copy@CHeroCentreDef@@UAEXPBVCDefClassBase@@@Z @ 004e0a00 ===

CThingBuildingDef * __thiscall
CThingBuildingDef::_Copy_CHeroCentreDef__UAEXPBVCDefClassBase___Z
          (CThingBuildingDef *this,CThingBuildingDef *param_1)

{
  CThingBuildingDef *pCVar1;
  
  pCVar1 = operator=(this,param_1);
  return pCVar1;
}



//=== ?Copy@CHeroSpecialMovementDef@@UAEXPBVCDefClassBase@@@Z @ 004e1362 ===

void _Copy_CHeroSpecialMovementDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x28) = *(undefined4 *)(param_1 + 0x28);
  *(undefined4 *)(in_ECX + 0x2c) = *(undefined4 *)(param_1 + 0x2c);
  *(undefined4 *)(in_ECX + 0x30) = *(undefined4 *)(param_1 + 0x30);
  *(undefined4 *)(in_ECX + 0x34) = *(undefined4 *)(param_1 + 0x34);
  return;
}



//=== ~CHeroMarriageDef @ 004e26f1 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual __thiscall CHeroMarriageDef::~CHeroMarriageDef(void) */

void __thiscall CHeroMarriageDef::~CHeroMarriageDef(CHeroMarriageDef *this)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x74));
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 100));
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x4c));
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x40));
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x3c),
             extraout_EDX,unaff_ESI);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01230ba0;
  CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)this);
  return;
}



//=== CTCHeroRangedAttacks @ 004e30e7 ===

/* [bsim sim=0.8409775777921598 <- ego_r]
   public: __thiscall CTCHeroRangedAttacks::CTCHeroRangedAttacks(class CThing &) */

CTCHeroRangedAttacks * __thiscall
CTCHeroRangedAttacks::CTCHeroRangedAttacks(CTCHeroRangedAttacks *this,CThing *param_1)

{
  CTCDriverBase::CTCDriverBase((CTCDriverBase *)this,param_1);
  *(undefined4 *)(this + 0x1c) = 0xffffffff;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined ***)this = &PTR__vector_deleting_destructor__01242a54;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3c] = (CTCHeroRangedAttacks)0x0;
  this[0x3d] = (CTCHeroRangedAttacks)0x1;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x50) = 0xffffffff;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  return this;
}



//=== CHeroMorphDef @ 004e373b ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall CHeroMorphDef::CHeroMorphDef(void) */

CHeroMorphDef * __thiscall CHeroMorphDef::CHeroMorphDef(CHeroMorphDef *this)

{
  CShoveHaPennyRegionDef::CShoveHaPennyRegionDef((CShoveHaPennyRegionDef *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01242bcc;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  this[0x35] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  this[0x45] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  this[0x55] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  this[0x65] = (CHeroMorphDef)0x0;
  return this;
}



//=== ?GetSizeofClass@CHeroMorphDef@@UAEKXZ @ 004e377f ===

undefined4 _GetSizeofClass_CHeroMorphDef__UAEKXZ(void)

{
  return 0x68;
}



//=== ~CHeroExperienceDef @ 004e3d4a ===

/* [bsim sim=0.8669709852456278 <- ego_r]
   public: virtual __thiscall CHeroExperienceDef::~CHeroExperienceDef(void) */

void __thiscall CHeroExperienceDef::~CHeroExperienceDef(CHeroExperienceDef *this)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xe0),in_EDX,
             unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xdc),
             extraout_EDX,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xd8),
             extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0xd4),
             extraout_EDX_01,unaff_ESI);
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0xb0));
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x9c),
             extraout_EDX_02,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x98),
             extraout_EDX_03,unaff_ESI);
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x84));
  CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures
            ((CEngineScreenEffectDisplacementRenderer *)(this + 0x50));
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x34),
             extraout_EDX_04,unaff_ESI);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01230ba0;
  CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)this);
  return;
}



//=== ?Copy@CExperienceDef@@UAEXPBVCDefClassBase@@@Z @ 004e4d83 ===

void _Copy_CExperienceDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  *(undefined4 *)(in_ECX + 0x28) = *(undefined4 *)(param_1 + 0x28);
  FUN_004e4eec(param_1 + 0x2c);
  FUN_004e4dc7(param_1 + 0x38);
  FUN_004e4eec(param_1 + 0x44);
  return;
}



//=== ?Transfer@CExperienceDef@@UAEXAAVCPersistContext@@@Z @ 004e5011 ===

/* [bsim sim=0.7521279987471763 <- ego_r]
   public: void __thiscall std::deque<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >::insert<class std::_Deque_const_iterator<class NUISystem::CComponent
   *,class std::allocator<class NUISystem::CComponent *> > >(class std::_Deque_const_iterator<class
   NUISystem::CComponent *,class std::allocator<class NUISystem::CComponent *> >,class
   std::_Deque_const_iterator<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >,class std::_Deque_const_iterator<class NUISystem::CComponent *,class
   std::allocator<class NUISystem::CComponent *> >) */

void __thiscall
std::deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>::
_Transfer_CExperienceDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x38));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  return;
}



//=== ?Copy@CHeroSuitDef@@UAEXPBVCDefClassBase@@@Z @ 004e54ad ===

void _Copy_CHeroSuitDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  FUN_004e54cc(param_1 + 0x28);
  return;
}



//=== ?Transfer@CHeroSuitDef@@UAEXAAVCPersistContext@@@Z @ 004e55f1 ===

void _Transfer_CHeroSuitDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Copy@CHeroMorphDef@@UAEXPBVCDefClassBase@@@Z @ 004e6f06 ===

void _Copy_CHeroMorphDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  FUN_004e6f74(param_1 + 0x28);
  in_ECX[0x35] = param_1[0x35];
  FUN_004e72c8(param_1 + 0x38);
  in_ECX[0x45] = param_1[0x45];
  FUN_004e711e(param_1 + 0x48);
  in_ECX[0x55] = param_1[0x55];
  FUN_004e711e(param_1 + 0x58);
  in_ECX[0x65] = param_1[0x65];
  return;
}



//=== ?Transfer@CHeroMorphDef@@UAEXAAVCPersistContext@@@Z @ 004e77e9 ===

/* [bsim sim=0.7521279987471763 <- ego_r]
   public: void __thiscall std::deque<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >::insert<class std::_Deque_const_iterator<class NUISystem::CComponent
   *,class std::allocator<class NUISystem::CComponent *> > >(class std::_Deque_const_iterator<class
   NUISystem::CComponent *,class std::allocator<class NUISystem::CComponent *> >,class
   std::_Deque_const_iterator<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> >,class std::_Deque_const_iterator<class NUISystem::CComponent *,class
   std::allocator<class NUISystem::CComponent *> >) */

void __thiscall
std::deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_>::
_Transfer_CHeroMorphDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this)

{
  FUN_004e7822(this + 0x28);
  FUN_004e788a(this + 0x38);
  FUN_004e78f2(this + 0x48);
  FUN_004e78f2(this + 0x58);
  return;
}



//=== CTCHero @ 004e7cc3 ===

/* [bsim sim=0.7602197964921581 <- ego_r]
   public: __thiscall CTCHero::CTCHero(class CThing &) */

CTCHero * __thiscall CTCHero::CTCHero(CTCHero *this,CThing *param_1)

{
  NInventory::CTCInventoryBase::CTCInventoryBase((CTCInventoryBase *)this,param_1);
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x130));
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124356c;
  *(undefined ***)(this + 0x130) = &PTR_LAB_01243558;
  *(undefined4 *)(this + 0x134) = 0;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x13c) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x140));
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x14c));
  *(undefined4 *)(this + 0x158) = 0;
  this[0x15c] = (CTCHero)0x0;
  this[0x15d] = (CTCHero)0x0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x160));
  *(undefined4 *)(this + 0x16c) = 0;
  return this;
}



//=== ?GetSizeofClass@CHeroExperienceDef@@UAEKXZ @ 004eba68 ===

undefined4 _GetSizeofClass_CHeroExperienceDef__UAEKXZ(void)

{
  return 0xb4;
}



//=== ?Copy@CHeroExperienceDef@@UAEXPBVCDefClassBase@@@Z @ 004f9145 ===

void _Copy_CHeroExperienceDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  FUN_00454886(param_1 + 0x28);
  FUN_00454886(param_1 + 0x34);
  FUN_00454886(param_1 + 0x40);
  FUN_004f9207(param_1 + 0x4c);
  FUN_004f9660(param_1 + 0x58);
  *(undefined4 *)(in_ECX + 100) = *(undefined4 *)(param_1 + 100);
  *(undefined4 *)(in_ECX + 0x68) = *(undefined4 *)(param_1 + 0x68);
  FUN_004f9207(param_1 + 0x6c);
  FUN_004f9207(param_1 + 0x78);
  FUN_004f9207(param_1 + 0x84);
  FUN_004f9207(param_1 + 0x90);
  FUN_004f9831(param_1 + 0x9c);
  FUN_004f932c(param_1 + 0xa8);
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00528760 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== CTCHeroPodium @ 00555180 ===

/* [bsim sim=0.7739202776121789 <- ego_r]
   public: __thiscall CTCHeroPodium::CTCHeroPodium(class CThing &) */

CTCHeroPodium * __thiscall CTCHeroPodium::CTCHeroPodium(CTCHeroPodium *this,CThing *param_1)

{
  NUISystem::CDraggableInto::CDraggableInto((CDraggableInto *)this,(long)param_1);
  *(undefined4 *)(this + 0x16c) = 0;
  *(undefined4 *)(this + 0x170) = 0;
  *(undefined4 *)(this + 0x174) = 0;
  this[0x178] = (CTCHeroPodium)0x0;
  this[0x179] = (CTCHeroPodium)0x0;
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124a72c;
  *(undefined ***)(this + 4) = &PTR_LAB_0124a708;
  *(undefined ***)(this + 0x18) = &PTR_LAB_0124a700;
  return this;
}



//=== _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CActivity>,CTCHeroOnlineScoreboard::CActivity,CTCHeroOnlineScoreboard::CActivity> @ 00565780 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CTCHeroOnlineScoreboard::CActivity>,class CTCHeroOnlineScoreboard::CActivity,class
   CTCHeroOnlineScoreboard::CActivity>(class std::allocator<class
   CTCHeroOnlineScoreboard::CActivity> &,class CTCHeroOnlineScoreboard::CActivity *,class
   CTCHeroOnlineScoreboard::CActivity &&) */

void __fastcall
std::
_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CActivity>,CTCHeroOnlineScoreboard::CActivity,CTCHeroOnlineScoreboard::CActivity>
          (allocator<CTCHeroOnlineScoreboard::CActivity> *param_1,CActivity *param_2,
          CActivity *param_3)

{
  if (param_1 != (allocator<CTCHeroOnlineScoreboard::CActivity> *)0x0) {
    CCharString::CCharString((CCharString *)param_1,(CCharString *)param_2);
    CCharString::CCharString((CCharString *)(param_1 + 4),(CCharString *)(param_2 + 4));
    CCharString::CCharString((CCharString *)(param_1 + 8),(CCharString *)(param_2 + 8));
    param_1[0xc] = *(allocator<CTCHeroOnlineScoreboard::CActivity> *)(param_2 + 0xc);
    param_1[0xd] = *(allocator<CTCHeroOnlineScoreboard::CActivity> *)(param_2 + 0xd);
  }
  return;
}



//=== _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanInfo>,CTCHeroOnlineScoreboard::CClanInfo,CTCHeroOnlineScoreboard::CClanInfo_const&> @ 00566180 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CTCHeroOnlineScoreboard::CClanInfo>,class CTCHeroOnlineScoreboard::CClanInfo,class
   CTCHeroOnlineScoreboard::CClanInfo const &>(class std::allocator<class
   CTCHeroOnlineScoreboard::CClanInfo> &,class CTCHeroOnlineScoreboard::CClanInfo *,class
   CTCHeroOnlineScoreboard::CClanInfo const &) */

void __fastcall
std::
_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanInfo>,CTCHeroOnlineScoreboard::CClanInfo,CTCHeroOnlineScoreboard::CClanInfo_const&>
          (allocator<CTCHeroOnlineScoreboard::CClanInfo> *param_1,CClanInfo *param_2,
          CClanInfo *param_3)

{
  if (param_1 != (allocator<CTCHeroOnlineScoreboard::CClanInfo> *)0x0) {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)param_2);
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_2 + 4);
    CWideString::CWideString((CWideString *)(param_1 + 8),(CWideString *)(param_2 + 8));
    CCharString::CCharString((CCharString *)(param_1 + 0xc),(CCharString *)(param_2 + 0xc));
    *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_2 + 0x10);
  }
  return;
}



//=== _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&> @ 005661c0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember>,class CTCHeroOnlineScoreboard::CClanMember,class
   CTCHeroOnlineScoreboard::CClanMember const &>(class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember> &,class CTCHeroOnlineScoreboard::CClanMember *,class
   CTCHeroOnlineScoreboard::CClanMember const &) */

void __fastcall
std::
_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>
          (allocator<CTCHeroOnlineScoreboard::CClanMember> *param_1,CClanMember *param_2,
          CClanMember *param_3)

{
  if (param_1 != (allocator<CTCHeroOnlineScoreboard::CClanMember> *)0x0) {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)param_2);
    *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_2 + 4);
    CWideString::CWideString((CWideString *)(param_1 + 8),(CWideString *)(param_2 + 8));
  }
  return;
}



//=== GetExperienceStatMovieName @ 0056e6b6 ===

/* [bsim sim=0.8514881179716965 <- ego_r]
   public: class CCharString __thiscall CTCInventoryExperience::GetExperienceStatMovieName(enum
   EHeroTrainableStatType) */

EHeroTrainableStatType __thiscall
CTCInventoryExperience::GetExperienceStatMovieName
          (CTCInventoryExperience *this,EHeroTrainableStatType param_1)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  int in_stack_00000008;
  
  CCharString::CCharString((CCharString *)&stack0xfffffff8);
  if (in_stack_00000008 == 1) {
    CCharString::operator=((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_health.wmv");
  }
  if (in_stack_00000008 == 0) {
    CCharString::operator=((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_strength.wmv");
  }
  if (in_stack_00000008 == 2) {
    CCharString::operator=((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_tough.wmv");
  }
  if (in_stack_00000008 == 3) {
    CCharString::operator=((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_speed.wmv");
  }
  if (in_stack_00000008 == 5) {
    CCharString::operator=((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_stealth.wmv");
  }
  if (in_stack_00000008 == 4) {
    CCharString::operator=
              ((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_decapitation.wmv");
  }
  if (in_stack_00000008 == 9) {
    CCharString::operator=((CCharString *)&stack0xfffffff8,"Data\\Video\\exp_spend_will.wmv");
  }
  CCharString::CCharString((CCharString *)param_1,(CCharString *)&stack0xfffffff8);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff8,
             extraout_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)this);
  return param_1;
}



//=== GetExperienceAbilityMovieName @ 0056e75f ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: class CWideString __thiscall CTCInventoryExperience::GetExperienceAbilityMovieName(enum
   EHeroAbility) */

EHeroAbility __thiscall
CTCInventoryExperience::GetExperienceAbilityMovieName
          (CTCInventoryExperience *this,EHeroAbility param_1)

{
  int in_stack_00000008;
  wchar_t *pwVar1;
  
  if (in_stack_00000008 == 0xd) {
    pwVar1 = L"Data\\Video\\gui_spell_Assassin_Rush.wmv";
  }
  else if (in_stack_00000008 == 0xc) {
    pwVar1 = L"Data\\Video\\gui_spell_Battle_Charge.wmv";
  }
  else if (in_stack_00000008 == 8) {
    pwVar1 = L"Data\\Video\\gui_spell_Beserk.wmv";
  }
  else if (in_stack_00000008 == 6) {
    pwVar1 = L"Data\\Video\\gui_spell_drain.wmv";
  }
  else if (in_stack_00000008 == 3) {
    pwVar1 = L"Data\\Video\\gui_spell_enflame.wmv";
  }
  else if (in_stack_00000008 == 0x10) {
    pwVar1 = L"Data\\Video\\gui_spell_fireball.wmv";
  }
  else if (in_stack_00000008 == 1) {
    pwVar1 = L"Data\\Video\\gui_spell_force_push.wmv";
  }
  else if (in_stack_00000008 == 0xf) {
    pwVar1 = L"Data\\Video\\gui_spell_ghostly_sword.wmv";
  }
  else if (in_stack_00000008 == 0xe) {
    pwVar1 = L"Data\\Video\\gui_spell_heal.wmv";
  }
  else if (in_stack_00000008 == 0xb) {
    pwVar1 = L"Data\\Video\\gui_spell_lightning.wmv";
  }
  else if (in_stack_00000008 == 0x11) {
    pwVar1 = L"Data\\Video\\gui_spell_Multi_Arrow.wmv";
  }
  else if (in_stack_00000008 == 9) {
    pwVar1 = L"Data\\Video\\gui_spell_MultiStrike.wmv";
  }
  else if (in_stack_00000008 == 4) {
    pwVar1 = L"Data\\Video\\gui_spell_Shield.wmv";
  }
  else if (in_stack_00000008 == 2) {
    pwVar1 = L"Data\\Video\\gui_spell_slow_time.wmv";
  }
  else if (in_stack_00000008 == 10) {
    pwVar1 = L"Data\\Video\\gui_Spell_Summon.wmv";
  }
  else if (in_stack_00000008 == 5) {
    pwVar1 = L"Data\\Video\\gui_spell_TurnCoat.wmv";
  }
  else if (in_stack_00000008 == 0x12) {
    pwVar1 = L"Data\\Video\\gui_spell_divine_wrath.wmv";
  }
  else if (in_stack_00000008 == 0x13) {
    pwVar1 = L"Data\\Video\\gui_spell_unholy_power.wmv";
  }
  else {
    pwVar1 = L"Data\\Video\\lionhead_logo.wmv";
  }
  FUN_0099b6b0(pwVar1);
  return param_1;
}



//=== GetOriginalExperience @ 0056ea9a ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: long __thiscall CTCInventoryExperience::GetOriginalExperience(long) */

long __thiscall
CTCInventoryExperience::GetOriginalExperience(CTCInventoryExperience *this,long param_1)

{
  long lVar1;
  
  if (param_1 == 0) {
    lVar1 = *(long *)(this + 0x1ac);
  }
  else if (param_1 == 1) {
    lVar1 = *(long *)(this + 0x1b0);
  }
  else if (param_1 == 2) {
    lVar1 = *(long *)(this + 0x1b4);
  }
  else if (param_1 == 3) {
    lVar1 = *(long *)(this + 0x1b8);
  }
  else {
    lVar1 = 0;
  }
  return lVar1;
}



//=== GetExperienceStatCategoryName @ 0056ed08 ===

/* [bsim sim=1.0 <- ego_r]
   public: class CWideString __thiscall CTCInventoryExperience::GetExperienceStatCategoryName(enum
   EHeroExperienceStatCategory) */

EHeroExperienceStatCategory __thiscall
CTCInventoryExperience::GetExperienceStatCategoryName
          (CTCInventoryExperience *this,EHeroExperienceStatCategory param_1)

{
  CDataBank *this_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  int in_stack_00000008;
  char *pcVar1;
  
  this_00 = *(CDataBank **)(DAT_013b86a0 + 0x14);
  if (in_stack_00000008 == 1) {
    pcVar1 = "TEXT_GUI_MENU_SKILL";
  }
  else if (in_stack_00000008 == 0) {
    pcVar1 = "TEXT_GUI_MENU_STRENGTH";
  }
  else {
    if (in_stack_00000008 != 2) {
      FUN_0099b6b0(&DAT_0122d70c);
      return param_1;
    }
    pcVar1 = "TEXT_GUI_MENU_WILL";
  }
  CCharString::CCharString((CCharString *)&stack0x00000008,pcVar1,-1);
  NGameText::CDataBank::GetTextBySymbol(this_00,(CCharString *)param_1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000008,
             extraout_EDX,unaff_ESI);
  return param_1;
}



//=== GetExperienceAbilityColour @ 0056edd2 ===

/* [bsim sim=0.8943504739925233 <- ego_r]
   public: class CRGBFloatColour __thiscall CTCInventoryExperience::GetExperienceAbilityColour(enum
   EHeroAbility) */

void __thiscall
CTCInventoryExperience::GetExperienceAbilityColour
          (CTCInventoryExperience *this,EHeroAbility param_1)

{
  undefined4 uVar1;
  int iVar2;
  int in_stack_00000008;
  int local_4c [18];
  
  local_4c[0] = 3;
  local_4c[1] = 0x10;
  local_4c[2] = 0xc;
  local_4c[3] = 9;
  local_4c[4] = 0xb;
  local_4c[5] = 0x12;
  local_4c[6] = 0x13;
  local_4c[0xd] = 1;
  local_4c[0xe] = 5;
  local_4c[0xf] = 2;
  local_4c[0x10] = 6;
  local_4c[0x11] = 10;
  local_4c[7] = 4;
  local_4c[8] = 8;
  local_4c[9] = 0xd;
  local_4c[10] = 0xe;
  local_4c[0xb] = 0xf;
  local_4c[0xc] = 0x11;
  iVar2 = 0;
  do {
    if (local_4c[iVar2] == in_stack_00000008) {
      uVar1 = 0x3f000000;
      goto LAB_0056ee82;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 != 5);
  iVar2 = 0;
  do {
    if (local_4c[iVar2 + 0xd] == in_stack_00000008) {
      *(undefined4 *)(param_1 + 8) = 0x3f333333;
      *(undefined4 *)param_1 = 0x3dcccccd;
      *(undefined4 *)(param_1 + 4) = 0x3dcccccd;
      goto LAB_0056ee91;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 != 5);
  iVar2 = 0;
  do {
    if (local_4c[iVar2 + 7] == in_stack_00000008) {
      *(undefined4 *)param_1 = 0x3f000000;
      uVar1 = 0x3f000000;
      goto LAB_0056ee89;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 != 6);
  uVar1 = 0;
LAB_0056ee82:
  *(undefined4 *)param_1 = uVar1;
  uVar1 = 0;
LAB_0056ee89:
  *(undefined4 *)(param_1 + 4) = uVar1;
  *(undefined4 *)(param_1 + 8) = 0x3f800000;
LAB_0056ee91:
  *(undefined4 *)(param_1 + 0xc) = 0x3f800000;
  return;
}



//=== UpdateExperienceSpendPool @ 0056ef6f ===

/* [bsim sim=0.9967804172623785 <- ego_r]
   public: void __thiscall CTCInventoryExperience::UpdateExperienceSpendPool(enum
   EHeroExperienceStatCategory,long) */

void __thiscall
CTCInventoryExperience::UpdateExperienceSpendPool
          (CTCInventoryExperience *this,EHeroExperienceStatCategory param_1,long param_2)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  if (param_2 < 1) {
    iVar3 = (param_2 ^ param_2 >> 0x1f) - (param_2 >> 0x1f);
    iVar2 = *(int *)(this + 0x1e0);
    if (*(int *)(iVar2 + 0x28) < *(int *)(iVar2 + 0x24) + iVar3) {
      iVar2 = *(int *)(this + 0x1e0);
      if (*(int *)(iVar2 + 0x24) < *(int *)(iVar2 + 0x28) + iVar3) {
        piVar1 = (int *)(iVar2 + param_1 * 0xc);
        *piVar1 = *piVar1 + (*(int *)(iVar2 + 0x24) - *(int *)(iVar2 + 0x28)) + iVar3;
        *(undefined4 *)(*(int *)(this + 0x1e0) + 0x24) =
             *(undefined4 *)(*(int *)(this + 0x1e0) + 0x28);
      }
      else if (*(int *)(iVar2 + 0x24) == *(int *)(iVar2 + 0x28)) {
        piVar1 = (int *)(*(int *)(this + 0x1e0) + param_1 * 0xc);
        *piVar1 = *piVar1 + iVar3;
      }
    }
    else {
      *(int *)(iVar2 + 0x24) = *(int *)(iVar2 + 0x24) + iVar3;
    }
  }
  else {
    iVar2 = *(int *)(this + 0x1e0);
    iVar3 = param_1 * 0xc;
    piVar1 = (int *)(iVar3 + iVar2);
    if (*piVar1 < param_2) {
      if ((*piVar1 == 0) && (param_2 <= *(int *)(iVar2 + 0x24))) {
        *(int *)(iVar2 + 0x24) = *(int *)(iVar2 + 0x24) - param_2;
      }
      else {
        iVar2 = *(int *)(this + 0x1e0);
        if (param_2 - *(int *)(iVar2 + iVar3) < *(int *)(iVar2 + 0x24)) {
          *(int *)(iVar2 + 0x24) = *(int *)(iVar2 + 0x24) + (*(int *)(iVar2 + iVar3) - param_2);
          *(undefined4 *)(iVar3 + *(int *)(this + 0x1e0)) = 0;
        }
      }
    }
    else {
      *piVar1 = *piVar1 - param_2;
    }
  }
  return;
}



//=== ResetExperienceSpendValues @ 0056f03d ===

/* [bsim sim=0.6119537160574215 <- ego_r]
   public: void __thiscall CTCInventoryExperience::ResetExperienceSpendValues(void) */

void __thiscall CTCInventoryExperience::ResetExperienceSpendValues(CTCInventoryExperience *this)

{
  undefined4 *puVar1;
  int iVar2;
  
  iVar2 = 0;
  do {
    puVar1 = (undefined4 *)(*(int *)(this + 0x1e0) + iVar2);
    iVar2 = iVar2 + 0xc;
    *puVar1 = puVar1[1];
  } while (iVar2 < 0x30);
  iVar2 = 0;
  do {
    puVar1 = (undefined4 *)(*(int *)(this + 0x1ec) + iVar2);
    iVar2 = iVar2 + 0xc;
    *puVar1 = puVar1[1];
  } while (iVar2 < 0x78);
  iVar2 = 0xc;
  do {
    puVar1 = (undefined4 *)(*(int *)(this + 0x1f8) + iVar2);
    iVar2 = iVar2 + 0xc;
    *puVar1 = puVar1[1];
  } while (iVar2 < 0xf0);
  *(undefined4 *)(this + 0x204) = 0;
  return;
}



//=== GetExperienceStatPic @ 0056f177 ===

/* [bsim sim=0.8335154172060192 <- ego_r]
   public: class NUISystem::CComponent * __thiscall
   CTCInventoryExperience::GetExperienceStatPic(enum EHeroTrainableStatType) */

CComponent * __thiscall
CTCInventoryExperience::GetExperienceStatPic
          (CTCInventoryExperience *this,EHeroTrainableStatType param_1)

{
  int iVar1;
  EHeroTrainableStatType EVar2;
  bool bVar3;
  CFrontEndManager *this_00;
  CComponent *pCVar4;
  undefined3 extraout_var;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CQuestManager *local_10 [2];
  CCharString local_8 [4];
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar5;
  
  CCharString::CCharString(local_8);
  EVar2 = param_1;
  if (param_1 == 0) {
    CCharString::operator=(local_8,"UI_RING_PIC_PHYSIQUE");
  }
  if (EVar2 == 1) {
    CCharString::operator=(local_8,"UI_RING_PIC_HEALTH");
  }
  if (EVar2 == 2) {
    CCharString::operator=(local_8,"UI_RING_PIC_TOUGHNESS");
  }
  if (EVar2 == 3) {
    CCharString::operator=(local_8,"UI_RING_PIC_SPEED");
  }
  if (EVar2 == 4) {
    CCharString::operator=(local_8,"UI_RING_PIC_ACCURACY");
  }
  if (EVar2 == 5) {
    CCharString::operator=(local_8,"UI_RING_PIC_GUILE");
  }
  if (EVar2 == 9) {
    CCharString::operator=(local_8,"UI_RING_PIC_MAGIC_POWER");
  }
  this_00 = NUISystem::CFrontEndManager::GetInstance();
  CCharString::CCharString((CCharString *)&param_1,"UI_RING_PIC",-1);
  pCVar4 = NUISystem::CManager::CreateComponent((CManager *)this_00,(CCharString *)&param_1,false);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,extraout_EDX,
             unaff_EDI);
  ::CCountedPointer<CBankFileAsyncEntry>::CCountedPointer<CBankFileAsyncEntry>
            ((CCountedPointer<CBankFileAsyncEntry> *)local_10,
             (CCountedPointer<class_CBankFileAsyncEntry> *)(this_00 + 0x10));
  iVar1 = *(int *)pCVar4;
  bVar3 = CQuestManager::OnQuestStartScreenDisplayed(local_10[0],local_8);
  ppVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)CONCAT31(extraout_var,bVar3);
  (**(code **)(iVar1 + 0x244))();
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_10
             ,extraout_EDX_00);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,
             extraout_EDX_01,ppVar5);
  return pCVar4;
}



//=== GetExperienceAbilityName @ 0056f26f ===

/* [bsim sim=1.0 <- ego_r]
   public: class CWideString __thiscall CTCInventoryExperience::GetExperienceAbilityName(long) */

long __thiscall
CTCInventoryExperience::GetExperienceAbilityName(CTCInventoryExperience *this,long param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int in_stack_00000008;
  
  piVar2 = *(int **)(this + 0x68);
  if (piVar2 != (int *)0x0) {
    piVar2[1] = piVar2[1] + 1;
  }
  iVar3 = *(int *)(piVar2[0x157] + in_stack_00000008 * 4);
  piVar1 = piVar2 + 1;
  *piVar1 = *piVar1 + -1;
  if (*piVar1 == 0) {
    (**(code **)(*piVar2 + 4))();
  }
  if (iVar3 == 0) {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
  }
  else {
    NInventory::CTCInventoryBase::GetInventoryText((CTCInventoryBase *)this,param_1);
  }
  return param_1;
}



//=== GetExperienceAbilityName @ 0056f2bd ===

/* [bsim sim=1.0 <- ego_r]
   public: class CWideString __thiscall CTCInventoryExperience::GetExperienceAbilityName(long) */

long __thiscall
CTCInventoryExperience::GetExperienceAbilityName(CTCInventoryExperience *this,long param_1)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int in_stack_00000008;
  
  piVar2 = *(int **)(this + 0x68);
  if (piVar2 != (int *)0x0) {
    piVar2[1] = piVar2[1] + 1;
  }
  iVar3 = *(int *)(piVar2[0x15a] + in_stack_00000008 * 4);
  piVar1 = piVar2 + 1;
  *piVar1 = *piVar1 + -1;
  if (*piVar1 == 0) {
    (**(code **)(*piVar2 + 4))();
  }
  if (iVar3 == 0) {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
  }
  else {
    NInventory::CTCInventoryBase::GetInventoryText((CTCInventoryBase *)this,param_1);
  }
  return param_1;
}



//=== RevertExperiencePoints @ 0056f4ba ===

/* [bsim sim=0.878555106430534 <- ego_r]
   public: void __thiscall SExpSpendUndo::RevertExperiencePoints(class CTCHeroExperience *) */

void __thiscall
SExpSpendUndo::RevertExperiencePoints(SExpSpendUndo *this,CTCHeroExperience *param_1)

{
  list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
  *this_00;
  int iVar1;
  long lVar2;
  
  CTCHeroExperience::ForceSpendableExperienceTo
            (param_1,*(int *)(*(int *)(*(int *)this + 4) + 8) + *(int *)(param_1 + 0x14));
  this_00 = (list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
             *)(this + 4);
  iVar1 = *(int *)(*(int *)this_00 + 4);
  lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)param_1,0);
  CTCHeroExperience::ForceTrainableStatExperienceTo(param_1,0,lVar2 + *(int *)(iVar1 + 8));
  iVar1 = *(int *)(*(int *)(this + 8) + 4);
  lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)param_1,1);
  CTCHeroExperience::ForceTrainableStatExperienceTo(param_1,1,lVar2 + *(int *)(iVar1 + 8));
  iVar1 = *(int *)(*(int *)(this + 0xc) + 4);
  lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)param_1,2);
  CTCHeroExperience::ForceTrainableStatExperienceTo(param_1,2,lVar2 + *(int *)(iVar1 + 8));
  *(int *)(this + 0x14) = *(int *)(this + 0x14) - *(int *)(*(int *)(*(int *)this + 4) + 8);
  *(int *)(this + 0x18) = *(int *)(this + 0x18) - *(int *)(*(int *)(*(int *)this_00 + 4) + 8);
  *(int *)(this + 0x1c) = *(int *)(this + 0x1c) - *(int *)(*(int *)(*(int *)(this + 8) + 4) + 8);
  *(int *)(this + 0x20) = *(int *)(this + 0x20) - *(int *)(*(int *)(*(int *)(this + 0xc) + 4) + 8);
  std::list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
  ::pop_back((list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
              *)this);
  std::list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
  ::pop_back(this_00);
  std::list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
  ::pop_back((list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
              *)(this + 8));
  std::list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
  ::pop_back((list<CVsyncCallbackTimer::CPendingQuery,std::allocator<CVsyncCallbackTimer::CPendingQuery>_>
              *)(this + 0xc));
  *(int *)(this + 0x10) = *(int *)(this + 0x10) + -1;
  return;
}



//=== GetExperienceStatDescription @ 0056fdc3 ===

/* [bsim sim=0.941200941799833 <- ego_r]
   public: class CWideString __thiscall
   CTCInventoryExperience::GetExperienceStatDescription(long,long) */

long __thiscall
CTCInventoryExperience::GetExperienceStatDescription
          (CTCInventoryExperience *this,long param_1,long param_2)

{
  int *piVar1;
  char cVar2;
  int *piVar3;
  int extraout_EAX;
  long lVar4;
  int iVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar6;
  CTCHeroExperience *this_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar7;
  int in_stack_0000000c;
  CTCInventoryExperience *local_c;
  CTCInventoryExperience *local_8;
  
  iVar5 = *(int *)(this + 4);
  local_c = this;
  local_8 = this;
  cVar2 = FUN_00410de0(0x68);
  ppVar6 = extraout_EDX;
  this_00 = (CTCHeroExperience *)param_1;
  if (cVar2 != '\0') {
    local_c = (CTCInventoryExperience *)&DAT_00000068;
    uVar7 = CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
            LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                        *)(iVar5 + 0x44),(ETCInterfaceType *)&local_c);
    ppVar6 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar7 >> 0x20);
    piVar3 = (int *)uVar7;
    piVar1 = *(int **)(iVar5 + 0x48);
    if ((piVar3 == piVar1) || (0x68 < *piVar3)) {
      piVar3 = piVar1;
    }
    this_00 = (CTCHeroExperience *)piVar3[1];
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,ppVar6,
             unaff_EDI);
  iVar5 = *(int *)(extraout_EAX + 0xdc);
  lVar4 = CTCHeroExperience::GetTrainableStatMaxLevel(this_00,param_2);
  if (in_stack_0000000c == lVar4 + 1) {
    iVar5 = *(int *)(*(int *)(iVar5 + 0xfc) + 0x4c + param_2 * 0x50);
  }
  else {
    iVar5 = *(int *)(*(int *)(*(int *)(iVar5 + 0xfc) + 0x40 + param_2 * 0x50) + 0x44 +
                    in_stack_0000000c * 0x48);
  }
  if (iVar5 == 0) {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
  }
  else {
    NInventory::CTCInventoryBase::GetInventoryText((CTCInventoryBase *)local_8,param_1);
  }
  return param_1;
}



//=== GetExperienceAbilityDescription @ 0056fe7c ===

/* [bsim sim=0.6366830313565665 <- ego_r]
   public: class CWideString __thiscall
   CTCInventoryExperience::GetExperienceAbilityDescription(long,long) */

long __thiscall
CTCInventoryExperience::GetExperienceAbilityDescription
          (CTCInventoryExperience *this,long param_1,long param_2)

{
  int *piVar1;
  CTCInventoryAbilities *this_00;
  char cVar2;
  int *piVar3;
  long lVar4;
  int iVar5;
  CTCInventoryExperience *pCVar6;
  int in_stack_0000000c;
  CTCInventoryExperience *local_c;
  CTCInventoryExperience *local_8;
  
  iVar5 = *(int *)(this + 4);
  local_c = this;
  local_8 = this;
  cVar2 = FUN_00410de0(0x6f);
  if (cVar2 == '\0') {
    CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
  }
  else {
    local_c = (CTCInventoryExperience *)0x6f;
    piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar5 + 0x44),(ETCInterfaceType *)&local_c);
    piVar1 = *(int **)(iVar5 + 0x48);
    if ((piVar3 == piVar1) || (0x6f < *piVar3)) {
      piVar3 = piVar1;
    }
    this_00 = (CTCInventoryAbilities *)piVar3[1];
    CTCInventoryAbilities::GetAbilityPDef(this_00,(EHeroAbility)&local_8);
    lVar4 = CTCInventoryAbilities::GetAbilityMaximumLevel(this_00,param_2);
    if (in_stack_0000000c == lVar4 + 1) {
      iVar5 = *(int *)(local_8 + 0x6c);
    }
    else {
      iVar5 = *(int *)(*(int *)(local_8 + 0x60) + in_stack_0000000c * 4);
    }
    if (iVar5 == 0) {
      CWideString::CWideString((CWideString *)param_1,(CWideString *)&DAT_013bca24);
    }
    else {
      NInventory::CTCInventoryBase::GetInventoryText((CTCInventoryBase *)this,param_1);
    }
    if (local_8 != (CTCInventoryExperience *)0x0) {
      pCVar6 = local_8 + 4;
      *(int *)pCVar6 = *(int *)pCVar6 + -1;
      if (*(int *)pCVar6 == 0) {
        (**(code **)(*(int *)local_8 + 4))();
      }
    }
  }
  return param_1;
}



//=== GetTCHeroExperience @ 0056ff3d ===

/* [bsim sim=0.5906218523568799 <- ego_r]
   private: class CTCHeroExperience * __thiscall CTCInventoryExperience::GetTCHeroExperience(void)
    */

CTCHeroExperience * __thiscall
CTCInventoryExperience::GetTCHeroExperience(CTCInventoryExperience *this)

{
  int iVar1;
  int *piVar2;
  char cVar3;
  int *piVar4;
  CTCHeroExperience *local_8;
  
  iVar1 = *(int *)(this + 4);
  local_8 = (CTCHeroExperience *)this;
  cVar3 = FUN_00410de0(0x68);
  if (cVar3 != '\0') {
    local_8 = (CTCHeroExperience *)0x68;
    piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&local_8);
    piVar2 = *(int **)(iVar1 + 0x48);
    if ((piVar4 == piVar2) || (0x68 < *piVar4)) {
      piVar4 = piVar2;
    }
    local_8 = (CTCHeroExperience *)piVar4[1];
  }
  return local_8;
}



//=== GetTCHeroExperience @ 0056ff81 ===

/* [bsim sim=0.5906218523568799 <- ego_r]
   private: class CTCHeroExperience * __thiscall CTCInventoryExperience::GetTCHeroExperience(void)
    */

CTCHeroExperience * __thiscall
CTCInventoryExperience::GetTCHeroExperience(CTCInventoryExperience *this)

{
  int iVar1;
  int *piVar2;
  char cVar3;
  int *piVar4;
  CTCHeroExperience *local_8;
  
  iVar1 = *(int *)(this + 4);
  local_8 = (CTCHeroExperience *)this;
  cVar3 = FUN_00410de0(4);
  if (cVar3 != '\0') {
    local_8 = (CTCHeroExperience *)0x4;
    piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&local_8);
    piVar2 = *(int **)(iVar1 + 0x48);
    if ((piVar4 == piVar2) || (4 < *piVar4)) {
      piVar4 = piVar2;
    }
    local_8 = (CTCHeroExperience *)piVar4[1];
  }
  return local_8;
}



//=== GetTCHeroExperience @ 0056ffc5 ===

/* [bsim sim=0.5906218523568799 <- ego_r]
   private: class CTCHeroExperience * __thiscall CTCInventoryExperience::GetTCHeroExperience(void)
    */

CTCHeroExperience * __thiscall
CTCInventoryExperience::GetTCHeroExperience(CTCInventoryExperience *this)

{
  int iVar1;
  int *piVar2;
  char cVar3;
  int *piVar4;
  CTCHeroExperience *local_8;
  
  iVar1 = *(int *)(this + 4);
  local_8 = (CTCHeroExperience *)this;
  cVar3 = FUN_00410de0(0x6f);
  if (cVar3 != '\0') {
    local_8 = (CTCHeroExperience *)0x6f;
    piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&local_8);
    piVar2 = *(int **)(iVar1 + 0x48);
    if ((piVar4 == piVar2) || (0x6f < *piVar4)) {
      piVar4 = piVar2;
    }
    local_8 = (CTCHeroExperience *)piVar4[1];
  }
  return local_8;
}



//=== SaveExperienceValuesForCountDown @ 00570161 ===

/* [bsim sim=0.5859591483951535 <- ego_r]
   public: void __thiscall CTCInventoryExperience::SaveExperienceValuesForCountDown(void) */

void __thiscall
CTCInventoryExperience::SaveExperienceValuesForCountDown(CTCInventoryExperience *this)

{
  int iVar1;
  int *piVar2;
  CTCInventoryAbilities *this_00;
  char cVar3;
  int *piVar4;
  long lVar5;
  CTCInventoryExperience *local_8;
  
  iVar1 = *(int *)(this + 4);
  local_8 = this;
  cVar3 = FUN_00410de0(0x68);
  if (cVar3 != '\0') {
    local_8 = (CTCInventoryExperience *)0x68;
    piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&local_8);
    piVar2 = *(int **)(iVar1 + 0x48);
    if ((piVar4 == piVar2) || (0x68 < *piVar4)) {
      piVar4 = piVar2;
    }
    this_00 = (CTCInventoryAbilities *)piVar4[1];
    *(undefined4 *)(this + 0x260) = *(undefined4 *)(this_00 + 0x14);
    lVar5 = CTCInventoryAbilities::GetAbilityLevel(this_00,0);
    *(long *)(this + 0x264) = lVar5;
    lVar5 = CTCInventoryAbilities::GetAbilityLevel(this_00,1);
    *(long *)(this + 0x268) = lVar5;
    lVar5 = CTCInventoryAbilities::GetAbilityLevel(this_00,2);
    *(long *)(this + 0x26c) = lVar5;
  }
  return;
}



//=== UpdateExperienceSpendLevelUp @ 005721fa ===

/* [bsim sim=0.6852253445870216 <- ego_r]
   public: void __thiscall CTCInventoryExperience::UpdateExperienceSpendLevelUp(void) */

void __thiscall CTCInventoryExperience::UpdateExperienceSpendLevelUp(CTCInventoryExperience *this)

{
  int iVar1;
  CTCHeroExperience *pCVar2;
  long lVar3;
  undefined4 *puVar4;
  int iVar5;
  EHeroTrainableStatType EVar6;
  bool bVar7;
  EHeroTrainableStatType local_8;
  
  iVar5 = 0;
  local_8 = 0;
  do {
    iVar1 = *(int *)(this + 0x1ec);
    EVar6 = local_8;
    pCVar2 = GetTCHeroExperience(this);
    lVar3 = CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar2,EVar6);
    *(long *)(iVar5 + 8 + iVar1) = lVar3;
    puVar4 = (undefined4 *)(*(int *)(this + 0x1ec) + iVar5);
    local_8 = local_8 + 1;
    iVar5 = iVar5 + 0xc;
    puVar4[1] = *puVar4;
  } while (iVar5 < 0x78);
  local_8 = 1;
  iVar5 = 0xc;
  do {
    iVar1 = *(int *)(this + 0x1f8);
    bVar7 = true;
    lVar3 = 1;
    EVar6 = local_8;
    pCVar2 = GetTCHeroExperience(this);
    lVar3 = CTCInventoryAbilities::GetExperienceCostToUpgradeAbility
                      ((CTCInventoryAbilities *)pCVar2,EVar6,lVar3,bVar7);
    *(long *)(iVar5 + 8 + iVar1) = lVar3;
    puVar4 = (undefined4 *)(*(int *)(this + 0x1f8) + iVar5);
    local_8 = local_8 + 1;
    iVar5 = iVar5 + 0xc;
    puVar4[1] = *puVar4;
  } while (iVar5 < 0xf0);
  iVar5 = 0;
  do {
    puVar4 = (undefined4 *)(*(int *)(this + 0x1e0) + iVar5);
    iVar5 = iVar5 + 0xc;
    puVar4[1] = *puVar4;
  } while (iVar5 < 0x30);
  return;
}



//=== UpdateExperienceSpendStat @ 0057229b ===

/* [bsim sim=0.8597420165761533 <- ego_r]
   public: void __thiscall CTCInventoryExperience::UpdateExperienceSpendStat(void) */

void __thiscall CTCInventoryExperience::UpdateExperienceSpendStat(CTCInventoryExperience *this)

{
  int *piVar1;
  int *piVar2;
  EHeroAbility EVar3;
  CTCHeroExperience *this_00;
  EHeroExperienceStatCategory EVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  EHeroAbility EVar9;
  
  piVar2 = *(int **)(this + 0x68);
  if (piVar2 != (int *)0x0) {
    piVar2[1] = piVar2[1] + 1;
  }
  EVar3 = GetHeroTrainableAbilityType(this,*(long *)(this + 0x1bc));
  EVar9 = EVar3;
  this_00 = GetTCHeroExperience(this);
  EVar4 = CTCHeroExperience::GetCategoryTrainableStatIsIn(this_00,EVar9);
  iVar8 = piVar2[0x156] * *(int *)(this + 0x204);
  if (iVar8 < 1) {
    if (-1 < iVar8) goto LAB_0057237e;
    iVar6 = EVar3 * 0xc;
    *(int *)(*(int *)(this + 0x1ec) + iVar6) = *(int *)(*(int *)(this + 0x1ec) + iVar6) + iVar8;
    if (*(int *)(*(int *)(this + 0x1ec) + iVar6) < ((int *)(*(int *)(this + 0x1ec) + iVar6))[1]) {
      puVar5 = (undefined4 *)(*(int *)(this + 0x1ec) + iVar6);
      uVar7 = puVar5[1];
      goto LAB_0057236e;
    }
  }
  else {
    iVar6 = EVar3 * 0xc;
    if ((*(int *)(*(int *)(this + 0x1ec) + 8 + iVar6) < 1) ||
       ((*(int *)(*(int *)(this + 0x1e0) + EVar4 * 0xc) + *(int *)(*(int *)(this + 0x1e0) + 0x24)) -
        iVar8 < 0)) goto LAB_0057237e;
    *(int *)(*(int *)(this + 0x1ec) + iVar6) = *(int *)(*(int *)(this + 0x1ec) + iVar6) + iVar8;
    if (((int *)(*(int *)(this + 0x1ec) + iVar6))[2] < *(int *)(*(int *)(this + 0x1ec) + iVar6)) {
      puVar5 = (undefined4 *)(*(int *)(this + 0x1ec) + iVar6);
      uVar7 = puVar5[2];
LAB_0057236e:
      *puVar5 = uVar7;
      goto LAB_0057237e;
    }
    if (*(int *)(iVar6 + *(int *)(this + 0x1ec)) < 0) {
      *(int *)(iVar6 + *(int *)(this + 0x1ec)) = 0;
      goto LAB_0057237e;
    }
  }
  UpdateExperienceSpendPool(this,EVar4,iVar8);
LAB_0057237e:
  piVar1 = piVar2 + 1;
  *piVar1 = *piVar1 + -1;
  if (*piVar1 == 0) {
    (**(code **)(*piVar2 + 4))();
  }
  return;
}



//=== UpdateExperienceSpendAbility @ 0057238f ===

/* [bsim sim=0.9475782120519503 <- ego_r]
   public: void __thiscall CTCInventoryExperience::UpdateExperienceSpendAbility(void) */

void __thiscall CTCInventoryExperience::UpdateExperienceSpendAbility(CTCInventoryExperience *this)

{
  int *piVar1;
  int iVar2;
  bool bVar3;
  EHeroAbility EVar4;
  CTCHeroExperience *pCVar5;
  EHeroExperienceStatCategory EVar6;
  int iVar7;
  int iVar8;
  undefined4 *puVar9;
  int *piVar10;
  int iVar11;
  EHeroAbility EVar12;
  
  piVar1 = *(int **)(this + 0x68);
  if (piVar1 != (int *)0x0) {
    piVar1[1] = piVar1[1] + 1;
  }
  EVar4 = GetHeroTrainableAbilityType(this,*(long *)(this + 0x1bc));
  EVar12 = EVar4;
  pCVar5 = GetTCHeroExperience(this);
  EVar6 = CTCInventoryAbilities::GetAbilityStatType((CTCInventoryAbilities *)pCVar5,EVar12);
  iVar11 = piVar1[0x156] * *(int *)(this + 0x204);
  iVar7 = *(int *)(*(int *)(this + 0x1e0) + EVar6 * 0xc);
  iVar2 = *(int *)(*(int *)(this + 0x1e0) + 0x24);
  if (iVar11 < 1) {
    if (iVar11 < 0) {
      EVar12 = EVar4;
      pCVar5 = GetTCHeroExperience(this);
      bVar3 = CTCInventoryAbilities::IsAbilityAvailable((CTCInventoryAbilities *)pCVar5,EVar12);
      if (bVar3) {
        iVar7 = EVar4 * 0xc;
        *(int *)(*(int *)(this + 0x1f8) + iVar7) = *(int *)(*(int *)(this + 0x1f8) + iVar7) + iVar11
        ;
        if (*(int *)(*(int *)(this + 0x1f8) + iVar7) < ((int *)(*(int *)(this + 0x1f8) + iVar7))[1])
        {
          *(undefined4 *)(iVar7 + *(int *)(this + 0x1f8)) =
               ((undefined4 *)(iVar7 + *(int *)(this + 0x1f8)))[1];
        }
        else {
          UpdateExperienceSpendPool(this,EVar6,iVar11);
        }
      }
    }
  }
  else {
    iVar8 = EVar4 * 0xc;
    if (0 < *(int *)(*(int *)(this + 0x1f8) + 8 + iVar8)) {
      pCVar5 = GetTCHeroExperience(this);
      bVar3 = CTCInventoryAbilities::IsAbilityAvailable((CTCInventoryAbilities *)pCVar5,EVar4);
      if ((bVar3) && (-1 < (iVar7 + iVar2) - iVar11)) {
        *(int *)(*(int *)(this + 0x1f8) + iVar8) = *(int *)(*(int *)(this + 0x1f8) + iVar8) + iVar11
        ;
        if (((int *)(*(int *)(this + 0x1f8) + iVar8))[2] < *(int *)(*(int *)(this + 0x1f8) + iVar8))
        {
          puVar9 = (undefined4 *)(iVar8 + *(int *)(this + 0x1f8));
          *puVar9 = puVar9[2];
        }
        else {
          piVar10 = (int *)(iVar8 + *(int *)(this + 0x1f8));
          if (*piVar10 < 0) {
            *piVar10 = 0;
          }
          else {
            UpdateExperienceSpendPool(this,EVar6,iVar11);
          }
        }
      }
    }
  }
  piVar10 = piVar1 + 1;
  *piVar10 = *piVar10 + -1;
  if (*piVar10 != 0) {
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x00572459. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*piVar1 + 4))();
  return;
}



//=== InitialiseExperienceSpendValues @ 005724cd ===

/* [bsim sim=0.7994118222188783 <- ego_r]
   public: void __thiscall CTCInventoryExperience::InitialiseExperienceSpendValues(void) */

void __thiscall
CTCInventoryExperience::InitialiseExperienceSpendValues(CTCInventoryExperience *this)

{
  CTCHeroExperience *pCVar1;
  long lVar2;
  EHeroAbility EVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  bool bVar7;
  EHeroAbility EVar8;
  EHeroAbility local_8;
  
  EVar3 = 0;
  do {
    if (EVar3 == 3) {
      pCVar1 = GetTCHeroExperience(this);
      *(undefined4 *)(*(int *)(this + 0x1e0) + 0x24) = *(undefined4 *)(pCVar1 + 0x14);
      pCVar1 = GetTCHeroExperience(this);
      *(undefined4 *)(*(int *)(this + 0x1e0) + 0x28) = *(undefined4 *)(pCVar1 + 0x14);
      *(undefined4 *)(*(int *)(this + 0x1e0) + 0x2c) = 0;
    }
    else {
      iVar5 = *(int *)(this + 0x1e0);
      iVar4 = EVar3 * 0xc;
      EVar8 = EVar3;
      pCVar1 = GetTCHeroExperience(this);
      lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar1,EVar8);
      *(long *)(iVar4 + iVar5) = lVar2;
      iVar5 = *(int *)(this + 0x1e0);
      EVar8 = EVar3;
      pCVar1 = GetTCHeroExperience(this);
      lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar1,EVar8);
      *(long *)(iVar4 + 4 + iVar5) = lVar2;
      *(undefined4 *)(iVar4 + 8 + *(int *)(this + 0x1e0)) = 0;
    }
    EVar3 = EVar3 + 1;
  } while ((int)EVar3 < 4);
  iVar5 = 0;
  local_8 = 0;
  do {
    iVar4 = *(int *)(this + 0x1ec);
    EVar3 = local_8;
    pCVar1 = GetTCHeroExperience(this);
    lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar1,EVar3);
    *(long *)(iVar5 + iVar4) = lVar2;
    iVar4 = *(int *)(this + 0x1ec);
    EVar3 = local_8;
    pCVar1 = GetTCHeroExperience(this);
    lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar1,EVar3);
    *(long *)(iVar5 + 4 + iVar4) = lVar2;
    iVar4 = *(int *)(this + 0x1ec);
    EVar3 = local_8;
    pCVar1 = GetTCHeroExperience(this);
    lVar2 = CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar1,EVar3);
    local_8 = local_8 + 1;
    *(long *)(iVar5 + 8 + iVar4) = lVar2;
    iVar5 = iVar5 + 0xc;
  } while (iVar5 < 0x78);
  local_8 = 1;
  iVar5 = 0xc;
  do {
    iVar4 = *(int *)(this + 0x1f8);
    EVar3 = local_8;
    pCVar1 = GetTCHeroExperience(this);
    lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar1,EVar3);
    *(long *)(iVar5 + iVar4) = lVar2;
    iVar4 = *(int *)(this + 0x1f8);
    EVar3 = local_8;
    pCVar1 = GetTCHeroExperience(this);
    lVar2 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar1,EVar3);
    bVar7 = true;
    lVar6 = 1;
    *(long *)(iVar5 + 4 + iVar4) = lVar2;
    iVar4 = *(int *)(this + 0x1f8);
    EVar3 = local_8;
    pCVar1 = GetTCHeroExperience(this);
    lVar2 = CTCInventoryAbilities::GetExperienceCostToUpgradeAbility
                      ((CTCInventoryAbilities *)pCVar1,EVar3,lVar6,bVar7);
    local_8 = local_8 + 1;
    *(long *)(iVar5 + 8 + iVar4) = lVar2;
    iVar5 = iVar5 + 0xc;
  } while (iVar5 < 0xf0);
  return;
}



//=== ProcessExperienceAction @ 00572636 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCInventoryExperience::ProcessExperienceAction(void) */

void __thiscall CTCInventoryExperience::ProcessExperienceAction(CTCInventoryExperience *this)

{
  CCategory *this_00;
  EInventoryCategoryIdentifier EVar1;
  
  this_00 = NInventory::CTCInventoryBase::GetSelectedCategory((CTCInventoryBase *)this);
  if ((this_00 == (CCategory *)0x0) ||
     (EVar1 = NInventory::CCategory::GetCategoryIdentifier(this_00), (int)EVar1 < 1)) {
    return;
  }
  if (2 < (int)EVar1) {
    if (5 < (int)EVar1) {
      return;
    }
    if (*(int *)(this + 0x1bc) != 0) {
      UpdateExperienceSpendAbility(this);
      return;
    }
  }
  UpdateExperienceSpendStat(this);
  return;
}



//=== SpendExperienceOnStat @ 005746f7 ===

/* [bsim sim=0.8534432268922765 <- ego_r]
   public: void __thiscall CTCInventoryExperience::SpendExperienceOnStat(enum
   EHeroTrainableStatType) */

void __thiscall
CTCInventoryExperience::SpendExperienceOnStat
          (CTCInventoryExperience *this,EHeroTrainableStatType param_1)

{
  int iVar1;
  int iVar2;
  CTCHeroExperience *pCVar3;
  long lVar4;
  int iVar5;
  EHeroTrainableStatType EVar6;
  int iVar7;
  
  EVar6 = param_1;
  pCVar3 = GetTCHeroExperience(this);
  lVar4 = CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar3,EVar6);
  iVar5 = param_1 * 0xc;
  iVar1 = *(int *)(*(int *)(this + 0x1ec) + iVar5);
  iVar7 = ((int *)(*(int *)(this + 0x1ec) + iVar5))[1];
  if ((0 < lVar4) && (iVar1 == lVar4)) {
    EVar6 = param_1;
    pCVar3 = GetTCHeroExperience(this);
    CTCHeroExperience::UpgradeTrainableStat(pCVar3,EVar6);
    *(undefined4 *)(iVar5 + 4 + *(int *)(this + 0x1ec)) = 0;
    *(undefined4 *)(iVar5 + *(int *)(this + 0x1ec)) = 0;
    iVar2 = *(int *)(this + 0x1ec);
    EVar6 = param_1;
    pCVar3 = GetTCHeroExperience(this);
    lVar4 = CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar3,EVar6);
    *(long *)(iVar5 + 8 + iVar2) = lVar4;
  }
  if ((0 < iVar1) && (iVar7 < iVar1)) {
    EVar6 = param_1;
    iVar7 = iVar1;
    pCVar3 = GetTCHeroExperience(this);
    CTCHeroExperience::SpendExperience(pCVar3,EVar6,iVar7);
  }
  CTCHero::GiveTutorialForCategory((CTCHero *)this,param_1,SUB41(iVar1,0));
  return;
}



//=== SpendExperienceOnAbility @ 005747a3 ===

/* [bsim sim=0.9445641329335073 <- ego_r]
   public: void __thiscall CTCInventoryExperience::SpendExperienceOnAbility(enum EHeroAbility) */

void __thiscall
CTCInventoryExperience::SpendExperienceOnAbility(CTCInventoryExperience *this,EHeroAbility param_1)

{
  int iVar1;
  bool bVar2;
  CTCHeroExperience *pCVar3;
  EHeroExperienceStatCategory EVar4;
  int iVar5;
  EHeroAbility EVar6;
  long lVar7;
  
  EVar6 = param_1;
  pCVar3 = GetTCHeroExperience(this);
  bVar2 = CTCInventoryAbilities::IsAbilityAvailable((CTCInventoryAbilities *)pCVar3,EVar6);
  if (bVar2) {
    bVar2 = true;
    lVar7 = 1;
    EVar6 = param_1;
    pCVar3 = GetTCHeroExperience(this);
    lVar7 = CTCInventoryAbilities::GetExperienceCostToUpgradeAbility
                      ((CTCInventoryAbilities *)pCVar3,EVar6,lVar7,bVar2);
    if ((0 < lVar7) && (iVar5 = param_1 * 0xc, *(int *)(iVar5 + *(int *)(this + 0x1f8)) == lVar7)) {
      EVar6 = param_1;
      pCVar3 = GetTCHeroExperience(this);
      EVar4 = CTCInventoryAbilities::GetAbilityStatType((CTCInventoryAbilities *)pCVar3,EVar6);
      EVar6 = param_1;
      pCVar3 = GetTCHeroExperience(this);
      CTCInventoryAbilities::UpgradeAbility((CTCInventoryAbilities *)pCVar3,EVar6);
      pCVar3 = GetTCHeroExperience(this);
      CTCHeroExperience::SpendExperience(pCVar3,EVar4,lVar7);
      *(undefined4 *)(*(int *)(this + 0x1f8) + 4 + iVar5) = 0;
      *(undefined4 *)(iVar5 + *(int *)(this + 0x1f8)) = 0;
      iVar1 = *(int *)(this + 0x1f8);
      bVar2 = true;
      lVar7 = 1;
      EVar6 = param_1;
      pCVar3 = GetTCHeroExperience(this);
      lVar7 = CTCInventoryAbilities::GetExperienceCostToUpgradeAbility
                        ((CTCInventoryAbilities *)pCVar3,EVar6,lVar7,bVar2);
      *(long *)(iVar5 + 8 + iVar1) = lVar7;
    }
    lVar7 = *(long *)(*(int *)(this + 0x1e0) + 0x24);
    pCVar3 = GetTCHeroExperience(this);
    CTCHeroExperience::ForceSpendableExperienceTo(pCVar3,lVar7);
    CTCHero::GiveTutorialForCategory
              ((CTCHero *)this,param_1,
               SUB41(*(undefined4 *)(*(int *)(this + 0x1f8) + param_1 * 0xc),0));
  }
  return;
}



//=== ProcessExperienceActionAssign @ 00576c0b ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCInventoryExperience::ProcessExperienceActionAssign(void) */

void __thiscall CTCInventoryExperience::ProcessExperienceActionAssign(CTCInventoryExperience *this)

{
  EHeroTrainableStatType EVar1;
  EHeroAbility EVar2;
  
  EVar1 = 0;
  do {
    SpendExperienceOnStat(this,EVar1);
    EVar1 = EVar1 + 1;
  } while ((int)EVar1 < 10);
  EVar2 = 1;
  do {
    SpendExperienceOnAbility(this,EVar2);
    EVar2 = EVar2 + 1;
  } while ((int)EVar2 < 0x14);
  UpdateExperienceSpendLevelUp(this);
  return;
}



//=== CTCInventoryExperience @ 0057832d ===

/* [bsim sim=0.764391456491111 <- ego_r]
   public: __thiscall CTCInventoryExperience::CTCInventoryExperience(class CThing &) */

CTCInventoryExperience * __thiscall
CTCInventoryExperience::CTCInventoryExperience(CTCInventoryExperience *this,CThing *param_1)

{
  NInventory::CTCInventoryBase::CTCInventoryBase((CTCInventoryBase *)this,param_1);
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x130));
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124f11c;
  *(undefined ***)(this + 0x130) = &PTR_LAB_0124f108;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x134));
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x140));
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x14c));
  *(undefined4 *)(this + 0x158) = 0;
  *(undefined4 *)(this + 0x15c) = 0;
  *(undefined4 *)(this + 0x160) = 0;
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x168) = 0;
  *(undefined4 *)(this + 0x16c) = 0;
  *(undefined4 *)(this + 0x170) = 0;
  *(undefined4 *)(this + 0x174) = 0;
  *(undefined4 *)(this + 0x178) = 0;
  *(undefined4 *)(this + 0x17c) = 0;
  *(undefined4 *)(this + 0x180) = 0;
  *(undefined4 *)(this + 0x184) = 0;
  *(undefined4 *)(this + 0x188) = 0;
  *(undefined4 *)(this + 0x18c) = 0;
  *(undefined4 *)(this + 400) = 0;
  *(undefined4 *)(this + 0x194) = 0;
  *(undefined4 *)(this + 0x198) = 0;
  *(undefined4 *)(this + 0x19c) = 0;
  FUN_005792bf((int)&param_1 + 3);
  *(undefined4 *)(this + 0x1e0) = 0;
  *(undefined4 *)(this + 0x1e4) = 0;
  *(undefined4 *)(this + 0x1e8) = 0;
  *(undefined4 *)(this + 0x1ec) = 0;
  *(undefined4 *)(this + 0x1f0) = 0;
  *(undefined4 *)(this + 500) = 0;
  *(undefined4 *)(this + 0x1f8) = 0;
  *(undefined4 *)(this + 0x1fc) = 0;
  *(undefined4 *)(this + 0x200) = 0;
  *(undefined4 *)(this + 0x208) = 0;
  *(undefined4 *)(this + 0x20c) = 0;
  *(undefined4 *)(this + 0x210) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x224));
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x230));
  *(undefined4 *)(this + 0x23c) = 0;
  *(undefined4 *)(this + 0x240) = 0;
  *(undefined4 *)(this + 0x244) = 0;
  *(undefined4 *)(this + 0x248) = 0;
  *(undefined4 *)(this + 0x24c) = 0;
  *(undefined4 *)(this + 0x250) = 0;
  *(undefined4 *)(this + 0x254) = 0;
  *(undefined4 *)(this + 600) = 0;
  this[0x21c] = (CTCInventoryExperience)0x0;
  this[0x214] = (CTCInventoryExperience)0x0;
  this[0x1a0] = (CTCInventoryExperience)0x0;
  return this;
}



//=== CHeroExperienceDef @ 005797a4 ===

/* [bsim sim=0.7287605830286314 <- ego_r]
   public: __thiscall CHeroExperienceDef::CHeroExperienceDef(void) */

CHeroExperienceDef * __thiscall CHeroExperienceDef::CHeroExperienceDef(CHeroExperienceDef *this)

{
  undefined4 uStack_8;
  
  uStack_8 = this;
  FUN_0042ab58((int)&uStack_8 + 3);
  FUN_0042ab58((int)&uStack_8 + 3);
  FUN_0042ab58((int)&uStack_8 + 3);
  FUN_0042ab58((int)&uStack_8 + 3);
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return this;
}



//=== CHeroMarriageInfo @ 0057a3a2 ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CHeroMarriageInfo::CHeroMarriageInfo(class CHeroMarriageInfo const &) */

void __thiscall
CHeroMarriageInfo::CHeroMarriageInfo(CHeroMarriageInfo *this,CHeroMarriageInfo *param_1)

{
  *(undefined4 *)this = *(undefined4 *)param_1;
  *(undefined4 *)(this + 4) = *(undefined4 *)(param_1 + 4);
  *(undefined4 *)(this + 8) = *(undefined4 *)(param_1 + 8);
  *(undefined4 *)(this + 0xc) = *(undefined4 *)(param_1 + 0xc);
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_1 + 0x10);
  this[0x14] = param_1[0x14];
  return;
}



//=== AddExperience @ 0057a59b ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCCoopSpirit::AddExperience(long) */

void __thiscall CTCCoopSpirit::AddExperience(CTCCoopSpirit *this,long param_1)

{
  *(int *)(this + 0x50) = *(int *)(this + 0x50) + param_1;
  return;
}



//=== GetMoralityMax @ 0057a67e ===

/* [bsim sim=1.0 <- ego_r]
   public: long __thiscall CTCHeroStats::GetMoralityMax(void)const  */

long __thiscall CTCHeroStats::GetMoralityMax(CTCHeroStats *this)

{
  int extraout_EAX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_retaddr;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,
             unaff_retaddr);
  return *(long *)(*(int *)(extraout_EAX + 0xdc) + 0xd0);
}



//=== GetMoralityPercentage @ 0057a690 ===

/* [bsim sim=0.6085559806648004 <- ego_r]
   public: float __thiscall CTCHeroStats::GetMoralityPercentage(void)const  */

float __thiscall CTCHeroStats::GetMoralityPercentage(CTCHeroStats *this)

{
  long lVar1;
  long lVar2;
  
  lVar1 = GetMoralityMax(this);
  lVar2 = GetMoralityMax(this);
  return (float)(lVar2 + *(int *)(this + 0x28)) / (float)(lVar1 << 1);
}



//=== GetHeroStatExperience @ 0057a8af ===

/* [bsim sim=1.0 <- ego_r]
   public: long __thiscall CTCHeroStats::GetHeroStatExperience(enum
   EHeroExperienceStatCategory)const  */

long __thiscall
CTCHeroStats::GetHeroStatExperience(CTCHeroStats *this,EHeroExperienceStatCategory param_1)

{
  int iStack_4;
  
  iStack_4 = 0;
  if (param_1 != 0) {
    if (param_1 == 1) {
      iStack_4 = 1;
    }
    else if (param_1 == 2) {
      iStack_4 = 2;
    }
    else if (param_1 == 3) {
      iStack_4 = 3;
    }
  }
  return *(long *)(*(int *)(this + 0x118) + iStack_4 * 4);
}



//=== AddHeroStatExperience @ 0057a8da ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCHeroStats::AddHeroStatExperience(enum
   EHeroExperienceStatCategory,long) */

void __thiscall
CTCHeroStats::AddHeroStatExperience
          (CTCHeroStats *this,EHeroExperienceStatCategory param_1,long param_2)

{
  int *piVar1;
  int iStack_4;
  
  iStack_4 = 0;
  if (param_1 != 0) {
    if (param_1 == 1) {
      iStack_4 = 1;
    }
    else if (param_1 == 2) {
      iStack_4 = 2;
    }
    else if (param_1 == 3) {
      iStack_4 = 3;
    }
  }
  piVar1 = (int *)(*(int *)(this + 0x118) + iStack_4 * 4);
  *piVar1 = *piVar1 + param_2;
  return;
}



//=== GetPercentageThroughRenownLevel @ 0057aad9 ===

/* [bsim sim=0.7862626454675572 <- ego_r]
   public: float __thiscall CTCHeroStats::GetPercentageThroughRenownLevel(void)const  */

float __thiscall CTCHeroStats::GetPercentageThroughRenownLevel(CTCHeroStats *this)

{
  uint uVar1;
  int extraout_EAX;
  int *piVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,unaff_ESI)
  ;
  uVar1 = *(uint *)(this + 0x70);
  if ((-1 < (int)uVar1) &&
     (piVar2 = (int *)(*(int *)(extraout_EAX + 0xdc) + 0xac),
     uVar1 < (uint)(*(int *)(*(int *)(extraout_EAX + 0xdc) + 0xb0) - *piVar2 >> 2))) {
    return (float)*(int *)(this + 0x78) / (float)*(int *)(*piVar2 + uVar1 * 4);
  }
  return 0.0;
}



//=== GetMaxBoastingCrowdForRenownLevel @ 0057ab14 ===

/* [bsim sim=0.9024847985210812 <- ego_r]
   public: long __thiscall CTCHeroStats::GetMaxBoastingCrowdForRenownLevel(long)const  */

long __thiscall CTCHeroStats::GetMaxBoastingCrowdForRenownLevel(CTCHeroStats *this,long param_1)

{
  int iVar1;
  int extraout_EAX;
  long lVar2;
  uint uVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,unaff_ESI)
  ;
  iVar1 = *(int *)(extraout_EAX + 0xdc);
  if (*(int *)(iVar1 + 0xbc) - *(int *)(iVar1 + 0xb8) >> 2 == 0) {
    lVar2 = 0;
  }
  else {
    uVar3 = *(uint *)(this + 0x70);
    if ((uint)(*(int *)(iVar1 + 0xbc) - *(int *)(iVar1 + 0xb8) >> 2) <= uVar3) {
      uVar3 = (*(int *)(iVar1 + 0xbc) - *(int *)(iVar1 + 0xb8) >> 2) - 1;
    }
    lVar2 = *(long *)(*(int *)(iVar1 + 0xb8) + uVar3 * 4);
  }
  return lVar2;
}



//=== GetMaxBoastingCrowdForRenownLevel @ 0057ab6a ===

/* [bsim sim=0.9024847985210812 <- ego_r]
   public: long __thiscall CTCHeroStats::GetMaxBoastingCrowdForRenownLevel(long)const  */

long __thiscall CTCHeroStats::GetMaxBoastingCrowdForRenownLevel(CTCHeroStats *this,long param_1)

{
  int iVar1;
  int extraout_EAX;
  long lVar2;
  uint uVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,unaff_ESI)
  ;
  iVar1 = *(int *)(extraout_EAX + 0xdc);
  if (*(int *)(iVar1 + 200) - *(int *)(iVar1 + 0xc4) >> 2 == 0) {
    lVar2 = 0;
  }
  else {
    uVar3 = *(uint *)(this + 0x70);
    if ((uint)(*(int *)(iVar1 + 200) - *(int *)(iVar1 + 0xc4) >> 2) <= uVar3) {
      uVar3 = (*(int *)(iVar1 + 200) - *(int *)(iVar1 + 0xc4) >> 2) - 1;
    }
    lVar2 = *(long *)(*(int *)(iVar1 + 0xc4) + uVar3 * 4);
  }
  return lVar2;
}



//=== SetMorality @ 0057c2e6 ===

/* [bsim sim=0.7961808875343257 <- ego_r]
   public: void __thiscall CTCHeroStats::SetMorality(long) */

void __thiscall CTCHeroStats::SetMorality(CTCHeroStats *this,long param_1)

{
  int iVar1;
  int *piVar2;
  CTCEntityEvents *this_00;
  CTCHeroMorph *this_01;
  char cVar3;
  bool bVar4;
  int extraout_EAX;
  int *piVar5;
  int extraout_EAX_00;
  CTCHeroStats **ppCVar6;
  CEventBase *pCVar7;
  ulong uVar8;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int iVar9;
  float fVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  CTCHeroStats *local_c;
  CTCHeroStats *local_8;
  
  if (this[0x2c] != (CTCHeroStats)0x0) {
    local_c = this;
    local_8 = this;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,
               unaff_ESI);
    local_c = *(CTCHeroStats **)(*(int *)(extraout_EAX + 0xdc) + 0xd0);
    local_8 = (CTCHeroStats *)-(int)local_c;
    if (param_1 < -(int)local_c) {
      ppCVar6 = &local_8;
    }
    else {
      ppCVar6 = &local_c;
      if (param_1 <= (int)local_c) {
        ppCVar6 = (CTCHeroStats **)&param_1;
      }
    }
    local_8 = *ppCVar6;
    iVar9 = (int)local_8 - *(int *)(this + 0x28);
    if (iVar9 != 0) {
      iVar1 = *(int *)(this + 4);
      cVar3 = FUN_00410de0(0x29);
      if (cVar3 != '\0') {
        local_c = (CTCHeroStats *)0x29;
        piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                        ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                      *)(iVar1 + 0x44),(ETCInterfaceType *)&local_c);
        piVar2 = *(int **)(iVar1 + 0x48);
        if ((piVar5 == piVar2) || (0x29 < *piVar5)) {
          piVar5 = piVar2;
        }
        if ((CTCHero *)piVar5[1] != (CTCHero *)0x0) {
          CTCHero::GiveTutorialForCategory((CTCHero *)piVar5[1],0x19,false);
        }
      }
      CPlayerGui::AddMoralityChange(DAT_013b8790,iVar9);
      bVar4 = CThingCreatureBase::IsChild(*(CThingCreatureBase **)(this + 4));
      if (bVar4) {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX,unaff_EDI);
        local_8 = *(CTCHeroStats **)(*(int *)(extraout_EAX_00 + 0xdc) + 0xd4);
        local_c = (CTCHeroStats *)-(int)local_8;
        if (param_1 < -(int)local_8) {
          ppCVar6 = &local_c;
        }
        else {
          ppCVar6 = &local_8;
          if (param_1 <= (int)local_8) {
            ppCVar6 = (CTCHeroStats **)&param_1;
          }
        }
        local_8 = *ppCVar6;
        iVar9 = (int)local_8 - *(int *)(this + 0x28);
      }
      if (iVar9 != 0) {
        iVar1 = *(int *)(this + 4);
        cVar3 = FUN_00410de0(0x42);
        if (cVar3 != '\0') {
          param_1 = 0x42;
          piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                          ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                        *)(iVar1 + 0x44),(ETCInterfaceType *)&param_1);
          piVar2 = *(int **)(iVar1 + 0x48);
          if ((piVar5 == piVar2) || (0x42 < *piVar5)) {
            piVar5 = piVar2;
          }
          this_00 = (CTCEntityEvents *)piVar5[1];
          pCVar7 = ::operator_new(0xc);
          if (pCVar7 == (CEventBase *)0x0) {
            pCVar7 = (CEventBase *)0x0;
          }
          else {
            *(undefined4 *)(pCVar7 + 4) = 0;
            *(undefined ***)pCVar7 = &PTR_LAB_0124f2dc;
            *(int *)(pCVar7 + 8) = iVar9;
          }
          CTCEntityEvents::AddEvent(this_00,pCVar7);
        }
        *(CTCHeroStats **)(this + 0x28) = local_8;
        CheckForNewExpressions(this);
        iVar9 = *(int *)(this + 4);
        cVar3 = FUN_00410de0(0x10);
        if (cVar3 != '\0') {
          param_1 = 0x10;
          CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
          LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      *)(iVar9 + 0x44),(ETCInterfaceType *)&param_1);
          CCharString::CCharString((CCharString *)&param_1,"MORALITY_CHANGE",-1);
          uVar12 = 0;
          uVar11 = 1;
          uVar8 = CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
          FUN_0066bdf0(&param_1,uVar8,uVar11,uVar12);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                     extraout_EDX_00,unaff_EDI);
        }
      }
    }
    iVar9 = *(int *)(this + 4);
    cVar3 = FUN_00410de0(3);
    if (cVar3 != '\0') {
      param_1 = 3;
      piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar9 + 0x44),(ETCInterfaceType *)&param_1);
      piVar2 = *(int **)(iVar9 + 0x48);
      if ((piVar5 == piVar2) || (3 < *piVar5)) {
        piVar5 = piVar2;
      }
      this_01 = (CTCHeroMorph *)piVar5[1];
      fVar10 = GetMoralityPercentage(this);
      CTCHeroMorph::SetAge(this_01,fVar10);
    }
  }
  return;
}



//=== UpgradeRenownLevel @ 0057cae9 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCHeroStats::UpgradeRenownLevel(void) */

void __thiscall CTCHeroStats::UpgradeRenownLevel(CTCHeroStats *this)

{
  *(int *)(this + 0x70) = *(int *)(this + 0x70) + 1;
  if (*(int *)(this + 0x74) < *(int *)(this + 0x70)) {
    *(int *)(this + 0x70) = *(int *)(this + 0x74);
  }
  *(undefined4 *)(this + 0x78) = 0;
  CheckForNewExpressions(this);
  return;
}



//=== ForceRenownLevelTo @ 0057cb00 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCHeroStats::ForceRenownLevelTo(long) */

void __thiscall CTCHeroStats::ForceRenownLevelTo(CTCHeroStats *this,long param_1)

{
  *(long *)(this + 0x70) = param_1;
  if (*(int *)(this + 0x70) < 0) {
    *(undefined4 *)(this + 0x70) = 0;
  }
  else if (*(int *)(this + 0x74) < *(int *)(this + 0x70)) {
    *(int *)(this + 0x70) = *(int *)(this + 0x74);
  }
  *(undefined4 *)(this + 0x78) = 0;
  CheckForNewExpressions(this);
  return;
}



//=== CTCHeroStats @ 0057cdb5 ===

/* [bsim sim=0.6075937509552255 <- ego_r]
   public: __thiscall CTCHeroStats::CTCHeroStats(class CThing &) */

CTCHeroStats * __thiscall CTCHeroStats::CTCHeroStats(CTCHeroStats *this,CThing *param_1)

{
  CDisplayEngine *pCVar1;
  uint uVar2;
  float fVar3;
  long lVar4;
  int extraout_EAX;
  int extraout_EAX_00;
  int iVar5;
  int extraout_EAX_01;
  ulong *extraout_EDX;
  ulong *extraout_EDX_00;
  ulong *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CThing **ppCVar6;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124f70c;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0xc));
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  this[0x2c] = (CTCHeroStats)0x1;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x34) = 0x3f000000;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x9c) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0xa0) = 0;
  this[0x68] = (CTCHeroStats)0x0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0xa8) = 0x3f800000;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0xb0) = 0;
  *(undefined4 *)(this + 0xb4) = 0;
  *(undefined4 *)(this + 0xac) = 0x3f800000;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  *(undefined4 *)(this + 0xc0) = 0;
  *(undefined4 *)(this + 0xc4) = 0;
  *(undefined4 *)(this + 200) = 0;
  *(undefined4 *)(this + 0xcc) = 0;
  *(undefined4 *)(this + 0xd0) = 0;
  *(undefined4 *)(this + 0xd4) = 0;
  *(undefined4 *)(this + 0xd8) = 0;
  *(undefined4 *)(this + 0xdc) = 0;
  *(undefined4 *)(this + 0xe0) = 0;
  *(undefined4 *)(this + 0xe4) = 0;
  *(undefined4 *)(this + 0xe8) = 0;
  *(undefined4 *)(this + 0xec) = 0;
  *(undefined4 *)(this + 0xf0) = 0;
  *(undefined4 *)(this + 0xf4) = 0;
  *(undefined4 *)(this + 0xf8) = 0;
  *(undefined4 *)(this + 0xfc) = 0;
  *(undefined4 *)(this + 0x100) = 0;
  *(undefined4 *)(this + 0x104) = 0;
  *(undefined4 *)(this + 0x108) = 0;
  *(undefined4 *)(this + 0x10c) = 0;
  *(undefined4 *)(this + 0x110) = 0;
  *(undefined4 *)(this + 0x114) = 0;
  *(undefined4 *)(this + 0x118) = 0;
  *(undefined4 *)(this + 0x11c) = 0;
  *(undefined4 *)(this + 0x120) = 0;
  *(undefined4 *)(this + 0x124) = 0;
  *(undefined4 *)(this + 0x128) = 0;
  *(undefined4 *)(this + 300) = 0;
  *(undefined4 *)(this + 0x130) = 0;
  *(undefined4 *)(this + 0x134) = 0;
  *(undefined4 *)(this + 0x138) = 0;
  *(undefined4 *)(this + 0x15c) = 0;
  *(undefined4 *)(this + 0x13c) = 0;
  *(undefined4 *)(this + 0x140) = 0;
  *(undefined4 *)(this + 0x144) = 0;
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)(this + 0x150) = 0;
  *(undefined4 *)(this + 0x154) = 0;
  this[0x158] = (CTCHeroStats)0x0;
  this[0x159] = (CTCHeroStats)0x0;
  *(undefined4 *)(this + 0x160) = 0;
  *(undefined4 *)(this + 0x164) = 0;
  *(undefined4 *)(this + 0x168) = 0;
  this[0x16c] = (CTCHeroStats)0x0;
  CCharString::CCharString((CCharString *)(this + 0x170),(CCharString *)&DAT_013b8b0c);
  *(undefined4 *)(this + 0x178) = 0;
  *(undefined4 *)(this + 0x1ac) = 0xffffffff;
  *(undefined4 *)(this + 0x1b0) = 0xffffffff;
  *(undefined4 *)(this + 0x17c) = 0;
  this[0x174] = (CTCHeroStats)0x1;
  this[0x194] = (CTCHeroStats)0x0;
  this[0x195] = (CTCHeroStats)0x0;
  *(undefined4 *)(this + 0x180) = 0;
  this[0x196] = (CTCHeroStats)0x0;
  this[0x197] = (CTCHeroStats)0x0;
  this[0x198] = (CTCHeroStats)0x0;
  *(undefined4 *)(this + 0x184) = 0;
  this[0x199] = (CTCHeroStats)0x0;
  this[0x19a] = (CTCHeroStats)0x0;
  *(undefined4 *)(this + 0x1b4) = 0;
  *(undefined4 *)(this + 0x188) = 0;
  this[0x1b8] = (CTCHeroStats)0x1;
  *(undefined4 *)(this + 0x18c) = 0;
  *(undefined4 *)(this + 400) = 0;
  *(undefined4 *)(this + 0x19c) = 0;
  *(undefined4 *)(this + 0x1a0) = 0;
  *(undefined4 *)(this + 0x1a4) = 0x3f800000;
  *(undefined4 *)(this + 0x1bc) = 0;
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x1c0));
  *(undefined ***)(this + 0x1c0) = &PTR__scalar_deleting_destructor__0122f598;
  *(undefined4 *)(this + 0x1c4) = 0;
  this[0x1d4] = (CTCHeroStats)0x1;
  FUN_0057fda2((int)&param_1 + 3);
  param_1 = (CThing *)0x0;
  std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x18),0xb,(uint)&param_1)
  ;
  param_1 = (CThing *)0x0;
  std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x130),0x14,
             (uint)&param_1);
  ppCVar6 = &param_1;
  param_1 = (CThing *)0x0;
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  uVar2 = CWorldMap::DrawGetNoMaps((CWorldMap *)pCVar1);
  std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x124),uVar2,
             (uint)ppCVar6);
  param_1 = (CThing *)0x0;
  std::vector<unsigned_int,std::allocator<unsigned_int>_>::resize
            ((vector<unsigned_int,std::allocator<unsigned_int>_> *)(this + 0x118),4,(uint)&param_1);
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  fVar3 = (float)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)pCVar1);
  fVar3 = GFFloatRandom(fVar3,extraout_EDX);
  *(float *)(this + 0x1c8) = fVar3;
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  fVar3 = (float)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)pCVar1);
  fVar3 = GFFloatRandom(fVar3,extraout_EDX_00);
  *(float *)(this + 0x1cc) = fVar3;
  pCVar1 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  fVar3 = (float)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)pCVar1);
  fVar3 = GFFloatRandom(fVar3,extraout_EDX_01);
  *(float *)(this + 0x1d0) = fVar3;
  lVar4 = GetFatnessReducingFrequencyInFrames(this);
  *(long *)(this + 0x6c) = lVar4;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_02,
             unaff_EDI);
  *(undefined4 *)(this + 0x30) = *(undefined4 *)(*(int *)(extraout_EAX + 0xdc) + 0x68);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_03,
             unaff_EDI);
  iVar5 = *(int *)(*(int *)(extraout_EAX_00 + 0xdc) + 0x3c) + *(int *)(this + 0x90);
  *(int *)(this + 0x5c) = iVar5;
  *(int *)(this + 0x58) = iVar5;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_04,
             unaff_EDI);
  *(int *)(this + 0x74) =
       *(int *)(*(int *)(extraout_EAX_01 + 0xdc) + 0xb0) -
       *(int *)(*(int *)(extraout_EAX_01 + 0xdc) + 0xac) >> 2;
  CGuiControlTreePane::SortTreeRecursively
            ((CGuiControlTreePane *)(this + 0x1d8),
             (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
  return this;
}



//=== AddRenown @ 0057d8be ===

/* [bsim sim=0.9449218004318237 <- ego_r]
   public: void __thiscall CTCHeroStats::AddRenown(long) */

void __thiscall CTCHeroStats::AddRenown(CTCHeroStats *this,long param_1)

{
  int iVar1;
  int iVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar3;
  char cVar4;
  int extraout_EAX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  float fVar5;
  CTCHeroStats *local_8;
  
  *(int *)(this + 0x78) = *(int *)(this + 0x78) + param_1;
  if (*(int *)(this + 0x78) < 0) {
    *(undefined4 *)(this + 0x78) = 0;
  }
  *(int *)(this + 0x7c) = *(int *)(this + 0x7c) + param_1;
  if (*(int *)(this + 0x7c) < 0) {
    *(undefined4 *)(this + 0x7c) = 0;
  }
  local_8 = this;
  fVar5 = GetPercentageThroughRenownLevel(this);
  ppVar3 = extraout_EDX;
  while (1.0 <= fVar5) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,ppVar3,
               unaff_EDI);
    iVar1 = *(int *)(this + 0x78);
    iVar2 = *(int *)(*(int *)(*(int *)(extraout_EAX + 0xdc) + 0xac) + *(int *)(this + 0x70) * 4);
    UpgradeRenownLevel(this);
    *(int *)(this + 0x78) = iVar1 - iVar2;
    fVar5 = GetPercentageThroughRenownLevel(this);
    ppVar3 = extraout_EDX_00;
  }
  if (param_1 != 0) {
    iVar1 = *(int *)(this + 4);
    cVar4 = FUN_00410de0(0xb7);
    if (cVar4 != '\0') {
      local_8 = (CTCHeroStats *)0xb7;
      CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
      LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                  *)(iVar1 + 0x44),(ETCInterfaceType *)&local_8);
      FUN_007b7cd0((float)param_1);
    }
    CPlayerGui::AddMoralityChange(DAT_013b8790,param_1);
  }
  return;
}



//=== pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus><CCharString_const_,CTCHeroStats::MirroredCriminalStatus> @ 0057ed6c ===

/* [bsim sim=0.8532211459159396 <- ego_r]
   public: __thiscall std::pair<class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus>::pair<class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus><class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus>(struct std::pair<class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus> &&) */

pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus> * __thiscall
std::pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus>::
pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus><CCharString_const_,CTCHeroStats::MirroredCriminalStatus>
          (pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus> *this,
          pair<class_CCharString_const_,struct_CTCHeroStats::MirroredCriminalStatus> *param_1)

{
  int iVar1;
  pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus> *ppVar2;
  undefined4 *in_stack_00000008;
  
  CCharString::CCharString((CCharString *)this,(CCharString *)param_1);
  ppVar2 = this;
  for (iVar1 = 0x11; ppVar2 = ppVar2 + 4, iVar1 != 0; iVar1 = iVar1 + -1) {
    *(undefined4 *)ppVar2 = *in_stack_00000008;
    in_stack_00000008 = in_stack_00000008 + 1;
  }
  return this;
}



//=== GFSerialiseVectorBinaryOut<CHeroMarriageInfo> @ 0057f109 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   void __fastcall GFSerialiseVectorBinaryOut<class CHeroMarriageInfo>(class CDataOutputStream
   &,class std::vector<class CHeroMarriageInfo,class std::allocator<class CHeroMarriageInfo> > &) */

void __fastcall
GFSerialiseVectorBinaryOut<CHeroMarriageInfo>
          (CDataOutputStream *param_1,
          vector<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_> *param_2)

{
  int iVar1;
  int iVar2;
  CPersistTraits<CHeroMarriageInfo> local_18 [24];
  
  iVar1 = (*(int *)(param_2 + 4) - *(int *)param_2) / 0x18;
  CDataOutputStream::WriteCBYTE(param_1,(char)iVar1);
  if (0 < iVar1) {
    iVar2 = 0;
    do {
      CPersistTraits<CHeroMarriageInfo>::TransferBinaryOut
                (local_18,param_1,(CHeroMarriageInfo *)(*(int *)param_2 + iVar2));
      iVar2 = iVar2 + 0x18;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 0057f279 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x18;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 6; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus><CCharString_const_,CTCHeroStats::MirroredCriminalStatus> @ 0057f30a ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall std::pair<class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus>::pair<class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus><class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus>(struct std::pair<class CCharString const ,struct
   CTCHeroStats::MirroredCriminalStatus> &&) */

pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus> * __thiscall
std::pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus>::
pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus><CCharString_const_,CTCHeroStats::MirroredCriminalStatus>
          (pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus> *this,
          pair<class_CCharString_const_,struct_CTCHeroStats::MirroredCriminalStatus> *param_1)

{
  int iVar1;
  pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus> *ppVar2;
  
  CCharString::CCharString((CCharString *)this,(CCharString *)param_1);
  iVar1 = 0x11;
  ppVar2 = this;
  while( true ) {
    ppVar2 = ppVar2 + 4;
    param_1 = param_1 + 4;
    if (iVar1 == 0) break;
    iVar1 = iVar1 + -1;
    *(undefined4 *)ppVar2 = *(undefined4 *)param_1;
  }
  return this;
}



//=== _Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*> @ 0057f338 ===

/* [bsim sim=0.7435733930203144 <- ego_r]
   class CHeroMarriageInfo * __fastcall std::_Copy_backward<class CHeroMarriageInfo *,class
   CHeroMarriageInfo *>(class CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo
   *,struct std::_Nonscalar_ptr_iterator_tag) */

CHeroMarriageInfo * __fastcall
std::_Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*>
          (int param_1,undefined4 *param_2,CHeroMarriageInfo *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  CHeroMarriageInfo *pCVar4;
  
  iVar1 = ((int)param_2 - param_1) / 0x18;
  if (0 < iVar1) {
    do {
      param_3 = param_3 + -0x18;
      param_2 = param_2 + -6;
      iVar1 = iVar1 + -1;
      puVar3 = param_2;
      pCVar4 = param_3;
      for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
        *(undefined4 *)pCVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        pCVar4 = pCVar4 + 4;
      }
    } while (iVar1 != 0);
  }
  return param_3;
}



//=== _Copy_impl<std::_List_unchecked_iterator<std::_List_val<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>,std::back_insert_iterator<std::vector<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>_> @ 005810ae ===

/* [bsim sim=1.0 <- ego_r]
   class std::back_insert_iterator<class std::vector<class CHeroMarriageInfo,class
   std::allocator<class CHeroMarriageInfo> > > __fastcall std::_Copy_impl<class
   std::_List_unchecked_iterator<class std::_List_val<class CHeroMarriageInfo,class
   std::allocator<class CHeroMarriageInfo> > >,class std::back_insert_iterator<class
   std::vector<class CHeroMarriageInfo,class std::allocator<class CHeroMarriageInfo> > > >(class
   std::_List_unchecked_iterator<class std::_List_val<class CHeroMarriageInfo,class
   std::allocator<class CHeroMarriageInfo> > >,class std::_List_unchecked_iterator<class
   std::_List_val<class CHeroMarriageInfo,class std::allocator<class CHeroMarriageInfo> > >,class
   std::back_insert_iterator<class std::vector<class CHeroMarriageInfo,class std::allocator<class
   CHeroMarriageInfo> > >,struct std::_Nonscalar_ptr_iterator_tag) */

undefined4 * __fastcall
std::
_Copy_impl<std::_List_unchecked_iterator<std::_List_val<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>,std::back_insert_iterator<std::vector<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>_>_>
          (undefined4 *param_1,undefined4 param_2,undefined4 *param_3,undefined4 *param_4,
          undefined4 param_5)

{
  for (; param_3 != param_4; param_3 = (undefined4 *)*param_3) {
    FUN_00580500(param_3 + 2);
  }
  *param_1 = param_5;
  return param_1;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 0059264a ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x1c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 7; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*> @ 005927ab ===

/* [bsim sim=0.7435733930203144 <- ego_r]
   class CHeroMarriageInfo * __fastcall std::_Copy_backward<class CHeroMarriageInfo *,class
   CHeroMarriageInfo *>(class CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo
   *,struct std::_Nonscalar_ptr_iterator_tag) */

CHeroMarriageInfo * __fastcall
std::_Copy_backward<CHeroMarriageInfo*,CHeroMarriageInfo*>
          (int param_1,undefined4 *param_2,CHeroMarriageInfo *param_3)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  CHeroMarriageInfo *pCVar4;
  
  iVar1 = ((int)param_2 - param_1) / 0x1c;
  if (0 < iVar1) {
    do {
      param_3 = param_3 + -0x1c;
      param_2 = param_2 + -7;
      iVar1 = iVar1 + -1;
      puVar3 = param_2;
      pCVar4 = param_3;
      for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
        *(undefined4 *)pCVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        pCVar4 = pCVar4 + 4;
      }
    } while (iVar1 != 0);
  }
  return param_3;
}



//=== _Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_> @ 00592dfb ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall std::_Pair_base<long,class CDefPointer<class CHeroAbilityDef const >
   >::_Pair_base<long,class CDefPointer<class CHeroAbilityDef const > >(struct
   std::_Pair_base<long,class CDefPointer<class CHeroAbilityDef const > > const &) */

void __thiscall
std::_Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_>::
_Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_>
          (_Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_> *this,
          _Pair_base<long,class_CDefPointer<class_CHeroAbilityDef_const_>_> *param_1)

{
  int iVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  iVar1 = *(int *)(param_1 + 4);
  *(int *)(this + 4) = iVar1;
  if (iVar1 != 0) {
    *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + 1;
  }
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00593666 ===

/* [bsim sim=0.8063763850608386 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar2;
  CCharString *pCVar3;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar2 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar3 = *(CCharString **)param_2;
  if (pCVar3 != param_1) {
    if (pCVar3 != (CCharString *)0x0) {
      pCVar1 = pCVar3 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar3 + 4))();
      }
      *(undefined4 *)pCVar2 = 0;
    }
    *(CCharString **)pCVar2 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  pCVar3 = param_1 + 4;
  *(int *)pCVar3 = *(int *)pCVar3 + -1;
  if (*(int *)pCVar3 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== BuildRenownString @ 005bf414 ===

/* [bsim sim=0.5539374184457945 <- ego_r]
   public: class CWideString __thiscall CTCInventoryStats::BuildRenownString(void) */

void __thiscall CTCInventoryStats::BuildRenownString(CTCInventoryStats *this)

{
  char cVar1;
  int *piVar2;
  EWeaponClass *pEVar3;
  CCategory *pCVar4;
  EWeaponClass *unaff_EDI;
  CThing *in_stack_00000004;
  CThing *pCVar5;
  
  pCVar5 = in_stack_00000004;
  cVar1 = FUN_00410de0(0x1a);
  if (cVar1 == '\0') {
    pCVar4 = (CCategory *)0x0;
  }
  else {
    in_stack_00000004 = (CThing *)&DAT_0000001a;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(pCVar5 + 0x44),(ETCInterfaceType *)&stack0x00000004);
    if ((piVar2 == *(int **)(pCVar5 + 0x48)) || (0x1a < *piVar2)) {
      piVar2 = *(int **)(pCVar5 + 0x48);
    }
    pCVar4 = (CCategory *)piVar2[1];
  }
  pEVar3 = (EWeaponClass *)NInventory::CCategory::GetCategoryIdentifier(pCVar4);
  pCVar4 = (CCategory *)
           std::vector<EWeaponClass,std::allocator<EWeaponClass>_>::_Umove<EWeaponClass*>
                     (*(vector<EWeaponClass,std::allocator<EWeaponClass>_> **)(this + 0x20),pEVar3,
                      (EWeaponClass *)pCVar5,unaff_EDI);
  NInventory::CCategory::UpdateItemStatsForThing(pCVar4,pCVar5);
  return;
}



//=== CTCHero @ 005cd2a6 ===

/* [bsim sim=0.7314107551612937 <- ego_r]
   public: __thiscall CTCHero::CTCHero(class CThing &) */

CTCHero * __thiscall CTCHero::CTCHero(CTCHero *this,CThing *param_1)

{
  NInventory::CTCInventoryBase::CTCInventoryBase((CTCInventoryBase *)this,param_1);
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x130));
  *(undefined ***)this = &PTR__vector_deleting_destructor__01254adc;
  *(undefined ***)(this + 0x130) = &PTR_LAB_01254ac4;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x134));
  *(undefined4 *)(this + 0x140) = 0;
  this[0x144] = (CTCHero)0x0;
  *(undefined4 *)(this + 0x148) = 0;
  *(undefined4 *)(this + 0x14c) = 0;
  *(undefined4 *)(this + 0x150) = 0;
  return this;
}



//=== BuildRenownString @ 005cd433 ===

/* [bsim sim=0.908597419405007 <- ego_r]
   public: class CWideString __thiscall CTCInventoryStats::BuildRenownString(void) */

CCharString * __thiscall CTCInventoryStats::BuildRenownString(CTCInventoryStats *this)

{
  int iVar1;
  char cVar2;
  CCharString *pCVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CCharString *in_stack_00000004;
  CTCInventoryStats *local_c;
  CTCInventoryStats *local_8;
  
  iVar1 = *(int *)(this + 4);
  local_c = this;
  local_8 = this;
  cVar2 = FUN_00410de0(4);
  if (cVar2 != '\0') {
    local_c = (CTCInventoryStats *)&DAT_00000004;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
    LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                *)(iVar1 + 0x44),(ETCInterfaceType *)&local_c);
  }
  CCharString::CCharString((CCharString *)&local_8,"TEXT_GUI_STATS_RENOWNCLASS_LEVEL",-1);
  pCVar3 = (CCharString *)GFIntToCharString_API();
  CCharString::operator+=((CCharString *)&local_8,pCVar3);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_c,extraout_EDX,
             unaff_ESI);
  NGameText::CDataBank::GetTextBySymbol(*(CDataBank **)(DAT_013b86a0 + 0x14),in_stack_00000004);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,unaff_ESI);
  return in_stack_00000004;
}



//=== CreateExperienceSliders @ 005cdd86 ===

/* [bsim sim=0.7793982540988472 <- ego_r]
   public: class std::vector<class NUISystem::CComponent *,class std::allocator<class
   NUISystem::CComponent *> > __thiscall CTCInventoryStats::CreateExperienceSliders(class
   NUISystem::CComponent *,long,bool) */

CComponent * __thiscall
CTCInventoryStats::CreateExperienceSliders
          (CTCInventoryStats *this,CComponent *param_1,long param_2,bool param_3)

{
  int *piVar1;
  CTCInventoryStats *this_00;
  char cVar2;
  int *piVar3;
  CWideString *pCVar4;
  CComponent *pCVar5;
  long lVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_03;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int iVar7;
  undefined3 in_stack_0000000d;
  CRGBColour in_stack_00000010;
  C2DCoordI *pCVar8;
  long lVar9;
  CRGBColour *pCVar10;
  long lVar11;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar12;
  void *local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  int local_30;
  long local_2c [2];
  CCharString local_24 [4];
  ETCInterfaceType local_20;
  CTCInventoryStats *local_1c;
  CFrontEndManager *local_18;
  CDataBank *local_14;
  CTCInventoryExperience *local_10;
  CRGBColour local_c;
  undefined1 local_b;
  undefined1 local_a;
  undefined1 local_9;
  long local_8;
  
  local_14 = *(CDataBank **)(DAT_013b86a0 + 0x14);
  iVar7 = *(int *)(this + 4);
  local_40 = (void *)0x0;
  local_3c = 0;
  local_38 = 0;
  local_1c = this;
  cVar2 = FUN_00410de0(0x16);
  if (cVar2 != '\0') {
    local_20 = 0x16;
    piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar7 + 0x44),&local_20);
    piVar1 = *(int **)(iVar7 + 0x48);
    if ((piVar3 == piVar1) || (0x16 < *piVar3)) {
      piVar3 = piVar1;
    }
    local_10 = (CTCInventoryExperience *)piVar3[1];
  }
  local_18 = NUISystem::CFrontEndManager::GetInstance();
  iVar7 = *(int *)(local_1c + 4);
  cVar2 = FUN_00410de0(0x68);
  if (cVar2 != '\0') {
    local_20 = 0x68;
    piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar7 + 0x44),&local_20);
    piVar1 = *(int **)(iVar7 + 0x48);
    if ((piVar3 == piVar1) || (0x68 < *piVar3)) {
      piVar3 = piVar1;
    }
    local_1c = (CTCInventoryStats *)piVar3[1];
  }
  this_00 = local_1c;
  local_2c[0] = 0;
  local_2c[1] = 0;
  lVar11 = *(long *)(local_1c + 0x14);
  local_8 = lVar11;
  if (in_stack_00000010 == (CRGBColour)0x0) {
    local_8 = CTCInventoryExperience::GetOriginalExperience(local_10,0);
  }
  CCharString::CCharString((CCharString *)&local_20,"TEXT_GUI_STATS_EXPERIENCE_GENERAL",-1);
  pCVar10 = &local_c;
  pCVar8 = (C2DCoordI *)&local_34;
  local_a = 0;
  local_b = 0xb4;
  local_c = (CRGBColour)0x0;
  local_9 = 0xff;
  local_34 = 0;
  local_30 = 0;
  lVar6 = local_8;
  pCVar4 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(local_14,local_24);
  pCVar5 = NUISystem::CManager::CreateLevelSlider
                     ((CManager *)local_18,pCVar4,pCVar8,lVar11,lVar6,pCVar10);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)local_2c,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar5);
  CCharString::~CCharString(local_24);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_20,extraout_EDX
             ,unaff_EDI);
  lVar11 = param_2;
  ppVar12 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_2c;
  (**(code **)(*(int *)param_2 + 0xec))();
  param_2 = local_2c[0];
  FUN_00486024(&param_2);
  lVar6 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)this_00,0);
  param_2 = lVar6;
  if (in_stack_00000010 == (CRGBColour)0x0) {
    lVar6 = CTCInventoryExperience::GetOriginalExperience(local_10,1);
  }
  local_8 = lVar6;
  CCharString::CCharString((CCharString *)&local_20,"TEXT_GUI_STATS_EXPERIENCE_STRENGTH",-1);
  pCVar10 = &local_c;
  pCVar8 = (C2DCoordI *)&local_34;
  iVar7 = _param_3 + 0x19;
  local_a = 0xb4;
  local_b = 0;
  local_c = (CRGBColour)0x0;
  local_9 = 0xff;
  local_34 = 0;
  lVar6 = param_2;
  lVar9 = local_8;
  local_30 = iVar7;
  pCVar4 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(local_14,local_24);
  pCVar5 = NUISystem::CManager::CreateLevelSlider
                     ((CManager *)local_18,pCVar4,pCVar8,lVar6,lVar9,pCVar10);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)local_2c,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar5);
  CCharString::~CCharString(local_24);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_20,
             extraout_EDX_00,ppVar12);
  ppVar12 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_2c;
  (**(code **)(*(int *)lVar11 + 0xec))();
  param_2 = local_2c[0];
  FUN_00486024(&param_2);
  lVar6 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)local_1c,1);
  param_2 = lVar6;
  if (in_stack_00000010 == (CRGBColour)0x0) {
    lVar6 = CTCInventoryExperience::GetOriginalExperience(local_10,2);
  }
  local_8 = lVar6;
  CCharString::CCharString((CCharString *)&local_20,"TEXT_GUI_STATS_EXPERIENCE_SKILL",-1);
  local_30 = iVar7 * 2;
  pCVar10 = (CRGBColour *)&param_3;
  pCVar8 = (C2DCoordI *)&local_34;
  _param_3 = 0xffb4b400;
  local_34 = 0;
  lVar6 = param_2;
  lVar9 = local_8;
  pCVar4 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(local_14,local_24);
  pCVar5 = NUISystem::CManager::CreateLevelSlider
                     ((CManager *)local_18,pCVar4,pCVar8,lVar6,lVar9,pCVar10);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)local_2c,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar5);
  CCharString::~CCharString(local_24);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_20,
             extraout_EDX_01,ppVar12);
  ppVar12 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_2c;
  (**(code **)(*(int *)lVar11 + 0xec))();
  param_2 = local_2c[0];
  FUN_00486024(&param_2);
  lVar6 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)local_1c,2);
  param_2 = lVar6;
  if (in_stack_00000010 == (CRGBColour)0x0) {
    lVar6 = CTCInventoryExperience::GetOriginalExperience(local_10,3);
  }
  local_8 = lVar6;
  CCharString::CCharString((CCharString *)&param_3,"TEXT_GUI_STATS_EXPERIENCE_WILL",-1);
  local_30 = iVar7 * 3;
  pCVar10 = &stack0x00000010;
  pCVar8 = (C2DCoordI *)&local_34;
  _in_stack_00000010 = 0xff0000b4;
  local_34 = 0;
  lVar6 = param_2;
  lVar9 = local_8;
  pCVar4 = (CWideString *)NGameText::CDataBank::GetTextBySymbol(local_14,local_24);
  pCVar5 = NUISystem::CManager::CreateLevelSlider
                     ((CManager *)local_18,pCVar4,pCVar8,lVar6,lVar9,pCVar10);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)local_2c,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar5);
  CCharString::~CCharString(local_24);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
             extraout_EDX_02,ppVar12);
  (**(code **)(*(int *)lVar11 + 0xec))(local_2c);
  _in_stack_00000010 = local_2c[0];
  FUN_00486024(&stack0x00000010);
  _lua_stackspace_4(&local_40);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_2c
             ,extraout_EDX_03);
  if (local_40 != (void *)0x0) {
    free(local_40);
  }
  return param_1;
}



//=== ConstructExperienceList @ 005d6748 ===

/* [bsim sim=0.7892535172012272 <- ego_r]
   public: virtual class NUISystem::CComponent * __thiscall
   CTCInventoryStats::ConstructExperienceList(class NUISystem::CComponent *) */

CComponent * __thiscall
CTCInventoryStats::ConstructExperienceList(CTCInventoryStats *this,CComponent *param_1)

{
  CTCInventoryStats *this_00;
  char cVar1;
  int *piVar2;
  CComponent *pCVar3;
  CRumbleDef *this_01;
  CAction *pCVar4;
  int iVar5;
  int *piVar6;
  CFrontEndManager *this_02;
  CWideString *pCVar7;
  CComponent *pCVar8;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_01;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_02;
  undefined4 extraout_EDX_03;
  CTCInventoryAbilities *this_03;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar9;
  undefined8 uVar10;
  C2DCoordI *pCVar11;
  float fVar12;
  CRGBFloatColour *pCVar13;
  CRGBFloatColour *pCVar14;
  float fVar15;
  bool bVar16;
  bool bVar17;
  CCharString *pCVar18;
  CCharString CVar19;
  float fVar20;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar21;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 *local_34 [2];
  void *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  CTCInventoryStats *local_20;
  CComponent *local_1c;
  CTCInventoryExperience *local_18;
  CFrontEndManager *local_14;
  int local_10;
  undefined4 local_c;
  float local_8;
  
  local_20 = this;
  local_14 = NUISystem::CFrontEndManager::GetInstance();
  iVar5 = *(int *)(this + 4);
  cVar1 = FUN_00410de0();
  this_03 = (CTCInventoryAbilities *)param_1;
  if (cVar1 != '\0') {
    local_1c = (CComponent *)0x68;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar5 + 0x44),(ETCInterfaceType *)&local_1c);
    piVar6 = *(int **)(iVar5 + 0x48);
    if ((piVar2 == piVar6) || (0x68 < *piVar2)) {
      piVar2 = piVar6;
    }
    this_03 = (CTCInventoryAbilities *)piVar2[1];
  }
  iVar5 = *(int *)(this + 4);
  cVar1 = FUN_00410de0();
  if (cVar1 != '\0') {
    local_1c = (CComponent *)0x16;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar5 + 0x44),(ETCInterfaceType *)&local_1c);
    piVar6 = *(int **)(iVar5 + 0x48);
    if ((piVar2 == piVar6) || (0x16 < *piVar2)) {
      piVar2 = piVar6;
    }
    local_18 = (CTCInventoryExperience *)piVar2[1];
  }
  CCharString::CCharString((CCharString *)&local_10,"UI_STATS_EXP_LIST",-1);
  pCVar3 = NUISystem::CManager::CreateComponent((CManager *)local_14,(CCharString *)&local_10,false)
  ;
  local_1c = pCVar3;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,extraout_EDX
             ,unaff_EDI);
  iVar5 = *(int *)pCVar3;
  CCharString::CCharString((CCharString *)&stack0xffffff9c,"UI_STATS_EXP_VIEWPORT",-1);
  (**(code **)(iVar5 + 0xc))();
  this_01 = ::operator_new(0x10);
  if (this_01 == (CRumbleDef *)0x0) {
    pCVar4 = (CAction *)0x0;
  }
  else {
    pCVar4 = (CAction *)CRumbleDef::CRumbleDef(this_01);
  }
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_34,pCVar4);
  *local_34[0] = 3;
  iVar5 = *(int *)param_1;
  CCharString::CCharString((CCharString *)&stack0xffffff98,"UI_TOP_LEVEL_MENU_SCREEN",-1);
  iVar5 = (**(code **)(iVar5 + 0xc))();
  param_1 = (CComponent *)(-(uint)(iVar5 != 0) & iVar5 + 0x18U);
  FUN_0042b646();
  param_1 = ::operator_new(0xc);
  if (param_1 == (CComponent *)0x0) {
    param_1 = (CComponent *)0x0;
  }
  else {
    *(undefined4 *)(param_1 + 4) = 2;
    *(undefined ***)param_1 = &PTR_LAB_012366ec;
    *(undefined4 *)(param_1 + 8) = 1;
  }
  FUN_0042b646();
  iVar5 = *(int *)local_1c;
  CCharString::CCharString((CCharString *)&stack0xffffff94,"UI_STATS_EXP_DUMMY_MENU_ENTRY",-1);
  piVar6 = (int *)(**(code **)(iVar5 + 0xc))();
  if (piVar6 != (int *)0x0) {
    ppVar21 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_34;
    (**(code **)(*piVar6 + 0x110))();
    CCharString::CCharString((CCharString *)&param_1,"UI_RING_PIC_EXP_STATS",-1);
    CVar19 = (CCharString)0x0;
    pCVar18 = (CCharString *)&param_1;
    this_02 = NUISystem::CFrontEndManager::GetInstance();
    pCVar3 = NUISystem::CManager::CreateComponent((CManager *)this_02,pCVar18,(bool)CVar19);
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_28,(CAction *)pCVar3);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX_00,ppVar21);
    (**(code **)(*piVar6 + 0xec))();
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_28,extraout_EDX_01);
  }
  iVar5 = *(int *)local_1c;
  CCharString::CCharString((CCharString *)&stack0xffffff90,"UI_STATS_EXP_SCREEN",-1);
  uVar9 = (**(code **)(iVar5 + 0xc))();
  param_1 = (CComponent *)uVar9;
  if (param_1 != (CComponent *)0x0) {
    local_10 = *(int *)param_1;
    CCharString::CCharString
              ((CCharString *)&stack0xffffff8c,"UI_STATS_EXP_LEVEL_SLIDER_CONTAINER",-1);
    pCVar3 = param_1;
    iVar5 = (**(code **)(local_10 + 0xc))();
    if ((iVar5 != 0) &&
       (CreateExperienceSliders(local_20,(CComponent *)&local_2c,iVar5,true),
       local_2c != (void *)0x0)) {
      free(local_2c);
    }
    iVar5 = *(int *)pCVar3;
    CCharString::CCharString
              ((CCharString *)&stack0xffffff88,"UI_STATS_EXP_TICK_SLIDER_CONTAINER",-1);
    uVar10 = (**(code **)(iVar5 + 0xc))();
    uVar9 = CONCAT44((int)((ulonglong)uVar10 >> 0x20),param_1);
    pCVar3 = (CComponent *)uVar10;
    if (pCVar3 != (CComponent *)0x0) {
      param_1 = (CComponent *)0x0;
      local_28 = 0;
      local_24 = 0;
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,0);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,0);
      iVar5 = FUN_006d6fa0();
      fVar12 = local_8;
      pCVar14 = (CRGBFloatColour *)&local_44;
      param_1 = (CComponent *)((float)(iVar5 + (int)local_8) + (float)param_1);
      local_54 = 0x3f333333;
      local_c = 0;
      local_8 = 0.0;
      local_44 = 0;
      local_40 = 0;
      local_3c = 0;
      local_38 = 0x3f800000;
      local_50 = 0;
      local_4c = 0;
      local_48 = 0x3f800000;
      fVar20 = 20.0;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_54;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_10);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_10);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,1);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,1);
      iVar5 = FUN_006d6fa0(1);
      fVar12 = local_8;
      pCVar14 = (CRGBFloatColour *)&local_54;
      param_1 = (CComponent *)((float)(iVar5 + (int)local_8) + (float)param_1);
      local_44 = 0x3f333333;
      local_c = 0;
      local_8 = 4.2039e-44;
      local_54 = 0;
      local_50 = 0;
      local_4c = 0;
      local_48 = 0x3f800000;
      local_40 = 0;
      local_3c = 0;
      local_38 = 0x3f800000;
      fVar20 = 20.0;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_44;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_10);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_10);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,2);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,2);
      iVar5 = FUN_006d6fa0(2,&param_1);
      fVar12 = local_8;
      pCVar14 = (CRGBFloatColour *)&local_54;
      param_1 = (CComponent *)((float)(iVar5 + (int)local_8) + (float)param_1);
      local_44 = 0x3f333333;
      local_c = 0;
      local_8 = 8.40779e-44;
      local_54 = 0;
      local_50 = 0;
      local_4c = 0;
      local_48 = 0x3f800000;
      local_40 = 0;
      local_3c = 0;
      local_38 = 0x3f800000;
      fVar20 = 20.0;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_44;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_10);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_10);
      (**(code **)(*(int *)pCVar3 + 0xec))(&local_28);
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,3);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,3);
      iVar5 = FUN_006d6fa0(3,&param_1);
      fVar12 = local_8;
      param_1 = (CComponent *)((float)(iVar5 + (int)local_8) + (float)param_1);
      local_54 = 0;
      pCVar14 = (CRGBFloatColour *)&local_54;
      local_50 = 0;
      local_c = 0;
      local_8 = 1.54143e-43;
      local_4c = 0;
      local_48 = 0x3f800000;
      local_44 = 0x3f333333;
      local_40 = 0x3f333333;
      local_3c = 0;
      local_38 = 0x3f800000;
      fVar20 = 20.0;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_44;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_10);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_10);
      (**(code **)(*(int *)pCVar3 + 0xec))(&local_28);
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,4);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,4);
      iVar5 = FUN_006d6fa0(4,&param_1);
      fVar12 = local_8;
      param_1 = (CComponent *)((float)(iVar5 + (int)local_8) + (float)param_1);
      local_54 = 0;
      local_50 = 0;
      local_4c = 0;
      local_48 = 0x3f800000;
      local_44 = 0x3f333333;
      local_40 = 0x3f333333;
      pCVar14 = (CRGBFloatColour *)&local_54;
      local_c = 0;
      local_3c = 0;
      local_8 = 1.96182e-43;
      local_38 = 0x3f800000;
      fVar20 = 20.0;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_44;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_10);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_10);
      (**(code **)(*(int *)pCVar3 + 0xec))(&local_28);
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,5);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,5);
      iVar5 = FUN_006d6fa0(5,&param_1);
      fVar12 = local_8;
      iVar5 = iVar5 + (int)local_8;
      local_c = 0;
      local_8 = 2.38221e-43;
      param_1 = (CComponent *)((float)iVar5 + (float)param_1);
      local_54 = 0;
      local_50 = 0;
      local_4c = 0;
      local_48 = 0x3f800000;
      local_44 = 0x3f333333;
      local_40 = 0x3f333333;
      local_3c = 0;
      local_38 = 0x3f800000;
      fVar20 = 20.0;
      pCVar14 = (CRGBFloatColour *)&local_54;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_44;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_10);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_10);
      (**(code **)(*(int *)pCVar3 + 0xec))(&local_28);
      local_8 = (float)CTCInventoryAbilities::GetAbilityLevel(this_03,9);
      local_10 = CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_03,9);
      iVar5 = FUN_006d6fa0(9,&param_1);
      fVar12 = local_8;
      pCVar14 = (CRGBFloatColour *)&local_54;
      param_1 = (CComponent *)((float)(iVar5 + (int)local_8) + (float)param_1);
      local_3c = 0x3f333333;
      local_c = 0;
      local_8 = 3.08286e-43;
      local_54 = 0;
      local_50 = 0;
      local_4c = 0;
      local_48 = 0x3f800000;
      local_44 = 0;
      local_40 = 0;
      local_38 = 0x3f800000;
      fVar20 = 20.0;
      bVar17 = false;
      bVar16 = false;
      fVar15 = 270.0;
      pCVar13 = (CRGBFloatColour *)&local_44;
      pCVar11 = (C2DCoordI *)&local_c;
      iVar5 = local_10;
      pCVar8 = param_1;
      pCVar7 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_18,(long)&local_18);
      pCVar8 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_14,pCVar7,pCVar11,(long)fVar12,iVar5,pCVar13,
                          (float)pCVar8,fVar15,bVar16,bVar17,pCVar14,fVar20);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_28,(vector<CCharString,std::allocator<CCharString>_> *)pCVar8);
      CCharString::~CCharString((CCharString *)&local_18);
      (**(code **)(*(int *)pCVar3 + 0xec))(&local_28);
      this_00 = local_20;
      local_8 = 270.0;
      AddHeroAbility(local_20,pCVar3,3,&local_8);
      AddHeroAbility(this_00,pCVar3,0x10,&local_8);
      AddHeroAbility(this_00,pCVar3,0xc,&local_8);
      AddHeroAbility(this_00,pCVar3,9,&local_8);
      AddHeroAbility(this_00,pCVar3,0xb,&local_8);
      AddHeroAbility(this_00,pCVar3,0x12,&local_8);
      AddHeroAbility(this_00,pCVar3,0x13,&local_8);
      local_8 = local_8 + 15.0;
      AddHeroAbility(this_00,pCVar3,1,&local_8);
      AddHeroAbility(this_00,pCVar3,5,&local_8);
      AddHeroAbility(this_00,pCVar3,2,&local_8);
      AddHeroAbility(this_00,pCVar3,6,&local_8);
      AddHeroAbility(this_00,pCVar3,10,&local_8);
      local_8 = local_8 + 15.0;
      AddHeroAbility(this_00,pCVar3,4,&local_8);
      AddHeroAbility(this_00,pCVar3,8,&local_8);
      AddHeroAbility(this_00,pCVar3,0xd,&local_8);
      AddHeroAbility(this_00,pCVar3,0xe,&local_8);
      AddHeroAbility(this_00,pCVar3,0xf,&local_8);
      AddHeroAbility(this_00,pCVar3,0x11,&local_8);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_28,extraout_EDX_02);
      uVar9 = CONCAT44(extraout_EDX_03,param_1);
    }
  }
  param_1 = (CComponent *)uVar9;
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_34
             ,(CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *)
              ((ulonglong)uVar9 >> 0x20));
  return local_1c;
}



//=== ConstructExperienceListPC @ 005d7081 ===

/* [bsim sim=0.7770517298744094 <- ego_r]
   public: virtual class NUISystem::CComponent * __thiscall
   CTCInventoryStats::ConstructExperienceListPC(class NUISystem::CComponent *) */

CComponent * __thiscall
CTCInventoryStats::ConstructExperienceListPC(CTCInventoryStats *this,CComponent *param_1)

{
  CTCInventoryStats *this_00;
  char cVar1;
  int *piVar2;
  CComponent *pCVar3;
  CRumbleDef *pCVar4;
  CAction *pCVar5;
  undefined4 *puVar6;
  int iVar7;
  CWideString *pCVar8;
  CComponent *pCVar9;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_00;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_01;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_02;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_03;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_04;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *pCVar10;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_05;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_06;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_07;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_08;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_09;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_10;
  CTCInventoryAbilities *this_01;
  uint uVar11;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar12;
  C2DCoordI *pCVar13;
  int *piVar14;
  CRGBFloatColour *pCVar15;
  float fVar16;
  float fVar17;
  CRGBFloatColour *pCVar18;
  bool bVar19;
  bool bVar20;
  float fVar21;
  CCharString CVar22;
  CCharString CVar23;
  float fVar24;
  int iStack_a4;
  undefined4 **ppuStack_a0;
  void *local_78;
  undefined4 *local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 *local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 *local_54;
  undefined4 local_50;
  undefined4 *local_4c [2];
  undefined4 *local_44 [2];
  undefined4 *local_3c [2];
  undefined4 *local_34;
  int *local_30;
  undefined4 local_2c;
  int *local_28;
  undefined4 *local_24;
  CTCInventoryStats *local_20;
  CTCInventoryAbilities *local_1c;
  CFrontEndManager *local_18;
  CTCInventoryExperience *local_14;
  CComponent *local_10;
  int *local_c;
  int *local_8;
  
  local_20 = this;
  local_18 = NUISystem::CFrontEndManager::GetInstance();
  iVar7 = *(int *)(this + 4);
  this_01 = (CTCInventoryAbilities *)0x0;
  local_1c = (CTCInventoryAbilities *)0x0;
  local_14 = (CTCInventoryExperience *)0x0;
  cVar1 = FUN_00410de0();
  if (cVar1 != '\0') {
    local_28 = (int *)0x68;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar7 + 0x44),(ETCInterfaceType *)&local_28);
    piVar14 = *(int **)(iVar7 + 0x48);
    if ((piVar2 == piVar14) || (0x68 < *piVar2)) {
      piVar2 = piVar14;
    }
    this_01 = (CTCInventoryAbilities *)piVar2[1];
    local_1c = this_01;
  }
  iVar7 = *(int *)(local_20 + 4);
  cVar1 = FUN_00410de0();
  if (cVar1 != '\0') {
    local_28 = (int *)0x16;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar7 + 0x44),(ETCInterfaceType *)&local_28);
    piVar14 = *(int **)(iVar7 + 0x48);
    if ((piVar2 == piVar14) || (0x16 < *piVar2)) {
      piVar2 = piVar14;
    }
    local_14 = (CTCInventoryExperience *)piVar2[1];
  }
  CCharString::CCharString((CCharString *)&local_8,"PC_STATS_EXP_LIST",-1);
  pCVar3 = NUISystem::CManager::CreateComponent((CManager *)local_18,(CCharString *)&local_8,false);
  local_10 = pCVar3;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,extraout_EDX,
             unaff_EDI);
  iVar7 = *(int *)pCVar3;
  CCharString::CCharString((CCharString *)&stack0xffffff78,"PC_UI_EXPERIENCE_FRAME",-1);
  local_c = (int *)(**(code **)(iVar7 + 0xc))();
  local_28 = *(int **)param_1;
  CCharString::CCharString((CCharString *)&stack0xffffff74,"SCROLL_LIST_UP",-1);
  local_28 = (int *)(*(code *)local_28[3])();
  iVar7 = *(int *)param_1;
  CCharString::CCharString((CCharString *)&stack0xffffff70,"SCROLL_LIST_DOWN",-1);
  local_24 = (undefined4 *)(**(code **)(iVar7 + 0xc))();
  pCVar4 = ::operator_new(0x10);
  if (pCVar4 == (CRumbleDef *)0x0) {
    pCVar5 = (CAction *)0x0;
  }
  else {
    pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
  }
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_4c,pCVar5);
  *local_4c[0] = 0xf5;
  FUN_0053fde0();
  puVar6 = ::operator_new(0xc);
  if (puVar6 != (undefined4 *)0x0) {
    puVar6[1] = 5;
    *puVar6 = &PTR_LAB_0122f844;
    puVar6[2] = 0x19;
  }
  FUN_0042b646();
  pCVar4 = ::operator_new(0x10);
  if (pCVar4 == (CRumbleDef *)0x0) {
    pCVar5 = (CAction *)0x0;
  }
  else {
    pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
  }
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_34,pCVar5);
  *local_34 = 0xf9;
  FUN_0053fde0();
  puVar6 = ::operator_new(0xc);
  if (puVar6 != (undefined4 *)0x0) {
    puVar6[1] = 5;
    *puVar6 = &PTR_LAB_0122f844;
    puVar6[2] = 0x19;
  }
  FUN_0042b646();
  pCVar3 = local_10;
  (**(code **)(*(int *)local_10 + 0x10c))();
  (**(code **)(*(int *)pCVar3 + 0x118))();
  ppuStack_a0 = (undefined4 **)0x5d72a4;
  pCVar4 = ::operator_new(0x10);
  if (pCVar4 == (CRumbleDef *)0x0) {
    pCVar5 = (CAction *)0x0;
  }
  else {
    pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
  }
  ppuStack_a0 = (undefined4 **)0x5d72bd;
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_3c,pCVar5);
  *local_3c[0] = 0xf5;
  uVar11 = -(uint)(local_24 != (undefined4 *)0x0) & (uint)(local_24 + 6);
  iStack_a4 = 0x5d72df;
  ppuStack_a0 = (undefined4 **)uVar11;
  FUN_0053fde0();
  ppuStack_a0 = (undefined4 **)0x5d72e6;
  puVar6 = ::operator_new(0xc);
  if (puVar6 != (undefined4 *)0x0) {
    puVar6[1] = 5;
    *puVar6 = &PTR_LAB_0122f844;
    puVar6[2] = 0x19;
  }
  ppuStack_a0 = (undefined4 **)0x5d7316;
  FUN_0042b646();
  ppuStack_a0 = (undefined4 **)0x5d731d;
  pCVar4 = ::operator_new(0x10);
  if (pCVar4 == (CRumbleDef *)0x0) {
    pCVar5 = (CAction *)0x0;
  }
  else {
    pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
  }
  ppuStack_a0 = (undefined4 **)0x5d7336;
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_44,pCVar5);
  *local_44[0] = 0xf9;
  iStack_a4 = 0x5d734c;
  ppuStack_a0 = (undefined4 **)uVar11;
  FUN_0053fde0();
  ppuStack_a0 = (undefined4 **)0x5d7353;
  puVar6 = ::operator_new(0xc);
  if (puVar6 != (undefined4 *)0x0) {
    puVar6[1] = 5;
    *puVar6 = &PTR_LAB_0122f844;
    puVar6[2] = 0x19;
  }
  ppuStack_a0 = (undefined4 **)0x5d7383;
  FUN_0042b646();
  pCVar3 = local_10;
  ppuStack_a0 = (undefined4 **)0x5d7394;
  (**(code **)(*(int *)local_10 + 0x10c))();
  ppuStack_a0 = local_3c;
  iStack_a4 = 0x5d73a2;
  (**(code **)(*(int *)pCVar3 + 0x118))();
  iStack_a4 = 0x5d73aa;
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_44
             ,extraout_EDX_00);
  iStack_a4 = 0x5d73b2;
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_3c
             ,extraout_EDX_01);
  iStack_a4 = 0x5d73ba;
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_34,extraout_EDX_02);
  iStack_a4 = 0x5d73c2;
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_4c
             ,extraout_EDX_03);
  if (local_c != (int *)0x0) {
    iVar7 = *local_c;
    CCharString::CCharString((CCharString *)&iStack_a4,"SCROLL_STATS_VIEWPORT_UP",-1);
    piVar14 = local_c;
    piVar2 = (int *)(**(code **)(iVar7 + 0xc))();
    iVar7 = *piVar14;
    CCharString::CCharString((CCharString *)&stack0xffffff58,"SCROLL_STATS_VIEWPORT_DOWN",-1);
    local_28 = (int *)(**(code **)(iVar7 + 0xc))();
    iVar7 = *(int *)local_10;
    CCharString::CCharString((CCharString *)&stack0xffffff54,"PC_STATS_EXP_VIEWPORT",-1);
    local_30 = (int *)(**(code **)(iVar7 + 0xc))();
    if (local_30 != (int *)0x0) {
      (**(code **)(*local_c + 0x1f0))();
    }
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CRumbleDef *)0x0) {
      pCVar5 = (CAction *)0x0;
    }
    else {
      pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)local_44,pCVar5);
    *local_44[0] = 0xe6;
    local_24 = ::operator_new(0xc);
    if (local_24 == (undefined4 *)0x0) {
      local_24 = (undefined4 *)0x0;
    }
    else {
      local_24[1] = 3;
      *local_24 = &PTR_LAB_012366f4;
      local_24[2] = 6;
    }
    FUN_0042b646();
    FUN_0053fde0();
    (**(code **)(*piVar2 + 0x11c))();
    (**(code **)(*piVar2 + 0x120))();
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CRumbleDef *)0x0) {
      pCVar5 = (CAction *)0x0;
    }
    else {
      pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)local_3c,pCVar5);
    *local_3c[0] = 0xe6;
    local_24 = ::operator_new(0xc);
    if (local_24 == (undefined4 *)0x0) {
      local_24 = (undefined4 *)0x0;
    }
    else {
      local_24[1] = 3;
      *local_24 = &PTR_LAB_012366f4;
      local_24[2] = 7;
    }
    FUN_0042b646();
    FUN_0053fde0();
    piVar14 = local_28;
    (**(code **)(*local_28 + 0x11c))();
    (**(code **)(*piVar14 + 0x120))();
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CRumbleDef *)0x0) {
      pCVar5 = (CAction *)0x0;
    }
    else {
      pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_74,pCVar5);
    *local_74 = 0xf5;
    FUN_0053fde0();
    puVar6 = ::operator_new(0xc);
    if (puVar6 != (undefined4 *)0x0) {
      puVar6[1] = 5;
      *puVar6 = &PTR_LAB_0122f844;
      puVar6[2] = 0x19;
    }
    FUN_0042b646();
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CRumbleDef *)0x0) {
      pCVar5 = (CAction *)0x0;
    }
    else {
      pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_64,pCVar5);
    *local_64 = 0xf9;
    FUN_0053fde0();
    puVar6 = ::operator_new(0xc);
    if (puVar6 != (undefined4 *)0x0) {
      puVar6[1] = 5;
      *puVar6 = &PTR_LAB_0122f844;
      puVar6[2] = 0x19;
    }
    FUN_0042b646();
    pCVar3 = local_10;
    (**(code **)(*(int *)local_10 + 0x10c))();
    (**(code **)(*(int *)pCVar3 + 0x118))();
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CRumbleDef *)0x0) {
      pCVar5 = (CAction *)0x0;
    }
    else {
      pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_54,pCVar5);
    *local_54 = 0xf5;
    local_28 = local_28 + 6;
    FUN_0053fde0();
    puVar6 = ::operator_new(0xc);
    if (puVar6 != (undefined4 *)0x0) {
      puVar6[1] = 5;
      *puVar6 = &PTR_LAB_0122f844;
      puVar6[2] = 0x19;
    }
    FUN_0042b646();
    pCVar4 = ::operator_new(0x10);
    if (pCVar4 == (CRumbleDef *)0x0) {
      pCVar5 = (CAction *)0x0;
    }
    else {
      pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)local_4c,pCVar5);
    *local_4c[0] = 0xf9;
    FUN_0053fde0();
    puVar6 = ::operator_new(0xc);
    if (puVar6 != (undefined4 *)0x0) {
      puVar6[1] = 5;
      *puVar6 = &PTR_LAB_0122f844;
      puVar6[2] = 0x19;
    }
    FUN_0042b646();
    (**(code **)(*(int *)pCVar3 + 0x10c))();
    (**(code **)(*(int *)pCVar3 + 0x118))();
    iVar7 = *(int *)pCVar3;
    CCharString::CCharString
              ((CCharString *)&stack0xffffff30,"PC_STATS_EXP_TICK_SLIDER_CONTAINER",-1);
    uVar12 = (**(code **)(iVar7 + 0xc))();
    pCVar10 = (CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *)
              ((ulonglong)uVar12 >> 0x20);
    iStack_a4 = (int)uVar12;
    if (iStack_a4 != 0) {
      (**(code **)(*local_30 + 0x238))();
      pCVar10 = extraout_EDX_04;
    }
    iStack_a4 = 0x5d7787;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_4c,pCVar10);
    iStack_a4 = 0x5d778f;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_54,extraout_EDX_05);
    iStack_a4 = 0x5d7797;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_64,extraout_EDX_06);
    iStack_a4 = 0x5d779f;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_74,extraout_EDX_07);
    iStack_a4 = 0x5d77a7;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_3c,extraout_EDX_08);
    iStack_a4 = 0x5d77af;
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_44,extraout_EDX_09);
    pCVar3 = local_10;
  }
  iVar7 = *(int *)pCVar3;
  CCharString::CCharString((CCharString *)&iStack_a4,"PC_STATS_EXP_SCREEN",-1);
  local_8 = (int *)(**(code **)(iVar7 + 0xc))();
  if (local_8 != (int *)0x0) {
    iVar7 = *local_8;
    CCharString::CCharString
              ((CCharString *)&stack0xffffff58,"PC_STATS_EXP_LEVEL_SLIDER_CONTAINER",-1);
    piVar14 = local_8;
    iVar7 = (**(code **)(iVar7 + 0xc))();
    if ((iVar7 != 0) &&
       (CreateExperienceSliders(local_20,(CComponent *)&local_78,iVar7,true),
       local_78 != (void *)0x0)) {
      free(local_78);
    }
    iVar7 = *piVar14;
    CCharString::CCharString
              ((CCharString *)&stack0xffffff54,"PC_STATS_EXP_TICK_SLIDER_CONTAINER",-1);
    pCVar3 = (CComponent *)(**(code **)(iVar7 + 0xc))();
    if (pCVar3 != (CComponent *)0x0) {
      if (local_c != (int *)0x0) {
        (**(code **)(*local_c + 0x1f0))();
        local_30 = (int *)*local_8;
        CCharString::CCharString
                  ((CCharString *)&stack0xffffff4c,"PC_STATS_EXP_LEVEL_SLIDER_CONTAINER",-1);
        piVar14 = local_8;
        iVar7 = (*(code *)local_30[3])();
        if (iVar7 != 0) {
          (**(code **)(*local_c + 0x1f0))();
        }
        local_30 = (int *)*piVar14;
        CCharString::CCharString((CCharString *)&stack0xffffff48,"PC_TITLE_EXPERIENCE",-1);
        piVar14 = local_8;
        iVar7 = (*(code *)local_30[3])();
        if (iVar7 != 0) {
          (**(code **)(*local_c + 0x1f0))();
        }
        iVar7 = *piVar14;
        CCharString::CCharString
                  ((CCharString *)&stack0xffffff44,"PC_RING_PIC_EXPERIENCE_INDEPENDANT",-1);
        iVar7 = (**(code **)(iVar7 + 0xc))();
        this_01 = local_1c;
        if (iVar7 != 0) {
          (**(code **)(*local_c + 0x1f0))();
          this_01 = local_1c;
        }
      }
      local_2c = 0;
      local_28 = (int *)0x0;
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,0);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,0);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      fVar16 = (float)(int)local_30;
      pCVar18 = (CRGBFloatColour *)&local_6c;
      local_5c = 0x3f333333;
      local_74 = (undefined4 *)0x0;
      local_70 = 0;
      local_6c = 0;
      local_68 = 0;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_58 = 0;
      local_54 = (undefined4 *)0x0;
      local_50 = 0x3f800000;
      fVar24 = 53.0;
      CVar23 = (CCharString)0x0;
      CVar22 = (CCharString)0x0;
      fVar21 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      fVar17 = fVar16 + 0.0;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar17,fVar21,(bool)CVar22,(bool)CVar23,pCVar18,fVar24);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,1);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,1);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      pCVar18 = (CRGBFloatColour *)&local_6c;
      fVar16 = (float)(int)local_30 + fVar16 + 0.0;
      local_5c = 0x3f333333;
      local_74 = (undefined4 *)0x0;
      local_70 = 0x1e;
      local_6c = 0;
      local_68 = 0;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_58 = 0;
      local_54 = (undefined4 *)0x0;
      local_50 = 0x3f800000;
      fVar24 = 53.0;
      CVar22 = (CCharString)0x0;
      bVar19 = false;
      fVar21 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      fVar17 = fVar16;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar17,fVar21,bVar19,(bool)CVar22,pCVar18,fVar24);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,2);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,2);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      pCVar18 = (CRGBFloatColour *)&local_6c;
      fVar16 = (float)(int)local_30 + fVar16;
      local_5c = 0x3f333333;
      local_74 = (undefined4 *)0x0;
      local_70 = 0x3c;
      local_6c = 0;
      local_68 = 0;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_58 = 0;
      local_54 = (undefined4 *)0x0;
      local_50 = 0x3f800000;
      fVar24 = 53.0;
      bVar20 = false;
      bVar19 = false;
      fVar21 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      fVar17 = fVar16;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar17,fVar21,bVar19,bVar20,pCVar18,fVar24);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,3);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,3);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      fVar16 = (float)(int)local_30 + fVar16;
      local_6c = 0;
      pCVar18 = (CRGBFloatColour *)&local_6c;
      local_68 = 0;
      local_74 = (undefined4 *)0x0;
      local_70 = 0x6e;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_5c = 0x3f333333;
      local_58 = 0x3f333333;
      local_54 = (undefined4 *)0x0;
      local_50 = 0x3f800000;
      fVar24 = 53.0;
      bVar20 = false;
      bVar19 = false;
      fVar21 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      fVar17 = fVar16;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar17,fVar21,bVar19,bVar20,pCVar18,fVar24);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,4);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,4);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      fVar16 = (float)(int)local_30 + fVar16;
      local_6c = 0;
      local_68 = 0;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_5c = 0x3f333333;
      local_58 = 0x3f333333;
      pCVar18 = (CRGBFloatColour *)&local_6c;
      local_74 = (undefined4 *)0x0;
      local_54 = (undefined4 *)0x0;
      local_70 = 0x8c;
      local_50 = 0x3f800000;
      fVar24 = 53.0;
      bVar20 = false;
      bVar19 = false;
      fVar21 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      fVar17 = fVar16;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar17,fVar21,bVar19,bVar20,pCVar18,fVar24);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,5);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,5);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      local_74 = (undefined4 *)0x0;
      local_70 = 0xaa;
      fVar16 = (float)(int)local_30 + fVar16;
      local_6c = 0;
      local_68 = 0;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_5c = 0x3f333333;
      local_58 = 0x3f333333;
      local_54 = (undefined4 *)0x0;
      local_50 = 0x3f800000;
      fVar24 = 53.0;
      pCVar18 = (CRGBFloatColour *)&local_6c;
      bVar19 = false;
      CVar22 = (CCharString)0x0;
      fVar21 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      fVar17 = fVar16;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar17,fVar21,(bool)CVar22,bVar19,pCVar18,fVar24);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      local_8 = (int *)CTCInventoryAbilities::GetAbilityLevel(this_01,9);
      local_c = (int *)CTCHeroExperience::GetTrainableStatMaxLevel((CTCHeroExperience *)this_01,9);
      iVar7 = FUN_006d6fa0();
      local_30 = (int *)(iVar7 + (int)local_8);
      pCVar18 = (CRGBFloatColour *)&local_6c;
      fVar16 = (float)(int)local_30 + fVar16;
      local_54 = (undefined4 *)0x3f333333;
      local_74 = (undefined4 *)0x0;
      local_70 = 0xdc;
      local_6c = 0;
      local_68 = 0;
      local_64 = (undefined4 *)0x0;
      local_60 = 0x3f800000;
      local_5c = 0;
      local_58 = 0;
      local_50 = 0x3f800000;
      fVar21 = 53.0;
      CVar22 = (CCharString)0x0;
      bVar19 = false;
      fVar17 = 303.0;
      pCVar15 = (CRGBFloatColour *)&local_5c;
      pCVar13 = (C2DCoordI *)&local_74;
      piVar14 = local_8;
      piVar2 = local_c;
      pCVar8 = (CWideString *)
               CTCInventoryExperience::GetExperienceAbilityName(local_14,(long)&local_30);
      pCVar9 = NUISystem::CManager::CreateTickSlider
                         ((CManager *)local_18,pCVar8,pCVar13,(long)piVar14,(long)piVar2,pCVar15,
                          fVar16,fVar17,bVar19,(bool)CVar22,pCVar18,fVar21);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_2c,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
      CCharString::~CCharString((CCharString *)&local_30);
      (**(code **)(*(int *)pCVar3 + 0xec))();
      this_00 = local_20;
      local_8 = (int *)0x43870000;
      AddHeroAbility(local_20,pCVar3,3,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0x10,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0xc,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,9,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0xb,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0x12,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0x13,(float *)&local_8);
      local_8 = (int *)((float)local_8 + 15.0);
      AddHeroAbility(this_00,pCVar3,1,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,5,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,2,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,6,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,10,(float *)&local_8);
      local_8 = (int *)((float)local_8 + 15.0);
      AddHeroAbility(this_00,pCVar3,4,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,8,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0xd,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0xe,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0xf,(float *)&local_8);
      AddHeroAbility(this_00,pCVar3,0x11,(float *)&local_8);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_2c,extraout_EDX_10);
    }
  }
  return local_10;
}



//=== GetHeroStats @ 005eb4b0 ===

/* [bsim sim=0.6025601589155708 <- ego_r]
   public: void __thiscall CTCTavernGame::GetHeroStats(class CTCHeroStats * *) */

void __thiscall CTCTavernGame::GetHeroStats(CTCTavernGame *this,CTCHeroStats **param_1)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  int *in_stack_00000008;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)this);
  if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
     ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    *in_stack_00000008 = piVar1[1];
  }
  return;
}



//=== GetAbilityMoralityCostFactor @ 005edff7 ===

/* [bsim sim=0.7872114030608321 <- ego_r]
   public: float __thiscall CTCInventoryAbilities::GetAbilityMoralityCostFactor(enum EHeroAbility)
    */

float __thiscall
CTCInventoryAbilities::GetAbilityMoralityCostFactor
          (CTCInventoryAbilities *this,EHeroAbility param_1)

{
  float fVar1;
  int iVar2;
  
  iVar2 = *(int *)(*(int *)(this + 0x160) + 4 + param_1 * 8);
  if (iVar2 == 0) {
    fVar1 = 0.0;
  }
  else {
    fVar1 = *(float *)(iVar2 + 0x90);
  }
  return fVar1;
}



//=== GetAbilityMaximumLevelWithRenown @ 005ee6bf ===

/* [bsim sim=0.747615869609801 <- ego_r]
   public: long __thiscall CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown(enum
   EHeroAbility) */

long __thiscall
CTCInventoryAbilities::GetAbilityMaximumLevelWithRenown
          (CTCInventoryAbilities *this,EHeroAbility param_1)

{
  int iVar1;
  int *piVar2;
  char cVar3;
  int *piVar4;
  CTCHeroStats *this_00;
  float fVar5;
  CTCHeroStats *local_c;
  CTCInventoryAbilities *local_8;
  
  local_c = (CTCHeroStats *)this;
  local_8 = this;
  local_8 = (CTCInventoryAbilities *)GetAbilityMaximumLevel(this,param_1);
  iVar1 = *(int *)(this + 4);
  local_c = (CTCHeroStats *)0x0;
  cVar3 = FUN_00410de0(4);
  this_00 = local_c;
  if (cVar3 != '\0') {
    local_c = (CTCHeroStats *)&DAT_00000004;
    piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&local_c);
    piVar2 = *(int **)(iVar1 + 0x48);
    if ((piVar4 == piVar2) || (4 < *piVar4)) {
      piVar4 = piVar2;
    }
    this_00 = (CTCHeroStats *)piVar4[1];
  }
  local_c = (CTCHeroStats *)CTCHeroStats::GetMoralityPercentage(this_00);
  fVar5 = GetAbilityMoralityCostFactor(this,param_1);
  if (((0.33 < (float)local_c) && (fVar5 < -0.0001)) ||
     (((float)local_c < 0.66 && (0.0001 < fVar5)))) {
    local_8 = (CTCInventoryAbilities *)((int)local_8 + -1);
  }
  return (long)local_8;
}



//=== GetExperienceCostToUpgradeAbility @ 005ee77c ===

/* [bsim sim=0.6828378914607927 <- ego_r]
   public: long __thiscall CTCInventoryAbilities::GetExperienceCostToUpgradeAbility(enum
   EHeroAbility,long,bool)const  */

long __thiscall
CTCInventoryAbilities::GetExperienceCostToUpgradeAbility
          (CTCInventoryAbilities *this,EHeroAbility param_1,long param_2,bool param_3)

{
  int iVar1;
  float fVar2;
  char cVar3;
  int *piVar4;
  long lVar5;
  int iVar6;
  int *piVar7;
  undefined3 in_stack_0000000d;
  
  if (param_1 < (uint)(*(int *)(this + 0x164) - *(int *)(this + 0x160) >> 3)) {
    iVar6 = param_1 * 8;
    if ((*(int *)(iVar6 + 4 + *(int *)(this + 0x160)) != 0) &&
       (piVar7 = (int *)(iVar6 + *(int *)(this + 0x160)), iVar1 = piVar7[1],
       *piVar7 + param_2 <= (*(int *)(iVar1 + 0x84) - *(int *)(iVar1 + 0x80) >> 2) + -1)) {
      iVar1 = *(int *)(this + 4);
      param_2 = *(long *)(*(int *)(((int *)(*(int *)(this + 0x160) + iVar6))[1] + 0x80) +
                         (*(int *)(*(int *)(this + 0x160) + iVar6) + param_2) * 4);
      cVar3 = FUN_00410de0(4);
      if (cVar3 == '\0') {
        return param_2;
      }
      param_1 = 4;
      piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar1 + 0x44),&param_1);
      piVar7 = *(int **)(iVar1 + 0x48);
      if ((piVar4 != piVar7) && (*piVar4 < 5)) {
        piVar7 = piVar4;
      }
      param_1 = *(EHeroAbility *)((CTCHeroStats *)piVar7[1] + 0x28);
      fVar2 = (float)(int)param_1;
      lVar5 = CTCHeroStats::GetMoralityMax((CTCHeroStats *)piVar7[1]);
      if (param_3 == false) {
        return param_2;
      }
      return param_2 - (int)ROUND((float)param_2 *
                                  *(float *)(*(int *)(iVar6 + 4 + *(int *)(this + 0x160)) + 0x90) *
                                  (fVar2 / (float)lVar5));
    }
  }
  return -1;
}



//=== _Move<CDefPointer<CHeroAbilityDef_const_>*,CDefPointer<class_CHeroAbilityDef_const_>*> @ 005f847f ===

/* [bsim sim=0.8516916458834998 <- ego_r]
   class CDefPointer<class CHeroAbilityDef const > * __fastcall std::_Move<class CDefPointer<class
   CHeroAbilityDef const > *,class CDefPointer<class CHeroAbilityDef const > *>(class
   CDefPointer<class CHeroAbilityDef const > *,class CDefPointer<class CHeroAbilityDef const >
   *,class CDefPointer<class CHeroAbilityDef const > *,struct std::_Nonscalar_ptr_iterator_tag) */

CDefPointer<CHeroAbilityDef_const_> * __fastcall
std::_Move<CDefPointer<CHeroAbilityDef_const_>*,CDefPointer<class_CHeroAbilityDef_const_>*>
          (int *param_1,int param_2,CDefPointer<CHeroAbilityDef_const_> *param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_2 - (int)param_1 >> 2;
  if (0 < iVar4) {
    do {
      piVar2 = *(int **)param_3;
      if (piVar2 != (int *)*param_1) {
        if (piVar2 != (int *)0x0) {
          piVar1 = piVar2 + 1;
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)(*piVar2 + 4))();
          }
          *(int *)param_3 = 0;
        }
        iVar3 = *param_1;
        *(int *)param_3 = iVar3;
        if (iVar3 != 0) {
          *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + 1;
        }
      }
      param_1 = param_1 + 1;
      param_3 = param_3 + 4;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  return param_3;
}



//=== _Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_> @ 005f8578 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall std::_Pair_base<long,class CDefPointer<class CHeroAbilityDef const >
   >::_Pair_base<long,class CDefPointer<class CHeroAbilityDef const > >(struct
   std::_Pair_base<long,class CDefPointer<class CHeroAbilityDef const > > const &) */

void __thiscall
std::_Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_>::
_Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_>
          (_Pair_base<long,CDefPointer<CHeroAbilityDef_const_>_> *this,
          _Pair_base<long,class_CDefPointer<class_CHeroAbilityDef_const_>_> *param_1)

{
  int iVar1;
  
  *(undefined4 *)this = *(undefined4 *)param_1;
  iVar1 = *(int *)(param_1 + 4);
  *(int *)(this + 4) = iVar1;
  if (iVar1 != 0) {
    *(int *)(iVar1 + 4) = *(int *)(iVar1 + 4) + 1;
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 006131e0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x28;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 10; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== GFSerialiseVectorBinaryOut<CHeroMarriageInfo> @ 00614700 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   void __fastcall GFSerialiseVectorBinaryOut<class CHeroMarriageInfo>(class CDataOutputStream
   &,class std::vector<class CHeroMarriageInfo,class std::allocator<class CHeroMarriageInfo> > &) */

void __fastcall
GFSerialiseVectorBinaryOut<CHeroMarriageInfo>
          (CDataOutputStream *param_1,
          vector<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_> *param_2)

{
  int iVar1;
  int iVar2;
  CPersistTraits<CTCInventoryMap::CUIRegionInfo> local_28 [40];
  
  iVar2 = (*(int *)(param_2 + 4) - *(int *)param_2) / 0x28;
  CDataOutputStream::WriteCBYTE(param_1,(char)iVar2);
  if (0 < iVar2) {
    iVar1 = 0;
    do {
      CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryOut
                (local_28,param_1,(CUIRegionInfo *)(*(int *)param_2 + iVar1));
      iVar1 = iVar1 + 0x28;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  return;
}



//=== GetTCDriver<CTCDExperienceOrb> @ 00617d30 ===

/* [bsim sim=0.6404608140062503 <- ego_r]
   public: bool __thiscall CThing::GetTCDriver<class CTCDExperienceOrb>(class CTCDExperienceOrb * *)
    */

bool __thiscall CThing::GetTCDriver<CTCDExperienceOrb>(CThing *this,CTCDExperienceOrb **param_1)

{
  if (((byte)this[0x2c] & 0x10) != 0) {
    *param_1 = *(CTCDExperienceOrb **)(this + 0x5c);
    return true;
  }
  return false;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00637650 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00669770 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== AddExperience @ 0066fb00 ===

/* [bsim sim=0.954288192767905 <- ego_r]
   public: void __thiscall CTCCoopSpirit::AddExperience(long) */

void __thiscall CTCCoopSpirit::AddExperience(CTCCoopSpirit *this,long param_1)

{
  *(int *)(this + 0x10) = *(int *)(this + 0x10) + param_1;
  return;
}



//=== AddExperience @ 0066fb10 ===

/* [bsim sim=0.954288192767905 <- ego_r]
   public: void __thiscall CTCCoopSpirit::AddExperience(long) */

void __thiscall CTCCoopSpirit::AddExperience(CTCCoopSpirit *this,long param_1)

{
  *(int *)(this + 0x10) = *(int *)(this + 0x10) + param_1;
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 0067e4e0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x1c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 7; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== ?GetPInventory@CInputProcessInventoryExperienceScreen@@MBEPAVCTCInventoryBase@NInventory@@XZ @ 00689ae0 ===

undefined4
_GetPInventory_CInputProcessInventoryExperienceScreen__MBEPAVCTCInventoryBase_NInventory__XZ(void)

{
  int in_ECX;
  
  return *(undefined4 *)(in_ECX + 0x34);
}



//=== ?Init@CInputProcessInventoryExperienceScreen@@UAEXXZ @ 0068b290 ===

void _Init_CInputProcessInventoryExperienceScreen__UAEXXZ(void)

{
  int extraout_EAX;
  int *piVar1;
  int in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)(in_ECX + 0x18),
             in_EDX,unaff_EDI);
  if ((*(uint *)(extraout_EAX + 0x20) & 0x400000) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x16 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    *(int *)(in_ECX + 0x34) = piVar1[1];
  }
  return;
}



//=== ?ProcessInput@CInputProcessInventoryExperienceScreen@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z @ 0068b2e0 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* [bsim sim=0.5857783180816023 <- ego_r]
   public: virtual void __thiscall CInputProcessInventoryExperienceScreen::ProcessInput(class
   CInputEvent const &,class CProcessedInput &) */

void __thiscall
CInputProcessInventoryExperienceScreen::
_ProcessInput_CInputProcessInventoryExperienceScreen__UAEXABVCInputEvent__AAVCProcessedInput___Z
          (CInputProcessInventoryExperienceScreen *this,CInputEvent *param_1,
          CProcessedInput *param_2)

{
  bool bVar1;
  char extraout_AL;
  char extraout_AL_00;
  char extraout_AL_01;
  char extraout_AL_02;
  char extraout_AL_03;
  char extraout_AL_04;
  char extraout_AL_05;
  char cVar2;
  char extraout_AL_06;
  char extraout_AL_07;
  char extraout_AL_08;
  char extraout_AL_09;
  char extraout_AL_10;
  char extraout_AL_11;
  char extraout_AL_12;
  char extraout_AL_13;
  char extraout_AL_14;
  char extraout_AL_15;
  char extraout_AL_16;
  char extraout_AL_17;
  char extraout_AL_18;
  char extraout_AL_19;
  char extraout_AL_20;
  char extraout_AL_21;
  int extraout_EAX;
  int *piVar3;
  CFrontEndManager *pCVar4;
  CIDrawEnvironment *pCVar5;
  long lVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
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
  CTCInventoryMap *pCVar7;
  float unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  float fVar8;
  CWomanToAttract *in_stack_0000000c;
  ETCInterfaceType EStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined1 uStack_24;
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  pCVar7 = (CTCInventoryMap *)0x0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)(this + 0x18),in_EDX
             ,unaff_EDI);
  if ((*(uint *)(extraout_EAX + 0x20) & 0x8000000) != 0) {
    EStack_30 = 0x1b;
    piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),&EStack_30);
    if ((piVar3 == *(int **)(extraout_EAX + 0x48)) || (0x1b < *piVar3)) {
      piVar3 = *(int **)(extraout_EAX + 0x48);
    }
    pCVar7 = (CTCInventoryMap *)piVar3[1];
  }
  bVar1 = CInputProcessGameBase::IsEventButtonHeld
                    ((CInputProcessGameBase *)this,(CInputEvent *)param_2);
  if (bVar1) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX,
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x19);
    if (extraout_AL != '\0') {
      pCVar4 = NUISystem::CFrontEndManager::GetInstance();
      (*(code *)**(undefined4 **)pCVar4)(0x1a);
      goto LAB_0068b755;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_00
               ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_0000004c);
    if (extraout_AL_00 == '\0') {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_01,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x58);
      if (extraout_AL_01 != '\0') goto LAB_0068b550;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_02,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x59);
      if (extraout_AL_02 != '\0') {
LAB_0068b570:
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        (*(code *)**(undefined4 **)pCVar4)(7);
        goto LAB_0068b755;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_03,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000025);
      if (extraout_AL_03 != '\0') {
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        (*(code *)**(undefined4 **)pCVar4)(0);
        goto LAB_0068b755;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_04,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000026);
      if (extraout_AL_04 != '\0') {
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        (*(code *)**(undefined4 **)pCVar4)(1);
        goto LAB_0068b755;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_05,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000027);
      if (extraout_AL_05 == '\0') {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX_06,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x28);
        if (extraout_AL_06 == '\0') {
          return;
        }
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        cVar2 = (**(code **)(*(int *)pCVar4 + 0x20))();
        if (cVar2 == '\0') {
          pCVar4 = NUISystem::CFrontEndManager::GetInstance();
          (*(code *)**(undefined4 **)pCVar4)(4);
        }
        goto LAB_0068b755;
      }
      pCVar4 = NUISystem::CFrontEndManager::GetInstance();
      cVar2 = (**(code **)(*(int *)pCVar4 + 0x20))();
      if (cVar2 != '\0') goto LAB_0068b755;
    }
    pCVar4 = NUISystem::CFrontEndManager::GetInstance();
    (*(code *)**(undefined4 **)pCVar4)(5);
  }
  else {
    bVar1 = CInputProcessGameBase::IsEventButtonHeld
                      ((CInputProcessGameBase *)this,(CInputEvent *)param_2);
    if (bVar1) {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_07,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x19);
      if (extraout_AL_07 == '\0') {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX_08,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_0000004c);
        if (extraout_AL_08 == '\0') {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                     extraout_EDX_09,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000048);
          if (extraout_AL_09 == '\0') {
            return;
          }
          pCVar4 = NUISystem::CFrontEndManager::GetInstance();
          cVar2 = (**(code **)(*(int *)pCVar4 + 0x20))();
          if (cVar2 != '\0') {
            return;
          }
          local_4._0_2_ = (ushort)(byte)local_4;
          uStack_28 = 0x1b;
          uStack_24 = 0xff;
          local_4 = (uint)CONCAT12(1,(ushort)local_4);
          pCVar5 = CWorld::DrawGetEnvironment(*(CWorld **)(DAT_013b86a0 + 0x1c));
          uStack_24 = SUB41(pCVar5,0);
          CMainGameComponent::AddGameEvent(DAT_013b86a0,(CGameEvent *)&uStack_28);
        }
        else {
          pCVar4 = NUISystem::CFrontEndManager::GetInstance();
          (*(code *)**(undefined4 **)pCVar4)(0x1d);
        }
      }
      else {
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        (*(code *)**(undefined4 **)pCVar4)(0x1c);
      }
      goto LAB_0068b755;
    }
    bVar1 = CInputProcessGameBase::IsEventButtonHeld
                      ((CInputProcessGameBase *)this,(CInputEvent *)param_2);
    if (!bVar1) {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_10,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000047);
      if (extraout_AL_20 == '\0') {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX_20,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x65);
        if (extraout_AL_21 == '\0') {
          return;
        }
        fVar8 = CMap::DrawGetSeaLevel((CMap *)param_2,unaff_ESI);
        ceil(fVar8 * 0.008333334);
        lVar6 = __ftol2();
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        NUISystem::CManager::ScrollMenu((CManager *)pCVar4,lVar6);
      }
      else {
        EStack_30 = *(ETCInterfaceType *)(param_2 + 0xc);
        uStack_2c = *(undefined4 *)(param_2 + 0x10);
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        *(ETCInterfaceType *)(pCVar4 + 0xb0) = EStack_30;
        *(undefined4 *)(pCVar4 + 0xb4) = uStack_2c;
        pCVar4 = NUISystem::CFrontEndManager::GetInstance();
        (*(code *)**(undefined4 **)pCVar4)(0x19);
      }
      goto LAB_0068b755;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_10
               ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x19);
    if (extraout_AL_10 != '\0') {
      pCVar4 = NUISystem::CFrontEndManager::GetInstance();
      (*(code *)**(undefined4 **)pCVar4)(0x1f);
      goto LAB_0068b755;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_11
               ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_0000004c);
    if (extraout_AL_11 != '\0') {
      pCVar4 = NUISystem::CFrontEndManager::GetInstance();
      (*(code *)**(undefined4 **)pCVar4)(0x20);
      goto LAB_0068b755;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_12
               ,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x58);
    if (extraout_AL_12 == '\0') {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_13,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x59);
      if (extraout_AL_13 == '\0') {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX_14,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x67);
        if (extraout_AL_14 == '\0') {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                     extraout_EDX_15,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000066);
          if (extraout_AL_15 == '\0') {
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                       extraout_EDX_16,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x6b);
            if (extraout_AL_16 == '\0') {
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                         extraout_EDX_17,
                         (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x6a);
              if (extraout_AL_17 == '\0') {
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                           extraout_EDX_18,
                           (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000068)
                ;
                if (extraout_AL_18 == '\0') {
                  std::
                  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                             extraout_EDX_19,
                             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x69);
                  if (extraout_AL_19 == '\0') {
                    return;
                  }
                  if (pCVar7 != (CTCInventoryMap *)0x0) {
                    CTCInventoryMap::PanMap(pCVar7,0x3f800000,0);
                  }
                }
                else if (pCVar7 != (CTCInventoryMap *)0x0) {
                  CTCInventoryMap::PanMap(pCVar7,0xbf800000,0);
                }
              }
              else if (pCVar7 != (CTCInventoryMap *)0x0) {
                CTCInventoryMap::PanMap(pCVar7,0,0x3f800000);
              }
            }
            else if (pCVar7 != (CTCInventoryMap *)0x0) {
              CTCInventoryMap::PanMap(pCVar7,0,0xbf800000);
            }
          }
          else {
            EStack_30 = 0xbf800000;
            uStack_2c = 0xbf800000;
            pCVar4 = NUISystem::CFrontEndManager::GetInstance();
            (**(code **)(*(int *)pCVar4 + 0x98))(&EStack_30);
            pCVar4 = NUISystem::CFrontEndManager::GetInstance();
            (*(code *)**(undefined4 **)pCVar4)(0xd);
          }
        }
        else {
          EStack_30 = 0x3f800000;
          uStack_2c = 0x3f800000;
          pCVar4 = NUISystem::CFrontEndManager::GetInstance();
          (**(code **)(*(int *)pCVar4 + 0x98))(&EStack_30);
          pCVar4 = NUISystem::CFrontEndManager::GetInstance();
          (*(code *)**(undefined4 **)pCVar4)(0xc);
        }
        goto LAB_0068b755;
      }
      goto LAB_0068b570;
    }
LAB_0068b550:
    pCVar4 = NUISystem::CFrontEndManager::GetInstance();
    (*(code *)**(undefined4 **)pCVar4)(6);
  }
LAB_0068b755:
  NScript::CV_SickChildScript::CWomanToAttract::Init(in_stack_0000000c);
  return;
}



//=== ?IsAffectedByHeroStats@CCreatureActionBase@@UBE_NXZ @ 00692730 ===

undefined1 _IsAffectedByHeroStats_CCreatureActionBase__UBE_NXZ(void)

{
  return 0;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 006ae1c0 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetTCDriver<CTCDExperienceOrb> @ 006ae5c0 ===

/* [bsim sim=0.8897310149776794 <- ego_r]
   public: bool __thiscall CThing::GetTCDriver<class CTCDExperienceOrb>(class CTCDExperienceOrb * *)
    */

bool __thiscall CThing::GetTCDriver<CTCDExperienceOrb>(CThing *this,CTCDExperienceOrb **param_1)

{
  if ((*(uint *)(this + 0x2c) & 0x200) != 0) {
    *param_1 = *(CTCDExperienceOrb **)(this + 0x5c);
    return true;
  }
  return false;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 006b2660 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== CTCHeroOpinionDeedLog @ 006c5a80 ===

/* [bsim sim=0.779821202762761 <- ego_r]
   public: __thiscall CTCHeroOpinionDeedLog::CTCHeroOpinionDeedLog(class CThing &) */

CTCHeroOpinionDeedLog * __thiscall
CTCHeroOpinionDeedLog::CTCHeroOpinionDeedLog(CTCHeroOpinionDeedLog *this,CThing *param_1)

{
  CThing *pCVar1;
  bool bVar2;
  undefined1 *puVar3;
  void *pvVar4;
  CDisplayEngine *pCVar5;
  CDefinitionManager *this_00;
  long lVar6;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar7;
  CDefinitionManager *this_01;
  int iVar8;
  uint extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  uint extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  int iVar9;
  CTCHeroOpinionDeedLog *pCVar10;
  undefined **ppuVar11;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int *piVar12;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar13;
  CThing *local_30;
  undefined **local_2c;
  undefined4 *local_28;
  CTCHeroOpinionDeedLog *local_24;
  CCharString local_20 [4];
  CCharString local_1c [4];
  CCharString aCStack_18 [4];
  CCharString aCStack_14 [4];
  CTCHeroOpinionDeedLog *local_10;
  int *piStack_c;
  int iStack_8;
  CTCHeroOpinionDeedLog *local_4;
  
  local_4 = this;
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0125ea44;
  *(undefined4 *)(this + 0xc) = 0;
  puVar3 = malloc(0x18);
  *(undefined1 **)(this + 0xc) = puVar3;
  *(undefined4 *)(this + 0x10) = 0;
  *puVar3 = 0;
  *(undefined4 *)(*(int *)(this + 0xc) + 4) = 0;
  *(int *)(*(int *)(this + 0xc) + 8) = *(int *)(this + 0xc);
  *(int *)(*(int *)(this + 0xc) + 0xc) = *(int *)(this + 0xc);
  *(undefined4 *)(this + 0x18) = 0;
  pvVar4 = malloc(0x24);
  *(void **)pvVar4 = pvVar4;
  *(void **)((int)pvVar4 + 4) = pvVar4;
  *(void **)(this + 0x18) = pvVar4;
  *(undefined4 *)(this + 0x1c) = 0;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x20));
  *(undefined ***)(this + 0x20) = &PTR__vector_deleting_destructor__01238c6c;
  pCVar10 = this + 0xe8;
  iVar9 = 0x5c;
  do {
    _vector_constructor_iterator_(pCVar10,4,0x12,CCharString::CCharString);
    pCVar10 = pCVar10 + 0x90;
    iVar9 = iVar9 + -1;
  } while (iVar9 != 0);
  pCVar5 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  local_24 = this + 0x130;
  local_28 = (undefined4 *)(*(int *)(pCVar5 + 0x74) + 0x5a4);
  pCVar10 = this + 0x2e;
  local_2c = &PTR_s_OPINION_DEED_EFFECTS_NONE_0137fbc0;
  do {
    ppuVar11 = local_2c;
    local_10 = pCVar10;
    CCharString::CCharString(local_20,"CTCHeroOpinionDeedLog 1",-1);
    NProgressDisplay::DisplayProgress(local_20,(float)(extraout_EDX & 0xffffff00),false,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_20,
               extraout_EDX_00,unaff_EDI);
    CCharString::CCharString(local_1c,*ppuVar11,-1);
    ppVar7 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_1c;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX_01
               ,ppVar7);
    lVar6 = CDefinitionManager::GetDefGlobalIndexFromName(this_00,(CCharString *)ppVar7);
    *(ushort *)pCVar10 = (ushort)lVar6;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_1c,
               extraout_EDX_02,unaff_EDI);
    if (ppuVar11 != &PTR_s_OPINION_DEED_EFFECTS_NONE_0137fbc0) {
      ppVar7 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)(uint)*(ushort *)pCVar10;
      ppVar13 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)&local_30;
      param_1 = (CThing *)0x0;
      local_30 = (CThing *)0x0;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,ppVar13,
                 ppVar7);
      CDefinitionManager::GetDef<CThingPhysicalSwitchDef>
                (this_01,(long)ppVar7,(CDefPointer<CThingPhysicalSwitchDef_const_> *)ppVar13);
      pCVar1 = local_30;
      if (local_30 != (CThing *)0x0) {
        iVar9 = *(int *)(local_30 + 4);
        *(int *)(local_30 + 4) = iVar9 + 1;
        *(int *)(local_30 + 4) = iVar9;
        if (iVar9 == 0) {
          (**(code **)(*(int *)local_30 + 4))();
        }
        iVar9 = *(int *)(pCVar1 + 4);
        *(int *)(pCVar1 + 4) = iVar9 + 1;
        param_1 = pCVar1;
        *(int *)(pCVar1 + 4) = iVar9;
        if (iVar9 == 0) {
          (**(code **)(*(int *)pCVar1 + 4))();
        }
      }
      if (((byte)param_1[0x7c] & 4) != 0) {
        *local_28 = 0xffffffff;
      }
      local_30 = (CThing *)0x0;
      pCVar10 = local_24;
      do {
        CCharString::operator=((CCharString *)(pCVar10 + -0x48),"");
        *(undefined4 *)pCVar10 = 0;
        iVar9 = *(int *)(param_1 + 0x54);
        iVar8 = *(int *)(param_1 + 0x58);
        bVar2 = false;
        iStack_8 = iVar8;
        if (iVar9 != iVar8) {
          do {
            piStack_c = *(int **)(iVar9 + 0x40);
            piVar12 = *(int **)(iVar9 + 0x3c);
            if (piVar12 != piStack_c) {
              do {
                CCharString::CCharString(aCStack_18,"CTCHeroOpinionDeedLog 3",-1);
                NProgressDisplay::DisplayProgress
                          (aCStack_18,(float)(extraout_EDX_03 & 0xffffff00),false,false);
                std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                           aCStack_18,extraout_EDX_04,unaff_EDI);
                if (((CThing *)*piVar12 == local_30) && (!bVar2)) {
                  CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)aCStack_14);
                  CCharString::operator=((CCharString *)(pCVar10 + -0x48),aCStack_14);
                  std::
                  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                             aCStack_14,extraout_EDX_05,unaff_EDI);
                  *(undefined4 *)pCVar10 = *(undefined4 *)(iVar9 + 0x4c);
                  bVar2 = true;
                }
                piVar12 = piVar12 + 1;
                iVar8 = iStack_8;
              } while (piVar12 != piStack_c);
            }
            iVar9 = iVar9 + 0x50;
            this = local_4;
          } while (iVar9 != iVar8);
        }
        local_30 = (CThing *)((int)local_30 + 1);
        pCVar10 = pCVar10 + 4;
      } while ((int)local_30 < 0x12);
      pCVar1 = param_1 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      ppuVar11 = local_2c;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)param_1 + 4))();
        ppuVar11 = local_2c;
      }
    }
    local_2c = ppuVar11 + 1;
    pCVar10 = local_10 + 2;
    local_28 = local_28 + 1;
    local_24 = local_24 + 0x90;
  } while ((int)local_2c < 0x137fd30);
  *(undefined4 *)(this + 0x28) = 0xffffffff;
  this[0x2c] = (CTCHeroOpinionDeedLog)0x1;
  this[0x2d] = (CTCHeroOpinionDeedLog)0x0;
  return this;
}



//=== CTCHero @ 006c9fe0 ===

/* [bsim sim=0.8209409111784911 <- ego_r]
   public: __thiscall CTCHero::CTCHero(class CThing &) */

CTCHero * __thiscall CTCHero::CTCHero(CTCHero *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0125eb54;
  this[0xc] = (CTCHero)0x0;
  this[0xd] = (CTCHero)0x0;
  this[0xe] = (CTCHero)0x0;
  this[0xf] = (CTCHero)0x0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  this[0x1c] = (CTCHero)0x0;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x1d));
  *(undefined ***)(this + 0x1d) = &PTR__vector_deleting_destructor__01238c6c;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x30));
  *(undefined ***)(this + 0x30) = &PTR__vector_deleting_destructor__01238c6c;
  return this;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 006d3e80 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== ForceSpendableExperienceTo @ 006d6470 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCHeroExperience::ForceSpendableExperienceTo(long) */

void __thiscall CTCHeroExperience::ForceSpendableExperienceTo(CTCHeroExperience *this,long param_1)

{
  if (param_1 < 0) {
    param_1 = 0;
  }
  *(long *)(this + 0x14) = param_1;
  if (*(int *)(this + 0x10) < *(int *)(this + 0x14)) {
    *(int *)(this + 0x10) = *(int *)(this + 0x14);
    return;
  }
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(this + 0x10);
  return;
}



//=== GetRoughExperienceLevel @ 006d64b0 ===

/* [bsim sim=0.9928006940536193 <- ego_r]
   public: long __thiscall CTCHeroExperience::GetRoughExperienceLevel(void)const  */

long __thiscall CTCHeroExperience::GetRoughExperienceLevel(CTCHeroExperience *this)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  
  iVar1 = *(int *)(this + 0x54);
  piVar3 = *(int **)(iVar1 + 0x28);
  uVar2 = 0;
  if (*(int *)(iVar1 + 0x2c) - (int)piVar3 >> 2 != 0) {
    do {
      if (*(int *)(this + 0x10) < *piVar3) {
        return uVar2;
      }
      uVar2 = uVar2 + 1;
      piVar3 = piVar3 + 1;
    } while (uVar2 < (uint)(*(int *)(iVar1 + 0x2c) - *(int *)(iVar1 + 0x28) >> 2));
  }
  return uVar2;
}



//=== GetExperiencePickUpMultiplier @ 006d6570 ===

/* [bsim sim=1.0 <- ego_r]
   public: long __thiscall CTCHeroExperience::GetExperiencePickUpMultiplier(void)const  */

long __thiscall CTCHeroExperience::GetExperiencePickUpMultiplier(CTCHeroExperience *this)

{
  long lVar1;
  
  lVar1 = __ftol2();
  if (lVar1 < 1) {
    lVar1 = 1;
  }
  return lVar1;
}



//=== SpendExperience @ 006d6590 ===

/* [bsim sim=0.9770815692962305 <- ego_r]
   public: void __thiscall CTCHeroExperience::SpendExperience(enum EHeroExperienceStatCategory,long)
    */

void __thiscall
CTCHeroExperience::SpendExperience
          (CTCHeroExperience *this,EHeroExperienceStatCategory param_1,long param_2)

{
  int *piVar1;
  int *piVar2;
  
  piVar1 = (int *)(*(int *)(this + 0x18) + param_1 * 4);
  *piVar1 = *piVar1 - param_2;
  piVar2 = (int *)(*(int *)(this + 0x18) + param_1 * 4);
  if (*piVar1 < 0) {
    *(int *)(this + 0x14) = *(int *)(this + 0x14) + *piVar2;
    *piVar2 = 0;
  }
  return;
}



//=== ForceTrainableStatExperienceTo @ 006d6830 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCHeroExperience::ForceTrainableStatExperienceTo(enum
   EHeroExperienceStatCategory,long) */

void __thiscall
CTCHeroExperience::ForceTrainableStatExperienceTo
          (CTCHeroExperience *this,EHeroExperienceStatCategory param_1,long param_2)

{
  if (param_2 < 0) {
    param_2 = 0;
  }
  *(long *)(*(int *)(this + 0x18) + param_1 * 4) = param_2;
  return;
}



//=== SpendExperience @ 006d6f60 ===

/* [bsim sim=0.9786393391240767 <- ego_r]
   public: void __thiscall CTCHeroExperience::SpendExperience(enum EHeroTrainableStatType,long) */

void __thiscall
CTCHeroExperience::SpendExperience
          (CTCHeroExperience *this,EHeroTrainableStatType param_1,long param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  iVar3 = *(int *)(*(int *)(*(int *)(this + 0x5c) + 0xfc) + 0x3c + param_1 * 0x50);
  piVar1 = (int *)(*(int *)(this + 0x18) + iVar3 * 4);
  *piVar1 = *piVar1 - param_2;
  piVar2 = (int *)(*(int *)(this + 0x18) + iVar3 * 4);
  if (*piVar1 < 0) {
    *(int *)(this + 0x14) = *(int *)(this + 0x14) + *piVar2;
    *piVar2 = 0;
  }
  return;
}



//=== CTCHeroExperience @ 006d7170 ===

/* [bsim sim=0.5967139201737669 <- ego_r]
   public: __thiscall CTCHeroExperience::CTCHeroExperience(class CThing &) */

CTCHeroExperience * __thiscall
CTCHeroExperience::CTCHeroExperience(CTCHeroExperience *this,CThing *param_1)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  undefined4 *puVar4;
  void *pvVar5;
  int extraout_EAX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0125f6c4;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0xc) = 1;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  puVar4 = malloc(0xc);
  *(undefined4 **)(this + 0x18) = puVar4;
  *(undefined4 **)(this + 0x20) = puVar4 + 3;
  *puVar4 = 0;
  puVar4[1] = 0;
  puVar4[2] = 0;
  *(undefined4 **)(this + 0x1c) = puVar4 + 3;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  puVar4 = malloc(0x28);
  *(undefined4 **)(this + 0x24) = puVar4;
  *(undefined4 **)(this + 0x2c) = puVar4 + 10;
  *puVar4 = 0;
  puVar4[1] = 0;
  puVar4[2] = 0;
  puVar4[3] = 0;
  puVar4[4] = 0;
  puVar4[5] = 0;
  puVar4[6] = 0;
  puVar4[7] = 0;
  puVar4[8] = 0;
  puVar4[9] = 0;
  *(undefined4 **)(this + 0x28) = puVar4 + 10;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3c] = (CTCHeroExperience)0x1;
  *(undefined4 *)(this + 0x40) = 0;
  pvVar5 = malloc(0x10);
  *(void **)pvVar5 = pvVar5;
  *(void **)((int)pvVar5 + 4) = pvVar5;
  *(void **)(this + 0x40) = pvVar5;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  puVar4 = malloc(0x28);
  *(undefined4 **)(this + 0x44) = puVar4;
  *(undefined4 **)(this + 0x4c) = puVar4 + 10;
  *puVar4 = 0;
  puVar4[1] = 0;
  puVar4[2] = 0;
  puVar4[3] = 0;
  puVar4[4] = 0;
  puVar4[5] = 0;
  puVar4[6] = 0;
  puVar4[7] = 0;
  puVar4[8] = 0;
  puVar4[9] = 0;
  *(undefined4 **)(this + 0x48) = puVar4 + 10;
  *(undefined4 *)(this + 0x50) = 0;
  pvVar5 = malloc(0x10);
  *(void **)pvVar5 = pvVar5;
  *(void **)((int)pvVar5 + 4) = pvVar5;
  *(void **)(this + 0x50) = pvVar5;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX,
             unaff_EDI);
  piVar2 = *(int **)(extraout_EAX + 0xdc);
  if (piVar2 != (int *)0x0) {
    piVar2[1] = piVar2[1] + 1;
  }
  piVar3 = *(int **)(this + 0x5c);
  if (piVar3 != piVar2) {
    if (piVar3 != (int *)0x0) {
      piVar1 = piVar3 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar3 + 4))();
      }
      *(undefined4 *)(this + 0x5c) = 0;
    }
    *(int **)(this + 0x5c) = piVar2;
    if (piVar2 == (int *)0x0) {
      return this;
    }
    piVar2[1] = piVar2[1] + 1;
  }
  if (piVar2 != (int *)0x0) {
    piVar3 = piVar2 + 1;
    *piVar3 = *piVar3 + -1;
    if (*piVar3 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
  }
  return this;
}



//=== GetWeaponModifiedExperienceMultiplierForThing @ 006d76c0 ===

/* [bsim sim=0.5084305945518799 <- ego_r]
   float __fastcall NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(class CThing
   const &) */

float __fastcall NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(CThing *param_1)

{
  int iVar1;
  int *piVar2;
  CBaseObject *pCVar3;
  CBaseObject *pCVar4;
  float fVar5;
  float local_8;
  ETCInterfaceType local_4;
  
  local_8 = 1.0;
  if ((*(uint *)(param_1 + 0x20) & 0x80000) == 0) {
    return 1.0;
  }
  local_4 = 0x13;
  piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                  ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                *)(param_1 + 0x44),&local_4);
  if ((piVar2 == *(int **)(param_1 + 0x48)) || (0x13 < *piVar2)) {
    piVar2 = *(int **)(param_1 + 0x48);
  }
  iVar1 = piVar2[1];
  pCVar3 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(iVar1 + 0x134));
  pCVar4 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(iVar1 + 0x141));
  if (((pCVar3 != (CBaseObject *)0x0) && (((byte)pCVar3[0x91] & 1) == 0)) &&
     (*(int *)(pCVar3 + 0x34) < 0)) {
    local_4 = 0xbf;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(pCVar3 + 0x44),&local_4);
    if ((piVar2 == *(int **)(pCVar3 + 0x48)) || (0xbf < *piVar2)) {
      piVar2 = *(int **)(pCVar3 + 0x48);
    }
    local_8 = CTCObjectAugmentations::GetDamageMultiplier((CTCObjectAugmentations *)piVar2[1]);
  }
  if (((pCVar4 != (CBaseObject *)0x0) && (((byte)pCVar4[0x91] & 1) == 0)) &&
     (*(int *)(pCVar4 + 0x34) < 0)) {
    local_4 = 0xbf;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(pCVar4 + 0x44),&local_4);
    if ((piVar2 == *(int **)(pCVar4 + 0x48)) || (0xbf < *piVar2)) {
      piVar2 = *(int **)(pCVar4 + 0x48);
    }
    fVar5 = CTCObjectAugmentations::GetDamageMultiplier((CTCObjectAugmentations *)piVar2[1]);
    local_8 = fVar5 * local_8;
  }
  return local_8;
}



//=== AddExperience @ 006d77c0 ===

/* [bsim sim=0.725901606457585 <- ego_r]
   public: void __thiscall CTCHeroExperience::AddExperience(long) */

void __thiscall CTCHeroExperience::AddExperience(CTCHeroExperience *this,long param_1)

{
  int iVar1;
  undefined4 *puVar2;
  bool bVar3;
  int iVar4;
  int extraout_EAX;
  int extraout_EAX_00;
  ETextAlignement EVar5;
  int *piVar6;
  int iVar7;
  CTCHeroStats *this_00;
  CTCHeroOnlineScoreboard *this_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar8;
  
  if (this[0x3c] != (CTCHeroExperience)0x0) {
    uVar8 = __ftol2();
    iVar4 = (int)uVar8;
    if (iVar4 < 1) {
      iVar4 = 1;
    }
    iVar4 = iVar4 * param_1;
    *(int *)(this + 0x10) = *(int *)(this + 0x10) + iVar4;
    *(int *)(this + 0x14) = *(int *)(this + 0x14) + iVar4;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar8 >> 0x20),unaff_EDI)
    ;
    iVar1 = *(int *)(*(int *)(extraout_EAX + 0xdc) + 0x60);
    if (iVar1 < *(int *)(this + 0x14)) {
      *(int *)(this + 0x14) = iVar1;
    }
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX,
               unaff_EDI);
    if ((((*(int *)(*(int *)(extraout_EAX_00 + 0xdc) + 100) < *(int *)(this + 0x14)) &&
         (bVar3 = CQuestManager::IsBetweenQuestStartAndFinishScreens(DAT_013b89fc), !bVar3)) &&
        (EVar5 = NUISystem::CText::GetAlignment(*(CText **)(this + 4)), (char)EVar5 == '\0')) &&
       (iVar1 = *(int *)(this + 4), (*(uint *)(iVar1 + 0x24) & 0x200) != 0)) {
      param_1 = 0x29;
      piVar6 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar1 + 0x44),(ETCInterfaceType *)&param_1);
      if ((piVar6 == *(int **)(iVar1 + 0x48)) || (0x29 < *piVar6)) {
        piVar6 = *(int **)(iVar1 + 0x48);
      }
      CTCHero::GiveTutorialForCategory((CTCHero *)piVar6[1],0x18,false);
    }
    iVar7 = __ftol2();
    iVar1 = *(int *)(this + 0x40);
    piVar6 = malloc(0x10);
    if (piVar6 + 2 != (int *)0x0) {
      piVar6[2] = iVar7;
      piVar6[3] = iVar4;
    }
    puVar2 = *(undefined4 **)(iVar1 + 4);
    *piVar6 = iVar1;
    piVar6[1] = (int)puVar2;
    *puVar2 = piVar6;
    *(int **)(iVar1 + 4) = piVar6;
    iVar1 = *(int *)(this + 4);
    this_00 = (CTCHeroStats *)param_1;
    if ((*(byte *)(iVar1 + 0x20) & 0x10) != 0) {
      param_1 = 4;
      piVar6 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar1 + 0x44),(ETCInterfaceType *)&param_1);
      if ((piVar6 == *(int **)(iVar1 + 0x48)) || (4 < *piVar6)) {
        piVar6 = *(int **)(iVar1 + 0x48);
      }
      this_00 = (CTCHeroStats *)piVar6[1];
    }
    CTCHeroStats::AddHeroStatExperience(this_00,3,iVar4);
    iVar4 = *(int *)(this + 4);
    this_01 = (CTCHeroOnlineScoreboard *)param_1;
    if ((*(uint *)(iVar4 + 0x40) & 0x2000) != 0) {
      param_1 = 0x10d;
      piVar6 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar4 + 0x44),(ETCInterfaceType *)&param_1);
      if ((piVar6 == *(int **)(iVar4 + 0x48)) || (0x10d < *piVar6)) {
        piVar6 = *(int **)(iVar4 + 0x48);
      }
      this_01 = (CTCHeroOnlineScoreboard *)piVar6[1];
    }
    CTCHeroOnlineScoreboard::UpdateScoreForActivity(this_01,3,(float)*(int *)(this + 0x10),false);
  }
  return;
}



//=== GetExperiencePickUpMultiplier @ 006d7de0 ===

/* [bsim sim=0.7056542661155119 <- ego_r]
   public: long __thiscall CTCHeroExperience::GetExperiencePickUpMultiplier(void)const  */

long __thiscall CTCHeroExperience::GetExperiencePickUpMultiplier(CTCHeroExperience *this)

{
  int iVar1;
  long lVar2;
  
  iVar1 = __ftol2();
  if (iVar1 < 1) {
    iVar1 = 1;
  }
  lVar2 = FUN_006d73d0(iVar1);
  return lVar2;
}



//=== GetExperiencePickUpMultiplier @ 006d7e10 ===

/* [bsim sim=0.5027524882283793 <- ego_r]
   public: long __thiscall CTCHeroExperience::GetExperiencePickUpMultiplier(void)const  */

long __thiscall CTCHeroExperience::GetExperiencePickUpMultiplier(CTCHeroExperience *this)

{
  int iVar1;
  undefined1 local_5;
  undefined4 local_4;
  
  iVar1 = __ftol2();
  if (iVar1 < 1) {
    iVar1 = 1;
  }
  local_4 = FUN_006d73d0(iVar1);
  iVar1 = FUN_006bcc10(&local_4,&local_5);
  return iVar1 - *(int *)(*(int *)(this + 0x54) + 0x40) >> 2;
}



//=== AddTrainableStatExperience @ 006d8300 ===

/* [bsim sim=0.8378066399936921 <- ego_r]
   public: void __thiscall CTCHeroExperience::AddTrainableStatExperience(enum
   EHeroExperienceStatCategory,long) */

void __thiscall
CTCHeroExperience::AddTrainableStatExperience
          (CTCHeroExperience *this,EHeroExperienceStatCategory param_1,long param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  
  if (this[0x3c] != (CTCHeroExperience)0x0) {
    iVar2 = __ftol2();
    if (iVar2 < 1) {
      iVar2 = 1;
    }
    NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(*(CThing **)(this + 4));
    param_2 = iVar2 * param_2;
    iVar2 = __ftol2();
    piVar3 = (int *)(param_1 * 4 + *(int *)(this + 0x18));
    *piVar3 = *piVar3 + iVar2;
    piVar3 = (int *)(*(int *)(this + 0x18) + param_1 * 4);
    if (*piVar3 < 0) {
      *piVar3 = 0;
    }
    iVar1 = *(int *)(this + 4);
    if ((*(byte *)(iVar1 + 0x20) & 0x10) != 0) {
      param_2 = 4;
      piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar1 + 0x44),(ETCInterfaceType *)&param_2);
      if ((piVar3 == *(int **)(iVar1 + 0x48)) || (4 < *piVar3)) {
        piVar3 = *(int **)(iVar1 + 0x48);
      }
      CTCHeroStats::AddHeroStatExperience((CTCHeroStats *)piVar3[1],param_1,iVar2);
    }
  }
  return;
}



//=== SetExperienceStatus @ 006d8b50 ===

/* [bsim sim=0.9137411137496629 <- ego_r]
   public: void __thiscall CTCHeroExperience::SetExperienceStatus(class std::vector<long,class
   std::allocator<long> > const &) */

void __thiscall
CTCHeroExperience::SetExperienceStatus
          (CTCHeroExperience *this,vector<long,std::allocator<long>_> *param_1)

{
  uint uVar1;
  int iVar2;
  EHeroTrainableStatType EVar3;
  
  *(undefined4 *)(this + 0x10) = **(undefined4 **)param_1;
  *(undefined4 *)(this + 0x14) = *(undefined4 *)(*(int *)param_1 + 4);
  uVar1 = 0;
  iVar2 = 2;
  if (*(int *)(this + 0x1c) - *(int *)(this + 0x18) >> 2 != 0) {
    do {
      *(undefined4 *)(*(int *)(this + 0x18) + uVar1 * 4) =
           *(undefined4 *)(*(int *)param_1 + 8 + uVar1 * 4);
      uVar1 = uVar1 + 1;
      iVar2 = iVar2 + 1;
    } while (uVar1 < (uint)(*(int *)(this + 0x1c) - *(int *)(this + 0x18) >> 2));
  }
  uVar1 = 0;
  if (*(int *)(this + 0x28) - *(int *)(this + 0x24) >> 2 != 0) {
    iVar2 = iVar2 << 2;
    do {
      *(undefined4 *)(*(int *)(this + 0x24) + uVar1 * 4) = *(undefined4 *)(iVar2 + *(int *)param_1);
      uVar1 = uVar1 + 1;
      iVar2 = iVar2 + 4;
    } while (uVar1 < (uint)(*(int *)(this + 0x28) - *(int *)(this + 0x24) >> 2));
  }
  EVar3 = 0;
  do {
    UpdateAbilitiesForStat(this,EVar3,false);
    EVar3 = EVar3 + 1;
  } while ((int)EVar3 < 10);
  OnStatLevelChange(this,false);
  return;
}



//=== ?Transfer@CHeroExperienceDef@@UAEXAAVCPersistContext@@@Z @ 006d8f10 ===

void _Transfer_CHeroExperienceDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  CMemoryDataInputStream *unaff_EBP;
  list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_> *unaff_ESI;
  CMemoryDataInputStream *unaff_EDI;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CRGBColour>
              (*(CMemoryDataInputStream **)(this + 0x24),
               (vector<CRGBColour,std::allocator<CRGBColour>_> *)(in_ECX + 0x28));
  }
  else {
    GFSerialiseVectorBinaryOut<float>
              (*(CDataOutputStream **)(this + 0x28),
               (vector<float,std::allocator<float>_> *)(in_ECX + 0x28));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CRGBColour>
              (*(CMemoryDataInputStream **)(this + 0x24),
               (vector<CRGBColour,std::allocator<CRGBColour>_> *)(in_ECX + 0x34));
  }
  else {
    GFSerialiseVectorBinaryOut<float>
              (*(CDataOutputStream **)(this + 0x28),
               (vector<float,std::allocator<float>_> *)(in_ECX + 0x34));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CRGBColour>
              (*(CMemoryDataInputStream **)(this + 0x24),
               (vector<CRGBColour,std::allocator<CRGBColour>_> *)(in_ECX + 0x40));
  }
  else {
    GFSerialiseVectorBinaryOut<float>
              (*(CDataOutputStream **)(this + 0x28),
               (vector<float,std::allocator<float>_> *)(in_ECX + 0x40));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(this + 0x24),unaff_EDI,unaff_ESI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dae40();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    FUN_006dcfa0();
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dc350();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 100));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x68));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    FUN_006dd1c0();
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dc450();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(this + 0x24),unaff_EBP,
                     (list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_> *)
                     unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dae40();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(this + 0x24),unaff_EBP,
                     (list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_> *)
                     unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dae40();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(this + 0x24),unaff_EBP,
                     (list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_> *)
                     unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dae40();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(this + 0x24),unaff_EBP,
                     (list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_> *)
                     unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dae40();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    FUN_006dd1c0();
  }
  else if (*(int *)(this + 0x18) == 3) {
    FUN_006dc450();
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) != 2) {
    GFSerialiseVectorBinaryOut<CMusicEntry>
              (*(CDataOutputStream **)(this + 0x28),
               (vector<CMusicEntry,std::allocator<CMusicEntry>_> *)(in_ECX + 0xa8));
    return;
  }
  GFSerialiseVectorBinaryIn<CBrainUpdateZone>
            (*(CMemoryDataInputStream **)(this + 0x24),
             (vector<CBrainUpdateZone,std::allocator<CBrainUpdateZone>_> *)(in_ECX + 0xa8));
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 006f0e10 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetHeroStatsInOpinionFormat @ 007009d0 ===

/* [bsim sim=0.7334188745160719 <- ego_r]
   public: float __thiscall COpinionReactionManager::GetHeroStatsInOpinionFormat(enum EOpinion)const
    */

float __thiscall
COpinionReactionManager::GetHeroStatsInOpinionFormat(COpinionReactionManager *this,EOpinion param_1)

{
  CTCHeroStats *this_00;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *extraout_EAX;
  int extraout_EAX_00;
  int *piVar1;
  long lVar2;
  double *pdVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  float in_stack_00000008;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *in_stack_fffffff0;
  float fStack_c;
  float fStack_8;
  undefined4 uStack_4;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)(this + 4),in_EDX,
             in_stack_fffffff0);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (extraout_EAX,extraout_EDX,in_stack_fffffff0);
  if ((extraout_EAX_00 != 0) && ((*(byte *)(extraout_EAX_00 + 0x20) & 0x10) != 0)) {
    fStack_c = 5.60519e-45;
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX_00 + 0x44),(ETCInterfaceType *)&fStack_c);
    if ((piVar1 == *(int **)(extraout_EAX_00 + 0x48)) || (4 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX_00 + 0x48);
    }
    this_00 = (CTCHeroStats *)piVar1[1];
    switch(in_stack_00000008) {
    case 0.0:
      in_stack_00000008 = *(float *)(this_00 + 0x28);
      fStack_8 = *(float *)(this_00 + 0xa4);
      fStack_c = (float)(int)in_stack_00000008;
      uStack_4 = 0x3f800000;
      lVar2 = CTCHeroStats::GetMoralityMax(this_00);
      in_stack_00000008 = fStack_c / (float)lVar2 + fStack_8;
      pdVar3 = GFLimitReturn<double>
                         ((double *)&stack0x00000008,(double *)&stack0x00000000,(double *)&uStack_4)
      ;
      return *(float *)pdVar3;
    case 1.4013e-45:
      return (float)*(int *)(this_00 + 0x70) / (float)*(int *)(this_00 + 0x74);
    case 2.8026e-45:
      return *(float *)(this_00 + 0xa0);
    case 4.2039e-45:
      return *(float *)(this_00 + 0x80);
    case 5.60519e-45:
      return *(float *)(this_00 + 0x9c);
    }
  }
  return 0.0;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00701c90 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== ?GetMeshesUsed@CTCHeroAttachableAppearanceModifiers@@UBEXAAV?$vector@VCMeshInfo@NGraphicAppearance@@V?$allocator@VCMeshInfo@NGraphicAppearance@@@std@@@std@@@Z @ 00706c90 ===

/* [bsim sim=0.5658694003899749 <- ego_r]
   public: void __thiscall CAIBrain::SetAllStateGroupsEnabled(bool) */

void __thiscall
CAIBrain::
_GetMeshesUsed_CTCHeroAttachableAppearanceModifiers__UBEXAAV__vector_VCMeshInfo_NGraphicAppearance__V__allocator_VCMeshInfo_NGraphicAppearance___std___std___Z
          (CAIBrain *this,bool param_1)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined3 in_stack_00000005;
  undefined4 local_c;
  undefined4 local_8;
  uint local_4;
  
  iVar2 = _param_1;
  iVar3 = *(int *)(this + 0x30);
  if (iVar3 != *(int *)(this + 0x34)) {
    do {
      local_8 = *(undefined4 *)(iVar3 + 8);
      local_c = *(undefined4 *)(iVar3 + 4);
      local_4 = local_4 & 0xfffffffe;
      puVar1 = *(undefined4 **)(iVar2 + 4);
      if (puVar1 == *(undefined4 **)(iVar2 + 8)) {
        FUN_007091a0(puVar1,&local_c,&param_1,1,1);
      }
      else {
        if (puVar1 != (undefined4 *)0x0) {
          *puVar1 = local_c;
          puVar1[1] = local_8;
          puVar1[2] = local_4;
        }
        *(int *)(iVar2 + 4) = *(int *)(iVar2 + 4) + 0xc;
      }
      iVar3 = iVar3 + 0xc;
    } while (iVar3 != *(int *)(this + 0x34));
  }
  return;
}



//=== ?OnAppearanceDraw@CTCHeroAttachableAppearanceModifiers@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z @ 00707440 ===

undefined4
_OnAppearanceDraw_CTCHeroAttachableAppearanceModifiers__UAE_NAAVCTCGraphicAppearance_NGraphicAppearance__ABVCGraphicAppearanceDrawInfo_3__Z
          (CTCGraphicAppearance *param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int *piVar5;
  int in_ECX;
  long *plVar6;
  undefined4 *_Memory;
  int iVar7;
  int *piVar8;
  int iVar9;
  undefined4 *puVar10;
  uint uVar11;
  int iStack_108;
  uint uStack_104;
  undefined1 *puStack_100;
  int iStack_fc;
  int *piStack_f0;
  undefined4 *puStack_ec;
  undefined4 *puStack_e8;
  undefined4 uStack_e4;
  undefined4 uStack_e0;
  undefined **ppuStack_dc;
  int *piStack_d8;
  int iStack_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  undefined1 uStack_c8;
  undefined1 uStack_c7;
  undefined1 uStack_c6;
  undefined1 uStack_c5;
  undefined1 uStack_c4;
  undefined1 uStack_c3;
  undefined1 uStack_c2;
  undefined1 uStack_c1;
  float fStack_c0;
  float fStack_bc;
  undefined4 uStack_b8;
  CEnginePrimitiveMorphedAnimatingMesh aCStack_b4 [128];
  int *piStack_34;
  
  fStack_bc = *(float *)(param_1 + 0x34);
  fStack_c0 = NGraphicAppearance::CTCGraphicAppearance::GetFadeDistance(param_1);
  if ((*(byte *)(in_ECX + 8) & 2) != 0) {
    if (*(int *)(in_ECX + 0xc) != *(int *)(in_ECX + 0x10)) {
      do {
        NGraphicAppearance::CTCGraphicAppearance::RemovePrimitive
                  (param_1,*(CPrimitive **)(*(int *)(in_ECX + 0x10) + -4));
        *(int *)(in_ECX + 0x10) = *(int *)(in_ECX + 0x10) + -4;
      } while (*(int *)(in_ECX + 0xc) != *(int *)(in_ECX + 0x10));
    }
    FUN_00708f50();
  }
  piVar8 = *(int **)(in_ECX + 0x30);
  if (piVar8 == *(int **)(in_ECX + 0x34)) {
    *(byte *)(in_ECX + 8) = *(byte *)(in_ECX + 8) & 0xfd;
    return 1;
  }
  iStack_108 = 0;
  do {
    iVar9 = 0;
    if (((*(byte *)(in_ECX + 8) & 2) == 0) &&
       (((iVar7 = *(int *)(param_2 + 0x1c), iVar7 == 2 || (iVar7 == 1)) || (iVar7 == 3)))) {
      NGraphicAppearance::CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos
                (param_1,*(CPrimitive **)(iStack_108 + *(int *)(in_ECX + 0xc)),
                 *(C3DAnimationInterpolator **)(param_2 + 0xc),
                 *(C3DPositionInterpolator **)(param_2 + 4),
                 *(C3DOrientationInterpolator **)(param_2 + 8));
    }
    else {
      iVar7 = *piVar8;
      puStack_ec = (undefined4 *)0x0;
      puStack_e8 = (undefined4 *)0x0;
      uStack_e4 = 0;
      if ((iVar7 != 0) && (0 < (int)(*(int *)(iVar7 + 0x2c) - *(int *)(iVar7 + 0x28) & 0xfffffff8U))
         ) {
        do {
          uStack_d0 = *(undefined4 *)(*(int *)(iVar7 + 0x28) + 4 + iVar9 * 8);
          uStack_cc = 3;
          uStack_c6 = 0;
          uStack_c7 = 0;
          uStack_c8 = 0;
          uStack_c5 = 0xff;
          uStack_c2 = 0;
          uStack_c3 = 0;
          uStack_c4 = 0;
          uStack_c1 = 0;
          CRegion::IsMapIncluded
                    ((CRegion *)&puStack_ec,*(long *)(*(int *)(iVar7 + 0x28) + iVar9 * 8));
          iVar7 = *piVar8;
          iVar9 = iVar9 + 1;
        } while (iVar9 < *(int *)(iVar7 + 0x2c) - *(int *)(iVar7 + 0x28) >> 3);
      }
      piVar5 = *(int **)(param_2 + 0x10);
      if (piVar5 != (int *)0x0) {
        iVar9 = *piVar5;
        iVar7 = piVar5[1] - iVar9 >> 0x1f;
        uStack_104 = 0;
        if ((piVar5[1] - iVar9) / 0x14 + iVar7 != iVar7) {
          iVar7 = 0;
          do {
            plVar6 = (long *)(iVar7 + iVar9);
            uVar11 = 0;
            if (plVar6[3] - plVar6[2] >> 4 != 0) {
              do {
                CRegion::IsMapIncluded((CRegion *)&puStack_ec,*plVar6);
                plVar6 = (long *)(*piVar5 + iVar7);
                uVar11 = uVar11 + 1;
              } while (uVar11 < (uint)(plVar6[3] - plVar6[2] >> 4));
            }
            iVar9 = *piVar5;
            uStack_104 = uStack_104 + 1;
            iVar7 = iVar7 + 0x14;
          } while (uStack_104 < (uint)((piVar5[1] - iVar9) / 0x14));
        }
      }
      if ((*(byte *)(in_ECX + 8) & 2) != 0) {
        iVar9 = piVar8[1];
        uStack_e0 = 2;
        NHeroInformationScreens::CBase::CBase((CBase *)&ppuStack_dc);
        ppuStack_dc = &PTR__scalar_deleting_destructor__0122f598;
        piStack_d8 = (int *)0x0;
        iStack_d4 = iVar9;
        uVar4 = FUN_004c0050();
        puVar2 = *(undefined4 **)(in_ECX + 0x10);
        if (puVar2 == *(undefined4 **)(in_ECX + 0x14)) {
          FUN_006422b0(puVar2);
        }
        else {
          if (puVar2 != (undefined4 *)0x0) {
            *puVar2 = uVar4;
          }
          *(int *)(in_ECX + 0x10) = *(int *)(in_ECX + 0x10) + 4;
        }
        ppuStack_dc = &PTR__scalar_deleting_destructor__0122f598;
        if (piStack_d8 != (int *)0x0) {
          if (piStack_d8[1] == 1) {
            (**(code **)(*piStack_d8 + 0xc))();
          }
          if ((piStack_d8 != (int *)0x0) &&
             (iVar9 = piStack_d8[1], piStack_d8[1] = iVar9 + -1, iVar9 + -1 == 0)) {
            (**(code **)(*piStack_d8 + 4))();
          }
        }
        piStack_d8 = (int *)0x0;
        NHeroInformationScreens::CBase::CBase((CBase *)&ppuStack_dc);
      }
      puStack_100 = (undefined1 *)0x0;
      puStack_100 = malloc(0x14);
      iStack_fc = 0;
      *puStack_100 = 0;
      *(undefined4 *)(puStack_100 + 4) = 0;
      *(undefined1 **)(puStack_100 + 8) = puStack_100;
      *(undefined1 **)(puStack_100 + 0xc) = puStack_100;
      uVar4 = *(undefined4 *)(*(int *)(in_ECX + -8) + 0x18);
      uVar3 = *(undefined4 *)(*(int *)(in_ECX + -8) + 0x1c);
      uStack_b8 = CONCAT31(uStack_b8._1_3_,param_1[0x5a]);
      FUN_004bd040(&stack0xfffffed0);
      CEnginePrimitiveMorphedAnimatingMesh::CEnginePrimitiveMorphedAnimatingMesh
                (aCStack_b4,uVar4,uVar3,piVar8[1],&puStack_ec,&puStack_100,
                 *(undefined4 *)(param_2 + 0xc),*(undefined4 *)(param_2 + 4),
                 *(undefined4 *)(param_2 + 8),fStack_bc * (float)piVar8[2],1,uStack_b8);
      piVar5 = (int *)FUN_004bcf50();
      if (piStack_34 != (int *)*piVar5) {
        if ((piStack_34 != (int *)0x0) &&
           (iVar9 = piStack_34[1], piStack_34[1] = iVar9 + -1, iVar9 + -1 == 0)) {
          (**(code **)(*piStack_34 + 4))();
        }
        piStack_34 = (int *)*piVar5;
        if (piStack_34 != (int *)0x0) {
          piStack_34[1] = piStack_34[1] + 1;
        }
      }
      if (piStack_f0 != (int *)0x0) {
        iVar9 = piStack_f0[1];
        piStack_f0[1] = iVar9 + -1;
        if (iVar9 + -1 == 0) {
          (**(code **)(*piStack_f0 + 4))();
        }
        piStack_f0 = (int *)0x0;
      }
      NGraphicAppearance::CTCGraphicAppearance::SetPrimitiveGraphics
                (param_1,*(CPrimitive **)(iStack_108 + *(int *)(in_ECX + 0xc)),
                 (CEnginePrimitive *)aCStack_b4,0);
      CTCAssassinRush::~CTCAssassinRush((CTCAssassinRush *)aCStack_b4);
      if (iStack_fc != 0) {
        LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray
                  ((LTextTreeWalkThrough *)&puStack_100,*(LTextBinNode **)(puStack_100 + 4));
        *(undefined1 **)(puStack_100 + 8) = puStack_100;
        *(undefined4 *)(puStack_100 + 4) = 0;
        *(undefined1 **)(puStack_100 + 0xc) = puStack_100;
        iStack_fc = 0;
      }
      if (puStack_100 != (undefined1 *)0x0) {
        free(puStack_100);
      }
      puVar2 = puStack_e8;
      _Memory = puStack_ec;
      if (puStack_ec != puStack_e8) {
        puVar10 = puStack_ec + 2;
        do {
          if ((void *)*puVar10 != (void *)0x0) {
            free((void *)*puVar10);
            _Memory = puStack_ec;
          }
          puVar1 = puVar10 + 3;
          puVar10 = puVar10 + 5;
        } while (puVar1 != puVar2);
      }
      if (_Memory != (undefined4 *)0x0) {
        free(_Memory);
      }
    }
    iStack_108 = iStack_108 + 4;
    piVar8 = piVar8 + 3;
  } while (piVar8 != *(int **)(in_ECX + 0x34));
  *(byte *)(in_ECX + 8) = *(byte *)(in_ECX + 8) & 0xfd;
  return 1;
}



//=== ?OnAppearanceClear@CTCHeroAttachableAppearanceModifiers@@UAEXXZ @ 00707970 ===

/* WARNING: Removing unreachable block (ram,0x007079bb) */
/* [bsim sim=0.7866338653492789 <- ego_r]
   public: virtual void __thiscall CTCHeroAttachableAppearanceModifiers::EditFrameUpdate(void) */

void __thiscall
CTCHeroAttachableAppearanceModifiers::
_OnAppearanceClear_CTCHeroAttachableAppearanceModifiers__UAEXXZ
          (CTCHeroAttachableAppearanceModifiers *this)

{
  CTCHeroAttachableAppearanceModifiers *this_00;
  
  this_00 = this + -0xc;
  if (((byte)this[8] & 1) != 0) {
    this[8] = (CTCHeroAttachableAppearanceModifiers)((byte)this[8] & 0xfe);
    UpdateHiddenModifiers(this_00);
    UpdateHiddenMaterials(this_00);
    UpdateGraphics(this_00);
    NGraphicAppearance::CTCGraphicAppearance::SetAsChanged
              (*(CTCGraphicAppearance **)(*(int *)(this + -8) + 100),0xf);
  }
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(this + 0xc);
  this[8] = (CTCHeroAttachableAppearanceModifiers)((byte)this[8] | 2);
  return;
}



//=== _Move<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*,CTCHeroAttachableAppearanceModifiers::CModifierGraphic*> @ 00708c20 ===

/* [bsim sim=0.8708737176318783 <- ego_r]
   class CTCHeroAttachableAppearanceModifiers::CModifierGraphic * __fastcall std::_Move<class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *>(class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,struct std::_Nonscalar_ptr_iterator_tag)
    */

CModifierGraphic * __fastcall
std::
_Move<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*,CTCHeroAttachableAppearanceModifiers::CModifierGraphic*>
          (int *param_1,int param_2,CModifierGraphic *param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = (param_2 - (int)param_1) / 0xc;
  if (iVar4 < 1) {
    return param_3;
  }
  do {
    piVar2 = *(int **)param_3;
    if (piVar2 != (int *)*param_1) {
      if (piVar2 != (int *)0x0) {
        piVar1 = piVar2 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*piVar2 + 4))();
        }
        *(int *)param_3 = 0;
      }
      iVar3 = *param_1;
      *(int *)param_3 = iVar3;
      if (iVar3 != 0) {
        *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + 1;
      }
    }
    *(int *)(param_3 + 4) = param_1[1];
    *(int *)(param_3 + 8) = param_1[2];
    param_1 = param_1 + 3;
    param_3 = param_3 + 0xc;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  return param_3;
}



//=== CTCHero @ 0071afe0 ===

/* [bsim sim=0.9266217260919907 <- ego_r]
   public: __thiscall CTCHero::CTCHero(class CThing &) */

CTCHero * __thiscall CTCHero::CTCHero(CTCHero *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01265abc;
  *(undefined4 *)(this + 0xc) = 0;
  CCharString::CCharString((CCharString *)(this + 0x10));
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 2;
  *(undefined4 *)(this + 0x20) = 8;
  this[0x24] = (CTCHero)0xff;
  this[0x25] = (CTCHero)0x1;
  this[0x26] = (CTCHero)0x1;
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x28));
  *(undefined ***)(this + 0x28) = &PTR__scalar_deleting_destructor__0122f598;
  *(undefined4 *)(this + 0x2c) = 0;
  return this;
}



//=== CTCHeroMorph @ 0071e180 ===

/* [bsim sim=0.5104973490033303 <- ego_r]
   public: __thiscall CTCHeroMorph::CTCHeroMorph(class CThing &) */

CTCHeroMorph * __thiscall CTCHeroMorph::CTCHeroMorph(CTCHeroMorph *this,CThing *param_1)

{
  CCountedPointer<CIPhysicsFacesSegmentBase_const_> *this_00;
  void *pvVar1;
  undefined1 *puVar2;
  CIPhysicsFacesSegmentBase *pCVar3;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01265cf4;
  this_00 = (CCountedPointer<CIPhysicsFacesSegmentBase_const_> *)(this + 0xc);
  *(undefined4 *)this_00 = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x14) = pvVar1;
  *(undefined4 *)(this + 0x18) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x18) = pvVar1;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  puVar2 = malloc(0x34);
  *(undefined1 **)(this + 0x28) = puVar2;
  *(undefined4 *)(this + 0x2c) = 0;
  *puVar2 = 0;
  *(undefined4 *)(*(int *)(this + 0x28) + 4) = 0;
  *(int *)(*(int *)(this + 0x28) + 8) = *(int *)(this + 0x28);
  *(int *)(*(int *)(this + 0x28) + 0xc) = *(int *)(this + 0x28);
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3c] = (CTCHeroMorph)0x0;
  this[0x3d] = (CTCHeroMorph)0x0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0x3f000000;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  this[0x60] = (CTCHeroMorph)0x0;
  pCVar3 = ::operator_new(0x18);
  if (pCVar3 != (CIPhysicsFacesSegmentBase *)0x0) {
    *(undefined4 *)pCVar3 = 0;
    *(undefined4 *)(pCVar3 + 4) = 0;
    *(undefined4 *)(pCVar3 + 8) = 0;
    *(undefined4 *)(pCVar3 + 0xc) = 0;
    *(undefined4 *)(pCVar3 + 0x10) = 0;
    *(undefined4 *)(pCVar3 + 0x14) = 0;
    CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset(this_00,pCVar3);
    return this;
  }
  CCountedPointer<CIPhysicsFacesSegmentBase_const_>::Reset(this_00,(CIPhysicsFacesSegmentBase *)0x0)
  ;
  return this;
}



//=== swap<CTCHeroMorph::CTextureLayer> @ 0071fdf0 ===

/* [bsim sim=0.8880965923552114 <- ego_r]
   void __fastcall std::swap<class CTCHeroMorph::CTextureLayer>(class CTCHeroMorph::CTextureLayer
   &,class CTCHeroMorph::CTextureLayer &) */

void __fastcall
std::swap<CTCHeroMorph::CTextureLayer>(CTextureLayer *param_1,CTextureLayer *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  uVar1 = *(undefined4 *)(param_1 + 4);
  uVar2 = *(undefined4 *)param_1;
  CCompositeBlendDesc::CCompositeBlendDesc
            ((CCompositeBlendDesc *)&local_10,(CCompositeBlendDesc *)(param_1 + 8));
  *(undefined4 *)param_1 = *(undefined4 *)param_2;
  *(undefined4 *)(param_1 + 4) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(param_1 + 8) = *(undefined4 *)(param_2 + 8);
  *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_2 + 0xc);
  *(undefined4 *)(param_1 + 0x10) = *(undefined4 *)(param_2 + 0x10);
  *(undefined4 *)(param_1 + 0x14) = *(undefined4 *)(param_2 + 0x14);
  *(undefined4 *)param_2 = uVar2;
  *(undefined4 *)(param_2 + 4) = uVar1;
  *(undefined4 *)(param_2 + 8) = local_10;
  *(undefined4 *)(param_2 + 0xc) = local_c;
  *(undefined4 *)(param_2 + 0x10) = local_8;
  *(undefined4 *)(param_2 + 0x14) = local_4;
  return;
}



//=== _Make_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,int,std::pair<long,class_CTCHeroMorph::CTextureLayer>,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_> @ 00720710 ===

/* [bsim sim=0.5415758112652092 <- donor]
   void __fastcall std::_Make_heap<struct std::pair<long,class CTCHeroMorph::CTextureLayer>
   *,int,struct std::pair<long,class CTCHeroMorph::CTextureLayer>,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer> >(struct std::pair<long,class
   CTCHeroMorph::CTextureLayer> *,struct std::pair<long,class CTCHeroMorph::CTextureLayer> *,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer>,int *,struct std::pair<long,class
   CTCHeroMorph::CTextureLayer> *) */

void __fastcall
std::
_Make_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,int,std::pair<long,class_CTCHeroMorph::CTextureLayer>,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_>
          (int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = (param_2 - param_1) / 0x18;
  if (1 < iVar1) {
    iVar3 = (iVar1 + -2) / 2;
    iVar2 = param_1 + 0x11 + iVar3 * 0x18;
    while( true ) {
      _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
                (param_1,iVar3,iVar1,*(undefined4 *)(iVar2 + -0x11),*(undefined4 *)(iVar2 + -0xd),
                 *(undefined4 *)(iVar2 + -9),*(undefined4 *)(iVar2 + -5),*(undefined1 *)(iVar2 + -1)
                 ,*(undefined1 *)(iVar2 + 3),param_3);
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      iVar2 = iVar2 + -0x18;
    }
  }
  return;
}



//=== _Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_> @ 007214a0 ===

/* [bsim sim=0.8593839224562976 <- ego_r]
   void __fastcall std::_Sort_heap<struct std::pair<long,class CTCHeroMorph::CTextureLayer> *,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer> >(struct std::pair<long,class
   CTCHeroMorph::CTextureLayer> *,struct std::pair<long,class CTCHeroMorph::CTextureLayer> *,class
   CKeyPairCompareLess<long,class CTCHeroMorph::CTextureLayer>) */

void __fastcall
std::
_Sort_heap<std::pair<long,CTCHeroMorph::CTextureLayer>*,CKeyPairCompareLess<long,CTCHeroMorph::CTextureLayer>_>
          (undefined4 *param_1,int param_2,undefined4 param_3)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  
  if (1 < (param_2 - (int)param_1) / 0x18) {
    param_2 = param_2 + -7;
    do {
      uVar3 = *(undefined4 *)(param_2 + -5);
      uVar4 = *(undefined4 *)(param_2 + -0xd);
      uVar5 = *(undefined4 *)(param_2 + -0x11);
      uVar6 = *(undefined4 *)(param_2 + -9);
      uVar1 = *(undefined1 *)(param_2 + -1);
      uVar2 = *(undefined1 *)(param_2 + 3);
      *(undefined4 *)(param_2 + -0x11) = *param_1;
      *(undefined4 *)(param_2 + -0xd) = param_1[1];
      *(undefined4 *)(param_2 + -9) = param_1[2];
      *(undefined4 *)(param_2 + -5) = param_1[3];
      *(undefined4 *)(param_2 + -1) = param_1[4];
      *(undefined4 *)(param_2 + 3) = param_1[5];
      _Adjust_heap<std::pair<EHeroMorphType,CTextureMorphEntry>*,int,std::pair<enum_EHeroMorphType,class_CTextureMorphEntry>,CKeyPairCompareLess<EHeroMorphType,CTextureMorphEntry>_>
                (param_1,0,((-0x11 - (int)param_1) + param_2) / 0x18,uVar5,uVar4,uVar6,uVar3,uVar1,
                 uVar2,param_3);
      param_2 = param_2 + -0x18;
    } while (1 < ((7 - (int)param_1) + param_2) / 0x18);
  }
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00737ff0 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetTCDriver<CTCDExperienceOrb> @ 007512c0 ===

/* [bsim sim=0.6404608140062503 <- ego_r]
   public: bool __thiscall CThing::GetTCDriver<class CTCDExperienceOrb>(class CTCDExperienceOrb * *)
    */

bool __thiscall CThing::GetTCDriver<CTCDExperienceOrb>(CThing *this,CTCDExperienceOrb **param_1)

{
  if (((byte)this[0x28] & 2) != 0) {
    *param_1 = *(CTCDExperienceOrb **)(this + 0x5c);
    return true;
  }
  return false;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00758220 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x2c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0xb; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 007588a0 ===

/* [bsim sim=0.5469729922808704 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  int *piVar5;
  int local_2c [11];
  
  while (param_1 != param_2) {
    local_2c[0] = *(int *)param_1;
    iVar2 = 10;
    piVar5 = local_2c;
    pCVar3 = param_1;
    while( true ) {
      piVar5 = piVar5 + 1;
      pCVar3 = pCVar3 + 4;
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      *piVar5 = *(int *)pCVar3;
    }
    iVar2 = *(int *)(param_1 + -0x2c);
    pCVar4 = param_1 + -0x2c;
    pCVar3 = param_1;
    while (pCVar1 = pCVar4, local_2c[0] < iVar2) {
      pCVar4 = pCVar1;
      for (iVar2 = 0xb; iVar2 != 0; iVar2 = iVar2 + -1) {
        *(int *)pCVar3 = *(int *)pCVar4;
        pCVar4 = pCVar4 + 4;
        pCVar3 = pCVar3 + 4;
      }
      pCVar4 = pCVar1 + -0x2c;
      pCVar3 = pCVar1;
      iVar2 = *(int *)(pCVar1 + -0x2c);
    }
    param_1 = param_1 + 0x2c;
    piVar5 = local_2c;
    for (iVar2 = 0xb; iVar2 != 0; iVar2 = iVar2 + -1) {
      *(int *)pCVar3 = *piVar5;
      piVar5 = piVar5 + 1;
      pCVar3 = pCVar3 + 4;
    }
  }
  return;
}



//=== CTCHeroCentre @ 0075cc60 ===

/* [bsim sim=0.9021128810576062 <- ego_r]
   public: __thiscall CTCHeroCentre::CTCHeroCentre(class CThing &) */

CTCHeroCentre * __thiscall CTCHeroCentre::CTCHeroCentre(CTCHeroCentre *this,CThing *param_1)

{
  CTCOpinionSource::CTCOpinionSource((CTCOpinionSource *)this,param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01269c84;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  this[0x80] = (CTCHeroCentre)0x0;
  this[0x81] = (CTCHeroCentre)0x0;
  this[0x82] = (CTCHeroCentre)0x0;
  return this;
}



//=== CTCHeroRangedAttacks @ 0075dd10 ===

/* [bsim sim=0.5598600404235132 <- ego_r]
   public: __thiscall CTCHeroRangedAttacks::CTCHeroRangedAttacks(class CThing &) */

CTCHeroRangedAttacks * __thiscall
CTCHeroRangedAttacks::CTCHeroRangedAttacks(CTCHeroRangedAttacks *this,CThing *param_1)

{
  void *pvVar1;
  undefined1 *puVar2;
  
  CTCOpinionSource::CTCOpinionSource((CTCOpinionSource *)this,param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126a0bc;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  pvVar1 = malloc(0xc);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x98) = pvVar1;
  CCharString::CCharString((CCharString *)(this + 0x9c));
  *(undefined4 *)(this + 0xb0) = 0;
  *(undefined4 *)(this + 0xc0) = 0;
  puVar2 = malloc(0x14);
  *(undefined1 **)(this + 0xc0) = puVar2;
  *(undefined4 *)(this + 0xc4) = 0;
  *puVar2 = 0;
  *(undefined4 *)(*(int *)(this + 0xc0) + 4) = 0;
  *(int *)(*(int *)(this + 0xc0) + 8) = *(int *)(this + 0xc0);
  *(int *)(*(int *)(this + 0xc0) + 0xc) = *(int *)(this + 0xc0);
  *(undefined4 *)(this + 0xa8) = 0;
  this[0xa0] = (CTCHeroRangedAttacks)0x0;
  *(undefined4 *)(this + 0xbc) = 0;
  *(undefined4 *)(this + 0x74) = *(undefined4 *)(this + 0x6c);
  *(undefined4 *)(this + 0xa4) = 0x3f4ccccd;
  return this;
}



//=== CTCHeroPodium @ 00771db0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall CTCHeroPodium::CTCHeroPodium(class CThing &) */

CTCHeroPodium * __thiscall CTCHeroPodium::CTCHeroPodium(CTCHeroPodium *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126a89c;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  return this;
}



//=== ~CTCDExperienceOrb @ 00779cd0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual __thiscall CTCDExperienceOrb::~CTCDExperienceOrb(void) */

void __thiscall CTCDExperienceOrb::~CTCDExperienceOrb(CTCDExperienceOrb *this)

{
  int *piVar1;
  int *piVar2;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126abfc;
  piVar2 = *(int **)(this + 0xc);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0xc) = 0;
  }
  *(undefined4 *)(this + 0xc) = 0;
  piVar2 = *(int **)(this + 0xc);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0xc) = 0;
  }
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)this);
  return;
}



//=== CombineWithExperienceOrb @ 0077a130 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   private: void __thiscall CTCDExperienceOrb::CombineWithExperienceOrb(class CThing &) */

void __thiscall CTCDExperienceOrb::CombineWithExperienceOrb(CTCDExperienceOrb *this,CThing *param_1)

{
  int iVar1;
  C3DVector *pCVar2;
  bool bVar3;
  
  iVar1 = 0;
  if ((*(uint *)(param_1 + 0x2c) & 0x200) != 0) {
    iVar1 = *(int *)(param_1 + 0x5c);
  }
  FUN_00779e10(*(int *)(iVar1 + 0x10) + *(int *)(this + 0x10));
  iVar1 = *(int *)(this + 0xc);
  bVar3 = false;
  pCVar2 = (C3DVector *)CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
  CTCDParticleEmitter::Create(*(long *)(iVar1 + 0x28),pCVar2,bVar3);
  CThing::Kill(param_1,true);
  return;
}



//=== ?Transfer@CHeroCentreDef@@UAEXAAVCPersistContext@@@Z @ 0077d3a0 ===

void _Transfer_CHeroCentreDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== CTCHeroCentre @ 0077d470 ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CTCHeroCentre::CTCHeroCentre(class CThing &) */

CTCHeroCentre * __thiscall CTCHeroCentre::CTCHeroCentre(CTCHeroCentre *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126af94;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  this[0x30] = (CTCHeroCentre)0x0;
  return this;
}



//=== CTCHero @ 0077f570 ===

/* [bsim sim=0.9227944341368364 <- ego_r]
   public: __thiscall CTCHero::CTCHero(class CThing &) */

CTCHero * __thiscall CTCHero::CTCHero(CTCHero *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126b044;
  *(undefined4 *)(this + 0xc) = 0x3f800000;
  *(undefined4 *)(this + 0x10) = 0;
  this[0x14] = (CTCHero)0x1;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x15));
  *(undefined ***)(this + 0x15) = &PTR__vector_deleting_destructor__01238c6c;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x1d));
  *(undefined ***)(this + 0x1d) = &PTR__vector_deleting_destructor__01238c6c;
  return this;
}



//=== ~CTCHeroCentreDoorMarker @ 0077f5c0 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual __thiscall CTCHeroCentreDoorMarker::~CTCHeroCentreDoorMarker(void) */

void __thiscall CTCHeroCentreDoorMarker::~CTCHeroCentreDoorMarker(CTCHeroCentreDoorMarker *this)

{
  CBaseIntelligentPointer *this_00;
  CBaseObject *pCVar1;
  bool bVar2;
  undefined **local_8 [2];
  
  this_00 = (CBaseIntelligentPointer *)(this + 0x15);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126b044;
  pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
  if (pCVar1 != (CBaseObject *)0x0) {
    pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
    if (((byte)pCVar1[0x91] & 1) == 0) {
      bVar2 = true;
      pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
      CThing::Kill((CThing *)pCVar1,bVar2);
      CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)local_8);
      local_8[0] = &PTR__vector_deleting_destructor__01238c6c;
      CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)local_8,(CBaseObject *)0x0);
      pCVar1 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_8);
      CBaseIntelligentPointer::SetPItem(this_00,pCVar1);
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)local_8);
    }
  }
  CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x1d));
  CBaseIntelligentPointer::~CBaseIntelligentPointer(this_00);
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)this);
  return;
}



//=== UpdateParticleExperience @ 0077f7e0 ===

/* [bsim sim=0.9833003447638061 <- ego_r]
   public: void __thiscall CTCHeroCentreDoorMarker::UpdateParticleExperience(void) */

void __thiscall CTCHeroCentreDoorMarker::UpdateParticleExperience(CTCHeroCentreDoorMarker *this)

{
  CBaseIntelligentPointer *this_00;
  CBaseObject *pCVar1;
  CDisplayEngine *pCVar2;
  C3DVector *pCVar3;
  CThing *pCVar4;
  int *piVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CTCDParticleEmitter *this_01;
  bool bVar6;
  undefined **local_10 [2];
  undefined **local_8 [2];
  
  this_00 = (CBaseIntelligentPointer *)(this + 0x15);
  pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
  if ((pCVar1 == (CBaseObject *)0x0) ||
     (pCVar1 = CBaseIntelligentPointer::GetPItem(this_00), ((byte)pCVar1[0x91] & 1) != 0)) {
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    if (pCVar2[0xd7] != (CDisplayEngine)0x0) {
      CCharString::CCharString((CCharString *)local_10,"MARKTELEPORTER",-1);
      bVar6 = false;
      pCVar3 = (C3DVector *)CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
      pCVar4 = CTCDParticleEmitter::Create((CCharString *)local_10,pCVar3,bVar6);
      CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)local_8);
      local_8[0] = &PTR__vector_deleting_destructor__01238c6c;
      CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)local_8,(CBaseObject *)pCVar4);
      pCVar1 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_8);
      CBaseIntelligentPointer::SetPItem(this_00,pCVar1);
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)local_8);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,
                 extraout_EDX,unaff_EDI);
      pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
      if ((pCVar1 != (CBaseObject *)0x0) &&
         (pCVar1 = CBaseIntelligentPointer::GetPItem(this_00), ((byte)pCVar1[0x91] & 1) == 0)) {
        this_01 = (CTCDParticleEmitter *)0x0;
        pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
        if ((*(uint *)(pCVar1 + 0x28) & 0x800) != 0) {
          local_10[0] = (undefined **)0x4b;
          piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                          ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                        *)(pCVar1 + 0x44),(ETCInterfaceType *)local_10);
          if ((piVar5 == *(int **)(pCVar1 + 0x48)) || (0x4b < *piVar5)) {
            piVar5 = *(int **)(pCVar1 + 0x48);
          }
          this_01 = (CTCDParticleEmitter *)piVar5[1];
        }
        CCharString::CCharString((CCharString *)local_10,"",-1);
        CTCDParticleEmitter::AttachToThing
                  (this_01,*(CThing **)(this + 4),0x20,(CCharString *)local_10,0,0.0);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,
                   extraout_EDX_00,unaff_EDI);
      }
    }
  }
  else {
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    if (pCVar2[0xd7] == (CDisplayEngine)0x0) {
      bVar6 = true;
      pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
      CThing::Kill((CThing *)pCVar1,bVar6);
      CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)local_10);
      local_10[0] = &PTR__vector_deleting_destructor__01238c6c;
      CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)local_10,(CBaseObject *)0x0);
      pCVar1 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_10);
      CBaseIntelligentPointer::SetPItem(this_00,pCVar1);
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)local_10);
      return;
    }
  }
  return;
}



//=== UpdateParticleExperience @ 0077f970 ===

/* [bsim sim=0.9833003447638061 <- ego_r]
   public: void __thiscall CTCHeroCentreDoorMarker::UpdateParticleExperience(void) */

void __thiscall CTCHeroCentreDoorMarker::UpdateParticleExperience(CTCHeroCentreDoorMarker *this)

{
  CBaseIntelligentPointer *this_00;
  CBaseObject *pCVar1;
  CDisplayEngine *pCVar2;
  C3DVector *pCVar3;
  CThing *pCVar4;
  int *piVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CTCDParticleEmitter *this_01;
  bool bVar6;
  undefined **local_10 [2];
  undefined **local_8 [2];
  
  this_00 = (CBaseIntelligentPointer *)(this + 0x15);
  pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
  if ((pCVar1 == (CBaseObject *)0x0) ||
     (pCVar1 = CBaseIntelligentPointer::GetPItem(this_00), ((byte)pCVar1[0x91] & 1) != 0)) {
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    if (pCVar2[0xd6] != (CDisplayEngine)0x0) {
      CCharString::CCharString((CCharString *)local_10,"EXPERIENCE_SPEND",-1);
      bVar6 = false;
      pCVar3 = (C3DVector *)CVertexBufferWin32::DoSizeof(*(CVertexBufferWin32 **)(this + 4));
      pCVar4 = CTCDParticleEmitter::Create((CCharString *)local_10,pCVar3,bVar6);
      CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)local_8);
      local_8[0] = &PTR__vector_deleting_destructor__01238c6c;
      CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)local_8,(CBaseObject *)pCVar4);
      pCVar1 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_8);
      CBaseIntelligentPointer::SetPItem(this_00,pCVar1);
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)local_8);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,
                 extraout_EDX,unaff_EDI);
      pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
      if ((pCVar1 != (CBaseObject *)0x0) &&
         (pCVar1 = CBaseIntelligentPointer::GetPItem(this_00), ((byte)pCVar1[0x91] & 1) == 0)) {
        this_01 = (CTCDParticleEmitter *)0x0;
        pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
        if ((*(uint *)(pCVar1 + 0x28) & 0x800) != 0) {
          local_10[0] = (undefined **)0x4b;
          piVar5 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                          ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                        *)(pCVar1 + 0x44),(ETCInterfaceType *)local_10);
          if ((piVar5 == *(int **)(pCVar1 + 0x48)) || (0x4b < *piVar5)) {
            piVar5 = *(int **)(pCVar1 + 0x48);
          }
          this_01 = (CTCDParticleEmitter *)piVar5[1];
        }
        CCharString::CCharString((CCharString *)local_10,"",-1);
        CTCDParticleEmitter::AttachToThing
                  (this_01,*(CThing **)(this + 4),0x20,(CCharString *)local_10,0,0.0);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,
                   extraout_EDX_00,unaff_EDI);
      }
    }
  }
  else {
    pCVar2 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
    if (pCVar2[0xd6] == (CDisplayEngine)0x0) {
      bVar6 = true;
      pCVar1 = CBaseIntelligentPointer::GetPItem(this_00);
      CThing::Kill((CThing *)pCVar1,bVar6);
      CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)local_10);
      local_10[0] = &PTR__vector_deleting_destructor__01238c6c;
      CBaseIntelligentPointer::SetPItem((CBaseIntelligentPointer *)local_10,(CBaseObject *)0x0);
      pCVar1 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)local_10);
      CBaseIntelligentPointer::SetPItem(this_00,pCVar1);
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)local_10);
      return;
    }
  }
  return;
}



//=== CTCHeroCentre @ 00780820 ===

/* [bsim sim=0.8683903433051924 <- ego_r]
   public: __thiscall CTCHeroCentre::CTCHeroCentre(class CThing &) */

CTCHeroCentre * __thiscall CTCHeroCentre::CTCHeroCentre(CTCHeroCentre *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126b1a4;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  this[0x1c] = (CTCHeroCentre)0x0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  this[0x30] = (CTCHeroCentre)0x0;
  this[0x31] = (CTCHeroCentre)0x0;
  return this;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00792350 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 007923c0 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== ~CHeroMorphDef @ 007acac0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual __thiscall CHeroMorphDef::~CHeroMorphDef(void) */

void __thiscall CHeroMorphDef::~CHeroMorphDef(CHeroMorphDef *this)

{
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x90),in_EDX,
             unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x8c),
             extraout_EDX,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 0x6c),
             extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(this + 100),
             extraout_EDX_01,unaff_ESI);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01230ba0;
  CSimpleMemoryPool::CMemoryBlock::~CMemoryBlock((CMemoryBlock *)this);
  return;
}



//=== CTCHeroRangedAttacks @ 007b2db0 ===

/* [bsim sim=0.8615588855599937 <- ego_r]
   public: __thiscall CTCHeroRangedAttacks::CTCHeroRangedAttacks(class CThing &) */

CTCHeroRangedAttacks * __thiscall
CTCHeroRangedAttacks::CTCHeroRangedAttacks(CTCHeroRangedAttacks *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126d80c;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  FUN_007b4420(0);
  *(undefined4 *)(this + 0x48) = 0xf3333334;
  *(undefined4 *)(this + 0x4c) = 0xf3333334;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  this[0x50] = (CTCHeroRangedAttacks)0x0;
  *(undefined4 *)(this + 0x40) = 0xc0000000;
  return this;
}



//=== CTCHeroCentre @ 007bae00 ===

/* [bsim sim=0.6345339745028583 <- ego_r]
   public: __thiscall CTCHeroCentre::CTCHeroCentre(class CThing &) */

CTCHeroCentre * __thiscall CTCHeroCentre::CTCHeroCentre(CTCHeroCentre *this,CThing *param_1)

{
  void *pvVar1;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126de14;
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x2d));
  *(undefined ***)(this + 0x2d) = &PTR__vector_deleting_destructor__01238c6c;
  *(undefined4 *)(this + 0x38) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x38) = pvVar1;
  *(undefined4 *)(this + 0x10) = 0xffffffff;
  *(undefined4 *)(this + 0x14) = 0xffffffff;
  *(undefined4 *)(this + 0x18) = 0xffffffff;
  *(undefined4 *)(this + 0xc) = 0xffffffff;
  *(undefined4 *)(this + 0x20) = 0xffffffff;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  this[0x2c] = (CTCHeroCentre)0x0;
  return this;
}



//=== ~CTCHeroReceiveItems @ 007ca8e0 ===

/* WARNING: Removing unreachable block (ram,0x007ca930) */
/* WARNING: Removing unreachable block (ram,0x007ca935) */
/* WARNING: Removing unreachable block (ram,0x007ca93a) */
/* WARNING: Removing unreachable block (ram,0x007ca982) */
/* WARNING: Removing unreachable block (ram,0x007ca987) */
/* WARNING: Removing unreachable block (ram,0x007ca98c) */
/* [bsim sim=0.8563112814027996 <- ego_r]
   public: virtual __thiscall CTCHeroReceiveItems::~CTCHeroReceiveItems(void) */

void __thiscall CTCHeroReceiveItems::~CTCHeroReceiveItems(CTCHeroReceiveItems *this)

{
  int *piVar1;
  CBase *pCVar2;
  int *piVar3;
  void *_Memory;
  list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *unaff_EDI;
  
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126ebfc;
  CTCBase::RemoveFromGlobalTCs((CTCBase *)this);
  pCVar2 = (CBase *)(this + 0xa4);
  *(undefined ***)pCVar2 = &PTR__scalar_deleting_destructor__0122f598;
  piVar3 = *(int **)(this + 0xa8);
  if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
    (**(code **)(*piVar3 + 0xc))(pCVar2);
  }
  piVar3 = *(int **)(this + 0xa8);
  if (piVar3 != (int *)0x0) {
    piVar1 = piVar3 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar3 + 4))();
    }
    *(undefined4 *)(this + 0xa8) = 0;
  }
  *(undefined4 *)(this + 0xa8) = 0;
  NHeroInformationScreens::CBase::CBase(pCVar2);
  pCVar2 = (CBase *)(this + 0x9c);
  *(undefined ***)pCVar2 = &PTR__scalar_deleting_destructor__0122f598;
  piVar3 = *(int **)(this + 0xa0);
  if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
    (**(code **)(*piVar3 + 0xc))(pCVar2);
  }
  piVar3 = *(int **)(this + 0xa0);
  if (piVar3 != (int *)0x0) {
    piVar1 = piVar3 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar3 + 4))();
    }
    *(undefined4 *)(this + 0xa0) = 0;
  }
  *(undefined4 *)(this + 0xa0) = 0;
  NHeroInformationScreens::CBase::CBase(pCVar2);
  CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x29));
  CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)(this + 0x1c));
  if (*(void **)(this + 0x10) != (void *)0x0) {
    free(*(void **)(this + 0x10));
  }
  CGuiControlTreePane::SortTreeRecursively((CGuiControlTreePane *)(this + 0xc),unaff_EDI);
  _Memory = *(void **)(this + 0xc);
  if (_Memory != (void *)0x0) {
    free(_Memory);
  }
  CChunkedFileChunk::~CChunkedFileChunk((CChunkedFileChunk *)this);
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00819280 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x28;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 10; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00819360 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x24;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 9; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 0081a940 ===

/* [bsim sim=0.5469729922808704 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  int *piVar5;
  int local_28 [10];
  
  while (param_1 != param_2) {
    local_28[0] = *(int *)param_1;
    iVar2 = 9;
    piVar5 = local_28;
    pCVar3 = param_1;
    while( true ) {
      piVar5 = piVar5 + 1;
      pCVar3 = pCVar3 + 4;
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      *piVar5 = *(int *)pCVar3;
    }
    iVar2 = *(int *)(param_1 + -0x28);
    pCVar4 = param_1 + -0x28;
    pCVar3 = param_1;
    while (pCVar1 = pCVar4, local_28[0] < iVar2) {
      pCVar4 = pCVar1;
      for (iVar2 = 10; iVar2 != 0; iVar2 = iVar2 + -1) {
        *(int *)pCVar3 = *(int *)pCVar4;
        pCVar4 = pCVar4 + 4;
        pCVar3 = pCVar3 + 4;
      }
      pCVar4 = pCVar1 + -0x28;
      pCVar3 = pCVar1;
      iVar2 = *(int *)(pCVar1 + -0x28);
    }
    param_1 = param_1 + 0x28;
    piVar5 = local_28;
    for (iVar2 = 10; iVar2 != 0; iVar2 = iVar2 + -1) {
      *(int *)pCVar3 = *piVar5;
      piVar5 = piVar5 + 1;
      pCVar3 = pCVar3 + 4;
    }
  }
  return;
}



//=== CHeroMorphDef @ 0082a460 ===

/* [bsim sim=0.6350208300458788 <- ego_r]
   public: __thiscall CHeroMorphDef::CHeroMorphDef(void) */

CHeroMorphDef * __thiscall CHeroMorphDef::CHeroMorphDef(CHeroMorphDef *this)

{
  void *pvVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01231768;
  std::
  map<long,CEnginePrimitiveHandle,std::less<long>,std::allocator<std::pair<long_const_,CEnginePrimitiveHandle>_>_>
  ::operator[]((map<long,CEnginePrimitiveHandle,std::less<long>,std::allocator<std::pair<long_const_,CEnginePrimitiveHandle>_>_>
                *)(this + 4),(long *)0x1);
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  pvVar1 = malloc(0x18);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x20) = pvVar1;
  this[0x24] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  this[0x31] = (CHeroMorphDef)0x0;
  this[0x30] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x3c) = 0xff000000;
  this[0x40] = (CHeroMorphDef)0x0;
  this[0x41] = (CHeroMorphDef)0x0;
  this[0x42] = (CHeroMorphDef)0x0;
  this[0x43] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  CRegionMinimap::CRegionMinimap((CRegionMinimap *)(this + 0x54));
  *(undefined4 *)(this + 0xb8) = 0;
  this[0xbc] = (CHeroMorphDef)0x0;
  return this;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 0083b000 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetRoughExperienceLevel @ 00856f00 ===

/* [bsim sim=0.5296678700634876 <- ego_r]
   public: long __thiscall CTCHeroExperience::GetRoughExperienceLevel(void)const  */

long __thiscall CTCHeroExperience::GetRoughExperienceLevel(CTCHeroExperience *this)

{
  int iVar1;
  uint3 uVar2;
  uint uVar3;
  int *piVar4;
  int in_stack_00000004;
  undefined1 local_c [12];
  
  iVar1 = std::
          _For_each<std::_List_unchecked_const_iterator<std::_List_val<CCountedPointer<CCombatActionBase::CStrikeEvent>,std::allocator<CCountedPointer<CCombatActionBase::CStrikeEvent>_>_>,std::_Iterator_base0>,_anon_AD05D24C::CGetActiveStrikeEventThatIsFirstToFinish>
                    (local_c,0,**(undefined4 **)(this + 0xbc),*(undefined4 **)(this + 0xbc),
                     *(undefined4 *)(this + 0x1c),0,1000000);
  iVar1 = *(int *)(iVar1 + 4);
  uVar2 = (uint3)((uint)iVar1 >> 8);
  if (iVar1 != 0) {
    piVar4 = *(int **)(iVar1 + 0x1c);
    uVar3 = 0;
    if (*(int *)(iVar1 + 0x20) - (int)piVar4 >> 2 != 0) {
      do {
        if (*piVar4 == in_stack_00000004) {
          return CONCAT31(uVar2,1);
        }
        uVar3 = uVar3 + 1;
        piVar4 = piVar4 + 1;
      } while (uVar3 < (uint)(*(int *)(iVar1 + 0x20) - *(int *)(iVar1 + 0x1c) >> 2));
    }
  }
  return (uint)uVar2 << 8;
}



//=== _Move<CHeroLogBook::CQuestObjectiveDesc*,CHeroLogBook::CQuestObjectiveDesc*> @ 0086d5a0 ===

/* [bsim sim=0.5681008299568104 <- ego_r]
   class CHeroLogBook::CQuestObjectiveDesc * __fastcall std::_Move<class
   CHeroLogBook::CQuestObjectiveDesc *,class CHeroLogBook::CQuestObjectiveDesc *>(class
   CHeroLogBook::CQuestObjectiveDesc *,class CHeroLogBook::CQuestObjectiveDesc *,class
   CHeroLogBook::CQuestObjectiveDesc *,struct std::_Nonscalar_ptr_iterator_tag) */

CQuestObjectiveDesc * __fastcall
std::_Move<CHeroLogBook::CQuestObjectiveDesc*,CHeroLogBook::CQuestObjectiveDesc*>
          (int param_1,int param_2,CQuestObjectiveDesc *param_3)

{
  int iVar1;
  CQuestObjectiveDesc *pCVar2;
  
  iVar1 = param_2 - param_1 >> 4;
  if (iVar1 < 1) {
    return param_3;
  }
  do {
    pCVar2 = param_3 + -0x10;
    FUN_006741e0(param_2 + -0x10);
    iVar1 = iVar1 + -1;
    param_3[-3] = *(CQuestObjectiveDesc *)(param_2 + -3);
    param_2 = param_2 + -0x10;
    param_3 = pCVar2;
  } while (iVar1 != 0);
  return pCVar2;
}



//=== ?SetExperienceSpendingAsEnabled@CGameScriptInterface@@UBEX_N@Z @ 0088f460 ===

void _SetExperienceSpendingAsEnabled_CGameScriptInterface__UBEX_N_Z(undefined1 param_1)

{
  int in_ECX;
  
  *(undefined1 *)(*(int *)(in_ECX + 4) + 0xd6) = param_1;
  return;
}



//=== ?AddLogBookEntry@CGameScriptInterface@@UBEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@Z @ 0088fe00 ===

void _AddLogBookEntry_CGameScriptInterface__UBEXABVCWideString__00W4ECategory_CHeroLogBookEntry_CHeroLogBook___Z
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  Catch_0040d959();
  FUN_0047a9f0(param_1,param_2,param_3,param_4,1);
  return;
}



//=== ?SetQuestCardRenownReward@CGameScriptInterface@@UBEXABVCCharString@@J@Z @ 00891500 ===

void _SetQuestCardRenownReward_CGameScriptInterface__UBEXABVCCharString__J_Z
               (CCharString *param_1,long param_2)

{
  CQuestManager::SetQuestCardGoldReward(DAT_013b89fc,param_1,param_2);
  return;
}



//=== ?MsgOnLeavingExperienceSpendingScreen@CGameScriptInterface@@UBE_NXZ @ 00893ab0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual bool __thiscall CGameScriptInterface::MsgIsActionModeButtonPressed(void)const  */

bool __thiscall
CGameScriptInterface::_MsgOnLeavingExperienceSpendingScreen_CGameScriptInterface__UBE_NXZ
          (CGameScriptInterface *this)

{
  long lVar1;
  CMessageEvent *pCVar2;
  undefined4 local_14;
  long local_10;
  long local_c;
  undefined4 *local_8;
  long *local_4;
  
  lVar1 = GetMaxWorldFrameForMessages(this);
  local_c = GetMaxWorldFrameForMessages(this);
  local_8 = &local_14;
  local_4 = &local_10;
  local_14 = 0x4a;
  local_10 = lVar1;
  pCVar2 = CMessageEventManager::
           FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>_>
                     (*(CMessageEventManager **)(*(int *)(this + 4) + 0x60),
                      (CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>
                       *)&local_8);
  return pCVar2 != (CMessageEvent *)0x0;
}



//=== ?SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden@CGameScriptInterface@@UBEX_N@Z @ 00894ec0 ===

/* [bsim sim=0.6822830737192805 <- ego_r]
   public: virtual void __thiscall
   CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool)const  */

void __thiscall
CGameScriptInterface::
_SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden_CGameScriptInterface__UBEX_N_Z
          (CGameScriptInterface *this,bool param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  undefined4 *puVar3;
  bool bVar4;
  undefined4 *_Memory;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined4 *local_c;
  CCharString local_8 [4];
  undefined4 local_4;
  
  _Memory = malloc(0xc);
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  local_c = _Memory;
  CThingSearchTools::
  GetAllThings<CThingAICreature,std::vector<CThingAICreature*,std::allocator<CThingAICreature*>_>,CThingFilter_None>
            (*(CThingSearchTools **)(*(int *)(this + 4) + 0x20),(CThingFilter_None *)&param_1,
             (vector<CThingAICreature*,std::allocator<CThingAICreature*>_> *)&local_c);
  local_4 = 0x69;
  CThingSearchTools::
  GetAllThings<CThing,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>_>
            (*(CThingSearchTools **)(*(int *)(this + 4) + 0x20),
             (CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook> *)&local_4,
             (vector<CThing*,std::allocator<CThing*>_> *)&local_c);
  CCharString::CCharString((CCharString *)&local_4,"OBJECT_CHEST_REWARD_ON_DEATH",-1);
  CCharString::CCharString(local_8,(CCharString *)&local_4);
  CThingSearchTools::
  GetAllThings<CThingMarker,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasScriptName>
            (*(CThingSearchTools **)(*(int *)(this + 4) + 0x20),
             (CThingFilter_HasScriptName *)local_8,
             (vector<CThing*,std::allocator<CThing*>_> *)&local_c);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,extraout_EDX,
             unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
             extraout_EDX_00,unaff_EDI);
  CCharString::CCharString(local_8,"OBJECT_GOLDBAG_SMALL_WITH_COINS",-1);
  CCharString::CCharString((CCharString *)&local_4,local_8);
  CThingSearchTools::
  GetAllThings<CThingMarker,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasScriptName>
            (*(CThingSearchTools **)(*(int *)(this + 4) + 0x20),
             (CThingFilter_HasScriptName *)&local_4,
             (vector<CThing*,std::allocator<CThing*>_> *)&local_c);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_4,
             extraout_EDX_01,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,
             extraout_EDX_02,unaff_EDI);
  local_4 = 0xcc;
  CThingSearchTools::
  GetAllThings<CThingMarker,std::vector<CThing*,std::allocator<CThing*>_>,CThingFilter_HasScriptName>
            (*(CThingSearchTools **)(*(int *)(this + 4) + 0x20),
             (CThingFilter_HasScriptName *)&local_4,
             (vector<CThing*,std::allocator<CThing*>_> *)&local_c);
  bVar4 = param_1;
  for (puVar1 = (undefined4 *)*_Memory; puVar1 != _Memory; puVar1 = (undefined4 *)*puVar1) {
    piVar2 = (int *)puVar1[2];
    if ((piVar2 != (int *)0x0) && ((*(byte *)((int)piVar2 + 0x91) & 1) == 0)) {
      (**(code **)(*piVar2 + 0x70))(bVar4 == false);
    }
  }
  puVar1 = (undefined4 *)*_Memory;
  while (puVar1 != _Memory) {
    puVar3 = (undefined4 *)*puVar1;
    free(puVar1);
    puVar1 = puVar3;
  }
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  free(_Memory);
  return;
}



//=== ?GiveHeroExperience@CGameScriptInterface@@UBEXJ@Z @ 00899000 ===

void _GiveHeroExperience_CGameScriptInterface__UBEXJ_Z(undefined4 param_1,long param_2)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
     ((*(uint *)(extraout_EAX + 0x2c) & 0x100) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x68 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    CTCHeroExperience::AddExperience((CTCHeroExperience *)piVar1[1],param_2);
  }
  return;
}



//=== ?SetHeroAbleToGainExperience@CGameScriptInterface@@UBEX_N@Z @ 00899060 ===

void _SetHeroAbleToGainExperience_CGameScriptInterface__UBEX_N_Z
               (undefined4 param_1,undefined1 param_2)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
     ((*(uint *)(extraout_EAX + 0x2c) & 0x100) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x68 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    *(undefined1 *)(piVar1[1] + 0x3c) = param_2;
  }
  return;
}



//=== ?GiveHeroRenownPoints@CGameScriptInterface@@UBEXJ@Z @ 008990c0 ===

void _GiveHeroRenownPoints_CGameScriptInterface__UBEXJ_Z(undefined4 param_1,CTCHeroStats *param_2)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0) {
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
      if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX + 0x48);
      }
      CTCHeroStats::AddRenown((CTCHeroStats *)piVar1[1],(long)param_2);
      return;
    }
    CTCHeroStats::AddRenown(param_2,(long)param_2);
  }
  return;
}



//=== ?GetHeroRenownLevel@CGameScriptInterface@@UBEJXZ @ 00899140 ===

undefined4 _GetHeroRenownLevel_CGameScriptInterface__UBEJXZ(void)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  ETCInterfaceType unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0) {
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
      if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX + 0x48);
      }
      return *(undefined4 *)(piVar1[1] + 0x70);
    }
    return *(undefined4 *)(unaff_retaddr + 0x70);
  }
  return 0xffffffff;
}



//=== ?IsHeroRenownLevelFull@CGameScriptInterface@@UBE_NXZ @ 008991a0 ===

uint _IsHeroRenownLevelFull_CGameScriptInterface__UBE_NXZ(void)

{
  uint extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  float fVar3;
  CTCHeroStats *unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX == 0) || ((*(byte *)(extraout_EAX + 0x91) & 1) != 0)) {
    return extraout_EAX & 0xffffff00;
  }
  if ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    unaff_retaddr = (CTCHeroStats *)piVar1[1];
  }
  fVar3 = CTCHeroStats::GetPercentageThroughRenownLevel(unaff_retaddr);
  if (fVar3 <= 0.9999) {
    return 0;
  }
  return 1;
}



//=== ?IncreaseHeroRenownLevel@CGameScriptInterface@@UBEXXZ @ 00899220 ===

void _IncreaseHeroRenownLevel_CGameScriptInterface__UBEXXZ(void)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  CTCHeroStats *unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0) {
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
      if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX + 0x48);
      }
      CTCHeroStats::UpgradeRenownLevel((CTCHeroStats *)piVar1[1]);
      return;
    }
    CTCHeroStats::UpgradeRenownLevel(unaff_retaddr);
  }
  return;
}



//=== ?GetHeroRoughExperienceLevel@CGameScriptInterface@@UBEJXZ @ 00899770 ===

long _GetHeroRoughExperienceLevel_CGameScriptInterface__UBEJXZ(void)

{
  int extraout_EAX;
  int *piVar1;
  long lVar2;
  undefined8 uVar3;
  CTCHeroExperience *unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar3 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(uint *)(extraout_EAX + 0x2c) & 0x100) != 0) {
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
      if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x68 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX + 0x48);
      }
      unaff_retaddr = (CTCHeroExperience *)piVar1[1];
    }
    if (unaff_retaddr != (CTCHeroExperience *)0x0) {
      lVar2 = CTCHeroExperience::GetRoughExperienceLevel(unaff_retaddr);
      return lVar2;
    }
  }
  return 0;
}



//=== ?GetHeroExperienceAvailableToSpend@CGameScriptInterface@@UBEJXZ @ 008997e0 ===

undefined4 _GetHeroExperienceAvailableToSpend_CGameScriptInterface__UBEJXZ(void)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  ETCInterfaceType unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(uint *)(extraout_EAX + 0x2c) & 0x100) != 0) {
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
      if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x68 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX + 0x48);
      }
      unaff_retaddr = piVar1[1];
    }
    if (unaff_retaddr != 0) {
      return *(undefined4 *)(unaff_retaddr + 0x14);
    }
  }
  return 0;
}



//=== ?GiveHeroMorality@CGameScriptInterface@@UBEXM@Z @ 00899ef0 ===

void _GiveHeroMorality_CGameScriptInterface__UBEXM_Z
               (ETCInterfaceType param_1,undefined4 param_2,CTCHeroStats *param_3)

{
  int extraout_EAX;
  int extraout_EAX_00;
  int *piVar1;
  long lVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CTCHeroStats *this;
  undefined8 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  
  uVar3 = Catch_0040d959();
  ppVar4 = extraout_var;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),unaff_ESI);
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    uVar3 = Catch_0040d959();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),ppVar4);
    this = param_3;
    if ((*(byte *)(extraout_EAX_00 + 0x20) & 0x10) != 0) {
      param_1 = 4;
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX_00 + 0x44),&param_1);
      if ((piVar1 == *(int **)(extraout_EAX_00 + 0x48)) || (4 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX_00 + 0x48);
      }
      this = (CTCHeroStats *)piVar1[1];
    }
    lVar2 = CTCHeroStats::GetMoralityMax(this);
    param_3 = (CTCHeroStats *)((float)lVar2 * (float)param_3);
    param_1 = (ETCInterfaceType)ROUND((float)param_3);
    CTCHeroStats::SetMorality(this,*(int *)(this + 0x28) + param_1);
  }
  return;
}



//=== ?GetHeroMorality@CGameScriptInterface@@UBEMXZ @ 00899f90 ===

float10 _GetHeroMorality_CGameScriptInterface__UBEMXZ(void)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  float fVar3;
  CTCHeroStats *unaff_retaddr;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0) {
      piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
      if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
        piVar1 = *(int **)(extraout_EAX + 0x48);
      }
      fVar3 = CTCHeroStats::GetMoralityPercentage((CTCHeroStats *)piVar1[1]);
      return (float10)fVar3;
    }
    fVar3 = CTCHeroStats::GetMoralityPercentage(unaff_retaddr);
    return (float10)fVar3;
  }
  return (float10)0.5;
}



//=== ?GetHeroMoralityCategory@CGameScriptInterface@@UBE?AW4EMorality@@XZ @ 0089a000 ===

undefined4 _GetHeroMoralityCategory_CGameScriptInterface__UBE_AW4EMorality__XZ(void)

{
  int extraout_EAX;
  undefined4 uVar1;
  undefined8 uVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if ((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) {
    if ((*(byte *)(extraout_EAX + 0x20) & 0x10) == 0) {
      uVar1 = FUN_0057a6b8();
      return uVar1;
    }
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
    LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    uVar1 = FUN_0057a6b8();
    return uVar1;
  }
  return 3;
}



//=== ?ChangeHeroMoralityDueToTheft@CGameScriptInterface@@UBEXXZ @ 0089a070 ===

void _ChangeHeroMoralityDueToTheft_CGameScriptInterface__UBEXXZ(void)

{
  CTCHeroStats *this;
  int extraout_EAX;
  int *piVar1;
  long lVar2;
  undefined8 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar3 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),extraout_var
            );
  if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
     ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    this = (CTCHeroStats *)piVar1[1];
    lVar2 = GFGetPreMainMemoryUsed();
    CTCHeroStats::SetMorality(this,*(int *)(*(int *)(lVar2 + 0xdc) + 0xe4) + *(int *)(this + 0x28));
  }
  return;
}



//=== ?ChangeHeroMoralityDueToPicklock@CGameScriptInterface@@UBEXXZ @ 0089a0e0 ===

void _ChangeHeroMoralityDueToPicklock_CGameScriptInterface__UBEXXZ(void)

{
  CTCHeroStats *this;
  int extraout_EAX;
  int *piVar1;
  long lVar2;
  undefined8 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar3 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar3,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar3 >> 0x20),extraout_var
            );
  if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
     ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    this = (CTCHeroStats *)piVar1[1];
    lVar2 = GFGetPreMainMemoryUsed();
    CTCHeroStats::SetMorality(this,*(int *)(*(int *)(lVar2 + 0xdc) + 0xe8) + *(int *)(this + 0x28));
  }
  return;
}



//=== ?SetMoralityChangingAsEnabled@CGameScriptInterface@@UBEX_N@Z @ 0089ef30 ===

void _SetMoralityChangingAsEnabled_CGameScriptInterface__UBEX_N_Z
               (undefined4 param_1,undefined1 param_2)

{
  int extraout_EAX;
  int *piVar1;
  undefined8 uVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_var;
  
  uVar2 = Catch_0040d959();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar2,
             (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar2 >> 0x20),extraout_var
            );
  if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
     ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    *(undefined1 *)(piVar1[1] + 0x2c) = param_2;
  }
  return;
}



//=== ?CreateExperienceOrb@CGameScriptInterface@@UBE?AVCScriptThing@@ABVC3DVector@@J@Z @ 0089fbb0 ===

CBase * _CreateExperienceOrb_CGameScriptInterface__UBE_AVCScriptThing__ABVC3DVector__J_Z
                  (CBase *param_1,C3DVector *param_2,undefined4 param_3)

{
  CDefinitionManager *this;
  long lVar1;
  CThing *pCVar2;
  int *piVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  CIsThingAlive *extraout_EDX_00;
  CBase *pCVar4;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString *pCVar5;
  CCharString aCStack_4 [4];
  
  CCharString::CCharString(aCStack_4,"THING_EXPERIENCE_ORB",-1);
  pCVar5 = aCStack_4;
  this = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  lVar1 = CDefinitionManager::GetDefGlobalIndexFromName(this,pCVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4,extraout_EDX
             ,unaff_EDI);
  if (((0 < lVar1) && (pCVar2 = CTCDCameraPoint::Create(lVar1,param_2), pCVar2 != (CThing *)0x0)) &&
     (((byte)pCVar2[0x91] & 1) == 0)) {
    pCVar4 = param_1;
    if ((*(uint *)(pCVar2 + 0x2c) & 0x200) != 0) {
      param_2 = (C3DVector *)0x69;
      piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(pCVar2 + 0x44),(ETCInterfaceType *)&param_2);
      if ((piVar3 == *(int **)(pCVar2 + 0x48)) || (0x69 < *piVar3)) {
        piVar3 = *(int **)(pCVar2 + 0x48);
      }
      pCVar4 = (CBase *)piVar3[1];
    }
    FUN_00779e10(param_3);
    pCVar4[0x1c] = (CBase)0x0;
    NScript::
    GFPredicateAnd<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar2,extraout_EDX_00);
    return param_1;
  }
  NHeroInformationScreens::CBase::CBase(param_1);
  *(undefined ***)param_1 = &PTR__scalar_deleting_destructor__01238c8c;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return param_1;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 008abf80 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 008abff0 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 008aee60 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== ?MsgOnMoralityChanged@CGameScriptThing@@UBE_NAAJ@Z @ 008d3af0 ===

uint _MsgOnMoralityChanged_CGameScriptThing__UBE_NAAJ_Z(int *param_1)

{
  CBaseIntelligentPointer *this;
  int iVar1;
  CBaseObject *pCVar2;
  CBaseObject *pCVar3;
  int *piVar4;
  int in_ECX;
  int iVar5;
  int iStack_8;
  ETCInterfaceType EStack_4;
  
  this = (CBaseIntelligentPointer *)(in_ECX + 0xc);
  iVar5 = 0;
  *param_1 = 0;
  pCVar2 = CBaseIntelligentPointer::GetPItem(this);
  pCVar3 = (CBaseObject *)0x0;
  if ((pCVar2 != (CBaseObject *)0x0) &&
     (pCVar3 = CBaseIntelligentPointer::GetPItem(this), ((byte)pCVar3[0x91] & 1) == 0)) {
    iVar1 = *(int *)(DAT_013b89fc + 0x88);
    if ((iVar1 == 0) || (*(int *)(iVar1 + 8) == 0)) {
      iStack_8 = 0;
    }
    else {
      iStack_8 = *(int *)(*(int *)(*(int *)(iVar1 + 8) + 0x2c) + 0x18);
    }
    if ((iVar1 != 0) && (*(int *)(iVar1 + 8) != 0)) {
      iVar5 = *(int *)(*(int *)(*(int *)(iVar1 + 8) + 0x2c) + 0x1c);
    }
    pCVar3 = CBaseIntelligentPointer::GetPItem(this);
    if (((byte)pCVar3[0x28] & 4) != 0) {
      EStack_4 = 0x42;
      piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(pCVar3 + 0x44),&EStack_4);
      if ((piVar4 == *(int **)(pCVar3 + 0x48)) || (0x42 < *piVar4)) {
        piVar4 = *(int **)(pCVar3 + 0x48);
      }
      iVar1 = piVar4[1];
      pCVar3 = *(CBaseObject **)(iVar1 + 0xc);
      pCVar2 = *(CBaseObject **)pCVar3;
      if (pCVar2 != pCVar3) {
        do {
          piVar4 = *(int **)(pCVar2 + 8);
          pCVar3 = (CBaseObject *)(**(code **)(*piVar4 + 4))();
          if (((pCVar3 == (CBaseObject *)&DAT_00000023) &&
              (pCVar3 = (CBaseObject *)piVar4[1], iStack_8 < (int)pCVar3)) && ((int)pCVar3 <= iVar5)
             ) {
            iVar5 = piVar4[2];
            *param_1 = iVar5;
            return CONCAT31((int3)((uint)iVar5 >> 8),1);
          }
          pCVar2 = *(CBaseObject **)pCVar2;
        } while (pCVar2 != *(CBaseObject **)(iVar1 + 0xc));
      }
    }
  }
  return (uint)pCVar3 & 0xffffff00;
}



//=== ModifyHeroStats @ 008f5bf0 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(class
   CTCHeroStats &)const  */

void __thiscall
NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(CKilledThing *this,CTCHeroStats *param_1)

{
  CTCHeroStats::SetMorality(param_1,*(int *)(param_1 + 0x28) + *(int *)(*(int *)(this + 4) + 0x28));
  CTCHeroStats::AddRenown(param_1,*(long *)(*(int *)(this + 4) + 0x38));
  return;
}



//=== ModifyHeroStats @ 008f5c20 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(class
   CTCHeroStats &)const  */

void __thiscall
NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(CKilledThing *this,CTCHeroStats *param_1)

{
  CTCHeroStats::SetMorality(param_1,*(int *)(param_1 + 0x28) + *(int *)(*(int *)(this + 4) + 0x28));
  CTCHeroStats::AddRenown(param_1,*(long *)(*(int *)(this + 4) + 0x38));
  return;
}



//=== ModifyHeroStats @ 008f5c50 ===

/* [bsim sim=0.964542729733393 <- ego_r]
   public: virtual void __thiscall NHeroStatModifierEvent::CHitThing::ModifyHeroStats(class
   CTCHeroStats &)const  */

void __thiscall
NHeroStatModifierEvent::CHitThing::ModifyHeroStats(CHitThing *this,CTCHeroStats *param_1)

{
  int iVar1;
  CTCHeroStats *this_00;
  int *piVar2;
  long lVar3;
  CTCHeroExperience *this_01;
  
  this_00 = param_1;
  iVar1 = *(int *)(param_1 + 4);
  this_01 = (CTCHeroExperience *)param_1;
  if ((*(uint *)(iVar1 + 0x2c) & 0x100) != 0) {
    param_1 = (CTCHeroStats *)&DAT_00000068;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&param_1);
    if ((piVar2 == *(int **)(iVar1 + 0x48)) || (0x68 < *piVar2)) {
      piVar2 = *(int **)(iVar1 + 0x48);
    }
    this_01 = (CTCHeroExperience *)piVar2[1];
  }
  if (this[0x10] != (CHitThing)0x0) {
    lVar3 = __ftol2();
    CTCHeroExperience::AddTrainableStatExperience
              (this_01,*(EHeroExperienceStatCategory *)(this + 0xc),lVar3);
  }
  if (this[8] == (CHitThing)0x0) {
    CTCHeroStats::SetMorality
              (this_00,*(int *)(*(int *)(this + 4) + 0x30) + *(int *)(this_00 + 0x28));
    return;
  }
  CTCHeroStats::SetMorality(this_00,*(int *)(*(int *)(this + 4) + 0x2c) + *(int *)(this_00 + 0x28));
  return;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 009824e0 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== AddExperience @ 009fc160 ===

/* [bsim sim=0.954288192767905 <- ego_r]
   public: void __thiscall CTCCoopSpirit::AddExperience(long) */

void __thiscall CTCCoopSpirit::AddExperience(CTCCoopSpirit *this,long param_1)

{
  *(int *)(this + 0x2c) = *(int *)(this + 0x2c) + param_1;
  return;
}



//=== ModifyHeroStats @ 009fdba0 ===

/* [bsim sim=0.5058072066416505 <- donor]
   public: virtual void __thiscall NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(class
   CTCHeroStats &)const  */

void __thiscall
NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(CKilledThing *this,CTCHeroStats *param_1)

{
  CBankFile *this_00;
  ulong uVar1;
  CBankFileAsyncData *in_stack_00000008;
  ulong in_stack_0000000c;
  
  this_00 = *(CBankFile **)(*(int *)(this + 0xc) + 0x20);
  uVar1 = CBankFile::GetEntryDataType(this_00,*(ulong *)(*(int *)(this + 0xc) + 0x24));
  CGraphicDataBank::LoadVolumeGraphic
            ((CGraphicDataBank *)this_00,(CManagedTexture *)param_1,in_stack_00000008,
             *(CGraphicHeader **)(this + 0xc),true,in_stack_0000000c,uVar1 == 2);
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 009fff90 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x2c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0xb; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00a32100 ===

/* [bsim sim=0.8005214876901383 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  int iVar1;
  CHeroMarriageInfo *pCVar2;
  CHeroMarriageInfo *pCVar3;
  
  for (; param_1 != param_2; param_1 = param_1 + 0x40) {
    pCVar2 = param_3;
    pCVar3 = param_1;
    for (iVar1 = 0xd; iVar1 != 0; iVar1 = iVar1 + -1) {
      *(undefined4 *)pCVar3 = *(undefined4 *)pCVar2;
      pCVar2 = pCVar2 + 4;
      pCVar3 = pCVar3 + 4;
    }
    CCharString::operator=((CCharString *)(param_1 + 0x34),(CCharString *)(param_3 + 0x34));
    *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(param_3 + 0x38);
    param_1[0x3c] = param_3[0x3c];
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00a323d0 ===

/* [bsim sim=0.5469729922808704 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  uint uVar1;
  CHeroMarriageInfo *pCVar2;
  int iVar3;
  CHeroMarriageInfo *pCVar4;
  CHeroMarriageInfo *pCVar5;
  uint *puVar6;
  uint local_38 [14];
  
  while (param_1 != param_2) {
    local_38[0] = *(uint *)param_1;
    iVar3 = 0xd;
    puVar6 = local_38;
    pCVar4 = param_1;
    while( true ) {
      puVar6 = puVar6 + 1;
      pCVar4 = pCVar4 + 4;
      if (iVar3 == 0) break;
      iVar3 = iVar3 + -1;
      *puVar6 = *(uint *)pCVar4;
    }
    uVar1 = *(uint *)(param_1 + -0x38);
    pCVar5 = param_1 + -0x38;
    pCVar4 = param_1;
    while (pCVar2 = pCVar5, local_38[0] < uVar1) {
      pCVar5 = pCVar2;
      for (iVar3 = 0xe; iVar3 != 0; iVar3 = iVar3 + -1) {
        *(uint *)pCVar4 = *(uint *)pCVar5;
        pCVar5 = pCVar5 + 4;
        pCVar4 = pCVar4 + 4;
      }
      pCVar5 = pCVar2 + -0x38;
      pCVar4 = pCVar2;
      uVar1 = *(uint *)(pCVar2 + -0x38);
    }
    param_1 = param_1 + 0x38;
    puVar6 = local_38;
    for (iVar3 = 0xe; iVar3 != 0; iVar3 = iVar3 + -1) {
      *(uint *)pCVar4 = *puVar6;
      puVar6 = puVar6 + 1;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00a5a540 ===

/* [bsim sim=0.655421118186787 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  float fVar1;
  CHeroMarriageInfo *pCVar2;
  CHeroMarriageInfo *pCVar3;
  
  for (; param_1 != param_2; param_1 = param_1 + 4) {
    fVar1 = *(float *)param_1;
    pCVar3 = param_1;
    while (pCVar2 = pCVar3 + -4, fVar1 < *(float *)pCVar2) {
      *(float *)pCVar3 = *(float *)pCVar2;
      pCVar3 = pCVar2;
    }
    *(float *)pCVar3 = fVar1;
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00a94600 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x20;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 8; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00ab10f0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x34;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0xd; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00ad04a0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x48;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0x12; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Move<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*,CTCHeroAttachableAppearanceModifiers::CModifierGraphic*> @ 00af1ce0 ===

/* [bsim sim=0.8708737176318783 <- ego_r]
   class CTCHeroAttachableAppearanceModifiers::CModifierGraphic * __fastcall std::_Move<class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *>(class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,class
   CTCHeroAttachableAppearanceModifiers::CModifierGraphic *,struct std::_Nonscalar_ptr_iterator_tag)
    */

CModifierGraphic * __fastcall
std::
_Move<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*,CTCHeroAttachableAppearanceModifiers::CModifierGraphic*>
          (int *param_1,int param_2,CModifierGraphic *param_3)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = (param_2 - (int)param_1) / 0xc;
  if (iVar4 < 1) {
    return param_3;
  }
  do {
    piVar2 = *(int **)param_3;
    if (piVar2 != (int *)*param_1) {
      if (piVar2 != (int *)0x0) {
        piVar1 = piVar2 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*piVar2 + 4))();
        }
        *(int *)param_3 = 0;
      }
      iVar3 = *param_1;
      *(int *)param_3 = iVar3;
      if (iVar3 != 0) {
        *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + 1;
      }
    }
    *(int *)(param_3 + 4) = param_1[1];
    *(int *)(param_3 + 8) = param_1[2];
    param_1 = param_1 + 3;
    param_3 = param_3 + 0xc;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  return param_3;
}



//=== CHeroMorphDef @ 00af4e10 ===

/* [bsim sim=0.624647954122192 <- ego_r]
   public: __thiscall CHeroMorphDef::CHeroMorphDef(void) */

CHeroMorphDef * __thiscall CHeroMorphDef::CHeroMorphDef(CHeroMorphDef *this)

{
  void *pvVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129f4e8;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  this[0x19] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x1c) = 0;
  pvVar1 = malloc(0xc);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x1c) = pvVar1;
  *(undefined4 *)(this + 0x20) = 0;
  pvVar1 = malloc(0xc);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x20) = pvVar1;
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x24));
  *(undefined ***)(this + 0x24) = &PTR__vector_deleting_destructor__0125ffdc;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  this[0x35] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  this[0x45] = (CHeroMorphDef)0x0;
  CCharString::CCharString((CCharString *)(this + 0x6c));
  *(undefined4 *)(this + 0x54) = 0x3f800000;
  *(undefined4 *)(this + 0x58) = 0x3f800000;
  *(undefined4 *)(this + 0x60) = 0x3f800000;
  *(undefined4 *)(this + 100) = 0x3f800000;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x68) = 0x3f800000;
  this[0x50] = (CHeroMorphDef)0x82;
  return this;
}



//=== CHeroStatsDef @ 00b0c9c0 ===

/* [bsim sim=0.6168351193694189 <- ego_r]
   public: __thiscall CHeroStatsDef::CHeroStatsDef(void) */

CHeroStatsDef * __thiscall CHeroStatsDef::CHeroStatsDef(CHeroStatsDef *this)

{
  void *pvVar1;
  
  NParticleEngine::CPSCBase::CPSCBase((CPSCBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0129fc1c;
  *(undefined4 *)(this + 0xc) = 0x441fa000;
  this[0x10] = (CHeroStatsDef)((byte)this[0x10] & 0xc0);
  CCharString::CCharString((CCharString *)(this + 0x14));
  *(undefined4 *)(this + 0x18) = 0x42700000;
  *(undefined4 *)(this + 0x1c) = 0x40a00000;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0x3f000000;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x2c) = 0x3f800000;
  *(undefined4 *)(this + 0x4c) = 0x3f800000;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  *(undefined4 *)(this + 0x5c) = 0;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0x3f800000;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x9c) = pvVar1;
  return this;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00b28e40 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00b48ef0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x70;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0x1c; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00b6b580 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x38;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0xe; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00b7aac0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x30;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0xc; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00b8e880 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x1c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 7; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



//=== ModifyHeroStats @ 00c783b0 ===

/* [bsim sim=0.5754979697088439 <- ego_r]
   public: virtual void __thiscall NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(class
   CTCHeroStats &)const  */

void __thiscall
NHeroStatModifierEvent::CKilledThing::ModifyHeroStats(CKilledThing *this,CTCHeroStats *param_1)

{
  FUN_00c77d90(param_1 + 0x58,this + 0x10,4,*(undefined4 *)(*(int *)(this + 0x28) + 4));
  FUN_00c77d90(param_1 + 0x58,this + 0x20,2,*(undefined4 *)(*(int *)(this + 0x28) + 4));
  return;
}



//=== CHeroExperienceDef @ 00ca61d0 ===

/* [bsim sim=0.5306814852262307 <- ego_r]
   public: __thiscall CHeroExperienceDef::CHeroExperienceDef(void) */

CHeroExperienceDef * __thiscall CHeroExperienceDef::CHeroExperienceDef(CHeroExperienceDef *this)

{
  CThingMapManager *in_stack_00000008;
  C2DBoxI *in_stack_0000000c;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_012169de;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  NThingMap::CThingMap::CThingMap((CThingMap *)this,in_stack_00000008,in_stack_0000000c);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_012bf498;
  *(undefined ***)(this + 0xc) = &PTR_LAB_012bf458;
  *(undefined ***)(this + 0x10) = &PTR_LAB_012bf444;
  *(undefined4 *)(this + 0x50) = 0;
  FUN_00cb0190(0);
  local_4._0_1_ = 1;
  FUN_00cb0190(1);
  local_4 = CONCAT31(local_4._1_3_,2);
  FUN_00cb0190(1);
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 100) = 0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 0x7c));
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 0x94));
  *(undefined4 *)(this + 0xac) = 0;
  *(undefined4 *)(this + 0xb0) = 1;
  *(undefined4 *)(this + 0xb4) = 0;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  *(undefined4 *)(this + 0xc0) = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(this + 0xc4));
  SetEvent(*(HANDLE *)(this + 0x5c));
  ExceptionList = local_c;
  return this;
}



//=== CHeroMorphDef @ 00cb8110 ===

/* [bsim sim=0.7488871934775853 <- ego_r]
   public: __thiscall CHeroMorphDef::CHeroMorphDef(void)
   FSE ASLR source:
   ..\..\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\FableScriptExtender\FableAPI.cpp:172
   type=tCScriptBase_Constructor */

CHeroMorphDef * __thiscall CHeroMorphDef::CHeroMorphDef(CHeroMorphDef *this)

{
  void *pvVar1;
  
  NHeroInformationScreens::CBase::CBase((CBase *)this);
  *(undefined ***)this = &PTR__vector_deleting_destructor__012c1648;
  *(undefined4 *)(this + 4) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 4) = pvVar1;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  this[0x15] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  this[0x25] = (CHeroMorphDef)0x0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  this[0x3d] = (CHeroMorphDef)0x0;
  return this;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00cd2994 ===

/* [bsim sim=0.8063763850608386 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar2;
  CCharString *pCVar3;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar2 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar3 = *(CCharString **)param_2;
  if (pCVar3 != param_1) {
    if (pCVar3 != (CCharString *)0x0) {
      pCVar1 = pCVar3 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar3 + 4))();
      }
      *(undefined4 *)pCVar2 = 0;
    }
    *(CCharString **)pCVar2 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  pCVar3 = param_1 + 4;
  *(int *)pCVar3 = *(int *)pCVar3 + -1;
  if (*(int *)pCVar3 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00cd2a71 ===

/* [bsim sim=0.8063763850608386 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar2;
  CCharString *pCVar3;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar2 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar3 = *(CCharString **)param_2;
  if (pCVar3 != param_1) {
    if (pCVar3 != (CCharString *)0x0) {
      pCVar1 = pCVar3 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar3 + 4))();
      }
      *(undefined4 *)pCVar2 = 0;
    }
    *(CCharString **)pCVar2 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  pCVar3 = param_1 + 4;
  *(int *)pCVar3 = *(int *)pCVar3 + -1;
  if (*(int *)pCVar3 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== GetDef<CHeroPostcardGeneratorDef> @ 00cde280 ===

/* [bsim sim=0.8088081535884785 <- ego_r]
   public: bool __thiscall CDefinitionManager::GetDef<class CHeroPostcardGeneratorDef>(class
   CCharString const &,class CDefPointer<class CHeroPostcardGeneratorDef const > *)const  */

bool __thiscall
CDefinitionManager::GetDef<CHeroPostcardGeneratorDef>
          (CDefinitionManager *this,CCharString *param_1,
          CDefPointer<CHeroPostcardGeneratorDef_const_> *param_2)

{
  CCharString *pCVar1;
  CCharString *pCVar2;
  int iVar3;
  CDefPointer<CHeroPostcardGeneratorDef_const_> *pCVar4;
  
  GetPDefFromInstantiationName(this,(CCharString *)&param_1);
  pCVar4 = param_2;
  if (param_1 == (CCharString *)0x0) {
    return false;
  }
  pCVar2 = *(CCharString **)param_2;
  if (pCVar2 != param_1) {
    if (pCVar2 != (CCharString *)0x0) {
      pCVar1 = pCVar2 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)pCVar2 + 4))();
      }
      *(undefined4 *)pCVar4 = 0;
    }
    *(CCharString **)pCVar4 = param_1;
    if (param_1 == (CCharString *)0x0) {
      return true;
    }
    *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 1;
  }
  iVar3 = *(int *)(param_1 + 4);
  *(int *)(param_1 + 4) = iVar3 + -1;
  if (iVar3 + -1 == 0) {
    (**(code **)(*(int *)param_1 + 4))();
  }
  return true;
}



//=== Tutorial_RenownLevelIncrease @ 00cf02a0 ===

/* [bsim sim=0.7070091184882322 <- ego_r]
   public: void __thiscall NScript::CGameflowAssistanceScript::Tutorial_RenownLevelIncrease(void) */

void __thiscall
NScript::CGameflowAssistanceScript::Tutorial_RenownLevelIncrease(CGameflowAssistanceScript *this)

{
  bool bVar1;
  char cVar2;
  int iVar3;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString *pCVar5;
  CCharString aCStack_30 [4];
  CCharString aCStack_2c [8];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_24 [4];
  CCharString local_20 [4];
  CCharString local_1c [4];
  CCharString local_18 [4];
  CCharString local_14 [4];
  CCharString local_10 [4];
  CCharString local_c [4];
  CCharString local_8 [4];
  CCharString local_4 [4];
  
  CCharString::CCharString(local_20,"TEXT_QST_080_RENOWN_LEVEL_ONE",-1);
  CCharString::CCharString(local_1c,"TEXT_QST_080_RENOWN_LEVEL_TWO",-1);
  CCharString::CCharString(local_18,"TEXT_QST_080_RENOWN_LEVEL_THREE",-1);
  CCharString::CCharString(local_14,"TEXT_QST_080_RENOWN_LEVEL_FOUR",-1);
  CCharString::CCharString(local_10,"TEXT_QST_080_RENOWN_LEVEL_FIVE",-1);
  CCharString::CCharString(local_c,"TEXT_QST_080_RENOWN_LEVEL_SIX",-1);
  CCharString::CCharString(local_8,"TEXT_QST_080_RENOWN_LEVEL_SEVEN",-1);
  CCharString::CCharString(local_4);
  bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  ppVar4 = extraout_EDX;
  while( true ) {
    if (bVar1) {
      iVar3 = 8;
      do {
        register0x00000010 = (BADSPACEBASE *)((int)register0x00000010 + -4);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                   register0x00000010,ppVar4,unaff_EDI);
        iVar3 = iVar3 + -1;
        ppVar4 = extraout_EDX_06;
      } while (iVar3 != 0);
      return;
    }
    if (*(char *)(*(int *)(this + 0x44) + 0x67) != '\0') break;
    iVar3 = (**(code **)(**(int **)(this + 0x40) + 0x288))();
    cVar2 = (**(code **)(**(int **)(this + 0x40) + 0x628))();
    if (cVar2 != '\0') {
      bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar1) {
        iVar3 = 8;
        ppVar4 = extraout_EDX_00;
        do {
          register0x00000010 = (BADSPACEBASE *)((int)register0x00000010 + -4);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                     register0x00000010,ppVar4,unaff_EDI);
          iVar3 = iVar3 + -1;
          ppVar4 = extraout_EDX_10;
        } while (iVar3 != 0);
        return;
      }
      if (*(int *)(this + 0x60) < iVar3) {
        bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar1) {
          _vector_constructor_iterator_
                    (local_20,4,8,
                     std::
                     _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    );
          return;
        }
        *(int *)(this + 0x60) = iVar3;
        if (iVar3 < 8) {
          bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
          if (bVar1) {
            _vector_constructor_iterator_
                      (local_20,4,8,
                       std::
                       _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      );
            return;
          }
          CCharString::CCharString(aCStack_30,"",-1);
          (**(code **)(**(int **)(this + 0x40) + 0x448))(aaStack_24 + iVar3 * 4,aCStack_30,1,1);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_30,
                     extraout_EDX_01,unaff_EDI);
        }
        if (1 < iVar3) {
          bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
          if (bVar1) {
            _vector_constructor_iterator_
                      (local_20,4,8,
                       std::
                       _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      );
            return;
          }
          CCharString::CCharString(aCStack_2c,"DUMMY_QUEST_HAS_NO_SCRIPT",-1);
          pCVar5 = aCStack_2c;
          (**(code **)(**(int **)(this + 0x40) + 0x494))();
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_30,
                     extraout_EDX_02,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar5);
          CCharString::CCharString(aCStack_2c,"DUMMY_QUEST_HAS_NO_SCRIPT",-1);
          pCVar5 = aCStack_2c;
          (**(code **)(**(int **)(this + 0x40) + 0x494))();
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_30,
                     extraout_EDX_03,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar5);
          CCharString::CCharString(aCStack_2c,"DUMMY_QUEST_HAS_NO_SCRIPT",-1);
          (**(code **)(**(int **)(this + 0x40) + 0x494))(aCStack_2c);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (aaStack_24,extraout_EDX_04,unaff_EDI);
        }
      }
    }
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    ppVar4 = extraout_EDX_05;
  }
  bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  iVar3 = 8;
  ppVar4 = extraout_EDX_07;
  if (bVar1) {
    do {
      register0x00000010 = (BADSPACEBASE *)((int)register0x00000010 + -4);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)register0x00000010
                 ,ppVar4,unaff_EDI);
      iVar3 = iVar3 + -1;
      ppVar4 = extraout_EDX_08;
    } while (iVar3 != 0);
    return;
  }
  do {
    register0x00000010 = (BADSPACEBASE *)((int)register0x00000010 + -4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)register0x00000010,
               ppVar4,unaff_EDI);
    iVar3 = iVar3 + -1;
    ppVar4 = extraout_EDX_09;
  } while (iVar3 != 0);
  return;
}



//=== AssignMoralityEtcForEvilEnding @ 00e2bac0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: void __thiscall NScript::CV_AmbushScamScript::AssignMoralityEtcForEvilEnding(void) */

void __thiscall
NScript::CV_AmbushScamScript::AssignMoralityEtcForEvilEnding(CV_AmbushScamScript *this)

{
  (**(code **)(**(int **)(this + 0x40) + 0x270))(*(undefined4 *)(DAT_0143e90c + 0x2b4));
  (**(code **)(**(int **)(this + 0x40) + 0x284))(*(undefined4 *)(DAT_0143e90c + 0x2ac));
  (**(code **)(**(int **)(this + 0x40) + 0x200))(*(undefined4 *)(DAT_0143e90c + 0x2a8));
  return;
}



//=== AssignMoralityEtcForEvilEnding @ 00e2bb10 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: void __thiscall NScript::CV_AmbushScamScript::AssignMoralityEtcForEvilEnding(void) */

void __thiscall
NScript::CV_AmbushScamScript::AssignMoralityEtcForEvilEnding(CV_AmbushScamScript *this)

{
  (**(code **)(**(int **)(this + 0x40) + 0x270))(*(undefined4 *)(DAT_0143e90c + 0x2b0));
  (**(code **)(**(int **)(this + 0x40) + 0x284))(*(undefined4 *)(DAT_0143e90c + 0x2ac));
  (**(code **)(**(int **)(this + 0x40) + 0x200))(*(undefined4 *)(DAT_0143e90c + 0x2a8));
  return;
}



//=== ProcessSongsMorality @ 00eda9b0 ===

/* [bsim sim=0.5675479043104589 <- ego_r]
   public: void __thiscall NScript::CV_TalentlessBardScript::ProcessSongsMorality(void) */

void __thiscall
NScript::CV_TalentlessBardScript::ProcessSongsMorality(CV_TalentlessBardScript *this)

{
  undefined4 uVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CCharString aCStack_14 [4];
  CCharString aCStack_10 [4];
  CCharString aCStack_c [4];
  CCharString aCStack_8 [4];
  CCharString aCStack_4 [4];
  
  uVar1 = (**(code **)(**(int **)(this + 0x40) + 0x278))();
  switch(uVar1) {
  case 0:
    CCharString::CCharString(aCStack_14,"TEXT_QST_163_MORAL_EVIL",-1);
    FUN_0044bff0(aCStack_14);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_14,
               extraout_EDX,unaff_ESI);
    return;
  case 1:
  case 2:
    CCharString::CCharString(aCStack_10,"TEXT_QST_163_MORAL_BAD",-1);
    FUN_0044bff0(aCStack_10);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_10,
               extraout_EDX_00,unaff_ESI);
    return;
  case 3:
    CCharString::CCharString(aCStack_c,"TEXT_QST_163_MORAL_NEUT",-1);
    FUN_0044bff0(aCStack_c);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_c,
               extraout_EDX_01,unaff_ESI);
    return;
  case 4:
  case 5:
    CCharString::CCharString(aCStack_8,"TEXT_QST_163_MORAL_GOOD",-1);
    FUN_0044bff0(aCStack_8);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_8,
               extraout_EDX_02,unaff_ESI);
    return;
  case 6:
    CCharString::CCharString(aCStack_4,"TEXT_QST_163_MORAL_PERFECT",-1);
    FUN_0044bff0(aCStack_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4,
               extraout_EDX_03,unaff_ESI);
  }
  return;
}



//=== ProcessSongsRenown @ 00edaad0 ===

/* [bsim sim=0.6050115412669613 <- ego_r]
   public: void __thiscall NScript::CV_TalentlessBardScript::ProcessSongsRenown(void) */

void __thiscall NScript::CV_TalentlessBardScript::ProcessSongsRenown(CV_TalentlessBardScript *this)

{
  undefined4 uVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  CCharString aCStack_c [4];
  CCharString aCStack_8 [4];
  CCharString aCStack_4 [4];
  
  uVar1 = (**(code **)(**(int **)(this + 0x40) + 0x288))();
  switch(uVar1) {
  case 0:
  case 1:
  case 2:
    CCharString::CCharString(aCStack_c,"TEXT_QST_163_RENOWN_NONE",-1);
    FUN_0044bff0(aCStack_c);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_c,
               extraout_EDX,unaff_ESI);
    return;
  case 3:
  case 4:
  case 5:
    CCharString::CCharString(aCStack_8,"TEXT_QST_163_RENOWN_SOME",-1);
    FUN_0044bff0(aCStack_8);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_8,
               extraout_EDX_00,unaff_ESI);
    return;
  case 6:
  case 7:
    CCharString::CCharString(aCStack_4,"TEXT_QST_163_RENOWN_LOTS",-1);
    FUN_0044bff0(aCStack_4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_4,
               extraout_EDX_01,unaff_ESI);
  }
  return;
}



//=== _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&> @ 00f25b00 ===

/* [bsim sim=0.8438385635009688 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember>,class CTCHeroOnlineScoreboard::CClanMember,class
   CTCHeroOnlineScoreboard::CClanMember const &>(class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember> &,class CTCHeroOnlineScoreboard::CClanMember *,class
   CTCHeroOnlineScoreboard::CClanMember const &) */

void __fastcall
std::
_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>
          (allocator<CTCHeroOnlineScoreboard::CClanMember> *param_1,CClanMember *param_2,
          CClanMember *param_3)

{
  if (param_1 != (allocator<CTCHeroOnlineScoreboard::CClanMember> *)0x0) {
    CDefClassBase::CDefClassBase((CDefClassBase *)param_1,(CDefClassBase *)param_2);
    *(undefined ***)param_1 = &PTR__vector_deleting_destructor__012fb288;
    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(param_2 + 0x28);
    FUN_00f25bb0(param_2 + 0x2c);
  }
  return;
}



//=== _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&> @ 00f26db0 ===

/* [bsim sim=0.8438385635009688 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember>,class CTCHeroOnlineScoreboard::CClanMember,class
   CTCHeroOnlineScoreboard::CClanMember const &>(class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember> &,class CTCHeroOnlineScoreboard::CClanMember *,class
   CTCHeroOnlineScoreboard::CClanMember const &) */

void __fastcall
std::
_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>
          (allocator<CTCHeroOnlineScoreboard::CClanMember> *param_1,CClanMember *param_2,
          CClanMember *param_3)

{
  if (param_1 != (allocator<CTCHeroOnlineScoreboard::CClanMember> *)0x0) {
    CDefClassBase::CDefClassBase((CDefClassBase *)param_1,(CDefClassBase *)param_2);
    *(undefined ***)param_1 = &PTR__vector_deleting_destructor__012fb430;
    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(param_2 + 0x28);
    CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)(param_1 + 0x2c));
  }
  return;
}



//=== _Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&> @ 00f26f00 ===

/* [bsim sim=0.8438385635009688 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember>,class CTCHeroOnlineScoreboard::CClanMember,class
   CTCHeroOnlineScoreboard::CClanMember const &>(class std::allocator<class
   CTCHeroOnlineScoreboard::CClanMember> &,class CTCHeroOnlineScoreboard::CClanMember *,class
   CTCHeroOnlineScoreboard::CClanMember const &) */

void __fastcall
std::
_Cons_val<std::allocator<CTCHeroOnlineScoreboard::CClanMember>,CTCHeroOnlineScoreboard::CClanMember,CTCHeroOnlineScoreboard::CClanMember_const&>
          (allocator<CTCHeroOnlineScoreboard::CClanMember> *param_1,CClanMember *param_2,
          CClanMember *param_3)

{
  if (param_1 != (allocator<CTCHeroOnlineScoreboard::CClanMember> *)0x0) {
    CDefClassBase::CDefClassBase((CDefClassBase *)param_1,(CDefClassBase *)param_2);
    *(undefined ***)param_1 = &PTR__vector_deleting_destructor__012fb498;
    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(param_2 + 0x28);
    FUN_00f26fb0(param_2 + 0x2c);
  }
  return;
}



//=== _Fill<CHeroMarriageInfo*,CHeroMarriageInfo> @ 00f37a00 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   void __fastcall std::_Fill<class CHeroMarriageInfo *,class CHeroMarriageInfo>(class
   CHeroMarriageInfo *,class CHeroMarriageInfo *,class CHeroMarriageInfo const &) */

void __fastcall
std::_Fill<CHeroMarriageInfo*,CHeroMarriageInfo>
          (CHeroMarriageInfo *param_1,CHeroMarriageInfo *param_2,CHeroMarriageInfo *param_3)

{
  CHeroMarriageInfo *pCVar1;
  int iVar2;
  CHeroMarriageInfo *pCVar3;
  CHeroMarriageInfo *pCVar4;
  
  while (param_1 != param_2) {
    pCVar1 = param_1 + 0x2c;
    pCVar3 = param_3;
    pCVar4 = param_1;
    for (iVar2 = 0xb; param_1 = pCVar1, iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pCVar3;
      pCVar3 = pCVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
  }
  return;
}



