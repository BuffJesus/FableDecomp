//=== FUN_00c16e10 @ 00c16e10 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

uint FUN_00c16e10(void)

{
  undefined4 uVar1;
  int iVar2;
  uint local_120 [2];
  undefined1 local_118 [264];
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01211dde;
  local_c = ExceptionList;
  local_10 = DAT_0139c8a8;
  ExceptionList = &local_c;
  uVar1 = FUN_00c0bfe0("LHFileSegmentBankInfo");
  local_4 = 0;
  iVar2 = FUN_00c2d9c0(uVar1);
  local_4 = 0xffffffff;
  FUN_00c0ba40();
  if (((-1 < iVar2) && (iVar2 = thunk_FUN_00c3a9e0(local_118,0x104), iVar2 == 0x104)) &&
     (iVar2 = thunk_FUN_00c3a9e0(local_118,0x104), iVar2 == 0x104)) {
    local_120[0] = 0;
    iVar2 = thunk_FUN_00c3a9e0(local_120,4);
    if (-1 < iVar2) {
      ExceptionList = local_c;
      return -(uint)(iVar2 != 0) & local_120[0];
    }
  }
  ExceptionList = local_c;
  return 0xffffffff;
}



//=== FUN_00c16f00 @ 00c16f00 ===

void FUN_00c16f00(C3DAnimFileXSequenceChunk *param_1,int param_2,int param_3)

{
  CChunkedFileChunk *pCVar1;
  CIsThingAlive *pCVar2;
  int iVar3;
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  *pCVar4;
  undefined4 uVar5;
  void *pvVar6;
  undefined4 in_EDX;
  char *pcVar7;
  int *piVar8;
  uint uVar9;
  __uint64 _Var10;
  uint local_cc [2];
  int local_c4;
  undefined4 local_c0;
  CGameScriptThing local_bc [124];
  int local_40;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01211e09;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00c2da20(in_EDX,*(undefined4 *)(param_3 + 8));
  local_4 = 0;
  if (local_40 != 0) {
    pCVar1 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar1,(CIsThingAlive *)"There were problems decoding the file segment structure")
    ;
    local_4 = 0xffffffff;
    FUN_00c2dbe0();
    ExceptionList = local_c;
    return;
  }
  pCVar2 = (CIsThingAlive *)FUN_00c16e10();
  if ((int)pCVar2 < 0) {
    pCVar1 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar1,(CIsThingAlive *)"Could not extract the version number");
    local_4 = 0xffffffff;
    FUN_00c2dbe0();
    ExceptionList = local_c;
    return;
  }
  iVar3 = FUN_00c16c10();
  if (iVar3 == 0) {
    pCVar1 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar1,(CIsThingAlive *)"This bank is not a normal LUG!");
LAB_00c17003:
    local_4 = 0xffffffff;
    FUN_00c2dbe0();
    ExceptionList = local_c;
    return;
  }
  if (pCVar2 != (CIsThingAlive *)&DAT_00000004) {
    pCVar1 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                            *)pCVar1,(CIsThingAlive *)"Expecting deferred bank version number ");
    pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          (pCVar4,(CIsThingAlive *)&DAT_00000004);
    pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)NScript::
                operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          (pCVar4,(CIsThingAlive *)"... got ");
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              (pCVar4,pCVar2);
    goto LAB_00c17003;
  }
  uVar5 = FUN_00c0bfe0("LHAudioBankCompData");
  local_4._0_1_ = 1;
  iVar3 = FUN_00c2d9c0(uVar5);
  local_4._0_1_ = 0;
  FUN_00c0ba40();
  if (iVar3 < 0) {
    pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              *)C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    pcVar7 = "Could not open the composite data segment";
  }
  else {
    _Var10 = CGameScriptThing::GetPThingUniqueID(local_bc);
    local_c4 = (int)_Var10;
    uVar5 = FUN_00c0bfe0("LHAudioBankLookupTable");
    local_4._0_1_ = 2;
    iVar3 = FUN_00c2d9c0(uVar5);
    local_4._0_1_ = 0;
    FUN_00c0ba40();
    if (iVar3 < 0) {
      pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
      pcVar7 = "Could not open the lookup table";
    }
    else {
      iVar3 = thunk_FUN_00c3a9e0(&local_c0,4);
      if (iVar3 < 4) {
        pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
        pcVar7 = "Could not read the highest priority";
      }
      else {
        iVar3 = thunk_FUN_00c3a9e0(local_cc,4);
        if (3 < iVar3) {
          if (local_cc[0] != 0) {
            pvVar6 = operator_new__(local_cc[0] * 0xc);
            iVar3 = thunk_FUN_00c3a9e0(pvVar6,local_cc[0] * 0xc);
            if (iVar3 < (int)(local_cc[0] * 0xc)) {
              operator_delete__(pvVar6);
              pCVar1 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
              NScript::
              operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                        ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                          *)pCVar1,(CIsThingAlive *)"Could not read in the lookup table");
            }
            else {
              uVar9 = 0;
              if (local_cc[0] != 0) {
                piVar8 = (int *)((int)pvVar6 + 8);
                do {
                  FUN_00c0e9e0(piVar8[-2],*piVar8 + local_c4,piVar8[-1]);
                  uVar9 = uVar9 + 1;
                  piVar8 = piVar8 + 3;
                } while (uVar9 < local_cc[0]);
              }
              *(undefined4 *)(param_2 + 0x4c) = local_c0;
              *(undefined4 *)(param_2 + 0x48) = 1;
              operator_delete__(pvVar6);
            }
            local_4 = 0xffffffff;
            FUN_00c2dbe0();
            ExceptionList = local_c;
            return;
          }
          goto LAB_00c170af;
        }
        pCVar4 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
        pcVar7 = "Could not read the number of samples";
      }
    }
  }
  NScript::
  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
            (pCVar4,(CIsThingAlive *)pcVar7);
LAB_00c170af:
  local_4 = 0xffffffff;
  FUN_00c2dbe0();
  ExceptionList = local_c;
  return;
}



//=== FUN_00c30f60 @ 00c30f60 ===

void FUN_00c30f60(undefined4 param_1,C3DAnimFileXSequenceChunk *param_2)

{
  bool bVar1;
  CChunkedFileChunk *pCVar2;
  CIsThingAlive *pCVar3;
  int iVar4;
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  *pCVar5;
  undefined4 uVar6;
  undefined4 in_ECX;
  __uint64 _Var7;
  undefined **local_d0 [2];
  undefined4 local_c8;
  undefined **local_c4 [2];
  CGameScriptThing local_bc [124];
  int local_40;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01214082;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00c2da20(in_ECX,param_1);
  local_4 = 0;
  if (local_40 != 0) {
    pCVar2 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_2);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar2,(CIsThingAlive *)"There were problems decoding the file segment structure")
    ;
    local_4 = 0xffffffff;
    FUN_00c2dbe0();
    ExceptionList = local_c;
    return;
  }
  pCVar3 = (CIsThingAlive *)FUN_00c16e10();
  if ((int)pCVar3 < 0) {
    pCVar2 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_2);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar2,(CIsThingAlive *)"Could not extract the version number");
    local_4 = 0xffffffff;
    FUN_00c2dbe0();
  }
  else {
    iVar4 = FUN_00c30e90();
    if (iVar4 == 0) {
      pCVar2 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_2);
      NScript::
      operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)pCVar2,(CIsThingAlive *)"This bank is not a normal LUG!");
    }
    else if (pCVar3 == (CIsThingAlive *)0x0) {
      uVar6 = FUN_00c0bfe0("LHAudioWaveData");
      local_4._0_1_ = 1;
      iVar4 = FUN_00c2d9c0(uVar6);
      local_4 = (uint)local_4._1_3_ << 8;
      FUN_00c0ba40();
      if (iVar4 < 0) {
        pCVar2 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_2);
        NScript::
        operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                    *)pCVar2,(CIsThingAlive *)"Could not open the wav segment");
      }
      else {
        _Var7 = CGameScriptThing::GetPThingUniqueID(local_bc);
        __EH_epilog3(local_bc);
        local_d0[0] = &PTR_FUN_012b4d3c;
        local_c8 = (int)_Var7;
        FUN_00c3bb50(param_2,local_d0);
        bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned((CTCInventoryBase *)param_2)
        ;
        if (bVar1) {
          local_d0[0] = &PTR_LAB_012b4d34;
          FUN_00c3bb50(param_2,local_d0);
          bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned
                            ((CTCInventoryBase *)param_2);
          if (bVar1) {
            local_c4[0] = &PTR_LAB_012b4d40;
            FUN_00c3bcb0(param_2,local_c4);
            NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned((CTCInventoryBase *)param_2);
          }
        }
      }
    }
    else {
      pCVar2 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_2);
      pCVar5 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)NScript::
                  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                            ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                              *)pCVar2,(CIsThingAlive *)"Expecting normal version number ");
      pCVar5 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)NScript::
                  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                            (pCVar5,(CIsThingAlive *)0x0);
      pCVar5 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)NScript::
                  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                            (pCVar5,(CIsThingAlive *)"... got ");
      NScript::
      operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                (pCVar5,pCVar3);
    }
    local_4 = 0xffffffff;
    FUN_00c2dbe0();
  }
  ExceptionList = local_c;
  return;
}



