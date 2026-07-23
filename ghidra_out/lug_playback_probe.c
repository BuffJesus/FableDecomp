// vtable 12b4d3c slot0 -> c31230
//=== FUN_00c31230 @ 00c31230 ===

void FUN_00c31230(undefined4 param_1,CCharString *param_2)

{
  CCharString *pCVar1;
  int iVar2;
  int in_ECX;
  
  pCVar1 = param_2;
  param_2 = *(CCharString **)(param_2 + 0x108);
  iVar2 = (**(code **)(**(int **)(in_ECX + 4) + 0x44))(&param_2);
  if (iVar2 == 0) {
    iVar2 = FUN_00c312c0(&param_1);
  }
  CShaderResource::GetPixelShader((CShaderResource *)(iVar2 + 4),pCVar1);
  *(undefined4 *)(iVar2 + 8) = *(undefined4 *)(pCVar1 + 0x10c);
  *(int *)(iVar2 + 0xc) = *(int *)(pCVar1 + 0x110) + *(int *)(in_ECX + 8);
  *(undefined2 *)(iVar2 + 0x10) = *(undefined2 *)(pCVar1 + 0x126);
  *(undefined2 *)(iVar2 + 0x12) = *(undefined2 *)(pCVar1 + 0x124);
  *(undefined4 *)(iVar2 + 0x14) = *(undefined4 *)(pCVar1 + 0x128);
  *(undefined4 *)(iVar2 + 0x18) = *(undefined4 *)(pCVar1 + 0x138);
  *(undefined4 *)(iVar2 + 0x1c) = *(undefined4 *)(pCVar1 + 0x13c);
  return;
}



// vtable 12b4d34 slot0 -> c313c0
//=== no function at 00c313c0 ===
// vtable 12b4d40 slot0 -> c31660
//=== no function at 00c31660 ===
//=== FUN_00c0e9e0 @ 00c0e9e0 ===

void FUN_00c0e9e0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  int in_ECX;
  
  puVar1 = operator_new(0xc);
  puVar1[1] = param_2;
  *puVar1 = param_1;
  puVar1[2] = param_3;
  (**(code **)(*(int *)(in_ECX + 0x60) + 0x24))(puVar1);
  return;
}



