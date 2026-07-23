//=== CControlsDef @ 004330f0 ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CControlsDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x49));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4a));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4b));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d));
  return;
}



//=== CFrontEndDef @ 004331c0 ===

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
_Transfer_CFrontEndDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CCreatureAction_FireMissileWeapon *param_2)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_2);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_2,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_2,(CEngineCamera *)(this + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_2,(CEngineCamera *)(this + 0x50));
  return;
}



//=== CEngineDef @ 00433fe0 ===

/* [bsim sim=0.7377739590628846 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CEngineDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  return;
}



//=== CEngineVideoOptionsDef @ 00434120 ===

/* [bsim sim=0.7218905769583901 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CEngineVideoOptionsDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6f));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x71));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x72));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x73));
  return;
}



//=== CConfigOptionsDefaultsDef @ 00434290 ===

/* [bsim sim=0.7534714130295807 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CConfigOptionsDefaultsDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  return;
}



//=== CThingMultiArrowShotDef @ 00451ca2 ===

/* [bsim sim=0.7746988183602541 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CThingMultiArrowShotDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  return;
}



//=== CInventoryItemDef @ 00451d28 ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CInventoryItemDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25));
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x61));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  return;
}



//=== CPhysicsDef @ 0045212a ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CPhysicsDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  return;
}



//=== CTCNoiseDef @ 00452184 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CTCNoiseDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x30));
  return (bool)extraout_AL;
}



//=== CCarrySlotDef @ 00452256 ===

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
_Transfer_CCarrySlotDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x34));
  return;
}



//=== CSmashableDef @ 004522c6 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CSmashableDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x25));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== CHeroMarriageDef @ 00452398 ===

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



//=== CSpecialAbilitiesDrainLifeDataDef @ 00452593 ===

void _Transfer_CSpecialAbilitiesDrainLifeDataDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CSpecialAbilitiesForcePushDataDef @ 004525a3 ===

void _Transfer_CSpecialAbilitiesForcePushDataDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CCreatureStatsDef @ 004525e9 ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCreatureStatsDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CCoopSpiritDef @ 0045266f ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CCoopSpiritDef__UAEXAAVCPersistContext___Z(CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  return;
}



//=== CContextSensitiveItemDef @ 004526b3 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CContextSensitiveItemDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CBonusItemDef @ 0045279b ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CBonusItemDef__UAEXAAVCPersistContext___Z(void)

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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== CHeroTitleDef @ 00452884 ===

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



//=== CTattooDef @ 004529ae ===

/* [bsim sim=0.7477622976889651 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTattooDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  FUN_00431143(in_ECX + 0x28);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  FUN_00431143(in_ECX + 0x4c);
  FUN_00431143(in_ECX + 0x50);
  return;
}



//=== CHairCardDef @ 00452b04 ===

void _Transfer_CHairCardDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CCombatAbilityAttackBase @ 00452b14 ===

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
_Transfer_CCombatAbilityAttackBase__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x41));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  NUISystem::CScrollingComponent::ChangeState((CScrollingComponent *)param_2,(ulong)(this + 0x28));
  return;
}



//=== CCombatAbilityStrafeDef @ 00452b93 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CCombatAbilityStrafeDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  NUISystem::CScrollingComponent::ChangeState((CScrollingComponent *)param_1,(ulong)(this + 0x28));
  return (bool)extraout_AL;
}



//=== CCombatAbilityUseProjectileWeaponDef @ 00452bb4 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CCombatAbilityUseProjectileWeaponDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x44));
  NUISystem::CScrollingComponent::ChangeState((CScrollingComponent *)param_1,(ulong)(this + 0x28));
  return (bool)extraout_AL;
}



//=== CDragonActionSwoopDef @ 00452be2 ===

/* [bsim sim=0.7477622976889651 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CDragonActionSwoopDef__UAEXAAVCPersistContext___Z
          (void)

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
  return;
}



//=== CDragonActionNapalmDef @ 00452c94 ===

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
_Transfer_CDragonActionNapalmDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x34));
  return;
}



//=== CDragonActionHoverDef @ 00452ccd ===

void _Transfer_CDragonActionHoverDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CAugmentationDef @ 00452cdd ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CAugmentationDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CAbilityDef @ 00452df1 ===

void _Transfer_CAbilityDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CJackDragonDef @ 00452ea5 ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CJackDragonDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  return;
}



//=== CBriarRoseDef @ 00452f8e ===

/* [bsim sim=0.7746988183602541 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CBriarRoseDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CSnowTrollDef @ 00453014 ===

/* [bsim sim=0.7534714130295807 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CSnowTrollDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CGuildMasterDef @ 004530bb ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CGuildMasterDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== CSummonerDef @ 004530dc ===

/* [bsim sim=0.7597760295477823 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CSummonerDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CExplosiveTrailDef @ 00453178 ===

void _Transfer_CExplosiveTrailDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  CEngineCamera *pCVar1;
  undefined4 uVar2;
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x40));
  pCVar1 = (CEngineCamera *)(in_ECX + 0x44);
  CEngineLightingManager::UpdateShadowScene(param_1,pCVar1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x48));
  if (0.0 <= *(float *)pCVar1) {
    if (*(float *)pCVar1 <= 1.0) {
      return;
    }
    uVar2 = 0x3f800000;
  }
  else {
    uVar2 = 0;
  }
  *(undefined4 *)pCVar1 = uVar2;
  return;
}



//=== CThingBuildingDef @ 00454b8e ===

/* [bsim sim=0.6904191417984273 <- ego_r]
   public: virtual void __thiscall CThingBuildingDef::Transfer(class CPersistContext &) */

void __thiscall
CThingBuildingDef::_Transfer_CThingBuildingDef__UAEXAAVCPersistContext___Z
          (CThingBuildingDef *this,CPersistContext *param_1)

{
  CPersistContext *this_00;
  undefined4 uStack_8;
  
  this_00 = param_1;
  uStack_8 = this;
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x70));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x74));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x78));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x88));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x8c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x9c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x90));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x94));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x98));
  param_1 = (CPersistContext *)(CONCAT13(this[0xa0],param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingBuildingDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"UseHighDetailQuadTree",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0xa0] = (CThingBuildingDef)((byte)this[0xa0] ^ ((byte)this[0xa0] ^ param_1._3_1_) & 1);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0xa0] >> 1,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingBuildingDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsSelfIlluminating",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  uStack_8 = (CThingBuildingDef *)((uint)uStack_8 & 0xffffff);
  this[0xa0] = (CThingBuildingDef)((byte)this[0xa0] ^ (param_1._3_1_ << 1 ^ (byte)this[0xa0]) & 2);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0xa0] >> 2,param_1._0_3_) & 0x1ffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsPartOfHeroGuild",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0xa0] = (CThingBuildingDef)((byte)this[0xa0] ^ (param_1._3_1_ << 2 ^ (byte)this[0xa0]) & 4);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0xa0] >> 3,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingBuildingDef *)CONCAT13(1,(undefined3)uStack_8);
  CPersistContext::Transfer<signed_char>
            (this_00,"HasWeatherMask",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0xa0] = (CThingBuildingDef)((byte)this[0xa0] ^ (param_1._3_1_ << 3 ^ (byte)this[0xa0]) & 8);
  return;
}



//=== CThingHolySiteDef @ 00454dcb ===

void _Transfer_CThingHolySiteDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x7e));
  return;
}



//=== CThingMarkerDef @ 00454e21 ===

void _Transfer_CThingMarkerDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x84));
  return;
}



//=== CThingSwitchDef @ 00454f15 ===

void _Transfer_CThingSwitchDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== CThingVillageDef @ 00454f5f ===

void _Transfer_CThingVillageDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== CThingPhysicalSwitchDef @ 00454fc3 ===

void _Transfer_CThingPhysicalSwitchDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x82));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x90));
  return;
}



//=== CInventoryCategoryDef @ 00455588 ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CInventoryCategoryDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x45));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x46));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  return;
}



//=== CMessageEventDef @ 004556ce ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CMessageEventDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x48));
  return;
}



//=== CEnvironmentDef @ 004558df ===

/* [bsim sim=0.6550307215668234 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CEnvironmentDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  FUN_00431143(in_ECX + 0x3c);
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xfc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x104));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x108));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x110));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x11c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x130));
  return;
}



//=== CThingNoiseDef @ 00455f1a ===

void _Transfer_CThingNoiseDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CPersistContext::TransferSubComponent<CTCNoiseDef>(param_1,"Noise",(CTCNoiseDef *)(in_ECX + 0x70))
  ;
  return;
}



//=== CCameraModeDef @ 00456564 ===

/* [bsim sim=0.6781623563618424 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCameraModeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x61));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x99));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  return;
}



//=== CCameraManagerSetDef @ 00456818 ===

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
_Transfer_CCameraManagerSetDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x48));
  return;
}



//=== CHeroPostcardGeneratorDef @ 00456883 ===

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



//=== CExpressionDef @ 004569a7 ===

/* [bsim sim=0.7096103603605007 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CExpressionDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x85));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x86));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  return;
}



//=== CCombatDialogueDef @ 00456dfb ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCombatDialogueDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  return;
}



//=== CVillagerInteractionsDef @ 00456f27 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CVillagerInteractionsDef__UAEXAAVCPersistContext___Z
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



//=== CSpecialAbilitiesGhostSwordDef @ 00457a0d ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesGhostSwordDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  return;
}



//=== CSpecialAbilitiesDrunkennessDef @ 00457f13 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CSpecialAbilitiesDrunkennessDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x4c));
  return;
}



//=== CSpecialAbilitiesThunderLightningStormDef @ 00457f91 ===

/* [bsim sim=0.7746988183602541 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesThunderLightningStormDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  return;
}



//=== CHitLocationDef @ 004585fe ===

/* [bsim sim=0.6527649716567461 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CHitLocationDef__UAEXAAVCPersistContext___Z(void)

{
  CEngineLightingManager *this;
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  this = in_stack_00000004;
  FUN_00458833(in_ECX + 0x3c);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  in_stack_00000004 = *(CEngineLightingManager **)(in_ECX + 0x68);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)&stack0x00000004);
  *(CEngineLightingManager **)(in_ECX + 0x68) = in_stack_00000004;
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x6e));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x6f));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x6d));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x75));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x78));
  return;
}



//=== CLightningDef @ 00458911 ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CLightningDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  return;
}



//=== CMeleeCombatKnockdownEffects @ 00458c9d ===

/* [bsim sim=0.7746988183602541 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CMeleeCombatKnockdownEffects__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x51));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x59));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5a));
  return;
}



//=== CThingShotDef @ 00458d6f ===

/* [bsim sim=0.6895265977087581 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CThingShotDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x81));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x82));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x89));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x91));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa9));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xaa));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xab));
  return;
}



//=== CThingObjectDef @ 0045912f ===

/* [bsim sim=0.6517343712563446 <- ego_r]
   public: virtual void __thiscall CThingObjectDef::Transfer(class CPersistContext &) */

void __thiscall
CThingObjectDef::_Transfer_CThingObjectDef__UAEXAAVCPersistContext___Z
          (CThingObjectDef *this,CPersistContext *param_1)

{
  int iVar1;
  CPersistContext *this_00;
  undefined4 uStack_8;
  
  this_00 = param_1;
  uStack_8 = this;
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  iVar1 = *(int *)(this_00 + 0x18);
  if ((((iVar1 != 2) && (iVar1 != 3)) && (iVar1 != 1)) && (*(float *)(this + 0x50) == 0.0)) {
    *(undefined4 *)(this + 0x50) = 0x3f800000;
  }
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x70));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x80));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x84));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x88));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x8c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x90));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x94));
  param_1 = (CPersistContext *)(CONCAT13(this[0x98],param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsFragile",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x98] = (CThingObjectDef)((byte)this[0x98] ^ ((byte)this[0x98] ^ param_1._3_1_) & 1);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x98] >> 1,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsClothing",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x98] = (CThingObjectDef)((byte)this[0x98] ^ (param_1._3_1_ << 1 ^ (byte)this[0x98]) & 2);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x98] >> 2,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsSelfIlluminating",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x98] = (CThingObjectDef)((byte)this[0x98] ^ (param_1._3_1_ << 2 ^ (byte)this[0x98]) & 4);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x98] >> 3,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"UseHighDetailQuadTree",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x98] = (CThingObjectDef)((byte)this[0x98] ^ (param_1._3_1_ << 3 ^ (byte)this[0x98]) & 8);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x98] >> 4,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CThingObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"Electrocutable",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x98] = (CThingObjectDef)((byte)this[0x98] ^ (param_1._3_1_ << 4 ^ (byte)this[0x98]) & 0x10);
  return;
}



//=== CCreatureAbilityDef @ 00459353 ===

void _Transfer_CCreatureAbilityDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== CThingGroupDef @ 0045954d ===

void _Transfer_CThingGroupDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== CPlayerDef @ 00459582 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CPlayerDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  return (bool)extraout_AL;
}



//=== CPlayerMovementDef @ 00459615 ===

/* [bsim sim=0.7425545198594122 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CPlayerMovementDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  return;
}



//=== CSimBuildingDef @ 00459704 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CSimBuildingDef__UAEXAAVCPersistContext___Z
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



//=== CEngineThemeGroupDef @ 00459764 ===

void _Transfer_CEngineThemeGroupDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== CEngineThemeDef @ 0045979b ===

/* [bsim sim=0.5148157476993952 <- donor]
   public: virtual void __thiscall CEngineThemeDef::Transfer(class CPersistContext &) */

void __thiscall
CEngineThemeDef::_Transfer_CEngineThemeDef__UAEXAAVCPersistContext___Z
          (CEngineThemeDef *this,CPersistContext *param_1)

{
  CPersistContext *this_00;
  undefined4 uStack_8;
  
  this_00 = param_1;
  uStack_8 = this;
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x50));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x54));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x58));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x5c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x60));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 100));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x68));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x7c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x80));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x84));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x88));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x8c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x90));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x78));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x94));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x98));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x9c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x6c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x70));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x74));
  param_1 = (CPersistContext *)(CONCAT13(this[0xa0],param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineThemeDef *)CONCAT13(1,(undefined3)uStack_8);
  CPersistContext::Transfer<signed_char>
            (this_00,"Passable",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0xa0] = (CEngineThemeDef)((byte)this[0xa0] ^ ((byte)this[0xa0] ^ param_1._3_1_) & 1);
  return;
}



//=== CSpecialEffectsDef @ 0045c8c3 ===

void _Transfer_CSpecialEffectsDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_0045c8d3(in_ECX + 0x28);
  return;
}



//=== CTargetingDef @ 0045d923 ===

/* [bsim sim=0.7044046192807111 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTargetingDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x39));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b));
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  return;
}



//=== COpinionReactionMaskDef @ 0045de67 ===

void _Transfer_COpinionReactionMaskDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  CMemoryAllocatorVariableSize *pCVar1;
  CMemoryAllocatorVariableSize *pCVar2;
  CMemoryAllocatorVariableSize *this;
  CMemoryAllocatorVariableSize *pCVar3;
  int in_ECX;
  CMemoryAllocatorVariableSize *extraout_ECX;
  CMemoryAllocatorVariableSize *extraout_ECX_00;
  CMemoryAllocatorVariableSize *pCVar4;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x39));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  pCVar1 = *(CMemoryAllocatorVariableSize **)(in_ECX + 0x3c);
  pCVar3 = *(CMemoryAllocatorVariableSize **)(pCVar1 + 8);
  this = extraout_ECX;
  while (pCVar2 = pCVar3, pCVar2 != pCVar1) {
    pCVar4 = pCVar2;
    pCVar3 = (CMemoryAllocatorVariableSize *)CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this)
    ;
    this = pCVar4;
    if (*(int *)(pCVar2 + 0x10) < 0x12) {
      FUN_0045deba(pCVar2);
      this = extraout_ECX_00;
    }
  }
  return;
}



//=== COpinionDeedMaskDef @ 0045e2e9 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_COpinionDeedMaskDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x39));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  return (bool)extraout_AL;
}



//=== COpinionPersonalityDef @ 0045e7cf ===

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
_Transfer_COpinionPersonalityDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  FUN_0045e808(this + 0x3c);
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x50));
  return;
}



//=== COpinionSourceDef @ 0045f178 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void _Transfer_COpinionSourceDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  CMemoryAllocatorVariableSize CVar1;
  ulong uVar2;
  CMemoryAllocatorVariableSize *pCVar3;
  undefined1 uVar4;
  ulong uVar5;
  int in_ECX;
  uint uVar6;
  undefined4 local_74 [5];
  CMemoryAllocatorVariableSize aCStack_60 [80];
  CMemoryAllocatorVariableSize aCStack_10 [8];
  undefined4 local_8;
  
  local_8 = DAT_0139c8a8;
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x49));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x4a));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x4b));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x5d));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x5e));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x5f));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x68));
  if ((*(int *)(param_1 + 0x18) == 2) || (*(int *)(param_1 + 0x18) == 3)) {
    uVar6 = 0;
    do {
      CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x78 + uVar6));
      uVar6 = uVar6 + 1;
    } while (uVar6 < 0x4f);
    uVar6 = 0;
    do {
      CEngineLightingManager::UpdateShadowScene
                (param_1,(CEngineCamera *)(*(int *)(in_ECX + 0x6c) + uVar6));
      uVar6 = uVar6 + 4;
    } while (uVar6 < 0x14);
  }
  else {
    uVar2 = *(ulong *)(in_ECX + 0x3c);
    local_74[0] = 0;
    local_74[1] = 0;
    local_74[2] = 0;
    local_74[3] = 0;
    local_74[4] = 0;
    uVar5 = *(ulong *)(uVar2 + 8);
    while (uVar5 != uVar2) {
      pCVar3 = *(CMemoryAllocatorVariableSize **)(uVar5 + 0x10);
      local_74[(int)pCVar3] = *(undefined4 *)(uVar5 + 0x14);
      aCStack_10[(int)pCVar3] = (CMemoryAllocatorVariableSize)0x1;
      uVar5 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(pCVar3);
    }
    uVar6 = 0;
    do {
      *(undefined4 *)(uVar6 + *(int *)(in_ECX + 0x6c)) = *(undefined4 *)((int)local_74 + uVar6);
      uVar6 = uVar6 + 4;
    } while (uVar6 < 0x14);
    uVar6 = 0;
    do {
      if (uVar6 < 0x12) {
        uVar4 = 0;
      }
      else {
        uVar4 = *(undefined1 *)(in_ECX + 0x4c);
      }
      *(undefined1 *)(in_ECX + 0x78 + uVar6) = uVar4;
      uVar6 = uVar6 + 1;
    } while (uVar6 < 0x4f);
    uVar2 = *(ulong *)(in_ECX + 0x50);
    uVar5 = *(ulong *)(uVar2 + 8);
    while (uVar5 != uVar2) {
      pCVar3 = *(CMemoryAllocatorVariableSize **)(uVar5 + 0x10);
      CVar1 = *(CMemoryAllocatorVariableSize *)(uVar5 + 0x14);
      aCStack_60[(int)pCVar3] = (CMemoryAllocatorVariableSize)0x1;
      pCVar3[in_ECX + 0x78] = CVar1;
      uVar5 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(pCVar3);
    }
  }
  return;
}



//=== CArmourDef @ 004615fb ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CArmourDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x61));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x62));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  return;
}



//=== CSoundThemeDef @ 0046329e ===

/* [bsim sim=0.6808950289375171 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CSoundThemeDef__UAEXAAVCPersistContext___Z(void)

{
  CEngineLightingManager *this;
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  this = in_stack_00000004;
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  in_stack_00000004 = (CEngineLightingManager *)0x0;
  CPersistContext::TransferMap<EGameAction,CCharString>
            ((CPersistContext *)this,"AtmosGroupMap",
             (map<CCharString,EGameAction,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,EGameAction>_>_>
              *)(in_ECX + 0x54),(EGameAction *)&stack0x00000004);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x62));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x61));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 99));
  return;
}



//=== CMeleeCombatAbilityDef @ 00463bff ===

/* [bsim sim=0.7534714130295807 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CMeleeCombatAbilityDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x49));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x65));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  return;
}



//=== CSimVoicesDef @ 00464b8a ===

void _Transfer_CSimVoicesDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_00464b9a(in_ECX + 0x3c);
  return;
}



//=== CSpecialAbilitiesAssassinRushDef @ 00464be9 ===

/* [bsim sim=0.7333613133811833 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesAssassinRushDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  return;
}



//=== CSpecialAbilitiesBulletTimeDef @ 00464db5 ===

/* [bsim sim=0.652807268440793 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesBulletTimeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xfc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x104));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x108));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x110));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x11c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x188));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x184));
  return;
}



//=== CSpecialAbilitiesForcePushDef @ 00465163 ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesForcePushDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  return;
}



//=== CSpecialAbilitiesEnflameDef @ 00465258 ===

/* [bsim sim=0.7096103603605007 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesEnflameDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  return;
}



//=== CSpecialAbilitiesPhysicalShieldDef @ 00465390 ===

/* [bsim sim=0.6656390742609041 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesPhysicalShieldDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x144));
  return;
}



//=== CSpecialAbilitiesHealLifeDef @ 00465634 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CSpecialAbilitiesHealLifeDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  return (bool)extraout_AL;
}



//=== CSpecialAbilitiesTurncoatSpellDef @ 00465662 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CSpecialAbilitiesTurncoatSpellDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  return;
}



//=== CSpecialAbilitiesFireballSpellDef @ 004656a6 ===

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
_Transfer_CSpecialAbilitiesFireballSpellDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x3c));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_2);
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x50));
  return;
}



//=== CSpecialAbilitiesDivineWrathDef @ 004656df ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesDivineWrathDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  return;
}



//=== CSpecialAbilitiesUnholyPowerDef @ 00465b41 ===

/* [bsim sim=0.7254558305426602 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesUnholyPowerDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  return;
}



//=== CCombatAbilityBlockDefBase @ 00465c54 ===

void _Transfer_CCombatAbilityBlockDefBase__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  CEngineLightingManager *pCVar1;
  CActiveFile local_24 [12];
  CEngineLightingManager *local_18;
  CEngineLightingManager *local_14;
  undefined4 local_10;
  undefined1 local_c [8];
  
  pCVar1 = param_1;
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(pCVar1,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(pCVar1,(CEngineCamera *)(in_ECX + 0x45));
  NUISystem::CScrollingComponent::ChangeState((CScrollingComponent *)pCVar1,in_ECX + 0x28);
  if ((*(int *)(pCVar1 + 0x18) == 1) || (*(int *)(pCVar1 + 0x18) == 3)) {
    local_18 = (CEngineLightingManager *)0x0;
    local_14 = (CEngineLightingManager *)0x0;
    local_10 = 0;
    FUN_00465e44(*(undefined4 *)(*(int *)(in_ECX + 0x48) + 8),*(int *)(in_ECX + 0x48),
                 (int)&param_1 + 3);
    CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)pCVar1);
  }
  else {
    local_18 = (CEngineLightingManager *)0x0;
    local_14 = (CEngineLightingManager *)0x0;
    local_10 = 0;
    CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)pCVar1);
    pCVar1 = local_18;
    param_1 = local_14;
    FUN_00451add((int)&param_1 + 3,(int)&param_1 + 3);
    for (; pCVar1 != param_1; pCVar1 = pCVar1 + 4) {
      FUN_00465d36(local_c,pCVar1);
    }
    FUN_00461bf0(local_24);
    CFileInstaller::CActiveFile::OnReadFinished(local_24);
  }
  if (local_18 != (CEngineLightingManager *)0x0) {
    free(local_18);
  }
  return;
}



//=== CHeroCombatDef @ 004665b6 ===

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



//=== CBrainDef @ 00466b37 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CBrainDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  return (bool)extraout_AL;
}



//=== CPlayerInventoryDef @ 00467553 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CPlayerInventoryDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(this + 0x6c));
  return;
}



//=== CInventoryDef @ 004678e2 ===

/* [bsim sim=0.652807268440793 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CInventoryDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xfc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x108));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x13c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x144));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x14c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x170));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x16c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x174));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x178));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x17c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x180));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x184));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x188));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 400));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x198));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 500));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x204));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x20c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x210));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x218));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x21c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x220));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x224));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x228));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x22c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x230));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x238));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x240));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x248));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x256));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x264));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x272));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x280));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x288));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x290));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x298));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x29c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 700));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x304));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x314));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x31c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x324));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x328));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x32c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x330));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x334));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x338));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x33c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x340));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x344));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x348));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x354));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x35c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x364));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x368));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x36c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x370));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x378));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x380));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x388));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x390));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x398));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 1000));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x400));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x404));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x408));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x410));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x418));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x420));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x428));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x430));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x434));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x450));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x458));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x460));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x468));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x470));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x474));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x478));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x438));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x43c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x440));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x444));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x448));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x47c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x480));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x484));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x488));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x490));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x494));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x498));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x49c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x500));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x504));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x508));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x510));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x514));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x522));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x530));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x53e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x554));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x558));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x574));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x582));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x590));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x59e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5ba));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x794));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x798));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x79c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x600));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x604));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x608));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x610));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x614));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x618));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x61c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x620));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x624));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x628));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x62c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x630));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x634));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x638));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x63c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x640));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x644));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x648));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x650));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x658));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x668));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x660));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x670));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x678));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x680));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x688));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x690));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x694));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x69c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6b2));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6ba));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d8));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x800));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x808));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x816));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x824));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x832));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x840));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x85c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x864));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x86c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x874));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x87c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x884));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x894));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x898));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x89c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8ca));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8e6));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x904));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x908));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x910));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x914));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x918));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x91c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x920));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x924));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x928));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x92c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x93a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x948));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x956));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x964));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x972));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x980));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x988));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x788));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x790));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x700));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x708));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x710));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x718));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x720));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x728));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x730));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x738));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x750));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x754));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x758));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x75c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x760));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x764));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x768));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x770));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x76c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x774));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x778));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x780));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x990));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x998));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c6));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9e2));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9fe));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa06));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa16));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa1e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa26));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa30));
  return;
}



//=== CMaterialDef @ 0046923d ===

/* [bsim sim=0.7667980480313481 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CMaterialDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  return;
}



//=== CCreatureGenerationFamilyDef @ 004696c1 ===

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
_Transfer_CCreatureGenerationFamilyDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CCreatureAction_FireMissileWeapon *param_2)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_2);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_2,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_2,(CEngineCamera *)(this + 0x4c));
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_2);
  return;
}



//=== CHeroAbilityDef @ 00469ab3 ===

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



//=== CTextureReplacementDef @ 00469c2e ===

void _Transfer_CTextureReplacementDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CCameraManagerDef @ 0046a0b0 ===

/* [bsim sim=0.7836975757083103 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCameraManagerDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== CSpecialAbilitiesBattleChargeDef @ 0046a178 ===

/* [bsim sim=0.7333613133811833 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesBattleChargeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  return;
}



//=== CSpecialAbilitiesMultiArrowDef @ 0046a260 ===

/* [bsim sim=0.7597760295477823 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesMultiArrowDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  return;
}



//=== CSpecialAbilitiesBerserkDef @ 0046a2fc ===

/* [bsim sim=0.6702392876556056 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesBerserkDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x130));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x144));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x148));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x14c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x180));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x184));
  return;
}



//=== CSpecialAbilitiesLightningSpellDef @ 0046a568 ===

/* [bsim sim=0.7044046192807111 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesLightningSpellDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x110));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x11c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  return;
}



//=== CSpecialAbilitiesSummonSpellDef @ 0046a6dd ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesSummonSpellDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  FUN_00431143(in_ECX + 100);
  FUN_00431143(in_ECX + 0x68);
  return;
}



//=== CSpecialAbilitiesBurntEffectDef @ 0046a742 ===

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
_Transfer_CSpecialAbilitiesBurntEffectDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_2);
  return;
}



//=== CSpecialAbilitiesElectrocutedEffectDef @ 0046a77b ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesElectrocutedEffectDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  return;
}



//=== CSpecialAbilitiesCreatureTintDef @ 0046a7e0 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CSpecialAbilitiesCreatureTintDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  return (bool)extraout_AL;
}



//=== CVillagePeopleDef @ 0046a80e ===

/* [bsim sim=0.6859922208721605 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CVillagePeopleDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25));
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
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  return;
}



//=== CSoundDef @ 0046a9ce ===

/* [bsim sim=0.5984244111284146 <- ego_r]
   public: virtual void __thiscall CAbilityTrackLevelDef::Transfer(class CPersistContext &) */

void __thiscall
CAbilityTrackLevelDef::_Transfer_CSoundDef__UAEXAAVCPersistContext___Z
          (CAbilityTrackLevelDef *this,CPersistContext *param_1)

{
  CPersistContext *this_00;
  
  this_00 = param_1;
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"AtmosListenerDampingMin",(char *)(this + 0x78),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"AtmosListenerDampingRange",(char *)(this + 0x7c),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"DistanceModelCurve",(char *)(this + 0x80),(char *)&param_1);
  return;
}



//=== CAttackPatternDef @ 0046c284 ===

void _Transfer_CAttackPatternDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CObjectFamilyDef @ 0046c294 ===

void _Transfer_CObjectFamilyDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::GFTransferVectorField_CObjectFamilyDef_Objects(param_1);
  return;
}



//=== CHitLocationsDef @ 0046c6e0 ===

void _Transfer_CHitLocationsDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CCombatSequenceDef @ 0046c6f0 ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCombatSequenceDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  return;
}



//=== CQuickAccessItemButtonGuiDef @ 0046ee6a ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CQuickAccessItemButtonGuiDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x48));
  return (bool)extraout_AL;
}



//=== CAbilityButtonGuiDef @ 0046f203 ===

/* [bsim sim=0.7977435906169258 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CAbilityButtonGuiDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  return;
}



//=== CLensFlareElementDef @ 0046f727 ===

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
_Transfer_CLensFlareElementDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  return;
}



//=== CCombatTypeDef @ 0046f7c8 ===

/* [bsim sim=0.7218905769583901 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCombatTypeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x95));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  return;
}



//=== CAppearanceModifierDef @ 0046fc51 ===

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



//=== CEntitySoundDef @ 004702c8 ===

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
_Transfer_CEntitySoundDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  FUN_00470301(this + 0x28);
  FUN_00431143(this + 0x48);
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x4c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x50));
  return;
}



//=== CRegionDef @ 004715ab ===

/* [bsim sim=0.6913960620116536 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CRegionDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  FUN_00470301(in_ECX + 0x50);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x71));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x89));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x91));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9f));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  return;
}



//=== CFactionDef @ 004717a1 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CFactionDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  return (bool)extraout_AL;
}



//=== COpinionDeedReactionDef @ 00472095 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_COpinionDeedReactionDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x4c));
  return (bool)extraout_AL;
}



//=== CSpecialAbilitiesMultiStrikeDef @ 00472415 ===

/* [bsim sim=0.7534714130295807 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CSpecialAbilitiesMultiStrikeDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
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
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
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



//=== CSpecialAbilitiesDrainLifeDef @ 004724c2 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CSpecialAbilitiesDrainLifeDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(this + 0x6c));
  return;
}



//=== CGlobalDef @ 00472506 ===

/* [bsim sim=0.658593080812477 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CGlobalDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  vector<NSpeechGainManager::CDialogueLayerDef,std::allocator<NSpeechGainManager::CDialogueLayerDef>_>
  *unaff_EDI;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  FUN_00431143(in_ECX + 0x90);
  FUN_00431143(in_ECX + 0x94);
  FUN_00431143(in_ECX + 0x98);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  FUN_00431143(in_ECX + 0xac);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe6));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CPersistContext::TransferVectorOfSubComponents<NSpeechGainManager::CDialogueLayerDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x10c),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x11c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x130));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x138));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x13c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x140));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x144));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x148));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x14c));
  return;
}



//=== CEnvironmentThemeDef @ 004732e0 ===

/* [bsim sim=0.6895265977087581 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CEnvironmentThemeDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  return;
}



//=== CAppearanceDef @ 00473504 ===

/* [bsim sim=0.7534165361289483 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CAppearanceDef__UAEXAAVCPersistContext___Z(CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x25));
  FUN_0047367a((CTCInventoryQuests *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x55));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x56));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x6a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x74));
  CTCInventoryQuests::ProcessButtonBReleased((CTCInventoryQuests *)(this + 0x34));
  return;
}



//=== CPlayerGuiDef @ 004736c4 ===

/* [bsim sim=0.5512960438745383 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CPlayerGuiDef__UAEXAAVCPersistContext___Z(void)

{
  CEngineLightingManager *this;
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  CEngineLightingManager *in_stack_00000004;
  
  this = in_stack_00000004;
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x104));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x110));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x11c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x130));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x138));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x140));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x144));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x148));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x14c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x15c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x164));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x168));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x16c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x170));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x174));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x17c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x180));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x188));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 500));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x208));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x210));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x214));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x21c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x220));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x224));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x22c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x230));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x238));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x23c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x240));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x244));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x248));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x24c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x250));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x254));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x264));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x26c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x270));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x278));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x280));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x288));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x290));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x294));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x298));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x29c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 700));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x304));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x310));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x318));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 800));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x328));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x32c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x330));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x334));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x338));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x33c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x340));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x344));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x354));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x368));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x370));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x374));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x37c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x390));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 1000));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x404));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x408));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x410));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x418));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x420));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x424));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x428));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x42c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x430));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x434));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x43c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x468));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x46c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x470));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x474));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x478));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x47c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x480));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x484));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x488));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x490));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x494));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x498));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x49c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4dc));
  FUN_00431143(in_ECX + 0x4e0);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x504));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x514));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x51c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x520));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x524));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x528));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x530));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x534));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x538));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x53c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x564));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x56c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x574));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x578));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x57c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x580));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x584));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x588));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x590));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x594));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x59c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5d0));
  FUN_00431143(in_ECX + 0x5d4);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5e4));
  FUN_00431143(in_ECX + 0x5e8);
  FUN_00431143(in_ECX + 0x5ec);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x604));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x614));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x61c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x624));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x62c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x634));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x63c));
  FUN_00431143(in_ECX + 0x644);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x648));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x64c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x650));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x654));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x658));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x65c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x660));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x664));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x668));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x66c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x670));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x674));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x678));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x67c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x680));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x684));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x688));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x690));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x694));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x69c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6a4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6ac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6e4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6f0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6f8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x700));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x704));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x714));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x71c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x724));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x728),unaff_EDI);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x734),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x740));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x744));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x748));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x750));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x754));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x75c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x760));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x764));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x768));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x76c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x770));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x774));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x778));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x77c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x780));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x784));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x788));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x790));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x794));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x79c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7a0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7a8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7b0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7b4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7b8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7bc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 2000));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7f4));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x7fc),unaff_EDI);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x808),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x814));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x81c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x824));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x838));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x840));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x848));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x850));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x854));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x858));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x85c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x860));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x864));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8cc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8d4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8d8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8dc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8e0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8e8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8ec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8f4));
  FUN_00431143(in_ECX + 0x8f8);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8fc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x904));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x908));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  in_stack_00000004 = (CEngineLightingManager *)0x0;
  CPersistContext::TransferMap<EGameAction,CCharString>
            ((CPersistContext *)this,"ScriptSprites",
             (map<CCharString,EGameAction,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,EGameAction>_>_>
              *)(in_ECX + 0x934),(EGameAction *)&stack0x00000004);
  in_stack_00000004 = (CEngineLightingManager *)0x0;
  CPersistContext::TransferMap<EGameAction,CCharString>
            ((CPersistContext *)this,"GameActionValues",
             (map<CCharString,EGameAction,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,EGameAction>_>_>
              *)(in_ECX + 0x940),(EGameAction *)&stack0x00000004);
  in_stack_00000004 = (CEngineLightingManager *)0x0;
  CPersistContext::TransferMap<EGameAction,CCharString>
            ((CPersistContext *)this,"MiniMapGraphics",
             (map<CCharString,EGameAction,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,EGameAction>_>_>
              *)(in_ECX + 0x94c),(EGameAction *)&stack0x00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x964));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x96c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x974));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x978));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x97c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x980));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x984));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x988));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x994));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x98c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x990));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x99c));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x998));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9b8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9c0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9c8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9d0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9d8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9e0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9e8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9ec));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9f0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9f4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9f8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9fc));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa00));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa08));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa10));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa14));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa18));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa1c));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa30));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa34));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa38));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa3c));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa4c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa54));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa5c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa64));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa6c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa74));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa7c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa80));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa84));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa88));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa8c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa90));
  FUN_00431143(in_ECX + 0xa94);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa98));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa9c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xaa0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xaa4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xaa8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xaac));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xab0));
  return;
}



//=== CSkyDef @ 0047691e ===

/* [bsim sim=0.7477622976889651 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CSkyDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x74),unaff_EDI);
  return;
}



//=== CEnvironmentThemeDaySetDef @ 00476dc3 ===

/* [bsim sim=0.7836975757083103 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CEnvironmentThemeDaySetDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  CPersistContext *in_stack_00000004;
  
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            (in_stack_00000004,(char *)(in_ECX + 0x3c),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  return;
}



//=== CEngineLocalDetailObjectDef @ 0047769e ===

/* [bsim sim=0.5121604030046741 <- ego_r]
   public: virtual void __thiscall CEngineLocalDetailObjectDef::Transfer(class CPersistContext &) */

