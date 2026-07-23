//=== FUN_0066b080 @ 0066b080 (seed 0x0066b080) ===

EAIStateGroupType FUN_0066b080(undefined4 param_1,int param_2,undefined4 *param_3,bool param_4)

{
  CThingSoundEmitter *this;
  EAIStateGroupType EVar1;
  CTCSoundPlayer *in_ECX;
  int iVar2;
  undefined4 *puVar3;
  undefined4 local_20 [5];
  undefined4 local_c;
  
  if ((param_2 != 0) && (DAT_013b8394 != 0)) {
    this = CTCSoundPlayer::CreatePAttachedEmitter(in_ECX,param_4);
    if (this != (CThingSoundEmitter *)0x0) {
      if (param_3[5] == 0) {
        puVar3 = local_20;
        for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
          *puVar3 = *param_3;
          param_3 = param_3 + 1;
          puVar3 = puVar3 + 1;
        }
        local_c = 0x8010;
        EVar1 = CAIBrain::GetCurrentStateGroupType((CAIBrain *)this);
        return EVar1;
      }
      EVar1 = CAIBrain::GetCurrentStateGroupType((CAIBrain *)this);
      return EVar1;
    }
  }
  return 0xffffffff;
}


// CALLEES of FUN_0066b080:
//   00836810  GetCurrentStateGroupType
//   0066aef0  CreatePAttachedEmitter

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
//   004c7a80  GetMeshBank
//   00497890  GFROR13
//   0066b080  FUN_0066b080
//   00686d60  GetDisplayEngine

//=== FUN_0066bd00 @ 0066bd00 (seed 0x0066bd00) ===

int FUN_0066bd00(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                undefined4 param_5)

{
  CMeshDataBank *pCVar1;
  int in_ECX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *paVar3;
  undefined8 uVar4;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *local_c;
  allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *local_8;
  undefined4 local_4;
  
  if ((DAT_0137e974 != '\0') && (DAT_013b8394 != 0)) {
    pCVar1 = CAnimateThing::GetMeshBank(*(CAnimateThing **)(in_ECX + 4));
    local_c = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
    local_8 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0;
    local_4 = 0;
    FUN_0066bc10(param_1,&local_c);
    paVar3 = local_c;
    if (local_c != local_8) {
      do {
        uVar4 = FUN_0066b120(pCVar1,paVar3,param_2,param_3,param_4,param_5);
        ppVar2 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar4 >> 0x20);
        if ((int)uVar4 != -1) {
          CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&local_c);
          return (int)uVar4;
        }
        paVar3 = paVar3 + 4;
      } while (paVar3 != local_8);
      paVar3 = local_c;
      if (local_c != local_8) {
        do {
          std::
          _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                    (paVar3,ppVar2,unaff_EDI);
          paVar3 = paVar3 + 4;
          ppVar2 = extraout_EDX;
        } while (paVar3 != local_8);
      }
    }
    if (local_c != (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)0x0) {
      free(local_c);
    }
  }
  return -1;
}


// CALLEES of FUN_0066bd00:
//   004c7a80  GetMeshBank
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   0066bc10  FUN_0066bc10
//   00414ea0  ResetRings
//   00bfea14  free
//   0066b120  FUN_0066b120

