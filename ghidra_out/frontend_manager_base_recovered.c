//=== CManager @ 0041e3f6 (seed 0041e3f6) ===

/* [bsim sim=0.9490024296799859 <- ego_r]
   protected: __thiscall NUISystem::CManager::CManager(void) */

CManager * __thiscall NUISystem::CManager::CManager(CManager *this)

{
  ulong uVar1;
  undefined4 uVar2;
  CEngineInternalPrimitiveBase *this_00;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar4;
  EGenericVarType unaff_EBX;
  CEnginePrimitive *unaff_ESI;
  CNavigationLayer *pCVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  CNavigationLayer local_40 [4];
  CPixelFormat local_3c [8];
  CPixelFormat local_34 [4];
  CPixelFormat local_30 [4];
  CPixelFormat local_2c [4];
  CPixelFormat local_28 [4];
  undefined1 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined1 local_18;
  undefined1 *local_14;
  undefined4 local_10;
  undefined1 *local_c [2];
  
  CConsoleCommandParameters::CConsoleCommandParameters
            ((CConsoleCommandParameters *)this,(EGenericVarType)unaff_ESI,unaff_EBX);
  *(undefined ***)this = &PTR_LAB_01230134;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x24));
  std__list__list();
  Std_DoubleLinkedList_CreateNode();
  *(undefined4 *)(this + 100) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x70));
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x88));
  *(undefined8 *)(this + 0xa8) = 0;
  this[0x9c] = (CManager)0x0;
  *(undefined4 *)(this + 0xa0) = 0;
  this[0xa4] = (CManager)0x0;
  *(undefined4 *)(this + 0xb8) = 0;
  *(undefined4 *)(this + 0xbc) = 0;
  *(undefined4 *)(this + 0xc4) = 0xbf800000;
  this[200] = (CManager)0x0;
  this[0xc9] = (CManager)0x0;
  this[0xca] = (CManager)0x0;
  this[0xcb] = (CManager)0x0;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = 0;
  *(undefined4 *)(this + 0x40) = 0;
  *(undefined4 *)(this + 0x44) = 0;
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  *(undefined4 *)(this + 0x54) = 0;
  *(undefined4 *)(this + 0x58) = 0;
  CUIScreenUnitConverter::SetIsUsingRelativeCoords(true);
  SetInput(this,0);
  *(undefined4 *)(this + 0x3c) = 0;
  CNavQuadTree::CNavigationLayer::CNavigationLayer(local_40);
  CDisplayManager::GetBestCompressedAlphaTextureFormat(DAT_013b8390,0x10,(CPixelFormat *)local_40);
  CDisplayManager::GetBestCompressedAlphaTextureFormat(DAT_013b8390,0x10,local_3c);
  CDisplayManager::GetBestCompressedAlphaTextureFormat(DAT_013b8390,0x10,local_34);
  CDisplayManager::GetBestAlphaTextureFormat(DAT_013b8390,0x20,local_2c);
  CDisplayManager::GetBestOpaqueTextureFormat(DAT_013b8390,0x20,local_30,false);
  CDisplayManager::GetBestSignedTextureFormat(DAT_013b8390,0x10,local_28);
  local_20 = 0xffffffff;
  local_1c = 0xffffffff;
  local_24 = 0;
  local_18 = 0;
  if ((DAT_013b871c == 0) || (*(char *)(DAT_013b871c + 9) == '\0')) {
    uVar1 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,unaff_ESI,unaff_EBX);
    local_10 = *(undefined4 *)(uVar1 + 0x6c);
    local_14 = &stack0xffffffb0;
    uVar7 = 1;
    uVar6 = 1;
    pCVar5 = local_40;
    uVar2 = CCharString::CCharString((CCharString *)local_c,"GBANK_MAIN",-1);
    CGraphicDataBank__CreateAndInitialize(local_14,uVar2,pCVar5,uVar6,uVar7);
    SetGraphicsBank(this);
    paVar3 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c;
    ppVar4 = extraout_EDX_00;
  }
  else {
    uVar1 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,unaff_ESI,unaff_EBX);
    local_10 = *(undefined4 *)(uVar1 + 0x6c);
    local_c[0] = &stack0xffffffb0;
    uVar7 = 1;
    uVar6 = 1;
    pCVar5 = local_40;
    uVar2 = CCharString::CCharString((CCharString *)&local_14,"GBANK_FRONT_END",-1);
    CGraphicDataBank__CreateAndInitialize(local_c[0],uVar2,pCVar5,uVar6,uVar7);
    SetGraphicsBank(this);
    paVar3 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_14;
    ppVar4 = extraout_EDX;
  }
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (paVar3,ppVar4,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_ESI);
  SetMetaLayer(this,0);
  return this;
}


// CALLEES of CManager:
//   0042a9b7  SetGraphicsBank
//   0042bf67  CopyBackBufferToTexture
//   00415b80  CNavigationLayer
//   009be8b0  GetBestCompressedAlphaTextureFormat
//   0099ebf0  CCharString
//   009f83d0  CGraphicDataBank::CreateAndInitialize
//   0042ab58  std::list::list
//   009be830  GetBestCompressedAlphaTextureFormat
//   0042ab01  Std_DoubleLinkedList_CreateNode
//   0042bf85  CopyBackBufferToTexture
//   0042be7b  CConsoleCommandParameters
//   009be6c0  GetBestAlphaTextureFormat
//   009be610  GetBestOpaqueTextureFormat
//   009be590  GetBestSignedTextureFormat
//   004299a8  SetIsUsingRelativeCoords
//   009be870  GetBestCompressedAlphaTextureFormat
//   0042d28b  CopyBackBufferToTexture
//   009a4ec0  AddChildPrimitive
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0041e1cd  SetMetaLayer
//   0041df10  SetInput

//=== CFrontEndManager @ 005953e2 (seed 005953e2) ===

/* [bsim sim=0.7212874166685309 <- ego_r]
   public: __thiscall NUISystem::CFrontEndManager::CFrontEndManager(void) */

CFrontEndManager * __thiscall NUISystem::CFrontEndManager::CFrontEndManager(CFrontEndManager *this)

{
  uchar unaff_SI;
  undefined4 uStack_4;
  
  uStack_4 = this;
  CFrontEndManager__Initialize();
  *(undefined ***)this = &PTR__vector_deleting_destructor__012521a8;
  CCharString::CCharString((CCharString *)(this + 0x14));
  Std_DoubleLinkedList_CreateNode((int)&uStack_4 + 3);
  CGameEvent::AddUBYTE((CGameEvent *)(this + 0x20),unaff_SI);
  *(undefined4 *)(this + 0x48) = 0;
  *(undefined4 *)(this + 0x4c) = 0;
  *(undefined4 *)(this + 0x50) = 0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0x54));
  *(undefined4 *)(this + 0x60) = 0;
  this[100] = (CFrontEndManager)0x0;
  *(undefined4 *)(this + 0x68) = 0;
  *(undefined4 *)(this + 0x6c) = 0;
  *(undefined4 *)(this + 0x70) = 0;
  *(undefined4 *)(this + 0x74) = 0;
  *(undefined4 *)(this + 0x78) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x80) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)(this + 0x8c) = 0;
  *(undefined4 *)(this + 0x90) = 0;
  *(undefined4 *)(this + 0x94) = 0;
  *(undefined4 *)(this + 0x98) = 0;
  *(undefined4 *)(this + 0x9c) = 0;
  this[0xa0] = (CFrontEndManager)0x0;
  *(undefined4 *)(this + 0xa4) = 0;
  *(undefined4 *)(this + 0xa8) = 0;
  CCharString::CCharString((CCharString *)(this + 0xac));
  CCharString::CCharString((CCharString *)(this + 0xb0));
  this[0xc0] = (CFrontEndManager)0x0;
  CDisplayManager::CopyBackBufferToTexture((CDisplayManager *)(this + 0xc4));
  this[0xd8] = (CFrontEndManager)0x0;
  this[0xd9] = (CFrontEndManager)0x0;
  return this;
}


// CALLEES of CFrontEndManager:
//   00486055  CopyBackBufferToTexture
//   0052d9e0  CFrontEndManager::Initialize
//   0059b32a  CopyBackBufferToTexture
//   0059b310  AddUBYTE
//   0099aed0  CCharString
//   0042ab01  Std_DoubleLinkedList_CreateNode

//=== GetInstance @ 00595582 (seed 00595582) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: static class NUISystem::CFrontEndManager * __fastcall
   NUISystem::CFrontEndManager::GetInstance(void) */

CFrontEndManager * __fastcall NUISystem::CFrontEndManager::GetInstance(void)

{
  CFrontEndManager *this;
  
  if (DAT_013b8b5c == (CFrontEndManager *)0x0) {
    this = ::operator_new(0xe0);
    if (this == (CFrontEndManager *)0x0) {
      DAT_013b8b5c = (CFrontEndManager *)0x0;
    }
    else {
      DAT_013b8b5c = (CFrontEndManager *)CFrontEndManager(this);
    }
  }
  return DAT_013b8b5c;
}


// CALLEES of GetInstance:
//   005953e2  CFrontEndManager
//   00bfea1a  operator_new

//=== GotoProfileMenu @ 00597b20 (seed 00597b20) ===

/* [bsim sim=0.7175784189178503 <- ego_r]
   protected: void __thiscall NUISystem::CFrontEndManager::GotoProfileMenu(void) */

void __thiscall NUISystem::CFrontEndManager::GotoProfileMenu(CFrontEndManager *this)

{
  CTCTrophy *this_00;
  int *piVar1;
  uint uVar2;
  long *plVar3;
  int *extraout_EAX;
  int *extraout_EAX_00;
  CFrontEndManager *pCVar4;
  CThing *unaff_EDI;
  bool bVar5;
  CFrontEndManager *local_c;
  CFrontEndManager *local_8;
  
  local_c = this;
  local_8 = this;
  local_c = (CFrontEndManager *)RefreshAvailableProfiles(this);
  this_00 = (CTCTrophy *)(this + 0x20);
  uVar2 = NThingMap::CThingMapManager::GetMemoryFootprint((CThingMapManager *)(this + 0x30));
  if (1 < uVar2) {
    local_8 = (CFrontEndManager *)0xb;
    plVar3 = std::
             map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
             ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                           *)(this + 0x54),(EUsedKeys *)&local_8);
    CTCTrophy::OnShowTrophyInitialise(this_00,unaff_EDI);
    if (*extraout_EAX == *plVar3) {
      CTCTrophy::OnShowTrophyInitialise(this_00,unaff_EDI);
      piVar1 = (int *)*extraout_EAX_00;
      (**(code **)(*piVar1 + 0xc0))(6);
      pCVar4 = GetInstance();
      (**(code **)(*(int *)pCVar4 + 0x14))(piVar1 + 1);
      CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
      bVar5 = local_c == (CFrontEndManager *)0x0;
      local_c = (CFrontEndManager *)0x7;
      if (bVar5) {
        local_c = (CFrontEndManager *)0xc;
      }
      plVar3 = std::
               map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
               ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                             *)(this + 0x54),(EUsedKeys *)&local_c);
      *(long *)(this + 0x9c) = *plVar3;
      *(int **)(this + 0x98) = piVar1;
      CCreatureAction_FireMissileWeapon::FrameUpdate((CCreatureAction_FireMissileWeapon *)this_00);
      return;
    }
  }
  local_c = (CFrontEndManager *)0x7;
  plVar3 = std::
           map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
           ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                         *)(this + 0x54),(EUsedKeys *)&local_c);
  GotoNextScreen(this,(CComponent *)*plVar3,false);
  return;
}


// CALLEES of GotoProfileMenu:
//   0059aee5  FrameUpdate
//   0059b039  OnShowTrophyInitialise
//   0059b61c  FrameUpdate
//   00596cce  RefreshAvailableProfiles
//   0059a8f3  GetMemoryFootprint
//   0059b5d7  operator[]
//   00596763  GotoNextScreen
//   0041e5f2  GetInstance

//=== RefreshAvailableProfiles @ 00596cce (seed 00596cce) ===

/* [bsim sim=0.5039661763012944 <- ego_r]
   protected: long __thiscall NUISystem::CFrontEndManager::RefreshAvailableProfiles(void) */

long __thiscall NUISystem::CFrontEndManager::RefreshAvailableProfiles(CFrontEndManager *this)

{
  int *piVar1;
  int iVar2;
  char cVar3;
  long *plVar4;
  int *piVar5;
  CComponent *pCVar6;
  CRumbleDef *pCVar7;
  CAction *pCVar8;
  undefined4 uVar9;
  CActionParamString *this_00;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *extraout_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_05;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_06;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_07;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_08;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_09;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar10;
  CWideString *local_68;
  int local_64;
  undefined4 local_60;
  CAction *local_5c;
  CCPPointerInfo *local_58;
  int *local_54 [2];
  undefined4 *local_4c [2];
  int *local_44;
  CActiveFile local_40 [4];
  int *local_3c;
  undefined4 local_38;
  int *local_34 [2];
  int *local_2c;
  CCharString local_28 [4];
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  local_24 [4];
  int *local_20;
  CManager *local_1c;
  CCharString local_18 [4];
  CCharString local_14 [4];
  float local_10;
  CFrontEndManager *local_c;
  CWideString *local_8;
  
  local_68 = (CWideString *)0x0;
  local_64 = 0;
  local_60 = 0;
  local_c = this;
  GetProfileNames(this,(vector<CWideString,std::allocator<CWideString>_> *)&local_68);
  local_1c = (CManager *)GetInstance();
  local_20 = (int *)0x7;
  plVar4 = std::
           map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
           ::operator[]((map<NUISystem::CManager::EUsedKeys,long,std::less<NUISystem::CManager::EUsedKeys>,std::allocator<std::pair<NUISystem::CManager::EUsedKeys_const_,long>_>_>
                         *)(this + 0x54),(EUsedKeys *)&local_20);
  piVar1 = (int *)*plVar4;
  iVar2 = *piVar1;
  ppVar10 = extraout_ECX;
  CCharString::CCharString((CCharString *)&stack0xffffff88,"UI_FRONTEND_LIST_FOR_PROFILES",-1);
  piVar5 = (int *)(**(code **)(iVar2 + 0xc))();
  local_20 = piVar5;
  if (piVar5 != (int *)0x0) {
    CBankFile::GetBankHandle((CBankFile *)&local_44);
    iVar2 = *piVar5;
    local_44 = (int *)0x1;
    std::pair<unsigned_long,CAnimationEntry>::pair<unsigned_long,CAnimationEntry>
              ((pair<unsigned_long,CAnimationEntry> *)&stack0xffffff80,
               (pair<unsigned_long,class_CAnimationEntry> *)&local_44);
    (**(code **)(iVar2 + 0xf8))();
    CFileInstaller::CActiveFile::OnReadFinished(local_40);
  }
  CCharString::CCharString((CCharString *)&local_10,"UI_FRONTEND_LIST_FOR_PROFILES",-1);
  pCVar6 = CManager::CreateComponent(local_1c,(CCharString *)&local_10,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_5c,(CAction *)pCVar6);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,extraout_EDX
             ,ppVar10);
  local_3c = (int *)0x0;
  local_38 = 0;
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_3c,local_5c,local_58);
  ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_5c;
  (**(code **)(*piVar1 + 0xec))();
  CCharString::CCharString((CCharString *)&local_10,"TEXT_GUI_MENU_NEW_PROFILE",-1);
  NScript::
  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
            (local_24,(CIsThingAlive *)&local_10);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,
             extraout_EDX_00,ppVar10);
  CCharString::CCharString((CCharString *)&local_8,"",-1);
  CCharString::CCharString((CCharString *)&local_10,"UI_FRONTEND_BUTTON_FOR_PROFILES_LIST",-1);
  pCVar6 = CreateFrontEndButtonComponent
                     (local_c,(CWideString *)local_24,0.0,(CCharString *)&local_10,
                      (CCharString *)&local_8,true);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_54,(CAction *)pCVar6);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_10,
             extraout_EDX_01,ppVar10);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_02,ppVar10);
  pCVar7 = ::operator_new(0x10);
  if (pCVar7 == (CRumbleDef *)0x0) {
    pCVar8 = (CAction *)0x0;
  }
  else {
    pCVar8 = (CAction *)CRumbleDef::CRumbleDef(pCVar7);
  }
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_4c,pCVar8);
  *local_4c[0] = 0x125;
  (**(code **)(*local_54[0] + 0x108))();
  (**(code **)(*local_54[0] + 0x11c))(local_4c);
  ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_54;
  (**(code **)(*local_3c + 0xf0))();
  local_2c = (int *)(local_64 - (int)local_68 >> 2);
  local_10 = 50.0;
  if (local_2c != (int *)0x0) {
    local_8 = local_68;
    local_20 = local_2c;
    do {
      CCharString::CCharString(local_18,"",-1);
      CCharString::CCharString(local_14,"UI_FRONTEND_BUTTON_FOR_PROFILES_LIST",-1);
      pCVar6 = CreateFrontEndButtonComponent(local_c,local_8,local_10,local_14,local_18,false);
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_44,(CAction *)pCVar6);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_14,
                 extraout_EDX_03,ppVar10);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18,
                 extraout_EDX_04,ppVar10);
      pCVar7 = ::operator_new(0x10);
      if (pCVar7 == (CRumbleDef *)0x0) {
        pCVar8 = (CAction *)0x0;
      }
      else {
        pCVar8 = (CAction *)CRumbleDef::CRumbleDef(pCVar7);
      }
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)local_34,pCVar8);
      CTBaseSingleton<CCodeSectionManager>::Get();
      uVar9 = CPersistContext::GetDefaultVal<CSpecialEffectsStringMap>();
      cVar3 = CUserProfileManager_VerifyAndLoadProfile(uVar9);
      CCharString::~CCharString(local_28);
      *local_34[0] = (-(uint)(cVar3 != '\0') & 0x49) + 0xdb;
      this_00 = ::operator_new(0xc);
      if (this_00 == (CActionParamString *)0x0) {
        local_1c = (CManager *)0x0;
      }
      else {
        local_1c = (CManager *)NUISystem::CActionParamString::CActionParamString(this_00,local_8);
      }
      CUIState__operator_(&local_1c);
      (**(code **)(*local_44 + 0x108))(local_34);
      (**(code **)(*local_44 + 0x11c))(local_34);
      (**(code **)(*local_3c + 0xf0))(&local_44);
      local_10 = local_10 + 30.0;
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 local_34,extraout_EDX_05);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_44,extraout_EDX_06);
      local_8 = local_8 + 4;
      local_20 = (int *)((int)local_20 + -1);
    } while (local_20 != (int *)0x0);
    local_20 = (int *)0x0;
  }
  piVar5 = local_2c;
  (**(code **)(*piVar1 + 0xac))();
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_4c
             ,extraout_EDX_07);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_54
             ,extraout_EDX_08);
  CCharString::~CCharString((CCharString *)local_24);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_3c,extraout_EDX_09);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_5c,extraout_EDX_10);
  CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&local_68);
  return (long)piVar5;
}


// CALLEES of RefreshAvailableProfiles:
//   0099b510  ~CCharString
//   0099b6a0  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
//   00429160  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   00429c15  CCountedPointer<NUISystem::CAction>
//   0041e5f2  GetInstance
//   0042bed4  GetBankHandle
//   0042abca  OnReadFinished
//   0059a994  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   005955ab  GetProfileNames
//   0042b646  CUIState::operator=
//   00412130  ResetRings
//   004082d0  GetDefaultVal<CSpecialEffectsStringMap>
//   0042cd84  pair<unsigned_long,CAnimationEntry>
//   0054dae0  CActionParamString
//   0042aa29  CCountedPointer<NUISystem::CAction>
//   0099ebf0  CCharString
//   0040d2a0  Get
//   0059b5d7  operator[]
//   0041db1d  CreateComponent
//   0059aa86  ShareData
//   005957d9  CUserProfileManager_VerifyAndLoadProfile
//   0042be50  CRumbleDef
//   004291de  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   00596a68  CreateFrontEndButtonComponent
//   00bfea1a  operator_new
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>