void __thiscall
CEngineLocalDetailObjectDef::_Transfer_CEngineLocalDetailObjectDef__UAEXAAVCPersistContext___Z
          (CEngineLocalDetailObjectDef *this,CPersistContext *param_1)

{
  CPersistContext *this_00;
  undefined4 uStack_8;
  
  this_00 = param_1;
  uStack_8 = this;
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x48));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x50));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x54));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x58));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x5c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x60));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 100));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x68));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x6c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x70));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x74));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x78));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x7c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x80));
  param_1 = (CPersistContext *)(CONCAT13(this[0x84],param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"CastShadows",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ ((byte)this[0x84] ^ param_1._3_1_) & 1);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x84] >> 1,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)CONCAT13(1,(undefined3)uStack_8);
  CPersistContext::Transfer<signed_char>
            (this_00,"ReceiveShadows",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ (param_1._3_1_ << 1 ^ (byte)this[0x84]) & 2);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x84] >> 2,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsRepeatedMesh",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ (param_1._3_1_ << 2 ^ (byte)this[0x84]) & 4);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x84] >> 3,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsZSprite",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ (param_1._3_1_ << 3 ^ (byte)this[0x84]) & 8);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x84] >> 4,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"AlphaIsBoolean",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ (param_1._3_1_ << 4 ^ (byte)this[0x84]) & 0x10);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x84] >> 5,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"HasLandscapeNormalLighting",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ (param_1._3_1_ << 5 ^ (byte)this[0x84]) & 0x20);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x84] >> 6,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"TiltToSlope",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)
               ((byte)this[0x84] ^ (param_1._3_1_ << 6 ^ (byte)this[0x84]) & 0x40);
  param_1 = (CPersistContext *)CONCAT13((byte)this[0x84] >> 7,param_1._0_3_);
  uStack_8 = (CEngineLocalDetailObjectDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"HasWindSkew",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x84] = (CEngineLocalDetailObjectDef)((byte)this[0x84] & 0x7f | param_1._3_1_ << 7);
  return;
}



//=== CEngineLocalDetailLayerDef @ 00477cb1 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CEngineLocalDetailLayerDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_ESI;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)param_1,(char *)(this + 0x48),unaff_ESI);
  return (bool)extraout_AL;
}



//=== CAppearanceModifierScalingDef @ 004784e4 ===

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



//=== CAbilityTrackLevelDef @ 00478827 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CAbilityTrackLevelDef__UAEXAAVCPersistContext___Z
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



//=== CAbilityTrackDef @ 00478ab7 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CAbilityTrackDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)param_1,(char *)(this + 0x40),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x4c));
  return (bool)extraout_AL;
}



//=== COpinionDeedEffectsDef @ 00478ee2 ===

/* [bsim sim=0.7018145674530322 <- donor]
   public: virtual void __thiscall COpinionDeedEffectsDef::Transfer(class CPersistContext &) */

void __thiscall
COpinionDeedEffectsDef::_Transfer_COpinionDeedEffectsDef__UAEXAAVCPersistContext___Z
          (COpinionDeedEffectsDef *this,CPersistContext *param_1)

{
  CPersistContext *this_00;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  undefined4 uStack_8;
  
  this_00 = param_1;
  uStack_8 = this;
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x48));
  FUN_004792cf(this + 0x4c);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x50));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            (this_00,(char *)(this + 0x54),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x60));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x70));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x74));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)this_00,(CEngineCamera *)(this + 0x78));
  param_1 = (CPersistContext *)(CONCAT13(this[0x7c],param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"CanBeHeard",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x7c] = (COpinionDeedEffectsDef)((byte)this[0x7c] ^ ((byte)this[0x7c] ^ param_1._3_1_) & 1);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7c] >> 1,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"ReactionRestrictToTarget",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x7c] = (COpinionDeedEffectsDef)
               ((byte)this[0x7c] ^ (param_1._3_1_ << 1 ^ (byte)this[0x7c]) & 2);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7c] >> 2,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"AllowSimultaneousReactionDialogue",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x7c] = (COpinionDeedEffectsDef)
               ((byte)this[0x7c] ^ (param_1._3_1_ << 2 ^ (byte)this[0x7c]) & 4);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7c] >> 3,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"CanApologise",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x7c] = (COpinionDeedEffectsDef)
               ((byte)this[0x7c] ^ (param_1._3_1_ << 3 ^ (byte)this[0x7c]) & 8);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7c] >> 4,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"MustNotRunWhenWarned",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3)
            );
  this[0x7c] = (COpinionDeedEffectsDef)
               ((byte)this[0x7c] ^ (param_1._3_1_ << 4 ^ (byte)this[0x7c]) & 0x10);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7c] >> 5,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"OnlyGetFinedForOneInstance",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  this[0x7c] = (COpinionDeedEffectsDef)
               ((byte)this[0x7c] ^ (param_1._3_1_ << 5 ^ (byte)this[0x7c]) & 0x20);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7c] >> 6,param_1._0_3_) & 0x1ffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsFlirting",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x7c] = (COpinionDeedEffectsDef)
               ((byte)this[0x7c] ^ (param_1._3_1_ << 6 ^ (byte)this[0x7c]) & 0x40);
  param_1 = (CPersistContext *)CONCAT13((byte)this[0x7c] >> 7,param_1._0_3_);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"OnReactSetGreeted",(char *)((int)&param_1 + 3),(char *)((int)&uStack_8 + 3));
  this[0x7c] = (COpinionDeedEffectsDef)(param_1._3_1_ << 7 | (byte)this[0x7c] & 0x7f);
  param_1 = (CPersistContext *)(CONCAT13(this[0x7d],param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"AllowSpeechOnNonPureAISpeaker",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x7d] = (COpinionDeedEffectsDef)((byte)this[0x7d] ^ ((byte)this[0x7d] ^ param_1._3_1_) & 1);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7d] >> 1,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"AllowIndirectReactWhileCarrying",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x7d] = (COpinionDeedEffectsDef)
               ((byte)this[0x7d] ^ (param_1._3_1_ << 1 ^ (byte)this[0x7d]) & 2);
  param_1 = (CPersistContext *)(CONCAT13((byte)this[0x7d] >> 2,param_1._0_3_) & 0x1ffffff);
  uStack_8 = (COpinionDeedEffectsDef *)((uint)uStack_8 & 0xffffff);
  CPersistContext::Transfer<signed_char>
            (this_00,"BlockWhileFollowingPlayer",(char *)((int)&param_1 + 3),
             (char *)((int)&uStack_8 + 3));
  this[0x7d] = (COpinionDeedEffectsDef)
               ((byte)this[0x7d] ^ (param_1._3_1_ << 2 ^ (byte)this[0x7d]) & 4);
  return;
}



//=== CEngineLocalDetailGeneratorDef @ 0047989b ===

void _Transfer_CEngineLocalDetailGeneratorDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_retaddr;
  
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            (param_1,(char *)(in_ECX + 0x3c),unaff_retaddr);
  return;
}



//=== CHeroStatsDef @ 00479cba ===

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



//=== CDrunkennessDef @ 004dc96f ===

/* [bsim sim=1.0 <- ego_r]
   private: virtual void __thiscall
   NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::OnPersist(class CPersistContext &) */

void __thiscall
NScript::CQ_GuildTrainingScript::CApprenticeSpeedTest::
_Transfer_CDrunkennessDef__UAEXAAVCPersistContext___Z
          (CApprenticeSpeedTest *this,CPersistContext *param_1)

{
  char local_8 [4];
  
  local_8[0] = '\0';
  local_8[1] = '\0';
  local_8[2] = '\0';
  local_8[3] = '\0';
  CPersistContext::Transfer<signed_char>
            (param_1,"DrunkennessThresholdMult",(char *)(this + 0x28),local_8);
  return;
}



//=== CCarryableDef @ 004dd924 ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CCarryableDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  return;
}



//=== CVillageMemberDef @ 004dd989 ===

void _Transfer_CVillageMemberDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x25));
  return;
}



//=== CBedDef @ 004dd999 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CBedDef__UAEXAAVCPersistContext___Z(CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CSmokeGeneratorDef @ 004dd9dd ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CSmokeGeneratorDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== CCreatureNavigationDef @ 004dd9fe ===

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
_Transfer_CCreatureNavigationDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x34));
  return;
}



//=== CBuyHouseDef @ 004ddb56 ===

void _Transfer_CBuyHouseDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x25));
  return;
}



//=== CWifeDef @ 004ddb66 ===

void _Transfer_CWifeDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CDoorDef @ 004ddb76 ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CDoorDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x39));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3a));
  return;
}



//=== CKickableDef @ 004ddc74 ===

/* [bsim sim=0.7746988183602541 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CKickableDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CLightDef @ 004ddcfa ===

/* [bsim sim=0.7477622976889651 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CLightDef__UAEXAAVCPersistContext___Z(void)

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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x41));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x42));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  return;
}



//=== CSpotLightDef @ 004dde50 ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CSpotLightDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CHeroDef @ 004ddeaa ===

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



//=== CPerceivedThingDef @ 004ddf9e ===

/* [bsim sim=0.7836975757083103 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CPerceivedThingDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  return;
}



//=== CStealthDef @ 004de0bd ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CStealthDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CTrophyDef @ 004de122 ===

/* [bsim sim=0.7425545198594122 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTrophyDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x35));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  FUN_00431143(in_ECX + 0x48);
  FUN_00431143(in_ECX + 0x4c);
  FUN_00431143(in_ECX + 0x50);
  FUN_00431143(in_ECX + 0x54);
  FUN_00431143(in_ECX + 0x58);
  FUN_00431143(in_ECX + 0x5c);
  FUN_00431143(in_ECX + 0x60);
  return;
}



//=== CChestDef @ 004de204 ===

/* [bsim sim=0.7977435906169258 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall NTCScriptedControl::CActionTalkBase::CChestDef__Transfer(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  FUN_00431143(in_ECX + 0x30);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  return;
}



//=== CExplodingObjectDef @ 004de253 ===

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
_Transfer_CExplodingObjectDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x25));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  return;
}



//=== CLookDef @ 004de28c ===

/* [bsim sim=0.7477622976889651 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CLookDef__UAEXAAVCPersistContext___Z(void)

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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4e));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4f));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  return;
}



//=== CEnemyDef @ 004de33e ===

void _Transfer_CEnemyDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== COpinionOfHeroDef @ 004de36b ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_COpinionOfHeroDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CShopItemDef @ 004de3af ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CShopItemDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CStockItemDef @ 004de4b2 ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CStockItemDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== CTimeAppearanceFadeDef @ 004de50c ===

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
_Transfer_CTimeAppearanceFadeDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x34));
  return;
}



//=== CGiftDef @ 004de545 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CGiftDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== CBoastingPodiumDef @ 004de772 ===

void _Transfer_CBoastingPodiumDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CShipDef @ 004de782 ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CShipDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CExpressionSubDef @ 004de881 ===

void _Transfer_CExpressionSubDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CSummonableCreatureDef @ 004de891 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CSummonableCreatureDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== COccupiableDef @ 004de8b2 ===

void _Transfer_COccupiableDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CInterestingToVillagersDef @ 004de916 ===

void _Transfer_CInterestingToVillagersDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CGuardDef @ 004de926 ===

/* [bsim sim=0.7836975757083103 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CGuardDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CCrateStackDef @ 004de9a1 ===

/* [bsim sim=0.6667451391019226 <- ego_r]
   public: virtual void __thiscall CCarryingDef::Transfer(class CPersistContext &) */

void __thiscall
CCarryingDef::_Transfer_CCrateStackDef__UAEXAAVCPersistContext___Z
          (CCarryingDef *this,CPersistContext *param_1)

{
  CCarryingDef *local_8;
  
  local_8 = this;
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  local_8 = (CCarryingDef *)0x0;
  CPersistContext::Transfer<signed_char>(param_1,"Priority",(char *)(this + 0x2c),(char *)&local_8);
  return;
}



//=== CTavernTableDef @ 004de9d2 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CTavernTableDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x25));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x26));
  return (bool)extraout_AL;
}



//=== CTavernDef @ 004de9f3 ===

void _Transfer_CTavernDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CFireballSpellLevelDef @ 004dea03 ===

void _Transfer_CFireballSpellLevelDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CThingDrainLifeShotDef @ 004dea13 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CThingDrainLifeShotDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CGoldDef @ 004debc4 ===

void _Transfer_CGoldDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CCoinGameObstacleDef @ 004dec1c ===

void _Transfer_CCoinGameObstacleDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CTavernGameCoinBaseDef @ 004dec2c ===

/* [bsim sim=0.9415942478091848 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTavernGameCoinBaseDef__UAEXAAVCPersistContext___Z
          (void)

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
  return;
}



//=== CTavernGameCoinGolfDef @ 004dec86 ===

/* [bsim sim=0.7597760295477823 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTavernGameCoinGolfDef__UAEXAAVCPersistContext___Z
          (void)

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
  return;
}



//=== CDecapitationDef @ 004ded4e ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CDecapitationDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== CWallMountEffectsDef @ 004dedac ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CWallMountEffectsDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x30));
  return (bool)extraout_AL;
}



//=== CFishDef @ 004dee7e ===

/* [bsim sim=0.7667980480313481 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CFishDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CTeleporterDef @ 004def0f ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CTeleporterDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x30));
  return (bool)extraout_AL;
}



//=== CHeroSpecialMovementDef @ 004def3d ===

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



//=== CResurrectionItemDef @ 004def76 ===

void _Transfer_CResurrectionItemDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CKrakenTentacleDef @ 004df050 ===

/* [bsim sim=0.7534714130295807 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CKrakenTentacleDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CAICreatureWillPowerIndicatorDef @ 004df0f7 ===

void _Transfer_CAICreatureWillPowerIndicatorDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CFishingRodDef @ 004df107 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CFishingRodDef__UAEXAAVCPersistContext___Z(CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CNymphDef @ 004df1e9 ===

/* [bsim sim=0.7836975757083103 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CNymphDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  return;
}



//=== CSummonDef @ 004df264 ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CSummonDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== CCameraCollisionDef @ 004df31c ===

void _Transfer_CCameraCollisionDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CTCVolumeContainmentTrackerDef @ 004df32c ===

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
_Transfer_CTCVolumeContainmentTrackerDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x25));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x26));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  return;
}



//=== CWillResponseDef @ 004df562 ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CWillResponseDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x26));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x27));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x29));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2b));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  return;
}



//=== CBettingDef @ 004df5c7 ===

/* [bsim sim=0.7667980480313481 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CBettingDef__UAEXAAVCPersistContext___Z(void)

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
  return;
}



//=== COracleMinigameDef @ 004df658 ===

/* [bsim sim=0.7597760295477823 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_COracleMinigameDef__UAEXAAVCPersistContext___Z(void)

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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  return;
}



//=== CFireheartMinigameDef @ 004df6f4 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CFireheartMinigameDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  FUN_00431143(this + 0x28);
  FUN_00431143(this + 0x2c);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CLightningOrbDef @ 004df738 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CLightningOrbDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x38));
  return;
}



//=== CHasNameDef @ 004df77c ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CHasNameDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x30));
  return (bool)extraout_AL;
}



//=== CMazeBattleDef @ 004e3842 ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CMazeBattleDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  return;
}



//=== CThunderBattleDef @ 004e38e4 ===

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
_Transfer_CThunderBattleDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x48));
  return;
}



//=== CWhisperBattleDef @ 004e394f ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CWhisperBattleDef__UAEXAAVCPersistContext___Z
          (CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  return (bool)extraout_AL;
}



//=== CWaspQueenBattleDef @ 004e39a2 ===

void _Transfer_CWaspQueenBattleDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== CJackOfBladesBattleDef @ 004e39e6 ===

/* [bsim sim=0.7377739590628846 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CJackOfBladesBattleDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  return;
}



//=== CScorpionKingBattleDef @ 004e3ae0 ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CScorpionKingBattleDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
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
  return;
}



//=== CTrollBattleDef @ 004e3b82 ===

/* [bsim sim=0.7941018284107619 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTrollBattleDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  return;
}



//=== CBalverineBattleDef @ 004e3c24 ===

/* [bsim sim=0.7252756668699446 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CBalverineBattleDef__UAEXAAVCPersistContext___Z
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



//=== CAreaOfEffectAttackDef @ 004e3f7b ===

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
_Transfer_CAreaOfEffectAttackDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x40));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x44));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x48));
  return;
}



//=== CClockDef @ 004e448d ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CClockDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x34));
  return (bool)extraout_AL;
}



//=== CExperienceDef @ 004e5011 ===

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



//=== CHeroSuitDef @ 004e55f1 ===

void _Transfer_CHeroSuitDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== CFlammableDef @ 004e599d ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CFlammableDef__UAEXAAVCPersistContext___Z(CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  FUN_004e59e1(this + 0x38);
  return;
}



//=== CTrapDef @ 004e5d08 ===

/* [bsim sim=0.7597760295477823 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTrapDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x54));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  return;
}



//=== CIdleSchedulerDef @ 004e6248 ===

void _Transfer_CIdleSchedulerDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== CRumbleDef @ 004e676a ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CRumbleDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x34));
  return (bool)extraout_AL;
}



//=== CCarryingDef @ 004e77c8 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CCarryingDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_1);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x34));
  return (bool)extraout_AL;
}



//=== CHeroMorphDef @ 004e77e9 ===

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



//=== CQuestCardDef @ 004e795a ===

/* [bsim sim=0.7333613133811833 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CQuestCardDef__UAEXAAVCPersistContext___Z(void)

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
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x59));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5b));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== CBuyableHouseDef @ 004e7d97 ===

/* [bsim sim=0.8063601184507746 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CBuyableHouseDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x26));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x49));
  return;
}



//=== CSkeletalMorphDef @ 004e7e36 ===

void _Transfer_CSkeletalMorphDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CCreatureModeDef @ 004e7ea7 ===

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
_Transfer_CCreatureModeDef__UAEXAAVCPersistContext___Z
          (deque<NUISystem::CComponent*,std::allocator<NUISystem::CComponent*>_> *this,
          CEngineLightingManager *param_2)

{
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(param_2,(CEngineCamera *)(this + 0x30));
  CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)param_2);
  return;
}



//=== CBossDef @ 004e7ee0 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CBossDef__UAEXAAVCPersistContext___Z(CGuiControlComboBox *this)

{
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(this + 0x4c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(this + 0x50));
  return;
}



//=== CTurncoatDef @ 004e7f24 ===

/* [bsim sim=0.7836975757083103 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTurncoatDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x25));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  return;
}



//=== CTavernGameSpotTheAdditionDef @ 004e7fd8 ===

/* [bsim sim=0.7185451120063443 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::
_Transfer_CTavernGameSpotTheAdditionDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  return;
}



//=== CKrakenDef @ 004e80e3 ===

/* [bsim sim=0.7292684543307638 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CKrakenDef__UAEXAAVCPersistContext___Z(void)

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
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  return;
}



//=== CVillageDef @ 004e82c7 ===

/* [bsim sim=0.6753655655904763 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CVillageDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  CCreatureAction_FireMissileWeapon *in_stack_00000004;
  
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x50));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CCreatureAction_FireMissileWeapon::FrameUpdate(in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe5));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe6));
  FUN_00431143(in_ECX + 0xe8);
  FUN_00431143(in_ECX + 0xec);
  FUN_00431143(in_ECX + 0xf0);
  FUN_00431143(in_ECX + 0xf4);
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0xfc));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x104));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)in_stack_00000004,(CEngineCamera *)(in_ECX + 0x108));
  return;
}



//=== CContainerRewardHeroDef @ 004e8987 ===

void _Transfer_CContainerRewardHeroDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::GFTransferVectorField_CContainerRewardHeroDef_ObjectFamilies
            (param_1);
  return;
}



//=== CWeaponDef @ 004e8ce9 ===

/* [bsim sim=0.5013493663487226 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CWeaponDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CCharString::CCharString((CCharString *)&stack0x00000004,"SWORD",-1);
  FUN_00431143(in_ECX + 0x34);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000004,
             extraout_EDX,unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x40));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x44));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x48));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x4c));
  FUN_004e9550(in_ECX + 0x50);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CCharString::CCharString((CCharString *)&stack0x00000004,"weapon_pos_a",-1);
  FUN_00431143(in_ECX + 0x98);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000004,
             extraout_EDX_00,unaff_EDI);
  CCharString::CCharString((CCharString *)&stack0x00000004,"weapon_pos_b",-1);
  FUN_00431143(in_ECX + 0x9c);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0x00000004,
             extraout_EDX_01,unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc5));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc6));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  FUN_00431143(in_ECX + 0xd4);
  FUN_00431143(in_ECX + 0xd8);
  FUN_00431143(in_ECX + 0xdc);
  FUN_00431143(in_ECX + 0xe0);
  return;
}



//=== CParticleAttacherDef @ 004e9caa ===

void _Transfer_CParticleAttacherDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CReplaceableMeshDef @ 004ea20e ===

void _Transfer_CReplaceableMeshDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_004ea21e(in_ECX + 0x28);
  return;
}



//=== CPrizeScoreDef @ 004eac7b ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CPrizeScoreDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x2c));
  return (bool)extraout_AL;
}



//=== CExplosionDef @ 004eb02f ===

/* [bsim sim=0.7333613133811833 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CExplosionDef__UAEXAAVCPersistContext___Z(void)

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
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x65));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x66));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x68));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x6c));
  return;
}



//=== CMultiStaticMeshEntryDef @ 004eb8c3 ===

/* [bsim sim=0.7892611984812585 <- ego_r]
   public: virtual void __thiscall CGuiControlComboBox::OnDraw(void)const  */

void __thiscall
CGuiControlComboBox::_Transfer_CMultiStaticMeshEntryDef__UAEXAAVCPersistContext___Z
          (CGuiControlComboBox *this)

{
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x2d));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(this + 0x34));
  return;
}



//=== CCardPositionsDef @ 004ebf22 ===

void _Transfer_CCardPositionsDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CCardDef @ 004ec0b3 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual bool __thiscall CEngine::ClipBox(class C3DBoundingBox const &)const  */

bool __thiscall
CEngine::_Transfer_CCardDef__UAEXAAVCPersistContext___Z(CEngine *this,C3DBoundingBox *param_1)

{
  undefined1 extraout_AL;
  
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x3c));
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(this + 0x40));
  return (bool)extraout_AL;
}



//=== CSoundAtmospheresDef @ 004ec153 ===

void _Transfer_CSoundAtmospheresDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CObjectAugmentationsDef @ 004ec53f ===

/* [bsim sim=0.7477622976889651 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CObjectAugmentationsDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x3c));
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CCreatureAction_FireMissileWeapon::FrameUpdate
            ((CCreatureAction_FireMissileWeapon *)in_stack_00000004);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  FUN_00431143(in_ECX + 0x88);
  return;
}



//=== CShopDef @ 004ecb4d ===

/* [bsim sim=0.6843185498115376 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CShopDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
  CEngineLightingManager *in_stack_00000004;
  
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x28));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x2c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x30));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x34));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x38));
  FUN_00431143(in_ECX + 0x3c);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x40),unaff_EDI);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x4c),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x58));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x5c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x60));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9a));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x99));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9b));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  return;
}



//=== CAnimatingObjectDef @ 004ed459 ===

void _Transfer_CAnimatingObjectDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_0047367a((CTCInventoryQuests *)(in_ECX + 0x28));
  CTCInventoryQuests::ProcessButtonBReleased((CTCInventoryQuests *)(in_ECX + 0x28));
  return;
}



//=== CTavernGameDef @ 004ed472 ===

/* [bsim sim=0.652807268440793 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTavernGameDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
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
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xbc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xc4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 200));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xcc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xe8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xec));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf0));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0xfc));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x104));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x108));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x10c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x110));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x114));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x118));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x11c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x120));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 300));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x130));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x134));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x138));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x13c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x140));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x144));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x148));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x14c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x150));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x154));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x158));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x15c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x160));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x164));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x168));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x16c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x170));
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x174),unaff_EDI);
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x180));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x184));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x188));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x18c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 400));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x194));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x198));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x19c));
  CEngineLightingManager::UpdateShadowScene(in_stack_00000004,(CEngineCamera *)(in_ECX + 0x1a0));
  return;
}



//=== CMultiStaticMeshDef @ 004ede1b ===

void _Transfer_CMultiStaticMeshDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_retaddr;
  
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            (param_1,(char *)(in_ECX + 0x28),unaff_retaddr);
  return;
}



//=== CTavernGameCardBaseDef @ 004f9a90 ===

/* [bsim sim=0.7333613133811833 <- ego_r]
   private: static void __fastcall
   NTCScriptedControl::CActionTalkBase::InitialiseDefaultCameraTags(void) */

void __fastcall
NTCScriptedControl::CActionTalkBase::_Transfer_CTavernGameCardBaseDef__UAEXAAVCPersistContext___Z
          (void)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_EDI;
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
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x6c),unaff_EDI);
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            ((CPersistContext *)in_stack_00000004,(char *)(in_ECX + 0x78),unaff_EDI);
  return;
}



//=== CThingCreatureDef @ 00671600 ===

void _Transfer_CThingCreatureDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  C3DVector *pCVar1;
  CEngineLightingManager *this;
  C3DVector *pCVar2;
  int in_ECX;
  byte bVar3;
  undefined4 uStack_4;
  
  this = param_1;
  uStack_4 = in_ECX;
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x74));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x78));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x7c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x84));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x88));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x8c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x90));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x94));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x98));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x9c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xa8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xac));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xb0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xb4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xb8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xbc));
  FUN_00671c10(in_ECX + 0xc0);
  FUN_00671cf0(in_ECX + 0xc4);
  CEnginePrimitiveRenderer2D::AddPrimitive
            ((CEnginePrimitiveRenderer2D *)this,(CEngineInternalPrimitiveBase *)(in_ECX + 200));
  if ((*(int *)(this + 0x18) != 2) && (*(int *)(this + 0x18) != 3)) {
    FUN_006720b0();
    pCVar1 = *(C3DVector **)(in_ECX + 0xcc);
    pCVar2 = (C3DVector *)
             std::_Unique<ENavigatorType*>
                       (*(ENavigatorType **)(in_ECX + 200),(ENavigatorType *)pCVar1);
    if (pCVar2 != pCVar1) {
      pCVar2 = std::_Remove_if<C3DVector*,std::unary_negate<CVectorFilter_IsWithinHeightRange>_>
                         (pCVar2,pCVar1,pCVar2,param_1,&param_1);
    }
    FUN_00672070(pCVar2,pCVar1);
  }
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xd4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xd8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xdc));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xe0));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xe4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xe8));
  FUN_00671dd0(in_ECX + 0xec);
  FUN_00671eb0(in_ECX + 0xf0);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xf4));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xf8));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0xfc));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x100));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x10e));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x11c));
  FUN_00671f90(in_ECX + 0x120);
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x124));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x128));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 300));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x130));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x134));
  param_1 = (CEngineLightingManager *)
            (CONCAT31(param_1._1_3_,*(undefined1 *)(in_ECX + 0x138)) & 0xffffff01);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"DeferHitsIfBehindHitter",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x138) =
       *(byte *)(in_ECX + 0x138) ^ (*(byte *)(in_ECX + 0x138) ^ (byte)param_1) & 1;
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x13c));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x140));
  CEngineLightingManager::UpdateShadowScene(this,(CEngineCamera *)(in_ECX + 0x144));
  param_1 = (CEngineLightingManager *)
            (CONCAT31(param_1._1_3_,*(undefined1 *)(in_ECX + 0x148)) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"LeaveDeadCreature",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ (*(byte *)(in_ECX + 0x148) ^ (byte)param_1) & 1;
  param_1 = (CEngineLightingManager *)(CONCAT31(param_1._1_3_,bVar3 >> 1) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"AbleToStrafe",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ ((byte)param_1 << 1 ^ bVar3) & 2;
  param_1 = (CEngineLightingManager *)(CONCAT31(param_1._1_3_,bVar3 >> 2) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"CreateDummyOnDeath",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ ((byte)param_1 << 2 ^ bVar3) & 4;
  param_1 = (CEngineLightingManager *)(CONCAT31(param_1._1_3_,bVar3 >> 3) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"ImmuneToAttack",(char *)&param_1,(char *)((int)&uStack_4 + 3))
  ;
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ ((byte)param_1 << 3 ^ bVar3) & 8;
  param_1 = (CEngineLightingManager *)(CONCAT31(param_1._1_3_,bVar3 >> 4) & 0xffffff01);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"UseMeleeWeaponAsDefault",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ ((byte)param_1 << 4 ^ bVar3) & 0x10;
  param_1 = (CEngineLightingManager *)(CONCAT31(param_1._1_3_,bVar3 >> 5) & 0xffffff01);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"UnsheatheWeaponsAutomatically",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ ((byte)param_1 << 5 ^ bVar3) & 0x20;
  param_1 = (CEngineLightingManager *)(CONCAT31(param_1._1_3_,bVar3 >> 6) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"IsWillable",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x148);
  *(byte *)(in_ECX + 0x148) = bVar3 ^ ((byte)param_1 << 6 ^ bVar3) & 0x40;
  uStack_4 = uStack_4 & 0xffffff;
  param_1._0_1_ = bVar3 >> 7;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"IsIndestructible",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x148) = (byte)param_1 << 7 | *(byte *)(in_ECX + 0x148) & 0x7f;
  param_1._0_1_ = *(byte *)(in_ECX + 0x149) & 1;
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"IsInvulnerableDuringGenericResponse",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  bVar3 = *(byte *)(in_ECX + 0x149) ^ (byte)param_1;
  param_1 = (CEngineLightingManager *)
            (CONCAT31(param_1._1_3_,*(byte *)(in_ECX + 0x149) >> 1) & 0xffffff01);
  *(byte *)(in_ECX + 0x149) = *(byte *)(in_ECX + 0x149) ^ bVar3 & 1;
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            ((CPersistContext *)this,"IsPushedByExplosions",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x149) =
       *(byte *)(in_ECX + 0x149) ^ ((byte)param_1 << 1 ^ *(byte *)(in_ECX + 0x149)) & 2;
  return;
}



//=== CCreatureDef @ 00675e20 ===

void _Transfer_CCreatureDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CExpressionSet *pCVar1;
  int *piVar2;
  int in_ECX;
  CPersistContext *pCVar3;
  
  pCVar1 = *(CExpressionSet **)(in_ECX + 0x28);
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CWoundMorphs::TransferBinaryIn
              ((CWoundMorphs *)pCVar1,*(CMemoryDataInputStream **)(param_1 + 0x24));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CExpressionSet::TransferBinaryOut(pCVar1,*(CDataOutputStream **)(param_1 + 0x28));
  }
  FUN_00676740(*(undefined4 *)(in_ECX + 0x30));
  pCVar1 = *(CExpressionSet **)(in_ECX + 0x38);
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CExpressionSet::TransferBinaryIn(pCVar1,*(CMemoryDataInputStream **)(param_1 + 0x24));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CExpressionSet::TransferBinaryOut(pCVar1,*(CDataOutputStream **)(param_1 + 0x28));
  }
  piVar2 = *(int **)(in_ECX + 0x40);
  CPersistContext::CheckCRC(param_1,"");
  pCVar3 = param_1;
  (**(code **)(*piVar2 + 0x48))();
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CRGBColour>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (vector<CRGBColour,std::allocator<CRGBColour>_> *)(in_ECX + 0x48));
  }
  else {
    GFSerialiseVectorBinaryOut<float>
              (*(CDataOutputStream **)(param_1 + 0x28),
               (vector<float,std::allocator<float>_> *)(in_ECX + 0x48));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CRGBColour>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (vector<CRGBColour,std::allocator<CRGBColour>_> *)(in_ECX + 0x54));
  }
  else {
    GFSerialiseVectorBinaryOut<float>
              (*(CDataOutputStream **)(param_1 + 0x28),
               (vector<float,std::allocator<float>_> *)(in_ECX + 0x54));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseMapBinaryIn<std::pair<long,long>,CCharString,std::less<CCharString>_>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (map<CCharString,std::pair<long,long>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,std::pair<long,long>_>_>_>
                *)(in_ECX + 0x60));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_004314c8();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CDefString>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (vector<CDefString,std::allocator<CDefString>_> *)(in_ECX + 0x6c));
  }
  else {
    GFSerialiseVectorBinaryOut<float>
              (*(CDataOutputStream **)(param_1 + 0x28),
               (vector<float,std::allocator<float>_> *)(in_ECX + 0x6c));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x78));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x78),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x7c));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x7c),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x80));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x80),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x84));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x84),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x88));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x88),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x8c));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x8c),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x94));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x94),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x90));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x90),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x98));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x98),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0x9c));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0x9c),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xa0));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xa0),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xa4));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xa4),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xa8));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xa8),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xac));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xac),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0xb0),*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(float *)pCVar3);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),(char)*(undefined4 *)(in_ECX + 0xb0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xb4));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xb4),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xb8));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xb8),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xbc));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xbc),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xc0));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xc0),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&stack0x00000000,
               *(CMemoryDataInputStream **)(param_1 + 0x24),(float *)(in_ECX + 0xc4));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),SUB41(*(float *)(in_ECX + 0xc4),0));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 0xcd));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 0xcd));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 200));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 200));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 0xc9));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 0xc9));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 0xca));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 0xca));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 0xcb));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 0xcb));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 0xcc));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 0xcc));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseMapBinaryIn<long,long,std::less<long>_>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (map<long,long,std::less<long>,std::allocator<std::pair<long_const_,long>_>_> *)
               (in_ECX + 0xd0));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_00466848();
  }
  CEngineLightingManager::UpdateShadowScene
            ((CEngineLightingManager *)param_1,(CEngineCamera *)(in_ECX + 0xdc));
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&stack0x00000000,*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,(bool *)(in_ECX + 0xe4));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(param_1 + 0x28),*(bool *)(in_ECX + 0xe4));
    return;
  }
  return;
}



//=== CThingBaseDef @ 00676be0 ===

void _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  byte bVar1;
  byte bVar2;
  int in_ECX;
  float *unaff_EDI;
  undefined4 uStack_4;
  
  this = param_1;
  uStack_4 = in_ECX;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CThingComponentSet::TransferBinaryIn
              ((CThingComponentSet *)(in_ECX + 0x39),*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CThingComponentSet::TransferBinaryOut
              ((CThingComponentSet *)(in_ECX + 0x39),*(CDataOutputStream **)(this + 0x28));
  }
  operator_delete__((void *)0x0);
  param_1 = (CPersistContext *)(CONCAT31(param_1._1_3_,*(undefined1 *)(in_ECX + 0x41)) & 0xffffff01)
  ;
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            (this,"AvailableInEditor",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x41) =
       *(byte *)(in_ECX + 0x41) ^ (*(byte *)(in_ECX + 0x41) ^ (byte)param_1._0_1_) & 1;
  param_1 = (CPersistContext *)(CONCAT31(param_1._1_3_,*(byte *)(in_ECX + 0x42) >> 1) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"GameStatePersisted",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x42) =
       *(byte *)(in_ECX + 0x42) ^ ((char)param_1._0_1_ << 1 ^ *(byte *)(in_ECX + 0x42)) & 2;
  param_1 = (CPersistContext *)(CONCAT31(param_1._1_3_,*(byte *)(in_ECX + 0x41) >> 3) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"LevelLess",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x41);
  *(byte *)(in_ECX + 0x41) = bVar1 ^ ((char)param_1._0_1_ << 3 ^ bVar1) & 8;
  param_1 = (CPersistContext *)(CONCAT31(param_1._1_3_,bVar1 >> 4) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"IsDrawable",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x41) ^ ((char)param_1._0_1_ << 4 ^ *(byte *)(in_ECX + 0x41)) & 0x10;
  *(byte *)(in_ECX + 0x41) = bVar1;
  param_1 = (CPersistContext *)(CONCAT31(param_1._1_3_,bVar1 >> 1) & 0xffffff01);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            (this,"AlwaysFrameUpdate",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x41);
  *(byte *)(in_ECX + 0x41) = bVar1 ^ ((char)param_1._0_1_ << 1 ^ bVar1) & 2;
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 7);
  CPersistContext::Transfer<signed_char>
            (this,"InMapSearches",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x41);
  *(byte *)(in_ECX + 0x41) = bVar1 & 0x7f | (char)param_1._0_1_ << 7;
  param_1._0_1_ = (CPersistTraits<float>)((bVar1 & 4) >> 2);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"StartKillLocked",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x41) =
       *(byte *)(in_ECX + 0x41) ^ ((char)param_1._0_1_ << 2 ^ *(byte *)(in_ECX + 0x41)) & 4;
  param_1._0_1_ = (CPersistTraits<float>)(*(byte *)(in_ECX + 0x42) & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"IsLoadableGlobal",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x42);
  *(byte *)(in_ECX + 0x42) = bVar1 ^ (*(byte *)(in_ECX + 0x42) ^ (byte)param_1._0_1_) & 1;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 2 & 1);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            (this,"CanComeBetweenCameraAndObservedThing",(char *)&param_1,
             (char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x42);
  *(byte *)(in_ECX + 0x42) = bVar1 ^ ((char)param_1._0_1_ << 2 ^ bVar1) & 4;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 5 & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"IsVulnerableToMelee",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x42);
  *(byte *)(in_ECX + 0x42) = bVar1 ^ ((char)param_1._0_1_ << 5 ^ bVar1) & 0x20;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 6 & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"Damageable",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x42) =
       *(byte *)(in_ECX + 0x42) ^ ((char)param_1._0_1_ << 6 ^ *(byte *)(in_ECX + 0x42)) & 0x40;
  param_1._0_1_ = (CPersistTraits<float>)(*(byte *)(in_ECX + 0x41) >> 6 & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"ForceNoSerialise",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x41) =
       *(byte *)(in_ECX + 0x41) ^ ((char)param_1._0_1_ << 6 ^ *(byte *)(in_ECX + 0x41)) & 0x40;
  param_1._0_1_ = (CPersistTraits<float>)(*(byte *)(in_ECX + 0x42) >> 3 & 1);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            (this,"DrawWeaponTrails",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x42);
  *(byte *)(in_ECX + 0x42) = bVar1 ^ ((char)param_1._0_1_ << 3 ^ bVar1) & 8;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 4 & 1);
  uStack_4 = CONCAT13(1,(undefined3)uStack_4);
  CPersistContext::Transfer<signed_char>
            (this,"DrawProjectileWeaponTrails",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x42);
  bVar2 = (char)param_1._0_1_ << 4;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 7);
  *(byte *)(in_ECX + 0x42) = bVar1 ^ (bVar2 ^ bVar1) & 0x10;
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"AlwaysIncludeInObjectStrikeScans",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x42) = *(byte *)(in_ECX + 0x42) & 0x7f | (char)param_1._0_1_ << 7;
  param_1._0_1_ = (CPersistTraits<float>)(*(byte *)(in_ECX + 0x43) & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"DisableOcclusionTesting",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x43);
  *(byte *)(in_ECX + 0x43) = bVar1 ^ (*(byte *)(in_ECX + 0x43) ^ (byte)param_1._0_1_) & 1;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 1 & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"DrawAfterWater",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x43);
  *(byte *)(in_ECX + 0x43) = bVar1 ^ ((char)param_1._0_1_ << 1 ^ bVar1) & 2;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 2 & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"ForceRenderedLastFrameOverride",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x43);
  *(byte *)(in_ECX + 0x43) = bVar1 ^ ((char)param_1._0_1_ << 2 ^ bVar1) & 4;
  param_1._0_1_ = (CPersistTraits<float>)(bVar1 >> 3 & 1);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"AddToComboMultiplierOnHit",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  bVar1 = *(byte *)(in_ECX + 0x43);
  *(byte *)(in_ECX + 0x43) = bVar1 ^ ((char)param_1._0_1_ << 3 ^ bVar1) & 8;
  param_1 = (CPersistContext *)(CONCAT31(param_1._1_3_,bVar1 >> 4) & 0xffffff01);
  uStack_4 = uStack_4 & 0xffffff;
  CPersistContext::Transfer<signed_char>
            (this,"GiveHeroStatChangesOnBeingHit",(char *)&param_1,(char *)((int)&uStack_4 + 3));
  *(byte *)(in_ECX + 0x43) =
       *(byte *)(in_ECX + 0x43) ^ ((char)param_1._0_1_ << 4 ^ *(byte *)(in_ECX + 0x43)) & 0x10;
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x44));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x44),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x48));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x4c));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x4c),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x50));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x54));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x58));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x5c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x60));
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
              ((CPersistTraits<float> *)(in_ECX + 0x68),*(CMemoryDataInputStream **)(this + 0x24),
               unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x68));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0x6c),*(CMemoryDataInputStream **)(this + 0x24),
               unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x6c));
    return;
  }
  return;
}



//=== CActionUseDef @ 006773f0 ===

void _Transfer_CActionUseDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  float *unaff_EDI;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0x28),*(CMemoryDataInputStream **)(this + 0x24),
               unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x28));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x2c));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x2c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x2d));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x2d));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x30));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x30),0));
    return;
  }
  return;
}



//=== COpinionReactionManagerDef @ 00679740 ===

void _Transfer_COpinionReactionManagerDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_> *unaff_ESI;
  vector<CEngineLocalDetailObjectDef,std::allocator<CEngineLocalDetailObjectDef>_> *unaff_EDI;
  
  CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>
            (param_1,(char *)(in_ECX + 0x3c),unaff_EDI);
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(param_1 + 0x24),(CMemoryDataInputStream *)unaff_EDI,unaff_ESI);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0067b8b0();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(param_1 + 0x24),(CMemoryDataInputStream *)unaff_EDI,unaff_ESI);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0067b930();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(param_1 + 0x24),(CMemoryDataInputStream *)unaff_EDI,unaff_ESI);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0067b9b0();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>::
    TransferBinaryIn(*(CPersistTraits<std::list<COpinionTransientOffset,std::allocator<COpinionTransientOffset>_>_>
                       **)(param_1 + 0x24),(CMemoryDataInputStream *)unaff_EDI,unaff_ESI);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_004ecd6b();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseMapBinaryIn<bool,EOpinionAttitudeType,std::less<EOpinionAttitudeType>_>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (map<EOpinionAttitudeType,bool,std::less<EOpinionAttitudeType>,std::allocator<std::pair<EOpinionAttitudeType_const_,bool>_>_>
                *)(in_ECX + 0x78));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0045df1e();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseMapBinaryIn<bool,EOpinionAttitudeType,std::less<EOpinionAttitudeType>_>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (map<EOpinionAttitudeType,bool,std::less<EOpinionAttitudeType>,std::allocator<std::pair<EOpinionAttitudeType_const_,bool>_>_>
                *)(in_ECX + 0x84));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0045df1e();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseMapBinaryIn<bool,EOpinionAttitudeType,std::less<EOpinionAttitudeType>_>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (map<EOpinionAttitudeType,bool,std::less<EOpinionAttitudeType>,std::allocator<std::pair<EOpinionAttitudeType_const_,bool>_>_>
                *)(in_ECX + 0x90));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0045df1e();
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    GFSerialiseMapBinaryIn<bool,EOpinionAttitudeType,std::less<EOpinionAttitudeType>_>
              (*(CMemoryDataInputStream **)(param_1 + 0x24),
               (map<EOpinionAttitudeType,bool,std::less<EOpinionAttitudeType>,std::allocator<std::pair<EOpinionAttitudeType_const_,bool>_>_>
                *)(in_ECX + 0x9c));
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    FUN_0045df1e();
  }
  FUN_0067bfb0(in_ECX + 0xa8);
  CPersistContext_TransferableComponent<CReactionFrequencyTraitsArray>::TransferOut
            ((CPersistContext_TransferableComponent<CReactionFrequencyTraitsArray> *)param_1,
             (CPersistContext *)(in_ECX + 0xac),(CCharString *)unaff_EDI);
  return;
}



//=== CBlurredAttitudesDef @ 0067cdb0 ===

void _Transfer_CBlurredAttitudesDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== CDegradableDef @ 0067e290 ===

