//=== ?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z @ 004aac60 (seed 0x004AAC60) ===

uint _MsgIsPresentedWithItem_CScriptThing__UBE_NAAVCCharString___Z(void)

{
  uint in_EAX;
  uint uVar1;
  int in_ECX;
  
  if (*(int **)(in_ECX + 4) == (int *)0x0) {
    return in_EAX & 0xffffff00;
  }
                    /* WARNING: Could not recover jumptable at 0x004aac6e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(**(int **)(in_ECX + 4) + 0x8c))();
  return uVar1;
}


// CALLEES of ?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z:

//=== ?MsgIsPresentedWithItem@CGameScriptThing@@UBE_NAAVCCharString@@@Z @ 008d23c0 (seed 0x008D23C0) ===

/* [bsim sim=0.8316616986994059 <- ego_r]
   public: virtual bool __thiscall CGameScriptThing::MsgIsPresentedWithItem(class CCharString
   &)const  */

bool __thiscall
CGameScriptThing::_MsgIsPresentedWithItem_CGameScriptThing__UBE_NAAVCCharString___Z
          (CGameScriptThing *this,CCharString *param_1)

{
  CBaseIntelligentPointer *this_00;
  int iVar1;
  CBaseObject *pCVar2;
  int *piVar3;
  int iVar4;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  int iVar5;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  int *piVar6;
  int local_14;
  CCharString aCStack_10 [4];
  ETCInterfaceType local_c [3];
  
  this_00 = (CBaseIntelligentPointer *)(this + 0xc);
  pCVar2 = CBaseIntelligentPointer::GetPItem(this_00);
  if ((pCVar2 != (CBaseObject *)0x0) &&
     (pCVar2 = CBaseIntelligentPointer::GetPItem(this_00), ((byte)pCVar2[0x91] & 1) == 0)) {
    iVar1 = *(int *)(DAT_013b89fc + 0x88);
    if ((iVar1 == 0) || (*(int *)(iVar1 + 8) == 0)) {
      iVar5 = 0;
    }
    else {
      iVar5 = *(int *)(*(int *)(*(int *)(iVar1 + 8) + 0x2c) + 0x18);
    }
    if ((iVar1 == 0) || (*(int *)(iVar1 + 8) == 0)) {
      local_14 = 0;
    }
    else {
      local_14 = *(int *)(*(int *)(*(int *)(iVar1 + 8) + 0x2c) + 0x1c);
    }
    pCVar2 = CBaseIntelligentPointer::GetPItem(this_00);
    if (((byte)pCVar2[0x28] & 4) != 0) {
      local_c[0] = 0x42;
      piVar3 = (int *)CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                      ::LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                                    *)(pCVar2 + 0x44),local_c);
      if ((piVar3 == *(int **)(pCVar2 + 0x48)) || (0x42 < *piVar3)) {
        piVar3 = *(int **)(pCVar2 + 0x48);
      }
      iVar1 = piVar3[1];
      piVar3 = *(int **)(iVar1 + 0xc);
      piVar6 = (int *)*piVar3;
      if (piVar6 != piVar3) {
        do {
          piVar3 = (int *)piVar6[2];
          if ((iVar5 < piVar3[1]) && (piVar3[1] <= local_14)) {
            iVar4 = (**(code **)(*piVar3 + 4))();
            if (iVar4 == 10) {
              pCVar2 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(piVar3 + 4));
              CThing::GetDefName((CThing *)pCVar2);
              CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)local_c);
              CCharString::operator=(param_1,(CCharString *)local_c);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_c,
                         extraout_EDX_00,unaff_EDI);
              return true;
            }
            if (iVar4 == 0xb) {
              pCVar2 = CBaseIntelligentPointer::GetPItem((CBaseIntelligentPointer *)(piVar3 + 4));
              CThing::GetDefName((CThing *)pCVar2);
              CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)aCStack_10);
              CCharString::operator=(param_1,aCStack_10);
              std::
              _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                        ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_10
                         ,extraout_EDX,unaff_EDI);
              return true;
            }
          }
          piVar6 = (int *)*piVar6;
        } while (piVar6 != (int *)*(int *)(iVar1 + 0xc));
      }
    }
  }
  return false;
}


// CALLEES of ?MsgIsPresentedWithItem@CGameScriptThing@@UBE_NAAVCCharString@@@Z:
//   009d49b0  GetString
//   0099efb0  operator=
//   004c7cc0  GetDefName
//   0099eae0  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
//   00a01b50  GetPItem
//   004365b0  LowerBound

