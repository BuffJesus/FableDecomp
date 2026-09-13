//=== SetText @ 00cbe87f (seed 0x00CBE87F) ===

/* [bsim sim=0.5079268553761639 <- ego_r]
   public: void __thiscall CSubtitleRenderer::SetText(class CFontBank const &,class CCharString
   const &,class CCharString const &,class CCharString const &,class CCharString const &,unsigned
   long,float,bool,bool) */

void __thiscall
CSubtitleRenderer::SetText
          (CSubtitleRenderer *this,CFontBank *param_1,CCharString *param_2,CCharString *param_3,
          CCharString *param_4,CCharString *param_5,ulong param_6,float param_7,bool param_8,
          bool param_9)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_ESI;
  undefined *puVar5;
  undefined4 uVar6;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_1c [4];
  CCharString local_18 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_14 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_10 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_8 [4];
  
  puVar5 = &DAT_01244db4;
  uVar2 = GFIntToCharString_API(&DAT_01244db4);
  CCharString::CCharString(local_18,"TEXT_QST_LOG_STORY_",-1);
  CCharString__AppendData(uVar2);
  CCharString__AppendCString(puVar5);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_14,extraout_EDX,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_18,
             extraout_EDX_00,unaff_ESI);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_1c,extraout_EDX_01,unaff_ESI);
  CCharString__AppendCString(&DAT_012c174c);
  CCharString__AppendCString(&DAT_012c1744);
  iVar1 = *DAT_0143e8f8;
  uVar6 = 1;
  uVar2 = CPersistContext::GetDefaultVal<CAnimationSet>();
  uVar3 = CPersistContext::GetDefaultVal<CAnimationSet>();
  ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
           CPersistContext::GetDefaultVal<CAnimationSet>();
  (**(code **)(iVar1 + 0x4d0))(ppVar4,uVar3,uVar2,uVar6);
  CCharString::~CCharString((CCharString *)local_1c);
  CCharString::~CCharString(local_18);
  CCharString::~CCharString((CCharString *)local_14);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_10,extraout_EDX_02,ppVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_8,extraout_EDX_03,ppVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_c,extraout_EDX_04,ppVar4);
  return;
}


// CALLEES of SetText:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099e480  GetDefaultVal<CAnimationSet>
//   0099f570  CCharString::AppendData
//   0099f600  CCharString::AppendCString
//   0099f830  GFIntToCharString_API
//   0099ebf0  CCharString
//   0099b510  ~CCharString

//=== SetText @ 00cbe960 (seed 0x00CBE960) ===

/* [bsim sim=0.6522511769220943 <- ego_r]
   public: void __thiscall CSubtitleRenderer::SetText(class CFontBank const &,class CCharString
   const &,class CCharString const &,class CCharString const &,class CCharString const &,unsigned
   long,float,bool,bool) */

void __thiscall
CSubtitleRenderer::SetText
          (CSubtitleRenderer *this,CFontBank *param_1,CCharString *param_2,CCharString *param_3,
          CCharString *param_4,CCharString *param_5,ulong param_6,float param_7,bool param_8,
          bool param_9)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  undefined4 uVar5;
  CCharString local_18 [4];
  CCharString local_14 [4];
  CCharString local_10 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_8 [4];
  
  CCharString__AppendCString("_NAME");
  CCharString__AppendCString("_DESC");
  iVar1 = *DAT_0143e8f8;
  uVar5 = 1;
  uVar2 = CPersistContext::GetDefaultVal<CAnimationSet>();
  uVar3 = CPersistContext::GetDefaultVal<CAnimationSet>();
  ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
           CPersistContext::GetDefaultVal<CAnimationSet>();
  (**(code **)(iVar1 + 0x4d0))(ppVar4,uVar3,uVar2,uVar5);
  CCharString::~CCharString(local_10);
  CCharString::~CCharString(local_14);
  CCharString::~CCharString(local_18);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_c,extraout_EDX,ppVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_8,extraout_EDX_00,ppVar4);
  return;
}


// CALLEES of SetText:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099e480  GetDefaultVal<CAnimationSet>
//   0099f600  CCharString::AppendCString
//   0099b510  ~CCharString

//=== SetText @ 00cbe9ee (seed 0x00CBE9EE) ===

/* [bsim sim=0.5033094326160077 <- ego_r]
   public: void __thiscall CSubtitleRenderer::SetText(class CFontBank const &,class CCharString
   const &,class CCharString const &,class CCharString const &,class CCharString const &,unsigned
   long,float,bool,bool) */

void __thiscall
CSubtitleRenderer::SetText
          (CSubtitleRenderer *this,CFontBank *param_1,CCharString *param_2,CCharString *param_3,
          CCharString *param_4,CCharString *param_5,ulong param_6,float param_7,bool param_8,
          bool param_9)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  undefined4 uVar5;
  CCharString local_18 [4];
  CCharString local_14 [4];
  CCharString local_10 [4];
  CCharString local_c [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> local_8 [4];
  
  CCharString__AppendCString("_TITLE");
  CCharString::CCharString(local_c,(CCharString *)this);
  iVar1 = *DAT_0143e8f8;
  uVar5 = 2;
  uVar2 = CPersistContext::GetDefaultVal<CAnimationSet>();
  uVar3 = CPersistContext::GetDefaultVal<CAnimationSet>();
  ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
           CPersistContext::GetDefaultVal<CAnimationSet>();
  (**(code **)(iVar1 + 0x4d0))(ppVar4,uVar3,uVar2,uVar5);
  CCharString::~CCharString(local_10);
  CCharString::~CCharString(local_14);
  CCharString::~CCharString(local_18);
  (**(code **)(*DAT_0143e8f8 + 0x1c))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,extraout_EDX,
             ppVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            (local_8,extraout_EDX_00,ppVar4);
  return;
}


// CALLEES of SetText:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099e480  GetDefaultVal<CAnimationSet>
//   0099f600  CCharString::AppendCString
//   0099ec30  CCharString
//   0099b510  ~CCharString

//=== PostAttackStuff @ 00dbeb20 (seed 0x00DBECC1) ===

/* [bsim sim=0.8013377134215132 <- ego_r]
   public: void __thiscall NScript::CQ_NewOakValeIntroScript::PostAttackStuff(void) */

void __thiscall NScript::CQ_NewOakValeIntroScript::PostAttackStuff(CQ_NewOakValeIntroScript *this)

{
  int iVar1;
  char cVar2;
  bool bVar3;
  CCarriedReadableDef CVar4;
  int *piVar5;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar6;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar7;
  CScriptThing *pCVar8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar9;
  CCountedPointer<NUISystem::CComponent> *this_00;
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
  CQ_NewOakValeIntroScript *pCStack_d8;
  undefined1 *puStack_d0;
  undefined4 uStack_c4;
  undefined4 uStack_c0;
  undefined4 uStack_b8;
  CScriptGameResourceObjectScriptedThingBase *pCVar10;
  undefined1 *puStack_98;
  float fVar11;
  CCharString *pCVar12;
  CCharString *pCVar13;
  CCharString *pCVar14;
  ulong uVar15;
  undefined1 uVar16;
  undefined4 local_3c;
  int *local_38 [14];
  
  while( true ) {
    CCharString::CCharString((CCharString *)&local_3c,"M_PostAttackStart",-1);
    ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_38;
    piVar5 = (int *)(**(code **)(**(int **)(this + 0x40) + 0x120))();
    cVar2 = (**(code **)(*piVar5 + 300))();
    if ((local_38[0] != (int *)0x0) && (*local_38[0] = *local_38[0] + -1, *local_38[0] == 0)) {
      (*(code *)local_38[0][1])();
      operator_delete(local_38[0]);
    }
    local_3c = 0;
    local_38[0] = (int *)0x0;
    NHeroInformationScreens::CBase::CBase((CBase *)&stack0xffffffc0);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffbc,
               extraout_EDX,ppVar9);
    if (cVar2 != '\0') break;
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar3) {
      return;
    }
  }
  bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  if (!bVar3) {
    (**(code **)(**(int **)(this + 0x40) + 0xae8))();
    CCharString::CCharString((CCharString *)&stack0xffffffb8,"M_PostAttackStart",-1);
    piVar5 = *(int **)(this + 0x40);
    iVar1 = *piVar5;
    uVar16 = SUB41(&stack0xffffffbc,0);
    CVar4 = (CCarriedReadableDef)(**(code **)(iVar1 + 0x120))();
    ppVar6 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
             (**(code **)(*piVar5 + 0x118))();
    (**(code **)(iVar1 + 0x760))();
    C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&stack0xffffffa8);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffa4,
               extraout_EDX_00,ppVar6);
    CCharString::CCharString((CCharString *)&stack0xffffffa4,"V_OakVale",-1);
    iVar1 = **(int **)(this + 0x40);
    uVar15 = 1;
    pCVar14 = (CCharString *)&stack0xffffffa4;
    pCVar13 = (CCharString *)&stack0xffffffa8;
    ppVar7 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
             (**(code **)(iVar1 + 0x120))();
    (**(code **)(iVar1 + 0x6e0))();
    C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&stack0xffffff98);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff94,
               extraout_EDX_01,ppVar7);
    pCVar12 = (CCharString *)0x0;
    (**(code **)(**(int **)(this + 0x40) + 0x554))();
    CCharString::CCharString((CCharString *)&stack0xffffff90,"OBJECT_TEDDY_BEAR_UNGIVEABLE",-1);
    ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xffffff90;
    (**(code **)(**(int **)(this + 0x40) + 500))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff8c,
               extraout_EDX_02,ppVar9);
    CSubtitleRenderer::SetText
              ((CSubtitleRenderer *)&DAT_00000014,(CFontBank *)ppVar9,pCVar12,(CCharString *)ppVar7,
               pCVar13,pCVar14,uVar15,(float)ppVar6,(bool)CVar4,(bool)uVar16);
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (!bVar3) {
      (**(code **)(**(int **)(this + 0x40) + 0x684))();
      (**(code **)(**(int **)(this + 0x40) + 0x680))();
      (**(code **)(**(int **)(this + 0x40) + 0xae8))();
      (**(code **)(**(int **)(this + 0x40) + 0x5d8))();
      CCharString::CCharString((CCharString *)&stack0xffffff84,"MK_OVI_DADTRIGGER",-1);
      ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xffffff94;
      (**(code **)(**(int **)(this + 0x40) + 0x120))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff7c,
                 extraout_EDX_03,ppVar9);
      fVar11 = 5.0;
      puStack_98 = (undefined1 *)0xdbed4b;
      pCVar8 = (CScriptThing *)(**(code **)(**(int **)(this + 0x40) + 0x118))();
      puStack_98 = (undefined1 *)0xdbed56;
      bVar3 = IsDistanceBetweenThingsUnder(pCVar8,(CScriptThing *)&stack0xffffff8c,fVar11);
      while (!bVar3) {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) goto LAB_00dbef57;
        fVar11 = 5.0;
        puStack_98 = (undefined1 *)0xdbed87;
        pCVar8 = (CScriptThing *)(**(code **)(**(int **)(this + 0x40) + 0x118))();
        puStack_98 = (undefined1 *)0xdbed92;
        bVar3 = IsDistanceBetweenThingsUnder(pCVar8,(CScriptThing *)&stack0xffffff8c,fVar11);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (!bVar3) {
        this[0x51] = (CQ_NewOakValeIntroScript)0x1;
        CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&stack0xffffff98);
        iVar1 = **(int **)(this + 0x40);
        puStack_98 = &stack0xffffff98;
        ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                 (**(code **)(iVar1 + 0x118))();
        (**(code **)(iVar1 + 0x20))();
        StdMap_Construct_API();
        CCharString::CCharString((CCharString *)&stack0xffffff70,"HERO",-1);
        pCVar10 = (CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff8c;
        this_00 = std::
                  map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_>
                  ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_>
                                *)&stack0xffffff74,(CCharString *)&stack0xffffff70);
        CScriptGameResourceObjectScriptedThingBase::operator=
                  ((CScriptGameResourceObjectScriptedThingBase *)this_00,pCVar10);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff70
                   ,extraout_EDX_04,ppVar9);
        CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&stack0xffffff9c);
        CCharString::CCharString((CCharString *)&stack0xffffff70,"",-1);
        ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xffffff70;
        (**(code **)(**(int **)(this + 0x40) + 0x5c8))();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&puStack_98,
                   extraout_EDX_05,ppVar9);
        piVar5 = *(int **)(this + 0x40);
        (**(code **)(*piVar5 + 0x5ec))();
        (**(code **)(**(int **)(this + 0x40) + 0x5cc))();
        uStack_b8 = 0xdbee6a;
        CCharString::CCharString((CCharString *)&stack0xffffff60,"CS_OAKVALEINTRO_HESDEADJIM",-1);
        uStack_b8 = 0;
        ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
        uStack_c0 = 0xdbee7f;
        RunCutsceneMacro_Func();
        uStack_c0 = 0xdbee88;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff50
                   ,extraout_EDX_06,ppVar9);
        uStack_c0 = 0;
        uStack_c4 = 0xdbee95;
        (**(code **)(**(int **)(this + 0x40) + 0x5cc))();
        uStack_c4 = 0;
        (**(code **)(*piVar5 + 0x5ec))();
        CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase
                  ((CScriptGameResourceObjectMovieBase *)&stack0xffffff74);
        StdMap_Destroy_API();
        CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase
                  ((CScriptGameResourceObjectMovieBase *)&stack0xffffff64);
        puStack_d0 = (undefined1 *)0xdbeecc;
        CCharString::CCharString((CCharString *)&uStack_b8,"V_OakVale",-1);
        iVar1 = **(int **)(this + 0x40);
        puStack_d0 = &stack0xffffff4c;
        ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
                 (**(code **)(iVar1 + 0x120))();
        pCStack_d8 = (CQ_NewOakValeIntroScript *)0xdbeeed;
        (**(code **)(iVar1 + 0x6e0))();
        pCStack_d8 = (CQ_NewOakValeIntroScript *)0xdbeef6;
        C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&uStack_c4);
        pCStack_d8 = (CQ_NewOakValeIntroScript *)0xdbeeff;
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffff38
                   ,extraout_EDX_07,ppVar9);
        pCStack_d8 = this + 0x4c;
        (**(code **)(**(int **)(this + 0x40) + 0xa20))(0);
        CCharString::CCharString((CCharString *)&puStack_d0,"Q__OakValeIntro_PostAttack",-1);
        ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&puStack_d0;
        (**(code **)(**(int **)(this + 0x40) + 0x460))(ppVar9,0);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_d8,
                   extraout_EDX_08,ppVar9);
        (**(code **)(**(int **)(this + 0x40) + 0xa44))(0);
        (**(code **)(**(int **)(this + 0x40) + 0xae4))(0);
      }
LAB_00dbef57:
      C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&stack0xffffff8c);
    }
  }
  return;
}


// CALLEES of PostAttackStuff:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   004aa840  ~C3DClothPrimitive
//   006e7b60  CCarriedReadableDef
//   008abd10  operator=
//   0099ebf0  CCharString
//   00cbfb7d  RunCutsceneMacro_Func
//   00cdbf70  StdMap_Construct_API
//   00cdbfb0  StdMap_Destroy_API
//   00cb7940  IsActiveThreadTerminating
//   007e72a0  CCarriedReadableDef
//   00bfe9bc  operator_delete
//   007e74d0  ~CScriptGameResourceObjectMovieBase
//   00cd3d2e  operator[]
//   006e7b80  ~CScriptGameResourceObjectMovieBase
//   00cbe87f  SetText
//   00cbe2ff  IsDistanceBetweenThingsUnder
//   0099a2e0  CBase

//=== AddBadDeed @ 00daea70 (seed 0x00DAEA70) ===

/* [bsim sim=0.8691698143337615 <- ego_r]
   public: void __thiscall NScript::CQ_NewOakValeIntroScript::AddBadDeed(enum
   NScript::CQ_NewOakValeIntroScript::EBadDeeds) */

void __thiscall
NScript::CQ_NewOakValeIntroScript::AddBadDeed(CQ_NewOakValeIntroScript *this,EBadDeeds param_1)

{
  bool bVar1;
  char cVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  CCharString *unaff_ESI;
  CCharString *unaff_EDI;
  CCharString *unaff_retaddr;
  CCharString *pCVar3;
  float in_stack_00000008;
  bool in_stack_0000000c;
  bool in_stack_00000010;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  CCharString *pCVar5;
  
  *(int *)(this + 0x58) = *(int *)(this + 0x58) + 1;
  pCVar5 = (CCharString *)-*(float *)(DAT_0143e90c + 0xd64);
  (**(code **)(**(int **)(this + 0x40) + 0x270))();
  if ((*(int *)(this + 0x58) == 1) && (*(int *)(this + 0x54) == 0)) {
    pCVar3 = unaff_retaddr;
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (!bVar1) {
      CCharString::CCharString
                ((CCharString *)&stack0x00000000,"TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED",-1);
      ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)register0x00000010;
      (**(code **)(**(int **)(this + 0x40) + 0x1cc))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffffc,
                 extraout_EDX,ppVar4);
      cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
      while (cVar2 == '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar1) {
          return;
        }
        cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
      }
      bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (!bVar1) {
        CCharString::CCharString((CCharString *)&stack0xfffffffc,"TEXT_QST_LOG_BASICS_MAP",-1);
        CSubtitleRenderer::SetText
                  ((CSubtitleRenderer *)&stack0xfffffffc,(CFontBank *)ppVar4,pCVar5,unaff_EDI,
                   unaff_ESI,pCVar3,param_1,in_stack_00000008,in_stack_0000000c,in_stack_00000010);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffffc
                   ,extraout_EDX_00,ppVar4);
        (unaff_retaddr + 0xfc)[(int)this] = (CCharString)0x1;
        return;
      }
    }
  }
  else {
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (!bVar1) {
      if ((unaff_retaddr + 0xfc)[(int)this] == (CCharString)0x0) {
        bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar1) {
          return;
        }
        CCharString::CCharString
                  ((CCharString *)&stack0x00000000,"TEXT_QST_048_SCRMSG_DID_BAD_DEED",-1);
        ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)register0x00000010;
        (**(code **)(**(int **)(this + 0x40) + 0x1cc))();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffffc
                   ,extraout_EDX_01,ppVar4);
        cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
        while (cVar2 == '\0') {
          (**(code **)(**(int **)(this + 0x40) + 0x1c))();
          bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
          if (bVar1) {
            return;
          }
          cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
        }
        bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar1) {
          return;
        }
      }
      (unaff_retaddr + 0xfc)[(int)this] = (CCharString)0x1;
    }
  }
  return;
}


// CALLEES of AddBadDeed:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00cb7940  IsActiveThreadTerminating
//   00cbe9ee  SetText
//   0099ebf0  CCharString

//=== AddGoodDeed @ 00db0660 (seed 0x00DB0660) ===

/* [bsim sim=0.8954400838025012 <- ego_r]
   public: void __thiscall NScript::CQ_NewOakValeIntroScript::AddGoodDeed(void) */

void __thiscall NScript::CQ_NewOakValeIntroScript::AddGoodDeed(CQ_NewOakValeIntroScript *this)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  CCharString *unaff_ESI;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  undefined1 unaff_retaddr;
  bool in_stack_00000004;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar5;
  CCharString *pCVar6;
  CCharString *in_stack_fffffff0;
  CCharString *in_stack_fffffff4;
  ulong in_stack_fffffff8;
  float in_stack_fffffffc;
  
  *(int *)(this + 0x54) = *(int *)(this + 0x54) + 1;
  pCVar6 = *(CCharString **)(DAT_0143e90c + 0xd64);
  (**(code **)(**(int **)(this + 0x40) + 0x270))();
  if ((*(int *)(this + 0x54) == 1) && (*(int *)(this + 0x58) == 0)) {
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar1) {
      return;
    }
    CCharString::CCharString
              ((CCharString *)&stack0xffffffec,"TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED",-1);
    ppVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xffffffec;
    (**(code **)(**(int **)(this + 0x40) + 0x1cc))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe8,
               extraout_EDX,ppVar5);
    cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
    while (cVar2 == '\0') {
      (**(code **)(**(int **)(this + 0x40) + 0x1c))();
      bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar1) {
        return;
      }
      cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
    }
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar1) {
      return;
    }
    CCharString::CCharString((CCharString *)&stack0xffffffe8,"TEXT_QST_LOG_BASICS_MAP",-1);
    CSubtitleRenderer::SetText
              ((CSubtitleRenderer *)&stack0xffffffe8,(CFontBank *)ppVar5,pCVar6,unaff_ESI,
               in_stack_fffffff0,in_stack_fffffff4,in_stack_fffffff8,in_stack_fffffffc,
               (bool)unaff_retaddr,in_stack_00000004);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe8,
               extraout_EDX_00,ppVar5);
  }
  else {
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar1) {
      return;
    }
    CCharString::CCharString((CCharString *)&stack0xffffffec,"TEXT_QST_048_SCRMSG_DID_GOOD_DEED",-1)
    ;
    ppVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xffffffec;
    (**(code **)(**(int **)(this + 0x40) + 0x1cc))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe8,
               extraout_EDX_01,ppVar5);
    cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
    while (cVar2 == '\0') {
      (**(code **)(**(int **)(this + 0x40) + 0x1c))();
      bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar1) {
        return;
      }
      cVar2 = (**(code **)(**(int **)(this + 0x40) + 0xa0))();
    }
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar1) {
      return;
    }
  }
  iVar3 = (**(code **)(**(int **)(this + 0x40) + 0x1fc))();
  if ((iVar3 < 3) && (this[0x94] == (CQ_NewOakValeIntroScript)0x0)) {
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar1) {
      return;
    }
    CCharString::CCharString((CCharString *)&stack0xfffffff0,"",-1);
    CCharString::CCharString((CCharString *)&stack0xffffffec,"",-1);
    CCharString::CCharString
              ((CCharString *)&stack0xffffffe8,"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02",-1);
    iVar3 = **(int **)(this + 0x40);
    uVar4 = (**(code **)(iVar3 + 0xa3c))
                      (&stack0xfffffff4,&stack0xffffffe8,&stack0xffffffec,&stack0xfffffff0);
    (**(code **)(iVar3 + 0x4a0))(uVar4);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff4,
               extraout_EDX_02,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe8,
               extraout_EDX_03,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffec,
               extraout_EDX_04,unaff_EDI);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff0,
               extraout_EDX_05,unaff_EDI);
  }
  (**(code **)(**(int **)(this + 0x40) + 0x53c))
            (*(undefined4 *)(this + 0x5c),*(undefined4 *)(this + 0x54),0xffffffff);
  return;
}


// CALLEES of AddGoodDeed:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00cb7940  IsActiveThreadTerminating
//   00cbe9ee  SetText
//   0099ebf0  CCharString

