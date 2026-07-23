//=== ?PlaySoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z @ 008a0a20 (seed 0x008a0a20) ===

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


// CALLEES of ?PlaySoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z:
//   00a01920  GetSoundIndexFromSymbol
//   0040f020  LowerBound
//   0066b280  FUN_0066b280

//=== ?PlayCriteriaSoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z @ 008a09b0 (seed 0x008a09b0) ===

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


// CALLEES of ?PlayCriteriaSoundOnThing@CGameScriptInterface@@UBEKABVCScriptThing@@ABVCCharString@@@Z:
//   0066bd00  FUN_0066bd00
//   0040f020  LowerBound

