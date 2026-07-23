//=== GetDataFromDef @ 0041ac20 ===

/* [bsim sim=0.7733961436093034 <- ego_r]
   public: void __thiscall NUISystem::CSprite::GetDataFromDef(void) */

void __thiscall NUISystem::CSprite::GetDataFromDef(CSprite *this)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  CResource *pCVar5;
  float fVar6;
  bool bVar7;
  undefined4 uVar8;
  int *piVar9;
  CFrontEndManager *pCVar10;
  int iVar11;
  CFrameData *pCVar12;
  int *unaff_EBX;
  CGraphicInfoFrame *this_00;
  CGraphicFrameData *this_01;
  int *local_8 [2];
  
  (**(code **)(*(int *)this + 0x1b0))(local_8);
  *(int *)(this + 0x164) = unaff_EBX[0x2e];
  uVar8 = __ftol2();
  *(undefined4 *)(this + 0x168) = uVar8;
  uVar8 = __ftol2();
  *(undefined4 *)(this + 0x16c) = uVar8;
  *(int *)(this + 0x174) = unaff_EBX[0x89];
  iVar11 = unaff_EBX[0x11] - unaff_EBX[0x10] >> 0x1f;
  if ((unaff_EBX[0x11] - unaff_EBX[0x10]) / 0x7c + iVar11 == iVar11) {
    *(undefined4 *)(this + 0x178) = 0;
  }
  else {
    piVar9 = (int *)(**(code **)(*(int *)this + 0x1b0))(local_8);
    uVar8 = *(undefined4 *)(*(int *)(*piVar9 + 0x40) + 0x3c);
    if ((local_8[0] != (int *)0x0) &&
       (iVar11 = local_8[0][1], local_8[0][1] = iVar11 + -1, iVar11 + -1 == 0)) {
      (**(code **)(*local_8[0] + 4))();
    }
    *(undefined4 *)(this + 0x178) = uVar8;
  }
  if (*(int *)(this + 0x178) == 0) goto LAB_0041af6f;
  pCVar10 = CFrontEndManager::GetInstance();
  piVar9 = *(int **)(pCVar10 + 0x14);
  piVar2 = *(int **)(pCVar10 + 0x10);
  if (piVar9 != (int *)0x0) {
    *piVar9 = *piVar9 + 1;
  }
  iVar11 = (**(code **)(*piVar2 + 0x54))(*(undefined4 *)(this + 0x178),0);
  fVar6 = (float)iVar11;
  if (iVar11 < 0) {
    fVar6 = fVar6 + 4.2949673e+09;
  }
  *(float *)(this + 0xcc) = fVar6;
  if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
    (*(code *)piVar9[1])();
    operator_delete(piVar9);
  }
  pCVar10 = CFrontEndManager::GetInstance();
  piVar9 = *(int **)(pCVar10 + 0x14);
  piVar2 = *(int **)(pCVar10 + 0x10);
  if (piVar9 != (int *)0x0) {
    *piVar9 = *piVar9 + 1;
  }
  local_8[0] = (int *)(**(code **)(*piVar2 + 0x58))(*(undefined4 *)(this + 0x178),0);
  fVar6 = (float)(int)local_8[0];
  if ((int)local_8[0] < 0) {
    fVar6 = fVar6 + 4.2949673e+09;
  }
  *(float *)(this + 0xd0) = fVar6;
  if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
    (*(code *)piVar9[1])();
    operator_delete(piVar9);
  }
  pCVar10 = CFrontEndManager::GetInstance();
  piVar9 = *(int **)(pCVar10 + 0x14);
  iVar11 = *(int *)(pCVar10 + 0x10);
  if (piVar9 != (int *)0x0) {
    *piVar9 = *piVar9 + 1;
  }
  iVar3 = *(int *)(iVar11 + 0x1e0);
  uVar4 = *(uint *)(this + 0x178);
  if ((uVar4 < (uint)((*(int *)(iVar11 + 0x1e4) - iVar3) / 0x2c)) &&
     (uVar1 = uVar4 * 0x2c + iVar3, uVar1 != 0)) {
    this_00 = *(CGraphicInfoFrame **)
               ((-(uint)(uVar4 < (uint)((*(int *)(iVar11 + 0x1e4) - iVar3) / 0x2c)) & uVar1) + 0x28)
    ;
  }
  else {
    this_00 = (CGraphicInfoFrame *)0x0;
  }
  if ((piVar9 != (int *)0x0) && (*piVar9 = *piVar9 + -1, *piVar9 == 0)) {
    (*(code *)piVar9[1])();
    operator_delete(piVar9);
  }
  CGraphicInfoFrame::BeginLoading(this_00,0,0x10);
  pCVar12 = *(CFrameData **)(this_00 + 8);
  if (pCVar12 == (CFrameData *)0x0) {
    pCVar12 = ::operator_new(0x44);
    if (pCVar12 == (CFrameData *)0x0) {
      pCVar12 = (CFrameData *)0x0;
    }
    else {
      pCVar12 = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar12,this_00);
    }
    piVar9 = *(int **)(this_00 + 8);
    if (piVar9 != (int *)0x0) {
      piVar2 = piVar9 + 1;
      *piVar2 = *piVar2 + -1;
      if (*piVar2 == 0) {
        (**(code **)(*piVar9 + 4))();
      }
      *(undefined4 *)(this_00 + 8) = 0;
    }
    *(CFrameData **)(this_00 + 8) = pCVar12;
LAB_0041af04:
    CGraphicInfoFrame::CFrameData::BeginLoading(pCVar12,0,1);
    bVar7 = CGraphicInfoFrame::InternalGetFrameForcedLoading(this_00,1);
    if (!bVar7) goto LAB_0041af6f;
LAB_0041af19:
    this_01 = *(CGraphicFrameData **)(*(int *)(this_00 + 8) + 0x1c);
  }
  else {
    if (*(int *)(pCVar12 + 0x1c) == 0) goto LAB_0041af04;
    *(undefined4 *)(pCVar12 + 0x3c) =
         *(undefined4 *)(*(int *)(*(int *)(this_00 + 0xc) + 0x20) + 0x194);
    pCVar5 = *(CResource **)(*(int *)(this_00 + 8) + 0x24);
    if (((pCVar5 != (CResource *)0x0) && (*(int *)(pCVar5 + 8) != 0)) &&
       (*(int *)(pCVar5 + 0x18) != *(int *)(*(int *)(pCVar5 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar5);
      CResource::ListInsert
                (pCVar5,*(CResource **)(*(int *)(pCVar5 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar5 + 8) + 4));
      *(undefined4 *)(pCVar5 + 0x18) = *(undefined4 *)(*(int *)(pCVar5 + 8) + 0x2c);
    }
    pCVar5 = *(CResource **)(this_00 + 8);
    if ((*(int *)(pCVar5 + 8) != 0) &&
       (*(int *)(pCVar5 + 0x18) != *(int *)(*(int *)(pCVar5 + 8) + 0x2c))) {
      CResource::ListRemove(pCVar5);
      CResource::ListInsert
                (pCVar5,*(CResource **)(*(int *)(pCVar5 + 8) + 0x10),
                 (CResource *)(*(int *)(pCVar5 + 8) + 4));
      *(undefined4 *)(pCVar5 + 0x18) = *(undefined4 *)(*(int *)(pCVar5 + 8) + 0x2c);
    }
    if (*(char *)(*(int *)(this_00 + 8) + 0x3a) != '\0') {
      *(undefined1 *)(*(int *)(this_00 + 8) + 0x3a) = 0;
    }
    pCVar12 = *(CFrameData **)(this_00 + 8);
    if (pCVar12[0x38] != (CFrameData)0x0) {
      CGraphicInfoFrame::CFrameData::BeginLoading(pCVar12,0,1);
      goto LAB_0041af19;
    }
    this_01 = *(CGraphicFrameData **)(pCVar12 + 0x1c);
  }
  if (this_01 != (CGraphicFrameData *)0x0) {
    iVar11 = *(int *)this_01;
    *(int *)this_01 = iVar11 + 1;
    *(int *)this_01 = iVar11;
    if (iVar11 == 0) {
      if (((byte)this_01[0x1a] & 0x10) != 0) {
        CGraphicFrameData::Uninitialise(this_01);
      }
      piVar9 = *(int **)(this_01 + 0xc);
      if (piVar9 != (int *)0x0) {
        *piVar9 = *piVar9 + -1;
        if (**(int **)(this_01 + 0xc) == 0) {
          (*(code *)(*(int **)(this_01 + 0xc))[1])();
          operator_delete(*(void **)(this_01 + 0xc));
        }
      }
      *(undefined4 *)(this_01 + 8) = 0;
      *(undefined4 *)(this_01 + 0xc) = 0;
      operator_delete(this_01);
    }
  }
LAB_0041af6f:
  if ((unaff_EBX != (int *)0x0) &&
     (iVar11 = unaff_EBX[1], unaff_EBX[1] = iVar11 + -1, iVar11 + -1 == 0)) {
    (**(code **)(*unaff_EBX + 4))();
  }
  return;
}



//=== ?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ @ 004aa900 ===

/* [bsim sim=0.9999999999999998 <- ego_r]
   public: virtual class CCharString __thiscall CScriptThing::GetCurrentMapName(void)const  */

CCharString * __thiscall
CScriptThing::_GetDataString_CScriptThing__UBE_AVCCharString__XZ(CScriptThing *this)

{
  CCharString *in_stack_00000004;
  
  if (*(int **)(this + 4) == (int *)0x0) {
    CCharString::CCharString(in_stack_00000004,(CCharString *)&DAT_0143e8ec);
    return in_stack_00000004;
  }
  (**(code **)(**(int **)(this + 4) + 0xc))();
  return in_stack_00000004;
}



//=== ?GetDataFormatVersion@CWADBank@@MAEKXZ @ 0050b450 ===

undefined4 _GetDataFormatVersion_CWADBank__MAEKXZ(void)

{
  return 2;
}



//=== GetDataFromDef @ 005331a0 ===

/* [bsim sim=0.7028486057536425 <- ego_r]
   public: void __thiscall NUISystem::CComponent::GetDataFromDef(void) */

void __thiscall NUISystem::CComponent::GetDataFromDef(CComponent *this)

{
  CRumbleDef *this_00;
  CAction *pCVar1;
  CFrontEndManager *this_01;
  int *unaff_EBX;
  CComponent *pCVar2;
  int iVar3;
  uint uVar4;
  long lVar5;
  CComponent *local_5c;
  CComponent *local_58;
  int *piStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined1 uStack_38;
  undefined1 uStack_37;
  undefined1 uStack_36;
  undefined1 uStack_35;
  undefined4 uStack_34;
  undefined1 uStack_30;
  uint uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined1 uStack_10;
  undefined1 uStack_f;
  undefined1 uStack_e;
  undefined1 uStack_d;
  undefined4 uStack_c;
  undefined1 uStack_8;
  
  local_58 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_5c);
  uVar4 = 0;
  if (unaff_EBX[0x35] != 0) {
    this_00 = ::operator_new(0x10);
    if (this_00 == (CRumbleDef *)0x0) {
      pCVar1 = (CAction *)0x0;
    }
    else {
      pCVar1 = (CAction *)CRumbleDef::CRumbleDef(this_00);
    }
    CCountedPointer<NUISystem::CAction>::CCountedPointer<NUISystem::CAction>
              ((CCountedPointer<NUISystem::CAction> *)&local_58,pCVar1);
    *(int *)local_58 = unaff_EBX[0x35];
    (**(code **)(*(int *)this + 0x208))(&local_58);
    if ((piStack_54 != (int *)0x0) && (*piStack_54 = *piStack_54 + -1, *piStack_54 == 0)) {
      (*(code *)piStack_54[1])();
      operator_delete(piStack_54);
    }
  }
  this[300] = (CComponent)(*(byte *)(unaff_EBX + 0xf) & 0x1f);
  this[0x12d] = *(CComponent *)(unaff_EBX + 0xf);
  if ((char)unaff_EBX[0x7e] != '\0') {
    this[300] = (CComponent)((byte)this[300] | 0x80);
  }
  if (*(char *)((int)unaff_EBX + 0xbf) != '\0') {
    this[300] = (CComponent)((byte)this[300] | 0x40);
  }
  if ((char)unaff_EBX[0x77] != '\0') {
    this[300] = (CComponent)((byte)this[300] | 0x20);
  }
  if ((char)unaff_EBX[0x62] != '\0') {
    this[0x12e] = (CComponent)((byte)this[0x12e] | 1);
  }
  if ((char)unaff_EBX[0x2f] != '\0') {
    this[0x12e] = (CComponent)((byte)this[0x12e] | 2);
  }
  if ((char)unaff_EBX[0x82] != '\0') {
    this[0x12e] = (CComponent)((byte)this[0x12e] | 0x40);
  }
  if (*(char *)((int)unaff_EBX + 0x209) != '\0') {
    this[0x12e] = (CComponent)((byte)this[0x12e] | 0x80);
  }
  this[0x12f] = *(CComponent *)(unaff_EBX + 0x2d);
  iVar3 = unaff_EBX[0x11] - unaff_EBX[0x10] >> 0x1f;
  if ((unaff_EBX[0x11] - unaff_EBX[0x10]) / 0x7c + iVar3 != iVar3) {
    pCVar2 = this + 0x24;
    iVar3 = 0;
    do {
      CUIState::CUIState((CUIState *)&uStack_50,(CUIStateDef *)(unaff_EBX[0x10] + iVar3));
      uStack_20 = uStack_48;
      uStack_14 = uStack_3c;
      uStack_28 = uStack_50;
      uStack_24 = uStack_4c;
      uStack_10 = uStack_38;
      uStack_1c = uStack_44;
      uStack_18 = uStack_40;
      uStack_8 = uStack_30;
      uStack_e = uStack_36;
      uStack_f = uStack_37;
      uStack_d = uStack_35;
      uStack_c = uStack_34;
      uStack_2c = uVar4;
      FUN_0042d2c6(&local_58,*(undefined4 *)(*(int *)pCVar2 + 8),&uStack_2c);
      uVar4 = uVar4 + 1;
      iVar3 = iVar3 + 0x7c;
      this = local_5c;
    } while (uVar4 < (uint)((unaff_EBX[0x11] - unaff_EBX[0x10]) / 0x7c));
  }
  uVar4 = 0;
  if (unaff_EBX[0x1d] - unaff_EBX[0x1c] >> 2 != 0) {
    do {
      lVar5 = *(long *)(unaff_EBX[0x1c] + uVar4 * 4);
      this_01 = CFrontEndManager::GetInstance();
      local_58 = CManager::CreateComponent((CManager *)this_01,lVar5);
      if ((local_58 == (CComponent *)0x0) ||
         (piStack_54 = ::operator_new(0xc), piStack_54 == (int *)0x0)) {
        piStack_54 = (int *)0x0;
      }
      else {
        *piStack_54 = 1;
        piStack_54[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
        piStack_54[2] = (int)local_58;
      }
      (**(code **)(*(int *)this + 0xec))(&local_58);
      if ((piStack_54 != (int *)0x0) && (*piStack_54 = *piStack_54 + -1, *piStack_54 == 0)) {
        (*(code *)piStack_54[1])();
        operator_delete(piStack_54);
      }
      uVar4 = uVar4 + 1;
      local_58 = (CComponent *)0x0;
      piStack_54 = (int *)0x0;
    } while (uVar4 < (uint)(unaff_EBX[0x1d] - unaff_EBX[0x1c] >> 2));
  }
  FUN_0045bc09(unaff_EBX + 0x85);
  if ((unaff_EBX != (int *)0x0) &&
     (iVar3 = unaff_EBX[1], unaff_EBX[1] = iVar3 + -1, iVar3 + -1 == 0)) {
    (**(code **)(*unaff_EBX + 4))();
  }
  return;
}



//=== GetDataFromDef @ 0053822b ===

/* [bsim sim=0.8088297520402257 <- ego_r]
   public: void __thiscall NUISystem::CList::GetDataFromDef(void) */

void __thiscall NUISystem::CList::GetDataFromDef(CList *this)

{
  undefined4 uVar1;
  EWeaponClass *pEVar2;
  float fVar3;
  EWeaponClass EVar4;
  EWeaponClass *pEVar5;
  int *piVar6;
  EWeaponClass *unaff_EBX;
  int **ppiVar7;
  EWeaponClass local_10;
  int *local_c;
  undefined1 local_5;
  
  ppiVar7 = &local_c;
  (**(code **)(*(int *)this + 0x1b0))();
  if ((char)local_c[0x50] != '\0') {
    this[0x12e] = (CList)((byte)this[0x12e] | 8);
  }
  if (*(char *)((int)local_c + 0x14b) != '\0') {
    this[0x12e] = (CList)((byte)this[0x12e] | 0x10);
  }
  if (*(char *)((int)local_c + 0x141) != '\0') {
    this[0x12e] = (CList)((byte)this[0x12e] | 0x20);
  }
  *(int *)(this + 0x1ec) = local_c[0x61];
  *(undefined4 *)(this + 0x1f0) = *(undefined4 *)((int)local_c + 0x142);
  *(undefined4 *)(this + 500) = *(undefined4 *)((int)local_c + 0x146);
  this[0x1f8] = *(CList *)((int)local_c + 0x14a);
  *(undefined4 *)(this + 400) = *(undefined4 *)((int)local_c + 0x142);
  *(undefined4 *)(this + 0x194) = *(undefined4 *)((int)local_c + 0x146);
  *(undefined4 *)(this + 0x188) = *(undefined4 *)((int)local_c + 0x142);
  uVar1 = *(undefined4 *)((int)local_c + 0x146);
  *(float *)(this + 400) = -*(float *)(this + 400);
  *(undefined4 *)(this + 0x18c) = uVar1;
  local_10 = 0;
  fVar3 = (float)*(byte *)((int)local_c + 0x14a) * 0.003921569;
  *(float *)(this + 0x19c) = fVar3;
  *(float *)(this + 0x198) = fVar3;
  if (*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3 != 0) {
    do {
      EVar4 = local_10;
      pEVar2 = (EWeaponClass *)local_c[0x26];
      pEVar5 = std::vector<EWeaponClass,std::allocator<EWeaponClass>_>::_Umove<EWeaponClass*>
                         ((vector<EWeaponClass,std::allocator<EWeaponClass>_> *)local_c[0x25],
                          &local_10,unaff_EBX,(EWeaponClass *)ppiVar7);
      local_10 = *(EWeaponClass *)(*(int *)(this + 0xb0) + EVar4 * 8);
      if (pEVar5 == pEVar2) {
        FUN_00486024(&local_10);
        local_5 = 1;
        FUN_0053cb4f(&local_5);
      }
      else {
        FUN_00486024(&local_10);
      }
      local_10 = EVar4 + 1;
    } while (local_10 < (uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3));
  }
  if (local_c != (int *)0x0) {
    piVar6 = local_c + 1;
    *piVar6 = *piVar6 + -1;
    if (*piVar6 == 0) {
      (**(code **)(*local_c + 4))();
    }
  }
  return;
}



//=== GetDataFromDef @ 0053ed40 ===

/* [bsim sim=0.9159918899565794 <- ego_r]
   public: void __thiscall NUISystem::CScrollingViewport::GetDataFromDef(void) */

void __thiscall NUISystem::CScrollingViewport::GetDataFromDef(CScrollingViewport *this)

{
  int iVar1;
  int *unaff_ESI;
  CScrollingViewport *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  *(int *)(this + 0x174) = unaff_ESI[0x30];
  *(int *)(this + 0x178) = unaff_ESI[0x29];
  *(int *)(this + 0x17c) = unaff_ESI[0x2a];
  *(int *)(this + 0x180) = unaff_ESI[0x2b];
  *(int *)(this + 0x184) = unaff_ESI[0x2c];
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00541390 ===

/* [bsim sim=0.5465005889743761 <- ego_r]
   protected: void __thiscall NUISystem::CDynamicList::GetDataFromDef(void) */

void __thiscall NUISystem::CDynamicList::GetDataFromDef(CDynamicList *this)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  int iVar4;
  int *piStack_c;
  uint local_8 [2];
  
  (**(code **)(*(int *)this + 0x1b0))(local_8);
  *(undefined4 *)(this + 0x160) = *(undefined4 *)((int)piStack_c + 0x142);
  *(undefined4 *)(this + 0x164) = *(undefined4 *)((int)piStack_c + 0x146);
  *(int *)(this + 0x170) = piStack_c[0x4f] - piStack_c[0x4e];
  local_8[0] = 0;
  if (*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3 != 0) {
    do {
      uVar3 = local_8[0];
      iVar1 = piStack_c[0x26];
      iVar4 = FUN_00535f5a(local_8,&stack0xfffffff3);
      if (iVar4 == iVar1) {
        puVar2 = *(uint **)(this + 0x178);
        local_8[0] = *(uint *)(*(int *)(this + 0xb0) + uVar3 * 8);
        if (puVar2 == *(uint **)(this + 0x17c)) {
          FUN_00485dbc(puVar2,local_8,&stack0xfffffff3,1,1);
        }
        else {
          if (puVar2 != (uint *)0x0) {
            *puVar2 = local_8[0];
          }
          *(int *)(this + 0x178) = *(int *)(this + 0x178) + 4;
        }
      }
      local_8[0] = uVar3 + 1;
    } while (local_8[0] < (uint)(*(int *)(this + 0xb4) - *(int *)(this + 0xb0) >> 3));
  }
  if ((piStack_c != (int *)0x0) &&
     (iVar1 = piStack_c[1], piStack_c[1] = iVar1 + -1, iVar1 + -1 == 0)) {
    (**(code **)(*piStack_c + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00541fd0 ===

/* [bsim sim=0.6883442725027461 <- ego_r]
   public: void __thiscall NUISystem::CIconText::GetDataFromDef(void) */

void __thiscall NUISystem::CIconText::GetDataFromDef(CIconText *this)

{
  int *piVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  bool bVar5;
  CIconText CVar6;
  ulong uVar7;
  undefined3 extraout_var;
  CEngineInternalPrimitiveBase *this_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar8;
  int unaff_EBX;
  int *unaff_ESI;
  ulong unaff_EDI;
  CEnginePrimitive *pCVar9;
  CEnginePrimitive local_10 [4];
  undefined4 uStack_c;
  int *piStack_8;
  
  pCVar9 = local_10;
  (**(code **)(*(int *)this + 0x1b0))();
  CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)local_10);
  uVar7 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,pCVar9,unaff_EDI);
  bVar5 = CMessageEventFilter_CanBeSeenOrHeard::operator()
                    (*(CMessageEventFilter_CanBeSeenOrHeard **)(uVar7 + 0x84),
                     (CMessageEvent *)&uStack_c);
  piVar1 = (int *)((undefined4 *)CONCAT31(extraout_var,bVar5))[1];
  uVar2 = *(undefined4 *)CONCAT31(extraout_var,bVar5);
  piVar3 = *(int **)(this + 0x168);
  ppVar8 = extraout_EDX;
  if (piVar3 != piVar1) {
    if (piVar3 != (int *)0x0) {
      *piVar3 = *piVar3 + -1;
      if (**(int **)(this + 0x168) == 0) {
        (*(code *)(*(int **)(this + 0x168))[1])();
        operator_delete(*(void **)(this + 0x168));
        ppVar8 = extraout_EDX_00;
      }
    }
    *(undefined4 *)(this + 0x164) = uVar2;
    *(int **)(this + 0x168) = piVar1;
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + 1;
    }
  }
  if (piStack_8 != (int *)0x0) {
    *piStack_8 = *piStack_8 + -1;
    if (*piStack_8 == 0) {
      (*(code *)piStack_8[1])();
      operator_delete(piStack_8);
      ppVar8 = extraout_EDX_01;
    }
  }
  uStack_c = 0;
  piStack_8 = (int *)0x0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,ppVar8,
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
  *(undefined4 *)(this + 0x198) = *(undefined4 *)(unaff_EBX + 0xa4);
  *(undefined4 *)(this + 0x19c) = *(undefined4 *)(unaff_EBX + 0xa8);
  *(undefined4 *)(this + 0x1a0) = *(undefined4 *)(unaff_EBX + 0xac);
  *(undefined4 *)(this + 0x1a4) = *(undefined4 *)(unaff_EBX + 0xb0);
  *(undefined4 *)(this + 0x1a8) = *(undefined4 *)(unaff_EBX + 0x142);
  *(undefined4 *)(this + 0x1ac) = *(undefined4 *)(unaff_EBX + 0x146);
  (**(code **)(*(int *)this + 0x240))(unaff_EBX + 0x54);
  iVar4 = unaff_ESI[0x7f];
  if (iVar4 == 0) {
    CVar6 = (CIconText)((byte)this[0x12e] | 8);
  }
  else if (iVar4 == 1) {
    CVar6 = (CIconText)((byte)this[0x12e] | 0x10);
  }
  else {
    if (iVar4 != 2) goto LAB_00542123;
    CVar6 = (CIconText)((byte)this[0x12e] | 0x20);
  }
  this[0x12e] = CVar6;
LAB_00542123:
  iVar4 = unaff_ESI[1];
  unaff_ESI[1] = iVar4 + -1;
  if (iVar4 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00543fa0 ===

/* [bsim sim=0.9399278674939336 <- ego_r]
   public: void __thiscall NUISystem::CParticleEffect::GetDataFromDef(void) */

void __thiscall NUISystem::CParticleEffect::GetDataFromDef(CParticleEffect *this)

{
  int *unaff_ESI;
  int iVar1;
  CParticleEffect *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  iVar1 = unaff_ESI[0x11] - unaff_ESI[0x10] >> 0x1f;
  if ((unaff_ESI[0x11] - unaff_ESI[0x10]) / 0x7c + iVar1 == iVar1) {
    *(undefined4 *)(this + 0x160) = 0;
  }
  else {
    *(undefined4 *)(this + 0x160) = *(undefined4 *)(unaff_ESI[0x10] + 0x3c);
  }
  if ((unaff_ESI != (int *)0x0) && (iVar1 = unaff_ESI[1] + -1, unaff_ESI[1] = iVar1, iVar1 == 0)) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00544480 ===

/* [bsim sim=0.7825460310087032 <- ego_r]
   public: void __thiscall NUISystem::CSpellContainerList::GetDataFromDef(void) */

void __thiscall NUISystem::CSpellContainerList::GetDataFromDef(CSpellContainerList *this)

{
  int iVar1;
  int *unaff_ESI;
  uint local_8 [2];
  
  (**(code **)(*(int *)this + 0x1b0))(local_8);
  *(undefined4 *)(this + 0x198) = *(undefined4 *)((int)unaff_ESI + 0x142);
  *(undefined4 *)(this + 0x19c) = *(undefined4 *)((int)unaff_ESI + 0x146);
  *(undefined4 *)(this + 0x180) = *(undefined4 *)((int)unaff_ESI + 0x142);
  *(undefined4 *)(this + 0x184) = *(undefined4 *)((int)unaff_ESI + 0x146);
  local_8[0] = (uint)*(byte *)((int)unaff_ESI + 0x14a);
  *(float *)(this + 0x1a0) = (float)local_8[0] * 0.003921569;
  *(float *)(this + 0x188) = (float)local_8[0] * 0.003921569;
  *(int *)(this + 0x1a4) = unaff_ESI[0x61];
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00546aa0 ===

/* [bsim sim=0.7723793235093176 <- ego_r]
   public: void __thiscall NUISystem::CTextContainer::GetDataFromDef(void) */

void __thiscall NUISystem::CTextContainer::GetDataFromDef(CTextContainer *this)

{
  int iVar1;
  int *unaff_ESI;
  CTextContainer *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  *(int *)(this + 0x17c) = unaff_ESI[0x61];
  *(int *)(this + 0x180) = unaff_ESI[0x30];
  this[0x178] = (CTextContainer)0x0;
  this[0x179] = (CTextContainer)0x0;
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00546cd0 ===

/* [bsim sim=0.9131198222003186 <- ego_r]
   public: void __thiscall NUISystem::CTextContainer::GetDataFromDef(void) */

void __thiscall NUISystem::CTextContainer::GetDataFromDef(CTextContainer *this)

{
  int iVar1;
  int *unaff_ESI;
  CTextContainer *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  *(int *)(this + 0x15c) = unaff_ESI[0x29];
  *(int *)(this + 0x160) = unaff_ESI[0x2a];
  *(int *)(this + 0x164) = unaff_ESI[0x2b];
  *(int *)(this + 0x168) = unaff_ESI[0x2c];
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00547500 ===

/* [bsim sim=0.5308109062593769 <- ego_r]
   public: void __thiscall NUISystem::CList::GetDataFromDef(void) */

void __thiscall NUISystem::CList::GetDataFromDef(CList *this)

{
  undefined4 *puVar1;
  int iVar2;
  uint uVar3;
  int *piStack_10;
  undefined4 local_c;
  undefined4 uStack_8;
  
  (**(code **)(*(int *)this + 0x1b0))(&local_c);
  if ((char)piStack_10[0x80] != '\0') {
    this[0x12e] = (CList)((byte)this[0x12e] | 8);
  }
  uVar3 = 0;
  if (piStack_10[0x79] - piStack_10[0x78] >> 2 != 0) {
    do {
      local_c = *(undefined4 *)(piStack_10[0x78] + uVar3 * 4);
      uStack_8 = 0;
      if (uVar3 < (uint)(piStack_10[0x7c] - piStack_10[0x7b] >> 2)) {
        uStack_8 = *(undefined4 *)(piStack_10[0x7b] + uVar3 * 4);
      }
      puVar1 = *(undefined4 **)(this + 0x160);
      if (puVar1 == *(undefined4 **)(this + 0x164)) {
        FUN_005479a0(puVar1,&local_c,&stack0xffffffef,1,1);
      }
      else {
        if (puVar1 != (undefined4 *)0x0) {
          *puVar1 = local_c;
          puVar1[1] = uStack_8;
        }
        *(int *)(this + 0x160) = *(int *)(this + 0x160) + 8;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < (uint)(piStack_10[0x79] - piStack_10[0x78] >> 2));
  }
  if ((piStack_10 != (int *)0x0) &&
     (iVar2 = piStack_10[1], piStack_10[1] = iVar2 + -1, iVar2 + -1 == 0)) {
    (**(code **)(*piStack_10 + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00547c10 ===

/* [bsim sim=0.6086360973917236 <- ego_r]
   public: void __thiscall NUISystem::CTextContainer::GetDataFromDef(void) */

void __thiscall NUISystem::CTextContainer::GetDataFromDef(CTextContainer *this)

{
  int iVar1;
  undefined4 uVar2;
  int *unaff_ESI;
  CTextContainer *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  uVar2 = __ftol2();
  *(undefined4 *)(this + 0x168) = uVar2;
  uVar2 = __ftol2();
  *(undefined4 *)(this + 0x16c) = uVar2;
  this[0x170] = *(CTextContainer *)(unaff_ESI + 0x2f);
  CWideString::operator=((CWideString *)(this + 0x174),(CWideString *)(unaff_ESI + 0x15));
  *(int *)(this + 400) = unaff_ESI[0x2e];
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 0054e5d0 ===

/* [bsim sim=0.9399278674939336 <- ego_r]
   public: void __thiscall NUISystem::CParticleEffect::GetDataFromDef(void) */

void __thiscall NUISystem::CParticleEffect::GetDataFromDef(CParticleEffect *this)

{
  int *unaff_ESI;
  int iVar1;
  CParticleEffect *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  iVar1 = unaff_ESI[0x11] - unaff_ESI[0x10] >> 0x1f;
  if ((unaff_ESI[0x11] - unaff_ESI[0x10]) / 0x7c + iVar1 == iVar1) {
    *(undefined4 *)(this + 0x170) = 0;
  }
  else {
    *(undefined4 *)(this + 0x170) = *(undefined4 *)(unaff_ESI[0x10] + 0x3c);
  }
  if ((unaff_ESI != (int *)0x0) && (iVar1 = unaff_ESI[1] + -1, unaff_ESI[1] = iVar1, iVar1 == 0)) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 0054ed90 ===

/* [bsim sim=0.7007382807307786 <- ego_r]
   public: void __thiscall NUISystem::CText::GetDataFromDef(void) */

void __thiscall NUISystem::CText::GetDataFromDef(CText *this)

{
  int *piVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  bool bVar5;
  CText CVar6;
  ulong uVar7;
  undefined3 extraout_var;
  CEngineInternalPrimitiveBase *this_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *ppVar8;
  int unaff_EBX;
  ulong unaff_EDI;
  CEnginePrimitive *pCVar9;
  CEnginePrimitive local_10 [4];
  undefined4 uStack_c;
  int *piStack_8;
  
  pCVar9 = local_10;
  (**(code **)(*(int *)this + 0x1b0))();
  CDefStringTable::GetString((CDefStringTable *)&DAT_013ca828,(long)local_10);
  uVar7 = CEngineInternalPrimitiveBase::AddChildPrimitive(this_00,pCVar9,unaff_EDI);
  bVar5 = CMessageEventFilter_CanBeSeenOrHeard::operator()
                    (*(CMessageEventFilter_CanBeSeenOrHeard **)(uVar7 + 0x84),
                     (CMessageEvent *)&uStack_c);
  piVar1 = (int *)((undefined4 *)CONCAT31(extraout_var,bVar5))[1];
  uVar2 = *(undefined4 *)CONCAT31(extraout_var,bVar5);
  piVar3 = *(int **)(this + 0x164);
  ppVar8 = extraout_EDX;
  if (piVar3 != piVar1) {
    if (piVar3 != (int *)0x0) {
      *piVar3 = *piVar3 + -1;
      if (**(int **)(this + 0x164) == 0) {
        (*(code *)(*(int **)(this + 0x164))[1])();
        operator_delete(*(void **)(this + 0x164));
        ppVar8 = extraout_EDX_00;
      }
    }
    *(undefined4 *)(this + 0x160) = uVar2;
    *(int **)(this + 0x164) = piVar1;
    if (piVar1 != (int *)0x0) {
      *piVar1 = *piVar1 + 1;
    }
  }
  if (piStack_8 != (int *)0x0) {
    *piStack_8 = *piStack_8 + -1;
    if (*piStack_8 == 0) {
      (*(code *)piStack_8[1])();
      operator_delete(piStack_8);
      ppVar8 = extraout_EDX_01;
    }
  }
  uStack_c = 0;
  piStack_8 = (int *)0x0;
  std::
  _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
            ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)local_10,ppVar8,
             (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar9);
  *(undefined4 *)(this + 0x178) = *(undefined4 *)(unaff_EBX + 0xa4);
  *(undefined4 *)(this + 0x17c) = *(undefined4 *)(unaff_EBX + 0xa8);
  *(undefined4 *)(this + 0x180) = *(undefined4 *)(unaff_EBX + 0xac);
  *(undefined4 *)(this + 0x184) = *(undefined4 *)(unaff_EBX + 0xb0);
  this[0x188] = *(CText *)(unaff_EBX + 0xbe);
  (**(code **)(*(int *)this + 0x240))(unaff_EBX + 0x54,1,*(undefined1 *)(unaff_EBX + 0xbd));
  iVar4 = *(int *)(pCVar9 + 0x1fc);
  if (iVar4 == 0) {
    CVar6 = (CText)((byte)this[0x12e] | 8);
  }
  else if (iVar4 == 1) {
    CVar6 = (CText)((byte)this[0x12e] | 0x10);
  }
  else {
    if (iVar4 != 2) goto LAB_0054eee2;
    CVar6 = (CText)((byte)this[0x12e] | 0x20);
  }
  this[0x12e] = CVar6;
LAB_0054eee2:
  iVar4 = *(int *)(pCVar9 + 4);
  *(int *)(pCVar9 + 4) = iVar4 + -1;
  if (iVar4 + -1 == 0) {
    (**(code **)(*(int *)pCVar9 + 4))();
  }
  return;
}



//=== GetDataFromDef @ 005500b0 ===

/* [bsim sim=0.7404499377699721 <- ego_r]
   public: void __thiscall NUISystem::CScrollingViewport::GetDataFromDef(void) */

void __thiscall NUISystem::CScrollingViewport::GetDataFromDef(CScrollingViewport *this)

{
  int iVar1;
  undefined4 uVar2;
  int *unaff_ESI;
  CScrollingViewport *local_4;
  
  local_4 = this;
  (**(code **)(*(int *)this + 0x1b0))(&local_4);
  *(int *)(this + 0x15c) = unaff_ESI[0x13];
  *(int *)(this + 0x160) = unaff_ESI[0x28];
  uVar2 = __ftol2();
  *(undefined4 *)(this + 0x16c) = uVar2;
  uVar2 = __ftol2();
  *(undefined4 *)(this + 0x170) = uVar2;
  *(int *)(this + 0x174) = unaff_ESI[0x41];
  *(int *)(this + 0x178) = unaff_ESI[0x42];
  *(int *)(this + 0x17c) = unaff_ESI[0x43];
  *(int *)(this + 0x180) = unaff_ESI[0x44];
  *(int *)(this + 0x184) = unaff_ESI[0x45];
  *(int *)(this + 0x188) = unaff_ESI[0x46];
  *(int *)(this + 0x18c) = unaff_ESI[0x47];
  *(int *)(this + 400) = unaff_ESI[0x48];
  *(int *)(this + 0x194) = unaff_ESI[0x49];
  *(int *)(this + 0x198) = unaff_ESI[0x4a];
  *(int *)(this + 0x19c) = unaff_ESI[0x4b];
  iVar1 = unaff_ESI[1];
  unaff_ESI[1] = iVar1 + -1;
  if (iVar1 + -1 == 0) {
    (**(code **)(*unaff_ESI + 4))();
  }
  return;
}



//=== GetDataFromDef @ 00551340 ===

/* [bsim sim=0.6755399530457568 <- ego_r]
   public: void __thiscall NUISystem::CTable::GetDataFromDef(void) */

void __thiscall NUISystem::CTable::GetDataFromDef(CTable *this)

{
  int *piVar1;
  int iVar2;
  void *pvVar3;
  undefined4 uVar4;
  CFrontEndManager *this_00;
  CComponent *pCVar5;
  int *piVar6;
  undefined4 *puVar7;
  int iVar8;
  int iVar9;
  int *unaff_EBX;
  long lVar10;
  CTable *local_10;
  CComponent *pCStack_c;
  
  (**(code **)(*(int *)this + 0x1b0))(&local_10);
  *(int *)(this + 0xcc) = unaff_EBX[0x17];
  *(int *)(this + 0xd0) = unaff_EBX[0x16];
  pvVar3 = ::operator_new(0xc);
  if (pvVar3 == (void *)0x0) {
    uVar4 = 0;
  }
  else {
    uVar4 = FUN_00535470(unaff_EBX + 0x1f);
  }
  *(undefined4 *)(this + 0x168) = uVar4;
  pvVar3 = ::operator_new(0xc);
  if (pvVar3 == (void *)0x0) {
    uVar4 = 0;
  }
  else {
    uVar4 = FUN_00535470(unaff_EBX + 0x22);
  }
  *(undefined4 *)(this + 0x16c) = uVar4;
  iVar8 = *(int *)(unaff_EBX[0x19] + 8);
  if (iVar8 != unaff_EBX[0x19]) {
    local_10 = this + 0x15c;
    do {
      lVar10 = *(long *)(iVar8 + 0x14);
      this_00 = CFrontEndManager::GetInstance();
      pCVar5 = CManager::CreateComponent((CManager *)this_00,lVar10);
      pCStack_c = pCVar5;
      if ((pCVar5 == (CComponent *)0x0) || (piVar6 = ::operator_new(0xc), piVar6 == (int *)0x0)) {
        piVar6 = (int *)0x0;
      }
      else {
        piVar6[2] = (int)pCVar5;
        *piVar6 = 1;
        piVar6[1] = (int)CCountedPointer<CEditTransactionBase>::DeleteData;
      }
      puVar7 = (undefined4 *)FUN_00554030(iVar8 + 0x10);
      piVar1 = (int *)puVar7[1];
      if (piVar1 == piVar6) {
LAB_00551463:
        if ((piVar6 != (int *)0x0) && (*piVar6 = *piVar6 + -1, *piVar6 == 0)) {
          (*(code *)piVar6[1])();
          operator_delete(piVar6);
        }
      }
      else {
        if (piVar1 != (int *)0x0) {
          *piVar1 = *piVar1 + -1;
          if (*(int *)puVar7[1] == 0) {
            (*(code *)((int *)puVar7[1])[1])();
            operator_delete((void *)puVar7[1]);
          }
        }
        *puVar7 = pCStack_c;
        puVar7[1] = piVar6;
        if (piVar6 != (int *)0x0) {
          *piVar6 = *piVar6 + 1;
          goto LAB_00551463;
        }
      }
      puVar7 = (undefined4 *)FUN_00554030(iVar8 + 0x10);
      (**(code **)(*(int *)*puVar7 + 0xac))();
      iVar9 = *(int *)(iVar8 + 0xc);
      if (iVar9 == 0) {
        iVar9 = *(int *)(iVar8 + 4);
        if (iVar8 == *(int *)(iVar9 + 0xc)) {
          do {
            iVar8 = iVar9;
            iVar9 = *(int *)(iVar8 + 4);
          } while (iVar8 == *(int *)(iVar9 + 0xc));
        }
        if (*(int *)(iVar8 + 0xc) != iVar9) {
          iVar8 = iVar9;
        }
      }
      else {
        for (iVar2 = *(int *)(iVar9 + 8); iVar8 = iVar9, iVar2 != 0; iVar2 = *(int *)(iVar2 + 8)) {
          iVar9 = iVar2;
        }
      }
    } while (iVar8 != unaff_EBX[0x19]);
  }
  iVar8 = unaff_EBX[1];
  unaff_EBX[1] = iVar8 + -1;
  if (iVar8 + -1 == 0) {
    (**(code **)(*unaff_EBX + 4))();
  }
  return;
}



//=== GetDataFromDef @ 005542f0 ===

/* [bsim sim=0.8049532407836639 <- ego_r]
   public: void __thiscall NUISystem::CMorphingSprite::GetDataFromDef(void) */

void __thiscall NUISystem::CMorphingSprite::GetDataFromDef(CMorphingSprite *this)

{
  int *piVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  CResource *pCVar7;
  bool bVar8;
  CFrontEndManager *pCVar9;
  CFrameData *pCVar10;
  CGraphicInfoFrame *this_00;
  CGraphicFrameData *this_01;
  int iVar11;
  int *piStack_1c;
  uint local_18;
  CMorphingSprite *pCStack_14;
  undefined1 auStack_10 [4];
  uint uStack_c;
  undefined4 uStack_8;
  
  (**(code **)(*(int *)this + 0x1b0))(&local_18);
  iVar11 = piStack_1c[0x11] - piStack_1c[0x10] >> 0x1f;
  local_18 = 0;
  if ((piStack_1c[0x11] - piStack_1c[0x10]) / 0x7c + iVar11 != iVar11) {
    pCStack_14 = this + 0x18c;
    iVar11 = 0;
    do {
      uStack_8 = *(undefined4 *)(iVar11 + 0x3c + piStack_1c[0x10]);
      uStack_c = local_18;
      FUN_005537f0(auStack_10,*(undefined4 *)(*(int *)pCStack_14 + 8),&uStack_c);
      iVar3 = piStack_1c[0x10];
      pCVar9 = CFrontEndManager::GetInstance();
      piVar4 = *(int **)(pCVar9 + 0x14);
      iVar5 = *(int *)(pCVar9 + 0x10);
      if (piVar4 != (int *)0x0) {
        *piVar4 = *piVar4 + 1;
      }
      uVar6 = *(uint *)(iVar11 + 0x3c + iVar3);
      iVar3 = *(int *)(iVar5 + 0x1e0);
      if ((uVar6 < (uint)((*(int *)(iVar5 + 0x1e4) - iVar3) / 0x2c)) &&
         (uVar2 = uVar6 * 0x2c + iVar3, uVar2 != 0)) {
        this_00 = *(CGraphicInfoFrame **)
                   ((-(uint)(uVar6 < (uint)((*(int *)(iVar5 + 0x1e4) - iVar3) / 0x2c)) & uVar2) +
                   0x28);
      }
      else {
        this_00 = (CGraphicInfoFrame *)0x0;
      }
      if ((piVar4 != (int *)0x0) && (*piVar4 = *piVar4 + -1, *piVar4 == 0)) {
        (*(code *)piVar4[1])();
        operator_delete(piVar4);
      }
      CGraphicInfoFrame::BeginLoading(this_00,0,0x10);
      pCVar10 = *(CFrameData **)(this_00 + 8);
      if (pCVar10 == (CFrameData *)0x0) {
        pCVar10 = ::operator_new(0x44);
        if (pCVar10 == (CFrameData *)0x0) {
          pCVar10 = (CFrameData *)0x0;
        }
        else {
          pCVar10 = (CFrameData *)CGraphicInfoFrame::CFrameData::CFrameData(pCVar10,this_00);
        }
        piVar4 = *(int **)(this_00 + 8);
        if (piVar4 != (int *)0x0) {
          piVar1 = piVar4 + 1;
          *piVar1 = *piVar1 + -1;
          if (*piVar1 == 0) {
            (**(code **)(*piVar4 + 4))();
          }
          *(undefined4 *)(this_00 + 8) = 0;
        }
        *(CFrameData **)(this_00 + 8) = pCVar10;
LAB_00554505:
        CGraphicInfoFrame::CFrameData::BeginLoading(pCVar10,0,1);
        bVar8 = CGraphicInfoFrame::InternalGetFrameForcedLoading(this_00,1);
        if (bVar8) {
LAB_0055451a:
          this_01 = *(CGraphicFrameData **)(*(int *)(this_00 + 8) + 0x1c);
          goto LAB_00554520;
        }
      }
      else {
        if (*(int *)(pCVar10 + 0x1c) == 0) goto LAB_00554505;
        *(undefined4 *)(pCVar10 + 0x3c) =
             *(undefined4 *)(*(int *)(*(int *)(this_00 + 0xc) + 0x20) + 0x194);
        pCVar7 = *(CResource **)(*(int *)(this_00 + 8) + 0x24);
        if (((pCVar7 != (CResource *)0x0) && (*(int *)(pCVar7 + 8) != 0)) &&
           (*(int *)(pCVar7 + 0x18) != *(int *)(*(int *)(pCVar7 + 8) + 0x2c))) {
          CResource::ListRemove(pCVar7);
          CResource::ListInsert
                    (pCVar7,*(CResource **)(*(int *)(pCVar7 + 8) + 0x10),
                     (CResource *)(*(int *)(pCVar7 + 8) + 4));
          *(undefined4 *)(pCVar7 + 0x18) = *(undefined4 *)(*(int *)(pCVar7 + 8) + 0x2c);
        }
        pCVar7 = *(CResource **)(this_00 + 8);
        if ((*(int *)(pCVar7 + 8) != 0) &&
           (*(int *)(pCVar7 + 0x18) != *(int *)(*(int *)(pCVar7 + 8) + 0x2c))) {
          CResource::ListRemove(pCVar7);
          CResource::ListInsert
                    (pCVar7,*(CResource **)(*(int *)(pCVar7 + 8) + 0x10),
                     (CResource *)(*(int *)(pCVar7 + 8) + 4));
          *(undefined4 *)(pCVar7 + 0x18) = *(undefined4 *)(*(int *)(pCVar7 + 8) + 0x2c);
        }
        if (*(char *)(*(int *)(this_00 + 8) + 0x3a) != '\0') {
          *(undefined1 *)(*(int *)(this_00 + 8) + 0x3a) = 0;
        }
        pCVar10 = *(CFrameData **)(this_00 + 8);
        if (pCVar10[0x38] != (CFrameData)0x0) {
          CGraphicInfoFrame::CFrameData::BeginLoading(pCVar10,0,1);
          goto LAB_0055451a;
        }
        this_01 = *(CGraphicFrameData **)(pCVar10 + 0x1c);
LAB_00554520:
        if (this_01 != (CGraphicFrameData *)0x0) {
          iVar3 = *(int *)this_01;
          *(int *)this_01 = iVar3 + 1;
          *(int *)this_01 = iVar3;
          if (iVar3 == 0) {
            if (((byte)this_01[0x1a] & 0x10) != 0) {
              CGraphicFrameData::Uninitialise(this_01);
            }
            piVar4 = *(int **)(this_01 + 0xc);
            if (piVar4 != (int *)0x0) {
              *piVar4 = *piVar4 + -1;
              if (**(int **)(this_01 + 0xc) == 0) {
                (*(code *)(*(int **)(this_01 + 0xc))[1])();
                operator_delete(*(void **)(this_01 + 0xc));
              }
            }
            *(undefined4 *)(this_01 + 8) = 0;
            *(undefined4 *)(this_01 + 0xc) = 0;
            operator_delete(this_01);
          }
        }
      }
      iVar11 = iVar11 + 0x7c;
      local_18 = local_18 + 1;
    } while (local_18 < (uint)((piStack_1c[0x11] - piStack_1c[0x10]) / 0x7c));
  }
  if ((piStack_1c != (int *)0x0) &&
     (iVar11 = piStack_1c[1], piStack_1c[1] = iVar11 + -1, iVar11 + -1 == 0)) {
    (**(code **)(*piStack_1c + 4))();
  }
  return;
}



//=== GetDataFromDef @ 005e21f0 ===

/* [bsim sim=0.5031350210460788 <- ego_r]
   public: void __thiscall NUISystem::CParticleEffect::GetDataFromDef(void) */

void __thiscall NUISystem::CParticleEffect::GetDataFromDef(CParticleEffect *this)

{
  int *piVar1;
  int in_EDX;
  CAnimationEntry *in_stack_00000004;
  undefined4 in_stack_00000008;
  undefined4 in_stack_0000000c;
  undefined4 in_stack_00000010;
  undefined4 in_stack_00000014;
  int *in_stack_00000018;
  undefined4 in_stack_0000001c;
  
  CAnimationEntry::operator=(in_stack_00000004,(CAnimationEntry *)this);
  if (in_stack_00000018 != (int *)0x0) {
    in_stack_00000018[1] = in_stack_00000018[1] + 1;
  }
  std::
  _Adjust_heap<std::pair<unsigned_long,CAnimationEntry>*,int,std::pair<unsigned_long,class_CAnimationEntry>,CKeyPairCompareLess<unsigned_long,CAnimationEntry>_>
            (this,0,(in_EDX - (int)this) / 0x14,in_stack_00000008,in_stack_0000000c,
             in_stack_00000010,in_stack_00000014,in_stack_00000018,in_stack_0000001c);
  if (in_stack_00000018 != (int *)0x0) {
    piVar1 = in_stack_00000018 + 1;
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      (**(code **)(*in_stack_00000018 + 4))();
    }
  }
  return;
}



//=== GetDataFromEventString @ 00870c90 ===

/* [bsim sim=0.745629000148121 <- ego_r]
   bool __fastcall `anonymous namespace'::GetDataFromEventString(class CCharString const
   &,long,class CCharString &) */

bool __fastcall
_anon_AF422EAE::GetDataFromEventString(CCharString *param_1,long param_2,CCharString *param_3)

{
  CCharString *this;
  long lVar1;
  CCharString *pCVar2;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_00;
  pair<EHeroMorphType,CParticleMorphs::CEntry> *extraout_EDX_01;
  pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *unaff_EDI;
  
  this = param_3;
  CCharString::operator=(param_3,param_1);
  lVar1 = CCharString::FindCharPos(param_1,' ');
  if (lVar1 + 1U != 0) {
    pCVar2 = (CCharString *)CCharString::GetSubString(param_1,(ulong)&param_3,lVar1 + 1U);
    CCharString::operator=(this,pCVar2);
    std::
    _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
              ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
               extraout_EDX,unaff_EDI);
    while( true ) {
      param_2 = param_2 + -1;
      if (param_2 == -1) {
        CCharString::FindCharPos(this,' ');
        pCVar2 = (CCharString *)CCharString::GetSubString(this,(ulong)&param_3,0);
        CCharString::operator=(this,pCVar2);
        std::
        _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                  ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
                   extraout_EDX_01,unaff_EDI);
        return true;
      }
      lVar1 = CCharString::FindCharPos(this,' ');
      if (lVar1 + 1U == 0) break;
      pCVar2 = (CCharString *)CCharString::GetSubString(this,(ulong)&param_3,lVar1 + 1U);
      CCharString::operator=(this,pCVar2);
      std::
      _Cons_val<std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>,std::pair<EHeroMorphType,CParticleMorphs::CEntry>,std::pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>_const&>
                ((allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)&param_3,
                 extraout_EDX_00,unaff_EDI);
    }
  }
  return false;
}



//=== ?GetDataString@CGameScriptThing@@UBE?AVCCharString@@XZ @ 008cfe00 ===

/* [bsim sim=1.0 <- ego_r]
   public: class CCharString __thiscall CBankFile::GetBankHandle(void)const  */

CCharString * __thiscall
CBankFile::_GetDataString_CGameScriptThing__UBE_AVCCharString__XZ(CBankFile *this)

{
  CCharString *in_stack_00000004;
  
  CCharString::CCharString(in_stack_00000004,(CCharString *)(this + 0x24));
  return in_stack_00000004;
}



//=== SetupBuffer @ 009d52d0 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   private: bool __thiscall CBankFileAsyncData::SetupBuffer(bool) */

bool __thiscall CBankFileAsyncData::SetupBuffer(CBankFileAsyncData *this,bool param_1)

{
  bool bVar1;
  ulong uVar2;
  void *pvVar3;
  uint uVar4;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX_00;
  allocator<NCreatureHitNotification::CFlash> *paVar5;
  allocator<NCreatureHitNotification::CFlash> *extraout_ECX_01;
  CFlash *extraout_EDX;
  CFlash *extraout_EDX_00;
  CFlash *pCVar6;
  CFlash *extraout_EDX_01;
  CCriticalSectionLock local_8 [4];
  char local_4;
  
  CCriticalSectionLock::CCriticalSectionLock
            (local_8,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),!param_1);
  if (local_4 == '\0') {
    CCriticalSectionLock::~CCriticalSectionLock(local_8);
    return false;
  }
  if (*(int *)(this + 0xc) != 0) {
    CCriticalSectionLock::~CCriticalSectionLock(local_8);
    return true;
  }
  if (*(int *)(this + 0x1c) != 0) {
    uVar2 = CBankFile::Align(*(CBankFile **)(this + 0x28),
                             *(int *)(this + 0x10) + *(int *)(this + 0x14));
    uVar4 = DAT_013ca868;
    if (!param_1) {
      uVar4 = DAT_013ca868 | 3;
    }
    bVar1 = CMovableResourceMemoryPool::AllocateResource
                      (*(CMovableResourceMemoryPool **)(this + 0x1c),(long *)(this + 0x20),
                       (CMovableResource *)(this + 4),uVar2,uVar4);
    if (bVar1) {
      pvVar3 = CHandleBasedMemoryPool::GetPointerFromHandle
                         (*(CHandleBasedMemoryPool **)(this + 0x1c),*(long *)(this + 0x20));
      *(void **)(this + 0xc) = pvVar3;
      *(int *)(this + 8) = *(int *)(this + 0x14) + (int)pvVar3;
      CCriticalSectionLock::~CCriticalSectionLock(local_8);
      return true;
    }
    if (!param_1) {
      CCriticalSectionLock::~CCriticalSectionLock(local_8);
      return false;
    }
  }
  uVar2 = CBankFile::Align(*(CBankFile **)(this + 0x28),
                           *(int *)(this + 0x10) + *(int *)(this + 0x14));
  *(ulong *)(this + 0x24) = uVar2;
  paVar5 = extraout_ECX;
  pCVar6 = extraout_EDX;
  if (DAT_013caa90 == (CMemoryAllocator *)0x0) {
    CMemoryAllocator::Create();
    paVar5 = extraout_ECX_00;
    pCVar6 = extraout_EDX_00;
  }
  std::_Dest_val<std::allocator<NCreatureHitNotification::CFlash>,NCreatureHitNotification::CFlash>
            (paVar5,pCVar6);
  pvVar3 = CMemoryAllocator::InternalAllocate(DAT_013caa90,uVar2,0,false);
  std::_Dest_val<std::allocator<NCreatureHitNotification::CFlash>,NCreatureHitNotification::CFlash>
            (extraout_ECX_01,extraout_EDX_01);
  *(int *)(this + 8) = *(int *)(this + 0x14) + (int)pvVar3;
  *(void **)(this + 0xc) = pvVar3;
  CCriticalSectionLock::~CCriticalSectionLock(local_8);
  return true;
}



//=== GetData @ 009d5530 ===

/* [bsim sim=0.9927045380975223 <- ego_r]
   public: void * __thiscall CBankFileAsyncData::GetData(void) */

void * __thiscall CBankFileAsyncData::GetData(CBankFileAsyncData *this)

{
  void *pvVar1;
  CCriticalSectionLock local_8 [8];
  
  CCriticalSectionLock::CCriticalSectionLock
            (local_8,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),false);
  if (((*(CHandleBasedMemoryPool **)(this + 0x1c) != (CHandleBasedMemoryPool *)0x0) &&
      (*(int *)(this + 0x20) != -1)) && (this[0x45] == (CBankFileAsyncData)0x0)) {
    CHandleBasedMemoryPool::Lock(*(CHandleBasedMemoryPool **)(this + 0x1c),*(int *)(this + 0x20));
    this[0x45] = (CBankFileAsyncData)0x1;
  }
  pvVar1 = *(void **)(this + 8);
  CCriticalSectionLock::~CCriticalSectionLock(local_8);
  return pvVar1;
}



//=== GetData @ 009d5580 ===

/* [bsim sim=0.9927045380975223 <- ego_r]
   public: void * __thiscall CBankFileAsyncData::GetData(void) */

void * __thiscall CBankFileAsyncData::GetData(CBankFileAsyncData *this)

{
  void *pvVar1;
  CCriticalSectionLock local_8 [8];
  
  CCriticalSectionLock::CCriticalSectionLock
            (local_8,(_RTL_CRITICAL_SECTION *)(*(int *)(this + 0x28) + 0x14c),false);
  if (((*(CHandleBasedMemoryPool **)(this + 0x1c) != (CHandleBasedMemoryPool *)0x0) &&
      (*(int *)(this + 0x20) != -1)) && (this[0x45] == (CBankFileAsyncData)0x0)) {
    CHandleBasedMemoryPool::Lock(*(CHandleBasedMemoryPool **)(this + 0x1c),*(int *)(this + 0x20));
    this[0x45] = (CBankFileAsyncData)0x1;
  }
  pvVar1 = *(void **)(this + 8);
  CCriticalSectionLock::~CCriticalSectionLock(local_8);
  return pvVar1;
}



//=== ?GetDataFormatVersion@CGraphicDataBank@@MAEKXZ @ 009fd0b0 ===

undefined4 _GetDataFormatVersion_CGraphicDataBank__MAEKXZ(void)

{
  return 0x7a;
}



//=== ?GetDataFormatVersion@CMeshDataBank@@MAEKXZ @ 00a23ac0 ===

undefined4 _GetDataFormatVersion_CMeshDataBank__MAEKXZ(void)

{
  return 0xcd;
}



//=== ?GetDataFormatVersion@CLipSyncDataBank@@UAEKXZ @ 00a42790 ===

undefined4 _GetDataFormatVersion_CLipSyncDataBank__UAEKXZ(void)

{
  return 0x70;
}



//=== ?GetDataFormatVersion@CStreamingFontDataBank@@MAEKXZ @ 00a5ee90 ===

undefined4 _GetDataFormatVersion_CStreamingFontDataBank__MAEKXZ(void)

{
  return 0x6d;
}



//=== ?GetDataFormatVersion@CShaderDataBank@@MAEKXZ @ 00a61f90 ===

undefined4 _GetDataFormatVersion_CShaderDataBank__MAEKXZ(void)

{
  return 0x6b;
}



//=== ?GetDataFormatVersion@CFontDataBank@@MAEKXZ @ 00a63340 ===

undefined4 _GetDataFormatVersion_CFontDataBank__MAEKXZ(void)

{
  return 0x69;
}



//=== GetDataTypeSize @ 00ab3740 ===

/* [bsim sim=0.9999999999999999 <- ego_r]
   public: static unsigned long __fastcall CVertexStreamFormat::GetDataTypeSize(enum
   EVertexStreamDataType) */

ulong __fastcall CVertexStreamFormat::GetDataTypeSize(EVertexStreamDataType param_1)

{
  switch(param_1) {
  case 0:
  case 4:
  case 5:
  case 7:
  case 9:
  case 0xc:
  case 0xd:
    return 4;
  case 1:
  case 6:
  case 0xb:
    return 8;
  case 2:
    return 0xc;
  case 3:
    return 0x10;
  case 8:
    return 2;
  case 10:
    return 6;
  default:
    return 0;
  }
}



//=== ?GetDataFormatVersion@CStaticMapBankFile@@UAEKXZ @ 00b3edb0 ===

undefined4 _GetDataFormatVersion_CStaticMapBankFile__UAEKXZ(void)

{
  return 0xae;
}



