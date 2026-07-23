//=== FUN_00c5f000 @ 00c5f000 (seed 0xc5f000) ===

int FUN_00c5f000(void)

{
  int iVar1;
  int in_ECX;
  uint in_EDX;
  
  iVar1 = (in_ECX * 4 & 0xffffU) * ((in_EDX & 0xffff) - 1) + 7;
  iVar1 = ((int)(iVar1 + (iVar1 >> 0x1f & 7U)) >> 3) + 7;
  return (in_ECX + ((int)(iVar1 + (iVar1 >> 0x1f & 7U)) >> 3) * 2) * 4;
}


// CALLEES of FUN_00c5f000:

//=== FUN_00c4dbe0 @ 00c4dbe0 (seed 0xc4dbe0) ===

int FUN_00c4dbe0(void)

{
  FARPROC pFVar1;
  int iVar2;
  int in_ECX;
  
  if (*(int *)(in_ECX + 0xe0) == 0) {
    if (*(char *)(in_ECX + 0x104) == '\0') {
      pFVar1 = GetProcAddress(*(HMODULE *)(in_ECX + 4),"alGetError");
    }
    else {
      pFVar1 = (FARPROC)FUN_00c4d740("alGetError");
    }
    *(FARPROC *)(in_ECX + 0xe0) = pFVar1;
  }
  iVar2 = (**(code **)(in_ECX + 0xe0))();
  if (iVar2 != 0) {
    return (uint)(iVar2 == 0xa005) * 4 + -9;
  }
  return 0;
}


// CALLEES of FUN_00c4dbe0:
//   00c4d740  FUN_00c4d740
//   EXTERNAL:000000c2  GetProcAddress

//=== FUN_00c5f350 @ 00c5f350 (seed 0xc5f350) ===

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


// CALLEES of FUN_00c5f350:

