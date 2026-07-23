//=== ?GetInstantiationName@CDefClassBase@@UBE?AVCDefString@@XZ @ 0042da10 ===

void _GetInstantiationName_CDefClassBase__UBE_AVCDefString__XZ(undefined4 *param_1)

{
  *param_1 = 0xffffffff;
  return;
}



//=== ?SetInstantiationName@CDefClassBase@@UAEXVCDefString@@@Z @ 0042da20 ===

void _SetInstantiationName_CDefClassBase__UAEXVCDefString___Z(void)

{
  return;
}



//=== ?GetInstantiationName@CParentDefClassBase@@UBE?AVCDefString@@XZ @ 004303b0 ===

void _GetInstantiationName_CParentDefClassBase__UBE_AVCDefString__XZ(undefined4 *param_1)

{
  int in_ECX;
  
  *param_1 = *(undefined4 *)(in_ECX + 0x25);
  return;
}



//=== ?SetInstantiationName@CParentDefClassBase@@UAEXVCDefString@@@Z @ 004303c0 ===

void _SetInstantiationName_CParentDefClassBase__UAEXVCDefString___Z(undefined4 param_1)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0x25) = param_1;
  return;
}



//=== ?Transfer@CControlsDef@@UAEXAAVCPersistContext@@@Z @ 004330f0 ===

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



//=== ?Transfer@CFrontEndDef@@UAEXAAVCPersistContext@@@Z @ 004331c0 ===

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



//=== ?Transfer@CEngineDef@@UAEXAAVCPersistContext@@@Z @ 00433fe0 ===

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



//=== ?Transfer@CEngineVideoOptionsDef@@UAEXAAVCPersistContext@@@Z @ 00434120 ===

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



//=== ?Transfer@CConfigOptionsDefaultsDef@@UAEXAAVCPersistContext@@@Z @ 00434290 ===

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



//=== ?Transfer@CThingMultiArrowShotDef@@UAEXAAVCPersistContext@@@Z @ 00451ca2 ===

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



//=== ?Transfer@CInventoryItemDef@@UAEXAAVCPersistContext@@@Z @ 00451d28 ===

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



//=== ?Transfer@CPhysicsDef@@UAEXAAVCPersistContext@@@Z @ 0045212a ===

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



//=== ?Transfer@CTCNoiseDef@@UAEXAAVCPersistContext@@@Z @ 00452184 ===

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



//=== ?Transfer@CCarrySlotDef@@UAEXAAVCPersistContext@@@Z @ 00452256 ===

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



//=== ?Transfer@CSmashableDef@@UAEXAAVCPersistContext@@@Z @ 004522c6 ===

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



//=== ?Transfer@CSpecialAbilitiesDrainLifeDataDef@@UAEXAAVCPersistContext@@@Z @ 00452593 ===

void _Transfer_CSpecialAbilitiesDrainLifeDataDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CSpecialAbilitiesForcePushDataDef@@UAEXAAVCPersistContext@@@Z @ 004525a3 ===

void _Transfer_CSpecialAbilitiesForcePushDataDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CCreatureStatsDef@@UAEXAAVCPersistContext@@@Z @ 004525e9 ===

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



//=== ?Transfer@CCoopSpiritDef@@UAEXAAVCPersistContext@@@Z @ 0045266f ===

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



//=== ?Transfer@CContextSensitiveItemDef@@UAEXAAVCPersistContext@@@Z @ 004526b3 ===

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



//=== ?Transfer@CBonusItemDef@@UAEXAAVCPersistContext@@@Z @ 0045279b ===

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



//=== ?Transfer@CTattooDef@@UAEXAAVCPersistContext@@@Z @ 004529ae ===

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



//=== ?Transfer@CHairCardDef@@UAEXAAVCPersistContext@@@Z @ 00452b04 ===

void _Transfer_CHairCardDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CCombatAbilityAttackBase@@UAEXAAVCPersistContext@@@Z @ 00452b14 ===

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



//=== ?Transfer@CCombatAbilityStrafeDef@@UAEXAAVCPersistContext@@@Z @ 00452b93 ===

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



//=== ?Transfer@CCombatAbilityUseProjectileWeaponDef@@UAEXAAVCPersistContext@@@Z @ 00452bb4 ===

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



//=== ?Transfer@CDragonActionSwoopDef@@UAEXAAVCPersistContext@@@Z @ 00452be2 ===

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



//=== ?Transfer@CDragonActionNapalmDef@@UAEXAAVCPersistContext@@@Z @ 00452c94 ===

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



//=== ?Transfer@CDragonActionHoverDef@@UAEXAAVCPersistContext@@@Z @ 00452ccd ===

void _Transfer_CDragonActionHoverDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CAugmentationDef@@UAEXAAVCPersistContext@@@Z @ 00452cdd ===

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



//=== ?Transfer@CAbilityDef@@UAEXAAVCPersistContext@@@Z @ 00452df1 ===

void _Transfer_CAbilityDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CJackDragonDef@@UAEXAAVCPersistContext@@@Z @ 00452ea5 ===

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



//=== ?Transfer@CBriarRoseDef@@UAEXAAVCPersistContext@@@Z @ 00452f8e ===

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



//=== ?Transfer@CSnowTrollDef@@UAEXAAVCPersistContext@@@Z @ 00453014 ===

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



//=== ?Transfer@CGuildMasterDef@@UAEXAAVCPersistContext@@@Z @ 004530bb ===

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



//=== ?Transfer@CSummonerDef@@UAEXAAVCPersistContext@@@Z @ 004530dc ===

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



//=== ?Transfer@CExplosiveTrailDef@@UAEXAAVCPersistContext@@@Z @ 00453178 ===

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



//=== ?Transfer@CThingBuildingDef@@UAEXAAVCPersistContext@@@Z @ 00454b8e ===

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



//=== ?Transfer@CThingHolySiteDef@@UAEXAAVCPersistContext@@@Z @ 00454dcb ===

void _Transfer_CThingHolySiteDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x7e));
  return;
}



//=== ?Transfer@CThingMarkerDef@@UAEXAAVCPersistContext@@@Z @ 00454e21 ===

void _Transfer_CThingMarkerDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x80));
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x84));
  return;
}



//=== ?Transfer@CThingSwitchDef@@UAEXAAVCPersistContext@@@Z @ 00454f15 ===

void _Transfer_CThingSwitchDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== ?Transfer@CThingVillageDef@@UAEXAAVCPersistContext@@@Z @ 00454f5f ===

void _Transfer_CThingVillageDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x70));
  return;
}



//=== ?Transfer@CThingPhysicalSwitchDef@@UAEXAAVCPersistContext@@@Z @ 00454fc3 ===

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



//=== ?Transfer@CInventoryCategoryDef@@UAEXAAVCPersistContext@@@Z @ 00455588 ===

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



//=== ?Transfer@CMessageEventDef@@UAEXAAVCPersistContext@@@Z @ 004556ce ===

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



//=== ?Transfer@CEnvironmentDef@@UAEXAAVCPersistContext@@@Z @ 004558df ===

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



//=== ?Transfer@CThingNoiseDef@@UAEXAAVCPersistContext@@@Z @ 00455f1a ===

void _Transfer_CThingNoiseDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  
  _Transfer_CThingBaseDef__UAEXAAVCPersistContext___Z(param_1);
  CPersistContext::TransferSubComponent<CTCNoiseDef>(param_1,"Noise",(CTCNoiseDef *)(in_ECX + 0x70))
  ;
  return;
}



//=== ?Transfer@CCameraModeDef@@UAEXAAVCPersistContext@@@Z @ 00456564 ===

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



//=== ?Transfer@CCameraManagerSetDef@@UAEXAAVCPersistContext@@@Z @ 00456818 ===

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



//=== ?Transfer@CExpressionDef@@UAEXAAVCPersistContext@@@Z @ 004569a7 ===

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



//=== ?Transfer@CCombatDialogueDef@@UAEXAAVCPersistContext@@@Z @ 00456dfb ===

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



//=== ?Transfer@CVillagerInteractionsDef@@UAEXAAVCPersistContext@@@Z @ 00456f27 ===

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



//=== ?Transfer@CSpecialAbilitiesGhostSwordDef@@UAEXAAVCPersistContext@@@Z @ 00457a0d ===

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



//=== ?Transfer@CSpecialAbilitiesDrunkennessDef@@UAEXAAVCPersistContext@@@Z @ 00457f13 ===

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



//=== ?Transfer@CSpecialAbilitiesThunderLightningStormDef@@UAEXAAVCPersistContext@@@Z @ 00457f91 ===

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



//=== ?Transfer@CHitLocationDef@@UAEXAAVCPersistContext@@@Z @ 004585fe ===

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



//=== ?Transfer@CLightningDef@@UAEXAAVCPersistContext@@@Z @ 00458911 ===

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



//=== ?Transfer@CMeleeCombatKnockdownEffects@@UAEXAAVCPersistContext@@@Z @ 00458c9d ===

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



//=== ?Transfer@CThingShotDef@@UAEXAAVCPersistContext@@@Z @ 00458d6f ===

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



//=== ?Transfer@CThingObjectDef@@UAEXAAVCPersistContext@@@Z @ 0045912f ===

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



//=== ?Transfer@CCreatureAbilityDef@@UAEXAAVCPersistContext@@@Z @ 00459353 ===

void _Transfer_CCreatureAbilityDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== ?Transfer@CThingGroupDef@@UAEXAAVCPersistContext@@@Z @ 0045954d ===

void _Transfer_CThingGroupDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== ?Transfer@CPlayerDef@@UAEXAAVCPersistContext@@@Z @ 00459582 ===

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



//=== ?Transfer@CPlayerMovementDef@@UAEXAAVCPersistContext@@@Z @ 00459615 ===

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



//=== ?Transfer@CSimBuildingDef@@UAEXAAVCPersistContext@@@Z @ 00459704 ===

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



//=== ?Transfer@CEngineThemeGroupDef@@UAEXAAVCPersistContext@@@Z @ 00459764 ===

void _Transfer_CEngineThemeGroupDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== ?Transfer@CEngineThemeDef@@UAEXAAVCPersistContext@@@Z @ 0045979b ===

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



//=== ?Transfer@CSpecialEffectsDef@@UAEXAAVCPersistContext@@@Z @ 0045c8c3 ===

void _Transfer_CSpecialEffectsDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_0045c8d3(in_ECX + 0x28);
  return;
}



//=== ?Transfer@CTargetingDef@@UAEXAAVCPersistContext@@@Z @ 0045d923 ===

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



//=== ?Transfer@COpinionReactionMaskDef@@UAEXAAVCPersistContext@@@Z @ 0045de67 ===

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



//=== ?Transfer@COpinionDeedMaskDef@@UAEXAAVCPersistContext@@@Z @ 0045e2e9 ===

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



//=== ?Transfer@COpinionPersonalityDef@@UAEXAAVCPersistContext@@@Z @ 0045e7cf ===

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



//=== ?Transfer@COpinionSourceDef@@UAEXAAVCPersistContext@@@Z @ 0045f178 ===

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



//=== ?Transfer@CArmourDef@@UAEXAAVCPersistContext@@@Z @ 004615fb ===

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



//=== ?Transfer@CSoundThemeDef@@UAEXAAVCPersistContext@@@Z @ 0046329e ===

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



//=== ?Transfer@CMeleeCombatAbilityDef@@UAEXAAVCPersistContext@@@Z @ 00463bff ===

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



//=== ?Transfer@CSimVoicesDef@@UAEXAAVCPersistContext@@@Z @ 00464b8a ===

void _Transfer_CSimVoicesDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_00464b9a(in_ECX + 0x3c);
  return;
}



//=== ?Transfer@CSpecialAbilitiesAssassinRushDef@@UAEXAAVCPersistContext@@@Z @ 00464be9 ===

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



//=== ?Transfer@CSpecialAbilitiesBulletTimeDef@@UAEXAAVCPersistContext@@@Z @ 00464db5 ===

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



//=== ?Transfer@CSpecialAbilitiesForcePushDef@@UAEXAAVCPersistContext@@@Z @ 00465163 ===

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



//=== ?Transfer@CSpecialAbilitiesEnflameDef@@UAEXAAVCPersistContext@@@Z @ 00465258 ===

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



//=== ?Transfer@CSpecialAbilitiesPhysicalShieldDef@@UAEXAAVCPersistContext@@@Z @ 00465390 ===

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



//=== ?Transfer@CSpecialAbilitiesHealLifeDef@@UAEXAAVCPersistContext@@@Z @ 00465634 ===

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



//=== ?Transfer@CSpecialAbilitiesTurncoatSpellDef@@UAEXAAVCPersistContext@@@Z @ 00465662 ===

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



//=== ?Transfer@CSpecialAbilitiesFireballSpellDef@@UAEXAAVCPersistContext@@@Z @ 004656a6 ===

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



//=== ?Transfer@CSpecialAbilitiesDivineWrathDef@@UAEXAAVCPersistContext@@@Z @ 004656df ===

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



//=== GFSerialiseVectorBinaryIn<CDefString> @ 00465852 ===

/* [bsim sim=0.8269882226338185 <- ego_r]
   void __fastcall GFSerialiseVectorBinaryIn<class CDefString>(class CMemoryDataInputStream &,class
   std::vector<class CDefString,class std::allocator<class CDefString> > &) */

void __fastcall
GFSerialiseVectorBinaryIn<CDefString>
          (CMemoryDataInputStream *param_1,vector<CDefString,std::allocator<CDefString>_> *param_2)

{
  int iVar1;
  float *unaff_EDI;
  pair<long,long> *local_8;
  
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&local_8,4);
    }
    else {
      local_8 = (pair<long,long> *)**(undefined4 **)(param_1 + 0xc);
      *(undefined4 **)(param_1 + 0xc) = *(undefined4 **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  CArray<std::pair<long,long>_>::push_back((CArray<std::pair<long,long>_> *)param_2,local_8);
  iVar1 = 0;
  if (0 < (int)local_8) {
    do {
      CPersistTraits<float>::TransferBinaryIn
                ((CPersistTraits<float> *)(*(int *)param_2 + iVar1 * 4),param_1,unaff_EDI);
      iVar1 = iVar1 + 1;
    } while (iVar1 < (int)local_8);
  }
  return;
}



//=== ?Transfer@CSpecialAbilitiesUnholyPowerDef@@UAEXAAVCPersistContext@@@Z @ 00465b41 ===

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



//=== ?Transfer@CCombatAbilityBlockDefBase@@UAEXAAVCPersistContext@@@Z @ 00465c54 ===

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



//=== ?Transfer@CBrainDef@@UAEXAAVCPersistContext@@@Z @ 00466b37 ===

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



//=== ?Transfer@CPlayerInventoryDef@@UAEXAAVCPersistContext@@@Z @ 00467553 ===

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



//=== ?Transfer@CInventoryDef@@UAEXAAVCPersistContext@@@Z @ 004678e2 ===

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



//=== ?Transfer@CMaterialDef@@UAEXAAVCPersistContext@@@Z @ 0046923d ===

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



//=== GFSerialiseMapBinaryIn<CDefString,EObjectAugmentationType,std::less<EObjectAugmentationType>_> @ 00469381 ===

/* [bsim sim=0.853810610611717 <- ego_r]
   void __fastcall GFSerialiseMapBinaryIn<class CDefString,enum EObjectAugmentationType,struct
   std::less<enum EObjectAugmentationType> >(class CMemoryDataInputStream &,class std::map<enum
   EObjectAugmentationType,class CDefString,struct std::less<enum EObjectAugmentationType>,class
   std::allocator<struct std::pair<enum EObjectAugmentationType const ,class CDefString> > > &) */

void __fastcall
GFSerialiseMapBinaryIn<CDefString,EObjectAugmentationType,std::less<EObjectAugmentationType>_>
          (CMemoryDataInputStream *param_1,
          map<EObjectAugmentationType,CDefString,std::less<EObjectAugmentationType>,std::allocator<std::pair<EObjectAugmentationType_const_,CDefString>_>_>
          *param_2)

{
  float local_3c;
  undefined4 local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  float local_28;
  undefined4 local_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined1 local_14 [8];
  map<EObjectAugmentationType,CDefString,std::less<EObjectAugmentationType>,std::allocator<std::pair<EObjectAugmentationType_const_,CDefString>_>_>
  *local_c;
  int local_8;
  
  local_c = param_2;
  CTCCarryable::OnKill((CTCCarryable *)param_2);
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&local_8,4);
    }
    else {
      local_8 = **(int **)(param_1 + 0xc);
      *(int **)(param_1 + 0xc) = *(int **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  if (0 < local_8) {
    do {
      CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo((CVsyncCallbackTimerInfo *)&local_28);
      CPersistTraits<float>::TransferBinaryIn((CPersistTraits<float> *)&local_c,param_1,&local_28);
      CBlendedParticleEffectSet::TransferBinaryIn((CBlendedParticleEffectSet *)&local_24,param_1);
      local_3c = local_28;
      local_38 = local_24;
      uStack_34 = uStack_20;
      uStack_30 = uStack_1c;
      uStack_2c = uStack_18;
      FUN_004695b3(local_14,&local_3c);
      local_8 = local_8 + -1;
    } while (local_8 != 0);
  }
  return;
}



//=== ?Transfer@CCreatureGenerationFamilyDef@@UAEXAAVCPersistContext@@@Z @ 004696c1 ===

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



//=== ?Transfer@CTextureReplacementDef@@UAEXAAVCPersistContext@@@Z @ 00469c2e ===

void _Transfer_CTextureReplacementDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CCameraManagerDef@@UAEXAAVCPersistContext@@@Z @ 0046a0b0 ===

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



//=== ?Transfer@CSpecialAbilitiesBattleChargeDef@@UAEXAAVCPersistContext@@@Z @ 0046a178 ===

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



//=== ?Transfer@CSpecialAbilitiesMultiArrowDef@@UAEXAAVCPersistContext@@@Z @ 0046a260 ===

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



//=== ?Transfer@CSpecialAbilitiesBerserkDef@@UAEXAAVCPersistContext@@@Z @ 0046a2fc ===

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



//=== ?Transfer@CSpecialAbilitiesLightningSpellDef@@UAEXAAVCPersistContext@@@Z @ 0046a568 ===

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



//=== ?Transfer@CSpecialAbilitiesSummonSpellDef@@UAEXAAVCPersistContext@@@Z @ 0046a6dd ===

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



//=== ?Transfer@CSpecialAbilitiesBurntEffectDef@@UAEXAAVCPersistContext@@@Z @ 0046a742 ===

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



//=== ?Transfer@CSpecialAbilitiesElectrocutedEffectDef@@UAEXAAVCPersistContext@@@Z @ 0046a77b ===

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



//=== ?Transfer@CSpecialAbilitiesCreatureTintDef@@UAEXAAVCPersistContext@@@Z @ 0046a7e0 ===

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



//=== ?Transfer@CVillagePeopleDef@@UAEXAAVCPersistContext@@@Z @ 0046a80e ===

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



//=== ?Transfer@CSoundDef@@UAEXAAVCPersistContext@@@Z @ 0046a9ce ===

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



//=== ?Transfer@CAttackPatternDef@@UAEXAAVCPersistContext@@@Z @ 0046c284 ===

void _Transfer_CAttackPatternDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CObjectFamilyDef@@UAEXAAVCPersistContext@@@Z @ 0046c294 ===

void _Transfer_CObjectFamilyDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::GFTransferVectorField_CObjectFamilyDef_Objects(param_1);
  return;
}



//=== ?Transfer@CHitLocationsDef@@UAEXAAVCPersistContext@@@Z @ 0046c6e0 ===

void _Transfer_CHitLocationsDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CCombatSequenceDef@@UAEXAAVCPersistContext@@@Z @ 0046c6f0 ===

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



//=== ?Transfer@CQuickAccessItemButtonGuiDef@@UAEXAAVCPersistContext@@@Z @ 0046ee6a ===

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



//=== ?Transfer@CAbilityButtonGuiDef@@UAEXAAVCPersistContext@@@Z @ 0046f203 ===

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



//=== ?Transfer@CLensFlareElementDef@@UAEXAAVCPersistContext@@@Z @ 0046f727 ===

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



//=== ?Transfer@CCombatTypeDef@@UAEXAAVCPersistContext@@@Z @ 0046f7c8 ===

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



//=== ?Transfer@CEntitySoundDef@@UAEXAAVCPersistContext@@@Z @ 004702c8 ===

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



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 0047127a ===

/* [bsim sim=0.6849684911415447 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (undefined4 *param_1,undefined4 *param_2,undefined4 param_3)

{
  pair<unsigned_long,CDefString> *in_EAX;
  undefined4 *puVar1;
  
  if (param_1 != param_2) {
    while (puVar1 = param_1 + 2, puVar1 != param_2) {
      in_EAX = (pair<unsigned_long,CDefString> *)FUN_004712b2(*puVar1,param_1[3],param_3);
      param_1 = puVar1;
    }
  }
  return in_EAX;
}



//=== ?Transfer@CRegionDef@@UAEXAAVCPersistContext@@@Z @ 004715ab ===

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



//=== ?Transfer@CFactionDef@@UAEXAAVCPersistContext@@@Z @ 004717a1 ===

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



//=== ?Transfer@COpinionDeedReactionDef@@UAEXAAVCPersistContext@@@Z @ 00472095 ===

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



//=== ?Transfer@CSpecialAbilitiesMultiStrikeDef@@UAEXAAVCPersistContext@@@Z @ 00472415 ===

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



//=== ?Transfer@CSpecialAbilitiesDrainLifeDef@@UAEXAAVCPersistContext@@@Z @ 004724c2 ===

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



//=== ?Transfer@CGlobalDef@@UAEXAAVCPersistContext@@@Z @ 00472506 ===

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



//=== ?Transfer@CEnvironmentThemeDef@@UAEXAAVCPersistContext@@@Z @ 004732e0 ===

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



//=== ?Transfer@CAppearanceDef@@UAEXAAVCPersistContext@@@Z @ 00473504 ===

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



//=== ?Transfer@CPlayerGuiDef@@UAEXAAVCPersistContext@@@Z @ 004736c4 ===

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



//=== ?Transfer@CSkyDef@@UAEXAAVCPersistContext@@@Z @ 0047691e ===

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



//=== ?Transfer@CEnvironmentThemeDaySetDef@@UAEXAAVCPersistContext@@@Z @ 00476dc3 ===

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



//=== ?Transfer@CEngineLocalDetailObjectDef@@UAEXAAVCPersistContext@@@Z @ 0047769e ===

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



//=== ?Transfer@CEngineLocalDetailLayerDef@@UAEXAAVCPersistContext@@@Z @ 00477cb1 ===

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



//=== ?Transfer@CAbilityTrackLevelDef@@UAEXAAVCPersistContext@@@Z @ 00478827 ===

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



//=== ?Transfer@CAbilityTrackDef@@UAEXAAVCPersistContext@@@Z @ 00478ab7 ===

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



//=== ?Transfer@COpinionDeedEffectsDef@@UAEXAAVCPersistContext@@@Z @ 00478ee2 ===

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



//=== ?Transfer@CEngineLocalDetailGeneratorDef@@UAEXAAVCPersistContext@@@Z @ 0047989b ===

void _Transfer_CEngineLocalDetailGeneratorDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_retaddr;
  
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            (param_1,(char *)(in_ECX + 0x3c),unaff_retaddr);
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



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 0047c800 ===

/* [bsim sim=0.5696582825956504 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (pair<EHeroMorphType,CParticleMorphs::CEntry> *param_1,
          pair<EHeroMorphType,CParticleMorphs::CEntry> *param_2)

{
  int iVar1;
  pair<unsigned_long,CDefString> *in_EAX;
  pair<unsigned_long,CDefString> *extraout_EAX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  int iVar3;
  
  ppVar2 = param_2;
  for (; param_1 != param_2; param_1 = param_1 + 0x18) {
    iVar1 = *(int *)(param_1 + 0x10);
    for (iVar3 = *(int *)(param_1 + 0xc); iVar3 != iVar1; iVar3 = iVar3 + 0xc) {
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(iVar3 + 4),ppVar2
                 ,unaff_ESI);
      ppVar2 = extraout_EDX;
    }
    if (*(void **)(param_1 + 0xc) != (void *)0x0) {
      free(*(void **)(param_1 + 0xc));
      ppVar2 = extraout_EDX_00;
    }
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)(param_1 + 4),ppVar2
               ,unaff_ESI);
    ppVar2 = extraout_EDX_01;
    in_EAX = extraout_EAX;
  }
  return in_EAX;
}



//=== CPersistContextReadSection @ 00485cd2 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall CPersistContextReadSection::CPersistContextReadSection(void) */

CPersistContextReadSection * __thiscall
CPersistContextReadSection::CPersistContextReadSection(CPersistContextReadSection *this)

{
  ::C3DAnimationBlendState::C3DAnimationBlendState((C3DAnimationBlendState *)(this + 0x2c));
  ::CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase
            ((CAIStateGroupBehaviourBase *)(this + 0x48));
  CCharString::CCharString((CCharString *)(this + 0x70));
  return this;
}



//=== ?Transfer@CDrunkennessDef@@UAEXAAVCPersistContext@@@Z @ 004dc96f ===

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



//=== ?Transfer@CCarryableDef@@UAEXAAVCPersistContext@@@Z @ 004dd924 ===

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



//=== ?Transfer@CVillageMemberDef@@UAEXAAVCPersistContext@@@Z @ 004dd989 ===

void _Transfer_CVillageMemberDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x25));
  return;
}



//=== ?Transfer@CBedDef@@UAEXAAVCPersistContext@@@Z @ 004dd999 ===

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



//=== ?Transfer@CSmokeGeneratorDef@@UAEXAAVCPersistContext@@@Z @ 004dd9dd ===

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



//=== ?Transfer@CCreatureNavigationDef@@UAEXAAVCPersistContext@@@Z @ 004dd9fe ===

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



//=== ?Transfer@CBuyHouseDef@@UAEXAAVCPersistContext@@@Z @ 004ddb56 ===

void _Transfer_CBuyHouseDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x25));
  return;
}



//=== ?Transfer@CWifeDef@@UAEXAAVCPersistContext@@@Z @ 004ddb66 ===

void _Transfer_CWifeDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CDoorDef@@UAEXAAVCPersistContext@@@Z @ 004ddb76 ===

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



//=== ?Transfer@CKickableDef@@UAEXAAVCPersistContext@@@Z @ 004ddc74 ===

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



//=== ?Transfer@CLightDef@@UAEXAAVCPersistContext@@@Z @ 004ddcfa ===

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



//=== ?Transfer@CSpotLightDef@@UAEXAAVCPersistContext@@@Z @ 004dde50 ===

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



//=== ?Transfer@CPerceivedThingDef@@UAEXAAVCPersistContext@@@Z @ 004ddf9e ===

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



//=== ?Transfer@CStealthDef@@UAEXAAVCPersistContext@@@Z @ 004de0bd ===

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



//=== ?Transfer@CTrophyDef@@UAEXAAVCPersistContext@@@Z @ 004de122 ===

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



//=== ?Transfer@CExplodingObjectDef@@UAEXAAVCPersistContext@@@Z @ 004de253 ===

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



//=== ?Transfer@CLookDef@@UAEXAAVCPersistContext@@@Z @ 004de28c ===

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



//=== ?Transfer@CEnemyDef@@UAEXAAVCPersistContext@@@Z @ 004de33e ===

void _Transfer_CEnemyDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@COpinionOfHeroDef@@UAEXAAVCPersistContext@@@Z @ 004de36b ===

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



//=== ?Transfer@CShopItemDef@@UAEXAAVCPersistContext@@@Z @ 004de3af ===

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



//=== ?Transfer@CStockItemDef@@UAEXAAVCPersistContext@@@Z @ 004de4b2 ===

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



//=== ?Transfer@CTimeAppearanceFadeDef@@UAEXAAVCPersistContext@@@Z @ 004de50c ===

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



//=== ?Transfer@CGiftDef@@UAEXAAVCPersistContext@@@Z @ 004de545 ===

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



//=== ?Transfer@CBoastingPodiumDef@@UAEXAAVCPersistContext@@@Z @ 004de772 ===

void _Transfer_CBoastingPodiumDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CShipDef@@UAEXAAVCPersistContext@@@Z @ 004de782 ===

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



//=== ?Transfer@CExpressionSubDef@@UAEXAAVCPersistContext@@@Z @ 004de881 ===

void _Transfer_CExpressionSubDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CSummonableCreatureDef@@UAEXAAVCPersistContext@@@Z @ 004de891 ===

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



//=== ?Transfer@COccupiableDef@@UAEXAAVCPersistContext@@@Z @ 004de8b2 ===

void _Transfer_COccupiableDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CInterestingToVillagersDef@@UAEXAAVCPersistContext@@@Z @ 004de916 ===

void _Transfer_CInterestingToVillagersDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CGuardDef@@UAEXAAVCPersistContext@@@Z @ 004de926 ===

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



//=== ?Transfer@CCrateStackDef@@UAEXAAVCPersistContext@@@Z @ 004de9a1 ===

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



//=== ?Transfer@CTavernTableDef@@UAEXAAVCPersistContext@@@Z @ 004de9d2 ===

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



//=== ?Transfer@CTavernDef@@UAEXAAVCPersistContext@@@Z @ 004de9f3 ===

void _Transfer_CTavernDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CFireballSpellLevelDef@@UAEXAAVCPersistContext@@@Z @ 004dea03 ===

void _Transfer_CFireballSpellLevelDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CThingDrainLifeShotDef@@UAEXAAVCPersistContext@@@Z @ 004dea13 ===

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



//=== ?Transfer@CGoldDef@@UAEXAAVCPersistContext@@@Z @ 004debc4 ===

void _Transfer_CGoldDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CCoinGameObstacleDef@@UAEXAAVCPersistContext@@@Z @ 004dec1c ===

