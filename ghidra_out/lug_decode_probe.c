//=== FUN_00c5fa50 @ 00c5fa50 (callers:
//     <- 00c42688 FUN_00c425e0
// ) ===

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



//=== FUN_00c565f0 @ 00c565f0 (callers:
//     <- 00c57470
// ) ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

undefined4 FUN_00c565f0(char param_1)

{
  char cVar1;
  DWORD DVar2;
  HMODULE pHVar3;
  undefined4 uVar4;
  UINT UVar5;
  HANDLE hFindFile;
  CEngineWaterRenderer *in_ECX;
  CHAR local_350 [264];
  undefined1 local_248 [260];
  _WIN32_FIND_DATAA local_144;
  undefined4 local_4;
  
  local_4 = DAT_0139c8a8;
  if ((in_ECX[0x10c] != (CEngineWaterRenderer)0x0) && (param_1 == '\0')) {
    return 0;
  }
  in_ECX[0x10c] = (CEngineWaterRenderer)0x1;
  DVar2 = GetCurrentDirectoryA(0x105,local_350);
  if (DVar2 == 0) {
LAB_00c56713:
    UVar5 = GetSystemDirectoryA(local_350,0x105);
    if (UVar5 != 0) {
      FUN_00c56090(local_350,0x104,"/OpenAL32.dll");
      hFindFile = FindFirstFileA(local_350,&local_144);
      if (hFindFile != (HANDLE)0xffffffff) {
        FindClose(hFindFile);
        pHVar3 = LoadLibraryA(local_350);
        *(HMODULE *)(in_ECX + 4) = pHVar3;
        if (pHVar3 != (HMODULE)0x0) {
          cVar1 = FUN_00c55980();
          if (cVar1 != '\0') {
            cVar1 = FUN_00c55aa0(0);
            if (cVar1 != '\0') {
              cVar1 = FUN_00c55da0(local_248);
              CEngineWaterRenderer::DestroyWaterGenerator(in_ECX);
              if (cVar1 != '\0') {
                cVar1 = FUN_00c55aa0(local_248);
                if (cVar1 != '\0') {
                  cVar1 = FUN_00c55b00();
                  if (cVar1 != '\0') {
                    cVar1 = FUN_00c562f0();
                    if (cVar1 != '\0') goto LAB_00c566ea;
                    FUN_00c559c0();
                  }
                  CEngineWaterRenderer::DestroyWaterGenerator(in_ECX);
                }
              }
            }
          }
          FreeLibrary(*(HMODULE *)(in_ECX + 4));
          *(undefined4 *)(in_ECX + 4) = 0;
        }
      }
    }
    uVar4 = 0;
  }
  else {
    FUN_00c56090(local_350,0x104,"/OpenAL32.dll");
    pHVar3 = LoadLibraryA(local_350);
    *(HMODULE *)(in_ECX + 4) = pHVar3;
    if (pHVar3 == (HMODULE)0x0) {
      DVar2 = GetCurrentDirectoryA(0x105,local_350);
      if (DVar2 != 0) {
        FUN_00c56090(local_350,0x104,"/wrap_oal.dll");
        pHVar3 = LoadLibraryA(local_350);
        *(HMODULE *)(in_ECX + 4) = pHVar3;
      }
    }
    if (*(int *)(in_ECX + 4) == 0) {
LAB_00c5670c:
      CMemoryFile::Open((CMemoryFile *)in_ECX);
      goto LAB_00c56713;
    }
    cVar1 = FUN_00c55980();
    if (cVar1 == '\0') {
LAB_00c566ff:
      FreeLibrary(*(HMODULE *)(in_ECX + 4));
      *(undefined4 *)(in_ECX + 4) = 0;
      goto LAB_00c5670c;
    }
    cVar1 = FUN_00c55aa0("DirectSound3D");
    if (cVar1 == '\0') goto LAB_00c566ff;
    cVar1 = FUN_00c55b00();
    if (cVar1 == '\0') {
LAB_00c566f8:
      CEngineWaterRenderer::DestroyWaterGenerator(in_ECX);
      goto LAB_00c566ff;
    }
    cVar1 = FUN_00c562f0();
    if (cVar1 == '\0') {
      FUN_00c559c0();
      goto LAB_00c566f8;
    }
LAB_00c566ea:
    uVar4 = 1;
  }
  return uVar4;
}



//=== FUN_00c312c0 @ 00c312c0 (callers:
//     <- 00c3125e FUN_00c31230
// ) ===

int FUN_00c312c0(undefined4 *param_1)

{
  int iVar1;
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  *pCVar2;
  int *in_ECX;
  CLandscapeLayerMesh *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_012140a3;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  local_10 = operator_new(0x20);
  local_4 = 0;
  if (local_10 == (CLandscapeLayerMesh *)0x0) {
    iVar1 = 0;
  }
  else {
    iVar1 = CLandscapeLayerMesh::CLandscapeLayerMesh(local_10,(CEngineLandscapeMap *)*param_1);
  }
  local_4 = 0xffffffff;
  if (iVar1 == 0) {
    local_10 = (CLandscapeLayerMesh *)PTR_DAT_0139c900;
    local_4 = 1;
    pCVar2 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                            *)&local_10,
                           (CIsThingAlive *)"d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCAutoMap.h");
    pCVar2 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          (pCVar2,(CIsThingAlive *)&DAT_0122e028);
    pCVar2 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          (pCVar2,(CIsThingAlive *)0x28);
    pCVar2 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          (pCVar2,(CIsThingAlive *)&DAT_012b1ae4);
    pCVar2 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          (pCVar2,(CIsThingAlive *)&DAT_012b1b58);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              (pCVar2,(CIsThingAlive *)&DAT_0129b208);
    if (DAT_0143bb2c != (code *)0x0) {
      (*DAT_0143bb2c)();
    }
    local_4 = 0xffffffff;
    FUN_00c0ba40();
    std::list<CMessageEvent_const*,std::allocator<CMessageEvent_const*>_>::
    list<CMessageEvent_const*,std::allocator<CMessageEvent_const*>_>
              ((list<CMessageEvent_const*,std::allocator<CMessageEvent_const*>_> *)&param_1);
  }
  (**(code **)(*in_ECX + 0x24))(iVar1);
  ExceptionList = local_10;
  return iVar1;
}



