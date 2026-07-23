//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 0040df00 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x1c;
  }
  return;
}



//=== SetClothingMappingToMenuEntry @ 0042d405 ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCInventoryClothing::SetClothingMappingToMenuEntry(class
   NUISystem::CComponent *,long) */

void __thiscall
CTCInventoryClothing::SetClothingMappingToMenuEntry
          (CTCInventoryClothing *this,CComponent *param_1,long param_2)

{
  long *plVar1;
  
  plVar1 = std::
           map<NUISystem::CComponent*,long,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent*_const,long>_>_>
           ::operator[]((map<NUISystem::CComponent*,long,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent*_const,long>_>_>
                         *)(this + 0x170),&param_1);
  *plVar1 = param_2;
  return;
}



//=== SetClothingMappingToMenuEntry @ 0042d41e ===

/* [bsim sim=1.0 <- ego_r]
   public: void __thiscall CTCInventoryClothing::SetClothingMappingToMenuEntry(class
   NUISystem::CComponent *,long) */

void __thiscall
CTCInventoryClothing::SetClothingMappingToMenuEntry
          (CTCInventoryClothing *this,CComponent *param_1,long param_2)

{
  long *plVar1;
  
  plVar1 = std::
           map<NUISystem::CComponent*,long,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent*_const,long>_>_>
           ::operator[]((map<NUISystem::CComponent*,long,std::less<NUISystem::CComponent*>,std::allocator<std::pair<NUISystem::CComponent*_const,long>_>_>
                         *)(this + 0x160),&param_1);
  *plVar1 = param_2;
  return;
}



//=== ?GetSizeofClass@CAppearanceModifierDef@@UAEKXZ @ 004546d5 ===

undefined4 _GetSizeofClass_CAppearanceModifierDef__UAEKXZ(void)

{
  return 0x70;
}



//=== ?Copy@CAppearanceModifierDef@@UAEXPBVCDefClassBase@@@Z @ 0045cbff ===

void _Copy_CAppearanceModifierDef__UAEXPBVCDefClassBase___Z(CThingBuildingDef *param_1)

{
  CThingBuildingDef *in_ECX;
  
  CThingBuildingDef::operator=(in_ECX,param_1);
  FUN_00432ee9(param_1 + 0x28);
  FUN_0045cc79(param_1 + 0x34);
  *(undefined4 *)(in_ECX + 0x40) = *(undefined4 *)(param_1 + 0x40);
  in_ECX[0x44] = param_1[0x44];
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
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 0046aeaa ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 5; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x14;
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 0046c16a ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 0x15; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x54;
  }
  return;
}



//=== ?Transfer@CAppearanceModifierDef@@UAEXAAVCPersistContext@@@Z @ 0046fc51 ===

/* [bsim sim=0.7534714130295807 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CAppearanceModifierDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  FUN_0046fe40(in_ECX + 0x34);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 004701ac ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x18;
  }
  return;
}



//=== ?GetSizeofClass@CAppearanceModifierScalingDef@@UAEKXZ @ 004784e0 ===

undefined4 _GetSizeofClass_CAppearanceModifierScalingDef__UAEKXZ(void)

{
  return 0x48;
}



//=== ?Transfer@CAppearanceModifierScalingDef@@UAEXAAVCPersistContext@@@Z @ 004784e4 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CAppearanceModifierScalingDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x44));
  return (bool)extraout_AL;
}



//=== ?Copy@CAppearanceModifierScalingDef@@UAEXPBVCDefClassBase@@@Z @ 0047852e ===

CBalverineBattleDef * __thiscall
CBalverineBattleDef::_Copy_CAppearanceModifierScalingDef__UAEXPBVCDefClassBase___Z
          (CBalverineBattleDef *this,CBalverineBattleDef *param_1)

{
  CBalverineBattleDef *pCVar1;
  
  pCVar1 = operator=(this,param_1);
  return pCVar1;
}



//=== CTCDHeroSuit @ 004de7fd ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CTCDHeroSuit::CTCDHeroSuit(class CThing &) */

CTCDHeroSuit * __thiscall CTCDHeroSuit::CTCDHeroSuit(CTCDHeroSuit *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124176c;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  return this;
}



//=== CTCDHeroSuit @ 004e079c ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CTCDHeroSuit::CTCDHeroSuit(class CThing &) */

CTCDHeroSuit * __thiscall CTCDHeroSuit::CTCDHeroSuit(CTCDHeroSuit *this,CThing *param_1)

{
  CTCDriverBase::CTCDriverBase((CTCDriverBase *)this,param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__01242004;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  return this;
}



//=== ?GetSizeofClass@CHeroSuitDef@@UAEKXZ @ 004e091a ===

undefined4 _GetSizeofClass_CHeroSuitDef__UAEKXZ(void)

{
  return 0x34;
}



//=== CTCDHeroSuit @ 004e0aab ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CTCDHeroSuit::CTCDHeroSuit(class CThing &) */

CTCDHeroSuit * __thiscall CTCDHeroSuit::CTCDHeroSuit(CTCDHeroSuit *this,CThing *param_1)

{
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__scalar_deleting_destructor__012421e4;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 1000;
  return this;
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



//=== _Copy_impl<CAppearanceModifierGraphics::CEntry*,CAppearanceModifierGraphics::CEntry*> @ 00564d5d ===

/* [bsim sim=0.5252409581875608 <- donor]
   struct CAppearanceModifierGraphics::CEntry * __fastcall std::_Copy_impl<struct
   CAppearanceModifierGraphics::CEntry *,struct CAppearanceModifierGraphics::CEntry *>(struct
   CAppearanceModifierGraphics::CEntry *,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry *,struct std::_Nonscalar_ptr_iterator_tag) */

CEntry * __fastcall
std::_Copy_impl<CAppearanceModifierGraphics::CEntry*,CAppearanceModifierGraphics::CEntry*>
          (uint param_1,undefined4 param_2,CEntry *param_3,CEntry *param_4)

{
  uint uVar1;
  CEntry *pCVar2;
  
  pCVar2 = param_3;
  for (uVar1 = param_1 >> 2; uVar1 != 0; uVar1 = uVar1 - 1) {
    *(undefined4 *)pCVar2 = *(undefined4 *)param_4;
    param_4 = param_4 + 4;
    pCVar2 = pCVar2 + 4;
  }
  for (param_1 = param_1 & 3; param_1 != 0; param_1 = param_1 - 1) {
    *pCVar2 = *param_4;
    param_4 = param_4 + 1;
    pCVar2 = pCVar2 + 1;
  }
  return param_3;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00578f1a ===

/* [bsim sim=0.6048687854874351 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  void *pvVar1;
  int iVar2;
  undefined4 *puVar3;
  
  pvVar1 = malloc(0x24);
  if ((undefined4 *)((int)pvVar1 + 8) != (undefined4 *)0x0) {
    puVar3 = (undefined4 *)((int)pvVar1 + 8);
    for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar3 = *(undefined4 *)param_3;
      param_3 = param_3 + 4;
      puVar3 = puVar3 + 1;
    }
  }
  return;
}



//=== ConstructTradeDescriptionClothing @ 005aaf28 ===

/* [bsim sim=0.8828358353861008 <- ego_r]
   public: class NUISystem::CComponent * __thiscall
   CTCInventoryTrade::ConstructTradeDescriptionClothing(long,unsigned long,bool,bool) */

CComponent * __thiscall
CTCInventoryTrade::ConstructTradeDescriptionClothing
          (CTCInventoryTrade *this,long param_1,ulong param_2,bool param_3,bool param_4)

{
  CTCInventoryClothing *this_00;
  char cVar1;
  bool bVar2;
  byte bVar3;
  CFrontEndManager *this_01;
  CComponent *pCVar4;
  COptimisedPrimitive *this_02;
  long lVar5;
  CDefinitionManager *this_03;
  CIDrawEnvironment *pCVar6;
  int *piVar7;
  CDefinitionManager *this_04;
  undefined4 extraout_EAX;
  CWideString *pCVar8;
  undefined4 extraout_EAX_00;
  CCharString *pCVar9;
  int iVar10;
  int extraout_EAX_01;
  int *piVar11;
  int *piVar12;
  undefined4 extraout_EAX_02;
  undefined4 extraout_EAX_03;
  int extraout_EAX_04;
  int extraout_EAX_05;
  undefined4 extraout_EAX_06;
  undefined4 extraout_EAX_07;
  undefined4 extraout_EAX_08;
  undefined4 extraout_EAX_09;
  undefined4 extraout_EAX_10;
  undefined4 uVar13;
  CCharString *pCVar14;
  CDefinitionManager *this_05;
  undefined4 extraout_EAX_11;
  undefined4 uVar15;
  ulong uVar16;
  CDefClassBase *pCVar17;
  CMemoryAllocatorVariableSize *extraout_ECX;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar18;
  CMemoryAllocatorVariableSize *extraout_ECX_00;
  CMemoryAllocatorVariableSize *this_06;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar19;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_23;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_24;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_25;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_26;
  byte *pbVar20;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined8 uVar21;
  undefined3 in_stack_0000000d;
  undefined3 in_stack_00000011;
  bool *pbVar22;
  CTCInventoryExperience **ppCVar23;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar24;
  uint *puVar25;
  int *piVar26;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar27;
  undefined1 uVar28;
  char *pcVar29;
  int **ppiVar30;
  int *local_78;
  undefined4 local_74;
  int *local_70;
  uint local_6c;
  CCharString local_68 [4];
  CGuiControlNumber local_64 [4];
  CCharString local_60 [4];
  CCharString local_5c [4];
  CCharString local_58 [4];
  CCharString local_54 [4];
  CCharString local_50 [4];
  CCharString local_4c [4];
  CTCInventoryExperience *local_48;
  CDefClassBase *local_44;
  ETCInterfaceType local_40;
  CCharString local_3c [4];
  CCharString local_38 [4];
  CCharString local_34 [4];
  CDefClassBase *local_30;
  int *local_2c;
  int *local_28;
  CTCInventoryClothing *local_24;
  CTCShop *local_20;
  long local_1c;
  CDataBank *local_18;
  CComponent *local_14;
  CTCInventoryTrade *local_10;
  CFrontEndManager *local_c;
  CCharString local_8 [4];
  
  local_10 = this;
  this_01 = NUISystem::CFrontEndManager::GetInstance();
  local_78 = (int *)0x0;
  local_74 = 0;
  local_c = this_01;
  if (this[0x218] == (CTCInventoryTrade)0x0) {
    CCharString::CCharString(local_34,"UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE",-1);
    local_14 = NUISystem::CManager::CreateComponent((CManager *)this_01,local_34,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_34,
               extraout_EDX_00,unaff_EDI);
    pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
  }
  else {
    CCharString::CCharString(local_34,"PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT",-1);
    local_14 = NUISystem::CManager::CreateComponent((CManager *)this_01,local_34,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_34,
               extraout_EDX,unaff_EDI);
    pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
  }
  CCharString::CCharString((CCharString *)&local_2c,pcVar29,-1);
  pCVar4 = NUISystem::CManager::CreateComponent((CManager *)this_01,(CCharString *)&local_2c,false);
  CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
            ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)&local_78,
             (vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_2c,
             extraout_EDX_01,unaff_EDI);
  CCharString::CCharString(local_8);
  local_28 = (int *)0x0;
  CCharString::CCharString((CCharString *)&local_2c,"UI_MISC_THINGS_DEF",-1);
  ppiVar30 = &local_2c;
  this_02 = (COptimisedPrimitive *)GFGetPreMainMemoryUsed();
  lVar5 = QuadricOptimiserInternals::COptimisedPrimitive::ActualToUniqueIndex
                    (this_02,(ulong)ppiVar30);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_2c,
             extraout_EDX_02,unaff_EDI);
  pCVar24 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_28;
  this_03 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
  CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_03,lVar5,pCVar24);
  piVar12 = local_78;
  local_20 = (CTCShop *)0x0;
  pCVar6 = CWorld::DrawGetEnvironment((CWorld *)local_10);
  cVar1 = FUN_00410de0(0x53);
  if (cVar1 != '\0') {
    local_40 = 0x53;
    piVar7 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(pCVar6 + 0x44),&local_40);
    if ((piVar7 == *(int **)(pCVar6 + 0x48)) || (0x53 < *piVar7)) {
      piVar7 = *(int **)(pCVar6 + 0x48);
    }
    local_20 = (CTCShop *)piVar7[1];
  }
  if ((param_3 == false) || (param_4 != false)) {
    local_1c = CTCShop::GetCurrentBuyPrice(local_20,param_1,1);
    ppVar19 = extraout_EDX_04;
  }
  else {
    local_1c = CTCShop::GetCurrentBuyPrice(local_20,param_1,1);
    ppVar19 = extraout_EDX_03;
  }
  ppVar27 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_30;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,ppVar19,
             ppVar27);
  CDefinitionManager::GetPDefFromGlobalIndex(this_04,(long)ppVar27);
  local_2c = (int *)0x0;
  if ((local_30 == (CDefClassBase *)0x0) ||
     (bVar2 = CDefClassBase::GetSubDef<CAbilityDef>
                        (local_30,(CDefPointer<CAbilityDef_const_> *)&local_2c), !bVar2)) {
    local_24 = (CTCInventoryClothing *)0x1;
  }
  else {
    local_24 = (CTCInventoryClothing *)local_2c[10];
  }
  piVar7 = local_28;
  local_18 = *(CDataBank **)(DAT_013b86a0 + 0x14);
  if (param_4 == false) {
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_44,(long)unaff_EDI);
    piVar11 = piVar7 + 0xf;
    piVar26 = piVar7 + 0x11;
    uVar13 = extraout_EAX_02;
    FUN_0099b2c0(&local_28);
    NGameText::CDataBank::GetTextBySymbol(local_18,local_38);
    FUN_0099be70(piVar26);
    FUN_0099be70(piVar11);
    pCVar8 = (CWideString *)FUN_0099be70(uVar13);
    CWideString::operator=((CWideString *)local_8,pCVar8);
    CCharString::~CCharString((CCharString *)&param_4);
    CCharString::~CCharString((CCharString *)&local_40);
    CCharString::~CCharString(local_3c);
    CCharString::~CCharString(local_38);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_28,
               extraout_EDX_07,unaff_EDI);
    CCharString::~CCharString((CCharString *)&local_44);
    piVar11 = piVar7 + 0x14;
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)local_3c,(long)piVar11);
    uVar13 = extraout_EAX_03;
    FUN_0099be70(piVar7 + 0x13);
    FUN_0099be70(uVar13);
    pCVar9 = (CCharString *)FUN_0099be70(piVar11);
    CCharString::operator+=(local_8,pCVar9);
    CCharString::~CCharString((CCharString *)&param_4);
    CCharString::~CCharString((CCharString *)&local_44);
    CCharString::~CCharString((CCharString *)&local_40);
    CCharString::~CCharString(local_3c);
    (**(code **)(*(int *)local_14 + 0xec))(&local_78);
    uVar28 = 1;
    pCVar9 = local_8;
    ppVar27 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x1;
    (**(code **)(*piVar12 + 0x240))();
    if (((int)local_24 < local_1c) && (param_3 != false)) {
LAB_005ab4b6:
      pbVar22 = &param_4;
      _param_4 = (CDefClassBase *)0x0;
      (**(code **)(*local_78 + 0x148))(pbVar22);
      iVar10 = FUN_0042d5b1(pbVar22);
      _param_4 = (CDefClassBase *)(uint)*(byte *)(iVar10 + 0x18);
      bVar3 = __ftol2();
      *(byte *)(iVar10 + 0x18) = bVar3;
      pbVar22 = &param_4;
      _param_4 = (CDefClassBase *)0x0;
      (**(code **)(*local_78 + 0x148))(pbVar22);
      iVar10 = FUN_0042d5b1(pbVar22);
      pbVar20 = (byte *)(iVar10 + 0x19);
      bVar3 = *pbVar20;
LAB_005ab56e:
      _param_4 = (CDefClassBase *)(uint)bVar3;
      bVar3 = __ftol2();
      *pbVar20 = bVar3;
    }
    else {
      if (local_1c < (int)local_24) {
        if (param_3 != false) goto LAB_005ab517;
        goto LAB_005ab4b6;
      }
      if (((int)local_24 < local_1c) && (param_3 == false)) {
LAB_005ab517:
        pbVar22 = &param_4;
        _param_4 = (CDefClassBase *)0x0;
        (**(code **)(*local_78 + 0x148))(pbVar22);
        iVar10 = FUN_0042d5b1(pbVar22);
        _param_4 = (CDefClassBase *)(uint)*(byte *)(iVar10 + 0x18);
        bVar3 = __ftol2();
        *(byte *)(iVar10 + 0x18) = bVar3;
        pbVar22 = &param_4;
        _param_4 = (CDefClassBase *)0x0;
        (**(code **)(*local_78 + 0x148))(pbVar22);
        iVar10 = FUN_0042d5b1(pbVar22);
        pbVar20 = (byte *)(iVar10 + 0x1a);
        bVar3 = *pbVar20;
        goto LAB_005ab56e;
      }
    }
    uVar21 = Catch_0040d959();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar21,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar21 >> 0x20),
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
    local_24 = (CTCInventoryClothing *)0x0;
    _param_4 = (CDefClassBase *)CTCInventoryItem::GetInventoryCategory(param_1);
    cVar1 = FUN_00410de0(_param_4);
    if (cVar1 != '\0') {
      local_44 = _param_4;
      piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                       ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                     *)(extraout_EAX_04 + 0x44),(ETCInterfaceType *)&local_44);
      if ((piVar12 == *(int **)(extraout_EAX_04 + 0x48)) || ((int)_param_4 < *piVar12)) {
        piVar12 = *(int **)(extraout_EAX_04 + 0x48);
      }
      local_24 = (CTCInventoryClothing *)piVar12[1];
    }
    bVar2 = CTCShop::IsBarberShop(local_20);
    if ((bVar2) || (bVar2 = CTCShop::IsBarberShop(local_20), bVar2)) {
LAB_005ab5fe:
      _param_4 = (CDefClassBase *)CONCAT13(1,_param_4);
    }
    else {
      bVar2 = CTCShop::IsBarberShop(local_20);
      _param_4 = (CDefClassBase *)((uint)_param_4 & 0xffffff);
      if (bVar2) goto LAB_005ab5fe;
    }
    if (param_3 == false) {
      if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
          (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
        pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
      }
      else {
        pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
      }
      CCharString::CCharString((CCharString *)&local_28,pcVar29,-1);
      pCVar4 = NUISystem::CManager::CreateComponent
                         ((CManager *)local_c,(CCharString *)&local_28,false);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_78,(vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_28,
                 extraout_EDX_08,ppVar27);
      local_28 = local_78;
      NInventory::CTCInventoryBase::GetItemDescription
                ((CTCInventoryBase *)local_24,(CWideString *)param_1,(long)ppVar27,(bool)uVar28);
      lVar5 = local_1c;
      if (local_1c - extraout_EAX_05 < 0) {
        CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_1c,(long)ppVar27);
        piVar12 = piVar7 + 0xf;
        piVar11 = piVar7 + 0x11;
        uVar13 = extraout_EAX_07;
        FUN_0099b2c0(local_38);
        NGameText::CDataBank::GetTextBySymbol(local_18,local_3c);
        FUN_0099be70(piVar11);
        FUN_0099be70(piVar12);
        pCVar8 = (CWideString *)FUN_0099be70(uVar13);
        CWideString::operator=((CWideString *)local_8,pCVar8);
        CCharString::~CCharString((CCharString *)&local_48);
        CCharString::~CCharString((CCharString *)&local_44);
        CCharString::~CCharString((CCharString *)&local_40);
        CCharString::~CCharString(local_3c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_38,
                   extraout_EDX_10,ppVar27);
        CCharString::~CCharString((CCharString *)&local_1c);
        ppCVar23 = &local_48;
        local_48 = (CTCInventoryExperience *)0x0;
        (**(code **)(*local_78 + 0x148))(ppCVar23);
        iVar10 = FUN_0042d5b1(ppCVar23);
        local_48 = (CTCInventoryExperience *)(uint)*(byte *)(iVar10 + 0x18);
        bVar3 = __ftol2();
        *(byte *)(iVar10 + 0x18) = bVar3;
        ppCVar23 = &local_48;
        local_48 = (CTCInventoryExperience *)0x0;
        (**(code **)(*local_78 + 0x148))(ppCVar23);
        iVar10 = FUN_0042d5b1(ppCVar23);
        pbVar20 = (byte *)(iVar10 + 0x19);
        bVar3 = *pbVar20;
LAB_005ab831:
        local_48 = (CTCInventoryExperience *)(uint)bVar3;
        bVar3 = __ftol2();
        *pbVar20 = bVar3;
      }
      else {
        CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_48,(long)ppVar27);
        piVar12 = piVar7 + 0xf;
        piVar11 = piVar7 + 0x11;
        uVar13 = extraout_EAX_06;
        FUN_0099b2c0(&local_1c);
        NGameText::CDataBank::GetTextBySymbol(local_18,local_38);
        FUN_0099be70(piVar11);
        FUN_0099be70(piVar12);
        pCVar8 = (CWideString *)FUN_0099be70(uVar13);
        CWideString::operator=((CWideString *)local_8,pCVar8);
        CCharString::~CCharString((CCharString *)&local_44);
        CCharString::~CCharString((CCharString *)&local_40);
        CCharString::~CCharString(local_3c);
        CCharString::~CCharString(local_38);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_1c,
                   extraout_EDX_09,ppVar27);
        CCharString::~CCharString((CCharString *)&local_48);
        if (lVar5 != extraout_EAX_05) {
          ppCVar23 = &local_48;
          local_48 = (CTCInventoryExperience *)0x0;
          (**(code **)(*local_78 + 0x148))(ppCVar23);
          iVar10 = FUN_0042d5b1(ppCVar23);
          local_48 = (CTCInventoryExperience *)(uint)*(byte *)(iVar10 + 0x18);
          bVar3 = __ftol2();
          *(byte *)(iVar10 + 0x18) = bVar3;
          ppCVar23 = &local_48;
          local_48 = (CTCInventoryExperience *)0x0;
          (**(code **)(*local_78 + 0x148))(ppCVar23);
          iVar10 = FUN_0042d5b1(ppCVar23);
          pbVar20 = (byte *)(iVar10 + 0x1a);
          bVar3 = *pbVar20;
          goto LAB_005ab831;
        }
      }
      (**(code **)(*(int *)local_14 + 0xec))(&local_78);
      (**(code **)(*local_28 + 0x240))(local_8,1,1);
    }
    iVar10 = (**(code **)(*(int *)local_24 + 0x54))();
    if (iVar10 == 0x12) {
      if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
          (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
        pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
      }
      else {
        pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
      }
      CCharString::CCharString((CCharString *)&local_28,pcVar29,-1);
      pCVar4 = NUISystem::CManager::CreateComponent
                         ((CManager *)local_c,(CCharString *)&local_28,false);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_78,(vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_28,
                 extraout_EDX_11,ppVar27);
      piVar12 = local_78;
      CWideString::operator=((CWideString *)local_8,(CWideString *)&DAT_013bca24);
      CTCInventoryClothing::GetItemDescriptionArmourRating(local_24,(CWideString *)local_8,param_1);
      cVar1 = FUN_0099b0f0(&DAT_013bca24);
      if (cVar1 != '\0') {
        (**(code **)(*(int *)local_14 + 0xec))(&local_78);
        (**(code **)(*piVar12 + 0x240))(local_8,1,1);
      }
    }
    if (param_3 == false) {
LAB_005aba0b:
      if (in_stack_00000011._2_1_ == '\0') {
        if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
            (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
          pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
        }
        else {
          pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
        }
        CCharString::CCharString((CCharString *)&param_4,pcVar29,-1);
        pCVar4 = NUISystem::CManager::CreateComponent
                           ((CManager *)local_c,(CCharString *)&param_4,false);
        CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                  ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                   &local_78,(vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_4,
                   extraout_EDX_14,ppVar27);
        piVar26 = local_78;
        _param_4 = (CDefClassBase *)CTCShop::GetMaxStock(local_20,param_1);
        local_28 = (int *)CTCShop::GetMaxStock(local_20,param_1);
        CGuiControlNumber::SetSLongValue((CGuiControlNumber *)local_38,(long)ppVar27);
        piVar12 = piVar7 + 0xf;
        piVar11 = piVar7 + 0x11;
        uVar13 = extraout_EAX_09;
        FUN_0099b2c0(local_3c);
        NGameText::CDataBank::GetTextBySymbol(local_18,(CCharString *)&local_40);
        FUN_0099be70(piVar11);
        FUN_0099be70(piVar12);
        pCVar8 = (CWideString *)FUN_0099be70(uVar13);
        CWideString::operator=((CWideString *)local_8,pCVar8);
        CCharString::~CCharString((CCharString *)&param_4);
        CCharString::~CCharString((CCharString *)&local_48);
        CCharString::~CCharString((CCharString *)&local_44);
        CCharString::~CCharString((CCharString *)&local_40);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_3c,
                   extraout_EDX_15,ppVar27);
        CCharString::~CCharString(local_38);
        CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_48,(long)ppVar27);
        pCVar9 = (CCharString *)FUN_0099bff0(extraout_EAX_10);
        CCharString::operator+=(local_8,pCVar9);
        CCharString::~CCharString((CCharString *)&param_4);
        CCharString::~CCharString((CCharString *)&local_48);
        (**(code **)(*(int *)local_14 + 0xec))(&local_78);
        pCVar9 = local_8;
        (**(code **)(*piVar26 + 0x240))(pCVar9,1,1);
        if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
            (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
          pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
        }
        else {
          pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
        }
        CCharString::CCharString((CCharString *)&param_4,pcVar29,-1);
        pCVar4 = NUISystem::CManager::CreateComponent
                           ((CManager *)local_c,(CCharString *)&param_4,false);
        CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                  ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                   &local_78,(vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_4,
                   extraout_EDX_16,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9
                  );
        piVar26 = local_78;
        _param_4 = (CDefClassBase *)CTCShop::GetDaysTillRestock(local_20,param_1);
        piVar12 = piVar7 + 0xf;
        piVar11 = piVar7 + 0x11;
        FUN_0099b2c0(local_3c);
        NGameText::CDataBank::GetTextBySymbol(local_18,(CCharString *)&local_40);
        FUN_0099be70(piVar11);
        pCVar8 = (CWideString *)FUN_0099be70(piVar12);
        CWideString::operator=((CWideString *)local_8,pCVar8);
        CCharString::~CCharString((CCharString *)&local_48);
        CCharString::~CCharString((CCharString *)&local_44);
        CCharString::~CCharString((CCharString *)&local_40);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_3c,
                   extraout_EDX_17,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9
                  );
        if (_param_4 == (CDefClassBase *)0x0) {
          FUN_0099b2c0(&local_48);
          pCVar14 = (CCharString *)
                    NGameText::CDataBank::GetTextBySymbol(local_18,(CCharString *)&param_4);
          CCharString::operator+=(local_8,pCVar14);
          CCharString::~CCharString((CCharString *)&param_4);
          paVar18 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_48;
          ppVar19 = extraout_EDX_19;
        }
        else {
          FUN_0099b2c0(local_38);
          uVar13 = NGameText::CDataBank::GetTextBySymbol(local_18,local_3c);
          piVar12 = piVar7 + 0xf;
          CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_40,(long)piVar12);
          FUN_0099be70(piVar12);
          pCVar14 = (CCharString *)FUN_0099be70(uVar13);
          CCharString::operator+=(local_8,pCVar14);
          CCharString::~CCharString((CCharString *)&local_48);
          CCharString::~CCharString((CCharString *)&local_44);
          CCharString::~CCharString((CCharString *)&local_40);
          CCharString::~CCharString(local_3c);
          paVar18 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_38;
          ppVar19 = extraout_EDX_18;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar18,ppVar19,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9
                  );
        (**(code **)(*(int *)local_14 + 0xec))(&local_78);
        (**(code **)(*piVar26 + 0x240))(local_8,1,1);
      }
    }
    else if (in_stack_00000011._2_1_ == '\0') {
      if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
          (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
        pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
      }
      else {
        pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
      }
      CCharString::CCharString((CCharString *)&local_28,pcVar29,-1);
      pCVar4 = NUISystem::CManager::CreateComponent
                         ((CManager *)local_c,(CCharString *)&local_28,false);
      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                 &local_78,(vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_28,
                 extraout_EDX_12,ppVar27);
      piVar26 = local_78;
      NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory
                ((CTCInventoryBase *)local_24,param_1);
      CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_28,(long)ppVar27);
      piVar12 = piVar7 + 0xf;
      piVar11 = piVar7 + 0x11;
      uVar13 = extraout_EAX_08;
      FUN_0099b2c0(local_38);
      NGameText::CDataBank::GetTextBySymbol(local_18,local_3c);
      FUN_0099be70(piVar11);
      FUN_0099be70(piVar12);
      pCVar8 = (CWideString *)FUN_0099be70(uVar13);
      CWideString::operator=((CWideString *)local_8,pCVar8);
      CCharString::~CCharString((CCharString *)&local_48);
      CCharString::~CCharString((CCharString *)&local_44);
      CCharString::~CCharString((CCharString *)&local_40);
      CCharString::~CCharString(local_3c);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_38,
                 extraout_EDX_13,ppVar27);
      CCharString::~CCharString((CCharString *)&local_28);
      (**(code **)(*(int *)local_14 + 0xec))(&local_78);
      (**(code **)(*piVar26 + 0x240))(local_8,1,1);
      goto LAB_005aba0b;
    }
    if ((param_3 != false) && (bVar2 = CTCInventoryItem::IsAugmentation(param_1), bVar2)) {
      _param_4 = (CDefClassBase *)0x0;
      pCVar24 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&param_4;
      lVar5 = param_1;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,
                 extraout_EDX_20,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1)
      ;
      CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_05,lVar5,pCVar24);
      _param_3 = (int *)0x0;
      CDefClassBase::GetSubDef<CAbilityDef>(_param_4,(CDefPointer<CAbilityDef_const_> *)&param_3);
      if (_param_3[0xb] == 2) {
        iVar10 = *(int *)(local_10 + 4);
        cVar1 = FUN_00410de0(0x68);
        if (cVar1 != '\0') {
          local_48 = (CTCInventoryExperience *)&DAT_00000068;
          piVar11 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                           ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                         *)(iVar10 + 0x44),(ETCInterfaceType *)&local_48);
          piVar12 = *(int **)(iVar10 + 0x48);
          if ((piVar11 != piVar12) && (*piVar11 < 0x69)) {
            piVar12 = piVar11;
          }
          local_20 = (CTCShop *)piVar12[1];
          iVar10 = *(int *)(local_10 + 4);
          cVar1 = FUN_00410de0(0x16);
          if (cVar1 != '\0') {
            local_48 = (CTCInventoryExperience *)0x16;
            piVar11 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                             ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                           *)(iVar10 + 0x44),(ETCInterfaceType *)&local_48);
            piVar12 = *(int **)(iVar10 + 0x48);
            if ((piVar11 != piVar12) && (*piVar11 < 0x17)) {
              piVar12 = piVar11;
            }
            local_48 = (CTCInventoryExperience *)piVar12[1];
            piVar12 = *(int **)(local_20 + 0x58);
            local_70 = piVar12;
            if (piVar12 != (int *)0x0) {
              piVar12[1] = piVar12[1] + 1;
              piVar12[1] = piVar12[1] + 1;
              piVar11 = piVar12 + 1;
              *piVar11 = *piVar11 + -1;
              if (*piVar11 == 0) {
                (**(code **)(*piVar12 + 4))();
              }
              uVar16 = *(ulong *)(piVar12[0x4f] + 8);
              if (uVar16 != piVar12[0x4f]) {
                do {
                  lVar5 = CTCInventoryAbilities::GetAbilityLevel
                                    ((CTCInventoryAbilities *)local_20,
                                     *(EHeroAbility *)(uVar16 + 0x10));
                  this_06 = extraout_ECX;
                  if (lVar5 < *(int *)(uVar16 + 0x14)) {
                    if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                         (local_10 + 0x218) ==
                        (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
                      CCharString::CCharString(local_38,"UI_TEXT_INSIDE_CONTAINER_TEMPLATE",-1);
                      pCVar4 = NUISystem::CManager::CreateComponent
                                         ((CManager *)local_c,local_38,false);
                      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>
                                  *)&local_78,
                                 (vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
                      paVar18 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                                local_38;
                      ppVar19 = extraout_EDX_22;
                    }
                    else {
                      CCharString::CCharString
                                ((CCharString *)&local_1c,"PC_TEXT_INSIDE_CONTAINER_TEMPLATE",-1);
                      pCVar4 = NUISystem::CManager::CreateComponent
                                         ((CManager *)local_c,(CCharString *)&local_1c,false);
                      CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                                ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>
                                  *)&local_78,
                                 (vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
                      paVar18 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                                &local_1c;
                      ppVar19 = extraout_EDX_21;
                    }
                    std::
                    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                              (paVar18,ppVar19,ppVar27);
                    local_28 = local_78;
                    CCharString::CCharString((CCharString *)&local_40,"TEXT_GUI_MENU_REQUIRED",-1);
                    CCharString::CCharString(local_3c,"TEXT_GUI_MENU_EXP_LEVEL",-1);
                    lVar5 = NGameText::CDataBank::GetTextBySymbol(local_18,local_68);
                    CGuiControlNumber::SetSLongValue(local_64,lVar5);
                    piVar12 = piVar7 + 0xf;
                    piVar11 = piVar12;
                    uVar13 = extraout_EAX_11;
                    uVar15 = NGameText::CDataBank::GetTextBySymbol(local_18,local_60);
                    CTCInventoryExperience::GetExperienceAbilityName(local_48,(long)local_5c);
                    FUN_0099be70(piVar12);
                    FUN_0099be70(uVar15);
                    FUN_0099be70(piVar11);
                    FUN_0099be70(uVar13);
                    pCVar8 = (CWideString *)FUN_0099be70(lVar5);
                    CWideString::operator=((CWideString *)local_8,pCVar8);
                    CCharString::~CCharString((CCharString *)&local_44);
                    CCharString::~CCharString(local_4c);
                    CCharString::~CCharString(local_50);
                    CCharString::~CCharString(local_54);
                    CCharString::~CCharString(local_58);
                    CCharString::~CCharString(local_5c);
                    CCharString::~CCharString(local_60);
                    CCharString::~CCharString((CCharString *)local_64);
                    CCharString::~CCharString(local_68);
                    std::
                    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                               local_3c,extraout_EDX_23,ppVar27);
                    std::
                    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                               &local_40,extraout_EDX_24,ppVar27);
                    local_6c = 0;
                    puVar25 = &local_6c;
                    (**(code **)(*local_78 + 0x148))(puVar25);
                    iVar10 = FUN_0042d5b1(puVar25);
                    local_6c = (uint)*(byte *)(iVar10 + 0x18);
                    bVar3 = __ftol2();
                    local_6c = 0;
                    *(byte *)(iVar10 + 0x18) = bVar3;
                    puVar25 = &local_6c;
                    (**(code **)(*local_78 + 0x148))(puVar25);
                    iVar10 = FUN_0042d5b1(puVar25);
                    local_6c = (uint)*(byte *)(iVar10 + 0x19);
                    bVar3 = __ftol2();
                    *(byte *)(iVar10 + 0x19) = bVar3;
                    (**(code **)(*(int *)local_14 + 0xec))(&local_78);
                    (**(code **)(*local_28 + 0x240))(local_8,1,1);
                    this_06 = extraout_ECX_00;
                    piVar12 = local_70;
                  }
                  uVar16 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_06);
                } while (uVar16 != piVar12[0x4f]);
              }
              piVar11 = piVar12 + 1;
              *piVar11 = *piVar11 + -1;
              if (*piVar11 == 0) {
                (**(code **)(*piVar12 + 4))();
              }
            }
          }
        }
      }
      piVar12 = _param_3 + 1;
      *piVar12 = *piVar12 + -1;
      if (*piVar12 == 0) {
        (**(code **)(*_param_3 + 4))();
      }
      if (_param_4 != (CDefClassBase *)0x0) {
        pCVar17 = _param_4 + 4;
        *(int *)pCVar17 = *(int *)pCVar17 + -1;
        if (*(int *)pCVar17 == 0) {
          (**(code **)(*(int *)_param_4 + 4))();
        }
      }
    }
    this_00 = local_24;
    iVar10 = (**(code **)(*(int *)local_24 + 0x54))();
    if (iVar10 != 0x12) goto LAB_005ac097;
    if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
        (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
      pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
    }
    else {
      pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
    }
    CCharString::CCharString((CCharString *)&param_3,pcVar29,-1);
    pCVar4 = NUISystem::CManager::CreateComponent((CManager *)local_c,(CCharString *)&param_3,false)
    ;
    CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
              ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)&local_78,
               (vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
               extraout_EDX_25,ppVar27);
    piVar12 = local_78;
    CWideString::operator=((CWideString *)local_8,(CWideString *)&DAT_013bca24);
    CTCInventoryClothing::GetItemDescriptionNoArmourRating(this_00,(CWideString *)local_8,param_1);
    (**(code **)(*(int *)local_14 + 0xec))(&local_78);
    iVar10 = *piVar12;
  }
  else {
    CCharString::CCharString((CCharString *)&param_3,"TEXT_GUI_PRICE",-1);
    piVar7 = local_28;
    piVar11 = local_28 + 0xf;
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_20,(long)unaff_EDI);
    piVar26 = piVar7 + 0x11;
    uVar13 = extraout_EAX;
    NGameText::CDataBank::GetTextBySymbol(local_18,local_38);
    FUN_0099be70(piVar26);
    FUN_0099be70(piVar11);
    pCVar8 = (CWideString *)FUN_0099be70(uVar13);
    CWideString::operator=((CWideString *)local_8,pCVar8);
    CCharString::~CCharString((CCharString *)&param_4);
    CCharString::~CCharString((CCharString *)&local_40);
    CCharString::~CCharString(local_3c);
    CCharString::~CCharString(local_38);
    CCharString::~CCharString((CCharString *)&local_20);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
               extraout_EDX_05,unaff_EDI);
    piVar11 = piVar7 + 0x14;
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)local_3c,(long)piVar11);
    uVar13 = extraout_EAX_00;
    FUN_0099be70(piVar7 + 0x13);
    FUN_0099be70(uVar13);
    pCVar9 = (CCharString *)FUN_0099be70(piVar11);
    CCharString::operator+=(local_8,pCVar9);
    CCharString::~CCharString((CCharString *)&param_3);
    CCharString::~CCharString((CCharString *)&param_4);
    CCharString::~CCharString((CCharString *)&local_40);
    CCharString::~CCharString(local_3c);
    (**(code **)(*(int *)local_14 + 0xec))(&local_78);
    pCVar9 = local_8;
    (**(code **)(*piVar12 + 0x240))(pCVar9,1,1);
    if (local_1c < (int)local_24) {
      _param_3 = (int *)0x0;
      pbVar22 = &param_3;
      (**(code **)(*local_78 + 0x148))(pbVar22);
      iVar10 = FUN_0042d5b1(pbVar22);
      _param_3 = (int *)(uint)*(byte *)(iVar10 + 0x18);
      bVar3 = __ftol2();
      _param_3 = (int *)0x0;
      *(byte *)(iVar10 + 0x18) = bVar3;
      pbVar22 = &param_3;
      (**(code **)(*local_78 + 0x148))(pbVar22);
      iVar10 = FUN_0042d5b1(pbVar22);
      pbVar20 = (byte *)(iVar10 + 0x19);
      bVar3 = *pbVar20;
LAB_005ab2a7:
      _param_3 = (int *)(uint)bVar3;
      bVar3 = __ftol2();
      *pbVar20 = bVar3;
    }
    else if ((int)local_24 < local_1c) {
      _param_3 = (int *)0x0;
      pbVar22 = &param_3;
      (**(code **)(*local_78 + 0x148))(pbVar22);
      iVar10 = FUN_0042d5b1(pbVar22);
      _param_3 = (int *)(uint)*(byte *)(iVar10 + 0x18);
      bVar3 = __ftol2();
      _param_3 = (int *)0x0;
      *(byte *)(iVar10 + 0x18) = bVar3;
      pbVar22 = &param_3;
      (**(code **)(*local_78 + 0x148))(pbVar22);
      iVar10 = FUN_0042d5b1(pbVar22);
      pbVar20 = (byte *)(iVar10 + 0x1a);
      bVar3 = *pbVar20;
      goto LAB_005ab2a7;
    }
    if (*(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(local_10 + 0x218) ==
        (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>)0x0) {
      pcVar29 = "UI_TEXT_INSIDE_CONTAINER_TEMPLATE";
    }
    else {
      pcVar29 = "PC_TEXT_INSIDE_CONTAINER_TEMPLATE";
    }
    CCharString::CCharString((CCharString *)&param_3,pcVar29,-1);
    pCVar4 = NUISystem::CManager::CreateComponent((CManager *)local_c,(CCharString *)&param_3,false)
    ;
    CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
              ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)&local_78,
               (vector<CCharString,std::allocator<CCharString>_> *)pCVar4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
               extraout_EDX_06,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
    local_28 = local_78;
    uVar21 = Catch_0040d959();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar21,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar21 >> 0x20),
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
    _param_3 = (int *)0x0;
    piVar11 = (int *)CTCInventoryItem::GetInventoryCategory(param_1);
    cVar1 = FUN_00410de0(piVar11);
    piVar12 = _param_3;
    if (cVar1 != '\0') {
      _param_3 = piVar11;
      piVar12 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                       ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                     *)(extraout_EAX_01 + 0x44),(ETCInterfaceType *)&param_3);
      if ((piVar12 == *(int **)(extraout_EAX_01 + 0x48)) || ((int)piVar11 < *piVar12)) {
        piVar12 = *(int **)(extraout_EAX_01 + 0x48);
      }
      piVar12 = (int *)piVar12[1];
    }
    FUN_0099b800(&DAT_0122d70c);
    (**(code **)(*piVar12 + 0x138))(local_8,param_1,1);
    (**(code **)(*(int *)local_14 + 0xec))(&local_78);
    iVar10 = *local_28;
  }
  (**(code **)(iVar10 + 0x240))(local_8,1,1);
LAB_005ac097:
  if (local_2c != (int *)0x0) {
    piVar12 = local_2c + 1;
    *piVar12 = *piVar12 + -1;
    if (*piVar12 == 0) {
      (**(code **)(*local_2c + 4))();
    }
  }
  if (local_30 != (CDefClassBase *)0x0) {
    pCVar17 = local_30 + 4;
    *(int *)pCVar17 = *(int *)pCVar17 + -1;
    if (*(int *)pCVar17 == 0) {
      (**(code **)(*(int *)local_30 + 4))();
    }
    local_30 = (CDefClassBase *)0x0;
  }
  if (piVar7 != (int *)0x0) {
    piVar12 = piVar7 + 1;
    *piVar12 = *piVar12 + -1;
    if (*piVar12 == 0) {
      (**(code **)(*piVar7 + 4))();
    }
  }
  CCharString::~CCharString(local_8);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_78,extraout_EDX_26);
  return local_14;
}



//=== SetThingAsWearingClothingSuit @ 005b3d7a ===

/* [bsim sim=0.7595351391410975 <- ego_r]
   private: void __thiscall CTCInventoryClothing::SetThingAsWearingClothingSuit(class CThing &,long)
    */

void __thiscall
CTCInventoryClothing::SetThingAsWearingClothingSuit
          (CTCInventoryClothing *this,CThing *param_1,long param_2)

{
  CThing *pCVar1;
  int *piVar2;
  CTCHeroAttachableAppearanceModifiers *this_00;
  char cVar3;
  int *piVar4;
  CDefinitionManager *this_01;
  long lVar5;
  ulong uVar6;
  CMemoryAllocatorVariableSize *extraout_ECX;
  CMemoryAllocatorVariableSize *extraout_ECX_00;
  CMemoryAllocatorVariableSize *this_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  long lVar7;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar8;
  
  pCVar1 = param_1;
  cVar3 = FUN_00410de0(0x5e);
  if (cVar3 != '\0') {
    param_1 = (CThing *)0x5e;
    piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(pCVar1 + 0x44),(ETCInterfaceType *)&param_1);
    piVar2 = *(int **)(pCVar1 + 0x48);
    if ((piVar4 == piVar2) || (0x5e < *piVar4)) {
      piVar4 = piVar2;
    }
    this_00 = (CTCHeroAttachableAppearanceModifiers *)piVar4[1];
    CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(this_00,2);
    param_1 = (CThing *)0x0;
    pCVar8 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&param_1;
    lVar7 = param_2;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX,
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_2);
    CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_01,lVar7,pCVar8);
    param_2 = 0;
    CDefClassBase::GetSubDef<CAbilityDef>
              ((CDefClassBase *)param_1,(CDefPointer<CAbilityDef_const_> *)&param_2);
    lVar7 = param_2;
    uVar6 = *(ulong *)(*(ulong *)(param_2 + 0x28) + 8);
    if (uVar6 != *(ulong *)(param_2 + 0x28)) {
      do {
        lVar5 = NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory
                          ((CTCInventoryBase *)this,*(long *)(uVar6 + 0x14));
        this_02 = extraout_ECX;
        if (0 < lVar5) {
          CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace
                    (this_00,*(long *)(uVar6 + 0x14));
          this_02 = extraout_ECX_00;
        }
        uVar6 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_02);
      } while (uVar6 != *(ulong *)(lVar7 + 0x28));
    }
    CTCHeroAttachableAppearanceModifiers::EditFrameUpdate(this_00);
    piVar2 = (int *)(lVar7 + 4);
    *piVar2 = *piVar2 + -1;
    if (*piVar2 == 0) {
      (**(code **)(*(int *)lVar7 + 4))();
    }
    if (param_1 != (CThing *)0x0) {
      pCVar1 = param_1 + 4;
      *(int *)pCVar1 = *(int *)pCVar1 + -1;
      if (*(int *)pCVar1 == 0) {
        (**(code **)(*(int *)param_1 + 4))();
      }
    }
  }
  return;
}



//=== RemoveAllClothing @ 005b40e6 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCInventoryClothing::RemoveAllClothing(void) */

void __thiscall CTCInventoryClothing::RemoveAllClothing(CTCInventoryClothing *this)

{
  CDefinitionManager *this_00;
  long lVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar2;
  CTCInventoryClothing *local_8;
  
  this[0x15c] = (CTCInventoryClothing)0x0;
  local_8 = this;
  CCharString::CCharString((CCharString *)&local_8,"HERO_SUIT_NAKED",-1);
  ppVar2 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_8;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX,
             ppVar2);
  lVar1 = CDefinitionManager::GetDefGlobalIndexFromName(this_00,(CCharString *)ppVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,unaff_EDI);
  NInventory::CTCInventoryBase::SelectItemWithDefIndex((CTCInventoryBase *)this,lVar1);
  SetThingAsWearingSelectedClothes(this,*(CThing **)(this + 4));
  this[0x15c] = (CTCInventoryClothing)0x1;
  return;
}



//=== AddClothingToThing @ 005b43a2 ===

/* WARNING: Removing unreachable block (ram,0x005b4516) */
/* [bsim sim=0.5150639591864332 <- donor]
   public: void __thiscall CTCInventoryClothing::AddClothingToThing(long,class CThing *) */

void __thiscall
CTCInventoryClothing::AddClothingToThing(CTCInventoryClothing *this,long param_1,CThing *param_2)

{
  int *piVar1;
  CThing *pCVar2;
  int *piVar3;
  CDefClassBase *pCVar4;
  CDefClassBase *pCVar5;
  char cVar6;
  bool bVar7;
  int *piVar8;
  CDefinitionManager *this_00;
  CDefinitionManager *this_01;
  long lVar9;
  EWeaponClass *pEVar10;
  ulong uVar11;
  CDefinitionManager *this_02;
  CThing *pCVar12;
  TreeNode *pTVar13;
  int iVar14;
  CMemoryAllocatorVariableSize *extraout_ECX;
  CMemoryAllocatorVariableSize *extraout_ECX_00;
  CMemoryAllocatorVariableSize *extraout_ECX_01;
  CMemoryAllocatorVariableSize *this_03;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar15;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  EWeaponClass *unaff_ESI;
  EWeaponClass *unaff_EDI;
  MedianCut *this_04;
  undefined8 uVar16;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar17;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar18;
  EWeaponClass *local_30;
  undefined4 local_2c;
  ETCInterfaceType local_28;
  int *local_24;
  CDefClassBase *local_20;
  CDefClassBase *local_1c;
  int *local_18;
  CTCHeroAttachableAppearanceModifiers *local_14;
  int *local_10;
  CTCInventoryClothing *local_c;
  int *local_8;
  
  pCVar12 = param_2;
  local_c = this;
  cVar6 = FUN_00410de0();
  if (cVar6 != '\0') {
    local_28 = 0x5e;
    uVar16 = CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
             LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                         *)(pCVar12 + 0x44),&local_28);
    piVar8 = (int *)uVar16;
    if ((piVar8 == *(int **)(pCVar12 + 0x48)) || (0x5e < *piVar8)) {
      piVar8 = *(int **)(pCVar12 + 0x48);
    }
    local_14 = (CTCHeroAttachableAppearanceModifiers *)piVar8[1];
    pCVar18 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_1c;
    local_1c = (CDefClassBase *)0x0;
    lVar9 = param_1;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,
               (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar16 >> 0x20),
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1);
    CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_00,lVar9,pCVar18);
    local_18 = (int *)0x0;
    FUN_0042ab58();
    FUN_00706800();
    bVar7 = CDefClassBase::GetSubDef<CAbilityDef>
                      (local_1c,(CDefPointer<CAbilityDef_const_> *)&local_18);
    if (bVar7) {
      piVar8 = (int *)*local_8;
      local_30 = (EWeaponClass *)0x0;
      local_2c = 0;
      ppVar15 = extraout_EDX;
      if (piVar8 != local_8) {
        do {
          pCVar18 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&param_2;
          ppVar17 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)piVar8[2];
          param_2 = (CThing *)0x0;
          param_1 = 0;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,
                     ppVar15,ppVar17);
          bVar7 = CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>
                            (this_01,(long)ppVar17,pCVar18);
          pCVar12 = param_2;
          ppVar15 = extraout_EDX_00;
          if (bVar7) {
            bVar7 = CDefClassBase::GetSubDef<CAbilityDef>
                              ((CDefClassBase *)param_2,(CDefPointer<CAbilityDef_const_> *)&param_1)
            ;
            lVar9 = param_1;
            ppVar15 = extraout_EDX_01;
            if (bVar7) {
              if (*(char *)(param_1 + 0x44) == '\0') {
                FUN_005bba94();
                ppVar15 = extraout_EDX_03;
              }
              else {
                CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(local_14,piVar8[2]);
                ppVar15 = extraout_EDX_02;
              }
            }
            if (lVar9 != 0) {
              piVar1 = (int *)(lVar9 + 4);
              *piVar1 = *piVar1 + -1;
              if (*piVar1 == 0) {
                (**(code **)(*(int *)lVar9 + 4))();
                ppVar15 = extraout_EDX_04;
              }
            }
          }
          if (pCVar12 != (CThing *)0x0) {
            pCVar2 = pCVar12 + 4;
            *(int *)pCVar2 = *(int *)pCVar2 + -1;
            if (*(int *)pCVar2 == 0) {
              (**(code **)(*(int *)pCVar12 + 4))();
              ppVar15 = extraout_EDX_05;
            }
          }
          piVar8 = (int *)*piVar8;
        } while (piVar8 != local_8);
      }
      piVar8 = local_18;
      uVar11 = *(ulong *)(local_18[10] + 8);
      if (uVar11 != local_18[10]) {
        do {
          lVar9 = NInventory::CTCInventoryBase::GetNumberOfItemsOfTypeInInventory
                            ((CTCInventoryBase *)local_c,*(long *)(uVar11 + 0x14));
          this_03 = extraout_ECX;
          if ((0 < lVar9) &&
             (pEVar10 = std::vector<EWeaponClass,std::allocator<EWeaponClass>_>::
                        _Umove<EWeaponClass*>
                                  ((vector<EWeaponClass,std::allocator<EWeaponClass>_> *)0x0,
                                   (EWeaponClass *)(uVar11 + 0x10),unaff_EDI,unaff_ESI),
             this_03 = extraout_ECX_00, pEVar10 == local_30)) {
            CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace
                      (local_14,*(long *)(uVar11 + 0x14));
            this_03 = extraout_ECX_01;
          }
          uVar11 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_03);
        } while (uVar11 != piVar8[10]);
      }
    }
    else {
      local_10 = (int *)0x0;
      local_28 = 0;
      bVar7 = CDefClassBase::GetSubDef<CAbilityDef>
                        (local_1c,(CDefPointer<CAbilityDef_const_> *)&local_10);
      if (bVar7) {
        local_28 = local_10[0x13];
      }
      piVar8 = (int *)*local_8;
      ppVar15 = extraout_EDX_06;
      if (piVar8 != local_8) {
        do {
          pCVar18 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_20;
          ppVar17 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)piVar8[2];
          local_20 = (CDefClassBase *)0x0;
          local_24 = (int *)0x0;
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,
                     ppVar15,ppVar17);
          bVar7 = CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>
                            (this_02,(long)ppVar17,pCVar18);
          pCVar5 = local_20;
          ppVar15 = extraout_EDX_07;
          if (bVar7) {
            bVar7 = CDefClassBase::GetSubDef<CAbilityDef>
                              (local_20,(CDefPointer<CAbilityDef_const_> *)&local_24);
            piVar1 = local_24;
            ppVar15 = extraout_EDX_08;
            if (bVar7) {
              if (local_24[0x13] == local_28) {
                if ((char)local_24[0x11] == '\0') {
                  piVar8 = local_24 + 1;
                  *piVar8 = *piVar8 + -1;
                  if (*piVar8 == 0) {
                    (**(code **)(*local_24 + 4))();
                  }
                  if (pCVar5 != (CDefClassBase *)0x0) {
                    pCVar4 = pCVar5 + 4;
                    *(int *)pCVar4 = *(int *)pCVar4 + -1;
                    if (*(int *)pCVar4 == 0) {
                      (**(code **)(*(int *)pCVar5 + 4))();
                    }
                  }
                  if (local_10 != (int *)0x0) {
                    piVar8 = local_10 + 1;
                    *piVar8 = *piVar8 + -1;
                    if (*piVar8 == 0) {
                      (**(code **)(*local_10 + 4))();
                    }
                  }
                  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&local_8);
                  if (local_18 == (int *)0x0) goto LAB_005b473b;
                  piVar8 = local_18 + 1;
                  *piVar8 = *piVar8 + -1;
                  if (*piVar8 != 0) goto LAB_005b473b;
                  iVar14 = *local_18;
                  goto LAB_005b4738;
                }
                uVar16 = FUN_00429b02();
                ppVar15 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)
                          ((ulonglong)uVar16 >> 0x20);
                piVar8 = (int *)*(undefined4 *)uVar16;
              }
              else {
                piVar8 = (int *)*piVar8;
              }
            }
            if (piVar1 != (int *)0x0) {
              piVar3 = piVar1 + 1;
              *piVar3 = *piVar3 + -1;
              if (*piVar3 == 0) {
                (**(code **)(*piVar1 + 4))();
                ppVar15 = extraout_EDX_09;
              }
            }
          }
          if (pCVar5 != (CDefClassBase *)0x0) {
            pCVar4 = pCVar5 + 4;
            *(int *)pCVar4 = *(int *)pCVar4 + -1;
            if (*(int *)pCVar4 == 0) {
              (**(code **)(*(int *)pCVar5 + 4))();
              ppVar15 = extraout_EDX_10;
            }
          }
          pCVar12 = param_2;
        } while (piVar8 != local_8);
      }
      CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(local_14,2);
      CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(local_14,param_1);
      if (((*(CThing **)(local_c + 4) == pCVar12) && (local_10 != (int *)0x0)) &&
         ((char)local_10[0x11] == '\0')) {
        this_04 = (MedianCut *)(local_c + 0x134);
        pTVar13 = *(TreeNode **)this_04;
        while (pTVar13 != *(TreeNode **)(local_c + 0x138)) {
          if (*(int *)pTVar13 == local_10[0x13]) {
            iVar14 = **(int **)(pTVar13 + 4);
            CCharString::CCharString((CCharString *)&stack0xffffffbc,"PC_MENU_ENTRY_ARROW",-1);
            pCVar12 = (CThing *)(**(code **)(iVar14 + 0xc))();
            param_2 = pCVar12;
            if (pCVar12 != (CThing *)0x0) {
              CBankFile::GetBankHandle((CBankFile *)&local_30);
              iVar14 = *(int *)pCVar12;
              local_30 = (EWeaponClass *)0x1;
              std::pair<unsigned_long,CAnimationEntry>::pair<unsigned_long,CAnimationEntry>
                        ((pair<unsigned_long,CAnimationEntry> *)&stack0xffffffb4,
                         (pair<unsigned_long,class_CAnimationEntry> *)&local_30);
              (**(code **)(iVar14 + 0xf8))();
              CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&local_2c);
            }
            (**(code **)(**(int **)(pTVar13 + 4) + 0x174))();
            pTVar13 = MedianCut::GetNewTreeNode(this_04);
          }
          else {
            pTVar13 = pTVar13 + 8;
          }
        }
      }
      piVar8 = (int *)*local_8;
      if (piVar8 != local_8) {
        do {
          CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(local_14,piVar8[2])
          ;
          piVar8 = (int *)*piVar8;
        } while (piVar8 != local_8);
      }
      piVar8 = local_18;
      if (local_10 != (int *)0x0) {
        piVar1 = local_10 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*local_10 + 4))();
          piVar8 = local_18;
        }
      }
    }
    CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&local_8);
    if (piVar8 != (int *)0x0) {
      piVar1 = piVar8 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        iVar14 = *piVar8;
LAB_005b4738:
        (**(code **)(iVar14 + 4))();
      }
    }
LAB_005b473b:
    if (local_1c != (CDefClassBase *)0x0) {
      pCVar5 = local_1c + 4;
      *(int *)pCVar5 = *(int *)pCVar5 + -1;
      if (*(int *)pCVar5 == 0) {
        (**(code **)(*(int *)local_1c + 4))();
      }
    }
  }
  return;
}



//=== AddClothing @ 005b4753 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCInventoryClothing::AddClothing(long) */

void __thiscall CTCInventoryClothing::AddClothing(CTCInventoryClothing *this,long param_1)

{
  AddClothingToThing(this,param_1,*(CThing **)(this + 4));
  NInventory::CTCInventoryBase::SelectItemWithDefIndex((CTCInventoryBase *)this,param_1);
  return;
}



//=== AddClothingMenuEntry @ 005b4771 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCInventoryClothing::AddClothingMenuEntry(enum EClothingSuitPart,class
   NUISystem::CComponent *) */

void __thiscall
CTCInventoryClothing::AddClothingMenuEntry
          (CTCInventoryClothing *this,EClothingSuitPart param_1,CComponent *param_2)

{
  EClothingSuitPart local_c;
  CComponent *local_8;
  
  local_c = param_1;
  local_8 = param_2;
  FUN_005bba5b(&local_c);
  return;
}



//=== AddClothingToMannequin @ 005b4ea8 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CTCInventoryClothing::AddClothingToMannequin(long) */

void __thiscall
CTCInventoryClothing::AddClothingToMannequin(CTCInventoryClothing *this,long param_1)

{
  AddClothingToThing(this,param_1,*(CThing **)(this + 0x158));
  return;
}



//=== ConstructClothingListPC @ 005b6a66 ===

/* [bsim sim=0.8405671743583695 <- ego_r]
   private: class NUISystem::CComponent * __thiscall
   CTCInventoryClothing::ConstructClothingListPC(class std::vector<struct std::pair<long,long>,class
   std::allocator<struct std::pair<long,long> > > &,class NUISystem::CViewport *,class
   NUISystem::CComponent *,enum EClothingSuitPart,bool) */

CComponent * __thiscall
CTCInventoryClothing::ConstructClothingListPC
          (CTCInventoryClothing *this,
          vector<std::pair<long,long>,std::allocator<std::pair<long,long>_>_> *param_1,
          CViewport *param_2,CComponent *param_3,EClothingSuitPart param_4,bool param_5)

{
  undefined4 uVar1;
  CComponent *pCVar2;
  int iVar3;
  CRumbleDef *pCVar4;
  CAction *pCVar5;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar7;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_08;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_09;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_10;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_11;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_12;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_13;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_14;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *pCVar8;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_15;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_16;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_17;
  CComponent *pCVar9;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar11;
  int *local_80 [2];
  undefined4 *local_78 [2];
  undefined4 *local_70 [2];
  undefined4 *local_68 [2];
  CAction *local_60;
  CCPPointerInfo *local_5c;
  int *local_58;
  undefined4 local_54;
  CAction *local_50;
  CCPPointerInfo *local_4c;
  undefined4 *local_48 [2];
  undefined4 *local_40 [2];
  CComponent *local_38;
  undefined4 local_34;
  undefined4 local_30;
  uint local_2c;
  CTCInventoryClothing *local_28;
  undefined4 *local_24;
  CCharString local_20 [4];
  CCharString local_1c [4];
  CCharString local_18 [4];
  CCharString local_14 [4];
  CComponent *local_10;
  CCharString local_c [4];
  CManager *local_8;
  
  local_28 = this;
  NInventory::CTCInventoryBase::OpenInventory((CTCInventoryBase *)this);
  local_8 = (CManager *)NUISystem::CFrontEndManager::GetInstance();
  CCharString::CCharString((CCharString *)&local_10,"PC_UI_LIST_CLOTHING_ACTIONS",-1);
  pCVar2 = NUISystem::CManager::CreateComponent(local_8,(CCharString *)&local_10,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_80,(CAction *)pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,extraout_EDX
             ,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_10,"PC_TITLE",-1);
  pCVar2 = NUISystem::CManager::CreateComponent(local_8,(CCharString *)&local_10,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_60,(CAction *)pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,
             extraout_EDX_00,unaff_EDI);
  local_58 = (int *)0x0;
  local_54 = 0;
  CCharString::CCharString((CCharString *)&local_10,"CLOTHING TITLE",-1);
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_10;
  (**(code **)(*(int *)local_60 + 0x14c))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,
             extraout_EDX_01,ppVar11);
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_58,local_60,local_5c);
  CCharString::CCharString(local_c);
  ConstructTitleString(local_28,(CWideString *)local_c);
  (**(code **)(*local_58 + 0x240))();
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_60;
  (**(code **)(*local_80[0] + 0x244))();
  pCVar2 = param_3;
  if (param_3 != (CComponent *)0x0) {
    (**(code **)(*(int *)param_3 + 0x1f0))();
  }
  if (*(int *)param_1 == *(int *)(param_1 + 4)) {
    pCVar9 = (CComponent *)0x0;
  }
  else {
    CCharString::CCharString(local_14,"PC_SUB_LIST",-1);
    local_10 = NUISystem::CManager::CreateComponent(local_8,local_14,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_14,
               extraout_EDX_02,ppVar11);
    CCharString::CCharString((CCharString *)&param_3,"PC_TITLE",-1);
    pCVar9 = NUISystem::CManager::CreateComponent(local_8,(CCharString *)&param_3,false);
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_50,(CAction *)pCVar9);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
               extraout_EDX_03,ppVar11);
    CCharString::CCharString((CCharString *)&param_3,"CLOTHING TITLE",-1);
    ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&param_3;
    (**(code **)(*(int *)local_50 + 0x14c))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
               extraout_EDX_04,ppVar10);
    CCountedPointer<NUISystem::CAction>::ShareData
              ((CCountedPointer<NUISystem::CAction> *)&local_58,local_50,local_4c);
    (**(code **)(*local_58 + 0x240))(local_c);
    pCVar9 = local_10;
    (**(code **)(*(int *)local_10 + 0x244))(&local_50);
    if (pCVar2 != (CComponent *)0x0) {
      (**(code **)(*(int *)pCVar2 + 0x1f0))(local_50);
    }
    (**(code **)(*(int *)pCVar9 + 0x270))(*(int *)(param_1 + 4) - *(int *)param_1 >> 3);
    iVar3 = *(int *)param_1;
    param_3 = (CComponent *)0x0;
    pCVar8 = extraout_EDX_05;
    if (*(int *)(param_1 + 4) - iVar3 >> 3 != 0) {
      local_2c = -(uint)(param_2 != (CViewport *)0x0) & (uint)(param_2 + 0x18);
      do {
        uVar1 = *(undefined4 *)(iVar3 + (int)param_3 * 8);
        local_38 = (CComponent *)0x0;
        local_34 = 0;
        local_30 = uVar1;
        if (param_5 == false) {
          CCharString::CCharString(local_20,"PC_MENU_ENTRY_BRANCH_TEMPLATE",-1);
          FUN_0099b6b0();
          CCharString::CCharString((CCharString *)&stack0xffffff5c,"PC_BUTTON_BASTARD",-1);
          CCharString::CCharString((CCharString *)&stack0xffffff54,"",-1);
          pCVar9 = NUISystem::CManager::CreateMenuEntryComponent
                             (local_8,local_1c,0,0x3f800000,local_20,0);
          CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                    ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                     &local_38,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
          CCharString::~CCharString(local_1c);
          paVar6 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_20;
          ppVar7 = extraout_EDX_07;
        }
        else {
          CCharString::CCharString(local_18,"PC_MENU_ENTRY_TEMPLATE",-1);
          FUN_0099b6b0();
          CCharString::CCharString((CCharString *)&stack0xffffff5c,"PC_BUTTON_BASTARD",-1);
          CCharString::CCharString((CCharString *)&stack0xffffff54,"",-1);
          pCVar9 = NUISystem::CManager::CreateMenuEntryComponent
                             (local_8,&param_2,0,0x3f800000,local_18,0);
          CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_>::Reset
                    ((CCountedPointer<std::vector<CCharString,std::allocator<CCharString>_>_> *)
                     &local_38,(vector<CCharString,std::allocator<CCharString>_> *)pCVar9);
          CCharString::~CCharString((CCharString *)&param_2);
          paVar6 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18;
          ppVar7 = extraout_EDX_06;
        }
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (paVar6,ppVar7,ppVar11);
        pCVar9 = local_38;
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_40,pCVar5);
        *local_40[0] = 0x7d2;
        local_24 = ::operator_new(0xc);
        if (local_24 == (undefined4 *)0x0) {
          local_24 = (undefined4 *)0x0;
        }
        else {
          local_24[1] = 3;
          *local_24 = &PTR_LAB_012366f4;
          local_24[2] = uVar1;
        }
        FUN_0042b646();
        FUN_0053fde0();
        FUN_0053fde0();
        FUN_0053fde0();
        (**(code **)(*(int *)local_38 + 0x10c))();
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_68,pCVar5);
        *local_68[0] = 0x7e1;
        FUN_0053fde0();
        (**(code **)(*(int *)pCVar9 + 0x260))();
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_48,pCVar5);
        *local_48[0] = 0x7d5;
        local_24 = ::operator_new(0xc);
        if (local_24 == (undefined4 *)0x0) {
          local_24 = (undefined4 *)0x0;
        }
        else {
          local_24[1] = 3;
          *local_24 = &PTR_LAB_012366f4;
          local_24[2] = uVar1;
        }
        FUN_0042b646();
        FUN_0053fde0(-(uint)(local_38 != (CComponent *)0x0) & (uint)(local_38 + 0x18));
        FUN_0053fde0(-(uint)(pCVar2 != (CComponent *)0x0) & (uint)(pCVar2 + 0x18));
        (**(code **)(*(int *)pCVar9 + 0x264))();
        (**(code **)(*(int *)pCVar9 + 600))(uVar1);
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_70,pCVar5);
        *local_70[0] = 0x1f;
        FUN_0042b687(&local_30);
        (**(code **)(*(int *)local_38 + 0x10c))(local_70);
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_78,pCVar5);
        *local_78[0] = 0x27;
        (**(code **)(*(int *)local_38 + 0x110))(local_78);
        (**(code **)(*(int *)local_10 + 0x260))(&local_38);
        if (param_5 == false) {
          (**(code **)(*(int *)local_38 + 0xec))(local_80);
        }
        else {
          (**(code **)(*(int *)local_38 + 0x170))();
        }
        AddClothingMenuEntry(local_28,param_4,local_38);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_78,extraout_EDX_08);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_70,extraout_EDX_09);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_48,extraout_EDX_10);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_68,extraout_EDX_11);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_40,extraout_EDX_12);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   &local_38,extraout_EDX_13);
        iVar3 = *(int *)param_1;
        param_3 = param_3 + 1;
        pCVar8 = extraout_EDX_14;
        pCVar9 = local_10;
      } while (param_3 < (CComponent *)(*(int *)(param_1 + 4) - iVar3 >> 3));
    }
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_50,pCVar8);
  }
  CCharString::~CCharString(local_c);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_58,extraout_EDX_15);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_60,extraout_EDX_16);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_80
             ,extraout_EDX_17);
  return pCVar9;
}



//=== ~CTCInventoryClothing @ 005b7049 ===

/* [bsim sim=0.6692330905382543 <- ego_r]
   public: virtual __thiscall CTCInventoryClothing::~CTCInventoryClothing(void) */

void __thiscall CTCInventoryClothing::~CTCInventoryClothing(CTCInventoryClothing *this)

{
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124356c;
  *(undefined ***)(this + 0x130) = &PTR_LAB_01243558;
  if (*(CThing **)(this + 0x158) != (CThing *)0x0) {
    CThing::Kill(*(CThing **)(this + 0x158),false);
  }
  Unwind_00fbdab0();
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x160));
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x14c));
  CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)(this + 0x140));
  if (*(void **)(this + 0x134) != (void *)0x0) {
    free(*(void **)(this + 0x134));
  }
  *(undefined ***)(this + 0x130) = &PTR_LAB_0123a3e0;
  CTCActionUseBuyOrSteal::~CTCActionUseBuyOrSteal((CTCActionUseBuyOrSteal *)this);
  return;
}



//=== ConstructClothingList @ 005b70b8 ===

/* [bsim sim=0.8609266063446643 <- ego_r]
   private: class NUISystem::CComponent * __thiscall
   CTCInventoryClothing::ConstructClothingList(class std::vector<struct std::pair<long,long>,class
   std::allocator<struct std::pair<long,long> > > &,class NUISystem::CViewport *,enum
   EClothingSuitPart,bool) */

CComponent * __thiscall
CTCInventoryClothing::ConstructClothingList
          (CTCInventoryClothing *this,
          vector<std::pair<long,long>,std::allocator<std::pair<long,long>_>_> *param_1,
          CViewport *param_2,EClothingSuitPart param_3,bool param_4)

{
  vector<std::pair<long,long>,std::allocator<std::pair<long,long>_>_> *pvVar1;
  CFrontEndManager *this_00;
  CComponent *pCVar2;
  int iVar3;
  CRumbleDef *pCVar4;
  CAction *pCVar5;
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
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_11;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_12;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_13;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_14;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_15;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_16;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_17;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *pCVar6;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_18;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_19;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_20;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_21;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_22;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_23;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_24;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CComponent *pCVar7;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar8;
  CCountedPointer<NUISystem::CAction> *pCVar9;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar10;
  CCountedPointer<NUISystem::CAction> local_94 [8];
  CCountedPointer<NUISystem::CAction> local_8c [8];
  CCountedPointer<NUISystem::CAction> local_84 [8];
  CCountedPointer<NUISystem::CAction> local_7c [8];
  int *local_74 [2];
  uint local_6c;
  undefined4 *local_68 [2];
  undefined4 *local_60 [2];
  CAction *local_58;
  CCPPointerInfo *local_54;
  CTCInventoryClothing *local_50;
  undefined4 *local_4c [2];
  CCharString local_44 [4];
  CCharString local_40 [4];
  CAction *local_3c;
  CCPPointerInfo *local_38;
  int *local_34;
  undefined4 local_30;
  undefined4 *local_2c [2];
  undefined4 *local_24 [2];
  CCharString local_1c [4];
  undefined4 *local_18;
  CFrontEndManager *local_14;
  CComponent *local_10 [2];
  uint local_8;
  
  local_50 = this;
  this_00 = NUISystem::CFrontEndManager::GetInstance();
  local_14 = this_00;
  CCharString::CCharString((CCharString *)&local_8,"UI_LIST_CLOTHING_ACTIONS",-1);
  pCVar7 = (CComponent *)0x0;
  pCVar2 = NUISystem::CManager::CreateComponent((CManager *)this_00,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_74,(CAction *)pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,extraout_EDX,
             unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_BACK_FOR_LIST",-1);
  pCVar2 = NUISystem::CManager::CreateComponent((CManager *)this_00,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            (local_94,(CAction *)pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,unaff_EDI);
  CCharString::CCharString((CCharString *)&local_8,"UI_SELECT_FOR_LIST",-1);
  pCVar2 = NUISystem::CManager::CreateComponent((CManager *)this_00,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            (local_84,(CAction *)pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_01,unaff_EDI);
  (**(code **)(*local_74[0] + 0x244))();
  pCVar9 = local_84;
  (**(code **)(*local_74[0] + 0x244))();
  CCharString::CCharString((CCharString *)&local_8,"UI_TITLE_NEW",-1);
  pCVar2 = NUISystem::CManager::CreateComponent((CManager *)this_00,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_3c,(CAction *)pCVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_02,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
  local_34 = (int *)0x0;
  local_30 = 0;
  CCharString::CCharString((CCharString *)&local_8,"CLOTHING TITLE",-1);
  ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_8;
  (**(code **)(*(int *)local_3c + 0x14c))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_03,ppVar10);
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_34,local_3c,local_38);
  CCharString::CCharString(local_1c);
  ConstructTitleString(local_50,(CWideString *)local_1c);
  (**(code **)(*local_34 + 0x240))();
  ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_3c;
  (**(code **)(*local_74[0] + 0x244))();
  pvVar1 = param_1;
  if (*(int *)param_1 != *(int *)(param_1 + 4)) {
    CCharString::CCharString(local_44,"UI_SUB_LIST",-1);
    pCVar7 = NUISystem::CManager::CreateComponent((CManager *)local_14,local_44,false);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_44,
               extraout_EDX_04,ppVar10);
    CCharString::CCharString((CCharString *)&param_1,"UI_BACK_FOR_LIST",-1);
    pCVar2 = NUISystem::CManager::CreateComponent
                       ((CManager *)local_14,(CCharString *)&param_1,false);
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              (local_8c,(CAction *)pCVar2);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX_05,ppVar10);
    CCharString::CCharString((CCharString *)&param_1,"UI_SELECT_FOR_LIST",-1);
    pCVar2 = NUISystem::CManager::CreateComponent
                       ((CManager *)local_14,(CCharString *)&param_1,false);
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              (local_7c,(CAction *)pCVar2);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX_06,ppVar10);
    (**(code **)(*(int *)pCVar7 + 0x244))();
    pCVar9 = local_7c;
    (**(code **)(*(int *)pCVar7 + 0x244))();
    CCharString::CCharString((CCharString *)&param_1,"UI_TITLE_NEW",-1);
    pCVar2 = NUISystem::CManager::CreateComponent
                       ((CManager *)local_14,(CCharString *)&param_1,false);
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_58,(CAction *)pCVar2);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX_07,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
    CCharString::CCharString((CCharString *)&param_1,"CLOTHING TITLE",-1);
    ppVar8 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&param_1;
    (**(code **)(*(int *)local_58 + 0x14c))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX_08,ppVar8);
    CCountedPointer<NUISystem::CAction>::ShareData
              ((CCountedPointer<NUISystem::CAction> *)&local_34,local_58,local_54);
    (**(code **)(*local_34 + 0x240))(local_1c,1,1);
    (**(code **)(*(int *)pCVar7 + 0x244))(&local_58);
    (**(code **)(*(int *)pCVar7 + 0x270))(*(int *)(pvVar1 + 4) - *(int *)pvVar1 >> 3);
    iVar3 = *(int *)pvVar1;
    local_8 = 0;
    pCVar6 = extraout_EDX_09;
    if (*(int *)(pvVar1 + 4) - iVar3 >> 3 != 0) {
      local_6c = -(uint)(param_2 != (CViewport *)0x0) & (uint)(param_2 + 0x18);
      do {
        param_1 = *(vector<std::pair<long,long>,std::allocator<std::pair<long,long>_>_> **)
                   (iVar3 + local_8 * 8);
        CCharString::CCharString(local_40,"UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",-1);
        FUN_0099b6b0();
        CCharString::CCharString((CCharString *)&stack0xffffff40,"PC_BUTTON_BASTARD",-1);
        CCharString::CCharString((CCharString *)&stack0xffffff38,"",-1);
        pCVar2 = NUISystem::CManager::CreateMenuEntryComponent
                           ((CManager *)local_14,&param_2,0,0x3f800000,local_40,0);
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_10,(CAction *)pCVar2);
        CCharString::~CCharString((CCharString *)&param_2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_40,
                   extraout_EDX_10,ppVar10);
        pCVar2 = local_10[0];
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_24,pCVar5);
        *local_24[0] = 0x7d2;
        local_18 = ::operator_new(0xc);
        if (local_18 == (undefined4 *)0x0) {
          local_18 = (undefined4 *)0x0;
        }
        else {
          local_18[1] = 3;
          *local_18 = &PTR_LAB_012366f4;
          local_18[2] = param_1;
        }
        FUN_0042b646();
        FUN_0053fde0();
        FUN_0053fde0();
        (**(code **)(*(int *)local_10[0] + 0x10c))();
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_4c,pCVar5);
        *local_4c[0] = 0x7e1;
        FUN_0053fde0();
        (**(code **)(*(int *)pCVar2 + 0x260))();
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_2c,pCVar5);
        *local_2c[0] = 0x7d5;
        local_18 = ::operator_new(0xc);
        if (local_18 == (undefined4 *)0x0) {
          local_18 = (undefined4 *)0x0;
        }
        else {
          local_18[1] = 3;
          *local_18 = &PTR_LAB_012366f4;
          local_18[2] = param_1;
        }
        FUN_0042b646();
        FUN_0053fde0(-(uint)(local_10[0] != (CComponent *)0x0) & (uint)(local_10[0] + 0x18));
        (**(code **)(*(int *)pCVar2 + 0x264))();
        (**(code **)(*(int *)pCVar2 + 600))(param_1);
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_60,pCVar5);
        *local_60[0] = 0x1f;
        FUN_0042b687(&param_1);
        (**(code **)(*(int *)local_10[0] + 0x10c))(local_60);
        pCVar4 = ::operator_new(0x10);
        if (pCVar4 == (CRumbleDef *)0x0) {
          pCVar5 = (CAction *)0x0;
        }
        else {
          pCVar5 = (CAction *)CRumbleDef::CRumbleDef(pCVar4);
        }
        CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                  ((CCountedPointer<NUISystem::CAction> *)local_68,pCVar5);
        *local_68[0] = 0x27;
        (**(code **)(*(int *)local_10[0] + 0x110))(local_68);
        (**(code **)(*(int *)pCVar7 + 0x260))(local_10);
        if (param_4 == false) {
          (**(code **)(*(int *)local_10[0] + 0xec))(local_74);
        }
        else {
          (**(code **)(*(int *)local_10[0] + 0x170))();
        }
        AddClothingMenuEntry(local_50,param_3,local_10[0]);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_68,extraout_EDX_11);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_60,extraout_EDX_12);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_2c,extraout_EDX_13);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_4c,extraout_EDX_14);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_24,extraout_EDX_15);
        std::
        _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                  ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                   local_10,extraout_EDX_16);
        iVar3 = *(int *)pvVar1;
        local_8 = local_8 + 1;
        pCVar6 = extraout_EDX_17;
      } while (local_8 < (uint)(*(int *)(pvVar1 + 4) - iVar3 >> 3));
    }
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_58,pCVar6);
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_7c,extraout_EDX_18);
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               local_8c,extraout_EDX_19);
  }
  CCharString::~CCharString(local_1c);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_34,extraout_EDX_20);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_3c,extraout_EDX_21);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_84
             ,extraout_EDX_22);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_94
             ,extraout_EDX_23);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_74
             ,extraout_EDX_24);
  return pCVar7;
}



//=== IsClothing @ 005d9c60 ===

/* [bsim sim=1.0 <- ego_r]
   public: static bool __fastcall CTCInventoryItem::IsClothing(long) */

bool __fastcall CTCInventoryItem::IsClothing(long param_1)

{
  bool bVar1;
  
  bVar1 = CacheDef(param_1);
  if (bVar1) {
    return *(int *)(DAT_013b8c18 + 0x50) == 0x12;
  }
  return false;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00613210 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 10; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x28;
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00673d30 ===

/* [bsim sim=0.630647447134403 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  CEntry *pCVar4;
  
  if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2) {
    paVar2 = param_1 + 4;
    do {
      if (param_3 != (CEntry *)0x0) {
        *(undefined4 *)param_3 = *(undefined4 *)param_1;
        paVar3 = paVar2;
        pCVar4 = param_3;
        for (iVar1 = 10; pCVar4 = pCVar4 + 4, iVar1 != 0; iVar1 = iVar1 + -1) {
          *(undefined4 *)pCVar4 = *(undefined4 *)paVar3;
          paVar3 = paVar3 + 4;
        }
      }
      param_1 = param_1 + 0x2c;
      paVar2 = paVar2 + 0x2c;
      param_3 = param_3 + 0x2c;
    } while (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2);
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 0067ea30 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 7; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x1c;
  }
  return;
}



//=== ?GetPInventory@CInputProcessInventoryClothing@@MBEPAVCTCInventoryBase@NInventory@@XZ @ 006896b0 ===

undefined4
_GetPInventory_CInputProcessInventoryClothing__MBEPAVCTCInventoryBase_NInventory__XZ(void)

{
  int in_ECX;
  
  return *(undefined4 *)(in_ECX + 0x34);
}



//=== ?ProcessInput@CInputProcessInventoryClothing@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z @ 006896c0 ===

/* [bsim sim=0.7991096703443319 <- ego_r]
   public: virtual void __thiscall CInputProcessHeroAbilitiesScreen::ProcessInput(class CInputEvent
   const &,class CProcessedInput &) */

void __thiscall
CInputProcessHeroAbilitiesScreen::
_ProcessInput_CInputProcessInventoryClothing__UAEXABVCInputEvent__AAVCProcessedInput___Z
          (CInputProcessHeroAbilitiesScreen *this,CInputEvent *param_1,CProcessedInput *param_2)

{
  bool bVar1;
  char extraout_AL;
  char extraout_AL_00;
  char extraout_AL_01;
  char extraout_AL_02;
  char extraout_AL_03;
  char extraout_AL_04;
  char extraout_AL_05;
  char extraout_AL_06;
  CIDrawEnvironment *pCVar2;
  CFrontEndManager *pCVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  undefined4 uVar4;
  float fStack_8;
  float fStack_4;
  
  (**(code **)(*(int *)this + 0x24))();
  pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (pCVar2 == (CIDrawEnvironment *)0x11) {
    CPersistContext::GetDefaultVal<CEngineGraphic>();
    if (0.6 < fStack_4) goto LAB_006897e8;
    if (-0.6 <= fStack_4) {
      if (-0.9 <= fStack_8) {
        if (fStack_8 <= 0.9) {
          return;
        }
        pCVar3 = NUISystem::CFrontEndManager::GetInstance();
        uVar4 = 3;
        goto LAB_006897ef;
      }
LAB_006897b1:
      pCVar3 = NUISystem::CFrontEndManager::GetInstance();
      uVar4 = 2;
      goto LAB_006897ef;
    }
  }
  else {
    bVar1 = CInputProcessGameBase::IsEventButtonHeld((CInputProcessGameBase *)this,param_1);
    if (bVar1) {
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,extraout_EDX,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000025);
      if (extraout_AL == '\0') {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX_00,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000026);
        if (extraout_AL_00 == '\0') {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                     extraout_EDX_01,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000027);
          if ((extraout_AL_01 == '\0') &&
             (std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                         extraout_EDX_02,
                         (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000022),
             extraout_AL_02 == '\0')) {
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                       extraout_EDX_03,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x28);
            if ((extraout_AL_03 == '\0') &&
               (std::
                _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                          ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                           extraout_EDX_04,
                           (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000021)
               , extraout_AL_04 == '\0')) {
              return;
            }
            pCVar3 = NUISystem::CFrontEndManager::GetInstance();
            uVar4 = 3;
            goto LAB_006897ef;
          }
          goto LAB_006897b1;
        }
LAB_006897e8:
        pCVar3 = NUISystem::CFrontEndManager::GetInstance();
        uVar4 = 1;
        goto LAB_006897ef;
      }
    }
    else {
      bVar1 = CInputProcessGameBase::IsEventButtonHeld((CInputProcessGameBase *)this,param_1);
      if (!bVar1) {
        return;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                 extraout_EDX_05,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000025);
      if (extraout_AL_05 == '\0') {
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,
                   extraout_EDX_06,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&DAT_00000026);
        if (extraout_AL_06 == '\0') {
          return;
        }
        goto LAB_006897e8;
      }
    }
  }
  pCVar3 = NUISystem::CFrontEndManager::GetInstance();
  uVar4 = 0;
LAB_006897ef:
  (*(code *)**(undefined4 **)pCVar3)(uVar4);
  NScript::CV_SickChildScript::CWomanToAttract::Init((CWomanToAttract *)param_1);
  return;
}



//=== ?Init@CInputProcessInventoryClothing@@UAEXXZ @ 0068acf0 ===

void _Init_CInputProcessInventoryClothing__UAEXXZ(void)

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
  if ((*(uint *)(extraout_EAX + 0x20) & 0x40000) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x12 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    *(int *)(in_ECX + 0x34) = piVar1[1];
  }
  return;
}



//=== IsWearingAppearanceModifier @ 00706000 ===

/* [bsim sim=0.7494367275698207 <- ego_r]
   public: bool __thiscall
   CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier(long,enum
   EHeroAttachableAppearanceModifierType)const  */

bool __thiscall
CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier
          (CTCHeroAttachableAppearanceModifiers *this,long param_1,
          EHeroAttachableAppearanceModifierType param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  
  puVar1 = (undefined4 *)(*(int *)(this + 0x30) + param_2 * 0xc);
  piVar2 = (int *)*puVar1;
  while( true ) {
    if (piVar2 == (int *)puVar1[1]) {
      return false;
    }
    if (*piVar2 == param_1) break;
    piVar2 = piVar2 + 2;
  }
  return true;
}



//=== AddAppearanceModifier @ 00706370 ===

/* [bsim sim=0.7465165756312842 <- ego_r]
   public: void __thiscall CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(long) */

void __thiscall
CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier
          (CTCHeroAttachableAppearanceModifiers *this,long param_1)

