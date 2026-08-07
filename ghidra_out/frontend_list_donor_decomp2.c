//=== ?ProcessEvent@CScrollableList@NUISystem@@UAEXW4EEvent@2@@Z @ 022ad270 (seed 022ad270) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CScrollableList_NUISystem__UAEXW4EEvent_2__Z(undefined4 param_1)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  int *piVar5;
  int *piVar6;
  int in_ECX;
  int iStack_28;
  undefined1 auStack_14 [4];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 uStack_8;
  
  uStack_8 = 0xffffffff;
  puStack_c = &LAB_03d201d8;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _ProcessEvent_CComponent_NUISystem__UAEXW4EEvent_2__Z(param_1);
  switch(param_1) {
  case 4:
    iVar3 = _Get_CMainGameComponent__SIPAV1_XZ();
    if ((iVar3 != 0) &&
       (cVar1 = _CanAcceptChanges_CRedefinerList_NUISystem__SI_NXZ(), cVar1 != '\0')) {
      _GetPreviousSelected_CList_NUISystem__SIAAV__list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__XZ
                ();
      uVar2 = _size___list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__QBEIXZ()
      ;
      if (1 < uVar2) {
        _GetPreviousSelected_CList_NUISystem__SIAAV__list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__XZ
                  ();
        puVar4 = (undefined4 *)
                 _front___list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__QAEAAPAVCList_NUISystem__XZ
                           ();
        (**(code **)(*(int *)*puVar4 + 0xc0))(6);
        _GetPreviousSelected_CList_NUISystem__SIAAV__list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__XZ
                  ();
        _pop_front___list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__QAEXXZ();
        _GetPreviousSelected_CList_NUISystem__SIAAV__list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__XZ
                  ();
        puVar4 = (undefined4 *)
                 _front___list_PAVCList_NUISystem__V__allocator_PAVCList_NUISystem___std___std__QAEAAPAVCList_NUISystem__XZ
                           ();
        (**(code **)(*(int *)*puVar4 + 0xc0))(3);
        iStack_28 = in_ECX;
        if (in_ECX == 4) {
          iStack_28 = 0;
        }
        piVar5 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
        (**(code **)(*piVar5 + 0x14))(iStack_28);
      }
      piVar5 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      piVar6 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(*piVar5 + 0xa8))(auStack_14,1);
      uStack_8 = 0;
      iVar3 = __C__CDefPointer_VCUIMiscThingsDef_NUISystem____QBEPAVCUIMiscThingsDef_NUISystem__XZ()
      ;
      (**(code **)(*piVar6 + 0xa0))(iVar3 + 0x2d0);
      uStack_8 = 0xffffffff;
      __1__CDefPointer_VCUIMiscThingsDef_NUISystem____QAE_XZ();
      piVar5 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(*piVar5 + 0x80))(0);
      _Get_CUserProfileManager__SIAAV1_XZ();
      _Save_CUserProfileManager__QAEXXZ();
    }
    break;
  case 0x10:
  case 0x24:
    if (*(int *)(in_ECX + 0x1b0) != 0) {
      *(int *)(in_ECX + 0x1b0) = *(int *)(in_ECX + 0x1b0) + -1;
      _UpdateVisibleList_CScrollableList_NUISystem__IAEXMM_Z(0x3e4ccccd,0x3e4ccccd);
    }
    break;
  case 0x11:
  case 0x25:
    uVar2 = _size___vector_V__CCountedPointer_VCComponent_NUISystem____V__allocator_V__CCountedPointer_VCComponent_NUISystem_____std___std__QBEIXZ
                      ();
    if ((*(uint *)(in_ECX + 0x1b4) < uVar2) &&
       (iVar3 = _size___vector_V__CCountedPointer_VCComponent_NUISystem____V__allocator_V__CCountedPointer_VCComponent_NUISystem_____std___std__QBEIXZ
                          (), *(uint *)(in_ECX + 0x1b0) < (uint)(iVar3 - *(int *)(in_ECX + 0x1b4))))
    {
      *(int *)(in_ECX + 0x1b0) = *(int *)(in_ECX + 0x1b0) + 1;
      _UpdateVisibleList_CScrollableList_NUISystem__IAEXMM_Z(0x3e4ccccd,0x3e4ccccd);
    }
  }
  ExceptionList = local_10;
  return;
}


// CALLEES of ?ProcessEvent@CScrollableList@NUISystem@@UAEXW4EEvent@2@@Z:
//   0181c7de  ?ProcessEvent@CComponent@NUISystem@@UAEXW4EEvent@2@@Z

//=== ?OnActivate@CScrollableList@NUISystem@@UAEXXZ @ 022acb70 (seed 022acb70) ===

/* [ported from ego_r via strfp] */

void _OnActivate_CScrollableList_NUISystem__UAEXXZ(void)

{
  int *piVar1;
  int in_ECX;
  undefined4 local_24;
  undefined4 local_18;
  undefined4 local_c;
  
  _OnActivate_CChangingStateComponent_NUISystem__MAEXXZ();
  _UpdateVisibleList_CScrollableList_NUISystem__IAEXMM_Z(0,0x3e4ccccd);
  *(undefined4 *)(in_ECX + 0x1c4) = 0x3e4ccccd;
  (**(code **)(*(int *)(in_ECX + 4) + 0xc))(0x25);
  (**(code **)(*(int *)(in_ECX + 4) + 0xc))(0x24);
  (**(code **)(*(int *)(in_ECX + 4) + 0xc))(4);
  if (in_ECX == 0) {
    local_c = 0;
  }
  else {
    local_c = in_ECX + 4;
  }
  piVar1 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
  (**(code **)(*piVar1 + 8))(local_c);
  if (*(int *)(in_ECX + 0x1c8) == 0) {
    local_18 = 0;
  }
  else {
    local_18 = *(int *)(in_ECX + 0x1c8) + 4;
  }
  piVar1 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
  (**(code **)(*piVar1 + 0x14))(local_18);
  if (*(int *)(in_ECX + 0x1cc) == 0) {
    local_24 = 0;
  }
  else {
    local_24 = *(int *)(in_ECX + 0x1cc) + 4;
  }
  piVar1 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
  (**(code **)(*piVar1 + 0x14))(local_24);
  return;
}


// CALLEES of ?OnActivate@CScrollableList@NUISystem@@UAEXXZ:
//   0181594d  ?OnActivate@CChangingStateComponent@NUISystem@@MAEXXZ
//   01854e77  ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z
//   017ac010  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ

//=== ?OnDeactivate@CScrollableList@NUISystem@@UAEXXZ @ 022acce0 (seed 022acce0) ===

/* [ported from ego_r via strfp] */

void _OnDeactivate_CScrollableList_NUISystem__UAEXXZ(void)

{
  int in_ECX;
  
  _OnDeactivate_CChangingStateComponent_NUISystem__MAEXXZ();
  _HideList_CScrollableList_NUISystem__IAEXM_Z(0x3e4ccccd);
  (**(code **)(*(int *)(in_ECX + 4) + 0x10))(0x25);
  (**(code **)(*(int *)(in_ECX + 4) + 0x10))(0x24);
  return;
}


// CALLEES of ?OnDeactivate@CScrollableList@NUISystem@@UAEXXZ:
//   01795c66  ?HideList@CScrollableList@NUISystem@@IAEXM@Z
//   017cc87e  ?OnDeactivate@CChangingStateComponent@NUISystem@@MAEXXZ

//=== ?Initialise@CRedefinerList@NUISystem@@UAEXXZ @ 022af000 (seed 022af000) ===

/* [ported from ego_r via strfp] */

void _Initialise_CRedefinerList_NUISystem__UAEXXZ(void)

