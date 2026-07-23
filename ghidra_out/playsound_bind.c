//=== ?PlaySoundAtPos@CGameScriptInterface@@UBEKABVC3DVector@@ABVCCharString@@@Z @ 00891170 ===

EAIStateGroupType
_PlaySoundAtPos_CGameScriptInterface__UBEKABVC3DVector__ABVCCharString___Z
          (undefined4 param_1,CCharString *param_2)

{
  long lVar1;
  CAIBrain *this;
  EAIStateGroupType EVar2;
  
  lVar1 = CASoundBank::GetSoundIndexFromSymbol(*(CASoundBank **)(DAT_013b86a0 + 0x10),param_2);
  if (lVar1 != 0) {
    this = (CAIBrain *)FUN_008369f0(1);
    if ((this != (CAIBrain *)0x0) && (((byte)this[0x91] & 1) == 0)) {
      EVar2 = CAIBrain::GetCurrentStateGroupType(this);
      return EVar2;
    }
  }
  return 0xffffffff;
}



//=== ?PlayCriteriaSoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z @ 008a09b0 ===

/* [bsim sim=0.6904252350342439 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::SetRegionEntranceAsActive(class
   CScriptThing const &,bool)const  */

void __thiscall
CGameScriptInterface::
_PlayCriteriaSoundOnThing_CGameScriptInterface__UBEKABVCScriptThing__ABVCCharString___Z
          (CGameScriptInterface *this,CScriptThing *param_1,bool param_2)

{
  CScriptThing *pCVar1;
  char cVar2;
  int iVar3;
  undefined3 in_stack_00000009;
  
  pCVar1 = param_1;
  cVar2 = (**(code **)(*(int *)param_1 + 300))();
  if ((cVar2 != '\0') &&
     (iVar3 = (**(code **)(*(int *)pCVar1 + 0x2c))(), (*(uint *)(iVar3 + 0x20) & 0x10000) != 0)) {
    param_1 = (CScriptThing *)&DAT_00000010;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
    LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                *)(iVar3 + 0x44),(ETCInterfaceType *)&param_1);
    FUN_0066bd00(_param_2,1,0,0,0xffffffff);
    return;
  }
  return;
}



//=== ?PlaySoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z @ 008a0a20 ===

/* [bsim sim=0.6909173094247147 <- ego_r]
   public: virtual void __thiscall CGameScriptInterface::RemoveItemFromContainer(class CScriptThing
   const &,class CCharString const &)const  */

void __thiscall
CGameScriptInterface::
_PlaySoundOnThing_CGameScriptInterface__UBEKABVCScriptThing__ABVCCharString___Z
          (CGameScriptInterface *this,CScriptThing *param_1,CCharString *param_2)

{
  CScriptThing *pCVar1;
  char cVar2;
  long lVar3;
  int iVar4;
  long lStack_28;
  long lStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  pCVar1 = param_1;
  cVar2 = (**(code **)(*(int *)param_1 + 300))();
  if (((cVar2 != '\0') &&
      (lVar3 = CASoundBank::GetSoundIndexFromSymbol(*(CASoundBank **)(DAT_013b86a0 + 0x10),param_2),
      lVar3 != 0)) &&
     (iVar4 = (**(code **)(*(int *)pCVar1 + 0x2c))(), (*(uint *)(iVar4 + 0x20) & 0x10000) != 0)) {
    param_1 = (CScriptThing *)&DAT_00000010;
    CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>::
    LowerBound((CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*>_>
                *)(iVar4 + 0x44),(ETCInterfaceType *)&param_1);
    uStack_14 = 1;
    uStack_10 = 1;
    uStack_20 = 0x3f800000;
    uStack_1c = 0x3f800000;
    uStack_18 = 0;
    uStack_8 = 0;
    uStack_4 = 0xffffffff;
    uStack_c = 0x8010;
    lStack_28 = lVar3;
    lStack_24 = lVar3;
    FUN_0066b280(&lStack_28,&uStack_20,1,0,0);
    return;
  }
  return;
}



//=== GetSoundIndexFromSymbol @ 00a01920 ===

/* [bsim sim=1.0 <- ego_r]
   public: long __thiscall CASoundBank::GetSoundIndexFromSymbol(class CCharString const &)const  */

long __thiscall CASoundBank::GetSoundIndexFromSymbol(CASoundBank *this,CCharString *param_1)

{
  long lVar1;
  long local_4;
  
  local_4 = 0;
  lVar1 = 0;
  if (*(int *)(this + 4) != 0) {
    FUN_00a38420(param_1,&local_4);
    lVar1 = local_4;
  }
  return lVar1;
}



