//=== FUN_004199c0 @ 004199c0 (seed 0x004199c0) ===

undefined4 FUN_004199c0(void)

{
  int iVar1;
  CBasicString<char> *unaff_EBX;
  undefined4 unaff_EBP;
  char *unaff_ESI;
  char *pcVar2;
  char *unaff_EDI;
  char *pcVar3;
  bool bVar4;
  
  iVar1 = 0x1d;
  bVar4 = true;
  do {
    if (iVar1 == 0) break;
    iVar1 = iVar1 + -1;
    bVar4 = *unaff_ESI == *unaff_EDI;
    unaff_ESI = unaff_ESI + 1;
    unaff_EDI = unaff_EDI + 1;
  } while (bVar4);
  if (bVar4) {
    return 0xb;
  }
  if (unaff_EBX == (CBasicString<char> *)0x0) {
    iVar1 = 0x1b;
    bVar4 = true;
    pcVar2 = "HERO_ABILITY_BATTLE_CHARGE";
    pcVar3 = "";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar2 == *pcVar3;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + 1;
    } while (bVar4);
    if (bVar4) {
      return 0xc;
    }
  }
  else {
    bVar4 = CBasicString<char>::operator==(unaff_EBX,"HERO_ABILITY_BATTLE_CHARGE");
    if (bVar4) {
      return 0xc;
    }
  }
  if (unaff_EBX == (CBasicString<char> *)0x0) {
    iVar1 = 0x1b;
    bVar4 = true;
    pcVar2 = "HERO_ABILITY_ASSASSIN_RUSH";
    pcVar3 = "";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar2 == *pcVar3;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + 1;
    } while (bVar4);
    if (bVar4) {
      return 0xd;
    }
  }
  else {
    bVar4 = CBasicString<char>::operator==(unaff_EBX,"HERO_ABILITY_ASSASSIN_RUSH");
    if (bVar4) {
      return 0xd;
    }
  }
  if (unaff_EBX == (CBasicString<char> *)0x0) {
    iVar1 = 0x1d;
    bVar4 = true;
    pcVar2 = "HERO_ABILITY_HEAL_LIFE_SPELL";
    pcVar3 = "";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar2 == *pcVar3;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + 1;
    } while (bVar4);
    if (bVar4) {
      return 0xe;
    }
  }
  else {
    bVar4 = CBasicString<char>::operator==(unaff_EBX,"HERO_ABILITY_HEAL_LIFE_SPELL");
    if (bVar4) {
      return 0xe;
    }
  }
  if (unaff_EBX == (CBasicString<char> *)0x0) {
    iVar1 = 0x1f;
    bVar4 = true;
    pcVar2 = "HERO_ABILITY_GHOST_SWORD_SPELL";
    pcVar3 = "";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar2 == *pcVar3;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + 1;
    } while (bVar4);
    if (bVar4) {
      return 0xf;
    }
  }
  else {
    bVar4 = CBasicString<char>::operator==(unaff_EBX,"HERO_ABILITY_GHOST_SWORD_SPELL");
    if (bVar4) {
      return 0xf;
    }
  }
  if (unaff_EBX == (CBasicString<char> *)0x0) {
    iVar1 = 0x1c;
    bVar4 = true;
    pcVar2 = "HERO_ABILITY_FIREBALL_SPELL";
    pcVar3 = "";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar2 == *pcVar3;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + 1;
    } while (bVar4);
    if (bVar4) {
      return 0x10;
    }
  }
  else {
    bVar4 = CBasicString<char>::operator==(unaff_EBX,"HERO_ABILITY_FIREBALL_SPELL");
    if (bVar4) {
      return 0x10;
    }
  }
  if (unaff_EBX == (CBasicString<char> *)0x0) {
    iVar1 = 0x19;
    bVar4 = true;
    pcVar2 = "HERO_ABILITY_MULTI_ARROW";
    pcVar3 = "";
    do {
      if (iVar1 == 0) break;
      iVar1 = iVar1 + -1;
      bVar4 = *pcVar2 == *pcVar3;
      pcVar2 = pcVar2 + 1;
      pcVar3 = pcVar3 + 1;
    } while (bVar4);
    if (!bVar4) {
      return unaff_EBP;
    }
  }
  else {
    bVar4 = CBasicString<char>::operator==(unaff_EBX,"HERO_ABILITY_MULTI_ARROW");
    if (!bVar4) {
      return unaff_EBP;
    }
  }
  return 0x11;
}


// CALLEES of FUN_004199c0:
//   004115a0  operator==

//=== GetFrame @ 0041c260 (seed 0x0041c2a0) ===

/* [bsim sim=0.7081390688707754 <- ego_r]
   public: bool __thiscall CGraphicInfo::GetFrame(class CGraphicFrame &,unsigned long,unsigned
   long,unsigned long)const  */

bool __thiscall
CGraphicInfo::GetFrame
          (CGraphicInfo *this,CGraphicFrame *param_1,ulong param_2,ulong param_3,ulong param_4)

