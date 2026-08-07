//=== Catch@0047e8cc @ 00442d50 (seed 00442d70) ===

/* [bsim sim=0.5579388566011471 <- ego_r] */

void Catch_0047e8cc(byte param_1)

{
  undefined4 *puVar1;
  void *in_ECX;
  undefined4 *puVar2;
  list<CGuiControlTreeNode,std::allocator<CGuiControlTreeNode>_> *unaff_EDI;
  
  CGuiControlTreePane::SortTreeRecursively((CGuiControlTreePane *)((int)in_ECX + 0x28),unaff_EDI);
  puVar1 = *(undefined4 **)((int)in_ECX + 0x20);
  for (puVar2 = *(undefined4 **)((int)in_ECX + 0x1c); puVar2 != puVar1; puVar2 = puVar2 + 2) {
    (**(code **)*puVar2)(0);
  }
  if (*(void **)((int)in_ECX + 0x1c) != (void *)0x0) {
    free(*(void **)((int)in_ECX + 0x1c));
  }
  if ((param_1 & 1) != 0) {
    operator_delete(in_ECX);
  }
  return;
}


// CALLEES of Catch@0047e8cc:
//   0043fcf0  SortTreeRecursively
//   00bfea14  free
//   00bfe9bc  operator_delete

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
//   00a04410  CDefaultEngineMemoryInit
//   009f55c0  CInputEventQueue_Insert
//   00bfe9f9  __security_check_cookie
//   00a03b30  $E2
//   00a03be0  Close
//   009f4ed0  EventScanStart
//   009f4b10  IsEventProcessed
//   00449990  GetPlayerNumberFromJoystickDeviceNumber
//   009f4f10  EventScanNext
//   009f4ac0  EventScanEnd
//   00a0d3b0  DrawGetEnvironment
//   00a0d300  CGameEventPackage
//   00a03b50  DrawGetEnvironment
//   00a03b40  DrawGetEnvironment
//   00a0d3a0  __EH_epilog3
//   00a03b70  GetKey
//   009f50a0  IsKeyboardEventInQueue

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
//   00447c80  LowerBound
//   00447ce0  UpperBound

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
//   00447c80  LowerBound
//   00447ce0  UpperBound

