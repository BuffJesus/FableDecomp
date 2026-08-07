//=== GetButtonPressureForEvent @ 00445800 (seed 00445800) ===

/* [bsim sim=0.8973034607371664 <- ego_r]
   public: float __thiscall CGamePlayerInterface::GetButtonPressureForEvent(class CInputEvent const
   &)const  */

float __thiscall
CGamePlayerInterface::GetButtonPressureForEvent(CGamePlayerInterface *this,CInputEvent *param_1)

{
  CIDrawEnvironment *pCVar1;
  float unaff_ESI;
  float fVar2;
  
  pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (((pCVar1 == (CIDrawEnvironment *)&DAT_00000013) ||
      (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1),
      pCVar1 == (CIDrawEnvironment *)&DAT_00000014)) ||
     (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1), pCVar1 == (CIDrawEnvironment *)0x15))
  {
    fVar2 = CMap::DrawGetSeaLevel((CMap *)param_1,unaff_ESI);
    return fVar2;
  }
  pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1);
  if (((((pCVar1 != (CIDrawEnvironment *)0x1) &&
        (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1), pCVar1 != (CIDrawEnvironment *)0x2)
        ) && ((pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1),
              pCVar1 != (CIDrawEnvironment *)&DAT_00000004 &&
              ((pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1),
               pCVar1 != (CIDrawEnvironment *)&DAT_00000005 &&
               (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1),
               pCVar1 != (CIDrawEnvironment *)&DAT_00000007)))))) &&
      (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1),
      pCVar1 != (CIDrawEnvironment *)&DAT_00000008)) &&
     ((pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1), pCVar1 != (CIDrawEnvironment *)0xa &&
      (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1), pCVar1 != (CIDrawEnvironment *)0xb)))
     ) {
    pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1);
    if ((0x15 < (int)pCVar1) &&
       (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1), (int)pCVar1 < 0x1b)) {
      return 1.0;
    }
    pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1);
    if ((0x1a < (int)pCVar1) &&
       (pCVar1 = CWorld::DrawGetEnvironment((CWorld *)param_1), (int)pCVar1 < 0x20)) {
      return 1.0;
    }
    return 0.0;
  }
  return 1.0;
}


// CALLEES of GetButtonPressureForEvent:
//   00a03ba0  DrawGetSeaLevel
//   00a03b40  DrawGetEnvironment

//=== GetMouseMovement @ 004458f0 (seed 004458f0) ===

/* [bsim sim=1.0000000000000002 <- ego_r]
   public: class C2DVector __thiscall CGamePlayerInterface::GetMouseMovement(class CInputEvent const
   &)const  */

void __thiscall
CGamePlayerInterface::GetMouseMovement(CGamePlayerInterface *this,CInputEvent *param_1)

{
  CTCPhysicsRigidBody *in_stack_00000008;
  undefined4 local_c;
  float local_8;
  
  CTCPhysicsRigidBody::GetFacingVector(in_stack_00000008);
  *(undefined4 *)param_1 = local_c;
  *(float *)(param_1 + 4) = -local_8;
  return;
}


// CALLEES of GetMouseMovement:
//   00a043d0  GetFacingVector

//=== IsEventGameAction @ 00445bd0 (seed 00445bd0) ===

/* [bsim sim=0.5510748654226243 <- ego_r]
   public: bool __thiscall CGamePlayerInterface::IsEventGameAction(enum EGameAction,class
   CInputEvent const &)const  */

bool __thiscall
CGamePlayerInterface::IsEventGameAction
          (CGamePlayerInterface *this,EGameAction param_1,CInputEvent *param_2)

{
  CInputEvent *pCVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  
  iVar3 = CVectorMap<EGameAction,CCountedPointer<CInputTypeBase_const_>,CKeyPairCompareLess<EGameAction,CCountedPointer<CInputTypeBase_const_>_>_>
          ::UpperBound((CVectorMap<EGameAction,CCountedPointer<CInputTypeBase_const_>,CKeyPairCompareLess<EGameAction,CCountedPointer<CInputTypeBase_const_>_>_>
                        *)(this + 0x7a8),&param_1);
  iVar4 = CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing>_>_>
          ::LowerBound((CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing>_>_>
                        *)(this + 0x7a8),(long *)&param_1);
  pCVar1 = param_2;
  while( true ) {
    if (iVar4 == iVar3) {
      return false;
    }
    cVar2 = (**(code **)(**(int **)(iVar4 + 4) + 0xc))(pCVar1);
    if (cVar2 != '\0') break;
    iVar4 = iVar4 + 0xc;
  }
  return true;
}


// CALLEES of IsEventGameAction:
//   00447ce0  UpperBound
//   00447c80  LowerBound

//=== GetControlMovementFromGameActionEvent @ 00445c30 (seed 00445c30) ===

/* [bsim sim=0.6641073649861536 <- ego_r]
   public: bool __thiscall CGamePlayerInterface::GetControlMovementFromGameActionEvent(enum
   EGameAction,class CInputEvent const &,class C2DVector &)const  */

bool __thiscall
CGamePlayerInterface::GetControlMovementFromGameActionEvent
          (CGamePlayerInterface *this,EGameAction param_1,CInputEvent *param_2,C2DVector *param_3)

{
  int *piVar1;
  CInputEvent *pCVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined1 auStack_8 [8];
  
  iVar4 = CVectorMap<EGameAction,CCountedPointer<CInputTypeBase_const_>,CKeyPairCompareLess<EGameAction,CCountedPointer<CInputTypeBase_const_>_>_>
          ::UpperBound((CVectorMap<EGameAction,CCountedPointer<CInputTypeBase_const_>,CKeyPairCompareLess<EGameAction,CCountedPointer<CInputTypeBase_const_>_>_>
                        *)(this + 0x7a8),&param_1);
  iVar5 = CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing>_>_>
          ::LowerBound((CVectorMap<long,CIntelligentPointer<CThing>,CKeyPairCompareLess<long,CIntelligentPointer<CThing>_>_>
                        *)(this + 0x7a8),(long *)&param_1);
  pCVar2 = param_2;
  while( true ) {
    if (iVar5 == iVar4) {
      return false;
    }
    piVar1 = *(int **)(iVar5 + 4);
    cVar3 = (**(code **)(*piVar1 + 0xc))(pCVar2);
    if (cVar3 != '\0') break;
    iVar5 = iVar5 + 0xc;
  }
  puVar6 = (undefined4 *)(**(code **)(*piVar1 + 0x10))(auStack_8,pCVar2);
  *(undefined4 *)param_1 = *puVar6;
  *(undefined4 *)(param_1 + 4) = puVar6[1];
  return true;
}


// CALLEES of GetControlMovementFromGameActionEvent:
//   00447ce0  UpperBound
//   00447c80  LowerBound

//=== IsEventAssignableSpecialMoveButton @ 00445e30 (seed 00445e30) ===

/* [bsim sim=1.0 <- ego_r]
   public: bool __thiscall CGamePlayerInterface::IsEventAssignableSpecialMoveButton(class
   CInputEvent const &)const  */

bool __thiscall
CGamePlayerInterface::IsEventAssignableSpecialMoveButton
          (CGamePlayerInterface *this,CInputEvent *param_1)

{
  char cVar1;
  uint uVar2;
  
  if (*(int *)(this + 0x7bc) - *(int *)(this + 0x7b8) >> 3 != 0) {
    uVar2 = 0;
    do {
      cVar1 = (**(code **)(**(int **)(*(int *)(this + 0x7b8) + uVar2 * 8) + 0xc))(param_1);
      if (cVar1 != '\0') {
        return true;
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < (uint)(*(int *)(this + 0x7bc) - *(int *)(this + 0x7b8) >> 3));
  }
  return false;
}


// CALLEES of IsEventAssignableSpecialMoveButton:

//=== IsEventContextSensitiveItemButton @ 00446000 (seed 00446000) ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: bool __thiscall CGamePlayerInterface::IsEventContextSensitiveItemButton(class CInputEvent
   const &,long &)const  */

bool __thiscall
CGamePlayerInterface::IsEventContextSensitiveItemButton
          (CGamePlayerInterface *this,CInputEvent *param_1,long *param_2)

{
  char cVar1;
  uint uVar2;
  
  if (*(int *)(this + 0x7c8) - *(int *)(this + 0x7c4) >> 3 != 0) {
    uVar2 = 0;
    do {
      cVar1 = (**(code **)(**(int **)(*(int *)(this + 0x7c4) + uVar2 * 8) + 0xc))(param_1);
      if (cVar1 != '\0') {
        *param_2 = uVar2;
        return true;
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < (uint)(*(int *)(this + 0x7c8) - *(int *)(this + 0x7c4) >> 3));
  }
  return false;
}


// CALLEES of IsEventContextSensitiveItemButton:

//=== IsEventContextSensitiveItemButton @ 00446110 (seed 00446110) ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: bool __thiscall CGamePlayerInterface::IsEventContextSensitiveItemButton(class CInputEvent
   const &,long &)const  */

bool __thiscall
CGamePlayerInterface::IsEventContextSensitiveItemButton
          (CGamePlayerInterface *this,CInputEvent *param_1,long *param_2)

{
  char cVar1;
  uint uVar2;
  
  if (*(int *)(this + 0x7d4) - *(int *)(this + 2000) >> 3 != 0) {
    uVar2 = 0;
    do {
      cVar1 = (**(code **)(**(int **)(*(int *)(this + 2000) + uVar2 * 8) + 0xc))(param_1);
      if (cVar1 != '\0') {
        *param_2 = uVar2;
        return true;
      }
      uVar2 = uVar2 + 1;
    } while (uVar2 < (uint)(*(int *)(this + 0x7d4) - *(int *)(this + 2000) >> 3));
  }
  return false;
}


// CALLEES of IsEventContextSensitiveItemButton:

//=== GetInputProcessEventFromUpdate @ 00446220 (seed 00446220) ===

/* WARNING: Function: __EH_epilog3 replaced with injection: EH_epilog3 */
/* WARNING: Unable to track spacebase fully for stack */
/* [bsim sim=1.0000000000000002 <- ego_r]
   private: bool __thiscall CGamePlayerInterface::GetInputProcessEventFromUpdate(class
   CProcessedInput &)const  */

bool __thiscall
CGamePlayerInterface::GetInputProcessEventFromUpdate
          (CGamePlayerInterface *this,CProcessedInput *param_1)

{
  int iVar1;
  code *pcVar2;
  undefined4 *puVar3;
  int *piVar4;
  int iVar5;
  undefined1 *puVar6;
  undefined4 *puVar7;
  int unaff_EBP;
  
  if (this[0x894] != (CGamePlayerInterface)0x0) {
    return false;
  }
  this[0x894] = (CGamePlayerInterface)0x1;
  CGameEventPackage::CGameEventPackage((CGameEventPackage *)param_1);
  CWorld::DrawGetEnvironment(*(CWorld **)(this + 0x6fc));
  puVar6 = (undefined1 *)(unaff_EBP + 4);
  iVar1 = *(int *)(this + 0x10);
  *(int *)(unaff_EBP + 0x20) = iVar1;
  iVar5 = 0;
  if (iVar1 != 0) {
    *(int *)(iVar1 + 0x14) = unaff_EBP + 0x10;
    iVar5 = *(int *)(unaff_EBP + 0x20);
  }
  *(int *)(this + 0x10) = unaff_EBP + 0x10;
  *(CGamePlayerInterface **)(unaff_EBP + 0x28) = this + 4;
  piVar4 = *(int **)(this + 4);
  *(undefined4 *)(unaff_EBP + 0x24) = 0;
  *(undefined1 *)(unaff_EBP + 0x1c) = 1;
  if (piVar4 != (int *)0x0) {
    *(int **)(unaff_EBP + 0x10) = piVar4;
    *(int *)(unaff_EBP + 0x14) = piVar4[1];
    *(undefined4 *)(unaff_EBP + 0x18) = 0;
    piVar4 = (int *)*piVar4;
    if (piVar4 != (int *)0x0) {
      do {
        iVar1 = *piVar4;
        *(CProcessedInput **)(puVar6 + -4) = param_1;
        pcVar2 = *(code **)(iVar1 + 0x18);
        puVar7 = (undefined4 *)(puVar6 + -8);
        puVar6 = puVar6 + -8;
        *puVar7 = 0x4462bd;
        (*pcVar2)();
        puVar3 = *(undefined4 **)(puVar6 + 0x10);
        *(undefined4 **)(puVar6 + 0xc) = puVar3;
        if (puVar3 == (undefined4 *)0x0) break;
        *(undefined4 *)(puVar6 + 0x10) = puVar3[1];
        *(undefined4 *)(puVar6 + 0x14) = puVar3[2];
        piVar4 = (int *)*puVar3;
      } while (piVar4 != (int *)0x0);
      if (*(int *)(puVar6 + 0x20) != 0) {
        *(undefined4 *)(*(int *)(puVar6 + 0x20) + 0x10) = *(undefined4 *)(puVar6 + 0x1c);
        if (*(int *)(puVar6 + 0x1c) != 0) {
          *(undefined4 *)(*(int *)(puVar6 + 0x1c) + 0x14) = *(undefined4 *)(puVar6 + 0x20);
        }
        goto LAB_0044630b;
      }
      iVar5 = *(int *)(puVar6 + 0x1c);
    }
  }
  *(int *)(this + 0x10) = iVar5;
  if (iVar5 != 0) {
    *(undefined4 *)(iVar5 + 0x14) = 0;
  }
LAB_0044630b:
  if (param_1[0xa8] == (CProcessedInput)0x0) {
    return false;
  }
  return true;
}


// CALLEES of GetInputProcessEventFromUpdate:
//   00449700  DrawGetEnvironment
//   00a0d3a0  __EH_epilog3
//   00a0d300  CGameEventPackage

//=== GetInputProcessEvent @ 00446330 (seed 00446330) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Function: __EH_epilog3 replaced with injection: EH_epilog3 */
/* [bsim sim=0.995207890897863 <- ego_r]
   private: bool __thiscall CGamePlayerInterface::GetInputProcessEvent(class CProcessedInput &)const
    */

bool __thiscall
CGamePlayerInterface::GetInputProcessEvent(CGamePlayerInterface *this,CProcessedInput *param_1)

{
  int *piVar1;
  code *pcVar2;
  bool bVar3;
  char cVar4;
  bool bVar5;
  CIDrawEnvironment *pCVar6;
  EInputKey EVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  int *piVar11;
  CGameEventPackage *pCVar12;
  CDefaultEngineMemoryInit *pCVar13;
  CGameEventPackage *pCVar14;
  CDefaultEngineMemoryInit *pCVar15;
  bool local_23a;
  int *piStack_238;
  int *piStack_234;
  int iStack_230;
  undefined1 local_22c;
  int local_228;
  int local_224;
  CGamePlayerInterface *local_220;
  CIDrawEnvironment *local_21c;
  CInputEventScanInfo local_218 [8];
  CGamePlayerInterface *local_210;
  CDefaultEngineMemoryInit local_20c [36];
  long lStack_1e8;
  CDefaultEngineMemoryInit local_1d8 [52];
  CDefaultEngineMemoryInit aCStack_1a4 [52];
  CGameEventPackage aCStack_170 [4];
  int iStack_16c;
  undefined1 local_164;
  undefined1 local_142;
  undefined1 local_13c;
  undefined1 local_11a;
  undefined1 local_114;
  undefined1 local_f2;
  undefined1 local_ec;
  undefined1 local_ca;
  CGameEventPackage local_c0 [12];
  undefined1 local_b4;
  undefined1 local_92;
  undefined1 local_8c;
  undefined1 local_6a;
  undefined1 local_64;
  undefined1 local_42;
  undefined1 local_3c;
  undefined1 local_1a;
  undefined4 local_c;
  
  puVar10 = (undefined4 *)&stack0xfffffffc;
  local_c = DAT_0139c8a8;
  local_22c = 0;
  local_228 = 0;
  local_224 = 0;
  local_220 = (CGamePlayerInterface *)0x0;
  local_210 = this;
  CDefaultEngineMemoryInit::CDefaultEngineMemoryInit(local_20c);
  CDefaultEngineMemoryInit::CDefaultEngineMemoryInit(local_1d8);
  local_164 = 0xff;
  local_142 = 0;
  local_13c = 0xff;
  local_11a = 0;
  local_114 = 0xff;
  local_f2 = 0;
  local_ec = 0xff;
  local_ca = 0;
  local_b4 = 0xff;
  local_92 = 0;
  local_8c = 0xff;
  local_6a = 0;
  local_64 = 0xff;
  local_42 = 0;
  local_3c = 0xff;
  local_1a = 0;
  CWADFile::Close((CWADFile *)local_1d8);
  CGameEventPackage::CGameEventPackage(local_c0);
  local_23a = false;
  if ((DAT_013b86a0 != (int *)0x0) && (cVar4 = (**(code **)(*DAT_013b86a0 + 0x24))(), cVar4 == '\0')
     ) {
    CDefaultEngineMemoryInit::CDefaultEngineMemoryInit(aCStack_1a4);
    local_23a = CInputManager::IsKeyboardEventInQueue
                          (DAT_013b8388,2,0x38,(CInputEvent *)aCStack_1a4);
    ::_E2();
  }
  local_21c = (CIDrawEnvironment *)0x0;
  bVar3 = false;
  local_218[0] = (CInputEventScanInfo)0x0;
  bVar5 = CInputManager::EventScanStart(DAT_013b8388,local_218,(CInputEvent *)local_20c);
  puVar9 = (undefined4 *)&stack0xfffffffc;
  if (bVar5) {
    do {
      if ((((local_23a == false) ||
           (pCVar6 = CWorld::DrawGetEnvironment((CWorld *)local_20c),
           pCVar6 != (CIDrawEnvironment *)0x2)) ||
          (EVar7 = CInputEvent::GetKey((CInputEvent *)local_20c), EVar7 != 0xf)) &&
         ((pCVar6 = CWorld::DrawGetEnvironment((CWorld *)local_20c),
          pCVar6 != (CIDrawEnvironment *)0x0 &&
          (bVar5 = CInputManager::IsEventProcessed(DAT_013b8388,(CInputEvent *)local_20c), !bVar5)))
         ) {
        local_228 = *(int *)(this + 0x10);
        local_220 = this + 4;
        if (local_228 != 0) {
          *(int ***)(local_228 + 0x14) = &piStack_238;
        }
        *(int ***)(this + 0x10) = &piStack_238;
        piStack_238 = *(int **)local_220;
        local_224 = 0;
        local_22c = 1;
        if (piStack_238 == (int *)0x0) {
          piStack_238 = (int *)0x0;
          piStack_234 = (int *)0x0;
          iStack_230 = 0;
LAB_004465ba:
          *(int *)(this + 0x10) = local_228;
          if (local_228 != 0) {
            *(undefined4 *)(local_228 + 0x14) = 0;
          }
        }
        else {
          piStack_234 = (int *)piStack_238[1];
          iStack_230 = 0;
          piVar11 = (int *)*piStack_238;
          puVar9 = puVar10;
          if (piVar11 == (int *)0x0) goto LAB_004465ba;
          do {
            CGameEventPackage::CGameEventPackage(aCStack_170);
            cVar4 = (**(code **)(*piVar11 + 0x20))(local_20c);
            puVar10 = puVar9;
            if (cVar4 != '\0') {
              CPlayerManager::GetPlayerNumberFromJoystickDeviceNumber
                        (*(CPlayerManager **)(this + 0x6fc),lStack_1e8);
              puVar10 = (undefined4 *)*puVar9;
              pcVar2 = *(code **)(*piVar11 + 0x1c);
              *puVar9 = 0x446560;
              cVar4 = (*pcVar2)();
              if ((cVar4 != '\0') || (*(char *)(DAT_013b86a0[9] + 0xa4) == '\0')) {
                (**(code **)(*piVar11 + 0x10))(local_20c,aCStack_170);
              }
            }
            if (iStack_16c == 1) {
              bVar3 = true;
              pCVar6 = CWorld::DrawGetEnvironment((CWorld *)aCStack_170);
              if ((int)local_21c < (int)pCVar6) {
                local_21c = CWorld::DrawGetEnvironment((CWorld *)aCStack_170);
                pCVar12 = aCStack_170;
                pCVar14 = local_c0;
                for (iVar8 = 0x2c; iVar8 != 0; iVar8 = iVar8 + -1) {
                  *(undefined4 *)pCVar14 = *(undefined4 *)pCVar12;
                  pCVar12 = pCVar12 + 4;
                  pCVar14 = pCVar14 + 4;
                }
                pCVar13 = local_20c;
                pCVar15 = local_1d8;
                for (iVar8 = 0xd; this = local_210, iVar8 != 0; iVar8 = iVar8 + -1) {
                  *(undefined4 *)pCVar15 = *(undefined4 *)pCVar13;
                  pCVar13 = pCVar13 + 4;
                  pCVar15 = pCVar15 + 4;
                }
              }
            }
            else if (iStack_16c == 2) {
              CInputEventQueue_Insert(local_20c);
              break;
            }
            piStack_238 = piStack_234;
            if (piStack_234 == (int *)0x0) break;
            piVar1 = piStack_234 + 1;
            iStack_230 = piStack_234[2];
            piVar11 = (int *)*piStack_234;
            puVar9 = puVar10;
            piStack_234 = (int *)*piVar1;
          } while (piVar11 != (int *)0x0);
          if (local_224 == 0) goto LAB_004465ba;
          *(int *)(local_224 + 0x10) = local_228;
          if (local_228 != 0) {
            *(int *)(local_228 + 0x14) = local_224;
          }
        }
        local_22c = 0;
        local_228 = 0;
        local_224 = 0;
        local_220 = (CGamePlayerInterface *)0x0;
      }
      bVar5 = CInputManager::EventScanNext(DAT_013b8388,local_218,(CInputEvent *)local_20c);
      puVar9 = puVar10;
    } while (bVar5);
  }
  CInputManager::EventScanEnd(DAT_013b8388,local_218);
  if (!bVar3) {
    ::_E2();
    ::_E2();
    return false;
  }
  pCVar12 = local_c0;
  puVar10 = (undefined4 *)puVar9[2];
  for (iVar8 = 0x2c; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar10 = *(undefined4 *)pCVar12;
    pCVar12 = pCVar12 + 4;
    puVar10 = puVar10 + 1;
  }
  pCVar6 = CWorld::DrawGetEnvironment((CWorld *)local_1d8);
  if (pCVar6 != (CIDrawEnvironment *)0x0) {
    CInputEventQueue_Insert(local_1d8);
  }
  ::_E2();
  ::_E2();
  return true;
}


// CALLEES of GetInputProcessEvent:
//   009f50a0  IsKeyboardEventInQueue
//   009f55c0  CInputEventQueue_Insert
//   00a0d3b0  DrawGetEnvironment
//   009f4f10  EventScanNext
//   00bfe9f9  __security_check_cookie
//   009f4ed0  EventScanStart
//   00a03be0  Close
//   00a03b50  DrawGetEnvironment
//   00a03b70  GetKey
//   00a03b30  $E2
//   00a0d3a0  __EH_epilog3
//   00449990  GetPlayerNumberFromJoystickDeviceNumber
//   00a04410  CDefaultEngineMemoryInit
//   009f4b10  IsEventProcessed
//   00a03b40  DrawGetEnvironment
//   00a0d300  CGameEventPackage
//   009f4ac0  EventScanEnd

//=== ?IsInputEventToBeSentToThisInputProcess@CAInputProcess@@UBE_NABVCInputEvent@@@Z @ 00a0d400 (seed 00a0d400) ===

undefined1 _IsInputEventToBeSentToThisInputProcess_CAInputProcess__UBE_NABVCInputEvent___Z(void)

{
  return 1;
}


// CALLEES of ?IsInputEventToBeSentToThisInputProcess@CAInputProcess@@UBE_NABVCInputEvent@@@Z:

//=== UISystem_ProcessInputEventLoop @ 00c0ec90 (seed 00c0ec90) ===

/* [forge-autoname4 conf=medium] Fastcall polls param_1+0x34 for manager count, calls GetKeyPressed
   on each, clears queue when key pressed; input-polling loop confirmed. */

void UISystem_ProcessInputEventLoop(void)

{
  CManager *this;
  wchar_t wVar1;
  int in_ECX;
  CManager *pCVar2;
  uint uVar3;
  
  if (*(int *)(in_ECX + 0x48) == 0) {
    return;
  }
LAB_00c0eca0:
  do {
    pCVar2 = (CManager *)0x0;
    uVar3 = 0;
    do {
      if (*(uint *)(in_ECX + 0x34) <= uVar3) {
        if (pCVar2 == (CManager *)0x0) {
          return;
        }
        CTCMapwho_ClearMessageQueue(pCVar2);
        goto LAB_00c0eca0;
      }
      this = *(CManager **)(*(int *)(in_ECX + 0x2c) + uVar3 * 4);
      wVar1 = NUISystem::CManager::GetKeyPressed(this);
      if ((char)wVar1 != '\0') {
        pCVar2 = this;
      }
      uVar3 = uVar3 + 1;
    } while (pCVar2 == (CManager *)0x0);
    CTCMapwho_ClearMessageQueue(pCVar2);
  } while( true );
}


// CALLEES of UISystem_ProcessInputEventLoop:
//   00c1ea20  GetKeyPressed
//   00c0ec10  CTCMapwho_ClearMessageQueue

