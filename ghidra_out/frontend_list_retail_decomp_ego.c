//=== ?ProcessEvent@CList@NUISystem@@UAEXW4EEvent@2@@Z @ 005af56d (seed 005af56d) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CList_NUISystem__UAEXW4EEvent_2__Z(int param_1)

{
  char cVar1;
  int *piVar2;
  int iVar3;
  uint in_ECX;
  int *piVar4;
  int *local_c;
  undefined4 uStack_8;
  
  local_c = (int *)0x0;
  if (*(float *)(in_ECX + 0x22c) <= *(float *)(in_ECX + 0x34) - *(float *)(in_ECX + 0x18c)) {
    uStack_8 = in_ECX;
    piVar2 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
    cVar1 = (**(code **)(*piVar2 + 0x2c))();
    if ((cVar1 == '\0') || (param_1 != 0x19)) {
      iVar3 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      if (*(char *)(iVar3 + 0xd0) == '\0') {
        piVar4 = (int *)(in_ECX - 4);
        piVar2 = (int *)(**(code **)(*piVar4 + 0x1b0))(&local_c);
        if ((*(char *)(*piVar2 + 0x23a) != '\0') ||
           (uStack_8 = CONCAT13(1,(undefined3)uStack_8), DAT_0126de20 == 0)) {
          uStack_8 = uStack_8 & 0xffffff;
        }
        if ((local_c != (int *)0x0) && (local_c[1] = local_c[1] + -1, local_c[1] == 0)) {
          (**(code **)(*local_c + 4))();
        }
        if (uStack_8._3_1_ == '\0') {
          if (param_1 == 0) {
            (**(code **)(*piVar4 + 0x2d8))();
          }
          else if (param_1 == 1) {
            (**(code **)(*piVar4 + 0x2dc))();
          }
        }
      }
    }
  }
  return;
}


// CALLEES of ?ProcessEvent@CList@NUISystem@@UAEXW4EEvent@2@@Z:
//   00438c20  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ

//=== ?ProcessEvent@CSpellContainerList@NUISystem@@UAEXW4EEvent@2@@Z @ 005be6c0 (seed 005be6c0) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CSpellContainerList_NUISystem__UAEXW4EEvent_2__Z(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  float *pfVar7;
  float *pfVar8;
  int iVar9;
  undefined4 *puVar10;
  int *piVar11;
  void *pvVar12;
  undefined4 uVar13;
  int *piVar14;
  uint in_ECX;
  int **ppiVar15;
  undefined4 **ppuVar16;
  undefined4 *puStack_5c;
  int *piStack_58;
  int *piStack_54;
  undefined4 *puStack_50;
  undefined4 *apuStack_4c [2];
  undefined1 auStack_44 [4];
  int *piStack_40;
  float fStack_3c;
  int *piStack_38;
  float fStack_34;
  undefined1 auStack_28 [4];
  undefined1 auStack_24 [12];
  undefined1 auStack_18 [4];
  undefined1 auStack_14 [16];
  
  if (*(float *)(in_ECX + 0x34) - *(float *)(in_ECX + 0x188) < *(float *)(in_ECX + 0x1d4)) {
    return;
  }
  switch(param_1) {
  case 2:
  case 5:
    piVar14 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
    uVar6 = (**(code **)(*piVar14 + 0x1c))();
    if ((-(uint)(in_ECX != 4) & in_ECX) == uVar6) {
      piVar14 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(*piVar14 + 0x10))(-(uint)(in_ECX != 4) & in_ECX);
      puVar10 = (undefined4 *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)*puVar10)(5);
      (**(code **)(*(int *)(in_ECX - 4) + 0x248))();
      *(undefined4 *)(in_ECX + 0x188) = *(undefined4 *)(in_ECX + 0x34);
      uVar6 = 0;
      if (*(int *)(in_ECX + 0x1a0) - *(int *)(in_ECX + 0x19c) >> 2 != 0) {
        do {
          (**(code **)(**(int **)(*(int *)(in_ECX + 0x19c) + uVar6 * 4) + 0x248))();
          uVar6 = uVar6 + 1;
        } while (uVar6 < (uint)(*(int *)(in_ECX + 0x1a0) - *(int *)(in_ECX + 0x19c) >> 2));
      }
      pvVar12 = operator_new(0x20);
      if (pvVar12 == (void *)0x0) {
        uVar13 = 0;
      }
      else {
        uVar13 = __0CAction_NUISystem__QAE_XZ();
      }
      __0__CCountedPointer_VCAction_NUISystem____QAE_PAVCAction_NUISystem___Z(uVar13);
      *puStack_50 = 0xd3;
      piVar14 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(*piVar14 + 0x38))(&puStack_50);
      _ReleaseData___CCountedPointer_VCEventBase_NEntityEvents____QAEXXZ();
      return;
    }
    break;
  case 0xe:
    iVar9 = *(int *)(in_ECX + 0x18c);
    if (iVar9 == 0) {
      piVar14 = (int *)(**(code **)(**(int **)(*(int *)(in_ECX + 0x19c) + *(int *)(in_ECX + 400) * 4
                                              ) + 0xd4))();
      *(int *)(in_ECX + 0x18c) = (piVar14[1] - *piVar14 >> 3) + -1;
    }
    else {
      *(int *)(in_ECX + 0x18c) = iVar9 + -1;
    }
    if (*(int *)(in_ECX + 0x194) == 0) goto LAB_005be921;
    piVar14 = *(int **)(*(int *)(in_ECX + 0x19c) + *(int *)(in_ECX + 400) * 4);
    piVar11 = (int *)(**(code **)(*piVar14 + 0xd4))();
    piStack_58 = *(int **)(in_ECX + 0x18c);
    piVar11 = *(int **)(*piVar11 + iVar9 * 8);
    piVar14 = (int *)(**(code **)(*piVar14 + 0xd4))();
    piVar14 = *(int **)(*piVar14 + (int)piStack_58 * 8);
    pfVar7 = (float *)(**(code **)(*piVar11 + 0xdc))(auStack_28);
    pfVar8 = (float *)(**(code **)(*piVar14 + 0xdc))(auStack_24);
    fStack_3c = pfVar8[1] - pfVar7[1];
    ppiVar15 = &piStack_40;
    piStack_40 = (int *)(*pfVar8 - *pfVar7);
    goto LAB_005be904;
  case 0xf:
    iVar9 = *(int *)(in_ECX + 0x18c);
    piVar14 = (int *)(**(code **)(**(int **)(*(int *)(in_ECX + 0x19c) + *(int *)(in_ECX + 400) * 4)
                                 + 0xd4))();
    if (*(int *)(in_ECX + 0x18c) == (piVar14[1] - *piVar14 >> 3) + -1) {
      *(undefined4 *)(in_ECX + 0x18c) = 0;
    }
    else {
      *(int *)(in_ECX + 0x18c) = *(int *)(in_ECX + 0x18c) + 1;
    }
    if (*(int *)(in_ECX + 0x194) == 0) goto LAB_005be921;
    piVar14 = *(int **)(*(int *)(in_ECX + 0x19c) + *(int *)(in_ECX + 400) * 4);
    piVar11 = (int *)(**(code **)(*piVar14 + 0xd4))();
    piStack_58 = *(int **)(in_ECX + 0x18c);
    piVar11 = *(int **)(*piVar11 + iVar9 * 8);
    piVar14 = (int *)(**(code **)(*piVar14 + 0xd4))();
    piVar14 = *(int **)(*piVar14 + (int)piStack_58 * 8);
    pfVar7 = (float *)(**(code **)(*piVar11 + 0xdc))(auStack_18);
    pfVar8 = (float *)(**(code **)(*piVar14 + 0xdc))(auStack_14);
    fStack_34 = pfVar8[1] - pfVar7[1];
    ppiVar15 = &piStack_38;
    piStack_38 = (int *)(*pfVar8 - *pfVar7);
LAB_005be904:
    (**(code **)(**(int **)(in_ECX + 0x194) + 0x80))(ppiVar15,0x3e4ccccd,0);
LAB_005be921:
    *(undefined4 *)(in_ECX + 0x188) = *(undefined4 *)(in_ECX + 0x34);
    return;
  case 0x10:
    (**(code **)(*(int *)(in_ECX - 4) + 0x260))();
    *(undefined4 *)(in_ECX + 0x188) = *(undefined4 *)(in_ECX + 0x34);
    return;
  case 0x11:
    (**(code **)(*(int *)(in_ECX - 4) + 0x25c))();
    *(undefined4 *)(in_ECX + 0x188) = *(undefined4 *)(in_ECX + 0x34);
    return;
  case 0x12:
    piVar14 = *(int **)(*(int *)(in_ECX + 0x19c) + *(int *)(in_ECX + 400) * 4);
    __0CCharString__QAE_PBDJ_Z("UI_SPELL_TEMPLATE",0xffffffff);
    uVar13 = 0;
    ppuVar16 = &puStack_5c;
    _GetInstance_CManager_NUISystem__SIPAV12_XZ(ppuVar16,0);
    uVar13 = _CreateComponent_CManager_NUISystem__QAEPAVCComponent_2_ABVCCharString___N_Z
                       (ppuVar16,uVar13);
    __0__CCountedPointer_VCVillageTaskHorn____QAE_PAVCVillageTaskHorn___Z(uVar13);
    __1CCharString__QAE_XZ();
    (**(code **)(*piStack_40 + 0x244))(*(undefined4 *)(in_ECX + 0x1ac));
    iVar9 = (**(code **)(*piVar14 + 0x23c))(auStack_44,*(undefined4 *)(in_ECX + 0x18c));
    if (iVar9 != -1) {
      pvVar12 = operator_new(0x20);
      if (pvVar12 == (void *)0x0) {
        uVar13 = 0;
      }
      else {
        uVar13 = __0CAction_NUISystem__QAE_XZ();
      }
      __0__CCountedPointer_VCAction_NUISystem____QAE_PAVCAction_NUISystem___Z(uVar13);
      *puStack_5c = 0xd2;
      puVar10 = operator_new(0xc);
      if (puVar10 != (undefined4 *)0x0) {
        iVar1 = *(int *)(in_ECX + 0x1a0);
        iVar2 = *(int *)(in_ECX + 0x19c);
        piVar11 = (int *)(**(code **)(*piVar14 + 0x250))();
        iVar3 = piVar11[1];
        iVar4 = *piVar11;
        iVar5 = *(int *)(in_ECX + 400);
        puVar10[1] = 5;
        *puVar10 = NUISystem::CActionParamULONG::vftable;
        puVar10[2] = (iVar3 - iVar4 >> 3) * (((iVar1 - iVar2 >> 2) - iVar5) + -1) + iVar9;
      }
      _push_back___list_JV__allocator_J_std___std__QAEXABJ_Z(&stack0xffffff9c);
      piVar11 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(*piVar11 + 0x38))(&puStack_5c);
      _ReleaseData___CCountedPointer_VCEventBase_NEntityEvents____QAEXXZ();
    }
    pvVar12 = operator_new(0x20);
    if (pvVar12 == (void *)0x0) {
      uVar13 = 0;
    }
    else {
      uVar13 = __0CAction_NUISystem__QAE_XZ();
    }
    __0__CCountedPointer_VCAction_NUISystem____QAE_PAVCAction_NUISystem___Z(uVar13);
    *piStack_54 = 0xd1;
    puVar10 = operator_new(0xc);
    if (puVar10 != (undefined4 *)0x0) {
      iVar9 = *(int *)(in_ECX + 0x1a0);
      iVar1 = *(int *)(in_ECX + 0x19c);
      piVar14 = (int *)(**(code **)(*piVar14 + 0x250))();
      iVar2 = piVar14[1];
      iVar3 = *piVar14;
      iVar4 = *(int *)(in_ECX + 400);
      iVar5 = *(int *)(in_ECX + 0x18c);
      puVar10[1] = 5;
      *puVar10 = NUISystem::CActionParamULONG::vftable;
      puVar10[2] = (iVar2 - iVar3 >> 3) * (((iVar9 - iVar1 >> 2) - iVar4) + -1) + iVar5;
    }
    _push_back___list_JV__allocator_J_std___std__QAEXABJ_Z(&stack0xffffff9c);
    piVar14 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
    (**(code **)(*piVar14 + 0x38))(&piStack_54);
    *(undefined4 *)(in_ECX + 0x188) = *(undefined4 *)(in_ECX + 0x34);
    _ReleaseData___CCountedPointer_VCEventBase_NEntityEvents____QAEXXZ();
    _ReleaseData___CCountedPointer_VCEventBase_NEntityEvents____QAEXXZ();
    return;
  case 0x13:
    piStack_54 = *(int **)(*(int *)(in_ECX + 0x19c) + *(int *)(in_ECX + 400) * 4);
    (**(code **)(*piStack_54 + 0x240))(*(undefined4 *)(in_ECX + 0x18c));
    pvVar12 = operator_new(0x20);
    if (pvVar12 == (void *)0x0) {
      uVar13 = 0;
    }
    else {
      uVar13 = __0CAction_NUISystem__QAE_XZ();
    }
    __0__CCountedPointer_VCAction_NUISystem____QAE_PAVCAction_NUISystem___Z(uVar13);
    *apuStack_4c[0] = 0xd2;
    puStack_5c = operator_new(0xc);
    if (puStack_5c == (undefined4 *)0x0) {
      puStack_5c = (undefined4 *)0x0;
    }
    else {
      iVar9 = *(int *)(in_ECX + 0x1a0);
      iVar1 = *(int *)(in_ECX + 0x19c);
      piVar14 = (int *)(**(code **)(*piStack_58 + 0x250))();
      iVar2 = piVar14[1];
      iVar3 = *piVar14;
      iVar4 = *(int *)(in_ECX + 400);
      iVar5 = *(int *)(in_ECX + 0x18c);
      puStack_5c[1] = 5;
      *puStack_5c = NUISystem::CActionParamULONG::vftable;
      puStack_5c[2] = (iVar2 - iVar3 >> 3) * (((iVar9 - iVar1 >> 2) - iVar4) + -1) + iVar5;
    }
    _push_back___list_JV__allocator_J_std___std__QAEXABJ_Z(&puStack_5c);
    piVar14 = (int *)_GetInstance_CManager_NUISystem__SIPAV12_XZ();
    (**(code **)(*piVar14 + 0x38))(apuStack_4c);
    *(undefined4 *)(in_ECX + 0x188) = *(undefined4 *)(in_ECX + 0x34);
    _ReleaseData___CCountedPointer_VCEventBase_NEntityEvents____QAEXXZ();
  }
  return;
}


// CALLEES of ?ProcessEvent@CSpellContainerList@NUISystem@@UAEXW4EEvent@2@@Z:

//=== ?ProcessEvent@CDynamicList@NUISystem@@UAEXW4EEvent@2@@Z @ 005ba5d0 (seed 005ba5d0) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CDynamicList_NUISystem__UAEXW4EEvent_2__Z(void)

{
  return;
}


// CALLEES of ?ProcessEvent@CDynamicList@NUISystem@@UAEXW4EEvent@2@@Z:

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
//   005ced60  ?UpdateVisibleList@CScrollableList@NUISystem@@IAEXMM@Z
//   005b5df1  ?pop_front@?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAEXXZ
//   00409310  ?Save@CUserProfileManager@@QAEXXZ
//   00438c20  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
//   005a83f0  ?ProcessEvent@CComponent@NUISystem@@UAEXW4EEvent@2@@Z
//   005cf8f0  ?CanAcceptChanges@CRedefinerList@NUISystem@@SI_NXZ
//   0040bfc0  ?Get@CUserProfileManager@@SIAAV1@XZ

