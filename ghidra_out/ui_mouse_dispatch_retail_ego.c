//=== ?ProcessEvent@CClickable@NUISystem@@UAEXW4EEvent@2@@Z @ 005d41b0 (seed 005d41b0) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CClickable_NUISystem__UAEXW4EEvent_2__Z(undefined4 param_1)

{
  float fVar1;
  int in_ECX;
  
  switch(param_1) {
  case 0x1a:
    if (*(char *)(in_ECX + 0x18c) != '\0') {
      (**(code **)(*(int *)(in_ECX + -4) + 0x248))();
      *(undefined1 *)(in_ECX + 0x19c) = 1;
    }
    break;
  case 0x1b:
    if (*(char *)(in_ECX + 0x18c) != '\0') {
      (**(code **)(*(int *)(in_ECX + -4) + 0x250))();
      *(undefined1 *)(in_ECX + 0x1b0) = 1;
    }
    break;
  case 0x1c:
    if (*(char *)(in_ECX + 0x19c) != '\0') {
      (**(code **)(*(int *)(in_ECX + -4) + 0x24c))();
      *(undefined1 *)(in_ECX + 0x19c) = 0;
    }
    break;
  case 0x1d:
    if (*(char *)(in_ECX + 0x1b0) != '\0') {
      (**(code **)(*(int *)(in_ECX + -4) + 0x254))();
      *(undefined1 *)(in_ECX + 0x1b0) = 0;
    }
    break;
  default:
    goto switchD_005d41c4_caseD_1e;
  case 0x1f:
    fVar1 = *(float *)(in_ECX + 0x34) - *(float *)(in_ECX + 0x1bc);
    if (*(float *)(in_ECX + 0x1b8) < fVar1 == (*(float *)(in_ECX + 0x1b8) == fVar1))
    goto switchD_005d41c4_caseD_1e;
    if (*(char *)(in_ECX + 0x19c) != '\0') {
      (**(code **)(*(int *)(in_ECX + -4) + 0x20c))(*(undefined4 *)(in_ECX + 0x1a4));
    }
    break;
  case 0x20:
    if (*(char *)(in_ECX + 0x1b0) != '\0') {
      (**(code **)(*(int *)(in_ECX + -4) + 0x20c))(*(undefined4 *)(in_ECX + 0x1b4));
    }
  }
  *(undefined4 *)(in_ECX + 0x1bc) = *(undefined4 *)(in_ECX + 0x34);
switchD_005d41c4_caseD_1e:
  _ProcessEvent_CHoverable_NUISystem__UAEXW4EEvent_2__Z(param_1);
  return;
}


// CALLEES of ?ProcessEvent@CClickable@NUISystem@@UAEXW4EEvent@2@@Z:
//   005d5070  ?ProcessEvent@CHoverable@NUISystem@@UAEXW4EEvent@2@@Z

//=== ?OnLeftClicked@CClickable@NUISystem@@UAEXXZ @ 005d43b0 (seed 005d43b0) ===

/* WARNING: Restarted to delay deadcode elimination for space: stack */
/* [ported from ego_r via strfp] */

void _OnLeftClicked_CClickable_NUISystem__UAEXXZ(void)

{
  code *pcVar1;
  int *piVar2;
  int *in_ECX;
  int unaff_retaddr;
  
  pcVar1 = *(code **)(*in_ECX + 0x1b0);
  in_ECX[0x67] = in_ECX[0x5e];
  piVar2 = (int *)(*pcVar1)();
  (**(code **)(*in_ECX + 0xc0))(*(undefined4 *)(*piVar2 + 0x23c));
  if ((&stack0x00000000 != &DAT_00000004) && (unaff_retaddr == 1)) {
    (*(code *)in_ECX[1])();
  }
  (**(code **)(*in_ECX + 0x20c))(in_ECX[0x69]);
  (**(code **)(in_ECX[1] + 0xc))(0x1c);
  return;
}


// CALLEES of ?OnLeftClicked@CClickable@NUISystem@@UAEXXZ:

//=== ?OnLeftUnclicked@CClickable@NUISystem@@UAEXXZ @ 005d4140 (seed 005d4140) ===

/* [ported from ego_r via strfp] */

void _OnLeftUnclicked_CClickable_NUISystem__UAEXXZ(void)

{
  int *in_ECX;
  
  (**(code **)(*in_ECX + 0xc0))(in_ECX[0x67]);
  (**(code **)(in_ECX[1] + 0x10))(0x1c);
  (**(code **)(*in_ECX + 0x20c))(in_ECX[0x6b]);
  return;
}


// CALLEES of ?OnLeftUnclicked@CClickable@NUISystem@@UAEXXZ:

//=== ?ProcessEvent@CHoverable@NUISystem@@UAEXW4EEvent@2@@Z @ 005d5070 (seed 005d5070) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CHoverable_NUISystem__UAEXW4EEvent_2__Z(int param_1)

{
  int in_ECX;
  
  if (param_1 == 0x19) {
    (**(code **)(*(int *)(in_ECX + -4) + 0x244))();
  }
  return;
}


// CALLEES of ?ProcessEvent@CHoverable@NUISystem@@UAEXW4EEvent@2@@Z:

//=== ?UpdateHovering@CHoverable@NUISystem@@MAEXXZ @ 005d5660 (seed 005d5660) ===

/* [ported from ego_r via strfp] */

void _UpdateHovering_CHoverable_NUISystem__MAEXXZ(void)

{
  int *piVar1;
  code *pcVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int *in_ECX;
  int *piVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 auStack_2c [12];
  int *piStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined1 auStack_14 [12];
  undefined4 uStack_8;
  
  iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
  if ((*(char *)(iVar4 + 0xd0) == '\0') && (cVar3 = (**(code **)(in_ECX[1] + 8))(), cVar3 != '\0'))
  {
    iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
    if (*(int *)(iVar4 + 0xe8) != 0) {
      iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(**(int **)(iVar4 + 0xe8) + 0x40))();
      uStack_18 = __ftol2_sse();
      iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(**(int **)(iVar4 + 0xe8) + 0x40))();
      uStack_18 = __ftol2_sse();
      iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      piVar6 = *(int **)(iVar4 + 0xe8);
      iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      (**(code **)(**(int **)(iVar4 + 0xe8) + 0x5c))(&stack0xffffffd0);
      puVar8 = auStack_2c;
      (**(code **)(*piVar6 + 0x40))(puVar8);
      uStack_1c = __ftol2_sse();
      iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      piVar6 = *(int **)(iVar4 + 0xe8);
      iVar4 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
      puVar7 = &stack0xffffffd0;
      iVar4 = (**(code **)(**(int **)(iVar4 + 0xe8) + 0x5c))(puVar7);
      (**(code **)(*piVar6 + 0x40))(&stack0xffffffc4,puVar7,puVar8,(double)*(float *)(iVar4 + 4));
      uStack_8 = __ftol2_sse();
    }
    if ((char)in_ECX[100] == '\0') {
      cVar3 = (**(code **)(*in_ECX + 0x238))();
      if ((cVar3 != '\0') &&
         (cVar3 = _GIsAnyAlreadyHovered_CHoverable_NUISystem__KI_NAAVC2DBoxI__PAVCComponent_2__Z(),
         cVar3 == '\0')) {
        __0__list_PAVCHoverable_NUISystem__V__allocator_PAVCHoverable_NUISystem___std___std__QAE_ABV01__Z
                  ();
        piVar6 = (int *)*piStack_20;
        if (piVar6 != piStack_20) {
          do {
            piVar1 = (int *)piVar6[2];
            cVar3 = (**(code **)(*in_ECX + 0x210))();
            if (((cVar3 != '\0') && (cVar3 = (**(code **)(*piVar1 + 0x210))(), cVar3 != '\0')) &&
               (cVar3 = (**(code **)(*piVar1 + 0x238))(auStack_14), cVar3 != '\0')) {
              iVar4 = (**(code **)(*piVar1 + 0x204))();
              iVar5 = (**(code **)(*in_ECX + 0x204))();
              if (iVar5 < iVar4) {
                (**(code **)(*piVar1 + 0x240))();
                pcVar2 = *(code **)(*piVar1 + 0x214);
                *(undefined1 *)(piVar1 + 100) = 0;
                (*pcVar2)();
              }
            }
            piVar6 = (int *)*piVar6;
          } while (piVar6 != piStack_20);
        }
        (**(code **)(*in_ECX + 0x23c))();
        _SetWasHovered_CHoverable_NUISystem__IAEX_N_Z();
        __1__list_PAVCHoverable_NUISystem__V__allocator_PAVCHoverable_NUISystem___std___std__QAE_XZ
                  ();
        return;
      }
      if ((char)in_ECX[100] == '\0') {
        return;
      }
    }
    cVar3 = (**(code **)(*in_ECX + 0x238))();
    if (cVar3 == '\0') {
      (**(code **)(*in_ECX + 0x240))();
      pcVar2 = *(code **)(*in_ECX + 0x214);
      *(undefined1 *)(in_ECX + 100) = 0;
      (*pcVar2)();
    }
  }
  return;
}


// CALLEES of ?UpdateHovering@CHoverable@NUISystem@@MAEXXZ:
//   005d51f0  ?GIsAnyAlreadyHovered@CHoverable@NUISystem@@KI_NAAVC2DBoxI@@PAVCComponent@2@@Z
//   00b14fb0  ??0?$list@PAVCHoverable@NUISystem@@V?$allocator@PAVCHoverable@NUISystem@@@std@@@std@@QAE@ABV01@@Z
//   00c96090  __ftol2_sse
//   00438c20  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
//   005d53e0  ?SetWasHovered@CHoverable@NUISystem@@IAEX_N@Z
//   005941f0  ??1?$list@PAVCHoverable@NUISystem@@V?$allocator@PAVCHoverable@NUISystem@@@std@@@std@@QAE@XZ

//=== ?TurnToPointer@CMouseCursor@NUISystem@@UAEXXZ @ 005d59b0 (seed 005d59b0) ===

/* [ported from ego_r via strfp] */

void _TurnToPointer_CMouseCursor_NUISystem__UAEXXZ(void)

{
  int iVar1;
  undefined4 uVar2;
  int *in_ECX;
  
  iVar1 = *in_ECX;
  uVar2 = _GetDefaultGraphicIndex_CSprite_NUISystem__QAEKXZ();
  (**(code **)(iVar1 + 0x244))(uVar2);
  return;
}


// CALLEES of ?TurnToPointer@CMouseCursor@NUISystem@@UAEXXZ:
//   0042c230  ?GetDefaultGraphicIndex@CSprite@NUISystem@@QAEKXZ

//=== ?ProcessEvent@CMouseCursor@NUISystem@@UAEXW4EEvent@2@@Z @ 005d5ae0 (seed 005d5ae0) ===

/* [ported from ego_r via strfp] */

void _ProcessEvent_CMouseCursor_NUISystem__UAEXW4EEvent_2__Z(int param_1)

{
  int iVar1;
  int *piVar2;
  int in_ECX;
  undefined4 unaff_ESI;
  float fVar3;
  float fVar4;
  undefined8 uVar5;
  double dVar6;
  undefined4 local_2c [2];
  undefined1 auStack_24 [8];
  undefined1 auStack_1c [8];
  undefined1 auStack_14 [20];
  
  if (param_1 == 0x19) {
    uVar5 = CONCAT44(*(undefined4 *)(in_ECX + 0x38),unaff_ESI);
    local_2c[0] = *(undefined4 *)(in_ECX + 0x3c);
    iVar1 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
    fVar4 = 0.0;
    fVar3 = 0.0;
    (**(code **)(*(int *)(in_ECX + -4) + 0x80))(iVar1 + 0xe0,0,0,uVar5);
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    dVar6 = (double)*(float *)(in_ECX + 0x38);
    piVar2 = (int *)_GetRenderTargetDimensions_CDisplayManager__QAE_BVC2DExtentsI__XZ(local_2c);
    if ((double)*piVar2 < dVar6 != ((double)*piVar2 == dVar6)) {
      _GFGetSystemManager__YIPAVCSystemManager__XZ();
      piVar2 = (int *)_GetRenderTargetDimensions_CDisplayManager__QAE_BVC2DExtentsI__XZ(auStack_24);
      *(float *)(in_ECX + 0x38) = (float)*piVar2;
    }
    if (*(float *)(in_ECX + 0x38) < 0.0) {
      *(undefined4 *)(in_ECX + 0x38) = 0;
    }
    _GFGetSystemManager__YIPAVCSystemManager__XZ();
    dVar6 = (double)*(float *)(in_ECX + 0x3c);
    iVar1 = _GetRenderTargetDimensions_CDisplayManager__QAE_BVC2DExtentsI__XZ(auStack_1c);
    if ((double)*(int *)(iVar1 + 4) < dVar6 != ((double)*(int *)(iVar1 + 4) == dVar6)) {
      _GFGetSystemManager__YIPAVCSystemManager__XZ();
      iVar1 = _GetRenderTargetDimensions_CDisplayManager__QAE_BVC2DExtentsI__XZ(auStack_14);
      *(float *)(in_ECX + 0x3c) = (float)*(int *)(iVar1 + 4);
    }
    if (*(float *)(in_ECX + 0x3c) < 0.0) {
      *(undefined4 *)(in_ECX + 0x3c) = 0;
    }
    fVar3 = *(float *)(in_ECX + 0x38) - fVar3;
    fVar4 = *(float *)(in_ECX + 0x3c) - fVar4;
    iVar1 = _GetInstance_CManager_NUISystem__SIPAV12_XZ();
    *(float *)(iVar1 + 0xe0) = fVar3;
    *(float *)(iVar1 + 0xe4) = fVar4;
  }
  return;
}


// CALLEES of ?ProcessEvent@CMouseCursor@NUISystem@@UAEXW4EEvent@2@@Z:
//   00af3080  ?GFGetSystemManager@@YIPAVCSystemManager@@XZ
//   00438c20  ?GetInstance@CManager@NUISystem@@SIPAV12@XZ
//   00b242a0  ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ

//=== ?OnLeftClicked@CFrontEndButton@NUISystem@@MAEXXZ @ 005c7d10 (seed 005c7d10) ===

/* [ported from ego_r via strfp] */

void _OnLeftClicked_CFrontEndButton_NUISystem__MAEXXZ(void)

{
  char cVar1;
  int *piVar2;
  int *in_ECX;
  int *unaff_EBX;
  undefined1 local_4 [4];
  
  piVar2 = (int *)(**(code **)(*in_ECX + 0x1b0))(local_4);
  cVar1 = *(char *)(*piVar2 + 0x255);
  if ((unaff_EBX != (int *)0x0) && (unaff_EBX[1] = unaff_EBX[1] + -1, unaff_EBX[1] == 0)) {
    (**(code **)(*unaff_EBX + 4))();
  }
  if (cVar1 != '\0') {
    (**(code **)(*in_ECX + 0x20c))(in_ECX[0x69]);
  }
  return;
}


// CALLEES of ?OnLeftClicked@CFrontEndButton@NUISystem@@MAEXXZ:

//=== ?OnLeftUnclicked@CFrontEndButton@NUISystem@@MAEXXZ @ 005c7d70 (seed 005c7d70) ===

/* [ported from ego_r via strfp] */

void _OnLeftUnclicked_CFrontEndButton_NUISystem__MAEXXZ(void)

{
  char cVar1;
  int *piVar2;
  int *in_ECX;
  int *unaff_EBX;
  undefined1 local_4 [4];
  
  piVar2 = (int *)(**(code **)(*in_ECX + 0x1b0))(local_4);
  cVar1 = *(char *)(*piVar2 + 0x255);
  if ((unaff_EBX != (int *)0x0) && (unaff_EBX[1] = unaff_EBX[1] + -1, unaff_EBX[1] == 0)) {
    (**(code **)(*unaff_EBX + 4))();
  }
  if (cVar1 != '\0') {
    (**(code **)(*in_ECX + 0x20c))(in_ECX[0x6b]);
  }
  return;
}


// CALLEES of ?OnLeftUnclicked@CFrontEndButton@NUISystem@@MAEXXZ:

