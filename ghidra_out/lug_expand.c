//=== FUN_00c5f350 @ 00c5f350 (callers:
//     <- 00c5f73c FUN_00c5f6a0
//     <- 00c5f77d FUN_00c5f6a0
//     <- 00c5f8e6 FUN_00c5f7f0
//     <- 00c5f929 FUN_00c5f7f0
// ) ===

int FUN_00c5f350(int param_1)

{
  int iVar1;
  uint in_ECX;
  int in_EDX;
  
  iVar1 = param_1 >> 3;
  if ((in_ECX & 4) != 0) {
    iVar1 = iVar1 + param_1;
  }
  if ((in_ECX & 2) != 0) {
    iVar1 = iVar1 + (param_1 >> 1);
  }
  if ((in_ECX & 1) != 0) {
    iVar1 = iVar1 + (param_1 >> 2);
  }
  if ((in_ECX & 8) != 0) {
    iVar1 = -iVar1;
  }
  iVar1 = iVar1 + in_EDX;
  if ((short)iVar1 != iVar1) {
    iVar1 = ((-0x8001 < iVar1) - 1 & 0xffff0001) + 0x7fff;
  }
  return iVar1;
}



//=== FUN_00c5f2d0 @ 00c5f2d0 (callers:
//     <- 00c5f424 FUN_00c5f3b0
//     <- 00c5f463 FUN_00c5f3b0
//     <- 00c5f5a3 FUN_00c5f4e0
//     <- 00c5f5ef FUN_00c5f4e0
// ) ===

void FUN_00c5f2d0(int param_1)

{
  int in_ECX;
  int *in_EDX;
  int iVar1;
  bool bVar2;
  
  iVar1 = in_ECX - *in_EDX;
  bVar2 = -1 < iVar1;
  if (!bVar2) {
    iVar1 = -iVar1;
  }
  if (param_1 <= iVar1) {
    iVar1 = iVar1 - param_1;
  }
  if (param_1 >> 1 <= iVar1) {
    iVar1 = iVar1 - (param_1 >> 1);
  }
  if (param_1 >> 2 <= iVar1) {
    iVar1 = iVar1 - (param_1 >> 2);
  }
  if (bVar2) {
    iVar1 = (param_1 >> 3) - iVar1;
  }
  else {
    iVar1 = iVar1 - (param_1 >> 3);
  }
  iVar1 = iVar1 + in_ECX;
  if (0x7fff < iVar1) {
    *in_EDX = 0x7fff;
    return;
  }
  if (iVar1 < -0x8000) {
    *in_EDX = -0x8000;
    return;
  }
  *in_EDX = iVar1;
  return;
}



