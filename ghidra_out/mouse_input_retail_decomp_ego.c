//=== ?ConvertMouseEventToInputEvent@CMouseDX@@IAE_NPBVCMouseEvent@@PAVCInputEvent@@@Z @ 00c55d20 (seed 00c55d20) ===

/* [ported from ego_r via strfp] */

undefined1
_ConvertMouseEventToInputEvent_CMouseDX__IAE_NPBVCMouseEvent__PAVCInputEvent___Z(double *param_1)

{
  int iVar1;
  int in_ECX;
  int *piVar2;
  undefined1 local_51;
  int local_50;
  float local_4c;
  undefined4 local_48;
  undefined4 local_44;
  
  local_4c = (float)*param_1;
  local_48 = *(undefined4 *)(in_ECX + 0x341c);
  local_50 = 0;
  local_51 = 0;
  iVar1 = *(int *)(param_1 + 1);
  local_44 = *(undefined4 *)(in_ECX + 0x3420);
  switch(iVar1) {
  case 4:
    local_50 = 5;
    break;
  case 5:
    local_50 = 0xb;
    break;
  case 6:
    local_50 = 8;
    break;
  case 7:
  case 8:
  case 9:
  case 10:
  case 0xb:
  case 0xc:
  case 0xd:
  case 0xe:
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
    break;
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
    local_50 = iVar1 + 7;
    break;
  default:
    goto switchD_00c55d67_default;
  }
  switch(iVar1) {
  case 4:
  case 5:
  case 6:
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
    piVar2 = (int *)**(int **)(in_ECX + 0x340c);
    if (piVar2 != *(int **)(in_ECX + 0x340c)) {
      do {
        iVar1 = _GetType_CInputEvent__QBE_AW4EInputEventType__XZ();
        if (iVar1 == local_50) {
          local_4c = (float)piVar2[0xf];
          break;
        }
        piVar2 = (int *)*piVar2;
      } while (piVar2 != (int *)*(int *)(in_ECX + 0x340c));
    }
  }
switchD_00c55d67_default:
  iVar1 = *(int *)(param_1 + 1);
  switch(iVar1) {
  case 1:
    _SetAsLeftMouseButtonPressed_CInputEvent__QAEXABVC2DVector__N_Z
              (&local_48,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20));
    return 1;
  case 2:
    _SetAsRightMouseButtonPressed_CInputEvent__QAEXABVC2DVector__N_Z
              (&local_48,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20));
    return 1;
  case 3:
    _SetAsMiddleMouseButtonPressed_CInputEvent__QAEXABVC2DVector__N_Z
              (&local_48,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20));
    return 1;
  case 4:
    _SetAsLeftMouseButtonReleased_CInputEvent__QAEXABVC2DVector__NN_Z
              (&local_48,(double)local_4c,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20));
    return 1;
  case 5:
    _SetAsRightMouseButtonReleased_CInputEvent__QAEXABVC2DVector__NN_Z
              (&local_48,(double)local_4c,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20));
    return 1;
  case 6:
    _SetAsMiddleMouseButtonReleased_CInputEvent__QAEXABVC2DVector__NN_Z
              (&local_48,(double)local_4c,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20));
    return 1;
  case 10:
    _SetAsMouseWheelMovement_CInputEvent__QAEXMN_Z
              (*(undefined4 *)((int)param_1 + 0xc),SUB84(*param_1,0),
               (int)((ulonglong)*param_1 >> 0x20));
    local_51 = 1;
    break;
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
    _SetAsExtraMouseButtonPressed_CInputEvent__QAEXABVC2DVector__NJ_Z
              (&local_48,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20),iVar1 + -0xf);
    return 1;
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
    _SetAsExtraMouseButtonReleased_CInputEvent__QAEXABVC2DVector__NNJ_Z
              (&local_48,(double)local_4c,SUB84(*param_1,0),(int)((ulonglong)*param_1 >> 0x20),
               iVar1 + -0x14);
    return 1;
  }
  return local_51;
}


// CALLEES of ?ConvertMouseEventToInputEvent@CMouseDX@@IAE_NPBVCMouseEvent@@PAVCInputEvent@@@Z:
//   00b6e860  ?SetAsMiddleMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z
//   00b6e600  ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
//   00b6e830  ?SetAsLeftMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z
//   00b6eb00  ?SetAsMouseWheelMovement@CInputEvent@@QAEXMN@Z
//   00b6e920  ?SetAsRightMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z
//   00b6ea70  ?SetAsExtraMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NNJ@Z
//   00b6e9e0  ?SetAsExtraMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@NJ@Z
//   00b6e9b0  ?SetAsRightMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z
//   00b6e740  ?SetAsLeftMouseButtonPressed@CInputEvent@@QAEXABVC2DVector@@N@Z
//   00b6e8f0  ?SetAsMiddleMouseButtonReleased@CInputEvent@@QAEXABVC2DVector@@NN@Z

//=== ?Update@CMouseDX@@UAEXXZ @ 00c56010 (seed 00c56010) ===

/* [ported from ego_r via strfp] */

void _Update_CMouseDX__UAEXXZ(void)

{
  HWND hWnd;
  int *piVar1;
  code *pcVar2;
  float fVar3;
  char cVar4;
  int iVar5;
  undefined4 uVar6;
  int *in_ECX;
  float *pfVar7;
  undefined8 uVar8;
  tagPOINT local_6c;
  float local_64;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  tagRECT local_4c;
  undefined1 local_34 [52];
  
  __0CInputEvent__QAE_XZ();
  local_64 = 0.0;
  local_60 = 0.0;
  local_5c = 0.0;
  local_58 = 0.0;
  local_54 = 0.0;
  local_50 = 0.0;
  _EmptyEventStore_CMouse__IAEXXZ();
  while( true ) {
    if ((char)in_ECX[0xd11] == '\x01') {
      cVar4 = _GetExclusiveModeDeviceMouseEvent_CMouseDX__IAE_NPAVCMouseEvent___Z();
    }
    else {
      cVar4 = _GetNonExclusiveModeDeviceMouseEvent_CMouseDX__IAE_NPAVCMouseEvent___Z();
    }
    if (cVar4 != '\x01') break;
    if (*(char *)((int)in_ECX + 0x3445) == '\x01') {
      _UpdateCurrentStatusFromEvent_CMouseDX__IAEXPBVCMouseEvent___Z();
      switch(local_4c.right) {
      case 7:
      case 0xc:
        local_64 = local_64 + (float)local_4c.bottom;
        break;
      case 8:
      case 0xd:
        local_60 = local_60 + (float)local_4c.bottom;
        break;
      case 9:
      case 0xe:
        local_5c = local_5c + (float)local_4c.bottom;
      }
      cVar4 = _ConvertMouseEventToInputEvent_CMouseDX__IAE_NPBVCMouseEvent__PAVCInputEvent___Z
                        (&local_4c,local_34);
      if (cVar4 != '\0') {
        _AddEventToStore_CMouse__IAE_NPBVCInputEvent___Z();
        _ProcessMaintainedEventsFromEvent_CMouse__IAEXPBVCInputEvent___Z();
      }
    }
  }
  fVar3 = ABS(local_64);
  local_6c.x = (LONG)fVar3;
  if (((0.001 < fVar3) || (local_6c.x = (LONG)ABS(local_60), 0.001 < (float)local_6c.x)) ||
     (local_6c.x = (LONG)ABS(local_5c), 0.001 < (float)local_6c.x)) {
    if (*(char *)((int)in_ECX + 0x4853) == '\0') {
      local_58 = local_64;
      local_54 = local_60;
      local_50 = local_5c;
    }
    else {
      local_58 = local_64;
      if ((0.0 < fVar3) && (0.0 < ABS((float)in_ECX[0x1215]))) {
        local_58 = ((float)in_ECX[0x1215] + local_64) * 0.5;
      }
      local_54 = local_60;
      if ((0.0 < ABS(local_60)) && (0.0 < ABS((float)in_ECX[0x1216]))) {
        local_54 = ((float)in_ECX[0x1216] + local_60) * 0.5;
      }
      local_6c.x = (LONG)ABS(local_5c);
      if (((float)local_6c.x <= 0.0) ||
         (local_6c.x = (LONG)ABS((float)in_ECX[0x1217]), (float)local_6c.x <= 0.0)) {
        local_50 = local_5c;
      }
      else {
        local_50 = (local_5c + (float)in_ECX[0x1217]) * 0.5;
      }
    }
    uVar8 = CONCAT44(local_4c.top,local_4c.left);
    in_ECX[0x1215] = (int)local_64;
    in_ECX[0x1216] = (int)local_60;
    pcVar2 = *(code **)(*in_ECX + 8);
    in_ECX[0x1217] = (int)local_5c;
    pfVar7 = &local_58;
    uVar6 = (*pcVar2)(pfVar7,uVar8);
    _SetAsMouseMovement_CInputEvent__QAEXABVC2DVector__ABVC3DVector__N_Z(uVar6,pfVar7,uVar8);
    _AddEventToStore_CMouse__IAE_NPBVCInputEvent___Z();
  }
  if (*(char *)((int)in_ECX + 0x3445) != '\0') {
    _UpdateMaintainedEvents_CMouse__IAEXXZ();
    _AddMaintainedEventsToEventStore_CMouse__IAEXXZ();
  }
  if (*(char *)((int)in_ECX + 0x3446) != '\0') {
    iVar5 = _GFGetSystemManager__YIPAVCSystemManager__XZ();
    hWnd = *(HWND *)(iVar5 + 0x8c);
    local_6c.x = 0;
    local_6c.y = 0;
    ClientToScreen(hWnd,&local_6c);
    GetClientRect(hWnd,&local_4c);
    OffsetRect(&local_4c,local_6c.x,local_6c.y);
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    _GetDimensions_CSurface__QBE_AVC2DExtentsI__XZ();
    SetCursorPos((int)local_64 / 2 + local_4c.left,(int)local_60 / 2 + local_4c.top);
    in_ECX[0xd07] = (int)(float)((int)local_64 / 2);
    local_6c.x = (LONG)(float)((int)local_60 / 2);
    in_ECX[0xd08] = local_6c.x;
    in_ECX[0xd10] = local_6c.x;
    in_ECX[0xd0f] = (int)(float)((int)local_64 / 2);
  }
  iVar5 = 0;
  if (0 < in_ECX[0xd01]) {
    do {
      _GetEventFromStore_CMouse__IBEPBVCInputEvent__K_Z();
      _GetType_CInputEvent__QBE_AW4EInputEventType__XZ();
      iVar5 = iVar5 + 1;
    } while (iVar5 < in_ECX[0xd01]);
  }
  if ((char)in_ECX[0xd11] != '\0') {
    iVar5 = _GFGetSystemManager__YIPAVCSystemManager__XZ();
    piVar1 = *(int **)(*(int *)(iVar5 + 0x60) + 0x58);
    iVar5 = *piVar1;
    uVar6 = __ftol2_sse();
    uVar6 = __ftol2_sse(uVar6);
    (**(code **)(iVar5 + 0x2c))(piVar1,uVar6);
    __1CInputEvent__QAE_XZ();
    return;
  }
  (**(code **)(*in_ECX + 0xc))();
  iVar5 = _GFGetSystemManager__YIPAVCSystemManager__XZ();
  piVar1 = *(int **)(*(int *)(iVar5 + 0x60) + 0x58);
  iVar5 = *piVar1;
  uVar6 = __ftol2_sse(1);
  uVar6 = __ftol2_sse(uVar6);
  (**(code **)(iVar5 + 0x2c))(piVar1,uVar6);
  __1CInputEvent__QAE_XZ();
  return;
}


// CALLEES of ?Update@CMouseDX@@UAEXXZ:
//   00b6e600  ?GetType@CInputEvent@@QBE?AW4EInputEventType@@XZ
//   00af3080  ?GFGetSystemManager@@YIPAVCSystemManager@@XZ
//   EXTERNAL:00000154  OffsetRect
//   00c08a50  ?GetEventFromStore@CMouse@@IBEPBVCInputEvent@@K@Z
//   00b6e5f0  ??1CInputEvent@@QAE@XZ
//   EXTERNAL:00000155  SetCursorPos
//   00b6efe0  ??0CInputEvent@@QAE@XZ
//   00c55470  ?GetNonExclusiveModeDeviceMouseEvent@CMouseDX@@IAE_NPAVCMouseEvent@@@Z
//   00c08af0  ?UpdateMaintainedEvents@CMouse@@IAEXXZ
//   00c08da0  ?AddMaintainedEventsToEventStore@CMouse@@IAEXXZ
//   00b6eab0  ?SetAsMouseMovement@CInputEvent@@QAEXABVC2DVector@@ABVC3DVector@@N@Z
//   00c08a00  ?EmptyEventStore@CMouse@@IAEXXZ
//   00b46c70  ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ
//   00c55a60  ?UpdateCurrentStatusFromEvent@CMouseDX@@IAEXPBVCMouseEvent@@@Z
//   00c55d20  ?ConvertMouseEventToInputEvent@CMouseDX@@IAE_NPBVCMouseEvent@@PAVCInputEvent@@@Z
//   00c08a10  ?AddEventToStore@CMouse@@IAE_NPBVCInputEvent@@@Z
//   00c96090  __ftol2_sse
//   00c551c0  ?GetExclusiveModeDeviceMouseEvent@CMouseDX@@IAE_NPAVCMouseEvent@@@Z
//   00c08f00  ?ProcessMaintainedEventsFromEvent@CMouse@@IAEXPBVCInputEvent@@@Z
//   EXTERNAL:0000017c  ClientToScreen
//   EXTERNAL:0000016f  GetClientRect

//=== ?UpdateCurrentStatusFromEvent@CMouseDX@@IAEXPBVCMouseEvent@@@Z @ 00c55a60 (seed 00c55a60) ===

/* [ported from ego_r via strfp] */

void _UpdateCurrentStatusFromEvent_CMouseDX__IAEXPBVCMouseEvent___Z(int param_1)

{
  float fVar1;
  int in_ECX;
  int local_8;
  int local_4;
  
  _GFGetSystemManager__YIPAVCSystemManager__XZ();
  _GetDimensions_CSurface__QBE_AVC2DExtentsI__XZ(&local_8);
  switch(*(undefined4 *)(param_1 + 8)) {
  case 1:
    *(undefined1 *)(in_ECX + 0x4850) = 1;
    return;
  case 2:
    *(undefined1 *)(in_ECX + 0x4852) = 1;
    return;
  case 3:
    *(undefined1 *)(in_ECX + 0x4851) = 1;
    return;
  case 4:
    *(undefined1 *)(in_ECX + 0x4850) = 0;
    return;
  case 5:
    *(undefined1 *)(in_ECX + 0x4852) = 0;
    return;
  case 6:
    *(undefined1 *)(in_ECX + 0x4851) = 0;
    return;
  case 7:
    fVar1 = *(float *)(param_1 + 0xc) + *(float *)(in_ECX + 0x341c);
    *(float *)(in_ECX + 0x341c) = fVar1;
    if (fVar1 < 0.0) {
      *(undefined4 *)(in_ECX + 0x341c) = 0;
      return;
    }
    if ((float)(local_8 + -1) < fVar1) {
      *(float *)(in_ECX + 0x341c) = (float)(local_8 + -1);
      return;
    }
    break;
  case 8:
    fVar1 = *(float *)(param_1 + 0xc) + *(float *)(in_ECX + 0x3420);
    *(float *)(in_ECX + 0x3420) = fVar1;
    if (fVar1 < 0.0) {
      *(undefined4 *)(in_ECX + 0x3420) = 0;
      return;
    }
    if ((float)(local_4 + -1) < fVar1) {
      *(float *)(in_ECX + 0x3420) = (float)(local_4 + -1);
      return;
    }
    break;
  case 0xb:
    *(undefined4 *)(in_ECX + 0x341c) = *(undefined4 *)(param_1 + 0x10);
    *(undefined4 *)(in_ECX + 0x3420) = *(undefined4 *)(param_1 + 0x14);
    return;
  }
  return;
}


// CALLEES of ?UpdateCurrentStatusFromEvent@CMouseDX@@IAEXPBVCMouseEvent@@@Z:
//   00af3080  ?GFGetSystemManager@@YIPAVCSystemManager@@XZ
//   00b46c70  ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ

//=== ?GetExclusiveModeDeviceMouseEvent@CMouseDX@@IAE_NPAVCMouseEvent@@@Z @ 00c551c0 (seed 00c551c0) ===

/* [ported from ego_r via strfp] */

undefined4 _GetExclusiveModeDeviceMouseEvent_CMouseDX__IAE_NPAVCMouseEvent___Z(double *param_1)

{
  int *piVar1;
  float fVar2;
  char cVar3;
  int iVar4;
  int *in_ECX;
  undefined4 uVar5;
  int *piStack_4;
  
  piStack_4 = in_ECX;
  if (in_ECX[0x1213] <= in_ECX[0x1212]) {
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    cVar3 = _IsApplicationActive_CSystemManager__QAE_NXZ();
    if (cVar3 == '\0') {
LAB_00c551fb:
      uVar5 = 0;
    }
    else {
      _GFGetSystemManager__YIPAVCSystemManager__XZ();
      cVar3 = _IsApplicationInputFocus_CSystemManager__QAE_NXZ();
      if (cVar3 == '\0') goto LAB_00c551fb;
      uVar5 = 1;
    }
    (**(code **)(*in_ECX + 0x38))(uVar5);
    if (((char)in_ECX[0xd0b] == '\x01') && (piVar1 = (int *)in_ECX[0xd09], piVar1 != (int *)0x0)) {
      piStack_4 = (int *)&DAT_00000100;
      iVar4 = (**(code **)(*piVar1 + 0x28))(piVar1,0x14,in_ECX + 0xd12,&piStack_4,0);
      if (iVar4 == -0x7ff8ffe2) {
        (**(code **)(*(int *)in_ECX[0xd09] + 0x1c))((int *)in_ECX[0xd09]);
        iVar4 = (**(code **)(*(int *)in_ECX[0xd09] + 0x28))
                          ((int *)in_ECX[0xd09],0x14,in_ECX + 0xd12,&stack0xfffffff8,0);
      }
      if (-1 < iVar4) {
        in_ECX[0x1212] = 0;
        in_ECX[0x1213] = (int)piStack_4;
      }
    }
  }
  iVar4 = in_ECX[0x1212];
  if (in_ECX[0x1213] <= iVar4) {
    return 0;
  }
  piVar1 = in_ECX + iVar4 * 5 + 0xd12;
  in_ECX[0x1212] = iVar4 + 1;
  switch(*piVar1) {
  case 0:
    *(undefined4 *)(param_1 + 1) = 7;
    iVar4 = __ftol2_sse();
    *(float *)((int)param_1 + 0xc) = (float)iVar4;
    break;
  default:
    return 0;
  case 4:
    *(undefined4 *)(param_1 + 1) = 8;
    iVar4 = __ftol2_sse();
    *(float *)((int)param_1 + 0xc) = (float)iVar4;
    break;
  case 8:
    *(undefined4 *)(param_1 + 1) = 10;
    *(float *)((int)param_1 + 0xc) = (float)piVar1[1];
    break;
  case 0xc:
    *(uint *)(param_1 + 1) = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffd) + 4;
    break;
  case 0xd:
    iVar4 = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffd) + 5;
    goto LAB_00c553f2;
  case 0xe:
    *(uint *)(param_1 + 1) = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffd) + 6;
    break;
  case 0xf:
    *(uint *)(param_1 + 1) = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffb) + 0x14;
    break;
  case 0x10:
    iVar4 = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffb) + 0x15;
    goto LAB_00c553f2;
  case 0x11:
    *(uint *)(param_1 + 1) = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffb) + 0x16;
    break;
  case 0x12:
    *(uint *)(param_1 + 1) = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffb) + 0x17;
    break;
  case 0x13:
    iVar4 = (-(uint)((*(byte *)(piVar1 + 1) & 0x80) != 0) & 0xfffffffb) + 0x18;
LAB_00c553f2:
    *(int *)(param_1 + 1) = iVar4;
  }
  fVar2 = (float)piVar1[2];
  if (piVar1[2] < 0) {
    fVar2 = fVar2 + 4.2949673e+09;
  }
  *param_1 = (double)(fVar2 / 1000.0);
  return 1;
}


// CALLEES of ?GetExclusiveModeDeviceMouseEvent@CMouseDX@@IAE_NPAVCMouseEvent@@@Z:
//   00af3800  ?IsApplicationActive@CSystemManager@@QAE_NXZ
//   00af3080  ?GFGetSystemManager@@YIPAVCSystemManager@@XZ
//   00c96090  __ftol2_sse
//   00af3170  ?IsApplicationInputFocus@CSystemManager@@QAE_NXZ

