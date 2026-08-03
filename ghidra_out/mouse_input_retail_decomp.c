//=== ConvertMouseEventToInputEvent @ 00ab5420 (seed 00ab5420) ===

/* [bsim sim=1.0 <- ego_r]
   protected: bool __thiscall CMouseDX::ConvertMouseEventToInputEvent(class CMouseEvent const
   *,class CInputEvent *) */

bool __thiscall
CMouseDX::ConvertMouseEventToInputEvent(CMouseDX *this,CMouseEvent *param_1,CInputEvent *param_2)

{
  int iVar1;
  CIDrawEnvironment *pCVar2;
  undefined4 unaff_EBX;
  int *piVar3;
  undefined4 unaff_ESI;
  long unaff_EDI;
  bool local_19;
  CIDrawEnvironment *local_18;
  float local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  local_14 = (float)*(double *)param_1;
  local_c = *(undefined4 *)(this + 0x3418);
  local_18 = (CIDrawEnvironment *)0x0;
  local_19 = false;
  local_10 = *(undefined4 *)(this + 0x3414);
  iVar1 = *(int *)(param_1 + 8);
  switch(iVar1) {
  case 4:
    local_18 = (CIDrawEnvironment *)&DAT_00000005;
    break;
  case 5:
    local_18 = (CIDrawEnvironment *)0xb;
    break;
  case 6:
    local_18 = (CIDrawEnvironment *)&DAT_00000008;
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
    local_18 = (CIDrawEnvironment *)(iVar1 + 7);
    break;
  default:
    goto switchD_00ab5467_default;
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
    piVar3 = (int *)**(int **)(this + 0x340c);
    if (piVar3 != *(int **)(this + 0x340c)) {
      do {
        pCVar2 = CWorld::DrawGetEnvironment((CWorld *)(piVar3 + 3));
        if (pCVar2 == local_18) {
          local_14 = (float)piVar3[0xf];
          break;
        }
        piVar3 = (int *)*piVar3;
      } while (piVar3 != (int *)*(int *)(this + 0x340c));
    }
  }
switchD_00ab5467_default:
  iVar1 = *(int *)(param_1 + 8);
  switch(iVar1) {
  case 1:
    CInputEvent::SetAsLeftMouseButtonPressed
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)));
    return true;
  case 2:
    CInputEvent::SetAsLeftMouseButtonPressed
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)));
    return true;
  case 3:
    CInputEvent::SetAsLeftMouseButtonPressed
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)));
    return true;
  case 4:
    CInputEvent::SetAsLeftMouseButtonReleased
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44((int)*(undefined8 *)param_1,
                                (int)((ulonglong)(double)local_14 >> 0x20)),
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)));
    return true;
  case 5:
    CInputEvent::SetAsLeftMouseButtonReleased
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44((int)*(undefined8 *)param_1,
                                (int)((ulonglong)(double)local_14 >> 0x20)),
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)));
    return true;
  case 6:
    CInputEvent::SetAsLeftMouseButtonReleased
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44((int)*(undefined8 *)param_1,
                                (int)((ulonglong)(double)local_14 >> 0x20)),
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)));
    return true;
  case 10:
    CInputEvent::SetAsKeyboardKeyHeld
              (param_2,*(EInputKey *)(param_1 + 0xc),
               (double)CONCAT44(unaff_EDI,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)),
               (double)CONCAT44(unaff_EBX,unaff_ESI));
    local_19 = true;
    break;
  case 0xf:
  case 0x10:
  case 0x11:
  case 0x12:
  case 0x13:
    CInputEvent::SetAsExtraMouseButtonPressed
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44(iVar1 + -0xf,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)),
               unaff_EDI);
    return true;
  case 0x14:
  case 0x15:
  case 0x16:
  case 0x17:
  case 0x18:
    CInputEvent::SetAsExtraMouseButtonReleased
              (param_2,(C2DVector *)&local_10,
               (double)CONCAT44((int)*(undefined8 *)param_1,
                                (int)((ulonglong)(double)local_14 >> 0x20)),
               (double)CONCAT44(iVar1 + -0x14,(int)((ulonglong)*(undefined8 *)param_1 >> 0x20)),
               unaff_EDI);
    return true;
  }
  return local_19;
}


// CALLEES of ConvertMouseEventToInputEvent:
//   00a03f70  SetAsExtraMouseButtonReleased
//   00a03e10  SetAsLeftMouseButtonReleased
//   00a03d90  SetAsLeftMouseButtonPressed
//   00a03b40  DrawGetEnvironment
//   00a03ef0  SetAsExtraMouseButtonPressed
//   00a03e40  SetAsLeftMouseButtonPressed
//   00a03c80  SetAsLeftMouseButtonPressed
//   00a03d60  SetAsLeftMouseButtonReleased
//   00a03ec0  SetAsLeftMouseButtonReleased
//   00a04000  SetAsKeyboardKeyHeld

//=== ProcessMaintainedEventsFromEvent @ 00a66fd0 (seed 00a66fd0) ===

/* [bsim sim=0.8006018080216989 <- ego_r]
   protected: void __thiscall CMouse::ProcessMaintainedEventsFromEvent(class CInputEvent const *) */

void __thiscall CMouse::ProcessMaintainedEventsFromEvent(CMouse *this,CInputEvent *param_1)

{
  int iVar1;
  CIDrawEnvironment *pCVar2;
  undefined4 *puVar3;
  CIDrawEnvironment *pCVar4;
  int *piVar5;
  int *piVar6;
  double local_38;
  undefined4 local_30;
  undefined4 local_2c;
  double local_28;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  
  pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (pCVar2 == (CIDrawEnvironment *)&DAT_00000004) {
    CInputEvent_AllocNode(&local_38,*(undefined4 *)(this + 0x340c));
    iVar1 = *(int *)(*(int *)(this + 0x340c) + 4);
    puVar3 = (undefined4 *)
             CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing
                       ((CMessageEventFilter_IsOriginatedByThing *)param_1,(CThing *)&local_28);
    local_30 = *puVar3;
    local_2c = puVar3[1];
    local_20 = 0;
    local_1c = 0;
    local_18 = 0;
    CInputEvent::SetAsLeftMouseButtonHeld
              ((CInputEvent *)(iVar1 + 0xc),(C2DVector *)&local_30,(C3DVector *)&local_20,
               (double)*(float *)(param_1 + 0x30),(double)*(float *)(param_1 + 0x30));
    return;
  }
  pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (pCVar2 == (CIDrawEnvironment *)&DAT_00000007) {
    CInputEvent_AllocNode(&local_38,*(undefined4 *)(this + 0x340c));
    iVar1 = *(int *)(*(int *)(this + 0x340c) + 4);
    puVar3 = (undefined4 *)
             CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing
                       ((CMessageEventFilter_IsOriginatedByThing *)param_1,(CThing *)&local_28);
    local_30 = *puVar3;
    local_2c = puVar3[1];
    local_20 = 0;
    local_1c = 0;
    local_18 = 0;
    CInputEvent::SetAsLeftMouseButtonHeld
              ((CInputEvent *)(iVar1 + 0xc),(C2DVector *)&local_30,(C3DVector *)&local_20,
               (double)*(float *)(param_1 + 0x30),(double)*(float *)(param_1 + 0x30));
    return;
  }
  pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (pCVar2 == (CIDrawEnvironment *)0xa) {
    CInputEvent_AllocNode(&local_38,*(undefined4 *)(this + 0x340c));
    iVar1 = *(int *)(*(int *)(this + 0x340c) + 4);
    puVar3 = (undefined4 *)
             CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing
                       ((CMessageEventFilter_IsOriginatedByThing *)param_1,(CThing *)&local_28);
    local_30 = *puVar3;
    local_2c = puVar3[1];
    local_20 = 0;
    local_1c = 0;
    local_18 = 0;
    CInputEvent::SetAsLeftMouseButtonHeld
              ((CInputEvent *)(iVar1 + 0xc),(C2DVector *)&local_30,(C3DVector *)&local_20,
               (double)*(float *)(param_1 + 0x30),(double)*(float *)(param_1 + 0x30));
    return;
  }
  pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (pCVar2 == (CIDrawEnvironment *)&DAT_00000006) {
    piVar5 = (int *)**(int **)(this + 0x340c);
    if (piVar5 != *(int **)(this + 0x340c)) {
      do {
        pCVar2 = CWorld::DrawGetEnvironment((CWorld *)(piVar5 + 3));
        if (pCVar2 == (CIDrawEnvironment *)&DAT_00000005) {
          EventNode_Unlink(&local_38,piVar5);
          return;
        }
        piVar5 = (int *)*piVar5;
      } while (piVar5 != (int *)*(int *)(this + 0x340c));
      return;
    }
  }
  else {
    pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
    if (pCVar2 == (CIDrawEnvironment *)&DAT_0000000c) {
      piVar5 = (int *)**(int **)(this + 0x340c);
      if (piVar5 != *(int **)(this + 0x340c)) {
        do {
          pCVar2 = CWorld::DrawGetEnvironment((CWorld *)(piVar5 + 3));
          if (pCVar2 == (CIDrawEnvironment *)0xb) {
            EventNode_Unlink(&local_38,piVar5);
            return;
          }
          piVar5 = (int *)*piVar5;
        } while (piVar5 != (int *)*(int *)(this + 0x340c));
        return;
      }
    }
    else {
      pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
      if (pCVar2 == (CIDrawEnvironment *)&DAT_00000009) {
        piVar6 = (int *)**(int **)(this + 0x340c);
        if (piVar6 == *(int **)(this + 0x340c)) {
          return;
        }
        while (pCVar2 = CWorld::DrawGetEnvironment((CWorld *)(piVar6 + 3)),
              pCVar2 != (CIDrawEnvironment *)&DAT_00000008) {
          piVar6 = (int *)*piVar6;
          if (piVar6 == (int *)*(int *)(this + 0x340c)) {
            return;
          }
        }
      }
      else {
        pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
        switch(pCVar2) {
        case (CIDrawEnvironment *)0x16:
        case (CIDrawEnvironment *)0x17:
        case (CIDrawEnvironment *)0x18:
        case (CIDrawEnvironment *)0x19:
        case (CIDrawEnvironment *)0x1a:
          CDefString::ToWideString((CDefString *)(this + 0x340c));
          iVar1 = *(int *)(*(int *)(this + 0x340c) + 4);
          puVar3 = (undefined4 *)
                   CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing
                             ((CMessageEventFilter_IsOriginatedByThing *)param_1,(CThing *)&local_20
                             );
          local_30 = *puVar3;
          local_38 = (double)*(float *)(param_1 + 0x30);
          local_2c = puVar3[1];
          local_28 = (double)*(float *)(param_1 + 0x30);
          local_14 = 0;
          local_10 = 0;
          local_c = 0;
          pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
          CInputEvent::SetAsExtraMouseButtonHeld
                    ((CInputEvent *)(iVar1 + 0xc),(C2DVector *)&local_30,(C3DVector *)&local_14,
                     local_28,local_38,(long)(pCVar2 + -0x16));
          return;
        default:
          goto switchD_00a67292_caseD_1b;
        case (CIDrawEnvironment *)0x20:
        case (CIDrawEnvironment *)0x21:
        case (CIDrawEnvironment *)0x22:
        case (CIDrawEnvironment *)0x23:
        case (CIDrawEnvironment *)0x24:
          pCVar2 = CWorld::DrawGetEnvironment((CWorld *)param_1);
          piVar5 = *(int **)(this + 0x340c);
          piVar6 = (int *)*piVar5;
          if (piVar6 == piVar5) {
            return;
          }
          while (pCVar4 = CWorld::DrawGetEnvironment((CWorld *)(piVar6 + 3)), pCVar4 != pCVar2 + -5)
          {
            piVar6 = (int *)*piVar6;
            if (piVar6 == *(int **)(this + 0x340c)) {
              return;
            }
          }
        }
      }
      EventNode_Unlink(&local_38,piVar6);
    }
  }
switchD_00a67292_caseD_1b:
  return;
}


// CALLEES of ProcessMaintainedEventsFromEvent:
//   009e47e0  EventNode_Unlink
//   00a03dc0  SetAsLeftMouseButtonHeld
//   00a03f20  SetAsExtraMouseButtonHeld
//   00a03b40  DrawGetEnvironment
//   00a03d10  SetAsLeftMouseButtonHeld
//   009e4aa0  ToWideString
//   009e4a20  CInputEvent_AllocNode
//   00a04380  CMessageEventFilter_IsOriginatedByThing
//   00a03e70  SetAsLeftMouseButtonHeld

//=== ResetNonExclusiveMouse @ 00ab5030 (seed 00ab50a0) ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   protected: void __thiscall CMouseDX::ResetNonExclusiveMouse(void) */

void __thiscall CMouseDX::ResetNonExclusiveMouse(CMouseDX *this)

{
  HWND hWnd;
  CMouseDX CVar1;
  ulong uVar2;
  int iVar3;
  CTCInventoryBase *pCVar4;
  CEngineInternalPrimitiveBase *this_00;
  CEngineInternalPrimitiveBase *this_01;
  CEngineInternalPrimitiveBase *this_02;
  CEngineInternalPrimitiveBase *this_03;
  CEngineInternalPrimitiveBase *this_04;
  CEngineInternalPrimitiveBase *this_05;
  CEnginePrimitive *unaff_EBP;
  CEnginePrimitive *unaff_ESI;
  CEnginePrimitive *unaff_EDI;
  ulong in_stack_ffffffd0;
  int iVar5;
  tagPOINT local_28;
  tagPOINT local_20;
  int local_18;
  int local_14;
  tagRECT local_10;
  
  uVar2 = CEngineInternalPrimitiveBase::AddChildPrimitive
                    ((CEngineInternalPrimitiveBase *)this,unaff_EBP,in_stack_ffffffd0);
  if (*(CEngineInternalPrimitiveBase **)(uVar2 + 0x60) != (CEngineInternalPrimitiveBase *)0x0) {
    uVar2 = CEngineInternalPrimitiveBase::AddChildPrimitive
                      (*(CEngineInternalPrimitiveBase **)(uVar2 + 0x60),unaff_ESI,(ulong)unaff_EBP);
    hWnd = *(HWND *)(uVar2 + 0x94);
    GetCursorPos(&local_20);
    local_28.x = 0;
    local_28.y = 0;
    ClientToScreen(hWnd,&local_28);
    GetClientRect(hWnd,&local_10);
    if ((((local_28.x <= local_20.x) && (local_20.x < local_10.right + local_28.x)) &&
        (local_28.y <= local_20.y)) && (local_20.y < local_10.bottom + local_28.y)) {
      this_00 = (CEngineInternalPrimitiveBase *)(local_20.y - local_28.y);
      iVar3 = local_20.x - local_28.x;
      iVar5 = iVar3;
      local_20.x = iVar3;
      local_20.y = (LONG)this_00;
      uVar2 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,unaff_EDI,(ulong)unaff_ESI);
      CSurface::GetDimensions((CSurface *)(*(int *)(uVar2 + 0x60) + 0x2c));
      if (((-1 < iVar3) && (iVar3 < local_18)) && ((-1 < (int)this_00 && ((int)this_00 < local_14)))
         ) {
        *(float *)(this + 0x3434) = (float)iVar5;
        *(float *)(this + 0x3438) = (float)(int)this_00;
        *(float *)(this + 0x3414) = (float)iVar5;
        *(float *)(this + 0x3418) = (float)(int)this_00;
      }
      pCVar4 = (CTCInventoryBase *)
               CEngineInternalPrimitiveBase::AddChildPrimitive(this_01,unaff_EDI,(ulong)unaff_ESI);
      CVar1 = (CMouseDX)NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(pCVar4);
      this[0x342c] = CVar1;
      pCVar4 = (CTCInventoryBase *)
               CEngineInternalPrimitiveBase::AddChildPrimitive(this_02,unaff_EDI,(ulong)unaff_ESI);
      CVar1 = (CMouseDX)NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(pCVar4);
      this[0x342d] = CVar1;
      pCVar4 = (CTCInventoryBase *)
               CEngineInternalPrimitiveBase::AddChildPrimitive(this_03,unaff_EDI,(ulong)unaff_ESI);
      CVar1 = (CMouseDX)NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(pCVar4);
      this[0x342e] = CVar1;
      pCVar4 = (CTCInventoryBase *)
               CEngineInternalPrimitiveBase::AddChildPrimitive(this_04,unaff_EDI,(ulong)unaff_ESI);
      CVar1 = (CMouseDX)NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(pCVar4);
      this[0x342f] = CVar1;
      pCVar4 = (CTCInventoryBase *)
               CEngineInternalPrimitiveBase::AddChildPrimitive(this_05,unaff_EDI,(ulong)unaff_ESI);
      CVar1 = (CMouseDX)NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(pCVar4);
      this[0x3430] = CVar1;
    }
  }
  return;
}


// CALLEES of ResetNonExclusiveMouse:
//   009a4fe0  GetCanItemBeQuickAssigned
//   009a4ff0  GetCanItemBeQuickAssigned
//   009a5000  GetCanItemBeQuickAssigned
//   009a4ec0  AddChildPrimitive
//   009a4fc0  GetCanItemBeQuickAssigned
//   EXTERNAL:0000017e  GetCursorPos
//   EXTERNAL:00000162  GetClientRect
//   009a4fd0  GetCanItemBeQuickAssigned
//   009f2f90  GetDimensions
//   EXTERNAL:00000155  ClientToScreen

//=== EventScanStart @ 00aaf800 (seed 00aaf800) ===

/* [bsim sim=0.5010345927539245 <- ego_r]
   public: bool __thiscall CInputManager::EventScanStart(class CInputEventScanInfo &,class
   CInputEvent &)const  */

bool __thiscall
CInputManager::EventScanStart(CInputManager *this,CInputEventScanInfo *param_1,CInputEvent *param_2)

{
  undefined1 uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  puVar3 = *(undefined4 **)(this + 0x44);
  if (puVar3 == *(undefined4 **)(this + 0x48)) {
    uVar1 = Vector_InsertAt(puVar3,param_1,&param_1,1,1);
    return (bool)uVar1;
  }
  if (puVar3 != (undefined4 *)0x0) {
    for (iVar2 = 0xc; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar3 = *(undefined4 *)param_1;
      param_1 = param_1 + 4;
      puVar3 = puVar3 + 1;
    }
  }
  *(int *)(this + 0x44) = *(int *)(this + 0x44) + 0x30;
  return (bool)((char)this + '@');
}


// CALLEES of EventScanStart:
//   00aafe50  Vector_InsertAt

//=== IsMouseEventInQueue @ 009f4c00 (seed 009f4c00) ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: bool __thiscall CInputManager::IsMouseEventInQueue(enum EInputEventType,class CInputEvent
   &)const  */

bool __thiscall
CInputManager::IsMouseEventInQueue(CInputManager *this,EInputEventType param_1,CInputEvent *param_2)

{
  CIDrawEnvironment *pCVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar3 = 0;
  if (0 < *(int *)(this + 0x1c)) {
    iVar2 = 0;
    do {
      pCVar1 = CWorld::DrawGetEnvironment((CWorld *)(iVar2 + *(int *)(this + 0x10)));
      if (pCVar1 == (CIDrawEnvironment *)param_1) {
        puVar4 = (undefined4 *)(iVar3 * 0x34 + *(int *)(this + 0x10));
        for (iVar2 = 0xd; iVar2 != 0; iVar2 = iVar2 + -1) {
          *(undefined4 *)param_2 = *puVar4;
          puVar4 = puVar4 + 1;
          param_2 = param_2 + 4;
        }
        return true;
      }
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + 0x34;
    } while (iVar3 < *(int *)(this + 0x1c));
  }
  return false;
}


// CALLEES of IsMouseEventInQueue:
//   00a03b40  DrawGetEnvironment

//=== IsJoystickEventInQueue @ 009f4c60 (seed 009f4c60) ===

/* [bsim sim=1.0 <- ego_r]
   public: bool __thiscall CInputManager::IsJoystickEventInQueue(enum EInputEventType,long,class
   CInputEvent &,long)const  */

