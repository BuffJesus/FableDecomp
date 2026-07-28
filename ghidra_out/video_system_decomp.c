//=== ?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z @ 0042ec7c (seed 0042ec7c) ===

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 _Run_CNewFrontendGameComponent__UAE_NPAPAVCGameComponent___Z(undefined4 *param_1)

{
  int iVar1;
  int iVar2;
  CGame *pCVar3;
  bool bVar4;
  CFrontEndManager *pCVar5;
  CSystemManager *pCVar6;
  ESystemReturn EVar7;
  CWideString *pCVar8;
  CXMVPlayerBuffers *this;
  ulong uVar9;
  int iVar10;
  CFrontendGameComponent *this_00;
  undefined4 uVar11;
  CMainGameComponent *this_01;
  CNewFrontendGameComponent *in_ECX;
  CEngineInternalPrimitiveBase *this_02;
  CEngineInternalPrimitiveBase *this_03;
  CEngineInternalPrimitiveBase *extraout_ECX;
  CEngineInternalPrimitiveBase *extraout_ECX_00;
  CEngineInternalPrimitiveBase *extraout_ECX_01;
  CEngineInternalPrimitiveBase *extraout_ECX_02;
  CEngineInternalPrimitiveBase *this_04;
  CEngineInternalPrimitiveBase *this_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  CMacro *extraout_EDX_06;
  int *piVar12;
  ulong unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined1 *puVar13;
  double dVar14;
  float fVar15;
  CCharString *pCVar16;
  CXMVPlayerBuffers **ppCVar17;
  char *pcVar18;
  wchar_t *pwVar19;
  CEnginePrimitive *pCVar20;
  undefined1 uVar21;
  ulong uVar22;
  CMainGameComponentInit local_9c [12];
  CWideString local_90 [4];
  CEngineInterpolationInfo local_8c [16];
  char *local_7c;
  ulong local_78 [2];
  undefined1 local_70 [4];
  DWORD local_6c;
  ulong local_68;
  float local_64;
  code *local_60;
  char *local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined1 local_50;
  undefined1 local_4f;
  undefined1 local_4e;
  undefined1 local_4d;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  code *local_40;
  char *local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined1 local_30;
  undefined1 local_2f;
  undefined1 local_2e;
  undefined1 local_2d;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  code *local_20;
  char local_19;
  undefined4 local_18;
  undefined4 local_14;
  CCharString local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  CXMVPlayerBuffers *local_c;
  CEngineInternalPrimitiveBase *local_8;
  
  dVar14 = GFGetTime();
  *(float *)(in_ECX + 0xb8) = (float)dVar14;
  dVar14 = GFGetTime();
  *(float *)(in_ECX + 0xbc) = (float)dVar14;
  piVar12 = (int *)0x0;
  local_64 = 20.0;
  local_44 = 0x41a00000;
  local_7c = "Data\\Video\\lionhead_logo.xmv";
  local_78[0] = 0x280;
  local_78[1] = 400;
  local_70[2] = 0xff;
  local_70[1] = 0xff;
  local_70[0] = 0xff;
  local_70[3] = 0xff;
  local_6c = 0;
  local_68 = 0x1a0;
  local_60 = FrontEnd_CheckQuickAssignInventory;
  local_5c = "Data\\Video\\Microsoft_Logo.xmv";
  local_58 = 0x280;
  local_54 = 0x1e0;
  local_4e = 0;
  local_4f = 0;
  local_50 = 0;
  local_4d = 0xff;
  local_4c = 0;
  local_48 = 0x1a0;
  local_40 = FrontEnd_CheckQuickAssignInventory;
  local_3c = "Data\\Video\\intro_comp.xmv";
  local_38 = 0x280;
  local_34 = 0x168;
  local_2e = 0;
  local_2f = 0;
  local_30 = 0;
  local_2d = 0;
  local_2c = 0;
  local_28 = 0x1a0;
  local_24 = 0x41200000;
  local_20 = FrontEnd_CheckQuickAssignInventory;
  if ((DAT_01375448 != '\0') && (DAT_0137544a != '\0')) {
    local_c = (CXMVPlayerBuffers *)0x0;
    local_18 = 0xff000000;
    puVar13 = local_70 + 1;
    do {
      uVar11 = local_14;
      local_14._0_3_ = CONCAT12(puVar13[1],(undefined2)local_14);
      local_14._0_2_ = CONCAT11(*puVar13,SUB41(uVar11,0));
      local_14 = CONCAT31(local_14._1_3_,puVar13[-1]);
      local_14 = CONCAT13(puVar13[2],(undefined3)local_14);
      _DAT_013961e0 = local_14;
      DAT_013ca7e9 = 1;
      CCharString::CCharString(&local_10);
      CCharString::CCharString((CCharString *)&local_8,*(char **)(puVar13 + -0xd),-1);
      local_19 = GFPlayFullScreenXBoxMovie
                           ((CCharString *)&local_8,*(ulong *)(puVar13 + 7),*(ulong *)(puVar13 + -9)
                            ,*(ulong *)(puVar13 + -5),0xa00000,*(CDataBank **)(in_ECX + 0x60),
                            *(CIEngine **)(in_ECX + 0x58),true,*(float *)(puVar13 + 0xb),
                            *(_func_bool **)(puVar13 + 0xf),(CXMVPlayerBuffers *)0x0,
                            (CWideString *)&local_10);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
                 extraout_EDX,unaff_EDI);
      if (local_19 == '\0') {
        in_ECX[0x144] = (CNewFrontendGameComponent)0x0;
        pCVar5 = NUISystem::CFrontEndManager::GetInstance();
        CWideString::operator=((CWideString *)(pCVar5 + 0x14),(CWideString *)&local_10);
      }
      DAT_013ca7e9 = 0;
      _DAT_013961e0 = local_18;
      Sleep(*(DWORD *)(puVar13 + 3));
      pCVar6 = (CSystemManager *)
               CEngineInternalPrimitiveBase::AddChildPrimitive
                         (this_02,(CEnginePrimitive *)unaff_EDI,unaff_ESI);
      EVar7 = CSystemManager::Update(pCVar6);
      if (EVar7 == 2) {
        CCharString::~CCharString(&local_10);
        return 0;
      }
      CCharString::~CCharString(&local_10);
      local_c = local_c + 1;
      puVar13 = puVar13 + 0x20;
    } while (local_c < (CXMVPlayerBuffers *)0x3);
  }
  if (DAT_013b8616 != '\0') {
    CCharString__AssignFromWide(&DAT_0122daa4);
    uVar21 = 1;
    pwVar19 = L"frontend.big";
    ppCVar17 = &local_c;
    CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&local_18);
    CCharString__AssignFromData(ppCVar17);
    pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
    CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
    CCharString::~CCharString((CCharString *)&local_8);
    CCharString::~CCharString(&local_10);
    CCharString::~CCharString((CCharString *)&local_18);
    uVar21 = 0;
    pwVar19 = L"text.big";
    NResourceDirectoryNames::GetDVDDialogueDir();
    pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
    CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
    CCharString::~CCharString((CCharString *)&local_8);
    CCharString::~CCharString(&local_10);
    uVar21 = 0;
    pwVar19 = L"shaders.big";
    ppCVar17 = &local_c;
    CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&local_18);
    CCharString__AssignFromData(ppCVar17);
    pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
    CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
    CCharString::~CCharString((CCharString *)&local_8);
    CCharString::~CCharString(&local_10);
    CCharString::~CCharString((CCharString *)&local_18);
    CCharString::~CCharString((CCharString *)&local_c);
  }
  in_ECX[9] = (CNewFrontendGameComponent)0x1;
  CNewFrontendGameComponent::Init2(in_ECX);
  CCharString::CCharString((CCharString *)&local_8,"Init Engine",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,unaff_EDI);
  CNewFrontendGameComponent::InitialiseEngine(in_ECX);
  _E2();
  CCharString::CCharString((CCharString *)&local_8,"Init frontend",-1);
  _E2();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_01,unaff_EDI);
  this = operator_new(0x10);
  if (this == (CXMVPlayerBuffers *)0x0) {
    local_c = (CXMVPlayerBuffers *)0x0;
  }
  else {
    local_c = (CXMVPlayerBuffers *)CXMVPlayerBuffers::CXMVPlayerBuffers(this);
  }
  *(undefined1 *)(DAT_013b838c + 0x38) = 0;
  CCharString__AssignFromWide(L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz()");
  uVar22 = 1;
  pCVar20 = (CEnginePrimitive *)&local_8;
  (**(code **)(**(int **)(in_ECX + 0x10) + 0x10))();
  CCharString::~CCharString((CCharString *)&local_8);
  uVar9 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_03,pCVar20,uVar22);
  local_8 = *(CEngineInternalPrimitiveBase **)(uVar9 + 0x60);
  local_e = 0;
  local_f = 0;
  local_10 = (CCharString)0x0;
  local_d = 0xff;
  CRenderManager2D::ClearRenderTarget(DAT_013b8384,(CRGBColour *)&local_10,0);
  CDisplayManager::SwapScreens((CDisplayManager *)local_8);
  CFrontEnd_PlayAutoSaveSound(0);
  NUISystem::CFrontEndManager::ChangeStateFirstTime(*(CFrontEndManager **)(in_ECX + 0xb4));
  CStopWatch::StartZero((CStopWatch *)(in_ECX + 0x100));
  CStopWatch::Reset((CStopWatch *)(in_ECX + 0x100));
  _E2();
  NUISystem::CUIScreenUnitConverter::ResetDimensions();
  this_04 = extraout_ECX;
  while( true ) {
    pCVar6 = (CSystemManager *)
             CEngineInternalPrimitiveBase::AddChildPrimitive(this_04,pCVar20,uVar22);
    EVar7 = CSystemManager::Update(pCVar6);
    if ((EVar7 == 2) || (in_ECX[8] != (CNewFrontendGameComponent)0x0)) break;
    uVar9 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_05,pCVar20,uVar22);
    local_8 = *(CEngineInternalPrimitiveBase **)(uVar9 + 0x170);
    if (*(char *)(uVar9 + 0x16c) != '\0') {
      uVar9 = CEngineInternalPrimitiveBase::AddChildPrimitive(local_8,pCVar20,uVar22);
      *(undefined4 *)(uVar9 + 0x170) = 0;
      *(undefined1 *)(uVar9 + 0x16c) = 0;
      CFrontEnd_PlayAutoSaveSound(local_8);
    }
    if ((in_ECX[0x138] != (CNewFrontendGameComponent)0x0) &&
       (fVar15 = CStopWatch::GetElapsedSeconds((CStopWatch *)(in_ECX + 0x120)), 0.2 <= fVar15)) {
      CStopWatch::Stop((CStopWatch *)(in_ECX + 0x120));
    }
    CNewFrontendGameComponent::Input(in_ECX);
    CNewFrontendGameComponent::Update(in_ECX);
    CEngineInterpolationInfo::CEngineInterpolationInfo(local_8c);
    CNewFrontendGameComponent::UpdateInterpolation(in_ECX,local_8c);
    CNewFrontendGameComponent::Draw(in_ECX,local_8c);
    fVar15 = CStopWatch::GetElapsedSeconds((CStopWatch *)(in_ECX + 0x100));
    this_04 = extraout_ECX_00;
    if ((120.0 <= fVar15) &&
       (bVar4 = CThingFilter_IsUnconscious::operator()
                          (*(CThingFilter_IsUnconscious **)(in_ECX + 0xb4),(CThing *)pCVar20),
       this_04 = extraout_ECX_01, bVar4)) {
      if (DAT_013b8394 != (int *)0x0) {
        (**(code **)(*DAT_013b8394 + 0x48))(0);
      }
      iVar1 = *(int *)(*(int *)(in_ECX + 0x3c) + 0x40);
      iVar2 = *(int *)(*(int *)(in_ECX + 0x3c) + 0x3c);
      iVar10 = rand();
      CCharString::CCharString
                ((CCharString *)&local_14,
                 (CCharString *)
                 (*(int *)(*(int *)(in_ECX + 0x3c) + 0x3c) + (iVar10 % (iVar1 - iVar2 >> 2)) * 4));
      uVar9 = NLocalisation::GetLanguage();
      if (uVar9 == 4) {
        pcVar18 = "English";
      }
      else if (uVar9 == 7) {
        pcVar18 = "German";
      }
      else if (uVar9 == 10) {
        pcVar18 = "Spanish";
      }
      else if (uVar9 == 0xc) {
        pcVar18 = "French";
      }
      else if (uVar9 == 0x10) {
        pcVar18 = "Italian";
      }
      else {
        pcVar18 = "English";
      }
      CCharString::operator+=((CCharString *)&local_14,pcVar18);
      CCharString::operator+=((CCharString *)&local_14,".wmv");
      DAT_013ca7e9 = 1;
      GFPlayFullScreenXBoxMovie
                ((CCharString *)&local_14,0x221,0,0,0x100000,*(CDataBank **)(in_ECX + 0x60),
                 *(CIEngine **)(in_ECX + 0x58),true,-1.0,(_func_bool *)0x0,local_c,
                 (CWideString *)0x0);
      DAT_013ca7e9 = 0;
      CStopWatch::Reset((CStopWatch *)(in_ECX + 0x100));
      CInputManager::ClearEvents(DAT_013b8388);
      CStopWatch::StartZero((CStopWatch *)(in_ECX + 0x120));
      CFrontEnd_PlayAutoSaveSound(0);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_14,
                 extraout_EDX_02,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      this_04 = extraout_ECX_02;
    }
  }
  in_ECX[8] = (CNewFrontendGameComponent)0x1;
  in_ECX[9] = (CNewFrontendGameComponent)0x0;
  if (local_c != (CXMVPlayerBuffers *)0x0) {
    (*(code *)**(undefined4 **)local_c)(1);
  }
  CNewFrontendGameComponent::ReleaseXMVCode(in_ECX);
  DAT_013b871c = 0;
  CEngineResourceManager::ChangeScene((CEngineResourceManager *)in_ECX);
  if (in_ECX[0x2a] == (CNewFrontendGameComponent)0x0) {
    if (in_ECX[0x29] == (CNewFrontendGameComponent)0x0) {
      DAT_013b871c = 0;
      CNewFrontendGameComponent::Shutdown(in_ECX);
      uVar11 = 0;
      DAT_013b7d58 = in_ECX;
    }
    else {
      DAT_01375448 = '\0';
      CCharString::CCharString((CCharString *)&local_8,"Leave frontend",-1);
      _E2();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
                 extraout_EDX_03,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      if (DAT_013b8394 != (int *)0x0) {
        (**(code **)(*DAT_013b8394 + 0x48))(500);
      }
      if (DAT_013b8616 != '\0') {
        CCharString__AssignFromWide(&DAT_0122daa4);
        pwVar19 = L"frontend.big";
        pCVar16 = &local_10;
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&local_18);
        CCharString__AssignFromData(pCVar16);
        pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
        CBankFileManager::CloseRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8);
        CCharString::~CCharString((CCharString *)&local_8);
        CCharString::~CCharString((CCharString *)&local_c);
        CCharString::~CCharString((CCharString *)&local_18);
        uVar21 = 1;
        pwVar19 = L"graphics.big";
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&local_c);
        pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&local_8);
        CCharString::~CCharString((CCharString *)&local_c);
        uVar21 = 1;
        pwVar19 = L"textures.big";
        pCVar16 = &local_10;
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&local_18);
        CCharString__AssignFromData(pCVar16);
        pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&local_8);
        CCharString::~CCharString((CCharString *)&local_c);
        CCharString::~CCharString((CCharString *)&local_18);
        uVar21 = 0;
        pwVar19 = L"dialogue.big";
        NResourceDirectoryNames::GetDVDDialogueDir();
        pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&local_8);
        CCharString::~CCharString((CCharString *)&local_c);
        uVar21 = 0;
        pwVar19 = L"effects.big";
        pCVar16 = &local_10;
        CUserProfileManager::GetAutoSaveFileName((CUserProfileManager *)&local_18);
        CCharString__AssignFromData(pCVar16);
        pCVar8 = (CWideString *)CCharString__AssignFromValue(pwVar19);
        CBankFileManager::OpenRetailBank((CBankFileManager *)&DAT_013ca79c,pCVar8,(bool)uVar21);
        CCharString::~CCharString((CCharString *)&local_8);
        CCharString::~CCharString((CCharString *)&local_c);
        CCharString::~CCharString((CCharString *)&local_18);
        CCharString::~CCharString(&local_10);
      }
      CTBaseSingleton<CFileInstaller>::Release();
      CMainGameComponentInit::CMainGameComponentInit(local_9c);
      CWideString::operator=(local_90,(CWideString *)(in_ECX + 0x84));
      CCharString::CCharString((CCharString *)&local_8,"FinalAlbion.wld",-1);
      pCVar8 = (CWideString *)
               NScript::
               operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                         ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                           *)&local_c,(CIsThingAlive *)&local_8);
      CWideString::operator=((CWideString *)local_9c,pCVar8);
      CCharString::~CCharString((CCharString *)&local_c);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
                 extraout_EDX_04,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      _E2();
      pCVar3 = *(CGame **)(in_ECX + 0xc);
      CNewFrontendGameComponent::Shutdown(in_ECX);
      CCharString::CCharString((CCharString *)&local_8,"Init Game",-1);
      _E2();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
                 extraout_EDX_05,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar20)
      ;
      this_01 = operator_new(0x161e8);
      if (this_01 != (CMainGameComponent *)0x0) {
        piVar12 = (int *)CMainGameComponent::CMainGameComponent(this_01,pCVar3,local_9c);
      }
      (**(code **)(*piVar12 + 4))();
      *param_1 = piVar12;
      DAT_013b7d58 = in_ECX;
      std::_Dest_val<std::allocator<CMacro>,CMacro>((allocator<CMacro> *)local_9c,extraout_EDX_06);
      uVar11 = 1;
    }
  }
  else {
    CNewFrontendGameComponent::Shutdown(in_ECX);
    this_00 = operator_new(0x1e60);
    if (this_00 == (CFrontendGameComponent *)0x0) {
      piVar12 = (int *)0x0;
    }
    else {
      local_8 = (CEngineInternalPrimitiveBase *)0x0;
      piVar12 = (int *)CFrontendGameComponent::CFrontendGameComponent
                                 (this_00,*(CGame **)(in_ECX + 0xc),
                                  (CFrontendGameComponentInit *)&local_8);
    }
    (**(code **)(*piVar12 + 4))();
    *param_1 = piVar12;
    DAT_013b871c = 0;
    uVar11 = 1;
    DAT_013b7d58 = in_ECX;
  }
  return uVar11;
}


// CALLEES of ?Run@CNewFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z:
//   0099be70  CCharString::AssignFromData
//   009d8250  $E2
//   0099b6b0  CCharString::AssignFromWide
//   0099f100  operator+=
//   0062f900  GetElapsedSeconds
//   0062f850  Stop
//   0042e204  InitialiseEngine
//   0041a0c0  GetAutoSaveFileName
//   004131a0  CMainGameComponentInit
//   0042dd28  ChangeScene
//   009d8240  $E2
//   00bfea1a  operator_new
//   009a78d0  CloseRetailBank
//   0099ec30  CCharString
//   00404490  Release
//   0099b7d0  operator=
//   0041a330  GetDVDDialogueDir
//   00496070  CFrontendGameComponent
//   0041a1e0  GetAutoSaveFileName
//   009d8cf0  ClearRenderTarget
//   0042e98f  Init2
//   0042dbd8  ReleaseXMVCode
//   00595a03  operator()
//   0042ded5  CFrontEnd_PlayAutoSaveSound
//   0099aed0  CCharString
//   0099ebf0  CCharString
//   00415070  GetLanguage
//   0042dc94  Update
//   006286f0  GFPlayFullScreenXBoxMovie
//   009a6460  Update
//   0041a100  GetAutoSaveFileName
//   0042ebb6  Shutdown
//   00595582  GetInstance
//   009f5540  ClearEvents
//   009e1bc0  GFGetTime
//   00418dca  CMainGameComponent
//   0042db40  CXMVPlayerBuffers
//   009beeb0  SwapScreens
//   0042dbfa  UpdateInterpolation
//   0062f800  StartZero
//   0042e3ee  Input
//   0042fa30  CEngineInterpolationInfo
//   00bfeb16  rand
//   009a8840  OpenRetailBank
//   005952c3  ChangeStateFirstTime
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099b6a0  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
//   004131d0  _Dest_val<std::allocator<CMacro>,CMacro>
//   0099b510  ~CCharString
//   0099bf30  CCharString::AssignFromValue
//   EXTERNAL:000000b8  Sleep
//   0062f8b0  Reset
//   0042df9e  Draw
//   009a4ec0  AddChildPrimitive
//   0040f0e0  ResetDimensions

//=== CMovie::FreeTextures @ 00547b90 (seed 00547b90) ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   protected: void __thiscall NUISystem::CMovie::FreeTextures(void) */

void __thiscall NUISystem::CMovie::CMovie__FreeTextures(CMovie *this)

{
  if (DAT_013b8ac0 != 0) {
    (**(code **)(*DAT_013b8ab8 + 0x118))(DAT_013b8ac0);
    DAT_013b8ac0 = 0;
  }
  return;
}


// CALLEES of CMovie::FreeTextures:

//=== CMovie::RestoreEngineBuffers @ 00547bc0 (seed 00547bc0) ===

void CMovie__RestoreEngineBuffers(void)

{
  return;
}


// CALLEES of CMovie::RestoreEngineBuffers:

//=== CMovie::Initialise @ 00547bd0 (seed 00547bd0) ===

void __thiscall
NUISystem::CChangingStateComponent::CMovie__Initialise(CChangingStateComponent *this)

{
  Initialise(this);
  return;
}


// CALLEES of CMovie::Initialise:
//   0052c730  Initialise

//=== CMovie::GetSize @ 00547be0 (seed 00547be0) ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual class C2DVector __thiscall NUISystem::CMovie::GetSize(void)const  */

void __thiscall NUISystem::CMovie::CMovie__GetSize(CMovie *this)

{
  float fVar1;
  float fVar2;
  float *in_stack_00000004;
  
  fVar2 = *(float *)(this + 0xd0);
  fVar1 = *(float *)(this + 0x60);
  *in_stack_00000004 = *(float *)(this + 0xcc) * *(float *)(this + 0x5c);
  in_stack_00000004[1] = fVar2 * fVar1;
  return;
}


// CALLEES of CMovie::GetSize:

//=== CMovie::GetDataFromDef @ 00547c10 (seed 00547c10) ===

/* [bsim sim=0.6086360973917236 <- ego_r]
   public: void __thiscall NUISystem::CTextContainer::GetDataFromDef(void) */

void __thiscall NUISystem::CTextContainer::CMovie__GetDataFromDef(CTextContainer *this)

{
  int iVar1;
  undefined4 uVar2;
  int *unaff_ESI;
  CTextContainer *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  uVar2 = __ftol2();
  *(undefined4 *)(this + 0x168) = uVar2;
  uVar2 = __ftol2();
  *(undefined4 *)(this + 0x16c) = uVar2;
  this[0x170] = *(CTextContainer *)(unaff_ESI + 0x2f);
  CWideString::operator=((CWideString *)(this + 0x174),(CWideString *)(unaff_ESI + 0x15));
  *(int *)(this + 400) = unaff_ESI[0x2e];
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}


// CALLEES of CMovie::GetDataFromDef:
//   00bfea70  __ftol2
//   0099b7d0  operator=

//=== CMovie::ChangeState @ 00547c90 (seed 00547c90) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual void __thiscall NUISystem::CMovie::ChangeState(unsigned long) */

void __thiscall NUISystem::CMovie::CMovie__ChangeState(CMovie *this,ulong param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  ulong uVar4;
  char cVar5;
  CVideoSys *this_00;
  long lVar6;
  CPixelFormat aCStack_c [4];
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uVar4 = param_1;
  iVar2 = *(int *)(this + 0x148);
  CChangingStateComponent::ChangeState((CChangingStateComponent *)this,param_1);
  if ((uVar4 == 3) && (iVar2 != 3)) {
    if (DAT_013b8abc == (CVideoSys *)0x0) {
      this_00 = ::operator_new(0x8c);
      if (this_00 == (CVideoSys *)0x0) {
        DAT_013b8abc = (CVideoSys *)0x0;
      }
      else {
        DAT_013b8abc = (CVideoSys *)CVideoSys::CVideoSys__CVideoSys(this_00);
      }
      *(undefined4 *)(DAT_013b8abc + 0x24) = 0;
    }
    if (DAT_013b8ab4 != (CMovie *)0x0) {
      cVar5 = (**(code **)(*(int *)DAT_013b8ab4 + 0x23c))();
      if ((cVar5 != '\0') && (DAT_013b8abc != (CVideoSys *)0x0)) {
        CVideoSys::CVideoSys__Close(DAT_013b8abc);
      }
    }
    this[0x15c] = (CMovie)0x0;
    CAFile::ConvertPathToFullPath((CWideString *)&param_1);
    lVar6 = CVideoSys::CVideoSys__Init(DAT_013b8abc,(CWideString *)&param_1,(CTexture *)0x0,true);
    if (-1 < lVar6) {
      *(float *)(this + 0xcc) = (float)*(int *)(*(int *)(DAT_013b8abc + 0x20) + 0x16c);
      *(float *)(this + 0xd0) = (float)*(int *)(*(int *)(DAT_013b8abc + 0x20) + 0x170);
      piVar3 = *(int **)(this + 0x164);
      if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
        (**(code **)(*piVar3 + 0xc))(this + 0x160);
      }
      piVar3 = *(int **)(this + 0x164);
      if (piVar3 != (int *)0x0) {
        piVar1 = piVar3 + 1;
        *piVar1 = *piVar1 + -1;
        if (*piVar1 == 0) {
          (**(code **)(*piVar3 + 4))();
        }
        *(undefined4 *)(this + 0x164) = 0;
      }
      *(undefined4 *)(this + 0x164) = 0;
      if (*(int **)(this + 0xdc) != (int *)0x0) {
        (**(code **)(**(int **)(this + 0xdc) + 0x16c))();
      }
      if (DAT_013b8ac0 != 0) {
        (**(code **)(*DAT_013b8ab8 + 0x118))(DAT_013b8ac0);
        DAT_013b8ac0 = 0;
      }
      uStack_4 = *(undefined4 *)(*(int *)(DAT_013b8abc + 0x20) + 0x170);
      uStack_8 = *(undefined4 *)(*(int *)(DAT_013b8abc + 0x20) + 0x16c);
      CPixelFormat::Initialise(aCStack_c,8,8,8,8,false);
      DAT_013b8ac0 = (**(code **)(*DAT_013b8ab8 + 0x114))(0,&uStack_8,1,aCStack_c);
      this[0x194] = (CMovie)0x1;
      this[0x15c] = (CMovie)0x1;
    }
    DAT_013b8ab4 = this;
    CCharString::~CCharString((CCharString *)&param_1);
  }
  return;
}


// CALLEES of CMovie::ChangeState:
//   0052cf40  ChangeState
//   00999110  ConvertPathToFullPath
//   00bfea1a  operator_new
//   00a3b9d0  CVideoSys::Init
//   00a3b380  CVideoSys::Close
//   00a3bc70  CVideoSys::CVideoSys
//   0099b510  ~CCharString
//   009e3790  Initialise

//=== CMovie::Update @ 00547e40 (seed 00547e40) ===

void CMovie__Update(float param_1)

{
  int *piVar1;
  int *piVar2;
  CEngineLandscapePatch *pCVar3;
  bool bVar4;
  char cVar5;
  CChangingStateComponent *in_ECX;
  
  NUISystem::CChangingStateComponent::Update(in_ECX,param_1);
  if ((((*(int *)(in_ECX + 0x144) == 3) && (DAT_013b8abc != (CVideoSys *)0x0)) &&
      (in_ECX[0x15c] == (CChangingStateComponent)0x1)) && (DAT_013b8ab4 == in_ECX)) {
    if (in_ECX[0x194] != (CChangingStateComponent)0x0) {
      bVar4 = CVideoSys::CVideoSys__AttemptToPlay(DAT_013b8abc);
      if (bVar4) {
        in_ECX[0x194] = (CChangingStateComponent)0x0;
      }
    }
    CVideoSys::CVideoSys__Process(DAT_013b8abc);
  }
  cVar5 = (**(code **)(*(int *)in_ECX + 0x224))();
  if (((cVar5 != '\0') && (*(int *)(in_ECX + 0x144) != 3)) &&
     ((in_ECX[0x15c] == (CChangingStateComponent)0x1 && (DAT_013b8ab4 == in_ECX)))) {
    CVideoSys::CVideoSys__Close(DAT_013b8abc);
    in_ECX[0x15c] = (CChangingStateComponent)0x0;
    DAT_013b8ab4 = (CChangingStateComponent *)0x0;
    NUISystem::CMovie::CMovie__FreeTextures((CMovie *)in_ECX);
    pCVar3 = (CEngineLandscapePatch *)DAT_013b8abc;
    if (DAT_013b8abc != (CVideoSys *)0x0) {
      CEngineLandscapePatch::CVideoSys___CVideoSys((CEngineLandscapePatch *)DAT_013b8abc);
      operator_delete(pCVar3);
    }
    DAT_013b8abc = (CVideoSys *)0x0;
    piVar2 = *(int **)(in_ECX + 0x164);
    if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
      (**(code **)(*piVar2 + 0xc))(in_ECX + 0x160);
    }
    piVar2 = *(int **)(in_ECX + 0x164);
    if (piVar2 != (int *)0x0) {
      piVar1 = piVar2 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar2 + 4))();
      }
      *(undefined4 *)(in_ECX + 0x164) = 0;
    }
    *(undefined4 *)(in_ECX + 0x164) = 0;
    if (*(int **)(in_ECX + 0xdc) != (int *)0x0) {
      (**(code **)(**(int **)(in_ECX + 0xdc) + 0x16c))();
    }
  }
  return;
}


// CALLEES of CMovie::Update:
//   00a3bc20  CVideoSys::~CVideoSys
//   00a3b1a0  CVideoSys::AttemptToPlay
//   00a3b380  CVideoSys::Close
//   00a3b430  CVideoSys::Process
//   0052c7e0  Update
//   00bfe9bc  operator_delete
//   00547b90  CMovie::FreeTextures

//=== CMovie::Draw @ 00547f60 (seed 00547f60) ===

void CMovie__Draw(int param_1,undefined4 param_2,int *param_3)

{
  int *piVar1;
  float fVar2;
  int *piVar3;
  char cVar4;
  char cVar5;
  CFrontEndManager *pCVar6;
  EEngine2DLayer EVar7;
  DWORD DVar8;
  int *in_ECX;
  int iVar9;
  undefined4 unaff_ESI;
  int *unaff_retaddr;
  uchar uStack_7d;
  undefined4 uStack_78;
  float fStack_74;
  float fStack_70;
  float fStack_6c;
  float fStack_68;
  float fStack_64;
  float fStack_60;
  float fStack_5c;
  float afStack_58 [22];
  
  uStack_7d = (uchar)((uint)unaff_ESI >> 0x18);
  if (((DAT_013b8ab4 != in_ECX) || (in_ECX[0x51] != 3)) || ((char)in_ECX[0x57] == '\0')) {
    piVar3 = (int *)in_ECX[0x59];
    if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
      (**(code **)(*piVar3 + 0xc))(in_ECX + 0x58);
    }
    piVar3 = (int *)in_ECX[0x59];
    if (piVar3 != (int *)0x0) {
      piVar1 = piVar3 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar3 + 4))();
      }
      in_ECX[0x59] = 0;
    }
    in_ECX[0x59] = 0;
    return;
  }
  if ((*(char *)((int)in_ECX + 0x97) == '\0') ||
     (((float)in_ECX[0x1f] < 0.0 != ((float)in_ECX[0x1f] == 0.0) &&
      ((float)in_ECX[0x20] < 0.0 != ((float)in_ECX[0x20] == 0.0))))) {
    piVar3 = (int *)in_ECX[0x59];
    if ((piVar3 != (int *)0x0) && (piVar3[1] == 1)) {
      (**(code **)(*piVar3 + 0xc))(in_ECX + 0x58);
    }
    piVar3 = (int *)in_ECX[0x59];
    if (piVar3 != (int *)0x0) {
      piVar1 = piVar3 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar3 + 4))();
      }
      in_ECX[0x59] = 0;
    }
    in_ECX[0x59] = 0;
    if ((int *)in_ECX[0x37] != (int *)0x0) {
      (**(code **)(*(int *)in_ECX[0x37] + 0x16c))();
      return;
    }
  }
  else {
    iVar9 = in_ECX[0x5a];
    if (iVar9 == 0) {
      fStack_6c = (float)in_ECX[0x33];
    }
    else {
      fStack_6c = (float)iVar9;
      if (iVar9 < 0) {
        fStack_6c = fStack_6c + 4.2949673e+09;
      }
    }
    iVar9 = in_ECX[0x5b];
    if (iVar9 == 0) {
      fVar2 = (float)in_ECX[0x34];
    }
    else {
      fVar2 = (float)iVar9;
      if (iVar9 < 0) {
        fVar2 = fVar2 + 4.2949673e+09;
      }
    }
    fStack_74 = (float)in_ECX[0x3e];
    fStack_6c = fStack_6c * (float)in_ECX[0x42];
    fStack_70 = (float)in_ECX[0x3f];
    fStack_5c = fStack_6c + fStack_74;
    afStack_58[0] = fStack_70 + fVar2 * (float)in_ECX[0x43];
    fStack_64 = fStack_74;
    fStack_60 = fStack_70;
    if ((char)in_ECX[0x5c] != '\0') {
      fStack_6c = fStack_74 - fStack_6c * 0.5;
      fStack_68 = fStack_70 - fVar2 * (float)in_ECX[0x43] * 0.5;
      C2DBoxF::Move((C2DBoxF *)&fStack_64,(C2DVector *)&fStack_6c);
    }
    cVar4 = (**(code **)(*in_ECX + 0x194))();
    cVar5 = (**(code **)(*in_ECX + 0x1a0))();
    if ((cVar4 == '\0') && (cVar5 == '\0')) {
      pCVar6 = NUISystem::CFrontEndManager::GetInstance();
      iVar9 = (int)*(char *)((int)in_ECX + 0x12f) + (int)param_3;
    }
    else {
      pCVar6 = NUISystem::CFrontEndManager::GetInstance();
      iVar9 = (int)*(char *)((int)in_ECX + 0x12f);
    }
    EVar7 = (**(code **)(*(int *)pCVar6 + 0x90))(iVar9);
    DVar8 = WaitForSingleObjectEx(*(HANDLE *)(DAT_013b8abc + 0x7c),100,1);
    if ((DVar8 == 0) && (DAT_013b8ac0 != (CTexture *)0x0)) {
      EnterCriticalSection((LPCRITICAL_SECTION)(DAT_013b8abc + 100));
      CTexture::CopyFromTexture(DAT_013b8ac0,*(CTexture **)(DAT_013b8abc + 0x80),0xffffffff);
      LeaveCriticalSection((LPCRITICAL_SECTION)(DAT_013b8abc + 100));
      fStack_70 = 0.0;
      fStack_6c = 0.0;
      uStack_78 = 0;
      fStack_74 = 0.0;
      CEnginePrimitive2DSprite::CEnginePrimitive2DSprite
                ((CEnginePrimitive2DSprite *)afStack_58,0,(C2DBoxF *)&fStack_68,EVar7,DAT_013b8ac0,2
                 ,0,(float)in_ECX[100],(float)in_ECX[100],uStack_7d,(uchar)((uint)unaff_ESI >> 0x10)
                 ,(uchar)((uint)unaff_ESI >> 8),(uchar)unaff_ESI,2,(C2DVector *)&uStack_78,
                 (C2DVector *)&fStack_70);
      if ((param_1 == 0) || (param_3 == (int *)0x0)) {
        (**(code **)(*unaff_retaddr + 0x5c))(in_ECX + 0x58,afStack_58,0x40);
        return;
      }
      iVar9 = *param_3;
      *param_3 = iVar9 + 1;
      (**(code **)(*unaff_retaddr + 0x70))(param_1,iVar9,afStack_58,0x40);
    }
  }
  return;
}


// CALLEES of CMovie::Draw:
//   009fa4e0  CopyFromTexture
//   0041e5f2  GetInstance
//   EXTERNAL:000000b5  LeaveCriticalSection
//   EXTERNAL:000000b4  EnterCriticalSection
//   0041a810  Move
//   0041bf60  CEnginePrimitive2DSprite
//   EXTERNAL:000000b3  WaitForSingleObjectEx

//=== CMovie::CMovie @ 005482d0 (seed 005482d0) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: __thiscall NUISystem::CMovie::CMovie(class CIEngine &,long) */

CMovie * __thiscall NUISystem::CMovie::CMovie__CMovie(CMovie *this,CIEngine *param_1,long param_2)

{
  CDraggableInto::CDraggableInto((CDraggableInto *)this,param_2);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124881c;
  *(undefined ***)(this + 4) = &PTR_LAB_012487f4;
  *(undefined ***)(this + 0x18) = &PTR_LAB_012487ec;
  this[0x15c] = (CMovie)0x0;
  NHeroInformationScreens::CBase::CBase((CBase *)(this + 0x160));
  *(undefined ***)(this + 0x160) = &PTR__scalar_deleting_destructor__0122f598;
  *(undefined4 *)(this + 0x164) = 0;
  CCharString::CCharString((CCharString *)(this + 0x174));
  ::CXMVPlaybackInfo::CXMVPlaybackInfo((CXMVPlaybackInfo *)(this + 0x178));
  this[0x194] = (CMovie)0x0;
  DAT_013b8ab8 = param_1;
  CTextContainer::CMovie__GetDataFromDef((CTextContainer *)this);
  return this;
}


// CALLEES of CMovie::CMovie:
//   0099aed0  CCharString
//   009bc7d0  CXMVPlaybackInfo
//   0099a2d0  CBase
//   0052cc50  CDraggableInto
//   00547c10  CMovie::GetDataFromDef

//=== CMovie::CopyConstructor @ 00548350 (seed 00548350) ===

void CMovie__CopyConstructor(undefined4 param_1)

{
  CTextContainer *in_ECX;
  
  func_0x0052cca0(param_1);
  *(undefined ***)in_ECX = &PTR__vector_deleting_destructor__0124881c;
  *(undefined ***)(in_ECX + 4) = &PTR_LAB_012487f4;
  *(undefined ***)(in_ECX + 0x18) = &PTR_LAB_012487ec;
  in_ECX[0x15c] = (CTextContainer)0x0;
  NHeroInformationScreens::CBase::CBase((CBase *)(in_ECX + 0x160));
  *(undefined ***)(in_ECX + 0x160) = &PTR__scalar_deleting_destructor__0122f598;
  *(undefined4 *)(in_ECX + 0x164) = 0;
  CCharString::CCharString((CCharString *)(in_ECX + 0x174));
  CXMVPlaybackInfo::CXMVPlaybackInfo((CXMVPlaybackInfo *)(in_ECX + 0x178));
  in_ECX[0x194] = (CTextContainer)0x0;
  NUISystem::CTextContainer::CMovie__GetDataFromDef(in_ECX);
  return;
}


// CALLEES of CMovie::CopyConstructor:
//   0099aed0  CCharString
//   009bc7d0  CXMVPlaybackInfo
//   0099a2d0  CBase
//   00547c10  CMovie::GetDataFromDef

//=== CMovie::~CMovie @ 005483c0 (seed 005483c0) ===

/* WARNING: Removing unreachable block (ram,0x005484c3) */
/* WARNING: Removing unreachable block (ram,0x005484c8) */
/* WARNING: Removing unreachable block (ram,0x005484cd) */
/* [bsim sim=0.941841344092242 <- ego_r]
   public: virtual __thiscall NUISystem::CMovie::~CMovie(void) */

void __thiscall NUISystem::CMovie::CMovie___CMovie(CMovie *this)

{
  int *piVar1;
  CBase *this_00;
  int *piVar2;
  CEngineLandscapePatch *pCVar3;
  
  this_00 = (CBase *)(this + 0x160);
  *(undefined ***)this = &PTR__vector_deleting_destructor__0124881c;
  *(undefined ***)(this + 4) = &PTR_LAB_012487f4;
  *(undefined ***)(this + 0x18) = &PTR_LAB_012487ec;
  piVar2 = *(int **)(this + 0x164);
  if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
    (**(code **)(*piVar2 + 0xc))(this_00);
  }
  piVar2 = *(int **)(this + 0x164);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x164) = 0;
  }
  *(undefined4 *)(this + 0x164) = 0;
  if (*(int **)(this + 0xdc) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0xdc) + 0x16c))();
  }
  if (DAT_013b8ab4 == this) {
    if (this[0x15c] != (CMovie)0x0) {
      CVideoSys::CVideoSys__Close(DAT_013b8abc);
    }
    DAT_013b8ab4 = (CMovie *)0x0;
    if (DAT_013b8ac0 != 0) {
      (**(code **)(*DAT_013b8ab8 + 0x118))(DAT_013b8ac0);
      DAT_013b8ac0 = 0;
    }
    pCVar3 = (CEngineLandscapePatch *)DAT_013b8abc;
    if (DAT_013b8abc != (CVideoSys *)0x0) {
      CEngineLandscapePatch::CVideoSys___CVideoSys((CEngineLandscapePatch *)DAT_013b8abc);
      operator_delete(pCVar3);
    }
    DAT_013b8abc = (CVideoSys *)0x0;
  }
  CCharString::~CCharString((CCharString *)(this + 0x174));
  *(undefined ***)this_00 = &PTR__scalar_deleting_destructor__0122f598;
  piVar2 = *(int **)(this + 0x164);
  if ((piVar2 != (int *)0x0) && (piVar2[1] == 1)) {
    (**(code **)(*piVar2 + 0xc))(this_00);
  }
  piVar2 = *(int **)(this + 0x164);
  if (piVar2 != (int *)0x0) {
    piVar1 = piVar2 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*piVar2 + 4))();
    }
    *(undefined4 *)(this + 0x164) = 0;
  }
  *(undefined4 *)(this + 0x164) = 0;
  NHeroInformationScreens::CBase::CBase(this_00);
  CTCIdleScheduler::Clear((CTCIdleScheduler *)this);
  return;
}


// CALLEES of CMovie::~CMovie:
//   00a3bc20  CVideoSys::~CVideoSys
//   0099a2e0  CBase
//   0052ccf0  Clear
//   00a3b380  CVideoSys::Close
//   00bfe9bc  operator_delete
//   0099b510  ~CCharString

//=== CMovie::SetMovie @ 00548510 (seed 00548510) ===

void CMovie__SetMovie(CWideString *param_1)

{
  int in_ECX;
  
  CWideString::operator=((CWideString *)(in_ECX + 0x174),param_1);
  return;
}


// CALLEES of CMovie::SetMovie:
//   0099b7d0  operator=

//=== CMovie::IsPlaying @ 00548520 (seed 00548520) ===

undefined1 CMovie__IsPlaying(void)

{
  int in_ECX;
  
  return *(undefined1 *)(in_ECX + 0x15c);
}


// CALLEES of CMovie::IsPlaying:

//=== CVideoSys::WaitForState @ 00a3b0f0 (seed 00a3b0f0) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CVideoSys::WaitForState(enum _FilterState) */

void __thiscall CVideoSys::CVideoSys__WaitForState(CVideoSys *this,_FilterState param_1)

{
  _FilterState _Var1;
  int iVar2;
  
  _Var1 = param_1;
  iVar2 = 0;
  do {
    (**(code **)(**(int **)(this + 4) + 0x28))(*(int **)(this + 4),10,&param_1);
    iVar2 = iVar2 + 1;
    if (_Var1 == param_1) {
      return;
    }
  } while (iVar2 < 100);
  return;
}


// CALLEES of CVideoSys::WaitForState:

//=== CVideoSys::Play @ 00a3b130 (seed 00a3b130) ===

/* [bsim sim=0.9317890791756035 <- ego_r]
   public: bool __thiscall CVideoSys::Play(void) */

bool __thiscall CVideoSys::CVideoSys__Play(CVideoSys *this)

{
  int iVar1;
  int iVar2;
  
  if (((*(int *)(this + 0x28) != 0) && (*(int *)(this + 4) != 0)) && (*(int *)(this + 8) != 0)) {
    do {
      iVar1 = (**(code **)(**(int **)(this + 8) + 0x20))(*(int **)(this + 8),0);
    } while (iVar1 != 0);
    iVar1 = 0;
    do {
      if (0x31 < iVar1) break;
      iVar1 = iVar1 + 1;
      iVar2 = (**(code **)(**(int **)(this + 4) + 0x1c))(*(int **)(this + 4));
    } while (iVar2 != 0);
    if (iVar1 != 0x32) {
      CVideoSys__WaitForState(this,2);
      *(undefined4 *)(this + 0x28) = 1;
      return true;
    }
  }
  return false;
}


// CALLEES of CVideoSys::Play:
//   00a3b0f0  CVideoSys::WaitForState

//=== CVideoSys::AttemptToPlay @ 00a3b1a0 (seed 00a3b1a0) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: bool __thiscall CVideoSys::AttemptToPlay(void) */

bool __thiscall CVideoSys::CVideoSys__AttemptToPlay(CVideoSys *this)

{
  int iVar1;
  
  iVar1 = (**(code **)(**(int **)(this + 4) + 0x1c))(*(int **)(this + 4));
  if (iVar1 == 0) {
    *(undefined4 *)(this + 0x28) = 1;
    return true;
  }
  return false;
}


// CALLEES of CVideoSys::AttemptToPlay:

//=== CVideoSys::Pause @ 00a3b1c0 (seed 00a3b1c0) ===

/* [bsim sim=0.6489697723765849 <- ego_r]
   protected: bool __thiscall NGraphicAppearance::CTCGraphicAppearance::GetAnims(class
   C3DAnimationInterpolator &)const  */

bool __thiscall
NGraphicAppearance::CTCGraphicAppearance::CVideoSys__Pause
          (CTCGraphicAppearance *this,C3DAnimationInterpolator *param_1)

{
  if (*(int *)(this + 0x28) == 0) {
    return false;
  }
  (**(code **)(**(int **)(this + 4) + 0x20))(*(int **)(this + 4));
  CVideoSys::CVideoSys__WaitForState((CVideoSys *)this,1);
  *(undefined4 *)(this + 0x28) = 2;
  return true;
}


// CALLEES of CVideoSys::Pause:
//   00a3b0f0  CVideoSys::WaitForState

//=== CVideoSys::Stop @ 00a3b1f0 (seed 00a3b1f0) ===

/* [bsim sim=0.6489697723765849 <- ego_r]
   protected: bool __thiscall NGraphicAppearance::CTCGraphicAppearance::GetAnims(class
   C3DAnimationInterpolator &)const  */

bool __thiscall
NGraphicAppearance::CTCGraphicAppearance::CVideoSys__Stop
          (CTCGraphicAppearance *this,C3DAnimationInterpolator *param_1)

{
  if (*(int *)(this + 0x28) == 0) {
    return false;
  }
  (**(code **)(**(int **)(this + 4) + 0x24))(*(int **)(this + 4));
  CVideoSys::CVideoSys__WaitForState((CVideoSys *)this,0);
  *(undefined4 *)(this + 0x28) = 3;
  return true;
}


// CALLEES of CVideoSys::Stop:
//   00a3b0f0  CVideoSys::WaitForState

//=== CVideoSys::GetLength @ 00a3b220 (seed 00a3b220) ===

float10 CVideoSys__GetLength(void)

{
  int *piVar1;
  int in_ECX;
  undefined8 uStack_8;
  
  if (*(int *)(in_ECX + 8) == 0) {
    return (float10)0.0;
  }
  piVar1 = *(int **)(in_ECX + 8);
  uStack_8 = 0;
  (**(code **)(*piVar1 + 0x1c))();
  return (float10)(double)CONCAT44(&uStack_8,piVar1);
}


// CALLEES of CVideoSys::GetLength:

//=== CVideoSys::GetCurTime @ 00a3b260 (seed 00a3b260) ===

float10 CVideoSys__GetCurTime(void)

{
  int *piVar1;
  int in_ECX;
  undefined8 uStack_8;
  
  if (*(int *)(in_ECX + 8) == 0) {
    return (float10)0.0;
  }
  piVar1 = *(int **)(in_ECX + 8);
  uStack_8 = 0;
  (**(code **)(*piVar1 + 0x24))();
  return (float10)(double)CONCAT44(&uStack_8,piVar1);
}


// CALLEES of CVideoSys::GetCurTime:

//=== CVideoSys::GetProgress @ 00a3b2a0 (seed 00a3b2a0) ===

/* WARNING: Type propagation algorithm not settling */
/* [bsim sim=0.6264140332266089 <- ego_r]
   public: float __thiscall CVideoSys::GetProgress(void) */

float __thiscall CVideoSys::CVideoSys__GetProgress(CVideoSys *this)

{
  int *piVar1;
  double *pdVar2;
  double local_10 [2];
  
  if (*(int *)(this + 8) == 0) {
    local_10[0] = (double)((ulonglong)local_10[0] & 0xffffffff00000000);
  }
  else {
    local_10[0] = 0.0;
    (**(code **)(**(int **)(this + 8) + 0x24))(*(int **)(this + 8),local_10);
    local_10[0] = (double)CONCAT44(local_10[0]._4_4_,(float)local_10[0]);
  }
  piVar1 = *(int **)(this + 8);
  if (piVar1 != (int *)0x0) {
    pdVar2 = local_10 + 1;
    local_10[1] = 0.0;
    (**(code **)(*piVar1 + 0x1c))(piVar1);
    if ((float)local_10[0] != 0.0) {
      return (float)pdVar2 / (float)local_10[0];
    }
  }
  return 0.0;
}


// CALLEES of CVideoSys::GetProgress:

//=== CVideoSys::GetTexture @ 00a3b320 (seed 00a3b320) ===

undefined4 CVideoSys__GetTexture(void)

{
  int in_ECX;
  
  return *(undefined4 *)(*(int *)(in_ECX + 0x20) + 0x164);
}


// CALLEES of CVideoSys::GetTexture:

//=== CVideoSys::IsFinished @ 00a3b330 (seed 00a3b330) ===

/* [bsim sim=0.7323961207255326 <- ego_r]
   public: bool __thiscall CVideoSys::IsFinished(void) */

bool __thiscall CVideoSys::CVideoSys__IsFinished(CVideoSys *this)

{
  float fVar1;
  
  if ((*(int *)(this + 0x28) != 1) && (*(int *)(this + 0x28) != 2)) {
    return true;
  }
  fVar1 = CVideoSys__GetProgress(this);
  if ((1.0 <= fVar1) && (*(int *)(this + 0x28) != 0)) {
    (**(code **)(**(int **)(this + 4) + 0x24))(*(int **)(this + 4));
    CVideoSys__WaitForState(this,0);
    *(undefined4 *)(this + 0x28) = 3;
  }
  return false;
}


// CALLEES of CVideoSys::IsFinished:
//   00a3b2a0  CVideoSys::GetProgress
//   00a3b0f0  CVideoSys::WaitForState

//=== CVideoSys::Close @ 00a3b380 (seed 00a3b380) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: void __thiscall CVideoSys::Close(void) */

void __thiscall CVideoSys::CVideoSys__Close(CVideoSys *this)

{
  int *piVar1;
  
  piVar1 = *(int **)(this + 4);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 4) = 0;
  }
  piVar1 = *(int **)(this + 8);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 8) = 0;
  }
  piVar1 = *(int **)(this + 0xc);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 0xc) = 0;
  }
  piVar1 = *(int **)(this + 0x14);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 0x14) = 0;
  }
  piVar1 = *(int **)(this + 0x1c);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 0x1c) = 0;
  }
  piVar1 = *(int **)(this + 0x18);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 0x18) = 0;
  }
  piVar1 = *(int **)(this + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)(this + 0x10) = 0;
  }
  piVar1 = *(int **)this;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
    *(undefined4 *)this = 0;
  }
  if (*(HANDLE *)(this + 0x7c) != (HANDLE)0x0) {
    CloseHandle(*(HANDLE *)(this + 0x7c));
    *(undefined4 *)(this + 0x7c) = 0;
    DeleteCriticalSection((LPCRITICAL_SECTION)(this + 100));
  }
  return;
}


// CALLEES of CVideoSys::Close:
//   EXTERNAL:000000aa  DeleteCriticalSection
//   EXTERNAL:000000bd  CloseHandle

//=== CVideoSys::Process @ 00a3b430 (seed 00a3b430) ===

/* [bsim sim=0.9899665842833637 <- ego_r]
   public: bool __thiscall CVideoSys::Process(void) */

bool __thiscall CVideoSys::CVideoSys__Process(CVideoSys *this)

{
  C3DAnimationInterpolator *pCVar1;
  undefined4 *puVar2;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  pCVar1 = *(C3DAnimationInterpolator **)(this + 0x10);
  local_c = 0;
  local_4 = 0;
  local_8 = 0;
  if (pCVar1 != (C3DAnimationInterpolator *)0x0) {
    puVar2 = &local_c;
    (**(code **)(*(int *)pCVar1 + 0x20))(pCVar1,puVar2,&local_4,&local_8,0);
    if (puVar2 == (undefined4 *)0x1) {
      if (*(int *)(this + 0x24) == 0) {
        CVideoSys__Play(this);
        return true;
      }
      NGraphicAppearance::CTCGraphicAppearance::CVideoSys__Stop((CTCGraphicAppearance *)this,pCVar1)
      ;
    }
  }
  return true;
}


// CALLEES of CVideoSys::Process:
//   00a3b130  CVideoSys::Play
//   00a3b1f0  CVideoSys::Stop

//=== CVideoSys::TogglePause @ 00a3b4a0 (seed 00a3b4a0) ===

void CVideoSys__TogglePause(char param_1)

{
  int iVar1;
  CVideoSys *in_ECX;
  
  if (param_1 == '\0') {
    if (((*(int *)(in_ECX + 0x28) != 0) && (*(int *)(in_ECX + 4) != 0)) &&
       (*(int *)(in_ECX + 8) != 0)) {
      do {
        iVar1 = (**(code **)(**(int **)(in_ECX + 4) + 0x1c))(*(int **)(in_ECX + 4));
      } while (iVar1 != 0);
      CVideoSys::CVideoSys__WaitForState(in_ECX,2);
      *(undefined4 *)(in_ECX + 0x28) = 1;
    }
  }
  else if (*(int *)(in_ECX + 0x28) != 0) {
    (**(code **)(**(int **)(in_ECX + 4) + 0x20))(*(int **)(in_ECX + 4));
    CVideoSys::CVideoSys__WaitForState(in_ECX,1);
    *(undefined4 *)(in_ECX + 0x28) = 2;
    return;
  }
  return;
}


// CALLEES of CVideoSys::TogglePause:
//   00a3b0f0  CVideoSys::WaitForState

//=== CTextureRenderer::CTextureRenderer @ 00a3b510 (seed 00a3b510) ===

/* [bsim sim=0.569559329002901 <- donor]
   public: __thiscall CTextureRenderer::CTextureRenderer(struct IUnknown *,long *,class CTexture *)
    */

CTextureRenderer * __thiscall
CTextureRenderer::CTextureRenderer__CTextureRenderer
          (CTextureRenderer *this,IUnknown *param_1,long *param_2,CTexture *param_3)

{
  NUISystem::CSpellContainerList::CSpellContainerList((CSpellContainerList *)this,0x129d150);
  *(undefined ***)this = &PTR_LAB_0129d08c;
  *(undefined ***)(this + 0xc) = &PTR_FUN_0129d04c;
  *(undefined ***)(this + 0x10) = &PTR_FUN_0129d030;
  *(undefined ***)(this + 0xe0) = &PTR_FUN_0129d008;
  *(undefined ***)(this + 0xe4) = &PTR_FUN_0129cff0;
  *(undefined4 *)(this + 0x160) = 0;
  this[0x178] = (CTextureRenderer)0x0;
  *param_2 = 0;
  *(CTexture **)(this + 0x164) = param_3;
  if (param_3 == (CTexture *)0x0) {
    this[0x178] = (CTextureRenderer)0x1;
  }
  return this;
}


// CALLEES of CTextureRenderer::CTextureRenderer:
//   00ca7360  CSpellContainerList

//=== CTextureRenderer::CheckMediaType @ 00a3b590 (seed 00a3b590) ===

undefined4 CTextureRenderer__CheckMediaType(int *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  bool bVar5;
  
  iVar2 = 4;
  bVar5 = true;
  piVar3 = param_1 + 0xb;
  piVar4 = &DAT_012aabb4;
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    bVar5 = *piVar3 == *piVar4;
    piVar3 = piVar3 + 1;
    piVar4 = piVar4 + 1;
  } while (bVar5);
  uVar1 = 0x80004005;
  if (!bVar5) {
    return 0x80070057;
  }
  iVar2 = 4;
  bVar5 = true;
  piVar3 = param_1;
  piVar4 = &DAT_012ab884;
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    bVar5 = *piVar3 == *piVar4;
    piVar3 = piVar3 + 1;
    piVar4 = piVar4 + 1;
  } while (bVar5);
  if (bVar5) {
    iVar2 = 4;
    bVar5 = true;
    piVar3 = param_1 + 4;
    piVar4 = &DAT_012ab604;
    do {
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      bVar5 = *piVar3 == *piVar4;
      piVar3 = piVar3 + 1;
      piVar4 = piVar4 + 1;
    } while (bVar5);
    if (bVar5) {
      uVar1 = 0;
    }
  }
  return uVar1;
}


// CALLEES of CTextureRenderer::CheckMediaType:

//=== CTextureRenderer::SetMediaType @ 00a3b5f0 (seed 00a3b5f0) ===

int CTextureRenderer__SetMediaType(int param_1)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  undefined4 *puVar4;
  int in_ECX;
  uint uVar5;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined1 auStack_40 [20];
  int iStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined1 auStack_20 [32];
  
  iVar3 = *(int *)(param_1 + 0x44);
  *(undefined4 *)(in_ECX + 0x16c) = *(undefined4 *)(iVar3 + 0x34);
  uVar1 = *(uint *)(iVar3 + 0x38);
  uVar5 = (int)uVar1 >> 0x1f;
  iVar3 = (uVar1 ^ uVar5) - uVar5;
  *(int *)(in_ECX + 0x170) = iVar3;
  *(int *)(*(int *)(in_ECX + 0x160) + 0x84) = iVar3;
  *(undefined4 *)(*(int *)(in_ECX + 0x160) + 0x88) = *(undefined4 *)(in_ECX + 0x16c);
  *(uint *)(in_ECX + 0x174) = (*(int *)(in_ECX + 0x16c) + 1) * 3 & 0xfffffffc;
  if (*(undefined4 **)(in_ECX + 0x164) == (undefined4 *)0x0) {
    puVar4 = operator_new(8);
    if (puVar4 == (undefined4 *)0x0) {
      puVar4 = (undefined4 *)0x0;
    }
    else {
      *puVar4 = 0;
      puVar4[1] = 0;
    }
    *(undefined4 **)(in_ECX + 0x164) = puVar4;
    CPixelFormat::Initialise((CPixelFormat *)&param_1,8,8,8,8,false);
    uStack_48 = *(undefined4 *)(in_ECX + 0x16c);
    uStack_44 = *(undefined4 *)(in_ECX + 0x170);
    CTexture::Initialise
              (*(CTexture **)(in_ECX + 0x164),(C2DExtentsI *)&uStack_48,1,(CPixelFormat *)&param_1,0
               ,1,false,false);
  }
  else {
    piVar2 = (int *)**(undefined4 **)(in_ECX + 0x164);
    (**(code **)(*piVar2 + 0x44))(piVar2,0,auStack_40);
    *(undefined4 *)(in_ECX + 0x16c) = uStack_28;
    *(undefined4 *)(in_ECX + 0x170) = uStack_24;
  }
  iVar3 = (**(code **)(*(int *)**(undefined4 **)(in_ECX + 0x164) + 0x44))
                    ((int *)**(undefined4 **)(in_ECX + 0x164),0,auStack_20);
  if (-1 < iVar3) {
    *(int *)(in_ECX + 0x168) = iStack_2c;
    if ((iStack_2c != 0x15) && (iStack_2c != 0x19)) {
      return -0x7ffbfdd6;
    }
    iVar3 = 0;
  }
  return iVar3;
}


// CALLEES of CTextureRenderer::SetMediaType:
//   009fa280  Initialise
//   00bfea1a  operator_new
//   009e3790  Initialise

//=== CTextureRenderer::DoRenderSample @ 00a3b730 (seed 00a3b730) ===

/* autoname-t5 [medium]: Enters critical section, locks CTexture, handles format 0x15 (RGB->RGBA
   conversion) and 0x19 (RGB->565 conversion), unlocks — texture format conversion worker. */

undefined4 CTextureRenderer__DoRenderSample(int *param_1)

{
  int in_ECX;
  byte *pbVar1;
  int iVar2;
  int iVar3;
  byte *unaff_EBP;
  byte *pbVar4;
  byte *local_1c;
  undefined1 auStack_18 [8];
  int iStack_10;
  byte *pbStack_c;
  int iStack_4;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(*(int *)(in_ECX + 0x160) + 100));
  (**(code **)(*param_1 + 0xc))(param_1,&local_1c);
  CTexture::Lock(*(CTexture **)(in_ECX + 0x164),(ulong)auStack_18,0);
  if ((*(int *)(in_ECX + 0x168) == 0x15) && (iStack_4 = 0, 0 < *(int *)(in_ECX + 0x170))) {
    iVar3 = *(int *)(in_ECX + 0x16c);
    do {
      iVar2 = 0;
      pbVar1 = pbStack_c;
      pbVar4 = unaff_EBP;
      if (0 < iVar3) {
        do {
          *pbVar1 = *pbVar4;
          pbVar1[1] = pbVar4[1];
          pbVar1[2] = pbVar4[2];
          pbVar1[3] = 0xff;
          iVar3 = *(int *)(in_ECX + 0x16c);
          iVar2 = iVar2 + 1;
          pbVar1 = pbVar1 + 4;
          pbVar4 = pbVar4 + 3;
        } while (iVar2 < iVar3);
      }
      unaff_EBP = unaff_EBP + *(int *)(in_ECX + 0x174);
      pbStack_c = pbStack_c + iStack_10;
      iStack_4 = iStack_4 + 1;
    } while (iStack_4 < *(int *)(in_ECX + 0x170));
  }
  if ((*(int *)(in_ECX + 0x168) == 0x19) && (iStack_4 = 0, 0 < *(int *)(in_ECX + 0x170))) {
    iVar3 = *(int *)(in_ECX + 0x16c);
    do {
      local_1c = pbStack_c;
      iVar2 = 0;
      pbVar1 = local_1c;
      pbVar4 = unaff_EBP;
      if (0 < iVar3) {
        do {
          *(ushort *)pbVar1 =
               (ushort)(*pbVar4 >> 3) +
               (((pbVar4[2] & 0xf8) + 0x100) * 0x20 + (ushort)(pbVar4[1] & 0xf8)) * 4;
          iVar3 = *(int *)(in_ECX + 0x16c);
          iVar2 = iVar2 + 1;
          pbVar1 = pbVar1 + 2;
          pbVar4 = pbVar4 + 3;
        } while (iVar2 < iVar3);
      }
      unaff_EBP = unaff_EBP + *(int *)(in_ECX + 0x174);
      iStack_4 = iStack_4 + 1;
      pbStack_c = local_1c + iStack_10;
    } while (iStack_4 < *(int *)(in_ECX + 0x170));
  }
  CTexture::Unlock(*(CTexture **)(in_ECX + 0x164),0);
  *(undefined4 *)(*(int *)(in_ECX + 0x160) + 0x80) = *(undefined4 *)(in_ECX + 0x164);
  SetEvent(*(HANDLE *)(*(int *)(in_ECX + 0x160) + 0x7c));
  LeaveCriticalSection((LPCRITICAL_SECTION)(*(int *)(in_ECX + 0x160) + 100));
  return 0;
}


// CALLEES of CTextureRenderer::DoRenderSample:
//   009f9de0  Unlock
//   EXTERNAL:000000b5  LeaveCriticalSection
//   009fa450  Lock
//   EXTERNAL:000000b4  EnterCriticalSection
//   EXTERNAL:000000a9  SetEvent

//=== CVideoSys::SetVolume @ 00a3b920 (seed 00a3b920) ===

/* [bsim sim=0.7890180491410433 <- ego_r]
   public: void __thiscall CVideoSys::SetVolume(float) */

void __thiscall CVideoSys::CVideoSys__SetVolume(CVideoSys *this,float param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (*(int *)(this + 0x14) != 0) {
    piVar1 = *(int **)(this + 0x14);
    iVar2 = *piVar1;
    uVar3 = __ftol2();
    (**(code **)(iVar2 + 0x1c))(piVar1,uVar3);
  }
  return;
}


// CALLEES of CVideoSys::SetVolume:
//   00bfea70  __ftol2

//=== CTextureRenderer::~CTextureRenderer @ 00a3b970 (seed 00a3b970) ===

/* [bsim sim=0.5518156585547008 <- ego_r]
   public: virtual __thiscall C3DAnimation2::~C3DAnimation2(void) */

void __thiscall C3DAnimation2::CTextureRenderer___CTextureRenderer(C3DAnimation2 *this)

{
  CTexture *this_00;
  
  this_00 = *(CTexture **)(this + 0x164);
  *(undefined ***)this = &PTR_LAB_0129d08c;
  *(undefined ***)(this + 0xc) = &PTR_FUN_0129d04c;
  *(undefined ***)(this + 0x10) = &PTR_FUN_0129d030;
  *(undefined ***)(this + 0xe0) = &PTR_FUN_0129d008;
  *(undefined ***)(this + 0xe4) = &PTR_FUN_0129cff0;
  if ((this_00 != (CTexture *)0x0) && (this[0x178] != (C3DAnimation2)0x0)) {
    CTexture::Uninitialise(this_00);
    operator_delete(this_00);
  }
  CTCStealth::OnCreate((CTCStealth *)this);
  return;
}


// CALLEES of CTextureRenderer::~CTextureRenderer:
//   009f9f70  Uninitialise
//   00ca7310  OnCreate
//   00bfe9bc  operator_delete

//=== CVideoSys::Init @ 00a3b9d0 (seed 00a3b9d0) ===

/* [bsim sim=0.6883608928424445 <- ego_r]
   public: long __thiscall CVideoSys::Init(class CWideString const &,class CTexture *,bool) */

long __thiscall
CVideoSys::CVideoSys__Init(CVideoSys *this,CWideString *param_1,CTexture *param_2,bool param_3)

{
  CInputManager *this_00;
  bool bVar1;
  ulong uVar2;
  wchar_t *pwVar3;
  long lVar4;
  CTextureRenderer *this_01;
  int iVar5;
  char *pcVar6;
  int iVar7;
  HANDLE pvVar8;
  ulong unaff_EBP;
  CEnginePrimitive *unaff_EDI;
  int *piVar9;
  undefined4 uVar10;
  undefined1 *puVar11;
  int *piVar12;
  char cVar13;
  wchar_t *pwVar14;
  HRESULT aHStack_8 [2];
  
  if (DAT_013d2825 != '\0') {
    return 0;
  }
  uVar2 = CEngineInternalPrimitiveBase::AddChildPrimitive
                    ((CEngineInternalPrimitiveBase *)this,unaff_EDI,unaff_EBP);
  this_00 = *(CInputManager **)(uVar2 + 0x58);
  (**(code **)(*(int *)this_00 + 8))();
  CInputManager::ClearEvents(this_00);
  DAT_013d2826 = 0;
  *(undefined4 *)(this + 0x34) = *(undefined4 *)(this + 0x60);
  CMovieSubtitles::ParseSubtitles((CMovieSubtitles *)(this + 0x30),param_1);
  pwVar14 = L".wmv";
  bVar1 = false;
  pwVar3 = (wchar_t *)CCharString::operator_char_const_((CCharString *)param_1);
  pwVar3 = wcsstr(pwVar3,pwVar14);
  if (pwVar3 == (wchar_t *)0x0) {
    pwVar14 = L".asf";
    pwVar3 = (wchar_t *)CCharString::operator_char_const_((CCharString *)param_1);
    pwVar3 = wcsstr(pwVar3,pwVar14);
    if (pwVar3 == (wchar_t *)0x0) goto LAB_00a3ba51;
  }
  bVar1 = true;
LAB_00a3ba51:
  lVar4 = CoCreateInstance((IID *)&DAT_012ab174,(LPUNKNOWN)0x0,1,(IID *)&DAT_012a9934,(LPVOID *)this
                          );
  if (-1 < lVar4) {
    aHStack_8[0] = lVar4;
    if (*(int *)(this + 0x20) == 0) {
      this_01 = ::operator_new(0x180);
      if (this_01 == (CTextureRenderer *)0x0) {
        iVar5 = 0;
      }
      else {
        iVar5 = CTextureRenderer::CTextureRenderer__CTextureRenderer
                          (this_01,(IUnknown *)0x0,aHStack_8,param_2);
      }
      *(int *)(this + 0x20) = iVar5;
      *(CVideoSys **)(iVar5 + 0x160) = this;
      (**(code **)(*(int *)(*(int *)(this + 0x20) + 0xc) + 4))();
    }
    if (*(int *)(this + 0x20) == 0) {
      piVar9 = (int *)0x0;
    }
    else {
      piVar9 = (int *)(*(int *)(this + 0x20) + 0xc);
    }
    cVar13 = -0x54;
    piVar12 = piVar9;
    lVar4 = (**(code **)(**(int **)this + 0xc))(*(int **)this);
    if (-1 < lVar4) {
      if (bVar1) {
        iVar5 = **(int **)this;
        uVar10 = 0;
        pcVar6 = CCharString::operator_char_const_((CCharString *)param_1);
        lVar4 = (**(code **)(iVar5 + 0x34))(*(undefined4 *)this,pcVar6,uVar10);
        if (lVar4 < 0) {
          return lVar4;
        }
      }
      else {
        iVar5 = **(int **)this;
        puVar11 = &stack0xfffffff0;
        pwVar3 = L"SOURCE";
        pcVar6 = CCharString::operator_char_const_((CCharString *)param_1);
        iVar5 = (**(code **)(iVar5 + 0x38))(*(undefined4 *)this,pcVar6,pwVar3);
        if (iVar5 < 0) {
          return iVar5;
        }
        iVar7 = (**(code **)(*piVar12 + 0x2c))(piVar12,L"Output",&stack0xffffffe8);
        if (iVar7 < 0) {
          return iVar7;
        }
        if (cVar13 == '\0') {
          iVar5 = (**(code **)(*piVar9 + 0x2c))(piVar9,&DAT_0129d184,&stack0xffffffe4);
          if (iVar5 < 0) {
            return iVar5;
          }
          iVar5 = (**(code **)(**(int **)this + 0x2c))(*(int **)this,iVar7,puVar11);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
        else {
          (**(code **)(**(int **)this + 0x30))(*(int **)this,iVar5);
        }
      }
      (**(code **)**(undefined4 **)this)(*(undefined4 **)this,&DAT_012aa094,this + 4);
      (**(code **)**(undefined4 **)this)(*(undefined4 **)this,&DAT_012aa064,this + 8);
      (**(code **)**(undefined4 **)this)(*(undefined4 **)this,&DAT_012a9a04,this + 0xc);
      (**(code **)**(undefined4 **)this)(*(undefined4 **)this,&DAT_012aa084,this + 0x10);
      (**(code **)**(undefined4 **)this)(*(undefined4 **)this,&DAT_012aa054,this + 0x14);
      *(undefined4 *)(this + 0x28) = 3;
      InitializeCriticalSection((LPCRITICAL_SECTION)(this + 100));
      pvVar8 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,0,0,(LPCWSTR)0x0);
      *(HANDLE *)(this + 0x7c) = pvVar8;
      lVar4 = 0;
    }
  }
  return lVar4;
}


// CALLEES of CVideoSys::Init:
//   0099af00  operator_char_const*
//   00bfeb2e  wcsstr
//   00bfea1a  operator_new
//   0099af10  operator_char_const*
//   009f5540  ClearEvents
//   EXTERNAL:0000019f  CoCreateInstance
//   009a4ec0  AddChildPrimitive
//   009bd460  ParseSubtitles
//   EXTERNAL:000000ea  InitializeCriticalSection
//   EXTERNAL:000000a3  CreateEventW
//   00a3b510  CTextureRenderer::CTextureRenderer

//=== CVideoSys::~CVideoSys @ 00a3bc20 (seed 00a3bc20) ===

/* [bsim sim=0.5960711499401491 <- ego_r]
   private: void __thiscall CEngineLandscapePatch::DeleteLayerMeshes(void) */

void __thiscall CEngineLandscapePatch::CVideoSys___CVideoSys(CEngineLandscapePatch *this)

{
  if (*(int **)(this + 0x20) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x20) + 0xc))(1);
    *(undefined4 *)(this + 0x20) = 0;
  }
  CoUninitialize();
  CCharString::~CCharString((CCharString *)(this + 0x44));
  list_node_destructor();
  if (*(void **)(this + 0x30) != (void *)0x0) {
    free(*(void **)(this + 0x30));
  }
  return;
}


// CALLEES of CVideoSys::~CVideoSys:
//   EXTERNAL:000001a1  CoUninitialize
//   00bfea14  free
//   0099b510  ~CCharString
//   009be060  list_node_destructor

//=== CVideoSys::CVideoSys @ 00a3bc70 (seed 00a3bc70) ===

/* [bsim sim=1.0 <- ego_r]
   public: __thiscall CVideoSys::CVideoSys(void) */

CVideoSys * __thiscall CVideoSys::CVideoSys__CVideoSys(CVideoSys *this)

{
  CThing *unaff_EDI;
  
  *(undefined4 *)(this + 0x18) = 0;
  *(undefined4 *)(this + 0x1c) = 0;
  CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator
            ((CTCCreatureGeneratorCreator *)(this + 0x30),unaff_EDI);
  this[0x5c] = (CVideoSys)0x1;
  *(undefined4 *)(this + 0x60) = 0;
  *(undefined4 *)(this + 0x7c) = 0;
  *(undefined4 *)(this + 0x84) = 0;
  *(undefined4 *)(this + 0x88) = 0;
  *(undefined4 *)this = 0;
  *(undefined4 *)(this + 4) = 0;
  *(undefined4 *)(this + 8) = 0;
  *(undefined4 *)(this + 0xc) = 0;
  *(undefined4 *)(this + 0x10) = 0;
  *(undefined4 *)(this + 0x14) = 0;
  *(undefined4 *)(this + 0x20) = 0;
  *(undefined4 *)(this + 0x24) = 1;
  *(undefined4 *)(this + 0x28) = 0;
  this[0x2c] = (CVideoSys)0x1;
  CoInitialize((LPVOID)0x0);
  return this;
}


// CALLEES of CVideoSys::CVideoSys:
//   009bdba0  CTCCreatureGeneratorCreator
//   EXTERNAL:000001a0  CoInitialize