void _Transfer_CCoinGameObstacleDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CTavernGameCoinBaseDef@@UAEXAAVCPersistContext@@@Z @ 004dec2c ===

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



//=== ?Transfer@CTavernGameCoinGolfDef@@UAEXAAVCPersistContext@@@Z @ 004dec86 ===

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



//=== ?Transfer@CDecapitationDef@@UAEXAAVCPersistContext@@@Z @ 004ded4e ===

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



//=== ?Transfer@CWallMountEffectsDef@@UAEXAAVCPersistContext@@@Z @ 004dedac ===

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



//=== ?Transfer@CFishDef@@UAEXAAVCPersistContext@@@Z @ 004dee7e ===

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



//=== ?Transfer@CTeleporterDef@@UAEXAAVCPersistContext@@@Z @ 004def0f ===

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



//=== ?Transfer@CResurrectionItemDef@@UAEXAAVCPersistContext@@@Z @ 004def76 ===

void _Transfer_CResurrectionItemDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CKrakenTentacleDef@@UAEXAAVCPersistContext@@@Z @ 004df050 ===

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



//=== ?Transfer@CAICreatureWillPowerIndicatorDef@@UAEXAAVCPersistContext@@@Z @ 004df0f7 ===

void _Transfer_CAICreatureWillPowerIndicatorDef__UAEXAAVCPersistContext___Z
               (CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CFishingRodDef@@UAEXAAVCPersistContext@@@Z @ 004df107 ===

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



//=== ?Transfer@CNymphDef@@UAEXAAVCPersistContext@@@Z @ 004df1e9 ===

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



//=== ?Transfer@CSummonDef@@UAEXAAVCPersistContext@@@Z @ 004df264 ===

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



//=== ?Transfer@CCameraCollisionDef@@UAEXAAVCPersistContext@@@Z @ 004df31c ===

void _Transfer_CCameraCollisionDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CTCVolumeContainmentTrackerDef@@UAEXAAVCPersistContext@@@Z @ 004df32c ===

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



//=== ?Transfer@CWillResponseDef@@UAEXAAVCPersistContext@@@Z @ 004df562 ===

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



//=== ?Transfer@CBettingDef@@UAEXAAVCPersistContext@@@Z @ 004df5c7 ===

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



//=== ?Transfer@COracleMinigameDef@@UAEXAAVCPersistContext@@@Z @ 004df658 ===

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



//=== ?Transfer@CFireheartMinigameDef@@UAEXAAVCPersistContext@@@Z @ 004df6f4 ===

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



//=== ?Transfer@CLightningOrbDef@@UAEXAAVCPersistContext@@@Z @ 004df738 ===

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



//=== ?Transfer@CHasNameDef@@UAEXAAVCPersistContext@@@Z @ 004df77c ===

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



//=== ?Transfer@CMazeBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e3842 ===

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



//=== ?Transfer@CThunderBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e38e4 ===

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



//=== ?Transfer@CWhisperBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e394f ===

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



//=== ?Transfer@CWaspQueenBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e39a2 ===

void _Transfer_CWaspQueenBattleDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== ?Transfer@CJackOfBladesBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e39e6 ===

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



//=== ?Transfer@CScorpionKingBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e3ae0 ===

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



//=== ?Transfer@CTrollBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e3b82 ===

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



//=== ?Transfer@CBalverineBattleDef@@UAEXAAVCPersistContext@@@Z @ 004e3c24 ===

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



//=== ?Transfer@CAreaOfEffectAttackDef@@UAEXAAVCPersistContext@@@Z @ 004e3f7b ===

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



//=== ?Transfer@CClockDef@@UAEXAAVCPersistContext@@@Z @ 004e448d ===

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



//=== ?Transfer@CHeroSuitDef@@UAEXAAVCPersistContext@@@Z @ 004e55f1 ===

void _Transfer_CHeroSuitDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CFlammableDef@@UAEXAAVCPersistContext@@@Z @ 004e599d ===

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



//=== GFSerialiseMapBinaryIn<long,CDefString,CCompareDefString<long>_> @ 004e5a76 ===

/* [bsim sim=0.8673132385345146 <- ego_r]
   void __fastcall GFSerialiseMapBinaryIn<long,class CDefString,class CCompareDefString<long>
   >(class CMemoryDataInputStream &,class std::map<class CDefString,long,class
   CCompareDefString<long>,class std::allocator<struct std::pair<class CDefString const ,long> > >
   &) */

void __fastcall
GFSerialiseMapBinaryIn<long,CDefString,CCompareDefString<long>_>
          (CMemoryDataInputStream *param_1,
          map<CDefString,long,CCompareDefString<long>,std::allocator<std::pair<CDefString_const_,long>_>_>
          *param_2)

{
  float *unaff_ESI;
  float fVar1;
  undefined1 local_20 [8];
  undefined4 local_18;
  float local_14;
  map<CDefString,long,CCompareDefString<long>,std::allocator<std::pair<CDefString_const_,long>_>_>
  *local_10;
  undefined4 local_c;
  float local_8;
  
  local_10 = param_2;
  CTCCarryable::OnKill((CTCCarryable *)param_2);
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&local_8,4);
    }
    else {
      local_8 = **(float **)(param_1 + 0xc);
      *(float **)(param_1 + 0xc) = *(float **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  fVar1 = local_8;
  if (0 < (int)local_8) {
    do {
      local_c = 0xffffffff;
      local_8 = 0.0;
      CPersistTraits<float>::TransferBinaryIn((CPersistTraits<float> *)&local_c,param_1,unaff_ESI);
      CPersistTraits<float>::TransferBinaryIn((CPersistTraits<float> *)&local_10,param_1,&local_8);
      local_18 = local_c;
      local_14 = local_8;
      FUN_004e5b21(local_20,&local_18);
      fVar1 = (float)((int)fVar1 + -1);
    } while (fVar1 != 0.0);
  }
  return;
}



//=== ?Transfer@CTrapDef@@UAEXAAVCPersistContext@@@Z @ 004e5d08 ===

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



//=== ?Transfer@CIdleSchedulerDef@@UAEXAAVCPersistContext@@@Z @ 004e6248 ===

void _Transfer_CIdleSchedulerDef__UAEXAAVCPersistContext___Z(CEngineLightingManager *param_1)

{
  int in_ECX;
  
  CEngineLightingManager::UpdateShadowScene(param_1,(CEngineCamera *)(in_ECX + 0x3c));
  return;
}



//=== ?Transfer@CRumbleDef@@UAEXAAVCPersistContext@@@Z @ 004e676a ===

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



//=== ?Transfer@CCarryingDef@@UAEXAAVCPersistContext@@@Z @ 004e77c8 ===

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



//=== ?Transfer@CQuestCardDef@@UAEXAAVCPersistContext@@@Z @ 004e795a ===

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



//=== ?Transfer@CBuyableHouseDef@@UAEXAAVCPersistContext@@@Z @ 004e7d97 ===

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



//=== ?Transfer@CSkeletalMorphDef@@UAEXAAVCPersistContext@@@Z @ 004e7e36 ===

void _Transfer_CSkeletalMorphDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CCreatureModeDef@@UAEXAAVCPersistContext@@@Z @ 004e7ea7 ===

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



//=== ?Transfer@CBossDef@@UAEXAAVCPersistContext@@@Z @ 004e7ee0 ===

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



//=== ?Transfer@CTurncoatDef@@UAEXAAVCPersistContext@@@Z @ 004e7f24 ===

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



//=== ?Transfer@CTavernGameSpotTheAdditionDef@@UAEXAAVCPersistContext@@@Z @ 004e7fd8 ===

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



//=== ?Transfer@CKrakenDef@@UAEXAAVCPersistContext@@@Z @ 004e80e3 ===

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



//=== ?Transfer@CVillageDef@@UAEXAAVCPersistContext@@@Z @ 004e82c7 ===

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



//=== ?Transfer@CContainerRewardHeroDef@@UAEXAAVCPersistContext@@@Z @ 004e8987 ===

void _Transfer_CContainerRewardHeroDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::GFTransferVectorField_CContainerRewardHeroDef_ObjectFamilies
            (param_1);
  return;
}



//=== ?Transfer@CWeaponDef@@UAEXAAVCPersistContext@@@Z @ 004e8ce9 ===

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



//=== ?Transfer@CParticleAttacherDef@@UAEXAAVCPersistContext@@@Z @ 004e9caa ===

void _Transfer_CParticleAttacherDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CReplaceableMeshDef@@UAEXAAVCPersistContext@@@Z @ 004ea20e ===

void _Transfer_CReplaceableMeshDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_004ea21e(in_ECX + 0x28);
  return;
}



//=== ?Transfer@CPrizeScoreDef@@UAEXAAVCPersistContext@@@Z @ 004eac7b ===

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



//=== ?Transfer@CExplosionDef@@UAEXAAVCPersistContext@@@Z @ 004eb02f ===

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



//=== ?Transfer@CMultiStaticMeshEntryDef@@UAEXAAVCPersistContext@@@Z @ 004eb8c3 ===

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



//=== ?Transfer@CCardPositionsDef@@UAEXAAVCPersistContext@@@Z @ 004ebf22 ===

void _Transfer_CCardPositionsDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CCardDef@@UAEXAAVCPersistContext@@@Z @ 004ec0b3 ===

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



//=== ?Transfer@CSoundAtmospheresDef@@UAEXAAVCPersistContext@@@Z @ 004ec153 ===

void _Transfer_CSoundAtmospheresDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CObjectAugmentationsDef@@UAEXAAVCPersistContext@@@Z @ 004ec53f ===

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



//=== GFSerialiseMapBinaryIn<CDefString,EObjectAugmentationType,std::less<EObjectAugmentationType>_> @ 004ec66c ===

/* [bsim sim=0.8673132385345146 <- ego_r]
   void __fastcall GFSerialiseMapBinaryIn<class CDefString,enum EObjectAugmentationType,struct
   std::less<enum EObjectAugmentationType> >(class CMemoryDataInputStream &,class std::map<enum
   EObjectAugmentationType,class CDefString,struct std::less<enum EObjectAugmentationType>,class
   std::allocator<struct std::pair<enum EObjectAugmentationType const ,class CDefString> > > &) */

void __fastcall
GFSerialiseMapBinaryIn<CDefString,EObjectAugmentationType,std::less<EObjectAugmentationType>_>
          (CMemoryDataInputStream *param_1,
          map<EObjectAugmentationType,CDefString,std::less<EObjectAugmentationType>,std::allocator<std::pair<EObjectAugmentationType_const_,CDefString>_>_>
          *param_2)

{
  float *unaff_ESI;
  int iVar1;
  undefined1 local_20 [8];
  undefined4 local_18;
  int local_14;
  map<EObjectAugmentationType,CDefString,std::less<EObjectAugmentationType>,std::allocator<std::pair<EObjectAugmentationType_const_,CDefString>_>_>
  *local_10;
  undefined4 local_c;
  int local_8;
  
  local_10 = param_2;
  CTCCarryable::OnKill((CTCCarryable *)param_2);
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&local_8,4);
    }
    else {
      local_8 = **(int **)(param_1 + 0xc);
      *(int **)(param_1 + 0xc) = *(int **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  iVar1 = local_8;
  if (0 < local_8) {
    do {
      local_c = 0;
      local_8 = -1;
      CPersistTraits<CDefIndex>::TransferBinaryIn((CPersistTraits<CDefIndex> *)&local_10,param_1);
      CPersistTraits<float>::TransferBinaryIn((CPersistTraits<float> *)&local_8,param_1,unaff_ESI);
      local_18 = local_c;
      local_14 = local_8;
      FUN_004ec717(local_20,&local_18);
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return;
}



//=== ?Transfer@CShopDef@@UAEXAAVCPersistContext@@@Z @ 004ecb4d ===

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



//=== ?Transfer@CAnimatingObjectDef@@UAEXAAVCPersistContext@@@Z @ 004ed459 ===

void _Transfer_CAnimatingObjectDef__UAEXAAVCPersistContext___Z(void)

{
  int in_ECX;
  
  FUN_0047367a((CTCInventoryQuests *)(in_ECX + 0x28));
  CTCInventoryQuests::ProcessButtonBReleased((CTCInventoryQuests *)(in_ECX + 0x28));
  return;
}



//=== ?Transfer@CTavernGameDef@@UAEXAAVCPersistContext@@@Z @ 004ed472 ===

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



//=== ?Transfer@CMultiStaticMeshDef@@UAEXAAVCPersistContext@@@Z @ 004ede1b ===

void _Transfer_CMultiStaticMeshDef__UAEXAAVCPersistContext___Z(CPersistContext *param_1)

{
  int in_ECX;
  vector<CAbilityButtonGuiDef,std::allocator<CAbilityButtonGuiDef>_> *unaff_retaddr;
  
  CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef>
            (param_1,(char *)(in_ECX + 0x28),unaff_retaddr);
  return;
}



//=== ?Transfer@CTavernGameCardBaseDef@@UAEXAAVCPersistContext@@@Z @ 004f9a90 ===

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



//=== Transfer<CDefIndex> @ 00580371 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: void __thiscall CPersistContext::Transfer<class CDefIndex>(char const *,class CDefIndex
   &,class CDefIndex const &) */

void __thiscall
CPersistContext::Transfer<CDefIndex>
          (CPersistContext *this,char *param_1,CDefIndex *param_2,CDefIndex *param_3)

{
  int iVar1;
  CPersistContext *local_c;
  CPersistContext *local_8;
  
  local_c = this;
  local_8 = this;
  CheckCRC(this,param_1);
  iVar1 = *(int *)(this + 0x18);
  if (iVar1 == 0) {
    if (this[0x14] != (CPersistContext)0x0) {
      *(undefined4 *)param_2 = *(undefined4 *)param_3;
    }
    local_8 = (CPersistContext *)param_2;
    local_c = (CPersistContext *)&PTR_TransferOut_0124f2f4;
    TransferObjectLoadText
              (this,param_1,(CPersistContext_TransferableComponentGenericType *)&local_c);
  }
  else if (iVar1 == 1) {
    local_8 = (CPersistContext *)param_2;
    local_c = (CPersistContext *)&PTR_TransferOut_0124f2f4;
    TransferObjectSaveText
              (this,param_1,(CPersistContext_TransferableComponentGenericType *)&local_c);
  }
  else if (iVar1 == 2) {
    CPersistTraits<CDefIndex>::TransferBinaryIn
              ((CPersistTraits<CDefIndex> *)&param_2,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (iVar1 == 3) {
    CDataOutputStream::WriteCBYTE(*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)param_2)
    ;
  }
  else {
    *(undefined4 *)param_2 = *(undefined4 *)param_3;
  }
  return;
}



//=== CPersistContextReadSection @ 005819f2 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall CPersistContextReadSection::CPersistContextReadSection(void) */

undefined4 __thiscall
CPersistContextReadSection::CPersistContextReadSection(CPersistContextReadSection *this)

{
  int iVar1;
  undefined4 in_stack_00000004;
  
  NDisplayView::CViewBase::GetDisplayEngine((CViewBase *)this);
  iVar1 = Catch_0040d959();
  CShaderPreParser::GetConstantCount(*(CShaderPreParser **)(iVar1 + 0xc));
  return in_stack_00000004;
}



//=== Transfer<CDefIndex> @ 005cc1e8 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: void __thiscall CPersistContext::Transfer<class CDefIndex>(char const *,class CDefIndex
   &,class CDefIndex const &) */

void __thiscall
CPersistContext::Transfer<CDefIndex>
          (CPersistContext *this,char *param_1,CDefIndex *param_2,CDefIndex *param_3)

{
  int iVar1;
  CPersistContext *local_c;
  CPersistContext *local_8;
  
  local_c = this;
  local_8 = this;
  CheckCRC(this,param_1);
  iVar1 = *(int *)(this + 0x18);
  if (iVar1 == 0) {
    if (this[0x14] != (CPersistContext)0x0) {
      *(undefined4 *)param_2 = *(undefined4 *)param_3;
    }
    local_8 = (CPersistContext *)param_2;
    local_c = (CPersistContext *)&PTR_TransferIn_01253f18;
    TransferObjectLoadText
              (this,param_1,(CPersistContext_TransferableComponentGenericType *)&local_c);
  }
  else if (iVar1 == 1) {
    local_8 = (CPersistContext *)param_2;
    local_c = (CPersistContext *)&PTR_TransferIn_01253f18;
    TransferObjectSaveText
              (this,param_1,(CPersistContext_TransferableComponentGenericType *)&local_c);
  }
  else if (iVar1 == 2) {
    CPersistTraits<CDefIndex>::TransferBinaryIn
              ((CPersistTraits<CDefIndex> *)&param_2,*(CMemoryDataInputStream **)(this + 0x24));
  }
  else if (iVar1 == 3) {
    CDataOutputStream::WriteCBYTE(*(CDataOutputStream **)(this + 0x28),(char)*(undefined4 *)param_2)
    ;
  }
  else {
    *(undefined4 *)param_2 = *(undefined4 *)param_3;
  }
  return;
}



//=== ?Initialise@CAnimComponentFollowingOnAnim@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005db4c0 ===

undefined4
_Initialise_CAnimComponentFollowingOnAnim__UAE_NAAVCPersistContext__ABVCCharString___Z
          (undefined4 param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  int *in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_4 [4];
  
  (**(code **)(*in_ECX + 0x20))();
  if ((*(int *)param_2 != 0) && (2 < *(int *)(*(int *)param_2 + 4))) {
    pCVar1 = (CCharString *)CCharString::GetSubString(param_2,(ulong)aaStack_4,1);
    CRegion::SetMiniMapRegionExitTextOffsetX((CRegion *)&param_2,pCVar1,(float)unaff_ESI);
    in_ECX[2] = (int)param_2;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_4,extraout_EDX,unaff_ESI);
    return 1;
  }
  return 0;
}



//=== ?Initialise@CAnimComponentFollowingOnAnimFilter@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005db530 ===

undefined4
_Initialise_CAnimComponentFollowingOnAnimFilter__UAE_NAAVCPersistContext__ABVCCharString___Z
          (undefined4 param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  int *in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_4 [4];
  
  (**(code **)(*in_ECX + 0x20))();
  if ((*(int *)param_2 != 0) && (2 < *(int *)(*(int *)param_2 + 4))) {
    pCVar1 = (CCharString *)CCharString::GetSubString(param_2,(ulong)aaStack_4,1);
    CRegion::SetMiniMapRegionExitTextOffsetX((CRegion *)&param_2,pCVar1,(float)unaff_ESI);
    in_ECX[2] = (int)param_2;
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_4,extraout_EDX,unaff_ESI);
    return 1;
  }
  return 0;
}



//=== ?Initialise@CAnimComponentStrikeResponseAnim@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005db5a0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual bool __thiscall CAnimComponentStrikeResponseAnim::Initialise(class
   CPersistContext &,class CCharString const &) */

bool __thiscall
CAnimComponentStrikeResponseAnim::
_Initialise_CAnimComponentStrikeResponseAnim__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentStrikeResponseAnim *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  
  (**(code **)(*(int *)this + 0x20))();
  if ((*(int *)param_2 != 0) && (2 < *(int *)(*(int *)param_2 + 4))) {
    pCVar1 = (CCharString *)CCharString::GetSubString(param_2,(ulong)&param_2,1);
    CCharString::operator=((CCharString *)(this + 8),pCVar1);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_2,
               extraout_EDX,unaff_ESI);
    return true;
  }
  return false;
}



//=== ?Initialise@CAnimComponentFlags@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc3c0 ===

/* [bsim sim=0.8567387451088749 <- ego_r]
   public: virtual bool __thiscall CAnimComponentFlags::Initialise(class CPersistContext &,class
   CCharString const &) */

bool __thiscall
CAnimComponentFlags::_Initialise_CAnimComponentFlags__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentFlags *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  long lVar2;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar2 = 0;
  pCVar1 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar1,lVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_ESI);
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_2);
  *(uint *)(this + 8) = *(uint *)(this + 8) | (uint)param_2;
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_01,unaff_ESI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentTransitionInTime@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc450 ===

/* [bsim sim=0.8491687747743696 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatRecoil::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatRecoil::
_Initialise_CAnimComponentTransitionInTime__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatRecoil *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  long lVar2;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar2 = 0;
  pCVar1 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar1,lVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_ESI);
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_2);
  *(CCharString **)(this + 8) = param_2;
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_01,unaff_ESI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentDelayTime@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc4d0 ===

/* [bsim sim=0.8491687747743696 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatRecoil::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatRecoil::
_Initialise_CAnimComponentDelayTime__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatRecoil *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  long lVar2;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar2 = 0;
  pCVar1 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar1,lVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_ESI);
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_2);
  *(CCharString **)(this + 8) = param_2;
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_01,unaff_ESI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentCombatHandedness@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc550 ===

/* [bsim sim=0.8733902835114342 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatHandedness::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatHandedness::
_Initialise_CAnimComponentCombatHandedness__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatHandedness *this,CPersistContext *param_1,CCharString *param_2)

{
  CPersistContext *pCVar1;
  bool bVar2;
  CCharString *pCVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  long lVar4;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar4 = 0;
  pCVar3 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar3,lVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_EDI);
  pCVar1 = param_1;
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_1);
  CCharString::CCharString((CCharString *)&param_2,",",-1);
  bVar2 = CStringParser::SkipPastNextItemIfIs(aCStack_28,(CCharString *)&param_2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_2,
             extraout_EDX_00,unaff_EDI);
  if (!bVar2) {
    CGuiControlTreePane::SortTreeRecursively
              (aCStack_14,
               (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_18,extraout_EDX_01,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_1c,extraout_EDX_02,unaff_EDI);
    NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
    return false;
  }
  CPersistTraitsSLONG::TransferIn(pCVar1,aCStack_28,(long *)&param_2);
  *(CPersistContext **)(this + 8) = param_1;
  *(CCharString **)(this + 0xc) = param_2;
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_03,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_04,unaff_EDI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentCombatComboChain@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc650 ===

/* [bsim sim=0.7253656955781848 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatComboChain::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatComboChain::
_Initialise_CAnimComponentCombatComboChain__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatComboChain *this,CPersistContext *param_1,CCharString *param_2)

{
  CPersistContext *pCVar1;
  bool bVar2;
  CCharString *pCVar3;
  CPersistContext *pCVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  long lVar5;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar5 = 0;
  pCVar3 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar3,lVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_EDI);
  pCVar1 = param_1;
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_1);
  CCharString::CCharString((CCharString *)&param_2,",",-1);
  bVar2 = CStringParser::SkipPastNextItemIfIs(aCStack_28,(CCharString *)&param_2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_2,
             extraout_EDX_00,unaff_EDI);
  pCVar4 = param_1;
  if ((bVar2) &&
     (CPersistTraitsSLONG::TransferIn(pCVar1,aCStack_28,(long *)&param_2), pCVar4 = param_1,
     param_2 != (CCharString *)0xffffffff)) {
    *(CPersistContext **)(this + 8) = param_1;
    pCVar4 = (CPersistContext *)param_2;
  }
  *(CPersistContext **)(this + 0xc) = pCVar4;
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_01,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_02,unaff_EDI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentCombatRecoil@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc730 ===

/* [bsim sim=0.8491687747743696 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatRecoil::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatRecoil::
_Initialise_CAnimComponentCombatRecoil__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatRecoil *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  long lVar2;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar2 = 0;
  pCVar1 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar1,lVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_ESI);
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_2);
  *(CCharString **)(this + 8) = param_2;
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_01,unaff_ESI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentCombatMisc@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc7b0 ===

/* [bsim sim=0.8635608411194192 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatMisc::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatMisc::
_Initialise_CAnimComponentCombatMisc__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatMisc *this,CPersistContext *param_1,CCharString *param_2)

{
  bool bVar1;
  CCharString *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  long lVar3;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar3 = 0;
  pCVar2 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar2,lVar3);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_EDI);
  CPersistTraits<bool>::TransferIn
            ((CPersistTraits<bool> *)&param_2,param_1,aCStack_28,(bool *)(this + 8));
  CCharString::CCharString((CCharString *)&param_2,",",-1);
  bVar1 = CStringParser::SkipPastNextItemIfIs(aCStack_28,(CCharString *)&param_2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_2,
             extraout_EDX_00,unaff_EDI);
  if (!bVar1) {
    CGuiControlTreePane::SortTreeRecursively
              (aCStack_14,
               (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_18,extraout_EDX_01,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_1c,extraout_EDX_02,unaff_EDI);
    NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
    return false;
  }
  CPersistTraits<bool>::TransferIn
            ((CPersistTraits<bool> *)&param_2,param_1,aCStack_28,(bool *)(this + 9));
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_03,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_04,unaff_EDI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentCombatFlourishTargetOffset@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc8b0 ===

/* [bsim sim=0.8694463373024278 <- ego_r]
   public: virtual bool __thiscall CAnimComponentCombatFlourishTargetOffset::Initialise(class
   CPersistContext &,class CCharString const &) */

bool __thiscall
CAnimComponentCombatFlourishTargetOffset::
_Initialise_CAnimComponentCombatFlourishTargetOffset__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentCombatFlourishTargetOffset *this,CPersistContext *param_1,
          CCharString *param_2)

{
  bool bVar1;
  CCharString *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  long lVar3;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar3 = 0;
  this[8] = (CAnimComponentCombatFlourishTargetOffset)0x1;
  pCVar2 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar2,lVar3);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_EDI);
  CPersistTraits<float>::TransferIn
            ((CPersistTraits<float> *)&param_2,param_1,aCStack_28,(float *)(this + 9));
  CCharString::CCharString((CCharString *)&param_2,",",-1);
  bVar1 = CStringParser::SkipPastNextItemIfIs(aCStack_28,(CCharString *)&param_2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_2,
             extraout_EDX_00,unaff_EDI);
  if (!bVar1) {
    CGuiControlTreePane::SortTreeRecursively
              (aCStack_14,
               (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_18,extraout_EDX_01,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_1c,extraout_EDX_02,unaff_EDI);
    NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
    return false;
  }
  CPersistTraits<float>::TransferIn
            ((CPersistTraits<float> *)&param_2,param_1,aCStack_28,(float *)(this + 0xd));
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_EDI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_03,unaff_EDI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_04,unaff_EDI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== ?Initialise@CAnimComponentSetCreatureMode@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dc9b0 ===

/* [bsim sim=0.833614249072113 <- ego_r]
   public: virtual bool __thiscall CAnimComponentSetCreatureMode::Initialise(class CPersistContext
   &,class CCharString const &) */

bool __thiscall
CAnimComponentSetCreatureMode::
_Initialise_CAnimComponentSetCreatureMode__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentSetCreatureMode *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  long lVar2;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar2 = 0;
  pCVar1 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar1,lVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_ESI);
  CPersistTraitsSLONG::TransferIn(param_1,aCStack_28,(long *)&param_2);
  if (param_2 != (CCharString *)0x0) {
    *(CCharString **)(this + 8) = param_2;
    CGuiControlTreePane::SortTreeRecursively
              (aCStack_14,
               (list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_18,extraout_EDX_00,unaff_ESI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (aaStack_1c,extraout_EDX_01,unaff_ESI);
    NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
    return true;
  }
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_02,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_03,unaff_ESI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return false;
}



//=== ?Initialise@CAnimComponentStrikeSpeedMultiplier@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z @ 005dca60 ===

/* [bsim sim=0.8374309097203094 <- ego_r]
   public: virtual bool __thiscall CAnimComponentStrikeSpeedMultiplier::Initialise(class
   CPersistContext &,class CCharString const &) */

bool __thiscall
CAnimComponentStrikeSpeedMultiplier::
_Initialise_CAnimComponentStrikeSpeedMultiplier__UAE_NAAVCPersistContext__ABVCCharString___Z
          (CAnimComponentStrikeSpeedMultiplier *this,CPersistContext *param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  long lVar2;
  CCharString aCStack_2c [4];
  CStringParser aCStack_28 [12];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_18 [4];
  CGuiControlTreePane aCStack_14 [20];
  
  (**(code **)(*(int *)this + 0x20))();
  lVar2 = 0;
  pCVar1 = (CCharString *)CCharString::CCharString(aCStack_2c,"",-1);
  CStringParser::CStringParser(aCStack_28,param_2,(_func_void_CCharString_ptr *)0x0,pCVar1,lVar2);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_2c,
             extraout_EDX,unaff_ESI);
  CPersistTraits<float>::TransferIn
            ((CPersistTraits<float> *)&param_2,param_1,aCStack_28,(float *)(this + 8));
  CGuiControlTreePane::SortTreeRecursively
            (aCStack_14,(list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *)unaff_ESI)
  ;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_18,extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (aaStack_1c,extraout_EDX_01,unaff_ESI);
  NHeroInformationScreens::CBase::CBase((CBase *)aCStack_28);
  return true;
}



//=== _Copy_backward<CDefIndex*,CDefIndex*> @ 005f7c13 ===

/* [bsim sim=0.5989469809237268 <- ego_r]
   class CDefIndex * __fastcall std::_Copy_backward<class CDefIndex *,class CDefIndex *>(class
   CDefIndex *,class CDefIndex *,class CDefIndex *,struct std::_Nonscalar_ptr_iterator_tag) */

CDefIndex * __fastcall
std::_Copy_backward<CDefIndex*,CDefIndex*>(int param_1,undefined4 *param_2,CDefIndex *param_3)

{
  int iVar1;
  
  iVar1 = (int)param_2 - param_1 >> 2;
  if (0 < iVar1) {
    do {
      param_3 = param_3 + -4;
      param_2 = param_2 + -1;
      iVar1 = iVar1 + -1;
      *(undefined4 *)param_3 = *param_2;
    } while (iVar1 != 0);
  }
  return param_3;
}



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 005f9cde ===

