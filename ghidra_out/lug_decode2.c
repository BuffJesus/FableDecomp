//=== FUN_00c425e0 @ 00c425e0 (callers:
//     <- 00c3465e FUN_00c34630
// ) ===

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