{
  int *piVar1;
  int iVar2;
  CResource *pCVar3;
  CGraphicFrameData *pCVar4;
  bool bVar5;
  CFrameData *pCVar6;
  int *piVar7;
  
  pCVar6 = *(CFrameData **)(this + 8);
  if (pCVar6 == (CFrameData *)0x0) {
    if ((param_3 & 4) != 0) {
      return false;
    }
    pCVar6 = ::operator_new(0x44);
    if (pCVar6 == (CFrameData *)0x0) {
      pCVar6 = (CFrameData *)0x0;
    }
    else {
      pCVar6 = (CFrameData *)
               CGraphicInfoFrame::CFrameData::CFrameData(pCVar6,(CGraphicInfoFrame *)this);
    }
    piVar7 = *(int **)(this + 8);
    if (piVar7 != (int *)0x0) {
      piVar1 = piVar7 + 1;
      *piVar1 = *piVar1 + -1;
      if (*piVar1 == 0) {
        (**(code **)(*piVar7 + 4))();
      }
      *(undefined4 *)(this + 8) = 0;
    }
    *(CFrameData **)(this + 8) = pCVar6;
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar6,param_2,param_3);
LAB_0041c41a:
    if ((param_3 & 1) == 0) {
      return false;
    }
    bVar5 = CGraphicInfoFrame::InternalGetFrameForcedLoading((CGraphicInfoFrame *)this,param_3);
    if (!bVar5) {
      return false;
    }
  }
  else {
    if (*(int *)(pCVar6 + 0x1c) == 0) {
LAB_0041c365:
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar6,param_2,param_3);
      goto LAB_0041c41a;
    }
    if (((param_3 & 8) == 0) &&
       ((iVar2 = *(int *)(*(int *)(*(int *)(this + 0xc) + 0x20) + 0x194),
        *(int *)(pCVar6 + 0x3c) != iVar2 || (param_2 <= (uint)(int)(char)pCVar6[0x3a])))) {
      *(int *)(*(int *)(this + 8) + 0x3c) = iVar2;
      pCVar3 = *(CResource **)(*(int *)(this + 8) + 0x24);
      if ((pCVar3 != (CResource *)0x0) &&
         ((*(int *)(pCVar3 + 8) != 0 &&
          (*(int *)(pCVar3 + 0x18) != *(int *)(*(int *)(pCVar3 + 8) + 0x2c))))) {
        CResource::ListRemove(pCVar3);
        CResource::ListInsert
                  (pCVar3,*(CResource **)(*(int *)(pCVar3 + 8) + 0x10),
                   (CResource *)(*(int *)(pCVar3 + 8) + 4));
        *(undefined4 *)(pCVar3 + 0x18) = *(undefined4 *)(*(int *)(pCVar3 + 8) + 0x2c);
      }
      pCVar3 = *(CResource **)(this + 8);
      if (((param_2 <= (uint)(int)(char)pCVar3[0x38]) && (*(int *)(pCVar3 + 8) != 0)) &&
         (*(int *)(pCVar3 + 0x18) != *(int *)(*(int *)(pCVar3 + 8) + 0x2c))) {
        CResource::ListRemove(pCVar3);
        CResource::ListInsert
                  (pCVar3,*(CResource **)(*(int *)(pCVar3 + 8) + 0x10),
                   (CResource *)(*(int *)(pCVar3 + 8) + 4));
        *(undefined4 *)(pCVar3 + 0x18) = *(undefined4 *)(*(int *)(pCVar3 + 8) + 0x2c);
      }
      if (param_2 < (uint)(int)*(char *)(*(int *)(this + 8) + 0x3a)) {
        *(char *)(*(int *)(this + 8) + 0x3a) = (char)param_2;
      }
    }
    pCVar6 = *(CFrameData **)(this + 8);
    if ((uint)(int)(char)pCVar6[0x38] <= param_2) {
      pCVar4 = *(CGraphicFrameData **)param_1;
      if ((pCVar4 != (CGraphicFrameData *)0x0) &&
         (*(int *)pCVar4 = *(int *)pCVar4 + -1, *(int *)pCVar4 == 0)) {
        if (((byte)pCVar4[0x1a] & 0x10) != 0) {
          CGraphicFrameData::Uninitialise(pCVar4);
        }
        piVar7 = *(int **)(pCVar4 + 0xc);
        if (piVar7 != (int *)0x0) {
          *piVar7 = *piVar7 + -1;
          if (**(int **)(pCVar4 + 0xc) == 0) {
            (*(code *)(*(int **)(pCVar4 + 0xc))[1])();
            operator_delete(*(void **)(pCVar4 + 0xc));
          }
        }
        *(undefined4 *)(pCVar4 + 8) = 0;
        *(undefined4 *)(pCVar4 + 0xc) = 0;
        operator_delete(pCVar4);
      }
      piVar7 = *(int **)(pCVar6 + 0x1c);
      goto LAB_0041c47d;
    }
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar6,param_2,param_3);
    if (((param_3 & 0x10) != 0) &&
       (pCVar6 = *(CFrameData **)(this + 8), *(int *)(pCVar6 + 0x30) != 0)) goto LAB_0041c365;
  }
  pCVar4 = *(CGraphicFrameData **)param_1;
  iVar2 = *(int *)(this + 8);
  if ((pCVar4 != (CGraphicFrameData *)0x0) &&
     (*(int *)pCVar4 = *(int *)pCVar4 + -1, *(int *)pCVar4 == 0)) {
    if (((byte)pCVar4[0x1a] & 0x10) != 0) {
      CGraphicFrameData::Uninitialise(pCVar4);
    }
    piVar7 = *(int **)(pCVar4 + 0xc);
    if (piVar7 != (int *)0x0) {
      *piVar7 = *piVar7 + -1;
      if (**(int **)(pCVar4 + 0xc) == 0) {
        (*(code *)(*(int **)(pCVar4 + 0xc))[1])();
        operator_delete(*(void **)(pCVar4 + 0xc));
      }
    }
    *(undefined4 *)(pCVar4 + 8) = 0;
    *(undefined4 *)(pCVar4 + 0xc) = 0;
    operator_delete(pCVar4);
  }
  piVar7 = *(int **)(iVar2 + 0x1c);
LAB_0041c47d:
  *(int **)param_1 = piVar7;
  if (piVar7 != (int *)0x0) {
    *piVar7 = *piVar7 + 1;
  }
  return true;
}


// CALLEES of GetFrame:
//   009ff6b0  InternalGetFrameForcedLoading
//   009fbfb0  ListRemove
//   00bfea1a  operator_new
//   009fbf90  ListInsert
//   009fcba0  Uninitialise
//   009fe380  CFrameData
//   00bfe9bc  operator_delete
//   009fde80  BeginLoading

//=== 0x004199b0 : NO FUNCTION ===
//=== AddStatToListPC @ 00573792 (seed 0x005743b0) ===

/* [bsim sim=0.52712340725141 <- ego_r]
   private: void __thiscall CTCInventoryExperience::AddStatToListPC(class NUISystem::CComponent
   *,enum EHeroTrainableStatType,class NUISystem::CList *,bool,class CCountedPointer<class
   NUISystem::CComponent> *) */

void __thiscall
CTCInventoryExperience::AddStatToListPC
          (CTCInventoryExperience *this,CComponent *param_1,EHeroTrainableStatType param_2,
          CList *param_3,bool param_4,CCountedPointer<NUISystem::CComponent> *param_5)

{
  CDataBank *this_00;
  char cVar1;
  int *piVar2;
  long lVar3;
  long lVar4;
  CCharString *pCVar5;
  CTCHeroExperience *pCVar6;
  CCharString *extraout_EAX;
  CComponent *pCVar7;
  CWideString *pCVar8;
  CRGBFloatColour *pCVar9;
  int *piVar10;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *ppVar11;
  CRumbleDef *pCVar12;
  CAction *pCVar13;
  undefined4 *puVar14;
  int iVar15;
  EHeroExperienceStatCategory EVar16;
  CTCInventoryExperience *this_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_03;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_04;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_05;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_06;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_07;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_08;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_09;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_10;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_11;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_12;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_13;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_14;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_15;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *pCVar17;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_16;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_17;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_18;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_19;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_20;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_21;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_22;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_23;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_24;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_25;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_26;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_27;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_28;
  CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> *extraout_EDX_29;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_30;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  C2DCoordI *pCVar18;
  CTCInventoryExperience *pCVar19;
  EHeroTrainableStatType EVar20;
  EHeroExperienceStatCategory EVar21;
  float fVar22;
  float fVar23;
  CCharString CVar24;
  undefined1 uVar25;
  CRGBFloatColour *pCVar26;
  float fVar27;
  EHeroTrainableStatType EVar28;
  CCountedPointer<NUISystem::CAction> local_b4 [8];
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *local_ac [4];
  int *local_9c;
  undefined4 local_98;
  CCharString local_94 [4];
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *local_90 [2];
  CAction *local_88;
  CCPPointerInfo *local_84;
  CAction *local_80;
  CDataBank *local_7c;
  CAction *local_78;
  CCPPointerInfo *local_74;
  int *local_70;
  undefined4 local_6c;
  int *local_68;
  int *local_64;
  undefined4 *local_60;
  CCharString local_5c [4];
  CCharString local_58 [4];
  float local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 *local_48;
  undefined4 local_44;
  CCharString local_40 [4];
  int *local_3c;
  CTCInventoryExperience *local_38;
  CCharString local_34 [4];
  int *local_30 [2];
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  local_28 [4];
  CFrontEndManager *local_24;
  CCharString local_20 [4];
  undefined4 *local_1c;
  undefined4 local_18;
  undefined4 *local_14;
  CTCHeroExperience *local_10;
  CTCHeroExperience *local_c;
  ETCInterfaceType local_8;
  
  CCharString::CCharString(local_20,"SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_",-1);
  EVar20 = param_2;
  if (param_2 == 2) {
    CCharString::operator+=(local_20,"TOUGHNESS");
  }
  if (EVar20 == 0) {
    CCharString::operator+=(local_20,"PHYSIQUE");
  }
  if (EVar20 == 1) {
    CCharString::operator+=(local_20,"HEALTH");
  }
  if (EVar20 == 3) {
    CCharString::operator+=(local_20,"SPEED");
  }
  if (EVar20 == 4) {
    CCharString::operator+=(local_20,"ACCURACY");
  }
  if (EVar20 == 5) {
    CCharString::operator+=(local_20,"GUILE");
  }
  if (EVar20 == 9) {
    CCharString::operator+=(local_20,"MAGICPOWER");
  }
  local_7c = *(CDataBank **)(DAT_013b86a0 + 0x14);
  local_24 = NUISystem::CFrontEndManager::GetInstance();
  GetExperienceAbilityName(this,(long)local_40);
  iVar15 = *(int *)(this + 4);
  cVar1 = FUN_00410de0();
  if (cVar1 != '\0') {
    local_8 = 0x68;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar15 + 0x44),&local_8);
    piVar10 = *(int **)(iVar15 + 0x48);
    if ((piVar2 == piVar10) || (0x68 < *piVar2)) {
      piVar2 = piVar10;
    }
    local_c = (CTCHeroExperience *)piVar2[1];
  }
  lVar3 = CTCHeroExperience::GetTrainableStatMaxLevel(local_c,EVar20);
  lVar4 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)local_c,EVar20);
  if (lVar4 < lVar3) {
    CCharString::CCharString((CCharString *)&local_8," +",-1);
    pCVar5 = (CCharString *)
             NScript::
             operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                       (local_28,(CIsThingAlive *)&local_8);
    CCharString::operator+=(local_40,pCVar5);
    CCharString::~CCharString((CCharString *)local_28);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
               extraout_EDX,unaff_EDI);
    EVar28 = EVar20;
    pCVar6 = GetTCHeroExperience(this);
    CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar6,EVar28);
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)local_28,(long)unaff_EDI);
    CCharString::operator+=(local_40,extraout_EAX);
    CCharString::~CCharString((CCharString *)local_28);
  }
  CCharString::CCharString((CCharString *)&local_8,"PC_MENU_ENTRY_BRANCH_TEMPLATE",-1);
  CCharString::CCharString((CCharString *)&stack0xffffff3c,"PC_BUTTON_BASTARD",-1);
  CCharString::CCharString((CCharString *)&stack0xffffff34,"",-1);
  pCVar7 = NUISystem::CManager::CreateMenuEntryComponent((CManager *)local_24);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_30,(CAction *)pCVar7);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_00,unaff_EDI);
  CCharString::CCharString((CCharString *)&stack0xffffff3c,local_20);
  AddSoundAction(this);
  local_64 = local_30[0];
  local_68 = (int *)EVar20;
  FUN_00579958();
  CCharString::CCharString((CCharString *)&local_8,"PC_MOVIE",-1);
  pCVar7 = NUISystem::CManager::CreateComponent((CManager *)local_24,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_90,(CAction *)pCVar7);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_01,unaff_EDI);
  ppVar11 = local_90[0];
  GetExperienceStatMovieName(this,(EHeroTrainableStatType)local_28);
  CPersistContext::GetDefaultVal<CAnimationSet>();
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_28,
             extraout_EDX_02,unaff_EDI);
  (**(code **)(*(int *)ppVar11 + 0x238))();
  (**(code **)(*local_30[0] + 0xec))();
  if (*(int **)(this + 0x1d8) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x1d8) + 0x238))();
  }
  (**(code **)(*(int *)param_1 + 0x1f0))();
  CCharString::CCharString((CCharString *)&local_8,"PC_TEXT_EXPERIENCE_STAT_NAME",-1);
  pCVar7 = NUISystem::CManager::CreateComponent((CManager *)local_24,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)&local_78,(CAction *)pCVar7);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_03,local_90[0]);
  local_68 = (int *)0x0;
  local_64 = (int *)0x0;
  CCountedPointer<NUISystem::CAction>::ShareData
            ((CCountedPointer<NUISystem::CAction> *)&local_68,local_78,local_74);
  FUN_0099b6b0();
  CCharString::CCharString(local_34);
  lVar3 = CTCHeroExperience::GetTrainableStatMaxLevel(local_c,EVar20);
  lVar4 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)local_c,EVar20);
  if (lVar4 < lVar3) {
    CCharString::CCharString((CCharString *)&local_8,"TEXT_GUI_MENU_EXP_LEVEL",-1);
    lVar3 = FUN_0099b6b0();
    EVar28 = EVar20;
    pCVar6 = GetTCHeroExperience(this);
    CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar6,EVar28);
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_38,lVar3);
    NGameText::CDataBank::GetTextBySymbol(local_7c,(CCharString *)&local_18);
    GetExperienceAbilityName(this,(long)&local_44);
    FUN_0099be70();
    FUN_0099be70();
    FUN_0099be70();
    FUN_0099be70();
    pCVar8 = (CWideString *)FUN_0099be70();
    CWideString::operator=((CWideString *)local_34,pCVar8);
    CCharString::~CCharString((CCharString *)local_28);
    CCharString::~CCharString((CCharString *)&local_84);
    CCharString::~CCharString((CCharString *)&local_6c);
    CCharString::~CCharString(local_5c);
    CCharString::~CCharString((CCharString *)&local_98);
    CCharString::~CCharString((CCharString *)&local_44);
    CCharString::~CCharString((CCharString *)&local_18);
    CCharString::~CCharString((CCharString *)&local_38);
    CCharString::~CCharString((CCharString *)&local_10);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
               extraout_EDX_04,local_90[0]);
    CCharString::CCharString((CCharString *)&local_8,"TEXT_GUI_MENU_EXPERIENCE_COST",-1);
    this_00 = *(CDataBank **)(DAT_013b86a0 + 0x14);
    EVar28 = EVar20;
    pCVar6 = GetTCHeroExperience(this);
    CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar6,EVar28);
    CGuiControlNumber::SetSLongValue((CGuiControlNumber *)&local_98,(long)local_90[0]);
    FUN_0099b6b0();
    NGameText::CDataBank::GetTextBySymbol(this_00,(CCharString *)&local_18);
    FUN_0099be70();
    pCVar5 = (CCharString *)FUN_0099be70();
    CCharString::operator+=(local_34,pCVar5);
    CCharString::~CCharString((CCharString *)&local_10);
    CCharString::~CCharString((CCharString *)&local_38);
    CCharString::~CCharString((CCharString *)&local_18);
    CCharString::~CCharString((CCharString *)&local_44);
    CCharString::~CCharString((CCharString *)&local_98);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
               extraout_EDX_05,local_90[0]);
    pCVar5 = (CCharString *)FUN_0099b6b0();
    CCharString::operator+=(local_34,pCVar5);
    CCharString::~CCharString((CCharString *)&local_10);
  }
  CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)local_c,EVar20);
  pCVar5 = (CCharString *)GetExperienceStatDescription(this,(long)&local_10,EVar20);
  CCharString::operator+=(local_34,pCVar5);
  CCharString::~CCharString((CCharString *)&local_10);
  (**(code **)(*local_68 + 0x240))();
  (**(code **)(*local_30[0] + 0xec))();
  (**(code **)(*(int *)param_1 + 0x1f0))();
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_78;
  if (*(int **)(this + 0x1dc) != (int *)0x0) {
    (**(code **)(**(int **)(this + 0x1dc) + 0x238))();
    ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_78;
  }
  iVar15 = *(int *)(this + 4);
  cVar1 = FUN_00410de0();
  if (cVar1 != '\0') {
    local_10 = (CTCHeroExperience *)0x68;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar15 + 0x44),(ETCInterfaceType *)&local_10);
    piVar10 = *(int **)(iVar15 + 0x48);
    if ((piVar2 == piVar10) || (0x68 < *piVar2)) {
      piVar2 = piVar10;
    }
    local_c = (CTCHeroExperience *)piVar2[1];
  }
  local_54 = 0.0;
  lVar3 = CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)local_c,EVar20);
  local_38 = (CTCInventoryExperience *)CTCHeroExperience::GetTrainableStatMaxLevel(local_c,EVar20);
  local_c = (CTCHeroExperience *)FUN_006d6fa0();
  local_10 = local_c + lVar3;
  local_54 = (float)(int)local_10 + local_54;
  CCharString::CCharString((CCharString *)&local_8,"PC_EXPERIENCE_SPENDING_TICK_CONTAINER",-1);
  pCVar7 = NUISystem::CManager::CreateComponent((CManager *)local_24,(CCharString *)&local_8,false);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            ((CCountedPointer<NUISystem::CAction> *)local_ac,(CAction *)pCVar7);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&local_8,
             extraout_EDX_06,ppVar11);
  local_50 = 0;
  local_4c = 0;
  pCVar26 = (CRGBFloatColour *)&local_50;
  local_1c = (undefined4 *)0x0;
  local_18 = 0;
  local_48 = (undefined4 *)0x0;
  local_44 = 0;
  fVar27 = 40.0;
  uVar25 = 0;
  CVar24 = (CCharString)0x0;
  fVar23 = 310.0;
  fVar22 = local_54;
  pCVar9 = (CRGBFloatColour *)FUN_0056ed74();
  pCVar18 = (C2DCoordI *)&local_1c;
  pCVar19 = local_38;
  pCVar8 = (CWideString *)GetExperienceAbilityName(this_01,(long)&local_10);
  pCVar7 = NUISystem::CManager::CreateTickSlider
                     ((CManager *)local_24,pCVar8,pCVar18,lVar3,(long)pCVar19,pCVar9,fVar22,fVar23,
                      (bool)CVar24,(bool)uVar25,pCVar26,fVar27);
  CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
            (local_b4,(CAction *)pCVar7);
  CCharString::~CCharString((CCharString *)&local_10);
  (**(code **)(*(int *)local_ac[0] + 0xec))();
  (**(code **)(*local_30[0] + 0xec))();
  (**(code **)(*(int *)param_1 + 0x1f0))();
  pCVar19 = this + 0x140;
  local_38 = pCVar19;
  local_8 = FUN_00579114();
  if (((int)local_c < 1) && (local_8 == *(ETCInterfaceType *)pCVar19)) {
    iVar15 = *local_30[0];
    CCharString::CCharString((CCharString *)&stack0xffffff10,"UI_SELECTION_ARROW_RIGHT",-1);
    piVar10 = (int *)(**(code **)(iVar15 + 0xc))();
    if (piVar10 != (int *)0x0) {
      CBankFile::GetBankHandle((CBankFile *)&local_14);
      iVar15 = *piVar10;
      local_14 = (undefined4 *)0x1;
      std::pair<unsigned_long,CAnimationEntry>::pair<unsigned_long,CAnimationEntry>
                ((pair<unsigned_long,CAnimationEntry> *)&stack0xffffff08,
                 (pair<unsigned_long,class_CAnimationEntry> *)&local_14);
      (**(code **)(iVar15 + 0xf8))();
      CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&local_10);
    }
    (**(code **)(*local_30[0] + 0x170))();
  }
  else {
    CCharString::CCharString((CCharString *)&param_1,"PC_SUB_LIST",-1);
    pCVar7 = NUISystem::CManager::CreateComponent
                       ((CManager *)local_24,(CCharString *)&param_1,false);
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_80,(CAction *)pCVar7);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
               extraout_EDX_07,local_ac[0]);
    local_9c = (int *)0x0;
    local_98 = 0;
    CCountedPointer<NUISystem::CAction>::ShareData
              ((CCountedPointer<NUISystem::CAction> *)&local_9c,local_80,(CCPPointerInfo *)local_7c)
    ;
    piVar10 = local_9c;
    if (param_5 != (CCountedPointer<NUISystem::CComponent> *)0x0) {
      (**(code **)(*local_9c + 0x244))();
    }
    ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x1;
    if ((0 < (int)local_c) && (local_8 != *(ETCInterfaceType *)pCVar19)) {
      ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x2;
    }
    (**(code **)(*piVar10 + 0x270))();
    (**(code **)(*local_30[0] + 0xec))();
    pCVar12 = ::operator_new(0x10);
    if (pCVar12 == (CRumbleDef *)0x0) {
      pCVar13 = (CAction *)0x0;
    }
    else {
      pCVar13 = (CAction *)CRumbleDef::CRumbleDef(pCVar12);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_1c,pCVar13);
    *local_1c = 0xd5;
    param_1 = (CComponent *)**(undefined4 **)(this + 0x23c);
    iVar15 = *(int *)param_1;
    CCharString::CCharString((CCharString *)&stack0xffffff08,"UI_LEVEL_SLIDER_NUMBER",-1);
    (**(code **)(iVar15 + 0xc))();
    FUN_0053fde0();
    puVar14 = ::operator_new(0xc);
    if (puVar14 != (undefined4 *)0x0) {
      puVar14[2] = 0xffffffff;
      puVar14[1] = 3;
      *puVar14 = &PTR_LAB_012366f4;
    }
    FUN_0053fdb0();
    param_1 = *(CComponent **)(*(int *)(this + 0x23c) + 4);
    iVar15 = *(int *)param_1;
    CCharString::CCharString((CCharString *)&stack0xffffff04,"UI_LEVEL_SLIDER_NUMBER",-1);
    (**(code **)(iVar15 + 0xc))();
    FUN_0053fde0();
    puVar14 = ::operator_new(0xc);
    if (puVar14 != (undefined4 *)0x0) {
      puVar14[2] = 0xffffffff;
      puVar14[1] = 3;
      *puVar14 = &PTR_LAB_012366f4;
    }
    FUN_0053fdb0();
    param_1 = *(CComponent **)(*(int *)(this + 0x23c) + 8);
    iVar15 = *(int *)param_1;
    CCharString::CCharString((CCharString *)&stack0xffffff00,"UI_LEVEL_SLIDER_NUMBER",-1);
    iVar15 = (**(code **)(iVar15 + 0xc))();
    FUN_0053fde0(-(uint)(iVar15 != 0) & iVar15 + 0x18U);
    puVar14 = ::operator_new(0xc);
    if (puVar14 != (undefined4 *)0x0) {
      puVar14[2] = 0xffffffff;
      puVar14[1] = 3;
      *puVar14 = &PTR_LAB_012366f4;
    }
    FUN_0053fdb0();
    param_1 = *(CComponent **)(*(int *)(this + 0x23c) + 0xc);
    iVar15 = *(int *)param_1;
    CCharString::CCharString((CCharString *)&stack0xfffffefc,"UI_LEVEL_SLIDER_NUMBER",-1);
    iVar15 = (**(code **)(iVar15 + 0xc))();
    FUN_0053fde0(-(uint)(iVar15 != 0) & iVar15 + 0x18U,&local_1c);
    puVar14 = ::operator_new(0xc);
    if (puVar14 == (undefined4 *)0x0) {
      puVar14 = (undefined4 *)0x0;
    }
    else {
      puVar14[2] = 0xffffffff;
      puVar14[1] = 3;
      *puVar14 = &PTR_LAB_012366f4;
    }
    FUN_0053fdb0(puVar14);
    (**(code **)(*local_30[0] + 0x10c))(&local_1c);
    (**(code **)(*local_30[0] + 0x110))(&local_1c);
    pCVar17 = extraout_EDX_08;
    if (0 < (int)local_c) {
      *(int *)(this + 0x218) = *(int *)(this + 0x218) + 1;
      CCharString::CCharString((CCharString *)&param_1,"TEXT_GUI_MENU_BUY",-1);
      NGameText::CDataBank::GetTextBySymbol
                (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&local_c);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_09,ppVar11);
      CCharString::CCharString((CCharString *)&param_1,"PC_MENU_ENTRY_TEMPLATE",-1);
      CCharString::CCharString((CCharString *)&stack0xffffff0c,"PC_BUTTON_BASTARD",-1);
      CCharString::CCharString((CCharString *)&stack0xffffff04,"",-1);
      pCVar7 = NUISystem::CManager::CreateMenuEntryComponent
                         ((CManager *)local_24,&local_c,0,0x3f800000);
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_88,(CAction *)pCVar7);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_10,ppVar11);
      local_70 = (int *)0x0;
      local_6c = 0;
      CCountedPointer<NUISystem::CAction>::ShareData
                ((CCountedPointer<NUISystem::CAction> *)&local_70,local_88,local_84);
      pCVar12 = ::operator_new(0x10);
      if (pCVar12 == (CRumbleDef *)0x0) {
        pCVar13 = (CAction *)0x0;
      }
      else {
        pCVar13 = (CAction *)CRumbleDef::CRumbleDef(pCVar12);
      }
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_60,pCVar13);
      *local_60 = 7;
      puVar14 = ::operator_new(0xc);
      if (puVar14 != (undefined4 *)0x0) {
        puVar14[1] = 3;
        *puVar14 = &PTR_LAB_012366f4;
        puVar14[2] = param_2;
      }
      FUN_0053fdb0();
      puVar14 = ::operator_new(0xc);
      if (puVar14 != (undefined4 *)0x0) {
        EVar20 = param_2;
        pCVar6 = GetTCHeroExperience(this);
        lVar3 = CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar6,EVar20);
        puVar14[1] = 3;
        *puVar14 = &PTR_LAB_012366f4;
        puVar14[2] = lVar3;
      }
      FUN_0053fdb0();
      (**(code **)(*local_70 + 0x108))();
      (**(code **)(*local_9c + 0x260))();
      EVar20 = param_2;
      pCVar6 = GetTCHeroExperience(this);
      param_5 = (CCountedPointer<NUISystem::CComponent> *)
                CTCHeroExperience::GetCostToUpgradeTrainableStat(pCVar6,EVar20);
      EVar20 = param_2;
      pCVar6 = GetTCHeroExperience(this);
      EVar16 = CTCHeroExperience::GetCategoryTrainableStatIsIn(pCVar6,EVar20);
      EVar21 = EVar16;
      pCVar6 = GetTCHeroExperience(this);
      param_1 = (CComponent *)
                CTCInventoryAbilities::GetAbilityLevel((CTCInventoryAbilities *)pCVar6,EVar21);
      pCVar12 = ::operator_new(0x10);
      if (pCVar12 == (CRumbleDef *)0x0) {
        pCVar13 = (CAction *)0x0;
      }
      else {
        pCVar13 = (CAction *)CRumbleDef::CRumbleDef(pCVar12);
      }
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_48,pCVar13);
      *local_48 = 0xd5;
      if (0 < (int)param_1) {
        local_10 = *(CTCHeroExperience **)(*(int *)(this + 0x23c) + 4 + EVar16 * 4);
        iVar15 = *(int *)local_10;
        CCharString::CCharString((CCharString *)&stack0xffffff04,"UI_LEVEL_SLIDER_NUMBER",-1);
        (**(code **)(iVar15 + 0xc))();
        FUN_0053fde0();
        if ((int)param_5 < (int)param_1) {
          puVar14 = ::operator_new(0xc);
          pCVar7 = (CComponent *)param_5;
        }
        else {
          puVar14 = ::operator_new(0xc);
          pCVar7 = param_1;
        }
        if (puVar14 != (undefined4 *)0x0) {
          puVar14[1] = 3;
          *puVar14 = &PTR_LAB_012366f4;
          puVar14[2] = pCVar7;
        }
        FUN_0053fdb0();
      }
      iVar15 = (int)param_5 - (int)param_1;
      if (0 < iVar15) {
        param_1 = *(CComponent **)**(undefined4 **)(this + 0x23c);
        CCharString::CCharString((CCharString *)&stack0xffffff04,"UI_LEVEL_SLIDER_NUMBER",-1);
        (**(code **)(param_1 + 0xc))();
        FUN_0053fde0();
        puVar14 = ::operator_new(0xc);
        if (puVar14 != (undefined4 *)0x0) {
          puVar14[1] = 3;
          *puVar14 = &PTR_LAB_012366f4;
          puVar14[2] = iVar15;
        }
        FUN_0053fdb0();
      }
      (**(code **)(*local_30[0] + 0x10c))();
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_48,extraout_EDX_11);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_60,extraout_EDX_12);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_70,extraout_EDX_13);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_88,extraout_EDX_14);
      CCharString::~CCharString((CCharString *)&local_c);
      pCVar17 = extraout_EDX_15;
    }
    if (local_8 != *(ETCInterfaceType *)local_38) {
      CCharString::CCharString((CCharString *)&param_1,"TEXT_GUI_MENU_UNDO_LEVEL_UP",-1);
      NGameText::CDataBank::GetTextBySymbol
                (*(CDataBank **)(DAT_013b86a0 + 0x14),(CCharString *)&param_5);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_16,ppVar11);
      CCharString::CCharString((CCharString *)&param_1,"PC_MENU_ENTRY_TEMPLATE",-1);
      CCharString::CCharString((CCharString *)&stack0xffffff0c,"PC_BUTTON_BASTARD",-1);
      CCharString::CCharString((CCharString *)&stack0xffffff04,"",-1);
      pCVar7 = NUISystem::CManager::CreateMenuEntryComponent
                         ((CManager *)local_24,&param_5,0,0x3f800000);
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_3c,(CAction *)pCVar7);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_1,
                 extraout_EDX_17,ppVar11);
      (**(code **)(*local_9c + 0x260))();
      pCVar12 = ::operator_new(0x10);
      if (pCVar12 == (CRumbleDef *)0x0) {
        pCVar13 = (CAction *)0x0;
      }
      else {
        pCVar13 = (CAction *)CRumbleDef::CRumbleDef(pCVar12);
      }
      CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
                ((CCountedPointer<NUISystem::CAction> *)&local_14,pCVar13);
      *local_14 = 0xc9;
      puVar14 = ::operator_new(0xc);
      if (puVar14 != (undefined4 *)0x0) {
        puVar14[1] = 3;
        *puVar14 = &PTR_LAB_012366f4;
        puVar14[2] = param_2;
      }
      FUN_0053fdb0();
      (**(code **)(*local_3c + 0x108))();
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_14,extraout_EDX_18);
      std::
      _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
                ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
                 &local_3c,extraout_EDX_19);
      CCharString::~CCharString((CCharString *)&param_5);
      pCVar17 = extraout_EDX_20;
    }
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_1c,pCVar17);
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_9c,extraout_EDX_21);
    std::
    _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
              ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
               &local_80,extraout_EDX_22);
  }
  ppVar11 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)local_30;
  if (param_4 == false) {
    (**(code **)(*(int *)param_3 + 0x260))();
    pCVar17 = extraout_EDX_24;
  }
  else {
    (**(code **)(*(int *)param_3 + 0x264))();
    pCVar17 = extraout_EDX_23;
  }
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_b4
             ,pCVar17);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_ac
             ,extraout_EDX_25);
  CCharString::~CCharString(local_34);
  CCharString::~CCharString(local_58);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_68,extraout_EDX_26);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)
             &local_78,extraout_EDX_27);
  CCharString::~CCharString(local_94);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_90
             ,extraout_EDX_28);
  std::
  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
            ((allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> *)local_30
             ,extraout_EDX_29);
  CCharString::~CCharString(local_40);
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_20,
             extraout_EDX_30,ppVar11);
  return;
}


// CALLEES of AddStatToListPC:
//   0099f100  operator+=
//   00429460  ShareData
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0042cd84  pair<unsigned_long,CAnimationEntry>
//   00429160  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0056ff3d  GetTCHeroExperience
//   0099ebf0  CCharString
//   0056fdc3  GetExperienceStatDescription
//   0056ed74  FUN_0056ed74
//   009c95e0  GetTextBySymbol
//   00bfea1a  operator_new
//   0041e5f2  GetInstance
//   00429288  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   006d67b0  GetCostToUpgradeTrainableStat
//   006d6510  GetAbilityLevel
//   0056e6b6  GetExperienceStatMovieName
//   006d6fa0  FUN_006d6fa0
//   006d6850  GetCategoryTrainableStatIsIn
//   0042be50  CRumbleDef
//   0099b7d0  operator=
//   0042aa29  CCountedPointer<NUISystem::CAction>
//   00410de0  FUN_00410de0
//   0099aed0  CCharString
//   00579958  FUN_00579958
//   006d6770  GetTrainableStatMaxLevel
//   0099e480  GetDefaultVal<CAnimationSet>
//   00428494  CreateMenuEntryComponent
//   0042920a  ShareData
//   0099be70  FUN_0099be70
//   0099b6b0  FUN_0099b6b0
//   00579114  FUN_00579114
//   0056f26f  GetExperienceAbilityName
//   0040f020  LowerBound
//   0099ec30  CCharString
//   0099b6a0  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
//   00405f00  SetSLongValue
//   0042925c  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0053fde0  FUN_0053fde0
//   00429c15  CCountedPointer<NUISystem::CAction>
//   0041db1d  CreateComponent
//   006d6520  GetAbilityLevel
//   00427216  CreateTickSlider
//   0042bed4  GetBankHandle
//   0053fdb0  FUN_0053fdb0
//   00429486  ShareData
//   004291de  _Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>
//   0042abca  OnReadFinished
//   0099b8d0  operator+=
//   0056f615  AddSoundAction
//   0099b510  ~CCharString

//=== FUN_0066b280 @ 0066b280 (seed 0x0066b280) ===

void FUN_0066b280(int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5)

{
  int iVar1;
  CDisplayEngine *pCVar2;
  uint uVar3;
  CMeshDataBank *pCVar4;
  CViewBase *in_ECX;
  ulong uVar5;
  uint uVar6;
  uint uVar7;
  
  pCVar2 = NDisplayView::CViewBase::GetDisplayEngine(in_ECX);
  uVar5 = *(int *)(pCVar2 + 0x16140) * 0x24a1 + 0x24df;
  uVar7 = (param_1[1] - *param_1) + 1;
  *(ulong *)(pCVar2 + 0x16140) = uVar5;
  uVar3 = GFROR13(uVar5);
  uVar6 = 0;
  *(uint *)(pCVar2 + 0x16140) = uVar3;
  if (uVar7 != 0) {
    uVar6 = uVar3 % uVar7;
  }
  iVar1 = *param_1;
  pCVar4 = CAnimateThing::GetMeshBank(*(CAnimateThing **)(in_ECX + 4));
  FUN_0066b080(pCVar4,iVar1 + uVar6,param_2,param_3,param_4,param_5);
  return;
}


// CALLEES of FUN_0066b280:
//   00497890  GFROR13
//   004c7a80  GetMeshBank
//   00686d60  GetDisplayEngine
//   0066b080  FUN_0066b080