/* [bsim sim=0.6849684911415447 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (undefined4 *param_1,undefined4 *param_2,undefined4 param_3)

{
  pair<unsigned_long,CDefString> *in_EAX;
  undefined4 *puVar1;
  
  if (param_1 != param_2) {
    while (puVar1 = param_1 + 2, puVar1 != param_2) {
      in_EAX = (pair<unsigned_long,CDefString> *)FUN_005f9a98(*puVar1,param_1[3],param_3);
      param_1 = puVar1;
    }
  }
  return in_EAX;
}



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 005f9d16 ===

/* [bsim sim=0.6849684911415447 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (undefined4 *param_1,undefined4 *param_2,undefined4 param_3)

{
  pair<unsigned_long,CDefString> *in_EAX;
  undefined4 *puVar1;
  
  if (param_1 != param_2) {
    while (puVar1 = param_1 + 2, puVar1 != param_2) {
      in_EAX = (pair<unsigned_long,CDefString> *)FUN_005f9adb(*puVar1,param_1[3],param_3);
      param_1 = puVar1;
    }
  }
  return in_EAX;
}



//=== ?Transfer@CThingCreatureDef@@UAEXAAVCPersistContext@@@Z @ 00671600 ===

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



//=== ?Transfer@CCreatureDef@@UAEXAAVCPersistContext@@@Z @ 00675e20 ===

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



//=== ?Transfer@CThingBaseDef@@UAEXAAVCPersistContext@@@Z @ 00676be0 ===

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



//=== ?Transfer@CActionUseDef@@UAEXAAVCPersistContext@@@Z @ 006773f0 ===

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



//=== ?Transfer@COpinionReactionManagerDef@@UAEXAAVCPersistContext@@@Z @ 00679740 ===

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



//=== ?Transfer@CBlurredAttitudesDef@@UAEXAAVCPersistContext@@@Z @ 0067cdb0 ===

void _Transfer_CBlurredAttitudesDef__UAEXAAVCPersistContext___Z
               (CCreatureAction_FireMissileWeapon *param_1)

{
  CCreatureAction_FireMissileWeapon::FrameUpdate(param_1);
  return;
}



//=== ?Transfer@CDegradableDef@@UAEXAAVCPersistContext@@@Z @ 0067e290 ===

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



//=== GFSerialiseVectorBinaryIn<CDefString> @ 006c9e40 ===

/* [bsim sim=0.8559108616348808 <- ego_r]
   void __fastcall GFSerialiseVectorBinaryIn<class CDefString>(class CMemoryDataInputStream &,class
   std::vector<class CDefString,class std::allocator<class CDefString> > &) */

void __fastcall
GFSerialiseVectorBinaryIn<CDefString>
          (CMemoryDataInputStream *param_1,vector<CDefString,std::allocator<CDefString>_> *param_2)

{
  float *pfVar1;
  uint uVar2;
  int iVar3;
  uint local_8;
  vector<CDefString,std::allocator<CDefString>_> *local_4;
  
  uVar2 = local_8;
  local_4 = param_2;
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)&local_8,4);
      uVar2 = local_8;
    }
    else {
      uVar2 = **(uint **)(param_1 + 0xc);
      *(uint **)(param_1 + 0xc) = *(uint **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  std::vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_>::resize
            ((vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_> *)
             param_2,uVar2);
  iVar3 = 0;
  if (0 < (int)uVar2) {
    do {
      pfVar1 = (float *)(*(int *)param_2 + iVar3 * 8);
      CPersistTraits<float>::TransferBinaryIn((CPersistTraits<float> *)&local_4,param_1,pfVar1);
      CPersistTraits<COpinionDeed*>::TransferBinaryIn
                ((CPersistTraits<COpinionDeed*> *)&local_4,param_1,(COpinionDeed **)(pfVar1 + 1));
      iVar3 = iVar3 + 1;
      param_2 = local_4;
    } while (iVar3 < (int)uVar2);
  }
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



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 006f44e0 ===

/* [bsim sim=0.5818202266828859 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (int param_1,int param_2)

{
  int iVar1;
  pair<unsigned_long,CDefString> *in_EAX;
  pair<unsigned_long,CDefString> *extraout_EAX;
  int iVar2;
  
  for (; param_1 != param_2; param_1 = param_1 + 0x1c) {
    iVar1 = *(int *)(param_1 + 0x14);
    for (iVar2 = *(int *)(param_1 + 0x10); iVar2 != iVar1; iVar2 = iVar2 + 0x1c) {
      CBaseIntelligentPointer::~CBaseIntelligentPointer((CBaseIntelligentPointer *)(iVar2 + 0x14));
    }
    in_EAX = (pair<unsigned_long,CDefString> *)0x0;
    if (*(void **)(param_1 + 0x10) != (void *)0x0) {
      free(*(void **)(param_1 + 0x10));
      in_EAX = extraout_EAX;
    }
  }
  return in_EAX;
}



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 00722130 ===

/* [bsim sim=0.5358644161073244 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (int param_1,int param_2)

{
  int iVar1;
  pair<unsigned_long,CDefString> *in_EAX;
  pair<unsigned_long,CDefString> *extraout_EAX;
  int iVar2;
  
  for (; param_1 != param_2; param_1 = param_1 + 0x10) {
    iVar1 = *(int *)(param_1 + 8);
    for (iVar2 = *(int *)(param_1 + 4); iVar2 != iVar1; iVar2 = iVar2 + 0x14) {
      if (*(void **)(iVar2 + 8) != (void *)0x0) {
        free(*(void **)(iVar2 + 8));
      }
    }
    in_EAX = (pair<unsigned_long,CDefString> *)0x0;
    if (*(void **)(param_1 + 4) != (void *)0x0) {
      free(*(void **)(param_1 + 4));
      in_EAX = extraout_EAX;
    }
  }
  return in_EAX;
}



//=== ?Transfer@CCreatureGeneratorDef@@UAEXAAVCPersistContext@@@Z @ 0074c5f0 ===

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



//=== ?Transfer@CHighlightItemDef@@UAEXAAVCPersistContext@@@Z @ 0077ce00 ===

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



//=== ?Transfer@CHeroCentreDef@@UAEXAAVCPersistContext@@@Z @ 0077d3a0 ===

void _Transfer_CHeroCentreDef__UAEXAAVCPersistContext___Z(void)

{
  return;
}



//=== ?Transfer@CAIScratchpadDef@@UAEXAAVCPersistContext@@@Z @ 007ad300 ===

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



//=== ?Transfer@CFishingDef@@UAEXAAVCPersistContext@@@Z @ 007b1f10 ===

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



//=== GFSerialiseVectorBinaryIn<CDefString> @ 007b5370 ===

/* [bsim sim=0.9173630586562052 <- ego_r]
   void __fastcall GFSerialiseVectorBinaryIn<class CDefString>(class CMemoryDataInputStream &,class
   std::vector<class CDefString,class std::allocator<class CDefString> > &) */

void __fastcall
GFSerialiseVectorBinaryIn<CDefString>
          (CMemoryDataInputStream *param_1,vector<CDefString,std::allocator<CDefString>_> *param_2)

{
  uint uVar1;
  int iVar2;
  uint local_8 [2];
  
  uVar1 = local_8[0];
  if (*(int *)(param_1 + 4) + 4U < 0x80000000) {
    if (*(int *)(param_1 + 0x14) < 4) {
      CDataInputStream::ReadWithSrcChunkOverflow((CDataInputStream *)param_1,(uchar *)local_8,4);
      uVar1 = local_8[0];
    }
    else {
      uVar1 = **(uint **)(param_1 + 0xc);
      *(uint **)(param_1 + 0xc) = *(uint **)(param_1 + 0xc) + 1;
      *(int *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + -4;
      *(uint *)(param_1 + 4) = *(int *)(param_1 + 4) + 4U;
    }
  }
  std::vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_>::resize
            ((vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_> *)
             param_2,uVar1);
  iVar2 = 0;
  if (0 < (int)uVar1) {
    do {
      CPersistTraits<CCrimeWaitingForBribe>::TransferBinaryIn
                ((CPersistTraits<CCrimeWaitingForBribe> *)local_8,param_1,
                 (CCrimeWaitingForBribe *)(*(int *)param_2 + iVar2 * 8));
      iVar2 = iVar2 + 1;
    } while (iVar2 < (int)uVar1);
  }
  return;
}



//=== ?Transfer@CActivateQuestDef@@UAEXAAVCPersistContext@@@Z @ 007b5740 ===

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



//=== ?Transfer@COverheadDisplayDef@@UAEXAAVCPersistContext@@@Z @ 007b7d20 ===

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



//=== ?Transfer@CShoveHaPennyRegionDef@@UAEXAAVCPersistContext@@@Z @ 007d0070 ===

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



//=== ?Transfer@CTavernGameShoveHaPennyDef@@UAEXAAVCPersistContext@@@Z @ 007d0a40 ===

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



//=== ?Transfer@CCarriedReadableDef@@UAEXAAVCPersistContext@@@Z @ 007e1e30 ===

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



//=== ?Transfer@CReadableDef@@UAEXAAVCPersistContext@@@Z @ 007e81b0 ===

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



//=== _Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_> @ 0081c5f0 ===

/* [bsim sim=0.6460374837736084 <- ego_r]
   void __fastcall std::_Insertion_sort1<struct std::pair<unsigned long,class CDefString> *,class
   CKeyPairCompareLess<unsigned long,class CDefString>,struct std::pair<unsigned long,class
   CDefString> >(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned
   long,class CDefString> *,class CKeyPairCompareLess<unsigned long,class CDefString>,struct
   std::pair<unsigned long,class CDefString> *) */

void __fastcall
std::
_Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_>
          (uint *param_1,uint *param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  
  puVar4 = param_1;
  if (param_1 != param_2) {
    while (puVar4 = puVar4 + 3, puVar4 != param_2) {
      uVar1 = *puVar4;
      uVar2 = puVar4[1];
      uVar3 = puVar4[2];
      if (uVar1 < *param_1) {
        _Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>
                  (param_1,puVar4,puVar4 + 3,&param_3,0);
        *param_1 = uVar1;
        param_1[1] = uVar2;
        param_1[2] = uVar3;
      }
      else {
        FUN_00819a60(uVar1,uVar2,uVar3,param_3);
      }
    }
  }
  return;
}



//=== _Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_> @ 0086c7b0 ===

/* [bsim sim=0.6460374837736084 <- ego_r]
   void __fastcall std::_Insertion_sort1<struct std::pair<unsigned long,class CDefString> *,class
   CKeyPairCompareLess<unsigned long,class CDefString>,struct std::pair<unsigned long,class
   CDefString> >(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned
   long,class CDefString> *,class CKeyPairCompareLess<unsigned long,class CDefString>,struct
   std::pair<unsigned long,class CDefString> *) */

void __fastcall
std::
_Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_>
          (uint *param_1,uint *param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  
  puVar4 = param_1;
  if (param_1 != param_2) {
    while (puVar4 = puVar4 + 3, puVar4 != param_2) {
      uVar1 = *puVar4;
      uVar2 = puVar4[1];
      uVar3 = puVar4[2];
      if (uVar1 < *param_1) {
        _Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>
                  (param_1,puVar4,puVar4 + 3,&param_3,0);
        *param_1 = uVar1;
        param_1[1] = uVar2;
        param_1[2] = uVar3;
      }
      else {
        FUN_0086a640(uVar1,uVar2,uVar3,param_3);
      }
    }
  }
  return;
}



//=== CPersistContextReadSection @ 0099a540 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall CPersistContextReadSection::CPersistContextReadSection(void) */

CPersistContextReadSection * __thiscall
CPersistContextReadSection::CPersistContextReadSection(CPersistContextReadSection *this)

{
  ::CDateAndTime::CDateAndTime((CDateAndTime *)this);
  ::CDateAndTime::CDateAndTime((CDateAndTime *)(this + 0x1c));
  ::CDateAndTime::CDateAndTime((CDateAndTime *)(this + 0x38));
  return this;
}



//=== _Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_> @ 009b5670 ===

/* [bsim sim=0.6460374837736084 <- ego_r]
   void __fastcall std::_Insertion_sort1<struct std::pair<unsigned long,class CDefString> *,class
   CKeyPairCompareLess<unsigned long,class CDefString>,struct std::pair<unsigned long,class
   CDefString> >(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned
   long,class CDefString> *,class CKeyPairCompareLess<unsigned long,class CDefString>,struct
   std::pair<unsigned long,class CDefString> *) */

void __fastcall
std::
_Insertion_sort1<std::pair<unsigned_long,CDefString>*,CKeyPairCompareLess<unsigned_long,CDefString>,std::pair<unsigned_long,class_CDefString>_>
          (uint *param_1,uint *param_2,undefined4 param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  
  puVar4 = param_1;
  if (param_1 != param_2) {
    while (puVar4 = puVar4 + 3, puVar4 != param_2) {
      uVar1 = *puVar4;
      uVar2 = puVar4[1];
      uVar3 = puVar4[2];
      if (uVar1 < *param_1) {
        _Move_backward<std::pair<unsigned___int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*,std::pair<unsigned___int64,class_NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>
                  (param_1,puVar4,puVar4 + 3,&param_3,0);
        *param_1 = uVar1;
        param_1[1] = uVar2;
        param_1[2] = uVar3;
      }
      else {
        FUN_009b1a30(uVar1,uVar2,uVar3,param_3);
      }
    }
  }
  return;
}



//=== CPersistContext @ 009ba9d0 ===

/* [bsim sim=0.7976171574137905 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CStringParser &,void
   (__fastcall*)(class CCharString const &),bool,class CDefinitionManager const *,class CSymbolMap
   const *,class CCharString *) */

void __thiscall
CPersistContext::CPersistContext
          (CPersistContext *this,CStringParser *param_1,_func_void_CCharString_ptr *param_2,
          bool param_3,CDefinitionManager *param_4,CSymbolMap *param_5,CCharString *param_6)

{
  *(CDefinitionManager **)this = param_4;
  *(CSymbolMap **)(this + 4) = param_5;
  *(CStringParser **)(this + 8) = param_1;
  *(_func_void_CCharString_ptr **)(this + 0xc) = param_2;
  *(CCharString **)(this + 0x10) = param_6;
  *(undefined4 *)(this + 0x18) = 0;
  this[0x14] = (CPersistContext)param_3;
  this[0x15] = (CPersistContext)0x0;
  this[0x17] = (CPersistContext)0x0;
  this[0x16] = (CPersistContext)0x0;
  return;
}



//=== CPersistContext @ 009baa30 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CDataOutputStream &,bool) */

void __thiscall
CPersistContext::CPersistContext(CPersistContext *this,CDataOutputStream *param_1,bool param_2)

{
  *(CDataOutputStream **)(this + 0x24) = param_1;
  *(undefined4 *)(this + 0x18) = 2;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)this = 0;
  this[0x15] = (CPersistContext)0x0;
  return;
}



//=== CPersistContext @ 009baa50 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CDataOutputStream &,bool) */

void __thiscall
CPersistContext::CPersistContext(CPersistContext *this,CDataOutputStream *param_1,bool param_2)

{
  *(CDataOutputStream **)(this + 0x28) = param_1;
  *(undefined4 *)(this + 0x18) = 3;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)this = 0;
  this[0x15] = (CPersistContext)0x0;
  return;
}



//=== CPersistContext @ 009badd0 ===

/* [bsim sim=0.7976171574137905 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CStringParser &,void
   (__fastcall*)(class CCharString const &),bool,class CDefinitionManager const *,class CSymbolMap
   const *,class CCharString *) */

void __thiscall
CPersistContext::CPersistContext
          (CPersistContext *this,CStringParser *param_1,_func_void_CCharString_ptr *param_2,
          bool param_3,CDefinitionManager *param_4,CSymbolMap *param_5,CCharString *param_6)

{
  *(CDefinitionManager **)this = param_4;
  *(CSymbolMap **)(this + 4) = param_5;
  *(CStringParser **)(this + 8) = param_1;
  *(_func_void_CCharString_ptr **)(this + 0xc) = param_2;
  *(CCharString **)(this + 0x10) = param_6;
  *(undefined4 *)(this + 0x18) = 0;
  this[0x14] = (CPersistContext)param_3;
  this[0x15] = (CPersistContext)0x0;
  this[0x17] = (CPersistContext)0x0;
  this[0x16] = (CPersistContext)0x0;
  return;
}



//=== CPersistContext @ 009bae10 ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CCharString &) */

void __thiscall CPersistContext::CPersistContext(CPersistContext *this,CCharString *param_1)

{
  *(undefined4 *)(this + 0x18) = 1;
  *(CCharString **)(this + 0x20) = param_1;
  *(undefined4 *)this = 0;
  this[0x15] = (CPersistContext)0x0;
  return;
}



//=== CPersistContext @ 009bae30 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CDataOutputStream &,bool) */

void __thiscall
CPersistContext::CPersistContext(CPersistContext *this,CDataOutputStream *param_1,bool param_2)

{
  *(CDataOutputStream **)(this + 0x24) = param_1;
  *(undefined4 *)(this + 0x18) = 2;
  *(undefined4 *)(this + 0x28) = 0;
  *(undefined4 *)this = 0;
  this[0x15] = (CPersistContext)0x0;
  return;
}



//=== CPersistContext @ 009bae50 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: __thiscall CPersistContext::CPersistContext(class CDataOutputStream &,bool) */

void __thiscall
CPersistContext::CPersistContext(CPersistContext *this,CDataOutputStream *param_1,bool param_2)

{
  *(CDataOutputStream **)(this + 0x28) = param_1;
  *(undefined4 *)(this + 0x18) = 3;
  *(undefined4 *)(this + 0x24) = 0;
  *(undefined4 *)this = 0;
  this[0x15] = (CPersistContext)0x0;
  return;
}



//=== CPersistContextReadSection @ 00cd45e0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall CPersistContextReadSection::CPersistContextReadSection(void) */

CPersistContextReadSection * __thiscall
CPersistContextReadSection::CPersistContextReadSection(CPersistContextReadSection *this)

{
  CCharString::CCharString((CCharString *)this);
  CCharString::CCharString((CCharString *)(this + 4));
  CCharString::CCharString((CCharString *)(this + 8));
  return this;
}



//=== _Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*> @ 00cdc000 ===

/* [bsim sim=0.5595659242102596 <- ego_r]
   struct std::pair<unsigned long,class CDefString> * __fastcall std::_Move_backward<struct
   std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class CDefString>
   *>(struct std::pair<unsigned long,class CDefString> *,struct std::pair<unsigned long,class
   CDefString> *,struct std::pair<unsigned long,class CDefString> *,struct
   std::_Nonscalar_ptr_iterator_tag) */

pair<unsigned_long,CDefString> * __fastcall
std::_Move_backward<std::pair<unsigned_long,CDefString>*,std::pair<unsigned_long,class_CDefString>*>
          (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *param_1,
          allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *param_2)

{
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar1;
  pair<unsigned_long,CDefString> *in_EAX;
  pair<unsigned_long,CDefString> *extraout_EAX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *extraout_EDX_02;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar3;
  
  paVar2 = param_2;
  for (; param_1 != param_2; param_1 = param_1 + 0x1c) {
    paVar1 = *(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)(param_1 + 0x14);
    for (paVar3 = *(allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> **)
                   (param_1 + 0x10); paVar3 != paVar1; paVar3 = paVar3 + 4) {
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (paVar3,(pair<EHeroMorphType,CParticleMorphs::CEntry> *)paVar2,unaff_ESI);
      paVar2 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)extraout_EDX;
    }
    if (*(void **)(param_1 + 0x10) != (void *)0x0) {
      free(*(void **)(param_1 + 0x10));
      paVar2 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)extraout_EDX_00;
    }
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (param_1 + 0xc,(pair<EHeroMorphType,CParticleMorphs::CEntry> *)paVar2,unaff_ESI);
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              (param_1,extraout_EDX_01,unaff_ESI);
    paVar2 = extraout_EDX_02;
    in_EAX = extraout_EAX;
  }
  return in_EAX;
}



