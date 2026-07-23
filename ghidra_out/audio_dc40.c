//=== FUN_00c4dc40 @ 00c4dc40 (seed 0xc4dc40) ===

undefined4 FUN_00c4dc40(int *param_1)

{
  char cVar1;
  int iVar2;
  undefined4 *in_ECX;
  
  if (2 < (uint)param_1[1]) {
    return 0xfffffff2;
  }
  if (DAT_0143be90 == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143be90 = GetProcAddress(DAT_0143bdb4,"alGetError");
    }
    else {
      DAT_0143be90 = (FARPROC)FUN_00c4d740("alGetError");
    }
  }
  (*DAT_0143be90)();
  cVar1 = DAT_0143bf60;
  if (DAT_0143bdcc == (FARPROC)0x0) {
    DAT_0143bdcc = GetProcAddress(DAT_0143bdb4,"alcSuspendContext");
  }
  (*DAT_0143bdcc)(DAT_0143beb0);
  DAT_0143bf60 = '\x01';
  if (DAT_0143bddc == (FARPROC)0x0) {
    if (DAT_0143beb4 == '\0') {
      DAT_0143bddc = GetProcAddress(DAT_0143bdb4,"alGenBuffers");
    }
    else {
      DAT_0143bddc = (FARPROC)FUN_00c4d740("alGenBuffers");
    }
  }
  (*DAT_0143bddc)(1);
  if (cVar1 == '\0') {
    if (DAT_0143bdd0 == (FARPROC)0x0) {
      DAT_0143bdd0 = GetProcAddress(DAT_0143bdb4,"alcProcessContext");
    }
    (*DAT_0143bdd0)(DAT_0143beb0);
    DAT_0143bf60 = '\0';
  }
  iVar2 = FUN_00c4dbe0();
  if (-1 < iVar2) {
    if (*param_1 == 1) {
      iVar2 = (uint)(param_1[1] != 1) * 2 + 0x1101;
    }
    else if (*param_1 == 2) {
      iVar2 = (uint)(param_1[1] != 1) * 2 + 0x1100;
    }
    else {
      iVar2 = 0;
    }
    FUN_00c4d910(*in_ECX,iVar2,param_1[6],param_1[3],param_1[2]);
    iVar2 = FUN_00c4dbe0();
    cVar1 = DAT_0143bf60;
    if (-1 < iVar2) {
      in_ECX[3] = param_1;
      in_ECX[2] = in_ECX[2] | 1;
      DAT_0143bf64 = DAT_0143bf64 + 1;
      return 0;
    }
    FUN_00c4d790();
    FUN_00c4d8c0(1,in_ECX);
    if (cVar1 == '\0') {
      FUN_00c4d7d0();
    }
    return 0xfffffff2;
  }
  return 0xfffffffb;
}


// CALLEES of FUN_00c4dc40:
//   00c4d910  FUN_00c4d910
//   00c4d8c0  FUN_00c4d8c0
//   00c4dbe0  FUN_00c4dbe0
//   00c4d790  FUN_00c4d790
//   00c4d740  FUN_00c4d740
//   00c4d7d0  FUN_00c4d7d0
//   EXTERNAL:000000c2  GetProcAddress

//=== FUN_00c5fa50 @ 00c5fa50 (seed 0xc5fa50) ===

/* WARNING: Removing unreachable block (ram,0x00c5fe34) */
/* WARNING: Removing unreachable block (ram,0x00c5fc5f) */

undefined4 FUN_00c5fa50(int param_1,undefined4 *param_2,int param_3)

{
  uint *puVar1;
  uint *puVar2;
  ushort uVar3;
  code *pcVar4;
  ulonglong uVar5;
  undefined4 uVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  int iVar9;
  int in_ECX;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  undefined4 *puVar15;
  undefined4 *puVar16;
  ulonglong uVar17;
  undefined8 uVar18;
  uint local_30;
  int local_2c;
  int local_20;
  undefined4 *local_1c;
  uint local_18;
  undefined4 local_14;
  int local_10;
  
  uVar3 = *(ushort *)(in_ECX + 0xc);
  uVar10 = (uint)uVar3;
  uVar13 = (uint)*(ushort *)(in_ECX + 0x12) * (uint)*(ushort *)(in_ECX + 2);
  uVar11 = uVar10 * param_3;
  puVar7 = param_2;
  for (uVar12 = uVar11 >> 2; uVar12 != 0; uVar12 = uVar12 - 1) {
    *puVar7 = 0;
    puVar7 = puVar7 + 1;
  }
  for (uVar12 = uVar11 & 3; uVar12 != 0; uVar12 = uVar12 - 1) {
    *(undefined1 *)puVar7 = 0;
    puVar7 = (undefined4 *)((int)puVar7 + 1);
  }
  uVar12 = uVar13 * 2;
  switch(*(undefined4 *)(in_ECX + 0x14)) {
  case 0:
    uVar6 = (**(code **)(in_ECX + 0x20))
                      (param_3,*(undefined2 *)(in_ECX + 0xc),*(undefined2 *)(in_ECX + 0x12),
                       in_ECX + 0x18,in_ECX + 0x1c);
    return uVar6;
  case 1:
    printf("Using normal encoding...\n");
    puVar7 = operator_new__(uVar12 * param_3);
    if (puVar7 == (undefined4 *)0x0) {
      return 0;
    }
    pcVar4 = *(code **)("Using whole file encoding...\n" + (uint)*(ushort *)(in_ECX + 2) * 4 + 0x1c)
    ;
    iVar9 = (**(code **)(in_ECX + 0x20))
                      (param_3,*(undefined2 *)(in_ECX + 0xc),*(undefined2 *)(in_ECX + 0x12),
                       in_ECX + 0x18,in_ECX + 0x1c);
    if ((iVar9 == 0) ||
       (iVar9 = (*pcVar4)(param_3,*(undefined2 *)(in_ECX + 0xc),0x40,in_ECX + 0x18,in_ECX + 0x1c),
       iVar9 == 0)) {
LAB_00c5fd1b:
      operator_delete__(puVar7);
      return 0;
    }
    break;
  case 2:
    printf("Using whole file encoding...\n");
    puVar7 = operator_new__(uVar11);
    if (puVar7 == (undefined4 *)0x0) {
      return 0;
    }
    uVar10 = uVar12 * param_3;
    puVar8 = operator_new__(uVar10);
    if (puVar8 != (undefined4 *)0x0) {
      pcVar4 = *(code **)("Using whole file encoding...\n" +
                         (uint)*(ushort *)(in_ECX + 2) * 4 + 0x1c);
      uVar5 = 0xffffffffffffffff;
      local_1c = (undefined4 *)0xffffffff;
      local_30 = 0;
      do {
        puVar15 = puVar7;
        for (uVar13 = uVar11 >> 2; uVar13 != 0; uVar13 = uVar13 - 1) {
          *puVar15 = 0;
          puVar15 = puVar15 + 1;
        }
        for (uVar13 = uVar11 & 3; uVar13 != 0; uVar13 = uVar13 - 1) {
          *(undefined1 *)puVar15 = 0;
          puVar15 = (undefined4 *)((int)puVar15 + 1);
        }
        puVar15 = puVar8;
        for (uVar13 = uVar10 >> 2; uVar13 != 0; uVar13 = uVar13 - 1) {
          *puVar15 = 0;
          puVar15 = puVar15 + 1;
        }
        for (uVar13 = uVar10 & 3; uVar13 != 0; uVar13 = uVar13 - 1) {
          *(undefined1 *)puVar15 = 0;
          puVar15 = (undefined4 *)((int)puVar15 + 1);
        }
        puVar1 = (uint *)(in_ECX + 0x1c);
        *(uint *)(in_ECX + 0x18) = local_30;
        *puVar1 = local_30;
        iVar9 = (**(code **)(in_ECX + 0x20))
                          (param_3,*(undefined2 *)(in_ECX + 0xc),*(undefined2 *)(in_ECX + 0x12),
                           (uint *)(in_ECX + 0x18),puVar1);
        if (((iVar9 != 0) &&
            (iVar9 = (*pcVar4)(param_3,*(undefined2 *)(in_ECX + 0xc),0x40,in_ECX + 0x18,puVar1),
            iVar9 != 0)) &&
           (uVar17 = FUN_00c5f150(param_1,puVar8,param_3,param_3,uVar12), uVar17 < uVar5)) {
          puVar15 = puVar7;
          puVar16 = param_2;
          for (uVar13 = uVar11 >> 2; uVar13 != 0; uVar13 = uVar13 - 1) {
            *puVar16 = *puVar15;
            puVar15 = puVar15 + 1;
            puVar16 = puVar16 + 1;
          }
          for (uVar13 = uVar11 & 3; uVar13 != 0; uVar13 = uVar13 - 1) {
            *(undefined1 *)puVar16 = *(undefined1 *)puVar15;
            puVar15 = (undefined4 *)((int)puVar15 + 1);
            puVar16 = (undefined4 *)((int)puVar16 + 1);
          }
          local_1c = (undefined4 *)local_30;
          uVar5 = uVar17;
        }
        local_14 = (undefined4)(uVar5 >> 0x20);
        local_18 = (uint)uVar5;
        local_30 = local_30 + 1;
      } while (local_30 < 0x59);
      printf("Difference between original and decoded streams: 0x%I64x\n",local_18,local_14);
      printf("Step index chosen: %d\n",local_1c);
      operator_delete__(puVar7);
      operator_delete__(puVar8);
      return 1;
    }
    operator_delete__(puVar7);
    return 0;
  case 3:
    printf("Using per-block encoding\n\n");
    puVar7 = operator_new__(uVar10);
    if (puVar7 == (undefined4 *)0x0) {
      return 0;
    }
    puVar8 = operator_new__(uVar12);
    if (puVar8 == (undefined4 *)0x0) goto LAB_00c5fd1b;
    pcVar4 = *(code **)("Using whole file encoding...\n" + (uint)*(ushort *)(in_ECX + 2) * 4 + 0x1c)
    ;
    uVar11 = 0xffffffff;
    if (param_3 != 0) {
      local_1c = param_2;
      local_2c = param_1;
      local_10 = param_3;
      do {
        uVar5 = 0xffffffffffffffff;
        local_18 = 0xffffffff;
        if (uVar11 == 0xffffffff) {
          uVar14 = 0;
          local_20 = 0x59;
          uVar11 = local_18;
        }
        else {
          uVar14 = uVar11 - 0x18;
          local_20 = uVar11 + 0x19;
          uVar11 = local_18;
        }
        for (; (int)uVar14 < local_20; uVar14 = uVar14 + 1) {
          local_18 = uVar11;
          if ((-1 < (int)uVar14) && (uVar14 < 0x59)) {
            puVar15 = puVar7;
            for (uVar11 = (uint)(uVar3 >> 2); uVar11 != 0; uVar11 = uVar11 - 1) {
              *puVar15 = 0;
              puVar15 = puVar15 + 1;
            }
            for (uVar11 = uVar10 & 3; uVar11 != 0; uVar11 = uVar11 - 1) {
              *(undefined1 *)puVar15 = 0;
              puVar15 = (undefined4 *)((int)puVar15 + 1);
            }
            puVar15 = puVar8;
            for (uVar11 = (uVar13 & 0x7fffffff) >> 1; uVar11 != 0; uVar11 = uVar11 - 1) {
              *puVar15 = 0;
              puVar15 = puVar15 + 1;
            }
            for (uVar11 = uVar12 & 3; uVar11 != 0; uVar11 = uVar11 - 1) {
              *(undefined1 *)puVar15 = 0;
              puVar15 = (undefined4 *)((int)puVar15 + 1);
            }
            puVar1 = (uint *)(in_ECX + 0x1c);
            puVar2 = (uint *)(in_ECX + 0x18);
            *puVar1 = uVar14;
            *puVar2 = uVar14;
            iVar9 = (**(code **)(in_ECX + 0x20))
                              (1,*(undefined2 *)(in_ECX + 0xc),*(undefined2 *)(in_ECX + 0x12),puVar2
                               ,puVar1);
            if (((iVar9 != 0) &&
                (iVar9 = (*pcVar4)(1,*(undefined2 *)(in_ECX + 0xc),0x40,puVar2,puVar1), iVar9 != 0))
               && (uVar17 = FUN_00c5f150(local_2c,puVar8,1,param_3,uVar12), uVar17 < uVar5)) {
              puVar15 = puVar7;
              puVar16 = local_1c;
              for (uVar11 = (uint)(uVar3 >> 2); uVar11 != 0; uVar11 = uVar11 - 1) {
                *puVar16 = *puVar15;
                puVar15 = puVar15 + 1;
                puVar16 = puVar16 + 1;
              }
              for (uVar11 = uVar10 & 3; local_18 = uVar14, uVar5 = uVar17, uVar11 != 0;
                  uVar11 = uVar11 - 1) {
                *(undefined1 *)puVar16 = *(undefined1 *)puVar15;
                puVar15 = (undefined4 *)((int)puVar15 + 1);
                puVar16 = (undefined4 *)((int)puVar16 + 1);
              }
            }
          }
          uVar11 = local_18;
        }
        local_2c = local_2c + uVar12;
        local_1c = (undefined4 *)((int)local_1c + uVar10);
        local_10 = local_10 + -1;
      } while (local_10 != 0);
    }
    operator_delete__(puVar7);
    operator_delete__(puVar8);
    puVar7 = operator_new__(uVar12 * param_3);
    (*pcVar4)(param_3,*(undefined2 *)(in_ECX + 0xc),0x40,in_ECX + 0x18,in_ECX + 0x1c);
    break;
  default:
    goto switchD_00c5fa9d_default;
  }
  uVar18 = FUN_00c5f150(param_1,puVar7,param_3,param_3,uVar12);
  printf("Difference between original and decoded streams: 0x%I64x\n",(int)uVar18,
         (int)((ulonglong)uVar18 >> 0x20));
  operator_delete__(puVar7);
switchD_00c5fa9d_default:
  return 1;
}


// CALLEES of FUN_00c5fa50:
//   EXTERNAL:0000000e  printf
//   00bfeb1c  operator_delete[]
//   00bfeb22  operator_new[]
//   00c5f150  FUN_00c5f150