//=== FUN_00c3bb50 @ 00c3bb50 ===

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

void FUN_00c3bb50(C3DAnimFileXSequenceChunk *param_1,undefined4 *param_2)

{
  bool bVar1;
  undefined4 uVar2;
  int iVar3;
  CChunkedFileChunk *pCVar4;
  uint uVar5;
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  *pCVar6;
  CIsThingAlive *pCVar7;
  CIsThingAlive *local_2a4 [2];
  undefined1 local_29c [260];
  int local_198;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01214e6b;
  local_c = ExceptionList;
  local_10 = DAT_0139c8a8;
  ExceptionList = &local_c;
  uVar2 = FUN_00c0bfe0("LHAudioBankSampleTable");
  pCVar7 = (CIsThingAlive *)0x0;
  local_4 = 0;
  iVar3 = FUN_00c2d9c0(uVar2);
  local_4 = 0xffffffff;
  FUN_00c0ba40();
  if (iVar3 < 0) {
    pCVar4 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar4,(CIsThingAlive *)"Could not find the bank sample table segment");
  }
  else {
    iVar3 = thunk_FUN_00c3a9e0(local_2a4,4);
    if (iVar3 < 4) {
      pCVar4 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
      NScript::
      operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)pCVar4,(CIsThingAlive *)"Could not read the number of samples!");
    }
    else {
      local_2a4[0] = (CIsThingAlive *)((uint)local_2a4[0] & 0xffff);
      if (local_2a4[0] != (CIsThingAlive *)0x0) {
        do {
          uVar5 = thunk_FUN_00c3a9e0(local_29c,0x28c);
          if (uVar5 < 0x28c) {
            pCVar4 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
            pCVar6 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                      *)NScript::
                        operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                                  ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                                    *)pCVar4,(CIsThingAlive *)"Could not read in resource ");
            NScript::
            operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                      (pCVar6,pCVar7);
            ExceptionList = local_c;
            return;
          }
          if (local_198 != 0) {
            (**(code **)*param_2)(param_1,local_29c);
            bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned
                              ((CTCInventoryBase *)param_1);
            if (!bVar1) {
              ExceptionList = local_c;
              return;
            }
          }
          pCVar7 = pCVar7 + 1;
        } while (pCVar7 < local_2a4[0]);
      }
    }
  }
  ExceptionList = local_c;
  return;
}



//=== FUN_00c30e90 @ 00c30e90 ===

bool FUN_00c30e90(void)

{
  bool bVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01214062;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  uVar3 = FUN_00c0bfe0("LHAudioWaveData");
  bVar2 = false;
  local_4 = 0;
  iVar4 = FUN_00c2d9c0(uVar3);
  if (-1 < iVar4) {
    uVar3 = FUN_00c0bfe0("LHAudioBankSampleTable");
    local_4 = 1;
    bVar2 = true;
    iVar4 = FUN_00c2d9c0(uVar3);
    bVar1 = false;
    if (-1 < iVar4) goto LAB_00c30f0d;
  }
  bVar1 = true;
LAB_00c30f0d:
  local_4 = 0;
  if (bVar2) {
    FUN_00c0ba40();
  }
  local_4 = 0xffffffff;
  FUN_00c0ba40();
  ExceptionList = local_c;
  return !bVar1;
}



//=== FUN_00c16d90 @ 00c16d90 ===

void FUN_00c16d90(void)

{
  bool bVar1;
  int in_ECX;
  CTCInventoryBase *unaff_EDI;
  WaypointData local_3c [48];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01211dc9;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  NScript::CV_TourGuideScript::WaypointData::WaypointData(local_3c);
  local_4 = 0;
  FUN_00c30f60(*(undefined4 *)(in_ECX + 8),unaff_EDI);
  bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(unaff_EDI);
  if (bVar1) {
    FUN_00c16ce0(unaff_EDI);
  }
  local_4 = 0xffffffff;
  NScript::CV_TourGuideScript::WaypointData::~WaypointData(local_3c);
  ExceptionList = local_c;
  return;
}



//=== UpdateCurrentAction @ 00c17260 ===

/* [bsim sim=0.5007573590512484 <- ego_r]
   protected: void __thiscall CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(void) */

void __thiscall
CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(CAIStateGroup_MeleeCombatNearbyBase *this)

{
  bool bVar1;
  char extraout_AL;
  CChunkedFileChunk *pCVar2;
  CTCInventoryBase *unaff_ESI;
  CTCBase *in_stack_ffffffc4;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01211e1b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  NScript::CV_TourGuideScript::WaypointData::WaypointData((WaypointData *)&stack0xffffffc4);
  local_4 = 0;
  FUN_00c30f60(0x2000,unaff_ESI);
  bVar1 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned(unaff_ESI);
  if (bVar1) {
    CTCInventory::Copy((CTCInventory *)&stack0xffffffc4,in_stack_ffffffc4);
    if (extraout_AL == '\0') {
      pCVar2 = C3DAnimFileXSequenceChunk::CreateNewInstance((C3DAnimFileXSequenceChunk *)unaff_ESI);
      NScript::
      operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)pCVar2,
                 (CIsThingAlive *)"There was a problem serialising the extracted meta data.");
    }
  }
  local_4 = 0xffffffff;
  NScript::CV_TourGuideScript::WaypointData::~WaypointData((WaypointData *)&stack0xffffffc4);
  ExceptionList = local_c;
  return;
}



//=== FUN_00c3bcb0 @ 00c3bcb0 ===

void FUN_00c3bcb0(C3DAnimFileXSequenceChunk *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  bool bVar2;
  undefined4 uVar3;
  int iVar4;
  CChunkedFileChunk *pCVar5;
  void *pvVar6;
  CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
  *pCVar7;
  char *pcVar8;
  void *pvVar9;
  int local_18;
  uint local_14;
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_01214e7d;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  uVar3 = FUN_00c0bfe0("LHAudioBankCriteiaInfo");
  local_4 = 0;
  iVar4 = FUN_00c2d9c0(uVar3);
  local_4 = 0xffffffff;
  FUN_00c0ba40();
  if (iVar4 < 0) {
    pCVar5 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar5,(CIsThingAlive *)"Could not find the criteria info segment");
    ExceptionList = local_c;
    return;
  }
  iVar4 = thunk_FUN_00c3a9e0(&local_10,4);
  puVar1 = param_2;
  if (iVar4 < 4) {
    pCVar5 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
    NScript::
    operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
              ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)pCVar5,(CIsThingAlive *)"Could not read the number of criteria entries!");
    ExceptionList = local_c;
    return;
  }
  local_14 = 0;
  if (local_10 == 0) {
    ExceptionList = local_c;
    return;
  }
  while( true ) {
    iVar4 = thunk_FUN_00c3a9e0(&param_2,4);
    if (iVar4 < 4) {
      pCVar5 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
      NScript::
      operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  *)pCVar5,(CIsThingAlive *)"Could not read the text length");
      ExceptionList = local_c;
      return;
    }
    pvVar6 = operator_new__((int)param_2 + 1);
    *(undefined1 *)((int)pvVar6 + (int)param_2) = 0;
    iVar4 = thunk_FUN_00c3a9e0(pvVar6,param_2);
    if (iVar4 < (int)param_2) break;
    iVar4 = thunk_FUN_00c3a9e0(&local_18,4);
    if (iVar4 < 4) {
      pCVar7 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                *)C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
      pcVar8 = "Could not read the number of variations";
      goto LAB_00c3bebb;
    }
    pvVar9 = (void *)0x0;
    if (local_18 != 0) {
      pvVar9 = operator_new__(local_18 * 4);
      iVar4 = thunk_FUN_00c3a9e0(pvVar9,local_18 * 4);
      if (iVar4 < local_18 * 4) {
        pCVar5 = C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
        NScript::
        operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                  ((CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
                    *)pCVar5,(CIsThingAlive *)"Could not read the driver list");
        operator_delete(pvVar6);
        operator_delete__(pvVar9);
        ExceptionList = local_c;
        return;
      }
    }
    (**(code **)*puVar1)(param_1,pvVar6,local_18,pvVar9);
    operator_delete(pvVar6);
    if (pvVar9 != (void *)0x0) {
      operator_delete__(pvVar9);
    }
    bVar2 = NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned((CTCInventoryBase *)param_1);
    if (!bVar2) {
      ExceptionList = local_c;
      return;
    }
    local_14 = local_14 + 1;
    if (local_10 <= local_14) {
      ExceptionList = local_c;
      return;
    }
  }
  pCVar7 = (CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
            *)C3DAnimFileXSequenceChunk::CreateNewInstance(param_1);
  pcVar8 = "Could not read text";
LAB_00c3bebb:
  NScript::
  operator&&<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CPredicate_And<NScript::CIsThingAlive,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>,NScript::CIsThingAlive>
            (pCVar7,(CIsThingAlive *)pcVar8);
  operator_delete(pvVar6);
  ExceptionList = local_c;
  return;
}



