//=== FUN_00c5f6a0 @ 00c5f6a0 ===

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



//=== FUN_00c5f7f0 @ 00c5f7f0 ===

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



//=== FUN_00c5f3b0 @ 00c5f3b0 ===

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



//=== FUN_00c5f4e0 @ 00c5f4e0 ===

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