{
  CDefClassBase *pCVar1;
  int *piVar2;
  bool bVar3;
  CDefinitionManager *this_00;
  int *piVar4;
  long lVar5;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  int *piVar6;
  CDefClassBase *pCVar7;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar8;
  CDefClassBase *local_8;
  int *local_4;
  
  pCVar8 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_8;
  piVar6 = (int *)0x0;
  local_8 = (CDefClassBase *)0x0;
  local_4 = (int *)0x0;
  lVar5 = param_1;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1);
  bVar3 = CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_00,lVar5,pCVar8);
  pCVar7 = local_8;
  if ((bVar3) &&
     (bVar3 = CDefClassBase::GetSubDef<CAbilityDef>
                        (local_8,(CDefPointer<CAbilityDef_const_> *)&local_4), piVar6 = local_4,
     bVar3)) {
    piVar2 = (int *)(*(int *)(this + 0x30) + local_4[0x14] * 0xc);
    piVar4 = (int *)*piVar2;
    if (piVar4 != *(int **)(*(int *)(this + 0x30) + 4 + local_4[0x14] * 0xc)) {
      do {
        if (*piVar4 == param_1) {
          piVar4 = (int *)FUN_00708600(piVar4);
        }
        else {
          piVar4 = piVar4 + 2;
        }
      } while (piVar4 != (int *)piVar2[1]);
    }
    this[0x14] = (CTCHeroAttachableAppearanceModifiers)((byte)this[0x14] | 1);
    pCVar7 = local_8;
  }
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  lVar5 = GFGetPreMainMemoryUsed();
  *(long *)(this + 0x10) = lVar5;
  if (piVar6 != (int *)0x0) {
    piVar2 = piVar6 + 1;
    *piVar2 = *piVar2 + -1;
    if (*piVar2 == 0) {
      (**(code **)(*piVar6 + 4))();
    }
  }
  if (pCVar7 != (CDefClassBase *)0x0) {
    pCVar1 = pCVar7 + 4;
    *(int *)pCVar1 = *(int *)pCVar1 + -1;
    if (*(int *)pCVar1 == 0) {
      (**(code **)(*(int *)pCVar7 + 4))();
    }
  }
  return;
}



//=== RemoveAllAppearanceModifiers @ 00706640 ===

/* WARNING: Removing unreachable block (ram,0x00706662) */
/* WARNING: Removing unreachable block (ram,0x00706663) */
/* WARNING: Removing unreachable block (ram,0x00706676) */
/* [bsim sim=0.6479012864685688 <- ego_r]
   public: void __thiscall CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(enum
   EHeroAttachableAppearanceModifierType) */

void __thiscall
CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers
          (CTCHeroAttachableAppearanceModifiers *this,EHeroAttachableAppearanceModifierType param_1)

{
  long lVar1;
  
  *(undefined4 *)(*(int *)(this + 0x30) + param_1 * 0xc + 4) =
       *(undefined4 *)(*(int *)(this + 0x30) + param_1 * 0xc);
  this[0x14] = (CTCHeroAttachableAppearanceModifiers)((byte)this[0x14] | 1);
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  lVar1 = GFGetPreMainMemoryUsed();
  *(long *)(this + 0x10) = lVar1;
  return;
}



//=== RemoveAppearanceModifiersCoveringBodyArea @ 007066a0 ===

/* [bsim sim=0.5595524233324779 <- ego_r]
   public: void __thiscall
   CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea(long,enum
   EHeroAttachableAppearanceModifierType) */

void __thiscall
CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifiersCoveringBodyArea
          (CTCHeroAttachableAppearanceModifiers *this,long param_1,
          EHeroAttachableAppearanceModifierType param_2)

{
  int *piVar1;
  CDefClassBase *pCVar2;
  int *piVar3;
  undefined4 *puVar4;
  bool bVar5;
  undefined4 *_Memory;
  CDefinitionManager *this_00;
  uint uVar6;
  undefined4 *puVar7;
  long lVar8;
  undefined4 *puVar9;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar10;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar11;
  CDefClassBase *local_c;
  int *local_8;
  int *local_4;
  
  piVar3 = (int *)(*(int *)(this + 0x30) + param_2 * 0xc);
  local_4 = piVar3;
  _Memory = malloc(0xc);
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  puVar9 = (undefined4 *)*piVar3;
  if (puVar9 != (undefined4 *)piVar3[1]) {
    do {
      ppVar11 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)&local_c;
      local_c = (CDefClassBase *)0x0;
      local_8 = (int *)0x0;
      ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)*puVar9;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,ppVar11,
                 ppVar10);
      bVar5 = CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>
                        (this_00,(long)ppVar10,
                         (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)ppVar11);
      if (bVar5) {
        bVar5 = CDefClassBase::GetSubDef<CAbilityDef>
                          (local_c,(CDefPointer<CAbilityDef_const_> *)&local_8);
        piVar3 = local_8;
        if (bVar5) {
          uVar6 = local_8[0x10] & param_1;
          if ((uVar6 != 0) &&
             ((((param_2 != 2 || ((param_1 & 0x100U) == 0)) || ((local_8[0x10] & 0x100U) == 0)) ||
              ((uVar6 & 0xfffffeff) != 0)))) {
            puVar7 = malloc(0xc);
            if (puVar7 + 2 != (undefined4 *)0x0) {
              puVar7[2] = *puVar9;
            }
            puVar4 = (undefined4 *)_Memory[1];
            *puVar7 = _Memory;
            puVar7[1] = puVar4;
            *puVar4 = puVar7;
            _Memory[1] = puVar7;
          }
        }
        if (piVar3 != (int *)0x0) {
          piVar1 = piVar3 + 1;
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)(*piVar3 + 4))();
          }
        }
      }
      if (local_c != (CDefClassBase *)0x0) {
        pCVar2 = local_c + 4;
        *(int *)pCVar2 = *(int *)pCVar2 + -1;
        if (*(int *)pCVar2 == 0) {
          (**(code **)(*(int *)local_c + 4))();
        }
      }
      puVar9 = puVar9 + 2;
    } while (puVar9 != (undefined4 *)local_4[1]);
  }
  for (puVar9 = (undefined4 *)*_Memory; puVar9 != _Memory; puVar9 = (undefined4 *)*puVar9) {
    AddAppearanceModifier(this,puVar9[2]);
  }
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  lVar8 = GFGetPreMainMemoryUsed();
  *(long *)(this + 0x10) = lVar8;
  puVar9 = (undefined4 *)*_Memory;
  while (puVar9 != _Memory) {
    puVar7 = (undefined4 *)*puVar9;
    free(puVar9);
    puVar9 = puVar7;
  }
  *_Memory = _Memory;
  _Memory[1] = _Memory;
  free(_Memory);
  return;
}



//=== AddAppearanceModifier @ 00706880 ===

/* [bsim sim=0.747728915186518 <- ego_r]
   public: void __thiscall CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(long) */

void __thiscall
CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier
          (CTCHeroAttachableAppearanceModifiers *this,long param_1)

{
  int *piVar1;
  CDefClassBase *pCVar2;
  int iVar3;
  long *plVar4;
  CDefClassBase *pCVar5;
  bool bVar6;
  CDefinitionManager *this_00;
  long lVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  int *piVar8;
  long lVar9;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar10;
  int *local_c;
  CDefClassBase *local_8;
  long local_4;
  
  lVar7 = param_1;
  pCVar10 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_8;
  piVar8 = (int *)0x0;
  local_8 = (CDefClassBase *)0x0;
  local_c = (int *)0x0;
  lVar9 = param_1;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1);
  bVar6 = CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_00,lVar9,pCVar10);
  pCVar5 = local_8;
  if (bVar6) {
    bVar6 = CDefClassBase::GetSubDef<CAbilityDef>
                      (local_8,(CDefPointer<CAbilityDef_const_> *)&local_c);
    piVar8 = local_c;
    if (bVar6) {
      iVar3 = *(int *)(this + 0x30) + local_c[0x14] * 0xc;
      plVar4 = *(long **)(iVar3 + 4);
      local_8 = (CDefClassBase *)lVar7;
      local_4 = CONCAT31(local_4._1_3_,1);
      if (plVar4 == *(long **)(iVar3 + 8)) {
        FUN_00708e40(plVar4,&local_8,&param_1,1,1);
      }
      else {
        if (plVar4 != (long *)0x0) {
          *plVar4 = lVar7;
          plVar4[1] = local_4;
        }
        *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + 8;
      }
      this[0x14] = (CTCHeroAttachableAppearanceModifiers)((byte)this[0x14] | 1);
    }
  }
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  lVar7 = GFGetPreMainMemoryUsed();
  *(long *)(this + 0x10) = lVar7;
  if (piVar8 != (int *)0x0) {
    piVar1 = piVar8 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar8 + 4))();
    }
  }
  if (pCVar5 != (CDefClassBase *)0x0) {
    pCVar2 = pCVar5 + 4;
    *(int *)pCVar2 = *(int *)pCVar2 + -1;
    if (*(int *)pCVar2 == 0) {
      (**(code **)(*(int *)pCVar5 + 4))();
    }
  }
  return;
}



//=== AddAppearanceModifierWithReplace @ 00706950 ===

/* [bsim sim=0.7243677167210426 <- ego_r]
   public: void __thiscall
   CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace(long) */

void __thiscall
CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace
          (CTCHeroAttachableAppearanceModifiers *this,long param_1)

{
  int *piVar1;
  CDefClassBase *pCVar2;
  int iVar3;
  long *plVar4;
  bool bVar5;
  CDefinitionManager *this_00;
  CDisplayEngine *pCVar6;
  long lVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *in_EDX;
  CDefClassBase *pCVar8;
  int *piVar9;
  long lVar10;
  CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *pCVar11;
  CDefClassBase *local_10;
  int *local_c;
  long local_8;
  undefined4 local_4;
  
  lVar7 = param_1;
  pCVar11 = (CDefPointer<CSpecialAbilitiesDrunkennessDef_const_> *)&local_10;
  piVar9 = (int *)0x0;
  local_10 = (CDefClassBase *)0x0;
  local_c = (int *)0x0;
  lVar10 = param_1;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)this,in_EDX,
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)param_1);
  bVar5 = CDefinitionManager::GetDef<CSpecialAbilitiesDrunkennessDef>(this_00,lVar10,pCVar11);
  pCVar8 = local_10;
  if ((bVar5) &&
     (bVar5 = CDefClassBase::GetSubDef<CAbilityDef>
                        (local_10,(CDefPointer<CAbilityDef_const_> *)&local_c), piVar9 = local_c,
     bVar5)) {
    RemoveAppearanceModifiersCoveringBodyArea(this,local_c[0x10],local_c[0x14]);
    iVar3 = *(int *)(this + 0x30) + piVar9[0x14] * 0xc;
    plVar4 = *(long **)(iVar3 + 4);
    local_c = (int *)lVar7;
    local_8 = CONCAT31(local_8._1_3_,1);
    if (plVar4 == *(long **)(iVar3 + 8)) {
      FUN_00708e40(plVar4,&local_c,&param_1,1,1);
    }
    else {
      if (plVar4 != (long *)0x0) {
        *plVar4 = lVar7;
        plVar4[1] = local_8;
      }
      *(int *)(iVar3 + 4) = *(int *)(iVar3 + 4) + 8;
    }
    this[0x14] = (CTCHeroAttachableAppearanceModifiers)((byte)this[0x14] | 1);
    pCVar8 = local_10;
    if (piVar9[0x14] == 0) {
      if ((*(byte *)(piVar9 + 0x10) & 0x10) != 0) {
        local_c = (int *)0x0;
        local_8 = 0;
        local_4 = 0;
        pCVar6 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
        CMessageEventManager::AddMessage
                  (*(CMessageEventManager **)(pCVar6 + 0x60),0x39,DAT_01375550,(C3DVector *)&local_c
                   ,(CThing *)0x0,(CThing *)0x0,0.0,0.0,lVar7);
      }
      if ((char)piVar9[0x10] < '\0') {
        local_c = (int *)0x0;
        local_8 = 0;
        local_4 = 0;
        pCVar6 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
        CMessageEventManager::AddMessage
                  (*(CMessageEventManager **)(pCVar6 + 0x60),0x3a,DAT_01375550,(C3DVector *)&local_c
                   ,(CThing *)0x0,(CThing *)0x0,0.0,0.0,lVar7);
      }
      pCVar8 = local_10;
      if ((piVar9[0x10] & 0x100U) != 0) {
        local_c = (int *)0x0;
        local_8 = 0;
        local_4 = 0;
        pCVar6 = NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
        CMessageEventManager::AddMessage
                  (*(CMessageEventManager **)(pCVar6 + 0x60),0x3b,DAT_01375550,(C3DVector *)&local_c
                   ,(CThing *)0x0,(CThing *)0x0,0.0,0.0,lVar7);
        pCVar8 = local_10;
      }
    }
  }
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  lVar7 = GFGetPreMainMemoryUsed();
  *(long *)(this + 0x10) = lVar7;
  if (piVar9 != (int *)0x0) {
    piVar1 = piVar9 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar9 + 4))();
    }
  }
  if (pCVar8 != (CDefClassBase *)0x0) {
    pCVar2 = pCVar8 + 4;
    *(int *)pCVar2 = *(int *)pCVar2 + -1;
    if (*(int *)pCVar2 == 0) {
      (**(code **)(*(int *)pCVar8 + 4))();
    }
  }
  return;
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



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 007586e0 ===

/* [bsim sim=0.630647447134403 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  CEntry *pCVar4;
  
  if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2) {
    paVar2 = param_1 + 4;
    do {
      if (param_3 != (CEntry *)0x0) {
        *(undefined4 *)param_3 = *(undefined4 *)param_1;
        paVar3 = paVar2;
        pCVar4 = param_3;
        for (iVar1 = 10; pCVar4 = pCVar4 + 4, iVar1 != 0; iVar1 = iVar1 + -1) {
          *(undefined4 *)pCVar4 = *(undefined4 *)paVar3;
          paVar3 = paVar3 + 4;
        }
      }
      param_1 = param_1 + 0x2c;
      paVar2 = paVar2 + 0x2c;
      param_3 = param_3 + 0x2c;
    } while (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2);
  }
  return;
}



//=== CTCDHeroSuit @ 007b7bc0 ===

/* [bsim sim=0.503414093452605 <- ego_r]
   public: __thiscall CTCDHeroSuit::CTCDHeroSuit(class CThing &) */

CTCDHeroSuit * __thiscall CTCDHeroSuit::CTCDHeroSuit(CTCDHeroSuit *this,CThing *param_1)

{
  void *pvVar1;
  
  ::CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite
            ((CEngineSubPrimitive2DClockSprite *)this,
             (CEngineInternalPrimitive2DClockSprite *)param_1);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0126da94;
  *(undefined4 *)(this + 0xc) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0xc) = pvVar1;
  *(undefined4 *)(this + 0x10) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x10) = pvVar1;
  *(undefined4 *)(this + 0x14) = 0;
  pvVar1 = malloc(0x10);
  *(void **)pvVar1 = pvVar1;
  *(void **)((int)pvVar1 + 4) = pvVar1;
  *(void **)(this + 0x14) = pvVar1;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 0;
  return this;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 0081a270 ===

/* [bsim sim=0.630647447134403 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  CEntry *pCVar4;
  
  if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2) {
    paVar2 = param_1 + 4;
    do {
      if (param_3 != (CEntry *)0x0) {
        *(undefined4 *)param_3 = *(undefined4 *)param_1;
        paVar3 = paVar2;
        pCVar4 = param_3;
        for (iVar1 = 9; pCVar4 = pCVar4 + 4, iVar1 != 0; iVar1 = iVar1 + -1) {
          *(undefined4 *)pCVar4 = *(undefined4 *)paVar3;
          paVar3 = paVar3 + 4;
        }
      }
      param_1 = param_1 + 0x28;
      paVar2 = paVar2 + 0x28;
      param_3 = param_3 + 0x28;
    } while (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2);
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 0081a750 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 9; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x24;
  }
  return;
}



//=== ?MsgOnHeroHairTypeChanged@CGameScriptInterface@@UBE_NW4EClothingCoversArea@@AAVCCharString@@@Z @ 00893c00 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual bool __thiscall CGameScriptInterface::MsgOnHeroHairTypeChanged(enum
   EClothingCoversArea,class CCharString &)const  */

bool __thiscall
CGameScriptInterface::
_MsgOnHeroHairTypeChanged_CGameScriptInterface__UBE_NW4EClothingCoversArea__AAVCCharString___Z
          (CGameScriptInterface *this,EClothingCoversArea param_1,CCharString *param_2)

{
  CMessageEventManager *this_00;
  EClothingCoversArea *pEVar1;
  CMessageEvent *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  EClothingCoversArea *local_10;
  EClothingCoversArea **local_c;
  EClothingCoversArea *local_8;
  EClothingCoversArea **local_4;
  
  pEVar1 = (EClothingCoversArea *)GetMaxWorldFrameForMessages(this);
  local_4 = (EClothingCoversArea **)GetMaxWorldFrameForMessages(this);
  this_00 = *(CMessageEventManager **)(*(int *)(this + 4) + 0x60);
  if (param_1 == 0x10) {
    local_8 = &param_1;
    param_1 = 0x39;
    local_10 = pEVar1;
    local_c = local_4;
    local_4 = &local_10;
    pCVar2 = CMessageEventManager::
             FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>_>
                       (this_00,(CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>
                                 *)&local_8);
    if (pCVar2 != (CMessageEvent *)0x0) {
      CDefinitionManager::GetDefNameFromGlobalIndex
                (*(CDefinitionManager **)(this + 0x10),(long)&local_10);
      CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)&param_1);
      CCharString::operator=(param_2,(CCharString *)&param_1);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX,unaff_EDI);
      return true;
    }
  }
  else {
    local_10 = &param_1;
    local_c = &local_8;
    local_8 = pEVar1;
    if (param_1 == 0x80) {
      param_1 = 0x3a;
      pCVar2 = CMessageEventManager::
               FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>_>
                         (this_00,(CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>
                                   *)&local_10);
      if (pCVar2 != (CMessageEvent *)0x0) {
        CDefinitionManager::GetDefNameFromGlobalIndex
                  (*(CDefinitionManager **)(this + 0x10),(long)&local_10);
        CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)&param_1);
        CCharString::operator=(param_2,(CCharString *)&param_1);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                   extraout_EDX_00,unaff_EDI);
        return true;
      }
    }
    else {
      param_1 = 0x3b;
      pCVar2 = CMessageEventManager::
               FindMostRecentMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>_>
                         (this_00,(CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_CanBeSeenOrHeard>
                                   *)&local_10);
      if (pCVar2 != (CMessageEvent *)0x0) {
        CDefinitionManager::GetDefNameFromGlobalIndex
                  (*(CDefinitionManager **)(this + 0x10),(long)&local_10);
        CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)&param_1);
        CCharString::operator=(param_2,(CCharString *)&param_1);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                   extraout_EDX_01,unaff_EDI);
        return true;
      }
    }
  }
  return false;
}



//=== ?IsWearingClothingItem@CGameScriptInterface@@UBE_NABVCScriptThing@@ABVCCharString@@@Z @ 0089b2b0 ===

/* [bsim sim=0.9272545149879791 <- ego_r]
   public: virtual bool __thiscall CGameScriptInterface::IsWearingClothingItem(class CScriptThing
   const &,class CCharString const &)const  */

bool __thiscall
CGameScriptInterface::
_IsWearingClothingItem_CGameScriptInterface__UBE_NABVCScriptThing__ABVCCharString___Z
          (CGameScriptInterface *this,CScriptThing *param_1,CCharString *param_2)

{
  CTCHeroAttachableAppearanceModifiers *this_00;
  CScriptThing *pCVar1;
  bool bVar2;
  int iVar3;
  int *piVar4;
  long lVar5;
  
  pCVar1 = param_1;
  iVar3 = (**(code **)(*(int *)param_1 + 0x2c))();
  if ((iVar3 != 0) && ((*(byte *)(iVar3 + 0x91) & 1) == 0)) {
    iVar3 = (**(code **)(*(int *)pCVar1 + 0x2c))();
    if ((*(uint *)(iVar3 + 0x28) & 0x40000000) != 0) {
      param_1 = (CScriptThing *)0x5e;
      piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar3 + 0x44),(ETCInterfaceType *)&param_1);
      if ((piVar4 == *(int **)(iVar3 + 0x48)) || (0x5e < *piVar4)) {
        piVar4 = *(int **)(iVar3 + 0x48);
      }
      this_00 = (CTCHeroAttachableAppearanceModifiers *)piVar4[1];
      lVar5 = CDefinitionManager::GetDefGlobalIndexFromName
                        (*(CDefinitionManager **)(this + 0x10),param_2);
      if (0 < lVar5) {
        bVar2 = CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifier(this_00,lVar5,2);
        return bVar2;
      }
    }
  }
  return false;
}



//=== ?RemoveHeroClothing@CGameScriptInterface@@UBEXXZ @ 0089b390 ===

void _RemoveHeroClothing_CGameScriptInterface__UBEXXZ(void)

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
     ((*(uint *)(extraout_EAX + 0x20) & 0x40000) != 0)) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x12 < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    CTCInventoryClothing::RemoveAllClothing((CTCInventoryClothing *)piVar1[1]);
  }
  return;
}



//=== WifeClothingUpdate @ 00985f40 ===

/* [bsim sim=0.9356308337378697 <- ego_r]
   private: bool __thiscall CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate(enum
   EOpinionAttitudeType) */

bool __thiscall
CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate
          (CAIStateGroup_OpinionReactionRoot *this,EOpinionAttitudeType param_1)

{
  int iVar1;
  CTCWife *this_00;
  bool bVar2;
  int *piVar3;
  EAIActionReturn EVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CCharString local_8 [4];
  ETCInterfaceType local_4;
  
  iVar1 = *(int *)(*(int *)(this + 4) + 0x18);
  if ((*(uint *)(iVar1 + 0x38) & 0x800000) != 0) {
    local_4 = 0xd7;
    piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),&local_4);
    if ((piVar3 == *(int **)(iVar1 + 0x48)) || (0xd7 < *piVar3)) {
      piVar3 = *(int **)(iVar1 + 0x48);
    }
    this_00 = (CTCWife *)piVar3[1];
    bVar2 = CTCWife::GetAppearanceChangedNoticed(this_00,&local_4);
    if (bVar2) {
      CCharString::CCharString(local_8,(CCharString *)(&DAT_013bab40 + local_4 * 4));
      CPlayer::GetCurrentMode((CPlayer *)this);
      CCharString::operator+=(local_8,(CCharString *)(&DAT_013babd0 + param_1 * 4));
      EVar4 = SayToThingReactingTo(this,local_8);
      if (EVar4 == 2) {
        CTCWife::ClearAppearanceChangedNoticed(this_00,&local_4);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,
                   extraout_EDX_00,unaff_EDI);
        return true;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_8,
                 extraout_EDX,unaff_EDI);
    }
  }
  return false;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 009fffc0 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 0xb; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x2c;
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00a32060 ===

/* [bsim sim=0.630647447134403 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  CEntry *pCVar4;
  
  if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2) {
    paVar2 = param_1 + 4;
    do {
      if (param_3 != (CEntry *)0x0) {
        *(undefined4 *)param_3 = *(undefined4 *)param_1;
        paVar3 = paVar2;
        pCVar4 = param_3;
        for (iVar1 = 0xd; pCVar4 = pCVar4 + 4, iVar1 != 0; iVar1 = iVar1 + -1) {
          *(undefined4 *)pCVar4 = *(undefined4 *)paVar3;
          paVar3 = paVar3 + 4;
        }
      }
      param_1 = param_1 + 0x38;
      paVar2 = paVar2 + 0x38;
      param_3 = param_3 + 0x38;
    } while (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)param_2);
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00ab1410 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 0xd; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x34;
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



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00b497b0 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 0x1c; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x70;
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00b6bba0 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x38;
  }
  return;
}



//=== _Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&> @ 00b7afa0 ===

/* [bsim sim=0.6902240017849421 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct
   CAppearanceModifierGraphics::CEntry>,struct CAppearanceModifierGraphics::CEntry,struct
   CAppearanceModifierGraphics::CEntry &>(class std::allocator<struct
   CAppearanceModifierGraphics::CEntry> &,struct CAppearanceModifierGraphics::CEntry *,struct
   CAppearanceModifierGraphics::CEntry &) */

void __fastcall
std::
_Cons_val<std::allocator<CAppearanceModifierGraphics::CEntry>,CAppearanceModifierGraphics::CEntry,CAppearanceModifierGraphics::CEntry&>
          (allocator<CAppearanceModifierGraphics::CEntry> *param_1,CEntry *param_2,CEntry *param_3)

{
  int iVar1;
  CEntry *pCVar2;
  allocator<CAppearanceModifierGraphics::CEntry> *paVar3;
  
  for (; param_2 != (CEntry *)0x0; param_2 = param_2 + -1) {
    if (param_1 != (allocator<CAppearanceModifierGraphics::CEntry> *)0x0) {
      pCVar2 = param_3;
      paVar3 = param_1;
      for (iVar1 = 0xc; iVar1 != 0; iVar1 = iVar1 + -1) {
        *(undefined4 *)paVar3 = *(undefined4 *)pCVar2;
        pCVar2 = pCVar2 + 4;
        paVar3 = paVar3 + 4;
      }
    }
    param_1 = param_1 + 0x30;
  }
  return;
}



//=== _Cons_val<std::allocator<std::pair<EClothingSuitPart_const_,long>_>,std::pair<EClothingSuitPart_const_,long>,std::pair<EClothingSuitPart,long>&> @ 00c19680 ===

/* [bsim sim=0.6048866950844456 <- ego_r]
   void __fastcall std::_Cons_val<class std::allocator<struct std::pair<enum EClothingSuitPart const
   ,long> >,struct std::pair<enum EClothingSuitPart const ,long>,struct std::pair<enum
   EClothingSuitPart,long> &>(class std::allocator<struct std::pair<enum EClothingSuitPart const
   ,long> > &,struct std::pair<enum EClothingSuitPart const ,long> *,struct std::pair<enum
   EClothingSuitPart,long> &) */

void __fastcall
std::
_Cons_val<std::allocator<std::pair<EClothingSuitPart_const_,long>_>,std::pair<EClothingSuitPart_const_,long>,std::pair<EClothingSuitPart,long>&>
          (allocator<std::pair<EClothingSuitPart_const_,long>_> *param_1,
          pair<EClothingSuitPart_const_,long> *param_2,pair<EClothingSuitPart,long> *param_3)

{
  return;
}



//=== AddClothing @ 00c2c7b0 ===

/* [bsim sim=0.8043379507066797 <- donor]
   public: void __thiscall CTCInventoryClothing::AddClothing(long) */

void __thiscall CTCInventoryClothing::AddClothing(CTCInventoryClothing *this,long param_1)

{
  std::pair<CCharString,C3DMeshVolume>::pair<CCharString,C3DMeshVolume>
            (*(pair<CCharString,C3DMeshVolume> **)(this + 4),
             (pair<class_CCharString,class_C3DMeshVolume> *)param_1);
  FUN_00c2c670(param_1);
  return;
}



//=== ~CTCInventoryClothing @ 00c6a780 ===

/* [bsim sim=0.5450188053587928 <- ego_r]
   public: virtual __thiscall CTCInventoryClothing::~CTCInventoryClothing(void) */

void __thiscall CTCInventoryClothing::~CTCInventoryClothing(CTCInventoryClothing *this)

{
  pair<CCharString,CScriptInfo> *extraout_EDX;
  CMemoryDataInputStream *unaff_EDI;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_012166ca;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)this = &PTR__vector_deleting_destructor__012b9594;
  local_4 = 9;
  this[0x294] = (CTCInventoryClothing)0x0;
  if (*(CBase **)(this + 0x280) != (CBase *)0x0) {
    CTCHeroStats::InformOfEvent((CTCHeroStats *)this,*(CBase **)(this + 0x280));
    *(undefined4 *)(this + 0x280) = 0;
  }
  CLandscapeBackgroundPatch::ClearTextures((CLandscapeBackgroundPatch *)this);
  NScript::CQ_GuildTrainingSkillScript::Init((CQ_GuildTrainingSkillScript *)(this + 0x25c));
  local_4._0_1_ = 8;
  CPersistTraits<NInventory::CItem>::TransferBinaryIn
            ((CPersistTraits<NInventory::CItem> *)(this + 0x25c),unaff_EDI);
  local_4._0_1_ = 7;
  *(undefined ***)(this + 0x248) = &PTR__vector_deleting_destructor__012b957c;
  FUN_00c74450();
  local_4._0_1_ = 6;
  _eh_vector_destructor_iterator_(this + 0x1f8,0x14,4,thunk_FUN_00c74450);
  local_4._0_1_ = 5;
  *(undefined ***)(this + 0x1e0) = &PTR__vector_deleting_destructor__012b9580;
  FUN_00c74450();
  local_4._0_1_ = 4;
  NScript::CV_TourGuideScript::WaypointData::~WaypointData((WaypointData *)(this + 0x144));
  local_4._0_1_ = 3;
  std::
  _Dest_val<std::allocator<std::pair<CCharString,CScriptInfo>_>,std::pair<CCharString,CScriptInfo>_>
            ((allocator<std::pair<CCharString,CScriptInfo>_> *)(this + 0x94),extraout_EDX);
  local_4._0_1_ = 2;
  *(undefined ***)(this + 0x80) = &PTR__vector_deleting_destructor__012b9578;
  FUN_00c74450();
  local_4 = CONCAT31(local_4._1_3_,1);
  *(undefined ***)(this + 0x6c) = &PTR__vector_deleting_destructor__012b9578;
  FUN_00c74450();
  *(undefined ***)(this + 0x28) = &PTR_LAB_012b89c4;
  local_4 = 0xffffffff;
  CTCInventoryItem::OnDie((CTCInventoryItem *)this);
  ExceptionList = local_c;
  return;
}