void _Transfer_CDegradableDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  float *pfVar1;
  vector<CDegradableInfo,std::allocator<CDegradableInfo>_> *pvVar2;
  int iVar3;
  float fVar4;
  CPersistContext *this;
  int iVar5;
  int in_ECX;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x25));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x25));
  }
  pfVar1 = (float *)(in_ECX + 0x28);
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),pfVar1);
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE(*(CDataOutputStream **)(this + 0x28),SUB41(*pfVar1,0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x2c));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x2c));
  }
  pvVar2 = (vector<CDegradableInfo,std::allocator<CDegradableInfo>_> *)(in_ECX + 0x30);
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    GFSerialiseVectorBinaryIn<CBrainUpdateZone>
              (*(CMemoryDataInputStream **)(this + 0x24),
               (vector<CBrainUpdateZone,std::allocator<CBrainUpdateZone>_> *)pvVar2);
  }
  else {
    GFSerialiseVectorBinaryOut<CDegradableInfo>(*(CDataOutputStream **)(this + 0x28),pvVar2);
  }
  if (*(int *)(this + 0x18) == 0) {
    FUN_0067f3d0(param_1);
    iVar3 = *(int *)(in_ECX + 0x34);
    fVar4 = *pfVar1;
    for (iVar5 = *(int *)pvVar2; iVar5 != iVar3; iVar5 = iVar5 + 0x1c) {
      *(char *)(iVar5 + 0x11) = SUB41(fVar4,0);
    }
  }
  return;
}



//=== CHeroExperienceDef @ 006d8f10 ===

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



//=== CCreatureGeneratorDef @ 0074c5f0 ===

void _Transfer_CCreatureGeneratorDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  float *unaff_EDI;
  
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
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0x34),*(CMemoryDataInputStream **)(this + 0x24),
               unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x34));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x38));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x38),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x3c));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x3c),0));
    return;
  }
  return;
}



//=== CHighlightItemDef @ 0077ce00 ===

void _Transfer_CHighlightItemDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x40));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x40));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x28));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x28),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x2c));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x2c),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x30));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x34));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x3c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x38));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x44));
    return;
  }
  return;
}



//=== CHeroCentreDef @ 0077d3a0 ===

void _Transfer_CHeroCentreDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== CAIScratchpadDef @ 007ad300 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall CAIScratchpadDef::Transfer(class CPersistContext &) */

void __thiscall
CAIScratchpadDef::_Transfer_CAIScratchpadDef__UAEXAAVCPersistContext___Z
          (CAIScratchpadDef *this,CPersistContext *param_1)

{
  CPersistContext *this_00;
  CIntelligentPointer<class_CThing> *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  char local_10 [4];
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  this_00 = param_1;
  local_10[0] = '\0';
  local_10[1] = '\0';
  local_10[2] = '\0';
  local_10[3] = '\0';
  CPersistContext::Transfer<signed_char>(param_1,"ThankingPhrase",(char *)(this + 0x28),local_10);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"IgnoringPhrase",(char *)(this + 0x2c),(char *)&param_1);
  uStack_c = 0;
  uStack_8 = 0;
  uStack_4 = 0;
  CPersistContext::Transfer<CFloatRange>
            (this_00,"WanderCentrePoint",(CFloatRange *)(this + 0x30),(CFloatRange *)&uStack_c);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"WanderMinDistance",(char *)(this + 0x3c),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"WanderMaxDistance",(char *)(this + 0x40),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"GossipCounter",(char *)(this + 0x44),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"MaxGossipPhrase",(char *)(this + 0x48),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"WarningPhrase",(char *)(this + 0x4c),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"BeerRequestPhrase",(char *)(this + 0x50),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"ScriptingStateGroup",(char *)(this + 0x54),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"MaxHeroReactionDistance",(char *)(this + 0x58),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"ActionFrequency",(char *)(this + 0x5c),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"ActionFrequencyVariation",(char *)(this + 0x60),(char *)&param_1);
  pCVar1 = (CIntelligentPointer<class_CThing> *)
           CCharString::CCharString((CCharString *)&param_1,"",-1);
  CPersistContext::Transfer<CIntelligentPointer<CThing>_>
            (this_00,"Action",(CIntelligentPointer<CThing> *)(this + 100),pCVar1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,extraout_EDX,
             unaff_EDI);
  param_1 = (CPersistContext *)((uint)param_1 & 0xffffff00);
  CPersistContext::Transfer<signed_char>
            (this_00,"FaceHeroForAction",(char *)(this + 0x68),(char *)&param_1);
  pCVar1 = (CIntelligentPointer<class_CThing> *)
           CCharString::CCharString((CCharString *)&param_1,"",-1);
  CPersistContext::Transfer<CIntelligentPointer<CThing>_>
            (this_00,"TargetName",(CIntelligentPointer<CThing> *)(this + 0x6c),pCVar1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_00,unaff_EDI);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"FollowDistance",(char *)(this + 0x70),(char *)&param_1);
  param_1 = (CPersistContext *)((uint)param_1 & 0xffffff00);
  CPersistContext::Transfer<signed_char>
            (this_00,"AttackHeroOnSight",(char *)(this + 0x74),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"TimeToSpendHarassingHero",(char *)(this + 0x78),(char *)&param_1);
  param_1 = (CPersistContext *)0x40a00000;
  CPersistContext::Transfer<signed_char>
            (this_00,"CombatNearbyEnemyFleeingBreakOffRange",(char *)(this + 0x7c),(char *)&param_1)
  ;
  param_1 = (CPersistContext *)0x41200000;
  CPersistContext::Transfer<signed_char>
            (this_00,"CombatNearbyBreakOffRange",(char *)(this + 0x80),(char *)&param_1);
  param_1 = (CPersistContext *)((uint)param_1 & 0xffffff00);
  CPersistContext::Transfer<signed_char>
            (this_00,"StealStealableItems",(char *)(this + 0x84),(char *)&param_1);
  param_1 = (CPersistContext *)((uint)param_1 & 0xffffff00);
  CPersistContext::Transfer<signed_char>
            (this_00,"RecoverStealableItems",(char *)(this + 0x85),(char *)&param_1);
  param_1 = (CPersistContext *)((uint)param_1 & 0xffffff00);
  CPersistContext::Transfer<signed_char>
            (this_00,"TakeStealableItemToRandomDestination",(char *)(this + 0x86),(char *)&param_1);
  param_1 = (CPersistContext *)CONCAT31(param_1._1_3_,1);
  CPersistContext::Transfer<signed_char>
            (this_00,"KillSelfAndStealableItemAfterReachingDestination",(char *)(this + 0x87),
             (char *)&param_1);
  param_1 = (CPersistContext *)CONCAT31(param_1._1_3_,1);
  CPersistContext::Transfer<signed_char>
            (this_00,"AllowedToFollow",(char *)(this + 0x88),(char *)&param_1);
  pCVar1 = (CIntelligentPointer<class_CThing> *)
           CCharString::CCharString((CCharString *)&param_1,"",-1);
  CPersistContext::Transfer<CIntelligentPointer<CThing>_>
            (this_00,"TableName",(CIntelligentPointer<CThing> *)(this + 0x8c),pCVar1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_01,unaff_EDI);
  pCVar1 = (CIntelligentPointer<class_CThing> *)
           CCharString::CCharString((CCharString *)&param_1,"",-1);
  CPersistContext::Transfer<CIntelligentPointer<CThing>_>
            (this_00,"SeatName",(CIntelligentPointer<CThing> *)(this + 0x90),pCVar1);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
             extraout_EDX_02,unaff_EDI);
  param_1 = (CPersistContext *)((uint)param_1 & 0xffffff00);
  CPersistContext::Transfer<signed_char>
            (this_00,"DisableHeadLooking",(char *)(this + 0x94),(char *)&param_1);
  param_1 = (CPersistContext *)CONCAT31(param_1._1_3_,1);
  CPersistContext::Transfer<signed_char>
            (this_00,"IsPushableByHero",(char *)(this + 0x95),(char *)&param_1);
  param_1 = (CPersistContext *)CONCAT31(param_1._1_3_,1);
  CPersistContext::Transfer<signed_char>
            (this_00,"LookForFiniteTime",(char *)(this + 0x96),(char *)&param_1);
  param_1 = (CPersistContext *)CONCAT31(param_1._1_3_,1);
  CPersistContext::Transfer<signed_char>
            (this_00,"AvoidRegionExits",(char *)(this + 0x97),(char *)&param_1);
  param_1 = (CPersistContext *)0x0;
  CPersistContext::Transfer<signed_char>
            (this_00,"TargetingDistanceOffset",(char *)(this + 0x98),(char *)&param_1);
  return;
}



//=== CFishingDef @ 007b1f10 ===

void _Transfer_CFishingDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  
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
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x34));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x38));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x3c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x40));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x44));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x44),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x48));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x4c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x50));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x54));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x58));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x5c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x60));
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
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x6c));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x70));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x70),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x74));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x74),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x78));
    return;
  }
  return;
}



//=== CActivateQuestDef @ 007b5740 ===

void _Transfer_CActivateQuestDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  float *unaff_EDI;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0x28),*(CMemoryDataInputStream **)(this + 0x24),
               unaff_EDI);
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x28));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x2c));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x2c));
    return;
  }
  return;
}



//=== COverheadDisplayDef @ 007b7d20 ===

void _Transfer_COverheadDisplayDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x25));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x25));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x26));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x26));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (bool *)(in_ECX + 0x27));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(this + 0x28),*(bool *)(in_ECX + 0x27));
    return;
  }
  return;
}



//=== CShoveHaPennyRegionDef @ 007d0070 ===

void _Transfer_CShoveHaPennyRegionDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  
  this = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x28));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x2c));
    return;
  }
  return;
}



//=== CTavernGameShoveHaPennyDef @ 007d0a40 ===

void _Transfer_CTavernGameShoveHaPennyDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *this;
  int in_ECX;
  vector<CEngineLocalDetailObjectDef,std::allocator<CEngineLocalDetailObjectDef>_> *unaff_EDI;
  
  this = param_1;
  NTCScriptedControl::CActionTalkBase::_Transfer_CTavernGameDef__UAEXAAVCPersistContext___Z();
  CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>
            (this,(char *)(in_ECX + 0x1a4),unaff_EDI);
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x1b0));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x1b0),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x1b4));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x1b4),0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1cc));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1d0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1d4));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1d8));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1dc));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1e0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1e4));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1e8));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1ec));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 500));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1f0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1b8));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1bc));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1c0));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1c4));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1c8));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)(in_ECX + 0x1f8));
  }
  CPersistContext::CheckCRC(this,"");
  if (*(int *)(this + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)&param_1,*(CMemoryDataInputStream **)(this + 0x24),
               (float *)(in_ECX + 0x1fc));
  }
  else if (*(int *)(this + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(this + 0x28),SUB41(*(float *)(in_ECX + 0x1fc),0));
    return;
  }
  return;
}



//=== CCarriedReadableDef @ 007e1e30 ===

void _Transfer_CCarriedReadableDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  float *unaff_EDI;
  
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0x28),*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,unaff_EDI);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),(char)*(undefined4 *)(in_ECX + 0x28));
  }
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(param_1 + 0x18) == 2) {
    CPersistTraits<float>::TransferBinaryIn
              ((CPersistTraits<float> *)(in_ECX + 0x2c),*(CMemoryDataInputStream **)(param_1 + 0x24)
               ,unaff_EDI);
  }
  else if (*(int *)(param_1 + 0x18) == 3) {
    CDataOutputStream::WriteCBYTE
              (*(CDataOutputStream **)(param_1 + 0x28),(char)*(undefined4 *)(in_ECX + 0x2c));
    return;
  }
  return;
}



//=== CReadableDef @ 007e81b0 ===

void _Transfer_CReadableDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  CPersistContext *pCVar1;
  int in_ECX;
  
  pCVar1 = param_1;
  CPersistContext::CheckCRC(param_1,"");
  if (*(int *)(pCVar1 + 0x18) == 2) {
    CPersistTraits<bool>::TransferBinaryIn
              ((CPersistTraits<bool> *)&param_1,*(CMemoryDataInputStream **)(pCVar1 + 0x24),
               (bool *)(in_ECX + 0x25));
  }
  else if (*(int *)(pCVar1 + 0x18) == 3) {
    CDataOutputStream::WriteEBOOL(*(CDataOutputStream **)(pCVar1 + 0x28),*(bool *)(in_ECX + 0x25));
    return;
  }
  return;
}



