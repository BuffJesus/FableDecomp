 '0x1f70940' '0x1f70650' '0x1c81740'  (HeadlessAnalyzer)  
INFO  SCRIPT: D:\Documents\FableTLC\tools\ghidra_scripts\DecompFuncs.java (HeadlessAnalyzer)  
==================== ?LoadBinary@CRegion@@QAEXAAVCDataInputStream@@@Z @ 01f70940 ==================== 

/* WARNING: Removing unreachable block (ram,0x01f709ce) */
/* WARNING: Removing unreachable block (ram,0x01f70985) */
/* WARNING: Removing unreachable block (ram,0x01f70bb6) */
/* [ported from ego_r via strfp] */

void _LoadBinary_CRegion__QAEXAAVCDataInputStream___Z(void)

{
  undefined1 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  float10 fVar5;
  undefined1 local_58 [8];
  undefined1 local_50 [8];
  undefined1 local_48 [8];
  undefined1 local_40 [8];
  undefined4 local_38;
  undefined4 local_34;
  undefined1 local_30 [8];
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03ccb098;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_14 = _ReadSLONG_CDataInputStream__QAEJXZ();
  local_18 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _resize___vector_JV__allocator_J_std___std__QAEXI_Z(local_14);
  for (local_20 = 0; local_20 < local_14; local_20 = local_20 + 1) {
    uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
    puVar4 = (undefined4 *)__A__vector_JV__allocator_J_std___std__QAEAAJI_Z(local_20);
    *puVar4 = uVar2;
  }
  _resize___vector_JV__allocator_J_std___std__QAEXI_Z(local_18);
  for (local_24 = 0; local_24 < local_18; local_24 = local_24 + 1) {
    uVar2 = _ReadSLONG_CDataInputStream__QAEJXZ();
    puVar4 = (undefined4 *)__A__vector_JV__allocator_J_std___std__QAEAAJI_Z(local_24);
    *puVar4 = uVar2;
  }
  uVar2 = _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_40);
  local_8 = 0;
  _SetName_CRegion__QAEXABVCCharString___Z(uVar2);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar2 = _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_48);
  local_8 = 1;
  _SetDisplayName_CRegion__QAEXABVCCharString___Z(uVar2);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar2 = _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_50);
  local_8 = 2;
  _SetRegionDefName_CRegion__QAEXABVCCharString___Z(uVar2);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar2 = _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_58);
  local_8 = 3;
  _SetMiniMapGraphic_CRegion__QAEXABVCCharString___Z(uVar2);
  local_8 = 0xffffffff;
  __1CCharString__QAE_XZ();
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  _SetAsOnWorldMap_CRegion__QAEX_N_Z(uVar1);
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  _SetCreatureGenerationEnabled_CRegion__QAEX_N_Z(uVar1);
  uVar1 = _ReadEBOOL_CDataInputStream__QAE_NXZ();
  _SetSoundThemesEnabled_CRegion__QAEX_N_Z(uVar1);
  fVar5 = (float10)_ReadFloat_CDataInputStream__QAEMXZ();
  _SetMiniMapScale_CRegion__QAEXM_Z((float)fVar5);
  iVar3 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _SetMiniMapOffsetX_CRegion__QAEXM_Z((float)iVar3);
  iVar3 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _SetMiniMapOffsetY_CRegion__QAEXM_Z((float)iVar3);
  iVar3 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _SetWorldMapOffsetX_CRegion__QAEXM_Z((float)iVar3);
  iVar3 = _ReadSLONG_CDataInputStream__QAEJXZ();
  _SetWorldMapOffsetY_CRegion__QAEXM_Z((float)iVar3);
  local_1c = _ReadSLONG_CDataInputStream__QAEJXZ();
  for (local_28 = 0; local_28 < local_1c; local_28 = local_28 + 1) {
    _ReadPresizedString_CDataInputStream__QAE_AVCCharString__XZ(local_30);
    local_8 = 4;
    _Read2DVector_CDataInputStream__QAE_AVC2DVector__XZ(&local_38);
    puVar4 = (undefined4 *)
             __A__map_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__std__QAEAAVC2DVector__ABVCCharString___Z
                       (local_30);
    *puVar4 = local_38;
    puVar4[1] = local_34;
    local_8 = 0xffffffff;
    __1CCharString__QAE_XZ();
  }
  ExceptionList = local_10;
  return;
}

 
==================== ?SaveBinary@CRegion@@QAEXAAVCDataOutputStream@@@Z @ 01f70650 ==================== 

/* WARNING: Removing unreachable block (ram,0x01f706dc) */
/* WARNING: Removing unreachable block (ram,0x01f706a3) */
/* WARNING: Removing unreachable block (ram,0x01f70800) */
/* [ported from ego_r via strfp] */

void __fastcall _SaveBinary_CRegion__QAEXAAVCDataOutputStream___Z(int param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 local_40 [15];
  char local_31;
  undefined1 local_30 [12];
  int local_24;
  int local_20;
  undefined4 local_1c;
  int local_18;
  int local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_03ccb060;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_14 = _size___vector_JV__allocator_J_std___std__QBEIXZ();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_14);
  local_18 = _size___vector_JV__allocator_J_std___std__QBEIXZ();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_18);
  for (local_20 = 0; local_20 < local_14; local_20 = local_20 + 1) {
    puVar1 = (undefined4 *)__A__vector_JV__allocator_J_std___std__QAEAAJI_Z(local_20);
    _WriteSLONG_CDataOutputStream__QAEXJ_Z(*puVar1);
  }
  for (local_24 = 0; local_24 < local_18; local_24 = local_24 + 1) {
    puVar1 = (undefined4 *)__A__vector_JV__allocator_J_std___std__QAEAAJI_Z(local_24);
    _WriteSLONG_CDataOutputStream__QAEXJ_Z(*puVar1);
  }
  _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(param_1 + 0x28);
  _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(param_1 + 0x30);
  _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(param_1 + 0x38);
  _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(param_1 + 0x44);
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)(param_1 + 0x7c));
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)(param_1 + 0x7d));
  _WriteEBOOL_CDataOutputStream__QAEX_N_Z(*(undefined1 *)(param_1 + 0x7e));
  _WriteFloat_CDataOutputStream__QAEXM_Z(*(undefined4 *)(param_1 + 0x4c));
  uVar2 = __ftol2_sse();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(uVar2);
  uVar2 = __ftol2_sse();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(uVar2);
  uVar2 = __ftol2_sse();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(uVar2);
  uVar2 = __ftol2_sse();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(uVar2);
  local_1c = _size____Tree_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__QBEIXZ
                       ();
  _WriteSLONG_CDataOutputStream__QAEXJ_Z(local_1c);
  _begin____Tree_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___2_XZ
            (local_30);
  local_8 = 0;
  while( true ) {
    uVar2 = _end____Tree_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std__QAE_AV___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___2_XZ
                      (local_40);
    local_8._0_1_ = 1;
    local_31 = __9___Tree_const_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QBE_NABV01__Z
                         (uVar2);
    local_8 = (uint)local_8._1_3_ << 8;
    __1___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QAE_XZ
              ();
    if (local_31 == '\0') break;
    uVar2 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QBEPAU__pair___CBVCCharString__VC2DVector___1_XZ
                      ();
    _WritePresizedString_CDataOutputStream__QAEXABVCCharString___Z(uVar2);
    iVar3 = __C___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QBEPAU__pair___CBVCCharString__VC2DVector___1_XZ
                      ();
    _Write2DVector_CDataOutputStream__QAEXABVC2DVector___Z(iVar3 + 8);
    __E___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QAEAAV01_XZ
              ();
  }
  local_8 = 0xffffffff;
  __1___Tree_iterator_V___Tree_val_V___Tmap_traits_VCCharString__VC2DVector__U__less_VCCharString___std__V__allocator_U__pair___CBVCCharString__VC2DVector___std___4__0A__std___std___std__QAE_XZ
            ();
  ExceptionList = local_10;
  return;
}

 
==================== ?AddLevelFromFile@CWorldMap@@IAEJABVC2DCoordI@@ABVCCharString@@@Z @ 01c81740 ==================== 

/* WARNING: Removing unreachable block (ram,0x01c8175b) */
/* [ported from ego_r via strfp] */

uint __thiscall
_AddLevelFromFile_CWorldMap__IAEJABVC2DCoordI__ABVCCharString___Z
          (undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined1 local_20 [16];
  uint local_10;
  undefined4 *local_c;
  uint local_8;
  
  local_c = (undefined4 *)0x0;
  local_8 = 0;
  local_10 = 1;
  do {
    uVar1 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ(param_1);
    if (uVar1 <= local_10) {
LAB_01c817b2:
      if (local_c == (undefined4 *)0x0) {
        iVar2 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ(param_1);
        _resize___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QAEXI_Z(iVar2 + 1);
        iVar2 = _size___vector_VCMapInfo__V__allocator_VCMapInfo___std___std__QBEIXZ();
        local_8 = iVar2 - 1;
        local_c = (undefined4 *)__A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_8);
      }
      *(undefined1 *)(local_c + 0xb) = 1;
      __4CCharString__QAEABV0_ABV0__Z(param_3);
      *(undefined1 *)((int)local_c + 0x2d) = 0;
      puVar3 = (undefined4 *)
               _GetLevelDimensions_CWorldMap__QBE_AVC2DBoxI__ABVCCharString___Z(local_20,param_3);
      *local_c = *puVar3;
      local_c[1] = puVar3[1];
      local_c[2] = puVar3[2];
      local_c[3] = puVar3[3];
      _Move_C2DBoxI__QAEXABVC2DCoordI___Z(param_2);
      uVar4 = _AllocateMapUID_CWorldMap__IAEKJ_Z(local_8);
      local_c[10] = uVar4;
      uVar1 = local_8;
      _Get___CTBaseSingleton_VCEngineManager____SIAAVCEngineManager__XZ(local_8);
      _AddMap_CEngineManager__QAEXJ_Z(uVar1);
      _LoadLoadableGlobalThingsForLevel_CWorldMap__IAEXJ_Z(local_8);
      return local_8;
    }
    iVar2 = __A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_10);
    if (*(char *)(iVar2 + 0x2c) == '\0') {
      local_8 = local_10;
      local_c = (undefined4 *)__A__CArray_VCMapInfo____QAEAAVCMapInfo__I_Z(local_10);
      goto LAB_01c817b2;
    }
    local_10 = local_10 + 1;
  } while( true );
}

 