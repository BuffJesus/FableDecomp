//=== Input @ 0042e3ee (seed 0042e3ee) ===

/* [bsim sim=0.9346371375725461 <- ego_r]
   private: void __thiscall CNewFrontendGameComponent::Input(void) */

void __thiscall CNewFrontendGameComponent::Input(CNewFrontendGameComponent *this)

{
  bool bVar1;
  bool bVar2;
  wchar_t wVar3;
  CIDrawEnvironment *pCVar4;
  CCodeSectionManager *this_00;
  EInputKey EVar5;
  int iVar6;
  CGameJoystickManager *pCVar7;
  CIDrawEnvironment *pCVar8;
  undefined3 extraout_var_00;
  CFrontEndManager *pCVar9;
  uint uVar10;
  float unaff_ESI;
  float fVar11;
  CDefaultEngineMemoryInit *pCVar12;
  undefined4 uVar13;
  CDefaultEngineMemoryInit local_88 [52];
  CDefaultEngineMemoryInit local_54 [12];
  undefined4 local_48;
  undefined4 local_44;
  CInputEventScanInfo local_20 [8];
  undefined4 local_18;
  undefined4 local_14;
  float local_10;
  float local_c;
  uint local_8;
  undefined3 extraout_var;
  
  if (this[0x138] != (CNewFrontendGameComponent)0x0) {
    CInputManager::ClearEvents(DAT_013b8388);
    return;
  }
  local_20[0] = (CInputEventScanInfo)0x0;
  CDefaultEngineMemoryInit::CDefaultEngineMemoryInit(local_54);
  CDefaultEngineMemoryInit::CDefaultEngineMemoryInit(local_88);
  (**(code **)(*(int *)DAT_013b8388 + 8))();
  local_8 = 0;
  bVar1 = CInputManager::EventScanStart(DAT_013b8388,local_20,(CInputEvent *)local_54);
  while (bVar1 != false) {
    pCVar4 = CWorld::DrawGetEnvironment((CWorld *)local_54);
    if ((int)pCVar4 < 0x12) {
      if (pCVar4 == (CIDrawEnvironment *)0x11) {
        if (this[0x28] != (CNewFrontendGameComponent)0x0) {
          CPersistContext::GetDefaultVal<CEngineGraphic>();
          if (local_c <= 0.25) {
            if (local_c < -0.25) {
              local_8 = local_8 | 1;
            }
          }
          else {
            local_8 = local_8 | 2;
          }
          if (local_10 <= 0.25) {
            if (local_10 < -0.25) {
              local_8 = local_8 | 4;
            }
          }
          else {
            local_8 = local_8 | 8;
          }
        }
      }
      else {
        if ((int)pCVar4 < 0xb) {
          if (pCVar4 == (CIDrawEnvironment *)0xa) {
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x1b;
          }
          else if (pCVar4 == (CIDrawEnvironment *)0x1) {
            pCVar12 = local_54;
            this_00 = CTBaseSingleton<CCodeSectionManager>::Get();
            CThingFilter_IsUnconscious::operator()
                      ((CThingFilter_IsUnconscious *)this_00,(CThing *)pCVar12);
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x6f) {
              local_8 = local_8 | 4;
            }
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x70) {
              local_8 = local_8 | 8;
            }
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x6d) {
              local_8 = local_8 | 1;
            }
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x72) {
              local_8 = local_8 | 2;
            }
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x1e) {
              local_8 = local_8 | 0x100;
            }
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x30) {
              local_8 = local_8 | 0x200;
            }
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            if (EVar5 == 0x15) {
              local_8 = local_8 | 0x20000;
            }
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            iVar6 = *(int *)pCVar9;
            EVar5 = CInputEvent::GetKey((CInputEvent *)local_54);
            (**(code **)(iVar6 + 0x30))(EVar5);
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x21;
          }
          else if (pCVar4 == (CIDrawEnvironment *)&DAT_00000004) {
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x1a;
          }
          else if (pCVar4 == (CIDrawEnvironment *)&DAT_00000006) {
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x1c;
          }
          else {
            if (pCVar4 != (CIDrawEnvironment *)&DAT_00000007) goto LAB_0042e7f0;
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x23;
          }
        }
        else if (pCVar4 == (CIDrawEnvironment *)0xd) {
          local_18 = local_48;
          local_14 = local_44;
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          *(undefined4 *)(pCVar9 + 0xb0) = local_18;
          *(undefined4 *)(pCVar9 + 0xb4) = local_14;
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x19;
        }
        else if (pCVar4 == (CIDrawEnvironment *)0xe) {
          fVar11 = CMap::DrawGetSeaLevel((CMap *)local_54,unaff_ESI);
          if (fVar11 <= 0.0001) {
            fVar11 = CMap::DrawGetSeaLevel((CMap *)local_54,unaff_ESI);
            if (-0.0001 <= fVar11) goto LAB_0042e7f0;
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x25;
          }
          else {
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x24;
          }
        }
        else {
          if (pCVar4 != (CIDrawEnvironment *)&DAT_0000000f) goto LAB_0042e7f0;
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          iVar6 = *(int *)pCVar9;
          wVar3 = CInputEvent::GetChar((CInputEvent *)local_54);
          (**(code **)(iVar6 + 0x30))(wVar3);
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x22;
        }
LAB_0042e5ab:
        (*(code *)**(undefined4 **)pCVar9)(uVar13);
      }
    }
    else if (pCVar4 == (CIDrawEnvironment *)&DAT_00000013) {
      bVar1 = true;
      pCVar7 = CGameJoystickManager::Get();
      bVar2 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned((CTCInventoryBase *)pCVar7);
      if (bVar2) {
        pCVar7 = CGameJoystickManager::Get();
        pCVar4 = CWorld::DrawGetEnvironment((CWorld *)pCVar7);
        pCVar8 = CWorld::DrawGetEnvironment((CWorld *)local_54);
        bVar1 = pCVar4 == pCVar8;
      }
      if ((this[0x28] != (CNewFrontendGameComponent)0x0) && (bVar1)) {
        pCVar4 = CWorld::DrawGetEnvironment((CWorld *)local_54);
        *(CIDrawEnvironment **)(this + 0x140) = pCVar4;
        bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned
                          ((CTCInventoryBase *)local_54);
        iVar6 = CONCAT31(extraout_var_00,bVar1);
        if (iVar6 < 0xd) {
          if (iVar6 == 0xc) {
            local_8 = local_8 | 0x800;
          }
          else if (iVar6 == 1) {
            local_8 = local_8 | 0x10000;
          }
          else if (iVar6 == 2) {
            local_8 = local_8 | 0x20000;
          }
          else if (iVar6 == 4) {
            local_8 = local_8 | 0x100;
          }
          else if (iVar6 == 5) {
            local_8 = local_8 | 0x200;
          }
          else if (iVar6 == 0xb) {
            local_8 = local_8 | 0x400;
          }
        }
        else if (iVar6 == 0xd) {
          *(uint *)(this + 0xfc) = *(uint *)(this + 0xfc) | 0x10;
        }
        else if (iVar6 == 0xe) {
          *(uint *)(this + 0xfc) = *(uint *)(this + 0xfc) | 0x20;
        }
        else if (iVar6 == 0xf) {
          *(uint *)(this + 0xfc) = *(uint *)(this + 0xfc) | 0x40;
        }
        else if (iVar6 == 0x10) {
          this[0xfc] = (CNewFrontendGameComponent)((byte)this[0xfc] | 0x80);
        }
      }
    }
    else {
      if (pCVar4 != (CIDrawEnvironment *)0x15) {
        if (pCVar4 == (CIDrawEnvironment *)&DAT_00000016) {
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x26;
        }
        else if (pCVar4 == (CIDrawEnvironment *)0x17) {
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x27;
        }
        else if (pCVar4 == (CIDrawEnvironment *)0x18) {
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x28;
        }
        else if (pCVar4 == (CIDrawEnvironment *)0x19) {
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x29;
        }
        else {
          if (pCVar4 != (CIDrawEnvironment *)&DAT_0000001a) goto LAB_0042e7f0;
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x2a;
        }
        goto LAB_0042e5ab;
      }
      if (this[0x28] != (CNewFrontendGameComponent)0x0) {
        bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned
                          ((CTCInventoryBase *)local_54);
        iVar6 = CONCAT31(extraout_var,bVar1);
        if (iVar6 == 0xd) {
          *(uint *)(this + 0xfc) = *(uint *)(this + 0xfc) & 0xffffffef;
        }
        else if (iVar6 == 0xe) {
          *(uint *)(this + 0xfc) = *(uint *)(this + 0xfc) & 0xffffffdf;
        }
        else if (iVar6 == 0xf) {
          *(uint *)(this + 0xfc) = *(uint *)(this + 0xfc) & 0xffffffbf;
        }
        else if (iVar6 == 0x10) {
          this[0xfc] = (CNewFrontendGameComponent)((byte)this[0xfc] & 0x7f);
        }
      }
    }
LAB_0042e7f0:
    bVar1 = CInputManager::EventScanNext(DAT_013b8388,local_20,(CInputEvent *)local_54);
  }
  CInputManager::EventScanEnd(DAT_013b8388,local_20);
  uVar10 = local_8;
  if (*(uint *)(this + 0xfc) != 0) {
    uVar10 = local_8 | *(uint *)(this + 0xfc);
  }
  if (uVar10 == 0) {
    *(undefined4 *)(this + 0xf8) = 0x3eaa7efa;
    CStopWatch::Stop((CStopWatch *)(this + 0xd8));
  }
  else {
    CStopWatch::Reset((CStopWatch *)(this + 0x100));
    if (this[0xf0] == (CNewFrontendGameComponent)0x0) {
      *(undefined4 *)(this + 0xf8) = 0x3eaa7efa;
    }
    else {
      fVar11 = CStopWatch::GetElapsedSeconds((CStopWatch *)(this + 0xd8));
      if (fVar11 < *(float *)(this + 0xf8)) goto LAB_0042e977;
      *(undefined4 *)(this + 0xf8) = 0x3dae147b;
    }
    CStopWatch::StartZero((CStopWatch *)(this + 0xd8));
  }
  if ((uVar10 & 0x400) == 0) {
    if ((uVar10 & 0x800) == 0) {
      if ((uVar10 & 0x100) != 0) goto LAB_0042e8a8;
      if ((uVar10 & 0x200) != 0) goto LAB_0042e8b9;
      if ((uVar10 & 0x20000) == 0) {
        if ((uVar10 & 0x44) == 0) {
          if ((uVar10 & 0x88) == 0) {
            if ((uVar10 & 0x11) == 0) {
              if ((uVar10 & 0x22) == 0) {
                if ((uVar10 & 0x1000) == 0) {
                  if ((uVar10 & 0x2000) == 0) {
                    if ((uVar10 & 0x4000) == 0) {
                      if ((char)(uVar10 >> 8) < '\0') {
                        pCVar9 = NUISystem::CFrontEndManager::GetInstance();
                        uVar13 = 0xb;
                      }
                      else {
                        if ((uVar10 & 0x10000) == 0) goto LAB_0042e977;
                        pCVar9 = NUISystem::CFrontEndManager::GetInstance();
                        uVar13 = 0x17;
                      }
                    }
                    else {
                      pCVar9 = NUISystem::CFrontEndManager::GetInstance();
                      uVar13 = 10;
                    }
                  }
                  else {
                    pCVar9 = NUISystem::CFrontEndManager::GetInstance();
                    uVar13 = 9;
                  }
                }
                else {
                  pCVar9 = NUISystem::CFrontEndManager::GetInstance();
                  uVar13 = 8;
                }
              }
              else {
                pCVar9 = NUISystem::CFrontEndManager::GetInstance();
                uVar13 = 1;
              }
            }
            else {
              pCVar9 = NUISystem::CFrontEndManager::GetInstance();
              uVar13 = 0;
            }
          }
          else {
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            (*(code *)**(undefined4 **)pCVar9)(3);
            pCVar9 = NUISystem::CFrontEndManager::GetInstance();
            uVar13 = 0x15;
          }
        }
        else {
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          (*(code *)**(undefined4 **)pCVar9)(2);
          pCVar9 = NUISystem::CFrontEndManager::GetInstance();
          uVar13 = 0x14;
        }
      }
      else {
        pCVar9 = NUISystem::CFrontEndManager::GetInstance();
        uVar13 = 0x16;
      }
    }
    else {
LAB_0042e8b9:
      pCVar9 = NUISystem::CFrontEndManager::GetInstance();
      uVar13 = 5;
    }
  }
  else {
LAB_0042e8a8:
    pCVar9 = NUISystem::CFrontEndManager::GetInstance();
    uVar13 = 4;
  }
  (*(code *)**(undefined4 **)pCVar9)(uVar13);
LAB_0042e977:
  ::_E2();
  ::_E2();
  return;
}


// CALLEES of Input:
//   00a03b60  DrawGetEnvironment
//   0062f8b0  Reset
//   00a03bb0  DrawGetSeaLevel
//   0049bb50  GetCanItemBeQuickAssigned
//   0049bb40  DrawGetEnvironment
//   00a04410  CDefaultEngineMemoryInit
//   009f4ed0  EventScanStart
//   0062f800  StartZero
//   009f5540  ClearEvents
//   009ea530  operator()
//   00a03b70  GetKey
//   009f4ac0  EventScanEnd
//   0062f850  Stop
//   00a03b30  $E2
//   0062f900  GetElapsedSeconds
//   00a03b90  GetCanItemBeQuickAssigned
//   0049bb90  Get
//   00a043a0  GetDefaultVal<CEngineGraphic>
//   00a03b40  DrawGetEnvironment
//   009f4f10  EventScanNext
//   00414c90  Get
//   00a03b80  GetChar
//   0041e5f2  GetInstance

