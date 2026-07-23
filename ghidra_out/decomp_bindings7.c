//=== AddPersonToConversation @ 006e5800 ===

/* [bsim sim=0.6920857532200123 <- ego_r]
   public: void __thiscall CScriptConversationManager::AddPersonToConversation(long,class CThing &)
    */

void __thiscall
CScriptConversationManager::AddPersonToConversation
          (CScriptConversationManager *this,long param_1,CThing *param_2)

{
  CScriptConversationManager *local_4;
  
  local_4 = this;
  FUN_006e6990(&local_4,&param_1);
  if (local_4 != *(CScriptConversationManager **)this) {
    FUN_006e5790(param_2);
  }
  return;
}



//=== AddLineToConversation @ 006e5950 ===

/* [bsim sim=0.6920857532200123 <- ego_r]
   public: void __thiscall CScriptConversationManager::AddLineToConversation(long,class CThing const
   &,class CThing const *,class CCharString const &,bool) */

void __thiscall
CScriptConversationManager::AddLineToConversation
          (CScriptConversationManager *this,long param_1,CThing *param_2,CThing *param_3,
          CCharString *param_4,bool param_5)

{
  undefined3 in_stack_00000015;
  CScriptConversationManager *local_4;
  
  local_4 = this;
  FUN_006e6990(&local_4,&param_1);
  if (local_4 != *(CScriptConversationManager **)this) {
    FUN_006e58a0(param_2,param_3,param_4,_param_5);
  }
  return;
}



//=== CreateEffectOnHit @ 00839420 ===

/* [bsim sim=0.9198033241031143 <- ego_r]
   void __fastcall NThingHelpers::CreateEffectOnHit(class CThing const &,class CHitParameters const
   &,class CWorldMap const &,float) */

void __fastcall
NThingHelpers::CreateEffectOnHit
          (CThing *param_1,CHitParameters *param_2,CWorldMap *param_3,float param_4)

{
  CDefClassBase *pCVar1;
  CMaterialDef *pCVar2;
  int *piVar3;
  float fVar4;
  CDefClassBase *this;
  int *piVar5;
  int iVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  bool bVar10;
  CBaseObject *this_00;
  CUIState *pCVar11;
  float *pfVar12;
  float *pfVar13;
  CDefinitionManager *this_01;
  CIDrawEnvironment *pCVar14;
  CCategory *pCVar15;
  CDefString *this_02;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  C3DVector *unaff_EBX;
  CMaterialDef *pCVar16;
  CThing *this_03;
  long lVar17;
  int *local_80;
  CMaterialDef *local_7c;
  CThingBuilding *pCStack_78;
  long lStack_74;
  float fStack_70;
  CThingBuilding *local_6c;
  undefined1 auStack_68 [4];
  float local_64;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  undefined4 local_50;
  float fStack_4c;
  float fStack_48;
  undefined4 uStack_44;
  float fStack_40;
  float fStack_3c;
  undefined4 uStack_38;
  float fStack_34;
  float fStack_30;
  float fStack_2c;
  undefined4 uStack_28;
  CParticleUpdateParameters<unsigned_long,unsigned_long> aCStack_24 [36];
  
  local_6c = (CThingBuilding *)0x0;
  this_00 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(param_2 + 0x50));
  local_6c = (CThingBuilding *)this_00;
  bVar10 = NCombatActionHelpers::IsToCreateCombatParticleEffects(param_1,(CThing *)this_00);
  if (bVar10) {
    local_64 = *(float *)(param_2 + 2);
    local_60 = *(float *)(param_2 + 6);
    local_5c = *(float *)(param_2 + 10);
    if ((DAT_013756f5 != '\0') && (param_2[0x23] != (CHitParameters)0x0)) {
      local_58 = *(float *)(param_2 + 0xe);
      local_54 = *(float *)(param_2 + 0x12);
      local_50 = *(undefined4 *)(param_2 + 0x16);
      pCVar16 = *(CMaterialDef **)(param_2 + 0x4c);
      if (pCVar16 != (CMaterialDef *)0x0) {
        *(int *)(pCVar16 + 4) = *(int *)(pCVar16 + 4) + 1;
      }
      local_7c = pCVar16;
      if ((((this_00 != (CBaseObject *)0x0) && (((byte)this_00[0x91] & 1) == 0)) &&
          (((byte)param_1[0x6c] & 8) != 0)) &&
         (pCVar11 = NUISystem::CChangingStateComponent::GetCurrentState
                              ((CChangingStateComponent *)param_2), (char)pCVar11 != '\0')) {
        this = *(CDefClassBase **)(param_1 + 0x70);
        local_80 = (int *)0x0;
        if (this != (CDefClassBase *)0x0) {
          *(int *)(this + 4) = *(int *)(this + 4) + 1;
        }
        bVar10 = CDefClassBase::GetSubDef<CAbilityDef>
                           (this,(CDefPointer<CAbilityDef_const_> *)&local_80);
        if ((bVar10) && (*(char *)((int)local_80 + 0xcd) == '\0')) {
          bVar10 = true;
        }
        else {
          bVar10 = false;
        }
        if (this != (CDefClassBase *)0x0) {
          pCVar1 = this + 4;
          *(int *)pCVar1 = *(int *)pCVar1 + -1;
          if (*(int *)pCVar1 == 0) {
            (**(code **)(*(int *)this + 4))();
          }
        }
        if (bVar10) {
          pfVar12 = (float *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
          pfVar13 = (float *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)this_00);
          fStack_48 = pfVar13[1] - pfVar12[1];
          fStack_4c = *pfVar13 - *pfVar12;
          uStack_44 = 0;
          if (9.999999e-09 < fStack_48 * fStack_48 + fStack_4c * fStack_4c) {
            C3DVector::GetScaled((C3DVector *)&fStack_4c,unaff_EBX);
            fStack_30 = fStack_48 * *(float *)(*(int *)(param_1 + 0x60) + 0x34);
            fStack_40 = fStack_4c * *(float *)(*(int *)(param_1 + 0x60) + 0x34) * 0.75;
            fStack_3c = fStack_30 * 0.75;
            pfVar12 = (float *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
            local_64 = fStack_40 + *pfVar12;
            fStack_2c = local_5c;
            fStack_40 = local_58;
            uStack_38 = local_50;
            local_60 = fStack_3c + pfVar12[1];
            fStack_3c = local_54;
            fStack_34 = local_64;
            fStack_30 = local_60;
            if (((byte)this_00[0x6c] & 8) != 0) {
              fVar7 = -fStack_4c;
              if (*(int *)(this_00 + 0x138) == 0) {
                uStack_44 = 0x80000000;
                local_58 = fStack_48 * -1.0;
                local_50 = 0x80000000;
                local_54 = fVar7 * -1.0;
                fStack_4c = local_58;
                fStack_48 = fVar7 * -1.0;
              }
              else if (*(int *)(this_00 + 0x138) == 1) {
                uStack_44 = 0;
                local_50 = 0;
                fStack_4c = fStack_48;
                local_58 = fStack_48;
                local_54 = fVar7;
                fStack_48 = fVar7;
              }
            }
          }
        }
        pCVar16 = local_7c;
        if (local_80 != (int *)0x0) {
          piVar5 = local_80 + 1;
          *piVar5 = *piVar5 + -1;
          if (*piVar5 == 0) {
            (**(code **)(*local_80 + 4))();
            pCVar16 = local_7c;
          }
        }
      }
      bVar10 = CWorldMap::IsPosInMap(param_3,(C3DVector *)&local_64);
      if (bVar10) {
        piVar5 = *(int **)(param_1 + 0x70);
        if (piVar5 != (int *)0x0) {
          piVar5[1] = piVar5[1] + 1;
        }
        if (piVar5 != (int *)0x0) {
          piVar3 = piVar5 + 1;
          *piVar3 = *piVar3 + -1;
          if (*piVar3 == 0) {
            (**(code **)(*piVar5 + 4))();
          }
        }
        if (piVar5 != (int *)0x0) {
          (**(code **)(**(int **)(DAT_013b86a0 + 0x24) + 0x20))(&fStack_34);
          fVar7 = local_64 - fStack_34;
          piVar5 = *(int **)(param_1 + 0x70);
          fVar8 = local_60 - fStack_30;
          fVar9 = local_5c - fStack_2c;
          if (piVar5 != (int *)0x0) {
            piVar5[1] = piVar5[1] + 1;
          }
          fVar4 = (float)piVar5[0x12];
          piVar3 = piVar5 + 1;
          *piVar3 = *piVar3 + -1;
          if (*piVar3 == 0) {
            (**(code **)(*piVar5 + 4))();
          }
          if (fVar4 * fVar4 < fVar8 * fVar8 + fVar9 * fVar9 + fVar7 * fVar7) {
            if (local_7c == (CMaterialDef *)0x0) {
              return;
            }
            pCVar16 = local_7c + 4;
            *(int *)pCVar16 = *(int *)pCVar16 + -1;
            if (*(int *)pCVar16 != 0) {
              return;
            }
            (**(code **)(*(int *)local_7c + 4))();
            return;
          }
        }
        if (local_7c != (CMaterialDef *)0x0) {
          this_01 = (CDefinitionManager *)GFGetPreMainMemoryUsed();
          pCVar14 = CWorld::DrawGetEnvironment((CWorld *)param_3);
          pCVar15 = NInventory::CTCInventoryBase::GetCategoryWithIndex
                              ((CTCInventoryBase *)param_3,(long)pCVar14);
          piVar5 = *(int **)(pCVar15 + 0x24);
          if (piVar5 != (int *)0x0) {
            iVar6 = piVar5[1];
            piVar5[1] = iVar6 + 1;
            piVar5[1] = iVar6;
            if (iVar6 == 0) {
              (**(code **)(*piVar5 + 4))();
            }
          }
          lVar17 = 0;
          if (piVar5 != (int *)0x0) {
            this_02 = (CDefString *)(**(code **)(*piVar5 + 0x28))(auStack_68,&pCStack_78);
            CDefString::operator_class_CCharString(this_02);
            lVar17 = CDefinitionManager::GetDefGlobalIndexFromName
                               (this_01,(CCharString *)&pCStack_78);
            std::
            _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                      ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&pCStack_78,
                       extraout_EDX,
                       (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)unaff_EBX);
          }
          pCVar16 = local_7c;
          this_03 = (CThing *)0x0;
          pCStack_78 = (CThingBuilding *)0x0;
          local_80 = (int *)0x0;
          lStack_74 = 0;
          fStack_70 = 0.0;
          bVar10 = CMaterialDef::GetBlendedSwordHitEffect
                             (local_7c,lVar17,param_4,(long *)&local_80,&lStack_74,&fStack_70);
          if (bVar10) {
            if ((local_80 != (int *)0x0) && (lStack_74 != 0)) {
              this_03 = CTCDParticleEmitter::Create
                                  ((long)local_80,lStack_74,fStack_70,(C3DVector *)&local_64,false);
              pCStack_78 = (CThingBuilding *)this_03;
            }
          }
          else {
            bVar10 = IS_THING_ALIVE<CThingBuilding*>(&local_6c);
            if ((bVar10) && (((byte)*(CBaseObject *)(local_6c + 0x30) & 0x10) != 0)) {
              local_80 = (int *)CMaterialDef::GetRandomBerserkedSwordHitParticleEffect(pCVar16);
            }
            else {
              local_80 = (int *)CMaterialDef::GetRandomBerserkedSwordHitParticleEffect(pCVar16);
            }
            if (0 < (int)local_80) {
              this_03 = CTCDParticleEmitter::Create((long)local_80,(C3DVector *)&local_64,false);
              pCStack_78 = (CThingBuilding *)this_03;
            }
          }
          bVar10 = IS_THING_ALIVE<CThingBuilding*>(&pCStack_78);
          if (bVar10) {
            local_6c = (CThingBuilding *)0x0;
            CThing::GetTCDriver<CTCDExperienceOrb>(this_03,(CTCDExperienceOrb **)&local_6c);
            fStack_34 = (float)CCRC::Calc(0,"DIRECTION",9);
            fStack_2c = local_54;
            fStack_30 = local_58;
            uStack_28 = local_50;
            NParticleEngine::CParticleUpdateParameters<unsigned_long,unsigned_long>::
            CParticleUpdateParameters<unsigned_long,unsigned_long>(aCStack_24);
            FUN_0076b120(&fStack_34);
            CTCDParticleEmitter::SetEmitterUpdateParameters
                      ((CTCDParticleEmitter *)local_6c,aCStack_24);
            Catch_0040d88c();
          }
          pCVar2 = pCVar16 + 4;
          *(int *)pCVar2 = *(int *)pCVar2 + -1;
          if (*(int *)pCVar2 == 0) {
            (**(code **)(*(int *)pCVar16 + 4))();
          }
        }
      }
      else if (pCVar16 != (CMaterialDef *)0x0) {
        pCVar2 = pCVar16 + 4;
        *(int *)pCVar2 = *(int *)pCVar2 + -1;
        if (*(int *)pCVar2 == 0) {
          (**(code **)(*(int *)pCVar16 + 4))();
          return;
        }
      }
    }
  }
  return;
}



//=== ?OpenHouseDoors@CGameScriptInterface@@UBEXABVCScriptThing@@@Z @ 0088dfd0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::CloseHouseDoors(class CScriptThing const
   &)const  */

void __thiscall
CGameScriptInterface::_OpenHouseDoors_CGameScriptInterface__UBEXABVCScriptThing___Z
          (CGameScriptInterface *this,CScriptThing *param_1)

{
  char cVar1;
  CThingBuilding *this_00;
  
  cVar1 = (**(code **)(*(int *)param_1 + 300))();
  if (cVar1 != '\0') {
    this_00 = (CThingBuilding *)(**(code **)(*(int *)param_1 + 0x2c))();
    if (((byte)this_00[0x6c] & 0x40) != 0) {
      CThingBuilding::LockAllDoors(this_00,false,false);
    }
  }
  return;
}



//=== ?CloseHouseDoors@CGameScriptInterface@@UBEXABVCScriptThing@@@Z @ 0088e000 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::CloseHouseDoors(class CScriptThing const
   &)const  */

void __thiscall
CGameScriptInterface::_CloseHouseDoors_CGameScriptInterface__UBEXABVCScriptThing___Z
          (CGameScriptInterface *this,CScriptThing *param_1)

{
  char cVar1;
  CThingBuilding *this_00;
  
  cVar1 = (**(code **)(*(int *)param_1 + 300))();
  if (cVar1 != '\0') {
    this_00 = (CThingBuilding *)(**(code **)(*(int *)param_1 + 0x2c))();
    if (((byte)this_00[0x6c] & 0x40) != 0) {
      CThingBuilding::LockAllDoors(this_00,true,false);
    }
  }
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



//=== ?AddNewConversation@CGameScriptInterface@@UBEJABVCScriptThing@@_N1@Z @ 008906c0 ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: virtual long __thiscall CGameScriptInterface::AddNewConversation(class CScriptThing const
   &,bool,bool)const  */

long __thiscall
CGameScriptInterface::_AddNewConversation_CGameScriptInterface__UBEJABVCScriptThing___N1_Z
          (CGameScriptInterface *this,CScriptThing *param_1,bool param_2,bool param_3)

{
  CScriptConversationManager *this_00;
  char cVar1;
  CThing *pCVar2;
  long lVar3;
  
  cVar1 = (**(code **)(*(int *)param_1 + 300))();
  if (cVar1 != '\0') {
    this_00 = *(CScriptConversationManager **)(*(int *)(this + 4) + 0x7c);
    pCVar2 = (CThing *)(**(code **)(*(int *)param_1 + 0x2c))();
    lVar3 = CScriptConversationManager::MakeNewConversation(this_00,pCVar2,param_2,param_3);
    return lVar3;
  }
  return 0;
}



//=== ?AddPersonToConversation@CGameScriptInterface@@UBEXJABVCScriptThing@@@Z @ 00890710 ===

/* [bsim sim=1.0 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::AddPersonToConversation(long,class
   CScriptThing const &)const  */

void __thiscall
CGameScriptInterface::_AddPersonToConversation_CGameScriptInterface__UBEXJABVCScriptThing___Z
          (CGameScriptInterface *this,long param_1,CScriptThing *param_2)

{
  CScriptConversationManager *this_00;
  char cVar1;
  CThing *pCVar2;
  
  cVar1 = (**(code **)(*(int *)param_2 + 300))();
  if (cVar1 != '\0') {
    this_00 = *(CScriptConversationManager **)(*(int *)(this + 4) + 0x7c);
    pCVar2 = (CThing *)(**(code **)(*(int *)param_2 + 0x2c))();
    CScriptConversationManager::AddPersonToConversation(this_00,param_1,pCVar2);
  }
  return;
}



//=== ?AddLineToConversation@CGameScriptInterface@@UBEXJABVCCharString@@_NABVCScriptThing@@2@Z @ 00890750 ===

void _AddLineToConversation_CGameScriptInterface__UBEXJABVCCharString___NABVCScriptThing__2_Z
               (long param_1,CCharString *param_2,undefined1 param_3,int *param_4,int *param_5)

{
  CScriptConversationManager *this;
  char cVar1;
  CThing *pCVar2;
  int in_ECX;
  CThing *pCVar3;
  
  cVar1 = (**(code **)(*param_4 + 300))();
  if (cVar1 != '\0') {
    pCVar3 = (CThing *)0x0;
    cVar1 = (**(code **)(*param_5 + 300))();
    if (cVar1 != '\0') {
      pCVar3 = (CThing *)(**(code **)(*param_5 + 0x2c))();
    }
    this = *(CScriptConversationManager **)(*(int *)(in_ECX + 4) + 0x7c);
    pCVar2 = (CThing *)(**(code **)(*param_4 + 0x2c))();
    CScriptConversationManager::AddLineToConversation
              (this,param_1,pCVar2,pCVar3,param_2,(bool)param_3);
  }
  return;
}



//=== ?JamDoor@CGameScriptInterface@@UBEXABVCScriptThing@@@Z @ 00895e10 ===

/* [bsim sim=0.6276327465828244 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::EntitySetShotAccuracyPercentage(class
   CScriptThing const &,long)const  */

void __thiscall
CGameScriptInterface::_JamDoor_CGameScriptInterface__UBEXABVCScriptThing___Z
          (CGameScriptInterface *this,CScriptThing *param_1,long param_2)

{
  CScriptThing *pCVar1;
  char cVar2;
  int iVar3;
  int *piVar4;
  
  pCVar1 = param_1;
  cVar2 = (**(code **)(*(int *)param_1 + 300))();
  if (cVar2 != '\0') {
    iVar3 = (**(code **)(*(int *)pCVar1 + 0x2c))();
    if ((*(byte *)(iVar3 + 0x24) & 0x40) != 0) {
      param_1 = (CScriptThing *)&DAT_00000026;
      piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(iVar3 + 0x44),(ETCInterfaceType *)&param_1);
      if ((piVar4 == *(int **)(iVar3 + 0x48)) || (0x26 < *piVar4)) {
        piVar4 = *(int **)(iVar3 + 0x48);
      }
      *(undefined4 *)(piVar4[1] + 0xc) = 1;
    }
  }
  return;
}



//=== ?SetHouseOwnedByPlayer@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z @ 00895ed0 ===

void _SetHouseOwnedByPlayer_CGameScriptInterface__UBEXABVCScriptThing___N1_Z
               (int *param_1,ETCInterfaceType param_2,undefined4 param_3)

{
  CTCBuyableHouse *this;
  int iVar1;
  int iVar2;
  int *piVar3;
  int extraout_EAX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EBX;
  undefined8 uVar4;
  bool bVar5;
  
  iVar2 = (**(code **)(*param_1 + 0x2c))();
  if (((iVar2 != 0) && ((*(byte *)(iVar2 + 0x91) & 1) == 0)) &&
     ((*(uint *)(iVar2 + 0x38) & 0x200000) != 0)) {
    param_1 = (int *)0xd5;
    piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar2 + 0x44),(ETCInterfaceType *)&param_1);
    if ((piVar3 == *(int **)(iVar2 + 0x48)) || (0xd5 < *piVar3)) {
      piVar3 = *(int **)(iVar2 + 0x48);
    }
    this = (CTCBuyableHouse *)piVar3[1];
    if (this != (CTCBuyableHouse *)0x0) {
      bVar5 = SUB41(param_2,0);
      CTCBuyableHouse::SetOwnedByPlayer(this,bVar5);
      CTCBuyableHouse::SetRented(this,SUB41(param_3,0));
      uVar4 = Catch_0040d959();
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)uVar4,
                 (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar4 >> 0x20),
                 unaff_EBX);
      if (((extraout_EAX != 0) && ((*(byte *)(extraout_EAX + 0x91) & 1) == 0)) &&
         ((*(byte *)(extraout_EAX + 0x20) & 0x10) != 0)) {
        param_2 = 4;
        piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                        ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                      *)(extraout_EAX + 0x44),&param_2);
        if ((piVar3 == *(int **)(extraout_EAX + 0x48)) || (4 < *piVar3)) {
          piVar3 = *(int **)(extraout_EAX + 0x48);
        }
        iVar1 = piVar3[1];
        if ((*(uint *)(iVar2 + 0x28) & 0x80000) != 0) {
          if (bVar5 == false) {
            *(int *)(iVar1 + 0xe0) = *(int *)(iVar1 + 0xe0) + -1;
            return;
          }
          *(int *)(iVar1 + 0xe0) = *(int *)(iVar1 + 0xe0) + 1;
          return;
        }
        if (bVar5 != false) {
          *(int *)(iVar1 + 0xe4) = *(int *)(iVar1 + 0xe4) + 1;
          return;
        }
        *(int *)(iVar1 + 0xe4) = *(int *)(iVar1 + 0xe4) + -1;
      }
    }
  }
  return;
}



//=== ?SetBuyableHouseAsScripted@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z @ 00896000 ===

void _SetBuyableHouseAsScripted_CGameScriptInterface__UBEXABVCScriptThing___N_Z
               (int *param_1,undefined1 param_2)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = (**(code **)(*param_1 + 0x2c))();
  if (((iVar1 != 0) && ((*(byte *)(iVar1 + 0x91) & 1) == 0)) &&
     ((*(uint *)(iVar1 + 0x38) & 0x200000) != 0)) {
    param_1 = (int *)0xd5;
    piVar2 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(iVar1 + 0x44),(ETCInterfaceType *)&param_1);
    if ((piVar2 == *(int **)(iVar1 + 0x48)) || (0xd5 < *piVar2)) {
      piVar2 = *(int **)(iVar1 + 0x48);
    }
    if (piVar2[1] != 0) {
      *(undefined1 *)(piVar2[1] + 0x10) = param_2;
    }
  }
  return;
}



//=== ?CreateEffect@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABV2@00_N2@Z @ 0089f910 ===

CBase * _CreateEffect_CGameScriptInterface__UBE_AVCScriptThing__ABVCCharString__ABV2_00_N2_Z
                  (CBase *param_1,CCharString *param_2,int *param_3,CCharString *param_4,
                  undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  CTCDParticleEmitter *this;
  char cVar1;
  CVertexBufferWin32 *this_00;
  C3DVector *pCVar2;
  CThing *pCVar3;
  int *piVar4;
  CIsThingAlive *extraout_EDX;
  CIsThingAlive *extraout_EDX_00;
  CIsThingAlive *pCVar5;
  bool bVar6;
  
  piVar4 = param_3;
  cVar1 = (**(code **)(*param_3 + 300))();
  if (cVar1 != '\0') {
    this_00 = (CVertexBufferWin32 *)(**(code **)(*piVar4 + 0x2c))();
    bVar6 = false;
    pCVar2 = (C3DVector *)CVertexBufferWin32::DoSizeof(this_00);
    pCVar3 = CTCDParticleEmitter::Create(param_2,pCVar2,bVar6);
    if (pCVar3 != (CThing *)0x0) {
      pCVar5 = extraout_EDX;
      if ((*(uint *)(pCVar3 + 0x28) & 0x800) != 0) {
        param_3 = (int *)0x4b;
        piVar4 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                        ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                      *)(pCVar3 + 0x44),(ETCInterfaceType *)&param_3);
        if ((piVar4 == *(int **)(pCVar3 + 0x48)) || (0x4b < *piVar4)) {
          piVar4 = *(int **)(pCVar3 + 0x48);
        }
        this = (CTCDParticleEmitter *)piVar4[1];
        CTCDParticleEmitter::AttachToThing(this,(CThing *)this_00,10,param_4,0,0.0);
        CTCDParticleEmitter::SetAlwaysUpdate(this,SUB41(param_7,0));
        pCVar5 = extraout_EDX_00;
      }
      NScript::
      GFPredicateAnd<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)pCVar3,pCVar5);
      return param_1;
    }
  }
  NHeroInformationScreens::CBase::CBase(param_1);
  *(undefined ***)param_1 = &PTR__scalar_deleting_destructor__01238c8c;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return param_1;
}



//=== ?CreateEffect@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABVC3DVector@@0M_N2@Z @ 0089f9e0 ===

CBase * _CreateEffect_CGameScriptInterface__UBE_AVCScriptThing__ABVCCharString__ABVC3DVector__0M_N2_Z
                  (CBase *param_1,CCharString *param_2,C3DVector *param_3,undefined4 param_4,
                  undefined4 param_5,undefined4 param_6,bool param_7)

{
  CTCDParticleEmitter *this;
  CThing *pCVar1;
  CIsThingAlive *extraout_EDX;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  pCVar1 = CTCDParticleEmitter::Create(param_2,param_3,false);
  if ((pCVar1 != (CThing *)0x0) && ((*(uint *)(pCVar1 + 0x28) & 0x800) != 0)) {
    this = *(CTCDParticleEmitter **)(pCVar1 + 0x5c);
    uStack_18 = 0;
    uStack_14 = 0;
    uStack_10 = 0x3f800000;
    uStack_c = 0;
    uStack_8 = 0x3f800000;
    uStack_4 = 0;
    FUN_00a22f80(&uStack_18,param_5);
    CTCDParticleEmitter::SetEmitterOrientation(this,(CRightHandedSet *)&uStack_18);
    CTCDParticleEmitter::SetAlwaysUpdate(this,param_7);
    NScript::
    GFPredicateAnd<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar1,extraout_EDX);
    return param_1;
  }
  NHeroInformationScreens::CBase::CBase(param_1);
  *(undefined ***)param_1 = &PTR__scalar_deleting_destructor__01238c8c;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined4 *)(param_1 + 8) = 0;
  return param_1;
}



//=== ?UpdateOnlineScore_Archery@CGameScriptInterface@@UBEXM@Z @ 008a1040 ===

void _UpdateOnlineScore_Archery_CGameScriptInterface__UBEXM_Z
               (undefined4 param_1,CTCHeroOnlineScoreboard *param_2)

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
  if ((*(uint *)(extraout_EAX + 0x40) & 0x2000) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x10d < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    CTCHeroOnlineScoreboard::UpdateScoreForActivity
              ((CTCHeroOnlineScoreboard *)piVar1[1],8,(float)param_2,false);
    return;
  }
  CTCHeroOnlineScoreboard::UpdateScoreForActivity(param_2,8,(float)param_2,false);
  return;
}



//=== ?UpdateOnlineScore_ChickenKick@CGameScriptInterface@@UBEXM@Z @ 008a10c0 ===

void _UpdateOnlineScore_ChickenKick_CGameScriptInterface__UBEXM_Z
               (undefined4 param_1,CTCHeroOnlineScoreboard *param_2)

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
  if ((*(uint *)(extraout_EAX + 0x40) & 0x2000) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x10d < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    CTCHeroOnlineScoreboard::UpdateScoreForActivity
              ((CTCHeroOnlineScoreboard *)piVar1[1],0x11,(float)param_2,false);
    return;
  }
  CTCHeroOnlineScoreboard::UpdateScoreForActivity(param_2,0x11,(float)param_2,false);
  return;
}



//=== ?UpdateOnlineScore_ChapelOrTemple@CGameScriptInterface@@UBEXM@Z @ 008a1140 ===

void _UpdateOnlineScore_ChapelOrTemple_CGameScriptInterface__UBEXM_Z
               (undefined4 param_1,CTCHeroOnlineScoreboard *param_2)

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
  if ((*(uint *)(extraout_EAX + 0x40) & 0x2000) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x10d < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    CTCHeroOnlineScoreboard::UpdateScoreForActivity
              ((CTCHeroOnlineScoreboard *)piVar1[1],10,(float)param_2,false);
    return;
  }
  CTCHeroOnlineScoreboard::UpdateScoreForActivity(param_2,10,(float)param_2,false);
  return;
}



//=== ?UpdateOnlineScore_FishingCompo@CGameScriptInterface@@UBEXM@Z @ 008a11c0 ===

void _UpdateOnlineScore_FishingCompo_CGameScriptInterface__UBEXM_Z
               (undefined4 param_1,CTCHeroOnlineScoreboard *param_2)

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
  if ((*(uint *)(extraout_EAX + 0x40) & 0x2000) != 0) {
    piVar1 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                    ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                  *)(extraout_EAX + 0x44),(ETCInterfaceType *)&stack0x00000000);
    if ((piVar1 == *(int **)(extraout_EAX + 0x48)) || (0x10d < *piVar1)) {
      piVar1 = *(int **)(extraout_EAX + 0x48);
    }
    CTCHeroOnlineScoreboard::UpdateScoreForActivity
              ((CTCHeroOnlineScoreboard *)piVar1[1],0x26,(float)param_2,false);
    return;
  }
  CTCHeroOnlineScoreboard::UpdateScoreForActivity(param_2,0x26,(float)param_2,false);
  return;
}



//=== ?UpdateScore_FishingCompetition@CGameScriptInterface@@UBEXM@Z @ 008a1240 ===

void _UpdateScore_FishingCompetition_CGameScriptInterface__UBEXM_Z
               (undefined4 param_1,undefined4 param_2)

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
    *(undefined4 *)(piVar1[1] + 0x15c) = param_2;
  }
  return;
}



//=== ?GetAllThingsInLevel@CGameScriptInterface@@UBEJABVCCharString@@AAV?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@@Z @ 008a8af0 ===

/* [bsim sim=0.7298819294206376 <- ego_r]
   public: virtual long __thiscall CGameScriptInterface::GetAllThingsInLevel(class CCharString const
   &,class std::vector<class CScriptThing,class std::allocator<class CScriptThing> > &)const  */

long __thiscall
CGameScriptInterface::
_GetAllThingsInLevel_CGameScriptInterface__UBEJABVCCharString__AAV__vector_VCScriptThing__V__allocator_VCScriptThing___std___std___Z
          (CGameScriptInterface *this,CCharString *param_1,
          vector<CScriptThing,std::allocator<CScriptThing>_> *param_2)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined4 *_Memory;
  char cVar4;
  CWorldMap *this_00;
  int *piVar5;
  byte bVar6;
  CIsThingAlive *extraout_EDX;
  CIsThingAlive *extraout_EDX_00;
  undefined4 *puVar8;
  CCharString *pCVar9;
  int *piStack_18;
  int *piStack_14;
  undefined4 *puStack_10;
  undefined4 *puStack_c;
  undefined4 uStack_8;
  CIsThingAlive *pCVar7;
  
  pCVar9 = param_1;
  this_00 = (CWorldMap *)(**(code **)(**(int **)(this + 4) + 0x30))();
  piStack_18 = (int *)CWorldMap::GetMapNumberFromScriptName(this_00,pCVar9);
  piVar5 = (int *)(**(code **)(**(int **)(this + 4) + 0x30))();
  (**(code **)(*piVar5 + 0x28))(piStack_18);
  puStack_10 = (undefined4 *)0x0;
  puStack_c = (undefined4 *)0x0;
  uStack_8 = 0;
  (**(code **)(**(int **)(this + 4) + 0x30))();
  CThingSearchTools::
  GetAllThings<CThing,std::vector<CThing*,std::allocator<CThing*>_>,CFilter_Bind2<CThingFilter_InMap,std::unary_negate<CThingFilter_InArea>_>_>
            (*(CThingSearchTools **)(*(int *)(this + 4) + 0x20),
             (CFilter_Bind2<CThingFilter_InMap,std::unary_negate<CThingFilter_InArea>_> *)
             &stack0xffffffe4,(vector<CThing*,std::allocator<CThing*>_> *)&puStack_10);
  _Memory = puStack_10;
  pCVar7 = extraout_EDX;
  puVar8 = puStack_10;
  if (puStack_10 != puStack_c) {
    do {
      bVar6 = ~(byte)((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                       *)*puVar8)[0x91];
      pCVar7 = (CIsThingAlive *)CONCAT31((int3)((uint)pCVar7 >> 8),bVar6);
      if ((bVar6 & 1) != 0) {
        NScript::
        GFPredicateAnd<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                    *)*puVar8,pCVar7);
        if ((piStack_18 != (int *)0x0) &&
           (cVar4 = (**(code **)(*piStack_18 + 0x134))(), cVar4 == '\0')) {
          puVar1 = *(undefined4 **)(param_1 + 4);
          if (puVar1 == *(undefined4 **)(param_1 + 8)) {
            FUN_008ad5f0(puVar1,&stack0xffffffe4,&stack0x00000000,1,1);
          }
          else {
            if (puVar1 != (undefined4 *)0x0) {
              *puVar1 = &PTR__scalar_deleting_destructor__01238c8c;
              puVar1[1] = piStack_18;
              puVar1[2] = piStack_14;
              if (piStack_14 != (int *)0x0) {
                *piStack_14 = *piStack_14 + 1;
              }
            }
            *(int *)(param_1 + 4) = *(int *)(param_1 + 4) + 0xc;
          }
        }
        if ((piStack_14 != (int *)0x0) && (*piStack_14 = *piStack_14 + -1, *piStack_14 == 0)) {
          (*(code *)piStack_14[1])();
          operator_delete(piStack_14);
        }
        piStack_18 = (int *)0x0;
        piStack_14 = (int *)0x0;
        NHeroInformationScreens::CBase::CBase((CBase *)&stack0xffffffe4);
        pCVar7 = extraout_EDX_00;
      }
      puVar8 = puVar8 + 1;
    } while (puVar8 != puStack_c);
  }
  iVar2 = *(int *)param_1;
  iVar3 = *(int *)(param_1 + 4);
  if (_Memory != (undefined4 *)0x0) {
    free(_Memory);
  }
  return (iVar3 - iVar2) / 0xc;
}



//=== CreateEffect @ 008f7e70 ===

/* [bsim sim=0.8290156007883394 <- ego_r]
   void __fastcall NCombatActionHelpers::CreateEffect(class CThingCreatureBase &,long,class
   C3DVector const &,class CCharString const *) */

void __fastcall
NCombatActionHelpers::CreateEffect
          (CThingCreatureBase *param_1,long param_2,C3DVector *param_3,CCharString *param_4)

{
  C3DVector *pCVar1;
  CThing *pCVar2;
  CTCDParticleEmitter *this;
  bool bVar3;
  ulong local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined **local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined1 local_13;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined1 local_3;
  
  if (param_2 != 0) {
    bVar3 = false;
    pCVar1 = (C3DVector *)CVertexBufferWin32::DoSizeof((CVertexBufferWin32 *)param_1);
    pCVar2 = CTCDParticleEmitter::Create(param_2,pCVar1,bVar3);
    if (pCVar2 != (CThing *)0x0) {
      this = (CTCDParticleEmitter *)0x0;
      if ((*(uint *)(pCVar2 + 0x28) & 0x800) != 0) {
        this = *(CTCDParticleEmitter **)(pCVar2 + 0x5c);
      }
      if (param_4 != (CCharString *)0x0) {
        CTCDParticleEmitter::AttachToThing(this,(CThing *)param_1,0xc,param_4,0,0.0);
      }
      local_34 = CCRC::Calc(0,"DIRECTION",9);
      local_30 = *(undefined4 *)param_3;
      local_2c = *(undefined4 *)(param_3 + 4);
      local_28 = *(undefined4 *)(param_3 + 8);
      NHeroInformationScreens::CBase::CBase((CBase *)&local_24);
      local_24 = &PTR__vector_deleting_destructor__0125ffdc;
      local_20 = 0;
      local_1c = 0;
      local_18 = 0;
      local_13 = 0;
      local_10 = 0;
      local_c = 0;
      local_8 = 0;
      local_3 = 0;
      FUN_0076b120(&local_34);
      CTCDParticleEmitter::SetEmitterUpdateParameters
                (this,(CParticleUpdateParameters<unsigned_long,unsigned_long> *)&local_24);
      Catch_0040d88c();
    }
  }
  return;
}



//=== CreateEffect @ 008f7f50 ===

/* [bsim sim=0.7868477693374983 <- ego_r]
   void __fastcall NCombatActionHelpers::CreateEffect(class CThingCreatureBase const &,long,class
   C3DVector const &,class C3DVector const &) */

void __fastcall
NCombatActionHelpers::CreateEffect
          (CThingCreatureBase *param_1,long param_2,C3DVector *param_3,C3DVector *param_4)

{
  CThing *pCVar1;
  CTCDParticleEmitter *this;
  ulong local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined **local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined1 local_13;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined1 local_3;
  
  if (param_2 != 0) {
    pCVar1 = CTCDParticleEmitter::Create(param_2,param_4,false);
    if (pCVar1 != (CThing *)0x0) {
      this = (CTCDParticleEmitter *)0x0;
      if ((*(uint *)(pCVar1 + 0x28) & 0x800) != 0) {
        this = *(CTCDParticleEmitter **)(pCVar1 + 0x5c);
      }
      local_34 = CCRC::Calc(0,"DIRECTION",9);
      local_30 = *(undefined4 *)param_3;
      local_2c = *(undefined4 *)(param_3 + 4);
      local_28 = *(undefined4 *)(param_3 + 8);
      NHeroInformationScreens::CBase::CBase((CBase *)&local_24);
      local_24 = &PTR__vector_deleting_destructor__0125ffdc;
      local_20 = 0;
      local_1c = 0;
      local_18 = 0;
      local_13 = 0;
      local_10 = 0;
      local_c = 0;
      local_8 = 0;
      local_3 = 0;
      FUN_0076b120(&local_34);
      CTCDParticleEmitter::SetEmitterUpdateParameters
                (this,(CParticleUpdateParameters<unsigned_long,unsigned_long> *)&local_24);
      Catch_0040d88c();
    }
  }
  return;
}