bool __thiscall
CInputManager::IsJoystickEventInQueue
          (CInputManager *this,EInputEventType param_1,long param_2,CInputEvent *param_3,
          long param_4)

{
  bool bVar1;
  CIDrawEnvironment *pCVar2;
  undefined3 extraout_var;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  
  iVar4 = 0;
  if (0 < *(int *)(this + 0x1c)) {
    iVar3 = 0;
    do {
      pCVar2 = CWorld::DrawGetEnvironment((CWorld *)(iVar3 + *(int *)(this + 0x10)));
      if (pCVar2 == (CIDrawEnvironment *)param_1) {
        pCVar2 = CWorld::DrawGetEnvironment((CWorld *)(iVar3 + *(int *)(this + 0x10)));
        if (pCVar2 == (CIDrawEnvironment *)param_4) {
          if (((param_1 != 0x13) && (param_1 != 0x14)) && (param_1 != 0x15)) {
LAB_009f4cc9:
            puVar5 = (undefined4 *)(iVar4 * 0x34 + *(int *)(this + 0x10));
            for (iVar3 = 0xd; iVar3 != 0; iVar3 = iVar3 + -1) {
              *(undefined4 *)param_3 = *puVar5;
              puVar5 = puVar5 + 1;
              param_3 = param_3 + 4;
            }
            return true;
          }
          bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned
                            ((CTCInventoryBase *)(iVar3 + *(int *)(this + 0x10)));
          if (CONCAT31(extraout_var,bVar1) == param_2) goto LAB_009f4cc9;
        }
      }
      iVar4 = iVar4 + 1;
      iVar3 = iVar3 + 0x34;
    } while (iVar4 < *(int *)(this + 0x1c));
  }
  return false;
}


// CALLEES of IsJoystickEventInQueue:
//   00a03b40  DrawGetEnvironment
//   00a03b60  DrawGetEnvironment
//   00a03b90  GetCanItemBeQuickAssigned

//=== SetAsMouseMovement @ 00a03fb0 (seed 00a03fb0) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: void __thiscall CInputEvent::SetAsMouseMovement(class C2DVector const &,class C3DVector
   const &,double) */

void __thiscall
CInputEvent::SetAsMouseMovement
          (CInputEvent *this,C2DVector *param_1,C3DVector *param_2,double param_3)

{
  undefined4 uVar1;
  
  *(undefined4 *)(this + 0x20) = 3;
  *(undefined4 *)(this + 0x28) = 0xd;
  *(undefined4 *)(this + 0xc) = *(undefined4 *)param_2;
  *(undefined4 *)(this + 0x10) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(this + 0x14) = *(undefined4 *)(param_2 + 8);
  *(undefined4 *)(this + 0x18) = *(undefined4 *)param_1;
  uVar1 = *(undefined4 *)(param_1 + 4);
  *(float *)(this + 0x30) = (float)param_3;
  *(float *)(this + 0x2c) = (float)param_3;
  *(undefined4 *)(this + 0x1c) = uVar1;
  return;
}


// CALLEES of SetAsMouseMovement:

//=== SetAsExtraMouseButtonPressed @ 00a03ef0 (seed 00a03ef0) ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: void __thiscall CInputEvent::SetAsExtraMouseButtonPressed(class C2DVector const
   &,double,long) */

void __thiscall
CInputEvent::SetAsExtraMouseButtonPressed
          (CInputEvent *this,C2DVector *param_1,double param_2,long param_3)

{
  undefined4 uVar1;
  undefined4 in_stack_00000008;
  
  *(int *)(this + 0x28) = param_2._4_4_ + 0x16;
  *(undefined4 *)(this + 0x20) = 3;
  *(undefined4 *)(this + 0x18) = *(undefined4 *)param_1;
  uVar1 = *(undefined4 *)(param_1 + 4);
  *(float *)(this + 0x30) = (float)(double)CONCAT44(param_2._0_4_,in_stack_00000008);
  *(float *)(this + 0x2c) = (float)(double)CONCAT44(param_2._0_4_,in_stack_00000008);
  *(undefined4 *)(this + 0x1c) = uVar1;
  return;
}


// CALLEES of SetAsExtraMouseButtonPressed:

