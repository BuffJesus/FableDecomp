==================== ?BuildLayersFromThemes@CEngineLandscapeMeshBuilder@@AAEPAVCWaterPatchDescriptors@@PBVCEngineMap@@JJ@Z @ 02cb0660 ====================

/* WARNING: Removing unreachable block (ram,0x02cb07e3) */
/* WARNING: Removing unreachable block (ram,0x02cb0682) */
/* WARNING: Removing unreachable block (ram,0x02cb06a7) */
/* WARNING: Removing unreachable block (ram,0x02cb0808) */
/* WARNING: Removing unreachable block (ram,0x02cb08b8) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void * _BuildLayersFromThemes_CEngineLandscapeMeshBuilder__AAEPAVCWaterPatchDescriptors__PBVCEngineMap__JJ_Z
                 (undefined4 param_1,int param_2,int param_3)

{
  code *pcVar1;
  char cVar2;
  int *piVar3;
  BOOL BVar4;
  void *pvVar5;
  uint uVar6;
  int iVar7;
  undefined4 *puVar8;
  float10 fVar9;
  undefined1 *puVar10;
  undefined1 *puVar11;
  undefined1 *puVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined1 local_54 [8];
  undefined1 local_4c [8];
  undefined1 local_44 [11];
  char local_39;
  uint local_38;
  int local_34;
  float local_30;
  int local_2c;
  int local_28;
  int local_24;
  uint local_20;
  int local_1c;
  byte local_15;
  void *local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e5cea8;
  local_10 = ExceptionList;
  local_1c = 0;
  ExceptionList = &local_10;
  do {
    if (4 < local_1c) {
      local_15 = 0;
      local_14 = operator_new(0x484);
      memset(local_14,0,0x484);
      for (local_24 = 0; local_24 < 0x11; local_24 = local_24 + 1) {
        for (local_28 = 0; local_28 < 0x11; local_28 = local_28 + 1) {
          local_2c = param_2 + local_28;
          local_34 = param_3 + local_24;
          _ReadThemesAndCreateLayers_CEngineLandscapeMeshBuilder__AAEXPBVCEngineMap__JJJJ_Z
                    (param_1,local_2c,local_34,local_28,local_24);
          fVar9 = (float10)_PeekInterpolatedWaterHeight_CEngineMap__QBEMJJJ_Z(local_2c,local_34,2);
          local_30 = (float)fVar9;
          *(float *)((int)local_14 + local_24 * 4 + local_28 * 0x44) = local_30;
          local_15 = local_15 | _DAT_0443b590 < local_30 != (_DAT_0443b590 == local_30);
        }
      }
      local_38 = 0;
      while (uVar6 = _size___vector_UCLayer_CEngineLandscapeMeshBuilder__V__allocator_UCLayer_CEngineLandscapeMeshBuilder___std___std__QBEIXZ
                               (), local_38 < uVar6) {
        iVar7 = __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                          (local_38);
        uVar13 = *(undefined4 *)(iVar7 + 0x328);
        __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                  (local_38);
        puVar8 = (undefined4 *)__A__CArray_J__QAEAAJI_Z(uVar13);
        *puVar8 = 0xffffffff;
        local_38 = local_38 + 1;
      }
      if (local_15 == 0) {
        local_14 = (void *)0x0;
      }
      ExceptionList = local_10;
      return local_14;
    }
    local_20 = 0;
    while (uVar6 = _size___vector_JV__allocator_J_std___std__QBEIXZ(), local_20 < uVar6) {
      piVar3 = (int *)__A__CArray_J__QAEAAJI_Z(local_20);
      if (-1 < *piVar3) {
        __0CWideString__QAE_PB_W_Z(L"Engine_Landscape_Mesh_Builder.cpp");
        local_8 = 0;
        __0CCharString__QAE_PBDJ_Z(&DAT_0443b68f,0xffffffff);
        local_8._0_1_ = 1;
        __0CCharString__QAE_PBDJ_Z("PassMappingTable[dir][t]<0",0xffffffff);
        local_8._0_1_ = 2;
        uVar14 = 1;
        uVar13 = 0x231;
        puVar12 = local_44;
        puVar11 = local_4c;
        puVar10 = local_54;
        _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar10,puVar11,puVar12,0x231,1);
        _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
        cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                          (puVar10,puVar11,puVar12,uVar13,uVar14);
        local_39 = '\x01' - (cVar2 != '\x01');
        local_8._0_1_ = 1;
        __1CCharString__QAE_XZ();
        local_8 = (uint)local_8._1_3_ << 8;
        __1CCharString__QAE_XZ();
        local_8 = 0xffffffff;
        __1CWideString__QAE_XZ();
        if ((local_39 != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
          pcVar1 = (code *)swi(3);
          pvVar5 = (void *)(*pcVar1)();
          return pvVar5;
        }
      }
      local_20 = local_20 + 1;
    }
    local_1c = local_1c + 1;
  } while( true );
}


==================== ?Build@CWaterPatchMesh@@QAE_NABVCWaterPatchDescriptors@@@Z @ 02e689b0 ====================

/* WARNING: Removing unreachable block (ram,0x02e68a43) */
/* WARNING: Removing unreachable block (ram,0x02e68a1e) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

bool __thiscall _Build_CWaterPatchMesh__QAE_NABVCWaterPatchDescriptors___Z(int param_1,int param_2)

{
  bool bVar1;
  char cVar2;
  void *pvVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  float10 fVar7;
  undefined4 local_2c;
  int local_1c;
  int local_18;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e86c6b;
  local_10 = ExceptionList;
  bVar1 = false;
  ExceptionList = &local_10;
  pvVar3 = operator_new(0x484);
  local_8 = 0;
  if (pvVar3 == (void *)0x0) {
    local_2c = 0;
  }
  else {
    local_2c = __0CVertexHeightData_CWaterPatchMesh__QAE_XZ();
  }
  local_8 = 0xffffffff;
  *(undefined4 *)(param_1 + 0x14) = local_2c;
  for (local_18 = 0; local_18 < 0x11; local_18 = local_18 + 1) {
    for (local_1c = 0; local_1c < 0x11; local_1c = local_1c + 1) {
      if (*(float *)(local_18 * 0x44 + param_2 + local_1c * 4) <= (float)_DAT_0401df40) {
        cVar2 = _FindCorrectWaterLevel_CWaterPatchMesh__ABE_NAAMABVCWaterPatchDescriptors__JJ_Z
                          (*(int *)(param_1 + 0x14) + local_18 * 0x44 + local_1c * 4,param_2,
                           local_18,local_1c);
        if (cVar2 == '\0') {
          iVar6 = local_1c + *(int *)(param_1 + 0xc);
          iVar4 = local_18 + *(int *)(param_1 + 8);
          _PeekEngineMap_CEngineLandscapeMap__QBEPBVCEngineMap__XZ(iVar4,iVar6);
          fVar7 = (float10)_PeekLandscapeHeight_CEngineMap__QBEMJJ_Z(iVar4,iVar6);
          *(float *)(local_18 * 0x44 + *(int *)(param_1 + 0x14) + local_1c * 4) =
               (float)(fVar7 - (float10)_DAT_0401df70);
        }
      }
      else {
        bVar1 = true;
        *(undefined4 *)(local_18 * 0x44 + *(int *)(param_1 + 0x14) + local_1c * 4) =
             *(undefined4 *)(local_18 * 0x44 + param_2 + local_1c * 4);
      }
    }
  }
  if (bVar1) {
    uVar5 = _GetWaterType_CWaterPatchMesh__ABE_BW4EWaterType__XZ();
    *(undefined4 *)(param_1 + 0x1c) = uVar5;
  }
  ExceptionList = local_10;
  return bVar1;
}


==================== ?Save@CWaterPatchMesh@@QAEXAAVCDataOutputStream@@@Z @ 02e68bf0 ====================

/* WARNING: Removing unreachable block (ram,0x02e68c88) */
/* [ported from ego_r via strfp] */

void __thiscall _Save_CWaterPatchMesh__QAEXAAVCDataOutputStream___Z(int param_1,int *param_2)

{
  code *pcVar1;
  char cVar2;
  undefined2 uVar3;
  int iVar4;
  BOOL BVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined1 *puVar8;
  undefined4 uVar9;
  uint uVar10;
  undefined1 local_78 [8];
  undefined1 local_70 [8];
  undefined1 local_68 [11];
  char local_5d;
  int local_5c;
  undefined4 local_58;
  undefined4 local_50;
  int local_4c;
  undefined1 local_24 [20];
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e86cdb;
  local_10 = ExceptionList;
  uVar10 = 0;
  ExceptionList = &local_10;
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(*(undefined4 *)(param_1 + 8));
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(*(undefined4 *)(param_1 + 0xc));
  _WriteFloat_CDataOutputStream__QAEXM_Z(*(undefined4 *)(param_1 + 0x18));
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(*(undefined4 *)(param_1 + 0x1c));
  __0__CArray_E__QAE_XZ();
  local_8 = 0;
  __0CMemoryDataOutputStream__QAE_XZ();
  local_8 = CONCAT31(local_8._1_3_,1);
  local_4c = _Lock_CVertexBufferBase__QAEPAEKKK_Z(0,0,0);
  for (local_5c = 0; local_5c < 0x121; local_5c = local_5c + 1) {
    _WriteUWORD_CDataOutputStream__QAEXG_Z(*(undefined2 *)(local_4c + local_5c * 0x44));
    _WriteUWORD_CDataOutputStream__QAEXG_Z(*(undefined2 *)(local_4c + 2 + local_5c * 0x44));
    _WriteFloat_CDataOutputStream__QAEXM_Z(*(undefined4 *)(local_4c + 4 + local_5c * 0x44));
    _WriteSWORD_CDataOutputStream__QAEXF_Z(*(undefined2 *)(local_4c + 8 + local_5c * 0x44));
    _WriteSWORD_CDataOutputStream__QAEXF_Z(*(undefined2 *)(local_4c + 10 + local_5c * 0x44));
    uVar3 = _GetDepthFromTargetVertex_CWaterPatchMesh__AAEFABVCTVertexWaterForeground___Z
                      (local_5c * 0x44 + local_4c);
    _WriteSWORD_CDataOutputStream__QAEXF_Z(uVar3);
    _WriteFloat_CDataOutputStream__QAEXM_Z(*(undefined4 *)(local_4c + 0x10 + local_5c * 0x44));
    _Write_CMemoryDataOutputStream__UAEXPBXJ_Z(local_4c + 0x14 + local_5c * 0x44,0x30);
  }
  _Unlock_CVertexBufferBase__QAEXXZ();
  local_58 = 0x42;
  iVar4 = _GetLength_CMemoryDataOutputStream__QBEKXZ();
  if (iVar4 == 0x4a82) goto LAB_02e68ea4;
  if (DAT_04bac270 == 0) {
LAB_02e68e33:
    uVar9 = 1;
  }
  else {
    __0CWideString__QAE_PB_W_Z(L"Engine_Water_Patch.cpp");
    local_8 = CONCAT31(local_8._1_3_,2);
    uVar10 = uVar10 | 1;
    __0CCharString__QAE_PBDJ_Z(&DAT_044762a3,0xffffffff);
    local_8 = 3;
    uVar10 = uVar10 | 2;
    __0CCharString__QAE_PBDJ_Z
              ("saved_vertex_size*WATER_NUM_VERTS_IN_PATCH==vertex_stream.GetLength()",0xffffffff);
    local_8 = 4;
    uVar10 = uVar10 | 4;
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (local_78,local_70,local_68,0x1c7,1);
    if (cVar2 == '\x01') goto LAB_02e68e33;
    uVar9 = 0;
  }
  local_5d = (char)uVar9;
  local_8 = 3;
  if ((uVar10 & 4) != 0) {
    uVar10 = uVar10 & 0xfffffffb;
    __1CCharString__QAE_XZ();
  }
  local_8 = 2;
  if ((uVar10 & 2) != 0) {
    uVar10 = uVar10 & 0xfffffffd;
    __1CCharString__QAE_XZ();
  }
  local_8 = 1;
  if ((uVar10 & 1) != 0) {
    uVar10 = uVar10 & 0xfffffffe;
    __1CWideString__QAE_XZ();
  }
  if ((local_5d != '\0') && (BVar5 = IsDebuggerPresent(), BVar5 != 0)) {
    pcVar1 = (code *)swi(3);
    (*pcVar1)(uVar9,param_1,uVar10);
    return;
  }
LAB_02e68ea4:
  puVar8 = local_24;
  uVar7 = 0x121;
  uVar9 = local_58;
  uVar6 = _PeekData_CMemoryDataOutputStream__QBEPBXXZ(0x121,local_58,puVar8);
  local_50 = _Compress_CRangeCompressor__QAEJPBXJJAAV__CArray_E___Z(uVar6,uVar7,uVar9,puVar8);
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_50);
  uVar9 = _GetAsCArray___CArray_E__QAEPAEXZ(local_50);
  (**(code **)(*param_2 + 0x10))(uVar9);
  local_8 = local_8 & 0xffffff00;
  FID_conflict__time_put<char,std::ostreambuf_iterator<char,std::char_traits<char>_>_>();
  local_8 = 0xffffffff;
  __1__CArray_E__QAE_XZ();
  ExceptionList = local_10;
  return;
}


==================== ?Load@CWaterPatchMesh@@AAE_NAAVCDataInputStream@@@Z @ 02e673b0 ====================

/* WARNING: Removing unreachable block (ram,0x02e6770f) */
/* [ported from ego_r via strfp] */

undefined4 __fastcall _Load_CWaterPatchMesh__AAE_NAAVCDataInputStream___Z(int param_1)

{
  code *pcVar1;
  char cVar2;
  undefined2 uVar3;
  undefined4 uVar4;
  BOOL BVar5;
  float10 fVar6;
  undefined1 *puVar7;
  int iVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined1 local_a0 [8];
  undefined1 local_98 [8];
  undefined1 local_90 [11];
  char local_85;
  undefined1 local_84 [8];
  undefined1 local_7c [8];
  undefined1 local_74 [11];
  char local_69;
  int local_68;
  undefined4 local_64;
  int local_48;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e86b81;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  fVar6 = (float10)_ReadFloat_CDataInputStream__QAEMXZ();
  *(float *)(param_1 + 0x18) = (float)fVar6;
  uVar4 = _ReadSLONG_CDataInputStream__QAEJXZ();
  switch(uVar4) {
  case 0:
    *(undefined4 *)(param_1 + 0x1c) = 1;
    break;
  case 1:
    *(undefined4 *)(param_1 + 0x1c) = 1;
    break;
  case 2:
    *(undefined4 *)(param_1 + 0x1c) = 2;
    break;
  case 3:
    __0CWideString__QAE_PB_W_Z(L"Engine_Water_Patch.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z("Legacy Sea (water) type used.",0xffffffff);
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z(&DAT_04476252,0xffffffff);
    local_8._0_1_ = 2;
    uVar13 = 2;
    uVar4 = 0x43;
    puVar10 = local_74;
    puVar9 = local_7c;
    puVar7 = local_84;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar9,puVar10,0x43,2);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar7,puVar9,puVar10,uVar4,uVar13);
    local_69 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_69 != '\0') && (BVar5 = IsDebuggerPresent(), BVar5 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar4 = (*pcVar1)();
      return uVar4;
    }
    *(undefined4 *)(param_1 + 0x1c) = 3;
    break;
  case 4:
    *(undefined4 *)(param_1 + 0x1c) = 4;
    break;
  case 5:
    *(undefined4 *)(param_1 + 0x1c) = 5;
    break;
  case 6:
    *(undefined4 *)(param_1 + 0x1c) = 6;
    break;
  default:
    __0CWideString__QAE_PB_W_Z(L"Engine_Water_Patch.cpp");
    local_8 = 3;
    __0CCharString__QAE_PBDJ_Z("Unrecognised water type - assuming lake",0xffffffff);
    local_8._0_1_ = 4;
    __0CCharString__QAE_PBDJ_Z(&DAT_04476253,0xffffffff);
    local_8._0_1_ = 5;
    uVar13 = 1;
    uVar4 = 0x50;
    puVar10 = local_90;
    puVar9 = local_98;
    puVar7 = local_a0;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar9,puVar10,0x50,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar7,puVar9,puVar10,uVar4,uVar13);
    local_85 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 4;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,3);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_85 != '\0') && (BVar5 = IsDebuggerPresent(), BVar5 != 0)) {
      pcVar1 = (code *)swi(3);
      uVar4 = (*pcVar1)();
      return uVar4;
    }
    *(undefined4 *)(param_1 + 0x1c) = 1;
    break;
  case 8:
    *(undefined4 *)(param_1 + 0x1c) = 8;
  }
  uVar4 = _GetVertexBufferUsage_CEngineResourceManager__QAEKXZ();
  uVar12 = 0x44;
  uVar11 = 0x121;
  uVar13 = 0;
  iVar8 = param_1;
  _GetMemoryManager_CEngineLandscapeRenderer__QAEPAVCEngineLandscapeMemoryManager__XZ
            (param_1,0,0x121,0x44,uVar4);
  uVar4 = _AllocateVertexBuffer_CEngineLandscapeMemoryManager__QAEPAVCVertexBufferWin32__PAVCMovableResource__W4EVertexType__JJK_Z
                    (iVar8,uVar13,uVar11,uVar12,uVar4);
  *(undefined4 *)(param_1 + 0x10) = uVar4;
  if (*(int *)(param_1 + 0x10) == 0) {
    uVar4 = 0;
  }
  else {
    local_14 = _ReadSLONG_CDataInputStream__QAEJXZ();
    local_64 = 0x42;
    __0__CArray_E__QAE_I_Z(0x4a82);
    local_8 = 6;
    __0__CArray_E__QAE_I_Z(local_14);
    local_8._0_1_ = 7;
    uVar4 = local_14;
    uVar13 = _GetAsCArray___CArray_E__QAEPAEXZ(local_14);
    _Read_CDataInputStream__QAEXPAXJ_Z(uVar13,uVar4);
    uVar13 = _GetAsCArray___CArray_E__QAEPAEXZ();
    uVar12 = 0x121;
    uVar4 = local_64;
    uVar11 = _GetAsCArray___CArray_E__QAEPAEXZ(0x121,local_64,uVar13);
    _Decompress_CRangeCompressor__QAEXPBEJJPAX_Z(uVar11,uVar12,uVar4,uVar13);
    uVar4 = _size___vector_EV__allocator_E_std___std__QBEIXZ();
    uVar13 = _GetAsCArray___CArray_E__QAEPAEXZ(uVar4);
    __0CMemoryDataInputStream__QAE_PBXK_Z(uVar13,uVar4);
    local_8 = CONCAT31(local_8._1_3_,8);
    local_48 = _Lock_CVertexBufferBase__QAEPAEKKK_Z(0,0,0);
    for (local_68 = 0; local_68 < 0x121; local_68 = local_68 + 1) {
      uVar3 = _ReadUWORD_CDataInputStream__QAEGXZ();
      *(undefined2 *)(local_48 + local_68 * 0x44) = uVar3;
      uVar3 = _ReadUWORD_CDataInputStream__QAEGXZ();
      *(undefined2 *)(local_48 + 2 + local_68 * 0x44) = uVar3;
      fVar6 = (float10)_ReadFloat_CDataInputStream__QAEMXZ();
      *(float *)(local_48 + 4 + local_68 * 0x44) = (float)fVar6;
      uVar3 = _ReadSWORD_CDataInputStream__QAEFXZ();
      *(undefined2 *)(local_48 + 8 + local_68 * 0x44) = uVar3;
      uVar3 = _ReadSWORD_CDataInputStream__QAEFXZ();
      *(undefined2 *)(local_48 + 10 + local_68 * 0x44) = uVar3;
      uVar3 = _ReadSWORD_CDataInputStream__QAEFXZ();
      _SetTargetVertexDepth_CWaterPatchMesh__AAEXAAVCTVertexWaterForeground__F_Z
                (local_68 * 0x44 + local_48,uVar3);
      fVar6 = (float10)_ReadFloat_CDataInputStream__QAEMXZ();
      *(float *)(local_48 + 0x10 + local_68 * 0x44) = (float)fVar6;
      _Read_CDataInputStream__QAEXPAXJ_Z(local_48 + 0x14 + local_68 * 0x44,0x30);
    }
    _Unlock_CVertexBufferBase__QAEXXZ();
    local_8._0_1_ = 7;
    __1CMemoryDataInputStream__UAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,6);
    __1__CArray_E__QAE_XZ();
    local_8 = 0xffffffff;
    __1__CArray_E__QAE_XZ();
    uVar4 = 1;
  }
  ExceptionList = local_10;
  return uVar4;
}


==================== ??0CWaterPatchMesh@@QAE@PBVCEngineLandscapeMap@@JJ@Z @ 02e671f0 ====================

/* [ported from ego_r via strfp] */

undefined4 * __thiscall
__0CWaterPatchMesh__QAE_PBVCEngineLandscapeMap__JJ_Z
          (undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e86af8;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CMovableResource__QAE_XZ();
  local_8 = 0;
  *param_1 = CWaterPatchMesh::vftable;
  param_1[1] = param_2;
  __0C2DCoordI__QAE_JJ_Z(param_3,param_4);
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[7] = 0;
  ExceptionList = local_10;
  return param_1;
}


==================== ?FindCorrectWaterLevel@CWaterPatchMesh@@ABE_NAAMABVCWaterPatchDescriptors@@JJ@Z @ 02e67af0 ====================

/* WARNING: Removing unreachable block (ram,0x02e67bac) */
/* WARNING: Removing unreachable block (ram,0x02e67b8d) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

bool _FindCorrectWaterLevel_CWaterPatchMesh__ABE_NAAMABVCWaterPatchDescriptors__JJ_Z
               (float *param_1,int param_2,int param_3,int param_4)

{
  float fVar1;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_24;
  int local_20;
  float local_1c;
  float local_14;
  
  if (param_3 + -2 < 0) {
    local_2c = 0;
  }
  else {
    local_2c = param_3 + -2;
  }
  if (param_3 + 2 < 0x10) {
    local_30 = param_3 + 2;
  }
  else {
    local_30 = 0x10;
  }
  if (param_4 + -2 < 0) {
    local_34 = 0;
  }
  else {
    local_34 = param_4 + -2;
  }
  if (param_4 + 2 < 0x10) {
    local_38 = param_4 + 2;
  }
  else {
    local_38 = 0x10;
  }
  local_14 = 0.0;
  local_1c = 0.0;
  for (local_20 = local_2c; local_20 <= local_30; local_20 = local_20 + 1) {
    for (local_24 = local_34; local_24 <= local_38; local_24 = local_24 + 1) {
      if ((float)_DAT_0401df40 < *(float *)(local_20 * 0x44 + param_2 + local_24 * 4)) {
        local_14 = local_14 + *(float *)(local_20 * 0x44 + param_2 + local_24 * 4);
        local_1c = local_1c + (float)_DAT_0401df70;
      }
    }
  }
  fVar1 = (float)_DAT_0401df40;
  if (fVar1 < local_1c) {
    *param_1 = local_14 / local_1c;
  }
  return fVar1 < local_1c;
}


==================== ?BuildVertexBuffer@CWaterPatchMesh@@QAEXXZ @ 02e68320 ====================

/* WARNING: Removing unreachable block (ram,0x02e68485) */
/* WARNING: Removing unreachable block (ram,0x02e68460) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void __fastcall _BuildVertexBuffer_CWaterPatchMesh__QAEXXZ(int param_1)

{
  code *pcVar1;
  char cVar2;
  undefined2 uVar3;
  BOOL BVar4;
  float10 fVar5;
  float10 fVar6;
  undefined1 *puVar7;
  int iVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  int iVar15;
  undefined2 local_f4;
  undefined2 local_e8;
  double local_dc;
  double local_d4;
  double local_8c;
  double local_6c;
  undefined1 local_60 [8];
  undefined1 local_58 [8];
  undefined1 local_50 [11];
  char local_45;
  float local_44;
  float local_40;
  float local_3c;
  float local_38;
  undefined4 local_34;
  int local_30;
  int local_2c;
  float local_28;
  undefined2 *local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e86c48;
  local_10 = ExceptionList;
  if (*(int *)(param_1 + 0x10) == 0) {
    ExceptionList = &local_10;
    if (*(int *)(param_1 + 0x14) == 0) {
      ExceptionList = &local_10;
      __0CWideString__QAE_PB_W_Z(L"Engine_Water_Patch.cpp");
      local_8 = 0;
      __0CCharString__QAE_PBDJ_Z(&DAT_044762a2,0xffffffff);
      local_8._0_1_ = 1;
      __0CCharString__QAE_PBDJ_Z("VertexHeightData",0xffffffff);
      local_8._0_1_ = 2;
      uVar14 = 1;
      uVar12 = 0x14b;
      puVar10 = local_50;
      puVar9 = local_58;
      puVar7 = local_60;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar9,puVar10,0x14b,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar7,puVar9,puVar10,uVar12,uVar14);
      local_45 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 1;
      __1CCharString__QAE_XZ();
      local_8 = (uint)local_8._1_3_ << 8;
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CWideString__QAE_XZ();
      if ((local_45 != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    uVar12 = _GetVertexBufferUsage_CEngineResourceManager__QAEKXZ();
    uVar13 = 0x44;
    uVar11 = 0x121;
    uVar14 = 0;
    iVar8 = param_1;
    _GetMemoryManager_CEngineLandscapeRenderer__QAEPAVCEngineLandscapeMemoryManager__XZ
              (param_1,0,0x121,0x44,uVar12);
    uVar12 = _AllocateVertexBuffer_CEngineLandscapeMemoryManager__QAEPAVCVertexBufferWin32__PAVCMovableResource__W4EVertexType__JJK_Z
                       (iVar8,uVar14,uVar11,uVar13,uVar12);
    *(undefined4 *)(param_1 + 0x10) = uVar12;
    uVar12 = _GetSize_CVertexBufferBase__QBEJXZ();
    local_18 = _Lock_CVertexBufferBase__QAEPAEKKK_Z(0,0,uVar12);
    local_14 = 0;
    for (local_1c = 0; local_1c < 0x11; local_1c = local_1c + 1) {
      for (local_20 = 0; local_20 < 0x11; local_20 = local_20 + 1) {
        local_38 = *(float *)(local_1c * 0x44 + *(int *)(param_1 + 0x14) + local_20 * 4);
        local_30 = local_1c + *(int *)(param_1 + 8);
        local_2c = local_20 + *(int *)(param_1 + 0xc);
        _PeekEngineMap_CEngineLandscapeMap__QBEPBVCEngineMap__XZ();
        uVar12 = _PeekWorldPosition_CEngineMap__QBEABVC3DVector__XZ();
        __0C3DVector__QAE_ABV0__Z(uVar12);
        local_44 = (float)local_30 + local_44;
        local_40 = (float)local_2c + local_40;
        if (*(int *)(param_1 + 0x1c) == 8) {
          if ((local_3c + local_38) - _DAT_0447621c <= (float)_DAT_0401df40) {
            local_6c = 0.0;
          }
          else {
            local_6c = (double)((local_3c + local_38) - _DAT_0447621c);
          }
          fVar5 = (float10)((longlong)ROUND(local_6c * _DAT_0417cc80) & 0xffffffff) /
                  (float10)_DAT_0417cc80;
        }
        else {
          if ((local_3c + local_38) - (float)_DAT_04022078 <= (float)_DAT_0401df40) {
            local_8c = 0.0;
          }
          else {
            local_8c = (double)((local_3c + local_38) - (float)_DAT_04022078);
          }
          fVar5 = (float10)((longlong)ROUND(local_8c * _DAT_0417cc80) & 0xffffffff) /
                  (float10)_DAT_0417cc80;
        }
        local_3c = (float)fVar5;
        local_24 = (undefined2 *)(local_14 * 0x44 + local_18);
        local_14 = local_14 + 1;
        fVar5 = (float10)_GFSin__YINM_Z(local_44 / (float)_DAT_04071768);
        fVar6 = (float10)_GFSin__YINM_Z(local_40 / (float)_DAT_04071768);
        uVar3 = _GFFloatToLongNear__YIJM_Z
                          ((float)(((fVar6 + (float10)(double)fVar5) * (float10)_DAT_0434bb98) /
                                  (float10)_DAT_0401df60));
        local_24[4] = uVar3;
        fVar5 = (float10)_GFCos__YINM_Z(local_44 / (float)_DAT_04071768);
        fVar6 = (float10)_GFCos__YINM_Z(local_40 / (float)_DAT_04071768);
        uVar3 = _GFFloatToLongNear__YIJM_Z
                          ((float)(((fVar6 + (float10)(double)fVar5) * (float10)_DAT_0434bb98) /
                                  (float10)_DAT_0401df60));
        local_24[5] = uVar3;
        iVar8 = local_30;
        iVar15 = local_2c;
        _PeekEngineMap_CEngineLandscapeMap__QBEPBVCEngineMap__XZ(local_30,local_2c);
        fVar5 = (float10)_PeekLandscapeHeight_CEngineMap__QBEMJJ_Z(iVar8,iVar15);
        local_28 = (float)fVar5;
        if ((float)_DAT_0401df40 <= (local_3c + (float)_DAT_04022078) - local_28) {
          if ((local_3c + (float)_DAT_04022078) - local_28 <= (float)_DAT_0401df60) {
            local_dc = (double)((local_3c + (float)_DAT_04022078) - local_28);
          }
          else {
            local_dc = _DAT_0401df60;
          }
          local_d4 = local_dc;
        }
        else {
          local_d4 = 0.0;
        }
        *(float *)(local_24 + 6) = (float)local_d4;
        local_34 = _GetWaterGenerator_CEngineWaterRenderer__QAEPAVCWaterGenerator__XZ();
        _ConstructVertexDistanceToShoreArray_CWaterGenerator__QAE_NABVC3DVector__PAMJAAM2_Z
                  (&local_44,local_24 + 10,0xc,param_1 + 0x18,local_24 + 8);
        local_e8 = (undefined2)(int)ROUND(local_44);
        *local_24 = local_e8;
        local_f4 = (undefined2)(int)ROUND(local_40);
        local_24[1] = local_f4;
        *(float *)(local_24 + 2) = local_3c;
      }
    }
    _Unlock_CVertexBufferBase__QAEXXZ();
  }
  ExceptionList = local_10;
  return;
}


==================== ?SetTargetVertexDepth@CWaterPatchMesh@@AAEXAAVCTVertexWaterForeground@@F@Z @ 02e679a0 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void _SetTargetVertexDepth_CWaterPatchMesh__AAEXAAVCTVertexWaterForeground__F_Z
               (int param_1,short param_2)

{
  undefined8 local_20;
  undefined8 local_14;
  
  if (_DAT_0401df40 <= ((double)(int)param_2 + (double)(int)param_2) / _DAT_0434bb98) {
    if (((double)(int)param_2 + (double)(int)param_2) / _DAT_0434bb98 <= _DAT_0401df60) {
      local_20 = ((double)(int)param_2 + (double)(int)param_2) / _DAT_0434bb98;
    }
    else {
      local_20 = _DAT_0401df60;
    }
    local_14 = local_20;
  }
  else {
    local_14 = 0.0;
  }
  *(float *)(param_1 + 0xc) = (float)local_14;
  return;
}


==================== ?GetDepthFromTargetVertex@CWaterPatchMesh@@AAEFABVCTVertexWaterForeground@@@Z @ 02e68fe0 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void _GetDepthFromTargetVertex_CWaterPatchMesh__AAEFABVCTVertexWaterForeground___Z(int param_1)

{
  undefined4 local_10;
  undefined4 local_c;
  
  if ((float)_DAT_0401df40 <= *(float *)(param_1 + 0xc)) {
    if (*(float *)(param_1 + 0xc) <= (float)_DAT_0401df60) {
      local_10 = *(float *)(param_1 + 0xc);
    }
    else {
      local_10 = _DAT_0402a320;
    }
    local_c = local_10;
  }
  else {
    local_c = 0.0;
  }
  _GFFloatToLongNear__YIJM_Z((local_c * (float)_DAT_0434bb98) / (float)_DAT_0401df60);
  return;
}


==================== ?AddForegroundPatch@CEngineWaterRenderer@@QAEPAVCWaterPatchMesh@@PAVCMovableResource@@PBVCEngineLandscapeMap@@JJABVCWaterPatchDescriptors@@@Z @ 02d54b40 ====================

/* [ported from ego_r via strfp] */

undefined4
_AddForegroundPatch_CEngineWaterRenderer__QAEPAVCWaterPatchMesh__PAVCMovableResource__PBVCEngineLandscapeMap__JJABVCWaterPatchDescriptors___Z
          (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5)

{
  code *pcVar1;
  char cVar2;
  undefined4 uVar3;
  BOOL BVar4;
  undefined4 uVar5;
  char local_48;
  undefined4 extraout_var;
  uint uVar6;
  undefined1 local_38 [8];
  undefined1 local_30 [8];
  undefined1 local_28 [11];
  char local_1d;
  int local_1c;
  undefined4 local_18;
  undefined4 local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e6c1f6;
  local_10 = ExceptionList;
  uVar6 = 0;
  ExceptionList = &local_10;
  local_1c = __2CWaterPatchMesh__SIPAXIPAVCMovableResource___Z();
  local_8 = 0;
  uVar5 = extraout_var;
  if (local_1c == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = __0CWaterPatchMesh__QAE_PBVCEngineLandscapeMap__JJ_Z(param_2,param_3,param_4);
  }
  local_8 = 0xffffffff;
  local_18 = uVar3;
  local_14 = uVar3;
  cVar2 = _Build_CWaterPatchMesh__QAE_NABVCWaterPatchDescriptors___Z(param_5);
  if (cVar2 != '\0') {
    ExceptionList = local_10;
    return local_14;
  }
  if (DAT_04bac270 != 0) {
    __0CWideString__QAE_PB_W_Z(L"Engine_Water_Renderer.cpp");
    local_8 = 1;
    uVar6 = uVar6 | 1;
    __0CCharString__QAE_PBDJ_Z(&DAT_0444e556,0xffffffff);
    local_8 = 2;
    uVar6 = uVar6 | 2;
    __0CCharString__QAE_PBDJ_Z("patch->Build(water_data)",0xffffffff);
    local_8 = 3;
    uVar6 = uVar6 | 4;
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (local_38,local_30,local_28,0x676,1);
    if (cVar2 != '\x01') {
      local_48 = '\0';
      goto LAB_02d54c6a;
    }
  }
  local_48 = '\x01';
LAB_02d54c6a:
  local_1d = local_48;
  local_8 = 2;
  if ((uVar6 & 4) != 0) {
    uVar6 = uVar6 & 0xfffffffb;
    __1CCharString__QAE_XZ();
  }
  local_8 = 1;
  if ((uVar6 & 2) != 0) {
    uVar6 = uVar6 & 0xfffffffd;
    __1CCharString__QAE_XZ();
  }
  local_8 = 0xffffffff;
  if ((uVar6 & 1) != 0) {
    uVar6 = uVar6 & 0xfffffffe;
    __1CWideString__QAE_XZ();
  }
  if ((local_1d != '\0') && (BVar4 = IsDebuggerPresent(), BVar4 != 0)) {
    pcVar1 = (code *)swi(3);
    uVar5 = (*pcVar1)(uVar3,uVar5,uVar6);
    return uVar5;
  }
  ExceptionList = local_10;
  return local_14;
}


==================== ?LoadPatch@CEngineWaterRenderer@@QAEPAVCWaterPatchMesh@@PAVCMovableResource@@PBVCEngineLandscapeMap@@AAVCDataInputStream@@@Z @ 02dfcf50 ====================

/* [ported from ego_r via strfp] */

undefined4
_LoadPatch_CEngineWaterRenderer__QAEPAVCWaterPatchMesh__PAVCMovableResource__PBVCEngineLandscapeMap__AAVCDataInputStream___Z
          (undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 local_34;
  undefined1 local_1c [12];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e7c77b;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  uVar1 = _ReadSLONG_CDataInputStream__QAEJXZ();
  uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
  __0C2DCoordI__QAE_JJ_Z(uVar1,uVar2);
  iVar3 = __2CWaterPatchMesh__SIPAXIPAVCMovableResource___Z();
  local_8 = 0;
  if (iVar3 == 0) {
    local_34 = 0;
  }
  else {
    local_34 = __0CWaterPatchMesh__QAE_PBVCEngineLandscapeMap__AAVCDataInputStream__AAVC2DCoordI___Z
                         (param_2,param_3,local_1c);
  }
  ExceptionList = local_10;
  return local_34;
}


==================== ?Save@CEngineWaterBackgroundSubPatch@@QBEXAAVCDataOutputStream@@GGJJ@Z @ 02e055e0 ====================

/* [ported from ego_r via strfp] */

void __thiscall
_Save_CEngineWaterBackgroundSubPatch__QBEXAAVCDataOutputStream__GGJJ_Z(int param_1,int *param_2)

{
  code *pcVar1;
  char cVar2;
  BOOL BVar3;
  undefined1 *puVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined1 local_70 [8];
  undefined1 local_68 [8];
  undefined1 local_60 [11];
  char local_55;
  undefined1 local_54 [8];
  undefined1 local_4c [8];
  undefined1 local_44 [11];
  char local_39;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_2c;
  undefined4 local_28;
  undefined1 local_24 [20];
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e7cec8;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _WriteUWORD_CDataOutputStream__QAEXG_Z(*(undefined2 *)(param_1 + 0x32));
  _WriteUWORD_CDataOutputStream__QAEXG_Z(*(undefined2 *)(param_1 + 0x30));
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(*(undefined4 *)(param_1 + 0x40));
  if (*(short *)(param_1 + 0x32) == 0) {
    __0CWideString__QAE_PB_W_Z(L"Engine_Water_Background_Sub_Patch.cpp");
    local_8 = 0;
    __0CCharString__QAE_PBDJ_Z(&DAT_044689bc,0xffffffff);
    local_8._0_1_ = 1;
    __0CCharString__QAE_PBDJ_Z("VertexCount",0xffffffff);
    local_8._0_1_ = 2;
    uVar8 = 1;
    uVar7 = 0x9a;
    puVar6 = local_44;
    puVar5 = local_4c;
    puVar4 = local_54;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar4,puVar5,puVar6,0x9a,1);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar4,puVar5,puVar6,uVar7,uVar8);
    local_39 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 1;
    __1CCharString__QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if ((local_39 != '\0') && (BVar3 = IsDebuggerPresent(), BVar3 != 0)) {
      pcVar1 = (code *)swi(3);
      (*pcVar1)();
      return;
    }
  }
  switch(*(undefined4 *)(param_1 + 0x40)) {
  case 1:
  case 2:
  case 6:
  case 8:
    local_34 = 0x38;
    break;
  case 3:
  case 4:
  case 5:
    local_34 = 0xc;
    break;
  default:
    __0CWideString__QAE_PB_W_Z(L"Engine_Water_Background_Sub_Patch.cpp");
    local_8 = 3;
    __0CCharString__QAE_PBDJ_Z("Unsupported water type",0xffffffff);
    local_8._0_1_ = 4;
    __0CCharString__QAE_PBDJ_Z(&DAT_044689bd,0xffffffff);
    local_8._0_1_ = 5;
    uVar8 = 2;
    uVar7 = 0xad;
    puVar6 = local_60;
    puVar5 = local_68;
    puVar4 = local_70;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar4,puVar5,puVar6,0xad,2);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar4,puVar5,puVar6,uVar7,uVar8);
    local_55 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 4;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,3);
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    __1CWideString__QAE_XZ();
    if (local_55 == '\0') {
      ExceptionList = local_10;
      return;
    }
    BVar3 = IsDebuggerPresent();
    if (BVar3 == 0) {
      ExceptionList = local_10;
      return;
    }
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
  __0__CArray_E__QAE_XZ();
  local_8 = 6;
  local_28 = _Lock_CVertexBufferBase__QAEPAEKKK_Z(0,0,0);
  local_2c = _Compress_CRangeCompressor__QAEJPBXJJAAV__CArray_E___Z
                       (local_28,*(undefined2 *)(param_1 + 0x32),local_34,local_24);
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_34);
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_2c);
  uVar7 = _GetAsCArray___CArray_E__QAEPAEXZ(local_2c);
  (**(code **)(*param_2 + 0x10))(uVar7);
  _Unlock_CVertexBufferBase__QAEXXZ();
  if (*(short *)(param_1 + 0x30) != 0) {
    local_38 = _Lock_CIndexBuffer__QAEPAEKKK_Z(0,0,0);
    local_2c = _Compress_CRangeCompressor__QAEJPBXJJAAV__CArray_E___Z
                         (local_38,(uint)*(ushort *)(param_1 + 0x30) * 3,2,local_24);
    _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_2c);
    uVar7 = _GetAsCArray___CArray_E__QAEPAEXZ(local_2c);
    (**(code **)(*param_2 + 0x10))(uVar7);
    _Unlock_CIndexBuffer__QAEXXZ();
  }
  local_8 = 0xffffffff;
  __1__CArray_E__QAE_XZ();
  ExceptionList = local_10;
  return;
}


==================== ?Load@CEngineWaterBackgroundSubPatch@@QAE_NAAVCDataInputStream@@GGJJMM@Z @ 02e052b0 ====================

/* [ported from ego_r via strfp] */

undefined4 __thiscall
_Load_CEngineWaterBackgroundSubPatch__QAE_NAAVCDataInputStream__GGJJMM_Z
          (int param_1,undefined4 param_2,undefined2 param_3,undefined2 param_4,undefined4 param_5,
          undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  ushort uVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e7ce78;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  *(undefined4 *)(param_1 + 0x4c) = param_5;
  *(undefined4 *)(param_1 + 0x50) = param_6;
  *(undefined2 *)(param_1 + 0x54) = param_3;
  *(undefined2 *)(param_1 + 0x56) = param_4;
  *(undefined4 *)(param_1 + 0x58) = param_7;
  *(undefined4 *)(param_1 + 0x5c) = param_8;
  uVar2 = _ReadUWORD_CDataInputStream__QAEGXZ();
  *(undefined2 *)(param_1 + 0x32) = uVar2;
  uVar2 = _ReadUWORD_CDataInputStream__QAEGXZ();
  *(undefined2 *)(param_1 + 0x30) = uVar2;
  if (*(int *)(param_1 + 0xc) != 0) {
    uVar3 = *(undefined4 *)(param_1 + 0xc);
    _GetMemoryManager_CEngineLandscapeRenderer__QAEPAVCEngineLandscapeMemoryManager__XZ(uVar3);
    _DeleteVertexBuffer_CEngineLandscapeMemoryManager__QAEXPAVCVertexBufferWin32___Z(uVar3);
  }
  uVar3 = _ReadSLONG_CDataInputStream__QAEJXZ();
  *(undefined4 *)(param_1 + 0x40) = uVar3;
  uVar4 = _ReadSLONG_CDataInputStream__QAEJXZ();
  uVar5 = _GetVertexBufferUsage_CEngineResourceManager__QAEKXZ();
  uVar7 = (uint)*(ushort *)(param_1 + 0x32);
  uVar10 = 0;
  iVar9 = param_1;
  uVar3 = uVar4;
  _GetMemoryManager_CEngineLandscapeRenderer__QAEPAVCEngineLandscapeMemoryManager__XZ
            (param_1,0,uVar7,uVar4,uVar5);
  uVar3 = _AllocateVertexBuffer_CEngineLandscapeMemoryManager__QAEPAVCVertexBufferWin32__PAVCMovableResource__W4EVertexType__JJK_Z
                    (iVar9,uVar10,uVar7,uVar3,uVar5);
  *(undefined4 *)(param_1 + 0xc) = uVar3;
  if (*(int *)(param_1 + 0xc) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = _Lock_CVertexBufferBase__QAEPAEKKK_Z(0,0,0);
    uVar5 = _ReadSLONG_CDataInputStream__QAEJXZ();
    __0__CArray_E__QAE_I_Z(uVar5);
    local_8 = 0;
    uVar10 = _GetAsCArray___CArray_E__QAEPAEXZ(uVar5);
    _Read_CDataInputStream__QAEXPAXJ_Z(uVar10,uVar5);
    uVar7 = (uint)*(ushort *)(param_1 + 0x32);
    uVar5 = _GetAsCArray___CArray_E__QAEPAEXZ(uVar7,uVar4,uVar3);
    _Decompress_CRangeCompressor__QAEXPBEJJPAX_Z(uVar5,uVar7,uVar4,uVar3);
    _Unlock_CVertexBufferBase__QAEXXZ();
    if (*(int *)(param_1 + 8) != 0) {
      uVar3 = *(undefined4 *)(param_1 + 8);
      _GetMemoryManager_CEngineLandscapeRenderer__QAEPAVCEngineLandscapeMemoryManager__XZ(uVar3);
      _DeleteIndexBuffer_CEngineLandscapeMemoryManager__QAEXPAVCIndexBuffer___Z(uVar3);
    }
    uVar1 = *(ushort *)(param_1 + 0x30);
    iVar6 = (uint)uVar1 * 3;
    uVar5 = 4;
    uVar4 = 0x65;
    uVar3 = _GetIndexBufferUsage_CEngineResourceManager__QAEKXZ(0x65,4);
    iVar8 = (uint)uVar1 * 6;
    iVar9 = param_1;
    _GetMemoryManager_CEngineLandscapeRenderer__QAEPAVCEngineLandscapeMemoryManager__XZ
              (param_1,iVar8,uVar3);
    uVar3 = _AllocateIndexBuffer_CEngineLandscapeMemoryManager__QAEPAVCIndexBuffer__PAVCMovableResource__JKW4EIndexBufferFormat__W4EPrimitiveType___Z
                      (iVar9,iVar8,uVar3,uVar4,uVar5);
    *(undefined4 *)(param_1 + 8) = uVar3;
    if (*(int *)(param_1 + 8) == 0) {
      local_8 = 0xffffffff;
      __1__CArray_E__QAE_XZ();
      uVar3 = 0;
    }
    else {
      if (iVar6 != 0) {
        uVar3 = _Lock_CIndexBuffer__QAEPAEKKK_Z(0,0,0);
        uVar4 = _ReadSLONG_CDataInputStream__QAEJXZ();
        _resize___vector_EV__allocator_E_std___std__QAEXI_Z(uVar4);
        uVar5 = _GetAsCArray___CArray_E__QAEPAEXZ(uVar4);
        _Read_CDataInputStream__QAEXPAXJ_Z(uVar5,uVar4);
        uVar5 = 2;
        uVar4 = _GetAsCArray___CArray_E__QAEPAEXZ(iVar6,2,uVar3);
        _Decompress_CRangeCompressor__QAEXPBEJJPAX_Z(uVar4,iVar6,uVar5,uVar3);
        _Unlock_CIndexBuffer__QAEXXZ();
      }
      *(undefined4 *)(param_1 + 0x48) = 0;
      local_8 = 0xffffffff;
      __1__CArray_E__QAE_XZ();
      uVar3 = 1;
    }
  }
  ExceptionList = local_10;
  return uVar3;
}


==================== ?BuildStaticMapBackgroundBuffers@CWaterGenerator@@QAEXPBVCEngineMap@@JPAVCEngineWaterBackgroundSubPatch@@JPBVCTVertexLandscapeBackground@@JPBGPAJ4PAVCTVertexWaterBackground@@@Z @ 02e067c0 ====================

/* WARNING: Removing unreachable block (ram,0x02e06f45) */
/* WARNING: Removing unreachable block (ram,0x02e06dcc) */
/* WARNING: Removing unreachable block (ram,0x02e06aa7) */
/* WARNING: Removing unreachable block (ram,0x02e06a78) */
/* WARNING: Removing unreachable block (ram,0x02e06cef) */
/* WARNING: Removing unreachable block (ram,0x02e06b3d) */
/* WARNING: Removing unreachable block (ram,0x02e07032) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void _BuildStaticMapBackgroundBuffers_CWaterGenerator__QAEXPBVCEngineMap__JPAVCEngineWaterBackgroundSubPatch__JPBVCTVertexLandscapeBackground__JPBGPAJ4PAVCTVertexWaterBackground___Z
               (undefined4 param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
               int param_7,int param_8,int param_9,int param_10)

{
  code *pcVar1;
  char cVar2;
  BOOL BVar3;
  int iVar4;
  undefined2 *puVar5;
  float10 fVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined1 local_148 [8];
  undefined1 local_140 [8];
  undefined1 local_138 [11];
  char local_12d;
  undefined1 local_12c [8];
  undefined1 local_124 [8];
  undefined1 local_11c [11];
  char local_111;
  undefined1 local_110 [8];
  undefined1 local_108 [8];
  undefined1 local_100 [11];
  char local_f5;
  undefined1 local_f4 [8];
  undefined1 local_ec [8];
  undefined1 local_e4 [11];
  char local_d9;
  float *local_d8;
  short *local_d4;
  int local_d0;
  int local_b8;
  int local_b4;
  int local_b0;
  int local_ac;
  undefined1 local_a8 [12];
  undefined1 local_9c [4];
  undefined1 local_98 [4];
  int local_94;
  short *local_90;
  short *local_8c;
  int local_88;
  uint local_84;
  int local_80;
  undefined4 local_7c [4];
  undefined4 local_6c;
  undefined4 local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  int local_54;
  int local_50 [15];
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e7d0d7;
  local_10 = ExceptionList;
  if (0 < param_4) {
    ExceptionList = &local_10;
    if (param_3 == 0) {
      ExceptionList = &local_10;
      __0CWideString__QAE_PB_W_Z(L"Engine_Water_Generator.cpp");
      local_8 = 0;
      __0CCharString__QAE_PBDJ_Z(&DAT_044690ba,0xffffffff);
      local_8._0_1_ = 1;
      __0CCharString__QAE_PBDJ_Z("patch",0xffffffff);
      local_8._0_1_ = 2;
      uVar11 = 1;
      uVar10 = 0x59;
      puVar9 = local_e4;
      puVar8 = local_ec;
      puVar7 = local_f4;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8,puVar9,0x59,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar7,puVar8,puVar9,uVar10,uVar11);
      local_d9 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 1;
      __1CCharString__QAE_XZ();
      local_8 = (uint)local_8._1_3_ << 8;
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CWideString__QAE_XZ();
      if ((local_d9 != '\0') && (BVar3 = IsDebuggerPresent(), BVar3 != 0)) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    if (param_4 < 3) {
      __0CWideString__QAE_PB_W_Z(L"Engine_Water_Generator.cpp");
      local_8 = 3;
      __0CCharString__QAE_PBDJ_Z(&DAT_044690bb,0xffffffff);
      local_8._0_1_ = 4;
      __0CCharString__QAE_PBDJ_Z("vertex_count > 2",0xffffffff);
      local_8._0_1_ = 5;
      uVar11 = 1;
      uVar10 = 0x5a;
      puVar9 = local_100;
      puVar8 = local_108;
      puVar7 = local_110;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8,puVar9,0x5a,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar7,puVar8,puVar9,uVar10,uVar11);
      local_f5 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 4;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,3);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CWideString__QAE_XZ();
      if ((local_f5 != '\0') && (BVar3 = IsDebuggerPresent(), BVar3 != 0)) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    if ((param_6 < 3) || (param_6 % 3 != 0)) {
      __0CWideString__QAE_PB_W_Z(L"Engine_Water_Generator.cpp");
      local_8 = 6;
      __0CCharString__QAE_PBDJ_Z(&DAT_044690c2,0xffffffff);
      local_8._0_1_ = 7;
      __0CCharString__QAE_PBDJ_Z("index_count > 2 && index_count % 3 == 0",0xffffffff);
      local_8._0_1_ = 8;
      uVar11 = 1;
      uVar10 = 0x5b;
      puVar9 = local_11c;
      puVar8 = local_124;
      puVar7 = local_12c;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8,puVar9,0x5b,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar7,puVar8,puVar9,uVar10,uVar11);
      local_111 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 7;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,6);
      __1CCharString__QAE_XZ();
      local_8 = 0xffffffff;
      __1CWideString__QAE_XZ();
      if ((local_111 != '\0') && (BVar3 = IsDebuggerPresent(), BVar3 != 0)) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    for (local_5c = 0; local_5c < param_4; local_5c = local_5c + 1) {
      *(undefined4 *)(param_8 + local_5c * 4) = 0xffffffff;
    }
    for (local_60 = 0; local_60 < param_6; local_60 = local_60 + 1) {
      *(undefined4 *)(param_9 + local_60 * 4) = 0xffffffff;
    }
    local_54 = 0;
    local_58 = 0;
    _PeekWorldPosition_CEngineMap__QBEABVC3DVector__XZ();
    local_14 = __ftol2_sse();
    _PeekWorldPosition_CEngineMap__QBEABVC3DVector__XZ();
    local_50[9] = __ftol2_sse();
    __0__CArray_G__QAE_I_Z(param_6);
    local_8 = 9;
    local_50[0] = 0;
    local_50[1] = 0;
    local_50[2] = 0;
    local_50[3] = 0;
    local_50[4] = 0;
    local_50[5] = 0;
    local_50[6] = 0;
    local_50[7] = 0;
    local_50[8] = 0;
    for (local_64 = 0; local_64 < param_6; local_64 = local_64 + 3) {
      __0C2DCoordI__QAE_JJ_Z
                ((uint)*(ushort *)(param_5 + (uint)*(ushort *)(param_7 + local_64 * 2) * 0x18) -
                 local_14,(uint)*(ushort *)
                                 (param_5 + 2 + (uint)*(ushort *)(param_7 + local_64 * 2) * 0x18) -
                          local_50[9]);
      __0C2DCoordI__QAE_JJ_Z
                ((uint)*(ushort *)(param_5 + (uint)*(ushort *)(param_7 + 2 + local_64 * 2) * 0x18) -
                 local_14,(uint)*(ushort *)
                                 (param_5 + 2 + (uint)*(ushort *)(param_7 + 2 + local_64 * 2) * 0x18
                                 ) - local_50[9]);
      __0C2DCoordI__QAE_JJ_Z
                ((uint)*(ushort *)(param_5 + (uint)*(ushort *)(param_7 + 4 + local_64 * 2) * 0x18) -
                 local_14,(uint)*(ushort *)
                                 (param_5 + 2 + (uint)*(ushort *)(param_7 + 4 + local_64 * 2) * 0x18
                                 ) - local_50[9]);
      cVar2 = _PeekInterpolatedHasWaterFast_CEngineMap__QBE_NJJJ_Z(local_7c[0],local_7c[1],1);
      if (((cVar2 != '\0') ||
          (cVar2 = _PeekInterpolatedHasWaterFast_CEngineMap__QBE_NJJJ_Z(local_7c[2],local_7c[3],1),
          cVar2 != '\0')) ||
         (cVar2 = _PeekInterpolatedHasWaterFast_CEngineMap__QBE_NJJJ_Z(local_6c,local_68,1),
         cVar2 != '\0')) {
        iVar4 = _PeekInterpolatedWaterType_CEngineMap__QBE_AW4EWaterType__JJJ_Z
                          (local_7c[0],local_7c[1],1);
        local_50[iVar4] = local_50[iVar4] + 1;
        iVar4 = _PeekInterpolatedWaterType_CEngineMap__QBE_AW4EWaterType__JJJ_Z
                          (local_7c[2],local_7c[3],1);
        local_50[iVar4] = local_50[iVar4] + 1;
        iVar4 = _PeekInterpolatedWaterType_CEngineMap__QBE_AW4EWaterType__JJJ_Z(local_6c,local_68,1)
        ;
        local_50[iVar4] = local_50[iVar4] + 1;
        for (local_80 = 0; iVar4 = local_58, local_80 < 3; local_80 = local_80 + 1) {
          local_84 = (uint)*(ushort *)(param_7 + (local_64 + local_80) * 2);
          local_88 = *(int *)(param_8 + local_84 * 4);
          if (local_88 < 0) {
            local_90 = (short *)(local_84 * 0x18 + param_5);
            local_88 = local_58;
            local_58 = local_58 + 1;
            local_8c = (short *)(iVar4 * 0x38 + param_10);
            *local_8c = *local_90;
            local_8c[1] = local_90[1];
            local_94 = param_2 << 1;
            fVar6 = (float10)_PeekInterpolatedWaterHeight_CEngineMap__QBEMJJJ_Z
                                       (local_7c[local_80 * 2],local_7c[local_80 * 2 + 1],local_94);
            *(float *)(local_8c + 2) = (float)fVar6;
            for (local_ac = 0;
                (*(float *)(local_8c + 2) < (float)_DAT_0401df40 !=
                 (*(float *)(local_8c + 2) == (float)_DAT_0401df40) && (local_ac < 2));
                local_ac = local_ac + 1) {
              fVar6 = (float10)_PeekInterpolatedWaterHeight_CEngineMap__QBEMJJJ_Z
                                         (local_7c[((local_80 + 1 + local_ac) % 3) * 2],
                                          local_7c[((local_80 + 1 + local_ac) % 3) * 2 + 1],local_94
                                         );
              *(float *)(local_8c + 2) = (float)fVar6;
            }
            __0C3DVector__QAE_MMM_Z
                      ((float)(int)*local_8c,(float)(int)local_8c[1],*(undefined4 *)(local_8c + 2));
            _ConstructVertexDistanceToShoreArray_CWaterGenerator__QAE_NABVC3DVector__PAMJAAM2_Z
                      (local_a8,local_8c + 4,0xc,local_98,local_9c);
            *(int *)(param_8 + local_84 * 4) = local_88;
          }
          *(int *)(param_9 + (local_64 + local_80) * 4) = local_54;
          puVar5 = (undefined2 *)__A__CArray_G__QAEAAGI_Z(local_54);
          *puVar5 = (undefined2)local_88;
          local_54 = local_54 + 1;
        }
      }
    }
    if (0 < local_58) {
      local_b0 = 0;
      local_b4 = 0;
      for (local_b8 = 0; local_b8 < 9; local_b8 = local_b8 + 1) {
        if ((local_b8 != 0) && (local_b0 < local_50[local_b8])) {
          local_b0 = local_50[local_b8];
          local_b4 = local_b8;
        }
      }
      _SetWaterType_CEngineWaterBackgroundSubPatch__QAEXW4EWaterType___Z(local_b4);
      iVar4 = local_54;
      uVar10 = _GetAsCArray___CArray_G__QAEPAGXZ(local_54);
      _SetIndexBuffer_CEngineWaterBackgroundSubPatch__QAEXPAGK_Z(uVar10,iVar4);
      switch(local_b4) {
      case 1:
      case 2:
      case 6:
      case 8:
        _SetVertexBuffer_CEngineWaterBackgroundSubPatch__QAEXPAEJK_Z(param_10,0x38,local_58);
        break;
      case 3:
      case 4:
      case 5:
        __0__CArray_VCTVertexSeaBackground____QAE_I_Z(local_58);
        local_8._0_1_ = 10;
        for (local_d0 = 0; local_d0 < local_58; local_d0 = local_d0 + 1) {
          local_d8 = (float *)__A__CArray_VCTVertexSeaBackground____QAEAAVCTVertexSeaBackground__I_Z
                                        (local_d0);
          local_d4 = (short *)(local_d0 * 0x38 + param_10);
          *local_d8 = (float)(int)*local_d4;
          local_d8[1] = (float)(int)local_d4[1];
          local_d8[2] = *(float *)(local_d4 + 2);
        }
        uVar11 = 0xc;
        iVar4 = local_58;
        uVar10 = _GetAsCArray___CArray_VCTVertexSeaBackground____QAEPAVCTVertexSeaBackground__XZ
                           (0xc,local_58);
        _SetVertexBuffer_CEngineWaterBackgroundSubPatch__QAEXPAEJK_Z(uVar10,uVar11,iVar4);
        local_8 = CONCAT31(local_8._1_3_,9);
        __1__CArray_VCTVertexSeaBackground____QAE_XZ();
        break;
      default:
        __0CWideString__QAE_PB_W_Z(L"Engine_Water_Generator.cpp");
        local_8._0_1_ = 0xb;
        __0CCharString__QAE_PBDJ_Z("Unsupported water type",0xffffffff);
        local_8._0_1_ = 0xc;
        __0CCharString__QAE_PBDJ_Z(&DAT_044690c3,0xffffffff);
        local_8._0_1_ = 0xd;
        uVar11 = 2;
        uVar10 = 0xd2;
        puVar9 = local_138;
        puVar8 = local_140;
        puVar7 = local_148;
        _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8,puVar9,0xd2,2);
        _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
        cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                          (puVar7,puVar8,puVar9,uVar10,uVar11);
        local_12d = '\x01' - (cVar2 != '\x01');
        local_8._0_1_ = 0xc;
        __1CCharString__QAE_XZ();
        local_8._0_1_ = 0xb;
        __1CCharString__QAE_XZ();
        local_8 = CONCAT31(local_8._1_3_,9);
        __1CWideString__QAE_XZ();
        if ((local_12d != '\0') && (BVar3 = IsDebuggerPresent(), BVar3 != 0)) {
          pcVar1 = (code *)swi(3);
          (*pcVar1)();
          return;
        }
      }
    }
    local_8 = 0xffffffff;
    __1__CArray_G__QAE_XZ();
  }
  ExceptionList = local_10;
  return;
}


==================== ?FindShorePointsInMap@CWaterGenerator@@QAEXPBVCEngineMap@@AAV?$CArray@VC2DCoordI@@@@@Z @ 02e07490 ====================

/* WARNING: Removing unreachable block (ram,0x02e07693) */
/* WARNING: Removing unreachable block (ram,0x02e0757d) */
/* WARNING: Removing unreachable block (ram,0x02e07671) */
/* WARNING: Removing unreachable block (ram,0x02e07556) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void _FindShorePointsInMap_CWaterGenerator__QAEXPBVCEngineMap__AAV__CArray_VC2DCoordI_____Z
               (undefined4 param_1,undefined4 param_2)

{
  bool bVar1;
  code *pcVar2;
  char cVar3;
  int iVar4;
  BOOL BVar5;
  float10 fVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puStack_140;
  char *pcStack_13c;
  wchar_t *pwStack_138;
  undefined4 local_134;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 local_128;
  undefined4 local_124;
  undefined4 local_120;
  undefined4 local_11c;
  undefined4 local_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_108;
  uint local_104;
  int local_100;
  undefined1 *local_fc;
  int local_f8;
  undefined1 *local_f4;
  int local_f0;
  undefined1 *local_ec;
  undefined1 local_e4 [12];
  undefined1 *local_d8;
  undefined1 local_d4 [15];
  char local_c5;
  undefined1 local_c4 [12];
  undefined1 local_b8 [15];
  char local_a9;
  undefined1 local_a8 [8];
  undefined1 local_a0 [8];
  undefined1 local_98 [11];
  char local_8d;
  undefined1 local_8c [12];
  undefined1 local_80 [15];
  char local_71;
  int local_70;
  int local_6c;
  undefined1 local_68 [12];
  undefined1 local_5c [8];
  undefined1 local_54 [8];
  undefined1 *local_4c;
  int local_48;
  undefined1 *local_44;
  int local_40;
  undefined1 *local_3c;
  int local_38;
  byte local_31;
  undefined1 *local_30;
  int local_2c;
  int local_28;
  undefined1 *local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e7d16d;
  local_10 = ExceptionList;
  pwStack_138 = 
  L"䖉诤ࡍ\xf8e8魼觾\xf045䶋\xe808﹤ﺝ䖉诜ࡍ雨ꎶ觾\xec45䶋\xe808﹎ﺝ\xe883蔁绀謓ࡍ㿨鷾菾Ǩ薉８\xffff૫藇８\xffff"
  ;
  ExceptionList = &local_10;
  local_20 = _PeekWorldX_CEngineMap__QBEJXZ();
  pwStack_138 = (wchar_t *)0x2e074c7;
  local_14 = _PeekWorldY_CEngineMap__QBEJXZ();
  pwStack_138 = 
  L"䖉诜ࡍ雨ꎶ觾\xec45䶋\xe808﹎ﺝ\xe883蔁绀謓ࡍ㿨鷾菾Ǩ薉８\xffff૫藇８\xffff"
  ;
  local_28 = _PeekWidth_CEngineMap__QBEJXZ();
  pwStack_138 = L"䖉诬ࡍ仨鷾菾Ǩ삅፾䶋\xe808︿ﺝ\xe883褁ᢅ\xffff\xebff윊ᢅ\xffffÿ";
  local_18 = _PeekHeight_CEngineMap__QBEJXZ();
  pwStack_138 = L"\xe883蔁绀謓ࡍ㿨鷾菾Ǩ薉８\xffff૫藇８\xffff";
  iVar4 = _PeekWidth_CEngineMap__QBEJXZ();
  if (iVar4 == 1 || iVar4 + -1 < 0) {
    local_ec = (undefined1 *)0x0;
  }
  else {
    pwStack_138 = L"\xe883褁ᢅ\xffff\xebff윊ᢅ\xffffÿ";
    iVar4 = _PeekWidth_CEngineMap__QBEJXZ();
    local_ec = (undefined1 *)(iVar4 + -1);
  }
  local_24 = local_ec;
  pwStack_138 = L"\xe883蔁绀謓ࡍ䟨ꎶ菾Ǩ薉４\xffff૫藇４\xffff";
  iVar4 = _PeekHeight_CEngineMap__QBEJXZ();
  if (iVar4 == 1 || iVar4 + -1 < 0) {
    local_f0 = 0;
  }
  else {
    pwStack_138 = L"\xe883褁ᒅ\xffff\xebff윊ᒅ\xffffÿ";
    local_f0 = _PeekHeight_CEngineMap__QBEJXZ();
    local_f0 = local_f0 + -1;
  }
  local_1c = local_f0;
  pwStack_138 = (wchar_t *)0x2e07552;
  _clear___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAEXXZ();
  local_2c = 0;
  do {
    if (local_18 <= local_2c) {
      pwStack_138 = (wchar_t *)param_2;
      pcStack_13c = (char *)0x2e07aae;
      _Append___CArray_VC2DCoordI____QAEXABV1__Z();
      ExceptionList = local_10;
      return;
    }
    for (local_30 = (undefined1 *)0x0; (int)local_30 < local_28; local_30 = local_30 + 1) {
      pwStack_138 = (wchar_t *)0x1;
      pcStack_13c = (char *)local_2c;
      puStack_140 = local_30;
      cVar3 = _PeekInterpolatedHasWater_CEngineMap__QBE_NJJJ_Z();
      if (cVar3 == '\0') {
        if (local_30 == (undefined1 *)0x1 || (int)(local_30 + -1) < 0) {
          local_f4 = (undefined1 *)0x0;
        }
        else {
          local_f4 = local_30 + -1;
        }
        local_3c = local_f4;
        if (local_2c == 1 || local_2c + -1 < 0) {
          local_f8 = 0;
        }
        else {
          local_f8 = local_2c + -1;
        }
        local_40 = local_f8;
        if ((int)(local_30 + 1) < (int)local_24) {
          local_fc = local_30 + 1;
        }
        else {
          local_fc = local_24;
        }
        local_44 = local_fc;
        if (local_2c + 1 < local_1c) {
          local_100 = local_2c + 1;
        }
        else {
          local_100 = local_1c;
        }
        local_38 = local_100;
        local_31 = 0;
        for (local_48 = local_f8; local_48 <= local_38; local_48 = local_48 + 1) {
          for (local_4c = local_3c; (int)local_4c <= (int)local_44; local_4c = local_4c + 1) {
            pwStack_138 = (wchar_t *)0x1;
            pcStack_13c = (char *)local_48;
            puStack_140 = local_4c;
            fVar6 = (float10)_PeekInterpolatedWaterDepth_CEngineMap__QBEMJJJ_Z();
            bVar1 = (float10)_DAT_0446907c < fVar6 != ((float10)_DAT_0446907c == fVar6);
            local_104 = (uint)bVar1;
            local_31 = local_31 | bVar1;
          }
        }
        if (local_31 != 0) {
          pwStack_138 = (wchar_t *)(local_2c + local_14);
          pcStack_13c = local_30 + local_20;
          puStack_140 = (undefined1 *)0x2e07718;
          __0C2DCoordI__QAE_JJ_Z();
          pwStack_138 = (wchar_t *)local_8c;
          pcStack_13c = (char *)0x2e07727;
          pwStack_138 = (wchar_t *)
                        _end___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_XZ
                                  ();
          local_8 = 0;
          pcStack_13c = local_54;
          puStack_140 = local_80;
          local_10c = pwStack_138;
          local_108 = pwStack_138;
          local_114 = _Find___CArray_VC2DCoordI____QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__ABVC2DCoordI___Z
                                ();
          local_8._0_1_ = 1;
          pcStack_13c = (char *)0x2e07778;
          local_110 = local_114;
          cVar3 = FID_conflict_operator__();
          local_71 = '\x01' - (cVar3 != '\0');
          local_8 = (uint)local_8._1_3_ << 8;
          pwStack_138 = (wchar_t *)0x2e07791;
          __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                    ();
          local_8 = 0xffffffff;
          pwStack_138 = L"똏鍍즅萏Ë";
          __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                    ();
          if (local_71 != '\0') {
            pwStack_138 = L"Engine_Water_Generator.cpp";
            pcStack_13c = (char *)0x2e077bf;
            __0CWideString__QAE_PB_W_Z();
            local_8 = 2;
            pwStack_138 = (wchar_t *)0xffffffff;
            pcStack_13c = "";
            puStack_140 = (undefined1 *)0x2e077d8;
            __0CCharString__QAE_PBDJ_Z();
            local_8._0_1_ = 3;
            pwStack_138 = (wchar_t *)0xffffffff;
            pcStack_13c = "shore_points.Find(shore_pt) == shore_points.end()";
            puStack_140 = (undefined1 *)0x2e077ee;
            __0CCharString__QAE_PBDJ_Z();
            local_8._0_1_ = 4;
            pwStack_138 = (wchar_t *)0x1;
            pcStack_13c = (char *)0x100;
            puStack_140 = local_98;
            puVar8 = local_a0;
            puVar7 = local_a8;
            _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8);
            _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
            cVar3 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                              (puVar7,puVar8);
            local_8d = '\x01' - (cVar3 != '\x01');
            local_8._0_1_ = 3;
            pwStack_138 = 
            L"䗆˼趍､\xffff\xe1e8ꆩ쟾ﱅ\xffff\xffff趍ｬ\xffff쿨鸪࿾薶ｷ\xffff삅୴ᗿ䁠Ҽ삅Ŵ跌끍譑్꣨ꊣ\xe9feȇ"
            ;
            __1CCharString__QAE_XZ();
            local_8 = CONCAT31(local_8._1_3_,2);
            pwStack_138 = (wchar_t *)0x2e07852;
            __1CCharString__QAE_XZ();
            local_8 = 0xffffffff;
            pwStack_138 = (wchar_t *)0x2e07864;
            __1CWideString__QAE_XZ();
            if (local_8d != '\0') {
              pwStack_138 = L"삅Ŵ跌끍譑్꣨ꊣ\xe9feȇ";
              BVar5 = IsDebuggerPresent();
              if (BVar5 != 0) {
                pcVar2 = (code *)swi(3);
                (*pcVar2)();
                return;
              }
            }
          }
          pwStack_138 = (wchar_t *)local_54;
          pcStack_13c = (char *)0x2e07886;
          _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
        }
      }
      else {
        pwStack_138 = (wchar_t *)0x7;
        pcStack_13c = (char *)local_2c;
        puStack_140 = local_30;
        cVar3 = _PeekWaterHasType_CEngineMap__QBE_NJJW4EWaterType___Z();
        if (cVar3 == '\0') {
          if ((((local_30 == (undefined1 *)0x0) || (local_30 == local_24)) || (local_2c == 0)) ||
             (local_2c == local_1c)) {
            pwStack_138 = (wchar_t *)(local_2c + local_14);
            pcStack_13c = local_30 + local_20;
            puStack_140 = (undefined1 *)0x2e0799a;
            __0C2DCoordI__QAE_JJ_Z();
            if (local_30 == local_24) {
              local_70 = local_70 + 1;
            }
            else if (local_2c == local_1c) {
              local_6c = local_6c + 1;
            }
            pwStack_138 = (wchar_t *)&local_70;
            pcStack_13c = local_68;
            puStack_140 = (undefined1 *)0x2e079d4;
            _Find___CArray_VC2DCoordI____QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__ABVC2DCoordI___Z
                      ();
            local_8 = 7;
            pwStack_138 = (wchar_t *)local_d4;
            pcStack_13c = (char *)0x2e079f0;
            pwStack_138 = (wchar_t *)
                          _end___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_XZ
                                    ();
            local_8._0_1_ = 8;
            pcStack_13c = (char *)0x2e07a15;
            local_12c = pwStack_138;
            local_128 = pwStack_138;
            local_c5 = FID_conflict_operator__();
            local_8 = CONCAT31(local_8._1_3_,7);
            pwStack_138 = (wchar_t *)0x2e07a2a;
            __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                      ();
            if (local_c5 == '\0') {
              local_d8 = (undefined1 *)&puStack_140;
              local_130 = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<wchar_t>_>_>
                                    (local_68);
              local_134 = _erase___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_V___Vector_const_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2__Z
                                    (local_e4);
              pwStack_138 = 
              L"䗇￼\xffff跿鱍Ῠꓖ\xe9fe﫴\xffff죩\xfffa诿ౕ譒᲍\xffff菿㣁ꏨꃽ课\xf44d襤\r"
              ;
              __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                        ();
            }
            else {
              pwStack_138 = (wchar_t *)&local_70;
              pcStack_13c = (char *)0x2e07a41;
              _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
            }
            local_8 = 0xffffffff;
            pwStack_138 = L"\xf4e9\xfffa\xe9ff靖\xffff喋刌趋＜\xffff솃\xe838ﶣﺠ䶋擴ඉ";
            __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                      ();
          }
        }
        else {
          pwStack_138 = (wchar_t *)(local_2c + local_14);
          pcStack_13c = local_30 + local_20;
          puStack_140 = (undefined1 *)0x2e078be;
          __0C2DCoordI__QAE_JJ_Z();
          pwStack_138 = (wchar_t *)local_c4;
          pcStack_13c = (char *)0x2e078cd;
          pwStack_138 = (wchar_t *)
                        _end___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_XZ
                                  ();
          local_8 = 5;
          pcStack_13c = local_5c;
          puStack_140 = local_b8;
          local_11c = pwStack_138;
          local_118 = pwStack_138;
          local_124 = _Find___CArray_VC2DCoordI____QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__ABVC2DCoordI___Z
                                ();
          local_8._0_1_ = 6;
          pcStack_13c = (char *)0x2e07921;
          local_120 = local_124;
          local_a9 = FID_conflict_operator__();
          local_8 = CONCAT31(local_8._1_3_,5);
          pwStack_138 = 
          L"䗇￼\xffff跿䂍\xffff\xe8ff흩ﺤ똏宍\xffff藿瓉贌ꡕ譒్쿨ꊢ\xe9feĮ";
          __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                    ();
          local_8 = 0xffffffff;
          pwStack_138 = L"똏宍\xffff藿瓉贌ꡕ譒్쿨ꊢ\xe9feĮ";
          __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                    ();
          if (local_a9 != '\0') {
            pwStack_138 = (wchar_t *)local_5c;
            pcStack_13c = (char *)0x2e0795f;
            _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
          }
        }
      }
    }
    local_2c = local_2c + 1;
  } while( true );
}


==================== ?GenerateShoreMapUCoords@CWaterGenerator@@AAEXXZ @ 02e09e30 ====================

/* WARNING: Removing unreachable block (ram,0x02e09ee6) */
/* WARNING: Removing unreachable block (ram,0x02e09e54) */
/* [ported from ego_r via strfp] */

void __fastcall _GenerateShoreMapUCoords_CWaterGenerator__AAEXXZ(undefined4 param_1)

{
  undefined4 uVar1;
  uint uVar2;
  undefined4 *puVar3;
  int iVar4;
  float10 fVar5;
  undefined1 local_34 [8];
  float local_2c;
  undefined4 local_28;
  undefined4 local_24;
  uint local_20;
  float local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  uint local_8;
  
  uVar1 = _size___vector_V__CArray_VC2DCoordI____V__allocator_V__CArray_VC2DCoordI_____std___std__QBEIXZ
                    (param_1);
  _resize___vector_V__CArray_U__pair_VC2DVector__M_std____V__allocator_V__CArray_U__pair_VC2DVector__M_std_____std___std__QAEXI_Z
            (uVar1);
  local_8 = 0;
  while (uVar2 = _size___vector_V__CArray_VC2DCoordI____V__allocator_V__CArray_VC2DCoordI_____std___std__QBEIXZ
                           (param_1), local_8 < uVar2) {
    local_10 = __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z(local_8);
    local_c = __A__CArray_V__CArray_U__pair_VC2DVector__M_std______QAEAAV__CArray_U__pair_VC2DVector__M_std____I_Z
                        (local_8);
    local_1c = 0.0;
    puVar3 = &local_18;
    __A__CArray_VC2DCoordI____QAEAAVC2DCoordI__I_Z(0);
    _ToC2DCoordF_C2DCoordI__QBE_AVC2DVector__XZ(puVar3);
    puVar3 = (undefined4 *)
             __A__CArray_U__pair_VC2DVector__M_std____QAEAAU__pair_VC2DVector__M_std__I_Z(0);
    *puVar3 = local_18;
    puVar3[1] = local_14;
    iVar4 = __A__CArray_U__pair_VC2DVector__M_std____QAEAAU__pair_VC2DVector__M_std__I_Z(0);
    *(float *)(iVar4 + 8) = local_1c;
    local_20 = 1;
    while (uVar2 = _size___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QBEIXZ(),
          local_20 < uVar2) {
      puVar3 = &local_28;
      __A__CArray_VC2DCoordI____QAEAAVC2DCoordI__I_Z(local_20);
      _ToC2DCoordF_C2DCoordI__QBE_AVC2DVector__XZ(puVar3);
      __GC2DVector__QBE_AV0_ABV0__Z(local_34,&local_18);
      fVar5 = (float10)_GetSquaredMagnitude_C2DVector__QBEMXZ();
      fVar5 = (float10)_GFRoot__YIMM_Z((float)fVar5);
      local_2c = (float)fVar5;
      local_1c = local_1c + local_2c;
      puVar3 = (undefined4 *)
               __A__CArray_U__pair_VC2DVector__M_std____QAEAAU__pair_VC2DVector__M_std__I_Z
                         (local_20);
      *puVar3 = local_28;
      puVar3[1] = local_24;
      iVar4 = __A__CArray_U__pair_VC2DVector__M_std____QAEAAU__pair_VC2DVector__M_std__I_Z(local_20)
      ;
      *(float *)(iVar4 + 8) = local_1c;
      local_18 = local_28;
      local_14 = local_24;
      local_20 = local_20 + 1;
    }
    local_8 = local_8 + 1;
  }
  return;
}


==================== ?SortShorePointsIntoWaterBodies@CWaterGenerator@@QAEXMM@Z @ 02e083b0 ====================

/* WARNING: Removing unreachable block (ram,0x02e0890e) */
/* [ported from ego_r via strfp] */

void _SortShorePointsIntoWaterBodies_CWaterGenerator__QAEXMM_Z(float param_1,float param_2)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  BOOL BVar4;
  uint uVar5;
  undefined4 uVar6;
  float10 fVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  int *local_128;
  char *local_124;
  wchar_t *local_120;
  undefined4 local_11c;
  undefined4 local_118;
  int local_114;
  int local_110;
  int local_10c;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  int local_e0;
  undefined1 local_dc [12];
  undefined1 *local_d0;
  undefined1 local_cc [12];
  undefined1 local_c0 [12];
  undefined1 *local_b4;
  undefined1 local_b0 [8];
  undefined1 local_a8 [8];
  int local_a0 [2];
  char local_95;
  uint local_80;
  undefined1 local_7c [8];
  undefined1 local_74 [8];
  int local_6c;
  int local_68;
  undefined1 local_64 [12];
  int *local_58;
  float local_54;
  int *local_44;
  undefined1 local_40 [12];
  int local_34 [2];
  char local_29;
  undefined1 local_28 [20];
  float local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e7d23f;
  local_10 = ExceptionList;
  local_120 = L"삅蘏כ";
  ExceptionList = &local_10;
  iVar3 = _size___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QBEIXZ();
  if (iVar3 == 0) {
    ExceptionList = local_10;
    return;
  }
  local_120 = L"䗇ð";
  _CombineShorePointArrays_CWaterGenerator__AAEXXZ();
  local_14 = 0.0;
LAB_02e083fc:
  local_120 = L"똏藀࿀鮅\x05贀炍\xffff\xe8ff㧼ﺚ薉＠\xffff趋＠\xffff趉＜\xffff䗇ü";
  cVar2 = _empty___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QBE_NXZ();
  if (cVar2 != '\0') {
    local_120 = L"開Ｄ\xffff䋆Ą䶋擴ඉ";
    _FindAndStoreExtents_CWaterGenerator__AAEXXZ();
    *(undefined1 *)(local_e0 + 4) = 1;
    ExceptionList = local_10;
    return;
  }
  local_120 = L"薉＠\xffff趋＠\xffff趉＜\xffff䗇ü";
  local_120 = (wchar_t *)__0__CArray_VC2DCoordI____QAE_XZ();
  local_8 = 0;
  local_124 = (char *)0x2e0844e;
  local_e8 = local_120;
  local_e4 = local_120;
  _push_back___CArray_V__CArray_VC2DCoordI______QAEXABV__CArray_VC2DCoordI_____Z();
  local_8 = 0xffffffff;
  local_120 = L"趋Ｄ\xffff솃\xe84cשׂﺜ䶋菰ǁ섻萏Ë";
  __1__CArray_VC2DCoordI____QAE_XZ();
  local_120 = L"䶋菰ǁ섻萏Ë";
  iVar3 = _size___vector_V__CArray_VC2DCoordI____V__allocator_V__CArray_VC2DCoordI_____std___std__QBEIXZ
                    ();
  if (iVar3 != (int)local_14 + 1) {
    local_120 = L"Engine_Water_Generator.cpp";
    local_124 = (char *)0x2e0848c;
    __0CWideString__QAE_PB_W_Z();
    local_8 = 1;
    local_120 = (wchar_t *)0xffffffff;
    local_124 = "";
    local_128 = (int *)0x2e084a5;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 2;
    local_120 = (wchar_t *)0xffffffff;
    local_124 = "WaterBodyShorePts.size() == c_water_body+1";
    local_128 = (int *)0x2e084bb;
    __0CCharString__QAE_PBDJ_Z();
    local_8._0_1_ = 3;
    local_120 = (wchar_t *)0x1;
    local_124 = (char *)0x185;
    local_128 = local_a0;
    puVar9 = local_a8;
    puVar8 = local_b0;
    _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar8,puVar9);
    _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
    cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                      (puVar8,puVar9);
    local_95 = '\x01' - (cVar2 != '\x01');
    local_8._0_1_ = 2;
    local_120 = (wchar_t *)0x2e08510;
    __1CCharString__QAE_XZ();
    local_8 = CONCAT31(local_8._1_3_,1);
    local_120 = 
    L"䗇￼\xffff跿撍\xffff\xe8ffḂﺞ똏澅\xffff藿瓀＋怕뱀蔄瓀찁趋Ｄ\xffff솃\xe838얁ﺣ譐\xf04d譑⒍\xffff菿䳁싨ꃧ课\xe8c8隿ﺢ閍ｄ\xffff譒⒍\xffff菿㣁篨ꏘ觾ᢅ\xffff诿ᢅ\xffff觿ᒅ\xffff쟿ﱅ\x04"
    ;
    __1CCharString__QAE_XZ();
    local_8 = 0xffffffff;
    local_120 = 
    L"똏澅\xffff藿瓀＋怕뱀蔄瓀찁趋Ｄ\xffff솃\xe838얁ﺣ譐\xf04d譑⒍\xffff菿䳁싨ꃧ课\xe8c8隿ﺢ閍ｄ\xffff譒⒍\xffff菿㣁篨ꏘ觾ᢅ\xffff诿ᢅ\xffff觿ᒅ\xffff쟿ﱅ\x04"
    ;
    __1CWideString__QAE_XZ();
    if (local_95 != '\0') {
      local_120 = (wchar_t *)0x2e08542;
      BVar4 = IsDebuggerPresent();
      if (BVar4 != 0) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
  }
  local_120 = 
  L"譐\xf04d譑⒍\xffff菿䳁싨ꃧ课\xe8c8隿ﺢ閍ｄ\xffff譒⒍\xffff菿㣁篨ꏘ觾ᢅ\xffff诿ᢅ\xffff觿ᒅ\xffff쟿ﱅ\x04"
  ;
  local_120 = (wchar_t *)
              _front___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAEAAVC2DCoordI__XZ()
  ;
  local_124 = (char *)local_14;
  local_128 = (int *)0x2e08568;
  __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
  local_124 = (char *)0x2e0856f;
  _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
  local_120 = (wchar_t *)local_c0;
  local_124 = (char *)0x2e08584;
  local_f0 = _begin___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_XZ
                       ();
  local_8 = 4;
  local_b4 = (undefined1 *)&local_128;
  local_ec = local_f0;
  local_f4 = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<wchar_t>_>_>
                       (local_f0);
  local_f8 = _erase___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_V___Vector_const_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2__Z
                       (local_cc);
  local_120 = L"䗇￼\xffff跿䒍\xffff\xe8ff쪿ﺤ䶍\xe8dc輺ﺙ䗇׼";
  __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
            ();
  local_8 = 0xffffffff;
  local_120 = L"䶍\xe8dc輺ﺙ䗇׼";
  __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
            ();
  local_120 = L"䗇׼";
  __0__CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std____QAE_XZ
            ();
  local_8 = 5;
  local_29 = '\0';
  do {
    if (local_29 == '\0') {
      local_120 = L"똏藐痒윌ࢅ\xffffÿ";
      cVar2 = _empty___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QBE_NXZ();
      if (cVar2 != '\0') goto LAB_02e0862e;
      local_fc = 0;
    }
    else {
LAB_02e0862e:
      local_fc = 1;
    }
    if (local_fc != 0) break;
    local_120 = (wchar_t *)local_14;
    local_124 = (char *)0x2e08657;
    __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
    local_120 = (wchar_t *)0x2e0865e;
    local_120 = (wchar_t *)
                _back___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAEAAVC2DCoordI__XZ
                          ();
    local_124 = (char *)0x2e08667;
    __0C2DCoordI__QAE_ABV0__Z();
    local_120 = (wchar_t *)0x2e0866f;
    __0___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
              ();
    local_8._0_1_ = 6;
    local_120 = (wchar_t *)local_28;
    local_124 = (char *)local_14;
    local_128 = local_34;
    cVar2 = _FindShorePointCloserThan_CWaterGenerator__AAE_NAAV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__MABVC2DCoordI__JAAV__CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std_____Z
                      (local_40,param_1);
    if (cVar2 == '\0') {
      local_120 = (wchar_t *)local_14;
      local_124 = (char *)0x2e086e1;
      __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
      local_120 = (wchar_t *)0x2e086e8;
      local_44 = (int *)_front___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAEAAVC2DCoordI__XZ
                                  ();
      local_120 = (wchar_t *)local_14;
      local_124 = (char *)0x2e086fd;
      __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
      local_120 = (wchar_t *)0x2e08704;
      local_58 = (int *)_back___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAEAAVC2DCoordI__XZ
                                  ();
      local_120 = (wchar_t *)0x0;
      local_100 = local_58[1] - local_44[1];
      local_124 = (char *)(float)local_100;
      local_104 = *local_58 - *local_44;
      local_128 = (int *)(float)local_104;
      __0C3DVector__QAE_MMM_Z();
      local_120 = (wchar_t *)0x2e08753;
      fVar7 = (float10)_GetAccurateMagnitude_C3DVector__QBEMXZ();
      local_54 = (float)fVar7;
      if (local_54 <= param_2) {
        if ((local_54 <= param_1) || (param_2 <= local_54)) {
          local_29 = '\x01';
        }
        else {
          local_110 = local_58[1] - local_44[1];
          local_120 = (wchar_t *)0x2e088b5;
          local_120 = (wchar_t *)__ftol2_sse();
          local_114 = *local_58 - *local_44;
          local_124 = (char *)0x2e088de;
          local_124 = (char *)__ftol2_sse();
          local_128 = (int *)0x2e088e7;
          __0C2DCoordI__QAE_JJ_Z();
          local_120 = (wchar_t *)local_7c;
          local_124 = (char *)local_14;
          local_128 = (int *)0x2e088fd;
          __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
          local_124 = (char *)0x2e08904;
          _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
        }
      }
      else {
        local_120 = (wchar_t *)0x2e08771;
        __0___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                  ();
        local_8 = CONCAT31(local_8._1_3_,7);
        local_120 = (wchar_t *)local_28;
        local_124 = (char *)local_14;
        local_128 = local_58;
        cVar2 = _FindShorePointCloserThan_CWaterGenerator__AAE_NAAV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__MABVC2DCoordI__JAAV__CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std_____Z
                          (local_64,param_2);
        if (cVar2 == '\0') {
          local_29 = '\x01';
        }
        else {
          local_120 = (wchar_t *)0x2e087aa;
          local_120 = (wchar_t *)
                      __D___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QBEAAVC2DCoordI__XZ
                                ();
          local_124 = (char *)0x2e087b3;
          __0C2DCoordI__QAE_ABV0__Z();
          local_108 = local_68 - local_58[1];
          local_120 = 
          L"譐걅Û䶋讬顕ᄫ閉ﻸ\xffff藛ﻸ\xffffො⺸Ђ쇞뻨ꉒ僾䶍\xe890ﭝﺡ䖍傐䶋凰趋Ｄ\xffff솃\xe84c\xe509ﺠ좋ۨꊔ跾顕譒\xf045譐⒍\xffff菿䳁\xece8ꃤ课\xe8c8鏩ﺢ䶍冠䶍\xe8dc팿ﺜӫ䗆Ǜ䗆ۼ䶍\xe8a0졎ﺤꋩ"
          ;
          local_120 = (wchar_t *)__ftol2_sse();
          local_10c = local_6c - *local_58;
          local_124 = (char *)0x2e08802;
          local_124 = (char *)__ftol2_sse();
          local_128 = (int *)0x2e0880b;
          __0C2DCoordI__QAE_JJ_Z();
          local_120 = (wchar_t *)local_74;
          local_124 = (char *)local_14;
          local_128 = (int *)0x2e08821;
          __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
          local_124 = (char *)0x2e08828;
          _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
          local_120 = (wchar_t *)&local_6c;
          local_124 = (char *)local_14;
          local_128 = (int *)0x2e0883e;
          __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
          local_124 = (char *)0x2e08845;
          _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
          local_120 = (wchar_t *)local_64;
          local_124 = (char *)0x2e08851;
          _push_back___CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std____QAEXABV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std___Z
                    ();
        }
        local_8._0_1_ = 6;
        local_120 = L"ꋩ";
        __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                  ();
      }
    }
    else {
      local_120 = (wchar_t *)0x2e086a4;
      local_120 = (wchar_t *)
                  __D___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QBEAAVC2DCoordI__XZ
                            ();
      local_124 = (char *)local_14;
      local_128 = (int *)0x2e086b7;
      __A__CArray_V__CArray_VC2DCoordI______QAEAAV__CArray_VC2DCoordI____I_Z();
      local_124 = (char *)0x2e086be;
      _push_back___CArray_VC2DCoordI____QAEXABVC2DCoordI___Z();
      local_120 = (wchar_t *)local_40;
      local_124 = (char *)0x2e086ca;
      _push_back___CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std____QAEXABV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std___Z
                ();
    }
    local_80 = 0;
    while( true ) {
      local_120 = (wchar_t *)0x2e0892a;
      uVar5 = _size___vector_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__V__allocator_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std___2__std__QBEIXZ
                        ();
      if (uVar5 <= local_80) break;
      local_120 = (wchar_t *)local_80;
      local_124 = (char *)0x2e0893b;
      uVar6 = __A__CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std____QAEAAV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__I_Z
                        ();
      local_d0 = (undefined1 *)&local_128;
      local_118 = FID_conflict__String_const_iterator<std::_String_val<std::_Simple_types<wchar_t>_>_>
                            (uVar6);
      local_11c = _erase___vector_VC2DCoordI__V__allocator_VC2DCoordI___std___std__QAE_AV___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2_V___Vector_const_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___2__Z
                            (local_dc);
      local_120 = (wchar_t *)0x2e08978;
      __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
                ();
      local_80 = local_80 + 1;
    }
    local_120 = (wchar_t *)0x2e08982;
    _clear___vector_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__V__allocator_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std___2__std__QAEXXZ
              ();
    local_8 = CONCAT31(local_8._1_3_,5);
    local_120 = (wchar_t *)0x2e0898e;
    __1___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std__QAE_XZ
              ();
  } while( true );
  local_14 = (float)((int)local_14 + 1);
  local_8 = 0xffffffff;
  local_120 = L"䳩\xfffa诿⒍\xffff\xe8ffꖯﺞ開Ｄ\xffff䋆Ą䶋擴ඉ";
  __1__CArray_V___Vector_iterator_V___Vector_val_VC2DCoordI__V__allocator_VC2DCoordI___std___std___std____QAE_XZ
            ();
  goto LAB_02e083fc;
}


==================== ?BuildAndSaveWaterData@CEngineWorldMap@@AAEXXZ @ 02d68be0 ====================

/* [ported from ego_r via strfp] */

void __fastcall _BuildAndSaveWaterData_CEngineWorldMap__AAEXXZ(int param_1)

{
  char local_9;
  int local_8;
  
  local_8 = _FindIndexBySymbol_CBankFile__QBEKABVCCharString___Z(&DAT_04ac92f4);
  if (local_8 == 0) {
    local_8 = _CreateNewBankMap_CEngineWorldMap__AAEKABVCCharString___Z(&DAT_04ac92f4);
    local_9 = '\x01';
  }
  else {
    local_9 = _WaterDataNeedsUpdate_CEngineWorldMap__AAE_NK_Z(local_8);
  }
  if (local_9 != '\0') {
    _BuildAndSaveNonPatchData_CEngineWaterRenderer__QAEXAAVCStaticMapBankFile__J_Z
              (param_1 + 0x4c,local_8);
  }
  return;
}


==================== ?LoadWaterData@CEngineWorldMap@@AAEXXZ @ 02d5fe00 ====================

/* [ported from ego_r via strfp] */

void _LoadWaterData_CEngineWorldMap__AAEXXZ(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 extraout_var;
  undefined1 local_84 [48];
  undefined4 local_54;
  undefined4 local_50;
  undefined1 local_38 [28];
  undefined1 local_1c [8];
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e6d488;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_14 = _FindIndexBySymbol_CBankFile__QBEKABVCCharString___Z(&DAT_04ac92f4);
  if (local_14 != 0) {
    uVar3 = extraout_var;
    uVar1 = _PeekEntryUpdateData_CBankFile__QBEPBVCBankFileEntryUpdateData__K_Z(local_14);
    local_54 = _GetInfoSize_CBankFileEntryUpdateData__QBEKXZ(uVar3,uVar1);
    __0__CArray_D__QAE_I_Z(local_54);
    local_8 = 0;
    uVar3 = local_54;
    uVar1 = _GetAsCArray___CArray_D__QAEPADXZ(local_54);
    _ReadInfo_CBankFileEntryUpdateData__QBEXKPAXK_Z(0,uVar1,uVar3);
    uVar3 = local_54;
    uVar1 = _GetAsCArray___CArray_D__QAEPADXZ(local_54);
    __0CMemoryDataInputStream__QAE_PBXK_Z(uVar1,uVar3);
    local_8._0_1_ = 1;
    _GetAsyncEntry_CBankFileAsync__QAE_AV__CCountedPointer_VCBankFileAsyncEntry____K_Z
              (local_1c,local_14);
    local_8._0_1_ = 2;
    __C__CCountedPointer_VCBankFileAsyncEntry____QBEPAVCBankFileAsyncEntry__XZ();
    local_50 = _GetDataSize_CBankFileAsyncEntry__QBEKXZ();
    __0__CArray_E__QAE_I_Z(local_50);
    local_8._0_1_ = 3;
    uVar3 = local_50;
    uVar1 = _GetAsCArray___CArray_E__QAEPAEXZ(local_50);
    uVar2 = 0;
    __C__CCountedPointer_VCBankFileAsyncEntry____QBEPAVCBankFileAsyncEntry__XZ(0,uVar1);
    _ReadDataNonAsync_CBankFileAsyncEntry__QAEXKPAXK_Z(uVar2,uVar1,uVar3);
    uVar3 = local_50;
    uVar1 = _GetAsCArray___CArray_E__QAEPAEXZ(local_50);
    __0CMemoryDataInputStream__QAE_PBXK_Z(uVar1,uVar3);
    local_8._0_1_ = 4;
    _LoadNonPatchData_CEngineWaterRenderer__QAEXAAVCMemoryDataInputStream__0_Z(local_38,local_84);
    local_8._0_1_ = 3;
    __1CMemoryDataInputStream__UAE_XZ();
    local_8._0_1_ = 2;
    __1__CArray_E__QAE_XZ();
    local_8._0_1_ = 1;
    __1__CCountedPointer_VCBankFileAsyncEntry____QAE_XZ();
    local_8 = (uint)local_8._1_3_ << 8;
    __1CMemoryDataInputStream__UAE_XZ();
    local_8 = 0xffffffff;
    __1__CArray_D__QAE_XZ();
  }
  ExceptionList = local_10;
  return;
}


==================== ?BuildAndSaveNonPatchData@CEngineWaterRenderer@@QAEXAAVCStaticMapBankFile@@J@Z @ 02d55d40 ====================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* [ported from ego_r via strfp] */

void _BuildAndSaveNonPatchData_CEngineWaterRenderer__QAEXAAVCStaticMapBankFile__J_Z
               (undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  undefined4 uVar4;
  undefined4 extraout_var;
  undefined1 local_90 [36];
  undefined1 local_6c [28];
  undefined1 local_50 [36];
  undefined1 local_2c [28];
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03e6c569;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  __0CCharString__QAE_PBDJ_Z("Build Water Data",0xffffffff);
  local_8 = 0;
  uVar1 = extraout_var;
  _DisplayProgress_NProgressDisplay__YIXABVCCharString__M_N1_Z(_DAT_0401de00,0);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ(uVar1);
  __0CMemoryDataOutputStream__QAE_XZ();
  local_8 = 1;
  __0CMemoryDataOutputStream__QAE_XZ();
  local_8._0_1_ = 2;
  _BuildAndSaveNonPatchData_CEngineWaterRenderer__QAEXAAVCMemoryDataOutputStream__0_Z
            (local_50,local_90);
  __0CMemoryDataInputStream__QAE_ABVCMemoryDataOutputStream___Z(local_90);
  local_8._0_1_ = 3;
  __0CMemoryDataInputStream__QAE_ABVCMemoryDataOutputStream___Z(local_50);
  local_8._0_1_ = 4;
  _SetPosition_CDataInputStream__UAEXK_Z(0);
  _SetPosition_CDataInputStream__UAEXK_Z(0);
  __0CCharString__QAE_PBDJ_Z("Writing To Static Map Bank (Do not interrupt) ",0xffffffff);
  local_8._0_1_ = 5;
  _DisplayProgress_NProgressDisplay__YIXABVCCharString__M_N1_Z(_DAT_0401de00,0);
  local_8._0_1_ = 4;
  __1CCharString__QAE_XZ();
  _BeginUpdateEntries_CBankFile__QAEXXZ();
  uVar4 = 1;
  uVar1 = _GetLength_CMemoryDataOutputStream__QBEKXZ(1);
  puVar3 = local_6c;
  uVar2 = _GetLength_CMemoryDataOutputStream__QBEKXZ(puVar3,uVar1);
  _UpdateEntry_CBankFile__QAEXKAAVCDataInputStream__K0K_N_Z
            (param_2,local_2c,uVar2,puVar3,uVar1,uVar4);
  _EndUpdateEntries_CBankFile__QAEXXZ();
  local_8._0_1_ = 3;
  __1CMemoryDataInputStream__UAE_XZ();
  local_8._0_1_ = 2;
  __1CMemoryDataInputStream__UAE_XZ();
  local_8 = CONCAT31(local_8._1_3_,1);
  FID_conflict__time_put<char,std::ostreambuf_iterator<char,std::char_traits<char>_>_>();
  local_8 = 0xffffffff;
  FID_conflict__time_put<char,std::ostreambuf_iterator<char,std::char_traits<char>_>_>();
  ExceptionList = local_10;
  return;
}


==================== ?BuildPatchMesh@CEngineLandscapeMeshBuilder@@QAEPAVCWaterPatchDescriptors@@PAVCMovableResource@@PBVCEngineLandscapeMap@@JJPAPAVCLandscapeLayerMesh@@@Z @ 02cb1040 ====================

/* WARNING: Removing unreachable block (ram,0x02cb109d) */
/* WARNING: Removing unreachable block (ram,0x02cb1165) */
/* WARNING: Removing unreachable block (ram,0x02cb1141) */
/* [ported from ego_r via strfp] */

undefined4 __thiscall
_BuildPatchMesh_CEngineLandscapeMeshBuilder__QAEPAVCWaterPatchDescriptors__PAVCMovableResource__PBVCEngineLandscapeMap__JJPAPAVCLandscapeLayerMesh___Z
          (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5,
          undefined4 *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_14;
  
  uVar2 = param_4;
  uVar4 = param_5;
  uVar1 = _PeekEngineMap_CEngineLandscapeMap__QBEPBVCEngineMap__XZ(param_4,param_5);
  _BuildMapDirMask_CEngineLandscapeMeshBuilder__AAEXPBVCEngineMap__JJ_Z(uVar1,uVar2,uVar4);
  *(undefined4 *)(param_1 + 0xce8) = 0;
  uVar2 = param_4;
  uVar4 = param_5;
  uVar1 = _PeekEngineMap_CEngineLandscapeMap__QBEPBVCEngineMap__XZ(param_4,param_5);
  uVar2 = _BuildLayersFromThemes_CEngineLandscapeMeshBuilder__AAEPAVCWaterPatchDescriptors__PBVCEngineMap__JJ_Z
                    (uVar1,uVar2,uVar4);
  local_14 = 0;
  local_1c = 0;
  for (local_20 = 0; local_20 < *(int *)(param_1 + 0xce8); local_20 = local_20 + 1) {
    iVar5 = __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                      (local_20);
    if (local_14 < *(int *)(iVar5 + 0x324)) {
      local_1c = local_20;
      iVar5 = __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                        (local_20);
      local_14 = *(int *)(iVar5 + 0x324);
    }
  }
  uVar4 = param_3;
  uVar1 = param_4;
  uVar7 = param_5;
  uVar3 = __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                    (local_1c);
  uVar4 = _BuildLayerMesh_CEngineLandscapeMeshBuilder__AAEPAVCLandscapeLayerMesh__PAVCMovableResource__ABUCLayer_1_PBVCEngineLandscapeMap__JJ_Z
                    (param_2,uVar3,uVar4,uVar1,uVar7);
  for (local_24 = 0; local_24 < *(int *)(param_1 + 0xce8); local_24 = local_24 + 1) {
    if ((local_24 != local_1c) &&
       (iVar5 = __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                          (local_24), 0 < *(int *)(iVar5 + 0x324))) {
      uVar1 = param_3;
      uVar7 = param_4;
      uVar3 = param_5;
      uVar6 = __A__CArray_UCLayer_CEngineLandscapeMeshBuilder____QAEAAUCLayer_CEngineLandscapeMeshBuilder__I_Z
                        (local_24);
      uVar1 = _BuildLayerMesh_CEngineLandscapeMeshBuilder__AAEPAVCLandscapeLayerMesh__PAVCMovableResource__ABUCLayer_1_PBVCEngineLandscapeMap__JJ_Z
                        (param_2,uVar6,uVar1,uVar7,uVar3);
      _SetNext_CLandscapeLayerMesh__QAEXPAV1__Z(uVar1);
    }
  }
  *param_6 = uVar4;
  return uVar2;
}

