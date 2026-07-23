//=== FUN_00c5f000 @ 00c5f000 (callers:
//     <- 00c5f08b FUN_00c5f070
//     <- 00c5f13b FUN_00c5f110
// ) ===

int FUN_00c5f000(void)

{
  int iVar1;
  int in_ECX;
  uint in_EDX;
  
  iVar1 = (in_ECX * 4 & 0xffffU) * ((in_EDX & 0xffff) - 1) + 7;
  iVar1 = ((int)(iVar1 + (iVar1 >> 0x1f & 7U)) >> 3) + 7;
  return (in_ECX + ((int)(iVar1 + (iVar1 >> 0x1f & 7U)) >> 3) * 2) * 4;
}



//=== FUN_00c5f110 @ 00c5f110 (callers:
//     <- 00c5f9ea FUN_00c5f9e0
// ) ===

bool FUN_00c5f110(void)

{
  short sVar1;
  short *in_ECX;
  
  if ((((*in_ECX == 0x69) && (in_ECX[8] == 2)) && (in_ECX[1] != 0)) &&
     (((ushort)in_ECX[1] < 3 && (in_ECX[7] == 4)))) {
    sVar1 = FUN_00c5f000();
    return sVar1 == in_ECX[6];
  }
  return false;
}



