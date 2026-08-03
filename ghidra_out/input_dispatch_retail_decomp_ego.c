//=== ?GetInputProcessEventFromUpdate@CGamePlayerInterface@@ABE_NAAVCProcessedInput@@@Z @ 00443200 (seed 00443200) ===

/* [ported from ego_r via strfp] */

undefined4
_GetInputProcessEventFromUpdate_CGamePlayerInterface__ABE_NAAVCProcessedInput___Z(int param_1)

{
  undefined4 uVar1;
  int in_ECX;
  int *piVar2;
  int *local_1c;
  int *local_18;
  int local_14;
  undefined1 local_10;
  int local_c;
  int local_8;
  int *local_4;
  
  if (*(char *)(in_ECX + 0x8a4) != '\0') {
    return 0;
  }
  *(undefined1 *)(in_ECX + 0x8a4) = 1;
  local_10 = 0;
  local_c = 0;
  local_8 = 0;
  local_4 = (int *)0x0;
  _Clear_CProcessedInput__QAEXXZ();
  uVar1 = _GetMainPlayerNumber_CPlayerManager__QBEJXZ();
  _SetPlayer_CProcessedInput__QAEXK_Z(uVar1);
  local_c = *(int *)(in_ECX + 0x10);
  local_4 = (int *)(in_ECX + 4);
  if (local_c != 0) {
    *(int ***)(local_c + 0x14) = &local_1c;
  }
  *(int ***)(in_ECX + 0x10) = &local_1c;
  local_1c = (int *)*local_4;
  local_8 = 0;
  local_10 = 1;
  if (local_1c != (int *)0x0) {
    local_14 = 0;
    piVar2 = (int *)*local_1c;
    local_18 = (int *)local_1c[1];
    if (piVar2 != (int *)0x0) {
      do {
        (**(code **)(*piVar2 + 0x18))(param_1);
        local_1c = local_18;
        if (local_18 == (int *)0x0) break;
        local_14 = local_18[2];
        piVar2 = (int *)*local_18;
        local_18 = (int *)local_18[1];
      } while (piVar2 != (int *)0x0);
      if (local_8 != 0) {
        *(int *)(local_8 + 0x10) = local_c;
        if (local_c != 0) {
          *(int *)(local_c + 0x14) = local_8;
        }
        goto LAB_004432eb;
      }
    }
  }
  *(int *)(in_ECX + 0x10) = local_c;
  if (local_c != 0) {
    *(undefined4 *)(local_c + 0x14) = 0;
  }
LAB_004432eb:
  if (*(char *)(param_1 + 0xa8) == '\0') {
    return 0;
  }
  return 1;
}


// CALLEES of ?GetInputProcessEventFromUpdate@CGamePlayerInterface@@ABE_NAAVCProcessedInput@@@Z:
//   00b74cb0  ?SetPlayer@CProcessedInput@@QAEXK@Z
//   00444ee0  ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ
//   00b74bf0  ?Clear@CProcessedInput@@QAEXXZ

//=== ?GetInputProcessEvent@CGamePlayerInterface@@ABE_NAAVCProcessedInput@@@Z @ 00443310 (seed 00443310) ===

/* [ported from ego_r via strfp] */

undefined4 _GetInputProcessEvent_CGamePlayerInterface__ABE_NAAVCProcessedInput___Z(int *param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int in_ECX;
  int *piVar5;
  undefined4 *puVar6;
  int *piVar7;
  undefined4 *puVar8;
  char local_236;
  int *piStack_234;
  int *piStack_230;
  int iStack_22c;
  undefined1 local_228;
  int local_224;
  int local_220;
  int *local_21c;
  int local_218;
  undefined1 local_214 [8];
  int local_20c;
  undefined4 local_208 [9];
  undefined4 uStack_1e4;
  undefined4 local_1d4 [13];
  int aiStack_1a0 [3];
  undefined1 local_194;
  undefined1 local_172;
  undefined1 local_16c;
  undefined1 local_14a;
  undefined1 local_144;
  undefined1 local_122;
  undefined1 local_11c;
  undefined1 local_fa;
  int local_f0 [3];
  undefined1 local_e4;
  undefined1 local_c2;
  undefined1 local_bc;
  undefined1 local_9a;
  undefined1 local_94;
  undefined1 local_72;
  undefined1 local_6c;
  undefined1 local_4a;
  undefined1 auStack_3c [56];
  
  local_228 = 0;
  local_224 = 0;
  local_220 = 0;
  local_21c = (int *)0x0;
  local_20c = in_ECX;
  __0CInputEvent__QAE_XZ();
  __0CInputEvent__QAE_XZ();
  local_194 = 0xff;
  local_172 = 0;
  local_16c = 0xff;
  local_14a = 0;
  local_144 = 0xff;
  local_122 = 0;
  local_11c = 0xff;
  local_fa = 0;
  local_e4 = 0xff;
  local_c2 = 0;
  local_bc = 0xff;
  local_9a = 0;
  local_94 = 0xff;
  local_72 = 0;
  local_6c = 0xff;
  local_4a = 0;
  _SetAsNull_CInputEvent__QAEXXZ();
  _Clear_CProcessedInput__QAEXXZ();
  local_236 = '\0';
  if ((DAT_0126d1f8 != (int *)0x0) && (cVar2 = (**(code **)(*DAT_0126d1f8 + 0x24))(), cVar2 == '\0')
     ) {
    __0CInputEvent__QAE_XZ();
    local_236 = _IsKeyboardEventInQueue_CInputManager__QBE_NW4EInputEventType__W4EInputKey__AAVCInputEvent___Z
                          (2,0x38,auStack_3c);
    __1CInputEvent__QAE_XZ();
  }
  local_218 = 0;
  bVar1 = false;
  local_214[0] = 0;
  cVar2 = _EventScanStart_CInputManager__QBE_NAAVCInputEventScanInfo__AAVCInputEvent___Z
                    (local_214,local_208);
  do {
    if (cVar2 == '\0') {
      _EventScanEnd_CInputManager__QBEXAAVCInputEventScanInfo___Z(local_214);
      if (bVar1) {
        piVar5 = local_f0;
        for (iVar3 = 0x2c; iVar3 != 0; iVar3 = iVar3 + -1) {
          *param_1 = *piVar5;
          piVar5 = piVar5 + 1;
          param_1 = param_1 + 1;
        }
        iVar3 = _GetType_CInputEvent__QBE_AW4EInputEventType__XZ();
        if (iVar3 != 0) {
          _FlagEventAsProcessed_CInputManager__QAEXABVCInputEvent___Z(local_1d4);
        }
        __1CInputEvent__QAE_XZ();
        __1CInputEvent__QAE_XZ();
        return 1;
      }
      __1CInputEvent__QAE_XZ();
      __1CInputEvent__QAE_XZ();
      return 0;
    }
    if ((((local_236 == '\0') ||
         (iVar3 = _GetDeviceType_CInputEvent__QBE_AW4EInputDeviceType__XZ(), iVar3 != 2)) ||
        (iVar3 = _GetKey_CInputEvent__QBE_AW4EInputKey__XZ(), iVar3 != 0xf)) &&
       ((iVar3 = _GetType_CInputEvent__QBE_AW4EInputEventType__XZ(), iVar3 != 0 &&
        (cVar2 = _IsEventProcessed_CInputManager__QBE_NABVCInputEvent___Z(local_208), cVar2 == '\0')
        ))) {
      local_224 = *(int *)(in_ECX + 0x10);
      local_21c = (int *)(in_ECX + 4);
      if (local_224 != 0) {
        *(int ***)(local_224 + 0x14) = &piStack_234;
      }
      *(int ***)(in_ECX + 0x10) = &piStack_234;
      piStack_234 = (int *)*local_21c;
      local_220 = 0;
      local_228 = 1;
      if (piStack_234 == (int *)0x0) {
        piStack_234 = (int *)0x0;
        piStack_230 = (int *)0x0;
        iStack_22c = 0;
LAB_00443591:
        *(int *)(in_ECX + 0x10) = local_224;
        if (local_224 != 0) {
          *(undefined4 *)(local_224 + 0x14) = 0;
        }
      }
      else {
        piStack_230 = (int *)piStack_234[1];
        iStack_22c = 0;
        piVar5 = (int *)*piStack_234;
        if (piVar5 == (int *)0x0) goto LAB_00443591;
        do {
          _Clear_CProcessedInput__QAEXXZ();
          cVar2 = (**(code **)(*piVar5 + 0x20))(local_208);
          if (cVar2 != '\0') {
            uVar4 = _GetPlayerNumberFromJoystickDeviceNumber_CPlayerManager__QBEJJ_Z(uStack_1e4);
            _SetPlayer_CProcessedInput__QAEXK_Z(uVar4);
            cVar2 = (**(code **)(*piVar5 + 0x1c))();
            if ((cVar2 != '\0') || (*(char *)(DAT_0126d1f8[9] + 0xa4) == '\0')) {
              (**(code **)(*piVar5 + 0x10))(local_208,aiStack_1a0);
            }
          }
          if (aiStack_1a0[1] == 1) {
            bVar1 = true;
            iVar3 = _GetPriority_CProcessedInput__QAE_AW4EGameEventPriority__XZ();
            if (local_218 < iVar3) {
              local_218 = _GetPriority_CProcessedInput__QAE_AW4EGameEventPriority__XZ();
              piVar5 = aiStack_1a0;
              piVar7 = local_f0;
              for (iVar3 = 0x2c; iVar3 != 0; iVar3 = iVar3 + -1) {
                *piVar7 = *piVar5;
                piVar5 = piVar5 + 1;
                piVar7 = piVar7 + 1;
              }
              puVar6 = local_208;
              puVar8 = local_1d4;
              for (iVar3 = 0xd; in_ECX = local_20c, iVar3 != 0; iVar3 = iVar3 + -1) {
                *puVar8 = *puVar6;
                puVar6 = puVar6 + 1;
                puVar8 = puVar8 + 1;
              }
            }
          }
          else if (aiStack_1a0[1] == 2) {
            _FlagEventAsProcessed_CInputManager__QAEXABVCInputEvent___Z(local_208);
            break;
          }
          piStack_234 = piStack_230;
          if (piStack_230 == (int *)0x0) break;
          piVar7 = piStack_230 + 1;
          iStack_22c = piStack_230[2];
          piVar5 = (int *)*piStack_230;
          piStack_230 = (int *)*piVar7;
        } while (piVar5 != (int *)0x0);
        if (local_220 == 0) goto LAB_00443591;
        *(int *)(local_220 + 0x10) = local_224;
        if (local_224 != 0) {
          *(int *)(local_224 + 0x14) = local_220;
        }
      }
      local_228 = 0;
      local_224 = 0;
      local_220 = 0;
      local_21c = (int *)0x0;
    }
    cVar2 = _EventScanNext_CInputManager__QBE_NAAVCInputEventScanInfo__AAVCInputEvent___Z
                      (local_214,local_208);
  } while( true );
}


// CALLEES of ?GetInputProcessEvent@CGamePlayerInterface@@ABE_NAAVCProcessedInput@@@Z:
//   00b6efe0  ??0CInputEvent@@QAE@XZ
//   00b6e610  ?GetDeviceType@CInputEvent@@QBE?AW4EInputDeviceType@@XZ
//   00b48650  ?EventScanEnd@CInputManager@@QBEXAAVCInputEventScanInfo@@@Z
//   00b74cc0  ?GetPriority@CProcessedInput@@QAE?AW4EGameEventPriority@@XZ
//   00b74bf0  ?Clear@CProcessedInput@@QAEXXZ
//   00b6e600  ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
//   00b49120  ?FlagEventAsProcessed@CInputManager@@QAEXABVCInputEvent@@@Z
//   00b48b60  ?IsKeyboardEventInQueue@CInputManager@@QBE_NW4EInputEventType@@W4EInputKey@@AAVCInputEvent@@@Z
//   00b48ac0  ?EventScanStart@CInputManager@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z
//   00b74cb0  ?SetPlayer@CProcessedInput@@QAEXK@Z
//   00b6e5f0  ??1CInputEvent@@QAE@XZ
//   00b48d20  ?IsEventProcessed@CInputManager@@QBE_NABVCInputEvent@@@Z
//   00b6e6a0  ?SetAsNull@CInputEvent@@QAEXXZ
//   00b48b00  ?EventScanNext@CInputManager@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z
//   00b6e630  ?GetKey@CInputEvent@@QBE?AW4EInputKey@@XZ
//   00445190  ?GetPlayerNumberFromJoystickDeviceNumber@CPlayerManager@@QBEJJ@Z

//=== ?IsEventGameAction@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z @ 004439f0 (seed 004439f0) ===

/* [ported from ego_r via strfp] */

undefined4
_IsEventGameAction_CGamePlayerInterface__QBE_NW4EGameAction__ABVCInputEvent___Z
          (undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  char cVar2;
  int local_8;
  int local_4;
  
  _UpperBound___CVectorMap_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase____V__CKeyPairCompareLess_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase________QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase_____std__V__allocator_U__pair_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase_____std___2__std___std__ABW4EGameAction___Z
            (&local_4,&param_1);
  _LowerBound___CVectorMap_JV__CIntelligentPointer_VCThing____V__CKeyPairCompareLess_JV__CIntelligentPointer_VCThing________QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_JV__CIntelligentPointer_VCThing_____std__V__allocator_U__pair_JV__CIntelligentPointer_VCThing_____std___2__std___std__ABJ_Z
            (&local_8,&param_1);
  uVar1 = param_2;
  while( true ) {
    if (local_8 == local_4) {
      return 0;
    }
    cVar2 = (**(code **)(**(int **)(local_8 + 4) + 0xc))(uVar1);
    if (cVar2 != '\0') break;
    local_8 = local_8 + 0xc;
  }
  return 1;
}


// CALLEES of ?IsEventGameAction@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z:
//   00444aa0  ?UpperBound@?$CVectorMap@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@@QBE?AV?$_Vector_const_iterator@V?$_Vector_val@U?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@V?$allocator@U?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@@2@@std@@@std@@ABW4EGameAction@@@Z
//   0085a270  ?LowerBound@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@V?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@@QBE?AV?$_Vector_const_iterator@V?$_Vector_val@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@V?$allocator@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@2@@std@@@std@@ABJ@Z

//=== ?GetControlMovementFromGameActionEvent@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z @ 00443a60 (seed 00443a60) ===

/* [ported from ego_r via strfp] */

uint _GetControlMovementFromGameActionEvent_CGamePlayerInterface__QBE_NW4EGameAction__ABVCInputEvent__AAVC2DVector___Z
               (undefined4 *param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 uVar2;
  uint uVar3;
  undefined4 *puVar4;
  int local_c;
  int local_8 [2];
  
  _UpperBound___CVectorMap_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase____V__CKeyPairCompareLess_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase________QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase_____std__V__allocator_U__pair_W4EGameAction__V__CCountedPointer___CBVCInputTypeBase_____std___2__std___std__ABW4EGameAction___Z
            (local_8,&param_1);
  uVar3 = _LowerBound___CVectorMap_JV__CIntelligentPointer_VCThing____V__CKeyPairCompareLess_JV__CIntelligentPointer_VCThing________QBE_AV___Vector_const_iterator_V___Vector_val_U__pair_JV__CIntelligentPointer_VCThing_____std__V__allocator_U__pair_JV__CIntelligentPointer_VCThing_____std___2__std___std__ABJ_Z
                    (&local_c,&param_1);
  uVar2 = param_2;
  while( true ) {
    if (local_c == local_8[0]) {
      return uVar3 & 0xffffff00;
    }
    piVar1 = *(int **)(local_c + 4);
    uVar3 = (**(code **)(*piVar1 + 0xc))(uVar2);
    if ((char)uVar3 != '\0') break;
    local_c = local_c + 0xc;
  }
  puVar4 = (undefined4 *)(**(code **)(*piVar1 + 0x10))(local_8,uVar2);
  *param_1 = *puVar4;
  uVar2 = puVar4[1];
  param_1[1] = uVar2;
  return CONCAT31((int3)((uint)uVar2 >> 8),1);
}


// CALLEES of ?GetControlMovementFromGameActionEvent@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z:
//   00444aa0  ?UpperBound@?$CVectorMap@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@V?$CKeyPairCompareLess@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@@@@QBE?AV?$_Vector_const_iterator@V?$_Vector_val@U?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@V?$allocator@U?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@@2@@std@@@std@@ABW4EGameAction@@@Z
//   0085a270  ?LowerBound@?$CVectorMap@JV?$CIntelligentPointer@VCThing@@@@V?$CKeyPairCompareLess@JV?$CIntelligentPointer@VCThing@@@@@@@@QBE?AV?$_Vector_const_iterator@V?$_Vector_val@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@V?$allocator@U?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@2@@std@@@std@@ABJ@Z

//=== ?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z @ 006b2e00 (seed 006b2e00) ===

/* [ported from ego_r via strfp] */

void _IsEventGameAction_CInputProcessGameBase__QBE_NW4EGameAction__ABVCInputEvent___Z(void)

{
  _IsEventGameAction_CGamePlayerInterface__QBE_NW4EGameAction__ABVCInputEvent___Z();
  return;
}


// CALLEES of ?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z:
//   004439f0  ?IsEventGameAction@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z

//=== ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z @ 006b2e10 (seed 006b2e10) ===

/* [ported from ego_r via strfp] */

void _GetControlMovementFromGameActionEvent_CInputProcessGameBase__QBE_NW4EGameAction__ABVCInputEvent__AAVC2DVector___Z
               (void)

{
  _GetControlMovementFromGameActionEvent_CGamePlayerInterface__QBE_NW4EGameAction__ABVCInputEvent__AAVC2DVector___Z
            ();
  return;
}


// CALLEES of ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z:
//   00443a60  ?GetControlMovementFromGameActionEvent@CGamePlayerInterface@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z

//=== ?ProcessEvent@CFrontEndList@NUISystem@@UAEXW4EEvent@2@@Z @ 005c62e0 (seed 005c62e0) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CFrontEndList_NUISystem__UAEXW4EEvent_2__Z(void)

{
  return;
}


// CALLEES of ?ProcessEvent@CFrontEndList@NUISystem@@UAEXW4EEvent@2@@Z:

//=== ?ProcessEvent@CScrollableList@NUISystem@@UAEXW4EEvent@2@@Z @ 005ceff0 (seed 005ceff0) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CScrollableList_NUISystem__UAEXW4EEvent_2__Z(undefined4 param_1)

{
  undefined4 uVar1;
  char cVar2;
  uint uVar3;
  int *piVar4;
  int *piVar5;
  uint in_ECX;
  int *unaff_EDI;
  
  uVar1 = param_1;
  _ProcessEvent_CComponent_NUISystem__UAEXW4EEvent_2__Z(param_1);
  switch(uVar1) {
  case 4:
    if ((DAT_0126d1f8 != 0) &&
       (cVar2 = _CanAcceptChanges_CRedefinerList_NUISystem__SI_NXZ(), cVar2 != '\0')) {
      if (1 < DAT_0126de20) {
        (**(code **)(**(int **)(*DAT_0126de1c + 8) + 0xc0))(6);
        _pop_front___list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__QAEXXZ();
        (**(code **)(**(int **)(*DAT_0126de1c + 8) + 0xc0))(3);
        piVar4 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
        (**(code **)(*piVar4 + 0x14))(-(uint)(in_ECX != 4) & in_ECX);
      }
      piVar4 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      piVar5 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      piVar4 = (int *)(**(code **)(*piVar4 + 0xa8))(&param_1);
      (**(code **)(*piVar5 + 0xa0))(*piVar4 + 0x1a8,1);
      if ((unaff_EDI != (int *)0x0) && (unaff_EDI[1] = unaff_EDI[1] + -1, unaff_EDI[1] == 0)) {
        (**(code **)(*unaff_EDI + 4))();
      }
      piVar4 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(*piVar4 + 0x80))(0);
      _Get_CUserProfileManager__SIAAV1_XZ();
      _Save_CUserProfileManager__QAEXXZ();
    }
    break;
  case 0x10:
  case 0x24:
    if (*(int *)(in_ECX + 0x188) != 0) {
      *(int *)(in_ECX + 0x188) = *(int *)(in_ECX + 0x188) + -1;
LAB_005cf02d:
      _UpdateVisibleList_CScrollableList_NUISystem__IAEXMM_Z(0x3e4ccccd,0x3e4ccccd);
      return;
    }
    break;
  case 0x11:
  case 0x25:
    uVar3 = *(int *)(in_ECX + 0xb8) - *(int *)(in_ECX + 0xb4) >> 3;
    if ((*(uint *)(in_ECX + 0x18c) < uVar3) &&
       (*(uint *)(in_ECX + 0x188) < uVar3 - *(uint *)(in_ECX + 0x18c))) {
      *(uint *)(in_ECX + 0x188) = *(uint *)(in_ECX + 0x188) + 1;
      goto LAB_005cf02d;
    }
  }
  return;
}


// CALLEES of ?ProcessEvent@CScrollableList@NUISystem@@UAEXW4EEvent@2@@Z:
//   005cf8f0  ?CanAcceptChanges@CRedefinerList@NUISystem@@SI_NXZ
//   005ced60  ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z
//   0040bfc0  ?Get@CUserProfileManager@@SIAAV1@XZ
//   005a83f0  ?ProcessEvent@CComponent@NUISystem@@UAEXW4EEvent@2@@Z
//   005b5df1  ?pop_front@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAEXXZ
//   00438c20  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
//   00409310  ?Save@CUserProfileManager@@QAEXXZ

