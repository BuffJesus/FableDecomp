//=== FUN_00c425e0 @ 00c425e0 (seed 0xc425e0) ===

uint FUN_00c425e0(int param_1,undefined4 param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined4 in_ECX;
  uint in_EDX;
  undefined1 local_44 [20];
  CFourierAnalysis local_30 [36];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_012152d8;
  local_c = ExceptionList;
  uVar1 = in_EDX / (uint)(param_1 * 0x24);
  uVar2 = param_1 * 0x80 * uVar1;
  if (uVar2 < param_3 || uVar2 - param_3 == 0) {
    ExceptionList = &local_c;
    CFourierAnalysis::CFourierAnalysis(local_30);
    local_4 = 0;
    FUN_00c5f070(0x40,local_44);
    iVar3 = FUN_00c5f9e0(local_44,0);
    if (iVar3 != 0) {
      iVar3 = FUN_00c5fa50(in_ECX,param_2,uVar1);
      local_4 = 0xffffffff;
      if (iVar3 != 0) {
        _E2();
        ExceptionList = local_c;
        return uVar2;
      }
    }
    local_4 = 0xffffffff;
    _E2();
  }
  ExceptionList = local_c;
  return 0xffffffff;
}


// CALLEES of FUN_00c425e0:
//   00c5efb0  $E2
//   00c5fa50  FUN_00c5fa50
//   00c5f070  FUN_00c5f070
//   00c5f9e0  FUN_00c5f9e0
//   00c5efa0  CFourierAnalysis

//=== FUN_00c5f9e0 @ 00c5f9e0 (seed 0xc5f9e0) ===

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


// CALLEES of FUN_00c5f9e0:
//   00c5f110  FUN_00c5f110

//=== FUN_00c34630 @ 00c34630 (seed 0xc34630) ===

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


// CALLEES of FUN_00c34630:
//   00c425e0  FUN_00c425e0
//   00c2d800  AddSuccessfulBlockParry
//   00c2d820  AddSuccessfulBlockParry

//=== FUN_00c4e1b0 @ 00c4e1b0 (seed 0xc4e1b0) ===

int FUN_00c4e1b0(int *param_1)

{
  int iVar1;
  undefined4 *in_ECX;
  
  if (2 < (uint)param_1[1]) {
    return -0xe;
  }
  if ((*(byte *)(in_ECX + 2) & 1) == 0) {
    iVar1 = FUN_00c4dc40(param_1);
    return iVar1;
  }
  FUN_00c4d960();
  if (*param_1 == 1) {
    iVar1 = (uint)(param_1[1] != 1) * 2 + 0x1101;
  }
  else if (*param_1 == 2) {
    iVar1 = (uint)(param_1[1] != 1) * 2 + 0x1100;
  }
  else {
    iVar1 = 0;
  }
  FUN_00c4d910(*in_ECX,iVar1,param_1[6],param_1[3],param_1[2]);
  iVar1 = FUN_00c4dbe0();
  if ((-1 < iVar1) && ((*(byte *)(in_ECX + 2) & 2) != 0)) {
    iVar1 = FUN_00c4de00();
  }
  return iVar1;
}


// CALLEES of FUN_00c4e1b0:
//   00c4dc40  FUN_00c4dc40
//   00c4d960  FUN_00c4d960
//   00c4dbe0  FUN_00c4dbe0
//   00c4d910  FUN_00c4d910
//   00c4de00  FUN_00c4de00

