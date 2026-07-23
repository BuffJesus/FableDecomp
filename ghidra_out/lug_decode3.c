//=== FUN_00c5f070 @ 00c5f070 (callers:
//     <- 00c4263f FUN_00c425e0
// ) ===

void FUN_00c5f070(ushort param_1,undefined2 *param_2)

{
  ushort uVar1;
  undefined2 in_CX;
  int in_EDX;
  
  *param_2 = 0x69;
  param_2[1] = in_CX;
  *(int *)(param_2 + 2) = in_EDX;
  uVar1 = FUN_00c5f000();
  param_2[6] = uVar1;
  param_2[9] = param_1;
  param_2[7] = 4;
  param_2[8] = 2;
  *(uint *)(param_2 + 4) = ((uint)uVar1 * in_EDX) / (uint)param_1;
  return;
}



//=== FUN_00c5f9e0 @ 00c5f9e0 (callers:
//     <- 00c4264f FUN_00c425e0
// ) ===

undefined4 FUN_00c5f9e0(undefined4 *param_1,int param_2)

{
  int iVar1;
  undefined4 *in_ECX;
  
  iVar1 = FUN_00c5f110();
  if (iVar1 == 0) {
    return 0;
  }
  *in_ECX = *param_1;
  in_ECX[1] = param_1[1];
  in_ECX[2] = param_1[2];
  in_ECX[3] = param_1[3];
  in_ECX[4] = param_1[4];
  in_ECX[5] = param_2;
  in_ECX[8] = *(undefined4 *)
               (&DAT_012b9270 + ((uint)*(ushort *)((int)in_ECX + 2) + (uint)(param_2 != 0) * 2) * 2)
  ;
  in_ECX[7] = 0;
  in_ECX[6] = 0;
  return 1;
}



//=== FUN_00c5f150 @ 00c5f150 (callers:
//     <- 00c5fef0 FUN_00c5fa50
//     <- 00c5fc52 FUN_00c5fa50
//     <- 00c5fe27 FUN_00c5fa50
// ) ===

longlong FUN_00c5f150(short *param_1,int param_2,int param_3,undefined4 param_4,int param_5)

{
  longlong lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int in_ECX;
  int iVar6;
  short *psVar7;
  short *psVar8;
  longlong lVar9;
  
  psVar8 = param_1;
  lVar1 = 0;
  __aulldiv(0xffffffff,0xffffffff,param_4,0);
  uVar3 = (uint)*(ushort *)(in_ECX + 0x12);
  __allmul(uVar3,0,0,1);
  if (param_3 != 0) {
    param_2 = param_2 - (int)param_1;
    param_1 = (short *)param_3;
    uVar2 = uVar3;
    lVar1 = 0;
    psVar7 = psVar8;
    do {
      for (; uVar2 != 0; uVar2 = uVar2 - 1) {
        uVar4 = (uint)*(short *)(param_2 + (int)psVar8);
        uVar5 = (uint)*psVar8;
        iVar6 = (((int)uVar4 >> 0x1f) - ((int)uVar5 >> 0x1f)) - (uint)(uVar4 < uVar5);
        __allmul(uVar4 - uVar5,iVar6,uVar4 - uVar5,iVar6);
        psVar8 = psVar8 + 1;
      }
      lVar9 = __ftol2();
      lVar1 = lVar9 + lVar1;
      psVar8 = (short *)((int)psVar7 + param_5);
      param_1 = (short *)((int)param_1 + -1);
      uVar2 = uVar3;
      psVar7 = psVar8;
    } while (param_1 != (short *)0x0);
  }
  return lVar1;
}



//=== FUN_00c34630 @ 00c34630 (callers:
//     <- 00c261a4 FUN_00c26090
//     <- 00c1e44c FUN_00c1e300
// ) ===

undefined4 FUN_00c34630(undefined4 param_1,uint param_2)

{
  int extraout_EAX;
  int iVar1;
  undefined4 extraout_EAX_00;
  undefined4 extraout_EAX_01;
  CTCHeroExperience *in_ECX;
  
  if (*(short *)(in_ECX + 0xe) == 1) {
    CTCHeroExperience::AddSuccessfulBlockParry(in_ECX);
    if ((uint)(extraout_EAX << 1) <= param_2) {
      iVar1 = FUN_00c425e0(*(undefined2 *)(in_ECX + 0xc),param_1,extraout_EAX << 1);
      if (0 < iVar1) {
        *(undefined2 *)(in_ECX + 0xc) = *(undefined2 *)(in_ECX + 0xc);
        *(undefined2 *)(in_ECX + 0xe) = 0;
        *(int *)in_ECX = iVar1;
        CTCHeroExperience::AddSuccessfulBlockParry(in_ECX);
        CTCHeroExperience::AddSuccessfulBlockParry(in_ECX);
        *(undefined4 *)(in_ECX + 4) = extraout_EAX_00;
        CTCHeroExperience::AddSuccessfulBlockParry(in_ECX);
        CTCHeroExperience::AddSuccessfulBlockParry(in_ECX);
        *(undefined4 *)(in_ECX + 8) = extraout_EAX_01;
        *(undefined4 *)(in_ECX + 0x10) = *(undefined4 *)(in_ECX + 0x10);
        return 0;
      }
    }
  }
  return 0xffffffff;
}



