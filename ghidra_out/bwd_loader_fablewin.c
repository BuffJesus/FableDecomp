//=== ?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z @ 01c7f9b0 ===

/* WARNING: Removing unreachable block (ram,0x01c7face) */
/* WARNING: Removing unreachable block (ram,0x01c7fa72) */
/* WARNING: Removing unreachable block (ram,0x01c7fb0d) */
/* [ported from ego_r via strfp] */

void _LoadWorldFromBinaryFile_CWorldMap__QAEXAAVCWideString___Z(undefined4 param_1)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  BOOL BVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined4 uVar10;
  undefined1 local_94 [8];
  undefined1 local_8c [8];
  undefined1 local_84 [11];
  char local_79;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  undefined1 local_54 [28];
  int local_38;
  CAsyncMonikerFile local_34 [32];
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03c76856;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  _GFOutputDebugString__YIXPBD_Z();
  _GFOutputDebugString__YIXPBD_Z();
  __0CDiskFileWin32__QAE_ABVCWideString__W4FileAccessType__K_Z(param_1,1,2);
  local_8 = 0;
  iVar3 = _GetLength_CDiskFileWin32__UBEJXZ();
  __0__CArray_D__QAE_I_Z(iVar3 + 1);
  local_8._0_1_ = 1;
  uVar10 = 0;
  uVar4 = _GetByteSize___CArray_D__QBEIXZ(0);
  uVar5 = _GetAsCArray___CArray_D__QAEPADXZ(uVar4);
  _Read_CDiskFileWin32__UAEXPAXJPAVCASuspendableProcess___Z(uVar5,uVar4,uVar10);
  uVar4 = _GetByteSize___CArray_D__QBEIXZ();
  uVar5 = _GetAsCArray___CArray_D__QAEPADXZ(uVar4);
  __0CMemoryDataInputStream__QAE_PBXK_Z(uVar5,uVar4);
  local_8 = CONCAT31(local_8._1_3_,2);
  local_14 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _resize___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QAEXI_Z(local_14);
  for (local_6c = 1; local_6c < local_14; local_6c = local_6c + 1) {
    puVar9 = local_54;
    __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_6c);
    _LoadBinary_CMapInfo__QAEXAAVCDataInputStream___Z(puVar9);
  }
  local_38 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _resize___vector_VCRegion__V__allocator_VCRegion___std___std__QAEXI_Z(local_38);
  for (local_70 = 1; local_70 < local_38; local_70 = local_70 + 1) {
    puVar9 = local_54;
    __A__vector_VCRegion__V__allocator_VCRegion___std___std__QAEAAVCRegion__I_Z(local_70);
    _LoadBinary_CRegion__QAEXAAVCDataInputStream___Z(puVar9);
  }
  local_74 = 1;
  do {
    if (local_14 <= local_74) {
      local_8._0_1_ = 1;
      __1CMemoryDataInputStream__UAE_XZ();
      local_8 = (uint)local_8._1_3_ << 8;
      __1__CArray_D__QAE_XZ();
      local_8 = 0xffffffff;
      CAsyncMonikerFile::~CAsyncMonikerFile(local_34);
      ExceptionList = local_10;
      return;
    }
    local_78 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_74);
    if (*(int *)(local_78 + 0x28) == 0) {
      __0CWideString__QAE_PB_W_Z(L"world_map.cpp");
      local_8._0_1_ = 3;
      __0CCharString__QAE_PBDJ_Z(&DAT_040b77c5,0xffffffff);
      local_8._0_1_ = 4;
      __0CCharString__QAE_PBDJ_Z("map_info.MapUID!=0",0xffffffff);
      local_8._0_1_ = 5;
      uVar5 = 1;
      uVar4 = 0x36d;
      puVar9 = local_84;
      puVar8 = local_8c;
      puVar7 = local_94;
      _GFGetSystemManager__YIPAVCSystemManager__XZ(puVar7,puVar8,puVar9,0x36d,1);
      _GetDebugManager_CSystemManager__QAEPAVCDebugManager__XZ();
      cVar2 = _DoErrorMessage_CDebugManager__QAE_NABVCCharString__0ABVCWideString__KW4EErrorType_NDebugManager___Z
                        (puVar7,puVar8,puVar9,uVar4,uVar5);
      local_79 = '\x01' - (cVar2 != '\x01');
      local_8._0_1_ = 4;
      __1CCharString__QAE_XZ();
      local_8._0_1_ = 3;
      __1CCharString__QAE_XZ();
      local_8 = CONCAT31(local_8._1_3_,2);
      __1CWideString__QAE_XZ();
      if ((local_79 != '\0') && (BVar6 = IsDebuggerPresent(), BVar6 != 0)) {
        pcVar1 = (code *)swi(3);
        (*pcVar1)();
        return;
      }
    }
    _SetMapPlacement_CWorldMap__IAEXABVC2DBoxI__J_Z(local_78,local_74);
    iVar3 = local_74;
    _Get___CTBaseSingleton_VCEngineManager____SIAAVCEngineManager__XZ(local_74);
    _AddMap_CEngineManager__QAEXJ_Z(iVar3);
    local_74 = local_74 + 1;
  } while( true );
}