{
  code *pcVar1;
  char cVar2;
  BOOL BVar3;
  int iVar4;
  undefined4 uStack_64;
  undefined1 *puStack_60;
  undefined1 *puStack_5c;
  undefined1 *puStack_58;
  char *pcStack_54;
  wchar_t *pwStack_50;
  int *local_4c;
  int *local_48;
  int *local_44;
  int *local_40;
  undefined1 *local_3c;
  undefined1 local_38 [8];
  undefined1 local_30 [8];
  undefined1 local_28 [11];
  char local_1d;
  undefined1 local_1c [8];
  undefined1 local_14 [4];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03d20448;
  local_10 = ExceptionList;
  pwStack_50 = L"㶃∸Ҭༀ뢄";
  ExceptionList = &local_10;
  _Initialise_CScrollableList_NUISystem__UAEXXZ();
  if ((DAT_04ac2238 != (int *)0x0) && (DAT_04ac2238 != local_40)) {
    pwStack_50 = L"ui_redefiner_list.cpp";
    pcStack_54 = (char *)0x22af04e;
    __0CWideString__QAE_PB_W_Z();
    local_8 = 0;
    pwStack_50 = (wchar_t *)0xffffffff;
    pcStack_54 = "";
    puStack_58 = (undefined1 *)0x22af064;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 1;
    pwStack_50 = (wchar_t *)0xffffffff;
    pcStack_54 = "PRedefinerList == NULL || PRedefinerList == this";
    puStack_58 = (undefined1 *)0x22af077;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 2;
    pwStack_50 = (wchar_t *)0x1;
    pcStack_54 = (char *)0x113;
    puStack_58 = local_28;
    puStack_5c = local_30;
    puStack_60 = local_38;
    uStack_64 = 0x22af093;
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    uStack_64 = 0x22af09a;
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    uStack_64 = 0x22af0a1;
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      ();
    local_1d = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 1;
    pwStack_50 = (wchar_t *)0x22af0bd;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    pwStack_50 = (wchar_t *)0x22af0c9;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    pwStack_50 = 
    L"똏\xe755튅୴ᗿ䁠Ҽ삅Ŵ诌쑅㢣갢贄\xf04d譑쑕ʋ䶋评낐\x01＀쟒ﱅ\x03";
    __1CWideString__QAE_XZ();
    if (local_1d != '\0') {
      pwStack_50 = L"삅Ŵ诌쑅㢣갢贄\xf04d譑쑕ʋ䶋评낐\x01＀쟒ﱅ\x03";
      BVar3 = IsDebuggerPresent();
      if (BVar3 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  DAT_04ac2238 = local_40;
  pwStack_50 = (wchar_t *)local_14;
  pcStack_54 = (char *)0x22af107;
  (**(code **)(*local_40 + 0x1b0))();
  local_8 = 3;
  pcStack_54 = (char *)0x22af116;
  iVar4 = __C__CDefPointer___CBVCUIDef_NUISystem____QBEPBVCUIDef_NUISystem__XZ();
  if (*(int *)(iVar4 + 0x1dc) != 0) {
    pcStack_54 = (char *)0x22af12b;
    iVar4 = __C__CDefPointer___CBVCUIDef_NUISystem____QBEPBVCUIDef_NUISystem__XZ();
    pcStack_54 = *(char **)(iVar4 + 0x1dc);
    puStack_58 = (undefined1 *)0x22af137;
    _GetInstance_CManager_NUISystem__SIPAV12_XZ();
    puStack_58 = (undefined1 *)0x22af13e;
    iVar4 = _CreateComponent_CManager_NUISystem__QAEPAVCComponent_2_J_Z();
    local_40[0x78] = iVar4;
    pcStack_54 = (char *)local_40[0x78];
    puStack_58 = (undefined1 *)0x22af159;
    __0__CCountedPointer_VCComponent_NUISystem____QAE_PAVCComponent_NUISystem___Z();
    local_8._0_1_ = 4;
    pcStack_54 = (char *)0x22af16d;
    local_44 = (int *)(**(code **)(*local_40 + 0xd0))();
    pcStack_54 = local_1c;
    puStack_58 = (undefined1 *)0x22af184;
    (**(code **)(*local_44 + 0xec))();
    puStack_58 = (undefined1 *)0x22af18c;
    local_48 = (int *)__C__CCountedPointer_VCComponent_NUISystem____QBEPAVCComponent_NUISystem__XZ()
    ;
    puStack_58 = (undefined1 *)0x22af19f;
    puStack_58 = (undefined1 *)(**(code **)(*local_40 + 0xd0))();
    puStack_5c = (undefined1 *)0x22af1b0;
    (**(code **)(*local_48 + 0xcc))();
    puStack_5c = (undefined1 *)0x22af1b8;
    local_4c = (int *)__C__CCountedPointer_VCComponent_NUISystem____QBEPAVCComponent_NUISystem__XZ()
    ;
    puStack_5c = (undefined1 *)0x22af1cb;
    (**(code **)(*local_4c + 0xac))();
    puStack_5c = (undefined1 *)0x0;
    local_3c = (undefined1 *)&uStack_64;
    (**(code **)(*(int *)local_40[0x78] + 0x40))(&uStack_64);
    puStack_60 = (undefined1 *)0x0;
    uStack_64 = 0;
    _HideComponent_CScrollableList_NUISystem__IAEXPAVCComponent_2_MMVC2DVector__E_Z(local_40[0x78]);
    local_8 = CONCAT31(local_8._1_3_,3);
    pcStack_54 = (char *)0x22af219;
    __1__CCountedPointer_VCComponent_NUISystem____QAE_XZ();
  }
  local_8 = 0xffffffff;
  pcStack_54 = (char *)0x22af228;
  __1__CDefPointer___CBVCUIDef_NUISystem____QAE_XZ();
  ExceptionList = local_10;
  return;
}


// CALLEES of ?Initialise@CRedefinerList@NUISystem@@UAEXXZ:
//   01822233  ??1CCharString@@QAE@XZ
//   0179e6f9  ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ
//   0181c126  ??0CCharString@@QAE@PBDJ@Z
//   0183552c  ?DoErrorMessage@CDebugManager@@QAE_NABVCCharString@@0ABVCWideString@@KW4EErrorType@NDebugManager@@@Z
//   EXTERNAL:00000128  IsDebuggerPresent
//   017dc6fc  ?GetDebugManager@CSystemManager@@QAEPAVCDebugManager@@XZ
//   017a9e1e  ??C?$CDefPointer@$$CBVCUIDef@NUISystem@@@@QBEPBVCUIDef@NUISystem@@XZ
//   0185474c  ?CreateComponent@CManager@NUISystem@@QAEPAVCComponent@2@J@Z
//   017d065e  ??0?$CCountedPointer@VCComponent@NUISystem@@@@QAE@PAVCComponent@NUISystem@@@Z
//   017f8bae  ?GFGetSystemManager@@YIPAVCSystemManager@@XZ
//   017cee99  ?HideComponent@CScrollableList@NUISystem@@IAEXPAVCComponent@2@MMVC2DVector@@E@Z
//   0183923f  ??C?$CCountedPointer@VCComponent@NUISystem@@@@QBEPAVCComponent@NUISystem@@XZ
//   017ea333  ??1CWideString@@QAE@XZ
//   017b2f37  ??1?$CDefPointer@$$CBVCUIDef@NUISystem@@@@QAE@XZ
//   0180fe4e  ?Initialise@CScrollableList@NUISystem@@UAEXXZ
//   017ac010  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
//   017f059e  ??0CWideString@@QAE@PB_W@Z

//=== ?ClearList@CRedefinerList@NUISystem@@IAEXXZ @ 022af2d0 (seed 022af2d0) ===

/* [ported from ego_r via strfp] */

void _ClearList_CRedefinerList_NUISystem__IAEXXZ(void)

{
  uint uVar1;
  int iVar2;
  int *in_ECX;
  uint local_8;
  
  local_8 = 0;
  while (uVar1 = _size___vector_V__CCountedPointer_VCComponent_NUISystem____V__allocator_V__CCountedPointer_VCComponent_NUISystem_____std___std__QBEIXZ
                           (), local_8 < uVar1) {
    __A__vector_V__CCountedPointer_VCComponent_NUISystem____V__allocator_V__CCountedPointer_VCComponent_NUISystem_____std___std__QAEAAV__CCountedPointer_VCComponent_NUISystem____I_Z
              (local_8);
    _Get___CCountedPointer_VCComponent_NUISystem____QBEPAVCComponent_NUISystem__XZ();
    iVar2 = ___GFDebugPDynamicCast_PAVCComponent_NUISystem__PAVCKeyRedefiner_2___YIPAVCKeyRedefiner_NUISystem__PAVCComponent_1_PAV01__Z
                      ();
    if (iVar2 == 0) {
      local_8 = local_8 + 1;
    }
    else {
      (**(code **)(*in_ECX + 0x100))(local_8);
    }
  }
  _ResetButtonCounts_CKeyRedefiner_NUISystem__SIXXZ();
  return;
}


// CALLEES of ?ClearList@CRedefinerList@NUISystem@@IAEXXZ:
//   0184146c  ?ResetButtonCounts@CKeyRedefiner@NUISystem@@SIXXZ
//   017f0a3f  ??A?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@QAEAAV?$CCountedPointer@VCComponent@NUISystem@@@@I@Z
//   018498ec  ?size@?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@QBEIXZ
//   017bad8b  ?Get@?$CCountedPointer@VCComponent@NUISystem@@@@QBEPAVCComponent@NUISystem@@XZ
//   0181ad2b  ??$GFDebugPDynamicCast@PAVCComponent@NUISystem@@PAVCKeyRedefiner@2@@@YIPAVCKeyRedefiner@NUISystem@@PAVCComponent@1@PAV01@@Z

//=== ?Update@CRedefinerList@NUISystem@@UAEXM@Z @ 022af660 (seed 022af660) ===

/* [ported from ego_r via strfp] */

void _Update_CRedefinerList_NUISystem__UAEXM_Z(float param_1)

{
  int in_ECX;
  
  _Update_CScrollableList_NUISystem__UAEXM_Z(param_1);
  if ((0.0 < *(float *)(in_ECX + 0x1d8)) &&
     (*(float *)(in_ECX + 0x1d8) = *(float *)(in_ECX + 0x1d8) - param_1,
     *(float *)(in_ECX + 0x1d8) < 0.0 != (*(float *)(in_ECX + 0x1d8) == 0.0))) {
    *(undefined4 *)(in_ECX + 0x1d8) = 0;
    if (*(char *)(in_ECX + 0x1d5) == '\0') {
      _ClearList_CRedefinerList_NUISystem__IAEXXZ();
    }
    else {
      _BuildKeyList_CRedefinerList_NUISystem__IAEXXZ();
    }
  }
  return;
}


// CALLEES of ?Update@CRedefinerList@NUISystem@@UAEXM@Z:
//   017c4f5c  ?Update@CScrollableList@NUISystem@@UAEXM@Z
//   017e7246  ?BuildKeyList@CRedefinerList@NUISystem@@IAEXXZ
//   017a04c2  ?ClearList@CRedefinerList@NUISystem@@IAEXXZ

//=== ?GetMouseWheelMovement@CInputEvent@@QBEMXZ @ 0317c620 (seed 0317c620) ===

/* [ported from ego_r via strfp] */

float10 _GetMouseWheelMovement_CInputEvent__QBEMXZ(void)

{
  code *pcVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  BOOL BVar6;
  int in_ECX;
  float10 fVar7;
  char local_38;
  undefined1 local_2c [8];
  undefined1 local_24 [8];
  undefined1 local_1c [11];
  char local_11;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03edc05b;
  local_10 = ExceptionList;
  bVar4 = false;
  bVar3 = false;
  bVar2 = false;
  if (*(int *)(in_ECX + 0x20) == 3) goto LAB_0317c75a;
  ExceptionList = &local_10;
  if (DAT_04bac270 == 0) {
LAB_0317c6e9:
    local_38 = '\x01';
  }
  else {
    ExceptionList = &local_10;
    __0CWideString__QAE_PB_W_Z(L"lib_input_event.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_044d7ce5,0xffffffff);
    local_8 = 1;
    __0CCharString__QAE_PBDJ_Z("DeviceType==INPUT_DEVICE_TYPE_MOUSE",0xffffffff);
    local_8 = 2;
    bVar4 = true;
    bVar3 = true;
    bVar2 = true;
    cVar5 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (local_2c,local_24,local_1c,0x85,1);
    if (cVar5 == '\x01') goto LAB_0317c6e9;
    local_38 = '\0';
  }
  local_11 = local_38;
  local_8 = 1;
  if (bVar3) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0;
  if (bVar4) {
    __1CCharString__QAE_XZ();
  }
  local_8 = 0xffffffff;
  if (bVar2) {
    __1CWideString__QAE_XZ();
  }
  if (local_11 != '\0') {
    BVar6 = IsDebuggerPresent();
    if (BVar6 != 0) {
      pcVar1 = (code *)swi(3);
      fVar7 = (float10)(*pcVar1)();
      return fVar7;
    }
  }
LAB_0317c75a:
  ExceptionList = local_10;
  return (float10)*(float *)(in_ECX + 0x14);
}


// CALLEES of ?GetMouseWheelMovement@CInputEvent@@QBEMXZ:
//   01822233  ??1CCharString@@QAE@XZ
//   0181c126  ??0CCharString@@QAE@PBDJ@Z
//   0183552c  ?DoErrorMessage@CDebugManager@@QAE_NABVCCharString@@0ABVCWideString@@KW4EErrorType@NDebugManager@@@Z
//   EXTERNAL:00000128  IsDebuggerPresent
//   017ea333  ??1CWideString@@QAE@XZ
//   017f059e  ??0CWideString@@QAE@PB_W@Z

//=== ?SetAsMouseWheelMovement@CInputEvent@@QAEXMN@Z @ 0317da90 (seed 0317da90) ===

/* [ported from ego_r via strfp] */

void _SetAsMouseWheelMovement_CInputEvent__QAEXMN_Z(undefined4 param_1,double param_2)

{
  int in_ECX;
  
  *(undefined4 *)(in_ECX + 0x20) = 3;
  *(undefined4 *)(in_ECX + 0x28) = 0xe;
  *(undefined4 *)(in_ECX + 0x14) = param_1;
  *(float *)(in_ECX + 0x30) = (float)param_2;
  *(float *)(in_ECX + 0x2c) = (float)param_2;
  return;
}


// CALLEES of ?SetAsMouseWheelMovement@CInputEvent@@QAEXMN@Z:

