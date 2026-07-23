//=== FUN_00c4de00 @ 00c4de00 (seed 0xc4de00) ===

CTCHeroStats * FUN_00c4de00(void)

{
  CBase CVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  char cVar5;
  CBase *extraout_EAX;
  uint uVar6;
  int iVar7;
  int in_ECX;
  CTCHeroStats *pCVar8;
  CTCHeroStats *this;
  CTCHeroStats *extraout_ECX;
  code *pcVar9;
  CTCHeroStats *pCVar10;
  undefined4 local_8;
  
  pCVar8 = (CTCHeroStats *)(*(int *)(*(int *)(in_ECX + 0xc) + 0xc) << 1);
  CTCHeroStats::InformOfEvent(pCVar8,(CBase *)pCVar8);
  if (extraout_EAX == (CBase *)0x0) {
    return (CTCHeroStats *)0xfffffffb;
  }
  pCVar8 = *(CTCHeroStats **)(in_ECX + 0xc);
  if (*(int *)pCVar8 == 1) {
    iVar7 = *(int *)(pCVar8 + 0x18);
    uVar6 = 0;
    if ((*(uint *)(pCVar8 + 0xc) & 0xfffffffe) != 0) {
      do {
        uVar2 = *(undefined2 *)(iVar7 + uVar6 * 2);
        *(undefined2 *)(extraout_EAX + uVar6 * 4) = uVar2;
        *(undefined2 *)(extraout_EAX + uVar6 * 4 + 2) = uVar2;
        uVar6 = uVar6 + 1;
      } while (uVar6 < *(uint *)(*(int *)(in_ECX + 0xc) + 0xc) >> 1);
    }
    local_8 = 0x1103;
  }
  else {
    if (*(int *)pCVar8 != 2) {
      CTCHeroStats::InformOfEvent(pCVar8,extraout_EAX);
      return (CTCHeroStats *)0xfffffff2;
    }
    iVar7 = *(int *)(pCVar8 + 0x18);
    uVar6 = 0;
    if (*(int *)(pCVar8 + 0xc) != 0) {
      do {
        CVar1 = *(CBase *)(uVar6 + iVar7);
        extraout_EAX[uVar6 * 2] = CVar1;
        extraout_EAX[uVar6 * 2 + 1] = CVar1;
        uVar6 = uVar6 + 1;
      } while (uVar6 < *(uint *)(*(int *)(in_ECX + 0xc) + 0xc));
    }
    local_8 = 0x1102;
  }
  pcVar9 = GetProcAddress_exref;
  if (DAT_0143be90 == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143be90 = GetProcAddress(DAT_0143bdb4,"alGetError");
    }
    else {
      DAT_0143be90 = (FARPROC)FUN_00c4d740("alGetError");
      pcVar9 = GetProcAddress_exref;
    }
  }
  (*DAT_0143be90)();
  if ((*(byte *)(in_ECX + 8) & 2) == 0) {
    if (DAT_0143be90 == (FARPROC)0x0) {
      if (DAT_0143beb4 == '\0') {
        DAT_0143be90 = (FARPROC)(*pcVar9)(DAT_0143bdb4,"alGetError");
      }
      else {
        DAT_0143be90 = (FARPROC)FUN_00c4d740("alGetError");
      }
    }
    (*DAT_0143be90)();
    cVar5 = DAT_0143bf60;
    if (DAT_0143bdcc == (code *)0x0) {
      DAT_0143bdcc = (code *)(*pcVar9)(DAT_0143bdb4,"alcSuspendContext");
    }
    (*DAT_0143bdcc)(DAT_0143beb0);
    DAT_0143bf60 = '\x01';
    if (DAT_0143bddc == (code *)0x0) {
      if (DAT_0143beb4 == '\0') {
        DAT_0143bddc = (code *)(*pcVar9)(DAT_0143bdb4,"alGenBuffers");
      }
      else {
        DAT_0143bddc = (code *)FUN_00c4d740("alGenBuffers");
      }
    }
    (*DAT_0143bddc)(1,in_ECX + 4);
    if (cVar5 == '\0') {
      if (DAT_0143bdd0 == (code *)0x0) {
        DAT_0143bdd0 = (code *)(*pcVar9)(DAT_0143bdb4,"alcProcessContext");
      }
      (*DAT_0143bdd0)(DAT_0143beb0);
      DAT_0143bf60 = '\0';
    }
    pCVar8 = (CTCHeroStats *)FUN_00c4dbe0();
    if ((int)pCVar8 < 0) {
      CTCHeroStats::InformOfEvent(this,extraout_EAX);
      return pCVar8;
    }
    *(uint *)(in_ECX + 8) = *(uint *)(in_ECX + 8) | 2;
  }
  iVar7 = *(int *)(*(int *)(in_ECX + 0xc) + 0xc);
  uVar3 = *(undefined4 *)(*(int *)(in_ECX + 0xc) + 8);
  uVar4 = *(undefined4 *)(in_ECX + 4);
  if (DAT_0143bde8 == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143bde8 = GetProcAddress(DAT_0143bdb4,"alBufferData");
    }
    else {
      DAT_0143bde8 = (FARPROC)FUN_00c4d740("alBufferData");
    }
  }
  (*DAT_0143bde8)(uVar4,local_8,extraout_EAX,iVar7 << 1,uVar3);
  pcVar9 = GetProcAddress_exref;
  if (DAT_0143be90 == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143be90 = GetProcAddress(DAT_0143bdb4,"alGetError");
    }
    else {
      DAT_0143be90 = (FARPROC)FUN_00c4d740("alGetError");
      pcVar9 = GetProcAddress_exref;
    }
  }
  iVar7 = (*DAT_0143be90)();
  if (iVar7 == 0) {
    pCVar10 = (CTCHeroStats *)0x0;
    pCVar8 = extraout_ECX;
  }
  else {
    pCVar8 = (CTCHeroStats *)((uint)(iVar7 == 0xa005) * 4 + -9);
    pCVar10 = pCVar8;
  }
  CTCHeroStats::InformOfEvent(pCVar8,extraout_EAX);
  cVar5 = DAT_0143bf60;
  if (-1 < (int)pCVar10) {
    return (CTCHeroStats *)0x0;
  }
  if (DAT_0143bdcc == (code *)0x0) {
    DAT_0143bdcc = (code *)(*pcVar9)(DAT_0143bdb4,"alcSuspendContext");
  }
  (*DAT_0143bdcc)(DAT_0143beb0);
  DAT_0143bf60 = '\x01';
  if (DAT_0143bde0 == (code *)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143bde0 = (code *)(*pcVar9)(DAT_0143bdb4,"alDeleteBuffers");
    }
    else {
      DAT_0143bde0 = (code *)FUN_00c4d740("alDeleteBuffers");
    }
  }
  (*DAT_0143bde0)(1,(undefined4 *)(in_ECX + 4));
  if (cVar5 == '\0') {
    if (DAT_0143bdd0 == (code *)0x0) {
      DAT_0143bdd0 = (code *)(*pcVar9)(DAT_0143bdb4,"alcProcessContext");
    }
    (*DAT_0143bdd0)(DAT_0143beb0);
    DAT_0143bf60 = '\0';
  }
  *(uint *)(in_ECX + 8) = *(uint *)(in_ECX + 8) & 0xfffffffd;
  *(undefined4 *)(in_ECX + 4) = 0xc0daf00d;
  return pCVar10;
}


// CALLEES of FUN_00c4de00:
//   00c4d740  FUN_00c4d740
//   00c3c370  InformOfEvent
//   EXTERNAL:000000c2  GetProcAddress
//   00c3c360  InformOfEvent
//   00c4dbe0  FUN_00c4dbe0

//=== FUN_00c5f070 @ 00c5f070 (seed 0xc5f070) ===

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


// CALLEES of FUN_00c5f070:
//   00c5f000  FUN_00c5f000

//=== FUN_00c5f6a0 @ 00c5f6a0 (seed 0xc5f6a0) ===

undefined4 FUN_00c5f6a0(int param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined2 uVar2;
  uint *in_ECX;
  int iVar3;
  undefined2 *in_EDX;
  uint *puVar4;
  undefined2 *puVar5;
  uint uVar6;
  
  if (param_1 != 0) {
    do {
      param_1 = param_1 + -1;
      uVar6 = *in_ECX >> 0x10 & 0xff;
      puVar4 = in_ECX + 1;
      if (0x58 < uVar6) {
        return 0;
      }
      *in_EDX = (short)*in_ECX;
      puVar5 = in_EDX + 1;
      for (iVar3 = param_3 + -1; in_EDX = puVar5, iVar3 != 0; iVar3 = iVar3 + -2) {
        uVar1 = *puVar4;
        puVar4 = (uint *)((int)puVar4 + 1);
        uVar2 = FUN_00c5f350((int)(short)(&DAT_012b91c0)[uVar6]);
        uVar6 = (int)*(short *)(&DAT_012b91a0 + ((byte)uVar1 & 0xf) * 2) + uVar6;
        if ((int)uVar6 < 0) {
          uVar6 = 0;
        }
        else if (0x58 < uVar6) {
          uVar6 = 0x58;
        }
        *puVar5 = uVar2;
        in_EDX = puVar5 + 1;
        if (iVar3 == 1) break;
        uVar2 = FUN_00c5f350((int)(short)(&DAT_012b91c0)[uVar6]);
        uVar6 = (int)*(short *)(&DAT_012b91a0 + (uint)(byte)((byte)uVar1 >> 4) * 2) + uVar6;
        if ((int)uVar6 < 0) {
          uVar6 = 0;
        }
        else if (0x58 < uVar6) {
          uVar6 = 0x58;
        }
        *in_EDX = uVar2;
        puVar5 = puVar5 + 2;
      }
      in_ECX = (uint *)((int)in_ECX + param_2);
    } while (param_1 != 0);
  }
  return 1;
}


// CALLEES of FUN_00c5f6a0:
//   00c5f350  FUN_00c5f350

//=== FUN_00c5f7f0 @ 00c5f7f0 (seed 0xc5f7f0) ===

undefined4 FUN_00c5f7f0(int param_1,int param_2,int param_3)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  uint *in_ECX;
  uint *in_EDX;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint *local_24;
  uint local_20;
  uint local_1c;
  uint local_18;
  uint local_14;
  
  if (param_1 != 0) {
    local_24 = in_EDX;
    do {
      uVar4 = *in_ECX >> 0x10 & 0xff;
      param_1 = param_1 + -1;
      if (0x58 < uVar4) {
        return 0;
      }
      uVar6 = in_ECX[1] >> 0x10 & 0xff;
      if (0x58 < uVar6) {
        return 0;
      }
      *local_24 = in_ECX[1] << 0x10 | *in_ECX & 0xffff;
      local_24 = local_24 + 1;
      puVar1 = in_ECX;
      for (uVar5 = param_3 - 1; uVar5 != 0; uVar5 = uVar5 - local_18) {
        local_20 = puVar1[2];
        local_1c = puVar1[3];
        if (uVar5 < 8) {
          local_18 = uVar5;
          if (uVar5 != 0) goto LAB_00c5f8bb;
        }
        else {
          local_18 = 8;
LAB_00c5f8bb:
          local_14 = local_18;
          do {
            uVar2 = FUN_00c5f350((int)(short)(&DAT_012b91c0)[uVar4]);
            uVar4 = (int)*(short *)(&DAT_012b91a0 + (local_20 & 0xf) * 2) + uVar4;
            if ((int)uVar4 < 0) {
              uVar4 = 0;
            }
            else if (0x58 < uVar4) {
              uVar4 = 0x58;
            }
            iVar3 = FUN_00c5f350((int)(short)(&DAT_012b91c0)[uVar6]);
            uVar6 = (int)*(short *)(&DAT_012b91a0 + (local_1c & 0xf) * 2) + uVar6;
            if ((int)uVar6 < 0) {
              uVar6 = 0;
            }
            else if (0x58 < uVar6) {
              uVar6 = 0x58;
            }
            *local_24 = iVar3 << 0x10 | uVar2 & 0xffff;
            local_24 = local_24 + 1;
            local_20 = local_20 >> 4;
            local_1c = local_1c >> 4;
            local_14 = local_14 - 1;
          } while (local_14 != 0);
        }
        puVar1 = puVar1 + 2;
      }
      in_ECX = (uint *)((int)in_ECX + param_2);
    } while (param_1 != 0);
  }
  return 1;
}


// CALLEES of FUN_00c5f7f0:
//   00c5f350  FUN_00c5f350

//=== FUN_00c5f3b0 @ 00c5f3b0 (seed 0xc5f3b0) ===

undefined4 FUN_00c5f3b0(int param_1,int param_2,int param_3,uint *param_4)

{
  int iVar1;
  int iVar2;
  ushort *in_ECX;
  uint *puVar3;
  uint *in_EDX;
  int iVar4;
  uint uVar5;
  ushort *puVar6;
  int local_c;
  
  uVar5 = *param_4;
  if (param_1 != 0) {
    local_c = param_1;
    do {
      *in_EDX = uVar5 << 0x10 | (uint)*in_ECX;
      puVar3 = in_EDX + 1;
      iVar4 = param_3 + -1;
      in_ECX = in_ECX + 1;
      while (iVar4 != 0) {
        puVar6 = in_ECX + 1;
        iVar1 = FUN_00c5f2d0((int)(short)(&DAT_012b91c0)[uVar5]);
        uVar5 = (int)*(short *)(&DAT_012b91a0 + iVar1 * 2) + uVar5;
        if ((int)uVar5 < 0) {
          uVar5 = 0;
        }
        else if (0x58 < uVar5) {
          uVar5 = 0x58;
        }
        if (iVar4 == 1) {
          iVar2 = 0;
          iVar4 = 0;
        }
        else {
          puVar6 = in_ECX + 2;
          iVar4 = iVar4 + -2;
          iVar2 = FUN_00c5f2d0((int)(short)(&DAT_012b91c0)[uVar5]);
          uVar5 = (int)*(short *)(&DAT_012b91a0 + iVar2 * 2) + uVar5;
          if ((int)uVar5 < 0) {
            uVar5 = 0;
          }
          else if (0x58 < uVar5) {
            uVar5 = 0x58;
          }
        }
        *(byte *)puVar3 = (char)iVar2 << 4 | (byte)iVar1;
        puVar3 = (uint *)((int)puVar3 + 1);
        in_ECX = puVar6;
      }
      in_EDX = (uint *)((int)in_EDX + param_2);
      local_c = local_c + -1;
    } while (local_c != 0);
  }
  *param_4 = uVar5;
  return 1;
}


// CALLEES of FUN_00c5f3b0:
//   00c5f2d0  FUN_00c5f2d0

//=== FUN_00c5f4e0 @ 00c5f4e0 (seed 0xc5f4e0) ===

undefined4 FUN_00c5f4e0(uint param_1,int param_2,uint param_3,uint *param_4,uint *param_5)

{
  ushort *puVar1;
  byte bVar2;
  uint *puVar3;
  int iVar4;
  uint uVar5;
  ushort *in_ECX;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint *in_EDX;
  byte bVar9;
  uint local_24;
  uint local_20;
  int local_10;
  
  uVar7 = *param_4;
  uVar8 = *param_5;
  if (param_1 != 0) {
    uVar6 = param_3 - 1;
    local_10 = param_1;
    do {
      *in_EDX = uVar7 << 0x10 | (uint)*in_ECX;
      puVar1 = in_ECX + 1;
      in_ECX = in_ECX + 2;
      in_EDX[1] = uVar8 << 0x10 | (uint)*puVar1;
      puVar3 = in_EDX;
      for (uVar5 = uVar6; uVar5 != 0; uVar5 = uVar5 - local_24) {
        param_1 = 0;
        param_3 = 0;
        if (uVar5 < 8) {
          local_24 = uVar5;
          if (uVar5 != 0) goto LAB_00c5f581;
        }
        else {
          local_24 = 8;
LAB_00c5f581:
          bVar9 = 0;
          local_20 = local_24;
          do {
            iVar4 = FUN_00c5f2d0((int)(short)(&DAT_012b91c0)[uVar7]);
            uVar7 = (int)*(short *)(&DAT_012b91a0 + iVar4 * 2) + uVar7;
            if ((int)uVar7 < 0) {
              uVar7 = 0;
            }
            else if (0x58 < uVar7) {
              uVar7 = 0x58;
            }
            param_1 = param_1 | iVar4 << (bVar9 & 0x1f);
            in_ECX = in_ECX + 2;
            iVar4 = FUN_00c5f2d0((int)(short)(&DAT_012b91c0)[uVar8]);
            uVar8 = (int)*(short *)(&DAT_012b91a0 + iVar4 * 2) + uVar8;
            if ((int)uVar8 < 0) {
              uVar8 = 0;
            }
            else if (0x58 < uVar8) {
              uVar8 = 0x58;
            }
            bVar2 = bVar9 & 0x1f;
            bVar9 = bVar9 + 4;
            param_3 = param_3 | iVar4 << bVar2;
            local_20 = local_20 - 1;
          } while (local_20 != 0);
        }
        puVar3[2] = param_1;
        puVar3[3] = param_3;
        puVar3 = puVar3 + 2;
      }
      in_EDX = (uint *)((int)in_EDX + param_2);
      local_10 = local_10 + -1;
    } while (local_10 != 0);
  }
  *param_4 = uVar7;
  *param_5 = uVar8;
  return 1;
}


// CALLEES of FUN_00c5f4e0:
//   00c5f2d0  FUN_00c5f2d0

//=== FUN_00c5f110 @ 00c5f110 (seed 0xc5f110) ===

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


// CALLEES of FUN_00c5f110:
//   00c5f000  FUN_00c5f000

//=== FUN_00c4d910 @ 00c4d910 (seed 0xc4d910) ===

void FUN_00c4d910(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5)

{
  FARPROC pFVar1;
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x38) == 0) {
    if (*(char *)(in_ECX + 0x104) == '\0') {
      pFVar1 = GetProcAddress(*(HMODULE *)(in_ECX + 4),"alBufferData");
    }
    else {
      pFVar1 = (FARPROC)FUN_00c4d740("alBufferData");
    }
    *(FARPROC *)(in_ECX + 0x38) = pFVar1;
  }
  (**(code **)(in_ECX + 0x38))(param_1,param_2,param_3,param_4,param_5);
  return;
}


// CALLEES of FUN_00c4d910:
//   00c4d740  FUN_00c4d740
//   EXTERNAL:000000c2  GetProcAddress

//=== FUN_00c57b60 @ 00c57b60 (seed 0xc57b60) ===

void FUN_00c57b60(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int in_ECX;
  int iVar5;
  
  if (DAT_0143be90 == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143be90 = GetProcAddress(DAT_0143bdb4,"alGetError");
    }
    else {
      DAT_0143be90 = (FARPROC)FUN_00c4d740("alGetError");
    }
  }
  (*DAT_0143be90)();
  uVar1 = *(undefined4 *)(in_ECX + 0x610);
  uVar2 = *(undefined4 *)(in_ECX + 0x60c);
  uVar3 = *(undefined4 *)(*(int *)(in_ECX + 0x604) + *(int *)(in_ECX + 0x620) * 4);
  if (DAT_0143bde8 == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143bde8 = GetProcAddress(DAT_0143bdb4,"alBufferData");
    }
    else {
      DAT_0143bde8 = (FARPROC)FUN_00c4d740("alBufferData");
    }
  }
  (*DAT_0143bde8)(uVar3,uVar2,in_ECX + 4,uVar1,0xac44);
  iVar5 = *(int *)(in_ECX + 0x604);
  iVar4 = *(int *)(in_ECX + 0x620);
  uVar1 = *(undefined4 *)(in_ECX + 0x608);
  if (DAT_0143be3c == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143be3c = GetProcAddress(DAT_0143bdb4,"alSourceQueueBuffers");
    }
    else {
      DAT_0143be3c = (FARPROC)FUN_00c4d740("alSourceQueueBuffers");
    }
  }
  (*DAT_0143be3c)(uVar1,1,iVar5 + iVar4 * 4);
  iVar5 = *(int *)(in_ECX + 0x620) + 1;
  *(int *)(in_ECX + 0x620) = iVar5;
  if (iVar5 == *(int *)(in_ECX + 0x618)) {
    *(undefined4 *)(in_ECX + 0x620) = 0;
  }
  *(int *)(in_ECX + 0x61c) = *(int *)(in_ECX + 0x61c) + 1;
  return;
}


// CALLEES of FUN_00c57b60:
//   00c4d740  FUN_00c4d740
//   EXTERNAL:000000c2  GetProcAddress

