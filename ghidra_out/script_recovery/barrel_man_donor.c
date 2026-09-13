//=== ??0CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z @ 0382cff0 (seed 0x0382cff0) ===

/* [ported from ego_r via strfp] */

undefined4 *
__0CNOVI_BarrelMan_CQ_NewOakValeIntroScript_NScript__QAE_AAV12_PAVCScriptDataBase__ABVCScriptThing___Z
          (int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *in_ECX;
  
  __0CEntityScriptBase__QAE_AAVCGameScriptInterfaceBase__ABVCScriptThing___Z
            (*(undefined4 *)(param_1 + 100),param_3);
  *in_ECX = NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan::vftable;
  in_ECX[5] = param_1;
  in_ECX[6] = param_2;
  return in_ECX;
}


// CALLEES of ??0CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z:
//   018148ae  ??0CEntityScriptBase@@QAE@AAVCGameScriptInterfaceBase@@ABVCScriptThing@@@Z

//=== ?Init@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ @ 0382d060 (seed 0x0382d060) ===

/* [ported from ego_r via strfp] */

void _Init_CNOVI_BarrelMan_CQ_NewOakValeIntroScript_NScript__EAEXXZ(void)

{
  int iVar1;
  undefined4 *puVar2;
  int in_ECX;
  undefined1 local_10 [12];
  
  __4CTimer__QAEAAV0_J_Z(0);
  *(undefined1 *)(in_ECX + 0x1c) = 0;
  *(undefined4 *)(in_ECX + 0x20) = 0;
  (**(code **)(**(int **)(in_ECX + 4) + 0x810))(in_ECX + 8,0);
  (**(code **)(**(int **)(in_ECX + 4) + 0x814))(in_ECX + 8,0,0);
  (**(code **)(**(int **)(in_ECX + 4) + 0x838))(in_ECX + 8,0);
  (**(code **)(**(int **)(in_ECX + 4) + 0x5a0))(in_ECX + 8,0,1,0);
  puVar2 = (undefined4 *)(**(code **)(*(int *)(in_ECX + 8) + 0x1c))(local_10);
  iVar1 = *(int *)(in_ECX + 0x14);
  *(undefined4 *)(iVar1 + 0xa9) = *puVar2;
  *(undefined4 *)(iVar1 + 0xad) = puVar2[1];
  *(undefined4 *)(iVar1 + 0xb1) = puVar2[2];
  *(undefined1 *)(in_ECX + 0x1d) = 0;
  *(undefined1 *)(in_ECX + 0x1e) = 0;
  (**(code **)(**(int **)(in_ECX + 4) + 0xa00))(in_ECX + 8,0x41200000);
  return;
}


// CALLEES of ?Init@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ:
//   01850c73  ??4CTimer@@QAEAAV0@J@Z

//=== ?Main@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ @ 0382d1c0 (seed 0x0382d1c0) ===
// <decompile failed: Exception while decompiling 0382d1c0: process: timeout
>
// CALLEES of ?Main@CNOVI_BarrelMan@CQ_NewOakValeIntroScript@NScript@@EAEXXZ:
//   01822233  ??1CCharString@@QAE@XZ
//   017c9872  ??0CRGBColour@@QAE@EEE@Z
//   017a6fac  ?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z
//   018107fe  ?IsPerformingScriptTask@CScriptGameResourceObjectScriptedThingBase@@UBE_NXZ
//   017dd822  ~time_put<>
//   0181c126  ??0CCharString@@QAE@PBDJ@Z
//   017ef9a0  ?AddBadDeed@CQ_NewOakValeIntroScript@NScript@@QAEXW4EBadDeeds@12@@Z
//   017cbe74  ??0CTimer@@QAE@XZ
//   EXTERNAL:0000008e  rand
//   017d7260  ??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ
//   0183c575  ??0CScriptThing@@QAE@ABV0@@Z
//   01842d0d  ??0CScriptGameResourceObjectMovieBase@@QAE@XZ
//   01850c73  ??4CTimer@@QAEAAV0@J@Z
//   017c1aa5  ??1CWideScreenMagicPauseEntities@@QAE@XZ
//   01837c9b  ??0CScriptThing@@QAE@XZ
//   01853b35  ?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z
//   018413a4  ?MoveToPosition@CScriptGameResourceObjectScriptedThingBase@@UAEXABVC3DVector@@MW4EScriptEntityMoveType@@_N2@Z
//   017fb1f1  ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z
//   017c19f1  ?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ
//   017aafa3  ~time_put<>
//   0182e420  ?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ
//   017e6e09  ~CChevronOwnerDrawMenu
//   017ea518  ?Return@TRetVoid@@QAEXXZ
//   01847b14  ??1CTimer@@QAE@XZ
//   018572df  ?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ
//   018270fd  ?GetTime@CTimer@@QBEJXZ
//   0179beb8  ??0CIsThingConscious@NScript@@QAE@ABVCScriptThing@@@Z
//   017ebc88  ~CChevronOwnerDrawMenu
//   01845526  ??0CWideScreenMagicPauseEntities@@QAE@AAVCGameScriptInterfaceBase@@@Z
//   017de8ee  ?IsDistanceFromThingToPositionOver@@YI_NABVCScriptThing@@ABVC3DVector@@M@Z
//   017973bd  ?GetScriptThing@CScriptGameResourceObjectScriptedThingBase@@UBE?AVCScriptThing@@XZ

