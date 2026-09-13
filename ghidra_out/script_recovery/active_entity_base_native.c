//=== CActiveEntityScriptBase @ 00ce1110 (seed 0x00CE1110) ===

/* [bsim sim=0.8814093276045902 <- ego_r]
   public: __thiscall CActiveEntityScriptBase::CActiveEntityScriptBase(class CScriptThing const
   &,unsigned __int64,unsigned long) */

CActiveEntityScriptBase * __thiscall
CActiveEntityScriptBase::CActiveEntityScriptBase
          (CActiveEntityScriptBase *this,CScriptThing *param_1,__uint64 param_2,ulong param_3)

{
  int *piVar1;
  CCharString *pCVar2;
  undefined4 in_stack_00000008;
  
  pCVar2 = (CCharString *)(**(code **)(*(int *)param_1 + 4))();
  CCreatureAction_CastBattleCharge__CCreatureAction_CastBattleCharge(1,0x40,0x3dcccccd);
  *(undefined ***)this = &PTR__vector_deleting_destructor__012c2f9c;
  *(undefined4 *)(this + 0x14) = 1;
  *(undefined4 *)(this + 0x18) = 0xffffffff;
  *(undefined4 *)(this + 0x1c) = 0xffffffff;
  CCharString::CCharString((CCharString *)(this + 0x20),pCVar2);
  CCharString::CCharString((CCharString *)(this + 0x24));
  this[0x28] = (CActiveEntityScriptBase)0x0;
  this[0x29] = (CActiveEntityScriptBase)0x0;
  *(undefined4 *)(this + 0x2c) = 0;
  *(undefined4 *)(this + 0x30) = 0;
  *(undefined ***)this = &PTR__vector_deleting_destructor__012c3594;
  *(undefined4 *)(this + 0x34) = 0;
  *(undefined4 *)(this + 0x38) = 0;
  *(undefined4 *)(this + 0x3c) = param_2._4_4_;
  *(undefined4 *)(this + 0x40) = in_stack_00000008;
  *(undefined4 *)(this + 0x44) = (undefined4)param_2;
  *(undefined ***)(this + 0x48) = &PTR__scalar_deleting_destructor__01238c8c;
  *(undefined4 *)(this + 0x4c) = *(undefined4 *)(param_1 + 4);
  piVar1 = *(int **)(param_1 + 8);
  *(int **)(this + 0x50) = piVar1;
  if (piVar1 != (int *)0x0) {
    *piVar1 = *piVar1 + 1;
  }
  return this;
}


// CALLEES of CActiveEntityScriptBase:
//   0099e4b0  CCharString
//   00a44740  CCreatureAction_CastBattleCharge::CCreatureAction_CastBattleCharge
//   0099ec30  CCharString

