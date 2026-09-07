//=== 00ce9613 : NO FUNCTION ===
//=== 00cecbb9 : NO FUNCTION ===
//=== Main @ 00e1ee30 (seed 00e1ee30) ===

/* [bsim sim=0.5939494846212634 <- ego_r]
   private: virtual void __thiscall NScript::CQR_EscortTraderScript::CAttackingBandit::Main(void) */

void __thiscall NScript::CQR_EscortTraderScript::CAttackingBandit::Main(CAttackingBandit *this)

{
  int iVar1;
  char extraout_AL;
  bool bVar2;
  char cVar3;
  char extraout_AL_00;
  char extraout_AL_01;
  char extraout_AL_02;
  char extraout_AL_03;
  char extraout_AL_04;
  char extraout_AL_05;
  undefined4 uVar4;
  CScriptThing *pCVar5;
  CCharString *pCVar6;
  CScriptThing *pCVar7;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  CCharString *unaff_EBP;
  undefined4 unaff_ESI;
  CCharString *unaff_EDI;
  float fVar8;
  CScriptThing *pCVar9;
  CCharString *pCVar10;
  CCharString *pCVar11;
  CScriptThing CVar12;
  int *in_stack_ffffffcc;
  CScriptGameResourceObjectScriptedThingBase aCStack_2c [12];
  undefined **appuStack_20 [2];
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined **local_10;
  CScriptThing local_c [12];
  
  pCVar7 = (CScriptThing *)(this + 8);
  local_10 = &PTR_LAB_012c32f0;
  CScriptThing::CScriptThing(local_c,pCVar7);
  CWorldMap::AllocateMapThingUID((CWorldMap *)this,(long)&local_10);
  local_10 = &PTR_LAB_012c32f0;
  C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)local_c);
  (**(code **)(**(int **)(this + 4) + 0x1c))();
  CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this);
  if (extraout_AL != '\0') {
    return;
  }
  CBaseIntelligentPointer::CBaseIntelligentPointer((CBaseIntelligentPointer *)appuStack_20);
  appuStack_20[0] = &PTR__scalar_deleting_destructor__0127094c;
  uStack_18 = 0;
  uStack_14 = 0;
  bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_20);
  if (bVar2) {
    CMemoryDataOutputStream::Clear((CMemoryDataOutputStream *)appuStack_20);
  }
  pCVar11 = (CCharString *)0x3;
  pCVar10 = (CCharString *)appuStack_20;
  pCVar9 = pCVar7;
  cVar3 = (**(code **)(**(int **)(this + 4) + 0x20))();
  while (cVar3 == '\0') {
    (**(code **)(**(int **)(this + 4) + 0x1c))();
    CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this);
    if (extraout_AL_00 != '\0') goto LAB_00e1f0b1;
    cVar3 = (**(code **)(**(int **)(this + 4) + 0x20))(pCVar7,aCStack_2c,3);
  }
  CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this);
  if (extraout_AL_01 != '\0') goto LAB_00e1f0b1;
  CCharString::CCharString((CCharString *)&stack0xffffffc0,"TraderToEscort",-1);
  (**(code **)(**(int **)(this + 4) + 0x120))(&stack0xffffffc8,&stack0xffffffc0);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffc0,
             extraout_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
  if ((in_stack_ffffffcc == (int *)0x0) ||
     (cVar3 = (**(code **)(*in_stack_ffffffcc + 300))(), cVar3 == '\0')) {
LAB_00e1f0a1:
    CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this);
  }
  else {
    iVar1 = **(int **)(this + 4);
    uVar4 = (**(code **)(iVar1 + 0x118))();
    (**(code **)(iVar1 + 0x954))(pCVar7,uVar4);
    (**(code **)(**(int **)(this + 4) + 0x954))(pCVar7,&stack0xffffffc0);
    CScriptGameResourceObjectScriptedThingBase::
    _FollowThing_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing__M_N_Z
              (aCStack_2c,(CScriptThing *)&stack0xffffffc8,&DAT_3f800000,(CCharString *)0x1,
               unaff_EBP,(CCharString *)pCVar9,SUB41(pCVar10,0),(bool)SUB41(pCVar11,0));
    if (this[0x1c] == (CAttackingBandit)0x0) {
      CCreatureAction_TrollWhackGroundBase::Initialise((CCreatureAction_TrollWhackGroundBase *)this)
      ;
      if (extraout_AL_02 == '\0') {
        fVar8 = 10.0;
        pCVar5 = (CScriptThing *)(**(code **)(**(int **)(this + 4) + 0x118))();
        bVar2 = IsDistanceBetweenThingsUnder(pCVar7,pCVar5,fVar8);
        CVar12 = SUB41(unaff_ESI,0);
        while (!bVar2) {
          (**(code **)(**(int **)(this + 4) + 0x1c))();
          CCreatureAction_TrollWhackGroundBase::Initialise
                    ((CCreatureAction_TrollWhackGroundBase *)this);
          if (extraout_AL_03 != '\0') goto LAB_00e1f0a8;
          fVar8 = 10.0;
          pCVar5 = (CScriptThing *)(**(code **)(**(int **)(this + 4) + 0x118))();
          bVar2 = IsDistanceBetweenThingsUnder(pCVar7,pCVar5,fVar8);
          CVar12 = SUB41(unaff_ESI,0);
        }
        CCreatureAction_TrollWhackGroundBase::Initialise
                  ((CCreatureAction_TrollWhackGroundBase *)this);
        if (extraout_AL_04 == '\0') {
          CCharString::CCharString
                    ((CCharString *)&stack0xffffffc0,"SND_MAN_05_NONSPEECH_AMBUSHLAUGH_0",-1);
          pCVar6 = (CCharString *)GFIntToCharString_API();
          CCharString::operator+=((CCharString *)&stack0xffffffc0,pCVar6);
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                     &stack0xffffffc4,extraout_EDX_00,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
          (**(code **)(**(int **)(this + 4) + 0xac8))(pCVar7,&stack0xffffffc0);
          this[0x1c] = (CAttackingBandit)0x1;
          pCVar7 = (CScriptThing *)(**(code **)(**(int **)(this + 4) + 0x118))();
          CScriptGameResourceObjectScriptedThingBase::
          _StopFollowingThing_CScriptGameResourceObjectScriptedThingBase__UAEXABVCScriptThing___Z
                    (aCStack_2c,pCVar7,(char *)pCVar9,pCVar10,pCVar11,unaff_EDI,(bool)CVar12,
                     SUB41(in_stack_ffffffcc,0));
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)
                     &stack0xffffffc0,extraout_EDX_01,
                     (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
          goto LAB_00e1f078;
        }
      }
    }
    else {
LAB_00e1f078:
      do {
        if ((in_stack_ffffffcc == (int *)0x0) ||
           (cVar3 = (**(code **)(*in_stack_ffffffcc + 300))(), cVar3 == '\0')) goto LAB_00e1f0a1;
        (**(code **)(**(int **)(this + 4) + 0x1c))();
        CCreatureAction_TrollWhackGroundBase::Initialise
                  ((CCreatureAction_TrollWhackGroundBase *)this);
      } while (extraout_AL_05 == '\0');
    }
  }
LAB_00e1f0a8:
  C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&stack0xffffffc8);
LAB_00e1f0b1:
  CScriptGameResourceObjectMovieBase::~CScriptGameResourceObjectMovieBase
            ((CScriptGameResourceObjectMovieBase *)aCStack_2c);
  return;
}


// CALLEES of Main:
//   00cd23b9  HasPhysicsMesh
//   0099a380  CBaseIntelligentPointer
//   00f35b30  Initialise
//   004abe90  CScriptThing
//   0099ebf0  CCharString
//   004aa840  ~C3DClothPrimitive
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00f35b10  AllocateMapThingUID
//   0099f0a0  operator+=
//   007e7320  ?FollowThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@M_N@Z
//   00cd2770  Clear
//   007e7330  ?StopFollowingThing@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@@Z
//   0099f830  GFIntToCharString_API
//   00cbe2ff  IsDistanceBetweenThingsUnder
//   007e74d0  ~CScriptGameResourceObjectMovieBase

//=== 00e1f1bf : NO FUNCTION ===
//=== Init @ 00e1f3e0 (seed 00e1f3e0) ===

/* [bsim sim=0.6680735802522798 <- ego_r]
   private: virtual void __thiscall NScript::CQR_EscortTraderScript::CAttackingBandit::Init(void) */

void __thiscall NScript::CQR_EscortTraderScript::CAttackingBandit::Init(CAttackingBandit *this)

{
  undefined4 uVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  CSpawnedFunc<NScript::CExpression_FollowScript> *this_00;
  undefined4 uVar5;
  CCharString *pCVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar7;
  CAttackingBandit *pCVar8;
  CCharString *pCVar9;
  CCharString local_1c [4];
  CCharString aCStack_18 [4];
  undefined **ppuStack_14;
  undefined4 local_10;
  int *piStack_c;
  
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  local_10 = 0;
  this[0x1c] = (CAttackingBandit)0x0;
  CCharString::CCharString(local_1c,"QR_EscortTrader",-1);
  pCVar9 = local_1c;
  pCVar8 = this + 8;
  (**(code **)(**(int **)(this + 4) + 0x744))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffdc,
             extraout_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
  this_00 = ::operator_new(0x48);
  if (this_00 == (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0) {
    this_00 = (CSpawnedFunc<NScript::CExpression_FollowScript> *)0x0;
  }
  else {
    local_10 = *(undefined4 *)(this + 0xc);
    piStack_c = *(int **)(this + 0x10);
    bVar4 = true;
    bVar3 = true;
    bVar2 = true;
    ppuStack_14 = &PTR__scalar_deleting_destructor__01238c8c;
    if (piStack_c != (int *)0x0) {
      *piStack_c = *piStack_c + 1;
    }
    uVar1 = *(undefined4 *)(this + 0x14);
    uVar5 = CCharString::CCharString(aCStack_18,"WatchForBanditDead",-1);
    CCharString::CCharString(local_1c,"ParentClass.",-1);
    pCVar6 = (CCharString *)CCharString__AppendData(uVar5);
    CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>
              (this_00,pCVar6,0,(CExpression_FollowScript *)pCVar8,(_func_void *)pCVar9);
    *(undefined ***)this_00 = &PTR__scalar_deleting_destructor__012e15d0;
    *(undefined1 **)(this_00 + 0x34) = &LAB_00e1f520;
    *(undefined4 *)(this_00 + 0x38) = uVar1;
    CScriptThing::CScriptThing((CScriptThing *)(this_00 + 0x3c),(CScriptThing *)&ppuStack_14);
    C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&ppuStack_14);
  }
  CCharString::CCharString((CCharString *)&stack0xffffffdc,"",-1);
  CGuiVarTransferStruct::Add
            (*(CGuiVarTransferStruct **)(this + 0x14),(CGuiVarTransferBase *)this_00);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffdc,
             extraout_EDX_00,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
  ppVar7 = extraout_EDX_01;
  if (bVar2) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe0,
               extraout_EDX_01,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    ppVar7 = extraout_EDX_02;
  }
  if (bVar3) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_1c,ppVar7,
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
    ppVar7 = extraout_EDX_03;
  }
  if (bVar4) {
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_18,ppVar7,
               (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
  }
  return;
}


// CALLEES of Init:
//   0099f570  CCharString::AppendData
//   004aa840  ~C3DClothPrimitive
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00cdd450  CSpawnedFunc<NScript::CExpression_FollowScript>
//   00bfea1a  operator_new
//   0099ebf0  CCharString
//   004abe90  CScriptThing
//   00cb7e50  Add

//=== CScriptGameResourceObjectScriptedThingBase_HandleTraderEscortQuestEnd @ 00e1e530 (seed 00e1e530) ===

/* autoname-t5 [medium]: Calls EndMission, CCharString constructor; references TraderToEscort
   entity; implements quest completion for escort mission. */

void CScriptGameResourceObjectScriptedThingBase_HandleTraderEscortQuestEnd(void)

{
  CScriptBase CVar1;
  int iVar2;
  bool bVar3;
  char cVar4;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar5;
  CScriptBase *in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar6;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  undefined4 uStack_2c;
  undefined4 uStack_1c;
  int *piStack_14;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> apStack_10 [16];
  
  CVar1 = in_ECX[0x49];
  while ((CVar1 == (CScriptBase)0x0 && (in_ECX[0x48] == (CScriptBase)0x0))) {
    uStack_1c = 0xe1e54f;
    (**(code **)(**(int **)(in_ECX + 0x40) + 0x1c))();
    uStack_1c = 0xe1e556;
    bVar3 = CScriptBase::IsActiveThreadTerminating(in_ECX);
    if (bVar3) {
      return;
    }
    CVar1 = in_ECX[0x49];
  }
  uStack_1c = 0xe1e56c;
  bVar3 = CScriptBase::IsActiveThreadTerminating(in_ECX);
  if (!bVar3) {
    uStack_1c = 0xe1e57b;
    NScript::CQR_EscortTraderScript::EndMission((CQR_EscortTraderScript *)in_ECX);
    uStack_1c = 0;
    (**(code **)(**(int **)(in_ECX + 0x40) + 0x504))();
    CCharString::CCharString((CCharString *)&piStack_14,"TraderToEscort",-1);
    ppVar5 = apStack_10;
    (**(code **)(**(int **)(in_ECX + 0x40) + 0x120))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_1c,
               extraout_EDX,ppVar5);
    CCharString::CCharString((CCharString *)&stack0xffffffe0);
    CVar1 = in_ECX[0x4b];
    while (((CVar1 == (CScriptBase)0x0 && (piStack_14 != (int *)0x0)) &&
           (cVar4 = (**(code **)(*piStack_14 + 300))(), cVar4 != '\0'))) {
      uStack_2c = 0xe1e5e7;
      cVar4 = (**(code **)(**(int **)(in_ECX + 0x40) + 0x58))();
      if (cVar4 != '\0') break;
      (**(code **)(**(int **)(in_ECX + 0x40) + 0x1c))();
      bVar3 = CScriptBase::IsActiveThreadTerminating(in_ECX);
      ppVar6 = extraout_EDX_00;
      if (bVar3) goto LAB_00e1e610;
      CVar1 = in_ECX[0x4b];
    }
    bVar3 = CScriptBase::IsActiveThreadTerminating(in_ECX);
    ppVar6 = extraout_EDX_01;
    if (bVar3) {
LAB_00e1e610:
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe0,
                 ppVar6,ppVar5);
      C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&stack0xffffffe8);
      return;
    }
    iVar2 = **(int **)(in_ECX + 0x40);
    uStack_2c = 0;
    ppVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
             (**(code **)(**(int **)(in_ECX + 0x40) + 0xa3c))(&uStack_1c);
    (**(code **)(iVar2 + 0x464))();
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffd8,
               extraout_EDX_02,ppVar5);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_2c,
               extraout_EDX_03,ppVar5);
    C3DClothPrimitive::~C3DClothPrimitive((C3DClothPrimitive *)&stack0xffffffdc);
  }
  return;
}


// CALLEES of CScriptGameResourceObjectScriptedThingBase_HandleTraderEscortQuestEnd:
//   004aa840  ~C3DClothPrimitive
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00e1e670  EndMission
//   0099e4b0  CCharString
//   0099ebf0  CCharString
//   00cb7940  IsActiveThreadTerminating

//=== EndMission @ 00e1e670 (seed 00e1e670) ===

/* [bsim sim=0.7278717850589695 <- ego_r]
   public: void __thiscall NScript::CQR_EscortTraderScript::EndMission(void) */

void __thiscall NScript::CQR_EscortTraderScript::EndMission(CQR_EscortTraderScript *this)

{
  int iVar1;
  bool bVar2;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined *puStack_28;
  CCharString *pCVar4;
  CCharString local_8 [4];
  CCharString local_4 [4];
  
  CCharString::CCharString(local_4,"OrchardFarm",-1);
  CCharString::CCharString(local_8,"GreatwoodEntrance",-1);
  pCVar4 = local_8;
  (**(code **)(**(int **)(this + 0x40) + 0x588))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffec,
             extraout_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar4);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff0,
             extraout_EDX_00,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar4);
  iVar1 = **(int **)(this + 0x40);
  ppVar3 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)(**(code **)(iVar1 + 0xa3c))()
  ;
  puStack_28 = (undefined *)0xe1e6da;
  (**(code **)(iVar1 + 0x498))();
  puStack_28 = (undefined *)0xe1e6e3;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffe8,
             extraout_EDX_01,ppVar3);
  if (this[0x49] == (CQR_EscortTraderScript)0x0) {
    puStack_28 = (undefined *)0xe1e6f1;
    bVar2 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (!bVar2) {
      puStack_28 = *(undefined **)(DAT_0143e90c + 0x9bc);
      puStack_2c = (undefined1 *)0xe1e710;
      (**(code **)(**(int **)(this + 0x40) + 0x284))();
      puStack_2c = (undefined1 *)0x0;
      uStack_30 = 0;
      this[0x48] = (CQR_EscortTraderScript)0x1;
      iVar1 = **(int **)(this + 0x40);
      ppVar3 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
               (**(code **)(iVar1 + 0xa3c))(&stack0xffffffe4,0);
      (**(code **)(iVar1 + 0x480))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_30,
                 extraout_EDX_02,ppVar3);
      return;
    }
  }
  else {
    puStack_28 = (undefined *)0xe1e748;
    bVar2 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (!bVar2) {
      puStack_28 = &DAT_0122d70c;
      puStack_2c = (undefined1 *)0xe1e75a;
      CCharString__AssignFromWide();
      iVar1 = **(int **)(this + 0x40);
      puStack_28 = (undefined *)0x0;
      puStack_2c = &stack0xffffffe8;
      uStack_30 = 0;
      ppVar3 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)
               (**(code **)(iVar1 + 0xa3c))(&stack0xffffffe4);
      (**(code **)(iVar1 + 0x484))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_30,
                 extraout_EDX_03,ppVar3);
      CCharString::~CCharString((CCharString *)&puStack_2c);
    }
  }
  return;
}


// CALLEES of EndMission:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099b510  ~CCharString
//   0099ebf0  CCharString
//   0099b6b0  CCharString::AssignFromWide
//   00cb7940  IsActiveThreadTerminating

//=== WatchForFailure @ 00e1f2d0 (seed 00e1f2d0) ===

/* [bsim sim=0.8759816088487229 <- ego_r]
   public: void __thiscall NScript::CQR_EscortTraderScript::WatchForFailure(void) */

void __thiscall NScript::CQR_EscortTraderScript::WatchForFailure(CQR_EscortTraderScript *this)

{
  bool bVar1;
  char cVar2;
  int *piVar3;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar4;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_8 [4];
  undefined4 local_4;
  
  local_4 = 0;
  (**(code **)(**(int **)(this + 0x40) + 0x1c))();
  bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  if (!bVar1) {
    while( true ) {
      CCharString::CCharString((CCharString *)&local_4,"",-1);
      piVar3 = (int *)(**(code **)(**(int **)(this + 0x40) + 0x118))();
      ppVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_4;
      cVar2 = (**(code **)(*piVar3 + 0x48))();
      if ((((cVar2 != '\0') || (this[0x49] != (CQR_EscortTraderScript)0x0)) ||
          (this[0x48] != (CQR_EscortTraderScript)0x0)) ||
         ((this[0x4e] != (CQR_EscortTraderScript)0x0 ||
          (bVar1 = true, this[0x4c] != (CQR_EscortTraderScript)0x0)))) {
        bVar1 = false;
      }
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_8,extraout_EDX,ppVar4);
      if (!bVar1) break;
      (**(code **)(**(int **)(this + 0x40) + 0x1c))();
      bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar1) {
        return;
      }
    }
    bVar1 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if ((!bVar1) && (this[0x48] == (CQR_EscortTraderScript)0x0)) {
      this[0x49] = (CQR_EscortTraderScript)0x1;
    }
  }
  return;
}


// CALLEES of WatchForFailure:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099ebf0  CCharString
//   00cb7940  IsActiveThreadTerminating

//=== GetSuccessType @ 00e24100 (seed 00e24100) ===

/* [bsim sim=0.88267261874335 <- ego_r]
   public: enum NScript::CQR_EscortTraderScript::EscortTraderSuccessType __thiscall
   NScript::CQR_EscortTraderScript::GetSuccessType(class CScriptThing const &) */

EscortTraderSuccessType __thiscall
NScript::CQR_EscortTraderScript::GetSuccessType(CQR_EscortTraderScript *this,CScriptThing *param_1)

{
  int iVar1;
  float10 fVar2;
  
  if ((*(int *)(this + 100) == 0) &&
     (fVar2 = (float10)(**(code **)(**(int **)(this + 0x40) + 0x420))(param_1),
     (float10)*(float *)(this + 0x6c) == fVar2)) {
    return 3;
  }
  iVar1 = DAT_0143e90c;
  if ((*(int *)(this + 100) <= *(int *)(DAT_0143e90c + 0x988)) &&
     (fVar2 = (float10)(**(code **)(**(int **)(this + 0x40) + 0x420))(param_1),
     (float10)*(float *)(iVar1 + 0x990) <= fVar2 / (float10)*(float *)(this + 0x6c))) {
    return 2;
  }
  iVar1 = DAT_0143e90c;
  if ((*(int *)(this + 100) <= *(int *)(DAT_0143e90c + 0x98c)) &&
     (fVar2 = (float10)(**(code **)(**(int **)(this + 0x40) + 0x420))(param_1),
     (float10)*(float *)(iVar1 + 0x994) <= fVar2 / (float10)*(float *)(this + 0x6c))) {
    return 1;
  }
  return 0;
}


// CALLEES of GetSuccessType:

//=== Main @ 00e24e10 (seed 00e24e10) ===

/* [bsim sim=0.9228507904199091 <- ego_r]
   public: virtual void __thiscall NScript::CQR_EscortTrader_ManagerScript::Main(void) */

void __thiscall NScript::CQR_EscortTrader_ManagerScript::Main(CQR_EscortTrader_ManagerScript *this)

{
  int iVar1;
  int iVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
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
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar6;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBX;
  undefined8 uVar7;
  CCharString *pCVar8;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar9;
  CCharString aCStack_28 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_24 [4];
  CCharString aCStack_20 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_1c [4];
  CCharString aCStack_18 [4];
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_14 [4];
  uint uStack_10;
  uint uStack_c;
  undefined4 local_8;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_4 [4];
  
  local_8 = 0;
  if (this[0x50] == (CQR_EscortTrader_ManagerScript)0x0) {
    bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar3) {
      return;
    }
    *(undefined4 *)(this + 0x48) = 0;
    *(undefined4 *)(this + 0x54) = 0;
    this[0x50] = (CQR_EscortTrader_ManagerScript)0x1;
  }
  bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  if (bVar3) {
    return;
  }
  do {
    iVar1 = *(int *)(this + 0x54);
    if (iVar1 == 0) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      Activate(this);
      *(undefined4 *)(this + 0x54) = 1;
    }
    else if (iVar1 == 1) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString(aCStack_28,"QR_EscortTrader",-1);
      pCVar8 = aCStack_28;
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffd4,
                 extraout_EDX,(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      while (cVar4 != '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)&stack0xffffffd4,"QR_EscortTrader",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))(&stack0xffffffd4);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xffffffd4
                   ,extraout_EDX_00,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString(aCStack_28,"Q_TraderEscort",-1);
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(aCStack_28);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_24,extraout_EDX_01,unaff_EBX);
      while (cVar4 == '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)aaStack_24,"Q_TraderEscort",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(aaStack_24);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_24,extraout_EDX_02,unaff_EBX);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      *(undefined4 *)(this + 0x54) = 2;
    }
    else if (iVar1 == 2) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      Activate(this);
      *(undefined4 *)(this + 0x54) = 3;
    }
    else if (iVar1 == 3) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString(aCStack_20,"QR_EscortTrader",-1);
      pCVar8 = aCStack_20;
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_24,extraout_EDX_03,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      while (cVar4 != '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)aaStack_24,"QR_EscortTrader",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))(aaStack_24);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_24,extraout_EDX_04,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString(aCStack_20,"Q_BanditCamp",-1);
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(aCStack_20);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_1c,extraout_EDX_05,unaff_EBX);
      while (cVar4 == '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)aaStack_1c,"Q_BanditCamp",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(aaStack_1c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_1c,extraout_EDX_06,unaff_EBX);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      *(undefined4 *)(this + 0x54) = 4;
    }
    else if (iVar1 == 4) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      Activate(this);
      *(undefined4 *)(this + 0x54) = 5;
    }
    else if (iVar1 == 5) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString(aCStack_18,"QR_EscortTrader",-1);
      pCVar8 = aCStack_18;
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_1c,extraout_EDX_07,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      while (cVar4 != '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)aaStack_1c,"QR_EscortTrader",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))(aaStack_1c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_1c,extraout_EDX_08,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString(aCStack_18,"Q_Arena",-1);
      pCVar8 = aCStack_18;
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_1c,extraout_EDX_09,
                 (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      while (cVar4 == '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)aaStack_1c,"Q_Arena",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(aaStack_1c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_1c,extraout_EDX_10,
                   (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar8);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString((CCharString *)&uStack_c,"Q_OrchardFarm_Barricade",-1);
      (**(code **)(**(int **)(this + 0x40) + 0x450))(&uStack_c);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_4,extraout_EDX_11,unaff_EBX);
      *(undefined4 *)(this + 0x54) = 6;
    }
    else if (iVar1 == 6) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      Activate(this);
      *(undefined4 *)(this + 0x54) = 7;
    }
    else if (iVar1 == 7) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString((CCharString *)&uStack_10,"QR_EscortTrader",-1);
      ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&uStack_10;
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                (aaStack_14,extraout_EDX_12,ppVar9);
      while (cVar4 != '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)aaStack_14,"QR_EscortTrader",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))(aaStack_14);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  (aaStack_14,extraout_EDX_13,ppVar9);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      CCharString::CCharString((CCharString *)&uStack_10,"Q_OpeningGraveyardSecretPassage",-1);
      cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(&uStack_10);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_c,
                 extraout_EDX_14,unaff_EBX);
      while (cVar4 == '\0') {
        (**(code **)(**(int **)(this + 0x40) + 0x1c))();
        bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
        if (bVar3) {
          return;
        }
        CCharString::CCharString((CCharString *)&uStack_c,"Q_OpeningGraveyardSecretPassage",-1);
        cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x478))(&uStack_c);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_c,
                   extraout_EDX_15,unaff_EBX);
      }
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      *(undefined4 *)(this + 0x54) = 8;
    }
    else if (iVar1 == 8) {
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      Activate(this);
      bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (bVar3) {
        return;
      }
      break;
    }
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar3) {
      return;
    }
  } while( true );
LAB_00e25460:
  CCharString::CCharString((CCharString *)&uStack_c,"QR_EscortTrader",-1);
  ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&uStack_c;
  cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_10,
             extraout_EDX_16,ppVar9);
  while (cVar4 != '\0') {
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar3) {
      return;
    }
    CCharString::CCharString((CCharString *)&uStack_10,"QR_EscortTrader",-1);
    cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x470))(&uStack_10);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_10,
               extraout_EDX_17,ppVar9);
  }
  bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  if (bVar3) {
    return;
  }
  do {
    CCharString::CCharString((CCharString *)&local_8,"LookoutPoint",-1);
    ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_8;
    uStack_c = uStack_c | 1;
    cVar4 = (**(code **)(**(int **)(this + 0x40) + 0x30))();
    iVar1 = DAT_0143e90c;
    ppVar6 = extraout_EDX_18;
    if (cVar4 == '\0') {
      iVar5 = (**(code **)(**(int **)(this + 0x40) + 0xa2c))();
      iVar2 = DAT_0143e90c;
      ppVar6 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)(iVar5 % *(int *)(iVar1 + 0x9d4));
      if (ppVar6 != (pair<EHeroMorphType,CParticleMorphs::CEntry> *)0x0) goto LAB_00e25555;
      uVar7 = (**(code **)(**(int **)(this + 0x40) + 0xa1c))();
      ppVar6 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar7 >> 0x20);
      if ((int)uVar7 <= *(int *)(iVar2 + 0x9d8)) goto LAB_00e25555;
      bVar3 = false;
    }
    else {
LAB_00e25555:
      bVar3 = true;
    }
    if ((uStack_10 & 1) != 0) {
      uStack_10 = uStack_10 & 0xfffffffe;
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&uStack_c,ppVar6,
                 ppVar9);
    }
    if (!bVar3) break;
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar3) {
      return;
    }
  } while( true );
  bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  if (bVar3) {
    return;
  }
  Activate(this);
  (**(code **)(**(int **)(this + 0x40) + 0x1c))();
  bVar3 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
  if (bVar3) {
    return;
  }
  goto LAB_00e25460;
}


// CALLEES of Main:
//   00e255d0  Activate
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099ebf0  CCharString
//   00cb7940  IsActiveThreadTerminating

//=== Activate @ 00e255d0 (seed 00e255d0) ===

/* [bsim sim=0.951184502700164 <- ego_r]
   public: void __thiscall NScript::CQR_EscortTrader_ManagerScript::Activate(void) */

void __thiscall
NScript::CQR_EscortTrader_ManagerScript::Activate(CQR_EscortTrader_ManagerScript *this)

{
  char cVar1;
  bool bVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar3;
  CQR_EscortTrader_ManagerScript *local_4;
  
  local_4 = this;
  CCharString::CCharString((CCharString *)&local_4,"LookoutPoint",-1);
  ppVar3 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&local_4;
  cVar1 = (**(code **)(**(int **)(this + 0x40) + 0x30))();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff8,
             extraout_EDX,ppVar3);
  while( true ) {
    if (cVar1 == '\0') {
      bVar2 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
      if (!bVar2) {
        *(int *)(*(int *)(this + 0x44) + 0x5c) = *(int *)(*(int *)(this + 0x44) + 0x5c) + 1;
        CCharString::CCharString((CCharString *)&stack0xfffffff8,"QR_EscortTrader",-1);
        ppVar3 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)&stack0xfffffff8;
        (**(code **)(**(int **)(this + 0x40) + 0x450))();
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff4
                   ,extraout_EDX_01,ppVar3);
      }
      return;
    }
    (**(code **)(**(int **)(this + 0x40) + 0x1c))();
    bVar2 = CScriptBase::IsActiveThreadTerminating((CScriptBase *)this);
    if (bVar2) break;
    CCharString::CCharString((CCharString *)&stack0xfffffff8,"LookoutPoint",-1);
    cVar1 = (**(code **)(**(int **)(this + 0x40) + 0x30))(&stack0xfffffff8);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&stack0xfffffff8,
               extraout_EDX_00,ppVar3);
  }
  return;
}


// CALLEES of Activate:
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0099ebf0  CCharString
//   00cb7940  IsActiveThreadTerminating

